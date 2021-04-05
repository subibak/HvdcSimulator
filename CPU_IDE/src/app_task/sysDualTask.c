
#include	"usrTypes.h"
#include	"sysDefs.h"
#include	"segScheduler.h"
#include	"pcsDefs.h"
#include	"systemDiag.h"
#include	"sysConfig.h"
#include	"usrErrCode.h"
#include	"vmeDef.h"

#include	"network.h"
#include	"usrQueData.h"
#include	"msgQueData.h"
#include	"msgQue.h"
#include	"sysDual.h"

#include	"fbMem.h"
#include	"sfcMem.h"

#include	"debugInfo.h"
#include	"usrDefBuf.h"
#include	"usrSema.h"

#include	"netProtocol.h"
#include	"osDepend.h"

#define	MAX_SYS_DUAL_MSG_QUEUE_COUNT 	(512)


void* gSysDualMsgQueId; 

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
void* gSysDualSendMsgQueId; 
strSysDualLiveCountInfo gSysDualLiveCount; 


uint32	gDluNetCurSendCnt=0,gDluNetPrevSendCnt=0,gDluNetSendCntPerSec=0;
#endif


extern uint32 *gDualDataBusPtr; 

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
strSysDualInfo gLocShelfDualInfo, gRmtShelfDualInfo;
extern uint32  gUniRMTRecvSockFd;
extern uint32  gLiveCountRecvSockFd;


extern uint32   gDualRecvInfoFlag;
extern uint32   gDualSendInfoFlag;
extern strDebugDataInfo gDualRecvData[DEBUG_INFO_MAX_ARRAY_NUM];
extern strDebugDataInfo gDualSendData[DEBUG_INFO_MAX_ARRAY_NUM];

extern uint32   gDualUnicastDetailRecvFlag, gDualBroadcastDetailRecvFlag;
extern uint32   gDualUnicastDetailSendFlag, gDualBroadcastDetailSendFlag;
extern uint32   gDualTransRateCalcuFlag;
#endif

extern strSysRunningConfigInfo		*gSysRunningInfoPtr;
extern strMpuDiuRunningStatusInfo 	*gDiuRasInfoPtr;

extern uint32	sysPing(int8 *);

extern strSysExecInfo *gSysExeInfoLocalPtr;


void	usrDualTask(int,int,int,int,int,int,int,int,int,int);
uint32	sysRedundancyVarInit(void);
uint32	sysRedundancyVarSet(strUserBootParaInfo *);
uint32  dualCmdProcess(strMsgQueData *recvPtr);
static uint32  dualFbRuntimeCopyCmdProcess(strMsgQueData *recvPtr);

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
void	dualDataRecvTask(int32, int32,int32,int32,int32, int32, int32,int32,int32,int32 );
void	usrDualSTask(int32, int32,int32,int32,int32, int32, int32,int32,int32,int32);
static uint32   dataReadAndSendToDualProcTask(int32  sockId);
#else
uint32	dualCmdMsgSendToDualTask(strSysDualInfo *);
#endif


extern int32	messageQueueCreate(strMsgQueInfo *, void **);
extern int32	messageQueueSend(void *, int8 *, int32,int32, int32);
extern int32	messageQueueRecv (void *, int8 *,int32 ,int32,int32 *);
extern void		memoryClear ( uint8 *, uint32);
extern void		setErrorCode(uint8 *,uint32,uint8 *,uint32);
extern uint32	myBusDataAcc(uint8 *, uint32, uint32	, uint32 ,uint8 *, uint8 *);
extern uint32	systemCnfgInfoRead( strSysConfigInfo *);
extern int32	inOutMultiplex( int32 , void   *, void *,void *,   void *);
extern int32	dataRecvFromServerOrClient( strDataBuf  *, void *,uint32   );
extern  void	memoryCopy (uint8 *, uint8 *, uint32);
extern void		setErrorCode(	uint8 *, uint32,uint8 *,uint32);
extern void		setErrorCodeWithVal(	uint8 *, uint32,uint8 *,uint32,
									uint8 *,uint32,uint8 *,uint32,uint8 *,uint32);
