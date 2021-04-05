#ifndef FCEDGEDECT_H
#define FCEDGEDECT_H

/*********************************************************************
**	Function Type
**********************************************************************/
extern uint32	stdFtrigInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdFtrigRunFunc	(uint32 , uint32 );

extern uint32	stdRtrigInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdRtrigRunFunc	(uint32 , uint32 );

/*********************************************************************
**	
**	Falling Edge Trigger FB 정의
**
**********************************************************************/
#define	FTRIG_CODE			211
#define	FTRIG_SPEC_NUM		1
#define	FTRIG_VAR_NUM		1
#define	FTRIG_OUTPUT_NUM	1
		
typedef struct {		
	/* 스펙 - 1개*/
	uint32 	input; 		/* S01: 입력값 */

	/* 내부변수 - 1개 */
	uint32	prevInput;

    /* 출력 - 1개 */
    uint32	output;		/* N01: 출력값 */
} strFC0211Info;		

/*********************************************************************
**	
**	rising Edge Trigger FB 정의
**
**********************************************************************/					
#define	RTRIG_CODE			213
#define	RTRIG_SPEC_NUM		1
#define	RTRIG_VAR_NUM		1
#define	RTRIG_OUTPUT_NUM	1
		
typedef struct {		
	/* 스펙 - 1개*/
	uint32 	input; 		/* S01: 입력값 */

	/* 내부변수 - 1개 */
	uint32	prevInput;

    /* 출력 - 1개 */
    uint32	output;			/* N01: 출력값 */
} strFC0213Info;		


#endif