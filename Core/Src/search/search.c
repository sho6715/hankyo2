/*
 * search.c
 *
 *  Created on: 2021/02/04
 *      Author: shohe
 */


#include "search/search.h"

#define MAP_SMAP_MAX_VAL	( MAP_X_SIZE * MAP_Y_SIZE ) 			///< ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩmap?øΩÃç≈ëÔøΩl
#define MAP_SMAP_MAX_PRI_VAL	( MAP_SMAP_MAX_VAL * 4 )				///< ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩmap?øΩÃóD?øΩ?øΩx?øΩ≈ëÔøΩl

#define MOVE_BACK_DIST		(0.24f)

enMAP_HEAD_DIR	en_Head;										///< ?øΩ}?øΩE?øΩX?øΩÃêi?øΩs?øΩ?øΩ?øΩ?øΩ 0:N 1:E 2:S 3:W
uint8_t		my;												///< ?øΩ}?øΩE?øΩX?øΩÃÇw?øΩ?øΩ?øΩW
uint8_t		mx;												///< ?øΩ}?øΩE?øΩX?øΩÃÇx?øΩ?øΩ?øΩW
float		f_MoveBackDist;									///< ?øΩ«ìÔøΩ?øΩƒìÔøΩ?øΩ?øΩ≈åÔøΩﬁÇÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ]
uint8_t		uc_SlaCnt = 0;									// ?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩA?øΩ?øΩ?øΩ?øΩ
uint8_t		uc_back[ MAP_Y_SIZE ][ MAP_X_SIZE ];			// ?øΩ?øΩ?øΩH?øΩf?øΩ[?øΩ^

uint8_t		GOAL_MAP_X;					//?øΩS?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩW?øΩœçX?øΩv?øΩ?øΩ?øΩO?øΩ?øΩ?øΩ?øΩ?øΩp?øΩ?øΩ
uint8_t		GOAL_MAP_Y;					//?øΩS?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩW?øΩœçX?øΩv?øΩ?øΩ?øΩO?øΩ?øΩ?øΩ?øΩ?øΩp?øΩ?øΩ

uint8_t		GOAL_SIZE;
//?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ}?øΩb?øΩv?øΩ?øΩ?øΩX?øΩV?øΩ?øΩ?øΩ~?øΩﬂÇÈÇΩ?øΩﬂÇÃà⁄ìÔøΩ?øΩ?øΩ?øΩK?øΩ?øΩœêÔøΩ
uint8_t		uc_max_x = GOAL_MAP_X_def;
uint8_t		uc_max_y = GOAL_MAP_Y_def;

//TKR
/* ?øΩ?øΩ?øΩm?øΩ?øΩ‘âÔøΩ?øΩ?øΩ */
typedef struct{
	uint8_t	uc_StrCnt;
	bool	bl_Known;
}stMAP_KNOWN;

stMAP_KNOWN		st_known = { 0,FALSE };

uint8_t			SLA_count = 10;

void MAP_init( void )
{
//	uint8_t uc_dummy[ MAP_Y_SIZE ][ MAP_X_SIZE ];			// ?øΩ?øΩ?øΩH?øΩf?øΩ[?øΩ^

	/* ?øΩ?øΩ?øΩW?øΩA?øΩ?øΩ?øΩ?øΩ?øΩA?øΩ?øΩ?øΩH?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ */
	en_Head		= NORTH;
	mx		= 0;
	my		= 0;
	MAP_clearMap();
	
	/* ?øΩ?øΩ?øΩs?øΩp?øΩÃÉp?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ^ */
	f_MoveBackDist = 0;
	uc_SlaCnt = 0;

}

void MAP_Goal_init( void )
{
	GOAL_MAP_X = GOAL_MAP_X_def;
	GOAL_MAP_Y = GOAL_MAP_Y_def;
}

void MAP_Goal_change_x( void )
{
//	LED4 = GOAL_MAP_X;
	while(1){
		if ( SW_ON == SW_IsOn_1() ){
			GOAL_MAP_X++;
			if (GOAL_MAP_X == 16){
				GOAL_MAP_X = 0;
			}
//			LED4 = GOAL_MAP_X;
			LL_mDelay(200);			// SW?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ‹Ç≈ë“ÇÔøΩ
			printf("GOAL_x %d\r\n",GOAL_MAP_X);
		} 
		else if (( SW_ON == SW_IsOn_0() )||(TRUE == MODE_CheckExe())){
			SetLED(0x00);
			break;
		}
	}
}

void MAP_Goal_change_y( void )
{
//	LED4 = GOAL_MAP_Y;
	while(1){
		if ( SW_ON == SW_IsOn_1() ){
			GOAL_MAP_Y++;
			if (GOAL_MAP_Y == 16){
				GOAL_MAP_Y = 0;
			}
//			LED4 = GOAL_MAP_Y;
			LL_mDelay(200);			// SW?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ‹Ç≈ë“ÇÔøΩ
			printf("GOAL_y %d\r\n",GOAL_MAP_Y);
		} 
		else if (( SW_ON == SW_IsOn_0() )||(TRUE == MODE_CheckExe())){
			SetLED(0x00);
			break;
		}
	}
}

void MAP_ClearMapData( void )
{
	/* ?øΩ?øΩ?øΩW?øΩA?øΩ?øΩ?øΩ?øΩ?øΩA?øΩ?øΩ?øΩH?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ */
	en_Head		= NORTH;
	mx			= 0;
	my			= 0;
	MAP_clearMap();
	
	/* ?øΩ?øΩ?øΩs?øΩp?øΩÃÉp?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ^ */
	f_MoveBackDist = 0;
	uc_SlaCnt = 0;

//	Storage_Clear( sizeof(g_sysMap), ADR_MAP );			// ?øΩf?øΩ[?øΩ^?øΩZ?øΩ[?øΩu
}

void MAP_setPos( uint8_t uc_x, uint8_t uc_y, enMAP_HEAD_DIR en_dir )
{
	mx		= uc_x;
	my		= uc_y;
	en_Head		= en_dir;
	
	MAP_setCmdPos( uc_x, uc_y, en_dir );

}

void MAP_showLog( void )
{
	int16_t	x,y;
	uint8_t	c_data;
	
	/* ---------- */
	/*  ?øΩ èÔøΩ?øΩ?øΩH  */
	/* ---------- */
	printf("\n\r  /* ---------- */   ");
	printf("\n\r  /*  ?øΩ èÔøΩ?øΩ?øΩH  */   ");
	printf("\n\r  /* ---------- */   ");

	printf("\n\r     ");
	for( x=0; x<MAP_X_SIZE; x++){
		printf("._");
	}
	printf(".\n\r");
	
	for( y=MAP_Y_SIZE-1; y>-1; y-- ){
		
		printf("   %2d",y);
		for( x=0; x<MAP_X_SIZE; x++){
			c_data = (uint8_t)g_sysMap[y][x];
			if ( ( c_data & 0x08 ) == 0 ){
				printf(".");
			}
			else{
				printf("|");
			}
			if ( ( c_data & 0x04 ) == 0 ){
				printf(" ");
			}
			else{
				printf("_");
			}
		}
		printf("|   ");
		
		for( x=0; x<MAP_X_SIZE; x++ ){
			c_data = g_sysMap[y][x];
			c_data = c_data >> 4;
			printf("%x", c_data);
		}
		
		printf("\n\r");
	}
	
	printf("     ");
	for( x=0; x<MAP_X_SIZE; x++){
		printf("%2d",x%10);
	}
	printf("\n\r");

}

void MAP_showcountLog(void)
{
	int16_t	x, y;
	uint16_t	c_data;

	/* ---------- */
	/*  ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩH  */
	/* ---------- */
	printf("\n\r  /* ------------ */   ");
	printf("\n\r  /*  ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩH  */   ");
	printf("\n\r  /* ------------ */   ");

	printf("\n\r");
	for (y = MAP_Y_SIZE - 1; y > -1; y--) {
		for (x = 0; x < MAP_X_SIZE; x++) {
			c_data = (uint16_t)us_cmap[y][x];
			printf("%3d ", c_data);
		}
		printf("\n\r");
	}
}

void MAP_clearMap( void )
{
	uint16_t	x, y;
	uint8_t	uc_data;

	/* ?øΩ?øΩ?øΩ◊ÇƒÇÃÉ}?øΩb?øΩv?øΩf?øΩ[?øΩ^?øΩ?¢íT?øΩ?øΩ?øΩ?øΩ‘Ç…ÇÔøΩ?øΩ?øΩ */
	for ( y = 0; y < MAP_Y_SIZE; y++){
		for( x = 0; x < MAP_X_SIZE; x++){
			uc_data = 0x00;
			if ( ( x == 0) && ( y == 0 ) ) uc_data = 0xfe;
			else if ( ( x == 1 ) && ( y == 0 ) ) uc_data = 0xcc;
			else if ( ( x == (MAP_X_SIZE-1) ) && ( y == 0 ) ) uc_data = 0x66;
			else if ( ( x == 0 ) && ( y == (MAP_Y_SIZE-1) ) ) uc_data = 0x99;
			else if ( ( x == (MAP_X_SIZE-1) ) && ( y == (MAP_Y_SIZE-1) ) ) uc_data = 0x33;
			else if ( x == 0 ) uc_data = 0x88;
			else if ( x == (MAP_X_SIZE-1) ) uc_data = 0x22;
			else if ( y == 0 ) uc_data = 0x44;
			else if ( y == (MAP_Y_SIZE-1) ) uc_data = 0x11;
			g_sysMap[y][x] = uc_data;
		}
	}

}

uint8_t MAP_getWallData( void )
{
	uint8_t	 uc_wall;

//	LED_offAll();			// debug

	// ?øΩZ?øΩ?øΩ?øΩT?øΩ?øΩ?©ÇÔøΩ«èÔøΩ?øΩ?ê¨
	uc_wall = 0;
	if( TRUE == DIST_isWall_FRONT() ){
		uc_wall = uc_wall | 0x11;
//		LED_on(LED3);			// debug
//		LED_on(LED2);			// debug
	}
	if( TRUE == DIST_isWall_L_SIDE() ){
//		LED_on(LED0);			// debug
		uc_wall = uc_wall | 0x88;
	}
	if( TRUE == DIST_isWall_R_SIDE() ){
//		LED_on(LED1);			// debug
		uc_wall = uc_wall | 0x22;
	}

	// ?øΩ}?øΩE?øΩX?øΩÃêi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩ…ÇÔøΩ?øΩ˙ûπ?øΩƒÉZ?øΩ?øΩ?øΩT?øΩf?øΩ[?øΩ^?øΩ?øΩ?øΩ⁄ìÔøΩ?øΩ?øΩ?øΩ«Éf?øΩ[?øΩ^?øΩ∆ÇÔøΩ?øΩ?øΩ
	if		( en_Head == EAST ){
		uc_wall = uc_wall >> 3;
	}
	else if ( en_Head == SOUTH ){
		uc_wall = uc_wall >> 2;
	}
	else if ( en_Head == WEST ){
		uc_wall = uc_wall >> 1;
	}

	//	?øΩT?øΩ?øΩ?øΩœÇ›Ét?øΩ?øΩ?øΩO?øΩ?ßÇƒÇÔøΩ
	return ( uc_wall | 0xf0 );
}

