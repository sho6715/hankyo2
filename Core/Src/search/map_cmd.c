/*
 * map_cmd.c
 *
 *  Created on: 2021/02/04
 *      Author: shohe
 */

#include "search/map_cmd.h"

#define LIST_NUM			( 4096 )				// ?申R?申}?申?申?申h?申?申?申s?申�??申?申X?申g?申?申

uint8_t		dcom[LIST_NUM];					// ?申?申?申n?申M?申?申?申?申p
uint8_t		scom[LIST_NUM];					// ?申X?申?申?申?申?申[?申?申?申p
uint8_t		tcom[LIST_NUM];					// ?申����鐃�?申s?申p
uint16_t		us_totalCmd;					// ?申g?申[?申^?申?申?申R?申}?申?申?申h?申?申

float		f_PosX;							// X?申?申?申W
float		f_PosY;							// Y?申?申?申W
int16_t		s_PosDir;						// ?申i?申s?申?申?申?申?申i[0]?申k [1]?申k?申?申 [2]?申?申 [3]?申?�� [4]?申?申 [5]?申?�� [6]?申?申 [7]?申k?申?申 ?申j

/* ?申?申?申s?申R?申}?申?申?申h */
typedef enum{

	STOP = 0,	// ?申?申~?申R?申}?申?申?申h
	
	/* ?申?申?申i */
	GO1,		// [  1]  1?申?申��O?申i?申R?申}?申?申?申h
	GO2,		// [  2]  2?申?申��O?申i?申R?申}?申?申?申h
	GO3,		// [  3]  3?申?申��O?申i?申R?申}?申?申?申h
	GO4,		// [  4]  4?申?申��O?申i?申R?申}?申?申?申h
	GO5,		// [  5]  5?申?申��O?申i?申R?申}?申?申?申h
	GO6,		// [  6]  6?申?申��O?申i?申R?申}?申?申?申h
	GO7,		// [  7]  7?申?申��O?申i?申R?申}?申?申?申h
	GO8,		// [  8]  8?申?申��O?申i?申R?申}?申?申?申h
	GO9,		// [  9]  9?申?申��O?申i?申R?申}?申?申?申h
	GO10,		// [ 10] 10?申?申��O?申i?申R?申}?申?申?申h
	GO11,		// [ 11] 11?申?申��O?申i?申R?申}?申?申?申h
	GO12,		// [ 12] 12?申?申��O?申i?申R?申}?申?申?申h
	GO13,		// [ 13] 13?申?申��O?申i?申R?申}?申?申?申h
	GO14,		// [ 14] 14?申?申��O?申i?申R?申}?申?申?申h
	GO15,		// [ 15] 15?申?申��O?申i?申R?申}?申?申?申h
	GO16,		// [ 16] 16?申?申��O?申i?申R?申}?申?申?申h
	GO17,		// [ 17] 17?申?申��O?申i?申R?申}?申?申?申h
	GO18,		// [ 18] 18?申?申��O?申i?申R?申}?申?申?申h
	GO19,		// [ 19] 19?申?申��O?申i?申R?申}?申?申?申h
	GO20,		// [ 20] 20?申?申��O?申i?申R?申}?申?申?申h
	GO21,		// [ 21] 21?申?申��O?申i?申R?申}?申?申?申h
	GO22,		// [ 22] 22?申?申��O?申i?申R?申}?申?申?申h
	GO23,		// [ 23] 23?申?申��O?申i?申R?申}?申?申?申h
	GO24,		// [ 24] 24?申?申��O?申i?申R?申}?申?申?申h
	GO25,		// [ 25] 25?申?申��O?申i?申R?申}?申?申?申h
	GO26,		// [ 26] 26?申?申��O?申i?申R?申}?申?申?申h
	GO27,		// [ 27] 27?申?申��O?申i?申R?申}?申?申?申h
	GO28,		// [ 28] 28?申?申��O?申i?申R?申}?申?申?申h
	GO29,		// [ 29] 29?申?申��O?申i?申R?申}?申?申?申h
	GO30,		// [ 30] 30?申?申��O?申i?申R?申}?申?申?申h
	GO31,		// [ 31] 31?申?申��O?申i?申R?申}?申?申?申h
	GO32,		// [ 32] 32?申?申��O?申i?申R?申}?申?申?申h
	GO33,		// [ 33] 33?申?申��O?申i?申R?申}?申?申?申h
	GO34,		// [ 34] 34?申?申��O?申i?申R?申}?申?申?申h
	GO35,		// [ 35] 35?申?申��O?申i?申R?申}?申?申?申h
	GO36,		// [ 36] 36?申?申��O?申i?申R?申}?申?申?申h
	GO37,		// [ 37] 37?申?申��O?申i?申R?申}?申?申?申h
	GO38,		// [ 38] 38?申?申��O?申i?申R?申}?申?申?申h
	GO39,		// [ 39] 39?申?申��O?申i?申R?申}?申?申?申h
	GO40,		// [ 40] 40?申?申��O?申i?申R?申}?申?申?申h
	GO41,		// [ 41] 41?申?申��O?申i?申R?申}?申?申?申h
	GO42,		// [ 42] 42?申?申��O?申i?申R?申}?申?申?申h
	GO43,		// [ 43] 43?申?申��O?申i?申R?申}?申?申?申h
	GO44,		// [ 44] 44?申?申��O?申i?申R?申}?申?申?申h
	GO45,		// [ 45] 45?申?申��O?申i?申R?申}?申?申?申h
	GO46,		// [ 46] 46?申?申��O?申i?申R?申}?申?申?申h
	GO47,		// [ 47] 47?申?申��O?申i?申R?申}?申?申?申h
	GO48,		// [ 48] 48?申?申��O?申i?申R?申}?申?申?申h
	GO49,		// [ 49] 49?申?申��O?申i?申R?申}?申?申?申h
	GO50,		// [ 50] 50?申?申��O?申i?申R?申}?申?申?申h
	GO51,		// [ 51] 51?申?申��O?申i?申R?申}?申?申?申h
	GO52,		// [ 52] 52?申?申��O?申i?申R?申}?申?申?申h
	GO53,		// [ 53] 53?申?申��O?申i?申R?申}?申?申?申h
	GO54,		// [ 54] 54?申?申��O?申i?申R?申}?申?申?申h
	GO55,		// [ 55] 55?申?申��O?申i?申R?申}?申?申?申h
	GO56,		// [ 56] 56?申?申��O?申i?申R?申}?申?申?申h
	GO57,		// [ 57] 57?申?申��O?申i?申R?申}?申?申?申h
	GO58,		// [ 58] 58?申?申��O?申i?申R?申}?申?申?申h
	GO59,		// [ 59] 59?申?申��O?申i?申R?申}?申?申?申h
	GO60,		// [ 60] 60?申?申��O?申i?申R?申}?申?申?申h
	GO61,		// [ 61] 61?申?申��O?申i?申R?申}?申?申?申h
	GO62,		// [ 62] 62?申?申��O?申i?申R?申}?申?申?申h
	GO63,		// [ 63] 63?申?申��O?申i?申R?申}?申?申?申h
	GO64,		// [ 64] 64?申?申��O?申i?申R?申}?申?申?申h
	GO65,		// [ 65] 65?申?申��O?申i?申R?申}?申?申?申h
	GO66,		// [ 66] 66?申?申��O?申i?申R?申}?申?申?申h
	GO67,		// [ 67] 67?申?申��O?申i?申R?申}?申?申?申h
	GO68,		// [ 68] 68?申?申��O?申i?申R?申}?申?申?申h
	GO69,		// [ 69] 69?申?申��O?申i?申R?申}?申?申?申h
	GO70,		// [ 70] 70?申?申��O?申i?申R?申}?申?申?申h
	GO71,		// [ 71] 71?申?申��O?申i?申R?申}?申?申?申h
	
	/* ?申?申?申n?申M?申?申?申?申 */
	R90,		// [ 72] ?申E90?申x?申E?申?申?申M?申?申?申?申R?申}?申?申?申h
	L90,		// [ 73] ?申?申90?申x?申?申?申?申?申M?申?申?申?申R?申}?申?申?申h
	R180,		// [ 74] ?申E180?申x?申?申?申M?申?申?申?申R?申}?申?申?申h
	L180,		// [ 75] ?申?申180?申x?申?申?申M?申?申?申?申R?申}?申?申?申h

	/* ?申X?申?申?申?申?申[?申?申 */
	R90S,		// [ 76] ?申E90?申x?申?申?申?申R?申}?申?申?申h
	L90S,		// [ 77] ?申?申90?申x?申?申?申?申R?申}?申?申?申h
	R90F,		// [ 78] ?申E?申?申?申?申90?申x?申?申?申?申R?申}?申?申?申h
	L90F,		// [ 79] ?申?申?申?申?申?申90?申x?申?申?申?申R?申}?申?申?申h
	R180F,		// [ 80] ?申E?申?申?申?申180?申x?申?申?申?申R?申}?申?申?申h
	L180F,		// [ 81] ?申?申?申?申?申?申180?申x?申?申?申?申R?申}?申?申?申h

	/* ?申��鐃� */
	NGO1,		// [ 82] ?申��鐃� 1?申?申��O?申i?申R?申}?申?申?申h
	NGO2,		// [ 83] ?申��鐃� 2?申?申��O?申i?申R?申}?申?申?申h
	NGO3,		// [ 84] ?申��鐃� 3?申?申��O?申i?申R?申}?申?申?申h
	NGO4,		// [ 85] ?申��鐃� 4?申?申��O?申i?申R?申}?申?申?申h
	NGO5,		// [ 86] ?申��鐃� 5?申?申��O?申i?申R?申}?申?申?申h
	NGO6,		// [ 87] ?申��鐃� 6?申?申��O?申i?申R?申}?申?申?申h
	NGO7,		// [ 88] ?申��鐃� 7?申?申��O?申i?申R?申}?申?申?申h
	NGO8,		// [ 89] ?申��鐃� 8?申?申��O?申i?申R?申}?申?申?申h
	NGO9,		// [ 90] ?申��鐃� 9?申?申��O?申i?申R?申}?申?申?申h
	NGO10,		// [ 91] ?申��鐃�10?申?申��O?申i?申R?申}?申?申?申h
	NGO11,		// [ 92] ?申��鐃�11?申?申��O?申i?申R?申}?申?申?申h
	NGO12,		// [ 93] ?申��鐃�12?申?申��O?申i?申R?申}?申?申?申h
	NGO13,		// [ 94] ?申��鐃�13?申?申��O?申i?申R?申}?申?申?申h
	NGO14,		// [ 95] ?申��鐃�14?申?申��O?申i?申R?申}?申?申?申h
	NGO15,		// [ 96] ?申��鐃�15?申?申��O?申i?申R?申}?申?申?申h
	NGO16,		// [ 97] ?申��鐃�16?申?申��O?申i?申R?申}?申?申?申h
	NGO17,		// [ 98] ?申��鐃�17?申?申��O?申i?申R?申}?申?申?申h
	NGO18,		// [ 99] ?申��鐃�18?申?申��O?申i?申R?申}?申?申?申h
	NGO19,		// [100] ?申��鐃�19?申?申��O?申i?申R?申}?申?申?申h
	NGO20,		// [101] ?申��鐃�20?申?申��O?申i?申R?申}?申?申?申h
	NGO21,		// [102] ?申��鐃�21?申?申��O?申i?申R?申}?申?申?申h
	NGO22,		// [103] ?申��鐃�22?申?申��O?申i?申R?申}?申?申?申h
	NGO23,		// [104] ?申��鐃�23?申?申��O?申i?申R?申}?申?申?申h
	NGO24,		// [105] ?申��鐃�24?申?申��O?申i?申R?申}?申?申?申h
	NGO25,		// [106] ?申��鐃�25?申?申��O?申i?申R?申}?申?申?申h
	NGO26,		// [107] ?申��鐃�26?申?申��O?申i?申R?申}?申?申?申h
	NGO27,		// [108] ?申��鐃�27?申?申��O?申i?申R?申}?申?申?申h
	NGO28,		// [109] ?申��鐃�28?申?申��O?申i?申R?申}?申?申?申h
	NGO29,		// [110] ?申��鐃�29?申?申��O?申i?申R?申}?申?申?申h
	NGO30,		// [111] ?申��鐃�30?申?申��O?申i?申R?申}?申?申?申h
	NGO31,		// [112] ?申��鐃�31?申?申��O?申i?申R?申}?申?申?申h
	NGO32,		// [113] ?申��鐃�32?申?申��O?申i?申R?申}?申?申?申h
	NGO33,		// [114] ?申��鐃�33?申?申��O?申i?申R?申}?申?申?申h
	NGO34,		// [115] ?申��鐃�34?申?申��O?申i?申R?申}?申?申?申h
	NGO35,		// [116] ?申��鐃�35?申?申��O?申i?申R?申}?申?申?申h
	NGO36,		// [117] ?申��鐃�36?申?申��O?申i?申R?申}?申?申?申h
	NGO37,		// [118] ?申��鐃�37?申?申��O?申i?申R?申}?申?申?申h
	NGO38,		// [119] ?申��鐃�38?申?申��O?申i?申R?申}?申?申?申h
	NGO39,		// [120] ?申��鐃�39?申?申��O?申i?申R?申}?申?申?申h
	NGO40,		// [121] ?申��鐃�40?申?申��O?申i?申R?申}?申?申?申h
	NGO41,		// [122] ?申��鐃�41?申?申��O?申i?申R?申}?申?申?申h
	NGO42,		// [123] ?申��鐃�42?申?申��O?申i?申R?申}?申?申?申h
	NGO43,		// [124] ?申��鐃�43?申?申��O?申i?申R?申}?申?申?申h
	NGO44,		// [125] ?申��鐃�44?申?申��O?申i?申R?申}?申?申?申h
	NGO45,		// [126] ?申��鐃�45?申?申��O?申i?申R?申}?申?申?申h
	NGO46,		// [127] ?申��鐃�46?申?申��O?申i?申R?申}?申?申?申h
	NGO47,		// [128] ?申��鐃�47?申?申��O?申i?申R?申}?申?申?申h
	NGO48,		// [129] ?申��鐃�48?申?申��O?申i?申R?申}?申?申?申h
	NGO49,		// [130] ?申��鐃�49?申?申��O?申i?申R?申}?申?申?申h
	NGO50,		// [131] ?申��鐃�50?申?申��O?申i?申R?申}?申?申?申h
	NGO51,		// [132] ?申��鐃�51?申?申��O?申i?申R?申}?申?申?申h
	NGO52,		// [133] ?申��鐃�52?申?申��O?申i?申R?申}?申?申?申h
	NGO53,		// [134] ?申��鐃�53?申?申��O?申i?申R?申}?申?申?申h
	NGO54,		// [135] ?申��鐃�54?申?申��O?申i?申R?申}?申?申?申h
	NGO55,		// [136] ?申��鐃�55?申?申��O?申i?申R?申}?申?申?申h
	NGO56,		// [137] ?申��鐃�56?申?申��O?申i?申R?申}?申?申?申h
	NGO57,		// [138] ?申��鐃�57?申?申��O?申i?申R?申}?申?申?申h
	NGO58,		// [139] ?申��鐃�58?申?申��O?申i?申R?申}?申?申?申h
	NGO59,		// [140] ?申��鐃�59?申?申��O?申i?申R?申}?申?申?申h
	NGO60,		// [141] ?申��鐃�60?申?申��O?申i?申R?申}?申?申?申h
	NGO61,		// [142] ?申��鐃�61?申?申��O?申i?申R?申}?申?申?申h
	NGO62,		// [143] ?申��鐃�62?申?申��O?申i?申R?申}?申?申?申h
	NGO63,		// [144] ?申��鐃�63?申?申��O?申i?申R?申}?申?申?申h
	NGO64,		// [145] ?申��鐃�64?申?申��O?申i?申R?申}?申?申?申h
	NGO65,		// [146] ?申��鐃�65?申?申��O?申i?申R?申}?申?申?申h
	NGO66,		// [147] ?申��鐃�66?申?申��O?申i?申R?申}?申?申?申h
	NGO67,		// [148] ?申��鐃�67?申?申��O?申i?申R?申}?申?申?申h
	NGO68,		// [149] ?申��鐃�68?申?申��O?申i?申R?申}?申?申?申h
	NGO69,		// [150] ?申��鐃�69?申?申��O?申i?申R?申}?申?申?申h
	NGO70,		// [151] ?申��鐃�70?申?申��O?申i?申R?申}?申?申?申h
	NGO71,		// [152] ?申��鐃�71?申?申��O?申i?申R?申}?申?申?申h

	RS45N,		// [153] ?申?申?申i -> ?申E45   -> ?申��鐃�
	LS45N,		// [154] ?申?申?申i -> ?申?申45   -> ?申��鐃�
	RS135N,		// [155] ?申?申?申i -> ?申E135  -> ?申��鐃�
	LS135N,		// [156] ?申?申?申i -> ?申?申135  -> ?申��鐃�
	RN45S,		// [157] ?申��鐃� -> ?申E45   -> ?申?申?申i
	LN45S,		// [158] ?申��鐃� -> ?申?申45   -> ?申?申?申i
	RN135S,		// [159] ?申��鐃� -> ?申E135  -> ?申?申?申i
	LN135S,		// [160] ?申��鐃� -> ?申?申135  -> ?申?申?申i
	RN90N,		// [161] ?申��鐃� -> ?申E90   -> ?申��鐃�
	LN90N,		// [162] ?申��鐃� -> ?申?申90   -> ?申��鐃�

	CEND = 250,	// [250] ?申R?申}?申?申?申h?申I?申?申

	MAP_CMD_MAX,
}enMAP_CMD;

