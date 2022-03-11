/*
 * search.c
 *
 *  Created on: 2021/02/04
 *      Author: shohe
 */


#include "search/search.h"

#define MAP_SMAP_MAX_VAL	( MAP_X_SIZE * MAP_Y_SIZE ) 			///< ?��?��?��?��?��?��map?��̍ő�l
#define MAP_SMAP_MAX_PRI_VAL	( MAP_SMAP_MAX_VAL * 4 )				///< ?��?��?��?��?��?��map?��̗D?��?��x?��ő�l

#define MOVE_BACK_DIST		(0.23f)

enMAP_HEAD_DIR	en_Head;										///< ?��}?��E?��X?��̐i?��s?��?��?��?�� 0:N 1:E 2:S 3:W
uint8_t		my;												///< ?��}?��E?��X?��̂w?��?��?��W
uint8_t		mx;												///< ?��}?��E?��X?��̂x?��?��?��W
float		f_MoveBackDist;									///< ?��Ǔ�?��ē�?��?��Ō�ނ�?��?��?��?��?��?��[?��?��?��]
uint8_t		uc_SlaCnt = 0;									// ?��X?��?��?��?��?��[?��?��?��A?��?��?��?��
uint8_t		uc_back[ MAP_Y_SIZE ][ MAP_X_SIZE ];			// ?��?��?��H?��f?��[?��^

uint8_t		GOAL_MAP_X;					//?��S?��[?��?��?��?��?��W?��ύX?��v?��?��?��O?��?��?��?��?��p?��?��
uint8_t		GOAL_MAP_Y;					//?��S?��[?��?��?��?��?��W?��ύX?��v?��?��?��O?��?��?��?��?��p?��?��

uint8_t		GOAL_SIZE;
//?��?��?��?��?��?��?��}?��b?��v?��?��?��X?��V?��?��?��~?��߂邽?��߂̈ړ�?��?��?��K?��?��ϐ�
uint8_t		uc_max_x = GOAL_MAP_X_def;
uint8_t		uc_max_y = GOAL_MAP_Y_def;

//TKR
/* ?��?��?��m?��?��ԉ�?��?�� */
typedef struct{
	uint8_t	uc_StrCnt;
	bool	bl_Known;
}stMAP_KNOWN;

stMAP_KNOWN		st_known = { 0,FALSE };

uint8_t			SLA_count = 30;

uint8_t near_wall = 0;

void MAP_init( void )
{
//	uint8_t uc_dummy[ MAP_Y_SIZE ][ MAP_X_SIZE ];			// ?��?��?��H?��f?��[?��^

	/* ?��?��?��W?��A?��?��?��?��?��A?��?��?��H?��?��?��?��?��?��?��?��?��?�� */
	en_Head		= NORTH;
	mx		= 0;
	my		= 0;
	MAP_clearMap();
	
	/* ?��?��?��s?��p?��̃p?��?��?��?��?��[?��^ */
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
			LL_mDelay(200);			// SW?��?��?��?��?��?��?��?��?��܂ő҂�
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
			LL_mDelay(200);			// SW?��?��?��?��?��?��?��?��?��܂ő҂�
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
	/* ?��?��?��W?��A?��?��?��?��?��A?��?��?��H?��?��?��?��?��?��?��?��?��?�� */
	en_Head		= NORTH;
	mx			= 0;
	my			= 0;
	MAP_clearMap();
	
	/* ?��?��?��s?��p?��̃p?��?��?��?��?��[?��^ */
	f_MoveBackDist = 0;
	uc_SlaCnt = 0;

//	Storage_Clear( sizeof(g_sysMap), ADR_MAP );			// ?��f?��[?��^?��Z?��[?��u
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
	/*  ?��ʏ�?��?��H  */
	/* ---------- */
	printf("\n\r  /* ---------- */   ");
	printf("\n\r  /*  ?��ʏ�?��?��H  */   ");
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
	/*  ?��?��?��?��?��?��?��?��?��H  */
	/* ---------- */
	printf("\n\r  /* ------------ */   ");
	printf("\n\r  /*  ?��?��?��?��?��?��?��?��?��H  */   ");
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

	/* ?��?��?��ׂẴ}?��b?��v?��f?��[?��^?��?��T?��?��?��?��Ԃɂ�?��?�� */
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

	// ?��Z?��?��?��T?��?��?���Ǐ�?��?��
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
	if(DIST_getNowVal(DIST_SEN_L_SIDE)>L_SIDE_REF){
		near_wall = 1;
	}
	if(DIST_getNowVal(DIST_SEN_R_SIDE)>R_SIDE_REF){
		near_wall = 1;
	}

	// ?��}?��E?��X?��̐i?��s?��?��?��?��?��ɂ�?�����?��ăZ?��?��?��T?��f?��[?��^?��?��?��ړ�?��?��?��ǃf?��[?��^?��Ƃ�?��?��
	if		( en_Head == EAST ){
		uc_wall = uc_wall >> 3;
	}
	else if ( en_Head == SOUTH ){
		uc_wall = uc_wall >> 2;
	}
	else if ( en_Head == WEST ){
		uc_wall = uc_wall >> 1;
	}

	//	?��T?��?��?��ς݃t?��?��?��O?��?��Ă�
	return ( uc_wall | 0xf0 );
}

void MAP_makeMapData( void )
{
	uint8_t uc_wall;

	//	?��?��?��s?��?��?��̕Ǐ�?��?��?��?��H?��?��?��ɏ�?��?��
	if ( ( mx == 0 ) && ( my == 0 ) ){
		uc_wall = 0xfe;
	}
	else{
		uc_wall = MAP_getWallData();
	}
	g_sysMap[my][mx] = uc_wall;

	//	?��ׂ̋�Ԃ̂l?��`?��o?��f?��[?��^?��?��?��X?��V?��?��?��?��
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
	uint8_t uc_goalX, 			///< [in] ?��S?��[?��?��X?��?��?��W
	uint8_t uc_goalY, 			///< [in] ?��S?��[?��?��Y?��?��?��W
	enMAP_ACT_MODE	en_type		///< [in] ?��v?��Z?��?��?��@?��i?��܂�?��?��?��g?��p?��j
){
	uint16_t		x, y, i;		// ?��?��?��[?��v?��ϐ�
	uint16_t		uc_dase;		// ?�����?��l
	uint16_t		uc_new;			// ?��V?��l
	uint16_t		uc_level;		// ?��?��?��?��?��?��
	uint8_t		uc_wallData;	// ?��Ǐ�?��

	en_type = en_type;		// ?��R?��?��?��p?��C?��?��?��?��?��[?��j?��?��?��O?��?��?��?��i?��?��?��?��?��?������j

	/* ?��?��?��?��?��?��?��}?��b?��v?��?��?��?��?��?��?��?��?��?��?��?�� */
	for ( i = 0; i < MAP_SMAP_MAX_VAL; i++ ){
		us_cmap[ i / MAP_Y_SIZE][ i & (MAP_X_SIZE-1) ] = MAP_SMAP_MAX_VAL - 1;
	}
	/* ?��ڕW?��n?��_?��̓�?��?��?��?��?��?��0?��ɐݒ� */
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

	/* ?��?��?��?��?��?��?��}?��b?��v?��?��?��?�� */
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
					/* ?��T?��?��?��?��?��s */
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
					/* ?��ŒZ?��?��?��s */
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
	/* (x, y) がフィールド�??か確�? */
	if (x < 0 || y < 0 || x >= MAP_X_SIZE || y >= MAP_Y_SIZE) {
		printf( "referred to out of field\r\n");
		return;
	}
	us_cmap[y][x] = step;
}

