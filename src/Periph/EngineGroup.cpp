/*
 * EngineGroup.cpp
 *
 *  Created on: Nov 16, 2018
 *      Author: xgallom
 */

#include <Periph/EngineGroup.h>

namespace Periph {

EngineGroup::EngineGroup(Engine *engine1, Engine *engine2, Engine *engine3)
	: m_engines{engine1, engine2, engine3}
{}

void EngineGroup::start()
{
	for(uint8_t n = 0; n < Size; ++n)
		m_engines[n]->start();
}

void EngineGroup::stop()
{
	for(uint8_t n = 0; n < Size; ++n)
		m_engines[n]->stop();
}

void EngineGroup::setTargetSpeed(uint8_t speed)
{
	for(uint8_t n = 0; n < Size; ++n)
		m_engines[n]->setTargetSpeed(speed);
}

void EngineGroup::setTargetDirection(Periph::Dirs::Enum direction)
{
	for(uint8_t n = 0; n < Size; ++n)
		m_engines[n]->setTargetDirection(direction);
}

void EngineGroup::update()
{
	for(uint8_t n = 0; n < Size; ++n)
		m_engines[n]->update();
}

}