typedef struct{
	enMAP_CMD	en_cmd;			// ?申R?申}?申?申?申h
	float		f_x0_x1;		// [0]/[1]?申?申X?申?申?申W?申?申?申Z?申l
	float		f_y0_y1;		// [0]/[1]?申?申y?申?申?申W?申?申?申Z?申l
	float		f_x2_x3;		// [2]/[3]?申?申X?申?申?申W?申?申?申Z?申l
	float		f_y2_y3;		// [2]/[3]?申?申y?申?申?申W?申?申?申Z?申l
	float		f_x4_x5;		// [4]/[5]?申?申X?申?申?申W?申?申?申Z?申l
	float		f_y4_y5;		// [4]/[5]?申?申y?申?申?申W?申?申?申Z?申l
	float		f_x6_x7;		// [6]/[7]?申?申X?申?申?申W?申?申?申Z?申l
	float		f_y6_y7;		// [6]/[7]?申?申y?申?申?申W?申?申?申Z?申l
	int16_t		s_dir;			// ?申i?申s?申?申?申?申?申i[0]?申k [1]?申k?申?申 [2]?申?申 [3]?申?�� [4]?申?申 [5]?申?�� [6]?申?申 [7]?申k?申?申 ?申j
}stMAP_SIM;

/* ?申R?申}?申?申?申h?申��鐃�?申?申?申?申?申?申?申W?申X?申V?申f?申[?申^ */
const stMAP_SIM st_PosData[] = {
	
	//	?申R?申}?申?申?申h	[0]/[1]?申?申X	[0]/[1]?申?申Y	[2]/[3]?申?申X	[2]/[3]?申?申Y	[4]/[5]?申?申X	[4]/[5]?申?申Y	[6]/[7]?申?申X	[6]/[7]?申?申Y	?申?申?申?申
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
	uint8_t uc_index = 0;			// ?申e?申[?申u?申?申?申��C?申?申?申f?申b?申N?申X?申��鐃�
	
	/* ------------------------------------------ */
	/*  ?申R?申}?申?申?申h?申?申?申?申e?申[?申u?申?申?申��C?申?申?申f?申b?申N?申X?申?申?申���  */
	/* ------------------------------------------ */
	/* ?申?申?申i */
	if( ( uc_cmd <=  GO71 ) && ( uc_cmd >=  GO1) ){
		
		uc_index = 14;		// st_PosData?申e?申[?申u?申?申?申��鐃�?申i?申��C?申?申?申f?申b?申N?申X?申��鐃�
	}
	/* ?申����鐃�?申i */
	else if( ( uc_cmd <=  NGO71 ) && ( uc_cmd >=  NGO1) ){
		
		uc_index = 15;		// st_PosData?申e?申[?申u?申?申?申�ﾎ���鐃�?申i?申��C?申?申?申f?申b?申N?申X?申��鐃�
	}
	/* ?申?申?申��鐃�?申��R?申}?申?申?申h */
	else{
		while(1){
			
			if( st_PosData[uc_index].en_cmd == uc_cmd )      break;			// ?申R?申}?申?申?申h?申?申?申?申
			if( st_PosData[uc_index].en_cmd == MAP_CMD_MAX ) return;		// ?申R?申}?申?申?申h?申?申?申?申?申?申
			uc_index++;
		}
	}
	
	/* ?申��u?申X?申V */
	switch( s_PosDir ){
		
		/* [0]?申k [1]?申k?申?申 */
		case 0:
		case 1:
		
			/* ?申?申?申i */
			if( uc_index == 14 ){
				
				f_PosX += st_PosData[uc_index].f_x0_x1 * uc_cmd;
				f_PosY += st_PosData[uc_index].f_y0_y1 * uc_cmd;
			}
			/* ?申����鐃�?申i */
			else if( uc_index == 15 ){
				
				f_PosX += st_PosData[uc_index].f_x0_x1 * ( uc_cmd - 81 );
				f_PosY += st_PosData[uc_index].f_y0_y1 * ( uc_cmd - 81 );
			}
			/* ?申?申?申��鐃�?申��J?申[?申u */
			else{
				f_PosX += st_PosData[uc_index].f_x0_x1;
				f_PosY += st_PosData[uc_index].f_y0_y1;
			}
			break;
		
		/* [2]?申?申 [3]?申?�� */
		case 2:
		case 3:

			/* ?申?申?申i */
			if( uc_index == 14 ){
				
				f_PosX += st_PosData[uc_index].f_x2_x3 * uc_cmd;
				f_PosY += st_PosData[uc_index].f_y2_y3 * uc_cmd;
			}
			/* ?申����鐃�?申i */
			else if( uc_index == 15 ){
				
				f_PosX += st_PosData[uc_index].f_x2_x3 * ( uc_cmd - 81 );
				f_PosY += st_PosData[uc_index].f_y2_y3 * ( uc_cmd - 81 );
			}
			/* ?申?申?申��鐃�?申��J?申[?申u */
			else{
				f_PosX += st_PosData[uc_index].f_x2_x3;
				f_PosY += st_PosData[uc_index].f_y2_y3;
			}
			break;

		/* [4]?申?申 [5]?申?�� */
		case 4:
		case 5:

			/* ?申?申?申i */
			if( uc_index == 14 ){
				
				f_PosX += st_PosData[uc_index].f_x4_x5 * uc_cmd;
				f_PosY += st_PosData[uc_index].f_y4_y5 * uc_cmd;
			}
			/* ?申����鐃�?申i */
			else if( uc_index == 15 ){
				
				f_PosX += st_PosData[uc_index].f_x4_x5 * ( uc_cmd - 81 );
				f_PosY += st_PosData[uc_index].f_y4_y5 * ( uc_cmd - 81 );
			}
			/* ?申?申?申��鐃�?申��J?申[?申u */
			else{
				f_PosX += st_PosData[uc_index].f_x4_x5;
				f_PosY += st_PosData[uc_index].f_y4_y5;
			}
			break;

		/* [6]?申?申 [7]?申k?申?申 */
		case 6:
		case 7:

			/* ?申?申?申i */
			if( uc_index == 14 ){
				
				f_PosX += st_PosData[uc_index].f_x6_x7 * uc_cmd;
				f_PosY += st_PosData[uc_index].f_y6_y7 * uc_cmd;
			}
			/* ?申����鐃�?申i */
			else if( uc_index == 15 ){
				
				f_PosX += st_PosData[uc_index].f_x6_x7 * ( uc_cmd - 81 );
				f_PosY += st_PosData[uc_index].f_y6_y7 * ( uc_cmd - 81 );
			}
			/* ?申?申?申��鐃�?申��J?申[?申u */
			else{
				f_PosX += st_PosData[uc_index].f_x6_x7;
				f_PosY += st_PosData[uc_index].f_y6_y7;
			}
			break;
	}
	
	/* ?申i?申s?申?申?申?申?申X?申V */
	s_PosDir += st_PosData[uc_index].s_dir;
	if( s_PosDir < 0 ) s_PosDir += 8;				// [0]?申`[7]?申��鐃�?申?申?申?申
	else if( s_PosDir > 7 ) s_PosDir -= 8;
	
	f_LogPosX[us_LogIndex] = f_PosX;
	f_LogPosY[us_LogIndex] = f_PosY;
	
	us_LogIndex++;
	us_LogIndex %= 30;
}

