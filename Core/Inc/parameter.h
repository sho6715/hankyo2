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

#define HALF_BLOCK			( 0.045f )					
#define BLOCK				( 0.09f )					
#define HALF_BLOCK_SKEW			( 0.06364f )
#define BLOCK_SKEW			( 0.12728f )		

#define		SW_CHATTERING_WAIT		(200) 	

#define	FF_BALANCE_R				( 1.00f )					
#define	FF_BALANCE_L				( 1.40f )		
#define FF_HIT_BALANCE_R			(1.00f)				
#define FF_HIT_BALANCE_L			(1.00f)

#define SEARCH_SPEED				(0.3)
#define MAP_KNOWN_ACC_SPEED			(0.5)

#define DEG_TO_RAD  (3.1416f/180.0f)
#define RAD_TO_DEG  (180.0f/3.1416f)

#define MOT_WALL_EDGE_DIST			( 28.0f )

#define 		f_FB_speed_kp		(35.0)
#define 		f_FB_speed_ki		(4.0)
#define 		f_FB_speed_kd		(0.5)
#define		f_FB_angleS_kp		(130.0)//130
#define		f_FB_angleS_ki		(9.5)//9.5
#define		f_FB_angleS_kd		(0.2)//0.2
#define		f_FB_wall_kp		(0.5)
#define		f_FB_wall_kd		(0.1)

/*
#define 		f_FB_speed_kp		(10.0)
#define 		f_FB_speed_ki		(3.0)
#define 		f_FB_speed_kd		(0.2)
#define		f_FB_angleS_kp		(10.0)
#define		f_FB_angleS_ki		(0.4)
#define		f_FB_angleS_kd		(0.1)
#define		f_FB_wall_kp		(0.5)
#define		f_FB_wall_kd		(0.1)
*/
typedef enum{
	
	PARAM_ST_TOP = 0,		

		PARAM_ACC,			
		PARAM_CONST,			
		PARAM_DEC,		
//		PARAM_BACK_ACC,			
//		PARAM_BACK_CONST,		
//		PARAM_BACK_DEC,			
		PARAM_SKEW_ACC,		
		PARAM_SKEW_CONST,	
		PARAM_SKEW_DEC,		
		PARAM_HIT_WALL,		

	PARAM_ST_BTM,		

	PARAM_TRUN_TOP,	

		PARAM_ACC_TRUN,		
		PARAM_CONST_TRUN,	
		PARAM_DEC_TRUN,	
	
	PARAM_TRUN_BTM,	

	PARAM_SLA_TOP,			

		PARAM_ENTRY_SURA,	
		PARAM_ACC_SURA,		
		PARAM_CONST_SURA,	
		PARAM_DEC_SURA,	
		PARAM_EXIT_SURA,		

	PARAM_SLA_BTM,			
	
	PARAM_ST,		
	PARAM_TRUN,			
	PARAM_SLA,	

	PARAM_ST_MAX		= PARAM_ST_BTM   - PARAM_ST_TOP,		
	PARAM_TRUN_MAX		= PARAM_TRUN_BTM - PARAM_TRUN_TOP,		
	PARAM_SURA_MAX		= PARAM_SLA_BTM  - PARAM_SLA_TOP,			
	
	PARAM_NC = 0xff,
	
}enPARAM_MODE;


typedef enum{
	
	PARAM_VERY_SLOW = 0,	
	PARAM_SLOW,				
	PARAM_NORMAL,			
	PARAM_FAST,				
	PARAM_VERY_FAST,		

	PARAM_MOVE_SPEED_MAX
	
}enPARAM_MOVE_SPEED;

typedef struct{
	float			f_acc;					
	float			f_dec;					
	float			f_accAngle;				
	float			f_decAngle;				
}stSPEED;

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


typedef enum{
	SLA_90,
	SLA_45,	
	SLA_135,
	SLA_N90,				
	SLA_TYPE_MAX
}enSLA_TYPE;


void PARAM_setSpeedType( enPARAM_MODE en_mode, enPARAM_MOVE_SPEED en_speed );
const stSPEED* PARAM_getSpeed( enPARAM_MODE en_mode );
//const stGAIN* PARAM_getGain( enPARAM_MODE en_mode );

void PARAM_makeSra( float f_speed, float f_angAcc, float f_g , enSLA_TYPE en_mode);
stSLA* PARAM_getSra( enSLA_TYPE en_mode );

#endif /* INC_PARAMETER_H_ */
