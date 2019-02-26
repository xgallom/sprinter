/*
 * Coordinates.h
 *
 *  Created on: Februry 23, 2019
 *      Author: Zahorack
 */

#ifndef COORDINATES_H_
#define COORDINATES_H_

#include "stm32f4xx.h"

namespace Localisation {

class Coordinates {

public:
	float m_x, m_y;
	float m_F;

	void add(Coordinates add);
};

}

#endif