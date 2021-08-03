/*
 * CTRL.c
 *
 *  Created on: 2021/01/30
 *      Author: shohe
 */


#include "hal/CTRL.h"


/* 制御  */
uint8_t 			uc_CtrlFlag			= FALSE;	// フィードバック or フィードフォワード 制御有効フラグ（FALSE:無効、1：有効）
int32_t			l_CntR;							// 右モータのカウント変化量						（1[msec]毎に更新される）
int32_t			l_CntL;							// 左モータのカウント変化量						（1[msec]毎に更新される）
// 制御
float			f_Time 				= 0;		// 動作時間[sec]								（1[msec]毎に更新される）
float			f_TrgtTime 			= 1000;		// 動作目標時間 [msec]							（設定値）
// 速度制御//////////////////////////////////////////
float 			f_Acc			= 0;		// [速度制御]   加速度							（設定値）
float			f_BaseSpeed		= 0;		// [速度制御]   初速度							（設定値）
float			f_LastSpeed 		= 0;		// [速度制御]   最終目標速度					（設定値）
float			f_NowSpeed		= 0;		// [速度制御]   現在の速度 [mm/s]				（1[msec]毎に更新される）
 float			f_TrgtSpeed 		= 0;		// [速度制御]   目標移動速度 [mm/s]				（1[msec]毎に更新される）
float			f_ErrSpeedBuf		= 0;		// [速度制御] 　速度エラー値のバッファ	（1[msec]毎に更新される）
float			f_SpeedErrSum 		= 0;		// [速度制御]   速度積分制御のサム値			（1[msec]毎に更新される）
// 距離制御
float			f_BaseDist		= 0;		// [距離制御]   初期位置						（設定値）
float			f_LastDist 		= 0;		// [距離制御]   最終移動距離					（設定値）
float			f_TrgtDist 		= 0;		// [距離制御]   目標移動距離					（1[msec]毎に更新される）
volatile float 		f_NowDist		= 0;		// [距離制御]   現在距離						（1[msec]毎に更新される）
float			f_NowDistR		= 0;		// [距離制御]   現在距離（右）					（1[msec]毎に更新される）
float 			f_NowDistL		= 0;		// [距離制御]   現在距離（左）					（1[msec]毎に更新される）
float			f_DistErrSum 		= 0;		// [距離制御]   距離積分制御のサム値			（1[msec]毎に更新される）
// 角速度制御
float 			f_AccAngleS		= 0;		// [角速度制御] 角加速度						（設定値）
float			f_BaseAngleS		= 0;		// [角速度制御] 初期角速度						（設定値）
float			f_LastAngleS 		= 0;		// [角速度制御] 最終目標角速度					（設定値）
float			f_TrgtAngleS 		= 0;		// [角速度制御] 目標角速度 [rad/s]				（1[msec]毎に更新される）
float			f_ErrAngleSBuf		= 0;		// [角速度制御] 角速度エラー値のバッファ	（1[msec]毎に更新される）
float			f_AngleSErrSum 		= 0;		// [角速度制御]   角度積分制御のサム値			（1[msec]毎に更新される）
// 角度制御
float			f_BaseAngle		= 0;		// [角度制御]   初期角度						（設定値）
float			f_LastAngle 		= 0;		// [角度制御]   最終目標角度					（設定値）
volatile float 		f_NowAngle		= 0;		// [角度制御]   現在角度　	volatileをつけないとwhileから抜けられなくなる（最適化のせい）（1[msec]毎に更新される）
float			f_TrgtAngle 		= 0;		// [角度制御]   目標角度						（1[msec]毎に更新される）
float			f_AngleErrSum 		= 0;		// [角度制御]   角度積分制御のサム値			（1[msec]毎に更新される）
// 壁制御
int32_t 			l_WallErr 		= 0;		// [壁制御]     壁との偏差						（1[msec]毎に更新される）
float			f_ErrDistBuf		= 0;		// [壁制御]     距離センサーエラー値のバッファ	（1[msec]毎に更新される）

//フェイルセーフ
float  			f_ErrChkAngle; 			  // ジャイロセンサのエラー検出用の角度
bool   			bl_ErrChk; 				  // ジャイロセンサのエラー検出（FALSE：検知しない、TRUE：検知する）
bool			bl_failsafe		= FALSE;	// マウスがの制御不能（TRUE：制御不能、FALSE：制御可能）


float Get_NowSpeed(void){
	return f_NowSpeed;
}

