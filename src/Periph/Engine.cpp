/*
 * Engine.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: Zahorack
 */

#include "Periph/Engine.h"

namespace Periph {


static Pwms::Enum enginesToPwms(Engines::Enum id)
{
	return static_cast<Pwms::Enum>(id);
}

static DirPins::Enum enginesToDirPins(Engines::Enum id)
{
	return static_cast<DirPins::Enum>(id);
}


Engine::Engine(Engines::Enum id) :
	pwm(1000),
	dir(enginesToDirPins(id)),
	id(id),
	m_speed(0)
{}

void Engine::setSpeed(uint8_t value)
{
	m_speed = value;
}

uint8_t Engine::getSpeed()
{
	return m_speed;
}

void Engine::setDirection(Dirs::Enum direction)
{
	dir.update(direction);
}

void Engine::update()
{
	pwm.write(enginesToPwms(id), m_speed);
}

void Engine::update(uint8_t speed)
{
	setSpeed(speed);
	update();
}

void Engine::update(uint8_t speed, Dirs::Enum direction)
{
	setDirection(direction);
	setSpeed(speed);
	update();
}

void Engine::stop()
{
	m_speed = 0;
	update();
}

} /* namespace Periph */
