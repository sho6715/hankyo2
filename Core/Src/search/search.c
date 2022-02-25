/*
 * search.c
 *
 *  Created on: 2021/02/04
 *      Author: shohe
 */


#include "search/search.h"

#define MAP_SMAP_MAX_VAL	( MAP_X_SIZE * MAP_Y_SIZE ) 			///< ?申?申?申?申?申?申map?申����鐃�l
#define MAP_SMAP_MAX_PRI_VAL	( MAP_SMAP_MAX_VAL * 4 )				///< ?申?申?申?申?申?申map?申��D?申?申x?申��鐃�l

#define MOVE_BACK_DIST		(0.24f)

enMAP_HEAD_DIR	en_Head;										///< ?申}?申E?申X?申��i?申s?申?申?申?申 0:N 1:E 2:S 3:W
uint8_t		my;												///< ?申}?申E?申X?申��w?申?申?申W
uint8_t		mx;												///< ?申}?申E?申X?申��x?申?申?申W
float		f_MoveBackDist;									///< ?申��鐃�?申��鐃�?申?申��鐃殉�鐃�?申?申?申?申?申?申[?申?申?申]
uint8_t		uc_SlaCnt = 0;									// ?申X?申?申?申?申?申[?申?申?申A?申?申?申?申
uint8_t		uc_back[ MAP_Y_SIZE ][ MAP_X_SIZE ];			// ?申?申?申H?申f?申[?申^

uint8_t		GOAL_MAP_X;					//?申S?申[?申?申?申?申?申W?申��X?申v?申?申?申O?申?申?申?申?申p?申?申
uint8_t		GOAL_MAP_Y;					//?申S?申[?申?申?申?申?申W?申��X?申v?申?申?申O?申?申?申?申?申p?申?申

uint8_t		GOAL_SIZE;
//?申?申?申?申?申?申?申}?申b?申v?申?申?申X?申V?申?申?申~?申�����?申������鐃�?申?申?申K?申?申��鐃�
uint8_t		uc_max_x = GOAL_MAP_X_def;
uint8_t		uc_max_y = GOAL_MAP_Y_def;

//TKR
/* ?申?申?申m?申?申��鐃�?申?申 */
typedef struct{
	uint8_t	uc_StrCnt;
	bool	bl_Known;
}stMAP_KNOWN;

stMAP_KNOWN		st_known = { 0,FALSE };

uint8_t			SLA_count = 10;

void MAP_init( void )
{
//	uint8_t uc_dummy[ MAP_Y_SIZE ][ MAP_X_SIZE ];			// ?申?申?申H?申f?申[?申^

	/* ?申?申?申W?申A?申?申?申?申?申A?申?申?申H?申?申?申?申?申?申?申?申?申?申 */
	en_Head		= NORTH;
	mx		= 0;
	my		= 0;
	MAP_clearMap();
	
	/* ?申?申?申s?申p?申��p?申?申?申?申?申[?申^ */
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
			LL_mDelay(200);			// SW?申?申?申?申?申?申?申?申?申������鐃�
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
			LL_mDelay(200);			// SW?申?申?申?申?申?申?申?申?申������鐃�
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
	/* ?申?申?申W?申A?申?申?申?申?申A?申?申?申H?申?申?申?申?申?申?申?申?申?申 */
	en_Head		= NORTH;
	mx			= 0;
	my			= 0;
	MAP_clearMap();
	
	/* ?申?申?申s?申p?申��p?申?申?申?申?申[?申^ */
	f_MoveBackDist = 0;
	uc_SlaCnt = 0;

//	Storage_Clear( sizeof(g_sysMap), ADR_MAP );			// ?申f?申[?申^?申Z?申[?申u
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
	/*  ?申��鐃�?申?申H  */
	/* ---------- */
	printf("\n\r  /* ---------- */   ");
	printf("\n\r  /*  ?申��鐃�?申?申H  */   ");
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
	/*  ?申?申?申?申?申?申?申?申?申H  */
	/* ---------- */
	printf("\n\r  /* ------------ */   ");
	printf("\n\r  /*  ?申?申?申?申?申?申?申?申?申H  */   ");
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

	/* ?申?申?申������}?申b?申v?申f?申[?申^?申?��T?申?申?申?申����鐃�?申?申 */
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

	// ?申Z?申?申?申T?申?申?��鐃叔�鐃�?申?��
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

	// ?申}?申E?申X?申��i?申s?申?申?申?申?申��鐃�?申���?申��Z?申?申?申T?申f?申[?申^?申?申?申��鐃�?申?申?申��f?申[?申^?申��鐃�?申?申
	if		( en_Head == EAST ){
		uc_wall = uc_wall >> 3;
	}
	else if ( en_Head == SOUTH ){
		uc_wall = uc_wall >> 2;
	}
	else if ( en_Head == WEST ){
		uc_wall = uc_wall >> 1;
	}

	//	?申T?申?申?申����t?申?申?申O?申?����鐃�
	return ( uc_wall | 0xf0 );
}

void MAP_makeMapData( void )
{
	uint8_t uc_wall;

	//	?申?申?申s?申?申?申����鐃�?申?申?申?申H?申?申?申��鐃�?申?申
	if ( ( mx == 0 ) && ( my == 0 ) ){
		uc_wall = 0xfe;
	}
	else{
		uc_wall = MAP_getWallData();
	}
	g_sysMap[my][mx] = uc_wall;

	//	?申����鐃峻���l?申`?申o?申f?申[?申^?申?申?申X?申V?申?申?申?申
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
	uint8_t uc_goalX, 			///< [in] ?申S?申[?申?申X?申?申?申W
	uint8_t uc_goalY, 			///< [in] ?申S?申[?申?申Y?申?申?申W
	enMAP_ACT_MODE	en_type		///< [in] ?申v?申Z?申?申?申@?申i?申��鐃�?申?申?申g?申p?申j
){
	uint16_t		x, y, i;		// ?申?申?申[?申v?申��鐃�
	uint16_t		uc_dase;		// ?申���?申l
	uint16_t		uc_new;			// ?申V?申l
	uint16_t		uc_level;		// ?申?申?申?申?申?申
	uint8_t		uc_wallData;	// ?申��鐃�?申

	en_type = en_type;		// ?申R?申?申?申p?申C?申?申?申?申?申[?申j?申?申?申O?申?申?申?申i?申?申?申?申?申?申���鐃�j

	/* ?申?申?申?申?申?申?申}?申b?申v?申?申?申?申?申?申?申?申?申?申?申?申 */
	for ( i = 0; i < MAP_SMAP_MAX_VAL; i++ ){
		us_cmap[ i / MAP_Y_SIZE][ i & (MAP_X_SIZE-1) ] = MAP_SMAP_MAX_VAL - 1;
	}
	/* ?申��W?申n?申_?申��鐃�?申?申?申?申?申?申0?申����鐃� */
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

	/* ?申?申?申?申?申?申?申}?申b?申v?申?申?申?�� */
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
					/* ?申T?申?申?申?申?申s */
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
					/* ?申��Z?申?申?申s */
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
	/* (x, y) ��������ｃ�若������??���腆肴�? */
	if (x < 0 || y < 0 || x >= MAP_X_SIZE || y >= MAP_Y_SIZE) {
		printf( "referred to out of field\r\n");
		return;
	}
	us_cmap[y][x] = step;
}

