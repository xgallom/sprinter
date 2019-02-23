/*
 *  Suntracker.h
 *
 *  Created on: January 6, 2019
 *      Author: Zahorack
 */

#ifndef SUNTRACKER_H_
#define SUNTRACKER_H_

#include "stm32f4xx.h"
#include "Util/Timer.h"
#include "Periph/Adc.h"
#include "Periph/Servo.h"

namespace Device {

namespace Kvadrants {
/*
 * Top view
 ___________
|  II |  I  |
|_____O_____|
| III |  IV |
|_____|_____|
*/

enum Enum : uint8_t {
	I = 0,
	II,
	III,
	IV,
	O,

	Size
};
}

class Suntracker {
	Periph::Adc m_adcs[5] = {
			Periph::Adc(Periph::AdcPins::AdcPin1), Periph::Adc(Periph::AdcPins::AdcPin2),
			Periph::Adc(Periph::AdcPins::AdcPin3), Periph::Adc(Periph::AdcPins::AdcPin4),
			Periph::Adc(Periph::AdcPins::AdcPin5),
	};
	Periph::Servo 	m_servo1, m_servo2;
	Util::Timer m_timer;

	void measure();
public:
	Suntracker();

	void update();

};

}

#endif