void MAP_makeMapData( void )
{
	uint8_t uc_wall;

	//	?øΩ?øΩ?øΩs?øΩ?øΩ?øΩÃï«èÔøΩ?øΩ?øΩ?øΩ?øΩH?øΩ?øΩ?øΩ…èÔøΩ?øΩ?øΩ
	if ( ( mx == 0 ) && ( my == 0 ) ){
		uc_wall = 0xfe;
	}
	else{
		uc_wall = MAP_getWallData();
	}
	g_sysMap[my][mx] = uc_wall;

	//	?øΩ◊ÇÃãÔøΩ‘ÇÃÇl?øΩ`?øΩo?øΩf?øΩ[?øΩ^?øΩ?øΩ?øΩX?øΩV?øΩ?øΩ?øΩ?øΩ
	if ( mx != (MAP_X_SIZE-1) ){
		g_sysMap[my][mx+1] = ( g_sysMap[my][mx+1] & 0x77 ) | 0x80 | ( ( uc_wall << 2 ) & 0x08 );
	}
	if ( mx !=  0 ){
		g_sysMap[my][mx-1] = ( g_sysMap[my][mx-1] & 0xdd ) | 0x20 | ( ( uc_wall >> 2 ) & 0x02 );
	}
	if ( my != (MAP_Y_SIZE-1) ){
		g_sysMap[my+1][mx] = ( g_sysMap[my+1][mx] & 0xbb ) | 0x40 | ( ( uc_wall << 2 ) & 0x04 );
	}
	if ( my !=  0 ){
		g_sysMap[my-1][mx] = ( g_sysMap[my-1][mx] & 0xee ) | 0x10 | ( ( uc_wall >> 2 ) & 0x01 );
	}

}

void  MAP_makeContourMap( 
	uint8_t uc_goalX, 			///< [in] ?øΩS?øΩ[?øΩ?øΩX?øΩ?øΩ?øΩW
	uint8_t uc_goalY, 			///< [in] ?øΩS?øΩ[?øΩ?øΩY?øΩ?øΩ?øΩW
	enMAP_ACT_MODE	en_type		///< [in] ?øΩv?øΩZ?øΩ?øΩ?øΩ@?øΩi?øΩ‹ÇÔøΩ?øΩ?øΩ?øΩg?øΩp?øΩj
){
	uint16_t		x, y, i;		// ?øΩ?øΩ?øΩ[?øΩv?øΩœêÔøΩ
	uint16_t		uc_dase;		// ?øΩ˚´Ä?øΩl
	uint16_t		uc_new;			// ?øΩV?øΩl
	uint16_t		uc_level;		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
	uint8_t		uc_wallData;	// ?øΩ«èÔøΩ?øΩ

	en_type = en_type;		// ?øΩR?øΩ?øΩ?øΩp?øΩC?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩj?øΩ?øΩ?øΩO?øΩ?øΩ?øΩ?øΩi?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ˙´úÔøΩj

	/* ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ}?øΩb?øΩv?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ */
	for ( i = 0; i < MAP_SMAP_MAX_VAL; i++ ){
		us_cmap[ i / MAP_Y_SIZE][ i & (MAP_X_SIZE-1) ] = MAP_SMAP_MAX_VAL - 1;
	}
	/* ?øΩ⁄ïW?øΩn?øΩ_?øΩÃìÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ0?øΩ…ê›íÔøΩ */
	us_cmap[uc_goalY][uc_goalX] = 0;
	if (GOAL_SIZE == 4) {
		us_cmap[uc_goalY + 1][uc_goalX] = 0;
		us_cmap[uc_goalY][uc_goalX + 1] = 0;
		us_cmap[uc_goalY + 1][uc_goalX + 1] = 0;
	}
	else if (GOAL_SIZE == 9){
		us_cmap[uc_goalY+1][uc_goalX] = 0;
		us_cmap[uc_goalY][uc_goalX+1] = 0;
		us_cmap[uc_goalY+1][uc_goalX+1] = 0;
		us_cmap[uc_goalY+2][uc_goalX] = 0;
		us_cmap[uc_goalY+2][uc_goalX+1] = 0;
		us_cmap[uc_goalY][uc_goalX+2] = 0;
		us_cmap[uc_goalY+1][uc_goalX+2] = 0;
		us_cmap[uc_goalY+2][uc_goalX+2] = 0;
	}

	if (mx > uc_max_x)uc_max_x = mx;
	if (my > uc_max_y)uc_max_y = my;

	/* ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ}?øΩb?øΩv?øΩ?øΩ?øΩ?ê¨ */
	uc_dase = 0;
	do{
		uc_level = 0;
		uc_new = uc_dase + 1;
		for ( y = 0; y < MAP_Y_SIZE; y++ ){
			if (uc_max_y+1 < y) break;
			for ( x = 0; x < MAP_X_SIZE; x++ ){
				if (uc_max_x+1 < x) break;
				if ( us_cmap[y][x] == uc_dase ){
					uc_wallData = g_sysMap[y][x];
					/* ?øΩT?øΩ?øΩ?øΩ?øΩ?øΩs */
					if( SEARCH == en_type ){
						if ( ( ( uc_wallData & 0x01 ) == 0x00 ) && ( y != (MAP_Y_SIZE-1) ) ){
							if ( us_cmap[y+1][x] == MAP_SMAP_MAX_VAL - 1 ){
								us_cmap[y+1][x] = uc_new;
								uc_level++;
							}
						}
						if ( ( ( uc_wallData & 0x02 ) == 0x00 ) && ( x != (MAP_X_SIZE-1) ) ){
							if ( us_cmap[y][x+1] == MAP_SMAP_MAX_VAL - 1 ){
								us_cmap[y][x+1] = uc_new;
								uc_level++;
							}
						}
						if ( ( ( uc_wallData & 0x04 ) == 0x00 ) && ( y != 0 ) ){
							if ( us_cmap[y-1][x] == MAP_SMAP_MAX_VAL - 1 ){
								us_cmap[y-1][x] = uc_new;
								uc_level++;
							}
						}
						if ( ( ( uc_wallData & 0x08 ) == 0x00 ) && ( x != 0 ) ){
							if ( us_cmap[y][x-1] == MAP_SMAP_MAX_VAL - 1 ){
								us_cmap[y][x-1] = uc_new;
								uc_level++;
							}
						}
					}
					/* ?øΩ≈íZ?øΩ?øΩ?øΩs */
					else{
						if ( ( ( uc_wallData & 0x11 ) == 0x10 ) && ( y != (MAP_Y_SIZE-1) ) ){
							if ( us_cmap[y+1][x] == MAP_SMAP_MAX_VAL - 1 ){
								us_cmap[y+1][x] = uc_new;
								uc_level++;
							}
						}
						if ( ( ( uc_wallData & 0x22 ) == 0x20 ) && ( x != (MAP_X_SIZE-1) ) ){
							if ( us_cmap[y][x+1] == MAP_SMAP_MAX_VAL - 1 ){
								us_cmap[y][x+1] = uc_new;
								uc_level++;
							}
						}
						if ( ( ( uc_wallData & 0x44 ) == 0x40 ) && ( y != 0 ) ){
							if ( us_cmap[y-1][x] == MAP_SMAP_MAX_VAL - 1 ){
								us_cmap[y-1][x] = uc_new;
								uc_level++;
							}
						}
						if ( ( ( uc_wallData & 0x88 ) == 0x80 ) && ( x != 0 ) ){
							if ( us_cmap[y][x-1] == MAP_SMAP_MAX_VAL - 1 ){
								us_cmap[y][x-1] = uc_new;
								uc_level++;
							}
						}
					}
					if ((x == mx)&&(y == my))break;
				}
			}
			if ((x == mx)&&(y == my))break;
		}
		if ((x == mx)&&(y == my))break;
		uc_dase = uc_dase + 1;
	}
	while( uc_level != 0 );
	
}

void setStep(const int8_t x, const int8_t y, const uint16_t step) {
	/* (x, y) „Åå„Éï„Ç£„Éº„É´„ÉâÂ??„ÅãÁ¢∫Ë™? */
	if (x < 0 || y < 0 || x >= MAP_X_SIZE || y >= MAP_Y_SIZE) {
		printf( "referred to out of field\r\n");
		return;
	}
	us_cmap[y][x] = step;
}

void  MAP_makeContourMap_kai2(
	uint8_t uc_goalX, 			///< [in] „Ç¥„Éº„É´XÂ∫ßÊ®?
	uint8_t uc_goalY, 			///< [in] „Ç¥„Éº„É´YÂ∫ßÊ®?
	enMAP_ACT_MODE	en_type		///< [in] Ë®àÁÆóÊñπÊ≥ïÔºà„Åæ„Å?Êú™‰ΩøÁî®?º?
) {
	uint16_t		x, y, i;		// „É´„Éº„ÉóÂ§âÊï∞
	uint16_t		uc_dase;		// Âü∫Ê∫ñÂÄ§
	uint16_t		uc_new;			// Êñ∞ÂÄ§
	uint16_t		uc_level;		// Á≠âÈ´òÁ∑?
	uint8_t		uc_wallData;	// Â£ÅÊÉÖÂ†±

	stPOSITION		st_pos;

	en_type = en_type;		// „Ç≥„É≥„Éë„Ç§„É´„ÉØ„Éº„Éã„É≥„Ç∞ÂõûÈÅø?ºà„ÅÑ„Åö„ÇåÂâäÈô§?º?

	queue_t queue;
	queue_t* pQueue = &queue;

	initQueue(pQueue);

	/* Á≠âÈ´òÁ∑ö„?û„ÉÉ„Éó„ÇíÂàùÊúüÂåñ„Åô„Ç? */
	for (i = 0; i < MAP_SMAP_MAX_VAL; i++) {
		us_cmap[i / MAP_Y_SIZE][i & (MAP_X_SIZE - 1)] = MAP_SMAP_MAX_VAL - 1;
	}

	/* „Çπ„É?„É?„Éó„?ÆÊõ¥Êñ∞‰∫àÁ¥?„ÅÆ„Ç≠„É•„Éº */
//	std::queue<stPOSITION> q;
//	QueryPerformanceCounter(&start);

	/* ÁõÆÊ®ôÂú∞ÁÇπ„ÅÆÁ≠âÈ´òÁ∑ö„Çí0„Å´Ë®≠ÂÆ? */
	setStep(uc_goalX, uc_goalY, 0);
	st_pos.x = uc_goalX;
	st_pos.y = uc_goalY;
	st_pos.step = 0;

	enqueue(pQueue,st_pos);

	/* Á≠âÈ´òÁ∑ö„?û„ÉÉ„Éó„Çí‰ΩúÊ?? */
	while (pQueue->flag != EMPTY) {
		const stPOSITION focus = dequeue(pQueue);
//		q.pop();
		const uint16_t focus_step = focus.step;
		x = focus.x;
		y = focus.y;
		stPOSITION next = focus;
		uc_wallData = g_sysMap[y][x];

		if (((uc_wallData & 0x01) == 0x00) && (y != (MAP_Y_SIZE - 1))) {
			if (us_cmap[y + 1][x] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y + 1][x] = focus_step + 1;
				next.x = x;
				next.y = y + 1;
				enqueue(pQueue,next);
			}
		}
		if (((uc_wallData & 0x02) == 0x00) && (x != (MAP_X_SIZE - 1))) {
			if (us_cmap[y][x + 1] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y][x + 1] = focus_step + 1;
				next.x = x + 1;
				next.y = y;
				enqueue(pQueue, next);
			}
		}
		if (((uc_wallData & 0x04) == 0x00) && (y != 0)) {
			if (us_cmap[y - 1][x] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y - 1][x] = focus_step + 1;
				next.x = x;
				next.y = y - 1;
				enqueue(pQueue, next);
			}
		}
		if (((uc_wallData & 0x08) == 0x00) && (x != 0)) {
			if (us_cmap[y][x - 1] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y][x - 1] = focus_step + 1;
				next.x = x - 1;
				next.y = y;
				enqueue(pQueue, next);
			}
		}

	}

}

