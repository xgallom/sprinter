/*
 * Encoder.cpp
 *
 *  Created on: 12.2.2019
 *      Author: Zahorack
 */

#include "Periph/Encoder.h"
#include "Periph/Engine.h"
#include "Util/Trace.h"
#include <Periph/SysTickCounter.h>

namespace Periph {

static const uint8_t  WheelRadius   		= 90;
static const uint16_t EncoderImpulsMaxDealy 	= 500;
static const uint32_t EncoderAngleOnePick  	= 3;
static const uint32_t FullCircleAngle 		= 360;

static const float Circumference = 2 * 3.14 * WheelRadius;
static const float OnePickDistance = (Circumference / FullCircleAngle) * EncoderAngleOnePick;

typedef struct {
        float x_est_last = 0;
        float P_last = 0;
} kalmanArgs_t;

struct {
	GPIO_TypeDef	*port;
	uint16_t 	pin;
	uint8_t		EXTIpinSource;
	uint32_t 	line;
	uint8_t 	IRQChannel;
}static const config[EncoderPins::Size] = {
		{ /* EncoderPin1 */
				.port = GPIOG,
				.pin = GPIO_Pin_0,
				.EXTIpinSource = EXTI_PinSource0,
				.line = EXTI_Line0,
				.IRQChannel = EXTI0_IRQn
		},
		{ /* EncoderPin2 */
				.port = GPIOG,
				.pin = GPIO_Pin_1,
				.EXTIpinSource = EXTI_PinSource1,
				.line = EXTI_Line1,
				.IRQChannel = EXTI1_IRQn
		},
		{ /* EncoderPin3 */
				.port = GPIOE,
				.pin = GPIO_Pin_2,
				.EXTIpinSource = EXTI_PinSource2,
				.line = EXTI_Line2,
				.IRQChannel = EXTI2_IRQn
		},
		{ /* EncoderPin4 */
				.port = GPIOE,
				.pin = GPIO_Pin_3,
				.EXTIpinSource = EXTI_PinSource3,
				.line = EXTI_Line3,
				.IRQChannel = EXTI3_IRQn
		},
		{ /* EncoderPin5 */
				.port = GPIOE,
				.pin = GPIO_Pin_4,
				.EXTIpinSource = EXTI_PinSource4,
				.line = EXTI_Line4,
				.IRQChannel = EXTI4_IRQn
		},
		{ /* EncoderPin6 */
				.port = GPIOE,
				.pin = GPIO_Pin_5,
				.EXTIpinSource = EXTI_PinSource5,
				.line = EXTI_Line5,
				.IRQChannel = EXTI9_5_IRQn
		}
};

volatile int32_t EncoderCounters[EncoderPins::Size];
volatile uint32_t EncoderPeriodMilis[EncoderPins::Size];
volatile uint32_t EncoderLastTickMilis[EncoderPins::Size];
kalmanArgs_t KalmanArgs[EncoderPins::Size] = {0};

Encoder::Encoder(EncoderPins::Enum a_id):
		id(a_id),
		m_timer(Util::Time::FromMilliSeconds(100))
{
	initGpio();
	initExti();
	initNvic();
	m_timer.start();
	reset();

	TRACE("Encoder init OK\n\r");
}

void Encoder::initGpio()
{
	uint32_t rccPort;

	if(config[id].port == GPIOA)
		rccPort = RCC_AHB1Periph_GPIOA;
	else if(config[id].port == GPIOB)
		rccPort = RCC_AHB1Periph_GPIOB;
	else if(config[id].port == GPIOC)
		rccPort = RCC_AHB1Periph_GPIOC;
	else if(config[id].port == GPIOD)
		rccPort = RCC_AHB1Periph_GPIOD;
	else if(config[id].port == GPIOE)
		rccPort = RCC_AHB1Periph_GPIOE;
	else if(config[id].port == GPIOF)
		rccPort = RCC_AHB1Periph_GPIOF;
	else if(config[id].port == GPIOG)
		rccPort = RCC_AHB1Periph_GPIOG;
	else if(config[id].port == GPIOH)
		rccPort = RCC_AHB1Periph_GPIOH;
	else if(config[id].port == GPIOI)
		rccPort = RCC_AHB1Periph_GPIOI;
	else if(config[id].port == GPIOJ)
		rccPort = RCC_AHB1Periph_GPIOJ;
	else if(config[id].port == GPIOK)
		rccPort = RCC_AHB1Periph_GPIOK;

	RCC_AHB1PeriphClockCmd(rccPort, ENABLE);

	/* Enable clock for SYSCFG */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	GPIO_InitTypeDef init = {
		/* Set pin as input */
		.GPIO_Pin = config[id].pin,
		.GPIO_Mode = GPIO_Mode_IN,
		.GPIO_Speed = GPIO_High_Speed,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_DOWN
	};

	GPIO_Init(config[id].port, &init);


	uint32_t sourcePort;

	if(config[id].port == GPIOA)
		sourcePort = EXTI_PortSourceGPIOA;
	else if(config[id].port == GPIOB)
		sourcePort = EXTI_PortSourceGPIOB;
	else if(config[id].port == GPIOC)
		sourcePort = EXTI_PortSourceGPIOC;
	else if(config[id].port == GPIOD)
		sourcePort = EXTI_PortSourceGPIOD;
	else if(config[id].port == GPIOE)
		sourcePort = EXTI_PortSourceGPIOE;
	else if(config[id].port == GPIOF)
		sourcePort = EXTI_PortSourceGPIOF;
	else if(config[id].port == GPIOG)
		sourcePort = EXTI_PortSourceGPIOG;
	else if(config[id].port == GPIOH)
		sourcePort = EXTI_PortSourceGPIOH;
	else if(config[id].port == GPIOI)
		sourcePort = EXTI_PortSourceGPIOI;
	else if(config[id].port == GPIOJ)
		sourcePort = EXTI_PortSourceGPIOJ;
	else if(config[id].port == GPIOK)
		sourcePort = EXTI_PortSourceGPIOK;

	SYSCFG_EXTILineConfig(sourcePort, config[id].EXTIpinSource);
}

void Encoder::initExti()
{
	EXTI_InitTypeDef init = {
		.EXTI_Line = config[id].line,
		.EXTI_Mode = EXTI_Mode_Interrupt,
		.EXTI_Trigger = EXTI_Trigger_Rising_Falling,
		.EXTI_LineCmd = ENABLE
	};

	EXTI_Init(&init);
}

void Encoder::initNvic()
{
	NVIC_InitTypeDef init = {
		.NVIC_IRQChannel = config[id].IRQChannel,
		/* Default priority Droup2 - 2bit preemp and 2bit sub */
		.NVIC_IRQChannelPreemptionPriority = 0x02,
		.NVIC_IRQChannelSubPriority = 0,//(id < 3 ? id : 3),
		.NVIC_IRQChannelCmd = ENABLE
	};
	/* Add to NVIC */
	NVIC_Init(&init);
}


int32_t Encoder::getCounter()
{
	return EncoderCounters[id];
}

/* Time between two encoder's signal rising - tick */
uint32_t Encoder::getPeriod()
{
	return EncoderPeriodMilis[id];
}

/* Time spend on one encoder tick
 *
 * __maximum value for 100% PWM duty is 91 [deg/s]
 */
uint8_t Encoder::getAngularSpeed()
{
	return (uint8_t)(EncoderAngleOnePick / (getPeriod() / 1000.0));
}

/* Return speed coresponding with engine speeds, resp. in percents [0 - 100 %]*/
uint8_t Encoder::getAngularSpeedInScale()
{
	return (uint8_t)((EncoderAngleOnePick / (getPeriod() / 1000.0)) * 1.1);
}

/* Needed when wheel isn't turning around */
void Encoder::update()
{
	if(m_timer.run()) {
		int64_t currentMilis = Get_Milis();

		if((currentMilis - EncoderLastTickMilis[id]) >= EncoderImpulsMaxDealy) {
			EncoderPeriodMilis[id] = (uint32_t)(currentMilis - EncoderLastTickMilis[id]);
		}
	}
}

void Encoder::reset()
{
	Periph::EncoderCounters[id] = 0;
}

int32_t Encoder::getDistance()
{
	return OnePickDistance * getCounter();
}

}  /* End of Periph namespace */


