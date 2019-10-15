//
// Created by xgallom on 10/2/19.
//

#include <sprinter/run.h>
#include <core/scheduler.h>
#include <core/log.h>
#include <main.h>
#include <transport/engine_control.h>

void run(void)
{
	log("\n\nSprinter started\n");

	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);

	transport::initialize();

	transport::setDirection(transport::Engine::Engine1, transport::EngineDirection::Forward);
	transport::setSpeed(transport::Engine::Engine1, 4);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

	for(;;)
		core::scheduler::scheduleAll();

#pragma clang diagnostic pop
}
