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

class Application {
	static Application *m_insance;

	Periph::Led appRunningLed;
	Periph::Usart usart2;

public:
	Application();

	void run();

	static Application *instance();
};

#define app ::Application::instance()

#endif /* APPLICATION_H_ */
