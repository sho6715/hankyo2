/*
 * map_cmd.c
 *
 *  Created on: 2021/02/04
 *      Author: shohe
 */

#include "search/map_cmd.h"

#define LIST_NUM			( 4096 )				// ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?øΩ?øΩs?øΩÃ??øΩ?øΩX?øΩg?øΩ?øΩ

uint8_t		dcom[LIST_NUM];					// ?øΩ?øΩ?øΩn?øΩM?øΩ?øΩ?øΩ?øΩp
uint8_t		scom[LIST_NUM];					// ?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩp
uint8_t		tcom[LIST_NUM];					// ?øΩŒÇﬂëÔøΩ?øΩs?øΩp
uint16_t		us_totalCmd;					// ?øΩg?øΩ[?øΩ^?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?øΩ

float		f_PosX;							// X?øΩ?øΩ?øΩW
float		f_PosY;							// Y?øΩ?øΩ?øΩW
int16_t		s_PosDir;						// ?øΩi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩi[0]?øΩk [1]?øΩk?øΩ?øΩ [2]?øΩ?øΩ [3]?øΩ?ìå [4]?øΩ?øΩ [5]?øΩ?êº [6]?øΩ?øΩ [7]?øΩk?øΩ?øΩ ?øΩj

/* ?øΩ?øΩ?øΩs?øΩR?øΩ}?øΩ?øΩ?øΩh */
typedef enum{

	STOP = 0,	// ?øΩ?øΩ~?øΩR?øΩ}?øΩ?øΩ?øΩh
	
	/* ?øΩ?øΩ?øΩi */
	GO1,		// [  1]  1?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO2,		// [  2]  2?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO3,		// [  3]  3?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO4,		// [  4]  4?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO5,		// [  5]  5?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO6,		// [  6]  6?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO7,		// [  7]  7?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO8,		// [  8]  8?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO9,		// [  9]  9?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO10,		// [ 10] 10?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO11,		// [ 11] 11?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO12,		// [ 12] 12?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO13,		// [ 13] 13?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO14,		// [ 14] 14?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO15,		// [ 15] 15?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO16,		// [ 16] 16?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO17,		// [ 17] 17?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO18,		// [ 18] 18?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO19,		// [ 19] 19?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO20,		// [ 20] 20?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO21,		// [ 21] 21?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO22,		// [ 22] 22?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO23,		// [ 23] 23?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO24,		// [ 24] 24?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO25,		// [ 25] 25?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO26,		// [ 26] 26?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO27,		// [ 27] 27?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO28,		// [ 28] 28?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO29,		// [ 29] 29?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO30,		// [ 30] 30?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO31,		// [ 31] 31?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO32,		// [ 32] 32?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO33,		// [ 33] 33?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO34,		// [ 34] 34?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO35,		// [ 35] 35?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO36,		// [ 36] 36?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO37,		// [ 37] 37?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO38,		// [ 38] 38?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO39,		// [ 39] 39?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO40,		// [ 40] 40?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO41,		// [ 41] 41?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO42,		// [ 42] 42?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO43,		// [ 43] 43?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO44,		// [ 44] 44?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO45,		// [ 45] 45?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO46,		// [ 46] 46?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO47,		// [ 47] 47?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO48,		// [ 48] 48?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO49,		// [ 49] 49?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO50,		// [ 50] 50?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO51,		// [ 51] 51?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO52,		// [ 52] 52?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO53,		// [ 53] 53?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO54,		// [ 54] 54?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO55,		// [ 55] 55?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO56,		// [ 56] 56?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO57,		// [ 57] 57?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO58,		// [ 58] 58?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO59,		// [ 59] 59?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO60,		// [ 60] 60?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO61,		// [ 61] 61?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO62,		// [ 62] 62?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO63,		// [ 63] 63?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO64,		// [ 64] 64?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO65,		// [ 65] 65?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO66,		// [ 66] 66?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO67,		// [ 67] 67?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO68,		// [ 68] 68?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO69,		// [ 69] 69?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO70,		// [ 70] 70?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	GO71,		// [ 71] 71?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	
	/* ?øΩ?øΩ?øΩn?øΩM?øΩ?øΩ?øΩ?øΩ */
	R90,		// [ 72] ?øΩE90?øΩx?øΩE?øΩ?øΩ?øΩM?øΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh
	L90,		// [ 73] ?øΩ?øΩ90?øΩx?øΩ?øΩ?øΩ?øΩ?øΩM?øΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh
	R180,		// [ 74] ?øΩE180?øΩx?øΩ?øΩ?øΩM?øΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh
	L180,		// [ 75] ?øΩ?øΩ180?øΩx?øΩ?øΩ?øΩM?øΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh

	/* ?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ */
	R90S,		// [ 76] ?øΩE90?øΩx?øΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh
	L90S,		// [ 77] ?øΩ?øΩ90?øΩx?øΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh
	R90F,		// [ 78] ?øΩE?øΩ?øΩ?øΩ?øΩ90?øΩx?øΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh
	L90F,		// [ 79] ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ90?øΩx?øΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh
	R180F,		// [ 80] ?øΩE?øΩ?øΩ?øΩ?øΩ180?øΩx?øΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh
	L180F,		// [ 81] ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ180?øΩx?øΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh

	/* ?øΩŒÇÔøΩ */
	NGO1,		// [ 82] ?øΩŒÇÔøΩ 1?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO2,		// [ 83] ?øΩŒÇÔøΩ 2?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO3,		// [ 84] ?øΩŒÇÔøΩ 3?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO4,		// [ 85] ?øΩŒÇÔøΩ 4?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO5,		// [ 86] ?øΩŒÇÔøΩ 5?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO6,		// [ 87] ?øΩŒÇÔøΩ 6?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO7,		// [ 88] ?øΩŒÇÔøΩ 7?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO8,		// [ 89] ?øΩŒÇÔøΩ 8?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO9,		// [ 90] ?øΩŒÇÔøΩ 9?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO10,		// [ 91] ?øΩŒÇÔøΩ10?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO11,		// [ 92] ?øΩŒÇÔøΩ11?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO12,		// [ 93] ?øΩŒÇÔøΩ12?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO13,		// [ 94] ?øΩŒÇÔøΩ13?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO14,		// [ 95] ?øΩŒÇÔøΩ14?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO15,		// [ 96] ?øΩŒÇÔøΩ15?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO16,		// [ 97] ?øΩŒÇÔøΩ16?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO17,		// [ 98] ?øΩŒÇÔøΩ17?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO18,		// [ 99] ?øΩŒÇÔøΩ18?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO19,		// [100] ?øΩŒÇÔøΩ19?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO20,		// [101] ?øΩŒÇÔøΩ20?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO21,		// [102] ?øΩŒÇÔøΩ21?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO22,		// [103] ?øΩŒÇÔøΩ22?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO23,		// [104] ?øΩŒÇÔøΩ23?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO24,		// [105] ?øΩŒÇÔøΩ24?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO25,		// [106] ?øΩŒÇÔøΩ25?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO26,		// [107] ?øΩŒÇÔøΩ26?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO27,		// [108] ?øΩŒÇÔøΩ27?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO28,		// [109] ?øΩŒÇÔøΩ28?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO29,		// [110] ?øΩŒÇÔøΩ29?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO30,		// [111] ?øΩŒÇÔøΩ30?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO31,		// [112] ?øΩŒÇÔøΩ31?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO32,		// [113] ?øΩŒÇÔøΩ32?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO33,		// [114] ?øΩŒÇÔøΩ33?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO34,		// [115] ?øΩŒÇÔøΩ34?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO35,		// [116] ?øΩŒÇÔøΩ35?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO36,		// [117] ?øΩŒÇÔøΩ36?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO37,		// [118] ?øΩŒÇÔøΩ37?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO38,		// [119] ?øΩŒÇÔøΩ38?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO39,		// [120] ?øΩŒÇÔøΩ39?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO40,		// [121] ?øΩŒÇÔøΩ40?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO41,		// [122] ?øΩŒÇÔøΩ41?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO42,		// [123] ?øΩŒÇÔøΩ42?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO43,		// [124] ?øΩŒÇÔøΩ43?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO44,		// [125] ?øΩŒÇÔøΩ44?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO45,		// [126] ?øΩŒÇÔøΩ45?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO46,		// [127] ?øΩŒÇÔøΩ46?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO47,		// [128] ?øΩŒÇÔøΩ47?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO48,		// [129] ?øΩŒÇÔøΩ48?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO49,		// [130] ?øΩŒÇÔøΩ49?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO50,		// [131] ?øΩŒÇÔøΩ50?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO51,		// [132] ?øΩŒÇÔøΩ51?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO52,		// [133] ?øΩŒÇÔøΩ52?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO53,		// [134] ?øΩŒÇÔøΩ53?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO54,		// [135] ?øΩŒÇÔøΩ54?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO55,		// [136] ?øΩŒÇÔøΩ55?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO56,		// [137] ?øΩŒÇÔøΩ56?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO57,		// [138] ?øΩŒÇÔøΩ57?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO58,		// [139] ?øΩŒÇÔøΩ58?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO59,		// [140] ?øΩŒÇÔøΩ59?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO60,		// [141] ?øΩŒÇÔøΩ60?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO61,		// [142] ?øΩŒÇÔøΩ61?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO62,		// [143] ?øΩŒÇÔøΩ62?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO63,		// [144] ?øΩŒÇÔøΩ63?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO64,		// [145] ?øΩŒÇÔøΩ64?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO65,		// [146] ?øΩŒÇÔøΩ65?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO66,		// [147] ?øΩŒÇÔøΩ66?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO67,		// [148] ?øΩŒÇÔøΩ67?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO68,		// [149] ?øΩŒÇÔøΩ68?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO69,		// [150] ?øΩŒÇÔøΩ69?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO70,		// [151] ?øΩŒÇÔøΩ70?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh
	NGO71,		// [152] ?øΩŒÇÔøΩ71?øΩ?øΩ‘ëO?øΩi?øΩR?øΩ}?øΩ?øΩ?øΩh

	RS45N,		// [153] ?øΩ?øΩ?øΩi -> ?øΩE45   -> ?øΩŒÇÔøΩ
	LS45N,		// [154] ?øΩ?øΩ?øΩi -> ?øΩ?øΩ45   -> ?øΩŒÇÔøΩ
	RS135N,		// [155] ?øΩ?øΩ?øΩi -> ?øΩE135  -> ?øΩŒÇÔøΩ
	LS135N,		// [156] ?øΩ?øΩ?øΩi -> ?øΩ?øΩ135  -> ?øΩŒÇÔøΩ
	RN45S,		// [157] ?øΩŒÇÔøΩ -> ?øΩE45   -> ?øΩ?øΩ?øΩi
	LN45S,		// [158] ?øΩŒÇÔøΩ -> ?øΩ?øΩ45   -> ?øΩ?øΩ?øΩi
	RN135S,		// [159] ?øΩŒÇÔøΩ -> ?øΩE135  -> ?øΩ?øΩ?øΩi
	LN135S,		// [160] ?øΩŒÇÔøΩ -> ?øΩ?øΩ135  -> ?øΩ?øΩ?øΩi
	RN90N,		// [161] ?øΩŒÇÔøΩ -> ?øΩE90   -> ?øΩŒÇÔøΩ
	LN90N,		// [162] ?øΩŒÇÔøΩ -> ?øΩ?øΩ90   -> ?øΩŒÇÔøΩ

	CEND = 250,	// [250] ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩI?øΩ?øΩ

	MAP_CMD_MAX,
}enMAP_CMD;

