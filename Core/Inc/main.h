/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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

#include "stm32g4xx_ll_adc.h"
#include "stm32g4xx_ll_dma.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_crs.h"
#include "stm32g4xx_ll_system.h"
#include "stm32g4xx_ll_exti.h"
#include "stm32g4xx_ll_cortex.h"
#include "stm32g4xx_ll_utils.h"
#include "stm32g4xx_ll_pwr.h"
#include "stm32g4xx_ll_spi.h"
#include "stm32g4xx_ll_tim.h"
#include "stm32g4xx_ll_usart.h"
#include "stm32g4xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#ifdef _VSCODE
#include "Inc/hal/sen_batt.h"
#include "Inc/hal/gyro.h"
#include "Inc/hal/sen_dist.h"
#else
#include "hal/sen_batt.h"
#include "hal/gyro.h"
#include "hal/sen_dist.h"
#endif

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SEN3_Pin LL_GPIO_PIN_13
#define SEN3_GPIO_Port GPIOC
#define SEN2_Pin LL_GPIO_PIN_14
#define SEN2_GPIO_Port GPIOC
#define LED3_Pin LL_GPIO_PIN_15
#define LED3_GPIO_Port GPIOC
#define LED2_Pin LL_GPIO_PIN_0
#define LED2_GPIO_Port GPIOF
#define LED1_Pin LL_GPIO_PIN_1
#define LED1_GPIO_Port GPIOF
#define SEN1_Pin LL_GPIO_PIN_3
#define SEN1_GPIO_Port GPIOA
#define SEN0_Pin LL_GPIO_PIN_4
#define SEN0_GPIO_Port GPIOA
#define CS_gyro_Pin LL_GPIO_PIN_0
#define CS_gyro_GPIO_Port GPIOB
#define LED0_Pin LL_GPIO_PIN_2
#define LED0_GPIO_Port GPIOB
#define CS_enL_Pin LL_GPIO_PIN_10
#define CS_enL_GPIO_Port GPIOB
#define BAT_LED_Pin LL_GPIO_PIN_11
#define BAT_LED_GPIO_Port GPIOB
#define SW0_Pin LL_GPIO_PIN_11
#define SW0_GPIO_Port GPIOA
#define SW1_Pin LL_GPIO_PIN_12
#define SW1_GPIO_Port GPIOA
#define MOT0_Pin LL_GPIO_PIN_15
#define MOT0_GPIO_Port GPIOA
#define MOT1_Pin LL_GPIO_PIN_4
#define MOT1_GPIO_Port GPIOB
#define LED4_Pin LL_GPIO_PIN_6
#define LED4_GPIO_Port GPIOB
#define CS_enR_Pin LL_GPIO_PIN_7
#define CS_enR_GPIO_Port GPIOB
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
