#ifndef FCSELECTION_H
#define FCSELECTION_H

/*********************************************************************
**	사용자 정의
**********************************************************************/
#ifndef MY_MIN
	#define MY_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef MY_MAX
	#define MY_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

/*********************************************************************
**	Function Type
**********************************************************************/
extern uint32	stdLimitInitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdLimitRunFunc	(uint32, uint32);

extern uint32	stdMax2InitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdMax2RunFunc	(uint32, uint32);

extern uint32	stdMax4InitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdMax4RunFunc	(uint32, uint32);

extern uint32	stdMax8InitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdMax8RunFunc	(uint32, uint32);

extern uint32	stdMin2InitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdMin2RunFunc	(uint32, uint32);

extern uint32	stdMin4InitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdMin4RunFunc	(uint32, uint32);

extern uint32	stdMin8InitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdMin8RunFunc	(uint32, uint32);

extern uint32	stdMux2InitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdMux2RunFunc	(uint32, uint32);

extern uint32	stdMux4InitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdMux4RunFunc	(uint32, uint32);

extern uint32	stdMux8InitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdMux8RunFunc	(uint32, uint32);

extern uint32	stdSelInitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdSelRunFunc	(uint32, uint32);

/*********************************************************************
**	
**	Limit FB 정의
**
**********************************************************************/
#define	LIMIT_CODE					231
#define	LIMIT_SPEC_NUM				3
#define	LIMIT_VAR_NUM				0
#define	LIMIT_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */
	uint32 	min; 		/* S01: 입력값 */
	uint32	input; 		/* S02: 입력값 */
	uint32	max; 		/* S03: 입력값 */

	/* 내부변수 - 0개 */

    /* 출력 */
    uint32	output;		/* N01: 출력값 */
} strFC0231Info;		

/*********************************************************************
**	
**	2개 입력에 대해 Max 값 선택 FB 정의
**
**********************************************************************/
#define	MAX2_CODE				232
#define	MAX2_SPEC_NUM			2
#define	MAX2_VAR_NUM			0
#define	MAX2_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[2];
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	out;
} strFC0232Info;		
	
/*********************************************************************
**	
**	4개 입력에 대해 Max 값 선택 FB 정의
**
**********************************************************************/
#define	MAX4_CODE				234
#define	MAX4_SPEC_NUM			4
#define	MAX4_VAR_NUM			0
#define	MAX4_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[4];
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	out;
} strFC0234Info;		

/*********************************************************************
**	
**	8개 입력에 대해 Max 값 선택 FB 정의
**
**********************************************************************/
#define	MAX8_CODE				238
#define	MAX8_SPEC_NUM			8
#define	MAX8_VAR_NUM			0
#define	MAX8_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[8];
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	out;
} strFC0238Info;		

/*********************************************************************
**	
**	2개 입력에 대해 Min. 값 선택 FB 정의
**
**********************************************************************/
#define	MIN2_CODE				240
#define	MIN2_SPEC_NUM			2
#define	MIN2_VAR_NUM			0
#define	MIN2_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[2];
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	out;
} strFC0240Info;		

/*********************************************************************
**	
**	4개 입력에 대해 Min. 값 선택 FB 정의
**
**********************************************************************/
#define	MIN4_CODE				242
#define	MIN4_SPEC_NUM			4
#define	MIN4_VAR_NUM			0
#define	MIN4_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[4];
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	out;
} strFC0242Info;		

/*********************************************************************
**	
**	8개 입력에 대해 Min. 값 선택 FB 정의
**
**********************************************************************/
#define	MIN8_CODE				246
#define	MIN8_SPEC_NUM			8
#define	MIN8_VAR_NUM			0
#define	MIN8_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	float	in[8];
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	out;
} strFC0246Info;		

/*********************************************************************
**	
**	2개 입력에 대해 index에 따라 선택 츨력 FB 정의
**
**********************************************************************/
#define	MUX2_CODE				248
#define	MUX2_SPEC_NUM			3
#define	MUX2_VAR_NUM			0
#define	MUX2_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	int32   choiceIndex;
	float	in[2];
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	out;
} strFC0248Info;		

/*********************************************************************
**	
**	4개 입력에 대해 index에 따라 선택 츨력 FB 정의
**
**********************************************************************/
#define	MUX4_CODE				250
#define	MUX4_SPEC_NUM			5
#define	MUX4_VAR_NUM			0
#define	MUX4_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	int32   choiceIndex;
	float	in[4];
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	out;
} strFC0250Info;		

/*********************************************************************
**	
**	8개 입력에 대해 index에 따라 선택 츨력 FB 정의
**
**********************************************************************/
#define	MUX8_CODE				254
#define	MUX8_SPEC_NUM			9
#define	MUX8_VAR_NUM			0
#define	MUX8_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	int32   choiceIndex;
	float	in[8];
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	out;
} strFC0254Info;		

/*********************************************************************
**	
**	2개 입력에 대해 index에 따라 선택 츨력 FB 정의
**
**********************************************************************/
#define	SEL_CODE				256
#define	SEL_SPEC_NUM			3
#define	SEL_VAR_NUM				0
#define	SEL_OUTPUT_NUM			1
		
typedef struct {		
	/* 스펙 */	
	uint32  choiceIndex;
	float   ina;
	float   inb;
		
	/* 내부변수 */	
		
	/* 출력 */	
	float	out;
} strFC0256Info;		






#endif