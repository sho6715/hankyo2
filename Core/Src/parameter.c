/*
 * parameter.c
 *
 *  Created on: 2021/02/04
 *      Author: shohe
 */

#include "parameter.h"


#define GET_INDEX_ST(i)			( i - PARAM_ST_TOP - 1 )		// ���i�p�̃C���f�b�N�X���擾
#define GET_INDEX_TRUN(i)		( i - PARAM_TRUN_TOP - 1 )		// ����p�̃C���f�b�N�X���擾
#define GET_INDEX_SLA(i)		( i - PARAM_SLA_TOP - 1 )		// �X�����[���p�̃C���f�b�N�X���擾

enPARAM_MOVE_SPEED		en_Speed_st	= PARAM_NORMAL;			// ���i���̈ړ����x�^�C�v
enPARAM_MOVE_SPEED		en_Speed_trun	= PARAM_NORMAL;				// ���񎞂̈ړ����x�^�C�v
enPARAM_MOVE_SPEED		en_Speed_sla	= PARAM_NORMAL;				// �X�����[�����̈ړ����x�^�C�v
stSLA				st_Sla[SLA_TYPE_MAX];					// �X�����[�����̑��s�p�����[�^


/* ============ */
/*  ���x�f�[�^  */
/* ============ */

	/* ���i���x�f�[�^ */
	const stSPEED f_StSpeedData[PARAM_MOVE_SPEED_MAX] = {
		
		//	�����x		�����x		�p�����x		�p�����x
		{ 800,			1000,		0,				0,				},		// ���ᑬ(PARAM_VERY_SLOW)
		{ 1800,			1800,		0,				0,				},		// �ᑬ(PARAM_SLOW)
		{ 1800,			1800,		0,				0,				},		// �ʏ�(PARAM_NORMAL)
		{ 2500,			2500,		0,				0,				},		// ����(PARAM_FAST)
		{ 4000,			4000,		0,				0,				},		// ������(PARAM_VERY_FAST)
	};

	/* ���񑬓x�f�[�^ */
	const stSPEED f_TurnSpeedData[PARAM_MOVE_SPEED_MAX] = {
		
		//	�����x		�����x		�p�����x		�p�����x
		{ 0,			0,			4000,			4000,			},		// ���ᑬ(PARAM_VERY_SLOW)
		{ 0,			0,			4000,			4000,			},		// �ᑬ(PARAM_SLOW)
		{ 0,			0,			4000,			4000,			},		// �ʏ�(PARAM_NORMAL)
		{ 0,			0,			4000,			4000,			},		// ����(PARAM_FAST)
		{ 0,			0,			4000,			4000,			},		// ������(PARAM_VERY_FAST)
	};

	/* �X�����[�����x�f�[�^ */
	const stSPEED f_SlaSpeedData[PARAM_MOVE_SPEED_MAX] = {
		
		//	�����x		�����x		�p�����x		�p�����x
		{ 1800,			1800,		1800,			1800,			},		// ���ᑬ(PARAM_VERY_SLOW)
		{ 1800,			1800,		1800,			1800,			},		// �ᑬ(PARAM_SLOW)
		{ 1800,			1800,		1800,			1800,			},		// �ʏ�(PARAM_NORMAL)
		{ 2500,			2500,		1800,			1800,			},		// ����(PARAM_FAST)
		{ 4000,			4000,		1800,			1800,			},		// ������(PARAM_VERY_FAST)
	};


