
#include	<stdio.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"lkv120Board.h"
#include	"vmeDef.h"
#include	"pcsDefs.h"
#include	"systemDiag.h"
#include	"sysConfig.h"
#include	"usrErrCode.h"
#include	"sysDual.h"
#include	"segScheduler.h"
#include	"osDepend.h"

#include	"fbMem.h"
#include	"network.h"
#include	"netProtocol.h"
#include	"msgQueData.h"
#include	"usrDefBuf.h"

#include	"usrQueData.h"
#include	"sfcMem.h"

#include	"protocolBuf.h"

#include	"cmdList.h"

#include	"fbForcing.h"
#include	"sysConfig.h"

static uint32	sRmtShelfPrevLiveCount = 0;
static uint32	sRmtErrAccCount = 0;


static  uint32 sRmtLiveCountErrCount = 0;

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	static  uint32 sRmtDataRcvCountErrCount = 0;
	static 	uint32	sRmtStatusErrCount 		= 0;
#endif

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
#endif 

uint32	gStandByRmtShelfStatusCheckFlag = STANDBY_SHELF_STATUS_CHECK_DISABLE;


uint32	gRunRmtShelfStatusCheckFlag = RUN_SHELF_STATUS_CHECK_DISABLE;

static uint32 sDualMpuDataSyncFlag	= DATA_SYNC_DISABLE;
static uint32 sDualDiuDataSyncFlag	= DATA_SYNC_DISABLE;			

uint32	gDualMpuDataCopyFlag = DATA_SYNC_DISABLE;
uint32	gDualDiuDataCopyFlag = DATA_SYNC_DISABLE;

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	uint32  gUniRMTSendSockFd 		= 0 ;
	uint32  gUniRMTRecvSockFd 		= 0 ;
	uint32  gLiveCountRecvSockFd 	= 0 ;
	uint32  gReadRespRecvSockFd 	= 0 ;	

	uint32	gRunByRmtShelfLiveCntSendFlag = RUN_SHELF_LIVECNT_SEND_DISABLE;
	uint16  sDualSendSeqNum 		= 0;
	uint32 gDualRecvDataLiveCount 	= 0;
	uint32 sDualRecvPrevLiveCount 	= 0;

	strDualProtocolInfo *netMsgPtr;

	extern uint32  gReadRespRecvSockFd;
	extern  strSysDualInfo gRmtShelfDualInfo;
	extern void* gSysDualSendMsgQueId; 

#endif 


extern int32 gDebugPortFd;
extern uint32 *gDualDataBusPtr; 
extern strMpuSegSchedulerInfo	*gMpuSegSchdInfoPtr;
extern strDiuSegSchedulerInfo	*gDiuSegSchdInfoPtr;

extern strSysRunningConfigInfo		*gSysRunningInfoPtr;
extern strMpuDiuRunningStatusInfo 	*gDiuRasInfoPtr;
extern strNetRunningStatusInfo	   *gNetRasInfoPtr;

extern uint32  gUniEWSRecvSockFd_A;
extern void *gSendQueId; 
extern uint32	gIntrAccFlag;

extern strSysConfigInfo	gSysCnfgInfo;

extern strSysExecInfo *gSysExeInfoLocalPtr, *gSysExeInfoBusPtr;
extern int32 gInputScanFirstDelayTime;
extern uint32 gOutputScanFirstDelayDone;
extern strSystemRunningStatusInfo	gSysRunningStatusInfo;

uint32 sysDualLinkBoardInit(void);
uint32 sysDualLinkRmtBoardChk(void);
uint32 remoteShelfReset (uint8 *, uint32);

uint32	fbDataCopyFromRedundancy(uint32 ,uint32, uint32);
uint32	dualCmdAccFunc(strSysDualInfo *);
uint32	dualCmdMsgSendToRmtShelf(strSysDualInfo *);

uint32	dualMsgWriteToMyShelf(uint32, uint32 );
uint32	dualMsgWriteToRmtShelf(uint32 , uint32);

uint32	dualMsgReadFromMyShelf(uint32, uint32 *);
uint32	dualMsgReadFromRmtShelf(uint32, uint32 *);

uint32	dualMsgDumpFromMyShelf(strSysDualInfo *);
uint32	dualMsgDumpFromRmtShelf (strSysDualInfo *);

uint32	rmtResetAndLocalRun (uint8 *, uint32, uint32);
void	dualMsgStatusInfoDisplay(void);
uint32	localShelfStatusMonitoring(void);

void dualDataSyncFlagDisable(uint32);
static void dualDataSyncFlagSet(uint32);

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)

uint32	dualCmdGenAndMsgSend(uint32);
uint32	liveCountMonitoring(void);
uint32	redundancyDataSendandRecv( uint8 *, uint8 *,uint32, uint32 );
uint32	redundancyDataNumGen(uint32, uint32 *, uint32 * );
uint32  dualConfigDataGen(strDataBuf *, strinetAddr *,uint32);
uint32	myRmtDataAcc(uint8 *, uint32, uint32, uint32, int8 *, int8 *);
uint32  dualLiveCountGen(strDualProtocolInfo	*msgPtr);
uint32	dualCommandGen(strDualProtocolInfo	*msgPtr, uint32 );
int64	GetLongClockRate(void);
uint64	GetTickLongCount(void);
uint32  rmtShelfPowerOffCmd (uint8 *, uint32, uint32);
#endif


extern void		fourBytesDataCopy ( uint32 *, uint32 *, uint32 );
extern void		systemRasInfoSet(uint32 , uint32 );
extern uint32	myBusDataAcc(uint8 *, uint32, uint32	, uint32 ,uint8 *, uint8 *);
extern uint32	systemCnfgInfoRead( strSysConfigInfo *);

extern uint32	busMemoryDataCopyToLocalMem(uint32);
extern uint32	rmtEthComSeqNumChkDBSet(uint32);
extern void 	debugMsgDisplayInISR(	uint8 *, uint8 *, uint8 *, uint32, uint32, uint32,uint32,uint32);
extern void		setErrorCode(uint8 *,uint32,uint8 *,uint32);
extern void		memoryClear ( uint8 *, uint32 );
extern void		sysTaskDelay (int32 );
extern int32	keyHit(void);
extern void		logMessageSet (int8 *, uint32, int8 *, uint32,uint32 );
extern uint32	segSchedulerInfoInit(uint32);
extern uint32	schedulerModeChange(uint32 , uint32	);
extern void		sysRmtIpAddrSetToSysConfigMemory(uint32, uint32 );
extern void		busMemFbDataCopyToLocalFbMem(void);
extern uint32	dualFlagCheckInRunShelf(void);
extern uint32	fbExecIdInfoClear(uint32 , uint32 );
extern uint32	fbExecIdInfoSet(uint32 , uint32 );
extern uint32	sfcExecIdInfoClear(uint32 , uint32 );
extern uint32	sfcExecIdInfoSet(uint32 , uint32 );
extern uint32	fbSpecUnusedMemoryInfoClear( uint32 );
extern uint32	fbSpecUnusedMemoryInfoSet( uint32 );
extern uint32	getFbTaskTblInfo(uint32 , uint32 , strFbTaskTblMemInfo *);
extern uint32	segmentLocalDataChecksumCheck(uint32);
extern uint32	allSegmentModeErrSet(uint32 );
extern uint32	sfcMemPointerGet(uint32, strSfcMemPointerInfo *);
extern uint32	moduleInfoRead(uint32 ,strModuleInfo	*);
extern void		fbExeSequenceQueuePtrClear(uint32);
extern uint32	simFlagMpuSegmentWrite(uint32, uint32 ,uint32, uint32 * );                 
extern uint32	fbOutputUnusedMemoryInfoClear( uint32 );
extern uint32	fbOutputUnusedMemoryInfoSet(uint32);
extern uint32   sfcCurExecInfoSet(uint32);
extern uint32	fbSysExeModuleInfoRead( uint32, strModuleExecInfo* );
extern uint32	rmtComInfoQueClear(void);
extern uint32 	forcingFlagWrite(strFBforcingInfo *);
extern void 	mpuDiufbSimFbNumSetInDualInfo (void);
extern uint32	fbMemInfoRead(uint32, uint32, strFbMemInfo *);
extern void		setErrorCodeWithVal(uint8 *,uint32,uint8 *,uint32, uint8 *,uint32,
							uint8 *,uint32,uint8 *,uint32);
extern uint32	fbCodeParaTypePointerGet(uint32	,strFbCodeTypePointerInfo	*);
extern uint32	fbMemPointerGet(uint32 , strMemPointerInfo *);
extern void		memoryCopy(uint8 *, uint8 *, uint32);
extern uint32 	dualFlagCheckInStbyShelf(void);
extern uint32	rmtShelfCheckByCtrlNetCom (uint32, uint32 *, uint32);
extern int32	systemClockRateGet(void);
extern void		fmcEngDataCopyCheckAndSet(void); 
extern void   errorCodeSetInISR(uint8 *, uint32, uint8 *, uint32);

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)


extern int32	dataRecvFromServerOrClient( strDataBuf  *, void *,uint32   );
extern int32	inOutMultiplex( int32 , void   *, void *,void *,   void *);
extern uint32	rmtFbDataCopyRespFunc(strDualProtocolInfo*, strBufInfo*);
extern uint32	comPortInfoGet(uint32, strinetAddr *, strinetAddr *);
extern int32	messageQueueSend(void *, int8 *, int32,int32, int32);
extern uint32	fbSysExeInfoPointerGet(strSysExecInfo**, uint32 );
extern  uint32	busFbMemPointerGet(uint32,strMemPointerInfo	*);
extern void		moduleFbMemoryClear(uint32,uint32 );
extern void		sysExeInfoMemoryClear(uint32);
extern uint32   netMsgChecksumCalcu(uint32  ,uint32 *,uint32    *);
extern void systemRasInfoGet(uint32 , uint32 *);
#endif

uint32 sysDualLinkBoardInit(void)
{
	uint32	status = NO_ERROR;
	
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)

	int8  rmtIpAddr[20];
	
	strSysConfigInfo  sysCnfgInfo;
	uint32 hexData;
	strinetAddr     localAddr;
    uint32  optionVal;
    
    strSystemBootParaInfo sysBootParaInfo, *bootPara;

    memoryClear ( (uint8 *)&sysBootParaInfo, sizeof(strSystemBootParaInfo));
    bootPara = &sysBootParaInfo;

    status  = nvRamRead((int8 *)&sysBootParaInfo,
						USR_ENET_CONFIG_PARA_OFFSET, 
						USR_ENET_CONFIG_PARA_SIZE
					   );	
					   				   
	if(status != NO_ERROR)
		printf("[ERROR] - Boot Parameter Read Error[0x%x]\n", status);
	
	
	systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);

	memoryClear(rmtIpAddr,strlen((int8*)rmtIpAddr));
	if( (status = ipAddrMake((uint8 *)rmtIpAddr,
							sysCnfgInfo.stationId,
							DLU_RMT_LINE,
							sysCnfgInfo.shelfType
							)
		))
	{
		bootPara->usrBootParaInfo.rmtDualLinkIpHex = 0;
		systemInitErr	(	(uint8 *)__FILE__, 
							__LINE__,
        	                (uint8 *)"DLU Board IP Addr Make", 
        	                status
						);
	}
	else 
	{
		ipAddrStrToHex(&bootPara->usrBootParaInfo.rmtDualLinkIpHex,rmtIpAddr); 
	}
	
	if(status == NO_ERROR)
	{	
		
		
		if(sysPing(rmtIpAddr) == NO_ERROR) 
		{
			fdprintf(gDebugPortFd,"Remote Shelf is Alive, Current Shelf Boot Delay [3 Seconds]\n");
			sysTaskDelay(systemClockRateGet() * 3); 

			sysCnfgInfo.redNetCnfgInfo.rmtNetDualStatus = BOARD_ENABLE;
		}
		else
		{
			fdprintf(gDebugPortFd,"[ERROR]- Remote Shelf Ping Error By DLU Board!!!!\n");
			sysCnfgInfo.redNetCnfgInfo.rmtNetDualStatus = BOARD_DISABLE;
		}
		
		ipAddrStrToHex(&hexData, bootPara->usrBootParaInfo.dualLinkIpAddr); 

		
		memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));
		status = addrInfoBuild( (void   *)&localAddr,
								 NULL,
								 UNI_READ_UDP_RECV_PORT
							  );

		
		if(status   == NO_ERROR)
		{
			
			status = (int32)connectionEstablishInServer((void *)   &localAddr,
																   UDP_PROTO,
																   &gReadRespRecvSockFd
													   );
			if(status != NO_ERROR)
				fdprintf(gDebugPortFd,"[gReadRespRecvSockFd]- connectionEstablishInServer Error !!!!\n");
		}

		
		memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));
		status = addrInfoBuild( (void   *)&localAddr,
								 NULL,
								 UNI_RMT_UDP_RECV_PORT
							  );

		
		if(status   == NO_ERROR)
		{
			
			status = (int32)connectionEstablishInServer((void *)   &localAddr,
																   UDP_PROTO,
																   &gUniRMTRecvSockFd
													   );
			if(status != NO_ERROR)
				fdprintf(gDebugPortFd,"[gUniRMTRecvSockFd]- connectionEstablishInServer Error !!!!\n");
		}

		
		memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));
		status = addrInfoBuild( (void   *)&localAddr,
								 NULL,
								UNI_LIV_UDP_RECV_PORT
							  );
		if(status   == NO_ERROR)
			status = (int32)connectionEstablishInClient (
															 (void   *) &localAddr,
#ifdef VXWORKS
																	UDP_PROTO,
#else
																	UDP_UNICASTING,
#endif
															  &gLiveCountRecvSockFd
														 );


		
		memoryClear (   (uint8 *)   &localAddr, sizeof(strinetAddr));
		status = addrInfoBuild( (void   *)&localAddr,
								(uint32 *)&hexData,
								UNI_RMT_UDP_SEND_PORT
							  );
		if(status   == NO_ERROR)
			status = (int32)connectionEstablishInClient (
															 (void   *) &localAddr,
#ifdef VXWORKS
																	UDP_PROTO,
#else
																	UDP_UNICASTING,
#endif
															  &gUniRMTSendSockFd
														 );

		
		if(status   == NO_ERROR)
		{
			
			optionVal   =   MAX_SOCK_BUF_SIZE;

			status = socketOptionSet(gUniRMTSendSockFd,SENDBUF_SIZE_OPT,
									 (void   *)&optionVal,   sizeof(optionVal));

			
			if(status   == NO_ERROR)
				status = socketOptionSet(gLiveCountRecvSockFd,RECVBUF_SIZE_OPT,
									 (void   *)&optionVal,   sizeof(optionVal));
		
			
			if(status   == NO_ERROR)
				status = socketOptionSet(gReadRespRecvSockFd,RECVBUF_SIZE_OPT,
									 (void   *)&optionVal,   sizeof(optionVal));
				
			
			if(status   == NO_ERROR)
				status = socketOptionSet(gUniRMTRecvSockFd,RECVBUF_SIZE_OPT,
										 (void   *)&optionVal,   sizeof(optionVal));
		}	

	}

	if(status != NO_ERROR) 
	{
		sysCnfgInfo.redNetCnfgInfo.ctrlNetDualStatus = BOARD_DISABLE;
		fdprintf(gDebugPortFd, "[ERROR] - DLU Board Init Error !!\n");
	}
	else
	{
		
		sysCnfgInfo.redNetCnfgInfo.ctrlNetDualStatus = BOARD_ENABLE;
		ipAddrStrToHex(&hexData, bootPara->usrBootParaInfo.dualLinkIpAddr);
		sysCnfgInfo.redNetCnfgInfo.myDluIpAddrHex = hexData;

		sysCnfgInfo.redNetCnfgInfo.rmtDluIpAddrHex = bootPara->usrBootParaInfo.rmtDualLinkIpHex;

		
		memoryCopy ((uint8 *) &gSysCnfgInfo.redNetCnfgInfo,
    			 	(uint8 *) &sysCnfgInfo.redNetCnfgInfo,
				 	sizeof(strRedundancyNetConfigInfo)
				   );
	}

