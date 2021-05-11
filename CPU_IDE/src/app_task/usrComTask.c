
#include	<stdio.h>
#include	<string.h>
#include    "usrTypes.h"
#include	"sysDefs.h"
#include    "pcsDefs.h"
#include    "netProtocol.h"
#include    "cmdList.h"
#include    "sysConfig.h"
#include    "network.h"
#include    "msgQueData.h"
#include    "usrQueData.h"
#include    "msgQue.h"
#include    "osDepend.h"
#include    "usrDefBuf.h"
#include    "usrErrCode.h"
#include	"segScheduler.h"

#include    "debugInfo.h"
#include    "protocolBuf.h"
#include	"systemDiag.h"
#include	"sysDefs.h"
#include	"usrSema.h"

#include	"rmt_Comm\rmtDevUtil.h"
#include	"sysConfig.h"
#include	"sysDual.h"

#include	"rmt_Comm\ethModbus.h"

#ifdef _IPC_INTERFACE
#include	"ipc_if\ipcmgr.h"
#include	"ipc_if\ipcnetwork.h"

#include	"fbMem.h"

#include	<Psapi.h>
#endif

#include	"modbustcp\mbtServer.h"

/**************************************************************************
**		사용자 정의
**************************************************************************/
/* [V108] : 통신 FB사용시 큐 full 발생 하여 큐 갯수 증가(512-->2048) */
#define MAX_SYS_MSG_QUEUE_COUNT     (2048)

#define DUAL_LINE_SRC_INCLUDE   (1)
#define DUAL_LINE_DEBUGGING     (0)

#define NET_STATUS_CHECK_3_SEC	(3)			
#define NET_CONNECT_RETRY_1_SEC	(1)			

#define BitMask(bit)		(0x1u << (bit))
#define BitTest(val, bit)   ((val) & BitMask(bit))


uint32  gUniEWSRecvSockFd_A         =   0;
uint32  gUniEWSSendSockFd_A         =   0;
uint32  gUniEWSRecvSockFd_B         =   0;
uint32  gUniEWSSendSockFd_B         =   0;

uint32  gUniWDCRecvSockFd_A         =   0;
uint32  gUniWDCSendSockFd_A         =   0;
uint32  gUniWDCRecvSockFd_B         =   0;
uint32  gUniWDCSendSockFd_B         =   0;


uint32  gUniHSTRecvSockFd_A         =   0;
uint32  gUniHSTRecvSockFd_B         =   0;
uint32  gUniHSTSendSockFd_A         =   0;
uint32  gUniHSTSendSockFd_B         =   0;

uint32  gBroadSendRecvSockFd_A  =   0;
uint32  gBroadSendRecvSockFd_B  =   0;

uint32  gBroadEWSSendRecvSockFd_A  =   0;
uint32  gBroadEWSSendRecvSockFd_B  =   0;

uint32 gUniSendRecvSockFd_A = 0;
uint32 gUniSendRecvSockFd_B = 0;

strMBClntAndSlvDevInfo	gMBClntAndSlvDevInfo; 

void *gSendQueId;   
void *gRecvQueId;   
void *gHartSendQueId; 

uint32 gCurNetCommCount =   0;
uint32 gALineLiveCount =   0;
uint32 gBLineLiveCount =   0;
uint32 gMv4100PortErrFlag = 0;


uint32 gRmtShelfCurNetCommCount =   0;

#ifdef _WIN32

uint32   sSendAPortBind = 0 , sSendBPortBind = 0 ;

#endif 


static uint32 sUniDualChkSndRcvSockFd_A	= 0;
static uint32 sUniDualChkSndRcvSockFd_B	= 0;

static uint32	sDualChkOpenSockNum = 0;
static uint32	sDualOpenSockFd[2];
static struct   fd_set      sDualChkReadFds;


struct fd_set gTcpFdVar;

#ifdef _IPC_INTERFACE
	static int32 gIPCSendSockFd;
	
	
	PROCREG_t gProcessToRunList [REG_PRO_END+1];
	strModuleProcStatInfo gModuleProcStatInfo;
#endif

extern uint32   gRecvInfoFlag;
extern uint32   gSendInfoFlag;
extern strDebugDataInfo gDebugRecvData[DEBUG_INFO_MAX_ARRAY_NUM];
extern strDebugDataInfo gDebugSendData[DEBUG_INFO_MAX_ARRAY_NUM];
    
extern uint32   gTransUnicastDetailRecvFlag;
extern uint32   gTransBroadcastDetailRecvFlag;
extern uint32   gTransUnicastDetailSendFlag;
extern uint32   gTransBroadcastDetailSendFlag;

extern uint32   gTransRateCalcuFlag;


extern uint32   gRmtDevRecvDisplayFlag;
extern uint32   gRmtDevSendDisplayFlag;

extern strSysRunningConfigInfo		*gSysRunningInfoPtr;
extern strMpuDiuRunningStatusInfo 	*gDiuRasInfoPtr;
extern strNetRunningStatusInfo *gNetRasInfoPtr;

extern strSysConfigInfo	gSysCnfgInfo;

extern int32 gTcpSocketSemaId;

extern uint32	gDebugErrStringFlag;
extern strSystemRunningStatusInfo	gSysRunningStatusInfo;


void	usrNetRTask(int32, int32,int32,int32,int32, int32, int32,int32,int32,int32 );
void	netTcpSocketMngrTask(int32, int32,int32,int32,int32, int32, int32,int32,int32,int32 );
void	netTcpStatusCheckTask(int32, int32,int32,int32,int32, int32, int32,int32,int32,int32 );
void	usrNCmdTask(int32, int32,int32,int32,int32, int32, int32,int32,int32,int32 );
void	usrNetSTask(int32, int32,int32,int32,int32, int32, int32,int32,int32,int32);


#ifdef _IPC_INTERFACE
	void	ipcDataSendTask(int32, int32,int32,int32,int32, int32, int32,int32,int32,int32 );

	#if 1
		int32 compareUserProcessName (int8 *processName);
		int32 checkUserProcess ();
		int32 parseRegProcessConfigData (LT_INT * numOfProc);
		void procStatusCheckTask(int32, int32,int32,int32,int32, int32, int32,int32,int32,int32 );
	#endif
#endif 

uint32	netCommTaskVarInit(void);
uint32	commandProcess(strMsgQueData *);
uint32	optionCodeCheck(uint32  ,   uint32 );
#ifdef _WIN32
uint32	unicastSendPortBinding(uint32 lineKind);
#endif
uint32  netDataSendInFbTask(strNewNetProtocolInfo  *);

static uint32   taskQueueInit(void);
static uint32   hartCmdQueueInit(void);
static uint32   commConnectionInit(void);
static uint32	commConnectionReBinding(void);
static uint32   dataReadAndSendToCMDProcTask(uint32, uint32);
static uint32   myBroadcastDataCheck(strControlNetConfigInfo *, uint32);
static uint32   broadcastMsgCheck(strNewNetProtocolInfo *, strSysConfigInfo*);
static uint32   ethernetSharedDataCommConnectInit(void);
static void		netCommCountSetForRas(strControlNetConfigInfo   *, uint32   );
static uint32   responseCmdProcess (strNewNetProtocolInfo  *);
uint32    rmtShelfCheckCommConnectInit(void);
static uint32	rmtShelfCheckCommRecvAndRsp (strinetAddr *,strDataBuf *, uint16);
static uint32 clearTcpConnection(uint32, uint32 , int32);
static uint32 clearTcpConnectionWithIp(uint32, uint32 , int32);
static uint32 setTcpConnection(uint32 , uint32, int32);
static uint32 setDataLiveCount(uint32 , uint32 );


extern uint32   xrBroadcastCmd(strNewNetProtocolInfo   *);

extern uint32   sharedDataBroadcastCmd (strNewNetProtocolInfo *);
extern uint32   timeSyncBroadcastCmd (strNewNetProtocolInfo    *);
extern uint32   rmtDevProtocolEncode (strNewNetProtocolInfo *,strDataBuf * );
extern uint32   rmtDevMsgInfoSet    (strDataBuf *,strNewNetProtocolInfo *);
extern uint32   ethernetDataShareComInfoGet  (      strinetAddr  *,
                                                    strNewNetProtocolInfo *,
                                                    uint32  
                                              );
extern uint32   rmtDeviceTypeGet(strNewNetProtocolInfo *,uint32   *);
extern void		memoryClear(uint8   *, uint32);
extern void		memoryCopy(uint8 *, uint8   *, uint32);
extern void		setErrorCode(uint8 *, uint32, uint8   *, uint32);
extern void		setErrorCodeWithVal(uint8 *, uint32, uint8 *, uint32,
					uint8   *, uint32,uint8 *, uint32,uint8 *, uint32);
extern int32	messageQueueCreate(strMsgQueInfo   *, void **);
extern int32	messageQueueSend(void *, int8 *,int32,int32, int32);
extern int32	messageQueueRecv(void *, int8 *,int32,int32, int32 *);
extern int32	addrInfoBuild(void *,uint32 *,uint16   );
extern int32	connectionEstablishInServer(void   *, int16 , int32 *);
extern int32	connectionEstablishInClient(void   *, int16 , int32 *);
extern int32	socketOptionSet(    int32 ,int32   ,void   *,int32 );
extern int32	inOutMultiplex( int32 , void   *, void *,void *,   void *);
extern void		debugInfoSet(strMsgQueData *,   strDebugDataInfo *);
extern uint32   transInfoSet( strMsgQueData *);
extern uint32	dLMsgCheck(strMsgQueData *);
extern uint32   netMsgChecksumCalcu(uint32  ,uint32 *,uint32    *);
extern uint32	comPortInfoGet(uint32, strinetAddr *, strinetAddr *);


extern void		debugCountIncr(uint32   *);
extern int32	dataSendToServerOrClient(strDataBuf *,void *,uint32 );
extern void		networkPerformanceCountSet(uint32   ,   uint32 );

extern void		rmtDevMsgDisplay (uint8 *,   uint8   *, uint32, strinetAddr *);
extern void		rmtRCSComCountSet(  strDataBuf *);
extern uint32   rmtComMngrInit(void);


extern uint32   systemCnfgInfoRead( strSysConfigInfo *);
extern int32	dataRecvFromServerOrClient( strDataBuf  *, void *,uint32   );
#ifdef _WIN32
extern int32	tcpRecvFromModbusDevice(	strDataBuf  *,void *,uint32 );
#endif

extern void		longDataConversion( uint32 *, uint32 *, uint32 );
#ifdef VXWORKS
extern void		curLongDataConversion( uint32 *, uint32 );
#endif
extern void 	sysTaskDelay (int32 );
extern int32	systemClockRateGet(void);
   

uint32 fbDataGrHeaderCheck(strFbDataGrHeaderInfo *,strSysConfigInfo  *shelfCfgPtr);
uint32 requestCmdProcess(strNewNetProtocolInfo *,strBufInfo   *,strinetAddr *);
uint32 broadcastCmdProcess (strNewNetProtocolInfo *);
static uint32	appMsgProtocolHeadCheck( strNewNetProtocolInfo *, uint32) ;

extern uint32	dualLinePortInfoGet(strinetAddr *, strinetAddr  *, strNewNetProtocolInfo *);
extern uint32	stringCompareWithLen( uint8 *, uint8 *, uint32  );
extern void		netMsgChecksumCheckForDebugging(uint32 ,strNewNetProtocolInfo *);
				
extern uint32 (*CodeAccessCmdOPFuncList[]) (strNewNetProtocolInfo *,strBufInfo *);
extern uint32 (*CtrlModeAccessCmdOPFuncList[]) (strNewNetProtocolInfo *,strBufInfo *);
extern uint32 (*VariableAccessCmdOPFuncList[]) (strNewNetProtocolInfo *,strBufInfo *);
extern uint32 (*sysTimeCmdOPFuncList[]) (strNewNetProtocolInfo *,strBufInfo *);
extern uint32 (*forcingCmdOPFuncList[]) (strNewNetProtocolInfo *,strBufInfo *);
extern uint32 (*fbOnlineDwnCmdOPFuncList[])  (strNewNetProtocolInfo *,strBufInfo *);
extern uint32  (*sysRmtDataAccReqCmdOPFuncList[])(strNewNetProtocolInfo *,strBufInfo *);
extern uint32  (*sysRmtStatusReqCmdOPFuncList[])(strNewNetProtocolInfo *,strBufInfo *, strinetAddr *);
extern uint32  (*sysRmtStatusRespCmdOPFuncList[])(strNewNetProtocolInfo   *);
extern uint32  (*sysRmtDataAccRespCmdOPFuncList[])(strNewNetProtocolInfo  *); 
extern uint32  (*sysStatusCmdOPFuncList[]) (strNewNetProtocolInfo *,strBufInfo *);

#ifdef _WIN32
	extern uint32	exceptFilterFunc(LPEXCEPTION_POINTERS, uint8*,uint32);
	extern uint32   taskSuspendToListStatus(uint32);
#endif

extern void	dataDisplay (uint8 *, uint8 *, uint16);

#ifdef _IPC_INTERFACE
	extern LT_INT	messageIPCSendPortInit(	LT_INT tid, LT_INT * ipc_send_fd);
	extern LT_INT	getIpcProcessID(PROCESS_t process, LT_INT * targetProcessID);
#endif

#ifdef _WIN32
	extern int MBTS_CheckMaxCommIdleTime (INF_CHANNEL_MBTS_ST *pCh);
#endif


uint32  netCommTaskVarInit(void)
{
    uint32 status   =   NO_ERROR;

    status = taskQueueInit();
	if(status   != NO_ERROR) return(status);

	
	status = hartCmdQueueInit();
    if(status   != NO_ERROR) return(status);
        
    status = commConnectionInit();
    if(status   != NO_ERROR) return(status);

     
    status = rmtComMngrInit();
    if(status   != NO_ERROR) return(status);

    return(status);
}


static uint32   taskQueueInit(void)
{
    uint32  status = NO_ERROR;
    strMsgQueInfo       queInfo;

    queInfo.queueCount          =   MAX_SYS_MSG_QUEUE_COUNT;
    queInfo.queueSize           =   sizeof(strMsgQueData);
    queInfo.queueOptions        =   MYMSG_Q_FIFO;

    
    status = messageQueueCreate(&queInfo,   &gSendQueId);

    
    if(status   == NO_ERROR)
        status = messageQueueCreate(&queInfo,   &gRecvQueId);

    return(status);
}


static uint32   hartCmdQueueInit(void)
{
    uint32  status = NO_ERROR;
    strMsgQueInfo       queInfo;

    queInfo.queueCount          =   1;
    queInfo.queueSize           =   sizeof(strMsgQueData);
    queInfo.queueOptions        =   MYMSG_Q_FIFO;

    
    status = messageQueueCreate(&queInfo, &gHartSendQueId);

    return(status);
}   


