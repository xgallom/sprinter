/*
 * Dir.h
 *
 *  Created on: Jun 22, 2018
 *      Author: Zahorack
 */

#ifndef PERIPH_DIR_H_
#define PERIPH_DIR_H_

#include "stm32f4xx.h"
#include "../Util/RefCounter.h"

namespace Periph {

namespace DirPins {
enum Enum : uint8_t {
	DirPin1 = 0,
	DirPin2,
	DirPin3,
	DirPin4,
	DirPin5,
	DirPin6,

	Size
};
}

namespace Dirs {
enum Enum : bool {
	Forward = false,
	Backward = true
};
}

class Dir {

	static Util::RefCounter s_refCounter;
	const DirPins::Enum id;

	void initGpio();
	void deinitGpio();

public:
	Dir(DirPins::Enum id);
	~Dir();

	void update(Dirs::Enum dir);


};



} /* namespace Periph */

#endif /* PERIPH_DIR_H_ */