/* ============== */
/*  �Q�C���f�[�^  */
/* ============== */
	// �y�A�h�o�C�X�z 
	//    �������Q�C���̃p�����[�^���𑝂₵�����ꍇ�́AstGAIN�̃����o�Ɓ��̃f�[�^�𑝂₷������OK�ł��B
	//    PARAM_getGain()�Ńp�����[�^�̃A�h���X���擾���āA�ǉ����������o���Q�Ƃ��ĉ������B

	/* ���i�Q�C���f�[�^ */
	const stGAIN f_StGainData[PARAM_MOVE_SPEED_MAX][PARAM_ST_MAX] = {
		
		/* ���ᑬ(PARAM_VERY_SLOW) */
		{	//	FFSA	FFS		FFAA	FFA		���xkp	���xki	���xkd	�ʒukp	�ʒuki	�p���xkp	�p���xki	�p���xkd	�p�xkp	�p�xki	��kp	��kd
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		0,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_ACC
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		0,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_CONST
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		6,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_DEC
//			{ 0.0242f*4,3,	0,	0,		0,	0,	0.6,		0,		0,		3.5,		0,		1,		0,	},	// PARAM_BACK_ACC
//			{ 0,		3,	0,	0,		0,	0,	0.6,		0,		0,		3.5,		0,		1,		0,	},	// PARAM_BACK_CONST
//			{ 0,		3,	0,	0,		0,	0.1,	0.5,		0,		0,		3,		0,		1,		0,	},	// PARAM_BACK_DEC
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		0,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_SKEW_ACC
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		0,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_SKEW_CONST
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		6,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_SKEW_DEC
			{ 0.05,		0,		0,		0,		0,	0,		0,		0,		0,		0,			0,			0,		0,		0,		0,			0,		},	// PARAM_HIT_WALL
		},
		/* �ᑬ(PARAM_SLOW) */
		{	//	FFSA	FFS		FFAA	FFA		���xkp		���xki	���xkd	�ʒukp	�ʒuki	�p���xkp	�p���xki	�p���xkd	�p�xkp	�p�xki	��kp	��kd
			{ 0.01,		0.2,	0,		0,		0.5,		0.1,		0,		0,		0,		0.8,		0.05,		0,		0.5,		0.5,		0.3,	0.03,	},	// PARAM_ACC
			{ 0.01,		0.2,	0,		0,		0.5,		0.1,		0,		0,		0,		0.8,		0.05,		0,		0.5,		0.5,		0.3,	0.03,	},	// PARAM_CONST
			{ 0.01,		0.2,	0,		0,		0.5,		0.1,		0,		5,		0.1,	0.8,		0.05,		0,		0.5,		0.5,		0.3,	0.03,	},	// PARAM_DEC
//			{ 0.0242f*4,3,	0,	0,		0,	0,	0.6,		0,		0,		3.5,		0,		1,		0,	},	// PARAM_BACK_ACC
//			{ 0,		3,	0,	0,		0,	0,	0.6,		0,		0,		3.5,		0,		1,		0,	},	// PARAM_BACK_CONST
//			{ 0,		3,	0,	0,		0,	0.1,	0.5,		0,		0,		3,		0,		1,		0,	},	// PARAM_BACK_DEC
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		0,		0,		0.8,		0.05,		0,		0.5,		0.5,		0.3,	0.03,	},	// PARAM_SKEW_ACC
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		0,		0,		0.8,		0.05,		0,		0.5,		0.5,		0.3,	0.03,	},	// PARAM_SKEW_CONST
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		5,		0.1,	0.8,		0.05,		0,		0.5,		0.5,		0.3,	0.03,	},	// PARAM_SKEW_DEC
			{ 0.07,		0,		0,		0,		0,	0,		0,		0,		0,		0,			0,			0,		0,		0,		0,			0,		},	// PARAM_HIT_WALL
		},
		/* �ʏ�(PARAM_NORMAL) */
		{	//	FFSA	FFS		FFAA	FFA		���xkp	���xki	���xkd	�ʒukp	�ʒuki	�p���xkp	�p���xki	�p���xkd	�p�xkp	�p�xki	��kp	��kd
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		0,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_ACC
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		0,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_CONST
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		6,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_DEC
//			{ 0.0242f*4,3,	0,	0,		0,	0,	0.6,		0,		0,		3.5,		0,		1,		0,	},	// PARAM_BACK_ACC
//			{ 0,		3,	0,	0,		0,	0,	0.6,		0,		0,		3.5,		0,		1,		0,	},	// PARAM_BACK_CONST
//			{ 0,		3,	0,	0,		0,	0.1,	0.5,		0,		0,		3,		0,		1,		0,	},	// PARAM_BACK_DEC
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		0,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_SKEW_ACC
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		0,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_SKEW_CONST
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		6,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_SKEW_DEC
			{ 0.05,		0,		0,		0,		0,	0,		0,		0,		0,		0,			0,			0,		0,		0,		0,			0,		},	// PARAM_HIT_WALL
		},
		/* ����(PARAM_FAST) */
		{	//	FFSA	FFS		FFAA	FFA		���xkp	���xki	���xkd	�ʒukp	�ʒuki	�p���xkp	�p���xki	�p���xkd	�p�xkp	�p�xki	��kp	��kd
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		0,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_ACC
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		0,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_CONST
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		6,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_DEC
//			{ 0.0242f*4,3,	0,	0,		0,	0,	0.6,		0,		0,		3.5,		0,		1,		0,	},	// PARAM_BACK_ACC
//			{ 0,		3,	0,	0,		0,	0,	0.6,		0,		0,		3.5,		0,		1,		0,	},	// PARAM_BACK_CONST
//			{ 0,		3,	0,	0,		0,	0.1,	0.5,		0,		0,		3,		0,		1,		0,	},	// PARAM_BACK_DEC
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		0,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_SKEW_ACC
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		0,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_SKEW_CONST
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		6,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_SKEW_DEC
			{ 0.05,		0,		0,		0,		0,	0,		0,		0,		0,		0,			0,			0,		0,		0,		0,			0,		},	// PARAM_HIT_WALL
		},
		/* ������(PARAM_VERY_FAST) */
		{	//	FFSA	FFS		FFAA	FFA		���xkp	���xki	���xkd	�ʒukp	�ʒuki	�p���xkp	�p���xki	�p���xkd	�p�xkp	�p�xki	��kp	��kd
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		0,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_ACC
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		0,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_CONST
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		6,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_DEC
//			{ 0.0242f*4,3,	0,	0,		0,	0,	0.6,		0,		0,		3.5,		0,		1,		0,	},	// PARAM_BACK_ACC
//			{ 0,		3,	0,	0,		0,	0,	0.6,		0,		0,		3.5,		0,		1,		0,	},	// PARAM_BACK_CONST
//			{ 0,		3,	0,	0,		0,	0.1,	0.5,		0,		0,		3,		0,		1,		0,	},	// PARAM_BACK_DEC
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		0,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_SKEW_ACC
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		0,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_SKEW_CONST
			{ 0.01,		0.2,	0,		0,		0.5,		0.03,		0,		6,		0,		0.2,		0.01,		0,		0.5,		0.1,		0.1,	0.03,	},	// PARAM_SKEW_DEC
			{ 0.05,		0,		0,		0,		0,	0,		0,		0,		0,		0,			0,			0,		0,		0,		0,			0,		},	// PARAM_HIT_WALL
		}
	};

	/* ����Q�C���f�[�^ */
	const stGAIN f_TurnGainData[PARAM_MOVE_SPEED_MAX][PARAM_TRUN_MAX] = {
		
		/* ���ᑬ(PARAM_VERY_SLOW) */
		{	//	FFSA	FFS		FFAA	FFA		���xkp	���xki	���xkd	�ʒukp	�ʒuki	�p���xkp	�p���xki	�p���xkd	�p�xkp	�p�xki		��kp	��kd
			{ 0.0,		0,		0.016,	0.22,	3.0,	0.1,	0,		2.0,	0,		0.8,		0.0,		0,			0,		0.1,			0,		0,	},	// PARAM_ACC_TRUN
			{ 0,		0,		0.016,	0.22,	3.0,	0.1,	0,		2.0,	0,		0.8,		0.0,		0,			0,		0.1,			0,		0,	},	// PARAM_CONST_TRUN
			{ 0,		0,		0.016,	0.22,	3.0,	0.1,	0,		2.0,	0,		0.8,		0.0,		0,			5,		0.5,			0,		0,	},	// PARAM_DEC_TRUN
		},
		/* �ᑬ(PARAM_SLOW) */
		{	//	FFSA	FFS		FFAA	FFA		���xkp	���xki	���xkd	�ʒukp	�ʒuki	�p���xkp	�p���xki	�p���xkd	�p�xkp	�p�xki		��kp	��kd
			{ 0.0,		0,		0.016,	0.22,	1.0,	0.1,	0,		2.0,	0,		0.8,		0.0,		0,			0,		0.1,			0,		0,	},	// PARAM_ACC_TRUN
			{ 0,		0,		0.016,	0.22,	1.0,	0.1,	0,		2.0,	0,		0.8,		0.0,		0,			0,		0.1,			0,		0,	},	// PARAM_CONST_TRUN
			{ 0,		0,		0.016,	0.22,	1.0,	0.1,	0,		2.0,	0,		0.8,		0.0,		0,			5,		0.5,			0,		0,	},	// PARAM_DEC_TRUN
		},
		/* �ʏ�(PARAM_NORMAL) */
		{	//	FFSA	FFS		FFAA	FFA		���xkp	���xki	���xkd	�ʒukp	�ʒuki	�p���xkp	�p���xki	�p���xkd	�p�xkp	�p�xki		��kp	��kd
			{ 0.0,		0,		0.016,	0.22,	3.0,	0.1,	0,		2.0,	0,		0.8,		0.0,		0,			0,		0.1,			0,		0,	},	// PARAM_ACC_TRUN
			{ 0,		0,		0.016,	0.22,	3.0,	0.1,	0,		2.0,	0,		0.8,		0.0,		0,			0,		0.1,			0,		0,	},	// PARAM_CONST_TRUN
			{ 0,		0,		0.016,	0.22,	3.0,	0.1,	0,		2.0,	0,		0.8,		0.0,		0,			5,		0.5,			0,		0,	},	// PARAM_DEC_TRUN
		},
		/* ����(PARAM_FAST) */
		{	//	FFSA	FFS		FFAA	FFA		���xkp	���xki	���xkd	�ʒukp	�ʒuki	�p���xkp	�p���xki	�p���xkd	�p�xkp	�p�xki		��kp	��kd
			{ 0.0,		0,		0.016,	0.22,	3.0,	0.1,	0,		2.0,	0,		0.8,		0.0,		0,			0,		0.1,			0,		0,	},	// PARAM_ACC_TRUN
			{ 0,		0,		0.016,	0.22,	3.0,	0.1,	0,		2.0,	0,		0.8,		0.0,		0,			0,		0.1,			0,		0,	},	// PARAM_CONST_TRUN
			{ 0,		0,		0.016,	0.22,	3.0,	0.1,	0,		2.0,	0,		0.8,		0.0,		0,			5,		0.5,			0,		0,	},	// PARAM_DEC_TRUN
		},
		/* ������(PARAM_VERY_FAST) */
		{	//	FFSA	FFS		FFAA	FFA		���xkp	���xki	���xkd	�ʒukp	�ʒuki	�p���xkp	�p���xki	�p���xkd	�p�xkp	�p�xki		��kp	��kd
			{ 0.0,		0,		0.016,	0.22,	3.0,	0.1,	0,		2.0,	0,		0.8,		0.0,		0,			0,		0.1,			0,		0,	},	// PARAM_ACC_TRUN
			{ 0,		0,		0.016,	0.22,	3.0,	0.1,	0,		2.0,	0,		0.8,		0.0,		0,			0,		0.1,			0,		0,	},	// PARAM_CONST_TRUN
			{ 0,		0,		0.016,	0.22,	3.0,	0.1,	0,		2.0,	0,		0.8,		0.0,		0,			5,		0.5,			0,		0,	},	// PARAM_DEC_TRUN
		}
	};

	/* �X�����[���Q�C���f�[�^ */
	const stGAIN f_SlaGainData[PARAM_MOVE_SPEED_MAX][PARAM_SURA_MAX] = {

		/* ���ᑬ(PARAM_VERY_SLOW) */
		{	//	FFSA	FFS		FFAA	FFA		���xkp	���xki	���xkd	�ʒukp	�ʒuki	�p���xkp	�p���xki	�p���xkd	�p�xkp		�p�xki		��kp	��kd
			{ 0.01,		0.2,	0,		0,		0.3,	0.03,	0,		0.1,	0,		0.5,		0.01,		0,			0.5,		0.5,		0.1,	0.03,	},	// PARAM_ENTRY_SURA
			{ 0.01,		0.2,	0.0055,	0.08,	0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0.0,		0.5,		0.5,		0,		0,	},	// PARAM_ACC_SURA
			{ 0.01,		0.2,	0.0055,	0.08,	0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0.0,		0.5,		0.5,		0,		0,	},	// PARAM_CONST_SURA
			{ 0.01,		0.2,	0.0055,	0.08,	0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0.0,		0.5,		0.5,		0,		0,	},	// PARAM_DEC_SURA
			{ 0.01,		0.2,	0,		0,		0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0,			0.5,		0.5,		0.1,	0.03,	},	// PARAM_EXIT_SURA
		},
		/* �ᑬ(PARAM_SLOW) */
		{	//	FFSA	FFS		FFAA	FFA		���xkp	���xki	���xkd	�ʒukp	�ʒuki	�p���xkp	�p���xki	�p���xkd	�p�xkp		�p�xki		��kp	��kd
			{ 0.01,		0.2,	0,		0,		0.3,	0.03,	0,		0.1,	0,		0.5,		0.01,		0,			0.5,		0.5,		0.3,	0.03,	},	// PARAM_ENTRY_SURA
			{ 0.01,		0.2,	0.0055,	0.08,	0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0.0,		0.5,		0.5,		0,		0,	},	// PARAM_ACC_SURA
			{ 0.01,		0.2,	0.0055,	0.08,	0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0.0,		0.5,		0.5,		0,		0,	},	// PARAM_CONST_SURA
			{ 0.01,		0.2,	0.0055,	0.08,	0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0.0,		0.5,		0.5,		0,		0,	},	// PARAM_DEC_SURA
			{ 0.01,		0.2,	0,		0,		0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0,			0.5,		0.5,		0.3,	0.03,	},	// PARAM_EXIT_SURA
		},
		/* �ʏ�(PARAM_NORMAL)500 *///�p���xkp500 �p�xkp25000
		{	//	FFSA	FFS		FFAA	FFA		���xkp	���xki	���xkd	�ʒukp	�ʒuki	�p���xkp	�p���xki	�p���xkd	�p�xkp		�p�xki		��kp	��kd
			{ 0.01,		0.2,	0,		0,		0.3,	0.03,	0,		0.1,	0,		0.5,		0.01,		0,			0.5,		0.5,		0.1,	0.03,	},	// PARAM_ENTRY_SURA
			{ 0.01,		0.2,	0.0055,	0.08,	0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0.0,		0.5,		0.5,		0,		0,	},	// PARAM_ACC_SURA
			{ 0.01,		0.2,	0.0055,	0.08,	0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0.0,		0.5,		0.5,		0,		0,	},	// PARAM_CONST_SURA
			{ 0.01,		0.2,	0.0055,	0.08,	0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0.0,		0.5,		0.5,		0,		0,	},	// PARAM_DEC_SURA
			{ 0.01,		0.2,	0,		0,		0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0,			0.5,		0.5,		0.1,	0.03,	},	// PARAM_EXIT_SURA
		},
		/* ����(PARAM_FAST) 600*/
		{	//	FFSA	FFS		FFAA	FFA		���xkp	���xki	���xkd	�ʒukp	�ʒuki	�p���xkp	�p���xki	�p���xkd	�p�xkp		�p�xki		��kp	��kd
			{ 0.01,		0.2,	0,		0,		0.3,	0.03,	0,		0.1,	0,		0.5,		0.01,		0,			0.5,		0.5,		0.1,	0.03,	},	// PARAM_ENTRY_SURA
			{ 0.01,		0.2,	0.0055,	0.08,	0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0.0,		0.5,		0.5,		0,		0,	},	// PARAM_ACC_SURA
			{ 0.01,		0.2,	0.0055,	0.08,	0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0.0,		0.5,		0.5,		0,		0,	},	// PARAM_CONST_SURA
			{ 0.01,		0.2,	0.0055,	0.08,	0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0.0,		0.5,		0.5,		0,		0,	},	// PARAM_DEC_SURA
			{ 0.01,		0.2,	0,		0,		0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0,			0.5,		0.5,		0.1,	0.03,	},	// PARAM_EXIT_SURA
		},
		/* ������(PARAM_VERY_FAST) */
		{	//	FFSA	FFS		FFAA	FFA		���xkp	���xki	���xkd	�ʒukp	�ʒuki	�p���xkp	�p���xki	�p���xkd	�p�xkp		�p�xki		��kp	��kd
			{ 0.01,		0.2,	0,		0,		0.3,	0.03,	0,		0.1,	0,		0.5,		0.01,		0,			0.5,		0.5,		0.1,	0.03,	},	// PARAM_ENTRY_SURA
			{ 0.01,		0.2,	0.0055,	0.08,	0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0.0,		0.5,		0.5,		0,		0,	},	// PARAM_ACC_SURA
			{ 0.01,		0.2,	0.0055,	0.08,	0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0.0,		0.5,		0.5,		0,		0,	},	// PARAM_CONST_SURA
			{ 0.01,		0.2,	0.0055,	0.08,	0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0.0,		0.5,		0.5,		0,		0,	},	// PARAM_DEC_SURA
			{ 0.01,		0.2,	0,		0,		0.3,	0.03,	0,		0.1,	0,		1.3,		0.13,		0,			0.5,		0.5,		0.1,	0.03,	},	// PARAM_EXIT_SURA
		}
	};