static uint32   commConnectionInit(void)
{
    uint32  status = NO_ERROR;
    strinetAddr     localAddr;
    uint32  optionVal;
    strSysConfigInfo sysCnfgInfo;
    strControlNetConfigInfo *netInfoPtr;
	uint32 i = 0, j = 0;

    systemCnfgInfoRead( (strSysConfigInfo   *)&sysCnfgInfo);
    
    netInfoPtr = &sysCnfgInfo.netCnfgInfo;
    
    

	
    memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));

    status = addrInfoBuild( (void   *)&localAddr,
                             NULL,
                             UNI_EWS_UDP_RECV_PORT_A
                          );

    
    if(status   == NO_ERROR){
        
        status = (int32)connectionEstablishInServer((void *)   &localAddr,
                                                               UDP_PROTO,
                                                    (int32*)   &gUniEWSRecvSockFd_A
                                                   );
    }

	if(status)
	{
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		return(status);
	}
	
    else {
		
	    memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));

        status = addrInfoBuild( (void   *)&localAddr,
                                &netInfoPtr->myIpAddr[0],
                                UNI_EWS_UDP_SEND_PORT_A
                              );
        
        if(status   == NO_ERROR)
        {
            status = (int32)connectionEstablishInClient (
                                                             (void   *) &localAddr,
#ifdef VXWORKS
                                                              UDP_PROTO,
#else
                                                              UDP_UNICASTING,

#endif
                                                              (int32*) &gUniEWSSendSockFd_A
                                                         );
        }
   	}

	if(status)
	{
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		return(status);
	}
		
	
    memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));

    status = addrInfoBuild( (void   *)&localAddr,
                             NULL,
                             UNI_WDC_UDP_RECV_PORT_A
                          );

    
    if(status   == NO_ERROR){
        
        status = (int32)connectionEstablishInServer((void *)   &localAddr,
                                                               UDP_PROTO,
                                                    (int32*)   &gUniWDCRecvSockFd_A
                                                   );
    }

	if(status)
	{
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		return(status);
	}

    else {
		
	    memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));

        status = addrInfoBuild( (void   *)&localAddr,
                                &netInfoPtr->myIpAddr[0],
                                UNI_WDC_UDP_SEND_PORT_A
                              );
        
        if(status   == NO_ERROR)
            status = (int32)connectionEstablishInClient (
                                                             (void   *) &localAddr,
#ifdef VXWORKS
                                                              UDP_PROTO,
#else
                                                              UDP_UNICASTING,
#endif
                                                              (int32*)&gUniWDCSendSockFd_A
                                                         );
    }
	
	if(status)
	{
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		return(status);
	}

    else {
		
	    memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));

        status = addrInfoBuild( (void   *)&localAddr,
                                NULL,
                                BROAD_UDP_SEND_RECV_PORT_A
                              );
       	if(status   == NO_ERROR)
        	status = (int32)connectionEstablishInClient (
            												(void   *) &localAddr,
                                                            UDP_PROTO,
                                                            (int32*)&gBroadSendRecvSockFd_A
                                                        );
	}
	
	if(status)
	{
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		return(status);
	}

    else {
		
	    memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));

        status = addrInfoBuild( (void   *)&localAddr,
                                NULL,
                                BROAD_EWS_SEND_RECV_PORT_A
                              );
       	if(status   == NO_ERROR)
        	status = (int32)connectionEstablishInClient (
            												(void   *) &localAddr,
                                                            UDP_PROTO,
                                                            (int32*)&gBroadEWSSendRecvSockFd_A
                                                        );
	}

	if(status)
	{
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		return(status);
	}
	else {
		
	    memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));
	
	    status = addrInfoBuild( (void   *)&localAddr,
	                             NULL,
	                             UNI_HST_UDP_RECV_PORT_A
	                          );
	    if(status   == NO_ERROR)
	        status = (int32)connectionEstablishInServer((void *)   &localAddr,
	                                                               UDP_PROTO,
	                                                    (int32*)   &gUniHSTRecvSockFd_A
	                                                   );
	 }

	if(status)
	{
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		return(status);
	}
	
	else 
	{
		
	    memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));
	
        status = addrInfoBuild( (void   *)&localAddr,
                                &netInfoPtr->myIpAddr[0],
                                UNI_HST_UDP_SEND_PORT_A
								);

        if(status   == NO_ERROR)
            status = (int32)connectionEstablishInClient (	(void   *) &localAddr,
#ifdef VXWORKS
																	UDP_PROTO,
#else
																	UDP_UNICASTING,
#endif
                                                            (int32*)&gUniHSTSendSockFd_A
															);
	 }

	if(status)
	{
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		return(status);
	}
	
    
    else {
        
        optionVal   =  MAX_SOCK_BUF_SIZE;

        status = socketOptionSet(gUniEWSSendSockFd_A,SENDBUF_SIZE_OPT,
                                 (void   *)&optionVal,   sizeof(optionVal));
	    
        if(status   == NO_ERROR)
        {
            status = socketOptionSet(gUniEWSRecvSockFd_A,RECVBUF_SIZE_OPT,
                                     (void   *)&optionVal,   sizeof(optionVal));
        }

        
		if(status   == NO_ERROR)
		    status = socketOptionSet(gUniWDCSendSockFd_A,SENDBUF_SIZE_OPT,
                                 (void   *)&optionVal,   sizeof(optionVal));
	    
        if(status   == NO_ERROR)
            status = socketOptionSet(gUniWDCRecvSockFd_A,RECVBUF_SIZE_OPT,
                                     (void   *)&optionVal,   sizeof(optionVal));
                                     

        
		if(status == NO_ERROR)
		{
			if( (gUniHSTSendSockFd_A!=0) && (gUniHSTSendSockFd_A!=((uint32)-1)) )	
			{
				status = socketOptionSet(	gUniHSTSendSockFd_A,SENDBUF_SIZE_OPT,
											(void   *)&optionVal,   sizeof(optionVal));
			}
		}

	    
        if(status   == NO_ERROR)
            status = socketOptionSet(gUniWDCRecvSockFd_A,RECVBUF_SIZE_OPT,
                                     (void   *)&optionVal,   sizeof(optionVal));
    }

	if(status)
	{
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		return(status);
	}

    
    else {
        optionVal   =   1;
        status = socketOptionSet(       gBroadSendRecvSockFd_A,
                                        DATA_BROADCAST_OPT,
                                        (void   *)&optionVal,
                                        sizeof(optionVal)
                                );

		if(status   == NO_ERROR)
			status = socketOptionSet( 	gBroadEWSSendRecvSockFd_A,
                                        DATA_BROADCAST_OPT,
                                        (void   *)&optionVal,
                                        sizeof(optionVal)
                                );


	    
	    if(status   == NO_ERROR){
	        
	        optionVal   =  MAX_SOCK_BUF_SIZE;
	
	        status = socketOptionSet(gBroadSendRecvSockFd_A,SENDBUF_SIZE_OPT,
	                                 (void   *)&optionVal,   sizeof(optionVal));

		    
	        if(status   == NO_ERROR)
	            status = socketOptionSet(gBroadSendRecvSockFd_A,RECVBUF_SIZE_OPT,
	                                     (void   *)&optionVal,   sizeof(optionVal));

	        optionVal   =  MAX_SOCK_BUF_SIZE;
	        
			if(status   == NO_ERROR)
			    status = socketOptionSet(gBroadEWSSendRecvSockFd_A,SENDBUF_SIZE_OPT,
	                                 (void   *)&optionVal,   sizeof(optionVal));
		    
	        if(status   == NO_ERROR)
	            status = socketOptionSet(gBroadEWSSendRecvSockFd_A,RECVBUF_SIZE_OPT,
	                                     (void   *)&optionVal,   sizeof(optionVal));
	    }
    }

 	if(status)
 	{
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		return(status);
	}

   	if(netInfoPtr->ctrlNetDualStatus == CTRL_NET_LINE_DUAL) {
		
        memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));

        status = addrInfoBuild( (void   *)&localAddr,
                                NULL,
                                UNI_EWS_UDP_RECV_PORT_B
                              );

        
        if(status   == NO_ERROR){
	        
            status = (int32)connectionEstablishInServer(     (void *)   &localAddr,
                                                             UDP_PROTO,
                                                             (int32*) &gUniEWSRecvSockFd_B
                                                       );
        }

		if(status)
		{
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			return(status);
		}

        else {
			
	        memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));

            status = addrInfoBuild( (void   *)&localAddr,
                                    &netInfoPtr->myIpAddr[1],
                                    UNI_EWS_UDP_SEND_PORT_B
                                  );
            
            if(status   == NO_ERROR)
	            status = (int32)connectionEstablishInClient (
                                                              (void   *) &localAddr,
#ifdef VXWORKS
                                                              UDP_PROTO,
#else
                                                              UDP_UNICASTING,
#endif
                                                              (int32*)&gUniEWSSendSockFd_B
                                                             );
		}

		if(status)
		{
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			return(status);
		}
		
        memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));

        status = addrInfoBuild( (void   *)&localAddr,
                                NULL,
                                UNI_WDC_UDP_RECV_PORT_B
                              );

        
        if(status   == NO_ERROR){
	        
            status = (int32)connectionEstablishInServer(     (void *)   &localAddr,
                                                             UDP_PROTO,
                                                             (int32*) &gUniWDCRecvSockFd_B
                                                             );
		}

		if(status)
		{
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			return(status);
		}
		
        else {
			
	        memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));

            status = addrInfoBuild( (void   *)&localAddr,
                                    &netInfoPtr->myIpAddr[1],
                                    UNI_WDC_UDP_SEND_PORT_B
                                  );
            
            if(status   == NO_ERROR)
	            status = (int32)connectionEstablishInClient (
                                                              (void   *) &localAddr,
#ifdef VXWORKS
                                                              UDP_PROTO,
#else
                                                              UDP_UNICASTING,
#endif
                                                              (int32*) &gUniWDCSendSockFd_B
                                                             );
		}

		if(status)
		{
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			return(status);
		}
		
        else {
			
	        memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));

            status = addrInfoBuild( (void   *)&localAddr,
                                    NULL,   
                                    BROAD_UDP_SEND_RECV_PORT_B
                                  );
           	if(status   == NO_ERROR)
            	status = (int32)connectionEstablishInClient (
                                                               (void   *) &localAddr,
                                                               UDP_PROTO,
                                                               (int32*)&gBroadSendRecvSockFd_B
                                                             );
		}

		if(status)
		{
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			return(status);
		}

        else {
			
	        memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));

            status = addrInfoBuild( (void   *)&localAddr,
                                    NULL,   
                                    BROAD_EWS_SEND_RECV_PORT_B
                                  );
           	if(status   == NO_ERROR)
            	status = (int32)connectionEstablishInClient (
                                                               (void   *) &localAddr,
                                                               UDP_PROTO,
                                                               (int32*)&gBroadEWSSendRecvSockFd_B
                                                             );
		}

		if(status)
		{
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			return(status);
		}
		else {
		
			
	        memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));
	
	        status = addrInfoBuild( (void   *)&localAddr,
	                                NULL,
	                                UNI_HST_UDP_RECV_PORT_B
	                              );
	        if(status   == NO_ERROR)
	            status = (int32)connectionEstablishInServer(     (void *)   &localAddr,
	                                                             UDP_PROTO,
	                                                             (int32*) &gUniHSTRecvSockFd_B
	                                                             );
		}

		if(status)
		{
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			return(status);
		}
		else {
			
	        memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));
	
            status = addrInfoBuild( (void   *)&localAddr,
                                    &netInfoPtr->myIpAddr[1],
                                    UNI_HST_UDP_SEND_PORT_B
                                  );
            if(status   == NO_ERROR)
	            status = (int32)connectionEstablishInClient (
                                                              (void   *) &localAddr,
                                                              UDP_PROTO,
                                                              (int32*) &gUniHSTSendSockFd_B
                                                             );
		}

		if(status)
		{
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			return(status);
		}
        
        else {
            
            optionVal   =   MAX_SOCK_BUF_SIZE;
            status = socketOptionSet(gUniEWSSendSockFd_B,SENDBUF_SIZE_OPT,
                                     (void   *)&optionVal,   sizeof(optionVal));
            
            if(status   == NO_ERROR)
	            status = socketOptionSet(gUniEWSRecvSockFd_B,RECVBUF_SIZE_OPT,
                                        (void   *)&optionVal,   sizeof(optionVal));

            
            if(status   == NO_ERROR)
            status = socketOptionSet(gUniWDCSendSockFd_B,SENDBUF_SIZE_OPT,
                                     (void   *)&optionVal,   sizeof(optionVal));
            
            if(status   == NO_ERROR)
	            status = socketOptionSet(gUniWDCRecvSockFd_B,RECVBUF_SIZE_OPT,
                                        (void   *)&optionVal,   sizeof(optionVal));
                                        
            
            if(status == NO_ERROR)
			{
				if( (gUniHSTSendSockFd_B!=0) && (gUniHSTSendSockFd_B!=((uint32)-1)) )
				{
					status = socketOptionSet(gUniHSTSendSockFd_B,SENDBUF_SIZE_OPT,
						                 (void   *)&optionVal,   sizeof(optionVal));
				}
			}
            
            if(status   == NO_ERROR)
	            status = socketOptionSet(gUniHSTRecvSockFd_B,RECVBUF_SIZE_OPT,
                                        (void   *)&optionVal,   sizeof(optionVal));
        }
        
		if(status)
		{
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			return(status);
		}

        
       	else {
            optionVal   =   1;
            status = socketOptionSet(       gBroadSendRecvSockFd_B,
                                            DATA_BROADCAST_OPT,
                                            (void   *)&optionVal,
                                            sizeof(optionVal)
                                    );
			if(status   == NO_ERROR)
				status = socketOptionSet(   gBroadEWSSendRecvSockFd_B,
                                            DATA_BROADCAST_OPT,
                                            (void   *)&optionVal,
                                            sizeof(optionVal)
                                    );

		    
		    else {
		        
		        
		        optionVal   =  MAX_SOCK_BUF_SIZE;
		
		        status = socketOptionSet(gBroadSendRecvSockFd_B,SENDBUF_SIZE_OPT,
		                                 (void   *)&optionVal,   sizeof(optionVal));

			    
		        if(status   == NO_ERROR)
		            status = socketOptionSet(gBroadSendRecvSockFd_B,RECVBUF_SIZE_OPT,
		                                     (void   *)&optionVal,   sizeof(optionVal));
		
		        optionVal   =  MAX_SOCK_BUF_SIZE;

		        
				if(status   == NO_ERROR)
				    status = socketOptionSet(gBroadEWSSendRecvSockFd_B,SENDBUF_SIZE_OPT,
		                                 (void   *)&optionVal,   sizeof(optionVal));
			    
		        if(status   == NO_ERROR)
		            status = socketOptionSet(gBroadEWSSendRecvSockFd_B,RECVBUF_SIZE_OPT,
		                                     (void   *)&optionVal,   sizeof(optionVal));
		    }
        }
		if(status)
		{
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			return(status);
		}
    }

     
    if(status == NO_ERROR)
    	status = ethernetSharedDataCommConnectInit();

	 
	memoryClear((uint8 *)&gMBClntAndSlvDevInfo, sizeof(gMBClntAndSlvDevInfo));	

	for(i=0; i< MODBUS_TCP_MAX_SLAVE_NUM; i++)
	{
		for(j=0; j<MODBUS_TCP_MAX_CNET; j++)
		{
			gMBClntAndSlvDevInfo.slaveDeviceListInfo[i].tcpClntSock[j]			= -1;
			gMBClntAndSlvDevInfo.slaveDeviceListInfo[i].lastConnectTime[j]		= 0;		
			gMBClntAndSlvDevInfo.slaveDeviceListInfo[i].lastStatCheckTime[j]	= 0;
			gMBClntAndSlvDevInfo.slaveDeviceListInfo[i].dataRecvLiveCount[j]	= 0;
			gMBClntAndSlvDevInfo.slaveDeviceListInfo[i].preRecvLiveCount[j]		= 0;
		}

		gMBClntAndSlvDevInfo.slaveDeviceListInfo[i].lifeCount					= 0;
	}

	FD_ZERO(&gTcpFdVar);	

    return(status);
}



