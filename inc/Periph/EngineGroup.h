/*
 * EngineGroup.h
 *
 *  Created on: Nov 16, 2018
 *      Author: xgallom
 */

#ifndef PERIPH_ENGINEGROUP_H_
#define PERIPH_ENGINEGROUP_H_

#include "Periph/Engine.h"
#include "Util/State.h"

namespace Periph {

namespace EngineFlags {
enum Flags {
	Engine1 = 0,
	Engine2,
	Engine3,
	Engine4,
	Engine5,
	Engine6,
	Engine7
};
}

class EngineGroup {
	static const uint8_t Size = 3;

	Engine *m_engines[Size];

public:
	EngineGroup(Engine *engine1, Engine *engine2, Engine *engine3);

	void start();
	void stop();

	void setTargetSpeed(uint8_t speed);

	void setTargetDirection(Periph::Dirs::Enum direction);

	void update();
};

}

#endif /* PERIPH_ENGINEGROUP_H_ */