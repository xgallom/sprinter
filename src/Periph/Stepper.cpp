/*
* Stepper.cpp
*
*  Created on: 17.9. 2018
*      Author: Zahorack
*/

#include "Periph/Stepper.h"
#include "Util/State.h"

namespace Periph {

namespace States {
enum Flags : uint8_t {
	IsRunning = 0x00,
};
}

static Util::State<uint16_t> s_steppersState;

struct {
	GPIO_TypeDef *portDirection, *portStep, *portEnable;
	uint16_t pinDirection, pinStep, pinEnable;
} static const Config[Steppers::Size] = {
		{ /* M1 */
				.portDirection = GPIOB,
				.portStep = GPIOG,
				.portEnable = GPIOD,
				.pinDirection = GPIO_Pin_10,
				.pinStep = GPIO_Pin_2,
				.pinEnable = GPIO_Pin_0
		},
		{ /* M2 */
				.portDirection = GPIOB,
				.portStep = GPIOG,
				.portEnable = GPIOD,
				.pinDirection = GPIO_Pin_11,
				.pinStep = GPIO_Pin_3,
				.pinEnable = GPIO_Pin_1
		}
};

static bool directionToPin(Util::Dirs::Enum direction)
{
	return direction == Util::Dirs::Backward ? true : false;
}

static Util::Dirs::Enum pinToDirection(bool pinState)
{
	return pinState ? Util::Dirs::Backward : Util::Dirs::Forward;
}

static uint32_t s_remainingSteps[Steppers::Size] = {0, 0};

Stepper::Stepper(Steppers::Enum id) :
	id(id),
	m_direction(Config[id].portDirection, Config[id].pinDirection),
	m_step(Config[id].portStep, Config[id].pinStep),
	m_enable(Config[id].portEnable, Config[id].pinEnable),
	m_timer(Util::Time::FromMilliSeconds(1))
{
	m_timer.start();

	m_step.resetPin();

	disable();
}

Stepper::~Stepper()
{
	disable();
}

void Stepper::disable()
{
	setRemainingSteps(0);
	m_enable.setPin();
}

void Stepper::enable()
{
	m_enable.resetPin();
}

bool Stepper::isEnabled() const
{
	return !m_enable.readPin();
}

void Stepper::pause()
{
	m_step.resetPin();
	s_steppersState.resetFlag(id);
};

void Stepper::resume()
{
	s_steppersState.setFlag(id);
}

bool Stepper::isRunning() const
{
	return isEnabled() && s_steppersState.flag(id);
}

void Stepper::moveForever()
{
	setRemainingSteps(1);
}

void Stepper::setRemainingSteps(uint32_t steps)
{
	s_remainingSteps[id] = steps << 1;
}

void Stepper::moveSteps(uint32_t steps)
{
	s_remainingSteps[id] += steps << 1;
}

void Stepper::cancelMovement()
{
	s_remainingSteps[id] = 0;
}

uint32_t Stepper::getRemainingSteps() const
{
	return s_remainingSteps[id];
}

void Stepper::setDirection(Util::Dirs::Enum direction)
{
	m_direction.setPinTo(directionToPin(direction));
}

Util::Dirs::Enum Stepper::getDirection() const
{
	return pinToDirection(m_direction.readPin());
}

void Stepper::update()
{
	if(isRunning() && m_timer.run()) {
		if(s_remainingSteps[id]) {
			m_step.togglePin();

			--s_remainingSteps[id];
		}
	}
}

} /* namespace Periph */
