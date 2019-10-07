//
// Created by xgallom on 10/7/19.
//

#ifndef SPRINTER_SPRINTER_CORE_INCLUDE_CORE_TASK_TYPE_TRANSFORMER_H
#define SPRINTER_SPRINTER_CORE_INCLUDE_CORE_TASK_TYPE_TRANSFORMER_H

#include "task_type.h"

namespace core {
	struct IntoForever {
		static constexpr auto Type = TaskType::Forever;
	};

	struct IntoPeriodical {
		static constexpr auto Type = TaskType::Periodical;

		Time interval = {};
	};

	struct IntoOnce {
		static constexpr auto Type = TaskType::Once;
	};
}

#endif //SPRINTER_SPRINTER_CORE_INCLUDE_CORE_TASK_TYPE_TRANSFORMER_H