#endif 

	return(status);
}


uint32 sysDualLinkRmtBoardChk(void)
{
	uint32 rmtShelfStatus;
	
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	uint32	lStatus, rStatus, retStatus = NO_ERROR;

	strinetAddr localAddr,peerAddr;
    struct timeval timeOut;
	strDataBuf sendData,recvData;

    static struct   fd_set      readFds, tmpFds;
	strSysConfigInfo sysCnfgInfo;
	int connectTime = 0, maxTime = 0;
	uint32 *opCmd,calcuChecksum = 0;
	strDualProtocolInfo	*netPtr;

#if (BOARD_TYPE == LKV120_BOARD)
    int fd_max = 0;
#endif 

	systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);

	
	if(sysCnfgInfo.shelfType == SLAVE_SHELF)
		maxTime = MAX_DUAL_EXE_ITER;
	else maxTime = MAX_DUAL_DATA_ITER;
	
	if(sysCnfgInfo.redNetCnfgInfo.ctrlNetDualStatus == BOARD_ENABLE)
	{

		
		FD_ZERO( &readFds );
		FD_SET( gReadRespRecvSockFd, &readFds);
	}
	else retStatus = 1; 

    
	if( sysCnfgInfo.redNetCnfgInfo.rmtNetDualStatus == BOARD_ENABLE) 
	{
		lStatus = 0;
	}
	else 
		lStatus = 1; 

	timeOut.tv_sec = 1; 
	timeOut.tv_usec = 0;

	if(lStatus == NO_ERROR)
	{
		fdprintf(gDebugPortFd, "Remote DLU Board is INSTALLED!!!\n");
		
		
		retStatus = dualConfigDataGen(&sendData,&localAddr,DUAL_CONF_READ);
	}
	else retStatus = 1;

	
	connectTime = 0;
	while((lStatus == NO_ERROR)&&(retStatus == NO_ERROR))
	{
		retStatus = dataSendToServerOrClient( &sendData, (void *) &localAddr, UDP_PROTO);
		
		
		tmpFds =   readFds;
#if (BOARD_TYPE == LKV120_BOARD)
		fd_max = 0;
		{
            if(FD_ISSET(gReadRespRecvSockFd, &tmpFds))
			{
				if(gReadRespRecvSockFd>fd_max) 
				{
	        		fd_max = gReadRespRecvSockFd;
				}
			}
		}
#endif 
#if (BOARD_TYPE == LKV120_BOARD)
		retStatus = inOutMultiplex(  fd_max + 1,
									  &tmpFds,
									  NULL,
									  NULL,
									  (void *)&timeOut
		
								   );
#else
		retStatus = inOutMultiplex(  MAX_SOCKET_FD_NUM, 
									  &tmpFds,
									  NULL,
									  NULL,
									  (void *)&timeOut
		
								   );
#endif

		if(retStatus   == NO_ERROR) 
		{

			if(FD_ISSET(gReadRespRecvSockFd,   &tmpFds))
			{
				recvData.socket_fd = gReadRespRecvSockFd;
				
				retStatus = dataRecvFromServerOrClient( &recvData,
													 	(void *)   &peerAddr,
													 	UDP_PROTO
												   	);
				if(retStatus == NO_ERROR)  
				{
					 
					opCmd = (uint32*)&recvData.data_buf[MAX_DUAL_FRAME_HEAD_LEN];
					if( *opCmd != ((DUAL_ACCESS_CMD<<16) | (DUAL_CONF_READ&0xffff)) ) 
					{
						goto INCREASE_TIME;
					}
									
					
					netPtr = (strDualProtocolInfo	*)recvData.data_buf;
					(void)netMsgChecksumCalcu(	netPtr->dataLength,
												(uint32*)  netPtr->dataBuff,
												(uint32*) &calcuChecksum
											 );
					if(netPtr->checkSum != (uint16)calcuChecksum) 
					{
						goto INCREASE_TIME;
					}

					
					retStatus = rmtDualConfigReadRespFunc((strDualProtocolInfo *)recvData.data_buf, NULL);
					if(retStatus != NO_ERROR) 
					{
						retStatus = 1; 
						fdprintf(gDebugPortFd, "[rmtDualConfigReadRespFunc] - RESPONSE ERROR!!!\n");
					}
					else 
					{
						break; 
					}
					
				}
			}
		}
		
INCREASE_TIME:
		
		connectTime++;
		if(connectTime >maxTime) 
		{ 
			retStatus=1;
			fdprintf(gDebugPortFd, "CONNECTION IS FAIL!!!\n");
			break;
		}
	}
		
	
	
	if (retStatus != NO_ERROR) 
	{
		rStatus = 0;
		fdprintf(gDebugPortFd, "[RmtBoardStatusRead] - RESPONSE ERROR!!!\n");
	}
	else
	{
		rStatus = 1;
		
		dualConfigDataGen(&sendData,&localAddr,DUAL_CONF_WRITE);
		dataSendToServerOrClient( &sendData, (void *) &localAddr, UDP_PROTO);
		
		fdprintf(gDebugPortFd,"REMOTE SHELF Connection SUCCESS(IterTimes=%d)!!!\n",connectTime);
	}

	FD_CLR( gReadRespRecvSockFd, &readFds);

	if((lStatus == 0) && (rStatus == 1))
    	rmtShelfStatus = 0; 
	else
    	rmtShelfStatus = 1;	
    	
#endif

   	return(rmtShelfStatus);

}

uint32 remoteShelfReset (uint8 *fileName, uint32 lineNum) 
{
	uint32 status = NO_ERROR;

#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)

	strSysDualInfo dualCmdInfo;

	dualCmdInfo.dualCmd = RMT_RESET_CMD;
    
	status = dualCmdMsgSendToRmtShelf(&dualCmdInfo);
    if(status)
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
	
	memoryClear((uint8*) &gRmtShelfDualInfo, sizeof(strSysDualInfo));
    
	
	logMessageSet(fileName, lineNum,"remoteShelfReset", RMT_SHELF_RESET, 0);

#endif

    return(status);
}

uint32 fbDataCopyFromRedundancy(uint32 shelfType, uint32 copyFlag, uint32 printFlag)
{
	uint32	*mpuSrcPtr = NULL, *mpuDestPtr = NULL;
	uint32	*diuSrcPtr = NULL, *diuDestPtr = NULL;
	uint32	status = NO_ERROR;
#if (SYSTEM_DATA_SYNC_BOARD != ETH3_BOARD)
	uint32  mpuDataNum = 0, diuDataNum = 0 , i;
#endif
	
    switch(shelfType) {
		case MASTER_SHELF:
        	mpuSrcPtr 	= (uint32 *)MPU_FB_MEMORY_START_BUS_ADDR;
            mpuDestPtr	= (uint32 *)MEM_BOARD_BUS_ADDR_ON_SLAVE;

        	diuSrcPtr 	= (uint32 *)DIU_FB_MEMORY_START_BUS_ADDR;
            diuDestPtr	= (uint32 *)(MEM_BOARD_BUS_ADDR_ON_SLAVE +
									 MAX_MPU_MEM_SIZE
									);
        break;

        case SLAVE_SHELF:
        	mpuSrcPtr 	= (uint32 *)MPU_FB_MEMORY_START_BUS_ADDR;
            mpuDestPtr	= (uint32 *)MEM_BOARD_BUS_ADDR_ON_MASTER;

        	diuSrcPtr 	= (uint32 *)DIU_FB_MEMORY_START_BUS_ADDR;
            diuDestPtr	= (uint32 *)(MEM_BOARD_BUS_ADDR_ON_MASTER +
									 MAX_MPU_MEM_SIZE
									);
        break;

        default:
        	status = STATION_SHELF_CONFIG_ERR;
        break;
    }

	if(printFlag!=PRINT_FLAG) printf("\n");

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	if(status == NO_ERROR) 
	{
		if(copyFlag == MPU_DIU_DUAL_ACC) {

			moduleFbMemoryClear(LOGIC_ID,printFlag);
			
    		
			status = redundancyDataNumGen (	LOGIC_ID,
											(uint32 *)mpuDestPtr, 
											(uint32 *)mpuSrcPtr
										 );
        	if(status == NO_ERROR)
        		printf("-----[MPU DUAL COPY SUCCESS]\n");
			else {
        		printf("**************[ERROR]****************\n");
        		printf("*****       [MPU DUAL COPY FAIL]\n");
        		printf("*************************************\n");
        		return(status);
			}

		}
		
		else if(copyFlag == MPU_ONLY_DUAL_ACC) {
			moduleFbMemoryClear(LOGIC_ID,printFlag);
    		
			status = redundancyDataNumGen (	LOGIC_ID,
												(uint32 *)mpuDestPtr, 
												(uint32 *)mpuSrcPtr
											 );
        	if(status == NO_ERROR)
        		printf("-----[MPU DUAL COPY SUCCESS]\n");
			else {
        		printf("**************[ERROR]****************\n");
        		printf("*****       [MPU DUAL COPY FAIL]\n");
        		printf("*************************************\n");
        		return(status);
			}
		}
		else {
    		status = DUAL_STATUS_FLAG_ERR;
    		setErrorCode (__FILE__,__LINE__,"fbDataCopyFromRedundancy",status);	
    	}
		
	}
#else	
    if(status == NO_ERROR) {
		printf("\n");

    	mpuDataNum = MAX_MPU_MEM_SIZE/4;
     	diuDataNum = MAX_DIU_MEM_SIZE/4;

		if(copyFlag == MPU_DIU_DUAL_ACC) {		
    		
        	for(i = 0; i <  mpuDataNum; i++, mpuDestPtr++, mpuSrcPtr++) {
				status = myBusDataAcc(	__FILE__,
										__LINE__,	
										BUS_LONG_ACC, 
										BUS_DATA_READ ,
										(uint8 *)mpuDestPtr, 
										(uint8 *)mpuSrcPtr
									 );
				if(status) break;
			}
        	if(status == NO_ERROR){
				if(printFlag == PRINT_FLAG)
	        		printf("-----[MPU DUAL COPY SUCCESS]\n");
        	}
			else {
				if(printFlag == PRINT_FLAG) {
	        		printf("**************[ERROR]****************\n");
	        		printf("*****       [MPU DUAL COPY FAIL]\n");
	        		printf("*************************************\n");
				}
        		return(status);
			}

    		
       		for(i = 0; i <  diuDataNum; i++, diuDestPtr++, diuSrcPtr++) {
				status = myBusDataAcc(	__FILE__,
										__LINE__,	
										BUS_LONG_ACC, 
										BUS_DATA_READ ,
										(uint8 *)diuDestPtr, 
										(uint8 *)diuSrcPtr
									 );
				if(status) break;
			}
        	if(status == NO_ERROR){
				if(printFlag == PRINT_FLAG) 
	        		printf("-----[DIU DUAL COPY SUCCESS]\n");		
			}
			else{
				if(printFlag == PRINT_FLAG) {
	        		printf("**************[ERROR]****************\n");
	        		printf("*****       [DIU DUAL COPY FAIL]\n");
	        		printf("*************************************\n");
				}
        		return(status);
        	}
		}
		else if(copyFlag == MPU_ONLY_DUAL_ACC) {
    		
        	for(i = 0; i <  mpuDataNum; i++, mpuDestPtr++, mpuSrcPtr++) {
				status = myBusDataAcc(	__FILE__,
										__LINE__,	
										BUS_LONG_ACC, 
										BUS_DATA_READ ,
										(uint8 *)mpuDestPtr, 
										(uint8 *)mpuSrcPtr
									 );
				if(status) break;
			}
        	if(status == NO_ERROR){
				if(printFlag == PRINT_FLAG) 
        			printf("-----[MPU DUAL COPY SUCCESS]\n");
			}
			else {
				if(printFlag == PRINT_FLAG) {
	        		printf("**************[ERROR]****************\n");
	        		printf("*****       [MPU DUAL COPY FAIL]\n");
	        		printf("*************************************\n");
        		}
        		
        		return(status);
			}
		}
    	else {
    		status = DUAL_STATUS_FLAG_ERR;
    		setErrorCode (__FILE__,__LINE__,"fbDataCopyFromRedundancy",status);	
    	}
	}
#endif 

    return(status);
}


