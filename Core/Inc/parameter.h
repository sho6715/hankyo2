/*
 * parameter.h
 *
 *  Created on: 2021/02/04
 *      Author: shohe
 */

#ifndef INC_PARAMETER_H_
#define INC_PARAMETER_H_

#ifdef _VSCODE
#include "Inc/glob.h"
#else
#include "glob.h"
#endif

#define HALF_BLOCK			( 45.0f )					// ����� [mm]
#define BLOCK				( 90.0f )					// �P��� [mm]
#define HALF_BLOCK_SKEW			( 63.64f )				// �΂ߔ���� [mm]
#define BLOCK_SKEW			( 127.28f )				// �΂߂P��� [mm]

#define		SW_CHATTERING_WAIT		(200) 		//�X�C�b�`�̃`���^�����O�΍�

#define	FF_BALANCE_R				( 1.00f )					// [FF] �E�̃o�����X�W��
#define	FF_BALANCE_L				( 1.00f )					// [FF] ���̃o�����X�W�� 
#define FF_HIT_BALANCE_R			(1.00f)						//�o�b�N���̃o�����X�W��
#define FF_HIT_BALANCE_L			(1.00f)

#define SEARCH_SPEED				(300)
#define MAP_KNOWN_ACC_SPEED			(500)

#define DEG_TO_RAD  (3.1416f/180.0f)
#define RAD_TO_DEG  (180.0f/3.1416f)

#define MOT_WALL_EDGE_DIST			( 28.0f )	// �ǐ؂�Z���TOFF�`�ǂ܂�

#define		f_FF_speed_acc		(0.0016)				// �t�B�[�h�t�H���[�h�A�����x
#define		f_FF_angleS_acc		(0.01)		// �t�B�[�h�t�H���[�h�A�p�����x
#define 		f_FB_speed_kp		(1.8)			// �t�B�[�h�o�b�N�A���x ��ᐧ��
#define 		f_FB_speed_ki		(0.2)			// �t�B�[�h�o�b�N�A���x �ϕ�����
#define 		f_FB_speed_kd		(0.3)			// �t�B�[�h�o�b�N�A���x ��������
#define		f_FB_angleS_kp		(0.8)			// �t�B�[�h�o�b�N�A�p���x ��ᐧ��
#define		f_FB_angleS_ki		(0.1)			// �t�B�[�h�o�b�N�A�p���x �ϕ�����
#define		f_FB_angleS_kd		(0.08)			// �t�B�[�h�o�b�N�A�p���x ��������
#define		f_FB_wall_kp		(0.1)			// �t�B�[�h�o�b�N�A�� ��ᐧ��
#define		f_FB_wall_kd		(0.03)			// �t�B�[�h�o�b�N�A�� ��������

/* ������@ */
typedef enum{
	
	/* ========================================== */ 
	/*  �p�����[�^���擾����ۂɎg�p����V���{��  */ 
	/* ========================================== */ 
	/* ---------- */
	/*  ���i����  */
	/* ---------- */
	PARAM_ST_TOP = 0,				// �J�E���g�p
	// �� �����ǉ�����ꍇ�ɂ͂��̊ԂɋL��

		PARAM_ACC,					// ������(���i)
		PARAM_CONST,				// ������(���i)
		PARAM_DEC,					// ������(���i)
//		PARAM_BACK_ACC,				// ������(��i)
//		PARAM_BACK_CONST,			// ������(��i)
//		PARAM_BACK_DEC,				// ������(��i)
		PARAM_SKEW_ACC,				// ������(�΂�)
		PARAM_SKEW_CONST,			// ������(�΂�)
		PARAM_SKEW_DEC,				// ������(�΂�)
		PARAM_HIT_WALL,				// �ǂ��Đ���

	// ��  �����ǉ�����ꍇ�ɂ͂��̊ԂɋL��
	PARAM_ST_BTM,					// �J�E���g�p
	
	/* -------- */
	/*  �^�[��  */
	/* -------- */
	PARAM_TRUN_TOP,					// �J�E���g�p
	// ��  �����ǉ�����ꍇ�ɂ͂��̊ԂɋL��

		PARAM_ACC_TRUN,				// ������(���n�M����)
		PARAM_CONST_TRUN,			// ������(���n�M����)
		PARAM_DEC_TRUN,				// ������(���n�M����)

	// ��  �����ǉ�����ꍇ�ɂ͂��̊ԂɋL��
	PARAM_TRUN_BTM,					// �J�E���g�p
	
	/* ------------ */
	/*  �X�����[��  */
	/* ------------ */
	PARAM_SLA_TOP,					// �J�E���g�p
	// ��  �����ǉ�����ꍇ�ɂ͂��̊ԂɋL��

		PARAM_ENTRY_SURA,			// �X�����[���O�̑O�i����(�X�����[��)
		PARAM_ACC_SURA,				// ������(�X�����[��)
		PARAM_CONST_SURA,			// ������(�X�����[��)
		PARAM_DEC_SURA,				// ������(�X�����[��)
		PARAM_EXIT_SURA,			// �X�����[����̑O�i����(�X�����[��)

	// ��  �����ǉ�����ꍇ�ɂ͂��̊ԂɋL��
	PARAM_SLA_BTM,					// �J�E���g�p
	
	
	/* ===================================================================== */ 
	/*  PARAM_setGainType()�ɂă��[�h�����߂�ۂɈ����Ƃ��Ďg�p����V���{��  */ 
	/* ===================================================================== */ 
	PARAM_ST,						// ���i����
	PARAM_TRUN,						// ���񐧌�
	PARAM_SLA,						// �X�����[������
	
	
	/* ====================================================== */ 
	/*  �쐬����f�[�^�����J�E���g���邽�߂Ɏg�p����V���{��  */ 
	/* ====================================================== */ 
	PARAM_ST_MAX		= PARAM_ST_BTM   - PARAM_ST_TOP,		// ���i�ő吔
	PARAM_TRUN_MAX		= PARAM_TRUN_BTM - PARAM_TRUN_TOP,		// ����ő吔
	PARAM_SURA_MAX		= PARAM_SLA_BTM  - PARAM_SLA_TOP,		// �X�����[���ő吔
	
	
	PARAM_NC = 0xff,
	
}enPARAM_MODE;


