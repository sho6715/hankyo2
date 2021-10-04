/*
 * run.c
 *
 *  Created on: 2021/01/30
 *      Author: shohe
 */


#include "hal/run.h"

#define MOT_MOVE_ST_THRESHOLD			( 0.013 )							// 直進移動距離の閾値[mm]
#define MOT_MOVE_ST_MIN					( 0.010 )							// 直進移動距離の最低移動量[mm]

//20170815 �?信地旋回実�?時に追�?
#define A1_MIN					( 25.0f/180.0f*PI )						// 第1最低移動角度
#define A2_MIN					( 30.0f/180.0f*PI )						// 第2最低移動角度
#define A3_MIN					( 20.0f/180.0f*PI )						// 第3最低移動角度

#define ANGLE_OFFSET1_R				( 0 )	//-12					// 角度のオフセ�?ト値?��バ�?ファリングによる誤差を埋めるための値?�?
#define ANGLE_OFFSET1				( 0 )	//-12					// 角度のオフセ�?ト値?��バ�?ファリングによる誤差を埋めるための値?�?
#define ANGLE_OFFSET2_R				( 0 )	//3
#define ANGLE_OFFSET2				( 0 )						// 角度のオフセ�?ト値?��バ�?ファリングによる誤差を埋めるための値?�?
#define ANGLE_OFFSET3				( 0 )					// 角度のオフセ�?ト値?��バ�?ファリングによる誤差を埋めるための値?�?

#define IS_R_SLA(a)			( ( (a) % 2 == 0 ) ? (TRUE) : (FALSE))

typedef struct{

	float			f_time;			// 時間					[msec]

	/* 速度制御 */
	float			f_acc1;			// �?速度1				[m/s2]
	float			f_acc3;			// �?速度3				[m/s2]
	float			f_now;			// 現在速度				[m/s]
	float			f_trgt;			// �?速後�?�目標速度		[m/s]
	float			f_last;			// 減速後�?�最終速度		[m/s]

	/* 距離制御 */
	float			f_dist;			// 移動距離				[m]
	float			f_l1;			// 第1移動距離			[m]
	float			f_l1_2;			// 第1+2移動距離		[m]

	/* 角速度制御 */
	float			f_accAngleS1;	// 角加速度1			[rad/s2]
	float			f_accAngleS3;	// 角加速度3			[rad/s2]
	float			f_nowAngleS;	// 現在角速度			[rad/s]
	float			f_trgtAngleS;	// �?速後�?�目標角速度	[rad/s]
	float			f_lastAngleS;	// 減速後�?�最終角速度	[rad/s]

	/* 角度制御 */
	float			f_angle;		// 移動角度				[rad]
	float			f_angle1;		// 第1移動角度			[rad]
	float			f_angle1_2;		// 第1+2移動角度		[rad]
}stMOT_DATA;

stMOT_DATA 		st_Info;				// シーケンス�?ータ
/* 動�? */
float 			f_MotNowSpeed 		= 0.0f;		// 現在速度
float 			f_MotTrgtSpeed 		= 0.0f;		// 目標速度
stMOT_DATA 		st_Info;				// シーケンス�?ータ
float			f_MotSuraStaSpeed	= 0.0f;
enMOT_WALL_EDGE_TYPE	en_WallEdge = MOT_WALL_EDGE_NONE;	// 壁�??れ補正
bool			bl_IsWallEdge = FALSE;				// 壁�??れ検知?�?TRUE:検知、FALSE?��非検知?�?
float			f_WallEdgeAddDist = 0;				// 壁�??れ補正の移動距離



float MOT_getAcc1( void )
{
	return PARAM_getSpeed( PARAM_ST )->f_acc;
}

float MOT_getAcc3( void )
{
	return PARAM_getSpeed( PARAM_ST )->f_dec;
}

