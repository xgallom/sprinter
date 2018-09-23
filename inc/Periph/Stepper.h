/*
 * Stepper.h
 *
 *  Created on: 17.9. 2018
 *      Author: Zahorack
 */

#ifndef PERIPH_STEPPER_H_
#define PERIPH_STEPPER_H_

#include <Periph/DigitalOutputPin.h>
#include "stm32f4xx.h"
#include "Pwm.h"

namespace Periph {

namespace Steppers {
enum Enum : uint8_t {
	Stepper1 = 0,
	Stepper2,

	Size
};
}

class Stepper {

};

} /* namespace Stepper */

#endif /* PERIPH_STEPPER_H_ */
