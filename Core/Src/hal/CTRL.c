/*
 * CTRL.c
 *
 *  Created on: 2021/01/30
 *      Author: shohe
 */


#include "hal/CTRL.h"


/* control  */
uint8_t 			uc_CtrlFlag			= FALSE;	// FF,FB active Flag (1:active)
int32_t			l_CntR;							// right motor count (updated 1[msec])
int32_t			l_CntL;							// left motor count (updated 1[msec])
float			f_Time 				= 0;		// operating time[sec] (updated 1[msec])
float			f_TrgtTime 			= 1000;		// operating target time [msec]	(set value)
// velocity control
float 			f_Acc			= 0;		// [velocity control]   acceleration[m/s2]	(set value)
float			f_BaseSpeed		= 0;		// [velocity control]   initial velocity[m/s]	(set value)
float			f_LastSpeed 		= 0;		// [velocity control]   last target velocity[m/s]	(set value)
float			f_NowSpeed		= 0;		// [velocity control]   now velocity [m/s]	(updated 1[msec])
 float			f_TrgtSpeed 		= 0;		// [velocity control]   target velocity[m/s]		(updated 1[msec])
float			f_ErrSpeedBuf		= 0;		// [velocity control] 縲velocity error buffer	(updated 1[msec])
float			f_SpeedErrSum 		= 0;		// [velocity control]   sum of velocity integral control	(updated 1[msec])

float			f_MotorR_AngleS = 0;
float			f_MotorL_AngleS = 0;

// dist
float			f_BaseDist		= 0;		// [dist]   initial distance[m]		(set value)
float			f_LastDist 		= 0;		// [霍晞屬蛻ｶ蠕｡]   譛邨らｧｻ蜍戊ｷ晞屬					???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ險ｭ螳壼､???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
float			f_TrgtDist 		= 0;		// [霍晞屬蛻ｶ蠕｡]   逶ｮ讓咏ｧｻ蜍戊ｷ晞屬					???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?
volatile float 		f_NowDist		= 0;		// [霍晞屬蛻ｶ蠕｡]   迴ｾ蝨ｨ霍晞屬						???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?
float			f_NowDistR		= 0;		// [霍晞屬蛻ｶ蠕｡]   迴ｾ蝨ｨ霍晞屬???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ蜿ｳ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?					???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?
float 			f_NowDistL		= 0;		// [霍晞屬蛻ｶ蠕｡]   迴ｾ蝨ｨ霍晞屬???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ蟾ｦ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?					???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?
float			f_DistErrSum 		= 0;		// [霍晞屬蛻ｶ蠕｡]   霍晞屬遨搾ｿｽ??蛻ｶ蠕｡縺ｮ繧ｵ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?蛟､			???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?
// angular velocity control
float 			f_AccAngleS		= 0;		// [angle velocity control] angular acceleration[rad/s2]	(set value)
float			f_BaseAngleS		= 0;		// [angle velocity control] initial angular velocity[rad/s]		(set value)
float			f_LastAngleS 		= 0;		// [angle velocity control] last target angular velocity	(set value)
float			f_TrgtAngleS 		= 0;		// [angle velocity control] target angular velocity [rad/s]	(updated 1[msec])
float			f_ErrAngleSBuf		= 0;		// [angle velocity control] angular velocity buffer	(updated 1[msec])
float			f_AngleSErrSum 		= 0;		// [angle velocity control] sum of angular velocity integral contorol	(updated 1[msec])
// angle
float			f_BaseAngle		= 0;		// [隗貞ｺｦ蛻ｶ蠕｡]   蛻晄悄隗貞ｺｦ						???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ險ｭ螳壼､???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
float			f_LastAngle 		= 0;		// [隗貞ｺｦ蛻ｶ蠕｡]   譛邨ら岼讓呵ｧ貞ｺｦ					???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ險ｭ螳壼､???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
volatile float 		f_NowAngle		= 0;		// [隗貞ｺｦ蛻ｶ蠕｡]   迴ｾ蝨ｨ隗貞ｺｦ縲	volatile繧偵▽縺代↑??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?縺ｨwhile縺九ｉ謚懊￠繧峨ｌ縺ｪ縺上↑繧具ｼ域怙驕ｩ蛹厄ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ縺帙＞???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?
float			f_TrgtAngle 		= 0;		// [隗貞ｺｦ蛻ｶ蠕｡]   逶ｮ讓呵ｧ貞ｺｦ						???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?
float			f_AngleErrSum 		= 0;		// [隗貞ｺｦ蛻ｶ蠕｡]   隗貞ｺｦ遨搾ｿｽ??蛻ｶ蠕｡縺ｮ繧ｵ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?蛟､			???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?
// wall control
int32_t 			l_WallErr 		= 0;		// [wall control]     wall error		(updated 1[msec])
float			f_ErrDistBuf		= 0;		// [wall control]     wall error buffer	(updated 1[msec])	

//fail safe
float  			f_ErrChkAngle; 			  // 繧ｸ繝｣繧､繝ｭ繧ｻ繝ｳ繧ｵ縺ｮ繧ｨ繝ｩ繝ｼ讀懶ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ逕ｨ縺ｮ隗貞ｺｦ
bool   			bl_ErrChk; 				  // 繧ｸ繝｣繧､繝ｭ繧ｻ繝ｳ繧ｵ縺ｮ繧ｨ繝ｩ繝ｼ讀懶ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?FALSE???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ讀懃衍縺励↑??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?縲ゝRUE???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ讀懃衍縺吶ｋ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
bool			bl_failsafe		= FALSE;	// 繝槭え繧ｹ縺鯉ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ蛻ｶ蠕｡荳搾ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?TRUE???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ蛻ｶ蠕｡荳搾ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ縲：ALSE???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ蛻ｶ蠕｡蜿ｯ閭ｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?


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
	if( Msec_in > 999 ){		// msec ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ? sec
		Msec_in  = 0;
		Sec_in++;
	}
	if( Sec_in > 59 ){			// sec ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ? min
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
	DCM_brakeMot( DCM_R );		// 繝悶Ξ繝ｼ繧ｭ
	DCM_brakeMot( DCM_L );		// 繝悶Ξ繝ｼ繧ｭ
}

