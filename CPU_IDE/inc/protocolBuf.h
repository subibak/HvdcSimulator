
#ifndef PROTOCOLBUF_H
#define PROTOCOLBUF_H

#include "pcsDefs.h"

#include "memDef.h"
typedef struct {
	uint32	taskId;
	uint32	excutionMode;
    uint32	excutionID;
	uint32  taskPriority;				
	uint32  eventTasksetAction;			
	uint32  eventTasksetAddress;		
	uint32	paraMemSize;				
}strSegTblWriteReqInfo;

typedef struct {
	uint32	taskId;
    uint32	status;
}strSegTblWriteRespInfo;

typedef struct {
	uint32	cmdCode;
	uint32	memoryKind;
    uint32	segmentNum;
}strFbDataGrHeaderInfo;

typedef struct{
	uint8	pathType;
	uint8	pathAddr;
}strPathInfo;

typedef struct {
	uint32	fileId;

    uint16	year;
    uint8	month;
    uint8	day;

    uint8	hour;
    uint8	minute;
    uint8	second;
    uint8	reserved;

    uint16	totalPathNum;
    uint16	pathCounter;

	strPathInfo	pathInfo[MAX_PUMP_DATA_TRANS_PATH];

    uint16	fbBlockAddr;
    uint16	timeTblByteLen;
	uint32	dTimeTblData[MAX_PUMP_TIME_TBL_DATA];
}strPumpTimeTblDwlReqInfo;

typedef struct {
	uint16	fileId;
    uint16	status;
}strPumpTimeTblDwlRespInfo;



typedef struct {
	uint32	cmdCode;
	uint32	memoryKind;
    uint32	startIndex;
	uint32	byteSize;
	uint32	endFlag;
}strFieldbusEngDwlHeaderInfo;


typedef struct {
	uint16	year;				uint8	month;	uint8	date;
    uint8 hour;	uint8 minute; 	uint8	second; uint8	msecond;
}strSysTimeRasDetailInfo;


typedef struct {
	uint32	moduleRasSummary;
    uint32	liveCount;
    uint32	version;
    uint16	year;	uint8 month; uint8 date;
}strSysModuleRasDetailInfo;




#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
typedef struct {							
	uint32      masterStatus;				  
	uint32      detailError;				
	uint32		romVersion;					
	uint32      releaseDate;				
	uint32      fmsStatus[16];				
	uint32		slaveDiag[62];				
											
											  
}strMpuProfibusRasSummaryInfo; 				
											  
											
typedef struct {							
	uint32		profiShelfRasSummary;		    
	uint32		scanCmdStatus;				  
	uint32		pcuStatus;					  
	uint32		fmcBoardStatus[6];			  
                							
                							 
                							
	uint32		fmcDualRunInfo;				
											
	uint32		slvDiagCount[12];			
											
											
}strIoShelfProfibusRasSummaryInfo;			

typedef struct {
	union {
		strMpuProfibusRasSummaryInfo		mpuProfibusRasInfo;
		strIoShelfProfibusRasSummaryInfo	ioShelfProfibusRasInfo[4];
	}u;
}strFieldbusRasSummaryInfo;
#else
typedef struct {							
	uint32      masterStatus;				  
	uint32      detailError;				
	uint8		romVersion[8];				
									 		
	uint32		slaveStatus[62];			  
											
}strFieldbusRasSummaryInfo;					  
#endif 

typedef struct {							
	uint32      etosDpuStatus;				  
	uint32      portComStatus;				
	uint32		romVersion;					
	uint32      releaseDate;				
	uint32		portStaus[32];				
											
}strTmtcRasSummaryInfo;						
					 						
typedef struct {							
	uint32      rasSummary;					
	uint32      liveCount;					
	uint32		romVersion;					
	uint32      releaseDate;				
}strMpuFbcRasHeaderInfo;					

typedef struct {							
	uint32		ioShelfRasSummary;			    
	uint32		scanCmdStatus;				  
	uint32		dcuStatus;					  
	uint32		ioBoardStatus[6];			  
                							
                							 
                							
#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
	uint32		ioDualRunInfo;				
                							
	uint32		ioCardDiag[3];				  
#endif
}strIORasSummaryInfo;						
											  
											
											  
											




typedef struct {							
	strMpuFbcRasHeaderInfo fbcRasSummary;	    
	strIORasSummaryInfo ioRasSummary[MAX_IO_SHELF_NUM];	
											  
											
}strFbcRasSummaryInfo;						  
											

