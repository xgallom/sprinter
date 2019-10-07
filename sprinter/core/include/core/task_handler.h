//
// Created by xgallom on 10/7/19.
//

#ifndef SPRINTER_SPRINTER_CORE_INCLUDE_CORE_TASK_HANDLER_H
#define SPRINTER_SPRINTER_CORE_INCLUDE_CORE_TASK_HANDLER_H

#include "task_type.h"

namespace core {
	struct TaskHandler {
		TaskType::Enum type = TaskType::Invalid;
		uint32_t id = {};

		explicit inline operator bool() const { return type != TaskType::Invalid; }
	};
}

#endif //SPRINTER_SPRINTER_CORE_INCLUDE_CORE_TASK_HANDLER_H
