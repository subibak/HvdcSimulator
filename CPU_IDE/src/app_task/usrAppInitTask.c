
#include	<stdio.h>
#include	<string.h>
#include	"usrTypes.h"
#include	"sysDefs.h"
#include	"segScheduler.h"
#include	"pcsDefs.h"
#include	"systemDiag.h"
#include	"sysConfig.h"
#include	"fbMem.h"

#include	"memDef.h"
#include	"lkv120Board.h"
#include	"usrErrCode.h"
#include	"sysDual.h"
#include	"network.h"
#include	"fbForcing.h"

#include	"vmeDef.h"


#if (BOARD_TYPE == MVME3100_BOARD)
#include	<..\config\mpc8540\config.h>
#include	<..\config\mpc8540\sysEpic.h>
#include	<..\config\mpc8540\sbc8540A.h>
#endif


#include	"pmsUtil.h"

#include    "usrQueData.h"
#include    "msgQue.h"
#include    "msgQueData.h"
#include	"debugInfo.h"

int32	gDebugPortFd = 0;
uint32	gNetLineCnfg = CTRL_NET_LINE_SINGLE;

uint32	gIntrAccFlag = INTR_ACC_DISABLE;

uint32 gEthDluModuleID = 0; 


extern strMpuDiuRunningStatusInfo *gMpuRasInfoPtr; 
extern strMpuDiuRunningStatusInfo *gDiuRasInfoPtr; 


extern uint32	gDebugErrStringFlag;


extern strBaseShelfIoInfo gBio;


extern void *gAutoVariableLocalPtr;
extern uint32 SCHD_TIME_BASE;

extern uint8	*gMmemVariableBusPtr,*gBusMpuFbStartAddr,*gSTSharedMemDataBusPtr;
extern uint32	M_MEMORY_AREA_START_OFFSET, M_MEMORY_AREA_MAX_SIZE;
extern uint32	S_MEMORY_AREA_START_OFFSET, S_MEMORY_AREA_MAX_SIZE;


void usrInitTask(int32,int32,int32,int32,int32,int32,int32,int32,int32,int32);
static void		systemVariableInit (void);
static uint32	usrQueueInit (void);
static uint32	fbCodeVariableInit (void);
static uint32	taskVariableInit (void);

extern void memoryClear(uint8 *, uint32);
extern uint32 appTaskCreate(uint8 *);
extern void   memoryCopy(uint8 *,uint8 *,uint32);
extern void   systemInitErr(uint8 *,uint32,uint8 *,uint32);
extern uint32 debugConsolePortInit(int32 *);
extern void   bootParaSequence (strSystemBootParaInfo *);
extern uint32 networkDeviceInit(uint32 *, strUserBootParaInfo *);
extern uint32 routeTblInfoUpdate(uint32, strRouteTblInfo *);
extern uint32   systemCnfgInfoSet(strSystemBootParaInfo *);
extern uint32 fbMemoryAddrInit(uint32);
extern uint32 segmentBusDataChecksumCheck(uint32);

extern uint32 getFbTaskTblInfo(uint32,uint32, strFbTaskTblMemInfo *);
extern void   usrTaskDelete(uint32);
extern void	  systemRunningStatusInfoInit(void);
extern uint32 dLMsgMngrInit(void);
extern uint32 debugQueueInit(void);
extern uint32 fbDbTableSorting(void);
extern uint32 fbDbTableTypeInfoInit(void);
extern uint32 netCommTaskVarInit(void);
extern uint32 rasTaskVarInit(void);
extern void   debugVarInit(void);
extern uint32 usrTaskSyncVarInit(void);
extern uint32 schedulerModeChange(uint32 , uint32	);
extern uint32   segSchedulerInfoInit(uint32);
extern void   systemRasInfoSet(uint32 , uint32 );
extern uint32 fatalErrorLogInit(void);
extern void   logMessageSet (int8 *, uint32, int8 *, uint32,uint32 );
extern void   fbMemoryClearCheck(void);
extern uint32 busMemoryDataCopyToLocalMem(uint32);
extern uint32 tmtcSerialDeviceOpen(uint32, uint32 );
extern uint32 tmtcInit(void);
extern void   boardExceptionHaFBC_801_BOARDndlerSet(void);
#ifdef VXWORKS
extern uint32 systemIntrSet(void);
#endif
extern uint32 sysRedundancyVarInit(void);
extern uint32 dualMsgWriteToMyShelf(uint32, uint32);
extern uint32 sysRedundancyVarSet(strUserBootParaInfo *);
extern uint32 stringCompareWithLen( uint8 *, uint8 *, uint32  );
extern void	  sysTaskDelay (int32 );
extern void   *memoryAlloc(int32 );
extern uint32 myBusDataAcc(int8 *, uint32, uint32	, uint32 ,int8 *, int8 *);