bool MAP_setWallCut( uint8_t uc_cmd )
{
	uint8_t uc_val = 0;			// 1?申?申?申O?申��R?申[?申i?申[?申?申?申����鐃�?申?申?申���?申i0?申��O?申��鐃叔�鐃�?申?申j
	uint8_t uc_valPrev = 0;		// 2?申?申?申O?申��R?申[?申i?申[?申?申?申����鐃�?申?申?申���?申i0?申��O?申��鐃叔�鐃�?申?申j
	bool bl_wallCut = FALSE;
	
	/* ?申��u?申X?申V */
	switch( uc_cmd ){
		
		case R90S:
		case RS135N:
			
			/* 1?申?申?申O?申��R?申[?申i?申[?申?申?申����鐃�?申?申?申���?申i0?申��O?申�����?申?申j */
			// s_PosDir?申F?申i?申s?申?申?申?申?申i[0]?申k [1]?申k?申?申 [2]?申?申 [3]?申?�� [4]?申?申 [5]?申?�� [6]?申?申 [7]?申k?申?申 ?申j
			switch( s_PosDir ){
				
				/* ?申?申?申���?申��鐃�?申?��鐃縮���A?申?申?申?申?申?申O?申?申?申����L?申?申?申?����鐃�?申?申?申?申?申W?申����鐃�i?申?申?申��Fg_sysMap?申?申2?申?申?申?申?申z?申?申��鐃�?申j */
				case 0: 
					if( 0 < f_PosY-0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY-0.5)][(uint8_t)(f_PosX)] & 0x02;		// ?申k?申?申?申?申?申?申?申��鐃�?申?申����鐃�?申?申?申����鐃�?申?申?申���
					if( 0 < f_PosY-1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY-1.5)][(uint8_t)(f_PosX)] & 0x02;		// ?申k?申?申?申?申?申?申?申��鐃�?申?申����鐃�?申?申?申����鐃�?申?申?申���
					break;	
				case 2: 
					if( 0 < f_PosX-0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX-0.5)] & 0x04;		// ?申?申?申?申?申?申?申?申?申��鐃�?申?申�������?申����鐃�?申?申?申���
					if( 0 < f_PosX-1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX-1.5)] & 0x04;		// ?申?申?申?申?申?申?申?申?申��鐃�?申?申�������?申����鐃�?申?申?申���
					break;
				case 4: 
					if( MAP_Y_SIZE_REAL > f_PosY+0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY+0.5)][(uint8_t)(f_PosX)] & 0x08;		// ?申?申?申?申?申?申?申?申��鐃�?申?申����鐃�?申?申?申����鐃�?申?申?申���
					if( MAP_Y_SIZE_REAL > f_PosY+1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY+1.5)][(uint8_t)(f_PosX)] & 0x08;		// ?申?申?申?申?申?申?申?申��鐃�?申?申����鐃�?申?申?申����鐃�?申?申?申���
					break;
				case 6:
					if( MAP_X_SIZE_REAL > f_PosX+0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX+0.5)] & 0x01;		// ?申?申?申?申?申?申?申?申?申��鐃�?申?申����k?申?申?申����鐃�?申?申?申���
					if( MAP_X_SIZE_REAL > f_PosX+1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX+1.5)] & 0x01;		// ?申?申?申?申?申?申?申?申?申��鐃�?申?申����k?申?申?申����鐃�?申?申?申���
					break;
			}
			/* ?申��鐃�?申?申?申���?申������鐃盾��?申?申?申s?申?申 */
			if( ( uc_val != 0 ) || ( ( uc_val != 0 ) && ( uc_valPrev != 0 ) ) ){
				
				MOT_setWallEdgeType( MOT_WALL_EDGE_RIGHT );		// ?申����鐃盾��?申?申?申?申?申{?申?申?申?申
				bl_wallCut = TRUE;
			}
			break;
			
		case L90S:
		case LS135N:
			/* 1?申?申?申O?申��R?申[?申i?申[?申?申?申����鐃�?申?申?申���?申i0?申��O?申�����?申?申j */
			// s_PosDir?申F?申i?申s?申?申?申?申?申i[0]?申k [1]?申k?申?申 [2]?申?申 [3]?申?�� [4]?申?申 [5]?申?�� [6]?申?申 [7]?申k?申?申 ?申j
			switch( s_PosDir ){
				
				/* ?申?申?申���?申��鐃�?申?��鐃縮���A?申?申?申?申?申?申O?申?申?申����L?申?申?申?����鐃�?申?申?申?申?申W?申����鐃�i?申?申?申��Fg_sysMap?申?申2?申?申?申?申?申z?申?申��鐃�?申j */
				case 0: 
					if( 0 < f_PosY-0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY-0.5)][(uint8_t)(f_PosX)] & 0x08;			// ?申k?申?申?申?申?申?申?申��鐃�?申?申����鐃�?申?申?申����鐃�?申?申?申���
					if( 0 < f_PosY-1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY-1.5)][(uint8_t)(f_PosX)] & 0x08;			// ?申k?申?申?申?申?申?申?申��鐃�?申?申����鐃�?申?申?申����鐃�?申?申?申���
					break;
				case 2: 
					if( 0 < f_PosX-0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX-0.5)] & 0x01;			// ?申?申?申?申?申?申?申?申?申��鐃�?申?申����k?申?申?申����鐃�?申?申?申���
					if( 0 < f_PosX-1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX-1.5)] & 0x01;			// ?申?申?申?申?申?申?申?申?申��鐃�?申?申����k?申?申?申����鐃�?申?申?申���
					break;
				case 4: 
					if( MAP_Y_SIZE_REAL > f_PosY+0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY+0.5)][(uint8_t)(f_PosX)] & 0x02;			// ?申?申?申?申?申?申?申?申��鐃�?申?申����鐃�?申?申?申����鐃�?申?申?申���
					if( MAP_Y_SIZE_REAL > f_PosY+1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY+1.5)][(uint8_t)(f_PosX)] & 0x02;			// ?申?申?申?申?申?申?申?申��鐃�?申?申����鐃�?申?申?申����鐃�?申?申?申���
					break;
				case 6: 
					if( MAP_X_SIZE_REAL > f_PosX+0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX+0.5)] & 0x04;			// ?申?申?申?申?申?申?申?申?申��鐃�?申?申�������?申����鐃�?申?申?申���
					if( MAP_X_SIZE_REAL > f_PosX+1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX+1.5)] & 0x04;			// ?申?申?申?申?申?申?申?申?申��鐃�?申?申�������?申����鐃�?申?申?申���
					break;
			}
			/* ?申��鐃�?申?申?申���?申������鐃盾��?申?申?申s?申?申 */
			if( ( uc_val != 0 ) || ( ( uc_val != 0 ) && ( uc_valPrev != 0 ) ) ){
				
				MOT_setWallEdgeType( MOT_WALL_EDGE_LEFT );		// ?申����鐃盾��?申?申?申?申?申{?申?申?申?申
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
	s_PosDir = (int16_t)(en_dir * 2);	// ?申i?申s?申?申?申?申?申i[0]?申k [1]?申k?申?申 [2]?申?申 [3]?申?�� [4]?申?申 [5]?申?�� [6]?申?申 [7]?申k?申?申 ?申j?申A2?申{?申?申?申?申��鐃�?申x?申l?申?申?申?申?申v?申?申?申?申
}

