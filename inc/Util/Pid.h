/*
 * Pid.h
 *
 *  Created on: Februry 20, 2019
 *      Author: Zahorack
 */

#ifndef PID_H_
#define PID_H_

#include "stm32f4xx.h"

namespace Util {

typedef struct {
	float	Kp;
	float	Ki;
	float	Kd;
	float	dt;
	float	min;
	float	max;
}PidArgs_t;

class Pid {

	const PidArgs_t *m_args;
	float	pre_error;
	float	integral;
public:


	Pid(const PidArgs_t *m_args);

	//void setParameters(PidArgs_t args);
	uint8_t process(uint8_t, uint8_t);

};

}

#endif