void  MAP_makeContourMap_kai2(
	uint8_t uc_goalX, 			///< [in] ゴールX座�?
	uint8_t uc_goalY, 			///< [in] ゴールY座�?
	enMAP_ACT_MODE	en_type		///< [in] 計算方法（ま�?未使用?�?
) {
	uint16_t		x, y, i;		// ループ変数
	uint16_t		uc_dase;		// 基準値
	uint16_t		uc_new;			// 新値
	uint16_t		uc_level;		// 等高�?
	uint8_t		uc_wallData;	// 壁情報

	stPOSITION		st_pos;

	en_type = en_type;		// コンパイルワーニング回避?��いずれ削除?�?

	queue_t queue;
	queue_t* pQueue = &queue;

	initQueue(pQueue);

	/* 等高線�?�ップを初期化す�? */
	for (i = 0; i < MAP_SMAP_MAX_VAL; i++) {
		us_cmap[i / MAP_Y_SIZE][i & (MAP_X_SIZE - 1)] = MAP_SMAP_MAX_VAL - 1;
	}

	/* ス�?�?プ�?�更新予�?のキュー */
//	std::queue<stPOSITION> q;
//	QueryPerformanceCounter(&start);

	/* 目標地点の等高線を0に設�? */
	setStep(uc_goalX, uc_goalY, 0);
	st_pos.x = uc_goalX;
	st_pos.y = uc_goalY;
	st_pos.step = 0;

	enqueue(pQueue,st_pos);

	/* 等高線�?�ップを作�?? */
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
	uint8_t uc_goalX, 			///< [in] ?��S?��[?��?��X?��?��?��W
	uint8_t uc_goalY, 			///< [in] ?��S?��[?��?��Y?��?��?��W
	enMAP_ACT_MODE	en_type		///< [in] ?��v?��Z?��?��?��@?��i?��܂�?��?��?��g?��p?��j
){
	uint16_t		x, y, i;		// ?��?��?��[?��v?��ϐ�
	uint16_t		uc_dase;		// ?�����?��l
	uint16_t		uc_new;			// ?��V?��l
	uint16_t		uc_level;		// ?��?��?��?��?��?��
	uint8_t		uc_wallData;	// ?��Ǐ�?��

	en_type = en_type;		// ?��R?��?��?��p?��C?��?��?��?��?��[?��j?��?��?��O?��?��?��?��i?��?��?��?��?��?������j

	/* ?��?��?��?��?��?��?��}?��b?��v?��?��?��?��?��?��?��?��?��?��?��?�� */
	for ( i = 0; i < MAP_SMAP_MAX_VAL; i++ ){
		us_cmap[ i / MAP_Y_SIZE][ i & (MAP_X_SIZE-1) ] = MAP_SMAP_MAX_VAL - 1;
	}
	/* ?��ڕW?��n?��_?��̓�?��?��?��?��?��?��0?��ɐݒ� */
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

	/* ?��?��?��?��?��?��?��}?��b?��v?��?��?��?�� */
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
					/* ?��T?��?��?��?��?��s */
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
					/* ?��ŒZ?��?��?��s */
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
	enMAP_SEARCH_TYPE	en_calcType,	///< [in] ?��v?��Z?��?��?��@
	enMAP_HEAD_DIR* 	p_head			///< [out] ?��i?��s?��?��?��?��?��i?��߂�l?��j
){
	uint8_t		uc_wall;				// ?��Ǐ�?��
	uint16_t		us_base;				// ?��?��?��?��?��?��?��D?��?��x?��?��?��?��l
	uint16_t		us_new;
	enMAP_HEAD_DIR	en_tmpHead;

	/* ?��?��?��?��?��v?��Z */
	// ?��?��?��?��?��?��MAP?��@
	if( CONTOUR_SYSTEM == en_calcType ){
		// ?��?��?��ӂ�4?��?��?��ň�ԖړI?��n?��ɋ߂�?��ړ�?��?��?��?��?��?��?��Z?��o?��?��?��?��B
		// ?��?��?��?��?��?��?��A?��ړ�?��ł�?��?��?��ԋ߂�?��?��Ԃ�?��?��?��?��?��?��?��?���??��ɂ́A?��?��?��̏�?��őI?��?��?��?��?��?��B
		// ?��@?��?��?��T?��?��?��?��?��,?��?��?��i ?��A?��?��?��T?��?��?��?��?��,?��?��?��?�� ?��B?��?��?��T?��?��?��?��?��,?��?��?��i ?��C?��?��?��T?��?��?��?��?��,?��?��?��?��
		uc_wall = g_sysMap[my][mx];
		us_base = MAP_SMAP_MAX_PRI_VAL;					// 16[?��?��?��]?��~16[?��?��?��]?��~4[?��?��?��?��]

		/* 4?��?��?��?��?��?��?��r */
		//	?��k?��?��?��?��?��̋�?��̊m?��F
		if ( ( uc_wall & 1 ) == 0 ){
			us_new = us_cmap[my+1][mx] * 4 + 4;
			if ( ( g_sysMap[my+1][mx] & 0xf0 ) != 0xf0 ) us_new = us_new - 2;
			if ( en_Head == NORTH ) us_new = us_new - 1;
			if ( us_new < us_base ){
				us_base = us_new;
				en_tmpHead = NORTH;
			}
		}
		//	?��?��?��?��?��?��?��̋�?��̊m?��F
		if ( ( uc_wall & 2 ) == 0 ){
			us_new = us_cmap[my][mx+1] * 4 + 4;
			if ( ( g_sysMap[my][mx+1] & 0xf0 ) != 0xf0 ) us_new = us_new - 2;
			if ( en_Head == EAST) us_new = us_new - 1;
			if ( us_new < us_base ){
				us_base = us_new;
				en_tmpHead = EAST;
			}
		}
		//	?��?��?��?��?��?��̋�?��̊m?��F
		if ( ( uc_wall & 4 ) == 0 ){
			us_new = us_cmap[my-1][mx] * 4 + 4;
			if ( ( g_sysMap[my-1][mx] & 0xf0 ) != 0xf0) us_new = us_new - 2;
			if ( en_Head == SOUTH ) us_new = us_new - 1;
			if ( us_new < us_base ){
				us_base = us_new;
				en_tmpHead = SOUTH;
			}
		}
		//	?��?��?��?��?��?��?��̋�?��̊m?��F
		if ( ( uc_wall & 8 ) == 0 ){
			us_new = us_cmap[my][mx-1] * 4 + 4;
			if ( ( g_sysMap[my][mx-1] & 0xf0 ) != 0xf0 ) us_new = us_new - 2;
			if ( en_Head == WEST ) us_new = us_new - 1;
			if ( us_new < us_base ){
				us_base = us_new;
				en_tmpHead = WEST;
			}
		}
		
		*p_head = (enMAP_HEAD_DIR)( (en_tmpHead - en_Head) & 3 );		// ?��ړ�?��?��?��?��
		
	}
	// ?��?��?��?��?��?��@?��w?��?��Ȃ�
	else{
		*p_head = (enMAP_HEAD_DIR)0;
	}

}

