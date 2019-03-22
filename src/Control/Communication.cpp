/*
 * Comunication.cpp
 *
 *  Created on: 2.9. 2018
 *      Author: Zahorack
 */

#include "Control/Communication.h"

namespace Control
{
	Communication::Communication() :
		m_rfModule(Periph::Usarts::Usart2, 9600)
	{}

	Container::Result<Packet> Communication::update()
	{
		switch(m_state) {
		case WaitingForNextPacket:
			waitForNextPacket();
			break;

		case ReadingPacketHeader:
			readPacketHeader();
			break;

		case ReadingPacketContents:
			return readPacketContents();
		}

		return Container::Result<Packet>();
	}

	void Communication::sendStatus() const
	{
	 // TODO
	}

	void Communication::waitForNextPacket()
	{
		while(m_rfModule.bytesAvailable() > sizeof(Packet::Mark)) {
			if(m_rfModule.readWord() == Packet::Mark) {
				m_state = ReadingPacketHeader;
				break;
			}
		}
	}

	void Communication::readPacketHeader()
	{
		if(m_rfModule.bytesAvailable() >= sizeof(PacketHeader) + sizeof(Crc)) {
			m_rfModule.readStruct(m_currentPacket.header);

			if(checkCrc())
				m_state = ReadingPacketContents;
		}
	}

	Container::Result<Packet> Communication::readPacketContents()
	{
		if(m_rfModule.bytesAvailable() >= Packet::SizeForType(m_currentPacket.header.type) + sizeof(Crc)) {
			m_rfModule.readBytes(
					reinterpret_cast<uint8_t *>(&m_currentPacket.contents),
					Packet::SizeForType(m_currentPacket.header.type)
			);

			if(checkCrc()) {
				m_state = WaitingForNextPacket;

				return Container::Result<Packet>(m_currentPacket);
			}
		}

		return Container::Result<Packet>();
	}

	bool Communication::checkCrc()
	{
		Crc crc = m_rfModule.read();

		if(crc != Packet::CalculateCRC8(m_currentPacket.header)) {
			const PacketHeader nackPacket = {
					.id = m_currentPacket.header.id,
					.type = PacketType::Nack
			};

			m_rfModule.writeStruct(nackPacket);

			m_state = WaitingForNextPacket;

			return false;
		}

		return true;
	}
}
