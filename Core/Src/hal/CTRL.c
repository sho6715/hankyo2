/*
 * CTRL.c
 *
 *  Created on: 2021/01/30
 *      Author: shohe
 */


#include "hal/CTRL.h"


/* 制御  */
uint8_t 			uc_CtrlFlag			= FALSE;	// フィードバ�?ク or フィードフォワー�? 制御有効フラグ?�?FALSE:無効�?1?��有効?�?
int32_t			l_CntR;							// 右モータのカウント変化�?						?�?1[msec]毎に更新される�?
int32_t			l_CntL;							// 左モータのカウント変化�?						?�?1[msec]毎に更新される�?
// 制御
float			f_Time 				= 0;		// 動作時間[sec]								?�?1[msec]毎に更新される�?
float			f_TrgtTime 			= 1000;		// 動作目標時�? [msec]							?��設定値?�?
// 速度制御//////////////////////////////////////////
float 			f_Acc			= 0;		// [速度制御]   �?速度							?��設定値?�?
float			f_BaseSpeed		= 0;		// [速度制御]   初速度							?��設定値?�?
float			f_LastSpeed 		= 0;		// [速度制御]   最終目標速度					?��設定値?�?
float			f_NowSpeed		= 0;		// [速度制御]   現在の速度 [mm/s]				?�?1[msec]毎に更新される�?
 float			f_TrgtSpeed 		= 0;		// [速度制御]   目標移動速度 [mm/s]				?�?1[msec]毎に更新される�?
float			f_ErrSpeedBuf		= 0;		// [速度制御] 　速度エラー値のバッファ	?�?1[msec]毎に更新される�?
float			f_SpeedErrSum 		= 0;		// [速度制御]   速度積�??制御のサ�?値			?�?1[msec]毎に更新される�?
// 距離制御
float			f_BaseDist		= 0;		// [距離制御]   初期位置						?��設定値?�?
float			f_LastDist 		= 0;		// [距離制御]   最終移動距離					?��設定値?�?
float			f_TrgtDist 		= 0;		// [距離制御]   目標移動距離					?�?1[msec]毎に更新される�?
volatile float 		f_NowDist		= 0;		// [距離制御]   現在距離						?�?1[msec]毎に更新される�?
float			f_NowDistR		= 0;		// [距離制御]   現在距離?��右?�?					?�?1[msec]毎に更新される�?
float 			f_NowDistL		= 0;		// [距離制御]   現在距離?��左?�?					?�?1[msec]毎に更新される�?
float			f_DistErrSum 		= 0;		// [距離制御]   距離積�??制御のサ�?値			?�?1[msec]毎に更新される�?
// 角速度制御
float 			f_AccAngleS		= 0;		// [角速度制御] 角加速度						?��設定値?�?
float			f_BaseAngleS		= 0;		// [角速度制御] 初期角速度						?��設定値?�?
float			f_LastAngleS 		= 0;		// [角速度制御] 最終目標角速度					?��設定値?�?
float			f_TrgtAngleS 		= 0;		// [角速度制御] 目標角速度 [rad/s]				?�?1[msec]毎に更新される�?
float			f_ErrAngleSBuf		= 0;		// [角速度制御] 角速度エラー値のバッファ	?�?1[msec]毎に更新される�?
float			f_AngleSErrSum 		= 0;		// [角速度制御]   角度積�??制御のサ�?値			?�?1[msec]毎に更新される�?
// 角度制御
float			f_BaseAngle		= 0;		// [角度制御]   初期角度						?��設定値?�?
float			f_LastAngle 		= 0;		// [角度制御]   最終目標角度					?��設定値?�?
volatile float 		f_NowAngle		= 0;		// [角度制御]   現在角度　	volatileをつけな�?とwhileから抜けられなくなる（最適化�?�せい?���?1[msec]毎に更新される�?
float			f_TrgtAngle 		= 0;		// [角度制御]   目標角度						?�?1[msec]毎に更新される�?
float			f_AngleErrSum 		= 0;		// [角度制御]   角度積�??制御のサ�?値			?�?1[msec]毎に更新される�?
// 壁制御
int32_t 			l_WallErr 		= 0;		// [壁制御]     壁との偏差						?�?1[msec]毎に更新される�?
float			f_ErrDistBuf		= 0;		// [壁制御]     距離センサーエラー値のバッファ	?�?1[msec]毎に更新される�?

//フェイルセー�?
float  			f_ErrChkAngle; 			  // ジャイロセンサのエラー検�?�用の角度
bool   			bl_ErrChk; 				  // ジャイロセンサのエラー検�?�?�?FALSE?��検知しな�?、TRUE?��検知する?�?
bool			bl_failsafe		= FALSE;	// マウスが�?�制御不�?�?�?TRUE?��制御不�?�、FALSE?��制御可能?�?


float Get_NowSpeed(void){
	return f_NowSpeed;
}

