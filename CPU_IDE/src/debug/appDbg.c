
#include	<stdio.h>
#include	<string.h>
#include	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"sysConfig.h"
#include	"pcsDefs.h"
#include	"network.h"
#include	"usrQueData.h"
#include	"msgQueData.h"
#include	"usrSema.h"
#include	"debugInfo.h"
#include	"lkv120Board.h"
#include	"osDepend.h"
#include	"netProtocol.h"
#include	"usrErrCode.h"
#include	"cmdList.h"
#include	"fbMem.h"
#include	"systemDiag.h"
#include	"segScheduler.h"
#include	"usrDefBuf.h"

#include	"segScheduler.h"


#include    "fbForcing.h"
#include    "sysDual.h"
#include    "sfcMem.h"
#include    "taskDefs.h"
#include    "protocolBuf.h"


static strErrorInfo			sErrInfo[MAX_ERR_QUEUE_NUM];
strErrQueMngrInfo		sErrQInfo;

static strDetailErrorInfo	sDetailErrInfo[MAX_ERR_QUEUE_NUM];
strDetailErrQueMngrInfo	sDetailErrQInfo;

strDebugDataInfo gDebugRecvData[DEBUG_INFO_MAX_ARRAY_NUM];
strDebugDataInfo gDebugSendData[DEBUG_INFO_MAX_ARRAY_NUM];
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
strDebugDataInfo gDualRecvData[DEBUG_INFO_MAX_ARRAY_NUM];
strDebugDataInfo gDualSendData[DEBUG_INFO_MAX_ARRAY_NUM];
#endif

strTransDetailInfo  sTransDetailData[TRANS_DETAIL_INFO_QUE_NUM];
strTransDetailQueMngrInfo   sTransQueMngrInfo;

int32	sErrQSemaId;
static int32   sTransMsgDbgSemaId;


strNetworkPerInfo	sNetPerTransInfo;



uint32	gDebugErrStringFlag = DEBUG_DISABLE;
static uint32	sISRDebugErrStringFlag;


uint32 gRecvInfoFlag;
uint32 gSendInfoFlag;


uint32 gTransBroadcastDetailRecvFlag;
uint32 gTransUnicastDetailRecvFlag;
uint32 gTransBroadcastDetailSendFlag;
uint32 gTransUnicastDetailSendFlag;


uint32	gTransRateCalcuFlag;
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
uint32 gDualBroadcastDetailRecvFlag;
uint32 gDualUnicastDetailRecvFlag;
uint32 gDualBroadcastDetailSendFlag;
uint32 gDualUnicastDetailSendFlag;


uint32  gDualTransRateCalcuFlag;	
uint32  gDualRecvInfoFlag;
uint32  gDualSendInfoFlag;
#endif

uint32  gUsrTaskSchedulingFlag;
uint32	gMpuSegmentSchedulingFlag;

#ifdef VXWORKS
uint32	sDebugErrStringPortId = 100;
#else
uint32	sDebugErrStringPortId = 0;
#endif


uint32	gRmtDevSendDisplayFlag = DEBUG_DISABLE;
uint32	gRmtDevRecvDisplayFlag = DEBUG_DISABLE;


#ifdef _WIN32

static strFbMemInfo		*gMpuDebugLocalFbMemInfoPtr = NULL,*gMpuDebugRemoteFbMemInfoPtr = NULL;
static strFbMemInfo		*gDiuDebugLocalFbMemInfoPtr = NULL,*gDiuDebugRemoteFbMemInfoPtr = NULL;


static uint32		*gMpuDebugLocalSpecAndVarDataPtr,*gMpuDebugRemoteSpecAndVarDataPtr;
static uint32		*gDiuDebugLocalSpecAndVarDataPtr,*gDiuDebugRemoteSpecAndVarDataPtr;


static uint32		*gMpuDebugLocalOutputDataPtr,*gMpuDebugRemoteOutputDataPtr;
static uint32		*gDiuDebugLocalOutputDataPtr,*gDiuDebugRemoteOutputDataPtr;


static strFbTaskTblMemInfo	*gMpuDebugLocalFbSegInfoPtr,*gMpuDebugRemoteFbSegInfoPtr;
static strFbTaskTblMemInfo	*gDiuDebugLocalFbSegInfoPtr,*gDiuDebugRemoteFbSegInfoPtr;


static uint32	*gDebugLocalSTSharedMemDataPtr,*gDebugRemoteSTSharedMemDataPtr;


static uint32	*gDebugLocalVarInfoPtr,*gDebugRemoteVarInfoPtr;


static uint32	*gDebugLocalSFCInfoPtr,*gDebugRemoteSFCInfoPtr;


static uint32	*gDebugLocalSysExeInfoPtr,*gDebugRemoteSysExeInfoPtr;


extern strFbMemInfo		*gMpuFbSegTblInfoLocalPtr,*gMpuFbSegTblInfoBusPtr;
extern strFbMemInfo		*gDiuFbSegTblInfoLocalPtr,*gDiuFbSegTblInfoBusPtr;
#endif


extern int32 gDebugPortFd;
#ifdef _WIN32
extern strSysRunningConfigInfo		*gSysRunningInfoPtr;
extern strMpuDiuRunningStatusInfo 	*gDiuRasInfoPtr;
#endif

uint32 gMvme3100Tsec1QHaltErrCount = 0;
uint32 gMpc85xxTsec1THaltErrCount = 0;
uint32 gMpc85xxTsec1PrevRecvPackets = 0;
uint32 gMvme4100Tsec1LinkReloadCount = 0;
uint32 gMvme4100Tsec1LinkReloadErrCount = 0;


uint32 debugQueueInit(void);
uint32 debugVarInit(void);
void setErrorCode(uint8 *,uint32,uint8 *,uint32);
void setErrorCodeWithPort(uint8 *,uint32,uint8 *,uint32, uint32);
void setErrorCodeWithVal(uint8 *,uint32,uint8 *,uint32, uint8 *,uint32,
							uint8 *,uint32,uint8 *,uint32);
void systemInitErr(uint8 *,uint32,uint8 *,uint32);
void systemInitErrWithVal(uint8 *,uint32,uint8 *,uint32, uint8 *,uint32,
							uint8 *,uint32,uint8 *,uint32);
void errorCodeSetInISR(uint8 *,uint32,uint8 *,uint32);
void errorStringDisplay(uint8 *,uint32,uint8 *,uint32);
static void detailErrorStringDisplay(uint8 *,uint32,uint8 *,uint32, uint8 *,uint32,
							uint8 *,uint32,uint8 *,uint32);
void appErrStringMonitoring (uint32, uint32);
void errorQueueClear(void);
void errQAccessAndDisplay(void);
void errorQueueCountDisplay(void);
void debugCountSet(uint32 *);
void debugValSet(uint32 *, uint32);
static uint32 debugInfoCheck(strMsgQueData *, strDebugDataInfo *);
void debugInfoSet(strMsgQueData *, strDebugDataInfo *);

void transDataInfoDisplay(strDebugDataInfo *, uint8 *);

static void commandStringGet(strCommandListInfo *, uint32 , uint32 );
static void transDetailInfoPrint(strTransDetailInfo *,uint8 *,uint8,uint8);
void transDataDetailInfoDisplay(uint8, uint8);
static void transInfoQueClear(void);
static uint32  transInfoGet(strTransDetailInfo *);
uint32  transInfoSet( strMsgQueData *);
static uint32 transInfoMngrInit(void);
void networkPerformanceDisplay(uint32, uint32);
void networkPerformanceCountSet(uint32 , uint32 );
void fatalErrStringDisplay(int8 *, uint32,uint8 *, uint32 ,uint32, uint8 *, uint32 );
void debugMsgDisplayInISR(	uint8 *, uint8 *, uint8 *, uint32, uint32, uint32,uint32,uint32);
void dataDisplayForRmtDevice(uint32, uint32);
void networkDebugVarClear(uint32, uint32);
static void cmdTransDataInfoPrint(strTransDetailInfo *, uint32, uint32 , uint32);

#ifdef _WIN32

static uint32	debugLocalFbMemoryAddrInit(void);
static uint32	debugRemoteFbMemoryAddrInit(void);
static uint32 segTblInfoReadForDebugging(uint32,uint32,strFbTaskTblMemInfo *, uint32);
static uint32 fbMemInfoReadForDebugging(uint32, uint32, strFbMemInfo *, uint32);
static uint32 dataCompare(uint32 *, uint32*, uint32 );

static uint32 debugDataCopyFromLocalRamOrBusMemoryBoard(uint32 , uint32	);
static uint32 compareSegmentTableInfo(uint32 , strModuleInfo *);
static uint32 compareFbDataInfo(uint32 , strModuleInfo *);
static void dataCompareBetweenMemory(uint32 , uint32 );
extern uint32	myBusDataAcc(uint8 *, uint32, uint32	, uint32 ,int8 *, int8 *);
extern uint32	myRmtDataAcc(uint8 *, uint32, uint32	, uint32 ,int8 *, int8 *);
static uint32 compareSysExecInfo(uint32 );
static uint32 compareSfcDataInfo(uint32 );
#endif


extern uint8* getSourceFileName(uint8*,uint32*);

static void rasDetailIOShelfInfoPrint(strNewRasSummaryInfo *, uint32 );
static void rasDetailProfiShelfInfoPrint(strNewRasSummaryInfo *,uint32);
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
void	dualTransDataInfoDisplay(strDebugDataInfo *, uint8 *);
uint32  transDualInfoSet( strMsgQueData *);
void	dualDebugVarClear(void);
void	debugDualInfoSet(strMsgQueData *, strDebugDataInfo *);
static uint32 debugDualInfoCheck(strMsgQueData *, strDebugDataInfo *);
void	dualDataDetailInfoDisplay(uint8, uint8);
static  void dualDetailInfoPrint(strTransDetailInfo *,uint8 *,uint8,uint8);
#endif
#ifdef _WIN32
void   sysMsgLogSet( uint8*,uint32,  uint8*, uint32, uint8*, uint32) ;
uint32 filelistDisplay(uint32);
void   flashDiskinfo(void);
uint32 romImgUpdate();
#endif



extern void		memoryClear (uint8 *, uint32);
extern void		memoryCopy (uint8 *, uint8 *, uint32);
extern int32	keyHit (void);
extern void		errorStringGet(uint32,uint8 *);
extern void		sysTaskDelay (int32 );
extern int32	systemClockRateGet(void);
extern int32 	semaphoreCreate(strSemaInfo *, int32 *);
extern int32 	semaphoreGet(int32 , int32 );
#ifdef VXWORKS
extern int32 	semaphoreRelease(int32);
#else
extern int32 	semaphoreRelease(int32,int32);
extern int32	semaphoreRelease2(uint8 *, uint32, int32, int32);
#endif
extern strDoubleLink *nodeGetAndUnlink(strDoubleLink **);
extern int32	nodePutOnLast(strDoubleLink **,strDoubleLink *);
extern void		dataDisplay(uint8 *, uint8 *, uint16 );
extern strDoubleLink *nextNodeRef(strDoubleLink *,strDoubleLink *);
extern void		ipAddrHexToString(int8 *, uint32 );
extern void		schedulerDebugVarInit(void);
extern uint32	netMsgChecksumCalcu(uint32	,uint32	*,uint32	*);

extern int32   ioRead(int32 , int8 *, int32 );
extern void spaceSkip(int8 **);

extern uint32	moduleInfoRead(uint32 , strModuleInfo	*);
extern uint32	getFbTaskTblInfo(uint32 , uint32 , strFbTaskTblMemInfo *);
extern void		fourBytesDataCopy(uint32 *, uint32 *, uint32);
extern uint32	systemCnfgInfoRead (strSysConfigInfo *);
extern uint32	fbSysExeModuleInfoRead( uint32, strModuleExecInfo* );
extern void rmtComCountDebugVarInit(void);
#ifdef VXWORKS
extern void boardLedBlink (uint8 , int32);
#endif

#ifdef VXWORKS
extern int32	ioSysCtrlFunc(int32 , int32 , int32 );
extern int32	sysTaskIdListGet(int32 *, int32 );
#endif

#ifdef _WIN32
extern void		logMsg(char *, int,int,int,int,int,int); 
extern char* systoasctime(SYSTEMTIME *st);
#endif

/* [V104] 2021.01.12 : 부팅 에러 발생시 boot Config 모드로 Set */
extern void bootParaConfig(void);

void systemInitErr
			(
		uint8 	*fileName,
        uint32	lineNum,
        uint8	*moduleName,
        uint32	errorCode
        	)
{
	uint8	errorString[128];
	uint32	loop = 0;
	
    memoryClear( (uint8 *)&errorString[0], 128);

	errorStringGet(errorCode, (uint8 *)&errorString[0]);

    for(loop = 0; loop < 3; loop++) 
    {
		printf("[%s(%d)]-[%s]-[0x%x::%s]\n",fileName,lineNum,moduleName,errorCode,errorString);
        
        sysTaskDelay(systemClockRateGet() * 2);
	}
	
	/* [V104] 2021.01.12 : 부팅 에러 발생시 boot Config 모드로 Set */
	bootParaConfig();

	return;	
}


void appErrStringMonitoring (uint32 arg1, uint32 arg2)
{
	gDebugErrStringFlag 	= DEBUG_ENABLE;
	sISRDebugErrStringFlag  = DEBUG_ENABLE;

	while(!keyHit()) {

		    	
    	errQAccessAndDisplay();

		
    	sysTaskDelay(systemClockRateGet()/10);
	}

	gDebugErrStringFlag 	= DEBUG_DISABLE;
	sISRDebugErrStringFlag  = DEBUG_DISABLE;

	
    
  	sysTaskDelay(systemClockRateGet());
	
    
	errorQueueClear();
}


void errorQueueClear(void)
{
    strErrorInfo *errData = (strErrorInfo *)NULL;
    strDetailErrorInfo *errDetailData = (strDetailErrorInfo *)NULL;
    int32 status = NO_ERROR;

    
    if(sErrQInfo.usedList != NULL){
        status = semaphoreGet(sErrQSemaId, FOREVERWAIT);

        if(status != NO_ERROR) goto MODULE_END;

        errData = sErrQInfo.usedList;
        while(errData != NULL) {
            errData = (strErrorInfo *)nodeGetAndUnlink (
                                    (strDoubleLink **)&sErrQInfo.usedList
                                                      );
            if(errData != NULL) {
                
                errorStringDisplay( errData->fileName,
                                    errData->lineNumber,
                                    errData->moduleName,
                                    errData->errorCode
                                  );
                nodePutOnLast (
                        (strDoubleLink **) &sErrQInfo.freeList,
                        (strDoubleLink *) errData
                              );
		    	sysTaskDelay(systemClockRateGet()/10);
            }
        }
#ifdef VXWORKS
		semaphoreRelease( sErrQSemaId);
#else
		semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sErrQSemaId);
#endif
    }

    if(sDetailErrQInfo.usedList != NULL)
	{

        status = semaphoreGet(sErrQSemaId, FOREVERWAIT);

        if(status != NO_ERROR) goto MODULE_END;

        errDetailData = sDetailErrQInfo.usedList;
        while(errDetailData != NULL) {
            errDetailData = (strDetailErrorInfo *)nodeGetAndUnlink (
                                    (strDoubleLink **)&sDetailErrQInfo.usedList
                                                      );
			
            if(errDetailData != NULL) {
                
				detailErrorStringDisplay(
							errDetailData->errInfo.fileName,
        					errDetailData->errInfo.lineNumber,
                            errDetailData->errInfo.moduleName,
                            errDetailData->errInfo.errorCode,
							errDetailData->errValInfo[0].errString,
                            errDetailData->errValInfo[0].refVal,
							errDetailData->errValInfo[1].errString,
                            errDetailData->errValInfo[1].refVal,
							errDetailData->errValInfo[2].errString,
                            errDetailData->errValInfo[2].refVal
						  );
                nodePutOnLast (
                        (strDoubleLink **) &sDetailErrQInfo.freeList,
                        (strDoubleLink *) errDetailData
                              );

		    	sysTaskDelay(systemClockRateGet()/10);
            }
        }
#ifdef VXWORKS
		semaphoreRelease( sErrQSemaId);
#else
		semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sErrQSemaId);
#endif
    }

MODULE_END:
	return;
}


static void detailErrorStringDisplay(    uint8 *fileName, uint32 line,
                            uint8 *moduleName, uint32 errCode,
                            uint8 *str1, uint32 val1,
                            uint8 *str2, uint32 val2,
                            uint8 *str3, uint32 val3
                        )
{
	uint8 errString[128];
	uint32 nameLen = 0;
	
	memoryClear( errString, 128);
    
	errorStringGet(errCode, errString);

	
	if( (fileName==NULL)||(moduleName==NULL) ){
		printf("FileName or ModuleName is NULL\n!!!");
		return;
	}
	
	fileName = getSourceFileName(fileName, &nameLen);

	printf("[%s(%d)]-[%s]-[%s(0x%x)]::\n",  fileName,
                 line, moduleName, errString, errCode);

	if( (str1==NULL)||(str2==NULL)||(str3==NULL)  ){
		printf("Detail Error String is NULL!!!\n");
		return;
	}
	
	printf( "[%s(0x%x)]-[%s(0x%x)]-[%s(0x%x)]\n",
    			str1, val1, str2, val2, str3, val3);
    return;
}


void errQAccessAndDisplay(void)
 {
	strErrorInfo	*errPtr = (strErrorInfo *)NULL;
	strDetailErrorInfo	*errDetailPtr = (strDetailErrorInfo *)NULL;
	int32	status = NO_ERROR;

     
    if(sErrQInfo.usedList != NULL) {
    	errPtr = (strErrorInfo *) NULL;

		status = semaphoreGet(sErrQSemaId, FOREVERWAIT);

		if(status == NO_ERROR){
			errPtr = (strErrorInfo *)nodeGetAndUnlink (
                                    (strDoubleLink **)&sErrQInfo.usedList
                                                        );
			
#ifdef VXWORKS
            semaphoreRelease(sErrQSemaId);
#else
            semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sErrQSemaId);
#endif
		}
		else {
			printf( "[%s(%d)]-[errQAccessAndDisplay]-[Semaphore Get Error]\n",
									(uint8 *)__FILE__, __LINE__);

		}
	} 
	if(errPtr != (strErrorInfo *)NULL) {
		errorStringDisplay(	errPtr->fileName,
        					errPtr->lineNumber,
                            errPtr->moduleName,
                            errPtr->errorCode
						  );
        
        status = semaphoreGet(sErrQSemaId, FOREVERWAIT);

        if(status == NO_ERROR){
            nodePutOnLast ( (strDoubleLink **) &sErrQInfo.freeList,
                            (strDoubleLink *) errPtr
                          );
#ifdef VXWORKS
			semaphoreRelease( sErrQSemaId);
#else
			semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sErrQSemaId);
#endif
		}
        else {
			printf( "[%s(%d)]-[errQAccessAndDisplay]-[Semaphore Get Error]\n",
									(uint8 *)__FILE__, __LINE__);
		}
    }

     
    if(sDetailErrQInfo.usedList != NULL) {
    	errDetailPtr = (strDetailErrorInfo *) NULL;

		status = semaphoreGet(sErrQSemaId, FOREVERWAIT);
		if(status == NO_ERROR){
			errDetailPtr = (strDetailErrorInfo *)nodeGetAndUnlink (
                                    (strDoubleLink **)&sDetailErrQInfo.usedList
                                                        );
			
#ifdef VXWORKS
            semaphoreRelease( sErrQSemaId);
#else
            semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sErrQSemaId);
#endif
		}
		else {
			printf( "[%s(%d)]-[errQAccessAndDisplay]-[Semaphore Get Error]\n",
									(uint8 *)__FILE__, __LINE__); 
		}
	} 
	if(errDetailPtr != (strDetailErrorInfo *)NULL) {
		detailErrorStringDisplay(	errDetailPtr->errInfo.fileName,
        					errDetailPtr->errInfo.lineNumber,
                            errDetailPtr->errInfo.moduleName,
                            errDetailPtr->errInfo.errorCode,
							errDetailPtr->errValInfo[0].errString,
                            errDetailPtr->errValInfo[0].refVal,
							errDetailPtr->errValInfo[1].errString,
                            errDetailPtr->errValInfo[1].refVal,
							errDetailPtr->errValInfo[2].errString,
                            errDetailPtr->errValInfo[2].refVal
						  );

        
        status = semaphoreGet(sErrQSemaId, FOREVERWAIT);

        if(status == NO_ERROR){
            nodePutOnLast ( (strDoubleLink **) &sDetailErrQInfo.freeList,
                            (strDoubleLink *) errDetailPtr
                          );
#ifdef VXWORKS
            semaphoreRelease( sErrQSemaId);
#else
            semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sErrQSemaId);
#endif
		}
        else {
			printf( "[%s(%d)]-[errQAccessAndDisplay]-[Semaphore Get Error]\n",
									(uint8 *)__FILE__, __LINE__);  

		}
    }
}


void errorStringDisplay(    uint8 *fileName, uint32 line,
                            uint8 *moduleName, uint32 errCode
                        )
{
	uint8 errString[128];
	uint32 nameLen = 0;
	
	memoryClear( errString, 128);
    
	errorStringGet(errCode, errString);

	
	if( (fileName==NULL)||(moduleName==NULL) ){
		printf("FileName or ModuleName is NULL!!!\n");
		return;
	}
	
	fileName = getSourceFileName(fileName,&nameLen);

	printf("[%s(%d)]-[%s]-[%s(0x%x)] \n",  fileName,
                 line, moduleName, errString, errCode);

	return;
}


void fatalErrStringDisplay( int8 *filename, uint32 lineNum,   
							uint8 *moduleName, uint32 errorNum,
							uint32 busAddr,
                            uint8 *timeStr, uint32 index
                        )
{
	uint8	errString[128];
	uint32	matchFlag = BUS_DATA_ACC_ERR;

    memoryClear ( errString, 128);

    errorStringGet( errorNum, errString);

    printf( "[%03d] : %s",index,timeStr);
            
    if(errorNum == matchFlag){
    	printf( "\t [%s::%4d]-[%s] : %s[0x%x]\n",  
									filename,
									lineNum,	
									moduleName,
                    	            errString,
                    	            busAddr
            	);
  	}
	else { 
    	printf( "\t [%s::%4d]-[%s] : %s\n",  
									filename,
									lineNum,	
									moduleName,
                    	            errString
            	);
 	}
}


