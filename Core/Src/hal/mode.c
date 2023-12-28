/*
 * mode.c
 *
 *  Created on: 2021/01/30
 *      Author: shohe
 */


#include "hal/mode.h"

#define mode_1  (0x01)
#define mode_2  (0x10)

typedef enum{
	MODE_0 = 0,
	MODE_1,
	MODE_2,
	MODE_3,
	MODE_4,
	MODE_5,
	MODE_6,
	MODE_7,
	MODE_MAX
}enMODE;

uint8_t now_mode = mode_1;

enMODE		en_Mode;		//現在のモー�?

uint8_t		wall_hit_flag = 0; //0 = not wall_hit

extern uint8_t			SLA_count;

void SYS_start( void )
{
	/* タイトル表示 */
	printf(" ------------------------------\r\n");
	printf(" | Robo Name  : hankyo2       |\r\n");
	printf(" | Developer  : sho sato      |\r\n");
	printf(" | Version    : ver1          |\r\n");
	printf(" | Project By : RT Corporation|\r\n");
	printf(" ------------------------------\r\n");

	printf("\r\n turn 45 \r\r");
	PARAM_makeSra( (float)SEARCH_SPEED, 100.0f, 2.50f, SLA_45 );
	printf("\r\n turn 90 \r\r");		
	PARAM_makeSra( (float)SEARCH_SPEED, 200.0f, 3.50f, SLA_90 );	
//	PARAM_makeSra( (float)SEARCH_SPEED, 150.0f, 3.00f, SLA_90 );
	printf("\r\n turn 135 \r\r");	
	PARAM_makeSra( (float)SEARCH_SPEED, 200.0f, 4.00f, SLA_135 );	
	printf("\r\n turn N90 \r\r");	
	PARAM_makeSra( (float)SEARCH_SPEED, 300.0f, 4.00f, SLA_N90 );		

/*
	printf("\r\n turn 45 \r\r");
	PARAM_makeSra( 0.6, 550.0f, 7.50f, SLA_45 );
	printf("\r\n turn 90 \r\r");		
	PARAM_makeSra( 0.6, 900.0f, 10.00f, SLA_90 );	
//	PARAM_makeSra( (float)SEARCH_SPEED, 150.0f, 3.00f, SLA_90 );
	printf("\r\n turn 135 \r\r");	
	PARAM_makeSra( 0.6, 1350.0f, 13.00f, SLA_135 );	
	printf("\r\n turn N90 \r\r");	
	PARAM_makeSra( 0.6, 1500.0f, 14.00f, SLA_N90 );	
*/
}

void MODE_inc( void )
{
	en_Mode++;		// モードを�?�?

	/* 最大値チェ�?ク */
	if( MODE_MAX == en_Mode ){
		en_Mode = MODE_0;
	}

	/* モード表示 */
	switch( en_Mode ){

		case MODE_0:
			SetLED(0x00 | now_mode);
			break;

		case MODE_1:
			SetLED((0x01<<1) | now_mode);
			break;

		case MODE_2:
			SetLED((0x02<<1) | now_mode);
			break;

		case MODE_3:
			SetLED((0x03<<1) | now_mode);
			break;

		case MODE_4:
			SetLED((0x04<<1) | now_mode);
			break;

		case MODE_5:
			SetLED((0x05<<1) | now_mode);
			break;

		case MODE_6:
			SetLED((0x06<<1) | now_mode);
			break;

		case MODE_7:
			SetLED((0x07<<1) | now_mode);
			break;

		default:
			break;
	}
}

void MODE_exe_m0( void )
{
	enMAP_HEAD_DIR		en_endDir2;
	GYRO_SetRef();

	CTRL_clrData();
	CTRL_clrAngleErrSum();
	CTRL_clrNowData();
	/* モード表示 */
	switch( en_Mode ){

		case MODE_0:
			SetLED(0x0e);
			CTRL_clrNowData();
			CTRL_clrData();
//			LL_TIM_EnableIT_UPDATE(TIM4);
//			LL_TIM_EnableCounter(TIM4);
			printf("\n");
			LL_mDelay(1000);
			GYRO_SetRef();
			while(1){
				printf("  gyro%5.2f ref%5.2f \r", 
					GYRO_getNowAngle(),GYRO_getRef()
				);
				LL_mDelay( 500 );
			}
			break;

		case MODE_1:
			SetLED(0x0e);
			printf("\n");
			LL_mDelay(1000);
			SetLED(0x00);
			while(1){
				printf("  ENC_R%5d ENC_L%5d \r", 
					Get_encoder_value(enR),Get_encoder_value(enL)
				);
				LL_mDelay( 500 );
			}
			break;

		case MODE_2:
			SetLED(0x0e);
			log_read2();
			break;

		case MODE_3:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED);
			MOT_setSuraStaSpeed( SEARCH_SPEED );							// スラロー�?開始速度設�?
			PARAM_setSpeedType( PARAM_ST,   PARAM_VERY_SLOW );							// [直進] 速度普�?
			PARAM_setSpeedType( PARAM_TRUN, PARAM_VERY_SLOW );							// [旋回] 速度普�?
			PARAM_setSpeedType( PARAM_SLA,  PARAM_VERY_SLOW );							// [スラ] 速度普�?
			SetLED(0x00);
			LL_mDelay(500);
			CTRL_clrData();
			CTRL_clrAngleErrSum();
			CTRL_clrNowData();
			log_flag_on();
			DIST_Front_Wall_correction();
			log_flag_off();
			CTRL_stop();
			DCM_brakeMot( DCM_R );		// �u���[�L
			DCM_brakeMot( DCM_L );		// �u���[�L
			SetLED(0x0e);
			break;

		case MODE_4:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED);
			MOT_setSuraStaSpeed( SEARCH_SPEED );							// スラロー�?開始速度設�?
			PARAM_setSpeedType( PARAM_ST,   PARAM_VERY_SLOW );							// [直進] 速度普�?
			PARAM_setSpeedType( PARAM_TRUN, PARAM_VERY_SLOW );							// [旋回] 速度普�?
			PARAM_setSpeedType( PARAM_SLA,  PARAM_VERY_SLOW );							// [スラ] 速度普�?
			SetLED(0x00);
			LL_mDelay(500);
			CTRL_clrData();
			CTRL_clrAngleErrSum();
			CTRL_clrNowData();
			log_flag_on();