void MAP_refMousePos( 
	enMAP_HEAD_DIR 			en_head			///< [in] ?��i?��s?��?��?��?��
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
	enMAP_HEAD_DIR 	en_head,		///< [in] ?��?��?��ΐi?��s?��?��?��?��?��i?��}?��E?��X?��i?��s?��?��?��?��?��?��k?��Ƃ�?��Ă�?��?��j
	bool*			p_type			///< [in] FALSE: ?��P?��?��ԑO?��i?��?��ԁATURE:?��?��?��?��ԑO?��i?��?��?��
){
	*p_type = TRUE;
	f_MoveBackDist = 0;				// ?��ړ�?��?��?��?��?��?��?��?��?��Z?��l?��N?��?��?��A
	
	/* ?��?��?��?�� */
	switch( en_head ){

		/* ?��?��?��̂܂ܑO?��i */
		case NORTH:
			*p_type = FALSE;
			MOT_goBlock_Const( 1 );				// 1?��?��?��O?��i
			break;
		// ?��E?��ɐ�?��?���
		case EAST:
			MOT_goBlock_FinSpeed( 0.5, 0 );		// ?��?��?��?��?��O?��i
			MOT_turn(MOT_R90);									// ?��E90?��x?��?��?��?��
			break;
		// ?��?��?��ɐ�?��?���
		case WEST:
			MOT_goBlock_FinSpeed( 0.5, 0 );		// ?��?��?��?��?��O?��i
			MOT_turn(MOT_L90);									// ?��E90?��x?��?��?��?��
			break;
		// ?��?��?��]?��?��?��Ė߂�
		case SOUTH:
			MOT_goBlock_FinSpeed( 0.5, 0 );		// ?��?��?��?��?��O?��i
			MOT_turn(MOT_R180);									// ?��E180?��x?��?��?��?��
			
			/* ?��Ǔ�?��Ďp?��?��?��?��?��?��i?��?��?��ɕǂ�?��?��?��?��?��?��?��?��o?��b?��N?��{?��ړ�?��?��?��?��?��?��?��?��?��Z?��?��?��?��j */
			if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) )  ||		// ?��k?��?��?��?��?��?��?��Ă�?��Ėk?��ɕǂ�?��?��?��?��
				( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��?��Ă�?��ē�?��ɕǂ�?��?��?��?��
				( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��Ă�?��ē�ɕǂ�?��?��?��?��
				( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) ) 			// ?��?��?��?��?��?��?��?��?��Ă�?��Đ�?��ɕǂ�?��?��?��?��
			){
				MOT_goHitBackWall();					// ?��o?��b?��N?��?��?��?��
				f_MoveBackDist = MOVE_BACK_DIST;		// ?��o?��b?��N?��?��?��?��?��?��?��̈ړ�?��?��?��?��[?��?��?��]?��?��?��?��?��Z
			}
			break;
		default:
			break;
	}
	
	/* ?��O?��i?��?��?��Ƀp?��?��?��[?��?��?��?��?��[?��X?��@?��\?��?��?��?��?��?��?���??��?��W?��?��?��[?��?��?��?��?��Ȃ�?��?��΂Ȃ�Ȃ� */
/*	if( ( TRUE == DCMC_isPowerRelease() ) && ( en_head == NORTH ) ){
		
		MOT_goBack_Const( MOT_BACK_POLE );					// ?��P?��O?��̒�?��܂Ō�?��
		MAP_makeMapData();									// ?��ǃf?��[?��^?��?��?��?��?��?��H?��f?��[?��^?��?��?��?��			?��?�� ?��?��?��?��?��Ńf?��[?��^?��?��?��?��?��~?��X?��?��?��Ă�?��?��
		MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);			// ?��?��?��?��?��?��MAP?��@?��Ői?��s?��?��?��?��?��?��?��Z?��o			?��?�� ?��?��?��?��?��MAP?��?��?��?��
		MAP_moveNextBlock(en_head, p_type);					// ?��?��?��?��?��P?��x?��Ăяo?��?��?��i?��?��?��̋�?��ֈړ�?��j
	}
	else{*/
		/* ?��i?��s?��?��?��?��?��X?��V */
		en_Head = (enMAP_HEAD_DIR)( (en_Head + en_head) & (MAP_HEAD_DIR_MAX-1) );
//	}
}