uint32 dualCmdAccFunc(strSysDualInfo	*dualInfoPtr)
{
	uint32	sysRedundancyErrStatus, i;
	uint32  sysRunStatus,sysRedundancyRunStatus;
	strSysConfigInfo sysCnfgInfo;
	strSysRunningConfigInfo	*sysStatusInfoPtr = gSysRunningInfoPtr;
	uint32	status = NO_ERROR, taskId;
	strSysDualInfo dualCmdInfo;
	strFbTaskTblMemInfo	segTbl;
	strModuleExecInfo	*sysMpuExeInfoPtr = &gSysExeInfoLocalPtr->mpuSysExeInfo;
	strFBforcingInfo forcingInfo;

	strFbMemInfo	fbMemInfo;
	strFbCodeTypePointerInfo	typePInfo;
	strMemPointerInfo	memP;
	strFbSpecActionInfo	*specActionInfoPtr;
	uint32	*specTypePtr, *specSizePtr, *specStartPtr, specWriteSize, *specDataPtr;
	uint32	rmtShelfErrSummary, dummy;	
	
    switch((dualInfoPtr->dualCmd)) {
		
		
		case REDUNDANCY_ERR_CLR_CMD:
	        
        	sysRedundancyErrStatus = NO_ERROR;
			systemRasInfoSet(	RAS_REDUNDANCY_ERR_STATUS, 
								sysRedundancyErrStatus
							);

			
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN)
				gStandByRmtShelfStatusCheckFlag = STANDBY_SHELF_STATUS_CHECK_ENABLE;
			else
				gRunRmtShelfStatusCheckFlag 	= RUN_SHELF_STATUS_CHECK_ENABLE;
				

			
			if(sysStatusInfoPtr->sysOperationMode == SYS_RAS_SYSTEM_MANUAL_MODE )
				gRunByRmtShelfLiveCntSendFlag = RUN_SHELF_LIVECNT_SEND_ENABLE;
#endif

   			debugMsgDisplayInISR(	"dualCmdAccFunc", 
									"REDUNDANCY_ERR_CLR_CMD",
       	        		            __FILE__, __LINE__,0,0,0,0
								);
			
			dualDataSyncFlagSet (LOGIC_ID);
        break;
		
		
		
		case REMOTE_RUN_CMD:
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			rmtResetAndLocalRun (__FILE__, __LINE__, 0);
#endif
   			debugMsgDisplayInISR(	"dualCmdAccFunc", 
									"REMOTE_RUN_CMD",
       	        		            __FILE__, __LINE__,0,0,0,0
								);
        break;

		
		
		case CHG_AUTO_TO_MANUAL_CMD:
            
			systemRasInfoSet(	RAS_SYSTEM_MODE, 
								SYS_RAS_SYSTEM_MANUAL_MODE
							);	

			if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_STANDBY) {
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
				
				gRunByRmtShelfLiveCntSendFlag = RUN_SHELF_LIVECNT_SEND_ENABLE;
#else
         		
        		gRunRmtShelfStatusCheckFlag = RUN_SHELF_STATUS_CHECK_DISABLE;
#endif        		
			}        		

   			debugMsgDisplayInISR(	"dualCmdAccFunc", 
									"CHG_AUTO_TO_MANUAL_CMD",
       	        		            __FILE__, __LINE__,0,0,0,0
								);
        break;

		
		
		case CHG_MANUAL_TO_AUTO_CMD:
			if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_STANDBY) {
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			  	
        		gRunRmtShelfStatusCheckFlag = RUN_SHELF_STATUS_CHECK_DISABLE;
				
				
				gRunByRmtShelfLiveCntSendFlag = RUN_SHELF_LIVECNT_SEND_DISABLE;
#endif
	            
            	systemCnfgInfoRead(&sysCnfgInfo);
				status = fbDataCopyFromRedundancy(	sysCnfgInfo.shelfType,
													MPU_DIU_DUAL_ACC,
													NO_PRINT
												); 
    			if(status != NO_ERROR){
					setErrorCode	(	__FILE__,
       									__LINE__,
           	    	    	    		(uint8 *)"dualCmdAccFunc",
                       	    			status
									);
        			
        			
        			
        			
        		
     				
     				systemRasInfoSet(RAS_MPU_DUAL_COPY_STATUS, SYS_DUAL_COPY_ERR);
    			
    				
					allSegmentModeErrSet(LOGIC_ID);
					
					goto DUAL_OPERATION_SEND_1;
				}
				else { 
     				systemRasInfoSet(RAS_MPU_DUAL_COPY_STATUS, NO_ERROR);			
				}
				
				sysTaskDelay(systemClockRateGet());
							 
				systemRasInfoSet(RAS_MPU_EXEC_LOOP_STATUS, 0);

				
				status = segmentLocalDataChecksumCheck(LOGIC_ID);
				if(status != NO_ERROR){
					systemRasInfoSet(RAS_MPU_CHECKSUM_STATUS, SEG_CHECKSUM_ERR);
					goto DUAL_OPERATION_SEND_1;
				}
				else
					systemRasInfoSet(RAS_MPU_CHECKSUM_STATUS, NO_ERROR);

                for(taskId = 1; taskId <= sysMpuExeInfoPtr->curMaxSegmentId; taskId++) {
					status = rmtEthComSeqNumChkDBSet(taskId);
                    if(status)
                    	setErrorCode (	__FILE__,
                        					__LINE__,
                                            "dualCmdAccFunc",
                                            status
										);
				}	
				
				status = rmtComInfoQueClear();
                if(status)
                	setErrorCode (	__FILE__,
                    					__LINE__,
                                        "dualCmdAccFunc",
                                        status
									);
			
				

				
				(void)fbExeSequenceQueuePtrClear(TOTAL_DWL);
				
				
				
				
                for(taskId = 1; taskId <= MAX_LOGIC_TASK_NUM; taskId++) {
					status = getFbTaskTblInfo	(	LOGIC_ID,	taskId,(strFbTaskTblMemInfo	*) &segTbl);
					if(status != NO_ERROR)
                    	setErrorCode (	__FILE__,__LINE__,
                                            "dualCmdAccFunc",status
										);
					if(segTbl.logicType == SFC_LOGIC_TYPE){
                		
						status = sfcExecIdInfoClear(taskId, LOGIC_ID);
						if(status)
                    		setErrorCode (	__FILE__,
                        						__LINE__,
												"dualCmdAccFunc",
												status
											);
						
						
						status = sfcExecIdInfoSet(taskId, LOGIC_ID);
						if(status)
                    		setErrorCode (	__FILE__,
                        						__LINE__,
												"dualCmdAccFunc",
												status
											);
					}				
					else {
                		
						status = fbExecIdInfoClear(taskId, LOGIC_ID);
						if(status)
                    		setErrorCode (	__FILE__,
                        						__LINE__,
												"dualCmdAccFunc",
												status
											);
						
						
						status = fbExecIdInfoSet(taskId, LOGIC_ID);
						if(status)
                    		setErrorCode (	__FILE__,
                        						__LINE__,
												"dualCmdAccFunc",
												status
											);
					}
				}				
				
														
	            

				status = fbSpecUnusedMemoryInfoClear(LOGIC_ID);
                if(status)
                    setErrorCode (	__FILE__,__LINE__,
                                        "dualCmdAccFunc",status
									);
				
				
				status = fbSpecUnusedMemoryInfoSet(LOGIC_ID);
                if(status)
                    setErrorCode (	__FILE__,__LINE__,
                                        "dualCmdAccFunc",status
									);
				
				status = fbOutputUnusedMemoryInfoClear(LOGIC_ID);
                if(status)
                    setErrorCode (	__FILE__,__LINE__,
                                        "dualCmdAccFunc",status
									);
				
				
				status = fbOutputUnusedMemoryInfoSet(LOGIC_ID);
                if(status)
                    setErrorCode (	__FILE__,__LINE__,
                                        "dualCmdAccFunc",status
									);
				
				mpuDiufbSimFbNumSetInDualInfo ();
				
				
				status = segSchedulerInfoInit(MPU_ONLY_DUAL_ACC);
				if(status!=NO_ERROR) {
		    	    setErrorCode (__FILE__,
	    	    	   				__LINE__,
	        	                    "dualCmdAccFunc",
	            	   	            status
									);
				}

				schedulerModeChange(LOGIC_ID, SCHEDULER_ENABLE);
		
				
				
				
				
DUAL_OPERATION_SEND_1:
	                dualCmdInfo.dualCmd = SYS_DUAL_OPERATION_START;

                
				status = dualCmdMsgSendToRmtShelf(&dualCmdInfo);
                if(status)
                   	setErrorCode (	__FILE__,
                       					__LINE__,
                                        "dualCmdAccFunc",
                                        status
   									);
				
       			
        		
        		
        		
        		
        		sysTaskDelay(systemClockRateGet());
    		
        		
        		gRunRmtShelfStatusCheckFlag = RUN_SHELF_STATUS_CHECK_ENABLE;

			}
 			systemRasInfoSet(	RAS_SYSTEM_MODE, 
								SYS_RAS_SYSTEM_AUTO_MODE
							);	

   			debugMsgDisplayInISR(	"dualCmdAccFunc", 
									"MANUAL_TO AUTO_CHG_CMD",
       	        		            __FILE__, __LINE__,0,0,0,0
								);
       	break;

        
        
		case CHG_RUN_TO_STANDBY_CMD:
        	
	        sysRunStatus 	= SYS_RAS_SYSTEM_STOP;
		    systemRasInfoSet(RAS_SYSTEM_STATUS, sysRunStatus);

            
			sysRedundancyRunStatus 	= SYS_RAS_REDUNDANCY_STANDBY; 
		    systemRasInfoSet(RAS_REDUNDANCY_STATUS, sysRedundancyRunStatus);

            
			status = dualMsgDumpFromRmtShelf (&dualCmdInfo);
			if(status != NO_ERROR) {
	    	    setErrorCode (__FILE__,
    	    	   				__LINE__,
        	                    "dualCmdAccFunc",
            	   	            status
								);
				return(status);
			} 
		
       		gStandByRmtShelfStatusCheckFlag = STANDBY_SHELF_STATUS_CHECK_DISABLE;

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	        
        	gRunRmtShelfStatusCheckFlag = RUN_SHELF_STATUS_CHECK_ENABLE;
#endif
  			
  			debugMsgDisplayInISR(	"dualCmdAccFunc", 
									"CHG_RUN_TO_STANDBY_CMD",
       	        		            __FILE__, __LINE__,0,0,0,0
								);
			
			dualDataSyncFlagDisable(LOGIC_ID);
        break;

        
        
        
        
        
		case SYS_DUAL_OPERATION_START:
        	gStandByRmtShelfStatusCheckFlag = STANDBY_SHELF_STATUS_CHECK_ENABLE;
        	
        	
			systemRasInfoSet(	RAS_SYSTEM_MODE, 
								SYS_RAS_SYSTEM_AUTO_MODE
							);	

   			debugMsgDisplayInISR(	"dualCmdAccFunc", 
									"SYSTEM DUAL MODE START",
       	        		            __FILE__, __LINE__,0,0,0,0
								);
			
			dualDataSyncFlagSet (LOGIC_ID);
        break;
        case RMT_POWER_OFF_CMD:
			logMessageSet(	__FILE__, 
							__LINE__,
							"dualCmdAccFunc", 
							RMT_POWER_OFF, 0
						);
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			
			rmtShelfPowerOffCmd(__FILE__, __LINE__, RMT_RESET_CMD);
#endif
			 
			systemRasInfoSet(	RAS_REDUNDANCY_ERR_STATUS, 
								SYS_REDUNDANCY_ERR
							);	
		break;
        case RMT_POWER_OFF_AND_LOCAL_RUN:
				 
			systemRasInfoSet(RAS_MPU_EXEC_LOOP_STATUS, 0);
	
			
			status = segmentLocalDataChecksumCheck(LOGIC_ID);
			if(status != NO_ERROR){
				systemRasInfoSet(RAS_MPU_CHECKSUM_STATUS, SEG_CHECKSUM_ERR);
				logMessageSet(__FILE__,__LINE__,"dualCmdAccFunc",MPU_SEG_CHKSUM_ERR,0);
				return(status);
			}
			else
				systemRasInfoSet(RAS_MPU_CHECKSUM_STATUS, NO_ERROR);
	
			for(taskId = 1;taskId <= MAX_LOGIC_TASK_NUM; taskId++) 
				status = sfcCurExecInfoSet(taskId);
			
			if(status)
				logMessageSet(	__FILE__, __LINE__,		
							"dualCmdAccFunc", 
							RMT_RESET_AND_SFC_EXE_INFO_SET_ERR, 0);
						
			
			systemRasInfoSet(	RAS_REDUNDANCY_ERR_STATUS, 
								SYS_REDUNDANCY_ERR
							);
	
	
		    systemRasInfoSet(RAS_SYSTEM_STATUS, SYS_RAS_SYSTEM_RUN);
			systemRasInfoSet(RAS_REDUNDANCY_STATUS, SYS_RAS_REDUNDANCY_RUN);
			
			logMessageSet(	__FILE__, 
							__LINE__,
							"dualCmdAccFunc", 
							RMT_POWEROFF_AND_LOCAL_RUN, 0
						);
        break;

		
		
		
		
		
		case ONLINE_DWL_CHG_MANUAL_TO_AUTO_CMD:
			if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_STANDBY) {
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			  	
        		gRunRmtShelfStatusCheckFlag = RUN_SHELF_STATUS_CHECK_DISABLE;
				
				gRunByRmtShelfLiveCntSendFlag = RUN_SHELF_LIVECNT_SEND_DISABLE;
#endif
	            
            	systemCnfgInfoRead(&sysCnfgInfo);
				
				status = fbDataCopyFromRedundancy(	sysCnfgInfo.shelfType,
													MPU_ONLY_DUAL_ACC,
													NO_PRINT
												);
    			if(status != NO_ERROR){
					setErrorCode	(	__FILE__,
       									__LINE__,
           	    	    	    		(uint8 *)"dualCmdAccFunc",
                       	    			status
									);
        			
        			
        			
        			
        		
     				
     				systemRasInfoSet(RAS_MPU_DUAL_COPY_STATUS, SYS_DUAL_COPY_ERR);
    			
    				
					allSegmentModeErrSet(LOGIC_ID);
					
					goto DUAL_OPERATION_SEND_2;
				}
				else { 
     				systemRasInfoSet(RAS_MPU_DUAL_COPY_STATUS, NO_ERROR);
				}
				
				systemRasInfoSet(RAS_MPU_EXEC_LOOP_STATUS, 0);

				
				status = segmentLocalDataChecksumCheck(LOGIC_ID);
				if(status != NO_ERROR){
					systemRasInfoSet(RAS_MPU_CHECKSUM_STATUS, SEG_CHECKSUM_ERR);
					goto DUAL_OPERATION_SEND_2;
				}
				else
					systemRasInfoSet(RAS_MPU_CHECKSUM_STATUS, NO_ERROR);

				
                for(taskId = 1; taskId <= sysMpuExeInfoPtr->curMaxSegmentId; taskId++) {
					status = rmtEthComSeqNumChkDBSet(taskId);
                    if(status)
                    	setErrorCode (	__FILE__,
                        					__LINE__,
                                            "dualCmdAccFunc",
                                            status
										);
				}				
				
				(void)fbExeSequenceQueuePtrClear(TOTAL_DWL);

				
				
				
                for(taskId = 1; taskId <= MAX_LOGIC_TASK_NUM; taskId++) {
					status = getFbTaskTblInfo	(	LOGIC_ID,	taskId,(strFbTaskTblMemInfo	*) &segTbl);
					if(status != NO_ERROR)
                    	setErrorCode (	__FILE__,__LINE__,
                                            "dualCmdAccFunc",status
										);
					if(segTbl.logicType == SFC_LOGIC_TYPE){
                		
						status = sfcExecIdInfoClear(taskId, LOGIC_ID);
						if(status)
                    		setErrorCode (	__FILE__,
                        						__LINE__,
												"dualCmdAccFunc",
												status
											);
						
						
						status = sfcExecIdInfoSet(taskId, LOGIC_ID);
						if(status)
                    		setErrorCode (	__FILE__,
                        						__LINE__,
												"dualCmdAccFunc",
												status
											);
					}
				
					else {
                		
						status = fbExecIdInfoClear(taskId, LOGIC_ID);
						if(status)
                    		setErrorCode (	__FILE__,
                        						__LINE__,
												"dualCmdAccFunc",
												status
											);
						
						
						status = fbExecIdInfoSet(taskId, LOGIC_ID);
						if(status)
                    		setErrorCode (	__FILE__,
                        						__LINE__,
												"dualCmdAccFunc",
												status
											);
					}
				}	
				
				
														
	            
				status = fbSpecUnusedMemoryInfoClear( LOGIC_ID);
                if(status)
                    setErrorCode (	__FILE__,
                        				__LINE__,
                                        "dualCmdAccFunc",
                                        status
									);
				
				
				status = fbSpecUnusedMemoryInfoSet(LOGIC_ID);
                if(status)
                    setErrorCode (	__FILE__,
                        				__LINE__,
                                        "dualCmdAccFunc",
                                        status
									);
				
				status = fbOutputUnusedMemoryInfoClear(LOGIC_ID);
                if(status)
                    setErrorCode (	__FILE__,__LINE__,
                                        "dualCmdAccFunc",status
									);
				
				
				status = fbOutputUnusedMemoryInfoSet(LOGIC_ID);
                if(status)
                    setErrorCode (	__FILE__,__LINE__,
                                        "dualCmdAccFunc",status
									);

				
				mpuDiufbSimFbNumSetInDualInfo ();

				
				segSchedulerInfoInit(MPU_ONLY_DUAL_ACC);

				
				schedulerModeChange(LOGIC_ID, SCHEDULER_ENABLE);
			
				
				
				
				
				
				
				
				
				
DUAL_OPERATION_SEND_2:				
                dualCmdInfo.dualCmd = SYS_DUAL_OPERATION_START;

                
				status = dualCmdMsgSendToRmtShelf(&dualCmdInfo);
                if(status)
                   	setErrorCode (	__FILE__,
                       					__LINE__,
                                        "dualCmdAccFunc",
                                        status
   									);

				
       			
        		
        		
        		
        		
        		sysTaskDelay(systemClockRateGet());
        		
        		
        		gRunRmtShelfStatusCheckFlag = RUN_SHELF_STATUS_CHECK_ENABLE;
			}
			
 			systemRasInfoSet(	RAS_SYSTEM_MODE, 
								SYS_RAS_SYSTEM_AUTO_MODE
							);	

   			debugMsgDisplayInISR(	"dualCmdAccFunc", 
									"MANUAL_TO AUTO_CHG_CMD",
       	        		            __FILE__, __LINE__,0,0,0,0
								);
        break;
        case ONLINE_DWL_REMOTE_RUN_CMD:
        	       	
			for(taskId=1;taskId<=MAX_LOGIC_TASK_NUM;taskId++) {
				status = sfcCurExecInfoSet(taskId);
			}
			
			logMessageSet(	__FILE__, 
							__LINE__,
							"dualCmdAccFunc", 
							LOCAL_RUN_BY_RMT_ONLINE_DWL_UPDATE_ERR, 0
				  		);			
        break;

        case RMT_FBCODE_FORCING_ON_OFF_CMD:
   			debugMsgDisplayInISR(	"dualCmdAccFunc", 
									"RMT_FBCODE_SIMUL_SINGLE_OFF_CMD",
       	        		            __FILE__, __LINE__,0,0,0,0
								);
			dualMsgDumpFromMyShelf(dualInfoPtr);
			
			
			if(	(dualInfoPtr->u.sysDualSimInfo.simFbCodeNum == 0) ||
				(dualInfoPtr->u.sysDualSimInfo.simFbCodeNum > MAX_DUAL_SIM_INFO_FBCODE_NUM)){
				status = SIM_FB_NUM_EXCEED_ERR;
				setErrorCodeWithVal(	__FILE__,
		           						__LINE__,
		                                "dualCmdAccFunc", 
		                                status,
		                                "recv Sim FB Code Num",	dualInfoPtr->u.sysDualSimInfo.simFbCodeNum,
		                                "Max Sim Fb Code Num",	MAX_DUAL_SIM_INFO_FBCODE_NUM,
		                                "Not Using", 0
									  );
				return(status);			
			}
			
			for(i = 0; i < dualInfoPtr->u.sysDualSimInfo.simFbCodeNum; i++) {								
				forcingInfo.moduleId 	= dualInfoPtr->u.sysDualSimInfo.moduleId; 					
				forcingInfo.taskId 		= dualInfoPtr->u.sysDualSimInfo.taskId; 					
				forcingInfo.FBCode 		= dualInfoPtr->u.sysDualSimInfo.FBCode[i]; 		
				forcingInfo.FBAddr 		= dualInfoPtr->u.sysDualSimInfo.FBAddr[i]; 		
				forcingInfo.pointId 	= (uint16)dualInfoPtr->u.sysDualSimInfo.pointId[i]; 	
				forcingInfo.forcingFlag = (uint16)dualInfoPtr->u.sysDualSimInfo.forcingFlag[i]; 

				status = forcingFlagWrite(&forcingInfo);
				if(status != NO_ERROR)
					setErrorCode(	(uint8 *)__FILE__, 
									(uint32)__LINE__, 
									(uint8 *)"dualCmdAccFunc",
	                                status
	                	    	);
			}
        break;  
        case RMT_FBCODE_SPEC_DATA_WRITE_CMD:
			debugMsgDisplayInISR(	"dualCmdAccFunc", 
									"RMT_FBCODE_SPEC_WRITE_CMD",
       	        		            __FILE__, __LINE__,0,0,0,0
								);
			dualMsgDumpFromMyShelf(dualInfoPtr);

		     
			status = fbMemInfoRead	(	dualInfoPtr->u.sysDualSpecInfo.moduleId,
		            					dualInfoPtr->u.sysDualSpecInfo.fbBlockAddr,
		            					(strFbMemInfo *)&fbMemInfo
		            				);
			if(status != NO_ERROR){
				setErrorCode(	(uint8 *)__FILE__, 
								(uint32)__LINE__, 
								(uint8 *)"dualCmdAccFunc",
                                status
                	    	);
				return(status);
			}
			
		    
		    if(dualInfoPtr->u.sysDualSpecInfo.fbId != fbMemInfo.fbId) {
		    	status = FB_CODE_MISMATCH_ERR;
				setErrorCodeWithVal(	__FILE__,
		           						__LINE__,
		                                "dualCmdAccFunc", 
		                                status,
		                                "Recv Block Addr",	dualInfoPtr->u.sysDualSpecInfo.fbBlockAddr,
		                                "Recv FB Code",	dualInfoPtr->u.sysDualSpecInfo.fbId,
		                                "Read FB Code",	fbMemInfo.fbId
									  );
		        return(status);
			}
			
		    
		    if( (dualInfoPtr->u.sysDualSpecInfo.writeIndex < 1) || 
		    	(dualInfoPtr->u.sysDualSpecInfo.writeIndex > fbMemInfo.inputNo) ){
		    	status = FB_CODE_SPEC_INDEX_ERR; 
				setErrorCodeWithVal(	__FILE__,
		           						__LINE__,
		                                "dualCmdAccFunc", status,
		                                "Block Addr",	dualInfoPtr->u.sysDualSpecInfo.fbBlockAddr,
		                                "FB Code",		dualInfoPtr->u.sysDualSpecInfo.fbId,
		                                "Write Index",	dualInfoPtr->u.sysDualSpecInfo.writeIndex
									  );
		        return(status);
			}

			status = fbCodeParaTypePointerGet(
										dualInfoPtr->u.sysDualSpecInfo.fbId,
										(strFbCodeTypePointerInfo *) &typePInfo
											);
			if(status != NO_ERROR) {
				setErrorCode	(
								__FILE__, 
								__LINE__,
								"dualCmdAccFunc",
								status
								);
				return(status);
			}
		
			
		    specTypePtr = (uint32 *)typePInfo.specTypeStartAddr;
			specTypePtr += (dualInfoPtr->u.sysDualSpecInfo.writeIndex - 1);
		
			specSizePtr = (uint32 *)typePInfo.specSizeStartAddr;
			specSizePtr += (dualInfoPtr->u.sysDualSpecInfo.writeIndex - 1);
		
			
			
			if( !(CHK_IS_ANY_TYPE(*specTypePtr)) && 
				(GET_BASIC_DATA_TYPE(*specTypePtr) != dualInfoPtr->u.sysDualSpecInfo.writeSpecType)) {
				status = FB_CODE_SPEC_TYPE_ERR;
				setErrorCode	(
								__FILE__, 
								__LINE__,
								"dualCmdAccFunc",
								status
								);
				return(status);
			}
		
			status = fbMemPointerGet(	dualInfoPtr->u.sysDualSpecInfo.moduleId,
		               					(strMemPointerInfo	*)&memP
					                );
			if(status != NO_ERROR) {
				setErrorCode	(
								__FILE__, 
								__LINE__,
								"dualCmdAccFunc",
								status
								);
				return(status);
			}
		
			specStartPtr = (uint32 *)memP.paraMemPtr;
		    
			specDataPtr = (uint32 *)(specStartPtr + fbMemInfo.specIndex + (*specSizePtr));
		
		    
			if(dualInfoPtr->u.sysDualSpecInfo.moduleId == LOGIC_ID) {
				specActionInfoPtr = (strFbSpecActionInfo *)(specStartPtr + fbMemInfo.specActionIndex + (dualInfoPtr->u.sysDualSpecInfo.writeIndex - 1));
				if(specActionInfoPtr->refType == SPEC_TYPE_BYREF) {
					setErrorCode	(
									__FILE__, 
									__LINE__,
									"dualCmdAccFunc",
									L_PARA_TUNE_ERR
									);
					return(L_PARA_TUNE_ERR);
				}
			}
					
			
			if(GET_TYPE_SIZE(*specTypePtr)==SIZE64_TYPE) specWriteSize = 2;
			else specWriteSize = 1;
		
		    
		    memoryCopy ( (uint8 *) specDataPtr,
		    			 (uint8 *) dualInfoPtr->u.sysDualSpecInfo.writeData,
		                 DOUBLE_SIZE * specWriteSize
						);
        break;  
        case RMT_RESET_CMD:
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			logMessageSet(	__FILE__, 
							__LINE__,
							"RMT RESET CMD", 
							LOCAL_POWER_OFF, 0);
			boardReboot();