extern void		networkPerformanceCountSet(uint32   ,   uint32 );
extern int32	dataSendToServerOrClient(strDataBuf *,void *,uint32 );
extern uint32	netMsgChecksumCalcu(uint32  ,uint32 *,uint32    *);
extern int32	semaphoreGet(int32 , int32 );
extern uint32	comPortInfoGet(uint32, strinetAddr *, strinetAddr *);
extern void		logMessageSet (int8 *, uint32, int8 *, uint32 , uint32);

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	extern uint32   transDualInfoSet( strMsgQueData *);
	extern void		debugDualInfoSet(strMsgQueData *,   strDebugDataInfo *);
	extern uint32 (*responseDualCmdOPFuncList[]) (strDualProtocolInfo *,strBufInfo *);
	extern uint32 (*requestDualCmdOPFuncList[]) (strDualProtocolInfo *,strBufInfo *);
	extern uint32 (*remotecastDualCmdOPFuncList[]) (strDualProtocolInfo *,strBufInfo *);
#endif

#ifdef _WIN32
#if (MPU_RTOS == WIN_CE)
	extern uint32   exceptFilterFunc(LPEXCEPTION_POINTERS , uint8*, uint32);
	extern uint32   taskSuspendToListStatus(uint32);
#endif
#endif

extern uint32 periodicExecutionCheck(uint32 *, uint32, uint32 *);
extern int32	systemClockRateGet (void);

extern void		systemRasInfoSet(uint32 , uint32 );
extern uint32	allSegmentModeErrSet(uint32 );
extern void sysTaskDelay (int32 );
extern uint32	segmentLocalDataChecksumCheck(uint32);
extern uint32	rmtEthComSeqNumChkDBSet(uint32);
extern uint32	rmtComInfoQueClear(void);
extern void		fbExeSequenceQueuePtrClear(uint32);
extern uint32	getFbTaskTblInfo(uint32 , uint32 , strFbTaskTblMemInfo *);
extern uint32	sfcExecIdInfoClear(uint32 , uint32 );
extern uint32	sfcExecIdInfoSet(uint32 , uint32 );
extern uint32	fbExecIdInfoClear(uint32 , uint32 );
extern uint32	fbExecIdInfoSet(uint32 , uint32 );
extern uint32	fbSpecUnusedMemoryInfoClear( uint32 );
extern uint32	fbSpecUnusedMemoryInfoSet( uint32 );
extern uint32	fbOutputUnusedMemoryInfoClear( uint32 );
extern uint32	fbOutputUnusedMemoryInfoSet(uint32);
extern void longDataConversion( uint32 *, uint32 *, uint32 );
extern uint32		segSchedulerInfoInit(uint32);
extern uint32	schedulerModeChange(uint32 , uint32	);
extern void		debugMsgDisplayInISR(uint8 *, uint8 *,uint8 *,uint32, uint32, uint32,uint32,uint32);
extern uint32	fbSysExeModuleInfoRead( uint32, strModuleExecInfo* );


void usrDualTask(int a,int b,int c,int d,int e,int f,int g,int h,int ii,int j)
{
    int32	retLen = 0;
	uint32	status = NO_ERROR;
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	strMsgQueData   queData;
#ifdef _WIN32
__try{
#endif
	while(1L) {
    		
			status = messageQueueRecv(  gSysDualMsgQueId,
										(int8   *)&queData,
										sizeof(strMsgQueData),
										FOREVERWAIT,
										(uint32*)&retLen
									  );
			if(status) {
				setErrorCode	((uint8 *)__FILE__,
								 __LINE__,
								(uint8 *)"usrDualTask",
								status
								);
#if 0
				logMessageSet(__FILE__,__LINE__,
					"usrDualTask",DUAL_PROC_RECV_ERR,0);
#endif
			}

			if(status == NO_ERROR) {
				status = dualCmdProcess(&queData);
				if(status) {
        			setErrorCode ((uint8 *)__FILE__,
            						__LINE__,
									(uint8 *)"usrDualTask",
									status
									);
#if 0
					logMessageSet(__FILE__,__LINE__,
						"usrDualTask",status,0);
#endif
				}
			}
    }  
#ifdef _WIN32
  }
__except( exceptFilterFunc(GetExceptionInformation(), __FILE__,__LINE__ ) ) {
	taskSuspendToListStatus(GetCurrentThreadId());
}
#endif
#endif
}


