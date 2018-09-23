/*
* Servo.cpp
*
*  Created on: 17.9. 2018
*      Author: Zahorack
*/

#include "Periph/Servo.h"
#include "Util/State.h"

/*Servo  specification
 *
 * Left position   s_servoConstatnt
 * Home position:  s_servoConstatnt + 1000
 * Right position  s_servoConstatnt + 2000
 *
 * Working resolution: +-1000 samples
 *
 */

static constexpr uint16_t  s_servoConstatnt = 500;	 	//first acceptable value in us
static constexpr uint16_t  s_servoHomePosition = s_servoConstatnt + 1000;


namespace Periph {
uint16_t s_servoAngles[Servos::Size] = { 0, 0, 0, 0 };

static Pwms::Enum servosToPwms(Servos::Enum id)
{
	return static_cast<Pwms::Enum>(id + static_cast<Servos::Enum>(Pwms::Pwm9));
}

Servo::Servo(Servos::Enum id) :
	id(id),
	m_pwm(1000)
{
	m_pwm.write(servosToPwms(id), s_servoHomePosition);
}

Servo::~Servo(){}

void Servo::setTargetAngle(uint16_t angle)
{
	s_servoAngles[id] = angle;
}

uint16_t Servo::getTargetAngle() const
{
	return s_servoAngles[id];
}

uint16_t Servo::getCurrentAngle() const
{
	return m_pwm.read(servosToPwms(id));
}

void Servo::incrementAngle()
{
	m_pwm.write(servosToPwms(id), getCurrentAngle() + 1);
}

void Servo::decrementAngle()
{
	m_pwm.write(servosToPwms(id), getCurrentAngle() - 1);
}

void Servo::update()
{
	if(getCurrentAngle() < getTargetAngle())
		incrementAngle();

	else if(getCurrentAngle() > getTargetAngle())
		decrementAngle();
}

void Servo::test(){

	if(getCurrentAngle() == getTargetAngle()){
		setTargetAngle(getCurrentAngle() == 2500 ? 500 : 2500);
			}

		update();
}

} /* namespace Periph */
