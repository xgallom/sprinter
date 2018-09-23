/*
 * Pwm.h
 *
 *  Created on: Jun 18, 2018
 *      Author: Zahorack
 */


#ifndef PERIPH_PWM_H_
#define PERIPH_PWM_H_

#include "stm32f4xx.h"
#include "Util/RefCounter.h"

namespace Periph {

namespace Pwms {
enum Enum : uint8_t{
	Pwm1 = 0,
	Pwm2,
	Pwm3,
	Pwm4,
	Pwm5,
	Pwm6,
	Pwm7,
	Pwm8,
	Pwm9,
	Pwm10,
	Pwm11,
	Pwm12,

	Size
};
} /* namespace Pwm */

class Pwm {
	static Util::RefCounter s_refCounter;

	void initRCC();
	void initGpio();
	void initTimOC(uint32_t frequency);
	void initTimTB(uint32_t frequency);
	void initPwm();
	void deinitPwm();

public:
	Pwm(uint32_t frequency);
	~Pwm();

	void write(Pwms::Enum id, uint16_t value);
	uint16_t read(Pwms::Enum id) const;
	void deintServoPwm();
	void intServoPwm();
};

} /* namespace Periph */

#endif /* PERIPH_PWM_H_ */