uint32	sysRedundancyVarInit(void)
{
	uint32	status = NO_ERROR;
    strMsgQueInfo   queInfo;
	uint8	*srcPtr, *destPtr;
    uint8	clearBuf[MAX_DUAL_MEM_SIZE];
	uint32	i;
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	uint32	mpuForcingNumber, diuForcingNumber;
#endif
	strSysDualInfo *dualInfoPtr;

    
	queInfo.queueOptions	= MYMSG_Q_FIFO;	
    queInfo.queueCount      = MAX_SYS_DUAL_MSG_QUEUE_COUNT;
    queInfo.queueSize       = sizeof(strMsgQueData);

    status = messageQueueCreate(&queInfo, &gSysDualMsgQueId);

    if(status != NO_ERROR) {
    	return(status);
	}

#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
	
    memoryClear ( clearBuf, MAX_DUAL_MEM_SIZE);

    srcPtr 	= clearBuf;
    destPtr = (uint8 *)gDualDataBusPtr;

	dualInfoPtr =  (strSysDualInfo*)gDualDataBusPtr;

	for(i = 0; i < (uint32)MAX_DUAL_MEM_SIZE; i++, srcPtr++, destPtr++){
		status = myBusDataAcc(__FILE__,__LINE__,	BUS_BYTE_ACC, 
								BUS_DATA_WRITE ,
								(int8 *)destPtr, 
								(int8 *)srcPtr
							 );
		if(status) return(status);
	}
#endif
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	else status = messageQueueCreate(&queInfo, &gSysDualSendMsgQueId);

    if(status != NO_ERROR) {
    	return(status);
	}
	
    memoryClear ( clearBuf, MAX_DUAL_MEM_SIZE);

    srcPtr 	= clearBuf;
    destPtr = (uint8 *)gDualDataBusPtr;

	
	dualInfoPtr =  (strSysDualInfo*)gDualDataBusPtr;
	mpuForcingNumber = dualInfoPtr->forcingMpuFbCodeNum;
	diuForcingNumber = dualInfoPtr->forcingDiuFbCodeNum;

	for(i = 0; i < (uint32)MAX_DUAL_MEM_SIZE; i++, srcPtr++, destPtr++){
		status = myBusDataAcc(__FILE__,__LINE__,	BUS_BYTE_ACC, 
								BUS_DATA_WRITE ,
								(int8 *)destPtr, 
								(int8 *)srcPtr
							 );
		if(status) break;
	}

	
	dualInfoPtr->forcingMpuFbCodeNum = mpuForcingNumber;
	dualInfoPtr->forcingDiuFbCodeNum = diuForcingNumber;

	
	memoryClear((uint8*)&gRmtShelfDualInfo, sizeof(strSysDualInfo ));
#endif

    return(status);
}


uint32	sysRedundancyVarSet(strUserBootParaInfo *paraPtr)
{
	uint32	status = NO_ERROR;


    return(status);
}

#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)

uint32	dualCmdMsgSendToDualTask(strSysDualInfo	*dualInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	qDataLen = sizeof(strSysDualInfo);

    status = messageQueueSend(	gSysDualMsgQueId,
                               	(int8 *) dualInfoPtr,
                               	qDataLen,
                               	NOWAIT,
                                MYMSG_PRI_NORMAL
                             );

    return(status);
}

#elif (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)

void dualDataRecvTask(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)
int32   arg1,   arg2,   arg3,   arg4,   arg5,arg6, arg7, arg8, arg9, arg10;
{
    static struct   fd_set      readFds, tmpFds;
    int32   status = NO_ERROR;
    uint32  openSockNum =   0;
    strSysConfigInfo sysCnfgInfo;
    strRedundancyNetConfigInfo *redNetCnfgInfoPtr;
    int32       usrSockFd[2];
	uint32 i =0;
	

#if (BOARD_TYPE == LKV120_BOARD)
    int fd_max = 0;
#endif 

#ifdef _WIN32  
__try {
#endif

    systemCnfgInfoRead( (strSysConfigInfo   *)&sysCnfgInfo);
    redNetCnfgInfoPtr = &sysCnfgInfo.redNetCnfgInfo;

    if(redNetCnfgInfoPtr->ctrlNetDualStatus == BOARD_ENABLE){
		

	    FD_ZERO( &readFds   );
		FD_SET( gUniRMTRecvSockFd, &readFds);
		FD_SET( gLiveCountRecvSockFd, &readFds);

		usrSockFd[0] = gUniRMTRecvSockFd;
		usrSockFd[1] = gLiveCountRecvSockFd;
		openSockNum = 2; 

		while(1L)   {
			tmpFds =   readFds;

#if (BOARD_TYPE == LKV120_BOARD)
		fd_max = 0;
		for( i = 0; i   <   openSockNum; i++)   {
            if(FD_ISSET(usrSockFd[i], &tmpFds))
			{
				if(usrSockFd[i]>fd_max) {
	        		fd_max = usrSockFd[i];
				}
			}
		}

#endif 

#if (BOARD_TYPE == LKV120_BOARD)
			status = inOutMultiplex( fd_max + 1,
									  &tmpFds,
									  NULL,
									  NULL,
									  NULL
								   );
#else
			status = inOutMultiplex(  MAX_SOCKET_FD_NUM, 
									  &tmpFds,
									  NULL,
									  NULL,
									  NULL
								   );
#endif
			if(status   == NO_ERROR) {
				   for( i = 0; i   <   openSockNum; i++)   {
					   if(FD_ISSET(usrSockFd[i],   &tmpFds)) {
                			status = dataReadAndSendToDualProcTask(usrSockFd[i]);
					   }
				   }
			}
			if(status)
				setErrorCode( __FILE__,   __LINE__,
							  (uint8 *)"dualDataRecvTask", status
							);
		}
    }

#ifdef _WIN32
  }
__except( exceptFilterFunc(GetExceptionInformation(), __FILE__,__LINE__ ) ) {
	taskSuspendToListStatus(GetCurrentThreadId());
}
#endif
    return;
}


