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
#include "Util/Direction.h"
#include "Util/Timer.h"

// ZMENY
// PREROBIT ZDIELANIE

namespace Periph {

namespace Engines {
enum Enum : uint8_t {
	Engine1 = 0,
	Engine2,
	Engine3,
	Engine4,
	Engine5,
	Engine6,
	Engine7,

	Size
};
}

class Engine {
	const Engines::Enum id;

	Pwm m_pwm;
	DigitalOutputPin m_direction;
	Util::Timer m_timer;

public:
	Engine(Engines::Enum id);

	void start();
	void stop();
	bool isRunning() const;

	void setTargetSpeed(uint8_t speed);
	uint8_t getTargetSpeed() const;
	uint8_t getCurrentSpeed() const;

	void setTargetDirection(Util::Dirs::Enum direction);
	Util::Dirs::Enum getTargetDirection() const;
	Util::Dirs::Enum getCurrentDirection() const;

	void update();

private:
	void speedUp();
	void slowDown();

	void turnAround();
	void moveInDirection();

	void setCurrentSpeed(uint8_t speed);
	void setCurrentDirection(Util::Dirs::Enum direction);
};

} /* namespace Periph */

#endif /* PERIPH_ENGINE_H_ */