void  MAP_makeContourMap_kai2(
	uint8_t uc_goalX, 			///< [in] ��眼�若��X綺ф�?
	uint8_t uc_goalY, 			///< [in] ��眼�若��Y綺ф�?
	enMAP_ACT_MODE	en_type		///< [in] 荐�膊���号��鐚���障�?���篏睡��?�?
) {
	uint16_t		x, y, i;		// �����若��紊����
	uint16_t		uc_dase;		// ��堺�����
	uint16_t		uc_new;			// ��医��
	uint16_t		uc_level;		// 膈�蕭�膩?
	uint8_t		uc_wallData;	// 紕�������

	stPOSITION		st_pos;

	en_type = en_type;		// ��潟�潟����ゃ�������若����潟�医�����?�����������������?�?

	queue_t queue;
	queue_t* pQueue = &queue;

	initQueue(pQueue);

	/* 膈�蕭�膩��?������������������������? */
	for (i = 0; i < MAP_SMAP_MAX_VAL; i++) {
		us_cmap[i / MAP_Y_SIZE][i & (MAP_X_SIZE - 1)] = MAP_SMAP_MAX_VAL - 1;
	}

	/* ��鴻�?��?����?���贋�遺��膣?��������ャ�� */
//	std::queue<stPOSITION> q;
//	QueryPerformanceCounter(&start);

	/* ���罔���亥�鴻��膈�蕭�膩����0���荐�絎? */
	setStep(uc_goalX, uc_goalY, 0);
	st_pos.x = uc_goalX;
	st_pos.y = uc_goalY;
	st_pos.step = 0;

	enqueue(pQueue,st_pos);

	/* 膈�蕭�膩��?����������篏��?? */
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
	uint8_t uc_goalX, 			///< [in] ?申S?申[?申?申X?申?申?申W
	uint8_t uc_goalY, 			///< [in] ?申S?申[?申?申Y?申?申?申W
	enMAP_ACT_MODE	en_type		///< [in] ?申v?申Z?申?申?申@?申i?申��鐃�?申?申?申g?申p?申j
){
	uint16_t		x, y, i;		// ?申?申?申[?申v?申��鐃�
	uint16_t		uc_dase;		// ?申���?申l
	uint16_t		uc_new;			// ?申V?申l
	uint16_t		uc_level;		// ?申?申?申?申?申?申
	uint8_t		uc_wallData;	// ?申��鐃�?申

	en_type = en_type;		// ?申R?申?申?申p?申C?申?申?申?申?申[?申j?申?申?申O?申?申?申?申i?申?申?申?申?申?申���鐃�j

	/* ?申?申?申?申?申?申?申}?申b?申v?申?申?申?申?申?申?申?申?申?申?申?申 */
	for ( i = 0; i < MAP_SMAP_MAX_VAL; i++ ){
		us_cmap[ i / MAP_Y_SIZE][ i & (MAP_X_SIZE-1) ] = MAP_SMAP_MAX_VAL - 1;
	}
	/* ?申��W?申n?申_?申��鐃�?申?申?申?申?申?申0?申����鐃� */
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

	/* ?申?申?申?申?申?申?申}?申b?申v?申?申?申?�� */
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
					/* ?申T?申?申?申?申?申s */
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
					/* ?申��Z?申?申?申s */
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
	enMAP_SEARCH_TYPE	en_calcType,	///< [in] ?申v?申Z?申?申?申@
	enMAP_HEAD_DIR* 	p_head			///< [out] ?申i?申s?申?申?申?申?申i?申��鐃�l?申j
){
	uint8_t		uc_wall;				// ?申��鐃�?申
	uint16_t		us_base;				// ?申?申?申?申?申?申?申D?申?申x?申?申?申?申l
	uint16_t		us_new;
	enMAP_HEAD_DIR	en_tmpHead;

	/* ?申?申?申?申?申v?申Z */
	// ?申?申?申?申?申?申MAP?申@
	if( CONTOUR_SYSTEM == en_calcType ){
		// ?申?申?申��鐃�4?申?申?申��鐃峻���I?申n?申����鐃�?申��鐃�?申?申?申?申?申?申?申Z?申o?申?申?申?申B
		// ?申?申?申?申?申?申?申A?申��鐃�?申��鐃�?申?申?申����鐃�?申?申��鐃�?申?申?申?申?申?申?申?申��??申����A?申?申?申��鐃�?申��I?申?申?申?申?申?申B
		// ?申@?申?申?申T?申?申?申?申?申,?申?申?申i ?申A?申?申?申T?申?申?申?申?申,?申?申?申?申 ?申B?申?申?申T?申?申?申?申?申,?申?申?申i ?申C?申?申?申T?申?申?申?申?申,?申?申?申?申
		uc_wall = g_sysMap[my][mx];
		us_base = MAP_SMAP_MAX_PRI_VAL;					// 16[?申?申?申]?申~16[?申?申?申]?申~4[?申?申?申?申]

		/* 4?申?申?申?申?申?申?申r */
		//	?申k?申?申?申?申?申��鐃�?申��m?申F
		if ( ( uc_wall & 1 ) == 0 ){
			us_new = us_cmap[my+1][mx] * 4 + 4;
			if ( ( g_sysMap[my+1][mx] & 0xf0 ) != 0xf0 ) us_new = us_new - 2;
			if ( en_Head == NORTH ) us_new = us_new - 1;
			if ( us_new < us_base ){
				us_base = us_new;
				en_tmpHead = NORTH;
			}
		}
		//	?申?申?申?申?申?申?申��鐃�?申��m?申F
		if ( ( uc_wall & 2 ) == 0 ){
			us_new = us_cmap[my][mx+1] * 4 + 4;
			if ( ( g_sysMap[my][mx+1] & 0xf0 ) != 0xf0 ) us_new = us_new - 2;
			if ( en_Head == EAST) us_new = us_new - 1;
			if ( us_new < us_base ){
				us_base = us_new;
				en_tmpHead = EAST;
			}
		}
		//	?申?申?申?申?申?申��鐃�?申��m?申F
		if ( ( uc_wall & 4 ) == 0 ){
			us_new = us_cmap[my-1][mx] * 4 + 4;
			if ( ( g_sysMap[my-1][mx] & 0xf0 ) != 0xf0) us_new = us_new - 2;
			if ( en_Head == SOUTH ) us_new = us_new - 1;
			if ( us_new < us_base ){
				us_base = us_new;
				en_tmpHead = SOUTH;
			}
		}
		//	?申?申?申?申?申?申?申��鐃�?申��m?申F
		if ( ( uc_wall & 8 ) == 0 ){
			us_new = us_cmap[my][mx-1] * 4 + 4;
			if ( ( g_sysMap[my][mx-1] & 0xf0 ) != 0xf0 ) us_new = us_new - 2;
			if ( en_Head == WEST ) us_new = us_new - 1;
			if ( us_new < us_base ){
				us_base = us_new;
				en_tmpHead = WEST;
			}
		}
		
		*p_head = (enMAP_HEAD_DIR)( (en_tmpHead - en_Head) & 3 );		// ?申��鐃�?申?申?申?申
		
	}
	// ?申?申?申?申?申?申@?申w?申?申��鐃�
	else{
		*p_head = (enMAP_HEAD_DIR)0;
	}

}

