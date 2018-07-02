/*
 * I2C.h
 *
 *  Created on: Jun 28, 2018
 *      Author: Zahorack
 */

#ifndef PERIPH_I2C_H_
#define PERIPH_I2C_H_

#include "stm32f4xx.h"

namespace Periph {

namespace I2Cs {
enum Enum : uint8_t {
	I2Cx1 = 0,
	I2Cx2,

	Size
};
}

class I2C {
	const I2Cs::Enum id;

	void initGpio();
	void initRcc();
	void initI2C();
	void initNvic();

	bool startCondition();
	void stopCondition();

	bool start(uint8_t slaveAddress, uint8_t regAddress);
	bool startRead(uint8_t slaveAddress, uint8_t regAddress);
	bool checkEvent(uint32_t eventId) const;
	bool sendRegisterAddress(uint8_t regAddress);
	bool sendSlaveAddressForWrite(uint8_t slaveAddress);
	bool sendSlaveAddressForRead(uint8_t slaveAddress);

public:
	I2C(I2Cs::Enum id);


	bool read(uint8_t slaveAddress, uint8_t regAddress, uint8_t *buff, uint16_t count);
	bool write(uint8_t slaveAddress, uint8_t regAddress, uint8_t *buff, uint16_t count);
	bool write(uint8_t slaveAddress, uint8_t regAddress, uint8_t data);

};
}
#endif /* PERIPH_I2C_H_ */