/* ���쑬�x */
typedef enum{
	
	PARAM_VERY_SLOW = 0,	// ���ᑬ
	PARAM_SLOW,				// �ᑬ
	PARAM_NORMAL,			// �ʏ�
	PARAM_FAST,				// ����
	PARAM_VERY_FAST,		// ������
	
	PARAM_MOVE_SPEED_MAX
	
}enPARAM_MOVE_SPEED;

/* ���x�f�[�^ */
typedef struct{
	float			f_acc;					// �����x�i�������j
	float			f_dec;					// �����x�i�������j
	float			f_accAngle;				// �p�����x�i�������j
	float			f_decAngle;				// �p�����x�i�������j
}stSPEED;

/* �Q�C�� */
/*
typedef struct{
	float			f_FF_speed_acc;				// �t�B�[�h�t�H���[�h�A�����x
	float			f_FF_angleS_acc;		// �t�B�[�h�t�H���[�h�A�p�����x
	float 			f_FB_speed_kp;			// �t�B�[�h�o�b�N�A���x ��ᐧ��
	float 			f_FB_speed_ki;			// �t�B�[�h�o�b�N�A���x �ϕ�����
	float 			f_FB_speed_kd;			// �t�B�[�h�o�b�N�A���x ��������
	float			f_FB_angleS_kp;			// �t�B�[�h�o�b�N�A�p���x ��ᐧ��
	float			f_FB_angleS_ki;			// �t�B�[�h�o�b�N�A�p���x �ϕ�����
	float			f_FB_angleS_kd;			// �t�B�[�h�o�b�N�A�p���x ��������
	float			f_FB_wall_kp;			// �t�B�[�h�o�b�N�A�� ��ᐧ��
	float			f_FB_wall_kd;			// �t�B�[�h�o�b�N�A�� ��������
}stGAIN;
*/
/* �X�����[���f�[�^ */
typedef struct{
	float	f_speed;
	float	f_angAcc;
	float	f_angvel;
	float	f_entryLen;
	float	f_escapeLen;
	uint16_t	us_accAngvelTime;
	uint16_t	us_constAngvelTime;
	float	f_ang_AccEnd;
	float	f_ang_ConstEnd;
	float	f_ang_Total;
}stSLA;


/* �X�����[���^�C�v */
typedef enum{
	SLA_90,
	SLA_45,	
	SLA_135,
	SLA_N90,				// �΂� �� 90���� �΂�
	SLA_TYPE_MAX
}enSLA_TYPE;


void PARAM_setSpeedType( enPARAM_MODE en_mode, enPARAM_MOVE_SPEED en_speed );
const stSPEED* PARAM_getSpeed( enPARAM_MODE en_mode );
//const stGAIN* PARAM_getGain( enPARAM_MODE en_mode );

void PARAM_makeSra( float f_speed, float f_angAcc, float f_g , enSLA_TYPE en_mode);
stSLA* PARAM_getSra( enSLA_TYPE en_mode );

#endif /* INC_PARAMETER_H_ */