typedef struct {							
	strMpuFbcRasHeaderInfo mpuRasSummary;	    
	uint32      mpuTaskStatus[32];			
											
}strMpuRasSummaryInfo;						
					 						


#define RAS_PROTOCOL_MP5000		(1)
#define	RAS_PROTOCOL_MP3000AT	(2)
#define	RAS_PROTOCOL_HVDC		(3)
#define	RAS_PROTOCOL_MP5000NEW	(4)

typedef struct {							
	uint32		opCommand;					
	uint32      YearMonthDay;				
	uint32      HourMinSecmSec;				
	uint32		dualStatus;					
	uint32		forcingStatus;				
	uint32      lineCount;					
	uint32		rcsHwCnfg;					
	uint32		reserved[7];				
											
}strRasHeadInfo;							
											
						
typedef struct {							
	strRasHeadInfo       rasheadInfo;		
	strMpuRasSummaryInfo mpuRasInfo;		
	strFbcRasSummaryInfo fbcRasInfo;		
	strTmtcRasSummaryInfo tmtcRasInfo;		
	strFieldbusRasSummaryInfo				
					  fieldbusRasInfo;		
}strNewRasSummaryInfo;					 


#define	MAX_IO_POINTER_RAS_SIZE	(12 * 5)
#define	IO_POINTER_STATUS_RAS_SIZE	5

typedef struct {
	uint16	ioBrdShelfId;
	uint16	ioBrdSlotId;
	uint16	ioBrdType;
	uint16	ioBrdPointId;
}strSysRasDetailNewReqInfo;


#define	ETH_RMT_COM_RCV_DONE	(2)

#define	LK11_CPU	(0x1)

#define	MP_5000_ETH_CODE_FUNC	(0x0)
#define	MP_3000_ETH_CODE_FUNC	(0x10000000)
#define	ETH_DEST_ADDR_MASK		(0x0fffffff)
#define	ETH_FB_CODE_MASK		(0x10000000)


#define	RMT_BYTE_ACC	(1)
#define	RMT_WORD_ACC	(2)
#define	RMT_LONG_ACC	(3)
#define	RMT_DLONG_ACC	(4)

#define	RMT_BIT_INPUT_ACC	(5)
#define	RMT_BIT_OUTPUT_ACC	(6)
#define	RMT_WORD_INPUT_ACC	(7)
#define	RMT_WORD_OUTPUT_ACC	(8)


#define	MAX_RMT_ST_ACC_DATA_NUM		200		
#define	MAX_SHARED_ACC_DATA_NUM		200

															 	
typedef struct {
	uint32	destLineAIpAddr;
    uint32	destLineBIpAddr;
    uint32	srcFbBlockAddr;
    uint32	destFbBlockAddr;
    uint32	deviceType;		
    uint32	dataAccType; 	
    uint32	numOfData;
}strEthDataReadReqInfo;

typedef struct {
	uint32	destLineAIpAddr;
    uint32	destLineBIpAddr;
    uint32	srcFbBlockAddr;
    uint32	destFbBlockAddr;
    uint32	deviceType;		
    uint32	dataAccType; 	
    uint32	numOfData;
    uint32	readData[MAX_RMT_ST_ACC_DATA_NUM];
}strEthDataReadRespInfo;

typedef struct {
	uint32	destLineAIpAddr;
    uint32	destLineBIpAddr;
    uint32	srcFbBlockAddr;
    uint32	destFbBlockAddr;
    uint32	deviceType;		
    uint32	dataAccType; 	
    uint32	numOfData;
    uint32	writeData[MAX_RMT_ST_ACC_DATA_NUM];
}strEthDataWriteReqInfo;

typedef struct {
	uint32	destLineAIpAddr;
    uint32	destLineBIpAddr;
    uint32	srcFbBlockAddr;
    uint32	destFbBlockAddr;
    uint32	deviceType;		
    uint32	dataAccType; 	
    uint32	numOfData;
    uint32	accStatus;
}strEthDataWriteRespInfo;


#define	NET_LINE_LINK_ERR	(0)
#define	NET_SINGLE_LINE		(1)
#define	NET_DUAL_LINE		(2)


#define	NET_LINE_LINK_A		(1)
#define	NET_LINE_LINK_B		(2)
#define	NET_LINE_LINK_DUAL	(3)


#define	SYSTEM_SHELF_SINGLE_STATUS	(1)
#define	SYSTEM_SHELF_DUAL_STATUS	(2)


