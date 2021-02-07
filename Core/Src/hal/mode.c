/*
 * mode.c
 *
 *  Created on: 2021/01/30
 *      Author: shohe
 */


#include "hal/mode.h"

#define mode_1  (0x01)
#define mode_2  (0x10)

typedef enum{
	MODE_0 = 0,
	MODE_1,
	MODE_2,
	MODE_3,
	MODE_4,
	MODE_5,
	MODE_6,
	MODE_7,
	MODE_MAX
}enMODE;

uint8_t now_mode = mode_1;

enMODE		en_Mode;		//現在のモード

void SYS_start( void )
{
	/* タイトル表示 */
	printf(" ┏━━━━━━━━━━━━━━━━━━━━━┓\r\n");
	printf(" ┃ Robo Name  : hankyo2     ┃\r\n");
	printf(" ┃ Developer  : sho sato              ┃\r\n");
	printf(" ┃ Version    : ver1                ┃\r\n");
	printf(" ┃ Project By : RT Corporation    ┃\r\n");
	printf(" ┗━━━━━━━━━━━━━━━━━━━━━┛\r\n");
/*
	PARAM_makeSra( (float)SEARCH_SPEED, 100.0f, 2500.0f, SLA_45 );		// 進入速度[mm/s]、角加速度[rad/s^2]、横G[mm/s^2]、スラロームタイプ
	PARAM_makeSra( (float)SEARCH_SPEED, 250.0f, 2500.0f, SLA_90 );		// 進入速度[mm/s]、角加速度[rad/s^2]、横G[mm/s^2]、スラロームタイプ
	PARAM_makeSra( (float)SEARCH_SPEED, 150.0f, 6000.0f, SLA_135 );		// 進入速度[mm/s]、角加速度[rad/s^2]、横G[mm/s^2]、スラロームタイプ
	PARAM_makeSra( (float)SEARCH_SPEED, 200.0f, 7000.0f, SLA_N90 );		// 進入速度[mm/s]、角加速度[rad/s^2]、横G[mm/s^2]、スラロームタイプ
*/

}

void MODE_inc( void )
{
	en_Mode++;		// モードを加算

	/* 最大値チェック */
	if( MODE_MAX == en_Mode ){
		en_Mode = MODE_0;
	}

	/* モード表示 */
	switch( en_Mode ){

		case MODE_0:
			SetLED(0x00 & now_mode);
			break;

		case MODE_1:
			SetLED((0x01>>1) & now_mode);
			break;

		case MODE_2:
			SetLED((0x02>>1) & now_mode);
			break;

		case MODE_3:
			SetLED((0x03>>1) & now_mode);
			break;

		case MODE_4:
			SetLED((0x04>>1) & now_mode);
			break;

		case MODE_5:
			SetLED((0x05>>1) & now_mode);
			break;

		case MODE_6:
			SetLED((0x06>>1) & now_mode);
			break;

		case MODE_7:
			SetLED((0x07>>1) & now_mode);
			break;

		default:
			break;
	}
}

void MODE_exe_m0( void )
{
	enMAP_HEAD_DIR		en_endDir2;
	now_mode = mode_2;
	/* モード表示 */
	switch( en_Mode ){

		case MODE_0:
			SetLED(0x0e);
			break;

		case MODE_1:
			SetLED(0x0e);
			break;

		case MODE_2:
			SetLED(0x0e);
			break;

		case MODE_3:
			SetLED(0x0e);
			break;

		case MODE_4:
			SetLED(0x0e);
			break;

		case MODE_5:
			SetLED(0x0e);
			break;

		case MODE_6:
			SetLED(0x0e);
			break;

		case MODE_7:
			SetLED(0x0e);
			break;

		default:
			break;
	}
}

void MODE_exe1( void )
{
//	uint16_t *read;
	enMAP_HEAD_DIR		en_endDir;

	now_mode = mode_1;
	uint8_t i;
	GYRO_SetRef();
	ENC_setref();
	Failsafe_flag_off();
//	log_flag_on();	//ログ関数実行用フラグ　大会時には削除
	/* モード表示 */
	switch( en_Mode ){

		case MODE_0:	//モジュール調整用プログラム群
			SetLED(0x0e);
			en_Mode = MODE_0;	//注意：MODE_incを利用するため最初にen_Modeを初期化　最後にen_Modeを戻す操作が必要
			LL_mDelay(100);
			SetLED(0x00);
			while(1){
				if ( SW_IsOn_1() == SW_ON ){
					MODE_inc();								// モードを1つ進める
					LL_mDelay(200);			// SWが離されるまで待つ
					printf("mode selecting_0\r\n");
				}
				else if (( SW_IsOn_0() == SW_ON )||(TRUE == MODE_CheckExe())){
					MODE_exe_m0();								// モード実行
					LL_mDelay(200);				// SWが離されるまで待つ
					if (en_Mode == MODE_7)break;
				}

			}
			en_Mode = MODE_0;
			break;

		case MODE_1:
			SetLED(0x0e);
			break;

		case MODE_2:
			SetLED(0x0e);
			break;

		case MODE_3:
			SetLED(0x0e);
			break;

		case MODE_4:
			SetLED(0x0e);
			break;

		case MODE_5:
			SetLED(0x0e);
			break;

		case MODE_6:
			SetLED(0x0e);
			break;

		case MODE_7:
			SetLED(0x0e);
			break;

		default:
			break;
	}
}

bool MODE_DistRightCheck(void)
{
	int16_t s_rightval;
	bool bl_check;

	s_rightval = DIST_getNowVal(DIST_SEN_R_FRONT);

	if( s_rightval >= 100 ){
		bl_check=TRUE;
	}
	else{
		bl_check=FALSE;
	}

	return bl_check;
}

bool MODE_DistLeftCheck(void){

	int16_t 	s_leftval;
	bool	bl_check;

	s_leftval 	= DIST_getNowVal(DIST_SEN_L_FRONT);

	if( s_leftval >= 100 ){
		bl_check = TRUE;

	}else{
		bl_check = FALSE;

	}

	return bl_check;
}

bool MODE_setWaitCheck(void){

	bool bl_check;

	if( TRUE == MODE_DistRightCheck() ){	// 右だけ検知
		SetLED(0x02);
	}
	if( TRUE == MODE_DistLeftCheck() ){		// 左だけ検知
		SetLED(0x08);
	}

	if( ( TRUE == MODE_DistRightCheck() ) && ( TRUE == MODE_DistLeftCheck() ) ){
		SetLED(0x0e);
		bl_check = TRUE;

	}else{
		bl_check = FALSE;
	}
	return bl_check;
}

bool MODE_CheckExe(void){

	bool bl_check;

	if( TRUE == MODE_setWaitCheck() ){
		LL_mDelay(200);

		if( FALSE == MODE_setWaitCheck() ){
			SetLED(0x00);
			LL_mDelay(1000);
			bl_check = TRUE;

		}else{
			bl_check = FALSE;

		}

	}else{

		bl_check = FALSE;
	}

	return bl_check;
}