void CTRL_clrData( void )
{
//	recv_spi_encoder();								// 繧ｨ繝ｳ繧ｳ繝ｼ繝繝｢繧ｸ繝･繝ｼ繝ｫ蛻晄悄??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
//	ENC_R_CNT_old	= ENC_R_CNT;
//	ENC_L_CNT_old	= ENC_L_CNT;
/*	ENC_setref();
	l_CntR			= 0;						// 繧ｫ繧ｦ繝ｳ繧ｿ繧ｯ繝ｪ繧｢
	l_CntL			= 0;						// 繧ｫ繧ｦ繝ｳ繧ｿ繧ｯ繝ｪ繧｢
*/
	/* 迴ｾ蝨ｨ蛟､ */
/*	f_NowDist 		= 0;						// 遘ｻ蜍戊ｷ晞屬繝ｪ繧ｻ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
	f_NowDistR 		= 0;
	f_NowDistL 		= 0;
	f_NowSpeed		= 0;						// [騾溷ｺｦ蛻ｶ蠕｡]   迴ｾ蝨ｨ縺ｮ騾溷ｺｦ [m/s]			???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?
	f_NowAngle		= 0;						// [隗貞ｺｦ蛻ｶ蠕｡]   迴ｾ蝨ｨ隗貞ｺｦ					???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?
	s_GyroVal		= 0;						// 繧ｸ繝｣繧､繝ｭ蛟､繧ｯ繝ｪ繧｢
	f_GyroNowAngle	= 0;							// 繧ｸ繝｣繧､繝ｭ蛟､繧ｯ繝ｪ繧｢
*/
	/* 逶ｮ讓吝､ */
	f_TrgtSpeed		= 0;						// [騾溷ｺｦ蛻ｶ蠕｡]   逶ｮ讓咏ｧｻ蜍暮溷ｺｦ [m/s]			???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?
	f_TrgtDist 		= 0;						// [霍晞屬蛻ｶ蠕｡]   逶ｮ讓咏ｧｻ蜍戊ｷ晞屬				???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?
	f_TrgtAngleS	= 0;							// [隗帝溷ｺｦ蛻ｶ蠕｡] 逶ｮ讓呵ｧ帝溷ｺｦ [rad/s]			???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?
	f_TrgtAngle		= 0;						// [隗貞ｺｦ蛻ｶ蠕｡]   逶ｮ讓呵ｧ貞ｺｦ					???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?

	/* 蛻ｶ蠕｡??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?繝ｼ繧ｿ */
	f_SpeedErrSum	= 0;
	f_DistErrSum 	= 0;						// [霍晞屬蛻ｶ蠕｡]   霍晞屬遨搾ｿｽ??蛻ｶ蠕｡縺ｮ繧ｵ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?蛟､			???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?
	f_AngleSErrSum	= 0;
	f_AngleErrSum 	= 0;						// [隗貞ｺｦ蛻ｶ蠕｡]   隗貞ｺｦ遨搾ｿｽ??蛻ｶ蠕｡縺ｮ繧ｵ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?蛟､			???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?
	f_ErrSpeedBuf	= 0;
	f_ErrDistBuf	= 0;						// [螢∝宛蠕｡]     霍晞屬繧ｻ繝ｳ繧ｵ繝ｼ繧ｨ繝ｩ繝ｼ蛟､縺ｮ繝舌ャ繝輔ぃ		???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?
	f_ErrAngleSBuf  = 0;
}

void CTRL_clrNowData(void)
{
	ENC_setref();
	l_CntR			= 0;						// 繧ｫ繧ｦ繝ｳ繧ｿ繧ｯ繝ｪ繧｢
	l_CntL			= 0;						// 繧ｫ繧ｦ繝ｳ繧ｿ繧ｯ繝ｪ繧｢

	/* 迴ｾ蝨ｨ蛟､ */
	f_NowDist 		= 0;						// 遘ｻ蜍戊ｷ晞屬繝ｪ繧ｻ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
	f_NowDistR 		= 0;
	f_NowDistL 		= 0;
	f_NowSpeed		= 0;						// [騾溷ｺｦ蛻ｶ蠕｡]   迴ｾ蝨ｨ縺ｮ騾溷ｺｦ [m/s]			???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?
	f_NowAngle		= 0;						// [隗貞ｺｦ蛻ｶ蠕｡]   迴ｾ蝨ｨ隗貞ｺｦ					???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?1[msec]豈弱↓譖ｴ譁ｰ縺輔ｌ繧具ｿｽ?
	s_GyroVal		= 0;						// 繧ｸ繝｣繧､繝ｭ蛟､繧ｯ繝ｪ繧｢
	f_GyroNowAngle	= 0;							// 繧ｸ繝｣繧､繝ｭ蛟､繧ｯ繝ｪ繧｢
}

void CTRL_setData( stCTRL_DATA* p_data )
{
	/* 蛻ｶ蠕｡譁ｹ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ? */
	en_Type					= p_data->en_type;

	/* 騾溷ｺｦ蛻ｶ蠕｡ */
	f_Acc 					= p_data->f_acc;
	f_BaseSpeed				= p_data->f_now;
	f_LastSpeed				= p_data->f_trgt;

	/* 霍晞屬蛻ｶ蠕｡ */
	f_BaseDist 				= p_data->f_nowDist;
	f_LastDist 				= p_data->f_dist;

	/* 隗帝溷ｺｦ蛻ｶ蠕｡ */
	f_AccAngleS 			= p_data->f_accAngleS;
	f_BaseAngleS			= p_data->f_nowAngleS;
	f_LastAngleS			= p_data->f_trgtAngleS;

	/* 隗貞ｺｦ蛻ｶ蠕｡ */
	f_BaseAngle 			= p_data->f_nowAngle;
	f_LastAngle 			= p_data->f_angle;

	f_Time 					= 0;
	f_TrgtTime				= p_data->f_time;

	escape_wait			= 0;

	CTRL_sta();				// 蛻ｶ蠕｡髢具ｿｽ?

}