void usrDualSTask(arg1, arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)
int32   arg1,   arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10;
{
	strMsgQueData       queData;
	int32   status = NO_ERROR, retLen;
	strinetAddr         peerAddr; 
	strDataBuf          sendData;
	strSysConfigInfo    sysCnfgInfo;

    uint32  *cmdCode, opCode, optionCode;
	

	strDualProtocolInfo  *lgPtr;
	systemCnfgInfoRead( (strSysConfigInfo   *) &sysCnfgInfo);
#ifdef _WIN32  
__try {
#endif
	while(1L)   {

		memoryClear (   (uint8 *)   &queData,   sizeof(strMsgQueData));

		
	
		status = messageQueueRecv(   gSysDualSendMsgQueId,
									(int8 *)&queData,
									sizeof(strMsgQueData),
									FOREVERWAIT,
									&retLen
								);
		if(status) {
			setErrorCode ( __FILE__, __LINE__,
							(uint8 *)"usrDualSTask", 
							status
							);
			logMessageSet(__FILE__,__LINE__,
					"usrDualSTask",DUAL_SEND_QUEUE_RECV_ERR,0);
		}
		else {
			lgPtr   =   (strDualProtocolInfo *)queData.protoData.msgData;

			
			cmdCode = (uint32*)(lgPtr->dataBuff);
 			opCode = (*cmdCode)>>16;
			optionCode = (*cmdCode)&0xffff;

			memoryCopy ( (int8 *)&peerAddr,
						(int8 *)&queData.peerAddr,
						sizeof(strinetAddr)
						);

			

			
			if(gDualUnicastDetailSendFlag || gDualBroadcastDetailSendFlag ){
				memoryCopy ((int8 *)&(queData.peerAddr),
							(int8 *)&peerAddr,
							sizeof(strinetAddr)
							);
				transDualInfoSet(  &queData);
			}


			
			sendData.socket_fd = peerAddr.sockFd;
				
			
			sendData.data_len   =   queData.protoData.numOfBytes;
			sendData.socket_fd 	= peerAddr.sockFd;
			memoryCopy( sendData.data_buf,
						(uint8 *)   lgPtr,
						(uint32)sendData.data_len
						);

			
			status = dataSendToServerOrClient(  &sendData,
												(void *)&peerAddr,
												UDP_PROTO
											);

			if(status) {
				
				setErrorCode (    __FILE__,   __LINE__,
								(uint8 *)"usrDualSTask", status
								);
			}


			
			if(gDualSendInfoFlag){
				memoryCopy ( (int8 *)&(queData.peerAddr),
							(int8 *)&peerAddr,
							sizeof(strinetAddr)
							);
				debugDualInfoSet(&queData, gDualSendData);
			}	

			
			gDluNetCurSendCnt++;

			
			if(gDualTransRateCalcuFlag)
				networkPerformanceCountSet( SEND_PACKET_F,
											sendData.data_len
										); 
		} 
	}
#ifdef _WIN32
  }
__except( exceptFilterFunc(GetExceptionInformation(), __FILE__,__LINE__ ) ) {
	taskSuspendToListStatus(GetCurrentThreadId());
}
#endif
}