float Get_NowDist(void){
	return f_NowDist;
}

float Get_TrgtDist(void){
	return f_TrgtDist;
}

float Get_TrgtSpeed(void){
	return f_TrgtSpeed;
}

void Set_TrgtSpeed(float speed){
	f_TrgtSpeed = speed;
}

float Get_NowAngle(void){
	return f_NowAngle;
}

float Get_TrgtAngle(void){
	return f_TrgtAngle;
}

float Get_TrgtAngleS(void){
	return f_TrgtAngleS;
}

void INTC_sys(void)
{
	Msec_in++;					// msec
	if( Msec_in > 999 ){		// msec �? sec
		Msec_in  = 0;
		Sec_in++;
	}
	if( Sec_in > 59 ){			// sec �? min
		Sec_in = 0;
		Min_in++;
	}

	CTRL_pol();
}

void CTRL_sta( void )
{
	uc_CtrlFlag = TRUE;
}

void CTRL_stop( void )
{
	uc_CtrlFlag = FALSE;
	DCM_brakeMot( DCM_R );		// ブレーキ
	DCM_brakeMot( DCM_L );		// ブレーキ
}

void CTRL_clrData( void )
{
//	recv_spi_encoder();								// エンコーダモジュール初期�?
//	ENC_R_CNT_old	= ENC_R_CNT;
//	ENC_L_CNT_old	= ENC_L_CNT;
/*	ENC_setref();
	l_CntR			= 0;						// カウンタクリア
	l_CntL			= 0;						// カウンタクリア
*/
	/* 現在値 */
/*	f_NowDist 		= 0;						// 移動距離リセ�?�?
	f_NowDistR 		= 0;
	f_NowDistL 		= 0;
	f_NowSpeed		= 0;						// [速度制御]   現在の速度 [mm/s]			?�?1[msec]毎に更新される�?
	f_NowAngle		= 0;						// [角度制御]   現在角度					?�?1[msec]毎に更新される�?
	s_GyroVal		= 0;						// ジャイロ値クリア
	f_GyroNowAngle	= 0;							// ジャイロ値クリア
*/
	/* 目標値 */
	f_TrgtSpeed		= 0;						// [速度制御]   目標移動速度 [mm/s]			?�?1[msec]毎に更新される�?
	f_TrgtDist 		= 0;						// [距離制御]   目標移動距離				?�?1[msec]毎に更新される�?
	f_TrgtAngleS	= 0;							// [角速度制御] 目標角速度 [rad/s]			?�?1[msec]毎に更新される�?
	f_TrgtAngle		= 0;						// [角度制御]   目標角度					?�?1[msec]毎に更新される�?

	/* 制御�?ータ */
	f_SpeedErrSum	= 0;
	f_DistErrSum 	= 0;						// [距離制御]   距離積�??制御のサ�?値			?�?1[msec]毎に更新される�?
	f_AngleSErrSum	= 0;
	f_AngleErrSum 	= 0;						// [角度制御]   角度積�??制御のサ�?値			?�?1[msec]毎に更新される�?
	f_ErrSpeedBuf	= 0;
	f_ErrDistBuf	= 0;						// [壁制御]     距離センサーエラー値のバッファ		?�?1[msec]毎に更新される�?
	f_ErrAngleSBuf  = 0;
}

void CTRL_clrNowData(void)
{
	ENC_setref();
	l_CntR			= 0;						// カウンタクリア
	l_CntL			= 0;						// カウンタクリア

	/* 現在値 */
	f_NowDist 		= 0;						// 移動距離リセ�?�?
	f_NowDistR 		= 0;
	f_NowDistL 		= 0;
	f_NowSpeed		= 0;						// [速度制御]   現在の速度 [mm/s]			?�?1[msec]毎に更新される�?
	f_NowAngle		= 0;						// [角度制御]   現在角度					?�?1[msec]毎に更新される�?
	s_GyroVal		= 0;						// ジャイロ値クリア
	f_GyroNowAngle	= 0;							// ジャイロ値クリア
}

void CTRL_setData( stCTRL_DATA* p_data )
{
	/* 制御方�? */
	en_Type					= p_data->en_type;

	/* 速度制御 */
	f_Acc 					= p_data->f_acc;
	f_BaseSpeed				= p_data->f_now;
	f_LastSpeed				= p_data->f_trgt;

	/* 距離制御 */
	f_BaseDist 				= p_data->f_nowDist;
	f_LastDist 				= p_data->f_dist;

	/* 角速度制御 */
	f_AccAngleS 			= p_data->f_accAngleS;
	f_BaseAngleS			= p_data->f_nowAngleS;
	f_LastAngleS			= p_data->f_trgtAngleS;

	/* 角度制御 */
	f_BaseAngle 			= p_data->f_nowAngle;
	f_LastAngle 			= p_data->f_angle;

	f_Time 					= 0;
	f_TrgtTime				= p_data->f_time;

	escape_wait			= 0;

	CTRL_sta();				// 制御開�?

}

