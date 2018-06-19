/*
 * Application.cpp
 *
 *  Created on: Jun 17, 2018
 *      Author: xgallom
 */

#include "Application.h"
#include "stm32f4xx.h"

Application::Application() :
	m_insance(this),
	appRunningLed(Periph::Leds::Blue),
	usart2(Periph::Usarts::Usart2, 9600)
{}

void Application::run()
{
	appRunningLed.turnOn();

	for(;;) {
		if(usart2.bytesAvailable()) {
			usart2.write(usart2.read());
		}
	}
}

static Application *Application::instance()
{
	return m_insance;
}