void networkPerformanceDisplay(uint32 arg1, uint32 arg2)
{
    int8    *indexStr[] = {
        (int8 *)"SENDR", (int8 *)"RECVR", (int8 *)"SBPS", (int8 *)"RBPS", (int8 *)NULL
    };

    int8    *titleHelp[] =
    {
        (int8 *)"SENDR      - Sending Rate Per Second",
        (int8 *)"RECVR      - Receiving Rate Per Second",
        (int8 *)"SBPS       - Sending Bits Per Second( k Bps )",
        (int8 *)"RBPS       - Receiving Bits Per Second( K Bps )",
        (int8 *)NULL
    };
    int8    **tMsg;
    uint32  prevTSendCount = 0;
    uint32  prevTRecvCount = 0;
    uint32  sendRate = 0, recvRate = 0;
    float   sendBps = 0.0, recvBps = 0.0;
    uint32  prevByteLenOfSendingPacket = 0;
    uint32  prevByteLenOfRecvingPacket = 0;

    printf( "\n\t************* NETWORK PERFORMANCE[Update Per 1 second] *************\n\n");

    printf( "\t\t [Title Message Help List]\n");

    for(tMsg = titleHelp; *tMsg != NULL; tMsg++)
        printf( "\t%s\n", *tMsg);

    printf( "\n\n");

    for(tMsg = indexStr; *tMsg != NULL; tMsg++)
        printf( "%10s", *tMsg);
    printf( "\n");

    for(tMsg = indexStr; *tMsg != NULL; tMsg++)
        printf( "%10s", "------");

    printf( "\n");

    
    gTransRateCalcuFlag = DEBUG_ENABLE;

    while(!keyHit()) {
        
        sendRate =  sNetPerTransInfo.totalSendCount - prevTSendCount;
        recvRate =  sNetPerTransInfo.totalRecvCount- prevTRecvCount;

        
        sendBps =  (float)sNetPerTransInfo.byteLenOfSendingPacket;
        sendBps =  (float)(sendBps - prevByteLenOfSendingPacket);
        sendBps =  (float)(sendBps * 8 /1000);

        recvBps =  (float)sNetPerTransInfo.byteLenOfRecvingPacket;
        recvBps =  (float)(recvBps - prevByteLenOfRecvingPacket);
        recvBps =  (float)(recvBps * 8 /1000);

        printf( "%10d%10d%10.2f%10.2f\r",
                                sendRate,
                                recvRate,
                                sendBps,
                                recvBps
				);

        prevTSendCount = sNetPerTransInfo.totalSendCount;
        prevTRecvCount = sNetPerTransInfo.totalRecvCount;

        prevByteLenOfSendingPacket = sNetPerTransInfo.byteLenOfSendingPacket;
        prevByteLenOfRecvingPacket = sNetPerTransInfo.byteLenOfRecvingPacket;

        
        sysTaskDelay(systemClockRateGet());
    }

    
    gTransRateCalcuFlag = DEBUG_DISABLE;

	memoryClear ( 	(uint8 *) &sNetPerTransInfo,
    				sizeof(strNetworkPerInfo)
				);

    printf( "\n");
}


void networkDebugVarClear(uint32 arg1, uint32 arg2)
{
    char	buf[64];
    
	printf( "\b******** Network Debugger Variable Clear !!!\n");

    
    memoryClear ( (uint8 *)gDebugRecvData,
    			  sizeof(strDebugDataInfo) * DEBUG_INFO_MAX_ARRAY_NUM
				);

    memoryClear ( (uint8 *)gDebugSendData,
    			  sizeof(strDebugDataInfo) * DEBUG_INFO_MAX_ARRAY_NUM
				);
	
	rmtComCountDebugVarInit();
	
	
#ifdef VXWORKS
	gMvme3100Tsec1QHaltErrCount = 0;
	gMvme4100Tsec1LinkReloadCount = 0;
	gMvme4100Tsec1LinkReloadErrCount = 0;
	

	if(gMpc85xxTsec1THaltErrCount != 0)

	{
		memoryClear( (uint8 *)buf, 64);
		sprintf (buf,"%s%d down", NET_DEV_NAME,0);
		
		if (ifconfig (buf) == ERROR)
		{
			printf ("Failed to bring %s down.\n",buf);
			
			gMvme4100Tsec1LinkReloadErrCount++;
		}
		else
		{
			sprintf (buf,"%s%d up", NET_DEV_NAME,0);
			
			if (ifconfig (buf) == ERROR)
			{
				printf ("Failed to bring %s up.\n",buf);
				
				gMvme4100Tsec1LinkReloadErrCount++;
			}
			else
			{
				printf ("Success %s re-up\n", buf);
				gMvme4100Tsec1LinkReloadCount++;
				gMpc85xxTsec1THaltErrCount = 0;
			}
		}
	}
#endif
}

void transDataInfoDisplay(strDebugDataInfo *dataPtr, uint8 *transMsg)
{
    int8    line[16];
    int8    ipArray[20];
  
    int8    *pString[] = 
    {
        (int8 *)"INDEX",
		(int8 *)"IP ADDR",
		(int8 *)"PORT",
		(int8 *)"MSG KIND",
		(int8 *)"GRP CMD",
		(int8 *)"ACTION CODE",
		(int8 *)NULL
    };
    
    int8 *msgKindStr[] = 
    { 
		(int8 *)"UNICAST",
		(int8 *)"BROADCAST",
		(int8 *)"NULL"};

    int8  *cString[] =
    {
        (int8 *)"IP ADDR",
		(int8 *)"PORT NUM",
		(int8 *)"SOCK FD",
		(int8 *)"SEQ NUM",
		(int8 *)"DATA SIZE",
		(int8 *)"TRANS COUNT",
		(int8 *)NULL
    };

    int8    **pMsg;
    int32   showIndex, i, index;
    strDebugDataInfo    *curPtr = dataPtr;
    uint32  ipHexVal;
	strCommandListInfo	cmdList;
	
    
    gRecvInfoFlag = 1;
    gSendInfoFlag = 1;

    memoryClear ( (uint8 *)ipArray, 20);

    
    sysTaskDelay(systemClockRateGet());

    printf( "\n\t ******* Debug Table Info ********\n");
	
    pMsg = pString;
	printf( "%2s", *pMsg);
    pMsg++;
    
	for(; *pMsg != NULL; pMsg++) 
	{ 
        printf( "%14s",*pMsg);
	}
	
	printf( "\n");

    printf( "%2s%14s%14s%14s%14s%14s\n",
				"-----","-------","----","---------","---------","----------");

    
    for(i = 0; i < DEBUG_INFO_MAX_ARRAY_NUM; i++,curPtr++)
    {
        ipHexVal = curPtr->peerAddr.iAddr;

        switch(htons(curPtr->peerAddr.port)) 
        {
		   	case UNI_EWS_UDP_RECV_PORT_A:
		   	case UNI_EWS_UDP_RECV_PORT_B:
		   	case UNI_EWS_UDP_SEND_PORT_A:
		   	case UNI_EWS_UDP_SEND_PORT_B:
        	case UNI_WDC_UDP_RECV_PORT_A:
			case UNI_WDC_UDP_RECV_PORT_B:
			case UNI_WDC_UDP_SEND_PORT_A:
			case UNI_WDC_UDP_SEND_PORT_B:

        	case UNI_HST_UDP_RECV_PORT_A:
			case UNI_HST_UDP_RECV_PORT_B:
			case UNI_HST_UDP_SEND_PORT_A:
			case UNI_HST_UDP_SEND_PORT_B:				
			case UNI_UDP_SEND_RECV_PORT_A:
			case UNI_UDP_SEND_RECV_PORT_B:
            	index = 0;
			break;
            case BROAD_UDP_SEND_RECV_PORT_A:
		  	case BROAD_UDP_SEND_RECV_PORT_B:
	        case BROAD_EWS_SEND_RECV_PORT_A:
		  	case BROAD_EWS_SEND_RECV_PORT_B:
				index = 1;
			break;
			default:
				index = 2;
			break;	  
        }
        
        if(ipHexVal) 
        {
			memoryClear ( 	(uint8 *) &cmdList, 
							sizeof(strCommandListInfo)
						);			
			commandStringGet(	&cmdList, 
								curPtr->opCode, 
								curPtr->optionCode
							);
        
            ipAddrHexToString(ipArray, ipHexVal);
            
            printf( "%2d%20s%10d%15s%16s%16s\n", 
					i, ipArray,htons(curPtr->peerAddr.port), msgKindStr[index],
					cmdList.opCode, cmdList.optionCode);
        }
    }
    
    for(;;) 
    {
        printf( "\nEnter the INDEX for Display ? ");
        ioRead(gDebugPortFd, line, sizeof(line));
        showIndex = atoi((const char *)line);
        
        if( (showIndex < 0) || (showIndex >= DEBUG_INFO_MAX_ARRAY_NUM)) 
        {
            printf( "\t ****** INVALID INDEX[0x%x]\n", showIndex);
		}
        else 
        {
            break;
		}
    }

    
    curPtr = dataPtr;
    for(i = 0; i < showIndex; i++) curPtr++;

    printf( "\n\t**********%s Data Info Display**********\n", transMsg);

    for(pMsg = cString; *pMsg != NULL; pMsg++) 
    {
        printf( "%12s", *pMsg);
	}
    printf( "\n");

    for(pMsg = cString; *pMsg != NULL; pMsg++) 
    {
        printf( "%12s", "----------");
	}
    printf( "\n");

    ipAddrHexToString(ipArray, curPtr->peerAddr.iAddr);

    while(!keyHit()) 
    {
        printf( "%15s%9d%12d%12x%12d%12d\r",  ipArray,
                                        htons(curPtr->peerAddr.port),
                                        curPtr->peerAddr.sockFd,
                                        curPtr->seqNum,
                                        curPtr->msgDataSize,
                                        curPtr->transTotalCount);
        sysTaskDelay(systemClockRateGet()/2);
    }
    printf( "\n");

    
#ifdef VXWORKS
    (void)ioSysCtrlFunc (gDebugPortFd, MYIOFLUSH, 0);
#endif
    
    gRecvInfoFlag = 0;
    gSendInfoFlag = 0;

    printf("\n");
}

static void commandStringGet(strCommandListInfo *cmdPtr, uint32 opCode, uint32 optionCode)
{

    int8 *opCodeList[] =
    {
        "UNKNOWN GRP CMD",
        "SYS_DIAG_CMD", "MODE_ACC_CMD", "CODE_ACC_CMD", 
		"FB_ONL_DWL_CMD", "VAR_ACC_CMD", "FRC_ACC_CMD",
		"XR_BROD_CMD","TIME_ACC_CMD","ETH_DATA_CMD",
		"ETH_STAT_CMD","HIGH_SPEED_CMD"
		"UNKNOWN CMD"
    };

    int8    *CodeAccessOptionCodeList[] = 
    { 
        "UNKNOWN OPTION CODE",
        "FB_TASK_DWL", "FB_CODE_DWL", "FB_TASKTBL_DWL",
		"FB_OUTPUT_READ","FB_OUTPUT_WRITE", "FB_PARA_READ", "FB_PARA_WRITE", 
		"FB_STAT_READ","IO_CONFIG_READ", 
		"SFC_INFO_DWL","SFC_LINK_DWL",
		"SFC_ACT_DWL","SFC_TRS_DWL", "SFC_STATUS_READ",
		"SFC_STEP_JUMP","UNKNOWN OPTION CODE"
    };

    int8    *CtrlModeAccessOptionCodeList[] =
    {
        "UNKNOWN OPTION CODE",
        "MODE_SEG_READ", "MODE_SEG_WRITE",
        "MODE_CTRL_READ", "MODE_CTRL_WRITE",
        "UNKNOWN OPTION CODE"
    };

    int8    *VariableAccessOptionCodeList[] =
    { 
        "UNKNOWN OPTION CODE",
        "VAR_BLOCK_READ", "VAR_ITEM_READ",
        "VAR_BLOCK_WRITE", "VAR_ITEM_WRITE",
        "VAR_INFO_DOWN","VAR_ALLMEM_READ",
		"VAR_ALLMEM_WRITE",
        "UNKNOWN OPTION CODE"
    };

    int8    *sysTimeOptionCodeList[] =
    {
        "UNKNOWN OPTION CODE",
        "TIME_SYNC_BROAD", "TIME_READ_RCS",
		"TIME_WRITE_RCS",
        "UNKNOWN OPTION CODE"
    };

    int8    *forcingOptionCodeList[] =
    { 
        "UNKNOWN OPTION CODE",
        "FRC_FLAG_WRITE", "FRC_STATION_WRITE",
		"FRC_STATUS_READ", "FRC_TAKS_WRITE",
        "UNKNOWN OPTION CODE"
    };

	 int8    *sysStatusOptionCodeList[] =
    { 
        "UNKNOWN OPTION CODE",
        "SYS_DIAG_STATUS", "SYS_HISTORY_READ","TASK_STATUS_READ",
		"SYS_INFODATA_READ", "SYS_DATA_WTITE",
        "UNKNOWN OPTION CODE"
    };

	 int8    *xrBroadcastOptionCodeList[] =
    { 
        "UNKNOWN OPTION CODE",
        "XR_SEND",
        "UNKNOWN OPTION CODE"
    };

	int8   *onlineDownloadOptionCodeList[] =
	{ 
        "UNKNOWN OPTION CODE",
        "ONLINE_DWL_ADD",  "ONLINE_DWL_DEL",
		"ONLINE_DWL_SPEC_CHG", "ONLINE_DWL_SEQ_ID_CHG",
		"ONLINE_DWL_VAR_INFO_CHG","ONLINE_DWL_SFC_DATA_CHG",
		"SPEC_MEM_REALLOC","ONLINE_LOGIC_UPDATE_OR_CANCEL",
		"VERSION_INFO_WRITE", "VERSION_INFO_READ","MEMORY_STAUTS_READ",
		"ONLINE_FBINFO_CHG", "ONLINE_DWL_IO_SPEC_CHG",
		"UNKNOWN OPTION CODE"
    };

	 int8    *ethRmtDataAccOptionCodeList[] =
    { 
        "UNKNOWN OPTION CODE",
        "ETH_DATA_READ", "ETH_DATA_WRITE", "ETH_DATA_BROAD",
		"UNKNOWN OPTION CODE"
    };

    int8    *ethRmtComPathAccOptionCodeList[] =
    { 
        "UNKNOWN OPTION CODE",
        "ETH_STATUS_READ", 
        "UNKNOWN OPTION CODE"
    };

    int8	*highSpeedCmdOPFuncList[] =
    { /* 0x10 */
        "UNKNOWN OPTION CODE",		/* 0x00 */		
		"HST_DATA_SEND",			/* 0x01 */	
		"HST_STATUS_READ",			/* 0x02 */
        "UNKNOWN OPTION CODE"
	};
	
    int8    defaultString[] = "UNKNOWN COMMAND";
    uint32  status = 0;

    switch(opCode)
    {
        case CODE_ACCESS_CMD: 
            if( (optionCode < CODE_SEG_DOWN) ||
                (optionCode > SFC_STEP_JUMP)
              )
                status = 1;
            else {
                memoryCopy( cmdPtr->optionCode,
                            CodeAccessOptionCodeList[optionCode],
                            strlen(CodeAccessOptionCodeList[optionCode])
                          );
            }
        break;
        case MODE_ACCESS_CMD: 
            if( (optionCode < MODE_SEG_READ) ||
                (optionCode > MODE_CONTROL_WRITE)
               )
                status = 1;
            else {
                memoryCopy( cmdPtr->optionCode,
                            CtrlModeAccessOptionCodeList[optionCode],
                            strlen(CtrlModeAccessOptionCodeList[optionCode])
                          );
            }
        break;
       case VAR_ACCESS_CMD: 
            if( (optionCode < VAR_BLOCK_READ) ||
                (optionCode > VAR_ALLMEM_WRITE)
              )
                status = 1;
            else {
                memoryCopy( cmdPtr->optionCode,
                            VariableAccessOptionCodeList[optionCode],
                            strlen(VariableAccessOptionCodeList[optionCode])
                          );
            }
        break;

        case TIME_ACCESS_CMD: 
            if( (optionCode < TIME_SYNC_BROAD) ||
                (optionCode > TIME_WRITE_RCS)
              )
                status = 1;
            else {
                memoryCopy( cmdPtr->optionCode,
                            sysTimeOptionCodeList[optionCode],
                            strlen(sysTimeOptionCodeList[optionCode])
                          );
            }
        break;
        case FRC_ACCESS_CMD:    
            if( (optionCode < FRC_FLAG_WRITE) ||
                (optionCode > FRC_TAKS_WRITE)
               )
                status = 1;
            else {
                memoryCopy( cmdPtr->optionCode,
                            forcingOptionCodeList[optionCode],
                            strlen(forcingOptionCodeList[optionCode])
                          );
            }
        break;

        case SYS_STATUS_CMD:    
            if( (optionCode < SYS_DIAG_STATUS) ||
                (optionCode > SYS_DATA_WTITE)
               )
                status = 1;
            else {
                memoryCopy( cmdPtr->optionCode,
                            sysStatusOptionCodeList[optionCode],
                            strlen(sysStatusOptionCodeList[optionCode])
                          );
            }
        break;

        case XR_BROADCAST_CMD:    
            if( (optionCode != XR_SEND)
               )
                status = 1;
            else {
                memoryCopy( cmdPtr->optionCode,
                            xrBroadcastOptionCodeList[optionCode],
                            strlen(xrBroadcastOptionCodeList[optionCode])
                          );
            }
        break;


		case FB_ONLINE_DOWNLOAD_CMD: /* 0x4 */
            if( (optionCode < FB_ONLINE_DOWNLOAD_ADD) ||
                (optionCode > IO_ONLINE_DOWNLOAD_SPEC_CHG)
              )
                status = 1;
            else {
                memoryCopy( cmdPtr->optionCode,
                            onlineDownloadOptionCodeList[optionCode],
                            strlen(onlineDownloadOptionCodeList[optionCode])
                          );
            }
        break;


        case ETHERNET_DATA_ACC_OP: 
            if( (optionCode < ETHERNET_DATA_READ) ||
                (optionCode > ETHERNET_DATA_BROAD  )
              )
                status = 1;
            else {
                memoryCopy( cmdPtr->optionCode,
                            ethRmtDataAccOptionCodeList[optionCode],
                            strlen(ethRmtDataAccOptionCodeList[optionCode])
                          );
            }
        break;
        case ETHERNET_COM_PATH_OP: 
            if(optionCode != ETHERNET_DEV_STATUS_READ)
                status = 1;
            else {
                memoryCopy( cmdPtr->optionCode,
                            ethRmtComPathAccOptionCodeList[optionCode],
                            strlen(ethRmtComPathAccOptionCodeList[optionCode])
                          );
            }
        break;

        default:
            status = 1;
        break;
    }

    if(status) {
        memoryCopy(cmdPtr->opCode, defaultString, strlen(defaultString));
        memoryCopy(cmdPtr->optionCode, defaultString, strlen(defaultString));
    }
    else
        memoryCopy( cmdPtr->opCode,
                    opCodeList[opCode],
                    strlen(opCodeList[opCode])
                  );
}


void transDataDetailInfoDisplay(uint8 transInfoFlag, uint8 dataDispFlag)
{
    strTransDetailInfo  detailData;
    uint8   *cString;

    if(transInfoFlag == DETAIL_DATA_FOR_RECV) 
    {
    	if( (dataDispFlag == UNICAST_HEAD_PRINT) ||
    		(dataDispFlag == UNICAST_DETAIL_DATA_PRINT)
    	)
        	gTransUnicastDetailRecvFlag = 1;
		else
			gTransBroadcastDetailRecvFlag = 1;
			        	
        cString = (uint8 *)"RECV DATA DETAIL INFO";
    }
    else 
    {
    	if( (dataDispFlag == UNICAST_HEAD_PRINT) ||
    		(dataDispFlag == UNICAST_DETAIL_DATA_PRINT)
    	)
        	gTransUnicastDetailSendFlag = 1;
		else
			gTransBroadcastDetailSendFlag = 1;
			        	
        cString = (uint8 *)"SEND DATA DETAIL INFO";
    }

    while(!keyHit()) 
    {
        if(transInfoGet(&detailData) == NO_ERROR)
            transDetailInfoPrint(	&detailData, 
									cString, 
									dataDispFlag, 
									transInfoFlag
								);

        
        
        sysTaskDelay(systemClockRateGet()/2);
    }
    
    if(transInfoFlag == DETAIL_DATA_FOR_RECV)
    {
     	gTransBroadcastDetailRecvFlag 	= 0;
     	gTransUnicastDetailRecvFlag		= 0;
    }
    else 
    { 
    	gTransBroadcastDetailSendFlag 	= 0;
    	gTransUnicastDetailSendFlag		= 0;
    }
    
    sysTaskDelay(systemClockRateGet()/2);

    
    (void)transInfoQueClear();
}

static void transInfoQueClear(void)
{
    strTransDetailInfo *detailData = (strTransDetailInfo *)NULL;
    uint32 status = NO_ERROR;

    
    if(sTransQueMngrInfo.usedList != NULL){

        status = semaphoreGet(sTransMsgDbgSemaId, FOREVERWAIT);
        while(sTransQueMngrInfo.usedList != NULL) {
            detailData = (strTransDetailInfo *)nodeGetAndUnlink (
                                    (strDoubleLink **)&sTransQueMngrInfo.usedList
                                                                );
            nodePutOnLast ( (strDoubleLink **) &sTransQueMngrInfo.freeList,
                            (strDoubleLink *) detailData
                          );

        }
#ifdef VXWORKS
        semaphoreRelease( sTransMsgDbgSemaId);
#else
        semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sTransMsgDbgSemaId);
#endif
    }
}


static void transDetailInfoPrint(   strTransDetailInfo *dataPtr,
                                    uint8 *cString,
                                    uint8   dataDispFlag,
                                    uint8   transInfoFlag
                                )
{
    int8    *msgType[] =
    {
        (int8 *)NULL,
        (int8 *)"REQUEST",
		(int8 *)"REPLY",
		(int8 *)"BROADCAST",
        (int8 *)NULL
    };

    uint8   *dispDataPtr;
    uint8   ipArray[20];
    strCommandListInfo  cmdList;

	strNewNetProtocolInfo *netMsgPtr;

	uint32 opCmd, opCode, optionCode;

	netMsgPtr = (strNewNetProtocolInfo *)dataPtr->dataBuff;

	memoryCopy((uint8*)&opCmd, (uint8*)&netMsgPtr->dataBuff[0], DOUBLE_SIZE);
	opCode = (opCmd & 0xffff0000) >> 16;
	optionCode = (opCmd & 0xffff);

    if ( (dataDispFlag == UNICAST_HEAD_PRINT) ||
		 (dataDispFlag == UNICAST_DETAIL_DATA_PRINT)
	){ 
        if (netMsgPtr->messageType >= BROADCAST)
			goto MODULE_END;
    }
    else if ( (dataDispFlag == BROADCAST_HEAD_PRINT) ||
		 (dataDispFlag == BROADCAST_DETAIL_DATA_PRINT)
	){ 
        if ( (netMsgPtr->messageType != BROADCAST) || 
			(opCode == XR_BROADCAST_CMD) )
			goto MODULE_END;
    }
    else  if ( dataDispFlag == BROADCAST_DETAIL_XR_DATA_PRINT)
	{ 
        if(	(netMsgPtr->messageType != BROADCAST) ||
        	((opCode) != XR_BROADCAST_CMD)
        )
         	goto MODULE_END;
    }
	
    memoryClear((uint8 *)ipArray, 20);
    memoryClear((uint8 *) &cmdList, sizeof(strCommandListInfo));

    printf( "\n\t***********************[%s]********************\n", cString);

    ipAddrHexToString((int8 *)ipArray, dataPtr->peerAddr.iAddr);

    if(transInfoFlag == DETAIL_DATA_FOR_RECV) {
        printf( "\t\tSource Internet Address : %s\n",ipArray);
	}
    else {
        printf( "\t\t  Dest Internet Address : %s\n",ipArray);
	}
    printf( "\t\t            Port Number : %d\n", ntohs(dataPtr->peerAddr.port));

    printf( "\t\t         Total Byte Len : %d\n", dataPtr->totalBytesLen);
    printf( "\t\t        Sequence Number : 0x%x\n", netMsgPtr->sequenceID);
    printf( "\t\t              Source ID : 0x%x\n", netMsgPtr->sourceID);
    printf( "\t\t         Destination ID : 0x%x\n", netMsgPtr->destinationID);

    if( (netMsgPtr->messageType < REQUEST) || (netMsgPtr->messageType > BROADCAST)) {
        printf( "\t\t           Message Type : UNKNOWN[0x%x]\n", netMsgPtr->messageType);
	}
    else {
        printf( "\t\t           Message Type : %s\n", msgType[netMsgPtr->messageType]);
	}
    printf( "\t\t                Dual ID : 0x%x\n", netMsgPtr->dualStatus);
    commandStringGet(   &cmdList,
                        opCode, 
                        optionCode
                    );
    printf( "\t\t                OP Code : %s\n", cmdList.opCode);
    printf( "\t\t            Option Code : %s\n", cmdList.optionCode);

    
    if( (dataDispFlag == UNICAST_DETAIL_DATA_PRINT) ||
    	(dataDispFlag == BROADCAST_DETAIL_DATA_PRINT)
	){
        
        dispDataPtr = (uint8 *)netMsgPtr;

        dataDisplay(    (uint8 *)"PROTOCOL DATA",
                        dispDataPtr,
                        (uint16)dataPtr->totalBytesLen
                    );
    }
MODULE_END:
	return;
}


