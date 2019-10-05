//
// Created by xgallom on 10/5/19.
//

#include <core/time.h>
#include <main.h>

namespace core {
	Time Time::Now()
	{
		return {HAL_GetTick()};
	}
}
