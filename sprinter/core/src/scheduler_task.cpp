//
// Created by xgallom on 10/5/19.
//

#include <core/scheduler_task.h>
#include <core/log.h>

namespace logImpl {
	void log(core::TaskType::Enum x)
	{
		static constexpr char NameTable[core::TaskType::Size + 1][11] = {
				"Forever",
				"Periodical",
				"Once",
				"Invalid"
		};

		log(NameTable[x]);
	}
}
