/*
 * sen_dist.c
 *
 *  Created on: Jan 8, 2021
 *      Author: shohe
 */

#include "hal/sen_dist.h"
#include <string.h>

#define		DIST_REF_NUM		(400)


/* 距離センサ�?報?��前壁�?�み、データフラ�?シュ用構�?体としても使用する?�? */
typedef struct{
	uint16_t		s_wallHit;					///< @var : 壁に当たって�?てもおかしくな�?値         ( AD 値 ) ?��前壁とマウス間が�?2mmの時�?�値?�?					///< @var : 斜め走行時の補正閾値3                    ( AD 値 )
	uint16_t		s_skewErr1;					///< @var : 斜め走行時の補正閾値1                    ( AD 値 )
	uint16_t		s_skewErr2;					///< @var : 斜め走行時の補正閾値2                    ( AD 値 )
	uint16_t		s_skewErr3;					///< @var : 斜め走行時の補正閾値3
}stDIST_FRONT_SEN;


/* 距離センサ�?報?���?�センサ共通、データフラ�?シュ用構�?体�?�みに使用?�? */
typedef struct{
	uint16_t		s_ref;						///< @var : 区画の中�?に置�?た時の距離センサの基準値 ( AD 値 )
	uint16_t		s_limit;					///< @var : 距離センサの閾値                         ( AD 値 ) ( こ�?�値より大きい場合、壁ありと判断する )
	uint16_t		s_ctrl;						///< @var : 制御有効化する際の閾値                   ( AD 値 ) 主に前壁で使用
	uint16_t		s_noCtrl;					///< @var : 壁に近すぎるため制御無効化する際の閾値   ( AD 値 ) 主に前壁で使用
}stDIST_SEN_DATA;

/* 距離センサ�?報?���?�センサ共通�? 注　hal.hに移�?*/
typedef struct{
	uint16_t		s_now;						// LED 点灯中の距離センサの現在値           ( AD 値 )
	uint16_t		s_old;						// LED 点灯中の距離センサの1つ前�?�値        ( AD 値 )
	uint16_t		s_limit;					// 距離センサの閾値                         ( AD 値 ) ( こ�?�値より大きい場合、壁ありと判断する )
	uint16_t		s_ref;						// 区画の中�?に置�?た時の距離センサの基準値 ( AD 値 )
	uint16_t		s_offset;					// LED 消�?�中の距離センサの値               ( AD 値 )
	uint16_t		s_ctrl;						// 制御有効化する際の閾値                   ( AD 値 ) 主に前壁で使用
	uint16_t		s_noCtrl;					// 壁に近すぎるため制御無効化する際の閾値   ( AD 値 ) 主に前壁で使用
}stDIST_SEN;

stDIST_SEN		st_sen[DIST_SEN_NUM];					// 距離センサ
stDIST_FRONT_SEN		st_senF[DIST_SEN_NUM];


void DIST_init( void )
{
	memset( st_sen, 0, sizeof(st_sen) );				// 距離センサ(全センサ共�?)
	st_sen[DIST_SEN_R_FRONT].s_ref       = R_FRONT_REF;
	st_sen[DIST_SEN_L_FRONT].s_ref       = L_FRONT_REF;
	st_sen[DIST_SEN_R_SIDE].s_ref        = R_SIDE_REF;
	st_sen[DIST_SEN_L_SIDE].s_ref        = L_SIDE_REF;
	st_sen[DIST_SEN_R_FRONT].s_limit     = R_FRONT_WALL;
	st_sen[DIST_SEN_L_FRONT].s_limit     = L_FRONT_WALL;
	st_sen[DIST_SEN_R_SIDE].s_limit      = R_SIDE_WALL;
	st_sen[DIST_SEN_L_SIDE].s_limit      = L_SIDE_WALL;
	st_senF[DIST_SEN_R_FRONT].s_skewErr1	= R_FRONT_SKEW_ERR1;
	st_senF[DIST_SEN_L_FRONT].s_skewErr1	= L_FRONT_SKEW_ERR1;
	st_senF[DIST_SEN_R_FRONT].s_skewErr2	= R_FRONT_SKEW_ERR2;
	st_senF[DIST_SEN_L_FRONT].s_skewErr2	= L_FRONT_SKEW_ERR2;
	st_senF[DIST_SEN_R_FRONT].s_skewErr3	= R_FRONT_SKEW_ERR3;
	st_senF[DIST_SEN_L_FRONT].s_skewErr3	= L_FRONT_SKEW_ERR3;
	st_sen[DIST_SEN_R_FRONT].s_noCtrl = R_FRONT_NOCTRL;
	st_sen[DIST_SEN_L_FRONT].s_noCtrl = L_FRONT_NOCTRL;
	st_sen[DIST_SEN_R_FRONT].s_ctrl = R_FRONT_CTRL;
	st_sen[DIST_SEN_L_FRONT].s_ctrl = L_FRONT_CTRL;

}

