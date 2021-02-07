/*
 * map_cmd.c
 *
 *  Created on: 2021/02/04
 *      Author: shohe
 */

#include "search/map_cmd.h"

#define LIST_NUM			( 4096 )				// �R�}���h���s�̃��X�g��

uint8_t		dcom[LIST_NUM];					// ���n�M����p
uint8_t		scom[LIST_NUM];					// �X�����[���p
uint8_t		tcom[LIST_NUM];					// �΂ߑ��s�p
uint16_t		us_totalCmd;					// �g�[�^���R�}���h��

float		f_PosX;							// X���W
float		f_PosY;							// Y���W
int16_t		s_PosDir;						// �i�s�����i[0]�k [1]�k�� [2]�� [3]�쓌 [4]�� [5]�쐼 [6]�� [7]�k�� �j

/* ���s�R�}���h */
typedef enum{

	STOP = 0,	// ��~�R�}���h
	
	/* ���i */
	GO1,		// [  1]  1��ԑO�i�R�}���h
	GO2,		// [  2]  2��ԑO�i�R�}���h
	GO3,		// [  3]  3��ԑO�i�R�}���h
	GO4,		// [  4]  4��ԑO�i�R�}���h
	GO5,		// [  5]  5��ԑO�i�R�}���h
	GO6,		// [  6]  6��ԑO�i�R�}���h
	GO7,		// [  7]  7��ԑO�i�R�}���h
	GO8,		// [  8]  8��ԑO�i�R�}���h
	GO9,		// [  9]  9��ԑO�i�R�}���h
	GO10,		// [ 10] 10��ԑO�i�R�}���h
	GO11,		// [ 11] 11��ԑO�i�R�}���h
	GO12,		// [ 12] 12��ԑO�i�R�}���h
	GO13,		// [ 13] 13��ԑO�i�R�}���h
	GO14,		// [ 14] 14��ԑO�i�R�}���h
	GO15,		// [ 15] 15��ԑO�i�R�}���h
	GO16,		// [ 16] 16��ԑO�i�R�}���h
	GO17,		// [ 17] 17��ԑO�i�R�}���h
	GO18,		// [ 18] 18��ԑO�i�R�}���h
	GO19,		// [ 19] 19��ԑO�i�R�}���h
	GO20,		// [ 20] 20��ԑO�i�R�}���h
	GO21,		// [ 21] 21��ԑO�i�R�}���h
	GO22,		// [ 22] 22��ԑO�i�R�}���h
	GO23,		// [ 23] 23��ԑO�i�R�}���h
	GO24,		// [ 24] 24��ԑO�i�R�}���h
	GO25,		// [ 25] 25��ԑO�i�R�}���h
	GO26,		// [ 26] 26��ԑO�i�R�}���h
	GO27,		// [ 27] 27��ԑO�i�R�}���h
	GO28,		// [ 28] 28��ԑO�i�R�}���h
	GO29,		// [ 29] 29��ԑO�i�R�}���h
	GO30,		// [ 30] 30��ԑO�i�R�}���h
	GO31,		// [ 31] 31��ԑO�i�R�}���h
	GO32,		// [ 32] 32��ԑO�i�R�}���h
	GO33,		// [ 33] 33��ԑO�i�R�}���h
	GO34,		// [ 34] 34��ԑO�i�R�}���h
	GO35,		// [ 35] 35��ԑO�i�R�}���h
	GO36,		// [ 36] 36��ԑO�i�R�}���h
	GO37,		// [ 37] 37��ԑO�i�R�}���h
	GO38,		// [ 38] 38��ԑO�i�R�}���h
	GO39,		// [ 39] 39��ԑO�i�R�}���h
	GO40,		// [ 40] 40��ԑO�i�R�}���h
	GO41,		// [ 41] 41��ԑO�i�R�}���h
	GO42,		// [ 42] 42��ԑO�i�R�}���h
	GO43,		// [ 43] 43��ԑO�i�R�}���h
	GO44,		// [ 44] 44��ԑO�i�R�}���h
	GO45,		// [ 45] 45��ԑO�i�R�}���h
	GO46,		// [ 46] 46��ԑO�i�R�}���h
	GO47,		// [ 47] 47��ԑO�i�R�}���h
	GO48,		// [ 48] 48��ԑO�i�R�}���h
	GO49,		// [ 49] 49��ԑO�i�R�}���h
	GO50,		// [ 50] 50��ԑO�i�R�}���h
	GO51,		// [ 51] 51��ԑO�i�R�}���h
	GO52,		// [ 52] 52��ԑO�i�R�}���h
	GO53,		// [ 53] 53��ԑO�i�R�}���h
	GO54,		// [ 54] 54��ԑO�i�R�}���h
	GO55,		// [ 55] 55��ԑO�i�R�}���h
	GO56,		// [ 56] 56��ԑO�i�R�}���h
	GO57,		// [ 57] 57��ԑO�i�R�}���h
	GO58,		// [ 58] 58��ԑO�i�R�}���h
	GO59,		// [ 59] 59��ԑO�i�R�}���h
	GO60,		// [ 60] 60��ԑO�i�R�}���h
	GO61,		// [ 61] 61��ԑO�i�R�}���h
	GO62,		// [ 62] 62��ԑO�i�R�}���h
	GO63,		// [ 63] 63��ԑO�i�R�}���h
	GO64,		// [ 64] 64��ԑO�i�R�}���h
	GO65,		// [ 65] 65��ԑO�i�R�}���h
	GO66,		// [ 66] 66��ԑO�i�R�}���h
	GO67,		// [ 67] 67��ԑO�i�R�}���h
	GO68,		// [ 68] 68��ԑO�i�R�}���h
	GO69,		// [ 69] 69��ԑO�i�R�}���h
	GO70,		// [ 70] 70��ԑO�i�R�}���h
	GO71,		// [ 71] 71��ԑO�i�R�}���h
	
	/* ���n�M���� */
	R90,		// [ 72] �E90�x�E���M����R�}���h
	L90,		// [ 73] ��90�x�����M����R�}���h
	R180,		// [ 74] �E180�x���M����R�}���h
	L180,		// [ 75] ��180�x���M����R�}���h

	/* �X�����[�� */
	R90S,		// [ 76] �E90�x����R�}���h
	L90S,		// [ 77] ��90�x����R�}���h
	R90F,		// [ 78] �E����90�x����R�}���h
	L90F,		// [ 79] ������90�x����R�}���h
	R180F,		// [ 80] �E����180�x����R�}���h
	L180F,		// [ 81] ������180�x����R�}���h

	/* �΂� */
	NGO1,		// [ 82] �΂� 1��ԑO�i�R�}���h
	NGO2,		// [ 83] �΂� 2��ԑO�i�R�}���h
	NGO3,		// [ 84] �΂� 3��ԑO�i�R�}���h
	NGO4,		// [ 85] �΂� 4��ԑO�i�R�}���h
	NGO5,		// [ 86] �΂� 5��ԑO�i�R�}���h
	NGO6,		// [ 87] �΂� 6��ԑO�i�R�}���h
	NGO7,		// [ 88] �΂� 7��ԑO�i�R�}���h
	NGO8,		// [ 89] �΂� 8��ԑO�i�R�}���h
	NGO9,		// [ 90] �΂� 9��ԑO�i�R�}���h
	NGO10,		// [ 91] �΂�10��ԑO�i�R�}���h
	NGO11,		// [ 92] �΂�11��ԑO�i�R�}���h
	NGO12,		// [ 93] �΂�12��ԑO�i�R�}���h
	NGO13,		// [ 94] �΂�13��ԑO�i�R�}���h
	NGO14,		// [ 95] �΂�14��ԑO�i�R�}���h
	NGO15,		// [ 96] �΂�15��ԑO�i�R�}���h
	NGO16,		// [ 97] �΂�16��ԑO�i�R�}���h
	NGO17,		// [ 98] �΂�17��ԑO�i�R�}���h
	NGO18,		// [ 99] �΂�18��ԑO�i�R�}���h
	NGO19,		// [100] �΂�19��ԑO�i�R�}���h
	NGO20,		// [101] �΂�20��ԑO�i�R�}���h
	NGO21,		// [102] �΂�21��ԑO�i�R�}���h
	NGO22,		// [103] �΂�22��ԑO�i�R�}���h
	NGO23,		// [104] �΂�23��ԑO�i�R�}���h
	NGO24,		// [105] �΂�24��ԑO�i�R�}���h
	NGO25,		// [106] �΂�25��ԑO�i�R�}���h
	NGO26,		// [107] �΂�26��ԑO�i�R�}���h
	NGO27,		// [108] �΂�27��ԑO�i�R�}���h
	NGO28,		// [109] �΂�28��ԑO�i�R�}���h
	NGO29,		// [110] �΂�29��ԑO�i�R�}���h
	NGO30,		// [111] �΂�30��ԑO�i�R�}���h
	NGO31,		// [112] �΂�31��ԑO�i�R�}���h
	NGO32,		// [113] �΂�32��ԑO�i�R�}���h
	NGO33,		// [114] �΂�33��ԑO�i�R�}���h
	NGO34,		// [115] �΂�34��ԑO�i�R�}���h
	NGO35,		// [116] �΂�35��ԑO�i�R�}���h
	NGO36,		// [117] �΂�36��ԑO�i�R�}���h
	NGO37,		// [118] �΂�37��ԑO�i�R�}���h
	NGO38,		// [119] �΂�38��ԑO�i�R�}���h
	NGO39,		// [120] �΂�39��ԑO�i�R�}���h
	NGO40,		// [121] �΂�40��ԑO�i�R�}���h
	NGO41,		// [122] �΂�41��ԑO�i�R�}���h
	NGO42,		// [123] �΂�42��ԑO�i�R�}���h
	NGO43,		// [124] �΂�43��ԑO�i�R�}���h
	NGO44,		// [125] �΂�44��ԑO�i�R�}���h
	NGO45,		// [126] �΂�45��ԑO�i�R�}���h
	NGO46,		// [127] �΂�46��ԑO�i�R�}���h
	NGO47,		// [128] �΂�47��ԑO�i�R�}���h
	NGO48,		// [129] �΂�48��ԑO�i�R�}���h
	NGO49,		// [130] �΂�49��ԑO�i�R�}���h
	NGO50,		// [131] �΂�50��ԑO�i�R�}���h
	NGO51,		// [132] �΂�51��ԑO�i�R�}���h
	NGO52,		// [133] �΂�52��ԑO�i�R�}���h
	NGO53,		// [134] �΂�53��ԑO�i�R�}���h
	NGO54,		// [135] �΂�54��ԑO�i�R�}���h
	NGO55,		// [136] �΂�55��ԑO�i�R�}���h
	NGO56,		// [137] �΂�56��ԑO�i�R�}���h
	NGO57,		// [138] �΂�57��ԑO�i�R�}���h
	NGO58,		// [139] �΂�58��ԑO�i�R�}���h
	NGO59,		// [140] �΂�59��ԑO�i�R�}���h
	NGO60,		// [141] �΂�60��ԑO�i�R�}���h
	NGO61,		// [142] �΂�61��ԑO�i�R�}���h
	NGO62,		// [143] �΂�62��ԑO�i�R�}���h
	NGO63,		// [144] �΂�63��ԑO�i�R�}���h
	NGO64,		// [145] �΂�64��ԑO�i�R�}���h
	NGO65,		// [146] �΂�65��ԑO�i�R�}���h
	NGO66,		// [147] �΂�66��ԑO�i�R�}���h
	NGO67,		// [148] �΂�67��ԑO�i�R�}���h
	NGO68,		// [149] �΂�68��ԑO�i�R�}���h
	NGO69,		// [150] �΂�69��ԑO�i�R�}���h
	NGO70,		// [151] �΂�70��ԑO�i�R�}���h
	NGO71,		// [152] �΂�71��ԑO�i�R�}���h

	RS45N,		// [153] ���i -> �E45   -> �΂�
	LS45N,		// [154] ���i -> ��45   -> �΂�
	RS135N,		// [155] ���i -> �E135  -> �΂�
	LS135N,		// [156] ���i -> ��135  -> �΂�
	RN45S,		// [157] �΂� -> �E45   -> ���i
	LN45S,		// [158] �΂� -> ��45   -> ���i
	RN135S,		// [159] �΂� -> �E135  -> ���i
	LN135S,		// [160] �΂� -> ��135  -> ���i
	RN90N,		// [161] �΂� -> �E90   -> �΂�
	LN90N,		// [162] �΂� -> ��90   -> �΂�

	CEND = 250,	// [250] �R�}���h�I��

	MAP_CMD_MAX,
}enMAP_CMD;