extern uint32 sysDualLinkBoardInit(void);
extern uint32 sysDualLinkRmtBoardChk(void);

extern void   sysRmtIpAddrSetToSysConfigMemory(uint32, uint32 );

extern uint32 systemCnfgInfoRead(strSysConfigInfo *);

extern uint32 execSequenceVarInit(uint32);
extern uint32 allSegmentModeErrSet(uint32 );

extern uint32 onlineDownloadVarInit();

extern void setErrorCode(uint8 *,uint32,uint8 *,uint32);

extern uint32 fbMemoryDef(uint32);
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
extern uint32 busVarInitDataCopyTobusVarMem(uint32);
#endif
extern uint32	fmcBoardInit(void);

extern void mpuDiufbSimFbNumSetInDualInfo (void);
extern int32	systemClockRateGet(void);

extern uint32	dualChkAppTaskCreate(uint8 *);
extern uint32	rmtShelfCheckByCtrlNetCom (uint32, uint32 *, uint32);
extern uint32    rmtShelfCheckCommConnectInit(void);
extern void	ipAddrStrToHex (uint32 *, uint8 *);

#if ((BOARD_TYPE == LKV120_BOARD))
extern uint32 sysEtsecSet(uint32,uint32,uint32);
#endif 
	
extern strSysExecInfo *gSysExeInfoLocalPtr, *gSysExeInfoBusPtr;

#if (BOARD_TYPE == LKV120_BOARD)
    extern uint32 sysLkp410Detect (void);
    extern uint32 sysLkp420Detect (void);
    extern uint32 sysLkp414Detect (void);

    extern void ledOn(int led);
    extern void ledOff(int led);
#endif

    
#ifdef _WIN32
#define	ETH_INIT_ERR_ALARM_BELL		(0x07)
#endif

unnCpuRasInfo gunHvdcCpuRasInfo;

#if 0
LOCAL void ext_timer_isr(void)
{
#define	ET_CTRL_ENABLE_COUNTER		(0x1)
#define	ET_CTRL_CLEAR_COUNTER		(0x2)
#define	ET_CTRL_CLEAR_OVERFLOW		(0x4)
#define	ET_CTRL_ENABLE_INTERRUPT	(0x100)
#define ET_CTRL_CLEAR_INTERRUPT		(0x200)
#define ET_CTRL_PEND_INTERRUPT		(0x400)

	if((*(uint32*)BRD_TICK_TIMER_1_CTRL_REG)&ET_CTRL_PEND_INTERRUPT) {
		
		
		*(uint32*)(MEMORY_BOARD_BUS_ADDR + MEM613BOARD_END_OFFSET - 3) = 0x11111111;
		
		*(uint32*)BRD_TICK_TIMER_1_CTRL_REG |= ET_CTRL_CLEAR_INTERRUPT;
	}
}
#endif

void usrInitTask(int32 arg1,int32 arg2,int32 arg3,int32 arg4,int32 arg5,
				 int32 arg6,int32 arg7,int32 arg8,int32 arg9,int32 arg10)
{
	uint32 status = NO_ERROR;
	uint8 taskName[32];
	strSystemBootParaInfo	bootPara;
	uint32  ioShelfIndex, portFd;
	uint32	chanNum = 0;
	uint32	rmtShelfStatus = REMOTE_SHELF_DISCONNECT;
	uint32  sysRunStatus;
    uint32  sysRunMode;
    uint32  sysShelfRunStatus;
	uint32	sysRedundancyRunStatus, comRetryCnt = 0;
	uint32	sysRedundancyErrStatus, rmtSysRedundancyRunStatus;
	strSysDualInfo dualCmdInfo;
	uint32	*ioAddrPtr = NULL;

	uint32  localDluBrdProbeStatus;

	uint32 rmtTargetIpAddr = 0;
	uint32 rmtModuleIpAddr = 0;
	uint32 ioInitNeed = 0, profiShelfIndex;
	uint32 memTestLoop = 0;

	status = debugConsolePortInit((int32 *)&portFd);
	if(status != NO_ERROR) 
	{
		systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, status);
	}
	else {
    	gDebugPortFd = portFd;
		printf( "\n");
	}
   	
	memoryClear( (uint8 *)&bootPara, sizeof(strSystemBootParaInfo));
	bootParaSequence ((strSystemBootParaInfo *)&bootPara);
	printf( "Board Boot Sequence SUCCESS !!!\n");
	