uint32 dualCmdProcess(strMsgQueData *recvPtr)
{
    uint32 status   =   NO_ERROR;
    uint32 *cmdCode, opCode, optionCode;
    
	strDualProtocolInfo  respMsg;
    strDualProtocolInfo  *netPtr;
	
    strBufInfo  respData;
	uint32 calcuChecksum,	qDataLen;
	strMsgQueData       queData;

	netPtr = (strDualProtocolInfo *)recvPtr->protoData.msgData;

	cmdCode = (uint32*)(netPtr->dataBuff);
 	opCode = (*cmdCode)>>16;
	optionCode = (*cmdCode)&0xffff;

	memoryClear (   (uint8 *)   &respData, sizeof(strBufInfo));

	if(opCode != DUAL_ACCESS_CMD) 	{
		status = L_OP_CODE_ERR;
		setErrorCode( __FILE__,   __LINE__,
					   (uint8 *)"dualCmdProcess", status
					  );

	    goto MODULE_END;
	}

	
	if(	(opCode == DUAL_ACCESS_CMD) &&
		(optionCode >= DUAL_RUNDATA_COPY)&&
		(optionCode <= DUAL_RUNBUS_COPY))
	{
		status = L_OPTION_CODE_ERR;
		setErrorCode( __FILE__,   __LINE__,
					   (uint8 *)"dualCmdProcess", status
					  );

	    goto MODULE_END;
	}

	
	(void)netMsgChecksumCalcu(	netPtr->dataLength,
								(uint32*)  netPtr->dataBuff,
								(uint32 *) &calcuChecksum
							 );
	if(netPtr->checkSum != (uint16)calcuChecksum) {
		status = MSG_CHECKSUM_ERR;
		setErrorCodeWithVal( __FILE__,   __LINE__,
					   (uint8 *)"dualCmdProcess", status,
					   "OPCMD",*cmdCode ,
					   "RECV CHEKCSUM",netPtr->checkSum,
					   "CALC CHEKCSUM",calcuChecksum
					  );
	    goto MODULE_END;
	}

	switch(netPtr->messageType) 
	{
		case RMTREQST:
			status = requestDualCmdOPFuncList[optionCode](netPtr,(strBufInfo *) &respData);

			if(status   != NO_ERROR)
				setErrorCode (__FILE__,   __LINE__,   "dualCmdProcess",   status);
		break;
		
		case RMTRESPS:
			status = responseDualCmdOPFuncList[optionCode](netPtr,(strBufInfo *) &respData);

			if(status   != NO_ERROR)
				setErrorCode (__FILE__,   __LINE__,   "dualCmdProcess",   status);
		break;

		case RMTCAST:
			status = remotecastDualCmdOPFuncList[optionCode](netPtr,(strBufInfo *) &respData);

			if(status   != NO_ERROR)
				setErrorCode (__FILE__,   __LINE__,   "dualCmdProcess",   status);
		break;

		default:
			status = L_MSG_TYPE_ERR;
		break;


	}

	if((netPtr->messageType==RMTREQST)) 
	{	
		
		if(respData.dataLen >   (MAX_NET_MSG_LEN - MAX_DUAL_FRAME_HEAD_LEN)) {
			status = DATA_LEN_EXCEED_ERR;
			setErrorCodeWithVal   (   __FILE__,
										__LINE__,
									   "dualCmdProcess",
										status,
									   "Calcu Data Len",   respData.dataLen,
									   "Recv Op Code",   opCode,
									   "Recv Option Code",   optionCode
									);
			goto MODULE_END;
		}

		
		memoryClear( (uint8 *) &respMsg, sizeof(strDualProtocolInfo));

		
		memoryCopy( (uint8 *)   &respMsg,
					(uint8 *)   netPtr,
					MAX_DUAL_FRAME_HEAD_LEN  
					);

		
		memoryCopy ( (uint8 *)respMsg.dataBuff,
					 (uint8 *)respData.dataBuf,
					 respData.dataLen
					 );
    
		
		respMsg.dualDataArea      =   netPtr->dualDataArea;

		
		respMsg.messageType     =   (uint8)RMTRESPS;
		respMsg.dataLength  =   (uint16)respData.dataLen ;

		 
		calcuChecksum = 0;
		(void)netMsgChecksumCalcu   (   (uint32)respMsg.dataLength,
										(uint32 *) &respMsg.dataBuff,
										(uint32 *) &calcuChecksum
									);
		
		respMsg.checkSum = (uint16)calcuChecksum;
		
		
		memoryClear (   (uint8 *)&queData, sizeof(strMsgQueData));

		
		status = comPortInfoGet(RMTREAD,   &recvPtr->peerAddr, &queData.peerAddr);
		if(status   != NO_ERROR){
			setErrorCode (__FILE__,   __LINE__,   "dualCmdProcess",   status);
			goto MODULE_END;
		}

		
		queData.protoData.numOfBytes = MAX_DUAL_FRAME_HEAD_LEN + respMsg.dataLength; 
		memoryCopy( (int8   *)queData.protoData.msgData,
					(int8   *)&respMsg,
					queData.protoData.numOfBytes
				  );
		
		
		qDataLen = SOCK_ADDR_LENGTH +   queData.protoData.numOfBytes + 4;

		status = messageQueueSend(  gSysDualSendMsgQueId,
									(int8   *) &queData,
									qDataLen,
									FOREVERWAIT,
									MYMSG_PRI_URGENT
								  );
		if(status   != NO_ERROR) {
			setErrorCode (__FILE__,   __LINE__,   "dualCmdProcess",   status);
			logMessageSet(__FILE__,__LINE__,"dualCmdProcess",DUAL_SEND_QUEUE_SEND_ERR,0);
		}
	}

MODULE_END:
	return(status);
}