void MAP_refMousePos( 
	enMAP_HEAD_DIR 			en_head			///< [in] ?申i?申s?申?申?申?申
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
	enMAP_HEAD_DIR 	en_head,		///< [in] ?申?申?申��i?申s?申?申?申?申?申i?申}?申E?申X?申i?申s?申?申?申?申?申?申k?申��鐃�?申��鐃�?申?申j
	bool*			p_type			///< [in] FALSE: ?申P?申?申��O?申i?申?申��ATURE:?申?申?申?申��O?申i?申?申?申
){
	*p_type = TRUE;
	f_MoveBackDist = 0;				// ?申��鐃�?申?申?申?申?申?申?申?申?申Z?申l?申N?申?申?申A
	
	/* ?申?申?申?申 */
	switch( en_head ){

		/* ?申?申?申������O?申i */
		case NORTH:
			*p_type = FALSE;
			MOT_goBlock_Const( 1 );				// 1?申?申?申O?申i
			break;
		// ?申E?申��鐃�?申?��鐃�
		case EAST:
			MOT_goBlock_FinSpeed( 0.5, 0 );		// ?申?申?申?申?申O?申i
			MOT_turn(MOT_R90);									// ?申E90?申x?申?申?申?申
			break;
		// ?申?申?申��鐃�?申?��鐃�
		case WEST:
			MOT_goBlock_FinSpeed( 0.5, 0 );		// ?申?申?申?申?申O?申i
			MOT_turn(MOT_L90);									// ?申E90?申x?申?申?申?申
			break;
		// ?申?申?申]?申?申?申����鐃�
		case SOUTH:
			MOT_goBlock_FinSpeed( 0.5, 0 );		// ?申?申?申?申?申O?申i
			MOT_turn(MOT_R180);									// ?申E180?申x?申?申?申?申
			
			/* ?申��鐃�?申��p?申?申?申?申?申?申i?申?申?申����鐃�?申?申?申?申?申?申?申?申o?申b?申N?申{?申��鐃�?申?申?申?申?申?申?申?申?申Z?申?申?申?申j */
			if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) )  ||		// ?申k?申?申?申?申?申?申?申��鐃�?申��k?申����鐃�?申?申?申?申
				( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
				( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申��鐃�?申��鐃宿���鐃�?申?申?申?申
				( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) ) 			// ?申?申?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
			){
				MOT_goHitBackWall();					// ?申o?申b?申N?申?申?申?申
				f_MoveBackDist = MOVE_BACK_DIST;		// ?申o?申b?申N?申?申?申?申?申?申?申����鐃�?申?申?申?申[?申?申?申]?申?申?申?申?申Z
			}
			break;
		default:
			break;
	}
	
	/* ?申O?申i?申?申?申��p?申?申?申[?申?申?申?申?申[?申X?申@?申\?申?申?申?申?申?申?申�??申?申W?申?申?申[?申?申?申?申?申��鐃�?申?申����鐃夙�鐃� */
/*	if( ( TRUE == DCMC_isPowerRelease() ) && ( en_head == NORTH ) ){
		
		MOT_goBack_Const( MOT_BACK_POLE );					// ?申P?申��O?申��鐃�?申����鐃�?申
		MAP_makeMapData();									// ?申��f?申[?申^?申?申?申?申?申?申H?申f?申[?申^?申?申?申?��			?申?申 ?申?申?申?申?申��f?申[?申^?申?��?申?申?申~?申X?申?申?申��鐃�?申?申
		MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);			// ?申?申?申?申?申?申MAP?申@?申��i?申s?申?申?申?申?申?申?申Z?申o			?申?申 ?申?申?申?申?申MAP?申?申?申?��
		MAP_moveNextBlock(en_head, p_type);					// ?申?申?申?申?申P?申x?申����o?申?申?申i?申?申?申��鐃�?申����鐃�?申j
	}
	else{*/
		/* ?申i?申s?申?申?申?申?申X?申V */
		en_Head = (enMAP_HEAD_DIR)( (en_Head + en_head) & (MAP_HEAD_DIR_MAX-1) );
//	}
}