typedef struct{
	enMAP_CMD	en_cmd;			// ?øΩR?øΩ}?øΩ?øΩ?øΩh
	float		f_x0_x1;		// [0]/[1]?øΩ?øΩX?øΩ?øΩ?øΩW?øΩ?øΩ?øΩZ?øΩl
	float		f_y0_y1;		// [0]/[1]?øΩ?øΩy?øΩ?øΩ?øΩW?øΩ?øΩ?øΩZ?øΩl
	float		f_x2_x3;		// [2]/[3]?øΩ?øΩX?øΩ?øΩ?øΩW?øΩ?øΩ?øΩZ?øΩl
	float		f_y2_y3;		// [2]/[3]?øΩ?øΩy?øΩ?øΩ?øΩW?øΩ?øΩ?øΩZ?øΩl
	float		f_x4_x5;		// [4]/[5]?øΩ?øΩX?øΩ?øΩ?øΩW?øΩ?øΩ?øΩZ?øΩl
	float		f_y4_y5;		// [4]/[5]?øΩ?øΩy?øΩ?øΩ?øΩW?øΩ?øΩ?øΩZ?øΩl
	float		f_x6_x7;		// [6]/[7]?øΩ?øΩX?øΩ?øΩ?øΩW?øΩ?øΩ?øΩZ?øΩl
	float		f_y6_y7;		// [6]/[7]?øΩ?øΩy?øΩ?øΩ?øΩW?øΩ?øΩ?øΩZ?øΩl
	int16_t		s_dir;			// ?øΩi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩi[0]?øΩk [1]?øΩk?øΩ?øΩ [2]?øΩ?øΩ [3]?øΩ?ìå [4]?øΩ?øΩ [5]?øΩ?êº [6]?øΩ?øΩ [7]?øΩk?øΩ?øΩ ?øΩj
}stMAP_SIM;

/* ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ…âÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩW?øΩX?øΩV?øΩf?øΩ[?øΩ^ */
const stMAP_SIM st_PosData[] = {
	
	//	?øΩR?øΩ}?øΩ?øΩ?øΩh	[0]/[1]?øΩ?øΩX	[0]/[1]?øΩ?øΩY	[2]/[3]?øΩ?øΩX	[2]/[3]?øΩ?øΩY	[4]/[5]?øΩ?øΩX	[4]/[5]?øΩ?øΩY	[6]/[7]?øΩ?øΩX	[6]/[7]?øΩ?øΩY	?øΩ?øΩ?øΩ?øΩ
	{ R90,			0.5,		0.5,		0.5,		-0.5,		-0.5,		-0.5,		-0.5,		0.5,		+2 },		// [0]
	{ L90,			-0.5,		0.5,		0.5,		0.5,		0.5,		-0.5,		-0.5,		-0.5,		-2 },		// [1]
	{ R90S,			0.5,		0.5,		0.5,		-0.5,		-0.5,		-0.5,		-0.5,		0.5,		+2 },		// [2]
	{ L90S,			-0.5,		0.5,		0.5,		0.5,		0.5,		-0.5,		-0.5,		-0.5,		-2 },		// [3]
	{ RS45N,		0.25,		0.75,		0.75,		-0.25,		-0.25,		-0.75,		-0.75,		0.25,		+1 },		// [4]
	{ LS45N,		-0.25,		0.75,		0.75,		0.25,		0.25,		-0.75,		-0.75,		-0.25,		-1 },		// [5]
	{ RS135N,		0.75,		0.25,		0.25,		-0.75,		-0.75,		-0.25,		-0.25,		0.75,		+3 },		// [6]
	{ LS135N,		-0.75,		0.25,		0.25,		0.75,		0.75,		-0.25,		-0.25,		-0.75,		-3 },		// [7]
	{ RN45S,		0.75,		0.25,		0.25,		-0.75,		-0.75,		-0.25,		-0.25,		0.75,		+1 },		// [8]
	{ LN45S,		0.25,		0.75,		0.75,		-0.25,		-0.25,		-0.75,		-0.75,		0.25,		-1 },		// [9]
	{ RN135S,		0.75,		-0.25,		-0.25,		-0.75,		-0.75,		0.25,		0.25,		0.75,		+3 },		// [10]
	{ LN135S,		-0.25,		0.75,		0.75,		0.25,		0.25,		-0.75,		-0.75,		-0.25,		-3 },		// [11]
	{ RN90N,		0.5,		0,			0,			-0.5,		-0.5,		0,			0,			0.5,		+2 },		// [12]
	{ LN90N,		0,			0.5,		0.5,		0,			0,			-0.5,		-0.5,		0,			-2 },		// [13]
	{ GO1,			0,			0.5,		0.5,		0,			0,			-0.5,		-0.5,		0,			0  },		// [14]
	{ NGO1,			0.5,		0.5,		0.5,		-0.5,		-0.5,		-0.5,		-0.5,		0.5,		0  },		// [15]
	{ MAP_CMD_MAX,	0,			0,			0,			0,			0,			0,			0,			0,			0  },
};

float f_LogPosX[30];
float f_LogPosY[30];
uint16_t us_LogIndex = 0;
uint16_t us_LogWallCut[30];
uint16_t us_LogIndexWallCut = 0;

