
#include	<stdio.h>
#include	<string.h>
#include	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include	"segScheduler.h"
#include	"network.h"
#include	"usrQueData.h"
#include	"msgQueData.h"
#include	"usrSema.h"


#include	"sysConfig.h"
#include	"fbMem.h"
#include	"debugInfo.h"
#include	"osDepend.h"
#include	"taskDefs.h"
#include	"lkv120Board.h"
#include	"vmeDef.h"
#include	"usrErrCode.h"

#include	"sysDual.h"

#include	"netProtocol.h"
#include	"protocolBuf.h"

#include	"systemDiag.h"

#ifdef VXWORKS
#include    "..\MHCAP\project_def.h"
#endif

#define	INCLUDE_VXWORKS_SHOW_ROUTINE


strHartCommDebugInfo g_hartCommDbgInfo;
uint32	g_hartCommFrameDisplayFlag = DEBUG_DISABLE;


strHostTblInfo	gHostMngrTblInfo;

uint32  gTaskErrStringFlag = 0;
#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))
	static char *remoteBootImageDwlErrMsg = { "\
	    [ERROR] - MPU ROM Download And Flash Write Error !!\n\
	              Check PC IP Address And tftp.exe\n\
	    " };
	static char *remoteBootImageDwlSuccessMsg = { "\
	    [SUCCESS] - MPU ROM Download And Flash Write SUCCESS !!\n\
	                You Must Reboot !!\n\
	    " };
#endif

#ifdef VXWORKS
uint32 gLinkComDgbFlag = 0;
#endif

extern int32 gDebugPortFd;
extern void *gSendQueId;
extern void *gRecvQueId;

extern uint32	gDebugErrStringFlag;

extern int8 *verDate;

extern uint32 romSwVersion;
extern uint32 releaseDate;

extern int8 *vxIntStackBase;
extern int8 *vxIntStackEnd;


extern strDebugDataInfo gDebugRecvData[DEBUG_INFO_MAX_ARRAY_NUM];
extern strDebugDataInfo gDebugSendData[DEBUG_INFO_MAX_ARRAY_NUM];


extern uint32  	gUsrTaskSchedulingFlag;
extern uint32	gMpuSegmentSchedulingFlag;

#if (BOARD_TYPE == MVME2100_BOARD)
extern uint32	univBaseAdrs;
#endif

extern strDpuDebugInfo	gDpuDebugInfo;	

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
extern strDebugDataInfo gDualRecvData[DEBUG_INFO_MAX_ARRAY_NUM];
extern strDebugDataInfo gDualSendData[DEBUG_INFO_MAX_ARRAY_NUM];
#endif


void cpuBrdRebootByManual(uint32 , uint32 );
static void sysDebugHelpCommand(uint32 , uint32 );
static void queueCountDisplay(uint32 , uint32);
static void appVersionDisplay(uint32, uint32);
static void stationAttatchCheck(uint32, uint32);
void taskInfoDisplay(uint32, uint32);
static void usrTaskInfoPrint(void *, int32 );
void sysErrorPrint(int32 errNo);
void taskExecutionTimeDisplay(uint32, uint32);
static void networkDebugger(uint32, uint32);
static void netDebugHelpCommand(uint32, uint32);
static void fbLogicDebugger(uint32, uint32);
static void fbLogicDbgHelpCmd(uint32, uint32);

static void systemTimeDisplay(uint32, uint32);
static void taskSchedulingInfoDisplay(uint32, uint32);

#if (BOARD_TYPE == MVME2100_BOARD)
	static void univerRegisterDataDisplay(void);
	static void univerRegisterDataWrite(void);
#endif

static void busInterruptGen(void);
static void pciDeviceHeaderShow(uint32, uint32);
void usrAppTaskToSuspend(uint32, uint32);

#ifdef _WIN32
void	usrAppTaskToResume(void);
#endif

static void fbDwlDebugger(uint32 , uint32 );

static void onlineDwlDebugHelpCommand(uint32, uint32);

static void segmentSchedulingTimeDisplay(uint32, uint32); 
static void uniRcvMsgHeadDataDisplay(uint32 arg1, uint32 arg2);
static void uniRcvMsgDataDisplay(uint32 arg1, uint32 arg2);
static void brodRcvMsgHeadDataDisplay(uint32 arg1, uint32 arg2);
static void brodRcvMsgDataDisplay(uint32 arg1, uint32 arg2);
static void uniSendMsgHeadDataDisplay(uint32 arg1, uint32 arg2);
static void uniSendMsgDataDisplay(uint32 arg1, uint32 arg2);
static void brodSendMsgHeadDataDisplay(uint32 arg1, uint32 arg2);
static void brodSendMsgDataDisplay(uint32 arg1, uint32 arg2);
static void boardNetIfShow(uint32 , uint32 );
static void rcvTransDataInfoDisplay(uint32 arg1, uint32 arg2);
static void sndTransDataInfoDisplay(uint32 arg1, uint32 arg2);

#ifdef _WIN32
static void targetDebugHelpCommand(void);
static void userTargetDebugger(void);
extern uint32 filelistDisplay(uint32);
extern void flashDiskinfo(void);
#endif 

#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))
	static romDownloadAndFlashWrite(void);
#endif

static void sioBoardDebugger(void);
static void sioDebugHelpCommand(void);
#if (BOARD_TYPE == MVME3100_BOARD)
static void netIfUnitDataDisplay (void);
#elif (BOARD_TYPE == MVME2500_BOARD)

#endif
extern void mbtsDebugger (uint32, uint32);
extern void mbtcDebugger (uint32, uint32);

extern void memoryCopy (uint8 *, uint8 *, uint32);
extern void memoryClear(uint8 *, uint32);
extern void appErrStringMonitoring (uint32, uint32);
extern void bootParaDisplay(uint32, uint32);
extern void errorQueueCountDisplay(void);
extern void dlMsgQueueCountDisplay(void);
extern uint32  stringCompare(int8 *, int8 *);
extern int32   ioRead(int32 , int8 *, int32 );
extern uint32  ipAddrValidCheck(uint8 *);
extern void sysInetHostAdd( int8 *, int8 *);
extern uint32 sysPing(int8 *);
extern int32   sysTaskIdListGet (int32 *, int32 );
extern uint32   sysTaskInfoGet(int32 , void *);
extern int32 sysErrStringFindByVal(int32,  int8 *,int32 *, uint8 *);
extern void routeInfoShow(uint32, uint32);
extern void schedulerExecInfoDisplay(uint32, uint32);
extern void schedulerInfoDisplay(uint32, uint32);
extern void transDataInfoDisplay(strDebugDataInfo *, uint8 *);
extern void networkDebugVarClear(uint32, uint32);

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
extern void dualTransDataInfoDisplay(strDebugDataInfo *, uint8 *);
extern void dualDebugVarClear(void);
#endif

extern void transDataDetailInfoDisplay(uint8, uint8);
extern void  sharedMemUpDatingCountDisplay(void);
extern void systemRunningStatusInfoDisplay(uint32 , uint32);

extern void segmentStatusDisplay(uint32, uint32);
extern uint32 timeOfDayGet(void);
extern void usrDefineFbCodeInfoDisplay(uint32, uint32);
extern void sharedMemDataDisplay(void);
extern void networkPerformanceDisplay(uint32, uint32);
extern void fatalErrQueCountDisplay(void);
extern void fatalErrMsgClear (uint32, uint32);
extern void fatalErrMsgDisplay(uint32, uint32);
extern void fbTaskSchdInfoDisplay(void);
extern void rasTaskSchdInfoDisplay(void);

extern void dataDisplayForRmtDevice(uint32, uint32 );
extern void rmtComCountDisplay(uint32, uint32);
extern void remoteDevStatusDisplay(uint32, uint32);

extern void rmtCommQueCountDisplay(void);
extern void ioAccOnMpuDBInfoDisplay(void);

extern void dpuComCnfgParameterDisplay(void);
extern int32   keyHit(void);
extern int32 systemClockRateGet(void);
extern int32   sysMsgQueNumGet(void *);
extern void cursorPositionSet(void);
extern void screenClear(void);
extern int32 sysErrNoGet(void);
extern uint32	myBusDataAcc(uint8 *, uint32, uint32	, uint32 ,uint8 *, uint8 *);
extern void dataDisplay(uint8 *, uint8 *, uint16 );
#ifdef VXWORKS
extern int32   ioSysCtrlFunc(int32 , int32 , int32 );
#endif
	
extern int32 spyClkStart(int32);
extern void spyReport (void);
extern void spyClkStop(void);

extern void sysTaskDelay (int32 );
extern uint32 systemTickGet();

extern void ethRmtFbComSeqNoChkDBDisplay (uint32, uint32); 

extern void fbRunningErrorInfoDisplay(uint32, uint32);

extern void spaceSkip(int8 **);

extern uint32   sysTaskStatusChangeToSuspend(int32 );
#ifdef _WIN32
extern uint32   sysTaskStatusChangeToResume(int32);
#endif

extern void fbExecIdQueCountInfoDisplay(uint32 , uint32 );
extern void fbExecIdQueInfoDisplay(uint32 , uint32 );

extern void fbUnusedSpecMemQueInfoDisplay(uint32, uint32 );
extern void fbSpecUnusedMemInfoDisplay(uint32 , uint32 );

extern void onlineDwlfbCodeAddInfoDisplay(void);
extern void onlineDwlfbCodeDelInfoDisplay(void);
extern void onlineDwlfbCodeSpecChgInfoDisplay(void);
extern void onlineDwlfbCodeSeqIdAddAndChgInfoDisplay(void);
extern void fbTaskComQueCountInfoDisplay(void);
extern void fbTaskComQueDataInfoDisplay(void);
extern void mpuSchdInfoDisplay(void);
extern void onlineDwlDelfbCodeMemInfoDisplay(void);

extern void variableMemoryTest(void);

extern void boardReboot();
extern uint32 remoteShelfReset (uint8 *, uint32); 
extern uint32 BusMemDataCopyToFileMemory(void);
extern void xrTaskSchdInfoDisplay(void);

extern void variableMemoryDataDisplay(uint32, uint32);
extern uint32 systemCnfgInfoRead( strSysConfigInfo *);
extern void cmdTransDataInfoDisplay(uint32, uint32);
extern void	fbParaInMemDisplay(uint32, uint32);