int16_t DIST_getNowVal( enDIST_SEN_ID en_id )
{
	return st_sen[en_id].s_now;
}


void DIST_getErr( int32_t* p_err )
{
	volatile int16_t	s_threshold_R = 0;		// 右センサの閾値
	volatile int16_t	s_threshold_L = 0;		// 左センサの閾値
	int16_t	s_temp;

	/* ---------- */
	/*  右壁制御  */
	/* ---------- */
	/* 壁�?��?れ目対�? */
	// 急激にセンサの値が変化した場合�?�、壁�?�有無の基準値を閾値に変更する
	s_temp = st_sen[DIST_SEN_R_SIDE].s_now - st_sen[DIST_SEN_R_SIDE].s_old;
	if( ( s_temp < -1 * DIST_NO_WALL_DIV_FILTER ) || ( DIST_NO_WALL_DIV_FILTER < s_temp )
	){
		s_threshold_R = st_sen[DIST_SEN_R_SIDE].s_ref + DIST_REF_UP;		// 基準値?��αを壁�?�存在する閾値にする
	}
	else{
		s_threshold_R = st_sen[DIST_SEN_R_SIDE].s_limit;		// 通常通り
	}

	/* ---------- */
	/*  左壁制御  */
	/* ---------- */
	/* 壁�?��?れ目対�? */
	// 急激にセンサの値が変化した場合�?�、壁�?�有無の基準値を閾値に変更する
	s_temp = st_sen[DIST_SEN_L_SIDE].s_now - st_sen[DIST_SEN_L_SIDE].s_old;
	if( ( s_temp < -1 * DIST_NO_WALL_DIV_FILTER ) || ( DIST_NO_WALL_DIV_FILTER < s_temp )
	){
		s_threshold_L = st_sen[DIST_SEN_L_SIDE].s_ref + DIST_REF_UP;		// 基準値?��αを壁�?�存在する閾値にする
	}
	else{
		s_threshold_L = st_sen[DIST_SEN_L_SIDE].s_limit;		// 通常通り
	}

	/* ------------ */
	/*  制御値算�?�  */
	/* ------------ */
	*p_err = 0;		// クリア

	/* 前壁がも�?�すごく近い�? */
	if( ( st_sen[DIST_SEN_R_FRONT].s_now > st_sen[DIST_SEN_R_FRONT].s_noCtrl ) &&
		( st_sen[DIST_SEN_L_FRONT].s_now > st_sen[DIST_SEN_L_FRONT].s_noCtrl )
	){
//		printf("[Val]%6d 前壁がも�?�すご�?近い 	\n\r", *p_err);
		*p_err = 0;
	}
	/* 前�? */
	else if( ( st_sen[DIST_SEN_R_FRONT].s_now > st_sen[DIST_SEN_R_FRONT].s_ctrl ) &&
		( st_sen[DIST_SEN_L_FRONT].s_now > st_sen[DIST_SEN_L_FRONT].s_ctrl )
	){
		*p_err = ( st_sen[DIST_SEN_L_FRONT].s_now - st_sen[DIST_SEN_L_FRONT].s_ref ) -
				 ( st_sen[DIST_SEN_R_FRONT].s_now - st_sen[DIST_SEN_R_FRONT].s_ref );
//		printf("[Val]%6d 前壁制御 	\n\r", *p_err);
	}
	/* 右壁と左壁あ�? */
	else if( ( s_threshold_R < st_sen[DIST_SEN_R_SIDE].s_now ) && ( s_threshold_L < st_sen[DIST_SEN_L_SIDE].s_now )
	){
		*p_err = ( st_sen[DIST_SEN_R_SIDE].s_now - st_sen[DIST_SEN_R_SIDE].s_ref ) +
				 ( st_sen[DIST_SEN_L_SIDE].s_ref - st_sen[DIST_SEN_L_SIDE].s_now );
//		printf("[Val]%6d 両壁制御 	\n\r", *p_err);
	}
	/* 右壁あ�? */
	else if( s_threshold_R < st_sen[DIST_SEN_R_SIDE].s_now ){
		*p_err = ( st_sen[DIST_SEN_R_SIDE].s_now - st_sen[DIST_SEN_R_SIDE].s_ref ) * 2;
//		printf("[Val]%6d 右壁制御 	\n\r", *p_err);
	}
	/* 左壁あ�? */
	else if( s_threshold_L < st_sen[DIST_SEN_L_SIDE].s_now ){
		*p_err = ( st_sen[DIST_SEN_L_SIDE].s_ref - st_sen[DIST_SEN_L_SIDE].s_now ) * 2;
//		printf("[Val]%6d 左壁制御 	\n\r", *p_err);
	}
}

