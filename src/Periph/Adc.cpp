/*
 * Adc.cpp
 *
 *  Created on: January 6, 2019
 *      Author: Zahorack
 */

//https://stackoverflow.com/questions/47488228/stm32-adc-interrupt

#include "Periph/Adc.h"
#include "Util/Trace.h"

namespace Periph {

struct {
	GPIO_TypeDef	*port;
	ADC_TypeDef  	*adc;
	uint16_t 	pin;
	uint8_t 	channel;
} static const config[AdcPins::Size] = {
		{ /* AdcPin1 */
				.port = GPIOC,
				.adc = ADC1,
				.pin = GPIO_Pin_0,
				.channel = ADC_Channel_10
		},
		{ /* AdcPin2 */
				.port = GPIOC,
				.adc = ADC1,
				.pin = GPIO_Pin_1,
				.channel = ADC_Channel_11
		},
		{ /* AdcPin3 */
				.port = GPIOC,
				.adc = ADC1,
				.pin = GPIO_Pin_2,
				.channel = ADC_Channel_12
		},
		{ /* AdcPin4 */
				.port = GPIOC,
				.adc = ADC1,
				.pin = GPIO_Pin_3,
				.channel = ADC_Channel_13
		},
		{ /* AdcPin5 */
				.port = GPIOC,
				.adc = ADC1,
				.pin = GPIO_Pin_4,
				.channel = ADC_Channel_14
		}
};

Adc::Adc(AdcPins::Enum a_id):
	id(a_id)
{
	initGpio();
	initRCC();
	initAdc();
}

void Adc::initGpio()
{
	GPIO_InitTypeDef init = {
		.GPIO_Pin = config[id].pin,
		.GPIO_Mode = GPIO_Mode_AN,
		.GPIO_Speed = GPIO_Fast_Speed,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_NOPULL,
	};

	GPIO_Init(config[id].port, &init);
}

void Adc::initRCC()
{
	uint32_t rccPort;
	uint32_t rccAdc;

	if(config[id].port == GPIOA)
		rccPort = RCC_AHB1Periph_GPIOA;
	else if(config[id].port == GPIOB)
		rccPort = RCC_AHB1Periph_GPIOB;
	else if(config[id].port == GPIOC)
		rccPort = RCC_AHB1Periph_GPIOC;
	else if(config[id].port == GPIOD)
		rccPort = RCC_AHB1Periph_GPIOD;
	else if(config[id].port == GPIOE)
		rccPort = RCC_AHB1Periph_GPIOE;
	else if(config[id].port == GPIOF)
		rccPort = RCC_AHB1Periph_GPIOF;
	else if(config[id].port == GPIOG)
		rccPort = RCC_AHB1Periph_GPIOG;
	else if(config[id].port == GPIOH)
		rccPort = RCC_AHB1Periph_GPIOH;
	else if(config[id].port == GPIOI)
		rccPort = RCC_AHB1Periph_GPIOI;
	else if(config[id].port == GPIOJ)
		rccPort = RCC_AHB1Periph_GPIOJ;
	else if(config[id].port == GPIOK)
		rccPort = RCC_AHB1Periph_GPIOK;

	RCC_AHB1PeriphClockCmd(rccPort, ENABLE);

	if(config[id].adc == ADC1) {
		rccAdc = RCC_APB2Periph_ADC1;
		TRACE("ADC clock OK\n\r");
	}
	else if(config[id].adc == ADC2)
		rccAdc = RCC_APB2Periph_ADC2;
	else if(config[id].adc == ADC3)
		rccAdc = RCC_APB2Periph_ADC3;

	RCC_APB2PeriphClockCmd(rccAdc, ENABLE);
}

void Adc::initAdc()
{
	ADC_InitTypeDef init = {
		.ADC_Resolution = ADC_Resolution_12b,
		.ADC_ScanConvMode = DISABLE,
		.ADC_ContinuousConvMode = ENABLE,
		.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None,
		.ADC_ExternalTrigConv = DISABLE,
		.ADC_DataAlign = ADC_DataAlign_Right,
		.ADC_NbrOfConversion = 1,
	};

	ADC_Init(config[id].adc, &init);
	ADC_Cmd(config[id].adc, ENABLE);

	//ADC_RegularChannelConfig(config[id].adc, config[id].channel, 1, ADC_SampleTime_84Cycles);
}

uint16_t Adc::read()
{
	ADC_RegularChannelConfig(config[id].adc, config[id].channel, 1, ADC_SampleTime_480Cycles);
	ADC_SoftwareStartConv(config[id].adc);

	while(ADC_GetFlagStatus(config[id].adc, ADC_FLAG_EOC) == RESET);

	ADC_ClearFlag(config[id].adc, ADC_FLAG_EOC);

	return ADC_GetConversionValue(config[id].adc);
}




}