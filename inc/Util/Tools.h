/*
 * Tool.h
 *
 *  Created on: 23.9. 2018
 *      Author: Zahorack
 */

#ifndef UTIL_TOOLS_H_
#define UTIL_TOOLS_H_

#include "stm32f4xx.h"

namespace Util
{
	template<typename T>
	constexpr T clamp(T val, T min, T max)
	{
		return val < min ? min : (val > max ? max : val);
	}

	template<typename T>
	constexpr bool inRange(T x, T low, T high)
	{
		return x <= high && x >= low;
	}

	template<typename T>
	constexpr T map(T x, T inMin, T inMax, T outMin, T outMax)
	{
		return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
	}
}

#endif /* UTIL_TOOLS_H_ */
