/*
 * sen_dist.h
 *
 *  Created on: Jan 8, 2021
 *      Author: shohe
 */

#ifndef INC_SEN_DIST_H_
#define INC_SEN_DIST_H_

#ifdef _VSCODE
#include "Inc/main.h"
#include "Inc/adc.h"
#include "Inc/gpio.h"
#include "Inc/glob.h"
#else
#include "main.h"
#include "adc.h"
#include "gpio.h"
#include "glob.h"
#endif

#define R_FRONT_REF					( 270 )
#define L_FRONT_REF					(3000)//( 210 )
#define R_SIDE_REF					(3000)//( 240 )
#define L_SIDE_REF					(3000)//( 200 )
#define R_FRONT_WALL					(3500)//( 45 )
#define L_FRONT_WALL					(3000)//( 45 )
#define R_SIDE_WALL					(3000)//( 110 )
#define L_SIDE_WALL					(3000)//( 70 )
#define R_FRONT_SKEW_ERR1				( 80 )//仮
#define L_FRONT_SKEW_ERR1				( 70 )
#define R_FRONT_SKEW_ERR2				( 192 )
#define L_FRONT_SKEW_ERR2				( 160 )
#define R_FRONT_SKEW_ERR3				( 250 )
#define L_FRONT_SKEW_ERR3				( 250 )
#define R_FRONT_CTRL					(3500)//( 370 )	//maekabehoseiniryuuyou
#define L_FRONT_CTRL					(3000)//( 300 )
#define R_FRONT_NOCTRL					(3500)//( 530 )
#define L_FRONT_NOCTRL					(3000)//( 450 )


/*距離センサポーリングタイプ*/
typedef enum{
	DIST_POL_FRONT = 0,
	DIST_POL_SIDE,
	DISR_POL_MAX
}enDIST_POL;

/*距離センサ動作状態*/
typedef enum{
	DIST_STANDBAY = 0,
	DIST_NO_CTRL,
	DIST_NO_WALL,
	DIST_FRONT,
	DIST_RIGHT,
	DIST_LEFT,
	DIST_STATE_MAX
}enDIST_SEN_STATE;

void DIST_init( void );
int16_t DIST_getNowVal( enDIST_SEN_ID en_id );
void DIST_getErr( int32_t* p_err );
void DIST_getErrSkew( int32_t* p_err );
void DIST_Pol_FL( void );
void DIST_Pol_SL( void );
void DIST_Pol_FR( void );
void DIST_Pol_SR( void );
void Get_Sen_Nowdata(void);
bool DIST_isWall_FRONT( void );
bool DIST_isWall_R_SIDE( void );
bool DIST_isWall_L_SIDE( void );



#endif /* INC_SEN_DIST_H_ */
