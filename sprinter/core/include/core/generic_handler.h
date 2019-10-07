//
// Created by xgallom on 10/5/19.
//

#ifndef SPRINTER_SPRINTER_CORE_INCLUDE_CORE_GENERIC_HANDLER_H
#define SPRINTER_SPRINTER_CORE_INCLUDE_CORE_GENERIC_HANDLER_H

#include "task_handler.h"
#include <cstdint>

namespace core {
	constexpr uint32_t
			MaximumTasksCount = 32,
			GenericHandlerArgumentsSize = 32;

	struct GenericHandlerArgument {
		using Buffer = uint8_t[GenericHandlerArgumentsSize];
		Buffer buffer;
	};

	using GenericHandler = void (*)(TaskHandler task, GenericHandlerArgument::Buffer *arg);

	template<typename Arg> using Handler = void (*)(TaskHandler task, Arg *arg);
	using VoidHandler = void (*)(TaskHandler task);

	struct GenericArguments {
		uint32_t size = {};
		GenericHandlerArgument argument = {};
	};
}

#endif //SPRINTER_SPRINTER_CORE_INCLUDE_CORE_GENERIC_HANDLER_H
