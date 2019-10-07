//
// Created by xgallom on 10/8/19.
//

#ifndef SPRINTER_SPRINTER_CORE_SRC_SCHEDULER_STATE_H
#define SPRINTER_SPRINTER_CORE_SRC_SCHEDULER_STATE_H

#include <core/generic_handler.h>
#include <core/time.h>

namespace core::scheduler {
	inline uint32_t maskFor(uint32_t id) { return 1u << id; }

	extern uint32_t s_tasksMask[TaskType::Size];
	extern GenericHandler s_handlers[TaskType::Size][MaximumTasksCount];
	extern uint32_t s_handlerArgumentSizes[TaskType::Size][MaximumTasksCount];
	extern GenericHandlerArgument s_handlerArguments[TaskType::Size][MaximumTasksCount];

	extern Time s_periodicalTasksIntervals[MaximumTasksCount];
	extern Time s_periodicalTasksNext[MaximumTasksCount];
}

#endif //SPRINTER_SPRINTER_CORE_SRC_SCHEDULER_STATE_H
