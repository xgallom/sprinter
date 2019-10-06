//
// Created by xgallom on 10/2/19.
//

#include <core/defines.h>
#include <core/log.h>
#include <cstring>
#include <stm32f4xx_hal.h>

extern "C" { extern UART_HandleTypeDef huart1; }

namespace logImpl {
	void log(const char *message)
	{
#ifndef BUILD_TYPE_RELEASE
		log(message, strlen(message));
#endif
	}

	void log(const char *message, size_t length)
	{
#ifndef BUILD_TYPE_RELEASE
		HAL_UART_Transmit(&huart1,
						  reinterpret_cast<uint8_t *>(const_cast<char *>(message)),
						  length,
						  1000);
#endif
	}
}