/*			MOT_goBlock_FinSpeed(0.5, SEARCH_SPEED);
			MOT_goBlock_Const(1);
			MOT_goSla(MOT_R90S, PARAM_getSra( SLA_90 ));
			MOT_goBlock_FinSpeed(0.5, 0);
*/
			MOT_goBlock_FinSpeed(0.5,SEARCH_SPEED);
			MOT_goSla(MOT_L45S_S2N,PARAM_getSra( SLA_45 ));
			MOT_goSkewBlock_FinSpeed(0.5,SEARCH_SPEED);
			MOT_goSla(MOT_R135S_N2S,PARAM_getSra( SLA_135 ));
			MOT_goBlock_FinSpeed(0.5, 0);
			log_flag_off();
			break;

		case MODE_5:
			SetLED(0x0e);

			printf("\r\n turn 45 \r\r");
			PARAM_makeSra( 0.6, 550.0f, 7.50f, SLA_45 );
			printf("\r\n turn 90 \r\r");		
			PARAM_makeSra( 0.6, 900.0f, 10.00f, SLA_90 );	
			printf("\r\n turn 135 \r\r");	
			PARAM_makeSra( 0.6, 1350.0f, 13.00f, SLA_135 );	
			printf("\r\n turn N90 \r\r");	
			PARAM_makeSra( 0.6, 1500.0f, 14.00f, SLA_N90 );	

			MOT_setTrgtSpeed(SEARCH_SPEED*4.0);
			MOT_setSuraStaSpeed( SEARCH_SPEED*2.0 );							// スラロー�?開始速度設�?
			PARAM_setSpeedType( PARAM_ST,   PARAM_VERY_FAST );							// [直進] 速度普�?
			PARAM_setSpeedType( PARAM_TRUN, PARAM_NORMAL );							// [旋回] 速度普�?
			PARAM_setSpeedType( PARAM_SLA,  PARAM_NORMAL );							// [スラ] 速度普�?
			SetLED(0x00);
			LL_mDelay(500);
			CTRL_clrData();
			CTRL_clrAngleErrSum();
			CTRL_clrNowData();
			Set_DutyTIM8(600);
			LL_mDelay(2000);
			log_flag_on();

			MOT_goBlock_FinSpeed(1.5, SEARCH_SPEED*2.0);
			MOT_goSla(MOT_R90S, PARAM_getSra( SLA_90 ));
			MOT_goBlock_FinSpeed(0.5, 0);

/*
			MOT_goBlock_FinSpeed(1.0, SEARCH_SPEED);
			MOT_goSla(MOT_R45S_S2N, PARAM_getSra( SLA_45 ));
			MOT_goSkewBlock_FinSpeed(0.5, 0);
*/
/*
			MOT_goSkewBlock_FinSpeed(0.5, SEARCH_SPEED);
			MOT_goSla(MOT_R90S_N, PARAM_getSra( SLA_N90 ));
			MOT_goSkewBlock_FinSpeed(0.5, 0);
*/
/*
			MOT_goBlock_FinSpeed(0.5, SEARCH_SPEED);
			MOT_goSla(MOT_R135S_S2N, PARAM_getSra( SLA_135 ));
			MOT_goSkewBlock_FinSpeed(0.5, 0);
*/
			log_flag_off();

			Set_DutyTIM8(0);
			LL_mDelay(2000);

			PARAM_makeSra( (float)SEARCH_SPEED, 100.0f, 2.50f, SLA_45 );
			printf("\r\n turn 90 \r\r");		
			PARAM_makeSra( (float)SEARCH_SPEED, 200.0f, 3.50f, SLA_90 );	
		//	PARAM_makeSra( (float)SEARCH_SPEED, 150.0f, 3.00f, SLA_90 );
			printf("\r\n turn 135 \r\r");	
			PARAM_makeSra( (float)SEARCH_SPEED, 200.0f, 4.00f, SLA_135 );	
			printf("\r\n turn N90 \r\r");	
			PARAM_makeSra( (float)SEARCH_SPEED, 300.0f, 4.00f, SLA_N90 );		

			MOT_setTrgtSpeed(SEARCH_SPEED);
			MOT_setSuraStaSpeed( SEARCH_SPEED );							// スラロー�?開始速度設�?
			break;

		case MODE_6:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED);
			MOT_setSuraStaSpeed( SEARCH_SPEED );							// スラロー�?開始速度設�?
			PARAM_setSpeedType( PARAM_ST,   PARAM_VERY_FAST );							// [直進] 速度普�?
			PARAM_setSpeedType( PARAM_TRUN, PARAM_VERY_FAST );							// [旋回] 速度普�?
			PARAM_setSpeedType( PARAM_SLA,  PARAM_VERY_FAST );							// [スラ] 速度普�?
			SetLED(0x00);
			LL_mDelay(500);
			CTRL_clrData();
			CTRL_clrAngleErrSum();
			CTRL_clrNowData();
			log_flag_on();
