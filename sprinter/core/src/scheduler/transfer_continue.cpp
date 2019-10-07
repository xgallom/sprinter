//
// Created by xgallom on 10/8/19.
//

#include "scheduler_state.h"
#include <core/scheduler/add_remove_replace.h>
#include <core/scheduler/transfer_continue.h>
#include <core/log.h>
#include <cstring>

namespace core::scheduler {
	void continueWith(TaskHandler task, GenericHandler newHandler)
	{
		s_tasksMask[task.type] |= maskFor(task.id);
		s_handlers[task.type][task.id] = newHandler;
	}

	namespace {
		template<typename Into>
		void copy(TaskHandler task, uint32_t id, Into into)
		{
			s_handlers[Into::Type][id] = s_handlers[task.type][task.id];

			const auto argumentSize = s_handlerArgumentSizes[task.type][task.id];

			s_handlerArgumentSizes[Into::Type][id] = argumentSize;
			memcpy(s_handlerArguments[Into::Type][id].buffer,
				   s_handlerArguments[task.type][task.id].buffer,
				   argumentSize);
		}

		template<>
		void copy(TaskHandler task, uint32_t id, IntoPeriodical into)
		{
			using Into = IntoPeriodical;

			s_handlers[Into::Type][id] = s_handlers[task.type][task.id];

			const auto argumentSize = s_handlerArgumentSizes[task.type][task.id];

			s_handlerArgumentSizes[Into::Type][id] = argumentSize;
			memcpy(s_handlerArguments[Into::Type][id].buffer,
				   s_handlerArguments[task.type][task.id].buffer,
				   argumentSize);

			const auto interval = into.interval;

			s_periodicalTasksIntervals[id] = interval;
			s_periodicalTasksNext[id] = Time::Now() + interval;
		}
	}

	template<typename Into>
	TaskHandler transfer(TaskHandler &task, Into into)
	{
		uint32_t &tasksMask = s_tasksMask[Into::Type];

		if(!task) {
			log("Trying to transfer invalid task\n");
			return {};
		}
		if(Into::Type == task.type) {
			log("Trying to transfer ", Into::Type, " task into itself\n");
			return {};
		}

		for(uint32_t id = 0; id < MaximumTasksCount; ++id) {
			const uint32_t taskMask = maskFor(id);

			if(!(tasksMask & taskMask)) {
				tasksMask |= taskMask;

				copy(task, id, into);
				remove(task);

				return {Into::Type, id};
			}
		}

		return {};
	}
	template TaskHandler transfer(TaskHandler &task, IntoForever into);
	template TaskHandler transfer(TaskHandler &task, IntoPeriodical into);
	template TaskHandler transfer(TaskHandler &task, IntoOnce into);

	template<typename Into>
	TaskHandler continueInto(TaskHandler &task, GenericHandler newHandler, Into into)
	{
		continueWith(task, newHandler);
		return transfer(task, into);
	}
	template TaskHandler continueInto(TaskHandler &task, GenericHandler newHandler, IntoForever into);
	template TaskHandler continueInto(TaskHandler &task, GenericHandler newHandler, IntoPeriodical into);
	template TaskHandler continueInto(TaskHandler &task, GenericHandler newHandler, IntoOnce into);
}

