/*
* Stepper.cpp
*
*  Created on: 17.9. 2018
*      Author: Zahorack
*/

#include "Periph/Stepper.h"
#include "Util/State.h"

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

Stepper::Stepper(Steppers::Enum id) :
	id(id),
	m_direction(DirPinsConfig[id].port, DirPinsConfig[id].id),
	m_step(StepPinsConfig[id].port, StepPinsConfig[id].id),
	m_enable(EnablePinsConfig[id].port, EnablePinsConfig[id].id),
	m_timer(Util::Time::FromMilliSeconds(1))
{
	m_timer.start();
	enable();
};

Stepper::~Stepper()
{};

void Stepper::run()
{
	//enable();
	if(m_timer.run()){
		m_step.togglePin();
	}
}

void Stepper::setCurrentDirection(Dirs::Enum direction)
{
	m_direction.setPinTo(directionToPin(direction));
}

void Stepper::disable()
{
	m_enable.resetPin();
}

void Stepper::enable()
{
	m_enable.resetPin();
}


} /* namespace Periph */
