/*
 * Dir.h
 *
 *  Created on: Jun 22, 2018
 *      Author: Zahorack
 */

#ifndef PERIPH_DIGITALOUTPUTPIN_H_
#define PERIPH_DIGITALOUTPUTPIN_H_

#include "stm32f4xx.h"
#include "Util/RefCounter.h"

namespace Periph {

class DigitalOutputPin {
	static Util::RefCounter s_refCounter;
	const uint16_t id;
	GPIO_TypeDef * const port;

	void initGpio();
	void deinitGpio();

public:
	DigitalOutputPin(GPIO_TypeDef *port, uint16_t id);
	~DigitalOutputPin();

	void setPin();
	void resetPin();
	void setPinTo(bool on);
	void togglePin();

	bool readPin() const;
};

} /* namespace Periph */

#endif /* PERIPH_DIGITALOUTPUTPIN_H_ */