#endif
	    break;
        case RMT_LIVE_CNT_ERR_CMD:
			if(	(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN)&& 
				(gDiuRasInfoPtr->redundancyStatus != SYS_REDUNDANCY_ERR)
			){
				
				
				status = remoteShelfReset(__FILE__, __LINE__);
				if(status)
					logMessageSet(	__FILE__, 
									__LINE__,
									"dualCmdAccFunc", 
									status, 
									0
								  );        	
				else 
					logMessageSet(	__FILE__, 
									__LINE__,
									"dualCmdAccFunc", 
									STBY_LIVE_COUNT_ERR, 
									0
								  );        	
			}
			else if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_STANDBY){
							
				
				status = rmtShelfCheckByCtrlNetCom(RMT_SHELF_LIVE_CHK_CMD, &dummy, 30 );

				if(status == NO_ERROR){
					
		   			debugMsgDisplayInISR(	"dualCmdAccFunc", 
											"REMOTE ALIVE BY COM CHK",
		       	        		            __FILE__, __LINE__,0,0,0,0
										);
					
					systemRasInfoSet(	RAS_REDUNDANCY_ERR_STATUS, 
										SYS_REDUNDANCY_ERR
									);
					gRunRmtShelfStatusCheckFlag = STANDBY_SHELF_STATUS_CHECK_DISABLE;

					logMessageSet(	__FILE__, 
									__LINE__,
									"dualCmdAccFunc", 
									RMT_SHELF_ALIVE_BY_COMM_CHK, 0
								);
									
					return (NO_ERROR); 	
				}
				else {
					rmtResetAndLocalRun (__FILE__, __LINE__, 0);
					
			    	
		   			debugMsgDisplayInISR(	"dualCmdAccFunc", 
											"REMOTE LIVE COUNT ERR",
	       		        		            __FILE__, __LINE__, 0,0,0,0
										);

	
					
					systemRasInfoSet(	RAS_REDUNDANCY_ERR_STATUS, 
										SYS_REDUNDANCY_ERR
									);
					logMessageSet(	__FILE__, 
									__LINE__,
									"dualCmdAccFunc", 
									RUN_LIVE_COUNT_ERR, 
									0
								  );        	
				}
			}
        break;
        default:
			status = SYS_DUAL_CMD_ERR;
      		setErrorCode(	(uint8 *)__FILE__,
    	        			(uint32)__LINE__,
        	        	    (uint8 *)"dualCmdAccFunc",
            	            status
						);
        break;
    }

    return(status);
}


uint32 dualCmdMsgSendToRmtShelf(strSysDualInfo *dualInfoPtr)
{
	uint32	status = NO_ERROR;
	strMpuDiuRunningStatusInfo *diuStatusInfoPtr = gDiuRasInfoPtr;

	if(diuStatusInfoPtr->redundancyStatus != NO_ERROR) {
		
        return(NO_ERROR);
    }
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
   	status = dualMsgWriteToRmtShelf(DUAL_CMD_ACC, 
									dualInfoPtr->dualCmd
								   );
#endif	
	if(status != NO_ERROR) {
		setErrorCode (	__FILE__,
        					__LINE__,
                            "dualCmdMsgSendToRmtShelf",
                            status
						);
		return(status);
    }

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	
	if(status == NO_ERROR) {
		status = dualCmdGenAndMsgSend (dualInfoPtr->dualCmd);
	}
#endif

  	return(status);
}


uint32	dualMsgDumpFromRmtShelf (strSysDualInfo *dualInfoPtr)
{
	uint32	status = NO_ERROR;
	
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	
	memoryCopy((uint8*)dualInfoPtr, (uint8*)&gRmtShelfDualInfo, sizeof(strSysDualInfo));

#else
	strSysConfigInfo sysCnfgInfo;
	uint32	*destPtr, *srcPtr;
	uint32	dualDataAddr, i;
	strMpuDiuRunningStatusInfo *diuStatusInfoPtr = gDiuRasInfoPtr;

	if(diuStatusInfoPtr->redundancyStatus != NO_ERROR)
		return(status);

    
    
    
	dualDataAddr = (uint32)gDualDataBusPtr;
    dualDataAddr &= 0xffffff;

    
    systemCnfgInfoRead(&sysCnfgInfo);

    if(sysCnfgInfo.shelfType == MASTER_SHELF) {
		srcPtr = (uint32 *)dualInfoPtr;
       	destPtr	= (uint32 *)(MEM_BOARD_BUS_ADDR_ON_SLAVE +
							 dualDataAddr
							);
    }
    else if(sysCnfgInfo.shelfType == SLAVE_SHELF) {
		srcPtr = (uint32 *)dualInfoPtr;
       	destPtr	= (uint32 *)(MEM_BOARD_BUS_ADDR_ON_MASTER +
							 dualDataAddr
							);
    }
    else {
		status = STATION_SHELF_CONFIG_ERR;
        return(status);
    }
    
    for(i = 0; i < sizeof(strSysDualInfo)/4;i++, destPtr++, srcPtr++){
		status = myBusDataAcc(	__FILE__,
								__LINE__,	
								BUS_LONG_ACC, 
								BUS_DATA_READ ,
								(uint8 *)destPtr, 
								(uint8 *)srcPtr
							 );
		if(status != NO_ERROR)
        	break;
	}
#endif

    return(status);
}


uint32 dualMsgWriteToRmtShelf(uint32 index, uint32 data)
{
	uint32	status = NO_ERROR, cmd;
	strSysConfigInfo *sysCnfgInfoPtr = &gSysCnfgInfo;
	uint32	*destPtr, *srcPtr, *indexPtr;
	uint32	dualDataAddr;
	strSysDualInfo	*dualInfoPtr;
	strMpuDiuRunningStatusInfo *diuStatusInfoPtr = gDiuRasInfoPtr;

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	if (diuStatusInfoPtr->redundancyStatus != NO_ERROR) {
		if((data != RMT_RESET_CMD) && (data != RMT_POWER_OFF_CMD))
			return(status);
	}
#else
	if(diuStatusInfoPtr->redundancyStatus != NO_ERROR)
		return(status);
#endif											
    dualInfoPtr = (strSysDualInfo *)gDualDataBusPtr;
    
    cmd = data;
    
    switch(index) {
		case DUAL_CMD_ACC:
		    
    		if( (cmd != REDUNDANCY_ERR_CLR_CMD) &&
    			(cmd != REMOTE_RUN_CMD) 		&&
        		(cmd != CHG_AUTO_TO_MANUAL_CMD) &&
        		(cmd != CHG_MANUAL_TO_AUTO_CMD) &&
        		(cmd != CHG_RUN_TO_STANDBY_CMD) && 
        		(cmd != SYS_DUAL_OPERATION_START) &&
				(cmd != ONLINE_DWL_IOCHG_MANUAL_TO_AUTO_CMD)&&
        		(cmd != ONLINE_DWL_CHG_MANUAL_TO_AUTO_CMD)&&
        		(cmd != ONLINE_DWL_REMOTE_RUN_CMD)
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
        		&&(cmd != RMT_RESET_CMD) 
        		&&(cmd != RMT_POWER_OFF_CMD)
#endif
			) {
    			status = SYS_DUAL_CMD_ERR;
    			return(status);
			}
       		indexPtr = (uint32 *) &dualInfoPtr->dualCmd;
       		
        break;
		case DUAL_SHELF_MODE_ACC:
        	indexPtr = (uint32 *) &dualInfoPtr->shelfMode;
        break;
		case DUAL_LIVE_CNT_ACC:
        	indexPtr = (uint32 *) &dualInfoPtr->shelfLiveCount;
        break;
		case DUAL_RMT_ERR_SUMMARY_ACC:
        	indexPtr = (uint32 *) &dualInfoPtr->rmtShelfErrSummary;
        break;
		case DUAL_MY_ERR_SUMMARY_ACC:
        	indexPtr = (uint32 *) &dualInfoPtr->myShelfErrSummary;
        break;
		case DUAL_SHELF_TYPE_ACC:
        	indexPtr = (uint32 *) &dualInfoPtr->shelfType;
        break;
		case DUAL_STATION_ID_ACC:
        	indexPtr = (uint32 *) &dualInfoPtr->stationId;
        break;
		case DUAL_RMT_LIVE_CNT_ACC:
        	indexPtr = (uint32 *) &dualInfoPtr->rmtShelfLiveCount;
        break;
		case DUAL_IO_INIT_FLAG_ACC:
        	indexPtr = (uint32 *) &dualInfoPtr->vmeIOInitFlag;
		break;
		
		case DUAL_FMC_GLV_ERR_SUM_ACC:
        	indexPtr = (uint32 *) &dualInfoPtr->fmcGblErrSummary;
		break;
		case DUAL_FORCING_MPU_CNT_ACC:
			indexPtr = (uint32 *) &dualInfoPtr->forcingMpuFbCodeNum;
		break;
		case DUAL_FORCING_DIU_CNT_ACC:
			indexPtr = (uint32 *) &dualInfoPtr->forcingDiuFbCodeNum;
		break;
        default:
        	status = DUAL_STATUS_FLAG_ERR;
            return(status);
		break;
	}	

    
    
    
    if(sysCnfgInfoPtr->memoryBoardType == MEM_BOARD_NONE)
		dualDataAddr = ((uint32)indexPtr - (uint32)MEMORY_BOARD_BUS_EMUL_ADDR);
#if (BOARD_TYPE == LKV120_BOARD)
    else if(sysCnfgInfoPtr->memoryBoardType == MEM_4MB_MRAM)
		dualDataAddr = ((uint32)indexPtr - (uint32)MEMORY_BOARD_BUS_EMUL_ADDR);
#endif
	else
		dualDataAddr = ((uint32)indexPtr - (uint32)MEMORY_BOARD_BUS_ADDR);
    dualDataAddr &= 0xffffff;
	srcPtr = &data;

    if(sysCnfgInfoPtr->shelfType == MASTER_SHELF) {
       	destPtr	= (uint32 *)(MEM_BOARD_BUS_ADDR_ON_SLAVE +
							 dualDataAddr
							);
    }
    else if(sysCnfgInfoPtr->shelfType == SLAVE_SHELF) {
       	destPtr	= (uint32 *)(MEM_BOARD_BUS_ADDR_ON_MASTER +
							 dualDataAddr
							);
    }
    else {
		status = STATION_SHELF_CONFIG_ERR;
        return(status);
    }

	
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	status = myRmtDataAcc(	__FILE__,
							__LINE__,
							BUS_LONG_ACC, 
							BUS_DATA_WRITE ,
							(uint8 *)destPtr, 
							(uint8 *)srcPtr
						 );
#endif

  	return(status);
}