void PARAM_setSpeedType( enPARAM_MODE en_mode, enPARAM_MOVE_SPEED en_speed )
{
	switch( en_mode ){
		
		case PARAM_ST:
			en_Speed_st = en_speed;
			break;
		
		case PARAM_TRUN:
			en_Speed_trun = en_speed;
			break;
		
		case PARAM_SLA:
			en_Speed_sla = en_speed;
			break;
			
		default:
			printf("�ݒ肵�����x�̃p�����[�^�^�C�v������܂��� \n\r");
			break;
	}
}

const stSPEED* PARAM_getSpeed( enPARAM_MODE en_mode )
{
	const stSPEED* p_adr;
	
	switch( en_mode ){
		
		case PARAM_ST:													// ���i
		case PARAM_ACC:													// ������(���i)
		case PARAM_CONST:												// ������(���i)
		case PARAM_DEC:													// ������(���i)
//		case PARAM_BACK_ACC:											// ������(��i)
//		case PARAM_BACK_CONST:											// ������(��i)
//		case PARAM_BACK_DEC:											// ������(��i)
		case PARAM_SKEW_ACC:											// ������(�΂�)
		case PARAM_SKEW_CONST:											// ������(�΂�)
		case PARAM_SKEW_DEC:											// ������(�΂�)
		case PARAM_HIT_WALL:											// �ǂ��Đ���
			p_adr = &f_StSpeedData[en_Speed_st];
			break;
			
		case PARAM_TRUN:												// ����
		case PARAM_ACC_TRUN:											// ������(���n�M����)
		case PARAM_CONST_TRUN:											// ������(���n�M����)
		case PARAM_DEC_TRUN:											// ������(���n�M����)
			p_adr = &f_TurnSpeedData[en_Speed_trun];
			break;
			
		case PARAM_SLA:													// �X�����[��
		case PARAM_ENTRY_SURA:											// �X�����[���O�̑O�i����(�X�����[��)
		case PARAM_ACC_SURA:											// ������(�X�����[��)
		case PARAM_CONST_SURA:											// ������(�X�����[��)
		case PARAM_DEC_SURA:											// ������(�X�����[��)
		case PARAM_EXIT_SURA:											// �X�����[����̑O�i����(�X�����[��)
			p_adr = &f_SlaSpeedData[en_Speed_sla];
			break;

		default:														// Err�A�Ƃ肠�����E�E�E�i�������j���h�����߁j
			printf("�ݒ肵�����x�^�C�v������܂��� \n\r");
			p_adr = &f_SlaSpeedData[en_Speed_sla];
			break;
	}
	
	return p_adr;
}

