//
// Created by xgallom on 10/15/19.
//

#ifndef SPRINTER_SPRINTER_TRANSPORT_INCLUDE_TRANSPORT_ENGINE_CONTROL_H
#define SPRINTER_SPRINTER_TRANSPORT_INCLUDE_TRANSPORT_ENGINE_CONTROL_H

#include <core/task_handler.h>

namespace transport {
	namespace Engine {
		enum Enum : uint8_t {
			Engine1,
			Engine2,
			Engine3,
			Engine4,
			Engine5,
			Engine6,
			Engine7,

			Size
		};
	}

	namespace EngineDirection {
		enum Enum : bool {
			Backward = false,
			Forward = true
		};
	}

	void setDirection(Engine::Enum engine, EngineDirection::Enum direction);
	void setSpeed(Engine::Enum engine, uint8_t speed);

	void initialize();
}

namespace logImpl {
	void log(const transport::Engine::Enum &engine);
}

#endif //SPRINTER_SPRINTER_TRANSPORT_INCLUDE_TRANSPORT_ENGINE_CONTROL_H
