/*
 * Coordinates.cpp
 *
 *  Created on: Februry 23, 2019
 *      Author: Zahorack
 */

#include "Localisation/Coordinates.h"

namespace Localisation {

void Coordinates::add(Coordinates add)
{
	m_x += add.m_x;
	m_y += add.m_y;
	m_F += add.m_F;

	if(m_F > 360) {
		m_F -= 360;
	}
}

}