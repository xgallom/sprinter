/*
 * SysTick.h
 *
 *  Created on: Jun 25, 2018
 *      Author: xgallom
 */

#ifndef PERIPH_SYSTICKCOUNTER_H_
#define PERIPH_SYSTICKCOUNTER_H_

#include "stm32f4xx.h"

typedef uint64_t micros_t;

namespace Periph {

class SysTickCounter {
	void init();
	void deinit();

public:
	SysTickCounter();

	micros_t microsSinceLastReset() const;
};

} /* namespace Periph */

extern "C" {
void SysTick_Handler(void);
}

#endif /* PERIPH_SYSTICKCOUNTER_H_ */