void MAP_refPos( uint8_t uc_cmd )
{
	uint8_t uc_index = 0;			// ?øΩe?øΩ[?øΩu?øΩ?øΩ?øΩÃÉC?øΩ?øΩ?øΩf?øΩb?øΩN?øΩX?øΩ‘çÔøΩ
	
	/* ------------------------------------------ */
	/*  ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?øΩ?øΩ?øΩe?øΩ[?øΩu?øΩ?øΩ?øΩÃÉC?øΩ?øΩ?øΩf?øΩb?øΩN?øΩX?øΩ?øΩ?øΩÊìæ  */
	/* ------------------------------------------ */
	/* ?øΩ?øΩ?øΩi */
	if( ( uc_cmd <=  GO71 ) && ( uc_cmd >=  GO1) ){
		
		uc_index = 14;		// st_PosData?øΩe?øΩ[?øΩu?øΩ?øΩ?øΩÃíÔøΩ?øΩi?øΩÃÉC?øΩ?øΩ?øΩf?øΩb?øΩN?øΩX?øΩ‘çÔøΩ
	}
	/* ?øΩŒÇﬂíÔøΩ?øΩi */
	else if( ( uc_cmd <=  NGO71 ) && ( uc_cmd >=  NGO1) ){
		
		uc_index = 15;		// st_PosData?øΩe?øΩ[?øΩu?øΩ?øΩ?øΩÃéŒÇﬂíÔøΩ?øΩi?øΩÃÉC?øΩ?øΩ?øΩf?øΩb?øΩN?øΩX?øΩ‘çÔøΩ
	}
	/* ?øΩ?øΩ?øΩÃëÔøΩ?øΩÃÉR?øΩ}?øΩ?øΩ?øΩh */
	else{
		while(1){
			
			if( st_PosData[uc_index].en_cmd == uc_cmd )      break;			// ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?øΩ?øΩ?øΩ
			if( st_PosData[uc_index].en_cmd == MAP_CMD_MAX ) return;		// ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
			uc_index++;
		}
	}
	
	/* ?øΩ íu?øΩX?øΩV */
	switch( s_PosDir ){
		
		/* [0]?øΩk [1]?øΩk?øΩ?øΩ */
		case 0:
		case 1:
		
			/* ?øΩ?øΩ?øΩi */
			if( uc_index == 14 ){
				
				f_PosX += st_PosData[uc_index].f_x0_x1 * uc_cmd;
				f_PosY += st_PosData[uc_index].f_y0_y1 * uc_cmd;
			}
			/* ?øΩŒÇﬂíÔøΩ?øΩi */
			else if( uc_index == 15 ){
				
				f_PosX += st_PosData[uc_index].f_x0_x1 * ( uc_cmd - 81 );
				f_PosY += st_PosData[uc_index].f_y0_y1 * ( uc_cmd - 81 );
			}
			/* ?øΩ?øΩ?øΩÃëÔøΩ?øΩÃÉJ?øΩ[?øΩu */
			else{
				f_PosX += st_PosData[uc_index].f_x0_x1;
				f_PosY += st_PosData[uc_index].f_y0_y1;
			}
			break;
		
		/* [2]?øΩ?øΩ [3]?øΩ?ìå */
		case 2:
		case 3:

			/* ?øΩ?øΩ?øΩi */
			if( uc_index == 14 ){
				
				f_PosX += st_PosData[uc_index].f_x2_x3 * uc_cmd;
				f_PosY += st_PosData[uc_index].f_y2_y3 * uc_cmd;
			}
			/* ?øΩŒÇﬂíÔøΩ?øΩi */
			else if( uc_index == 15 ){
				
				f_PosX += st_PosData[uc_index].f_x2_x3 * ( uc_cmd - 81 );
				f_PosY += st_PosData[uc_index].f_y2_y3 * ( uc_cmd - 81 );
			}
			/* ?øΩ?øΩ?øΩÃëÔøΩ?øΩÃÉJ?øΩ[?øΩu */
			else{
				f_PosX += st_PosData[uc_index].f_x2_x3;
				f_PosY += st_PosData[uc_index].f_y2_y3;
			}
			break;

		/* [4]?øΩ?øΩ [5]?øΩ?êº */
		case 4:
		case 5:

			/* ?øΩ?øΩ?øΩi */
			if( uc_index == 14 ){
				
				f_PosX += st_PosData[uc_index].f_x4_x5 * uc_cmd;
				f_PosY += st_PosData[uc_index].f_y4_y5 * uc_cmd;
			}
			/* ?øΩŒÇﬂíÔøΩ?øΩi */
			else if( uc_index == 15 ){
				
				f_PosX += st_PosData[uc_index].f_x4_x5 * ( uc_cmd - 81 );
				f_PosY += st_PosData[uc_index].f_y4_y5 * ( uc_cmd - 81 );
			}
			/* ?øΩ?øΩ?øΩÃëÔøΩ?øΩÃÉJ?øΩ[?øΩu */
			else{
				f_PosX += st_PosData[uc_index].f_x4_x5;
				f_PosY += st_PosData[uc_index].f_y4_y5;
			}
			break;

		/* [6]?øΩ?øΩ [7]?øΩk?øΩ?øΩ */
		case 6:
		case 7:

			/* ?øΩ?øΩ?øΩi */
			if( uc_index == 14 ){
				
				f_PosX += st_PosData[uc_index].f_x6_x7 * uc_cmd;
				f_PosY += st_PosData[uc_index].f_y6_y7 * uc_cmd;
			}
			/* ?øΩŒÇﬂíÔøΩ?øΩi */
			else if( uc_index == 15 ){
				
				f_PosX += st_PosData[uc_index].f_x6_x7 * ( uc_cmd - 81 );
				f_PosY += st_PosData[uc_index].f_y6_y7 * ( uc_cmd - 81 );
			}
			/* ?øΩ?øΩ?øΩÃëÔøΩ?øΩÃÉJ?øΩ[?øΩu */
			else{
				f_PosX += st_PosData[uc_index].f_x6_x7;
				f_PosY += st_PosData[uc_index].f_y6_y7;
			}
			break;
	}
	
	/* ?øΩi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩX?øΩV */
	s_PosDir += st_PosData[uc_index].s_dir;
	if( s_PosDir < 0 ) s_PosDir += 8;				// [0]?øΩ`[7]?øΩ…ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
	else if( s_PosDir > 7 ) s_PosDir -= 8;
	
	f_LogPosX[us_LogIndex] = f_PosX;
	f_LogPosY[us_LogIndex] = f_PosY;
	
	us_LogIndex++;
	us_LogIndex %= 30;
}

