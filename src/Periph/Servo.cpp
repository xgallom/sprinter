/*
* Servo.cpp
*
*  Created on: 17.9. 2018
*      Author: Zahorack
*/

#include "Periph/Servo.h"
#include "Util/State.h"

namespace Periph
{
	namespace States
	{
		enum Flags : uint8_t {
			IsRunning = 0x00,
		};
	}

	static Util::State<uint8_t> s_servoState;

	/* Servo  specification
	 *
	 * Left position   s_servoConstant
	 * Home position:  s_servoConstant + 1000
	 * Right position  s_servoConstant + 2000
	 *
	 * Working resolution: +-1000 samples
	 */

	static constexpr uint16_t
		ServoConstant = 500,
		ServoHomePosition = 1000,
		ServoMaxPosition = 2000,
		ServoMinPosition = 0;

	static uint16_t s_servoAngles[Servos::Size] = { 0, 0, 0, 0 };

	static Pwms::Enum servosToPwms(Servos::Enum id)
	{
		return static_cast<Pwms::Enum>(id + static_cast<Servos::Enum>(Pwms::Pwm9));
	}

	Servo::Servo(Servos::Enum id) :
		id(id),
		m_timer(Util::Time::FromMilliSeconds(10))
	{
		start();
		m_timer.start();

		setTargetAngle(ServoHomePosition);
	}

	Servo::Servo(Servos::Enum id, uint16_t defaultAngle) :
		id(id),
		m_timer(Util::Time::FromMilliSeconds(10))
	{
		start();
		m_timer.start();

		setTargetAngle(ServoHomePosition + defaultAngle);
	}

	Servo::~Servo()
	{
		stop();
	}

	void Servo::start()
	{
		s_servoState.setFlag(id);
	}

	void Servo::stop()
	{
		s_servoState.resetFlag(id);
	}

	bool Servo::isRunning() const
	{
		return s_servoState.flag(id);
	}

	void Servo::setTargetAngle(uint16_t angle)
	{
		s_servoAngles[id] = ServoConstant + Util::clamp(angle, ServoMinPosition, ServoMaxPosition);
	}

	uint16_t Servo::getTargetAngle() const
	{
		return s_servoAngles[id] - ServoConstant;
	}

	uint16_t Servo::getCurrentAngle() const
	{
		return m_pwm.read(servosToPwms(id));
	}

	void Servo::incrementAngle()
	{
		setTargetAngle(getTargetAngle() + 1);
	}

	void Servo::decrementAngle()
	{
		setTargetAngle(getTargetAngle() - 1);
	}

	void Servo::addAngle(int16_t angle)
	{
		setTargetAngle(getTargetAngle() + angle);
	}

	void Servo::update()
	{
		if(isRunning() && m_timer.run()) {
			if(getCurrentAngle() < getTargetAngle())
				incrementAngleInternal();
			else if(getCurrentAngle() > getTargetAngle())
				decrementAngleInternal();
		}
	}

	void Servo::incrementAngleInternal()
	{
		if(getCurrentAngle() < ServoHomePosition + ServoMaxPosition)
			m_pwm.write(servosToPwms(id), getCurrentAngle() + 1);
	}

	void Servo::decrementAngleInternal()
	{
		if(getCurrentAngle() > ServoHomePosition + ServoMinPosition)
			m_pwm.write(servosToPwms(id), getCurrentAngle() - 1);
	}
}
