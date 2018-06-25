/*
 * Timer.h
 *
 *  Created on: Jun 25, 2018
 *      Author: xgallom
 */

#ifndef UTIL_TIMER_H_
#define UTIL_TIMER_H_

#include "Util/Time.h"
#include "Periph/SysTickCounter.h"

namespace Util {

class Timer {
	Periph::SysTickCounter m_currentTime;

	bool m_isRunning;
	Time m_interval, m_lastTimeTriggered;

public:
	explicit Timer(Time a_interval = Util::Time::ZeroTime());

	void start();
	void stop();

	Time interval() const;
	void setInterval(Time a_interval);

	bool shouldUpdate() const;

	void wasTriggeredNow();

	bool run();
};

} /* namespace Util */

#endif /* UTIL_TIMER_H_ */
