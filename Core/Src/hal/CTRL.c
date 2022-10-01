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
float			f_ErrSpeedBuf		= 0;		// [velocity control] ã€€velocity error buffer	(updated 1[msec])
float			f_SpeedErrSum 		= 0;		// [velocity control]   sum of velocity integral control	(updated 1[msec])

float			f_MotorR_AngleS = 0;
float			f_MotorL_AngleS = 0;

// dist
float			f_BaseDist		= 0;		// [dist]   initial distance[m]		(set value)
float			f_LastDist 		= 0;		// [è·é›¢åˆ¶å¾¡]   æœ€çµ‚ç§»å‹•è·é›¢					????¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½è¨­å®šå€¤????¿½?¿½??¿½?¿½?
float			f_TrgtDist 		= 0;		// [è·é›¢åˆ¶å¾¡]   ç›®æ¨™ç§»å‹•è·é›¢					????¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?
volatile float 		f_NowDist		= 0;		// [è·é›¢åˆ¶å¾¡]   ç¾åœ¨è·é›¢						????¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?
float			f_NowDistR		= 0;		// [è·é›¢åˆ¶å¾¡]   ç¾åœ¨è·é›¢????¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½å³????¿½?¿½??¿½?¿½?					????¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?
float 			f_NowDistL		= 0;		// [è·é›¢åˆ¶å¾¡]   ç¾åœ¨è·é›¢????¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½å·¦????¿½?¿½??¿½?¿½?					????¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?
float			f_DistErrSum 		= 0;		// [è·é›¢åˆ¶å¾¡]   è·é›¢ç©ï¿½??åˆ¶å¾¡ã®ã‚µ???¿½?¿½??¿½?¿½?å€¤			????¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?
// angular velocity control
float 			f_AccAngleS		= 0;		// [angle velocity control] angular acceleration[rad/s2]	(set value)
float			f_BaseAngleS		= 0;		// [angle velocity control] initial angular velocity[rad/s]		(set value)
float			f_LastAngleS 		= 0;		// [angle velocity control] last target angular velocity	(set value)
float			f_TrgtAngleS 		= 0;		// [angle velocity control] target angular velocity [rad/s]	(updated 1[msec])
float			f_ErrAngleSBuf		= 0;		// [angle velocity control] angular velocity buffer	(updated 1[msec])
float			f_AngleSErrSum 		= 0;		// [angle velocity control] sum of angular velocity integral contorol	(updated 1[msec])
// angle
float			f_BaseAngle		= 0;		// [è§’åº¦åˆ¶å¾¡]   åˆæœŸè§’åº¦						????¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½è¨­å®šå€¤????¿½?¿½??¿½?¿½?
float			f_LastAngle 		= 0;		// [è§’åº¦åˆ¶å¾¡]   æœ€çµ‚ç›®æ¨™è§’åº¦					????¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½è¨­å®šå€¤????¿½?¿½??¿½?¿½?
volatile float 		f_NowAngle		= 0;		// [è§’åº¦åˆ¶å¾¡]   ç¾åœ¨è§’åº¦ã€€	volatileã‚’ã¤ã‘ãª???¿½?¿½??¿½?¿½?ã¨whileã‹ã‚‰æŠœã‘ã‚‰ã‚Œãªããªã‚‹ï¼ˆæœ€é©åŒ–ï¿½????¿½?¿½??¿½?¿½ã›ã„????¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?
float			f_TrgtAngle 		= 0;		// [è§’åº¦åˆ¶å¾¡]   ç›®æ¨™è§’åº¦						????¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?
float			f_AngleErrSum 		= 0;		// [è§’åº¦åˆ¶å¾¡]   è§’åº¦ç©ï¿½??åˆ¶å¾¡ã®ã‚µ???¿½?¿½??¿½?¿½?å€¤			????¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?
// wall control
int32_t 			l_WallErr 		= 0;		// [wall control]     wall error		(updated 1[msec])
float			f_ErrDistBuf		= 0;		// [wall control]     wall error buffer	(updated 1[msec])	

//fail safe
float  			f_ErrChkAngle; 			  // ã‚¸ãƒ£ã‚¤ãƒ­ã‚»ãƒ³ã‚µã®ã‚¨ãƒ©ãƒ¼æ¤œï¿½????¿½?¿½??¿½?¿½ç”¨ã®è§’åº¦
bool   			bl_ErrChk; 				  // ã‚¸ãƒ£ã‚¤ãƒ­ã‚»ãƒ³ã‚µã®ã‚¨ãƒ©ãƒ¼æ¤œï¿½????¿½?¿½??¿½?¿½????¿½?¿½??¿½?¿½?FALSE????¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½æ¤œçŸ¥ã—ãª???¿½?¿½??¿½?¿½?ã€TRUE????¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½æ¤œçŸ¥ã™ã‚‹????¿½?¿½??¿½?¿½?
bool			bl_failsafe		= FALSE;	// ãƒã‚¦ã‚¹ãŒï¿½????¿½?¿½??¿½?¿½åˆ¶å¾¡ä¸ï¿½????¿½?¿½??¿½?¿½????¿½?¿½??¿½?¿½?TRUE????¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½åˆ¶å¾¡ä¸ï¿½????¿½?¿½??¿½?¿½ã€FALSE????¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½åˆ¶å¾¡å¯èƒ½????¿½?¿½??¿½?¿½?


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
	if( Msec_in > 999 ){		// msec ???¿½?¿½??¿½?¿½? sec
		Msec_in  = 0;
		Sec_in++;
	}
	if( Sec_in > 59 ){			// sec ???¿½?¿½??¿½?¿½? min
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
	DCM_brakeMot( DCM_R );		// ãƒ–ãƒ¬ãƒ¼ã‚­
	DCM_brakeMot( DCM_L );		// ãƒ–ãƒ¬ãƒ¼ã‚­
}

