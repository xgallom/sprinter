/*
* Stepper.cpp
*
*  Created on: 17.9. 2018
*      Author: Zahorack
*/

#include "Periph/Stepper.h"
#include "Util/State.h"
#include "Util/Tool.h"
#include "Util/Time.h"

namespace Periph {


namespace Timers{
enum Enum : uint8_t{
	Timer1 = 0,

	Size
};
} /* namespace Timers */

static constexpr struct {
	GPIO_TypeDef *port;
	uint16_t id;
} DirPinsConfig[Steppers::Size] = {
		{ /* M1 */
				port: GPIOB,
				id: GPIO_Pin_10
		},
		{ /* M2 */
				port: GPIOB,
				id: GPIO_Pin_11
		}
};

static constexpr struct {
	GPIO_TypeDef *port;
	uint16_t id;
} StepPinsConfig[Steppers::Size] = {
		{ /* M1 */
				port: GPIOG,
				id: GPIO_Pin_2
		},
		{ /* M2 */
				port: GPIOG,
				id: GPIO_Pin_3
		}
};

static constexpr struct {
	GPIO_TypeDef *port;
	uint16_t id;
} EnablePinsConfig[Steppers::Size] = {
		{ /* M1 */
				port: GPIOD,
				id: GPIO_Pin_0
		},
		{ /* M2 */
				port: GPIOD,
				id: GPIO_Pin_1
		}
};
static bool directionToPin(Dirs::Enum direction)
{
	return direction == Dirs::Backward ? true : false;
}

static Dirs::Enum pinToDirection(bool pinState)
{
	return pinState ? Dirs::Backward : Dirs::Forward;
}

Stepper::Stepper(Steppers::Enum id) :
	id(id),
	m_direction(DirPinsConfig[id].port, DirPinsConfig[id].id),
	m_step(StepPinsConfig[id].port, StepPinsConfig[id].id),
	m_enable(EnablePinsConfig[id].port, EnablePinsConfig[id].id),
	m_timer(Util::Time::FromMilliSeconds(1))
{
	init();
	m_timer.start();
};

Stepper::~Stepper()
{};

void Stepper::init(){

	m_step.resetPin();
	disable();
}

void Stepper::freeRun(){

	if(m_timer.run()){
		if(getSoftwareState()){
			hardwareEnable();
			m_step.togglePin();
		}
		else hardwareDisable();
	}
}

void Stepper::run()
{
	if(m_timer.run()){
		//if(getSoftwareState() && getHardwareState()){
			update();
		//}
		//else hardwareDisable();
	}
}

void Stepper::update()
{
	if(isBussy()){
		m_step.togglePin();
		m_currentSteps++;
	}
}

void Stepper::stop()
{
	disable();
}

void Stepper::start()
{
	enable();
}

void Stepper::setSpeed(uint16_t speed){
	Util::Tool tool;
	uint16_t time = tool.map(speed, 0, 100, 100, 1);
	m_timer.setInterval(Util::Time::FromMilliSeconds(time));
}

void Stepper::setTargetSteps(uint32_t steps)
{
	m_currentSteps = 0;
	m_targetSteps = steps;
}

uint32_t Stepper::getCurrentSteps() const
{
	return m_currentSteps;
}

uint32_t Stepper::getTargetSteps() const
{
	return m_targetSteps;
}

void Stepper::setCurrentDirection(Dirs::Enum direction)
{
		m_direction.setPinTo(directionToPin(direction));
}

Dirs::Enum Stepper::getCurrentDirection() const
{
	return pinToDirection(m_direction.readPin());
}

void Stepper::hardwareDisable()
{
	if(m_enable.readPin() != true)
		m_enable.setPin();
}

void Stepper::hardwareEnable()
{
	if(m_enable.readPin() != false)
		m_enable.resetPin();
}

void Stepper::softwareDisable(){

	if(m_state != false){
		m_step.resetPin();
		m_state = false;
	}
};

void Stepper::softwareEnable(){

	if(m_state != true){
		m_state = true;
	}
}

void Stepper::enable(){
	softwareEnable();
	hardwareEnable();
}

void Stepper::disable(){
	softwareDisable();
	hardwareDisable();
}

uint8_t Stepper::getState() const{

	return (getSoftwareState() && getHardwareState());
}

} /* namespace Periph */
