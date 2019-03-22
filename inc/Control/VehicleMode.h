/*
 * VehicleMode.h
 *
 *  Created on: Nov 16, 2018
 *      Author: xgallom
 */

#ifndef CONTROL_VEHICLEMODE_H_
#define CONTROL_VEHICLEMODE_H_

#include "Control/Communication.h"
#include "Periph/EngineGroup.h"

namespace Control
{
	class VehicleMode {
	public:
		void setControlData(
				const ManualControlPacket &controlData,
				Periph::EngineGroup &leftEngineGroup,
				Periph::EngineGroup &rightEngineGroup
				);
	};
}

#endif /* CONTROL_VEHICLEMODE_H_ */