/*
			MOT_setTrgtSpeed( SEARCH_SPEED );
			MOT_goBlock_FinSpeed(3.0, 0);
			MOT_setTrgtSpeed( SEARCH_SPEED );
*/
//			MOT_turn(MOT_R90);
/*			
			MOT_goBlock_FinSpeed(0.5, SEARCH_SPEED);
			MOT_goSla(MOT_R90S, PARAM_getSra( SLA_90 ));
			MOT_goSla(MOT_L90S, PARAM_getSra( SLA_90 ));
			MOT_goBlock_FinSpeed(0.5, 0);
*/
/*
			MOT_goBlock_FinSpeed(1.0, SEARCH_SPEED);
			MOT_goSla(MOT_R90S, PARAM_getSra( SLA_45 ));
			MOT_goSkewBlock_FinSpeed(0.5, 0);
*/
/*
			MOT_goSkewBlock_FinSpeed(0.5, SEARCH_SPEED);
			MOT_goSla(MOT_R90S, PARAM_getSra( SLA_N90 ));
			MOT_goSkewBlock_FinSpeed(0.5, 0);
*/

			MOT_goBlock_FinSpeed(1.5, SEARCH_SPEED);
			MOT_goSla(MOT_R90S, PARAM_getSra( SLA_135 ));
			MOT_goSkewBlock_FinSpeed(1.0, 0);

			log_flag_off();
			break;

		case MODE_7:
			SetLED(0x0e);
			//cant use
			break;

		default:
			break;
	}
}

void MODE_exe_m1( void )
{
	enMAP_HEAD_DIR		en_endDir2;

	uint64_t data =0;

	GYRO_SetRef();
	CTRL_clrData();
	CTRL_clrAngleErrSum();
	CTRL_clrNowData();

	switch( en_Mode ){

		case MODE_0:
			SetLED(0x0e);
			break;

		case MODE_1:
			SetLED(0x0e);
			map_erase();
			break;

		case MODE_2:
			SetLED(0x0e);
			MAP_showLog();
			break;

		case MODE_3:
			SetLED(0x0e);
			map_copy();
			break;

		case MODE_4:
			SetLED(0x0e);
			map_write();
			break;

		case MODE_5:
			SetLED(0x0e);
			MAP_setPos( 0, 0, NORTH );												// スタート位置
			MAP_Goalsize(1);
			MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
			MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
			MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
			MAP_makeSkewCmdList();													
			MAP_showCmdLog();
			break;

		case MODE_6:
			SetLED(0x0e);
			break;

		case MODE_7:
			SetLED(0x0e);
			//cant use
			break;

		default:
			break;
	}
}

void MODE_exe_m2( void )
{
	enMAP_HEAD_DIR		en_endDir2;

	uint64_t data =0;
	Min_in = 0;
	Sec_in = 0;
	Msec_in = 0;

	GYRO_SetRef();
	CTRL_clrData();
	CTRL_clrAngleErrSum();
	CTRL_clrNowData();

	switch( en_Mode ){

		case MODE_0:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED);
			MOT_setSuraStaSpeed( SEARCH_SPEED );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_SLOW );							
			SetLED(0x00);
			LL_mDelay(100);
			MAP_Goalsize(1);
			MAP_setPos( 0, 0, NORTH );						

			MAP_searchGoal(GOAL_MAP_X_def, GOAL_MAP_Y_def, SEARCH, SEARCH_SURA );			

			if (( SW_IsOn_1() == SW_ON)||(SYS_isOutOfCtrl() == TRUE)){}
			else{
				map_write();
			}
			
			SetLED(0x0e);
			MAP_Goalsize(1);
			SetLED(0x00);

			MAP_searchGoal( 0, 0, SEARCH, SEARCH_SURA );

			if (( SW_IsOn_1() == SW_ON)||(SYS_isOutOfCtrl() == TRUE)){}
			else{
				map_write();
				MAP_setPos( 0, 0, NORTH );								// スタート位置
				MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
				MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
				MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
				MAP_makeSkewCmdList();
				SetLED(0x00);
			}
			break;

		case MODE_1:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED);
			MOT_setSuraStaSpeed( SEARCH_SPEED );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_SLOW );							
			SetLED(0x00);
			LL_mDelay(100);
			MAP_Goalsize(1);
			MAP_setPos( 0, 0, NORTH );							

			MAP_searchGoalKnown(GOAL_MAP_X_def, GOAL_MAP_Y_def, SEARCH, SEARCH_SURA );			

			if (( SW_IsOn_1() == SW_ON)||(SYS_isOutOfCtrl() == TRUE)){}
			else{
				map_write();
			}
			
			SetLED(0x0e);
			MAP_Goalsize(1);
			SetLED(0x00);
			log_flag_on();
			MAP_searchGoalKnown( 0, 0, SEARCH, SEARCH_SURA );