void MAP_moveNextBlock_Sura( 
	enMAP_HEAD_DIR 	en_head,		///< [in] ?申?申?申��i?申s?申?申?申?申?申i?申}?申E?申X?申i?申s?申?申?申?申?申?申k?申��鐃�?申��鐃�?申?申j
	bool*			p_type,			///< [in] FALSE: ?申P?申?申��O?申i?申?申��ATURE:?申?申?申?申��O?申i?申?申?申
	bool			bl_resume		///< [in] FALSE: ?申?申?申W?申?申?申[?申?申?申?申?申?申������鐃�?申ATURE:?申?申?申W?申?申?申[?申?申?申?申?申?申
){
	*p_type = FALSE;
	f_MoveBackDist = 0;				// ?申��鐃�?申?申?申?申?申?申?申?申?申Z?申l?申N?申?申?申A
	
	/* ?申?申?申?申 */
	switch( en_head ){

		// ?申?申?申������O?申i
		case NORTH:
			
			/* ?申?申?申W?申?申?申[?申?申?申?申?申?申������鐃� */
			if( bl_resume == FALSE ){
		
				MOT_goBlock_Const( 1 );					// 1?申?申?申O?申i
//				uc_SlaCnt = 0;							// ?申X?申?申?申?申?申[?申?申?申?申?申��鐃�?申��鐃�
			}
			/* ?申?申?申W?申?申?申[?申?申?申?申?申?申 */
			else{
				MOT_goBlock_FinSpeed( 1.0f, SEARCH_SPEED );		// ?申?申?申?申?申O?申i(?申o?申b?申N?申����鐃�?申��鐃�?申��鐃�)
//				uc_SlaCnt = 0;										// ?申X?申?申?申?申?申[?申?申?申?申?申��鐃�?申��鐃�
			}
			break;

		// ?申E?申��X?申?申?申?申?申[?申?申?申?申?申?申
		case EAST:
			if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?申k?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申?申��鐃�?申��鐃宿���鐃�?申?申?申?申
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) ) 			// ?申?申?申?申?申?申?申?申?申��鐃�?申��k?申����鐃�?申?申?申?申
				){
				uc_dist_control = 0.01;
				}
			else{
				uc_dist_control = 0;
			}
			if( uc_SlaCnt < SLA_count ){
				MOT_goSla( MOT_R90S, PARAM_getSra( SLA_90 ) );	// ?申E?申X?申?申?申?申?申[?申?申
				uc_SlaCnt++;
			}
			else{
				/* ?申��鐃�?申��p?申?申?申?申?申?申i?申?申?申����鐃�?申?申?申?申?申?申?申?申o?申b?申N?申{?申��鐃�?申?申?申?申?申?申?申?申?申Z?申?申?申?申j */
				if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?申k?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申?申��鐃�?申��k?申����鐃�?申?申?申?申
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) ) 			// ?申?申?申?申?申?申?申?申?申��鐃�?申��鐃宿���鐃�?申?申?申?申
				){
					MOT_goBlock_FinSpeed( 0.5, 0 );			// ?申?申?申?申?申O?申i
					MOT_turn(MOT_R90);						// ?申E90?申x?申?申?申?申
					uc_SlaCnt = 0;
					MOT_goHitBackWall();					// ?申o?申b?申N?申?申?申?申
					f_MoveBackDist = MOVE_BACK_DIST;		// ?申o?申b?申N?申?申?申?申?申?申?申����鐃�?申?申?申?申[?申?申?申]?申?申?申?申?申Z
					*p_type = TRUE;							// ?申?申?申��鐃�?申?申��i?申{?申o?申b?申N?申j?申?申?申i?申��鐃�
				}
				else{
					MOT_goSla( MOT_R90S, PARAM_getSra( SLA_90 ) );	// ?申E?申X?申?申?申?申?申[?申?申
					uc_SlaCnt++;
				}
			}
			break;

		// ?申?申?申��X?申?申?申?申?申[?申?申?申?申?申?申
		case WEST:
			if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?申k?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申?申��鐃�?申��k?申����鐃�?申?申?申?申
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) ) 			// ?申?申?申?申?申?申?申?申?申��鐃�?申��鐃宿���鐃�?申?申?申?申
				){
				uc_dist_control = 0.01;
				}
			else{
				uc_dist_control = 0;
			}
			if( uc_SlaCnt < SLA_count ){
				MOT_goSla( MOT_L90S, PARAM_getSra( SLA_90 ) );	// ?申?申?申X?申?申?申?申?申[?申?申
				uc_SlaCnt++;
			}
			else{
				/* ?申��鐃�?申��p?申?申?申?申?申?申i?申?申?申����鐃�?申?申?申?申?申?申?申?申o?申b?申N?申{?申��鐃�?申?申?申?申?申?申?申?申?申Z?申?申?申?申j */
				if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?申k?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申?申��鐃�?申��鐃宿���鐃�?申?申?申?申
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) ) 			// ?申?申?申?申?申?申?申?申?申��鐃�?申��k?申����鐃�?申?申?申?申
				){
					MOT_goBlock_FinSpeed( 0.5, 0 );		// ?申?申?申?申?申O?申i
					MOT_turn(MOT_L90);					// ?申E90?申x?申?申?申?申
					uc_SlaCnt = 0;
					MOT_goHitBackWall();					// ?申o?申b?申N?申?申?申?申
					f_MoveBackDist = MOVE_BACK_DIST;		// ?申o?申b?申N?申?申?申?申?申?申?申����鐃�?申?申?申?申[?申?申?申]?申?申?申?申?申Z
					*p_type = TRUE;							// ?申?申?申��鐃�?申?申��i?申{?申o?申b?申N?申j?申?申?申i?申��鐃�
				}
				else{
					MOT_goSla( MOT_L90S, PARAM_getSra( SLA_90 ) );	// ?申?申?申X?申?申?申?申?申[?申?申
					uc_SlaCnt++;
				}
			}
			break;

		// ?申?申?申]?申?申?申����鐃�
		case SOUTH:
			MOT_goBlock_FinSpeed( 0.5, 0 );			// ?申?申?申?申?申O?申i
			MOT_turn(MOT_R180);									// ?申E180?申x?申?申?申?申
			uc_SlaCnt = 0;
			
			/* ?申��鐃�?申��p?申?申?申?申?申?申i?申?申?申����鐃�?申?申?申?申?申?申?申?申o?申b?申N?申{?申��鐃�?申?申?申?申?申?申?申?申?申Z?申?申?申?申j */
			if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) )  ||		// ?申k?申?申?申?申?申?申?申��鐃�?申��k?申����鐃�?申?申?申?申
				( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
				( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申��鐃�?申��鐃宿���鐃�?申?申?申?申
				( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) ) 			// ?申?申?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
			){
				MOT_goHitBackWall();					// ?申o?申b?申N?申?申?申?申
				f_MoveBackDist = MOVE_BACK_DIST;		// ?申o?申b?申N?申?申?申?申?申?申?申����鐃�?申?申?申?申[?申?申?申]?申?申?申?申?申Z
			}
			*p_type = TRUE;								// ?申?申?申��鐃�?申?申��{?申o?申b?申N?申?申?申i?申��鐃�
			break;
			
		default:
			break;
	}
	
	/* ?申O?申i?申?申?申��p?申?申?申[?申?申?申?申?申[?申X?申@?申\?申?申?申?申?申?申?申�??申?申W?申?申?申[?申?申?申?申?申��鐃�?申?申����鐃夙�鐃� */
