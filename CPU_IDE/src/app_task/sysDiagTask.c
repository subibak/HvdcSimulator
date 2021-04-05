
#include	<stdio.h>
#include	"usrTypes.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include	"netProtocol.h"

#include	"systemDiag.h"
#include	"sysDual.h"
#include	"network.h"
#include	"msgQueData.h"
#include	"usrSema.h"
#include	"osDepend.h"
#include	"usrErrCode.h"
#include	"segScheduler.h"


#include	"sysConfig.h"

#include	"usrQueData.h"
#include	"debugInfo.h"
#include	"osDepend.h"
#include	"lkv120Board.h"

#ifdef _WIN32
#include	"fbMem.h"
#include	"memDef.h"
#endif

#ifdef _WIN32
#define	RAS_TASK_CHK_PERIOD		BOARD_CLK_RATE * 3		
#endif


 
#define	RAS_TASK_DELAY_FACTOR	5


#define RAS_TASK_EXEC_DELAY (int32)(systemClockRateGet()/RAS_TASK_DELAY_FACTOR)  


#define	RAS_DATA_SEND_TIME		(RAS_TASK_DELAY_FACTOR)


#define	RAS_DATA_SET_TIME		(RAS_TASK_DELAY_FACTOR/2)


#define NET_COMM_ERR_CHECK_PERIOD     (RAS_TASK_DELAY_FACTOR*2) 

#define RMT_NET_COMM_ERR_CHECK_PERIOD (RAS_TASK_DELAY_FACTOR*1) 


static uint32	gRasTaskSemaId		= 0;
static int32	sCyclicMngrWdgTimer = 0;

static uint32	sRasDataSendTimer 		= 0;
static uint32	sEwsRasDataSendTimer 	= 0;

static uint32	sNetErrChkTimer = 0;
static uint32	sRmtNetErrChkTimer = 0;


uint32  gNetCommErrInitCount = 0;


static int32	sRasTaskElpTime = 0;
static int32	sRasTaskStartTime = 0, sRasTaskEndTime = 0;
static uint32   sWdtCount = 0;

#ifdef _WIN32
static uint32 	sRasTaskExecCnt 		= 0;
static uint32 	sPrevRasTaskExecCnt 	= 0;
static uint32	sRasTaskChkPeriod 		= 0;
#endif

#ifdef EVALUATION_USE_BUILD
static uint32 sEvaluationUseCountSec = 0;
#endif 


extern void *gSendQueId;  
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	extern void* gSysDualSendMsgQueId; 
#endif

extern uint32  gUsrTaskSchedulingFlag;

extern  strSysDualInfo gRmtShelfDualInfo;

extern strDiuSegSchedulerInfo	*gDiuSegSchdInfoPtr;

extern uint32 gFieldbusTaskSemaId;

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
extern uint32	gDluNetCurSendCnt, gDluNetPrevSendCnt, gDluNetSendCntPerSec;
#endif

extern int32	gDebugPortFd;

#ifdef _WIN32
extern uint32	gHstFbCodeBlockAddr;
extern uint32	gHstFbCodeSegId;
extern uint32 	gUniHSTSendSockFd_A;
extern uint32 	gUniHSTSendSockFd_B;
extern uint32 	gHstMasterConnected;
extern uint32 	gHstSlaveConnected;
#endif

extern strSysRunningConfigInfo		*gSysRunningInfoPtr;
extern strSystemRunningStatusInfo	gSysRunningStatusInfo;

uint32	rasTaskVarInit(void);
void wdgCyclicMngrISR(int32 );
static void rasGenAndMsgSend(uint32);
void uDiagnosisTask (	int32,int32,int32,int32,int32,
				int32,int32,int32,int32,int32
			);
#ifdef _WIN32
void rasTaskExecCheck (void);
#endif



extern void memoryClear(uint8 *, uint32);
extern void memoryCopy(uint8 *,uint8 *,uint32);
extern uint32 semaphoreCreate(strSemaInfo *, int32 *);
extern int32 wdgTimerCreate(void);
extern int32 wdgTimerStart(int32 , int32 , func_ptr ,int32);
extern uint32	comPortInfoGet(uint32,strinetAddr *, strinetAddr *);
extern int32   messageQueueSend(void *, int8 *, int32,int32, int32);
extern uint32 dualInfoStatusSet(void);
extern uint32	localShelfStatusMonitoring(void);

