/*
 * Pwm.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: Zahorack
 */


#include "Periph/Pwm.h"

static constexpr uint8_t  pwm_steps = 100;
static constexpr uint32_t pulse_1ms = 180000;
static constexpr uint16_t  servo_pwm_frequency = 50;

namespace Periph {

Util::RefCounter Pwm::s_refCounter;

namespace Timers{
enum Enum : uint8_t{
	Timer1 = 0,
	Timer2,
	Timer3,

	Size
};
} /* namespace Timers */

struct {
		GPIO_TypeDef 	*gpio;
		TIM_TypeDef     *tim;
		uint32_t        p1, p2, p3, p4;
		uint8_t p1Source, p2Source, p3Source, p4Source, gpioAf;
} constexpr config[Timers::Size] = {
		/*Timer1*/ {					//16bit
			gpio: GPIOC,
			tim: TIM8,
			p1: GPIO_Pin_6,
			p2: GPIO_Pin_7,
			p3: GPIO_Pin_8,
			p4: GPIO_Pin_9,
			p1Source: GPIO_PinSource6,
			p2Source: GPIO_PinSource7,
			p3Source: GPIO_PinSource8,
			p4Source: GPIO_PinSource9,
			gpioAf: GPIO_AF_TIM8
		}, /*TImer2*/ {					//16bit
			gpio: GPIOD,
			tim: TIM4,
			p1: GPIO_Pin_12,
			p2: GPIO_Pin_13,
			p3: GPIO_Pin_14,
			p4: GPIO_Pin_15,
			p1Source: GPIO_PinSource12,
			p2Source: GPIO_PinSource13,
			p3Source: GPIO_PinSource14,
			p4Source: GPIO_PinSource14,
			gpioAf: GPIO_AF_TIM4
		}, /*TImer3*/ {					//32bit
			gpio: GPIOA,
			tim: TIM2,
			p1: GPIO_Pin_0,
			p2: GPIO_Pin_1,
			p3: GPIO_Pin_2,
			p4: GPIO_Pin_3,
			p1Source: GPIO_PinSource0,
			p2Source: GPIO_PinSource1,
			p3Source: GPIO_PinSource2,
			p4Source: GPIO_PinSource3,
			gpioAf: GPIO_AF_TIM2
		}
};

/* PWM output pin */
static volatile uint32_t *OutChnl[Periph::Pwms::Size] = {

		&(config[Timers::Timer1].tim->CCR4), // PC9
		&(config[Timers::Timer1].tim->CCR3), // PC8
		&(config[Timers::Timer1].tim->CCR2), // PC7
		&(config[Timers::Timer1].tim->CCR1), // PC6

		&(config[Timers::Timer2].tim->CCR1), // PD12
		&(config[Timers::Timer2].tim->CCR2), // PD13
		&(config[Timers::Timer2].tim->CCR3), // PD14
		&(config[Timers::Timer2].tim->CCR4), // PD15

		&(config[Timers::Timer3].tim->CCR1), // PA0
		&(config[Timers::Timer3].tim->CCR2), // PA1
		&(config[Timers::Timer3].tim->CCR3), // PA2
		&(config[Timers::Timer3].tim->CCR4), // PA3

};

static uint16_t s_pwmSpeed[Periph::Pwms::Size] = {};

void Pwm::initRCC()
{
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
}

void Pwm::initGpio()
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 GPIO_StructInit(&GPIO_InitStructure);

	 GPIO_InitStructure.GPIO_Pin = config[Timers::Timer3].p1 | config[Timers::Timer3].p2 |
			 config[Timers::Timer3].p3 | config[Timers::Timer3].p4;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	 GPIO_Init(config[Timers::Timer3].gpio, &GPIO_InitStructure);

	 GPIO_InitStructure.GPIO_Pin = config[Timers::Timer2].p1 | config[Timers::Timer2].p2 |
			 config[Timers::Timer2].p3 | config[Timers::Timer2].p4;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	 GPIO_Init(config[Timers::Timer2].gpio, &GPIO_InitStructure);


