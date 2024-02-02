/*
 * parameter.c
 *
 *  Created on: 2021/02/04
 *      Author: shohe
 */

#include "parameter.h"

#define GET_INDEX_ST(i)			( i - PARAM_ST_TOP - 1 )		
#define GET_INDEX_TRUN(i)		( i - PARAM_TRUN_TOP - 1 )		
#define GET_INDEX_SLA(i)		( i - PARAM_SLA_TOP - 1 )		

enPARAM_MOVE_SPEED		en_Speed_st	= PARAM_NORMAL;			// 直進時の移動速度タイプ
enPARAM_MOVE_SPEED		en_Speed_trun	= PARAM_NORMAL;		// 旋回時の移動速度タイプ
enPARAM_MOVE_SPEED		en_Speed_sla	= PARAM_NORMAL;		// スラローム時の移動速度タイプ
stSLA				st_Sla[SLA_TYPE_MAX];					// スラローム時の走行パラメータ


/* ============ */
/*  速度データ  */
/* ============ */

	/* 直進速度データ */
	const stSPEED f_StSpeedData[PARAM_MOVE_SPEED_MAX] = {
		
		//	加速度		減速度		角加速度		角減速度
		{ 1.8,			1.8,		0,				0,				},		// 超低速(PARAM_VERY_SLOW)
		{ 1.8,			1.8,		0,				0,				},		// 低速(PARAM_SLOW)
		{ 2.5,			2.5,		0,				0,				},		// 通常(PARAM_NORMAL)
		{ 4.0,			4.0,		0,				0,				},		// 高速(PARAM_FAST)
		{ 10.0,			10.0,		0,				0,				},		// 超高速(PARAM_VERY_FAST)
	};

	/* 旋回速度データ */
	const stSPEED f_TurnSpeedData[PARAM_MOVE_SPEED_MAX] = {
		
		//	加速度		減速度		角加速度		角減速度
		{ 0,			0,			22.2*PI,		22.2*PI,		},		// 超低速(PARAM_VERY_SLOW)
		{ 0,			0,			22.2*PI,		22.2*PI,		},		// 低速(PARAM_SLOW)
		{ 0,			0,			22.2*PI,		22.2*PI,		},		// 通常(PARAM_NORMAL)
		{ 0,			0,			22.2*PI,		22.2*PI,		},		// 高速(PARAM_FAST)
		{ 0,			0,			22.2*PI,		22.2*PI,		},		// 超高速(PARAM_VERY_FAST)
	};

	/* スラローム速度データ */
	const stSPEED f_SlaSpeedData[PARAM_MOVE_SPEED_MAX] = {
		
		//	加速度		減速度		角加速度		角減速度
		{ 1.8,			1.8,		10.0*PI,			10.0*PI,			},		// 超低速(PARAM_VERY_SLOW)
		{ 1.8,			1.8,		10.0*PI,			10.0*PI,			},		// 低速(PARAM_SLOW)
		{ 1.8,			1.8,		10.0*PI,			10.0*PI,			},		// 通常(PARAM_NORMAL)
		{ 1.8,			1.8,		10.0*PI,			10.0*PI,			},		// 高速(PARAM_FAST)
		{ 1.8,			1.8,		10.0*PI,			10.0*PI,			},		// 超高速(PARAM_VERY_FAST)
	};


