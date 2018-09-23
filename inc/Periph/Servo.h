/*
 * Servo.h
 *
 *  Created on: 17.9. 2018
 *      Author: Zahorack
 */

#ifndef PERIPH_SERVO_H_
#define PERIPH_SERVO_H_

#include <Periph/DigitalOutputPin.h>
#include "stm32f4xx.h"
#include "Pwm.h"

namespace Periph {

namespace Servos {
enum Enum : uint8_t {
	Servo1 = 0,
	Servo2,
	Servo3,
	Servo4,

	Size
};
}

class Servo {
	const Servos::Enum id;
	Pwm m_pwm;

	void incrementAngle();
	void decrementAngle();

public:
	Servo(Servos::Enum id);
	~Servo();

	void setTargetAngle(uint16_t angle);
	uint16_t getCurrentAngle() const;
	uint16_t getTargetAngle() const;
	void test();
	void update();

};

} /* namespace Servo */

#endif /* PERIPH_SERVO_H_ */
