/*
 * Adc.h
 *
 *  Created on: January 6, 2019
 *      Author: Zahorack
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32f4xx.h"

namespace Periph {


namespace AdcPins {
enum Enum : uint8_t {
	AdcPin1 = 0,
	AdcPin2,
	AdcPin3,
	AdcPin4,
	AdcPin5,
	AdcPin6,
	AdcPin7,
	AdcPin8,
	AdcPin9,
	AdcPin10,
	AdcPin11,

	Size
};
}

class Adc {
	const AdcPins::Enum id;
public:
	Adc(AdcPins::Enum a_id);

	void initGpio();
	void initRCC();
	void initAdc();

	uint16_t read();

};

}

#endif