/*	if( ( TRUE == DCMC_isPowerRelease() ) && ( en_head == NORTH ) ){
		
		MOT_goBack_Const( MOT_BACK_POLE );					// ?申P?申��O?申��鐃�?申����鐃�?申
		MAP_makeMapData();									// ?申��f?申[?申^?申?申?申?申?申?申H?申f?申[?申^?申?申?申?��			?申?申 ?申?申?申?申?申��f?申[?申^?申?��?申?申?申~?申X?申?申?申��鐃�?申?申
		MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);			// ?申?申?申?申?申?申MAP?申@?申��i?申s?申?申?申?申?申?申?申Z?申o			?申?申 ?申?申?申?申?申MAP?申?申?申?��
		MAP_moveNextBlock_Sura(en_head, p_type, TRUE );		// ?申?申?申?申?申P?申x?申����o?申?申?申i?申?申?申��鐃�?申����鐃�?申j
	}
	else{*/
		/* ?申i?申s?申?申?申?申?申X?申V */
		en_Head = (enMAP_HEAD_DIR)( (en_Head + en_head) & (MAP_HEAD_DIR_MAX-1) );
//	}
}

void MAP_actGoal( void )
{	
	MOT_goBlock_FinSpeed( 0.5, 0 );			// ?申?申?申?申?申O?申i
	LL_mDelay(500);
	MOT_turn(MOT_R180);										// ?申E180?申x?申?申?申?申
	LL_mDelay(500);
	
//	MAP_SaveMapData();						// ?申?申?申H?申?申?申��o?申b?申N?申A?申b?申v
	log_flag_off();
	MAP_actGoalLED();
	
	en_Head = (enMAP_HEAD_DIR)( (en_Head + 2) & (MAP_HEAD_DIR_MAX-1) );			//	?申i?申s?申?申?申?申?申X?申V

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
	uint16_t		x, y, i;		// ?申?申?申[?申v?申��鐃�
	uint16_t		uc_dase;		// ?申���?申l
	uint16_t		uc_new;			// ?申V?申l
	uint16_t		uc_level;		// ?申?申?申?申?申?申
	uint8_t		uc_wallData;	// ?申��鐃�?申


	queue_t queue;
	queue_t* pQueue = &queue;

	initQueue(pQueue);

	/* ?申?申?申?申?申?申?申}?申b?申v?申?申?申?申?申?申?申?申?申?申?申?申 */
	for (i = 0; i < MAP_SMAP_MAX_VAL; i++) {
		us_cmap[i / MAP_Y_SIZE][i & (MAP_X_SIZE - 1)] = MAP_SMAP_MAX_VAL - 1;
	}
	/* ?申��W?申n?申_?申��鐃�?申?申?申?申?申?申0?申����鐃� */
//	us_cmap[0][0] = 0;
	setStep(0, 0, 0);
	st_pos.x = 0;
	st_pos.y = 0;
	st_pos.step = 0;

	enqueue(pQueue,st_pos);

	/* ?申?申?申?申?申?申?申}?申b?申v?申?申?申?�� */
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

	/* ?申?申?申?申 */
	switch (en_head) {

		/* ?申?申?申������O?申i */
	case NORTH:
//		*p_type = FALSE;
//		LED = LED6;
		if (MAP_KnownAcc() == FALSE) {					// ?申?申?申��i?申��鐃醇��?申?申?申T?申?申?申����鐃�
			if (st_known.bl_Known == TRUE){
				if (st_known.uc_StrCnt < 2) {
					MOT_goBlock_Const(1);					// 1?申?申?申�����?申��鐃�?申?申?申����鐃�
				}
				else {
					MOT_setTrgtSpeed(MAP_KNOWN_ACC_SPEED);									// ?申?申?申m?申?申��鐃�?申?申?申?申?申?申��鐃�?申����W?申?申?申x	
					MOT_goBlock_FinSpeed((float)(st_known.uc_StrCnt), SEARCH_SPEED);				// n?申?申?申O?申i
					MOT_setTrgtSpeed(SEARCH_SPEED);										// ?申��W?申?申?申x?申?申?申f?申t?申H?申?申?申g?申l?申����鐃�
				}
			}
			MOT_goBlock_Const(1);	////////////////////
			st_known.uc_StrCnt = 0;
			st_known.bl_Known = FALSE;

		}
		else {

			st_known.uc_StrCnt++;			// ?申��鐃�?申?申?申��鐃�?申Z
			st_known.bl_Known = TRUE;
		}

		break;

		/* ?申E?申��鐃�?申?��鐃� */
	case EAST:
//		LED = LED8;
		if (st_known.bl_Known == TRUE) {		// ?申?申?申?申?申?申?申?申?申?申?申?申
			if (st_known.uc_StrCnt < 2) {
				MOT_goBlock_Const(1);					// 1?申?申?申�����?申��鐃�?申?申?申����鐃�
			}
			else {
//				LED = LED_ALL_ON;
				MOT_setTrgtSpeed(MAP_KNOWN_ACC_SPEED);									// ?申?申?申m?申?申��鐃�?申?申?申?申?申?申��鐃�?申����W?申?申?申x	
				MOT_goBlock_FinSpeed((float)(st_known.uc_StrCnt), SEARCH_SPEED);				// n?申?申?申O?申i
				MOT_setTrgtSpeed(SEARCH_SPEED);										// ?申��W?申?申?申x?申?申?申f?申t?申H?申?申?申g?申l?申����鐃�
//				LED = LED_ALL_OFF;
			}
			st_known.uc_StrCnt = 0;		/////////////////////////////////////////
			st_known.bl_Known = FALSE;
		}
		if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?申k?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申?申��鐃�?申��鐃宿���鐃�?申?申?申?申
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) ) 			// ?申?申?申?申?申?申?申?申?申��鐃�?申��k?申����鐃�?申?申?申?申
			){
			uc_dist_control = 0.01;
			}
		else{
			uc_dist_control = 0;
		}
		if( uc_SlaCnt < SLA_count ){
				MOT_goSla( MOT_R90S, PARAM_getSra( SLA_90 ) );	// ?申E?申X?申?申?申?申?申[?申?申
				uc_SlaCnt++;
			}
			else{
				f_MoveBackDist = 0;
				/* ?申��鐃�?申��p?申?申?申?申?申?申i?申?申?申����鐃�?申?申?申?申?申?申?申?申o?申b?申N?申{?申��鐃�?申?申?申?申?申?申?申?申?申Z?申?申?申?申j */
				if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?申k?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申?申��鐃�?申��k?申����鐃�?申?申?申?申
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) ) 			// ?申?申?申?申?申?申?申?申?申��鐃�?申��鐃宿���鐃�?申?申?申?申
				){
					MOT_goBlock_FinSpeed( 0.5, 0 );			// ?申?申?申?申?申O?申i
					MOT_turn(MOT_R90);						// ?申E90?申x?申?申?申?申
					uc_SlaCnt = 0;
					MOT_goHitBackWall();					// ?申o?申b?申N?申?申?申?申
					f_MoveBackDist = MOVE_BACK_DIST;		// ?申o?申b?申N?申?申?申?申?申?申?申����鐃�?申?申?申?申[?申?申?申]?申?申?申?申?申Z
					*p_type = TRUE;							// ?申?申?申��鐃�?申?申��i?申{?申o?申b?申N?申j?申?申?申i?申��鐃�
				}
				else{
					MOT_goSla( MOT_R90S, PARAM_getSra( SLA_90 ) );	// ?申E?申X?申?申?申?申?申[?申?申
					uc_SlaCnt++;
				}
			}
		break;

		/* ?申?申?申��鐃�?申?��鐃� */
	case WEST:
		if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?申k?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申?申��鐃�?申��k?申����鐃�?申?申?申?申
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) ) 			// ?申?申?申?申?申?申?申?申?申��鐃�?申��鐃宿���鐃�?申?申?申?申
			){
			uc_dist_control = 0.01;
			}
		else{
			uc_dist_control = 0;
		}
		if (st_known.bl_Known == TRUE) {		// ?申?申?申?申?申?申?申?申?申?申?申?申
			if (st_known.uc_StrCnt < 2) {
				MOT_goBlock_Const(1);					// 1?申?申?申�����?申��鐃�?申?申?申����鐃�
			}
			else {
//				LED = LED_ALL_ON;
				MOT_setTrgtSpeed(MAP_KNOWN_ACC_SPEED);									// ?申?申?申m?申?申��鐃�?申?申?申?申?申?申��鐃�?申����W?申?申?申x	
				MOT_goBlock_FinSpeed((float)(st_known.uc_StrCnt), SEARCH_SPEED);				// n?申?申?申O?申i
				MOT_setTrgtSpeed(SEARCH_SPEED);										// ?申��W?申?申?申x?申?申?申f?申t?申H?申?申?申g?申l?申����鐃�
//				LED = LED_ALL_OFF;
			}
			st_known.uc_StrCnt = 0;			//////////////////////////////////////
			st_known.bl_Known = FALSE;
		}

		if( uc_SlaCnt < SLA_count ){
				MOT_goSla( MOT_L90S, PARAM_getSra( SLA_90 ) );	// ?申?申?申X?申?申?申?申?申[?申?申
				uc_SlaCnt++;
			}
			else{
				f_MoveBackDist = 0;
				/* ?申��鐃�?申��p?申?申?申?申?申?申i?申?申?申����鐃�?申?申?申?申?申?申?申?申o?申b?申N?申{?申��鐃�?申?申?申?申?申?申?申?申?申Z?申?申?申?申j */
				if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?申k?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申?申��鐃�?申��鐃宿���鐃�?申?申?申?申
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?申?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) ) 			// ?申?申?申?申?申?申?申?申?申��鐃�?申��k?申����鐃�?申?申?申?申
				){
					MOT_goBlock_FinSpeed( 0.5, 0 );		// ?申?申?申?申?申O?申i
					MOT_turn(MOT_L90);					// ?申E90?申x?申?申?申?申
					uc_SlaCnt = 0;
					MOT_goHitBackWall();					// ?申o?申b?申N?申?申?申?申
					f_MoveBackDist = MOVE_BACK_DIST;		// ?申o?申b?申N?申?申?申?申?申?申?申����鐃�?申?申?申?申[?申?申?申]?申?申?申?申?申Z
					*p_type = TRUE;							// ?申?申?申��鐃�?申?申��i?申{?申o?申b?申N?申j?申?申?申i?申��鐃�
				}
				else{
					MOT_goSla( MOT_L90S, PARAM_getSra( SLA_90 ) );	// ?申?申?申X?申?申?申?申?申[?申?申
					uc_SlaCnt++;
				}
			}
		break;

		/* ?申?申?申]?申?申?申����鐃� */
	case SOUTH:
