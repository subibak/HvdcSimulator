#ifndef FCARITHMETIC_H
#define FCARITHMETIC_H

/*********************************************************************
**	Function Type
**********************************************************************/
extern uint32	stdMoveInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdMoveRunFunc	(uint32 , uint32 );

extern uint32	stdAdd2InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdAdd2RunFunc	(uint32 , uint32 );

extern uint32	stdAdd4InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdAdd4RunFunc	(uint32 , uint32 );

extern uint32	stdAdd8InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdAdd8RunFunc	(uint32 , uint32 );

extern uint32	stdMul2InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdMul2RunFunc	(uint32 , uint32 );

extern uint32	stdMul4InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdMul4RunFunc	(uint32 , uint32 );

extern uint32	stdMul8InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdMul8RunFunc	(uint32 , uint32 );

extern uint32	stdMulDivInitFunc(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdMulDivRunFunc	(uint32 , uint32 );

extern uint32	stdSubInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdSubRunFunc	(uint32 , uint32 );

extern uint32	stdDivInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdDivRunFunc	(uint32 , uint32 );
		
extern uint32	stdModInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdModRunFunc	(uint32 , uint32 );

extern uint32	stdExptInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdExptRunFunc	(uint32 , uint32 );

/*********************************************************************
**	
**	Bit Rotation Left FB 정의
**
**********************************************************************/
#define	MOVE_CODE				13
#define	MOVE_SPEC_NUM			1
#define	MOVE_VAR_NUM			0	
#define	MOVE_OUTPUT_NUM			1
#define	MOVE_REF_SINDEX			1

typedef struct {		
	/* 스펙 */	
	float	input;
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	output;
} strFC0013Info;		

/*********************************************************************
**	
**	2개 입력 ADD FB 정의
**
**********************************************************************/
#define	ADD2_CODE				2
#define	ADD2_SPEC_NUM			2
#define	ADD2_VAR_NUM			0	
#define	ADD2_OUTPUT_NUM			1
#define	ADD2_REF_SINDEX			1

typedef struct {
	/* 스펙 - 2개*/
	float	input; 		/* S01: 입력값 */
	float	input2; 		/* S02: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    float	output;		/* N00: 입력값의 합 */
}strFC0002InfoForReal;

typedef struct {
	/* 스펙 - 2개*/
	int32	input; 		/* S01: 입력값 */
	int32	input2; 		/* S02: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
	uint32	output;	/* NOO: ENABLE 출력 */
}strFC0002InfoForInt;

typedef struct {
	/* 스펙 - 2개*/
	uint32	input; 		/* S01: 입력값 */
	uint32	input2; 		/* S02: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    uint32	output;		/* N00: 입력값의 합 */
}strFC0002InfoForUint;

typedef union {
	strFC0002InfoForReal	forReal;
	strFC0002InfoForInt		forInt;
	strFC0002InfoForUint	forUint;
} strFC0002Info;

/*********************************************************************
**	
**	4개 입력 ADD FB 정의
**
**********************************************************************/
#define	ADD4_CODE				4
#define	ADD4_SPEC_NUM			4
#define	ADD4_VAR_NUM			0	
#define	ADD4_OUTPUT_NUM			1
#define	ADD4_REF_SINDEX			1

typedef struct {
	/* 스펙 - 4개*/
	float	input; 			/* S01: 입력값 */
	float	input2; 		/* S02: 입력값 */
	float	input3; 		/* S03: 입력값 */
	float	input4; 		/* S03: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    float	output;			/* N00: 입력값의 합 */
}strFC0004InfoForReal;

typedef struct {
	/* 스펙 - 4개*/
	int32	input; 			/* S01: 입력값 */
	int32	input2; 		/* S02: 입력값 */
	int32	input3; 		/* S03: 입력값 */
	int32	input4; 		/* S03: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
	uint32	output;			/* NOO: ENABLE 출력 */
}strFC0004InfoForInt;

typedef struct {
	/* 스펙 - 4개*/
	uint32	input; 			/* S01: 입력값 */
	uint32	input2; 		/* S02: 입력값 */
	uint32	input3; 		/* S03: 입력값 */
	uint32	input4; 		/* S03: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    uint32	output;			/* N00: 입력값의 합 */
}strFC0004InfoForUint;

typedef union {
	strFC0004InfoForReal	forReal;
	strFC0004InfoForInt		forInt;
	strFC0004InfoForUint	forUint;
} strFC0004Info;

/*********************************************************************
**	
**	8개 입력 ADD FB 정의
**
**********************************************************************/
#define	ADD8_CODE				8
#define	ADD8_SPEC_NUM			8
#define	ADD8_VAR_NUM			0	
#define	ADD8_OUTPUT_NUM			1
#define	ADD8_REF_SINDEX			1

typedef struct {
	/* 스펙 - 4개*/
	float	input; 			/* S01: 입력값 */
	float	input2; 		/* S02: 입력값 */
	float	input3; 		/* S03: 입력값 */
	float	input4; 		/* S04: 입력값 */
	float	input5; 		/* S05: 입력값 */
	float	input6; 		/* S06: 입력값 */
	float	input7; 		/* S07: 입력값 */
	float	input8; 		/* S08: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    float	output;			/* N00: 입력값의 합 */
}strFC0008InfoForReal;

typedef struct {
	/* 스펙 - 8개*/
	int32	input; 			/* S01: 입력값 */
	int32	input2; 		/* S02: 입력값 */
	int32	input3; 		/* S03: 입력값 */
	int32	input4; 		/* S04: 입력값 */
	int32	input5; 		/* S05: 입력값 */
	int32	input6; 		/* S06: 입력값 */
	int32	input7; 		/* S07: 입력값 */
	int32	input8; 		/* S08: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
	uint32	output;			/* NOO: ENABLE 출력 */
}strFC0008InfoForInt;

typedef struct {
	/* 스펙 - 8개*/
	uint32	input; 			/* S01: 입력값 */
	uint32	input2; 		/* S02: 입력값 */
	uint32	input3; 		/* S03: 입력값 */
	uint32	input4; 		/* S04: 입력값 */
	uint32	input5; 		/* S05: 입력값 */
	uint32	input6; 		/* S06: 입력값 */
	uint32	input7; 		/* S07: 입력값 */
	uint32	input8; 		/* S08: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    uint32	output;			/* N00: 입력값의 합 */

}strFC0008InfoForUint;

typedef union {
	strFC0008InfoForReal	forReal;
	strFC0008InfoForInt		forInt;
	strFC0008InfoForUint	forUint;
} strFC0008Info;

/*********************************************************************
**	
**	2개 입력 Multiply FB 정의
**
**********************************************************************/
#define	MUL2_CODE				14
#define	MUL2_SPEC_NUM			2
#define	MUL2_VAR_NUM			0	
#define	MUL2_OUTPUT_NUM			1
#define MUL2_REF_SINEX			1

typedef struct {
	/* 스펙 - 2개*/
	float	input; 			/* S01: 입력값 */
	float	input2; 		/* S02: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 2개 */
    float	output;			/* N00: 입력값의 곱 */
}strFC0014InfoForReal;

typedef struct {
	/* 스펙 - 2개*/
	int32	input; 			/* S01: 입력값 */
	int32	input2; 		/* S02: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 2개 */
    int32	output;			/* N00: 입력값의 곱 */
}strFC0014InfoForInt;

typedef struct {
	/* 스펙 - 2개*/
	uint32	input; 			/* S01: 입력값 */
	uint32	input2; 		/* S02: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 2개 */
    uint32	output;			/* N00: 입력값의 곱 */
}strFC0014InfoForUint;

typedef union {
	strFC0014InfoForReal	forReal;
	strFC0014InfoForInt		forInt;
	strFC0014InfoForUint	forUint;
} strFC0014Info;

/*********************************************************************
**	
**	4개 입력 Multiply FB 정의
**
**********************************************************************/
#define	MUL4_CODE				16
#define	MUL4_SPEC_NUM			4
#define	MUL4_VAR_NUM			0	
#define	MUL4_OUTPUT_NUM			1
#define MUL4_REF_SINEX			1

typedef struct {
	/* 스펙 - 4개*/
	float	input; 			/* S01: 입력값 */
	float	input2; 		/* S02: 입력값 */
	float	input3; 		/* S03: 입력값 */
	float	input4; 		/* S04: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 2개 */
    float	output;			/* N00: 입력값의 곱 */
}strFC0016InfoForReal;

typedef struct {
	/* 스펙 - 4개*/
	int32	input; 			/* S01: 입력값 */
	int32	input2; 		/* S02: 입력값 */
	int32	input3; 		/* S03: 입력값 */
	int32	input4; 		/* S04: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 2개 */
    int32	output;			/* N00: 입력값의 곱 */
}strFC0016InfoForInt;

typedef struct {
	/* 스펙 - 4개*/
	uint32	input; 			/* S01: 입력값 */
	uint32	input2; 		/* S02: 입력값 */
	uint32	input3; 		/* S03: 입력값 */
	uint32	input4; 		/* S04: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 2개 */
    uint32	output;			/* N00: 입력값의 곱 */
}strFC0016InfoForUint;

typedef union {
	strFC0016InfoForReal	forReal;
	strFC0016InfoForInt		forInt;
	strFC0016InfoForUint	forUint;
} strFC0016Info;

/*********************************************************************
**	
**	8개 입력 Multiply FB 정의
**
**********************************************************************/
#define	MUL8_CODE				20
#define	MUL8_SPEC_NUM			8
#define	MUL8_VAR_NUM			0	
#define	MUL8_OUTPUT_NUM			1
#define MUL8_REF_SINEX			1

typedef struct {
	/* 스펙 - 8개*/
	float	input; 			/* S01: 입력값 */
	float	input2; 		/* S02: 입력값 */
	float	input3; 		/* S03: 입력값 */
	float	input4; 		/* S04: 입력값 */
	float	input5; 		/* S05: 입력값 */
	float	input6; 		/* S06: 입력값 */
	float	input7; 		/* S07: 입력값 */
	float	input8; 		/* S08: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    float	output;			/* N00: 입력값의 곱 */
}strFC0020InfoForReal;

typedef struct {
	/* 스펙 - 8개*/
	int32	input; 			/* S01: 입력값 */
	int32	input2; 		/* S02: 입력값 */
	int32	input3; 		/* S03: 입력값 */
	int32	input4; 		/* S04: 입력값 */
	int32	input5; 		/* S05: 입력값 */
	int32	input6; 		/* S06: 입력값 */
	int32	input7; 		/* S07: 입력값 */
	int32	input8; 		/* S08: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    int32	output;			/* N00: 입력값의 곱 */
}strFC0020InfoForInt;

typedef struct {
	/* 스펙 - 8개*/
	uint32	input; 			/* S01: 입력값 */
	uint32	input2; 		/* S02: 입력값 */
	uint32	input3; 		/* S03: 입력값 */
	uint32	input4; 		/* S04: 입력값 */
	uint32	input5; 		/* S05: 입력값 */
	uint32	input6; 		/* S06: 입력값 */
	uint32	input7; 		/* S07: 입력값 */
	uint32	input8; 		/* S08: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    uint32	output;			/* N00: 입력값의 곱 */
}strFC0020InfoForUint;

typedef union {
	strFC0020InfoForReal	forReal;
	strFC0020InfoForInt		forInt;
	strFC0020InfoForUint	forUint;
} strFC0020Info;


/*********************************************************************
**	
**	8개 입력 Multiply FB 정의
**
**********************************************************************/
#define	MULDIV_CODE				22
#define	MULDIV_SPEC_NUM			3
#define	MULDIV_VAR_NUM			0
#define	MULDIV_OUTPUT_NUM		2
		
typedef struct {		
	/* 스펙 */	
    int32 input;
    int32 multiplier;
    int32 dividor;
		
	/* 내부변수 */	
		
	/* 출력 */	
	int32 quotient;		/* 몫 */
	int32 remainder;	/* 나머지 */
} strFC0022Info;		

/*********************************************************************
**	
**	 Subtraction FB 정의
**
**********************************************************************/
#define	SUB_CODE				24
#define	SUB_SPEC_NUM			2
#define	SUB_VAR_NUM				0	
#define	SUB_OUTPUT_NUM			1
#define	SUB_REF_SINDEX			1

typedef struct {
	/* 스펙 - 2개*/
	float	input; 			/* S01: 입력값 */
	float	input2; 		/* S02: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    float	output;			/* N00: 입력값의 차 */
}strFC0024InfoForReal;

typedef struct {
	/* 스펙 - 2개*/
	int32	input; 			/* S01: 입력값 */
	int32	input2; 		/* S02: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    int32	output;			/* N00: 입력값의 차 */
}strFC0024InfoForInt;

typedef struct {
	/* 스펙 - 2개*/
	uint32	input; 			/* S01: 입력값 */
	uint32	input2; 		/* S02: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    uint32	output;			/* N00: 입력값의 차 */
}strFC0024InfoForUint;

typedef union {
	strFC0024InfoForReal	forReal;
	strFC0024InfoForInt		forInt;
	strFC0024InfoForUint	forUint;
} strFC0024Info;

/*********************************************************************
**	
**	 Division FB 정의
**
**********************************************************************/
#define	DIV_CODE				10
#define	DIV_SPEC_NUM			2
#define	DIV_VAR_NUM				0	
#define	DIV_OUTPUT_NUM			1
#define	DIV_REF_SINDEX			1

typedef struct {
	/* 스펙 - 2개*/
	int32	input; 			/* S01: 입력값 */
	int32	dividor; 		/* S02: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    int32	quotient;			/* N00: S01/S02 */
}strFC0010InfoForInt;

typedef struct {
	/* 스펙 - 2개*/
	uint32	input; 			/* S01: 입력값 */
	uint32	dividor; 		/* S02: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    uint32	quotient;			/* N00: S01/S02 */
}strFC0010InfoForUint;

typedef struct {
	/* 스펙 - 2개*/
	float	input; 			/* S01: 입력값 */
	float	dividor; 		/* S02: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    float	quotient;		/* N00: S01/S02 */
}strFC0010InfoForReal;
	
typedef union {
	strFC0010InfoForReal	forReal;
	strFC0010InfoForInt		forInt;
	strFC0010InfoForUint	forUint;
} strFC0010Info;

/*********************************************************************
**	
**	 MOdulo FB 정의
**
**********************************************************************/
#define	MOD_CODE				12
#define	MOD_SPEC_NUM			2
#define	MOD_VAR_NUM				0	
#define	MOD_OUTPUT_NUM			1
#define	MOD_REF_SINDEX			1

typedef struct {
	/* 스펙 - 2개*/
	int32	input; 			/* S01: 입력값 */
	int32	dividor; 		/* S02: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    int32	remainder;			/* N00: S01%S02 */
}strFC0012InfoForInt;

typedef struct {
	/* 스펙 - 2개*/
	uint32	input; 			/* S01: 입력값 */
	uint32	dividor; 		/* S02: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    uint32	remainder;			/* N00: S01%S02 */
}strFC0012InfoForUint;

typedef union {
	strFC0012InfoForInt		forInt;
	strFC0012InfoForUint	forUint;
} strFC0012Info;

/*********************************************************************
**	
**	 거듭 제곱 FB 정의
**
**********************************************************************/
#define	EXPT_CODE				11
#define	EXPT_SPEC_NUM			2
#define	EXPT_VAR_NUM			0	
#define	EXPT_OUTPUT_NUM			1
#define	EXPT_REF_SINDEX			2

typedef struct {
	/* 스펙 - 2개*/
	float	input; 			/* S01: 입력값 */
	float	input2; 		/* S02: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    float	output;			/* N00: S01^S02 */
}strFC0011InfoForReal;

typedef struct {
	/* 스펙 - 2개*/
	float	input; 			/* S01: 입력값 */
	int32	input2; 		/* S02: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    float	output;			/* N00: S01^S02 */
}strFC0011InfoForInt;

typedef struct {
	/* 스펙 - 1개*/
	float	input; 			/* S01: 입력값 */
	uint32	input2; 		/* S02: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    float	output;			/* N00: S01^S02 */
}strFC0011InfoForUint;

typedef union {
	strFC0011InfoForReal	forReal;
	strFC0011InfoForInt		forInt;
	strFC0011InfoForUint	forUint;
} strFC0011Info;


#endif