extern void networkCommErrCheck(void);
extern void rmtNetworkCommErrCheck(void);
extern uint32	systemCnfgInfoRead( strSysConfigInfo *);
extern int32 systemTickGet(void);
extern int32 	semaphoreGet(int32 , int32 );
#ifdef _WIN32
extern int32	semaphoreRelease(int32, int32);
extern int32	semaphoreRelease2(uint8 *, uint32, int32, int32);
#else
extern int32	semaphoreRelease (int32);
#endif

extern void setErrorCode(uint8 *,uint32,uint8 *,uint32);
extern int32 systemClockRateGet(void);
extern uint32 sysAuxTickCountGet(void);
extern int32 systemAuxClockRateGet(void);
extern uint32	rasSummaryGen(strNewNetProtocolInfo	*,uint32);

#ifdef _WIN32
extern void		sysMsgLogSet( uint8*,uint32,  uint8*, uint32, uint8*, uint32) ;
extern uint32	exceptFilterFunc(LPEXCEPTION_POINTERS, uint8*,uint32);
extern uint32   taskSuspendToListStatus(uint32);
#endif
extern void boardLedBlink (uint8 	, int32	);
extern uint32	ewsRasSummaryGen(strNewNetProtocolInfo	*msgPtr);
extern void fbcSWVersionGet(void);
extern void boardEthernetRegisterCheckAndErrClear();

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	static void		liveCountGenAndMsgSend(void);
	extern uint32	liveCountSet(void);
#endif

#if (BOARD_TYPE == LKV120_BOARD)
    extern void ledOn(int led);
    extern void ledOff(int led);
#endif

extern void boardReboot();

extern int linkTxReady (int lp_inx);

uint32	rasTaskVarInit(void)
{
	uint32	status = NO_ERROR;
	strSemaInfo	semaInfo;
	int32	isrArg = 0;

    
    semaInfo.semaType       =   BINARY_SEMA; 

    semaInfo.semaOptions    =   MYSEM_Q_PRIORITY;
    semaInfo.semaInitVal    =   SEM_EMPTY;
#ifdef _WIN32
	semaInfo.semaName		= "RASSema";
#endif
    status = semaphoreCreate(   (strSemaInfo *) &semaInfo,
                                (int32 *) &gRasTaskSemaId
                             );

	if(status) goto MODULE_END;

    isrArg = gRasTaskSemaId;

    sCyclicMngrWdgTimer = wdgTimerCreate();

    if(sCyclicMngrWdgTimer <= 0)
        status = WDG_CREATE_ERR;
    else
        status = (uint32)wdgTimerStart( sCyclicMngrWdgTimer,
                                        (uint32)RAS_TASK_EXEC_DELAY,
                                        (func_ptr)wdgCyclicMngrISR,
                                        isrArg
                                      );
MODULE_END:

    return(status);
}


void wdgCyclicMngrISR(int32 isrArg)
{
    uint32  timerDelay  = RAS_TASK_EXEC_DELAY;
    uint32  status  = NO_ERROR;
    uint32  status1 = NO_ERROR;
    uint32  status2 = NO_ERROR;
	strSysConfigInfo sysCnfgInfo;

	systemCnfgInfoRead(&sysCnfgInfo);
	

#ifdef _WIN32
    status1 = semaphoreRelease2(__FILE__,__LINE__,BINARY_SEMA, gRasTaskSemaId);
#else
    status1 = semaphoreRelease(gRasTaskSemaId);
#endif
#ifdef VXWORKS
    status = wdgTimerStart(    sCyclicMngrWdgTimer,
                                timerDelay,
                                (func_ptr)wdgCyclicMngrISR,
                                isrArg
                            );
    if(status1 || status )
        sysMsgLogSet("[WDGCyclicMngrISR]: RAS SemaRelErr [%x/%x] - WdgTimerStartErr[%x] -Errno[%x]\n",
                                status1, status, errno,0,0,0);
    if(status2 || status)
        sysMsgLogSet("[WDGCyclicMngrISR]: FMC SemaRelErr [%x/%x] - WdgTimerStartErr[%x] -Errno[%x]\n",
                                status2, status, errno,0,0,0);
#endif

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	
	if(++sWdtCount < RAS_TASK_DELAY_FACTOR) return;
	else 
	{
		sWdtCount = 0; 
		
		gDluNetSendCntPerSec 	= gDluNetCurSendCnt - gDluNetPrevSendCnt;
		gDluNetPrevSendCnt 		= gDluNetCurSendCnt;
	}
#endif

	return;
}