	 GPIO_InitStructure.GPIO_Pin = config[Timers::Timer1].p1 | config[Timers::Timer1].p2 |
			 config[Timers::Timer1].p3 | config[Timers::Timer1].p4;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	 GPIO_Init(config[Timers::Timer1].gpio, &GPIO_InitStructure);


	 /* set alternative function */

	 GPIO_PinAFConfig(config[Timers::Timer1].gpio, config[Timers::Timer1].p1Source, config[Timers::Timer1].gpioAf);
	 GPIO_PinAFConfig(config[Timers::Timer1].gpio, config[Timers::Timer1].p2Source, config[Timers::Timer1].gpioAf);
	 GPIO_PinAFConfig(config[Timers::Timer1].gpio, config[Timers::Timer1].p3Source, config[Timers::Timer1].gpioAf);
	 GPIO_PinAFConfig(config[Timers::Timer1].gpio, config[Timers::Timer1].p4Source, config[Timers::Timer1].gpioAf);

	 GPIO_PinAFConfig(config[Timers::Timer2].gpio, config[Timers::Timer2].p1Source, config[Timers::Timer2].gpioAf);
	 GPIO_PinAFConfig(config[Timers::Timer2].gpio, config[Timers::Timer2].p2Source, config[Timers::Timer2].gpioAf);
	 GPIO_PinAFConfig(config[Timers::Timer2].gpio, config[Timers::Timer2].p3Source, config[Timers::Timer2].gpioAf);
	 GPIO_PinAFConfig(config[Timers::Timer2].gpio, config[Timers::Timer2].p4Source, config[Timers::Timer2].gpioAf);

	 GPIO_PinAFConfig(config[Timers::Timer3].gpio, config[Timers::Timer3].p1Source, config[Timers::Timer3].gpioAf);
	 GPIO_PinAFConfig(config[Timers::Timer3].gpio, config[Timers::Timer3].p2Source, config[Timers::Timer3].gpioAf);
	 GPIO_PinAFConfig(config[Timers::Timer3].gpio, config[Timers::Timer3].p3Source, config[Timers::Timer3].gpioAf);
	 GPIO_PinAFConfig(config[Timers::Timer3].gpio, config[Timers::Timer3].p4Source, config[Timers::Timer3].gpioAf);
}


void Pwm::initTimOC(uint32_t frequency)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitTypeDef servo;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCStructInit(&servo);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = SystemCoreClock / frequency;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;

	servo.TIM_OCMode = TIM_OCMode_PWM2;
	servo.TIM_OutputState = TIM_OutputState_Enable;
	servo.TIM_Pulse = SystemCoreClock / servo_pwm_frequency;
	servo.TIM_OCPolarity = TIM_OCPolarity_Low;
	servo.TIM_OCIdleState = TIM_OCIdleState_Set;

	TIM_OC1Init(config[Timers::Timer1].tim, &TIM_OCInitStructure);
	TIM_OC2Init(config[Timers::Timer1].tim, &TIM_OCInitStructure);
	TIM_OC3Init(config[Timers::Timer1].tim, &TIM_OCInitStructure);
	TIM_OC4Init(config[Timers::Timer1].tim, &TIM_OCInitStructure);

	TIM_OC1Init(config[Timers::Timer2].tim, &TIM_OCInitStructure);
	TIM_OC2Init(config[Timers::Timer2].tim, &TIM_OCInitStructure);
	TIM_OC3Init(config[Timers::Timer2].tim, &TIM_OCInitStructure);
	TIM_OC4Init(config[Timers::Timer2].tim, &TIM_OCInitStructure);

	TIM_OC1Init(config[Timers::Timer3].tim, &servo);
	TIM_OC2Init(config[Timers::Timer3].tim, &servo);
	TIM_OC3Init(config[Timers::Timer3].tim, &servo);
	TIM_OC4Init(config[Timers::Timer3].tim, &servo);
}


