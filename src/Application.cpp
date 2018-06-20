/*
 * Application.cpp
 *
 *  Created on: Jun 17, 2018
 *      Author: xgallom
 */

#include "Application.h"
#include "stm32f4xx.h"

Application *Application::m_instance = nullptr;

Application::Application() :
	appRunningLed(Periph::Leds::Blue),
	usart2(Periph::Usarts::Usart2, 9600)
{
	m_instance = this;
}

void Application::run()
{
	appRunningLed.turnOn();

	usart2.write("Application started running.\n");

	for(;;) {
		while(usart2.bytesAvailable()) {
			usart2.write(usart2.read());
		}

		for(int n = 0; n < 1000000; n++)
		{}
	}
}

Application *Application::instance()
{
	return m_instance;
}

