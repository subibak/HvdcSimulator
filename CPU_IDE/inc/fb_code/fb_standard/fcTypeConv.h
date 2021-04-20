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
#define	B2R_CODE			131
#define	B2R_SPEC_NUM		1
#define	B2R_VAR_NUM			0
#define	B2R_OUTPUT_NUM		1
		
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
#define	R2B_CODE			132
#define	R2B_SPEC_NUM		1
#define	R2B_VAR_NUM			0
#define	R2B_OUTPUT_NUM		1
		
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
#define	B2DI_CODE			133
#define	B2DI_SPEC_NUM		1
#define	B2DI_VAR_NUM		0
#define	B2DI_OUTPUT_NUM		1
		
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
#define	DI2B_CODE			134
#define	DI2B_SPEC_NUM		1
#define	DI2B_VAR_NUM		0
#define	DI2B_OUTPUT_NUM	1
		
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
#define	DI2R_CODE			135
#define	DI2R_SPEC_NUM		1
#define	DI2R_VAR_NUM		0
#define	DI2R_OUTPUT_NUM		1
		
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
#define	R2DI_CODE			136
#define	R2DI_SPEC_NUM		1
#define	R2DI_VAR_NUM		0
#define	R2DI_OUTPUT_NUM		1
		
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
#define	MB2DI_CODE				137
#define	MB2DI_SPEC_NUM			32
#define	MB2DI_VAR_NUM			0
#define	MB2DI_OUTPUT_NUM		1
		
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
#define	B2I_CODE			138
#define	B2I_SPEC_NUM		16
#define	B2I_VAR_NUM			0
#define	B2I_OUTPUT_NUM		1
		
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
#define	DI2MB_CODE				139
#define	DI2MB_SPEC_NUM			1
#define	DI2MB_VAR_NUM			0
#define	DI2MB_OUTPUT_NUM		32
		
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
#define	DI2I_CODE			140
#define	DI2I_SPEC_NUM		1
#define	DI2I_VAR_NUM		0
#define	DI2I_OUTPUT_NUM		1
		
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
#define	I2B_CODE			141
#define	I2B_SPEC_NUM		1
#define	I2B_VAR_NUM			0
#define	I2B_OUTPUT_NUM		16
		
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
#define	I2DI_CODE			142
#define	I2DI_SPEC_NUM		1
#define	I2DI_VAR_NUM		0
#define	I2DI_OUTPUT_NUM		1
		
typedef struct {		
	/* 스펙 */	
	int32	in;
		
	/* 내부변수 */	
		
	/* 출력 */	
	int32	out;
} strFC0142Info;		



	







#endif