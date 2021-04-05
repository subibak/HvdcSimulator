#ifndef FCBITSTABLEELE_H
#define FCBITSTABLEELE_H

/*********************************************************************
**	Function Type
**********************************************************************/
extern uint32	stdRsInitFunc (uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdRsRunFunc	 (uint32 , uint32 );

extern uint32	stdSrInitFunc (uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdSrRunFunc	 (uint32 taskId, uint32 fbBlockAddr);

/*********************************************************************
**	
**	Reset dominant Bitstable FB ����
**
**********************************************************************/
#define	RS_CODE				212
#define	RS_SPEC_NUM			2
#define	RS_VAR_NUM			0
#define	RS_OUTPUT_NUM		1
		
typedef struct {		
	/* �Է� - 2�� */
	uint32 	set; 				/* S01: �Է°� */
	uint32	resetDominant; 		/* S02: �Է°� */

	/* ���κ��� - 0�� */

    /* ��� - 1�� */
    uint32	q1;				/* N01: ��°� */
} strFC0212Info;		

/*********************************************************************
**	
**	set dominant Bitstable FB ����
**
**********************************************************************/
#define	SR_CODE				214
#define	SR_SPEC_NUM			2
#define	SR_VAR_NUM			0
#define	SR_OUTPUT_NUM		1
		
typedef struct {		
	/* ���� - 2��*/
	uint32 	setDominant; 	/* S01: �Է°� */
	uint32	reset; 			/* S02: �Է°� */

	/* ���κ��� - 0�� */

    /* ��� - 1�� */
    uint32	q1;		/* N01: ��°� */
} strFC0214Info;		




#endif