void CTRL_refNow( void )
{
	float f_speedR		= 0;							// 右モータ現在速度 [mm/s]
	float f_speedL		= 0;							// 左モータ現在速度 [mm/s]
	float f_r 			= F_CNT2MM(l_CntR);				// 右モータの進んだ距離 [mm]
	float f_l 			= F_CNT2MM(l_CntL);				// 左モータの進んだ距離 [mm]

	/* 速度更新 */
	f_speedR = f_r * 1000;								// 右モータ速度 [mm/s] ( 移動距離[カウン�?] * 1パルスの移動量(0.0509[mm]) * 1000(msec→sec)
	f_speedL = f_l * 1000;								// 左モータ速度 [mm/s] ( 移動距離[カウン�?] * 1パルスの移動量(0.0509[mm]) * 1000(msec→sec)
	f_NowSpeed  = ( f_speedR + f_speedL ) / 2;			// マウス?��進行方向中�?軸?�? [1mm/s]

	/* 距離更新 */
	f_NowDistR += f_r;									// カウント更新
	f_NowDistL += f_l;									// カウント更新
	f_NowDist  = ( f_NowDistR + f_NowDistL ) / 2;		// 平�?値更新
}

void CTRL_refTarget( void )
{
	/* mode */
	switch( en_Type ){

		/* acc(straight) */
		case CTRL_ACC:
		case CTRL_SKEW_ACC:
			if( f_TrgtSpeed < (f_LastSpeed -(f_Acc * 0.001)) ){												// �?速目標更新区�?
				f_TrgtSpeed = f_BaseSpeed + f_Acc * f_Time;									// 目標速度
			}
			else{
				f_TrgtSpeed = f_LastSpeed;
			}
			break;

		/* const(straight) */
		case CTRL_CONST:
		case CTRL_SKEW_CONST:
			f_TrgtSpeed = f_BaseSpeed;														// 目標速度
			break;

		/* dec(straight) */
		case CTRL_DEC:
		case CTRL_SKEW_DEC:
			/* speed CTRL + position CTRL */
			if( f_TrgtSpeed > (f_LastSpeed +(f_Acc * 0.001))){												// 減速目標更新区�?
				f_TrgtSpeed = f_BaseSpeed - f_Acc * f_Time;									// 目標速度
				f_TrgtDist  = f_BaseDist + ( f_BaseSpeed + f_TrgtSpeed ) * f_Time / 2;		// 目標距離
			}
			/* position CTRL */
			else{
				f_TrgtSpeed = f_LastSpeed;
				f_TrgtDist  = f_LastDist;													// 目標距離
			}
			break;

		/* acc(Turn) */
		case CTRL_ACC_TRUN:

			/* CCW  hidari*/
			if( f_LastAngle > 0 ){
				if ( f_TrgtAngleS < (f_LastAngleS -(f_AccAngleS * 0.001)) ){
					f_TrgtAngleS = 0.0 + f_AccAngleS * f_Time;									// 目標角速度
				}
				else{
					f_TrgtAngleS = f_LastAngleS;
				}
			}
			/* CW  migi */
			else{
				if( f_TrgtAngleS > (f_LastAngleS +(f_AccAngleS * 0.001)) ){
				f_TrgtAngleS = 0.0 - f_AccAngleS * f_Time;									// 目標角速度
				}
				else{
					f_TrgtAngleS = f_LastAngleS;
				}
			}
			break;

		/* const(Turn) */
		case CTRL_CONST_TRUN:
//			f_TrgtAngleS =f_BaseAngleS;
			break;

		/* dec(Turn) */
		case CTRL_DEC_TRUN:
			/* CCW */
			if( f_LastAngle > 0 ){

				/* Angle speed CTRL + Angle CTRL */
				if( f_TrgtAngleS > (f_LastAngleS +(f_AccAngleS * 0.001)) ){												// 減速目標更新区�?
					f_TrgtAngleS = f_BaseAngleS - f_AccAngleS * f_Time;							// 目標角速度
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// 目標角度
				}
				/* Angle CTRL */
				else{
					f_TrgtAngleS = f_LastAngleS;
					f_TrgtAngle  = f_LastAngle;													// 目標距離
				}
			}
			/* CW */
			else{

				/* Angle speed CTRL + Angle CTRL */
				if( f_TrgtAngleS < (f_LastAngleS -(f_AccAngleS * 0.001))){												// 減速目標更新区�?
					f_TrgtAngleS = f_BaseAngleS + f_AccAngleS * f_Time;							// 目標角速度
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// 目標角度
				}
				/* Angle CTRL */
				else{
					f_TrgtAngleS = f_LastAngleS;
					f_TrgtAngle  = f_LastAngle;													// 目標距離
				}
			}
			break;

		/* entry(sura) */
		case CTRL_ENTRY_SURA:
			f_TrgtSpeed = f_BaseSpeed;
			if( f_TrgtDist <= f_LastDist - (f_TrgtSpeed * 0.001) ){
				f_TrgtDist  = f_BaseDist + f_TrgtSpeed * f_Time;								// 目標距離
			}
			break;

		/* acc(スラロー�?) */
		case CTRL_ACC_SURA:
			f_TrgtSpeed = f_BaseSpeed;

			/* CCW */
			if( f_LastAngle > 0 ){
				if( f_TrgtAngleS < (f_LastAngleS +(f_AccAngleS * 0.001))){
					f_TrgtAngleS = f_BaseAngleS + f_AccAngleS * f_Time;							// 目標角速度
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// 目標角度
//					printf("%5.2f %5.2f %5.4f %5.2f %5.2f\n\r",f_TrgtAngleS,f_AccAngleS,f_Time,f_TrgtAngle,f_LastAngleS);
				}
				else{
					f_TrgtAngle  = f_LastAngle;													// 目標距離
				}
			}
			/* CW */
			else{
				if( f_TrgtAngleS > (f_LastAngleS -(f_AccAngleS * 0.001)) ){
					f_TrgtAngleS = f_BaseAngleS + f_AccAngleS * f_Time;							// 目標角速度
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// 目標角度
//					printf("%5.2f %5.2f %5.4f %5.2f %5.2f\n\r",f_TrgtAngleS,f_AccAngleS,f_Time,f_TrgtAngle,f_LastAngleS);
				}
				else{
					f_TrgtAngle  = f_LastAngle;													// 目標距離
				}
			}

			/* Position CTRL */
			if( f_LastDist > (f_TrgtDist - (f_TrgtSpeed * 0.001)) ){													// 目標更新区�?
				f_TrgtDist  = f_BaseDist + f_TrgtSpeed * f_Time;							// 目標位置
			}
			else{
				f_TrgtDist  = f_LastDist;													// 目標距離
			}
			break;

		/* const(sura) */
		case CTRL_CONST_SURA:
			f_TrgtSpeed = f_BaseSpeed;
			f_TrgtAngleS = f_BaseAngleS;							// 目標角速度

			/* CCW */
			if( f_LastAngle > 0 ){
				if( f_TrgtAngle < (f_LastAngle +(f_AccAngleS * 0.001)) ){
					f_TrgtAngle  = f_BaseAngle + f_TrgtAngleS * f_Time;			// 目標角度
				}
				else{
					f_TrgtAngle  = f_LastAngle;									// 目標角度
				}
			}
			/* CW */
			else{
				if( f_TrgtAngle > (f_LastAngle -(f_AccAngleS * 0.001)) ){
					f_TrgtAngle  = f_BaseAngle + f_TrgtAngleS * f_Time;			// 目標角度
				}
				else{
					f_TrgtAngle  = f_LastAngle;									// 目標角度
				}
			}

			/* Position CTRL */
			if( f_LastDist > (f_TrgtDist - (f_TrgtSpeed * 0.001)) ){													// 目標更新区�?
				f_TrgtDist  = f_BaseDist + f_TrgtSpeed * f_Time;							// 目標位置
			}
			else{
				f_TrgtDist  = f_LastDist;													// 目標距離
			}
			break;

		/* dec(sura) */
		case CTRL_DEC_SURA:
			f_TrgtSpeed = f_BaseSpeed;

			/* CCW */
			if( f_LastAngle > 0 ){
				if( f_TrgtAngleS > (f_LastAngle -(f_AccAngleS * 0.001)) ){
					f_TrgtAngleS = f_BaseAngleS + f_AccAngleS * f_Time;							// 目標角速度
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// 目標角度
				}
				else{
					f_TrgtAngleS = 0.0;
					f_TrgtAngle  = f_LastAngle;													// 目標距離
				}
			}
			/*CW*/
			else{
				if( f_TrgtAngleS < (f_LastAngle +(f_AccAngleS * 0.001)) ){
					f_TrgtAngleS = f_BaseAngleS + f_AccAngleS * f_Time;							// 目標角速度
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// 目標角度
				}
				else{
					f_TrgtAngleS = 0.0;
					f_TrgtAngle  = f_LastAngle;													// 目標距離
				}
			}

			/* Position CTRL */
			if( f_LastDist > (f_TrgtDist - (f_TrgtSpeed * 0.001)) ){													// 目標更新区�?
				f_TrgtDist  = f_BaseDist + f_TrgtSpeed * f_Time;							// 目標位置
			}
			else{
				f_TrgtDist  = f_LastDist;													// 目標距離
			}
			break;

		/* escape(sura) */
		case CTRL_EXIT_SURA:
			f_TrgtSpeed = f_BaseSpeed;
			f_TrgtAngleS = 0;
			if( f_TrgtDist <= (f_LastDist -f_TrgtSpeed * 0.001)){
				f_TrgtDist  = f_BaseDist + f_TrgtSpeed * f_Time;								// 目標距離
			}
			else{
				f_TrgtDist  = f_LastDist;														// 目標距離
			}
			break;

		/* etc */
		default:
			break;
	}
}

