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
#include "Util/Timer.h"
#include "Periph/Engine.h"

namespace Periph {

//namespace Dirs {
//enum Enum : bool {
//	Forward = false,
//	Backward = true
//};
//}

namespace Steppers {
enum Enum : uint8_t {
	Stepper1 = 0,
	Stepper2,

	Size
};
}

class Stepper{
	const Steppers::Enum id;
	DigitalOutputPin m_direction;
	DigitalOutputPin m_step;
	DigitalOutputPin m_enable;
	Util::Timer m_timer;
public:
	Stepper(Steppers::Enum id);
	~Stepper();

	void initTimTB();
	void initTimOC();
	void initGpio();
	void initRCC();

	void run();
	void setCurrentDirection(Dirs::Enum direction);
	void disable();
	void enable();

};


} /* namespace Stepper */

#endif /* PERIPH_STEPPER_H_ */