#if (BOARD_TYPE == LKV120_BOARD)

    
    if(sysLkp414Detect() == NO_ERROR)
    {
        printf("LKP-414 detected...\n");
        gEthDluModuleID = USING_LKP414_RTG; 
    }
    else if(sysLkp410Detect() == NO_ERROR)
    {
        printf("LKP-410 detected...\n");
        gEthDluModuleID = USING_LKP410_GEI; 
    }
    else if(sysLkp420Detect() == NO_ERROR)
    {
        printf("LKP-420 detected...\n");
        gEthDluModuleID = USING_LKP420_GEI;
    }
    
    if(     (bootPara.usrBootParaInfo.dluIfKind == DLU_IF_PMC_ETH)
        &&  (gEthDluModuleID == 0)
    ) {
        ledOn(4);  
		systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, DLU_INSTALL_ERR);
	}
#endif

   	
#ifdef VXWORKS
	status = systemIntrSet();
    if(status != NO_ERROR)
		systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, status);
#endif

#ifdef VXWORKS
    memoryClear((uint8*)&gunHvdcCpuRasInfo, sizeof(unnCpuRasInfo));

#if (BOARD_TYPE == LKV120_BOARD)

	/********************************************************************************** 
	* 			PMC502(광통신) 모듈 초기화 
	**********************************************************************************/
	if(Lkp502Init() == NO_ERROR)
	{
		printf("Optical Com PMC Module Init Success !!!\n");
	}
	else
	{
		printf("Optical Com PMC Module Init Error !!!\n");		
	}
	
	/* Initialize Link Communication Library */
	initLpEnvData ();

	/********************************************************************************** 
	* 			BUS 상에 있는 VME 보드 초기화
	**********************************************************************************/
	vmeBoardInit();	
	
#endif
#endif
	
	if(bootPara.usrBootParaInfo.memoryBoardType == MEM_BOARD_NONE)
	{
		MEMORY_BOARD_BUS_EMUL_ADDR = (uint8 *)malloc(MEM613BOARD_END_OFFSET+1);
		memoryClear( (uint8 *)MEMORY_BOARD_BUS_EMUL_ADDR, (MEM613BOARD_END_OFFSET+1));
	}
#if (BOARD_TYPE == LKV120_BOARD)
	else if(bootPara.usrBootParaInfo.memoryBoardType == MEM_4MB_MRAM)
	{
		MEMORY_BOARD_BUS_EMUL_ADDR = (uint8 *)MRAM_START_ADDR;
	}
#endif
   	
	status =  fbMemoryDef (bootPara.usrBootParaInfo.memoryBoardType);
    if(status != NO_ERROR)
		systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, status);
	else 
		printf( "FB Memory MAP Table INIT SUCCESS !!!\n");

    status = fbMemoryAddrInit(LOGIC_ID);
    if(status != NO_ERROR) 
		systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, status);
	else
		printf("FB Memory Address Init SUCCESS !!!\n");

	systemVariableInit();
	printf( "System Variables Initialized!\n");
  	
#ifdef VXWORKS
#ifdef INCLUDE_NET_INIT_IN_USR_APP 
    status = networkDeviceInit(	(uint32 *)&gNetLineCnfg, 
								(strUserBootParaInfo *)&bootPara.usrBootParaInfo
					 		   );
    if(status != NO_ERROR) 
    {
        printf( "\n\n**********************WARNING**************************\n");
        printf( "   Warning : Not Installed Dual Network Module !!!!\n");
        printf( "   Warning : Only 1 Line is Valid !!!!");
        printf( "\n*********************************************************\n\n");
    }
    else 
    {
        printf("Dual Network Line is Valid !!!\n");
    }