void MOT_goBlock_AccConstDec( float f_fin, enMOT_ST_TYPE en_type, enMOT_GO_ST_TYPE en_goType )
{

	stCTRL_DATA		st_data;					// CTRLdata
	GYRO_staErrChkAngle();

	/* ================ */
	/*      motion      */
	/* ================ */
	/* ------ */
	/*  acc   */
	/* ------ */
	if( ( en_type != MOT_CONST_DEC ) && ( en_type != MOT_CONST_DEC_CUSTOM ) ){

		if( MOT_GO_ST_NORMAL == en_goType ){
			st_data.en_type		= CTRL_ACC;
		}
		else{
			st_data.en_type		= CTRL_SKEW_ACC;
		}
		st_data.f_acc			= st_Info.f_acc1;		// �?速度�?�?
		st_data.f_now			= st_Info.f_now;		// 現在速度
		st_data.f_trgt			= st_Info.f_trgt;		// 目標速度
		st_data.f_nowDist		= 0;				// 進んで�?な�?
		st_data.f_dist			= st_Info.f_l1;			// �?速距離
		st_data.f_accAngleS		= 0;				// 角加速度
		st_data.f_nowAngleS		= 0;				// 現在角速度
		st_data.f_trgtAngleS		= 0;				// 目標角度
		st_data.f_nowAngle		= 0;				// 現在角度
		st_data.f_angle			= 0;				// 目標角度
		st_data.f_time 			= 0;				// 目標時�? [sec] �? �?定しな�?
		CTRL_clrData();								// 設定データをクリア
		CTRL_setData( &st_data );						// �?ータセ�?�?
		DCM_staMotAll();							// モータON
		while( Get_NowDist() < st_Info.f_l1 ){					// �?定距離到達�?ち
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		
				DCM_brakeMot( DCM_L );		
				break;
			}				
			MOT_setWallEdgeDist();
		}

	}

	/* ------ */
	/*  const */
	/* ------ */
	if( MOT_GO_ST_NORMAL == en_goType ){
		st_data.en_type		= CTRL_CONST;
	}
	else{
		st_data.en_type		= CTRL_SKEW_CONST;
	}
	st_data.f_acc			= 0;					// �?速度�?�?
	st_data.f_now			= st_Info.f_trgt;			// 現在速度
	st_data.f_trgt			= st_Info.f_trgt;			// 目標速度
	st_data.f_nowDist		= st_Info.f_l1;				// 現在位置
	st_data.f_dist			= st_Info.f_l1_2;			// 等速完�?位置
	st_data.f_accAngleS		= 0;					// 角加速度
	st_data.f_nowAngleS		= 0;					// 現在角速度
	st_data.f_trgtAngleS		= 0;					// 目標角度
	st_data.f_nowAngle		= 0;					// 現在角度
	st_data.f_angle			= 0;					// 目標角度
	st_data.f_time 			= 0;					// 目標時�? [sec] �? �?定しな�?
	if( ( en_type == MOT_CONST_DEC ) || ( en_type == MOT_CONST_DEC_CUSTOM ) ){
		CTRL_clrData();										// 設定データをクリア
	}
	CTRL_setData( &st_data );						// �?ータセ�?�?
	while( Get_NowDist() < st_Info.f_l1_2 ){				// �?定距離到達�?ち
		if( SYS_isOutOfCtrl() == TRUE ){
			CTRL_stop();
			DCM_brakeMot( DCM_R );		
			DCM_brakeMot( DCM_L );		
			break;
		}				
		MOT_setWallEdgeDist();
	}

	/* ------ */
	/*  dec   */
	/* ------ */
	if( ( en_type != MOT_ACC_CONST ) && ( en_type != MOT_ACC_CONST_CUSTOM ) ){

		if( MOT_GO_ST_NORMAL == en_goType ){
			st_data.en_type		= CTRL_DEC;
		}
		else{
			st_data.en_type		= CTRL_SKEW_DEC;
		}
		st_data.f_acc			= st_Info.f_acc3;			// 減�?
		st_data.f_now			= st_Info.f_trgt;			// 現在速度
		st_data.f_trgt			= st_Info.f_last;			// 最終速度
		st_data.f_nowDist		= st_Info.f_l1_2;			// 等速完�?位置
		st_data.f_dist			= st_Info.f_dist;			// 全移動完�?位置
		st_data.f_accAngleS		= 0;						// 角加速度
		st_data.f_nowAngleS		= 0;						// 現在角速度
		st_data.f_trgtAngleS		= 0;						// 目標角度
		st_data.f_nowAngle		= 0;						// 現在角度
		st_data.f_angle			= 0;						// 目標角度
		st_data.f_time 			= 0;						// 目標時�? [sec] �? �?定しな�?
		CTRL_setData( &st_data );							// �?ータセ�?�?
		while( Get_NowDist() < ( st_Info.f_dist ) ){		// �?定距離到達�?ち
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		
				DCM_brakeMot( DCM_L );		
				break;
			}				
			MOT_setWallEdgeDist();
			if((escape_wait>2.0)&&(search_flag == TRUE))break;
		}

	}

	/* -------------------- */
	/*  const walledge      */
	/* -------------------- */
	/* not found edge */
	if( ( en_WallEdge != MOT_WALL_EDGE_NONE ) && ( bl_IsWallEdge == FALSE )  ){

		st_data.en_type			= CTRL_CONST;
		st_data.f_acc			= 0;						// �?速度�?�?
		st_data.f_now			= st_Info.f_last;			// 現在速度
		st_data.f_trgt			= st_Info.f_last;			// 目標速度
		st_data.f_nowDist		= Get_NowDist();				// 現在位置
		st_data.f_dist			= Get_NowDist() + 45.0f;		// 等速完�?位置?�?45.0f?��壁�??れをどこまで救うか�?�距離?��、ここではf_NowDistをクリアしては�?けな�?�?
		st_data.f_accAngleS		= 0;						// 角加速度
		st_data.f_nowAngleS		= 0;						// 現在角速度
		st_data.f_trgtAngleS	= 0;						// 目標角度
		st_data.f_nowAngle		= 0;						// 現在角度
		st_data.f_angle			= 0;						// 目標角度
		st_data.f_time 			= 0;						// 目標時�? [sec] �? �?定しな�?
		CTRL_clrData();										// マウスの現在位置/角度をクリア
		CTRL_setData( &st_data );							// �?ータセ�?�?
		while( Get_NowDist() < st_data.f_dist ){				// �?定距離到達�?ち
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		
				DCM_brakeMot( DCM_L );		
				break;
			}				
			if( MOT_setWallEdgeDist_LoopWait() == TRUE ) break;	// 壁�??れ補正を実行する距離を設�?
		}
	}
	/* straight for edge */
	if( ( MOT_GO_ST_NORMAL == en_goType ) &&				// 直進時に追�?動作が�?要な場合にしか実施しな�?
		( f_WallEdgeAddDist != 0.0f ) &&
		( f_fin != 0.0f )
	){
		st_data.en_type			= CTRL_CONST;
		st_data.f_acc			= 0;						// �?速度�?�?
		st_data.f_now			= st_Info.f_last;			// 現在速度
		st_data.f_trgt			= st_Info.f_last;			// 目標速度
		st_data.f_nowDist		= 0;						// 現在位置
		st_data.f_dist			= f_WallEdgeAddDist;		// 等速完�?位置
		st_data.f_accAngleS		= 0;						// 角加速度
		st_data.f_nowAngleS		= 0;						// 現在角速度
		st_data.f_trgtAngleS	= 0;						// 目標角度
		st_data.f_nowAngle		= 0;						// 現在角度
		st_data.f_angle			= 0;						// 目標角度
		st_data.f_time 			= 0;						// 目標時�? [sec] �? �?定しな�?
		CTRL_clrData();										// マウスの現在位置/角度をクリア
		CTRL_setData( &st_data );							// �?ータセ�?�?
		while( Get_NowDist() < st_data.f_dist ){				// �?定距離到達�?ち
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		
				DCM_brakeMot( DCM_L );		
				break;
			}				
		}
	}

	/* stop */
	if( 0.0f == f_fin ){
		LL_mDelay(100);			
	 	CTRL_stop();				
		DCM_brakeMot( DCM_R );	
		DCM_brakeMot( DCM_L );	
	}

	f_MotNowSpeed = f_fin;		
	GYRO_endErrChkAngle();
	CTRL_clrNowData();
}

void MOT_setData_ACC_CONST_DEC( float f_num, float f_fin, enMOT_GO_ST_TYPE en_type )
{
	float			f_l3;						// 第3移動距離[m]
	float			f_1blockDist;				// 1区画の距離[m]

	/* 1区画の距離 */
	if( MOT_GO_ST_NORMAL == en_type ){		// 通常の直進
		f_1blockDist = BLOCK;
	}
	else{									// 斜めの直進
		f_1blockDist = BLOCK_SKEW;
	}

	/* �?速度 */
	st_Info.f_acc1 		= MOT_getAcc1();								// �?速度1[m/s^2]
	st_Info.f_acc3 		= MOT_getAcc3();								// �?速度3[m/s^2]

	/* 速度 */
	st_Info.f_now		= f_MotNowSpeed;								// 現在速度
	st_Info.f_trgt		= f_MotTrgtSpeed;								// 目標速度
	st_Info.f_last		= f_fin;									// 最終速度

	/* 距離 */
	st_Info.f_dist		= f_num * f_1blockDist;												// 移動距離[m]
	st_Info.f_l1		= ( f_MotTrgtSpeed * f_MotTrgtSpeed - f_MotNowSpeed * f_MotNowSpeed ) / ( st_Info.f_acc1 * 2 );			// 第1移動距離[m]
	f_l3			= ( f_fin * f_fin - f_MotTrgtSpeed * f_MotTrgtSpeed ) / ( ( st_Info.f_acc3 * -1 ) * 2 );			// 第3移動距離[m]
	st_Info.f_l1_2		= st_Info.f_dist - f_l3;											// 第1+2移動距離[m]

//	printf("1 %f,%f\r",st_Info.f_trgt,st_Info.f_l1);
}

void MOT_setData_MOT_ACC_CONST_DEC_CUSTOM( float f_num, float f_fin, enMOT_GO_ST_TYPE en_type )
{
	float			f_l3;						// 第3移動距離[m]
	float			f_1blockDist;				// 1区画の距離[m]

	/* 1区画の距離 */
	if( MOT_GO_ST_NORMAL == en_type ){		// 通常の直進
		f_1blockDist = BLOCK;
	}
	else{									// 斜めの直進
		f_1blockDist = BLOCK_SKEW;
	}

	/* �?速度 */
	st_Info.f_acc1 		= MOT_getAcc1();								// �?速度1[m/s^2]
	st_Info.f_acc3 		= MOT_getAcc3();								// �?速度3[m/s^2]


	/* 距離 */
	st_Info.f_dist		= f_num * f_1blockDist;												// 移動距離[m]

	/* 速度 */
	st_Info.f_now		= f_MotNowSpeed;												// 現在速度
	st_Info.f_last		= f_fin;													// 最終速度
	st_Info.f_trgt		= sqrt( 1 / ( ( st_Info.f_acc3 * -1 ) - st_Info.f_acc1 ) *
					( 2 * st_Info.f_acc1 * ( st_Info.f_acc3 * -1 ) * ( st_Info.f_dist - MOT_MOVE_ST_MIN ) +
					( st_Info.f_acc3 * -1 ) * f_MotNowSpeed * f_MotNowSpeed - st_Info.f_acc1 * f_fin * f_fin ) );

	st_Info.f_l1		= ( st_Info.f_trgt * st_Info.f_trgt - f_MotNowSpeed * f_MotNowSpeed ) / ( st_Info.f_acc1 * 2 );			// 第1移動距離[m]
	f_l3			= ( f_fin * f_fin - st_Info.f_trgt * st_Info.f_trgt ) / ( ( st_Info.f_acc3  * -1 ) * 2 );			// 第3移動距離[m]
	st_Info.f_l1_2		= st_Info.f_dist - f_l3;											// 第1+2移動距離[m]

//	printf("2 %f,%f,%f,%f\r",st_Info.f_trgt,st_Info.f_l1,f_fin,f_MotNowSpeed);
}

