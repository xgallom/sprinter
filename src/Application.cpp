/*
 * Application.cpp
 *
 *  Created on: Jun 17, 2018
 *      Author: xgallom
 */

#include <Util/Trace.h>
#include "Application.h"
#include "Util/Timer.h"
#include "Periph/Encoder.h"
#include "stm32f4xx.h"

LOGGER_MODULE(Application)

Application *Application::m_instance = nullptr;

Util::PidArgs_t pidArgs;

Application::ApplicationInitializator::ApplicationInitializator(Application *parent)
{
	m_instance = parent;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
}

Application::Application() :
	m_applicationInitializator(this),
	logger(&usartLog),
	usartLog(Periph::Usarts::Usart1, 9600),
	encoder(Periph::EncoderPins::EncoderPin1),
	m_appRunningLed(Periph::Leds::Blue),
	engine(Periph::Engines::M6)
{}

void Application::run()
{
	INF_LOG("Application started running.");

	m_appRunningLed.turnOn();
	Util::Timer timer(Util::Time::FromMilliSeconds(100));
	timer.start();

	pidArgs.Kd = 0.005;
	pidArgs.Ki = 3;
	pidArgs.Kp = 1;
	pidArgs.dt = 0.1;
	pidArgs.max = 100;
	pidArgs.min = 0;

	pid.setParameters(pidArgs);


	/* @non-terminating@ */
	for(;;) {
		ctrl.run();


		engine.update();
		encoder.update();

		if(timer.run()){
			uint8_t pidresult = pid.process(50, encoder.getAngularSpeed()*1.1);
			TRACE("PID : %d\n\r", encoder.getAngularSpeed());

			engine.setTargetSpeed(pidresult);

			//ctrl.update();
			static uint32_t cnt =0;
			cnt++;

			if(cnt > 100) {
				TRACE("angular speed: %d  period: %d  ticks: %d \n\r", encoder.getAngularSpeed(), encoder.getPeriod(), encoder.getCounter());
				cnt = 0;
			}
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

	NVIC_SystemReset();
	/* Infinite loop */

	while (1) {
	}
}
#endif

//NASA JPL assert example:
//#define c_assert(e) ((e) ? (true) : \
//TRACE("%s,%d: assertion '%s' failed\n", \
//__FILE__, __LINE__, #e), false)
//
//if (!c_assert(3 >= 0) == true) {
// return ERROR;
// }