#endif
#endif

    routeTblInfoUpdate (
		bootPara.usrBootParaInfo.routeIndex, 
		(strRouteTblInfo *) &bootPara.usrBootParaInfo.routeTbl[0]
						); 

	
    status = systemCnfgInfoSet((strSystemBootParaInfo *) &bootPara);
    if(status != NO_ERROR) 
		systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, status);
	else 
    	printf( "System Configure Set SUCCESS !!!\n");

    debugVarInit();

	fbMemoryClearCheck();

    status = usrQueueInit();
    if(status != NO_ERROR) 
		systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, status);
	else 
    	printf( "USR Queue Init SUCCESS !!!\n");

	
	status = fbCodeVariableInit();
    if(status != NO_ERROR) 
		systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, status);
	else 
    	printf( "FB Code Variabe Init SUCCESS !!!\n");

    status = fatalErrorLogInit();
    if(status) 
    {
		systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, status);
	}

	status = sysRedundancyVarInit();
    if(status != NO_ERROR)
		systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, status);
	else
    	printf( "System Redundancy Variabe Init SUCCESS !!!\n");

   	status = sysRedundancyVarSet(&bootPara.usrBootParaInfo);
    if(status != NO_ERROR)
		systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, status);
	else
    	printf( "System Redundancy Variable Set SUCCESS !!!\n");
	
    if(bootPara.usrBootParaInfo.shelfType == SLAVE_SHELF) {
    	sysTaskDelay(systemClockRateGet() * 5); 
	}

  	
	status = rmtShelfCheckCommConnectInit();
    if(status)
    {
		systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, status);
    }
		
	rmtTargetIpAddr = 0;
	ipAddrStrToHex(&rmtTargetIpAddr, bootPara.usrBootParaInfo.rmtTargetIpAddr);
	
	rmtModuleIpAddr = 0;
	ipAddrStrToHex(&rmtModuleIpAddr, bootPara.usrBootParaInfo.rmtModuleIpAddr);

	sysRmtIpAddrSetToSysConfigMemory(rmtTargetIpAddr, rmtModuleIpAddr);

	
    status = dualChkAppTaskCreate( (uint8 *) &taskName[0]);

    if(status) 
		systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, status);
	
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
#if (defined(VXWORKS))
    #if (BOARD_TYPE == LKV120_BOARD)
        if(gEthDluModuleID != 0)
        {
            localDluBrdProbeStatus = NO_ERROR;
        }
        else
        {
            localDluBrdProbeStatus = 1;
        }
    #else
	    localDluBrdProbeStatus = NO_ERROR;
	#endif
#else
	localDluBrdProbeStatus = NO_ERROR;
