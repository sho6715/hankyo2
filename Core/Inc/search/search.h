/*
 * search.h
 *
 *  Created on: 2021/02/04
 *      Author: shohe
 */

#ifndef INC_SEARCH_SEARCH_H_
#define INC_SEARCH_SEARCH_H_

#ifdef _VSCODE
#include "Inc/glob.h"
#include "Inc/gpio.h"
#include "Inc/hal/run.h"
#include "Inc/search/map_cmd.h"
#include "Inc/search/map_flash.h"
#include "Inc/hal/log.h"
#include "Inc/hal/mode.h"
#include "Inc/queue.h"
#else
#include "glob.h"
#include "gpio.h"
#include "hal/run.h"
#include "search/map_cmd.h"
#include "search/map_flash.h"
#include "hal/log.h"
#include "hal/mode.h"
#include "queue.h"
#endif

/* �T�����@ */
typedef enum{
	CONTOUR_SYSTEM =0,			// ������MAP�@
	MAP_SEARCH_TYPE_MAX,
}enMAP_SEARCH_TYPE;

/* �T�����@ */
typedef enum{
	SEARCH =0,			// �T��
	BEST_WAY,			// �ŒZ
	MAP_ACT_MODE_MAX,
}enMAP_ACT_MODE;

/* �T������ */
typedef enum{
	SEARCH_TURN =0,		// ���M�n����T��
	SEARCH_SURA,		// �X�����[���T��
	SEARCH_SKEW,		// �΂ߒT��
	SEARCH_RETURN,
	SEARCH_MAX,
}enSEARCH_MODE;

uint8_t	Return_X;
uint8_t	Return_Y;

uint16_t	us_cmap[MAP_Y_SIZE][MAP_X_SIZE];				///< ������ �f�[�^
uint8_t		g_sysMap[ MAP_Y_SIZE ][ MAP_X_SIZE ];			///< ���H���

bool			search_flag;

void MAP_init( void );
void MAP_Goal_init( void );
void MAP_Goal_change_x( void );
void MAP_Goal_change_y( void );
void MAP_showLog( void );
void MAP_clearMap( void );
void MAP_setPos( uint8_t uc_x, uint8_t uc_y, enMAP_HEAD_DIR en_dir );
void MAP_searchGoal( uint8_t uc_trgX, uint8_t uc_trgY, enMAP_ACT_MODE en_type, enSEARCH_MODE en_search );
void MAP_makeContourMap( uint8_t uc_goalX, uint8_t uc_goalY, enMAP_ACT_MODE en_type );
void MAP_makeContourMap_run( uint8_t uc_goalX,uint8_t uc_goalY,	enMAP_ACT_MODE	en_type);
void MAP_actGoalLED( void );

void MAP_ClearMapData( void );
void MAP_Goalsize(int size);

//TKR
void MAP_searchGoalKnown(uint8_t uc_trgX, uint8_t uc_trgY, enMAP_ACT_MODE en_type,enSEARCH_MODE	en_search);




#endif /* INC_SEARCH_SEARCH_H_ */