void MAP_moveNextBlock_Sura( 
	enMAP_HEAD_DIR 	en_head,		///< [in] ?��?��?��ΐi?��s?��?��?��?��?��i?��}?��E?��X?��i?��s?��?��?��?��?��?��k?��Ƃ�?��Ă�?��?��j
	bool*			p_type,			///< [in] FALSE: ?��P?��?��ԑO?��i?��?��ԁATURE:?��?��?��?��ԑO?��i?��?��?��
	bool			bl_resume		///< [in] FALSE: ?��?��?��W?��?��?��[?��?��?��?��?��?��ł͂Ȃ�?��ATURE:?��?��?��W?��?��?��[?��?��?��?��?��?��
){
	*p_type = FALSE;
	f_MoveBackDist = 0;				// ?��ړ�?��?��?��?��?��?��?��?��?��Z?��l?��N?��?��?��A
	
	/* ?��?��?��?�� */
	switch( en_head ){

		// ?��?��?��̂܂ܑO?��i
		case NORTH:
			
			/* ?��?��?��W?��?��?��[?��?��?��?��?��?��ł͂Ȃ� */
			if( bl_resume == FALSE ){
		
				MOT_goBlock_Const( 1 );					// 1?��?��?��O?��i
//				uc_SlaCnt = 0;							// ?��X?��?��?��?��?��[?��?��?��?��?��Ă�?��Ȃ�
			}
			/* ?��?��?��W?��?��?��[?��?��?��?��?��?�� */
			else{
				MOT_goBlock_FinSpeed( 1.0f, SEARCH_SPEED );		// ?��?��?��?��?��O?��i(?��o?��b?��N?��̈ړ�?��ʂ�?��܂�)
//				uc_SlaCnt = 0;										// ?��X?��?��?��?��?��[?��?��?��?��?��Ă�?��Ȃ�
			}
			break;

		// ?��E?��ɃX?��?��?��?��?��[?��?��?��?��?��?��
		case EAST:
			if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?��k?��?��?��?��?��?��?��Ă�?��ē�?��ɕǂ�?��?��?��?��
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��?��Ă�?��ē�ɕǂ�?��?��?��?��
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��Ă�?��Đ�?��ɕǂ�?��?��?��?��
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) ) 			// ?��?��?��?��?��?��?��?��?��Ă�?��Ėk?��ɕǂ�?��?��?��?��
				){
				uc_dist_control = 0.020;
				}
			else{
				uc_dist_control = 0;
			}
			if(( uc_SlaCnt < SLA_count)
				||(near_wall != 1)) {
				MOT_goSla( MOT_R90S, PARAM_getSra( SLA_90 ) );	// ?��E?��X?��?��?��?��?��[?��?��
				uc_SlaCnt++;
			}
			else{
				/* ?��Ǔ�?��Ďp?��?��?��?��?��?��i?��?��?��ɕǂ�?��?��?��?��?��?��?��?��o?��b?��N?��{?��ړ�?��?��?��?��?��?��?��?��?��Z?��?��?��?��j */
				if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?��k?��?��?��?��?��?��?��Ă�?��Đ�?��ɕǂ�?��?��?��?��
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��?��Ă�?��Ėk?��ɕǂ�?��?��?��?��
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��Ă�?��ē�?��ɕǂ�?��?��?��?��
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) ) 			// ?��?��?��?��?��?��?��?��?��Ă�?��ē�ɕǂ�?��?��?��?��
				){
					MOT_goBlock_FinSpeed( 0.5, 0 );			// ?��?��?��?��?��O?��i
					MOT_turn(MOT_R90);						// ?��E90?��x?��?��?��?��
					uc_SlaCnt = 0;
					MOT_goHitBackWall();					// ?��o?��b?��N?��?��?��?��
					f_MoveBackDist = MOVE_BACK_DIST;		// ?��o?��b?��N?��?��?��?��?��?��?��̈ړ�?��?��?��?��[?��?��?��]?��?��?��?��?��Z
					*p_type = TRUE;							// ?��?��?��͔�?��?��ԁi?��{?��o?��b?��N?��j?��?��?��i?��߂�
					near_wall = 0;
				}
				else{
					MOT_goSla( MOT_R90S, PARAM_getSra( SLA_90 ) );	// ?��E?��X?��?��?��?��?��[?��?��
					uc_SlaCnt++;
				}
			}
			break;

		// ?��?��?��ɃX?��?��?��?��?��[?��?��?��?��?��?��
		case WEST:
			if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?��k?��?��?��?��?��?��?��Ă�?��Đ�?��ɕǂ�?��?��?��?��
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��?��Ă�?��Ėk?��ɕǂ�?��?��?��?��
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��Ă�?��ē�?��ɕǂ�?��?��?��?��
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) ) 			// ?��?��?��?��?��?��?��?��?��Ă�?��ē�ɕǂ�?��?��?��?��
				){
				uc_dist_control = 0.02;
				}
			else{
				uc_dist_control = 0;
			}
			if(( uc_SlaCnt < SLA_count)
				||(near_wall != 1) ){
				MOT_goSla( MOT_L90S, PARAM_getSra( SLA_90 ) );	// ?��?��?��X?��?��?��?��?��[?��?��
				uc_SlaCnt++;
			}
			else{
				/* ?��Ǔ�?��Ďp?��?��?��?��?��?��i?��?��?��ɕǂ�?��?��?��?��?��?��?��?��o?��b?��N?��{?��ړ�?��?��?��?��?��?��?��?��?��Z?��?��?��?��j */
				if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?��k?��?��?��?��?��?��?��Ă�?��ē�?��ɕǂ�?��?��?��?��
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��?��Ă�?��ē�ɕǂ�?��?��?��?��
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��Ă�?��Đ�?��ɕǂ�?��?��?��?��
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) ) 			// ?��?��?��?��?��?��?��?��?��Ă�?��Ėk?��ɕǂ�?��?��?��?��
				){
					MOT_goBlock_FinSpeed( 0.5, 0 );		// ?��?��?��?��?��O?��i
					MOT_turn(MOT_L90);					// ?��E90?��x?��?��?��?��
					uc_SlaCnt = 0;
					MOT_goHitBackWall();					// ?��o?��b?��N?��?��?��?��
					f_MoveBackDist = MOVE_BACK_DIST;		// ?��o?��b?��N?��?��?��?��?��?��?��̈ړ�?��?��?��?��[?��?��?��]?��?��?��?��?��Z
					*p_type = TRUE;							// ?��?��?��͔�?��?��ԁi?��{?��o?��b?��N?��j?��?��?��i?��߂�
					near_wall = 0;
				}
				else{
					MOT_goSla( MOT_L90S, PARAM_getSra( SLA_90 ) );	// ?��?��?��X?��?��?��?��?��[?��?��
					uc_SlaCnt++;
				}
			}
			break;

		// ?��?��?��]?��?��?��Ė߂�
		case SOUTH:
			MOT_goBlock_FinSpeed( 0.5, 0 );			// ?��?��?��?��?��O?��i
			MOT_turn(MOT_R180);									// ?��E180?��x?��?��?��?��
			uc_SlaCnt = 0;
			
			/* ?��Ǔ�?��Ďp?��?��?��?��?��?��i?��?��?��ɕǂ�?��?��?��?��?��?��?��?��o?��b?��N?��{?��ړ�?��?��?��?��?��?��?��?��?��Z?��?��?��?��j */
			if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) )  ||		// ?��k?��?��?��?��?��?��?��Ă�?��Ėk?��ɕǂ�?��?��?��?��
				( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��?��Ă�?��ē�?��ɕǂ�?��?��?��?��
				( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��Ă�?��ē�ɕǂ�?��?��?��?��
				( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) ) 			// ?��?��?��?��?��?��?��?��?��Ă�?��Đ�?��ɕǂ�?��?��?��?��
			){
				MOT_goHitBackWall();					// ?��o?��b?��N?��?��?��?��
				f_MoveBackDist = MOVE_BACK_DIST;		// ?��o?��b?��N?��?��?��?��?��?��?��̈ړ�?��?��?��?��[?��?��?��]?��?��?��?��?��Z
			}
			*p_type = TRUE;								// ?��?��?��͔�?��?��ԁ{?��o?��b?��N?��?��?��i?��߂�
			break;
			
		default:
			break;
	}
	
	/* ?��O?��i?��?��?��Ƀp?��?��?��[?��?��?��?��?��[?��X?��@?��\?��?��?��?��?��?��?���??��?��W?��?��?��[?��?��?��?��?��Ȃ�?��?��΂Ȃ�Ȃ� */
/*	if( ( TRUE == DCMC_isPowerRelease() ) && ( en_head == NORTH ) ){
		
		MOT_goBack_Const( MOT_BACK_POLE );					// ?��P?��O?��̒�?��܂Ō�?��
		MAP_makeMapData();									// ?��ǃf?��[?��^?��?��?��?��?��?��H?��f?��[?��^?��?��?��?��			?��?�� ?��?��?��?��?��Ńf?��[?��^?��?��?��?��?��~?��X?��?��?��Ă�?��?��
		MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);			// ?��?��?��?��?��?��MAP?��@?��Ői?��s?��?��?��?��?��?��?��Z?��o			?��?�� ?��?��?��?��?��MAP?��?��?��?��
		MAP_moveNextBlock_Sura(en_head, p_type, TRUE );		// ?��?��?��?��?��P?��x?��Ăяo?��?��?��i?��?��?��̋�?��ֈړ�?��j
	}
	else{*/
		/* ?��i?��s?��?��?��?��?��X?��V */
		en_Head = (enMAP_HEAD_DIR)( (en_Head + en_head) & (MAP_HEAD_DIR_MAX-1) );
