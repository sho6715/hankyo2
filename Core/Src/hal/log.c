/*
 * log.c
 *
 *  Created on: 2021/01/30
 *      Author: shohe
 */

#include "hal/log.h"


#define log_num			(200)					//ログ取得数（変更時はこちらを変更）
//ログプログラム群（取得数変更はdefineへ）
float	Log_1[log_num];
float	Log_2[log_num];
float	Log_3[log_num];
float	Log_4[log_num];
/*PRIVATE float	Log_5[log_num];
float	Log_6[log_num];
float	Log_7[log_num];
float	Log_8[log_num];
float	Log_9[log_num];
float	Log_10[log_num];
float	Log_11[log_num];
float	Log_12[log_num];
*/
uint16_t	log_count = 0;
bool	b_logflag = FALSE;


void log_in2( 	float log1,float log2,
			float log3,float log4)/*,
			float log5,float log6,
			float log7,float log8,
			float log9,float log10,
			float log11,float log12)
*/
{
	if((b_logflag == TRUE)&&(log_count < log_num)){
		Log_1[log_count] = log1;
		Log_2[log_count] = log2;
		Log_3[log_count] = log3;
		Log_4[log_count] = log4;
/*		Log_5[log_count] = log5;
		Log_6[log_count] = log6;
		Log_7[log_count] = log7;
		Log_8[log_count] = log8;
		Log_9[log_count] = log9;
		Log_10[log_count] = log10;
		Log_11[log_count] = log11;
		Log_12[log_count] = log12;
*/
		log_count++;
	}
}

void log_interrupt ( void )
{
//	log_in2(GYRO_getSpeedErr(), f_TrgtAngleS,
//		f_NowAngle,f_TrgtAngle);

//	log_in2(f_NowSpeed, f_TrgtSpeed,
//		f_NowDist, f_TrgtDist);

	log_in2(DIST_getNowVal( DIST_SEN_R_FRONT ), DIST_getNowVal( DIST_SEN_L_FRONT ),
		DIST_getNowVal( DIST_SEN_R_SIDE ), DIST_getNowVal( DIST_SEN_L_SIDE ));

/*	log_in2(f_NowSpeed, f_TrgtSpeed,
		f_NowDist, f_TrgtDist,
		GYRO_getSpeedErr(), f_TrgtAngleS,
		f_NowAngle,f_TrgtAngle,
		f_AccAngleS,templog1,
		templog2,f_Duty_R);
*/
/*	log_in2(DIST_getNowVal( DIST_SEN_R_FRONT ), DIST_getNowVal( DIST_SEN_L_FRONT ),
		DIST_getNowVal( DIST_SEN_R_SIDE ), DIST_getNowVal( DIST_SEN_L_SIDE ),
		GYRO_getSpeedErr(), f_TrgtAngleS,
		f_NowAngle,f_TrgtAngle,
		templog2,templog1,
		f_Duty_L,f_Duty_R);
*/
}

void log_flag_on(void)
{
	b_logflag = TRUE;
}

void log_flag_off(void)
{
	b_logflag = FALSE;
}

void log_read2(void)
{
	int16_t i=0;
/*	while(i<log_num){
		printf("%5.2f,%5.2f,%5.2f,%5.2f,%5.2f,%5.2f,%5.2f,%5.2f,%5.2f,%5.2f,%5.2f,%5.2f\n\r",
		Log_1[i],Log_2[i],Log_3[i],Log_4[i],Log_5[i],Log_6[i],Log_7[i],Log_8[i],Log_9[i],Log_10[i],Log_11[i],Log_12[i]);
		i++;
	}
*/
	while(i<log_num){
		printf("%5.2f,%5.2f,%5.2f,%5.2f\n\r",
		Log_1[i],Log_2[i],Log_3[i],Log_4[i]);
		i++;
	}

}