//			MAP_searchGoal( 0, 0, SEARCH, SEARCH_RETURN );
			log_flag_off();
			if (( SW_IsOn_1() == SW_ON)||(SYS_isOutOfCtrl() == TRUE)){}
			else{
				map_write();
				MAP_setPos( 0, 0, NORTH );								// スタート位置
				MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
				MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
				MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
				MAP_makeSkewCmdList();
				SetLED(0x00);
			}
			break;

		case MODE_2:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED);
			MOT_setSuraStaSpeed( SEARCH_SPEED );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_SLOW );							
			SetLED(0x00);
			LL_mDelay(100);
//			PARAM_makeSra( (float)SEARCH_SPEED, 150.0f, 3.00f, SLA_90 );
			MAP_Goalsize(1);
			MAP_setPos( 0, 0, NORTH );							

			MAP_searchGoalKnown(GOAL_MAP_X_def, GOAL_MAP_Y_def, SEARCH, SEARCH_SURA );			

			if (( SW_IsOn_1() == SW_ON)||(SYS_isOutOfCtrl() == TRUE)){}
			else{
				map_write();
			}
			
			SetLED(0x0e);
			MAP_Goalsize(1);
			SetLED(0x00);

			MAP_searchGoalKnown( 0, 0, SEARCH, SEARCH_SURA );
//			MAP_searchGoalKnown( 0, 0, SEARCH, SEARCH_RETURN );

			if (( SW_IsOn_1() == SW_ON)||(SYS_isOutOfCtrl() == TRUE)){}
			else{
				map_write();
				MAP_setPos( 0, 0, NORTH );								// スタート位置
				MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
				MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
				MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
				MAP_makeSkewCmdList();
				SetLED(0x00);
			}
			break;

		case MODE_3:
			SetLED(0x0e);
			
			break;

		case MODE_4:
			SetLED(0x0e);
			break;

		case MODE_5:
			SetLED(0x0e);
			break;

		case MODE_6:
			SetLED(0x0e);
			break;

		case MODE_7:
			SetLED(0x0e);
			//cant use
			break;

		default:
			break;
	}
}

void MODE_exe_m3( void )
{
	enMAP_HEAD_DIR		en_endDir2;

	uint64_t data =0;

	map_copy();

	GYRO_SetRef();
	CTRL_clrData();
	CTRL_clrAngleErrSum();
	CTRL_clrNowData();

	switch( en_Mode ){

		case MODE_0:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED*4.0);
			MOT_setSuraStaSpeed( SEARCH_SPEED );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_SLOW );							
			SetLED(0x00);
			MAP_setPos( 0, 0, NORTH );												// スタート位置
			MAP_Goalsize(1);
			MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
			MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
			MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
			MAP_makeSkewCmdList();													
			LL_mDelay(500);
			Set_DutyTIM8(600);
			LL_mDelay(2000);
			MAP_drive( MAP_DRIVE_SURA );													
			Set_DutyTIM8(0);
			LL_mDelay(500);
			MOT_turn(MOT_R180);
			MAP_actGoalLED();
			Set_DutyTIM8(0);
			Failsafe_flag_off();
			break;

		case MODE_1:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED*5.0);
			MOT_setSuraStaSpeed( SEARCH_SPEED );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_NORMAL );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_NORMAL );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_NORMAL );							
			SetLED(0x00);
			MAP_setPos( 0, 0, NORTH );												// スタート位置
			MAP_Goalsize(1);
			MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
			MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
			MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
			MAP_makeSkewCmdList();													
			LL_mDelay(500);
			Set_DutyTIM8(600);
			LL_mDelay(2000);													
			MAP_drive( MAP_DRIVE_SURA );
			Set_DutyTIM8(0);
			LL_mDelay(500);
			MOT_turn(MOT_R180);
			MAP_actGoalLED();
			Set_DutyTIM8(0);
			Failsafe_flag_off();
			break;

		case MODE_2:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED*6.0);
			MOT_setSuraStaSpeed( SEARCH_SPEED );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_VERY_FAST );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_VERY_FAST );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_VERY_FAST );							
			SetLED(0x00);
			MAP_setPos( 0, 0, NORTH );												// スタート位置
			MAP_Goalsize(1);
			MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
			MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
			MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
			MAP_makeSkewCmdList();													
			LL_mDelay(500);
			Set_DutyTIM8(600);
			LL_mDelay(2000);												
			MAP_drive( MAP_DRIVE_SURA );
			Set_DutyTIM8(0);
			LL_mDelay(500);
			MOT_turn(MOT_R180);
			MAP_actGoalLED();
			Set_DutyTIM8(0);			
			Failsafe_flag_off();
			break;

		case MODE_3:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED*4.0);
			MOT_setSuraStaSpeed( SEARCH_SPEED );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_SLOW );							
			SetLED(0x00);
			MAP_setPos( 0, 0, NORTH );												// スタート位置
			MAP_Goalsize(1);
			MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
			MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
			MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
			MAP_makeSkewCmdList();													
			LL_mDelay(500);
			Set_DutyTIM8(600);
			LL_mDelay(2000);		
			
			log_flag_on();
			MAP_drive( MAP_DRIVE_SKEW );

			log_flag_off();
			Set_DutyTIM8(0);
			LL_mDelay(500);
			MOT_turn(MOT_R180);
			MAP_actGoalLED();
			Set_DutyTIM8(0);
			Failsafe_flag_off();
			break;

		case MODE_4:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED*5.0);
			MOT_setSuraStaSpeed( SEARCH_SPEED );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_NORMAL );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_NORMAL );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_NORMAL );							
			SetLED(0x00);
			MAP_setPos( 0, 0, NORTH );												// スタート位置
			MAP_Goalsize(1);
			MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
			MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
			MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
			MAP_makeSkewCmdList();													
			LL_mDelay(500);
			Set_DutyTIM8(600);
			LL_mDelay(2000);													
			MAP_drive( MAP_DRIVE_SKEW );
			Set_DutyTIM8(0);
			LL_mDelay(500);
			MOT_turn(MOT_R180);
			MAP_actGoalLED();
			Set_DutyTIM8(0);
			Failsafe_flag_off();
			break;

		case MODE_5:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED*6.0);
			MOT_setSuraStaSpeed( SEARCH_SPEED );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_VERY_FAST );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_VERY_FAST );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_VERY_FAST );							
			SetLED(0x00);
			MAP_setPos( 0, 0, NORTH );												// スタート位置
			MAP_Goalsize(1);
			MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
			MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
			MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
			MAP_makeSkewCmdList();													
			LL_mDelay(500);
			Set_DutyTIM8(600);
			LL_mDelay(2000);													
			MAP_drive( MAP_DRIVE_SKEW );
			Set_DutyTIM8(0);
			LL_mDelay(500);
			MOT_turn(MOT_R180);
			MAP_actGoalLED();
			Set_DutyTIM8(0);
			Failsafe_flag_off();
			break;

		case MODE_6:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED*5.0);
			MOT_setSuraStaSpeed( SEARCH_SPEED );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_NORMAL );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_NORMAL );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_NORMAL );							
			SetLED(0x00);
			MAP_setPos( 0, 0, NORTH );												// スタート位置

			MAP_Goal_init();
			MAP_makeContourMap_dijkstra_modoki(GOAL_MAP_X_def,GOAL_MAP_Y_def, BEST_WAY);
			MAP_Goalsize(1);
	
			MAP_makeCmdList_dijkstra_modoki(0, 0, NORTH, GOAL_MAP_X_def,GOAL_MAP_Y_def, &en_endDir2);		// ドライブコマンド作成
			MAP_makeSuraCmdList();													// スラロームコマンド作成
			MAP_makeSkewCmdList();

			LL_mDelay(500);
			Set_DutyTIM8(600);
			LL_mDelay(2000);													
			MAP_drive( MAP_DRIVE_SKEW );
			Set_DutyTIM8(0);
			LL_mDelay(500);
			MOT_turn(MOT_R180);
			MAP_actGoalLED();
			Set_DutyTIM8(0);
			Failsafe_flag_off();

			break;

		case MODE_7:
			SetLED(0x0e);
			//cant use
			break;

		default:
			break;
	}
}

