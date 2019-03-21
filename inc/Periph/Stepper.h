/*
 * Stepper.h
 *
 *  Created on: 17.9. 2018
 *      Author: Zahorack
 */

#ifndef PERIPH_STEPPER_H_
#define PERIPH_STEPPER_H_

#include "stm32f4xx.h"
#include "Periph/DigitalOutputPin.h"
#include "Periph/Pwm.h"
#include "Util/Timer.h"
#include "Util/Direction.h"

namespace Periph
{
	namespace Steppers {
		enum Enum : uint8_t {
			Stepper1 = 0,
			Stepper2,

			Size
		};
	}

	class Stepper {
		const Steppers::Enum id;

		DigitalOutputPin m_direction;
		DigitalOutputPin m_step;
		DigitalOutputPin m_enable;

		Util::Timer m_timer;

	public:
		Stepper(Steppers::Enum id);
		~Stepper();

		void enable();
		void disable();
		bool isEnabled() const;

		void resume();
		void pause();
		bool isRunning() const;

		void moveForever();
		void setRemainingSteps(uint32_t steps);
		void moveSteps(uint32_t steps);
		void cancelMovement();
		uint32_t getRemainingSteps() const;

		void setDirection(Util::Dirs::Enum direction);
		Util::Dirs::Enum getDirection() const;

		void update();
	};
}

#endif /* PERIPH_STEPPER_H_ */
