/*
 * gyro.c
 *
 *  Created on: Jan 13, 2021
 *      Author: shohe
 */

#include "hal/gyro.h"

#define		GYRO_REF_NUM		(200)		//ジャイロのリファレンス値をサンプリングする数
#define		ACCEL_REF_NUM		(200)		//ジャイロのリファレンス値をサンプリングする数

/*角速度取�?*/
/*
int16_t s_AccelVal; 					  				// �?速度の取得値
float f_NowAccel;										// �?速度の現在地
int32_t  l_AccelRef; 									// �?速度の基準値

//フェイルセー�?
float  			f_ErrChkAngle; 			  // ジャイロセンサのエラー検�?�用の角度
bool   			bl_ErrChk; 				  // ジャイロセンサのエラー検�?�?�?FALSE?��検知しな�?、TRUE?��検知する?�?
bool			bl_failsafe		= FALSE;	// マウスが�?�制御不�?�?�?TRUE?��制御不�?�、FALSE?��制御可能?�?
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
	uint8_t reg78 = 0x4E;	//pwr_mgmt0
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

	/* �?ータサンプリング */
	for( i=0; i<GYRO_REF_NUM; i++){			// 100回サンプリングした平�?値を基準�?�値とする�?
		ul_ref += (uint32_t)s_GyroVal;
		LL_mDelay(1);
	}

	/* 基準値算�?�?��平�?値?�? */
	l_GyroRef = (ul_ref * 100) / GYRO_REF_NUM ;		// 精度�?100倍にする
}

float GYRO_getSpeedErr( void )
{
	int32_t  l_val = (int32_t)s_GyroVal * 100 ;				// 100倍�?�精度にする
	int32_t  l_err = l_val - l_GyroRef ;
	float f_res;

	/* 角速度の偏差算�?� */
//	if( ( l_err < -0.01 * 100 ) || ( 0.01 * 100 < l_err ) ){
		f_res = (float)l_err /16.4 / 100 * DEG_TO_RAD;		
													// 100倍�?�精度
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
	f_speed = GYRO_getSpeedErr();			// 角速度取�? (0.001sec毎�?�角速度)
	f_GyroNowAngle += f_speed / 1000;		// 角度設�?   (0.001sec毎に�?算するた�?)

	/* エラーチェ�?ク */
	if( bl_ErrChk == TRUE ){

		f_ErrChkAngle += f_speed/1000;		// 角度設�?   (0.001sec毎に�?算するた�?)
/*
		if( ( f_ErrChkAngle < -0.5 ) || ( 0.5 < f_ErrChkAngle )){//||(f_speed <-1.500)||(1.500<f_speed) ){

			Failsafe_flag();
		}
*/
	}
}

void ACCEL_SetRef( void )
{
	uint16_t i;
	int32_t ul_ref = 0;

	/* �?ータサンプリング */
	for( i=0; i<ACCEL_REF_NUM; i++){			// 100回サンプリングした平�?値を基準�?�値とする�?
		ul_ref += (int32_t)s_AccelVal;
		LL_mDelay(1);
	}

	/* 基準値算�?�?��平�?値?�? */
	l_AccelRef = ul_ref / ACCEL_REF_NUM ;
//	l_GyroRef = 0x1304*100;
}

float Accel_getSpeedErr( void )
{
	int32_t  l_val = (int32_t)s_AccelVal ;				// 100倍�?�精度にする
	int32_t  l_err = l_val - l_AccelRef ;
	float f_res;

	f_res= (float)l_err/2048*9800;
	return f_res;
}

void ACCEL_Pol( void )
{
	/* �?速度の値を取得す�? */
//	s_AccelVal = (int16_t)recv_spi_accel();

	/* 現在の�?速度を更新する */
//	f_NowAccel = Accel_getSpeedErr();			// 角速度取�? (0.001sec毎�?��?速度)

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

