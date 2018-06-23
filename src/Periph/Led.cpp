/*
 * Led.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: xgallom
 */

#include "Periph/Led.h"

namespace Periph {

Led::Led(Leds::Enum id) :
		id(id)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef init = {
		GPIO_Pin: id,
		GPIO_Mode: GPIO_Mode_OUT,
		GPIO_Speed: GPIO_Medium_Speed,
		GPIO_OType: GPIO_OType_PP,
		GPIO_PuPd: GPIO_PuPd_NOPULL
	};

	GPIO_Init(GPIOB, &init);
}

Led::~Led() {
	turnOff();
}


void Led::turnOn()
{
	GPIO_SetBits(GPIOB, id);
}

void Led::turnOff()
{
	GPIO_ResetBits(GPIOB, id);
}

void Led::toggle()
{
	GPIO_ToggleBits(GPIOB, id);
}

} /* namespace periph */
