/*
 * Odometry.h
 *
 *  Created on: Februry 23, 2019
 *      Author: Zahorack
 */

#ifndef ODOMETRY_H_
#define ODOMETRY_H_

#include "stm32f4xx.h"
#include "Util/Trace.h"
#include "Periph/Encoder.h"
#include "Periph/Engine.h"
#include "Util/Timer.h"
#include "Localisation/Coordinates.h"

namespace Util {

//TODO: gyroskop fusion

class Odometry {
	Periph::Encoder m_leftEncoder, m_rightEncoder;

	Localisation::Coordinates m_coords;

	Util::Timer m_timer;

	Localisation::Coordinates computeNewCoords(int32_t, int32_t);
	float computeAngleDif(int32_t, int32_t);
public:
	Odometry(Periph::Encoder (&Encoders)[6]);

	Localisation::Coordinates getLocation();	// relative position in inertial coordinate system since last saved point
	void saveLocation(Localisation::Coordinates point);

	//void calibrate();
	void update();
	void reset();






};

}

#endif