/*
 * Comunication.h
 *
 *  Created on: 2.9. 2018
 *      Author: Zahorack
 */

#ifndef UTIL_COMMUNICATION_H_
#define UTIL_COMMUNICATION_H_

#include <Container/Result.h>
#include "stm32f4xx.h"
#include "Periph/Usart.h"
#include "Control/Packet.h"

namespace Control
{
	static constexpr uint16_t
		JostickQuadrantOffset = 70,
		JoystickMiddle = 500,
		JoystickTreshold = 7;

	class Communication {
		Periph::Usart m_rfModule;

		enum State {
			WaitingForNextPacket,
			ReadingPacketHeader,
			ReadingPacketContents
		};

		State m_state = WaitingForNextPacket;
		Packet m_currentPacket;

	public:
		Communication();

		Container::Result<Packet> update();
		void sendStatus();

	private:
		void waitForNextPacket();
		void readPacketHeader();
		Container::Result<Packet> readPacketContents();
		bool checkCrc();
	};
}
#endif /* UTIL_PACKET_H_ */