typedef struct{
	enMAP_CMD	en_cmd;			// �R�}���h
	float		f_x0_x1;		// [0]/[1]��X���W���Z�l
	float		f_y0_y1;		// [0]/[1]��y���W���Z�l
	float		f_x2_x3;		// [2]/[3]��X���W���Z�l
	float		f_y2_y3;		// [2]/[3]��y���W���Z�l
	float		f_x4_x5;		// [4]/[5]��X���W���Z�l
	float		f_y4_y5;		// [4]/[5]��y���W���Z�l
	float		f_x6_x7;		// [6]/[7]��X���W���Z�l
	float		f_y6_y7;		// [6]/[7]��y���W���Z�l
	int16_t		s_dir;			// �i�s�����i[0]�k [1]�k�� [2]�� [3]�쓌 [4]�� [5]�쐼 [6]�� [7]�k�� �j
}stMAP_SIM;

/* �R�}���h�ɉ��������W�X�V�f�[�^ */
const stMAP_SIM st_PosData[] = {
	
	//	�R�}���h	[0]/[1]��X	[0]/[1]��Y	[2]/[3]��X	[2]/[3]��Y	[4]/[5]��X	[4]/[5]��Y	[6]/[7]��X	[6]/[7]��Y	����
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
	uint8_t uc_index = 0;			// �e�[�u���̃C���f�b�N�X�ԍ�
	
	/* ------------------------------------------ */
	/*  �R�}���h����e�[�u���̃C���f�b�N�X���擾  */
	/* ------------------------------------------ */
	/* ���i */
	if( ( uc_cmd <=  GO71 ) && ( uc_cmd >=  GO1) ){
		
		uc_index = 14;		// st_PosData�e�[�u���̒��i�̃C���f�b�N�X�ԍ�
	}
	/* �΂ߒ��i */
	else if( ( uc_cmd <=  NGO71 ) && ( uc_cmd >=  NGO1) ){
		
		uc_index = 15;		// st_PosData�e�[�u���̎΂ߒ��i�̃C���f�b�N�X�ԍ�
	}
	/* ���̑��̃R�}���h */
	else{
		while(1){
			
			if( st_PosData[uc_index].en_cmd == uc_cmd )      break;			// �R�}���h����
			if( st_PosData[uc_index].en_cmd == MAP_CMD_MAX ) return;		// �R�}���h������
			uc_index++;
		}
	}
	
	/* �ʒu�X�V */
	switch( s_PosDir ){
		
		/* [0]�k [1]�k�� */
		case 0:
		case 1:
		
			/* ���i */
			if( uc_index == 14 ){
				
				f_PosX += st_PosData[uc_index].f_x0_x1 * uc_cmd;
				f_PosY += st_PosData[uc_index].f_y0_y1 * uc_cmd;
			}
			/* �΂ߒ��i */
			else if( uc_index == 15 ){
				
				f_PosX += st_PosData[uc_index].f_x0_x1 * ( uc_cmd - 81 );
				f_PosY += st_PosData[uc_index].f_y0_y1 * ( uc_cmd - 81 );
			}
			/* ���̑��̃J�[�u */
			else{
				f_PosX += st_PosData[uc_index].f_x0_x1;
				f_PosY += st_PosData[uc_index].f_y0_y1;
			}
			break;
		
		/* [2]�� [3]�쓌 */
		case 2:
		case 3:

			/* ���i */
			if( uc_index == 14 ){
				
				f_PosX += st_PosData[uc_index].f_x2_x3 * uc_cmd;
				f_PosY += st_PosData[uc_index].f_y2_y3 * uc_cmd;
			}
			/* �΂ߒ��i */
			else if( uc_index == 15 ){
				
				f_PosX += st_PosData[uc_index].f_x2_x3 * ( uc_cmd - 81 );
				f_PosY += st_PosData[uc_index].f_y2_y3 * ( uc_cmd - 81 );
			}
			/* ���̑��̃J�[�u */
			else{
				f_PosX += st_PosData[uc_index].f_x2_x3;
				f_PosY += st_PosData[uc_index].f_y2_y3;
			}
			break;

		/* [4]�� [5]�쐼 */
		case 4:
		case 5:

			/* ���i */
			if( uc_index == 14 ){
				
				f_PosX += st_PosData[uc_index].f_x4_x5 * uc_cmd;
				f_PosY += st_PosData[uc_index].f_y4_y5 * uc_cmd;
			}
			/* �΂ߒ��i */
			else if( uc_index == 15 ){
				
				f_PosX += st_PosData[uc_index].f_x4_x5 * ( uc_cmd - 81 );
				f_PosY += st_PosData[uc_index].f_y4_y5 * ( uc_cmd - 81 );
			}
			/* ���̑��̃J�[�u */
			else{
				f_PosX += st_PosData[uc_index].f_x4_x5;
				f_PosY += st_PosData[uc_index].f_y4_y5;
			}
			break;

		/* [6]�� [7]�k�� */
		case 6:
		case 7:

			/* ���i */
			if( uc_index == 14 ){
				
				f_PosX += st_PosData[uc_index].f_x6_x7 * uc_cmd;
				f_PosY += st_PosData[uc_index].f_y6_y7 * uc_cmd;
			}
			/* �΂ߒ��i */
			else if( uc_index == 15 ){
				
				f_PosX += st_PosData[uc_index].f_x6_x7 * ( uc_cmd - 81 );
				f_PosY += st_PosData[uc_index].f_y6_y7 * ( uc_cmd - 81 );
			}
			/* ���̑��̃J�[�u */
			else{
				f_PosX += st_PosData[uc_index].f_x6_x7;
				f_PosY += st_PosData[uc_index].f_y6_y7;
			}
			break;
	}
	
	/* �i�s�����X�V */
	s_PosDir += st_PosData[uc_index].s_dir;
	if( s_PosDir < 0 ) s_PosDir += 8;				// [0]�`[7]�ɂ�����
	else if( s_PosDir > 7 ) s_PosDir -= 8;
	
	f_LogPosX[us_LogIndex] = f_PosX;
	f_LogPosY[us_LogIndex] = f_PosY;
	
	us_LogIndex++;
	us_LogIndex %= 30;
}