extern void sfcDataDisplay();
extern void trsExecIdQueCountInfoDisplay(void);
extern void actExecIdQueCountInfoDisplay(void);
extern void sfcFBExecIdQueInfoDisplay(void);
extern void fbUnusedOutputMemQueInfoDisplay(uint32 , uint32 );
extern void sysExecInfoDisplay(uint32, uint32);
extern void onlineDwlVarChangeInfoDisplay(void);

extern void ipstatShow(uint8);
extern void udpstatShow(void);
extern void arptabShow(void);

#ifdef VXWORKS
extern void ifShow(int8 *);
extern void inetstatShow(void);

extern void routeTblInfoAllDelete(uint32, uint32);
extern void routeInfoDelete(uint32, uint32);
extern void routeInfoAdd(uint32, uint32);
extern int32 pciHeaderShow (int32, int32, int32);

#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))
#if (defined(INCLUDE_VXWORKS_SHOW_ROUTINE))
	
	extern void sysTempeShow (void);
	extern void sysTempeWinShow (void);
	
	
	extern void sysFlashShow (void);
#endif

	
    extern int32 programRemoteBootImage(int8 *,int8 *,int8 *,int8 *,int8 *);
	extern void	mpc8540Tsec1Dump(void);
	extern void	sysMpc8548Tsec1Show(void);

#endif

#endif
#ifdef VXWORKS
extern void netStackDataPoolShow(void);
extern void netStackSysPoolShow(void);
#endif

extern void memoryBoardParameterDisplay(uint32, uint32);
#ifdef _WIN32
extern uint32 	fbSysExeInfoPointerGet(strSysExecInfo** ,uint32 );
#endif

#ifdef _WIN32
extern strTaskListInfo gTaskList[MAX_USR_TASK_NUM];
uint32 taskExecutionTime () ; 
extern void routeTblShow(uint32, strRouteTblInfo*);
extern char *ctime (const time_t *tp);
extern uint32	exceptFilterFunc(LPEXCEPTION_POINTERS, uint8*,uint32);
extern uint32   taskSuspendToListStatus(uint32);
#endif
extern void dualDataDetailInfoDisplay(uint8, uint8);

extern void errQAccessAndDisplay(void);

extern void mpc8540Tsec1ErrCntDisplay (void);

extern void upperToLowerCaseConversion(int8 *, uint32 );
extern void boardTimeSet(uint32 , uint32 );
extern void transQueueCountDisplay(void);


static void sysDebugHelpCommand(uint32 arg1, uint32 arg2)
{
    int8    **pMsg;
    int8 *helpMsg[] =
    {
        "[?,h,H]",  	"- Print below command list.",
        "[bpd,BPD]",    "- Board booting parameter Display",
        "[brt, BRT]",	"- Board Reboot",
        "[btd,BTD]",	"- Board Time Display",
		"[bts,BTS]",	"- Board Time Set",
#ifdef _WIN32
        "[cm,CM]",  	"- Compare Data Between Local Ram and Memory Board",
#endif
        "[fdd,FDD]",	"- FB Download Debugger",
        "[fld,FLD]",	"- FB Logic Debugger ",
        "[lmc,LMC]",	"- Log Message Clear",
        "[lmd,LMD]",	"- Log Message Display",
        "[maid,MAID]", 	"- MEMORY Allocation Info Display",

#ifdef  _MBTS_PROGRAM	
		"[mbtc,MBTC]",	"- ModBus-Tcp-Client Debugger ",	
		"[mbts,MBTS]",	"- ModBus-Tcp-Server Debugger ",	
#endif	
		"[ndd,NDD]",	"- Network Data Debugger ",	
        "[ping,PING]",	"- Ping(Network Test)",
        "[qcd,QCD]",	"- System Queue Count Display",

#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))
        "[rdw,RDW]", 	"- MPU ROM File Download and FlashWrite",
#endif
        "[red,RED]",  	"- Runtime Error Display",
        "[rfed,RFED]", 	"- Running FB Code Error Display",
        "[rvd,RVD]",	"- ROM Version Display",
        "[scd,SCD]",	"- Scheduler runtime Count Display",
        "[sid,SID]",	"- Scheduler set Info Display",
        "[tetd,TETD]", 	"- Task Execution time Display",
        "[tid,TID]",	"- Task Information Display",
        "[tscs,TSCS]",	"- Task Stauts Change To SUSPEND",
        "[tsd,TSD]", 	"- Task Runtime Scheduling Info Display",
#ifdef _WIN32
        "[uts, UTS]",	"- User Target System Display ",
#endif
#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))
#if (defined(INCLUDE_VXWORKS_SHOW_ROUTINE))
        "[vd,VD]",		"- VME Chip Register Data Display",
        "[vwd,VWD]",	"- VME WINDOW Data Display",
#endif
#endif
        NULL
    };

    printf( "\n");

    for(pMsg = helpMsg; *pMsg != NULL; pMsg += 2)
        printf( "\t%-15s %s\n", *pMsg, *(pMsg+1));
}


static void appVersionDisplay(uint32 arg1, uint32 arg2)
{
	uint16	year;
	uint8 month,date;
	uint8 *tmpPtr = (uint8 *) &releaseDate;
	
#ifdef VXWORKS
	
	year = ((*(uint16*)tmpPtr));
	tmpPtr += 2;
	
	
	month =  ((*(uint8*)tmpPtr));
	tmpPtr += 1;
	
	
	date =  ((*(uint8*)tmpPtr));
#elif	(ENDIAN_TYPE == LITTLE_ENDIAN)
	
	date =  ((*(uint8*)tmpPtr));
	tmpPtr += 1;
	
	
	month =  ((*(uint8*)tmpPtr));
	tmpPtr += 1;
	
	
	year = ((*(uint16*)tmpPtr));
#endif

    printf( "\n");

#ifndef EVALUATION_USE_BUILD
    printf( "%10s%s","","==== HVDC Simulator System Software Information ===\n");
#elif (EVALUATION_TIMEOUT == EVALUATION_TIMEOUT_1DAY)
	printf( "%10s%s","","==== HVDC Control System Information(Evaluation-1Day) ===\n");
#elif (EVALUATION_TIMEOUT == EVALUATION_TIMEOUT_1HOUR)
	printf( "%10s%s","","==== HVDC Control System Information(Evaluation-1Hour) ===\n");
#elif (EVALUATION_TIMEOUT == EVALUATION_TIMEOUT_5MIN)
	printf( "%10s%s","","==== HVDC Control System Information(Evaluation-5Minute) ===\n");
#endif 
    printf( "\n");
    printf( "%10s%s%d","","Software Version              : ",romSwVersion);
    printf( "\n");
    printf( "%10s%s%x/%02x/%02x","","Software Date                 : ", year,month,date);
    printf( "\n");
    printf( "%10s%s%s","","Software Compilation Date     : ", verDate);
    printf( "\n");
#if (ROM_RELEASE_TO_USER != 1)
#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))
    printf( "%10s%s","","[WARNING] It is not ROM's Release Version !!!");
    printf( "\n");
    printf( "%10s%s","","          You Should Contact to Developer !!!");
    printf( "\n");
#endif
#endif
    
	return;
}

static void debugInfoPromptGet(int8 *stringPtr)
{
    int8    *dbgStrKind[] =
    {
    	"UNKNOWN",
    	"CTRL_DBG",
    	"PROT_DBG",
    	"VBC_DBG",
    	"MES_DBG"
    };
    
    uint32	strIndex;
    
	strSysConfigInfo sysCnfgInfo;

	systemCnfgInfoRead( (strSysConfigInfo *) &sysCnfgInfo);

	if((sysCnfgInfo.hvdcCpuType < HVDC_CPU_C_TYPE) || (sysCnfgInfo.hvdcCpuType > HVDC_CPU_M_TYPE))
		strIndex = 0;
	else
		strIndex = sysCnfgInfo.hvdcCpuType;	
		 	
  	(void)sprintf(stringPtr, "[%s_#%02d]> ", dbgStrKind[strIndex],sysCnfgInfo.stationId);

	return;
}

int32 usrDbgTask(int32 arg1,int32 arg2,int32 arg3,int32 arg4,int32 arg5,
                int32 arg6,int32 arg7,int32 arg8,int32 arg9,int32 arg10)
{
    int8    line[16], *pLine;
    int8    debugPrompt[32];
	
    int8    defaultStr[] = "Unrecognized Command. Type '?','h','H' for Help\n";
	
	
	strDbgCmdFuncInfo debugTaskCmdFunc[] =
	{
		{"?", 		NULL, 			NULL,	&sysDebugHelpCommand},
		{"h",		NULL,			NULL,	&sysDebugHelpCommand},
		{"bpd", 	EXT_VAR_READ,	NULL,	&bootParaDisplay},
		{"brt", 	NULL, 			NULL,	&cpuBrdRebootByManual},
		{"btd",		NULL,			NULL,	&systemTimeDisplay},
		{"bts", 	NULL, 			NULL,	&boardTimeSet},
		{"fdd", 	NULL, 			NULL,	&fbDwlDebugger},
		{"fld", 	NULL, 			NULL,	&fbLogicDebugger},
		{"lmc", 	NULL, 			NULL,	&fatalErrMsgClear},
		{"lmd", 	NULL, 			NULL,	&fatalErrMsgDisplay},
		{"maid", 	EXT_VAR_READ, 	NULL,	&memoryBoardParameterDisplay},
#ifdef  _MBTS_PROGRAM	
		{"mbtc",	NULL,			NULL,	&mbtcDebugger},
		{"mbts",	NULL,			NULL,	&mbtsDebugger},
#endif
		{"ndd", 	NULL, 			NULL,	&networkDebugger},
		{"ping",	NULL,			NULL,	&stationAttatchCheck},
		{"qcd",		NULL, 			NULL,	&queueCountDisplay},
		{"red", 	NULL, 			NULL,	&appErrStringMonitoring},
		{"rfed", 	NULL, 			NULL,	&fbRunningErrorInfoDisplay},
		{"rvd",		NULL, 			NULL,	&appVersionDisplay},
		{"scd", 	NULL, 			NULL,	&schedulerExecInfoDisplay},
		{"sid", 	NULL, 			NULL,	&schedulerInfoDisplay},
		{"tetd", 	NULL, 			NULL,	&taskExecutionTimeDisplay},
		{"tid", 	NULL, 			NULL,	&taskInfoDisplay},
		{"tscs",	NULL, 			NULL,	&usrAppTaskToSuspend},
		{"tsd", 	NULL, 			NULL,	&taskSchedulingInfoDisplay},
		{"NULL",	NULL, 			NULL,	NULL} 	 		
	};

	strDbgCmdFuncInfo *cmdFuncPtr;
	
	memoryClear( (uint8 *)&gHostMngrTblInfo, sizeof(strHostTblInfo));	

	gDebugErrStringFlag 	= DEBUG_DISABLE;

	errQAccessAndDisplay();

	memoryClear((uint8 *)debugPrompt, 32);

	debugInfoPromptGet(debugPrompt);
	
	printf("%s ", debugPrompt);
 
	for(;;) 
	{
	    (void)printf(debugPrompt);

        (void)ioRead(gDebugPortFd, line, sizeof(line));
		
        pLine = line;
        
	    spaceSkip(&pLine);
		
		upperToLowerCaseConversion( pLine, strlen(pLine));

		if(*pLine != ENDOFSTR) 
		{
			for(cmdFuncPtr = debugTaskCmdFunc; cmdFuncPtr->dbgFunc != NULL; cmdFuncPtr++)
			{
				if(stringCompare(pLine,cmdFuncPtr->dbgCmdStr) == NO_ERROR)
				{
					cmdFuncPtr->dbgFunc(cmdFuncPtr->arg1,cmdFuncPtr->arg2);
	                printf("\n");
					break;	
				} 	
			}
			
			if(cmdFuncPtr->dbgFunc == NULL)
			{
				printf("%s", defaultStr);
			}
		}
    }
}

