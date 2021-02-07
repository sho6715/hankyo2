/*
 * glob.h
 *
 *  Created on: 2021/01/30
 *      Author: shohe
 */

#ifndef INC_GLOB_H_
#define INC_GLOB_H_

#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#define SW_ON	(1)
#define SW_OFF	(0)
#define TRUE	(1)
#define FALSE	(0)
/* マクロ */
#define FABS(x)						( (x)>=0 ? (x) : -(x) )

#define GOAL_MAP_X_def					( 17 )				// ゴールのX区画数（横方向） [区画]17
#define GOAL_MAP_Y_def					( 13 )				// ゴールのY区画数（縦方向） [区画]13
#define MAP_X_SIZE					( 32 )				// 迷路のX区画数（横方向） [区画]
#define MAP_Y_SIZE					( 32 )				// 迷路のY区画数（縦方向） [区画]

#define MAP_X_SIZE_REAL				( 32 )					// 迷路の実X区画数（横方向） [区画]
#define MAP_Y_SIZE_REAL				( 32 )					// 迷路の実Y区画数（縦方向） [区画]

#define PI							( 3.14159f )								// π

#define VCC_MAX						( 4.2f )									// バッテリ最大電圧[V]、4.2[V]×1[セル]
#define TIRE_R						( 12.7f )	//推定値								// タイヤ直径 [mm]
#define ROTATE_PULSE				( 65536 )									// 1周の最大数値
#define DIST_1STEP					( PI * TIRE_R / ROTATE_PULSE )				// 1パルスで進む距離 [mm]
#define F_CNT2MM(cnt)				( (float)cnt * DIST_1STEP )				// [カウント値]から[mm]へ換算

typedef enum{
	DIST_SEN_R_FRONT = 0,
	DIST_SEN_L_FRONT,
	DIST_SEN_R_SIDE,
	DIST_SEN_L_SIDE,
	DIST_SEN_NUM
}enDIST_SEN_ID;

/* モータID */
typedef enum{
	DCM_R = 0,					// 右モータ
	DCM_L,						// 左モータ
	DCM_MAX
}enDCM_ID;

/* 方位 */
typedef enum{
	NORTH =0,
	EAST,
	SOUTH,
	WEST,
	MAP_HEAD_DIR_MAX,
}enMAP_HEAD_DIR;

volatile uint16_t		Msec_in;		// 内部時計[msec]
volatile uint8_t		Sec_in;		// 内部時計[sec]
volatile uint8_t		Min_in;		// 内部時計[min]


#endif /* INC_GLOB_H_ */