void MOT_setData_MOT_ACC_CONST( float f_num, float f_fin, enMOT_GO_ST_TYPE en_type )
{
	float			f_1blockDist;				// 1区画の距離[m]

	/* 1区画の距離 */
	if( MOT_GO_ST_NORMAL == en_type ){		// 通常の直進
		f_1blockDist = BLOCK;
	}
	else{									// 斜めの直進
		f_1blockDist = BLOCK_SKEW;
	}

	/* �?速度 */
	st_Info.f_acc1 		= MOT_getAcc1();													// �?速度1[m/s^2]
	st_Info.f_acc3 		= 0;																// �?速度3[m/s^2](未使用)

	/* 速度 */
	st_Info.f_now		= f_MotNowSpeed;													// 現在速度
	st_Info.f_trgt		= f_fin;															// 目標速度
	st_Info.f_last		= 0;																// 最終速度(未使用)

	/* 距離 */
	st_Info.f_dist		= f_num * f_1blockDist;												// 移動距離[m]
	st_Info.f_l1		= ( f_fin * f_fin - f_MotNowSpeed * f_MotNowSpeed ) / ( st_Info.f_acc1 * 2 );			// 第1移動距離[m]
	st_Info.f_l1_2		= st_Info.f_dist;													// 第1+2移動距離[m]
}

void MOT_setData_MOT_ACC_CONST_CUSTOM( float f_num, float f_fin, enMOT_GO_ST_TYPE en_type )
{
	float			f_1blockDist;				// 1区画の距離[m]

	/* 1区画の距離 */
	if( MOT_GO_ST_NORMAL == en_type ){		// 通常の直進
		f_1blockDist = BLOCK;
	}
	else{									// 斜めの直進
		f_1blockDist = BLOCK_SKEW;
	}

	/* 速度 */
	st_Info.f_now		= f_MotNowSpeed;													// 現在速度
	st_Info.f_trgt		= f_fin;															// 目標速度
	st_Info.f_last		= 0;																// 最終速度(未使用)

	/* 距離 */
	st_Info.f_dist		= f_num * f_1blockDist;												// 移動距離[m]

	/* �?速度 */
	st_Info.f_acc1 		= ( f_fin * f_fin - f_MotNowSpeed * f_MotNowSpeed ) / ( ( st_Info.f_dist - MOT_MOVE_ST_MIN ) * 2.0f );	// �?速度1[m/s^2]?��強制�?に書き換え�?
	st_Info.f_acc3 		= 0;																// �?速度3[m/s^2](未使用)

	/* 距離 */
	st_Info.f_l1		= ( f_fin * f_fin - f_MotNowSpeed * f_MotNowSpeed ) / ( st_Info.f_acc1 * 2 );			// 第1移動距離[m]
	st_Info.f_l1_2		= st_Info.f_dist;													// 第1+2移動距離[m]
}

void MOT_setData_MOT_CONST_DEC( float f_num, float f_fin, enMOT_GO_ST_TYPE en_type )
{
	float			f_1blockDist;				// 1区画の距離[mm]

	/* 1区画の距離 */
	if( MOT_GO_ST_NORMAL == en_type ){		// 通常の直進
		f_1blockDist = BLOCK;
	}
	else{									// 斜めの直進
		f_1blockDist = BLOCK_SKEW;
	}

	/* �?速度 */
	st_Info.f_acc1 		= 0;																// �?速度1[m/s^2](未使用)
	st_Info.f_acc3 		= MOT_getAcc3();													// �?速度3[m/s^2]

	/* 速度 */
	st_Info.f_now		= f_MotNowSpeed;													// 現在速度
	st_Info.f_trgt		= f_MotNowSpeed;													// 目標速度
	st_Info.f_last		= f_fin;															// 最終速度(未使用)

	/* 距離 */
	st_Info.f_dist		= f_num * f_1blockDist;												// 移動距離[m]
	st_Info.f_l1		= 0;																// 第1移動距離[m]
	st_Info.f_l1_2		= st_Info.f_dist - ( f_fin * f_fin - f_MotNowSpeed * f_MotNowSpeed ) / ( ( st_Info.f_acc3 * -1 ) * 2 );			// 第1-2移動距離[m]
}

void MOT_setData_MOT_CONST_DEC_CUSTOM( float f_num, float f_fin, enMOT_GO_ST_TYPE en_type )
{
	float			f_1blockDist;				// 1区画の距離[m]

	/* 1区画の距離 */
	if( MOT_GO_ST_NORMAL == en_type ){		// 通常の直進
		f_1blockDist = BLOCK;
	}
	else{									// 斜めの直進
		f_1blockDist = BLOCK_SKEW;
	}

	/* 速度 */
	st_Info.f_now		= f_MotNowSpeed;									// 現在速度
	st_Info.f_trgt		= f_MotNowSpeed;									// 目標速度
	st_Info.f_last		= f_fin;															// 最終速度

	/* 距離 */
	st_Info.f_dist		= f_num * f_1blockDist;									// 移動距離[mm]

	/* �?速度 */
	st_Info.f_acc1 		= 0;																// �?速度1[m/s^2](未使用)
	st_Info.f_acc3 		= ( f_fin * f_fin - f_MotNowSpeed * f_MotNowSpeed ) / ( ( st_Info.f_dist - MOT_MOVE_ST_MIN ) * 2.0f ) * -1;	// �?速度3[m/s^2]?��強制�?に書き換え�?

	/* 距離 */
	st_Info.f_l1		= 0;																// 第1移動距離[m]
	st_Info.f_l1_2		= st_Info.f_dist - ( f_fin * f_fin - f_MotNowSpeed * f_MotNowSpeed ) / ( ( st_Info.f_acc3 * -1 ) * 2 );			// 第1-2移動距離[m]
}