enPARAM_MODE Chg_ParamID( enCTRL_TYPE en_type )
{
	switch( en_type ){
		case CTRL_ACC:			return PARAM_ACC;				// �?速中(直進)
		case CTRL_CONST:		return PARAM_CONST;				// 等速中(直進)
		case CTRL_DEC:			return PARAM_DEC;				// 減速中(直進)
		case CTRL_HIT_WALL:		return PARAM_HIT_WALL;			// 壁あて制御
//		case DCMC_BACK_ACC:		return PARAM_BACK_ACC;			// �?速中(後進)
//		case DCMC_BACK_CONST:		return PARAM_BACK_CONST;		// 等速中(後進)
//		case DCMC_BACK_DEC:		return PARAM_BACK_DEC;			// 減速中(後進)
		case CTRL_SKEW_ACC:		return PARAM_SKEW_ACC;			// �?速中(直進)
		case CTRL_SKEW_CONST:		return PARAM_SKEW_CONST;		// 等速中(直進)
		case CTRL_SKEW_DEC:		return PARAM_SKEW_DEC;			// 減速中(直進)
		case CTRL_ACC_TRUN:		return PARAM_ACC_TRUN;			// �?速中(�?地信旋回)
		case CTRL_CONST_TRUN:		return PARAM_CONST_TRUN;		// 等速中(�?地信旋回)
		case CTRL_DEC_TRUN:		return PARAM_DEC_TRUN;			// 減速中(�?地信旋回)
		case CTRL_ENTRY_SURA:		return PARAM_ENTRY_SURA;		// スラロー�?前�?�前進動�?(スラロー�?)
		case CTRL_ACC_SURA:		return PARAM_ACC_SURA;			// �?速中(スラロー�?)
		case CTRL_CONST_SURA:		return PARAM_CONST_SURA;		// 等速中(スラロー�?)
		case CTRL_DEC_SURA:		return PARAM_DEC_SURA;			// 減速中(スラロー�?)
		case CTRL_EXIT_SURA:		return PARAM_EXIT_SURA;			// スラロー�?後�?�前進動�?(スラロー�?)
		default:			return PARAM_NC;
	}
}

