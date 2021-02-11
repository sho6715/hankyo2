/*
 * gyro.h
 *
 *  Created on: Jan 13, 2021
 *      Author: shohe
 */

#ifndef INC_GYRO_H_
#define INC_GYRO_H_

#ifdef _VSCODE
#include "Inc/spi.h"
#include "Inc/glob.h"
#else
#include "spi.h"
#include "glob.h"
#endif

/*?????*/
int16_t s_AccelVal; 					  				// ???????
float f_NowAccel;										// ???????
int32_t  l_AccelRef; 									// ???????

//???????
float  			f_ErrChkAngle; 			  // ?????????????????
bool   			bl_ErrChk; 				  // ??????????????FALSE???????TRUE??????

/*�W���C���Z���T*/
int16_t s_GyroVal; 					  				// �W���C���Z���T�̌��ݒl
int16_t s_GyroValBuf[8];								// �W���C���Z���T�̃o�b�t�@�l
float  f_GyroNowAngle;		 						// �W���C���Z���T�̌��݊p�x
int32_t  l_GyroRef; 									// �W���C���Z���T�̊�l

uint16_t Get_s_gyro(void);
void ICM_42688_whoami(void);
void ICM_42688_WriteByte(uint8_t reg,uint8_t data);
void ICM_42688_ReadByte(uint8_t reg,uint8_t length);
void ICM_42688_init(void);
void ICM_42688_GyroRead_DMA(uint8_t reg);
void ICM_42688_GyroData(void);

void GYRO_SetRef( void );
float GYRO_getSpeedErr( void );
float GYRO_getNowAngle( void );
float GYRO_getRef( void );
void GYRO_Pol( void );
void ACCEL_SetRef( void );
float Accel_getSpeedErr( void );
void ACCEL_Pol( void );
void GYRO_staErrChkAngle( void );
void GYRO_endErrChkAngle( void );

#endif /* INC_GYRO_H_ */
