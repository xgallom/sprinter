/*
 * Dir.cpp
 *
 *  Created on: Jun 22, 2018
 *      Author: Zahorack
 */

#include "Dir.h"

namespace Periph {

Util::RefCounter Dir::s_refCounter;

namespace config {
static GPIO_TypeDef *gpio = GPIOE;
static uint32_t ahb1Gpio = RCC_AHB1Periph_GPIOE;
static uint16_t OutPin[DirPins::Enum::Size ]={
		GPIO_Pin_7,
		GPIO_Pin_8,
		GPIO_Pin_9,
		GPIO_Pin_10,
		GPIO_Pin_11,
		GPIO_Pin_12
};
}


void Dir::initGpio()
{
	RCC_AHB1PeriphClockCmd(config::ahb1Gpio, ENABLE);

		GPIO_InitTypeDef init = {
			GPIO_Pin: config::OutPin[0] | config::OutPin[1] | config::OutPin[2] | config::OutPin[3] | config::OutPin[4] | config::OutPin[5],
			GPIO_Mode: GPIO_Mode_OUT,
			GPIO_Speed: GPIO_Medium_Speed,
			GPIO_OType: GPIO_OType_PP,
			GPIO_PuPd: GPIO_PuPd_NOPULL
		};

		GPIO_Init(config::gpio, &init);
}

void Dir::deinitGpio()
{
	RCC_AHB1PeriphClockCmd(config::ahb1Gpio, DISABLE);
}


Dir::Dir(DirPins::Enum id) :
		id(id)
{
	if(!s_refCounter.referenceExists()) {
			initGpio();
		}

		++s_refCounter;
}

Dir::~Dir()
{
	--s_refCounter;

		if(s_refCounter.destroyReference())
			deinitGpio();
}

void Dir::update(Dirs::Enum dir)
{
	if(dir) GPIO_SetBits(config::gpio, config::OutPin[id]);
	else 	GPIO_ResetBits(config::gpio, config::OutPin[id]);
}

} /* namespace Periph */
