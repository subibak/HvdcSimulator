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
**	TImer Off FB ����
**	
**********************************************************************/
#define	TOF_CODE		261
#define	TOF_SPEC_NUM	2
#define	TOF_VAR_NUM		2
#define	TOF_OUTPUT_NUM	2
		
typedef struct {		
	/* ���� */	
	uint32	in;
	int32	timeInput; /* Cycle �ֱ� ���� */
		
	/* ���κ��� */	
	uint32	prevValue;
	uint32	UNUSED;
		
	/* ��� */	
	int32	elapsedTime; /* Cycle �ֱ� ���� */
	uint32	Q;
} strFC0261Info;		

/*********************************************************************
**	
**	Time ON FB ����
**
**********************************************************************/
#define	TON_CODE		262
#define	TON_SPEC_NUM	2
#define	TON_VAR_NUM		2
#define	TON_OUTPUT_NUM	2
		
typedef struct {		
	/* ���� */	
	uint32	in;
	int32	timeInput; /* Cycle �ֱ� ���� */
		
	/* ���κ��� */	
	uint32	prevValue;
	uint32	UNUSED;
		
	/* ��� */	
	int32	elapsedTime; /* Cycle �ֱ� ���� */
	uint32	Q;
} strFC0262Info;		

/*********************************************************************
**	
**	Time PULSE FB ����
**
**********************************************************************/
#define	TP_CODE			263
#define	TP_SPEC_NUM		2
#define	TP_VAR_NUM		2
#define	TP_OUTPUT_NUM	2
		
typedef struct {		
	/* ���� */	
	uint32	in;
	int32	timeInput; /* Cycle �ֱ� ���� */
		
	/* ���κ��� */	
	uint32	prevValue;
	uint32	UNUSED;
		
	/* ��� */	
	int32	elapsedTime; /* Cycle �ֱ� ���� */
	uint32	Q;
} strFC0263Info;		





#endif
