//
// Created by xgallom on 10/2/19.
//

#include <core/log.h>
#include <cstring>
#include <stm32f4xx_hal.h>

extern "C" { extern UART_HandleTypeDef huart1; }

namespace core {
	void log(const char *message) { log(message, strlen(message)); }

	void log(const char *message, size_t length)
	{
		HAL_UART_Transmit(&huart1, reinterpret_cast<uint8_t *>(const_cast<char *>(message)), length, 1000);
	}
}