void Pwm::initTimTB(uint32_t frequency)
{
	TIM_TimeBaseInitTypeDef engine1;
	TIM_TimeBaseInitTypeDef engine2;
	TIM_TimeBaseInitTypeDef servo;
	TIM_TimeBaseStructInit(&engine1);
	TIM_TimeBaseStructInit(&engine2);
	TIM_TimeBaseStructInit(&servo);

	const uint32_t timer_frequency = SystemCoreClock;
	const uint32_t counter_frequency = pwm_steps * frequency;
	const uint32_t PSC_Value = (timer_frequency / counter_frequency) - 1;
	const uint16_t ARR_Value = pwm_steps - 1;

	engine1.TIM_Period = ARR_Value;
	engine1.TIM_Prescaler = PSC_Value;
	engine1.TIM_ClockDivision = 0;
	engine1.TIM_CounterMode = TIM_CounterMode_Up;

	engine2.TIM_Period = ARR_Value;
	engine2.TIM_Prescaler = PSC_Value/2;
	engine2.TIM_ClockDivision = 0;
	engine2.TIM_CounterMode = TIM_CounterMode_Up;


	/* Servo Motor time base configuration
	 * Recommended frequency : 50 Hz
	 * neutral position 1500 us
	 * -90deg  position	1000 us
	 * +90deg  position	2000 us
	 */
	const uint32_t servo_pwm_steps	   = 20000;			//20 000 us full period
	const uint32_t servo_counter_frequency = servo_pwm_steps * servo_pwm_frequency;
	const uint32_t servo_PSC_Value = (SystemCoreClock / servo_counter_frequency) - 1;
	const uint16_t servo_ARR_Value = servo_pwm_steps - 1;

	servo.TIM_Period = servo_ARR_Value;
	servo.TIM_Prescaler = servo_PSC_Value /2;
	servo.TIM_ClockDivision = 0;
	servo.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(config[Timers::Timer1].tim, &engine1);
	TIM_TimeBaseInit(config[Timers::Timer2].tim, &engine2);
	TIM_TimeBaseInit(config[Timers::Timer3].tim, &servo);

}

void Pwm::initPwm(){

	TIM_Cmd(config[Timers::Timer1].tim, ENABLE);
	TIM_Cmd(config[Timers::Timer2].tim, ENABLE);
	TIM_Cmd(config[Timers::Timer3].tim, ENABLE);

	TIM_CtrlPWMOutputs(config[Timers::Timer1].tim, ENABLE);
	TIM_CtrlPWMOutputs(config[Timers::Timer2].tim, ENABLE);
	TIM_CtrlPWMOutputs(config[Timers::Timer3].tim, ENABLE);
}

void Pwm::deinitPwm()
{
	TIM_Cmd(config[Timers::Timer1].tim, DISABLE);
	TIM_Cmd(config[Timers::Timer2].tim, DISABLE);
	TIM_Cmd(config[Timers::Timer3].tim, DISABLE);

	TIM_CtrlPWMOutputs(config[Timers::Timer1].tim, DISABLE);
	TIM_CtrlPWMOutputs(config[Timers::Timer2].tim, DISABLE);
	TIM_CtrlPWMOutputs(config[Timers::Timer3].tim, DISABLE);
}

void Pwm::deintServoPwm()
{
	TIM_Cmd(config[Timers::Timer3].tim, DISABLE);
	TIM_CtrlPWMOutputs(config[Timers::Timer3].tim, DISABLE);
}

void Pwm::intServoPwm()
{
	TIM_Cmd(config[Timers::Timer3].tim, ENABLE);
	TIM_CtrlPWMOutputs(config[Timers::Timer3].tim, ENABLE);
}

Pwm::Pwm(uint32_t frequency)
{
	if(!s_refCounter.referenceExists()) {
		initRCC();
		initGpio();
		initTimTB(frequency);
		initTimOC(frequency);
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
	 *OutChnl[id] = s_pwmSpeed[id] = value;
}

uint16_t Pwm::read(Pwms::Enum id) const
{
	return s_pwmSpeed[id];
}

} /* namespace Periph */