/* ============== */
/*  GainData  */
/* ============== */


	/* 直進ゲインデータ */
	const stGAIN f_StGainData[PARAM_MOVE_SPEED_MAX] = {
		
		/* 超低速(PARAM_VERY_SLOW) */
			//速度kp	速度ki	速度kd	角速度kp	角速度ki	角速度kd	角度kp		角度ki		壁kp	壁kd
			{ 35.0,		4.0,	0.3,	95.0,		6.0,		0.2,		0.45,		0.25,	0.65,	0.2,	},
		/* 低速(PARAM_SLOW) */
			//速度kp	速度ki	速度kd	角速度kp	角速度ki	角速度kd	角度kp		角度ki		壁kp	壁kd
			{ 35.0,		4.0,	0.3,	95.0,		6.0,		0.2,		0.45,		0.25,	0.65,	0.2,	},
		/* 通常(PARAM_NORMAL) */
			//速度kp	速度ki	速度kd	角速度kp	角速度ki	角速度kd	角度kp		角度ki		壁kp	壁kd
			{ 35.0,		4.0,	0.3,	95.0,		6.0,		0.2,		0.45,		0.25,	0.65,	0.2,	},
		/* 高速(PARAM_FAST) */
			//速度kp	速度ki	速度kd	角速度kp	角速度ki	角速度kd	角度kp		角度ki		壁kp	壁kd
			{ 35.0,		4.0,	0.3,	95.0,		6.0,		0.2,		0.45,		0.25,	0.65,	0.2,	},
		/* 超高速(PARAM_VERY_FAST) */
			//速度kp	速度ki	速度kd	角速度kp	角速度ki	角速度kd	角度kp		角度ki		壁kp	壁kd
			{ 35.0,		4.0,	0.3,	95.0,		6.0,		0.2,		0.45,		0.25,	0.65,	0.2,	}
	};

	/* 旋回ゲインデータ */
	const stGAIN f_TurnGainData[PARAM_MOVE_SPEED_MAX] = {
		
		/* 超低速(PARAM_VERY_SLOW) */
			//速度kp	速度ki	速度kd	角速度kp	角速度ki	角速度kd	角度kp		角度ki		壁kp	壁kd
			{ 35.0,		4.0,	0.3,	95.0,		6.0,		0.2,		0.45,		0.25,	0.65,	0.2,	},
		/* 低速(PARAM_SLOW) */
			//速度kp	速度ki	速度kd	角速度kp	角速度ki	角速度kd	角度kp		角度ki		壁kp	壁kd
			{ 35.0,		4.0,	0.3,	95.0,		6.0,		0.2,		0.45,		0.25,	0.65,	0.2,	},
		/* 通常(PARAM_NORMAL) */
			//速度kp	速度ki	速度kd	角速度kp	角速度ki	角速度kd	角度kp		角度ki		壁kp	壁kd
			{ 35.0,		4.0,	0.3,	95.0,		6.0,		0.2,		0.45,		0.25,	0.65,	0.2,	},
		/* 高速(PARAM_FAST) */
			//速度kp	速度ki	速度kd	角速度kp	角速度ki	角速度kd	角度kp		角度ki		壁kp	壁kd
			{ 35.0,		4.0,	0.3,	95.0,		6.0,		0.2,		0.45,		0.25,	0.65,	0.2,	},
		/* 超高速(PARAM_VERY_FAST) */
			//速度kp	速度ki	速度kd	角速度kp	角速度ki	角速度kd	角度kp		角度ki		壁kp	壁kd
			{ 35.0,		4.0,	0.3,	95.0,		6.0,		0.2,		0.45,		0.25,	0.65,	0.2,	}
	};

	/* スラロームゲインデータ */
	const stGAIN f_SlaGainData[PARAM_MOVE_SPEED_MAX] = {

		/* 超低速(PARAM_VERY_SLOW) *///吸引なし　300
			//速度kp	速度ki	速度kd	角速度kp	角速度ki	角速度kd	角度kp		角度ki		壁kp	壁kd
			{ 35.0,		4.0,	0.3,	95.0,		6.0,		0.2,		0.45,		0.25,	0.65,	0.2,	},
		/* 低速(PARAM_SLOW) *///吸引あり300
			//速度kp	速度ki	速度kd	角速度kp	角速度ki	角速度kd	角度kp		角度ki		壁kp	壁kd
			{ 35.0,		4.0,	0.3,	95.0,		6.0,		0.2,		0.45,		0.25,	0.65,	0.2,	},
		/* 通常(PARAM_NORMAL)*///吸引あり600
			//速度kp	速度ki	速度kd	角速度kp	角速度ki	角速度kd	角度kp		角度ki		壁kp	壁kd
			{ 35.0,		4.0,	0.3,	95.0,		1.0,		0.2,		0.45,		0.25,	0.65,	0.2,	},
		/* 高速(PARAM_FAST)*/
			//速度kp	速度ki	速度kd	角速度kp	角速度ki	角速度kd	角度kp		角度ki		壁kp	壁kd
			{ 35.0,		4.0,	0.3,	95.0,		1.0,		0.2,		0.45,		0.25,	0.65,	0.2,	},
		/* 超高速(PARAM_VERY_FAST) */
			//速度kp	速度ki	速度kd	角速度kp	角速度ki	角速度kd	角度kp		角度ki		壁kp	壁kd
			{ 35.0,		4.0,	0.3,	95.0,		1.0,		0.2,		0.45,		0.25,	0.65,	0.2,	}
	};

const stGAIN* PARAM_getGain( enPARAM_MODE en_mode )
{
	const stGAIN* p_adr;
	
	switch( en_mode ){
		
		case PARAM_ACC:													// 加速中(直進)
		case PARAM_CONST:												// 等速中(直進)
		case PARAM_DEC:													// 減速中(直進)
//		case PARAM_BACK_ACC:											// 加速中(後進)
//		case PARAM_BACK_CONST:											// 等速中(後進)
//		case PARAM_BACK_DEC:											// 減速中(後進)
		case PARAM_SKEW_ACC:											// 加速中(斜め)
		case PARAM_SKEW_CONST:											// 等速中(斜め)
		case PARAM_SKEW_DEC:											// 減速中(斜め)
		case PARAM_HIT_WALL:											// 壁あて制御
			p_adr = &f_StGainData[en_Speed_st];
			break;
			
		case PARAM_ACC_TRUN:											// 加速中(超地信旋回)
		case PARAM_CONST_TRUN:											// 等速中(超地信旋回)
		case PARAM_DEC_TRUN:											// 減速中(超地信旋回)
			p_adr = &f_TurnGainData[en_Speed_trun];
			break;
			
		case PARAM_ENTRY_SURA:											// スラローム前の前進動作(スラローム)
		case PARAM_ACC_SURA:											// 加速中(スラローム)
		case PARAM_CONST_SURA:											// 等速中(スラローム)
		case PARAM_DEC_SURA:											// 減速中(スラローム)
		case PARAM_EXIT_SURA:											// スラローム後の前進動作(スラローム)
			p_adr = &f_SlaGainData[en_Speed_sla];
			break;
		
		default:														// Err、とりあえず・・・（メモリ破壊を防ぐため）
			printf("設定したゲインタイプがありません \n\r");
			p_adr = &f_SlaGainData[en_Speed_sla];
			break;
	}
	
	return p_adr;
}

void PARAM_setSpeedType( enPARAM_MODE en_mode, enPARAM_MOVE_SPEED en_speed )
{
	switch( en_mode ){
		
		case PARAM_ST:
			en_Speed_st = en_speed;
			break;
		
		case PARAM_TRUN:
			en_Speed_trun = en_speed;
			break;
		
		case PARAM_SLA:
			en_Speed_sla = en_speed;
			break;
			
		default:
			printf("Can't find parameter type \n\r");
			break;
	}
}