#define	RMT_DEV_RUN_ERR		(0)
#define	RMT_DEV_MASTER_RUN	(1)
#define	RMT_DEV_SLAVE_RUN	(2)

															 	
typedef struct {
	uint32	destLineAIpAddr;
    uint32	destLineBIpAddr;
    uint32	srcFbBlockAddr;
    uint32	deviceType;		
}strEthCommPathReadReqInfo;

typedef struct {
	uint32	destLineAIpAddr;
    uint32	destLineBIpAddr;
    uint32	srcFbBlockAddr;
    uint32	deviceType;		
    uint32	lineStatus;		
    uint32	commLine;		
    uint32	redundancyStatus;
    uint32	dualStatus;		
}strEthCommPathReadRespInfo;

															 	
typedef struct {
	uint32	lineAIpAddr;
    uint32	lineBIpAddr;
    uint32	fbBlockAddr;
    uint32	deviceType;
}strEthCommonInfo;

															 	
typedef struct {
	uint32	destFbBlockAddr;
    uint32	dataAccType;
	uint32	numOfData;
}strEthDataAccCommonInfo;


#define	LOCAL_PLC_PROTOCOL_HEAD_LEN	(44)

typedef struct {
	uint32	frameLength;		
    uint32	sequenceNum;
    uint32	sourceStationId;	 
    uint32	destStationId;
    uint32	msgType;			  
    uint32	msgStatus;
    uint32	dualId;
    uint32	moduleId;			
    uint32	opCode;
    uint32	optionCode;
    uint32	itemNo;
}strLocalPlcHeadInfo;

typedef struct {
	strLocalPlcHeadInfo headData;
    uint8	dataBuff[MAX_NET_MSG_LEN - LOCAL_PLC_PROTOCOL_HEAD_LEN];
}strLocalPlcProtocolInfo;

															 	
typedef struct {
	uint32	blockAddr;
}strHighSpeedTrendStatusReqInfo;

typedef struct {
	uint32	blockAddr;
	uint32	status;
}strHighSpeedTrendStatusRespInfo;



#define	MAX_FB_SIM_STATUS_REQ_NUM	(400) 

typedef struct {
#ifdef VXWORKS
	uint16	simFbCode;
	uint16	simFbBlockAddr;
#else
	uint16	simFbBlockAddr;
	uint16	simFbCode;
#endif
}strFbSimStatusInfo;

typedef struct {
	
	
	uint32	numOfSimFbCode;
#ifdef VXWORKS
	uint16	numOfReplyFbCode;
	uint16	numOfRemainFbCode;
#else
	uint16	numOfRemainFbCode;
	uint16	numOfReplyFbCode;
#endif
	strFbSimStatusInfo	fbSimInfo[MAX_FB_SIM_STATUS_REQ_NUM]; 
}strFbSimStatusRespInfo;



#define	ONLINE_DWL_OPERATION_START	(0x1)
#define	ONLINE_DWL_OPERATION_STOP	(0x2)


#define	MAX_FB_LIST_NUM	MAX_FB_EXEC_SEQ_ID_NUM
typedef struct {
	uint32	numOfFbCode;
	uint32	startParaIndex;
	uint32	endParaIndex;
	uint32 	fbCodeBlkAddr[MAX_FB_LIST_NUM];
}strFbCodeAddrListInfo;



#define	ONLINE_DWL_FBCODE_ADD			0x1

#define	ONLINE_DWL_FBCODE_DEL			0x2

#define	ONLINE_DWL_FBCODE_SPEC_CHANGE	0x3

#define	ONLINE_DWL_FBCODE_SEQ_ID_ADD_AND_CHG	0x4

#define ONLINE_DWL_VAR_INFO_CHANGE		0x5

#define ONLINE_DWL_TRS_DATA_CHANGE		0x6

#define ONLINE_DWL_FBCODE_CHANGE		0x7

#define	ONLINE_DWL_IO_SPEC_CHANGE		0x8


#define	MAX_ONLINE_DWL_KIND				ONLINE_DWL_FBCODE_CHANGE 


#define	ONLINE_FB_PARA_REALLOC			0x9


#define	ONLINE_FB_LOGIC_UPDATE			0xa

#define	MAX_ONLINE_DWL_OPERATION_NUM	ONLINE_FB_PARA_REALLOC	




#define	MAX_FB_SPEC_PERMISSION_NUM	128

#define	MAX_FB_INPUT_HEAD_PERMISSION_NUM	128
#define	MAX_FB_INPUT_DATA_PERMISSION_NUM	256


