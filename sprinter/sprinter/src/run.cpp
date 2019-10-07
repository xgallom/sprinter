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
	if(GPIO_Pin == USER_BTN_Pin) {
		static core::Time lastPressTime = {};

		const auto now = core::Time::Now();

		if(now >= lastPressTime + core::Time::Millis(200)) {
			log("External interrupt from USER_BTN\n");
			core::scheduler::remove(s_task);
			lastPressTime = now;
		}
	}
}

static void doubleAdd(core::TaskHandler task, uint32_t *x);

static void writeAdd(core::TaskHandler task, uint32_t *x)
{
	log("Write ", ++(*x), "\n");

	if(*x >= 0x08) {
		log("Continuing with doubleAdd\n");
		core::scheduler::continueWith(task, doubleAdd);
	}
}

static void doubleAdd(core::TaskHandler task, uint32_t *x)
{
	uint32_t &xRef = *x;
	log("Write ", (xRef *= 2), "\n");

	if(xRef == 0x100) {
		log("Continuing into once writeAdd\n");
		s_task = core::scheduler::continueInto(task, writeAdd, core::IntoOnce{});
	}
}

void run(void)
{
	log("\n\nSprinter started\n");

	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);

	s_task = core::scheduler::add(core::Periodical(core::Time::Seconds(1), writeAdd));

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

	for(;;)
		core::scheduler::scheduleAll();

#pragma clang diagnostic pop
}