void CTRL_getFF_speed( float* p_err )
{
	float f_ff_speed_acc = 0.0f;

	f_ff_speed_acc	= f_FF_speed_acc;

	/* 動作モードに応じ�? */
	switch( en_Type ){

		// �?�?
		case CTRL_ACC:
		case CTRL_HIT_WALL:
		case CTRL_SKEW_ACC:
		case CTRL_ACC_TRUN:
		case CTRL_ACC_SURA:
			*p_err = f_Acc * f_ff_speed_acc;
			break;

		case CTRL_CONST:
		case CTRL_SKEW_CONST:
		case CTRL_CONST_TRUN:
		case CTRL_ENTRY_SURA:
		case CTRL_EXIT_SURA:
		case CTRL_CONST_SURA:
			break;

		case CTRL_DEC:
		case CTRL_SKEW_DEC:
		case CTRL_DEC_TRUN:
		case CTRL_DEC_SURA:
			*p_err = f_Acc * f_ff_speed_acc * (-1);
			break;

		// �?速以�?
		default:
			*p_err = 0;
			break;										// 何もしな�?
	}

}

void CTRL_getFF_angle( float* p_err )
{
	float f_ff_angleS_acc = 0.0f;

	f_ff_angleS_acc = f_FF_angleS_acc;

	/* 動作モードに応じ�? */
	switch( en_Type ){

		// �?�?
		case CTRL_ACC:
		case CTRL_HIT_WALL:
		case CTRL_SKEW_ACC:
		case CTRL_ACC_TRUN:
		case CTRL_ACC_SURA:
			*p_err =FABS(f_AccAngleS) * f_ff_angleS_acc;
			break;

		case CTRL_CONST:
		case CTRL_SKEW_CONST:
		case CTRL_CONST_TRUN:
		case CTRL_ENTRY_SURA:
		case CTRL_EXIT_SURA:
		case CTRL_CONST_SURA:
			break;

		case CTRL_DEC:
		case CTRL_SKEW_DEC:
		case CTRL_DEC_TRUN:
		case CTRL_DEC_SURA:
			*p_err = FABS(f_AccAngleS) * f_ff_angleS_acc *(-1);
			break;

		// �?速以�?
		default:
			*p_err = 0;
			break;										// 何もしな�?
	}

}