const stSPEED* PARAM_getSpeed( enPARAM_MODE en_mode )
{
	const stSPEED* p_adr;
	
	switch( en_mode ){
		
		case PARAM_ST:													// 直進
		case PARAM_ACC:													// 加速中(直進)
		case PARAM_CONST:												// 等速中(直進)
		case PARAM_DEC:													// 減速中(直進)
//		case PARAM_BACK_ACC:											// 加速中(後進)
//		case PARAM_BACK_CONST:											// 等速中(後進)
//		case PARAM_BACK_DEC:											// 減速中(後進)
		case PARAM_SKEW_ACC:											// 加速中(斜め)
		case PARAM_SKEW_CONST:											// 等速中(斜め)
		case PARAM_SKEW_DEC:											// 減速中(斜め)
		case PARAM_HIT_WALL:											// 壁あて制御
			p_adr = &f_StSpeedData[en_Speed_st];
			break;
			
		case PARAM_TRUN:												// 旋回
		case PARAM_ACC_TRUN:											// 加速中(超地信旋回)
		case PARAM_CONST_TRUN:											// 等速中(超地信旋回)
		case PARAM_DEC_TRUN:											// 減速中(超地信旋回)
			p_adr = &f_TurnSpeedData[en_Speed_trun];
			break;
			
		case PARAM_SLA:													// スラローム
		case PARAM_ENTRY_SURA:											// スラローム前の前進動作(スラローム)
		case PARAM_ACC_SURA:											// 加速中(スラローム)
		case PARAM_CONST_SURA:											// 等速中(スラローム)
		case PARAM_DEC_SURA:											// 減速中(スラローム)
		case PARAM_EXIT_SURA:											// スラローム後の前進動作(スラローム)
			p_adr = &f_SlaSpeedData[en_Speed_sla];
			break;

		default:														// Err、とりあえず・・・（メモリ破壊を防ぐため）
			printf("Can't find speed type \n\r");
			p_adr = &f_SlaSpeedData[en_Speed_sla];
			break;
	}
	
	return p_adr;
}
/*
const stGAIN* PARAM_getGain( enPARAM_MODE en_mode )
{
	const stGAIN* p_adr;
	
	switch( en_mode ){
		
		case PARAM_ACC:													// 加速中(直進)
		case PARAM_CONST:												// 等速中(直進)
		case PARAM_DEC:													// 減速中(直進)
//		case PARAM_BACK_ACC:											// 加速中(後進)
//		case PARAM_BACK_CONST:											// 等速中(後進)
//		case PARAM_BACK_DEC:											// 減速中(後進)
		case PARAM_SKEW_ACC:											// 加速中(斜め)
		case PARAM_SKEW_CONST:											// 等速中(斜め)
		case PARAM_SKEW_DEC:											// 減速中(斜め)
		case PARAM_HIT_WALL:											// 壁あて制御
			p_adr = &f_StGainData[en_Speed_st][GET_INDEX_ST( en_mode )];
			break;
			
		case PARAM_ACC_TRUN:											// 加速中(超地信旋回)
		case PARAM_CONST_TRUN:											// 等速中(超地信旋回)
		case PARAM_DEC_TRUN:											// 減速中(超地信旋回)
			p_adr = &f_TurnGainData[en_Speed_trun][GET_INDEX_TRUN( en_mode )];
			break;
			
		case PARAM_ENTRY_SURA:											// スラローム前の前進動作(スラローム)
		case PARAM_ACC_SURA:											// 加速中(スラローム)
		case PARAM_CONST_SURA:											// 等速中(スラローム)
		case PARAM_DEC_SURA:											// 減速中(スラローム)
		case PARAM_EXIT_SURA:											// スラローム後の前進動作(スラローム)
			p_adr = &f_SlaGainData[en_Speed_sla][GET_INDEX_SLA( en_mode )];
			break;
		
		default:														// Err、とりあえず・・・（メモリ破壊を防ぐため）
			printf("Can't find gain type \n\r");
			p_adr = &f_SlaGainData[en_Speed_sla][GET_INDEX_SLA( en_mode )];
			break;
	}
	
	return p_adr;
}
*/