static uint32 commConnectionReBinding(void)
{
    uint32 status = NO_ERROR;
	strinetAddr localAddr;
	strSysConfigInfo sysCnfgInfo;
	strControlNetConfigInfo *netInfoPtr;
	uint32 optionVal = 0;

	systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);
    netInfoPtr = &sysCnfgInfo.netCnfgInfo;

	
	if( (gUniHSTSendSockFd_A==0) || (gUniHSTSendSockFd_A==((uint32)-1)) )
	{
		
	    memoryClear((uint8 *)&localAddr, sizeof(strinetAddr));
	
        status = addrInfoBuild( (void   *)&localAddr,
                                &netInfoPtr->myIpAddr[0],	
                                UNI_HST_UDP_SEND_PORT_A
								);

        if(status == NO_ERROR)
		{
            status = (int32)connectionEstablishInClient( (void   *) &localAddr,
#ifdef VXWORKS
																	UDP_PROTO,
#else
																	UDP_UNICASTING,
#endif
                                                         (int32*)&gUniHSTSendSockFd_A
														 );

			if(status)
			{
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			}
			else
			{
				if((gUniHSTSendSockFd_A!=0) && (gUniHSTSendSockFd_A!=((uint32)-1)))
				{
					status = socketOptionSet(gUniHSTSendSockFd_A, SENDBUF_SIZE_OPT,
											 (void *)&optionVal, sizeof(optionVal));

					if(status == NO_ERROR)
					{
						printf("UDP : SOCKET RE-BIND OK!!! (NETWORK A LINE IP 0x%x) \n", netInfoPtr->myIpAddr[0]);
					}
				}
			}
		}
	}

	if( (gUniHSTSendSockFd_B==0) || (gUniHSTSendSockFd_B==((uint32)-1)) )
	{
		
	    memoryClear((uint8 *)&localAddr, sizeof(strinetAddr));
	
        status = addrInfoBuild( (void   *)&localAddr,
                                &netInfoPtr->myIpAddr[1],	
                                UNI_HST_UDP_SEND_PORT_B
								);

        if(status == NO_ERROR)
		{
            status = (int32)connectionEstablishInClient( (void   *) &localAddr,
#ifdef VXWORKS
																	UDP_PROTO,
#else
																	UDP_UNICASTING,
#endif
                                                         (int32*)&gUniHSTSendSockFd_B
														 );

			if(status)
			{
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			}
			else
			{
				if((gUniHSTSendSockFd_B!=0) && (gUniHSTSendSockFd_B!=((uint32)-1)))
				{
					status = socketOptionSet(gUniHSTSendSockFd_B, SENDBUF_SIZE_OPT,
											 (void *)&optionVal, sizeof(optionVal));

					if(status == NO_ERROR)
					{
						printf("UDP : SOCKET RE-BIND OK!!! (NETWORK B LINE IP 0x%x) \n", netInfoPtr->myIpAddr[1]);
					}
				}
			}
		}
	}

	if(netInfoPtr->ctrlNetDualStatus == CTRL_NET_LINE_DUAL) 
	{
		if( (gUniHSTSendSockFd_B==0) || (gUniHSTSendSockFd_B==((uint32)-1)))
		{
			
	        memoryClear((uint8 *)&localAddr, sizeof(strinetAddr));
	
            status = addrInfoBuild( (void   *)&localAddr,
                                    &netInfoPtr->myIpAddr[1],	
                                    UNI_HST_UDP_SEND_PORT_B
									);

            if(status == NO_ERROR)
			{
	            status = (int32)connectionEstablishInClient(  (void   *) &localAddr,
#ifdef VXWORKS
																	UDP_PROTO,
#else
																	UDP_UNICASTING,
#endif
                                                              (int32*) &gUniHSTSendSockFd_B
                                                              );

				if(status)
				{
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
				}
				else
				{
					if ((gUniHSTSendSockFd_B!=0) && (gUniHSTSendSockFd_B!=((uint32)-1)) )
					{
						status = socketOptionSet(gUniHSTSendSockFd_B, SENDBUF_SIZE_OPT,
												 (void*)&optionVal, sizeof(optionVal));

						if(status == NO_ERROR)
						{
							printf("UDP : SOCKET RE-BIND OK!!! (NETWORK B LINE IP 0x%x) \n", netInfoPtr->myIpAddr[1]);
						}
					}
				}
			}
		}
	}

	return(status);
}


static uint32    ethernetSharedDataCommConnectInit(void)
{
   	int32    status =   NO_ERROR;
   	int32       optionVal;
   	strinetAddr local;
   	strSysConfigInfo sysCnfgInfo;
    strControlNetConfigInfo *netInfoPtr;

    systemCnfgInfoRead( (strSysConfigInfo   *)&sysCnfgInfo);
    
    netInfoPtr = &sysCnfgInfo.netCnfgInfo;

	memoryClear( (int8 *)   &local, sizeof(strinetAddr));
	
    if(status   == NO_ERROR){

		status = addrInfoBuild( (void   *)&local,
                                &netInfoPtr->myIpAddr[0],
                                UNI_UDP_SEND_RECV_PORT_A
                              );
		
        if(status   == NO_ERROR)
        	status = (int32)connectionEstablishInClient (
            	                                            (void   *) &local,
#ifdef VXWORKS
                                                            UDP_PROTO,
#else
                                                            UDP_UNICASTING,
#endif
                                                            (int32*)&gUniSendRecvSockFd_A
                                                        );
        }
        /* 송,수신 소켓 -   B   */
        if( (status == NO_ERROR) && 
        	(netInfoPtr->ctrlNetDualStatus ==   CTRL_NET_LINE_DUAL)) {

			status = addrInfoBuild( (void   *)&local,
            	                    &netInfoPtr->myIpAddr[1],/*   ip address B */
                                    UNI_UDP_SEND_RECV_PORT_B
                                  );
            /* unicast Send socket open */
            if(status   == NO_ERROR)
            	status = (int32)connectionEstablishInClient (
                                                            (void   *) &local,
#ifdef VXWORKS
                                                            UDP_PROTO,
#else
                                                            UDP_UNICASTING,
#endif
                	                                        (int32*)&gUniSendRecvSockFd_B
                                                             );
        }

        
        if(status   == NO_ERROR){
	        

            
            optionVal   =   MAX_SOCK_BUF_SIZE;

            status = socketOptionSet(gUniSendRecvSockFd_A,SENDBUF_SIZE_OPT,
                                     (void   *)&optionVal,   sizeof(optionVal));

            if((status ==   NO_ERROR)   && 
	            (netInfoPtr->ctrlNetDualStatus ==   CTRL_NET_LINE_DUAL)) {
                /* 라인 -   B   송신 버퍼   size */
                status = socketOptionSet(gUniSendRecvSockFd_B,SENDBUF_SIZE_OPT,
                                         (void   *)&optionVal,   sizeof(optionVal));
		}
	}

    return(status);
}


void usrNetRTask(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)
int32   arg1,   arg2,   arg3,   arg4,   arg5,arg6, arg7, arg8, arg9, arg10;
{

    static struct fd_set readFds, tmpFds;
    int32   status = NO_ERROR;
    uint32       i;
    uint32  openSockNum =   0;
    strSysConfigInfo sysCnfgInfo;
    strControlNetConfigInfo *netCnfgInfoPtr;
    uint32       usrSockFd[MAX_RECV_SOCK_NUM];
	uint32 loop = 0;
#if (BOARD_TYPE == LKV120_BOARD)
    int fd_max;
#endif 
#ifdef _WIN32  
__try {
#endif
    
	FD_ZERO(&readFds);
    FD_SET(gUniEWSRecvSockFd_A, &readFds);
    FD_SET(gUniWDCRecvSockFd_A, &readFds);
    FD_SET(gUniHSTRecvSockFd_A, &readFds);

    FD_SET(gBroadSendRecvSockFd_A, &readFds);
    FD_SET(gBroadEWSSendRecvSockFd_A, &readFds);

    systemCnfgInfoRead( (strSysConfigInfo   *)&sysCnfgInfo);
    
    netCnfgInfoPtr = &sysCnfgInfo.netCnfgInfo;

    if(netCnfgInfoPtr->ctrlNetDualStatus == CTRL_NET_LINE_DUAL)
	{
		FD_SET( gUniEWSRecvSockFd_B, &readFds);
        FD_SET( gUniWDCRecvSockFd_B, &readFds);
        FD_SET( gUniHSTRecvSockFd_B, &readFds);

        FD_SET( gBroadSendRecvSockFd_B, &readFds);
        FD_SET( gBroadEWSSendRecvSockFd_B, &readFds);
    }

    
	FD_SET( gUniSendRecvSockFd_A, &readFds);
    if(netCnfgInfoPtr->ctrlNetDualStatus == CTRL_NET_LINE_DUAL){
	    FD_SET( gUniSendRecvSockFd_B, &readFds);
    }

    if(netCnfgInfoPtr->ctrlNetDualStatus == CTRL_NET_LINE_DUAL) {
        openSockNum =   MAX_RECV_SOCK_NUM;

        
        usrSockFd[0] = gUniEWSRecvSockFd_A;
	    usrSockFd[1] = gUniWDCRecvSockFd_A;
        usrSockFd[2] = gBroadSendRecvSockFd_A;
	    usrSockFd[3] = gBroadEWSSendRecvSockFd_A;
		usrSockFd[4] = gUniSendRecvSockFd_A;
        usrSockFd[5] = 0;
        usrSockFd[6] = 0;
        usrSockFd[7] = 0;

        
        usrSockFd[8] = 0;
        
        usrSockFd[9] = gUniHSTRecvSockFd_A;

	    usrSockFd[10]  = gUniEWSRecvSockFd_B;
        usrSockFd[11]  = gUniWDCRecvSockFd_B;
        usrSockFd[12] = gBroadSendRecvSockFd_B;
        usrSockFd[13] = gBroadEWSSendRecvSockFd_B;
	    usrSockFd[14] = gUniSendRecvSockFd_B;
        usrSockFd[15] = 0;
        usrSockFd[16] = 0;
        usrSockFd[17] = 0;
        
        
        usrSockFd[18] = 0;
        
        usrSockFd[19] = gUniHSTRecvSockFd_B;
#if (BOARD_TYPE == LKV120_BOARD)
        fd_max = usrSockFd[19];
#endif 
    }
    else { 
        openSockNum =   MINI_RECV_SOCK_NUM;

        
        usrSockFd[0] = gUniEWSRecvSockFd_A;
        usrSockFd[1] = gUniWDCRecvSockFd_A;
        usrSockFd[2] = gBroadSendRecvSockFd_A;
        usrSockFd[3] = gBroadEWSSendRecvSockFd_A;
        
		usrSockFd[4] = gUniSendRecvSockFd_A;
        usrSockFd[5] = 0;
        usrSockFd[6] = 0;
        usrSockFd[7] = 0;
        
        
        usrSockFd[8] = 0;
        
        
        usrSockFd[9] = gUniHSTRecvSockFd_A;
#if (BOARD_TYPE == LKV120_BOARD)
        fd_max = usrSockFd[9];
#endif 
    }

    for(;;)   
	{
        tmpFds = readFds;
#if (BOARD_TYPE == LKV120_BOARD)
        
        fd_max = 0; 
        for(i=0;i< openSockNum ;i++) 
		{
				
            if(FD_ISSET(usrSockFd[i], &tmpFds))
            {
                  if( usrSockFd[i] > fd_max)
                  {
                      fd_max = usrSockFd[i];
                  }
            }
                
        } 
#endif
#if (BOARD_TYPE == LKV120_BOARD)
		status = inOutMultiplex(  fd_max + 1,
                                  &tmpFds,
                                  NULL,
                                  NULL,
                                  NULL
								  );
#else
		status = inOutMultiplex( MAX_SOCKET_FD_NUM, 
                                  &tmpFds,
                                  NULL,
                                  NULL,
                                  NULL
								  );
#endif

		if(status == NO_ERROR) 
		{				
        	for( i = 0; i < openSockNum; i++)  
			{
            	if(FD_ISSET(usrSockFd[i],   &tmpFds))
				{
#ifdef _WIN32		
					
					if( (netCnfgInfoPtr->ctrlNetDualStatus == CTRL_NET_LINE_DUAL) && 
						(sSendAPortBind ==NOT_BINDING) && (i >= MINI_RECV_SOCK_NUM) 
					) {
						sSendBPortBind = SOCKET_BINDING;
						unicastSendPortBinding(NET_LINE_LINK_B);
					}
					else if (sSendAPortBind ==NOT_BINDING) 
					{
						sSendAPortBind = SOCKET_BINDING;
						unicastSendPortBinding(NET_LINE_LINK_A);
					}
#endif

                	status = dataReadAndSendToCMDProcTask(usrSockFd[i], UDP_PROTO);
				}
			} 
        }

        if(status)
		{
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
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

#ifdef _WIN32
uint32 unicastSendPortBinding(uint32 lineKind)
{ 
	int32   status = NO_ERROR;
	strinetAddr     localAddr;
   	strSysConfigInfo sysCnfgInfo;
    strControlNetConfigInfo *netInfoPtr;

    systemCnfgInfoRead( (strSysConfigInfo   *)&sysCnfgInfo);
    netInfoPtr = &sysCnfgInfo.netCnfgInfo;
    memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));


	if(lineKind==NET_LINE_LINK_B)
	{
		status = addrInfoBuild( (void   *)&localAddr,
									&netInfoPtr->myIpAddr[1],
									UNI_EWS_UDP_SEND_PORT_B
								  );
		if(status == NO_ERROR)
			status = bind(gUniEWSSendSockFd_B,(struct sockaddr *)&localAddr,SOCK_ADDR_LENGTH);

		if(status == NO_ERROR)
			status = addrInfoBuild( (void   *)&localAddr,
									&netInfoPtr->myIpAddr[1],
									UNI_WDC_UDP_SEND_PORT_B
								  );
		if(status == NO_ERROR)
			status = bind(gUniWDCSendSockFd_B,(struct sockaddr *)&localAddr,SOCK_ADDR_LENGTH);


		if(status == NO_ERROR)
			status = addrInfoBuild( (void   *)&localAddr,
									&netInfoPtr->myIpAddr[1],
									UNI_UDP_SEND_RECV_PORT_B
								  );
		if(status == NO_ERROR)
			status = bind(gUniSendRecvSockFd_B,(struct sockaddr *)&localAddr,SOCK_ADDR_LENGTH);
		if(status == NO_ERROR)
			status = addrInfoBuild( (void   *)&localAddr,
									&netInfoPtr->myIpAddr[1],
									UNI_HST_UDP_SEND_PORT_B
								  );
		if(status == NO_ERROR)
			status = bind(gUniHSTRecvSockFd_B,(struct sockaddr *)&localAddr,SOCK_ADDR_LENGTH);
	}
	else if(lineKind==NET_LINE_LINK_A)
	{
		status = addrInfoBuild( (void   *)&localAddr,
									&netInfoPtr->myIpAddr[0],
									UNI_EWS_UDP_SEND_PORT_A
								  );
		if(status == NO_ERROR)
			status = bind(gUniEWSSendSockFd_A,(struct sockaddr *)&localAddr,SOCK_ADDR_LENGTH);

		if(status == NO_ERROR)
			status = addrInfoBuild( (void   *)&localAddr,
									&netInfoPtr->myIpAddr[0],
									UNI_WDC_UDP_SEND_PORT_A
								  );
		if(status == NO_ERROR)
			status = bind(gUniWDCSendSockFd_A,(struct sockaddr *)&localAddr,SOCK_ADDR_LENGTH);


		if(status == NO_ERROR)
			status = addrInfoBuild( (void   *)&localAddr,
									&netInfoPtr->myIpAddr[0],
									UNI_UDP_SEND_RECV_PORT_A
								  );
		if(status == NO_ERROR)
			status = bind(gUniSendRecvSockFd_A,(struct sockaddr *)&localAddr,SOCK_ADDR_LENGTH);
		if(status == NO_ERROR)
			status = addrInfoBuild( (void   *)&localAddr,
									&netInfoPtr->myIpAddr[0],
									UNI_HST_UDP_SEND_PORT_A
								  );
		if(status == NO_ERROR)
			status = bind(gUniHSTRecvSockFd_A,(struct sockaddr *)&localAddr,SOCK_ADDR_LENGTH);
	}

	return(status);
}
#endif 


void netTcpStatusCheckTask(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)
int32   arg1,   arg2,   arg3,   arg4,   arg5,arg6, arg7, arg8, arg9, arg10;
{
    int32 status = NO_ERROR;
	uint32 i = 0;
	uint32 stationId = 0; 
	uint32 netIdx = 0;
	strModbusTcpDevInfo *ptrSlaveDeviceListInfo = NULL;
	uint32 timeInSec = 0;
	uint32 lastStatCheckTime = 0;

#ifdef _WIN32
	int iNumDevMbts = 0;
	INF_CHANNEL_MBTS_ST *pCh = NULL;
	INF_DEV_MBTS_ST *pI = NULL;
	int index = 0;
#endif

	for(;;)   
	{
		
		sysTaskDelay(systemClockRateGet()/1);
		
		commConnectionReBinding();

		for(i = 0; i < MODBUS_TCP_MAX_SLAVE_NUM; i++)
		{
			ptrSlaveDeviceListInfo = &gMBClntAndSlvDevInfo.slaveDeviceListInfo[i];

			stationId = ptrSlaveDeviceListInfo->slaveDeviceId;
			if( (stationId == 0) || (stationId > MODBUS_TCP_MAX_SLAVE_NUM)){
				if(stationId > MODBUS_TCP_MAX_SLAVE_NUM)
				{
					status = RMT_DEVICE_TYPE_ERR;
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
				}
				continue;
			}

			
            if (BitTest(gMBClntAndSlvDevInfo.destStation[(stationId-1)>>5], ((stationId-1)&0x1F))) 
			{
				for(netIdx=0; netIdx< MODBUS_TCP_MAX_CNET; netIdx++) 
				{
					
					if( (ptrSlaveDeviceListInfo->tcpClntSock[netIdx] != -1) && (ptrSlaveDeviceListInfo->ipAddrInfoList[netIdx] != 0) )
					{
						
						timeInSec = 0; 
						curTimeGetInSecond(&timeInSec);
						lastStatCheckTime = ptrSlaveDeviceListInfo->lastStatCheckTime[netIdx];

						if((abs(timeInSec - lastStatCheckTime) >= (NET_STATUS_CHECK_3_SEC*3)) || (timeInSec == 0) || (lastStatCheckTime == 0))
						{
							ptrSlaveDeviceListInfo->lastStatCheckTime[netIdx] = timeInSec;					

							if ( (ptrSlaveDeviceListInfo->dataRecvLiveCount[netIdx] != 0) &&	
								 (ptrSlaveDeviceListInfo->dataRecvLiveCount[netIdx] == ptrSlaveDeviceListInfo->preRecvLiveCount[netIdx]) )
							{
								#if 111
								if(gDebugErrStringFlag == DEBUG_ENABLE) printf("NETWORK STASTUS CHECK !!! stationID %d, netIdx %d, tcpClntSock 0x%x, cur_LiveCount %d, pre_LiveCount %d \n", 
										stationId, netIdx, ptrSlaveDeviceListInfo->tcpClntSock[netIdx], 
										ptrSlaveDeviceListInfo->dataRecvLiveCount[netIdx], ptrSlaveDeviceListInfo->preRecvLiveCount[netIdx]);
								#endif

								status = SOCK_CONNECT_ERR;
								setErrorCodeWithVal (	__FILE__,__LINE__, __FUNCTION__, status,
														"STATION", stationId,
														"NETIDX", netIdx,
														"CUR LIVE CNT", ptrSlaveDeviceListInfo->dataRecvLiveCount[netIdx]
														);

								clearTcpConnection(stationId, netIdx, ptrSlaveDeviceListInfo->tcpClntSock[netIdx]);

								ptrSlaveDeviceListInfo->dataRecvLiveCount[netIdx] = 0;
								ptrSlaveDeviceListInfo->preRecvLiveCount[netIdx] = 0;

							}
							else
							{
								ptrSlaveDeviceListInfo->preRecvLiveCount[netIdx] = ptrSlaveDeviceListInfo->dataRecvLiveCount[netIdx];
							}
						} 
					}
				}
			} 
		}
	
#ifdef _WIN32
		
		
		
		iNumDevMbts = g_MbtServerCfg.iNumDevMbts;
		for (index = 0; index < iNumDevMbts; index++)
		{
			pI = &g_MbtsInf[index];

			pCh = &pI->tChan[0];
			for (i=0; i<MAX_CHANNEL_MBTS; i++, pCh++)
			{
				if (pCh->commFd > 0)
				{
					MBTS_CheckMaxCommIdleTime (pCh);
				}
			}
		}
#endif	
	} 
 
    return;
}


void netTcpSocketMngrTask(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)
int32   arg1,   arg2,   arg3,   arg4,   arg5,arg6, arg7, arg8, arg9, arg10;
{
	int32 status = NO_ERROR;
	uint32 numOfSlave = 0;
	int32 clientSocket = 0;
	uint32 i = 0, netIdx = 0, stationID = 0;
	uint32 timeInSec = 0, lastConnectTime = 0;
	strinetAddr localAddr;
	strModbusTcpDevInfo * ptrSlaveDeviceListInfo = NULL;

	for(;;)   
	{
		
		sysTaskDelay(systemClockRateGet()/1);

        for (i = 0 ; i < MODBUS_TCP_MAX_SLAVE_NUM ; i++)
        {
			ptrSlaveDeviceListInfo = &gMBClntAndSlvDevInfo.slaveDeviceListInfo[i];

			stationID = ptrSlaveDeviceListInfo->slaveDeviceId;
			if( (stationID == 0) || (stationID > MODBUS_TCP_MAX_SLAVE_NUM)){
				if(stationID > MODBUS_TCP_MAX_SLAVE_NUM)
				{
                    status = RMT_DEVICE_TYPE_ERR;
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
				}
				continue;
			}

			
            if (BitTest(gMBClntAndSlvDevInfo.destStation[(stationID-1)>>5], ((stationID-1)&0x1F))) 
			{
                for(netIdx=0; netIdx< MODBUS_TCP_MAX_CNET; netIdx++) 
                {					
                    if( (ptrSlaveDeviceListInfo->tcpClntSock[netIdx] == -1) && (ptrSlaveDeviceListInfo->ipAddrInfoList[netIdx] != 0) )
                    {
						
                        timeInSec = 0;
                        (void)curTimeGetInSecond(&timeInSec);
						lastConnectTime = ptrSlaveDeviceListInfo->lastConnectTime[netIdx];

                        if( (abs(timeInSec - lastConnectTime) >= NET_CONNECT_RETRY_1_SEC) || (timeInSec == 0) || (lastConnectTime == 0) )
                        {
                            ptrSlaveDeviceListInfo->lastConnectTime[netIdx] = timeInSec;
                            memoryClear ((uint8 *)&localAddr, sizeof(strinetAddr));
			
                            status = addrInfoBuild( (void   *)&localAddr, 
                                                    &ptrSlaveDeviceListInfo->ipAddrInfoList[netIdx],
                                                    UNI_MODBUS_TCP_SEND_RECV_PORT
                                                    );
        
                            if(status != NO_ERROR)
                            {
								setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
                                continue;
                            }
   
                            
                            status = (int32)connectionEstablishInClient(	(void *) &localAddr,
                                                                            TCP_PROTO,
                                                                            (int32*)&clientSocket
                                                                            );
        
                            if(status)
                            {
								setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
                           		
								clearTcpConnection(stationID, netIdx, clientSocket);
                            }
                            else
                            {
                                setTcpConnection(stationID, netIdx, clientSocket);
                            }	
                        }
                    }
                
                }
            } 
		}
	}

    return;
}

static uint32   dataReadAndSendToCMDProcTask(uint32  sockId, uint32 proto_type)
{
	uint32  status = NO_ERROR;

	strMsgQueData       queData;
    strDataBuf          recvData;
    uint32				qDataLen;
    strSysConfigInfo    sysCnfgInfo;
    strNewNetProtocolInfo  *netPtr;

    memoryClear( (uint8 *) &recvData,   sizeof(strDataBuf));

    
    systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);

    
    recvData.socket_fd = sockId;

    
    status = dataRecvFromServerOrClient( &recvData,
                                         (void *)   &queData.peerAddr,
                                         proto_type
										 );


	{
#ifdef VXWORKS
		curLongDataConversion( (uint32*)recvData.data_buf, 	recvData.data_len);
#endif
	}
	
    if(status)
	{
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
	}
    else 
	{
		if( (sockId == gUniSendRecvSockFd_A) || (sockId == gUniSendRecvSockFd_B) )
		{
        	rmtRCSComCountSet(&recvData);
        }
        if(	myBroadcastDataCheck(
        			&sysCnfgInfo.netCnfgInfo,
        			queData.peerAddr.iAddr
        						) == MSG_MY_BROADCAST_DATA)
        {
            goto MODULE_END;
        }

        
        memoryCopy( (int8   *)queData.protoData.msgData,
                    (int8   *)recvData.data_buf,
                    recvData.data_len
                  );
        queData.protoData.numOfBytes = recvData.data_len;

        queData.peerAddr.sockFd =   sockId;

        
        if(gRecvInfoFlag)
            debugInfoSet(&queData, gDebugRecvData);

        
        if(gTransUnicastDetailRecvFlag || gTransBroadcastDetailRecvFlag ) {
            transInfoSet( &queData);
        }
        
        if(gTransRateCalcuFlag)
            networkPerformanceCountSet(RECV_PACKET_F,recvData.data_len); 
	    
	    netPtr = (strNewNetProtocolInfo  *)queData.protoData.msgData;
    
	    if( broadcastMsgCheck(
	                            netPtr,
	                            (strSysConfigInfo   *) &sysCnfgInfo
	                         )
	    )
	    {    	
		    goto MODULE_END;
		}            
        
        qDataLen = sizeof(strinetAddr) + queData.protoData.numOfBytes   +   4;
        
        if(qDataLen >   sizeof(strMsgQueData)) {
            status = QUE_DATA_LEN_ERR;
            setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
                                 "MAX BYTE   LEN",   sizeof(strMsgQueData),
                                 "Calcu Byte Len",   qDataLen,
                                 "Not Using", 0
                                );
            goto MODULE_END;
		}

	    status = messageQueueSend(   gRecvQueId,
                                     (int8 *)   &queData,
                                     qDataLen,
    	                             FOREVERWAIT,
                                     MYMSG_PRI_NORMAL
                                  );

		if(status)
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
	}

MODULE_END:
    return(status);
}


