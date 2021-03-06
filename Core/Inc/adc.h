/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_ADC1_Init(void);
void MX_ADC3_Init(void);
void MX_ADC4_Init(void);

/* USER CODE BEGIN Prototypes */
uint16_t GetSensor_SL(void);
uint16_t GetSensor_FL(void);
uint16_t GetSensor_FR(void);
uint16_t GetSensor_SR(void);

void ADC4_Start(void);
void ADC3_Start(void);
void ADC1_Start(void);

void ADC3_clearEOS(void);
void ADC1_clearEOS(void);
uint16_t GetBatVal(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