bool MAP_setWallCut( uint8_t uc_cmd )
{
	uint8_t uc_val = 0;			// 1���O�̃R�[�i�[���̕ǂ����邩�i0�ȊO�Ȃ�ǂ���j
	uint8_t uc_valPrev = 0;		// 2���O�̃R�[�i�[���̕ǂ����邩�i0�ȊO�Ȃ�ǂ���j
	bool bl_wallCut = FALSE;
	
	/* �ʒu�X�V */
	switch( uc_cmd ){
		
		case R90S:
		case RS135N:
			
			/* 1���O�̃R�[�i�[���̕ǂ����邩�i0�ȊO�Ȃ炠��j */
			// s_PosDir�F�i�s�����i[0]�k [1]�k�� [2]�� [3]�쓌 [4]�� [5]�쐼 [6]�� [7]�k�� �j
			switch( s_PosDir ){
				
				/* ����Ő��񂷂�̂ŁA������O���ǂ̗L���𒲂ׂ������W�ƂȂ�i���ӁFg_sysMap��2�����z��ł��j */
				case 0: 
					if( 0 < f_PosY-0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY-0.5)][(uint8_t)(f_PosX)] & 0x02;		// �k�������Ă���̂œ����̕ǂ����邩
					if( 0 < f_PosY-1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY-1.5)][(uint8_t)(f_PosX)] & 0x02;		// �k�������Ă���̂œ����̕ǂ����邩
					break;	
				case 2: 
					if( 0 < f_PosX-0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX-0.5)] & 0x04;		// ���������Ă���̂œ쑤�̕ǂ����邩
					if( 0 < f_PosX-1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX-1.5)] & 0x04;		// ���������Ă���̂œ쑤�̕ǂ����邩
					break;
				case 4: 
					if( MAP_Y_SIZE_REAL > f_PosY+0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY+0.5)][(uint8_t)(f_PosX)] & 0x08;		// ��������Ă���̂Ő����̕ǂ����邩
					if( MAP_Y_SIZE_REAL > f_PosY+1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY+1.5)][(uint8_t)(f_PosX)] & 0x08;		// ��������Ă���̂Ő����̕ǂ����邩
					break;
				case 6:
					if( MAP_X_SIZE_REAL > f_PosX+0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX+0.5)] & 0x01;		// ���������Ă���̂Ŗk���̕ǂ����邩
					if( MAP_X_SIZE_REAL > f_PosX+1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX+1.5)] & 0x01;		// ���������Ă���̂Ŗk���̕ǂ����邩
					break;
			}
			/* �ǂ����邽�ߕǐ؂�␳���s�� */
			if( ( uc_val != 0 ) || ( ( uc_val != 0 ) && ( uc_valPrev != 0 ) ) ){
				
				MOT_setWallEdgeType( MOT_WALL_EDGE_RIGHT );		// �ǐ؂�␳�����{����
				bl_wallCut = TRUE;
			}
			break;
			
		case L90S:
		case LS135N:
			/* 1���O�̃R�[�i�[���̕ǂ����邩�i0�ȊO�Ȃ炠��j */
			// s_PosDir�F�i�s�����i[0]�k [1]�k�� [2]�� [3]�쓌 [4]�� [5]�쐼 [6]�� [7]�k�� �j
			switch( s_PosDir ){
				
				/* ����Ő��񂷂�̂ŁA������O���ǂ̗L���𒲂ׂ������W�ƂȂ�i���ӁFg_sysMap��2�����z��ł��j */
				case 0: 
					if( 0 < f_PosY-0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY-0.5)][(uint8_t)(f_PosX)] & 0x08;			// �k�������Ă���̂Ő����̕ǂ����邩
					if( 0 < f_PosY-1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY-1.5)][(uint8_t)(f_PosX)] & 0x08;			// �k�������Ă���̂Ő����̕ǂ����邩
					break;
				case 2: 
					if( 0 < f_PosX-0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX-0.5)] & 0x01;			// ���������Ă���̂Ŗk���̕ǂ����邩
					if( 0 < f_PosX-1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX-1.5)] & 0x01;			// ���������Ă���̂Ŗk���̕ǂ����邩
					break;
				case 4: 
					if( MAP_Y_SIZE_REAL > f_PosY+0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY+0.5)][(uint8_t)(f_PosX)] & 0x02;			// ��������Ă���̂œ����̕ǂ����邩
					if( MAP_Y_SIZE_REAL > f_PosY+1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY+1.5)][(uint8_t)(f_PosX)] & 0x02;			// ��������Ă���̂œ����̕ǂ����邩
					break;
				case 6: 
					if( MAP_X_SIZE_REAL > f_PosX+0.5 ) uc_val     = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX+0.5)] & 0x04;			// ���������Ă���̂œ쑤�̕ǂ����邩
					if( MAP_X_SIZE_REAL > f_PosX+1.5 ) uc_valPrev = g_sysMap[(uint8_t)(f_PosY)][(uint8_t)(f_PosX+1.5)] & 0x04;			// ���������Ă���̂œ쑤�̕ǂ����邩
					break;
			}
			/* �ǂ����邽�ߕǐ؂�␳���s�� */
			if( ( uc_val != 0 ) || ( ( uc_val != 0 ) && ( uc_valPrev != 0 ) ) ){
				
				MOT_setWallEdgeType( MOT_WALL_EDGE_LEFT );		// �ǐ؂�␳�����{����
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
	s_PosDir = (int16_t)(en_dir * 2);	// �i�s�����i[0]�k [1]�k�� [2]�� [3]�쓌 [4]�� [5]�쐼 [6]�� [7]�k�� �j�A2�{����ƒ��x�l�����v����
}

