/*
 * Comunication.cpp
 *
 *  Created on: 2.9. 2018
 *      Author: Zahorack
 */

#include "Util/Communication.h"
#include <Util/Trace.h>


namespace Util {

Communication::Communication() :
	m_rfModule(Periph::Usarts::Usart2, 9600)
{}

void Communication::start(){}

void Communication::run()
{
	if(m_rfModule.bytesAvailable() >= 2){
		uint16_t mark = m_rfModule.readWord();

		if(mark == m_packet.mark){
			m_packet.parse();
		}
	}


}

void Communication::parse(){

	if(m_rfModule.bytesAvailable() >= sizeof(m_packet.header -2)){
		m_rfModule.readBytes((uint8_t *)&m_packet.header +2, sizeof(m_packet.header -2));
	}

	switch (m_packet.header->type){

	case packet_type_status :  break;
	case packet_type_bussy :  break;
	case packet_type_error :  break;
	case packet_type_ack :  break;
	case packet_type_data_ack :  break;
	case packet_type_header_ack :  break;
	case packet_type_controller_data :  break;
	case packet_type_move_data :  break;
	case packet_type_print_data :  break;
	case packet_type_nack :  break;

	default:
		DTRACE("BAD PACKET TYPE !");
		break;
	}
}

}