bool MAP_setWallCut( uint8_t uc_cmd )
{
	uint8_t uc_val = 0;			// 1?øΩ?øΩ?øΩO?øΩÃÉR?øΩ[?øΩi?øΩ[?øΩ?øΩ?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©?øΩi0?øΩ»äO?øΩ»ÇÔøΩ«ÇÔøΩ?øΩ?øΩj
	uint8_t uc_valPrev = 0;		// 2?øΩ?øΩ?øΩO?øΩÃÉR?øΩ[?øΩi?øΩ[?øΩ?øΩ?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©?øΩi0?øΩ»äO?øΩ»ÇÔøΩ«ÇÔøΩ?øΩ?øΩj
	bool bl_wallCut = FALSE;
	
	/* ?øΩ íu?øΩX?øΩV */
	switch( uc_cmd ){
		
		case R90S:
		case RS135N:
			
			/* 1?øΩ?øΩ?øΩO?øΩÃÉR?øΩ[?øΩi?øΩ[?øΩ?øΩ?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©?øΩi0?øΩ»äO?øΩ»ÇÁÇ†?øΩ?øΩj */
			// s_PosDir?øΩF?øΩi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩi[0]?øΩk [1]?øΩk?øΩ?øΩ [2]?øΩ?øΩ [3]?øΩ?ìå [4]?øΩ?øΩ [5]?øΩ?êº [6]?øΩ?øΩ [7]?øΩk?øΩ?øΩ ?øΩj
			switch( s_PosDir ){
				
				/* ?øΩ?øΩ?øΩ˚´Ä?øΩ≈êÔøΩ?øΩ?∑ÇÔøΩÃÇ≈ÅA?øΩ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩ?øΩ?øΩ«ÇÃóL?øΩ?øΩ?øΩ?≤Ç◊ÇÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩW?øΩ∆Ç»ÇÔøΩi?øΩ?øΩ?øΩ”ÅFg_sysMap?øΩ?øΩ2?øΩ?øΩ?øΩ?øΩ?øΩz?øΩ?øΩ≈ÇÔøΩ?øΩj */
				case 0: 
					if( 0 < f_PosY-0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY-0.5)][(uint8_t)(f_PosX)] & 0x02;		// ?øΩk?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩÃÇ≈ìÔøΩ?øΩ?øΩ?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©
					if( 0 < f_PosY-1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY-1.5)][(uint8_t)(f_PosX)] & 0x02;		// ?øΩk?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩÃÇ≈ìÔøΩ?øΩ?øΩ?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©
					break;	
				case 2: 
					if( 0 < f_PosX-0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX-0.5)] & 0x04;		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩÃÇ≈ìÏë§?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©
					if( 0 < f_PosX-1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX-1.5)] & 0x04;		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩÃÇ≈ìÏë§?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©
					break;
				case 4: 
					if( MAP_Y_SIZE_REAL > f_PosY+0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY+0.5)][(uint8_t)(f_PosX)] & 0x08;		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩÃÇ≈êÔøΩ?øΩ?øΩ?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©
					if( MAP_Y_SIZE_REAL > f_PosY+1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY+1.5)][(uint8_t)(f_PosX)] & 0x08;		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩÃÇ≈êÔøΩ?øΩ?øΩ?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©
					break;
				case 6:
					if( MAP_X_SIZE_REAL > f_PosX+0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX+0.5)] & 0x01;		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩÃÇ≈ñk?øΩ?øΩ?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©
					if( MAP_X_SIZE_REAL > f_PosX+1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX+1.5)] & 0x01;		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩÃÇ≈ñk?øΩ?øΩ?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©
					break;
			}
			/* ?øΩ«ÇÔøΩ?øΩ?øΩ?øΩÈÇΩ?øΩﬂï«êÿÇÔøΩ‚ê≥?øΩ?øΩ?øΩs?øΩ?øΩ */
			if( ( uc_val != 0 ) || ( ( uc_val != 0 ) && ( uc_valPrev != 0 ) ) ){
				
				MOT_setWallEdgeType( MOT_WALL_EDGE_RIGHT );		// ?øΩ«êÿÇÔøΩ‚ê≥?øΩ?øΩ?øΩ?øΩ?øΩ{?øΩ?øΩ?øΩ?øΩ
				bl_wallCut = TRUE;
			}
			break;
			
		case L90S:
		case LS135N:
			/* 1?øΩ?øΩ?øΩO?øΩÃÉR?øΩ[?øΩi?øΩ[?øΩ?øΩ?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©?øΩi0?øΩ»äO?øΩ»ÇÁÇ†?øΩ?øΩj */
			// s_PosDir?øΩF?øΩi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩi[0]?øΩk [1]?øΩk?øΩ?øΩ [2]?øΩ?øΩ [3]?øΩ?ìå [4]?øΩ?øΩ [5]?øΩ?êº [6]?øΩ?øΩ [7]?øΩk?øΩ?øΩ ?øΩj
			switch( s_PosDir ){
				
				/* ?øΩ?øΩ?øΩ˚´Ä?øΩ≈êÔøΩ?øΩ?∑ÇÔøΩÃÇ≈ÅA?øΩ?øΩ?øΩ?øΩ?øΩ?øΩO?øΩ?øΩ?øΩ«ÇÃóL?øΩ?øΩ?øΩ?≤Ç◊ÇÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩW?øΩ∆Ç»ÇÔøΩi?øΩ?øΩ?øΩ”ÅFg_sysMap?øΩ?øΩ2?øΩ?øΩ?øΩ?øΩ?øΩz?øΩ?øΩ≈ÇÔøΩ?øΩj */
				case 0: 
					if( 0 < f_PosY-0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY-0.5)][(uint8_t)(f_PosX)] & 0x08;			// ?øΩk?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩÃÇ≈êÔøΩ?øΩ?øΩ?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©
					if( 0 < f_PosY-1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY-1.5)][(uint8_t)(f_PosX)] & 0x08;			// ?øΩk?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩÃÇ≈êÔøΩ?øΩ?øΩ?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©
					break;
				case 2: 
					if( 0 < f_PosX-0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX-0.5)] & 0x01;			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩÃÇ≈ñk?øΩ?øΩ?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©
					if( 0 < f_PosX-1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX-1.5)] & 0x01;			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩÃÇ≈ñk?øΩ?øΩ?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©
					break;
				case 4: 
					if( MAP_Y_SIZE_REAL > f_PosY+0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY+0.5)][(uint8_t)(f_PosX)] & 0x02;			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩÃÇ≈ìÔøΩ?øΩ?øΩ?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©
					if( MAP_Y_SIZE_REAL > f_PosY+1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY+1.5)][(uint8_t)(f_PosX)] & 0x02;			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩÃÇ≈ìÔøΩ?øΩ?øΩ?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©
					break;
				case 6: 
					if( MAP_X_SIZE_REAL > f_PosX+0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX+0.5)] & 0x04;			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩÃÇ≈ìÏë§?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©
					if( MAP_X_SIZE_REAL > f_PosX+1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX+1.5)] & 0x04;			// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩÃÇ≈ìÏë§?øΩÃï«ÇÔøΩ?øΩ?øΩ?øΩÈÇ©
					break;
			}
			/* ?øΩ«ÇÔøΩ?øΩ?øΩ?øΩÈÇΩ?øΩﬂï«êÿÇÔøΩ‚ê≥?øΩ?øΩ?øΩs?øΩ?øΩ */
			if( ( uc_val != 0 ) || ( ( uc_val != 0 ) && ( uc_valPrev != 0 ) ) ){
				
				MOT_setWallEdgeType( MOT_WALL_EDGE_LEFT );		// ?øΩ«êÿÇÔøΩ‚ê≥?øΩ?øΩ?øΩ?øΩ?øΩ{?øΩ?øΩ?øΩ?øΩ
				bl_wallCut = TRUE;
			}
			break;
			
		default:
			break;
	}
	
	return bl_wallCut;
}

void MAP_setCmdPos( uint8_t uc_x, uint8_t uc_y, enMAP_HEAD_DIR en_dir )
{
	f_PosX   = (float)uc_x;
	f_PosX   = (float)uc_y;
	s_PosDir = (int16_t)(en_dir * 2);	// ?øΩi?øΩs?øΩ?øΩ?øΩ?øΩ?øΩi[0]?øΩk [1]?øΩk?øΩ?øΩ [2]?øΩ?øΩ [3]?øΩ?ìå [4]?øΩ?øΩ [5]?øΩ?êº [6]?øΩ?øΩ [7]?øΩk?øΩ?øΩ ?øΩj?øΩA2?øΩ{?øΩ?øΩ?øΩ?øΩ∆íÔøΩ?øΩx?øΩl?øΩ?øΩ?øΩ?øΩ?øΩv?øΩ?øΩ?øΩ?øΩ
}

void MAP_showCmdLog( void )
{
	uint16_t i=0;
	
	/* ?øΩ?øΩ?øΩM?øΩn?øΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh */
	while(1){
		
		printf("dcom[%4d] = %02d  \n\r",i,dcom[i]);
		if( dcom[i] == CEND ) break;
		i++;
	}
	i=0;
	
	/* ?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh */
	while(1){
		
		printf("scom[%4d] = %02d  \n\r",i,scom[i]);
		if( scom[i] == CEND ) break;
		i++;
	}
	i=0;

	/* ?øΩŒÇﬂëÔøΩ?øΩs?øΩR?øΩ}?øΩ?øΩ?øΩh */
	while(1){
		
		printf("tcom[%4d] = %02d  \n\r",i,tcom[i]);
		if( tcom[i] == CEND ) break;
		i++;
	}
}

