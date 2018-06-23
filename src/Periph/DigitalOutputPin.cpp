/*
 * Dir.cpp
 *
 *  Created on: Jun 22, 2018
 *      Author: Zahorack
 */

#include <Periph/DigitalOutputPin.h>

namespace Periph {

void DigitalOutputPin::initGpio()
{
	uint32_t rccPort;

	if(port == GPIOA)
		rccPort = RCC_AHB1Periph_GPIOA;
	else if(port == GPIOB)
		rccPort = RCC_AHB1Periph_GPIOB;
	else if(port == GPIOC)
		rccPort = RCC_AHB1Periph_GPIOC;
	else if(port == GPIOD)
		rccPort = RCC_AHB1Periph_GPIOD;
	else if(port == GPIOE)
		rccPort = RCC_AHB1Periph_GPIOE;
	else if(port == GPIOF)
		rccPort = RCC_AHB1Periph_GPIOF;
	else if(port == GPIOG)
		rccPort = RCC_AHB1Periph_GPIOG;
	else if(port == GPIOH)
		rccPort = RCC_AHB1Periph_GPIOH;
	else if(port == GPIOI)
		rccPort = RCC_AHB1Periph_GPIOI;
	else if(port == GPIOJ)
		rccPort = RCC_AHB1Periph_GPIOJ;
	else if(port == GPIOK)
		rccPort = RCC_AHB1Periph_GPIOK;

	RCC_AHB1PeriphClockCmd(rccPort, ENABLE);

	GPIO_InitTypeDef init = {
		GPIO_Pin: static_cast<uint32_t>(id),
		GPIO_Mode: GPIO_Mode_OUT,
		GPIO_Speed: GPIO_Medium_Speed,
		GPIO_OType: GPIO_OType_PP,
		GPIO_PuPd: GPIO_PuPd_NOPULL
	};

	GPIO_Init(port, &init);
}

DigitalOutputPin::DigitalOutputPin(GPIO_TypeDef *a_port, uint16_t a_id):
	id(a_id),
	port(a_port)
{
	initGpio();
}

DigitalOutputPin::~DigitalOutputPin()
{}

void DigitalOutputPin::setPin()
{
	GPIO_SetBits(port, id);
}

void DigitalOutputPin::resetPin()
{
	GPIO_ResetBits(port, id);
}

void DigitalOutputPin::setPinTo(bool on)
{
	if(on)
		setPin();
	else
		resetPin();
}

void DigitalOutputPin::togglePin()
{
	GPIO_ToggleBits(port, id);
}

bool DigitalOutputPin::readPin() const
{
	return GPIO_ReadOutputDataBit(port, id) ? true : false;
}

void DigitalOutputPin::deinitGpio()
{
}

} /* namespace Periph */
