/*
 * SysTick.cpp
 *
 *  Created on: Jun 25, 2018
 *      Author: xgallom
 */

#include <Periph/SysTickCounter.h>

namespace Periph {

static volatile micros_t s_microsSinceLastReset;
static bool s_wasInitialized = false;

void SysTickCounter::init()
{
	while(SysTick_Config(SystemCoreClock / 1000000) != 0)
	{}
}

SysTickCounter::SysTickCounter()
{
	if(!s_wasInitialized) {
		init();
		s_microsSinceLastReset = 0;

		s_wasInitialized = true;
	}
}

micros_t SysTickCounter::microsSinceLastReset() const
{
	return s_microsSinceLastReset;
}

} /* namespace Periph */

void SysTick_Handler(void)
{
	++Periph::s_microsSinceLastReset;
}