#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))
static romDownloadAndFlashWrite(void)
{
#ifdef VXWORKS
    int8    line[20], serverIpAddrArg[32], fileName[32];
    int8    *cPtr;
	int32	status;
	
    memoryClear ( (uint8 *)line, 20);
    memoryClear ( (uint8 *)serverIpAddrArg, 32);
    memoryClear ( (uint8 *)fileName, 32);

   	printf( "********************************************\n");
   	printf( "** MPU ROM Download and ROM Write Utility **\n");
   	printf( "********************************************\n");

#if defined(INCLUDE_VXWORKS_SHOW_ROUTINE)
	sysFlashShow ();
#endif
	
	
SERVER_IP_ADDR_SET:
	printf( "Server(PC) IP Address Enter ?(XXX.XXX.XXX.XXX) ");

    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    if( (*cPtr != ENDOFSTR) && (ipAddrValidCheck((uint8 *)cPtr) == NO_ERROR )){
		sprintf(serverIpAddrArg,"-h%s",cPtr);
    }
    else if ( *cPtr == '.'){
    	printf( "Rom Download and Flash Write Cancelled !!\n");
    	return;	
    }
    else {
    	printf("Invalid IP Address .. Retry !!\n");
    	goto SERVER_IP_ADDR_SET;
    } 

	
	printf("MPU ROM File Name Enter ?(3100.xxx) ");

    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    if(*cPtr == ENDOFSTR)
		sprintf(fileName,"-n%s","3100.rom");
    else if ( *cPtr == '.'){
    	printf("Rom Download and Flash Write Cancelled !!\n");
    	return;	
    }
    else {
		sprintf(fileName,"-n%s",cPtr);
    } 

	
    status = programRemoteBootImage(
    								 fileName,
    								 "-d/flash0",
    								 "-bA", 
    								 serverIpAddrArg,
    								 NULL
    								);
	if(status != NO_ERROR) 
		printf(remoteBootImageDwlErrMsg);
		
	else {
		printf(remoteBootImageDwlSuccessMsg);
		printf("\n\n@@@@@@ Board Reboot ?[Y/N] : ");
    	ioRead(gDebugPortFd, line, sizeof(line));
    	cPtr = line;

        if((*cPtr == 'Y') || (*cPtr == 'y')){
            printf( "\nBoard Rebootting ....\n");
            boardReboot();
	    }
	}
#endif
}
#endif


static void fbLogicDbgHelpCmd(uint32 argc1, uint32 argc2)
{
    int8    **pMsg;
    int8 *helpMsg[] =
    {
        "[?,h,H]",  	"- Print below command list.",
        "[q,Q]",		"- Exit MPU Debugger",
        "[lid,LID]", 	"- Logic Input Data Display",
        "[red,RED]",  	"- Runtime Error Display",
        "[rld,RLD]", 	"- Runtime Logic Info Display",
		"[rvid,RVID]",	"- Runtime Variable Info Display",
        "[trid,TRID]",	"- Task Runtime Info Display",
        "[tsd,TSD]",	"- Task Status Display",
        NULL
    };

    printf( "\n");

    for(pMsg = helpMsg; *pMsg != NULL; pMsg += 2)
        printf( "\t%-15s %s\n", *pMsg, *(pMsg+1));
	
	return;
}


static void fbLogicDebugger(uint32 arg1, uint32 arg2)
{
    int8    line[16], *pLine;
    
    int8    debugPrompt[32];
    int8	dbgKind[] = "LOGIC_DBG";
	strSysConfigInfo	sysCnfgInfo;
    
    int8    defaultStr[] = "Unrecognized Command. Type '?','h','H' for Help\n";

	strDbgCmdFuncInfo mpuDebugCmdFunc[] =
	{
		{"?", 		NULL, 		NULL,			&fbLogicDbgHelpCmd},
		{"h",		NULL,		NULL,			&fbLogicDbgHelpCmd},
		{"lid",		LOGIC_ID,	LOCAL_MEM_ACC,	&fbParaInMemDisplay},
		{"red",		NULL, 		NULL,			&appErrStringMonitoring},
		{"rld",		LOGIC_ID, 	NULL,			&usrDefineFbCodeInfoDisplay},
		{"rvid", 	NULL, 		NULL,			&variableMemoryDataDisplay},
		{"trid", 	NULL, 		NULL,			&segmentSchedulingTimeDisplay},
		{"tsd", 	LOGIC_ID,	NULL,			&segmentStatusDisplay},
		{"NULL",	NULL, 		NULL,			NULL} 	 	
	};

	strDbgCmdFuncInfo *cmdFuncPtr;
	
	memoryClear((uint8 *)debugPrompt, 32);
	systemCnfgInfoRead( (strSysConfigInfo *) &sysCnfgInfo);

  	(void)sprintf(debugPrompt, "[%s_#%02d]> ", dbgKind, sysCnfgInfo.stationId);

	for(;;) 
	{
	    (void)printf(debugPrompt);

        (void)ioRead(gDebugPortFd, line, sizeof(line));
		
        pLine = line;
        
	    spaceSkip(&pLine);
		
		upperToLowerCaseConversion( pLine, strlen(pLine));
		
		if(*pLine == 'q')
		{
			printf("\n   **** Return To Main Debugger !!!\n\n");
			break;
		}

		else if(*pLine != ENDOFSTR) 
		{
			for(cmdFuncPtr = mpuDebugCmdFunc; cmdFuncPtr->dbgFunc != NULL; cmdFuncPtr++)
			{
				if(stringCompare(pLine,cmdFuncPtr->dbgCmdStr) == NO_ERROR)
				{
					cmdFuncPtr->dbgFunc(cmdFuncPtr->arg1,cmdFuncPtr->arg2);
	                printf("\n");
					break;	
				} 	
			}
			
			if(cmdFuncPtr->dbgFunc == NULL)
			{
				printf("%s", defaultStr);
			}
		}
    }
}


static void systemTimeDisplay(uint32 arg1, uint32 arg2)
{
    time_t	timeData;

    timeData = timeOfDayGet();
	printf( "\n\t* System Time : %s\n", ctime(&timeData));

	return;
}

static void netDebugHelpCommand(uint32 arg1, uint32 arg2)
{
    int8    **pMsg;
    int8 *helpMsg[] =
    {
        "[?,h,H]",		"- Print below command list.",
#ifdef VXWORKS
        "[bnid,BNID]",	"- Board Network Info Display",
#endif
        "[cfcd,CFCD]",	"- Com-FB Snd/Rcv Count Display", 
        "[cfd,CFD]",	"- Com-FB DB Info Display", 
        "[dvc,DVC]",	"- Debug Variable Clear",
	    "[npd,NPD]",	"- Network Performance Display",
        "[nrid,NRID]",	"- Net. Receiving Info Display",
        "[nsid,NSID]",	"- Net. Sending Info Display",
        "[q,Q]",		"- Return To Main Debugger",
        "[rad,RAD]",	"- Router Add",
        "[rbdd,RBDD]",	"- Receiving Broadcast Data Data Display",
        "[rbhd,RBHD]",	"- Receiving Broadcast Data Head Display",
        "[rca,RCA]",	"- Router Info Clear All",
        "[rda,RDA]",	"- Router Info Display All",
        "[rde,RDE]",	"- Router Info Delete",
        "[red,RED]",	"- Runtime Error Display",
        "[rudd,RUDD]",	"- Receiving Unicast Data Display",
        "[ruhd,RUHD]",	"- Receiving Unicast Data Head Display",
        "[sbdd,sbdd]",	"- Sending Broadcast Data Display",
        "[sbhd,SBHD]",	"- Sending Broadcast Data Head Display",
        "[sudd,SUDD]",	"- Sending Unicast Data Display",
        "[suhd,SUHD]",	"- Sending Unicast Data Head Display",
        NULL
    };

    printf( "\n");

    for(pMsg = helpMsg; *pMsg != NULL; pMsg += 2)
        printf( "\t%-15s %s\n", *pMsg, *(pMsg+1));
	
	return;
}


