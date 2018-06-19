/*
 * Pwm.cpp
 *
 *  Created on: Jun 18, 2018
 *      Author: Zahorack
 */


#include "Pwm.h"

namespace Periph{




void Pwm::initRCC()
{
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,  ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,  ENABLE);

	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

}

void Pwm::initGpio()
{
	 GPIO_InitTypeDef    GPIO_InitStructure;
	 GPIO_StructInit(&GPIO_InitStructure);

	 /* port A */
	     GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	     GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	     GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	  	 GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* port C */
	  	 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	  	 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	  	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  	 GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	  	 GPIO_Init(GPIOC, &GPIO_InitStructure);


	  	/* set alternative function */
	  	 GPIO_PinAFConfig(GPIOA, GPIO_PinSource0,  GPIO_AF_TIM2);
	  	 GPIO_PinAFConfig(GPIOA, GPIO_PinSource1,  GPIO_AF_TIM2);
	  	 GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,  GPIO_AF_TIM2);
	  	 GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,  GPIO_AF_TIM2);

	  	 GPIO_PinAFConfig(GPIOC, GPIO_PinSource6,  GPIO_AF_TIM8);
	  	 GPIO_PinAFConfig(GPIOC, GPIO_PinSource7,  GPIO_AF_TIM8);
	  	 GPIO_PinAFConfig(GPIOC, GPIO_PinSource8,  GPIO_AF_TIM8);
	  	 GPIO_PinAFConfig(GPIOC, GPIO_PinSource9,  GPIO_AF_TIM8);

}


void initTimOC(){




}




}/* namespace Periph */
