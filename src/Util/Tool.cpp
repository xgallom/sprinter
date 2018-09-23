/*
* Tool.cpp
*
*  Created on: 23.9. 2018
*      Author: Zahorack
*/

#include "Util/Tool.h"


namespace Util {

Tool::Tool(){}


int32_t Tool::clamp(int32_t val, int32_t min, int32_t max) {
    return val < min ? min : (val > max ? max : val);
}

bool Tool::inRange( int32_t x, int32_t low, int32_t high)
{
    return  ((x <= high) && (x >= low));
}

int32_t Tool::map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

}
