/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
void SetLED(uint8_t data);
void SetBatLED(uint8_t data);
int8_t SW_IsOn_0(void);
int8_t SW_IsOn_1(void);
void Set_SenSL(uint8_t data);
void Set_SenFL(uint8_t data);
void Set_SenFR(uint8_t data);
void Set_SenSR(uint8_t data);
void Set_MOT0(uint8_t data);
void Set_MOT1(uint8_t data);
/* spiÇÃÇ∆Ç±ÇÎÇÕíºê⁄èëÇ¢ÇΩÇŸÇ§Ç™égÇ¢èüéËó«Ç≥ÇªÇ§Ç»ãCÇ™ÇµÇΩ
void Set_CS_L(uint8_t data);
void Set_CS_R(uint8_t data);
void Set_CS_gyro(uint8_t data);
*/
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