void CTRL_getSpeedFB( float* p_err )
{
	float		f_speedErr;					// [速度制御] 速度偏差
	float		f_kp = 0.0f;
	float		f_ki = 0.0f;
	float		f_kd = 0.0f;
	/* 速度制御 */
	f_speedErr  = f_TrgtSpeed - f_NowSpeed;					// 速度偏差[mm/s]
	f_kp = f_FB_speed_kp;
	f_ki = f_FB_speed_ki;
	f_kd = f_FB_speed_kd;

	/* I成�??演�? */
	f_SpeedErrSum += f_speedErr;// * f_ki;			// I成�??更新
	if( f_SpeedErrSum > 10000.0 ){
		f_SpeedErrSum = 10000.0;			// 上限リミッター
	}

	/* PID制御 */
//	*p_err = f_speedErr * f_kp + f_SpeedErrSum + ( f_speedErr - f_ErrSpeedBuf ) * f_kd;				// PI制御量算�?�

	f_ErrSpeedBuf = f_speedErr;		// 偏差をバ�?ファリング

	/* 累積偏差クリア */
//	if( FABS( f_speedErr ) < 20 ){
//		if(FABS( f_speedErr ) < 0.1)f_SpeedErrSum = 0;
//		else f_SpeedErrSum /= 2;
//	}
	*p_err = f_speedErr * f_kp + f_SpeedErrSum* f_ki + ( f_speedErr - f_ErrSpeedBuf ) * f_kd;				// PI制御量算�?�

}

void CTRL_getAngleSpeedFB( float* p_err )
{
	float f_err;					// [入力] ジャイロセンサーエラー値
	float f_kp = 0.0f;				// 比例ゲイン
	float f_ki = 0.0f;
	float f_kd = 0.0f;


	f_err = f_TrgtAngleS - GYRO_getSpeedErr();			// 目標角度 - ジャイロセンサ[deg/s]
	f_kp = f_FB_angleS_kp;
	f_ki = f_FB_angleS_ki;
	f_kd = f_FB_angleS_kd;

	f_AngleSErrSum += f_err;//*f_ki;

	if(f_AngleSErrSum > 10000.0){
		f_AngleSErrSum = 10000.0;			//上限リミッター
	}
	else if(f_AngleSErrSum <-10000.0){
		f_AngleSErrSum = -10000.0;
	}

//	templog2 = f_AngleSErrSum;
//	*p_err = f_err * f_kp + f_AngleSErrSum + ( f_err - f_ErrAngleSBuf ) * f_kd;		// PID制御

	f_ErrAngleSBuf = f_err;		// 偏差をバ�?ファリング
	// 累積偏差クリア
//	if( ( en_Type == CTRL_ACC ) || ( en_Type == CTRL_CONST ) || ( en_Type == CTRL_DEC )||
//			 ( en_Type == CTRL_ENTRY_SURA ) || ( en_Type == CTRL_EXIT_SURA ) ){
//		if( FABS( f_err ) < 0.5 ){
//			f_AngleSErrSum = 0;
//		}
//	}
//	else{
//		if( FABS( f_err ) < 10 ){
//			if(FABS( f_err ) < 2)f_AngleSErrSum = 0;
//			else f_AngleSErrSum /= 2;
//		}
//	}

	*p_err = f_err * f_kp + f_AngleSErrSum*f_ki + ( f_err - f_ErrAngleSBuf ) * f_kd;		// PID制御
}

void CTRL_getSenFB( float* p_err )
{
	float f_err 	= 0;
	float f_kp 		= 0.0f;				// 比例ゲイン
	float f_kd 		= 0.0f;				// 微�?ゲイン
	float gyro		= 0.0f;

	/* 直進�? */
	if( ( en_Type == CTRL_ACC ) || ( en_Type == CTRL_CONST ) || ( en_Type == CTRL_DEC )||
			 ( en_Type == CTRL_ENTRY_SURA ) || ( en_Type == CTRL_EXIT_SURA ) ){

		f_kp = f_FB_wall_kp;
		f_kd = f_FB_wall_kd;

		/* 偏差取�? */
		DIST_getErr( &l_WallErr );
		f_err = (float)l_WallErr;
//		templog2 = f_err;
		/* PD制御 */
		*p_err = f_err * f_kp + ( f_err - f_ErrDistBuf ) * f_kd;		// PD制御

		f_ErrDistBuf = f_err;		// 偏差をバ�?ファリング
	}
	else if( ( en_Type == CTRL_SKEW_ACC ) || ( en_Type == CTRL_SKEW_CONST ) || ( en_Type == CTRL_SKEW_DEC ) ){

		DIST_getErrSkew( &l_WallErr );
		f_err = (float)l_WallErr;

//		*p_err = f_err * f_kp + ( f_err - f_ErrDistBuf ) * f_kd;		// PD制御
		*p_err = f_err;
	}

}