void MAP_makeCmdList( 
	uint8_t uc_staX,					///< [in] ?øΩJ?øΩnX?øΩ?øΩ?øΩW
	uint8_t uc_staY,					///< [in] ?øΩJ?øΩnY?øΩ?øΩ?øΩW
	enMAP_HEAD_DIR en_staDir,		///< [in] ?øΩJ?øΩn?øΩ?øΩ?øΩÃïÔøΩ?øΩ?øΩ
	uint8_t uc_endX,					///< [in] ?øΩI?øΩ?øΩX?øΩ?øΩ?øΩW
	uint8_t uc_endY,					///< [in] ?øΩI?øΩ?øΩY?øΩ?øΩ?øΩW
	enMAP_HEAD_DIR* en_endDir		///< [out] ?øΩI?øΩ?øΩ?øΩ?øΩ?øΩÃïÔøΩ?øΩ?øΩ
){
	uint8_t			uc_goStep;									// ?øΩO?øΩi?øΩÃÉX?øΩe?øΩb?øΩv?øΩ?øΩ
	uint16_t			us_high;									// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩÃçÔøΩ?øΩ?øΩ
	uint16_t			us_pt;										// ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ|?øΩC?øΩ?øΩ?øΩ^
	enMAP_HEAD_DIR	en_nowDir;									// ?øΩ?øΩ?øΩ›É}?øΩE?øΩX?øΩÃåÔøΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩ?øΩŒïÔøΩ?øΩ?øΩ
	enMAP_HEAD_DIR	en_tempDir;									// ?øΩ?øΩ?øΩŒïÔøΩ?øΩ?øΩ
//	uint16_t			i;											// roop
	
	/* ?øΩO?øΩi?øΩX?øΩe?øΩb?øΩv?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ */
	uc_goStep = 0;
	us_pt = 0;

	/* ?øΩ?øΩ?øΩH?øΩ?øΩ?©ÇÔøΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?ê¨ */
	while(1){	
		us_high = us_cmap[uc_staY][uc_staX]-1;
		if (en_staDir == NORTH){
			if     (((g_sysMap[uc_staY][uc_staX] & 0x11)==0x10)&&(us_cmap[uc_staY+1][uc_staX]==us_high)) en_nowDir=NORTH;
			else if(((g_sysMap[uc_staY][uc_staX] & 0x22)==0x20)&&(us_cmap[uc_staY][uc_staX+1]==us_high)) en_nowDir=EAST;
			else if(((g_sysMap[uc_staY][uc_staX] & 0x88)==0x80)&&(us_cmap[uc_staY][uc_staX-1]==us_high)) en_nowDir=WEST;
			else if(((g_sysMap[uc_staY][uc_staX] & 0x44)==0x40)&&(us_cmap[uc_staY-1][uc_staX]==us_high)) en_nowDir=SOUTH;
			else   while(1);
		}else if (en_staDir == EAST){
			if     (((g_sysMap[uc_staY][uc_staX] & 0x22)==0x20)&&(us_cmap[uc_staY][uc_staX+1]==us_high)) en_nowDir=EAST;
			else if(((g_sysMap[uc_staY][uc_staX] & 0x11)==0x10)&&(us_cmap[uc_staY+1][uc_staX]==us_high)) en_nowDir=NORTH;
			else if(((g_sysMap[uc_staY][uc_staX] & 0x44)==0x40)&&(us_cmap[uc_staY-1][uc_staX]==us_high)) en_nowDir=SOUTH;
			else if(((g_sysMap[uc_staY][uc_staX] & 0x88)==0x80)&&(us_cmap[uc_staY][uc_staX-1]==us_high)) en_nowDir=WEST;
			else   while(1);
		}else if (en_staDir == SOUTH){
			if     (((g_sysMap[uc_staY][uc_staX] & 0x44)==0x40)&&(us_cmap[uc_staY-1][uc_staX]==us_high)) en_nowDir=SOUTH;
			else if(((g_sysMap[uc_staY][uc_staX] & 0x22)==0x20)&&(us_cmap[uc_staY][uc_staX+1]==us_high)) en_nowDir=EAST;
			else if(((g_sysMap[uc_staY][uc_staX] & 0x88)==0x80)&&(us_cmap[uc_staY][uc_staX-1]==us_high)) en_nowDir=WEST;
			else if(((g_sysMap[uc_staY][uc_staX] & 0x11)==0x10)&&(us_cmap[uc_staY+1][uc_staX]==us_high)) en_nowDir=NORTH;
			else   while(1);
		}else if (en_staDir == WEST){
			if     (((g_sysMap[uc_staY][uc_staX] & 0x88)==0x80)&&(us_cmap[uc_staY][uc_staX-1]==us_high)) en_nowDir=WEST;
			else if(((g_sysMap[uc_staY][uc_staX] & 0x11)==0x10)&&(us_cmap[uc_staY+1][uc_staX]==us_high)) en_nowDir=NORTH;
			else if(((g_sysMap[uc_staY][uc_staX] & 0x44)==0x40)&&(us_cmap[uc_staY-1][uc_staX]==us_high)) en_nowDir=SOUTH;
			else if(((g_sysMap[uc_staY][uc_staX] & 0x22)==0x20)&&(us_cmap[uc_staY][uc_staX+1]==us_high)) en_nowDir=EAST;
			else   while(1);
		}
		
		en_tempDir = (enMAP_HEAD_DIR)( (en_nowDir - en_staDir) & (enMAP_HEAD_DIR)3 );		// ?øΩ?øΩ?øΩ?øΩ?øΩX?øΩV
		en_staDir = en_nowDir;

		if (en_tempDir == NORTH){
			uc_goStep = uc_goStep + 2;
		}
		else if (en_tempDir == EAST){
			dcom[us_pt] = uc_goStep;
			dcom[++us_pt] = R90;
			uc_goStep = 2;
			us_pt++;
		}
		else if (en_tempDir == WEST){
			dcom[us_pt] = uc_goStep;
			dcom[++us_pt] = L90;
			uc_goStep = 2;
			us_pt++;
		}
		else{
			dcom[us_pt] = uc_goStep;
			dcom[++us_pt] = R180;
			uc_goStep = 2;
			us_pt++;
		}

		if      (en_nowDir == NORTH) uc_staY = uc_staY + 1;
		else if (en_nowDir == EAST) uc_staX = uc_staX + 1;
		else if (en_nowDir == SOUTH) uc_staY = uc_staY - 1;
		else if (en_nowDir == WEST) uc_staX = uc_staX - 1;
		
		en_staDir = en_nowDir;
		
		if ((uc_staX == uc_endX) &&(uc_staY == uc_endY)) break;
	}
	
	/* ?øΩ?øΩ?øΩn?øΩM?øΩ?øΩ?øΩ?øΩp?øΩÃÉR?øΩ}?øΩ?øΩ?øΩh?øΩ?øΩ?øΩX?øΩg?øΩ?ê¨ */
	dcom[us_pt] = uc_goStep;
	dcom[++us_pt] = STOP;
	dcom[++us_pt] = CEND;
	us_totalCmd = us_pt+1;			// ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?øΩ?øΩ?øΩ


	/* ?øΩ≈èI?øΩI?øΩ…åÔøΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ */
	*en_endDir = en_staDir;
}

void MAP_makeSuraCmdList( void )
{
	uint16_t dcom_temp[4096];			// ?øΩ?øΩ?øΩ?øΩÊí¥?øΩM?øΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?øΩ?øΩX?øΩg
	uint16_t i=0,j=0;					// roop
	
	/* ?øΩ?øΩ?øΩn?øΩM?øΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?øΩ?øΩR?øΩs?øΩ[ */
	for( i=0; i<us_totalCmd; i++ ){
		dcom_temp[i] = dcom[i];
	}

	i = 0;

	/* ?øΩz?øΩ?™êÔøΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?øΩ?øΩ?øΩ?øΩ`?øΩF?øΩb?øΩN */
	while(1)
	{
		if( dcom_temp[i] == R90 ){		// ?øΩE90?øΩ?øΩ
			dcom_temp[i-1] -= 1;		// 1?øΩ¬éÔøΩO?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
			dcom_temp[i+1] -= 1;		// 1?øΩ¬éÔøΩO?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
			dcom_temp[i] = R90S;		// ?øΩE?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ90?øΩ?øΩ
		}
		else if( dcom_temp[i] == L90 ){	// ?øΩ?øΩ90?øΩ?øΩ
			dcom_temp[i-1] -= 1;		// 1?øΩ¬éÔøΩO?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
			dcom_temp[i+1] -= 1;		// 1?øΩ¬éÔøΩO?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
			dcom_temp[i] = L90S;		// ?øΩ?øΩ?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ90?øΩ?øΩ
		}
		else{
			if( dcom_temp[i] == CEND ){
				break;
			}
		}
		i++;
	}

	i = j = 0;

	/* ?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩœäÔøΩ */
	while(1)
	{
		if( dcom_temp[i+1] == CEND ){
			scom[j] = STOP;
			scom[j+1] = CEND;
			break;
		}
		else
		{
			/* ?øΩf?øΩ[?øΩ^?øΩ?øΩ?øΩX?øΩg?øΩb?øΩv?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ */
			if( dcom_temp[i] == 0 ){
				i++;
			}
			
			scom[j] = dcom_temp[i];
			
			i++;
			j++;
		}
	}
}

