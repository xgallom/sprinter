/**
  * @file    Application.cpp
  */

#include "Application.h"
#include "Util/Timer.h"

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
	usartLog(Periph::Usarts::Usart1, 9600),
	m_appRunningLed(Periph::Leds::Blue)
{}

void Application::run()
{
	INF_LOG("Application started running.");

	m_appRunningLed.turnOn();

	/* @non-terminating@ */
	for(;;) {
		Container::Result<Control::ControlData> communicationResult = communication.update();

		if(communicationResult.isValid)
			control.setControlData(communicationResult.value);

		control.update();
	}

	INF_LOG("Application ended.");
}

Application *Application::instance()
{
	return m_instance;
}
