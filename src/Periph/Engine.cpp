/*
 * Engine.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: Zahorack
 */

#include "Engine.h"

namespace Periph {

Engine::Engine(Engines::Enum id) :
	pwm(1000),
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

void setDirection(Directions::Enum direction)
{
}

void Engine::run()
{
	pwm.write(Pwms::Enum(id), m_speed);
}

void Engine::run(uint8_t speed)
{
	setSpeed(speed);
	run();
}

void Engine::run(uint8_t speed, Directions::Enum direction)
{
	setDirection(direction);
	setSpeed(speed);
	run();
}

} /* namespace Periph */
