#ifndef FCBITWISE_H
#define FCBITWISE_H

/*********************************************************************
**	Function Type
**********************************************************************/
extern uint32	stdAnd2InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdAnd2RunFunc	(uint32 , uint32 );

extern uint32	stdAnd4InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdAnd4RunFunc	(uint32 , uint32 );

extern uint32	stdAnd8InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdAnd8RunFunc	(uint32 , uint32 );

extern uint32	stdOr2InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdOr2RunFunc	(uint32 , uint32 );

extern uint32	stdOr4InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdOr4RunFunc	(uint32 , uint32 );

extern uint32	stdOr8InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdOr8RunFunc	(uint32 , uint32 );

extern uint32	stdXor2InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdXor2RunFunc	(uint32 , uint32 );

extern uint32	stdXor4InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdXor4RunFunc	(uint32 , uint32 );

extern uint32	stdXor8InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdXor8RunFunc	(uint32 , uint32 );

extern uint32	stdNotInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdNotRunFunc	(uint32 , uint32 );

extern uint32	stdBgetInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdBgetRunFunc	(uint32 , uint32 );

extern uint32	stdBitAndInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdBitAndRunFunc	(uint32 , uint32 );

extern uint32	stdBitOrInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdBitOrRunFunc	(uint32 , uint32 );

extern uint32	stdBitSetInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdBitSetRunFunc	(uint32 , uint32 );

extern uint32	stdRegInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdRegRunFunc	(uint32 , uint32 );

extern uint32	stdSrFfInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdSrFfRunFunc	(uint32 , uint32 );

/*********************************************************************
**	
**	2�� �Է� AND FB ����
**
**********************************************************************/
#define	AND2_CODE				31
#define	AND2_SPEC_NUM			2
#define	AND2_VAR_NUM			0	
#define	AND2_OUTPUT_NUM			1

typedef struct {
	/* ���� */	
	uint32	input[AND2_SPEC_NUM];
		
	/* ���κ��� */	
		
	/* ��� */	
	uint32	output;
}strFC0031Info;

/*********************************************************************
**	
**	4�� �Է� AND FB ����
**
**********************************************************************/
#define	AND4_CODE				33
#define	AND4_SPEC_NUM			4
#define	AND4_VAR_NUM			0
#define	AND4_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */	
	uint32	input[AND4_SPEC_NUM];
		
	/* ���κ��� */	
		
	/* ��� */	
	uint32	output;
} strFC0033Info;		

/*********************************************************************
**	
**	8�� �Է� AND FB ����
**
**********************************************************************/
#define	AND8_CODE				37
#define	AND8_SPEC_NUM			8
#define	AND8_VAR_NUM			0
#define	AND8_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */	
	uint32	input[AND8_SPEC_NUM];
		
	/* ���κ��� */	
		
	/* ��� */	
	uint32	output;
} strFC0037Info;		

/*********************************************************************
**	
**	2�� �Է� OR FB ����
**
**********************************************************************/
#define	OR2_BOOL_CODE			40
#define	OR2_BOOL_SPEC_NUM		2
#define	OR2_BOOL_VAR_NUM		0	
#define	OR2_BOOL_OUTPUT_NUM		1

typedef struct {
	/* ���� - 2��*/
	uint32	input[OR2_BOOL_SPEC_NUM];

	/* ���κ��� - 0�� */

    /* ��� - 1�� */
    uint32	output;
}strFC0040Info;

/*********************************************************************
**	
**	4�� �Է� OR FB ����
**
**********************************************************************/
#define	OR4_BOOL_CODE			42
#define	OR4_BOOL_SPEC_NUM		4
#define	OR4_BOOL_VAR_NUM		0	
#define	OR4_BOOL_OUTPUT_NUM		1

typedef struct {
	/* ���� - 4��*/
	uint32	input[OR4_BOOL_SPEC_NUM];

	/* ���κ��� - 0�� */

    /* ��� - 1�� */
    uint32	output;
}strFC0042Info;

/*********************************************************************
**	
**	8�� �Է� OR FB ����
**
**********************************************************************/
#define	OR8_BOOL_CODE			46
#define	OR8_BOOL_SPEC_NUM		8
#define	OR8_BOOL_VAR_NUM		0	
#define	OR8_BOOL_OUTPUT_NUM		1

typedef struct {
	/* ���� - 8��*/
	uint32	input[OR8_BOOL_SPEC_NUM];

	/* ���κ��� - 0�� */

    /* ��� - 1�� */
    uint32	output;
}strFC0046Info;