#define	MAX_FB_INPUT_PERMISSION_NUM	(MAX_FB_INPUT_HEAD_PERMISSION_NUM+MAX_FB_INPUT_DATA_PERMISSION_NUM)


#if (BOARD_TYPE == MVME2100_BOARD)
#define MAX_ONLINE_DWL_FB_CODE_NUM		100 
#define MAX_ONLINE_DWL_SEG_NUM			  3
#else
#define MAX_ONLINE_DWL_FB_CODE_NUM		200  
#define MAX_ONLINE_DWL_SEG_NUM			  7
#endif

#define	MAX_ONLINE_DWL_FB_CODE_ADD_NUM	MAX_ONLINE_DWL_FB_CODE_NUM



#define	MAX_ONLINE_DWL_SEG_ADD_NUM	MAX_ONLINE_DWL_SEG_NUM


typedef struct {
	uint16	fbBlkAddr;
	uint16	fbId;
	uint32	numOfSpec;
	uint32  numOfSize;
	uint32	specData[MAX_FB_INPUT_PERMISSION_NUM];
	uint16	memReservedFlag; 	
								
								
								
								
								
								
								
	
	uint16  paraRunningType;
	uint16  logicType;
	uint16  usingType;
	uint16  ActionID;
	uint16  TransitionID;
	uint32  outputAction[2];
	uint32  taskId;  

}strOnlineDwlAddFbCodeInfo;


typedef struct {
	uint16	taskId;
	uint16	numOfFbCode;
	uint32  logicType;
	strOnlineDwlAddFbCodeInfo	addFbCodeInfo[MAX_ONLINE_DWL_FB_CODE_ADD_NUM];
}strOnlineDwlAddSegInfo;


#define	MEM_RESERVED_OFF	0
#define	MEM_RESERVED_ON		1
typedef struct {
	uint16	numOfTotalSegment;
	uint16	numOfTotalFbCode;
	
	
	uint16	dwlTotalFrameNum; 
	uint16	dwlCurFrameNum;
	uint32	dwlTotalFbCodeNum;
	
	strOnlineDwlAddSegInfo	*addSegInfoPtr;
}strOnlineDwlAddFbCodeMngrInfo;



#define	MAX_ONLINE_DWL_FB_CODE_DEL_NUM	MAX_ONLINE_DWL_FB_CODE_NUM



#define	MAX_ONLINE_DWL_SEG_DEL_NUM	MAX_ONLINE_DWL_SEG_NUM


typedef struct {
	uint16	fbBlkAddr;
	uint16	fbId;
	uint16  logicType;
	uint16  usingType;
	uint16  ActionID;
	uint16  TransitionID;
	uint32  taskId;  
}strOnlineDwlDelFbCodeInfo;


typedef struct {
	uint16	taskId;
	uint16	numOfFbCode;
	uint32  logicType;
	strOnlineDwlDelFbCodeInfo	delFbCodeInfo[MAX_ONLINE_DWL_FB_CODE_DEL_NUM];
}strOnlineDwlDelSegInfo;


typedef struct {
	uint16	numOfTotalSegment;
	uint16	numOfTotalFbCode;

	
	uint16	dwlTotalFrameNum; 
	uint16	dwlCurFrameNum;
	uint32	dwlTotalFbCodeNum;

	strOnlineDwlDelSegInfo	*delSegInfoPtr;
}strOnlineDwlDelFbCodeMngrInfo;



typedef struct {
	uint32	fbId;
	uint32	fbBlkAddr;
	uint32	totalSpecMemNum;
	uint32	usedSpecMemNum;
	uint32	usableSpecMemNum;

	uint32	totalOutputMemNum;
	uint32	usedOutputMemNum;
	uint32	usableOutputMemNum;

}strOnlineDwlDelFbMemInfo;


typedef struct {
	uint32	numOfFbCode;
	strOnlineDwlDelFbMemInfo	delFbMemInfo[MAX_ONLINE_DWL_FB_CODE_DEL_NUM*MAX_ONLINE_DWL_SEG_DEL_NUM];
}strOnlineDwlDelSegMemInfo;

typedef struct {
	uint32	numOfTotalSegment;
	strOnlineDwlDelSegMemInfo 	delSegMemInfo;
}strOnlineDwlDelFbMemMngrInfo;



#define	MAX_ONLINE_DWL_FB_SPEC_CHANGE_NUM	MAX_ONLINE_DWL_FB_CODE_NUM

 