void CTRL_outMot( float f_duty10_R, float f_duty10_L )
{
	float	f_temp;			// 計算用

//	f_Duty_R = f_duty10_R;
//	f_Duty_L = f_duty10_L;

	/* 電圧に応じてPWM出力を変更する */
	f_duty10_R = f_duty10_R * VCC_MAX / (get_battLv());
	f_duty10_L = f_duty10_L * VCC_MAX / (get_battLv());

//	log_in(f_duty10_R);
	/* 右モータ */
	if( 20 < f_duty10_R ){									// 前進
		DCM_setDirCw( DCM_R );
		DCM_setPwmDuty( DCM_R, (uint16_t)f_duty10_R );
	}
	else if( f_duty10_R < -20 ){							// 後退
		f_temp = f_duty10_R * -1;
		DCM_setDirCcw( DCM_R );
		DCM_setPwmDuty( DCM_R, (uint16_t)f_temp );
	}
	else{
		DCM_brakeMot( DCM_R );								// ブレーキ
	}

	/* 左モータ */
	if( 20 < f_duty10_L ){									// 前進
		DCM_setDirCw( DCM_L );
		DCM_setPwmDuty( DCM_L, (uint16_t)f_duty10_L );
	}
	else if( f_duty10_L < -20 ){							// 後退
		f_temp = f_duty10_L * -1;
		DCM_setDirCcw( DCM_L );
		DCM_setPwmDuty( DCM_L, (uint16_t)f_temp );
	}
	else{
		DCM_brakeMot( DCM_L );								// ブレーキ
	}
}