void  MAP_makeContourMap_run( 
	uint8_t uc_goalX, 			///< [in] ?øΩS?øΩ[?øΩ?øΩX?øΩ?øΩ?øΩW
	uint8_t uc_goalY, 			///< [in] ?øΩS?øΩ[?øΩ?øΩY?øΩ?øΩ?øΩW
	enMAP_ACT_MODE	en_type		///< [in] ?øΩv?øΩZ?øΩ?øΩ?øΩ@?øΩi?øΩ‹ÇÔøΩ?øΩ?øΩ?øΩg?øΩp?øΩj
){
	uint16_t		x, y, i;		// ?øΩ?øΩ?øΩ[?øΩv?øΩœêÔøΩ
	uint16_t		uc_dase;		// ?øΩ˚´Ä?øΩl
	uint16_t		uc_new;			// ?øΩV?øΩl
	uint16_t		uc_level;		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
	uint8_t		uc_wallData;	// ?øΩ«èÔøΩ?øΩ

	en_type = en_type;		// ?øΩR?øΩ?øΩ?øΩp?øΩC?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩj?øΩ?øΩ?øΩO?øΩ?øΩ?øΩ?øΩi?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ˙´úÔøΩj

	/* ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ}?øΩb?øΩv?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ */
	for ( i = 0; i < MAP_SMAP_MAX_VAL; i++ ){
		us_cmap[ i / MAP_Y_SIZE][ i & (MAP_X_SIZE-1) ] = MAP_SMAP_MAX_VAL - 1;
	}
	/* ?øΩ⁄ïW?øΩn?øΩ_?øΩÃìÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ0?øΩ…ê›íÔøΩ */
	us_cmap[uc_goalY][uc_goalX] = 0;
	if (GOAL_SIZE == 4) {
		us_cmap[uc_goalY + 1][uc_goalX] = 0;
		us_cmap[uc_goalY][uc_goalX + 1] = 0;
		us_cmap[uc_goalY + 1][uc_goalX + 1] = 0;
	}
	else if (GOAL_SIZE == 9){
		us_cmap[uc_goalY+1][uc_goalX] = 0;
		us_cmap[uc_goalY][uc_goalX+1] = 0;
		us_cmap[uc_goalY+1][uc_goalX+1] = 0;
		us_cmap[uc_goalY+2][uc_goalX] = 0;
		us_cmap[uc_goalY+2][uc_goalX+1] = 0;
		us_cmap[uc_goalY][uc_goalX+2] = 0;
		us_cmap[uc_goalY+1][uc_goalX+2] = 0;
		us_cmap[uc_goalY+2][uc_goalX+2] = 0;
	}

//	if (mx > uc_max_x)uc_max_x = mx;
//	if (my > uc_max_y)uc_max_y = my;

	/* ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ}?øΩb?øΩv?øΩ?øΩ?øΩ?ê¨ */
	uc_dase = 0;
	do{
		uc_level = 0;
		uc_new = uc_dase + 1;
		for ( y = 0; y < MAP_Y_SIZE; y++ ){
//			if (uc_max_y+1 < y) break;
			for ( x = 0; x < MAP_X_SIZE; x++ ){
//				if (uc_max_x+1 < x) break;
				if ( us_cmap[y][x] == uc_dase ){
					uc_wallData = g_sysMap[y][x];
					/* ?øΩT?øΩ?øΩ?øΩ?øΩ?øΩs */
					if( SEARCH == en_type ){
						if ( ( ( uc_wallData & 0x01 ) == 0x00 ) && ( y != (MAP_Y_SIZE-1) ) ){
							if ( us_cmap[y+1][x] == MAP_SMAP_MAX_VAL - 1 ){
								us_cmap[y+1][x] = uc_new;
								uc_level++;
							}
						}
						if ( ( ( uc_wallData & 0x02 ) == 0x00 ) && ( x != (MAP_X_SIZE-1) ) ){
							if ( us_cmap[y][x+1] == MAP_SMAP_MAX_VAL - 1 ){
								us_cmap[y][x+1] = uc_new;
								uc_level++;
							}
						}
						if ( ( ( uc_wallData & 0x04 ) == 0x00 ) && ( y != 0 ) ){
							if ( us_cmap[y-1][x] == MAP_SMAP_MAX_VAL - 1 ){
								us_cmap[y-1][x] = uc_new;
								uc_level++;
							}
						}
						if ( ( ( uc_wallData & 0x08 ) == 0x00 ) && ( x != 0 ) ){
							if ( us_cmap[y][x-1] == MAP_SMAP_MAX_VAL - 1 ){
								us_cmap[y][x-1] = uc_new;
								uc_level++;
							}
						}
					}
					/* ?øΩ≈íZ?øΩ?øΩ?øΩs */
					else{
						if ( ( ( uc_wallData & 0x11 ) == 0x10 ) && ( y != (MAP_Y_SIZE-1) ) ){
							if ( us_cmap[y+1][x] == MAP_SMAP_MAX_VAL - 1 ){
								us_cmap[y+1][x] = uc_new;
								uc_level++;
							}
						}
						if ( ( ( uc_wallData & 0x22 ) == 0x20 ) && ( x != (MAP_X_SIZE-1) ) ){
							if ( us_cmap[y][x+1] == MAP_SMAP_MAX_VAL - 1 ){
								us_cmap[y][x+1] = uc_new;
								uc_level++;
							}
						}
						if ( ( ( uc_wallData & 0x44 ) == 0x40 ) && ( y != 0 ) ){
							if ( us_cmap[y-1][x] == MAP_SMAP_MAX_VAL - 1 ){
								us_cmap[y-1][x] = uc_new;
								uc_level++;
							}
						}
						if ( ( ( uc_wallData & 0x88 ) == 0x80 ) && ( x != 0 ) ){
							if ( us_cmap[y][x-1] == MAP_SMAP_MAX_VAL - 1 ){
								us_cmap[y][x-1] = uc_new;
								uc_level++;
							}
						}
					}
//					if ((x == mx)&&(y == my))break;
				}
			}
//			if ((x == mx)&&(y == my))break;
		}
//		if ((x == mx)&&(y == my))break;
		uc_dase = uc_dase + 1;
	}
	while( uc_level != 0 );
	
}

void MAP_calcMouseDir( 
	enMAP_SEARCH_TYPE	en_calcType,	///< [in] ?øΩv?øΩZ?øΩ?øΩ?øΩ@
	enMAP_HEAD_DIR* 	p_head			///< [out] ?øΩi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩi?øΩﬂÇÔøΩl?øΩj
){
	uint8_t		uc_wall;				// ?øΩ«èÔøΩ?øΩ
	uint16_t		us_base;				// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩD?øΩ?øΩx?øΩ?øΩ?øΩ?øΩl
	uint16_t		us_new;
	enMAP_HEAD_DIR	en_tmpHead;

	/* ?øΩ?øΩ?øΩ?øΩ?øΩv?øΩZ */
	// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩMAP?øΩ@
	if( CONTOUR_SYSTEM == en_calcType ){
		// ?øΩ?øΩ?øΩ”ÇÔøΩ4?øΩ?øΩ?øΩ≈àÔøΩ‘ñ⁄ìI?øΩn?øΩ…ãﬂÇÔøΩ?øΩ⁄ìÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩZ?øΩo?øΩ?øΩ?øΩ?øΩB
		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩA?øΩ⁄ìÔøΩ?øΩ≈ÇÔøΩ?øΩ?øΩ?øΩ‘ãﬂÇÔøΩ?øΩ?øΩ‘ÇÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩÍç??øΩ…ÇÕÅA?øΩ?øΩ?øΩÃèÔøΩ?øΩ≈ëI?øΩ?øΩ?øΩ?øΩ?øΩ?øΩB
		// ?øΩ@?øΩ?øΩ?øΩT?øΩ?øΩ?øΩ?øΩ?øΩ,?øΩ?øΩ?øΩi ?øΩA?øΩ?øΩ?øΩT?øΩ?øΩ?øΩ?øΩ?øΩ,?øΩ?øΩ?øΩ?øΩ ?øΩB?øΩ?øΩ?øΩT?øΩ?øΩ?øΩ?øΩ?øΩ,?øΩ?øΩ?øΩi ?øΩC?øΩ?øΩ?øΩT?øΩ?øΩ?øΩ?øΩ?øΩ,?øΩ?øΩ?øΩ?øΩ
		uc_wall = g_sysMap[my][mx];
		us_base = MAP_SMAP_MAX_PRI_VAL;					// 16[?øΩ?øΩ?øΩ]?øΩ~16[?øΩ?øΩ?øΩ]?øΩ~4[?øΩ?øΩ?øΩ?øΩ]

		/* 4?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩr */
		//	?øΩk?øΩ?øΩ?øΩ?øΩ?øΩÃãÔøΩ?øΩÃäm?øΩF
		if ( ( uc_wall & 1 ) == 0 ){
			us_new = us_cmap[my+1][mx] * 4 + 4;
			if ( ( g_sysMap[my+1][mx] & 0xf0 ) != 0xf0 ) us_new = us_new - 2;
			if ( en_Head == NORTH ) us_new = us_new - 1;
			if ( us_new < us_base ){
				us_base = us_new;
				en_tmpHead = NORTH;
			}
		}
		//	?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩÃãÔøΩ?øΩÃäm?øΩF
		if ( ( uc_wall & 2 ) == 0 ){
			us_new = us_cmap[my][mx+1] * 4 + 4;
			if ( ( g_sysMap[my][mx+1] & 0xf0 ) != 0xf0 ) us_new = us_new - 2;
			if ( en_Head == EAST) us_new = us_new - 1;
			if ( us_new < us_base ){
				us_base = us_new;
				en_tmpHead = EAST;
			}
		}
		//	?øΩ?øΩ?øΩ?øΩ?øΩ?øΩÃãÔøΩ?øΩÃäm?øΩF
		if ( ( uc_wall & 4 ) == 0 ){
			us_new = us_cmap[my-1][mx] * 4 + 4;
			if ( ( g_sysMap[my-1][mx] & 0xf0 ) != 0xf0) us_new = us_new - 2;
			if ( en_Head == SOUTH ) us_new = us_new - 1;
			if ( us_new < us_base ){
				us_base = us_new;
				en_tmpHead = SOUTH;
			}
		}
		//	?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩÃãÔøΩ?øΩÃäm?øΩF
		if ( ( uc_wall & 8 ) == 0 ){
			us_new = us_cmap[my][mx-1] * 4 + 4;
			if ( ( g_sysMap[my][mx-1] & 0xf0 ) != 0xf0 ) us_new = us_new - 2;
			if ( en_Head == WEST ) us_new = us_new - 1;
			if ( us_new < us_base ){
				us_base = us_new;
				en_tmpHead = WEST;
			}
		}
		
		*p_head = (enMAP_HEAD_DIR)( (en_tmpHead - en_Head) & 3 );		// ?øΩ⁄ìÔøΩ?øΩ?øΩ?øΩ?øΩ
		
	}
	// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ@?øΩw?øΩ?øΩ»ÇÔøΩ
	else{
		*p_head = (enMAP_HEAD_DIR)0;
	}

}

