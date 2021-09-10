/*
 * parameter.h
 *
 *  Created on: 2021/02/04
 *      Author: shohe
 */

#ifndef INC_PARAMETER_H_
#define INC_PARAMETER_H_

#ifdef _VSCODE
#include "Inc/glob.h"
#else
#include "glob.h"
#endif

#define HALF_BLOCK			( 45.0f )					// 半区間 [mm]
#define BLOCK				( 90.0f )					// １区間 [mm]
#define HALF_BLOCK_SKEW			( 63.64f )				// 斜め半区間 [mm]
#define BLOCK_SKEW			( 127.28f )				// 斜め１区間 [mm]

#define		SW_CHATTERING_WAIT		(200) 		//スイッチのチャタリング対策

#define	FF_BALANCE_R				( 1.00f )					// [FF] 右のバランス係数
#define	FF_BALANCE_L				( 1.00f )					// [FF] 左のバランス係数 
#define FF_HIT_BALANCE_R			(1.00f)						//バック時のバランス係数
#define FF_HIT_BALANCE_L			(1.00f)

#define SEARCH_SPEED				(300)
#define MAP_KNOWN_ACC_SPEED			(500)

#define DEG_TO_RAD  (3.1416f/180.0f)
#define RAD_TO_DEG  (180.0f/3.1416f)

#define MOT_WALL_EDGE_DIST			( 28.0f )	// 壁切れセンサOFF～壁まで

#define		f_FF_speed_acc		(0.0016)				// フィードフォワード、加速度
#define		f_FF_angleS_acc		(0.01)		// フィードフォワード、角加速度
#define 		f_FB_speed_kp		(1.8)			// フィードバック、速度 比例制御
#define 		f_FB_speed_ki		(0.2)			// フィードバック、速度 積分制御
#define 		f_FB_speed_kd		(0.3)			// フィードバック、速度 微分制御
#define		f_FB_angleS_kp		(0.8)			// フィードバック、角速度 比例制御
#define		f_FB_angleS_ki		(0.1)			// フィードバック、角速度 積分制御
#define		f_FB_angleS_kd		(0.08)			// フィードバック、角速度 微分制御
#define		f_FB_wall_kp		(0.1)			// フィードバック、壁 比例制御
#define		f_FB_wall_kd		(0.03)			// フィードバック、壁 微分制御

/* 制御方法 */
typedef enum{
	
	/* ========================================== */ 
	/*  パラメータを取得する際に使用するシンボル  */ 
	/* ========================================== */ 
	/* ---------- */
	/*  直進制御  */
	/* ---------- */
	PARAM_ST_TOP = 0,				// カウント用
	// ↓ 動作を追加する場合にはこの間に記載

		PARAM_ACC,					// 加速中(直進)
		PARAM_CONST,				// 等速中(直進)
		PARAM_DEC,					// 減速中(直進)
//		PARAM_BACK_ACC,				// 加速中(後進)
//		PARAM_BACK_CONST,			// 等速中(後進)
//		PARAM_BACK_DEC,				// 減速中(後進)
		PARAM_SKEW_ACC,				// 加速中(斜め)
		PARAM_SKEW_CONST,			// 等速中(斜め)
		PARAM_SKEW_DEC,				// 減速中(斜め)
		PARAM_HIT_WALL,				// 壁あて制御

	// ↑  動作を追加する場合にはこの間に記載
	PARAM_ST_BTM,					// カウント用
	
	/* -------- */
	/*  ターン  */
	/* -------- */
	PARAM_TRUN_TOP,					// カウント用
	// ↓  動作を追加する場合にはこの間に記載

		PARAM_ACC_TRUN,				// 加速中(超地信旋回)
		PARAM_CONST_TRUN,			// 等速中(超地信旋回)
		PARAM_DEC_TRUN,				// 減速中(超地信旋回)

	// ↑  動作を追加する場合にはこの間に記載
	PARAM_TRUN_BTM,					// カウント用
	
	/* ------------ */
	/*  スラローム  */
	/* ------------ */
	PARAM_SLA_TOP,					// カウント用
	// ↓  動作を追加する場合にはこの間に記載

		PARAM_ENTRY_SURA,			// スラローム前の前進動作(スラローム)
		PARAM_ACC_SURA,				// 加速中(スラローム)
		PARAM_CONST_SURA,			// 等速中(スラローム)
		PARAM_DEC_SURA,				// 減速中(スラローム)
		PARAM_EXIT_SURA,			// スラローム後の前進動作(スラローム)

	// ↑  動作を追加する場合にはこの間に記載
	PARAM_SLA_BTM,					// カウント用
	
	
	/* ===================================================================== */ 
	/*  PARAM_setGainType()にてモードを決める際に引数として使用するシンボル  */ 
	/* ===================================================================== */ 
	PARAM_ST,						// 直進制御
	PARAM_TRUN,						// 旋回制御
	PARAM_SLA,						// スラローム制御
	
	
	/* ====================================================== */ 
	/*  作成するデータ数をカウントするために使用するシンボル  */ 
	/* ====================================================== */ 
	PARAM_ST_MAX		= PARAM_ST_BTM   - PARAM_ST_TOP,		// 直進最大数
	PARAM_TRUN_MAX		= PARAM_TRUN_BTM - PARAM_TRUN_TOP,		// 旋回最大数
	PARAM_SURA_MAX		= PARAM_SLA_BTM  - PARAM_SLA_TOP,		// スラローム最大数
	
	
	PARAM_NC = 0xff,
	
}enPARAM_MODE;