void PARAM_makeSra( float f_speed, float f_angAcc, float f_g , enSLA_TYPE en_mode)
{

	float	f_start_x;					// 開始x位置 [mm]
	float	f_start_y;					// 開始y位置 [mm]
	float	f_final_x;					// 最終x位置 [mm]
	float	f_final_y;					// 最終y位置 [mm]
	float	f_final_ang;				// 角減速時の最終角度 [rad]	
	float	f_maxAngleV		= 0;		// 最大角速度[rad/s]
	float	f_timeAcc		= 0;		// 加速時間[s]
	float	f_accAngle		= 0;		// 加速角度[rad]
	float	f_timeConst		= 0;		// 等速時間[s]
	float	f_constAngle	= 0;		// 等速角度[rad]
	float	f_ang			= 0;		// 演算用、角度 [rad]
	float	f_time			= 0;		// 演算用、時間 [s]
	float	f_x;						// 演算用x位置 [mm]
	float	f_y;						// 演算用y位置 [mm]
	uint16_t	i = 0;						// ループ用
	stSLA* 	p_adr = &st_Sla[en_mode];		// 記録する走行データ

	// スラロームに応じた設定値からスラロームに必要なパラメータを演算する 
	switch(en_mode){

		case SLA_90:
			f_start_x   = HALF_BLOCK;
			f_start_y   = 0.0f;
			f_final_x   = BLOCK;
			f_final_y   = HALF_BLOCK;
			f_final_ang = 90.0f * DEG_TO_RAD;
			break;

		case SLA_45:
			f_start_x   = HALF_BLOCK;
			f_start_y   = 0.0f;
			f_final_x   = BLOCK * 0.75f;
			f_final_y   = BLOCK * 0.75f;
			f_final_ang = 45.0f * DEG_TO_RAD;
			break;
			
		case SLA_N90:
			f_start_x   = HALF_BLOCK * 0.5f * 1.4142f;
			f_start_y   = 0.0f;
			f_final_x   = HALF_BLOCK * 1.4142f;
			f_final_y   = HALF_BLOCK * 0.5f * 1.4142f;
			f_final_ang = 90.0f * DEG_TO_RAD;
			break;
			
		case SLA_135:
			f_start_x   = HALF_BLOCK;
			f_start_y   = 0.0f;
			f_final_x   = BLOCK * 1.25f;
			f_final_y   = BLOCK * 0.25;
			f_final_ang = 135.0f * DEG_TO_RAD;
			break;

		default:
			break;
	}

	// caluculate acc and dec angle speed 
	f_maxAngleV		= f_g / f_speed;							// max angle speed[rad/s] (omega[rad/s] = g[m/s^2] / v[m/s] )
	f_timeAcc		= f_maxAngleV / f_angAcc;					// acc time[s]
	f_accAngle		= 0.5f * f_angAcc * f_timeAcc * f_timeAcc;	// acc angle[rad] (theta[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
	f_constAngle	= f_final_ang - f_accAngle * 2;				// const angle[rad] (theta[rad] = Totalangle - (acc angle + dec angle) )
	f_timeConst		= f_constAngle / f_maxAngleV;				// max angle speed time[s]( t[s] = theta[rad] / omega[rad/s] )

	// -------------------------------- 
	// sla end position 
	// -------------------------------- 
	// start position 
	f_x		= f_start_x;
	f_y		= f_start_y;

	// ACC 
	for( i=0; i<(uint16_t)(f_timeAcc*1000); i++ ){				// [msec]
	
		f_time	=  0.001f * (float)i;								// time[s]
		f_ang	=  0.5f * f_angAcc * f_time * f_time;				// angle[rad] (theta[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;			// Xposition[m]
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;			// Yposition[m]
	}
	
	// CONST 
	for( i=0; i<(uint16_t)(f_timeConst*1000); i++ ){				// [msec]
	
		f_time	 = 0.001f * (float)i;							// time[s]
		f_ang	 = f_accAngle + f_maxAngleV * f_time;			// angle[rad] (theta[rad] = omega[rad/s] * t[s] )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;		// Xposition[m]
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;		// Yposition[m]
	}

	// DEC
	for( i=0; i<(uint16_t)(f_timeAcc*1000); i++ ){				// [msec]
	
		f_time	 = 0.001f * (float)i;							// time[s]
		f_ang	 = f_accAngle + f_constAngle +0.5f * f_angAcc * f_time * f_time;	// angle[rad] (theta[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;		// Xposition[m]
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;		// Yposition[m]
	}

	// ---------------------------- 
	//  sla parameter  
	// ---------------------------- 
	p_adr->f_speed				= f_speed;
	printf("enter speed %5.2f\n\r",f_speed);
	p_adr->f_angAcc				= f_angAcc;// * RAD_TO_DEG ;
	printf("angle acc%5.2f\n\r",f_angAcc);// * RAD_TO_DEG);
	p_adr->f_angvel				= f_maxAngleV;// * RAD_TO_DEG;
	printf("max angle speed%5.2f\n\r",f_maxAngleV);// * RAD_TO_DEG);
	p_adr->us_accAngvelTime		= (uint16_t)( f_timeAcc * 1000.0f );
	printf("time of angle acc %5.2f\n\r",f_timeAcc * 1000.0f);
	p_adr->us_constAngvelTime	= (uint16_t)( f_timeConst * 1000.0f );
	printf("time of constant angle acc %5.2f\n\r",f_timeConst * 1000.0f);
	p_adr->f_ang_AccEnd			= f_accAngle;// * RAD_TO_DEG;
	printf("acc angle[deg] %5.2f\n\r",f_accAngle);// * RAD_TO_DEG);
	p_adr->f_ang_ConstEnd		= ( f_accAngle + f_constAngle );// * RAD_TO_DEG;
	printf("const angle[deg] %5.2f\n\r",( f_accAngle + f_constAngle ));// * RAD_TO_DEG);
	p_adr->f_ang_Total			= f_final_ang;// * RAD_TO_DEG;
	printf("angle [deg] %5.2f\n\r",f_final_ang);// * RAD_TO_DEG);
	
	// calaculate enter and exit length 
	switch(en_mode){
		case SLA_90:
			p_adr->f_escapeLen = f_final_x - f_x+0.003 ;
			p_adr->f_entryLen  = f_final_y - f_y-0.001 ;
			break;

		case SLA_45:
			p_adr->f_escapeLen = 1.4142f * ( f_final_x - f_x )+0.00;
			p_adr->f_entryLen  = f_final_y - f_y - ( f_final_x - f_x )+0.003;
			break;

		case SLA_N90:
			p_adr->f_escapeLen = f_final_x - f_x+0.002;
			p_adr->f_entryLen  = f_final_y - f_y-0.002;
			break;

		case SLA_135:
			p_adr->f_escapeLen = 1.4142f * ( f_final_x - f_x )+0.005;
			p_adr->f_entryLen  = f_final_y - f_y + ( f_final_x - f_x )-0.003;
			break;

		default:
			break;
	}
	printf("entry %6.4f\n\r",f_final_x - f_x);
	printf("escape %6.4f\n\r",f_final_y - f_y);
}

