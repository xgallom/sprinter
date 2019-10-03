/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USER_Btn_Pin GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define SUN_ADC1_Pin GPIO_PIN_0
#define SUN_ADC1_GPIO_Port GPIOC
#define SUN_ADC2_Pin GPIO_PIN_1
#define SUN_ADC2_GPIO_Port GPIOC
#define SUN_ADC3_Pin GPIO_PIN_2
#define SUN_ADC3_GPIO_Port GPIOC
#define SUN_ADC4_Pin GPIO_PIN_3
#define SUN_ADC4_GPIO_Port GPIOC
#define SRV1_POS_Pin GPIO_PIN_0
#define SRV1_POS_GPIO_Port GPIOA
#define SRV2_POS_Pin GPIO_PIN_1
#define SRV2_POS_GPIO_Port GPIOA
#define SUN_ADC5_Pin GPIO_PIN_4
#define SUN_ADC5_GPIO_Port GPIOC
#define LED_GREEN_Pin GPIO_PIN_0
#define LED_GREEN_GPIO_Port GPIOB
#define ENG1_DIR_Pin GPIO_PIN_7
#define ENG1_DIR_GPIO_Port GPIOE
#define ENG2_DIR_Pin GPIO_PIN_8
#define ENG2_DIR_GPIO_Port GPIOE
#define ENG3_DIR_Pin GPIO_PIN_9
#define ENG3_DIR_GPIO_Port GPIOE
#define ENG4_DIR_Pin GPIO_PIN_10
#define ENG4_DIR_GPIO_Port GPIOE
#define ENG5_DIR_Pin GPIO_PIN_11
#define ENG5_DIR_GPIO_Port GPIOE
#define ENG6_DIR_Pin GPIO_PIN_12
#define ENG6_DIR_GPIO_Port GPIOE
#define ENG7_DIR_Pin GPIO_PIN_14
#define ENG7_DIR_GPIO_Port GPIOE
#define STEP1_DIR_Pin GPIO_PIN_10
#define STEP1_DIR_GPIO_Port GPIOB
#define STEP2_DIR_Pin GPIO_PIN_11
#define STEP2_DIR_GPIO_Port GPIOB
#define LED_RED_Pin GPIO_PIN_14
#define LED_RED_GPIO_Port GPIOB
#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define ENG5_SPD_Pin GPIO_PIN_12
#define ENG5_SPD_GPIO_Port GPIOD
#define ENG6_SPD_Pin GPIO_PIN_13
#define ENG6_SPD_GPIO_Port GPIOD
#define ENG7_SPD_Pin GPIO_PIN_14
#define ENG7_SPD_GPIO_Port GPIOD
#define STEP1_SPD_Pin GPIO_PIN_2
#define STEP1_SPD_GPIO_Port GPIOG
#define STEP2_SPD_Pin GPIO_PIN_3
#define STEP2_SPD_GPIO_Port GPIOG
#define USB_PowerSwitchOn_Pin GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define USB_OverCurrent_Pin GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port GPIOG
#define ENG4_SPD_Pin GPIO_PIN_6
#define ENG4_SPD_GPIO_Port GPIOC
#define ENG3_SPD_Pin GPIO_PIN_7
#define ENG3_SPD_GPIO_Port GPIOC
#define ENG2_SPD_Pin GPIO_PIN_8
#define ENG2_SPD_GPIO_Port GPIOC
#define ENG1_SPD_Pin GPIO_PIN_9
#define ENG1_SPD_GPIO_Port GPIOC
#define USB_SOF_Pin GPIO_PIN_8
#define USB_SOF_GPIO_Port GPIOA
#define LOG_TX_Pin GPIO_PIN_9
#define LOG_TX_GPIO_Port GPIOA
#define LOG_RX_Pin GPIO_PIN_10
#define LOG_RX_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define STEP1_EN_Pin GPIO_PIN_0
#define STEP1_EN_GPIO_Port GPIOD
#define STEP2_EN_Pin GPIO_PIN_1
#define STEP2_EN_GPIO_Port GPIOD
#define RF_TX_Pin GPIO_PIN_5
#define RF_TX_GPIO_Port GPIOD
#define RF_RX_Pin GPIO_PIN_6
#define RF_RX_GPIO_Port GPIOD
#define LED_BLUE_Pin GPIO_PIN_7
#define LED_BLUE_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