static void networkDebugger(uint32 arg1, uint32 arg2)
{
    int8    line[16], *pLine;
    
    int8    debugPrompt[32];
    int8	dbgKind[] = "NET_DBG";
	strSysConfigInfo	sysCnfgInfo;

    static int8    defaultStr[] = "Unrecognized Command. Type '?','h','H' for Help\n";

	strDbgCmdFuncInfo netDebugCmdFunc[] =
	{
		{"?", 		NULL, 			NULL,		&netDebugHelpCommand},
		{"h",		NULL,			NULL,		&netDebugHelpCommand},
		{"bnid", 	NULL, 			NULL,		&boardNetIfShow},
		{"cfcd",	NULL,			NULL,		&rmtComCountDisplay},
		{"cfd", 	NULL,			NULL,		&ethRmtFbComSeqNoChkDBDisplay},
		{"dvc", 	NULL, 			NULL,		&networkDebugVarClear},
		{"npd", 	NULL, 			NULL,		&networkPerformanceDisplay},
		{"nrid", 	NULL, 			NULL,		&rcvTransDataInfoDisplay},
		{"nsid", 	NULL, 			NULL,		&sndTransDataInfoDisplay},
		{"rad", 	NULL, 			NULL,		&routeInfoAdd},
		{"rbdd", 	NULL, 			NULL,		&brodRcvMsgDataDisplay},
		{"rbhd", 	NULL, 			NULL,		&brodRcvMsgHeadDataDisplay},
		{"rca", 	NULL, 			NULL,		&routeTblInfoAllDelete},
		{"rda", 	EXT_VAR_READ,	NULL,		&routeInfoShow},
		{"rde", 	NULL, 			NULL,		&routeInfoDelete},
		{"red", 	NULL, 			NULL,		&appErrStringMonitoring},
		{"rudd", 	NULL, 			NULL,		&uniRcvMsgDataDisplay},
		{"ruhd", 	NULL, 			NULL,		&uniRcvMsgHeadDataDisplay},
		{"sbdd", 	NULL, 			NULL,		&brodSendMsgDataDisplay},
		{"sbhd", 	NULL, 			NULL,		&brodSendMsgHeadDataDisplay},
		{"sudd", 	NULL, 			NULL,		&uniSendMsgDataDisplay},
		{"suhd", 	NULL, 			NULL,		&uniSendMsgHeadDataDisplay},
		{"NULL",	NULL, 			NULL,		NULL} 	 	
	};

	strDbgCmdFuncInfo *cmdFuncPtr;
	
	memoryClear((uint8 *)debugPrompt, 32);
	systemCnfgInfoRead( (strSysConfigInfo *) &sysCnfgInfo);

  	(void)sprintf(debugPrompt, "[%s_#%02d]> ", dbgKind, sysCnfgInfo.stationId);

	for(;;) 
	{
	    (void)printf(debugPrompt);

        (void)ioRead(gDebugPortFd, line, sizeof(line));
		
        pLine = line;
        
	    spaceSkip(&pLine);
		
		upperToLowerCaseConversion( pLine, strlen(pLine));
		
		if(*pLine == 'q')
		{
			printf("\n   **** Return To Main Debugger !!!\n\n");
			break;
		}

		else if(*pLine != ENDOFSTR) 
		{
			for(cmdFuncPtr = netDebugCmdFunc; cmdFuncPtr->dbgFunc != NULL; cmdFuncPtr++)
			{
				if(stringCompare(pLine,cmdFuncPtr->dbgCmdStr) == NO_ERROR)
				{
					cmdFuncPtr->dbgFunc(cmdFuncPtr->arg1,cmdFuncPtr->arg2);
	                printf("\n");
					break;	
				} 	
			}
			
			if(cmdFuncPtr->dbgFunc == NULL)
			{
				printf("%s", defaultStr);
			}
		}
    }
}


#ifdef SYS_AUX_CLK_USED
void systemUsrDefinedAuxClockSet(void);
#endif

void taskExecutionTimeDisplay(uint32 arg1, uint32 arg2)
{
	uint32	timeOut = 5;
	
	printf("\n\t***** Waiting for Collecting Data ...");

#ifdef VXWORKS
    spyClkStart(0);

	sysTaskDelay(systemClockRateGet() * timeOut);

    spyClkStop();
#ifdef SYS_AUX_CLK_USED
	systemUsrDefinedAuxClockSet();
#endif

	printf( "\n");

    spyReport();
#endif

#ifdef _WIN32
	taskExecutionTime();
#endif

    return;
}


static void queueCountDisplay(uint32 arg1 , uint32 arg2)
{
    int8 **pMsg;
    int8 *queName[] = {
        "SYS. SEND QUEUE", "SYS. RECV QUEUE", NULL
    };

	
    printf( "\n");
	
	errorQueueCountDisplay();
    printf( "\n");
    
    dlMsgQueueCountDisplay();
    printf( "\n");

	fatalErrQueCountDisplay();
  
	rmtCommQueCountDisplay();

	transQueueCountDisplay();
    printf( "\n");
	
    printf( "\t");
    for(pMsg = queName; *pMsg != NULL; pMsg++)
        printf( "%-20s", *pMsg);

    printf( "\n");
    printf( "\t");

    for(pMsg = queName; *pMsg != NULL; pMsg++)
        printf( "%-20s","--------------");

    printf( "\n");
    printf( "\t");

    printf( "%8d%20d\n", sysMsgQueNumGet(gSendQueId),
                               sysMsgQueNumGet(gRecvQueId)
            );
	return;
}


static void stationAttatchCheck(uint32 arg1, uint32 arg2)
{
    int8    ipAddr[32], hostName[32];
    int32   index, same = 0;

INPUT_RETRY:

    memoryClear (ipAddr, 32);

    printf( "[HELP] - Enter Ip Addr Or  '.' to escape !!\n");

    printf( "Enter IP Address ? (XXX.XXX.XXX.XXX) ");

    ioRead(gDebugPortFd, ipAddr, sizeof(ipAddr));

    if(ipAddr[0] == '.') {
        printf( "CANCEL the Station Attatch Checking !!\n");
        return;
    }
    
    if(ipAddrValidCheck((uint8 *)ipAddr)!= NO_ERROR) {
        printf( "\n**********INVALID IP ADDRESS**********\n");
        goto INPUT_RETRY;
    }

    
    for(index = 0; index < MAX_HOST_NUM; index++) {
        if(!stringCompare(ipAddr, gHostMngrTblInfo.host[index].iAdrs)){ 
            same = 1;
            break;
        }
    }

    if(!same){ 
        
        index = gHostMngrTblInfo.curIndex;
        if(index == MAX_HOST_NUM) index = 0;

        sprintf(hostName,"%s%.2d","TARGET", index);

        memoryClear ((uint8 *) &gHostMngrTblInfo.host[index], sizeof(strHostsInfo));

        memoryCopy(gHostMngrTblInfo.host[index].hName, hostName, strlen(hostName));

        
        memoryCopy(gHostMngrTblInfo.host[index].iAdrs, ipAddr, strlen(ipAddr));

        
        sysInetHostAdd( gHostMngrTblInfo.host[index].hName,
                        gHostMngrTblInfo.host[index].iAdrs);

        
        if(++gHostMngrTblInfo.curIndex == MAX_HOST_NUM)
            gHostMngrTblInfo.curIndex = 0;
    }

    if(sysPing( ipAddr))
        printf( "\n\nThe '%s' is Not reached\n", ipAddr);
    else
        printf( "\n\nThe '%s' is  Alive\n", ipAddr);

	return;
}


static void segmentSchedulingTimeDisplay(uint32 arg1, uint32 arg2)
{
	int8 *titleStr[] =
	{
		"P:TASK","MODE","CYCLE", "SC_CNT", "MIN_TM","CUR_TM","MAX_TM","MAX_SC","STATUS", NULL
	};	
	int8 **pMsg;
	screenClear();
 
	gMpuSegmentSchedulingFlag = DEBUG_ENABLE;

    while(!keyHit()) 
    {
       	cursorPositionSet();
		
	    printf("\t***************** LOGIC Task Scheduling Info ******************\n");
        printf("\n");

		for(pMsg = titleStr; *pMsg != NULL; pMsg++) 
	 		printf("%7s", *pMsg);
	
 		printf("%9s\n", "TASKTICK");

		for(pMsg = titleStr; *pMsg != NULL; pMsg++) 
	 		printf("%7s", " ------");

 		printf("%7s\n", " --------");

		mpuSchdInfoDisplay();
		sysTaskDelay(systemClockRateGet()/2);
    }
	
	gMpuSegmentSchedulingFlag = DEBUG_DISABLE;

}


static void taskSchedulingInfoDisplay(uint32 arg1, uint32 arg2)
{
	int8 *titleStr[] =
	{
		"TASK_NAME","START TICK", "END TICK", "S_TIME(mSec)",NULL
	};	
	int8 **pMsg;
	screenClear();
 
	gUsrTaskSchedulingFlag = DEBUG_ENABLE;

    while(!keyHit()) 
    {
       	cursorPositionSet();
		
	    printf("\t***************** Task Scheduling Info ******************\n");
        printf("\n");

		for(pMsg = titleStr; *pMsg != NULL; pMsg++) 
	 		printf("%18s", *pMsg);
	
        printf("\n");

		for(pMsg = titleStr; *pMsg != NULL; pMsg++) 
	 		printf("%18s", "---------------");
		
        printf("\n");

		fbTaskSchdInfoDisplay();
		rasTaskSchdInfoDisplay();
		
		sysTaskDelay(systemClockRateGet());
    }
	
	gUsrTaskSchedulingFlag = DEBUG_DISABLE;

}


void taskInfoDisplay(uint32 arg1, uint32 arg2)
{

#ifdef _WIN32
    taskExecutionTime(); 
#endif

#ifdef VXWORKS   
	TASK_DESC  taskInfo[MAX_USR_TASK_NUM];
	int32  idList[MAX_USR_TASK_NUM];
    int32  retTaskNum, index;

	memoryClear( (uint8 *)taskInfo, sizeof(TASK_DESC));
	memoryClear( (uint8 *)idList, MAX_USR_TASK_NUM * 4);


    
    retTaskNum = sysTaskIdListGet( (int32 *)idList, MAX_USR_TASK_NUM);

    if(retTaskNum == 0){
        printf( "\n[ERROR] - No available Task Id List\n");
        goto MODULE_END;
    }
    for(index = 0; index < retTaskNum; index++){
        if(sysTaskInfoGet(idList[index], (void *)&taskInfo[index])){
            printf( "\n[ERROR] - Task Information Get Error\n");
            goto MODULE_END;
        }
    }

    
    usrTaskInfoPrint((void *)&taskInfo[0], retTaskNum);

MODULE_END:
#endif
	return;
}

