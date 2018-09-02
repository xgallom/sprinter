/*
 * Logger.cpp
 *
 *  Created on: Jun 22, 2018
 *      Author: xgallom
 */

#include "Util/Logger.h"
#include "Application.h"


namespace Util {

Logger::Logger(Periph::Usart *usart) :
	m_usart(usart)
{}

void Logger::write(const char module[], const char message[])
{
	m_usart->write(module);
	m_usart->write(message);
}

Logger *Logger::instance()
{
	return App ? &App->logger : nullptr;
}


} /* namespace Util */