#endif
#endif 

    if(localDluBrdProbeStatus == NO_ERROR) 
    {
		
		status = sysDualLinkBoardInit();
	    if(status != NO_ERROR)
			systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, status);
		else
	    	printf( "System Dual Link Unit Init SUCCESS !!!\n");

    	
		rmtShelfStatus = sysDualLinkRmtBoardChk();
		
		if(rmtShelfStatus == REMOTE_SHELF_CONNECT)
        	printf( "Remote Dual Link Unit  is Installed !!\n");
        else 
        {
        	printf( "Remote Dual Link Unit  is Not Installed !!\n");        	
        }
	}
    else
    {
		rmtShelfStatus = REMOTE_SHELF_DISCONNECT;
        printf( "Dual Link Unit Not Installed !!\n");
    }
	
	
	sysTaskDelay(systemClockRateGet()/2);
	gIntrAccFlag = INTR_ACC_ENABLE;
		
	if(rmtShelfStatus == REMOTE_SHELF_CONNECT) 
	{ 
	    	
READ_REMOTE_DUAL_CONFIG:
    	memoryClear ( (uint8 *)&dualCmdInfo, sizeof(strSysDualInfo));
		
		sysRmtIpAddrSetToSysConfigMemory(dualCmdInfo.myIpAddr_A, dualCmdInfo.myIpAddr_B);
		
    	if(	(status == NO_ERROR) &&
			(	(dualCmdInfo.shelfMode == SYS_RAS_SYSTEM_MANUAL_MODE)||
            	(dualCmdInfo.shelfMode == SYS_RAS_SYSTEM_AUTO_MODE)
			)
		)
	        sysRunMode		= dualCmdInfo.shelfMode;
		else
   	        sysRunMode		= SYS_RAS_SYSTEM_AUTO_MODE;

        sysRunStatus 	= SYS_RAS_SYSTEM_STOP;

        if(bootPara.usrBootParaInfo.shelfType == MASTER_SHELF)
        	sysShelfRunStatus = SYS_RAS_MASTER_RUN; 
		else
        	sysShelfRunStatus = SYS_RAS_SLAVE_RUN;

		sysRedundancyRunStatus 	= SYS_RAS_REDUNDANCY_STANDBY; 
		sysRedundancyErrStatus	= NO_ERROR;
   }
   else 
   {	
		sysRedundancyRunStatus 		= 0;
		rmtSysRedundancyRunStatus 	= 0;
		
		rmtShelfStatus = rmtShelfCheckByCtrlNetCom(	RMT_SHELF_LIVE_CHK_CMD,
													&rmtSysRedundancyRunStatus,
												  	1000 
												  );

   		if(rmtShelfStatus == NO_ERROR){ 
			printf("Remote Shelf's Status is received.. \n");

			if(rmtSysRedundancyRunStatus == SYS_RAS_REDUNDANCY_RUN){
				printf("Remote Shelf is RUN.. So Local Shelf is STANDBY !!!\n");
				sysRedundancyRunStatus 	= SYS_RAS_REDUNDANCY_STANDBY;
				
				comRetryCnt = 0;
COM_RETRY:
				
				rmtShelfStatus = rmtShelfCheckByCtrlNetCom(	RMT_SHELF_CHK_ENABLE_CMD,
															&rmtSysRedundancyRunStatus,
												  			1000 
												  			);
				if(rmtShelfStatus != NO_ERROR) {
					if(comRetryCnt++ > 2) {
						printf("[ERROR]-RMT_SHELF_CHK_ENABLE_CMD receive Error !!\n");
					}
					else
						goto COM_RETRY;					
				}
			} 
			else if(rmtSysRedundancyRunStatus == SYS_RAS_REDUNDANCY_STANDBY){
				printf("Remote Shelf is STANDBY.. So Local Shelf is RUN !!!\n");
				sysRedundancyRunStatus 	= SYS_RAS_REDUNDANCY_RUN; 
			}
			else {
				printf("[ERROR]- Unknown Remote Shelf Status[rmt Status : %d]\n",rmtSysRedundancyRunStatus); 
				goto COM_RETRY;
			}
		}
   		else 
   		{
			printf("Remote Shelf's Status is not received.. So Local Shelf is RUN !!!\n");
			sysRedundancyRunStatus 	= SYS_RAS_REDUNDANCY_RUN; 
   		}

        sysRunStatus 	= SYS_RAS_SYSTEM_RUN;
        sysRunMode		= SYS_RAS_SYSTEM_AUTO_MODE;

        if(bootPara.usrBootParaInfo.shelfType == MASTER_SHELF)
        	sysShelfRunStatus = SYS_RAS_MASTER_RUN; 
		else
        	sysShelfRunStatus = SYS_RAS_SLAVE_RUN;

		sysRedundancyErrStatus	= SYS_REDUNDANCY_ERR;
    }
   	
   	
    systemRasInfoSet(RAS_SYSTEM_STATUS, sysRunStatus);

    systemRasInfoSet(RAS_SYSTEM_MODE, sysRunMode);
    systemRasInfoSet(RAS_SHELF_STATUS, sysShelfRunStatus);
    systemRasInfoSet(RAS_REDUNDANCY_STATUS, sysRedundancyRunStatus);
    systemRasInfoSet(RAS_REDUNDANCY_ERR_STATUS, sysRedundancyErrStatus);

   	
	if(bootPara.usrBootParaInfo.memoryBoardType != MEM_BOARD_NONE) 
	{
		status = segmentBusDataChecksumCheck(LOGIC_ID);
	    if(status == APP_SEG_CHECKSUM_ERR) 
			systemRasInfoSet(RAS_MPU_CHECKSUM_STATUS, SEG_CHECKSUM_ERR);

		status = busMemoryDataCopyToLocalMem(MPU_DIU_DUAL_ACC);
	    if(status != NO_ERROR)
			systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, status);
	}

#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
    
	status = busVarInitDataCopyTobusVarMem(sysRedundancyErrStatus);
    if(status != NO_ERROR)
		systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, status);
