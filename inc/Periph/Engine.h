/*
 * Engine.h
 *
 *  Created on: Jun 18, 2018
 *      Author: Zahorack
 */

#ifndef PERIPH_ENGINE_H_
#define PERIPH_ENGINE_H_

#include <Periph/DigitalOutputPin.h>
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

	M7,	//differential

	Size
};
}

namespace Dirs {
enum Enum : bool {
	Forward = false,
	Backward = true
};
}

class Engine {
	const Engines::Enum id;

	Pwm m_pwm;
	DigitalOutputPin m_direction;

	void turnAround();
	void moveInDirection();

	void slowDown();
	void speedUp();

public:
	Engine(Engines::Enum id);

	void start();
	void stop();
	bool isRunning() const;
	void setCurrentDirection(Dirs::Enum direction);
	void setTargetSpeed(uint8_t speed);
	uint8_t getTargetSpeed() const;
	uint8_t getCurrentSpeed() const;

	void setTargetDirection(Dirs::Enum direction);
	Dirs::Enum getTargetDirection() const;
	Dirs::Enum getCurrentDirection() const;

	void update();
};

} /* namespace Periph */

#endif /* PERIPH_ENGINE_H_ */