void DIST_getErrSkew( int32_t* p_err )
{
	*p_err =0;

	/* 進行方向に壁が存在する場合によける動作を行う */
	if( st_sen[DIST_SEN_R_FRONT].s_now > st_senF[DIST_SEN_R_FRONT].s_skewErr3 ){
		*p_err = 300;
//		printf("右前が�?近い  [NOW]%d > [ERR3]%d", st_sen[DIST_SEN_R_FRONT].s_now, st_senF[DIST_SEN_R_FRONT].s_skewErr3 );
	}
	else if( st_sen[DIST_SEN_L_FRONT].s_now > st_senF[DIST_SEN_L_FRONT].s_skewErr3 ){
		*p_err = -300;
//		printf("左前が�?近い  [NOW]%d > [ERR3]%d", st_sen[DIST_SEN_L_FRONT].s_now, st_senF[DIST_SEN_L_FRONT].s_skewErr3 );
	}
	else if( st_sen[DIST_SEN_R_FRONT].s_now > st_senF[DIST_SEN_R_FRONT].s_skewErr2 ){
		*p_err = 200;
//		printf("右前が多少近い  [NOW]%d > [ERR2]%d", st_sen[DIST_SEN_R_FRONT].s_now, st_senF[DIST_SEN_R_FRONT].s_skewErr2 );
	}
	else if( st_sen[DIST_SEN_L_FRONT].s_now > st_senF[DIST_SEN_L_FRONT].s_skewErr2 ){
		*p_err = -200;
//		printf("左前が多少近い  [NOW]%d > [ERR2]%d", st_sen[DIST_SEN_L_FRONT].s_now, st_senF[DIST_SEN_L_FRONT].s_skewErr2 );
	}
	else if( st_sen[DIST_SEN_R_FRONT].s_now > st_senF[DIST_SEN_R_FRONT].s_skewErr1 ){
		*p_err = 100;
//		printf("右前が近い  [NOW]%d > [ERR1]%d", st_sen[DIST_SEN_R_FRONT].s_now, st_senF[DIST_SEN_R_FRONT].s_skewErr1 );
	}
	else if( st_sen[DIST_SEN_L_FRONT].s_now > st_senF[DIST_SEN_L_FRONT].s_skewErr1 ){
		*p_err = -100;
//		printf("左前が近い  [NOW]%d > [ERR1]%d", st_sen[DIST_SEN_L_FRONT].s_now, st_senF[DIST_SEN_L_FRONT].s_skewErr1 );
	}
	else{
	}

}

