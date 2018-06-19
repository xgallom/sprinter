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
	Periph::Led appRunningLed;
	Periph::Usart usart2;

public:
	Application();

	void run();
};

#endif /* APPLICATION_H_ */
