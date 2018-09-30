/*
 * Packet.h
 *
 *  Created on: 2.9. 2018
 *      Author: Zahorack
 */

#ifndef UTIL_PACKET_H_
#define UTIL_PACKET_H_

#include "stm32f4xx.h"

typedef struct {
	uint16_t	mark;
	uint8_t		type;
	uint16_t	data_len;
	uint8_t		head_crc;
} __attribute__((packed)) Packet_Header;


typedef struct {
	Packet_Header 	header;
	uint8_t 	crc;
	uint8_t		*data;
} __attribute__((packed)) Data_Packet;


typedef struct {
	uint32_t	uptime;
	uint16_t	battery_voltage;
} __attribute__((packed)) Status_Packet;

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
	Packet_Header *header;
	Data_Packet	data;
	const uint16_t mark = 0x4B6B;

	uint8_t calc_crc8(uint8_t* data, uint16_t len);
	void parse();

};


}
#endif /* UTIL_PACKET_H_ */