static uint32 dualFbRuntimeCopyCmdProcess(strMsgQueData *recvPtr)
{
    uint32 status   =   NO_ERROR;
    uint32 *cmdCode, opCode, optionCode;
    
    strDualProtocolInfo  *netPtr;
    strBufInfo  respData;
	uint32 calcuChecksum;

	netPtr = (strDualProtocolInfo *)recvPtr->protoData.msgData;

	cmdCode 	= (uint32*)(netPtr->dataBuff);
 	opCode 		= (*cmdCode)>>16;
	optionCode 	= (*cmdCode)&0xffff;

	memoryClear (   (uint8 *)   &respData, sizeof(strBufInfo));

	if(opCode != DUAL_ACCESS_CMD) 	{
		status = L_OP_CODE_ERR;
		setErrorCode( __FILE__,   __LINE__,
					   (uint8 *)"dualFbRuntimeCopyCmdProcess", status
					  );

	    goto MODULE_END;
	}

	
	if(	(optionCode < DUAL_RUNDATA_COPY) ||
		(optionCode > DUAL_RUNBUS_COPY))
	{
		status = L_OPTION_CODE_ERR;
		setErrorCode( __FILE__,   __LINE__,
					   (uint8 *)"dualFbRuntimeCopyCmdProcess", status
					  );

	    goto MODULE_END;
	}

	
	(void)netMsgChecksumCalcu(	netPtr->dataLength,
								(uint32*)  netPtr->dataBuff,
								(uint32 *) &calcuChecksum
							 );
	if(netPtr->checkSum != (uint16)calcuChecksum) {
		status = MSG_CHECKSUM_ERR;
		setErrorCodeWithVal( __FILE__,   __LINE__,
					   (uint8 *)"dualFbRuntimeCopyCmdProcess", status,
					   "OPCMD",*cmdCode ,
					   "RECV CHEKCSUM",netPtr->checkSum,
					   "CALC CHEKCSUM",calcuChecksum
					  );
	    goto MODULE_END;
	}

	switch(netPtr->messageType) 
	{	
		case RMTRESPS:
			status = responseDualCmdOPFuncList[optionCode](netPtr,(strBufInfo *) &respData);

			if(status   != NO_ERROR)
				setErrorCode (__FILE__,   __LINE__,   "dualFbRuntimeCopyCmdProcess",   status);
		break;

		default:
			status = L_MSG_TYPE_ERR;
		break;

	}

MODULE_END:
	return(status);
}


