#ifndef FCNUMERICAL_H
#define FCNUMERICAL_H

/*********************************************************************
**	int32의 최대 최소값
**********************************************************************/
#ifndef MIN_INT32_VALUE
	#define 	MIN_INT32_VALUE	(-2147483647-1)
#endif

#ifndef MAX_INT32_VALUE
	#define 	MAX_INT32_VALUE	( 2147483647)
#endif

/*********************************************************************
**	Function Type
**********************************************************************/
extern uint32	stdAbsInitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdAbsRunFunc	(uint32, uint32);

extern uint32	stdSqrtInitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdSqrtRunFunc	(uint32, uint32);

extern uint32	stdExpInitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdExpRunFunc	(uint32, uint32);

extern uint32	stdLnInitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdLnRunFunc		(uint32, uint32);

extern uint32	stdLogInitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdLogRunFunc	(uint32, uint32);

extern uint32	stdACosInitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdACosRunFunc	(uint32, uint32);

extern uint32	stdAsinInitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdAsinRunFunc	(uint32, uint32);

extern uint32	stdAtanInitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdAtanRunFunc	(uint32, uint32);

extern uint32	stdCosInitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdCosRunFunc	(uint32, uint32);

extern uint32	stdSinInitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdSinRunFunc	(uint32, uint32);

extern uint32	stdTanInitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdTanRunFunc	(uint32, uint32);

/*********************************************************************
**	
**	ABS FB 정의
**
**********************************************************************/
#define	ABS_CODE				1
#define	ABS_SPEC_NUM			1
#define	ABS_VAR_NUM				0	
#define	ABS_OUTPUT_NUM			1
#define	ABS_REF_SINDEX			2

typedef struct {
	/* 스펙 - 1개*/
	float	input; 		/* S01: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    float	output;		/* N00: S01의 절대값 */
} strFC0001InfoForReal;

typedef struct {
	/* 스펙 - 1개*/
	int32	input; 		/* S01: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    int32	output;		/* N00: S01의 절대값 */
} strFC0001InfoForInt;

typedef struct {
	/* 스펙 - 1개*/
	uint32	input; 		/* S01: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 - 1개 */
    uint32	output;		/* N00: S01의 절대값 */
} strFC0001InfoForUint;

typedef union {
	strFC0001InfoForReal	forReal;
	strFC0001InfoForInt		forInt;
	strFC0001InfoForUint	forUint;
} strFC0001Info;

/*********************************************************************
**	
**	SQRT FB 정의
**
**********************************************************************/
#define	SQRT_CODE				23
#define	SQRT_SPEC_NUM			1
#define	SQRT_VAR_NUM			0
#define	SQRT_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	input;
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	output;
} strFC0023Info;		

/*********************************************************************
**	
**	exponential FB 정의
**
**********************************************************************/
#define	EXP_CODE				25
#define	EXP_SPEC_NUM			1
#define	EXP_VAR_NUM				0
#define	EXP_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	input;
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	output;
} strFC0025Info;

/*********************************************************************
**	
**	자연로그 FB 정의(밑이 e)
**
**********************************************************************/
#define	LN_CODE					26
#define	LN_SPEC_NUM				1
#define	LN_VAR_NUM				0
#define	LN_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	input;
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	output;
} strFC0026Info;

/*********************************************************************
**	
**	상용로그 FB 정의(밑이 10)
**
**********************************************************************/
#define	LOG_CODE				27
#define	LOG_SPEC_NUM			1
#define	LOG_VAR_NUM				0
#define	LOG_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	input;
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	output;
} strFC0027Info;

/*********************************************************************
**	
**	arccosine FB 정의
**
**********************************************************************/
#define	ACOS_CODE				71
#define	ACOS_SPEC_NUM			1
#define	ACOS_VAR_NUM			0
#define	ACOS_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	input;
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	output;
} strFC0071Info;

/*********************************************************************
**	
**	arcsine FB 정의
**
**********************************************************************/
#define	ASIN_CODE				72
#define	ASIN_SPEC_NUM			1
#define	ASIN_VAR_NUM			0
#define	ASIN_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	input;
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	output;
} strFC0072Info;

/*********************************************************************
**	
**	arctangent FB 정의
**
**********************************************************************/
#define	ATAN_CODE				73
#define	ATAN_SPEC_NUM			1
#define	ATAN_VAR_NUM			0
#define	ATAN_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	input;
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	output;
} strFC0073Info;

/*********************************************************************
**	
**	cosine FB 정의
**
**********************************************************************/
#define	COS_CODE				74
#define	COS_SPEC_NUM			1
#define	COS_VAR_NUM				0
#define	COS_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	input;
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	output;
} strFC0074Info;

/*********************************************************************
**	
**	sine FB 정의
**
**********************************************************************/
#define	SIN_CODE				75
#define	SIN_SPEC_NUM			1
#define	SIN_VAR_NUM				0
#define	SIN_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	input;
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	output;
} strFC0075Info;

/*********************************************************************
**	
**	tangent FB 정의
**
**********************************************************************/
#define	TAN_CODE				76
#define	TAN_SPEC_NUM			1
#define	TAN_VAR_NUM				0
#define	TAN_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	input;
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	output;
} strFC0076Info;



#endif