static uint32 clearTcpConnection(uint32 stationId, uint32 netIdx, int32 commFd)
{
	uint32  status = NO_ERROR;
	strModbusTcpDevInfo * ptrSlaveDeviceListInfo = NULL;
	int32 tempClntSock = 0;
	uint8 strIPAddr[32];
	uint32 i = 0;
	uint32 isClose = 0;
	static uint8    *lineKind[] =
	{
		(uint8 *)"MASTER A Line", 
		(uint8 *)"MASTER B Line", 
		(uint8 *)"SLAVE A Line",
		(uint8 *)"SLAVE B Line",  
		NULL
	};

	if(commFd <= 0){
		status = SOCK_FD_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		return(status);
	}

	ptrSlaveDeviceListInfo	= &gMBClntAndSlvDevInfo.slaveDeviceListInfo[stationId -1];

	
	if(netIdx == MODBUS_TCP_MAX_CNET)
	{
		for(i=0; i<MODBUS_TCP_MAX_CNET; i++)
		{
			if(ptrSlaveDeviceListInfo->tcpClntSock[i] == commFd)
			{
				netIdx = i;
				break;
			}
		}
	}

	if(netIdx == MODBUS_TCP_MAX_CNET)
	{
		return(status);
	}

    status = semaphoreGet(gTcpSocketSemaId, FOREVERWAIT);
	if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		goto MODULE_END;
	}

	if( (ptrSlaveDeviceListInfo->tcpClntSock[netIdx] > 0) && (netIdx < MODBUS_TCP_MAX_CNET) )
	{
		if (ptrSlaveDeviceListInfo->isConnected[netIdx] == TRUE) 
		{
			FD_CLR(ptrSlaveDeviceListInfo->tcpClntSock[netIdx], &gTcpFdVar);
			
			#ifdef _WIN32
			if (shutdown(ptrSlaveDeviceListInfo->tcpClntSock[netIdx], SD_BOTH) < 0) 
			#else
			if (shutdown(ptrSlaveDeviceListInfo->tcpClntSock[netIdx], SHUT_RDWR) < 0) 
			#endif
			{
				printf("shutdown\n");
				status = SOCK_FD_ERR;
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			}

			#ifdef _WIN32
			if (closesocket(ptrSlaveDeviceListInfo->tcpClntSock[netIdx]) < 0) 	
			#else
			if (close(ptrSlaveDeviceListInfo->tcpClntSock[netIdx]) < 0) 
			#endif
			{
				printf("close error\n");
				status = SOCK_FD_ERR;
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			}
			else
			{
				tempClntSock = ptrSlaveDeviceListInfo->tcpClntSock[netIdx];
				ptrSlaveDeviceListInfo->tcpClntSock[netIdx] = -1;
				ptrSlaveDeviceListInfo->isConnected[netIdx] = FALSE;
				isClose = 1;
			}	
		}
	}

	
#ifdef _WIN32
	status = semaphoreRelease(MUTEX_SEMA, gTcpSocketSemaId);
#else
	status = semaphoreRelease(gTcpSocketSemaId);
#endif
	if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
	}

	if(isClose)
	{
		memset(strIPAddr, 0x0, sizeof(strIPAddr)); 
		ipAddrHexToString((int8 *)strIPAddr, (uint32)ptrSlaveDeviceListInfo->ipAddrInfoList[netIdx]);

		printf("ERROR!!! [ST%d] %s Tcp Socket Close. IP : %s, SOCK : %d (Pre %d)  \n", 
				stationId, lineKind[netIdx], strIPAddr, ptrSlaveDeviceListInfo->tcpClntSock[netIdx], tempClntSock);
	}

MODULE_END:
	return(status);
}

static uint32 clearTcpConnectionWithIp(uint32 iAddr, uint32 netIdx, int32 commFd)
{
    uint32 stationId, slaveLoop, lineLoop;
	uint32  status = NO_ERROR;
	strModbusTcpDevInfo * ptrSlaveDeviceListInfo = NULL;
	
    stationId = 0; 
    
    for(slaveLoop=1;slaveLoop<=MODBUS_TCP_MAX_SLAVE_NUM;slaveLoop++)
    {
    	ptrSlaveDeviceListInfo	= &gMBClntAndSlvDevInfo.slaveDeviceListInfo[slaveLoop-1];

        if(ptrSlaveDeviceListInfo->slaveDeviceId != 0)
        {        
    		for(lineLoop=0; lineLoop<MODBUS_TCP_MAX_CNET; lineLoop++)
    		{
    			if(ptrSlaveDeviceListInfo->ipAddrInfoList[lineLoop] == iAddr)
    			{
    				stationId = slaveLoop;
    				break;
    			}
    		}
		}
		
		if(stationId != 0) 
		{   
		    status = clearTcpConnection(stationId, netIdx, commFd);
		    if(status)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		    break;
		}
    }
        
    if(stationId == 0)
    {
    }

	return(status);
}

static uint32 setTcpConnection(uint32 stationId, uint32 netIdx, int32 commFd)
{
	uint32 status = NO_ERROR;
	uint8 strIPAddr[32];
	strModbusTcpDevInfo * ptrSlaveDeviceListInfo = NULL;
	uint8    *lineKind[] =
	{
		(uint8 *)"MASTER A Line", 
		(uint8 *)"MASTER B Line", 
		(uint8 *)"SLAVE A Line",
		(uint8 *)"SLAVE B Line",  
		NULL
	};

	if(commFd <= 0){
		status = SOCK_FD_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		return(status);
	}

	ptrSlaveDeviceListInfo = &gMBClntAndSlvDevInfo.slaveDeviceListInfo[stationId -1];

	status = semaphoreGet(gTcpSocketSemaId, FOREVERWAIT);
	if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		goto MODULE_END;
	}

	ptrSlaveDeviceListInfo->tcpClntSock[netIdx] = commFd;
    ptrSlaveDeviceListInfo->isConnected[netIdx] = TRUE;

	FD_SET(commFd, &gTcpFdVar);

	
	ptrSlaveDeviceListInfo->dataRecvLiveCount[netIdx] = 1;

	
	ptrSlaveDeviceListInfo->lastStatCheckTime[netIdx] = 0;

	
#ifdef _WIN32
	status = semaphoreRelease(MUTEX_SEMA, gTcpSocketSemaId);
#else
	status = semaphoreRelease(gTcpSocketSemaId);
#endif
	if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
	}

	memset(strIPAddr, 0x0, sizeof(strIPAddr)); 
	ipAddrHexToString((int8 *)strIPAddr, (uint32)ptrSlaveDeviceListInfo->ipAddrInfoList[netIdx]);

	if(gDebugErrStringFlag == DEBUG_ENABLE) printf(">>>>>* [ST%d] %s Tcp Socket Connect. IP : %s, SOCK : %d  \n", 
			stationId, lineKind[netIdx], strIPAddr, 
			ptrSlaveDeviceListInfo->tcpClntSock[netIdx]);