void MODE_exe_m4( void )
{
	enMAP_HEAD_DIR		en_endDir2;

	uint64_t data =0;
	Min_in = 0;
	Sec_in = 0;
	Msec_in = 0;

	GYRO_SetRef();
	CTRL_clrData();
	CTRL_clrAngleErrSum();
	CTRL_clrNowData();

	switch( en_Mode ){

		case MODE_0:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED);
			MOT_setSuraStaSpeed( SEARCH_SPEED );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_SLOW );							
			SetLED(0x00);
			LL_mDelay(100);
			MAP_Goalsize(1);
			MAP_setPos( 0, 0, NORTH );						

			MAP_searchGoal(GOAL_MAP_X_def, GOAL_MAP_Y_def, SEARCH, SEARCH_SURA );			

			if (( SW_IsOn_1() == SW_ON)||(SYS_isOutOfCtrl() == TRUE)){}
			else{
				map_write();
			}
			
			SetLED(0x0e);
			MAP_Goalsize(1);
			SetLED(0x00);

			MAP_searchGoal( 0, 0, SEARCH, SEARCH_SURA );

			if (( SW_IsOn_1() == SW_ON)||(SYS_isOutOfCtrl() == TRUE)){}
			else{
				map_write();
				MAP_setPos( 0, 0, NORTH );								// スタート位置
				MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
				MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
				MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
				MAP_makeSkewCmdList();
				SetLED(0x00);
			}
			break;

		case MODE_1:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED);
			MOT_setSuraStaSpeed( SEARCH_SPEED );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_SLOW );							
			SetLED(0x00);
			LL_mDelay(100);
			MAP_Goalsize(1);
			MAP_setPos( 0, 0, NORTH );							

			MAP_searchGoalKnown(GOAL_MAP_X_def, GOAL_MAP_Y_def, SEARCH, SEARCH_SURA );			

			if (( SW_IsOn_1() == SW_ON)||(SYS_isOutOfCtrl() == TRUE)){}
			else{
				map_write();
			}
			
			SetLED(0x0e);
			MAP_Goalsize(1);
			SetLED(0x00);

			MAP_searchGoalKnown( 0, 0, SEARCH, SEARCH_SURA );
//			MAP_searchGoal( 0, 0, SEARCH, SEARCH_RETURN );

			if (( SW_IsOn_1() == SW_ON)||(SYS_isOutOfCtrl() == TRUE)){}
			else{
				map_write();
				MAP_setPos( 0, 0, NORTH );								// スタート位置
				MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
				MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
				MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
				MAP_makeSkewCmdList();
				SetLED(0x00);
			}
			break;

		case MODE_2:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED);
			MOT_setSuraStaSpeed( SEARCH_SPEED );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_SLOW );							
			SetLED(0x00);
			LL_mDelay(100);