void MAP_refMousePos( 
	enMAP_HEAD_DIR 			en_head			///< [in] ?øΩi?øΩs?øΩ?øΩ?øΩ?øΩ
){
	switch( en_head ){
		case NORTH:
			my = my + 1;
			break;
		case EAST:
			mx = mx + 1;
			break;
		case SOUTH:
			my = my - 1;
			break;
		case WEST:
			mx = mx - 1;
			break;
		default:
			break;
	}
}

void MAP_moveNextBlock( 
	enMAP_HEAD_DIR 	en_head,		///< [in] ?øΩ?øΩ?øΩŒêi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩi?øΩ}?øΩE?øΩX?øΩi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩ?øΩk?øΩ∆ÇÔøΩ?øΩƒÇÔøΩ?øΩ?øΩj
	bool*			p_type			///< [in] FALSE: ?øΩP?øΩ?øΩ‘ëO?øΩi?øΩ?øΩ‘ÅATURE:?øΩ?øΩ?øΩ?øΩ‘ëO?øΩi?øΩ?øΩ?øΩ
){
	*p_type = TRUE;
	f_MoveBackDist = 0;				// ?øΩ⁄ìÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩZ?øΩl?øΩN?øΩ?øΩ?øΩA
	
	/* ?øΩ?øΩ?øΩ?øΩ */
	switch( en_head ){

		/* ?øΩ?øΩ?øΩÃÇ‹Ç‹ëO?øΩi */
		case NORTH:
			*p_type = FALSE;
			MOT_goBlock_Const( 1 );				// 1?øΩ?øΩ?øΩO?øΩi
			break;
		// ?øΩE?øΩ…êÔøΩ?øΩ?∑ÇÔøΩ
		case EAST:
			MOT_goBlock_FinSpeed( 0.5, 0 );		// ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩi
			MOT_turn(MOT_R90);									// ?øΩE90?øΩx?øΩ?øΩ?øΩ?øΩ
			break;
		// ?øΩ?øΩ?øΩ…êÔøΩ?øΩ?∑ÇÔøΩ
		case WEST:
			MOT_goBlock_FinSpeed( 0.5, 0 );		// ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩi
			MOT_turn(MOT_L90);									// ?øΩE90?øΩx?øΩ?øΩ?øΩ?øΩ
			break;
		// ?øΩ?øΩ?øΩ]?øΩ?øΩ?øΩƒñﬂÇÔøΩ
		case SOUTH:
			MOT_goBlock_FinSpeed( 0.5, 0 );		// ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩi
			MOT_turn(MOT_R180);									// ?øΩE180?øΩx?øΩ?øΩ?øΩ?øΩ
			
			/* ?øΩ«ìÔøΩ?øΩƒép?øΩ?øΩ?øΩ?øΩ?øΩ?øΩi?øΩ?øΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩo?øΩb?øΩN?øΩ{?øΩ⁄ìÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩZ?øΩ?øΩ?øΩ?øΩj */
			if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) )  ||		// ?øΩk?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒñk?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
				( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
				( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
				( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) ) 			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒêÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
			){
				MOT_goHitBackWall();					// ?øΩo?øΩb?øΩN?øΩ?øΩ?øΩ?øΩ
				f_MoveBackDist = MOVE_BACK_DIST;		// ?øΩo?øΩb?øΩN?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩÃà⁄ìÔøΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ]?øΩ?øΩ?øΩ?øΩ?øΩZ
			}
			break;
		default:
			break;
	}
	
	/* ?øΩO?øΩi?øΩ?øΩ?øΩ…Ép?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩX?øΩ@?øΩ\?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒ??øΩ?øΩW?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩ»ÇÔøΩ?øΩ?øΩŒÇ»ÇÔøΩ»ÇÔøΩ */
/*	if( ( TRUE == DCMC_isPowerRelease() ) && ( en_head == NORTH ) ){
		
		MOT_goBack_Const( MOT_BACK_POLE );					// ?øΩP?øΩ¬ëO?øΩÃíÔøΩ?øΩ‹Ç≈åÔøΩ?øΩ
		MAP_makeMapData();									// ?øΩ«Éf?øΩ[?øΩ^?øΩ?øΩ?øΩ?øΩ?øΩ?øΩH?øΩf?øΩ[?øΩ^?øΩ?øΩ?øΩ?ê¨			?øΩ?øΩ ?øΩ?øΩ?øΩ?øΩ?øΩ≈Éf?øΩ[?øΩ^?øΩ?ê¨?øΩ?øΩ?øΩ~?øΩX?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩ
		MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩMAP?øΩ@?øΩ≈êi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩZ?øΩo			?øΩ?øΩ ?øΩ?øΩ?øΩ?øΩ?øΩMAP?øΩ?øΩ?øΩ?ê¨
		MAP_moveNextBlock(en_head, p_type);					// ?øΩ?øΩ?øΩ?øΩ?øΩP?øΩx?øΩƒÇ—èo?øΩ?øΩ?øΩi?øΩ?øΩ?øΩÃãÔøΩ?øΩ÷à⁄ìÔøΩ?øΩj
	}
	else{*/
		/* ?øΩi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩX?øΩV */
		en_Head = (enMAP_HEAD_DIR)( (en_Head + en_head) & (MAP_HEAD_DIR_MAX-1) );
//	}
}