#define	MAX_ONLINE_DWL_SEG_SPEC_CHANGE_NUM	MAX_ONLINE_DWL_SEG_NUM


typedef	struct {
	uint32	specIndex;	
	uint32  specType;
	uint32  specActionData;
	uint32	specData[2];
}strOnlineDwlChgSpecInfo;

typedef struct {
	uint16	fbId;
	uint16	fbBlkAddr;
	uint32	numOfChangedSpec;
	strOnlineDwlChgSpecInfo	specDataInfo[MAX_FB_INPUT_PERMISSION_NUM];
	uint16  logicType;
	uint16  usingType;
	uint16  ActionID;
	uint16  TransitionID;
	uint32  taskId; 
    uint32  paraRunningType;
}strOnlineDwlSpecChgFbCodeInfo;


typedef struct {
	uint16	taskId;
	uint16	numOfFbCode;
	uint32  logicType;
	strOnlineDwlSpecChgFbCodeInfo	specChgFbCodeInfo[MAX_ONLINE_DWL_FB_SPEC_CHANGE_NUM];
}strOnlineDwlSpecChgSegInfo;


typedef struct {
	uint16	numOfTotalSegment;
	uint16	numOfTotalFbCode;

	
	uint16	dwlTotalFrameNum; 
	uint16	dwlCurFrameNum;
	uint32	dwlTotalFbCodeNum;

	strOnlineDwlSpecChgSegInfo	*specChgSegInfoPtr;
}strOnlineDwlSpecChgFbCodeMngrInfo;



#define	MAX_ONLINE_DWL_FB_SEQ_ID_ADD_AND_CHG_NUM	MAX_FB_EXEC_SEQ_ID_NUM 

 

#define	MAX_ONLINE_DWL_SEG_SEQ_ID_ADD_AND_CHG_NUM	MAX_ONLINE_DWL_SEG_NUM


typedef struct {
	uint16	fbBlkAddr;
	uint16	fbId;
	uint32	fbSeqId;
}strOnlineDwlFbSeqIdInfo;


typedef struct {
	uint32	taskId;
	uint32	numOfFbCode;
	uint32	startIndex;	
}strOnlineDwlFbSeqIdAddAndChgSegInfo;



#define	MAX_MPU_SEQ_ID_NUM	20000
typedef struct {
	
	uint32 	numOfTotalSegment;
	uint32	numOfTotalFbCode;
	
	uint32	dwlTotalFbCodeNum;
	uint32	dwlTotalFrameNum;
	uint32	dwlCurFrameNum;
	
	strOnlineDwlFbSeqIdAddAndChgSegInfo	fbSeqIdAddAndChgSegInfo[MAX_ONLINE_DWL_SEG_SEQ_ID_ADD_AND_CHG_NUM];
	strOnlineDwlFbSeqIdInfo	fbSeqIdInfo[MAX_MPU_SEQ_ID_NUM+1];
}strOnlineDwlFbSeqIdAddAndChgMngrInfo;


#define	MAX_FB_EXEC_SEQ_ID	MAX_FB_EXEC_SEQ_ID_NUM 	
													
typedef struct {
	uint32	fbExecId;
	uint16	fbBlkAddr;
	uint16	fbId;
}strFbExecIdReqInfo;

typedef struct {
	uint16	fbBlkAddr;
	uint16	fbId;
	uint32	status;
}strFbExecIdRespInfo;


#define	FB_ONLINE_LOGIC_UPDATE	(0x1)
#define	FB_ONLINE_LOGIC_CANCEL	(0x2)	
#define	IO_ONLINE_LOGIC_UPDATE	(0x3)
#define	IO_ONLINE_LOGIC_CANCEL	(0x4)
	

typedef struct {
	uint32	recvTotalFbCodeNum;
	uint32	sumOfDwlFbCodeNum;
}strFbSeqIdDwlInfo;


typedef struct {
	uint32	fbLogicCompileTime;
	uint32	reserved[2];
}strFbLogicVersionInfoWriteReqInfo;

typedef struct {
	uint32	fbLogicCompileTime;
	uint32	reserved[2];
}strFbLogicVersionInfoReadRespInfo;


#define	MAX_FB_SPEC_FRAGMENT_NUM	200
typedef struct {
	uint32	fragmentMemEndIndex;
	uint32	fragmentMemStartIndex;
}strFbParaMemStatusInfo;
	
