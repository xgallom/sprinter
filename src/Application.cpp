/*
 * Application.cpp
 *
 *  Created on: Jun 17, 2018
 *      Author: xgallom
 */

#include "Application.h"
#include "stm32f4xx.h"

LOGGER_MODULE(Application)

Application *Application::m_instance = nullptr;

Application::ApplicationInitializator::ApplicationInitializator(Application *parent)
{
	m_instance = parent;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
}

Application::Application() :
	m_applicationInitializator(this),
	logger(&usartLog),
	usartLog(Periph::Usarts::Usart2, 9600),
	m_appRunningLed(Periph::Leds::Blue)
{}

void Application::run()
{
	INF_LOG("Application started running.");

	m_appRunningLed.turnOn();

	for(;;) {
		while(usartLog.bytesAvailable()) {
			usartLog.write(usartLog.read());
		}

		for(int n = 0; n < 1000000; n++)
		{}
	}

	INF_LOG("Application ended.");
}

Application *Application::instance()
{
	return m_instance;
}

