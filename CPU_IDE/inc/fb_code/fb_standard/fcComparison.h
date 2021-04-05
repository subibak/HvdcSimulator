#ifndef FCCOMPARISON_H
#define FCCOMPARISON_H

/*********************************************************************
**	Function Type
**********************************************************************/
extern uint32	stdEq2InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdEq2RunFunc	(uint32 , uint32 );

extern uint32	stdEq4InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdEq4RunFunc	(uint32 , uint32 );

extern uint32	stdEq8InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdEq8RunFunc	(uint32 , uint32 );

extern uint32	stdGe2InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdGe2RunFunc	(uint32 , uint32 );

extern uint32	stdGe4InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdGe4RunFunc	(uint32 , uint32 );

extern uint32	stdGe8InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdGe8RunFunc	(uint32 , uint32 );

extern uint32	stdGt2InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdGt2RunFunc	(uint32 , uint32 );

extern uint32	stdGt4InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdGt4RunFunc	(uint32 , uint32 );

extern uint32	stdGt8InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdGt8RunFunc	(uint32 , uint32 );

extern uint32	stdLe2InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdLe2RunFunc	(uint32 , uint32 );

extern uint32	stdLe4InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdLe4RunFunc	(uint32 , uint32 );

extern uint32	stdLe8InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdLe8RunFunc	(uint32 , uint32 );

extern uint32	stdLt2InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdLt2RunFunc	(uint32 , uint32 );

extern uint32	stdLt4InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdLt4RunFunc	(uint32 , uint32 );

extern uint32	stdLt8InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdLt8RunFunc	(uint32 , uint32 );

extern uint32	stdNe2InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdNe2RunFunc	(uint32 , uint32 );

extern uint32	stdNe4InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdNe4RunFunc	(uint32 , uint32 );

extern uint32	stdNe8InitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdNe8RunFunc	(uint32 , uint32 );

/*********************************************************************
**	
**	2개 입력 equal 비교 FB 정의
**
**********************************************************************/
#define	EQ2_CODE				81
#define	EQ2_SPEC_NUM			2
#define	EQ2_VAR_NUM				0
#define	EQ2_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[2];
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0081Info;		

/*********************************************************************
**	
**	4개 입력 equal 비교 FB 정의
**
**********************************************************************/
#define	EQ4_CODE				83
#define	EQ4_SPEC_NUM			4
#define	EQ4_VAR_NUM				0
#define	EQ4_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[4];
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0083Info;		

/*********************************************************************
**	
**	8개 입력 equal 비교 FB 정의
**
**********************************************************************/
#define	EQ8_CODE				87
#define	EQ8_SPEC_NUM			8
#define	EQ8_VAR_NUM				0
#define	EQ8_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[8];
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0087Info;		

/*********************************************************************
**	
**	2개 입력 greater equal 비교 FB 정의
**
**********************************************************************/
#define	GE2_CODE				89
#define	GE2_SPEC_NUM			2
#define	GE2_VAR_NUM				0
#define	GE2_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[2];
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0089Info;		

/*********************************************************************
**	
**	4개 입력 greater equal 비교 FB 정의
**
**********************************************************************/
#define	GE4_CODE				91
#define	GE4_SPEC_NUM			4
#define	GE4_VAR_NUM				0
#define	GE4_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[4];
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0091Info;		

/*********************************************************************
**	
**	8개 입력 greater equal 비교 FB 정의
**
**********************************************************************/
#define	GE8_CODE				95
#define	GE8_SPEC_NUM			8
#define	GE8_VAR_NUM				0
#define	GE8_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[8];
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0095Info;		

/*********************************************************************
**	
**	2개 입력 greater than 비교 FB 정의
**
**********************************************************************/
#define	GT2_CODE				97
#define	GT2_SPEC_NUM			2
#define	GT2_VAR_NUM				0
#define	GT2_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[2];
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0097Info;		

/*********************************************************************
**	
**	4개 입력 greater than 비교 FB 정의
**
**********************************************************************/
#define	GT4_CODE				99
#define	GT4_SPEC_NUM			4
#define	GT4_VAR_NUM				0
#define	GT4_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[4];
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0099Info;		

/*********************************************************************
**	
**	8개 입력 greater than 비교 FB 정의
**
**********************************************************************/
#define	GT8_CODE				103
#define	GT8_SPEC_NUM			8
#define	GT8_VAR_NUM				0
#define	GT8_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[8];
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0103Info;		

/*********************************************************************
**	
**	2개 입력 Less or Equal 비교 FB 정의
**
**********************************************************************/
#define	LE2_CODE				105
#define	LE2_SPEC_NUM			2
#define	LE2_VAR_NUM				0
#define	LE2_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[2];
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0105Info;		

/*********************************************************************
**	
**	4개 입력 Less or Equal 비교 FB 정의
**
**********************************************************************/
#define	LE4_CODE				107
#define	LE4_SPEC_NUM			4
#define	LE4_VAR_NUM				0
#define	LE4_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[4];
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0107Info;		

/*********************************************************************
**	
**	8개 입력 Less or Equal 비교 FB 정의
**
**********************************************************************/
#define	LE8_CODE				111
#define	LE8_SPEC_NUM			8
#define	LE8_VAR_NUM				0
#define	LE8_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[8];
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0111Info;		

/*********************************************************************
**	
**	2개 입력 Less than 비교 FB 정의
**
**********************************************************************/
#define	LT2_CODE				113
#define	LT2_SPEC_NUM			2
#define	LT2_VAR_NUM				0
#define	LT2_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[2];
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0113Info;		

/*********************************************************************
**	
**	4개 입력 Less than 비교 FB 정의
**
**********************************************************************/
#define	LT4_CODE				115
#define	LT4_SPEC_NUM			4
#define	LT4_VAR_NUM				0
#define	LT4_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[4];
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0115Info;		

/*********************************************************************
**	
**	8개 입력 Less than 비교 FB 정의
**
**********************************************************************/
#define	LT8_CODE				119
#define	LT8_SPEC_NUM			8
#define	LT8_VAR_NUM				0
#define	LT8_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[8];
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0119Info;		

/*********************************************************************
**	
**	2개 입력 Not Equal 비교 FB 정의
**
**********************************************************************/
#define	NE2_CODE				121
#define	NE2_SPEC_NUM			2
#define	NE2_VAR_NUM				0
#define	NE2_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[2];
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0121Info;		

/*********************************************************************
**	
**	4개 입력 Not Equal 비교 FB 정의
**
**********************************************************************/
#define	NE4_CODE				123
#define	NE4_SPEC_NUM			4
#define	NE4_VAR_NUM				0
#define	NE4_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[4];
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0123Info;		

/*********************************************************************
**	
**	8개 입력 Not Equal 비교 FB 정의
**
**********************************************************************/
#define	NE8_CODE				127
#define	NE8_SPEC_NUM			8
#define	NE8_VAR_NUM				0
#define	NE8_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[8];
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0127Info;		

#endif