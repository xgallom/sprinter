/*
 * Application.cpp
 *
 *  Created on: Jun 17, 2018
 *      Author: xgallom
 */

#include "Application.h"
#include "stm32f4xx.h"

Application::Application() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef init = {
		GPIO_Pin: GPIO_Pin_7,
		GPIO_Mode: GPIO_Mode_OUT,
		GPIO_Speed: GPIO_Medium_Speed,
		GPIO_OType: GPIO_OType_PP,
		GPIO_PuPd: GPIO_PuPd_NOPULL
	};
	GPIO_Init(GPIOB, &init);

	GPIO_SetBits(GPIOB, GPIO_Pin_7);

	for(;;);
}

Application::~Application() {
	GPIO_ResetBits(GPIOB, GPIO_Pin_7);
}