void MAP_makeSkewCmdList( void )
{
	uint16_t	scom_temp[4096];			// ?øΩ?øΩ?øΩ?øΩÊí¥?øΩM?øΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?øΩ?øΩX?øΩg
	uint16_t	i;							// roop
	uint16_t	c1, c2, c3, c4;				// ?øΩv?øΩZ?øΩp
	uint16_t	x;
	uint16_t	ct_n=0, ct_st=0;
	uint16_t	flag = 3;					//	?øΩŒÇﬂëÔøΩ?øΩs?øΩp?øΩo?øΩb?øΩt?øΩ@  0:?øΩ?øΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ@1:?øΩŒÇÔøΩ  2:S135N ?øΩ?øΩ N135S  3:?øΩ?øΩ?øΩi
	
	/* ?øΩ?øΩ?øΩn?øΩM?øΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?øΩ?øΩR?øΩs?øΩ[ */
	for( i=0; i<us_totalCmd; i++ )
	{
		scom_temp[i] = scom[i];
	}

	i=0;

	/* ?øΩz?øΩ?™êÔøΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?øΩ?øΩ?øΩ?øΩ`?øΩF?øΩb?øΩN */
	while(1)
	{
		c1 = scom_temp[ct_st];
		c2 = scom_temp[ct_st+1];
		c3 = scom_temp[ct_st+2];
		c4 = scom_temp[ct_st+3];

		//	?øΩ?øΩ?øΩi ?øΩ?øΩ ?øΩE45?øΩx ?øΩ?øΩ ?øΩŒÇÔøΩ
		if( (c1<=GO32) && (c2==R90S) && (c3==L90S) )
		{
			if((ct_st == 0) && (c1-1 == 0)){
				tcom[ ct_n ] = scom_temp[ct_st];
				ct_st ++;
				ct_n ++;
			}
			else{
				if( c1-1 != 0 ) tcom[ ct_n++ ] = c1 - 1;		//	?øΩO?øΩÃïÔøΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ…ÇÔøΩ?øΩ?øΩƒíÔøΩ?øΩ?øΩ?øΩ?øΩ‘ÇÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ»ÇÔøΩ?øΩÍç?
				tcom[ ct_n++ ] = RS45N;
				ct_st ++;

				x = (uint16_t)(NGO1 - 1);		//	?øΩŒÇﬂ??øΩ?øΩ[?øΩh
				flag = 0;
			}
		}
		//	?øΩ?øΩ?øΩi ?øΩ?øΩ ?øΩ?øΩ45?øΩx ?øΩ?øΩ ?øΩŒÇÔøΩ
		else if( (c1<=GO32) && (c2==L90S) && (c3==R90S) )
		{
			if((ct_st == 0) && (c1-1 == 0)){
				tcom[ ct_n ] = scom_temp[ct_st];
				ct_st ++;
				ct_n ++;
			}
			else{
				if( c1-1 != 0 ) tcom[ ct_n++ ] = c1 - 1;		//	?øΩO?øΩÃïÔøΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ…ÇÔøΩ?øΩ?øΩƒíÔøΩ?øΩ?øΩ?øΩ?øΩ‘ÇÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ»ÇÔøΩ?øΩÍç?
				tcom[ ct_n++ ] = LS45N;
				ct_st ++;

				x = (uint16_t)(NGO1 - 1);		//	?øΩŒÇﬂ??øΩ?øΩ[?øΩh
				flag = 0;
			}
		}

		//	?øΩ?øΩ?øΩi ?øΩ?øΩ ?øΩE90?øΩx ?øΩ?øΩ ?øΩ?øΩ?øΩi
		else if( (c1<=GO32) && (c2==R90S) && (c3<=GO32) )
		{
			tcom[ ct_n++ ] = c1;
			tcom[ ct_n++ ] = R90S;
			ct_st += 2;
			flag = 3;		//	?øΩ?øΩ?øΩi
		}
		//	?øΩ?øΩ?øΩi ?øΩ?øΩ ?øΩ?øΩ90?øΩx ?øΩ?øΩ ?øΩ?øΩ?øΩi
		else if( (c1<=GO32) && (c2==L90S) && (c3<=GO32) )
		{
			tcom[ ct_n++ ] = c1;
			tcom[ ct_n++ ] = L90S;
			ct_st += 2;
			flag = 3;		//	?øΩ?øΩ?øΩi
		}
		//	?øΩ?øΩ?øΩi ?øΩ?øΩ ?øΩE135?øΩx ?øΩ?øΩ ?øΩŒÇÔøΩ
		else if( (c1<=GO32) && (c2==R90S) && (c3==R90S) && (c4==L90S) )
		{
			tcom[ ct_n++ ] = c1;
			tcom[ ct_n++ ] = RS135N;
			ct_st += 2;

			x = (uint16_t)(NGO1 - 1);		//	?øΩŒÇﬂ??øΩ?øΩ[?øΩh
			flag = 2;
		}
		//	?øΩ?øΩ?øΩi ?øΩ?øΩ ?øΩ?øΩ135?øΩx ?øΩ?øΩ ?øΩŒÇÔøΩ
		else if( (c1<=GO32) && (c2==L90S) && (c3==L90S) && (c4==R90S) )
		{
			tcom[ ct_n++ ] = c1;
			tcom[ ct_n++ ] = LS135N;
			ct_st += 2;

			x = (uint16_t)(NGO1 - 1);		//	?øΩŒÇﬂ??øΩ?øΩ[?øΩh
			flag = 2;
		}

		//	?øΩ?øΩ?øΩi ?øΩ?øΩ ?øΩE180?øΩx ?øΩ?øΩ ?øΩ?øΩ?øΩi
		else if( (c1<=GO32) && (c2==R90S) && (c3==R90S) && (c4<=GO32) )
		{
			tcom[ ct_n++ ] = c1;
			tcom[ ct_n++ ] = R90S;
			tcom[ ct_n++ ] = R90S;
			ct_st += 3;
			flag = 3;		//	?øΩ?øΩ?øΩi
		}
		//	?øΩ?øΩ?øΩi ?øΩ?øΩ ?øΩ?øΩ180?øΩx ?øΩ?øΩ ?øΩ?øΩ?øΩi
		else if( (c1<=GO32) && (c2==L90S) && (c2==L90S) && (c4<=GO32) )
		{
			tcom[ ct_n++ ] = c1;
			tcom[ ct_n++ ] = L90S;
			tcom[ ct_n++ ] = L90S;
			ct_st += 3;
			flag = 3;		//	?øΩ?øΩ?øΩi
		}

		//	?øΩŒÇÔøΩ ?øΩ?øΩ ?øΩE45?øΩx ?øΩ?øΩ ?øΩ?øΩ?øΩi
		else if( (c1==R90S) && (c2<=GO32)  && (flag != 3 ) )
		{
			if( flag==1 ) tcom[ ct_n++ ] = x;
			tcom[ ct_n++ ] = RN45S;
			scom_temp[ct_st+1] = c2 - 1;		//	?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ‘ÇÔøΩ1?øΩ¬åÔøΩ?øΩÁÇ∑
			ct_st ++;
			flag = 3;		//	?øΩ?øΩ?øΩi
		}
		//	?øΩŒÇÔøΩ ?øΩ?øΩ ?øΩ?øΩ45?øΩx ?øΩ?øΩ ?øΩ?øΩ?øΩi
		else if( (c1==L90S) && (c2<=GO32)  && (flag != 3 ) )
		{
			if( flag==1 ) tcom[ ct_n++ ] = x;
			tcom[ ct_n++ ] = LN45S;
			scom_temp[ct_st+1] = c2 - 1;		//	?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ‘ÇÔøΩ1?øΩ¬åÔøΩ?øΩÁÇ∑
			ct_st ++;
			flag = 3;		//	?øΩ?øΩ?øΩi
		}
		//	?øΩŒÇÔøΩ ?øΩ?øΩ ?øΩE90?øΩx ?øΩ?øΩ ?øΩŒÇÔøΩ
		else if( (c1==L90S) && (c2==R90S) && (c3==R90S) && (c4==L90S)  && (flag != 3 ) )
		{
			if( flag==0 ) tcom[ ct_n++ ] = NGO1;		//	45N?øΩ?øΩ?øΩ?øΩRN90N
			else if( flag==1 ) tcom[ ct_n++ ] = x+1;
			else if( flag==2 ) tcom[ ct_n++ ] = NGO1;
			tcom[ ct_n++ ] = RN90N;
			ct_st +=2;

			x = (uint16_t)(NGO1 - 1);		//	?øΩŒÇﬂ??øΩ?øΩ[?øΩh
			flag = 1;
		}
		//	?øΩŒÇÔøΩ ?øΩ?øΩ ?øΩ?øΩ90?øΩx ?øΩ?øΩ ?øΩŒÇÔøΩ
		else if( (c1==R90S) && (c2==L90S) && (c3==L90S) && (c4==R90S)  && (flag != 3 ) )
		{
			if( flag==0 ) tcom[ ct_n++ ] = NGO1;		//	45N?øΩ?øΩ?øΩ?øΩLN90N
			else if( flag==1 ) tcom[ ct_n++ ] = x+1;
			else if( flag==2 ) tcom[ ct_n++ ] = NGO1;
			tcom[ ct_n++ ] = LN90N;
			ct_st +=2;

			x = (uint16_t)(NGO1 - 1);		//	?øΩŒÇﬂ??øΩ?øΩ[?øΩh
			flag = 1;
		}
		//	?øΩŒÇÔøΩ ?øΩ?øΩ ?øΩE135?øΩx ?øΩ?øΩ ?øΩ?øΩ?øΩi
		else if( (c1==L90S) && (c2==R90S) && (c3==R90S) && (c4<=GO32)  && (flag != 3 ) )
		{
			if( flag==0 ) tcom[ ct_n++ ] = NGO1;		//	45N?øΩ?øΩ?øΩ?øΩLN90N
			else if( flag==1 ) tcom[ ct_n++ ] = x+1;
			else if( flag==2 ) tcom[ ct_n++ ] = NGO1;
			tcom[ ct_n++ ] = RN135S;
			ct_st += 3;
			flag = 3;		//	?øΩ?øΩ?øΩi
		}
		//	?øΩŒÇÔøΩ ?øΩ?øΩ ?øΩ?øΩ135?øΩx ?øΩ?øΩ ?øΩ?øΩ?øΩi
		else if( (c1==R90S) && (c2==L90S) && (c3==L90S) && (c4<=GO32)  && (flag != 3 ) )
		{
			if( flag==0 ) tcom[ ct_n++ ] = NGO1;		//	45N?øΩ?øΩ?øΩ?øΩLN90N
			else if( flag==1 ) tcom[ ct_n++ ] = x+1;
			else if( flag==2 ) tcom[ ct_n++ ] = NGO1;
			tcom[ ct_n++ ] = LN135S;
			ct_st += 3;
			flag = 3;		//	?øΩ?øΩ?øΩi
		}
		//	?øΩŒÇÔøΩ ?øΩ?øΩ ?øΩŒÇÔøΩ
		else if( (c1==R90S) && (c2==L90S) && ( (c3==R90S) || (c3==L90S) || ( c3<=GO32 ) ) && (flag != 3 ) )
		{
			x++;
			ct_st ++;

			flag = 1;		//	?øΩŒÇﬂëÔøΩ?øΩs?øΩo?øΩb?øΩt?øΩ@?øΩ?øΩ?øΩ?øΩ
		}
		else if( (c1==L90S) && (c2==R90S) && ( (c3==L90S) || (c3==R90S) || ( c3<=GO32 ) ) && (flag != 3 ) )
		{
			//	?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩo?øΩ?øΩ
			x++;
			ct_st ++;

			flag = 1;		//	?øΩŒÇﬂëÔøΩ?øΩs?øΩo?øΩb?øΩt?øΩ@?øΩ?øΩ?øΩ?øΩ
		}
		else
		{
			tcom[ ct_n ] = scom_temp[ct_st];
			if( tcom[ ct_n ] == CEND ) break;
			ct_st ++;
			ct_n ++;
		}
	}
}