const stGAIN* PARAM_getGain( enPARAM_MODE en_mode )
{
	const stGAIN* p_adr;
	
	switch( en_mode ){
		
		case PARAM_ACC:													// ������(���i)
		case PARAM_CONST:												// ������(���i)
		case PARAM_DEC:													// ������(���i)
//		case PARAM_BACK_ACC:											// ������(��i)
//		case PARAM_BACK_CONST:											// ������(��i)
//		case PARAM_BACK_DEC:											// ������(��i)
		case PARAM_SKEW_ACC:											// ������(�΂�)
		case PARAM_SKEW_CONST:											// ������(�΂�)
		case PARAM_SKEW_DEC:											// ������(�΂�)
		case PARAM_HIT_WALL:											// �ǂ��Đ���
			p_adr = &f_StGainData[en_Speed_st][GET_INDEX_ST( en_mode )];
			break;
			
		case PARAM_ACC_TRUN:											// ������(���n�M����)
		case PARAM_CONST_TRUN:											// ������(���n�M����)
		case PARAM_DEC_TRUN:											// ������(���n�M����)
			p_adr = &f_TurnGainData[en_Speed_trun][GET_INDEX_TRUN( en_mode )];
			break;
			
		case PARAM_ENTRY_SURA:											// �X�����[���O�̑O�i����(�X�����[��)
		case PARAM_ACC_SURA:											// ������(�X�����[��)
		case PARAM_CONST_SURA:											// ������(�X�����[��)
		case PARAM_DEC_SURA:											// ������(�X�����[��)
		case PARAM_EXIT_SURA:											// �X�����[����̑O�i����(�X�����[��)
			p_adr = &f_SlaGainData[en_Speed_sla][GET_INDEX_SLA( en_mode )];
			break;
		
		default:														// Err�A�Ƃ肠�����E�E�E�i�������j���h�����߁j
			printf("�ݒ肵���Q�C���^�C�v������܂��� \n\r");
			p_adr = &f_SlaGainData[en_Speed_sla][GET_INDEX_SLA( en_mode )];
			break;
	}
	
	return p_adr;
}

