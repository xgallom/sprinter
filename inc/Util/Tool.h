/*
 * Tool.h
 *
 *  Created on: 23.9. 2018
 *      Author: Zahorack
 */

#ifndef UTIL_TOOL_H_
#define UTIL_TOOL_H_

#include "stm32f4xx.h"

namespace Util {

class Tool {



public:
	Tool();

	int32_t clamp(int32_t val, int32_t min, int32_t max);
	bool inRange( int32_t x, int32_t low, int32_t high);
	int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);
};

} /* namespace Util */

#endif /* UTIL_TOOL_H_ */