void MAP_moveNextBlock_Sura( 
	enMAP_HEAD_DIR 	en_head,		///< [in] ?øΩ?øΩ?øΩŒêi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩi?øΩ}?øΩE?øΩX?øΩi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩ?øΩk?øΩ∆ÇÔøΩ?øΩƒÇÔøΩ?øΩ?øΩj
	bool*			p_type,			///< [in] FALSE: ?øΩP?øΩ?øΩ‘ëO?øΩi?øΩ?øΩ‘ÅATURE:?øΩ?øΩ?øΩ?øΩ‘ëO?øΩi?øΩ?øΩ?øΩ
	bool			bl_resume		///< [in] FALSE: ?øΩ?øΩ?øΩW?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ≈ÇÕÇ»ÇÔøΩ?øΩATURE:?øΩ?øΩ?øΩW?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
){
	*p_type = FALSE;
	f_MoveBackDist = 0;				// ?øΩ⁄ìÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩZ?øΩl?øΩN?øΩ?øΩ?øΩA
	
	/* ?øΩ?øΩ?øΩ?øΩ */
	switch( en_head ){

		// ?øΩ?øΩ?øΩÃÇ‹Ç‹ëO?øΩi
		case NORTH:
			
			/* ?øΩ?øΩ?øΩW?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ≈ÇÕÇ»ÇÔøΩ */
			if( bl_resume == FALSE ){
		
				MOT_goBlock_Const( 1 );					// 1?øΩ?øΩ?øΩO?øΩi
//				uc_SlaCnt = 0;							// ?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ»ÇÔøΩ
			}
			/* ?øΩ?øΩ?øΩW?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ */
			else{
				MOT_goBlock_FinSpeed( 1.0f, SEARCH_SPEED );		// ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩi(?øΩo?øΩb?øΩN?øΩÃà⁄ìÔøΩ?øΩ ÇÔøΩ?øΩ‹ÇÔøΩ)
//				uc_SlaCnt = 0;										// ?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ»ÇÔøΩ
			}
			break;

		// ?øΩE?øΩ…ÉX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
		case EAST:
			if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?øΩk?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒêÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) ) 			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒñk?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
				){
				uc_dist_control = 0.01;
				}
			else{
				uc_dist_control = 0;
			}
			if( uc_SlaCnt < SLA_count ){
				MOT_goSla( MOT_R90S, PARAM_getSra( SLA_90 ) );	// ?øΩE?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ
				uc_SlaCnt++;
			}
			else{
				/* ?øΩ«ìÔøΩ?øΩƒép?øΩ?øΩ?øΩ?øΩ?øΩ?øΩi?øΩ?øΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩo?øΩb?øΩN?øΩ{?øΩ⁄ìÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩZ?øΩ?øΩ?øΩ?øΩj */
				if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?øΩk?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒêÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒñk?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) ) 			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
				){
					MOT_goBlock_FinSpeed( 0.5, 0 );			// ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩi
					MOT_turn(MOT_R90);						// ?øΩE90?øΩx?øΩ?øΩ?øΩ?øΩ
					uc_SlaCnt = 0;
					MOT_goHitBackWall();					// ?øΩo?øΩb?øΩN?øΩ?øΩ?øΩ?øΩ
					f_MoveBackDist = MOVE_BACK_DIST;		// ?øΩo?øΩb?øΩN?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩÃà⁄ìÔøΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ]?øΩ?øΩ?øΩ?øΩ?øΩZ
					*p_type = TRUE;							// ?øΩ?øΩ?øΩÕîÔøΩ?øΩ?øΩ‘Åi?øΩ{?øΩo?øΩb?øΩN?øΩj?øΩ?øΩ?øΩi?øΩﬂÇÔøΩ
				}
				else{
					MOT_goSla( MOT_R90S, PARAM_getSra( SLA_90 ) );	// ?øΩE?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ
					uc_SlaCnt++;
				}
			}
			break;

		// ?øΩ?øΩ?øΩ…ÉX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
		case WEST:
			if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?øΩk?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒêÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒñk?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) ) 			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
				){
				uc_dist_control = 0.01;
				}
			else{
				uc_dist_control = 0;
			}
			if( uc_SlaCnt < SLA_count ){
				MOT_goSla( MOT_L90S, PARAM_getSra( SLA_90 ) );	// ?øΩ?øΩ?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ
				uc_SlaCnt++;
			}
			else{
				/* ?øΩ«ìÔøΩ?øΩƒép?øΩ?øΩ?øΩ?øΩ?øΩ?øΩi?øΩ?øΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩo?øΩb?øΩN?øΩ{?øΩ⁄ìÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩZ?øΩ?øΩ?øΩ?øΩj */
				if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?øΩk?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒêÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) ) 			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒñk?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
				){
					MOT_goBlock_FinSpeed( 0.5, 0 );		// ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩi
					MOT_turn(MOT_L90);					// ?øΩE90?øΩx?øΩ?øΩ?øΩ?øΩ
					uc_SlaCnt = 0;
					MOT_goHitBackWall();					// ?øΩo?øΩb?øΩN?øΩ?øΩ?øΩ?øΩ
					f_MoveBackDist = MOVE_BACK_DIST;		// ?øΩo?øΩb?øΩN?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩÃà⁄ìÔøΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ]?øΩ?øΩ?øΩ?øΩ?øΩZ
					*p_type = TRUE;							// ?øΩ?øΩ?øΩÕîÔøΩ?øΩ?øΩ‘Åi?øΩ{?øΩo?øΩb?øΩN?øΩj?øΩ?øΩ?øΩi?øΩﬂÇÔøΩ
				}
				else{
					MOT_goSla( MOT_L90S, PARAM_getSra( SLA_90 ) );	// ?øΩ?øΩ?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ
					uc_SlaCnt++;
				}
			}
			break;

		// ?øΩ?øΩ?øΩ]?øΩ?øΩ?øΩƒñﬂÇÔøΩ
		case SOUTH:
			MOT_goBlock_FinSpeed( 0.5, 0 );			// ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩi
			MOT_turn(MOT_R180);									// ?øΩE180?øΩx?øΩ?øΩ?øΩ?øΩ
			uc_SlaCnt = 0;
			
			/* ?øΩ«ìÔøΩ?øΩƒép?øΩ?øΩ?øΩ?øΩ?øΩ?øΩi?øΩ?øΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩo?øΩb?øΩN?øΩ{?øΩ⁄ìÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩZ?øΩ?øΩ?øΩ?øΩj */
			if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) )  ||		// ?øΩk?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒñk?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
				( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
				( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
				( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) ) 			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒêÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
			){
				MOT_goHitBackWall();					// ?øΩo?øΩb?øΩN?øΩ?øΩ?øΩ?øΩ
				f_MoveBackDist = MOVE_BACK_DIST;		// ?øΩo?øΩb?øΩN?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩÃà⁄ìÔøΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ]?øΩ?øΩ?øΩ?øΩ?øΩZ
			}
			*p_type = TRUE;								// ?øΩ?øΩ?øΩÕîÔøΩ?øΩ?øΩ‘Å{?øΩo?øΩb?øΩN?øΩ?øΩ?øΩi?øΩﬂÇÔøΩ
			break;
			
		default:
			break;
	}
	
	/* ?øΩO?øΩi?øΩ?øΩ?øΩ…Ép?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩX?øΩ@?øΩ\?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒ??øΩ?øΩW?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩ»ÇÔøΩ?øΩ?øΩŒÇ»ÇÔøΩ»ÇÔøΩ */
/*	if( ( TRUE == DCMC_isPowerRelease() ) && ( en_head == NORTH ) ){
		
		MOT_goBack_Const( MOT_BACK_POLE );					// ?øΩP?øΩ¬ëO?øΩÃíÔøΩ?øΩ‹Ç≈åÔøΩ?øΩ
		MAP_makeMapData();									// ?øΩ«Éf?øΩ[?øΩ^?øΩ?øΩ?øΩ?øΩ?øΩ?øΩH?øΩf?øΩ[?øΩ^?øΩ?øΩ?øΩ?ê¨			?øΩ?øΩ ?øΩ?øΩ?øΩ?øΩ?øΩ≈Éf?øΩ[?øΩ^?øΩ?ê¨?øΩ?øΩ?øΩ~?øΩX?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩ
		MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩMAP?øΩ@?øΩ≈êi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩZ?øΩo			?øΩ?øΩ ?øΩ?øΩ?øΩ?øΩ?øΩMAP?øΩ?øΩ?øΩ?ê¨
		MAP_moveNextBlock_Sura(en_head, p_type, TRUE );		// ?øΩ?øΩ?øΩ?øΩ?øΩP?øΩx?øΩƒÇ—èo?øΩ?øΩ?øΩi?øΩ?øΩ?øΩÃãÔøΩ?øΩ÷à⁄ìÔøΩ?øΩj
	}
	else{*/
		/* ?øΩi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩX?øΩV */
		en_Head = (enMAP_HEAD_DIR)( (en_Head + en_head) & (MAP_HEAD_DIR_MAX-1) );
//	}
}

void MAP_actGoal( void )
{	
	MOT_goBlock_FinSpeed( 0.5, 0 );			// ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩi
	LL_mDelay(500);
	MOT_turn(MOT_R180);										// ?øΩE180?øΩx?øΩ?øΩ?øΩ?øΩ
	LL_mDelay(500);
	
//	MAP_SaveMapData();						// ?øΩ?øΩ?øΩH?øΩ?øΩ?øΩÃÉo?øΩb?øΩN?øΩA?øΩb?øΩv
	log_flag_off();
	MAP_actGoalLED();
	
	en_Head = (enMAP_HEAD_DIR)( (en_Head + 2) & (MAP_HEAD_DIR_MAX-1) );			//	?øΩi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩX?øΩV

}

void MAP_actGoalLED( void )
{
	int i;
	for(i = 0;i<2;i++)
	{
		SetLED(0x02);
		LL_mDelay(100);
		SetLED(0x04);
		LL_mDelay(100);
		SetLED(0x08);
		LL_mDelay(100);
		SetLED(0x04);
		LL_mDelay(100);
		SetLED(0x02);
		LL_mDelay(100);
	}
	LL_mDelay(100);
	map_write();
	SetLED(0x00);
}

void MAP_Goalsize(int size)
{
	GOAL_SIZE= size;
	if (size == 4) {
		uc_max_x = uc_max_x + 1;
		uc_max_y = uc_max_y + 1;
	}
	else if (size == 9) {
		uc_max_x = uc_max_x + 2;
		uc_max_y = uc_max_y + 2;
	}
}

void  MAP_makeReturnContourMap(uint8_t uc_staX,uint8_t uc_staY) 
{
	uint16_t		x, y, i;		// ?øΩ?øΩ?øΩ[?øΩv?øΩœêÔøΩ
	uint16_t		uc_dase;		// ?øΩ˚´Ä?øΩl
	uint16_t		uc_new;			// ?øΩV?øΩl
	uint16_t		uc_level;		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
	uint8_t		uc_wallData;	// ?øΩ«èÔøΩ?øΩ

	stPOSITION		st_pos;
	queue_t queue;
	queue_t* pQueue = &queue;

	initQueue(pQueue);

	/* ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ}?øΩb?øΩv?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ */
	for (i = 0; i < MAP_SMAP_MAX_VAL; i++) {
		us_cmap[i / MAP_Y_SIZE][i & (MAP_X_SIZE - 1)] = MAP_SMAP_MAX_VAL - 1;
	}
	/* ?øΩ⁄ïW?øΩn?øΩ_?øΩÃìÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ0?øΩ…ê›íÔøΩ */
//	us_cmap[0][0] = 0;
	setStep(0, 0, 0);
	st_pos.x = 0;
	st_pos.y = 0;
	st_pos.step = 0;

	enqueue(pQueue,st_pos);

	/* ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ}?øΩb?øΩv?øΩ?øΩ?øΩ?ê¨ */
	while (pQueue->flag != EMPTY) {
		const stPOSITION focus = dequeue(pQueue);
//		q.pop();
		const uint16_t focus_step = focus.step;
		x = focus.x;
		y = focus.y;
		stPOSITION next = focus;
		uc_wallData = g_sysMap[y][x];

		if (((uc_wallData & 0x01) == 0x00) && (y != (MAP_Y_SIZE - 1))) {
			if (us_cmap[y + 1][x] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y + 1][x] = focus_step + 1;
				next.x = x;
				next.y = y + 1;
				enqueue(pQueue,next);
			}
		}
		if (((uc_wallData & 0x02) == 0x00) && (x != (MAP_X_SIZE - 1))) {
			if (us_cmap[y][x + 1] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y][x + 1] = focus_step + 1;
				next.x = x + 1;
				next.y = y;
				enqueue(pQueue, next);
			}
		}
		if (((uc_wallData & 0x04) == 0x00) && (y != 0)) {
			if (us_cmap[y - 1][x] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y - 1][x] = focus_step + 1;
				next.x = x;
				next.y = y - 1;
				enqueue(pQueue, next);
			}
		}
		if (((uc_wallData & 0x08) == 0x00) && (x != 0)) {
			if (us_cmap[y][x - 1] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y][x - 1] = focus_step + 1;
				next.x = x - 1;
				next.y = y;
				enqueue(pQueue, next);
			}
		}

	}

}

bool MAP_KnownAcc(void) {

	bool	bl_acc = FALSE;
	switch (en_Head) {
	case NORTH:
		if ((g_sysMap[my + 1][mx] & 0xf1) == 0xf0) {
			bl_acc = TRUE;
		}

		break;

	case EAST:
		if ((g_sysMap[my][mx + 1] & 0xf2) == 0xf0) {
			bl_acc = TRUE;
		}
		break;

	case SOUTH:
		if ((g_sysMap[my - 1][mx] & 0xf4) == 0xf0) {
			bl_acc = TRUE;
		}
		break;

	case WEST:
		if ((g_sysMap[my][mx - 1] & 0xf8) == 0xf0) {
			bl_acc = TRUE;
		}
		break;

	default:
		break;
	}
	return	bl_acc;
}

