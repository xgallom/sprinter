//
// Created by xgallom on 10/2/19.
//

#include <sprinter/run.h>
#include <core/scheduler.h>
#include <core/log.h>
#include <main.h>

extern "C" { extern TIM_HandleTypeDef htim8; }

static core::TaskHandler s_task = {};

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	log("External interrupt from: ", GPIO_Pin, "\n");
	core::scheduler::remove(s_task);
}

void writeAdd(uint32_t *x)
{
	log("Write ", (*x)++, "\n");
}

void writeAddAndSchedule(uint32_t *x)
{
	log("Run once\n");
	writeAdd(x);

	s_task = core::scheduler::add(
			core::Periodical(core::Time::Seconds(1), writeAdd, *x)
	);
}

void run(void)
{
	log("\n\nSprinter started\n");

	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);

	const auto task = core::scheduler::add(core::Once(writeAddAndSchedule));

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

	for(;;)
		core::scheduler::scheduleAll();

#pragma clang diagnostic pop
}
