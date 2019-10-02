//
// Created by xgallom on 10/2/19.
//

#include <sprinter/run.h>
#include <core/log.h>
#include <main.h>

void run(void)
{
	log("\n\nSprinter started\n");

	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

	for(;;) {}

#pragma clang diagnostic pop
}