void uDiagnosisTask (arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)
int32 arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10;
{
	uint32	status = NO_ERROR;
	int32	startTime = 0, timeFactor;
	uint32	rasType;
#ifdef _WIN32
	uint32	hstSrvStatus = NO_ERROR;
   	strFbMemInfo		fbMemInfo;
	uint32	destFbBlockAddr = 0;
 	uint32	destDpcLoop;
	uint8    ipAddr[32];
	strFC770Info		fd;
#endif

#ifdef _WIN32  
__try {
#endif  	
	for(;;) {

    	if(gUsrTaskSchedulingFlag == DEBUG_ENABLE){
#ifdef SYS_AUX_CLK_USED
	    	startTime = sysAuxTickCountGet();
#else
    		startTime = systemTickGet();
#endif
		}

		if((status = semaphoreGet(gRasTaskSemaId, FOREVERWAIT)) == NO_ERROR){	
			
			boardLedBlink (USR_LED_1, USR_LED_ON);

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			
			liveCountSet();
			
			
			liveCountGenAndMsgSend();
#endif
        	
			dualInfoStatusSet();

#if (BOARD_TYPE == LKV120_BOARD)			
            if(gSysRunningInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN)
            {
                ledOn(3);  /* S1_LED_BLUE_BIT */
            }
            else 
            {
                ledOff(3);  /* S1_LED_BLUE_BIT */
            }
#endif

			rasType = 0;
			
            
			sRasDataSendTimer++;
			if(	(sRasDataSendTimer == RAS_DATA_SEND_TIME))
			{
            	sRasDataSendTimer = 0;
				rasType |= WDC_RAS;
				
#if ((BOARD_TYPE == MVME2100_BOARD) || (BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))
				boardEthernetRegisterCheckAndErrClear();
#endif

#ifdef EVALUATION_USE_BUILD
				sEvaluationUseCountSec++;
				if(sEvaluationUseCountSec > EVALUATION_TIMEOUT)
				{
					boardReboot();
				}
#endif 
			}
			
			
			sEwsRasDataSendTimer++;
			if( (sEwsRasDataSendTimer == RAS_DATA_SEND_TIME/**3*/)  ) 
			{
            	sEwsRasDataSendTimer = 0;
				rasType |= EWS_RAS;

#ifdef _WIN32
				if(	((gHstFbCodeBlockAddr > 0) && (gHstFbCodeBlockAddr <= 20000)) && 
					((gHstFbCodeSegId > 0) && (gHstFbCodeSegId <= MAX_LOGIC_TASK_NUM)))
				{

					destFbBlockAddr = (gHstFbCodeBlockAddr & 0xffff);
	
					hstSrvStatus = fbMemInfoRead (	LOGIC_ID,
												destFbBlockAddr,
												&fbMemInfo
											);
					if(hstSrvStatus)
					{
						gHstFbCodeBlockAddr = 0;
						gHstFbCodeSegId = 0;
					}
				}
				else {
					hstSrvStatus = 1;
				}
				
				if( (hstSrvStatus == NO_ERROR) &&
					(fbMemInfo.functionCode == HIGH_SPEED_TREND_CODE)	)
				{

					hstSrvStatus = readRuntimeFbData(	gHstFbCodeSegId,
												LOGIC_ID,
												destFbBlockAddr,
												(uint32 *)&fd
												);
					if(hstSrvStatus) 
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					}
					else
					{
						for(destDpcLoop=1;destDpcLoop<=HST_SLAVE_DPC;destDpcLoop++)
						{
							if(		(destDpcLoop == HST_MASTER_DPC) 
									&& 
									((fd.masterDpcALineIP == 0)&&(fd.masterDpcBLineIP == 0))
							) {
								continue;
							}
							else if((destDpcLoop == HST_SLAVE_DPC) 
									&& 
									((fd.slaveDpcALineIP == 0)&&(fd.slaveDpcBLineIP == 0))
							) {
								continue;
							}
							else if( ((fd.masterDpcALineIP != 0) || (fd.slaveDpcALineIP != 0)) 
									&&
									((gUniHSTSendSockFd_A==0) || (gUniHSTSendSockFd_A==((uint32)-1)))	)
							{
								continue;
							}
		
							if(destDpcLoop == HST_MASTER_DPC)
							{
								memoryClear( (uint8 *)ipAddr, 32);
								ipAddrHexToString(ipAddr, htonl(fd.masterDpcALineIP));
								if(ping(ipAddr,1,MINTIME_FLAG)!=NO_ERROR)
								{
									gHstMasterConnected = 0;
									continue;
								}
								else
								{
									gHstMasterConnected = 1;
								}
							}
							else
							{
								memoryClear( (uint8 *)ipAddr, 32);
								ipAddrHexToString(ipAddr, htonl(fd.slaveDpcALineIP));
								if(ping(ipAddr,1,MINTIME_FLAG)!=NO_ERROR)
								{
									gHstSlaveConnected = 0;
									continue;
								}
								else
								{
									gHstSlaveConnected = 1;
								}
							}
						}
					}
				}
#endif 

			}
			
			if((rasType&EWS_RAS)||(rasType&WDC_RAS)) 
			{
				rasGenAndMsgSend(rasType);
			}

			
			sNetErrChkTimer++;
			if(sNetErrChkTimer == NET_COMM_ERR_CHECK_PERIOD) 
			{
            	sNetErrChkTimer = 0;
#ifndef _SINGLE_CONTROLLER_PROGRAM  
                networkCommErrCheck();
#endif
			}	 

			sRmtNetErrChkTimer++;
			if(sRmtNetErrChkTimer == RMT_NET_COMM_ERR_CHECK_PERIOD) 
			{
            	sRmtNetErrChkTimer = 0;
#ifndef _SINGLE_CONTROLLER_PROGRAM  
                rmtNetworkCommErrCheck();
#endif
			}	 
		}
		
        if(status != NO_ERROR)
        	setErrorCode(__FILE__, __FILE__, __FUNCTION__, status);

    	if(gUsrTaskSchedulingFlag == DEBUG_ENABLE) 
    	{
#ifdef SYS_AUX_CLK_USED
	    	sRasTaskEndTime = sysAuxTickCountGet();
	    	timeFactor = systemAuxClockRateGet();
#else
    		sRasTaskEndTime = systemTickGet();
    		timeFactor	= systemClockRateGet();
#endif
			sRasTaskStartTime = startTime;

			sRasTaskElpTime = (sRasTaskEndTime - sRasTaskStartTime);
			sRasTaskElpTime *= 1000;
			sRasTaskElpTime /= timeFactor;
		}
    }
#ifdef _WIN32
  }
