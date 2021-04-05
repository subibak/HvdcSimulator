
#ifndef DEBUGINFO_H
#define DEBUGINFO_H

#ifdef _WIN32
#define	MAX_NAME_LEN	128		
#define	FILE_NAME_LEN	128		
#else
#define	MAX_NAME_LEN	72 
#define	FILE_NAME_LEN	64  

#endif

#define	MAX_ERROR_STRING_LEN	128
#define	MAX_ERROR_DETAIL_NUM	3

#define	MAX_ERR_QUEUE_NUM	128

typedef struct {
    strDoubleLink link;
    uint8 	fileName[FILE_NAME_LEN];
    uint32	lineNumber;
    uint8 	moduleName[MAX_NAME_LEN];
    uint32  errorCode;
#ifdef _WIN32
	uint32	tickCountOfErrOccur;
#endif
}strErrorInfo;

typedef struct{
	uint8	errString[MAX_ERROR_STRING_LEN];
    uint32	refVal;
}strErrorValInfo;

typedef struct {
	strErrorInfo		errInfo;
    strErrorValInfo		errValInfo[MAX_ERROR_DETAIL_NUM];
}strDetailErrorInfo;
    
typedef struct {
    strErrorInfo    *freeList;
    strErrorInfo    *usedList;
}strErrQueMngrInfo;

typedef struct {
    strDetailErrorInfo    *freeList;
    strDetailErrorInfo    *usedList;
}strDetailErrQueMngrInfo;


#define DEBUG_INFO_MAX_ARRAY_NUM    128
typedef struct {
    strinetAddr peerAddr;
    uint32  seqNum;
    int32   msgDataSize;
    int32   transTotalCount;
    uint32	opCode;
    uint32	optionCode;
    int32   transErrCount;
}strDebugDataInfo;


typedef struct {
    strDoubleLink   link;
    strinetAddr     peerAddr;
    uint32          totalBytesLen; 
    uint8	dataBuff[MAX_DATA_LEN]; 
}strTransDetailInfo;


#define TRANS_DETAIL_INFO_QUE_NUM   128

typedef struct {
    strTransDetailInfo     *freeList;
    strTransDetailInfo     *usedList;
}strTransDetailQueMngrInfo;


typedef struct {
    int8    opCode[32];
    int8    optionCode[32];
}strCommandListInfo;


typedef struct {
    uint32  reqMsgRecvCount;
    uint32  respMsgSendCount;
    uint32  broadcastMsgRecvCount;
    uint32  broadcastMsgSendCount;
}strNetDebugCountInfo;


#define	DEBUG_DISABLE		(0x0)
#define	DEBUG_ENABLE		(0x1)


#define	SEND_PACKET_F	(0x1)
#define	RECV_PACKET_F	(0x2)


#define	UNICAST_HEAD_PRINT			(0x1)
#define	UNICAST_DETAIL_DATA_PRINT 	(0x2)
#define	BROADCAST_HEAD_PRINT			(0x3)
#define	BROADCAST_DETAIL_DATA_PRINT 	(0x4)
#define	BROADCAST_DETAIL_XR_DATA_PRINT 	(0x5)

#define DETAIL_DATA_FOR_RECV (0x1)
#define	DETAIL_DATA_FOR_SEND (0x2)


#define MAX_HOST_NUM    32

typedef struct {
    int8    hName[32];
    int8    iAdrs[32];
}strHostsInfo;

typedef struct {
    int32   curIndex;
    strHostsInfo    host[MAX_HOST_NUM];
}strHostTblInfo;


typedef struct {
	uint32	xrTaskSchdCount;
    uint32	elapsedTime;
    uint32	maxSchdTime;
}strXRTaskSchdDebugInfo;

#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)
typedef struct {
	uint32	dualRmtTaskSchdCount;
    uint32	elapsedTime;
    uint32	maxSchdTime;
}strDualRmtTaskSchdDebugInfo;
#endif 

typedef struct {
	uint32	cyclicSchdCount;
    uint32	cyclicOvertimeCount;
    uint32	cyclicElapsedTime;
    uint32	cyclicMaxSchdTime;
	uint32	nonCyclicSchdCount;
    uint32	nonCyclicElapsedTime;
}strFBTaskSchdDebugInfo;

typedef struct {
	uint32	ioScanTaskSchdCount;
    uint32	elapsedTime;
    uint32	maxSchdTime;

}strIOScanTaskSchdDebugInfo;

typedef struct {
	uint32	tmtcTaskSchdCount;
    uint32	elapsedTime;
    uint32	maxSchdTime;
}strTMTCTaskSchdDebugInfo;


typedef struct {
	uint32	fieldbusTaskSchdCount;
    uint32	elapsedTime;
    uint32	maxSchdTime;
}strFIELDBUSTaskSchdDebugInfo;

typedef struct {
	uint32	schdTimeBase;
	strFBTaskSchdDebugInfo		fbTaskSchdDebugInfo;
	strIOScanTaskSchdDebugInfo	ioScanTaskSchdDebugInfo;
	strXRTaskSchdDebugInfo		xrTaskSchdDebugInfo;
}strSysTaskSchedDebugInfo;