static void usrTaskInfoPrint( void *infoPtr, int32 taskNum)
{
#ifdef  VXWORKS
    int32   index;
    int8    *string[] =
    {
        "T_NAME", "T_PRIO", "T_ID","T_STAT", "S_SIZE", "S_USE", "S_MAX",
        "T_ERR","T_DLY", NULL
    };

    int8    **pMsg;

    int8    *pIntStackHigh;
    int8    *intStackBase   = vxIntStackBase;
    int8    *intStackEnd    = vxIntStackEnd;
	
	int8	tStatusString[64];
	int32	status;
	
    TASK_DESC   *taskInfoPtr = (TASK_DESC *)infoPtr;

    printf( "\n");

    for(pMsg = string; *pMsg != NULL; pMsg++)
        printf( "%-9s", *pMsg);
    printf( "\n");

    for(pMsg = string; *pMsg != NULL; pMsg++)
        printf( "%-9s", "-----");
    printf( "\n");

    for(index = 0; index < taskNum; index++, taskInfoPtr++) {
        printf( "%-10s", taskInfoPtr->td_name);
        printf( "%-9d",taskInfoPtr->td_priority);
        printf( "%-9d",taskInfoPtr->td_id);
        
        memoryClear( (uint8 *)tStatusString, 64);
		status = taskStatusString(taskInfoPtr->td_id,tStatusString);
		
		if(status)
			memoryCopy( (uint8 *)tStatusString, "G_ERR", strlen("G_ERR"));

        printf( "%-9s",tStatusString);			        
        printf( "%-9d",taskInfoPtr->td_stackSize);
        printf( "%-9d",taskInfoPtr->td_stackCurrent);
        printf( "%-9d",taskInfoPtr->td_stackHigh);
        printf( "%-9x",taskInfoPtr->td_errorStatus);
        printf( "%-9d",taskInfoPtr->td_delay);
        printf( "\n");
    }
    
#if     (_STACK_DIR == _STACK_GROWS_DOWN)
    for(    pIntStackHigh = intStackEnd;
            *(uint8 *)pIntStackHigh == 0xee;
            pIntStackHigh++
        );
#else
    for(    pIntStackHigh = intStackEnd - 1;
            *(uint8 *)pIntStackHigh == 0xee;
            pIntStackHigh--
        );
#endif

    printf( "%-10s","INTR");
    printf( "%-9d",0);
    printf( "%-9d",0);
    printf( "%-9d",(int32)((intStackEnd - intStackBase) * _STACK_DIR)); 
    printf( "%-9d",0);                                         
    printf( "%-9d",(int32)((pIntStackHigh - intStackBase) * _STACK_DIR)); 
    printf( "%-9s",(pIntStackHigh == intStackEnd) &&
                     (intStackEnd != intStackBase != 0) ? "OVERFLOW":"0");
    printf( "%-9d",0);
    printf( "\n");
#endif

}


