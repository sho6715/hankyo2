/*
 * init.c
 *
 *  Created on: 2021/01/30
 *      Author: shohe
 */


#include "hal/init.h"



void HAL_init( void )
{
	TIME_init();
	/* ジャイロセンサ */
	f_GyroNowAngle = 0;			// ジャイロセンサの現在角度(0にしても探索他�?�動くが、宴会とかtestrunとか�?�動かな�?)修正済みと思われる
	l_GyroRef  = 0;				// ジャイロセンサの基準値

	f_ErrChkAngle = 0;
	bl_ErrChk = FALSE;
	DIST_init();
	MAP_Goal_init();
	ADC4_Start();
  	ADC3_Start();
  	ADC2_Start();
  	ADC1_Start();

  	SPI1_Start();
  	SPI2_Start();
  	ICM_42688_init();
  	ICM_42688_whoami();
	TIMER_init();
	SYS_start();
	MAP_init();
	GYRO_SetRef();
}

void TIME_init( void )
{
	/* 初期�? */
	Msec_in = 0;		// �?部時�?[msec]
	Sec_in  = 0;		// �?部時�?[sec]
	Min_in  = 0;		// �?部時�?[min]
}



