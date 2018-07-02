/*
 * Application.h
 *
 *  Created on: Jun 17, 2018
 *      Author: xgallom
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "stm32f4xx.h"
#include "Periph/Led.h"
#include "Util/Logger.h"
#include "Periph/Engine.h"
#include "Periph/I2C.h"

class Application {
	static Application *m_instance;

	class ApplicationInitializator {
	public:
		ApplicationInitializator(Application *parent);
	} m_applicationInitializator;


public:
	Util::Logger logger;
	Periph::Usart usartLog;
	Periph::I2C i2c;

private:
	Periph::Led m_appRunningLed;
	Periph::Engine m_engine1;

public:
	Application();

	void run();

	static Application *instance();

};

#define App ::Application::instance()

#endif /* APPLICATION_H_ */