void MAP_moveNextBlock_acc(enMAP_HEAD_DIR en_head, bool* p_type)
{
	*p_type = FALSE;
	f_MoveBackDist = 0;

	/* ?øΩ?øΩ?øΩ?øΩ */
	switch (en_head) {

		/* ?øΩ?øΩ?øΩÃÇ‹Ç‹ëO?øΩi */
	case NORTH:
//		*p_type = FALSE;
//		LED = LED6;
		if (MAP_KnownAcc() == FALSE) {					// ?øΩ?øΩ?øΩ…êi?øΩﬁãÔøΩÊÇ™?øΩ?øΩ?øΩT?øΩ?øΩ?øΩÃÇ∆ÇÔøΩ
			if (st_known.bl_Known == TRUE){
				if (st_known.uc_StrCnt < 2) {
					MOT_goBlock_Const(1);					// 1?øΩ?øΩ?øΩÃèÍçá?øΩÕìÔøΩ?øΩ?øΩ?øΩÃÇ‹ÇÔøΩ
				}
				else {
					MOT_setTrgtSpeed(MAP_KNOWN_ACC_SPEED);									// ?øΩ?øΩ?øΩm?øΩ?øΩ‘âÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ∆ÇÔøΩ?øΩÃñ⁄ïW?øΩ?øΩ?øΩx	
					MOT_goBlock_FinSpeed((float)(st_known.uc_StrCnt), SEARCH_SPEED);				// n?øΩ?øΩ?øΩO?øΩi
					MOT_setTrgtSpeed(SEARCH_SPEED);										// ?øΩ⁄ïW?øΩ?øΩ?øΩx?øΩ?øΩ?øΩf?øΩt?øΩH?øΩ?øΩ?øΩg?øΩl?øΩ…ñﬂÇÔøΩ
				}
			}
			MOT_goBlock_Const(1);	////////////////////
			st_known.uc_StrCnt = 0;
			st_known.bl_Known = FALSE;

		}
		else {

			st_known.uc_StrCnt++;			// ?øΩ⁄ìÔøΩ?øΩ?øΩ?øΩÃâÔøΩ?øΩZ
			st_known.bl_Known = TRUE;
		}

		break;

		/* ?øΩE?øΩ…êÔøΩ?øΩ?∑ÇÔøΩ */
	case EAST:
//		LED = LED8;
		if (st_known.bl_Known == TRUE) {		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
			if (st_known.uc_StrCnt < 2) {
				MOT_goBlock_Const(1);					// 1?øΩ?øΩ?øΩÃèÍçá?øΩÕìÔøΩ?øΩ?øΩ?øΩÃÇ‹ÇÔøΩ
			}
			else {
//				LED = LED_ALL_ON;
				MOT_setTrgtSpeed(MAP_KNOWN_ACC_SPEED);									// ?øΩ?øΩ?øΩm?øΩ?øΩ‘âÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ∆ÇÔøΩ?øΩÃñ⁄ïW?øΩ?øΩ?øΩx	
				MOT_goBlock_FinSpeed((float)(st_known.uc_StrCnt), SEARCH_SPEED);				// n?øΩ?øΩ?øΩO?øΩi
				MOT_setTrgtSpeed(SEARCH_SPEED);										// ?øΩ⁄ïW?øΩ?øΩ?øΩx?øΩ?øΩ?øΩf?øΩt?øΩH?øΩ?øΩ?øΩg?øΩl?øΩ…ñﬂÇÔøΩ
//				LED = LED_ALL_OFF;
			}
			st_known.uc_StrCnt = 0;		/////////////////////////////////////////
			st_known.bl_Known = FALSE;
		}
		if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?øΩk?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒêÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) ) 			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒñk?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
			){
			uc_dist_control = 0.01;
			}
		else{
			uc_dist_control = 0;
		}
		if( uc_SlaCnt < SLA_count ){
				MOT_goSla( MOT_R90S, PARAM_getSra( SLA_90 ) );	// ?øΩE?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ
				uc_SlaCnt++;
			}
			else{
				f_MoveBackDist = 0;
				/* ?øΩ«ìÔøΩ?øΩƒép?øΩ?øΩ?øΩ?øΩ?øΩ?øΩi?øΩ?øΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩo?øΩb?øΩN?øΩ{?øΩ⁄ìÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩZ?øΩ?øΩ?øΩ?øΩj */
				if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?øΩk?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒêÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒñk?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) ) 			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
				){
					MOT_goBlock_FinSpeed( 0.5, 0 );			// ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩi
					MOT_turn(MOT_R90);						// ?øΩE90?øΩx?øΩ?øΩ?øΩ?øΩ
					uc_SlaCnt = 0;
					MOT_goHitBackWall();					// ?øΩo?øΩb?øΩN?øΩ?øΩ?øΩ?øΩ
					f_MoveBackDist = MOVE_BACK_DIST;		// ?øΩo?øΩb?øΩN?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩÃà⁄ìÔøΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ]?øΩ?øΩ?øΩ?øΩ?øΩZ
					*p_type = TRUE;							// ?øΩ?øΩ?øΩÕîÔøΩ?øΩ?øΩ‘Åi?øΩ{?øΩo?øΩb?øΩN?øΩj?øΩ?øΩ?øΩi?øΩﬂÇÔøΩ
				}
				else{
					MOT_goSla( MOT_R90S, PARAM_getSra( SLA_90 ) );	// ?øΩE?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ
					uc_SlaCnt++;
				}
			}
		break;

		/* ?øΩ?øΩ?øΩ…êÔøΩ?øΩ?∑ÇÔøΩ */
	case WEST:
		if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?øΩk?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒêÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒñk?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) ) 			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
			){
			uc_dist_control = 0.01;
			}
		else{
			uc_dist_control = 0;
		}
		if (st_known.bl_Known == TRUE) {		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
			if (st_known.uc_StrCnt < 2) {
				MOT_goBlock_Const(1);					// 1?øΩ?øΩ?øΩÃèÍçá?øΩÕìÔøΩ?øΩ?øΩ?øΩÃÇ‹ÇÔøΩ
			}
			else {
//				LED = LED_ALL_ON;
				MOT_setTrgtSpeed(MAP_KNOWN_ACC_SPEED);									// ?øΩ?øΩ?øΩm?øΩ?øΩ‘âÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ∆ÇÔøΩ?øΩÃñ⁄ïW?øΩ?øΩ?øΩx	
				MOT_goBlock_FinSpeed((float)(st_known.uc_StrCnt), SEARCH_SPEED);				// n?øΩ?øΩ?øΩO?øΩi
				MOT_setTrgtSpeed(SEARCH_SPEED);										// ?øΩ⁄ïW?øΩ?øΩ?øΩx?øΩ?øΩ?øΩf?øΩt?øΩH?øΩ?øΩ?øΩg?øΩl?øΩ…ñﬂÇÔøΩ
//				LED = LED_ALL_OFF;
			}
			st_known.uc_StrCnt = 0;			//////////////////////////////////////
			st_known.bl_Known = FALSE;
		}

		if( uc_SlaCnt < SLA_count ){
				MOT_goSla( MOT_L90S, PARAM_getSra( SLA_90 ) );	// ?øΩ?øΩ?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ
				uc_SlaCnt++;
			}
			else{
				f_MoveBackDist = 0;
				/* ?øΩ«ìÔøΩ?øΩƒép?øΩ?øΩ?øΩ?øΩ?øΩ?øΩi?øΩ?øΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩo?øΩb?øΩN?øΩ{?øΩ⁄ìÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩZ?øΩ?øΩ?øΩ?øΩj */
				if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?øΩk?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒêÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) ) 			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒñk?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
				){
					MOT_goBlock_FinSpeed( 0.5, 0 );		// ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩi
					MOT_turn(MOT_L90);					// ?øΩE90?øΩx?øΩ?øΩ?øΩ?øΩ
					uc_SlaCnt = 0;
					MOT_goHitBackWall();					// ?øΩo?øΩb?øΩN?øΩ?øΩ?øΩ?øΩ
					f_MoveBackDist = MOVE_BACK_DIST;		// ?øΩo?øΩb?øΩN?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩÃà⁄ìÔøΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ]?øΩ?øΩ?øΩ?øΩ?øΩZ
					*p_type = TRUE;							// ?øΩ?øΩ?øΩÕîÔøΩ?øΩ?øΩ‘Åi?øΩ{?øΩo?øΩb?øΩN?øΩj?øΩ?øΩ?øΩi?øΩﬂÇÔøΩ
				}
				else{
					MOT_goSla( MOT_L90S, PARAM_getSra( SLA_90 ) );	// ?øΩ?øΩ?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ
					uc_SlaCnt++;
				}
			}
		break;

		/* ?øΩ?øΩ?øΩ]?øΩ?øΩ?øΩƒñﬂÇÔøΩ */
	case SOUTH:
//		LED = LED_ALL_ON;
		MOT_goBlock_FinSpeed(0.5, 0);			// ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩi
		MOT_turn(MOT_R180);									// ?øΩE180?øΩx?øΩ?øΩ?øΩ?øΩ
		uc_SlaCnt = 0;

		/* ?øΩ«ìÔøΩ?øΩƒép?øΩ?øΩ?øΩ?øΩ?øΩ?øΩi?øΩ?øΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩo?øΩb?øΩN?øΩ{?øΩ⁄ìÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩZ?øΩ?øΩ?øΩ?øΩj */
		if (((en_Head == NORTH) && ((g_sysMap[my][mx] & 0x01) != 0)) ||		// ?øΩk?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒñk?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
			((en_Head == EAST) && ((g_sysMap[my][mx] & 0x02) != 0)) ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
			((en_Head == SOUTH) && ((g_sysMap[my][mx] & 0x04) != 0)) ||		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒìÔøΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
			((en_Head == WEST) && ((g_sysMap[my][mx] & 0x08) != 0)) 			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩƒêÔøΩ?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
			) {
			MOT_goHitBackWall();					// ?øΩo?øΩb?øΩN?øΩ?øΩ?øΩ?øΩ
			f_MoveBackDist = MOVE_BACK_DIST;	// ?øΩo?øΩb?øΩN?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩÃà⁄ìÔøΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ]?øΩ?øΩ?øΩ?øΩ?øΩZ
		}
		*p_type = TRUE;								// ?øΩ?øΩ?øΩÕîÔøΩ?øΩ?øΩ‘Å{?øΩo?øΩb?øΩN?øΩ?øΩ?øΩi?øΩﬂÇÔøΩ
		break;

	default:
		break;
	}

	/* ?øΩi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩX?øΩV */