void CTRL_refNow( void )
{
	float f_speedR		= 0;							// 蜿ｳ繝｢繝ｼ繧ｿ迴ｾ蝨ｨ騾溷ｺｦ [m/s]
	float f_speedL		= 0;							// 蟾ｦ繝｢繝ｼ繧ｿ迴ｾ蝨ｨ騾溷ｺｦ [m/s]
	float f_r 			= F_CNT2MM(l_CntR);				// 蜿ｳ繝｢繝ｼ繧ｿ縺ｮ騾ｲ繧薙□霍晞屬 [m]
	float f_l 			= F_CNT2MM(l_CntL);				// 蟾ｦ繝｢繝ｼ繧ｿ縺ｮ騾ｲ繧薙□霍晞屬 [m]

	/* 騾溷ｺｦ譖ｴ譁ｰ */
	f_speedR = f_r * 1000.0;								// 蜿ｳ繝｢繝ｼ繧ｿ騾溷ｺｦ [m/s] ( 遘ｻ蜍戊ｷ晞屬[繧ｫ繧ｦ繝ｳ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?] * 1繝代Ν繧ｹ縺ｮ遘ｻ蜍暮㍼(0.0509[mm]) * 1000(msec竊痴ec)
	f_speedL = f_l * 1000.0;								// 蟾ｦ繝｢繝ｼ繧ｿ騾溷ｺｦ [m/s] ( 遘ｻ蜍戊ｷ晞屬[繧ｫ繧ｦ繝ｳ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?] * 1繝代Ν繧ｹ縺ｮ遘ｻ蜍暮㍼(0.0509[mm]) * 1000(msec竊痴ec)
	f_NowSpeed  = ( f_speedR + f_speedL ) / 2;			// 繝槭え繧ｹ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ騾ｲ陦梧婿蜷台ｸｭ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?霆ｸ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ? [1m/s]

	/*motor AngleS*/
	f_MotorR_AngleS	= f_speedR /(PI*TIRE_D)/GEAR_RATIO;
	f_MotorL_AngleS = f_speedL /(PI*TIRE_D)/GEAR_RATIO;

	/* 霍晞屬譖ｴ譁ｰ */
	f_NowDistR += f_r;									// 繧ｫ繧ｦ繝ｳ繝域峩譁ｰ
	f_NowDistL += f_l;									// 繧ｫ繧ｦ繝ｳ繝域峩譁ｰ
	f_NowDist  = ( f_NowDistR + f_NowDistL ) / 2;		// 蟷ｳ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?蛟､譖ｴ譁ｰ
}

