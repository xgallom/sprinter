/*
 * VehicleMode.cpp
 *
 *  Created on: Nov 16, 2018
 *      Author: xgallom
 */

#include "Control/VehicleMode.h"
#include "Util/Tools.h"
#include "Util/Direction.h"

namespace Control
{
	void VehicleMode::setControlData(
			const ManualControlPacket &controlData,
			Periph::EngineGroup &leftEngineGroup,
			Periph::EngineGroup &rightEngineGroup
			)
	{
		uint8_t leftSpeed = 0, rightSpeed = 0;

		if(controlData.y > JoystickMiddle + JoystickTreshold) {
			leftEngineGroup.setTargetDirection(Util::Dirs::Forward);
			leftSpeed = controlData.y - JoystickMiddle;
		}
		else if(controlData.y < JoystickMiddle - JoystickTreshold) {
			leftEngineGroup.setTargetDirection(Util::Dirs::Backward);
			leftSpeed = JoystickMiddle - controlData.y;
		}

		if(controlData.x > JoystickMiddle + JoystickTreshold) {
			rightEngineGroup.setTargetDirection(Util::Dirs::Backward);
			rightSpeed = controlData.x - JoystickMiddle;
		}
		else if(controlData.x < JoystickMiddle - JoystickTreshold) {
			rightEngineGroup.setTargetDirection(Util::Dirs::Forward);
			rightSpeed = JoystickMiddle - controlData.x;
		}

		leftEngineGroup.setTargetSpeed(Util::clamp<uint8_t>(leftSpeed, 0, 100));
		rightEngineGroup.setTargetSpeed(Util::clamp<uint8_t>(rightSpeed, 0, 100));

		leftEngineGroup.start();
		rightEngineGroup.start();
	}
}
