//
// Created by xgallom on 10/7/19.
//

#ifndef SPRINTER_SPRINTER_CORE_INCLUDE_CORE_TASK_TYPE_H
#define SPRINTER_SPRINTER_CORE_INCLUDE_CORE_TASK_TYPE_H

#include <cstdint>

namespace core::TaskType {
	enum Enum : uint8_t {
		Forever,
		Periodical,
		Once,

		Invalid
	};

	constexpr uint8_t Size = Invalid;
}

#endif //SPRINTER_SPRINTER_CORE_INCLUDE_CORE_TASK_TYPE_H