//		LED = LED_ALL_ON;
		MOT_goBlock_FinSpeed(0.5, 0);			// ?申?申?申?申?申O?申i
		MOT_turn(MOT_R180);									// ?申E180?申x?申?申?申?申
		uc_SlaCnt = 0;

		/* ?申��鐃�?申��p?申?申?申?申?申?申i?申?申?申����鐃�?申?申?申?申?申?申?申?申o?申b?申N?申{?申��鐃�?申?申?申?申?申?申?申?申?申Z?申?申?申?申j */
		if (((en_Head == NORTH) && ((g_sysMap[my][mx] & 0x01) != 0)) ||		// ?申k?申?申?申?申?申?申?申��鐃�?申��k?申����鐃�?申?申?申?申
			((en_Head == EAST) && ((g_sysMap[my][mx] & 0x02) != 0)) ||		// ?申?申?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
			((en_Head == SOUTH) && ((g_sysMap[my][mx] & 0x04) != 0)) ||		// ?申?申?申?申?申?申?申?申��鐃�?申��鐃宿���鐃�?申?申?申?申
			((en_Head == WEST) && ((g_sysMap[my][mx] & 0x08) != 0)) 			// ?申?申?申?申?申?申?申?申?申��鐃�?申��鐃�?申����鐃�?申?申?申?申
			) {
			MOT_goHitBackWall();					// ?申o?申b?申N?申?申?申?申
			f_MoveBackDist = MOVE_BACK_DIST;	// ?申o?申b?申N?申?申?申?申?申?申?申����鐃�?申?申?申?申[?申?申?申]?申?申?申?申?申Z
		}
		*p_type = TRUE;								// ?申?申?申��鐃�?申?申��{?申o?申b?申N?申?申?申i?申��鐃�
		break;

	default:
		break;
	}

	/* ?申i?申s?申?申?申?申?申X?申V */
//	en_Head = (enMAP_HEAD_DIR)( (en_Head + en_head) & (MAP_HEAD_DIR_MAX-1) );
	en_Head = (enMAP_HEAD_DIR)(((uint8_t)en_Head + (uint8_t)en_head) & (MAP_HEAD_DIR_MAX - 1));
}

