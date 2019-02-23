/*
 * Encoder.h
 *
 *  Created on: 12.2.2019
 *      Author: Zahorack
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include "stm32f4xx.h"
#include "Util/Timer.h"

namespace Periph {


namespace EncoderPins {
enum Enum : uint8_t {
	EncoderPin1 = 0,
	EncoderPin2,
	EncoderPin3,
	EncoderPin4,
	EncoderPin5,
	EncoderPin6,

	Size
};
}
class Encoder {
	const EncoderPins::Enum id;

	uint32_t m_timerCounter;
	Util::Timer m_timer;

public:
	Encoder(EncoderPins::Enum a_id);

	void initGpio();
	void initNvic();
	void initExti();

	void handler();
	void update();
	uint32_t getCounter();
	uint32_t getPeriod();
	uint8_t getAngularSpeed();
};

}

#endif