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
	id(id)
{}

void Engine::setSpeed(uint8_t value)
{
	speed = value;
}

uint8_t Engine::getSpeed()
{
	return speed;
}

void setDirection(Dir::Enum direction)
{
}

void Engine::run()
{
	pwm.write(Pwms::Enum(id), speed);
}

void Engine::run(uint8_t value)
{
	speed = value;
	pwm.write(Pwms::Enum(id), speed);
}

void Engine::run(uint8_t value, Dir::Enum direction)
{
	setDirection(direction);
	speed = value;
	pwm.write(Pwms::Enum(id), speed);
}

} /* namespace Periph */