void PARAM_makeSra( float f_speed, float f_angAcc, float f_g , enSLA_TYPE en_mode)
{

	float	f_start_x;					// �J�nx�ʒu [mm]
	float	f_start_y;					// �J�ny�ʒu [mm]
	float	f_final_x;					// �ŏIx�ʒu [mm]
	float	f_final_y;					// �ŏIy�ʒu [mm]
	float	f_final_ang;				// �p�������̍ŏI�p�x [rad]	
	float	f_maxAngleV		= 0;		// �ő�p���x[rad/s]
	float	f_timeAcc		= 0;		// ��������[s]
	float	f_accAngle		= 0;		// �����p�x[rad]
	float	f_timeConst		= 0;		// ��������[s]
	float	f_constAngle	= 0;		// �����p�x[rad]
	float	f_ang			= 0;		// ���Z�p�A�p�x [rad]
	float	f_time			= 0;		// ���Z�p�A���� [s]
	float	f_x;						// ���Z�px�ʒu [mm]
	float	f_y;						// ���Z�py�ʒu [mm]
	uint16_t	i = 0;						// ���[�v�p
	stSLA* 	p_adr = &st_Sla[en_mode];		// �L�^���鑖�s�f�[�^

	/* �X�����[���ɉ������ݒ�l����X�����[���ɕK�v�ȃp�����[�^�����Z���� */
	switch(en_mode){

		case SLA_90:
			f_start_x   = HALF_BLOCK;
			f_start_y   = 0.0f;
			f_final_x   = BLOCK;
			f_final_y   = HALF_BLOCK;
			f_final_ang = 90.0f * DEG_TO_RAD;
			break;

		case SLA_45:
			f_start_x   = HALF_BLOCK;
			f_start_y   = 0.0f;
			f_final_x   = BLOCK * 0.75f;
			f_final_y   = BLOCK * 0.75f;
			f_final_ang = 45.0f * DEG_TO_RAD;
			break;
			
		case SLA_N90:
			f_start_x   = HALF_BLOCK * 0.5f * 1.4142f;
			f_start_y   = 0.0f;
			f_final_x   = HALF_BLOCK * 1.4142f;
			f_final_y   = HALF_BLOCK * 0.5f * 1.4142f;
			f_final_ang = 90.0f * DEG_TO_RAD;
			break;
			
		case SLA_135:
			f_start_x   = HALF_BLOCK;
			f_start_y   = 0.0f;
			f_final_x   = BLOCK * 1.25f;
			f_final_y   = BLOCK * 0.25;
			f_final_ang = 135.0f * DEG_TO_RAD;
			break;

		default:
			break;
	}

	/* �������p�x�̎Z�o */
	f_maxAngleV		= f_g / f_speed;							// �ő�p���x[rad/s] �i��[rad/s] = g[mm/s^2] / v[mm/s] �j
	f_timeAcc		= f_maxAngleV / f_angAcc;					// �ő�̊p���x�ɂȂ�܂ł̉�������[s]
	f_accAngle		= 0.5f * f_angAcc * f_timeAcc * f_timeAcc;	// �����������Ԃ̊p�x[rad] (��[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
	f_constAngle	= f_final_ang - f_accAngle * 2;				// ���p���x�̋�Ԃ̊p�x[rad] (��[rad] = Total�p�x - �����p�x + �����p�x )
	f_timeConst		= f_constAngle / f_maxAngleV;				// �ő�̊p���x�œ��삷�鎞��[s]�i t[s] = ��[rad] / ��[rad/s] �j

	/* -------------------------------- */
	/*  �X�����[���������̈ʒu�����߂�  */
	/* -------------------------------- */
	/* ���W�J�n�ʒu */
	f_x		= f_start_x;
	f_y		= f_start_y;

	/* �������̍��W���Z */
	for( i=0; i<(uint16_t)(f_timeAcc*1000); i++ ){				// [msec]
	
		f_time	=  0.001f * (float)i;								// ����[s]
		f_ang	=  0.5f * f_angAcc * f_time * f_time;				// �p�x[rad] (��[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;			// X���W[mm] 
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;			// Y���W[mm]
	}
	
	/* �������̍��W���Z */
	for( i=0; i<(uint16_t)(f_timeConst*1000); i++ ){				// [msec]
	
		f_time	 = 0.001f * (float)i;							// ����[s]
		f_ang	 = f_accAngle + f_maxAngleV * f_time;			// �p�x[rad] (��[rad] = ��[rad/s] * t[s] )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;		// X���W[mm] 
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;		// Y���W[mm]
	}

	/* �������̍��W���Z */
	for( i=0; i<(uint16_t)(f_timeAcc*1000); i++ ){				// [msec]
	
		f_time	 = 0.001f * (float)i;							// ����[s]
		f_ang	 = f_accAngle + f_constAngle +0.5f * f_angAcc * f_time * f_time;	// �p�x[rad] (��[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;		// X���W[mm] 
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;		// Y���W[mm]
	}

	/* ---------------------------- */
	/*  �X�����[���p�p�����[�^�쐬  */
	/* ---------------------------- */
	p_adr->f_speed				= f_speed;										// �i�����x[mm/s]
	printf("�i�����x %5.2f\n\r",f_speed);
	p_adr->f_angAcc				= f_angAcc * RAD_TO_DEG ;						// �p�����x[deg/s]
	printf("�p�����x %5.2f\n\r",f_angAcc * RAD_TO_DEG);
	p_adr->f_angvel				= f_maxAngleV * RAD_TO_DEG;						// �ő�p���x���Z�o �ő�p���x[deg/s]
	printf("�ő�p���x %5.2f\n\r",f_maxAngleV * RAD_TO_DEG);
	p_adr->us_accAngvelTime		= (uint16_t)( f_timeAcc * 1000.0f );				// �p��������[msec]
	printf("�p�������� %5.2f\n\r",f_timeAcc * 1000.0f);
	p_adr->us_constAngvelTime	= (uint16_t)( f_timeConst * 1000.0f );			// ���p������[msec]
	printf("���p������ %5.2f\n\r",f_timeConst * 1000.0f);
	p_adr->f_ang_AccEnd			= f_accAngle * RAD_TO_DEG;						// �p���������p�x[deg]
	printf("�p���� %5.2f\n\r",f_accAngle * RAD_TO_DEG);
	p_adr->f_ang_ConstEnd		= ( f_accAngle + f_constAngle ) * RAD_TO_DEG;	// ���p���x�����p�x[deg]
	printf("�p���� %5.2f\n\r",( f_accAngle + f_constAngle ) * RAD_TO_DEG);
	p_adr->f_ang_Total			= f_final_ang * RAD_TO_DEG;						// �S�ړ��p�x[deg]
	printf("�S�ړ� %5.2f\n\r",f_final_ang * RAD_TO_DEG);
	
	/* �K�v�Ȑi���Ƒޏo�̋������Z�o���� */
	switch(en_mode){
		case SLA_90:
			p_adr->f_escapeLen = f_final_x - f_x ;//-4
			p_adr->f_entryLen  = f_final_y - f_y ;//+8
			break;

		case SLA_45:
			p_adr->f_escapeLen = 1.4142f * ( f_final_x - f_x );
			p_adr->f_entryLen  = f_final_y - f_y - ( f_final_x - f_x );
			break;

		case SLA_N90:
			p_adr->f_escapeLen = f_final_x - f_x;
			p_adr->f_entryLen  = f_final_y - f_y;
			break;

		case SLA_135:
			p_adr->f_escapeLen = 1.4142f * ( f_final_x - f_x );
			p_adr->f_entryLen  = f_final_y - f_y + ( f_final_x - f_x );
			break;

		default:
			break;
	}
	printf("entry %5.2f\n\r",f_final_x - f_x);
	printf("escape %5.2f\n\r",f_final_y - f_y);
}

stSLA* PARAM_getSra( enSLA_TYPE en_mode )
{
	return &st_Sla[en_mode];
}
