/*
 * Control.h
 *
 *  Created on: 2.9 2018
 *      Author: Zahorack
 */

#ifndef UTIL_CONTROL_H_
#define UTIL_CONTROL_H_

#include "stm32f4xx.h"
#include "Periph/Engine.h"
#include "Periph/Usart.h"
#include "Periph/Servo.h"
#include "Periph/Stepper.h"
#include "Periph/Adc.h"
#include "Util/Packet.h"
#include "Util/Timer.h"
#include "Util/Tool.h"
#include "Device/Suntracker.h"
#include "Periph/Encoder.h"
#include "Util/Odometry.h"
#include "Util/Pid.h"
#include "Periph/EngineGroup.h"


#define	JOYSTICK_MIDDLE		100
#define	JOYSTICK_TRESHOLD	7

struct ControlData{
  uint8_t  x  	 = 0x00;
  uint8_t  y  	 = 0x00;
  uint8_t  pot	 = 0x00;
  uint8_t  button_left = 0x00;
  uint8_t  button_right= 0x00;
  uint8_t  state   = 0x00;
  uint8_t  mode    = 0x00;
  uint8_t data_crc = 0x00;
};

extern ControlData ctrlData;

namespace Util {

class Control {

	Periph::Encoder m_encoders[6] = {
		Periph::Encoder(Periph::EncoderPins::EncoderPin1), Periph::Encoder(Periph::EncoderPins::EncoderPin2),
		Periph::Encoder(Periph::EncoderPins::EncoderPin3), Periph::Encoder(Periph::EncoderPins::EncoderPin4),
		Periph::Encoder(Periph::EncoderPins::EncoderPin5), Periph::Encoder(Periph::EncoderPins::EncoderPin6)
	};

	Periph::Engine m_engines[7] = {
		Periph::Engine(Periph::Engines::M1), Periph::Engine(Periph::Engines::M2),
		Periph::Engine(Periph::Engines::M3), Periph::Engine(Periph::Engines::M4),
		Periph::Engine(Periph::Engines::M5), Periph::Engine(Periph::Engines::M6),
		Periph::Engine(Periph::Engines::M7)
	};

	const Util::PidArgs_t m_pidArgs = {
		.Kp = 1,
		.Ki = 6,
		.Kd = 0.05,
		.dt = 0.1,
		.min = 0,
		.max = 99
	};

	Util::Pid m_pids[6] = {
		Util::Pid(&m_pidArgs), Util::Pid(&m_pidArgs), Util::Pid(&m_pidArgs),
		Util::Pid(&m_pidArgs), Util::Pid(&m_pidArgs), Util::Pid(&m_pidArgs)
	};

	Periph::Servo 	m_servo1, m_servo2;
	Periph::Stepper m_stepper1, m_stepper2;
	Util::Packet 	m_packet;
	Device::Suntracker m_suntracker;

	Util::Timer	m_timer;
	Util::Timer 	m_watchdog;
	Util::Tool 	tool;
	Util::Odometry	odometry;

	uint32_t 	m_disconnectedTime = 0;
	bool 		m_state = true;

	void setRightSideSpeed(uint8_t speed);
	void setLeftSideSpeed(uint8_t speed);
	void setRightSideDirection(Periph::Dirs::Enum dir);
	void setLeftSideDirection(Periph::Dirs::Enum dir);

public:
	Control();
	~Control();

	Periph::Usart rfModule;
	void update();
	void run();
	void stop();
	void start();

	void updateVehicleData();
	void updatePrintingData();
	void updateSunTrackerData();
	void updateSimulation();
	void taskManager(uint8_t task);

	void updateEncoders();
	void stopEngines();
	void updateEngines();
	void stopServos();
	void startServos();

	void switchMode();
	void test();

};

} /* namespace Util */

#endif /* UTIL_CONTROL_H_ */