//	}
}

void MAP_actGoal( void )
{	
	MOT_goBlock_FinSpeed( 0.5, 0 );			// ?��?��?��?��?��O?��i
	LL_mDelay(500);
	MOT_turn(MOT_R180);										// ?��E180?��x?��?��?��?��
	LL_mDelay(500);
	
//	MAP_SaveMapData();						// ?��?��?��H?��?��?��̃o?��b?��N?��A?��b?��v
	log_flag_off();
	MAP_actGoalLED();
	
	en_Head = (enMAP_HEAD_DIR)( (en_Head + 2) & (MAP_HEAD_DIR_MAX-1) );			//	?��i?��s?��?��?��?��?��X?��V

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
	uint16_t		x, y, i;		// ?��?��?��[?��v?��ϐ�
	uint16_t		uc_dase;		// ?�����?��l
	uint16_t		uc_new;			// ?��V?��l
	uint16_t		uc_level;		// ?��?��?��?��?��?��
	uint8_t		uc_wallData;	// ?��Ǐ�?��

	stPOSITION		st_pos;
	queue_t queue;
	queue_t* pQueue = &queue;

	initQueue(pQueue);

	/* ?��?��?��?��?��?��?��}?��b?��v?��?��?��?��?��?��?��?��?��?��?��?�� */
	for (i = 0; i < MAP_SMAP_MAX_VAL; i++) {
		us_cmap[i / MAP_Y_SIZE][i & (MAP_X_SIZE - 1)] = MAP_SMAP_MAX_VAL - 1;
	}
	/* ?��ڕW?��n?��_?��̓�?��?��?��?��?��?��0?��ɐݒ� */
//	us_cmap[0][0] = 0;
	setStep(0, 0, 0);
	st_pos.x = 0;
	st_pos.y = 0;
	st_pos.step = 0;

	enqueue(pQueue,st_pos);

	/* ?��?��?��?��?��?��?��}?��b?��v?��?��?��?�� */
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

	/* ?��?��?��?�� */
	switch (en_head) {

		/* ?��?��?��̂܂ܑO?��i */
	case NORTH:
//		*p_type = FALSE;
//		LED = LED6;
		if (MAP_KnownAcc() == FALSE) {					// ?��?��?��ɐi?��ދ�悪?��?��?��T?��?��?��̂Ƃ�
			if (st_known.bl_Known == TRUE){
				if (st_known.uc_StrCnt < 2) {
					MOT_goBlock_Const(1);					// 1?��?��?��̏ꍇ?��͓�?��?��?��̂܂�
				}
				else {
					MOT_setTrgtSpeed(MAP_KNOWN_ACC_SPEED);									// ?��?��?��m?��?��ԉ�?��?��?��?��?��?��Ƃ�?��̖ڕW?��?��?��x	
					MOT_goBlock_FinSpeed((float)(st_known.uc_StrCnt), SEARCH_SPEED);				// n?��?��?��O?��i
					MOT_setTrgtSpeed(SEARCH_SPEED);										// ?��ڕW?��?��?��x?��?��?��f?��t?��H?��?��?��g?��l?��ɖ߂�
				}
			}
			MOT_goBlock_Const(1);	////////////////////
			st_known.uc_StrCnt = 0;
			st_known.bl_Known = FALSE;

		}
		else {

			st_known.uc_StrCnt++;			// ?��ړ�?��?��?��̉�?��Z
			st_known.bl_Known = TRUE;
		}

		break;

		/* ?��E?��ɐ�?��?��� */
	case EAST:
//		LED = LED8;
		if (st_known.bl_Known == TRUE) {		// ?��?��?��?��?��?��?��?��?��?��?��?��
			if (st_known.uc_StrCnt < 2) {
				MOT_goBlock_Const(1);					// 1?��?��?��̏ꍇ?��͓�?��?��?��̂܂�
			}
			else {
//				LED = LED_ALL_ON;
				MOT_setTrgtSpeed(MAP_KNOWN_ACC_SPEED);									// ?��?��?��m?��?��ԉ�?��?��?��?��?��?��Ƃ�?��̖ڕW?��?��?��x	
				MOT_goBlock_FinSpeed((float)(st_known.uc_StrCnt), SEARCH_SPEED);				// n?��?��?��O?��i
				MOT_setTrgtSpeed(SEARCH_SPEED);										// ?��ڕW?��?��?��x?��?��?��f?��t?��H?��?��?��g?��l?��ɖ߂�
//				LED = LED_ALL_OFF;
			}
			st_known.uc_StrCnt = 0;		/////////////////////////////////////////
			st_known.bl_Known = FALSE;
		}
		if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?��k?��?��?��?��?��?��?��Ă�?��ē�?��ɕǂ�?��?��?��?��
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��?��Ă�?��ē�ɕǂ�?��?��?��?��
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��Ă�?��Đ�?��ɕǂ�?��?��?��?��
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) ) 			// ?��?��?��?��?��?��?��?��?��Ă�?��Ėk?��ɕǂ�?��?��?��?��
			){
			uc_dist_control = 0.02;
			}
		else{
			uc_dist_control = 0;
		}
		if( uc_SlaCnt < SLA_count ){
				MOT_goSla( MOT_R90S, PARAM_getSra( SLA_90 ) );	// ?��E?��X?��?��?��?��?��[?��?��
				uc_SlaCnt++;
			}
			else{
				f_MoveBackDist = 0;
				/* ?��Ǔ�?��Ďp?��?��?��?��?��?��i?��?��?��ɕǂ�?��?��?��?��?��?��?��?��o?��b?��N?��{?��ړ�?��?��?��?��?��?��?��?��?��Z?��?��?��?��j */
				if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?��k?��?��?��?��?��?��?��Ă�?��Đ�?��ɕǂ�?��?��?��?��
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��?��Ă�?��Ėk?��ɕǂ�?��?��?��?��
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��Ă�?��ē�?��ɕǂ�?��?��?��?��
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) ) 			// ?��?��?��?��?��?��?��?��?��Ă�?��ē�ɕǂ�?��?��?��?��
				){
					MOT_goBlock_FinSpeed( 0.5, 0 );			// ?��?��?��?��?��O?��i
					MOT_turn(MOT_R90);						// ?��E90?��x?��?��?��?��
					uc_SlaCnt = 0;
					MOT_goHitBackWall();					// ?��o?��b?��N?��?��?��?��
					f_MoveBackDist = MOVE_BACK_DIST;		// ?��o?��b?��N?��?��?��?��?��?��?��̈ړ�?��?��?��?��[?��?��?��]?��?��?��?��?��Z
					*p_type = TRUE;							// ?��?��?��͔�?��?��ԁi?��{?��o?��b?��N?��j?��?��?��i?��߂�
				}
				else{
					MOT_goSla( MOT_R90S, PARAM_getSra( SLA_90 ) );	// ?��E?��X?��?��?��?��?��[?��?��
					uc_SlaCnt++;
				}
			}
		break;

		/* ?��?��?��ɐ�?��?��� */
	case WEST:
		if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?��k?��?��?��?��?��?��?��Ă�?��Đ�?��ɕǂ�?��?��?��?��
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��?��Ă�?��Ėk?��ɕǂ�?��?��?��?��
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��Ă�?��ē�?��ɕǂ�?��?��?��?��
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) ) 			// ?��?��?��?��?��?��?��?��?��Ă�?��ē�ɕǂ�?��?��?��?��
			){
			uc_dist_control = 0.02;
			}
		else{
			uc_dist_control = 0;
		}
		if (st_known.bl_Known == TRUE) {		// ?��?��?��?��?��?��?��?��?��?��?��?��
			if (st_known.uc_StrCnt < 2) {
				MOT_goBlock_Const(1);					// 1?��?��?��̏ꍇ?��͓�?��?��?��̂܂�
			}
			else {
//				LED = LED_ALL_ON;
				MOT_setTrgtSpeed(MAP_KNOWN_ACC_SPEED);									// ?��?��?��m?��?��ԉ�?��?��?��?��?��?��Ƃ�?��̖ڕW?��?��?��x	
				MOT_goBlock_FinSpeed((float)(st_known.uc_StrCnt), SEARCH_SPEED);				// n?��?��?��O?��i
				MOT_setTrgtSpeed(SEARCH_SPEED);										// ?��ڕW?��?��?��x?��?��?��f?��t?��H?��?��?��g?��l?��ɖ߂�
//				LED = LED_ALL_OFF;
			}
			st_known.uc_StrCnt = 0;			//////////////////////////////////////
			st_known.bl_Known = FALSE;
		}

		if( uc_SlaCnt < SLA_count ){
				MOT_goSla( MOT_L90S, PARAM_getSra( SLA_90 ) );	// ?��?��?��X?��?��?��?��?��[?��?��
				uc_SlaCnt++;
			}
			else{
				f_MoveBackDist = 0;
				/* ?��Ǔ�?��Ďp?��?��?��?��?��?��i?��?��?��ɕǂ�?��?��?��?��?��?��?��?��o?��b?��N?��{?��ړ�?��?��?��?��?��?��?��?��?��Z?��?��?��?��j */
				if( ( ( en_Head == NORTH ) && ( ( g_sysMap[my][mx] & 0x02 ) != 0 ) )  ||		// ?��k?��?��?��?��?��?��?��Ă�?��ē�?��ɕǂ�?��?��?��?��
					( ( en_Head == EAST  ) && ( ( g_sysMap[my][mx] & 0x04 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��?��Ă�?��ē�ɕǂ�?��?��?��?��
					( ( en_Head == SOUTH ) && ( ( g_sysMap[my][mx] & 0x08 ) != 0 ) )  ||		// ?��?��?��?��?��?��?��?��Ă�?��Đ�?��ɕǂ�?��?��?��?��
					( ( en_Head == WEST  ) && ( ( g_sysMap[my][mx] & 0x01 ) != 0 ) ) 			// ?��?��?��?��?��?��?��?��?��Ă�?��Ėk?��ɕǂ�?��?��?��?��
				){
					MOT_goBlock_FinSpeed( 0.5, 0 );		// ?��?��?��?��?��O?��i
					MOT_turn(MOT_L90);					// ?��E90?��x?��?��?��?��
					uc_SlaCnt = 0;
					MOT_goHitBackWall();					// ?��o?��b?��N?��?��?��?��
					f_MoveBackDist = MOVE_BACK_DIST;		// ?��o?��b?��N?��?��?��?��?��?��?��̈ړ�?��?��?��?��[?��?��?��]?��?��?��?��?��Z
					*p_type = TRUE;							// ?��?��?��͔�?��?��ԁi?��{?��o?��b?��N?��j?��?��?��i?��߂�
				}
				else{
					MOT_goSla( MOT_L90S, PARAM_getSra( SLA_90 ) );	// ?��?��?��X?��?��?��?��?��[?��?��
					uc_SlaCnt++;
				}
			}
		break;

		/* ?��?��?��]?��?��?��Ė߂� */
	case SOUTH:
//		LED = LED_ALL_ON;
		MOT_goBlock_FinSpeed(0.5, 0);			// ?��?��?��?��?��O?��i
		MOT_turn(MOT_R180);									// ?��E180?��x?��?��?��?��
		uc_SlaCnt = 0;

		/* ?��Ǔ�?��Ďp?��?��?��?��?��?��i?��?��?��ɕǂ�?��?��?��?��?��?��?��?��o?��b?��N?��{?��ړ�?��?��?��?��?��?��?��?��?��Z?��?��?��?��j */
		if (((en_Head == NORTH) && ((g_sysMap[my][mx] & 0x01) != 0)) ||		// ?��k?��?��?��?��?��?��?��Ă�?��Ėk?��ɕǂ�?��?��?��?��
			((en_Head == EAST) && ((g_sysMap[my][mx] & 0x02) != 0)) ||		// ?��?��?��?��?��?��?��?��?��Ă�?��ē�?��ɕǂ�?��?��?��?��
			((en_Head == SOUTH) && ((g_sysMap[my][mx] & 0x04) != 0)) ||		// ?��?��?��?��?��?��?��?��Ă�?��ē�ɕǂ�?��?��?��?��
			((en_Head == WEST) && ((g_sysMap[my][mx] & 0x08) != 0)) 			// ?��?��?��?��?��?��?��?��?��Ă�?��Đ�?��ɕǂ�?��?��?��?��
			) {
			MOT_goHitBackWall();					// ?��o?��b?��N?��?��?��?��
			f_MoveBackDist = MOVE_BACK_DIST;	// ?��o?��b?��N?��?��?��?��?��?��?��̈ړ�?��?��?��?��[?��?��?��]?��?��?��?��?��Z
		}
		*p_type = TRUE;								// ?��?��?��͔�?��?��ԁ{?��o?��b?��N?��?��?��i?��߂�
		break;

	default:
		break;
	}

	/* ?��i?��s?��?��?��?��?��X?��V */
//	en_Head = (enMAP_HEAD_DIR)( (en_Head + en_head) & (MAP_HEAD_DIR_MAX-1) );
	en_Head = (enMAP_HEAD_DIR)(((uint8_t)en_Head + (uint8_t)en_head) & (MAP_HEAD_DIR_MAX - 1));
}

void MAP_searchGoal(
	uint8_t 			uc_trgX, 		///< [in] ?��ڕWx?��?��?��W
	uint8_t 			uc_trgY, 		///< [in] ?��ڕWy?��?��?��W 
	enMAP_ACT_MODE 	en_type, 		///< [in] ?��T?��?��?��?��?��@
	enSEARCH_MODE	en_search 		///< [in] ?��T?��?��?��?��?��@
){
	enMAP_HEAD_DIR	en_head = NORTH;
	bool		bl_type = TRUE;			// ?��?��?��݈ʒu?��AFALSE: ?��P?��?��ԑO?��i?��?��ԁATURE:?��?��?��?��ԑO?��i?��?��?��
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

//	SYS_setDisable( SYS_MODE );				// ?��?��?��[?��h?��ύX?��֎~

	MOT_setTrgtSpeed(SEARCH_SPEED);		// ?��ڕW?��?��?��x
	MOT_setNowSpeed( 0.0f );
	f_MoveBackDist = 0;
	uc_SlaCnt = 0;
	if(uc_trgX == GOAL_MAP_X && uc_trgY == GOAL_MAP_Y){
		f_MoveBackDist = MOVE_BACK_DIST;
	}
	
	log_flag_on();	//?��?��?��O?��֐�?��X?��^?��[?��g?��i?��?��?��?������j
	
	/* ?��?��?��H?��T?��?�� */
	while(1){
		MAP_refMousePos( en_Head );								// ?��?��?��W?��X?��V
//		MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?��?��?��?��?��?��?��}?��b?��v?��?��?��?��?��
		
		/* ?��?��?��M?��n?��?��?��?��T?��?�� */
		if( SEARCH_TURN == en_search ){
//			MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?��?��?��?��?��?��?��}?��b?��v?��?��?��?��?��
			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
			if( TRUE == bl_type ){
				MOT_goBlock_FinSpeed( 0.5 + f_MoveBackDist, SEARCH_SPEED );		// ?��?��?��?��?��O?��i(?��o?��b?��N?��̈ړ�?��ʂ�?��܂�)
			}
			MAP_makeMapData();												// ?��ǃf?��[?��^?��?��?��?��?��?��H?��f?��[?��^?��?��?��?��			?��?�� ?��?��?��?��?��Ńf?��[?��^?��?��?��?��?��~?��X?��?��?��Ă�?��?��
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);						// ?��?��?��?��?��?��MAP?��@?��Ői?��s?��?��?��?��?��?��?��Z?��o			?��?�� ?��?��?��?��?��MAP?��?��?��?��
			
			/* ?��?��?��̋�?��ֈړ� */
			if(( mx == uc_trgX ) && ( my == uc_trgY )){
				MAP_actGoal();										// ?��S?��[?��?��?��?��?��̓�?��?��
				break;
			}
			else{
				MAP_moveNextBlock(en_head, &bl_type);				// ?��?��?��̋�?��ֈړ�								?��?�� ?��?��?��?��?��ŉ�?��߂�??��?��?��?��[?��X?��`?��F?��b?��N?��{?��Ǎēx?��?��?��{?��?��?��?��?��?��?��{?��?��?��M?��n?��?��?��?���
			}
		}
		/* ?��X?��?��?��?��?��[?��?��?��T?��?�� */
		else if( SEARCH_SURA == en_search ){
//			MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?��?��?��?��?��?��?��}?��b?��v?��?��?��?��?��
			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
			if( TRUE == bl_type ){
				
				MOT_goBlock_FinSpeed( 0.5 + f_MoveBackDist, SEARCH_SPEED );		// ?��?��?��?��?��O?��i(?��o?��b?��N?��̈ړ�?��ʂ�?��܂�)
			}
			MAP_makeMapData();		// ?��ǃf?��[?��^?��?��?��?��?��?��H?��f?��[?��^?��?��?��?��
			
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);				// ?��?��?��?��?��?��MAP?��@?��Ői?��s?��?��?��?��?��?��?��Z?��o			?��?�� ?��?��?��?��?��MAP?��?��?��?��
			
			/* ?��?��?��̋�?��ֈړ� */
			if(( mx == uc_trgX ) && ( my == uc_trgY )){
				MAP_actGoal();										// ?��S?��[?��?��?��?��?��̓�?��?��
				break;
			}
			else{
				MAP_moveNextBlock_Sura(en_head, &bl_type, FALSE );	// ?��?��?��̋�?��ֈړ�						?��?�� ?��?��?��?��?��ŉ�?��߂�??��?��?��?��[?��X?��`?��F?��b?��N?��{?��Ǎēx?��?��?��{?��?��?��?��?��?��?��{?��?��?��M?��n?��?��?��?���
//				MAP_moveNextBlock_acc(en_head, &bl_type);
			}
		}
		/* ?��A?��ҒT?��?�� */
		else if (SEARCH_RETURN == en_search) {
			
			if( TRUE == bl_type ){
				
				MOT_goBlock_FinSpeed( 0.5 + f_MoveBackDist, SEARCH_SPEED );		// ?��?��?��?��?��O?��i(?��o?��b?��N?��̈ړ�?��ʂ�?��܂�)
			}
			MAP_makeMapData();		// ?��ǃf?��[?��^?��?��?��?��?��?��H?��f?��[?��^?��?��?��?��
						
			MAP_makeReturnContourMap(uc_staX,uc_staY);
			MAP_searchCmdList(uc_staX, uc_staY, en_Head, uc_goalX, uc_goalX, &en_endDir);
			uc_trgX = Return_X;
			uc_trgY = Return_Y;
//			MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?��?��?��?��?��?��?��}?��b?��v?��?��?��?��?��
			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);	
			/* ?��?��?��̋�?��ֈړ� */
//			if ((us_cmap[my][mx] == 0)||((g_sysMap[uc_trgY][uc_trgX]&0xf0) == 0xf0)) {
			if ((mx == 0)&&(my == 0)){
				MAP_actGoal();
				break;
			}
//			}
			else {
				MAP_moveNextBlock_Sura(en_head, &bl_type, FALSE);	// ?��?��?��̋�?��ֈړ�						?��?�� ?��?��?��?��?��ŉ�?��߂�??��?��?��?��[?��X?��`?��F?��b?��N?��{?��Ǎēx?��?��?��{?��?��?��?��?��?��?��{?��?��?��M?��n?��?��?��?���
//				MAP_moveNextBlock_acc(en_head, &bl_type);
			}
//			LED_count(uc_trgY);
		}

		
		/* ?��r?��?��?��Ő�?��?��s?��\?��ɂȂ�?��?�� */
		if( SYS_isOutOfCtrl() == TRUE ){
			CTRL_stop();
			DCM_brakeMot( DCM_R );		// ?��u?��?��?��[?��L
			DCM_brakeMot( DCM_L );		// ?��u?��?��?��[?��L
			
			/* ?��?��?��H?��֘A?��?��?��?��?��?��?��?�� */
			en_Head		= NORTH;
			mx			= 0;
			my			= 0;
			f_MoveBackDist = 0;
			
			// DCMC?��͉�?���??��?��W?��?��?��[?��?��?��Ŋ�?��ɃN?��?��?��A?��Ƌً}?��?��~?��?��?��s?��?��?��Ă�?��?��B
			break;
		}
	}
	search_flag = FALSE;
	LL_mDelay(1000);
