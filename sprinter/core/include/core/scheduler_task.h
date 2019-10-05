//
// Created by xgallom on 10/5/19.
//

#ifndef SPRINTER_SPRINTER_CORE_INCLUDE_CORE_SCHEDULER_TASK_H
#define SPRINTER_SPRINTER_CORE_INCLUDE_CORE_SCHEDULER_TASK_H

#include "scheduler_handler.h"
#include <core/time.h>
#include <cstring>

namespace core {
	namespace TaskType {
		enum Enum : uint8_t {
			Forever,
			Periodical,
			Once,

			Invalid
		};

		static constexpr uint8_t Size = Invalid;
	}

	struct TaskHandler {
		TaskType::Enum type = TaskType::Invalid;
		uint32_t id = {};

		explicit inline operator bool() const { return type != TaskType::Invalid; }
	};

	struct GenericTask {
		GenericHandler handler = nullptr;
		GenericArguments handlerArguments;

		template<typename Arg>
		inline GenericTask(Handler<Arg> handler, const Arg &arg) :
				handler{reinterpret_cast<GenericHandler>(handler)},
				handlerArguments{sizeof(Arg)}
		{
			static_assert(sizeof(Arg) <= GenericHandlerArgumentsSize);
			memcpy(handlerArguments.argument.buffer, &arg, handlerArguments.size);
		}

		explicit inline GenericTask(VoidHandler handler) :
				handler{reinterpret_cast<GenericHandler>(handler)},
				handlerArguments{0} {}
	};

	struct Forever : GenericTask {
		static constexpr TaskType::Enum Type = TaskType::Forever;

		template<typename Arg>
		inline Forever(Handler<Arg> handler, const Arg &arg) : GenericTask(handler, arg) {}

		explicit inline Forever(VoidHandler handler) : GenericTask(handler) {}
	};

	struct Periodical : GenericTask {
		static constexpr TaskType::Enum Type = TaskType::Periodical;

		Time interval = {};

		template<typename Arg>
		inline Periodical(const Time &interval, Handler<Arg> handler, const Arg &arg) :
				GenericTask(handler, arg),
				interval(interval) {}

		explicit inline Periodical(const Time &interval, VoidHandler handler) :
				GenericTask(handler),
				interval(interval) {}
	};

	struct Once : GenericTask {
		static constexpr TaskType::Enum Type = TaskType::Once;

		template<typename Arg>
		inline Once(Handler<Arg> handler, const Arg &arg) : GenericTask(handler, arg) {}

		explicit inline Once(VoidHandler handler) : GenericTask(handler) {}
	};
}

namespace logImpl {
	void log(core::TaskType::Enum x);
}

#endif //SPRINTER_SPRINTER_CORE_INCLUDE_CORE_SCHEDULER_TASK_H
