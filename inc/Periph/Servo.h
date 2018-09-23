/*
 * Servo.h
 *
 *  Created on: 17.9. 2018
 *      Author: Zahorack
 */

#ifndef PERIPH_SERVO_H_
#define PERIPH_SERVO_H__

#include <Periph/DigitalOutputPin.h>
#include "stm32f4xx.h"
#include "Pwm.h"

namespace Periph {

namespace Serves {
enum Enum : uint8_t {
	Servo1 = 0,
	Servo2,
	Servo3,
	Servo4,

	Size
};
}

class Servo {
	const Serves::Enum id;
	Pwm m_pwm;

	void incrementAngle();
	void decrementAngle();

public:
	Servo(Serves::Enum id);

	void setTargetAngle(uint8_t angle);
	uint8_t getCurrentAngle();
	uint8_t getTargetAngle();

	void update();

};

} /* namespace Servo */

#endif /* PERIPH_SERVO_H_ */
