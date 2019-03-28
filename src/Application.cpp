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
	communication.sendStatus();

	Util::Timer timer(Util::Time::FromMilliSeconds(100));
	timer.start();

	/* @non-terminating@ */
	for(;;) {
		Container::Result<Control::Packet> communicationResult = communication.update();

		if(communicationResult.isValid && communicationResult.value.header.type == Control::PacketType::ManualControl)
			control.setControlData(communicationResult.value.contents.dataPacket);

		control.update();

		if(timer.run()) {
			communication.sendStatus();
			INF_LOG("Tick");
		}
	}

	INF_LOG("Application ended.");
}

Application *Application::instance()
{
	return m_instance;
}
