//
// Created by xgallom on 10/5/19.
//

#ifndef SPRINTER_SPRINTER_CORE_INCLUDE_CORE_SCHEDULER_H
#define SPRINTER_SPRINTER_CORE_INCLUDE_CORE_SCHEDULER_H

#include "scheduler_task.h"

namespace core::scheduler {
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

#endif //SPRINTER_SPRINTER_CORE_INCLUDE_CORE_SCHEDULER_H