//			PARAM_makeSra( (float)SEARCH_SPEED, 150.0f, 3.00f, SLA_90 );
			MAP_Goalsize(1);
			MAP_setPos( 0, 0, NORTH );							

			MAP_searchGoalKnown(GOAL_MAP_X_def, GOAL_MAP_Y_def, SEARCH, SEARCH_SURA );			

			if (( SW_IsOn_1() == SW_ON)||(SYS_isOutOfCtrl() == TRUE)){}
			else{
				map_write();
			}
			
			SetLED(0x0e);
			MAP_Goalsize(1);
			SetLED(0x00);

			MAP_searchGoalKnown( 0, 0, SEARCH, SEARCH_SURA );
//			MAP_searchGoalKnown( 0, 0, SEARCH, SEARCH_RETURN );

			if (( SW_IsOn_1() == SW_ON)||(SYS_isOutOfCtrl() == TRUE)){}
			else{
				map_write();
				MAP_setPos( 0, 0, NORTH );								// スタート位置
				MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
				MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
				MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
				MAP_makeSkewCmdList();
				SetLED(0x00);
			}
			break;

		case MODE_3:
			SetLED(0x0e);
			
			break;

		case MODE_4:
			SetLED(0x0e);
			break;

		case MODE_5:
			SetLED(0x0e);
			break;

		case MODE_6:
			SetLED(0x0e);
			break;

		case MODE_7:
			SetLED(0x0e);
			//cant use
			break;

		default:
			break;
	}
}