uint32	dualMsgWriteToMyShelf(uint32 index, uint32 data)
{
	uint32	status = NO_ERROR;
	uint32	*destPtr, *dataPtr;
	strSysDualInfo	*dualInfoPtr;
	
    dualInfoPtr = (strSysDualInfo *) gDualDataBusPtr;
	dataPtr = (uint32 *) &data;

    switch(index) {
		case DUAL_CMD_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->dualCmd;
        break;
		case DUAL_SHELF_MODE_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->shelfMode;
        break;
		case DUAL_LIVE_CNT_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->shelfLiveCount;
        break;
		case DUAL_RMT_ERR_SUMMARY_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->rmtShelfErrSummary;
        break;
		case DUAL_MY_ERR_SUMMARY_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->myShelfErrSummary;
        break;
		case DUAL_SHELF_TYPE_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->shelfType;
        break;
		case DUAL_STATION_ID_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->stationId;
        break;
		case DUAL_RMT_LIVE_CNT_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->rmtShelfLiveCount;
        break;
		case DUAL_IO_INIT_FLAG_ACC:
			destPtr = (uint32 *) &dualInfoPtr->vmeIOInitFlag;
		break;
		case DUAL_IP_ADDR_A_ACC:
			destPtr = (uint32 *) &dualInfoPtr->myIpAddr_A;
		break;
		case DUAL_IP_ADDR_B_ACC:
			destPtr = (uint32 *) &dualInfoPtr->myIpAddr_B;
		break;
		
		case DUAL_FMC_GLV_ERR_SUM_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->fmcGblErrSummary;
		break;

		case DUAL_FORCING_MPU_CNT_ACC:
			destPtr = (uint32 *) &dualInfoPtr->forcingMpuFbCodeNum;
		break;

		case DUAL_FORCING_DIU_CNT_ACC:
			destPtr = (uint32 *) &dualInfoPtr->forcingDiuFbCodeNum;
		break;
		case DUAL_MEM_BRD_TYPE_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->memoryBrdType;
        break;
		case DUAL_SW_ENGINE_TYPE_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->sysSchedlueType;
        break;
        default:
        	status = DUAL_STATUS_FLAG_ERR;
            return(status);
		break;
	}	

	status = myBusDataAcc(	__FILE__,
							__LINE__,	
							BUS_LONG_ACC, 
							BUS_DATA_WRITE ,
							(uint8 *)destPtr, 
							(uint8 *)dataPtr
						 );
    return(status);
}


uint32	dualMsgReadFromMyShelf(uint32 index, uint32 *dataPtr)
{
	uint32	status = NO_ERROR;
	uint32	*destPtr;
	strSysDualInfo	*dualInfoPtr;
	
    dualInfoPtr = (strSysDualInfo *) gDualDataBusPtr;

    switch(index) {
		case DUAL_CMD_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->dualCmd;
        break;
		case DUAL_SHELF_MODE_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->shelfMode;
        break;
		case DUAL_LIVE_CNT_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->shelfLiveCount;
        break;
		case DUAL_RMT_ERR_SUMMARY_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->rmtShelfErrSummary;
        break;
		case DUAL_MY_ERR_SUMMARY_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->myShelfErrSummary;
        break;
		case DUAL_SHELF_TYPE_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->shelfType;
        break;
		case DUAL_STATION_ID_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->stationId;
        break;
		case DUAL_RMT_LIVE_CNT_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->rmtShelfLiveCount;
        break;

		case DUAL_FORCING_MPU_CNT_ACC:
			destPtr = (uint32 *) &dualInfoPtr->forcingMpuFbCodeNum;
		break;
		case DUAL_FORCING_DIU_CNT_ACC:
			destPtr = (uint32 *) &dualInfoPtr->forcingDiuFbCodeNum;
		break;
        default:
        	status = DUAL_STATUS_FLAG_ERR;
            return(status);
		break;

	}

	status = myBusDataAcc(	__FILE__,
							__LINE__,	
							BUS_LONG_ACC, 
							BUS_DATA_READ,
							(uint8 *)destPtr, 
							(uint8 *)dataPtr
						 );
    return(status);
}


uint32	dualMsgReadFromRmtShelf (uint32 index, uint32 *dataPtr)
{
	uint32	status = NO_ERROR;
	strSysConfigInfo *sysCnfgInfoPtr = &gSysCnfgInfo;
	uint32	*destPtr, *srcPtr;
	uint32	dualDataAddr;
	strMpuDiuRunningStatusInfo *diuStatusInfoPtr = gDiuRasInfoPtr;

	strSysDualInfo	*dualInfoPtr;

    dualInfoPtr = (strSysDualInfo *)gDualDataBusPtr;

	if(diuStatusInfoPtr->redundancyStatus != NO_ERROR)
		return(status);

    switch(index) {
		case DUAL_CMD_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->dualCmd;
        break;
		case DUAL_SHELF_MODE_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->shelfMode;
        break;
		case DUAL_LIVE_CNT_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->shelfLiveCount;
        break;
		case DUAL_RMT_ERR_SUMMARY_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->rmtShelfErrSummary;
        break;
		case DUAL_MY_ERR_SUMMARY_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->myShelfErrSummary;
        break;
		case DUAL_SHELF_TYPE_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->shelfType;
        break;
		case DUAL_STATION_ID_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->stationId;
        break;
		case DUAL_RMT_LIVE_CNT_ACC:
        	destPtr = (uint32 *) &dualInfoPtr->rmtShelfLiveCount;
        break;

		case DUAL_FORCING_MPU_CNT_ACC:
			destPtr = (uint32 *) &dualInfoPtr->forcingMpuFbCodeNum;
		break;
		case DUAL_FORCING_DIU_CNT_ACC:
			destPtr = (uint32 *) &dualInfoPtr->forcingDiuFbCodeNum;
		break;
        default:
        	status = DUAL_STATUS_FLAG_ERR;
            return(status);
		break;

	}

    
    
    
	dualDataAddr = (uint32)destPtr;
    dualDataAddr &= 0xffffff;

	srcPtr = (uint32 *)dataPtr;

    if(sysCnfgInfoPtr->shelfType == MASTER_SHELF) 
       	destPtr	= (uint32 *)(MEM_BOARD_BUS_ADDR_ON_SLAVE +
							 dualDataAddr
							);
    
    else if(sysCnfgInfoPtr->shelfType == SLAVE_SHELF) 
       	destPtr	= (uint32 *)(MEM_BOARD_BUS_ADDR_ON_MASTER +
							 dualDataAddr
							);
    
    else {
		status = STATION_SHELF_CONFIG_ERR;
        return(status);
    }
    
	status = myBusDataAcc(	__FILE__,
							__LINE__,	
							BUS_LONG_ACC, 
							BUS_DATA_READ ,
							(uint8 *)destPtr, 
							(uint8 *)srcPtr
						 );

    return(status);
}


uint32	dualMsgDumpFromMyShelf(strSysDualInfo *dataPtr)
{
	uint32	status = NO_ERROR;
	uint32	*destPtr,*srcPtr, i, dataNum;
		
    srcPtr	= gDualDataBusPtr;
	destPtr = (uint32 *) dataPtr;

    dataNum = sizeof(strSysDualInfo)/4;

    memoryClear ( (uint8 *) dataPtr, dataNum*4);

	for(i = 0; i < dataNum; i++, srcPtr++, destPtr++) {
		status = myBusDataAcc(	__FILE__,
								__LINE__,	
								BUS_LONG_ACC, 
								BUS_DATA_READ,
								(uint8 *)srcPtr, 
								(uint8 *)destPtr
							 );
		if(status != NO_ERROR) break;
	}
    return(status);
}


uint32	localShelfStatusMonitoring(void)
{
	uint32	status = NO_ERROR;
	strSysDualInfo	rmtDualInfo,dualCmdInfo;
	uint32	errMask = 0, myErrSummary = 0;
	strSysConfigInfo sysCnfgInfo;

	memoryClear( (uint8 *)&rmtDualInfo, sizeof(strSysDualInfo));

	systemCnfgInfoRead(&sysCnfgInfo);
	
	
    if( dualFlagCheckInRunShelf() == DUAL_CHECK_ENABLE){ 
    	
		status = dualMsgDumpFromRmtShelf (&rmtDualInfo);
		if(status != NO_ERROR) {
			setErrorCode(	__FILE__,
       						__LINE__,
           	    		    "localShelfStatusMonitoring",
               	        	status
					   	  );
			return(status);
   		}

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
		errMask = DUAL_FBC_LIVECNT_ERR| USR_TASK_SUSPEND_ERR|DUAL_FMC_LIVECNT_ERR|DUAL_FMC_IO_COMM_ERR;
		if(rmtDualInfo.myShelfErrSummary & errMask) 
		{
			if(rmtDualInfo.myShelfErrSummary & USR_TASK_SUSPEND_ERR) 
				logMessageSet(__FILE__, __LINE__,__FUNCTION__, APP_TASK_SUSPEND_ERR, 0);
				
			
			myErrSummary = 0;
			status = dualMsgReadFromMyShelf(DUAL_MY_ERR_SUMMARY_ACC, 
											(uint32 *) &myErrSummary
											);
			if(status != NO_ERROR) 
			{
				setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
				return(status);
			}
			
			
			errMask = DUAL_FBC_LIVECNT_ERR| USR_TASK_SUSPEND_ERR|DUAL_FMC_LIVECNT_ERR|DUAL_FMC_IO_COMM_ERR;
			
			if(myErrSummary & errMask) 
			{					
				if(myErrSummary & USR_TASK_SUSPEND_ERR) 
					logMessageSet(__FILE__, __LINE__,__FUNCTION__, APP_TASK_SUSPEND_ERR, 0);				
			}
		} 
#endif
		
		myErrSummary = 0;
		status = dualMsgReadFromMyShelf(	DUAL_MY_ERR_SUMMARY_ACC, 
											(uint32 *) &myErrSummary
										);
		if(status != NO_ERROR) 
		{
			setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}

		if(	(myErrSummary != NO_ERROR) && (rmtDualInfo.myShelfErrSummary == NO_ERROR)){
 			errMask = DUAL_CTRL_NET_ERR|DUAL_FBC_LIVECNT_ERR| USR_TASK_SUSPEND_ERR|DUAL_FMC_LIVECNT_ERR|DUAL_FMC_IO_COMM_ERR;
	    	if(myErrSummary & errMask) {
				
				if(myErrSummary & DUAL_CTRL_NET_ERR) {
					logMessageSet(__FILE__, __LINE__, __FUNCTION__, CTRL_NET_COM_ERR, 0);
				}
									
				if(myErrSummary & USR_TASK_SUSPEND_ERR) {
					logMessageSet(__FILE__, __LINE__, __FUNCTION__, APP_TASK_SUSPEND_ERR, 0);
				}
			}
			else 
				return(status);
		    
            dualCmdInfo.dualCmd 			= REMOTE_RUN_CMD;
			dualCmdInfo.myShelfErrSummary	= myErrSummary;

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
				(void)dualMsgWriteToMyShelf(	DUAL_CMD_ACC, 
												dualCmdInfo.dualCmd
											);
#endif
            
			status = dualCmdMsgSendToRmtShelf(&dualCmdInfo);
            if(status)
				setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
			else {
				
			    systemRasInfoSet(RAS_SYSTEM_STATUS, SYS_RAS_SYSTEM_STOP);
			    
			    
			    
			    systemRasInfoSet(RAS_REDUNDANCY_STATUS, SYS_RAS_REDUNDANCY_STANDBY);
			    
				
				gStandByRmtShelfStatusCheckFlag = STANDBY_SHELF_STATUS_CHECK_DISABLE;
				gRunRmtShelfStatusCheckFlag 	= RUN_SHELF_STATUS_CHECK_DISABLE;

				logMessageSet(	__FILE__, __LINE__, __FUNCTION__, RMT_RUN_BY_LOCAL_ERR, 0);
			}
		}
	}

    return(status);
}


uint32 rmtResetAndLocalRun (uint8 *fileName, uint32 lineNum, uint32 rmtErrSummary)
{
	uint32	status = NO_ERROR;
	uint32  taskId =0;

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	
	remoteShelfReset(fileName, lineNum);
#endif
    
	systemRasInfoSet(	RAS_REDUNDANCY_ERR_STATUS, 
						SYS_REDUNDANCY_ERR
					);

	status = segmentLocalDataChecksumCheck(LOGIC_ID);
	if(status != NO_ERROR){
		systemRasInfoSet(RAS_MPU_CHECKSUM_STATUS, SEG_CHECKSUM_ERR);
		setErrorCode(	__FILE__,
						__LINE__,
    	    	        "rmtResetAndLocalRun",
        	    	    status
				   	);
		goto MODE_SET;
	}
	else
		systemRasInfoSet(RAS_MPU_CHECKSUM_STATUS, NO_ERROR);

	
	for(taskId = 1;taskId <= MAX_LOGIC_TASK_NUM; taskId++)
		status = sfcCurExecInfoSet(taskId);

MODE_SET:
	
	
    systemRasInfoSet(RAS_SYSTEM_STATUS, SYS_RAS_SYSTEM_RUN);
	systemRasInfoSet(RAS_REDUNDANCY_STATUS, SYS_RAS_REDUNDANCY_RUN);
	

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	
	gStandByRmtShelfStatusCheckFlag = STANDBY_SHELF_STATUS_CHECK_DISABLE;
	gRunRmtShelfStatusCheckFlag 	= RUN_SHELF_STATUS_CHECK_DISABLE;

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	
	
	gRunByRmtShelfLiveCntSendFlag = RUN_SHELF_STATUS_CHECK_DISABLE;
#endif
#endif

	logMessageSet(	fileName, 
					lineNum,
					"rmtResetAndLocalRun", 
					LOCAL_RUN, 0
				  );
	
    return(status);
}


