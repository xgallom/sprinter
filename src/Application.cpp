/*
 * Application.cpp
 *
 *  Created on: Jun 17, 2018
 *      Author: xgallom
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
static uint8_t *buffer;
Application::Application() :
	m_applicationInitializator(this),
	logger(&usartLog),
	usartLog(Periph::Usarts::Usart2, 9600),
	i2c(Periph::I2Cs::I2Cx2),
	m_appRunningLed(Periph::Leds::Blue),
	m_engine1(Periph::Engines::M3)

{}

void Application::run()
{
	INF_LOG("Application started running.");

	m_appRunningLed.turnOn();

	m_engine1.setTargetSpeed(100);
	m_engine1.setTargetDirection(Periph::Dirs::Forward);

	Util::Timer timer(Util::Time::FromMilliSeconds(1000));
	timer.start();


	for(;;) {
		//uint8_t input[256];
		//uint32_t inputSize = usartLog.readLine(input, 256);
		//usartLog.write(input, inputSize);

		uint8_t MPU_ADDR =  0b1101000 << 1;
		 uint8_t ACCEL_XOUT_H = 0x3B;
		 uint8_t ACCEL_XOUT_L = 0x3C;
		 uint8_t buff[256];


		if(timer.run()){

			i2c.read(MPU_ADDR, ACCEL_XOUT_H, buff,1);


//			INF_LOG("Connect.");
//
//			if(m_engine1.getCurrentSpeed() == m_engine1.getTargetSpeed()){
//			m_engine1.setTargetDirection(m_engine1.getCurrentDirection() ? Periph::Dirs::Forward : Periph::Dirs::Backward);
//					}
//
//			m_engine1.update();
	}
	}
	INF_LOG("Application ended.");
}

Application *Application::instance()
{
	return m_instance;
}

