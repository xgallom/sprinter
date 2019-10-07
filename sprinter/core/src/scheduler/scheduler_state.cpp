//
// Created by xgallom on 10/8/19.
//

#include "scheduler_state.h"

namespace core::scheduler {
	uint32_t s_tasksMask[TaskType::Size] = {};
	GenericHandler s_handlers[TaskType::Size][MaximumTasksCount] = {};
	uint32_t s_handlerArgumentSizes[TaskType::Size][MaximumTasksCount] = {};
	GenericHandlerArgument s_handlerArguments[TaskType::Size][MaximumTasksCount] = {};

	Time s_periodicalTasksIntervals[MaximumTasksCount] = {};
	Time s_periodicalTasksNext[MaximumTasksCount] = {};
}
