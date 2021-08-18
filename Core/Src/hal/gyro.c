/*
 * gyro.c
 *
 *  Created on: Jan 13, 2021
 *      Author: shohe
 */

#include "hal/gyro.h"

#define		GYRO_REF_NUM		(200)		//ジャイロのリファレンス値をサンプリングする数
#define		ACCEL_REF_NUM		(200)		//ジャイロのリファレンス値をサンプリングする数

/*角速度取得*/
/*
int16_t s_AccelVal; 					  				// 加速度の取得値
float f_NowAccel;										// 加速度の現在地
int32_t  l_AccelRef; 									// 加速度の基準値

//フェイルセーフ
float  			f_ErrChkAngle; 			  // ジャイロセンサのエラー検出用の角度
bool   			bl_ErrChk; 				  // ジャイロセンサのエラー検出（FALSE：検知しない、TRUE：検知する）
bool			bl_failsafe		= FALSE;	// マウスがの制御不能（TRUE：制御不能、FALSE：制御可能）
*/
uint16_t Get_s_gyro(void)
{
	return s_GyroVal;
}
void ICM_42688_whoami(void)
{
	SetSPI1TransmitData(0,(0x75 | 0x80));
	SetSPI1TransmitData(1, 0x00);
	SPI1_DMA_Communication(2);
	printf("who am i = %x\r\n",Get_SPI1ReciveData(1));

}


void ICM_42688_WriteByte(uint8_t reg,uint8_t data)
{
	SetSPI1TransmitData(0, reg);
	SetSPI1TransmitData(1, data);

	SPI1_DMA_Communication(2);
}

void ICM_42688_ReadByte(uint8_t reg,uint8_t length)
{
	SetSPI1TransmitData(0,(reg | 0x80));
	SetSPI1TransmitData(1,0x00);
	SetSPI1TransmitData(2,0x00);

	SPI1_DMA_Communication(3);
}


void ICM_42688_init(void)
{
	uint16_t reg78 = 0x4E;	//pwr_mgmt0
//	uint16_t reg79 = 0x4F;	//gyro_config0 default

/*
	uint16_t reg107 = 0x6B;
	uint16_t reg106 = 0x6A;
	uint16_t reg27 = 0x1B;
	uint16_t reg28 = 0x1C;
*/
	ICM_42688_WriteByte(reg78,0x0F);
	LL_mDelay(1);

/*
	ICM_42688_WriteByte(reg107,0x80);
	LL_mDelay(1);
	ICM_42688_WriteByte(reg106,0x11);
	LL_mDelay(1);
	ICM_42688_WriteByte(reg27,0x18);
	LL_mDelay(1);
	ICM_42688_WriteByte(reg28,0x18);
	LL_mDelay(1);
*/
}

void ICM_42688_GyroRead_DMA(uint8_t reg) //reg 29 2A
{
	ICM_42688_ReadByte(reg,3);
}

void ICM_42688_GyroData(void)
{
	s_GyroVal=((uint16_t)Get_SPI1ReciveData(1)<<8|Get_SPI1ReciveData(2));
}

void GYRO_SetRef( void )
{
	uint16_t i;
	uint32_t ul_ref = 0;

	/* データサンプリング */
	for( i=0; i<GYRO_REF_NUM; i++){			// 100回サンプリングした平均値を基準の値とする。
		ul_ref += (uint32_t)s_GyroVal;
		LL_mDelay(1);
	}

	/* 基準値算出（平均値） */
	l_GyroRef = (ul_ref * 100) / GYRO_REF_NUM ;		// 精度を100倍にする
}

float GYRO_getSpeedErr( void )
{
	int32_t  l_val = (int32_t)s_GyroVal * 100 ;				// 100倍の精度にする
	int32_t  l_err = l_val - l_GyroRef ;
	float f_res;

	/* 角速度の偏差算出 */
//	if( ( l_err < -0.01 * 100 ) || ( 0.01 * 100 < l_err ) ){
		f_res = (float)l_err /16.4 / 100;		
													// 100倍の精度
//	}
/*	else{
		f_res = 0;									// [deg/s]
	}
*/
	return f_res;
}

float GYRO_getNowAngle( void )
{
	return f_GyroNowAngle;
}

float GYRO_getRef( void )
{
	return l_GyroRef;
}

void GYRO_Pol( void )
{
	float f_speed;

	/* 現在の角度を更新する */
	f_speed = GYRO_getSpeedErr();			// 角速度取得 (0.001sec毎の角速度)
	f_GyroNowAngle += f_speed / 1000;		// 角度設定   (0.001sec毎に加算するため)

	/* エラーチェック */
	if( bl_ErrChk == TRUE ){

		f_ErrChkAngle += f_speed/1000;		// 角度設定   (0.001sec毎に加算するため)

		if( ( f_ErrChkAngle < -500 ) || ( 500 < f_ErrChkAngle )||(f_speed <-1500)||(1500<f_speed) ){

			Failsafe_flag();
		}

	}
}

void ACCEL_SetRef( void )
{
	uint16_t i;
	int32_t ul_ref = 0;

	/* データサンプリング */
	for( i=0; i<ACCEL_REF_NUM; i++){			// 100回サンプリングした平均値を基準の値とする。
		ul_ref += (int32_t)s_AccelVal;
		LL_mDelay(1);
	}

	/* 基準値算出（平均値） */
	l_AccelRef = ul_ref / ACCEL_REF_NUM ;
//	l_GyroRef = 0x1304*100;
}

float Accel_getSpeedErr( void )
{
	int32_t  l_val = (int32_t)s_AccelVal ;				// 100倍の精度にする
	int32_t  l_err = l_val - l_AccelRef ;
	float f_res;

	f_res= (float)l_err/2048*9800;
	return f_res;
}

void ACCEL_Pol( void )
{
	/* 加速度の値を取得する */
//	s_AccelVal = (int16_t)recv_spi_accel();

	/* 現在の加速度を更新する */
//	f_NowAccel = Accel_getSpeedErr();			// 角速度取得 (0.001sec毎の加速度)

}

void GYRO_staErrChkAngle( void )
{
	f_ErrChkAngle = 0;
	bl_ErrChk = TRUE;

}

void GYRO_endErrChkAngle( void )
{
	f_ErrChkAngle = 0;
	bl_ErrChk = FALSE;

}

