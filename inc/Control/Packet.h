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
	namespace PacketType
	{
		enum Enum : uint8_t {
			Nack = 0,
			Ack,
			Status,
			ManualControl
		};
	}

	struct __attribute__((packed)) PacketHeader {
		uint32_t id;
		PacketType::Enum type;
	};

	struct __attribute__((packed)) NackPacket {
	};

	struct __attribute__((packed)) AckPacket {
	};

	struct __attribute__((packed)) StatusPacket {
		uint32_t uptime;
		uint16_t batteryVoltage;
	};

	struct __attribute__((packed)) ManualControlPacket {
		uint8_t
			x,
			y,
			pot,
			buttonLeft,
			buttonRight,
			state,
			mode;
	};

	union PacketContents {
		NackPacket nackPacket;
		AckPacket ackPacket;
		StatusPacket statusPacket;
		ManualControlPacket dataPacket;
	};

	using Crc = uint8_t;

	class Packet {
	public:
		static constexpr uint16_t Mark = 0x4B6B;

		PacketHeader header;
		PacketContents contents;

		static uint32_t SizeForType(PacketType::Enum packetType);
		static Crc CalculateCRC8(const uint8_t *data, uint16_t length);

		template<typename T>
		static Crc CalculateCRC8(const T &data) {
			return Packet::CalculateCRC8(reinterpret_cast<const uint8_t *>(&data), sizeof(T));
		}

		template<PacketType::Enum Type>
		static constexpr Packet Create(uint32_t id) {
			return {
				{
					.id = id,
					.type = Type
				}
			};
		}
	};
}

#endif /* CONTROL_PACKET_H_ */