#define	MAX_SYM_STRING_LEN	256
void sysErrorPrint(int32 errNo)
{
    int8 statName [MAX_SYM_STRING_LEN+1];
    int32 pval;
    uint8 ptype;

    
	memoryClear( (uint8 *) statName, MAX_SYM_STRING_LEN+1);

    if (errNo == 0){
        errNo = sysErrNoGet();
#ifdef VXWORKS
        if (errNo == OK){
#else        
		if (errNo == 0){
#endif
         printf( "OK.\n");
            return;
        }
    }
    if(sysErrStringFindByVal (errNo, statName, &pval, &ptype))
        printf( "Error status symbol table not included (errno = %#x).\n",errNo);
    else{
        if (pval != errNo)
            printf( "Unknown errno = #%x\n", errNo);
        else
            printf( "%s\n", statName);
    }
}

void usrAppTaskToSuspend(uint32 arg1, uint32 arg2)
{
	int32	taskId;
   	int8    line[16], *cPtr;
   	uint32	status = NO_ERROR;
#ifdef VXWORKS
	printf( "Enter Task ID : ");
#else
	printf( "Enter Task ID(HEX) : 0x");
#endif

   	ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

	if((*cPtr == '.')||(*cPtr == 0)) {
		printf( "Task Status Changing is Cancelled !!\n");
		return;	
	}
#ifdef VXWORKS
	taskId = atoi(line);
#else
	taskId = strtoul(cPtr,(char**)&cPtr,16);
#endif
	status = sysTaskStatusChangeToSuspend(taskId);
	
	if(status != NO_ERROR)
		printf( "[ERROR] - Task Status Changing is ERROR\n");
	else 
#ifdef VXWORKS
		printf( "Task ID %d Status is Suspended!!!\n", taskId);
#else
		printf( "Task ID 0x%8x Status is Suspended!!!\n", taskId);
#endif
}

#ifdef _WIN32
void usrAppTaskToResume(void)
{
	int32	taskId;
   	int8    line[16], *cPtr;
   	uint32	status = NO_ERROR;

	printf( "Enter Task ID(HEX) : 0x");
   	ioRead(gDebugPortFd, line, sizeof(line));

    cPtr = line;
    spaceSkip(&cPtr);

	if(*cPtr == '.') {
		printf( "Task Status Changing is Cancelled !!\n");
		return;	
	}
	
	taskId = strtoul(cPtr,(char**)&cPtr,16);
	status = sysTaskStatusChangeToResume(taskId);
	
	if(status != NO_ERROR)
		printf( "[ERROR] - Task Status Changing is ERROR\n");
	else 
		printf( "Task ID 0x%8x Status is Suspended!!!\n", taskId);
}
#endif

static void onlineDwlDebugHelpCommand(uint32 arg1, uint32 arg2)
{
    int8    **pMsg;
    int8 *helpMsg[] =
    {
        "[?,h,H]",  	"- Print below command list.",
        "[q,Q]",		"- Return to Main Debugger",
		"[leid,LEID]",	"- Logic Exec. Info Display",
        "[liqd,LIQD]",	"- Logic Input Unused Mem. Queue Info Display",
        "[lmsd,LMSD]",	"- Logic Mem. Size Display",
        "[loqd,LOQD]",	"- Logic Output Unused Mem. Queue Info Display",
        "[lqcd,LQCD]",	"- Logic Exec. Queue Count Display",
        "[lqid,LQID]",	"- Logic Exec. Queue Info Display",
        "[red,RED]",  	"- Runtime Error Display",
        NULL
    };

    printf( "\n");

    for(pMsg = helpMsg; *pMsg != NULL; pMsg += 2)
        printf( "\t%-15s %s\n", *pMsg, *(pMsg+1));
	
	return;
}

static void fbDwlDebugger(uint32 arg1, uint32 arg2)
{
    int8    line[16], *pLine;
    
    int8    debugPrompt[32];
    int8	dbgKind[] = "LOGIC_DWL_DBG";
	strSysConfigInfo	sysCnfgInfo;

    static int8    defaultStr[] = "Unrecognized Command. Type '?','h','H' for Help\n";

	strDbgCmdFuncInfo netDebugCmdFunc[] =
	{
		{"?", 		NULL,	NULL,	&onlineDwlDebugHelpCommand},
		{"h",		NULL,	NULL,	&onlineDwlDebugHelpCommand},
		{"leid", 	NULL,	NULL,	&sysExecInfoDisplay},
		{"liqd", 	NULL,	NULL,	&fbUnusedSpecMemQueInfoDisplay},
		{"lmsd", 	NULL,	NULL,	&fbSpecUnusedMemInfoDisplay},
		{"loqd", 	NULL,	NULL,	&fbUnusedOutputMemQueInfoDisplay},
		{"lqcd", 	NULL,	NULL,	&fbExecIdQueCountInfoDisplay},
		{"lqid", 	NULL,	NULL,	&fbExecIdQueInfoDisplay},
		{"red", 	NULL,	NULL,	&appErrStringMonitoring},
		{"NULL",	NULL,	NULL,	NULL} 	 	
	};

	strDbgCmdFuncInfo *cmdFuncPtr;
	
	memoryClear((uint8 *)debugPrompt, 32);
	systemCnfgInfoRead( (strSysConfigInfo *) &sysCnfgInfo);

  	(void)sprintf(debugPrompt, "[%s_#%02d]> ", dbgKind, sysCnfgInfo.stationId);

	for(;;) 
	{
	    (void)printf(debugPrompt);

        (void)ioRead(gDebugPortFd, line, sizeof(line));
		
        pLine = line;
        
	    spaceSkip(&pLine);
		
		upperToLowerCaseConversion( pLine, strlen(pLine));
		
		if(*pLine == 'q')
		{
			printf("\n   **** Return To Main Debugger !!!\n\n");
			break;
		}

		else if(*pLine != ENDOFSTR) 
		{
			for(cmdFuncPtr = netDebugCmdFunc; cmdFuncPtr->dbgFunc != NULL; cmdFuncPtr++)
			{
				if(stringCompare(pLine,cmdFuncPtr->dbgCmdStr) == NO_ERROR)
				{
					cmdFuncPtr->dbgFunc(cmdFuncPtr->arg1,cmdFuncPtr->arg2);
	                printf("\n");
					break;	
				} 	
			}
			
			if(cmdFuncPtr->dbgFunc == NULL)
			{
				printf("%s", defaultStr);
			}
		}
    }
}

#ifdef VXWORKS

static void pciDeviceHeaderShow(uint32 arg1, uint32 arg2)
{
	int32	busNo, deviceNo, funcNo;
   	int8    line[16];
	
	printf( "Enter Bus NO : ");
   	ioRead(gDebugPortFd, line, sizeof(line));
	busNo = atoi(line);
		
	printf( "Enter Device NO : ");
   	ioRead(gDebugPortFd, line, sizeof(line));
	deviceNo = atoi(line);

	printf( "Enter Func NO : ");
   	ioRead(gDebugPortFd, line, sizeof(line));
	funcNo = atoi(line);

	printf("[BUS NO :: %d]-[DEVICE NO :: %d]-[FUNC NO :: %d]\n", 
				busNo, deviceNo, funcNo);
#ifdef VXWORKS
	if(pciHeaderShow(busNo, deviceNo, funcNo))
		printf( "[ERROR] - PCI HEADER SHOW ERROR !!\n");
#endif
	return;		
}



#if (BOARD_TYPE == MVME2100_BOARD)
static void univerRegisterDataDisplay(void)
{
	uint32 *basePtr = (uint32 *)univBaseAdrs;
	uint32	offSet, len;
	uint8	*addrPtr;

	addrPtr = (uint8 *)basePtr;

   	printf( "Enter Offset Address(HEX) : ");
	scanf("%x", &offSet);
    addrPtr += offSet;

  	printf( "   Enter Data Length(INT) : ");
	scanf("%d", &len);

	dataDisplay ( "UNIVERSE DATA", (uint8 *)addrPtr, len);	
	return;
}
#endif


#if (BOARD_TYPE == MVME2100_BOARD)
	extern uint32 univBaseAdrs;
	extern uint32 sysPciInLong(uint32);
	extern void sysPciOutLong(uint32,uint32);

static void univerRegisterDataWrite(void)
{
	uint32 offSet, data = 0;
	uint32 univData, saveData, addr;

   	printf( "Enter Offset Address(HEX) : ");
	scanf("%x", &offSet);

  	printf( "   Enter Write Data (HEX) : ");
	scanf("%x", &data);

    addr = univBaseAdrs + offSet;
	univData = sysPciInLong (addr);

	saveData = univData;
	
    univData |= data;

	sysPciOutLong (addr, univData);

	univData = sysPciInLong (addr);

    printf( "[WRITE ADDR(0x%08x)]-[PrevData(0x%08x)]-[CurData(0x%08x)] !!\n", 
							addr, saveData, univData);
	return;
}
#endif

#endif 


static void busInterruptGen(void)
{
#ifdef VXWORKS
	uint8 *intrKind[] = {
    	"********************************",
		"*  0 - Exit",
        "*  1 - Bus Error Intrrupt Gen",
    	"********************************",
        NULL
    };
    uint8 **pPtr;
	uint32	maxNum = 1;
    uint32	index;
    int8    line[16], *pLine;
	uint32	data = 0, intrData, offSet = 0;

	intrData = 0;
	
	for(;;) {
    	printf( "\n");
		for(pPtr = intrKind; *pPtr != NULL; pPtr++)
        	printf( "\t%s\n",*pPtr);
		 
    	printf( "\n\n");
        printf("\t Select Number(0 ~ %d) : ", maxNum);

        ioRead(gDebugPortFd, line, sizeof(line));

        pLine = line;

        index = atoi(pLine);

        if(index > maxNum) {
			printf( "\n\t Invalid Number [%d] !!!\n", index);
            continue;
        }
        if(index == 0)
        	return;
		else if(index == 1) { 
			data = 1 << 10;
            offSet = 0x304;
        }

#if (BOARD_TYPE == MVME2100_BOARD)
        intrData = sysPciInLong( univBaseAdrs + offSet);
	
		intrData |= data;
		
		sysPciOutLong( (univBaseAdrs + offSet), intrData); 
#endif
    }
#endif
}

#if (BOARD_TYPE == MVME3100_BOARD)
#include    <wrn\coreip\net\if_var.h>

static void netIfUnitDataDisplay (void)
{
    int8    line[16], *pLine;
	uint8	ifName[16], unit;
	struct ifnet	*ifp = NULL;	
	
	printf("\nSelect Network Interface Unit ? (1.Board,2.PMC682E#0,3.PMC682E#1, 4.PMC661J#0)");
	ioRead(gDebugPortFd, line, sizeof(line));
    pLine = line;

    switch(*(pLine)) {
        case '1':
	        sprintf(ifName,"%s",NET_DEV_NAME0); 
        break;
        case '2':
	        unit = 0;
	        sprintf(ifName,"%s%d",PMC682E_DEV_NAME, unit); 
        break;
        case '3':
	        unit = 1;
	        sprintf(ifName,"%s%d",PMC682E_DEV_NAME, unit); 
        break;
        case '4':
	        unit = 0;
	        sprintf(ifName,"%s%d",PMC661J_DEV_NAME, unit); 
        break;
		default:
			printf("\t[ERROR]-Unknown Network Interface Unit !!\n");
		break; 
	}
	
	ifp = ifunit(ifName);
	
	if(ifp == NULL) {
		printf("\t[ERROR]-Network Interface Information Pointer NULL !!\n");
		return;
	}
	printf("\t******************************************************\n");
	printf("\t**  Network Interface %s Information \n", ifName);
	printf("\t******************************************************\n");
	printf("\tMax Send Queue Len : %d\n", ifp->if_snd.ifq_maxlen);	
	printf("\tCur Send Queue Len : %d\n", ifp->if_snd.ifq_len);	
	printf("\tCur Intr Pending   : %d\n", ifp->if_ipending);	
	printf("\tCur Queue Drops    : %d\n", ifp->if_snd.ifq_drops);	
	printf("\tNetwork Flags      : 0x%x\n", ifp->if_flags);	
	printf("\tWatchDog Timer     : %d\n", ifp->if_timer);	
	printf("\n");
	
	
	return;
}
#endif
#ifdef _WIN32
uint32 taskExecutionTime ()  
{ 

    HANDLE        hThreadSnap = NULL; 
    BOOL          bRet        = FALSE; 
    THREADENTRY32 te32        = {0};
	FILETIME	CreationTime, ExitTime, KernelTime, UserTime;
	SYSTEMTIME	sysTypeKernelTime,sysTypeUserTime;
	int	i=0, taskNum = 0, taskIdFlag  = 0;

	DWORD dwOwnerPID = GetCurrentProcessId();

#ifdef _WINCE
	uint32	timeOut = 1;
	uint32	isOkFlag;
	uint32	startTick, stopTick, idleSt, idleEd;
	float	percentIdle;
	if( ((isOkFlag=GetIdleTime()) != MAXDWORD) ) {
		printf( "Waiting Moment[%d Sec] To Calculate the Idle Time ...",timeOut);
		startTick = GetTickCount();
		idleSt = GetIdleTime();
		sysTaskDelay(systemClockRateGet() * timeOut);
		stopTick = GetTickCount();
		idleEd = GetIdleTime();
		percentIdle = (float)((100*(idleEd - idleSt)) / (stopTick - startTick));
		printf( "\n\tThe Percent of Idle time is %f(%)\n\n",percentIdle);
	}
#endif
	memoryClear((uint8*)&CreationTime,sizeof(FILETIME));
	memoryClear((uint8*)&ExitTime,sizeof(FILETIME));
	memoryClear((uint8*)&KernelTime,sizeof(FILETIME));
	memoryClear((uint8*)&UserTime,sizeof(FILETIME));

    
    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0); 
    if (hThreadSnap == INVALID_HANDLE_VALUE) {
		printf( "Invalid Task Execution Infomation Handle[ErrCode=0x%08x]\n",GetLastError());
        return (FALSE);
	}
 
    

    te32.dwSize = sizeof(THREADENTRY32); 
	
    
    
    
	
	
	printf( "\n    NAME         ENTRY        TID       PRI     Kernel Time      User Time\n");
	printf(   " ---------     ---------   ----------   ---   ---------------  -------------\n");

     if (Thread32First(hThreadSnap, &te32)) 
    { 
        do 
        { 
            if (te32.th32OwnerProcessID == dwOwnerPID) 
            { 
				taskIdFlag = 0;

				for(i=0;i<MAX_TASK_LIST;i++) {
					if ((int32)te32.th32ThreadID == (int32)gTaskList[i].TaskId) {
						taskIdFlag = 1;
						break;
					}
				}

				if (taskIdFlag == 1)
				{
					printf( "%10s", gTaskList[i].td_name);
					printf( "    %10s", gTaskList[i].td_name);

  					printf( "   0x%08X", te32.th32ThreadID);
				    printf( "   %3d", te32.tpBasePri); 

					GetThreadTimes ((HANDLE)te32.th32ThreadID, &CreationTime, &ExitTime, &KernelTime, &UserTime);
					FileTimeToSystemTime (&KernelTime,&sysTypeKernelTime);
					printf( "\t%d:%d:%.3f",
											sysTypeKernelTime.wHour,
											sysTypeKernelTime.wMinute,
											(double)sysTypeKernelTime.wSecond + (double)sysTypeKernelTime.wMilliseconds/1000.0); 
					FileTimeToSystemTime (&UserTime,&sysTypeUserTime);
					printf( "\t%d:%d:%.3f",
											sysTypeUserTime.wHour,
											sysTypeUserTime.wMinute,
											(double)sysTypeUserTime.wSecond + (double)sysTypeUserTime.wMilliseconds/1000.0);					
				}
				else {
  					printf( "\t\t\t   0x%08X", te32.th32ThreadID);
		            printf( "   %3d", te32.tpBasePri); 
				}
		        printf( "\n"); 
            } 
        } 
        while (Thread32Next(hThreadSnap, &te32)); 
        bRet = TRUE; 
    } 
    else taskNum = 0;
        bRet = FALSE;          
 
    
    
#ifdef _WINCE
	CloseToolhelp32Snapshot(hThreadSnap); 
#endif
#ifdef _WINNT
	CloseHandle(hThreadSnap); 
#endif
    printf( "\n"); 



    return (bRet); 
}
#endif

#ifdef _WIN32
static void targetDebugHelpCommand(void)
{
    int8    **pMsg;
    int8 *helpMsg[] =
    {
        "[?,h,H]",  "- Print below command list.",
        "[q,Q]",	"- Exit User Test Debugger",
		"[fdi,FDI]","- FLASH Disk Info Display",
		"[rbt,RBT]","- Rebooting (HARD Reset)",
		"[rsr,RSR]","- Remote Shelf Reset",
		"[efl,EFL]","- Engineering Source File(*.Zip)List Display",
		"[rom,ROM]","- Rom Image File(cPCI3720AT.123)List Display",
		"[dbm,DBM]","- Text File(*.dbm)List Display",	
        NULL
    };

    printf( "\n");

    for(pMsg = helpMsg; *pMsg != NULL; pMsg += 2)
        printf( "\t%-15s %s\n", *pMsg, *(pMsg+1));
	return;
}


