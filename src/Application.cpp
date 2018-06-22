/*
 * Application.cpp
 *
 *  Created on: Jun 17, 2018
 *      Author: xgallom
 */

#include "Application.h"
#include "stm32f4xx.h"

Application *Application::m_instance = nullptr;

Application::ApplicationInitializator::ApplicationInitializator(Application *parent)
{
	m_instance = parent;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
}

Application::Application() :
	applicationInitializator(this),
	appRunningLed(Periph::Leds::Blue),
	usart2(Periph::Usarts::Usart2, 9600)
{}

void Application::run()
{
	appRunningLed.turnOn();

	usart2.write("Application::run()\n");

	for(;;) {
		uint8_t input[256];

		uint32_t inputSize = usart2.readLine(input, 256);

		for(int n = 0; n < 10000000; n++)
		{}

		usart2.write(input, inputSize);
	}
}

Application *Application::instance()
{
	return m_instance;
}

