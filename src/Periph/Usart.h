/*
 * Usart.h
 *
 *  Created on: Jun 18, 2018
 *      Author: xgallom
 */

#ifndef PERIPH_USART_H_
#define PERIPH_USART_H_

#include "stm32f4xx.h"

namespace Periph {

namespace Usarts {
enum Enum : uint8_t {
	Usart1 = 0,
	Usart2,
	/*
	Usart3,
	Uart4,
	Uart5,
	Usart6,
	*/

	Size
};
}

class Usart {
	const Usarts::Enum id;

	void initRcc();
	void initGpio();
	void initUsart(uint32_t baudRate);
	void initNvic();

public:
	Usart(Usarts::Enum id, uint32_t baudRate);
	~Usart();

	bool write(const uint8_t c);
	bool write(const char c[]);
	bool write(const uint8_t *c, uint32_t size);
	uint8_t read();

	bool bytesAvailable() const;
};

} /* namespace Periph */

extern "C" {
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
}

#endif /* PERIPH_USART_H_ */
