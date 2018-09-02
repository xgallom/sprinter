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
#include "Util/Packet.h"

struct ControlData{
  uint8_t  x  	 = 0x00;
  uint8_t  y  	 = 0x00;
  uint8_t   pot	 = 0x00;
  uint8_t   state = 0x00;
  uint8_t data_crc = 0x00;
};

extern ControlData ctrlData;

#define	JOYSTICK_MIDDLE		100
#define	JOYSTICK_TRESHOLD	7

namespace Util {

class Control {

	Periph::Engine m_engine1, m_engine2, m_engine3, m_engine4, m_engine5, m_engine6;
	Util::Packet m_packet;
public:
	Control();
	~Control();

	Periph::Usart rfModule;

	void update();
	void updateControllerData();
	void parseControllerData();
	void setRightSideSpeed(uint8_t speed);
	void setLeftSideSpeed(uint8_t speed);
	void setRightSideDirection(Periph::Dirs::Enum dir);
	void setLeftSideDirection(Periph::Dirs::Enum dir);
	int clamp(int val, int min, int max);

};

} /* namespace Util */

#endif /* UTIL_CONTROL_H_ */