void PARAM_makeSra_90( float f_speed, float f_angAcc, float f_g , enSLA_TYPE en_mode)
{

	float	f_start_x;					// 開始x位置 [mm]
	float	f_start_y;					// 開始y位置 [mm]
	float	f_final_x;					// 最終x位置 [mm]
	float	f_final_y;					// 最終y位置 [mm]
	float	f_final_ang;				// 角減速時の最終角度 [rad]	
	float	f_maxAngleV		= 0;		// 最大角速度[rad/s]
	float	f_timeAcc		= 0;		// 加速時間[s]
	float	f_accAngle		= 0;		// 加速角度[rad]
	float	f_timeConst		= 0;		// 等速時間[s]
	float	f_constAngle	= 0;		// 等速角度[rad]
	float	f_ang			= 0;		// 演算用、角度 [rad]
	float	f_time			= 0;		// 演算用、時間 [s]
	float	f_x;						// 演算用x位置 [mm]
	float	f_y;						// 演算用y位置 [mm]
	uint16_t	i = 0;						// ループ用
	stSLA* 	p_adr = &st_Sla[en_mode];		// 記録する走行データ

	/* スラロームに応じた設定値からスラロームに必要なパラメータを演算する */
	switch(en_mode){

		case SLA_90:
			f_start_x   = HALF_BLOCK;
			f_start_y   = 0.0f;
			f_final_x   = BLOCK;
			f_final_y   = HALF_BLOCK;
			f_final_ang = 90.0f * DEG_TO_RAD;
			break;

		default:
			break;
	}

	/* caluculate acc and dec angle speed */
	f_maxAngleV		= f_g / f_speed;							// max angle speed[rad/s] (omega[rad/s] = g[m/s^2] / v[m/s] )
	f_timeAcc		= f_maxAngleV / f_angAcc;					// acc time[s]
	f_accAngle		= 0.5f * f_angAcc * f_timeAcc * f_timeAcc;	// acc angle[rad] (theta[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
	f_constAngle	= f_final_ang - f_accAngle * 2;				// const angle[rad] (theta[rad] = Totalangle - (acc angle + dec angle) )
	f_timeConst		= f_constAngle / f_maxAngleV;				// max angle speed time[s]( t[s] = theta[rad] / omega[rad/s] )

	/* -------------------------------- */
	/* sla end position */
	/* -------------------------------- */
	/* start position */
	f_x		= f_start_x;
	f_y		= f_start_y;

	/* ACC */
	for( i=0; i<(uint16_t)(f_timeAcc*1000); i++ ){				// [msec]
	
		f_time	=  0.001f * (float)i;								// time[s]
		f_ang	=  0.5f * f_angAcc * f_time * f_time;				// angle[rad] (theta[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;			// Xposition[m]
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;			// Yposition[m]
	}
	
	/* CONST */
	for( i=0; i<(uint16_t)(f_timeConst*1000); i++ ){				// [msec]
	
		f_time	 = 0.001f * (float)i;							// time[s]
		f_ang	 = f_accAngle + f_maxAngleV * f_time;			// angle[rad] (theta[rad] = omega[rad/s] * t[s] )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;		// Xposition[m]
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;		// Yposition[m]
	}

	/* DEC*/
	for( i=0; i<(uint16_t)(f_timeAcc*1000); i++ ){				// [msec]
	
		f_time	 = 0.001f * (float)i;							// time[s]
		f_ang	 = f_accAngle + f_constAngle +0.5f * f_angAcc * f_time * f_time;	// angle[rad] (theta[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;		// Xposition[m]
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;		// Yposition[m]
	}

	/* ---------------------------- */
	/*  sla parameter  */
	/* ---------------------------- */
	p_adr->f_speed				= f_speed;
	printf("enter speed %5.2f\n\r",f_speed);
	p_adr->f_angAcc				= f_angAcc;// * RAD_TO_DEG ;
	printf("angle acc%5.2f\n\r",f_angAcc);// * RAD_TO_DEG);
	p_adr->f_angvel				= f_maxAngleV;// * RAD_TO_DEG;
	printf("max angle speed%5.2f\n\r",f_maxAngleV);// * RAD_TO_DEG);
	p_adr->us_accAngvelTime		= (uint16_t)( f_timeAcc * 1000.0f );
	printf("time of angle acc %5.2f\n\r",f_timeAcc * 1000.0f);
	p_adr->us_constAngvelTime	= (uint16_t)( f_timeConst * 1000.0f );
	printf("time of constant angle acc %5.2f\n\r",f_timeConst * 1000.0f);
	p_adr->f_ang_AccEnd			= f_accAngle;// * RAD_TO_DEG;
	printf("acc angle[deg] %5.2f\n\r",f_accAngle);// * RAD_TO_DEG);
	p_adr->f_ang_ConstEnd		= ( f_accAngle + f_constAngle );// * RAD_TO_DEG;
	printf("const angle[deg] %5.2f\n\r",( f_accAngle + f_constAngle ));// * RAD_TO_DEG);
	p_adr->f_ang_Total			= f_final_ang;// * RAD_TO_DEG;
	printf("angle [deg] %5.2f\n\r",f_final_ang);// * RAD_TO_DEG);
	
	/* calaculate enter and exit length */
	switch(en_mode){
		case SLA_90:
			p_adr->f_escapeLen = f_final_x - f_x+0.003 ;
			p_adr->f_entryLen  = f_final_y - f_y-0.001 ;
			break;

		default:
			break;
	}
	printf("entry %6.4f\n\r",f_final_x - f_x);
	printf("escape %6.4f\n\r",f_final_y - f_y);
}

