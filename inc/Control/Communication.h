/*
 * Comunication.h
 *
 *  Created on: 2.9. 2018
 *      Author: Zahorack
 */

#ifndef UTIL_COMMUNICATION_H_
#define UTIL_COMMUNICATION_H_

#include "stm32f4xx.h"
#include "Periph/Usart.h"
#include "Control/Packet.h"
#include "Container/OperationResult.h"

namespace Control {

static const uint16_t
	JostickQuadrantOffset = 70,
	JoystickMiddle = 500,
	JoystickTreshold = 7;

struct ControlData {
	struct Data {
		uint8_t
			x,
			y,
			pot,
			buttonLeft,
			buttonRight,
			state,
			mode;
	} data;

	uint8_t
		dataCrc,
		end;
} __attribute__((packed));

class Communication {
	Periph::Usart m_rfModule;

	enum State {
		WaitingForNextPacket,
		ReadingPacket
	};

	State m_state = WaitingForNextPacket;

public:
	Communication();

	Container::OperationResult<ControlData> update();
	void sendStatus();

private:
	void waitForNextPacket();
	Container::OperationResult<ControlData> readPacket();
};

}
#endif /* UTIL_PACKET_H_ */
