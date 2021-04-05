#ifndef FCMISCELLARY_H
#define FCMISCELLARY_H

/**************************************************************************
**		스테이션간 통신 FB 관련 정의
**************************************************************************/
#define ETH_NO_COM			(0)
#define ETH_MASTER_COM		(1)
#define ETH_SLAVE_COM		(2)

#define ETH_COM_SENDING		(0)
#define ETH_COM_WAITING		(1)

#define ETH_NO_RUN			(0)
#define ETH_MASTER_RUN		(1)
#define ETH_SLAVE_RUN		(2)

#define ETH_NO_POLLING		(0)
#define ETH_MASTER_POLLING	(1)
#define ETH_SLAVE_POLLING	(2)

#define	ETH_COM_INIT		(0)
#define	ETH_COM_ACCEPT		(1)
#define	ETH_COM_NOT_ACCEPT	(2)

#define	ETH_COM_NO_LINE		(0)
#define	ETH_COM_A_LINE		(1)
#define	ETH_COM_B_LINE		(2)
#define	ETH_COM_DUAL_LINE	(3)

#define ETH_BYTE_TYPE		(1)
#define ETH_WORD_TYPE		(2)
#define ETH_LWORD_TYPE		(3)

#define ETH_NO_SEND			(0)
#define ETH_SEND_READ		(1)
#define ETH_SEND_WRITE		(2)

#define FUNCTION_DISABLE	(0)
#define FUNCTION_ENABLE		(1)

/**************************************************************************
**		SWAP 정의
**************************************************************************/
#define SWAP_WORD(WordData)		((((WordData)<<8)|(((WordData)>>8)&0xFF))&0xFFFF)
#define SWAP_DWORD(DWordData)	((((DWordData)<<24)|(((DWordData)>>24)&0xFF)|((DWordData&0xFF00)<<8)|(((DWordData)>>8)&0xFF00))&0xFFFFFFFF)
#define SWAP_DWORD16(DWordData)	((((DWordData)<<16)|(((DWordData)>>16)&0xFFFF))&0xFFFFFFFF)

/**************************************************************************
**		외부 함수 정의
**************************************************************************/
extern uint32	ethDSndInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	ethDSndRunFunc(uint32, uint32);

extern uint32	ethDBrdInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	ethDBrdRunFunc(uint32, uint32);

extern uint32	pmc502RdInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	pmc502RdRunFunc(uint32, uint32);

extern uint32	sysStaCpInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	sysStaCpRunFunc(uint32, uint32);


/*********************************************************************
**	
**	스테이션간 Ethernet 통신, 즉 Read/Write 통신 FB 	
**
**********************************************************************/
#define	ETH_DATA_SEND_CODE			(302)
#define	ETH_DATA_SEND_SPEC_NUM		(20)
#define	ETH_DATA_SEND_VAR_NUM		(9)	
#define	ETH_DATA_SEND_OUTPUT_NUM	(9)

typedef struct {				
	/* 입력 변수 */			
	uint32	deviceSelect;		
	uint32	deviceStationNo;		
	uint32	masterALineIP;		
	uint32	masterBLineIP;		
	uint32	slaveALineIP;		
	uint32	slaveBLineIP;		
	uint32	comPeriodNo;		
	uint32	respondDelayNo;		
	uint32	comComand;		
	uint32  runStatus;			
	uint32	lineStatus;		
	uint32	runMode;		
	uint32	memoryTarget;		
	uint32	memoryAddress;		
	uint32	srcMemTarget;		
	uint32  srcSharedMemAddr;			
	uint32	dataType;		
	uint32	dataNumber;		
	uint32	op1;		
	uint32	op2;		
 	
 	/* 내부 변수 */			
	uint32	rcvTimeoutCheckFlag;  		
 	uint32	rcvFrameErrCnt;	      	
 	uint32	rcvDataNumber;        		
 	uint32	dualCopyCheckFlag;    		
 	uint32	seqNumErrCount;       		
 	float	periodSum;            		
 	float	respondDelaySum;      		
 	uint32  sequenceNo;	      	   	
 	uint32	dataSendFlag;         		
 	
 	/* 출력 변수 */							
	uint32	specErrOut;	     	
	uint32	runModeStatOut;      		
	uint32	sendCountOut;	     	
	uint32	sendErrCountOut;
	uint32	rcvDataNumberOut;      		
	uint32	rcvCountOut;	      	   		
	uint32	rcvTimeoutErrCntOut; 		
	uint32	rcvFrameErrCntOut;   		 
	uint32  seqNoMisErrCntOut;   							
}strFC0302Info;				
		