void CTRL_clrData( void )
{
//	recv_spi_encoder();								// ã‚¨ãƒ³ã‚³ãƒ¼ãƒ€ãƒ¢ã‚¸ãƒ¥ãƒ¼ãƒ«åˆæœŸ???¿½?¿½??¿½?¿½?
//	ENC_R_CNT_old	= ENC_R_CNT;
//	ENC_L_CNT_old	= ENC_L_CNT;
/*	ENC_setref();
	l_CntR			= 0;						// ã‚«ã‚¦ãƒ³ã‚¿ã‚¯ãƒªã‚¢
	l_CntL			= 0;						// ã‚«ã‚¦ãƒ³ã‚¿ã‚¯ãƒªã‚¢
*/
	/* ç¾åœ¨å€¤ */
/*	f_NowDist 		= 0;						// ç§»å‹•è·é›¢ãƒªã‚»???¿½?¿½??¿½?¿½????¿½?¿½??¿½?¿½?
	f_NowDistR 		= 0;
	f_NowDistL 		= 0;
	f_NowSpeed		= 0;						// [é€Ÿåº¦åˆ¶å¾¡]   ç¾åœ¨ã®é€Ÿåº¦ [m/s]			????¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?
	f_NowAngle		= 0;						// [è§’åº¦åˆ¶å¾¡]   ç¾åœ¨è§’åº¦					????¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?
	s_GyroVal		= 0;						// ã‚¸ãƒ£ã‚¤ãƒ­å€¤ã‚¯ãƒªã‚¢
	f_GyroNowAngle	= 0;							// ã‚¸ãƒ£ã‚¤ãƒ­å€¤ã‚¯ãƒªã‚¢
*/
	/* ç›®æ¨™å€¤ */
	f_TrgtSpeed		= 0;						// [é€Ÿåº¦åˆ¶å¾¡]   ç›®æ¨™ç§»å‹•é€Ÿåº¦ [m/s]			????¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?
	f_TrgtDist 		= 0;						// [è·é›¢åˆ¶å¾¡]   ç›®æ¨™ç§»å‹•è·é›¢				????¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?
	f_TrgtAngleS	= 0;							// [è§’é€Ÿåº¦åˆ¶å¾¡] ç›®æ¨™è§’é€Ÿåº¦ [rad/s]			????¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?
	f_TrgtAngle		= 0;						// [è§’åº¦åˆ¶å¾¡]   ç›®æ¨™è§’åº¦					????¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?

	/* åˆ¶å¾¡???¿½?¿½??¿½?¿½?ãƒ¼ã‚¿ */
	f_SpeedErrSum	= 0;
	f_DistErrSum 	= 0;						// [è·é›¢åˆ¶å¾¡]   è·é›¢ç©ï¿½??åˆ¶å¾¡ã®ã‚µ???¿½?¿½??¿½?¿½?å€¤			????¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?
	f_AngleSErrSum	= 0;
	f_AngleErrSum 	= 0;						// [è§’åº¦åˆ¶å¾¡]   è§’åº¦ç©ï¿½??åˆ¶å¾¡ã®ã‚µ???¿½?¿½??¿½?¿½?å€¤			????¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?
	f_ErrSpeedBuf	= 0;
	f_ErrDistBuf	= 0;						// [å£åˆ¶å¾¡]     è·é›¢ã‚»ãƒ³ã‚µãƒ¼ã‚¨ãƒ©ãƒ¼å€¤ã®ãƒãƒƒãƒ•ã‚¡		????¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?
	f_ErrAngleSBuf  = 0;
}

void CTRL_clrNowData(void)
{
	ENC_setref();
	l_CntR			= 0;						// ã‚«ã‚¦ãƒ³ã‚¿ã‚¯ãƒªã‚¢
	l_CntL			= 0;						// ã‚«ã‚¦ãƒ³ã‚¿ã‚¯ãƒªã‚¢

	/* ç¾åœ¨å€¤ */
	f_NowDist 		= 0;						// ç§»å‹•è·é›¢ãƒªã‚»???¿½?¿½??¿½?¿½????¿½?¿½??¿½?¿½?
	f_NowDistR 		= 0;
	f_NowDistL 		= 0;
	f_NowSpeed		= 0;						// [é€Ÿåº¦åˆ¶å¾¡]   ç¾åœ¨ã®é€Ÿåº¦ [m/s]			????¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?
	f_NowAngle		= 0;						// [è§’åº¦åˆ¶å¾¡]   ç¾åœ¨è§’åº¦					????¿½?¿½??¿½?¿½?1[msec]æ¯ã«æ›´æ–°ã•ã‚Œã‚‹ï¿½?
	s_GyroVal		= 0;						// ã‚¸ãƒ£ã‚¤ãƒ­å€¤ã‚¯ãƒªã‚¢
	f_GyroNowAngle	= 0;							// ã‚¸ãƒ£ã‚¤ãƒ­å€¤ã‚¯ãƒªã‚¢
}

void CTRL_setData( stCTRL_DATA* p_data )
{
	/* åˆ¶å¾¡æ–¹???¿½?¿½??¿½?¿½? */
	en_Type					= p_data->en_type;

	/* é€Ÿåº¦åˆ¶å¾¡ */
	f_Acc 					= p_data->f_acc;
	f_BaseSpeed				= p_data->f_now;
	f_LastSpeed				= p_data->f_trgt;

	/* è·é›¢åˆ¶å¾¡ */
	f_BaseDist 				= p_data->f_nowDist;
	f_LastDist 				= p_data->f_dist;

	/* è§’é€Ÿåº¦åˆ¶å¾¡ */
	f_AccAngleS 			= p_data->f_accAngleS;
	f_BaseAngleS			= p_data->f_nowAngleS;
	f_LastAngleS			= p_data->f_trgtAngleS;

	/* è§’åº¦åˆ¶å¾¡ */
	f_BaseAngle 			= p_data->f_nowAngle;
	f_LastAngle 			= p_data->f_angle;

	f_Time 					= 0;
	f_TrgtTime				= p_data->f_time;

	escape_wait			= 0;

	CTRL_sta();				// åˆ¶å¾¡é–‹ï¿½?

}

