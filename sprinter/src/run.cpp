//
// Created by xgallom on 10/2/19.
//

#include "sprinter/run.h"
#include "main.h"

void run(void)
{
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);

	for(;;) {}
}