static uint32  transInfoGet(strTransDetailInfo *dataPtr)
{
    strTransDetailInfo *detailData = (strTransDetailInfo *)NULL;
    uint32 status = NO_ERROR;

    
    if(sTransQueMngrInfo.usedList != NULL){

        status = semaphoreGet(sTransMsgDbgSemaId, FOREVERWAIT);

        if(status == NO_ERROR){
            detailData = (strTransDetailInfo *)nodeGetAndUnlink (
                                    (strDoubleLink **)&sTransQueMngrInfo.usedList
                                                                );

#ifdef VXWORKS
			semaphoreRelease( sTransMsgDbgSemaId);
#else
			semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sTransMsgDbgSemaId);
#endif
        }  
        else {
         	printf( "[TransIfoGet] - Sema Get Error !!\n");
		}

    }
    else{
     	status = 1;
    }

    if( (detailData != (strTransDetailInfo *)NULL)
                        &&
        (status == NO_ERROR)
       ) {

        
        memoryCopy ( (uint8 *) &dataPtr->peerAddr,
                     (uint8 *) &detailData->peerAddr,
                     (uint32) SOCK_ADDR_LENGTH
                   );
		dataPtr->totalBytesLen = detailData->totalBytesLen;
		memoryCopy ( (uint8 *)dataPtr->dataBuff,
                         (uint8 *)detailData->dataBuff,
                         (uint32)detailData->totalBytesLen
                    );

        

        status = semaphoreGet(sTransMsgDbgSemaId, FOREVERWAIT);

        if(status == NO_ERROR){
            nodePutOnLast ( (strDoubleLink **) &sTransQueMngrInfo.freeList,
                            (strDoubleLink *) detailData
                          );
#ifdef VXWORKS
			semaphoreRelease( sTransMsgDbgSemaId);
#else
			semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sTransMsgDbgSemaId);
#endif
        }
        else {
        	printf("[TransIfoGet] - Sema Get Error !!\n");
		}

    }
    return(status);
}



void setErrorCode
			(
		uint8 	*fileName,
        uint32	lineNum,
        uint8	*moduleName,
        uint32	errorCode
        	)
{
	int32	status = NO_ERROR;
	uint32	len = 0;
	strErrorInfo	*errPtr;
#ifdef VXWORKS
	boardLedBlink (USR_LED_1, USR_LED_OFF);
#endif

	
	if(gDebugErrStringFlag != DEBUG_ENABLE)
		return;

	status = semaphoreGet(sErrQSemaId, NOWAIT);

    if(status == NO_ERROR){
        
        errPtr = (strErrorInfo *)nodeGetAndUnlink (
                                    (strDoubleLink **)&sErrQInfo.freeList
                                                    );
        if(errPtr != (strErrorInfo *)NULL) {
            
              memoryClear(    (uint8 *)errPtr,
                            sizeof(strErrorInfo)
                        );
            if( (
                    len = strlen((const char*)fileName)
                ) > FILE_NAME_LEN
              )
                len = FILE_NAME_LEN;

            memoryCopy( (uint8 *)errPtr->fileName,
            			fileName,
                        len
					  );
            if( (
                    len = strlen((const char*)moduleName)
                ) > MAX_NAME_LEN
              )
                len = MAX_NAME_LEN;

            memoryCopy (    (uint8 *)errPtr->moduleName,
                            (uint8 *)moduleName,
                            len
                       );
          	errPtr->lineNumber = lineNum;
            errPtr->errorCode = errorCode;

            
            nodePutOnLast ( (strDoubleLink **) &sErrQInfo.usedList,
                            (strDoubleLink *) errPtr
                          );
        }

#ifdef VXWORKS
        semaphoreRelease( sErrQSemaId);
#else
        semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sErrQSemaId);
#endif
    }

	return;
}


uint32 debugQueueInit(void)
{
	uint32 status = NO_ERROR;
    strSemaInfo     sema;
	uint32 loop;

    
    
#ifdef VXWORKS
    sema.semaType       = MUTEX_SEMA;
    sema.semaOptions    = MYSEM_Q_PRIORITY|
                          MYSEM_DELETE_SAFE|
                          MYSEM_INVERSION_SAFE;
#else
    sema.semaType       = MUTEX_SEMA;
	sema.semaInitVal	= FALSE;
	sema.semaName		= "ErrorQ";
#endif
	
    status = semaphoreCreate(   (strSemaInfo *) &sema,
                                (int32 *) &sErrQSemaId
                            );

	if(status != NO_ERROR) {
		printf( "\n[%s(%d)] - [debugQueueInit] - [Semaphore Create Err] !!\n",
        						(uint8 *)__FILE__, __LINE__);

		return(status);
    }

    
    sErrQInfo.freeList = NULL;
    sErrQInfo.usedList = NULL;

    sDetailErrQInfo.freeList = NULL;
    sDetailErrQInfo.usedList = NULL;

    for( loop = 0; loop < MAX_ERR_QUEUE_NUM; loop++) {
        memoryClear (   (uint8 *) &sErrInfo[loop],
                        sizeof(strErrorInfo)
                    );
        (void) nodePutOnLast(   (strDoubleLink **) &sErrQInfo.freeList,
                                (strDoubleLink *) &sErrInfo[loop]
                            );

        memoryClear (   (uint8 *) &sDetailErrInfo[loop],
                        sizeof(strDetailErrorInfo)
                    );
        (void) nodePutOnLast(   (strDoubleLink **) &sDetailErrQInfo.freeList,
                                (strDoubleLink *) &sDetailErrInfo[loop]
                            );
    }
    return(status);
}


uint32 debugVarInit(void)
{
	uint32 status = NO_ERROR;

    
	schedulerDebugVarInit();

    
    memoryClear ( (uint8 *)gDebugRecvData,
    			  sizeof(strDebugDataInfo) * DEBUG_INFO_MAX_ARRAY_NUM
				);
 
	memoryClear ( (uint8 *)gDebugSendData,
    			  sizeof(strDebugDataInfo) * DEBUG_INFO_MAX_ARRAY_NUM
				);

	memoryClear ( 	(uint8 *) &sNetPerTransInfo,
    				sizeof(strNetworkPerInfo)
				);

    
	status = transInfoMngrInit();
	
	if(status != NO_ERROR) return(status);

	gDebugErrStringFlag 	= DEBUG_ENABLE;

	sISRDebugErrStringFlag  = DEBUG_DISABLE;

	gRecvInfoFlag = DEBUG_DISABLE;
	gSendInfoFlag = DEBUG_DISABLE;

    gTransBroadcastDetailRecvFlag 	= DEBUG_DISABLE;
    gTransUnicastDetailRecvFlag 	= DEBUG_DISABLE;
	gTransBroadcastDetailSendFlag 	= DEBUG_DISABLE;
	gTransUnicastDetailSendFlag 	= DEBUG_DISABLE;
	
	gTransRateCalcuFlag	= DEBUG_DISABLE;

    gUsrTaskSchedulingFlag 		= DEBUG_DISABLE; 
	gMpuSegmentSchedulingFlag 	= DEBUG_DISABLE;
    
    return(status);
}




void setErrorCodeWithVal
			(
		uint8 	*fileName,
        uint32	lineNum,
        uint8	*moduleName,
        uint32	errorCode,
        uint8   *errStr1,
        uint32	val1,
        uint8   *errStr2,
        uint32	val2,
        uint8   *errStr3,
        uint32	val3
        	)
{
	int32	status = NO_ERROR;
	uint32	len = 0;
	strDetailErrorInfo	*errPtr;
#ifdef VXWORKS
	boardLedBlink (USR_LED_1, USR_LED_OFF);
#endif

	
	if(gDebugErrStringFlag != DEBUG_ENABLE) 
		return;

    status = semaphoreGet(sErrQSemaId, NOWAIT);

    if(status == NO_ERROR){
        
		errPtr = (strDetailErrorInfo *)nodeGetAndUnlink (
                                    (strDoubleLink **)&sDetailErrQInfo.freeList
                                                  );

        if(errPtr != (strDetailErrorInfo *)NULL) {
            
            memoryClear(    (uint8 *)errPtr,
                            sizeof(strDetailErrorInfo)
                        );

			if( (
                    len = strlen((const char *)fileName)
                ) > MAX_NAME_LEN
              )
                len = MAX_NAME_LEN;
	
            memoryCopy( (uint8 *)errPtr->errInfo.fileName,
            			(uint8 *)fileName,
                        len
					  );

            if( (
                    len = strlen((const char *)moduleName)
                ) > MAX_NAME_LEN
              )
                len = MAX_NAME_LEN;

            memoryCopy (    (uint8 *)errPtr->errInfo.moduleName,
                            (uint8 *)moduleName,
                            len
                       );
			errPtr->errInfo.lineNumber = lineNum;
            errPtr->errInfo.errorCode = errorCode;

            if( (
                    len = strlen((const char *)errStr1)
                ) > MAX_ERROR_STRING_LEN
              )
                len = MAX_ERROR_STRING_LEN;
			
			memoryCopy ( 	(uint8 *)errPtr->errValInfo[0].errString,
            				errStr1,
                            len
						);
            if( (
                    len = strlen((const char *)errStr2)
                ) > MAX_ERROR_STRING_LEN
              )
                len = MAX_ERROR_STRING_LEN;
			
			memoryCopy ( 	(uint8 *)errPtr->errValInfo[1].errString,
            				errStr2,
                            len
						);
            if( (
                    len = strlen((const char *)errStr3)
                ) > MAX_ERROR_STRING_LEN
              )
                len = MAX_ERROR_STRING_LEN;

			memoryCopy ( 	(uint8 *)errPtr->errValInfo[2].errString,
            				errStr3,
                            len
						);
			errPtr->errValInfo[0].refVal = val1;
			errPtr->errValInfo[1].refVal = val2;
			errPtr->errValInfo[2].refVal = val3;

            
			nodePutOnLast ( (strDoubleLink **) &sDetailErrQInfo.usedList,
									(strDoubleLink *) errPtr
								  );
        }

#ifdef VXWORKS
		semaphoreRelease( sErrQSemaId);
#else
		semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sErrQSemaId);
#endif
    }
	return;
}


void setErrorCodeWithPort
			(
		uint8 	*fileName,
        uint32	lineNum,
        uint8	*moduleName,
        uint32	errorCode,
        uint32	portId
        	)
{
	int32	status = NO_ERROR;
	uint32	len = 0;
	strErrorInfo	*errPtr;

	boardLedBlink (USR_LED_1, USR_LED_OFF);

	
	if(sDebugErrStringPortId != portId)
		return;
	
    status = semaphoreGet(sErrQSemaId, NOWAIT);

    if(status == NO_ERROR){
        
        errPtr = (strErrorInfo *)nodeGetAndUnlink (
                                    (strDoubleLink **)&sErrQInfo.freeList
                                                    );
        if(errPtr != (strErrorInfo *)NULL) {
            
            memoryClear(    (uint8 *)errPtr,
                            sizeof(strErrorInfo)
                        );

            if( (
                    len = strlen((const char *)fileName)
                ) > FILE_NAME_LEN
              )
                len = FILE_NAME_LEN;

            memoryCopy( (uint8 *)errPtr->fileName,
            			fileName,
                        len
					  );
            if( (
                    len = strlen((const char *)moduleName)
                ) > MAX_NAME_LEN
              )
                len = MAX_NAME_LEN;

            memoryCopy (    (uint8 *)errPtr->moduleName,
                            (uint8 *)moduleName,
                            len
                       );
			errPtr->lineNumber = lineNum;
            errPtr->errorCode = errorCode;

            
            nodePutOnLast ( (strDoubleLink **) &sErrQInfo.usedList,
                            (strDoubleLink *) errPtr
                          );
        }
#ifdef VXWORKS
		semaphoreRelease( sErrQSemaId);
#else
		semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sErrQSemaId);
#endif
    }

	return;
}


void systemInitErrWithVal
			(
		uint8 	*fileName,
        uint32	lineNum,
        uint8	*moduleName,
        uint32	errorCode,
        uint8   *errStr1,
        uint32	val1,
        uint8   *errStr2,
        uint32	val2,
        uint8   *errStr3,
        uint32	val3
        	)
{
	uint8	errorString[128];
	uint32	loop = 0;

    memoryClear( (uint8 *)&errorString[0], 128);

	errorStringGet(errorCode, (uint8 *)&errorString[0]);

    for(loop = 0; loop < 3; loop++) 
    {
		printf("[%s(%d)]-[%s]-[%s]-[%s::0x%x]-[%s::0x%x]-[%s::0x%x]\n",
				fileName,lineNum,moduleName,errorString,
				errStr1, val1,errStr2, val2,errStr3, val3);
        
        sysTaskDelay(systemClockRateGet() * 2);
	}
	
	/* [V104] 2021.01.12 : 부팅 에러 발생시 boot Config 모드로 Set */
	bootParaConfig();

	return;	
}


static uint8 isrErrString[256];
static uint8 errString[128];

void errorCodeSetInISR
			(
		uint8 	*fileName,
        uint32	lineNum,
        uint8	*moduleName,
        uint32	errorCode
        	)
{

	if(sISRDebugErrStringFlag != DEBUG_ENABLE)
		return;
	memoryClear( errString, 128);
	memoryClear( isrErrString, 256);
	
    
	errorStringGet(errorCode, errString);

#ifdef _WIN32
	sprintf((char *)isrErrString,"ISR::%s",moduleName);

    sysMsgLogSet((uint8*)fileName,lineNum,(uint8*)&isrErrString,SYSTEM_ISR_ERR,
				 (uint8*)&errString,errorCode);
#endif 

#ifdef VXWORKS
	sprintf((char *)isrErrString,"[ISR::%s(%d)] - [%s] - [%s(0x%x)]\n", fileName, lineNum,
				moduleName, errString, errorCode);
	sysMsgLogSet(isrErrString,0,0,0,0,0,0);
#endif
	return;
} 


#ifdef _WIN32
void sysMsgLogSet
			(
		uint8 	*fileName,   uint32	lineNum,
        uint8	*moduleName, uint32	errorCode,
        uint8   *errStr1,    uint32	val1
		)
{	
	int32	status = NO_ERROR;
	uint32	len = 0;
	strDetailErrorInfo	*errPtr;


	if(sISRDebugErrStringFlag != DEBUG_ENABLE) 
		return;

    status = semaphoreGet(sErrQSemaId, NOWAIT);

    if(status == NO_ERROR){
        
		errPtr = (strDetailErrorInfo *)nodeGetAndUnlink (
                                    (strDoubleLink **)&sDetailErrQInfo.freeList
                                                  );

        if(errPtr != (strDetailErrorInfo *)NULL) {
            
            memoryClear(    (uint8 *)errPtr,
                            sizeof(strDetailErrorInfo)
                        );
			if( (
                    len = strlen((const char *)fileName)
                ) > MAX_NAME_LEN
              )
                len = MAX_NAME_LEN;
	
            memoryCopy( (uint8 *)errPtr->errInfo.fileName,
            			(uint8 *)fileName,
                        len
					  );

            if( (
                    len = strlen((const char *)moduleName)
                ) > MAX_NAME_LEN
              )
                len = MAX_NAME_LEN;

            memoryCopy (    (uint8 *)errPtr->errInfo.moduleName,
                            (uint8 *)moduleName,
                            len
                       );
            if( (
                    len = strlen((const char *)errStr1)
                ) > MAX_ERROR_STRING_LEN
              )
                len = MAX_ERROR_STRING_LEN;
			
			memoryCopy ( 	(uint8 *)errPtr->errValInfo[0].errString,
            				errStr1,
                            len
						);
			errPtr->errInfo.lineNumber = lineNum;
            errPtr->errInfo.errorCode = errorCode;
			errPtr->errInfo.tickCountOfErrOccur = systemTickGet();
			errPtr->errValInfo[0].refVal = val1;

			
			errPtr->errValInfo[1].errString[0] = 0;
			errPtr->errValInfo[2].errString[0] = 0;
			errPtr->errValInfo[1].refVal = 0;
			errPtr->errValInfo[2].refVal = 0;
   
            
			nodePutOnLast ( (strDoubleLink **) &sDetailErrQInfo.usedList,
									(strDoubleLink *) errPtr
								  );
        }

		semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sErrQSemaId);
    }
	return;
}
#endif


void errorQueueCountDisplay(void)
{
	uint32	freeListCount = 0;
    uint32	usedListCount = 0;
	uint32	freeListCount1 = 0;
    uint32	usedListCount1 = 0;
	uint32	status = NO_ERROR;
	strErrorInfo	*nodePtr;
	strDetailErrorInfo *nodePtr1;

    
    status = semaphoreGet(sErrQSemaId, FOREVERWAIT);
	if(status != NO_ERROR) 
	{
		printf( "[Error Queue] - [Get Semaphore Error]\n");
        goto MODULE_END;
    }
	
    
	nodePtr = sErrQInfo.freeList;
    while(nodePtr != NULL) 
   	{
    	freeListCount++;
        nodePtr = (strErrorInfo *)nextNodeRef((strDoubleLink *)sErrQInfo.freeList,
											  (strDoubleLink *)nodePtr);
	}

	nodePtr = sErrQInfo.usedList;
    while(nodePtr != NULL) 
    {
    	usedListCount++;
        nodePtr = (strErrorInfo *)nextNodeRef((strDoubleLink *)sErrQInfo.usedList,
											  (strDoubleLink *)nodePtr);	
	}

    
	freeListCount1 = 0;
    usedListCount1 = 0;

	nodePtr1 = sDetailErrQInfo.freeList;
    while(nodePtr1 != NULL) 
    {
    	freeListCount1++;
        nodePtr1 = (strDetailErrorInfo *)nextNodeRef((strDoubleLink *)sDetailErrQInfo.freeList,
													 (strDoubleLink *)nodePtr1);
	}

	nodePtr1 = sDetailErrQInfo.usedList;
    while(nodePtr1 != NULL) 
    {
    	usedListCount1++;
        nodePtr1 = (strDetailErrorInfo *)nextNodeRef((strDoubleLink *)sDetailErrQInfo.usedList,
													 (strDoubleLink *)nodePtr1);
	}

#ifdef VXWORKS
	semaphoreRelease( sErrQSemaId);
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sErrQSemaId);
#endif

   
    printf( "\t************[Runtime Error Queue : %d]************\n",MAX_ERR_QUEUE_NUM);
	printf( "\tFreeList : %d		UsedList : %d\n",freeListCount, usedListCount);
	
	printf("\n");
	
    printf( "\t************[Runtime Detail Error Queue : %d]******\n",MAX_ERR_QUEUE_NUM);
	printf( "\tFreeList : %d		UsedList : %d\n",freeListCount1, usedListCount1);

MODULE_END:
	return;
}


void debugCountSet(uint32 *dataPtr)
{
	*dataPtr += 1;
	if(*dataPtr > 0xffff)
    	*dataPtr = 0x1;
	
}

void debugValSet(uint32 *destPtr, uint32 val)
{
	*destPtr = val;
}


uint32 netMsgChecksumCheckForDebugging(uint32 dataLen,strNewNetProtocolInfo *dataPtr)
{
	uint32	status=NO_ERROR;
	uint32 retCheckSum =0;
    uint32 *cmdCode, opCode, optionCode;

	if(gDebugErrStringFlag != DEBUG_ENABLE) return(status);

	cmdCode = (uint32*)(&dataPtr->dataBuff[0]);
 	opCode = (*cmdCode)>>16;
	optionCode = (*cmdCode)&0xffff;

	 
	netMsgChecksumCalcu	(
			dataLen,
            (uint32	*)&dataPtr->dataBuff[0],
            (uint32	*)&retCheckSum
            	);
	
	if(dataPtr->checkSum != retCheckSum) {
		status = MSG_CHECKSUM_ERR;
    	setErrorCodeWithVal((uint8 *)__FILE__, __LINE__, 
							(uint8 *)"SendTask",MSG_CHECKSUM_ERR,
        					(uint8 *)"opCode",opCode,
							(uint8 *)"optionCode", optionCode,
                            (uint8 *)"Msg Len", dataLen);
	}

	return (status);
}


void debugInfoSet(strMsgQueData *dataPtr, strDebugDataInfo *debugPtr)
{
    uint32  sockLen = (uint32)SOCK_ADDR_LENGTH;
    strDebugDataInfo    *curPtr = debugPtr;
    int32   curIndex, i, *cmdCode;
    strNewNetProtocolInfo    *netMsgPtr;

    
    curIndex = debugInfoCheck(dataPtr, debugPtr);

    
    for(i = 0; i < curIndex; i++) curPtr++;

    
    memoryCopy( (uint8 *)&curPtr->peerAddr,
                (uint8 *)&dataPtr->peerAddr,
                sockLen
			  );

    netMsgPtr = (strNewNetProtocolInfo *)dataPtr->protoData.msgData;

    
    curPtr->seqNum      = netMsgPtr->sequenceID;
    curPtr->msgDataSize = dataPtr->protoData.numOfBytes;

	cmdCode = (int32*)(&netMsgPtr->dataBuff[0]);
 	curPtr->opCode = (*cmdCode)>>16;
	curPtr->optionCode = (*cmdCode)&0xffff;
    curPtr->transTotalCount++;
}