void MODE_exe_m5( void )
{
	enMAP_HEAD_DIR		en_endDir2;

	uint64_t data =0;

	printf("\r\n turn 45 \r\r");
	PARAM_makeSra( 0.6, 550.0f, 7.50f, SLA_45 );
	printf("\r\n turn 90 \r\r");		
	PARAM_makeSra( 0.6, 900.0f, 10.00f, SLA_90 );	
	printf("\r\n turn 135 \r\r");	
	PARAM_makeSra( 0.6, 1350.0f, 13.00f, SLA_135 );	
	printf("\r\n turn N90 \r\r");	
	PARAM_makeSra( 0.6, 1500.0f, 14.00f, SLA_N90 );	

	map_copy();

	GYRO_SetRef();
	CTRL_clrData();
	CTRL_clrAngleErrSum();
	CTRL_clrNowData();

	switch( en_Mode ){

		case MODE_0:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED*4.0);
			MOT_setSuraStaSpeed( 0.6 );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_SLOW );							
			SetLED(0x00);	
			MAP_setPos( 0, 0, NORTH );												// スタート位置
			MAP_Goalsize(1);
			MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
			MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
			MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
			MAP_makeSkewCmdList();													
			LL_mDelay(500);
			Set_DutyTIM8(600);
			LL_mDelay(2000);	
			MAP_drive( MAP_DRIVE_SURA );												
			Set_DutyTIM8(0);
			LL_mDelay(500);
			MOT_turn(MOT_R180);
			MAP_actGoalLED();
			Set_DutyTIM8(0);
			Failsafe_flag_off();
			break;

		case MODE_1:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED*5.0);
			MOT_setSuraStaSpeed( 0.6 );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_NORMAL );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_NORMAL );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_NORMAL );							
			SetLED(0x00);
			MAP_setPos( 0, 0, NORTH );												// スタート位置
			MAP_Goalsize(1);
			MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
			MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
			MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
			MAP_makeSkewCmdList();													
			LL_mDelay(500);
			Set_DutyTIM8(600);
			LL_mDelay(2000);													
			MAP_drive( MAP_DRIVE_SURA );
			Set_DutyTIM8(0);
			LL_mDelay(500);
			MOT_turn(MOT_R180);
			MAP_actGoalLED();
			Set_DutyTIM8(0);
			Failsafe_flag_off();
			break;

		case MODE_2:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED*6.0);
			MOT_setSuraStaSpeed( 0.6 );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_VERY_FAST );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_VERY_FAST );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_VERY_FAST );							
			SetLED(0x00);
			MAP_setPos( 0, 0, NORTH );												// スタート位置
			MAP_Goalsize(1);
			MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
			MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
			MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
			MAP_makeSkewCmdList();													
			LL_mDelay(500);
			Set_DutyTIM8(600);
			LL_mDelay(2000);												
			MAP_drive( MAP_DRIVE_SURA );
			Set_DutyTIM8(0);
			LL_mDelay(500);
			MOT_turn(MOT_R180);
			MAP_actGoalLED();
			Set_DutyTIM8(0);			
			Failsafe_flag_off();
			break;

		case MODE_3:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED*4.0);
			MOT_setSuraStaSpeed( 0.6 );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_SLOW );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_SLOW );							
			SetLED(0x00);
			MAP_setPos( 0, 0, NORTH );												// スタート位置
			MAP_Goalsize(1);
			MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
			MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
			MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
			MAP_makeSkewCmdList();													
			LL_mDelay(500);
			Set_DutyTIM8(600);
			LL_mDelay(2000);			

			log_flag_on();										
			MAP_drive( MAP_DRIVE_SKEW );
			log_flag_off();
			Set_DutyTIM8(0);
			LL_mDelay(500);
			MOT_turn(MOT_R180);
			MAP_actGoalLED();
			Set_DutyTIM8(0);
			Failsafe_flag_off();
			break;

		case MODE_4:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED*5.0);
			MOT_setSuraStaSpeed( 0.6 );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_NORMAL );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_NORMAL );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_NORMAL );							
			SetLED(0x00);
			MAP_setPos( 0, 0, NORTH );												// スタート位置
			MAP_Goalsize(1);
			MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
			MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
			MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
			MAP_makeSkewCmdList();													
			LL_mDelay(500);
			Set_DutyTIM8(600);
			LL_mDelay(2000);													
			MAP_drive( MAP_DRIVE_SKEW );
			Set_DutyTIM8(0);
			LL_mDelay(500);
			MOT_turn(MOT_R180);
			MAP_actGoalLED();
			Set_DutyTIM8(0);
			Failsafe_flag_off();
			break;

		case MODE_5:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED*6.0);
			MOT_setSuraStaSpeed( 0.6 );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_VERY_FAST );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_VERY_FAST );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_VERY_FAST );							
			SetLED(0x00);
			MAP_setPos( 0, 0, NORTH );												// スタート位置
			MAP_Goalsize(1);
			MAP_makeContourMap_run( GOAL_MAP_X_def, GOAL_MAP_Y_def, BEST_WAY );					// 等高線�?�ップを作る
			MAP_makeCmdList( 0, 0, NORTH, GOAL_MAP_X_def, GOAL_MAP_Y_def, &en_endDir2 );		// ドライブコマンド作�??
			MAP_makeSuraCmdList();													// スラロー�?コマンド作�??
			MAP_makeSkewCmdList();													
			LL_mDelay(500);
			Set_DutyTIM8(600);
			LL_mDelay(2000);													
			MAP_drive( MAP_DRIVE_SKEW );
			Set_DutyTIM8(0);
			LL_mDelay(500);
			MOT_turn(MOT_R180);
			MAP_actGoalLED();
			Set_DutyTIM8(0);
			Failsafe_flag_off();
			break;

		case MODE_6:
			SetLED(0x0e);
			MOT_setTrgtSpeed(SEARCH_SPEED*6.0);
			MOT_setSuraStaSpeed( 0.6 );							
			PARAM_setSpeedType( PARAM_ST,   PARAM_VERY_FAST );							
			PARAM_setSpeedType( PARAM_TRUN, PARAM_VERY_FAST );							
			PARAM_setSpeedType( PARAM_SLA,  PARAM_VERY_FAST );							
			SetLED(0x00);
			MAP_setPos( 0, 0, NORTH );												// スタート位置

			MAP_Goal_init();
			MAP_makeContourMap_dijkstra_modoki(GOAL_MAP_X_def,GOAL_MAP_Y_def, BEST_WAY);
			MAP_Goalsize(1);
	
			MAP_makeCmdList_dijkstra_modoki(0, 0, NORTH, GOAL_MAP_X_def,GOAL_MAP_Y_def, &en_endDir2);		// ドライブコマンド作成
			MAP_makeSuraCmdList();													// スラロームコマンド作成
			MAP_makeSkewCmdList();

			LL_mDelay(500);
			Set_DutyTIM8(600);
			LL_mDelay(2000);													
			MAP_drive( MAP_DRIVE_SKEW );
			Set_DutyTIM8(0);
			LL_mDelay(500);
			MOT_turn(MOT_R180);
			MAP_actGoalLED();
			Set_DutyTIM8(0);
			Failsafe_flag_off();
			break;

		case MODE_7:
			SetLED(0x0e);
			//cant use
			break;

		default:
			break;
	}
	printf("\r\n turn 45 \r\r");
	PARAM_makeSra( (float)SEARCH_SPEED, 100.0f, 2.50f, SLA_45 );
	printf("\r\n turn 90 \r\r");		
	PARAM_makeSra( (float)SEARCH_SPEED, 200.0f, 3.50f, SLA_90 );	
//	PARAM_makeSra( (float)SEARCH_SPEED, 150.0f, 3.00f, SLA_90 );
	printf("\r\n turn 135 \r\r");	
	PARAM_makeSra( (float)SEARCH_SPEED, 200.0f, 4.00f, SLA_135 );	
	printf("\r\n turn N90 \r\r");	
	PARAM_makeSra( (float)SEARCH_SPEED, 300.0f, 4.00f, SLA_N90 );	
}


