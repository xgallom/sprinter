/*
* Servo.cpp
*
*  Created on: 17.9. 2018
*      Author: Zahorack
*/

#include "Periph/Servo.h"
#include "Util/State.h"

namespace Periph {

namespace States {
enum Flags : uint8_t {
	IsRunning = 0x00,
	IsStopped,

	Offset
};
}
Util::State<uint16_t> s_servoState;

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
static constexpr uint16_t  s_servoMaxPosition = s_servoConstatnt + 2000;
static constexpr uint16_t  s_servoMinPosition = s_servoConstatnt;



uint16_t s_servoAngles[Servos::Size] = { 0, 0, 0, 0 };

static Pwms::Enum servosToPwms(Servos::Enum id)
{
	return static_cast<Pwms::Enum>(id + static_cast<Servos::Enum>(Pwms::Pwm9));
}

Servo::Servo(Servos::Enum id) :
	id(id),
	m_pwm(10000),
	m_timer(Util::Time::FromMilliSeconds(10))
{
	start();
	m_timer.start();
	m_pwm.write(servosToPwms(id), s_servoHomePosition);
}

Servo::~Servo(){
	stop();
}

void Servo::start()
{
	s_servoState.setFlag(States::IsRunning + States::Offset * id);
}

void Servo::setTargetAngle(uint16_t angle)
{
	s_servoAngles[id] = angle;
}

void Servo::stop()
{
	s_servoState.resetFlag(States::IsRunning + States::Offset * id);
}

bool Servo::isRunning() const
{
	return s_servoState.flag(States::IsRunning + States::Offset * id);
}

uint16_t Servo::getTargetAngle() const
{
	return s_servoAngles[id];
}

void Servo::addAngle(int16_t angle){
	if(getCurrentAngle() + angle < s_servoMaxPosition && getCurrentAngle() + angle > s_servoMinPosition){
		m_pwm.write(servosToPwms(id), getCurrentAngle() + angle);
	}
}

uint16_t Servo::getCurrentAngle() const
{
	return m_pwm.read(servosToPwms(id));
}

void Servo::incrementAngle()
{
	if(isRunning())
		if(getCurrentAngle() < s_servoMaxPosition){
			m_pwm.write(servosToPwms(id), getCurrentAngle() + 1);
		}
}

void Servo::decrementAngle()
{
	if(isRunning())
		if(getCurrentAngle() > s_servoMinPosition){
			m_pwm.write(servosToPwms(id), getCurrentAngle() - 1);
		}
}

void Servo::update()
{
	if(getCurrentAngle() < getTargetAngle())
		incrementAngle();

	else if(getCurrentAngle() > getTargetAngle())
		decrementAngle();
}

void Servo::run(){

	if(m_timer.run()){
		update();
	}
}

void Servo::hardStop(){
	m_pwm.deintServoPwm();
	stop();
}

void Servo::hardStart(){
	m_pwm.intServoPwm();
	start();
}

void Servo::test(){

	if(getCurrentAngle() == getTargetAngle()){
		setTargetAngle(getCurrentAngle() == 2500 ? 500 : 2500);
			}

		update();
}

} /* namespace Periph */