static uint32 debugInfoCheck(strMsgQueData *dataPtr, strDebugDataInfo *debugPtr)
{
    int32 index = 0;
    uint32 ipHexVal = dataPtr->peerAddr.iAddr;
    uint16 portNum = dataPtr->peerAddr.port;
    uint16 tmpPortNum;
    uint32  tmpHexVal;
    strDebugDataInfo    *tmpPtr = debugPtr;
	strNewNetProtocolInfo *netMsgPtr;
	uint32 opCode, optionCode, *cmdCode;

    netMsgPtr = (strNewNetProtocolInfo *)dataPtr->protoData.msgData;
	
	cmdCode = (uint32*)(&netMsgPtr->dataBuff[0]);
 	opCode = (*cmdCode)>>16;
	optionCode = (*cmdCode)&0xffff;

 
    
    for (index = 0; index < DEBUG_INFO_MAX_ARRAY_NUM; index++, tmpPtr++) {
        tmpHexVal = tmpPtr->peerAddr.iAddr;
        tmpPortNum = tmpPtr->peerAddr.port;

        if( (ipHexVal == tmpHexVal)
                    &&
            (portNum == tmpPortNum)
            		&&
			(opCode == tmpPtr->opCode)
            		&&
			(optionCode == tmpPtr->optionCode)
          )
            break;
    }
    if(index == DEBUG_INFO_MAX_ARRAY_NUM) {
        tmpPtr = debugPtr;

        
        
        for(index = 0; index < DEBUG_INFO_MAX_ARRAY_NUM; index++, tmpPtr++){
            if(tmpPtr->peerAddr.iAddr == 0x0)
                break;
        }
        
        if(index == DEBUG_INFO_MAX_ARRAY_NUM)
            index = DEBUG_INFO_MAX_ARRAY_NUM - 1;
    }
    return(index);
}



static uint32 transInfoMngrInit(void)
{
    uint32 status = NO_ERROR;
    uint32 loop;
	strSemaInfo	sema;

#ifdef VXWORKS	
    sema.semaType       = MUTEX_SEMA;
    sema.semaOptions    = MYSEM_Q_PRIORITY|
                          MYSEM_DELETE_SAFE|
                          MYSEM_INVERSION_SAFE;
#else			

    sema.semaType       = MUTEX_SEMA;
	sema.semaInitVal	= FALSE;
	sema.semaName		= "TransMsgDbgQ";
#endif
	
    status = semaphoreCreate(   (strSemaInfo *) &sema,
                                (int32 *)&sTransMsgDbgSemaId );


    if(status) {goto MODULE_END;}

    sTransQueMngrInfo.freeList = NULL;
    sTransQueMngrInfo.usedList = NULL;

    for(loop = 0; loop < TRANS_DETAIL_INFO_QUE_NUM; loop++) 
    {
        memoryClear((uint8 *) &sTransDetailData[loop],
				    sizeof(strTransDetailInfo));

        nodePutOnLast((strDoubleLink **)&sTransQueMngrInfo.freeList,
					  (strDoubleLink*) &sTransDetailData[loop]);
    }

MODULE_END:
    return(status);
}


uint32  transInfoSet( strMsgQueData *dataPtr)
{
    strTransDetailInfo *infoPtr = (strTransDetailInfo *)NULL;
    uint32 status = NO_ERROR;
    strNewNetProtocolInfo *netMsgPtr  = (strNewNetProtocolInfo *)dataPtr->protoData.msgData;
    
	if(gTransUnicastDetailRecvFlag || gTransUnicastDetailSendFlag) 
	{
		if(netMsgPtr->messageType == BROADCAST)
			return(status);
	}
	else if(gTransBroadcastDetailRecvFlag || gTransBroadcastDetailSendFlag) 
	{
		if((netMsgPtr->messageType == REQUEST) || (netMsgPtr->messageType == RESPONSE)) 
			return(status);	
	}

	else 
	{
		printf( "[transInfoSet] - Unknown Msg Type !!\n");
		return(status);
	}	

#ifdef VXWORKS
	status = semaphoreGet(sTransMsgDbgSemaId, FOREVERWAIT);
#else
	status = semaphoreGet(sTransMsgDbgSemaId, FOREVERWAIT);
#endif

    if(status == NO_ERROR)
    {       
        infoPtr = (strTransDetailInfo *)nodeGetAndUnlink (
                                    (strDoubleLink **)&sTransQueMngrInfo.freeList
                                                         );
        if(infoPtr != (strTransDetailInfo *)NULL) 
        {
            
            memoryCopy( (uint8 *)&infoPtr->peerAddr,
                        (uint8 *)&dataPtr->peerAddr,
                        (uint32)SOCK_ADDR_LENGTH
                       );

            
			infoPtr->totalBytesLen = dataPtr->protoData.numOfBytes;
			memoryCopy ( (uint8 *)infoPtr->dataBuff,
                         (uint8 *)&dataPtr->protoData.msgData,
                         (uint32)dataPtr->protoData.numOfBytes
                       );
            
            nodePutOnLast ( (strDoubleLink **) &sTransQueMngrInfo.usedList,
                            (strDoubleLink *) infoPtr
                          );
        }
        	
#ifdef VXWORKS
        semaphoreRelease( sTransMsgDbgSemaId);
#else
        semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sTransMsgDbgSemaId);
#endif
    }
    else 
    {
		printf("[transInfoSet] - Sema Get Error\n");
	}
    return(status);
}

void transQueueCountDisplay(void)
{
	uint32	freeListCount = 0;
    uint32	usedListCount = 0;
	uint32	status = NO_ERROR;

	strTransDetailInfo	*nodePtr;

    
    status = semaphoreGet(sTransMsgDbgSemaId, FOREVERWAIT);
	if(status != NO_ERROR) 
	{
		printf( "[Error Queue] - [Get Semaphore Error]\n");
        return;
    }
	
    
	nodePtr = sTransQueMngrInfo.freeList;
    while(nodePtr != NULL) 
    {
    	freeListCount++;
        nodePtr = (strErrorInfo *)nextNodeRef((strDoubleLink *)sTransQueMngrInfo.freeList,
											  (strDoubleLink *)nodePtr);
	}

	nodePtr = sTransQueMngrInfo.usedList;
    while(nodePtr != NULL) 
    {
    	usedListCount++;
        nodePtr = (strErrorInfo *)nextNodeRef((strDoubleLink *)sTransQueMngrInfo.usedList,
											  (strDoubleLink *)nodePtr);	
	}

#ifdef VXWORKS
	semaphoreRelease( sTransMsgDbgSemaId);
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sTransMsgDbgSemaId);
#endif

   
    printf( "\t************[Network Com. Info Queue : %d]************\n",TRANS_DETAIL_INFO_QUE_NUM);
	printf( "\tFreeList : %d		UsedList : %d\n",freeListCount, usedListCount);

	return;
}



void networkPerformanceCountSet(uint32 packetKind, uint32 byteLen)
{
	if(packetKind == SEND_PACKET_F) {
    	sNetPerTransInfo.totalSendCount += 1;
        sNetPerTransInfo.byteLenOfSendingPacket += byteLen;
    }
    else if(packetKind == RECV_PACKET_F) {
    	sNetPerTransInfo.totalRecvCount += 1;
        sNetPerTransInfo.byteLenOfRecvingPacket += byteLen;
	}
	else {
		printf("[ERROR] - Packet Kind Argument Error !!\n"); 
	}
}



void debugStringDisplay	(	uint8 *moduleName, 
							uint8 *string, 
							uint32 data1, 
							uint32 data2
						)
{
	uint8	stringBuf[512];

	if(gDebugErrStringFlag != DEBUG_ENABLE)
		return;

	memoryClear ( stringBuf, 512);

    sprintf((char *)stringBuf, "[%s]-[%s]-[Data : 0x%x]-[Data : 0x%x]\n", 
				moduleName, string, data1, data2);
	logMsg(stringBuf,0,0,0,0,0,0); 

}


void debugMsgDisplayInISR(	uint8 *moduleName, uint8 *msgPtr, 
							uint8 *fileName, uint32 line,
							uint32 data1,uint32 data2,uint32 data3,uint32 data4
						)	
{
	uint8	stringBuf[512];

	boardLedBlink (USR_LED_1, USR_LED_OFF);

	if (gDebugErrStringFlag == DEBUG_ENABLE) {
		memoryClear ( stringBuf, 512);

	    sprintf( stringBuf, "-------[%s]-[%s]-[%d](%s::%d)\n",moduleName, msgPtr,data1, fileName, line);
#ifdef VXWORKS
		sysMsgLogSet(stringBuf, 0,0,0,0,0,0);
#else
		sysMsgLogSet((uint8*)fileName,line,(uint8*)&moduleName,SYSTEM_ISR_ERR,
			 (uint8*)&msgPtr,data1);
#endif
	}

    return;
}


void dataDisplayForRmtDevice(uint32 flag, uint32 arg1) 
{
	while( !keyHit() ) {

    	if(flag == RECV_PACKET_F)
        	gRmtDevRecvDisplayFlag = DEBUG_ENABLE;
		else
        	gRmtDevSendDisplayFlag = DEBUG_ENABLE;
		
        sysTaskDelay(systemClockRateGet());
    }
   	gRmtDevRecvDisplayFlag = DEBUG_DISABLE;
   	gRmtDevSendDisplayFlag = DEBUG_DISABLE;
}



void cmdTransDataInfoDisplay(uint32 arg1, uint32 arg2)
{
    strTransDetailInfo  detailData;
	strCommandListInfo  cmdList;
	uint32 i, opCode = 0, optionCode = 1 , maxOptionCode = 1;
	uint32 transInfoFlag = 0; 
    int8    *cPtr, line[32];


	
	printf( "  ***************************[OP Code List]**************************\n");
	for(i=0;i< MAX_PROTOCOL_CMD;i++) {
	    memoryClear((uint8 *) &cmdList, sizeof(strCommandListInfo));
		opCode = i+1;
		commandStringGet( &cmdList, opCode, optionCode);
		printf( "   %02d(0x%02x) : %20s", opCode,opCode, cmdList.opCode);
		if(i%2) printf( "\n");
	}
	printf( "\n\n");

OPCODESET:
	printf( "  Enter the Opcode[1-%d]? ",MAX_PROTOCOL_CMD);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);
    opCode = atoi(cPtr);
	
	if(*cPtr=='.') return;
	else if(opCode<1 || opCode>MAX_PROTOCOL_CMD) {
		printf( "Invalid OPCODE!!!\n");
		goto OPCODESET;
	}
	
	
	optionCode=1;
	memoryClear((uint8 *) &cmdList, sizeof(strCommandListInfo));
	commandStringGet( &cmdList, opCode, optionCode);

	printf( "  *************************[Option Code List]************************\n");
	while(strcmp(cmdList.optionCode,"UNKNOWN COMMAND")&&
		strcmp(cmdList.optionCode,"UNKNOWN OPTION CODE"))
	{
		printf( "   %02d(0x%02x) : %20s", optionCode,optionCode, cmdList.optionCode);
		optionCode++;
		memoryClear((uint8 *) &cmdList, sizeof(strCommandListInfo));
		commandStringGet( &cmdList, opCode, optionCode);
		if(optionCode%2) printf( "\n");
		 
		if( (optionCode>0x0f)||(strlen(cmdList.optionCode) > 32 ) ) break;
	}
	printf( "\n\n");
	maxOptionCode = optionCode-1 ; 

OPTIONSET:
	printf( "  Enter the Option code[1-%d]? ", maxOptionCode);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);
    optionCode = atoi(cPtr);

	if(*cPtr=='.') return;
	else if( (optionCode<1) ||  (optionCode > maxOptionCode)  ) {
		printf( "Invalid OPTION CODE!!!\n");
		goto OPTIONSET;
	}

	
FLGASET:
	printf( "  Enter the RECV(1) or SEND(2)? ");
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);
    transInfoFlag = atoi(cPtr);

	if(*cPtr=='.') return;
	else if( (transInfoFlag<DETAIL_DATA_FOR_RECV) ||  (transInfoFlag > DETAIL_DATA_FOR_SEND)  ) 
	{
		printf( "Invalid RECEIVE(1) or SEND(2)!!!\n");
		goto FLGASET;
	}

	
	if( ((opCode == TIME_ACCESS_CMD )&&(optionCode ==TIME_SYNC_BROAD )) ||
		((opCode == SYS_STATUS_CMD )&&(optionCode == SYS_DIAG_STATUS)) 	||
		((opCode == XR_BROADCAST_CMD )&&(optionCode == XR_SEND )) || 
		((opCode == ETHERNET_DATA_ACC_OP)&&(optionCode ==ETHERNET_DATA_BROAD ))
		) 
	{
		if(transInfoFlag == DETAIL_DATA_FOR_RECV) 
			gTransBroadcastDetailRecvFlag = 1;
		else if(transInfoFlag == DETAIL_DATA_FOR_SEND) 
		{
			gTransBroadcastDetailSendFlag = 1;
		}
	}
	else 
	{
		if(transInfoFlag == DETAIL_DATA_FOR_RECV) 
			gTransUnicastDetailRecvFlag = 1;
		else if(transInfoFlag == DETAIL_DATA_FOR_SEND) 
			gTransUnicastDetailSendFlag = 1;
	}
	printf( "  Waiting Command Downloading....\n\n");

    while(!keyHit()) 
   	{
        if(transInfoGet(&detailData) == NO_ERROR) 
		{
            cmdTransDataInfoPrint(&detailData, opCode, optionCode, transInfoFlag) ;
		}
        
        
        sysTaskDelay(systemClockRateGet()/2);
    }
	
	gTransUnicastDetailSendFlag 	= 0;
	gTransBroadcastDetailSendFlag 	= 0;
	gTransUnicastDetailRecvFlag 	= 0;
	gTransBroadcastDetailRecvFlag 	= 0;

    
    sysTaskDelay(systemClockRateGet()/2);

    
    (void)transInfoQueClear();
}


static void cmdTransDataInfoPrint( strTransDetailInfo *dataPtr, uint32 setOpCode, 
								  uint32 setOptionCode, uint32 transInfoFlag)
{
    int8    *msgType[] =
    {
        (int8 *)NULL,
        (int8 *)"REQUEST",
		(int8 *)"REPLY",
		(int8 *)"BROADCAST",
        (int8 *)NULL
    };

	int8   *cString[] = 
	{
		(int8 *)NULL,
		(int8 *)"RECV DATA DETAIL INFO",
		(int8 *)"SEND DATA DETAIL INFO",
		(int8 *)NULL
	};

    uint8   *dispDataPtr;
    uint8   ipArray[20];
    strCommandListInfo  cmdList;

	strNewNetProtocolInfo *netMsgPtr;

	uint32 opCmd, opCode, optionCode;

	netMsgPtr = (strNewNetProtocolInfo *)dataPtr->dataBuff;

	memoryCopy((uint8*)&opCmd, (uint8*)&netMsgPtr->dataBuff[0], DOUBLE_SIZE);
	opCode = (opCmd & 0xffff0000) >> 16;
	optionCode = (opCmd & 0xffff);

    if ( (setOpCode != opCode) || (setOptionCode != optionCode))
	{ 
			goto MODULE_END;
    }
	
    memoryClear((uint8 *)ipArray, 20);
    memoryClear((uint8 *) &cmdList, sizeof(strCommandListInfo));

    printf( "\n\t***********************[%s]********************\n", cString[transInfoFlag]);

    ipAddrHexToString((int8 *)ipArray, dataPtr->peerAddr.iAddr);

    if(transInfoFlag == DETAIL_DATA_FOR_RECV) {
        printf( "\t\tSource Internet Address : %s\n",ipArray);
	}
    else {
        printf( "\t\t  Dest Internet Address : %s\n",ipArray);
	}
    printf( "\t\t            Port Number : %d\n", ntohs(dataPtr->peerAddr.port));

    printf( "\t\t         Total Byte Len : %d\n", dataPtr->totalBytesLen);
    printf( "\t\t        Sequence Number : 0x%x\n", netMsgPtr->sequenceID);
    printf( "\t\t              Source ID : 0x%x\n", netMsgPtr->sourceID);
    printf( "\t\t         Destination ID : 0x%x\n", netMsgPtr->destinationID);

    if( (netMsgPtr->messageType < REQUEST) || (netMsgPtr->messageType > BROADCAST)) {
        printf( "\t\t           Message Type : UNKNOWN[0x%x]\n", netMsgPtr->messageType);
	}
    else {
        printf( "\t\t           Message Type : %s\n", msgType[netMsgPtr->messageType]);
	}
    printf( "\t\t                Dual ID : 0x%x\n", netMsgPtr->dualStatus);
    commandStringGet(   &cmdList,
                        opCode, 
                        optionCode
                    );
    printf( "\t\t                OP Code : %s\n", cmdList.opCode);
    printf( "\t\t            Option Code : %s\n", cmdList.optionCode);

    
    dispDataPtr = (uint8 *)netMsgPtr;
	dataDisplay(    (uint8 *)"PROTOCOL DATA",
					dispDataPtr,
					(uint16)dataPtr->totalBytesLen
				);

    printf( "\n");


MODULE_END:

	return;
}


uint32 rasDetailInfoPrint(uint8*,uint32);

void rasDetailInfoDisplay(uint32 arg1, uint32 arg2)
{
	strTransDetailInfo detailData;

	gTransBroadcastDetailSendFlag = 1;
	
    while(!keyHit()) 
    {
        if(transInfoGet(&detailData) == NO_ERROR) 
		{
            rasDetailInfoPrint((uint8*)&detailData.dataBuff[0], detailData.totalBytesLen ) ;
		}
        
        
        sysTaskDelay(systemClockRateGet()/2);
    }
	gTransBroadcastDetailSendFlag = 0;
  
	
    sysTaskDelay(systemClockRateGet()/2);

	
    (void)transInfoQueClear();
    
	return;
}


void rasDetailKindInfoPrint(strNewRasSummaryInfo *rasInfoDataPtr, uint32 kind)
{
	uint32 i,j,lineCnt = 0;
	strRasHeadInfo *rasheadInfoPtr;
	strMpuFbcRasHeaderInfo* mpuRasHeadInfoPtr;
    strMpuFbcRasHeaderInfo * fbcRasHeadInfoPtr;
	strTmtcRasSummaryInfo *tmtcRasInfoPtr;
	strFieldbusRasSummaryInfo *fieldbusRasInfoPtr;	
	strSysConfigInfo sysCnfgInfo;
	
	if(kind==RAS_CNF_CHANGE_SET){
		rasheadInfoPtr = (strRasHeadInfo *)&rasInfoDataPtr->rasheadInfo;
		printf( "\n ** RAS DATA HEADER \n");

		printf( "  00. [OPCODE/OPTIONC CODE] 0x%x, 0x%x\n",
			((rasheadInfoPtr->opCommand>>16)&0xffff) , (rasheadInfoPtr->opCommand&0xffff) );

		printf( "  01. [DATE] Year:%d, Month:%d, Day:%d \n",
				(rasheadInfoPtr->YearMonthDay >>16)&0xffff,
				(rasheadInfoPtr->YearMonthDay >>8)&0xff ,
				(rasheadInfoPtr->YearMonthDay&0xff) ) ;

		printf( "  02. [TIME] Hour:%d, Mimute:%d, Sec:%d, mSec:%d \n",
			(rasheadInfoPtr->HourMinSecmSec >>24)&0xff,(rasheadInfoPtr->HourMinSecmSec >>16)&0xff,
			(rasheadInfoPtr->HourMinSecmSec >>8)&0xff,(rasheadInfoPtr->HourMinSecmSec&0xff) ) ;

		printf( "  03. (1)MAS/SLS:%d ,(2)AUTO/MAN:%d, (3)RUN/STBY: %d, (4)MAR/SLR:%d , (5)RUN/STOP:%d \n",
			(rasheadInfoPtr->dualStatus >>8)&0x3,(rasheadInfoPtr->dualStatus >>6)&0x3 ,
			(rasheadInfoPtr->dualStatus >>4)&0x3 ,(rasheadInfoPtr->dualStatus >>2)&0x3,
			(rasheadInfoPtr->dualStatus&0x3) ) ;
		
		printf( "  04. (1)LOGIC_ID:0x%x, (2)MPU_FORCING:%d, (3)DIU_ID:0x%x, (4)DIU_FORCING:%d \n",
			(rasheadInfoPtr->forcingStatus >>24)&0xff,
			(rasheadInfoPtr->forcingStatus >>16)&0xff ,
			(rasheadInfoPtr->forcingStatus >>8)&0xff,
			(rasheadInfoPtr->forcingStatus&0xff) ) ;

		printf( "  05. (1)[LINE CNT] BLINE_CNT:0x%x, (2)ALINE_CNT:0x%x \n",
				(rasheadInfoPtr->lineCount >>16)&0xffff,(rasheadInfoPtr->lineCount&0xffff) ) ;

		

		printf( "  06. ~ 13. [RESEVED 1~8] \n") ;
	}
	else if(kind==RAS_MPU_CHANGE_SET){
		lineCnt = sizeof(strRasHeadInfo)/DOUBLE_SIZE;
		mpuRasHeadInfoPtr = (strMpuFbcRasHeaderInfo*) &rasInfoDataPtr->mpuRasInfo.mpuRasSummary;
		printf( "\n ** MPU RAS DATA \n");

		printf( " %3d. [MPU SUM],(1)DC:%d,(2)JP:%d,(3)RD:%d,(4)OT:%d,(5)FB:%d,(6)CS:%d\n",
			lineCnt++, (mpuRasHeadInfoPtr->rasSummary >>5)&0x1,(mpuRasHeadInfoPtr->rasSummary >>4)&0x1,
			(mpuRasHeadInfoPtr->rasSummary >>3)&0x1 , (mpuRasHeadInfoPtr->rasSummary >>2)&0x1 ,
			(mpuRasHeadInfoPtr->rasSummary >>1)&0x1,(mpuRasHeadInfoPtr->rasSummary&0x1) ) ;

		printf( " %3d. [MPU LIVECNT]:0x%x \n", lineCnt++,mpuRasHeadInfoPtr->liveCount) ;
		printf( " %3d. [MPU ROM VERSION]:%3x \n", lineCnt++,mpuRasHeadInfoPtr->romVersion) ;

		printf( " %3d. [RELEASE DATE] Year:%d, Month:%d, Date:%d\n", lineCnt++,
			(mpuRasHeadInfoPtr->releaseDate >>16)&0xffff,
			(mpuRasHeadInfoPtr->releaseDate >>8)&0xff,
			(mpuRasHeadInfoPtr->releaseDate&0xff) ) ;
		printf( "      [TASK STATUS] 0:NO_MODE,1:INIT,2:DWL,3:DWLD,4:STOP,5:RUN,6:DEBUG,7:ERR\n");
		printf( "\t\t    |%d|%d|%d|%d|%d|%d|%d|%d|",1,2,3,4,5,6,7,8);
		printf( "\t\t\t     |%d|%d|%d|%d|%d|%d|%d|%d|\n",1,2,3,4,5,6,7,8);
		for(i = 0; i < MAX_LOGIC_TASK_NUM/8; i++) {
			printf( " %3d. [TASK %03d~%03d]"
				,lineCnt ,i*8+8, i*8+1);
			lineCnt++;
			for(j = 8; j > 0; j--) {
				printf( " %d", 
					( (rasInfoDataPtr->mpuRasInfo.mpuTaskStatus[i] >> (4*((j-1)%8))&0xf) ) );
			}
			if(((i+1)%2)==0) printf( "\n");
			else printf( "   | ");
		}
	}
	else if(kind==RAS_FBC_CHANGE_SET){
		lineCnt = (sizeof(strRasHeadInfo)+sizeof(strMpuRasSummaryInfo))/DOUBLE_SIZE;
		printf( "\n ** FBC RAS DATA \n");
		fbcRasHeadInfoPtr = (strMpuFbcRasHeaderInfo *)&rasInfoDataPtr->fbcRasInfo.fbcRasSummary;

		printf( " %3d. [FBC SUM](1)DC:%d,(2)COM:%d,(3)LC:%d,(4)RD:%d,(5)OT: %d,(6)FB:%d,(7)CS:%d \n",
			lineCnt++,(fbcRasHeadInfoPtr->rasSummary >>6)&0x1, (fbcRasHeadInfoPtr->rasSummary >>5)&0x1,
			(fbcRasHeadInfoPtr->rasSummary  >>4)&0x1,(fbcRasHeadInfoPtr->rasSummary  >>3)&0x1 , 
			(fbcRasHeadInfoPtr->rasSummary  >>2)&0x1 ,
			(fbcRasHeadInfoPtr->rasSummary  >>1)&0x1,(fbcRasHeadInfoPtr->rasSummary &0x1) ) ;

	    printf( " %3d. [FBC LIVECNT]:0x%x \n", lineCnt++,fbcRasHeadInfoPtr->liveCount ) ;
		printf( " %3d. [FBC ROM VERSION]:%3x \n", lineCnt++, fbcRasHeadInfoPtr->romVersion) ;
		printf( " %3d. [RELEASE DATE] (1)Year:%d, (2)Month:%d, (3)Date:%d\n\n",
			 lineCnt++, (fbcRasHeadInfoPtr->releaseDate >>16)&0xffff,
			 (fbcRasHeadInfoPtr->releaseDate >>8)&0xff,(fbcRasHeadInfoPtr->releaseDate)&0xff ) ;
	}
	else if(kind == RAS_DIU_CHANGE_SET){
		printf( "\n ** IO SHELF RAS DATA \n");
		for(i = 0; i < MAX_IO_SHELF_NUM; i++) {
			rasDetailIOShelfInfoPrint((strNewRasSummaryInfo *)rasInfoDataPtr,  i);
		}
	}
	else if((kind == RAS_TMTC_CHANGE_SET)||(kind == RAS_TMTC_CHANGE_SET_PRINT) )
	{
		lineCnt = (sizeof(strNewRasSummaryInfo) - sizeof(strTmtcRasSummaryInfo)
					- sizeof(strFieldbusRasSummaryInfo))/DOUBLE_SIZE;

		if(kind == RAS_TMTC_CHANGE_SET)
			(void)systemCnfgInfoRead( &sysCnfgInfo);

		{
			printf( " %3d. ~ %3d. ALL ZERO \n",
				lineCnt, lineCnt+sizeof(strTmtcRasSummaryInfo)/4 -1 ) ;
		}
	}
#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
	else if( (kind == RAS_FMC_CHANGE_SET)||(kind == RAS_FMC_CHANGE_SET_PRINT) ){
		lineCnt = (sizeof(strNewRasSummaryInfo) - sizeof(strFieldbusRasSummaryInfo))/DOUBLE_SIZE;

		(void)systemCnfgInfoRead( &sysCnfgInfo);

		{ 
			printf( " %3d. ~ %3d. ALL ZERO \n",
				lineCnt, lineCnt+sizeof(strFieldbusRasSummaryInfo)/DOUBLE_SIZE -1 ) ;
		}
	}
#endif 
	return;
}

