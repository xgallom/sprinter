/*
 * Usart.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: xgallom
 */

#include "Usart.h"

static volatile uint8_t inputBuffers[Periph::Usarts::Size][512];

namespace Periph {

struct {
	GPIO_TypeDef *gpio;
	uint32_t rx, tx;
	uint8_t rxSource, txSource, gpioAf;
	USART_TypeDef *usart;
	IRQn irqn;
} constexpr config[Usarts::Size] = {
		/* Usart1 */ {
				gpio: GPIOA,
				rx: GPIO_Pin_9,
				tx: GPIO_Pin_10,
				rxSource: GPIO_PinSource9,
				txSource: GPIO_PinSource10,
				gpioAf: GPIO_AF_USART1,
				usart: USART1,
				irqn: USART1_IRQn
		},
		/* Usart2 */ {
				gpio: GPIOA,
				rx: GPIO_Pin_3,
				tx: GPIO_Pin_2,
				rxSource: GPIO_PinSource3,
				txSource: GPIO_PinSource2,
				gpioAf: GPIO_AF_USART2,
				usart: USART2,
				irqn: USART2_IRQn
		}
};

void Usart::initRcc()
{
	switch(id) {
	case Usarts::Usart1:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		break;
	case Usarts::Usart2:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		break;
	}
}

void Usart::initGpio()
{
	GPIO_InitTypeDef gpioInitStruct = {
			GPIO_Pin: config[id].rx | config[id].tx,
			GPIO_Mode: GPIO_Mode_AF,
			GPIO_Speed: GPIO_Fast_Speed,
			GPIO_OType: GPIO_OType_PP,
			GPIO_PuPd: GPIO_PuPd_NOPULL
	};

	GPIO_Init(config[id].gpio, &gpioInitStruct);

	GPIO_PinAFConfig(config[id].gpio, config[id].rxSource, config[id].gpioAf);
	GPIO_PinAFConfig(config[id].gpio, config[id].txSource, config[id].gpioAf);
}

void Usart::initUsart(uint32_t baudRate)
{
	// TODO: Make configurable
	USART_InitTypeDef usartInitStruct = {
			USART_BaudRate: baudRate,
			USART_WordLength: USART_WordLength_8b,
			USART_StopBits: USART_StopBits_1,
			USART_Parity: USART_Parity_No,
			USART_Mode: USART_Mode_Rx | USART_Mode_Tx,
			USART_HardwareFlowControl: USART_HardwareFlowControl_None
	};

	USART_Init(config[id].usart, &usartInitStruct);
	USART_Cmd(config[id].usart, ENABLE);

	USART_ITConfig(config[id].usart, USART_IT_RXNE, ENABLE);
}

void Usart::initNvic()
{
	NVIC_InitTypeDef nvicInitStruct = {
			NVIC_IRQChannel: static_cast<uint8_t>(config[id].irqn),
			NVIC_IRQChannelPreemptionPriority: 0,
			NVIC_IRQChannelSubPriority: 0,
			NVIC_IRQChannelCmd: ENABLE
	};

	NVIC_Init(&nvicInitStruct);
}

Usart::Usart(Usarts::Enum id, uint32_t baudRate) :
		id(id)
{
	initRcc();
	initGpio();
	initUsart(baudRate);
	initNvic();
}

Usart::~Usart()
{
	USART_Cmd(config[id].usart, DISABLE);
	NVIC_DisableIRQ(config[id].irqn);
}

} /* namespace Periph */

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		USART_SendData(USART1, USART_ReceiveData(USART1));
	}
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		USART_SendData(USART2, USART_ReceiveData(USART2));
	}
}

