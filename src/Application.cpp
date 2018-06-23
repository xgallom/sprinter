/*
 * Application.cpp
 *
 *  Created on: Jun 17, 2018
 *      Author: xgallom
 */

#include "Application.h"
#include "Util/Logger.h"

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
		uint8_t input[256];

		uint32_t inputSize = usartLog.readLine(input, 256);

		for(int n = 0; n < 10000000; n++)
		{}

		usartLog.write(input, inputSize);
	}

	INF_LOG("Application ended.");
}

Application *Application::instance()
{
	return m_instance;
}

