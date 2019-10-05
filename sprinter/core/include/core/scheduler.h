//
// Created by xgallom on 10/5/19.
//

#ifndef SPRINTER_SPRINTER_CORE_INCLUDE_CORE_SCHEDULER_H
#define SPRINTER_SPRINTER_CORE_INCLUDE_CORE_SCHEDULER_H

#include <core/time.h>

namespace core {
	static constexpr uint32_t
			MaximumTasksCount = 32,
			GenericHandlerArgumentsSize = 32;

	struct GenericHandlerArguments {
		using Buffer = uint8_t[GenericHandlerArgumentsSize];
		Buffer buffer;
	};

	using GenericHandler = void (*)(GenericHandlerArguments::Buffer *args);

	namespace TaskType {
		enum Enum : uint8_t {
			Forever,
			Periodical,
			Once,

			Invalid
		};

		static constexpr uint8_t Size = Invalid;
	}

	struct GenericArguments {
		uint32_t size = {};
		GenericHandlerArguments arguments = {};
	};

	struct TaskHandler {
		TaskType::Enum type = TaskType::Invalid;
		uint32_t id = {};

		explicit inline operator bool() const { return type != TaskType::Invalid; }
	};

	struct GenericTask {
		GenericHandler handler = nullptr;
		GenericArguments handlerArguments;
	};

	struct Forever : GenericTask { static constexpr TaskType::Enum Type = TaskType::Forever; };
	struct Periodical : GenericTask {
		static constexpr TaskType::Enum Type = TaskType::Periodical;
		Time interval = {};
	};
	struct Once : GenericTask { static constexpr TaskType::Enum Type = TaskType::Once; };

	namespace scheduler {
		template<typename Task>
		TaskHandler add(const Task &task);
		extern template TaskHandler add(const Forever &task);
		extern template TaskHandler add(const Periodical &task);
		extern template TaskHandler add(const Once &task);

		void remove(TaskHandler task);

		template<typename Task>
		void replace(TaskHandler task, const Task &newTask);
		extern template void replace(TaskHandler oldTask, const Forever &newTask);
		extern template void replace(TaskHandler oldTask, const Periodical &newTask);
		extern template void replace(TaskHandler oldTask, const Once &newTask);

		void scheduleAll();
		void scheduleQuick();
	}
}

namespace logImpl {
	void log(core::TaskType::Enum x);
}

#endif //SPRINTER_SPRINTER_CORE_INCLUDE_CORE_SCHEDULER_H