void PARAM_makeSra_45( float f_speed, float f_angAcc, float f_g , enSLA_TYPE en_mode)
{

	float	f_start_x;					// 開始x位置 [mm]
	float	f_start_y;					// 開始y位置 [mm]
	float	f_final_x;					// 最終x位置 [mm]
	float	f_final_y;					// 最終y位置 [mm]
	float	f_final_ang;				// 角減速時の最終角度 [rad]	
	float	f_maxAngleV		= 0;		// 最大角速度[rad/s]
	float	f_timeAcc		= 0;		// 加速時間[s]
	float	f_accAngle		= 0;		// 加速角度[rad]
	float	f_timeConst		= 0;		// 等速時間[s]
	float	f_constAngle	= 0;		// 等速角度[rad]
	float	f_ang			= 0;		// 演算用、角度 [rad]
	float	f_time			= 0;		// 演算用、時間 [s]
	float	f_x;						// 演算用x位置 [mm]
	float	f_y;						// 演算用y位置 [mm]
	uint16_t	i = 0;						// ループ用
	stSLA* 	p_adr = &st_Sla[en_mode];		// 記録する走行データ

	/* スラロームに応じた設定値からスラロームに必要なパラメータを演算する */
	switch(en_mode){
		case SLA_45:
			f_start_x   = HALF_BLOCK;
			f_start_y   = 0.0f;
			f_final_x   = BLOCK * 0.75f;
			f_final_y   = BLOCK * 0.75f;
			f_final_ang = 45.0f * DEG_TO_RAD;
			break;

		default:
			break;
	}

	/* caluculate acc and dec angle speed */
	f_maxAngleV		= f_g / f_speed;							// max angle speed[rad/s] (omega[rad/s] = g[m/s^2] / v[m/s] )
	f_timeAcc		= f_maxAngleV / f_angAcc;					// acc time[s]
	f_accAngle		= 0.5f * f_angAcc * f_timeAcc * f_timeAcc;	// acc angle[rad] (theta[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
	f_constAngle	= f_final_ang - f_accAngle * 2;				// const angle[rad] (theta[rad] = Totalangle - (acc angle + dec angle) )
	f_timeConst		= f_constAngle / f_maxAngleV;				// max angle speed time[s]( t[s] = theta[rad] / omega[rad/s] )

	/* -------------------------------- */
	/* sla end position */
	/* -------------------------------- */
	/* start position */
	f_x		= f_start_x;
	f_y		= f_start_y;

	/* ACC */
	for( i=0; i<(uint16_t)(f_timeAcc*1000); i++ ){				// [msec]
	
		f_time	=  0.001f * (float)i;								// time[s]
		f_ang	=  0.5f * f_angAcc * f_time * f_time;				// angle[rad] (theta[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;			// Xposition[m]
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;			// Yposition[m]
	}
	
	/* CONST */
	for( i=0; i<(uint16_t)(f_timeConst*1000); i++ ){				// [msec]
	
		f_time	 = 0.001f * (float)i;							// time[s]
		f_ang	 = f_accAngle + f_maxAngleV * f_time;			// angle[rad] (theta[rad] = omega[rad/s] * t[s] )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;		// Xposition[m]
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;		// Yposition[m]
	}

	/* DEC*/
	for( i=0; i<(uint16_t)(f_timeAcc*1000); i++ ){				// [msec]
	
		f_time	 = 0.001f * (float)i;							// time[s]
		f_ang	 = f_accAngle + f_constAngle +0.5f * f_angAcc * f_time * f_time;	// angle[rad] (theta[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;		// Xposition[m]
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;		// Yposition[m]
	}

	/* ---------------------------- */
	/*  sla parameter  */
	/* ---------------------------- */
	p_adr->f_speed				= f_speed;
	printf("enter speed %5.2f\n\r",f_speed);
	p_adr->f_angAcc				= f_angAcc;// * RAD_TO_DEG ;
	printf("angle acc%5.2f\n\r",f_angAcc);// * RAD_TO_DEG);
	p_adr->f_angvel				= f_maxAngleV;// * RAD_TO_DEG;
	printf("max angle speed%5.2f\n\r",f_maxAngleV);// * RAD_TO_DEG);
	p_adr->us_accAngvelTime		= (uint16_t)( f_timeAcc * 1000.0f );
	printf("time of angle acc %5.2f\n\r",f_timeAcc * 1000.0f);
	p_adr->us_constAngvelTime	= (uint16_t)( f_timeConst * 1000.0f );
	printf("time of constant angle acc %5.2f\n\r",f_timeConst * 1000.0f);
	p_adr->f_ang_AccEnd			= f_accAngle;// * RAD_TO_DEG;
	printf("acc angle[deg] %5.2f\n\r",f_accAngle);// * RAD_TO_DEG);
	p_adr->f_ang_ConstEnd		= ( f_accAngle + f_constAngle );// * RAD_TO_DEG;
	printf("const angle[deg] %5.2f\n\r",( f_accAngle + f_constAngle ));// * RAD_TO_DEG);
	p_adr->f_ang_Total			= f_final_ang;// * RAD_TO_DEG;
	printf("angle [deg] %5.2f\n\r",f_final_ang);// * RAD_TO_DEG);
	
	/* calaculate enter and exit length */
	switch(en_mode){
		case SLA_45:
			p_adr->f_escapeLen = 1.4142f * ( f_final_x - f_x )+0.00;
			p_adr->f_entryLen  = f_final_y - f_y - ( f_final_x - f_x )+0.003;
			break;

		default:
			break;
	}
	printf("entry %6.4f\n\r",f_final_x - f_x);
	printf("escape %6.4f\n\r",f_final_y - f_y);
}

