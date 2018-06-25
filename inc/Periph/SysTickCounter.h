/*
 * SysTick.h
 *
 *  Created on: Jun 25, 2018
 *      Author: xgallom
 */

#ifndef PERIPH_SYSTICKCOUNTER_H_
#define PERIPH_SYSTICKCOUNTER_H_

#include "Util/Time.h"

namespace Periph {

class SysTickCounter {
	void init();
	void deinit();

public:
	SysTickCounter();

	micros_t microsSinceLastReset() const;
	Util::Time sinceLastReset() const;
};

} /* namespace Periph */

extern "C" {
void SysTick_Handler(void);
}

#endif /* PERIPH_SYSTICKCOUNTER_H_ */