MODULE_END:
	return(status);
}

strModbusTcpDevInfo * getDeviceInfoPrt(uint32 stationId, strMBClntAndSlvDevInfo * mbClntAndSlvDevInfoPtr) 
{
	uint32	status = NO_ERROR;
	uint32 i = 0;
	uint32 isFound = FALSE;
	strModbusTcpDevInfo *slaveDeviceInfoPtr = NULL;

	
	for(i=0; i< MODBUS_TCP_MAX_SLAVE_NUM; i++)
	{
		if(stationId == mbClntAndSlvDevInfoPtr->slaveDeviceListInfo[i].slaveDeviceId)
		{
			slaveDeviceInfoPtr = &mbClntAndSlvDevInfoPtr->slaveDeviceListInfo[i];
			isFound = TRUE;
			break;
		}
	}

	if(! isFound)
	{
		status = STATION_ID_RANGE_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
	}

	return slaveDeviceInfoPtr;
}


static uint32 setDataLiveCount(uint32 stationId, uint32 netIdx)
{
	uint32 status = NO_ERROR;
	strModbusTcpDevInfo * ptrSlaveDeviceListInfo = NULL;

	ptrSlaveDeviceListInfo = getDeviceInfoPrt(stationId, &gMBClntAndSlvDevInfo);

	 
	if(ptrSlaveDeviceListInfo->dataRecvLiveCount[netIdx] > 0x7fffffff)
	{
		ptrSlaveDeviceListInfo->dataRecvLiveCount[netIdx] = 1; 
	}
	else
	{
		ptrSlaveDeviceListInfo->dataRecvLiveCount[netIdx]++;
	}

	return(status);
}

static uint32   myBroadcastDataCheck(   strControlNetConfigInfo *configPtr,
                                    	uint32 ipHexVal
                                    )
{
    uint32 retVal   =   0;
 
    
    
    if( (configPtr->myIpAddr[0]   == ipHexVal)
                        ||
        (configPtr->myIpAddr[1]   == ipHexVal)
    ){
		retVal = MSG_MY_BROADCAST_DATA;
    }

    
    netCommCountSetForRas(configPtr, ipHexVal);

    return(retVal);
}


static void netCommCountSetForRas(strControlNetConfigInfo   *netCnfgInfoPtr, uint32 ipHexVal)
{
    
    
    
    gCurNetCommCount++;

    if(gCurNetCommCount > 0x7fff)
    	gCurNetCommCount = 1;

#if NOT_USED
	
    
    if((sysStatusInfoPtr->sysRedundancyStatus != SYS_RAS_REDUNDANCY_STANDBY)
    				||
    	(diuStatusInfoPtr->redundancyStatus != NO_ERROR)
    )
    	return;
#endif
    	
	
    if( (netCnfgInfoPtr->rmtIpAddr[0]   == ipHexVal)
                        ||
        (netCnfgInfoPtr->rmtIpAddr[1]   == ipHexVal)
    ){
    	gRmtShelfCurNetCommCount++;

        if(gRmtShelfCurNetCommCount >   0x7fff)
        	gRmtShelfCurNetCommCount = 1;
    }
	
	return;         
}


void usrNCmdTask (arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)
int32   arg1,   arg2,   arg3,   arg4,   arg5,arg6, arg7, arg8, arg9, arg10;
{
	strMsgQueData       queData;
	int32       retLen, status = NO_ERROR;
#ifdef _WIN32  
__try {
#endif
	for(;;)  {
		
		
		status = messageQueueRecv(  gRecvQueId,
									(int8   *)&queData,
									sizeof(strMsgQueData),
									FOREVERWAIT,
									&retLen
								);
		if(status   == NO_ERROR) {
			status = commandProcess(&queData);
		}

		if(status   != NO_ERROR)
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  

	}
#ifdef _WIN32
  }
__except( exceptFilterFunc(GetExceptionInformation(), __FILE__,__LINE__ ) ) {
	taskSuspendToListStatus(GetCurrentThreadId());
}

#endif

}



uint32 commandProcess(strMsgQueData *recvPtr)
{
    uint32 status   =   NO_ERROR;
    strNewNetProtocolInfo  respMsg;
    strNewNetProtocolInfo  *netPtr;

    uint32  qDataLen;
    strBufInfo  respData;
    uint32  calcuChecksum   =   0;
    strMsgQueData   queData;
    strSysConfigInfo    sysCnfgInfo;

    uint32 *cmdCode, opCode, optionCode;
		
    netPtr = (strNewNetProtocolInfo *)recvPtr->protoData.msgData;

    systemCnfgInfoRead( (strSysConfigInfo   *) &sysCnfgInfo);

	cmdCode = (uint32*)(netPtr->dataBuff);
	
 	opCode 		= (*cmdCode)>>16;
	optionCode 	= (*cmdCode)&0xffff;

	
	if( (status = appMsgProtocolHeadCheck(netPtr,recvPtr->protoData.numOfBytes) ) 
			  !=   NO_ERROR)
	{
        setErrorCodeWithVal( __FILE__,   __LINE__, __FUNCTION__, status,
							   (uint8*)"GROUP CMD ", opCode, 
							   (uint8*)"ACTION CODE ", optionCode,
							   (uint8*)"SEQ ID", netPtr->sequenceID);
		goto MODULE_END;
	}

	

#if DUAL_LINE_SRC_INCLUDE
    
#if DUAL_LINE_DEBUGGING
    if( (sysCnfgInfo.netCnfgInfo.ctrlNetDualStatus ==   CTRL_NET_LINE_DUAL) 
                                &&
        ( (status = dLMsgCheck(recvPtr))   != NO_ERROR)
    ){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
        goto MODULE_END;
    }
#else
    if( (sysCnfgInfo.netCnfgInfo.ctrlNetDualStatus ==   CTRL_NET_LINE_DUAL) 
                                &&
        (dLMsgCheck(recvPtr) >   0)
    ){
        goto MODULE_END;
    }
#endif

#endif

	
    memoryClear (   (uint8 *)   &respData, sizeof(strBufInfo));

	{
		switch(netPtr->messageType) {
			case REQUEST:
				status = requestCmdProcess( netPtr, 
										   (strBufInfo *) &respData,   
											&recvPtr->peerAddr
										);
				if(status   != NO_ERROR)
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			break;
			
			case RESPONSE:
				{
					status = responseCmdProcess(netPtr);
					if(status   != NO_ERROR)
						setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
				}
			break;
			
			case BROADCAST:
				{ 
					status = broadcastCmdProcess (netPtr);
					if(status   != NO_ERROR)
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
				}
			break;

			default:
				status = L_MSG_TYPE_ERR;
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
				goto MODULE_END;
			break;
		}
	}
	
    if( (netPtr->messageType ==   REQUEST)
	  )
	{
        /* 응답 데이터의 길이   Check   */
        if(respData.dataLen >   (MAX_NET_MSG_LEN - MAX_NEW_FRAME_HEAD_LEN)) {
            status = DATA_LEN_EXCEED_ERR;
            setErrorCodeWithVal   (   __FILE__, __LINE__,
                                       "commandProcess", status,
                                       "Calcu Data Len",   respData.dataLen,
                                       "Recv Op Code",   opCode,
                                       "Recv Option Code",   optionCode
                                    );
            goto MODULE_END;
        }

        /* 응답 메세지를 메세지를   송신하는 타스크로   보낸다 */
        memoryClear( (uint8 *) &respMsg, sizeof(strNewNetProtocolInfo));

        /* 프로토콜 Header copy - REQUEST인경우 SEQUENCE ID는 상위에서 결정 */
        memoryCopy( (uint8 *)   &respMsg,
                    (uint8 *)   netPtr,
                    MAX_NEW_FRAME_HEAD_LEN  /* 40   바이트 */
                    );

        /* 데이터   부분 Copy   */
        memoryCopy ( (uint8 *)respMsg.dataBuff,
                     (uint8 *)respData.dataBuf,
                     respData.dataLen
                     );
        
        /* Source, Dest Id, Dual Id 변경 */
        respMsg.destinationID   =   netPtr->sourceID;

		{
			respMsg.sourceID	=   netPtr->destinationID;
			respMsg.dualStatus  =   netPtr->dualStatus;
		}

		/* 메세지   타입을 RESPONSE로   변경 및 프레임 길이 변경 */
        respMsg.messageType     =   RESPONSE;
        respMsg.dataLength  =   respData.dataLen ;

        /* CheckSum 계산 - 헤더포함된 Data에서 Head부분을 제외한 체크섬  */ 
        (void)netMsgChecksumCalcu   (   respMsg.dataLength,
                                        (uint32 *) &respMsg.dataBuff,
	                                    (uint32 *) &calcuChecksum
                                    );
        /* Checksum을   opcode, option code에   attach */
		respMsg.checkSum = calcuChecksum;
		
        /* 송신 타스크에 보냄   */
        memoryClear (   (uint8 *)&queData, sizeof(strMsgQueData));

        /* 네트웍   정보 Get    comPort.c에 정의 */
        status = comPortInfoGet(RESPONSE,   &recvPtr->peerAddr, &queData.peerAddr);
        if(status   != NO_ERROR){
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
            goto MODULE_END;
        }

        /* 데이터   Copy */
        queData.protoData.numOfBytes = MAX_NEW_FRAME_HEAD_LEN + respMsg.dataLength; 
        memoryCopy( (int8   *)queData.protoData.msgData,
                    (int8   *)&respMsg,
                    queData.protoData.numOfBytes
                  );
        /* qDataLen =   netAddr크기 +   데이터 사이즈   +   데이터 길이를   나타내는 */
        /* 바이트                                 */
        qDataLen = SOCK_ADDR_LENGTH +   queData.protoData.numOfBytes + 4;
        status = messageQueueSend(  gSendQueId,
                                    (int8   *) &queData,
                                    qDataLen,
                                    FOREVERWAIT,
                                    MYMSG_PRI_URGENT
                                  );
        if(status   != NO_ERROR)
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
    }
MODULE_END:
    return(status);
}


static uint32   responseCmdProcess (strNewNetProtocolInfo  *netPtr)
{
    uint32  status = NO_ERROR;
    uint32  *cmdCode, opCode , optionCode;
	
	cmdCode = (uint32*)(netPtr->dataBuff);
 	opCode = (*cmdCode)>>16;
	optionCode = (*cmdCode)&0xffff;

    status = optionCodeCheck (opCode,   optionCode);

    if(status   != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
        goto MODULE_END;
    }
         
    switch(opCode) {
        case ETHERNET_DATA_ACC_OP:
           status = sysRmtDataAccRespCmdOPFuncList[optionCode] (netPtr);
        break;
        case ETHERNET_COM_PATH_OP:
           status = sysRmtStatusRespCmdOPFuncList[optionCode] (netPtr);
        break;
        default:
            status = L_OP_CODE_ERR;
        break;
	}
MODULE_END:

    return(status);
}


uint32 broadcastCmdProcess (strNewNetProtocolInfo *netPtr)
{
    uint32  status = NO_ERROR;
    uint32  *cmdCode, opCode,optionCode;
	strBufInfo tempBuf;

	cmdCode = (uint32*)(netPtr->dataBuff);
 	opCode = (*cmdCode)>>16;
 	optionCode = (*cmdCode)&0xffff;

    switch(opCode) {      
        case ETHERNET_DATA_ACC_OP:
			if(optionCode == ETHERNET_DATA_BROAD )
        		status = sysRmtDataAccRespCmdOPFuncList[optionCode]  ( netPtr );
			else status = L_OP_CODE_ERR;
        break;

        case TIME_ACCESS_CMD:
			if(optionCode == TIME_SYNC_BROAD)
          		status = sysTimeCmdOPFuncList[optionCode](netPtr,(strBufInfo *) &tempBuf);
			else status = L_OP_CODE_ERR;
        break;

        default:
            status = L_OP_CODE_ERR;
        break;
    }

        return(status);
}


uint32  requestCmdProcess   
                    (
 			            strNewNetProtocolInfo  *netPtr,
                        strBufInfo          *respMsgPtr,
                        strinetAddr         *peerAddr
                    )
{
    uint32  status = NO_ERROR;
    static strBufInfo  respBuf;
    uint32  *cmdCode, opCode, optionCode;
    strSysConfigInfo    sysCnfgInfo;
	strFbDataGrHeaderInfo * reqHeaderptr;

    systemCnfgInfoRead( (strSysConfigInfo   *) &sysCnfgInfo);
    
    memoryClear (   (uint8 *)   &respBuf,   sizeof(strBufInfo));

	
	cmdCode = (uint32*)(netPtr->dataBuff);
 	opCode = (*cmdCode)>>16;
	optionCode = (*cmdCode)&0xffff;
    
    
    
    
	if( 	(netPtr->destinationID != sysCnfgInfo.stationId)
		) {
		status = L_DEST_STATION_ID_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		goto MODULE_END;				
	} 
	  
    status = optionCodeCheck (opCode, optionCode);

    if(status   != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		goto MODULE_END;
    }        

    switch(opCode) {
        case CODE_ACCESS_CMD:
			status = systemCnfgInfoRead( (strSysConfigInfo   *) &sysCnfgInfo);
			if(status != NO_ERROR)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			else {
				reqHeaderptr = (strFbDataGrHeaderInfo *)netPtr->dataBuff;
				if(optionCode < IO_CONFIG_READ) {
					status = fbDataGrHeaderCheck(reqHeaderptr, &sysCnfgInfo);
					if(status) {
						setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
						goto MODULE_END;
					}
				}
				status = CodeAccessCmdOPFuncList[optionCode](netPtr,(strBufInfo *) &respBuf);
				if(status != NO_ERROR)
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			}
        break;

        case MODE_ACCESS_CMD:
			status = CtrlModeAccessCmdOPFuncList[optionCode](netPtr,(strBufInfo *) &respBuf);
			if(status != NO_ERROR)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
        break;
		
        case VAR_ACCESS_CMD:
			status = VariableAccessCmdOPFuncList[optionCode](netPtr,(strBufInfo *) &respBuf);
			if(status != NO_ERROR)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
        break;

        case TIME_ACCESS_CMD:
			if(optionCode != TIME_SYNC_BROAD){
				status = sysTimeCmdOPFuncList[optionCode](netPtr,(strBufInfo *) &respBuf);
				if(status != NO_ERROR)
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			}	
			else {
				status = L_OP_CODE_ERR;
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			}
        break;
		
        case FRC_ACCESS_CMD:
			status = forcingCmdOPFuncList[optionCode](netPtr,(strBufInfo *) &respBuf);
			if(status != NO_ERROR)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
        break;

		case SYS_STATUS_CMD:
			if(optionCode != SYS_DIAG_STATUS){
        		status = sysStatusCmdOPFuncList[optionCode]  ( netPtr,(strBufInfo *) &respBuf);
				if(status != NO_ERROR)
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			}
			else {
				status = L_OPTION_CODE_ERR;
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
			}
		break;

		case FB_ONLINE_DOWNLOAD_CMD:
			status = fbOnlineDwnCmdOPFuncList[optionCode](netPtr,(strBufInfo *) &respBuf);
			if(status != NO_ERROR)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		break;

        case ETHERNET_DATA_ACC_OP:
        	status = sysRmtDataAccReqCmdOPFuncList[optionCode]  ( netPtr,(strBufInfo *) &respBuf);
			if(status != NO_ERROR)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
        break;

        case ETHERNET_COM_PATH_OP:
            status = sysRmtStatusReqCmdOPFuncList[optionCode]   ( netPtr, (strBufInfo *) &respBuf,
                                                         peerAddr);
			if(status != NO_ERROR)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
        break;
        
		default:
 	       	status = L_OP_CODE_ERR;
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
        break;
	}

MODULE_END:

	respMsgPtr->dataLen = respBuf.dataLen ;

	memoryCopy (    (uint8 *)   respMsgPtr->dataBuf,
					(uint8 *)   respBuf.dataBuf,
					respBuf.dataLen
				);
				
    return(status);
}



#if 1 
extern void   systemRasInfoSet(uint32 , uint32 );
extern void   logMessageSet (int8 *, uint32, int8 *, uint32, uint32);
#endif


