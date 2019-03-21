/*
 * Control.h
 *
 *  Created on: 2.9 2018
 *      Author: Zahorack
 */

#ifndef UTIL_CONTROL_H_
#define UTIL_CONTROL_H_

#include "stm32f4xx.h"

#include "Periph/Engine.h"
#include "Periph/EngineGroup.h"
#include "Periph/Usart.h"
#include "Periph/Servo.h"
#include "Periph/Stepper.h"

#include "Control/VehicleMode.h"
#include "Control/SunTrackerMode.h"
#include "Control/PrintingMode.h"
#include "Control/SimulationMode.h"

#include "Control/Communication.h"

#include "Util/Timer.h"

namespace Control
{
	namespace ControlModes
	{
		enum Enum : uint8_t {
			VehicleMode = 0,
			SunTrackerMode,
			PrintingMode,
			SimulationMode,

			Stopped,

			Size
		};
	}

	class Control {
		Periph::Engine m_engines[7] = {
				Periph::Engine(Periph::Engines::Engine1), Periph::Engine(Periph::Engines::Engine2),
				Periph::Engine(Periph::Engines::Engine3), Periph::Engine(Periph::Engines::Engine4),
				Periph::Engine(Periph::Engines::Engine5), Periph::Engine(Periph::Engines::Engine6),
				Periph::Engine(Periph::Engines::Engine7)
		};
		Periph::Servo m_servos[2] = {
				Periph::Servo(Periph::Servos::Servo1),
				Periph::Servo(Periph::Servos::Servo2, 60)
		};
		Periph::Stepper m_steppers[2] = {
				Periph::Stepper(Periph::Steppers::Stepper1),
				Periph::Stepper(Periph::Steppers::Stepper2)
		};

		Periph::EngineGroup
			m_leftEngineGroup = Periph::EngineGroup(&m_engines[3], &m_engines[4], &m_engines[5]),
			m_rightEngineGroup = Periph::EngineGroup(&m_engines[0], &m_engines[1], &m_engines[2]);

		Util::Timer
			m_watchdog = Util::Timer(Util::Time::FromSeconds(1)),
			m_updateTimer = Util::Timer(Util::Time::FromMilliSeconds(10));

		VehicleMode m_vehicleMode;
		SunTrackerMode m_sunTrackerMode;
		PrintingMode m_printingMode;
		SimulationMode m_simulationMode;

		ControlModes::Enum m_controlMode = ControlModes::VehicleMode;

	public:
		Control();
		~Control();

		void stop();

		void setControlData(const ControlData &controlData);

		void update();
	};
}

#endif /* UTIL_CONTROL_H_ */