void CTRL_pol( void )
{
	float f_feedFoard_speed		= 0;		// [制御] フィードフォワード制御
	float f_feedFoard_angle		= 0;
	float f_speedCtrl			= 0;		// [制御] 速度制御�?
	float f_angleSpeedCtrl			= 0;		// [制御] 角速度制御�?
	float f_distSenCtrl			= 0;		// [制御] 距離センサー制御�?
	float f_duty10_R;						// [出力] 右モータPWM-DUTY比[0.1%]
	float f_duty10_L;						// [出力] 左モータPWM-DUTY比[0.1%]

	/* 制御を行うか�?�チェ�?ク */
	if( uc_CtrlFlag != TRUE ){
		 return;		// 制御無効状�?
	}
	if(SW_ON == SW_IsOn_0()){
		Failsafe_flag();
	}

	/* 制御不�?� */
	if (SYS_isOutOfCtrl() == TRUE ){

		f_DistErrSum = 0;				// 累積偏差クリア
		f_NowDist = f_LastDist;			// 強制�?に最終目標位置に変更
		f_NowAngle = f_LastAngle;		// 強制�?に最終目標時間に変更
		f_Time = f_TrgtTime;			// 強制�?に最終目標時間に変更

	 	CTRL_stop();				// 制御停止
		CTRL_clrData();					// �?ータクリア
		DCM_brakeMot( DCM_R );			// ブレーキ
		DCM_brakeMot( DCM_L );			// ブレーキ
	}

	/* �?種センサ入�? */
	ENC_GetDiv( &l_CntR, &l_CntL );					// 移動量[カウント値]を取�?
	CTRL_refNow();									// 制御に使用する値を現在の状態に更新
	CTRL_refTarget();								// 制御に使用する値を目標値に更新

//	templog1 = l_CntR;
//	templog2 = l_CntL;
	f_NowAngle = GYRO_getNowAngle();					// 現在角度[deg]

	/* 制御値取�? */
	CTRL_getFF_speed( &f_feedFoard_speed );					// [制御] フィードフォワー�?
	CTRL_getFF_angle( &f_feedFoard_angle );					// [制御] フィードフォワー�?
	CTRL_getSpeedFB( &f_speedCtrl );				// [制御] 速度
	CTRL_getAngleSpeedFB( &f_angleSpeedCtrl );			// [制御] 角速度
	CTRL_getSenFB( &f_distSenCtrl );				// [制御] �?

//	templog1 = f_angleSpeedCtrl;
//	templog1 = f_distSenCtrl;

	/* 直進制御 */
	if( ( en_Type == CTRL_ACC ) || ( en_Type == CTRL_CONST ) || ( en_Type == CTRL_DEC ) ||( en_Type == CTRL_ENTRY_SURA ) || ( en_Type == CTRL_EXIT_SURA ) ||
		( en_Type == CTRL_SKEW_ACC ) || ( en_Type == CTRL_SKEW_CONST ) || ( en_Type == CTRL_SKEW_DEC )
	){
		escape_wait = escape_wait+0.001;
		f_duty10_R = f_feedFoard_speed * FF_BALANCE_R + f_speedCtrl + f_angleSpeedCtrl + f_distSenCtrl;	// �E���[�^PWM-DUTY��[0.1%]
		f_duty10_L = f_feedFoard_speed * FF_BALANCE_L + f_speedCtrl - f_angleSpeedCtrl - f_distSenCtrl;	// �����[�^PWM-DUTY��[0.1%]
	}

	/* 壁あて制御 */
	else if( en_Type == CTRL_HIT_WALL ){
		f_duty10_R = f_feedFoard_speed * FF_HIT_BALANCE_R * (-1);																		// 右モータPWM-DUTY比[0.1%]
		f_duty10_L = f_feedFoard_speed * FF_HIT_BALANCE_L * (-1);
	}

	/* スラロー�?制御 */
	else if( ( en_Type == CTRL_ACC_SURA ) || (en_Type == CTRL_CONST_SURA)||( en_Type == CTRL_DEC_SURA ) ){
		/* 左旋回 */
		if( f_LastAngle > 0 ){
			f_duty10_R = f_feedFoard_speed * FF_BALANCE_R + f_feedFoard_angle * FF_BALANCE_R + f_angleSpeedCtrl + f_speedCtrl + 100.0;		// �E���[�^PWM-DUTY��[0.1%]
			f_duty10_L = f_feedFoard_speed * FF_BALANCE_L + f_feedFoard_angle * FF_BALANCE_L * (-1) - f_angleSpeedCtrl + f_speedCtrl - 100.0;		// �����[�^PWM-DUTY��[0.1%]
		}
		/*右旋回 */
		else{
			f_duty10_R = f_feedFoard_speed * FF_BALANCE_R + f_feedFoard_angle * FF_BALANCE_R * (-1) + f_angleSpeedCtrl + f_speedCtrl - 100.0;		// �E���[�^PWM-DUTY��[0.1%]
			f_duty10_L = f_feedFoard_speed * FF_BALANCE_L + f_feedFoard_angle * FF_BALANCE_L - f_angleSpeedCtrl + f_speedCtrl + 100.0;		// �����[�^PWM-DUTY��[0.1%]
		}
	}


	/* �?信地旋回 */
	else{
		/* ������ */
		if( f_LastAngle > 0 ){
//			f_duty10_R = f_feedFoard * FF_BALANCE_R        + f_angleCtrl + f_angleSpeedCtrl;									// �E���[�^PWM-DUTY��[0.1%]
//			f_duty10_L = f_feedFoard * FF_BALANCE_L * (-1) - f_angleCtrl - f_angleSpeedCtrl;									// �����[�^PWM-DUTY��[0.1%]
			f_duty10_R = f_feedFoard_angle * FF_BALANCE_R        + f_angleSpeedCtrl + f_speedCtrl + 100.0;		// �E���[�^PWM-DUTY��[0.1%]
			f_duty10_L = f_feedFoard_angle * FF_BALANCE_L * (-1) - f_angleSpeedCtrl + f_speedCtrl - 100.0;		// �����[�^PWM-DUTY��[0.1%]
		}
		/* �E���� */
		else{
//			f_duty10_R = f_feedFoard * FF_BALANCE_R * (-1) + f_angleCtrl + f_angleSpeedCtrl;									// �E���[�^PWM-DUTY��[0.1%]
//			f_duty10_L = f_feedFoard * FF_BALANCE_L        - f_angleCtrl - f_angleSpeedCtrl;									// �����[�^PWM-DUTY��[0.1%]
			f_duty10_R = f_feedFoard_angle * FF_BALANCE_R * (-1) + f_angleSpeedCtrl + f_speedCtrl - 100.0;		// �E���[�^PWM-DUTY��[0.1%]
			f_duty10_L = f_feedFoard_angle * FF_BALANCE_L        - f_angleSpeedCtrl + f_speedCtrl + 100.0;		// �����[�^PWM-DUTY��[0.1%]
		}
		escape_wait = escape_wait+0.001;
	}
	templog2 = f_duty10_R;

	CTRL_outMot( f_duty10_R, f_duty10_L );				// モータへ出�?

	f_Time += 0.001;

	/* 壁�??れチェ�?ク */
	if( MOT_getWallEdgeType() == MOT_WALL_EDGE_RIGHT ){

		/* 壁抜�? */
		if( DIST_isWall_R_SIDE() == FALSE ){

			MOT_setWallEdge( TRUE );		// 壁�?��?れ目を検知
		}
	}
	else if( MOT_getWallEdgeType() == MOT_WALL_EDGE_LEFT ){

		/* 壁抜�? */
		if( DIST_isWall_L_SIDE() == FALSE ){

			MOT_setWallEdge( TRUE );		// 壁�?��?れ目を検知
		}
	}
}

void Failsafe_flag(void)
{
	bl_failsafe = TRUE;
	SetLED(0x1F);
}

void Failsafe_flag_off(void)
{
	bl_failsafe = FALSE;
}

bool SYS_isOutOfCtrl( void )
{
	if( bl_failsafe == TRUE ){
		return TRUE;
	}
	else{
		return FALSE;
	}
}