enMOT_ST_TYPE MOT_getStType( float f_num, float f_fin, enMOT_GO_ST_TYPE en_type )
{
	float f_v1Div;
	float f_v3Div;
	float f_acc1;
	float f_acc3;
	float f_t1;
	float f_t3;
	float f_l1;							//�?速距離
	float f_l3;							//減速距離
	float f_total;							// 移動距離[m]

	/* 1区画の距離 */
	if( MOT_GO_ST_NORMAL == en_type ){		// 通常の直進
		f_total	= f_num * BLOCK;
	}
	else{									// 斜めの直進
		f_total	= f_num * BLOCK_SKEW;
	}


	/* ================ */
	/*  �?速＋等速動�?  */
	/* ================ */
	f_v1Div		= f_fin - f_MotNowSpeed;
	f_acc1		= MOT_getAcc1();				// �?速度1[m/s^2]
	f_t1		= f_v1Div / f_acc1;

	f_l1 = ( f_MotNowSpeed + f_fin ) * 0.5f * f_t1;

	/* �?速＋等速動�? */
	if( f_total <= ( f_l1 + MOT_MOVE_ST_THRESHOLD ) ){

		/* �?速が最終速度に対して完�?しな�? */
		if( f_total < ( f_l1 + MOT_MOVE_ST_MIN ) ){
//			printf("パターン4\n\r");
			return MOT_ACC_CONST_CUSTOM;		// パターン4?��強制�?に�?速度を変更する?�?
		}
		else{
//			printf("パターン3\n\r");
			return MOT_ACC_CONST;				// パターン3?��加速＋等速�?
		}
	}

	/* ================ */
	/*  等速＋減速動�?  */
	/* ================ */
	f_v3Div		= f_fin - f_MotNowSpeed;
	f_acc3		= MOT_getAcc3();				// �?速度3[m/s^2]
	f_t3		= f_v3Div / ( f_acc3 * -1 );

	f_l3 = ( f_MotNowSpeed + f_fin ) * 0.5f * f_t3;

	/* 等速＋減速動�? */
	if( f_total <= ( f_l3 + MOT_MOVE_ST_THRESHOLD ) ){

		/* 減速が最終速度に対して完�?しな�? */
		if( f_total < ( f_l3 + MOT_MOVE_ST_MIN ) ){
//			printf("パターン6\n\r");
			return MOT_CONST_DEC_CUSTOM;		// パターン6?��強制�?に�?速度を変更する?�?
		}
		else{
//			printf("パターン5\n\r");
			return MOT_CONST_DEC;				// パターン5?��等速＋減速�?
		}
	}

	/* ========== */
	/*  台形動�?  */
	/* ========== */
	f_v1Div		= f_MotTrgtSpeed - f_MotNowSpeed;					// 台形時�?�速度差
	f_t1		= f_v1Div / f_acc1;
	f_l1		= ( f_MotNowSpeed + f_MotTrgtSpeed ) * 0.5f * f_t1;

	f_v3Div		= f_fin - f_MotTrgtSpeed;							// 台形時�?�速度差
	f_acc3		= MOT_getAcc3();									// �?速度3[mm/s^2]
	f_t3		= -1.0f * f_v3Div / f_acc3;							// 減速時の所要時�?
	f_l3		= ( f_MotTrgtSpeed + f_fin ) * 0.5f * f_t3;

	/* 通常の台形動�? */
	if( ( f_total - f_l1 - f_l3 - MOT_MOVE_ST_MIN) >= 0 ){
//		printf("パターン1\n\r");
		return MOT_ACC_CONST_DEC;				// パターン1?��通常?�?
	}
	/* 等速値を変更する */
	else{
//		printf("パターン2\n\r");
		return MOT_ACC_CONST_DEC_CUSTOM;		// パターン2?��目標速度を変更?�?
	}
}

void MOT_go_FinSpeed( float f_num, float f_fin, enMOT_GO_ST_TYPE en_goStType )
{
	enMOT_ST_TYPE 		en_type 		= MOT_getStType( f_num, f_fin, en_goStType);			// 動作パターン取�?

	/* 移動距離と�?定値に応じで動作を変え�? */
	switch( en_type ){

		case MOT_ACC_CONST_DEC:				// [01] 台形�?�?
			MOT_setData_ACC_CONST_DEC( f_num, f_fin, en_goStType );					// 動作データ作�??
			MOT_goBlock_AccConstDec( f_fin, en_type, en_goStType );					// 動�?
			break;

		case MOT_ACC_CONST_DEC_CUSTOM:		// [02] 台形�?速（等速�?
			MOT_setData_MOT_ACC_CONST_DEC_CUSTOM( f_num, f_fin, en_goStType );		// 動作データ作�??
			MOT_goBlock_AccConstDec( f_fin, en_type, en_goStType );					// 動�?
			break;

		case MOT_ACC_CONST:				// [03] �?速＋等�?
			MOT_setData_MOT_ACC_CONST( f_num, f_fin, en_goStType );					// 動作データ作�??
			MOT_goBlock_AccConstDec( f_fin, en_type, en_goStType );					// 動�?
			break;

		case MOT_ACC_CONST_CUSTOM:		// [04] �?速＋等速（等速�?
			MOT_setData_MOT_ACC_CONST_CUSTOM( f_num, f_fin, en_goStType );			// 動作データ作�??
			MOT_goBlock_AccConstDec( f_fin, en_type, MOT_GO_ST_NORMAL );			// 動�?
			break;

		case MOT_CONST_DEC:				// [05] 等速＋減�?
			MOT_setData_MOT_CONST_DEC( f_num, f_fin, en_goStType );					// 動作データ作�??
			MOT_goBlock_AccConstDec( f_fin, en_type, en_goStType );					// 動�?
			break;

		case MOT_CONST_DEC_CUSTOM:		// [06] 等速＋減速（減速値変更?�?
			MOT_setData_MOT_CONST_DEC_CUSTOM( f_num, f_fin, en_goStType );			// 動作データ作�??
			MOT_goBlock_AccConstDec( f_fin, en_type, en_goStType );					// 動�?
			break;

		default:
			break;
	}

}

void MOT_goBlock_FinSpeed( float f_num, float f_fin )
{
	MOT_go_FinSpeed( f_num, f_fin, MOT_GO_ST_NORMAL );		// 通常の直進
}

void MOT_goSkewBlock_FinSpeed( float f_num, float f_fin )
{
	MOT_go_FinSpeed( f_num, f_fin, MOT_GO_ST_SKEW );		// 通常の直進
}

void MOT_goBlock_Const(float f_num)
{
	stCTRL_DATA		st_data;
	stMOT_DATA		st_info;

	GYRO_staErrChkAngle();

	/* ---------------- */
	/*  動作データ計�?  */
	/* ---------------- */
	/* 距離 */
	st_info.f_dist		= f_num * BLOCK;													// 移動距離[m]


	/* ------ */
	/*  等�?  */
	/* ------ */
	st_data.en_type			= CTRL_CONST;
	st_data.f_acc			= 0;					// �?速度�?�?
	st_data.f_now			= f_MotNowSpeed;			// 現在速度
	st_data.f_trgt			= f_MotNowSpeed;			// 目標速度
	st_data.f_nowDist		= 0;				// 現在位置
	st_data.f_dist			= st_info.f_dist;			// 等速完�?位置
	st_data.f_accAngleS		= 0;					// 角加速度
	st_data.f_nowAngleS		= 0;					// 現在角速度
	st_data.f_trgtAngleS		= 0;					// 目標角度
	st_data.f_nowAngle		= 0;					// 現在角度
	st_data.f_angle			= 0;					// 目標角度
	st_data.f_time 			= 0;					// 目標時�? [sec] �? �?定しな�?
	CTRL_clrData();										// 設定データをクリア
	CTRL_setData( &st_data );						// �?ータセ�?�?
	Set_TrgtSpeed(f_MotNowSpeed);
//	printf("目標速度 %f 目標位置 %f \r\n",st_data.f_trgt,st_data.f_dist);
	while( Get_NowDist() < st_info.f_dist ){				// �?定距離到達�?ち
		if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// ブレーキ
				DCM_brakeMot( DCM_L );		// ブレーキ
				break;
			}				// 途中で制御不�?�になっ�?
	}

	GYRO_endErrChkAngle();
	CTRL_clrNowData();
}

void testrun(void)
{
	stCTRL_DATA test;
		test.en_type=CTRL_ACC;
		test.f_acc=MOT_getAcc1();
		test.f_trgt	= 0;
		test.f_now = 0;
		test.f_nowDist = 0;
		test.f_dist = 0;

	CTRL_clrNowData();
	CTRL_clrData();
	CTRL_setData(&test);
}

float MOT_getAccAngle1( void )
{
//	return ( 1800 );
	return PARAM_getSpeed( PARAM_TRUN )->f_accAngle;
}

float MOT_getAccAngle3( void )
{
//	return ( 1800 );
	return PARAM_getSpeed( PARAM_TRUN )->f_decAngle;
}

