/*
 * SunTrackerMode.h
 *
 *  Created on: Nov 16, 2018
 *      Author: xgallom
 */

#ifndef CONTROL_SUNTRACKERMODE_H_
#define CONTROL_SUNTRACKERMODE_H_

#include "Control/Communication.h"
#include "Util/State.h"
#include "Periph/Servo.h"
#include "Periph/Engine.h"

namespace Control {

class SunTrackerMode {
	enum States : uint8_t {
		Enabled = 0,
		Direction
	};

	static const uint8_t Offset = 2;

	Util::State<uint8_t> m_state;

public:
	void setControlData(const ControlData &controlData, Periph::Servo &horizontalServo, Periph::Servo &verticalServo, Periph::Engine &differential);
	void update(Periph::Servo &horizontalServo, Periph::Servo &verticalServo);
};

}

#endif /* CONTROL_SUNTRACKERMODE_H_ */
