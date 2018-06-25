/*
 * Timer.cpp
 *
 *  Created on: Jun 25, 2018
 *      Author: xgallom
 */

#include <Util/Timer.h>

namespace Util {

Timer::Timer(Time a_interval) :
	m_currentTime(),
	m_isRunning(false),
	m_interval(a_interval),
	m_lastTimeTriggered(Util::Time::ZeroTime())
{}

void Timer::start()
{
	m_isRunning = true;
	wasTriggeredNow();
}

void Timer::stop()
{
	m_isRunning = false;
}

Time Timer::interval() const
{
	return m_interval;
}

void Timer::setInterval(Time a_interval)
{
	m_interval = a_interval;
}

bool Timer::shouldUpdate() const
{
	return m_isRunning && m_currentTime.sinceLastReset() >= m_lastTimeTriggered + m_interval;
}

void Timer::wasTriggeredNow()
{
	m_lastTimeTriggered = m_currentTime.sinceLastReset();
}

bool Timer::run()
{
	bool retval = shouldUpdate();

	if(retval)
		wasTriggeredNow();

	return retval;
}

} /* namespace Util */
