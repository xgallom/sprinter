//
// Created by xgallom on 10/5/19.
//

#ifndef SPRINTER_SPRINTER_CORE_INCLUDE_CORE_FATAL_H
#define SPRINTER_SPRINTER_CORE_INCLUDE_CORE_FATAL_H

#include "log.h"
#include <utility>

template<typename ... Args>
[[noreturn]] void fatal(Args ... args)
{
	log(std::forward<Args>(args) ...);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

	for(;;) {}

#pragma clang diagnostic pop
}

#endif //SPRINTER_SPRINTER_CORE_INCLUDE_CORE_FATAL_H
