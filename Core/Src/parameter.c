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
		{ 1800,			1800,		0,				0,				},		// ���ᑬ(PARAM_VERY_SLOW)
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
/*  GainData  */
/* ============== */

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
			printf("Can't find parameter type \n\r");
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
			printf("Can't find speed type \n\r");
			p_adr = &f_SlaSpeedData[en_Speed_sla];
			break;
	}
	
	return p_adr;
}
/*
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
			printf("Can't find gain type \n\r");
			p_adr = &f_SlaGainData[en_Speed_sla][GET_INDEX_SLA( en_mode )];
			break;
	}
	
	return p_adr;
}
*/
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

	/* caluculate acc and dec angle speed */
	f_maxAngleV		= f_g / f_speed;							// max angle speed[rad/s] (omega[rad/s] = g[mm/s^2] / v[mm/s] )
	f_timeAcc		= f_maxAngleV / f_angAcc;					// acc time[s]
	f_accAngle		= 0.5f * f_angAcc * f_timeAcc * f_timeAcc;	// acc angle[rad] (theta[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
	f_constAngle	= f_final_ang - f_accAngle * 2;				// const angle[rad] (theta[rad] = Totalangle - (acc angle + dec angle) )
	f_timeConst		= f_constAngle / f_maxAngleV;				// max angle speed time[s]( t[s] = theta[rad] / omega[rad/s] )

	/* -------------------------------- */
	/* sla end position */
	/* -------------------------------- */
	/* start position */
	f_x		= f_start_x;
	f_y		= f_start_y;

	/* ACC */
	for( i=0; i<(uint16_t)(f_timeAcc*1000); i++ ){				// [msec]
	
		f_time	=  0.001f * (float)i;								// time[s]
		f_ang	=  0.5f * f_angAcc * f_time * f_time;				// angle[rad] (theta[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;			// Xposition[mm]
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;			// Yposition[mm]
	}
	
	/* CONST */
	for( i=0; i<(uint16_t)(f_timeConst*1000); i++ ){				// [msec]
	
		f_time	 = 0.001f * (float)i;							// time[s]
		f_ang	 = f_accAngle + f_maxAngleV * f_time;			// angle[rad] (theta[rad] = omega[rad/s] * t[s] )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;		// Xposition[mm]
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;		// Yposition[mm]
	}

	/* DEC*/
	for( i=0; i<(uint16_t)(f_timeAcc*1000); i++ ){				// [msec]
	
		f_time	 = 0.001f * (float)i;							// time[s]
		f_ang	 = f_accAngle + f_constAngle +0.5f * f_angAcc * f_time * f_time;	// angle[rad] (theta[rad] = 1/2 * a[rad/s^2] * t[s]^2 )
		f_x		+= f_speed * (float)sin( f_ang ) * 0.001f;		// Xposition[mm]
		f_y		+= f_speed * (float)cos( f_ang ) * 0.001f;		// Yposition[mm]
	}

	/* ---------------------------- */
	/*  sla parameter  */
	/* ---------------------------- */
	p_adr->f_speed				= f_speed;
	printf("enter speed %5.2f\n\r",f_speed);
	p_adr->f_angAcc				= f_angAcc * RAD_TO_DEG ;
	printf("angle acc%5.2f\n\r",f_angAcc * RAD_TO_DEG);
	p_adr->f_angvel				= f_maxAngleV * RAD_TO_DEG;
	printf("max angle speed%5.2f\n\r",f_maxAngleV * RAD_TO_DEG);
	p_adr->us_accAngvelTime		= (uint16_t)( f_timeAcc * 1000.0f );
	printf("time of angle acc %5.2f\n\r",f_timeAcc * 1000.0f);
	p_adr->us_constAngvelTime	= (uint16_t)( f_timeConst * 1000.0f );
	printf("time of constant angle acc %5.2f\n\r",f_timeConst * 1000.0f);
	p_adr->f_ang_AccEnd			= f_accAngle * RAD_TO_DEG;
	printf("acc angle[deg] %5.2f\n\r",f_accAngle * RAD_TO_DEG);
	p_adr->f_ang_ConstEnd		= ( f_accAngle + f_constAngle ) * RAD_TO_DEG;
	printf("const angle[deg] %5.2f\n\r",( f_accAngle + f_constAngle ) * RAD_TO_DEG);
	p_adr->f_ang_Total			= f_final_ang * RAD_TO_DEG;
	printf("angle [deg] %5.2f\n\r",f_final_ang * RAD_TO_DEG);
	
	/* calaculate enter and exit length */
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