void MODE_exe( void )
{
//	uint16_t *read;
	enMAP_HEAD_DIR		en_endDir;

	now_mode = mode_1;
	uint8_t i;
	LL_mDelay(300);
	GYRO_SetRef();
	ENC_setref();
	Failsafe_flag_off();
//	log_flag_on();	

	switch( en_Mode ){

		case MODE_0://tune	
			SetLED(0x0e);
			en_Mode = MODE_0;	
			LL_mDelay(100);
			SetLED(0x00);
			now_mode = mode_2;
			while(1){
				if ( SW_IsOn_1() == SW_ON ){
					MODE_inc();								
					LL_mDelay(200);			
					printf("mode selecting_0\r\n");
				}
				else if (( SW_IsOn_0() == SW_ON )||(TRUE == MODE_CheckExe())){
					MODE_exe_m0();								
					LL_mDelay(200);				
					if (en_Mode == MODE_7){
						now_mode = mode_1;
						break;
					}
				}

			}
			en_Mode = MODE_0;
			break;

		case MODE_1://flash
			SetLED(0x0e);
			en_Mode = MODE_0;	
			LL_mDelay(100);
			SetLED(0x00);
			now_mode = mode_2;
			while(1){
				if ( SW_IsOn_1() == SW_ON ){
					MODE_inc();								
					LL_mDelay(200);			
					printf("mode selecting_1\r\n");
				}
				else if (( SW_IsOn_0() == SW_ON )||(TRUE == MODE_CheckExe())){
					MODE_exe_m1();								
					LL_mDelay(200);				
					if (en_Mode == MODE_7){
						now_mode = mode_1;
						break;
					}
				}

			}
			en_Mode = MODE_1;
			break;

		case MODE_2://search wall hit
			SetLED(0x0e);
			en_Mode = MODE_0;	
			LL_mDelay(100);
			SetLED(0x00);
			now_mode = mode_2;
			while(1){
				if ( SW_IsOn_1() == SW_ON ){
					MODE_inc();								
					LL_mDelay(200);			
					printf("mode selecting_1\r\n");
				}
				else if (( SW_IsOn_0() == SW_ON )||(TRUE == MODE_CheckExe())){
					MODE_exe_m2();								
					LL_mDelay(200);				
					if (en_Mode == MODE_7){
						now_mode = mode_1;
						break;
					}
				}

			}
			en_Mode = MODE_2;
			break;

		case MODE_3://drive
			SetLED(0x0e);
			en_Mode = MODE_0;	
			LL_mDelay(100);
			SetLED(0x00);
			now_mode = mode_2;
			while(1){
				if ( SW_IsOn_1() == SW_ON ){
					MODE_inc();								
					LL_mDelay(200);			
					printf("mode selecting_1\r\n");
				}
				else if (( SW_IsOn_0() == SW_ON )||(TRUE == MODE_CheckExe())){
					MODE_exe_m3();								
					LL_mDelay(200);				
					if (en_Mode == MODE_7){
						now_mode = mode_1;
						break;
					}
				}

			}
			en_Mode = MODE_3;
			break;

		case MODE_4: // wall hit search
			SetLED(0x0e);
			en_Mode = MODE_0;	
			LL_mDelay(100);
			SetLED(0x00);
			now_mode = mode_2;
			wall_hit_flag = 1;
			while(1){
				if ( SW_IsOn_1() == SW_ON ){
					MODE_inc();								
					LL_mDelay(200);			
					printf("mode selecting_1\r\n");
				}
				else if (( SW_IsOn_0() == SW_ON )||(TRUE == MODE_CheckExe())){
					MODE_exe_m4();								
					LL_mDelay(200);				
					if (en_Mode == MODE_7){
						now_mode = mode_1;
						wall_hit_flag = 0;
						break;
					}
				}

			}
			en_Mode = MODE_4;
			break;

		case MODE_5:
			SetLED(0x0e);
			en_Mode = MODE_0;	
			LL_mDelay(100);
			SetLED(0x00);
			now_mode = mode_2;
			while(1){
				if ( SW_IsOn_1() == SW_ON ){
					MODE_inc();								
					LL_mDelay(200);			
					printf("mode selecting_1\r\n");
				}
				else if (( SW_IsOn_0() == SW_ON )||(TRUE == MODE_CheckExe())){
					MODE_exe_m5();								
					LL_mDelay(200);				
					if (en_Mode == MODE_7){
						now_mode = mode_1;
						break;
					}
				}

			}
			en_Mode = MODE_5;
			break;

		case MODE_6:
			SetLED(0x0e);

			break;

		case MODE_7:
			SetLED(0x0e);
			
			break;


		default:
			break;
	}
}

bool MODE_DistRightCheck(void)
{
	int16_t s_rightval;
	bool bl_check;

	s_rightval = DIST_getNowVal(DIST_SEN_R_FRONT);

	if( s_rightval >= 300 ){
		bl_check=TRUE;
	}
	else{
		bl_check=FALSE;
	}

	return bl_check;
}

bool MODE_DistLeftCheck(void){

	int16_t 	s_leftval;
	bool	bl_check;

	s_leftval 	= DIST_getNowVal(DIST_SEN_L_FRONT);

	if( s_leftval >= 200 ){
		bl_check = TRUE;

	}else{
		bl_check = FALSE;

	}

	return bl_check;
}

bool MODE_setWaitCheck(void){

	bool bl_check;

	if( TRUE == MODE_DistRightCheck() ){	// 右�?け検知
		SetLED(0x08);
	}
	if( TRUE == MODE_DistLeftCheck() ){		// 左�?け検知
		SetLED(0x02);
	}

	if( ( TRUE == MODE_DistRightCheck() ) && ( TRUE == MODE_DistLeftCheck() ) ){
		SetLED(0x0e);
		bl_check = TRUE;

	}else{
		bl_check = FALSE;
	}
	return bl_check;
}

bool MODE_CheckExe(void){

	bool bl_check;

	if( TRUE == MODE_setWaitCheck() ){
		LL_mDelay(500);

		if( FALSE == MODE_setWaitCheck() ){
			SetLED(0x00);
			LL_mDelay(1000);
			bl_check = TRUE;

		}else{
			bl_check = FALSE;

		}

	}else{

		bl_check = FALSE;
	}

	return bl_check;
}