void MAP_showCmdLog( void )
{
	uint16_t i=0;
	
	/* ���M�n����R�}���h */
	while(1){
		
		printf("dcom[%4d] = %02d  \n\r",i,dcom[i]);
		if( dcom[i] == CEND ) break;
		i++;
	}
	i=0;
	
	/* �X�����[���R�}���h */
	while(1){
		
		printf("scom[%4d] = %02d  \n\r",i,scom[i]);
		if( scom[i] == CEND ) break;
		i++;
	}
	i=0;

	/* �΂ߑ��s�R�}���h */
	while(1){
		
		printf("tcom[%4d] = %02d  \n\r",i,tcom[i]);
		if( tcom[i] == CEND ) break;
		i++;
	}
}

void MAP_makeCmdList( 
	uint8_t uc_staX,					///< [in] �J�nX���W
	uint8_t uc_staY,					///< [in] �J�nY���W
	enMAP_HEAD_DIR en_staDir,		///< [in] �J�n���̕���
	uint8_t uc_endX,					///< [in] �I��X���W
	uint8_t uc_endY,					///< [in] �I��Y���W
	enMAP_HEAD_DIR* en_endDir		///< [out] �I�����̕���
){
	uint8_t			uc_goStep;									// �O�i�̃X�e�b�v��
	uint16_t			us_high;									// �������̍���
	uint16_t			us_pt;										// �R�}���h�|�C���^
	enMAP_HEAD_DIR	en_nowDir;									// ���݃}�E�X�̌����Ă����Ε���
	enMAP_HEAD_DIR	en_tempDir;									// ���Ε���
//	uint16_t			i;											// roop
	
	/* �O�i�X�e�b�v�������������� */
	uc_goStep = 0;
	us_pt = 0;

	/* ���H��񂩂�R�}���h�쐬 */
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
		
		en_tempDir = (enMAP_HEAD_DIR)( (en_nowDir - en_staDir) & (enMAP_HEAD_DIR)3 );		// �����X�V
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
	
	/* ���n�M����p�̃R�}���h���X�g�쐬 */
	dcom[us_pt] = uc_goStep;
	dcom[++us_pt] = STOP;
	dcom[++us_pt] = CEND;
	us_totalCmd = us_pt+1;			// �R�}���h����


	/* �ŏI�I�Ɍ����Ă������ */
	*en_endDir = en_staDir;
}