//	en_Head = (enMAP_HEAD_DIR)( (en_Head + en_head) & (MAP_HEAD_DIR_MAX-1) );
	en_Head = (enMAP_HEAD_DIR)(((uint8_t)en_Head + (uint8_t)en_head) & (MAP_HEAD_DIR_MAX - 1));
}

void MAP_searchGoal(
	uint8_t 			uc_trgX, 		///< [in] ?øΩ⁄ïWx?øΩ?øΩ?øΩW
	uint8_t 			uc_trgY, 		///< [in] ?øΩ⁄ïWy?øΩ?øΩ?øΩW 
	enMAP_ACT_MODE 	en_type, 		///< [in] ?øΩT?øΩ?øΩ?øΩ?øΩ?øΩ@
	enSEARCH_MODE	en_search 		///< [in] ?øΩT?øΩ?øΩ?øΩ?øΩ?øΩ@
){
	enMAP_HEAD_DIR	en_head = NORTH;
	bool		bl_type = TRUE;			// ?øΩ?øΩ?øΩ›à íu?øΩAFALSE: ?øΩP?øΩ?øΩ‘ëO?øΩi?øΩ?øΩ‘ÅATURE:?øΩ?øΩ?øΩ?øΩ‘ëO?øΩi?øΩ?øΩ?øΩ
	enMAP_HEAD_DIR		en_endDir;
	
	uint8_t uc_goalX;
	uint8_t uc_goalY;
	uint8_t uc_staX;
	uint8_t uc_staY;
	
	search_flag = TRUE;

	if (en_search == SEARCH_RETURN){
		uc_goalX = uc_trgX;
		uc_goalY = uc_trgY;
		uc_staX = mx;
		uc_staY = my;
//		printf("mx%d,my%d\n", mx, my);
//		MAP_makeContourMap(uc_trgX, uc_trgY, en_type);
		MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
		MAP_searchCmdList(uc_staX, uc_staY, en_Head, uc_goalX, uc_goalX, &en_endDir);
		uc_trgX = Return_X;
		uc_trgY = Return_Y;
//		printf("goalx%d,goaly%d\n", Return_X, Return_Y);
//		MAP_showcountLog();
	}

//	SYS_setDisable( SYS_MODE );				// ?øΩ?øΩ?øΩ[?øΩh?øΩœçX?øΩ÷é~

	MOT_setTrgtSpeed(SEARCH_SPEED);		// ?øΩ⁄ïW?øΩ?øΩ?øΩx
	MOT_setNowSpeed( 0.0f );
	f_MoveBackDist = 0;
	uc_SlaCnt = 0;
	if(uc_trgX == GOAL_MAP_X && uc_trgY == GOAL_MAP_Y){
		f_MoveBackDist = MOVE_BACK_DIST;
	}
	
	log_flag_on();	//?øΩ?øΩ?øΩO?øΩ÷êÔøΩ?øΩX?øΩ^?øΩ[?øΩg?øΩi?øΩ?øΩ?éû?øΩ˙´úÔøΩj
	
	/* ?øΩ?øΩ?øΩH?øΩT?øΩ?øΩ */
	while(1){
		MAP_refMousePos( en_Head );								// ?øΩ?øΩ?øΩW?øΩX?øΩV
//		MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ}?øΩb?øΩv?øΩ?øΩ?øΩ?øΩ?øΩ
		
		/* ?øΩ?øΩ?øΩM?øΩn?øΩ?øΩ?øΩ?øΩT?øΩ?øΩ */
		if( SEARCH_TURN == en_search ){
//			MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ}?øΩb?øΩv?øΩ?øΩ?øΩ?øΩ?øΩ
			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
			if( TRUE == bl_type ){
				MOT_goBlock_FinSpeed( 0.5 + f_MoveBackDist, SEARCH_SPEED );		// ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩi(?øΩo?øΩb?øΩN?øΩÃà⁄ìÔøΩ?øΩ ÇÔøΩ?øΩ‹ÇÔøΩ)
			}
			MAP_makeMapData();												// ?øΩ«Éf?øΩ[?øΩ^?øΩ?øΩ?øΩ?øΩ?øΩ?øΩH?øΩf?øΩ[?øΩ^?øΩ?øΩ?øΩ?ê¨			?øΩ?øΩ ?øΩ?øΩ?øΩ?øΩ?øΩ≈Éf?øΩ[?øΩ^?øΩ?ê¨?øΩ?øΩ?øΩ~?øΩX?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩ
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);						// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩMAP?øΩ@?øΩ≈êi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩZ?øΩo			?øΩ?øΩ ?øΩ?øΩ?øΩ?øΩ?øΩMAP?øΩ?øΩ?øΩ?ê¨
			
			/* ?øΩ?øΩ?øΩÃãÔøΩ?øΩ÷à⁄ìÔøΩ */
			if(( mx == uc_trgX ) && ( my == uc_trgY )){
				MAP_actGoal();										// ?øΩS?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩÃìÔøΩ?øΩ?øΩ
				break;
			}
			else{
				MAP_moveNextBlock(en_head, &bl_type);				// ?øΩ?øΩ?øΩÃãÔøΩ?øΩ÷à⁄ìÔøΩ								?øΩ?øΩ ?øΩ?øΩ?øΩ?øΩ?øΩ≈âÔøΩ?øΩﬂÇƒ??øΩ?øΩ?øΩ?øΩ[?øΩX?øΩ`?øΩF?øΩb?øΩN?øΩ{?øΩ«çƒìx?øΩ?ê¨?øΩ{?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ{?øΩ?øΩ?øΩM?øΩn?øΩ?øΩ?øΩ?ÆçÔøΩ
			}
		}
		/* ?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩT?øΩ?øΩ */
		else if( SEARCH_SURA == en_search ){
//			MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ}?øΩb?øΩv?øΩ?øΩ?øΩ?øΩ?øΩ
			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
			if( TRUE == bl_type ){
				
				MOT_goBlock_FinSpeed( 0.5 + f_MoveBackDist, SEARCH_SPEED );		// ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩi(?øΩo?øΩb?øΩN?øΩÃà⁄ìÔøΩ?øΩ ÇÔøΩ?øΩ‹ÇÔøΩ)
			}
			MAP_makeMapData();		// ?øΩ«Éf?øΩ[?øΩ^?øΩ?øΩ?øΩ?øΩ?øΩ?øΩH?øΩf?øΩ[?øΩ^?øΩ?øΩ?øΩ?ê¨
			
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);				// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩMAP?øΩ@?øΩ≈êi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩZ?øΩo			?øΩ?øΩ ?øΩ?øΩ?øΩ?øΩ?øΩMAP?øΩ?øΩ?øΩ?ê¨
			
			/* ?øΩ?øΩ?øΩÃãÔøΩ?øΩ÷à⁄ìÔøΩ */
			if(( mx == uc_trgX ) && ( my == uc_trgY )){
				MAP_actGoal();										// ?øΩS?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩÃìÔøΩ?øΩ?øΩ
				break;
			}
			else{
				MAP_moveNextBlock_Sura(en_head, &bl_type, FALSE );	// ?øΩ?øΩ?øΩÃãÔøΩ?øΩ÷à⁄ìÔøΩ						?øΩ?øΩ ?øΩ?øΩ?øΩ?øΩ?øΩ≈âÔøΩ?øΩﬂÇƒ??øΩ?øΩ?øΩ?øΩ[?øΩX?øΩ`?øΩF?øΩb?øΩN?øΩ{?øΩ«çƒìx?øΩ?ê¨?øΩ{?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ{?øΩ?øΩ?øΩM?øΩn?øΩ?øΩ?øΩ?ÆçÔøΩ
//				MAP_moveNextBlock_acc(en_head, &bl_type);
			}
		}
		/* ?øΩA?øΩ“íT?øΩ?øΩ */
		else if (SEARCH_RETURN == en_search) {
			
			if( TRUE == bl_type ){
				
				MOT_goBlock_FinSpeed( 0.5 + f_MoveBackDist, SEARCH_SPEED );		// ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩi(?øΩo?øΩb?øΩN?øΩÃà⁄ìÔøΩ?øΩ ÇÔøΩ?øΩ‹ÇÔøΩ)
			}
			MAP_makeMapData();		// ?øΩ«Éf?øΩ[?øΩ^?øΩ?øΩ?øΩ?øΩ?øΩ?øΩH?øΩf?øΩ[?øΩ^?øΩ?øΩ?øΩ?ê¨
						
			MAP_makeReturnContourMap(uc_staX,uc_staY);
			MAP_searchCmdList(uc_staX, uc_staY, en_Head, uc_goalX, uc_goalX, &en_endDir);
			uc_trgX = Return_X;
			uc_trgY = Return_Y;
//			MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ}?øΩb?øΩv?øΩ?øΩ?øΩ?øΩ?øΩ
			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);	
			/* ?øΩ?øΩ?øΩÃãÔøΩ?øΩ÷à⁄ìÔøΩ */
//			if ((us_cmap[my][mx] == 0)||((g_sysMap[uc_trgY][uc_trgX]&0xf0) == 0xf0)) {
			if ((mx == 0)&&(my == 0)){
				MAP_actGoal();
				break;
			}
//			}
			else {
				MAP_moveNextBlock_Sura(en_head, &bl_type, FALSE);	// ?øΩ?øΩ?øΩÃãÔøΩ?øΩ÷à⁄ìÔøΩ						?øΩ?øΩ ?øΩ?øΩ?øΩ?øΩ?øΩ≈âÔøΩ?øΩﬂÇƒ??øΩ?øΩ?øΩ?øΩ[?øΩX?øΩ`?øΩF?øΩb?øΩN?øΩ{?øΩ«çƒìx?øΩ?ê¨?øΩ{?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ{?øΩ?øΩ?øΩM?øΩn?øΩ?øΩ?øΩ?ÆçÔøΩ
//				MAP_moveNextBlock_acc(en_head, &bl_type);
			}
//			LED_count(uc_trgY);
		}

		
		/* ?øΩr?øΩ?øΩ?øΩ≈êÔøΩ?øΩ?øΩs?øΩ\?øΩ…Ç»ÇÔøΩ?øΩ?øΩ */
		if( SYS_isOutOfCtrl() == TRUE ){
			CTRL_stop();
			DCM_brakeMot( DCM_R );		// ?øΩu?øΩ?øΩ?øΩ[?øΩL
			DCM_brakeMot( DCM_L );		// ?øΩu?øΩ?øΩ?øΩ[?øΩL
			
			/* ?øΩ?øΩ?øΩH?øΩ÷òA?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ */
			en_Head		= NORTH;
			mx			= 0;
			my			= 0;
			f_MoveBackDist = 0;
			
			// DCMC?øΩÕâÔøΩ?øΩ ??øΩ?øΩW?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ≈äÔøΩ?øΩ…ÉN?øΩ?øΩ?øΩA?øΩ∆ãŸã}?øΩ?øΩ~?øΩ?øΩ?øΩs?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩB
			break;
		}
	}
	search_flag = FALSE;
	LL_mDelay(1000);
