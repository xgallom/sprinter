/*
 * Logger.cpp
 *
 *  Created on: Jun 22, 2018
 *      Author: xgallom
 */

#include "Util/Logger.h"
#include "Application.h"

#include <cstdarg>
#include <cstdio>

namespace Util
{
	Logger::Logger(Periph::Usart *usart) :
		m_usart(usart)
	{}

	void Logger::write(const char module[], const char message[], ...)
	{
		char buffer[512] = {0};
		va_list arg;

		va_start(arg, message);
		vsnprintf(buffer, sizeof(buffer) - 1, message, arg);
		va_end(arg);

		m_usart->write(module);
		m_usart->write(buffer);
	}

	Logger *Logger::instance()
	{
		return App ? &App->logger : nullptr;
	}
}
