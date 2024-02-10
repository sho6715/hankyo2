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
#include <stdint.h>

#define SW_ON	(1)
#define SW_OFF	(0)
#define TRUE	(1)
#define FALSE	(0)
/* ăăŻă­ */
#define FABS(x)						( (x)>=0 ? (x) : -(x) )

#define GOAL_MAP_X_def					( 1 )//2				// ă´ăźăŤăŽXĺşçťć°?źć¨Şćšĺďź? [ĺşçť]17
#define GOAL_MAP_Y_def					( 0 )//11				// ă´ăźăŤăŽYĺşçťć°?źç¸Śćšĺďź? [ĺşçť]13
#define MAP_X_SIZE					( 32 )				// čżˇčˇŻăŽXĺşçťć°?źć¨Şćšĺďź? [ĺşçť]
#define MAP_Y_SIZE					( 32 )				// čżˇčˇŻăŽYĺşçťć°?źç¸Śćšĺďź? [ĺşçť]

#define MAP_X_SIZE_REAL				( 32 )					// čżˇčˇŻăŽĺŽXĺşçťć°?źć¨Şćšĺďź? [ĺşçť]
#define MAP_Y_SIZE_REAL				( 32 )					// čżˇčˇŻăŽĺŽYĺşçťć°?źç¸Śćšĺďź? [ĺşçť]

#define PI							( 3.14159f )								// Ď

#define VCC_MAX						( 4.2f )									// ăăă?ăŞćĺ¤§éťĺ§[V]ă?4.2[V]Ă?1[ăťăŤ]
#define TIRE_D						( 0.01251f )	//ć¨ĺŽĺ¤								// ăżă¤ă¤ç´ĺž? [mm]
#define TREAD_imagin					(0.0386f)
#define TREAD						( 0.0335f )
#define ROTATE_PULSE				( 4096 )									// 1ĺ¨ăŽćĺ¤§ć°ĺ¤
#define DIST_1STEP					( PI * TIRE_D / ROTATE_PULSE )				// 1ăăŤăšă§é˛ăčˇé˘ [m]
#define F_CNT2MM(cnt)				( (float)cnt * DIST_1STEP )				// [ăŤăŚăłăĺ¤]ăă[mm]ă¸ćçŽ?
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

/* ă˘ăźăżID */
typedef enum{
	DCM_R = 0,					// ĺłă˘ăźăż
	DCM_L,						// ĺˇŚă˘ăźăż
	DCM_MAX
}enDCM_ID;

/* ćšĺ? */
typedef enum{
	NORTH =0,
	EAST,
	SOUTH,
	WEST,
	MAP_HEAD_DIR_MAX,
}enMAP_HEAD_DIR;

volatile uint16_t		Msec_in;		//ĺ?é¨ćč¨?[msec]
volatile uint8_t		Sec_in;		//ĺ?é¨ćč¨?[sec]
volatile uint8_t		Min_in;		//ĺ?é¨ćč¨?[min]


#endif /* INC_GLOB_H_ */