//	SYS_setEnable( SYS_MODE );				// ?��?��?��[?��h?��ύX?��L?��?��
}

void MAP_searchGoalKnown(
	uint8_t 			uc_trgX, 		///< [in] ?��ڕWx?��?��?��W
	uint8_t 			uc_trgY, 		///< [in] ?��ڕWy?��?��?��W 
	enMAP_ACT_MODE 	en_type, 		///< [in] ?��T?��?��?��?��?��@
	enSEARCH_MODE	en_search 		///< [in] ?��T?��?��?��?��?��@
){
	enMAP_HEAD_DIR	en_head = NORTH;
	bool		bl_type = TRUE;			// ?��?��?��݈ʒu?��AFALSE: ?��P?��?��ԑO?��i?��?��ԁATURE:?��?��?��?��ԑO?��i?��?��?��
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

//	SYS_setDisable( SYS_MODE );				// ?��?��?��[?��h?��ύX?��֎~

	MOT_setTrgtSpeed(SEARCH_SPEED);		// ?��ڕW?��?��?��x
	MOT_setNowSpeed( 0.0f );
	f_MoveBackDist = 0;
	uc_SlaCnt = 0;
	if(uc_trgX == GOAL_MAP_X && uc_trgY == GOAL_MAP_Y){
		f_MoveBackDist = MOVE_BACK_DIST;
	}
	
	log_flag_on();	//?��?��?��O?��֐�?��X?��^?��[?��g?��i?��?��?��?������j
	
	/* ?��?��?��H?��T?��?�� */
	while(1){
		MAP_refMousePos( en_Head );								// ?��?��?��W?��X?��V
//		MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?��?��?��?��?��?��?��}?��b?��v?��?��?��?��?��
		
		/* ?��?��?��M?��n?��?��?��?��T?��?�� */
		if( SEARCH_TURN == en_search ){
//			MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?��?��?��?��?��?��?��}?��b?��v?��?��?��?��?��
			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
			if( TRUE == bl_type ){
				MOT_goBlock_FinSpeed( 0.5 + f_MoveBackDist, SEARCH_SPEED );		// ?��?��?��?��?��O?��i(?��o?��b?��N?��̈ړ�?��ʂ�?��܂�)
			}
			MAP_makeMapData();												// ?��ǃf?��[?��^?��?��?��?��?��?��H?��f?��[?��^?��?��?��?��			?��?�� ?��?��?��?��?��Ńf?��[?��^?��?��?��?��?��~?��X?��?��?��Ă�?��?��
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);						// ?��?��?��?��?��?��MAP?��@?��Ői?��s?��?��?��?��?��?��?��Z?��o			?��?�� ?��?��?��?��?��MAP?��?��?��?��
			
			/* ?��?��?��̋�?��ֈړ� */
			if(( mx == uc_trgX ) && ( my == uc_trgY )){
				MAP_actGoal();										// ?��S?��[?��?��?��?��?��̓�?��?��
				break;
			}
			else{
				MAP_moveNextBlock(en_head, &bl_type);				// ?��?��?��̋�?��ֈړ�								?��?�� ?��?��?��?��?��ŉ�?��߂�??��?��?��?��[?��X?��`?��F?��b?��N?��{?��Ǎēx?��?��?��{?��?��?��?��?��?��?��{?��?��?��M?��n?��?��?��?���
			}
		}
		/* ?��X?��?��?��?��?��[?��?��?��T?��?�� */
		else if( SEARCH_SURA == en_search ){
//			MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?��?��?��?��?��?��?��}?��b?��v?��?��?��?��?��
			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
			if( TRUE == bl_type ){
				
				MOT_goBlock_FinSpeed( 0.5 + f_MoveBackDist, SEARCH_SPEED );		// ?��?��?��?��?��O?��i(?��o?��b?��N?��̈ړ�?��ʂ�?��܂�)
			}
			if (st_known.bl_Known != TRUE) {
				MAP_makeMapData();		// ?��ǃf?��[?��^?��?��?��?��?��?��H?��f?��[?��^?��?��?��?��
			}
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);				// ?��?��?��?��?��?��MAP?��@?��Ői?��s?��?��?��?��?��?��?��Z?��o			?��?�� ?��?��?��?��?��MAP?��?��?��?��
			
			/* ?��?��?��̋�?��ֈړ� */
			if(( mx == uc_trgX ) && ( my == uc_trgY )){
				MAP_actGoal();										// ?��S?��[?��?��?��?��?��̓�?��?��
				break;
			}
			else{
//				MAP_moveNextBlock_Sura(en_head, &bl_type, FALSE );	// ?��?��?��̋�?��ֈړ�						?��?�� ?��?��?��?��?��ŉ�?��߂�??��?��?��?��[?��X?��`?��F?��b?��N?��{?��Ǎēx?��?��?��{?��?��?��?��?��?��?��{?��?��?��M?��n?��?��?��?���
				MAP_moveNextBlock_acc(en_head, &bl_type);
			}
		}
		/* ?��A?��ҒT?��?�� */
		else if (SEARCH_RETURN == en_search) {
			
			if( TRUE == bl_type ){
				
				MOT_goBlock_FinSpeed( 0.5 + f_MoveBackDist, SEARCH_SPEED );		// ?��?��?��?��?��O?��i(?��o?��b?��N?��̈ړ�?��ʂ�?��܂�)
			}
			if (st_known.bl_Known != TRUE) {
				MAP_makeMapData();		// ?��ǃf?��[?��^?��?��?��?��?��?��H?��f?��[?��^?��?��?��?��
			}			
			MAP_makeReturnContourMap(uc_staX,uc_staY);
			MAP_searchCmdList(uc_staX, uc_staY, en_Head, uc_goalX, uc_goalX, &en_endDir);
			uc_trgX = Return_X;
			uc_trgY = Return_Y;
//			MAP_makeContourMap( uc_trgX, uc_trgY, en_type );		// ?��?��?��?��?��?��?��}?��b?��v?��?��?��?��?��
			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);	

			/* ?��?��?��̋�?��ֈړ� */
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

		
		/* ?��r?��?��?��Ő�?��?��s?��\?��ɂȂ�?��?�� */
		if( SYS_isOutOfCtrl() == TRUE ){
			CTRL_stop();
			DCM_brakeMot( DCM_R );		// ?��u?��?��?��[?��L
			DCM_brakeMot( DCM_L );		// ?��u?��?��?��[?��L
			
			/* ?��?��?��H?��֘A?��?��?��?��?��?��?��?�� */
			en_Head		= NORTH;
			mx			= 0;
			my			= 0;
			f_MoveBackDist = 0;
			
			// DCMC?��͉�?���??��?��W?��?��?��[?��?��?��Ŋ�?��ɃN?��?��?��A?��Ƌً}?��?��~?��?��?��s?��?��?��Ă�?��?��B
			break;
		}
	}
	search_flag = FALSE;
	LL_mDelay(1000);
//	SYS_setEnable( SYS_MODE );				// ?��?��?��[?��h?��ύX?��L?��?��

}
