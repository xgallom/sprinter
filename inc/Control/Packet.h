/*
 * Packet.h
 *
 *  Created on: 2.9. 2018
 *      Author: Zahorack
 */

#ifndef CONTROL_PACKET_H_
#define CONTROL_PACKET_H_

#include "stm32f4xx.h"

namespace Control
{
	class PacketHeader {
	public:
		uint16_t	mark;
		uint8_t		type;
		uint16_t	dataLength;
		uint8_t		crc;
	} __attribute__((packed));


	struct DataPacket {
		PacketHeader header;
		uint8_t crc;
		uint8_t *data;
	} __attribute__((packed));


	struct StatusPacket {
		uint32_t	uptime;
		uint16_t	batteryVoltage;
	} __attribute__((packed));

	namespace PacketType
	{
		enum Enum {
			Status = 0,
			Busy,
			Error,
			Ack,
			AckData,
			AckHeader,
			ControllerData,
			MoveData,
			PrintData,
			Nack
		};
	}

	class Packet {
	public:
		static const uint16_t Mark = 0x4B6B;

		PacketHeader header;
		DataPacket	data;

		static uint8_t CalculateCRC8(uint8_t* data, uint16_t len);
	};
}

#endif /* CONTROL_PACKET_H_ */