void CTRL_refTarget( void )
{
	/* mode */
	switch( en_Type ){

		/* acc(straight) */
		case CTRL_ACC:
		case CTRL_SKEW_ACC:
			if( f_TrgtSpeed < (f_LastSpeed -(f_Acc * 0.001)) ){												// ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?騾溽岼讓呎峩譁ｰ蛹ｺ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
				f_TrgtSpeed = f_BaseSpeed + f_Acc * f_Time;									// 逶ｮ讓咎溷ｺｦ
			}
			else{
				f_TrgtSpeed = f_LastSpeed;
			}
			break;

		/* const(straight) */
		case CTRL_CONST:
		case CTRL_SKEW_CONST:
			f_TrgtSpeed = f_BaseSpeed;														// 逶ｮ讓咎溷ｺｦ
			break;

		/* dec(straight) */
		case CTRL_DEC:
		case CTRL_SKEW_DEC:
			/* speed CTRL + position CTRL */
			if( f_TrgtSpeed > (f_LastSpeed +(f_Acc * 0.001))){												// 貂幃溽岼讓呎峩譁ｰ蛹ｺ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
				f_TrgtSpeed = f_BaseSpeed - f_Acc * f_Time;									// 逶ｮ讓咎溷ｺｦ
				f_TrgtDist  = f_BaseDist + ( f_BaseSpeed + f_TrgtSpeed ) * f_Time / 2;		// 逶ｮ讓呵ｷ晞屬
			}
			/* position CTRL */
			else{
				f_TrgtSpeed = f_LastSpeed;
				f_TrgtDist  = f_LastDist;													// 逶ｮ讓呵ｷ晞屬
			}
			break;

		/* acc(Turn) */
		case CTRL_ACC_TRUN:

			/* CCW  hidari*/
			if( f_LastAngle > 0 ){
				if ( f_TrgtAngleS < (f_LastAngleS -(f_AccAngleS * 0.001)) ){
					f_TrgtAngleS = 0.0 + f_AccAngleS * f_Time;									// 逶ｮ讓呵ｧ帝溷ｺｦ
				}
				else{
					f_TrgtAngleS = f_LastAngleS;
				}
			}
			/* CW  migi */
			else{
				if( f_TrgtAngleS > (f_LastAngleS +(f_AccAngleS * 0.001)) ){
				f_TrgtAngleS = 0.0 - f_AccAngleS * f_Time;									// 逶ｮ讓呵ｧ帝溷ｺｦ
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
				if( f_TrgtAngleS > (f_LastAngleS +(f_AccAngleS * 0.001)) ){												// 貂幃溽岼讓呎峩譁ｰ蛹ｺ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
					f_TrgtAngleS = f_BaseAngleS - f_AccAngleS * f_Time;							// 逶ｮ讓呵ｧ帝溷ｺｦ
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// 逶ｮ讓呵ｧ貞ｺｦ
				}
				/* Angle CTRL */
				else{
					f_TrgtAngleS = f_LastAngleS;
					f_TrgtAngle  = f_LastAngle;													// 逶ｮ讓呵ｷ晞屬
				}
			}
			/* CW */
			else{

				/* Angle speed CTRL + Angle CTRL */
				if( f_TrgtAngleS < (f_LastAngleS -(f_AccAngleS * 0.001))){												// 貂幃溽岼讓呎峩譁ｰ蛹ｺ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
					f_TrgtAngleS = f_BaseAngleS + f_AccAngleS * f_Time;							// 逶ｮ讓呵ｧ帝溷ｺｦ
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// 逶ｮ讓呵ｧ貞ｺｦ
				}
				/* Angle CTRL */
				else{
					f_TrgtAngleS = f_LastAngleS;
					f_TrgtAngle  = f_LastAngle;													// 逶ｮ讓呵ｷ晞屬
				}
			}
			break;

		/* entry(sura) */
		case CTRL_ENTRY_SURA:
			f_TrgtSpeed = f_BaseSpeed;
			if( f_TrgtDist <= f_LastDist - (f_TrgtSpeed * 0.001) ){
				f_TrgtDist  = f_BaseDist + f_TrgtSpeed * f_Time;								// 逶ｮ讓呵ｷ晞屬
			}
			break;

		/* acc(繧ｹ繝ｩ繝ｭ繝ｼ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?) */
		case CTRL_ACC_SURA:
			f_TrgtSpeed = f_BaseSpeed;

			/* CCW */
			if( f_LastAngle > 0 ){
				if( f_TrgtAngleS < (f_LastAngleS +(f_AccAngleS * 0.001))){
					f_TrgtAngleS = f_BaseAngleS + f_AccAngleS * f_Time;							// 逶ｮ讓呵ｧ帝溷ｺｦ
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// 逶ｮ讓呵ｧ貞ｺｦ
//					printf("%5.2f %5.2f %5.4f %5.2f %5.2f\n\r",f_TrgtAngleS,f_AccAngleS,f_Time,f_TrgtAngle,f_LastAngleS);
				}
				else{
					f_TrgtAngle  = f_LastAngle;													// 逶ｮ讓呵ｷ晞屬
				}
			}
			/* CW */
			else{
				if( f_TrgtAngleS > (f_LastAngleS -(f_AccAngleS * 0.001)) ){
					f_TrgtAngleS = f_BaseAngleS + f_AccAngleS * f_Time;							// 逶ｮ讓呵ｧ帝溷ｺｦ
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// 逶ｮ讓呵ｧ貞ｺｦ
//					printf("%5.2f %5.2f %5.4f %5.2f %5.2f\n\r",f_TrgtAngleS,f_AccAngleS,f_Time,f_TrgtAngle,f_LastAngleS);
				}
				else{
					f_TrgtAngle  = f_LastAngle;													// 逶ｮ讓呵ｷ晞屬
				}
			}

			/* Position CTRL */
			if( f_LastDist > (f_TrgtDist - (f_TrgtSpeed * 0.001)) ){													// 逶ｮ讓呎峩譁ｰ蛹ｺ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
				f_TrgtDist  = f_BaseDist + f_TrgtSpeed * f_Time;							// 逶ｮ讓吩ｽ咲ｽｮ
			}
			else{
				f_TrgtDist  = f_LastDist;													// 逶ｮ讓呵ｷ晞屬
			}
			break;

		/* const(sura) */
		case CTRL_CONST_SURA:
			f_TrgtSpeed = f_BaseSpeed;
			f_TrgtAngleS = f_BaseAngleS;							// 逶ｮ讓呵ｧ帝溷ｺｦ

			/* CCW */
			if( f_LastAngle > 0 ){
				if( f_TrgtAngle < (f_LastAngle +(f_AccAngleS * 0.001)) ){
					f_TrgtAngle  = f_BaseAngle + f_TrgtAngleS * f_Time;			// 逶ｮ讓呵ｧ貞ｺｦ
				}
				else{
					f_TrgtAngle  = f_LastAngle;									// 逶ｮ讓呵ｧ貞ｺｦ
				}
			}
			/* CW */
			else{
				if( f_TrgtAngle > (f_LastAngle -(f_AccAngleS * 0.001)) ){
					f_TrgtAngle  = f_BaseAngle + f_TrgtAngleS * f_Time;			// 逶ｮ讓呵ｧ貞ｺｦ
				}
				else{
					f_TrgtAngle  = f_LastAngle;									// 逶ｮ讓呵ｧ貞ｺｦ
				}
			}

			/* Position CTRL */
			if( f_LastDist > (f_TrgtDist - (f_TrgtSpeed * 0.001)) ){													// 逶ｮ讓呎峩譁ｰ蛹ｺ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
				f_TrgtDist  = f_BaseDist + f_TrgtSpeed * f_Time;							// 逶ｮ讓吩ｽ咲ｽｮ
			}
			else{
				f_TrgtDist  = f_LastDist;													// 逶ｮ讓呵ｷ晞屬
			}
			break;

		/* dec(sura) */
		case CTRL_DEC_SURA:
			f_TrgtSpeed = f_BaseSpeed;

			/* CCW */
			if( f_LastAngle > 0 ){
				if( f_TrgtAngleS > (f_LastAngle -(f_AccAngleS * 0.001)) ){
					f_TrgtAngleS = f_BaseAngleS + f_AccAngleS * f_Time;							// 逶ｮ讓呵ｧ帝溷ｺｦ
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// 逶ｮ讓呵ｧ貞ｺｦ
				}
				else{
					f_TrgtAngleS = 0.0;
					f_TrgtAngle  = f_LastAngle;													// 逶ｮ讓呵ｷ晞屬
				}
			}
			/*CW*/
			else{
				if( f_TrgtAngleS < (f_LastAngle +(f_AccAngleS * 0.001)) ){
					f_TrgtAngleS = f_BaseAngleS + f_AccAngleS * f_Time;							// 逶ｮ讓呵ｧ帝溷ｺｦ
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// 逶ｮ讓呵ｧ貞ｺｦ
				}
				else{
					f_TrgtAngleS = 0.0;
					f_TrgtAngle  = f_LastAngle;													// 逶ｮ讓呵ｷ晞屬
				}
			}

			/* Position CTRL */
			if( f_LastDist > (f_TrgtDist - (f_TrgtSpeed * 0.001)) ){													// 逶ｮ讓呎峩譁ｰ蛹ｺ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
				f_TrgtDist  = f_BaseDist + f_TrgtSpeed * f_Time;							// 逶ｮ讓吩ｽ咲ｽｮ
			}
			else{
				f_TrgtDist  = f_LastDist;													// 逶ｮ讓呵ｷ晞屬
			}
			break;

		/* escape(sura) */
		case CTRL_EXIT_SURA:
			f_TrgtSpeed = f_BaseSpeed;
			f_TrgtAngleS = 0;
			if( f_TrgtDist <= (f_LastDist -f_TrgtSpeed * 0.001)){
				f_TrgtDist  = f_BaseDist + f_TrgtSpeed * f_Time;								// 逶ｮ讓呵ｷ晞屬
			}
			else{
				f_TrgtDist  = f_LastDist;														// 逶ｮ讓呵ｷ晞屬
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
		case CTRL_ACC:			return PARAM_ACC;				// ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?騾滉ｸｭ(逶ｴ騾ｲ)
		case CTRL_CONST:		return PARAM_CONST;				// 遲蛾滉ｸｭ(逶ｴ騾ｲ)
		case CTRL_DEC:			return PARAM_DEC;				// 貂幃滉ｸｭ(逶ｴ騾ｲ)
		case CTRL_HIT_WALL:		return PARAM_HIT_WALL;			// 螢√≠縺ｦ蛻ｶ蠕｡
//		case DCMC_BACK_ACC:		return PARAM_BACK_ACC;			// ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?騾滉ｸｭ(蠕碁ｲ)
//		case DCMC_BACK_CONST:		return PARAM_BACK_CONST;		// 遲蛾滉ｸｭ(蠕碁ｲ)
//		case DCMC_BACK_DEC:		return PARAM_BACK_DEC;			// 貂幃滉ｸｭ(蠕碁ｲ)
		case CTRL_SKEW_ACC:		return PARAM_SKEW_ACC;			// ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?騾滉ｸｭ(逶ｴ騾ｲ)
		case CTRL_SKEW_CONST:		return PARAM_SKEW_CONST;		// 遲蛾滉ｸｭ(逶ｴ騾ｲ)
		case CTRL_SKEW_DEC:		return PARAM_SKEW_DEC;			// 貂幃滉ｸｭ(逶ｴ騾ｲ)
		case CTRL_ACC_TRUN:		return PARAM_ACC_TRUN;			// ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?騾滉ｸｭ(??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?蝨ｰ菫｡譌句屓)
		case CTRL_CONST_TRUN:		return PARAM_CONST_TRUN;		// 遲蛾滉ｸｭ(??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?蝨ｰ菫｡譌句屓)
		case CTRL_DEC_TRUN:		return PARAM_DEC_TRUN;			// 貂幃滉ｸｭ(??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?蝨ｰ菫｡譌句屓)
		case CTRL_ENTRY_SURA:		return PARAM_ENTRY_SURA;		// 繧ｹ繝ｩ繝ｭ繝ｼ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?蜑搾ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ蜑埼ｲ蜍包ｿｽ?(繧ｹ繝ｩ繝ｭ繝ｼ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?)
		case CTRL_ACC_SURA:		return PARAM_ACC_SURA;			// ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?騾滉ｸｭ(繧ｹ繝ｩ繝ｭ繝ｼ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?)
		case CTRL_CONST_SURA:		return PARAM_CONST_SURA;		// 遲蛾滉ｸｭ(繧ｹ繝ｩ繝ｭ繝ｼ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?)
		case CTRL_DEC_SURA:		return PARAM_DEC_SURA;			// 貂幃滉ｸｭ(繧ｹ繝ｩ繝ｭ繝ｼ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?)
		case CTRL_EXIT_SURA:		return PARAM_EXIT_SURA;			// 繧ｹ繝ｩ繝ｭ繝ｼ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?蠕鯉ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ蜑埼ｲ蜍包ｿｽ?(繧ｹ繝ｩ繝ｭ繝ｼ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?)
		default:			return PARAM_NC;
	}
}

void CTRL_getFF_speed( float* p_err )
{
	/* 蜍穂ｽ懊Δ繝ｼ繝峨↓蠢懊§??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ? */
	switch( en_Type ){

		// ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
		case CTRL_ACC:
		case CTRL_SKEW_ACC:
		case CTRL_ACC_TRUN:
		case CTRL_ACC_SURA:
			*p_err = f_Acc;
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
		case CTRL_HIT_WALL:
			*p_err = f_Acc * (-1);
			break;

		// ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?騾滉ｻ･??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
		default:
			*p_err = 0;
			break;										// 菴輔ｂ縺励↑??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
	}

}

void CTRL_getFF_angle( float* p_err )
{
	/* 蜍穂ｽ懊Δ繝ｼ繝峨↓蠢懊§??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ? */
	switch( en_Type ){

		// ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
		case CTRL_ACC:
		case CTRL_SKEW_ACC:
		case CTRL_ACC_TRUN:
		case CTRL_ACC_SURA:
			*p_err =FABS(f_AccAngleS);
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
			*p_err = FABS(f_AccAngleS) *(-1);
			break;

		// ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?騾滉ｻ･??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
		default:
			*p_err = 0;
			break;										// 菴輔ｂ縺励↑??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
	}

}

void CTRL_getSpeedFB( float* p_err )
{
	float		f_speedErr;					// [騾溷ｺｦ蛻ｶ蠕｡] 騾溷ｺｦ蛛丞ｷｮ
	float		f_kp = 0.0f;
	float		f_ki = 0.0f;
	float		f_kd = 0.0f;
	/* 騾溷ｺｦ蛻ｶ蠕｡ */
	f_speedErr  = f_TrgtSpeed - f_NowSpeed;					// 騾溷ｺｦ蛛丞ｷｮ[m/s]
	f_kp = f_FB_speed_kp;
	f_ki = f_FB_speed_ki;
	f_kd = f_FB_speed_kd;

	/* I謌撰ｿｽ??貍費ｿｽ? */
	f_SpeedErrSum += f_speedErr;// * f_ki;			// I謌撰ｿｽ??譖ｴ譁ｰ
	if( f_SpeedErrSum > 10000.0 ){
		f_SpeedErrSum = 10000.0;			// 荳企剞繝ｪ繝溘ャ繧ｿ繝ｼ
	}

	*p_err = f_speedErr * f_kp + f_SpeedErrSum* f_ki + ( f_speedErr - f_ErrSpeedBuf ) * f_kd;				// PI蛻ｶ蠕｡驥冗ｮ暦ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ

	f_ErrSpeedBuf = f_speedErr;		// 蛛丞ｷｮ繧偵ヰ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?繝輔ぃ繝ｪ繝ｳ繧ｰ
/*	if((f_speedErr>1.5)||(f_speedErr<-1.5)){
		Failsafe_flag();
	}
*/
}

void CTRL_getAngleSpeedFB( float* p_err )
{
	float f_err;					// [蜈･蜉嫋 繧ｸ繝｣繧､繝ｭ繧ｻ繝ｳ繧ｵ繝ｼ繧ｨ繝ｩ繝ｼ蛟､
	float f_kp = 0.0f;				// 豈比ｾ九ご繧､繝ｳ
	float f_ki = 0.0f;
	float f_kd = 0.0f;


	f_err = f_TrgtAngleS - GYRO_getSpeedErr();			// 逶ｮ讓呵ｧ貞ｺｦ - 繧ｸ繝｣繧､繝ｭ繧ｻ繝ｳ繧ｵ[rad/s]
	f_kp = f_FB_angleS_kp;
	f_ki = f_FB_angleS_ki;
	f_kd = f_FB_angleS_kd;

	f_AngleSErrSum += f_err;//*f_ki;

	if(f_AngleSErrSum > 10000.0){
		f_AngleSErrSum = 10000.0;			//荳企剞繝ｪ繝溘ャ繧ｿ繝ｼ
	}
	else if(f_AngleSErrSum <-10000.0){
		f_AngleSErrSum = -10000.0;
	}

	*p_err = f_err * f_kp + f_AngleSErrSum*f_ki + ( f_err - f_ErrAngleSBuf ) * f_kd;		// PID蛻ｶ蠕｡

	f_ErrAngleSBuf = f_err;		// 蛛丞ｷｮ繧偵ヰ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?繝輔ぃ繝ｪ繝ｳ繧ｰ
	if((f_err>14.0)||(f_err<-14.0)){
		if(!(en_Type == CTRL_HIT_WALL)){
			Failsafe_flag();
		}
	}
}

void CTRL_getAngleFB( float* p_err )
{
    float f_err;                    // [蜈･蜉嫋 繧ｸ繝｣繧､繝ｭ繧ｻ繝ｳ繧ｵ繝ｼ繧ｨ繝ｩ繝ｼ蛟､
    float f_kp = 0.0f;              // 豈比ｾ九ご繧､繝ｳ
    float f_ki = 0.0f;

    f_err = f_TrgtAngle - GYRO_getNowAngle();          // 逶ｮ讓呵ｧ貞ｺｦ - 繧ｸ繝｣繧､繝ｭ繧ｻ繝ｳ繧ｵ[rad]
    f_kp = f_FB_angle_kp;
    f_ki = f_FB_angle_ki;
    f_AngleErrSum += f_err;//*f_ki;
    if(f_AngleErrSum > 100.0){
        f_AngleErrSum = 100.0;           //荳企剞繝ｪ繝溘ャ繧ｿ繝ｼ
    }
    else if(f_AngleErrSum <-100.0){
        f_AngleErrSum = -100.0;
    }
    *p_err = f_err * f_kp + f_AngleErrSum*f_ki;        // PID蛻ｶ蠕｡

}


void CTRL_getSenFB( float* p_err )
{
	float f_err 	= 0;
	float f_kp 		= 0.0f;				// 豈比ｾ九ご繧､繝ｳ
	float f_kd 		= 0.0f;				// 蠕ｮ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?繧ｲ繧､繝ｳ
	float gyro		= 0.0f;

	/* 逶ｴ騾ｲ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ? */
	if( ( en_Type == CTRL_ACC ) || ( en_Type == CTRL_CONST ) || ( en_Type == CTRL_DEC )||
			 ( en_Type == CTRL_ENTRY_SURA ) || ( en_Type == CTRL_EXIT_SURA ) ){

		f_kp = f_FB_wall_kp;
		f_kd = f_FB_wall_kd;

		/* 蛛丞ｷｮ蜿厄ｿｽ? */
		DIST_getErr( &l_WallErr );
		f_err = (float)l_WallErr;
//		templog2 = f_err;
		/* PD蛻ｶ蠕｡ */

		f_ErrDistBuf = f_err;		// 蛛丞ｷｮ繧偵ヰ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?繝輔ぃ繝ｪ繝ｳ繧ｰ

		*p_err = f_err * f_kp + ( f_err - f_ErrDistBuf ) * f_kd;		// PD蛻ｶ蠕｡
	}
	else if( ( en_Type == CTRL_SKEW_ACC ) || ( en_Type == CTRL_SKEW_CONST ) || ( en_Type == CTRL_SKEW_DEC ) ){

		DIST_getErrSkew( &l_WallErr );
		f_err = (float)l_WallErr;

//		*p_err = f_err * f_kp + ( f_err - f_ErrDistBuf ) * f_kd;		// PD蛻ｶ蠕｡
		*p_err = f_err;
	}

}

void CTRL_getFloorFriction(float* p_err){
	float tread;
	if(( en_Type == CTRL_ACC_TRUN) || (en_Type == CTRL_CONST_TRUN)||( en_Type == CTRL_DEC_TRUN )){
		tread = TREAD_imagin;
	}else{
		tread = TREAD;
	}
//	*p_err = 0;
	if( ( en_Type == CTRL_ACC_SURA ) || (en_Type == CTRL_CONST_SURA)||( en_Type == CTRL_DEC_SURA ) ){
		if(f_TrgtAngleS<0){
			if(Get_NowAngle() < -0.002)
				*p_err = (-1)*0.38/1000.0 + (-1)*0.43/1000.0+f_TrgtAngleS*FABS(f_TrgtAngleS)*tread/2/PI/2300.0;
	//			*p_err = (-1)*0.35/1000.0 + (-1)*0.45/1000.0+f_TrgtAngleS*tread/2/PI/109.0;
			else
				*p_err = (-1)*0.37/1000.0;
			}
		else if(f_TrgtAngleS>0){
			if(Get_NowAngle() > 0.002)
				*p_err = 0.38/1000.0 + 0.43/1000.0+f_TrgtAngleS*FABS(f_TrgtAngleS)*tread/2/PI/2300.0;
	//			*p_err = 0.35/1000.0 + 0.45/1000.0+f_TrgtAngleS*tread/2/PI/109.0;
			else
				*p_err = 0.37/1000.0;
		}else{
			*p_err = 0;
		}
	}
	else{
		if(f_TrgtAngleS<0){
			if(Get_NowAngle() < -0.002)
				*p_err = (-1)*0.43/1000.0 + (-1)*0.46/1000.0+f_TrgtAngleS*FABS(f_TrgtAngleS)*tread/2/PI/740.0;
	//			*p_err = (-1)*0.35/1000.0 + (-1)*0.45/1000.0+f_TrgtAngleS*tread/2/PI/109.0;
			else
				*p_err = (-1)*0.37/1000.0;
			}
		else if(f_TrgtAngleS>0){
			if(Get_NowAngle() > 0.002)
				*p_err = 0.43/1000.0 + 0.46/1000.0+f_TrgtAngleS*FABS(f_TrgtAngleS)*tread/2/PI/740.0;
	//			*p_err = 0.35/1000.0 + 0.45/1000.0+f_TrgtAngleS*tread/2/PI/109.0;
			else
				*p_err = 0.37/1000.0;
		}else{
			*p_err = 0;
		}
	}

/*	if(*p_err>0.0014)
		*p_err = 0.0014;
	if(*p_err<-0.0014)
		*p_err = -0.0014;
*/
}

void CTRL_outMot( float f_duty10_R, float f_duty10_L )
{
	float	f_temp;			// 險育ｮ礼畑

	/* 髮ｻ蝨ｧ縺ｫ蠢懊§縺ｦPWM蜃ｺ蜉帙ｒ螟画峩縺吶ｋ */
	f_duty10_R = f_duty10_R*1000;
	f_duty10_L = f_duty10_L*1000;

	/* 蜿ｳ繝｢繝ｼ繧ｿ */
	if( 20 < f_duty10_R ){									// 蜑埼ｲ
		DCM_setDirCw( DCM_R );
		DCM_setPwmDuty( DCM_R, (uint16_t)f_duty10_R );
	}
	else if( f_duty10_R < -20 ){							// 蠕碁
		f_temp = f_duty10_R * -1;
		DCM_setDirCcw( DCM_R );
		DCM_setPwmDuty( DCM_R, (uint16_t)f_temp );
	}
	else{
		DCM_brakeMot( DCM_R );								// 繝悶Ξ繝ｼ繧ｭ
	}

	/* 蟾ｦ繝｢繝ｼ繧ｿ */
	if( 20 < f_duty10_L ){									// 蜑埼ｲ
		DCM_setDirCw( DCM_L );
		DCM_setPwmDuty( DCM_L, (uint16_t)f_duty10_L );
	}
	else if( f_duty10_L < -20 ){							// 蠕碁
		f_temp = f_duty10_L * -1;
		DCM_setDirCcw( DCM_L );
		DCM_setPwmDuty( DCM_L, (uint16_t)f_temp );
	}
	else{
		DCM_brakeMot( DCM_L );								// 繝悶Ξ繝ｼ繧ｭ
	}
}

void CTRL_pol( void )
{
	float f_feedFoard_speed		= 0;		// [蛻ｶ蠕｡] 繝輔ぅ繝ｼ繝峨ヵ繧ｩ繝ｯ繝ｼ繝牙宛蠕｡
	float f_feedFoard_angle		= 0;
	float f_speedCtrl			= 0;		// [蛻ｶ蠕｡] 騾溷ｺｦ蛻ｶ蠕｡??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
	float f_angleSpeedCtrl			= 0;		// [蛻ｶ蠕｡] 隗帝溷ｺｦ蛻ｶ蠕｡??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
	float f_angleCtrl			= 0;
	float f_distSenCtrl			= 0;		// [蛻ｶ蠕｡] 霍晞屬繧ｻ繝ｳ繧ｵ繝ｼ蛻ｶ蠕｡??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
	float f_floorfriction		= 0;
	float f_duty10_R;						// [蜃ｺ蜉嫋 蜿ｳ繝｢繝ｼ繧ｿPWM-DUTY豈納0.1%]
	float f_duty10_L;						// [蜃ｺ蜉嫋 蟾ｦ繝｢繝ｼ繧ｿPWM-DUTY豈納0.1%]

	float TR = 0.0;
	float TL = 0.0;
	float Ir = 0.0;
	float Il = 0.0;

	/* 蛻ｶ蠕｡繧定｡後≧縺具ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ繝√ぉ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?繧ｯ */
	if( uc_CtrlFlag != TRUE ){
		 return;		// 蛻ｶ蠕｡辟｡蜉ｹ迥ｶ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
	}
	if(SW_ON == SW_IsOn_0()){
		Failsafe_flag();
	}

	/* 蛻ｶ蠕｡荳搾ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ */
	if (SYS_isOutOfCtrl() == TRUE ){

		f_DistErrSum = 0;				// 邏ｯ遨榊￥蟾ｮ繧ｯ繝ｪ繧｢
		f_NowDist = f_LastDist;			// 蠑ｷ蛻ｶ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?縺ｫ譛邨ら岼讓吩ｽ咲ｽｮ縺ｫ螟画峩
		f_NowAngle = f_LastAngle;		// 蠑ｷ蛻ｶ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?縺ｫ譛邨ら岼讓呎凾髢薙↓螟画峩
		f_Time = f_TrgtTime;			// 蠑ｷ蛻ｶ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?縺ｫ譛邨ら岼讓呎凾髢薙↓螟画峩

	 	CTRL_stop();				// 蛻ｶ蠕｡蛛懈ｭ｢
		CTRL_clrData();					// ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?繝ｼ繧ｿ繧ｯ繝ｪ繧｢
		DCM_brakeMot( DCM_R );			// 繝悶Ξ繝ｼ繧ｭ
		DCM_brakeMot( DCM_L );			// 繝悶Ξ繝ｼ繧ｭ
	}

	/* ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?遞ｮ繧ｻ繝ｳ繧ｵ蜈･??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ? */
	ENC_GetDiv( &l_CntR, &l_CntL );					// 遘ｻ蜍暮㍼[繧ｫ繧ｦ繝ｳ繝亥､]繧貞叙??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
	//add get_motor_omega(l_CntR,l_CntL);
	CTRL_refNow();									// 蛻ｶ蠕｡縺ｫ菴ｿ逕ｨ縺吶ｋ蛟､繧堤樟蝨ｨ縺ｮ迥ｶ諷九↓譖ｴ譁ｰ
	CTRL_refTarget();								// 蛻ｶ蠕｡縺ｫ菴ｿ逕ｨ縺吶ｋ蛟､繧堤岼讓吝､縺ｫ譖ｴ譁ｰ

	f_NowAngle = GYRO_getNowAngle();					// 迴ｾ蝨ｨ隗貞ｺｦ[deg]

	/* 蛻ｶ蠕｡蛟､蜿厄ｿｽ? */
	CTRL_getFF_speed( &f_feedFoard_speed );					// [蛻ｶ蠕｡] 繝輔ぅ繝ｼ繝峨ヵ繧ｩ繝ｯ繝ｼ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
	CTRL_getFF_angle( &f_feedFoard_angle );					// [蛻ｶ蠕｡] 繝輔ぅ繝ｼ繝峨ヵ繧ｩ繝ｯ繝ｼ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
	CTRL_getSpeedFB( &f_speedCtrl );				// [蛻ｶ蠕｡] 騾溷ｺｦ
	CTRL_getAngleSpeedFB( &f_angleSpeedCtrl );			// [蛻ｶ蠕｡] 隗帝溷ｺｦ
	CTRL_getAngleFB( &f_angleCtrl );		//angle ctrl
	CTRL_getSenFB( &f_distSenCtrl );				// [蛻ｶ蠕｡] ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?
	CTRL_getFloorFriction( &f_floorfriction );

	/* 逶ｴ騾ｲ蛻ｶ蠕｡ */
	if( ( en_Type == CTRL_ACC ) || ( en_Type == CTRL_CONST ) || ( en_Type == CTRL_DEC ) ||( en_Type == CTRL_ENTRY_SURA ) || ( en_Type == CTRL_EXIT_SURA ) ||
		( en_Type == CTRL_SKEW_ACC ) || ( en_Type == CTRL_SKEW_CONST ) || ( en_Type == CTRL_SKEW_DEC )
	){
		TR = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))+(TIRE_D/2/TREAD)*(Inertia*(f_feedFoard_angle + f_angleSpeedCtrl+ f_distSenCtrl)))/GEAR_RATIO;
		TL = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))-(TIRE_D/2/TREAD)*(Inertia*(f_feedFoard_angle + f_angleSpeedCtrl+ f_distSenCtrl)))/GEAR_RATIO;
		Ir = (TR+0.0255/1000.0)/Torque_constant;
		Il = (TL+0.0255/1000.0)/Torque_constant;
	}

	/* 螢√≠縺ｦ蛻ｶ蠕｡ */
	else if( en_Type == CTRL_HIT_WALL ){
		TR = (TIRE_D/2/2)*(Weight*(f_feedFoard_speed * FF_HIT_BALANCE_R/2500.0 ));		// 蜿ｳ繝｢繝ｼ繧ｿPWM-DUTY豈納0.1%]
		TL = (TIRE_D/2/2)*(Weight*(f_feedFoard_speed * FF_HIT_BALANCE_R/2500.0 ));
		Ir = (TR-0.0255/1000.0)/Torque_constant;
		Il = (TL-0.0255/1000.0)/Torque_constant;
	}

	/* 繧ｹ繝ｩ繝ｭ繝ｼ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?蛻ｶ蠕｡ */
	else if( ( en_Type == CTRL_ACC_SURA ) || (en_Type == CTRL_CONST_SURA)||( en_Type == CTRL_DEC_SURA ) ){
		/* 蟾ｦ譌句屓 */
		if( f_LastAngle > 0 ){
			TR = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))+(TIRE_D/2/TREAD)*(4.6/1000000.0*(f_feedFoard_angle + f_angleSpeedCtrl+f_angleCtrl)+f_floorfriction))/GEAR_RATIO;
			TL = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))-(TIRE_D/2/TREAD)*(4.6/1000000.0*(f_feedFoard_angle + f_angleSpeedCtrl+f_angleCtrl)+f_floorfriction))/GEAR_RATIO;
			Ir = (TR+0.0255/1000.0)/Torque_constant;
			Il = (TL+0.0255/1000.0)/Torque_constant;
		}
		/*蜿ｳ譌句屓 */
		else{			
			TR = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))+(TIRE_D/2/TREAD)*(4.6/1000000.0*(f_feedFoard_angle*(-1) + f_angleSpeedCtrl+f_angleCtrl)+f_floorfriction))/GEAR_RATIO;
			TL = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))-(TIRE_D/2/TREAD)*(4.6/1000000.0*(f_feedFoard_angle*(-1) + f_angleSpeedCtrl+f_angleCtrl)+f_floorfriction))/GEAR_RATIO;
			Ir = (TR+0.0255/1000.0)/Torque_constant;
			Il = (TL+0.0255/1000.0)/Torque_constant;
		}
	}

	/* ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?菫｡蝨ｰ譌句屓 */
	else{
		/* ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ */
		if( f_LastAngle > 0 ){			
			TR = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))+(TIRE_D/2/TREAD_imagin)*(Inertia*(f_feedFoard_angle + f_angleSpeedCtrl+f_angleCtrl)+f_floorfriction))/GEAR_RATIO;
			TL = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))-(TIRE_D/2/TREAD_imagin)*(Inertia*(f_feedFoard_angle + f_angleSpeedCtrl+f_angleCtrl)+f_floorfriction))/GEAR_RATIO;
			Ir = (TR+0.0255/1000.0)/Torque_constant;
			Il = (TL-0.0255/1000.0)/Torque_constant;
		}
		/* ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽE??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ */
		else{			
			TR = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))+(TIRE_D/2/TREAD_imagin)*(Inertia*(f_feedFoard_angle*(-1) + f_angleSpeedCtrl+f_angleCtrl)+f_floorfriction))/GEAR_RATIO;
			TL = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))-(TIRE_D/2/TREAD_imagin)*(Inertia*(f_feedFoard_angle*(-1) + f_angleSpeedCtrl+f_angleCtrl)+f_floorfriction))/GEAR_RATIO;
			Ir = (TR-0.0255/1000.0)/Torque_constant;
			Il = (TL+0.0255/1000.0)/Torque_constant;
		}
	}
	f_duty10_R = FF_BALANCE_R*(Motor_Register*Ir+f_MotorR_AngleS*0.001033/1000.0/2.0/PI)/get_battLv();	
	f_duty10_L = FF_BALANCE_L*(Motor_Register*Il+f_MotorL_AngleS*0.001033/1000.0/2.0/PI)/get_battLv();	

	templog1 = f_floorfriction;
	templog2 = f_duty10_R;

	escape_wait = escape_wait+0.001;
	CTRL_outMot( f_duty10_R, f_duty10_L );				// 繝｢繝ｼ繧ｿ縺ｸ蜃ｺ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?

	f_Time += 0.001;

	/* ?�ｿｽ�ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??繧後メ繧ｧ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?繧ｯ */
	if( MOT_getWallEdgeType() == MOT_WALL_EDGE_RIGHT ){

		/* 螢∵栢??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ? */
		if( DIST_isWall_R_SIDE() == FALSE ){
			MOT_setWallEdge( TRUE );		// ?�ｿｽ�ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?繧檎岼繧呈､懃衍
		}
	}
	else if( MOT_getWallEdgeType() == MOT_WALL_EDGE_LEFT ){

		/* 螢∵栢??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ? */
		if( DIST_isWall_L_SIDE() == FALSE ){
			MOT_setWallEdge( TRUE );		// ?�ｿｽ�ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ???�ｿｽ�ｿｽ?�ｿｽ�ｿｽ??�ｿｽ�ｿｽ?�ｿｽ�ｿｽ?繧檎岼繧呈､懃衍
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
	return bl_failsafe;
}


