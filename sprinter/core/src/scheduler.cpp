//
// Created by xgallom on 10/5/19.
//

#include <core/scheduler.h>
#include <core/fatal.h>
#include <cstring>

namespace core::scheduler {
	namespace {
		constexpr uint32_t TasksFull = 0xffffffffu;

		uint32_t maskFor(uint32_t id) { return 1u << id; }

		uint32_t s_tasksMask[TaskType::Size] = {};
		GenericHandler s_handlers[TaskType::Size][MaximumTasksCount] = {};
		uint32_t s_handlerArgumentSizes[TaskType::Size][MaximumTasksCount] = {};
		GenericHandlerArgument s_handlerArguments[TaskType::Size][MaximumTasksCount] = {};

		Time s_periodicalTasksIntervals[MaximumTasksCount] = {};
		Time s_periodicalTasksNext[MaximumTasksCount] = {};

		template<typename Task>
		void insert(uint32_t n, const Task &task)
		{
			s_handlers[Task::Type][n] = task.handler;
			s_handlerArgumentSizes[Task::Type][n] = task.handlerArguments.size;
			memcpy(s_handlerArguments[Task::Type][n].buffer,
				   task.handlerArguments.argument.buffer,
				   task.handlerArguments.size);
		}

		template<>
		void insert<Periodical>(uint32_t n, const Periodical &task)
		{
			using Task = Periodical;

			s_handlers[Task::Type][n] = task.handler;
			s_handlerArgumentSizes[Task::Type][n] = task.handlerArguments.size;
			memcpy(s_handlerArguments[Task::Type][n].buffer,
				   task.handlerArguments.argument.buffer,
				   task.handlerArguments.size);

			s_periodicalTasksIntervals[n] = task.interval;
			s_periodicalTasksNext[n] = Time();
		}

		void call(GenericHandler handler, uint32_t argumentSize, GenericHandlerArgument &argument)
		{
			if(argumentSize)
				handler(&argument.buffer);
			else
				reinterpret_cast<VoidHandler>(handler)();
		}

		void runForevers()
		{
			static constexpr auto Type = TaskType::Forever;

			const auto tasksMask = s_tasksMask[Type];
			const auto *handlers = s_handlers[Type];
			const auto *handlerArgumentsSize = s_handlerArgumentSizes[Type];
			auto *handlerArguments = s_handlerArguments[Type];

			for(uint32_t id = 0; id < MaximumTasksCount; ++id) {
				if(tasksMask & maskFor(id))
					call(handlers[id], handlerArgumentsSize[id], handlerArguments[id]);
			}
		}

		void runPeriodicals()
		{
			static constexpr auto Type = TaskType::Periodical;

			const auto tasksMask = s_tasksMask[Type];
			const auto *handlers = s_handlers[Type];
			const auto *handlerArgumentsSize = s_handlerArgumentSizes[Type];
			auto *handlerArguments = s_handlerArguments[Type];
			const auto *periodicalTasksIntervals = s_periodicalTasksIntervals;
			auto *periodicalTasksNext = s_periodicalTasksNext;

			const auto now = Time::Now();

			for(uint32_t id = 0; id < MaximumTasksCount; ++id) {
				auto &taskNext = periodicalTasksNext[id];

				if((tasksMask & maskFor(id)) && (now >= taskNext)) {
					call(handlers[id], handlerArgumentsSize[id], handlerArguments[id]);
					taskNext = now + periodicalTasksIntervals[id];
				}
			}
		}

		void runOnces()
		{
			static constexpr auto Type = TaskType::Once;

			const auto tasksMask = s_tasksMask[Type];
			const auto *handlers = s_handlers[Type];
			const auto *handlerArgumentsSize = s_handlerArgumentSizes[Type];
			auto *handlerArguments = s_handlerArguments[Type];

			for(uint32_t id = 0; id < MaximumTasksCount; ++id) {
				if(tasksMask & maskFor(id)) {
					call(handlers[id], handlerArgumentsSize[id], handlerArguments[id]);
					remove({TaskType::Once, id});

					return;
				}
			}
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

	void remove(TaskHandler task)
	{
		s_tasksMask[task.type] &= ~maskFor(task.id);
	}

	template<typename Task>
	void replace(TaskHandler oldTask, const Task &newTask)
	{
		if(Task::Type != oldTask.type)
			fatal("Replacing tasks with different type - ", oldTask.type, " with ", Task::Type, "\n");

		insert(oldTask.id, newTask);
	}
	template void replace(TaskHandler oldTask, const Forever &newTask);
	template void replace(TaskHandler oldTask, const Periodical &newTask);
	template void replace(TaskHandler oldTask, const Once &newTask);


	void scheduleAll()
	{
		scheduleQuick();
		runOnces();
	}

	void scheduleQuick()
	{
		runForevers();
		runPeriodicals();
	}
}
