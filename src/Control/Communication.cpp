/*
 * Comunication.cpp
 *
 *  Created on: 2.9. 2018
 *      Author: Zahorack
 */

#include "Control/Communication.h"

namespace Control {

Communication::Communication() :
	m_rfModule(Periph::Usarts::Usart2, 9600)
{}

Container::OperationResult<ControlData> Communication::update()
{
	// TODO
	/*
	if(m_rfModule.bytesAvailable() >= sizeof(Control::PacketHeader)) {
		Control::PacketHeader header;

		m_rfModule.readBytes(reinterpret_cast<uint8_t *>(&header), sizeof(Control::PacketHeader))

		if(header.mark == Control::Packet::Mark) {
		}
	}
	*/

	if(m_rfModule.bytesAvailable() > sizeof(ControlData)) {
		ControlData controlData;

		m_rfModule.readStruct(controlData);

		if(controlData.end != ';' || controlData.dataCrc != Control::Packet::CalculateCRC8(reinterpret_cast<uint8_t *>(&controlData.data), sizeof(ControlData::Data)))
			return Container::OperationResult<ControlData>();

		return Container::OperationResult<ControlData>(controlData);
	}

	return Container::OperationResult<ControlData>();
}

void Communication::sendStatus()
{
 // TODO
}

}
