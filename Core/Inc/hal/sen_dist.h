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
