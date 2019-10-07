//
// Created by xgallom on 10/8/19.
//

#include "scheduler_state.h"
#include <core/scheduler/add_remove_replace.h>
#include <core/scheduler/schedule.h>

namespace core::scheduler {
	namespace {
		void runForevers()
		{
			static constexpr auto Type = TaskType::Forever;

			const auto tasksMask = s_tasksMask[Type];
			const auto *handlers = s_handlers[Type];
			auto *handlerArguments = s_handlerArguments[Type];

			for(uint32_t id = 0; id < MaximumTasksCount; ++id) {
				if(tasksMask & maskFor(id))
					handlers[id]({Type, id}, &handlerArguments[id].buffer);
			}
		}

		void runPeriodicals()
		{
			static constexpr auto Type = TaskType::Periodical;

			const auto tasksMask = s_tasksMask[Type];
			const auto *handlers = s_handlers[Type];
			auto *handlerArguments = s_handlerArguments[Type];
			const auto *periodicalTasksIntervals = s_periodicalTasksIntervals;
			auto *periodicalTasksNext = s_periodicalTasksNext;

			const auto now = Time::Now();

			for(uint32_t id = 0; id < MaximumTasksCount; ++id) {
				if(tasksMask & maskFor(id) && now >= periodicalTasksNext[id]) {
					periodicalTasksNext[id] = now + periodicalTasksIntervals[id];
					handlers[id]({Type, id}, &handlerArguments[id].buffer);
				}
			}
		}

		void runOnces()
		{
			static constexpr auto Type = TaskType::Once;

			const auto tasksMask = s_tasksMask[Type];
			const auto *handlers = s_handlers[Type];
			auto *handlerArguments = s_handlerArguments[Type];

			for(uint32_t id = 0; id < MaximumTasksCount; ++id) {
				if(tasksMask & maskFor(id)) {
					const auto taskHandler = TaskHandler{Type, id};

					removeUnsafe(taskHandler);
					handlers[id](taskHandler, &handlerArguments[id].buffer);

					return;
				}
			}
		}
	}

	void scheduleAll()
	{
		scheduleQuick();
		scheduleSlow();
	}

	void scheduleQuick()
	{
		runForevers();
		runPeriodicals();
	}

	void scheduleSlow()
	{
		runOnces();
	}
}
