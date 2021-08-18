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

/* ���䓮��^�C�v */
typedef enum{
	CTRL_ACC,				// [00] ������(���i)
	CTRL_CONST,				// [01] ������(���i)
	CTRL_DEC,				// [02] ������(���i)

	CTRL_SKEW_ACC,			// [03] �΂߉�����(���i)
	CTRL_SKEW_CONST,		// [04] �΂ߓ�����(���i)
	CTRL_SKEW_DEC,			// [05] �΂ߌ�����(���i)
	
	CTRL_HIT_WALL,			// [06]�Ǔ��ē���
	
	CTRL_ACC_TRUN,			// [07] ������(���M�n����)
	CTRL_CONST_TRUN,		// [08] ������(���M�n����)
	CTRL_DEC_TRUN,			// [09] ������(���M�n����)
	
	CTRL_ENTRY_SURA,		// [10]�X�����[���O�O�i
	CTRL_ACC_SURA,			// [11] ������(�X��)
	CTRL_CONST_SURA,		// [12] ������(�X��)
	CTRL_DEC_SURA,			// [13] ������(�X��)
	CTRL_EXIT_SURA,			// [14] �X�����[����O�i

	CTRL_MAX,

}enCTRL_TYPE;

/* ����f�[�^ */
typedef struct{
	enCTRL_TYPE		en_type;		// ����^�C�v
	float			f_time;			// �ڕW���� [sec]
	float			f_acc;			// [���x����]   �����x[mm/s2]
	float			f_now;			// [���x����]   ���ݑ��x[mm/s]
	float			f_trgt;			// [���x����]   �ŏI���x[mm/s]
	float			f_nowDist;		// [��������]   ���݋���[mm]
	float			f_dist;			// [��������]   �ŏI����[mm]
	float			f_accAngleS;	// [�p���x����] �p�����x[rad/s2]
	float			f_nowAngleS;	// [�p���x����] ���݊p���x[rad/s]
	float			f_trgtAngleS;	// [�p���x����] �ŏI�p���x[rad/s]
	float			f_nowAngle;		// [�p�x����]   ���݊p�x[rad]
	float			f_angle;		// [�p������]   �ŏI�p�x[rad]
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
void CTRL_setData( stCTRL_DATA* p_data );
void CTRL_refNow( void );
void CTRL_refTarget( void );
void CTRL_outMot( float f_duty10_R, float f_duty10_L );
void Failsafe_flag(void);
void Failsafe_flag_off(void);
bool SYS_isOutOfCtrl( void );

#endif /* INC_CTRL_H_ */