void MOT_turn( enMOT_TURN_CMD en_type )
{
	stMOT_DATA	st_info;	//シーケンス�?ータ
	stCTRL_DATA	st_data;	//制御�?ータ
//	float		f_angle2 = A2_MIN;	//最低第2移動角度[rad]
	float		f_angle1;	//第1移動角度[rad]
	float		f_angle3;	//第3移動角度[rad]
	float		us_trgtAngleS;	//目標角度[rad/s]

	us_trgtAngleS = 2.8*PI;//500;
	/* ---------------- */
	/*  動作データ計�?  */
	/* ---------------- */
	/* �?速度 */
	st_info.f_accAngleS1= MOT_getAccAngle1();												// 角加速度1[rad/s^2]
	st_info.f_accAngleS3= MOT_getAccAngle3();												// 角加速度3[rad/s^2]

	/* 角速度 */
	st_info.f_nowAngleS	= 0;																// 現在角速度
	st_info.f_trgtAngleS= (float)us_trgtAngleS;												// 目標角速度
	st_info.f_lastAngleS= 0;																// 最終角速度

	/* 角度 */
	switch( en_type ){
		case MOT_R90:	st_info.f_angle =  -PI/2 - ANGLE_OFFSET1_R;	break;					// 回転角度[rad]
		case MOT_L90:	st_info.f_angle =   PI/2 + ANGLE_OFFSET1;		break;					// 回転角度[rad]
		case MOT_R180:	st_info.f_angle = -PI - ANGLE_OFFSET2_R;	break;					// 回転角度[rad]
		case MOT_L180:	st_info.f_angle =  PI + ANGLE_OFFSET2;		break;					// 回転角度[rad]
		case MOT_R360:	st_info.f_angle = -2*PI - ANGLE_OFFSET3;		break;					// 回転角度[rad]
		case MOT_L360:	st_info.f_angle =  2*PI + ANGLE_OFFSET3;		break;					// 回転角度[rad]
		default:
			printf("error\r\n");
			break;
	}
	f_angle3 = ( st_info.f_trgtAngleS - st_info.f_lastAngleS ) / 2 * ( st_info.f_trgtAngleS - st_info.f_lastAngleS ) / st_info.f_accAngleS3;						// 第3移動角度[rad]
	f_angle1 = ( 0 - st_info.f_trgtAngleS) / 2 * ( 0 - st_info.f_trgtAngleS ) / st_info.f_accAngleS1;


	if( ( en_type == MOT_R90 ) || ( en_type == MOT_R180 ) || ( en_type == MOT_R360 ) ){		// -方�?
		st_info.f_trgtAngleS*= -1;															// 回転方向を�?にする
		f_angle1			*= -1;
//		f_angle2 			*= -1;															// 回転方向を�?にする
		f_angle3 			*= -1;															// 回転方向を�?にする
		st_info.f_angle1	= f_angle1;						// 第1移動角度[rad]
		st_info.f_angle1_2	= st_info.f_angle - f_angle3;									// 第1+2移動角度[rad]
		en_Turntype			= Right;

		/* 最小移動距離を上書�? */
		if( st_info.f_angle1 > ( A1_MIN * -1 ) ){
			st_info.f_angle1 = A1_MIN * -1;
		}
	}
	else{
		st_info.f_angle1	= f_angle1;						// 第1移動角度[rad]
		st_info.f_angle1_2	= st_info.f_angle - f_angle3;									// 第1+2移動角度[rad]
		en_Turntype			= Left;

		/* 最小移動距離を上書�? */
		if( st_info.f_angle1 < A1_MIN ){
			st_info.f_angle1 = A1_MIN;
		}
	}


	GYRO_staErrChkAngle();			// エラー検�?�開�?
//	printf("目標角度 %f %f %f\r\n",st_info.f_angle,st_info.f_angle1,st_info.f_angle1_2);
	/* ================ */
	/*      実動�?      */
	/* ================ */
	/* ------ */
	/*  �?�?  */
	/* ------ */
	st_data.en_type			= CTRL_ACC_TRUN;
	st_data.f_acc			= 0;						// �?速度�?�?
	st_data.f_now			= 0;						// 現在速度
	st_data.f_trgt			= 0;						// 目標速度
	st_data.f_nowDist		= 0;						// 進んで�?な�?
	st_data.f_dist			= 0;						// �?速距離
	st_data.f_accAngleS		= st_info.f_accAngleS1;		// 角加速度
	st_data.f_nowAngleS		= 0;						// 現在角速度
	st_data.f_trgtAngleS		= st_info.f_trgtAngleS;		// 目標角度
	st_data.f_nowAngle		= 0;						// 現在角度
	st_data.f_angle			= st_info.f_angle1;			// 目標角度
	st_data.f_time 			= 0;						// 目標時�? [sec] �? �?定しな�?
	CTRL_clrData();										// マウスの現在位置/角度をクリア
	CTRL_setData( &st_data );							// �?ータセ�?�?
	DCM_staMotAll();									// モータON

	if( ( en_type == MOT_R90 ) || ( en_type == MOT_R180 ) || ( en_type == MOT_R360 ) ){		// -方�?
		while( Get_NowAngle() > st_info.f_angle1 ){			// �?定角度到達�?ち
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// ブレーキ
				DCM_brakeMot( DCM_L );		// ブレーキ
				break;
			}				// 途中で制御不�?�になっ�?
		}
	}
	else{
		while( Get_NowAngle() < st_info.f_angle1 ){			// �?定角度到達�?ち
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// ブレーキ
				DCM_brakeMot( DCM_L );		// ブレーキ
				break;
			}				// 途中で制御不�?�になっ�?
		}
	}
//	printf("finish\n");

	/* ------ */
	/*  等�?  */
	/* ------ */
	if( ( en_type == MOT_R90 ) || ( en_type == MOT_R180 ) || ( en_type == MOT_R360 ) ){		// -方�?
		f_angle3			= ( Get_TrgtAngleS() - st_info.f_lastAngleS ) / 2 * ( Get_TrgtAngleS() - st_info.f_lastAngleS ) / st_info.f_accAngleS3;		// 第3移動角度[rad]
		f_angle3			= -1 * f_angle3;
		if( f_angle3 > A3_MIN*-1 ) f_angle3 = A3_MIN * -1;																	// 減速最低角度に書き換�?
		st_info.f_angle1_2		= st_info.f_angle - f_angle3;// 第1+2移動角度[rad]

	}
	else{
		f_angle3			= ( Get_TrgtAngleS() - st_info.f_lastAngleS ) / 2 * ( Get_TrgtAngleS() - st_info.f_lastAngleS ) / st_info.f_accAngleS3;		// 第3移動角度[rad]
		if( f_angle3 < A3_MIN ) f_angle3 = A3_MIN;																			// 減速最低角度に書き換�?
		st_info.f_angle1_2		= st_info.f_angle - f_angle3;																// 第1+2移動角度[rad]
//		printf("   [f_angle3]%d [f_angle1_2]%d\n\r", (int32_t)f_angle3, (int32_t)	st_info.f_angle1_2 );
	}