static void rasDetailIOShelfInfoPrint(strNewRasSummaryInfo *rasInfoDataPtr, uint32 i)
{
	strIORasSummaryInfo* ioRasInfoPtr;
	uint32 j,lineCnt;

	lineCnt = (sizeof(strRasHeadInfo)+sizeof(strMpuRasSummaryInfo)
				+sizeof(strMpuFbcRasHeaderInfo))/DOUBLE_SIZE;

	ioRasInfoPtr = (strIORasSummaryInfo*) &rasInfoDataPtr->fbcRasInfo.ioRasSummary[i];
	lineCnt += (sizeof(strIORasSummaryInfo)/DOUBLE_SIZE)*i;
	printf( " [IO SHELF NUMBER] #%d\n",i+1);
	
	printf( " %3d. [(1)SH/(2)MD/(3)MIS/(4)DS/(5)DM]",lineCnt++);
	printf( "%2d %d  %d%d%d\n",
		(ioRasInfoPtr->ioShelfRasSummary>>28)&0xf,(ioRasInfoPtr->ioShelfRasSummary>>24)&0x7 , 
		(ioRasInfoPtr->ioShelfRasSummary>>14)&0x1 ,(ioRasInfoPtr->ioShelfRasSummary>>13)&0x1 ,
		(ioRasInfoPtr->ioShelfRasSummary>>12)&0x1);

	printf( "      [ERR(12~1)] (6)%d%d%d%d (10)%d%d%d%d (14)%d%d%d%d \n",
		(ioRasInfoPtr->ioShelfRasSummary>>11)&0x1 ,(ioRasInfoPtr->ioShelfRasSummary>>10)&0x1 ,
		(ioRasInfoPtr->ioShelfRasSummary>>9)&0x1 ,(ioRasInfoPtr->ioShelfRasSummary>>8)&0x1 ,
		(ioRasInfoPtr->ioShelfRasSummary>>7)&0x1 ,(ioRasInfoPtr->ioShelfRasSummary>>6)&0x1 ,
		(ioRasInfoPtr->ioShelfRasSummary>>5)&0x1 ,(ioRasInfoPtr->ioShelfRasSummary>>4)&0x1 ,
		(ioRasInfoPtr->ioShelfRasSummary>>3)&0x1 ,(ioRasInfoPtr->ioShelfRasSummary>>2)&0x1 ,
		(ioRasInfoPtr->ioShelfRasSummary>>1)&0x1 ,(ioRasInfoPtr->ioShelfRasSummary)&0x1 );

	printf( " %3d. [(1)SCAN CMD/(2)SCAN STATUS] 0x%x 0x%x\n",
		lineCnt++,(ioRasInfoPtr->scanCmdStatus >>16)&0xffff,
		ioRasInfoPtr->scanCmdStatus&0xfff );
	printf( " %3d. [(1)DCU2/(2)DST/(3)SO/(4)COM/(5)CFG/(6)HW/(7)MEM]", lineCnt++);
	printf( "  0x%1x 0x%1x   %d %d %d %d %d\n",
		 (ioRasInfoPtr->dcuStatus >>28)&0xf,(ioRasInfoPtr->dcuStatus >>24)&0xf,
		 (ioRasInfoPtr->dcuStatus>>20)&0x1,
		 (ioRasInfoPtr->dcuStatus>>19)&0x1, (ioRasInfoPtr->dcuStatus>>18)&0x1 ,
		 (ioRasInfoPtr->dcuStatus >>17)&0x1 ,(ioRasInfoPtr->dcuStatus>>16)&0x1);

	printf( "      [(8)DCU1/(9)DST/(10)SO/(11)COM/(12)CFG/(13)HW/(14)MEM]");
	printf( "  0x%1x 0x%1x   %d %d %d %d %d\n",
			 (ioRasInfoPtr->dcuStatus>>12)&0xf,(ioRasInfoPtr->dcuStatus>>8)&0xf,
			  (ioRasInfoPtr->dcuStatus>>4)&0x1,
			 (ioRasInfoPtr->dcuStatus>>3)&0x1,(ioRasInfoPtr->dcuStatus>>2)&0x1 ,
			 (ioRasInfoPtr->dcuStatus>>1)&0x1 ,(ioRasInfoPtr->dcuStatus)&0x1  );

	printf( "\n");
	printf( "      [(1)HART/(2)FAILSAFE/(3)DISCON/(4)INIT/(5)PNT/(6)HW/(7)NOSET/(8)CFG] \n");
	printf( "      [(9)POINT(1:8P,2:16P,3:32P,4:4P)]\n");
	printf( "      [(10)TYPE(1:DI,2:DO,3:AI,4:AO,5:PI,6:PO,7:TC,8:RT,9:RR,a:RW,b:IR,c:IW,d:HT)]\n");

	for(j = 0; j < 6; j++) {
		printf( " %3d. CARD#%2d: (1)%d%d%d%d (5)%d%d%d%d (9)%d %x ",  lineCnt++,j*2+2, 
			 (ioRasInfoPtr->ioBoardStatus[j]>>15)&0x1,(ioRasInfoPtr->ioBoardStatus[j]>>14)&0x1 ,
			 (ioRasInfoPtr->ioBoardStatus[j]>>13)&0x1,(ioRasInfoPtr->ioBoardStatus[j]>>12)&0x1 ,
			 (ioRasInfoPtr->ioBoardStatus[j]>>11)&0x1 ,(ioRasInfoPtr->ioBoardStatus[j]>>10)&0x1 ,
			 (ioRasInfoPtr->ioBoardStatus[j]>>9)&0x1,(ioRasInfoPtr->ioBoardStatus[j]>>8)&0x1,
			 (ioRasInfoPtr->ioBoardStatus[j]>>4)&0xf ,(ioRasInfoPtr->ioBoardStatus[j])&0xf  );
		printf( "| CARD#%2d:  (11)%d%d%d%d (15)%d%d%d%d (19)%d %x\n", j*2+1,
			 (ioRasInfoPtr->ioBoardStatus[j]>>31)&0x1,(ioRasInfoPtr->ioBoardStatus[j]>>30)&0x1,
			 (ioRasInfoPtr->ioBoardStatus[j]>>29)&0x1,(ioRasInfoPtr->ioBoardStatus[j]>>28)&0x1,
			 (ioRasInfoPtr->ioBoardStatus[j]>>27)&0x1 ,(ioRasInfoPtr->ioBoardStatus[j]>>26)&0x1 ,
			 (ioRasInfoPtr->ioBoardStatus[j]>>25)&0x1,(ioRasInfoPtr->ioBoardStatus[j]>>24)&0x1,
			 (ioRasInfoPtr->ioBoardStatus[j]>>20)&0xf ,(ioRasInfoPtr->ioBoardStatus[j]>>16)&0xf  );

	}
#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
	printf( " %3d. [DUAL RUN(6~1)] (1)%d%d%d (4)%d%d%d \n",
		lineCnt++,

		((ioRasInfoPtr->ioDualRunInfo >>26)&0x3),((ioRasInfoPtr->ioDualRunInfo >>24)&0x3),
		((ioRasInfoPtr->ioDualRunInfo >>22)&0x3),((ioRasInfoPtr->ioDualRunInfo >>20)&0x3),
		((ioRasInfoPtr->ioDualRunInfo >>18)&0x3),((ioRasInfoPtr->ioDualRunInfo >>16)&0x3));

	printf( "      [DUAL ERR(6~1)| DUAL FLAG(6~1)] (7)%d%d%d (10)%d%d%d | (13)%d%d%d (16)%d%d%d\n",
		((ioRasInfoPtr->ioDualRunInfo >>13)&0x1),((ioRasInfoPtr->ioDualRunInfo >>12)&0x1),
		((ioRasInfoPtr->ioDualRunInfo >>11)&0x1),((ioRasInfoPtr->ioDualRunInfo >>10)&0x1),
		((ioRasInfoPtr->ioDualRunInfo >>9)&0x1),((ioRasInfoPtr->ioDualRunInfo >>8)&0x1),

		((ioRasInfoPtr->ioDualRunInfo >>5)&0x1),((ioRasInfoPtr->ioDualRunInfo >>4)&0x1),
		((ioRasInfoPtr->ioDualRunInfo >>3)&0x1),((ioRasInfoPtr->ioDualRunInfo >>2)&0x1),
		((ioRasInfoPtr->ioDualRunInfo >>1)&0x1),((ioRasInfoPtr->ioDualRunInfo >>0)&0x1));
	
	for(j = 0; j < 3; j++) {
		printf( " %3d. [ #%02d (1)PT/(2)ET | #%02d (3)PT/(4)ET | #%02d (5)PT/(6)ET | #%02d (7)PT/(8)ET]\n",
			lineCnt++, j*4+4, j*4+3,j*4+2,j*4+1);
		printf( "      0x%x 0x%x 0x%x 0x%x   0x%x 0x%x 0x%x 0x%x\n",
			(ioRasInfoPtr->ioCardDiag[j] >>27)&0x1f, (ioRasInfoPtr->ioCardDiag[j] >>24)&0x7,
			(ioRasInfoPtr->ioCardDiag[j] >>19)&0x1f,(ioRasInfoPtr->ioCardDiag[j] >>16)&0x7 ,
			(ioRasInfoPtr->ioCardDiag[j] >>11)&0x1f, (ioRasInfoPtr->ioCardDiag[j] >>8)&0x7,
			(ioRasInfoPtr->ioCardDiag[j] >>3)&0x1f,(ioRasInfoPtr->ioCardDiag[j])&0x7 );
	}
#endif 
	printf( "\n");

}


static void rasDetailProfiShelfInfoPrint(strNewRasSummaryInfo *rasInfoDataPtr, uint32 i)
{
#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
	strFieldbusRasSummaryInfo *fieldbusRasInfoPtr;	
	strIoShelfProfibusRasSummaryInfo *profiRasInfoPtr;	
	uint32 j,lineCnt;

	lineCnt = (sizeof(strNewRasSummaryInfo) - sizeof(strFieldbusRasSummaryInfo))/DOUBLE_SIZE;

	fieldbusRasInfoPtr = (strFieldbusRasSummaryInfo *)&rasInfoDataPtr->fieldbusRasInfo;
	
	profiRasInfoPtr = (strIoShelfProfibusRasSummaryInfo *) &fieldbusRasInfoPtr->u.ioShelfProfibusRasInfo[i];

	lineCnt += (sizeof(strIoShelfProfibusRasSummaryInfo)/DOUBLE_SIZE)*i;
	printf( " [PROFIBUS SHELF NUMBER] #%d\n",i+1);
	
	printf( " %3d. [(1)ShID/(2)Task/(3)Rev/(4)PcuSEr/(5)PcuMEr]",lineCnt++);
	printf( "%2d %d  %d%d%d\n",
		(profiRasInfoPtr->profiShelfRasSummary>>28)&0xf,(profiRasInfoPtr->profiShelfRasSummary>>24)&0x7 , 	
		(profiRasInfoPtr->profiShelfRasSummary>>14)&0x1,												 	
		(profiRasInfoPtr->profiShelfRasSummary>>13)&0x1,(profiRasInfoPtr->profiShelfRasSummary>>12)&0x1);	

	printf( "      [CardErr(12~1)] (6)%d%d%d%d (10)%d%d%d%d (14)%d%d%d%d \n",
		(profiRasInfoPtr->profiShelfRasSummary>>11)&0x1 ,(profiRasInfoPtr->profiShelfRasSummary>>10)&0x1 ,	
		(profiRasInfoPtr->profiShelfRasSummary>>9)&0x1 ,(profiRasInfoPtr->profiShelfRasSummary>>8)&0x1 ,	
		(profiRasInfoPtr->profiShelfRasSummary>>7)&0x1 ,(profiRasInfoPtr->profiShelfRasSummary>>6)&0x1 ,	
		(profiRasInfoPtr->profiShelfRasSummary>>5)&0x1 ,(profiRasInfoPtr->profiShelfRasSummary>>4)&0x1 ,	
		(profiRasInfoPtr->profiShelfRasSummary>>3)&0x1 ,(profiRasInfoPtr->profiShelfRasSummary>>2)&0x1 ,	
		(profiRasInfoPtr->profiShelfRasSummary>>1)&0x1 ,(profiRasInfoPtr->profiShelfRasSummary)&0x1 );		

	printf( " %3d. [(1)ScanCmd/(2)ScanStat] 0x%03x 0x%03x\n",
		lineCnt++,(profiRasInfoPtr->scanCmdStatus >>16)&0xffff,		
		profiRasInfoPtr->scanCmdStatus&0xfff );						
	printf( " %3d. [(1)Pcu2/(2)Run/(3)Ov/(4)Cm/(5)Cfg/(6)Hw/(7)Mem]", lineCnt++);
	printf( "  0x%1x 0x%1x %d %d %d %d %d\n",
		 (profiRasInfoPtr->pcuStatus >>28)&0xf,(profiRasInfoPtr->pcuStatus >>24)&0xf,	
		 (profiRasInfoPtr->pcuStatus>>20)&0x1,											
		 (profiRasInfoPtr->pcuStatus>>19)&0x1, (profiRasInfoPtr->pcuStatus>>18)&0x1 ,	
		 (profiRasInfoPtr->pcuStatus >>17)&0x1 ,(profiRasInfoPtr->pcuStatus>>16)&0x1);	

	printf( "      [(8)Pcu1/(9)Run/(10)Ov/(11)Cm/(12)Cfg/(13)Hw/(14)Mem]");
	printf( "  0x%1x 0x%1x %d %d %d %d %d\n",
			 (profiRasInfoPtr->pcuStatus>>12)&0xf,(profiRasInfoPtr->pcuStatus>>8)&0xf,	
			 (profiRasInfoPtr->pcuStatus>>4)&0x1,										
			 (profiRasInfoPtr->pcuStatus>>3)&0x1,(profiRasInfoPtr->pcuStatus>>2)&0x1 ,	
			 (profiRasInfoPtr->pcuStatus>>1)&0x1 ,(profiRasInfoPtr->pcuStatus)&0x1  );	

	printf( "\n");
	printf( "      [(1)CommErr/(2)InitErr/(3)HwErr/(4)InstErr] \n");

	for(j = 0; j < 6; j++) {
		printf( " %3d. CARD#%2d: (1)%x%x%x%x ",  lineCnt++,j*2+2,
			 (profiRasInfoPtr->fmcBoardStatus[j]>>19)&0x1,(profiRasInfoPtr->fmcBoardStatus[j]>>18)&0x1,
			 (profiRasInfoPtr->fmcBoardStatus[j]>>17)&0x1,(profiRasInfoPtr->fmcBoardStatus[j]>>16)&0x1  );
		printf( "| CARD#%2d:  (11)%x%x%x%x\n", j*2+1,
			 (profiRasInfoPtr->fmcBoardStatus[j]>>3)&0x1,(profiRasInfoPtr->fmcBoardStatus[j]>>2)&0x1,
			 (profiRasInfoPtr->fmcBoardStatus[j]>>1)&0x1,(profiRasInfoPtr->fmcBoardStatus[j])&0x1  );

	}

	printf( " %3d. [AM Mode(6~1)| Dual Run(6~1)] (1)%d%d%d%d%d%d | (7)%d%d%d%d%d%d \n",
		lineCnt++,
		((profiRasInfoPtr->fmcDualRunInfo >>29)&0x1),((profiRasInfoPtr->fmcDualRunInfo >>28)&0x1),
		((profiRasInfoPtr->fmcDualRunInfo >>27)&0x1),((profiRasInfoPtr->fmcDualRunInfo >>26)&0x1),
		((profiRasInfoPtr->fmcDualRunInfo >>25)&0x1),((profiRasInfoPtr->fmcDualRunInfo >>24)&0x1),
		
		((profiRasInfoPtr->fmcDualRunInfo >>21)&0x1),((profiRasInfoPtr->fmcDualRunInfo >>20)&0x1),
		((profiRasInfoPtr->fmcDualRunInfo >>19)&0x1),((profiRasInfoPtr->fmcDualRunInfo >>18)&0x1),
		((profiRasInfoPtr->fmcDualRunInfo >>17)&0x1),((profiRasInfoPtr->fmcDualRunInfo >>16)&0x1));

	printf( "      [DUAL ERR(6~1)| DUAL FLAG(6~1)] (13)%d%d%d%d%d%d | (19)%d%d%d%d%d%d\n",
		((profiRasInfoPtr->fmcDualRunInfo >>13)&0x1),((profiRasInfoPtr->fmcDualRunInfo >>12)&0x1),
		((profiRasInfoPtr->fmcDualRunInfo >>11)&0x1),((profiRasInfoPtr->fmcDualRunInfo >>10)&0x1),
		((profiRasInfoPtr->fmcDualRunInfo >>9)&0x1),((profiRasInfoPtr->fmcDualRunInfo >>8)&0x1),

		((profiRasInfoPtr->fmcDualRunInfo >>5)&0x1),((profiRasInfoPtr->fmcDualRunInfo >>4)&0x1),
		((profiRasInfoPtr->fmcDualRunInfo >>3)&0x1),((profiRasInfoPtr->fmcDualRunInfo >>2)&0x1),
		((profiRasInfoPtr->fmcDualRunInfo >>1)&0x1),((profiRasInfoPtr->fmcDualRunInfo >>0)&0x1));
	
	for(j = 0; j < 12; j++) {
		printf( " %3d. [ FMC#%02d (1)CfgNum/(2)OnNum/(3)ErrNum/(4)ComErrNum]\n",
			lineCnt++, (j+1));
		printf( "                  %3d       %3d      %3d       %3d\n",
			(profiRasInfoPtr->slvDiagCount[j] >>24)&0xFF, (profiRasInfoPtr->slvDiagCount[j] >>16)&0xFF,
			(profiRasInfoPtr->slvDiagCount[j] >>8)&0xFF,(profiRasInfoPtr->slvDiagCount[j] >>0)&0xFF);
	}

	printf( "\n");

#endif 
}


