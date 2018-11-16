/*
 * Simulation.cpp
 *
 *  Created on: Nov 16, 2018
 *      Author: xgallom
 */

#include <Control/SimulationMode.h>

namespace Control {

namespace Tasks {
enum Enum : uint8_t {
  Right = 0,
  Down,
  Left,
  Up,

  Size
};
}

void SimulationMode::setControlData(const ControlData &controlData, Periph::Stepper &horizontalStepper, Periph::Stepper &verticalStepper)
{
	m_currentTask = 0;

	horizontalStepper.enable();
	verticalStepper.enable();

	doTask(horizontalStepper, verticalStepper);
}

void SimulationMode::update(Periph::Stepper &horizontalStepper, Periph::Stepper &verticalStepper)
{
	Periph::Stepper &workingStepper = m_currentTask & 0x01 ? verticalStepper : horizontalStepper;

	if(!workingStepper.getRemainingSteps()) {
		incrementTask();

		doTask(horizontalStepper, verticalStepper);
	}
}

void SimulationMode::doTask(Periph::Stepper &horizontalStepper, Periph::Stepper &verticalStepper)
{
	Periph::Stepper *stepper;
	Util::Dirs::Enum direction;

	switch(static_cast<Tasks::Enum>(m_currentTask)) {
	case Tasks::Right:
		stepper = &horizontalStepper;
		direction = Util::Dirs::Forward;
		break;

	case Tasks::Down:
		stepper = &verticalStepper;
		direction = Util::Dirs::Backward;
		break;

	case Tasks::Left:
		stepper = &horizontalStepper;
		direction = Util::Dirs::Backward;
		break;

	case Tasks::Up:
		stepper = &verticalStepper;
		direction = Util::Dirs::Forward;
		break;

	case Tasks::Size:
		break;
	}

	stepper->setRemainingSteps(1000);
	stepper->setDirection(direction);
}

void SimulationMode::incrementTask()
{
	if(++m_currentTask >= Tasks::Size)
		m_currentTask = 0;
}

}
