/*
 * init.c
 *
 *  Created on: 2021/01/30
 *      Author: shohe
 */


#include "hal/init.h"


void HAL_init( void )
{
	/* ジャイロセンサ */
	f_GyroNowAngle = 0;			// ジャイロセンサの現在角度(0にしても探索他は動くが、宴会とかtestrunとかは動かない)修正済みと思われる
	l_GyroRef  = 0;				// ジャイロセンサの基準値

	f_ErrChkAngle = 0;
	bl_ErrChk = FALSE;
}

void TIME_init( void )
{
	/* 初期化 */
	Msec_in = 0;		// 内部時計[msec]
	Sec_in  = 0;		// 内部時計[sec]
	Min_in  = 0;		// 内部時計[min]
}



