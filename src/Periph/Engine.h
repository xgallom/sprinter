/*
 * Engine.h
 *
 *  Created on: Jun 18, 2018
 *      Author: Zahorack
 */

#ifndef PERIPH_ENGINE_H_
#define PERIPH_ENGINE_H_

#include "stm32f4xx.h"
#include "Pwm.h"

namespace Periph {

namespace Engines {
enum Enum : uint8_t {
	M1 = 0,
	M2,
	M3,
	M4,
	M5,
	M6,

	Size
};
}

namespace Dir {
enum Enum : bool {
	Forward = false,
	Backward = true
};
}

class Engine {
	Pwm pwm;

	const Engines::Enum id;
	uint8_t m_speed;

public:
	Engine(Engines::Enum id);

	void setSpeed(uint8_t speed);
	uint8_t getSpeed();

	void setDirection(Dir::Enum direction);

	void run();
	void run(uint8_t speed);
	void run(uint8_t speed, Dir::Enum direction);

	void stop();
};

} /* namespace Periph */

#endif /* PERIPH_ENGINE_H_ */