void CTRL_refNow( void )
{
	float f_speedR		= 0;							// å³ãƒ¢ãƒ¼ã‚¿ç¾åœ¨é€Ÿåº¦ [m/s]
	float f_speedL		= 0;							// å·¦ãƒ¢ãƒ¼ã‚¿ç¾åœ¨é€Ÿåº¦ [m/s]
	float f_r 			= F_CNT2MM(l_CntR);				// å³ãƒ¢ãƒ¼ã‚¿ã®é€²ã‚“ã è·é›¢ [m]
	float f_l 			= F_CNT2MM(l_CntL);				// å·¦ãƒ¢ãƒ¼ã‚¿ã®é€²ã‚“ã è·é›¢ [m]

	/* é€Ÿåº¦æ›´æ–° */
	f_speedR = f_r * 1000.0;								// å³ãƒ¢ãƒ¼ã‚¿é€Ÿåº¦ [m/s] ( ç§»å‹•è·é›¢[ã‚«ã‚¦ãƒ³???¿½?¿½??¿½?¿½?] * 1ãƒ‘ãƒ«ã‚¹ã®ç§»å‹•é‡(0.0509[mm]) * 1000(msecâ†’sec)
	f_speedL = f_l * 1000.0;								// å·¦ãƒ¢ãƒ¼ã‚¿é€Ÿåº¦ [m/s] ( ç§»å‹•è·é›¢[ã‚«ã‚¦ãƒ³???¿½?¿½??¿½?¿½?] * 1ãƒ‘ãƒ«ã‚¹ã®ç§»å‹•é‡(0.0509[mm]) * 1000(msecâ†’sec)
	f_NowSpeed  = ( f_speedR + f_speedL ) / 2;			// ãƒã‚¦ã‚¹????¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½é€²è¡Œæ–¹å‘ä¸­???¿½?¿½??¿½?¿½?è»¸????¿½?¿½??¿½?¿½? [1m/s]

	/*motor AngleS*/
	f_MotorR_AngleS	= f_speedR /(PI*TIRE_D)/GEAR_RATIO;
	f_MotorL_AngleS = f_speedL /(PI*TIRE_D)/GEAR_RATIO;

	/* è·é›¢æ›´æ–° */
	f_NowDistR += f_r;									// ã‚«ã‚¦ãƒ³ãƒˆæ›´æ–°
	f_NowDistL += f_l;									// ã‚«ã‚¦ãƒ³ãƒˆæ›´æ–°
	f_NowDist  = ( f_NowDistR + f_NowDistL ) / 2;		// å¹³???¿½?¿½??¿½?¿½?å€¤æ›´æ–°
}

