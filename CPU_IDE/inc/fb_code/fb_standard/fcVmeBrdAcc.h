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
**	VME Bus상에 장착할 수 있는 I/O 보드 Access Error 종류(FB 출력)
**********************************************************************/
#define	IO_FB_INPUT_PARA_ERR	-1
#define	IO_FB_NORMAL			0
#define	IO_FB_ACCESS_ERR		1
#define	IO_FB_BOARD_CNFG_ERR	2
#define	IO_FB_BOARD_READY_ERR	3
#define	IO_FB_BOARD_RUN_ERR		4

/*********************************************************************
**	VME Bus상에 장착할 수 있는 I/O 보드 수
**********************************************************************/
#define	MIN_VME_IO_BRD_NUM	0
#define	MAX_VME_IO_BRD_NUM	4

/*********************************************************************
**	
**	VME Bus상에 있는 DI Board Access FB 	
**
**********************************************************************/
#define	IOB_DI_CODE			311
#define	IOB_DI_SPEC_NUM		1
#define	IOB_DI_VAR_NUM		2
#define	IOB_DI_OUTPUT_NUM	2

		
typedef struct {		
	/* 스펙 */	
	int32	brdSlotId;		/* -1:미장착, 0~4:장착 */
		
	/* 내부변수 */	
	uint32	ioBrdRunCnt;
	uint32	ioBrdTickCnt;
		
	/* 출력 */	
	int32	errOutput;
	uint32	diOutput;
} strFC0311Info;		

/*********************************************************************
**	
**	VME Bus상에 있는 DI Board Access FB 	
**
**********************************************************************/
#define	IOB_DO_CODE			312
#define	IOB_DO_SPEC_NUM		2
#define	IOB_DO_VAR_NUM		2
#define	IOB_DO_OUTPUT_NUM	2
		
typedef struct {		
	/* 스펙 */	
	int32	brdSlotId;
	uint32	doInput;
		
	/* 내부변수 */	
	uint32	ioBrdRunCnt;
	uint32	ioBrdTickCnt;
		
	/* 출력 */	
	int32	errOutput;
	uint32	doOutput;
} strFC0312Info;		

/*********************************************************************
**	
**	VME Bus상에 있는 DSP Board Access FB 	
**
[구현 사항]
	- Data Read 시 DWORD  만 구현 됨
**********************************************************************/
#define	IOB_CC_CODE			314
#define	IOB_CC_SPEC_NUM		8
#define	IOB_CC_VAR_NUM		2
#define	IOB_CC_OUTPUT_NUM	1
		
typedef struct {		
	/* 스펙 */	
    int32 brdSlotId;
    int32 accDataType;			/* 1:Byte, 2:WORD, 3:DWORD */
    int32 readMemRegion;		/* 1: M영역, 2:S영역 */
    int32 startAddrOfReadMem;
    int32 numOfReadData;	
    int32 writeMemRegion;		/* 1: M영역, 2:S영역 */
    int32 startAddrOfWriteMem;
    int32 numOfWriteData;
	
	/* 내부변수 */	
	uint32	ioBrdRunCnt;
	uint32	ioBrdTickCnt;
		
	/* 출력 */	
    int32 errOutput;
} strFC0314Info;		

/* DSP 보드에 데이터를 Read/Write 할 경우 CPU 보드의 데이터 Access 메모리  Reagion */
#define	AIO_FB_MEM_M_REGION	(1)
#define	AIO_FB_MEM_S_REGION	(2)

/* DSP 보드 Access 데이터 최대 갯수 */
#define	MAX_DSP_BRD_ACC_DATA_NUM	256

/* DSP 보드 Read/Write 시 메모리 Offset */
#define DSP_BRD_DATA_WRITE_MEM_OFFSET	0x40
#define DSP_BRD_DATA_READ_MEM_OFFSET	0x440


#endif