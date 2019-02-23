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
  sunTracker_mode,
  printing_mode,
  simulation_mode,

  modes_size
  };

static 	uint8_t s_mode = 0x00;
static  bool dataOK = false;

static constexpr uint8_t KVADRANT_OFFSET = 70;

/*simulacia tlace stvorcovej plochy*/
enum tasks : uint8_t{
  up = 0,
  right,
  down,
  left,

  tasks_size
  };

static uint8_t task = right;

static uint8_t nextTask(){
	task++;

	if(task >= tasks_size)
		task = up;

	return task;
}

void Control::taskManager(uint8_t l_task){
	switch(l_task){
		case down:
			m_stepper1.setTargetSteps(1000);
			m_stepper1.setCurrentDirection(Periph::Dirs::Backward);
			break;
		case left:
			m_stepper2.setTargetSteps(1000);
			m_stepper2.setCurrentDirection(Periph::Dirs::Backward);
			break;
		case up:
			m_stepper1.setTargetSteps(1000);
			m_stepper1.setCurrentDirection(Periph::Dirs::Forward);
			break;
		case right:
			m_stepper2.setTargetSteps(1000);
			m_stepper2.setCurrentDirection(Periph::Dirs::Forward);
			break;

	}
}
void Control::updateSimulation(){
	m_stepper1.start();
	m_stepper2.start();

	if(task == up || task == down){
		if(!m_stepper1.isBussy()){
			taskManager(nextTask());
		}
	}
	else if(task == right || task == left){
		if(!m_stepper2.isBussy())
			taskManager(nextTask());
	}

}

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
	m_stepper2(Periph::Steppers::Stepper2),
	m_watchdog(Util::Time::FromMilliSeconds(100)),
	rfModule(Periph::Usarts::Usart2, 9600)

{
	m_watchdog.start();
	m_servo2.start();
	m_servo2.addAngle(60);   //korekcia pociatocnej polohy
	stop();

}

Control::~Control(){
	stop();
}


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

void Control::switchMode(){

	stop();
	s_mode = ctrlData.mode;

}

void Control::stop(){
		stopEngines();
		m_stepper2.stop();
		m_stepper1.stop();
		m_servo1.stop();
		m_servo2.stop();

		m_state = false;
}

void Control::start(){
		//m_stepper2.start();
		//m_stepper1.start();
		m_servo1.start();
		m_servo2.start();

		m_state = true;
}

void Control::run(){

	m_suntracker.update();

	if(s_mode == printing_mode){
		m_stepper1.freeRun();
		m_stepper2.freeRun();

		task = right;
		m_stepper1.setTargetSteps(0);
		m_stepper2.setTargetSteps(0);
		DTRACE("Stepper RUN");
	}
	else if(s_mode == simulation_mode){
		m_stepper1.run();
		m_stepper2.run();

	}
	else  {
		m_stepper1.stop();
		m_stepper2.stop();
	}

	if(m_watchdog.run()){
		m_disconnectedTime++;
	}

if(rfModule.Available())
		if(rfModule.bytesAvailable() >= sizeof(ctrlData) +1) {
			rfModule.readBytesUntil(';', (uint8_t *)&ctrlData, sizeof(ctrlData) +1);

			dataOK = false;
			uint8_t dataCRC = m_packet.calc_crc8((uint8_t *)&ctrlData, sizeof(ctrlData) -1);

			if(ctrlData.data_crc == dataCRC){
				m_disconnectedTime = 0;
				dataOK = true;

				if(ctrlData.mode != s_mode)
					switchMode();

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

void Control::updateSunTrackerData(){

	uint8_t sensitivity = ctrlData.pot;
	m_engine7.setTargetSpeed(0);

	if(ctrlData.button_left){
		m_engine7.setTargetSpeed(sensitivity);

		if(m_engine7.getCurrentDirection() != Periph::Dirs::Backward)
			m_engine7.setTargetDirection(Periph::Dirs::Backward);
	}

	if(ctrlData.button_right){
		m_engine7.setTargetSpeed(sensitivity);

		if(m_engine7.getCurrentDirection() != Periph::Dirs::Forward)
			m_engine7.setTargetDirection(Periph::Dirs::Forward);
	}

	if(ctrlData.x > 90) m_servo1.incrementAngle();
	else if(ctrlData.x < 10) m_servo1.decrementAngle();

	if(ctrlData.y > 90) m_servo2.incrementAngle();
	else if(ctrlData.y < 10) m_servo2.decrementAngle();
}

void Control::updatePrintingData(){

	if(ctrlData.x > (JOYSTICK_MIDDLE + KVADRANT_OFFSET)){
		m_stepper2.softwareEnable();
		m_stepper2.setCurrentDirection(Periph::Dirs::Backward);
	}
	else if(ctrlData.x < (JOYSTICK_MIDDLE - KVADRANT_OFFSET)){
		m_stepper2.softwareEnable();
		m_stepper2.setCurrentDirection(Periph::Dirs::Forward);
	}
	else 	m_stepper2.softwareDisable();

	if(ctrlData.y > (JOYSTICK_MIDDLE + KVADRANT_OFFSET)){
		m_stepper1.softwareEnable();
		m_stepper1.setCurrentDirection(Periph::Dirs::Forward);
	}
	else if(ctrlData.y < (JOYSTICK_MIDDLE - KVADRANT_OFFSET)){
		m_stepper1.softwareEnable();
		m_stepper1.setCurrentDirection(Periph::Dirs::Backward);
	}
	else 	m_stepper1.softwareDisable();
}

void Control::updateVehicleData(){

	int8_t right_speed =0, left_speed =0;

	if(ctrlData.x > (JOYSTICK_MIDDLE + JOYSTICK_TRESHOLD)){
		setRightSideDirection(Periph::Dirs::Backward);
		right_speed = ctrlData.x - JOYSTICK_MIDDLE;
	}
	else if(ctrlData.x < (JOYSTICK_MIDDLE - JOYSTICK_TRESHOLD)){
		setRightSideDirection(Periph::Dirs::Forward);
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
}

void Control::update(){
	if(!(ctrlData.state) || m_disconnectedTime >= 10){		//main STOP button on Joystick
		stop();
		//TRACE("DISCONNECTED\r\n");
	}
	else if(dataOK){
		start();

		if(s_mode == vehicle_mode) updateVehicleData();
		else if(s_mode == sunTracker_mode) updateSunTrackerData();
		else if(s_mode == printing_mode) updatePrintingData();
		else if(s_mode == simulation_mode) updateSimulation();
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
