/*
 * Engine.h
 *
 *  Created on: Jun 18, 2018
 *      Author: Zahorack
 */

#ifndef PERIPH_ENGINE_H_
#define PERIPH_ENGINE_H_

#include <Periph/DigitalOutputPin.h>
#include "stm32f4xx.h"
#include "Pwm.h"
#include "Util/Timer.h"

namespace Periph {

namespace Engines {
enum Enum : uint8_t {
	M1 = 0,
	M2,
	M3,
	M4,
	M5,
	M6,

	M7,	//differential

	Size
};

constexpr struct {
	GPIO_TypeDef *port;
	uint16_t id;
} DirPinsConfig[Engines::Size] = {
		{ /* M1 */
				port: GPIOE,
				id: GPIO_Pin_7
		},
		{ /* M2 */
				port: GPIOE,
				id: GPIO_Pin_8
		},
		{ /* M3 */
				port: GPIOE,
				id: GPIO_Pin_9
		},
		{ /* M4 */
				port: GPIOE,
				id: GPIO_Pin_10
		},
		{ /* M5 */
				port: GPIOE,
				id: GPIO_Pin_11
		},
		{ /* M6 */
				port: GPIOE,
				id: GPIO_Pin_12
		},
		{ /* M7 */
				port: GPIOE,
				id: GPIO_Pin_14
		}
};
}


namespace Dirs {
enum Enum : bool {
	Forward = true,
	Backward = false
};
}

class Engine {
	const Engines::Enum id;

	Pwm m_pwm;
	DigitalOutputPin m_direction;
	Util::Timer	m_timer;

	void turnAround();
	void moveInDirection();

	void slowDown();
	void speedUp();

public:
	Engine(Engines::Enum id);

	void start();
	void stop();
	bool isRunning() const;

	void setCurrentDirection(Dirs::Enum direction);
	void setTargetSpeed(uint8_t speed);
	void setCurrentSpeed(uint8_t speed);
	uint8_t getTargetSpeed() const;
	uint8_t getCurrentSpeed() const;

	void setTargetDirection(Dirs::Enum direction);
	Dirs::Enum getTargetDirection() const;
	Dirs::Enum getCurrentDirection() const;

	void update();
};

} /* namespace Periph */

#endif /* PERIPH_ENGINE_H_ */