//	SYS_setEnable( SYS_MODE );				// ?øΩ?øΩ?øΩ[?øΩh?øΩœçX?øΩL?øΩ?øΩ
}

void MAP_searchGoalKnown(
	uint8_t 			uc_trgX, 		///< [in] ?øΩ⁄ïWx?øΩ?øΩ?øΩW
	uint8_t 			uc_trgY, 		///< [in] ?øΩ⁄ïWy?øΩ?øΩ?øΩW 
	enMAP_ACT_MODE 	en_type, 		///< [in] ?øΩT?øΩ?øΩ?øΩ?øΩ?øΩ@
	enSEARCH_MODE	en_search 		///< [in] ?øΩT?øΩ?øΩ?øΩ?øΩ?øΩ@
){
	enMAP_HEAD_DIR	en_head = NORTH;
	bool		bl_type = TRUE;			// ?øΩ?øΩ?øΩ›à íu?øΩAFALSE: ?øΩP?øΩ?øΩ‘ëO?øΩi?øΩ?øΩ‘ÅATURE:?øΩ?øΩ?øΩ?øΩ‘ëO?øΩi?øΩ?øΩ?øΩ
	enMAP_HEAD_DIR		en_endDir;
	
	uint8_t uc_goalX;
	uint8_t uc_goalY;
	uint8_t uc_staX;
	uint8_t uc_staY;
	
	search_flag = TRUE;

	if (en_search == SEARCH_RETURN){
		uc_goalX = uc_trgX;
		uc_goalY = uc_trgY;
		uc_staX = mx;
		uc_staY = my;
//		printf("mx%d,my%d\n", mx, my);
//		MAP_makeContourMap(uc_trgX, uc_trgY, en_type);
		MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
		MAP_searchCmdList(uc_staX, uc_staY, en_Head, uc_goalX, uc_goalX, &en_endDir);
		uc_trgX = Return_X;
		uc_trgY = Return_Y;
//		printf("goalx%d,goaly%d\n", Return_X, Return_Y);
//		MAP_showcountLog();
	}

//	SYS_setDisable( SYS_MODE );				// ?øΩ?øΩ?øΩ[?øΩh?øΩœçX?øΩ÷é~

	MOT_setTrgtSpeed(SEARCH_SPEED);		// ?øΩ⁄ïW?øΩ?øΩ?øΩx
	MOT_setNowSpeed( 0.0f );
	f_MoveBackDist = 0;
	uc_SlaCnt = 0;
	if(uc_trgX == GOAL_MAP_X && uc_trgY == GOAL_MAP_Y){
		f_MoveBackDist = MOVE_BACK_DIST;
	}
	
	log_flag_on();	//?øΩ?øΩ?øΩO?øΩ÷êÔøΩ?øΩX?øΩ^?øΩ[?øΩg?øΩi?øΩ?øΩ?éû?øΩ˙´úÔøΩj
	
	/* ?øΩ?øΩ?øΩH?øΩT?øΩ?øΩ */
	while(1){
		MAP_refMousePos( en_Head );								// ?øΩ?øΩ?øΩW?øΩX?øΩV
//		MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ}?øΩb?øΩv?øΩ?øΩ?øΩ?øΩ?øΩ
		
		/* ?øΩ?øΩ?øΩM?øΩn?øΩ?øΩ?øΩ?øΩT?øΩ?øΩ */
		if( SEARCH_TURN == en_search ){
//			MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ}?øΩb?øΩv?øΩ?øΩ?øΩ?øΩ?øΩ
			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
			if( TRUE == bl_type ){
				MOT_goBlock_FinSpeed( 0.5 + f_MoveBackDist, SEARCH_SPEED );		// ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩi(?øΩo?øΩb?øΩN?øΩÃà⁄ìÔøΩ?øΩ ÇÔøΩ?øΩ‹ÇÔøΩ)
			}
			MAP_makeMapData();												// ?øΩ«Éf?øΩ[?øΩ^?øΩ?øΩ?øΩ?øΩ?øΩ?øΩH?øΩf?øΩ[?øΩ^?øΩ?øΩ?øΩ?ê¨			?øΩ?øΩ ?øΩ?øΩ?øΩ?øΩ?øΩ≈Éf?øΩ[?øΩ^?øΩ?ê¨?øΩ?øΩ?øΩ~?øΩX?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩ
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);						// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩMAP?øΩ@?øΩ≈êi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩZ?øΩo			?øΩ?øΩ ?øΩ?øΩ?øΩ?øΩ?øΩMAP?øΩ?øΩ?øΩ?ê¨
			
			/* ?øΩ?øΩ?øΩÃãÔøΩ?øΩ÷à⁄ìÔøΩ */
			if(( mx == uc_trgX ) && ( my == uc_trgY )){
				MAP_actGoal();										// ?øΩS?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩÃìÔøΩ?øΩ?øΩ
				break;
			}
			else{
				MAP_moveNextBlock(en_head, &bl_type);				// ?øΩ?øΩ?øΩÃãÔøΩ?øΩ÷à⁄ìÔøΩ								?øΩ?øΩ ?øΩ?øΩ?øΩ?øΩ?øΩ≈âÔøΩ?øΩﬂÇƒ??øΩ?øΩ?øΩ?øΩ[?øΩX?øΩ`?øΩF?øΩb?øΩN?øΩ{?øΩ«çƒìx?øΩ?ê¨?øΩ{?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ{?øΩ?øΩ?øΩM?øΩn?øΩ?øΩ?øΩ?ÆçÔøΩ
			}
		}
		/* ?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩT?øΩ?øΩ */
		else if( SEARCH_SURA == en_search ){
//			MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ}?øΩb?øΩv?øΩ?øΩ?øΩ?øΩ?øΩ
			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
			if( TRUE == bl_type ){
				
				MOT_goBlock_FinSpeed( 0.5 + f_MoveBackDist, SEARCH_SPEED );		// ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩi(?øΩo?øΩb?øΩN?øΩÃà⁄ìÔøΩ?øΩ ÇÔøΩ?øΩ‹ÇÔøΩ)
			}
			if (st_known.bl_Known != TRUE) {
				MAP_makeMapData();		// ?øΩ«Éf?øΩ[?øΩ^?øΩ?øΩ?øΩ?øΩ?øΩ?øΩH?øΩf?øΩ[?øΩ^?øΩ?øΩ?øΩ?ê¨
			}
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);				// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩMAP?øΩ@?øΩ≈êi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩZ?øΩo			?øΩ?øΩ ?øΩ?øΩ?øΩ?øΩ?øΩMAP?øΩ?øΩ?øΩ?ê¨
			
			/* ?øΩ?øΩ?øΩÃãÔøΩ?øΩ÷à⁄ìÔøΩ */
			if(( mx == uc_trgX ) && ( my == uc_trgY )){
				MAP_actGoal();										// ?øΩS?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩÃìÔøΩ?øΩ?øΩ
				break;
			}
			else{
//				MAP_moveNextBlock_Sura(en_head, &bl_type, FALSE );	// ?øΩ?øΩ?øΩÃãÔøΩ?øΩ÷à⁄ìÔøΩ						?øΩ?øΩ ?øΩ?øΩ?øΩ?øΩ?øΩ≈âÔøΩ?øΩﬂÇƒ??øΩ?øΩ?øΩ?øΩ[?øΩX?øΩ`?øΩF?øΩb?øΩN?øΩ{?øΩ«çƒìx?øΩ?ê¨?øΩ{?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ{?øΩ?øΩ?øΩM?øΩn?øΩ?øΩ?øΩ?ÆçÔøΩ
				MAP_moveNextBlock_acc(en_head, &bl_type);
			}
		}
		/* ?øΩA?øΩ“íT?øΩ?øΩ */
		else if (SEARCH_RETURN == en_search) {
			
			if( TRUE == bl_type ){
				
				MOT_goBlock_FinSpeed( 0.5 + f_MoveBackDist, SEARCH_SPEED );		// ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩi(?øΩo?øΩb?øΩN?øΩÃà⁄ìÔøΩ?øΩ ÇÔøΩ?øΩ‹ÇÔøΩ)
			}
			if (st_known.bl_Known != TRUE) {
				MAP_makeMapData();		// ?øΩ«Éf?øΩ[?øΩ^?øΩ?øΩ?øΩ?øΩ?øΩ?øΩH?øΩf?øΩ[?øΩ^?øΩ?øΩ?øΩ?ê¨
			}			
			MAP_makeReturnContourMap(uc_staX,uc_staY);
			MAP_searchCmdList(uc_staX, uc_staY, en_Head, uc_goalX, uc_goalX, &en_endDir);
			uc_trgX = Return_X;
			uc_trgY = Return_Y;
//			MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ}?øΩb?øΩv?øΩ?øΩ?øΩ?øΩ?øΩ
			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);	

			/* ?øΩ?øΩ?øΩÃãÔøΩ?øΩ÷à⁄ìÔøΩ */
//			if ((us_cmap[my][mx] == 0)||((g_sysMap[uc_trgY][uc_trgX]&0xf0) == 0xf0)) {
			if ((mx == 0)&&(my == 0)){
				MAP_actGoal();
				break;
			}
//			}
			else {
				MAP_moveNextBlock_acc(en_head, &bl_type);
			}
//			LED_count(uc_trgY);
		}

		
		/* ?øΩr?øΩ?øΩ?øΩ≈êÔøΩ?øΩ?øΩs?øΩ\?øΩ…Ç»ÇÔøΩ?øΩ?øΩ */
		if( SYS_isOutOfCtrl() == TRUE ){
			CTRL_stop();
			DCM_brakeMot( DCM_R );		// ?øΩu?øΩ?øΩ?øΩ[?øΩL
			DCM_brakeMot( DCM_L );		// ?øΩu?øΩ?øΩ?øΩ[?øΩL
			
			/* ?øΩ?øΩ?øΩH?øΩ÷òA?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ */
			en_Head		= NORTH;
			mx			= 0;
			my			= 0;
			f_MoveBackDist = 0;
			
			// DCMC?øΩÕâÔøΩ?øΩ ??øΩ?øΩW?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ≈äÔøΩ?øΩ…ÉN?øΩ?øΩ?øΩA?øΩ∆ãŸã}?øΩ?øΩ~?øΩ?øΩ?øΩs?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩB
			break;
		}
	}
	search_flag = FALSE;
	LL_mDelay(1000);
//	SYS_setEnable( SYS_MODE );				// ?øΩ?øΩ?øΩ[?øΩh?øΩœçX?øΩL?øΩ?øΩ

}
