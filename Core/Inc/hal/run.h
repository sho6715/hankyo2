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
#include "Inc/hal/sen_dist.h"
#else
#include "glob.h"
#include "parameter.h"
#include "hal/CTRL.h"
#include "hal/gyro.h"
#include "search/search.h"
#include "hal/DCM.h"
#include "hal/sen_dist.h"
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

/* 旋回コマンドリスト */
typedef enum{
	MOT_R90 =0,					// 右 90度超信地旋回
	MOT_L90,					// 左 90度超信地旋回
	MOT_R180,					// 右180度超信地旋回
	MOT_L180,					// 左180度超信地旋回
	MOT_R360,					// 右360度超信地旋回
	MOT_L360,					// 左360度超信地旋回
	MOT_TURN_CMD_MAX
}enMOT_TURN_CMD;

/* スラロームコマンドリスト */
typedef enum{
	MOT_R90S =0,				// 右 90度超スラローム
	MOT_L90S,					// 左 90度超スラローム
	MOT_R45S_S2N,				// [斜め用] 右 45度超スラローム、ストレート ⇒ 斜め
	MOT_L45S_S2N,				// [斜め用] 左 45度超スラローム、ストレート ⇒ 斜め
	MOT_R45S_N2S,				// [斜め用] 右 45度超スラローム、斜め ⇒ ストレート
	MOT_L45S_N2S,				// [斜め用] 左 45度超スラローム、斜め ⇒ ストレート
	MOT_R90S_N,					// [斜め用] 右 90度超スラローム、斜め ⇒ 斜め
	MOT_L90S_N,					// [斜め用] 左 90度超スラローム、斜め ⇒ 斜め
	MOT_R135S_S2N,				// [斜め用] 右135度超スラローム、ストレート ⇒ 斜め
	MOT_L135S_S2N,				// [斜め用] 左135度超スラローム、ストレート ⇒ 斜め
	MOT_R135S_N2S,				// [斜め用] 右135度超スラローム、斜め ⇒ ストレート
	MOT_L135S_N2S,				// [斜め用] 左135度超スラローム、斜め ⇒ ストレート
	MOT_SURA_CMD_MAX,
}enMOT_SURA_CMD;

/* 壁切れ補正 */
typedef enum{
	MOT_WALL_EDGE_NONE =0,		// 壁のエッジ検出での補正なし
	MOT_WALL_EDGE_RIGHT,		// 右壁のエッジ検出での補正
	MOT_WALL_EDGE_LEFT,			// 左壁のエッジ検出での補正
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
void DIST_Front_Wall_correction(void);


#endif /* INC_RUN_H_ */