uint32 rasDetailInfoPrint(uint8* dataPtr, uint32 dataLength)
{
	uint32 status = NO_ERROR;
	uint32 *opCmd; 
	strSysConfigInfo sysCnfgInfo;
	strNewRasSummaryInfo *rasSummaryInfoPtr;
	strRasHeadInfo *rasheadInfoPtr;
	
	opCmd = (uint32*)(dataPtr + MAX_NEW_FRAME_HEAD_LEN);

	if( (  ((*opCmd>>16)&0xffff)  == SYS_STATUS_CMD )
		&& ((*opCmd&0xffff) == SYS_DIAG_STATUS ))
	{	
		printf( "\n =========================== RAS Detail DATA[%d] ===========================\n", dataLength);
	
	
#if (MAXIOSHELF_CONFIG == MAX16IOSHELF)
		if(dataLength != (sizeof(strNewRasSummaryInfo)- (sizeof(strIoShelfProfibusRasSummaryInfo)*2) + MAX_NEW_FRAME_HEAD_LEN)){
#elif ((MAXIOSHELF_CONFIG == MAX8IOSHELF) || (MAXIOSHELF_CONFIG == MAX12IOSHELF))
		if(dataLength != (sizeof(strNewRasSummaryInfo)+ MAX_NEW_FRAME_HEAD_LEN)){
#endif
			printf( "RAS DATA LENTH(%d) IS INVALID!!!\n", dataLength);
			return(status);
		}

		
		(void)systemCnfgInfoRead( &sysCnfgInfo);

		rasSummaryInfoPtr = (strNewRasSummaryInfo *)(dataPtr + MAX_NEW_FRAME_HEAD_LEN);

		
		rasheadInfoPtr = (strRasHeadInfo *)&rasSummaryInfoPtr->rasheadInfo;
		rasDetailKindInfoPrint(rasSummaryInfoPtr, RAS_CNF_CHANGE_SET);
		rasDetailKindInfoPrint(rasSummaryInfoPtr, RAS_MPU_CHANGE_SET);
		rasDetailKindInfoPrint(rasSummaryInfoPtr, RAS_FBC_CHANGE_SET);
		rasDetailKindInfoPrint(rasSummaryInfoPtr, RAS_DIU_CHANGE_SET);
#if ((MAXIOSHELF_CONFIG == MAX8IOSHELF) || (MAXIOSHELF_CONFIG == MAX12IOSHELF))
		rasDetailKindInfoPrint(rasSummaryInfoPtr, RAS_TMTC_CHANGE_SET_PRINT);
		rasDetailKindInfoPrint(rasSummaryInfoPtr, RAS_FMC_CHANGE_SET_PRINT);
#endif

		printf( "\n");
	}
	return(status);
}

void rasDetailInfoGet(strNewRasSummaryInfo *rasInfoDataPtr)
{
	strTransDetailInfo detailData;
	uint32 *opCmdPtr;

	if(rasInfoDataPtr!=NULL){
		memoryClear((uint8*)rasInfoDataPtr,sizeof(strNewRasSummaryInfo) );
	}

	 
	gTransBroadcastDetailSendFlag = DEBUG_ENABLE;
    while(!keyHit()) {
        if(transInfoGet(&detailData) == NO_ERROR) 
		{
			opCmdPtr = (uint32*)( (uint32)&detailData.dataBuff[0] + MAX_NEW_FRAME_HEAD_LEN );

			if( (  ((*opCmdPtr>>16)&0xffff)  == SYS_STATUS_CMD )
				&& ((*opCmdPtr&0xffff) == SYS_DIAG_STATUS )
			){
				memoryCopy((uint8*)rasInfoDataPtr,(uint8*)opCmdPtr,sizeof(strNewRasSummaryInfo) );
				break;
			}
		}
        sysTaskDelay(systemClockRateGet()/2);
    }
	gTransBroadcastDetailSendFlag = DEBUG_DISABLE;
  	
    (void)transInfoQueClear();
}
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)

void dualDebugVarClear(void)
{
	fdprintf(gDebugPortFd, "\b******** Dual Debugger Variable Clear !!!\n");

    
    memoryClear ( (uint8 *)gDualRecvData,
    			  sizeof(strDebugDataInfo) * DEBUG_INFO_MAX_ARRAY_NUM
				);

    memoryClear ( (uint8 *)gDualSendData,
    			  sizeof(strDebugDataInfo) * DEBUG_INFO_MAX_ARRAY_NUM
				);
}


void debugDualInfoSet(strMsgQueData *dataPtr, strDebugDataInfo *debugPtr)
{
    uint32  sockLen = (uint32)SOCK_ADDR_LENGTH;
    strDebugDataInfo    *curPtr = debugPtr;
    int32   curIndex, i, *cmdCode;
    strDualProtocolInfo    *dualMsgPtr;

    
    curIndex = debugDualInfoCheck(dataPtr, debugPtr);

    
    for(i = 0; i < curIndex; i++) curPtr++;

    
    memoryCopy( (uint8 *)&curPtr->peerAddr,
                (uint8 *)&dataPtr->peerAddr,
                sockLen
			  );

    dualMsgPtr = (strDualProtocolInfo *)dataPtr->protoData.msgData;

    
    curPtr->seqNum      = dualMsgPtr->sequenceID;
    curPtr->msgDataSize = dataPtr->protoData.numOfBytes;

	cmdCode = (uint32*)(&dualMsgPtr->dataBuff[0]);
 	curPtr->opCode = (*cmdCode)>>16;
	curPtr->optionCode = (*cmdCode)&0xffff;
    curPtr->transTotalCount++;
}


static uint32 debugDualInfoCheck(strMsgQueData *dataPtr, strDebugDataInfo *debugPtr)
{
    int32 index = 0;
    uint32 ipHexVal = dataPtr->peerAddr.iAddr;
    uint16 portNum = dataPtr->peerAddr.port;
    uint16 tmpPortNum;
    uint32  tmpHexVal;
    strDebugDataInfo    *tmpPtr = debugPtr;
	strDualProtocolInfo *dulaMsgPtr;
	uint32 opCode, optionCode, *cmdCode;

    dulaMsgPtr = (strDualProtocolInfo *)dataPtr->protoData.msgData;
	
	cmdCode = (uint32*)(&dulaMsgPtr->dataBuff[0]);
 	opCode = (*cmdCode)>>16;
	optionCode = (*cmdCode)&0xffff;

    
    for (index = 0; index < DEBUG_INFO_MAX_ARRAY_NUM; index++, tmpPtr++) {
        tmpHexVal = tmpPtr->peerAddr.iAddr;
        tmpPortNum = tmpPtr->peerAddr.port;

        if( (ipHexVal == tmpHexVal)
                    &&
            (portNum == tmpPortNum)
            		&&
			(opCode == tmpPtr->opCode)
            		&&
			(optionCode == tmpPtr->optionCode)
          )
            break;
    }
    if(index == DEBUG_INFO_MAX_ARRAY_NUM) {
        tmpPtr = debugPtr;

        
        
        for(index = 0; index < DEBUG_INFO_MAX_ARRAY_NUM; index++, tmpPtr++){
            if(tmpPtr->peerAddr.iAddr == 0x0)
                break;
        }
        
        if(index == DEBUG_INFO_MAX_ARRAY_NUM)
            index = DEBUG_INFO_MAX_ARRAY_NUM - 1;
    }
    return(index);
}


uint32  transDualInfoSet( strMsgQueData *dataPtr)
{
    strTransDetailInfo *infoPtr = (strTransDetailInfo *)NULL;
    uint32 status = NO_ERROR;
    strDualProtocolInfo *netMsgPtr  = (strDualProtocolInfo *)dataPtr->protoData.msgData;
    
	
	
	if(gDualUnicastDetailRecvFlag ||gDualUnicastDetailSendFlag) {
		if((netMsgPtr->messageType != RMTREQST) && (netMsgPtr->messageType != RMTRESPS))
			return(status);
	}
	else if(gDualBroadcastDetailRecvFlag ||gDualBroadcastDetailSendFlag) {
		if(netMsgPtr->messageType != RMTCAST)
			return(status);	
	}
	

	else {
		fdprintf(gDebugPortFd, "[transInfoSet] - Unknown Msg Type !!\n");
		return(status);
	}	

	status = semaphoreGet(sTransMsgDbgSemaId, FOREVERWAIT);

    if(status == NO_ERROR){
        
        infoPtr = (strTransDetailInfo *)nodeGetAndUnlink (
                                    (strDoubleLink **)&sTransQueMngrInfo.freeList
                                                         );
        if(infoPtr != (strTransDetailInfo *)NULL) {
            
            memoryCopy( (uint8 *)&infoPtr->peerAddr,
                        (uint8 *)&dataPtr->peerAddr,
                        (uint32)SOCK_ADDR_LENGTH
                       );

            
			infoPtr->totalBytesLen = dataPtr->protoData.numOfBytes;
			memoryCopy ( (uint8 *)infoPtr->dataBuff,
                         (uint8 *)&dataPtr->protoData.msgData,
                         (uint32)dataPtr->protoData.numOfBytes
                       );

            
            nodePutOnLast ( (strDoubleLink **) &sTransQueMngrInfo.usedList,
                            (strDoubleLink *) infoPtr
                          );
        }

#ifdef VXWORKS
        semaphoreRelease(sTransMsgDbgSemaId);
#else
        semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sTransMsgDbgSemaId);
#endif
    }
    else {
		fdprintf(gDebugPortFd,"[transInfoSet] - Sema Get Error\n");
	}
    return(status);
}

void dualTransDataInfoDisplay(strDebugDataInfo *dataPtr, uint8 *transMsg)
{
    int8    line[16];
    int8    ipArray[20];
    int8  *cString[] =
    {
        (int8 *)"IP ADDR",
		(int8 *)"PORT NUM",
		(int8 *)"SOCK FD",
		(int8 *)"SEQ NUM",
		(int8 *)"DATA SIZE",
		(int8 *)"TRANS COUNT",
		(int8 *)NULL
    };
    int8    *pString[] = {
        (int8 *)"INDEX",
		(int8 *)"IP ADDR",
		(int8 *)"PORT",
		(int8 *)"MSG KIND",
		(int8 *)"GRP CMD",
		(int8 *)"ACTION CMD",
		(int8 *)NULL
    };
    int8 *msgKindStr[] = { 
		(int8 *)"UNICAST",
		(int8 *)"LIVECOUNT",
		(int8 *)"NULL"};


	int8 *dulaOptionStr[] = { 
		(int8 *)NULL,
		(int8 *)"DUAL_MEMORY_COPY",
		(int8 *)"DUAL_CONF_READ",
		(int8 *)"DUAL_CONF_WRITE",
		(int8 *)"DUAL_CMD_WRITE",
		(int8 *)"DUAL_LIVE_COUNT",
		(int8 *)"DUAL_RUNDATA_COPY",
		(int8 *)"DUAL_RUNTYPE_COPY",
		(int8 *)"DUAL_RUNBUS_COPY",
		(int8 *)NULL
	};

    int8    **pMsg;
    int32   showIndex, i, index;
    strDebugDataInfo    *curPtr = dataPtr;
    uint32  ipHexVal;
	strCommandListInfo	cmdList;
	
    
    gDualRecvInfoFlag = 1;
    gDualSendInfoFlag = 1;

    memoryClear ( (uint8 *)ipArray, 20);

    
    sysTaskDelay(systemClockRateGet());

    fdprintf(gDebugPortFd, "\n\t ******* Debug Table Info ********\n");
    pMsg = pString;
	fdprintf(gDebugPortFd, "%2s", *pMsg);
    pMsg++;

    for(; *pMsg != NULL; pMsg++) {
        fdprintf(gDebugPortFd, "%14s",*pMsg);
	}

	fdprintf(gDebugPortFd, "\n");

    fdprintf(gDebugPortFd, "%2s%14s%14s%14s%14s%14s\n",
				"-----","-------","----","---------","---------","----------");

    
    for(i = 0; i < DEBUG_INFO_MAX_ARRAY_NUM; i++,curPtr++){
        ipHexVal = curPtr->peerAddr.iAddr;

		switch(curPtr->optionCode) {
		   	case DUAL_MEMORY_COPY:
		   	case DUAL_CONF_READ:
		   	case DUAL_CONF_WRITE:
		   	case DUAL_CMD_WRITE:
		   	case DUAL_RUNDATA_COPY:
		   	case DUAL_RUNTYPE_COPY:
		   	case DUAL_RUNBUS_COPY:
            	index = 0;
			break;
		  	case DUAL_LIVE_COUNT:
				index = 1;
			break;
			default:
				index = 2;
			break;	  
		}
        if(ipHexVal) {
			memoryClear ( 	(uint8 *) &cmdList, 
							sizeof(strCommandListInfo)
						);			

			memoryCopy((uint8*)cmdList.opCode, (uint8*) "DUAL_ACCESS",12);
			memoryCopy((uint8*)cmdList.optionCode, (uint8*)dulaOptionStr[curPtr->optionCode],
				strlen(dulaOptionStr[curPtr->optionCode]) );
        
            ipAddrHexToString(ipArray, ipHexVal);
            
            fdprintf(gDebugPortFd, "%2d%20s%10d%13s%15s%18s\n", 
					i, ipArray,htons(curPtr->peerAddr.port), msgKindStr[index],
					cmdList.opCode, cmdList.optionCode);
        }
    }
    while(1L) {
        fdprintf(gDebugPortFd, "\nEnter the INDEX for Display ? ");
        ioRead(gDebugPortFd, line, sizeof(line));
        showIndex = atoi((const char *)line);
        if( (showIndex < 0) || (showIndex >= DEBUG_INFO_MAX_ARRAY_NUM)) {
            fdprintf(gDebugPortFd, "\t ****** INVALID INDEX[0x%x]\n", showIndex);
		}
        else {
            break;
		}
    }

    
    curPtr = dataPtr;
    for(i = 0; i < showIndex; i++) curPtr++;

    fdprintf(gDebugPortFd, "\n\t**********%s Data Info Display**********\n", transMsg);

    for(pMsg = cString; *pMsg != NULL; pMsg++) {
        fdprintf(gDebugPortFd, "%12s", *pMsg);
	}
    fdprintf(gDebugPortFd, "\n");

    for(pMsg = cString; *pMsg != NULL; pMsg++) {
        fdprintf(gDebugPortFd, "%12s", "----------");
	}
    fdprintf(gDebugPortFd, "\n");

    ipAddrHexToString(ipArray, curPtr->peerAddr.iAddr);

    while(!keyHit()) {
        fdprintf(gDebugPortFd, "%15s%9d%12d%12x%12d%12d\r",  ipArray,
                                        htons(curPtr->peerAddr.port),
                                        curPtr->peerAddr.sockFd,
                                        curPtr->seqNum,
                                        curPtr->msgDataSize,
                                        curPtr->transTotalCount);
		sysTaskDelay(systemClockRateGet()/20);
    }
    fdprintf(gDebugPortFd, "\n");

    
#ifdef	VXWORKS
    (void)ioSysCtrlFunc (gDebugPortFd, MYIOFLUSH, 0);
#endif
    
    gDualRecvInfoFlag = 0;
    gDualSendInfoFlag = 0;

    fdprintf(gDebugPortFd,"\n");
}


void dualDataDetailInfoDisplay(
				uint8 transInfoFlag,  
                                      
                uint8 dataDispFlag 	
                					
                                    
                                 )
{
    strTransDetailInfo  detailData;
    uint8   *pString;

    if(transInfoFlag == DETAIL_DATA_FOR_RECV) {
    	if( (dataDispFlag == UNICAST_HEAD_PRINT) ||
    		(dataDispFlag == UNICAST_DETAIL_DATA_PRINT)
    	)
        	gDualUnicastDetailRecvFlag = 1;
		else 
			gDualBroadcastDetailRecvFlag = 1;
	        	
        pString = (uint8 *)"RECV DATA DETAIL INFO";
    }
    else {
    	if( (dataDispFlag == UNICAST_HEAD_PRINT) ||
    		(dataDispFlag == UNICAST_DETAIL_DATA_PRINT)
    	)
        	gDualUnicastDetailSendFlag = 1;
		else
			gDualBroadcastDetailSendFlag = 1;
   	
        pString = (uint8 *)"SEND DATA DETAIL INFO";
    }

    while(!keyHit()) {
        if(transInfoGet(&detailData) == NO_ERROR)
            dualDetailInfoPrint(	&detailData, 
									pString, 
									dataDispFlag, 
									transInfoFlag
								);

        
        
		sysTaskDelay(systemClockRateGet()/20);
    }
    
    if(transInfoFlag == DETAIL_DATA_FOR_RECV){
     	gDualBroadcastDetailRecvFlag 	= 0;
     	gDualUnicastDetailRecvFlag		= 0;
    }
    else { 
    	gDualBroadcastDetailSendFlag 	= 0;
    	gDualUnicastDetailSendFlag		= 0;
    }
    
    sysTaskDelay(systemClockRateGet()/20);

    
    (void)transInfoQueClear();
}


static void dualDetailInfoPrint(   strTransDetailInfo *dataPtr,
                                    uint8 *string,
                                    uint8   dataDispFlag,
                                    uint8   transInfoFlag
                                )
{
    int8    *msgType[] =
    {
        (int8 *)NULL,
        (int8 *)"NULL",
		(int8 *)"NULL",
		(int8 *)"NULL",
		(int8 *)"RMTREQST",
		(int8 *)"RMTRESPS",
		(int8 *)"RMTCAST",
        (int8 *)NULL
    };
   int8    *strDualOptionCode[] =
    {
        (int8 *)NULL,
        (int8 *)"DUAL_MEMORY_COPY",
		(int8 *)"DUAL_CONF_READ",
		(int8 *)"DUAL_CONF_WRITE",
		(int8 *)"DUAL_CMD_WRITE",
		(int8 *)"DUAL_LIVE_COUNT",
		(int8 *)"DUAL_RUNDATA_COPY",
		(int8 *)"DUAL_RUNTYPE_COPY",
		(int8 *)"DUAL_RUNBUS_COPY",
        (int8 *)NULL
    };
   int8    *strDualArea[] =
    {
        (int8 *)"DUAL_DEFAULT_AREA",
        (int8 *)"MPU_FB_AREA",
		(int8 *)"VAR_DATA_AREA",
		(int8 *)"SFC_INFO_AREA",
		(int8 *)"SYS_EXE_AREA",
		(int8 *)"DIU_FB_AREA",
		(int8 *)"DUAL_INFO_AREA",
		(int8 *)"DUAL_RUNTYPE_COPY",
		(int8 *)"FERR_DATA_AREA",
        (int8 *)NULL
    };
   
    uint8   *dispDataPtr;
    uint8   ipArray[20];
    strCommandListInfo  cmdList;

	strDualProtocolInfo *netMsgPtr;
	uint32 *opCmd, opCode, optionCode;

	netMsgPtr = (strDualProtocolInfo *)dataPtr->dataBuff;

    if ( (dataDispFlag == UNICAST_HEAD_PRINT) ||
		 (dataDispFlag == UNICAST_DETAIL_DATA_PRINT)
	){ 
        if( (netMsgPtr->messageType <= BROADCAST) || (netMsgPtr->messageType == RMTCAST) )
			goto MODULE_END;
    }
    else 
    if ( (dataDispFlag == BROADCAST_HEAD_PRINT) ||
		 (dataDispFlag == BROADCAST_DETAIL_DATA_PRINT)
	){ 
        if(netMsgPtr->messageType != RMTCAST)
			goto MODULE_END;
    }
    memoryClear((uint8 *)ipArray, 20);
    memoryClear((uint8 *) &cmdList, sizeof(strCommandListInfo));

    fdprintf(gDebugPortFd, "\n\t***********************[%s]********************\n", string);

    ipAddrHexToString((int8 *)ipArray, dataPtr->peerAddr.iAddr);

    if(transInfoFlag == DETAIL_DATA_FOR_RECV) {
        fdprintf(gDebugPortFd, "\t\t Source DLU Address : %s\n",ipArray);
	}
    else {
        fdprintf(gDebugPortFd, "\t\t   Dest DLU Address : %s\n",ipArray);
	}
	fdprintf(gDebugPortFd, "\n");

    fdprintf(gDebugPortFd, "\t\t         Total Byte Len : %d\n", dataPtr->totalBytesLen);
    fdprintf(gDebugPortFd, "\t\t        Sequence Number : 0x%x\n", netMsgPtr->sequenceID);

    if( (netMsgPtr->messageType < RMTREQST) || (netMsgPtr->messageType > RMTCAST)) {
        fdprintf(gDebugPortFd, "\t\t           Message Type : UNKNOWN[0x%x]\n", netMsgPtr->messageType);
	}
    else {
        fdprintf(gDebugPortFd, "\t\t           Message Type : %s\n", msgType[netMsgPtr->messageType]);
	}
    fdprintf(gDebugPortFd, "\t\t         Dual Data Area : %s\n", strDualArea[netMsgPtr->dualDataArea]);
	
	opCmd = (uint32*)&netMsgPtr->dataBuff[0];
	opCode = *opCmd >> 16;
	optionCode = *opCmd &0xffff;

    fdprintf(gDebugPortFd, "\t\t            Option Code : %s\n", strDualOptionCode[optionCode]);

    
    if( (dataDispFlag == UNICAST_DETAIL_DATA_PRINT) ||
    	(dataDispFlag == BROADCAST_DETAIL_DATA_PRINT)
	){
        
        dispDataPtr = (uint8 *)netMsgPtr;

        dataDisplay(    (uint8 *)"PROTOCOL DATA",
                        dispDataPtr,
                        (uint32)dataPtr->totalBytesLen
                    );
    }
MODULE_END:
	return;
}

#ifdef _WIN32

uint32 filelistDisplay(uint32 fileKind) {
    WIN32_FIND_DATA find_file;
    HANDLE	hFile;
	SYSTEMTIME fileWriteTimeInfo;
	uint32  fileNumber = 0; 

	TCHAR	*strDirectoryName=TEXT(ROOT_DIR_NAME);
	TCHAR	delFileName[MAX_FILE_DIR_LENGTH] = TEXT(ROOT_DIR_NAME);
	char    fileName[MAX_PATH];
	int     fileLenth = 0;

	
	if(fileKind == FILE_KIND_ENGSRC)
		hFile = FindFirstFile(TEXT(ZIP_FILE_NAME), &find_file );
	else if(fileKind == FILE_KIND_ROMIMG)
		hFile = FindFirstFile(TEXT(ROM_FILE_NAME), &find_file );
	else if(fileKind == FILE_KIND_PBENG_FILE)
		hFile = FindFirstFile(TEXT(ALL_PBENG_FILE_NAME), &find_file );
	else 
		hFile = INVALID_HANDLE_VALUE;

	if (hFile == INVALID_HANDLE_VALUE)
		printf( " It has no files in current directory!!!\n" );
	else
	{
		printf( "\n>> Listing of Files In Current Directory:\n" );
		printf( "\n ARC  FILE                     SIZE(Byte) %9c DATE\n", ' ' );
		printf(   " ---  ---------------          ---------- %9c --------------------\n", ' ' );

		printf( ( find_file.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE )   ? "  Y  " : "  N  " );
		FileTimeToSystemTime(&find_file.ftLastWriteTime,&fileWriteTimeInfo);
#ifdef _WINCE
		fileLenth = wcslen(&find_file.cFileName[0]);
		WideCharToMultiByte(CP_ACP, (DWORD)0,
						(LPCWSTR)&find_file.cFileName[0],(int)fileLenth,
						(LPSTR)fileName, (int)fileLenth,
						(LPCSTR)NULL,(LPBOOL)NULL);
#endif
#ifdef _WINNT
		strcpy(&fileName[0],&find_file.cFileName[0]);
#endif
		printf( " %-22s %-9ld(%3.2fKB) %c %.24s", fileName, find_file.nFileSizeLow ,
				(double)find_file.nFileSizeLow/1024 ,' ',systoasctime(&fileWriteTimeInfo) );
		fileNumber++;

		
		while( FindNextFile( hFile, &find_file ))
		{
			printf( ( find_file.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE )   ? "  Y  " : "  N  " );
			FileTimeToSystemTime(&find_file.ftLastWriteTime,&fileWriteTimeInfo);
#ifdef _WINCE
			fileLenth = wcslen(&find_file.cFileName[0]);
			WideCharToMultiByte(CP_ACP, (DWORD)0,
							(LPCWSTR)&find_file.cFileName[0],(int)fileLenth,
							(LPSTR)fileName, (int)fileLenth,
							(LPCSTR)NULL,(LPBOOL)NULL);
#endif
#ifdef _WINNT
			strcpy(&fileName[0],&find_file.cFileName[0]);
#endif
			printf( " %-22s %-9ld(%3.2fKB) %c %.24s", find_file.cFileName, find_file.nFileSizeLow ,
				(double)find_file.nFileSizeLow/1024 ,' ',systoasctime(&fileWriteTimeInfo) );
			fileNumber++;
		}

		FindClose( hFile );
	}
	printf("\n");
	return(fileNumber);
}