uint32	dualIpAddrSetInMyMemory(void)
{
	uint32	status = NO_ERROR;
	strSysConfigInfo	sysCnfgInfo;
	
	systemCnfgInfoRead(&sysCnfgInfo);

	
	status = dualMsgWriteToMyShelf	(	DUAL_IP_ADDR_A_ACC, 
										sysCnfgInfo.netCnfgInfo.myIpAddr[0]
									);
	if(status == NO_ERROR) {
		
		status = dualMsgWriteToMyShelf(	DUAL_IP_ADDR_B_ACC, 
										sysCnfgInfo.netCnfgInfo.myIpAddr[1]
									);
	}
	return(status);
}

void dualDataSyncFlagEnableCheck(uint32	moduleId)
{
	switch(moduleId) {
		case LOGIC_ID:
			if(sDualMpuDataSyncFlag == DATA_SYNC_ENABLE)
				gDualMpuDataCopyFlag = DATA_SYNC_ENABLE;
			else
				gDualMpuDataCopyFlag = DATA_SYNC_DISABLE;
		break;
		default:	
		break;		
	}
	
	return;	
}


static void dualDataSyncFlagSet(uint32	moduleId)
{
	switch(moduleId) {
		case LOGIC_ID:
			
			if(gMpuSegSchdInfoPtr->cyclicSegNum != 0)
				sDualMpuDataSyncFlag = DATA_SYNC_ENABLE;
		break;
		default:	
		break;		
	}
	
	return;	
}


void dualDataSyncFlagDisable(uint32	moduleId)
{
	switch(moduleId) {
		case LOGIC_ID:
			if(gDualMpuDataCopyFlag == DATA_SYNC_ENABLE)
				sDualMpuDataSyncFlag = DATA_SYNC_DISABLE;
		break;
		default:	
		break;		
	}
	
	return;	
}

void dualDataSyncFlagDisplay (void)
{
    int8 *dualFlagStr[] = 
    {
    	"ENABLE ", "DISABLE"
    };
	uint8	strIndex;
	
	
	if(sDualMpuDataSyncFlag == DATA_SYNC_ENABLE)
		strIndex = 0;
	else
		strIndex = 1;
	printf( "\t  MPU Data Sync Flag : %s\n",dualFlagStr[strIndex]);

	if(gDualMpuDataCopyFlag == DATA_SYNC_ENABLE)
		strIndex = 0;
	else
		strIndex = 1;
	printf( "\t  MPU Data Copy Flag : %s\n",dualFlagStr[strIndex]);

	
	if(sDualDiuDataSyncFlag == DATA_SYNC_ENABLE)
		strIndex = 0;
	else
		strIndex = 1;
	printf( "\t  DIU Data Sync Flag : %s\n",dualFlagStr[strIndex]);

	if(gDualDiuDataCopyFlag == DATA_SYNC_ENABLE)
		strIndex = 0;
	else
		strIndex = 1;
	printf( "\t  DIU Data Copy Flag : %s\n",dualFlagStr[strIndex]);
	
}


void dualMsgStatusInfoDisplay(void)
{
	uint32 status = NO_ERROR;
	strSysDualInfo	dualInfo;
	uint8 *indexStr[] = {
		"CMD","MODE","LVE CNT","LCL ERR", "RMT ERR","TYPE","ID","RMT CNT","IO FLAG",
        NULL
    };
    uint8	**pPtr;
	uint8	*defaultStr = "UNKNOWN";

    uint8 *dualCmdStr[] = {
    	"NO CMD",
		"ERR CL",
        "RMT RUN",
        "A->M",
        "M->A",
        "R->S",
		"CHK_LIVE",
		"ONDWL M->A",
		"ONDWL RMT RUN",
		"FORCING",
		"SPEC WRITE"
    };
	uint8	*shelfModeStr[] = {
		"NO MODE ",
		"AUTO ",
		"MANUAL"
    };
    uint8 *shelfTypeStr [] = {
		"UNKNOWN",
        "MASTER",
        "SLAVE"
    };
    uint8	*errSummaryStr[] = {
		"NO E",
        "NET",			
        "IO",			
		"NET|IO",		
		"FMC",			
		"FMC|NET",		
        "FMC|IO",		
        "FMC|NET|IO",	
    };

	uint8	*cmdStrPtr,*shelfModeStrPtr, *shelfTypeStrPtr;
    uint8	*rmtErrStrPtr,*localErrStrPtr;

    printf( "\n");

    for( pPtr = indexStr; *pPtr != NULL; pPtr++)
    	printf( "%8s", *pPtr);

    printf( "\n");
    for( pPtr = indexStr; *pPtr != NULL; pPtr++)
    	printf( "%8s", "-------");

    printf( "\n");

    while( !keyHit()) {
		status = dualMsgDumpFromMyShelf(&dualInfo);

		if(status != NO_ERROR) break;

		if(dualInfo.dualCmd > RMT_FBCODE_SPEC_DATA_WRITE_CMD)
			cmdStrPtr = defaultStr;
		else
			cmdStrPtr = dualCmdStr[dualInfo.dualCmd];
		
		if(dualInfo.shelfMode > 2) 
			shelfModeStrPtr = defaultStr;
		else
			shelfModeStrPtr = shelfModeStr[dualInfo.shelfMode];

		if(dualInfo.shelfType > 2) 
			shelfTypeStrPtr = defaultStr;
		else
			shelfTypeStrPtr = shelfTypeStr[dualInfo.shelfType];

		if(dualInfo.rmtShelfErrSummary == NO_ERROR)
			rmtErrStrPtr = errSummaryStr[0];
		else {
			if( dualInfo.rmtShelfErrSummary <= (DUAL_CTRL_NET_ERR|DUAL_IO_COMM_ERR|DUAL_FMC_IO_COMM_ERR))
            	rmtErrStrPtr = errSummaryStr[dualInfo.rmtShelfErrSummary];
			else
				rmtErrStrPtr = defaultStr;

		} 			   
		if(dualInfo.myShelfErrSummary == NO_ERROR)
			localErrStrPtr = errSummaryStr[0];
		else {
			if( dualInfo.myShelfErrSummary <= (DUAL_CTRL_NET_ERR|DUAL_IO_COMM_ERR|DUAL_FMC_IO_COMM_ERR))
            	localErrStrPtr = errSummaryStr[dualInfo.myShelfErrSummary];
			else
            	localErrStrPtr = defaultStr;
		} 			   
		printf("%8s%8s%8x%8s%8s%8s%8d%8x%8x\r",
        						cmdStrPtr,
                                shelfModeStrPtr,
                                dualInfo.shelfLiveCount,
                                localErrStrPtr,
								rmtErrStrPtr,
                                shelfTypeStrPtr,
                                dualInfo.stationId,
                                dualInfo.rmtShelfLiveCount,
								dualInfo.vmeIOInitFlag
				);
    	sysTaskDelay(systemClockRateGet());
    }

    if(status != NO_ERROR)
    	printf( "\n[ERROR] - Dual Memory Read Error[0x%x]\n",
        						status
				);
	return;
}

void dualSysInfoDisplay(void)
{
	strSysDualInfo* dualInfoPtr;
	dualInfoPtr = (strSysDualInfo *) gDualDataBusPtr;

	printf("\n\t**************** Dual Info Data *****************\n");
	printf(" \t\tDual Cmd        : 0x%0x\n", dualInfoPtr->dualCmd);
	printf(" \t\tShelf Mode      : 0x%0x\n", dualInfoPtr->shelfMode);
	printf(" \t\tRmt Error Sum   : 0x%0x\n", dualInfoPtr->rmtShelfErrSummary);
	printf(" \t\tMy Error Sum    : 0x%0x\n", dualInfoPtr->myShelfErrSummary);
	printf(" \t\tShelf Type      : 0x%0x\n", dualInfoPtr->shelfType);
	printf(" \t\tShelf ID        : 0x%0x\n", dualInfoPtr->stationId);
	printf(" \t\tRmt Live Cnt    : 0x%0x\n", dualInfoPtr->rmtShelfLiveCount);
	printf(" \t\tVME I/O Init    : 0x%0x\n", dualInfoPtr->vmeIOInitFlag);
	printf(" \t\tLine_A IP Addr  : 0x%0x\n", dualInfoPtr->myIpAddr_A);
	printf(" \t\tLine_B IP Addr  : 0x%0x\n", dualInfoPtr->myIpAddr_B);
	printf(" \t\tFMC Error Sum   : 0x%0x\n", dualInfoPtr->fmcGblErrSummary);
	printf(" \t\tMPU Forcing Cnt : 0x%0x\n", dualInfoPtr->forcingMpuFbCodeNum);
	printf(" \t\tDIU Forcing Cnt : 0x%0x\n", dualInfoPtr->forcingDiuFbCodeNum);
	printf(" \t\tMem Board Type  : 0x%0x\n", dualInfoPtr->memoryBrdType);
}

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
uint32	liveCountMonitoring(void)
{
	uint32	status = NO_ERROR, dummy = 0;
	strSysDualInfo	rmtDualInfo;
	strSysRunningConfigInfo	*sysStatusInfoPtr = gSysRunningInfoPtr;
	uint32	rmtShelfLiveCount = 0;
	uint32  myErrSummary=0,errMask = 0;
	strMpuDiuRunningStatusInfo *diuStatusInfoPtr;
	
	diuStatusInfoPtr = (strMpuDiuRunningStatusInfo *)
							&gSysRunningStatusInfo.sysDiuRunningStatusInfo;

	memoryClear( (uint8 *)&rmtDualInfo, sizeof(strSysDualInfo));

	
    
	if( (dualFlagCheckInRunShelf() == DUAL_CHECK_ENABLE) 
								|| 
		( (sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN) &&
		  (diuStatusInfoPtr->redundancyStatus == NO_ERROR) &&
		  
		  (gRunByRmtShelfLiveCntSendFlag == STANDBY_SHELF_STATUS_CHECK_ENABLE) &&
		  (sysStatusInfoPtr->sysOperationMode == SYS_RAS_SYSTEM_MANUAL_MODE) )
	) {
		status = dualMsgDumpFromRmtShelf (&rmtDualInfo);
		if(status != NO_ERROR) {
			setErrorCode(	__FILE__,
       						__LINE__,
           	    		    "liveCountMonitoring",
               	        	status
					   	  );
			return(status);
   		}

		rmtShelfLiveCount = rmtDualInfo.shelfLiveCount;
		
		
	    if( rmtShelfLiveCount != sRmtShelfPrevLiveCount){
			sRmtShelfPrevLiveCount = rmtShelfLiveCount;
			sRmtLiveCountErrCount = 0;
			
            
			systemRasInfoSet(	RAS_REDUNDANCY_ERR_STATUS, 
								NO_ERROR
							);
			
			(void)dualMsgWriteToMyShelf(	DUAL_RMT_LIVE_CNT_ACC, 
											rmtShelfLiveCount
										);
		}
		else 
			sRmtLiveCountErrCount++;
			
		
	    if(gDualRecvDataLiveCount != sDualRecvPrevLiveCount){

			sDualRecvPrevLiveCount = gDualRecvDataLiveCount;
			sRmtDataRcvCountErrCount = 0;
					
            
			systemRasInfoSet(	RAS_REDUNDANCY_ERR_STATUS, 
								NO_ERROR
							);
		}
		else 
			sRmtDataRcvCountErrCount++;
		
		if( (sRmtLiveCountErrCount >= MAX_DUAL_LIVE_COUNT_ERR) 
								&&
			(sRmtDataRcvCountErrCount >= MAX_DUAL_LIVE_COUNT_ERR)
		){
			
			sRmtErrAccCount 			= 0;
            sRmtLiveCountErrCount 		= 0;
			sRmtDataRcvCountErrCount 	= 0;
			
			
			status = rmtShelfCheckByCtrlNetCom(	RMT_SHELF_LIVE_CHK_CMD,
												&dummy, 
												30 
												);
			if(status == NO_ERROR){ 
	   			debugMsgDisplayInISR(	"liveCountMonitoring", 
										"REMOTE ALIVE BY COM CHK",
	       	        		            __FILE__, __LINE__,0,0,0,0
									);	
				systemRasInfoSet(	RAS_REDUNDANCY_ERR_STATUS, 
									SYS_REDUNDANCY_ERR
								);
				logMessageSet(__FILE__, __LINE__,"liveCountMonitoring", STBY_LIVE_COUNT_ERR, 0);
			}
			else { 
	   			debugMsgDisplayInISR(	"liveCountMonitoring", 
										"REMOTE DIE BY COM CHK",
	       	        		            __FILE__, __LINE__,0,0,0,0
									);	
				
				
				
				
				gStandByRmtShelfStatusCheckFlag = STANDBY_SHELF_STATUS_CHECK_DISABLE;
				gRunRmtShelfStatusCheckFlag 	= RUN_SHELF_STATUS_CHECK_DISABLE;
				
				
				logMessageSet(__FILE__, __LINE__,"liveCountMonitoring", STBY_LIVE_COUNT_ERR, 0);

				status = remoteShelfReset(__FILE__,__LINE__);
				if(status)
					logMessageSet(__FILE__, __LINE__,"liveCountMonitoring", status, status);
	
				
				systemRasInfoSet(	RAS_REDUNDANCY_ERR_STATUS, 
									SYS_REDUNDANCY_ERR
								);
	
				
				
				gRunByRmtShelfLiveCntSendFlag = RUN_SHELF_LIVECNT_SEND_DISABLE;
			}
		}
	}
	
	
	
   	else if( ( (sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_STANDBY)
   									&&
		  		(diuStatusInfoPtr->redundancyStatus == NO_ERROR) 
		  							&&
   				(gRunByRmtShelfLiveCntSendFlag == STANDBY_SHELF_STATUS_CHECK_ENABLE) 
									&&
		    	(sysStatusInfoPtr->sysOperationMode == SYS_RAS_SYSTEM_MANUAL_MODE) 
		    )
									||
			( 	(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_STANDBY)
   									&&
		  		(diuStatusInfoPtr->redundancyStatus == NO_ERROR) 
		  							&&
				(gRunRmtShelfStatusCheckFlag == RUN_SHELF_STATUS_CHECK_ENABLE)
									&&
		    	(sysStatusInfoPtr->sysOperationMode == SYS_RAS_SYSTEM_AUTO_MODE) 
		    )
		
   	){ 

		
		status = dualMsgDumpFromRmtShelf (&rmtDualInfo);

		
		if( rmtDualInfo.shelfLiveCount != sRmtShelfPrevLiveCount) {
			sRmtShelfPrevLiveCount = rmtDualInfo.shelfLiveCount;
			
 			sRmtLiveCountErrCount = 0;
		}
		else
			sRmtLiveCountErrCount++;
		 
		
	    if(gDualRecvDataLiveCount != sDualRecvPrevLiveCount){

			sDualRecvPrevLiveCount = gDualRecvDataLiveCount;
			sRmtDataRcvCountErrCount = 0;
					
            
			systemRasInfoSet(	RAS_REDUNDANCY_ERR_STATUS, 
								NO_ERROR
							);
		}
		else
			sRmtDataRcvCountErrCount++;

		
		 
		if(  (sRmtLiveCountErrCount >= MAX_DUAL_LIVE_COUNT_ERR) &&
			 (sRmtDataRcvCountErrCount >= MAX_DUAL_LIVE_COUNT_ERR ) 	
		){
			
			sRmtLiveCountErrCount 		= 0;
			sRmtDataRcvCountErrCount	= 0;
			
			status = dualMsgReadFromMyShelf(	DUAL_MY_ERR_SUMMARY_ACC, 
												(uint32 *) &myErrSummary
											);

			errMask = DUAL_CTRL_NET_ERR|DUAL_IO_COMM_ERR|DUAL_FBC_LIVECNT_ERR|USR_TASK_SUSPEND_ERR;

			if(status != NO_ERROR) {
				setErrorCode(	__FILE__,__LINE__,
        	    				"liveCountMonitoring",status
					   		);
				return(status);
			}
			
			
			if((myErrSummary & (USR_TASK_SUSPEND_ERR))== NO_ERROR)
			{
				
				status = rmtShelfCheckByCtrlNetCom(	RMT_SHELF_LIVE_CHK_CMD,
													&dummy, 
													30 
													);
				if(status == NO_ERROR){	
					
					
		   			debugMsgDisplayInISR(	"liveCountMonitoring", 
											"REMOTE ALIVE BY COM CHK",
		       	        		            __FILE__, __LINE__,0,0,0,0
										);
					systemRasInfoSet(	RAS_REDUNDANCY_ERR_STATUS, 
										SYS_REDUNDANCY_ERR
									);
					logMessageSet(__FILE__, __LINE__,"liveCountMonitoring", RUN_LIVE_COUNT_ERR, 0);
				}			
				else {
					
	   				debugMsgDisplayInISR(	"liveCountMonitoring", 
											"REMOTE RESET FOR LIVE COUNT ERR",
       		        						__FILE__,__LINE__,0,0,0,0
										);
					logMessageSet(__FILE__, __LINE__,"liveCountMonitoring", RUN_LIVE_COUNT_ERR, 0);

					rmtResetAndLocalRun (__FILE__,__LINE__,0);
				}
			}
   		}
	}

    return(status);
}



