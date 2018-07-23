/*
 * I2C.cpp
 *
 *  Created on: Jun 28, 2018
 *      Author: Zahorack
 */

#include "Periph/I2C.h"
#include "Container/Queue.h"
#include "Util/State.h"

namespace Periph {
struct {
	GPIO_TypeDef *gpio;
	I2C_TypeDef *i2c;
	uint32_t ahb1Gpio, ahb1I2c;
	uint32_t sda, scl;
	uint8_t sdaSource, sclSource, gpioAf;
	IRQn irqnER;
	IRQn irqnEV;
} constexpr config[I2Cs::Size] = {
	{
			gpio: GPIOB,
			i2c: I2C1,
			ahb1Gpio: RCC_AHB1Periph_GPIOB,
			ahb1I2c: RCC_APB1Periph_I2C1,
			sda: GPIO_Pin_9,
			scl: GPIO_Pin_8,
			sdaSource: GPIO_PinSource9,
			sclSource: GPIO_PinSource8,
			gpioAf: GPIO_AF_I2C1,
			irqnER: I2C1_ER_IRQn,
			irqnEV: I2C1_EV_IRQn
	},	 {
			gpio: GPIOF,
			i2c: I2C2,
			ahb1Gpio: RCC_AHB1Periph_GPIOF,
			ahb1I2c: RCC_APB1Periph_I2C2,
			sda: GPIO_Pin_0,
			scl: GPIO_Pin_1,
			sdaSource: GPIO_PinSource0,
			sclSource: GPIO_PinSource1,
			gpioAf: GPIO_AF_I2C2,
			irqnER: I2C2_ER_IRQn,
			irqnEV: I2C2_EV_IRQn
	}
};

void I2C::initRcc()
{
	RCC_APB1PeriphClockCmd(config[id].ahb1I2c, ENABLE);
	RCC_AHB1PeriphClockCmd(config[id].ahb1Gpio, ENABLE);

}

void I2C::initGpio()
{
	GPIO_InitTypeDef gpioInitStruct = {
			GPIO_Pin: config[id].sda | config[id].scl,
			GPIO_Mode: GPIO_Mode_AF,
			GPIO_Speed: GPIO_High_Speed,
			GPIO_OType: GPIO_OType_OD,
			GPIO_PuPd: GPIO_PuPd_UP
	};

	GPIO_PinAFConfig(config[id].gpio, config[id].sdaSource, config[id].gpioAf);
	GPIO_PinAFConfig(config[id].gpio, config[id].sclSource, config[id].gpioAf);

	GPIO_Init(config[id].gpio, &gpioInitStruct);

}

void I2C::initI2C()
{
	//I2C_ITConfig(config[id].i2c, I2C_IT_EVT | I2C_IT_ERR, DISABLE);
	I2C_DeInit(config[id].i2c);
	I2C_Cmd(config[id].i2c, DISABLE);

	I2C_InitTypeDef  I2C_InitStructure = {
			I2C_ClockSpeed:	400000,
			I2C_Mode:	I2C_Mode_I2C,
			I2C_DutyCycle:	I2C_DutyCycle_2,
			I2C_OwnAddress1:	0x00,
			I2C_Ack:	I2C_Ack_Enable,
			I2C_AcknowledgedAddress:	I2C_AcknowledgedAddress_7bit
	  };

    I2C_Init(config[id].i2c, &I2C_InitStructure);
    I2C_Cmd(config[id].i2c, ENABLE);

}

void I2C::initNvic()
{
	  NVIC_InitTypeDef NVIC_InitStructure = {
			  NVIC_IRQChannel: static_cast<uint8_t>(config[id].irqnER | config[id].irqnEV),
			  NVIC_IRQChannelPreemptionPriority:	0,
			  NVIC_IRQChannelSubPriority:	0,
			  NVIC_IRQChannelCmd:	ENABLE

	  };
	  NVIC_Init(&NVIC_InitStructure);
}

I2C::I2C(I2Cs::Enum id) :
		id(id)
{
	initRcc();
	initGpio();
	initI2C();
	//initNvic();
}

bool I2C::startCondition()
{
	I2C_GenerateSTART(config[id].i2c, ENABLE);
	if (!checkEvent(I2C_EVENT_MASTER_MODE_SELECT))
		return false;

	return true;
}

void I2C::stopCondition()
{
	I2C_GenerateSTOP(config[id].i2c, ENABLE);
}

bool I2C::start(uint8_t slaveAddress, uint8_t regAddress)
{
	if(!startCondition()) return false;
	if(!sendSlaveAddressForWrite(slaveAddress)) return false;
	config[id].i2c->SR2;
	if(!sendRegisterAddress(regAddress)) return false;

	return true;
}

bool I2C::sendRegisterAddress(uint8_t regAddress)
{
	I2C_SendData(config[id].i2c, regAddress);
	if (!checkEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		return false;

	return true;
}

bool I2C::sendSlaveAddressForWrite(uint8_t slaveAddress)
{
	I2C_Send7bitAddress(config[id].i2c, slaveAddress, I2C_Direction_Transmitter);
	//config[id].i2c->DR = (slaveAddress << 1) | dir;
	if (!checkEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		return false;

	return true;
}

bool I2C::sendSlaveAddressForRead(uint8_t slaveAddress)
{
	I2C_Send7bitAddress(config[id].i2c, slaveAddress, I2C_Direction_Receiver);
	//config[id].i2c->DR = (slaveAddress << 1) | dir;
	if (!checkEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		return false;

	return true;
}

bool I2C::startRead(uint8_t slaveAddress, uint8_t regAddress)
{
	I2C_AcknowledgeConfig(config[id].i2c, ENABLE);
	if(!start(slaveAddress, regAddress))
		return false;

	if(!startCondition())
		return false;

	if(!sendSlaveAddressForRead(slaveAddress))
		return false;

	return true;
}

bool I2C::read(uint8_t slaveAddress, uint8_t regAddress, uint8_t *buff, uint16_t count)
{
	if(!startRead(slaveAddress, regAddress))
			return false;

	while(count--){
		if(!count){
	        I2C_AcknowledgeConfig(config[id].i2c,DISABLE);
	        stopCondition();
			}
	    if(!checkEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
	    	return false;

	    *buff++ = I2C_ReceiveData(config[id].i2c);
	}
	return true;
}

bool I2C::write(uint8_t slaveAddress, uint8_t regAddress, uint8_t data)
{
	if(!start(slaveAddress, regAddress))
		return false;

	I2C_SendData(config[id].i2c, data);

	if(!checkEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		return false;

	stopCondition();
	return true;
}

bool I2C::write(uint8_t slaveAddress, uint8_t regAddress, uint8_t *buff, uint16_t count)
{
	if(!start(slaveAddress, regAddress))
		return false;

	while(count--){
		I2C_SendData(config[id].i2c, *buff++);

		if(!checkEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
			return false;
	}
	stopCondition();
	return true;
}

bool I2C::checkEvent(uint32_t eventId) {

	static volatile uint32_t timeout = 180000;
	//TODO: timer timeout.
	while(!I2C_CheckEvent(config[id].i2c, eventId)) {
      if(!(timeout--))
        return false;
    }
    return true;

  }

} /* namespace Periph */