void DIST_Pol_FL( void )
{
	st_sen[DIST_SEN_L_FRONT].s_offset = GetSensor_FL();

	Set_SenFL(1);

	for(uint16_t i=0;i<400;i++);

	st_sen[DIST_SEN_L_FRONT].s_old = st_sen[DIST_SEN_L_FRONT].s_now;
	st_sen[DIST_SEN_L_FRONT].s_now = GetSensor_FL()- st_sen[DIST_SEN_L_FRONT].s_offset;
	if(st_sen[DIST_SEN_L_FRONT].s_now>(65535/2))st_sen[DIST_SEN_L_FRONT].s_now =0;
	ADC3_clearEOS();

	Set_SenFL(0);
}

void DIST_Pol_FR( void )
{

	st_sen[DIST_SEN_R_FRONT].s_offset = GetSensor_FR();

	Set_SenFR(1);

	for(uint16_t i=0;i<400;i++);

	st_sen[DIST_SEN_R_FRONT].s_old = st_sen[DIST_SEN_R_FRONT].s_now;
	st_sen[DIST_SEN_R_FRONT].s_now = GetSensor_FR()- st_sen[DIST_SEN_R_FRONT].s_offset;
	if(st_sen[DIST_SEN_R_FRONT].s_now>(65535/2))st_sen[DIST_SEN_R_FRONT].s_now =0;
	ADC2_clearEOS();

	Set_SenFR(0);

}


void DIST_Pol_SL( void )
{

	st_sen[DIST_SEN_L_SIDE].s_offset = GetSensor_SL();

	Set_SenSL(1);

	for(uint16_t i=0;i<400;i++);

	st_sen[DIST_SEN_L_SIDE].s_old = st_sen[DIST_SEN_L_SIDE].s_now;
	st_sen[DIST_SEN_L_SIDE].s_now = GetSensor_SL()- st_sen[DIST_SEN_L_SIDE].s_offset;
	if(st_sen[DIST_SEN_L_SIDE].s_now>(65535/2))st_sen[DIST_SEN_L_SIDE].s_now =0;
	ADC1_clearEOS();

	Set_SenSL(0);

}

void DIST_Pol_SR( void )
{

	st_sen[DIST_SEN_R_SIDE].s_offset = GetSensor_SR();

	Set_SenSR(1);

	for(uint16_t i=0;i<400;i++);

	st_sen[DIST_SEN_R_SIDE].s_old = st_sen[DIST_SEN_R_SIDE].s_now;
	st_sen[DIST_SEN_R_SIDE].s_now = GetSensor_SR()- st_sen[DIST_SEN_R_SIDE].s_offset;
	if(st_sen[DIST_SEN_R_SIDE].s_now>(65535/2))st_sen[DIST_SEN_R_SIDE].s_now =0;

	Set_SenSR(0);

}

void Get_Sen_Nowdata(void){
	printf("FL %4d SL %4d SR %4d FR %4d\r",
			st_sen[DIST_SEN_L_FRONT].s_now,st_sen[DIST_SEN_L_SIDE].s_now,st_sen[DIST_SEN_R_SIDE].s_now,st_sen[DIST_SEN_R_FRONT].s_now);
}

bool DIST_isWall_FRONT( void )
{
	bool bl_res 		= false;
//	printf("DIST_SEN_R_FRONT %5d \r\n",st_sen[DIST_SEN_R_FRONT].s_limit);
	if( ( st_sen[DIST_SEN_R_FRONT].s_now > st_sen[DIST_SEN_R_FRONT].s_limit ) ||
		( st_sen[DIST_SEN_L_FRONT].s_now > st_sen[DIST_SEN_L_FRONT].s_limit )
	){
		bl_res = true;
	}

	return bl_res;
}

bool DIST_isWall_R_SIDE( void )
{
	bool bl_res 		= false;

	if( st_sen[DIST_SEN_R_SIDE].s_now > st_sen[DIST_SEN_R_SIDE].s_limit ){
		bl_res = true;
	}

	return bl_res;
}

bool DIST_isWall_L_SIDE( void )
{
	bool bl_res 		= false;

	if( st_sen[DIST_SEN_L_SIDE].s_now > st_sen[DIST_SEN_L_SIDE].s_limit ){
		bl_res = true;
	}

	return bl_res;
}
