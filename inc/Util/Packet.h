/*
 * Packet.h
 *
 *  Created on: 2.9. 2018
 *      Author: Zahorack
 */

#ifndef UTIL_PACKET_H_
#define UTIL_PACKET_H_

#include "stm32f4xx.h"

namespace Util {

enum packet_type{
	packet_type_status = 1,
	packet_type_bussy,
	packet_type_error,
	packet_type_ack,
	packet_type_data_ack,
	packet_type_header_ack,
	packet_type_controller_data,
	packet_type_move_data,
	packet_type_print_data,
	packet_type_nack
};

class Packet {



public:
	uint8_t calc_crc8(uint8_t* data, uint16_t len);

};


}
#endif /* UTIL_PACKET_H_ */
