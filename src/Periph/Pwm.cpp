/*
 * Pwm.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: Zahorack
 */


#include "Periph/Pwm.h"

namespace Periph
{
	static constexpr uint32_t
		PwmFrequency = 10000;
	static constexpr uint16_t
		PwmSteps = 100,
		ServoPwmSteps = 20000,
		ServoPwmFrequency = 50;

	Util::RefCounter Pwm::s_refCounter = Util::RefCounter();

	namespace Timers
	{
		enum Enum : uint8_t {
			Timer1 = 0,
			Timer2,
			Timer3,

			Size
		};
	}

	struct {
			GPIO_TypeDef 	*gpio;
			TIM_TypeDef     *tim;
			uint32_t        p1, p2, p3, p4;
			uint8_t p1Source, p2Source, p3Source, p4Source;
			uint8_t gpioAf;
	} static constexpr Config[Timers::Size] = {
			{
				.gpio = GPIOC,
				.tim = TIM8,
				.p1 = GPIO_Pin_6,
				.p2 = GPIO_Pin_7,
				.p3 = GPIO_Pin_8,
				.p4 = GPIO_Pin_9,
				.p1Source = GPIO_PinSource6,
				.p2Source = GPIO_PinSource7,
				.p3Source = GPIO_PinSource8,
				.p4Source = GPIO_PinSource9,
				.gpioAf = GPIO_AF_TIM8
			},
			{
				.gpio = GPIOD,
				.tim = TIM4,
				.p1 = GPIO_Pin_12,
				.p2 = GPIO_Pin_13,
				.p3 = GPIO_Pin_14,
				.p4 = GPIO_Pin_15,
				.p1Source = GPIO_PinSource12,
				.p2Source = GPIO_PinSource13,
				.p3Source = GPIO_PinSource14,
				.p4Source = GPIO_PinSource14,
				.gpioAf = GPIO_AF_TIM4
			},
			{
				.gpio = GPIOA,
				.tim = TIM2,
				.p1 = GPIO_Pin_0,
				.p2 = GPIO_Pin_1,
				.p3 = GPIO_Pin_2,
				.p4 = GPIO_Pin_3,
				.p1Source = GPIO_PinSource0,
				.p2Source = GPIO_PinSource1,
				.p3Source = GPIO_PinSource2,
				.p4Source = GPIO_PinSource3,
				.gpioAf = GPIO_AF_TIM2
			}
	};

	static volatile uint32_t *s_outChnl[Pwms::Size] = {
			&(Config[Timers::Timer1].tim->CCR4), // PC9
			&(Config[Timers::Timer1].tim->CCR3), // PC8
			&(Config[Timers::Timer1].tim->CCR2), // PC7
			&(Config[Timers::Timer1].tim->CCR1), // PC6

			&(Config[Timers::Timer2].tim->CCR1), // PD12
			&(Config[Timers::Timer2].tim->CCR2), // PD13
			&(Config[Timers::Timer2].tim->CCR3), // PD14
			&(Config[Timers::Timer2].tim->CCR4), // PD15

			&(Config[Timers::Timer3].tim->CCR1), // PA0
			&(Config[Timers::Timer3].tim->CCR2), // PA1
			&(Config[Timers::Timer3].tim->CCR3), // PA2
			&(Config[Timers::Timer3].tim->CCR4), // PA3
	};

	void initRCC()
	{
		 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

		 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	}

	static void initSingleGpio(Timers::Enum timer)
	{
		GPIO_InitTypeDef init = {
			.GPIO_Pin = Config[timer].p1 | Config[timer].p2 | Config[timer].p3 | Config[timer].p4,
			.GPIO_Mode = GPIO_Mode_AF,
			.GPIO_Speed = GPIO_Speed_50MHz,
			.GPIO_OType = GPIO_OType_PP,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		};

		GPIO_Init(Config[timer].gpio, &init);

		GPIO_PinAFConfig(Config[timer].gpio, Config[timer].p1Source, Config[timer].gpioAf);
		GPIO_PinAFConfig(Config[timer].gpio, Config[timer].p2Source, Config[timer].gpioAf);
		GPIO_PinAFConfig(Config[timer].gpio, Config[timer].p3Source, Config[timer].gpioAf);
		GPIO_PinAFConfig(Config[timer].gpio, Config[timer].p4Source, Config[timer].gpioAf);
	}

	static void initGpio()
	{
		initSingleGpio(Timers::Timer1);
		initSingleGpio(Timers::Timer2);
		initSingleGpio(Timers::Timer3);
	}

