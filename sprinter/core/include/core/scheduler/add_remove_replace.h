//
// Created by xgallom on 10/8/19.
//

#ifndef SPRINTER_SPRINTER_CORE_INCLUDE_CORE_SCHEDULER_ADD_REMOVE_REPLACE_H
#define SPRINTER_SPRINTER_CORE_INCLUDE_CORE_SCHEDULER_ADD_REMOVE_REPLACE_H

#include "../task.h"

namespace core::scheduler {
	template<typename Task>
	[[nodiscard]] TaskHandler add(const Task &task);
	extern template TaskHandler add(const Forever &task);
	extern template TaskHandler add(const Periodical &task);
	extern template TaskHandler add(const Once &task);

	void remove(TaskHandler &task);
	void removeUnsafe(const TaskHandler &task);

	void reactivate(const TaskHandler &task);

	template<typename Task>
	void replace(const TaskHandler &task, const Task &newTask);
	extern template void replace(const TaskHandler &oldTask, const Forever &newTask);
	extern template void replace(const TaskHandler &oldTask, const Periodical &newTask);
	extern template void replace(const TaskHandler &oldTask, const Once &newTask);
}

#endif //SPRINTER_SPRINTER_CORE_INCLUDE_CORE_SCHEDULER_ADD_REMOVE_REPLACE_H