void flashDiskinfo(void) {
	int64	totalBytes, freeBytes, usedBytes;
	ULARGE_INTEGER uliUserFree, uliTotal, uliRealFree;
	TCHAR	*strDirectoryName=TEXT(ROOT_DIR_NAME);
	
	if(GetDiskFreeSpaceEx(strDirectoryName, &uliUserFree, &uliTotal, &uliRealFree))

	{
        totalBytes = uliTotal.LowPart+uliTotal.HighPart*(0x100000000);
        freeBytes = uliRealFree.LowPart+uliRealFree.HighPart*(0x100000000);
        usedBytes = totalBytes - freeBytes;

		printf( "\n================[Disk Size Info]===============\n");
		printf( "Total Disk Size : %10lu Bytes", totalBytes);
		printf( "(%3.2f MB)\n",((double)totalBytes/1024)/1024);
		printf( " Used Disk Size : %10luu Bytes", usedBytes);
		printf( "(%3.2f MB)\n",((double)usedBytes/1024)/1024);
		printf( " Free Disk Size : %10lu Bytes", freeBytes);
		printf( "(%3.2f MB)",((double)freeBytes/1024)/1024);
		printf( "\n===============================================\n");
	}
}

extern uint32  gMainThreadID;
uint32 romImgUpdate()
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	TCHAR*  fileExt;
	uint32 fileCnt = 0, fileExist = 0; 
	TCHAR fileName[32];

	 
	hFind = FindFirstFile(TEXT(ROM_FILE_NAME), &fd);
	if(hFind != INVALID_HANDLE_VALUE)  {
	  
	  do {
		 if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		 {
			 wsprintf(fileName,fd.cFileName);
			 fileExt = _tcsrchr(fileName,TEXT('.')) +1;
			 if(_tcslen(fileExt)<=4) {
				 
				 fileExist=FALSE;
				 while(*fileExt){
					 if(!isdigit(*fileExt++)) {
						 fileExist=FALSE;
						 break;
					 }
					 else fileExist=TRUE;
				 }
				 if(fileExist==TRUE) {
					 fileCnt++;
					 if(fileCnt>2) break;
				 }
			 } 
		 }
	  }
	  while ( FindNextFile(hFind,&fd) );
	  FindClose(hFind);
	}

	if(fileCnt == 2) {
		printf("\n It will be Exit Main Thread for ROM File Update!!!");
#ifdef _WINNT
		ExitProcess(0);
#else
		TerminateThread((HANDLE)gMainThreadID,0);

#endif
	}
    else return(1);

	return(NO_ERROR);
}
#endif 

#ifdef _WIN32

void dataCompareBetweenRamAndMemoryBoard(void)
{
	uint32	moduleId, index, maxSegNum;	
    int8    line[16], *pLine;

		
	if(gSysRunningInfoPtr->sysRedundancyStatus != SYS_RAS_REDUNDANCY_RUN){
		fdprintf(gDebugPortFd,"\t**** You Should Compare Data In RUN Shelf !!!! ********\n");
		return;	
	}

RETRY:	
    fdprintf(gDebugPortFd, "\n");
    fdprintf(gDebugPortFd, "\t* Select ModuleId (0:Cancel 1:MPU) ? ");
    ioRead(gDebugPortFd, line, sizeof(line));
	pLine = line;
    fdprintf(gDebugPortFd, "\n");
	
	index = atoi(pLine);
	switch(index){
		case 0:
			fdprintf(gDebugPortFd,"\t Cancel Fb FB Data Comparing!!!\n");
			return; 
		break;	
		case 1:
			moduleId 	= LOGIC_ID;
			maxSegNum 	= MAX_MPU_SCHD_SEG_ID;
		break;
		default:
			fdprintf(gDebugPortFd,"\tInvalid ModuleId\n");
			goto RETRY;
		break;
	}

RETRY_1:
    fdprintf(gDebugPortFd, "\n");
    fdprintf(gDebugPortFd, "\t* Comparing Field \n");
    fdprintf(gDebugPortFd, "\t  - 0.Cancel\n");
    fdprintf(gDebugPortFd, "\t  - 1.Between Local Ram and Local Memory Board\n");
#ifdef VXWORKS
    fdprintf(gDebugPortFd, "\t  - 2.Between Local Ram and Remote Memory Board\n");
    fdprintf(gDebugPortFd, "\t  - 3.Between Local Memory Board and Remote Memory Board\n");
#endif
    fdprintf(gDebugPortFd, "\t* Select Item ? ");

    ioRead(gDebugPortFd, line, sizeof(line));
	pLine = line;
    fdprintf(gDebugPortFd, "\n");
	
	index = atoi(pLine);
	switch(index){
		case COMPARE_CANCEL:
			fdprintf(gDebugPortFd,"\t Cancel Fb Data Comparing !!!\n");
			return; 
		break;	
		case COMPARE_BETWEEN_LOCAL_RAM_AND_LOCAL_BOARD:
		case COMPARE_BETWEEN_LOCAL_RAM_AND_REMOTE_BOARD:
		case COMPARE_BETWEEN_LOCAL_BOARD_AND_REMOTE_BOARD:
				
			if(gSysRunningInfoPtr->sysRedundancyStatus != SYS_RAS_REDUNDANCY_RUN){
				fdprintf(gDebugPortFd,"\t**** You Should Compare Data In RUN Shelf !!!! ********\n");
				return;	
			}

			
			if(gDiuRasInfoPtr->redundancyStatus != NO_ERROR){
				if((index == COMPARE_BETWEEN_LOCAL_RAM_AND_REMOTE_BOARD) ||
				   (index == COMPARE_BETWEEN_LOCAL_BOARD_AND_REMOTE_BOARD)){
					fdprintf(gDebugPortFd,"\t**** You Should Compare Data In Dual Status !!!! ********\n");
					return;						
				}
			}
#ifdef _WIN32
			
			index = COMPARE_BETWEEN_LOCAL_RAM_AND_LOCAL_BOARD;
#endif
			dataCompareBetweenMemory(moduleId, index);
		break;
		default:
			fdprintf(gDebugPortFd,"\tInvalid Comparing Field\n");
			goto RETRY_1;
		break;
	}
}


static void dataCompareBetweenMemory(uint32 moduleId, uint32 compareFlag)
{
	uint32	status = NO_ERROR;
	uint32	taskId;
	strModuleInfo		moduleInfo;
	strFbTaskTblMemInfo	segInfo;
	
		
	if((status = moduleInfoRead(moduleId, &moduleInfo)) != NO_ERROR) {
		fdprintf(gDebugPortFd, "[ERROR(%d)]- Module Info Read Error[0x%x]\n", __LINE__,status);
		return;
	}	

	for(taskId = 1; taskId <= moduleInfo.maxSegmentNum; taskId++) {
		if((status = getFbTaskTblInfo(moduleId, taskId, &segInfo)) != NO_ERROR){
			fdprintf(gDebugPortFd, "[ERROR(%d)]- Seg[%d] Tbl Info Read Error[0x%x]\n",__LINE__,taskId, status);
			return;
		}
		if((segInfo.taskMode == PCS_RUN)|| (segInfo.taskMode == PCS_STOP)) break;		
	}
	
	if((segInfo.taskMode != PCS_RUN)&& (segInfo.taskMode != PCS_STOP)) {
		fdprintf(gDebugPortFd, "\t**** There is No RUN Segment \n");
		fdprintf(gDebugPortFd, "\t**** You Should Change the Segment to RUN\n");
		return;		
	}

		
	status = debugDataCopyFromLocalRamOrBusMemoryBoard(	moduleId, compareFlag);
	if(status) return;

		
	 
	status = compareSysExecInfo(moduleId);
	if(status != NO_ERROR) return;

	
	status = compareSegmentTableInfo(moduleId, &moduleInfo);
	if(status != NO_ERROR) return;
	
	
	status = compareFbDataInfo(moduleId, &moduleInfo);
	if(status != NO_ERROR) return;

	
	compareSfcDataInfo(moduleId);
	
	return;
}



static uint32 compareFbDataInfo(uint32 moduleId, strModuleInfo *moduleInfoPtr)
{
	uint32 status = NO_ERROR, retVal = NO_ERROR;
	uint32	taskId, loop, fbBlockAddr;
	strFbTaskTblMemInfo	localSegInfo;
	strFbMemInfo	localFbMemInfo, remoteFbMemInfo;
	uint32	localData[1024], remoteData[1024];
	uint32 *srcDataPtr, *localDataPtr, *remoteDataPtr;
	uint32	compareResult = 0;
	strModuleExecInfo sysExeInfo; 

	status = fbSysExeModuleInfoRead(LOGIC_ID, &sysExeInfo);
	if(status != NO_ERROR) {
			fdprintf(gDebugPortFd,"[ERROR] - fbSysExeModuleInfoRead() Call Err !!\n");
	}
		
	for(taskId = 1; taskId <= sysExeInfo.curMaxSegmentId; taskId++) {
		
		
		
		status = segTblInfoReadForDebugging(	moduleId, 
												taskId, 
												&localSegInfo, 
												LOCAL_DEBUGGING_MEMORY_REGION
											);
		if(status != NO_ERROR){
			fdprintf(gDebugPortFd,"[ERROR(%d)] - Seg(#%d) Table Info Read Error !!\n",__LINE__,taskId);
			 return(1);
		}
		
		if((localSegInfo.taskMode != PCS_RUN) && (localSegInfo.taskMode != PCS_STOP)) 
			continue;
		
		for(fbBlockAddr = 0; fbBlockAddr <= sysExeInfo.curfbMaxBlockAddr;
			fbBlockAddr++) {
			
			status = fbMemInfoReadForDebugging(	moduleId,
												fbBlockAddr,
												&localFbMemInfo,
												LOCAL_DEBUGGING_MEMORY_REGION
											  );
			if(status){
				fdprintf(gDebugPortFd,"[ERROR(%d)] - fbMemInfoReadForDebugging Local [%d] !!\n",__LINE__,fbBlockAddr);
			 	return(1);				
			}			
			status = fbMemInfoReadForDebugging(	moduleId,
												fbBlockAddr,
												&remoteFbMemInfo,
												REMOTE_DEBUGGING_MEMORY_REGION
											  );
			if(status){
				fdprintf(gDebugPortFd,"[ERROR(%d)] - fbMemInfoReadForDebugging Remote [%d] !!\n",__LINE__,fbBlockAddr);
			 	return(1);				
			}
			
			if( (localFbMemInfo.functionCode == 0) && (remoteFbMemInfo.functionCode == 0))
				continue;

			if( (localFbMemInfo.fbTaskId != taskId) && (remoteFbMemInfo.fbTaskId != taskId))
				continue;

			
			retVal = dataCompare(	(uint32 *)&localFbMemInfo, 
									(uint32 *)&remoteFbMemInfo,
									sizeof(strFbMemInfo)/4
								);
			if(retVal != NO_ERROR) {
				compareResult = 1;
				localDataPtr 	= (uint32 *)&localFbMemInfo;
				remoteDataPtr 	= (uint32 *)&remoteFbMemInfo;

				fdprintf(gDebugPortFd,"****************************************************************************\n");
				fdprintf(gDebugPortFd,"*       #%03d Addr FB Memory Info Data Different\n", fbBlockAddr);
				fdprintf(gDebugPortFd,"****************************************************************************\n");
				for(loop = 0; loop < (sizeof(strFbMemInfo)/4); loop++) {
					fdprintf(gDebugPortFd,"[FB INFO(%03d)]- Local Data : %08d : Remote Data : %08d\n",
								loop, *localDataPtr++, *remoteDataPtr++);					
				}
			}
			else { 
				
				
				if(moduleId == LOGIC_ID)
					srcDataPtr = (uint32 *)gMpuDebugLocalSpecAndVarDataPtr;
				else
					srcDataPtr = (uint32 *)gDiuDebugLocalSpecAndVarDataPtr;

				fourBytesDataCopy ( 	(uint32 *)&localData[0],
										(uint32 *)(srcDataPtr+localFbMemInfo.specActionIndex),
										localFbMemInfo.inputNo
							  	);
					
				
				if(moduleId == LOGIC_ID)
					srcDataPtr = (uint32 *)gMpuDebugRemoteSpecAndVarDataPtr;
				else
					srcDataPtr = (uint32 *)gDiuDebugRemoteSpecAndVarDataPtr;

				fourBytesDataCopy ( 	(uint32 *)&remoteData[0],
										(uint32 *)(srcDataPtr+remoteFbMemInfo.specActionIndex),
										remoteFbMemInfo.inputNo
							  	);
				retVal = dataCompare(	(uint32 *)&localData, 
										(uint32 *)&remoteData,
										remoteFbMemInfo.inputNo
									);
				if(retVal != NO_ERROR) {
					compareResult = 1;
					fdprintf(gDebugPortFd,"****************************************************************************\n");
					fdprintf(gDebugPortFd,"*       #%03d Addr FB[%d] Spec Header Different\n", fbBlockAddr, remoteFbMemInfo.functionCode);
					fdprintf(gDebugPortFd,"****************************************************************************\n");
					for(loop = 0; loop < remoteFbMemInfo.inputNo; loop++){
						fdprintf(gDebugPortFd,"[SPEC(%03d)]- Local Data : %08d : Remote Data : %08d\n",
								loop, localData[loop], remoteData[loop]);
					}
				}

				
				
				if(moduleId == LOGIC_ID)
					srcDataPtr = (uint32 *)gMpuDebugLocalSpecAndVarDataPtr;
				else
					srcDataPtr = (uint32 *)gDiuDebugLocalSpecAndVarDataPtr;

				fourBytesDataCopy ( 	(uint32 *)&localData[0],
										(uint32 *)(srcDataPtr+localFbMemInfo.specIndex),
										localFbMemInfo.inputSize
							  	);
					
				
				if(moduleId == LOGIC_ID)
					srcDataPtr = (uint32 *)gMpuDebugRemoteSpecAndVarDataPtr;
				else
					srcDataPtr = (uint32 *)gDiuDebugRemoteSpecAndVarDataPtr;

				fourBytesDataCopy ( 	(uint32 *)&remoteData[0],
										(uint32 *)(srcDataPtr+remoteFbMemInfo.specIndex),
										remoteFbMemInfo.inputSize
							  	);
				retVal = dataCompare(	(uint32 *)&localData, 
										(uint32 *)&remoteData,
										remoteFbMemInfo.inputSize
									);
				if(retVal != NO_ERROR) {
					compareResult = 1;
					fdprintf(gDebugPortFd,"****************************************************************************\n");
					fdprintf(gDebugPortFd,"*       #%03d Addr FB[%d] Spec Data Different\n", fbBlockAddr, remoteFbMemInfo.functionCode);
					fdprintf(gDebugPortFd,"****************************************************************************\n");
					for(loop = 0; loop < remoteFbMemInfo.inputSize; loop++){
						fdprintf(gDebugPortFd,"[SPEC(%03d)]- Local Data : %08d : Remote Data : %08d\n",
								loop, localData[loop], remoteData[loop]);
					}
				}
				
			}
		}
	}
	
	if(compareResult == NO_ERROR)
		fdprintf(gDebugPortFd, "***** There is NO Different Data Between FB Data !!!\n");

	return(status);
}



static uint32 compareSegmentTableInfo(uint32 moduleId, strModuleInfo *moduleInfoPtr)
{
	uint32 status = NO_ERROR, retVal = NO_ERROR;
	uint32	taskId, loop;
	strFbTaskTblMemInfo	localSegInfo, remoteSegInfo;
	uint32	*localDataPtr, *remoteDataPtr;
	uint32	compareResult = 0;
	
	for(taskId = 1; taskId <= moduleInfoPtr->maxSegmentNum; taskId++) {
		
		status = segTblInfoReadForDebugging(	moduleId, 
												taskId, 
												&localSegInfo, 
												LOCAL_DEBUGGING_MEMORY_REGION
											);
		if(status != NO_ERROR){
			fdprintf(gDebugPortFd,"[ERROR] - Seg Table Info Read Error !!\n");
			 return(1);
		}
		
		status = segTblInfoReadForDebugging(	moduleId, 
												taskId, 
												&remoteSegInfo, 
												REMOTE_DEBUGGING_MEMORY_REGION
											);
		if(status != NO_ERROR){
			fdprintf(gDebugPortFd,"[ERROR] - Seg Table Info Read Error !!\n");			
			 return(1);
		}
		
		
		
		if( (localSegInfo.taskId != remoteSegInfo.taskId ) || 
			(localSegInfo.taskMode != remoteSegInfo.taskMode ) || 
			(localSegInfo.logicCheckSum != remoteSegInfo.logicCheckSum ) || 
			(localSegInfo.logicCheckSumStatus != remoteSegInfo.logicCheckSumStatus ) || 
			(localSegInfo.recvMemSize != remoteSegInfo.recvMemSize ) || 
			(localSegInfo.memInputSize != remoteSegInfo.memInputSize ) || 
			(localSegInfo.memOutputSize != remoteSegInfo.memOutputSize ) || 
			(localSegInfo.taskCycleTime != remoteSegInfo.taskCycleTime ) || 
			(localSegInfo.execMode != remoteSegInfo.execMode ) || 
			(localSegInfo.u1.evtActFbStartAddr != remoteSegInfo.u1.evtActFbStartAddr ) || 
			(localSegInfo.u2.evtSetFbEndAddr != remoteSegInfo.u2.evtSetFbEndAddr ) || 
			(localSegInfo.taskPriority != remoteSegInfo.taskPriority ) || 
			(localSegInfo.fbUsedNum != remoteSegInfo.fbUsedNum ) || 
			(localSegInfo.logicType != remoteSegInfo.logicType )  
		) {
			retVal = 1;
		}
		

		if(retVal != NO_ERROR){
			compareResult = 1;
			localDataPtr 	= (uint32 *)&localSegInfo;
			remoteDataPtr	= (uint32 *)&remoteSegInfo;
			
			fdprintf(gDebugPortFd,"****************************************************************************\n");
			fdprintf(gDebugPortFd,"*       #%03d Segment Table Info Data Different\n", taskId);
			fdprintf(gDebugPortFd,"****************************************************************************\n");
			for(loop = 0; loop < (sizeof(strFbTaskTblMemInfo)/4); loop++){
				fdprintf(gDebugPortFd,"[SegInfo(%03d)]- Local Data : %08d : Remote Data : %08d\n",
								loop, *localDataPtr++, *remoteDataPtr++);
			}
		}		
	}	
	
	if(compareResult == NO_ERROR)
		fdprintf(gDebugPortFd,"***** There is NO Different Data Between Segments !!!\n");
	return(status);
}

static uint32 dataCompare(uint32 *localDataPtr, uint32 *remoteDataPtr, uint32 dataNum)
{
	uint32	status = NO_ERROR;
	
	while(dataNum--) {
		if(*localDataPtr++ != *remoteDataPtr++) {
			status = 1;
			break;	
		}	
	}	
	return(status);
}
	

#define	MAX_DUAL_COPY_NUM		(10000)
#define	DUAL_COPY_DELAY_TIME	(10) 

static uint32 debugDataCopyFromLocalRamOrBusMemoryBoard(uint32 moduleId,uint32	copyFlag)
{
	uint32 *srcPtr, *destPtr;
	uint32	dataNum, status = NO_ERROR;
	uint32	loop = 0;
	strSysConfigInfo configData;
	
   	systemCnfgInfoRead((strSysConfigInfo *)&configData);

	fdprintf(gDebugPortFd, "\n Waitting For a seconds !\n");
	
	if(moduleId == LOGIC_ID) {
		switch(copyFlag){
			case COMPARE_BETWEEN_LOCAL_RAM_AND_LOCAL_BOARD:
		    	
    			srcPtr 		= (uint32 *)gMpuFbSegTblInfoLocalPtr;
    			destPtr 	= (uint32 *)gMpuDebugLocalFbSegInfoPtr;
   				dataNum		= MAX_MPU_MEM_SIZE / 4;

    			fourBytesDataCopy (destPtr, srcPtr, dataNum);

		    	
    			srcPtr 		= (uint32 *)gMpuFbSegTblInfoBusPtr ;
    			destPtr 	= (uint32 *)gMpuDebugRemoteFbSegInfoPtr;
   				dataNum		= MAX_MPU_MEM_SIZE / 4;
    			for(loop = 0; loop < dataNum; loop++, srcPtr++, destPtr++) {
					status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
											BUS_DATA_READ ,
											(uint8 *)srcPtr, 
											(uint8 *)destPtr
							 			 );
					if(status != NO_ERROR) {
						fdprintf(gDebugPortFd,"[ERROR(LINE%d)] - BUS READ ERROR[%d]\n", (uint32)__LINE__, loop);
						return(1);	
					}	
					
					if( (loop != 0) && !(loop%MAX_DUAL_COPY_NUM)){
						fdprintf(gDebugPortFd, ".");
						sysTaskDelay (DUAL_COPY_DELAY_TIME);		
					}				 			 
				}
			break;
#ifdef BIT3_BOARD
			case COMPARE_BETWEEN_LOCAL_RAM_AND_REMOTE_BOARD:
		    	
    			srcPtr 		= (uint32 *)gMpuFbSegTblInfoLocalPtr;
    			destPtr 	= (uint32 *)gMpuDebugLocalFbSegInfoPtr;
   				dataNum		= MAX_MPU_MEM_SIZE / 4;

    			fourBytesDataCopy (destPtr, srcPtr, dataNum);

		    	
    			if(configData.shelfType == MASTER_SHELF)
        		    srcPtr	= (uint32 *)MEM_BOARD_BUS_ADDR_ON_SLAVE;
				else if(configData.shelfType == SLAVE_SHELF)
        		    srcPtr	= (uint32 *)MEM_BOARD_BUS_ADDR_ON_MASTER;
        		else {
						fdprintf(gDebugPortFd,"[ERROR] - Shelf Type Error[%d]\n", configData.shelfType);
						return(1);	        			
        		}
	
	   			destPtr 	= (uint32 *)gMpuDebugRemoteFbSegInfoPtr;
   				dataNum		= MAX_MPU_MEM_SIZE / 4;
   				
    			for(loop = 0; loop < dataNum; loop++, srcPtr++, destPtr++) {
					status = myRmtDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
											BUS_DATA_READ ,
											(uint8 *)srcPtr, 
											(uint8 *)destPtr
							 			 );
					if(status != NO_ERROR) {
						fdprintf(gDebugPortFd,"[ERROR(LINE%d)] - Remote BUS READ ERROR[%d]\n", (uint32)__LINE__,loop);
						return(1);	
					}							 			 
					if( (loop != 0) && !(loop%MAX_DUAL_COPY_NUM)){
						fdprintf(gDebugPortFd, ".");
						sysTaskDelay (DUAL_COPY_DELAY_TIME);		
					}				 			 
				}
			break;
#endif 
#ifdef BIT3_BOARD
			case COMPARE_BETWEEN_LOCAL_BOARD_AND_REMOTE_BOARD:
		    	
    			srcPtr 		= (uint32 *)gMpuFbSegTblInfoBusPtr ;
    			destPtr 	= (uint32 *)gMpuDebugLocalFbSegInfoPtr;
   				dataNum		= MAX_MPU_MEM_SIZE / 4;
    			for(loop = 0; loop < dataNum; loop++, srcPtr++, destPtr++) {
					status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
											BUS_DATA_READ ,
											(uint8 *)srcPtr, 
											(uint8 *)destPtr
							 			 );
					if(status != NO_ERROR) {
						fdprintf(gDebugPortFd,"[ERROR(LINE%d)] - BUS READ ERROR[%d]\n",(uint32)__LINE__, loop);
						return(1);	
					}							 			 
					if( (loop != 0) && !(loop%MAX_DUAL_COPY_NUM)){
						fdprintf(gDebugPortFd, ".");
						sysTaskDelay (DUAL_COPY_DELAY_TIME);		
					}				 			 
				}
		    	
    			if(configData.shelfType == MASTER_SHELF)
        		    srcPtr	= (uint32 *)MEM_BOARD_BUS_ADDR_ON_SLAVE;
				else if(configData.shelfType == SLAVE_SHELF)
        		    srcPtr	= (uint32 *)MEM_BOARD_BUS_ADDR_ON_MASTER;
        		else {
						fdprintf(gDebugPortFd,"[ERROR(LINE%d)] - Shelf Type Error[%d]\n", (uint32)__LINE__,configData.shelfType);
						return(1);	        			
        		}
	
	   			destPtr 	= (uint32 *)gMpuDebugRemoteFbSegInfoPtr;
   				dataNum		= MAX_MPU_MEM_SIZE / 4;
   				
    			for(loop = 0; loop < dataNum; loop++, srcPtr++, destPtr++) {
					status = myRmtDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
											BUS_DATA_READ ,
											(uint8 *)srcPtr, 
											(uint8 *)destPtr
							 			 );
					if(status != NO_ERROR) {
						fdprintf(gDebugPortFd,"[ERROR(line%d)] - Remote BUS READ ERROR[%d]\n", (uint32)__LINE__,loop);
						return(1);	
					}							 			 
					if( (loop != 0) && !(loop%MAX_DUAL_COPY_NUM)){
						fdprintf(gDebugPortFd, ".");
						sysTaskDelay (DUAL_COPY_DELAY_TIME);		
					}				 			 
				}			
			break;
