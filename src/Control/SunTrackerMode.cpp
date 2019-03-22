/*
 * SunTrackerMode.cpp
 *
 *  Created on: Nov 16, 2018
 *      Author: xgallom
 */

#include "Control/SunTrackerMode.h"

namespace Control
{
	void SunTrackerMode::setControlData(
			const ManualControlPacket &controlData,
			Periph::Servo &horizontalServo,
			Periph::Servo &verticalServo,
			Periph::Engine &differential
			)
	{
		differential.setTargetSpeed(controlData.pot);

		if(controlData.buttonLeft)
			differential.setTargetDirection(Util::Dirs::Backward);
		else if(controlData.buttonRight)
			differential.setTargetDirection(Util::Dirs::Forward);
		else
			differential.setTargetSpeed(0);

		uint8_t state = 0x00;

		if(controlData.x > 90)
			state = 0b00000011;
		else if(controlData.x < 10)
			state = 0b00000001;

		if(controlData.y > 90)
			state |= 0b00001100;
		else if(controlData.y < 10)
			state |= 0b00000100;

		m_state.stateVariable = state;
	}

	void SunTrackerMode::update(
			Periph::Servo &horizontalServo,
			Periph::Servo &verticalServo
			)
	{
		if(m_state.flag(Enabled)) {
			if(horizontalServo.getCurrentAngle() == horizontalServo.getTargetAngle()) {
				if(m_state.flag(Direction))
					horizontalServo.incrementAngle();
				else
					horizontalServo.decrementAngle();
			}
		}

		if(m_state.flag(Enabled + Offset)) {
			if(verticalServo.getCurrentAngle() == verticalServo.getTargetAngle()) {
				if(m_state.flag(Direction + Offset))
					verticalServo.incrementAngle();
				else
					verticalServo.decrementAngle();
			}
		}
	}
}