/*********************************************************************
**	
**	특정 스테이션에서 네트웍 상에 있는 모든 스테이션에 데이터를 공유할 수 있는 Ethernet 통신
**	즉 Broadcast 통신 FB 	
**
**********************************************************************/

#define	ETH_DATA_BROAD_CODE			(303)
#define	ETH_DATA_BROAD_SPEC_NUM		(12)
#define	ETH_DATA_BROAD_VAR_NUM		(3)	
#define	ETH_DATA_BROAD_OUTPUT_NUM	(4)
typedef struct {					
					
	uint32	deviceSelect;		/* 불필요 */	
	uint32	comPeriodNo;			
	uint32	respondDelayNo;		/* 불필요 */	
	uint32	memoryTarget;		/* 불필요 */	
	uint32	memoryAddress;			
	uint32	rmtMemTarget;		/* 불필요 */	
	uint32	rmtMemAddress;		/* 불필요 */	
	uint32	dataType;			
	uint32	dataNumber;			
	uint32	enableIn;			
	uint32	op1;			
	uint32	op2;			
					
	float	periodSum;            			
	uint32  sequenceNo;	      	   		
	uint32	dataSendFlag;         			
					
					
	uint32	specErrOut;			 
	uint32	enableOut;			
	uint32	sendCountOut;	     		
	uint32	sendErrCountOut;     			
}strFC0303Info;					
			

/*********************************************************************
**	
**	 
**
**********************************************************************/
#define	AIO_FB_MEM_M_REGION	(1)
#define	AIO_FB_MEM_S_REGION	(2)


/*********************************************************************
**	
**	CPU 보드위에 장착되는 광 통신 PMC모듈을 Access하는 FB
**
**********************************************************************/
#define	PMC502_RD_CODE	        304
#define	PMC502_RD_SPEC_NUM	    5
#define	PMC502_RD_VAR_NUM	    0
#define	PMC502_RD_OUTPUT_NUM	2
		
typedef struct {		
	/* 스펙 */	
    int32 opticPortId; 				/* 포트 2개( 0 ~ 1), -1 : 미사용 */ 
    int32 readDataType;				/* 1:Byte, 2:Word, 3:double Wrod */
    int32 saveMemKindOfReadData;	/* 1:M영역, 2:S영역 */
    int32 startAddrOfMem;
    int32 numOfReadData;
	
	/* 내부변수 */	
		
	/* 출력 */	
    int32 accErrId;					/* -1:Port범위에러, 0:정상, 1:모듈 미장착, 2:링크에러, 3:프레임에러 */
    int32 readComSeqNum;  
} strFC0304Info;		

/* 광 Port Range */
#define	MIN_PMC502_PORT_ID	0
#define	MAX_PMC502_PORT_ID	1

/* FB Error Type */
#define	PMC502_INPUT_PARA_ERR	-1
#define	PMC502_NORMAL			0
#define	PMC502_NOT_INSTALL		1
#define	PMC502_LINK_ERR			2
#define	PMC502_FRAME_ERR		3

/* read할 수 있는 최대 데이터 갯수 */
#define	MAX_PMC502_READ_DATA_NUM	128	/* 최대 512 바이트 = 128 * 4 */

/* Optic 통신 Start Code */
#define	PMC502_COM_START_CODE	0xaabbccdd

/*********************************************************************
**	
**	시스템 진단 정보를 HMI(CIMON)이 Modbus를 통해 읽어갈 수 있도록 특정 메모리에 Write
**
[주의 사항]
	M영역의 주소는 5000 ~ 9000으로 고정 됨

[추후 구현]
	FB Code를 다시 구현 해야 함
**********************************************************************/
#define	SYSCFG_CODE			401
#define	SYSCFG_SPEC_NUM		8
#define	SYSCFG_VAR_NUM		0
#define	SYSCFG_OUTPUT_NUM	8
		
typedef struct 
{		
	/* 스펙 */	
    int32 CPU_I;
    int32 AD_I;
    uint32 DI_I;
    uint32 DO_I;
    uint32 AI_I;
    uint32 CC_I;
    uint32 VC_I;
    uint32 ODM_I;
	
	/* 내부변수 */	

		
	/* 출력 */	
    int32 CPUE_0;
    int32 DLEN_0;
    int32 DIE_0;
    int32 DOE_0;
    int32 AIE_0;
    int32 CCE_0;
    int32 VCE_0;
    int32 ODME_0;
} strFC0401Info;		

#endif
