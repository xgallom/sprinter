/*
 * Control.cpp
 *
 *  Created on: Jul 23, 2018
 *      Author: Zahorack
 */

#include "Util/Control.h"
#include "Application.h"
#include "Util/trace.h"



namespace Util {

ControlData ctrlData;
LOGGER_MODULE(Application)

Control::Control():
	m_engine1(Periph::Engines::M1),
	m_engine2(Periph::Engines::M2),
	m_engine3(Periph::Engines::M3),
	m_engine4(Periph::Engines::M4),
	m_engine5(Periph::Engines::M5),
	m_engine6(Periph::Engines::M6),

	rfModule(Periph::Usarts::Usart2, 9600)
{}

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

void Control::updateControllerData(){

	if(rfModule.bytesAvailable())
		if(rfModule.Available() >= sizeof(ctrlData) +1) {
			rfModule.readBytesUntil(';', (uint8_t *)&ctrlData, sizeof(ctrlData) +1);

//		TRACE("X: %d  ",ctrlData.x);
//		TRACE("Y: %d  ",ctrlData.y);
//		TRACE("POT: %d \r\n",ctrlData.pot);

	uint8_t DataCRC = m_packet.calc_crc8((uint8_t *)&ctrlData, sizeof(ctrlData) -1);

	if(ctrlData.data_crc == DataCRC){
		parseControllerData();
	}

//		TRACE("RX_CRC: %d  ",ctrlData.data_crc);
//		TRACE("DATA_CRC: %d \r\n", DataCRC);

	}
}

void Control::parseControllerData(){

	uint16_t right_speed =0, left_speed =0;

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

	setRightSideSpeed(clamp(right_speed, 0, 100));
	setLeftSideSpeed(clamp(left_speed, 0, 100));

	TRACE("right: %d  ",right_speed);
	TRACE("left: %d  ",left_speed);
	TRACE("state: %d  ",ctrlData.state);
	TRACE("POT: %d \r\n",ctrlData.pot);

}

void Control::update()
{
	if(ctrlData.state){		//main STOP button on Joystick
		m_engine1.update();
		m_engine2.update();
		m_engine3.update();
		m_engine4.update();
		m_engine5.update();
		m_engine6.update();
	}
	else{
		m_engine1.setTargetSpeed(0);
		m_engine2.setTargetSpeed(0);
		m_engine3.setTargetSpeed(0);
		m_engine4.setTargetSpeed(0);
		m_engine5.setTargetSpeed(0);
		m_engine6.setTargetSpeed(0);
	}
}

int Control::clamp(int val, int min, int max) {
    return val < min ? min : (val > max ? max : val);
}

} /* namespace Util */
