/*
 * VehicleMode.cpp
 *
 *  Created on: Nov 16, 2018
 *      Author: xgallom
 */

#include "Control/VehicleMode.h"
#include "Util/Tools.h"
#include "Util/Direction.h"

namespace Control {

void VehicleMode::setControlData(const ControlData &controlData,
		Periph::EngineGroup &leftEngineGroup, Periph::EngineGroup &rightEngineGroup)
{
	uint8_t leftSpeed = 0, rightSpeed = 0;

	if(controlData.data.y > JoystickMiddle + JoystickTreshold) {
		leftEngineGroup.setTargetDirection(Util::Dirs::Forward);
		leftSpeed= controlData.data.y - JoystickMiddle;
	}
	else if(controlData.data.y < JoystickMiddle - JoystickTreshold) {
		leftEngineGroup.setTargetDirection(Util::Dirs::Backward);
		leftSpeed = JoystickMiddle - controlData.data.y;
	}

	if(controlData.data.x > JoystickMiddle + JoystickTreshold) {
		rightEngineGroup.setTargetDirection(Util::Dirs::Backward);
		rightSpeed = controlData.data.x - JoystickMiddle;
	}
	else if(controlData.data.x < JoystickMiddle - JoystickTreshold) {
		rightEngineGroup.setTargetDirection(Util::Dirs::Forward);
		rightSpeed = JoystickMiddle - controlData.data.x;
	}

	leftEngineGroup.setTargetSpeed(Util::clamp<uint8_t>(leftSpeed, 0, 100));
	rightEngineGroup.setTargetSpeed(Util::clamp<uint8_t>(rightSpeed, 0, 100));

	leftEngineGroup.start();
	rightEngineGroup.start();
}

}