void MAP_searchGoal(
	uint8_t 			uc_trgX, 		///< [in] ?申��Wx?申?申?申W
	uint8_t 			uc_trgY, 		///< [in] ?申��Wy?申?申?申W 
	enMAP_ACT_MODE 	en_type, 		///< [in] ?申T?申?申?申?申?申@
	enSEARCH_MODE	en_search 		///< [in] ?申T?申?申?申?申?申@
){
	enMAP_HEAD_DIR	en_head = NORTH;
	bool		bl_type = TRUE;			// ?申?申?申����u?申AFALSE: ?申P?申?申��O?申i?申?申��ATURE:?申?申?申?申��O?申i?申?申?申
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

//	SYS_setDisable( SYS_MODE );				// ?申?申?申[?申h?申��X?申��~

	MOT_setTrgtSpeed(SEARCH_SPEED);		// ?申��W?申?申?申x
	MOT_setNowSpeed( 0.0f );
	f_MoveBackDist = 0;
	uc_SlaCnt = 0;
	if(uc_trgX == GOAL_MAP_X && uc_trgY == GOAL_MAP_Y){
		f_MoveBackDist = MOVE_BACK_DIST;
	}
	
	log_flag_on();	//?申?申?申O?申��鐃�?申X?申^?申[?申g?申i?申?申?��?申���鐃�j
	
	/* ?申?申?申H?申T?申?申 */
	while(1){
		MAP_refMousePos( en_Head );								// ?申?申?申W?申X?申V
//		MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?申?申?申?申?申?申?申}?申b?申v?申?申?申?申?申
		
		/* ?申?申?申M?申n?申?申?申?申T?申?申 */
		if( SEARCH_TURN == en_search ){
//			MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?申?申?申?申?申?申?申}?申b?申v?申?申?申?申?申
			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
			if( TRUE == bl_type ){
				MOT_goBlock_FinSpeed( 0.5 + f_MoveBackDist, SEARCH_SPEED );		// ?申?申?申?申?申O?申i(?申o?申b?申N?申����鐃�?申��鐃�?申��鐃�)
			}
			MAP_makeMapData();												// ?申��f?申[?申^?申?申?申?申?申?申H?申f?申[?申^?申?申?申?��			?申?申 ?申?申?申?申?申��f?申[?申^?申?��?申?申?申~?申X?申?申?申��鐃�?申?申
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);						// ?申?申?申?申?申?申MAP?申@?申��i?申s?申?申?申?申?申?申?申Z?申o			?申?申 ?申?申?申?申?申MAP?申?申?申?��
			
			/* ?申?申?申��鐃�?申����鐃� */
			if(( mx == uc_trgX ) && ( my == uc_trgY )){
				MAP_actGoal();										// ?申S?申[?申?申?申?申?申��鐃�?申?申
				break;
			}
			else{
				MAP_moveNextBlock(en_head, &bl_type);				// ?申?申?申��鐃�?申����鐃�								?申?申 ?申?申?申?申?申��鐃�?申���??申?申?申?申[?申X?申`?申F?申b?申N?申{?申����x?申?��?申{?申?申?申?申?申?申?申{?申?申?申M?申n?申?申?申?��鐃�
			}
		}
		/* ?申X?申?申?申?申?申[?申?申?申T?申?申 */
		else if( SEARCH_SURA == en_search ){
//			MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?申?申?申?申?申?申?申}?申b?申v?申?申?申?申?申
			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
			if( TRUE == bl_type ){
				
				MOT_goBlock_FinSpeed( 0.5 + f_MoveBackDist, SEARCH_SPEED );		// ?申?申?申?申?申O?申i(?申o?申b?申N?申����鐃�?申��鐃�?申��鐃�)
			}
			MAP_makeMapData();		// ?申��f?申[?申^?申?申?申?申?申?申H?申f?申[?申^?申?申?申?��
			
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);				// ?申?申?申?申?申?申MAP?申@?申��i?申s?申?申?申?申?申?申?申Z?申o			?申?申 ?申?申?申?申?申MAP?申?申?申?��
			
			/* ?申?申?申��鐃�?申����鐃� */
			if(( mx == uc_trgX ) && ( my == uc_trgY )){
				MAP_actGoal();										// ?申S?申[?申?申?申?申?申��鐃�?申?申
				break;
			}
			else{
				MAP_moveNextBlock_Sura(en_head, &bl_type, FALSE );	// ?申?申?申��鐃�?申����鐃�						?申?申 ?申?申?申?申?申��鐃�?申���??申?申?申?申[?申X?申`?申F?申b?申N?申{?申����x?申?��?申{?申?申?申?申?申?申?申{?申?申?申M?申n?申?申?申?��鐃�
//				MAP_moveNextBlock_acc(en_head, &bl_type);
			}
		}
		/* ?申A?申��T?申?申 */
		else if (SEARCH_RETURN == en_search) {
			
			if( TRUE == bl_type ){
				
				MOT_goBlock_FinSpeed( 0.5 + f_MoveBackDist, SEARCH_SPEED );		// ?申?申?申?申?申O?申i(?申o?申b?申N?申����鐃�?申��鐃�?申��鐃�)
			}
			MAP_makeMapData();		// ?申��f?申[?申^?申?申?申?申?申?申H?申f?申[?申^?申?申?申?��
						
			MAP_makeReturnContourMap(uc_staX,uc_staY);
			MAP_searchCmdList(uc_staX, uc_staY, en_Head, uc_goalX, uc_goalX, &en_endDir);
			uc_trgX = Return_X;
			uc_trgY = Return_Y;
//			MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?申?申?申?申?申?申?申}?申b?申v?申?申?申?申?申
			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);	
			/* ?申?申?申��鐃�?申����鐃� */
//			if ((us_cmap[my][mx] == 0)||((g_sysMap[uc_trgY][uc_trgX]&0xf0) == 0xf0)) {
			if ((mx == 0)&&(my == 0)){
				MAP_actGoal();
				break;
			}
//			}
			else {
				MAP_moveNextBlock_Sura(en_head, &bl_type, FALSE);	// ?申?申?申��鐃�?申����鐃�						?申?申 ?申?申?申?申?申��鐃�?申���??申?申?申?申[?申X?申`?申F?申b?申N?申{?申����x?申?��?申{?申?申?申?申?申?申?申{?申?申?申M?申n?申?申?申?��鐃�
//				MAP_moveNextBlock_acc(en_head, &bl_type);
			}
//			LED_count(uc_trgY);
		}

		
		/* ?申r?申?申?申��鐃�?申?申s?申\?申����鐃�?申?申 */
		if( SYS_isOutOfCtrl() == TRUE ){
			CTRL_stop();
			DCM_brakeMot( DCM_R );		// ?申u?申?申?申[?申L
			DCM_brakeMot( DCM_L );		// ?申u?申?申?申[?申L
			
			/* ?申?申?申H?申��A?申?申?申?申?申?申?申?申 */
			en_Head		= NORTH;
			mx			= 0;
			my			= 0;
			f_MoveBackDist = 0;
			
			// DCMC?申��鐃�?申�??申?申W?申?申?申[?申?申?申��鐃�?申��N?申?申?申A?申����}?申?申~?申?申?申s?申?申?申��鐃�?申?申B
			break;
		}
	}
	search_flag = FALSE;
	LL_mDelay(1000);
//	SYS_setEnable( SYS_MODE );				// ?申?申?申[?申h?申��X?申L?申?申
}

