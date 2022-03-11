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

#define GOAL_MAP_X_def					( 16 )				// ゴールのX区画数?��横方向�? [区画]17
#define GOAL_MAP_Y_def					( 16 )				// ゴールのY区画数?��縦方向�? [区画]13
#define MAP_X_SIZE					( 32 )				// 迷路のX区画数?��横方向�? [区画]
#define MAP_Y_SIZE					( 32 )				// 迷路のY区画数?��縦方向�? [区画]

#define MAP_X_SIZE_REAL				( 32 )					// 迷路の実X区画数?��横方向�? [区画]
#define MAP_Y_SIZE_REAL				( 32 )					// 迷路の実Y区画数?��縦方向�? [区画]

#define PI							( 3.14159f )								// π

#define VCC_MAX						( 4.2f )									// バッ�?リ最大電圧[V]�?4.2[V]�?1[セル]
#define TIRE_D						( 0.0126f )	//推定値								// タイヤ直�? [m]
#define TREAD_imagin					(0.0386f)
#define TREAD						( 0.0335f )
#define ROTATE_PULSE				( 4096 )									// 1周の最大数値
#define DIST_1STEP					( PI * TIRE_D / ROTATE_PULSE )				// 1パルスで進む距離 [m]
#define F_CNT2MM(cnt)				( (float)cnt * DIST_1STEP )				// [カウント値]から[m]へ換�?
#define GEAR_RATIO					(36.0f/10.0f)

#define Weight						(0.018f)
#define Inertia						(4.6f/1000000.0f)
#define Torque_constant				(0.594f/1000.0f)
#define Motor_Register				(4.8f)

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

/* 方�? */
typedef enum{
	NORTH =0,
	EAST,
	SOUTH,
	WEST,
	MAP_HEAD_DIR_MAX,
}enMAP_HEAD_DIR;

volatile uint16_t		Msec_in;		// �?部時�?[msec]
volatile uint8_t		Sec_in;		// �?部時�?[sec]
volatile uint8_t		Min_in;		// �?部時�?[min]


#endif /* INC_GLOB_H_ */