#endif

	status = execSequenceVarInit(LOGIC_ID);
  	if(status != NO_ERROR)
  	{
		systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, status);
	}
	printf( "FB Execution Sequence ID Info Init SUCCESS !!!\n");

	
	status = segSchedulerInfoInit(MPU_DIU_DUAL_ACC);
    if(status) 
		systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, status);
	
	status = taskVariableInit(); 
    if(status)
		systemInitErr	(	__FILE__, __LINE__, __FUNCTION__, status);

	printf( "Task Varialbles Initialized!!!\n");

	logMessageSet(__FILE__, __LINE__,"INIT", BOARD_POWER_ON_RESET, 0);

	
	if(	sysRedundancyRunStatus 	== SYS_RAS_REDUNDANCY_RUN) 
		logMessageSet(__FILE__, __LINE__,"INIT", LOCAL_RUN, 0);
	else
		logMessageSet(__FILE__, __LINE__,"INIT", LOCAL_STANDBY, 0);	


	mpuDiufbSimFbNumSetInDualInfo ();

	
    memoryClear( (uint8 *) &taskName[0], 32);

    memoryClear( (uint8 *) &gBio, sizeof(strBaseShelfIoInfo));

	status = appTaskCreate( (uint8 *) &taskName[0]);

    if(status) 
		systemInitErr	(	__FILE__, __LINE__,(uint8 *)&taskName[0],status);

	schedulerModeChange(LOGIC_ID, SCHEDULER_ENABLE);
	
	printf("\n");
	printf("***********************************************\n");
    printf("*******  SYSTEM BOOTING TASK END !!!!   *******\n");
	printf("***********************************************\n");
	
	usrTaskDelete(0);

    return;
}

static void systemVariableInit(void)
{

	
	systemRunningStatusInfoInit();

	
	
	return;
}


static uint32 usrQueueInit(void)
{
	uint32 status = NO_ERROR;
   
    status = dLMsgMngrInit();
    if(status != NO_ERROR) return(status);

    
    status = debugQueueInit();
    if(status != NO_ERROR) return(status);

    return(status);
}


static uint32 fbCodeVariableInit(void)
{
	uint32	status = NO_ERROR;

    
	status = fbDbTableSorting();

	if(status) return(status);

    
    
	status = fbDbTableTypeInfoInit();

    return(status);
}


static uint32 taskVariableInit(void)
{
	uint32	status = NO_ERROR;
 	
    status = netCommTaskVarInit();
    if(status != NO_ERROR) {
		printf( "netCommTaskVarInit Error !!\n");
 		return(status);
	}   
	
	
    status = rasTaskVarInit();
    if(status != NO_ERROR) {
		printf( "rasTaskVarInit Error !!\n");
 		return(status);
	} 

    status = usrTaskSyncVarInit();
    if(status != NO_ERROR) {
		printf( "usrTaskSyncVarInit Error !!\n");
 		return(status);
	}

    status = onlineDownloadVarInit();
    if(status != NO_ERROR) {
		printf( "onlineDownloadVarInit Error !!\n");
 		return(status);
	}
	return(status);
}

#ifdef _WIN32
static uint32  winVariableInit(void)
{
    uint32  status = NO_ERROR;
	int16 wVersionRequested;
	WSADATA wsaData;

	int8	HI_BYTE_WINSOCK_VER	= 0x2;
	int8	LO_BYTE_WINSOCK_VER	= 0x2;
	 
	wVersionRequested = WORD_MAKE(HI_BYTE_WINSOCK_VER, LO_BYTE_WINSOCK_VER);

	
	if(WSAStartup( wVersionRequested, &wsaData ) != 0) {
		status = WSASTARTUP_CALL_ERR;
        printf("WSAtartup Call Err !!\n");
		
		printf("The version of the Windows Sockets specification "); 
		printf("that the WS2_32.DLL expects the caller ");
		printf("to use[0x%x].\n",wsaData.wVersion);

		printf("The highest version of the Windows Sockets ");
		printf("specification that this DLL can support[0x%x]",wsaData.wHighVersion);
	}
	
	

	else if (	GET_LOBYTE( wsaData.wVersion ) != LO_BYTE_WINSOCK_VER ||
		        GET_HIBYTE( wsaData.wVersion ) != HI_BYTE_WINSOCK_VER 
			) {
		status = WSASTARTUP_CALL_ERR;
        printf("WinSock DLL Version Err\n");
		WSACleanup( );
	}
    return(status);
}
#endif 