static uint32   dataReadAndSendToDualProcTask(int32  sockId)
{
	uint32  status = NO_ERROR;

	strMsgQueData       queData;
    strDataBuf          recvData;
    uint32				qDataLen;
    strSysConfigInfo    sysCnfgInfo;

    uint32 *rcvDualCmdCodePtr, rcvDualOpCode, rcvDualOptionCode;
  	strDualProtocolInfo *rcvDualProtocolInfoPtr;

    memoryClear( (uint8 *) &recvData,   sizeof(strDataBuf));

    
    systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);

    
    recvData.socket_fd = sockId;

    
    status = dataRecvFromServerOrClient( &recvData,
                                         (void *)&queData.peerAddr,
                                         UDP_PROTO
                                       );
    if(status)
       	setErrorCode( __FILE__,   __LINE__,
        	             (uint8 *)"dataReadAndSendToDualProcTask", status
                    );
    else {
        memoryCopy( (int8   *)queData.protoData.msgData,
                    (int8   *)recvData.data_buf,
                    recvData.data_len
                  );
        queData.protoData.numOfBytes = recvData.data_len;

        
        
        
        queData.peerAddr.sockFd =   sockId;

        
        if(gDualRecvInfoFlag)
            debugDualInfoSet(&queData, gDualRecvData);

        
        if(gDualUnicastDetailRecvFlag || gDualBroadcastDetailRecvFlag ) {
            transDualInfoSet( &queData);
        }

        
        if(gDualTransRateCalcuFlag)
            networkPerformanceCountSet(RECV_PACKET_F,recvData.data_len); 

        
        
        
        
        qDataLen = sizeof(strinetAddr) + queData.protoData.numOfBytes   +   4;
        
        if(qDataLen >   sizeof(strMsgQueData)) {
            status = QUE_DATA_LEN_ERR;
            setErrorCodeWithVal   (__FILE__, __LINE__,
                                     "dataReadAndSendToDualProcTask",status,
                                     "MAX BYTE   LEN",   sizeof(strMsgQueData),
                                     "Calcu Byte Len",   qDataLen,
                                     "Not Using", 0
                                );
            goto MODULE_END;
		}

		

		rcvDualProtocolInfoPtr 	= (strDualProtocolInfo *)queData.protoData.msgData;
		rcvDualCmdCodePtr 		= (uint32*)(rcvDualProtocolInfoPtr->dataBuff);
 		rcvDualOpCode 			= (*rcvDualCmdCodePtr)>>16;
		rcvDualOptionCode 		= (*rcvDualCmdCodePtr)&0xffff;
 	
		
		if(	(rcvDualOpCode == DUAL_ACCESS_CMD) &&
			(rcvDualOptionCode >= DUAL_RUNDATA_COPY)&&
			(rcvDualOptionCode <= DUAL_RUNBUS_COPY))
		{
			status = dualFbRuntimeCopyCmdProcess(&queData);
			if(status) {
				setErrorCode( __FILE__,   __LINE__,
							   (uint8 *)"dataReadAndSendToDualProcTask", status
							  );
			}
		}
		else {
			status = messageQueueSend(   gSysDualMsgQueId,
										 (int8 *)   &queData,
										 qDataLen,
    									 FOREVERWAIT,
										 MYMSG_PRI_NORMAL
									  );

			if(status)
        		setErrorCode( __FILE__,   __LINE__,
							   (uint8 *)"dataReadAndSendToDualProcTask", status
							);
		}
	}

MODULE_END:
        return(status);
}


