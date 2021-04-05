#ifndef FCVMEBRDACC_H
#define FCVMEBRDACC_H

/*********************************************************************
**	Function Type
**********************************************************************/
extern uint32	diRdInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	diRdRunFunc		(uint32 , uint32 );

extern uint32	doWrInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	doWrRunFunc		(uint32 , uint32 );

extern uint32	dspRwInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	dspRwRunFunc		(uint32 , uint32 );

/*********************************************************************
**	VME Bus Access data typr
**********************************************************************/
#define VME_BYTE_ACC		(1)
#define VME_WORD_ACC		(2)
#define VME_DWORD_ACC		(3)

/*********************************************************************
**	VME Bus�� ������ �� �ִ� I/O ���� Access Error ����(FB ���)
**********************************************************************/
#define	IO_FB_INPUT_PARA_ERR	-1
#define	IO_FB_NORMAL			0
#define	IO_FB_ACCESS_ERR		1
#define	IO_FB_BOARD_CNFG_ERR	2
#define	IO_FB_BOARD_READY_ERR	3
#define	IO_FB_BOARD_RUN_ERR		4

/*********************************************************************
**	VME Bus�� ������ �� �ִ� I/O ���� ��
**********************************************************************/
#define	MIN_VME_IO_BRD_NUM	0
#define	MAX_VME_IO_BRD_NUM	4

/*********************************************************************
**	
**	VME Bus�� �ִ� DI Board Access FB 	
**
**********************************************************************/
#define	IOB_DI_CODE			311
#define	IOB_DI_SPEC_NUM		1
#define	IOB_DI_VAR_NUM		2
#define	IOB_DI_OUTPUT_NUM	2

		
typedef struct {		
	/* ���� */	
	int32	brdSlotId;		/* -1:������, 0~4:���� */
		
	/* ���κ��� */	
	uint32	ioBrdRunCnt;
	uint32	ioBrdTickCnt;
		
	/* ��� */	
	int32	errOutput;
	uint32	diOutput;
} strFC0311Info;		

/*********************************************************************
**	
**	VME Bus�� �ִ� DI Board Access FB 	
**
**********************************************************************/
#define	IOB_DO_CODE			312
#define	IOB_DO_SPEC_NUM		2
#define	IOB_DO_VAR_NUM		2
#define	IOB_DO_OUTPUT_NUM	2
		
typedef struct {		
	/* ���� */	
	int32	brdSlotId;
	uint32	doInput;
		
	/* ���κ��� */	
	uint32	ioBrdRunCnt;
	uint32	ioBrdTickCnt;
		
	/* ��� */	
	int32	errOutput;
	uint32	doOutput;
} strFC0312Info;		

/*********************************************************************
**	
**	VME Bus�� �ִ� DSP Board Access FB 	
**
[���� ����]
	- Data Read �� DWORD  �� ���� ��
**********************************************************************/
#define	IOB_CC_CODE			314
#define	IOB_CC_SPEC_NUM		8
#define	IOB_CC_VAR_NUM		2
#define	IOB_CC_OUTPUT_NUM	1
		
typedef struct {		
	/* ���� */	
    int32 brdSlotId;
    int32 accDataType;			/* 1:Byte, 2:WORD, 3:DWORD */
    int32 readMemRegion;		/* 1: M����, 2:S���� */
    int32 startAddrOfReadMem;
    int32 numOfReadData;	
    int32 writeMemRegion;		/* 1: M����, 2:S���� */
    int32 startAddrOfWriteMem;
    int32 numOfWriteData;
	
	/* ���κ��� */	
	uint32	ioBrdRunCnt;
	uint32	ioBrdTickCnt;
		
	/* ��� */	
    int32 errOutput;
} strFC0314Info;		

/* DSP ���忡 �����͸� Read/Write �� ��� CPU ������ ������ Access �޸�  Reagion */
#define	AIO_FB_MEM_M_REGION	(1)
#define	AIO_FB_MEM_S_REGION	(2)

/* DSP ���� Access ������ �ִ� ���� */
#define	MAX_DSP_BRD_ACC_DATA_NUM	256

/* DSP ���� Read/Write �� �޸� Offset */
#define DSP_BRD_DATA_WRITE_MEM_OFFSET	0x40
#define DSP_BRD_DATA_READ_MEM_OFFSET	0x440


#endif