/*********************************************************************
**	
**	2�� �Է� XOR FB ����
**
**********************************************************************/
#define	XOR2_BOOL_CODE			52
#define	XOR2_BOOL_SPEC_NUM		2
#define	XOR2_BOOL_VAR_NUM		0	
#define	XOR2_BOOL_OUTPUT_NUM	1

typedef struct {
	/* ���� - 2��*/
	uint32	input;
	uint32	input2;

	/* ���κ��� - 0�� */

    /* ��� - 1�� */
    uint32	output;
}strFC0052Info;

/*********************************************************************
**	
**	4�� �Է� XOR FB ����
**
**********************************************************************/
#define	XOR4_CODE				54
#define	XOR4_SPEC_NUM			4
#define	XOR4_VAR_NUM			0
#define	XOR4_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */	
	uint32	in[XOR4_SPEC_NUM];
		
	/* ���κ��� */	
		
	/* ��� */	
	uint32	out;
} strFC0054Info;		

/*********************************************************************
**	
**	8�� �Է� XOR FB ����
**
**********************************************************************/
#define	XOR8_CODE				58
#define	XOR8_SPEC_NUM			8
#define	XOR8_VAR_NUM			0
#define	XOR8_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */	
	uint32	in[8];
		
	/* ���κ��� */	
		
	/* ��� */	
	uint32	out;
} strFC0058Info;		

/*********************************************************************
**	
**	NOT FB ����
**
**********************************************************************/
#define	NOT_CODE				39
#define	NOT_SPEC_NUM			1
#define	NOT_VAR_NUM				0	
#define	NOT_OUTPUT_NUM			1
#define	NOT_REF_SINDEX			1

typedef struct {
	/* ���� - 1��*/
	uint32	input; 		/* S01: �Է°� */

	/* ���κ��� - 0�� */

    /* ��� - 2�� */
    uint32	output;		/* N01: ��°� */
}strFC0039Info;

/*********************************************************************
**	
**	Bit Get FB ����
**
**********************************************************************/
#define	BGET_CODE				61
#define	BGET_SPEC_NUM			2
#define	BGET_VAR_NUM			0
#define	BGET_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */	
	int32 	bitIndex;
	uint32 	input;
		
	/* ���κ��� */	
		
	/* ��� */	
	uint32 output;
} strFC0061Info;		

/*********************************************************************
**	
**	Bit And FB ����
**
**********************************************************************/
#define	BITAND_CODE				62
#define	BITAND_SPEC_NUM			2
#define	BITAND_VAR_NUM			0
#define	BITAND_OUTPUT_NUM		1
		
typedef struct {		
	/* ���� */	
	uint32	input[BITAND_SPEC_NUM];
		
	/* ���κ��� */	
		
	/* ��� */	
	uint32	output;
} strFC0062Info;		

/*********************************************************************
**	
**	Bit OR FB ����
**
**********************************************************************/
#define	BITOR_CODE				63
#define	BITOR_SPEC_NUM			2
#define	BITOR_VAR_NUM			0
#define	BITOR_OUTPUT_NUM		1
		
typedef struct {		
	/* ���� */	
	uint32	input[BITOR_SPEC_NUM];
		
	/* ���κ��� */	
		
	/* ��� */	
	uint32	output;
} strFC0063Info;		

/*********************************************************************
**	
**	Bit Set FB ����
**
**********************************************************************/
#define	BSET_CODE				64
#define	BSET_SPEC_NUM			4
#define	BSET_VAR_NUM			0
#define	BSET_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */	
	uint32	enable;
	uint32	bitIndex;
	uint32	bitValue;
	uint32	input;

	/* ���κ��� */	
		
	/* ��� */	
	uint32	output;
} strFC0064Info;		

/*********************************************************************
**	
**	Bit Register FB ����
**
**********************************************************************/
#define	REG_CODE				65
#define	REG_SPEC_NUM			4
#define	REG_VAR_NUM				1
#define	REG_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */	
	uint32	set;
	uint32	load;
	uint32	reset;
	float	input;
		
	/* ���κ��� */	
	uint32 prevOutput;
		
	/* ��� */	
	float	output;
} strFC0065Info;		

/*********************************************************************
**	
**	SR Flip flop FB ����
**
**********************************************************************/
#define	SRFF_CODE				66
#define	SRFF_SPEC_NUM			4
#define	SRFF_VAR_NUM			1
#define	SRFF_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */	
	uint32	setInput;
	uint32	dataInput;
	uint32	clockInput;
	uint32	resetInput;
		
	/* ���κ��� */	
	uint32 prevOutput;
		
	/* ��� */	
	uint32	output;
} strFC0066Info;		


#endif