uint32   usrDualChkTask(arg1, arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)
int32 arg1, arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10;
{
	uint32  status = NO_ERROR;
	strSysRunningConfigInfo *sysStatusInfoPtr 		= gSysRunningInfoPtr;
	strMpuDiuRunningStatusInfo *diuStatusInfoPtr	= gDiuRasInfoPtr;
	strModuleExecInfo	*sysMpuExeInfoPtr = &gSysExeInfoLocalPtr->mpuSysExeInfo;

	strSysConfigInfo sysCnfgInfo;
	uint32	taskId;
	strSysDualInfo dualCmdInfo;
	strFbTaskTblMemInfo	segTbl;
	int8	ipAddr[20];
	
	for (;;) {
		
			
        if(	(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_STANDBY)
        						&&
        	(diuStatusInfoPtr->redundancyStatus != NO_ERROR) 
        ){
            systemCnfgInfoRead(&sysCnfgInfo);
        	memoryClear(ipAddr,strlen((int8*)ipAddr));
        	ipAddrHexToString(ipAddr,sysCnfgInfo.redNetCnfgInfo.rmtDluIpAddrHex);
        	
			
			if(sysPing(ipAddr) == NO_ERROR) {
 
				
	            
            	systemCnfgInfoRead(&sysCnfgInfo);

				
				sysTaskDelay(systemClockRateGet());

#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
                
                
				status = busMemoryDataCopyToLocalMem(MPU_DIU_DUAL_ACC);
    			if(status != NO_ERROR)
					setErrorCode	(	__FILE__,
       									__LINE__,
           	    	    	    		(uint8 *)"usrDualChkTask",
                       	    			status
									);
#endif
			
				 
				systemRasInfoSet(RAS_MPU_EXEC_LOOP_STATUS, 0);

				
				status = segmentLocalDataChecksumCheck(LOGIC_ID);
				if(status != NO_ERROR){
					systemRasInfoSet(RAS_MPU_CHECKSUM_STATUS, SEG_CHECKSUM_ERR);
                   	setErrorCode (	__FILE__,
                       					__LINE__,
                                        "usrDualChkTask",
                                        status
   									);
					return(status);
				}
				else
					systemRasInfoSet(RAS_MPU_CHECKSUM_STATUS, NO_ERROR);

                for(taskId = 1; taskId <= sysMpuExeInfoPtr->curMaxSegmentId; taskId++) {
					status = rmtEthComSeqNumChkDBSet(taskId);
                if(status)
                   	setErrorCode (	__FILE__,
                       					__LINE__,
                                        "usrDualChkTask",
                                        status
   									);
				}
				
				status = rmtComInfoQueClear();
                if(status)
                	setErrorCode (	__FILE__,
                    					__LINE__,
                                        "usrDualChkTask",
                                        status
									);

				
				
				(void)fbExeSequenceQueuePtrClear(TOTAL_DWL);

				
				
				
                for(taskId = 1; taskId <= MAX_LOGIC_TASK_NUM; taskId++) {
					status = getFbTaskTblInfo	(	LOGIC_ID,	taskId,(strFbTaskTblMemInfo	*) &segTbl);
					if(status != NO_ERROR)
                    	setErrorCode (	__FILE__,__LINE__,
                                            "usrDualChkTask",status
										);
					if(segTbl.logicType == SFC_LOGIC_TYPE){
                		
						status = sfcExecIdInfoClear(taskId, LOGIC_ID);
						if(status)
                    		setErrorCode (	__FILE__,
                        						__LINE__,
												"usrDualChkTask",
												status
											);
						
						
						status = sfcExecIdInfoSet(taskId, LOGIC_ID);
						if(status)
                    		setErrorCode (	__FILE__,
                        						__LINE__,
												"usrDualChkTask",
												status
											);
					}
				
					else {
                		
						status = fbExecIdInfoClear(taskId, LOGIC_ID);
						if(status)
                    		setErrorCode (	__FILE__,
                        						__LINE__,
												"usrDualChkTask",
												status
											);
						
						
						status = fbExecIdInfoSet(taskId, LOGIC_ID);
						if(status)
                    		setErrorCode (	__FILE__,
                        						__LINE__,
												"usrDualChkTask",
												status
											);
					}
				}				
				
														
	            

				status = fbSpecUnusedMemoryInfoClear(LOGIC_ID);
                if(status)
                    setErrorCode (	__FILE__,__LINE__,
                                        "usrDualChkTask",status
									);
				
				
				status = fbSpecUnusedMemoryInfoSet(LOGIC_ID);
                if(status)
                    setErrorCode (	__FILE__,__LINE__,
                                        "usrDualChkTask",status
									);
				
				status = fbOutputUnusedMemoryInfoClear(LOGIC_ID);
                if(status)
                    setErrorCode (	__FILE__,__LINE__,
                                        "usrDualChkTask",status
									);
				
				
				status = fbOutputUnusedMemoryInfoSet(LOGIC_ID);
                if(status)
                    setErrorCode (	__FILE__,__LINE__,
                                        "usrDualChkTask",status
									);

				
				status = segSchedulerInfoInit(MPU_ONLY_DUAL_ACC);
				if(status!=NO_ERROR) {
		    	    setErrorCode(	__FILE__,
		    	    	   			__LINE__,
		        	                "usrDualChkTask",
		            	   	        status
								  );
				}

				schedulerModeChange(LOGIC_ID, SCHEDULER_ENABLE);             	
				
    			systemRasInfoSet(RAS_REDUNDANCY_ERR_STATUS, NO_ERROR);
                
                dualCmdInfo.dualCmd = REDUNDANCY_ERR_CLR_CMD;
			

	   			debugMsgDisplayInISR(	"usrDualChkTask", 
										"REDUNDANCY_ERR_CLR_CMD",
	       	        		            __FILE__, __LINE__,0,0,0,0
									);
 			}
		}
		
		sysTaskDelay (systemClockRateGet());
	}
	
	return (status);
}

#endif 



