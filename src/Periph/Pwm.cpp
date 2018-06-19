/*
 * Pwm.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: Zahorack
 */


#include "Pwm.h"

namespace Periph{

																			           //PWM output pin
volatile uint32_t *OutChnl[Periph::Pwms::Size] = { &(config[Timers::Timer1].tim->CCR4),			//	PC9
												&(config[Timers::Timer1].tim->CCR3),			//	PC8
												&(config[Timers::Timer1].tim->CCR2),			//	PC7
												&(config[Timers::Timer1].tim->CCR1),			//	PC6
												&(config[Timers::Timer2].tim->CCR1),			//	PA0
												&(config[Timers::Timer2].tim->CCR2),			//	PA1
												&(config[Timers::Timer2].tim->CCR3),			//	PA2
												&(config[Timers::Timer2].tim->CCR4), };		    //	PA3

struct {
		GPIO_TypeDef 	*gpio;
		TIM_TypeDef		*tim;
		uint32_t        p1, p2, p3, p4;
		uint8_t p1Source, p2Source, p3Source, p4Source, gpioAf;

	} constexpr config[Timers::Size] = {
		/*Timer1*/	{
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
		},
		/*TImer2*/{
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

void Pwm::initRCC()
{
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,  ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,  ENABLE);

	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

}

void Pwm::initGpio()
{
	 GPIO_InitTypeDef    GPIO_InitStructure;
	 GPIO_StructInit(&GPIO_InitStructure);


	 GPIO_InitStructure.GPIO_Pin   = config[Timers::Timer2].p1 | config[Timers::Timer2].p2 |
	 	 	   	   	   	   	   	   	 config[Timers::Timer2].p3 | config[Timers::Timer2].p4;
	 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	 GPIO_Init(config[Timers::Timer2].gpio, &GPIO_InitStructure);


	 GPIO_InitStructure.GPIO_Pin   = config[Timers::Timer1].p1 | config[Timers::Timer1].p2 |
	 	 	   	   	   	   	   	   	 config[Timers::Timer1].p3 | config[Timers::Timer1].p4;
	 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	 GPIO_Init(config[Timers::Timer1].gpio, &GPIO_InitStructure);


	 /* set alternative function */

	 GPIO_PinAFConfig(config[Timers::Timer1].gpio, config[Timers::Timer1].p1Source,  config[Timers::Timer1].gpioAf);
	 GPIO_PinAFConfig(config[Timers::Timer1].gpio, config[Timers::Timer1].p2Source,  config[Timers::Timer1].gpioAf);
	 GPIO_PinAFConfig(config[Timers::Timer1].gpio, config[Timers::Timer1].p3Source,  config[Timers::Timer1].gpioAf);
	 GPIO_PinAFConfig(config[Timers::Timer1].gpio, config[Timers::Timer1].p4Source,  config[Timers::Timer1].gpioAf);

	 GPIO_PinAFConfig(config[Timers::Timer2].gpio, config[Timers::Timer2].p1Source,  config[Timers::Timer2].gpioAf);
	 GPIO_PinAFConfig(config[Timers::Timer2].gpio, config[Timers::Timer2].p2Source,  config[Timers::Timer2].gpioAf);
	 GPIO_PinAFConfig(config[Timers::Timer2].gpio, config[Timers::Timer2].p3Source,  config[Timers::Timer2].gpioAf);
	 GPIO_PinAFConfig(config[Timers::Timer2].gpio, config[Timers::Timer2].p4Source,  config[Timers::Timer2].gpioAf);

}


void Pwm::initTimOC()
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse        = PULSE_1MS;
	TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set;

	TIM_OC1Init(config[Timers::Timer1].tim, &TIM_OCInitStructure);
	TIM_OC2Init(config[Timers::Timer1].tim, &TIM_OCInitStructure);
	TIM_OC3Init(config[Timers::Timer1].tim, &TIM_OCInitStructure);
	TIM_OC4Init(config[Timers::Timer1].tim, &TIM_OCInitStructure);

	TIM_OC1Init(config[Timers::Timer2].tim, &TIM_OCInitStructure);
	TIM_OC2Init(config[Timers::Timer2].tim, &TIM_OCInitStructure);
	TIM_OC3Init(config[Timers::Timer2].tim, &TIM_OCInitStructure);
	TIM_OC4Init(config[Timers::Timer2].tim, &TIM_OCInitStructure);


}


void Pwm::initTimTB(uint32_t frequency)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);


	uint32_t timer_frequency 	= SystemCoreClock;
	uint32_t counter_frequency 	= PWM_STEPS * frequency;
	uint32_t PSC_Value 			= (timer_frequency / counter_frequency) - 1;
	uint16_t ARR_Value 			= PWM_STEPS - 1;

	TIM_TimeBaseStructure.TIM_Period 		= ARR_Value;
	TIM_TimeBaseStructure.TIM_Prescaler 	= PSC_Value;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;

	TIM_TimeBaseInit(config[Timers::Timer1].tim, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(config[Timers::Timer2].tim, &TIM_TimeBaseStructure);

}


void Pwm::initPwm(){

	TIM_Cmd(config[Timers::Timer1].tim, ENABLE);
	TIM_Cmd(config[Timers::Timer2].tim, ENABLE);

	TIM_CtrlPWMOutputs(config[Timers::Timer1].tim, ENABLE);
	TIM_CtrlPWMOutputs(config[Timers::Timer2].tim, ENABLE);
}

void Pwm::deinitPwm()
{
	TIM_Cmd(config[Timers::Timer1].tim, DISABLE);
	TIM_Cmd(config[Timers::Timer2].tim, DISABLE);

	TIM_CtrlPWMOutputs(config[Timers::Timer1].tim, DISABLE);
	TIM_CtrlPWMOutputs(config[Timers::Timer2].tim, DISABLE);
}


Pwm::Pwm(uint32_t frequency)
{
	initRCC();
	initGpio();
	initTimTB(frequency);
	initTimOC();
	initPwm();
}

Pwm::~Pwm()
{
	deinitPwm();
}

void Pwm::write(Pwms::Enum id, uint8_t value)
{
	 *OutChnl[id] = value;
}

}/* namespace Periph */
