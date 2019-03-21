/*
 * Control.cpp
 *
 *  Created on: Jul 23, 2018
 *      Author: Zahorack
 */

#include "Control/Control.h"
#include "Application.h"
#include "Util/Tools.h"
#include "Util/Direction.h"

LOGGER_MODULE(Control)

namespace Control
{
	namespace Engines
	{
		enum Usage {
			Differential = Periph::Engines::Engine7
		};
	}

	namespace Servos
	{
		enum Usage {
			Horizontal = Periph::Servos::Servo1,
			Vertical = Periph::Servos::Servo2
		};
	}

	namespace Steppers
	{
		enum Usage {
			Horizontal = Periph::Steppers::Stepper2,
			Vertical = Periph::Steppers::Stepper1
		};
	}

	Control::Control()
	{
		m_watchdog.start();
		m_updateTimer.start();

		stop();

		m_servos[Servos::Horizontal].start();
		m_servos[Servos::Vertical].start();
	}

	Control::~Control(){
		stop();
	}

	void Control::stop()
	{
		m_leftEngineGroup.stop();
		m_rightEngineGroup.stop();
		m_servos[Servos::Horizontal].stop();
		m_servos[Servos::Vertical].stop();
		m_steppers[Steppers::Horizontal].disable();
		m_steppers[Steppers::Vertical].disable();
	}

	void Control::setControlData(const ControlData &controlData)
	{
		if(!controlData.data.state) {
			m_controlMode = ControlModes::Stopped;
			stop();
		}
		else if(m_controlMode != controlData.data.mode) {
			m_controlMode = static_cast<ControlModes::Enum>(controlData.data.mode);
			stop();
		}

		switch(m_controlMode) {
		case ControlModes::VehicleMode:
			m_vehicleMode.setControlData(controlData, m_leftEngineGroup, m_rightEngineGroup);
			break;

		case ControlModes::SunTrackerMode:
			m_sunTrackerMode.setControlData(controlData, m_servos[Servos::Horizontal], m_servos[Servos::Vertical], m_engines[Engines::Differential]);
			break;

		case ControlModes::PrintingMode:
			m_printingMode.setControlData(controlData, m_steppers[Steppers::Horizontal], m_steppers[Steppers::Vertical]);
			break;

		case ControlModes::SimulationMode:
			m_simulationMode.setControlData(controlData, m_steppers[Steppers::Horizontal], m_steppers[Steppers::Vertical]);
			break;

		case ControlModes::Stopped:
			break;
		case ControlModes::Size:
			break;
		}

		DBG_LOG("right: %d", controlData.data.x);
		DBG_LOG("left: %d", controlData.data.y);
		DBG_LOG("state: %d", controlData.data.state);
		DBG_LOG("pot: %d", controlData.data.pot);
	}

	void Control::update()
	{
		if(m_watchdog.run()) {
			stop();
			INF_LOG("DISCONNECTED");
		}

		switch(m_controlMode) {
		case ControlModes::VehicleMode:
			break;

		case ControlModes::SunTrackerMode:
			m_sunTrackerMode.update(m_servos[Servos::Horizontal], m_servos[Servos::Vertical]);
			break;

		case ControlModes::PrintingMode:
			m_printingMode.update(m_steppers[Steppers::Horizontal], m_steppers[Steppers::Vertical]);
			break;

		case ControlModes::SimulationMode:
			m_simulationMode.update(m_steppers[Steppers::Horizontal], m_steppers[Steppers::Vertical]);
			break;

		case ControlModes::Stopped:
			break;
		case ControlModes::Size:
			break;
		}

		m_leftEngineGroup.update();
		m_rightEngineGroup.update();
		m_servos[Servos::Horizontal].update();
		m_servos[Servos::Vertical].update();
		m_steppers[Steppers::Horizontal].update();
		m_steppers[Steppers::Vertical].update();
	}
}