void PARAM_makeSra_N90( float f_speed, float f_angAcc, float f_g , enSLA_TYPE en_mode)
{

	float	f_start_x;					// 開始x位置 [mm]
	float	f_start_y;					// 開始y位置 [mm]
	float	f_final_x;					// 最終x位置 [mm]
	float	f_final_y;					// 最終y位置 [mm]
	float	f_final_ang;				// 角減速時の最終角度 [rad]	
	float	f_maxAngleV		= 0;		// 最大角速度[rad/s]
	float	f_timeAcc		= 0;		// 加速時間[s]
	float	f_accAngle		= 0;		// 加速角度[rad]
	float	f_timeConst		= 0;		// 等速時間[s]
	float	f_constAngle	= 0;		// 等速角度[rad]
	float	f_ang			= 0;		// 演算用、角度 [rad]
	float	f_time			= 0;		// 演算用、時間 [s]
	float	f_x;						// 演算用x位置 [mm]
	float	f_y;						// 演算用y位置 [mm]
	uint16_t	i = 0;						// ループ用
	stSLA* 	p_adr = &st_Sla[en_mode];		// 記録する走行データ

	/* スラロームに応じた設定値からスラロームに必要なパラメータを演算する */
	switch(en_mode){
		case SLA_N90:
			f_start_x   = HALF_BLOCK * 0.5f * 1.4142f;
			f_start_y   = 0.0f;
			f_final_x   = HALF_BLOCK * 1.4142f;
			f_final_y   = HALF_BLOCK * 0.5f * 1.4142f;
			f_final_ang = 90.0f * DEG_TO_RAD;
			break;

		default:
			break;
	}

	/* caluculate acc and dec angle speed */
	f_maxAngleV		= f_g / f_speed;							// max angle speed[rad/s] (omega[rad/s] = g[m/s^2] / v[m/s] )
	f_timeAcc		= f_maxAngleV / f_angAcc;					// acc time[s]
	f_accAngle		= 0.5f * f_angAcc * f_timeAcc * f_timeAcc;	// acc angle[rad] (theta[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
	f_constAngle	= f_final_ang - f_accAngle * 2;				// const angle[rad] (theta[rad] = Totalangle - (acc angle + dec angle) )
	f_timeConst		= f_constAngle / f_maxAngleV;				// max angle speed time[s]( t[s] = theta[rad] / omega[rad/s] )

	/* -------------------------------- */
	/* sla end position */
	/* -------------------------------- */
	/* start position */
	f_x		= f_start_x;
	f_y		= f_start_y;

	/* ACC */
	for( i=0; i<(uint16_t)(f_timeAcc*1000); i++ ){				// [msec]
	
		f_time	=  0.001f * (float)i;								// time[s]
		f_ang	=  0.5f * f_angAcc * f_time * f_time;				// angle[rad] (theta[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;			// Xposition[m]
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;			// Yposition[m]
	}
	
	/* CONST */
	for( i=0; i<(uint16_t)(f_timeConst*1000); i++ ){				// [msec]
	
		f_time	 = 0.001f * (float)i;							// time[s]
		f_ang	 = f_accAngle + f_maxAngleV * f_time;			// angle[rad] (theta[rad] = omega[rad/s] * t[s] )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;		// Xposition[m]
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;		// Yposition[m]
	}

	/* DEC*/
	for( i=0; i<(uint16_t)(f_timeAcc*1000); i++ ){				// [msec]
	
		f_time	 = 0.001f * (float)i;							// time[s]
		f_ang	 = f_accAngle + f_constAngle +0.5f * f_angAcc * f_time * f_time;	// angle[rad] (theta[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;		// Xposition[m]
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;		// Yposition[m]
	}

	/* ---------------------------- */
	/*  sla parameter  */
	/* ---------------------------- */
	p_adr->f_speed				= f_speed;
	printf("enter speed %5.2f\n\r",f_speed);
	p_adr->f_angAcc				= f_angAcc;// * RAD_TO_DEG ;
	printf("angle acc%5.2f\n\r",f_angAcc);// * RAD_TO_DEG);
	p_adr->f_angvel				= f_maxAngleV;// * RAD_TO_DEG;
	printf("max angle speed%5.2f\n\r",f_maxAngleV);// * RAD_TO_DEG);
	p_adr->us_accAngvelTime		= (uint16_t)( f_timeAcc * 1000.0f );
	printf("time of angle acc %5.2f\n\r",f_timeAcc * 1000.0f);
	p_adr->us_constAngvelTime	= (uint16_t)( f_timeConst * 1000.0f );
	printf("time of constant angle acc %5.2f\n\r",f_timeConst * 1000.0f);
	p_adr->f_ang_AccEnd			= f_accAngle;// * RAD_TO_DEG;
	printf("acc angle[deg] %5.2f\n\r",f_accAngle);// * RAD_TO_DEG);
	p_adr->f_ang_ConstEnd		= ( f_accAngle + f_constAngle );// * RAD_TO_DEG;
	printf("const angle[deg] %5.2f\n\r",( f_accAngle + f_constAngle ));// * RAD_TO_DEG);
	p_adr->f_ang_Total			= f_final_ang;// * RAD_TO_DEG;
	printf("angle [deg] %5.2f\n\r",f_final_ang);// * RAD_TO_DEG);
	
	/* calaculate enter and exit length */
	switch(en_mode){
		case SLA_90:
			p_adr->f_escapeLen = f_final_x - f_x+0.003 ;
			p_adr->f_entryLen  = f_final_y - f_y-0.001 ;
			break;

		case SLA_45:
			p_adr->f_escapeLen = 1.4142f * ( f_final_x - f_x )+0.00;
			p_adr->f_entryLen  = f_final_y - f_y - ( f_final_x - f_x )+0.003;
			break;

		case SLA_N90:
			p_adr->f_escapeLen = f_final_x - f_x+0.002;
			p_adr->f_entryLen  = f_final_y - f_y-0.002;
			break;

		case SLA_135:
			p_adr->f_escapeLen = 1.4142f * ( f_final_x - f_x )+0.005;
			p_adr->f_entryLen  = f_final_y - f_y + ( f_final_x - f_x )-0.003;
			break;

		default:
			break;
	}
	printf("entry %6.4f\n\r",f_final_x - f_x);
	printf("escape %6.4f\n\r",f_final_y - f_y);
}