#endif 
			default:
				fdprintf(gDebugPortFd,"[ERROR(Line%d)]- Copy Field Flag[%d] Error\n", (uint32)__LINE__,copyFlag);
				return(1);
			break;	
		}			
	}	
	else {
		fdprintf(gDebugPortFd,"[ERROR(Line%d)] - Module ID[%d] Error\n", (uint32)__LINE__,moduleId);
		return(1);	
	}		

	fdprintf(gDebugPortFd, "\n");
	return(status);
}


uint32 debugFbMemoryAddrInit(void)
{
	uint32	status = NO_ERROR;
	
	status = debugLocalFbMemoryAddrInit();
	if(status) return(status);
	
	status = debugRemoteFbMemoryAddrInit();
	
	return(status);
}

static uint32	debugLocalFbMemoryAddrInit(void)
{
	uint32	status = NO_ERROR;
	uint32	fbCodeInfoAddr, fbSpecAndVarStartAddr;
	uint32	fbOutputStartAddr, fbSegTblStartAddr;
	uint32	sharedMemStartAddr,variableStartAddr;
	uint32  sfcInfoStartAddr, sysExeStartAddr;
	uint32 size1, size2;

	
	
	fbSegTblStartAddr = (uint32)(uint32)DEBUG_LOCAL_MEMORY_ADDR_1;
    gMpuDebugLocalFbSegInfoPtr = (strFbTaskTblMemInfo *)fbSegTblStartAddr;

	
	fbCodeInfoAddr = fbSegTblStartAddr + sizeof(strFbTaskTblMemInfo) * MAX_LOGIC_TASK_NUM;
    gMpuDebugLocalFbMemInfoPtr = (strFbMemInfo *)fbCodeInfoAddr;

	
	fbSpecAndVarStartAddr = fbCodeInfoAddr + sizeof(strFbMemInfo) *MAX_MPU_BLOCK_ADDR;            	
 	gMpuDebugLocalSpecAndVarDataPtr = (void *)fbSpecAndVarStartAddr;

	
	fbOutputStartAddr = fbSpecAndVarStartAddr + (MAX_MPU_SPEC_NUM * 4);
	gMpuDebugLocalOutputDataPtr = (void *)fbOutputStartAddr;

    
	sharedMemStartAddr = fbOutputStartAddr + (MAX_MPU_OUTPUT_NUM * 4);
	gDebugLocalSTSharedMemDataPtr = (uint32 *) sharedMemStartAddr;

	
	variableStartAddr = sharedMemStartAddr + MAX_SHARED_MEM_SIZE ;
	gDebugLocalVarInfoPtr = (void *) variableStartAddr;

	
	sfcInfoStartAddr = variableStartAddr + (MAX_MPU_RETAIN_NUM / 8)+ 
						(MAX_MPU_MMEM_NUM * 4 ) +(MAX_MPU_AUTOVAR_NUM * 4 );
	gDebugLocalSFCInfoPtr = (uint32 *)sfcInfoStartAddr;

	
	sysExeStartAddr = sfcInfoStartAddr + sizeof(strStepInfo) * MAX_STEP_NUM
		+ sizeof(strActionInfo) * MAX_ACTION_NUM + sizeof(strTransitionInfo) * MAX_TRANSITOIN_NUM;
	gDebugLocalSysExeInfoPtr  = (void *)sysExeStartAddr;

    
    
	size1 = sysExeStartAddr + MAX_SYS_MEM_SIZE;

	size2 = fbSegTblStartAddr + MAX_MPU_MEM_SIZE;

    if( size1 > size2 )
        status = MPU_MEM_SIZE_EXCEED_ERR;
	
	
    if( gMpuDebugLocalFbMemInfoPtr == NULL) return (MEM_ADDR_NULL_ERR);
 	
   	fbSegTblStartAddr = ((uint32)gMpuDebugLocalFbSegInfoPtr + MAX_MPU_MEM_SIZE);
	gDiuDebugLocalFbSegInfoPtr = (strFbTaskTblMemInfo *)fbSegTblStartAddr;

   	 
    fbCodeInfoAddr = fbSegTblStartAddr + sizeof(strFbTaskTblMemInfo) * MAX_DIU_SEG_NUM
			+ sizeof(strFbTaskTblMemInfo) * MAX_TMTC_SEG_NUM + sizeof(strFbTaskTblMemInfo) * MAX_FIELDBUS_SEG_NUM;
	gDiuDebugLocalFbMemInfoPtr = (strFbMemInfo *)fbCodeInfoAddr;
	
	 
	fbSpecAndVarStartAddr = fbCodeInfoAddr + sizeof(strFbMemInfo) *MAX_DIU_BLOCK_ADDR;            	
	gDiuDebugLocalSpecAndVarDataPtr = (uint32 *)fbSpecAndVarStartAddr;
 
	
	fbOutputStartAddr = fbSpecAndVarStartAddr + (MAX_DIU_SPEC_NUM * 4);
	gDiuDebugLocalOutputDataPtr = (uint32 *)fbOutputStartAddr;


    
    
	size1 = fbOutputStartAddr + (MAX_DIU_OUTPUT_NUM * 4) + (MAX_TMTC_OUTPUT_NUM * 4);
	size2 = fbSegTblStartAddr + MAX_DIU_MEM_SIZE;
	if( size1 > size2 )
            	status = DIU_MEM_SIZE_EXCEED_ERR;

    return(status);
}

static uint32	debugRemoteFbMemoryAddrInit(void)
{
	uint32	status = NO_ERROR;
	uint32	fbCodeInfoAddr, fbSpecAndVarStartAddr;
	uint32	fbOutputStartAddr, fbSegTblStartAddr;
	uint32	sharedMemStartAddr,variableStartAddr;
	uint32  sfcInfoStartAddr, sysExeStartAddr;
	uint32 size1, size2;

	
	
	fbSegTblStartAddr = (uint32)DEBUG_LOCAL_MEMORY_ADDR_1;
    gMpuDebugRemoteFbSegInfoPtr = (strFbTaskTblMemInfo *)fbSegTblStartAddr;

	
	fbCodeInfoAddr = fbSegTblStartAddr + sizeof(strFbTaskTblMemInfo) * MAX_LOGIC_TASK_NUM;
    gMpuDebugRemoteFbMemInfoPtr = (strFbMemInfo *)fbCodeInfoAddr;

	
	fbSpecAndVarStartAddr = fbCodeInfoAddr + sizeof(strFbMemInfo) *MAX_MPU_BLOCK_ADDR;            	
 	gMpuDebugRemoteSpecAndVarDataPtr = (void *)fbSpecAndVarStartAddr;

	
	fbOutputStartAddr = fbSpecAndVarStartAddr + (MAX_MPU_SPEC_NUM * 4);
	gMpuDebugRemoteOutputDataPtr = (void *)fbOutputStartAddr;

    
	sharedMemStartAddr = fbOutputStartAddr + (MAX_MPU_OUTPUT_NUM * 4);
	gDebugRemoteSTSharedMemDataPtr = (uint32 *) sharedMemStartAddr;

	
	variableStartAddr = sharedMemStartAddr + MAX_SHARED_MEM_SIZE ;
	gDebugRemoteVarInfoPtr = (void *) variableStartAddr;

	
	sfcInfoStartAddr = variableStartAddr + (MAX_MPU_RETAIN_NUM / 8)+ 
						(MAX_MPU_MMEM_NUM * 4 ) +(MAX_MPU_AUTOVAR_NUM * 4 );
	gDebugRemoteSFCInfoPtr = (uint32 *)sfcInfoStartAddr;

	sysExeStartAddr = sfcInfoStartAddr + sizeof(strStepInfo) * MAX_STEP_NUM
		+ sizeof(strActionInfo) * MAX_ACTION_NUM + sizeof(strTransitionInfo) * MAX_TRANSITOIN_NUM;
	gDebugRemoteSysExeInfoPtr  = (void *)sysExeStartAddr;

    
    
	size1 = sysExeStartAddr + MAX_SYS_MEM_SIZE;

	size2 = fbSegTblStartAddr + MAX_MPU_MEM_SIZE;

    if( size1 > size2 )
        status = MPU_MEM_SIZE_EXCEED_ERR;
	
	
    if( gMpuDebugRemoteFbMemInfoPtr == NULL) return (MEM_ADDR_NULL_ERR);
 	
   	fbSegTblStartAddr = ((uint32)gMpuDebugLocalFbSegInfoPtr + MAX_MPU_MEM_SIZE);
	gDiuDebugRemoteFbSegInfoPtr = (strFbTaskTblMemInfo *)fbSegTblStartAddr;

   	 
    fbCodeInfoAddr= fbSegTblStartAddr + sizeof(strFbTaskTblMemInfo) * MAX_DIU_SEG_NUM
			+ sizeof(strFbTaskTblMemInfo) * MAX_TMTC_SEG_NUM + sizeof(strFbTaskTblMemInfo) * MAX_FIELDBUS_SEG_NUM;
	gDiuDebugRemoteFbMemInfoPtr = (strFbMemInfo *)fbCodeInfoAddr;
	
	 
	fbSpecAndVarStartAddr = fbCodeInfoAddr + sizeof(strFbMemInfo) *MAX_DIU_BLOCK_ADDR;            	
	gDiuDebugRemoteSpecAndVarDataPtr = (uint32 *)fbSpecAndVarStartAddr;
 
	
	fbOutputStartAddr = fbSpecAndVarStartAddr + (MAX_DIU_SPEC_NUM * 4);
	gDiuDebugRemoteOutputDataPtr = (uint32 *)fbOutputStartAddr;


    
    
	size1 = fbOutputStartAddr + (MAX_DIU_OUTPUT_NUM * 4) + (MAX_TMTC_OUTPUT_NUM * 4);
	size2 = fbSegTblStartAddr + MAX_DIU_MEM_SIZE;
	if( size1 > size2 )
            	status = DIU_MEM_SIZE_EXCEED_ERR;

    return(status);
}

static uint32 segTblInfoReadForDebugging
				( 
			uint32 moduleId,
            uint32	taskId,
			strFbTaskTblMemInfo	*segTblPtr,
			uint32	memoryType
				)
{
	uint32	status = NO_ERROR;
	uint32	*srcMemoryPtr;
	
	switch(moduleId) {
		case LOGIC_ID:
			if(memoryType == LOCAL_DEBUGGING_MEMORY_REGION)
				srcMemoryPtr = (uint32 *)gMpuDebugLocalFbSegInfoPtr;
			else if(memoryType == REMOTE_DEBUGGING_MEMORY_REGION)
				srcMemoryPtr = (uint32 *)gMpuDebugRemoteFbSegInfoPtr;	
			else {
				fdprintf(gDebugPortFd,"[ERROR] - Invalid Memory Region[%d]\n", memoryType);
				return(1);	
			}
			
        	if( (taskId >= 1) && (taskId <= MAX_LOGIC_TASK_NUM))
            	memoryCopy ( 
						(uint8 *) segTblPtr,
   						(uint8 *)(srcMemoryPtr + sizeof(strFbTaskTblMemInfo)*(taskId - 1)),
						sizeof(strFbTaskTblMemInfo)
							);
			else
				status = L_SEG_ID_ERR; 
		break;
		default:
			fdprintf(gDebugPortFd,"[ERROR] - Module ID[0x%x] Error\n", moduleId);
			return(1);
		break;	
	}
	
	return(status);
}

static uint32 fbMemInfoReadForDebugging
				(
			uint32	moduleId,
            uint32	fbBlockAddr,
            strFbMemInfo	*fbMemPtr,
            uint32	memoryType
            	)
{
	uint32	status = NO_ERROR;
	strFbMemInfo	*dataPtr;

	uint32	*srcMemoryPtr;
	
	switch(moduleId) {
		case LOGIC_ID:
			if(memoryType == LOCAL_DEBUGGING_MEMORY_REGION)
				srcMemoryPtr = (uint32 *)gMpuDebugLocalFbMemInfoPtr;
			else if(memoryType == REMOTE_DEBUGGING_MEMORY_REGION)
				srcMemoryPtr = (uint32 *)gMpuDebugRemoteFbMemInfoPtr;	
			else {
				fdprintf(gDebugPortFd,"[ERROR] - Invalid Memory Region[%d] in fbMemInfoReadForDebugging\n", memoryType);
				return(1);	
			}
			
 		break;
		default:
			fdprintf(gDebugPortFd,"[Error(%d)]- Module Id Error[%d] In fbMemInfoReadForDebugging\n", __LINE__,moduleId);
			return(1);
		break;
	}
    if(status == NO_ERROR) {
		dataPtr = (strFbMemInfo *) srcMemoryPtr;

    	
    	dataPtr += fbBlockAddr;
 
    	memoryCopy ((uint8 *) fbMemPtr,
        			(uint8 *) dataPtr,
        			sizeof(strFbMemInfo)
        			);
	}
    return(status);
}




static uint32 compareSfcDataInfo(uint32 moduleId)
{
	uint32 status = NO_ERROR, retVal = NO_ERROR;

	strStepInfo	*localStepPtr, *remoteStepPtr;
	strTransitionInfo *localTrsPtr, *remoteTrsPtr;
	strActionInfo *localActPtr, *remoteActPtr;

	uint32  i , compareResult=0 , loop; 
	uint32  *localDataPtr, *remoteDataPtr;

	if(moduleId != LOGIC_ID) return(status);

	localStepPtr = (strStepInfo*)gDebugLocalSFCInfoPtr;
	remoteStepPtr = (strStepInfo*)gDebugRemoteSFCInfoPtr;

	for(i= 0; i < MAX_STEP_NUM;i++) {
		retVal = dataCompare(	(uint32 *)localStepPtr, 
								(uint32 *)remoteStepPtr,
								sizeof(strStepInfo)/4 - 3 ) ;

		if(retVal != NO_ERROR) {
			compareResult = 1;
			localDataPtr 	= (uint32 *)localStepPtr;
			remoteDataPtr 	= (uint32 *)remoteStepPtr;

			fdprintf(gDebugPortFd,"****************************************************************************\n");
			fdprintf(gDebugPortFd,"*       #%03d STEP Info Data Different\n", i+1);
			fdprintf(gDebugPortFd,"****************************************************************************\n");
			for(loop = 0; loop < (sizeof(strStepInfo)/4); loop++) {
				fdprintf(gDebugPortFd,"[STEP INFO(%03d)]- Local Data : 0x%08x : Remote Data : 0x%08x\n",
							loop, *localDataPtr++, *remoteDataPtr++);			
			}
		}
		localStepPtr++; 
		remoteStepPtr++;
	}
		

	localTrsPtr = (strTransitionInfo*) (gDebugLocalSFCInfoPtr + sizeof(strStepInfo) * MAX_STEP_NUM);
	remoteTrsPtr = (strTransitionInfo*) (gDebugRemoteSFCInfoPtr + sizeof(strStepInfo) * MAX_STEP_NUM);
	for(i= 0; i < MAX_TRANSITOIN_NUM;i++) {
		retVal = dataCompare(	(uint32 *)localTrsPtr, 
								(uint32 *)remoteTrsPtr,
								sizeof(strTransitionInfo)/4 - 1) ;

		if(retVal == NO_ERROR) {
			if( (localTrsPtr->preStepNum != remoteTrsPtr->preStepNum ) ||
				(localTrsPtr->NextStepNum != remoteTrsPtr->NextStepNum) 
		      ) {
					retVal = 1; 
			}
		}

		if(retVal != NO_ERROR) {
			compareResult = 1;
			localDataPtr 	= (uint32 *)localTrsPtr;
			remoteDataPtr 	= (uint32 *)remoteTrsPtr;

			fdprintf(gDebugPortFd,"****************************************************************************\n");
			fdprintf(gDebugPortFd,"*       #%03d TRANSITOIN Info Data Different\n", i+1);
			fdprintf(gDebugPortFd,"****************************************************************************\n");
			for(loop = 0; loop < (sizeof(strTransitionInfo)/4); loop++) {
				fdprintf(gDebugPortFd,"[TRS INFO(%03d)]- Local Data : %08d : Remote Data : %08d\n",
							loop, *localDataPtr++, *remoteDataPtr++);			
			}
		}
		localTrsPtr++; 
		remoteTrsPtr++;
	}

	localActPtr = (strActionInfo*) (gDebugLocalSFCInfoPtr
			+ sizeof(strStepInfo) * MAX_STEP_NUM + sizeof(strTransitionInfo) * MAX_TRANSITOIN_NUM );
	remoteActPtr = (strActionInfo*) (gDebugRemoteSFCInfoPtr
		    + sizeof(strStepInfo) * MAX_STEP_NUM + sizeof(strTransitionInfo) * MAX_TRANSITOIN_NUM);

	for(i= 0; i < MAX_ACTION_NUM;i++) {
		retVal = dataCompare(	(uint32 *)localActPtr, 
								(uint32 *)remoteActPtr,
								sizeof(strActionInfo)/4 - 3) ;

		if(retVal != NO_ERROR) {
			compareResult = 1;
			localDataPtr 	= (uint32 *)localActPtr;
			remoteDataPtr 	= (uint32 *)remoteActPtr;

			fdprintf(gDebugPortFd,"****************************************************************************\n");
			fdprintf(gDebugPortFd,"*       #%03d ACTOIN Info Data Different\n", i+1);
			fdprintf(gDebugPortFd,"****************************************************************************\n");
			for(loop = 0; loop < (sizeof(strActionInfo)/4); loop++) {
				fdprintf(gDebugPortFd,"[ACT INFO(%03d)]- Local Data : %08d : Remote Data : %08d\n",
							loop, *localDataPtr++, *remoteDataPtr++);			
			}
		}
		localActPtr++; 
		localActPtr++;
	}
	
	if(compareResult == NO_ERROR)
		fdprintf(gDebugPortFd,"***** There is NO Different Data Between SFC Data !!!\n");

	return(status);
}

static uint32 compareSysExecInfo(uint32 moduleId)
{
	uint32 status = NO_ERROR, retVal = NO_ERROR;
	strSysExecInfo *localSysExecPtr, *romoteSysExecPtr ;
	uint32 loop, compareResult = 0;

	localSysExecPtr = (strSysExecInfo *)gDebugLocalSysExeInfoPtr;
	romoteSysExecPtr = (strSysExecInfo *)gDebugRemoteSysExeInfoPtr;

	if(moduleId == LOGIC_ID) {
		retVal = dataCompare(	(uint32 *)&localSysExecPtr->mpuSysExeInfo, 
							(uint32 *)&romoteSysExecPtr->mpuSysExeInfo,
							sizeof(strModuleExecInfo)/4 -1  
						);
		if(retVal != NO_ERROR) {
			retVal = dataCompare(	(uint32 *)&localSysExecPtr->autovarSysExeInfo, 
					(uint32 *)&romoteSysExecPtr->autovarSysExeInfo,
					sizeof(strVariableExecInfo)/4
				);
		}
		if(retVal == NO_ERROR) {
			retVal = dataCompare(	(uint32 *)&localSysExecPtr->flagvarSysExeInfo, 
					(uint32 *)&romoteSysExecPtr->flagvarSysExeInfo,
					sizeof(strVariableExecInfo)/4
				);
		}
		if(retVal == NO_ERROR) {
			retVal = dataCompare(	(uint32 *)&localSysExecPtr->sfcSysExeInfo, 
					(uint32 *)&romoteSysExecPtr->sfcSysExeInfo,
					sizeof(strSfcMaxExecInfo)/4
				);
		}
	}
	else {
		retVal = dataCompare(	(uint32 *)&localSysExecPtr->diuSysExeInfo, 
							(uint32 *)&romoteSysExecPtr->diuSysExeInfo,
							sizeof(strModuleExecInfo)/4 - 1  
						);
		if(retVal == NO_ERROR) {
			retVal = dataCompare(	(uint32 *)&localSysExecPtr->fbtmSysExeInfo, 
							(uint32 *)&romoteSysExecPtr->fbtmSysExeInfo,
							sizeof(strfbtmModuleExecInfo)/4 - 2 
							);	
		}
	}

	if(retVal != NO_ERROR) {
		compareResult = 1;
		fdprintf(gDebugPortFd,"****************************************************************************\n");
		fdprintf(gDebugPortFd,"*       System Exec Memory Info Data Different\n");
		fdprintf(gDebugPortFd,"****************************************************************************\n");
		for(loop = 0; loop < (sizeof(strSysExecInfo)/4); loop++) {
			fdprintf(gDebugPortFd,"[SYS EXEC INFO(%03d)]- Local Data : %08d : Remote Data : %08d\n",
						loop, *localSysExecPtr++, *romoteSysExecPtr++);					
		}
	}

	if(compareResult == NO_ERROR)
		fdprintf(gDebugPortFd, "***** There is NO Different Data Between System Exec Data !!!\n");

	return(status);
}
#endif 

#endif 
