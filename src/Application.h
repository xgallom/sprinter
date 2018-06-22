/*
 * Application.h
 *
 *  Created on: Jun 17, 2018
 *      Author: xgallom
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "Periph/Led.h"
#include "Periph/Usart.h"
#include "Periph/Engine.h"

class Application {
	class ApplicationInitializator {
	public:
		ApplicationInitializator(Application *parent);
	};

	static Application *m_instance;

	ApplicationInitializator applicationInitializator;

	Periph::Led appRunningLed;
	Periph::Usart usart2;
	Periph::Engine engine1;

public:
	Application();

	void run();

	static Application *instance();
};

#define App ::Application::instance()

#endif /* APPLICATION_H_ */