//	printf("[f_TrgtAngleS] %5.2f,st_info.f_angle1_2%5.2f,f_angle2%5.2f\n\r",f_TrgtAngleS,st_info.f_angle1_2,f_angle3);
	st_data.en_type			= CTRL_CONST_TRUN;
	st_data.f_acc			= 0;						// �?速度�?�?
	st_data.f_now			= 0;						// 現在速度
	st_data.f_trgt			= 0;						// 目標速度
	st_data.f_nowDist		= 0;						// 進んで�?な�?
	st_data.f_dist			= 0;						// 等速完�?位置
	st_data.f_accAngleS		= 0;						// 角加速度
	st_data.f_nowAngleS		= Get_TrgtAngleS();				// 現在角速度
	st_data.f_trgtAngleS		= Get_TrgtAngleS();				// 目標角度
	st_data.f_nowAngle		= st_info.f_angle1;			// 現在角度
	st_data.f_angle			= st_info.f_angle1_2;			// 目標角度
	st_data.f_time 			= 0;						// 目標時�? [sec] �? �?定しな�?
	CTRL_setData( &st_data );							// �?ータセ�?�?
	if( ( en_type == MOT_R90 ) || ( en_type == MOT_R180 ) || ( en_type == MOT_R360 ) ){		// -方�?
		while( Get_NowAngle() > st_info.f_angle1_2 ){			// �?定距離到達�?ち
//			DCMC_getAngleSpeedFB(&f_err);
//			printf("[NOW]%d [Trgt]%d [TrgtS]%d \n\r", (int32_t)f_NowAngle, (int32_t)f_TrgtAngle, (int32_t)f_TrgtAngleS);
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// ブレーキ
				DCM_brakeMot( DCM_L );		// ブレーキ
				break;
			}				// 途中で制御不�?�になっ�?
//			log_in(f_TrgtAngle);
		}
	}
	else{
		while( Get_NowAngle() < st_info.f_angle1_2 ){			// �?定距離到達�?ち
//			DCMC_getAngleSpeedFB(&f_err);
//			printf("[NOW]%d [Trgt]%d [TrgtS]%d  \n\r", (int32_t)f_NowAngle, (int32_t)f_TrgtAngle, (int32_t)f_TrgtAngleS);
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// ブレーキ
				DCM_brakeMot( DCM_L );		// ブレーキ
				break;
			}				// 途中で制御不�?�になっ�?
//			log_in(f_TrgtAngleS);
		}
	}
//	printf("finish2\n");

	/* ------ */
	/*  減�?  */
	/* ------ */
	st_data.en_type			= CTRL_DEC_TRUN;
	st_data.f_acc			= 0;						// 減�?
	st_data.f_now			= 0;						// 現在速度
	st_data.f_trgt			= 0;						// 最終速度
	st_data.f_nowDist		= 0;						// 等速完�?位置
	st_data.f_dist			= 0;						// 全移動完�?位置
	st_data.f_accAngleS		= st_info.f_accAngleS3;		// 角加速度
	st_data.f_nowAngleS		= Get_TrgtAngleS();				// 現在角速度
	st_data.f_trgtAngleS		= 0;						// 目標角度
	st_data.f_nowAngle		= st_info.f_angle1_2;		// 現在角度
	st_data.f_angle			= st_info.f_angle;			// 目標角度
	st_data.f_time 			= 0;						// 目標時�? [sec] �? �?定しな�?
	CTRL_setData( &st_data );							// �?ータセ�?�?
	if( ( en_type == MOT_R90 ) || ( en_type == MOT_R180 ) || ( en_type == MOT_R360 ) ){		// -方�?
		while( Get_NowAngle() > ( st_info.f_angle) ){		// �?定距離到達�?ち
//			DCMC_getAngleSpeedFB(&f_err);
//			printf("[NOW]%d [Trgt]%d [TrgtS]%d  \n\r", (int32_t)f_NowAngle, (int32_t)f_TrgtAngle, (int32_t)f_TrgtAngleS );
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// ブレーキ
				DCM_brakeMot( DCM_L );		// ブレーキ
				break;
			}				// 途中で制御不�?�になっ�?
			if((escape_wait>2.0)&&(search_flag == TRUE))break;
		}
	}
	else{
		while( Get_NowAngle() < ( st_info.f_angle ) ){		// �?定距離到達�?ち
//			DCMC_getAngleSpeedFB(&f_err);
//			printf("[NOW]%d [Trgt]%d [TrgtS]%d  \n\r", (int32_t)f_NowAngle, (int32_t)f_TrgtAngle, (int32_t)f_TrgtAngleS);
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// ブレーキ
				DCM_brakeMot( DCM_L );		// ブレーキ
				break;
			}				// 途中で制御不�?�になっ�?
			if((escape_wait>2.0)&&(search_flag == TRUE))break;
//			log_in(f_TrgtAngle);
		}
	}
//	printf("finish3\n");
	/* 停止 */
	LL_mDelay(200);				// 安定�?ち
	CTRL_stop();			// 制御停止
	DCM_brakeMot( DCM_R );		// ブレーキ
	DCM_brakeMot( DCM_L );		// ブレーキ
	GYRO_endErrChkAngle();					// エラー検�?�終�?
	CTRL_clrNowData();
}

void MOT_setSuraStaSpeed( float f_speed )
{
	f_MotSuraStaSpeed = f_speed;

}

float MOT_getSuraStaSpeed( void )
{
	return f_MotSuraStaSpeed;
}

float MOT_setTrgtSpeed(float f_speed)
{
	f_MotTrgtSpeed = f_speed;
	return f_MotTrgtSpeed;
}

void MOT_setNowSpeed(float f_speed)
{
	f_MotNowSpeed = f_speed;
}

void MOT_goHitBackWall(void)
{
	stMOT_DATA	st_info;	//シーケンス�?ータ
	stCTRL_DATA	st_data;	//制御�?ータ

	/* ---------------- */
	/*  動作データ計�?  */
	/* ---------------- */
	/* �?速度 */
	st_info.f_acc1= 1200;												// 角加速度1[rad/s^2]												// 角加速度3[rad/s^2]

	GYRO_staErrChkAngle();			// エラー検�?�開�?
//	printf("");
	/* ================ */
	/*      実動�?      */
	/* ================ */
	/* ------ */
	/*  �?�?  */
	/* ------ */
	st_data.en_type			= CTRL_HIT_WALL;
	st_data.f_acc			= st_info.f_acc1;						// �?速度�?�?
	st_data.f_now			= 0;						// 現在速度
	st_data.f_trgt			= 0;						// 目標速度
	st_data.f_nowDist		= 0;						// 進んで�?な�?
	st_data.f_dist			= 0;						// �?速距離
	st_data.f_accAngleS		= 0;		// 角加速度
	st_data.f_nowAngleS		= 0;						// 現在角速度
	st_data.f_trgtAngleS		= 0;		// 目標角度
	st_data.f_nowAngle		= 0;						// 現在角度
	st_data.f_angle			= 0;			// 目標角度
	st_data.f_time 			= 0;						// 目標時�? [sec] �? �?定しな�?
	CTRL_clrData();										// マウスの現在位置/角度をクリア
	CTRL_setData( &st_data );							// �?ータセ�?�?
	DCM_staMotAll();									// モータON
//	printf("目標速度 %f 目標位置 %f\r\n",st_data.f_trgt,st_data.f_dist);

	/*停止*/
	LL_mDelay(400);				// 安定�?ち
	CTRL_stop();			// 制御停止
	DCM_brakeMot( DCM_R );		// ブレーキ
	DCM_brakeMot( DCM_L );		// ブレーキ

	LL_mDelay(100);

	f_MotNowSpeed = 0.0f;		//現在速度更新

	GYRO_endErrChkAngle();					// エラー検�?�終�?
	CTRL_clrNowData();

}

