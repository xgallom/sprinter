/*
 * Pid.cpp
 *
 *  Created on: Februry 20, 2019
 *      Author: Zahorack
 */

#include "Util/Pid.h"
#include <stdlib.h>
#include <string.h>
#include "Util/Trace.h"

namespace Util {

Pid::Pid(const PidArgs_t *p_args):
	m_args(p_args),
	pre_error(0),
	integral(0)
{}

//void Pid::setParameters(PidArgs_t args)
//{
////	memcpy(&m_args, &args, sizeof(m_args));
//
//	m_args.Kd = args.Kd;
//	m_args.Ki = args.Ki;
//	m_args.Kp = args.Kp;
//	m_args.dt = args.dt;
//	m_args.max = args.max;
//	m_args.min = args.min;
//}

uint8_t Pid::process(uint8_t set_value, uint8_t feedback_value)
{
	// Calculate error
	float error = set_value - feedback_value;

	// Proportional term
	float Pout = m_args->Kp * error;

	// Integral term
	if(integral < m_args->max) {
		integral += error * m_args->dt;	//upper limit Antiwindup
	}

	float Iout = m_args->Ki * integral;

	// Derivative term
	float derivative = (error - pre_error) / m_args->dt;
	float Dout = m_args->Kd * derivative;

	// Calculate total output
	int64_t output = (Pout + Iout + Dout);

	// Restrict to max/min
	if( output >= m_args->max ) {
		output = m_args->max;
	}
	else if( output < m_args->min) {
		output = m_args->min;
	}

	// Save error to previous error
	pre_error = error;

	//TRACE("error:%d int:%d der:%d P:%d I:%d D:%d\n\r", (int)error, (int)integral, (int)derivative, (int)Pout, (int)Iout, (int)Dout);

	return (uint8_t)output;
}

} /* Namespace Util*/