static uint32   broadcastMsgCheck   (
                                        strNewNetProtocolInfo *msgPtr,
 	                                    strSysConfigInfo    *shelfCfgPtr
                                    )
{
	uint32  status = NO_ERROR;
	strSysRunningConfigInfo		*sysStatusInfoPtr = gSysRunningInfoPtr;
	strMpuDiuRunningStatusInfo	*diuRasPtr
				= &gSysRunningStatusInfo.sysDiuRunningStatusInfo;
	strRasHeadInfo *mRasHeadInfo;
	uint8 retVal = 0;
    uint32  *cmdCode, opCode, optionCode;

	cmdCode 	= (uint32*)(msgPtr->dataBuff);
 	opCode 		= (*cmdCode)>>16;
	optionCode 	= (*cmdCode)&0xffff;
	
	
	
	if((opCode & 0xffff) ==   XR_BROADCAST_CMD){
		status = MSG_GARBAGE_DATA;
	}
	
	else if(  ((opCode & 0xffff) ==   SYS_STATUS_CMD) && 
		 ((optionCode & 0xffff) ==   SYS_DIAG_STATUS) 
	) {
#if 1  
	
		if( (msgPtr->sourceID == shelfCfgPtr->stationId ) &&
			(
				( (msgPtr->dualStatus == MASTER_SHELF) && (shelfCfgPtr->shelfType == SLAVE_SHELF ) ) ||
				( (msgPtr->dualStatus == SLAVE_SHELF) && (shelfCfgPtr->shelfType == MASTER_SHELF) ) 
			)
			
		  )
		{
			if(sysStatusInfoPtr->sysOperationMode == SYS_RAS_SYSTEM_AUTO_MODE ) 
			{
				mRasHeadInfo = (strRasHeadInfo*)&msgPtr->dataBuff[0];
			
				if( ( (mRasHeadInfo->dualStatus >> 4 & 0x3) == SYS_RAS_REDUNDANCY_RUN) &&
					( (mRasHeadInfo->dualStatus & 0x3) == SYS_RAS_SYSTEM_RUN ) 
				){
					
					if (	(diuRasPtr->redundancyStatus !=NO_ERROR)	&&
							(shelfCfgPtr->shelfType == SLAVE_SHELF)	&&
							(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN)	
					) { 
						 systemRasInfoSet(RAS_REDUNDANCY_STATUS, SYS_RAS_REDUNDANCY_STANDBY);
						 systemRasInfoSet(RAS_SYSTEM_STATUS, SYS_RAS_SYSTEM_STOP);

						 
						logMessageSet(__FILE__, __LINE__,"broadcastMsgCheck", SYS_FORCED_REBOOT, 0);
			    		boardReboot();
					}
				}
			}

		}
#endif
		status = MSG_GARBAGE_DATA;
	}

	else if(  (msgPtr->messageType   == BROADCAST)&&
			  (( (opCode &   0xffff) == ETHERNET_DATA_ACC_OP) && (optionCode == ETHERNET_DATA_BROAD) 	)
		   ) 
	{
		if( msgPtr->sourceID == shelfCfgPtr->stationId) 
			status = MSG_MY_BROADCAST_DATA;
	}
	return(status);
}



uint32  optionCodeCheck(uint32  opCode, uint32 optionCode)
{
    uint32  status = NO_ERROR;

    switch(opCode) {
 	   	case CODE_ACCESS_CMD:
    		if( (optionCode <   CODE_SEG_DOWN) ||   (optionCode > SFC_STEP_JUMP))
	        	status = L_OPTION_CODE_ERR;
	    break;
    	case MODE_ACCESS_CMD:
        	if( (optionCode <   MODE_SEG_READ) || (optionCode > MODE_CONTROL_WRITE))
            	status = L_OPTION_CODE_ERR;
	    break;
		case VAR_ACCESS_CMD:
			if( (optionCode <   VAR_BLOCK_READ)   || (optionCode > VAR_ALLMEM_WRITE))
				status = L_OPTION_CODE_ERR;
		break;

		case TIME_ACCESS_CMD:
			if( (optionCode <   TIME_SYNC_BROAD)   || (optionCode > TIME_WRITE_RCS))
				status = L_OPTION_CODE_ERR;
		break;

		case FRC_ACCESS_CMD:
			if( (optionCode <  FRC_FLAG_WRITE)   || (optionCode > FRC_TAKS_WRITE))
				status = L_OPTION_CODE_ERR;
		break;

		case SYS_STATUS_CMD:
			if( (optionCode <  SYS_DIAG_STATUS)   || (optionCode > SYS_DATA_WTITE))
				status = L_OPTION_CODE_ERR;
		break;		

		case XR_BROADCAST_CMD:
			if( (optionCode <  XR_SEND)   || (optionCode > XR_SEND))
				status = L_OPTION_CODE_ERR;
		break;

		case FB_ONLINE_DOWNLOAD_CMD:
			if( (optionCode <  FB_ONLINE_DOWNLOAD_ADD)   || (optionCode >IO_ONLINE_DOWNLOAD_SPEC_CHG))
				status = L_OPTION_CODE_ERR;
		break;
		
		case ETHERNET_DATA_ACC_OP:
			if( (optionCode <  ETHERNET_DATA_READ)   || (optionCode >ETHERNET_DATA_BROAD))
				status = L_OPTION_CODE_ERR;
		break;

		case ETHERNET_COM_PATH_OP:
			if( (optionCode <  ETHERNET_DEV_STATUS_READ)   || (optionCode >ETHERNET_DEV_STATUS_READ))
				status = L_OPTION_CODE_ERR;
		break;

		default:
			status = L_OP_CODE_ERR;
		break;
	}   

	return(status);
}


uint32 fbDataGrHeaderCheck(strFbDataGrHeaderInfo * msgPtr,strSysConfigInfo  *shelfCfgPtr)
{
    uint32  status = NO_ERROR;
	
	
	if (
          (msgPtr->memoryKind != LOGIC_ID)
       )
		 status = MSG_MODULE_ID_ERR;

	return(status);
}


void usrNetSTask(arg1, arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)
int32   arg1,   arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10;
{
	strMsgQueData       queData;
	int32   status = NO_ERROR, retLen;
	strinetAddr         *peerAddr, tmpAddr[2]; 
	strDataBuf          sendData;
	strDataBuf			convertData;
	strSysConfigInfo    sysCnfgInfo;
	uint32  i   =0, packetNum   =   0, rmtDevType;
    uint32  *cmdCode, opCode, optionCode;
    uint32  calcuCheckSum   =   0;
	strNewNetProtocolInfo  *lgPtr;

#if 000
	uint8 deviceType = 0;
#else
	uint32 deviceType = 0;
	int32 nonblocking = 0;
	uint32 *devicePtr = NULL;
#endif
#ifdef _WIN32  
__try {
#endif
	
	memoryClear ( (uint8 *) &queData, sizeof(strMsgQueData));
	systemCnfgInfoRead( (strSysConfigInfo *) &sysCnfgInfo);

	for(;;)   
	{
		
		status = messageQueueRecv(   gSendQueId,
									(int8 *)&queData,
									sizeof(strMsgQueData),
									FOREVERWAIT,
									&retLen
									);
		if(status)
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
		else 
		{
			lgPtr		=   (strNewNetProtocolInfo *)queData.protoData.msgData;
			
			
			cmdCode		= (uint32*)(lgPtr->dataBuff);
 			opCode		= (*cmdCode)>>16;
			optionCode	= (*cmdCode)&0xffff;
			
			deviceType = 0;

			memoryCopy ( (int8 *)&tmpAddr[0],
						 (int8 *)&queData.peerAddr,
						 sizeof(strinetAddr)
						 );

			
			netMsgChecksumCheckForDebugging(lgPtr->dataLength,
											(strNewNetProtocolInfo *)lgPtr
											);

			
			if( (sysCnfgInfo.netCnfgInfo.ctrlNetDualStatus == CTRL_NET_LINE_DUAL) &&
				( (status = dualLinePortInfoGet((strinetAddr *)&tmpAddr[0],(strinetAddr *)&tmpAddr[1],
								(strNewNetProtocolInfo *)lgPtr) )  == NO_ERROR ) )
			{
				packetNum   =   2;
			}
			else 
				packetNum = 1;

			for(i = 0; i < packetNum;   i++) 
			{
				peerAddr = &tmpAddr[i];

				if(peerAddr->sockFd == 0)   
				{
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, SOCK_FD_ERR);  
					continue;
				}
				
				if ((opCode == ETHERNET_DATA_ACC_OP)||(opCode == ETHERNET_COM_PATH_OP))
				{	
					if(peerAddr->iAddr == 0) 
						continue;
				}
				else 
				{
					if(peerAddr->iAddr == 0) 
					{ 
						setErrorCode( __FILE__, __LINE__, __FUNCTION__, SOCK_FD_ERR);  
						continue;
					}					
				}

				
				if(gTransUnicastDetailSendFlag || gTransBroadcastDetailSendFlag)
				{
					memoryCopy ((int8 *)&(queData.peerAddr),
								(int8 *)peerAddr,
								sizeof(strinetAddr)
								);
					transInfoSet( &queData);
				}

				
				sendData.socket_fd = peerAddr->sockFd;

				
				if ((opCode == ETHERNET_DATA_ACC_OP)||(opCode == ETHERNET_COM_PATH_OP))
				{
					if(i == 0)   
					{   
						status = rmtDevProtocolEncode(lgPtr, &sendData);
						if(status   != NO_ERROR) {
							setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
							continue;
						}
					}
				}
				else 
				{ 					
					sendData.data_len   = queData.protoData.numOfBytes;
					sendData.socket_fd 	= peerAddr->sockFd;
					memoryCopy( sendData.data_buf,
								(uint8 *)   lgPtr,
								(uint32)sendData.data_len
								);
				}
				
#ifdef VXWORKS
				longDataConversion(	(uint32 *)convertData.data_buf,	
									(uint32 *)sendData.data_buf,
									sendData.data_len
								  );			

				convertData.data_len	= sendData.data_len;	
				convertData.socket_fd	= sendData.socket_fd;
			
				status = dataSendToServerOrClient( &convertData, (void *)peerAddr,UDP_PROTO);
#else
				status = dataSendToServerOrClient( &sendData, (void *)peerAddr,UDP_PROTO);
#endif
				if(status != NO_ERROR)
				{
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
				}

				if(status) 
				{
					setErrorCodeWithVal(	__FILE__, 
											__LINE__,
											"usrNetSTask", 
											status,
											"Loop ID",i,
											"NOT_USING", 0,
											"NOT_USING", 0
								  		);				
				}
				else 
				{
					
					if ((opCode == ETHERNET_DATA_ACC_OP) ||(opCode == ETHERNET_COM_PATH_OP) )	
					{
						rmtDevMsgInfoSet (&sendData, lgPtr);
					}					
				}

				
				if(gSendInfoFlag)
				{
					memoryCopy ( (int8 *)&(queData.peerAddr),
								(int8 *)peerAddr,
								sizeof(strinetAddr)
								);
					debugInfoSet(&queData, gDebugSendData);
				}	

				
				if(gTransRateCalcuFlag)
					networkPerformanceCountSet( SEND_PACKET_F,
												sendData.data_len
												); 
			}
		}
	}

#ifdef _WIN32
  }
__except( exceptFilterFunc(GetExceptionInformation(), __FILE__,__LINE__ ) ) {
	taskSuspendToListStatus(GetCurrentThreadId());
}
#endif
}


uint32  netDataSendInFbTask(strNewNetProtocolInfo  *sendDataPtr)
{
    uint32  status = NO_ERROR;
    strMsgQueData       queData;
    uint32  qDataLen,   calcuCheckSum   =   0;

    
    if((sendDataPtr->messageType != REQUEST)&&(sendDataPtr->messageType != BROADCAST))
	    return(L_MSG_TYPE_ERR);

    
    (void)netMsgChecksumCalcu(
                                sendDataPtr->dataLength,
								(uint32 *) sendDataPtr->dataBuff,
                                (uint32 *) &calcuCheckSum
                             );

	sendDataPtr->checkSum = calcuCheckSum;  
	
    status = ethernetDataShareComInfoGet(   &queData.peerAddr,
                                            sendDataPtr,
                                            NET_LINE_LINK_A
											);
    if(status == NO_ERROR) 
	{
        queData.protoData.numOfBytes = sendDataPtr->dataLength + MAX_NEW_FRAME_HEAD_LEN ;

        memoryCopy( (int8   *)queData.protoData.msgData,
                    (int8   *)sendDataPtr,
                    queData.protoData.numOfBytes
                  );
       	                                
        qDataLen = SOCK_ADDR_LENGTH + queData.protoData.numOfBytes + 4;

        status = messageQueueSend(  gSendQueId,
                                    (int8   *) &queData,
                                    qDataLen,
                                    NOWAIT,
                                    MYMSG_PRI_URGENT
                                 );
        if(status)
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
	}

	if(status)
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  
	
    return(status);
}


static uint32    appMsgProtocolHeadCheck(
                                         strNewNetProtocolInfo  *msgPtr,
                                         uint32 netFlameLength
										 
                                     )
{
    uint32  status = NO_ERROR;
    uint32  calcuCheckSum   =   0;
    uint32  recvCheckSum    =   0;
    uint32  *cmdCode, opCode;

	

	cmdCode = (uint32*)(msgPtr->dataBuff);
 	opCode = (*cmdCode)>>16;
	
     


	
    
    if(
        (msgPtr->dataLength > MAX_NET_MSG_LEN - MAX_NEW_FRAME_HEAD_LEN) 
                           ||
        (msgPtr->dataLength < 0 ) 
     ){
		status = MSG_LEN_EXCEED_ERR;
        setErrorCodeWithVal   (
                        __FILE__,__LINE__,__FUNCTION__, status,
                        "Recv Data Len", netFlameLength,
						"Header Data Len", MAX_NEW_FRAME_HEAD_LEN,
					    "Msg Data Len ", msgPtr->dataLength
                         );

	}
    else if (
        netFlameLength != (MAX_NEW_FRAME_HEAD_LEN + msgPtr->dataLength)
            ){
        status = L_NET_MSG_LEN_ERR;
        setErrorCodeWithVal   (
                        __FILE__,__LINE__,__FUNCTION__, status,
                        "Recv Data Len", netFlameLength,
						"Header Data Len", MAX_NEW_FRAME_HEAD_LEN,
					    "Msg Data Len ", msgPtr->dataLength
                         );
    }
    
	

    
    else if(
			(msgPtr->messageType < REQUEST) 
                             ||
            (msgPtr->messageType > BROADCAST)
            )
		status = MSG_TYPE_ERR;

#if NOT_USED
	
	
    else if (stringCompareWithLen(msgPtr->companyID, COMPANY_ID, 8) != 0)
	{
        status = MSG_COMPANY_ID_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);  

    }
#endif	
	
    else if (
                ( opCode < MIN_PROTOCOL_CMD) 
                                 ||
                ( opCode > MAX_PROTOCOL_CMD) 
            )
		status = MSG_OPCODE_ERR;

	else{ 
		
		recvCheckSum    =   msgPtr->checkSum;
		(void)netMsgChecksumCalcu(	msgPtr->dataLength,
									(uint32*)  msgPtr->dataBuff,
									(uint32 *) &calcuCheckSum
							 	 );
		if(recvCheckSum != calcuCheckSum)
			status = MSG_CHECKSUM_ERR;
	}
    return(status);
}