void MOT_goSla( enMOT_SURA_CMD en_type, stSLA* p_sla )
{
	stMOT_DATA		st_info;					// シーケンス�?ータ
	stCTRL_DATA		st_data;					// 制御�?ータ
//	float			f_err;
	float			f_entryLen;
	float			f_escapeLen;

	/* ---------------- */
	/*  動作データ計�?  */
	/* ---------------- */
	/* �?速度 */
	st_info.f_acc1 		= 0;																// �?速度1[mm/s^2]
	st_info.f_acc3 		= 0;																// �?速度3[mm/s^2]

	/* 速度 */
	st_info.f_now		= p_sla->f_speed;													// 現在速度
	st_info.f_trgt		= p_sla->f_speed;													// 目標速度
	st_info.f_last		= p_sla->f_speed;													// 最終速度

	/* 距離 */
	st_info.f_dist		= 0;																// 移動距離
	st_info.f_l1		= 0;																// 第1移動距離[mm]
	st_info.f_l1_2		= 0;																// 第1+2移動距離[mm]

	/* 角加速度 */
	st_info.f_accAngleS1= p_sla->f_angAcc;													// 角加速度1[deg/s^2]
	st_info.f_accAngleS3= p_sla->f_angAcc;													// 角加速度3[deg/s^2]

	/* 角速度 */
	st_info.f_nowAngleS	= 0;																// 現在角速度[deg/s]
	st_info.f_trgtAngleS= p_sla->f_angvel;													// 目標角速度
	st_info.f_lastAngleS= 0;																// 最終角速度

	/* 角度 */
	st_info.f_angle		= p_sla->f_ang_Total;												// 旋回角度[deg]
	st_info.f_angle1	= p_sla->f_ang_AccEnd;												// 第1移動角度[deg]
	st_info.f_angle1_2	= p_sla->f_ang_ConstEnd;											// 第1+2移動角度[deg]

	/* 方向に応じて符号を変更 */
	if( ( en_type == MOT_R90S ) ||
		( en_type == MOT_R45S_S2N ) || ( en_type == MOT_R45S_N2S ) ||
		( en_type == MOT_R90S_N ) ||
		( en_type == MOT_R135S_S2N ) || ( en_type == MOT_R135S_N2S )
	){
		st_info.f_accAngleS1 *= -1;
		st_info.f_trgtAngleS *= -1;
		st_info.f_angle      *= -1;
		st_info.f_angle1     *= -1;
		st_info.f_angle1_2   *= -1;
	}
	else{
		st_info.f_accAngleS3 *= -1;
	}

	/* 斜め走行�?�タイプに応じて、スラロー�?前�?�距離とスラロー�?後�?�退避距離を�?�れ替える */
	if( ( en_type == MOT_R45S_N2S ) || ( en_type == MOT_L45S_N2S ) || ( en_type == MOT_R135S_N2S ) || ( en_type == MOT_L135S_N2S ) ){ 		// �?にするも�?�
		f_entryLen  = p_sla->f_escapeLen;
		f_escapeLen = p_sla->f_entryLen;
	}
	else{		// 通常
		f_entryLen  = p_sla->f_entryLen;
		f_escapeLen = p_sla->f_escapeLen;
	}

	GYRO_staErrChkAngle();			// エラー検�?�開�?

//	LED_on(LED1);
	/* ================ */
	/*      entry      */
	/* ================ */
	/* ------------------------ */
	/*  acc前�?�前進動�?  */
	/* ------------------------ */
	st_data.en_type			= CTRL_ENTRY_SURA;
	st_data.f_acc			= 0;						// �?速度�?�?
	st_data.f_now			= st_info.f_now;			// 現在速度
	st_data.f_trgt			= st_info.f_now;			// 目標速度
	st_data.f_nowDist		= 0;						// 進んで�?な�?
	st_data.f_dist			= f_entryLen;				// スラロー�?前�?�前進距離
	st_data.f_accAngleS		= 0;						// 角加速度
	st_data.f_nowAngleS		= 0;						// 現在角速度
	st_data.f_trgtAngleS	= 0;						// 目標角度
	st_data.f_nowAngle		= 0;						// 現在角度
	st_data.f_angle			= 0;						// 目標角度
	st_data.f_time 			= 0;						// 目標時�? [sec] �? �?定しな�?
	CTRL_clrData();										// マウスの現在位置/角度をクリア
	CTRL_setData( &st_data );							// �?ータセ�?�?
	DCM_staMotAll();									// モータON

	while( Get_NowDist() < f_entryLen + uc_dist_control ){				// �?定距離到達�?ち
		if((DIST_getNowVal( DIST_SEN_R_FRONT )>R_FRONT_CTRL)&&(DIST_getNowVal( DIST_SEN_L_FRONT )>L_FRONT_CTRL))break;
		if( SYS_isOutOfCtrl() == TRUE ){
			CTRL_stop();
			DCM_brakeMot( DCM_R );		// ブレーキ
			DCM_brakeMot( DCM_L );		// ブレーキ
			break;
		}				// 途中で制御不�?�になっ�?
	}
//	LED_off(LED1);
//	log_in(0);
	/* ------ */
	/*  acc  */
	/* ------ */
	st_data.en_type			= CTRL_ACC_SURA;
	st_data.f_acc			= 0;						// �?速度�?�?
	st_data.f_now			= st_info.f_now;			// 現在速度
	st_data.f_trgt			= st_info.f_now;			// 目標速度
	st_data.f_nowDist		= f_entryLen;				//
	st_data.f_dist			= f_entryLen + st_info.f_now * p_sla->us_accAngvelTime * 0.001;		// �?速距離
	st_data.f_accAngleS		= st_info.f_accAngleS1;		// 角加速度
	st_data.f_nowAngleS		= 0;						// 現在角速度
	st_data.f_trgtAngleS		= st_info.f_trgtAngleS;		// 目標角速度
	st_data.f_nowAngle		= 0;						// 現在角度
	st_data.f_angle			= st_info.f_angle1;			// 目標角度
	st_data.f_time 			= p_sla->us_accAngvelTime * 0.001;			// [msec] �? [sec]
	CTRL_setData( &st_data );							// �?ータセ�?�?
//	printf("trgtangleS %5.2f\n\r",st_data.f_trgtAngleS);
	if( IS_R_SLA( en_type ) == TRUE ) {		// -方�?
		while( ( Get_NowAngle() > st_info.f_angle1 ) && ( Get_NowDist() < st_data.f_dist ) ){			// �?定角度?��距離到達�?ち
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// ブレーキ
				DCM_brakeMot( DCM_L );		// ブレーキ
				break;
			}				// 途中で制御不�?�になっ�?
		}
	}
	else{
		while( ( Get_NowAngle() < st_info.f_angle1 ) && ( Get_NowDist() < st_data.f_dist ) ){			// �?定角度?��距離到達�?ち
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// ブレーキ
				DCM_brakeMot( DCM_L );		// ブレーキ
				break;
			}				// 途中で制御不�?�になっ�?
		}
	}

//	log_in(0);
//	log_in(f_NowAngle);
	/* ------ */
	/*  const  */
	/* ------ */
	st_data.en_type			= CTRL_CONST_SURA;
	st_data.f_acc			= 0;						// �?速度�?�?
	st_data.f_now			= st_info.f_now;			// 現在速度
	st_data.f_trgt			= st_info.f_now;			// 目標速度
	st_data.f_nowDist		= f_entryLen + st_info.f_now * p_sla->us_accAngvelTime * 0.001;
	st_data.f_dist			= f_entryLen + st_info.f_now * ( p_sla->us_constAngvelTime + p_sla->us_accAngvelTime ) * 0.001;		// 等速距離
	st_data.f_accAngleS		= 0;						// 角加速度
	st_data.f_nowAngleS		= st_info.f_trgtAngleS;		// 現在角速度
	st_data.f_trgtAngleS	= st_info.f_trgtAngleS;		// 目標角速度
	st_data.f_nowAngle		= st_info.f_angle1;			// 現在角度
	st_data.f_angle			= st_info.f_angle1_2;		// 目標角度
	st_data.f_time 			= p_sla->us_constAngvelTime * 0.001;		// [msec] �? [sec]
	CTRL_setData( &st_data );							// �?ータセ�?�?

	if( IS_R_SLA( en_type ) == TRUE ) {		// -方�?
		while( ( Get_NowAngle() > st_info.f_angle1_2 ) && ( Get_NowDist() < st_data.f_dist ) ){		// �?定角度?��距離到達�?ち
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// ブレーキ
				DCM_brakeMot( DCM_L );		// ブレーキ
				break;
			}				// 途中で制御不�?�になっ�?
		}
	}
	else{
		while( ( Get_NowAngle() < st_info.f_angle1_2 ) && ( Get_NowDist() < st_data.f_dist ) ){		// �?定角度?��距離到達�?ち
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// ブレーキ
				DCM_brakeMot( DCM_L );		// ブレーキ
				break;
			}				// 途中で制御不�?�になっ�?
		}
	}