static void userTargetDebugger(void)
{
    int8    line[16], *pLine;
    static int8    debugPrompt[] = "[UTS_BUG]> ";
    static int8    defaultStr[] = "Unrecognized Command. Type '?','h','H' for Help\n";

	uint32  retVal = NO_ERROR;
	strSysExecInfo *sysExeInfoPtr;	

    while(1L) {

        printf(debugPrompt);
        ioRead(gDebugPortFd, line, sizeof(line));

        pLine = line;

        switch(*(pLine)) {
            case ENDOFSTR:
            break;
            case '?':case 'h':case 'H':
				targetDebugHelpCommand();
                printf( "\n");
			break;
			case 'q': case 'Q':
            	pLine++;
                if(*pLine == ENDOFSTR){
                	printf("\nReturn To Main Debugger !!!\n");
					return;
				}
                else
	                printf("%s",defaultStr);
            break;
			case 'c': case 'C':
            	pLine++;
                if(*pLine == ENDOFSTR){
					retVal =  fbSysExeInfoPointerGet(&sysExeInfoPtr, LOCAL_MEM_ACC);
					if(retVal == NO_ERROR){
						sysExeInfoPtr->mpuSysExeInfo.fbLogicCompileTime = 0;
						printf("\nMPU Version Info Clear Sucess!!!\n");
					}
					return;
				}
                else
	                printf("%s",defaultStr);
            break;

#ifdef _WIN32		
			case 'f': case 'F':
			 	pLine++;
				if( (*pLine == 'd')|| (*pLine == 'D')) {
                	pLine++;
					if( (*pLine == 'i')|| (*pLine == 'I')) {
					   	pLine++;
	                    if(*pLine == ENDOFSTR)
								flashDiskinfo();
						else  printf( "%s", defaultStr);
					}
				    else printf( "%s", defaultStr);
				}
            break;
#endif
			case 'v': case 'V':
			 	pLine++;
                if( (*pLine == 'r')|| (*pLine == 'R')) {
                	pLine++;
                    if( (*pLine == 'w')|| (*pLine == 'W')) {
	                	pLine++;
	                    if(*pLine == ENDOFSTR)
							variableMemoryTest();
						else  printf( "%s", defaultStr);
                    }
                    else
                    	printf( "%s", defaultStr);
                }
                else
                	printf( "%s",defaultStr);
            break;
			
			case 'r': case 'R':
			 	pLine++;
				if( (*pLine == 'b')|| (*pLine == 'B')) {
                	pLine++;
					if( (*pLine == 't')|| (*pLine == 'T')) {
					   	pLine++;
	                    if(*pLine == ENDOFSTR)
								boardReboot();   
					    else  printf( "%s", defaultStr);
					}
				    else printf( "%s", defaultStr);
				}
#ifdef _WIN32
				else if( (*pLine == 'o')|| (*pLine == 'O')) {
					pLine++;
					if( (*pLine == 'm')|| (*pLine == 'M')) {
					   	pLine++;
	                    if(*pLine == ENDOFSTR)
						    	filelistDisplay(FILE_KIND_ROMIMG);
							
					    else  printf( "%s", defaultStr);
					}
				    else printf( "%s", defaultStr);	
				}
#endif	      
				else  printf("%s",defaultStr);
            break;
	
#ifdef _WIN32
			
			case 'e':case 'E':
		      	pLine++;
				if( (*pLine == 'f') || (*pLine == 'F')) {
                	pLine++;
                    if( (*pLine == 'l')|| (*pLine == 'L')) {
	                	pLine++;
	                    if(*pLine == ENDOFSTR)
                        (void)filelistDisplay(FILE_KIND_ENGSRC);		
						else  printf( "%s", defaultStr);
                    }
					else
						printf("%s", defaultStr);

				}
				else printf( "%s", defaultStr);
			break;
			
			case 'd':case 'D':
		      	pLine++;
				if( (*pLine == 'b') || (*pLine == 'B')) {
                	pLine++;
                    if( (*pLine == 'm')|| (*pLine == 'M')) {
	                	pLine++;
	                    if(*pLine == ENDOFSTR)
                        (void)filelistDisplay(FILE_KIND_PBENG_FILE);		
						else  printf( "%s", defaultStr);
                    }
					else
						printf("%s", defaultStr);

				}
				else printf( "%s", defaultStr);
			break;
#endif
		    default:
                printf("%s",defaultStr);
            break;
		}
	}
}

#define SHELL_GETMOD  0x0001
#define SHELL_GETPROC 0x0002
#define SHELL_GETTHRD 0x0004

#define NUM_THRDSTATES 8
const char rgszStates[NUM_THRDSTATES][16] = {
    "Runing ", 
    "Runabl ", 
    "Blockd ", 
    "Suspnd ", 
    "Sleepg ", 
    "Sl/Blk ", 
    "Sl/Spd ", 
    "S/S/Bk ", 
};

INT cNumProc;
char   rgbText[512];

typedef struct _THRD_INFO {
    struct _THRD_INFO   *pNext;
    DWORD   dwThreadID;
    DWORD   dwFlags;
#define THRD_INFO_FOUND 0x00000001
    FILETIME    ftKernThrdTime;
    FILETIME    ftUserThrdTime;
} THRD_INFO, *PTHRD_INFO;

struct {
    DWORD   dwProcid;
    DWORD   dwZone;
    DWORD   dwAccess;
    PTHRD_INFO  pThrdInfo;
} rgProcData[32];

#define MAX_MODULES 200

struct {
    DWORD   dwModid;
    DWORD   dwZone;
} rgModData[MAX_MODULES];
INT cNumMod;

#define ARRAYSIZE(a)    (sizeof (a) / sizeof ((a)[0]))

BOOL IsPatMatch(TCHAR *pszPat, TCHAR *pszText)
{
    BOOL fRet;
    TCHAR szText[64];       

    if (!pszPat)
        return TRUE;

    _tcsncpy(szText, pszText, ARRAYSIZE(szText));
    szText[ARRAYSIZE(szText) - 1] = 0;
    _tcslwr(szText);
    fRet = _tcsstr(szText, pszPat) != NULL;

    return fRet;
}


