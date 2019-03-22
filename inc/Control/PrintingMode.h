/*
 * PrintingMode.h
 *
 *  Created on: Nov 16, 2018
 *      Author: xgallom
 */

#ifndef CONTROL_PRINTINGMODE_H_
#define CONTROL_PRINTINGMODE_H_

#include "Control/Communication.h"
#include "Periph/Stepper.h"

namespace Control
{
	class PrintingMode {
	public:
		void setControlData(
				const ManualControlPacket &controlData,
				Periph::Stepper &horizontalStepper,
				Periph::Stepper &verticalStepper
				);

		void update(
				Periph::Stepper &horizontalStepper,
				Periph::Stepper &verticalStepper
				);
	};
}

#endif /* CONTROL_PRINTINGMODE_H_ */
