#ifndef FCTIMER_H
#define FCTIMER_H

/*********************************************************************
**	Function Type
**********************************************************************/
extern uint32	stdToffInitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdToffRunFunc	(uint32, uint32);

extern uint32	stdTonInitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdTonRunFunc	(uint32, uint32);

extern uint32	stdTpInitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdTpRunFunc		(uint32, uint32);

/*********************************************************************
**	
**	TImer Off FB 정의
**	
**********************************************************************/
#define	TOF_CODE		261
#define	TOF_SPEC_NUM	2
#define	TOF_VAR_NUM		2
#define	TOF_OUTPUT_NUM	2
		
typedef struct {		
	/* 스펙 */	
	uint32	in;
	int32	timeInput; /* Cycle 주기 단위 */
		
	/* 내부변수 */	
	uint32	prevValue;
	uint32	UNUSED;
		
	/* 출력 */	
	int32	elapsedTime; /* Cycle 주기 단위 */
	uint32	Q;
} strFC0261Info;		

/*********************************************************************
**	
**	Time ON FB 정의
**
**********************************************************************/
#define	TON_CODE		262
#define	TON_SPEC_NUM	2
#define	TON_VAR_NUM		2
#define	TON_OUTPUT_NUM	2
		
typedef struct {		
	/* 스펙 */	
	uint32	in;
	int32	timeInput; /* Cycle 주기 단위 */
		
	/* 내부변수 */	
	uint32	prevValue;
	uint32	UNUSED;
		
	/* 출력 */	
	int32	elapsedTime; /* Cycle 주기 단위 */
	uint32	Q;
} strFC0262Info;		

/*********************************************************************
**	
**	Time PULSE FB 정의
**
**********************************************************************/
#define	TP_CODE			263
#define	TP_SPEC_NUM		2
#define	TP_VAR_NUM		2
#define	TP_OUTPUT_NUM	2
		
typedef struct {		
	/* 스펙 */	
	uint32	in;
	int32	timeInput; /* Cycle 주기 단위 */
		
	/* 내부변수 */	
	uint32	prevValue;
	uint32	UNUSED;
		
	/* 출력 */	
	int32	elapsedTime; /* Cycle 주기 단위 */
	uint32	Q;
} strFC0263Info;		





#endif
