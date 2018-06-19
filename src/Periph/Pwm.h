/*
 * Pwm.h
 *
 *  Created on: Jun 18, 2018
 *      Author: Zahorack
 */


#ifndef PWM_H_
#define PWM_H_

#include "stm32f4xx.h"

#define	PWM_STEPS		  100
#define PULSE_1MS   	  180000  // 1ms pulse width
#define ESC_PULSE_PERIOD  5000    // pulse period (400Hz)

namespace Periph{

namespace Timers{
enum Enum : uint8_t{
	Timer1 = 0,
	Timer2,

	Size
};
} /* namespace TImers */

namespace Pwms{
enum Enum : uint8_t{
	Pwm1 = 0,
	Pwm2,
	Pwm3,
	Pwm4,
	Pwm5,
	Pwm6,
	Pwm7,
	Pwm8,


	Size
};
} /* namespace Pwm */


class Pwm {
	//const Pwms::Enum id;

	void initRCC();
	void initGpio();
	void initTimOC();
	void initTimTB(uint32_t frequency);
	void initPwm();
	void deinitPwm();


public:
	Pwm(uint32_t frequency);
	~Pwm();

	void write(Pwms::Enum id, uint8_t value);


};


} /* namespace Periph */



#endif /* PWM_H_ */
