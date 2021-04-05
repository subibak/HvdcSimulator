#ifndef FCCOUNTER_H
#define FCCOUNTER_H

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
extern uint32	stdCtdInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdCtdRunFunc	(uint32 , uint32 );

extern uint32	stdCtuInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdCtuRunFunc	(uint32 , uint32 );

extern uint32	stdCtudInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdCtudRunFunc	(uint32 , uint32 );

/*********************************************************************
**	
**	Down Counter FB 정의
**
**********************************************************************/
#define	CTD_CODE			201
#define	CTD_SPEC_NUM		3
#define	CTD_VAR_NUM			1
#define	CTD_OUTPUT_NUM		2
		
typedef struct {		
	/* 스펙 - 3개*/
	uint32 	loadCmd;
	uint32 	clkInput;
	int32 	loadVal;

	/* 내부변수 - 1개 */
	uint32	prevInput;

    /* 출력 - 2개 */
    uint32	downCountVal;
    uint32	q;
} strFC0201Info;		

/*********************************************************************
**	
**	Up Counter FB 정의
**
**********************************************************************/
#define	CTU_CODE			202
#define	CTU_SPEC_NUM		3
#define	CTU_VAR_NUM			1
#define	CTU_OUTPUT_NUM		2
		
typedef struct {		
	/* 스펙 - 3개*/
	uint32 	clkInput;
	uint32 	resetCmd;
	int32 	limitVal;

	/* 내부변수 - 1개 */
	uint32 	prevInput;

    /* 출력 - 2개 */
    int32	upCountVal;
    uint32	q;
} strFC0202Info;		

/*********************************************************************
**	
**	Up-Down Counter FB 정의
**
**********************************************************************/
#define	CTUD_CODE			203
#define	CTUD_SPEC_NUM		5
#define	CTUD_VAR_NUM		2
#define	CTUD_OUTPUT_NUM		3
		
typedef struct {		
	/* 스펙 - 5개*/
	uint32 	upClkInput;
	uint32 	downClkInput;
	uint32 	resetCmd;
	uint32 	loadCmd;
	int32 	loadVal;

	/* 내부변수 - 2개 */
	uint32 	prevUpClkInput;
	uint32 	prevDownClkInput;

    /* 출력 - 3개 */
    int32	upDownCountVal;
    uint32	qu;
    uint32	qd;
} strFC0203Info;		

#endif