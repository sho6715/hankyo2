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

/* ?ｿｽ?ｿｽ?ｿｽ苴ｮ?ｿｽ?ｿｽ^?ｿｽC?ｿｽv */
typedef enum{
	CTRL_ACC,				// [00] ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ(?ｿｽ?ｿｽ?ｿｽi)
	CTRL_CONST,				// [01] ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ(?ｿｽ?ｿｽ?ｿｽi)
	CTRL_DEC,				// [02] ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ(?ｿｽ?ｿｽ?ｿｽi)

	CTRL_SKEW_ACC,			// [03] ?ｿｽﾎめ会ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ(?ｿｽ?ｿｽ?ｿｽi)
	CTRL_SKEW_CONST,		// [04] ?ｿｽﾎめ難ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ(?ｿｽ?ｿｽ?ｿｽi)
	CTRL_SKEW_DEC,			// [05] ?ｿｽﾎめ鯉ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ(?ｿｽ?ｿｽ?ｿｽi)
	
	CTRL_HIT_WALL,			// [06]?ｿｽﾇ難ｿｽ?ｿｽﾄ難ｿｽ?ｿｽ?ｿｽ
	
	CTRL_ACC_TRUN,			// [07] ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ(?ｿｽ?ｿｽ?ｿｽM?ｿｽn?ｿｽ?ｿｽ?ｿｽ?ｿｽ)
	CTRL_CONST_TRUN,		// [08] ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ(?ｿｽ?ｿｽ?ｿｽM?ｿｽn?ｿｽ?ｿｽ?ｿｽ?ｿｽ)
	CTRL_DEC_TRUN,			// [09] ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ(?ｿｽ?ｿｽ?ｿｽM?ｿｽn?ｿｽ?ｿｽ?ｿｽ?ｿｽ)
	
	CTRL_ENTRY_SURA,		// [10]?ｿｽX?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ[?ｿｽ?ｿｽ?ｿｽO?ｿｽO?ｿｽi
	CTRL_ACC_SURA,			// [11] ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ(?ｿｽX?ｿｽ?ｿｽ)
	CTRL_CONST_SURA,		// [12] ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ(?ｿｽX?ｿｽ?ｿｽ)
	CTRL_DEC_SURA,			// [13] ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ(?ｿｽX?ｿｽ?ｿｽ)
	CTRL_EXIT_SURA,			// [14] ?ｿｽX?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ[?ｿｽ?ｿｽ?ｿｽ?ｿｽO?ｿｽi

	CTRL_MAX,

}enCTRL_TYPE;

/* ?ｿｽ?ｿｽ?ｿｽ?ｿｽf?ｿｽ[?ｿｽ^ */
typedef struct{
	enCTRL_TYPE		en_type;		// ?ｿｽ?ｿｽ?ｿｽ?ｿｽ^?ｿｽC?ｿｽv
	float			f_time;			// ?ｿｽﾚ標?ｿｽ?ｿｽ?ｿｽ?ｿｽ [sec]
	float			f_acc;			// [?ｿｽ?ｿｽ?ｿｽx?ｿｽ?ｿｽ?ｿｽ?ｿｽ]   ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽx[mm/s2]
	float			f_now;			// [?ｿｽ?ｿｽ?ｿｽx?ｿｽ?ｿｽ?ｿｽ?ｿｽ]   ?ｿｽ?ｿｽ?ｿｽﾝ托ｿｽ?ｿｽx[mm/s]
	float			f_trgt;			// [?ｿｽ?ｿｽ?ｿｽx?ｿｽ?ｿｽ?ｿｽ?ｿｽ]   ?ｿｽﾅ終?ｿｽ?ｿｽ?ｿｽx[mm/s]
	float			f_nowDist;		// [?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ]   ?ｿｽ?ｿｽ?ｿｽﾝ具ｿｽ?ｿｽ?ｿｽ[mm]
	float			f_dist;			// [?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ]   ?ｿｽﾅ終?ｿｽ?ｿｽ?ｿｽ?ｿｽ[mm]
	float			f_accAngleS;	// [?ｿｽp?ｿｽ?ｿｽ?ｿｽx?ｿｽ?ｿｽ?ｿｽ?ｿｽ] ?ｿｽp?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽx[rad/s2]
	float			f_nowAngleS;	// [?ｿｽp?ｿｽ?ｿｽ?ｿｽx?ｿｽ?ｿｽ?ｿｽ?ｿｽ] ?ｿｽ?ｿｽ?ｿｽﾝ角?ｿｽ?ｿｽ?ｿｽx[rad/s]
	float			f_trgtAngleS;	// [?ｿｽp?ｿｽ?ｿｽ?ｿｽx?ｿｽ?ｿｽ?ｿｽ?ｿｽ] ?ｿｽﾅ終?ｿｽp?ｿｽ?ｿｽ?ｿｽx[rad/s]
	float			f_nowAngle;		// [?ｿｽp?ｿｽx?ｿｽ?ｿｽ?ｿｽ?ｿｽ]   ?ｿｽ?ｿｽ?ｿｽﾝ角?ｿｽx[rad]
	float			f_angle;		// [?ｿｽp?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ]   ?ｿｽﾅ終?ｿｽp?ｿｽx[rad]
}stCTRL_DATA;

enCTRL_TYPE		en_Type;						// ????
//????????
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
void CTRL_setData( stCTRL_DATA* p_data );
void CTRL_refNow( void );
void CTRL_refTarget( void );
void CTRL_outMot( float f_duty10_R, float f_duty10_L );
void Failsafe_flag(void);
void Failsafe_flag_off(void);
bool SYS_isOutOfCtrl( void );

#endif /* INC_CTRL_H_ */