/* 動作速度 */
typedef enum{
	
	PARAM_VERY_SLOW = 0,	// 超低速
	PARAM_SLOW,				// 低速
	PARAM_NORMAL,			// 通常
	PARAM_FAST,				// 高速
	PARAM_VERY_FAST,		// 超高速
	
	PARAM_MOVE_SPEED_MAX
	
}enPARAM_MOVE_SPEED;

/* 速度データ */
typedef struct{
	float			f_acc;					// 加速度（加速時）
	float			f_dec;					// 加速度（減速時）
	float			f_accAngle;				// 角加速度（加速時）
	float			f_decAngle;				// 角加速度（減速時）
}stSPEED;

/* ゲイン */
/*
typedef struct{
	float			f_FF_speed_acc;				// フィードフォワード、加速度
	float			f_FF_angleS_acc;		// フィードフォワード、角加速度
	float 			f_FB_speed_kp;			// フィードバック、速度 比例制御
	float 			f_FB_speed_ki;			// フィードバック、速度 積分制御
	float 			f_FB_speed_kd;			// フィードバック、速度 微分制御
	float			f_FB_angleS_kp;			// フィードバック、角速度 比例制御
	float			f_FB_angleS_ki;			// フィードバック、角速度 積分制御
	float			f_FB_angleS_kd;			// フィードバック、角速度 微分制御
	float			f_FB_wall_kp;			// フィードバック、壁 比例制御
	float			f_FB_wall_kd;			// フィードバック、壁 微分制御
}stGAIN;
*/
/* スラロームデータ */
typedef struct{
	float	f_speed;
	float	f_angAcc;
	float	f_angvel;
	float	f_entryLen;
	float	f_escapeLen;
	uint16_t	us_accAngvelTime;
	uint16_t	us_constAngvelTime;
	float	f_ang_AccEnd;
	float	f_ang_ConstEnd;
	float	f_ang_Total;
}stSLA;


/* スラロームタイプ */
typedef enum{
	SLA_90,
	SLA_45,	
	SLA_135,
	SLA_N90,				// 斜め → 90°→ 斜め
	SLA_TYPE_MAX
}enSLA_TYPE;


void PARAM_setSpeedType( enPARAM_MODE en_mode, enPARAM_MOVE_SPEED en_speed );
const stSPEED* PARAM_getSpeed( enPARAM_MODE en_mode );
//const stGAIN* PARAM_getGain( enPARAM_MODE en_mode );

void PARAM_makeSra( float f_speed, float f_angAcc, float f_g , enSLA_TYPE en_mode);
stSLA* PARAM_getSra( enSLA_TYPE en_mode );

#endif /* INC_PARAMETER_H_ */
