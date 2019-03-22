/*
 * PrintingMode.cpp
 *
 *  Created on: Nov 16, 2018
 *      Author: xgallom
 */

#include "Control/PrintingMode.h"

namespace Control
{
	void PrintingMode::setControlData(
			const ManualControlPacket &controlData,
			Periph::Stepper &horizontalStepper,
			Periph::Stepper &verticalStepper
			)
	{
		horizontalStepper.enable();
		verticalStepper.enable();

		if(controlData.x > JoystickMiddle + JostickQuadrantOffset) {
			horizontalStepper.resume();
			horizontalStepper.setDirection(Util::Dirs::Backward);
		}
		else if(controlData.x < JoystickMiddle - JostickQuadrantOffset) {
			horizontalStepper.resume();
			horizontalStepper.setDirection(Util::Dirs::Forward);
		}
		else
			horizontalStepper.pause();

		if(controlData.y > JoystickMiddle + JostickQuadrantOffset) {
			verticalStepper.resume();
			verticalStepper.setDirection(Util::Dirs::Forward);
		}
		else if(controlData.y < JoystickMiddle - JostickQuadrantOffset) {
			verticalStepper.resume();
			verticalStepper.setDirection(Util::Dirs::Backward);
		}
		else
			verticalStepper.pause();
	}

	void PrintingMode::update(
			Periph::Stepper &horizontalStepper,
			Periph::Stepper &verticalStepper
			)
	{
		horizontalStepper.moveForever();
		verticalStepper.moveForever();
	}
}
