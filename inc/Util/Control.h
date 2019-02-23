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

	Periph::Engine 	m_engine1, m_engine2, m_engine3, m_engine4, m_engine5, m_engine6, m_engine7;
	Periph::Servo 	m_servo1, m_servo2;
	Periph::Stepper m_stepper1, m_stepper2;
	Util::Packet 	m_packet;
	Device::Suntracker m_suntracker;
	Timer 		m_watchdog;
	Util::Tool 	tool;
	uint32_t 	m_disconnectedTime = 0;
	bool 		m_state = true;

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

	void setRightSideSpeed(uint8_t speed);
	void setLeftSideSpeed(uint8_t speed);
	void setRightSideDirection(Periph::Dirs::Enum dir);
	void setLeftSideDirection(Periph::Dirs::Enum dir);
	void stopEngines();
	void updateEngines();
	void stopServos();
	void startServos();

	void switchMode();
	void test();

};

} /* namespace Util */

#endif /* UTIL_CONTROL_H_ */