typedef struct {
	uint32	totalSendCount;
    uint32	byteLenOfSendingPacket;
    uint32	totalRecvCount;
    uint32	byteLenOfRecvingPacket;
}strNetworkPerInfo;



#define MAX_FATAL_ERR_NUM      128

#define	FATAL_MODULE_NAME_LEN	16
#define	FALTA_FILE_NAME_LEN		16
typedef struct fatal_err_msg{
	int8	fileName[FALTA_FILE_NAME_LEN];
    uint32	lineNum;
    int8    moduleName[FATAL_MODULE_NAME_LEN];
    uint32  fatalErrorNum;
    uint32  totalSecTime;
    uint32	vmeAddr;
}strFatalErrMsgInfo;

typedef struct {
    uint32  firstFlag;
    uint32  firstSecTime;
    uint32	curIndex; 
}strFatalErrLogInfo;

#define FATAL_ERR_LOG_SIZE	(sizeof(strFatalErrLogInfo)+\
							(sizeof(strFatalErrMsgInfo)* MAX_FATAL_ERR_NUM))



typedef struct {
	
	uint32	recvSuccCount;
    uint32  recvErrCount;
    uint32  checkSumErrCount;
    uint8	dualStatus;     
    uint16  dpuConfigStatus;
    uint16  serialConStatus;
    uint16  telReqStatus;   
    uint16  modemLineStatus;
	
	
   	uint32	sendSuccCount;
    uint32  sendErrCount;
    uint8   tmsDualStatus;      
    uint16  tmsShelfStatus;     
                                
    uint8   serialConChange;    
    uint16  telLineRequest;     
}strDpuDebugInfo;


typedef struct {
	uint32	rmtDevStatusReadComSendCount;
	uint32	rmtDevStatusReadComRecvCount;

	uint32	rmtDevDataReadComSendCount;
	uint32	rmtDevDataReadComRecvCount;

	uint32	rmtDevDataWriteComSendCount;
	uint32	rmtDevDataWriteComRecvCount;
}strRmtComCountDisplayInfo;


typedef struct {
	uint32	tmtcRequestCount;
	uint32	tmtcRequestErrCount; 
	uint32	tmtcSendCount;
	uint32	tmtcSendErrCount;	
	uint32	tmtcRecvCount;
	uint32	tmtcRecvErrCount;	
	uint32	tmtcFbTimeoutCount;
	uint32	tmtcRS232RecvTimeoutCount;
}strTmtcComDebugInfo;


typedef struct {
	uint32	stepCount_1;
	uint32	stepCount_2;
	uint32	stepCount_3;
	uint32	stepCount_4;
	uint32	stepCount_5;
	uint32	stepCount_6;
	uint32	stepCount_7;
	uint32	stepCount_8;
	uint32	stepCount_9;
	uint32	stepCount_10;
	uint32	stepCount_11;
}strTmtc232CommTaskDebugInfo;


#define	TMTC_SEND_FRAME			0
#define	TMTC_RECV_FRAME			1
#define TM_MAX_SERIAL_BUF		1072
typedef struct {
	uint32		portId;
	uint32		frameKind;			
	strinetAddr	etosIpAddr;			
	uint32		cmdStringIndex;
	uint32		dataLength;
	uint8		dataBuf[TM_MAX_SERIAL_BUF];
} strTmtcSendAndRecvDebugDataInfo;

#define RAS_CNF_CHANGE_SET 1
#define RAS_MPU_CHANGE_SET 2
#define RAS_FBC_CHANGE_SET 3
#define RAS_DIU_CHANGE_SET 4
#define RAS_TMTC_CHANGE_SET 5
#define RAS_FMC_CHANGE_SET 6
#define RAS_TMTC_CHANGE_SET_PRINT 7
#define RAS_FMC_CHANGE_SET_PRINT 8


typedef struct {
	struct {
		uint32 sndOkCnt;
		uint32 sndErrCnt;
		uint32 rcvOkCnt;
		uint32 rcvErrCnt;
		uint32 busyAccCnt;
	} hmiToMpu;
	struct {
		uint32 sndOkCnt;
		uint32 sndErrCnt;
		uint32 rcvOkCnt;
		uint32 rcvErrCnt;
		uint32 timeoutCnt;
	} mpuToFbc;
	uint32 sendTick;
	uint32 recvTick;
	uint32 respTime;
} strHartCommDebugInfo;


typedef struct {
	uint32	sendCnt;
	uint32	sendErrCnt; 	
	uint32	sendFlagErrCnt; 
	
	uint32	recvCnt;
	uint32	recvTimeoutCnt; 
	uint32	recvErrCnt;		
	uint32	recvFramErrCnt;	
}strTmtc232ComSioBrdDebugInfo;

/***************************************************
**	디버그 타스크에서 Command 명령에 따른 함수 Mapping 구조체
***************************************************/
typedef struct {
	int8	dbgCmdStr[32];
	uint32	arg1;
	uint32	arg2;
	void	(*dbgFunc)(uint32, uint32);
}strDbgCmdFuncInfo;

#endif 