void MAP_drive( enMAP_DRIVE_TYPE en_driveType )
{
	uint16_t			us_rp = 0;				// ?øΩ?øΩ?øΩ›ÇÃì«Ç›çÔøΩ?øΩ›à íu
	enMOT_TURN_CMD 		en_type;
	bool			bl_isWallCut = FALSE;
	
	/* ?øΩ?øΩ?øΩM?øΩ?øΩ?øΩ?ÇÅ[?øΩh*/
	if( en_driveType == MAP_DRIVE_TURN )
	{
		while(1)
		{
			if ( dcom[us_rp] == CEND  ) break;								//	?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩI?øΩ?øΩ
			
			else if ( dcom[us_rp] == STOP  ){
			 	CTRL_stop();			// ?øΩ?øΩ?øΩ?øΩ?øΩ~
				DCM_brakeMot( DCM_R );		// ?øΩu?øΩ?øΩ?øΩ[?øΩL
				DCM_brakeMot( DCM_L );		// ?øΩu?øΩ?øΩ?øΩ[?øΩL
			}
			else if ( ( dcom[us_rp] <=  GO71 ) && ( dcom[us_rp] >=  GO1) )
			{
				MOT_goBlock_FinSpeed( (float)dcom[us_rp]*0.5f, 0 );		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩs?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩA?øΩ?øΩ?øΩ?øΩ‘ëO?øΩi?øΩ?øΩ…íÔøΩ~
			}
			else{
				
				if( dcom[us_rp] == R90 ) en_type = MOT_R90;
				else 					 en_type = MOT_L90;
				
				LL_mDelay(500);
				MOT_turn( en_type );		//	?øΩ?øΩ?øΩ?øΩ
				LL_mDelay(500);
			}
			us_rp++;
			
			/* ?øΩr?øΩ?øΩ?øΩ≈êÔøΩ?øΩ?øΩs?øΩ\?øΩ…Ç»ÇÔøΩ?øΩ?øΩ */
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot(DCM_R);
				DCM_brakeMot(DCM_L);
				break;
			}
			
		}
	 	CTRL_stop();			// ?øΩ?øΩ?øΩ?øΩ?øΩ~
		DCM_brakeMot( DCM_R );		// ?øΩu?øΩ?øΩ?øΩ[?øΩL
		DCM_brakeMot( DCM_L );		// ?øΩu?øΩ?øΩ?øΩ[?øΩL
	}
	/* ?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩh */
	else if( en_driveType == MAP_DRIVE_SURA )
	{
		while(1)
		{
			MAP_refPos( scom[us_rp] );									// ?øΩ?øΩ?øΩs?øΩ?øΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?øΩ?øΩI?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ íu?øΩ…çX?øΩV

			if ( scom[us_rp] == CEND  ) break;							//	?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩI?øΩ?øΩ
			
			else if ( scom[us_rp] == STOP  )
			{
			 	CTRL_stop();			// ?øΩ?øΩ?øΩ?øΩ?øΩ~
				DCM_brakeMot( DCM_R );		// ?øΩu?øΩ?øΩ?øΩ[?øΩL
				DCM_brakeMot( DCM_L );		// ?øΩu?øΩ?øΩ?øΩ[?øΩL
			}
			else if ( ( scom[us_rp] <=  GO71 ) && ( scom[us_rp] >=  GO1) )
			{
				if( scom[us_rp+1] == STOP  ){
					MOT_goBlock_FinSpeed( (float)scom[us_rp]*0.5f, 0 );						// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩs?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩA?øΩ?øΩ?øΩ?øΩ‘ëO?øΩi?øΩi?øΩ≈èI?øΩ?øΩ?øΩx?øΩ»ÇÔøΩ?øΩj
				}
				else{
					
					/* ?øΩ«ÇÃêÿÇÔøΩ⁄ï‚ê≥ */
					if( ( scom[us_rp+1] == R90S )   || ( scom[us_rp+1] == L90S ) ){
						bl_isWallCut = MAP_setWallCut( scom[us_rp+1] );		// ?øΩR?øΩ[?øΩi?øΩ[?øΩO?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ«ÇÃêÿÇÔøΩ⁄ï‚ê≥?øΩ?øΩ?øΩs?øΩ?øΩ?øΩ›íÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ
						
						if( bl_isWallCut == TRUE ){
							
							bl_isWallCut = FALSE;
							us_LogWallCut[us_LogIndexWallCut] = us_rp;
							us_LogIndexWallCut++;
							us_LogIndexWallCut %= 30;
						}
					}
					MOT_goBlock_FinSpeed( (float)scom[us_rp]*0.5f, MOT_getSuraStaSpeed() );		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩs?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩA?øΩ?øΩ?øΩ?øΩ‘ëO?øΩi?øΩi?øΩ≈èI?øΩ?øΩ?øΩx?øΩ?øΩ?øΩ?øΩj
				}
			}
			else if( scom[us_rp] == R90S )
			{
				MOT_goSla( MOT_R90S, PARAM_getSra( SLA_90 ) );	// ?øΩE?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ
			}
			else if( scom[us_rp] == L90S )
			{
				MOT_goSla( MOT_L90S, PARAM_getSra( SLA_90 ) );	// ?øΩ?øΩ?øΩX?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ?øΩ
			}
			us_rp++;
			
			/* ?øΩr?øΩ?øΩ?øΩ≈êÔøΩ?øΩ?øΩs?øΩ\?øΩ…Ç»ÇÔøΩ?øΩ?øΩ */
			if( SYS_isOutOfCtrl() == TRUE){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// ?øΩu?øΩ?øΩ?øΩ[?øΩL
				DCM_brakeMot( DCM_L );		// ?øΩu?øΩ?øΩ?øΩ[?øΩL
				break;
			}
			
		}
	}
	/* ?øΩŒÇﬂ??øΩ?øΩ[?øΩh */
	else if( en_driveType == MAP_DRIVE_SKEW )
	{
		while(1)
		{
			MAP_refPos( tcom[us_rp] );									// ?øΩ?øΩ?øΩs?øΩ?øΩ?øΩ?øΩ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?øΩ?øΩI?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ íu?øΩ…çX?øΩV
			
			if ( tcom[us_rp] == CEND  ) break;							//	?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩI?øΩ?øΩ

			else if ( tcom[us_rp] == STOP  )
			{
			 	CTRL_stop();			// ?øΩ?øΩ?øΩ?øΩ?øΩ~
				DCM_brakeMot( DCM_R );		// ?øΩu?øΩ?øΩ?øΩ[?øΩL
				DCM_brakeMot( DCM_L );		// ?øΩu?øΩ?øΩ?øΩ[?øΩL
			}
			else if ( ( tcom[us_rp] <=  GO71 ) && ( tcom[us_rp] >=  GO1) )
			{
				if( tcom[us_rp+1] == STOP  ){
					MOT_goBlock_FinSpeed( (float)tcom[us_rp]*0.5f, 0 );						// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩs?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩA?øΩ?øΩ?øΩ?øΩ‘ëO?øΩi?øΩi?øΩ≈èI?øΩ?øΩ?øΩx?øΩ»ÇÔøΩ?øΩj
				}
				else{
					
					/* ?øΩ«ÇÃêÿÇÔøΩ⁄ï‚ê≥ */
					if( ( tcom[us_rp+1] == R90S )   || ( tcom[us_rp+1] == L90S )   || 
					 	( tcom[us_rp+1] == RS135N ) || ( tcom[us_rp+1] == LS135N ) 
					 ){
						bl_isWallCut = MAP_setWallCut( tcom[us_rp+1] );		// ?øΩR?øΩ[?øΩi?øΩ[?øΩO?øΩ…ï«ÇÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ«ÇÃêÿÇÔøΩ⁄ï‚ê≥?øΩ?øΩ?øΩs?øΩ?øΩ?øΩ›íÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ
						
						if( bl_isWallCut == TRUE ){
							
							bl_isWallCut = FALSE;
							us_LogWallCut[us_LogIndexWallCut] = us_rp;
							us_LogIndexWallCut++;
							us_LogIndexWallCut %= 30;
						}
					}
					MOT_goBlock_FinSpeed( (float)tcom[us_rp]*0.5f, MOT_getSuraStaSpeed() );		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩs?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩA?øΩ?øΩ?øΩ?øΩ‘ëO?øΩi?øΩi?øΩ≈èI?øΩ?øΩ?øΩx?øΩ?øΩ?øΩ?øΩj
				}
			}
			else if ( ( tcom[us_rp] <=  NGO71 ) && ( tcom[us_rp] >=  NGO1) )
			{
				MOT_goSkewBlock_FinSpeed( (float)(tcom[us_rp]-81)*0.5f, MOT_getSuraStaSpeed());	// ?øΩŒÇﬂíÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩs?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩA?øΩ?øΩ?øΩ?øΩ‘ëO?øΩi?øΩi?øΩ≈èI?øΩ?øΩ?øΩx?øΩ?øΩ?øΩ?øΩj
			}
			else
			{
				switch( tcom[us_rp] )
				{

					/* ?øΩ?øΩ?øΩi ?øΩ?øΩ ?øΩ?øΩ?øΩi */
					case R90S:		MOT_goSla( MOT_R90S, PARAM_getSra( SLA_90 ) );			break;
					case L90S:		MOT_goSla( MOT_L90S, PARAM_getSra( SLA_90 ) );			break;
					
					/* ?øΩ?øΩ?øΩi ?øΩ?øΩ ?øΩŒÇÔøΩ */
					case RS45N:		MOT_goSla( MOT_R45S_S2N, PARAM_getSra( SLA_45 ) ); 		break;
					case LS45N:		MOT_goSla( MOT_L45S_S2N, PARAM_getSra( SLA_45 ) ); 		break;
					case RS135N:	MOT_goSla( MOT_R135S_S2N, PARAM_getSra( SLA_135 ) ); 	break;
					case LS135N:	MOT_goSla( MOT_L135S_S2N, PARAM_getSra( SLA_135 ) ); 	break;

					/* ?øΩŒÇÔøΩ ?øΩ?øΩ ?øΩ?øΩ?øΩi */
					case RN45S:		MOT_goSla( MOT_R45S_N2S, PARAM_getSra( SLA_45 ) ); 		break;
					case LN45S:		MOT_goSla( MOT_L45S_N2S, PARAM_getSra( SLA_45 ) ); 		break;
					case RN135S:	MOT_goSla( MOT_R135S_N2S, PARAM_getSra( SLA_135 ) ); 	break;
					case LN135S:	MOT_goSla( MOT_L135S_N2S, PARAM_getSra( SLA_135 ) ); 	break;

					/* ?øΩŒÇÔøΩ ?øΩ?øΩ ?øΩŒÇÔøΩ */
					case RN90N:		MOT_goSla( MOT_R90S_N, PARAM_getSra( SLA_N90 ) ); 		break;
					case LN90N:		MOT_goSla( MOT_L90S_N, PARAM_getSra( SLA_N90 ) );		break;
				}
			}
			us_rp++;
			
			/* ?øΩr?øΩ?øΩ?øΩ≈êÔøΩ?øΩ?øΩs?øΩ\?øΩ…Ç»ÇÔøΩ?øΩ?øΩ */
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// ?øΩu?øΩ?øΩ?øΩ[?øΩL
				DCM_brakeMot( DCM_L );		// ?øΩu?øΩ?øΩ?øΩ[?øΩL
				break;
			}
		}
	}
}

