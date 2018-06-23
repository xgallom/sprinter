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
#include "Dir.h"

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



class Engine {

	Pwm pwm;
	Dir dir;

	const Engines::Enum id;
	uint8_t m_speed;

	void setSpeed(uint8_t speed);
	void setDirection(Dirs::Enum direction);

public:
	Engine(Engines::Enum id);


	uint8_t getSpeed();

	void update();
	void update(uint8_t speed);
	void update(uint8_t speed, Dirs::Enum direction);

	void stop();
};

} /* namespace Periph */

#endif /* PERIPH_ENGINE_H_ */
