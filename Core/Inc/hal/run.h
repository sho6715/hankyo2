/*
 * run.h
 *
 *  Created on: 2021/01/30
 *      Author: shohe
 */

#ifndef INC_RUN_H_
#define INC_RUN_H_

#ifdef _VSCODE
#include "Inc/glob.h"
#include "Inc/parameter.h"
#include "Inc/hal/CTRL.h"
#include "Inc/hal/gyro.h"
#include "Inc/search/search.h"
#include "Inc/hal/DCM.h"
#else
#include "glob.h"
#include "parameter.h"
#include "hal/CTRL.h"
#include "hal/gyro.h"
#include "search/search.h"
#include "hal/DCM.h"
#endif

/* 動作タイプ */
typedef enum{
	MOT_ST_NC    =  0,
	MOT_ACC_CONST_DEC,			// [01] 台形加速
	MOT_ACC_CONST_DEC_CUSTOM,	// [02] 台形加速（等速値変更）
	MOT_ACC_CONST,				// [03] 加速＋等速
	MOT_ACC_CONST_CUSTOM,		// [04] 加速＋等速（加速値変更）
	MOT_CONST_DEC,				// [05] 等速＋減速
	MOT_CONST_DEC_CUSTOM,		// [06] 等速＋減速（減速値変更）
	MOT_ST_MAX,
}enMOT_ST_TYPE;

/* 直進タイプ */
typedef enum{
	MOT_GO_ST_NORMAL    =  0,	// 通常の直進
	MOT_GO_ST_SKEW,				// 斜めの直進
	MOT_GO_ST_MAX,
}enMOT_GO_ST_TYPE;

/* ����R�}���h���X�g */
typedef enum{
	MOT_R90 =0,					// �E 90�x���M�n����
	MOT_L90,					// �� 90�x���M�n����
	MOT_R180,					// �E180�x���M�n����
	MOT_L180,					// ��180�x���M�n����
	MOT_R360,					// �E360�x���M�n����
	MOT_L360,					// ��360�x���M�n����
	MOT_TURN_CMD_MAX
}enMOT_TURN_CMD;

/* �X�����[���R�}���h���X�g */
typedef enum{
	MOT_R90S =0,				// �E 90�x���X�����[��
	MOT_L90S,					// �� 90�x���X�����[��
	MOT_R45S_S2N,				// [�΂ߗp] �E 45�x���X�����[���A�X�g���[�g �� �΂�
	MOT_L45S_S2N,				// [�΂ߗp] �� 45�x���X�����[���A�X�g���[�g �� �΂�
	MOT_R45S_N2S,				// [�΂ߗp] �E 45�x���X�����[���A�΂� �� �X�g���[�g
	MOT_L45S_N2S,				// [�΂ߗp] �� 45�x���X�����[���A�΂� �� �X�g���[�g
	MOT_R90S_N,					// [�΂ߗp] �E 90�x���X�����[���A�΂� �� �΂�
	MOT_L90S_N,					// [�΂ߗp] �� 90�x���X�����[���A�΂� �� �΂�
	MOT_R135S_S2N,				// [�΂ߗp] �E135�x���X�����[���A�X�g���[�g �� �΂�
	MOT_L135S_S2N,				// [�΂ߗp] ��135�x���X�����[���A�X�g���[�g �� �΂�
	MOT_R135S_N2S,				// [�΂ߗp] �E135�x���X�����[���A�΂� �� �X�g���[�g
	MOT_L135S_N2S,				// [�΂ߗp] ��135�x���X�����[���A�΂� �� �X�g���[�g
	MOT_SURA_CMD_MAX,
}enMOT_SURA_CMD;

/* �ǐ؂�␳ */
typedef enum{
	MOT_WALL_EDGE_NONE =0,		// �ǂ̃G�b�W���o�ł̕␳�Ȃ�
	MOT_WALL_EDGE_RIGHT,		// �E�ǂ̃G�b�W���o�ł̕␳
	MOT_WALL_EDGE_LEFT,			// ���ǂ̃G�b�W���o�ł̕␳
	MOT_WALL_EDGE_MAX,
}enMOT_WALL_EDGE_TYPE;

typedef enum{
	Right,
	Left,
	MAX,
}en_TURNTYPE;

en_TURNTYPE		en_Turntype;

float MOT_getAcc1( void );
float MOT_getAcc3( void );
void MOT_goBlock_AccConstDec( float f_fin, enMOT_ST_TYPE en_type, enMOT_GO_ST_TYPE en_goType );
void MOT_setData_ACC_CONST_DEC( float f_num, float f_fin, enMOT_GO_ST_TYPE en_type );
void MOT_setData_MOT_ACC_CONST_DEC_CUSTOM( float f_num, float f_fin, enMOT_GO_ST_TYPE en_type );
void MOT_setData_MOT_ACC_CONST( float f_num, float f_fin, enMOT_GO_ST_TYPE en_type );
void MOT_setData_MOT_ACC_CONST_CUSTOM( float f_num, float f_fin, enMOT_GO_ST_TYPE en_type );
void MOT_setData_MOT_CONST_DEC( float f_num, float f_fin, enMOT_GO_ST_TYPE en_type );
void MOT_setData_MOT_CONST_DEC_CUSTOM( float f_num, float f_fin, enMOT_GO_ST_TYPE en_type );
enMOT_ST_TYPE MOT_getStType( float f_num, float f_fin, enMOT_GO_ST_TYPE en_type );
void MOT_go_FinSpeed( float f_num, float f_fin, enMOT_GO_ST_TYPE en_goStType );
void MOT_goBlock_FinSpeed( float f_num, float f_fin );
void MOT_goSkewBlock_FinSpeed( float f_num, float f_fin );
void MOT_goBlock_Const(float f_num);
void testrun(void);
float MOT_getAccAngle1( void );
float MOT_getAccAngle3( void );
void MOT_turn( enMOT_TURN_CMD en_type );
void MOT_setSuraStaSpeed( float f_speed );
float MOT_getSuraStaSpeed( void );
float MOT_setTrgtSpeed(float f_speed);
void MOT_setNowSpeed(float f_speed);
void MOT_goHitBackWall(void);
void MOT_goSla( enMOT_SURA_CMD en_type, stSLA* p_sla );
void turntable(void);
void MOT_setWallEdgeType( enMOT_WALL_EDGE_TYPE en_type );
enMOT_WALL_EDGE_TYPE MOT_getWallEdgeType( void );
void MOT_setWallEdge( bool bl_val );
bool MOT_setWallEdgeDist( void );
bool MOT_setWallEdgeDist_LoopWait( void );


#endif /* INC_RUN_H_ */