BOOL
IsModInList(
    DWORD dwModid
    )
{
    INT i;

    for (i = 0; i < MAX_MODULES; i++) {
        if (0 == rgModData[i].dwModid) {
            return FALSE;
        }
        if (dwModid == rgModData[i].dwModid) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL
GetModuleInfo(
    HANDLE hSnap,
    MODULEENTRY32 * mod,
    DWORD dwModid
    )
{
    mod->dwSize = sizeof(*mod);
    if (Module32First(hSnap,mod)) {
        do {
            if (dwModid == mod->th32ModuleID) {
                return TRUE;
            }
        } while (Module32Next(hSnap,mod));
    }
    return FALSE;
}

BOOL DoGetInfo (void) 
{
#ifdef _WINCE
    DWORD   dwMask = 0xFFFFFFFF;
    HANDLE hSnap;
    PROCESSENTRY32 proc;
    MODULEENTRY32 mod;
    THREADENTRY32 thread;
    TCHAR   *pszArg = NULL;
    BOOL    fMatch;
    BOOL    fDelta = FALSE;
    INT     i;
    WCHAR   szLastMod[MAX_PATH];
    BOOL    fSwapped;
	char    fileName[MAX_PATH];
	int     fileLenth = 0;

	dwMask = 0xFFFFFFFF;

    if (dwMask & SHELL_GETPROC) {
        hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPNOHEAPS | (dwMask & SHELL_GETTHRD ? TH32CS_SNAPTHREAD : 0),0);
        if (INVALID_HANDLE_VALUE != hSnap) {
            proc.dwSize = sizeof(proc);
            if (Process32First(hSnap,&proc)) {
                cNumProc = 0;
                printf("PROC: Name            hProcess: CurAKY :dwVMBase:CurZone\r\n");
                if (dwMask & SHELL_GETTHRD) {
                    printf("THRD: State :hCurThrd:hCurProc: CurAKY :Cp :Bp :Kernel Time  User Time\r\n");
                }
                do {
                    fMatch = IsPatMatch(pszArg, proc.szExeFile);
                    rgProcData[cNumProc].dwProcid = proc.th32ProcessID;
                    rgProcData[cNumProc].dwAccess = proc.th32AccessKey;
                    rgProcData[cNumProc++].dwZone = proc.dwFlags;
                    if (fMatch) {

						fileLenth = wcslen(&proc.szExeFile[0]);
						memset(fileName,0,MAX_PATH);
						WideCharToMultiByte(	CP_ACP, (DWORD)0,
												(LPCWSTR)&proc.szExeFile[0],(int)fileLenth,
												(LPSTR)fileName, (int)fileLenth,
									(LPCSTR)NULL,(LPBOOL)NULL);

						printf(" P%2.2d: %-15s %8.8lx %8.8lx %8.8lx %8.8lx\r\n",cNumProc,
												fileName,proc.th32ProcessID,proc.th32AccessKey,proc.th32MemoryBase,proc.dwFlags);
                    } else {
                        continue;
                    }
                    if (dwMask & SHELL_GETTHRD) {
                        PTHRD_INFO  pThrdInfo, pThrdInfoLast;
                        
                        thread.dwSize = sizeof(thread);

                        for (pThrdInfo=rgProcData[cNumProc-1].pThrdInfo; pThrdInfo; pThrdInfo=pThrdInfo->pNext) {
                            pThrdInfo->dwFlags &= ~(THRD_INFO_FOUND);
                        }
                        
                        if (Thread32First(hSnap,&thread)) {                            
                            DWORD dwAccess;
                            dwAccess = SetProcPermissions((DWORD)-1);
                            do {
                                if (thread.th32OwnerProcessID == proc.th32ProcessID) {
                                    FILETIME f1,f2,f3,f4;
                                    __int64 hrs, mins, secs, ms;

                                    for (pThrdInfo=rgProcData[cNumProc-1].pThrdInfo; pThrdInfo; pThrdInfo=pThrdInfo->pNext) {
                                        if (pThrdInfo->dwThreadID == (DWORD)thread.th32ThreadID) {
                                            break;
                                        }
                                    }
                                    if (NULL == pThrdInfo) {
                                        pThrdInfo = (PTHRD_INFO)LocalAlloc (LPTR, sizeof(THRD_INFO));
                                        if (pThrdInfo) {
                                            pThrdInfo->dwThreadID = (DWORD)thread.th32ThreadID;
                                            pThrdInfo->pNext = rgProcData[cNumProc-1].pThrdInfo;
                                            rgProcData[cNumProc-1].pThrdInfo = pThrdInfo;
                                        }
                                    }
                                    if (pThrdInfo) {
                                        pThrdInfo->dwFlags |= THRD_INFO_FOUND;
                                    }
                                    
                                    sprintf(rgbText," T    %6.6s %8.8lx %8.8lx %8.8lx %3d %3d",
                                        rgszStates[thread.dwFlags],thread.th32ThreadID,thread.th32CurrentProcessID,thread.th32AccessKey,
                                        thread.tpBasePri-thread.tpDeltaPri,thread.tpBasePri);
                                    if (GetThreadTimes((HANDLE)thread.th32ThreadID,&f1,&f2,&f3,&f4)) {
                                        if (pThrdInfo) {
                                            *(__int64 *)&f1 = *(__int64 *)&f3;
                                            *(__int64 *)&f2 = *(__int64 *)&f4;
                                            if (fDelta) {
                                                *(__int64 *)&f3 -= *(__int64 *)&(pThrdInfo->ftKernThrdTime);
                                                *(__int64 *)&f4 -= *(__int64 *)&(pThrdInfo->ftUserThrdTime);
                                            }

                                            *(__int64 *)&(pThrdInfo->ftKernThrdTime) = *(__int64 *)&f1;
                                            *(__int64 *)&(pThrdInfo->ftUserThrdTime) = *(__int64 *)&f2;
                                            if (fDelta && (0 == *(__int64 *)&f3) && (0 == *(__int64 *)&f4)) {
                                                continue;
                                            }
                                        }
                                        *(__int64 *)&f3 /= 10000;
                                        ms = *(__int64 *)&f3 % 1000;
                                        *(__int64 *)&f3 /= 1000;
                                        secs = *(__int64 *)&f3 % 60;
                                        *(__int64 *)&f3 /= 60;
                                        mins = *(__int64 *)&f3 % 60;
                                        *(__int64 *)&f3 /= 60;
                                        hrs = *(__int64 *)&f3;
                                        sprintf(rgbText + strlen(rgbText), " %2.2d:%2.2d:%2.2d.%3.3d", 
                                            (DWORD)hrs, (DWORD)mins, (DWORD)secs, (DWORD)ms);
                                        *(__int64 *)&f4 /= 10000;
                                        ms = *(__int64 *)&f4 % 1000;
                                        *(__int64 *)&f4 /= 1000;
                                        secs = *(__int64 *)&f4 % 60;
                                        *(__int64 *)&f4 /= 60;
                                        mins = *(__int64 *)&f4 % 60;
                                        *(__int64 *)&f4 /= 60;
                                        hrs = *(__int64 *)&f4;
                                        sprintf(rgbText + strlen(rgbText), " %2.2d:%2.2d:%2.2d.%3.3d\r\n", 
                                            (DWORD)hrs, (DWORD)mins, (DWORD)secs, (DWORD)ms);
                                    } else {
                                        sprintf(rgbText + strlen(rgbText), " No Thread Time\r\n");
                                    }
                                    printf("%s",rgbText);
                                }
                            } while (Thread32Next(hSnap,&thread));
                            SetProcPermissions(dwAccess);
                        }
                        pThrdInfoLast = NULL;
                        for (pThrdInfo=rgProcData[cNumProc-1].pThrdInfo; pThrdInfo; ) {
                            if (!(pThrdInfo->dwFlags & THRD_INFO_FOUND)) {
                                if (pThrdInfoLast) {
                                    pThrdInfoLast->pNext = pThrdInfo->pNext;
                                } else {
                                    rgProcData[cNumProc-1].pThrdInfo = pThrdInfo->pNext;
                                }
                                LocalFree (pThrdInfo);
                                if (pThrdInfoLast) {
                                    pThrdInfo = pThrdInfoLast->pNext;
                                } else {
                                    pThrdInfo = rgProcData[cNumProc-1].pThrdInfo;
                                }
                                
                            } else {
                                pThrdInfoLast = pThrdInfo;
                                pThrdInfo = pThrdInfo->pNext;
                            }
                        }

                    }
                } while (Process32Next(hSnap,&proc));
                if (dwMask & SHELL_GETMOD)
                    if (fMatch) {
                        printf("%s","\r\n");
                    }
            }
            CloseToolhelp32Snapshot(hSnap);
        } else {
            printf("Unable to obtain process/thread snapshot (%d)!\r\n",GetLastError());
        }
    }
    if (0) {
        hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_GETALLMODS,0);
        if (INVALID_HANDLE_VALUE != hSnap) {
            mod.dwSize = sizeof(mod);
            memset(rgModData, 0, sizeof(rgModData));
            cNumMod = 0;
            fSwapped = TRUE;

            while (fSwapped) {
                fSwapped = FALSE;
                szLastMod[0] = 0;
                if (Module32First(hSnap,&mod)) {
                    do {
                        if (!IsModInList(mod.th32ModuleID)) {
                            if (0 == szLastMod[0]) {
                                rgModData[cNumMod].dwModid = mod.th32ModuleID;
                                rgModData[cNumMod].dwZone = mod.dwFlags;
                                wcscpy(szLastMod, mod.szModule);
                                fSwapped = TRUE;
                            } else {
                                if (0 > wcsicmp(mod.szModule, szLastMod)) {
                                    fSwapped = TRUE;
                                    rgModData[cNumMod].dwModid = mod.th32ModuleID;
                                    rgModData[cNumMod].dwZone = mod.dwFlags;
                                    wcscpy(szLastMod, mod.szModule);
                                }
                            }
                        }
                    } while (Module32Next(hSnap,&mod));
                }

                if (fSwapped) {
                    cNumMod++;
                    if (MAX_MODULES == cNumMod) {
                        break;
                    }
                }
            }

            printf(" MOD: Name            pModule :dwInUSE :dwVMBase:CurZone\r\n");
            for (i = 0; i < cNumMod; i++) {
                if (GetModuleInfo(hSnap, &mod, rgModData[i].dwModid)) {
                    fMatch = IsPatMatch(pszArg, mod.szModule);
                    if (fMatch) {
						fileLenth = wcslen(&mod.szModule[0]);
						memset(fileName,0,MAX_PATH);
						WideCharToMultiByte(	CP_ACP, (DWORD)0,
												(LPCWSTR)&mod.szModule[0],(int)fileLenth,
												(LPSTR)fileName, (int)fileLenth,
									(LPCSTR)NULL,(LPBOOL)NULL);

                        printf(" M%2.2d: %-15s %8.8lx %8.8lx %8.8lx %8.8lx\r\n",i,
                                fileName,mod.th32ModuleID,mod.ProccntUsage,
                                (DWORD)(mod.modBaseAddr),mod.dwFlags);
                    }
                }
            }

            CloseToolhelp32Snapshot(hSnap);

        } else {
            printf("Unable to obtain process/thread snapshot (%d)!\r\n",GetLastError());
        }
    }
#endif
    return TRUE;
}

#endif

/**************************************************************************
**
**  Function 이름   : cpuBrdRebootByManual
**
**  개요            : Debugger Cmd 창에서 Manual로 CPU Board Reboot
**
**************************************************************************/
void cpuBrdRebootByManual(uint32 arg1, uint32 arg2)
{
	int8    YorN[2], *cPtr;
	
	(void)fdprintf(gDebugPortFd, "\nBoard Reboot ?[Y/N] : ");
	(void)ioRead(gDebugPortFd, YorN, 2);
	cPtr = YorN;
	
	if((*cPtr == 'Y') || (*cPtr == 'y'))
	{
		(void)fdprintf(gDebugPortFd, "Board Rebootting ....\n");
		sysTaskDelay(systemClockRateGet());
		boardReboot();
	}
	
	return;
}

static void uniRcvMsgHeadDataDisplay(uint32 arg1, uint32 arg2)
{
	transDataDetailInfoDisplay( DETAIL_DATA_FOR_RECV, UNICAST_HEAD_PRINT);

	return;
}

static void uniRcvMsgDataDisplay(uint32 arg1, uint32 arg2)
{
	transDataDetailInfoDisplay( DETAIL_DATA_FOR_RECV, UNICAST_DETAIL_DATA_PRINT);

	return;
}

static void brodRcvMsgHeadDataDisplay(uint32 arg1, uint32 arg2)
{
	transDataDetailInfoDisplay( DETAIL_DATA_FOR_RECV, BROADCAST_HEAD_PRINT);

	return;
}

static void brodRcvMsgDataDisplay(uint32 arg1, uint32 arg2)
{
	transDataDetailInfoDisplay( DETAIL_DATA_FOR_RECV, BROADCAST_DETAIL_DATA_PRINT);

	return;
}

static void uniSendMsgHeadDataDisplay(uint32 arg1, uint32 arg2)
{
	transDataDetailInfoDisplay( DETAIL_DATA_FOR_SEND, UNICAST_HEAD_PRINT);

	return;
}

static void uniSendMsgDataDisplay(uint32 arg1, uint32 arg2)
{
	transDataDetailInfoDisplay( DETAIL_DATA_FOR_SEND, UNICAST_DETAIL_DATA_PRINT);

	return;
}

static void brodSendMsgHeadDataDisplay(uint32 arg1, uint32 arg2)
{
	transDataDetailInfoDisplay( DETAIL_DATA_FOR_SEND, BROADCAST_HEAD_PRINT);

	return;
}

static void brodSendMsgDataDisplay(uint32 arg1, uint32 arg2)
{
	transDataDetailInfoDisplay( DETAIL_DATA_FOR_SEND, BROADCAST_DETAIL_DATA_PRINT);

	return;
}

static void boardNetIfShow(uint32 arg1, uint32 arg2)
{
	ifShow(NULL);
	
	return;	
}

static void rcvTransDataInfoDisplay(uint32 arg1, uint32 arg2)
{
	transDataInfoDisplay(gDebugRecvData, "Receiving");
	
	return;	
}

static void sndTransDataInfoDisplay(uint32 arg1, uint32 arg2)
{
	transDataInfoDisplay(gDebugSendData, "Sending");
	
	return;	
}

