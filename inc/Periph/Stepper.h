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

	uint8_t m_state = 0;
	uint32_t m_targetSteps;
	uint32_t m_currentSteps;
public:
	Stepper(Steppers::Enum id);
	~Stepper();

	void initTimTB();
	void initTimOC();
	void initGpio();
	void initRCC();

	void init();
	void run();
	void stop();
	void start();
	void freeRun();	/*asynchronics run*/

	bool isBussy(){ return (getTargetSteps() > getCurrentSteps()); };

	void setSpeed(uint16_t);
	void setTargetSteps(uint32_t steps);
	uint32_t getCurrentSteps() const;
	uint32_t getTargetSteps() const;
	void setCurrentDirection(Dirs::Enum direction);
	Dirs::Enum getCurrentDirection() const;

	void enable();
	void disable();
	uint8_t getState() const;

	void hardwareDisable();
	void hardwareEnable();
	uint8_t	getHardwareState() const{ return m_enable.readPin();};

	void softwareDisable();
	void softwareEnable();
	uint8_t getSoftwareState() const{ return m_state; };

	void update();
};


} /* namespace Stepper */

#endif /* PERIPH_STEPPER_H_ */
