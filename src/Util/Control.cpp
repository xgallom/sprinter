/*
 * Control.cpp
 *
 *  Created on: Jul 23, 2018
 *      Author: Zahorack
 */

#include <Util/Trace.h>
#include "Util/Control.h"
#include "Application.h"
#include "Util/Timer.h"

namespace Util {

ControlData ctrlData;
LOGGER_MODULE(Application)

enum modes : uint8_t{
  vehicle_mode = 0,
  printing_mode
  };

static 	uint8_t s_mode = 0x00;
static  bool steper_state = false;
static  bool dataOK = false;

Control::Control():
	m_engine1(Periph::Engines::M1),
	m_engine2(Periph::Engines::M2),
	m_engine3(Periph::Engines::M3),
	m_engine4(Periph::Engines::M4),
	m_engine5(Periph::Engines::M5),
	m_engine6(Periph::Engines::M6),
	m_engine7(Periph::Engines::M7),
	m_servo1(Periph::Servos::Servo1),
	m_servo2(Periph::Servos::Servo2),
	m_stepper1(Periph::Steppers::Stepper1),
	m_watchdog(Util::Time::FromMilliSeconds(100)),
	rfModule(Periph::Usarts::Usart2, 9600)

{
	m_watchdog.start();
	m_engine1.setTargetSpeed(0);
	m_engine2.setTargetSpeed(0);
	m_engine3.setTargetSpeed(0);
	m_engine4.setTargetSpeed(0);
	m_engine5.setTargetSpeed(0);
	m_engine6.setTargetSpeed(0);
	m_engine7.setTargetSpeed(0);
}

Control::~Control(){}


void Control::setRightSideSpeed(uint8_t speed){
	m_engine1.setTargetSpeed(speed);
	m_engine2.setTargetSpeed(speed);
	m_engine3.setTargetSpeed(speed);
}

void Control::setLeftSideSpeed(uint8_t speed){
	m_engine4.setTargetSpeed(speed);
	m_engine5.setTargetSpeed(speed);
	m_engine6.setTargetSpeed(speed);
}

void Control::setRightSideDirection(Periph::Dirs::Enum dir){
	m_engine1.setTargetDirection(dir);
	m_engine2.setTargetDirection(dir);
	m_engine3.setTargetDirection(dir);
}

void Control::setLeftSideDirection(Periph::Dirs::Enum dir){
	m_engine4.setTargetDirection(dir);
	m_engine5.setTargetDirection(dir);
	m_engine6.setTargetDirection(dir);
}

void Control::stopEngines(){
	m_engine1.setTargetSpeed(0);
	m_engine2.setTargetSpeed(0);
	m_engine3.setTargetSpeed(0);
	m_engine4.setTargetSpeed(0);
	m_engine5.setTargetSpeed(0);
	m_engine6.setTargetSpeed(0);
	m_engine7.setTargetSpeed(0);
}

void Control::updateEngines(){
	m_engine1.update();
	m_engine2.update();
	m_engine3.update();
	m_engine4.update();
	m_engine5.update();
	m_engine6.update();
	m_engine7.update();

}

void Control::stopServos(){
	m_servo1.hardStop();
	m_servo2.hardStop();
}
void Control::startServos(){
	m_servo1.hardStart();
	m_servo2.hardStart();
}

void Control::switchMode(){
	stopEngines();
	s_mode = ctrlData.mode;
	steper_state = false;
//	if(s_mode == printing_mode){
//		startServos();
//	}
//	else if(s_mode == vehicle_mode){
//		stopServos();
//	}
}


void Control::run(){


	if(steper_state && s_mode == vehicle_mode){
		m_stepper1.run();
	DTRACE("Stepper RUN");
	}
	else {
		//m_stepper1.disable();
	}

	if(m_watchdog.run()){
		m_disconnectedTime++;
		//TRACE("disconnectedTime: %d \r\n", m_disconnectedTime);
	}

	if(rfModule.Available())
		if(rfModule.bytesAvailable() >= sizeof(ctrlData) +1) {
			rfModule.readBytesUntil(';', (uint8_t *)&ctrlData, sizeof(ctrlData) +1);

			dataOK = false;
			uint8_t DataCRC = m_packet.calc_crc8((uint8_t *)&ctrlData, sizeof(ctrlData) -1);

			if(ctrlData.data_crc == DataCRC){
				m_disconnectedTime = 0;

				dataOK = true;
				if(ctrlData.mode != s_mode) switchMode();

				TRACE("right: %d  ",ctrlData.x);
				TRACE("left: %d  ",ctrlData.y);
				TRACE("state: %d  ",ctrlData.state);
//				TRACE("button L: %d  ",ctrlData.button_left);
//				TRACE("button R: %d  ",ctrlData.button_right);
//				TRACE("mode: %d  ",ctrlData.mode);
				TRACE("POT: %d \r\n",ctrlData.pot);
			}

	}
}
void Control::updatePrintingData(){

	uint8_t sensitivity = ctrlData.pot;
	m_engine7.setTargetSpeed(0);


	if(ctrlData.button_left){
		m_engine7.setTargetSpeed(sensitivity);
		m_engine7.setTargetDirection(Periph::Dirs::Backward);
	}

	if(ctrlData.button_right){
		m_engine7.setTargetSpeed(sensitivity);
		m_engine7.setTargetDirection(Periph::Dirs::Forward);
	}

	if(ctrlData.x == 100) m_servo1.incrementAngle();
	else if(ctrlData.x == 0) m_servo1.decrementAngle();

	if(ctrlData.y > 90) m_servo2.incrementAngle();
	else if(ctrlData.y < 10) m_servo2.decrementAngle();
}

void Control::updateVehicleData(){

	int8_t right_speed =0, left_speed =0;

	if(ctrlData.x > (JOYSTICK_MIDDLE + JOYSTICK_TRESHOLD)){
		setRightSideDirection(Periph::Dirs::Forward);
		right_speed = ctrlData.x - JOYSTICK_MIDDLE;
	}
	else if(ctrlData.x < (JOYSTICK_MIDDLE - JOYSTICK_TRESHOLD)){
		setRightSideDirection(Periph::Dirs::Backward);
		right_speed = JOYSTICK_MIDDLE - ctrlData.x;
	}
	else right_speed = 0;

	if(ctrlData.y > (JOYSTICK_MIDDLE + JOYSTICK_TRESHOLD)){
		setLeftSideDirection(Periph::Dirs::Forward);
		left_speed = ctrlData.y - JOYSTICK_MIDDLE;
	}
	else if(ctrlData.y < (JOYSTICK_MIDDLE - JOYSTICK_TRESHOLD)){
		setLeftSideDirection(Periph::Dirs::Backward);
		left_speed = JOYSTICK_MIDDLE - ctrlData.y;
	}
	else left_speed = 0;

	setRightSideSpeed(tool.clamp(right_speed, 0, 100));
	setLeftSideSpeed(tool.clamp(left_speed, 0, 100));

	steper_state = false;

	if(ctrlData.button_left){
		steper_state = true;
		m_stepper1.setCurrentDirection(Periph::Dirs::Backward);
	}

	if(ctrlData.button_right){
		steper_state = true;
		m_stepper1.setCurrentDirection(Periph::Dirs::Forward);

	}
}

void Control::update()
{
	if(!(ctrlData.state) || m_disconnectedTime >= 10){		//main STOP button on Joystick
		stopEngines();
		m_stepper1.disable();
		TRACE("DISCONNECTED\r\n");
	}
	else if(dataOK){
			if(ctrlData.mode == vehicle_mode) updateVehicleData();
			else updatePrintingData();
		}

	updateEngines();
}


void Control::test() {

	if(m_engine1.getCurrentSpeed() == m_engine1.getTargetSpeed()){
			m_engine1.setTargetDirection(m_engine1.getCurrentDirection() ? Periph::Dirs::Forward : Periph::Dirs::Backward);
		}

	update();
}

} /* namespace Util */
