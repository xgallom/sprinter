/*
 * Usart.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: xgallom
 */

#include "Usart.h"
#include "../Container/Queue.h"
#include "../Util/State.h"

namespace Periph {
static Container::Queue<volatile uint8_t, 512> s_readQueues[Usarts::Size];
static Container::Queue<volatile uint8_t, 512> s_writeQueues[Usarts::Size];

namespace States {
enum Flags : uint8_t {
	Writing = 0x01
};
}
static Util::State<uint8_t> s_states;

struct {
	GPIO_TypeDef *gpio;
	uint32_t ahb1Gpio;
	uint32_t rx, tx;
	uint8_t rxSource, txSource, gpioAf;
	USART_TypeDef *usart;
	IRQn irqn;
} constexpr config[Usarts::Size] = {
		/* Usart1 */ {
				gpio: GPIOA,
				ahb1Gpio: RCC_AHB1Periph_GPIOA,
				rx: GPIO_Pin_9,
				tx: GPIO_Pin_10,
				rxSource: GPIO_PinSource9,
				txSource: GPIO_PinSource10,
				gpioAf: GPIO_AF_USART1,
				usart: USART1,
				irqn: USART1_IRQn
		},
		/* Usart2 */ {
				gpio: GPIOD,
				ahb1Gpio: RCC_AHB1Periph_GPIOD,
				rx: GPIO_Pin_6,
				tx: GPIO_Pin_5,
				rxSource: GPIO_PinSource6,
				txSource: GPIO_PinSource5,
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
		break;
	case Usarts::Usart2:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		break;
	}

	RCC_AHB1PeriphClockCmd(config[id].ahb1Gpio, ENABLE);
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

	USART_ITConfig(config[id].usart, USART_IT_TXE, ENABLE);
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

bool Usart::write(const uint8_t c)
{
	if(!s_writeQueues[id].enqueue(c))
		return false;

	if(!s_states.flag(States::Writing << 1)) {
		s_states.setFlag(States::Writing << 1);

		USART_SendData(config[id].usart, static_cast<uint16_t>(c));
	}

	return true;
}

uint8_t Usart::read()
{
	Container::OperationResult<volatile uint8_t> readResult;

	while(!(readResult = s_readQueues[id].dequeue()).isValid) {}

	return readResult.value;
}

bool Usart::bytesAvailable() const
{
	return s_readQueues[id].isEmpty();
}

} /* namespace Periph */

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		Periph::s_readQueues[0].enqueue(static_cast<uint8_t>(USART_ReceiveData(USART1)));
	}
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		Periph::s_readQueues[1].enqueue(static_cast<uint8_t>(USART_ReceiveData(USART2)));
	}

	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET) {
		Container::OperationResult<volatile uint8_t> writeData = Periph::s_writeQueues[1].dequeue();

		if(writeData.isValid) { // We have more data in the buffer
			USART_SendData(USART2, writeData.value);
		}
		else {
			Periph::s_states.resetFlag(Periph::States::Writing << 1);
		}
	}
}