//	log_in(0);
//	log_in(f_NowAngle);
	/* ------ */
	/*  dec  */
	/* ------ */
	st_data.en_type			= CTRL_DEC_SURA;
	st_data.f_acc			= 0;						// �?速度�?�?
	st_data.f_now			= st_info.f_now;			// 現在速度
	st_data.f_trgt			= st_info.f_now;			// 目標速度
	st_data.f_nowDist		= f_entryLen + st_info.f_now * ( p_sla->us_constAngvelTime + p_sla->us_accAngvelTime ) * 0.001;
	st_data.f_dist			= f_entryLen + st_info.f_now * ( p_sla->us_constAngvelTime + p_sla->us_accAngvelTime * 2 ) * 0.001;		// 減速距離
	st_data.f_accAngleS		= st_info.f_accAngleS3;		// 角加速度
	st_data.f_nowAngleS		= st_info.f_trgtAngleS;		// 現在角速度
	st_data.f_trgtAngleS		= 0;				// 目標角速度
	st_data.f_nowAngle		= st_info.f_angle1_2;		// 現在角度
	st_data.f_angle			= st_info.f_angle;			// 目標角度
	st_data.f_time			= p_sla->us_accAngvelTime * 0.001;			// [msec] �? [sec]
	CTRL_setData( &st_data );							// �?ータセ�?�?
//	LED = LED_ALL_ON;
	if( IS_R_SLA( en_type ) == TRUE ) {		// -方�?
		while( ( Get_NowAngle() > st_info.f_angle ) && ( Get_NowDist() < st_data.f_dist ) ){			// �?定角度?��距離到達�?ち
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// ブレーキ
				DCM_brakeMot( DCM_L );		// ブレーキ
				break;
			}				// 途中で制御不�?�になっ�?

		}
	}
	else{
		while( ( Get_NowAngle() < st_info.f_angle ) && ( Get_NowDist() < st_data.f_dist ) ){			// �?定角度?��距離到達�?ち
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// ブレーキ
				DCM_brakeMot( DCM_L );		// ブレーキ
				break;
			}				// 途中で制御不�?�になっ�?

		}
	}


//	LED_on(LED1);
	/* ------------------------ */
	/*  escape  */
	/* ------------------------ */
	st_data.en_type			= CTRL_EXIT_SURA;
	st_data.f_acc			= 0;						// �?速度�?�?
	st_data.f_now			= st_info.f_now;			// 現在速度
	st_data.f_trgt			= st_info.f_now;			// 目標速度
	st_data.f_nowDist		= f_entryLen + st_info.f_now * ( p_sla->us_constAngvelTime + p_sla->us_accAngvelTime * 2  ) * 0.001;
	st_data.f_dist			= f_escapeLen + f_entryLen + st_info.f_now * ( p_sla->us_constAngvelTime + p_sla->us_accAngvelTime * 2 ) * 0.001;	// スラロー�?後�?�前進距離
	st_data.f_accAngleS		= 0;						// 角加速度
	st_data.f_nowAngleS		= 0;						// 現在角速度
	st_data.f_trgtAngleS		= 0;						// 目標角度
	st_data.f_nowAngle		= 0;						// 現在角度
	st_data.f_angle			= 0;						// 目標角度
	st_data.f_time 			= 0;						// 目標時�? [sec] �? �?定しな�?
	CTRL_setData( &st_data );							// �?ータセ�?�?
//	LED =LED_ALL_OFF;
	while( Get_NowDist() < ( st_data.f_dist ) ){	// �?定距離到達�?ち
		if( SYS_isOutOfCtrl() == TRUE ){
			CTRL_stop();
			DCM_brakeMot( DCM_R );		// ブレーキ
			DCM_brakeMot( DCM_L );		// ブレーキ
			break;
		}				// 途中で制御不�?�になっ�?
	}
//	LED_off(LED1);
//	log_in(f_NowAngle);
	f_MotNowSpeed = st_info.f_now;			// 現在速度更新
//	LED =LED_ALL_OFF;
	GYRO_endErrChkAngle();					// エラー検�?�終�?
	CTRL_clrNowData();

}

void turntable(void)
{
	stCTRL_DATA test;
		test.en_type = CTRL_CONST;
		test.f_acc			= 0;						// �?速度�?�?
		test.f_now			= 0;			// 現在速度
		test.f_trgt			= 0;			// 目標速度
		test.f_nowDist			= 0;
		test.f_dist			= 0;
		test.f_accAngleS		= 0;						// 角加速度
		test.f_nowAngleS		= GYRO_getSpeedErr();						// 現在角速度
		test.f_trgtAngleS		= 0;						// 目標角度
		test.f_nowAngle			= GYRO_getNowAngle();						// 現在角度
		test.f_angle			= 0;						// 目標角度
		test.f_time 			= 0;						// 目標時�? [sec] �? �?定しな�?

	CTRL_clrData();
	CTRL_clrNowData();
	CTRL_setData(&test);
	DCM_staMotAll();									// モータON
	while( 1 ){	// �?定距離到達�?ち
		if ( SW_ON == SW_IsOn_0() ){
			CTRL_stop();
			break;
		}
	}
}

void MOT_setWallEdgeType( enMOT_WALL_EDGE_TYPE en_type )
{
	en_WallEdge = en_type;
	bl_IsWallEdge = FALSE;			// 非検知

}

enMOT_WALL_EDGE_TYPE MOT_getWallEdgeType( void )
{
	return en_WallEdge;
}

void MOT_setWallEdge( bool bl_val )
{
	bl_IsWallEdge = bl_val;

}

bool MOT_setWallEdgeDist( void )
{
	float f_addDist;

	/* 壁�?��?れ目を検知して�?な�? */
	if( ( bl_IsWallEdge == FALSE ) || ( en_WallEdge == MOT_WALL_EDGE_NONE ) ){		// 壁�??れ設定されて�?な�?か、検�?�して�?な�?場合�?�処�?を抜ける

		return FALSE;
	}

	f_addDist = Get_NowDist() + MOT_WALL_EDGE_DIST;		// 旋回開始位置

	/* 多く走る�?要がある */
	if( f_addDist > st_Info.f_dist ){

		f_WallEdgeAddDist = f_addDist - st_Info.f_dist;
	}

	/* 壁�?��?れ目補正の変数を�?�期�? */
	en_WallEdge   = MOT_WALL_EDGE_NONE;		// 壁�?��?れ目タイ�?
	bl_IsWallEdge = FALSE;					// 壁�?��?れ目検知

	return TRUE;
}
bool MOT_setWallEdgeDist_LoopWait( void )
{
	/* 壁�?��?れ目を検知して�?な�? */
	if( bl_IsWallEdge == FALSE ){		// 壁�??れ設定されて�?な�?か、検�?�して�?な�?場合�?�処�?を抜ける

		return FALSE;
	}

	f_WallEdgeAddDist = MOT_WALL_EDGE_DIST;		// 旋回開始位置

	return TRUE;
}