int KalmanFilter(int z_measured, Periph::EncoderPins::Enum id)
{
//        static float x_est_last = 0;
//        static float P_last = 0;
        float K;
        float P;
        float P_temp;
        float x_temp_est;
        float x_est;
        float Q = 0.07;//0.05; //0.25;
        float R = 3; //0.7;

        x_temp_est = Periph::KalmanArgs[id].x_est_last;
        P_temp = Periph::KalmanArgs[id].P_last + Q;

        K = P_temp * (1.0/(P_temp + R));

        x_est = x_temp_est + K * ((int)z_measured - x_temp_est);
        P = (1- K) * P_temp;

        Periph::KalmanArgs[id].P_last = P;
        Periph::KalmanArgs[id].x_est_last = x_est;

        return (int)x_est;
}

void EncoderHandler(Periph::EncoderPins::Enum id)
{
	int64_t currentMilis = Get_Milis();

	/* Pozor treba osetrovat, kvoli rychlim skokom na hrane */
	if((currentMilis - Periph::EncoderLastTickMilis[id]) > 20 ) {
		if(GPIO_ReadOutputDataBit(Periph::Engines::DirPinsConfig[id].port, Periph::Engines::DirPinsConfig[id].id) == Periph::Dirs::Forward) {
			Periph::EncoderCounters[id]++;
		}
		else {
			Periph::EncoderCounters[id]--;
		}
		Periph::EncoderPeriodMilis[id] = KalmanFilter((currentMilis - Periph::EncoderLastTickMilis[id]), id);
		Periph::EncoderLastTickMilis[id] = currentMilis;
	}
}

extern "C" void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {

	EncoderHandler(Periph::EncoderPins::EncoderPin1);

        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

extern "C" void EXTI1_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line1) != RESET) {

	EncoderHandler(Periph::EncoderPins::EncoderPin2);

        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}
extern "C" void EXTI2_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line2) != RESET) {

	EncoderHandler(Periph::EncoderPins::EncoderPin3);

        EXTI_ClearITPendingBit(EXTI_Line2);
    }
}

extern "C" void EXTI3_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line3) != RESET) {

	EncoderHandler(Periph::EncoderPins::EncoderPin4);

        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}

extern "C" void EXTI4_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line4) != RESET) {

	EncoderHandler(Periph::EncoderPins::EncoderPin5);

        EXTI_ClearITPendingBit(EXTI_Line4);
    }
}

extern "C" void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line5) != RESET) {

	EncoderHandler(Periph::EncoderPins::EncoderPin6);

        EXTI_ClearITPendingBit(EXTI_Line5);
    }
}