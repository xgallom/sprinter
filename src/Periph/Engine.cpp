/*
 * Engine.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: Zahorack
 */

#include "Periph/Engine.h"
#include "Util/State.h"

namespace Periph
{
	namespace States
	{
		enum Flags : uint8_t {
			IsRunning = 0x00,
			IsBackward
		};

		static constexpr uint8_t Offset = 1;
	}

	static Util::State<uint16_t> s_engineState;
	static uint8_t s_engineSpeeds[Engines::Size] = { 0, 0, 0, 0, 0, 0 };

	static Pwms::Enum enginesToPwms(Engines::Enum id)
	{
		return static_cast<Pwms::Enum>(id);
	}

	static Util::Dirs::Enum pinToDirection(bool pinState)
	{
		return pinState ? Util::Dirs::Backward : Util::Dirs::Forward;
	}

	static bool directionToPin(Util::Dirs::Enum direction)
	{
		return direction == Util::Dirs::Backward ? true : false;
	}

	struct {
		GPIO_TypeDef *port;
		uint16_t id;
	} static constexpr DirPinsConfig[Engines::Size] = {
			{
					.port = GPIOE,
					.id = GPIO_Pin_7
			},
			{
					.port = GPIOE,
					.id = GPIO_Pin_8
			},
			{
					.port = GPIOE,
					.id = GPIO_Pin_9
			},
			{
					.port = GPIOE,
					.id = GPIO_Pin_10
			},
			{
					.port = GPIOE,
					.id = GPIO_Pin_11
			},
			{
					.port = GPIOE,
					.id = GPIO_Pin_12
			},
			{
					.port = GPIOE,
					.id = GPIO_Pin_14
			}
	};

	Engine::Engine(Engines::Enum id) :
		id(id),
		m_direction(DirPinsConfig[id].port, DirPinsConfig[id].id),
		m_timer(Util::Time::FromMilliSeconds(10))
	{
		m_timer.start();
		m_pwm.write(enginesToPwms(id), 0);
	}

	void Engine::start()
	{
		s_engineState.setFlag(States::IsRunning + (id << States::Offset));
	}

	void Engine::stop()
	{
		s_engineState.resetFlag(States::IsRunning + (id << States::Offset));
	}

	bool Engine::isRunning() const
	{
		return s_engineState.flag(States::IsRunning + (id << States::Offset));
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

	void Engine::setTargetDirection(Util::Dirs::Enum direction)
	{
		s_engineState.setFlagTo(States::IsBackward + (id << States::Offset), directionToPin(direction));
	}

	Util::Dirs::Enum Engine::getTargetDirection() const
	{
		return pinToDirection(s_engineState.flag(States::IsBackward + (id << States::Offset)));
	}

	Util::Dirs::Enum Engine::getCurrentDirection() const
	{
		return pinToDirection(m_direction.readPin());
	}

	void Engine::update()
	{
		if(m_timer.run()) {
			if(isRunning()) {
				if(getCurrentDirection() != getTargetDirection())
					turnAround();
				else
					moveInDirection();
			}
			else if(getCurrentSpeed())
				setCurrentSpeed(0);
		}
	}

	void Engine::speedUp()
	{
		setCurrentSpeed(getCurrentSpeed() + 1);
	}

	void Engine::slowDown()
	{
		setCurrentSpeed(getCurrentSpeed() - 1);
	}

	void Engine::turnAround()
	{
		if(getCurrentSpeed())
			slowDown();
		else
			setCurrentDirection(getTargetDirection());
	}

	void Engine::moveInDirection()
	{
		if(getCurrentSpeed() < getTargetSpeed())
			speedUp();
		else if(getCurrentSpeed() > getTargetSpeed())
			slowDown();
	}

	void Engine::setCurrentSpeed(uint8_t speed)
	{
		m_pwm.write(enginesToPwms(id), speed);
	}

	void Engine::setCurrentDirection(Util::Dirs::Enum direction)
	{
		m_direction.setPinTo(directionToPin(direction));
	}
}
