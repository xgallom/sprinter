/*
 *  Suntracker.cpp
 *
 *  Created on: January 6, 2019
 *      Author: Zahorack
 */

#include "Device/Suntracker.h"
#include "Util/Trace.h"

namespace Device {

static const uint16_t Samples = 100;
static const uint16_t FinnalPrecisionOffset = 300;
static const uint16_t PrecisionOffset = 20;

static const uint16_t  s_servoConstatnt = 500;	 	//first acceptable value in us
static const uint16_t  s_servoHomePosition = s_servoConstatnt + 1000;
static constexpr uint16_t  s_servoMaxPosition = s_servoConstatnt + 2000;
static constexpr uint16_t  s_servoMinPosition = s_servoConstatnt;

static uint16_t Intensity[5];

Suntracker::Suntracker() :
	m_servo1(Periph::Servos::Servo1),
	m_servo2(Periph::Servos::Servo2),
	m_timer(Util::Time::FromMilliSeconds(10))
{
	m_timer.start();
	m_servo2.addAngle(-100);
}

void Suntracker::measure()
{
	uint32_t temp = 0;

	for(int k=0; k < Kvadrants::Size; k++) {
		for(int i=0; i < Samples; i++) {
			temp += m_adcs[k].read();
		}
		Intensity[k] = temp/Samples;
		temp = 0;
 	}
//	TRACE("I: %d    II: %d    III: %d    IV: %d     O: %d   \n\r",
//		Intensity[Kvadrants::I], Intensity[Kvadrants::II], Intensity[Kvadrants::III],
//		Intensity[Kvadrants::IV], Intensity[Kvadrants::O]);
}

static uint16_t findMaxIntensity()
{
	uint16_t max = Intensity[Kvadrants::I];
	for(int k=1; k < Kvadrants::Size -1; k++) {
		if(Intensity[k] > max) {
			max = Intensity[k];
		}
	}
	return max;
}

void Suntracker::update()
{
	const uint8_t Step = 3;
	int16_t DirServo1, DirServo2;

	if(m_timer.run()) {
		measure();

		if(Intensity[Kvadrants::O] + FinnalPrecisionOffset > findMaxIntensity()) {
			//TRACE("Suntracker calibration OK\n\r");
		}
		else {
			if((Intensity[Kvadrants::I] + Intensity[Kvadrants::IV]) + PrecisionOffset >
			   (Intensity[Kvadrants::II] + Intensity[Kvadrants::III])) {
				DirServo2 = -1;
			}
			else if((Intensity[Kvadrants::I] + Intensity[Kvadrants::IV]) - PrecisionOffset <
			   	(Intensity[Kvadrants::II] + Intensity[Kvadrants::III])){
				DirServo2 = 1;
			}
			else DirServo2 =0;

			if((Intensity[Kvadrants::I] + Intensity[Kvadrants::II]) + PrecisionOffset>
			  (Intensity[Kvadrants::IV] + Intensity[Kvadrants::III])) {
				DirServo1 = 1;
			}
			else if((Intensity[Kvadrants::I] + Intensity[Kvadrants::II]) - PrecisionOffset <
			  	(Intensity[Kvadrants::IV] + Intensity[Kvadrants::III])) {
				DirServo1 = -1;
			}
			else DirServo1 =0;

			if(m_servo2.getCurrentAngle() < s_servoHomePosition) {
				DirServo1 *= -1;
			}
//			if(m_servo1.getCurrentAngle() > s_servoMaxPosition -200) {
//				m_servo1.addAngle(-m_servo1.getCurrentAngle());
//				DirServo2 *= -1;
//				DirServo1 *= -1;
//			}

			m_servo1.addAngle(DirServo1*Step);
			m_servo2.addAngle(DirServo2*Step);
		}
	}
}



}