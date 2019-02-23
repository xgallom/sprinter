/*
 * Pid.cpp
 *
 *  Created on: Februry 20, 2019
 *      Author: Zahorack
 */

#include "Util/Pid.h"

namespace Util {


void Pid::setParameters(PidArgs_t args)
{
	m_args.Kd = args.Kd;
	m_args.Ki = args.Ki;
	m_args.Kp = args.Kp;
	m_args.dt = args.dt;
	m_args.max = args.max;
	m_args.min = args.min;
}

uint8_t Pid::process(uint8_t set_value, uint8_t feedback_value)
{
	// Calculate error
	float error = set_value - feedback_value;

	// Proportional term
	float Pout = m_args.Kp * error;

	// Integral term
	integral += error * m_args.dt;
	float Iout = m_args.Ki * integral;

	// Derivative term
	float derivative = (error - pre_error) / m_args.dt;
	float Dout = m_args.Kd * derivative;

	// Calculate total output
	uint8_t output = (uint8_t)(Pout + Iout + Dout);

	// Restrict to max/min
	if( output > m_args.max ) {
		output = m_args.max;
	}
	else if( output < m_args.min) {
		output = m_args.min;
	}

	// Save error to previous error
	pre_error = error;

	return output;
}

} /* Namespace Util*/