float Get_NowDist(void){
	return f_NowDist;
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
	if( Msec_in > 999 ){		// msec → sec
		Msec_in  = 0;
		Sec_in++;
	}
	if( Sec_in > 59 ){			// sec → min
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
//	recv_spi_encoder();								// エンコーダモジュール初期化
//	ENC_R_CNT_old	= ENC_R_CNT;
//	ENC_L_CNT_old	= ENC_L_CNT;
//	ENC_setref();
	l_CntR			= 0;						// カウンタクリア
	l_CntL			= 0;						// カウンタクリア

	/* 現在値 */
	f_NowDist 		= 0;						// 移動距離リセット
	f_NowDistR 		= 0;
	f_NowDistL 		= 0;
	f_NowSpeed		= 0;						// [速度制御]   現在の速度 [mm/s]			（1[msec]毎に更新される）
	f_NowAngle		= 0;						// [角度制御]   現在角度					（1[msec]毎に更新される）
	s_GyroVal		= 0;						// ジャイロ値クリア
	f_GyroNowAngle	= 0;							// ジャイロ値クリア

	/* 目標値 */
	f_TrgtSpeed		= 0;						// [速度制御]   目標移動速度 [mm/s]			（1[msec]毎に更新される）
	f_TrgtDist 		= 0;						// [距離制御]   目標移動距離				（1[msec]毎に更新される）
	f_TrgtAngleS	= 0;							// [角速度制御] 目標角速度 [rad/s]			（1[msec]毎に更新される）
	f_TrgtAngle		= 0;						// [角度制御]   目標角度					（1[msec]毎に更新される）

	/* 制御データ */
	f_SpeedErrSum	= 0;
	f_DistErrSum 	= 0;						// [距離制御]   距離積分制御のサム値			（1[msec]毎に更新される）
	f_AngleSErrSum	= 0;
	f_AngleErrSum 	= 0;						// [角度制御]   角度積分制御のサム値			（1[msec]毎に更新される）
	f_ErrSpeedBuf	= 0;
	f_ErrDistBuf	= 0;						// [壁制御]     距離センサーエラー値のバッファ		（1[msec]毎に更新される）
	f_ErrAngleSBuf  = 0;
}

void CTRL_setData( stCTRL_DATA* p_data )
{
	/* 制御方法 */
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

	straight_wait			= 0;

	CTRL_sta();				// 制御開始

}

void CTRL_refNow( void )
{
	float f_speedR		= 0;							// 右モータ現在速度 [mm/s]
	float f_speedL		= 0;							// 左モータ現在速度 [mm/s]
	float f_r 			= F_CNT2MM(l_CntR);				// 右モータの進んだ距離 [mm]
	float f_l 			= F_CNT2MM(l_CntL);				// 左モータの進んだ距離 [mm]

	/* 速度更新 */
	f_speedR = f_r * 1000;								// 右モータ速度 [mm/s] ( 移動距離[カウント] * 1パルスの移動量(0.0509[mm]) * 1000(msec→sec)
	f_speedL = f_l * 1000;								// 左モータ速度 [mm/s] ( 移動距離[カウント] * 1パルスの移動量(0.0509[mm]) * 1000(msec→sec)
	f_NowSpeed  = ( f_speedR + f_speedL ) / 2;			// マウス（進行方向中心軸） [1mm/s]

	/* 距離更新 */
	f_NowDistR += f_r;									// カウント更新
	f_NowDistL += f_l;									// カウント更新
	f_NowDist  = ( f_NowDistR + f_NowDistL ) / 2;		// 平均値更新
}

void CTRL_refTarget( void )
{
	/* mode */
	switch( en_Type ){

		/* acc(straight) */
		case CTRL_ACC:
		case CTRL_SKEW_ACC:
			if( f_TrgtSpeed < (f_LastSpeed -(f_Acc * 0.001)) ){												// 加速目標更新区間
				f_TrgtSpeed = f_BaseSpeed + f_Acc * f_Time;									// 目標速度
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
			if( f_TrgtSpeed > (f_LastSpeed -(f_Acc * 0.001))){												// 減速目標更新区間
				f_TrgtSpeed = f_BaseSpeed - f_Acc * f_Time;									// 目標速度
				f_TrgtDist  = f_BaseDist + ( f_BaseSpeed + f_TrgtSpeed ) * f_Time / 2;		// 目標距離
			}
			/* position CTRL */
			else{
				f_TrgtDist  = f_LastDist;													// 目標距離
			}
			break;

		/* acc(Turn) */
		case CTRL_ACC_TRUN:

			/* CW */
			if( ( f_LastAngle > 0 ) && ( f_TrgtAngleS < (f_LastAngleS +(f_AccAngleS * 0.001)) ) ){
				f_TrgtAngleS = 0 + f_AccAngleS * f_Time;									// 目標角速度
			}
			/* CCW */
			else if( ( f_LastAngle < 0 ) && ( f_TrgtAngleS > (f_LastAngleS -(f_AccAngleS * 0.001)) ) ){
				f_TrgtAngleS = 0 - f_AccAngleS * f_Time;									// 目標角速度
			}
			break;

		/* const(Turn) */
		case CTRL_CONST_TRUN:
//			f_TrgtAngleS =f_BaseAngleS;
			break;

		/* dec(Turn) */
		case CTRL_DEC_TRUN:

			/* CW */
			if( f_LastAngle > 0 ){

				/* Angle speed CTRL + Angle CTRL */
				if( f_TrgtAngleS > (f_LastAngleS -(f_AccAngleS * 0.001)) ){												// 減速目標更新区間
					f_TrgtAngleS = f_BaseAngleS - f_AccAngleS * f_Time;							// 目標角速度
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// 目標角度
				}
				/* Angle CTRL */
				else{
					f_TrgtAngle  = f_LastAngle;													// 目標距離
				}
			}
			/* CCW */
			else{

				/* Angle speed CTRL + Angle CTRL */
				if( f_TrgtAngleS < (f_LastAngleS +(f_AccAngleS * 0.001))){												// 減速目標更新区間
					f_TrgtAngleS = f_BaseAngleS + f_AccAngleS * f_Time;							// 目標角速度
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// 目標角度
				}
				/* Angle CTRL */
				else{
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

		/* acc(スラローム) */
		case CTRL_ACC_SURA:
			f_TrgtSpeed = f_BaseSpeed;

			/* CW */
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
			/* CCW */
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
			if( f_LastDist > (f_TrgtDist - (f_TrgtSpeed * 0.001)) ){													// 目標更新区間
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

			/* CW */
			if( f_LastAngle > 0 ){
				if( f_TrgtAngle < (f_LastAngle +(f_AccAngleS * 0.001)) ){
					f_TrgtAngle  = f_BaseAngle + f_TrgtAngleS * f_Time;			// 目標角度
				}
				else{
					f_TrgtAngle  = f_LastAngle;									// 目標角度
				}
			}
			/* CCW */
			else{
				if( f_TrgtAngle > (f_LastAngle -(f_AccAngleS * 0.001)) ){
					f_TrgtAngle  = f_BaseAngle + f_TrgtAngleS * f_Time;			// 目標角度
				}
				else{
					f_TrgtAngle  = f_LastAngle;									// 目標角度
				}
			}

			/* Position CTRL */
			if( f_LastDist > (f_TrgtDist - (f_TrgtSpeed * 0.001)) ){													// 目標更新区間
				f_TrgtDist  = f_BaseDist + f_TrgtSpeed * f_Time;							// 目標位置
			}
			else{
				f_TrgtDist  = f_LastDist;													// 目標距離
			}
			break;

		/* dec(sura) */
		case CTRL_DEC_SURA:
			f_TrgtSpeed = f_BaseSpeed;

			/* CW */
			if( f_LastAngle > 0 ){
				if( f_TrgtAngleS > (f_LastAngle -(f_AccAngleS * 0.001)) ){
					f_TrgtAngleS = f_BaseAngleS + f_AccAngleS * f_Time;							// 目標角速度
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// 目標角度
				}
				else{
					f_TrgtAngle  = f_LastAngle;													// 目標距離
				}
			}
			/*CCW*/
			else{
				if( f_TrgtAngleS < (f_LastAngle +(f_AccAngleS * 0.001)) ){
					f_TrgtAngleS = f_BaseAngleS + f_AccAngleS * f_Time;							// 目標角速度
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// 目標角度
				}
				else{
					f_TrgtAngle  = f_LastAngle;													// 目標距離
				}
			}

			/* Position CTRL */
			if( f_LastDist > (f_TrgtDist - (f_TrgtSpeed * 0.001)) ){													// 目標更新区間
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
		case CTRL_ACC:			return PARAM_ACC;				// 加速中(直進)
		case CTRL_CONST:		return PARAM_CONST;				// 等速中(直進)
		case CTRL_DEC:			return PARAM_DEC;				// 減速中(直進)
		case CTRL_HIT_WALL:		return PARAM_HIT_WALL;			// 壁あて制御
//		case DCMC_BACK_ACC:		return PARAM_BACK_ACC;			// 加速中(後進)
//		case DCMC_BACK_CONST:		return PARAM_BACK_CONST;		// 等速中(後進)
//		case DCMC_BACK_DEC:		return PARAM_BACK_DEC;			// 減速中(後進)
		case CTRL_SKEW_ACC:		return PARAM_SKEW_ACC;			// 加速中(直進)
		case CTRL_SKEW_CONST:		return PARAM_SKEW_CONST;		// 等速中(直進)
		case CTRL_SKEW_DEC:		return PARAM_SKEW_DEC;			// 減速中(直進)
		case CTRL_ACC_TRUN:		return PARAM_ACC_TRUN;			// 加速中(超地信旋回)
		case CTRL_CONST_TRUN:		return PARAM_CONST_TRUN;		// 等速中(超地信旋回)
		case CTRL_DEC_TRUN:		return PARAM_DEC_TRUN;			// 減速中(超地信旋回)
		case CTRL_ENTRY_SURA:		return PARAM_ENTRY_SURA;		// スラローム前の前進動作(スラローム)
		case CTRL_ACC_SURA:		return PARAM_ACC_SURA;			// 加速中(スラローム)
		case CTRL_CONST_SURA:		return PARAM_CONST_SURA;		// 等速中(スラローム)
		case CTRL_DEC_SURA:		return PARAM_DEC_SURA;			// 減速中(スラローム)
		case CTRL_EXIT_SURA:		return PARAM_EXIT_SURA;			// スラローム後の前進動作(スラローム)
		default:			return PARAM_NC;
	}
}

void CTRL_getFF_speed( float* p_err )
{
	float f_ff_speed_acc = 0.0f;
	float f_ff_speed = 0.0f;

	f_ff_speed_acc	= PARAM_getGain( Chg_ParamID(en_Type) )->f_FF_speed_acc;
	f_ff_speed		= PARAM_getGain( Chg_ParamID(en_Type) )->f_FF_speed;

	/* 動作モードに応じる */
	switch( en_Type ){

		// 加速
		case CTRL_ACC:
		case CTRL_HIT_WALL:
		case CTRL_SKEW_ACC:
		case CTRL_ACC_TRUN:
		case CTRL_ACC_SURA:
			*p_err = f_Acc * f_ff_speed_acc + f_TrgtSpeed * f_ff_speed ;
			break;

		case CTRL_CONST:
		case CTRL_SKEW_CONST:
		case CTRL_CONST_TRUN:
		case CTRL_ENTRY_SURA:
		case CTRL_EXIT_SURA:
		case CTRL_CONST_SURA:
			*p_err = f_TrgtSpeed * f_ff_speed ;
			break;

		case CTRL_DEC:
		case CTRL_SKEW_DEC:
		case CTRL_DEC_TRUN:
		case CTRL_DEC_SURA:
			*p_err = f_Acc * f_ff_speed_acc * (-1) + f_TrgtSpeed * f_ff_speed;
			break;

		// 加速以外
		default:
			*p_err = 0;
			break;										// 何もしない
	}

}

void CTRL_getFF_angle( float* p_err )
{
	float f_ff_angleS_acc = 0.0f;
	float f_ff_angleS = 0.0f;

	f_ff_angleS_acc = PARAM_getGain( Chg_ParamID(en_Type) )->f_FF_angleS_acc;
	f_ff_angleS 	= PARAM_getGain( Chg_ParamID(en_Type) )->f_FF_angleS;

	/* 動作モードに応じる */
	switch( en_Type ){

		// 加速
		case CTRL_ACC:
		case CTRL_HIT_WALL:
		case CTRL_SKEW_ACC:
		case CTRL_ACC_TRUN:
		case CTRL_ACC_SURA:
			*p_err =FABS(f_AccAngleS) * f_ff_angleS_acc + FABS(f_TrgtAngleS) * f_ff_angleS;
			break;

		case CTRL_CONST:
		case CTRL_SKEW_CONST:
		case CTRL_CONST_TRUN:
		case CTRL_ENTRY_SURA:
		case CTRL_EXIT_SURA:
		case CTRL_CONST_SURA:
			*p_err = FABS(f_TrgtAngleS) * f_ff_angleS;
			break;

		case CTRL_DEC:
		case CTRL_SKEW_DEC:
		case CTRL_DEC_TRUN:
		case CTRL_DEC_SURA:
			*p_err = FABS(f_AccAngleS) * f_ff_angleS_acc *(-1) + FABS(f_TrgtAngleS) * f_ff_angleS;
			break;

		// 加速以外
		default:
			*p_err = 0;
			break;										// 何もしない
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
	f_kp = PARAM_getGain( Chg_ParamID(en_Type))->f_FB_speed_kp;
	f_ki = PARAM_getGain( Chg_ParamID(en_Type))->f_FB_speed_ki;
	f_kd = PARAM_getGain( Chg_ParamID(en_Type))->f_FB_speed_kd;

	/* I成分演算 */
	f_SpeedErrSum += f_speedErr * f_ki;			// I成分更新
	if( f_SpeedErrSum > 120 ){
		f_SpeedErrSum = 120;			// 上限リミッター
	}

	/* PID制御 */
	*p_err = f_speedErr * f_kp + f_SpeedErrSum + ( f_speedErr - f_ErrSpeedBuf ) * f_kd;				// PI制御量算出

	f_ErrSpeedBuf = f_speedErr;		// 偏差をバッファリング

	/* 累積偏差クリア */
	if( FABS( f_speedErr ) < 0.5 ){
		f_SpeedErrSum = 0;
	}

}

void CTRL_getDistFB( float* p_err )
{
	float				f_distErr;					// [距離制御] 距離偏差
//	PRIVATE float		f_limTime = 0;				// 飽和状態維持時間[sec]
	float 				f_kp = 0.0f;				// 比例ゲイン
	float 				f_ki = 0.0f;				// 積分ゲイン

	*p_err = 0;

	/* 加速/等速の位置制御 */
	if( ( en_Type == CTRL_ACC ) || ( en_Type == CTRL_CONST )||
		( en_Type == CTRL_SKEW_ACC ) || ( en_Type == CTRL_SKEW_CONST ))
	{
		// なにもしない
	}
	/* 減速のみ位置制御 */
	else if(( en_Type == CTRL_DEC )|| ( en_Type == CTRL_SKEW_DEC ) ||
			 ( en_Type == CTRL_ENTRY_SURA ) || ( en_Type == CTRL_EXIT_SURA ) ||
			 ( en_Type == CTRL_ACC_SURA ) || ( en_Type == CTRL_CONST_SURA ) || ( en_Type == CTRL_DEC_SURA ) ){

		f_kp = PARAM_getGain( Chg_ParamID(en_Type) )->f_FB_dist_kp;
		f_ki = PARAM_getGain( Chg_ParamID(en_Type) )->f_FB_dist_ki;

		/* 位置制御 */
		f_distErr  = f_TrgtDist - f_NowDist;					// 距離偏差[mm]

		/* I成分演算 */
		f_DistErrSum += f_distErr * f_ki;			// I成分更新
		if( f_DistErrSum > 100 ){
			f_DistErrSum = 100;			// 上限リミッター
		}

		/* PI制御 */
		*p_err = f_distErr * f_kp + f_DistErrSum;				// PI制御量算出

		/* 累積偏差クリア */
		if( FABS( f_TrgtDist - f_NowDist ) < 0.05 ){
			f_DistErrSum = 0;
		}
	}

/* 超信地旋回 */
	else if( ( en_Type == CTRL_ACC_TRUN ) || ( en_Type == CTRL_CONST_TRUN ) || ( en_Type == CTRL_DEC_TRUN ) ){
		f_kp = PARAM_getGain( Chg_ParamID(en_Type) )->f_FB_dist_kp;
		f_ki = PARAM_getGain( Chg_ParamID(en_Type) )->f_FB_dist_ki;

		/* 位置制御 */
		f_distErr  = f_TrgtDist - f_NowDist;					// 距離偏差[mm]

		/* I成分演算 */
		f_DistErrSum += f_distErr * f_ki;			// I成分更新
		if( f_DistErrSum > 100 ){
			f_DistErrSum = 100;			// 上限リミッター
		}

		/* PI制御 */
		*p_err = f_distErr * f_kp + f_DistErrSum;				// PI制御量算出

		/* 累積偏差クリア */
		if( FABS( f_TrgtDist - f_NowDist ) < 0.05 ){
			f_DistErrSum = 0;
		}

	}

}

void CTRL_getAngleSpeedFB( float* p_err )
{
	float f_err;					// [入力] ジャイロセンサーエラー値
	float f_kp = 0.0f;				// 比例ゲイン
	float f_ki = 0.0f;
	float f_kd = 0.0f;


	f_err = f_TrgtAngleS - GYRO_getSpeedErr();			// 目標角度 - ジャイロセンサ[deg/s]
	f_kp = PARAM_getGain( Chg_ParamID(en_Type) )->f_FB_angleS_kp;
	f_ki = PARAM_getGain( Chg_ParamID(en_Type) )->f_FB_angleS_ki;
	f_kd = PARAM_getGain( Chg_ParamID(en_Type) )->f_FB_angleS_kd;

	f_AngleSErrSum += f_err*f_ki;

	if(f_AngleSErrSum > 100){
		f_AngleSErrSum = 100;			//上限リミッター
	}
	else if(f_AngleSErrSum <-100){
		f_AngleSErrSum = -100;
	}

//	templog2 = f_AngleSErrSum;
	*p_err = f_err * f_kp + f_AngleSErrSum + ( f_err - f_ErrAngleSBuf ) * f_kd;		// PID制御

	f_ErrAngleSBuf = f_err;		// 偏差をバッファリング

	// 累積偏差クリア
	if( FABS( f_err ) < 0.3 ){
		f_AngleSErrSum = 0;
	}

}

void CTRL_getAngleFB( float* p_err )
{
	float f_err;					// [入力] 角度偏差[deg]
	float f_kp = 0.0f;				// 比例ゲイン
	float f_ki = 0.0f;				// 積分ゲイン

	*p_err = 0;

	f_NowAngle = GYRO_getNowAngle();					// 現在角度[deg]

	f_err = f_TrgtAngle - f_NowAngle;
	/* 直進時 */
	if( ( en_Type == CTRL_ACC ) || ( en_Type == CTRL_CONST ) || ( en_Type == CTRL_DEC )||
		( en_Type == CTRL_ENTRY_SURA ) || ( en_Type == CTRL_EXIT_SURA )||
		( en_Type == CTRL_SKEW_ACC ) || ( en_Type == CTRL_SKEW_CONST ) || ( en_Type == CTRL_SKEW_DEC )
	){
		f_kp = PARAM_getGain( Chg_ParamID(en_Type) )->f_FB_angle_kp;
		f_ki = PARAM_getGain( Chg_ParamID(en_Type) )->f_FB_angle_ki;

		f_AngleErrSum += f_err*f_ki;	//I成分更新
		if(f_AngleErrSum > 200){
			f_AngleErrSum = 200;			//上限リミッター
		}
		else if(f_AngleErrSum <-200){
			f_AngleErrSum = -200;
		}

		//*p_err = f_err * FB_ANG_KP_GAIN;					// P制御量算出
		*p_err = f_err * f_kp + f_AngleErrSum;					// PI制御量算出
//		templog2 = f_AngleErrSum;

		/* 累積偏差クリア */
		if( FABS( f_TrgtAngle - f_NowAngle ) < 0.3 ){
			f_AngleErrSum = 0;
		}

	}

	/* 超信地旋回時減速 */
	else if(( en_Type == CTRL_DEC_TRUN )||
			 ( en_Type == CTRL_ACC_SURA ) || ( en_Type == CTRL_CONST_SURA ) || ( en_Type == CTRL_DEC_SURA ))
	{
		f_kp = PARAM_getGain( Chg_ParamID(en_Type) )->f_FB_angle_kp;
		f_ki = PARAM_getGain( Chg_ParamID(en_Type) )->f_FB_angle_ki;

		f_AngleErrSum += f_err*f_ki;	//I成分更新
		if(f_AngleErrSum > 500){
			f_AngleErrSum = 500;			//上限リミッター
		}
		else if(f_AngleErrSum <-500){
			f_AngleErrSum = -500;
		}

		//*p_err = f_err * FB_ANG_KP_GAIN;					// P制御量算出
		*p_err = f_err * f_kp + f_AngleErrSum;					// PI制御量算出
//		templog2 = f_AngleErrSum;

		/* 累積偏差クリア */
		if( FABS( f_TrgtAngle - f_NowAngle ) < 0.1 ){
			f_AngleErrSum = 0;
		}
	}

}

void CTRL_getSenFB( float* p_err )
{
	float f_err 	= 0;
	float f_kp 		= 0.0f;				// 比例ゲイン
	float f_kd 		= 0.0f;				// 微分ゲイン
	float gyro		= 0.0f;

	/* 直進時 */
	if( ( en_Type == CTRL_ACC ) || ( en_Type == CTRL_CONST ) || ( en_Type == CTRL_DEC )||
			 ( en_Type == CTRL_ENTRY_SURA ) || ( en_Type == CTRL_EXIT_SURA ) ){

		f_kp = PARAM_getGain( Chg_ParamID(en_Type) )->f_FB_wall_kp;
		f_kd = PARAM_getGain( Chg_ParamID(en_Type) )->f_FB_wall_kd;

		/* 偏差取得 */
		DIST_getErr( &l_WallErr );
		f_err = (float)l_WallErr;
//		templog2 = f_err;
		/* PD制御 */
		*p_err = f_err * f_kp + ( f_err - f_ErrDistBuf ) * f_kd;		// PD制御

		f_ErrDistBuf = f_err;		// 偏差をバッファリング
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
	float f_speedCtrl			= 0;		// [制御] 速度制御量
	float f_distCtrl			= 0;		// [制御] 距離制御量
	float f_angleSpeedCtrl			= 0;		// [制御] 角速度制御量
	float f_angleCtrl			= 0;		// [制御] 角度制御量
	float f_distSenCtrl			= 0;		// [制御] 距離センサー制御量
	float f_duty10_R;						// [出力] 右モータPWM-DUTY比[0.1%]
	float f_duty10_L;						// [出力] 左モータPWM-DUTY比[0.1%]

	/* 制御を行うかのチェック */
	if( uc_CtrlFlag != TRUE ){
		 return;		// 制御無効状態
	}
	if(SW_ON == SW_IsOn_0()){
		Failsafe_flag();
	}

	/* 制御不能 */
	if (SYS_isOutOfCtrl() == TRUE ){

		f_DistErrSum = 0;				// 累積偏差クリア
		f_NowDist = f_LastDist;			// 強制的に最終目標位置に変更
		f_NowAngle = f_LastAngle;		// 強制的に最終目標時間に変更
		f_Time = f_TrgtTime;			// 強制的に最終目標時間に変更

	 	CTRL_stop();				// 制御停止
		CTRL_clrData();					// データクリア
		DCM_brakeMot( DCM_R );			// ブレーキ
		DCM_brakeMot( DCM_L );			// ブレーキ
	}

	/* 各種センサ入力 */
	ENC_GetDiv( &l_CntR, &l_CntL );					// 移動量[カウント値]を取得
	CTRL_refNow();									// 制御に使用する値を現在の状態に更新
	CTRL_refTarget();								// 制御に使用する値を目標値に更新

//	templog1 = l_CntR;
//	templog2 = l_CntL;

	/* 制御値取得 */
	CTRL_getFF_speed( &f_feedFoard_speed );					// [制御] フィードフォワード
	CTRL_getFF_angle( &f_feedFoard_angle );					// [制御] フィードフォワード
	CTRL_getSpeedFB( &f_speedCtrl );				// [制御] 速度
	CTRL_getDistFB( &f_distCtrl );					// [制御] 距離
	CTRL_getAngleSpeedFB( &f_angleSpeedCtrl );			// [制御] 角速度
	CTRL_getAngleFB( &f_angleCtrl );				// [制御] 角度
	CTRL_getSenFB( &f_distSenCtrl );				// [制御] 壁

//	templog1 = f_angleSpeedCtrl;
//	templog1 = f_distSenCtrl;

	/* 直進制御 */
	if( ( en_Type == CTRL_ACC ) || ( en_Type == CTRL_CONST ) || ( en_Type == CTRL_DEC ) ||( en_Type == CTRL_ENTRY_SURA ) || ( en_Type == CTRL_EXIT_SURA ) ||
		( en_Type == CTRL_SKEW_ACC ) || ( en_Type == CTRL_SKEW_CONST ) || ( en_Type == CTRL_SKEW_DEC )
	){
		straight_wait = straight_wait+0.001;
		f_duty10_R = f_feedFoard_speed * FF_BALANCE_R +  f_distCtrl + f_speedCtrl + f_angleCtrl + f_angleSpeedCtrl + f_distSenCtrl;	// 右モータPWM-DUTY比[0.1%]
		f_duty10_L = f_feedFoard_speed * FF_BALANCE_L +  f_distCtrl + f_speedCtrl - f_angleCtrl - f_angleSpeedCtrl - f_distSenCtrl;	// 左モータPWM-DUTY比[0.1%]
	}

	/* 壁あて制御 */
	else if( en_Type == CTRL_HIT_WALL ){
		f_duty10_R = f_feedFoard_speed * FF_HIT_BALANCE_R * (-1);																		// 右モータPWM-DUTY比[0.1%]
		f_duty10_L = f_feedFoard_speed * FF_HIT_BALANCE_L * (-1);
	}

	/* スラローム制御 */
	else if( ( en_Type == CTRL_ACC_SURA ) || (en_Type == CTRL_CONST_SURA)||( en_Type == CTRL_DEC_SURA ) ){
		/* 左旋回 */
		if( f_LastAngle > 0 ){
			f_duty10_R = f_feedFoard_speed * FF_BALANCE_R + f_feedFoard_angle * FF_BALANCE_R + f_angleCtrl + f_angleSpeedCtrl +  f_distCtrl + f_speedCtrl;		// 右モータPWM-DUTY比[0.1%]
			f_duty10_L = f_feedFoard_speed * FF_BALANCE_L + f_feedFoard_angle * FF_BALANCE_L * (-1) - f_angleCtrl - f_angleSpeedCtrl +  f_distCtrl + f_speedCtrl;		// 左モータPWM-DUTY比[0.1%]
		}
		/*右旋回 */
		else{
			f_duty10_R = f_feedFoard_speed * FF_BALANCE_R + f_feedFoard_angle * FF_BALANCE_R * (-1) + f_angleCtrl + f_angleSpeedCtrl +  f_distCtrl + f_speedCtrl;		// 右モータPWM-DUTY比[0.1%]
			f_duty10_L = f_feedFoard_speed * FF_BALANCE_L + f_feedFoard_angle * FF_BALANCE_L - f_angleCtrl - f_angleSpeedCtrl +  f_distCtrl + f_speedCtrl;		// 左モータPWM-DUTY比[0.1%]
		}
	}


	/* 超信地旋回 */
	else{
		/* 左旋回 */
		if( f_LastAngle > 0 ){
//			f_duty10_R = f_feedFoard * FF_BALANCE_R        + f_angleCtrl + f_angleSpeedCtrl;									// 右モータPWM-DUTY比[0.1%]
//			f_duty10_L = f_feedFoard * FF_BALANCE_L * (-1) - f_angleCtrl - f_angleSpeedCtrl;									// 左モータPWM-DUTY比[0.1%]
			f_duty10_R = f_feedFoard_angle * FF_BALANCE_R        + f_angleCtrl + f_angleSpeedCtrl +  f_distCtrl + f_speedCtrl;		// 右モータPWM-DUTY比[0.1%]
			f_duty10_L = f_feedFoard_angle * FF_BALANCE_L * (-1) - f_angleCtrl - f_angleSpeedCtrl +  f_distCtrl + f_speedCtrl;		// 左モータPWM-DUTY比[0.1%]
		}
		/* 右旋回 */
		else{
//			f_duty10_R = f_feedFoard * FF_BALANCE_R * (-1) + f_angleCtrl + f_angleSpeedCtrl;									// 右モータPWM-DUTY比[0.1%]
//			f_duty10_L = f_feedFoard * FF_BALANCE_L        - f_angleCtrl - f_angleSpeedCtrl;									// 左モータPWM-DUTY比[0.1%]
			f_duty10_R = f_feedFoard_angle * FF_BALANCE_R * (-1) + f_angleCtrl + f_angleSpeedCtrl +  f_distCtrl + f_speedCtrl;		// 右モータPWM-DUTY比[0.1%]
			f_duty10_L = f_feedFoard_angle * FF_BALANCE_L        - f_angleCtrl - f_angleSpeedCtrl +  f_distCtrl + f_speedCtrl;		// 左モータPWM-DUTY比[0.1%]
		}
	}

	CTRL_outMot( f_duty10_R, f_duty10_L );				// モータへ出力

	f_Time += 0.001;

	/* 壁切れチェック */
	if( MOT_getWallEdgeType() == MOT_WALL_EDGE_RIGHT ){

		/* 壁抜け */
		if( DIST_isWall_R_SIDE() == FALSE ){

			MOT_setWallEdge( TRUE );		// 壁の切れ目を検知
		}
	}
	else if( MOT_getWallEdgeType() == MOT_WALL_EDGE_LEFT ){

		/* 壁抜け */
		if( DIST_isWall_L_SIDE() == FALSE ){

			MOT_setWallEdge( TRUE );		// 壁の切れ目を検知
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