uint32    rmtShelfCheckCommConnectInit(void)
{
   	int32    status =   NO_ERROR;
   	strinetAddr local;
	strSysConfigInfo *sysCnfgInfoPtr = &gSysCnfgInfo;
    strControlNetConfigInfo *netInfoPtr;

    netInfoPtr = &sysCnfgInfoPtr->netCnfgInfo;

	memoryClear((int8 *)&local, sizeof(strinetAddr));

	
	
	status = addrInfoBuild( 
							(void   *)&local,
                            &netInfoPtr->myIpAddr[0],
                            UNI_DUAL_CHK_UDP_SEND_RECV_PORT_A
                          );
	
    if(status == NO_ERROR){
    	status = (int32)connectionEstablishInClient (
				                            (void   *) &local,
#ifdef VXWORKS
																	UDP_PROTO,
#else
																	UDP_UNICASTING,
#endif
				                            (int32*)&sUniDualChkSndRcvSockFd_A
                                                    );
    	if(status != NO_ERROR){
	    	printf("[ERROR] - %s(%d)connectionEstablishInClient() Call Error for Line-A[0x%x] !!!\n", 
	    		__FILE__, __LINE__, (int32)netInfoPtr->myIpAddr[0]);
    		return(status);
    	}
    }
    else {
    	printf("[ERROR] - %s(%d) addrInfoBuild() Call Error for Line-A !!!\n", __FILE__, __LINE__);
    	return(status);
    }
    	
    
    if(netInfoPtr->ctrlNetDualStatus == CTRL_NET_LINE_DUAL) {
		status = addrInfoBuild( 
								(void   *)&local,
	                            &netInfoPtr->myIpAddr[1],
	                            UNI_DUAL_CHK_UDP_SEND_RECV_PORT_B
	                          );
		
	    if(status == NO_ERROR){
	    	status = (int32)connectionEstablishInClient (
				                            (void   *) &local,
#ifdef VXWORKS
																	UDP_PROTO,
#else
																	UDP_UNICASTING,
#endif
				                            (int32*)&sUniDualChkSndRcvSockFd_B
	                                                    );
	    	if(status != NO_ERROR){
	    	printf("[ERROR] - %s(%d)connectionEstablishInClient() Call Error for Line-B[0x%x] !!!\n", 
	    		__FILE__, __LINE__, (int32)netInfoPtr->myIpAddr[1]);
	    		return(status);
	    	}
    	}
	    else {
	    	printf("[ERROR] - %s(%d)addrInfoBuild() Call Error for Line-B !!!\n", __FILE__, __LINE__);
	    	return(status);
	    }
	}
	
	
    
    FD_ZERO( &sDualChkReadFds   );

    if(netInfoPtr->ctrlNetDualStatus == CTRL_NET_LINE_DUAL) {
	    FD_SET( sUniDualChkSndRcvSockFd_A, &sDualChkReadFds);
	    FD_SET( sUniDualChkSndRcvSockFd_B, &sDualChkReadFds);

	    sDualChkOpenSockNum = 2;
	    sDualOpenSockFd[0] = sUniDualChkSndRcvSockFd_A;
	    sDualOpenSockFd[1] = sUniDualChkSndRcvSockFd_B;
	}
	else {
	    FD_SET( sUniDualChkSndRcvSockFd_A, &sDualChkReadFds);
	    sDualChkOpenSockNum = 1;
	    sDualOpenSockFd[0] = sUniDualChkSndRcvSockFd_A;
	    sDualOpenSockFd[1] = 0;
	}
    return(status);
}


uint32	rmtShelfCheckByCtrlNetCom (uint32 command, uint32 *rmtSysRunStatus, uint32 timeOut)
{
	uint32	rmtShelfStatus = RMT_SHELF_NON_ACTIVE;
	uint32	status = NO_ERROR;
	uint32	loop;
	uint16	port[2];
	strMpuDualChkByNetInfo	dualSendMsgInfo, *dualRecvMsgInfoPtr;
	strSysConfigInfo *sysCnfgInfoPtr = &gSysCnfgInfo;

	strinetAddr         peerAddr, rcvPeerAddr;
	strDataBuf          sendData,recvData;
    struct   fd_set      tmpFds;
	struct	timeval	waitInfo;
    strControlNetConfigInfo *netInfoPtr;
	uint32	rmtChkSndOkCnt,rmtChkRcvOkCnt;
	uint32	retStatus = NO_ERROR;
   
    netInfoPtr = &sysCnfgInfoPtr->netCnfgInfo;

#if NOT_USED
	
	if(gDiuRasInfoPtr->redundancyStatus == SYS_REDUNDANCY_ERR)
	{
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, CMD_NOT_EXEC);  
		return(NO_ERROR);
	}
	
	
    if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN) 
    {
    	return(NO_ERROR);
    }
#endif
    	
	
    tmpFds = sDualChkReadFds;

    waitInfo.tv_sec		= 0;
	waitInfo.tv_usec	= 100; 

#if (BOARD_TYPE == LKV120_BOARD)
    status = inOutMultiplex(  sDualOpenSockFd[sDualChkOpenSockNum-1] + 1,
                              &tmpFds,
                              NULL,
                              NULL,
                              (void   *)&waitInfo
                           );
#else
    status = inOutMultiplex(  MAX_SOCKET_FD_NUM,
                              &tmpFds,
                              NULL,
                              NULL,
                              (void   *)&waitInfo
                           );
#endif

	if(status == NO_ERROR) 
	{
		for(loop = 0; loop < sDualChkOpenSockNum; loop++) 
		{
	    	if(FD_ISSET(sDualOpenSockFd[loop],   &tmpFds)) 
	    	{
			    
			    recvData.socket_fd = sDualOpenSockFd[loop];
	
			    
			    dataRecvFromServerOrClient( &recvData,
			                                (void *)&rcvPeerAddr,
			                                UDP_PROTO
			                              );
			}
		}
	}

	
	dualSendMsgInfo.msgType			= RMT_SHELF_CHK_REQUEST;
	dualSendMsgInfo.dualMsgCmd		= command;
	dualSendMsgInfo.sendShelfType	= sysCnfgInfoPtr->shelfType;
	 
	sendData.data_len   = sizeof(strMpuDualChkByNetInfo);
	
	memoryCopy( sendData.data_buf,
				(uint8 *)&dualSendMsgInfo,
				(uint32)sendData.data_len
				);

    peerAddr.type   = AF_INET;
   	port[0]   		= htons((uint16)UNI_DUAL_CHK_UDP_SEND_RECV_PORT_A);
    port[1]   		= htons((uint16)UNI_DUAL_CHK_UDP_SEND_RECV_PORT_B);

	rmtChkSndOkCnt = 0;
	
	for(loop = 0; loop < sDualChkOpenSockNum; loop++) 
	{
    	peerAddr.port   	= port[loop];
    	peerAddr.iAddr  	= sysCnfgInfoPtr->netCnfgInfo.rmtIpAddr[loop];

		sendData.socket_fd 	= sDualOpenSockFd[loop];
	    peerAddr.sockFd 	= sendData.socket_fd;
		
		status = dataSendToServerOrClient( &sendData, (void *)&peerAddr,UDP_PROTO);
		if(status != NO_ERROR)
		{
			setErrorCode(	__FILE__,__LINE__,__FUNCTION__, status);
		}
		else
		{
			rmtChkSndOkCnt++;
		}
	}
	
    tmpFds = sDualChkReadFds;

    waitInfo.tv_sec		= 0;
    
	waitInfo.tv_usec	= 1000 * timeOut;
	
#if (BOARD_TYPE == LKV120_BOARD)
    status = inOutMultiplex(  sDualOpenSockFd[sDualChkOpenSockNum-1] + 1,
                              &tmpFds,
                              NULL,
                              NULL,
                              (void   *)&waitInfo
                           );
#else
    status = inOutMultiplex(  MAX_SOCKET_FD_NUM,
                              &tmpFds,
                              NULL,
                              NULL,
                              (void   *)&waitInfo
                           );
#endif
	if((rmtChkSndOkCnt != 0) && (status == NO_ERROR)) 
	{
		rmtShelfStatus = 0;

		for(loop = 0; loop < sDualChkOpenSockNum; loop++) 
		{
	    	if(FD_ISSET(sDualOpenSockFd[loop],   &tmpFds)) 
	    	{
			    
			    memoryClear ( (uint8 *) &recvData, sizeof(strDataBuf));
			    
			    recvData.socket_fd = sDualOpenSockFd[loop];
	
			    
			    retStatus = dataRecvFromServerOrClient( &recvData,
			                                (void *)&rcvPeerAddr,
			                                UDP_PROTO
			                              );
				if(retStatus == NO_ERROR)
				{
					dualRecvMsgInfoPtr = (strMpuDualChkByNetInfo *)recvData.data_buf;
				
					
					if(	(rcvPeerAddr.iAddr == sysCnfgInfoPtr->netCnfgInfo.myIpAddr[0]) 
			    								||
						(rcvPeerAddr.iAddr == sysCnfgInfoPtr->netCnfgInfo.myIpAddr[1]))
					{
	   					rmtShelfStatus |= RMT_SHELF_NON_ACTIVE;			
	   				}
	   				else
					if(	(rcvPeerAddr.iAddr == sysCnfgInfoPtr->netCnfgInfo.rmtIpAddr[0]) 
			    								||
						(rcvPeerAddr.iAddr == sysCnfgInfoPtr->netCnfgInfo.rmtIpAddr[1]))
					{
	     				rmtShelfStatus |= RMT_SHELF_ACTIVE;	 
	     				*rmtSysRunStatus = dualRecvMsgInfoPtr->shelfRunStatus;
	     			}
	     			else 
	     			{
	   					rmtShelfStatus |= RMT_SHELF_NON_ACTIVE;	
	   				}
				}
				else
				{
					rmtShelfStatus |= RMT_SHELF_NON_ACTIVE;
				}
			}
			else 
			{
	    		rmtShelfStatus |= RMT_SHELF_NON_ACTIVE;			
	    	}
		}
    }
	else 
	{
		setErrorCode(	__FILE__,__LINE__,__FUNCTION__, status);
        rmtShelfStatus = RMT_SHELF_NON_ACTIVE;	
	}
	
	if(rmtShelfStatus&RMT_SHELF_ACTIVE)
		return (NO_ERROR);
	else
		return (RMT_SHELF_COMM_CHK_ERR);
}


static uint32	rmtShelfCheckCommRecvAndRsp (	strinetAddr *peerAddrInfoPtr,
												strDataBuf	*recvDataInfoPtr,
												uint16	portNumber
									  		)
{
	uint32	status = NO_ERROR;
	strMpuDualChkByNetInfo	*dualRecvMsgInfoPtr, dualSendMsgInfo;
	strSysConfigInfo *sysCnfgInfoPtr = &gSysCnfgInfo;
	strSysRunningConfigInfo *sysStatusInfoPtr = gSysRunningInfoPtr;
	strDataBuf          sendData;

#if NOT_USED
	
	
	
    if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_STANDBY) {
    	return(NO_ERROR);
    }
#endif

	
    if(	(peerAddrInfoPtr->iAddr == sysCnfgInfoPtr->netCnfgInfo.myIpAddr[0]) 
    							||
		(peerAddrInfoPtr->iAddr == sysCnfgInfoPtr->netCnfgInfo.myIpAddr[1])
	){
		setErrorCode(	__FILE__,__LINE__,__FUNCTION__, PEER_IP_ADDR_ERR);
		return(NO_ERROR);
	}
		 						
	
	dualRecvMsgInfoPtr = (strMpuDualChkByNetInfo *)recvDataInfoPtr->data_buf;
	if(dualRecvMsgInfoPtr->msgType != RMT_SHELF_CHK_REQUEST){
		setErrorCode(	__FILE__,__LINE__,__FUNCTION__, CMD_NOT_EXEC);
 		return(NO_ERROR);
	}

	
	dualSendMsgInfo.msgType			= RMT_SHELF_CHK_RESPONSE;
	dualSendMsgInfo.dualMsgCmd		= dualRecvMsgInfoPtr->dualMsgCmd;
	dualSendMsgInfo.sendShelfType	= sysCnfgInfoPtr->shelfType;
	dualSendMsgInfo.shelfRunStatus	= sysStatusInfoPtr->sysRedundancyStatus;
	 
	sendData.data_len   = sizeof(strMpuDualChkByNetInfo);
	
	memoryCopy( sendData.data_buf,
				(uint8 *)&dualSendMsgInfo,
				(uint32)sendData.data_len
			  );

	sendData.socket_fd 		= recvDataInfoPtr->socket_fd;
	peerAddrInfoPtr->port	= portNumber;	

	status = dataSendToServerOrClient( &sendData, (void *)peerAddrInfoPtr,UDP_PROTO);
	if(status != NO_ERROR)
		setErrorCode(	__FILE__,__LINE__,__FUNCTION__, status);

	return(status);
}


void usrDC_ATask(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)
int32   arg1,   arg2,   arg3,   arg4,   arg5,arg6, arg7, arg8, arg9, arg10;
{
    strDataBuf  recvData;
	strinetAddr	peerAddr;
	uint32	status;
	strSysRunningConfigInfo *sysStatusInfoPtr = gSysRunningInfoPtr;
#if (BOARD_TYPE == LKV120_BOARD)
    static struct   fd_set      readFds, tmpFds;
    int fd_max = 0;

	FD_ZERO(&readFds);
    FD_SET( sUniDualChkSndRcvSockFd_A, &readFds);
#endif 

    
    recvData.socket_fd = sUniDualChkSndRcvSockFd_A;

	for(;;){
#if (BOARD_TYPE == LKV120_BOARD)
        tmpFds =   readFds;
		fd_max = 0;
        if(FD_ISSET( sUniDualChkSndRcvSockFd_A, &tmpFds))
		{
			if(sUniDualChkSndRcvSockFd_A>fd_max) {
        		fd_max = sUniDualChkSndRcvSockFd_A;
			}
		}

#endif 

	    if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN) {

#if (BOARD_TYPE == LKV120_BOARD)
			status = inOutMultiplex( fd_max + 1,
									  &tmpFds,
									  NULL,
									  NULL,
									  NULL
								   );
			if(status   == NO_ERROR)
            {
			   if(FD_ISSET(sUniDualChkSndRcvSockFd_A,   &tmpFds))
                {
#endif 

		    status = dataRecvFromServerOrClient( &recvData,
		                                         (void *)&peerAddr,
		                                         UDP_PROTO
		                                       );
			if(status)
			{
				setErrorCode(	__FILE__,__LINE__,__FUNCTION__, status);
				sysTaskDelay(systemClockRateGet()/50);
			}
			else					                                       
				rmtShelfCheckCommRecvAndRsp (	&peerAddr, 
												&recvData, 
												UNI_DUAL_CHK_UDP_SEND_RECV_PORT_A
											);

#if (BOARD_TYPE == LKV120_BOARD)
                } 
			} 
			if(status)
            {
				setErrorCode(	__FILE__,__LINE__,__FUNCTION__, status);
                sysTaskDelay(systemClockRateGet()/50);
            }
#endif 

		}
		else
			sysTaskDelay(systemClockRateGet()/50);
	}
}


void usrDC_BTask(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)
int32   arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10;
{
    strDataBuf  recvData;
	strinetAddr	peerAddr;
	uint32	status;
	strSysRunningConfigInfo *sysStatusInfoPtr = gSysRunningInfoPtr;
#if (BOARD_TYPE == LKV120_BOARD)
    static struct   fd_set      readFds, tmpFds;
    int fd_max = 0;

	FD_ZERO(&readFds);
    FD_SET( sUniDualChkSndRcvSockFd_B, &readFds);
#endif 

    
    recvData.socket_fd = sUniDualChkSndRcvSockFd_B;

	for(;;){
#if (BOARD_TYPE == LKV120_BOARD)
        tmpFds =   readFds;
		fd_max = 0;
        if(FD_ISSET( sUniDualChkSndRcvSockFd_B, &tmpFds))
		{
			if(sUniDualChkSndRcvSockFd_B>fd_max) {
        		fd_max = sUniDualChkSndRcvSockFd_B;
			}
		}

#endif 

	    if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN) {

#if (BOARD_TYPE == LKV120_BOARD)
			status = inOutMultiplex( fd_max + 1,
									  &tmpFds,
									  NULL,
									  NULL,
									  NULL
								   );
			if(status   == NO_ERROR)
            {
			   if(FD_ISSET(sUniDualChkSndRcvSockFd_B,   &tmpFds))
                {
#endif 

		    status = dataRecvFromServerOrClient( &recvData,
		                                         (void *)&peerAddr,
		                                         UDP_PROTO
		                                       );
			if(status)
			{
				setErrorCode(	__FILE__,__LINE__,__FUNCTION__, status);
				sysTaskDelay(systemClockRateGet()/50);
			}
			else					                                       
				rmtShelfCheckCommRecvAndRsp (	&peerAddr, 
												&recvData, 
												UNI_DUAL_CHK_UDP_SEND_RECV_PORT_B
											);		

#if (BOARD_TYPE == LKV120_BOARD)
                } 
			} 
			if(status)
            {
				setErrorCode(	__FILE__,__LINE__,__FUNCTION__, status);
                sysTaskDelay(systemClockRateGet()/50);
            }
#endif 

		}
		else
			sysTaskDelay(systemClockRateGet()/50);		
	}
}

