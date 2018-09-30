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
#include "Util/Packet.h"

namespace Util {



class Communication {

	Periph::Usart m_rfModule;
	Util::Packet  m_packet;
public:
	Communication();
	~Communication();

	void start();
	void run();

	void parse();
	void sendStatus();

};


}
#endif /* UTIL_PACKET_H_ */
