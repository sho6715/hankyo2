/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
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
void ADC2_Start(void);
void ADC1_Start(void);

void ADC3_clearEOS(void);
void ADC2_clearEOS(void);
void ADC1_clearEOS(void);
uint16_t GetBatVal(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

