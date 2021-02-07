/*
 * encoder.c
 *
 *  Created on: Jan 13, 2021
 *      Author: shohe
 */


#include "hal/encoder.h"

int16_t ENC_L_CNT;
int16_t ENC_R_CNT;
int16_t ENC_L_CNT_old;
int16_t ENC_R_CNT_old;

void MA702_ReadByte(en_endir dir)
{
	encoderdir = dir;
	SetSPI2TransmitData(0,0x00);
	SetSPI2TransmitData(1,0x00);

	SPI2_DMA_Communication(2,encoderdir);
}

void recv_spi_encoder(void)
{
	MA702_ReadByte(enL);
	ENC_L_CNT = ((uint16_t)Get_SPI2ReciveData(1)<<8|Get_SPI2ReciveData(0));

	MA702_ReadByte(enR);
	ENC_R_CNT = ((uint16_t)Get_SPI2ReciveData(1)<<8|Get_SPI2ReciveData(0));
}

uint8_t Runmode_check( enDCM_ID en_id )
{
	if(en_id == DCM_R){
		if((en_Type == 6)||(((en_Type == 7)||(en_Type == 8)||(en_Type == 9))&&(en_Turntype == Right)) ){
			return(0);
		}
		else{
			return(1);
		}
	}
	else{
		if((en_Type == 6)||(((en_Type == 7)||(en_Type == 8)||(en_Type == 9))&&(en_Turntype == Left)) ){
			return(0);
		}
		else{
			return(1);
		}
	}
}

void ENC_GetDiv( int32_t* p_r, int32_t* p_l )
{
	int32_t cntR;
	int32_t cntL;
	int32_t cntR_dif;
	int32_t cntL_dif;
	recv_spi_encoder();
	cntR_dif = ENC_R_CNT - ENC_R_CNT_old;
	cntL_dif = ENC_L_CNT_old - ENC_L_CNT;

	//モードを使って正逆をチェックして加算方法を決める
	if(Runmode_check(DCM_R) == 1){	//正方向カウント
		//右
		if(cntR_dif<-32768){
			cntR = cntR_dif + 65536;
		}
		else if (cntR_dif>32768){
			cntR = cntR_dif - 65536;
		}
		else{
			cntR = cntR_dif;
		}
	}
	else{
		if(cntR_dif>32768){
			cntR = cntR_dif - 65536;
		}
		else if(cntR_dif<-32768){
			cntR = cntR_dif + 65536;
		}
		else{
			cntR = cntR_dif;
		}
	}

	if(Runmode_check(DCM_L) == 1){
		//左
		if(cntL_dif<-32768){
			cntL = cntL_dif + 65536;
		}
		else if (cntL_dif >32768){
			cntL = cntL_dif -65536;
		}
		else{
			cntL = cntL_dif;
		}
	}
	else{
		if(cntL_dif>32768){
			cntL = cntL_dif - 65536;
		}
		else if(cntL_dif<-32768){
			cntL = cntL_dif + 65536;
		}
		else{
			cntL = cntL_dif;
		}
	}

	*p_r = cntR;		//2^16(65536) LSB/1回転
	*p_l = cntL;

	ENC_R_CNT_old = ENC_R_CNT;
	ENC_L_CNT_old = ENC_L_CNT;
}

void ENC_setref(void)
{
	recv_spi_encoder();
	ENC_R_CNT_old = ENC_R_CNT;
	ENC_L_CNT_old = ENC_L_CNT;
}

void ENC_print(void)
{
	printf("エンコーダ [R]=%d [L]=%d \r",ENC_R_CNT*360/65536,ENC_L_CNT*360/65536);
//	printf("%d,%d \r\n",ENC_R_CNT*360/65536,ENC_L_CNT*360/65536);
}

