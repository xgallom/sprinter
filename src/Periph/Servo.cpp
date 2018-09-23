/*
* Servo.cpp
*
*  Created on: 17.9. 2018
*      Author: Zahorack
*/

#include "Periph/Servo.h"
#include "Util/State.h"

namespace Periph {


uint8_t s_servoAngles[Serves::Size] = { 0, 0, 0, 0 };

static Pwms::Enum servesToPwms(Serves::Enum id)
{
	return static_cast<Pwms::Enum>(id);
}

Servo::Servo(Serves::Enum id) :
	id(id),
	m_pwm(50)
{
	m_pwm.write(servesToPwms(id), 0);
}



} /* namespace Periph */
