/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOF);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(SEN3_GPIO_Port, SEN3_Pin);

  /**/
  LL_GPIO_ResetOutputPin(SEN2_GPIO_Port, SEN2_Pin);

  /**/
  LL_GPIO_ResetOutputPin(LED3_GPIO_Port, LED3_Pin);

  /**/
  LL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED2_Pin);

  /**/
  LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);

  /**/
  LL_GPIO_ResetOutputPin(SEN1_GPIO_Port, SEN1_Pin);

  /**/
  LL_GPIO_ResetOutputPin(SEN0_GPIO_Port, SEN0_Pin);

  /**/
  LL_GPIO_ResetOutputPin(LED0_GPIO_Port, LED0_Pin);

  /**/
  LL_GPIO_ResetOutputPin(BAT_LED_GPIO_Port, BAT_LED_Pin);

  /**/
  LL_GPIO_ResetOutputPin(MOT0_GPIO_Port, MOT0_Pin);

  /**/
  LL_GPIO_ResetOutputPin(MOT1_GPIO_Port, MOT1_Pin);

  /**/
  LL_GPIO_ResetOutputPin(LED4_GPIO_Port, LED4_Pin);

  /**/
  LL_GPIO_SetOutputPin(CS_gyro_GPIO_Port, CS_gyro_Pin);

  /**/
  LL_GPIO_SetOutputPin(CS_enL_GPIO_Port, CS_enL_Pin);

  /**/
  LL_GPIO_SetOutputPin(CS_enR_GPIO_Port, CS_enR_Pin);

  /**/
  GPIO_InitStruct.Pin = SEN3_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(SEN3_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = SEN2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(SEN2_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LED3_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED3_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LED2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = SEN1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(SEN1_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = SEN0_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(SEN0_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CS_gyro_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CS_gyro_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LED0_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED0_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CS_enL_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CS_enL_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = BAT_LED_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(BAT_LED_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = SW0_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  LL_GPIO_Init(SW0_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = SW1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  LL_GPIO_Init(SW1_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = MOT0_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(MOT0_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = MOT1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(MOT1_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LED4_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED4_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CS_enR_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CS_enR_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
void SetLED(uint8_t data){
	if((data&0x01)==0x01){ 	LL_GPIO_SetOutputPin(LED0_GPIO_Port,LED0_Pin);
	}else{					LL_GPIO_ResetOutputPin(LED0_GPIO_Port,LED0_Pin);
	}
	if((data&0x02)==0x02){ 	LL_GPIO_SetOutputPin(LED1_GPIO_Port,LED1_Pin);
	}else{					LL_GPIO_ResetOutputPin(LED1_GPIO_Port,LED1_Pin);
	}
	if((data&0x04)==0x04){ 	LL_GPIO_SetOutputPin(LED2_GPIO_Port,LED2_Pin);
	}else{					LL_GPIO_ResetOutputPin(LED2_GPIO_Port,LED2_Pin);
	}
	if((data&0x08)==0x08){ 	LL_GPIO_SetOutputPin(LED3_GPIO_Port,LED3_Pin);
	}else{					LL_GPIO_ResetOutputPin(LED3_GPIO_Port,LED3_Pin);
	}
	if((data&0x10)==0x10){ 	LL_GPIO_SetOutputPin(LED4_GPIO_Port,LED4_Pin);
	}else{					LL_GPIO_ResetOutputPin(LED4_GPIO_Port,LED4_Pin);
	}
}

void SetBatLED(uint8_t data){
	if(data == 1)	LL_GPIO_SetOutputPin(BAT_LED_GPIO_Port,BAT_LED_Pin);
	else			LL_GPIO_ResetOutputPin(BAT_LED_GPIO_Port,BAT_LED_Pin);
}

int8_t SW_IsOn_0(void){//virtical
	return LL_GPIO_IsInputPinSet(SW0_GPIO_Port,SW0_Pin);
}

int8_t SW_IsOn_1(void){//horizontal
	return LL_GPIO_IsInputPinSet(SW1_GPIO_Port,SW1_Pin);
}

void Set_SenFL(uint8_t data){
	if(data == 1)	LL_GPIO_SetOutputPin(SEN0_GPIO_Port,SEN0_Pin);
	else			LL_GPIO_ResetOutputPin(SEN0_GPIO_Port,SEN0_Pin);
}

void Set_SenSL(uint8_t data){
	if(data == 1)	LL_GPIO_SetOutputPin(SEN1_GPIO_Port,SEN1_Pin);
	else			LL_GPIO_ResetOutputPin(SEN1_GPIO_Port,SEN1_Pin);
}

void Set_SenSR(uint8_t data){
	if(data == 1)	LL_GPIO_SetOutputPin(SEN2_GPIO_Port,SEN2_Pin);
	else			LL_GPIO_ResetOutputPin(SEN2_GPIO_Port,SEN2_Pin);
}

void Set_SenFR(uint8_t data){
	if(data == 1)	LL_GPIO_SetOutputPin(SEN3_GPIO_Port,SEN3_Pin);
	else			LL_GPIO_ResetOutputPin(SEN3_GPIO_Port,SEN3_Pin);
}

void Set_MOT0(uint8_t data){
	if(data == 1)	LL_GPIO_SetOutputPin(MOT0_GPIO_Port,MOT0_Pin);
	else			LL_GPIO_ResetOutputPin(MOT0_GPIO_Port,MOT0_Pin);
}

void Set_MOT1(uint8_t data){
	if(data == 1)	LL_GPIO_SetOutputPin(MOT1_GPIO_Port,MOT1_Pin);
	else			LL_GPIO_ResetOutputPin(MOT1_GPIO_Port,MOT1_Pin);
}
/*
void Set_CS_L(uint8_t data){
	if(data == 1)	LL_GPIO_SetOutputPin(CS_enL_GPIO_Port,CS_enL_Pin);
	else			LL_GPIO_ResetOutputPin(CS_enL_GPIO_Port,CS_enL_Pin);
}

void Set_CS_R(uint8_t data){
	if(data == 1)	LL_GPIO_SetOutputPin(CS_enR_GPIO_Port,CS_enR_Pin);
	else			LL_GPIO_ResetOutputPin(CS_enR_GPIO_Port,CS_enR_Pin);
}
void Set_CS_gyro(uint8_t data){
	if(data == 1)	LL_GPIO_SetOutputPin(CS_gyro_GPIO_Port,CS_gyro_Pin);
	else			LL_GPIO_ResetOutputPin(CS_gyro_GPIO_Port,CS_gyro_Pin);
}
*/
/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