void PARAM_makeSra_135( float f_speed, float f_angAcc, float f_g , enSLA_TYPE en_mode)
{

	float	f_start_x;					// 開始x位置 [mm]
	float	f_start_y;					// 開始y位置 [mm]
	float	f_final_x;					// 最終x位置 [mm]
	float	f_final_y;					// 最終y位置 [mm]
	float	f_final_ang;				// 角減速時の最終角度 [rad]	
	float	f_maxAngleV		= 0;		// 最大角速度[rad/s]
	float	f_timeAcc		= 0;		// 加速時間[s]
	float	f_accAngle		= 0;		// 加速角度[rad]
	float	f_timeConst		= 0;		// 等速時間[s]
	float	f_constAngle	= 0;		// 等速角度[rad]
	float	f_ang			= 0;		// 演算用、角度 [rad]
	float	f_time			= 0;		// 演算用、時間 [s]
	float	f_x;						// 演算用x位置 [mm]
	float	f_y;						// 演算用y位置 [mm]
	uint16_t	i = 0;						// ループ用
	stSLA* 	p_adr = &st_Sla[en_mode];		// 記録する走行データ

	/* スラロームに応じた設定値からスラロームに必要なパラメータを演算する */
	switch(en_mode){
		case SLA_135:
			f_start_x   = HALF_BLOCK;
			f_start_y   = 0.0f;
			f_final_x   = BLOCK * 1.25f;
			f_final_y   = BLOCK * 0.25;
			f_final_ang = 135.0f * DEG_TO_RAD;
			break;

		default:
			break;
	}

	/* caluculate acc and dec angle speed */
	f_maxAngleV		= f_g / f_speed;							// max angle speed[rad/s] (omega[rad/s] = g[m/s^2] / v[m/s] )
	f_timeAcc		= f_maxAngleV / f_angAcc;					// acc time[s]
	f_accAngle		= 0.5f * f_angAcc * f_timeAcc * f_timeAcc;	// acc angle[rad] (theta[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
	f_constAngle	= f_final_ang - f_accAngle * 2;				// const angle[rad] (theta[rad] = Totalangle - (acc angle + dec angle) )
	f_timeConst		= f_constAngle / f_maxAngleV;				// max angle speed time[s]( t[s] = theta[rad] / omega[rad/s] )

	/* -------------------------------- */
	/* sla end position */
	/* -------------------------------- */
	/* start position */
	f_x		= f_start_x;
	f_y		= f_start_y;

	/* ACC */
	for( i=0; i<(uint16_t)(f_timeAcc*1000); i++ ){				// [msec]
	
		f_time	=  0.001f * (float)i;								// time[s]
		f_ang	=  0.5f * f_angAcc * f_time * f_time;				// angle[rad] (theta[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;			// Xposition[m]
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;			// Yposition[m]
	}
	
	/* CONST */
	for( i=0; i<(uint16_t)(f_timeConst*1000); i++ ){				// [msec]
	
		f_time	 = 0.001f * (float)i;							// time[s]
		f_ang	 = f_accAngle + f_maxAngleV * f_time;			// angle[rad] (theta[rad] = omega[rad/s] * t[s] )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;		// Xposition[m]
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;		// Yposition[m]
	}

	/* DEC*/
	for( i=0; i<(uint16_t)(f_timeAcc*1000); i++ ){				// [msec]
	
		f_time	 = 0.001f * (float)i;							// time[s]
		f_ang	 = f_accAngle + f_constAngle +0.5f * f_angAcc * f_time * f_time;	// angle[rad] (theta[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;		// Xposition[m]
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;		// Yposition[m]
	}

	/* ---------------------------- */
	/*  sla parameter  */
	/* ---------------------------- */
	p_adr->f_speed				= f_speed;
	printf("enter speed %5.2f\n\r",f_speed);
	p_adr->f_angAcc				= f_angAcc;// * RAD_TO_DEG ;
	printf("angle acc%5.2f\n\r",f_angAcc);// * RAD_TO_DEG);
	p_adr->f_angvel				= f_maxAngleV;// * RAD_TO_DEG;
	printf("max angle speed%5.2f\n\r",f_maxAngleV);// * RAD_TO_DEG);
	p_adr->us_accAngvelTime		= (uint16_t)( f_timeAcc * 1000.0f );
	printf("time of angle acc %5.2f\n\r",f_timeAcc * 1000.0f);
	p_adr->us_constAngvelTime	= (uint16_t)( f_timeConst * 1000.0f );
	printf("time of constant angle acc %5.2f\n\r",f_timeConst * 1000.0f);
	p_adr->f_ang_AccEnd			= f_accAngle;// * RAD_TO_DEG;
	printf("acc angle[deg] %5.2f\n\r",f_accAngle);// * RAD_TO_DEG);
	p_adr->f_ang_ConstEnd		= ( f_accAngle + f_constAngle );// * RAD_TO_DEG;
	printf("const angle[deg] %5.2f\n\r",( f_accAngle + f_constAngle ));// * RAD_TO_DEG);
	p_adr->f_ang_Total			= f_final_ang;// * RAD_TO_DEG;
	printf("angle [deg] %5.2f\n\r",f_final_ang);// * RAD_TO_DEG);
	
	/* calaculate enter and exit length */
	switch(en_mode){
		case SLA_135:
			p_adr->f_escapeLen = 1.4142f * ( f_final_x - f_x )+0.005;
			p_adr->f_entryLen  = f_final_y - f_y + ( f_final_x - f_x )-0.003;
			break;

		default:
			break;
	}
	printf("entry %6.4f\n\r",f_final_x - f_x);
	printf("escape %6.4f\n\r",f_final_y - f_y);
}

stSLA* PARAM_getSra( enSLA_TYPE en_mode )
{
	return &st_Sla[en_mode];
}