uint32 dualCmdGenAndMsgSend(uint32 dualCmd)
{
	strMsgQueData	msgQData;
	uint32	status = NO_ERROR;
	strDualProtocolInfo	*netMsgPtr;
	uint32	qDataLen = 0;

	strinetAddr localAddr;
	strSysConfigInfo sysCnfgInfo;
	systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);
    	
	memoryClear( (uint8 *) &msgQData, sizeof(strMsgQueData));

   
	localAddr.iAddr= sysCnfgInfo.redNetCnfgInfo.rmtDluIpAddrHex;	
	status = comPortInfoGet(RMTRESPS, &localAddr, &msgQData.peerAddr);

	if(status != NO_ERROR) goto MODULE_END;

    netMsgPtr = (strDualProtocolInfo *)msgQData.protoData.msgData;

    status = dualCommandGen (netMsgPtr, dualCmd);
	if(status != NO_ERROR) goto MODULE_END;
    
    msgQData.protoData.numOfBytes = MAX_DUAL_FRAME_HEAD_LEN + netMsgPtr->dataLength; 

	
    
    qDataLen = SOCK_ADDR_LENGTH + msgQData.protoData.numOfBytes + DOUBLE_SIZE;
	
	
    status = messageQueueSend(	gSysDualSendMsgQueId,
                               	(int8 *) &msgQData,
                               	qDataLen,
                               	NOWAIT,
                                MYMSG_PRI_URGENT
                              );

MODULE_END:
	if(status) {
		setErrorCode(	__FILE__, 
						__LINE__,
						"dualCmdGenAndMsgSend",
						status
					  ); 	
		logMessageSet(__FILE__,__LINE__,
			"dualCmdGenAndMsgSend",DUAL_SEND_QUEUE_SEND_ERR,0);
	}

	return(status);
}


uint32 dualConfigDataGen(strDataBuf*sendData,strinetAddr*peerAddr,uint32 optionCode)
{
	uint32	status = NO_ERROR;
	strSysConfigInfo sysCnfgInfo;
	strDualProtocolInfo	*netMsgPtr;
	strSysDualInfo dualInfo;
	uint32 cmdCode, checkSum;
	strinetAddr localAddr;
	uint32	redundancyStatus = 0;
	
	memoryClear ( (uint8 *) &sysCnfgInfo, sizeof(strSysConfigInfo));
	systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);
	
	
	localAddr.iAddr = sysCnfgInfo.redNetCnfgInfo.rmtDluIpAddrHex;	
	status = comPortInfoGet(RMTRESPS, &localAddr, peerAddr); 
	if(status) {
		setErrorCode (	__FILE__,
							__LINE__,
							"dualConfigDataGen",
							status
						);
		return(status);	
	}	
	
	
	sendData->socket_fd = peerAddr->sockFd;
	netMsgPtr = (strDualProtocolInfo *)sendData->data_buf;
	memoryClear ( (uint8 *)netMsgPtr , sizeof(strDualProtocolInfo));

	
	if(++sDualSendSeqNum > 0x7fff) sDualSendSeqNum = 1;

	netMsgPtr->sequenceID 	= sDualSendSeqNum;
	netMsgPtr->dualDataArea = (uint8)DUAL_INFO_AREA; 

	switch(optionCode)
	{
		
		case DUAL_CONF_READ:
			
			cmdCode= (DUAL_ACCESS_CMD<<16) | (DUAL_CONF_READ&0xffff);  
			netMsgPtr->messageType = (uint8)RMTREQST; 
			netMsgPtr->dataLength  = (uint16)DOUBLE_SIZE; 
			
			
			fourBytesDataCopy((uint32*)&netMsgPtr->dataBuff[0],&cmdCode,1);
		break;

		case DUAL_CONF_WRITE:
			
			cmdCode= (DUAL_ACCESS_CMD<<16) | (DUAL_CONF_WRITE&0xffff);  
			netMsgPtr->messageType = (uint8)RMTRESPS; 
			netMsgPtr->dataLength  = (uint16)sizeof(strSysDualInfo); 
			
			
			fourBytesDataCopy((uint32*)&netMsgPtr->dataBuff[0],&cmdCode,1);
			status = dualMsgDumpFromMyShelf(&dualInfo);
			
			
			systemRasInfoGet(RAS_REDUNDANCY_STATUS, &redundancyStatus);
			dualInfo.shelfRedundancyStatus  = redundancyStatus;
			
			fourBytesDataCopy(	(uint32*)&netMsgPtr->dataBuff[4],
								(uint32*)&dualInfo,	
								sizeof(strSysDualInfo)/4
							);	
		break;

		default:
			status = L_OPTION_CODE_ERR;
			setErrorCode (	__FILE__,
								__LINE__,
								"dualConfigDataGen",
								status
							);
			return(status);	
		break;
		
	}
	sendData->data_len   =   netMsgPtr->dataLength + MAX_DUAL_FRAME_HEAD_LEN;

	(void)netMsgChecksumCalcu   (   (uint32) netMsgPtr->dataLength,
                                    (uint32 *) netMsgPtr->dataBuff,
	                                (uint32 *) &checkSum
                                );
	netMsgPtr->checkSum = (uint16)checkSum;
	
	return(status);
}




uint32 dualCommandGen(strDualProtocolInfo	*msgPtr, uint32 dualCmd)
{
	uint32	status = NO_ERROR;
	strSysConfigInfo	sysCnfgInfo;
	uint32 cmdCode, checkSum;
	cmdCode= (DUAL_ACCESS_CMD<<16) | (DUAL_CMD_WRITE&0xffff);  

	memoryClear ( (uint8 *) &sysCnfgInfo, sizeof(strSysConfigInfo));
    (void)systemCnfgInfoRead( &sysCnfgInfo);


	if(++sDualSendSeqNum > 0x7fff) sDualSendSeqNum = 1;
	msgPtr->sequenceID =  sDualSendSeqNum;
	msgPtr->dualDataArea  = (uint8)DUAL_DEFAULT_AREA ;

	msgPtr->messageType = (uint8)RMTRESPS; 
	msgPtr->dataLength  = (uint16)DOUBLE_SIZE*2; 


	fourBytesDataCopy((uint32*)&msgPtr->dataBuff[0],&cmdCode,1);
	fourBytesDataCopy((uint32*)&msgPtr->dataBuff[4],&dualCmd,1);

	(void)netMsgChecksumCalcu   (   (uint32)msgPtr->dataLength,
                            (uint32 *) msgPtr->dataBuff,
	                        (uint32 *) &checkSum
                        );
	msgPtr->checkSum = (uint16)checkSum;

	return(status);

}


uint32 dualLiveCountGen(strDualProtocolInfo	*msgPtr)
{
	uint32	status = NO_ERROR;

	uint32 cmdCode,checkSum;
	uint32 myliveCount, myErrorSummary;
	uint32 rmtliveCount, rmtErrorSummary;
	strSysConfigInfo	sysCnfgInfo;
	strSysRunningConfigInfo	*sysModeInfoPtr 
						= &gSysRunningStatusInfo.sysRunningConfigInfo;

	uint32 sysDualStatus = sysModeInfoPtr->sysRedundancyStatus;
    sysDualStatus  = sysModeInfoPtr->sysRunningMode;
    sysDualStatus |= sysModeInfoPtr->sysRedundancyRunningStatus 	<< 2;
    sysDualStatus |= sysModeInfoPtr->sysRedundancyStatus 		<< 4;
    sysDualStatus |= sysModeInfoPtr->sysOperationMode			<< 6;
    sysDualStatus |= sysModeInfoPtr->sysShelfId 					<< 8;
	
	memoryClear ( (uint8 *) &sysCnfgInfo, sizeof(strSysConfigInfo));
    (void)systemCnfgInfoRead( &sysCnfgInfo);

	cmdCode= (DUAL_ACCESS_CMD<<16) | (DUAL_LIVE_COUNT&0xffff);  

	
	if(++sDualSendSeqNum > 0x7fff) sDualSendSeqNum= 1;
		
	msgPtr->sequenceID = sDualSendSeqNum;
	msgPtr->dualDataArea  = (uint8)DUAL_DEFAULT_AREA;

	msgPtr->messageType = (uint8)RMTCAST; 
	msgPtr->dataLength  = (uint16)6*DOUBLE_SIZE; 

	status = dualMsgReadFromMyShelf( DUAL_LIVE_CNT_ACC, &myliveCount);
	status = dualMsgReadFromMyShelf( DUAL_MY_ERR_SUMMARY_ACC, &myErrorSummary);
	status = dualMsgReadFromMyShelf( DUAL_RMT_LIVE_CNT_ACC, &rmtliveCount);

	status = dualMsgReadFromMyShelf( DUAL_RMT_ERR_SUMMARY_ACC, &rmtErrorSummary);

	
	fourBytesDataCopy((uint32*)&msgPtr->dataBuff[0],&cmdCode,1);
	fourBytesDataCopy((uint32*)&msgPtr->dataBuff[4],&myliveCount,1);
	fourBytesDataCopy((uint32*)&msgPtr->dataBuff[8],&myErrorSummary,1);
	fourBytesDataCopy((uint32*)&msgPtr->dataBuff[12],&rmtliveCount,1);
	fourBytesDataCopy((uint32*)&msgPtr->dataBuff[16],&rmtErrorSummary,1);
	
	fourBytesDataCopy((uint32*)&msgPtr->dataBuff[20], &sysDualStatus,1);

	(void)netMsgChecksumCalcu   (   (uint32)msgPtr->dataLength,
		                            (uint32 *) msgPtr->dataBuff,
			                        (uint32 *) &checkSum
		                        );
	msgPtr->checkSum = (uint16)checkSum;
	return(status);

}


uint32 redundancyDataSendandRecv( 	uint8 *recvAddrPtr, 
									uint8 *sendAddrPtr,
								 	uint32 sendSize, 
								 	uint32 maxIter 
								 )
{
	uint32	status = NO_ERROR;
	static struct  fd_set  tmpFds,readFds;
	strSysConfigInfo sysCnfgInfo;
	strinetAddr peerAddr;
    struct timeval timeOut;

	uint32 connectTime;
	strDataBuf recvData;
	uint32 *opCmd, sendIterFlag = 0;
	uint32 sendDataAddr, *recvDataPtr, calcuChecksum;
	strDualProtocolInfo *netPtr;

#if (BOARD_TYPE == LKV120_BOARD)
    int fd_max = 0;
#endif 

	
	systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);

	
	timeOut.tv_sec = 1; timeOut.tv_usec = 0;

	if(sysCnfgInfo.redNetCnfgInfo.ctrlNetDualStatus == BOARD_ENABLE) 
	{
		FD_ZERO( &readFds );
		FD_SET( gReadRespRecvSockFd, &readFds);

		connectTime = 0;
		while(connectTime <= maxIter) 
		{
			if(sendIterFlag == NO_ERROR)
				status = myRmtDataAcc(	__FILE__,
										__LINE__, 
										BUS_LONG_ACC,	
										BUS_DATA_READ ,
										(uint8 *)sendAddrPtr , 	
										(uint8 *)&sendSize 
									 );
			else 
				sendIterFlag = 0;

			if(status != NO_ERROR) 
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);

			
			tmpFds =   readFds;

#if (BOARD_TYPE == LKV120_BOARD)
		fd_max = 0;
		{
            if(FD_ISSET(gReadRespRecvSockFd, &tmpFds))
			{
				if(gReadRespRecvSockFd>fd_max) {
	        		fd_max = gReadRespRecvSockFd;
				}
			}
		}
#endif 
#if (BOARD_TYPE == LKV120_BOARD)
			status = inOutMultiplex(  fd_max + 1,
									  &tmpFds,
									  NULL,
									  NULL,
									  (void *)&timeOut
								   );
#else
			status = inOutMultiplex(  MAX_SOCKET_FD_NUM, 
									  &tmpFds,
									  NULL,
									  NULL,
									  (void *)&timeOut
								   );
#endif

			if(status   == NO_ERROR) {
				if(FD_ISSET(gReadRespRecvSockFd,   &tmpFds)) {
					recvData.socket_fd = gReadRespRecvSockFd;
					
					status = dataRecvFromServerOrClient( &recvData,
														 (void *)   &peerAddr,
														 UDP_PROTO
													   );
					if(status == NO_ERROR)	{
						
						sendDataAddr = (uint32)sendAddrPtr;
						recvDataPtr = (uint32*)&recvData.data_buf[DUAL_FRAME_FB_EXE_INDEX];

						opCmd = (uint32*)&recvData.data_buf[MAX_DUAL_FRAME_HEAD_LEN];
						if( ( *opCmd != ((DUAL_ACCESS_CMD<<16) | (DUAL_MEMORY_COPY&0xffff))
							||(sendDataAddr != *recvDataPtr)
							) 
						) {
							sendIterFlag = 1;
							continue;
						}
						
						
						netPtr = (strDualProtocolInfo	*)recvData.data_buf;
						(void)netMsgChecksumCalcu(	netPtr->dataLength,
													(uint32*)  netPtr->dataBuff,
													(uint32 *) &calcuChecksum
												 );
						if(netPtr->checkSum != (uint16)calcuChecksum)
							continue; 

						
						status = rmtFbDataCopyRespFunc((strDualProtocolInfo *)recvData.data_buf, NULL );

						
						if(status == NO_ERROR) 	{
							memoryCopy	(	(uint8 *)recvAddrPtr, 
											(uint8*)&recvData.data_buf[DUAL_FRAME_FB_EXE_INDEX] , 
											(recvData.data_len-DUAL_FRAME_FB_EXE_INDEX)
									  );
							goto MODULE_END;
						}
						else
							setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
					}

					if(status != NO_ERROR) 
						setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				}
				else status = 1; 
			}
			connectTime++;
		}
MODULE_END:

		FD_CLR( gReadRespRecvSockFd, &readFds);
	}
	return (status);
}


