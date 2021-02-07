/**
  ******************************************************************************
  * @file    spi.h
  * @brief   This file contains all the function prototypes for
  *          the spi.c file
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
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#ifdef _VSCODE
#include "Inc/hal/encoder.h"
#else
#include "hal/encoder.h"
#endif

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

void MX_SPI1_Init(void);
void MX_SPI2_Init(void);

/* USER CODE BEGIN Prototypes */

void DMA1_Channel1_Init(void);
void DMA1_Channel2_Init(void);
void DMA1_Channel3_Init(void);
void DMA1_Channel4_Init(void);
void SPI1_Start(void);
void SPI1_DMA_Communication(uint8_t length);
void SPI1_DMA1_ReceiveComplete_Callback(void);
void SPI1_DMA1_TransmitComplete_Callback(void);
uint8_t Get_SPI1ReciveData(uint8_t num);
void SetSPI1TransmitData(uint8_t num, uint8_t data);

void SPI2_Start(void);
void SPI2_DMA_Communication(uint8_t length,uint8_t dir);
void SPI2_DMA1_ReceiveComplete_Callback(void);
void SPI2_DMA1_TransmitComplete_Callback(void);
uint8_t Get_SPI2ReciveData(uint8_t num);
void SetSPI2TransmitData(uint8_t num, uint8_t data);
/*
void SPI1_Start_test(void);
void SPI_Communication(SPI_TypeDef *SPIx, uint8_t *tx_data,uint8_t *rx_data, uint8_t length, GPIO_TypeDef *GPIOx, uint32_t CS_Pin);
void MPU9250_whoami(void);
*/
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