#ifdef _IPC_INTERFACE

void ipcDataSendTask(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)
int32   arg1,   arg2,   arg3,   arg4,   arg5,arg6, arg7, arg8, arg9, arg10;
{
    int32 status = NO_ERROR;
	int32 nWrite = NO_ERROR;
	int32 ipcInitFlag = TRUE;
	int32 targetProcessID = 0;
	uint32 maxAIReadBlock = 0;
	uint32 sendPeriod = 0;
	
	uint32 blockCnt = 0, dataCnt = 0;
	DEV_IO_DATA_t ipcArgData;
	IPC_MSG_DATA ipcData;
	strMemPointerInfo memInfoPtr;
	uint32 srcMemOffsetPtr;
	uint32 srcMemAddr = 0;
	uint16 scanDataWord = 0;
	uint32 maxDataNumber = 0;

	uint8 tempIoData[IPC_MAX_DATA_LEN];
	uint32 ipcDataSendCount = 0;

	
	if( messageIPCSendPortInit(TID_MAIN_EXE, &gIPCSendSockFd) == IPCDEFS_ERROR ) 
	{
		status = SOCK_OPEN_ERR;
		setErrorCode(	__FILE__,__LINE__,__FUNCTION__, status);
		ipcInitFlag = FALSE;
	}

	for(;;)
	{
		
		if(ipcInitFlag == FALSE)
		{
			if( messageIPCSendPortInit(TID_MAIN_EXE, &gIPCSendSockFd) == IPCDEFS_ERROR ) 
			{
				status = SOCK_OPEN_ERR;
				setErrorCode(	__FILE__,__LINE__,__FUNCTION__, status);
				mSleep(1000);	
		
				continue;
			}
			else
			{
				ipcInitFlag = TRUE;
			}
		}

		
		memoryClear((uint8 *)&memInfoPtr,
        			sizeof(strMemPointerInfo));

		status = fbMemPointerGet(LOGIC_ID, &memInfoPtr);
		if(status != NO_ERROR) {
			setErrorCode(	__FILE__,__LINE__,__FUNCTION__, status);
			return(status);
		}

		
		srcMemOffsetPtr = memInfoPtr.flagMemPtr;

		maxAIReadBlock	= 100;		
		maxDataNumber	= 100;		
		
		for(blockCnt = 0; blockCnt < maxAIReadBlock; blockCnt++)
		{
			memset(&ipcArgData, 0x0, sizeof(ipcArgData));

			ipcArgData.devId				  = 1;
			ipcArgData.opCode				  = CMD_READ_RESP;					
			ipcArgData.readBlock.dataNum	  = maxDataNumber * 2;
			ipcArgData.readBlock.memoryType   = MDOBUS_3_WORD_INPUT;
			ipcArgData.readBlock.memStartAddr = 30000 + (blockCnt * maxDataNumber);
			ipcArgData.seqId				  = ipcDataSendCount;

			
			if(ipcDataSendCount > 0x7fffffff) ipcDataSendCount = 1;
			else ipcDataSendCount++;
				
			
			srcMemAddr = 10000 + (blockCnt * maxDataNumber);
			
			
			memset(tempIoData, 0x0, sizeof(tempIoData));
			for(dataCnt = 0; dataCnt < maxDataNumber; dataCnt++)
			{
				scanDataWord = *(uint16*)((int16*)(srcMemOffsetPtr) + srcMemAddr + dataCnt);
				memcpy(&tempIoData[(dataCnt * 2)], &scanDataWord, sizeof(scanDataWord));
			}

			memcpy(	(char *)ipcArgData.readBlock.ioData, 
					(char *)tempIoData, 
					ipcArgData.readBlock.dataNum);

			
			
			memset(&ipcData, 0x0, sizeof(ipcData));
  			memcpy(	(char *)&ipcData.msgData.msgData[0], 
					(char *)&ipcArgData, 
					sizeof(ipcArgData)); 

			
			getIpcProcessID(PROCESS_MOD_IPC_PROC, &targetProcessID);

			
			nWrite = messageIPCSend(	TID_MAIN_EXE,		
										targetProcessID,	
										sizeof(ipcData), 
										(UN_IPC_DATA *)&ipcData,
										gIPCSendSockFd
										);

			if(nWrite != sizeof(ipcData))
			{
				printf("***ERROR- messageIPCSend, Dst PID(%d), ipc send fd(0x%x) \n", 
						targetProcessID, gIPCSendSockFd);
			}
			else
			{
				#if 000
				printf("***SUCCESS- messageIPCSend, DstPID(%d), send fd(0x%x), nWrite%d \n", 
						targetProcessID, gIPCSendSockFd, nWrite);
				#endif
			}	

			sysTaskDelay(systemClockRateGet()/2);	
		}

		
		sysTaskDelay(systemClockRateGet() * 10);

	} 
 
    return;
}




int32 checkUserProcess ()
{
	uint32	nUserProcessCount = 0;

#ifdef _WIN32
	DWORD		aProcesses[1024], cbNeeded, nCpuProcessCount;
	LT_UINT		i;
	HANDLE		hProcess;
	HMODULE		hMod;
	TCHAR		szProcessName[MAX_PATH] = TEXT("<unknown>");
	int8		strProcessName[MAX_PATH*2];

	if (!EnumProcesses (aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return (DEFS_ERROR);
	}

	
	nCpuProcessCount = cbNeeded / sizeof(DWORD);

	
	for (i = 0; i < nCpuProcessCount; i++)
	{
		if (aProcesses[i] != 0)
		{
			
			hProcess = OpenProcess (PROCESS_QUERY_INFORMATION |
									PROCESS_VM_READ,
									FALSE, aProcesses[i]);

			
			if (hProcess != NULL)
			{
				if (EnumProcessModules (hProcess, &hMod, sizeof(hMod), 
										&cbNeeded))
				{
					GetModuleBaseName (	hProcess, hMod, szProcessName, 
										sizeof(szProcessName)/sizeof(TCHAR));

					memset(strProcessName, 0x0, sizeof(strProcessName));
#if 111
					memcpy(	strProcessName, 
							szProcessName, 
							sizeof(szProcessName)/sizeof(TCHAR));
#else
					wcstombs(	strProcessName, 
								(LPTSTR)(LPCTSTR)szProcessName, 
								sizeof(szProcessName)/sizeof(TCHAR));				
#endif
					

					if (compareUserProcessName (strProcessName) > 0)
					{
						nUserProcessCount++;
					}
				}
			}

			CloseHandle (hProcess);
		}

	}

#else	

#define BURST   ((size_t)10)

	LT_INT	loopIdx, nCpuProcessCount;
	LT_INT	idx = 0;	

	#if defined (__hpux)

	struct pst_status	pst[BURST];

	while ((nCpuProcessCount = pstat_getproc (pst, sizeof(pst[0]), BURST, idx)) > 0)
	{
		for (loopIdx = 0; loopIdx < nCpuProcessCount; loopIdx++)
		{
			if (compareUserProcessName (pst[loopIdx].pst_ucomm) > 0)
			{
				nUserProcessCount++;
			}
		}
		idx = pst[nCpuProcessCount-1].pst_idx + 1;
	}

	#elif defined (__linux__ )

	#elif defined (__sun)

	#endif

#endif	

	return (nUserProcessCount);
}



int32 compareUserProcessName (int8 *processName)
{
	uint32	existCount = 0;
	uint32	processIdx;

	for (processIdx = REG_PRO_MAIN; processIdx < REG_PRO_END; processIdx++)
	{
		if (strlen(gProcessToRunList[processIdx].processName) != 0)
		{
			if (!strcmp (processName,
				gProcessToRunList[processIdx].processName))
			{
				existCount = 1;

				
				gProcessToRunList[processIdx].procRunFlag = existCount;

				break;
			}
		}
	}

	return (existCount);
}


int32 parseRegProcessConfigData (uint32 * numOfProc)
{
	FILE	*fp;
	uint32	charIndex;
	uint32	tIdx;
	int8	msg[512];
	int8	procName[MAX_NAMELENGTH];
	int8	procArgument[MAX_NAMELENGTH];
	int8	procDescription[MAX_NAMELENGTH];
	int8	procProtocol[MAX_NAMELENGTH];
	uint32	processInfoLen = strlen("PROCESS_");

	LT_BOOL	isProcNameSet[REG_PRO_END+1]		= {DEFS_FALSE, };
	LT_BOOL	isProcProtocolSet[REG_PRO_END+1]	= {DEFS_FALSE, };
	LT_BOOL	bFindProtocolSeparator;

	uint32 processIdx = 0;
	uint32 loop = 0;
	int8 *REGPROC_CFG_FILE	= "registerProcess.cfg";
	fp = fopen (REGPROC_CFG_FILE, "r");
	if (fp == NULL)
	{
		perror ("parseRegProcessConfigData:fopen");
		printf ("\n\n !!! =============================================================== !!!\n");
		printf (" !!!                                                                 !!!\n");
		printf (" !!! [%s] Failed to open config file. Contact Engineer.   !!!\n", REGPROC_CFG_FILE);
		printf (" !!!                                                                 !!!\n");
		printf (" !!! =============================================================== !!!\n\n");
		return (DEFS_ERROR);
	}

	(msg, 0, sizeof(msg));
	while (fgets (msg, sizeof(msg), fp))
	{
		charIndex = 0;
		while (isspace(msg[charIndex]))
		{
			if (charIndex >= (sizeof(msg)-1))
				break;
			charIndex++;
		}

		if (msg[charIndex] == '#')
		{
			memset (msg, 0, sizeof(msg));
			continue;
		}

		bFindProtocolSeparator = DEFS_FALSE;

		if (strncmp (&msg[charIndex], "PROCESS_NAME", processInfoLen + 4) == 0)
		{
			
			if (msg[charIndex+processInfoLen + 4] == '=')
			{
				memset (procName, 0, sizeof(procName));

				tIdx = 0;
				while(strlen (&msg[charIndex+processInfoLen+4+1])-1)
				{
					if (':' == msg[charIndex+processInfoLen+4+1+tIdx])
					{
						bFindProtocolSeparator = DEFS_TRUE;
						break;
					}
					else
					{
						procName[tIdx] = msg[charIndex+processInfoLen+4+1+tIdx];
						tIdx++;
					}
				}

				if (bFindProtocolSeparator == DEFS_TRUE)
				{
					
					for(loop = 0; loop < REG_PRO_END; loop++)
					{
						if (strlen(gProcessToRunList[loop].processName) != 0)
						{
							if (!strcmp(procName, gProcessToRunList[loop].processName))
							{
								printf ("\nProcess is already registered !!! (proto=%d, name=%s)\n",
										processIdx, gProcessToRunList[processIdx].processName); 
							}
						}
						else
						{
							strncpy (gProcessToRunList[loop].processName,
									 procName,
									 strlen(procName));

							printf ("\nRegister Process to run ..... '%s'\n", procName);
							
							break;
						}	
					}
				}
				else
				{
					printf ("\nProcess Name & Protocol separator(':') NOT FOUND !!!\n"); 
				}
			}
			else
			{
				printf ("'Process Name' must be followed by '=' ... [%s]\n",
						&msg[charIndex+processInfoLen + 4]);
			}
		}
		else if (strncmp (&msg[charIndex], "PROCESS_ARGUMENT", processInfoLen + 8) == 0)
		{
			
			if (msg[charIndex+processInfoLen + 8] == '=')
			{
				memset (procArgument, 0, sizeof(procArgument));
				for (tIdx = 0; tIdx < strlen (&msg[charIndex+processInfoLen+8+1])-1; tIdx++)
					procArgument[tIdx] = msg[charIndex+processInfoLen+8+1+tIdx];

				if (processIdx < REG_PRO_END)
				{
					strncpy (gProcessToRunList[processIdx].processArgument,
							procArgument,
							strlen(procArgument));
				}
			}
			else
			{
				printf ("'Process Argument' must be followed by '=' ... [%s]\n",
					&msg[charIndex+processInfoLen + 8]);
			}
		}
		else if (strncmp (&msg[charIndex], "PROCESS_DESCRIPTION", processInfoLen + 11) == 0)
		{
			
			if (msg[charIndex+processInfoLen + 11] == '=')
			{
				memset (procDescription, 0, sizeof(procDescription));
				for (tIdx = 0; tIdx < strlen (&msg[charIndex+processInfoLen+11+1])-1; tIdx++)
					procDescription[tIdx] = msg[charIndex+processInfoLen+11+1+tIdx];

				if (processIdx < REG_PRO_END)
				{
					strncpy (gProcessToRunList[processIdx].processDescription,
						procDescription,
						strlen(procDescription));
				}
			}
			else
			{
				printf ("'Process Description' must be followed by '=' ... [%s]\n",
					&msg[charIndex+processInfoLen + 11]);
			}

			processIdx++;
		}
	}

	* numOfProc = processIdx;

	fclose (fp);

	return (DEFS_OK);
}


void procStatusCheckTask(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10)
int32   arg1,   arg2,   arg3,   arg4,   arg5,arg6, arg7, arg8, arg9, arg10;
{
    int32 status = NO_ERROR;
	uint32 nTotalRegisterProc = 0;
	uint32 nUserProcessCount = 0;
	uint16 wordWriteVal = 0;
	uint32 exceptionCode = 0;
	uint32 mapAddress = 0;
	uint32 processIdx = 0;

	gModuleProcStatInfo.totalRegisterProc = 0;
	gModuleProcStatInfo.userProcessCount = 0;
	gModuleProcStatInfo.procStatus = 0;

	
	parseRegProcessConfigData(&nTotalRegisterProc);

	for(;;)
	{
		gModuleProcStatInfo.totalRegisterProc = nTotalRegisterProc;

		
		for (processIdx = REG_PRO_MAIN; processIdx < REG_PRO_END; processIdx++)
		{
			gProcessToRunList[processIdx].procRunFlag = 0;
		}

		
		nUserProcessCount = checkUserProcess();
		gModuleProcStatInfo.userProcessCount = nUserProcessCount;

		
		if(nTotalRegisterProc == nUserProcessCount)
		{
			wordWriteVal = 0;
		}
		else
		{
			
			if (nUserProcessCount > 0)
			{
				wordWriteVal = 1;
			}
			
			else
			{
				wordWriteVal = 1;
			}
		}

#if 000
		
		mapAddress = 40004;
		exceptionCode = mMemWordWritePoint(0,(uint32)(mapAddress), &wordWriteVal);
		if(exceptionCode != NO_ERROR )
		{
			setErrorCode(	__FILE__,__LINE__,__FUNCTION__, exceptionCode);
		}
#else

		gModuleProcStatInfo.procStatus = wordWriteVal;
		
		
		for (processIdx = REG_PRO_MAIN; processIdx < REG_PRO_END; processIdx++)
		{
			
			if(processIdx == 0)
			{
				gModuleProcStatInfo.proc1Run = gProcessToRunList[processIdx].procRunFlag;
			}
			else if(processIdx == 1)
			{
				gModuleProcStatInfo.proc2Run = gProcessToRunList[processIdx].procRunFlag;
			}
			else if(processIdx == 2)
			{
				gModuleProcStatInfo.proc3Run = gProcessToRunList[processIdx].procRunFlag;
			}
			else if(processIdx == 3)
			{
				gModuleProcStatInfo.proc4Run = gProcessToRunList[processIdx].procRunFlag;
			}
			else if(processIdx == 4)
			{
				gModuleProcStatInfo.proc5Run = gProcessToRunList[processIdx].procRunFlag;
			}
			else
			{
				continue;
			}
		}

#endif

		
		sysTaskDelay(systemClockRateGet());

	} 
 
    return;
}

#endif 