void MAP_searchGoalKnown(
	uint8_t 			uc_trgX, 		///< [in] ?申��Wx?申?申?申W
	uint8_t 			uc_trgY, 		///< [in] ?申��Wy?申?申?申W 
	enMAP_ACT_MODE 	en_type, 		///< [in] ?申T?申?申?申?申?申@
	enSEARCH_MODE	en_search 		///< [in] ?申T?申?申?申?申?申@
){
	enMAP_HEAD_DIR	en_head = NORTH;
	bool		bl_type = TRUE;			// ?申?申?申����u?申AFALSE: ?申P?申?申��O?申i?申?申��ATURE:?申?申?申?申��O?申i?申?申?申
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

//	SYS_setDisable( SYS_MODE );				// ?申?申?申[?申h?申��X?申��~

	MOT_setTrgtSpeed(SEARCH_SPEED);		// ?申��W?申?申?申x
	MOT_setNowSpeed( 0.0f );
	f_MoveBackDist = 0;
	uc_SlaCnt = 0;
	if(uc_trgX == GOAL_MAP_X && uc_trgY == GOAL_MAP_Y){
		f_MoveBackDist = MOVE_BACK_DIST;
	}
	
	log_flag_on();	//?申?申?申O?申��鐃�?申X?申^?申[?申g?申i?申?申?��?申���鐃�j
	
	/* ?申?申?申H?申T?申?申 */
	while(1){
		MAP_refMousePos( en_Head );								// ?申?申?申W?申X?申V
//		MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?申?申?申?申?申?申?申}?申b?申v?申?申?申?申?申
		
		/* ?申?申?申M?申n?申?申?申?申T?申?申 */
		if( SEARCH_TURN == en_search ){
//			MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?申?申?申?申?申?申?申}?申b?申v?申?申?申?申?申
			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
			if( TRUE == bl_type ){
				MOT_goBlock_FinSpeed( 0.5 + f_MoveBackDist, SEARCH_SPEED );		// ?申?申?申?申?申O?申i(?申o?申b?申N?申����鐃�?申��鐃�?申��鐃�)
			}
			MAP_makeMapData();												// ?申��f?申[?申^?申?申?申?申?申?申H?申f?申[?申^?申?申?申?��			?申?申 ?申?申?申?申?申��f?申[?申^?申?��?申?申?申~?申X?申?申?申��鐃�?申?申
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);						// ?申?申?申?申?申?申MAP?申@?申��i?申s?申?申?申?申?申?申?申Z?申o			?申?申 ?申?申?申?申?申MAP?申?申?申?��
			
			/* ?申?申?申��鐃�?申����鐃� */
			if(( mx == uc_trgX ) && ( my == uc_trgY )){
				MAP_actGoal();										// ?申S?申[?申?申?申?申?申��鐃�?申?申
				break;
			}
			else{
				MAP_moveNextBlock(en_head, &bl_type);				// ?申?申?申��鐃�?申����鐃�								?申?申 ?申?申?申?申?申��鐃�?申���??申?申?申?申[?申X?申`?申F?申b?申N?申{?申����x?申?��?申{?申?申?申?申?申?申?申{?申?申?申M?申n?申?申?申?��鐃�
			}
		}
		/* ?申X?申?申?申?申?申[?申?申?申T?申?申 */
		else if( SEARCH_SURA == en_search ){
//			MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?申?申?申?申?申?申?申}?申b?申v?申?申?申?申?申
			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
			if( TRUE == bl_type ){
				
				MOT_goBlock_FinSpeed( 0.5 + f_MoveBackDist, SEARCH_SPEED );		// ?申?申?申?申?申O?申i(?申o?申b?申N?申����鐃�?申��鐃�?申��鐃�)
			}
			if (st_known.bl_Known != TRUE) {
				MAP_makeMapData();		// ?申��f?申[?申^?申?申?申?申?申?申H?申f?申[?申^?申?申?申?��
			}
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);				// ?申?申?申?申?申?申MAP?申@?申��i?申s?申?申?申?申?申?申?申Z?申o			?申?申 ?申?申?申?申?申MAP?申?申?申?��
			
			/* ?申?申?申��鐃�?申����鐃� */
			if(( mx == uc_trgX ) && ( my == uc_trgY )){
				MAP_actGoal();										// ?申S?申[?申?申?申?申?申��鐃�?申?申
				break;
			}
			else{
//				MAP_moveNextBlock_Sura(en_head, &bl_type, FALSE );	// ?申?申?申��鐃�?申����鐃�						?申?申 ?申?申?申?申?申��鐃�?申���??申?申?申?申[?申X?申`?申F?申b?申N?申{?申����x?申?��?申{?申?申?申?申?申?申?申{?申?申?申M?申n?申?申?申?��鐃�
				MAP_moveNextBlock_acc(en_head, &bl_type);
			}
		}
		/* ?申A?申��T?申?申 */
		else if (SEARCH_RETURN == en_search) {
			
			if( TRUE == bl_type ){
				
				MOT_goBlock_FinSpeed( 0.5 + f_MoveBackDist, SEARCH_SPEED );		// ?申?申?申?申?申O?申i(?申o?申b?申N?申����鐃�?申��鐃�?申��鐃�)
			}
			if (st_known.bl_Known != TRUE) {
				MAP_makeMapData();		// ?申��f?申[?申^?申?申?申?申?申?申H?申f?申[?申^?申?申?申?��
			}			
			MAP_makeReturnContourMap(uc_staX,uc_staY);
			MAP_searchCmdList(uc_staX, uc_staY, en_Head, uc_goalX, uc_goalX, &en_endDir);
			uc_trgX = Return_X;
			uc_trgY = Return_Y;
//			MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?申?申?申?申?申?申?申}?申b?申v?申?申?申?申?申
			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);	

			/* ?申?申?申��鐃�?申����鐃� */
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

		
		/* ?申r?申?申?申��鐃�?申?申s?申\?申����鐃�?申?申 */
		if( SYS_isOutOfCtrl() == TRUE ){
			CTRL_stop();
			DCM_brakeMot( DCM_R );		// ?申u?申?申?申[?申L
			DCM_brakeMot( DCM_L );		// ?申u?申?申?申[?申L
			
			/* ?申?申?申H?申��A?申?申?申?申?申?申?申?申 */
			en_Head		= NORTH;
			mx			= 0;
			my			= 0;
			f_MoveBackDist = 0;
			
			// DCMC?申��鐃�?申�??申?申W?申?申?申[?申?申?申��鐃�?申��N?申?申?申A?申����}?申?申~?申?申?申s?申?申?申��鐃�?申?申B
			break;
		}
	}
	search_flag = FALSE;
	LL_mDelay(1000);
//	SYS_setEnable( SYS_MODE );				// ?申?申?申[?申h?申��X?申L?申?申

}
