/*
 * Engine.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: Zahorack
 */

#include "Periph/Engine.h"
#include "Util/State.h"

namespace Periph {

namespace States {
enum Flags : uint8_t {
	IsRunning = 0x00,
	IsBackward,

	Offset
};
}

Util::State<uint16_t> s_engineState;
uint8_t s_engineSpeeds[Engines::Size] = { 0, 0, 0, 0, 0, 0 };

static Pwms::Enum enginesToPwms(Engines::Enum id)
{
	return static_cast<Pwms::Enum>(id);
}

static constexpr struct {
	GPIO_TypeDef *port;
	uint16_t id;
} DirPinsConfig[Engines::Size] = {
		{ /* M1 */
				port: GPIOA,
				id: GPIO_Pin_4
		},
		{ /* M2 */
				port: GPIOA,
				id: GPIO_Pin_4
		},
		{ /* M3 */
				port: GPIOA,
				id: GPIO_Pin_4
		},
		{ /* M4 */
				port: GPIOA,
				id: GPIO_Pin_4
		},
		{ /* M5 */
				port: GPIOA,
				id: GPIO_Pin_4
		},
		{ /* M6 */
				port: GPIOA,
				id: GPIO_Pin_4
		}
};

Engine::Engine(Engines::Enum id) :
	id(id),
	m_pwm(1000),
	m_direction(DirPinsConfig[id].port, DirPinsConfig[id].id)
{}

void Engine::start()
{
	s_engineState.setFlag(States::IsRunning + States::Offset * id);
}

void Engine::stop()
{
	s_engineState.resetFlag(States::IsRunning + States::Offset * id);
}

bool Engine::isRunning() const
{
	return s_engineState.flag(States::IsRunning + States::Offset * id);
}

void Engine::setTargetSpeed(uint8_t speed)
{
	s_engineSpeeds[id] = speed;
}

uint8_t Engine::getTargetSpeed() const
{
	return s_engineSpeeds[id];
}

uint8_t Engine::getCurrentSpeed() const
{
	return m_pwm.read(enginesToPwms(id));
}

void Engine::setTargetDirection(Dirs::Enum direction)
{
	s_engineState.setFlagTo(States::IsBackward + States::Offset * id, direction == Dirs::Backward ? true : false);
}

Dirs::Enum Engine::getTargetDirection() const
{
	return s_engineState.flag(States::IsBackward + States::Offset * id) ? Dirs::Backward : Dirs::Forward;
}

Dirs::Enum Engine::getCurrentDirection() const
{
	return m_direction.readPin() ? Dirs::Backward : Dirs::Forward;
}

void Engine::update()
{
	// Todo: systick

	uint8_t deltaSpeed = 0x00;

	if(getCurrentSpeed() < getTargetSpeed())
		deltaSpeed = 0x01;
	else if(getCurrentSpeed() > getTargetSpeed())
		deltaSpeed = 0xff; // Overflow <=> -1

	m_pwm.write(enginesToPwms(id), getCurrentSpeed() + deltaSpeed);

	// TODO: Make changing direction slow down
	if(getCurrentDirection() != getTargetDirection())
		m_direction.setPinTo(getTargetDirection() ? true : false);
}

} /* namespace Periph */
