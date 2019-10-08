//
// Created by xgallom on 10/8/19.
//

#include "scheduler_state.h"
#include <core/scheduler/add_remove_replace.h>
#include <core/fatal.h>

namespace core::scheduler {
	namespace {
		template<typename Task>
		void insert(uint32_t id, const Task &task)
		{
			s_handlers[Task::Type][id] = task.handler;
			s_handlerArgumentSizes[Task::Type][id] = task.handlerArguments.size;
			memcpy(s_handlerArguments[Task::Type][id].buffer,
				   task.handlerArguments.argument.buffer,
				   task.handlerArguments.size);
		}

		template<>
		void insert<Periodical>(uint32_t id, const Periodical &task)
		{
			using Task = Periodical;

			s_handlers[Task::Type][id] = task.handler;

			const auto argumentSize = task.handlerArguments.size;

			s_handlerArgumentSizes[Task::Type][id] = argumentSize;
			memcpy(s_handlerArguments[Task::Type][id].buffer,
				   task.handlerArguments.argument.buffer,
				   argumentSize);

			const auto interval = task.interval;

			s_periodicalTasksIntervals[id] = interval;
			s_periodicalTasksNext[id] = Time::Now() + interval;
		}

	}

	template<typename Task>
	TaskHandler add(const Task &task)
	{
		uint32_t &tasksMask = s_tasksMask[Task::Type];

		for(uint32_t id = 0; id < MaximumTasksCount; ++id) {
			const uint32_t taskMask = maskFor(id);

			if(!(tasksMask & taskMask)) {
				tasksMask |= taskMask;
				insert(id, task);
				return {Task::Type, id};
			}
		}

		return {};
	}
	template TaskHandler add(const Forever &task);
	template TaskHandler add(const Periodical &task);
	template TaskHandler add(const Once &task);

	void remove(TaskHandler &task)
	{
		if(task)
			removeUnsafe(task);
		else
			log("Trying to remove invalid task\n");

		task = {};
	}

	void removeUnsafe(const TaskHandler &task)
	{
		s_tasksMask[task.type] &= ~maskFor(task.id);
	}

	void reactivate(const TaskHandler &task)
	{
		s_tasksMask[task.type] |= maskFor(task.id);
	}

	template<typename Task>
	void replace(const TaskHandler &task, const Task &newTask)
	{
		if(Task::Type != task.type)
			fatal("Replacing tasks with different type - ", task.type, " with ", Task::Type, "\n");

		s_tasksMask[Task::Type] |= maskFor(task.id);
		insert(task.id, newTask);
	}
	template void replace(const TaskHandler &oldTask, const Forever &newTask);
	template void replace(const TaskHandler &oldTask, const Periodical &newTask);
	template void replace(const TaskHandler &oldTask, const Once &newTask);
}