	static void initSingleTimerOC(Timers::Enum timer, uint32_t frequency)
	{
		static TIM_OCInitTypeDef init = {
				.TIM_OCMode = TIM_OCMode_PWM2,
				.TIM_OutputState = TIM_OutputState_Enable,
				.TIM_OutputNState = TIM_OutputNState_Disable,
				.TIM_Pulse = 0,
				.TIM_OCPolarity = TIM_OCPolarity_Low,
				.TIM_OCNPolarity = TIM_OCPolarity_High,
				.TIM_OCIdleState = TIM_OCIdleState_Set,
				.TIM_OCNIdleState = TIM_OCNIdleState_Reset
		};

		init.TIM_Pulse = SystemCoreClock / frequency,

		TIM_OC1Init(Config[timer].tim, &init);
		TIM_OC2Init(Config[timer].tim, &init);
		TIM_OC3Init(Config[timer].tim, &init);
		TIM_OC4Init(Config[timer].tim, &init);
	}

	static void initTimerOC()
	{
		initSingleTimerOC(Timers::Timer1, PwmFrequency);
		initSingleTimerOC(Timers::Timer2, PwmFrequency);
		initSingleTimerOC(Timers::Timer3, ServoPwmFrequency);
	}

	static void initSingleTimerTB(Timers::Enum timer, uint32_t period, uint32_t prescaler)
	{
		static TIM_TimeBaseInitTypeDef init = {
			.TIM_Prescaler = 0,
			.TIM_CounterMode = TIM_CounterMode_Up,
			.TIM_Period = 0,
			.TIM_ClockDivision = TIM_CKD_DIV1,
			.TIM_RepetitionCounter = 0
		};

		init.TIM_Period = period;
		init.TIM_Prescaler = prescaler;

		TIM_TimeBaseInit(Config[timer].tim, &init);
	}

	static void initTimerTB()
	{
		const uint32_t
			period         = PwmSteps       - 1,
			servoPeriod    = ServoPwmSteps - 1,
			prescaler      = (SystemCoreClock / (PwmSteps      * PwmFrequency       )) - 1,
			servoPrescaler = (SystemCoreClock / (ServoPwmSteps * ServoPwmFrequency)) - 1;

		initSingleTimerTB(Timers::Timer1, period, prescaler);
		initSingleTimerTB(Timers::Timer2, period, prescaler / 2);
		initSingleTimerTB(Timers::Timer3, servoPeriod, servoPrescaler / 2);

		/* Servo Motor time base configuration
		 * Recommended frequency : 50 Hz
		 * neutral position 1500 us
		 * -90deg  position	1000 us
		 * +90deg  position	2000 us
		 */
	}

	static void initPwm()
	{
		TIM_Cmd(Config[Timers::Timer1].tim, ENABLE);
		TIM_Cmd(Config[Timers::Timer2].tim, ENABLE);
		TIM_Cmd(Config[Timers::Timer3].tim, ENABLE);

		TIM_CtrlPWMOutputs(Config[Timers::Timer1].tim, ENABLE);
		TIM_CtrlPWMOutputs(Config[Timers::Timer2].tim, ENABLE);
		TIM_CtrlPWMOutputs(Config[Timers::Timer3].tim, ENABLE);
	}

	static void deinitPwm()
	{
		TIM_Cmd(Config[Timers::Timer1].tim, DISABLE);
		TIM_Cmd(Config[Timers::Timer2].tim, DISABLE);
		TIM_Cmd(Config[Timers::Timer3].tim, DISABLE);

		TIM_CtrlPWMOutputs(Config[Timers::Timer1].tim, DISABLE);
		TIM_CtrlPWMOutputs(Config[Timers::Timer2].tim, DISABLE);
		TIM_CtrlPWMOutputs(Config[Timers::Timer3].tim, DISABLE);
	}

	Pwm::Pwm()
	{
		if(!s_refCounter.referenceExists()) {
			initRCC();
			initGpio();
			initTimerTB();
			initTimerOC();
			initPwm();
		}

		++s_refCounter;
	}

	Pwm::~Pwm()
	{
		--s_refCounter;

		if(s_refCounter.destroyReference())
			deinitPwm();
	}

	void Pwm::write(Pwms::Enum id, uint16_t value)
	{
		 *s_outChnl[id] = value;
	}

	uint16_t Pwm::read(Pwms::Enum id) const
	{
		return *s_outChnl[id];
	}
}