void MAP_showCmdLog( void )
{
	uint16_t i=0;
	
	/* ?申?申?申M?申n?申?申?申?申R?申}?申?申?申h */
	while(1){
		
		printf("dcom[%4d] = %02d  \n\r",i,dcom[i]);
		if( dcom[i] == CEND ) break;
		i++;
	}
	i=0;
	
	/* ?申X?申?申?申?申?申[?申?申?申R?申}?申?申?申h */
	while(1){
		
		printf("scom[%4d] = %02d  \n\r",i,scom[i]);
		if( scom[i] == CEND ) break;
		i++;
	}
	i=0;

	/* ?申����鐃�?申s?申R?申}?申?申?申h */
	while(1){
		
		printf("tcom[%4d] = %02d  \n\r",i,tcom[i]);
		if( tcom[i] == CEND ) break;
		i++;
	}
}

void MAP_makeCmdList( 
	uint8_t uc_staX,					///< [in] ?申J?申nX?申?申?申W
	uint8_t uc_staY,					///< [in] ?申J?申nY?申?申?申W
	enMAP_HEAD_DIR en_staDir,		///< [in] ?申J?申n?申?申?申��鐃�?申?申
	uint8_t uc_endX,					///< [in] ?申I?申?申X?申?申?申W
	uint8_t uc_endY,					///< [in] ?申I?申?申Y?申?申?申W
	enMAP_HEAD_DIR* en_endDir		///< [out] ?申I?申?申?申?申?申��鐃�?申?申
){
	uint8_t			uc_goStep;									// ?申O?申i?申��X?申e?申b?申v?申?申
	uint16_t			us_high;									// ?申?申?申?申?申?申?申��鐃�?申?申
	uint16_t			us_pt;										// ?申R?申}?申?申?申h?申|?申C?申?申?申^
	enMAP_HEAD_DIR	en_nowDir;									// ?申?申?申��}?申E?申X?申��鐃�?申?申?申��鐃�?申?申?申��鐃�?申?申
	enMAP_HEAD_DIR	en_tempDir;									// ?申?申?申��鐃�?申?申
//	uint16_t			i;											// roop
	
	/* ?申O?申i?申X?申e?申b?申v?申?申?申?申?申?申?申?申?申?申?申?申?申?申 */
	uc_goStep = 0;
	us_pt = 0;

	/* ?申?申?申H?申?申?��鐃�R?申}?申?申?申h?申?�� */
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
		
		en_tempDir = (enMAP_HEAD_DIR)( (en_nowDir - en_staDir) & (enMAP_HEAD_DIR)3 );		// ?申?申?申?申?申X?申V
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
	
	/* ?申?申?申n?申M?申?申?申?申p?申��R?申}?申?申?申h?申?申?申X?申g?申?�� */
	dcom[us_pt] = uc_goStep;
	dcom[++us_pt] = STOP;
	dcom[++us_pt] = CEND;
	us_totalCmd = us_pt+1;			// ?申R?申}?申?申?申h?申?申?申?申


	/* ?申��I?申I?申��鐃�?申?申?申��鐃�?申?申?申?申?申 */
	*en_endDir = en_staDir;
}