void MAP_searchCmdList(
	uint8_t uc_staX,					///< [in] ?øΩJ?øΩnX?øΩ?øΩ?øΩW
	uint8_t uc_staY,					///< [in] ?øΩJ?øΩnY?øΩ?øΩ?øΩW
	enMAP_HEAD_DIR en_staDir,		///< [in] ?øΩJ?øΩn?øΩ?øΩ?øΩÃïÔøΩ?øΩ?øΩ
	uint8_t uc_endX,					///< [in] ?øΩI?øΩ?øΩX?øΩ?øΩ?øΩW
	uint8_t uc_endY,					///< [in] ?øΩI?øΩ?øΩY?øΩ?øΩ?øΩW
	enMAP_HEAD_DIR* en_endDir		///< [out] ?øΩI?øΩ?øΩ?øΩ?øΩ?øΩÃïÔøΩ?øΩ?øΩ
) {
	uint8_t			uc_goStep;									// ?øΩO?øΩi?øΩÃÉX?øΩe?øΩb?øΩv?øΩ?øΩ
	uint16_t			us_high;									// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩÃçÔøΩ?øΩ?øΩ
	uint16_t			us_pt;										// ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ|?øΩC?øΩ?øΩ?øΩ^
	enMAP_HEAD_DIR	en_nowDir;									// ?øΩ?øΩ?øΩ›É}?øΩE?øΩX?øΩÃåÔøΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩ?øΩŒïÔøΩ?øΩ?øΩ
	enMAP_HEAD_DIR	en_tempDir;									// ?øΩ?øΩ?øΩŒïÔøΩ?øΩ?øΩ
//	uint16_t			i;											// roop

	/* ?øΩO?øΩi?øΩX?øΩe?øΩb?øΩv?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ */
	uc_goStep = 0;
	us_pt = 0;
//	printf("mx%d,my%d\n", uc_staX, uc_staY);
	/* ?øΩ?øΩ?øΩH?øΩ?øΩ?©ÇÔøΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?ê¨ */
	while (1) {
		us_high = us_cmap[uc_staY][uc_staX] - 1;
		if ((g_sysMap[uc_staY][uc_staX]&0xf0) != 0xf0){
			Return_X = uc_staX;
			Return_Y = uc_staY;
			break;
		}

		if (en_staDir == NORTH) {
			if (((g_sysMap[uc_staY][uc_staX] & 0x11) == 0x10) && (us_cmap[uc_staY + 1][uc_staX] == us_high)) en_nowDir = NORTH;
			else if (((g_sysMap[uc_staY][uc_staX] & 0x22) == 0x20) && (us_cmap[uc_staY][uc_staX + 1] == us_high)) en_nowDir = EAST;
			else if (((g_sysMap[uc_staY][uc_staX] & 0x88) == 0x80) && (us_cmap[uc_staY][uc_staX - 1] == us_high)) en_nowDir = WEST;
			else if (((g_sysMap[uc_staY][uc_staX] & 0x44) == 0x40) && (us_cmap[uc_staY - 1][uc_staX] == us_high)) en_nowDir = SOUTH;
			else   while (1);
		}
		else if (en_staDir == EAST) {
			if (((g_sysMap[uc_staY][uc_staX] & 0x22) == 0x20) && (us_cmap[uc_staY][uc_staX + 1] == us_high)) en_nowDir = EAST;
			else if (((g_sysMap[uc_staY][uc_staX] & 0x11) == 0x10) && (us_cmap[uc_staY + 1][uc_staX] == us_high)) en_nowDir = NORTH;
			else if (((g_sysMap[uc_staY][uc_staX] & 0x44) == 0x40) && (us_cmap[uc_staY - 1][uc_staX] == us_high)) en_nowDir = SOUTH;
			else if (((g_sysMap[uc_staY][uc_staX] & 0x88) == 0x80) && (us_cmap[uc_staY][uc_staX - 1] == us_high)) en_nowDir = WEST;
			else   while (1);
		}
		else if (en_staDir == SOUTH) {
			if (((g_sysMap[uc_staY][uc_staX] & 0x44) == 0x40) && (us_cmap[uc_staY - 1][uc_staX] == us_high)) en_nowDir = SOUTH;
			else if (((g_sysMap[uc_staY][uc_staX] & 0x22) == 0x20) && (us_cmap[uc_staY][uc_staX + 1] == us_high)) en_nowDir = EAST;
			else if (((g_sysMap[uc_staY][uc_staX] & 0x88) == 0x80) && (us_cmap[uc_staY][uc_staX - 1] == us_high)) en_nowDir = WEST;
			else if (((g_sysMap[uc_staY][uc_staX] & 0x11) == 0x10) && (us_cmap[uc_staY + 1][uc_staX] == us_high)) en_nowDir = NORTH;
			else   while (1);
		}
		else if (en_staDir == WEST) {
			if (((g_sysMap[uc_staY][uc_staX] & 0x88) == 0x80) && (us_cmap[uc_staY][uc_staX - 1] == us_high)) en_nowDir = WEST;
			else if (((g_sysMap[uc_staY][uc_staX] & 0x11) == 0x10) && (us_cmap[uc_staY + 1][uc_staX] == us_high)) en_nowDir = NORTH;
			else if (((g_sysMap[uc_staY][uc_staX] & 0x44) == 0x40) && (us_cmap[uc_staY - 1][uc_staX] == us_high)) en_nowDir = SOUTH;
			else if (((g_sysMap[uc_staY][uc_staX] & 0x22) == 0x20) && (us_cmap[uc_staY][uc_staX + 1] == us_high)) en_nowDir = EAST;
			else   while (1);
		}

		en_tempDir = (enMAP_HEAD_DIR)((en_nowDir - en_staDir) & (enMAP_HEAD_DIR)3);		// ?øΩ?øΩ?øΩ?øΩ?øΩX?øΩV
		en_staDir = en_nowDir;

		if (en_nowDir == NORTH) uc_staY = uc_staY + 1;
		else if (en_nowDir == EAST) uc_staX = uc_staX + 1;
		else if (en_nowDir == SOUTH) uc_staY = uc_staY - 1;
		else if (en_nowDir == WEST) uc_staX = uc_staX - 1;

		en_staDir = en_nowDir;

//		if ((uc_staX == uc_endX) && (uc_staY == uc_endY)) break;
		if (us_cmap[uc_staY][uc_staX] == 0) {
			Return_X = 0;
			Return_Y = 0;
			break;
		}
	}


	/* ?øΩ≈èI?øΩI?øΩ…åÔøΩ?øΩ?øΩ?øΩƒÇÔøΩ?øΩ?øΩ?øΩ?øΩ?øΩ */
	*en_endDir = en_staDir;
}