__except( exceptFilterFunc(GetExceptionInformation(), __FILE__,__LINE__ ) ) {
	taskSuspendToListStatus(GetCurrentThreadId());
}
#endif
}

#ifdef _WIN32

void rasTaskExecCheck (void)
{
	if(sRasTaskExecCnt != 0) {
		if(++sRasTaskChkPeriod > RAS_TASK_CHK_PERIOD) {
			sRasTaskChkPeriod = 0;

			if(sPrevRasTaskExecCnt == sRasTaskExecCnt)
				boardReboot();
		
			else
				sPrevRasTaskExecCnt = sRasTaskExecCnt;
		}
	}

	return;	
}
#endif 


void rasTaskSchdInfoDisplay(void)
{
    uint32	startTime;
    uint32	endTime;
	uint32	elpTime;

    
    startTime 	= sRasTaskStartTime; 
    endTime		= sRasTaskEndTime;
	elpTime		= sRasTaskElpTime;

	printf("%18s%18d%18d%18d\n", 
			"DiagnosisTask", startTime, endTime, elpTime);
}


static void rasGenAndMsgSend(uint32 rasType)
{
	strMsgQueData	msgQData;
	uint32	status = NO_ERROR;
	strNewNetProtocolInfo	*netMsgPtr;
	uint32	qDataLen = 0;
	strinetAddr rasAddr;
	uint32 tmpRasType = rasType;

	if(		((tmpRasType&WDC_RAS) != WDC_RAS)
		&&	((tmpRasType&EWS_RAS) != EWS_RAS)
	){
	    status = RAS_TYPE_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return;
	}
	
    memoryClear( (uint8 *) &msgQData, sizeof(strMsgQueData));

	if((tmpRasType&WDC_RAS) == WDC_RAS) 
	{
		status = comPortInfoGet(BROADCAST, NULL, &msgQData.peerAddr);
		if(status != NO_ERROR) 
			{
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			return;
		}
	}
	
#ifndef _PMS_PROGRAM
	
    netMsgPtr = (strNewNetProtocolInfo *)msgQData.protoData.msgData;	
	
  	status = rasSummaryGen ( netMsgPtr,(tmpRasType&WDC_RAS) );
	if(status != NO_ERROR) 
	{
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		
		return;
	}
	
    msgQData.protoData.numOfBytes = MAX_NEW_FRAME_HEAD_LEN + netMsgPtr->dataLength; 

    
    
    qDataLen = SOCK_ADDR_LENGTH + msgQData.protoData.numOfBytes + 4;

    status = messageQueueSend(	gSendQueId,
                               	(int8 *) &msgQData,
                               	qDataLen,
                               	NOWAIT,
                                MSG_PRI_NORMAL 	
                              ); 
	if(status != NO_ERROR) 
	{
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
	}
#endif 

	if((tmpRasType&EWS_RAS) == EWS_RAS)
	{
	    memoryClear( (uint8 *) &rasAddr, sizeof(strinetAddr));
	    memoryClear( (uint8 *) &msgQData.peerAddr, sizeof(strinetAddr));
	    
		rasAddr.port = htons(BROAD_EWS_SEND_RECV_PORT_A); 
		
		status = comPortInfoGet(BROADCAST, &rasAddr, &msgQData.peerAddr);
		if(status != NO_ERROR) 
		{
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			
			return;
		}
	} 
	else 
	{
		return;
	}
	
    netMsgPtr = (strNewNetProtocolInfo *)msgQData.protoData.msgData;

#ifndef _PMS_PROGRAM
    status = ewsRasSummaryGen ( netMsgPtr );
#else
  	status = rasSummaryGen ( netMsgPtr,(tmpRasType&EWS_RAS) );

#endif 
	if(status != NO_ERROR)
	{
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		 return;
	}
	   
    msgQData.protoData.numOfBytes = MAX_NEW_FRAME_HEAD_LEN + netMsgPtr->dataLength; 
    
    qDataLen = SOCK_ADDR_LENGTH + msgQData.protoData.numOfBytes + 4;

    status = messageQueueSend(	gSendQueId,
                               	(int8 *) &msgQData,
                               	qDataLen,
                               	NOWAIT,
                                MSG_PRI_NORMAL 	
                              ); 
	if(status)
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

	return;
}

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
static void liveCountGenAndMsgSend(void)
{
	strMsgQueData	msgQData;
	uint32	status = NO_ERROR;
	strDualProtocolInfo	*netMsgPtr;
	uint32	qDataLen = 0;

	strinetAddr localAddr;
	strMpuDiuRunningStatusInfo	*diuRasPtr
				= &gSysRunningStatusInfo.sysDiuRunningStatusInfo;
	strSysConfigInfo sysCnfgInfo;
	strSysRunningConfigInfo		*sysCnfgInfoPtr;

	sysCnfgInfoPtr = (strSysRunningConfigInfo *)
							&gSysRunningStatusInfo.sysRunningConfigInfo;

	return;
}
#endif
