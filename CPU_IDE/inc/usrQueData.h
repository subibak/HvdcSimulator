
#ifndef USRQUEDATA_H
#define USRQUEDATA_H


#include "memDef.h"
typedef struct double_link {
    struct double_link  *prev;
    struct double_link  *next;
}strDoubleLink;


#define MAX_DLM_LIVE_COUNT      128
#define MAX_DLM_QUE_NUM         512

typedef struct {
    strDoubleLink   link;
    uint32  dLMsgLiveCount;
    uint32  dLMsgIpAddr;
    uint32  dLMsgSeqNum;
    uint32  dLMsgSrcId;
    uint32  dLMsgDestId;
    uint32  dLMsgOpCode;
    uint32  dLMsgOptionCode;
}strDualLineMsgInfo;

typedef struct {
    strDualLineMsgInfo  *freeList;
    strDualLineMsgInfo  *usedList;
}strDLMsgInfoMngr;


#define	MAX_PORTINFO_QUE_NUM 512
typedef struct {
    strDoubleLink   link;
    uint32  mngrCount;
    uint32  recvSockFd;
    uint32  recvPortNum;
    uint32  recvIpAddr;
    uint32  recvOpCode;
    uint32  recvOptionCode;
    uint32  recvSeqNum;
}strPortInfo;

typedef struct {
    strPortInfo     *freeList;
    strPortInfo     *usedList;
}strPortInfoMngr;


typedef struct {
    strDoubleLink   link;
	uint32	stationId;
    uint32	moduleId;
    uint32	fbBlockAddr;
    uint32	quality; 
	uint32	simulationFlag;	
    uint32	xrDataType;
    uint32	xrDataKind; 
    uint32	dataPointIndex; 
    uint32	xrStatus;
    uint8	xrData[4];
}strFbXrDataInfo;


typedef struct {
    strFbXrDataInfo     *freeList;
    strFbXrDataInfo     *usedList;
}strXRQueInfo;


typedef struct {
    strDoubleLink   link;
	uint32	fbExecId;
	uint32	taskId;
	uint32	fbId;
	uint32	fbBlockAddr;	
}strFbExecutionInfo;

typedef struct {
    strFbExecutionInfo     *freeList;
    strFbExecutionInfo     *usedList[MAX_LOGIC_TASK_NUM];
}strFbExecQueInfo;


#define	MEM_NOT_USING_IN_FUTURE	0
#define	MEM_USING_IN_FUTURE		1
typedef struct {
	strDoubleLink	link;
	uint32	taskId;
							
	uint32	memUsingFlag;   
							
							
							
	uint32	reservedFbAddr; 
	uint32	reservedFbCode;	
							
							
							
							
							
							
							
							
							
							
							

							
							
							
							
							
							
	
	uint32	usableMemSize;	
							
							
							
							
							
							
							
	uint32	unUsedMemStartIndex;
	uint32	unUsedMemEndIndex;
	uint32	freeMemSize;	
}strFbSpecUnusedMemInfo;

typedef struct {
	strFbSpecUnusedMemInfo	*freeList;
	strFbSpecUnusedMemInfo	*usedList;
}strFbSpecUnusedMemQueInfo;


#define	MAX_DATA_BUFF_NUM	256
typedef struct {
	strDoubleLink	link;
	uint32 	onlineDwlOperationCode;
	uint32	reqDataNum;
	uint32	reqDataBuf[MAX_DATA_BUFF_NUM];
}strOnlineDwlTaskComReqInfo;

typedef struct {
	strOnlineDwlTaskComReqInfo 	*freeList;
	strOnlineDwlTaskComReqInfo	*usedList;
}strOnlineDwlTaskComQueReqInfo;

typedef struct {
	strDoubleLink	link;
	uint32	onlineDwlOperationCode;
	uint32	respDataNum;
	uint32	respDataBuf[MAX_DATA_BUFF_NUM];
}strOnlineDwlTaskComRespInfo;

typedef struct {
	strOnlineDwlTaskComRespInfo 	*freeList;
	strOnlineDwlTaskComRespInfo		*usedList;
}strOnlineDwlTaskComQueRespInfo;



#define	ONLINE_DWL_SEMA_GET_TIME_OUT	(systemClockRateGet()/10)	


#define IS_JUMP_CODE 0x1
#define MAX_JUMP_CODE_NUM 1024


#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)
	#define	MAX_RMT_COPY_BUF_NUM	1024
	typedef struct {
		uint32 accDataNum;
		uint32 *accumPtr[MAX_RMT_COPY_BUF_NUM];
		uint32 accumData[MAX_RMT_COPY_BUF_NUM];
		uint32 accumType[MAX_RMT_COPY_BUF_NUM];
	}strAccumBusInfo;
#endif

#endif 