void MAP_makeSuraCmdList( void )
{
	uint16_t dcom_temp[4096];			// ����撴�M����R�}���h���X�g
	uint16_t i=0,j=0;					// roop
	
	/* ���n�M����R�}���h���R�s�[ */
	for( i=0; i<us_totalCmd; i++ ){
		dcom_temp[i] = dcom[i];
	}

	i = 0;

	/* �z�񂪐���R�}���h�����`�F�b�N */
	while(1)
	{
		if( dcom_temp[i] == R90 ){		// �E90��
			dcom_temp[i-1] -= 1;		// 1��O������
			dcom_temp[i+1] -= 1;		// 1��O������
			dcom_temp[i] = R90S;		// �E�X�����[��90��
		}
		else if( dcom_temp[i] == L90 ){	// ��90��
			dcom_temp[i-1] -= 1;		// 1��O������
			dcom_temp[i+1] -= 1;		// 1��O������
			dcom_temp[i] = L90S;		// ���X�����[��90��
		}
		else{
			if( dcom_temp[i] == CEND ){
				break;
			}
		}
		i++;
	}

	i = j = 0;

	/* �X�����[���R�}���h�ϊ� */
	while(1)
	{
		if( dcom_temp[i+1] == CEND ){
			scom[j] = STOP;
			scom[j+1] = CEND;
			break;
		}
		else
		{
			/* �f�[�^���X�g�b�v�R�}���h�������� */
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
	uint16_t	scom_temp[4096];			// ����撴�M����R�}���h���X�g
	uint16_t	i;							// roop
	uint16_t	c1, c2, c3, c4;				// �v�Z�p
	uint16_t	x;
	uint16_t	ct_n=0, ct_st=0;
	uint16_t	flag = 3;					//	�΂ߑ��s�p�o�b�t�@  0:�����R�}���h�@1:�΂�  2:S135N �� N135S  3:���i
	
	/* ���n�M����R�}���h���R�s�[ */
	for( i=0; i<us_totalCmd; i++ )
	{
		scom_temp[i] = scom[i];
	}

	i=0;

	/* �z�񂪐���R�}���h�����`�F�b�N */
	while(1)
	{
		c1 = scom_temp[ct_st];
		c2 = scom_temp[ct_st+1];
		c3 = scom_temp[ct_st+2];
		c4 = scom_temp[ct_st+3];

		//	���i �� �E45�x �� �΂�
		if( (c1<=GO32) && (c2==R90S) && (c3==L90S) )
		{
			if((ct_st == 0) && (c1-1 == 0)){
				tcom[ ct_n ] = scom_temp[ct_st];
				ct_st ++;
				ct_n ++;
			}
			else{
				if( c1-1 != 0 ) tcom[ ct_n++ ] = c1 - 1;		//	�O�̕����R�}���h�ɂ���Ē�����Ԃ������Ȃ��ꍇ
				tcom[ ct_n++ ] = RS45N;
				ct_st ++;

				x = (uint16_t)(NGO1 - 1);		//	�΂߃��[�h
				flag = 0;
			}
		}
		//	���i �� ��45�x �� �΂�
		else if( (c1<=GO32) && (c2==L90S) && (c3==R90S) )
		{
			if((ct_st == 0) && (c1-1 == 0)){
				tcom[ ct_n ] = scom_temp[ct_st];
				ct_st ++;
				ct_n ++;
			}
			else{
				if( c1-1 != 0 ) tcom[ ct_n++ ] = c1 - 1;		//	�O�̕����R�}���h�ɂ���Ē�����Ԃ������Ȃ��ꍇ
				tcom[ ct_n++ ] = LS45N;
				ct_st ++;

				x = (uint16_t)(NGO1 - 1);		//	�΂߃��[�h
				flag = 0;
			}
		}

		//	���i �� �E90�x �� ���i
		else if( (c1<=GO32) && (c2==R90S) && (c3<=GO32) )
		{
			tcom[ ct_n++ ] = c1;
			tcom[ ct_n++ ] = R90S;
			ct_st += 2;
			flag = 3;		//	���i
		}
		//	���i �� ��90�x �� ���i
		else if( (c1<=GO32) && (c2==L90S) && (c3<=GO32) )
		{
			tcom[ ct_n++ ] = c1;
			tcom[ ct_n++ ] = L90S;
			ct_st += 2;
			flag = 3;		//	���i
		}
		//	���i �� �E135�x �� �΂�
		else if( (c1<=GO32) && (c2==R90S) && (c3==R90S) && (c4==L90S) )
		{
			tcom[ ct_n++ ] = c1;
			tcom[ ct_n++ ] = RS135N;
			ct_st += 2;

			x = (uint16_t)(NGO1 - 1);		//	�΂߃��[�h
			flag = 2;
		}
		//	���i �� ��135�x �� �΂�
		else if( (c1<=GO32) && (c2==L90S) && (c3==L90S) && (c4==R90S) )
		{
			tcom[ ct_n++ ] = c1;
			tcom[ ct_n++ ] = LS135N;
			ct_st += 2;

			x = (uint16_t)(NGO1 - 1);		//	�΂߃��[�h
			flag = 2;
		}

		//	���i �� �E180�x �� ���i
		else if( (c1<=GO32) && (c2==R90S) && (c3==R90S) && (c4<=GO32) )
		{
			tcom[ ct_n++ ] = c1;
			tcom[ ct_n++ ] = R90S;
			tcom[ ct_n++ ] = R90S;
			ct_st += 3;
			flag = 3;		//	���i
		}
		//	���i �� ��180�x �� ���i
		else if( (c1<=GO32) && (c2==L90S) && (c2==L90S) && (c4<=GO32) )
		{
			tcom[ ct_n++ ] = c1;
			tcom[ ct_n++ ] = L90S;
			tcom[ ct_n++ ] = L90S;
			ct_st += 3;
			flag = 3;		//	���i
		}

		//	�΂� �� �E45�x �� ���i
		else if( (c1==R90S) && (c2<=GO32)  && (flag != 3 ) )
		{
			if( flag==1 ) tcom[ ct_n++ ] = x;
			tcom[ ct_n++ ] = RN45S;
			scom_temp[ct_st+1] = c2 - 1;		//	������Ԃ�1���炷
			ct_st ++;
			flag = 3;		//	���i
		}
		//	�΂� �� ��45�x �� ���i
		else if( (c1==L90S) && (c2<=GO32)  && (flag != 3 ) )
		{
			if( flag==1 ) tcom[ ct_n++ ] = x;
			tcom[ ct_n++ ] = LN45S;
			scom_temp[ct_st+1] = c2 - 1;		//	������Ԃ�1���炷
			ct_st ++;
			flag = 3;		//	���i
		}
		//	�΂� �� �E90�x �� �΂�
		else if( (c1==L90S) && (c2==R90S) && (c3==R90S) && (c4==L90S)  && (flag != 3 ) )
		{
			if( flag==0 ) tcom[ ct_n++ ] = NGO1;		//	45N����RN90N
			else if( flag==1 ) tcom[ ct_n++ ] = x+1;
			else if( flag==2 ) tcom[ ct_n++ ] = NGO1;
			tcom[ ct_n++ ] = RN90N;
			ct_st +=2;

			x = (uint16_t)(NGO1 - 1);		//	�΂߃��[�h
			flag = 1;
		}
		//	�΂� �� ��90�x �� �΂�
		else if( (c1==R90S) && (c2==L90S) && (c3==L90S) && (c4==R90S)  && (flag != 3 ) )
		{
			if( flag==0 ) tcom[ ct_n++ ] = NGO1;		//	45N����LN90N
			else if( flag==1 ) tcom[ ct_n++ ] = x+1;
			else if( flag==2 ) tcom[ ct_n++ ] = NGO1;
			tcom[ ct_n++ ] = LN90N;
			ct_st +=2;

			x = (uint16_t)(NGO1 - 1);		//	�΂߃��[�h
			flag = 1;
		}
		//	�΂� �� �E135�x �� ���i
		else if( (c1==L90S) && (c2==R90S) && (c3==R90S) && (c4<=GO32)  && (flag != 3 ) )
		{
			if( flag==0 ) tcom[ ct_n++ ] = NGO1;		//	45N����LN90N
			else if( flag==1 ) tcom[ ct_n++ ] = x+1;
			else if( flag==2 ) tcom[ ct_n++ ] = NGO1;
			tcom[ ct_n++ ] = RN135S;
			ct_st += 3;
			flag = 3;		//	���i
		}
		//	�΂� �� ��135�x �� ���i
		else if( (c1==R90S) && (c2==L90S) && (c3==L90S) && (c4<=GO32)  && (flag != 3 ) )
		{
			if( flag==0 ) tcom[ ct_n++ ] = NGO1;		//	45N����LN90N
			else if( flag==1 ) tcom[ ct_n++ ] = x+1;
			else if( flag==2 ) tcom[ ct_n++ ] = NGO1;
			tcom[ ct_n++ ] = LN135S;
			ct_st += 3;
			flag = 3;		//	���i
		}
		//	�΂� �� �΂�
		else if( (c1==R90S) && (c2==L90S) && ( (c3==R90S) || (c3==L90S) || ( c3<=GO32 ) ) && (flag != 3 ) )
		{
			x++;
			ct_st ++;

			flag = 1;		//	�΂ߑ��s�o�b�t�@����
		}
		else if( (c1==L90S) && (c2==R90S) && ( (c3==L90S) || (c3==R90S) || ( c3<=GO32 ) ) && (flag != 3 ) )
		{
			//	�R�}���h�o��
			x++;
			ct_st ++;

			flag = 1;		//	�΂ߑ��s�o�b�t�@����
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
	uint16_t			us_rp = 0;				// ���݂̓ǂݍ��݈ʒu
	enMOT_TURN_CMD 		en_type;
	bool			bl_isWallCut = FALSE;
	
	/* ���M���񃂁[�h*/
	if( en_driveType == MAP_DRIVE_TURN )
	{
		while(1)
		{
			if ( dcom[us_rp] == CEND  ) break;								//	�R�}���h�I��
			
			else if ( dcom[us_rp] == STOP  ){
			 	CTRL_stop();			// �����~
				DCM_brakeMot( DCM_R );		// �u���[�L
				DCM_brakeMot( DCM_L );		// �u���[�L
			}
			else if ( ( dcom[us_rp] <=  GO71 ) && ( dcom[us_rp] >=  GO1) )
			{
				MOT_goBlock_FinSpeed( (float)dcom[us_rp]*0.5f, 0 );		// �������s�R�}���h�A����ԑO�i��ɒ�~
			}
			else{
				
				if( dcom[us_rp] == R90 ) en_type = MOT_R90;
				else 					 en_type = MOT_L90;
				
				LL_mDelay(500);
				MOT_turn( en_type );		//	����
				LL_mDelay(500);
			}
			us_rp++;
			
			/* �r���Ő���s�\�ɂȂ��� */
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot(DCM_R);
				DCM_brakeMot(DCM_L);
				break;
			}
			
		}
	 	CTRL_stop();			// �����~
		DCM_brakeMot( DCM_R );		// �u���[�L
		DCM_brakeMot( DCM_L );		// �u���[�L
	}
	/* �X�����[�����[�h */
	else if( en_driveType == MAP_DRIVE_SURA )
	{
		while(1)
		{
			MAP_refPos( scom[us_rp] );									// ���s�����R�}���h���I�������ʒu�ɍX�V

			if ( scom[us_rp] == CEND  ) break;							//	�R�}���h�I��
			
			else if ( scom[us_rp] == STOP  )
			{
			 	CTRL_stop();			// �����~
				DCM_brakeMot( DCM_R );		// �u���[�L
				DCM_brakeMot( DCM_L );		// �u���[�L
			}
			else if ( ( scom[us_rp] <=  GO71 ) && ( scom[us_rp] >=  GO1) )
			{
				if( scom[us_rp+1] == STOP  ){
					MOT_goBlock_FinSpeed( (float)scom[us_rp]*0.5f, 0 );						// �������s�R�}���h�A����ԑO�i�i�ŏI���x�Ȃ��j
				}
				else{
					
					/* �ǂ̐؂�ڕ␳ */
					if( ( scom[us_rp+1] == R90S )   || ( scom[us_rp+1] == L90S ) ){
						bl_isWallCut = MAP_setWallCut( scom[us_rp+1] );		// �R�[�i�[�O�ɕǂ���������ǂ̐؂�ڕ␳���s���ݒ������
						
						if( bl_isWallCut == TRUE ){
							
							bl_isWallCut = FALSE;
							us_LogWallCut[us_LogIndexWallCut] = us_rp;
							us_LogIndexWallCut++;
							us_LogIndexWallCut %= 30;
						}
					}
					MOT_goBlock_FinSpeed( (float)scom[us_rp]*0.5f, MOT_getSuraStaSpeed() );		// �������s�R�}���h�A����ԑO�i�i�ŏI���x����j
				}
			}
			else if( scom[us_rp] == R90S )
			{
				MOT_goSla( MOT_R90S, PARAM_getSra( SLA_90 ) );	// �E�X�����[��
			}
			else if( scom[us_rp] == L90S )
			{
				MOT_goSla( MOT_L90S, PARAM_getSra( SLA_90 ) );	// ���X�����[��
			}
			us_rp++;
			
			/* �r���Ő���s�\�ɂȂ��� */
			if( SYS_isOutOfCtrl() == TRUE){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// �u���[�L
				DCM_brakeMot( DCM_L );		// �u���[�L
				break;
			}
			
		}
	}
	/* �΂߃��[�h */
	else if( en_driveType == MAP_DRIVE_SKEW )
	{
		while(1)
		{
			MAP_refPos( tcom[us_rp] );									// ���s�����R�}���h���I�������ʒu�ɍX�V
			
			if ( tcom[us_rp] == CEND  ) break;							//	�R�}���h�I��

			else if ( tcom[us_rp] == STOP  )
			{
			 	CTRL_stop();			// �����~
				DCM_brakeMot( DCM_R );		// �u���[�L
				DCM_brakeMot( DCM_L );		// �u���[�L
			}
			else if ( ( tcom[us_rp] <=  GO71 ) && ( tcom[us_rp] >=  GO1) )
			{
				if( tcom[us_rp+1] == STOP  ){
					MOT_goBlock_FinSpeed( (float)tcom[us_rp]*0.5f, 0 );						// �������s�R�}���h�A����ԑO�i�i�ŏI���x�Ȃ��j
				}
				else{
					
					/* �ǂ̐؂�ڕ␳ */
					if( ( tcom[us_rp+1] == R90S )   || ( tcom[us_rp+1] == L90S )   || 
					 	( tcom[us_rp+1] == RS135N ) || ( tcom[us_rp+1] == LS135N ) 
					 ){
						bl_isWallCut = MAP_setWallCut( tcom[us_rp+1] );		// �R�[�i�[�O�ɕǂ���������ǂ̐؂�ڕ␳���s���ݒ������
						
						if( bl_isWallCut == TRUE ){
							
							bl_isWallCut = FALSE;
							us_LogWallCut[us_LogIndexWallCut] = us_rp;
							us_LogIndexWallCut++;
							us_LogIndexWallCut %= 30;
						}
					}
					MOT_goBlock_FinSpeed( (float)tcom[us_rp]*0.5f, MOT_getSuraStaSpeed() );		// �������s�R�}���h�A����ԑO�i�i�ŏI���x����j
				}
			}
			else if ( ( tcom[us_rp] <=  NGO71 ) && ( tcom[us_rp] >=  NGO1) )
			{
				MOT_goSkewBlock_FinSpeed( (float)(tcom[us_rp]-81)*0.5f, MOT_getSuraStaSpeed());	// �΂ߒ������s�R�}���h�A����ԑO�i�i�ŏI���x����j
			}
			else
			{
				switch( tcom[us_rp] )
				{

					/* ���i �� ���i */
					case R90S:		MOT_goSla( MOT_R90S, PARAM_getSra( SLA_90 ) );			break;
					case L90S:		MOT_goSla( MOT_L90S, PARAM_getSra( SLA_90 ) );			break;
					
					/* ���i �� �΂� */
					case RS45N:		MOT_goSla( MOT_R45S_S2N, PARAM_getSra( SLA_45 ) ); 		break;
					case LS45N:		MOT_goSla( MOT_L45S_S2N, PARAM_getSra( SLA_45 ) ); 		break;
					case RS135N:	MOT_goSla( MOT_R135S_S2N, PARAM_getSra( SLA_135 ) ); 	break;
					case LS135N:	MOT_goSla( MOT_L135S_S2N, PARAM_getSra( SLA_135 ) ); 	break;

					/* �΂� �� ���i */
					case RN45S:		MOT_goSla( MOT_R45S_N2S, PARAM_getSra( SLA_45 ) ); 		break;
					case LN45S:		MOT_goSla( MOT_L45S_N2S, PARAM_getSra( SLA_45 ) ); 		break;
					case RN135S:	MOT_goSla( MOT_R135S_N2S, PARAM_getSra( SLA_135 ) ); 	break;
					case LN135S:	MOT_goSla( MOT_L135S_N2S, PARAM_getSra( SLA_135 ) ); 	break;

					/* �΂� �� �΂� */
					case RN90N:		MOT_goSla( MOT_R90S_N, PARAM_getSra( SLA_N90 ) ); 		break;
					case LN90N:		MOT_goSla( MOT_L90S_N, PARAM_getSra( SLA_N90 ) );		break;
				}
			}
			us_rp++;
			
			/* �r���Ő���s�\�ɂȂ��� */
			if( SYS_isOutOfCtrl() == TRUE ){
				CTRL_stop();
				DCM_brakeMot( DCM_R );		// �u���[�L
				DCM_brakeMot( DCM_L );		// �u���[�L
				break;
			}
		}
	}
}

void MAP_searchCmdList(
	uint8_t uc_staX,					///< [in] �J�nX���W
	uint8_t uc_staY,					///< [in] �J�nY���W
	enMAP_HEAD_DIR en_staDir,		///< [in] �J�n���̕���
	uint8_t uc_endX,					///< [in] �I��X���W
	uint8_t uc_endY,					///< [in] �I��Y���W
	enMAP_HEAD_DIR* en_endDir		///< [out] �I�����̕���
) {
	uint8_t			uc_goStep;									// �O�i�̃X�e�b�v��
	uint16_t			us_high;									// �������̍���
	uint16_t			us_pt;										// �R�}���h�|�C���^
	enMAP_HEAD_DIR	en_nowDir;									// ���݃}�E�X�̌����Ă����Ε���
	enMAP_HEAD_DIR	en_tempDir;									// ���Ε���
//	uint16_t			i;											// roop

	/* �O�i�X�e�b�v�������������� */
	uc_goStep = 0;
	us_pt = 0;
//	printf("mx%d,my%d\n", uc_staX, uc_staY);
	/* ���H��񂩂�R�}���h�쐬 */
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

		en_tempDir = (enMAP_HEAD_DIR)((en_nowDir - en_staDir) & (enMAP_HEAD_DIR)3);		// �����X�V
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


	/* �ŏI�I�Ɍ����Ă������ */
	*en_endDir = en_staDir;
}
