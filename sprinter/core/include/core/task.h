//
// Created by xgallom on 10/5/19.
//

#ifndef SPRINTER_SPRINTER_CORE_INCLUDE_CORE_TASK_H
#define SPRINTER_SPRINTER_CORE_INCLUDE_CORE_TASK_H

#include "generic_handler.h"
#include <core/time.h>
#include <cstring>

namespace core {
	struct GenericTask {
		GenericHandler handler = nullptr;
		GenericArguments handlerArguments;

	protected:
		template<typename Arg>
		explicit inline GenericTask(Handler<Arg> handler, const Arg &arg) :
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
		static constexpr auto Type = TaskType::Forever;

		template<typename Arg>
		explicit inline Forever(Handler<Arg> handler, const Arg &arg = {}) : GenericTask(handler, arg) {}
		explicit inline Forever(VoidHandler handler) : GenericTask(handler) {}
	};

	struct Periodical : GenericTask {
		static constexpr auto Type = TaskType::Periodical;

		Time interval = {};

		template<typename Arg>
		inline Periodical(const Time &interval, Handler<Arg> handler, const Arg &arg = Arg{}) :
				GenericTask(handler, arg),
				interval(interval) {}
		explicit inline Periodical(const Time &interval, VoidHandler handler) :
				GenericTask(handler),
				interval(interval) {}
	};

	struct Once : GenericTask {
		static constexpr auto Type = TaskType::Once;

		template<typename Arg>
		explicit inline Once(Handler<Arg> handler, const Arg &arg = Arg{}) : GenericTask(handler, arg) {}
		explicit inline Once(VoidHandler handler) : GenericTask(handler) {}
	};
}

namespace logImpl {
	void log(core::TaskType::Enum x);
}

#endif //SPRINTER_SPRINTER_CORE_INCLUDE_CORE_TASK_H
