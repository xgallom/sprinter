//
// Created by xgallom on 10/8/19.
//

#ifndef SPRINTER_SPRINTER_CORE_INCLUDE_CORE_SCHEDULER_TRANSFER_CONTINUE_H
#define SPRINTER_SPRINTER_CORE_INCLUDE_CORE_SCHEDULER_TRANSFER_CONTINUE_H

#include "../task_type_transformer.h"

namespace core::scheduler {
	void continueWith(TaskHandler task, GenericHandler newHandler);
	template<typename Arg>
	inline void continueWith(TaskHandler task, Handler<Arg> newHandler)
	{
		continueWith(task, reinterpret_cast<GenericHandler>(newHandler));
	}
	inline void continueWith(TaskHandler task, VoidHandler newHandler)
	{
		continueWith(task, reinterpret_cast<GenericHandler>(newHandler));
	}

	template<typename Into>
	[[nodiscard]] TaskHandler transfer(TaskHandler &task, Into into);
	extern template TaskHandler transfer(TaskHandler &task, IntoForever into);
	extern template TaskHandler transfer(TaskHandler &task, IntoPeriodical into);
	extern template TaskHandler transfer(TaskHandler &task, IntoOnce into);

	template<typename Into>
	[[nodiscard]] TaskHandler continueInto(TaskHandler &task, GenericHandler newHandler, Into into);
	extern template TaskHandler continueInto(TaskHandler &task, GenericHandler newHandler, IntoForever into);
	extern template TaskHandler continueInto(TaskHandler &task, GenericHandler newHandler, IntoPeriodical into);
	extern template TaskHandler continueInto(TaskHandler &task, GenericHandler newHandler, IntoOnce into);
	template<typename Into, typename Arg>
	[[nodiscard]] inline TaskHandler continueInto(TaskHandler &task, Handler<Arg> newHandler, Into into)
	{
		return continueInto(task, reinterpret_cast<GenericHandler>(newHandler), into);
	}
}

#endif //SPRINTER_SPRINTER_CORE_INCLUDE_CORE_SCHEDULER_TRANSFER_CONTINUE_H