void CTRL_refTarget( void )
{
	/* mode */
	switch( en_Type ){

		/* acc(straight) */
		case CTRL_ACC:
		case CTRL_SKEW_ACC:
			if( f_TrgtSpeed < (f_LastSpeed -(f_Acc * 0.001)) ){												// ???¿½?¿½??¿½?¿½?é€Ÿç›®æ¨™æ›´æ–°åŒº???¿½?¿½??¿½?¿½?
				f_TrgtSpeed = f_BaseSpeed + f_Acc * f_Time;									// ç›®æ¨™é€Ÿåº¦
			}
			else{
				f_TrgtSpeed = f_LastSpeed;
			}
			break;

		/* const(straight) */
		case CTRL_CONST:
		case CTRL_SKEW_CONST:
			f_TrgtSpeed = f_BaseSpeed;														// ç›®æ¨™é€Ÿåº¦
			break;

		/* dec(straight) */
		case CTRL_DEC:
		case CTRL_SKEW_DEC:
			/* speed CTRL + position CTRL */
			if( f_TrgtSpeed > (f_LastSpeed +(f_Acc * 0.001))){												// æ¸›é€Ÿç›®æ¨™æ›´æ–°åŒº???¿½?¿½??¿½?¿½?
				f_TrgtSpeed = f_BaseSpeed - f_Acc * f_Time;									// ç›®æ¨™é€Ÿåº¦
				f_TrgtDist  = f_BaseDist + ( f_BaseSpeed + f_TrgtSpeed ) * f_Time / 2;		// ç›®æ¨™è·é›¢
			}
			/* position CTRL */
			else{
				f_TrgtSpeed = f_LastSpeed;
				f_TrgtDist  = f_LastDist;													// ç›®æ¨™è·é›¢
			}
			break;

		/* acc(Turn) */
		case CTRL_ACC_TRUN:

			/* CCW  hidari*/
			if( f_LastAngle > 0 ){
				if ( f_TrgtAngleS < (f_LastAngleS -(f_AccAngleS * 0.001)) ){
					f_TrgtAngleS = 0.0 + f_AccAngleS * f_Time;									// ç›®æ¨™è§’é€Ÿåº¦
				}
				else{
					f_TrgtAngleS = f_LastAngleS;
				}
			}
			/* CW  migi */
			else{
				if( f_TrgtAngleS > (f_LastAngleS +(f_AccAngleS * 0.001)) ){
				f_TrgtAngleS = 0.0 - f_AccAngleS * f_Time;									// ç›®æ¨™è§’é€Ÿåº¦
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
				if( f_TrgtAngleS > (f_LastAngleS +(f_AccAngleS * 0.001)) ){												// æ¸›é€Ÿç›®æ¨™æ›´æ–°åŒº???¿½?¿½??¿½?¿½?
					f_TrgtAngleS = f_BaseAngleS - f_AccAngleS * f_Time;							// ç›®æ¨™è§’é€Ÿåº¦
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// ç›®æ¨™è§’åº¦
				}
				/* Angle CTRL */
				else{
					f_TrgtAngleS = f_LastAngleS;
					f_TrgtAngle  = f_LastAngle;													// ç›®æ¨™è·é›¢
				}
			}
			/* CW */
			else{

				/* Angle speed CTRL + Angle CTRL */
				if( f_TrgtAngleS < (f_LastAngleS -(f_AccAngleS * 0.001))){												// æ¸›é€Ÿç›®æ¨™æ›´æ–°åŒº???¿½?¿½??¿½?¿½?
					f_TrgtAngleS = f_BaseAngleS + f_AccAngleS * f_Time;							// ç›®æ¨™è§’é€Ÿåº¦
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// ç›®æ¨™è§’åº¦
				}
				/* Angle CTRL */
				else{
					f_TrgtAngleS = f_LastAngleS;
					f_TrgtAngle  = f_LastAngle;													// ç›®æ¨™è·é›¢
				}
			}
			break;

		/* entry(sura) */
		case CTRL_ENTRY_SURA:
			f_TrgtSpeed = f_BaseSpeed;
			if( f_TrgtDist <= f_LastDist - (f_TrgtSpeed * 0.001) ){
				f_TrgtDist  = f_BaseDist + f_TrgtSpeed * f_Time;								// ç›®æ¨™è·é›¢
			}
			break;

		/* acc(ã‚¹ãƒ©ãƒ­ãƒ¼???¿½?¿½??¿½?¿½?) */
		case CTRL_ACC_SURA:
			f_TrgtSpeed = f_BaseSpeed;

			/* CCW */
			if( f_LastAngle > 0 ){
				if( f_TrgtAngleS < (f_LastAngleS +(f_AccAngleS * 0.001))){
					f_TrgtAngleS = f_BaseAngleS + f_AccAngleS * f_Time;							// ç›®æ¨™è§’é€Ÿåº¦
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// ç›®æ¨™è§’åº¦
//					printf("%5.2f %5.2f %5.4f %5.2f %5.2f\n\r",f_TrgtAngleS,f_AccAngleS,f_Time,f_TrgtAngle,f_LastAngleS);
				}
				else{
					f_TrgtAngle  = f_LastAngle;													// ç›®æ¨™è·é›¢
				}
			}
			/* CW */
			else{
				if( f_TrgtAngleS > (f_LastAngleS -(f_AccAngleS * 0.001)) ){
					f_TrgtAngleS = f_BaseAngleS + f_AccAngleS * f_Time;							// ç›®æ¨™è§’é€Ÿåº¦
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// ç›®æ¨™è§’åº¦
//					printf("%5.2f %5.2f %5.4f %5.2f %5.2f\n\r",f_TrgtAngleS,f_AccAngleS,f_Time,f_TrgtAngle,f_LastAngleS);
				}
				else{
					f_TrgtAngle  = f_LastAngle;													// ç›®æ¨™è·é›¢
				}
			}

			/* Position CTRL */
			if( f_LastDist > (f_TrgtDist - (f_TrgtSpeed * 0.001)) ){													// ç›®æ¨™æ›´æ–°åŒº???¿½?¿½??¿½?¿½?
				f_TrgtDist  = f_BaseDist + f_TrgtSpeed * f_Time;							// ç›®æ¨™ä½ç½®
			}
			else{
				f_TrgtDist  = f_LastDist;													// ç›®æ¨™è·é›¢
			}
			break;

		/* const(sura) */
		case CTRL_CONST_SURA:
			f_TrgtSpeed = f_BaseSpeed;
			f_TrgtAngleS = f_BaseAngleS;							// ç›®æ¨™è§’é€Ÿåº¦

			/* CCW */
			if( f_LastAngle > 0 ){
				if( f_TrgtAngle < (f_LastAngle +(f_AccAngleS * 0.001)) ){
					f_TrgtAngle  = f_BaseAngle + f_TrgtAngleS * f_Time;			// ç›®æ¨™è§’åº¦
				}
				else{
					f_TrgtAngle  = f_LastAngle;									// ç›®æ¨™è§’åº¦
				}
			}
			/* CW */
			else{
				if( f_TrgtAngle > (f_LastAngle -(f_AccAngleS * 0.001)) ){
					f_TrgtAngle  = f_BaseAngle + f_TrgtAngleS * f_Time;			// ç›®æ¨™è§’åº¦
				}
				else{
					f_TrgtAngle  = f_LastAngle;									// ç›®æ¨™è§’åº¦
				}
			}

			/* Position CTRL */
			if( f_LastDist > (f_TrgtDist - (f_TrgtSpeed * 0.001)) ){													// ç›®æ¨™æ›´æ–°åŒº???¿½?¿½??¿½?¿½?
				f_TrgtDist  = f_BaseDist + f_TrgtSpeed * f_Time;							// ç›®æ¨™ä½ç½®
			}
			else{
				f_TrgtDist  = f_LastDist;													// ç›®æ¨™è·é›¢
			}
			break;

		/* dec(sura) */
		case CTRL_DEC_SURA:
			f_TrgtSpeed = f_BaseSpeed;

			/* CCW */
			if( f_LastAngle > 0 ){
				if( f_TrgtAngleS > (f_LastAngle -(f_AccAngleS * 0.001)) ){
					f_TrgtAngleS = f_BaseAngleS + f_AccAngleS * f_Time;							// ç›®æ¨™è§’é€Ÿåº¦
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// ç›®æ¨™è§’åº¦
				}
				else{
					f_TrgtAngleS = 0.0;
					f_TrgtAngle  = f_LastAngle;													// ç›®æ¨™è·é›¢
				}
			}
			/*CW*/
			else{
				if( f_TrgtAngleS < (f_LastAngle +(f_AccAngleS * 0.001)) ){
					f_TrgtAngleS = f_BaseAngleS + f_AccAngleS * f_Time;							// ç›®æ¨™è§’é€Ÿåº¦
					f_TrgtAngle  = f_BaseAngle + ( f_BaseAngleS + f_TrgtAngleS ) * f_Time / 2;	// ç›®æ¨™è§’åº¦
				}
				else{
					f_TrgtAngleS = 0.0;
					f_TrgtAngle  = f_LastAngle;													// ç›®æ¨™è·é›¢
				}
			}

			/* Position CTRL */
			if( f_LastDist > (f_TrgtDist - (f_TrgtSpeed * 0.001)) ){													// ç›®æ¨™æ›´æ–°åŒº???¿½?¿½??¿½?¿½?
				f_TrgtDist  = f_BaseDist + f_TrgtSpeed * f_Time;							// ç›®æ¨™ä½ç½®
			}
			else{
				f_TrgtDist  = f_LastDist;													// ç›®æ¨™è·é›¢
			}
			break;

		/* escape(sura) */
		case CTRL_EXIT_SURA:
			f_TrgtSpeed = f_BaseSpeed;
			f_TrgtAngleS = 0;
			if( f_TrgtDist <= (f_LastDist -f_TrgtSpeed * 0.001)){
				f_TrgtDist  = f_BaseDist + f_TrgtSpeed * f_Time;								// ç›®æ¨™è·é›¢
			}
			else{
				f_TrgtDist  = f_LastDist;														// ç›®æ¨™è·é›¢
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
		case CTRL_ACC:			return PARAM_ACC;				// ???¿½?¿½??¿½?¿½?é€Ÿä¸­(ç›´é€²)
		case CTRL_CONST:		return PARAM_CONST;				// ç­‰é€Ÿä¸­(ç›´é€²)
		case CTRL_DEC:			return PARAM_DEC;				// æ¸›é€Ÿä¸­(ç›´é€²)
		case CTRL_HIT_WALL:		return PARAM_HIT_WALL;			// å£ã‚ã¦åˆ¶å¾¡
//		case DCMC_BACK_ACC:		return PARAM_BACK_ACC;			// ???¿½?¿½??¿½?¿½?é€Ÿä¸­(å¾Œé€²)
//		case DCMC_BACK_CONST:		return PARAM_BACK_CONST;		// ç­‰é€Ÿä¸­(å¾Œé€²)
//		case DCMC_BACK_DEC:		return PARAM_BACK_DEC;			// æ¸›é€Ÿä¸­(å¾Œé€²)
		case CTRL_SKEW_ACC:		return PARAM_SKEW_ACC;			// ???¿½?¿½??¿½?¿½?é€Ÿä¸­(ç›´é€²)
		case CTRL_SKEW_CONST:		return PARAM_SKEW_CONST;		// ç­‰é€Ÿä¸­(ç›´é€²)
		case CTRL_SKEW_DEC:		return PARAM_SKEW_DEC;			// æ¸›é€Ÿä¸­(ç›´é€²)
		case CTRL_ACC_TRUN:		return PARAM_ACC_TRUN;			// ???¿½?¿½??¿½?¿½?é€Ÿä¸­(???¿½?¿½??¿½?¿½?åœ°ä¿¡æ—‹å›)
		case CTRL_CONST_TRUN:		return PARAM_CONST_TRUN;		// ç­‰é€Ÿä¸­(???¿½?¿½??¿½?¿½?åœ°ä¿¡æ—‹å›)
		case CTRL_DEC_TRUN:		return PARAM_DEC_TRUN;			// æ¸›é€Ÿä¸­(???¿½?¿½??¿½?¿½?åœ°ä¿¡æ—‹å›)
		case CTRL_ENTRY_SURA:		return PARAM_ENTRY_SURA;		// ã‚¹ãƒ©ãƒ­ãƒ¼???¿½?¿½??¿½?¿½?å‰ï¿½????¿½?¿½??¿½?¿½å‰é€²å‹•ï¿½?(ã‚¹ãƒ©ãƒ­ãƒ¼???¿½?¿½??¿½?¿½?)
		case CTRL_ACC_SURA:		return PARAM_ACC_SURA;			// ???¿½?¿½??¿½?¿½?é€Ÿä¸­(ã‚¹ãƒ©ãƒ­ãƒ¼???¿½?¿½??¿½?¿½?)
		case CTRL_CONST_SURA:		return PARAM_CONST_SURA;		// ç­‰é€Ÿä¸­(ã‚¹ãƒ©ãƒ­ãƒ¼???¿½?¿½??¿½?¿½?)
		case CTRL_DEC_SURA:		return PARAM_DEC_SURA;			// æ¸›é€Ÿä¸­(ã‚¹ãƒ©ãƒ­ãƒ¼???¿½?¿½??¿½?¿½?)
		case CTRL_EXIT_SURA:		return PARAM_EXIT_SURA;			// ã‚¹ãƒ©ãƒ­ãƒ¼???¿½?¿½??¿½?¿½?å¾Œï¿½????¿½?¿½??¿½?¿½å‰é€²å‹•ï¿½?(ã‚¹ãƒ©ãƒ­ãƒ¼???¿½?¿½??¿½?¿½?)
		default:			return PARAM_NC;
	}
}

void CTRL_getFF_speed( float* p_err )
{
	/* å‹•ä½œãƒ¢ãƒ¼ãƒ‰ã«å¿œã˜???¿½?¿½??¿½?¿½? */
	switch( en_Type ){

		// ???¿½?¿½??¿½?¿½????¿½?¿½??¿½?¿½?
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

		// ???¿½?¿½??¿½?¿½?é€Ÿä»¥???¿½?¿½??¿½?¿½?
		default:
			*p_err = 0;
			break;										// ä½•ã‚‚ã—ãª???¿½?¿½??¿½?¿½?
	}

}

void CTRL_getFF_angle( float* p_err )
{
	/* å‹•ä½œãƒ¢ãƒ¼ãƒ‰ã«å¿œã˜???¿½?¿½??¿½?¿½? */
	switch( en_Type ){

		// ???¿½?¿½??¿½?¿½????¿½?¿½??¿½?¿½?
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

		// ???¿½?¿½??¿½?¿½?é€Ÿä»¥???¿½?¿½??¿½?¿½?
		default:
			*p_err = 0;
			break;										// ä½•ã‚‚ã—ãª???¿½?¿½??¿½?¿½?
	}

}

void CTRL_getSpeedFB( float* p_err )
{
	float		f_speedErr;					// [é€Ÿåº¦åˆ¶å¾¡] é€Ÿåº¦åå·®
	float		f_kp = 0.0f;
	float		f_ki = 0.0f;
	float		f_kd = 0.0f;
	/* é€Ÿåº¦åˆ¶å¾¡ */
	f_speedErr  = f_TrgtSpeed - f_NowSpeed;					// é€Ÿåº¦åå·®[m/s]
	f_kp = f_FB_speed_kp;
	f_ki = f_FB_speed_ki;
	f_kd = f_FB_speed_kd;

	/* Iæˆï¿½??æ¼”ï¿½? */
	f_SpeedErrSum += f_speedErr;// * f_ki;			// Iæˆï¿½??æ›´æ–°
	if( f_SpeedErrSum > 10000.0 ){
		f_SpeedErrSum = 10000.0;			// ä¸Šé™ãƒªãƒŸãƒƒã‚¿ãƒ¼
	}

	*p_err = f_speedErr * f_kp + f_SpeedErrSum* f_ki + ( f_speedErr - f_ErrSpeedBuf ) * f_kd;				// PIåˆ¶å¾¡é‡ç®—ï¿½????¿½?¿½??¿½?¿½

	f_ErrSpeedBuf = f_speedErr;		// åå·®ã‚’ãƒ???¿½?¿½??¿½?¿½?ãƒ•ã‚¡ãƒªãƒ³ã‚°
/*	if((f_speedErr>1.5)||(f_speedErr<-1.5)){
		Failsafe_flag();
	}
*/
}

void CTRL_getAngleSpeedFB( float* p_err )
{
	float f_err;					// [å…¥åŠ›] ã‚¸ãƒ£ã‚¤ãƒ­ã‚»ãƒ³ã‚µãƒ¼ã‚¨ãƒ©ãƒ¼å€¤
	float f_kp = 0.0f;				// æ¯”ä¾‹ã‚²ã‚¤ãƒ³
	float f_ki = 0.0f;
	float f_kd = 0.0f;


	f_err = f_TrgtAngleS - GYRO_getSpeedErr();			// ç›®æ¨™è§’åº¦ - ã‚¸ãƒ£ã‚¤ãƒ­ã‚»ãƒ³ã‚µ[rad/s]
	f_kp = f_FB_angleS_kp;
	f_ki = f_FB_angleS_ki;
	f_kd = f_FB_angleS_kd;

	f_AngleSErrSum += f_err;//*f_ki;

	if(f_AngleSErrSum > 10000.0){
		f_AngleSErrSum = 10000.0;			//ä¸Šé™ãƒªãƒŸãƒƒã‚¿ãƒ¼
	}
	else if(f_AngleSErrSum <-10000.0){
		f_AngleSErrSum = -10000.0;
	}

	*p_err = f_err * f_kp + f_AngleSErrSum*f_ki + ( f_err - f_ErrAngleSBuf ) * f_kd;		// PIDåˆ¶å¾¡

	f_ErrAngleSBuf = f_err;		// åå·®ã‚’ãƒ???¿½?¿½??¿½?¿½?ãƒ•ã‚¡ãƒªãƒ³ã‚°
	if((f_err>14.0)||(f_err<-14.0)){
		if(!(en_Type == CTRL_HIT_WALL)){
			Failsafe_flag();
		}
	}
}

void CTRL_getSenFB( float* p_err )
{
	float f_err 	= 0;
	float f_kp 		= 0.0f;				// æ¯”ä¾‹ã‚²ã‚¤ãƒ³
	float f_kd 		= 0.0f;				// å¾®???¿½?¿½??¿½?¿½?ã‚²ã‚¤ãƒ³
	float gyro		= 0.0f;

	/* ç›´é€²???¿½?¿½??¿½?¿½? */
	if( ( en_Type == CTRL_ACC ) || ( en_Type == CTRL_CONST ) || ( en_Type == CTRL_DEC )||
			 ( en_Type == CTRL_ENTRY_SURA ) || ( en_Type == CTRL_EXIT_SURA ) ){

		f_kp = f_FB_wall_kp;
		f_kd = f_FB_wall_kd;

		/* åå·®å–ï¿½? */
		DIST_getErr( &l_WallErr );
		f_err = (float)l_WallErr;
//		templog2 = f_err;
		/* PDåˆ¶å¾¡ */

		f_ErrDistBuf = f_err;		// åå·®ã‚’ãƒ???¿½?¿½??¿½?¿½?ãƒ•ã‚¡ãƒªãƒ³ã‚°

		*p_err = f_err * f_kp + ( f_err - f_ErrDistBuf ) * f_kd;		// PDåˆ¶å¾¡
	}
	else if( ( en_Type == CTRL_SKEW_ACC ) || ( en_Type == CTRL_SKEW_CONST ) || ( en_Type == CTRL_SKEW_DEC ) ){

		DIST_getErrSkew( &l_WallErr );
		f_err = (float)l_WallErr;

//		*p_err = f_err * f_kp + ( f_err - f_ErrDistBuf ) * f_kd;		// PDåˆ¶å¾¡
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
	float	f_temp;			// è¨ˆç®—ç”¨

	/* é›»åœ§ã«å¿œã˜ã¦PWMå‡ºåŠ›ã‚’å¤‰æ›´ã™ã‚‹ */
	f_duty10_R = f_duty10_R*1000;
	f_duty10_L = f_duty10_L*1000;

	/* å³ãƒ¢ãƒ¼ã‚¿ */
	if( 20 < f_duty10_R ){									// å‰é€²
		DCM_setDirCw( DCM_R );
		DCM_setPwmDuty( DCM_R, (uint16_t)f_duty10_R );
	}
	else if( f_duty10_R < -20 ){							// å¾Œé€€
		f_temp = f_duty10_R * -1;
		DCM_setDirCcw( DCM_R );
		DCM_setPwmDuty( DCM_R, (uint16_t)f_temp );
	}
	else{
		DCM_brakeMot( DCM_R );								// ãƒ–ãƒ¬ãƒ¼ã‚­
	}

	/* å·¦ãƒ¢ãƒ¼ã‚¿ */
	if( 20 < f_duty10_L ){									// å‰é€²
		DCM_setDirCw( DCM_L );
		DCM_setPwmDuty( DCM_L, (uint16_t)f_duty10_L );
	}
	else if( f_duty10_L < -20 ){							// å¾Œé€€
		f_temp = f_duty10_L * -1;
		DCM_setDirCcw( DCM_L );
		DCM_setPwmDuty( DCM_L, (uint16_t)f_temp );
	}
	else{
		DCM_brakeMot( DCM_L );								// ãƒ–ãƒ¬ãƒ¼ã‚­
	}
}

void CTRL_pol( void )
{
	float f_feedFoard_speed		= 0;		// [åˆ¶å¾¡] ãƒ•ã‚£ãƒ¼ãƒ‰ãƒ•ã‚©ãƒ¯ãƒ¼ãƒ‰åˆ¶å¾¡
	float f_feedFoard_angle		= 0;
	float f_speedCtrl			= 0;		// [åˆ¶å¾¡] é€Ÿåº¦åˆ¶å¾¡???¿½?¿½??¿½?¿½?
	float f_angleSpeedCtrl			= 0;		// [åˆ¶å¾¡] è§’é€Ÿåº¦åˆ¶å¾¡???¿½?¿½??¿½?¿½?
	float f_distSenCtrl			= 0;		// [åˆ¶å¾¡] è·é›¢ã‚»ãƒ³ã‚µãƒ¼åˆ¶å¾¡???¿½?¿½??¿½?¿½?
	float f_floorfriction		= 0;
	float f_duty10_R;						// [å‡ºåŠ›] å³ãƒ¢ãƒ¼ã‚¿PWM-DUTYæ¯”[0.1%]
	float f_duty10_L;						// [å‡ºåŠ›] å·¦ãƒ¢ãƒ¼ã‚¿PWM-DUTYæ¯”[0.1%]

	float TR = 0.0;
	float TL = 0.0;
	float Ir = 0.0;
	float Il = 0.0;

	/* åˆ¶å¾¡ã‚’è¡Œã†ã‹ï¿½????¿½?¿½??¿½?¿½ãƒã‚§???¿½?¿½??¿½?¿½?ã‚¯ */
	if( uc_CtrlFlag != TRUE ){
		 return;		// åˆ¶å¾¡ç„¡åŠ¹çŠ¶???¿½?¿½??¿½?¿½?
	}
	if(SW_ON == SW_IsOn_0()){
		Failsafe_flag();
	}

	/* åˆ¶å¾¡ä¸ï¿½????¿½?¿½??¿½?¿½ */
	if (SYS_isOutOfCtrl() == TRUE ){

		f_DistErrSum = 0;				// ç´¯ç©åå·®ã‚¯ãƒªã‚¢
		f_NowDist = f_LastDist;			// å¼·åˆ¶???¿½?¿½??¿½?¿½?ã«æœ€çµ‚ç›®æ¨™ä½ç½®ã«å¤‰æ›´
		f_NowAngle = f_LastAngle;		// å¼·åˆ¶???¿½?¿½??¿½?¿½?ã«æœ€çµ‚ç›®æ¨™æ™‚é–“ã«å¤‰æ›´
		f_Time = f_TrgtTime;			// å¼·åˆ¶???¿½?¿½??¿½?¿½?ã«æœ€çµ‚ç›®æ¨™æ™‚é–“ã«å¤‰æ›´

	 	CTRL_stop();				// åˆ¶å¾¡åœæ­¢
		CTRL_clrData();					// ???¿½?¿½??¿½?¿½?ãƒ¼ã‚¿ã‚¯ãƒªã‚¢
		DCM_brakeMot( DCM_R );			// ãƒ–ãƒ¬ãƒ¼ã‚­
		DCM_brakeMot( DCM_L );			// ãƒ–ãƒ¬ãƒ¼ã‚­
	}

	/* ???¿½?¿½??¿½?¿½?ç¨®ã‚»ãƒ³ã‚µå…¥???¿½?¿½??¿½?¿½? */
	ENC_GetDiv( &l_CntR, &l_CntL );					// ç§»å‹•é‡[ã‚«ã‚¦ãƒ³ãƒˆå€¤]ã‚’å–???¿½?¿½??¿½?¿½?
	//add get_motor_omega(l_CntR,l_CntL);
	CTRL_refNow();									// åˆ¶å¾¡ã«ä½¿ç”¨ã™ã‚‹å€¤ã‚’ç¾åœ¨ã®çŠ¶æ…‹ã«æ›´æ–°
	CTRL_refTarget();								// åˆ¶å¾¡ã«ä½¿ç”¨ã™ã‚‹å€¤ã‚’ç›®æ¨™å€¤ã«æ›´æ–°

	f_NowAngle = GYRO_getNowAngle();					// ç¾åœ¨è§’åº¦[deg]

	/* åˆ¶å¾¡å€¤å–ï¿½? */
	CTRL_getFF_speed( &f_feedFoard_speed );					// [åˆ¶å¾¡] ãƒ•ã‚£ãƒ¼ãƒ‰ãƒ•ã‚©ãƒ¯ãƒ¼???¿½?¿½??¿½?¿½?
	CTRL_getFF_angle( &f_feedFoard_angle );					// [åˆ¶å¾¡] ãƒ•ã‚£ãƒ¼ãƒ‰ãƒ•ã‚©ãƒ¯ãƒ¼???¿½?¿½??¿½?¿½?
	CTRL_getSpeedFB( &f_speedCtrl );				// [åˆ¶å¾¡] é€Ÿåº¦
	CTRL_getAngleSpeedFB( &f_angleSpeedCtrl );			// [åˆ¶å¾¡] è§’é€Ÿåº¦
	CTRL_getSenFB( &f_distSenCtrl );				// [åˆ¶å¾¡] ???¿½?¿½??¿½?¿½?
	CTRL_getFloorFriction( &f_floorfriction );

	/* ç›´é€²åˆ¶å¾¡ */
	if( ( en_Type == CTRL_ACC ) || ( en_Type == CTRL_CONST ) || ( en_Type == CTRL_DEC ) ||( en_Type == CTRL_ENTRY_SURA ) || ( en_Type == CTRL_EXIT_SURA ) ||
		( en_Type == CTRL_SKEW_ACC ) || ( en_Type == CTRL_SKEW_CONST ) || ( en_Type == CTRL_SKEW_DEC )
	){
		TR = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))+(TIRE_D/2/TREAD)*(Inertia*(f_feedFoard_angle + f_angleSpeedCtrl+ f_distSenCtrl)))/GEAR_RATIO;
		TL = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))-(TIRE_D/2/TREAD)*(Inertia*(f_feedFoard_angle + f_angleSpeedCtrl+ f_distSenCtrl)))/GEAR_RATIO;
		Ir = (TR+0.0255/1000.0)/Torque_constant;
		Il = (TL+0.0255/1000.0)/Torque_constant;
	}

	/* å£ã‚ã¦åˆ¶å¾¡ */
	else if( en_Type == CTRL_HIT_WALL ){
		TR = (TIRE_D/2/2)*(Weight*(f_feedFoard_speed * FF_HIT_BALANCE_R/2500.0 ));		// å³ãƒ¢ãƒ¼ã‚¿PWM-DUTYæ¯”[0.1%]
		TL = (TIRE_D/2/2)*(Weight*(f_feedFoard_speed * FF_HIT_BALANCE_R/2500.0 ));
		Ir = (TR-0.0255/1000.0)/Torque_constant;
		Il = (TL-0.0255/1000.0)/Torque_constant;
	}

	/* ã‚¹ãƒ©ãƒ­ãƒ¼???¿½?¿½??¿½?¿½?åˆ¶å¾¡ */
	else if( ( en_Type == CTRL_ACC_SURA ) || (en_Type == CTRL_CONST_SURA)||( en_Type == CTRL_DEC_SURA ) ){
		/* å·¦æ—‹å› */
		if( f_LastAngle > 0 ){
			TR = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))+(TIRE_D/2/TREAD)*(4.65/1000000.0*(f_feedFoard_angle + f_angleSpeedCtrl)+f_floorfriction))/GEAR_RATIO;
			TL = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))-(TIRE_D/2/TREAD)*(4.65/1000000.0*(f_feedFoard_angle + f_angleSpeedCtrl)+f_floorfriction))/GEAR_RATIO;
			Ir = (TR+0.0255/1000.0)/Torque_constant;
			Il = (TL+0.0255/1000.0)/Torque_constant;
		}
		/*å³æ—‹å› */
		else{			
			TR = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))+(TIRE_D/2/TREAD)*(4.65/1000000.0*(f_feedFoard_angle*(-1) + f_angleSpeedCtrl)+f_floorfriction))/GEAR_RATIO;
			TL = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))-(TIRE_D/2/TREAD)*(4.65/1000000.0*(f_feedFoard_angle*(-1) + f_angleSpeedCtrl)+f_floorfriction))/GEAR_RATIO;
			Ir = (TR+0.0255/1000.0)/Torque_constant;
			Il = (TL+0.0255/1000.0)/Torque_constant;
		}
	}

	/* ???¿½?¿½??¿½?¿½?ä¿¡åœ°æ—‹å› */
	else{
		/* ???¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½ */
		if( f_LastAngle > 0 ){			
			TR = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))+(TIRE_D/2/TREAD_imagin)*(Inertia*(f_feedFoard_angle + f_angleSpeedCtrl)+f_floorfriction))/GEAR_RATIO;
			TL = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))-(TIRE_D/2/TREAD_imagin)*(Inertia*(f_feedFoard_angle + f_angleSpeedCtrl)+f_floorfriction))/GEAR_RATIO;
			Ir = (TR+0.0255/1000.0)/Torque_constant;
			Il = (TL-0.0255/1000.0)/Torque_constant;
		}
		/* ???¿½?¿½??¿½?¿½E???¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½ */
		else{			
			TR = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))+(TIRE_D/2/TREAD_imagin)*(Inertia*(f_feedFoard_angle*(-1) + f_angleSpeedCtrl)+f_floorfriction))/GEAR_RATIO;
			TL = ((TIRE_D/2/2)*(Weight*(f_feedFoard_speed + f_speedCtrl))-(TIRE_D/2/TREAD_imagin)*(Inertia*(f_feedFoard_angle*(-1) + f_angleSpeedCtrl)+f_floorfriction))/GEAR_RATIO;
			Ir = (TR-0.0255/1000.0)/Torque_constant;
			Il = (TL+0.0255/1000.0)/Torque_constant;
		}
	}
	f_duty10_R = FF_BALANCE_R*(Motor_Register*Ir+f_MotorR_AngleS*0.001033/1000.0/2.0/PI)/get_battLv();	
	f_duty10_L = FF_BALANCE_L*(Motor_Register*Il+f_MotorL_AngleS*0.001033/1000.0/2.0/PI)/get_battLv();	

	templog1 = f_floorfriction;
	templog2 = f_duty10_R;

	escape_wait = escape_wait+0.001;
	CTRL_outMot( f_duty10_R, f_duty10_L );				// ãƒ¢ãƒ¼ã‚¿ã¸å‡º???¿½?¿½??¿½?¿½?

	f_Time += 0.001;

	/* ??¿½?¿½????¿½?¿½??¿½?¿½??ã‚Œãƒã‚§???¿½?¿½??¿½?¿½?ã‚¯ */
	if( MOT_getWallEdgeType() == MOT_WALL_EDGE_RIGHT ){

		/* å£æŠœ???¿½?¿½??¿½?¿½? */
		if( DIST_isWall_R_SIDE() == FALSE ){
			MOT_setWallEdge( TRUE );		// ??¿½?¿½????¿½?¿½??¿½?¿½????¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½?ã‚Œç›®ã‚’æ¤œçŸ¥
		}
	}
	else if( MOT_getWallEdgeType() == MOT_WALL_EDGE_LEFT ){

		/* å£æŠœ???¿½?¿½??¿½?¿½? */
		if( DIST_isWall_L_SIDE() == FALSE ){
			MOT_setWallEdge( TRUE );		// ??¿½?¿½????¿½?¿½??¿½?¿½????¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½?ã‚Œç›®ã‚’æ¤œçŸ¥
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