void MAP_makeSuraCmdList( void )
{
	uint16_t dcom_temp[4096];			// ?申?申?申?申���?申M?申?申?申?申R?申}?申?申?申h?申?申?申X?申g
	uint16_t i=0,j=0;					// roop
	
	/* ?申?申?申n?申M?申?申?申?申R?申}?申?申?申h?申?申?申R?申s?申[ */
	for( i=0; i<us_totalCmd; i++ ){
		dcom_temp[i] = dcom[i];
	}

	i = 0;

	/* ?申z?申?��鐃�?申?申R?申}?申?申?申h?申?申?申?申?申`?申F?申b?申N */
	while(1)
	{
		if( dcom_temp[i] == R90 ){		// ?申E90?申?申
			dcom_temp[i-1] -= 1;		// 1?申��鐃�O?申?申?申?申?申?申
			dcom_temp[i+1] -= 1;		// 1?申��鐃�O?申?申?申?申?申?申
			dcom_temp[i] = R90S;		// ?申E?申X?申?申?申?申?申[?申?申90?申?申
		}
		else if( dcom_temp[i] == L90 ){	// ?申?申90?申?申
			dcom_temp[i-1] -= 1;		// 1?申��鐃�O?申?申?申?申?申?申
			dcom_temp[i+1] -= 1;		// 1?申��鐃�O?申?申?申?申?申?申
			dcom_temp[i] = L90S;		// ?申?申?申X?申?申?申?申?申[?申?申90?申?申
		}
		else{
			if( dcom_temp[i] == CEND ){
				break;
			}
		}
		i++;
	}

	i = j = 0;

	/* ?申X?申?申?申?申?申[?申?申?申R?申}?申?申?申h?申��鐃� */
	while(1)
	{
		if( dcom_temp[i+1] == CEND ){
			scom[j] = STOP;
			scom[j+1] = CEND;
			break;
		}
		else
		{
			/* ?申f?申[?申^?申?申?申X?申g?申b?申v?申R?申}?申?申?申h?申?申?申?申?申?申?申?申 */
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
	uint16_t	scom_temp[4096];			// ?申?申?申?申���?申M?申?申?申?申R?申}?申?申?申h?申?申?申X?申g
	uint16_t	i;							// roop
	uint16_t	c1, c2, c3, c4;				// ?申v?申Z?申p
	uint16_t	x;
	uint16_t	ct_n=0, ct_st=0;
	uint16_t	flag = 3;					//	?申����鐃�?申s?申p?申o?申b?申t?申@  0:?申?申?申?申?申R?申}?申?申?申h?申@1:?申��鐃�  2:S135N ?申?申 N135S  3:?申?申?申i
	
	/* ?申?申?申n?申M?申?申?申?申R?申}?申?申?申h?申?申?申R?申s?申[ */
	for( i=0; i<us_totalCmd; i++ )
	{
		scom_temp[i] = scom[i];
	}

	i=0;

	/* ?申z?申?��鐃�?申?申R?申}?申?申?申h?申?申?申?申?申`?申F?申b?申N */
	while(1)
	{
		c1 = scom_temp[ct_st];
		c2 = scom_temp[ct_st+1];
		c3 = scom_temp[ct_st+2];
		c4 = scom_temp[ct_st+3];

		//	?申?申?申i ?申?申 ?申E45?申x ?申?申 ?申��鐃�
		if( (c1<=GO32) && (c2==R90S) && (c3==L90S) )
		{
			if((ct_st == 0) && (c1-1 == 0)){
				tcom[ ct_n ] = scom_temp[ct_st];
				ct_st ++;
				ct_n ++;
			}
			else{
				if( c1-1 != 0 ) tcom[ ct_n++ ] = c1 - 1;		//	?申O?申��鐃�?申?申?申R?申}?申?申?申h?申��鐃�?申?申��鐃�?申?申?申?申��鐃�?申?申?申?申?申��鐃�?申��?
				tcom[ ct_n++ ] = RS45N;
				ct_st ++;

				x = (uint16_t)(NGO1 - 1);		//	?申���??申?申[?申h
				flag = 0;
			}
		}
		//	?申?申?申i ?申?申 ?申?申45?申x ?申?申 ?申��鐃�
		else if( (c1<=GO32) && (c2==L90S) && (c3==R90S) )
		{
			if((ct_st == 0) && (c1-1 == 0)){
				tcom[ ct_n ] = scom_temp[ct_st];
				ct_st ++;
				ct_n ++;
			}
			else{
				if( c1-1 != 0 ) tcom[ ct_n++ ] = c1 - 1;		//	?申O?申��鐃�?申?申?申R?申}?申?申?申h?申��鐃�?申?申��鐃�?申?申?申?申��鐃�?申?申?申?申?申��鐃�?申��?
				tcom[ ct_n++ ] = LS45N;
				ct_st ++;

				x = (uint16_t)(NGO1 - 1);		//	?申���??申?申[?申h
				flag = 0;
			}
		}

		//	?申?申?申i ?申?申 ?申E90?申x ?申?申 ?申?申?申i
		else if( (c1<=GO32) && (c2==R90S) && (c3<=GO32) )
		{
			tcom[ ct_n++ ] = c1;
			tcom[ ct_n++ ] = R90S;
			ct_st += 2;
			flag = 3;		//	?申?申?申i
		}
		//	?申?申?申i ?申?申 ?申?申90?申x ?申?申 ?申?申?申i
		else if( (c1<=GO32) && (c2==L90S) && (c3<=GO32) )
		{
			tcom[ ct_n++ ] = c1;
			tcom[ ct_n++ ] = L90S;
			ct_st += 2;
			flag = 3;		//	?申?申?申i
		}
		//	?申?申?申i ?申?申 ?申E135?申x ?申?申 ?申��鐃�
		else if( (c1<=GO32) && (c2==R90S) && (c3==R90S) && (c4==L90S) )
		{
			tcom[ ct_n++ ] = c1;
			tcom[ ct_n++ ] = RS135N;
			ct_st += 2;

			x = (uint16_t)(NGO1 - 1);		//	?申���??申?申[?申h
			flag = 2;
		}
		//	?申?申?申i ?申?申 ?申?申135?申x ?申?申 ?申��鐃�
		else if( (c1<=GO32) && (c2==L90S) && (c3==L90S) && (c4==R90S) )
		{
			tcom[ ct_n++ ] = c1;
			tcom[ ct_n++ ] = LS135N;
			ct_st += 2;

			x = (uint16_t)(NGO1 - 1);		//	?申���??申?申[?申h
			flag = 2;
		}

		//	?申?申?申i ?申?申 ?申E180?申x ?申?申 ?申?申?申i
		else if( (c1<=GO32) && (c2==R90S) && (c3==R90S) && (c4<=GO32) )
		{
			tcom[ ct_n++ ] = c1;
			tcom[ ct_n++ ] = R90S;
			tcom[ ct_n++ ] = R90S;
			ct_st += 3;
			flag = 3;		//	?申?申?申i
		}
		//	?申?申?申i ?申?申 ?申?申180?申x ?申?申 ?申?申?申i
		else if( (c1<=GO32) && (c2==L90S) && (c2==L90S) && (c4<=GO32) )
		{
			tcom[ ct_n++ ] = c1;
			tcom[ ct_n++ ] = L90S;
			tcom[ ct_n++ ] = L90S;
			ct_st += 3;
			flag = 3;		//	?申?申?申i
		}

		//	?申��鐃� ?申?申 ?申E45?申x ?申?申 ?申?申?申i
		else if( (c1==R90S) && (c2<=GO32)  && (flag != 3 ) )
		{
			if( flag==1 ) tcom[ ct_n++ ] = x;
			tcom[ ct_n++ ] = RN45S;
			scom_temp[ct_st+1] = c2 - 1;		//	?申?申?申?申?申?申��鐃�1?申��鐃�?申���
			ct_st ++;
			flag = 3;		//	?申?申?申i
		}
		//	?申��鐃� ?申?申 ?申?申45?申x ?申?申 ?申?申?申i
		else if( (c1==L90S) && (c2<=GO32)  && (flag != 3 ) )
		{
			if( flag==1 ) tcom[ ct_n++ ] = x;
			tcom[ ct_n++ ] = LN45S;
			scom_temp[ct_st+1] = c2 - 1;		//	?申?申?申?申?申?申��鐃�1?申��鐃�?申���
			ct_st ++;
			flag = 3;		//	?申?申?申i
		}
		//	?申��鐃� ?申?申 ?申E90?申x ?申?申 ?申��鐃�
		else if( (c1==L90S) && (c2==R90S) && (c3==R90S) && (c4==L90S)  && (flag != 3 ) )
		{
			if( flag==0 ) tcom[ ct_n++ ] = NGO1;		//	45N?申?申?申?申RN90N
			else if( flag==1 ) tcom[ ct_n++ ] = x+1;
			else if( flag==2 ) tcom[ ct_n++ ] = NGO1;
			tcom[ ct_n++ ] = RN90N;
			ct_st +=2;

			x = (uint16_t)(NGO1 - 1);		//	?申���??申?申[?申h
			flag = 1;
		}
		//	?申��鐃� ?申?申 ?申?申90?申x ?申?申 ?申��鐃�
		else if( (c1==R90S) && (c2==L90S) && (c3==L90S) && (c4==R90S)  && (flag != 3 ) )
		{
			if( flag==0 ) tcom[ ct_n++ ] = NGO1;		//	45N?申?申?申?申LN90N
			else if( flag==1 ) tcom[ ct_n++ ] = x+1;
			else if( flag==2 ) tcom[ ct_n++ ] = NGO1;
			tcom[ ct_n++ ] = LN90N;
			ct_st +=2;

			x = (uint16_t)(NGO1 - 1);		//	?申���??申?申[?申h
			flag = 1;
		}
		//	?申��鐃� ?申?申 ?申E135?申x ?申?申 ?申?申?申i
		else if( (c1==L90S) && (c2==R90S) && (c3==R90S) && (c4<=GO32)  && (flag != 3 ) )
		{
			if( flag==0 ) tcom[ ct_n++ ] = NGO1;		//	45N?申?申?申?申LN90N
			else if( flag==1 ) tcom[ ct_n++ ] = x+1;
			else if( flag==2 ) tcom[ ct_n++ ] = NGO1;
			tcom[ ct_n++ ] = RN135S;
			ct_st += 3;
			flag = 3;		//	?申?申?申i
		}
		//	?申��鐃� ?申?申 ?申?申135?申x ?申?申 ?申?申?申i
		else if( (c1==R90S) && (c2==L90S) && (c3==L90S) && (c4<=GO32)  && (flag != 3 ) )
		{
			if( flag==0 ) tcom[ ct_n++ ] = NGO1;		//	45N?申?申?申?申LN90N
			else if( flag==1 ) tcom[ ct_n++ ] = x+1;
			else if( flag==2 ) tcom[ ct_n++ ] = NGO1;
			tcom[ ct_n++ ] = LN135S;
			ct_st += 3;
			flag = 3;		//	?申?申?申i
		}
		//	?申��鐃� ?申?申 ?申��鐃�
		else if( (c1==R90S) && (c2==L90S) && ( (c3==R90S) || (c3==L90S) || ( c3<=GO32 ) ) && (flag != 3 ) )
		{
			x++;
			ct_st ++;

			flag = 1;		//	?申����鐃�?申s?申o?申b?申t?申@?申?申?申?申
		}
		else if( (c1==L90S) && (c2==R90S) && ( (c3==L90S) || (c3==R90S) || ( c3<=GO32 ) ) && (flag != 3 ) )
		{
			//	?申R?申}?申?申?申h?申o?申?申
			x++;
			ct_st ++;

			flag = 1;		//	?申����鐃�?申s?申o?申b?申t?申@?申?申?申?申
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
	uint16_t			us_rp = 0;				// ?申?申?申��������鐃�?申����u
	enMOT_TURN_CMD 		en_type;
	bool			bl_isWallCut = FALSE;
	
	/* ?申?申?申M?申?申?申?��[?申h*/
	if( en_driveType == MAP_DRIVE_TURN )
	{
		while(1)
		{
			if ( dcom[us_rp] == CEND  ) break;								//	?申R?申}?申?申?申h?申I?申?申
			
			else if ( dcom[us_rp] == STOP  ){
			 	CTRL_stop();			// ?申?申?申?申?申~
				DCM_brakeMot( DCM_R );		// ?申u?申?申?申[?申L
				DCM_brakeMot( DCM_L );		// ?申u?申?申?申[?申L
			}
			else if ( ( dcom[us_rp] <=  GO71 ) && ( dcom[us_rp] >=  GO1) )
			{
				MOT_goBlock_FinSpeed( (float)dcom[us_rp]*0.5f, 0 );		// ?申?申?申?申?申?申?申s?申R?申}?申?申?申h?申A?申?申?申?申��O?申i?申?申��鐃�~
			}
			else{
				
				if( dcom[us_rp] == R90 ) en_type = MOT_R90;
				else 					 en_type = MOT_L90;
				
				LL_mDelay(500);
				MOT_turn( en_type );		//	?申?申?申?申
				LL_mDelay(500);
			}
			us_rp++;
			
			/* ?申r?申?申?申��鐃�?申?申s?申\?申����鐃�?申?申 */
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot(DCM_R);
				DCM_brakeMot(DCM_L);
				break;
			}
			
		}
	 	CTRL_stop();			// ?申?申?申?申?申~
		DCM_brakeMot( DCM_R );		// ?申u?申?申?申[?申L
		DCM_brakeMot( DCM_L );		// ?申u?申?申?申[?申L
	}
	/* ?申X?申?申?申?申?申[?申?申?申?申?申[?申h */
	else if( en_driveType == MAP_DRIVE_SURA )
	{
		while(1)
		{
			MAP_refPos( scom[us_rp] );									// ?申?申?申s?申?申?申?申?申R?申}?申?申?申h?申?申?申I?申?申?申?申?申?申?申��u?申��X?申V

			if ( scom[us_rp] == CEND  ) break;							//	?申R?申}?申?申?申h?申I?申?申
			
			else if ( scom[us_rp] == STOP  )
			{
			 	CTRL_stop();			// ?申?申?申?申?申~
				DCM_brakeMot( DCM_R );		// ?申u?申?申?申[?申L
				DCM_brakeMot( DCM_L );		// ?申u?申?申?申[?申L
			}
			else if ( ( scom[us_rp] <=  GO71 ) && ( scom[us_rp] >=  GO1) )
			{
				if( scom[us_rp+1] == STOP  ){
					MOT_goBlock_FinSpeed( (float)scom[us_rp]*0.5f, 0 );						// ?申?申?申?申?申?申?申s?申R?申}?申?申?申h?申A?申?申?申?申��O?申i?申i?申��I?申?申?申x?申��鐃�?申j
				}
				else{
					
					/* ?申������鐃准���� */
					if( ( scom[us_rp+1] == R90S )   || ( scom[us_rp+1] == L90S ) ){
						bl_isWallCut = MAP_setWallCut( scom[us_rp+1] );		// ?申R?申[?申i?申[?申O?申����鐃�?申?申?申?申?申?申?申?申������鐃准����?申?申?申s?申?申?申��鐃�?申?申?申?申?申
						
						if( bl_isWallCut == TRUE ){
							
							bl_isWallCut = FALSE;
							us_LogWallCut[us_LogIndexWallCut] = us_rp;
							us_LogIndexWallCut++;
							us_LogIndexWallCut %= 30;
						}
					}
					MOT_goBlock_FinSpeed( (float)scom[us_rp]*0.5f, MOT_getSuraStaSpeed() );		// ?申?申?申?申?申?申?申s?申R?申}?申?申?申h?申A?申?申?申?申��O?申i?申i?申��I?申?申?申x?申?申?申?申j
				}
			}
			else if( scom[us_rp] == R90S )
			{
				MOT_goSla( MOT_R90S, PARAM_getSra( SLA_90 ) );	// ?申E?申X?申?申?申?申?申[?申?申
			}
			else if( scom[us_rp] == L90S )
			{
				MOT_goSla( MOT_L90S, PARAM_getSra( SLA_90 ) );	// ?申?申?申X?申?申?申?申?申[?申?申
			}
			us_rp++;
			
			/* ?申r?申?申?申��鐃�?申?申s?申\?申����鐃�?申?申 */
			if( SYS_isOutOfCtrl() == TRUE){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// ?申u?申?申?申[?申L
				DCM_brakeMot( DCM_L );		// ?申u?申?申?申[?申L
				break;
			}
			
		}
	}
	/* ?申���??申?申[?申h */
	else if( en_driveType == MAP_DRIVE_SKEW )
	{
		while(1)
		{
			MAP_refPos( tcom[us_rp] );									// ?申?申?申s?申?申?申?申?申R?申}?申?申?申h?申?申?申I?申?申?申?申?申?申?申��u?申��X?申V
			
			if ( tcom[us_rp] == CEND  ) break;							//	?申R?申}?申?申?申h?申I?申?申

			else if ( tcom[us_rp] == STOP  )
			{
			 	CTRL_stop();			// ?申?申?申?申?申~
				DCM_brakeMot( DCM_R );		// ?申u?申?申?申[?申L
				DCM_brakeMot( DCM_L );		// ?申u?申?申?申[?申L
			}
			else if ( ( tcom[us_rp] <=  GO71 ) && ( tcom[us_rp] >=  GO1) )
			{
				if( tcom[us_rp+1] == STOP  ){
					MOT_goBlock_FinSpeed( (float)tcom[us_rp]*0.5f, 0 );						// ?申?申?申?申?申?申?申s?申R?申}?申?申?申h?申A?申?申?申?申��O?申i?申i?申��I?申?申?申x?申��鐃�?申j
				}
				else{
					
					/* ?申������鐃准���� */
					if( ( tcom[us_rp+1] == R90S )   || ( tcom[us_rp+1] == L90S )   || 
					 	( tcom[us_rp+1] == RS135N ) || ( tcom[us_rp+1] == LS135N ) 
					 ){
						bl_isWallCut = MAP_setWallCut( tcom[us_rp+1] );		// ?申R?申[?申i?申[?申O?申����鐃�?申?申?申?申?申?申?申?申������鐃准����?申?申?申s?申?申?申��鐃�?申?申?申?申?申
						
						if( bl_isWallCut == TRUE ){
							
							bl_isWallCut = FALSE;
							us_LogWallCut[us_LogIndexWallCut] = us_rp;
							us_LogIndexWallCut++;
							us_LogIndexWallCut %= 30;
						}
					}
					MOT_goBlock_FinSpeed( (float)tcom[us_rp]*0.5f, MOT_getSuraStaSpeed() );		// ?申?申?申?申?申?申?申s?申R?申}?申?申?申h?申A?申?申?申?申��O?申i?申i?申��I?申?申?申x?申?申?申?申j
				}
			}
			else if ( ( tcom[us_rp] <=  NGO71 ) && ( tcom[us_rp] >=  NGO1) )
			{
				MOT_goSkewBlock_FinSpeed( (float)(tcom[us_rp]-81)*0.5f, MOT_getSuraStaSpeed());	// ?申����鐃�?申?申?申?申?申s?申R?申}?申?申?申h?申A?申?申?申?申��O?申i?申i?申��I?申?申?申x?申?申?申?申j
			}
			else
			{
				switch( tcom[us_rp] )
				{

					/* ?申?申?申i ?申?申 ?申?申?申i */
					case R90S:		MOT_goSla( MOT_R90S, PARAM_getSra( SLA_90 ) );			break;
					case L90S:		MOT_goSla( MOT_L90S, PARAM_getSra( SLA_90 ) );			break;
					
					/* ?申?申?申i ?申?申 ?申��鐃� */
					case RS45N:		MOT_goSla( MOT_R45S_S2N, PARAM_getSra( SLA_45 ) ); 		break;
					case LS45N:		MOT_goSla( MOT_L45S_S2N, PARAM_getSra( SLA_45 ) ); 		break;
					case RS135N:	MOT_goSla( MOT_R135S_S2N, PARAM_getSra( SLA_135 ) ); 	break;
					case LS135N:	MOT_goSla( MOT_L135S_S2N, PARAM_getSra( SLA_135 ) ); 	break;

					/* ?申��鐃� ?申?申 ?申?申?申i */
					case RN45S:		MOT_goSla( MOT_R45S_N2S, PARAM_getSra( SLA_45 ) ); 		break;
					case LN45S:		MOT_goSla( MOT_L45S_N2S, PARAM_getSra( SLA_45 ) ); 		break;
					case RN135S:	MOT_goSla( MOT_R135S_N2S, PARAM_getSra( SLA_135 ) ); 	break;
					case LN135S:	MOT_goSla( MOT_L135S_N2S, PARAM_getSra( SLA_135 ) ); 	break;

					/* ?申��鐃� ?申?申 ?申��鐃� */
					case RN90N:		MOT_goSla( MOT_R90S_N, PARAM_getSra( SLA_N90 ) ); 		break;
					case LN90N:		MOT_goSla( MOT_L90S_N, PARAM_getSra( SLA_N90 ) );		break;
				}
			}
			us_rp++;
			
			/* ?申r?申?申?申��鐃�?申?申s?申\?申����鐃�?申?申 */
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// ?申u?申?申?申[?申L
				DCM_brakeMot( DCM_L );		// ?申u?申?申?申[?申L
				break;
			}
		}
	}
}

void MAP_searchCmdList(
	uint8_t uc_staX,					///< [in] ?申J?申nX?申?申?申W
	uint8_t uc_staY,					///< [in] ?申J?申nY?申?申?申W
	enMAP_HEAD_DIR en_staDir,		///< [in] ?申J?申n?申?申?申��鐃�?申?申
	uint8_t uc_endX,					///< [in] ?申I?申?申X?申?申?申W
	uint8_t uc_endY,					///< [in] ?申I?申?申Y?申?申?申W
	enMAP_HEAD_DIR* en_endDir		///< [out] ?申I?申?申?申?申?申��鐃�?申?申
) {
	uint8_t			uc_goStep;									// ?申O?申i?申��X?申e?申b?申v?申?申
	uint16_t			us_high;									// ?申?申?申?申?申?申?申��鐃�?申?申
	uint16_t			us_pt;										// ?申R?申}?申?申?申h?申|?申C?申?申?申^
	enMAP_HEAD_DIR	en_nowDir;									// ?申?申?申��}?申E?申X?申��鐃�?申?申?申��鐃�?申?申?申��鐃�?申?申
	enMAP_HEAD_DIR	en_tempDir;									// ?申?申?申��鐃�?申?申
//	uint16_t			i;											// roop

	/* ?申O?申i?申X?申e?申b?申v?申?申?申?申?申?申?申?申?申?申?申?申?申?申 */
	uc_goStep = 0;
	us_pt = 0;
//	printf("mx%d,my%d\n", uc_staX, uc_staY);
	/* ?申?申?申H?申?申?��鐃�R?申}?申?申?申h?申?�� */
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

		en_tempDir = (enMAP_HEAD_DIR)((en_nowDir - en_staDir) & (enMAP_HEAD_DIR)3);		// ?申?申?申?申?申X?申V
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


	/* ?申��I?申I?申��鐃�?申?申?申��鐃�?申?申?申?申?申 */
	*en_endDir = en_staDir;
}
