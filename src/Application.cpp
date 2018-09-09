/*
 * Application.cpp
 *
 *  Created on: Jun 17, 2018
 *      Author: xgallom
 */

#include <Util/Trace.h>
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

	Util::Timer timer(Util::Time::FromMilliSeconds(10));
	timer.start();

	for(;;) {

		ctrl.run();


		if(timer.run()){
			//INF_LOG("Application update");
			ctrl.update();
	}
	}
	INF_LOG("Application ended.");
}

Application *Application::instance()
{
	return m_instance;
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */

void assert_failed(uint8_t* file, uint32_t line) {
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	TRACE("Wrong parameters value: file %s on line %d\r\n", file, line);

	/* Infinite loop */
	while (1) {
	}
}
#endif

