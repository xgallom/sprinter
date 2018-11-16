/*
 * Simulation.h
 *
 *  Created on: Nov 16, 2018
 *      Author: xgallom
 */

#ifndef CONTROL_SIMULATIONMODE_H_
#define CONTROL_SIMULATIONMODE_H_

#include "Periph/Stepper.h"
#include "Control/Communication.h"

namespace Control {

class SimulationMode {
	uint8_t m_currentTask = 0;

public:
	void setControlData(const ControlData &controlData, Periph::Stepper &horizontalStepper, Periph::Stepper &verticalStepper);
	void update(Periph::Stepper &horizontalStepper, Periph::Stepper &verticalStepper);

private:
	void doTask(Periph::Stepper &horizontalStepper, Periph::Stepper &verticalStepper);
	void incrementTask();
};

}

#endif /* CONTROL_SIMULATIONMODE_H_ */
