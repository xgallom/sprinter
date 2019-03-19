/*
 * Odometry.cpp
 *
 *  Created on: Februry 23, 2019
 *      Author: Zahorack
 */

#include "Util/Odometry.h"
#include "Util/Trace.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

namespace Util {

static const float VehicleWidth = 652.0;

/* Convert radians to degres */
static float radToDeg(float rad)
{
	return (float)(rad * (180.0 / 3.14));
}

/* Convert degrees to radians */
static float degToRad(float deg)
{
	return (float)(deg * (3.14 / 180.0));
}

Odometry::Odometry(Periph::Encoder (&Encoders)[6]):
		m_leftEncoder(Encoders[1]),
		m_rightEncoder(Encoders[5]),

		m_timer(Util::Time::FromMilliSeconds(1000))	//diskutabilne, kedy updatovat?
{
	m_timer.start();

	memset(&m_coords, 0, sizeof(m_coords));
}

void Odometry::update()
{
//	static int32_t lastLeftSideDistance;
//	static int32_t lastRightSideDistance;
//
//	lastLeftSideDistance += m_leftEncoder.getDistance();
//	lastRightSideDistance += m_rightEncoder.getDistance();

	m_leftEncoder.update();
	m_rightEncoder.update();

	if(m_timer.run()) {
		int32_t LeftSideDistance  = m_leftEncoder.getDistance();	//Wheels in the middle
		int32_t RightSideDistance = m_rightEncoder.getDistance();

		//TRACE("L: %d  R: %d\n\r", m_leftEncoder.getAngularSpeedInScale(), m_rightEncoder.getAngularSpeedInScale());

		m_coords.add(computeNewCoords(LeftSideDistance, RightSideDistance));	// Update position from start
		reset();

		//TRACE("left: %d right: %d   ",LeftSideDistance, RightSideDistance );
		TRACE("{XYPLOT|DATA|SeriesName|%d|%d}", (int)m_coords.m_x, (int)m_coords.m_y);
	}
}

float Odometry::computeAngleDif(int32_t left_distance, int32_t right_distance)
{
		return radToDeg((right_distance - left_distance) / VehicleWidth);
}

Localisation::Coordinates Odometry::computeNewCoords(int32_t left_distance , int32_t right_distance)
{
	Localisation::Coordinates newCoords;

	memset(&newCoords, 0, sizeof(newCoords));

	int32_t MiddleDistance = (left_distance + right_distance)/2;

	newCoords.m_x = MiddleDistance * std::cos(degToRad(m_coords.m_F));
	newCoords.m_y = MiddleDistance * std::sin(degToRad(m_coords.m_F));
	newCoords.m_F = computeAngleDif(left_distance, right_distance);
	//TRACE("nX: %d nY: %d nF: %d\n\r", (int)newCoords.m_x, (int)newCoords.m_y, (int)newCoords.m_F);

	return newCoords;
}

Localisation::Coordinates Odometry::getLocation()
{
	return m_coords;
}

void Odometry::reset()
{
	m_leftEncoder.reset();
	m_rightEncoder.reset();
}

}
