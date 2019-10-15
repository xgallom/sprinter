//
// Created by xgallom on 10/15/19.
//

#include <transport/engine_control.h>
#include <core/scheduler/add_remove_replace.h>

#include <main.h>
#include <core/log.h>

extern "C" {
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim8;
}

namespace transport {
	inline uint8_t maskFor(Engine::Enum engine) { return 1u << engine; }

	struct EnginesConfig {
		uint8_t direction = {};
		uint8_t velocities[Engine::Size] = {};
	};

	static constexpr auto UpdateInterval = 500;
	static core::TaskHandler s_task = {};

	static constexpr uint16_t DirectionPin[Engine::Size] = {
			ENG1_DIR_Pin,
			ENG2_DIR_Pin,
			ENG3_DIR_Pin,
			ENG4_DIR_Pin,
			ENG5_DIR_Pin,
			ENG6_DIR_Pin,
			ENG7_DIR_Pin
	};

	static void updateVelocities(core::TaskHandler taskHandler, EnginesConfig *enginesConfig);
	static EnginesConfig &arg() { return *reinterpret_cast<EnginesConfig *>(core::scheduler::arg(s_task)); }

	void setDirection(Engine::Enum engine, EngineDirection::Enum direction)
	{
		auto &directionConfig = arg().direction;
		auto mask = maskFor(engine);

		if(direction)
			directionConfig |= mask;
		else
			directionConfig &= ~mask;
	}

	void setSpeed(Engine::Enum engine, uint8_t speed)
	{
		arg().velocities[engine] = speed;
	}

	void initialize()
	{
		s_task = core::scheduler::add(core::Periodical(
				core::Time::Millis(UpdateInterval),
				updateVelocities,
				EnginesConfig{}
		));
	}

	static bool getCurrentDirection(uint16_t directionPin) { return ENG1_DIR_GPIO_Port->ODR & directionPin; }
	static void setCurrentDirection(uint16_t directionPin, bool targetDirection)
	{
		HAL_GPIO_WritePin(ENG1_DIR_GPIO_Port, directionPin, static_cast<GPIO_PinState>(targetDirection));
	}

	static void updateVelocity(Engine::Enum engine, const EnginesConfig &enginesConfig)
	{
		static volatile uint32_t *OutputChannel[Engine::Size] = {
				&htim8.Instance->CCR4,
				&htim8.Instance->CCR3,
				&htim8.Instance->CCR2,
				&htim8.Instance->CCR1,
				&htim4.Instance->CCR1,
				&htim4.Instance->CCR2,
				&htim4.Instance->CCR3
		};

		const auto targetVelocity = enginesConfig.velocities[engine];
		const bool targetDirection = enginesConfig.direction & maskFor(engine);

		auto &currentVelocity = *OutputChannel[engine];
		const auto currentDirection = getCurrentDirection(DirectionPin[engine]);

		if(currentDirection != targetDirection) {
			if(currentVelocity) {
				--currentVelocity;
				log(engine, ": --currentVelocity\n");
			}
			else {
				setCurrentDirection(DirectionPin[engine], targetDirection);
				log(engine, ": currentDirection = ", targetDirection, "\n");
			}
		}
		else if(currentVelocity < targetVelocity) {
			++currentVelocity;
			log(engine, ": ++currentVelocity\n");
		}
		else if(currentVelocity > targetVelocity) {
			--currentVelocity;
			log(engine, ": --currentVelocity\n");
		}
	}

	static void updateVelocities(core::TaskHandler, EnginesConfig *enginesConfig)
	{
		for(uint8_t engine = Engine::Engine1; engine < Engine::Size; ++engine)
			updateVelocity(static_cast<Engine::Enum>(engine), *enginesConfig);
	}
}

namespace logImpl {
	void log(const transport::Engine::Enum &engine)
	{
		static constexpr char NameTable[transport::Engine::Size][8] = {
				"Engine1",
				"Engine2",
				"Engine3",
				"Engine4",
				"Engine5",
				"Engine6",
				"Engine7",
		};

		log(NameTable[engine]);
	}
}
