/*
 * CTRL.h
 *
 *  Created on: 2021/01/30
 *      Author: shohe
 */

#ifndef INC_CTRL_H_
#define INC_CTRL_H_

#ifdef _VSCODE
#include "Inc/glob.h"
#include "Inc/hal/DCM.h"
#include "Inc/hal/gyro.h"
#include "Inc/gpio.h"
#include "Inc/hal/run.h"
#include "Inc/parameter.h"
#include "Inc/hal/encoder.h"
#else
#include "glob.h"
#include "hal/DCM.h"
#include "hal/gyro.h"
#include "gpio.h"
#include "hal/run.h"
#include "parameter.h"
#include "hal/encoder.h"
#endif

/* motion type */
typedef enum{
	CTRL_ACC,				// [00] 
	CTRL_CONST,				// [01] 
	CTRL_DEC,				// [02] 

	CTRL_SKEW_ACC,			// [03] 
	CTRL_SKEW_CONST,		// [04] 
	CTRL_SKEW_DEC,			// [05] 
	
	CTRL_HIT_WALL,			// [06]
	
	CTRL_ACC_TRUN,			// [07] 
	CTRL_CONST_TRUN,		// [08] 
	CTRL_DEC_TRUN,			// [09] 
	
	CTRL_ENTRY_SURA,		// [10]
	CTRL_ACC_SURA,			// [11] 
	CTRL_CONST_SURA,		// [12] 
	CTRL_DEC_SURA,			// [13] 
	CTRL_EXIT_SURA,			// [14] 

	CTRL_FRONT_WALL,		// [15]

	CTRL_MAX,

}enCTRL_TYPE;

/* 制御�?ータ */
typedef struct{
	enCTRL_TYPE		en_type;		// motion type
	float			f_time;			// target time[sec]
	float			f_acc;			// [speed ctrl] accel speed[mm/s2]
	float			f_now;			// [speed ctrl] now speed[mm/s]
	float			f_trgt;			// [speed ctrl] finish speed[mm/s]
	float			f_nowDist;		// [dist ctrl] now distance[mm]
	float			f_dist;			// [dist ctrl] finish distance[mm]
	float			f_accAngleS;	// [angle speed ctrl] angle accel speed[rad/s2]
	float			f_nowAngleS;	// [angle speed ctrl] now angle speed[rad/s]
	float			f_trgtAngleS;	// [angle speed ctrl] finish angle speed[rad/s]
	float			f_nowAngle;		// [angle ctrl] now angle[rad]
	float			f_angle;		// [angle ctrl] finish angle[rad]
}stCTRL_DATA;

enCTRL_TYPE		en_Type;						// motion type
//escape time 
float	escape_wait;

uint8_t	uc_dist_control;

float Get_NowSpeed(void);
float Get_NowDist(void);
float Get_TrgtDist(void);
float Get_TrgtSpeed(void);
void Set_TrgtSpeed(float speed);
float Get_NowAngle(void);
float Get_TrgtAngle(void);
float Get_TrgtAngleS(void);
void INTC_sys(void);
void CTRL_sta( void );
void CTRL_stop( void );
void CTRL_pol( void );
void CTRL_clrData( void );
void CTRL_clrNowData( void );
void CTRL_setNowData_Err(/*float trgt_Dist, */float trgt_Angle);
void CTRL_setData( stCTRL_DATA* p_data );
void CTRL_refNow( void );
void CTRL_refTarget( void );
void CTRL_outMot( float f_duty10_R, float f_duty10_L );
void Failsafe_flag(void);
void Failsafe_flag_off(void);
bool SYS_isOutOfCtrl( void );

#endif /* INC_CTRL_H_ */