typedef struct {

	uint32  inoutType;
	uint32	usedMemSize; 	
	uint32	freeMemSize;	
	uint32	fragmentMemNum;
	strFbParaMemStatusInfo	fbParaMemStatusInfo[MAX_FB_SPEC_FRAGMENT_NUM];
}strFbSpecMemStatusReadRespInfo;




#define	MAX_ONLINE_DWL_VAR_INFO_CHG_NUM	1000


typedef struct {
	uint32	varRetainIndex;
	uint16	memKind;
	uint16	retainData;
}strOnlineDwlVarRetainInfo;

typedef struct {
	uint32	varInitType;
	uint32	varInitIndex;
	uint32	varInitData[2];
	uint32  intlVarSize;
}strOnlineDwlVarInitInfo;


typedef struct {
	uint16	numOfTotalVarRetainNum;
	uint16	numOfTotalVarInitNum;
	
	
	uint16	dwlTotalFrameNum; 
	uint16	dwlCurFrameNum;

	uint16  dwlnumOfTotalRetainNum;
	uint16  dwlnumOfTotalInitNum;
	
	strOnlineDwlVarRetainInfo	chgVarRetainInfo[MAX_ONLINE_DWL_VAR_INFO_CHG_NUM];
	strOnlineDwlVarInitInfo		chgVarInitInfo[MAX_ONLINE_DWL_VAR_INFO_CHG_NUM];
}strOnlineDwlVarInfoChgMngrInfo;


#define	MAX_ONLINE_DWL_TRS_CHG_NUM	MAX_ONLINE_DWL_FB_CODE_NUM
#define	MAX_ONLINE_DWL_SEG_TRS_CHANGE_NUM	MAX_ONLINE_DWL_SEG_NUM


typedef struct {
	uint16  trsID;
	uint16  taskId;
	uint32	setActiveHeader;
	uint32  setActiveData;
}strOnlineDwlChgTrsInfo;


typedef struct {
	uint16	taskId;
	uint16	numOfTransition;
	uint32  logicType;
	strOnlineDwlChgTrsInfo	chgTrsDataInfo[MAX_ONLINE_DWL_TRS_CHG_NUM];
}strOnlineDwlChgTrsSegInfo;


typedef struct {
	uint16	numOfTotalSegment;
	uint16	numOfTotalTrs;
	
	
	uint16	dwlTotalFrameNum; 
	uint16	dwlCurFrameNum;
	uint32	dwlTotalTrsNum;
	
	strOnlineDwlChgTrsSegInfo	*chgTrsInfoPtr;
}strOnlineDwlChgTrsDataMngrInfo;


typedef struct {
	uint32	taskID;
	uint32  taskRunMode;
	uint32  maxExecTime;			
	uint32  curExecTime;			
	uint32  usedFbCodeNum;
	uint32  usedInputMem;
	uint32  usedOutputMem;
}strTaskStatusReadInfo;

#define MAX_TASK_INFO_NUM	64     

#ifdef VXWORKS
#define COMPANY_ID "LEKLNEVE"	   /* 53 49 53 4c 53 43 44 2d SISLSCD- */
#else
#define COMPANY_ID "LKELEVEN"        
#endif



#define	MAX_ONLINE_DWL_FB_INFO_CHANGE_NUM	MAX_ONLINE_DWL_FB_CODE_NUM

 

#define	MAX_ONLINE_DWL_SEG_FBINFO_CHANGE_NUM	MAX_ONLINE_DWL_SEG_NUM

typedef struct {
	uint16	fbId;
	uint16	fbBlkAddr;
	uint32  outputAction[2];
}strOnlineDwlFbInfoChgFbCodeInfo;


typedef struct {
	uint16	taskId;
	uint16	numOfFbCode;
	strOnlineDwlFbInfoChgFbCodeInfo	fbInfoChgFbCodeInfo[MAX_ONLINE_DWL_FB_INFO_CHANGE_NUM];
	uint32  logicType;
}strOnlineDwlFbInfoChgSegInfo;


typedef struct {
	uint16	numOfTotalSegment;
	uint16	numOfTotalFbCode;

	
	uint16	dwlTotalFrameNum; 
	uint16	dwlCurFrameNum;
	uint32	dwlTotalFbCodeNum;

	strOnlineDwlFbInfoChgSegInfo	*fbInfoChgSegInfoPtr;
}strOnlineDwlFbInfoChgFbCodeMngrInfo;

typedef struct {
	uint32	cmdCode;
	uint32	dcsInfo;
	uint32	reqStatus;
	uint32  reserved;
}strOpenProtoCommDataInfo;

#endif 
