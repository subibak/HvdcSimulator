#ifndef FCTYPECONV_H
#define FCTYPECONV_H

/*********************************************************************
**	Function Type
**********************************************************************/
extern uint32	stdB2RInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdB2RRunFunc	(uint32 , uint32 );

extern uint32	stdR2BInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdR2BRunFunc	(uint32 , uint32 );

extern uint32	stdB2DInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdB2DRunFunc	(uint32 , uint32 );

extern uint32	stdD2BInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdD2BRunFunc	(uint32 , uint32 );

extern uint32	stdD2RInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdD2RRunFunc	(uint32 , uint32 );

extern uint32	stdR2DInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdR2DRunFunc	(uint32 , uint32 );

extern uint32	stdMb2DInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdMb2DRunFunc	(uint32 , uint32 );

extern uint32	stdB2IInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdB2IRunFunc	(uint32 , uint32 );

extern uint32	stdD2MbInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdD2MbRunFunc	(uint32 , uint32 );

extern uint32	stdD2IInifFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdD2IRunFunc	(uint32 , uint32 );

extern uint32	stdI2BInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdI2BRunFunc	(uint32 , uint32 );

extern uint32	stdI2DInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdI2DRunFunc	(uint32 , uint32 );

/*********************************************************************
**	
**	Bool To Rea1 Type 변환 FB 정의
**	
**********************************************************************/
#define	BOOL2REAL_CODE			131
#define	BOOL2REAL_SPEC_NUM		1
#define	BOOL2REAL_VAR_NUM		0
#define	BOOL2REAL_OUTPUT_NUM	1
		
typedef struct {		
	/* 스펙 */	
	uint32	in;
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	out;
} strFC0131Info;		

/*********************************************************************
**	
**	Real To Bool Type 변환 FB 정의
**
**********************************************************************/
#define	REAL2BOOL_CODE			132
#define	REAL2BOOL_SPEC_NUM		1
#define	REAL2BOOL_VAR_NUM		0
#define	REAL2BOOL_OUTPUT_NUM	1
		
typedef struct {		
	/* 스펙 */	
	float	in;
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0132Info;		

/*********************************************************************
**	
**	Bool To int32 Type 변환 FB 정의
**
**********************************************************************/
#define	BOOL2DINT_CODE			133
#define	BOOL2DINT_SPEC_NUM		1
#define	BOOL2DINT_VAR_NUM		0
#define	BOOL2DINT_OUTPUT_NUM	1
		
typedef struct {		
	/* 스펙 */	
	uint32	in;
		
	/* 내부변수 */	
		
	/* 출력 */	
	int32	out;
} strFC0133Info;		

/*********************************************************************
**	
**	Int32 TO Bool Type 변환 FB 정의
**
**********************************************************************/
#define	DINT2BOOL_CODE			134
#define	DINT2BOOL_SPEC_NUM		1
#define	DINT2BOOL_VAR_NUM		0
#define	DINT2BOOL_OUTPUT_NUM	1
		
typedef struct {		
	/* 스펙 */	
	int32	in;
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out;
} strFC0134Info;		

/*********************************************************************
**	
**	Int32 TO Real Type 변환 FB 정의
**
**********************************************************************/
#define	DINT2REAL_CODE			135
#define	DINT2REAL_SPEC_NUM		1
#define	DINT2REAL_VAR_NUM		0
#define	DINT2REAL_OUTPUT_NUM	1
		
typedef struct {		
	/* 스펙 */	
	int32	in;
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	out;
} strFC0135Info;	

/*********************************************************************
**	
**	Real TO Int32 Type 변환 FB 정의
**
**********************************************************************/
#define	REAL2DINT_CODE			136
#define	REAL2DINT_SPEC_NUM		1
#define	REAL2DINT_VAR_NUM		0
#define	REAL2DINT_OUTPUT_NUM	1
		
typedef struct {		
	/* 스펙 */	
	float	in;
		
	/* 내부변수 */	
		
	/* 출력 */	
	int32	out;
} strFC0136Info;		

/*********************************************************************
**	
**	Bool TO Int32 Type 변환 FB 정의
**
**********************************************************************/
#define	M_BOOL_TO_DINT_CODE				137
#define	M_BOOL_TO_DINT_SPEC_NUM			32
#define	M_BOOL_TO_DINT_VAR_NUM			0
#define	M_BOOL_TO_DINT_OUTPUT_NUM		1
		
typedef struct {		
	/* 스펙 */	
	uint32	in[31];
	uint32	sign;
		
	/* 내부변수 */	
		
	/* 출력 */	
	int32	out;
} strFC0137Info;		

/*********************************************************************
**	
**	Bool TO Int16 Type 변환 FB 정의
**
**********************************************************************/
#define	BOOL_TO_INT_CODE			138
#define	BOOL_TO_INT_SPEC_NUM		16
#define	BOOL_TO_INT_VAR_NUM			0
#define	BOOL_TO_INT_OUTPUT_NUM		1
		
typedef struct {		
	/* 스펙 */	
	uint32	in[15];
	uint32 sign;
		
	/* 내부변수 */	
		
	/* 출력 */	
	int32	out;
} strFC0138Info;		

/*********************************************************************
**	
**	Int32 TO Bool Type 변환 FB 정의
**
**********************************************************************/
#define	DINT_TO_M_BOOL_CODE				139
#define	DINT_TO_M_BOOL_SPEC_NUM			1
#define	DINT_TO_M_BOOL_VAR_NUM			0
#define	DINT_TO_M_BOOL_OUTPUT_NUM		32
		
typedef struct {		
	/* 스펙 */	
	int32	in;
		
	/* 내부변수 */	
		
	/* 출력 */	
	int32	out[32];
} strFC0139Info;		

/*********************************************************************
**	
**	Int32 TO int16 Type 변환 FB 정의
**
**********************************************************************/
#define	DINT_TO_INT_CODE			140
#define	DINT_TO_INT_SPEC_NUM		1
#define	DINT_TO_INT_VAR_NUM			0
#define	DINT_TO_INT_OUTPUT_NUM		1
		
typedef struct {		
	/* 스펙 */	
	int32	in;
		
	/* 내부변수 */	
		
	/* 출력 */	
	int32	out;
} strFC0140Info;		

/*********************************************************************
**	
**	Int16 TO Bool Type 변환 FB 정의
**
**********************************************************************/
#define	INT_TO_BOOL_CODE			141
#define	INT_TO_BOOL_SPEC_NUM		1
#define	INT_TO_BOOL_VAR_NUM			0
#define	INT_TO_BOOL_OUTPUT_NUM		16
		
typedef struct {		
	/* 스펙 */	
	int32	in;
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	out[16];
} strFC0141Info;		

/*********************************************************************
**	
**	Int16 TO Int32 Type 변환 FB 정의
**
**********************************************************************/
#define	INT_TO_DINT_CODE		142
#define	INT_TO_DINT_SPEC_NUM	1
#define	INT_TO_DINT_VAR_NUM		0
#define	INT_TO_DINT_OUTPUT_NUM	1
		
typedef struct {		
	/* 스펙 */	
	int32	in;
		
	/* 내부변수 */	
		
	/* 출력 */	
	int32	out;
} strFC0142Info;		



	







#endif