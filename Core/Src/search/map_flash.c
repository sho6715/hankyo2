/*
 * map_flash.c
 *
 *  Created on: 2021/02/06
 *      Author: shohe
 */


#include "search/map_flash.h"

void map_write(void)
{
	uint64_t *map_add;
	map_add = (uint64_t *)g_sysMap;
	
	//DataFlash?申C?申?申?申[?申X
//    FLASH_Erase(0x7F);  //reg127
	//?申}?申b?申v?申f?申[?申^?申?申DataFlash?申��鐃�?申?申?申?申?申?申
    FLASH_WriteData(0x7F,(uint32_t)sta_add_127, map_add, 32*32);
}

void map_copy(void)
{
    uint64_t *map_add;
    map_add = (uint64_t *)&g_sysMap;
    uint32_t read_address = sta_add_127;

    for (uint16_t i = 0;i<MAP_X_SIZE*MAP_Y_SIZE;i += 8){
	    FLASH_ReadData(read_address, map_add, 8);
	    read_address +=8;
	    map_add++;
    }
}

void map_erase(void)
{
	FLASH_Unlock();
	FLASH_Erase(0x7F);
	FLASH_Lock();
}