uint32 redundancyDataNumGen(uint32 moduleId, uint32 *destPtr, uint32 *srcPtr )
{
	
	uint32 status = NO_ERROR;
	uint32 i, readSize, *readAddrPtr;
	strSysExecInfo *sysExeInfoPtr,sysExeInfo;
	strVariableExecInfo sysVarExeInfo;
	strMemPointerInfo	busMemPtr;
	uint32 readNum = 0;
	strSfcMemPointerInfo sfcMemPtr;
	strModuleInfo moduleInfo;
	uint8	recvData[MAX_DATA_LEN], *readDataPtr;

	strModuleExecInfo   sysMouleExeInfo;
	strfbtmModuleExecInfo  sysfbtmMouleExeInfo;

	
	readDataPtr = recvData;

	memoryClear( (uint8 *)&sysExeInfo, sizeof(strSysExecInfo));
	memoryClear( (uint8 *)&sysMouleExeInfo, sizeof(strModuleExecInfo));
	memoryClear( (uint8 *)&sysVarExeInfo, sizeof(strVariableExecInfo));
	memoryClear( (uint8 *)&sysfbtmMouleExeInfo, sizeof(strfbtmModuleExecInfo));

	
	
	status =  fbSysExeInfoPointerGet(&sysExeInfoPtr, RMT_MEM_ACC);
	if(status!=NO_ERROR){
		setErrorCode(__FILE__, __LINE__, "fbSysExeInfoPointerGet", status);
		return(status);
	}
	
	readAddrPtr = (uint32 *)(sysExeInfoPtr);
	readSize = sizeof(strSysExecInfo);

	status = redundancyDataSendandRecv(	(uint8 *)readDataPtr,
										(uint8 *)readAddrPtr, 
										readSize, 
										MAX_DUAL_EXE_ITER 
									);
	if(status!=NO_ERROR){
		printf("System Current Exe Info Read Err\n");
		return(status);
	}

	
	if(status == NO_ERROR) {
		
		memoryCopy((uint8*)&sysExeInfo , (uint8 *)&recvData[8], sizeof(strSysExecInfo) );
		
		if(moduleId == LOGIC_ID) {
			memoryCopy(	(uint8*)&sysMouleExeInfo,
						(uint8*)&sysExeInfo.mpuSysExeInfo,
						sizeof(strModuleExecInfo)
					  );
		}
		else {
			status = L_MODULE_ID_ERR;
			setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
			return(status);
		}

		status = moduleInfoRead(moduleId, &moduleInfo);
		if(status != NO_ERROR) {
			setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
			return(status);
		}
		
		if(
			(sysMouleExeInfo.curMaxSegmentId >	moduleInfo.maxSegmentNum) ||
			(sysMouleExeInfo.curfbMaxBlockAddr > moduleInfo.maxFbBlockAddr) ||
			(sysMouleExeInfo.curfbMaxParaIndex > moduleInfo.maxFbParaNum) ||
			(sysMouleExeInfo.curfbMaxOutIndex > moduleInfo.maxFbOutputNum)
		  ){
			status = L_EXEINFO_EXCEED_ERR;
			printf("[ERROR] - sysModuleExeInfo Exceed Err !!\n");

			printf("* moduleId = 0x%x \n",moduleId);
			printf(" CUR SEG NUM = %d, MAX SEG NUM = %d \n",
				sysMouleExeInfo.curMaxSegmentId, moduleInfo.maxSegmentNum );
			printf(" CUR FB ADDR = %d, MAX FB ADDR = %d \n",
				sysMouleExeInfo.curfbMaxBlockAddr, moduleInfo.maxFbBlockAddr );
			printf(" CUR PARA INDEX = %d, MAX PARA INDEX = %d \n",
				sysMouleExeInfo.curfbMaxParaIndex, moduleInfo.maxFbParaNum );
			printf(" CUR OUTPUT INDEX = %d, MAX OUTPUT INDEX = %d \n",
				sysMouleExeInfo.curfbMaxOutIndex, moduleInfo.maxFbOutputNum );
			
			return(status);
		}

	}	
		
	
	(void)busFbMemPointerGet(moduleId,(strMemPointerInfo*)&busMemPtr);

	

	readAddrPtr = (uint32*)( (uint32)destPtr + ( (uint32)busMemPtr.segTblMemPtr - (uint32)srcPtr) );
	readSize = sizeof(strFbTaskTblMemInfo) * (sysMouleExeInfo.curMaxSegmentId % 15);

	if(readSize != 0)
	{
		status = redundancyDataSendandRecv(	(uint8 *)readDataPtr,
											(uint8 *)readAddrPtr, 
											readSize, 
											MAX_DUAL_DATA_ITER 
										);
		if(status!=NO_ERROR){
			setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
			return(status);
		}
	}
	
	
	for(i=0;i<sysMouleExeInfo.curMaxSegmentId/15;i++)
	{
		readAddrPtr = (uint32*)( (uint32)readAddrPtr + (uint32)readSize);
		readSize = sizeof(strFbTaskTblMemInfo) * 15;
		status = redundancyDataSendandRecv(	(uint8 *)readDataPtr,
											(uint8 *)readAddrPtr, 
											readSize,
											MAX_DUAL_DATA_ITER 
										);
		if(status!=NO_ERROR){
			setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
			return(status);
		}
	}

	if(status != NO_ERROR) return(status);

	
	
	
	if(sysMouleExeInfo.curfbMaxBlockAddr > 0) {
		readNum = sysMouleExeInfo.curfbMaxBlockAddr +1 ;
		readAddrPtr = (uint32*)( (uint32)destPtr + ( (uint32)busMemPtr.fbMemPtr - (uint32)srcPtr) );
		readSize = sizeof(strFbMemInfo) * ( readNum % 20);
		
		if(readSize != 0)
		{
			status = redundancyDataSendandRecv(	(uint8 *)readDataPtr,
												(uint8 *)readAddrPtr, 
												readSize,
												MAX_DUAL_DATA_ITER 
											);
	
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}

		
		for(i=0;i< (readNum /20) ;i++)
		{
			readAddrPtr = (uint32*)( (uint32)readAddrPtr + (uint32)readSize);
			readSize = sizeof(strFbMemInfo) *  20;
			status = redundancyDataSendandRecv(	(uint8 *)readDataPtr,
												(uint8 *)readAddrPtr, 
												readSize ,
												MAX_DUAL_DATA_ITER 
											);
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}

		if(status != NO_ERROR) return(status);
	}

	

	
	if(sysMouleExeInfo.curfbMaxParaIndex > 0) {
		readNum = sysMouleExeInfo.curfbMaxParaIndex +1 ;
		readAddrPtr = (uint32*)( (uint32)destPtr + ( (uint32)busMemPtr.paraMemPtr - (uint32)srcPtr) );
		readSize = DOUBLE_SIZE * (readNum % 300);

		if(readSize != 0)
		{
			status = redundancyDataSendandRecv(	(uint8 *)readDataPtr,
												(uint8 *)readAddrPtr, 
												readSize,
												MAX_DUAL_DATA_ITER 
											);
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}

		
		for(i=0;i < (readNum/300) ;i++)
		{
			readAddrPtr = (uint32*)( (uint32)readAddrPtr + (uint32)readSize);
			readSize = DOUBLE_SIZE * 300;
			status = redundancyDataSendandRecv(	(uint8 *)readDataPtr,
												(uint8 *)readAddrPtr, 
												readSize,
												MAX_DUAL_DATA_ITER 
											);
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}
		if(status != NO_ERROR) return(status);
	}

	
		if(sysMouleExeInfo.curfbMaxOutIndex > 0){
		readNum = sysMouleExeInfo.curfbMaxOutIndex + 1;

		readAddrPtr = (uint32*)( (uint32)destPtr + ( (uint32)busMemPtr.outputMemPtr - (uint32)srcPtr) );
		readSize = DOUBLE_SIZE * (readNum % 300);

		if(readSize != 0)
		{
			status = redundancyDataSendandRecv(	(uint8 *)readDataPtr,
												(uint8 *)readAddrPtr, 
												readSize,
												MAX_DUAL_DATA_ITER 
											);
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}
	
		
		for(i=0;i<(readNum/300);i++)
		{
			readAddrPtr = (uint32*)( (uint32)readAddrPtr + (uint32)readSize);
			readSize = DOUBLE_SIZE * 300;
			status = redundancyDataSendandRecv(	(uint8 *)readDataPtr,
												(uint8 *)readAddrPtr, 
												readSize,
												MAX_DUAL_DATA_ITER 
											);
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}
		if(status != NO_ERROR) return(status);
	}

	
	
	if(moduleId == LOGIC_ID) {
		
		readNum = MAX_MPU_RETAIN_NUM/32;
		readAddrPtr = (uint32*)( (uint32)destPtr + ( (uint32)busMemPtr.retainMemPtr - (uint32)srcPtr) );
		readSize = DOUBLE_SIZE * (readNum % 300);

		if(readSize != 0)
		{
			status = redundancyDataSendandRecv(	(uint8 *)readDataPtr,
												(uint8 *)readAddrPtr, 
												readSize,
												MAX_DUAL_DATA_ITER 
											  );
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}

		
		for(i=0;i<(readNum/300);i++)
		{
			readAddrPtr = (uint32*)( (uint32)readAddrPtr + (uint32)readSize);
			readSize = DOUBLE_SIZE * 300;
			status = redundancyDataSendandRecv(	(uint8 *)readDataPtr,
												(uint8 *)readAddrPtr, 
												readSize,
												MAX_DUAL_DATA_ITER 
											);
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}
		if(status != NO_ERROR) return(status);

		
		memoryCopy(	(uint8*)&sysVarExeInfo,
					(uint8*)&sysExeInfo.autovarSysExeInfo,
					sizeof(strVariableExecInfo)
				  );

		
		readNum = MAX_MPU_AUTOVAR_NUM; 

		readAddrPtr = (uint32*)( (uint32)destPtr + ( (uint32)busMemPtr.autoMemPtr - (uint32)srcPtr) );
		readSize = DOUBLE_SIZE * (readNum % 300);

		if(readSize != 0)
		{
			status = redundancyDataSendandRecv(	(uint8 *)readDataPtr,
												(uint8 *)readAddrPtr, 
												readSize,
												MAX_DUAL_DATA_ITER 
											  );
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}

		
		for(i=0;i<(readNum/300);i++)
		{
			readAddrPtr = (uint32*)( (uint32)readAddrPtr + (uint32)readSize);
			readSize = DOUBLE_SIZE * 300;
			status = redundancyDataSendandRecv(	(uint8 *)readDataPtr,
												(uint8 *)readAddrPtr, 
												readSize,
												MAX_DUAL_DATA_ITER 
											  );
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}

		if(status != NO_ERROR) return(status);

		
		memoryCopy(	(uint8*)&sysVarExeInfo,
					(uint8*)&sysExeInfo.flagvarSysExeInfo,
					sizeof(strVariableExecInfo)
				  );

		
		readNum = MAX_MPU_MMEM_NUM; 

		readAddrPtr = (uint32*)( (uint32)destPtr + ( (uint32)busMemPtr.flagMemPtr - (uint32)srcPtr) );
		readSize = DOUBLE_SIZE * (readNum % 300);

		if(readSize != 0)
		{
			status = redundancyDataSendandRecv(	(uint8 *)readDataPtr,
												(uint8 *)readAddrPtr, 
												readSize,
												MAX_DUAL_DATA_ITER 
											 );
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}

		
		for(i=0;i<(readNum/300);i++)
		{
			readAddrPtr = (uint32*)( (uint32)readAddrPtr + (uint32)readSize);
			readSize = DOUBLE_SIZE * 300;
			status = redundancyDataSendandRecv(	(uint8 *)readDataPtr,
												(uint8 *)readAddrPtr, 
												readSize,
												MAX_DUAL_DATA_ITER 
											 );
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}

		
		readNum = MAX_SHARED_MEM_NUM;

		readAddrPtr = (uint32*)( (uint32)destPtr + ( (uint32)busMemPtr.stSharedMemPtr - (uint32)srcPtr) );
		readSize = DOUBLE_SIZE * (readNum % 300);

		if(readSize != 0)
		{
			status = redundancyDataSendandRecv(	(uint8 *)readDataPtr,
												(uint8 *)readAddrPtr, 
												readSize,
												MAX_DUAL_DATA_ITER 
											 );
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}

		
		for(i=0;i<(readNum/300);i++)
		{
			readAddrPtr = (uint32*)( (uint32)readAddrPtr + (uint32)readSize);
			readSize = DOUBLE_SIZE * 300;

			status = redundancyDataSendandRecv((uint8 *)readDataPtr,
												(uint8 *)readAddrPtr, 
												readSize,
												MAX_DUAL_DATA_ITER 
											  );
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}
	}

	
	if(moduleId == LOGIC_ID) {
		memoryClear( (uint8 *)&sfcMemPtr, sizeof(strSfcMemPointerInfo));
		status = sfcMemPointerGet(BUS_MEM_ACC, &sfcMemPtr);

		
		readNum = sysExeInfo.sfcSysExeInfo.curMaxStepId;
		readAddrPtr = (uint32*)( (uint32)destPtr + ( (uint32)sfcMemPtr.stepMemPtr - (uint32)srcPtr) );
		readSize = sizeof(strStepInfo) * (readNum % 5);  
		if(readSize != 0)
		{
			status = redundancyDataSendandRecv((uint8 *)readDataPtr,
										(uint8 *)readAddrPtr, readSize,MAX_DUAL_DATA_ITER );
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}

		for(i=0;i<(readNum/5);i++)
		{
			readAddrPtr = (uint32*)( (uint32)readAddrPtr + (uint32)readSize);
			readSize =  sizeof(strStepInfo) *5 ;
			status = redundancyDataSendandRecv((uint8 *)readDataPtr,
										(uint8 *)readAddrPtr, readSize,MAX_DUAL_DATA_ITER );
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}

		
		readNum = sysExeInfo.sfcSysExeInfo.curMaxTrsId;
		readAddrPtr = (uint32*)( (uint32)destPtr + ( (uint32)sfcMemPtr.trsMemPtr - (uint32)srcPtr) );
		readSize = sizeof(strTransitionInfo) * (readNum % 15);  
		if(readSize != 0)
		{
			status = redundancyDataSendandRecv((uint8 *)readDataPtr,
										(uint8 *)readAddrPtr, readSize,MAX_DUAL_DATA_ITER );
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}

		for(i=0;i<(readNum/15);i++)
		{
			readAddrPtr = (uint32*)( (uint32)readAddrPtr + (uint32)readSize);
			readSize =  sizeof(strTransitionInfo) *15 ;
			status = redundancyDataSendandRecv((uint8 *)readDataPtr,
										(uint8 *)readAddrPtr, readSize,MAX_DUAL_DATA_ITER );
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}

		
		readNum = sysExeInfo.sfcSysExeInfo.curMaxActId;
		readAddrPtr = (uint32*)( (uint32)destPtr + ( (uint32)sfcMemPtr.actMemPtr - (uint32)srcPtr) );
		readSize = sizeof(strActionInfo) * (readNum % 50);  
		if(readSize != 0)
		{
			status = redundancyDataSendandRecv((uint8 *)readDataPtr,
										(uint8 *)readAddrPtr, readSize ,MAX_DUAL_DATA_ITER);
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}

		for(i=0;i<(readNum/50);i++)
		{
			readAddrPtr = (uint32*)( (uint32)readAddrPtr + (uint32)readSize);
			readSize =  sizeof(strActionInfo) *50 ;
			status = redundancyDataSendandRecv((uint8 *)readDataPtr,
										(uint8 *)readAddrPtr, readSize,MAX_DUAL_DATA_ITER );
			if(status!=NO_ERROR){
				setErrorCode(__FILE__, __LINE__, "redundancyDataSendandRecv", status);
				return(status);
			}
		}
		
		if(status != NO_ERROR) 
			return(status);
	}

	return(status);
}


uint32 rmtShelfPowerOffCmd (uint8 *fileName, uint32 lineNum, uint32 dualCmd) 
{
	uint32 status = NO_ERROR;

	
	
	if( ( dualCmd != RMT_RESET_CMD) && (dualCmd != RMT_POWER_OFF_CMD) )
		return(status);

	status = dualMsgWriteToRmtShelf(DUAL_CMD_ACC, dualCmd);

	if(status != NO_ERROR) {
		setErrorCode (	__FILE__,__LINE__,
                            "rmtShelfPowerOffCmd",status
						);
		logMessageSet(__FILE__,__LINE__,"rmtShelfPowerOffCmd",STBY_RESET_REQ_WRITE_ERR,0);
		return(status);
    }

	
	if(status == NO_ERROR) {
		logMessageSet(fileName,lineNum,"rmtShelfPowerOffCmd",RMT_SHELF_POWER_OFF,0);
		status = dualCmdGenAndMsgSend (dualCmd);
	}
    return(status);
}
#endif
