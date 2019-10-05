//
// Created by xgallom on 10/2/19.
//

#include <sprinter/run.h>
#include <core/log.h>
#include <main.h>
#include <core/scheduler.h>

extern "C" { extern TIM_HandleTypeDef htim8; }

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	log("External interrupt from: ", GPIO_Pin, "\n");
}

void writeLol(uint32_t *xPtr)
{
	uint32_t &x = *xPtr;

	log("Write ", x++, "\n");
}

void runOnceLol(uint32_t *x)
{
	log("Run once\n");
	writeLol(x);
	core::scheduler::add(core::Once(writeLol, *x));
}

void run(void)
{
	log("\n\nSprinter started\n");

	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);

	const auto task = core::scheduler::add(core::Once(runOnceLol, uint32_t{0}));

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

	for(;;)
		core::scheduler::scheduleAll();

#pragma clang diagnostic pop
}
