
#include	<stdio.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include	"systemDiag.h"
#include	"usrDefBuf.h"
#include	"fbMem.h"
#include	"usrErrCode.h"
#include	"sysConfig.h"

#include	"memDef.h"
#include	"usrQueData.h"
#include	"osDepend.h"
#include	"usrSema.h"

#include	"netProtocol.h"

#include	"segScheduler.h"


#include	"protocolBuf.h"

#include	"fbDef.h"


#include	"rmt_comm\rmtDevUtil.h"
#include	"sysDual.h"

#include	"sfcMem.h"

#include	"segScheduler.h"

#define	MAX_FB_EXEC_SEQ_ID_DISPLAY_NUM 256

#define	FIND_SAME_MEM_SIZE		1
#define	FIND_GREATER_MEM_SIZE	2


static strFbCodeAddrListInfo	sFbCodeAddrListInfo;



int32 gFbExecSeqIdQSemaId,gSFCSequncerQSemaId;

static strFbExecQueInfo	sFbExecQMngrInfo;
static strFbExecutionInfo	sFbExecDataInfo[MAX_FB_EXEC_SEQ_ID_NUM];


static strOnlineDwlAddFbCodeMngrInfo		sOnlineDwlFbAddInfo;

static strOnlineDwlDelFbCodeMngrInfo		sOnlineDwlFbDelInfo;

static strOnlineDwlSpecChgFbCodeMngrInfo	sOnlineDwlFbSpecChgInfo;

static strOnlineDwlFbSeqIdAddAndChgMngrInfo	sOnlineDwlFbSeqIdAddAndChgInfo;

static strOnlineDwlVarInfoChgMngrInfo		sOnlineDwlVarInfoChgInfo;

static strOnlineDwlChgTrsDataMngrInfo		sOnlineDwlTrsChgInfo;

static strOnlineDwlFbInfoChgFbCodeMngrInfo	sOnlineDwlFbInfoChgInfo;





static strOnlineDwlDelFbMemMngrInfo	sOnlineDwlFbDelMemInfo;



#define	MAX_FB_SPEC_FREE_MEM_MNGR_NUM	2000

static int32 sFbSpecMemUnusedQSemaId;

static strFbSpecUnusedMemQueInfo	sFbSpecUnusedMemQMngrInfo;
static strFbSpecUnusedMemInfo		sFbSpecUnusedMemDataInfo[MAX_FB_SPEC_FREE_MEM_MNGR_NUM];


static int32 sFbOutputMemUnusedQSemaId;
static strFbSpecUnusedMemQueInfo	sFbOutputUnusedMemQMngrInfo;
static strFbSpecUnusedMemInfo		sFbOutputUnusedMemDataInfo[MAX_FB_SPEC_FREE_MEM_MNGR_NUM];



#define	MAX_ONLINE_DWL_TASK_COM_QUE_NUM	10

static int32	sOnlineDwlTaskComQSemaId;

static	strOnlineDwlTaskComReqInfo	sOnlineDwlTaskComReqDataInfo[MAX_ONLINE_DWL_TASK_COM_QUE_NUM];
static 	strOnlineDwlTaskComQueReqInfo	sOnlineDwlTaskComReqMngrInfo;

static strOnlineDwlTaskComRespInfo		sOnlineDwlTaskComRespDataInfo[MAX_ONLINE_DWL_TASK_COM_QUE_NUM];
static strOnlineDwlTaskComQueRespInfo	sOnlineDwlTaskComRespMngrInfo;

static uint32	sMpuOnlineDwlOperationFlag 	= ONLINE_DWL_OPERATION_STOP;
static uint32	sDiuOnlineDwlOperationFlag 	= ONLINE_DWL_OPERATION_STOP;


#define	ONLINE_DWL_LOGIC_UPDATE_RESP_DELAY (systemClockRateGet()/10) 

#define	ONLINE_DWL_LOGIC_UPDATE_RESP_TIMEOUT	5


#define	FB_SPEC_MEM_REALLOC_RESP_DELAY (systemClockRateGet()/10) 


#define	FB_SPEC_MEM_REALLOC_RESP_TIMEOUT	5




static strFbSeqIdDwlInfo	sOnlineDwlFbSeqIdDwlVarInfo[MAX_LOGIC_TASK_NUM];



static uint32	sPrevFbExecId = 0;

strFbExecutionInfo* gFBExeSequenceQueuePtr[MAX_FB_EXEC_SEQ_ID_NUM];


extern int32 gDebugPortFd;
extern strFbExecDbInfo	gFbCodeSortingTableInfo[MAX_FB_CODE_ID+1];
extern strFbMemInfo		*gMpuFbMemInfoLocalPtr,*gMpuFbMemInfoBusPtr;
extern void		*gMpuSpecAndVarDataLocalPtr,*gMpuSpecAndVarDataBusPtr;
extern strFbTaskTblMemInfo	*gMpuFbSegTblInfoLocalPtr,*gMpuFbSegTblInfoBusPtr;


extern strFbMemInfo		*gMpuFbMemInfoLocalPtr,*gMpuFbMemInfoBusPtr;
extern void		*gMpuSpecAndVarDataLocalPtr,*gMpuSpecAndVarDataBusPtr;

extern void		*gMpuOutputDataLocalPtr,*gMpuOutputDataBusPtr;

strFbTaskTblMemInfo	*gMpuFbSegTblInfoLocalPtr,*gMpuFbSegTblInfoBusPtr;

extern strMpuDiuRunningStatusInfo *gDiuRasInfoPtr;


extern strFbMemInfo *gDiuFbMemInfoLocalPtr, *gDiuFbMemInfoBusPtr;
extern void	*gDiuSpecAndVarDataLocalPtr,*gDiuSpecAndVarDataBusPtr;



extern uint32	gMpuOverTimeCheckFlag;


uint32	execSequenceVarInit(uint32);
uint32	sfcSequenceVarInit(uint32);

static uint32	fbExecIdQueInit(void);
uint32 fbExecIdInfoClear(uint32 , uint32 );
uint32 fbExecIdInfoSet(uint32 , uint32 );
strFbExecutionInfo *fbExecIdStartMemInfoGet(uint32 );
strFbExecutionInfo *fbExecIdNextMemInfoGet(uint32 , strFbExecutionInfo *);

void fbExecIdQueCountInfoDisplay(uint32 , uint32 );
void fbExecIdQueInfoDisplay(uint32 , uint32 );
void onlineDwlVariableClear(void);

static uint32	fbSpecUnusedMemoryQueInit(void);
uint32 fbSpecUnusedMemoryInfoClear( uint32 );
uint32 fbSpecUnusedMemoryInfoSet(uint32);
void fbUnusedSpecMemQueInfoDisplay(uint32, uint32 );
void fbSpecUnusedMemInfoDisplay(uint32 , uint32 );
uint32 onlineDwlFbCodeAddFuncInComTask(uint32 *,uint32 *,  uint32 *);
uint32 onlineDwlFbCodeSeqIdAddAndChgFuncInComTask(uint32 *,uint32 *,  uint32 *);
uint32 onlineDwlFbCodeSpecChgFuncInComTask(uint32 *,uint32 *,  uint32 *);
uint32 onlineDwlFbCodeDelFuncInComTask(uint32 *,uint32 *, uint32 *);
uint32 onlineDwlFbCodeVarInfoChgFuncInComTask(uint32 *,uint32 *, uint32 *);
uint32 onlineDwlFbCodeTrsDataChgFuncInComTask(uint32 *,uint32 *, uint32 *);
uint32 onlineDwlFbCodeInfoChgFuncInComTask(uint32 *,uint32 *,  uint32 *);

uint32 onlineDwlNullFunc(uint32 *,uint32 *, uint32 *);

static uint32 onlineDwlFbCodeAddFuncInFbTask(strOnlineDwlTaskComReqInfo *);
static uint32 onlineDwlFbCodeDelFuncInFbTask(strOnlineDwlTaskComReqInfo *);
static uint32 onlineDwlFbCodeSpecChgFuncInFbTask(strOnlineDwlTaskComReqInfo *);
static uint32 onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask(strOnlineDwlTaskComReqInfo *);
static uint32 onlineDwlFbParaMemReallocInFbTask(strOnlineDwlTaskComReqInfo *);
static uint32 onlineDwlFbCodeVarInfoChgFuncInFbTask(strOnlineDwlTaskComReqInfo *);
static uint32 onlineDwlFbCodeTrsDataChgFuncInFbTask(strOnlineDwlTaskComReqInfo *);
static uint32 onlineDwlFbCodeInfoChgFuncInFbTask(strOnlineDwlTaskComReqInfo *);

uint32 fbSpecMemorySpaceCheck (uint32);
uint32	fbSpecUsableMemSet(void);

static uint32	fbSpecMemRealloc(void);
static uint32	fbSpecFreeMemReallocCheck(void);

void onlineDwlfbCodeAddInfoDisplay(void);
void onlineDwlfbCodeDelInfoDisplay(void);
void onlineDwlfbCodeSpecChgInfoDisplay(void);
void onlineDwlfbCodeSeqIdAddAndChgInfoDisplay(void);
uint32	onlineDwlTaskQueInit(void);
uint32 onlineDwlReqDataSendToFbTask(uint32 , uint32 *, uint32 *);
void fbTaskComQueCountInfoDisplay(void);
void onlineDwlVarChangeInfoDisplay(void);

void onlineDownloadFlagCheckAndUpdate(uint32);

static uint32 fbSpecMemorySpaceGet (uint32, strFbSpecUnusedMemInfo *);
static uint32 onlineDwlFbCodeSetInLocalAndBusMemory
	(strOnlineDwlAddFbCodeInfo *,strFbSpecUnusedMemInfo *,strFbSpecUnusedMemInfo* );

static uint32 onlineDwlFbCodeDelInLocalAndBusMemory(strOnlineDwlDelFbCodeInfo	*);
static uint32 fbExecIdInfoDelete(uint32 , uint32 , uint32 );
static uint32	fbSpecMemorySpaceRelease (uint32,uint32);
static uint32 onlineDwlFbCodeSpecChgInLocalAndBusMemory(uint32, uint32,strOnlineDwlChgSpecInfo *);
static uint32 onlineDwlFbCodeTypeChgInLocalAndBusMemory(uint32, uint32, uint32);
static uint32 onlineDwlFbOutputActionChgInLocalAndBusMemory(uint32, uint32, uint32 *);
uint32	onlineDwlFbCodeSeqIdVarCompare(uint32 	);
uint32	onlineDwlFbCodeSeqIdVarSet(	uint32 , uint32, uint32); 
uint32 onlineDwlFbCodeSeqIdVarInit(uint32,uint32);

static uint32	onlineDwlSegmentCheckSumCalcu(uint32, uint32,uint32,uint32);

uint32	onlineDwlAccessFlagCheck(uint32);
static uint32 onlineDwlTaskReqQueClear(uint32 );

static uint32 onlineDwlDelFbMemorySpecCheck(uint32,uint32);
static uint32 onlineDwlDelFbMemoryOutputCheck(uint32,uint32);

static uint32	onlineDwlDelFbMemorySet(void);

uint32 fbUnusedSpecMemInfoRead(	strFbSpecMemStatusReadRespInfo *);
strFbExecutionInfo *nonCyclicFbExecIdStartMemInfoGet(uint32 , uint32 );

static void fineSameMemSizeAndInfoSet(void);

static uint32	fbOutputUnusedMemoryQueInit(void);
uint32 fbOutputUnusedMemoryInfoClear( uint32 );
uint32 fbOutputUnusedMemoryInfoSet(uint32);

uint32 fbOutputMemorySpaceCheck ( uint32 );
static uint32 fbOutputMemorySpaceGet (uint32, strFbSpecUnusedMemInfo *);
uint32	fbOutputUsableMemSet(void);
static uint32	fbOutputMemRealloc(void);
static uint32	fbOutputFreeMemReallocCheck(void);
static uint32	fbOutputMemorySpaceRelease (uint32,uint32);

uint32 onlineDwlFbAddVariableClear(void);
uint32 onlineDwlFbDelVariableClear(void);
uint32 onlineDwlFbSpecChgVariableClear(void);
uint32 onlineDwlFbSeqIdAddAndChgVariableClear(void);
uint32 onlineDwlVarInfoChgVariableClear(void);
uint32 onlineDwlTrsDataChgVariableClear(void);
uint32 onlineDwlFbInfoChgVariableClear(void);
void fbExeSequenceQueuePtrClear(uint32);
uint32 ioPointDisconnAlarmFlagSet(uint32, uint32, strFbMemInfo *, uint32	*);

static uint32  onlineDwlIOCodeSpecChgInLocalAndBusMemory(uint32, uint32,strOnlineDwlChgSpecInfo *);


extern void memoryClear(uint8 *, uint32);
extern void memoryCopy(uint8 *, uint8 *, uint32);

extern void setErrorCode(uint8 *, uint32, uint8 *, uint32);
extern void setErrorCodeWithVal(uint8 *, uint32, uint8 *, uint32,
				uint8 *, uint32, uint8 *, uint32, uint8 *, uint32);
extern uint32 fbCodeMemInfoRead(uint32, uint32, strFbMemInfo *);

extern uint32 getFbTaskTblInfo(uint32 , uint32 , strFbTaskTblMemInfo *);
extern uint32 nodePutOnLast(strDoubleLink **, strDoubleLink *);
extern uint32 nodePutOnLast(strDoubleLink **, strDoubleLink *);
extern strDoubleLink *nodeGetAndUnlink(strDoubleLink **);
extern strDoubleLink *nextNodeRef(strDoubleLink *,strDoubleLink *);
extern int32   ioRead(int32 , int8 *, int32 );
extern void spaceSkip(int8 **);
extern uint32	systemCnfgInfoRead( strSysConfigInfo *);
extern int32 nodePutOnPrev(strDoubleLink **,strDoubleLink *, strDoubleLink *);
extern int32 	semaphoreGet(int32 , int32 );
#ifdef VXWORKS
extern int32 	semaphoreRelease(int32);
#else	
extern int32 	semaphoreRelease(int32, int32);
extern int32	semaphoreRelease2(uint8 *, uint32, int32, int32);
#endif
extern int32 	semaphoreCreate(strSemaInfo *, int32 *);
extern void errorStringDisplay(uint8 *,uint32,uint8 *,uint32);
extern int32 nodeUnlink( strDoubleLink **,strDoubleLink *);
extern void segmentChecksumCalcu (strFbTaskTblMemInfo *, strFbMemInfo *,uint32	*,uint32);
extern int32 systemClockRateGet(void);
extern void sysTaskDelay (int32 );

extern uint32	rmtEthComSeqNumChkDBSet(uint32);
extern void fourBytesDataCopy ( uint32 *, uint32 *, uint32 );

extern uint32 readRuntimeFbData( uint32,uint32,uint32, uint32 *);
extern uint32 writeRuntimeFbData( uint32,uint32,uint32, uint32 *);
extern uint32 fbMemInfoRead(uint32, uint32, strFbMemInfo *);
extern void systemRasInfoSet(uint32 , uint32 );
extern void logMessageSet (int8 *, uint32, int8 *, uint32, uint32 );

extern void sysRmtIpAddrSetToSysConfigMemory(uint32, uint32 );

extern uint32 sfcExecIdInfoClear(uint32 , uint32 );
extern uint32 segTblPointerGet(uint32 ,uint32	,strFbTaskTblMemInfo	**);
extern uint32	fbSysExeModuleInfoRead( uint32, strModuleExecInfo* );
extern uint32 fbSysExeModuleInfoWrite(uint32, strModuleExecInfo* );

extern uint32 segTblInfoWrite(uint32 , uint32 , strFbTaskTblMemInfo *);
extern uint32 sfcExecIdQueInit(void);
extern uint32 sfcExecIdInfoSet(uint32, uint32);
extern uint32 sfcRuntimeInfoClear(void);

extern uint32 trsExecIdInfoDelete(uint32, uint32, uint32);
extern uint32 actExecIdInfoDelete(uint32, uint32, uint32);
extern uint32 fbCodeParaTypePointerGet(uint32	,strFbCodeTypePointerInfo *);

extern uint32	sfcMemPointerGet(uint32, strSfcMemPointerInfo *);
extern uint32  localVariableRetainInfoWrite(uint32, uint32, uint8);
extern uint32  busVariableRetainInfoWrite(uint32, uint32, uint8);
extern uint32 specActionDataCheck(uint32 *, uint32  );

extern uint32  refDataCopyToMem(uint32*, uint32, strFbSpecActionInfo, uint32);
extern uint32  refDataCopyToBusMem(uint32*, uint32, strFbSpecActionInfo, uint32);
extern uint32   refDataCopyToRmtMem(uint32*, uint32, strFbSpecActionInfo, uint32);
extern uint32  mpusegSchedulerStartAddrSet(uint32);

extern uint32 jmpFbCodeCheck(uint32);
extern uint32 jmpFbAddressCheck(uint32,uint32,strFbMemInfo*);
extern uint32 rmtbusFbMemPointerGet(uint32,strMemPointerInfo	*);
extern uint32 dualFlagCheckInRunShelf(void);
extern uint32 myBusDataAcc(uint8 *, uint32, uint32, uint32, uint8 *, uint8 *);
#ifdef _WIN32
extern uint32 myRmtDataAcc(uint8 *, uint32, uint32	, uint32 ,int8 *, int8 *);
#endif
extern uint32 segmentStatusRead (uint32, uint32, strSegStatusInfo *);
extern uint32 sfcLoopStatusWrite(uint32,uint32,uint32,uint32); 
extern uint32 fbSysMaxJmpLoopInfo(uint32);
extern void		eightByteConversion( uint32 *, uint32 *, uint8 );
extern void		mySwap(void *, void *, uint32);
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
extern uint32 refDatacopyToInitVarBusMem(uint32 *,uint32 , strFbSpecActionInfo , uint32);
#endif
extern uint32	fbSysExeVarInfoRead(uint32 , strVariableExecInfo* );
extern uint32	fbSysExeVarInfoWrite(uint32 , strVariableExecInfo* );


uint32 (*onlineDownloadAccessFuncInComTask[8])(uint32 *,uint32 *, uint32 *)
={
	onlineDwlFbCodeAddFuncInComTask,
	onlineDwlFbCodeDelFuncInComTask,
	onlineDwlFbCodeSpecChgFuncInComTask,
	onlineDwlFbCodeSeqIdAddAndChgFuncInComTask,
	onlineDwlFbCodeVarInfoChgFuncInComTask,
	onlineDwlFbCodeTrsDataChgFuncInComTask,
	onlineDwlFbCodeInfoChgFuncInComTask,
	onlineDwlNullFunc
};

uint32 (*onlineDownloadAccessFuncInFbTask[9])(strOnlineDwlTaskComReqInfo *)
={
	onlineDwlFbCodeAddFuncInFbTask,
	onlineDwlFbCodeDelFuncInFbTask,
	onlineDwlFbCodeSpecChgFuncInFbTask,
	onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask,
	onlineDwlFbCodeVarInfoChgFuncInFbTask,
	onlineDwlFbCodeTrsDataChgFuncInFbTask,
	onlineDwlFbCodeInfoChgFuncInFbTask,
	0/*onlineDwlIOCodeSpecChgFuncInFbTask*/,
	onlineDwlFbParaMemReallocInFbTask
};

uint32 onlineDwlNullFunc(	uint32 	*reqDataPtr, 
							uint32	*respDataPtr,
							uint32	*respRetLenPtr
							)
{
	return(	ONLINE_DWL_KIND_ERR);
}


void onlineDownloadFlagCheckAndUpdate(uint32 moduleID)
{
	uint32	status = NO_ERROR, errStatus,loop, index, copyIndex = 0, respDataNum = 0, operationCode;
	strOnlineDwlTaskComReqInfo	operationList[MAX_ONLINE_DWL_OPERATION_NUM];
	strOnlineDwlTaskComReqInfo	*reqNodePtr;
	strOnlineDwlTaskComRespInfo	*respNodePtr;
	strFBOnlineLogicUpdateRespInfo	updateRespInfo[MAX_ONLINE_DWL_KIND],*updateRespInfoPtr;

	strSysDualInfo	dualMsgInfo, dualCmdInfo;
	strMpuDiuRunningStatusInfo *diuStatusInfoPtr = gDiuRasInfoPtr;

	uint8	onlineDwlExecList[MAX_ONLINE_DWL_OPERATION_NUM] = {
						ONLINE_DWL_FBCODE_DEL,
						ONLINE_DWL_FBCODE_SPEC_CHANGE,		
						ONLINE_DWL_IO_SPEC_CHANGE,		
						ONLINE_DWL_FBCODE_ADD, 	
						ONLINE_DWL_FBCODE_SEQ_ID_ADD_AND_CHG,
						ONLINE_DWL_FBCODE_CHANGE,		
						ONLINE_DWL_VAR_INFO_CHANGE,
						ONLINE_DWL_TRS_DATA_CHANGE,
						ONLINE_FB_PARA_REALLOC	
	};	

		
	if(moduleID == LOGIC_ID) {
		if(sMpuOnlineDwlOperationFlag != ONLINE_DWL_OPERATION_START)
		return;
	}

	memoryClear ( (uint8 *)&operationList[0], sizeof(strOnlineDwlTaskComReqInfo)*MAX_ONLINE_DWL_OPERATION_NUM);
	memoryClear ( (uint8 *)&updateRespInfo[0], sizeof(strFBOnlineLogicUpdateRespInfo)*MAX_ONLINE_DWL_KIND);
	
	updateRespInfoPtr = &updateRespInfo[0];
		
	
	status = semaphoreGet(sOnlineDwlTaskComQSemaId, NOWAIT);
	if(status != NO_ERROR) {
		setErrorCode(__FILE__,__LINE__,"onlineDownloadFlagCheckAndUpdate",status);
		goto MODULE_END;	
	}
	
	reqNodePtr = (strOnlineDwlTaskComReqInfo *)sOnlineDwlTaskComReqMngrInfo.usedList;
	
	if(reqNodePtr == NULL) {
		status = NO_EXIST_NODE_ERR;
		setErrorCode(__FILE__,__LINE__,"onlineDownloadFlagCheckAndUpdate",status);

		respDataNum = 1;
		
		
		
		updateRespInfoPtr->status = NO_ERROR;
		
		goto SEMAPHORE_RELEASE;	
	}	

	while(sOnlineDwlTaskComReqMngrInfo.usedList != NULL) 
	{
       	
		reqNodePtr = (strOnlineDwlTaskComReqInfo *)nodeGetAndUnlink(
                    		(strDoubleLink **)&sOnlineDwlTaskComReqMngrInfo.usedList
																);
      	if(reqNodePtr != NULL){
       		copyIndex = reqNodePtr->onlineDwlOperationCode;
      		if((copyIndex == 0) || (copyIndex > MAX_ONLINE_DWL_OPERATION_NUM)) 
      			{
       			status = FB_ONLINE_OP_CODE_ERR;
       			setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);

				respDataNum = 1;
				updateRespInfoPtr->status = FB_LOGIC_UPDATE_ERR&0xffff;

       			goto SEMAPHORE_RELEASE;	
       		} 
       		
       		memoryCopy( (uint8 *) &operationList[copyIndex-1],
       					(uint8 *)reqNodePtr,
       					sizeof(strOnlineDwlTaskComReqInfo)
       				  );
       				  
       		
       		memoryClear ( (uint8 *) reqNodePtr, sizeof(strOnlineDwlTaskComReqInfo));
       		(void)nodePutOnLast(	(strDoubleLink **) &sOnlineDwlTaskComReqMngrInfo.freeList,
           	  		               	(strDoubleLink *)reqNodePtr
               	   		        );
    	}		
	}
	
	
	
	respDataNum = 0;
	for(loop = 0; loop < MAX_ONLINE_DWL_OPERATION_NUM; loop++) {
		index = onlineDwlExecList[loop];
		
		operationCode = operationList[index-1].onlineDwlOperationCode;

		if((operationCode == 0)|| (operationCode > MAX_ONLINE_DWL_OPERATION_NUM)) continue;
		
		status = onlineDownloadAccessFuncInFbTask[operationCode - 1](&operationList[index-1]);

		if(status != NO_ERROR){
			setErrorCodeWithVal(	__FILE__,__LINE__, __FUNCTION__,status,
									"Operation Code", operationCode,
									"Not Using",0,
									"Not Using",0
								  );
			
			logMessageSet(	__FILE__, 
							__LINE__,
							"onlineDownloadFlagCheckAndUpdate", 
							status,0
			  			  );			
			respDataNum += 1;
			
			updateRespInfoPtr->status 	= status & 0xffff;
			updateRespInfoPtr->dwlKind	= operationCode;	

										
			if(diuStatusInfoPtr->redundancyStatus != NO_ERROR) {
				
		    	systemRasInfoSet(RAS_SYSTEM_STATUS, SYS_RAS_SYSTEM_STOP);
				
				logMessageSet(	__FILE__, 
								__LINE__,
								"onlineDownloadFlagCheckAndUpdate", 
								LOCAL_STOP_BY_ONLINE_DWL_UPDATE_ERR,0
				  			  );			
			}
			else {
				 
		    	systemRasInfoSet(RAS_SYSTEM_STATUS, SYS_RAS_SYSTEM_STOP);
			    systemRasInfoSet(RAS_REDUNDANCY_STATUS, SYS_RAS_REDUNDANCY_STANDBY);
			    
	            dualMsgInfo.dualCmd = ONLINE_DWL_REMOTE_RUN_CMD;
				
				logMessageSet(	__FILE__, 
								__LINE__,
								"onlineDownloadFlagCheckAndUpdate", 
								LOCAL_STANDBY_BY_ONLINE_DWL_UPDATE_ERR,0
				  			  );			
			}

			break;
		}
		else {
			respDataNum	+= 1;	
			updateRespInfoPtr->status 	= NO_ERROR;
			updateRespInfoPtr->dwlKind	= operationCode;	
		}
		
		if(respDataNum > MAX_ONLINE_DWL_KIND) {
			setErrorCodeWithVal(	__FILE__,__LINE__,__FUNCTION__, ONLINE_DWL_KIND_ERR,
									"resp Data Num", respDataNum,
									"MAx Resp Data Num",MAX_ONLINE_DWL_KIND,
									"Not Using",0
								  );		
			break;	
		}
		
		updateRespInfoPtr++;
	}		
SEMAPHORE_RELEASE:

	
	respNodePtr = (strOnlineDwlTaskComRespInfo *)nodeGetAndUnlink(
                    		(strDoubleLink **)&sOnlineDwlTaskComRespMngrInfo.freeList
																);
	
	if(respNodePtr == NULL) 
	{
		status = NO_EXIST_NODE_ERR;
		setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
	}
	else {
        respNodePtr->respDataNum = respDataNum;
		memoryCopy ((uint8 *) &respNodePtr->respDataBuf[0],
					(uint8 *) &updateRespInfo,
					sizeof(strFBOnlineLogicUpdateRespInfo)*respDataNum
				   );                    												
		
       	(void)nodePutOnLast(	(strDoubleLink **) &sOnlineDwlTaskComRespMngrInfo.usedList,
          		               	(strDoubleLink *)respNodePtr
           	   		        );
	}
	
#ifdef VXWORKS
	semaphoreRelease (sOnlineDwlTaskComQSemaId);
#else	
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sOnlineDwlTaskComQSemaId);
#endif

MODULE_END:
		
	if(moduleID == LOGIC_ID) 
		sMpuOnlineDwlOperationFlag = ONLINE_DWL_OPERATION_STOP;	

	return;
}


uint32	execSequenceVarInit(uint32 moduleId)
{
	uint32 status = NO_ERROR;
	uint32	taskId;
    strSemaInfo     sema;
 
	if(moduleId != LOGIC_ID)
		return(L_MODULE_ID_ERR);
		

    
    
#ifdef VXWORKS
    sema.semaType       = MUTEX_SEMA;
    sema.semaOptions    = MYSEM_Q_PRIORITY|
                          MYSEM_DELETE_SAFE|
                          MYSEM_INVERSION_SAFE;
#else
    sema.semaType       = MUTEX_SEMA;
	sema.semaInitVal	= FALSE;
	sema.semaName		= "FBExeSeqId";

#endif
    
    status = semaphoreCreate(   (strSemaInfo *) &sema,
                                 (int32*)&gFbExecSeqIdQSemaId
                            );
	if(status != NO_ERROR) {
		printf( "\n[%s(%d)] - [execSequenceVarInit] - [Semaphore Create Err] !!\n",
        						(uint8 *)__FILE__, __LINE__);
		return(status);
    }

	
	status = fbExecIdQueInit();
	if(status != NO_ERROR) {
		printf( "[ERROR] - fbExecIdQueInit() Call Error\n");
		return(status);  	
	}
    
    
#ifdef VXWORKS
    sema.semaType       = MUTEX_SEMA;
    sema.semaOptions    = MYSEM_Q_PRIORITY|
                          MYSEM_DELETE_SAFE|
                          MYSEM_INVERSION_SAFE;
#else
    sema.semaType       = MUTEX_SEMA;
	sema.semaInitVal	= FALSE;
	sema.semaName		= "SFCLinkSeqId";

#endif

    status = semaphoreCreate(   (strSemaInfo *) &sema,
                                 (int32*)&gSFCSequncerQSemaId
                            );
	if(status != NO_ERROR) {
		printf( "\n[%s(%d)] - [execSequenceVarInit] - [Semaphore Create Err] !!\n",
        						(uint8 *)__FILE__, __LINE__);
		return(status);
    }

	
	status = sfcExecIdQueInit();
	if(status != NO_ERROR) {
		printf( "[ERROR] - sfcExecIdQueInit() Call Error\n");
		return(status);  	
	}

	
	status = sfcRuntimeInfoClear();
	if(status != NO_ERROR) {
		printf( "[ERROR] - sfcRuntimeInfoClear() Call Error\n");
		return(status);  	
	}
	
	
	fbExeSequenceQueuePtrClear(TOTAL_DWL);
		
	for(taskId = 1; taskId <= MAX_LOGIC_TASK_NUM; taskId++) {
		
		status = fbExecIdInfoSet(taskId, moduleId);
		if(status != NO_ERROR) {
			printf( "[ERROR] - fbExecIdInfoSet() Call Error In #%d Seg ID\n", taskId);
			return(status); 	
		}	

		
		status = sfcExecIdInfoSet(taskId, moduleId);
		if(status != NO_ERROR) {
			printf( "[ERROR] - sfcExecIdInfoSet() Call Error In #%d Seg ID\n", taskId);
			return(status);  	
		}
	}

    
    
#ifdef VXWORKS
    sema.semaType       = MUTEX_SEMA;
    sema.semaOptions    = MYSEM_Q_PRIORITY|
                          MYSEM_DELETE_SAFE|
                          MYSEM_INVERSION_SAFE;

#else
    sema.semaType       = MUTEX_SEMA;
	sema.semaInitVal	= FALSE;
	sema.semaName		= "FBUnusedSpecQId";
#endif
    

    status = semaphoreCreate(   (strSemaInfo *) &sema,
                                 &sFbSpecMemUnusedQSemaId
                            );
	if(status != NO_ERROR) {
		printf( "\n[%s(%d)] - [execSequenceVarInit] - [Semaphore Create Err] !!\n",
        						(uint8 *)__FILE__, __LINE__);
		return(status);
    }
	
	status = fbSpecUnusedMemoryQueInit();
	
	if(status != NO_ERROR) {
		printf( "[ERROR] - fbSpecUnusedMemoryQueInit() Call Error\n");
		return(status);  	
	}
	
	 
	status = fbSpecUnusedMemoryInfoSet(moduleId);
	if(status != NO_ERROR) {
		printf( "[ERROR] - fbSpecUnusedMemoryInfoSet() Call Error In #%d Module ID\n", moduleId);
		return(status); 	
	}	

    
    
#ifdef VXWORKS
    sema.semaType       = MUTEX_SEMA;
    sema.semaOptions    = MYSEM_Q_PRIORITY|
                          MYSEM_DELETE_SAFE|
                          MYSEM_INVERSION_SAFE;

#else
    sema.semaType       = MUTEX_SEMA;
	sema.semaInitVal	= FALSE;
	sema.semaName		= "FBUnusedOutputQId";
#endif
    

    status = semaphoreCreate(   (strSemaInfo *) &sema,
                                 &sFbOutputMemUnusedQSemaId
                            );
	if(status != NO_ERROR) {
		printf( "\n[%s(%d)] - [execSequenceVarInit] - [Semaphore Create Err] !!\n",
        						(uint8 *)__FILE__, __LINE__);
		return(status);
    }
	
	status = fbOutputUnusedMemoryQueInit();
	
	if(status != NO_ERROR) {
		printf( "[ERROR] - fbOutputUnusedMemoryQueInit() Call Error\n");
		return(status);  	
	}
	
	 
	status = fbOutputUnusedMemoryInfoSet(moduleId);
	if(status != NO_ERROR) {
		printf( "[ERROR] - fbOutputUnusedMemoryInfoSet() Call Error In #%d Module ID\n", moduleId);
		return(status); 	
	}	


   
    
#ifdef VXWORKS
    sema.semaType       = MUTEX_SEMA;
    sema.semaOptions    = MYSEM_Q_PRIORITY|
                          MYSEM_DELETE_SAFE|
                          MYSEM_INVERSION_SAFE;
#else
    sema.semaType       = MUTEX_SEMA;
	sema.semaInitVal	= FALSE;
	sema.semaName		= "OnlineDownComQId";
#endif
    

    status = semaphoreCreate(   (strSemaInfo *) &sema,
                                 &sOnlineDwlTaskComQSemaId
                            );
	if(status != NO_ERROR) {
		printf( "\n[%s(%d)] - [execSequenceVarInit] - [Semaphore Create Err] !!\n",
        						(uint8 *)__FILE__, __LINE__);
		return(status);
    }

	status = onlineDwlTaskQueInit();
	if(status != NO_ERROR) {
		printf( "[ERROR] - onlineDwlTaskQueInit() Call Error !!\n");
		return(status);	
	}
	return(status);
}



uint32	onlineDwlTaskQueInit(void)
{
	uint32	status = NO_ERROR;
 	uint32	loop;

    
    sOnlineDwlTaskComReqMngrInfo.freeList 	= NULL;
    sOnlineDwlTaskComReqMngrInfo.usedList 	= NULL;

    sOnlineDwlTaskComRespMngrInfo.freeList 	= NULL;
    sOnlineDwlTaskComRespMngrInfo.usedList 	= NULL;
	

    for( loop = 0; loop < MAX_ONLINE_DWL_TASK_COM_QUE_NUM; loop++) {
		memoryClear (   (char *) &sOnlineDwlTaskComReqDataInfo[loop],
                         sizeof(strOnlineDwlTaskComReqInfo)
                    );
        status = nodePutOnLast	(	(strDoubleLink **) &sOnlineDwlTaskComReqMngrInfo.freeList,
                                	(strDoubleLink *)  &sOnlineDwlTaskComReqDataInfo[loop]
                            	);
		if(status != NO_ERROR)
			return(status);                            
    
		memoryClear (   (char *) &sOnlineDwlTaskComRespDataInfo[loop],
                         sizeof(strOnlineDwlTaskComRespInfo)
                    );
        status = nodePutOnLast	(	(strDoubleLink **) &sOnlineDwlTaskComRespMngrInfo.freeList,
                                	(strDoubleLink *)  &sOnlineDwlTaskComRespDataInfo[loop]
                            	);
		if(status != NO_ERROR)
			return(status);                                
    }
	return(status);
}


uint32 onlineDwlTaskQueClear(void)
{
	uint32	status = NO_ERROR;
	strOnlineDwlTaskComReqInfo		*reqNodePtr;	
	strOnlineDwlTaskComRespInfo		*respNodePtr;
	
	
    status = semaphoreGet(sOnlineDwlTaskComQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR) 
	{
		setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}
	
	reqNodePtr = (strOnlineDwlTaskComReqInfo *)sOnlineDwlTaskComReqMngrInfo.usedList;
	
	if(reqNodePtr != NULL) {
		
    	while( sOnlineDwlTaskComReqMngrInfo.usedList != NULL) {
        	
   			reqNodePtr = (strOnlineDwlTaskComReqInfo *)nodeGetAndUnlink(
                    		(strDoubleLink **)&sOnlineDwlTaskComReqMngrInfo.usedList
																);
        	if(reqNodePtr != NULL){
           		
           		memoryClear ( (uint8 *) reqNodePtr, sizeof(strOnlineDwlTaskComReqInfo));
           		(void)nodePutOnLast(	(strDoubleLink **) &sOnlineDwlTaskComReqMngrInfo.freeList,
               	  		               	(strDoubleLink *)reqNodePtr
                   	   		        );
        	}
    	}
	}
	
	respNodePtr = (strOnlineDwlTaskComRespInfo *)sOnlineDwlTaskComRespMngrInfo.usedList;
	
	if(respNodePtr != NULL) {
		
    	while( sOnlineDwlTaskComRespMngrInfo.usedList != NULL) {
        	
   			respNodePtr = (strOnlineDwlTaskComRespInfo *)nodeGetAndUnlink(
                    		(strDoubleLink **)&sOnlineDwlTaskComRespMngrInfo.usedList
																);
        	if(respNodePtr != NULL){
           		
           		memoryClear ( (uint8 *) respNodePtr, sizeof(strOnlineDwlTaskComRespInfo));
           		(void)nodePutOnLast(	(strDoubleLink **) &sOnlineDwlTaskComRespMngrInfo.freeList,
               	  		               	(strDoubleLink *)respNodePtr
                   	   		        );
        	}
    	}		
	}

	
#ifdef VXWORKS
	semaphoreRelease (sOnlineDwlTaskComQSemaId);
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sOnlineDwlTaskComQSemaId);
#endif

	return(status);
}


static uint32 onlineDwlTaskReqQueClear(uint32 onlineDwlOperationCode)
{
	uint32	status = NO_ERROR;
	strOnlineDwlTaskComReqInfo		*reqNodePtr;	
	strOnlineDwlTaskComRespInfo		*respNodePtr;
	
	
    status = semaphoreGet(sOnlineDwlTaskComQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);

	if(status != NO_ERROR) 
	{
		setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}
	
	reqNodePtr = (strOnlineDwlTaskComReqInfo *)sOnlineDwlTaskComReqMngrInfo.usedList;
	
   	while( reqNodePtr != NULL) {
   		if(onlineDwlOperationCode == reqNodePtr->onlineDwlOperationCode) {
       		
       		status = nodeUnlink( 	
  	        					(strDoubleLink **)&sOnlineDwlTaskComReqMngrInfo.usedList,
   								(strDoubleLink *)reqNodePtr
           						);
       		if(reqNodePtr != NULL){
       			
       			memoryClear ( (uint8 *) reqNodePtr, sizeof(strOnlineDwlTaskComReqInfo));
       			(void)nodePutOnLast(	(strDoubleLink **) &sOnlineDwlTaskComReqMngrInfo.freeList,
           	  			               	(strDoubleLink *)reqNodePtr
               	   			        );
       		}
       		break;
       	}
       	else
			reqNodePtr = (strOnlineDwlTaskComReqInfo *) nextNodeRef (
													(strDoubleLink *)sOnlineDwlTaskComReqMngrInfo.usedList,
													(strDoubleLink *)reqNodePtr
																	);
	}
	
	respNodePtr = (strOnlineDwlTaskComRespInfo *)sOnlineDwlTaskComRespMngrInfo.usedList;
		
    while( respNodePtr != NULL) {
       	
   		respNodePtr = (strOnlineDwlTaskComRespInfo *)nodeGetAndUnlink(
               		(strDoubleLink **)&sOnlineDwlTaskComRespMngrInfo.usedList
																);
       	if(respNodePtr != NULL){
       		
       		memoryClear ( (uint8 *) respNodePtr, sizeof(strOnlineDwlTaskComRespInfo));
       		(void)nodePutOnLast(	(strDoubleLink **) &sOnlineDwlTaskComRespMngrInfo.freeList,
           	  		               	(strDoubleLink *)respNodePtr
               	   		        );
       	}
    }		

	
#ifdef VXWORKS
	semaphoreRelease (sOnlineDwlTaskComQSemaId);
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sOnlineDwlTaskComQSemaId);
#endif
	return(status);
}


uint32 onlineDwlReqDataSendToFbTask(uint32 operationCode, uint32 *reqDataPtr, uint32 *respDataPtr)
{
	uint32	status = NO_ERROR, waittingRespTime = 0, tmpStatus;
	strOnlineDwlTaskComRespInfo		*respNodePtr;
	strOnlineDwlTaskComReqInfo		*reqNodePtr, *curDataPtr, *reqUsedNodePtr;
	strSegStatusInfo segStatus;
	uint32 segLoopStatus = 0, SegIndex;
	
	switch(operationCode) {
		case ONLINE_DWL_FBCODE_ADD: 				
		case ONLINE_DWL_FBCODE_DEL:					
		case ONLINE_DWL_FBCODE_SPEC_CHANGE:			
		case ONLINE_DWL_FBCODE_SEQ_ID_ADD_AND_CHG:	
		case ONLINE_DWL_VAR_INFO_CHANGE	:			
		case ONLINE_DWL_TRS_DATA_CHANGE	:			
		case ONLINE_DWL_FBCODE_CHANGE	:			

			reqNodePtr = (strOnlineDwlTaskComReqInfo *)sOnlineDwlTaskComReqMngrInfo.freeList;
			
			if(reqNodePtr == NULL) {
				status = NO_EXIST_NODE_ERR;
       			setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
				return(status);	
			}

			
    		status = semaphoreGet(sOnlineDwlTaskComQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
			if(status != NO_ERROR) {
       			setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
				return(status);
			}
			
			reqUsedNodePtr = (strOnlineDwlTaskComReqInfo *)sOnlineDwlTaskComReqMngrInfo.usedList;

			while(reqUsedNodePtr != NULL) {
				if(reqUsedNodePtr->onlineDwlOperationCode == operationCode)
					break;	
					
				reqUsedNodePtr = (strOnlineDwlTaskComReqInfo *) nextNodeRef (
													(strDoubleLink *)sOnlineDwlTaskComReqMngrInfo.usedList,
													(strDoubleLink *)reqUsedNodePtr
																		);
			}
			
			if(reqUsedNodePtr == NULL) {

				
	 			reqNodePtr = (strOnlineDwlTaskComReqInfo *)nodeGetAndUnlink(
                    					(strDoubleLink **)&sOnlineDwlTaskComReqMngrInfo.freeList
																			);
      			if(reqNodePtr == NULL) {
					status = NO_EXIST_NODE_ERR;
	       			setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
 					goto SEMA_RELEASE;
       			}
 
      			reqNodePtr->onlineDwlOperationCode = operationCode;
      			
				(void)nodePutOnLast(	(strDoubleLink **) &sOnlineDwlTaskComReqMngrInfo.usedList,
       	  				               	(strDoubleLink *)reqNodePtr
           	   				        );
			}
SEMA_RELEASE:			           	   				        

			
#ifdef VXWORKS
			semaphoreRelease (sOnlineDwlTaskComQSemaId);
#else
			semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sOnlineDwlTaskComQSemaId);
#endif
	    break;		

		case ONLINE_FB_PARA_REALLOC:			
			
			memoryClear ((uint8 *)&sFbCodeAddrListInfo, sizeof(strFbCodeAddrListInfo));
			
			
			
			status = onlineDwlTaskQueClear();
			if(status != NO_ERROR) {
       			setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
				return(status);				
			}

			reqNodePtr = (strOnlineDwlTaskComReqInfo *)sOnlineDwlTaskComReqMngrInfo.freeList;
			
			
			if(reqNodePtr == NULL) {
				status = NO_ERROR;
				tmpStatus = NO_EXIST_NODE_ERR;
       			setErrorCode (__FILE__, __LINE__, __FUNCTION__, NO_EXIST_NODE_ERR);
				return(status);	
			}

			
    		status = semaphoreGet(sOnlineDwlTaskComQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
			if(status != NO_ERROR) {
       			setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
				return(status);
			}
	 		reqNodePtr = (strOnlineDwlTaskComReqInfo *)nodeGetAndUnlink(
                    					(strDoubleLink **)&sOnlineDwlTaskComReqMngrInfo.freeList
																		);
      		reqNodePtr->onlineDwlOperationCode = operationCode;
      		
      		curDataPtr = (strOnlineDwlTaskComReqInfo *)reqDataPtr;
      		
      		reqNodePtr->reqDataNum = curDataPtr->reqDataNum;
      		
			memoryCopy ((uint8 *)&reqNodePtr->reqDataBuf[0], 
						(uint8 *)&curDataPtr->reqDataBuf[0],
						reqNodePtr->reqDataNum * 4
					   );
        		
			(void)nodePutOnLast(	(strDoubleLink **)&sOnlineDwlTaskComReqMngrInfo.usedList,
       	  			               	(strDoubleLink *)reqNodePtr
           				        );
			
			
#ifdef VXWORKS
			semaphoreRelease (sOnlineDwlTaskComQSemaId);
#else
			semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sOnlineDwlTaskComQSemaId);
#endif

			
			sMpuOnlineDwlOperationFlag 	= ONLINE_DWL_OPERATION_START;
			
			
			gMpuOverTimeCheckFlag		= TASK_OVERTIME_CHK_ENABLE;
			
			
			
			
			for(;;) {
			
				respNodePtr = (strOnlineDwlTaskComRespInfo *)sOnlineDwlTaskComRespMngrInfo.usedList;
			
				if(respNodePtr != NULL) {
					
    				status = semaphoreGet(sOnlineDwlTaskComQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
					if(status != NO_ERROR) {
						setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
						goto CHECK_END;
					}
        			
   					respNodePtr = (strOnlineDwlTaskComRespInfo *)nodeGetAndUnlink(
                    				(strDoubleLink **)&sOnlineDwlTaskComRespMngrInfo.usedList
																				);
        			if(respNodePtr != NULL){
						
						memoryCopy ((uint8 *)respDataPtr, 
									(uint8 *)respNodePtr,
									sizeof(strOnlineDwlTaskComRespInfo)
								   );
							   
           				
           				memoryClear ( (uint8 *) respNodePtr, sizeof(strOnlineDwlTaskComRespInfo));
           				(void)nodePutOnLast(	(strDoubleLink **) &sOnlineDwlTaskComRespMngrInfo.freeList,
               	  		 	              		(strDoubleLink *)respNodePtr
                   	  	 		        	);
        				}
			
					
#ifdef VXWORKS
					semaphoreRelease (sOnlineDwlTaskComQSemaId);
#else
					semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sOnlineDwlTaskComQSemaId);
#endif

					goto CHECK_END;
				}
				sysTaskDelay(FB_SPEC_MEM_REALLOC_RESP_DELAY);
			
				
				if(++waittingRespTime > FB_SPEC_MEM_REALLOC_RESP_TIMEOUT) 
				{
	       			setErrorCode (__FILE__, __LINE__, __FUNCTION__, FB_MEM_REALLOC_TIMEOUT_ERR);
				
					status = FB_SPEC_MEM_REALLOC_ERR;
					goto CHECK_END;
				}
			}
CHECK_END:
			
			sMpuOnlineDwlOperationFlag = ONLINE_DWL_OPERATION_STOP;
			
			
			gMpuOverTimeCheckFlag		= TASK_OVERTIME_CHK_DISABLE;
			
		break;		

		case ONLINE_FB_LOGIC_UPDATE: 			
		
			
			sMpuOnlineDwlOperationFlag = ONLINE_DWL_OPERATION_START;
		
			
			gMpuOverTimeCheckFlag		= TASK_OVERTIME_CHK_ENABLE;

			
			
			for(;;) {
			
				respNodePtr = (strOnlineDwlTaskComRespInfo *)sOnlineDwlTaskComRespMngrInfo.usedList;
			
				if(respNodePtr != NULL) {
					
    				status = semaphoreGet(sOnlineDwlTaskComQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
					if(status != NO_ERROR) {
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						goto CHECK_END_1;
					}
        			
   					respNodePtr = (strOnlineDwlTaskComRespInfo *)nodeGetAndUnlink(
                	    				(strDoubleLink **)&sOnlineDwlTaskComRespMngrInfo.usedList
																				);
        			if(respNodePtr != NULL){
						
						memoryCopy ((uint8 *)respDataPtr, 
									(uint8 *)respNodePtr,
									sizeof(strOnlineDwlTaskComRespInfo)
								   );
							   
           				
           				memoryClear ( (uint8 *) respNodePtr, sizeof(strOnlineDwlTaskComRespInfo));
           				(void)nodePutOnLast(	(strDoubleLink **) &sOnlineDwlTaskComRespMngrInfo.freeList,
               	  			               	(strDoubleLink *)respNodePtr
                   	   			        	);
        			}
			
					
#ifdef VXWORKS
					semaphoreRelease (sOnlineDwlTaskComQSemaId);
#else
					semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sOnlineDwlTaskComQSemaId);
#endif
					goto CHECK_END_1;
				}
				sysTaskDelay(ONLINE_DWL_LOGIC_UPDATE_RESP_DELAY);
			
				
				if(++waittingRespTime > ONLINE_DWL_LOGIC_UPDATE_RESP_TIMEOUT) {
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, FB_UPDATE_RESP_TIMEOUT_ERR);
					status = FB_LOGIC_UPDATE_ERR;
					goto CHECK_END_1;
				}
			}
CHECK_END_1:
			
			for(SegIndex= 1; SegIndex<= MAX_LOGIC_TASK_NUM; SegIndex++) {
				status = segmentStatusRead (LOGIC_ID, SegIndex, &segStatus);
				if((status == NO_ERROR)&&(segStatus.segLoopStatus == FB_LOOPING_ERR)){
					segLoopStatus = 1;
					break;
				}
			}
			systemRasInfoSet(RAS_MPU_EXEC_LOOP_STATUS, segLoopStatus);

			
			sMpuOnlineDwlOperationFlag = ONLINE_DWL_OPERATION_STOP;

			
			gMpuOverTimeCheckFlag		= TASK_OVERTIME_CHK_DISABLE;
		break;

		default:
			status = L_OPTION_CODE_ERR;
			setErrorCodeWithVal (	__FILE__,__LINE__, __FUNCTION__, status,
									"operation Code", operationCode,
									"Not Using", 0, "Not Using", 0
								  );
		break;
	}

	return(status);
}


static uint32	fbSpecUnusedMemoryQueInit(void)
{
	uint32	status = NO_ERROR;
	uint32	loop;

    
    sFbSpecUnusedMemQMngrInfo.freeList = NULL;
    sFbSpecUnusedMemQMngrInfo.usedList = NULL;

    for( loop = 0; loop < MAX_FB_SPEC_FREE_MEM_MNGR_NUM; loop++) {
		memoryClear (   (char *) &sFbSpecUnusedMemDataInfo[loop],
                         sizeof(strFbSpecUnusedMemInfo)
                    );
        status = nodePutOnLast	(	(strDoubleLink **) &sFbSpecUnusedMemQMngrInfo.freeList,
                                	(strDoubleLink *)  &sFbSpecUnusedMemDataInfo[loop]
                            	);
		if(status != NO_ERROR)
			break;                            
    }
	
	return(status);
}


uint32 fbSpecUnusedMemoryInfoSet( uint32 moduleId)
{
	uint32	status = NO_ERROR, blockAddr = 0, usedMemSize;
	strFbMemInfo 		fbMemInfo;
	strFbSpecUnusedMemInfo	*curDataPtr,*tmpDataPtr;
	uint32	prevStartUnusedSpecMemIndex,prevEndUnusedSpecMemIndex;
	uint32 saveEndIndex, curEndSpecMemIndex;

	strModuleExecInfo sysExeInfo;
	strFbTaskTblMemInfo  segTblInfo;

	
	if( moduleId != LOGIC_ID){
		status = L_MODULE_ID_ERR;
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}	

	memoryClear( (uint8 *)&sysExeInfo, sizeof(strModuleExecInfo));
	status = fbSysExeModuleInfoRead(moduleId, &sysExeInfo);
	if(status!=NO_ERROR){
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}

	
    status = semaphoreGet(sFbSpecMemUnusedQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}

	
    if(	(MAX_MPU_BLOCK_ADDR - sysExeInfo.curfbMaxBlockAddr) > MAX_MPU_BLOCK_ADDR){
		status = L_MEM_SIZE_ERR;
        setErrorCodeWithVal( 	__FILE__, __LINE__, 
        						"fbSpecUnusedMemoryInfoSet",	status,
        						"Calcu FB PARA SIZE(BYTE)",sysExeInfo.curfbMaxBlockAddr,
        						"Recv FB PARA SIZE(BYTE)",MAX_MPU_BLOCK_ADDR  ,
								"NOT Usting", 0
        						);
		goto MODULE_END;    	
    }

	
   	curDataPtr = (strFbSpecUnusedMemInfo *)nodeGetAndUnlink (
                           		(strDoubleLink **)&sFbSpecUnusedMemQMngrInfo.freeList
            	                                    		);
	if(curDataPtr == NULL) {
		status = NO_EXIST_NODE_ERR;
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		goto MODULE_END;	
	}
	else {
		
		curDataPtr->unUsedMemStartIndex	= 0; 

		
		curDataPtr->unUsedMemEndIndex	= MAX_MPU_SPEC_NUM-1;
		 
		curDataPtr->freeMemSize	= curDataPtr->unUsedMemEndIndex - curDataPtr->unUsedMemStartIndex + 1;
		curDataPtr->freeMemSize	*= 4; 

		curDataPtr->usableMemSize = curDataPtr->freeMemSize;
		
		
		status = nodePutOnLast( (strDoubleLink **) &sFbSpecUnusedMemQMngrInfo.usedList,
       		            		(strDoubleLink *)curDataPtr
           		       		  );
		if(status) {
	        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
			goto MODULE_END;	
		}
	}
	
	
	for(blockAddr = 1; blockAddr <= sysExeInfo.curfbMaxBlockAddr; blockAddr++){  

		
    	if( (status = fbCodeMemInfoRead	(
											LOGIC_ID,
       										blockAddr,
       										(strFbMemInfo *) &fbMemInfo
       									)
			) != NO_ERROR
		){
	        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
			goto MODULE_END;
		}

		if( fbMemInfo.fbId == 0x0) continue;
			
		
		status = getFbTaskTblInfo(LOGIC_ID,
           						fbMemInfo.fbTaskId,
								(strFbTaskTblMemInfo	*) &segTblInfo);
			
  		if((status != NO_ERROR)||(segTblInfo.taskMode != PCS_RUN))
			continue;

		usedMemSize = (fbMemInfo.inputNo +  fbMemInfo.inputSize + fbMemInfo.intlVarSize) * 4;
    		
		
		
    	
    	curDataPtr = sFbSpecUnusedMemQMngrInfo.usedList;
    	
    	if(curDataPtr == NULL) goto MODULE_END;
    	
    	while(curDataPtr != NULL) {
    		
			
			prevStartUnusedSpecMemIndex 	= curDataPtr->unUsedMemStartIndex;
			prevEndUnusedSpecMemIndex		= curDataPtr->unUsedMemEndIndex;
	
    		
    		
    		if(fbMemInfo.specActionIndex == prevStartUnusedSpecMemIndex){

    			if(curDataPtr->freeMemSize == usedMemSize){
    				
					
   	        		status = nodeUnlink( 	
   	        					(strDoubleLink **)&sFbSpecUnusedMemQMngrInfo.usedList,
   								(strDoubleLink *)curDataPtr
                						);
		                            		
					if(status != NO_ERROR){
				        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
						goto MODULE_END;	
					}
					
					status = nodePutOnLast( (strDoubleLink **) &sFbSpecUnusedMemQMngrInfo.freeList,
       		        						(strDoubleLink *)curDataPtr
           		       		  			  );
					if(status != NO_ERROR){
				        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
						goto MODULE_END;	
					}
					
    				break;	
				}
				else if(curDataPtr->freeMemSize > usedMemSize){
    				
					curDataPtr->unUsedMemStartIndex		+= usedMemSize/4;
					curDataPtr->freeMemSize				-= usedMemSize; 
					curDataPtr->usableMemSize 			= curDataPtr->freeMemSize;
	   				break;						
				}
				else {
					status = FB_SPEC_MEM_EXCEED_ERR;
					setErrorCodeWithVal(	__FILE__, __LINE__,
											"fbSpecUnusedMemoryInfoSet",status,
											"Block Addr",blockAddr,
											"FB Code", fbMemInfo.fbId,
											"Free Mem Size",curDataPtr->freeMemSize
										 );
					goto MODULE_END;  
				}
    		}
    		
    		else if(fbMemInfo.specActionIndex > prevStartUnusedSpecMemIndex){
				curEndSpecMemIndex = fbMemInfo.specActionIndex + (usedMemSize/4) - 1;
				
				if(curEndSpecMemIndex < prevEndUnusedSpecMemIndex){
    					
    				
    				
    				saveEndIndex = curDataPtr->unUsedMemEndIndex;
					curDataPtr->unUsedMemEndIndex	= fbMemInfo.specActionIndex - 1;
					curDataPtr->freeMemSize			= (curDataPtr->unUsedMemEndIndex - curDataPtr->unUsedMemStartIndex + 1) * 4;     				
    				curDataPtr->usableMemSize 		= curDataPtr->freeMemSize;

    						 		
				   	tmpDataPtr = (strFbSpecUnusedMemInfo *)nodeGetAndUnlink (
                		           		(strDoubleLink **)&sFbSpecUnusedMemQMngrInfo.freeList
            	        				                            		);
					if(tmpDataPtr == NULL) {
						status = NO_EXIST_NODE_ERR;
				        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
						goto MODULE_END;	
					}
					
					tmpDataPtr->unUsedMemStartIndex			= fbMemInfo.specActionIndex + usedMemSize/4;

					tmpDataPtr->unUsedMemEndIndex			= saveEndIndex;
					tmpDataPtr->freeMemSize					= tmpDataPtr->unUsedMemEndIndex - tmpDataPtr->unUsedMemStartIndex + 1;
					tmpDataPtr->freeMemSize					*= 4; 
					tmpDataPtr->usableMemSize 				= tmpDataPtr->freeMemSize;

					
					status = nodePutOnLast( (strDoubleLink **) &sFbSpecUnusedMemQMngrInfo.usedList,
       		        								    		(strDoubleLink *)tmpDataPtr
           		       		  			  );
					if(status) {
				        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
						goto MODULE_END;	
					}
					
    				break;	
    			}
				else if(curEndSpecMemIndex == prevEndUnusedSpecMemIndex){
    					
    				
    				saveEndIndex = curDataPtr->unUsedMemEndIndex;
					curDataPtr->unUsedMemEndIndex		= fbMemInfo.specActionIndex - 1;
					curDataPtr->freeMemSize				= (curDataPtr->unUsedMemEndIndex - curDataPtr->unUsedMemStartIndex + 1) * 4;     				
			 		curDataPtr->usableMemSize 			= curDataPtr->freeMemSize;

 					
 					
    				break;	
  				}
				else if(curEndSpecMemIndex > prevEndUnusedSpecMemIndex){
    				goto NEXT_LOOP;	
				}
    		} 
NEXT_LOOP:
	        curDataPtr = (strFbSpecUnusedMemInfo *)nextNodeRef ( 	
	        						(strDoubleLink *)sFbSpecUnusedMemQMngrInfo.usedList,
	                        		(strDoubleLink *)curDataPtr		
	                        									);
    	} 
    	
		if(curDataPtr == NULL) {
			status = NO_EXIST_NODE_ERR;
	        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
			goto MODULE_END;	
		}
	} 

MODULE_END:
	
#ifdef VXWORKS
	semaphoreRelease (sFbSpecMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbSpecMemUnusedQSemaId);	
#endif

	return(status);
}


uint32 fbSpecUnusedMemoryInfoClear( uint32 moduleId)
{
	uint32	status = NO_ERROR;
	strFbSpecUnusedMemInfo	*curDataPtr;
		
	
	if( moduleId != LOGIC_ID){
		status = L_MODULE_ID_ERR;
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}	

	
	if(sFbSpecUnusedMemQMngrInfo.usedList == NULL) return(status);

	
    status = semaphoreGet(sFbSpecMemUnusedQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}

    while( sFbSpecUnusedMemQMngrInfo.usedList!= NULL) {
        
   		curDataPtr = (strFbSpecUnusedMemInfo *)nodeGetAndUnlink(
                    		(strDoubleLink **)&sFbSpecUnusedMemQMngrInfo.usedList
																);
        if(curDataPtr != NULL){
           	
           	memoryClear ( (uint8 *) curDataPtr, sizeof(strFbSpecUnusedMemInfo));
           	(void)nodePutOnLast( (strDoubleLink **) &sFbSpecUnusedMemQMngrInfo.freeList,
               	                 (strDoubleLink *)curDataPtr
                   	           );
        }
    }

	
#ifdef VXWORKS
	semaphoreRelease (sFbSpecMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbSpecMemUnusedQSemaId);	
#endif

	return(status);
}


static uint32	fbExecIdQueInit(void)
{
	uint32	status = NO_ERROR;
	uint32	loop;

    

    sFbExecQMngrInfo.freeList = NULL;
	
	for(loop = 0; loop < MAX_LOGIC_TASK_NUM; loop++)
    	sFbExecQMngrInfo.usedList[loop] = NULL;

    for( loop = 0; loop < MAX_FB_EXEC_SEQ_ID_NUM; loop++) {
		memoryClear (   (char *) &sFbExecDataInfo[loop],
                         sizeof(strFbExecutionInfo)
                    );
        status = nodePutOnLast	(	(strDoubleLink **) &sFbExecQMngrInfo.freeList,
                                	(strDoubleLink *)  &sFbExecDataInfo[loop]
                            	);
		if(status != NO_ERROR)
			break;                            
    }
	
	return(status);
}


uint32 fbExecIdInfoClear(uint32 taskId, uint32 moduleId)
{
	uint32	status = NO_ERROR;
	strFbExecutionInfo	*curDataPtr;
		
	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
		status = L_SEG_ID_ERR;
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}

	
	if( moduleId != LOGIC_ID){
		status = L_MODULE_ID_ERR;
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}	

	
	if(sFbExecQMngrInfo.usedList[taskId - 1] == NULL) return(status);

	
    status = semaphoreGet(gFbExecSeqIdQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}
	
    while( sFbExecQMngrInfo.usedList[taskId - 1]!= NULL) {
        
   		curDataPtr = (strFbExecutionInfo *)nodeGetAndUnlink(
                    				(strDoubleLink **)&sFbExecQMngrInfo.usedList[taskId - 1]
															);
       	if(curDataPtr != NULL){
			
			if( (curDataPtr->fbBlockAddr >0) && (curDataPtr->fbBlockAddr<= MAX_MPU_BLOCK_ADDR) )
				gFBExeSequenceQueuePtr[curDataPtr->fbBlockAddr -1] = NULL; 

           	
           	memoryClear( (uint8 *)curDataPtr, sizeof(strFbExecutionInfo));
           	(void)nodePutOnLast( (strDoubleLink **) &sFbExecQMngrInfo.freeList,
               	                 (strDoubleLink *)curDataPtr
                   	           );
        }
    }

	
#ifdef VXWORKS
	semaphoreRelease (gFbExecSeqIdQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gFbExecSeqIdQSemaId);	
#endif

	return(status);
}


static uint32 fbExecIdInfoDelete(uint32 taskId, uint32 fbId, uint32 fbBlkAddr)
{
	uint32	status = NO_ERROR;
	strFbExecutionInfo	*nodeDataPtr;
	uint32 delListNum,listLoop;
	uint32 delList[MAX_ONLINE_DWL_FB_CODE_DEL_NUM];

	status = semaphoreGet(gFbExecSeqIdQSemaId, NOWAIT);
	
	if(status != NO_ERROR) {
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}
	
	nodeDataPtr = sFbExecQMngrInfo.usedList[taskId - 1];

	if(nodeDataPtr == NULL) {
		status = NO_EXIST_NODE_ERR;
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		goto MODULE_END;		
	}
	
	delListNum = 0;
	while(nodeDataPtr != NULL) {
		if(nodeDataPtr->fbBlockAddr == fbBlkAddr){
			
			if(nodeDataPtr->fbId != fbId){
				status = L_FB_CODE_ERR;
				setErrorCodeWithVal(	__FILE__,__LINE__,"fbExecIdInfoDelete", status,
										"Req Blk Addr", fbBlkAddr,
										"Req Fb Code", fbId,
										"DB Fb Code", nodeDataPtr->fbId
									  );
				goto MODULE_END;	
			}
			delListNum += 1;
			delList[delListNum - 1] = (uint32)nodeDataPtr;
		}
		nodeDataPtr = (strFbExecutionInfo *)nextNodeRef ( 	
			        							(strDoubleLink *)sFbExecQMngrInfo.usedList[taskId - 1],
			                        			(strDoubleLink *)nodeDataPtr
										  						);
	}
	
	if(delListNum == 0) {
		status = L_FB_CODE_ERR;
		setErrorCodeWithVal(	__FILE__,__LINE__,"fbExecIdInfoDelete", status,
								"Req Blk Addr", fbBlkAddr,
								"Req Fb Code", fbId,
								"Req Seg ID", taskId
							  );
		goto MODULE_END;	
	}
	
	
	for(listLoop = 0; listLoop < delListNum; listLoop++){
   		status = nodeUnlink( 	
        					(strDoubleLink **)&sFbExecQMngrInfo.usedList[taskId - 1],
							(strDoubleLink *)delList[listLoop]
      						);
		                            		
		if(status != NO_ERROR){
	        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
			goto MODULE_END;	
		}
		
		status = nodePutOnLast( (strDoubleLink **) &sFbExecQMngrInfo.freeList,
       	   						(strDoubleLink *)delList[listLoop]
           		  			  );
		if(status != NO_ERROR){
	        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
			goto MODULE_END;	
		}		
	}
MODULE_END:
#ifdef VXWORKS
	semaphoreRelease (gFbExecSeqIdQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gFbExecSeqIdQSemaId);	
#endif

	return(status);
}



uint32 fbExecIdInfoSet(uint32 taskId, uint32 moduleId)
{
	uint32	status = NO_ERROR, blockAddr = 0;
	strFbTaskTblMemInfo	*segTblInfoPtr;
	strFbMemInfo 		fbMemInfo;
	strFbExecutionInfo	*curDataPtr, *nextDataPtr = NULL;
	strModuleExecInfo sysExeInfo;

	memoryClear( (uint8 *)&sysExeInfo, sizeof(strModuleExecInfo));
	status = fbSysExeModuleInfoRead(moduleId, &sysExeInfo);
	if(status!=NO_ERROR){
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}
	
	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
		status = L_SEG_ID_ERR;
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}

	
	if( moduleId != LOGIC_ID){
		status = L_MODULE_ID_ERR;
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}	

	status = segTblPointerGet(	LOGIC_ID,
           						taskId,
								(strFbTaskTblMemInfo	**) &segTblInfoPtr
							);
			
  	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);		
	}

	
	if( (segTblInfoPtr->logicType == SFC_LOGIC_TYPE) || (segTblInfoPtr->taskId != taskId) ) 
		return(status);

	if (sysExeInfo.curfbMaxBlockAddr >  MAX_MPU_BLOCK_ADDR){	
		status = L_FB_ADDR_ERR;
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);

	}

	
    status = semaphoreGet(gFbExecSeqIdQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}
	
	sPrevFbExecId = 0;

	for(blockAddr = 1;blockAddr <= sysExeInfo.curfbMaxBlockAddr; blockAddr++){
		
	    if( (status = fbCodeMemInfoRead	(
											LOGIC_ID,
           									blockAddr,
           									(strFbMemInfo *) &fbMemInfo
           								)
			) != NO_ERROR
		){
	        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
			goto MODULE_END;
		}

		if( fbMemInfo.fbId == 0x0) continue;
		if( fbMemInfo.fbTaskId != taskId) continue;
	
		
		if(	fbMemInfo.fbExecSequenceId == 0x0) {
			status = FB_SEQ_ID_NULL_ERR;
			setErrorCodeWithVal(	__FILE__,__LINE__,"fbExecIdInfoSet",status,
									"Fb Blk Addr",blockAddr,
									"Fb Code", fbMemInfo.fbId,
									"Seq Id",fbMemInfo.fbExecSequenceId
								  );  
			goto MODULE_END;
		}
		
		
		else if (fbMemInfo.fbExecSequenceId > MAX_FB_EXEC_SEQ_ID){
			status = FB_SEQ_ID_MAX_EXCEED_ERR;
			setErrorCodeWithVal(	__FILE__,__LINE__,"fbExecIdInfoSet",status,
									"Fb Blk Addr",blockAddr,
									"Fb Code", fbMemInfo.fbId,
									"Seq Id",fbMemInfo.fbExecSequenceId
								  );  
			goto MODULE_END;
		}
		
		
	    
	    curDataPtr = (strFbExecutionInfo *)nodeGetAndUnlink (
                            	(strDoubleLink **)&sFbExecQMngrInfo.freeList
            	                                    	);
		if(curDataPtr == NULL) {
			status = NO_EXIST_NODE_ERR;
	        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
			goto MODULE_END;	
		}
		else {
				
			curDataPtr->fbExecId 		= fbMemInfo.fbExecSequenceId;
			curDataPtr->taskId			= taskId;
			curDataPtr->fbId			= fbMemInfo.fbId;
			curDataPtr->fbBlockAddr		= blockAddr;

			
			gFBExeSequenceQueuePtr[blockAddr-1] = (strFbExecutionInfo*)curDataPtr;

			
			if(sysExeInfo.curfbMaxBlockAddr < blockAddr) 
				sysExeInfo.curfbMaxBlockAddr = blockAddr;

			if(fbMemInfo.fbExecSequenceId >= sPrevFbExecId){
				
				
				nextDataPtr = NULL;					
				
				
				
				
				sPrevFbExecId = fbMemInfo.fbExecSequenceId;
			}
			else if(fbMemInfo.fbExecSequenceId < sPrevFbExecId){
					
				
				
				nextDataPtr = sFbExecQMngrInfo.usedList[taskId - 1];
			
				while(nextDataPtr != NULL) {	
					
					
					if(curDataPtr->fbExecId < nextDataPtr->fbExecId)
						break;
					
			        nextDataPtr = (strFbExecutionInfo *)nextNodeRef ( 	
			        							(strDoubleLink *)sFbExecQMngrInfo.usedList[taskId - 1],
			                	        		(strDoubleLink *)nextDataPtr
											  					);
				} 
			}

			
			
			
			if((nextDataPtr == NULL)) {
				
    			status = nodePutOnLast( (strDoubleLink **) &sFbExecQMngrInfo.usedList[taskId - 1],
             			            	(strDoubleLink *)curDataPtr
                			       	  );
				if(status) {
			        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
					goto MODULE_END;
				}						
			} 
			else {
				
				status = nodePutOnPrev	(	
								(strDoubleLink **)&sFbExecQMngrInfo.usedList[taskId - 1],
								(strDoubleLink *)nextDataPtr,
								(strDoubleLink *)curDataPtr
										);
				if(status) {
			        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
					goto MODULE_END;
				}						
			} 
		} 	
	} 		
	
	status = fbSysExeModuleInfoWrite(moduleId, &sysExeInfo);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
	}

MODULE_END:
	
#ifdef VXWORKS
	semaphoreRelease (gFbExecSeqIdQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gFbExecSeqIdQSemaId);	
#endif

	return(status);
}	


strFbExecutionInfo *fbExecIdStartMemInfoGet(uint32 taskId)
{
	if( (taskId == 0) || (taskId >MAX_LOGIC_TASK_NUM) )
		return(NULL);
	else 
	    return((strFbExecutionInfo *)sFbExecQMngrInfo.usedList[taskId - 1]);	
}


strFbExecutionInfo *nonCyclicFbExecIdStartMemInfoGet(uint32 taskId, uint32 fbBlockAddr)
{
    strFbExecutionInfo *curNodePtr = NULL;
	strFbMemInfo	fbMemInfo;

	if( (taskId == 0) || (taskId >MAX_LOGIC_TASK_NUM) )
		return(NULL);

    curNodePtr = sFbExecQMngrInfo.usedList[taskId - 1];
    
   
   
    fbCodeMemInfoRead(LOGIC_ID,fbBlockAddr,(strFbMemInfo *) &fbMemInfo);
    
    while(curNodePtr != NULL) {
		if(curNodePtr->fbBlockAddr == fbBlockAddr)
			break;
     	curNodePtr = (strFbExecutionInfo *)nextNodeRef (	
     									(strDoubleLink *)sFbExecQMngrInfo.usedList[taskId - 1],
                                		(strDoubleLink *)curNodePtr
									  					);	
	}
	return(curNodePtr);
}


strFbExecutionInfo *fbExecIdNextMemInfoGet(uint32 taskId, strFbExecutionInfo *dataPtr)
{
	strDoubleLink *head_ptr = (strDoubleLink *)sFbExecQMngrInfo.usedList[taskId - 1];
    strDoubleLink *cur_node  = (strDoubleLink *)dataPtr;
    strDoubleLink *next_node = NULL;

    if( (head_ptr == NULL) || (cur_node == NULL))
        next_node = NULL;
    else if(cur_node->next != head_ptr)
        next_node = cur_node->next;
    else 
        next_node = NULL;

    return((strFbExecutionInfo *)next_node);
}



void onlineDwlVariableClear(void)
{
	uint32	status = NO_ERROR;

	sOnlineDwlFbAddInfo.dwlCurFrameNum 		= 0;
	sOnlineDwlFbAddInfo.dwlTotalFbCodeNum 	= 0;
	sOnlineDwlFbAddInfo.dwlTotalFrameNum 	= 0;
	sOnlineDwlFbAddInfo.numOfTotalFbCode 	= 0;
	sOnlineDwlFbAddInfo.numOfTotalSegment 	= 0;

	memoryClear( (uint8 *)sOnlineDwlFbAddInfo.addSegInfoPtr,
					 sizeof(strOnlineDwlAddSegInfo)*MAX_ONLINE_DWL_SEG_ADD_NUM);

	sOnlineDwlFbDelInfo.dwlCurFrameNum 		= 0;
	sOnlineDwlFbDelInfo.dwlTotalFbCodeNum 	= 0;
	sOnlineDwlFbDelInfo.dwlTotalFrameNum 	= 0;
	sOnlineDwlFbDelInfo.numOfTotalFbCode 	= 0;
	sOnlineDwlFbDelInfo.numOfTotalSegment 	= 0;
	
	memoryClear( (uint8 *)sOnlineDwlFbDelInfo.delSegInfoPtr,
					 sizeof(strOnlineDwlDelSegInfo)*MAX_ONLINE_DWL_SEG_DEL_NUM);

	sOnlineDwlFbSpecChgInfo.dwlCurFrameNum 		= 0;
	sOnlineDwlFbSpecChgInfo.dwlTotalFbCodeNum 	= 0;
	sOnlineDwlFbSpecChgInfo.dwlTotalFrameNum 	= 0;
	sOnlineDwlFbSpecChgInfo.numOfTotalFbCode 	= 0;
	sOnlineDwlFbSpecChgInfo.numOfTotalSegment 	= 0;
	memoryClear( (uint8 *)sOnlineDwlFbSpecChgInfo.specChgSegInfoPtr,
					 sizeof(strOnlineDwlSpecChgSegInfo)*MAX_ONLINE_DWL_SEG_SPEC_CHANGE_NUM);

	memoryClear( (uint8 *)&sOnlineDwlFbSeqIdAddAndChgInfo, sizeof(strOnlineDwlFbSeqIdAddAndChgMngrInfo));

	memoryClear( (uint8 *) &sOnlineDwlFbDelMemInfo, sizeof(strOnlineDwlDelFbMemMngrInfo));

	sOnlineDwlFbInfoChgInfo.dwlCurFrameNum 		= 0;
	sOnlineDwlFbInfoChgInfo.dwlTotalFbCodeNum 	= 0;
	sOnlineDwlFbInfoChgInfo.dwlTotalFrameNum 	= 0;
	sOnlineDwlFbInfoChgInfo.numOfTotalFbCode 	= 0;
	sOnlineDwlFbInfoChgInfo.numOfTotalSegment 	= 0;
	
	memoryClear( (uint8 *)sOnlineDwlFbInfoChgInfo.fbInfoChgSegInfoPtr,
					 sizeof(strOnlineDwlFbInfoChgSegInfo)*MAX_ONLINE_DWL_SEG_FBINFO_CHANGE_NUM);

	sOnlineDwlTrsChgInfo.dwlCurFrameNum 	= 0;
	sOnlineDwlTrsChgInfo.dwlTotalFrameNum 	= 0;
	sOnlineDwlTrsChgInfo.dwlTotalTrsNum 	= 0;
	sOnlineDwlTrsChgInfo.numOfTotalSegment 	= 0;
	sOnlineDwlTrsChgInfo.numOfTotalTrs 		= 0;
	memoryClear( (uint8 *)sOnlineDwlTrsChgInfo.chgTrsInfoPtr,
					 sizeof(strOnlineDwlChgTrsSegInfo)*MAX_ONLINE_DWL_SEG_TRS_CHANGE_NUM);

															
	
	
	
	
	fbSpecUsableMemSet();
	fbOutputUsableMemSet();

	
	status = onlineDwlTaskQueClear();
	if(status != NO_ERROR) 
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		
	return;
}


uint32 onlineDwlFbAddVariableClear(void)
{
	uint32	status = NO_ERROR;
	
	sOnlineDwlFbAddInfo.dwlCurFrameNum 		= 0;
	sOnlineDwlFbAddInfo.dwlTotalFbCodeNum 	= 0;
	sOnlineDwlFbAddInfo.dwlTotalFrameNum 	= 0;
	sOnlineDwlFbAddInfo.numOfTotalFbCode 	= 0;
	sOnlineDwlFbAddInfo.numOfTotalSegment 	= 0;
	memoryClear( (uint8 *)sOnlineDwlFbAddInfo.addSegInfoPtr,
					 sizeof(strOnlineDwlAddSegInfo)*MAX_ONLINE_DWL_SEG_ADD_NUM);


	
	
	
	
	fbSpecUsableMemSet();
	fbOutputUsableMemSet();
	
	
	status = onlineDwlTaskReqQueClear(ONLINE_DWL_FBCODE_ADD);
	if(status != NO_ERROR) 
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		
	return(status);
}


uint32 onlineDwlFbDelVariableClear(void)
{
	uint32	status = NO_ERROR;
	
	sOnlineDwlFbDelInfo.dwlCurFrameNum 		= 0;
	sOnlineDwlFbDelInfo.dwlTotalFbCodeNum 	= 0;
	sOnlineDwlFbDelInfo.dwlTotalFrameNum 	= 0;
	sOnlineDwlFbDelInfo.numOfTotalFbCode 	= 0;
	sOnlineDwlFbDelInfo.numOfTotalSegment 	= 0;
	memoryClear( (uint8 *)sOnlineDwlFbDelInfo.delSegInfoPtr,
					 sizeof(strOnlineDwlDelSegInfo)*MAX_ONLINE_DWL_SEG_DEL_NUM);


	memoryClear( (uint8 *) &sOnlineDwlFbDelMemInfo, sizeof(strOnlineDwlDelFbMemMngrInfo));

	
	status = onlineDwlTaskReqQueClear(ONLINE_DWL_FBCODE_DEL);
	if(status != NO_ERROR) 
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		
	return(status);
}


uint32 onlineDwlFbSpecChgVariableClear(void)
{
	uint32	status = NO_ERROR;
	
	sOnlineDwlFbSpecChgInfo.dwlCurFrameNum 		= 0;
	sOnlineDwlFbSpecChgInfo.dwlTotalFbCodeNum 	= 0;
	sOnlineDwlFbSpecChgInfo.dwlTotalFrameNum 	= 0;
	sOnlineDwlFbSpecChgInfo.numOfTotalFbCode 	= 0;
	sOnlineDwlFbSpecChgInfo.numOfTotalSegment 	= 0;
	memoryClear( (uint8 *)sOnlineDwlFbSpecChgInfo.specChgSegInfoPtr,
					 sizeof(strOnlineDwlSpecChgSegInfo)*MAX_ONLINE_DWL_SEG_SPEC_CHANGE_NUM);

	
	status = onlineDwlTaskReqQueClear(ONLINE_DWL_FBCODE_SPEC_CHANGE);
	if(status != NO_ERROR) 
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		
	return(status);
}


uint32 onlineDwlFbSeqIdAddAndChgVariableClear(void)
{
	uint32	status = NO_ERROR;
	
	memoryClear( (uint8 *)&sOnlineDwlFbSeqIdAddAndChgInfo, sizeof(strOnlineDwlFbSeqIdAddAndChgMngrInfo));

	
	status = onlineDwlTaskReqQueClear(ONLINE_DWL_FBCODE_SEQ_ID_ADD_AND_CHG);
	if(status != NO_ERROR) 
        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		
	return(status);
}


uint32 onlineDwlFbCodeAddFuncInComTask(	uint32 	*reqDataPtr, 
										uint32	*respDataPtr,
										uint32	*respRetLenPtr
							          )
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32	numOfSegment, numOfTotalSegment; 
	uint32 numOfAddFbCode = 0, numOfTotalAddFbCode;
	uint32 segmentId,fbAddr,fbId, sumOfFbCodeNum;	
	uint32	totalFrameNum, curFrameId, checkLoop,prevNumOfFbCode;
	uint32	*saveReqDataPtr;
	strFbTaskTblMemInfo	segTblInfo;
	uint32	maxSegId, loop, fbAddLoop, segAddLoop,fbSpecLoop;
	uint32	sumOfFbCodeSpecMemoryNum, numOfSpec, sizeofSpec;
	strOnlineDwlAddFbCodeMngrInfo	*onlineFbAddMngrInfoPtr = &sOnlineDwlFbAddInfo;
	strOnlineDwlAddSegInfo			*onlineSegAddInfoPtr = onlineFbAddMngrInfoPtr->addSegInfoPtr;
	strOnlineDwlAddFbCodeInfo		*onlineFbAddInfoPtr;

	uint32 sumOfFbCodeOutputMemoryNum = 0;
	uint32 taskLogicType = 0, numOftotalActTrs, numOfActTrs = 1, sfcAddLoop =0; 
	uint32 usingFbType, actTrsId;
	uint32 outputAction[2];
	uint32 anyFuncOutDataSize , anyFuncOutDataType;
	
	uint32 outputNegate[2], specData;
	strFbCodeTypePointerInfo typePInfo;
	uint32* paraTypePtr, *tempPtr;
	strFbSpecActionInfo *specActionParaTypeDefPtr;
	uint32	fbParaTypeDef;

	memoryClear( (uint8 *)&typePInfo, sizeof(strFbCodeTypePointerInfo));

	*respRetLenPtr	= 0;

	
	totalFrameNum	= (*reqDataPtr & 0xffff0000) >> 16;
	curFrameId 	= (*reqDataPtr++ & 0xffff);
	
	
	saveReqDataPtr 	= reqDataPtr;
	
	
	
	numOfTotalSegment	= (*reqDataPtr & 0xffff0000) >> 16;
	numOfSegment 		= (*reqDataPtr++ & 0xffff);

	
	*respDataPtr++ 	= numOfSegment;
	*respRetLenPtr 	+= 4;

		
    if( (numOfTotalSegment == 0) || (numOfTotalSegment > MAX_ONLINE_DWL_SEG_ADD_NUM)){
		status = ONLINE_DWL_SEG_NUM_ERR;
       	setErrorCodeWithVal( 	__FILE__,
       							__LINE__,
               	        		"onlineDwlFbCodeAddFuncInComTask",
                   	    		status,
                   	    		"Total Segment ADD Number",numOfTotalSegment,
                   	    		"MAX Segment ADD Number", MAX_ONLINE_DWL_SEG_ADD_NUM,
                   	    		"NOT Using", 0 
				  	  		);
		goto MODULE_END;
	}
	
    else if( (numOfSegment == 0) || (numOfSegment > MAX_ONLINE_DWL_SEG_ADD_NUM)){
		status = ONLINE_DWL_SEG_NUM_ERR;
       	setErrorCodeWithVal( 	__FILE__,
       							__LINE__,
               	        		"onlineDwlFbCodeAddFuncInComTask",
                   	    		status,
                   	    		"Total Fb Code ADD Number",numOfSegment,
                   	    		"Prev FbCode Add Numver", onlineFbAddMngrInfoPtr->numOfTotalFbCode,
                   	    		"MAX FB Code ADD Number", MAX_ONLINE_DWL_SEG_ADD_NUM
				  	  		);
		goto MODULE_END;
	}

	
	numOfTotalAddFbCode = *reqDataPtr++;
	
	if(	(numOfTotalAddFbCode > MAX_ONLINE_DWL_FB_CODE_ADD_NUM)||
		(numOfTotalAddFbCode == 0x0)
	){
		status = FB_NUM_EXCEED_ERR;
    	setErrorCodeWithVal( 	__FILE__,
    							__LINE__,
           		        		"onlineDwlFbCodeAddFuncInComTask",
               		    		status,
               	    			"Total FB Add  Number", numOfTotalAddFbCode,
               	    			"MAX FB Code ADD Number", MAX_ONLINE_DWL_FB_CODE_ADD_NUM,
               	    			"NOT Using",0
			  	  			);
		goto MODULE_END;
	}		
	
	
	if(curFrameId == 0x1) {
		onlineFbAddMngrInfoPtr->numOfTotalSegment 	= numOfTotalSegment;
		
		
		onlineFbAddMngrInfoPtr->dwlTotalFbCodeNum 	= numOfTotalAddFbCode;	
		onlineFbAddMngrInfoPtr->dwlTotalFrameNum 	= totalFrameNum;	
	}
	
	onlineFbAddMngrInfoPtr->dwlCurFrameNum 	= curFrameId;	
	
	sumOfFbCodeNum = 0;
	
	for(segAddLoop = 0; segAddLoop < numOfSegment; segAddLoop++) {
		
		taskLogicType   = (*reqDataPtr>> 31) & 0x1;	 
		segmentId 		= *reqDataPtr++ & 0xffff;

		if((segmentId == 0) || (segmentId > MAX_LOGIC_TASK_NUM)) {
			status = L_SEG_ID_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        		"onlineDwlFbCodeAddFuncInComTask",status,
           	        	    		"Segment ID",segmentId,
           	        	    		"MAX Segment ID", MAX_LOGIC_TASK_NUM,
           	        	    		"NOT Using", 0 
					  	  		);
			goto MODULE_END;
		}

		
		maxSegId = 0;
		for(loop = 1; loop <= MAX_LOGIC_TASK_NUM; loop++) {
			memoryClear( (uint8 *)&segTblInfo, sizeof(strFbTaskTblMemInfo));
		
   			(void)getFbTaskTblInfo( 
								LOGIC_ID,
           						loop,
								(strFbTaskTblMemInfo	*) &segTblInfo
								);
			if(segTblInfo.taskId > maxSegId)
				maxSegId = segTblInfo.taskId;
			
			if(segTblInfo.taskId == segmentId) break;
		}

		if((segTblInfo.taskId == 0) || (loop > MAX_LOGIC_TASK_NUM)){
			status = ONLINE_DWL_SEG_EXIST_ERR;
       		setErrorCodeWithVal( 	__FILE__,
       								__LINE__,
           	    	        		"onlineDwlFbCodeAddFuncInComTask",
              	     	    		status,
                	   	    		"Segment ID",segmentId,
                 	  	    		"Current Max Seg Id", maxSegId,
                  	 	    		"NOT Using", 0 
				  		  		);
			goto MODULE_END;		
		}

		if(segTblInfo.logicType != taskLogicType) {
			status = L_LOGIC_ID_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        		"onlineDwlFbCodeAddFuncInComTask",status,
									"Segment ID",segmentId,
           	        	    		"Curr Logic Type",segTblInfo.logicType,
           	        	    		"Recv Logic Type",taskLogicType
					  	  		);
			goto MODULE_END;
		}
		
		
		if(taskLogicType == SFC_LOGIC_TYPE){
			numOftotalActTrs	= (*reqDataPtr & 0xffff0000) >> 16;	 
			numOfActTrs			= *reqDataPtr++ & 0xffff;
		}
		else {
			numOftotalActTrs	= 0;	 
			numOfActTrs			= 1;
			reqDataPtr++;
		}

		for(sfcAddLoop = 0; sfcAddLoop < numOfActTrs; sfcAddLoop++) {
			
			if(taskLogicType == SFC_LOGIC_TYPE){
				usingFbType = (*reqDataPtr & 0xffff0000) >> 16;	 
				actTrsId =  *reqDataPtr++ & 0xffff;
			
				
				if(usingFbType == TRANSITION_TYPE) {
					if(actTrsId==0 ||actTrsId>MAX_TRANSITOIN_NUM){
						status = L_TRANSITION_ID_ERR;
       					setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        					"onlineDwlFbCodeAddFuncInComTask",status,
           	        	    					"Cur TRANSITION ID",actTrsId,
           	        	    					"MAX TRANSITION ID", MAX_TRANSITOIN_NUM ,
           	        	    					"NOT Using", 0 
					  	  					);
						goto MODULE_END;
					}
				}
				
				else if(usingFbType == ACTION_TYPE) {
					if(actTrsId==0 ||actTrsId>MAX_ACTION_NUM){
						status = L_TRANSITION_ID_ERR;
       					setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        					"onlineDwlFbCodeAddFuncInComTask",status,
           	        	    					"Cur ACTION ID",actTrsId,
           	        	    					"MAX ACTION ID", MAX_ACTION_NUM ,
           	        	    					"NOT Using", 0 
					  	  					);
						goto MODULE_END;
					}
				}
				else{
					status = SFC_TYPE_ID_ERR;
       				setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        				"onlineDwlFbCodeAddFuncInComTask",status,
           	        	    				"Cur SFC FB Type",usingFbType,
           	        	    				"NOT Using", 0 ,
           	        	    				"NOT Using", 0 
					  	  				);
					goto MODULE_END;
				}
				
			}
 			else reqDataPtr++;	

			
			numOfTotalAddFbCode	= (*reqDataPtr & 0xffff0000) >> 16;	 
			numOfAddFbCode		= *reqDataPtr++ & 0xffff;
			sumOfFbCodeNum		+= 	numOfAddFbCode;
			
			
			if( (numOfTotalAddFbCode > MAX_ONLINE_DWL_FB_CODE_ADD_NUM) ||
				(numOfAddFbCode > MAX_ONLINE_DWL_FB_CODE_ADD_NUM) ||
				(numOfTotalAddFbCode < numOfAddFbCode) ||
				(numOfTotalAddFbCode == 0x0) ||
				(numOfAddFbCode == 0x0)
			) {
				status = FB_NUM_EXCEED_ERR;
    			setErrorCodeWithVal( 	__FILE__,
    									__LINE__,
           			        			"onlineDwlFbCodeAddFuncInComTask",status,
               	    					"Total FB Add  Number", numOfTotalAddFbCode,
               	    					"Fb Add Number",numOfAddFbCode,
               	    					"MAX FB Code ADD Number", MAX_ONLINE_DWL_FB_CODE_ADD_NUM
			  	  					);
				goto MODULE_END;
			}

		
			sumOfFbCodeSpecMemoryNum = 0;
			
			for(fbAddLoop = 0; fbAddLoop < numOfAddFbCode; fbAddLoop++) {
				
				fbAddr = (*reqDataPtr >> 16) & 0xffff;
				fbId = *reqDataPtr++ & 0xffff;	
				
				
				anyFuncOutDataSize = (*reqDataPtr >> 16)&0xffff;
				anyFuncOutDataType = *reqDataPtr++& 0xffff;

				
				memoryCopy(((uint8*)&outputAction[0]),(uint8*)reqDataPtr++, sizeof(uint64));
				reqDataPtr++ ;

				
				if(gFbCodeSortingTableInfo[fbId].outputNo < 32  ) {
					if((outputAction[0] >> gFbCodeSortingTableInfo[fbId].outputNo)!= 0x0 ) break;				
				}
				else{
					if((outputAction[1] >> (gFbCodeSortingTableInfo[fbId].outputNo-32))!= 0x0 ) break;	
				}
				
				numOfSpec = (*reqDataPtr>> 16) & 0xffff;
				sizeofSpec = *reqDataPtr++& 0xffff;
				
				
				reqDataPtr += numOfSpec;
				reqDataPtr += sizeofSpec;
				
				
				if( (fbAddr == 0) || (fbAddr > MAX_MPU_BLOCK_ADDR) )
				{
					status = SEG_BLK_ADDR_EXCEED_ERR;
	       			setErrorCodeWithVal( 	__FILE__,__LINE__,
        	   	    	        			"onlineDwlFbCodeAddFuncInComTask",status,
                		   	    			"FB Addr",fbAddr,
                 		  	    			"Start FB Addr", 0,
                  	 		    			"End Fb Addr", MAX_MPU_BLOCK_ADDR 
				  		  				);
								  		  				
				}   
				
				else if(fbId > MAX_FB_CODE_ID){
					status = L_FB_CODE_ERR;
	       			setErrorCodeWithVal( 	__FILE__,__LINE__,
        	   	    	        			"onlineDwlFbCodeAddFuncInComTask",status,
                		   	    			"FB Code",fbId,
                 		  	    			"Max FB Code ", MAX_FB_CODE_ID,
                  	 		    			"Not Using", 0 
				  		  				);
				}				
				
				else if( fbId != gFbCodeSortingTableInfo[fbId].fbId){
					status = L_FB_CODE_ERR;
	       			setErrorCodeWithVal( 	__FILE__,__LINE__,
        	   	    	        			"onlineDwlFbCodeAddFuncInComTask",	status,
                		   	    			"FB Code",fbId,
                 		  	    			"Defined Fb Code ", gFbCodeSortingTableInfo[fbId].fbId,
                  	 		    			"Not Using", 0 
				  		  				);
				}	
				
				else if(IS_ANY_TYPE_FB_CODE(fbId)) {
					
					if( (anyFuncOutDataSize < 1) || (anyFuncOutDataSize > 5)){
						status = ANY_FUNC_OUT_SIZE_ERR;
						setErrorCodeWithVal(	(uint8 *)__FILE__, (uint32)__LINE__, 
												(uint8 *)"onlineDwlFbCodeAddFuncInComTask", status,
												"Block Addr", fbAddr,
												"FB Code Id",fbId,
												"Out Data Size", anyFuncOutDataSize
			  								);
						break;			
					}

					
					if( (anyFuncOutDataType < 1) || (anyFuncOutDataType > 10)){
						status = ANY_FUNC_OUT_TYPE_ERR;
						setErrorCodeWithVal(	(uint8 *)__FILE__, (uint32)__LINE__, 
												(uint8 *)"onlineDwlFbCodeAddFuncInComTask", status,
												"Block Addr", fbAddr,
												"FB Code Id",fbId,
												"Out Data Type", anyFuncOutDataType
			  					);
						break;			
					}
				}
			
				
				else if( numOfSpec != gFbCodeSortingTableInfo[fbId].inputNo){
					status = L_PARA_NO_ERR;
	       			setErrorCodeWithVal( 	__FILE__,__LINE__,
        	   	    	        			"onlineDwlFbCodeAddFuncInComTask",status,
                		   	    			"Req Spec Num",numOfSpec,
                 		  	    			"Defined Spec Number ", gFbCodeSortingTableInfo[fbId].inputNo,
                  	 		    			"Not Using", 0 
				  		  				);
				}	
									
				else if( numOfSpec > MAX_FB_INPUT_HEAD_PERMISSION_NUM){
					status = L_PARA_NO_ERR;
	       			setErrorCodeWithVal( 	__FILE__,
    	   									__LINE__,
        	   	    	        			"onlineDwlFbCodeAddFuncInComTask",
            	  	     	    			status,
                		   	    			"Req Spec Num",numOfSpec,
                 		  	    			"Max Spec Number ", MAX_FB_INPUT_HEAD_PERMISSION_NUM,
                  	 		    			"Not Using", 0 
				  		  				);
				}	
				
				
				else if( sizeofSpec != gFbCodeSortingTableInfo[fbId].inputSize){
					status = L_PARA_NO_ERR;
	       			setErrorCodeWithVal( 	__FILE__,__LINE__,
        	   	    	        			"onlineDwlFbCodeAddFuncInComTask",status,
                		   	    			"Req Spec Size",sizeofSpec,
                 		  	    			"Defined Spec Number ", gFbCodeSortingTableInfo[fbId].inputSize,
                  	 		    			"Not Using", 0 
				  		  				);
				}	
									
				else if((numOfSpec > MAX_FB_INPUT_DATA_PERMISSION_NUM)){
					status = L_PARA_NO_ERR;
	       			setErrorCodeWithVal( 	__FILE__,
    	   									__LINE__,
        	   	    	        			"onlineDwlFbCodeAddFuncInComTask",status,
                		   	    			"Req Spec Size",sizeofSpec,
                 		  	    			"Max Spec Size", MAX_FB_INPUT_DATA_PERMISSION_NUM,
                  	 		    			"Not Using", 0 
				  		  				);
				}				
				
				
				if(status == NO_ERROR) {
					sumOfFbCodeSpecMemoryNum = sizeofSpec + gFbCodeSortingTableInfo[fbId].intlVarSize
								+gFbCodeSortingTableInfo[fbId].inputNo;
		
					
					
					
					retVal = fbSpecMemorySpaceCheck(sumOfFbCodeSpecMemoryNum);

					if(retVal != NO_ERROR) {
						status = retVal;
				        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
					} 			
				}
			
				
				if(status == NO_ERROR) {
					sumOfFbCodeOutputMemoryNum = gFbCodeSortingTableInfo[fbId].outputSize;
		
					
					
					
					retVal = fbOutputMemorySpaceCheck(sumOfFbCodeOutputMemoryNum);

					if(retVal != NO_ERROR) {
						status = retVal;
				        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
					} 			
				}

				
				if(status != NO_ERROR) goto MAKE_RESP;	
			
			}	

		} 
		
		
			
MAKE_RESP:
		*respDataPtr 	= (status & 0xffff) << 16;
		*respDataPtr++	|= segmentId;
		*respRetLenPtr 	+= 4;	
		
		if(status != NO_ERROR) goto MODULE_END;
		
	} 
	
	
	if(status != NO_ERROR)
		goto MODULE_END;
	

	
	onlineFbAddMngrInfoPtr->numOfTotalFbCode 	+= sumOfFbCodeNum;

	
	if(totalFrameNum == curFrameId) {
		 if((onlineFbAddMngrInfoPtr->numOfTotalFbCode > MAX_ONLINE_DWL_FB_CODE_ADD_NUM)||
		 	(onlineFbAddMngrInfoPtr->numOfTotalFbCode == 0x0)||
			(onlineFbAddMngrInfoPtr->dwlTotalFbCodeNum != onlineFbAddMngrInfoPtr->numOfTotalFbCode)
		 ){
			status = FB_NUM_EXCEED_ERR;
       		setErrorCodeWithVal( 	__FILE__,
       								__LINE__,
               		        		"onlineDwlFbCodeAddFuncInComTask",
                   		    		status,
                   	    			"SUM Fb Code ADD Number",sumOfFbCodeNum,
                   	    			"Total FB Add  Number", onlineFbAddMngrInfoPtr->numOfTotalFbCode,
                   	    			"Total FB Set  Number", onlineFbAddMngrInfoPtr->dwlTotalFbCodeNum
				  	  			);
			goto MODULE_END;
		}		
	}
		
		
	
	reqDataPtr = saveReqDataPtr;

	
	numOfSegment = *reqDataPtr++&0xffff;

	
	numOfTotalAddFbCode = *reqDataPtr++;
	
	for(segAddLoop = 0; segAddLoop < numOfSegment; segAddLoop++){
		
		
		taskLogicType   = (*reqDataPtr>> 31) & 0x1;	 
		segmentId 		= *reqDataPtr++& 0xffff;
		
		
		for(checkLoop = 0; checkLoop < MAX_ONLINE_DWL_SEG_ADD_NUM; checkLoop++) {
			if((onlineSegAddInfoPtr->taskId == 0) || (onlineSegAddInfoPtr->taskId == segmentId))
				break;
			
			onlineSegAddInfoPtr++;	
		}
		if(checkLoop == MAX_ONLINE_DWL_SEG_ADD_NUM) {
			status = FB_ONLINE_DOWNLOAD_ERR;
	        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
			goto MODULE_END;	
		}
		
		if(taskLogicType==SFC_LOGIC_TYPE){
			
			numOftotalActTrs	= (*reqDataPtr & 0xffff0000) >> 16;	 
			numOfActTrs			= *reqDataPtr++ & 0xffff;
		}
		else {
			numOftotalActTrs	= 0;	 
			numOfActTrs			= 1;
			reqDataPtr++;	
		}

		onlineSegAddInfoPtr->taskId 			= segmentId;
		onlineSegAddInfoPtr->logicType		= taskLogicType;

		for(sfcAddLoop = 0; sfcAddLoop < numOfActTrs; sfcAddLoop++)
		{
			if(taskLogicType==SFC_LOGIC_TYPE){
				usingFbType = (*reqDataPtr & 0xffff0000) >> 16;	 
				actTrsId =  *reqDataPtr++ & 0xffff;
			}
			else {
				usingFbType = 0;
				actTrsId = 0;
				reqDataPtr++;	
			}
			numOfAddFbCode	= *reqDataPtr++ & 0xffff;
			prevNumOfFbCode = onlineSegAddInfoPtr->numOfFbCode;

			onlineSegAddInfoPtr->numOfFbCode 	+= numOfAddFbCode;	


			if(onlineSegAddInfoPtr->numOfFbCode  > MAX_ONLINE_DWL_FB_CODE_ADD_NUM) {
				status = FB_NUM_EXCEED_ERR;
		        setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
				goto MODULE_END;	
			}

			
			
			onlineFbAddInfoPtr = onlineSegAddInfoPtr->addFbCodeInfo + prevNumOfFbCode;
			
			for(fbAddLoop = 0; fbAddLoop < numOfAddFbCode; fbAddLoop++,onlineFbAddInfoPtr++){
				onlineFbAddInfoPtr->taskId =  segmentId;
				
				fbAddr = (*reqDataPtr >> 16) & 0xffff;
				fbId = *reqDataPtr++ & 0xffff;	

				
				anyFuncOutDataSize = (*reqDataPtr >> 16)&0xffff;
				anyFuncOutDataType = *reqDataPtr++& 0xffff;

				
				memoryCopy((uint8*)&outputNegate[0],(uint8*)reqDataPtr++,LONG_SIZE);
				reqDataPtr++;
					
				memoryCopy(((uint8*)&onlineFbAddInfoPtr->outputAction[0]),
							(uint8*)&outputNegate[0],LONG_SIZE);

				
				numOfSpec =(*reqDataPtr >> 16) & 0xffff;
				sizeofSpec = *reqDataPtr++ & 0xffff;	

				
				onlineFbAddInfoPtr->fbBlkAddr 	= (uint32)fbAddr;
				onlineFbAddInfoPtr->fbId		= (uint32)fbId;
				onlineFbAddInfoPtr->numOfSpec 	= numOfSpec;
				onlineFbAddInfoPtr->numOfSize	= sizeofSpec;

				onlineFbAddInfoPtr->paraRunningType  = (anyFuncOutDataType&0xF) | ((anyFuncOutDataSize&0xF)<<12);

				
				onlineFbAddInfoPtr->logicType	= taskLogicType;
				onlineFbAddInfoPtr->usingType	= usingFbType;
				if(onlineFbAddInfoPtr->usingType == TRANSITION_TYPE) {
					onlineFbAddInfoPtr->ActionID	= 0;
					onlineFbAddInfoPtr->TransitionID= actTrsId;
				}
				else if(onlineFbAddInfoPtr->usingType == ACTION_TYPE) {
					onlineFbAddInfoPtr->ActionID	= actTrsId;
					onlineFbAddInfoPtr->TransitionID= 0;
				}
				else {
					onlineFbAddInfoPtr->ActionID	= 0;
					onlineFbAddInfoPtr->TransitionID= 0;
				}

				if(numOfSpec+sizeofSpec > MAX_FB_INPUT_PERMISSION_NUM) {
					status = L_PARA_NO_ERR;
					setErrorCodeWithVal(__FILE__, __LINE__,
										"onlineDwlFbCodeAddFuncInComTask", status,
										"Input Header Numr",numOfSpec ,
										"Input Data Num",sizeofSpec ,
										"MAX Input Num",MAX_FB_INPUT_PERMISSION_NUM);
					goto MODULE_END;	
				}

				
				retVal= fbCodeParaTypePointerGet(onlineFbAddInfoPtr->fbId, 
												(strFbCodeTypePointerInfo *)&typePInfo);
				if(retVal != NO_ERROR) 
				{
			        setErrorCode( __FILE__, __LINE__, __FUNCTION__,retVal);
					goto MODULE_END;
				} else {
					paraTypePtr = (uint32 *)typePInfo.specTypeStartAddr;
				}


				
				for(fbSpecLoop = 0; fbSpecLoop < numOfSpec; fbSpecLoop++)
					onlineFbAddInfoPtr->specData[fbSpecLoop] = *reqDataPtr++;
				
				tempPtr = &onlineFbAddInfoPtr->specData[numOfSpec];
				for(fbSpecLoop = 0; fbSpecLoop < numOfSpec; fbSpecLoop++) {
					specActionParaTypeDefPtr = (strFbSpecActionInfo *)&onlineFbAddInfoPtr->specData[fbSpecLoop];
					fbParaTypeDef = *(uint32 *)(paraTypePtr + fbSpecLoop);

					
					specData = *reqDataPtr++;
					memoryCopy ( (uint8 *) tempPtr,(uint8 *) &specData, 4 );
					tempPtr++;
					
					if((GET_TYPE_SIZE(fbParaTypeDef)==SIZE64_TYPE) && 
						
						(GET_BASIC_SIZE(specActionParaTypeDefPtr->dataType) == SIZE64_TYPE)
					) {
						
						specData = *reqDataPtr++;
						memoryCopy ( (uint8 *) tempPtr,(uint8 *) &specData, 4 );

						if(specActionParaTypeDefPtr->refType == SPEC_TYPE_BYVAL)
							mySwap((uint32*)(tempPtr-1),(uint32*)tempPtr,DOUBLE_SIZE);
						tempPtr++;
					}
					else if((GET_TYPE_SIZE(fbParaTypeDef)==SIZE64_TYPE) && 
						(CHK_IS_ANY_TYPE(fbParaTypeDef))				&&
						
						(GET_BASIC_SIZE(specActionParaTypeDefPtr->dataType) == SIZE32_TYPE)
					) {
						
						
						specData = 0;
						memoryCopy ( (uint8 *) tempPtr,(uint8 *) &specData, 4 );
						tempPtr++;
					}
				}

			}	

		}
	} 

	
	retVal = onlineDwlReqDataSendToFbTask(ONLINE_DWL_FBCODE_ADD,NULL, NULL);	
	if(retVal != NO_ERROR){
		setErrorCode (__FILE__, __LINE__, "onlineDwlFbCodeAddFuncInComTask", retVal);
		status = FB_ONLINE_DOWNLOAD_ERR;
	}

MODULE_END:
	
	return(status);
}


uint32 onlineDwlFbCodeDelFuncInComTask(	uint32 	*reqDataPtr, 
										uint32	*respDataPtr,
										uint32	*respRetLenPtr
									)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32	numOfTotalSegment,numOfSegment, numOfDelFbCode,numOfTotalDelFbCode;
	uint32 segmentId,fbAddr,fbId, sumOfFbCodeNum;	
	uint32	*saveReqDataPtr;
	strFbTaskTblMemInfo	segTblInfo;
	strFbMemInfo	fbMemInfo;
	uint32	maxSegId, loop, fbDelLoop, segDelLoop, checkLoop;
	uint32	totalFrameNum, curFrameId,prevNumOfFbCode;
	strOnlineDwlDelFbCodeMngrInfo	*onlineFbDelMngrInfoPtr = &sOnlineDwlFbDelInfo;
	strOnlineDwlDelSegInfo			*onlineSegDelInfoPtr = onlineFbDelMngrInfoPtr->delSegInfoPtr;
	strOnlineDwlDelFbCodeInfo		*onlineFbDelInfoPtr;
	
	uint32 taskLogicType = 0, numOftotalActTrs, numOfActTrs = 1, sfcDelLoop =0; 
	uint32 usingFbType, actTrsId;

	*respRetLenPtr	= 0;
	
	
	totalFrameNum	= (*reqDataPtr & 0xffff0000) >> 16;
	curFrameId 		= *reqDataPtr++ & 0xffff;

	
	saveReqDataPtr 	= reqDataPtr;
	
	
	
	numOfTotalSegment	= (*reqDataPtr & 0xffff0000) >> 16;
	numOfSegment 		= (*reqDataPtr++ & 0xffff);

	
	*respDataPtr++ 	= numOfSegment;
	*respRetLenPtr 	+= 4;

		
    if( (numOfTotalSegment == 0) || (numOfTotalSegment > MAX_ONLINE_DWL_SEG_DEL_NUM)){
		status = ONLINE_DWL_SEG_NUM_ERR;
       	setErrorCodeWithVal( 	__FILE__,__LINE__,
               	        		"onlineDwlFbCodeDelFuncInComTask",status,
                   	    		"Total Segment ADD Number",numOfTotalSegment,
                   	    		"MAX Segment ADD Number", MAX_ONLINE_DWL_SEG_DEL_NUM,
                   	    		"NOT Using", 0 
				  	  		);
		goto MODULE_END;
	}
	
    else if( (numOfSegment == 0) || (numOfSegment > MAX_ONLINE_DWL_SEG_DEL_NUM)){
		status = ONLINE_DWL_SEG_NUM_ERR;
       	setErrorCodeWithVal( 	__FILE__,__LINE__,
               	        		"onlineDwlFbCodeDelFuncInComTask",status,
                   	    		"Total Fb Code ADD Number",numOfSegment,
                   	    		"Prev FbCode Add Numver", onlineFbDelMngrInfoPtr->numOfTotalFbCode,
                   	    		"MAX FB Code ADD Number", MAX_ONLINE_DWL_SEG_DEL_NUM
				  	  		);
		goto MODULE_END;
	}

	
	numOfTotalDelFbCode = *reqDataPtr++;
	
	if(	(numOfTotalDelFbCode > MAX_ONLINE_DWL_FB_CODE_DEL_NUM)||
		(numOfTotalDelFbCode == 0x0)
	){
		status = FB_NUM_EXCEED_ERR;
    	setErrorCodeWithVal( 	__FILE__,__LINE__,
           		        		"onlineDwlFbCodeDelFuncInComTask",status,
               	    			"Total FB Del Number", numOfTotalDelFbCode,
               	    			"MAX FB Code Del Number", MAX_ONLINE_DWL_FB_CODE_DEL_NUM,
               	    			"NOT Using",0
			  	  			);
		goto MODULE_END;
	}		
	
	
	if(curFrameId == 0x1) {
		onlineFbDelMngrInfoPtr->numOfTotalSegment 	= numOfTotalSegment;
		
		
		onlineFbDelMngrInfoPtr->dwlTotalFbCodeNum 	= numOfTotalDelFbCode;	
		onlineFbDelMngrInfoPtr->dwlTotalFrameNum 	= totalFrameNum;	
	}
	
	onlineFbDelMngrInfoPtr->dwlCurFrameNum 	= curFrameId;	
	
	
	sumOfFbCodeNum = 0;
	for(segDelLoop = 0; segDelLoop < numOfSegment; segDelLoop++) {
		
		
		taskLogicType   = (*reqDataPtr>> 31) & 0x1;	 
		segmentId 		= *reqDataPtr++ & 0xffff;

		
		if(taskLogicType == SFC_LOGIC_TYPE){
			numOftotalActTrs	= (*reqDataPtr & 0xffff0000) >> 16;	 
			numOfActTrs			= *reqDataPtr++ & 0xffff;
		}
		else {
			numOftotalActTrs	= 0;	 
			numOfActTrs			= 1;
			reqDataPtr++;
		}

		if((taskLogicType != 0) && (taskLogicType != SFC_LOGIC_TYPE)) {
			status = L_LOGIC_ID_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        		"onlineDwlFbCodeAddFuncInComTask",status,
           	        	    		"Cur Logic Type",taskLogicType,
           	        	    		"NOT Using", 0 ,
           	        	    		"NOT Using", 0 
					  	  		);
			goto MODULE_END;
		}

		if((segmentId == 0) || (segmentId > MAX_LOGIC_TASK_NUM)) {
			status = L_SEG_ID_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        		"onlineDwlFbCodeDelFuncInComTask",status,
           	        	    		"Segment ID",segmentId,
           	        	    		"MAX Segment ID", MAX_LOGIC_TASK_NUM,
           	        	    		"NOT Using", 0 
					  	  		);
			goto MODULE_END;
		}

		
		maxSegId = 0;
		for(loop = 1; loop <= MAX_LOGIC_TASK_NUM; loop++) {
			memoryClear( (uint8 *)&segTblInfo, sizeof(strFbTaskTblMemInfo));
		
   			(void)getFbTaskTblInfo( 
								LOGIC_ID,
           						loop,
								(strFbTaskTblMemInfo	*) &segTblInfo
								);
			if(segTblInfo.taskId > maxSegId)
				maxSegId = segTblInfo.taskId;
			
			if(segTblInfo.taskId == segmentId) break;
		}

		if(segTblInfo.taskId == 0) {
			status = ONLINE_DWL_SEG_EXIST_ERR;
       		setErrorCodeWithVal( 	__FILE__,	__LINE__,
           	    	        		"onlineDwlFbCodeDelFuncInComTask",status,
                	   	    		"Segment ID",segmentId,
                 	  	    		"Current Max Seg Id", maxSegId,
                  	 	    		"NOT Using", 0 
				  		  		);
			goto MODULE_END;		
		}
		
		if(segTblInfo.logicType != taskLogicType)
		{
			status = L_LOGIC_ID_ERR;
       		setErrorCodeWithVal( 	__FILE__,	__LINE__,
           	    	        		"onlineDwlFbCodeDelFuncInComTask",status,
                	   	    		"Segment ID",segmentId,
                 	  	    		"Current Max Seg Id", segTblInfo.logicType,
                  	 	    		"Recv Logic Type", taskLogicType
				  		  		);
			goto MODULE_END;		
		}

		for(sfcDelLoop = 0; sfcDelLoop < numOfActTrs; sfcDelLoop++) {
			
			if(taskLogicType == SFC_LOGIC_TYPE){
				usingFbType = (*reqDataPtr & 0xffff0000) >> 16;	 
				actTrsId =  *reqDataPtr++ & 0xffff;
			
				
				if(usingFbType == TRANSITION_TYPE) {
					if(actTrsId==0 ||actTrsId>MAX_TRANSITOIN_NUM){
						status = L_TRANSITION_ID_ERR;
       					setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        					"onlineDwlFbCodeAddFuncInComTask",status,
           	        	    					"Cur TRANSITION ID",actTrsId,
           	        	    					"MAX TRANSITION ID", MAX_TRANSITOIN_NUM ,
           	        	    					"NOT Using", 0 
					  	  					);
						goto MODULE_END;
					}
				}
				
				else if(usingFbType == ACTION_TYPE) {
					if(actTrsId==0 ||actTrsId>MAX_ACTION_NUM){
						status = L_TRANSITION_ID_ERR;
       					setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        					"onlineDwlFbCodeAddFuncInComTask",status,
           	        	    					"Cur ACTION ID",actTrsId,
           	        	    					"MAX ACTION ID", MAX_ACTION_NUM ,
           	        	    					"NOT Using", 0 
					  	  					);
						goto MODULE_END;
					}
				}
				else{
					status = SFC_TYPE_ID_ERR;
       				setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        				"onlineDwlFbCodeAddFuncInComTask",status,
           	        	    				"Cur SFC FB Type",usingFbType,
           	        	    				"NOT Using", 0 ,
           	        	    				"NOT Using", 0 
					  	  				);
					goto MODULE_END;
				}
				
			}
 			else reqDataPtr++;	

			
			
			numOfTotalDelFbCode	= (*reqDataPtr & 0xffff0000) >> 16;	 
			numOfDelFbCode		= *reqDataPtr++ & 0xffff;
			sumOfFbCodeNum		+= 	numOfDelFbCode;

			for(fbDelLoop = 0; fbDelLoop < numOfDelFbCode; fbDelLoop++) {
				
				fbAddr = (*reqDataPtr >> 16) & 0xffff;
				fbId = *reqDataPtr++ & 0xffff;	
				
				
				if( (fbAddr < 1) ||
					(fbAddr > MAX_MPU_BLOCK_ADDR)
				){
					status = SEG_BLK_ADDR_EXCEED_ERR;
	       			setErrorCodeWithVal( 	__FILE__,__LINE__,
        	   	    	        			"onlineDwlFbCodeDelFuncInComTask",status,
                		   	    			"FB Addr",fbAddr,
                  	 		    			"Max FB Addr", MAX_MPU_BLOCK_ADDR,
                  	 		    			"Not Using", 0 
				  		  				);
								  		  				
				}   
				
				
				else if(fbId > MAX_FB_CODE_ID){
					status = L_FB_CODE_ERR;
	       			setErrorCodeWithVal( 	__FILE__,__LINE__,
        	   	    	        			"onlineDwlFbCodeDelFuncInComTask",status,
                		   	    			"FB Code",fbId,
                 		  	    			"Max FB Code ", MAX_FB_CODE_ID,
                  	 		    			"Not Using", 0 
				  		  				);
				}				
				
				else if( fbId != gFbCodeSortingTableInfo[fbId].fbId){
					status = L_FB_CODE_ERR;
	       			setErrorCodeWithVal( 	__FILE__,__LINE__,
        	   	    	        			"onlineDwlFbCodeDelFuncInComTask",status,
                		   	    			"FB Code",fbId,
                 		  	    			"Defined Fb Code ", gFbCodeSortingTableInfo[fbId].fbId,
                  	 		    			"Not Using", 0 
				  		  				);
				}				
				

				
				else {
    				(void)fbCodeMemInfoRead	(
											LOGIC_ID,
       										fbAddr,
       										(strFbMemInfo *) &fbMemInfo
       										);
       				if(fbId != fbMemInfo.fbId) {
						status = ONLINE_FB_CODE_MISMATCH_ERR;
	       				setErrorCodeWithVal( 	__FILE__,__LINE__,
        	   	    		        			"onlineDwlFbCodeDelFuncInComTask",status,
                		   	    				"Block Addr",fbAddr,
                 		  	    				"Request FB Code ", fbId,
                  	 		    				"Read FB Code", fbMemInfo.fbId 
				  		  					);
					}
				}						
				
				if (status != NO_ERROR) goto MAKE_RESP;
			}	

		}
		
			
MAKE_RESP:
		*respDataPtr 	= (status & 0xffff) << 16;
		*respDataPtr++	|= segmentId;
		*respRetLenPtr 	+= 4;	
		
		if(status != NO_ERROR) goto MODULE_END;
	} 
	
	
	if(status != NO_ERROR)
		goto MODULE_END;
	
	
	onlineFbDelMngrInfoPtr->numOfTotalFbCode 	+= sumOfFbCodeNum;

	
	if(totalFrameNum == curFrameId) {
		 if((onlineFbDelMngrInfoPtr->numOfTotalFbCode > MAX_ONLINE_DWL_FB_CODE_DEL_NUM)||
		 	(onlineFbDelMngrInfoPtr->numOfTotalFbCode == 0x0)||
			(onlineFbDelMngrInfoPtr->dwlTotalFbCodeNum != onlineFbDelMngrInfoPtr->numOfTotalFbCode)
		 ){
			status = FB_NUM_EXCEED_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,
               		        		"onlineDwlFbCodeDelFuncInComTask",status,
                   	    			"SUM Fb Code DEL Number",sumOfFbCodeNum,
                   	    			"Total FB DEL  Number", onlineFbDelMngrInfoPtr->numOfTotalFbCode,
                   	    			"Total FB Set  Number", onlineFbDelMngrInfoPtr->dwlTotalFbCodeNum
				  	  			);
			goto MODULE_END;
		}
	}
		
		

	
	reqDataPtr = saveReqDataPtr;

	
	numOfSegment = *reqDataPtr++&0xffff;

	
	numOfTotalDelFbCode = *reqDataPtr++;
	
	for(segDelLoop = 0; segDelLoop < numOfSegment; segDelLoop++){
		
		
		taskLogicType   = (*reqDataPtr>> 31) & 0x1;
		segmentId 		= *reqDataPtr++ & 0xffff;
		
		
		for(checkLoop = 0; checkLoop < MAX_ONLINE_DWL_SEG_ADD_NUM; checkLoop++) {
			if((onlineSegDelInfoPtr->taskId == 0) || (onlineSegDelInfoPtr->taskId == segmentId))
				break;
			
			onlineSegDelInfoPtr++;	
		}
		if(checkLoop == MAX_ONLINE_DWL_SEG_DEL_NUM) {
			status = FB_ONLINE_DOWNLOAD_ERR;
			setErrorCode (__FILE__,__LINE__,"onlineDwlFbCodeDelFuncInComTask", status);
			goto MODULE_END;	
		}

		if(taskLogicType == SFC_LOGIC_TYPE){
			
			onlineSegDelInfoPtr->logicType = SFC_LOGIC_TYPE;
			numOftotalActTrs	= (*reqDataPtr & 0xffff0000) >> 16;	 
			numOfActTrs			= *reqDataPtr++ & 0xffff;
		}
		else {
			onlineSegDelInfoPtr->logicType = FB_LOGIC_TYPE;
			numOftotalActTrs	= 0;	 
			numOfActTrs			= 1;
			reqDataPtr++;	
		}
		
		onlineSegDelInfoPtr->taskId 			= segmentId;
		onlineSegDelInfoPtr->logicType		= taskLogicType;

		for(sfcDelLoop = 0; sfcDelLoop < numOfActTrs; sfcDelLoop++)
		{
			if(taskLogicType == SFC_LOGIC_TYPE){
				usingFbType = (*reqDataPtr & 0xffff0000) >> 16;	 
				actTrsId =  *reqDataPtr++ & 0xffff;
			}
			else {
				usingFbType		= 0;
				actTrsId		= 0;
				reqDataPtr ++; 
			}

			
			numOfDelFbCode	= *reqDataPtr++ & 0xffff;
			prevNumOfFbCode	= onlineSegDelInfoPtr->numOfFbCode;
			
			onlineSegDelInfoPtr->numOfFbCode 	+= numOfDelFbCode;	

			if(onlineSegDelInfoPtr->numOfFbCode  > MAX_ONLINE_DWL_FB_CODE_DEL_NUM) {
				status = FB_NUM_EXCEED_ERR;
				setErrorCode (__FILE__,__LINE__,"onlineDwlFbCodeDelFuncInComTask", status);
				goto MODULE_END;	
			}

			
			
			onlineFbDelInfoPtr = onlineSegDelInfoPtr->delFbCodeInfo + prevNumOfFbCode;

			for(fbDelLoop = 0; fbDelLoop < numOfDelFbCode; fbDelLoop++,onlineFbDelInfoPtr++){
				fbAddr = (*reqDataPtr >> 16) & 0xffff;
				fbId = *reqDataPtr++ & 0xffff;	
		
				onlineFbDelInfoPtr->fbBlkAddr 	= (uint16)fbAddr;
				onlineFbDelInfoPtr->fbId		= (uint16)fbId;

				if( onlineSegDelInfoPtr->logicType == SFC_LOGIC_TYPE )
				{
					onlineFbDelInfoPtr->logicType = SFC_LOGIC_TYPE;
					onlineFbDelInfoPtr->usingType = usingFbType;
		
					if(usingFbType == TRANSITION_TYPE) {
						onlineFbDelInfoPtr->TransitionID = actTrsId;
						onlineFbDelInfoPtr->ActionID	=   0 ;
					}
					else if(usingFbType == ACTION_TYPE) {
						onlineFbDelInfoPtr->TransitionID =  0 ;
						onlineFbDelInfoPtr->ActionID	=   actTrsId;
					}
				}
				else{
					onlineFbDelInfoPtr->logicType = FB_LOGIC_TYPE;
					onlineFbDelInfoPtr->usingType = 0;
					onlineFbDelInfoPtr->TransitionID = 0;
					onlineFbDelInfoPtr->ActionID	=  0 ;
				}
				onlineFbDelInfoPtr->taskId = segmentId;			

			}	

		} 
		
	} 

	
	
	onlineDwlDelFbMemorySet();	

	
	retVal = onlineDwlReqDataSendToFbTask(ONLINE_DWL_FBCODE_DEL,NULL, NULL);	
	
	if(retVal != NO_ERROR){
		setErrorCode (__FILE__, __LINE__, "onlineDwlFbCodeDelFuncInComTask",retVal);
		status = FB_ONLINE_DOWNLOAD_ERR;
	}
		
MODULE_END:

	return(status);
}


static uint32	onlineDwlDelFbMemorySet(void)
{
	uint32	status = NO_ERROR;
	uint32	segLoop, fbCodeLoop;
	strFbMemInfo	fbMemInfo;
	
	strOnlineDwlDelSegInfo		*delSegInfoPtr;
	strOnlineDwlDelFbCodeInfo	*delFbCodeInfoPtr;
	
	strOnlineDwlDelSegMemInfo	*delSegMemInfoPtr;
	strOnlineDwlDelFbMemInfo	*delFbCodeMemInfoPtr;

	
	sOnlineDwlFbDelMemInfo.numOfTotalSegment = sOnlineDwlFbDelInfo.numOfTotalSegment;

	delSegInfoPtr 		= sOnlineDwlFbDelInfo.delSegInfoPtr;
	delSegMemInfoPtr	= &sOnlineDwlFbDelMemInfo.delSegMemInfo;
	
	for(segLoop = 0; segLoop < sOnlineDwlFbDelInfo.numOfTotalSegment; 
	segLoop++,delSegInfoPtr++){ 
		
		
		
		delSegMemInfoPtr->numOfFbCode	+= delSegInfoPtr->numOfFbCode;
		
		delFbCodeInfoPtr 	= &delSegInfoPtr->delFbCodeInfo[0];
		delFbCodeMemInfoPtr	= &delSegMemInfoPtr->delFbMemInfo[0];

		for(fbCodeLoop = 0; fbCodeLoop < delSegInfoPtr->numOfFbCode;
			fbCodeLoop++,delFbCodeInfoPtr++,delFbCodeMemInfoPtr++) {
			status = fbMemInfoRead (	LOGIC_ID,
										delFbCodeInfoPtr->fbBlkAddr,
										&fbMemInfo
									);
			if(status != NO_ERROR) {
				setErrorCodeWithVal ( __FILE__, __LINE__, "onlineDwlDelFbMemorySet", status,
										"REQ BLK Addr", delFbCodeInfoPtr->fbBlkAddr,
										"NOT USING",0,
										"NOT USING", 0
										);
				return(status);											
			}
			else if(fbMemInfo.fbId != delFbCodeInfoPtr->fbId) {
				status = ONLINE_FB_CODE_MISMATCH_ERR;
				setErrorCodeWithVal ( __FILE__, __LINE__, "onlineDwlDelFbMemorySet", status,
										"REQ BLK Addr", delFbCodeInfoPtr->fbBlkAddr,
										"Del FB Code",delFbCodeInfoPtr->fbId,
										"Read FB Code",fbMemInfo.fbId
										);
				return(status);											
			}
			
			
			delFbCodeMemInfoPtr->fbId				= fbMemInfo.fbId;
			delFbCodeMemInfoPtr->fbBlkAddr			= delFbCodeInfoPtr->fbBlkAddr;
			delFbCodeMemInfoPtr->totalSpecMemNum	= fbMemInfo.inputNo + fbMemInfo.inputSize +  fbMemInfo.intlVarSize;
			delFbCodeMemInfoPtr->usableSpecMemNum	= fbMemInfo.inputNo + fbMemInfo.inputSize +  fbMemInfo.intlVarSize;
			delFbCodeMemInfoPtr->usedSpecMemNum 	= 0; 

			delFbCodeMemInfoPtr->totalOutputMemNum	= fbMemInfo.outputSize;
			delFbCodeMemInfoPtr->usableOutputMemNum	= fbMemInfo.outputSize;
			delFbCodeMemInfoPtr->usedOutputMemNum 	= 0; 

		} 
	}
	
	return(status);
}


static uint32 onlineDwlDelFbMemorySpecCheck(uint32	numOfMem, uint32 findFlag)
{
	uint32	 fbCodeLoop; 
	strOnlineDwlDelSegMemInfo	*delSegMemInfoPtr;
	strOnlineDwlDelFbMemInfo	*delFbCodeMemInfoPtr;

	delSegMemInfoPtr	= &sOnlineDwlFbDelMemInfo.delSegMemInfo;

	delFbCodeMemInfoPtr	= &delSegMemInfoPtr->delFbMemInfo[0];

	
	if(findFlag == FIND_SAME_MEM_SIZE) {
		for(fbCodeLoop = 0; fbCodeLoop < delSegMemInfoPtr->numOfFbCode;
			fbCodeLoop++,delFbCodeMemInfoPtr++) {
			
			if(delFbCodeMemInfoPtr->usableSpecMemNum == numOfMem) {
				delFbCodeMemInfoPtr->usedSpecMemNum 	+= numOfMem;
				delFbCodeMemInfoPtr->usableSpecMemNum	-= numOfMem;
				
				return(NO_ERROR);
			}
		}
	}
	else {
		
		delFbCodeMemInfoPtr	= &delSegMemInfoPtr->delFbMemInfo[0];
		for(fbCodeLoop = 0; fbCodeLoop < delSegMemInfoPtr->numOfFbCode;
			fbCodeLoop++,delFbCodeMemInfoPtr++) {
				
			if(delFbCodeMemInfoPtr->usableSpecMemNum > numOfMem) {
				delFbCodeMemInfoPtr->usedSpecMemNum 	+= numOfMem;
				delFbCodeMemInfoPtr->usableSpecMemNum	-= numOfMem;
				
				return(NO_ERROR);
			}
		}
	}

	return(1);
}



uint32 onlineDwlFbCodeSpecChgFuncInComTask(	uint32 	*reqDataPtr, 
											uint32	*respDataPtr,
											uint32	*respRetLenPtr
											)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32	numOfSegment, numOfSpecChgFbCode, numOfTotalSpecChgFbCode,segmentId,fbId, sumOfFbCodeNum;	
	uint32	*saveReqDataPtr, specIndex =0 ;
	uint32	fbBlkAddr, numOfChangeSpec,  prevNumOfSpec,numOfTotalSegment;
	strFbTaskTblMemInfo	segTblInfo;
	strFbMemInfo	fbMemInfo;
	uint32	maxSegId, loop, fbSpecChgLoop, segSpecChgLoop;
	uint32	totalFrameNum,curFrameId,checkLoop, prevNumOfFbCode;
	strOnlineDwlSpecChgFbCodeMngrInfo	*onlineFbSpecChgMngrInfoPtr = &sOnlineDwlFbSpecChgInfo;
	strOnlineDwlSpecChgSegInfo			*onlineSegSpecChgInfoPtr = onlineFbSpecChgMngrInfoPtr->specChgSegInfoPtr;
	strOnlineDwlSpecChgFbCodeInfo		*onlineFbSpecChgInfoPtr;
	strOnlineDwlChgSpecInfo				*specDataInfoPtr;

	strFbCodeTypePointerInfo fbParaTypeInfo;
	uint32 taskLogicType,numOftotalActTrs,numOfActTrs,sfcAddLoop,usingFbType,actTrsId;
	uint32 *specTypePtr,specType, specActionData, specData[2];
	uint32 anyFuncOutDataSize , anyFuncOutDataType;
    strFbSpecActionInfo *specActionParaTypeDefPtr;
					
	*respRetLenPtr	= 0;

	
	totalFrameNum	= (*reqDataPtr & 0xffff0000) >> 16;
	curFrameId 		= *reqDataPtr++ & 0xffff;
	
	
	saveReqDataPtr 	= reqDataPtr;
	
	
	
	numOfTotalSegment	= (*reqDataPtr & 0xffff0000) >> 16;
	numOfSegment 		= (*reqDataPtr++ & 0xffff);

	
	*respDataPtr++ 	= numOfSegment;
	*respRetLenPtr 	+= 4;

		
    if( (numOfTotalSegment == 0) || (numOfTotalSegment > MAX_ONLINE_DWL_SEG_SPEC_CHANGE_NUM)){
		status = ONLINE_DWL_SEG_NUM_ERR;
       	setErrorCodeWithVal( 	__FILE__,
       							__LINE__,
               	        		"onlineDwlFbCodeSpecChgFuncInComTask",
                   	    		status,
                   	    		"Total Segment ADD Number",numOfTotalSegment,
                   	    		"MAX Segment ADD Number", MAX_ONLINE_DWL_SEG_SPEC_CHANGE_NUM,
                   	    		"NOT Using", 0 
				  	  		);
		goto MODULE_END;
	}
	
    else if( (numOfSegment == 0) || (numOfSegment > MAX_ONLINE_DWL_SEG_SPEC_CHANGE_NUM)){
		status = ONLINE_DWL_SEG_NUM_ERR;
       	setErrorCodeWithVal( 	__FILE__,
       							__LINE__,
               	        		"onlineDwlFbCodeSpecChgFuncInComTask",
                   	    		status,
                   	    		"Current Segment Number",numOfSegment,
                   	    		"Prev Segment Number", onlineFbSpecChgMngrInfoPtr->numOfTotalFbCode,
                   	    		"MAX FB Code ADD Number", MAX_ONLINE_DWL_SEG_SPEC_CHANGE_NUM
				  	  		);
		goto MODULE_END;
	}

	
	numOfTotalSpecChgFbCode = *reqDataPtr++;
	
	if(	(numOfTotalSpecChgFbCode > MAX_ONLINE_DWL_FB_SPEC_CHANGE_NUM)||
		(numOfTotalSpecChgFbCode == 0x0)
	){
		status = FB_NUM_EXCEED_ERR;
    	setErrorCodeWithVal( 	__FILE__,
    							__LINE__,
           		        		"onlineDwlFbCodeSpecChgFuncInComTask",
               		    		status,
               	    			"Total FB Spec Chg Number", numOfTotalSpecChgFbCode,
               	    			"MAX FB Spec Chg Number", MAX_ONLINE_DWL_FB_SPEC_CHANGE_NUM,
               	    			"NOT Using",0
			  	  			);
		goto MODULE_END;
	}		
	
	
	if(curFrameId == 0x1) {
		onlineFbSpecChgMngrInfoPtr->numOfTotalSegment 	= numOfTotalSegment;
		
		
		onlineFbSpecChgMngrInfoPtr->dwlTotalFbCodeNum 	= numOfTotalSpecChgFbCode;	
		onlineFbSpecChgMngrInfoPtr->dwlTotalFrameNum 	= totalFrameNum;	
	}
	
	onlineFbSpecChgMngrInfoPtr->dwlCurFrameNum 	= curFrameId;	
	
	
	sumOfFbCodeNum = 0;
	for(segSpecChgLoop = 0; segSpecChgLoop < numOfSegment; segSpecChgLoop++) {

		
		taskLogicType   = (*reqDataPtr>> 31) & 0x1;	 
		segmentId 		= *reqDataPtr++ & 0xffff;

	
		if((segmentId == 0) || (segmentId > MAX_LOGIC_TASK_NUM)) {
			status = L_SEG_ID_ERR;
       		setErrorCodeWithVal( 	__FILE__,
       								__LINE__,
          	     	        		"onlineDwlFbCodeSpecChgFuncInComTask",
           	        	    		status,
           	        	    		"Segment ID",segmentId,
           	        	    		"MAX Segment ID", MAX_LOGIC_TASK_NUM,
           	        	    		"NOT Using", 0 
					  	  		);
			goto MODULE_END;
		}

		
		maxSegId = 0;
		for(loop = 1; loop <= MAX_LOGIC_TASK_NUM; loop++) {
			memoryClear( (uint8 *)&segTblInfo, sizeof(strFbTaskTblMemInfo));
		
   			(void)getFbTaskTblInfo( 
								LOGIC_ID,
           						loop,
								(strFbTaskTblMemInfo	*) &segTblInfo
								);
			if(segTblInfo.taskId > maxSegId)
				maxSegId = segTblInfo.taskId;
			
			if(segTblInfo.taskId == segmentId) break;
		}

		if(segTblInfo.taskId == 0) {
			status = ONLINE_DWL_SEG_EXIST_ERR;
       		setErrorCodeWithVal( 	__FILE__,
       								__LINE__,
           	    	        		"onlineDwlFbCodeSpecChgFuncInComTask",
              	     	    		status,
                	   	    		"Segment ID",segmentId,
                 	  	    		"Current Max Seg Id", maxSegId,
                  	 	    		"NOT Using", 0 
				  		  		);
			goto MODULE_END;		
		}

		if(segTblInfo.logicType != taskLogicType) {
			status = L_LOGIC_ID_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        		"onlineDwlFbCodeSpecChgFuncInComTask",status,
									"Segment ID",segmentId,
           	        	    		"Curr Logic Type",segTblInfo.logicType,
           	        	    		"Recv Logic Type",taskLogicType
					  	  		);
			goto MODULE_END;
		}
		
		
		if(taskLogicType == SFC_LOGIC_TYPE){
			numOftotalActTrs	= (*reqDataPtr & 0xffff0000) >> 16;	 
			numOfActTrs			= *reqDataPtr++ & 0xffff;
		}
		else {
			numOftotalActTrs	= 0;	 
			numOfActTrs			= 1;
			reqDataPtr++;
		}

		for(sfcAddLoop = 0; sfcAddLoop < numOfActTrs; sfcAddLoop++) {
			
			if(taskLogicType == SFC_LOGIC_TYPE){
				usingFbType = (*reqDataPtr & 0xffff0000) >> 16;	 
				actTrsId =  *reqDataPtr++ & 0xffff;
			
				
				if(usingFbType == TRANSITION_TYPE) {
					if(actTrsId==0 ||actTrsId>MAX_TRANSITOIN_NUM){
						status = L_TRANSITION_ID_ERR;
       					setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        					"onlineDwlFbCodeSpecChgFuncInComTask",status,
           	        	    					"Cur TRANSITION ID",actTrsId,
           	        	    					"MAX TRANSITION ID", MAX_TRANSITOIN_NUM ,
           	        	    					"NOT Using", 0 
					  	  					);
						goto MODULE_END;
					}
				}
				
				else if(usingFbType == ACTION_TYPE) {
					if(actTrsId==0 ||actTrsId>MAX_ACTION_NUM){
						status = L_TRANSITION_ID_ERR;
       					setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        					"onlineDwlFbCodeSpecChgFuncInComTask",status,
           	        	    					"Cur ACTION ID",actTrsId,
           	        	    					"MAX ACTION ID", MAX_ACTION_NUM ,
           	        	    					"NOT Using", 0 
					  	  					);
						goto MODULE_END;
					}
				}
				else{
					status = SFC_TYPE_ID_ERR;
       				setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        				"onlineDwlFbCodeSpecChgFuncInComTask",status,
           	        	    				"Cur SFC FB Type",usingFbType,
           	        	    				"NOT Using", 0 ,
           	        	    				"NOT Using", 0 
					  	  				);
					goto MODULE_END;
				}
				
			}
 			else reqDataPtr++;	


			
			
			numOfTotalSpecChgFbCode	= (*reqDataPtr & 0xffff0000) >> 16;	 
			numOfSpecChgFbCode		= *reqDataPtr++ & 0xffff;
			sumOfFbCodeNum			+= 	numOfSpecChgFbCode;

			
			if( (numOfTotalSpecChgFbCode > MAX_ONLINE_DWL_FB_SPEC_CHANGE_NUM) ||
				(numOfSpecChgFbCode > MAX_ONLINE_DWL_FB_SPEC_CHANGE_NUM) ||
				(numOfTotalSpecChgFbCode < numOfSpecChgFbCode) ||
				(numOfTotalSpecChgFbCode == 0x0) ||
				(numOfSpecChgFbCode == 0x0)
			) {
				status = FB_NUM_EXCEED_ERR;
    			setErrorCodeWithVal( 	__FILE__,
    									__LINE__,
           			        			"onlineDwlFbCodeSpecChgFuncInComTask",
               			    			status,
               	    					"Total FB Add  Number", numOfTotalSpecChgFbCode,
               	    					"Fb Add Number",numOfSpecChgFbCode,
               	    					"MAX FB Code ADD Number", MAX_ONLINE_DWL_FB_SPEC_CHANGE_NUM
			  	  					);
				goto MODULE_END;
			}
		
			for(fbSpecChgLoop = 0; fbSpecChgLoop < numOfSpecChgFbCode; fbSpecChgLoop++) {
				
				fbBlkAddr	= (*reqDataPtr >> 16) & 0xffff;
				fbId 		= *reqDataPtr++ & 0xffff;
				
				
				anyFuncOutDataSize = (*reqDataPtr >> 16)&0xffff;
				anyFuncOutDataType = *reqDataPtr++& 0xffff;

				numOfChangeSpec = *reqDataPtr++;
				
    			(void)fbCodeMemInfoRead	(
											LOGIC_ID,
       										fbBlkAddr,
       										(strFbMemInfo *) &fbMemInfo
       									);
       									
				
				if( (fbBlkAddr < 1) ||
					(fbBlkAddr > MAX_MPU_BLOCK_ADDR)
				){
					status = SEG_BLK_ADDR_EXCEED_ERR;
	       			setErrorCodeWithVal( 	__FILE__,__LINE__,
        	   	    	        			"onlineDwlFbCodeSpecChgFuncInComTask",status,
                		   	    			"FB Addr",fbBlkAddr,
                 		  	    			"MAX FB Addr", MAX_MPU_BLOCK_ADDR,
                  	 		    			"Not Using", 0
				  		  				);
				}   
				
				else if(fbId > MAX_FB_CODE_ID){
					status = L_FB_CODE_ERR;
	       			setErrorCodeWithVal( 	__FILE__,
    	   									__LINE__,
        	   	    	        			"onlineDwlFbCodeSpecChgFuncInComTask",
            	  	     	    			status,
                		   	    			"FB Code",fbId,
                 		  	    			"Max FB Code ", MAX_FB_CODE_ID,
                  	 		    			"Not Using", 0 
				  		  				);
				}				
				
				else if( fbId != gFbCodeSortingTableInfo[fbId].fbId){
					status = L_FB_CODE_ERR;
	       			setErrorCodeWithVal( 	__FILE__,
    	   									__LINE__,
        	   	    	        			"onlineDwlFbCodeSpecChgFuncInComTask",
            	  	     	    			status,
                		   	    			"FB Code",fbId,
                 		  	    			"Defined Fb Code ", gFbCodeSortingTableInfo[fbId].fbId,
                  	 		    			"Not Using", 0 
				  		  				);
				}				
			
				
				else if(fbId != fbMemInfo.fbId) {
					status = ONLINE_FB_CODE_MISMATCH_ERR;
	       			setErrorCodeWithVal( 	__FILE__,
    	   									__LINE__,
        	   	   		        			"onlineDwlFbCodeSpecChgFuncInComTask",
            	      		    			status,
                	   	    				"Block Addr",fbBlkAddr,
                 	  	    				"Request FB Code ", fbId,
                  		    				"Read FB Code", fbMemInfo.fbId 
				 	  					);
				}

				
				else if(IS_ANY_TYPE_FB_CODE(fbId)) {
					
					if( (anyFuncOutDataSize < 1) || (anyFuncOutDataSize > 5)){
						status = ANY_FUNC_OUT_SIZE_ERR;
						setErrorCodeWithVal(	(uint8 *)__FILE__, (uint32)__LINE__, 
												(uint8 *)"onlineDwlFbCodeSpecChgFuncInComTask", status,
												"Block Addr", fbBlkAddr,
												"FB Code Id",fbId,
												"Out Data Size", anyFuncOutDataSize
			  								);
						break;			
					}

					
					if( (anyFuncOutDataType < 1) || (anyFuncOutDataType > 10)){
						status = ANY_FUNC_OUT_TYPE_ERR;
						setErrorCodeWithVal(	(uint8 *)__FILE__, (uint32)__LINE__, 
												(uint8 *)"onlineDwlFbCodeSpecChgFuncInComTask", status,
												"Block Addr", fbBlkAddr,
												"FB Code Id",fbId,
												"Out Data Type", anyFuncOutDataType
			  					);
						break;			
					}
				}
				
				
				else if(numOfChangeSpec > fbMemInfo.inputNo){
					status = L_PARA_NO_ERR;
	       			setErrorCodeWithVal( 	__FILE__,
    	   									__LINE__,
        	    		        			"onlineDwlFbCodeSpecChgFuncInComTask",
              	     		    			status,
               		   	    				"fb Code",fbId,
               		  	    				"Request Change Spec Num ", numOfChangeSpec,
               	 		    				"FB Code Spec Num", fbMemInfo.inputNo 
			  		  					);
				}
				else if(numOfChangeSpec > MAX_FB_SPEC_PERMISSION_NUM){
					status = L_PARA_NO_ERR;
	       			setErrorCodeWithVal( 	__FILE__,
    	   									__LINE__,
        	    		        			"onlineDwlFbCodeSpecChgFuncInComTask",
              	     		    			status,
               		   	    				"fb Code",fbId,
               		  	    				"Request Change Spec Num ", numOfChangeSpec,
               	 		    				"FB Code Spec Num", fbMemInfo.inputNo 
			  		  					);
				}

				status = fbCodeParaTypePointerGet 
									(fbId, (strFbCodeTypePointerInfo	*)&fbParaTypeInfo );
				if(status != NO_ERROR) {
					setErrorCodeWithVal( 	(uint8 *)__FILE__,(uint32)__LINE__,
        		        	        		(uint8 *)"onlineDwlFbCodeSpecChgFuncInComTask",status,
                	            			(uint8 *)"fb Code", fbId,
                    	            		(uint8 *)"Not Using", 0,
                        					(uint8 *)"Not Using", 0
										);

				}

				else if((status == NO_ERROR)) {
					specTypePtr = (uint32 *)fbParaTypeInfo.specTypeStartAddr;
					
					
					for(loop = 0; loop < numOfChangeSpec; loop++) {
						specIndex 	= (*reqDataPtr>> 16) & 0xffff;
						specType	=*reqDataPtr++ & 0xffff;

						if((specIndex == 0) || (specIndex > fbMemInfo.inputNo)){
							status = L_PARA_NO_ERR;
	       					setErrorCodeWithVal( 	__FILE__,
    	   											__LINE__,
        	   	    			        			"onlineDwlFbCodeSpecChgFuncInComTask",status,
                		   	    					"fb Code",fbId,
                 		  	    					"Spec Index", specIndex,
                  	 		    					"FB Code Spec Num", fbMemInfo.inputNo 
				  		  						);				
						} 			
						
						if(!CHK_IS_ANY_TYPE(*(specTypePtr + specIndex - 1)) ) {
							
							if( specType != GET_BASIC_DATA_TYPE(*(specTypePtr + specIndex - 1)) )
							 {
									status = FB_CODE_SPEC_TYPE_ERR ;
									setErrorCodeWithVal( 	(uint8 *)__FILE__,(uint32)__LINE__,
        		        				(uint8 *)"onlineDwlFbCodeSpecChgFuncInComTask",status,
                						(uint8 *)"Input Index", specIndex,
                    					(uint8 *)"Recv Type", specType,
                        				(uint8 *)"Read Type", GET_BASIC_DATA_TYPE(*(specTypePtr + specIndex - 1))
									);
									setErrorCodeWithVal( 	(uint8 *)__FILE__,(uint32)__LINE__,
        		        				(uint8 *)"onlineDwlFbCodeSpecChgFuncInComTask",status,
                						(uint8 *)"FB ADDR", fbBlkAddr,
                    					(uint8 *)"FB CODE", fbId,
                        				(uint8 *)"TASK ID", segmentId
									);
							}
						}
						
						
						specActionData	= *reqDataPtr++;
						
						specData[0] = *reqDataPtr++;
						if (GET_TYPE_SIZE(*(specTypePtr + specIndex - 1))  == SIZE64_TYPE ) 
	    					specData[1] = *reqDataPtr++ ;
					}
				}						
				
				if(status != NO_ERROR) goto MAKE_RESP;
				
			}	
		}  
		
		
			
MAKE_RESP:
		*respDataPtr 	= (status & 0xffff) << 16;
		*respDataPtr++	|= segmentId;
		*respRetLenPtr 	+= 4;	
		
		if(status != NO_ERROR) goto MODULE_END;
	} 
	
	
	onlineFbSpecChgMngrInfoPtr->numOfTotalFbCode 	+= sumOfFbCodeNum;

	
	if(totalFrameNum == curFrameId) {
		 if((onlineFbSpecChgMngrInfoPtr->numOfTotalFbCode > MAX_ONLINE_DWL_FB_SPEC_CHANGE_NUM)||
		 	(onlineFbSpecChgMngrInfoPtr->numOfTotalFbCode == 0x0)||
			(onlineFbSpecChgMngrInfoPtr->dwlTotalFbCodeNum != onlineFbSpecChgMngrInfoPtr->numOfTotalFbCode)
		 ){
			status = FB_NUM_EXCEED_ERR;
       		setErrorCodeWithVal( 	__FILE__, __LINE__,
               		        		"onlineDwlFbCodeSpecChgFuncInComTask",	status,
                   	    			"SUM Fb Code ADD Number",sumOfFbCodeNum,
                   	    			"Total FB   Number", onlineFbSpecChgMngrInfoPtr->numOfTotalFbCode,
                   	    			"Total FB Set  Number", onlineFbSpecChgMngrInfoPtr->dwlTotalFbCodeNum
				  	  			);
			goto MODULE_END;
		}		
	}		

		
	
	reqDataPtr = saveReqDataPtr;

	
	numOfSegment = *reqDataPtr++&0xffff;

	
	numOfTotalSpecChgFbCode = *reqDataPtr++;

	for(segSpecChgLoop = 0; segSpecChgLoop < numOfSegment; segSpecChgLoop++){
		
		
		taskLogicType   = (*reqDataPtr>> 31) & 0x1;
		segmentId 		= *reqDataPtr++ & 0xffff;

		
		for(checkLoop = 0; checkLoop < MAX_ONLINE_DWL_SEG_SPEC_CHANGE_NUM; checkLoop++) {
			if((onlineSegSpecChgInfoPtr->taskId == 0) || (onlineSegSpecChgInfoPtr->taskId == segmentId))
				break;
			
			onlineSegSpecChgInfoPtr++;	
		}
		if(checkLoop == MAX_ONLINE_DWL_SEG_SPEC_CHANGE_NUM) {
			status = FB_ONLINE_DOWNLOAD_ERR;
			setErrorCode (__FILE__,__LINE__,"onlineDwlFbCodeSpecChgFuncInComTask", status);
			goto MODULE_END;	
		}
		
		if(taskLogicType==SFC_LOGIC_TYPE){
			
			numOftotalActTrs	= (*reqDataPtr & 0xffff0000) >> 16;	 
			numOfActTrs			= *reqDataPtr++ & 0xffff;
		}
		else {
			numOftotalActTrs	= 0;	 
			numOfActTrs			= 1;
			reqDataPtr++;	
		}
		
		onlineSegSpecChgInfoPtr->taskId 			= segmentId;
		onlineSegSpecChgInfoPtr->logicType		= taskLogicType;

		for(sfcAddLoop = 0; sfcAddLoop < numOfActTrs; sfcAddLoop++)
		{
			if(taskLogicType==SFC_LOGIC_TYPE){
				usingFbType = (*reqDataPtr & 0xffff0000) >> 16;	 
				actTrsId =  *reqDataPtr++ & 0xffff;
			}
			else {
				usingFbType = 0;
				actTrsId = 0;
				reqDataPtr++;	
			}

			
			numOfSpecChgFbCode	= *reqDataPtr++ & 0xffff;
			prevNumOfFbCode	= onlineSegSpecChgInfoPtr->numOfFbCode;

			onlineSegSpecChgInfoPtr->numOfFbCode 	+= numOfSpecChgFbCode;	

			if(onlineSegSpecChgInfoPtr->numOfFbCode  > MAX_ONLINE_DWL_FB_SPEC_CHANGE_NUM) {
				status = FB_NUM_EXCEED_ERR;
				setErrorCode (__FILE__,__LINE__,"onlineDwlFbCodeSpecChgFuncInComTask", status);
				goto MODULE_END;	
			}

			
			
			onlineFbSpecChgInfoPtr = onlineSegSpecChgInfoPtr->specChgFbCodeInfo + prevNumOfFbCode;

			for(fbSpecChgLoop = 0; fbSpecChgLoop < numOfSpecChgFbCode; fbSpecChgLoop++,onlineFbSpecChgInfoPtr++){
				
				fbBlkAddr		= (*reqDataPtr >> 16) & 0xffff;
				fbId 			= *reqDataPtr++ & 0xffff;

				
				anyFuncOutDataSize = (*reqDataPtr >> 16)&0xffff;
				anyFuncOutDataType = *reqDataPtr++& 0xffff;
				onlineFbSpecChgInfoPtr->paraRunningType  = (anyFuncOutDataType&0xF) | ((anyFuncOutDataSize&0xF)<<12);
				
				numOfChangeSpec		= *reqDataPtr++;
				
				
				onlineFbSpecChgInfoPtr->logicType	= taskLogicType;
				onlineFbSpecChgInfoPtr->usingType	= usingFbType;
				if(onlineFbSpecChgInfoPtr->usingType == TRANSITION_TYPE) {
					onlineFbSpecChgInfoPtr->ActionID	= 0;
					onlineFbSpecChgInfoPtr->TransitionID= actTrsId;
				}
				else if(onlineFbSpecChgInfoPtr->usingType == ACTION_TYPE) {
					onlineFbSpecChgInfoPtr->ActionID	= actTrsId;
					onlineFbSpecChgInfoPtr->TransitionID= 0;
				}
				else {
					onlineFbSpecChgInfoPtr->ActionID	= 0;
					onlineFbSpecChgInfoPtr->TransitionID= 0;
				}
				
				onlineFbSpecChgInfoPtr->fbId				= fbId;
				onlineFbSpecChgInfoPtr->fbBlkAddr 			= fbBlkAddr;
				onlineFbSpecChgInfoPtr->taskId				= segmentId;
				
				
				
				prevNumOfSpec	= onlineFbSpecChgInfoPtr->numOfChangedSpec;
				onlineFbSpecChgInfoPtr->numOfChangedSpec 	= numOfChangeSpec;
				specDataInfoPtr = &onlineFbSpecChgInfoPtr->specDataInfo[0] + prevNumOfSpec;
				
				if(numOfChangeSpec > MAX_FB_SPEC_PERMISSION_NUM){
					status = L_PARA_NO_ERR;
					setErrorCode (__FILE__,__LINE__,"onlineDwlFbCodeSpecChgFuncInComTask", status);
					goto MODULE_END;	
				}
					

				status = fbCodeParaTypePointerGet 
							(fbId, (strFbCodeTypePointerInfo	*)&fbParaTypeInfo );
				if(status != NO_ERROR) {
					setErrorCodeWithVal( 	(uint8 *)__FILE__,(uint32)__LINE__,
        		        	        		(uint8 *)"onlineDwlFbCodeSpecChgFuncInComTask",status,
                	            			(uint8 *)"fb Code", fbId,
                    	            		(uint8 *)"Not Using", 0,
                        					(uint8 *)"Not Using", 0
										);
				}
				specTypePtr = (uint32 *)fbParaTypeInfo.specTypeStartAddr;

				for(loop = 0; loop < numOfChangeSpec; loop++, specDataInfoPtr++){
					
					specDataInfoPtr->specIndex 	=(*reqDataPtr >> 16) & 0xffff;
					
					
					specIndex = specDataInfoPtr->specIndex;

					specDataInfoPtr->specType   =  *reqDataPtr++ & 0xffff;

					specDataInfoPtr->specActionData = *reqDataPtr++;
					specDataInfoPtr->specData[0] = *reqDataPtr++;

					specActionParaTypeDefPtr = (strFbSpecActionInfo *)&specDataInfoPtr->specActionData;

					if (GET_TYPE_SIZE(*(specTypePtr + specIndex - 1))  == SIZE64_TYPE ) {
						specDataInfoPtr->specType |= SIZE64_TYPE;
						if(GET_BASIC_SIZE(specActionParaTypeDefPtr->dataType) == SIZE64_TYPE) {
							specDataInfoPtr->specData[1] = *reqDataPtr++;
							if(specActionParaTypeDefPtr->refType == SPEC_TYPE_BYVAL)  {
								mySwap((uint32*)&specDataInfoPtr->specData[0],
						 			 (uint32*)&specDataInfoPtr->specData[1],DOUBLE_SIZE);
							}
						}

						else if ( (CHK_IS_ANY_TYPE(*(specTypePtr + specIndex - 1)) )&&
							 (GET_BASIC_SIZE(specActionParaTypeDefPtr->dataType) == SIZE32_TYPE)
						)
						{
							specDataInfoPtr->specData[1] = 0;
						}
					}
					else specDataInfoPtr->specType |= SIZE32_TYPE;
				}
			}	
		}	
	} 
	
	
	retVal = onlineDwlReqDataSendToFbTask(ONLINE_DWL_FBCODE_SPEC_CHANGE,NULL, NULL);	

	if(retVal != NO_ERROR){
		setErrorCode (__FILE__, __LINE__, "onlineDwlFbCodeSpecChgFuncInComTask", retVal);
		status = FB_ONLINE_DOWNLOAD_ERR;
	}

MODULE_END:

	return(status);
}



uint32 onlineDwlFbCodeSeqIdAddAndChgFuncInComTask(	uint32 	*reqDataPtr, 
													uint32	*respDataPtr,
													uint32	*respRetLenPtr
												)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32	numOfSegment,numOfTotalSegment, numOfSeqIdAddAndChgFbCode, segmentId,fbId, sumOfFbCodeNum;	
	uint32	*saveReqDataPtr;
	uint32	fbBlkAddr, curSeqId;
	uint32	totalFrameNum,curFrameId,numOfTotalSeqIdAddAndChgFbCode,checkLoop, startBufIndex;
	strFbTaskTblMemInfo	segTblInfo;
	uint32	maxSegId, loop, fbSeqIdAddAndChgLoop, segSeqIdAddAndChgLoop;
	strOnlineDwlFbSeqIdAddAndChgMngrInfo *onlineFbSeqIdAddAndChgMngrInfoPtr = &sOnlineDwlFbSeqIdAddAndChgInfo;
	strOnlineDwlFbSeqIdAddAndChgSegInfo	 *onlineSegSeqIdAddAndChgInfoPtr 
											= &onlineFbSeqIdAddAndChgMngrInfoPtr->fbSeqIdAddAndChgSegInfo[0];
	strOnlineDwlFbSeqIdInfo	*onlineFbSeqIdAddAndChgInfoPtr, *orgOnlineFbSeqIdAddAndChgInfoPtr;
	uint32  taskLogicType = 0;

	
	orgOnlineFbSeqIdAddAndChgInfoPtr = &onlineFbSeqIdAddAndChgMngrInfoPtr->fbSeqIdInfo[0];
	
	*respRetLenPtr	= 0;

	
	totalFrameNum	= (*reqDataPtr & 0xffff0000) >> 16;
	curFrameId 		= *reqDataPtr++ & 0xffff;
	
	
	saveReqDataPtr 	= reqDataPtr;
	
	
	
	numOfTotalSegment	= (*reqDataPtr & 0xffff0000) >> 16;
	numOfSegment 		= (*reqDataPtr++ & 0xffff);

	
	*respDataPtr++ 	= numOfSegment;
	*respRetLenPtr 	+= 4;

		
    if( (numOfTotalSegment == 0) || (numOfTotalSegment > MAX_ONLINE_DWL_SEG_SEQ_ID_ADD_AND_CHG_NUM)){
		status = ONLINE_DWL_SEG_NUM_ERR;
       	setErrorCodeWithVal( 	__FILE__,__LINE__,
               	        		"onlineDwlFbCodeSeqIdAddAndChgFuncInComTask",status,
                   	    		"Total Segment Number",numOfTotalSegment,
                   	    		"MAX Segment Number", MAX_ONLINE_DWL_SEG_SEQ_ID_ADD_AND_CHG_NUM,
                   	    		"NOT Using", 0 
				  	  		);
		goto MODULE_END;
	}
	
    else if( (numOfSegment == 0) || (numOfSegment > MAX_ONLINE_DWL_SEG_SEQ_ID_ADD_AND_CHG_NUM)){
		status = ONLINE_DWL_SEG_NUM_ERR;
       	setErrorCodeWithVal( 	__FILE__,
       							__LINE__,
               	        		"onlineDwlFbCodeSeqIdAddAndChgFuncInComTask",
                   	    		status,
                   	    		"Current Segment Number",numOfSegment,
                   	    		"Prev Segment Number", onlineFbSeqIdAddAndChgMngrInfoPtr->numOfTotalFbCode,
                   	    		"MAX Seqment Number", MAX_ONLINE_DWL_SEG_SEQ_ID_ADD_AND_CHG_NUM
				  	  		);
		goto MODULE_END;
	}

	
	numOfTotalSeqIdAddAndChgFbCode = *reqDataPtr++;

	if(	(numOfTotalSeqIdAddAndChgFbCode > MAX_ONLINE_DWL_FB_SEQ_ID_ADD_AND_CHG_NUM)
		|| (numOfTotalSeqIdAddAndChgFbCode == 0x0)
	){
		status = FB_NUM_EXCEED_ERR;
    	setErrorCodeWithVal( 	__FILE__, __LINE__,
           		        		"onlineDwlFbCodeSeqIdAddAndChgFuncInComTask", status,
               	    			"Total FB Seq ID Chg Number", numOfTotalSeqIdAddAndChgFbCode,
               	    			"MAX FB Seq ID Chg Number", MAX_ONLINE_DWL_FB_SEQ_ID_ADD_AND_CHG_NUM,
               	    			"NOT Using",0
			  	  			);
		goto MODULE_END;
	}	

	
	
	if(curFrameId == 0x1) {
		onlineFbSeqIdAddAndChgMngrInfoPtr->numOfTotalSegment 	= numOfTotalSegment;
		
		
		onlineFbSeqIdAddAndChgMngrInfoPtr->dwlTotalFbCodeNum 	= numOfTotalSeqIdAddAndChgFbCode;	
		onlineFbSeqIdAddAndChgMngrInfoPtr->dwlTotalFrameNum 	= totalFrameNum;	
	}
	
	onlineFbSeqIdAddAndChgMngrInfoPtr->dwlCurFrameNum 	= curFrameId;	

	
	sumOfFbCodeNum = 0;
	for(segSeqIdAddAndChgLoop = 0; segSeqIdAddAndChgLoop < numOfSegment; segSeqIdAddAndChgLoop++) {

		
		
		taskLogicType   = (*reqDataPtr>> 31) & 0x1;
		segmentId 		= *reqDataPtr++ & 0xffff;

		
		
		numOfTotalSeqIdAddAndChgFbCode	= (*reqDataPtr & 0xffff0000) >> 16;	 
		numOfSeqIdAddAndChgFbCode		= *reqDataPtr++ & 0xffff;
		sumOfFbCodeNum					+= 	numOfSeqIdAddAndChgFbCode;


		if((segmentId == 0) || (segmentId > MAX_LOGIC_TASK_NUM)) {
			status = L_SEG_ID_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        		"onlineDwlFbCodeSeqIdAddAndChgFuncInComTask",status,
           	        	    		"Segment ID",segmentId,
           	        	    		"MAX Segment ID", MAX_LOGIC_TASK_NUM,
           	        	    		"NOT Using", 0 
					  	  		);
			goto MODULE_END;
		}

		
		if( (numOfTotalSeqIdAddAndChgFbCode > MAX_ONLINE_DWL_FB_SEQ_ID_ADD_AND_CHG_NUM) ||
			(numOfSeqIdAddAndChgFbCode > MAX_ONLINE_DWL_FB_SEQ_ID_ADD_AND_CHG_NUM) ||
			(numOfTotalSeqIdAddAndChgFbCode < numOfSeqIdAddAndChgFbCode) ||
			(numOfTotalSeqIdAddAndChgFbCode == 0x0) ||
			(numOfSeqIdAddAndChgFbCode == 0x0)
		) {
			status = FB_NUM_EXCEED_ERR;
    		setErrorCodeWithVal( 	__FILE__,__LINE__,
           			        		"onlineDwlFbCodeSeqIdAddAndChgFuncInComTask",status,
               	    				"Total FB Add  Number", numOfTotalSeqIdAddAndChgFbCode,
               	    				"Fb Add Number",numOfSeqIdAddAndChgFbCode,
               	    				"MAX FB Code ADD Number", MAX_ONLINE_DWL_FB_SEQ_ID_ADD_AND_CHG_NUM
			  	  				);
			goto MODULE_END;
		}

		
		maxSegId = 0;
		for(loop = 1; loop <= MAX_LOGIC_TASK_NUM; loop++) {
			memoryClear( (uint8 *)&segTblInfo, sizeof(strFbTaskTblMemInfo));
		
   			(void)getFbTaskTblInfo( 
								LOGIC_ID,
           						loop,
								(strFbTaskTblMemInfo	*) &segTblInfo
								);
			if(segTblInfo.taskId > maxSegId)
				maxSegId = segTblInfo.taskId;
			
			if(segTblInfo.taskId == segmentId) break;
		}

		if(segTblInfo.taskId == 0) {
			status = ONLINE_DWL_SEG_EXIST_ERR;
       		setErrorCodeWithVal( 	__FILE__,
       								__LINE__,
           	    	        		"onlineDwlFbCodeSeqIdAddAndChgFuncInComTask", status,
                	   	    		"Segment ID",segmentId,
                 	  	    		"Current Max Seg Id", maxSegId,
                  	 	    		"NOT Using", 0 
				  		  		);
			goto MAKE_RESP; 	
		}
	
		
		
		
		for(fbSeqIdAddAndChgLoop = 0; fbSeqIdAddAndChgLoop < numOfSeqIdAddAndChgFbCode; fbSeqIdAddAndChgLoop++) {
			
			fbBlkAddr		= (*reqDataPtr >> 16) & 0xffff;
			fbId 			= *reqDataPtr++ & 0xffff;

			curSeqId 	= *reqDataPtr++;;
			
			
			if(fbId > MAX_FB_CODE_ID){

				status = L_FB_CODE_ERR;
	       		setErrorCodeWithVal( 	__FILE__,__LINE__,
        	   	    	        		"onlineDwlFbCodeSeqIdAddAndChgFuncInComTask",status,
                		   	    		"FB Code",fbId,
                 		  	    		"Max FB Code ", MAX_FB_CODE_ID,
                  	 		    		"Not Using", 0 
				  		  			);
			}				
			
			else if( fbId != gFbCodeSortingTableInfo[fbId].fbId){
				status = L_FB_CODE_ERR;
	       		setErrorCodeWithVal( 	__FILE__,__LINE__,
        	   	    	        		"onlineDwlFbCodeSeqIdAddAndChgFuncInComTask",status,
                		   	    		"FB Code",fbId,
                 		  	    		"Defined Fb Code ", gFbCodeSortingTableInfo[fbId].fbId,
                  	 		    		"Not Using", 0 
				  		  			);
			}				

			
			else if (curSeqId > MAX_FB_EXEC_SEQ_ID){
				status = FB_SEQ_ID_MAX_EXCEED_ERR;
	       		setErrorCodeWithVal( 	__FILE__,__LINE__,
        	   	    	        		"onlineDwlFbCodeSeqIdAddAndChgFuncInComTask",status,
                		   	    		"Req FB Code",fbId,
                 		  	    		"Request Blk Addr ", fbBlkAddr,
                  	 		    		"Max Seq ID", MAX_FB_EXEC_SEQ_ID 
				  		  			);
			}
			else if (curSeqId == 0x0){
				status = FB_SEQ_ID_NULL_ERR;
	       		setErrorCodeWithVal( 	__FILE__,__LINE__,
        	   	    	        		"onlineDwlFbCodeSeqIdAddAndChgFuncInComTask",status,
                		   	    		"Req FB Code",fbId,
                 		  	    		"Request Blk Addr ", fbBlkAddr,
                  	 		    		"Not Using", 0 
				  		  			);
			}
			if(status != NO_ERROR) goto MAKE_RESP;
			
		}	
		
			
MAKE_RESP:
		*respDataPtr 	= (status & 0xffff) << 16;
		*respDataPtr++	|= segmentId;
		*respRetLenPtr 	+= 4;	
		
		if(status != NO_ERROR) goto MODULE_END;
	} 
	
	
	if(status != NO_ERROR)
		goto MODULE_END;
			
	
	onlineFbSeqIdAddAndChgMngrInfoPtr->numOfTotalFbCode 	+= sumOfFbCodeNum;

	
	if(totalFrameNum == curFrameId) {
		 if((onlineFbSeqIdAddAndChgMngrInfoPtr->numOfTotalFbCode > MAX_ONLINE_DWL_FB_SEQ_ID_ADD_AND_CHG_NUM)||
		 	(onlineFbSeqIdAddAndChgMngrInfoPtr->numOfTotalFbCode == 0x0)||
			(onlineFbSeqIdAddAndChgMngrInfoPtr->dwlTotalFbCodeNum != onlineFbSeqIdAddAndChgMngrInfoPtr->numOfTotalFbCode)
		 ){
			status = FB_NUM_EXCEED_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,
               		        		"onlineDwlFbCodeSeqIdAddAndChgFuncInComTask",status,
                   	    			"SUM Fb Code Number",sumOfFbCodeNum,
                   	    			"Total FB   Number", onlineFbSeqIdAddAndChgMngrInfoPtr->numOfTotalFbCode,
                   	    			"Total FB Set  Number", onlineFbSeqIdAddAndChgMngrInfoPtr->dwlTotalFbCodeNum
				  	  			);
			goto MODULE_END;
		}		
	}		

		
	
	reqDataPtr = saveReqDataPtr;

	
	numOfSegment = *reqDataPtr++&0xffff;

	
	numOfTotalSeqIdAddAndChgFbCode = *reqDataPtr++;
	
	for(segSeqIdAddAndChgLoop = 0; segSeqIdAddAndChgLoop < numOfSegment; segSeqIdAddAndChgLoop++){
		
		
		taskLogicType   = (*reqDataPtr>> 31) & 0x1;	 
		segmentId 		= *reqDataPtr++ & 0xffff;
		
		
		numOfSeqIdAddAndChgFbCode	= *reqDataPtr++ & 0xffff;
		
		
		startBufIndex = 0;
		for(checkLoop = 0; checkLoop < MAX_ONLINE_DWL_SEG_SEQ_ID_ADD_AND_CHG_NUM; checkLoop++) {
			
			
			startBufIndex += onlineSegSeqIdAddAndChgInfoPtr->numOfFbCode;
			
			if((onlineSegSeqIdAddAndChgInfoPtr->taskId == 0) || (onlineSegSeqIdAddAndChgInfoPtr->taskId == segmentId))	
				break;
			
			onlineSegSeqIdAddAndChgInfoPtr++;	
		}
		if(checkLoop == MAX_ONLINE_DWL_SEG_SEQ_ID_ADD_AND_CHG_NUM) {
			status = FB_ONLINE_DOWNLOAD_ERR;
			setErrorCode (__FILE__,__LINE__,"onlineDwlFbCodeSeqIdAddAndChgFuncInComTask", status);
			goto MODULE_END;	
		}
		
		
		if(onlineSegSeqIdAddAndChgInfoPtr->taskId == 0)
			onlineSegSeqIdAddAndChgInfoPtr->startIndex = startBufIndex;
			
		onlineSegSeqIdAddAndChgInfoPtr->taskId 			= segmentId;
		onlineSegSeqIdAddAndChgInfoPtr->numOfFbCode 	+= numOfSeqIdAddAndChgFbCode;	

		if(onlineSegSeqIdAddAndChgInfoPtr->numOfFbCode  > MAX_ONLINE_DWL_FB_SEQ_ID_ADD_AND_CHG_NUM) {
			status = FB_NUM_EXCEED_ERR;
			setErrorCode (__FILE__,__LINE__,"onlineDwlFbCodeSeqIdAddAndChgFuncInComTask", status);
			goto MODULE_END;	
		}

		
		if( (startBufIndex + numOfSeqIdAddAndChgFbCode) > MAX_ONLINE_DWL_FB_SEQ_ID_ADD_AND_CHG_NUM) {
			status = FB_NUM_EXCEED_ERR;
       		setErrorCodeWithVal( 	__FILE__,
       								__LINE__,
               		        		"onlineDwlFbCodeSeqIdAddAndChgFuncInComTask",
                   		    		status,
                   	    			"Start Buf Index",startBufIndex,
                   	    			"Add Fb num", numOfSeqIdAddAndChgFbCode,
                   	    			"MAX FB NUM", MAX_ONLINE_DWL_FB_SEQ_ID_ADD_AND_CHG_NUM
				  	  			);
			goto MODULE_END;
		}		
					
		
		
		onlineFbSeqIdAddAndChgInfoPtr = orgOnlineFbSeqIdAddAndChgInfoPtr + startBufIndex;
		
		for(fbSeqIdAddAndChgLoop = 0; fbSeqIdAddAndChgLoop < numOfSeqIdAddAndChgFbCode; fbSeqIdAddAndChgLoop++,onlineFbSeqIdAddAndChgInfoPtr++){
			
			fbBlkAddr		= (*reqDataPtr >> 16) & 0xffff;
			fbId 			= *reqDataPtr++ & 0xffff;

			
			curSeqId 		= *reqDataPtr++;
						
			
			onlineFbSeqIdAddAndChgInfoPtr->fbId		= fbId;
			onlineFbSeqIdAddAndChgInfoPtr->fbBlkAddr 	= fbBlkAddr;
			onlineFbSeqIdAddAndChgInfoPtr->fbSeqId 		= curSeqId;
		}	
	} 
	
	
	retVal = onlineDwlReqDataSendToFbTask(ONLINE_DWL_FBCODE_SEQ_ID_ADD_AND_CHG,NULL, NULL);	

	if(retVal != NO_ERROR){
		setErrorCode (__FILE__, __LINE__, "onlineDwlFbCodeSeqIdAddAndChgFuncInComTask", retVal);
		status = FB_ONLINE_DOWNLOAD_ERR;
	}
		
MODULE_END:
	
	return(status);
}


uint32 fbSpecMemorySpaceCheck ( uint32	numOfMem)
{
	uint32	status = NO_ERROR , retVal = NO_ERROR;
	strFbSpecUnusedMemInfo	*curNodePtr, *saveNodePtr = NULL;
	uint32	usingMemSize = 0, unUsedMemSize = 0;
	uint32 memGap,maxMemGap = 0;
	
		
	status = semaphoreGet(sFbSpecMemUnusedQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "fbSpecMemorySpaceCheck",status);
		return(status);
	}
	
	
	
	retVal = onlineDwlDelFbMemorySpecCheck(numOfMem,FIND_SAME_MEM_SIZE);	
	if(retVal == NO_ERROR) {
		
		goto MODULE_END;
	}

	
	usingMemSize = numOfMem * 4;
	
	saveNodePtr = NULL;
	memGap = 0;
	maxMemGap = 0;
	
	curNodePtr = (strFbSpecUnusedMemInfo *)sFbSpecUnusedMemQMngrInfo.usedList;

	while(curNodePtr != NULL) {
		
		if(usingMemSize == curNodePtr->usableMemSize){ 
			saveNodePtr = curNodePtr;	
			break;
		}
		
		
		if(curNodePtr->usableMemSize > usingMemSize) {
			memGap = curNodePtr->usableMemSize - usingMemSize;	
			
			if(memGap > maxMemGap){
				maxMemGap = memGap;
				saveNodePtr = curNodePtr;
			}
		} 
		
		curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbSpecUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
                          									);	
	}		
	
	if(saveNodePtr != NULL) {
		
		saveNodePtr->usableMemSize -= usingMemSize;
		
		goto MODULE_END;	
	}	

	
	if ((curNodePtr == NULL) && (saveNodePtr == NULL)) {
		unUsedMemSize = 0;
		curNodePtr = (strFbSpecUnusedMemInfo *)sFbSpecUnusedMemQMngrInfo.usedList;
		
		while(curNodePtr != NULL) {
			unUsedMemSize += curNodePtr->usableMemSize;
		
			curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbSpecUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
                          									);	
		}		
	
		if( unUsedMemSize >= usingMemSize ){
			
			status =  FB_SPEC_MEM_EXCEED_ERR;
			setErrorCodeWithVal(	__FILE__, __LINE__, 
									"fbSpecMemorySpaceCheck", status,
									"Unused Spec MEM Size", unUsedMemSize,
									"Request SPEC MEM Size", usingMemSize,
									"Not Using",0
									);
		}
		else {
			
			retVal = onlineDwlDelFbMemorySpecCheck(numOfMem,FIND_GREATER_MEM_SIZE);	
			if(retVal != NO_ERROR) {
				
				
				
				status = SEG_MEM_EXCEED_ERR;
				setErrorCodeWithVal (	__FILE__,__LINE__,"fbSpecMemorySpaceCheck", status,
									"Request SPEC MEM Size", usingMemSize,
									"Unused Spec MEM Size", unUsedMemSize,
									"Not Using",0
								   );
			}
		}
	}
	
MODULE_END:	
#ifdef VXWORKS
	semaphoreRelease (sFbSpecMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbSpecMemUnusedQSemaId);	
#endif

	return(status);
}


static uint32 fbSpecMemorySpaceGet (	uint32	numOfMem, 
										strFbSpecUnusedMemInfo *retNodePtr
									)
{
	uint32	status = NO_ERROR;
	strFbSpecUnusedMemInfo	*curNodePtr, *saveNodePtr = NULL;
	uint32	usingMemSize;
	uint32 memGap,maxMemGap = 0;
	
	status = semaphoreGet(sFbSpecMemUnusedQSemaId, NOWAIT);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "fbSpecMemorySpaceGet",status);
		return(status);
	}
	
	usingMemSize = numOfMem * 4;
	
	
	curNodePtr = (strFbSpecUnusedMemInfo *)sFbSpecUnusedMemQMngrInfo.usedList;
	
	if(curNodePtr == NULL) {
			
		status = SEG_MEM_EXCEED_ERR;
		setErrorCode(	__FILE__,__LINE__,"fbSpecMemorySpaceGet", status);
		goto MODULE_END;	
	}

	while(curNodePtr != NULL) {
		
		if(curNodePtr->memUsingFlag != MEM_USING_IN_FUTURE)
			goto NEXT_NODE;
		
		if(usingMemSize == curNodePtr->freeMemSize){
			
			memoryCopy ( (uint8 *)retNodePtr,
						 (uint8 *)curNodePtr,
						 sizeof(strFbSpecUnusedMemInfo)
						);
						
						
			curNodePtr->memUsingFlag	= MEM_NOT_USING_IN_FUTURE;
			curNodePtr->reservedFbAddr	= 0;
			curNodePtr->reservedFbCode	= 0;

			
       		status = nodeUnlink( 	
   	        					(strDoubleLink **)&sFbSpecUnusedMemQMngrInfo.usedList,
   								(strDoubleLink *)curNodePtr
           						);
		                            		
			if(status != NO_ERROR)
				setErrorCode( __FILE__, __LINE__, "fbSpecMemorySpaceGet",status);
			else {
				
				status = nodePutOnLast( (strDoubleLink **) &sFbSpecUnusedMemQMngrInfo.freeList,
       	       							(strDoubleLink *)curNodePtr
               		  			 	 );
				if(status != NO_ERROR)
					setErrorCode( __FILE__, __LINE__, "fbSpecMemorySpaceGet",status);
			}
			goto MODULE_END;
		}
		
NEXT_NODE:		
		curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbSpecUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
                          									);	
	}		
	
	
	curNodePtr = (strFbSpecUnusedMemInfo *)sFbSpecUnusedMemQMngrInfo.usedList;
	if(curNodePtr == NULL) {
			
		status = SEG_MEM_EXCEED_ERR;
		setErrorCode(	__FILE__,__LINE__,"fbSpecMemorySpaceGet", status);
		goto MODULE_END;	
	}

	saveNodePtr = NULL;
	memGap = 0;
	maxMemGap = 0;
	
	while(curNodePtr != NULL) {
		
		if(curNodePtr->memUsingFlag == MEM_USING_IN_FUTURE)
			goto NEXT_NODE_1;
		
		if(usingMemSize == curNodePtr->freeMemSize){
			
			memoryCopy ( (uint8 *)retNodePtr,
						 (uint8 *)curNodePtr,
						 sizeof(strFbSpecUnusedMemInfo)
						);
						
			
       		status = nodeUnlink( 	
   	        					(strDoubleLink **)&sFbSpecUnusedMemQMngrInfo.usedList,
   								(strDoubleLink *)curNodePtr
           						);
		                            		
			if(status != NO_ERROR)
				setErrorCode( __FILE__, __LINE__, "fbSpecMemorySpaceGet",status);
			else {
				
				status = nodePutOnLast( (strDoubleLink **) &sFbSpecUnusedMemQMngrInfo.freeList,
       	       							(strDoubleLink *)curNodePtr
               		  			 	 );
				if(status != NO_ERROR)
					setErrorCode( __FILE__, __LINE__, "fbSpecMemorySpaceGet",status);
			}
			goto MODULE_END;
		}
		else {
			
			if(curNodePtr->freeMemSize > usingMemSize) {
				memGap = curNodePtr->freeMemSize - usingMemSize;	
			
				if(memGap > maxMemGap){
					maxMemGap = memGap;
					saveNodePtr = curNodePtr;
				}
			} 
		}
NEXT_NODE_1:		
		curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbSpecUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
                          									);	
	}		
	
	
	if(saveNodePtr != NULL) {
		
		retNodePtr->unUsedMemStartIndex	= saveNodePtr->unUsedMemStartIndex;
		retNodePtr->unUsedMemEndIndex	= saveNodePtr->unUsedMemStartIndex + numOfMem - 1; 

		retNodePtr->usableMemSize		= usingMemSize;
		retNodePtr->freeMemSize			= usingMemSize;
		
		
		saveNodePtr->unUsedMemStartIndex	+= numOfMem;

		saveNodePtr->usableMemSize			-= usingMemSize;
		saveNodePtr->freeMemSize			-= usingMemSize;
	}	
	else {
			
		
		
		setErrorCode(	__FILE__,__LINE__,"fbSpecMemorySpaceGet", NO_EXIST_NODE_ERR);
		status = FB_SPEC_MEM_EXCEED_ERR;
	}
MODULE_END:	
#ifdef VXWORKS
	semaphoreRelease (sFbSpecMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbSpecMemUnusedQSemaId);	
#endif

	return(status);
}


uint32	fbSpecUsableMemSet(void)
{
	uint32 status = NO_ERROR;
	strFbSpecUnusedMemInfo	*curNodePtr;
	

	status = semaphoreGet(sFbSpecMemUnusedQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "fbSpecUsableMemSet",status);
		return(status);
	}
	
	curNodePtr = (strFbSpecUnusedMemInfo *)sFbSpecUnusedMemQMngrInfo.usedList;

	
	if(curNodePtr == NULL) goto MODULE_END;

	while(curNodePtr != NULL) {
		curNodePtr->usableMemSize = curNodePtr->freeMemSize;
		
		
		curNodePtr->memUsingFlag	= MEM_NOT_USING_IN_FUTURE;
		curNodePtr->reservedFbAddr	= 0;
		curNodePtr->reservedFbCode	= 0;
		
		curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbSpecUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
                          									);	
	}		
	
MODULE_END:
#ifdef VXWORKS
	semaphoreRelease (sFbSpecMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbSpecMemUnusedQSemaId);	
#endif

	return(status);
}


static uint32	fbSpecFreeMemReallocCheck(void)
{
	uint32 status = NO_ERROR, freeSpaceNum = 0;
	strFbSpecUnusedMemInfo	*curNodePtr;
	
	status = semaphoreGet(sFbSpecMemUnusedQSemaId, NOWAIT);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "fbSpecFreeMemReallocCheck",status);
		return(status);
	}
	
	curNodePtr = (strFbSpecUnusedMemInfo *)sFbSpecUnusedMemQMngrInfo.usedList;
	
	if(curNodePtr == NULL){
		 
		status = SPEC_FREE_SPECE_NULL_ERR;
		goto MODULE_END;
	}
	
	freeSpaceNum = 0;
	while(curNodePtr != NULL) {
		freeSpaceNum++;
		
		curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbSpecUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
                          									);	
	}		
	
		
	if(freeSpaceNum == 0x1) 
		status = FB_PARA_MEM_REALLOC_NO_NEED;
		
MODULE_END:
#ifdef VXWORKS
	semaphoreRelease (sFbSpecMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbSpecMemUnusedQSemaId);	
#endif

	return(status);
}


static uint32	fbSpecMemRealloc()
{
	uint32 status = NO_ERROR, freeSpaceNum, tmpStatus;
	strFbSpecUnusedMemInfo	*curNodePtr, *miniNodePtr, *nextNodePtr = NULL;
	uint32	loop, loopCount;
	strFbTaskTblMemInfo  segTblInfo;
	uint32	blockAddr, specReallocEnable = 0;
    strFbMemInfo *fbMemInfoPtr;
   	uint32 offSet, calcuStartIndex, calcuEndIndex, copyNum;
	uint32	*srcDataPtr, *destDataPtr;
	uint32	numOfCheckFbCode, checkCount = 0;
	uint16 checkFbBlkAddrList[MAX_FB_LIST_NUM+1];

	uint32 taskId;
	strModuleExecInfo sysExeInfo;
	uint32	checkSumCalcuNeedFlag = 0;
	
	status = semaphoreGet(sFbSpecMemUnusedQSemaId, NOWAIT);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "fbSpecMemRealloc",status);
		return(status);
	}
	
	
	
	numOfCheckFbCode = 0;
	memoryClear ( (uint8 *) &checkFbBlkAddrList[0], MAX_FB_LIST_NUM*2);
						
	numOfCheckFbCode = 0;
	for(blockAddr = 1; blockAddr <= MAX_MPU_BLOCK_ADDR; blockAddr++){

		fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (blockAddr-1); 		
		
		if(fbMemInfoPtr->fbId == 0x0) continue;

		checkFbBlkAddrList[numOfCheckFbCode] = blockAddr;	
		numOfCheckFbCode += 1;
				
		
		if((numOfCheckFbCode >= MAX_FB_LIST_NUM) || (numOfCheckFbCode == 0x0)){
			status = DB_INDEX_EXCEED_ERR;
			setErrorCode (__FILE__, __LINE__, "fbSpecMemRealloc", status);
			goto MODULE_END;	
		}		
	}

	for(;;) {
 		sFbCodeAddrListInfo.numOfFbCode 	= 0;
 		sFbCodeAddrListInfo.startParaIndex 	= 0;
 		sFbCodeAddrListInfo.endParaIndex 	= 0;

		specReallocEnable = 0;
		curNodePtr = (strFbSpecUnusedMemInfo *)sFbSpecUnusedMemQMngrInfo.usedList;
	
		if(curNodePtr == NULL){
			 
			tmpStatus = SPEC_FREE_SPECE_NULL_ERR;;
			setErrorCode(	__FILE__,__LINE__, "fbSpecMemRealloc", tmpStatus);
			goto MODULE_END;
		}
	
		freeSpaceNum = 0;

		miniNodePtr = curNodePtr;
		
		while(curNodePtr != NULL) {
			curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbSpecUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
          	                									);	
			
			if(curNodePtr == NULL) break;
			
			
			if(miniNodePtr->unUsedMemEndIndex > curNodePtr->unUsedMemEndIndex)
				miniNodePtr = curNodePtr;	
			
			freeSpaceNum++;
		}		
	
		
			
		if(freeSpaceNum == 0x0){
			setErrorCodeWithVal (	__FILE__,__LINE__, "fbSpecMemRealloc", FB_PARA_MEM_REALLOC_NO_NEED,
								"Mini Start Index",miniNodePtr->unUsedMemStartIndex,
								"Mini End Index",miniNodePtr->unUsedMemEndIndex,
								"NOT Using", 0 
								  );
			goto MODULE_END;							  
		}

		
		curNodePtr	= (strFbSpecUnusedMemInfo *)sFbSpecUnusedMemQMngrInfo.usedList;
		nextNodePtr = NULL;
		
		while(curNodePtr != NULL) {
			if(curNodePtr != miniNodePtr) {
				if(nextNodePtr == NULL)
					nextNodePtr = curNodePtr;
				else if( nextNodePtr->unUsedMemEndIndex > curNodePtr->unUsedMemEndIndex)
					nextNodePtr = curNodePtr;	
			}
			curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbSpecUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
          	                									);	
		}		

		if(nextNodePtr == NULL){
			tmpStatus = FB_PARA_MEM_REALLOC_NO_NEED;
			setErrorCodeWithVal (	__FILE__,__LINE__, "fbSpecMemRealloc", tmpStatus,
								"Mini Start Index",miniNodePtr->unUsedMemStartIndex,
								"Mini End Index",miniNodePtr->unUsedMemEndIndex,
								"NOT Using", 0 
								  );
			goto MODULE_END;							  
		}
		
		
							
		loopCount = 0;	
		for(blockAddr = 1; blockAddr<=MAX_MPU_BLOCK_ADDR; blockAddr++){
			
			fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (blockAddr-1); 		
		
			if(fbMemInfoPtr->fbId == 0x0) continue;
		
			if((miniNodePtr->unUsedMemEndIndex+1) == fbMemInfoPtr->specActionIndex){
				sFbCodeAddrListInfo.startParaIndex = fbMemInfoPtr->specActionIndex;
				loopCount 	= 1;
				break;
			}
		}	
	
		if(loopCount == 0){
			tmpStatus = FB_PARA_MEM_REALLOC_NO_NEED;
			setErrorCodeWithVal (	__FILE__,__LINE__, "fbSpecMemRealloc", tmpStatus,
									"Mini Start Index",miniNodePtr->unUsedMemStartIndex,
									"Mini End Index",miniNodePtr->unUsedMemEndIndex,
									"NOT Using", 0 
								  );
			goto MODULE_END;							  
		}
	
							
		loopCount = 0;
		for(blockAddr = 1; blockAddr<=MAX_MPU_BLOCK_ADDR; blockAddr++){
			
			fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (blockAddr-1); 		
		
			if(fbMemInfoPtr->fbId == 0x0) continue;

			if(nextNodePtr->unUsedMemStartIndex == (fbMemInfoPtr->varIndex+fbMemInfoPtr->intlVarSize))
			{
				sFbCodeAddrListInfo.endParaIndex = nextNodePtr->unUsedMemStartIndex - 1;
				loopCount = 1;
				break;
			}			
		}

		if(loopCount == 0){
			tmpStatus = FB_PARA_MEM_REALLOC_NO_NEED;
			setErrorCodeWithVal (	__FILE__,__LINE__, "fbSpecMemRealloc", tmpStatus,
									"Mini Start Index",miniNodePtr->unUsedMemStartIndex,
									"Mini End Index",miniNodePtr->unUsedMemEndIndex,
									"NOT Using", 0 
								  );
			goto MODULE_END;							  
		}

								
		offSet 						= 0;
		calcuStartIndex 			= miniNodePtr->unUsedMemEndIndex + 1;
		calcuEndIndex				= nextNodePtr->unUsedMemStartIndex;

		checkCount = 0;

		
		
		while(numOfCheckFbCode != 0x0) {
			
			blockAddr = checkFbBlkAddrList[checkCount];
			
			fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (blockAddr-1); 		
						
			if(fbMemInfoPtr->fbId == 0x0){
				if(checkCount++ > MAX_FB_LIST_NUM)
					checkCount = 0;
					
			 	continue;
			}
			if((calcuStartIndex == fbMemInfoPtr->specActionIndex) && (blockAddr != 0x0) ){
				
				
				checkFbBlkAddrList[checkCount] = 0x0;
				numOfCheckFbCode -= 1;
				
				
				sFbCodeAddrListInfo.fbCodeBlkAddr[sFbCodeAddrListInfo.numOfFbCode] = blockAddr;
				sFbCodeAddrListInfo.numOfFbCode += 1;

				
				if(sFbCodeAddrListInfo.numOfFbCode >= MAX_FB_LIST_NUM) {
					status = DB_INDEX_EXCEED_ERR;
					setErrorCode (__FILE__, __LINE__, "fbSpecMemRealloc", status);
					goto MODULE_END;	
				}

				offSet = fbMemInfoPtr->inputNo + fbMemInfoPtr->inputSize +fbMemInfoPtr->intlVarSize;
				calcuStartIndex = fbMemInfoPtr->specActionIndex + offSet;
			
				
				if(calcuEndIndex == (fbMemInfoPtr->specActionIndex + offSet)){
					specReallocEnable 		= 1;
					
					
					checkSumCalcuNeedFlag	= 1;
					break;
				}
			} 

			if(checkCount++ > MAX_FB_LIST_NUM) {
				checkCount = 0;
				continue;
			}
		} 

		if(specReallocEnable == 1) {
			
			nextNodePtr->unUsedMemStartIndex 	-= miniNodePtr->freeMemSize/4;
			nextNodePtr->freeMemSize 			+= miniNodePtr->freeMemSize;
			nextNodePtr->usableMemSize			= nextNodePtr->freeMemSize;
			
			
   	    	status = nodeUnlink( 	
   	        					(strDoubleLink **)&sFbSpecUnusedMemQMngrInfo.usedList,
   								(strDoubleLink *)miniNodePtr
           						);
		                            		
			if(status != NO_ERROR){
   				setErrorCode( __FILE__, __LINE__, "fbSpecMemRealloc",status);
				goto MODULE_END;	
			}
			
			status = nodePutOnLast( (strDoubleLink **) &sFbSpecUnusedMemQMngrInfo.freeList,
       	       						(strDoubleLink *)miniNodePtr
               		  			  );
			if(status != NO_ERROR){
   				setErrorCode( __FILE__, __LINE__, "fbSpecMemRealloc",status);
				goto MODULE_END;	
			}
			
			
			
			
			srcDataPtr 	= (uint32 *)((uint32 *)gMpuSpecAndVarDataLocalPtr + sFbCodeAddrListInfo.startParaIndex);
			destDataPtr = (uint32 *)((uint32 *)gMpuSpecAndVarDataLocalPtr + miniNodePtr->unUsedMemStartIndex);
			
			copyNum = sFbCodeAddrListInfo.endParaIndex - sFbCodeAddrListInfo.startParaIndex + 1;
			
			for(loop = 0; loop < copyNum; loop++)
				*destDataPtr++ = *srcDataPtr++;
				
			
			srcDataPtr 	= (uint32 *)((uint32 *)gMpuSpecAndVarDataBusPtr + sFbCodeAddrListInfo.startParaIndex);
			destDataPtr = (uint32 *)((uint32 *)gMpuSpecAndVarDataBusPtr + miniNodePtr->unUsedMemStartIndex);
			
			copyNum = sFbCodeAddrListInfo.endParaIndex - sFbCodeAddrListInfo.startParaIndex + 1;
			
			for(loop = 0; loop < copyNum; loop++)
				*destDataPtr++ = *srcDataPtr++;
				
				
			for(loop = 0; loop < sFbCodeAddrListInfo.numOfFbCode; loop++) {
				blockAddr = sFbCodeAddrListInfo.fbCodeBlkAddr[loop];

				
				fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (blockAddr-1);			
				fbMemInfoPtr->specActionIndex -= miniNodePtr->freeMemSize/4;
				fbMemInfoPtr->specIndex -= miniNodePtr->freeMemSize/4;
				fbMemInfoPtr->varIndex 	-= miniNodePtr->freeMemSize/4;

				
				fbMemInfoPtr = gMpuFbMemInfoBusPtr + (blockAddr-1);
				fbMemInfoPtr->specActionIndex -= miniNodePtr->freeMemSize/4;
				fbMemInfoPtr->specIndex -= miniNodePtr->freeMemSize/4;
				fbMemInfoPtr->varIndex 	-= miniNodePtr->freeMemSize/4;
			}

			
			specReallocEnable = 0;
			
			sFbCodeAddrListInfo.numOfFbCode 	= 0;
 			sFbCodeAddrListInfo.startParaIndex 	= 0;
 			sFbCodeAddrListInfo.endParaIndex 	= 0;
		}
		else {
			
			
			status = FB_SPEC_MEM_REALLOC_ERR;
			setErrorCode (__FILE__, __LINE__, "fbSpecMemRealloc", status);
			goto MODULE_END;	
		}
	}

MODULE_END:

	
	if(checkSumCalcuNeedFlag) {
		
		memoryClear( (uint8 *)&sysExeInfo, sizeof(strModuleExecInfo));
	
		status = fbSysExeModuleInfoRead(LOGIC_ID, &sysExeInfo);
		if(status!=NO_ERROR){
			setErrorCode((uint8 *)__FILE__, __LINE__, (uint8 *)"fbSpecMemRealloc", status);
			goto MODULE_END;	
		}
		
		for(taskId=1 ;taskId<=sysExeInfo.curMaxSegmentId ;taskId++)
		{	
			if( (getFbTaskTblInfo(LOGIC_ID,taskId,&segTblInfo) == NO_ERROR) 
					&& (segTblInfo.taskId == taskId) )	{
				(void)onlineDwlSegmentCheckSumCalcu(taskId,ONLINE_FB_PARA_REALLOC, 0,0);
			}
			else continue;
		}
	}

#ifdef VXWORKS
	semaphoreRelease (sFbSpecMemUnusedQSemaId);	
#else
	semaphoreRelease2 (__FILE__,__LINE__,MUTEX_SEMA,sFbSpecMemUnusedQSemaId);	
#endif

	return(status);
}


static uint32 onlineDwlFbParaMemReallocInFbTask(strOnlineDwlTaskComReqInfo *argPtr)
{
	uint32	status = NO_ERROR, retVal;
	uint32  paraMemKind = 0; 

	paraMemKind = argPtr->reqDataBuf[0];
	if( (paraMemKind <INPUT_PARA_TYPE) && (paraMemKind >ALL_FB_PARA_TYPE) ){
		status = L_PARA_TYPE_ERR;
		setErrorCodeWithVal(	__FILE__,__LINE__,"onlineDwlFbParaMemReallocInFbTask",status,
								"FB PARA Kind", paraMemKind,
								"Not Usting", 0,
								"Not Usting", 0
							);
		return(status);
	}

	if( (paraMemKind == ALL_FB_PARA_TYPE)||(paraMemKind == INPUT_PARA_TYPE) ){
		
		retVal = fbSpecFreeMemReallocCheck();
		if(retVal != NO_ERROR) {
			
			
			
			setErrorCode(__FILE__, __LINE__, "onlineDwlFbParaMemReallocInFbTask",retVal);
			status = NO_ERROR;				
			return(status);	
		}
	
		
		status = fbSpecMemRealloc();
		if(status) {
			
			status = FB_SPEC_MEM_REALLOC_ERR;
			setErrorCode(__FILE__, __LINE__, "onlineDwlFbParaMemReallocInFbTask",status);
							
			return(status);
		}	
	}

	if( (paraMemKind == ALL_FB_PARA_TYPE)||(paraMemKind == OUTPUT_PARA_TYPE) ){
		
		retVal = fbOutputFreeMemReallocCheck();
		if(retVal != NO_ERROR) {
			
			
			
			setErrorCode(__FILE__, __LINE__, "onlineDwlFbParaMemReallocInFbTask",retVal);
			status = NO_ERROR;				
			return(status);	
		}
	
		
		status = fbOutputMemRealloc();
		if(status) {
			
			status = FB_OUTPUT_MEM_REALLOC_ERR;
			setErrorCode(__FILE__, __LINE__, "onlineDwlFbParaMemReallocInFbTask",status);
							
			return(status);
		}	
	}
	return(status);
}


void fbExecIdQueCountInfoDisplay(uint32 arg1, uint32 arg2)
{
	uint32	freeListCount = 0, usedListCount[MAX_LOGIC_TASK_NUM], totalUsedQueCount = 0;
	strFbExecutionInfo	*nodePtr;
	strFbTaskTblMemInfo	segTblInfo;
	uint32 taskId = 0, status = NO_ERROR, lastSegId = 0;

	memoryClear ( (uint8 *)	&usedListCount[0], MAX_LOGIC_TASK_NUM*4);

	printf( "\n");
	printf( "\t***************************************************\n");
	printf( "\t**             FBD EXEC Queue Count Info         **\n");
	printf( "\t***************************************************\n");
	
	printf( "\tMAX QUEUE COUNT                  : %05d\n",MAX_FB_EXEC_SEQ_ID_NUM);
	
	nodePtr = sFbExecQMngrInfo.freeList;

	
    status = semaphoreGet(gFbExecSeqIdQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
		return;
	}

    while(nodePtr != NULL) {
    	freeListCount++;

        nodePtr = (strFbExecutionInfo *)nextNodeRef ( 	(strDoubleLink *)sFbExecQMngrInfo.freeList,
	                        							(strDoubleLink *)nodePtr
							  						);
	}

	for(taskId = 1; taskId <= MAX_LOGIC_TASK_NUM; taskId++) {
		usedListCount[taskId - 1] = 0;
   		status = getFbTaskTblInfo	( LOGIC_ID, taskId, (strFbTaskTblMemInfo	*) &segTblInfo);
		if(status != NO_ERROR){
    	    printf( "[ERROR] - Seg ID %d Read Error\n", taskId); 
			goto MODULE_END;		
		}	

			
		if(segTblInfo.fbUsedNum == 0) continue; 
		nodePtr = sFbExecQMngrInfo.usedList[taskId - 1];
    	while(nodePtr != NULL) {
    		usedListCount[taskId - 1]++;
			lastSegId = taskId;
        	nodePtr = (strFbExecutionInfo *)nextNodeRef ( 	(strDoubleLink *)sFbExecQMngrInfo.usedList[taskId - 1],
	        	                							(strDoubleLink *)nodePtr
								  						);
		}
		totalUsedQueCount += usedListCount[taskId - 1];						  						
	}

MODULE_END:
	
#ifdef VXWORKS
	semaphoreRelease (gFbExecSeqIdQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gFbExecSeqIdQSemaId);	
#endif

	printf( "\tFree List Queue Count            : %05d\n",freeListCount);

	for(taskId = 1; taskId <= lastSegId; taskId++){
		printf( "\t[SEG_ID ::%03d] Used Queue Count  : %05d\n",taskId, usedListCount[taskId - 1]);
	}	
	printf( "\tTotal Used Queue Count           : %05d\n",totalUsedQueCount);



	return;
}


void fbExecIdQueInfoDisplay(uint32 arg1, uint32 arg2)
{
	uint32 taskId, startId, endId, printLoop, loopCount = 0, status = NO_ERROR;
	int8	line[16], *cPtr;
	strFbExecutionInfo	*nodePtr, saveData[MAX_FB_EXEC_SEQ_ID_DISPLAY_NUM], *saveDataPtr;
	strFbTaskTblMemInfo segTblInfo;

	printf("\t Enter Logic Task ID : ");
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    taskId = atoi(cPtr);

	if( (taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)) 
	{
		printf( "[ERROR] - Task ID Error !!\n");
		return;	
	}

	status = getFbTaskTblInfo(LOGIC_ID,taskId,&segTblInfo);
	if(status != NO_ERROR) 
	{
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
		return;
	}

	if(segTblInfo.logicType == SFC_LOGIC_TYPE) 
	{
		printf("\t Task#%03d  is SFC Logic Type!!!",taskId);
		return;
	}
	
	printf("\t Enter Start FB Executon ID : ");
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    startId = atoi(cPtr);

	printf("\t Enter End FB Executon ID   : ");
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    endId = atoi(cPtr);
	if(endId == 0) 
	{
		printf( "[ERROR] - End ID must be greater than 0\n");
		return;	
	}
    
    if(startId >= endId) 
    {
		printf( "[ERROR] - End ID must be greater than Start ID !!\n");
		return;	
    }
    
    if( (endId - startId + 1) > MAX_FB_EXEC_SEQ_ID_DISPLAY_NUM)
    {
		printf( "[ERROR] - The Number of Display(%d) is greater than MAX Number[%d]\n",
								(endId - startId + 1), MAX_FB_EXEC_SEQ_ID_DISPLAY_NUM);
		return;	
	}

 	
    status = semaphoreGet(gFbExecSeqIdQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR) 
	{
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
		return;
	}

	nodePtr = sFbExecQMngrInfo.usedList[taskId - 1];
	
  	while(nodePtr != NULL) 
  	{
		if(nodePtr->fbExecId == startId)
			break;
	   	nodePtr = (strFbExecutionInfo *)nextNodeRef ( 	(strDoubleLink *)sFbExecQMngrInfo.usedList[taskId - 1],
        	                							(strDoubleLink *)nodePtr
							  						);
	}

	if(nodePtr == NULL) 
	{
		printf( "[ERROR] - NO FB Code Exist !!\n");
		goto MODULE_END;	
	}

	loopCount = 0;
	
	while(nodePtr != NULL) 
	{
		
  		memoryCopy( (uint8 *) &saveData[loopCount], 
  					(uint8 *)nodePtr, 
  					sizeof(strFbExecutionInfo)
  				  );
  				  
		if( ++loopCount >= (endId-startId +1))
			 break;
		
	   	nodePtr = (strFbExecutionInfo *)nextNodeRef ( 	(strDoubleLink *)sFbExecQMngrInfo.usedList[taskId - 1],
        	                							(strDoubleLink *)nodePtr
							  						);
	}
	
MODULE_END:
	
#ifdef VXWORKS
	semaphoreRelease (gFbExecSeqIdQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gFbExecSeqIdQSemaId);	
#endif

	printf("+=====+========+========+=========+=====+========+========+=========+\n");
 	printf("| ID# | TSK_ID | FB_ADR | FB CODE | ID# | TSK_ID | FB_ADR | FB CODE |\n");
 	printf("+=====+========+========+=========+=====+========+========+=========+\n");

	printLoop = 0;
	saveDataPtr = &saveData[0];
	
	for(printLoop = 0; printLoop < loopCount; printLoop++, saveDataPtr++) {
	
		if((printLoop%2) == 0x0)
			printf("|%05d|%8d|%8d|%9d|", 	saveDataPtr->fbExecId,
															saveDataPtr->taskId,
															saveDataPtr->fbBlockAddr,
															saveDataPtr->fbId
					);
		if(printLoop%2)
			printf("%05d|%8d|%8d|%9d|\n", saveDataPtr->fbExecId,
															saveDataPtr->taskId,
															saveDataPtr->fbBlockAddr,
															saveDataPtr->fbId
					);
	}
	if(loopCount%2) printf( "\n");
 	printf("+=====+========+========+=========+=====+========+========+=========+\n");
}


void fbSpecUnusedMemInfoDisplay(uint32 arg1, uint32 arg2)
{
	uint32 status = NO_ERROR, blockAddr;
	uint32 calcuUnusedMemSize,calcuFbSpecMemSize,calcuUsableMemSize;

	strFbMemInfo 		fbMemInfo;
	strFbSpecUnusedMemInfo	*nodePtr;
	strModuleExecInfo sysExeInfo;
	uint32 calcuUnusedOutMemSize,calcuFbOutMemSize,calcuUsableOutMemSize;

	status = fbSysExeModuleInfoRead(LOGIC_ID, &sysExeInfo);
	if(status != NO_ERROR) return;

	calcuFbSpecMemSize = 0;
	calcuFbOutMemSize = 0;
	
	for(blockAddr = 1; blockAddr <= sysExeInfo.curfbMaxBlockAddr ;	blockAddr++)
	{ 
		status = fbMemInfoRead(LOGIC_ID, blockAddr, &fbMemInfo);
		if(status == NO_ERROR) {
			if(fbMemInfo.fbId == 0) continue;
		
			
			calcuFbSpecMemSize += (fbMemInfo.inputNo + fbMemInfo.inputSize + fbMemInfo.intlVarSize) * 4;
			calcuFbOutMemSize += (fbMemInfo.outputSize) * 4;
		}
		else 
		{
	        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
			 return;
		}
	}

    status = semaphoreGet(sFbSpecMemUnusedQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) 
	{
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
		return;
	}

 	nodePtr = sFbSpecUnusedMemQMngrInfo.usedList;
    
    calcuUnusedMemSize = 0;
    calcuUsableMemSize = 0;
    	
   	while(nodePtr != NULL) 
	{
 		calcuUnusedMemSize += nodePtr->freeMemSize;										  							
		calcuUsableMemSize += nodePtr->freeMemSize;
		
      	nodePtr = (strFbSpecUnusedMemInfo *)nextNodeRef ( 	
       								(strDoubleLink *)sFbSpecUnusedMemQMngrInfo.usedList,
	       	                		(strDoubleLink *)nodePtr
							  							);
	}
	
#ifdef VXWORKS
	semaphoreRelease (sFbSpecMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbSpecMemUnusedQSemaId);	
#endif

	printf( "\t***************************************************\n");
	printf( "\t**              FB INPUT Memory Info             **\n");
	printf( "\t***************************************************\n");

	printf( "\tMAX INPUT MEM NUM(SIZE)         : %05d(%05d)\n",MAX_MPU_SPEC_NUM,
																			MAX_MPU_SPEC_NUM*4);
	printf( "\tUSED INPUT MEM SIZE             : %05d(%05d)\n",sysExeInfo.curfbMaxParaIndex,
																			sysExeInfo.curfbMaxParaIndex*4);
	printf( "\tCALCU INPUT MEM SIZE            : %05d\n",calcuFbSpecMemSize);
	printf( "\tUSABLE INPUT MEM SIZE           : %05d\n",calcuUsableMemSize);
	printf( "\tUNUSED INPUT MEM SIZE           : %05d\n",calcuUnusedMemSize);
	
    status = semaphoreGet(sFbOutputMemUnusedQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) 
	{
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
		return;
	}

 	nodePtr = sFbOutputUnusedMemQMngrInfo.usedList;
    calcuUnusedOutMemSize = 0;
    calcuUsableOutMemSize = 0;
    	
   	while(nodePtr != NULL) 
   	{
 		calcuUnusedOutMemSize += nodePtr->freeMemSize;										  							
		calcuUsableOutMemSize += nodePtr->freeMemSize;
		
      	nodePtr = (strFbSpecUnusedMemInfo *)nextNodeRef ( 	
       								(strDoubleLink *)sFbOutputUnusedMemQMngrInfo.usedList,
	       	                		(strDoubleLink *)nodePtr
							  							);
	}
	
#ifdef VXWORKS
	semaphoreRelease (sFbOutputMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbOutputMemUnusedQSemaId);	
#endif

	printf("\n");
	printf( "\t***************************************************\n");
	printf( "\t**             FB OUTPUT Memory Info             **\n");
	printf( "\t***************************************************\n");
	
	printf( "\tMAX OUTPUT MEM NUM(SIZE)        : %05d(%05d)\n",MAX_MPU_OUTPUT_NUM,
																			MAX_MPU_OUTPUT_NUM*4);
	printf( "\tUSED OUTPUT MEM NUM(SIZE)       : %05d(%05d)\n",sysExeInfo.curfbMaxOutIndex,
																			sysExeInfo.curfbMaxOutIndex*4);
	printf( "\tCALCU OUTPUT MEM SIZE           : %05d\n",calcuFbOutMemSize);
	printf( "\tUSABLE OUTPUT MEM SIZE          : %05d\n",calcuUsableOutMemSize);
	printf( "\tUNUSED OUTPUT MEM SIZE          : %05d\n",calcuUnusedOutMemSize);

	return;
}


void fbUnusedSpecMemQueInfoDisplay(uint32 arg1, uint32 arg2)
{
	uint32 status = NO_ERROR, loop;
	strFbSpecUnusedMemInfo	*nodePtr;
	uint8	*memUsingString[] = {"MEM IS NOT RESERVED","MEM IS RESERVED", "UNKNOWN"};
	uint32	strIndex;

	
    status = semaphoreGet(sFbSpecMemUnusedQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) 
	{
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
		return;
	}

	printf( "\t***************************************************\n");
	printf( "\t**               FB Input Queue Info             **\n");
	printf( "\t***************************************************\n");
	
 	nodePtr = sFbSpecUnusedMemQMngrInfo.usedList;
    if(nodePtr == NULL) 
    {
    	printf( "\t There is NO Unused Spec Memory !!\n");
    	goto MODULE_END;
    }
    
    loop = 0;
    
   	while(nodePtr != NULL) 
   	{
		printf("\n\tIndex : %d\n", ++loop);
		printf( "\tUNUSED MEM START INDEX      : %05d\n",nodePtr->unUsedMemStartIndex);
		printf( "\tUNUSED MEM END INDEX        : %05d\n",nodePtr->unUsedMemEndIndex);
		printf( "\tUSABLE MEM END SIZE         : %05d\n",nodePtr->usableMemSize);
		printf( "\tUNUSED SPEC MEM SIZE        : %05d\n",nodePtr->freeMemSize);

		if(	(nodePtr->memUsingFlag != MEM_NOT_USING_IN_FUTURE) &&
			(nodePtr->memUsingFlag != MEM_USING_IN_FUTURE))
			strIndex = 2;
		else
			strIndex = nodePtr->memUsingFlag; 
		printf( "\tMEM Using Flag String       : %s\n",memUsingString[strIndex]);
		printf( "\tMEM Using FB Addr           : %05d\n",nodePtr->reservedFbAddr);
		printf( "\tMEM Using FB Code           : %05d\n",nodePtr->reservedFbCode);

       	nodePtr = (strFbSpecUnusedMemInfo *)nextNodeRef ( 	
       								(strDoubleLink *)sFbSpecUnusedMemQMngrInfo.usedList,
	       	                		(strDoubleLink *)nodePtr
							  							);
	}

MODULE_END:
	
#ifdef VXWORKS
	semaphoreRelease (sFbSpecMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbSpecMemUnusedQSemaId);	
#endif
	return;
}


void fbUnusedOutputMemQueInfoDisplay(uint32 arg1, uint32 arg2)
{
	uint32 status = NO_ERROR, loop;
	strFbSpecUnusedMemInfo	*nodePtr;
	uint8	*memUsingString[] = {"MEM IS NOT RESERVED","MEM IS RESERVED", "UNKNOWN"};
	uint32	strIndex;

	
    status = semaphoreGet(sFbOutputMemUnusedQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
		return;
	}

	printf( "\t***************************************************\n");
	printf( "\t**              FB Output Queue Info             **\n");
	printf( "\t***************************************************\n");
	
 	nodePtr = sFbOutputUnusedMemQMngrInfo.usedList;
    if(nodePtr == NULL) {
    	printf( "\t There is NO Unused Output Memory !!\n");
    	goto MODULE_END;
    }
    loop = 0;
   	while(nodePtr != NULL) {
		printf("\n\tIndex : %d\n", ++loop);
		printf( "\tUNUSED MEM START INDEX      : %05d\n",nodePtr->unUsedMemStartIndex);
		printf( "\tUNUSED MEM END INDEX        : %05d\n",nodePtr->unUsedMemEndIndex);
		printf( "\tUSABLE MEM END SIZE         : %05d\n",nodePtr->usableMemSize);
		printf( "\tUNUSED SPEC MEM SIZE        : %05d\n",nodePtr->freeMemSize);

		if(	(nodePtr->memUsingFlag != MEM_NOT_USING_IN_FUTURE) &&
			(nodePtr->memUsingFlag != MEM_USING_IN_FUTURE))
			strIndex = 2;
		else
			strIndex = nodePtr->memUsingFlag; 
		printf( "\tMEM Using Flag String       : %s\n",memUsingString[strIndex]);
		printf( "\tMEM Using FB Addr           : %05d\n",nodePtr->reservedFbAddr);
		printf( "\tMEM Using FB Code           : %05d\n",nodePtr->reservedFbCode);

       	nodePtr = (strFbSpecUnusedMemInfo *)nextNodeRef ( 	
       								(strDoubleLink *)sFbOutputUnusedMemQMngrInfo.usedList,
	       	                		(strDoubleLink *)nodePtr
							  							);
	}

MODULE_END:
	
#ifdef VXWORKS
	semaphoreRelease (sFbOutputMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbOutputMemUnusedQSemaId);	
#endif
	return;
}


void onlineDwlfbCodeAddInfoDisplay(void)
{
	strOnlineDwlAddFbCodeMngrInfo	*onlineFbAddMngrInfoPtr = &sOnlineDwlFbAddInfo;
	strOnlineDwlAddSegInfo			*onlineSegAddInfoPtr = onlineFbAddMngrInfoPtr->addSegInfoPtr;
	strOnlineDwlAddFbCodeInfo		*onlineFbAddInfoPtr;
	uint32	fbLoop, specLoop, segLoop, *specDataPtr;
	uint32	strIndex;
	
	printf( "************************************************************\n");	
	printf( "**		FB Code ADD Info Display\n");
	printf( "************************************************************\n");
	
	printf( "Total Add TAKS Number    : %d\n",onlineFbAddMngrInfoPtr->numOfTotalSegment);
	printf( "Total Add FB Code Number : %d\n",onlineFbAddMngrInfoPtr->numOfTotalFbCode);
	
	for(segLoop = 0; segLoop < onlineFbAddMngrInfoPtr->numOfTotalSegment; segLoop++, onlineSegAddInfoPtr++) {
		printf( "TAKS ID        : %d\n",onlineSegAddInfoPtr->taskId);
		printf( "Num Of FB Code : %d\n",onlineSegAddInfoPtr->numOfFbCode);
	
		onlineFbAddInfoPtr = &onlineSegAddInfoPtr->addFbCodeInfo[0];
		
		for(fbLoop = 1; fbLoop <= onlineSegAddInfoPtr->numOfFbCode; fbLoop++,onlineFbAddInfoPtr++){
			if(	(onlineFbAddInfoPtr->memReservedFlag != MEM_NOT_USING_IN_FUTURE) &&
				(onlineFbAddInfoPtr->memReservedFlag != MEM_USING_IN_FUTURE))
				strIndex = 2;
			else
				strIndex = onlineFbAddInfoPtr->memReservedFlag; 

			printf("\n\n***** FB Code[%d]-BLOCK ADDR[%d]-INPUT NUM[%d]bba *****\n",
					onlineFbAddInfoPtr->fbId,onlineFbAddInfoPtr->fbBlkAddr,
					onlineFbAddInfoPtr->numOfSpec);
	
			specDataPtr = &onlineFbAddInfoPtr->specData[0];
			for(specLoop = 0; specLoop < onlineFbAddInfoPtr->numOfSpec; specLoop++, specDataPtr++) {
				printf( "INPUT HEADER[%02d] - 0x%08x",specLoop+1,*specDataPtr);
				if((specLoop%2)) printf( "\n");
				else printf( "\t");
			}
			printf( "\n");
			
			for(specLoop = 0; specLoop < onlineFbAddInfoPtr->numOfSize; specLoop++, specDataPtr++) {
				printf( "INPUT DATA[%02d] - 0x%08x",specLoop+1,*specDataPtr);
				if((specLoop%2))  printf( "\n");
				else printf( "\t");
			}
		}
	} 	 
}


void onlineDwlfbCodeDelInfoDisplay(void)
{
	strOnlineDwlDelFbCodeMngrInfo	*onlineFbDelMngrInfoPtr = &sOnlineDwlFbDelInfo;
	strOnlineDwlDelSegInfo			*onlineSegDelInfoPtr = onlineFbDelMngrInfoPtr->delSegInfoPtr;
	strOnlineDwlDelFbCodeInfo		*onlineFbDelInfoPtr;
	uint32	fbLoop, segLoop;
	
	printf( "************************************************************\n");	
	printf( "**		FB Code Del Info Display\n");
	printf( "************************************************************\n");
	
	printf( "Total Del Segment Number : %d\n",onlineFbDelMngrInfoPtr->numOfTotalSegment);
	printf( "Total Del FB Code Number : %d\n",onlineFbDelMngrInfoPtr->numOfTotalFbCode);
	
	for(segLoop = 0; segLoop < onlineFbDelMngrInfoPtr->numOfTotalSegment; segLoop++,onlineSegDelInfoPtr++) {
		printf( "Segment ID     : %d\n",onlineSegDelInfoPtr->taskId);
		printf( "Num OF FB Code : %d\n",onlineSegDelInfoPtr->numOfFbCode);
	
	 	onlineFbDelInfoPtr = &onlineSegDelInfoPtr->delFbCodeInfo[0];
	 
		for(fbLoop = 0; fbLoop < onlineSegDelInfoPtr->numOfFbCode; fbLoop++,onlineFbDelInfoPtr++){
			printf("\nFB Code[%03d]-BLOCK ADDR[%05d]",
						onlineFbDelInfoPtr->fbId,onlineFbDelInfoPtr->fbBlkAddr);
		
			if((fbLoop != 0) && !(fbLoop%4))
				printf( "\n");
		}

		printf( "\n");		
	} 	 
}


void onlineDwlDelfbCodeMemInfoDisplay(void)
{
	strOnlineDwlDelFbMemMngrInfo	*onlineFbDelMemMngrInfoPtr = &sOnlineDwlFbDelMemInfo;
	strOnlineDwlDelSegMemInfo		*onlineSegDelMemInfoPtr = &onlineFbDelMemMngrInfoPtr->delSegMemInfo;
	strOnlineDwlDelFbMemInfo		*onlineFbDelMemInfoPtr;
	uint32	fbLoop, segLoop;
	
	printf( "************************************************************\n");	
	printf( "**		FB Code Del Input/Output Info Display\n");
	printf( "************************************************************\n");
	
	for(segLoop = 0; segLoop < onlineFbDelMemMngrInfoPtr->numOfTotalSegment; segLoop++,onlineSegDelMemInfoPtr++) {
		printf( "Num OF FB Code : %d\n",onlineSegDelMemInfoPtr->numOfFbCode);
	
	 	onlineFbDelMemInfoPtr = &onlineSegDelMemInfoPtr->delFbMemInfo[0];
	 	for(fbLoop = 0; fbLoop < onlineSegDelMemInfoPtr->numOfFbCode; fbLoop++,onlineFbDelMemInfoPtr++){
			printf("\nFB Code[%03d]-ADDR[%05d]-TOTAL INPUT[%05d]",
						onlineFbDelMemInfoPtr->fbId,onlineFbDelMemInfoPtr->fbBlkAddr,
						onlineFbDelMemInfoPtr->totalSpecMemNum
						);
			printf("\n             -USED INPUT[%05d]-USABLE INPUT[%05d]",
						onlineFbDelMemInfoPtr->usedSpecMemNum,
						onlineFbDelMemInfoPtr->usableSpecMemNum
						);

			if((fbLoop != 0) && !(fbLoop%4))
				printf( "\n");
		}	
	
	 	onlineFbDelMemInfoPtr = &onlineSegDelMemInfoPtr->delFbMemInfo[0];
		for(fbLoop = 0; fbLoop < onlineSegDelMemInfoPtr->numOfFbCode; fbLoop++,onlineFbDelMemInfoPtr++){
			printf("\nFB Code[%03d]-ADDR[%05d]-TOTAL OUTPUT[%05d]",
						onlineFbDelMemInfoPtr->fbId,onlineFbDelMemInfoPtr->fbBlkAddr,
						onlineFbDelMemInfoPtr->totalOutputMemNum
						);
			printf("\n             -USED OUTPUT[%05d]-USABLE OUTPUT[%05d]",
						onlineFbDelMemInfoPtr->usedOutputMemNum,
						onlineFbDelMemInfoPtr->usableOutputMemNum
						);
				
			if((fbLoop != 0) && !(fbLoop%4))
				printf( "\n");
		}

		printf( "\n");		
	} 	 	
}


void onlineDwlfbCodeSpecChgInfoDisplay(void)
{
	strOnlineDwlSpecChgFbCodeMngrInfo	*onlineFbSpecChgMngrInfoPtr = &sOnlineDwlFbSpecChgInfo;
	strOnlineDwlSpecChgSegInfo			*onlineSegSpecChgInfoPtr = onlineFbSpecChgMngrInfoPtr->specChgSegInfoPtr;
	strOnlineDwlSpecChgFbCodeInfo		*onlineFbSpecChgInfoPtr;
	strOnlineDwlChgSpecInfo				*chgSpecInfoPtr;
	strFbSpecActionInfo *specAction;
	uint32	fbLoop, segLoop, specLoop;
	
	printf( "************************************************************\n");	
	printf( "**		FB Code Block Input Change Info Display\n");
	printf( "************************************************************\n");
	
	printf( "Total Spec Change TASK Number    : %d\n",onlineFbSpecChgMngrInfoPtr->numOfTotalSegment);
	printf( "Total Spec Change FB Code Number : %d\n",onlineFbSpecChgMngrInfoPtr->numOfTotalFbCode);
	
	for(segLoop = 0; segLoop < onlineFbSpecChgMngrInfoPtr->numOfTotalSegment; segLoop++,onlineSegSpecChgInfoPtr++){ 
		printf( "TASK ID        : %d\n",onlineSegSpecChgInfoPtr->taskId);
		printf( "NUM Of FB Code : %d\n",onlineSegSpecChgInfoPtr->numOfFbCode);
	
	 	onlineFbSpecChgInfoPtr = &onlineSegSpecChgInfoPtr->specChgFbCodeInfo[0];
	
		for(fbLoop = 0; fbLoop < onlineSegSpecChgInfoPtr->numOfFbCode; fbLoop++,onlineFbSpecChgInfoPtr++){
			printf("\n******* FB Code[%d]-FB ADDR[%d]-CHG NUM[%d]********\n",
					onlineFbSpecChgInfoPtr->fbId,onlineFbSpecChgInfoPtr->fbBlkAddr,onlineFbSpecChgInfoPtr->numOfChangedSpec);
		
			chgSpecInfoPtr = &onlineFbSpecChgInfoPtr->specDataInfo[0];
			for(specLoop = 0; specLoop < onlineFbSpecChgInfoPtr->numOfChangedSpec; specLoop++,chgSpecInfoPtr++) {
				specAction = (strFbSpecActionInfo *)&chgSpecInfoPtr->specActionData;

				printf( "INDEX[%02d]-INPUT HEADER[0x%08x]\n",
								chgSpecInfoPtr->specIndex, chgSpecInfoPtr->specActionData);

				printf( "           -INPUT DATA[0x%08x]", chgSpecInfoPtr->specData[0]);
				if(specAction->dataSize==SPEC_SIZE_LWORD) {
				        printf( " [0x%08x]", chgSpecInfoPtr->specData[1]);
				}
				else 	printf( "\n");
			}
			
			printf( "*************************************************\n");
		}
	} 	 
}


void onlineDwlfbCodeSeqIdAddAndChgInfoDisplay(void)
{
	strOnlineDwlFbSeqIdAddAndChgMngrInfo	*onlineFbSeqIdChgMngrInfoPtr = &sOnlineDwlFbSeqIdAddAndChgInfo;
	strOnlineDwlFbSeqIdAddAndChgSegInfo		*onlineSegSeqIdChgInfoPtr = &onlineFbSeqIdChgMngrInfoPtr->fbSeqIdAddAndChgSegInfo[0];
	strOnlineDwlFbSeqIdInfo					*onlineFbSeqIdChgInfoPtr, *saveOnlineFbSeqIdChgInfoPtr;
	uint32	fbLoop, segLoop;
	int8	line[16], *cPtr;
	
	saveOnlineFbSeqIdChgInfoPtr = &onlineFbSeqIdChgMngrInfoPtr->fbSeqIdInfo[0];
	
	printf( "************************************************************\n");	
	printf( "**		FB Code Block Seq ID Change Info Display\n");
	printf( "************************************************************\n");
	
	printf( "Total Change Seq ID Segment Number : %d\n",onlineFbSeqIdChgMngrInfoPtr->numOfTotalSegment);
	printf( "Total Change Seq ID FB Code Number : %d\n",onlineFbSeqIdChgMngrInfoPtr->numOfTotalFbCode);
	
	for(segLoop = 0; segLoop < onlineFbSeqIdChgMngrInfoPtr->numOfTotalSegment; segLoop++,onlineSegSeqIdChgInfoPtr++){ 
NEXT_SEG_DISPLAY:
		printf( "Segment ID     : %d\n",onlineSegSeqIdChgInfoPtr->taskId);
		printf( "NUM Of FB Code : %d\n",onlineSegSeqIdChgInfoPtr->numOfFbCode);
	
	 	onlineFbSeqIdChgInfoPtr = saveOnlineFbSeqIdChgInfoPtr + onlineSegSeqIdChgInfoPtr->startIndex;
		
		printf("Do You Want Display Fb Code Info ?[Y/N] ");
    	ioRead(gDebugPortFd, line, sizeof(line));
    	cPtr = line;
    	spaceSkip(&cPtr);
		
		if( (*cPtr != 'Y') && (*cPtr != 'y')) continue;	

 		printf("+=====+========+========+=========+=====+========+========+=========+\n");
 		printf("| ID# | SEG_ID | FB_ADR | FB CODE | ID# | SEG_ID | FB_ADR | FB CODE |\n");
 		printf("+=====+========+========+=========+=====+========+========+=========+\n");

		for(fbLoop = 0; fbLoop < onlineSegSeqIdChgInfoPtr->numOfFbCode; fbLoop++,onlineFbSeqIdChgInfoPtr++){
			if((fbLoop%2) == 0x0)
				printf("|%05d|%08d|%08d|%09d|", 	onlineFbSeqIdChgInfoPtr->fbSeqId,
																onlineSegSeqIdChgInfoPtr->taskId,
																onlineFbSeqIdChgInfoPtr->fbBlkAddr,
																onlineFbSeqIdChgInfoPtr->fbId
					);

			if(fbLoop%2)
				printf("|%05d|%08d|%08d|%09d|\n", 	onlineFbSeqIdChgInfoPtr->fbSeqId,
																onlineSegSeqIdChgInfoPtr->taskId,
																onlineFbSeqIdChgInfoPtr->fbBlkAddr,
																onlineFbSeqIdChgInfoPtr->fbId
					);
			if( (fbLoop != 0) && !(fbLoop %99)){
 				printf("+=====+========+========+=========+=====+========+========+=========+\n");
				printf("Quit[q] Or Continue[return] ? ");
    			ioRead(gDebugPortFd, line, sizeof(line));
    			cPtr = line;
    			spaceSkip(&cPtr);
		
				if( (*cPtr == 'q') || (*cPtr == 'Q')) goto NEXT_SEG_DISPLAY;	
					
			}
		}

		if(onlineSegSeqIdChgInfoPtr->numOfFbCode%2) printf( "\n");
 		printf("+=====+========+========+=========+=====+========+========+=========+\n");
	} 	 
}


void fbTaskComQueCountInfoDisplay(void)
{
	uint32	reqFreeListCount = 0, reqUsedListCount = 0;
	uint32	respFreeListCount = 0, respUsedListCount = 0;
	uint32 status = NO_ERROR;
	strOnlineDwlTaskComReqInfo		*reqNodePtr;	
	strOnlineDwlTaskComRespInfo		*respNodePtr;
	
	printf( "\t***************************************************\n");
	printf( "\t**             FB COM REQ/RESP Queue Count Info  **\n");
	printf( "\t***************************************************\n");
	
	
    status = semaphoreGet(sOnlineDwlTaskComQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
		return;
	}

	
	reqNodePtr = sOnlineDwlTaskComReqMngrInfo.freeList;

    while(reqNodePtr != NULL) {
    	reqFreeListCount++;

        reqNodePtr = (strOnlineDwlTaskComReqInfo *)nextNodeRef ( 	
        												(strDoubleLink *)sOnlineDwlTaskComReqMngrInfo.freeList,
	                        							(strDoubleLink *)reqNodePtr
							  									);
	}

	reqNodePtr = sOnlineDwlTaskComReqMngrInfo.usedList;
    	
    while(reqNodePtr != NULL) {
    	reqUsedListCount++;
			
        reqNodePtr = (strOnlineDwlTaskComReqInfo *)nextNodeRef ( 	
        								(strDoubleLink *)sOnlineDwlTaskComReqMngrInfo.usedList,
	       	                			(strDoubleLink *)reqNodePtr
							  									);
	}

	
	respNodePtr = sOnlineDwlTaskComRespMngrInfo.freeList;

    while(respNodePtr != NULL) {
    	respFreeListCount++;

        respNodePtr = (strOnlineDwlTaskComRespInfo *)nextNodeRef ( 	
        												(strDoubleLink *)sOnlineDwlTaskComRespMngrInfo.freeList,
	                        							(strDoubleLink *)respNodePtr
							  									);
	}

	respNodePtr = sOnlineDwlTaskComRespMngrInfo.usedList;
    	
    while(respNodePtr != NULL) {
    	respUsedListCount++;
			
        respNodePtr = (strOnlineDwlTaskComRespInfo *)nextNodeRef ( 	
        								(strDoubleLink *)sOnlineDwlTaskComRespMngrInfo.usedList,
	       	                			(strDoubleLink *)respNodePtr
							  									);
	}
	
#ifdef VXWORKS
	semaphoreRelease (sOnlineDwlTaskComQSemaId);
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sOnlineDwlTaskComQSemaId);
#endif

	printf( "\tREQ MAX QUEUE COUNT                  : %05d\n",
								MAX_ONLINE_DWL_TASK_COM_QUE_NUM
			);
	printf( "\tREQ Free List Queue Count            : %05d\n",reqFreeListCount);
	printf( "\tREQ Total Used Queue Count           : %05d\n",reqUsedListCount);

	printf( "\tRESP MAX QUEUE COUNT                 : %05d\n",
								MAX_ONLINE_DWL_TASK_COM_QUE_NUM
			);	
	printf( "\tRESP Free List Queue Count           : %05d\n",respFreeListCount);
	printf( "\tRESP Total Used Queue Count          : %05d\n",respUsedListCount);
	
	return;
}


void fbTaskComQueDataInfoDisplay(void)
{
	uint32	reqUsedListCount = 0, respUsedListCount = 0;
	uint32 status = NO_ERROR, strIndex = 0;
	strOnlineDwlTaskComReqInfo		*reqNodePtr;	
	strOnlineDwlTaskComRespInfo		*respNodePtr;
	uint8	*reqComString[] =
	{
		"UnKNOWN CODE",
		"ONLINE_DWL_FBCODE_ADD",
		"ONLINE_DWL_FBCODE_DEL",
		"ONLINE_DWL_FBCODE_SPEC_CHANGE",
		"ONLINE_DWL_FBCODE_SEQ_ID_ADD_AND_CHANGE"
	};
		
	printf( "\t***************************************************\n");
	printf( "\t**             FB COM REQ/RESP Queue Data Info  **\n");
	printf( "\t***************************************************\n");
	
	
    status = semaphoreGet(sOnlineDwlTaskComQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
		return;
	}

	
	reqNodePtr = sOnlineDwlTaskComReqMngrInfo.usedList;

    while(reqNodePtr != NULL) {
    	reqUsedListCount++;
		if(reqNodePtr->onlineDwlOperationCode > MAX_ONLINE_DWL_KIND)
			strIndex = 0;
		else
			strIndex = reqNodePtr->onlineDwlOperationCode;
			
		printf("Count[%d]-REQ Operation Code[%s]-REQ DATA NUM[%d]\n",
							reqUsedListCount, reqComString[strIndex],
							reqNodePtr->reqDataNum
				);				
        reqNodePtr = (strOnlineDwlTaskComReqInfo *)nextNodeRef ( 	
        								(strDoubleLink *)sOnlineDwlTaskComReqMngrInfo.usedList,
	       	                			(strDoubleLink *)reqNodePtr
							  									);
	}

	
	respNodePtr = sOnlineDwlTaskComRespMngrInfo.usedList;
    	
    while(respNodePtr != NULL) {
    	respUsedListCount++;
		printf("Count[%d]-RESP Operation Code[%d]-RESP DATA NUM[%d]\n",
							respUsedListCount, respNodePtr->onlineDwlOperationCode,
							respNodePtr->respDataNum
				);				
			
        respNodePtr = (strOnlineDwlTaskComRespInfo *)nextNodeRef ( 	
        								(strDoubleLink *)sOnlineDwlTaskComRespMngrInfo.usedList,
	       	                			(strDoubleLink *)respNodePtr
							  									);
	}
	
#ifdef VXWORKS
	semaphoreRelease (sOnlineDwlTaskComQSemaId);
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sOnlineDwlTaskComQSemaId);
#endif
	return;
}


static uint32	onlineDwlSegmentCheckSumCalcu(	uint32 	taskId,
												uint32	operationCode,
												uint32	inputByteSize,
												uint32  outputByteSize
												)
{
	uint32	status = NO_ERROR;
    strFbTaskTblMemInfo	*segTblInfoPtr, *busSegTblInfoPtr, *rmtSegTblInfoPtr;
	uint32	segChecksum;
	strFbMemInfo		*fbMemStartPtr;
	int32	offSetInputByte,offSetOutputByte, usedBytes;
	strMemPointerInfo rmtbusMemAddrInfo;
	
	switch (operationCode) {
		case ONLINE_DWL_FBCODE_ADD:
			offSetInputByte 	= inputByteSize;
			offSetOutputByte 	= outputByteSize;
		break;
		case ONLINE_DWL_FBCODE_DEL:
			offSetInputByte 	= (int32)inputByteSize * -1;
			offSetOutputByte 	= (int32)outputByteSize * -1;
		break;
		case ONLINE_FB_PARA_REALLOC:
			offSetInputByte 	= 0;
			offSetOutputByte 	= 0;
		break;
		default:
			status = ONLINE_SEG_CHECKSUM_ERR;
			setErrorCodeWithVal(	__FILE__,__LINE__,"onlineDwlSegmentCheckSumCalcu", status,
									"Seg ID", taskId,
									"Operation Code", operationCode,
									"Not Using", 0
								  );
			return(status);
	}

	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
		setErrorCodeWithVal(	__FILE__,__LINE__,"onlineDwlSegmentCheckSumCalcu", status,
								"Seg ID", taskId,
								"Not Using", 0,
								"Not Using", 0
							  );
		return(status);
	}

	
	segTblInfoPtr = gMpuFbSegTblInfoLocalPtr + (taskId - 1);	
	
	
	if(operationCode == ONLINE_DWL_FBCODE_DEL) {
		if(segTblInfoPtr->memInputSize < inputByteSize) {
			status = ONLINE_SEG_CHECKSUM_ERR;
			setErrorCodeWithVal(	__FILE__,__LINE__,"onlineDwlSegmentCheckSumCalcu", status,
									"Seg ID", taskId,
									"Mem Used Size", segTblInfoPtr->memInputSize,
									"Del Input Size", inputByteSize
								  );
			return(status);
		}
		else if (segTblInfoPtr->memOutputSize < outputByteSize) {
			status = ONLINE_SEG_CHECKSUM_ERR;
			setErrorCodeWithVal(	__FILE__,__LINE__,"onlineDwlSegmentCheckSumCalcu", status,
									"Seg ID", taskId,
									"Mem Used Size", segTblInfoPtr->memOutputSize,
									"Del Output Size", outputByteSize
								  );
			return(status);
		}
	}

	
	segTblInfoPtr->memInputSize = segTblInfoPtr->memInputSize + offSetInputByte;
	segTblInfoPtr->memOutputSize = segTblInfoPtr->memOutputSize + offSetOutputByte;

	segTblInfoPtr->recvMemSize = segTblInfoPtr->recvMemSize + offSetInputByte ;

	usedBytes = segTblInfoPtr->memInputSize;
		
    if(segTblInfoPtr->taskMode != PCS_RUN) {
    	status = L_PCS_MODE_ERR;
		setErrorCodeWithVal(	__FILE__,__LINE__,"onlineDwlSegmentCheckSumCalcu", status,
								"Seg ID", taskId,
								"seg Mode", segTblInfoPtr->taskMode,
								"Not Using", 0
							  );
		return(status);
	}
    	
	segChecksum = 0;
 	fbMemStartPtr = gMpuFbMemInfoLocalPtr;
 
	
	segmentChecksumCalcu(segTblInfoPtr, fbMemStartPtr, &segChecksum,LOGIC_ID );
	
	
	segTblInfoPtr->logicCheckSum = segChecksum;
	
	
	busSegTblInfoPtr = gMpuFbSegTblInfoBusPtr + (taskId - 1);	

	busSegTblInfoPtr->logicCheckSum 	= segChecksum;
	busSegTblInfoPtr->memInputSize	= segTblInfoPtr->memInputSize;
	busSegTblInfoPtr->memOutputSize= segTblInfoPtr->memOutputSize;

	busSegTblInfoPtr->recvMemSize = segTblInfoPtr->recvMemSize;

	if( dualFlagCheckInRunShelf() == DUAL_CHECK_ENABLE){ 
	 	status = rmtbusFbMemPointerGet	(LOGIC_ID, &rmtbusMemAddrInfo);
		if(status == NO_ERROR){
			rmtSegTblInfoPtr = (strFbTaskTblMemInfo*)rmtbusMemAddrInfo.segTblMemPtr;

#ifdef VXWORKS
			status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
									BUS_DATA_WRITE ,
									(uint8 *)&rmtSegTblInfoPtr->logicCheckSum, 
									(uint8 *)&busSegTblInfoPtr->logicCheckSum
						);
#else
			status = myRmtDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
									BUS_DATA_WRITE ,
									(uint8 *)&rmtSegTblInfoPtr->logicCheckSum, 
									(uint8 *)&busSegTblInfoPtr->logicCheckSum
						);
#endif
			if(status==NO_ERROR){
#ifdef VXWORKS
				status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
							BUS_DATA_WRITE ,
							(uint8 *)&rmtSegTblInfoPtr->memInputSize, 
							(uint8 *)&busSegTblInfoPtr->memInputSize
				);
#else
				status = myRmtDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
							BUS_DATA_WRITE ,
							(uint8 *)&rmtSegTblInfoPtr->memInputSize, 
							(uint8 *)&busSegTblInfoPtr->memInputSize
				);
#endif
			}

			if(status==NO_ERROR){
#ifdef VXWORKS
				status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
							BUS_DATA_WRITE ,
							(uint8 *)&rmtSegTblInfoPtr->memOutputSize, 
							(uint8 *)&busSegTblInfoPtr->memOutputSize
				);
#else
				status = myRmtDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
							BUS_DATA_WRITE ,
							(uint8 *)&rmtSegTblInfoPtr->memOutputSize, 
							(uint8 *)&busSegTblInfoPtr->memOutputSize
				);
#endif
			}

			if(status==NO_ERROR){
#ifdef VXWORKS
				status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
							BUS_DATA_WRITE ,
							(uint8 *)&rmtSegTblInfoPtr->recvMemSize, 
							(uint8 *)&busSegTblInfoPtr->recvMemSize
				);
#else
				status = myRmtDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
							BUS_DATA_WRITE ,
							(uint8 *)&rmtSegTblInfoPtr->recvMemSize, 
							(uint8 *)&busSegTblInfoPtr->recvMemSize
				);
#endif
			}

		}

		if(status != NO_ERROR)
				setErrorCode(	(uint8 *)__FILE__, 	__LINE__, 
							    (uint8 *)"onlineDwlSegmentCheckSumCalcu", status );
	}
	return(status);
}


static uint32 onlineDwlFbCodeAddFuncInFbTask(strOnlineDwlTaskComReqInfo *argPtr)
{
	uint32	status = NO_ERROR;
	strOnlineDwlAddFbCodeMngrInfo	*onlineFbAddMngrInfoPtr = &sOnlineDwlFbAddInfo;
	strOnlineDwlAddSegInfo			*onlineSegAddInfoPtr = onlineFbAddMngrInfoPtr->addSegInfoPtr;
	strOnlineDwlAddFbCodeInfo		*onlineFbAddInfoPtr;
	strFbTaskTblMemInfo	segTblInfo, *segTblPtr;
	strFbMemInfo 		fbMemInfo;
	uint32 segLoop, loop = 0, specAndVarNum = 0;
	uint32	totalFbCodeAddNum = 0;
	strFbSpecUnusedMemInfo	specNodeInfo, outputNodeInfo;
	uint32	rmtComDbSefFlag			= 0;
	uint32	totalAddInputBytes	= 0, totalAddOutputBytes = 0;

	uint32  outputSizeNum = 0 ;
	strModuleExecInfo sysExeInfo;

	
	if(onlineFbAddMngrInfoPtr->dwlTotalFrameNum != onlineFbAddMngrInfoPtr->dwlCurFrameNum) {
		status = ONLINE_DWL_FRAME_NUM_ERR; 
		setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeAddFuncInFbTask",status,
								"Total Frame Number", onlineFbAddMngrInfoPtr->dwlTotalFrameNum,
								"Current Frame Number",onlineFbAddMngrInfoPtr->dwlCurFrameNum,
								"Not Using", 0
							 );
		return(status);							 							
	}
	
	
	if( (onlineFbAddMngrInfoPtr->numOfTotalSegment == 0) ||
		(onlineFbAddMngrInfoPtr->numOfTotalSegment > MAX_ONLINE_DWL_SEG_ADD_NUM)
	){
		status = ONLINE_DWL_SEG_NUM_ERR; 
		setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeAddFuncInFbTask",status,
								"Total Add Segment Number", onlineFbAddMngrInfoPtr->numOfTotalSegment,
								"Max Add Segment Number",MAX_ONLINE_DWL_SEG_ADD_NUM,
								"Not Using", 0
							 );
		return(status);							 					
	}
	
	
	if( (onlineFbAddMngrInfoPtr->numOfTotalFbCode == 0) ||
		(onlineFbAddMngrInfoPtr->numOfTotalFbCode > MAX_ONLINE_DWL_FB_CODE_ADD_NUM)
	){
		status = FB_NUM_EXCEED_ERR; 
		setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeAddFuncInFbTask",status,
								"Total Add FB Number", onlineFbAddMngrInfoPtr->numOfTotalFbCode,
								"Max Add FB Number",MAX_ONLINE_DWL_FB_CODE_ADD_NUM,
								"Not Using", 0
							 );
		return(status);							 					
	}
	
	
	
	
	totalFbCodeAddNum = 0;

	for(segLoop = 0; segLoop < onlineFbAddMngrInfoPtr->numOfTotalSegment; 
		segLoop++,onlineSegAddInfoPtr++){

		
		for(loop = 1; loop <= MAX_LOGIC_TASK_NUM; loop++) {
			memoryClear( (uint8 *)&segTblInfo, sizeof(strFbTaskTblMemInfo));
		
   			(void)getFbTaskTblInfo( 
								LOGIC_ID,
           						loop,
								(strFbTaskTblMemInfo	*) &segTblInfo
								);
			if(segTblInfo.taskId == onlineSegAddInfoPtr->taskId) break;
		}
			
		
		if(	(onlineSegAddInfoPtr->taskId == 0x0) || 
			(onlineSegAddInfoPtr->taskId > MAX_LOGIC_TASK_NUM) ||
			(segTblInfo.taskId != onlineSegAddInfoPtr->taskId)
		){
			status = L_SEG_ID_ERR;
			setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeAddFuncInFbTask",status,
									"Segmnet ID", onlineSegAddInfoPtr->taskId,
									"Not Using",0,
									"Not Using", 0
							 	);
			return(status);							 					
		}
		
		
		totalFbCodeAddNum += onlineSegAddInfoPtr->numOfFbCode;	
		
	} 
	
	if(totalFbCodeAddNum != onlineFbAddMngrInfoPtr->numOfTotalFbCode) {
		status = ONLINE_FB_CODE_NUM_MISMATCH_ERR; 
		setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeAddFuncInFbTask",status,
								"Total Add FB Number", onlineFbAddMngrInfoPtr->numOfTotalFbCode,
								"Calcu Add Fb Number",totalFbCodeAddNum,
								"Not Using", 0
							 );
		return(status);							 					
	}

	
	
	
	onlineSegAddInfoPtr = onlineFbAddMngrInfoPtr->addSegInfoPtr;

	for(segLoop = 0; segLoop < onlineFbAddMngrInfoPtr->numOfTotalSegment; 
		segLoop++,onlineSegAddInfoPtr++){

		(void)getFbTaskTblInfo( 
								LOGIC_ID,
           						onlineSegAddInfoPtr->taskId,
								(strFbTaskTblMemInfo	*) &segTblInfo
							);
		onlineFbAddInfoPtr  = &onlineSegAddInfoPtr->addFbCodeInfo[0];
	
		for(loop = 0; loop <onlineSegAddInfoPtr->numOfFbCode; loop++, onlineFbAddInfoPtr++){
		
			
			if(	(onlineFbAddInfoPtr->fbBlkAddr < 1) ||
				(onlineFbAddInfoPtr->fbBlkAddr > MAX_MPU_BLOCK_ADDR)
			){
				status = SEG_BLK_ADDR_EXCEED_ERR;
				setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeAddFuncInFbTask",status,
								"Segment ID",segTblInfo.taskId,
								"Block Addr", onlineFbAddInfoPtr->fbBlkAddr,
								"MAX Block Addr", MAX_MPU_BLOCK_ADDR
							 );
				return(status);							 					
			}
			
			
   			(void)fbCodeMemInfoRead	(
										LOGIC_ID,
       									onlineFbAddInfoPtr->fbBlkAddr,
       									(strFbMemInfo *) &fbMemInfo
   									);
			if(fbMemInfo.fbId != 0x0) {
				status = FB_ADD_PERMISSON_ERR;
				setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeAddFuncInFbTask",status,
										"Block Addr", onlineFbAddInfoPtr->fbBlkAddr,
										"FB Code",fbMemInfo.fbId,
										"TAKS ID",fbMemInfo.fbTaskId
							 		);
				return(status);							 									
			}
			
			
			if(onlineFbAddInfoPtr->fbId > MAX_FB_CODE_ID) {
				status = L_FB_CODE_ERR;
	       		setErrorCodeWithVal( 	__FILE__,__LINE__,"onlineDwlFbCodeAddFuncInFbTask",
            	  	     	    		status,
                		   	    		"Add FB Code",onlineFbAddInfoPtr->fbId,
                 		  	    		"Max FB Code ID ", MAX_FB_CODE_ID,
                  	 		    		"Not Using", 0 
				  		  			);
				 return(status);				
			}
			else if( onlineFbAddInfoPtr->fbId != gFbCodeSortingTableInfo[onlineFbAddInfoPtr->fbId].fbId){
				status = L_FB_CODE_ERR;
	       		setErrorCodeWithVal( 	__FILE__,__LINE__,"onlineDwlFbCodeAddFuncInFbTask",
            	  	     	    		status,
                		   	    		"Add FB Code",onlineFbAddInfoPtr->fbId,
                 		  	    		"Defined Fb Code ", gFbCodeSortingTableInfo[onlineFbAddInfoPtr->fbId].fbId,
                  	 		    		"Not Using", 0 
				  		  			);
				return(status);
			}				
			
			else if( onlineFbAddInfoPtr->numOfSpec != gFbCodeSortingTableInfo[onlineFbAddInfoPtr->fbId].inputNo){
				status = L_PARA_NO_ERR;
	       		setErrorCodeWithVal( 	__FILE__,__LINE__,"onlineDwlFbCodeAddFuncInFbTask",
            	  	     	    		status,
                		   	    		"Req Spec Number",onlineFbAddInfoPtr->numOfSpec,
                 		  	    		"Defined Spec Number ", gFbCodeSortingTableInfo[onlineFbAddInfoPtr->fbId].inputNo,
                  	 		    		"Not Using", 0 
				  		  			);
				return(status);				  		  			
			}						
		}  		
	} 

	if((status != NO_ERROR)) return(status);

	
	memoryClear( (uint8 *)&sysExeInfo, sizeof(strModuleExecInfo));
	status = fbSysExeModuleInfoRead(LOGIC_ID, &sysExeInfo);
	if(status != NO_ERROR){
		setErrorCode (	__FILE__,__LINE__,"onlineDwlFbCodeAddFuncInFbTask", status );
	}
	else if	(sysExeInfo.curfbMaxBlockAddr > MAX_MPU_BLOCK_ADDR) 
	{
		status = L_FB_ADDR_ERR;
		setErrorCodeWithVal (	__FILE__,__LINE__,"onlineDwlFbCodeAddFuncInFbTask", status,
							"Read Max FB Addr", sysExeInfo.curfbMaxBlockAddr,
							"MAX MPU FB Addr",MAX_MPU_BLOCK_ADDR,
							"NOT Using",0
						   );
	}  
	
	
	for(segLoop = 0; segLoop < onlineFbAddMngrInfoPtr->numOfTotalSegment; segLoop++){
		onlineSegAddInfoPtr = onlineFbAddMngrInfoPtr->addSegInfoPtr + segLoop;
		
		rmtComDbSefFlag			= 0;
				
		if( (status = segTblPointerGet( LOGIC_ID,segLoop+1, &segTblPtr ) ) == NO_ERROR) 
		{
			segTblPtr->fbUsedNum += onlineSegAddInfoPtr->numOfFbCode;
		}    
		else {
			setErrorCode(	(uint8 *)__FILE__, 
				(uint32)__LINE__, 
				(uint8 *)"onlineDwlFbCodeAddFuncInFbTask",status
			);
		}

		totalAddInputBytes = 0;
		totalAddOutputBytes = 0;

		for(loop = 0; loop < onlineSegAddInfoPtr->numOfFbCode; loop++) {
			onlineFbAddInfoPtr = &onlineSegAddInfoPtr->addFbCodeInfo[loop];
			
			if( (onlineFbAddInfoPtr->fbId == ETH_RMT_STATUS_READ_FB_CODE) ||
				(onlineFbAddInfoPtr->fbId == ETH_RMT_DATA_SEND_FB_CODE) ||
				(onlineFbAddInfoPtr->fbId == MBT_RMT_DATA_SEND_FB_CODE) ||
				(onlineFbAddInfoPtr->fbId == ETH_RMT_DATA_BROAD_FB_CODE) ||
				(onlineFbAddInfoPtr->fbId == ETH_RMT_DATA_READ_FB_CODE_FOR_REPLACE) ||
				(onlineFbAddInfoPtr->fbId == ETH_RMT_DATA_WRITE_FB_CODE_FOR_REPLACE))
				rmtComDbSefFlag = 1;
				
			
			specAndVarNum = gFbCodeSortingTableInfo[onlineFbAddInfoPtr->fbId].inputNo;
			specAndVarNum += gFbCodeSortingTableInfo[onlineFbAddInfoPtr->fbId].inputSize;
			specAndVarNum += gFbCodeSortingTableInfo[onlineFbAddInfoPtr->fbId].intlVarSize;
			
			totalAddInputBytes += specAndVarNum; 
			
			
			status = fbSpecMemorySpaceGet (	specAndVarNum, 
											(strFbSpecUnusedMemInfo *) &specNodeInfo
										   );
			if(status != NO_ERROR) {
				setErrorCodeWithVal (	__FILE__,__LINE__,"onlineDwlFbCodeAddFuncInFbTask", status,
										"Seg ID", onlineSegAddInfoPtr->taskId,
										"Fb Code",onlineFbAddInfoPtr->fbId,
										"Fb Addr",onlineFbAddInfoPtr->fbBlkAddr
									   );
				return(status);
			}
			
			
			
			outputSizeNum = gFbCodeSortingTableInfo[onlineFbAddInfoPtr->fbId].outputSize;
			totalAddOutputBytes += outputSizeNum;
			
			
			status = fbOutputMemorySpaceGet (	outputSizeNum, 
											(strFbSpecUnusedMemInfo *) &outputNodeInfo
										   );
			if(status != NO_ERROR) {
				setErrorCodeWithVal (	__FILE__,__LINE__,"onlineDwlFbCodeAddFuncInFbTask", status,
										"Seg ID", onlineSegAddInfoPtr->taskId,
										"Fb Code",onlineFbAddInfoPtr->fbId,
										"Fb Addr",onlineFbAddInfoPtr->fbBlkAddr
									   );
				return(status);
			}
			
			 
			
			status = onlineDwlFbCodeSetInLocalAndBusMemory (onlineFbAddInfoPtr,
															&specNodeInfo, &outputNodeInfo
															);
			if(status != NO_ERROR) {
				setErrorCodeWithVal (	__FILE__,__LINE__,"onlineDwlFbCodeAddFuncInFbTask", status,
										"Seg ID", onlineSegAddInfoPtr->taskId,
										"Fb Code",onlineFbAddInfoPtr->fbId,
										"Fb Addr",onlineFbAddInfoPtr->fbBlkAddr
									   );
				return(status);
			}

			
			if(onlineFbAddInfoPtr->fbBlkAddr>sysExeInfo.curfbMaxBlockAddr )
				sysExeInfo.curfbMaxBlockAddr = onlineFbAddInfoPtr->fbBlkAddr;

			if(specNodeInfo.unUsedMemStartIndex + specAndVarNum > sysExeInfo.curfbMaxParaIndex)
				sysExeInfo.curfbMaxParaIndex = specNodeInfo.unUsedMemStartIndex + specAndVarNum;

			if(outputNodeInfo.unUsedMemStartIndex + outputSizeNum > sysExeInfo.curfbMaxOutIndex  )
				sysExeInfo.curfbMaxOutIndex = outputNodeInfo.unUsedMemStartIndex + outputSizeNum;

			status = fbSysExeModuleInfoWrite(LOGIC_ID, &sysExeInfo);
			if(status != NO_ERROR) {
				setErrorCode (	__FILE__,__LINE__,"onlineDwlFbCodeAddFuncInFbTask", status);
				return(status);
			}


		} 
		
		 

		
    	if(rmtComDbSefFlag) {
			status = rmtEthComSeqNumChkDBSet(onlineSegAddInfoPtr->taskId);
			if(status != NO_ERROR){
            	setErrorCode (	__FILE__,__LINE__,
                    	            "onlineDwlFbCodeAddFuncInFbTask",
                        	        status
								);
				return(status);
			}
		}
		
		
		status = onlineDwlSegmentCheckSumCalcu(
												onlineSegAddInfoPtr->taskId, 
												ONLINE_DWL_FBCODE_ADD,
												totalAddInputBytes, totalAddOutputBytes
											  );
		if(status != NO_ERROR) {
			setErrorCodeWithVal (	__FILE__,__LINE__,"onlineDwlFbCodeAddFuncInFbTask", status,
									"Seg ID", onlineSegAddInfoPtr->taskId,
									"Not Using",0,
									"Not Using",0
								   );
			break;
		}
	} 	

	return(status);
}


static uint32 onlineDwlFbCodeSetInLocalAndBusMemory(strOnlineDwlAddFbCodeInfo	*addFbCodeInfo,
													strFbSpecUnusedMemInfo 	*specMemInfoPtr,
													strFbSpecUnusedMemInfo 	*outputMemInfoPtr
													)
{
	uint32 status = NO_ERROR;
	uint32 loop;
	uint32	*specStartPtr,*varStartPtr, *outputStartPtr;		
	strFbMemInfo	*fbMemInfoPtr;	
	
	
	fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (addFbCodeInfo->fbBlkAddr-1);
	
	fbMemInfoPtr->fbTaskId		= addFbCodeInfo->taskId;
	fbMemInfoPtr->fbFuncAddr	= addFbCodeInfo->fbBlkAddr;
	fbMemInfoPtr->fbId			= addFbCodeInfo->fbId;
	fbMemInfoPtr->inputNo		= gFbCodeSortingTableInfo[addFbCodeInfo->fbId].inputNo;
    fbMemInfoPtr->intlVarNo			= gFbCodeSortingTableInfo[addFbCodeInfo->fbId].intlVarNo;
    fbMemInfoPtr->outputNo		= gFbCodeSortingTableInfo[addFbCodeInfo->fbId].outputNo;
	fbMemInfoPtr->inputSize		= gFbCodeSortingTableInfo[addFbCodeInfo->fbId].inputSize;
	fbMemInfoPtr->intlVarSize		= gFbCodeSortingTableInfo[addFbCodeInfo->fbId].intlVarSize;
	fbMemInfoPtr->outputSize		= gFbCodeSortingTableInfo[addFbCodeInfo->fbId].outputSize;

    fbMemInfoPtr->specActionIndex = specMemInfoPtr->unUsedMemStartIndex;
    fbMemInfoPtr->specIndex		= fbMemInfoPtr->specActionIndex + fbMemInfoPtr->inputNo;
    fbMemInfoPtr->varIndex		= fbMemInfoPtr->specIndex + fbMemInfoPtr->inputSize;
    fbMemInfoPtr->outputIndex	= outputMemInfoPtr->unUsedMemStartIndex;
	fbMemInfoPtr->fbRunStatus	= 0;
	fbMemInfoPtr->paraRunType   = addFbCodeInfo->paraRunningType;
	
	fbMemInfoPtr->outputAction[0]  = addFbCodeInfo->outputAction[0];
	fbMemInfoPtr->outputAction[1]  = addFbCodeInfo->outputAction[1];
	fbMemInfoPtr->ActionID		= addFbCodeInfo->ActionID;
	fbMemInfoPtr->TransitionID  = addFbCodeInfo->TransitionID;
	fbMemInfoPtr->logicType     = addFbCodeInfo->logicType;
	fbMemInfoPtr->usingType		= addFbCodeInfo->usingType;

	
	
    fbMemInfoPtr->fbExecSequenceId = MAX_FB_EXEC_SEQ_ID;

	
	specStartPtr = (uint32 *)gMpuSpecAndVarDataLocalPtr + fbMemInfoPtr->specActionIndex;
	for(loop = 0; loop < fbMemInfoPtr->inputNo ; loop++)
		*specStartPtr++ = addFbCodeInfo->specData[loop];
	
	
	specStartPtr = (uint32 *)gMpuSpecAndVarDataLocalPtr + fbMemInfoPtr->specIndex;
	for(loop = 0; loop < fbMemInfoPtr->inputSize ; loop++)
		*specStartPtr++ = addFbCodeInfo->specData[fbMemInfoPtr->inputNo+loop];

	
	varStartPtr = (uint32 *)gMpuSpecAndVarDataLocalPtr + fbMemInfoPtr->varIndex;
	memoryClear( (uint8 *)varStartPtr, fbMemInfoPtr->intlVarSize * 4); 
		
	
	outputStartPtr = (uint32 *)gMpuOutputDataLocalPtr + fbMemInfoPtr->outputIndex;
	memoryClear( (uint8 *)outputStartPtr, fbMemInfoPtr->outputSize * 4); 
	
	
	fbMemInfoPtr = gMpuFbMemInfoBusPtr + (addFbCodeInfo->fbBlkAddr-1);
	
	fbMemInfoPtr->fbTaskId		= addFbCodeInfo->taskId;
	fbMemInfoPtr->fbFuncAddr	= addFbCodeInfo->fbBlkAddr;

	fbMemInfoPtr->fbId	= addFbCodeInfo->fbId;
	fbMemInfoPtr->inputNo		= gFbCodeSortingTableInfo[addFbCodeInfo->fbId].inputNo;
    fbMemInfoPtr->intlVarNo			= gFbCodeSortingTableInfo[addFbCodeInfo->fbId].intlVarNo;
    fbMemInfoPtr->outputNo		= gFbCodeSortingTableInfo[addFbCodeInfo->fbId].outputNo;
	fbMemInfoPtr->inputSize		= gFbCodeSortingTableInfo[addFbCodeInfo->fbId].inputSize;
	fbMemInfoPtr->intlVarSize		= gFbCodeSortingTableInfo[addFbCodeInfo->fbId].intlVarSize;
	fbMemInfoPtr->outputSize		= gFbCodeSortingTableInfo[addFbCodeInfo->fbId].outputSize;
	
    fbMemInfoPtr->specActionIndex = specMemInfoPtr->unUsedMemStartIndex;
    fbMemInfoPtr->specIndex		= fbMemInfoPtr->specActionIndex + fbMemInfoPtr->inputNo;
    fbMemInfoPtr->varIndex		= fbMemInfoPtr->specIndex + fbMemInfoPtr->inputSize;
    fbMemInfoPtr->outputIndex	= outputMemInfoPtr->unUsedMemStartIndex;
	fbMemInfoPtr->fbRunStatus	= 0;
	fbMemInfoPtr->paraRunType   = addFbCodeInfo->paraRunningType;

	fbMemInfoPtr->outputAction[0]  = addFbCodeInfo->outputAction[0];
	fbMemInfoPtr->outputAction[1]  = addFbCodeInfo->outputAction[1];
	fbMemInfoPtr->ActionID		= addFbCodeInfo->ActionID;
	fbMemInfoPtr->TransitionID  = addFbCodeInfo->TransitionID;
	fbMemInfoPtr->logicType     = addFbCodeInfo->logicType;
	fbMemInfoPtr->usingType		= addFbCodeInfo->usingType;

	
	
    fbMemInfoPtr->fbExecSequenceId = MAX_FB_EXEC_SEQ_ID;

	
	specStartPtr = (uint32 *)gMpuSpecAndVarDataBusPtr + fbMemInfoPtr->specActionIndex;
	for(loop = 0; loop < fbMemInfoPtr->inputNo; loop++)
		*specStartPtr++ = addFbCodeInfo->specData[loop];
	
	
	specStartPtr = (uint32 *)gMpuSpecAndVarDataBusPtr + fbMemInfoPtr->specIndex;
	for(loop = 0; loop < fbMemInfoPtr->inputSize; loop++)
		*specStartPtr++ = addFbCodeInfo->specData[fbMemInfoPtr->inputNo+loop];
	
	
	varStartPtr = (uint32 *)gMpuSpecAndVarDataBusPtr + fbMemInfoPtr->varIndex;
	memoryClear( (uint8 *)varStartPtr, fbMemInfoPtr->intlVarSize * 4); 
		
	
	outputStartPtr = (uint32 *)gMpuOutputDataBusPtr + fbMemInfoPtr->outputIndex;
	memoryClear( (uint8 *)outputStartPtr, fbMemInfoPtr->outputSize * 4); 

	return(status);
}


static uint32 onlineDwlFbCodeDelFuncInFbTask(strOnlineDwlTaskComReqInfo *argPtr)
{
	uint32	status = NO_ERROR;
	uint32	totalFbCodeDelNum;	
	strFbTaskTblMemInfo	segTblInfo, *segTblPtr;
	strFbMemInfo	fbMemInfo;
	uint32	loop = 0, segDelLoop;
	strOnlineDwlDelFbCodeMngrInfo	*onlineFbDelMngrInfoPtr = &sOnlineDwlFbDelInfo;
	strOnlineDwlDelSegInfo			*onlineSegDelInfoPtr = onlineFbDelMngrInfoPtr->delSegInfoPtr;
	strOnlineDwlDelFbCodeInfo		*onlineFbDelInfoPtr;
	uint32	rmtComDbSefFlag	= 0, specAndVarNum = 0;
	uint32	totalDelInputBytes = 0, totalDelOutputBytes = 0;

	
	
	if(onlineFbDelMngrInfoPtr->dwlTotalFrameNum != onlineFbDelMngrInfoPtr->dwlCurFrameNum) {
		status = ONLINE_DWL_FRAME_NUM_ERR; 
		setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeAddFuncInFbTask",status,
								"Total Frame Number", onlineFbDelMngrInfoPtr->dwlTotalFrameNum,
								"Current Frame Number",onlineFbDelMngrInfoPtr->dwlCurFrameNum,
								"Not Using", 0
							 );
		return(status);							 							
	}

		
 	if(	(onlineFbDelMngrInfoPtr->numOfTotalSegment == 0) ||
		(onlineFbDelMngrInfoPtr->numOfTotalSegment > MAX_ONLINE_DWL_SEG_DEL_NUM)
	){
		status = ONLINE_DWL_SEG_NUM_ERR;
       	setErrorCodeWithVal( 	__FILE__,
       							__LINE__,
               	        		"onlineDwlFbCodeDelFuncInFbTask",
                   	    		status,
                   	    		"Total Seg Del Numver", onlineFbDelMngrInfoPtr->numOfTotalSegment,
                   	    		"MAX FB Code Del Number", MAX_ONLINE_DWL_SEG_DEL_NUM,
                   	    		"Not Using",0
				  	  		);
		return(status);
	}
	
 	if(	(onlineFbDelMngrInfoPtr->numOfTotalFbCode == 0) ||
		(onlineFbDelMngrInfoPtr->numOfTotalFbCode > MAX_ONLINE_DWL_FB_CODE_DEL_NUM)
	){
		status = FB_NUM_EXCEED_ERR;
       	setErrorCodeWithVal( 	__FILE__,
       							__LINE__,
               	        		"onlineDwlFbCodeDelFuncInFbTask",
                   	    		status,
                  	    		"Total FbCode Del Numver", onlineFbDelMngrInfoPtr->numOfTotalFbCode,
                   	    		"MAX FB Code Del Number", MAX_ONLINE_DWL_FB_CODE_DEL_NUM,
                   	    		"Not Using",0
 				  	  		);
		return(status);
	}

	
	
	
	totalFbCodeDelNum = 0;

	for(segDelLoop = 0; segDelLoop < onlineFbDelMngrInfoPtr->numOfTotalSegment; 
		segDelLoop++,onlineSegDelInfoPtr++){

		
		for(loop = 1; loop <= MAX_LOGIC_TASK_NUM; loop++) {
			memoryClear( (uint8 *)&segTblInfo, sizeof(strFbTaskTblMemInfo));
		
   			(void)getFbTaskTblInfo( 
								LOGIC_ID,
           						loop,
								(strFbTaskTblMemInfo	*) &segTblInfo
								);
			if(segTblInfo.taskId == onlineSegDelInfoPtr->taskId) break;
		}
			
		
		if(	(onlineSegDelInfoPtr->taskId == 0x0) || 
			(onlineSegDelInfoPtr->taskId > MAX_LOGIC_TASK_NUM) ||
			(segTblInfo.taskId != onlineSegDelInfoPtr->taskId)
		){
			status = L_SEG_ID_ERR;
			setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeDelFuncInFbTask",status,
									"Segmnet ID", onlineSegDelInfoPtr->taskId,
									"Not Using",0,
									"Not Using", 0
							 	);
			return(status);							 					
		}
		
		
		totalFbCodeDelNum += onlineSegDelInfoPtr->numOfFbCode;	
		
	} 
	
	if(totalFbCodeDelNum != onlineFbDelMngrInfoPtr->numOfTotalFbCode) {
		status = ONLINE_FB_CODE_NUM_MISMATCH_ERR; 
		setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeDelFuncInFbTask",status,
								"Total Add FB Number", onlineFbDelMngrInfoPtr->numOfTotalFbCode,
								"Calcu Add Fb Number",totalFbCodeDelNum,
								"Not Using", 0
							 );
		return(status);							 					
	}

	
	
	onlineSegDelInfoPtr = onlineFbDelMngrInfoPtr->delSegInfoPtr;

	for(segDelLoop = 0; segDelLoop < onlineFbDelMngrInfoPtr->numOfTotalSegment; 
		segDelLoop++,onlineSegDelInfoPtr++){

		(void)getFbTaskTblInfo( 
								LOGIC_ID,
           						onlineSegDelInfoPtr->taskId,
								(strFbTaskTblMemInfo	*) &segTblInfo
							);
	
		onlineFbDelInfoPtr  = &onlineSegDelInfoPtr->delFbCodeInfo[0];
		for(loop = 0; loop <onlineSegDelInfoPtr->numOfFbCode; loop++, onlineFbDelInfoPtr++){
		
			
			if(	(onlineFbDelInfoPtr->fbBlkAddr < 1) ||
				(onlineFbDelInfoPtr->fbBlkAddr > MAX_MPU_BLOCK_ADDR)
			){
				status = SEG_BLK_ADDR_EXCEED_ERR;
				setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeDelFuncInFbTask",status,
								"Block Addr", onlineFbDelInfoPtr->fbBlkAddr,
								"MAX Addr",MAX_MPU_BLOCK_ADDR,
								"Not Using", 0
							 );
				return(status);							 					
			}
	
			
   			(void)fbCodeMemInfoRead	(
										LOGIC_ID,
       									onlineFbDelInfoPtr->fbBlkAddr,
       									(strFbMemInfo *) &fbMemInfo
   									);
			if(fbMemInfo.fbId == 0x0) {
				status = FB_DEL_PERMISSION_ERR;
				setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeDelFuncInFbTask",status,
										"Block Addr", onlineFbDelInfoPtr->fbBlkAddr,
										"FB Code",fbMemInfo.fbId,
										"Not Using", 0
							 		);
				return(status);							 									
			}
			
			
			if(onlineFbDelInfoPtr->fbId > MAX_FB_CODE_ID) {
				status = L_FB_CODE_ERR;
	       		setErrorCodeWithVal( 	__FILE__,__LINE__,"onlineDwlFbCodeDelFuncInFbTask",
            	  	     	    		status,
                		   	    		"Add FB Code",onlineFbDelInfoPtr->fbId,
                 		  	    		"Max FB Code ID ", MAX_FB_CODE_ID,
                  	 		    		"Not Using", 0 
				  		  			);
				 return(status);				
			}
			else if( onlineFbDelInfoPtr->fbId != gFbCodeSortingTableInfo[onlineFbDelInfoPtr->fbId].fbId){
				status = L_FB_CODE_ERR;
	       		setErrorCodeWithVal( 	__FILE__,__LINE__,"onlineDwlFbCodeDelFuncInFbTask",
            	  	     	    		status,
                		   	    		"Add FB Code",onlineFbDelInfoPtr->fbId,
                 		  	    		"Defined Fb Code ", gFbCodeSortingTableInfo[onlineFbDelInfoPtr->fbId].fbId,
                  	 		    		"Not Using", 0 
				  		  			);
				return(status);
			}

		}  		
	} 

	if((status != NO_ERROR)) return(status);

	
	onlineSegDelInfoPtr = onlineFbDelMngrInfoPtr->delSegInfoPtr;

	for(segDelLoop = 0; segDelLoop < onlineFbDelMngrInfoPtr->numOfTotalSegment; 
		segDelLoop++,onlineSegDelInfoPtr++)
	{

		onlineFbDelInfoPtr  = &onlineSegDelInfoPtr->delFbCodeInfo[0];

		rmtComDbSefFlag			= 0;
		
		if( (status = segTblPointerGet( LOGIC_ID,segDelLoop+1, &segTblPtr ) ) == NO_ERROR) 
		{
			segTblPtr->fbUsedNum -= onlineSegDelInfoPtr->numOfFbCode;
		}    
		else {
			setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
				(uint8 *)"onlineDwlFbCodeDelFuncInFbTask",status
			);
		}
		totalDelInputBytes = 0;
		totalDelOutputBytes = 0;

		for(loop = 0; loop <onlineSegDelInfoPtr->numOfFbCode; loop++, onlineFbDelInfoPtr++)
		{
			
			if( (onlineFbDelInfoPtr->fbId == ETH_RMT_STATUS_READ_FB_CODE) ||
				(onlineFbDelInfoPtr->fbId == ETH_RMT_DATA_SEND_FB_CODE) ||
				(onlineFbDelInfoPtr->fbId == MBT_RMT_DATA_SEND_FB_CODE) ||
				(onlineFbDelInfoPtr->fbId == ETH_RMT_DATA_BROAD_FB_CODE) ||
				(onlineFbDelInfoPtr->fbId == ETH_RMT_DATA_READ_FB_CODE_FOR_REPLACE) ||
				(onlineFbDelInfoPtr->fbId == ETH_RMT_DATA_WRITE_FB_CODE_FOR_REPLACE))				
				rmtComDbSefFlag = 1;

			
			specAndVarNum = gFbCodeSortingTableInfo[onlineFbDelInfoPtr->fbId].inputNo;
			specAndVarNum += gFbCodeSortingTableInfo[onlineFbDelInfoPtr->fbId].inputSize;
			specAndVarNum += gFbCodeSortingTableInfo[onlineFbDelInfoPtr->fbId].intlVarSize;
			
			totalDelInputBytes += specAndVarNum;
			totalDelOutputBytes += gFbCodeSortingTableInfo[onlineFbDelInfoPtr->fbId].outputSize;
			
			
			status = fbSpecMemorySpaceRelease(	onlineFbDelInfoPtr->fbId, 
												onlineFbDelInfoPtr->fbBlkAddr
											);
			if(status != NO_ERROR) {
				setErrorCodeWithVal(	__FILE__,__LINE__, "onlineDwlFbCodeDelFuncInFbTask", status,
										"Segment ID",onlineSegDelInfoPtr->taskId,
										"Fb Code", onlineFbDelInfoPtr->fbId,
										"Fb Addr", onlineFbDelInfoPtr->fbBlkAddr
									 );
				return(status);	
			}					
			
			status = fbOutputMemorySpaceRelease(	onlineFbDelInfoPtr->fbId, 
												onlineFbDelInfoPtr->fbBlkAddr
											);
			if(status != NO_ERROR) {
				setErrorCodeWithVal(	__FILE__,__LINE__, "onlineDwlFbCodeDelFuncInFbTask", status,
										"Segment ID",onlineSegDelInfoPtr->taskId,
										"Fb Code", onlineFbDelInfoPtr->fbId,
										"Fb Addr", onlineFbDelInfoPtr->fbBlkAddr
									 );
				return(status);	
			}				
			
			
			(void)onlineDwlFbCodeDelInLocalAndBusMemory(onlineFbDelInfoPtr);
			
		
			if (onlineSegDelInfoPtr->taskId == onlineFbDelInfoPtr->taskId)
			{
				if(onlineSegDelInfoPtr->logicType == FB_LOGIC_TYPE){
					
					status = fbExecIdInfoDelete(	onlineSegDelInfoPtr->taskId, 
													onlineFbDelInfoPtr->fbId, 
													onlineFbDelInfoPtr->fbBlkAddr
												);
					if(status != NO_ERROR) {
						setErrorCodeWithVal(	__FILE__,__LINE__, "onlineDwlFbCodeDelFuncInFbTask", status,
												"Segment ID",onlineSegDelInfoPtr->taskId,
												"Fb Code", onlineFbDelInfoPtr->fbId,
												"Fb Addr", onlineFbDelInfoPtr->fbBlkAddr
											 );
						return(status);	
					}					
				}
				
				else if( (onlineSegDelInfoPtr->logicType == SFC_LOGIC_TYPE) &&
						(onlineFbDelInfoPtr->logicType == SFC_LOGIC_TYPE) ) 
				{
					if(onlineFbDelInfoPtr->usingType == TRANSITION_TYPE) 
					{
							status = trsExecIdInfoDelete(	onlineFbDelInfoPtr->TransitionID, 
															onlineFbDelInfoPtr->fbId, 
															onlineFbDelInfoPtr->fbBlkAddr
														);
							if(status != NO_ERROR) {
								setErrorCodeWithVal(	__FILE__,__LINE__, "onlineDwlFbCodeDelFuncInFbTask", status,
														"TRS ID",onlineFbDelInfoPtr->TransitionID, 
														"Fb Code", onlineFbDelInfoPtr->fbId,
														"Fb Addr", onlineFbDelInfoPtr->fbBlkAddr
													 );
								return(status);	
							}
					}
					else if(onlineFbDelInfoPtr->usingType == ACTION_TYPE) 
					{
						status = actExecIdInfoDelete(	onlineFbDelInfoPtr->ActionID, 
														onlineFbDelInfoPtr->fbId, 
														onlineFbDelInfoPtr->fbBlkAddr
													);
						if(status != NO_ERROR) {
							setErrorCodeWithVal(	__FILE__,__LINE__, "onlineDwlFbCodeDelFuncInFbTask", status,
													"TRS ID",onlineFbDelInfoPtr->ActionID, 
													"Fb Code", onlineFbDelInfoPtr->fbId,
													"Fb Addr", onlineFbDelInfoPtr->fbBlkAddr
												 );
							return(status);	
						}
					}
					else{
						status = SFC_TYPE_ID_ERR;
						setErrorCodeWithVal(	__FILE__,__LINE__, "onlineDwlFbCodeDelFuncInFbTask", status,
								"FB Type",onlineFbDelInfoPtr->usingType, 
								"Fb Code", onlineFbDelInfoPtr->fbId,
								"Fb Addr", onlineFbDelInfoPtr->fbBlkAddr
							 );
						return(status);	
					}
				}
				else{
					status = L_LOGIC_ID_ERR;
					setErrorCodeWithVal(	__FILE__,__LINE__, "onlineDwlFbCodeDelFuncInFbTask", status,
							"Segment ID",onlineSegDelInfoPtr->taskId, 
							"Fb Code", onlineFbDelInfoPtr->fbId,
							"Fb Addr", onlineFbDelInfoPtr->fbBlkAddr
						 );
					return(status);	
				}
			}
			else{
				status = L_SEG_ID_ERR;
				setErrorCodeWithVal(	__FILE__,__LINE__, "onlineDwlFbCodeDelFuncInFbTask", status,
						"Segment ID",onlineSegDelInfoPtr->taskId, 
						"Fb Task ID", onlineFbDelInfoPtr->taskId,
						"Fb Addr", onlineFbDelInfoPtr->fbBlkAddr
					 );
				return(status);	
			}
			
		} 
		
		
		
    	if(rmtComDbSefFlag) {
			status = rmtEthComSeqNumChkDBSet(	onlineSegDelInfoPtr->taskId);
			if(status != NO_ERROR){
            	setErrorCode (__FILE__,__LINE__,"onlineDwlFbCodeDelFuncInFbTask", status);
				return(status);
			}
		}

		
		status = onlineDwlSegmentCheckSumCalcu(	onlineSegDelInfoPtr->taskId,
												ONLINE_DWL_FBCODE_DEL,
												totalDelInputBytes,totalDelOutputBytes
											  );
		if(status != NO_ERROR) {
			setErrorCodeWithVal (	__FILE__,__LINE__,"onlineDwlFbCodeDelFuncInFbTask", status,
									"Seg ID", onlineSegDelInfoPtr->taskId,
									"Not Using",0,
									"Not Using",0
								   );
			break;
		}
	} 
	
	return(status);
}


static uint32 onlineDwlFbCodeDelInLocalAndBusMemory(strOnlineDwlDelFbCodeInfo	*delFbCodeInfo)
{
	uint32 status = NO_ERROR;
	uint32	*specStartPtr, *outputStartPtr;		
	strFbMemInfo	*fbMemInfoPtr;	
	uint32	delOutputIndex, delSpecAndVarNum, delOutputNum;
	uint32  delSpecActionIndex;
	
	
	fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (delFbCodeInfo->fbBlkAddr-1);
	
	
	delSpecActionIndex	= fbMemInfoPtr->specActionIndex;
	delOutputIndex		= fbMemInfoPtr->outputIndex;
	delSpecAndVarNum 	= fbMemInfoPtr->inputNo +fbMemInfoPtr->inputSize + fbMemInfoPtr->intlVarSize;
	delOutputNum		= fbMemInfoPtr->outputSize;
	
	
	memoryClear( (uint8 *) fbMemInfoPtr, sizeof(strFbMemInfo));	
	
	
	specStartPtr = (uint32 *)gMpuSpecAndVarDataLocalPtr + delSpecActionIndex;
	memoryClear( (uint8 *)specStartPtr, delSpecAndVarNum * 4);

	
	outputStartPtr = (uint32 *)gMpuOutputDataLocalPtr + delOutputIndex;
	memoryClear( (uint8 *)outputStartPtr, delOutputNum * 4); 
	
	
	fbMemInfoPtr = gMpuFbMemInfoBusPtr + (delFbCodeInfo->fbBlkAddr-1);
	
	
	memoryClear( (uint8 *) fbMemInfoPtr, sizeof(strFbMemInfo));	
	
	
	specStartPtr = (uint32 *)gMpuSpecAndVarDataBusPtr + delSpecActionIndex;
	memoryClear( (uint8 *)specStartPtr, delSpecAndVarNum * 4);

	
	outputStartPtr = (uint32 *)gMpuOutputDataBusPtr + delOutputIndex;
	memoryClear( (uint8 *)outputStartPtr, delOutputNum * 4); 
	
	return(status);
}


static uint32 fbSpecMemorySpaceRelease (	uint32	fbId, 
											uint32	fbBlkAddr
										)
{
	uint32	status = NO_ERROR;
	strFbSpecUnusedMemInfo	*curNodePtr, *reqNodePtr, *saveNodePtr;
	uint32	unUsedMemByteNum;
	strFbMemInfo	*fbMemInfoPtr;	
	
	
	fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (fbBlkAddr-1);
	
	status = semaphoreGet(sFbSpecMemUnusedQSemaId, NOWAIT);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "fbSpecMemorySpaceRelease",status);
		return(status);
	}
	
	
	curNodePtr = (strFbSpecUnusedMemInfo *)sFbSpecUnusedMemQMngrInfo.usedList;
	
	if(curNodePtr == NULL) {
			
		reqNodePtr = (strFbSpecUnusedMemInfo *)nodeGetAndUnlink(
                    		(strDoubleLink **)&sFbSpecUnusedMemQMngrInfo.freeList
																);
		if(reqNodePtr == NULL){
			status = NO_EXIST_NODE_ERR;
			setErrorCode(	__FILE__,__LINE__,"fbSpecMemorySpaceRelease", status);
			goto MODULE_END;
		}
		unUsedMemByteNum = (fbMemInfoPtr->inputNo + fbMemInfoPtr->inputSize+ fbMemInfoPtr->intlVarSize) * 4;
		
		
		reqNodePtr->usableMemSize			= unUsedMemByteNum;	
		reqNodePtr->unUsedMemStartIndex		= fbMemInfoPtr->specActionIndex;
		reqNodePtr->unUsedMemEndIndex		= fbMemInfoPtr->specActionIndex + (unUsedMemByteNum/4) - 1;
		reqNodePtr->freeMemSize				= unUsedMemByteNum;	
	
		
		if( reqNodePtr->unUsedMemStartIndex> reqNodePtr->unUsedMemEndIndex){
			setErrorCode(	__FILE__,__LINE__,"fbSpecMemorySpaceRelease", status);
			goto MODULE_END;
		}	
		
		status = nodePutOnLast( (strDoubleLink **) &sFbSpecUnusedMemQMngrInfo.usedList,
       							(strDoubleLink *)reqNodePtr
        	  			 	 );
		if(status != NO_ERROR)
			setErrorCode(	__FILE__,__LINE__,"fbSpecMemorySpaceRelease", status);

		else {
			
			fineSameMemSizeAndInfoSet();
		}
		goto MODULE_END;	
	}
	else {
			
		
		
		saveNodePtr = NULL;

		unUsedMemByteNum = (fbMemInfoPtr->inputNo + fbMemInfoPtr->inputSize + fbMemInfoPtr->intlVarSize) * 4;

		while(curNodePtr != NULL){
			if( 
				((curNodePtr->unUsedMemEndIndex + 1) == fbMemInfoPtr->specActionIndex) ||
				(curNodePtr->unUsedMemStartIndex == (fbMemInfoPtr->specActionIndex+ (unUsedMemByteNum/4)))
			){
				saveNodePtr = curNodePtr;
				break;	
			}
	
			curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbSpecUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
    	                      									);	
		}
		
		if(curNodePtr != NULL) {
			
			if( (curNodePtr->unUsedMemEndIndex + 1) == fbMemInfoPtr->specActionIndex){
					
				
				saveNodePtr->usableMemSize			+= unUsedMemByteNum;	
				saveNodePtr->freeMemSize			+= unUsedMemByteNum;	
				saveNodePtr->unUsedMemEndIndex		+= (unUsedMemByteNum/4);
				
				
				saveNodePtr->memUsingFlag	= MEM_NOT_USING_IN_FUTURE;
				saveNodePtr->reservedFbAddr	= 0;
				saveNodePtr->reservedFbCode	= 0;
			}
			else {
					
				
				saveNodePtr->usableMemSize			+= unUsedMemByteNum;	
				saveNodePtr->freeMemSize			+= unUsedMemByteNum;	
				saveNodePtr->unUsedMemStartIndex	= fbMemInfoPtr->specActionIndex;

				
				saveNodePtr->memUsingFlag	= MEM_NOT_USING_IN_FUTURE;
				saveNodePtr->reservedFbAddr	= 0;
				saveNodePtr->reservedFbCode	= 0;
			}

			
			
			curNodePtr = (strFbSpecUnusedMemInfo *)sFbSpecUnusedMemQMngrInfo.usedList;
			while(curNodePtr != NULL) {
				if( 
					((saveNodePtr->unUsedMemEndIndex + 1) == curNodePtr->unUsedMemStartIndex) ||
					(saveNodePtr->unUsedMemStartIndex == (curNodePtr->unUsedMemEndIndex + 1))
				){
					break;	
				}
				
				curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbSpecUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
    	                      										);	
			}
			if(curNodePtr == NULL) goto MODULE_END;
				
			if( (saveNodePtr->unUsedMemEndIndex + 1) == curNodePtr->unUsedMemStartIndex){
					
				
				saveNodePtr->usableMemSize			+= curNodePtr->usableMemSize;	
				saveNodePtr->freeMemSize			+= curNodePtr->freeMemSize;	
				saveNodePtr->unUsedMemEndIndex		= curNodePtr->unUsedMemEndIndex;
				
				saveNodePtr->memUsingFlag	= MEM_NOT_USING_IN_FUTURE;
				saveNodePtr->reservedFbAddr	= 0;
				saveNodePtr->reservedFbCode	= 0;
			}
			else {
					
				
				saveNodePtr->usableMemSize			+= curNodePtr->usableMemSize;	
				saveNodePtr->freeMemSize			+= curNodePtr->freeMemSize;	
				saveNodePtr->unUsedMemStartIndex	= curNodePtr->unUsedMemStartIndex;
				
				saveNodePtr->memUsingFlag	= MEM_NOT_USING_IN_FUTURE;
				saveNodePtr->reservedFbAddr	= 0;
				saveNodePtr->reservedFbCode	= 0;
			}
			
			
   	        status = nodeUnlink( 	
   	        					(strDoubleLink **)&sFbSpecUnusedMemQMngrInfo.usedList,
   								(strDoubleLink *)curNodePtr
            					);
		                            		
			if(status != NO_ERROR){
   				setErrorCode( __FILE__, __LINE__, "fbSpecMemorySpaceRelease",status);
				goto MODULE_END;	
			}
			
			status = nodePutOnLast( (strDoubleLink **) &sFbSpecUnusedMemQMngrInfo.freeList,
       		   						(strDoubleLink *)curNodePtr
           	   		  			  );
			if(status != NO_ERROR){
   				setErrorCode( __FILE__, __LINE__, "fbSpecMemorySpaceRelease",status);
				goto MODULE_END;	
			}
		} 
		else if((curNodePtr == NULL)) { 
			
			
			reqNodePtr = (strFbSpecUnusedMemInfo *)nodeGetAndUnlink(
                    			(strDoubleLink **)&sFbSpecUnusedMemQMngrInfo.freeList
																	);
			if(reqNodePtr == NULL){
				status = NO_EXIST_NODE_ERR;
				setErrorCode(	__FILE__,__LINE__,"fbSpecMemorySpaceRelease", status);
				goto MODULE_END;
			}
			unUsedMemByteNum = (fbMemInfoPtr->inputNo + fbMemInfoPtr->inputSize + fbMemInfoPtr->intlVarSize) * 4;
		
			
			reqNodePtr->usableMemSize			= unUsedMemByteNum;	
			reqNodePtr->unUsedMemStartIndex		= fbMemInfoPtr->specActionIndex;
			reqNodePtr->unUsedMemEndIndex		= fbMemInfoPtr->specActionIndex + (unUsedMemByteNum/4) - 1;
			reqNodePtr->freeMemSize				= unUsedMemByteNum;	
		
	
			
			if( reqNodePtr->unUsedMemStartIndex> reqNodePtr->unUsedMemEndIndex){
				setErrorCode(	__FILE__,__LINE__,"fbSpecMemorySpaceRelease", status);
				goto MODULE_END;
			}	

			status = nodePutOnLast( (strDoubleLink **) &sFbSpecUnusedMemQMngrInfo.usedList,
       								(strDoubleLink *)reqNodePtr
        	  			 	 	  );
        	  			 	 	  
			if(status != NO_ERROR)
				setErrorCode(	__FILE__,__LINE__,"fbSpecMemorySpaceRelease", status);

			else {
				
				fineSameMemSizeAndInfoSet();
			}
			goto MODULE_END;	
		}
	} 


MODULE_END:	
#ifdef VXWORKS
	semaphoreRelease (sFbSpecMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbSpecMemUnusedQSemaId);	
#endif
	return(status);
}


static void fineSameMemSizeAndInfoSet(void)
{
	strFbSpecUnusedMemInfo	*curNodePtr;
	strOnlineDwlAddFbCodeMngrInfo	*onlineFbAddMngrInfoPtr = &sOnlineDwlFbAddInfo;
	strOnlineDwlAddSegInfo			*onlineFbAddSegInfoPtr;
	strOnlineDwlAddFbCodeInfo		*onlineFbAddInfoPtr;
	uint32	loop, specAndVarNum = 0, totalAddBytes = 0;
	
	
	if(	(onlineFbAddMngrInfoPtr->numOfTotalSegment == 0)|| 
		(onlineFbAddMngrInfoPtr->numOfTotalFbCode == 0))
		return;
	
	
	onlineFbAddSegInfoPtr = onlineFbAddMngrInfoPtr->addSegInfoPtr;

	
	
		
	curNodePtr = (strFbSpecUnusedMemInfo *)sFbSpecUnusedMemQMngrInfo.usedList;
	
	while(curNodePtr != NULL) {
		onlineFbAddInfoPtr = &onlineFbAddSegInfoPtr->addFbCodeInfo[0];

		
		if(curNodePtr->memUsingFlag == MEM_USING_IN_FUTURE)
			goto NEXT_NODE;
			
		for(loop = 0; loop < onlineFbAddSegInfoPtr->numOfFbCode; loop++, onlineFbAddInfoPtr++) {
			
			if(onlineFbAddInfoPtr->memReservedFlag == MEM_USING_IN_FUTURE)
				continue;
				
			specAndVarNum = gFbCodeSortingTableInfo[onlineFbAddInfoPtr->fbId].inputNo;
			specAndVarNum += gFbCodeSortingTableInfo[onlineFbAddInfoPtr->fbId].inputSize;
			specAndVarNum += gFbCodeSortingTableInfo[onlineFbAddInfoPtr->fbId].intlVarSize;
			
			totalAddBytes = (specAndVarNum * 4);
			
			if(curNodePtr->freeMemSize == totalAddBytes){
				
				onlineFbAddInfoPtr->memReservedFlag = MEM_USING_IN_FUTURE;
				
				curNodePtr->memUsingFlag 	= MEM_USING_IN_FUTURE;
				curNodePtr->reservedFbAddr 	= onlineFbAddInfoPtr->fbBlkAddr;
				curNodePtr->reservedFbCode 	= onlineFbAddInfoPtr->fbId;
			}
		}
NEXT_NODE:		
		curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
											(strDoubleLink *)sFbSpecUnusedMemQMngrInfo.usedList,
                      						(strDoubleLink *)curNodePtr
                      										);	
	}	
}



static uint32 onlineDwlFbCodeSpecChgFuncInFbTask(strOnlineDwlTaskComReqInfo *argPtr)
{
	uint32	status = NO_ERROR;
	uint32	numOfSegment, numOfSpecChgFbCode, segmentId,fbId, sumOfFbCodeNum, totalFbCodeNum;	
	uint32	fbBlkAddr, numOfChangeSpec, specIndexLoop;
	strFbTaskTblMemInfo	segTblInfo, *segTblInfoPtr;
	strFbMemInfo	fbMemInfo;
	uint32	maxSegId, loop, fbSpecChgLoop, segSpecChgLoop;
	strOnlineDwlSpecChgFbCodeMngrInfo	*onlineFbSpecChgMngrInfoPtr = &sOnlineDwlFbSpecChgInfo;
	strOnlineDwlSpecChgSegInfo			*onlineSegSpecChgInfoPtr = onlineFbSpecChgMngrInfoPtr->specChgSegInfoPtr;
	strOnlineDwlSpecChgFbCodeInfo		*onlineFbSpecChgInfoPtr;
	strOnlineDwlChgSpecInfo				*specDataInfoPtr;
	uint32  trsActID = 0;
	
			
	
	if(onlineFbSpecChgMngrInfoPtr->dwlTotalFrameNum != onlineFbSpecChgMngrInfoPtr->dwlCurFrameNum) {
		status = ONLINE_DWL_FRAME_NUM_ERR; 
		setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeSpecChgFuncInFbTask",status,
								"Total Frame Number", onlineFbSpecChgMngrInfoPtr->dwlTotalFrameNum,
								"Current Frame Number",onlineFbSpecChgMngrInfoPtr->dwlCurFrameNum,
								"Not Using", 0
							 );
		return(status);							 							
	}
	
	numOfSegment = onlineFbSpecChgMngrInfoPtr->numOfTotalSegment;
	totalFbCodeNum = onlineFbSpecChgMngrInfoPtr->numOfTotalFbCode;

    if( (numOfSegment == 0) || (numOfSegment > MAX_ONLINE_DWL_SEG_SPEC_CHANGE_NUM)){
		status = FB_NUM_EXCEED_ERR;
       	setErrorCodeWithVal( 	__FILE__,
       							__LINE__,
               	        		"onlineDwlFbCodeSpecChgFuncInFbTask",
                   	    		status,
                   	    		"Total Segment Blk Chg Number",numOfSegment,
                   	    		"MAX Segment Blk Chg Number", MAX_ONLINE_DWL_SEG_SPEC_CHANGE_NUM,
                   	    		"NOT Using", 0 
				  	  		);
		goto MODULE_END;
	}
    else if( (totalFbCodeNum == 0) || (totalFbCodeNum > MAX_ONLINE_DWL_FB_SPEC_CHANGE_NUM)){
		status = FB_NUM_EXCEED_ERR;
       	setErrorCodeWithVal( 	__FILE__,
       							__LINE__,
               	        		"onlineDwlFbCodeSpecChgFuncInFbTask",
                   	    		status,
                   	    		"Total Fb Code Blk Chg Number",totalFbCodeNum,
                   	    		"MAX FB Code Blk Chg Number", MAX_ONLINE_DWL_FB_SPEC_CHANGE_NUM,
                   	    		"NOT Using", 0 
				  	  		);
		goto MODULE_END;
	}
	
	sumOfFbCodeNum = 0;
	for(segSpecChgLoop = 0; segSpecChgLoop < numOfSegment; segSpecChgLoop++,onlineSegSpecChgInfoPtr++) {
		numOfSpecChgFbCode = onlineSegSpecChgInfoPtr->numOfFbCode;
		
		sumOfFbCodeNum += numOfSpecChgFbCode;
		
		segmentId = onlineSegSpecChgInfoPtr->taskId;
		
		if(sumOfFbCodeNum > totalFbCodeNum) {
			status = ONLINE_FB_CODE_NUM_MISMATCH_ERR;
			setErrorCodeWithVal (	__FILE__,__LINE__, "onlineDwlFbCodeSpecChgFuncInFbTask", status,
									"Total FB Code NUM",totalFbCodeNum,
									"Sum Of Fb Code NUM",sumOfFbCodeNum,
									"NOT USING", 0
								   );
			goto MODULE_END;  
		}
		
		if((segmentId == 0) || (segmentId > MAX_LOGIC_TASK_NUM)) {
			status = L_SEG_ID_ERR;
       		setErrorCodeWithVal( 	__FILE__,
       								__LINE__,
          	     	        		"onlineDwlFbCodeSpecChgFuncInFbTask",
           	        	    		status,
           	        	    		"Segment ID",segmentId,
           	        	    		"MAX Segment ID", MAX_LOGIC_TASK_NUM,
           	        	    		"NOT Using", 0 
					  	  		);
			goto MODULE_END;
		}

		
		maxSegId = 0;
		for(loop = 1; loop <= MAX_LOGIC_TASK_NUM; loop++) {
			memoryClear( (uint8 *)&segTblInfo, sizeof(strFbTaskTblMemInfo));
		
   			(void)getFbTaskTblInfo( 
								LOGIC_ID,
           						loop,
								(strFbTaskTblMemInfo	*) &segTblInfo
								);
			if(segTblInfo.taskId > maxSegId)
				maxSegId = segTblInfo.taskId;
			
			if(segTblInfo.taskId == segmentId) break;
		}

		if(segTblInfo.taskId == 0) {
			status = ONLINE_DWL_SEG_EXIST_ERR;
       		setErrorCodeWithVal( 	__FILE__,
       								__LINE__,
           	    	        		"onlineDwlFbCodeSpecChgFuncInFbTask",
              	     	    		status,
                	   	    		"Segment ID",segmentId,
                 	  	    		"Current Max Seg Id", maxSegId,
                  	 	    		"NOT Using", 0 
				  		  		);
			goto MODULE_END;		
		}
		
		numOfSpecChgFbCode = onlineSegSpecChgInfoPtr->numOfFbCode;
		
		onlineFbSpecChgInfoPtr = &onlineSegSpecChgInfoPtr->specChgFbCodeInfo[0];
		
		for(fbSpecChgLoop = 0; fbSpecChgLoop < numOfSpecChgFbCode; fbSpecChgLoop++,onlineFbSpecChgInfoPtr++) {
			fbId 		= onlineFbSpecChgInfoPtr->fbId;
			fbBlkAddr	= onlineFbSpecChgInfoPtr->fbBlkAddr;
			
			numOfChangeSpec = onlineFbSpecChgInfoPtr->numOfChangedSpec;
			
    		(void)fbCodeMemInfoRead	(
										LOGIC_ID,
       									fbBlkAddr,
       									(strFbMemInfo *) &fbMemInfo
       								);
       								
			
			if( (fbBlkAddr < 1) ||
				(fbBlkAddr > MAX_MPU_BLOCK_ADDR)
			){
				status = SEG_BLK_ADDR_EXCEED_ERR;
	       		setErrorCodeWithVal( 	__FILE__, __LINE__,
        	   	    	        		"onlineDwlFbCodeSpecChgFuncInFbTask",status,
                		   	    		"FB Addr",fbBlkAddr,
                 		  	    		"MAX FB Addr", MAX_MPU_BLOCK_ADDR,
                  	 		    		"Not Using", 0 
				  		  			);
							  		  				
			}   
			else if(fbId > MAX_FB_CODE_ID){
				status = L_FB_CODE_ERR;
	       		setErrorCodeWithVal( 	__FILE__,
    	   								__LINE__,
        	   	    	        		"onlineDwlFbCodeSpecChgFuncInFbTask",
            	  	     	    		status,
                		   	    		"FB Code",fbId,
                 		  	    		"Max FB Code ", MAX_FB_CODE_ID,
                  	 		    		"Not Using", 0 
				  		  			);
			}				
			
			else if( fbId != gFbCodeSortingTableInfo[fbId].fbId){
				status = L_FB_CODE_ERR;
	       		setErrorCodeWithVal( 	__FILE__,
    	   								__LINE__,
        	   	    	        		"onlineDwlFbCodeSpecChgFuncInFbTask",
            	  	     	    		status,
                		   	    		"FB Code",fbId,
                 		  	    		"Defined Fb Code ", gFbCodeSortingTableInfo[fbId].fbId,
                  	 		    		"Not Using", 0 
				  		  			);
			}				

			
			else if(fbId != fbMemInfo.fbId) {
				status = ONLINE_FB_CODE_MISMATCH_ERR;
	       		setErrorCodeWithVal( 	__FILE__,
    	   								__LINE__,
        	   	   		        		"onlineDwlFbCodeSpecChgFuncInFbTask",
            	      		    		status,
                	   	    			"Block Addr",fbBlkAddr,
                 	  	    			"Request FB Code ", fbId,
                  		    			"Read FB Code", fbMemInfo.fbId 
				 	  				);
			}
			
			else if(numOfChangeSpec > fbMemInfo.inputNo){
				status = L_PARA_NO_ERR;
	       		setErrorCodeWithVal( 	__FILE__,
    	   								__LINE__,
        	    		        		"onlineDwlFbCodeSpecChgFuncInComTask",
              	     		    		status,
               		   	    			"fb Code",fbId,
               		  	    			"Request Change Spec Num ", numOfChangeSpec,
               	 		    			"FB Code Spec Num", fbMemInfo.inputNo 
			  		  				);
			}
			else if(numOfChangeSpec > MAX_FB_SPEC_PERMISSION_NUM){
				status = L_PARA_NO_ERR;
	       		setErrorCodeWithVal( 	__FILE__,
    	   								__LINE__,
        	    		        		"onlineDwlFbCodeSpecChgFuncInComTask",
              	     		    		status,
               		   	    			"fb Code",fbId,
               		  	    			"Request Change Spec Num ", numOfChangeSpec,
               	 		    			"FB Code Spec Num", fbMemInfo.inputNo 
			  		  				);
			}
			
			specDataInfoPtr = &onlineFbSpecChgInfoPtr->specDataInfo[0];
			for(specIndexLoop = 0; specIndexLoop < numOfChangeSpec; specIndexLoop++, specDataInfoPtr++){
				
				if(	(specDataInfoPtr->specIndex == 0) ||
					(specDataInfoPtr->specIndex > fbMemInfo.inputNo)	
				){
					status = FB_PARA_INDEX_ERR;
	       			setErrorCodeWithVal( 	__FILE__,
    	   									__LINE__,
        	    		        			"onlineDwlFbCodeSpecChgFuncInComTask",
              	     		    			status,
               		   	    				"fb Code",fbId,
               		  	    				"Request Change Spec Index ", specDataInfoPtr->specIndex,
               	 		    				"FB Code Spec Num", fbMemInfo.inputNo 
			  		  					);
					goto MODULE_END;
				}
			}
		}	
	} 
	
	
	if(status != NO_ERROR)
		goto MODULE_END;
	
	if(sumOfFbCodeNum != totalFbCodeNum) {
		status = ONLINE_FB_CODE_NUM_MISMATCH_ERR;
		setErrorCodeWithVal (	__FILE__,__LINE__, "onlineDwlFbCodeSpecChgFuncInFbTask", status,
								"Total FB Code NUM",totalFbCodeNum,
								"Sum Of Fb Code NUM",sumOfFbCodeNum,
								"NOT USING", 0
							   );
		goto MODULE_END;  
	}
		
	onlineSegSpecChgInfoPtr = onlineFbSpecChgMngrInfoPtr->specChgSegInfoPtr;
	
	for(segSpecChgLoop = 0; segSpecChgLoop < numOfSegment; segSpecChgLoop++, onlineSegSpecChgInfoPtr++){
		 
		status = segTblPointerGet( LOGIC_ID,
           						onlineSegSpecChgInfoPtr->taskId,
								(strFbTaskTblMemInfo	**) &segTblInfoPtr
								);
		if(status == NO_ERROR) {
			segTblInfoPtr->loopStatus = 0;
		}

		numOfSpecChgFbCode	= onlineSegSpecChgInfoPtr->numOfFbCode;
		onlineFbSpecChgInfoPtr = &onlineSegSpecChgInfoPtr->specChgFbCodeInfo[0];
		
		for(fbSpecChgLoop = 0; fbSpecChgLoop < numOfSpecChgFbCode; fbSpecChgLoop++,onlineFbSpecChgInfoPtr++){
			numOfChangeSpec	= onlineFbSpecChgInfoPtr->numOfChangedSpec;
			
			 
			if(onlineFbSpecChgInfoPtr->logicType == SFC_LOGIC_TYPE){
				if(onlineFbSpecChgInfoPtr->usingType == TRANSITION_TYPE)
					trsActID = onlineFbSpecChgInfoPtr->TransitionID;
				else if(onlineFbSpecChgInfoPtr->usingType == ACTION_TYPE)
					trsActID = onlineFbSpecChgInfoPtr->ActionID;
				else status = SFC_TYPE_ID_ERR;

				if(status == NO_ERROR) {
					status = sfcLoopStatusWrite(onlineFbSpecChgInfoPtr->taskId,
								trsActID, onlineFbSpecChgInfoPtr->usingType, 0);
				}

				if(status != NO_ERROR) {
					setErrorCodeWithVal (	__FILE__,__LINE__, "onlineDwlFbCodeSpecChgFuncInFbTask", status,
								"TASK ID",onlineFbSpecChgInfoPtr->taskId,
								"TRSACT ID",trsActID,
								"FB CODE", onlineFbSpecChgInfoPtr->fbBlkAddr
							   );
				}
			}

			
			(void)onlineDwlFbCodeTypeChgInLocalAndBusMemory(	onlineFbSpecChgInfoPtr->fbId,
																onlineFbSpecChgInfoPtr->fbBlkAddr,
																onlineFbSpecChgInfoPtr->paraRunningType
															  );
			
			specDataInfoPtr = &onlineFbSpecChgInfoPtr->specDataInfo[0];

			for(loop = 0; loop < numOfChangeSpec; loop++, specDataInfoPtr++){
				(void)onlineDwlFbCodeSpecChgInLocalAndBusMemory(	onlineFbSpecChgInfoPtr->fbId,
																	onlineFbSpecChgInfoPtr->fbBlkAddr,
																	specDataInfoPtr
																  );
			}
			
		}	
		
		

	} 

MODULE_END:

	return(status);
}


static uint32 onlineDwlFbCodeTypeChgInLocalAndBusMemory(	uint32 fbId, 
															uint32 fbBlkAddr,
															uint32 paraRunningType
														)
{
	uint32 status = NO_ERROR;
	strFbMemInfo	*fbMemInfoPtr;	
	
	
	fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (fbBlkAddr-1);
	fbMemInfoPtr->paraRunType = paraRunningType;
	
	fbMemInfoPtr = gMpuFbMemInfoBusPtr + (fbBlkAddr-1);
	fbMemInfoPtr->paraRunType = paraRunningType;
	
	return(status);
}


static uint32 onlineDwlFbCodeSpecChgInLocalAndBusMemory(	uint32 fbId, 
															uint32 fbBlkAddr,
															strOnlineDwlChgSpecInfo	*specInfoPtr
														)
{
	uint32 status = NO_ERROR;
	uint32	*specActionStartPtr,*specStartPtr;		
	strFbMemInfo	*fbMemInfoPtr;	
	uint32	chgSpecActionIndex, chgSpecIndex;
	
	
	fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (fbBlkAddr-1);

	chgSpecActionIndex = fbMemInfoPtr->specActionIndex + specInfoPtr->specIndex - 1;
	specActionStartPtr = (uint32 *)gMpuSpecAndVarDataLocalPtr + chgSpecActionIndex;
	*specActionStartPtr = specInfoPtr->specActionData;

	chgSpecIndex = fbMemInfoPtr->specIndex + specInfoPtr->specIndex - 1;
	specStartPtr = (uint32 *)gMpuSpecAndVarDataLocalPtr + chgSpecIndex;
	*specStartPtr = specInfoPtr->specData[0];

	if(GET_TYPE_SIZE(specInfoPtr->specType) == SIZE64_TYPE) {
		 
		 *(++specStartPtr) = specInfoPtr->specData[1];
	}
	
	fbMemInfoPtr = gMpuFbMemInfoBusPtr + (fbBlkAddr-1);

	chgSpecActionIndex = fbMemInfoPtr->specActionIndex + specInfoPtr->specIndex - 1;
	specActionStartPtr = (uint32 *)gMpuSpecAndVarDataBusPtr + chgSpecActionIndex;
	*specActionStartPtr = specInfoPtr->specActionData;

	chgSpecIndex = fbMemInfoPtr->specIndex + specInfoPtr->specIndex - 1;
	specStartPtr = (uint32 *)gMpuSpecAndVarDataBusPtr + chgSpecIndex;
	*specStartPtr = specInfoPtr->specData[0];

	if(GET_TYPE_SIZE(specInfoPtr->specType) == SIZE64_TYPE) {
		 
		 *(++specStartPtr) = specInfoPtr->specData[1];
	}

	return(status);
}


static uint32 onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask(strOnlineDwlTaskComReqInfo *argPtr)
{
	uint32	status = NO_ERROR;
	uint32	numOfSegment, numOfSeqIdChgFbCode, segmentId,fbId, sumOfFbCodeNum, totalFbCodeNum;	
	uint32	fbBlkAddr, curSeqId;
	strFbTaskTblMemInfo	segTblInfo;
	strFbMemInfo	*fbLocalMemInfoPtr,*fbBusMemInfoPtr;
	uint32	maxSegId, loop, fbSeqIdChgLoop, segSeqIdChgLoop, startIndex;
	strOnlineDwlFbSeqIdAddAndChgMngrInfo	*onlineFbSeqIdAddAndChgMngrInfoPtr = &sOnlineDwlFbSeqIdAddAndChgInfo;
	strOnlineDwlFbSeqIdAddAndChgSegInfo		*onlineSegSeqIdAddAndChgInfoPtr = &onlineFbSeqIdAddAndChgMngrInfoPtr->fbSeqIdAddAndChgSegInfo[0];
	strOnlineDwlFbSeqIdInfo		*onlineFbSeqIdAddAndChgInfoPtr,*orgOnlineFbSeqIdAddAndChgInfoPtr;

	
	orgOnlineFbSeqIdAddAndChgInfoPtr = &onlineFbSeqIdAddAndChgMngrInfoPtr->fbSeqIdInfo[0];
	
			
	
	if(onlineFbSeqIdAddAndChgMngrInfoPtr->dwlTotalFrameNum != onlineFbSeqIdAddAndChgMngrInfoPtr->dwlCurFrameNum) {
		status = ONLINE_DWL_FRAME_NUM_ERR; 
		setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask",status,
								"Total Frame Number", onlineFbSeqIdAddAndChgMngrInfoPtr->dwlTotalFrameNum,
								"Current Frame Number",onlineFbSeqIdAddAndChgMngrInfoPtr->dwlCurFrameNum,
								"Not Using", 0
							 );
		return(status);							 							
	}
	
	numOfSegment = onlineFbSeqIdAddAndChgMngrInfoPtr->numOfTotalSegment;
	
		
    if( (numOfSegment == 0) || (numOfSegment > MAX_ONLINE_DWL_SEG_SEQ_ID_ADD_AND_CHG_NUM)){
		status = FB_NUM_EXCEED_ERR;
       	setErrorCodeWithVal( 	__FILE__,__LINE__,
               	        		"onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask", status,
                   	    		"Segment Number",numOfSegment,
                   	    		"MAX Segment Number", MAX_ONLINE_DWL_SEG_SEQ_ID_ADD_AND_CHG_NUM,
                   	    		"NOT Using", 0 
				  	  		);
		goto MODULE_END;
	}

	totalFbCodeNum = onlineFbSeqIdAddAndChgMngrInfoPtr->numOfTotalFbCode;
	
    if( (totalFbCodeNum == 0) || (totalFbCodeNum > MAX_ONLINE_DWL_FB_SEQ_ID_ADD_AND_CHG_NUM)){
		status = FB_NUM_EXCEED_ERR;
       	setErrorCodeWithVal( 	__FILE__, __LINE__,
               	        		"onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask", status,
                   	    		"Total Fb Code Number",totalFbCodeNum,
                   	    		"MAX FB Code Number", MAX_ONLINE_DWL_FB_SEQ_ID_ADD_AND_CHG_NUM,
                   	    		"NOT Using", 0 
				  	  		);
		goto MODULE_END;
	}

	
	sumOfFbCodeNum = 0;

	for(segSeqIdChgLoop = 0; segSeqIdChgLoop < numOfSegment; segSeqIdChgLoop++, onlineSegSeqIdAddAndChgInfoPtr++) {
		
		segmentId 			= onlineSegSeqIdAddAndChgInfoPtr->taskId;
		numOfSeqIdChgFbCode	= onlineSegSeqIdAddAndChgInfoPtr->numOfFbCode;

		
		
		startIndex			= onlineSegSeqIdAddAndChgInfoPtr->startIndex;
		
		sumOfFbCodeNum += numOfSeqIdChgFbCode;
		
		if(sumOfFbCodeNum > totalFbCodeNum) {
			status = ONLINE_FB_CODE_NUM_MISMATCH_ERR;
			setErrorCodeWithVal (	__FILE__,__LINE__, "onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask", status,
									"Total FB Code NUM",totalFbCodeNum,
									"Sum Of Fb Code NUM",sumOfFbCodeNum,
									"NOT USING", 0
								   );
			goto MODULE_END;  
		}
		
		if((segmentId == 0) || (segmentId > MAX_LOGIC_TASK_NUM)) {
			status = L_SEG_ID_ERR;
       		setErrorCodeWithVal( 	__FILE__, __LINE__,
          	     	        		"onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask", status,
           	        	    		"Segment ID",segmentId,
           	        	    		"MAX Segment ID", MAX_LOGIC_TASK_NUM,
           	        	    		"NOT Using", 0 
					  	  		);
			goto MODULE_END;
		}

		
		maxSegId = 0;
		for(loop = 1; loop <= MAX_LOGIC_TASK_NUM; loop++) {
			memoryClear( (uint8 *)&segTblInfo, sizeof(strFbTaskTblMemInfo));
		
   			(void)getFbTaskTblInfo( 
								LOGIC_ID,
           						loop,
								(strFbTaskTblMemInfo	*) &segTblInfo
								);
			if(segTblInfo.taskId > maxSegId)
				maxSegId = segTblInfo.taskId;
			
			if(segTblInfo.taskId == segmentId) break;
		}

		if(segTblInfo.taskId == 0) {
			status = ONLINE_DWL_SEG_EXIST_ERR;
       		setErrorCodeWithVal( 	__FILE__,
       								__LINE__,
           	    	        		"onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask",
              	     	    		status,
                	   	    		"Segment ID",segmentId,
                 	  	    		"Current Max Seg Id", maxSegId,
                  	 	    		"NOT Using", 0 
				  		  		);
			goto MODULE_END;		
		}
	
		onlineFbSeqIdAddAndChgInfoPtr = orgOnlineFbSeqIdAddAndChgInfoPtr + startIndex;

		for(fbSeqIdChgLoop = 0; fbSeqIdChgLoop < numOfSeqIdChgFbCode; fbSeqIdChgLoop++,onlineFbSeqIdAddAndChgInfoPtr++) {
			fbBlkAddr	= onlineFbSeqIdAddAndChgInfoPtr->fbBlkAddr;
			fbId 		= onlineFbSeqIdAddAndChgInfoPtr->fbId;

			curSeqId 	= onlineFbSeqIdAddAndChgInfoPtr->fbSeqId;
			
			fbLocalMemInfoPtr 	= gMpuFbMemInfoLocalPtr + (fbBlkAddr-1);	
			fbBusMemInfoPtr 	= gMpuFbMemInfoBusPtr + (fbBlkAddr-1);			
			
			if(	fbLocalMemInfoPtr->fbId != fbBusMemInfoPtr->fbId){
				status = ONLINE_FB_CODE_MISMATCH_ERR;
       			setErrorCodeWithVal( 	__FILE__, __LINE__,
           	    		        		"onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask",status,
                	 	  	    		"Fb Addr",fbBlkAddr,
                 	  		    		"Local Fb Code", fbLocalMemInfoPtr->fbId,
                  	 	 		   		"Bus Fb Code", fbBusMemInfoPtr->fbId 
				  		  			);
				goto MODULE_END;		
			}		
			
			else if( (fbBlkAddr < 1) ||
				(fbBlkAddr > MAX_MPU_BLOCK_ADDR)
			){
				status = SEG_BLK_ADDR_EXCEED_ERR;
	       		setErrorCodeWithVal( 	__FILE__,__LINE__,
        	   	    	        		"onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask",status,
                 		  	    		"SEG ID", segTblInfo.taskId,
                		   	    		"FB Addr",fbBlkAddr,
                  	 		    		"MAX FB Addr", MAX_MPU_BLOCK_ADDR
				  		  			);
				goto MODULE_END;
			}   
			
			else if(fbId > MAX_FB_CODE_ID){
				status = L_FB_CODE_ERR;
	       		setErrorCodeWithVal( 	__FILE__,__LINE__,
        	   	    	        		"onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask",status,
                		   	    		"FB Code",fbId,
                 		  	    		"Max FB Code ", MAX_FB_CODE_ID,
                  	 		    		"Not Using", 0 
				  		  			);
				goto MODULE_END;
			}				
			
			else if( fbId != gFbCodeSortingTableInfo[fbId].fbId){
				status = L_FB_CODE_ERR;
	       		setErrorCodeWithVal( 	__FILE__,__LINE__,
        	   	    	        		"onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask",status,
                		   	    		"FB Code",fbId,
                 		  	    		"Defined Fb Code ", gFbCodeSortingTableInfo[fbId].fbId,
                  	 		    		"Not Using", 0 
				  		  			);
				goto MODULE_END;
			}				
		
			
			else if(fbId != fbLocalMemInfoPtr->fbId) {
				status = ONLINE_FB_CODE_MISMATCH_ERR;
	       		setErrorCodeWithVal( 	__FILE__,
    	   								__LINE__,
        	   	   		        		"onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask",
            	      		    		status,
                	   	    			"Block Addr",fbBlkAddr,
                 	  	    			"Request FB Code ", fbId,
                  		    			"Read FB Code", fbLocalMemInfoPtr->fbId 
				 	  				);
				goto MODULE_END;
			}
			
			else if (curSeqId > MAX_FB_EXEC_SEQ_ID){
				status = FB_SEQ_ID_MAX_EXCEED_ERR;
	       		setErrorCodeWithVal( 	__FILE__,
    	   								__LINE__,
        	   	    	        		"onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask",
            	  	     	    		status,
                		   	    		"Req FB Code",fbId,
                 		  	    		"Request Blk Addr ", fbBlkAddr,
                  	 		    		"Max Seq ID", MAX_FB_EXEC_SEQ_ID 
				  		  			);
			}
			else if (curSeqId == 0x0){
				status = FB_SEQ_ID_NULL_ERR;
	       		setErrorCodeWithVal( 	__FILE__,
    	   								__LINE__,
        	   	    	        		"onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask",
            	  	     	    		status,
                		   	    		"Req FB Code",fbId,
                 		  	    		"Request Blk Addr ", fbBlkAddr,
                  	 		    		"Not Using", 0 
				  		  			);
			}
		}	
	} 
	
	if(sumOfFbCodeNum != totalFbCodeNum) {
		status = ONLINE_FB_CODE_NUM_MISMATCH_ERR;
		setErrorCodeWithVal (	__FILE__,__LINE__, "onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask", status,
								"Total FB Code NUM",totalFbCodeNum,
								"Sum Of Fb Code NUM",sumOfFbCodeNum,
								"NOT USING", 0
							   );
		goto MODULE_END;  
	}
	
	
	numOfSegment 	= onlineFbSeqIdAddAndChgMngrInfoPtr->numOfTotalSegment;
	totalFbCodeNum 	= onlineFbSeqIdAddAndChgMngrInfoPtr->numOfTotalFbCode;

	onlineSegSeqIdAddAndChgInfoPtr = &onlineFbSeqIdAddAndChgMngrInfoPtr->fbSeqIdAddAndChgSegInfo[0];
	for(segSeqIdChgLoop = 0; segSeqIdChgLoop < numOfSegment; segSeqIdChgLoop++, onlineSegSeqIdAddAndChgInfoPtr++){
		
		
		segmentId 			= onlineSegSeqIdAddAndChgInfoPtr->taskId;
		numOfSeqIdChgFbCode	= onlineSegSeqIdAddAndChgInfoPtr->numOfFbCode;
		startIndex			= onlineSegSeqIdAddAndChgInfoPtr->startIndex;
		
		
		onlineFbSeqIdAddAndChgInfoPtr = orgOnlineFbSeqIdAddAndChgInfoPtr + startIndex;

		for(fbSeqIdChgLoop = 0; fbSeqIdChgLoop < numOfSeqIdChgFbCode; fbSeqIdChgLoop++,onlineFbSeqIdAddAndChgInfoPtr++){
			fbId 		= onlineFbSeqIdAddAndChgInfoPtr->fbId;
			fbBlkAddr 	= onlineFbSeqIdAddAndChgInfoPtr->fbBlkAddr;
			curSeqId 	= onlineFbSeqIdAddAndChgInfoPtr->fbSeqId;

					
			
			fbLocalMemInfoPtr = gMpuFbMemInfoLocalPtr + (fbBlkAddr-1);	
			fbLocalMemInfoPtr->fbExecSequenceId = curSeqId;
											
			
			fbBusMemInfoPtr = gMpuFbMemInfoBusPtr + (fbBlkAddr-1);			
			fbBusMemInfoPtr->fbExecSequenceId = curSeqId;
			
		}	
		
		
		fbExeSequenceQueuePtrClear(segmentId);

		if(segTblInfo.logicType == SFC_LOGIC_TYPE)
		{
			
			status = sfcExecIdInfoClear(segmentId, LOGIC_ID);
			if(status != NO_ERROR) {
				setErrorCodeWithVal (	__FILE__,__LINE__, "onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask", status,
										"seg ID",segmentId,
										"NOT USING",0,
										"NOT USING", 0
									   );
				goto MODULE_END;  
			}
			
			
			status = sfcExecIdInfoSet(segmentId, LOGIC_ID);
			if(status != NO_ERROR) {
				setErrorCodeWithVal (	__FILE__,__LINE__, "onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask", status,
										"seg ID",segmentId,
										"NOT USING",0,
										"NOT USING", 0
									   );
				goto MODULE_END;  
			}

		}
		else{

			
			status = fbExecIdInfoClear(segmentId, LOGIC_ID);
			if(status != NO_ERROR) {
				setErrorCodeWithVal (	__FILE__,__LINE__, "onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask", status,
										"seg ID",segmentId,
										"NOT USING",0,
										"NOT USING", 0
									   );
				goto MODULE_END;  
			}
			
			
			status = fbExecIdInfoSet(segmentId, LOGIC_ID);
			if(status != NO_ERROR) {
				setErrorCodeWithVal (	__FILE__,__LINE__, "onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask", status,
										"seg ID",segmentId,
										"NOT USING",0,
										"NOT USING", 0
									   );
				goto MODULE_END;  
			}

			
			status = mpusegSchedulerStartAddrSet(segmentId);
			if(status != NO_ERROR){
				setErrorCode( __FILE__, __LINE__, "onlineDwlFbCodeSeqIdAddAndChgFuncInFbTask",status);
				goto MODULE_END;  
			}
		
		}

	} 
	
		
MODULE_END:
	
	return(status);
}


uint32 onlineDwlFbCodeSeqIdVarInit(uint32 taskId, uint32 dwlKind)
{
	uint32 status = NO_ERROR;

	if(dwlKind == TOTAL_DWL){
		memoryClear ( 	(uint8 *)&sOnlineDwlFbSeqIdDwlVarInfo[0],
						sizeof(strFbSeqIdDwlInfo) * MAX_LOGIC_TASK_NUM
					);	
	}
	else{
		if( (taskId<1)||(taskId>MAX_LOGIC_TASK_NUM) )
			status = L_SEG_ID_ERR;
		else{
			memoryClear ( 	(uint8 *)&sOnlineDwlFbSeqIdDwlVarInfo[taskId-1],
							sizeof(strFbSeqIdDwlInfo)
						);	
		}
	}
	return(status);				
}


uint32	onlineDwlFbCodeSeqIdVarSet(	uint32 	taskId, 
									uint32 	numOfFbCode,
									uint32	totalFbCodeNum
								)
{
	uint32	status = NO_ERROR;
	
	sOnlineDwlFbSeqIdDwlVarInfo[taskId - 1].recvTotalFbCodeNum = totalFbCodeNum;
	sOnlineDwlFbSeqIdDwlVarInfo[taskId - 1].sumOfDwlFbCodeNum += numOfFbCode;
	
	return(status);
}


uint32	onlineDwlFbCodeSeqIdVarCompare(	uint32 	taskId)
{
	uint32	status = NO_ERROR;
	uint32	totalFbCodeNum = 0, calcuFbCodeNum = 0;
	
	totalFbCodeNum 	= sOnlineDwlFbSeqIdDwlVarInfo[taskId - 1].recvTotalFbCodeNum;
	calcuFbCodeNum 	= sOnlineDwlFbSeqIdDwlVarInfo[taskId - 1].sumOfDwlFbCodeNum;
	
	if(totalFbCodeNum != calcuFbCodeNum) {
		status = ONLINE_FB_CODE_NUM_MISMATCH_ERR;
		setErrorCodeWithVal(	__FILE__,
								__LINE__, 
								"onlineDwlFbCodeSeqIdVarCompare",
								status,
								"Seg Id", taskId,
								"Total FB Code Num", totalFbCodeNum,
								"Recv Fb Code Num", calcuFbCodeNum
							  );
	}							  
	return(status);
}


uint32	onlineDwlAccessFlagCheck(uint32 moduleID)
{
	if(moduleID==LOGIC_ID ) {
		if(sMpuOnlineDwlOperationFlag == ONLINE_DWL_OPERATION_START)
			return(1);
		else
			return(0);
	}
	return(0);
}


uint32 fbUnusedSpecMemInfoRead(strFbSpecMemStatusReadRespInfo *respInfoPtr)
{
	uint32 status = NO_ERROR, loop;
	strFbSpecUnusedMemInfo	*nodePtr;

	
    status = semaphoreGet(sFbSpecMemUnusedQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

 	nodePtr = sFbSpecUnusedMemQMngrInfo.usedList;
    if(nodePtr == NULL) {
    	respInfoPtr->fragmentMemNum = 0;
    	goto MODULE_END;
    }
    
    loop = 0;
   	while(nodePtr != NULL) {
    	respInfoPtr->fragmentMemNum++;
		
		if(respInfoPtr->fragmentMemNum > MAX_FB_SPEC_FRAGMENT_NUM) {
			status = FB_SPEC_MEM_EXCEED_ERR;
			setErrorCodeWithVal(	__FILE__, __LINE__,
									"fbUnusedSpecMemInfoRead",status,
									"Fragment MEM NUM", respInfoPtr->fragmentMemNum,
									"MAX MEM NUM", MAX_FB_SPEC_FRAGMENT_NUM,
									"NOT USING", 0
						  		);
			break;
		}
		
		
		respInfoPtr->fbParaMemStatusInfo[loop].fragmentMemEndIndex		= nodePtr->unUsedMemEndIndex;
		respInfoPtr->fbParaMemStatusInfo[loop].fragmentMemStartIndex	= nodePtr->unUsedMemStartIndex;
		
		
		respInfoPtr->freeMemSize += (nodePtr->freeMemSize/4);
		loop++;
		
       	nodePtr = (strFbSpecUnusedMemInfo *)nextNodeRef ( 	
       								(strDoubleLink *)sFbSpecUnusedMemQMngrInfo.usedList,
	       	                		(strDoubleLink *)nodePtr
							  							);
	}

MODULE_END:
	
#ifdef VXWORKS
	semaphoreRelease (sFbSpecMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbSpecMemUnusedQSemaId);	
#endif
	return(status);
}




void fbExeSequenceQueuePtrClear(uint32 taskId)
{
	uint32 i, status;
	strModuleExecInfo sysExeInfo;
	memoryClear( (uint8 *)&sysExeInfo, sizeof(strModuleExecInfo));

	status = fbSysExeModuleInfoRead(LOGIC_ID, &sysExeInfo );
	if(status != NO_ERROR)
	{
		setErrorCode(__FILE__,__LINE__,
			"fbExeSequenceQueuePtrClear",status
			);
		return;
	}

    status = semaphoreGet(gFbExecSeqIdQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR){
		setErrorCode(__FILE__,__LINE__,
			"fbExeSequenceQueuePtrClear",status
			);
		return;
	}

	for(i=0;i<sysExeInfo.curfbMaxBlockAddr;i++){
		if(gFBExeSequenceQueuePtr[i] == NULL) continue;
		if((gFBExeSequenceQueuePtr[i]->taskId == taskId) || (taskId==TOTAL_DWL))
		{
			gFBExeSequenceQueuePtr[i] =  NULL;
		}
	}

	
	(void)fbSysMaxJmpLoopInfo(MAX_MPU_BLOCK_ADDR);

#ifdef VXWORKS
	semaphoreRelease (gFbExecSeqIdQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gFbExecSeqIdQSemaId);	
#endif
	
	return;
}


uint32 onlineDwlVarInfoChgVariableClear(void)
{
	uint32	status = NO_ERROR;
	
	memoryClear( (uint8 *)&sOnlineDwlVarInfoChgInfo, sizeof(strOnlineDwlVarInfoChgMngrInfo));

	
	status = onlineDwlTaskReqQueClear(ONLINE_DWL_VAR_INFO_CHANGE);
	if(status != NO_ERROR) 
		setErrorCode (__FILE__, __LINE__, "onlineDwlVarInfoChgVariableClear", status);
		
	return(status);
}


uint32 onlineDwlTrsDataChgVariableClear(void)
{
	uint32	status = NO_ERROR;
	
	
	sOnlineDwlTrsChgInfo.dwlCurFrameNum = 0;
	sOnlineDwlTrsChgInfo.dwlTotalFrameNum = 0;
	sOnlineDwlTrsChgInfo.dwlTotalTrsNum = 0;
	sOnlineDwlTrsChgInfo.numOfTotalSegment = 0;
	sOnlineDwlTrsChgInfo.numOfTotalTrs = 0;
	memoryClear( (uint8 *)sOnlineDwlTrsChgInfo.chgTrsInfoPtr,
					 sizeof(strOnlineDwlChgTrsSegInfo)*MAX_ONLINE_DWL_SEG_TRS_CHANGE_NUM);

	
	status = onlineDwlTaskReqQueClear(ONLINE_DWL_TRS_DATA_CHANGE);
	if(status != NO_ERROR) 
		setErrorCode (__FILE__, __LINE__, "onlineDwlTrsDataChgVariableClear", status);
		
	return(status);
}



uint32 onlineDwlFbCodeVarInfoChgFuncInComTask(	uint32 	*reqDataPtr, 
											  uint32	*respDataPtr,
											uint32	*respRetLenPtr
											)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32  totalFrameNum, curFrameId, numOfTotalRetainMum, numOfTotalInitNum;
	uint32	numOfRetainNum,numOfInitNum,prevNumOfRetain, prevNumOfInit;

	strOnlineDwlVarInfoChgMngrInfo	*onlineVarInfoChgMngrInfoPtr = &sOnlineDwlVarInfoChgInfo;
	strOnlineDwlVarRetainInfo *onlineVarRetainChgInfoPtr;
	strOnlineDwlVarInitInfo   *onlineVarInitChgInfoPtr;

	uint32  i, varRetainIndex = 0, varRetainKind, *saveRetainPtr;
	uint32  varInitIndex, varInitData[2], intlVarSize, *saveInitDataPtr;
	strFbSpecActionInfo *varActionData;
	uint32  retainData=0, *numOfRetainErr, *numOfInitErr,varInitType;
	
	*respRetLenPtr	= 0;

	
	totalFrameNum	= (*reqDataPtr & 0xffff0000) >> 16;
	curFrameId 		= *reqDataPtr++ & 0xffff;

	
	numOfTotalRetainMum	= *reqDataPtr++;
	numOfTotalInitNum	= *reqDataPtr++;

	numOfRetainNum		= *reqDataPtr++;
	numOfInitNum		= *reqDataPtr++;

	
	saveRetainPtr 	= reqDataPtr;

		
    if( (numOfTotalRetainMum > MAX_ONLINE_DWL_VAR_INFO_CHG_NUM) ||
		(numOfRetainNum > MAX_ONLINE_DWL_VAR_INFO_CHG_NUM) ||
		(numOfRetainNum > numOfTotalRetainMum) )
	{
		status = ONLINE_DWL_VAR_NUM_ERR;
       	setErrorCodeWithVal( 	__FILE__,__LINE__,
               	        		"onlineDwlFbCodeVarInfoChgFuncInComTask",status,
                   	    		"Total Var Retain CHG Number",numOfTotalRetainMum,
                   	    		"Cur Var Retain CHG Number",numOfRetainNum,
                   	    		"MAX Var Retain CHG Number", MAX_ONLINE_DWL_VAR_INFO_CHG_NUM
				  	  		);
		goto MODULE_END;
	}

	if( (numOfTotalInitNum > MAX_ONLINE_DWL_VAR_INFO_CHG_NUM) || 
		(numOfInitNum > MAX_ONLINE_DWL_VAR_INFO_CHG_NUM) || 
		(numOfInitNum > numOfTotalInitNum) )
	{
		status = ONLINE_DWL_VAR_NUM_ERR;
       	setErrorCodeWithVal( 	__FILE__,__LINE__,
               	        		"onlineDwlFbCodeVarInfoChgFuncInComTask",status,
                   	    		"Total Var Init CHG Number",numOfTotalInitNum,
                   	    		"Cur Var Init CHG Number",numOfInitNum,
                   	    		"MAX Var Init CHG Number", MAX_ONLINE_DWL_VAR_INFO_CHG_NUM
				  	  		);
		goto MODULE_END;
	}

	
	
	if(curFrameId == 0x1) {
		onlineVarInfoChgMngrInfoPtr->dwlnumOfTotalRetainNum	= numOfTotalRetainMum;
		onlineVarInfoChgMngrInfoPtr->dwlnumOfTotalInitNum   = numOfTotalInitNum;

		onlineVarInfoChgMngrInfoPtr->dwlTotalFrameNum = totalFrameNum;
	}
	
	onlineVarInfoChgMngrInfoPtr->dwlCurFrameNum 	= curFrameId;
	
	numOfRetainErr = respDataPtr++;
	*numOfRetainErr = 0;
	*respRetLenPtr += DOUBLE_SIZE;

	numOfInitErr = respDataPtr++;
	*numOfInitErr = 0;
	*respRetLenPtr += DOUBLE_SIZE;
		
	for(i=0; i<numOfRetainNum; i++)
	{
		varRetainIndex = *reqDataPtr++;
		varRetainKind  = (*reqDataPtr & 0xffff0000) >> 16;
		retainData	= (*reqDataPtr++ & 0xffff);

		
		if( (varRetainKind != AUTO_VAR)&&(varRetainKind != MMEM_VAR) ) {
			retVal = VARIABLE_KIND_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        		"onlineDwlFbCodeVarInfoChgFuncInComTask",retVal,
									"Variable Kind",varRetainKind,
									"Not Usting", 0, "Not Usting", 0
					  	  		);
		}

		
		if ( (varRetainKind == AUTO_VAR)&&(varRetainIndex > MAX_MPU_AUTOVAR_NUM) ){
			 retVal = VARIABLE_INDEX_ERR;
		}
		
		else if( (varRetainKind == MMEM_VAR) &&(varRetainIndex > MAX_MPU_MMEM_NUM) )  {
			retVal = VARIABLE_INDEX_ERR;
		}

		if(retVal != NO_ERROR) 	{
			setErrorCodeWithVal( 	__FILE__,__LINE__,
          	    "onlineDwlFbCodeVarInfoChgFuncInComTask",retVal,
				"Variable Index",varRetainIndex,
				"Not Usting", 0, "Not Usting", 0
			);
		}

		
		if( (retainData != TRUE) && (retainData != FALSE) ) {
			 retVal = VARIABLE_RETAIN_ERR;
			 setErrorCodeWithVal( 	__FILE__,__LINE__,
          	    "onlineDwlFbCodeVarInfoChgFuncInComTask",retVal,
				"Recv Retain Data",retainData,
				"Not Usting", 0, "Not Usting", 0
			);
		}

		if(retVal != NO_ERROR) 	{
			*numOfRetainErr += 1;
			*respDataPtr++ 	= varRetainIndex;
			*respDataPtr	= (varRetainKind & 0xffff)<<16;
			*respDataPtr++	|= ( retVal & 0xffff);
			*respRetLenPtr += DOUBLE_SIZE;
		}
	}
	
	prevNumOfRetain	= onlineVarInfoChgMngrInfoPtr->numOfTotalVarRetainNum;
	
	onlineVarInfoChgMngrInfoPtr->numOfTotalVarRetainNum += numOfRetainNum;

	
	if(totalFrameNum == curFrameId) {
		 if((onlineVarInfoChgMngrInfoPtr->numOfTotalVarRetainNum > MAX_ONLINE_DWL_VAR_INFO_CHG_NUM)||
			(onlineVarInfoChgMngrInfoPtr->dwlnumOfTotalRetainNum != onlineVarInfoChgMngrInfoPtr->numOfTotalVarRetainNum)
		 ){
			status = VAR_NUM_EXCEED_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,
									"onlineDwlFbCodeVarInfoChgFuncInComTask", status,
                   	    			"Total Var Number", onlineVarInfoChgMngrInfoPtr->numOfTotalVarRetainNum,
                   	    			"Total Var Set  Number", onlineVarInfoChgMngrInfoPtr->dwlnumOfTotalRetainNum,
									"NOT Usting",0
				  	  			);
			goto MODULE_END;
		}		
	}		

		
 	reqDataPtr = saveRetainPtr;

	
	
	onlineVarRetainChgInfoPtr = onlineVarInfoChgMngrInfoPtr->chgVarRetainInfo + prevNumOfRetain;

	for(i=0; i<numOfRetainNum; i++,onlineVarRetainChgInfoPtr++)
	{
		varRetainIndex = *reqDataPtr++;
		varRetainKind  = (*reqDataPtr & 0xffff0000) >> 16;
		retainData	= (*reqDataPtr++ & 0x1);

		onlineVarRetainChgInfoPtr->varRetainIndex = varRetainIndex;
		onlineVarRetainChgInfoPtr->memKind =  varRetainKind;
		onlineVarRetainChgInfoPtr->retainData = retainData;
	}


		
	
	saveInitDataPtr = reqDataPtr;

	for(i=0; i<numOfInitNum; i++)
	{
		varInitType	 = *reqDataPtr;
		varActionData  = (strFbSpecActionInfo*) reqDataPtr++;
		if((varActionData->dataSize == SPEC_SIZE_LWORD)&&(varActionData->bitposFlag != 1 ))
			intlVarSize = SIZE64_TYPE;
		else intlVarSize = SIZE32_TYPE;
		varInitIndex = *reqDataPtr++;

		
		if(intlVarSize == SIZE64_TYPE) {
			eightByteConversion((uint32*)&varInitData[0],(uint32*)reqDataPtr++,0);
			reqDataPtr++;
		}
		else {
			varInitData[0] = *reqDataPtr++;
			varInitData[1] = 0;
		}	

		retVal = specActionDataCheck(&varInitType, varInitIndex ) ; 
		if(retVal != NO_ERROR) 	{
			setErrorCodeWithVal( 	__FILE__,__LINE__,
          	    "onlineDwlFbCodeVarInfoChgFuncInComTask",retVal,
				"Variable Index",varRetainIndex,
				"Not Usting", 0, "Not Usting", 0
			);
		}

		if(retVal != NO_ERROR) 	{
			*numOfInitErr += 1;
			*respDataPtr++ 	= varInitType;
			*respDataPtr++	= varInitIndex;
			*respDataPtr++	= retVal;
			*respRetLenPtr += DOUBLE_SIZE;
		}
	}

	prevNumOfInit	= onlineVarInfoChgMngrInfoPtr->numOfTotalVarInitNum;
	
	onlineVarInfoChgMngrInfoPtr->numOfTotalVarInitNum += numOfInitNum;
   
	
	if(totalFrameNum == curFrameId) {
		 if((onlineVarInfoChgMngrInfoPtr->numOfTotalVarInitNum > MAX_ONLINE_DWL_VAR_INFO_CHG_NUM)||
			(onlineVarInfoChgMngrInfoPtr->dwlnumOfTotalInitNum != onlineVarInfoChgMngrInfoPtr->numOfTotalVarInitNum)
		 ){
			status = VAR_NUM_EXCEED_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,
									"onlineDwlFbCodeVarInfoChgFuncInComTask", status,
                   	    			"Total Var Number", onlineVarInfoChgMngrInfoPtr->numOfTotalVarInitNum,
                   	    			"Total Var Set  Number", onlineVarInfoChgMngrInfoPtr->dwlnumOfTotalInitNum,
									"NOT Using",0
				  	  			);
			goto MODULE_END;
		}		
	}		
	
		
	reqDataPtr = saveInitDataPtr;

	
	
	onlineVarInitChgInfoPtr = onlineVarInfoChgMngrInfoPtr->chgVarInitInfo + prevNumOfInit;

	for(i=0; i<numOfInitNum; i++, onlineVarInitChgInfoPtr++)
	{
		varInitType	 = *reqDataPtr;
		varActionData  = (strFbSpecActionInfo*) reqDataPtr++;
		if((varActionData->dataSize == SPEC_SIZE_LWORD)&&(varActionData->bitposFlag != 1 ))
			intlVarSize = SIZE64_TYPE;
		else intlVarSize = SIZE32_TYPE;

		varInitIndex = *reqDataPtr++;

		
		if(intlVarSize == SIZE64_TYPE) {
			eightByteConversion((uint32*)&varInitData[0],(uint32*)reqDataPtr++,0);
			reqDataPtr++;
		}
		else {
			varInitData[0] = *reqDataPtr++;
			varInitData[1] = 0;
		}	
		
		memoryCopy((uint8*)&onlineVarInitChgInfoPtr->varInitType,(uint8*)&varInitType,DOUBLE_SIZE );
		onlineVarInitChgInfoPtr->varInitIndex =  varInitIndex;
		onlineVarInitChgInfoPtr->varInitData[0] = varInitData[0];
		onlineVarInitChgInfoPtr->varInitData[1] = varInitData[1];
		onlineVarInitChgInfoPtr->intlVarSize = intlVarSize;
	}

	
	retVal = onlineDwlReqDataSendToFbTask(ONLINE_DWL_VAR_INFO_CHANGE,NULL, NULL);	

	if(retVal != NO_ERROR){
		setErrorCode (__FILE__, __LINE__, "onlineDwlFbCodeVarInfoChgFuncInComTask", retVal);
		status = FB_ONLINE_DOWNLOAD_ERR;
	}
MODULE_END:
	
	return(status);
}


uint32 onlineDwlFbCodeTrsDataChgFuncInComTask(	uint32 	*reqDataPtr, 
											uint32	*respDataPtr,
											uint32	*respRetLenPtr
											)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32  totalFrameNum, curFrameId,numOfTotalSegment,numOfSegment;
	strOnlineDwlChgTrsDataMngrInfo  *onlineTrsChgMngrInfoPtr = &sOnlineDwlTrsChgInfo;
	strOnlineDwlChgTrsSegInfo		*onlineSegTrsChgInfoPtr = onlineTrsChgMngrInfoPtr->chgTrsInfoPtr;
	strOnlineDwlChgTrsInfo		    *onlineTrsChgInfoPtr;

	uint32 *saveReqDataPtr;
	uint32 numOfTotalChgTrs, segChgLoop,numOfSegTotalTrs, numOfCutTrs, sumOfTrsNum,prevNumOfTrs;
	uint32 trsChgLoop, trsId, setActiveHeader, setActiveData;
	uint32 taskLogicType,maxSegId,segmentId, loop,checkLoop;

	strSfcMemPointerInfo sfcMemPtr;
	strFbTaskTblMemInfo  segTblInfo;
	strTransitionInfo *trsInfoPtr;

	*respRetLenPtr	= 0;

	
	totalFrameNum	= (*reqDataPtr & 0xffff0000) >> 16;
	curFrameId 	= (*reqDataPtr++ & 0xffff);
	
	
	saveReqDataPtr 	= reqDataPtr;

	
	
	numOfTotalSegment	= (*reqDataPtr & 0xffff0000) >> 16;
	numOfSegment 		= (*reqDataPtr++ & 0xffff);

	
	*respDataPtr++ 	= numOfSegment;
	*respRetLenPtr 	+= 4;

		
    if( (numOfTotalSegment == 0) || (numOfTotalSegment > MAX_ONLINE_DWL_SEG_ADD_NUM)){
		status = ONLINE_DWL_SEG_NUM_ERR;
       	setErrorCodeWithVal( 	__FILE__,__LINE__,
               	        		"onlineDwlFbCodeTrsDataChgFuncInComTask", status,
                   	    		"Total Seg TRS Chg Number",numOfTotalSegment,
                   	    		"MAX Seg TRS Chg Number", MAX_ONLINE_DWL_SEG_ADD_NUM,
                   	    		"NOT Using", 0 
				  	  		);
		goto MODULE_END;
	}
	
    else if( (numOfSegment == 0) || (numOfSegment > MAX_ONLINE_DWL_SEG_ADD_NUM)){
		status = ONLINE_DWL_SEG_NUM_ERR;
       	setErrorCodeWithVal( 	__FILE__,__LINE__,
               	        		"onlineDwlFbCodeTrsDataChgFuncInComTask",status,
                   	    		"Total Seg TRS Chg Number",numOfSegment,
                   	    		"Prev Seg TRS Chg Number", onlineTrsChgMngrInfoPtr->numOfTotalSegment ,
                   	    		"MAX Seg TRS Chg Number", MAX_ONLINE_DWL_SEG_ADD_NUM
				  	  		);
		goto MODULE_END;
	}

	
	numOfTotalChgTrs = *reqDataPtr++;
	
	if(	(numOfTotalChgTrs > MAX_ONLINE_DWL_TRS_CHG_NUM)||
		(numOfTotalChgTrs == 0x0)
	){
		status = TRS_NUM_EXCEED_ERR;
    	setErrorCodeWithVal( 	__FILE__,
    							__LINE__,
           		        		"onlineDwlFbCodeTrsDataChgFuncInComTask",status,
               	    			"Total TRS Chg Number", numOfTotalChgTrs,
               	    			"MAX TRS Chg Number", MAX_ONLINE_DWL_TRS_CHG_NUM,
               	    			"NOT Using",0
			  	  			);
		goto MODULE_END;
	}		
	
	
	if(curFrameId == 0x1) {
		onlineTrsChgMngrInfoPtr->numOfTotalSegment 	= numOfTotalSegment;
		
		
		onlineTrsChgMngrInfoPtr->dwlTotalTrsNum 		= numOfTotalChgTrs;	
		onlineTrsChgMngrInfoPtr->dwlTotalFrameNum 	= totalFrameNum;	
	}
	
	onlineTrsChgMngrInfoPtr->dwlCurFrameNum 	= curFrameId;	
	
	sumOfTrsNum = 0;

	for(segChgLoop = 0; segChgLoop < numOfSegment; segChgLoop++) {
		
		taskLogicType   = (*reqDataPtr>> 31) & 0x1;
		segmentId 		= *reqDataPtr++ & 0xffff;

		if((segmentId == 0) || (segmentId > MAX_LOGIC_TASK_NUM)) {
			status = L_SEG_ID_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        		"onlineDwlFbCodeTrsDataChgFuncInComTask",status,
           	        	    		"Segment ID",segmentId,
           	        	    		"MAX Segment ID", MAX_LOGIC_TASK_NUM,
           	        	    		"NOT Using", 0 
					  	  		);
			goto MODULE_END;
		}

		
		maxSegId = 0;
		for(loop = 1; loop <= MAX_LOGIC_TASK_NUM; loop++) {
			memoryClear( (uint8 *)&segTblInfo, sizeof(strFbTaskTblMemInfo));
		
   			(void)getFbTaskTblInfo( 
								LOGIC_ID,
           						loop,
								(strFbTaskTblMemInfo	*) &segTblInfo
								);
			if(segTblInfo.taskId > maxSegId)
				maxSegId = segTblInfo.taskId;
			
			if(segTblInfo.taskId == segmentId) break;
		}

		if((segTblInfo.taskId == 0) || (loop > MAX_LOGIC_TASK_NUM)){
			status = ONLINE_DWL_SEG_EXIST_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,
           	    	        		"onlineDwlFbCodeTrsDataChgFuncInComTask",status,
                	   	    		"Segment ID",segmentId,
                 	  	    		"Current Max Seg Id", maxSegId,
                  	 	    		"NOT Using", 0 
				  		  		);
			goto MODULE_END;		
		}

		if(segTblInfo.logicType != taskLogicType)  {
			status = L_LOGIC_ID_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        		"onlineDwlFbCodeTrsDataChgFuncInComTask",status,
									"Segment ID",segmentId,
           	        	    		"Curr Logic Type",segTblInfo.logicType,
           	        	    		"Recv Logic Type",taskLogicType
					  	  		);
			goto MODULE_END;
		}
		
		if(taskLogicType != SFC_LOGIC_TYPE) {
			status = L_LOGIC_ID_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        		"onlineDwlFbCodeTrsDataChgFuncInComTask",status,
									"Segment ID",segmentId,
           	        	    		"Recv Logic Typ",taskLogicType,
           	        	    		"Set Logic Type",SFC_LOGIC_TYPE
					  	  		);
			goto MODULE_END;
		}


		
		numOfSegTotalTrs = (*reqDataPtr & 0xffff0000) >> 16;	 
		numOfCutTrs		 = *reqDataPtr++ & 0xffff;
		sumOfTrsNum		 += 	numOfCutTrs;
		
		
		if( (numOfSegTotalTrs > MAX_ONLINE_DWL_TRS_CHG_NUM) ||
			(numOfCutTrs > MAX_ONLINE_DWL_TRS_CHG_NUM) ||
			(numOfSegTotalTrs < numOfCutTrs) ||
			(numOfSegTotalTrs == 0x0) ||
			(numOfCutTrs == 0x0)
		) {
			status = TRS_NUM_EXCEED_ERR;
    		setErrorCodeWithVal( 	__FILE__,__LINE__,
           			        		"onlineDwlFbCodeTrsDataChgFuncInComTask",status,
               	    				"Task TRS Chg  Number", numOfSegTotalTrs,
               	    				"Cur TRS Chg Number",numOfCutTrs,
               	    				"MAX TRS Chg Number", MAX_ONLINE_DWL_TRS_CHG_NUM
			  	  				);
			goto MODULE_END;
		}

		for(trsChgLoop = 0; trsChgLoop < numOfCutTrs; trsChgLoop++) {
			
			trsId = *reqDataPtr++;	 
			if(	(trsId < 1) || (trsId > MAX_TRANSITOIN_NUM) )
			{
    			setErrorCodeWithVal( 	__FILE__,__LINE__,
           			        			"onlineDwlFbCodeTrsDataChgFuncInComTask",status,
               	    					"Recv TRS ID", trsId,
               	    					"MAX TRS ID", MAX_TRANSITOIN_NUM,
										"Not Using", 0
			  	  					);
				goto MAKE_RESP;
			}

			
			status = sfcMemPointerGet(LOCAL_MEM_ACC, &sfcMemPtr);
			if(status != NO_ERROR){
				setErrorCode( (uint8 *)__FILE__,(uint32)__LINE__,
						(uint8 *)"onlineDwlFbCodeTrsDataChgFuncInComTask",status
						);
				goto MAKE_RESP;
			}
			trsInfoPtr = (strTransitionInfo*)sfcMemPtr.trsMemPtr;
			trsInfoPtr += (trsId-1);

			if(trsInfoPtr->transitionID !=trsId) 
			{
				status = L_TRANSITION_ID_ERR;
    			setErrorCodeWithVal( 	__FILE__,__LINE__,
           			        			"onlineDwlFbCodeTrsDataChgFuncInComTask",status,
										"Segment ID",segmentId,
               	    					"Recv TRS ID", trsId,
               	    					"SET TRS ID",trsInfoPtr->transitionID
			  	  					);
				goto MAKE_RESP;
			}
			if(trsInfoPtr->taskID !=segmentId) 
			{
				status = L_SEG_ID_ERR;
    			setErrorCodeWithVal( 	__FILE__,__LINE__,
           			        			"onlineDwlFbCodeTrsDataChgFuncInComTask",status,
               	    					"Recv TASK ID", segmentId,
               	    					"SET TASK ID",trsInfoPtr->taskID,
               	    					"Not Using", 0
			  	  					);
				goto MAKE_RESP;
			}
			
			setActiveHeader = *reqDataPtr++;	 
			setActiveData = *reqDataPtr++;	 
			
			status = specActionDataCheck(&setActiveHeader, setActiveData);
			if(status != NO_ERROR) {
				    setErrorCodeWithVal( __FILE__,__LINE__,
           			        "onlineDwlFbCodeTrsDataChgFuncInComTask",status,
							"TRS ID", trsId,
							"Task Id",segmentId, 
							"Not Using", 0
               	    );
			}

		}
		
		
			
MAKE_RESP:
		*respDataPtr 	= (status & 0xffff) << 16;
		*respDataPtr++	|= segmentId;
		*respRetLenPtr 	+= 4;	
		
		if(status != NO_ERROR) goto MODULE_END;
		
	} 

	
	if(status != NO_ERROR)
		goto MODULE_END;


	
	onlineTrsChgMngrInfoPtr->numOfTotalTrs 	+= sumOfTrsNum;

	
	if(totalFrameNum == curFrameId) {
		 if((onlineTrsChgMngrInfoPtr->numOfTotalTrs > MAX_ONLINE_DWL_TRS_CHG_NUM)||
		 	(onlineTrsChgMngrInfoPtr->numOfTotalTrs == 0x0)||
			(onlineTrsChgMngrInfoPtr->dwlTotalTrsNum != onlineTrsChgMngrInfoPtr->numOfTotalTrs)
		 ){
			status = TRS_NUM_EXCEED_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,
               		        		"onlineDwlFbCodeTrsDataChgFuncInComTask",status,
                   	    			"SUM Fb Code ADD Number",sumOfTrsNum,
                   	    			"Total FB Add  Number", onlineTrsChgMngrInfoPtr->numOfTotalTrs,
                   	    			"Total FB Set  Number", onlineTrsChgMngrInfoPtr->dwlTotalTrsNum
				  	  			);
			goto MODULE_END;
		}		
	}

		
	
	reqDataPtr = saveReqDataPtr;

	
	numOfSegment = *reqDataPtr++&0xffff;

	
	numOfTotalChgTrs = *reqDataPtr++;
	
	for(segChgLoop = 0; segChgLoop < numOfSegment; segChgLoop++){
		
		taskLogicType   = (*reqDataPtr>> 31) & 0x1;	 
		segmentId 		= *reqDataPtr++& 0xffff;

		
		for(checkLoop = 0; checkLoop < MAX_ONLINE_DWL_SEG_ADD_NUM; checkLoop++) {
			if((onlineSegTrsChgInfoPtr->taskId == 0) || (onlineSegTrsChgInfoPtr->taskId == segmentId))
				break;
			onlineSegTrsChgInfoPtr++;	
		}
		if(checkLoop == MAX_ONLINE_DWL_SEG_ADD_NUM) {
			status = FB_ONLINE_DOWNLOAD_ERR;
			setErrorCode (__FILE__,__LINE__,"onlineDwlFbCodeTrsDataChgFuncInComTask", status);
			goto MODULE_END;	
		}

		
		numOfSegTotalTrs = (*reqDataPtr & 0xffff0000) >> 16;	 
		numOfCutTrs		 = *reqDataPtr++ & 0xffff;

		prevNumOfTrs = onlineSegTrsChgInfoPtr->numOfTransition;

		onlineSegTrsChgInfoPtr->taskId = segmentId;
		onlineSegTrsChgInfoPtr->numOfTransition += numOfCutTrs;
		onlineSegTrsChgInfoPtr->logicType		= taskLogicType;


		if(onlineSegTrsChgInfoPtr->numOfTransition  > MAX_ONLINE_DWL_TRS_CHG_NUM) {
			status = FB_NUM_EXCEED_ERR;
			setErrorCode (__FILE__,__LINE__,"onlineDwlFbCodeTrsDataChgFuncInComTask", status);
			goto MODULE_END;	
		}
		
		
		onlineTrsChgInfoPtr = onlineSegTrsChgInfoPtr->chgTrsDataInfo + prevNumOfTrs;

		for(trsChgLoop = 0; trsChgLoop < numOfCutTrs; trsChgLoop++,onlineTrsChgInfoPtr++) {

			trsId = *reqDataPtr++;	 
			setActiveHeader = *reqDataPtr++;	 
			setActiveData = *reqDataPtr++;	 

			onlineTrsChgInfoPtr->trsID = trsId;
			onlineTrsChgInfoPtr->setActiveHeader = setActiveHeader;
			onlineTrsChgInfoPtr->setActiveData = setActiveData;
			onlineTrsChgInfoPtr->taskId = segmentId;
		}
	}
		
	
	retVal = onlineDwlReqDataSendToFbTask(ONLINE_DWL_TRS_DATA_CHANGE,NULL, NULL);	
	if(retVal != NO_ERROR){
		setErrorCode (__FILE__, __LINE__, "onlineDwlFbCodeTrsDataChgFuncInComTask", retVal);
		status = FB_ONLINE_DOWNLOAD_ERR;
	}


MODULE_END:
	return(status);
}



static uint32 onlineDwlFbCodeVarInfoChgFuncInFbTask(strOnlineDwlTaskComReqInfo *argPtr)
{
	uint32	status = NO_ERROR;

	strOnlineDwlVarInfoChgMngrInfo	*onlineVarInfoChgMngrInfoPtr = &sOnlineDwlVarInfoChgInfo;
	strOnlineDwlVarRetainInfo *onlineVarRetainChgInfoPtr;
	strOnlineDwlVarInitInfo   *onlineVarInitChgInfoPtr;

	uint32 totalRetainNum, totalVarInitNum;
	uint32 segVarChgLoop,varRetainKind, varRetainIndex,retainData;
	uint32 varInitType, varInitIndex,intlVarSize, varInitData[2],varInitAddr =0;
	strFbSpecActionInfo *varActionData;
	strVariableExecInfo sysVarExeInfo;


	
	if(onlineVarInfoChgMngrInfoPtr->dwlTotalFrameNum != onlineVarInfoChgMngrInfoPtr->dwlCurFrameNum) {
		status = ONLINE_DWL_FRAME_NUM_ERR; 
		setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeVarInfoChgFuncInFbTask",status,
								"Total Frame Number", onlineVarInfoChgMngrInfoPtr->dwlTotalFrameNum,
								"Current Frame Number",onlineVarInfoChgMngrInfoPtr->dwlCurFrameNum,
								"Not Using", 0
							 );
		return(status);							 							
	}

	
	totalRetainNum = onlineVarInfoChgMngrInfoPtr->numOfTotalVarRetainNum;
	totalVarInitNum = onlineVarInfoChgMngrInfoPtr->numOfTotalVarInitNum;

		
    if(totalRetainNum > MAX_ONLINE_DWL_VAR_INFO_CHG_NUM) 
	{
		status = ONLINE_DWL_VAR_NUM_ERR;
       	setErrorCodeWithVal( 	__FILE__,__LINE__,
               	        		"onlineDwlFbCodeVarInfoChgFuncInFbTask",status,
                   	    		"Total Var Retain CHG Number",totalRetainNum,
                	    		"MAX Var Retain CHG Number", MAX_ONLINE_DWL_VAR_INFO_CHG_NUM,
                   	    		"NOT Using", 0
				  	  		);
		goto RETAIN_END;
	}
	else if(totalRetainNum != onlineVarInfoChgMngrInfoPtr->dwlnumOfTotalRetainNum) {
		status = ONLINE_VAR_RETAIN_MISMATCH_ERR; 
		setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeVarInfoChgFuncInFbTask",status,
								"Total Chg VAR Number", onlineVarInfoChgMngrInfoPtr->dwlnumOfTotalRetainNum,
								"Calcu Chg VAR Number",totalRetainNum,
								"Not Using", 0
							 );
		return(status);							 					
	}

		
	else if (totalVarInitNum > MAX_ONLINE_DWL_VAR_INFO_CHG_NUM) 
	{
		status = ONLINE_DWL_VAR_NUM_ERR;
       	setErrorCodeWithVal( 	__FILE__,__LINE__,
               	        		"onlineDwlFbCodeVarInfoChgFuncInFbTask",status,
                   	    		"Total Var Init CHG Number",totalVarInitNum,
                   	    		"MAX Var Init CHG Number", MAX_ONLINE_DWL_VAR_INFO_CHG_NUM,
                   	    		"NOT Using", 0
				  	  		);
		goto RETAIN_END;
	}
	else if(totalVarInitNum != onlineVarInfoChgMngrInfoPtr->dwlnumOfTotalInitNum) {
		status = ONLINE_VAR_RETAIN_MISMATCH_ERR; 
		setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeVarInfoChgFuncInFbTask",status,
								"Total Chg VAR Number", onlineVarInfoChgMngrInfoPtr->dwlnumOfTotalInitNum,
								"Calcu Chg Fb Number",totalVarInitNum,
								"Not Using", 0
							 );
		return(status);							 					
	}

	
	else if ( ( totalRetainNum== 0) && (totalVarInitNum == 0 ) )
	{
		status = ONLINE_DWL_VAR_NUM_ERR;
       	setErrorCodeWithVal( 	__FILE__,__LINE__,
               	        		"onlineDwlFbCodeVarInfoChgFuncInFbTask",status,
 	        	    			"Total Var Retain CHG Number",totalRetainNum,
                   	    		"Total Var Init CHG Number",totalVarInitNum,
                   	    		"NOT Using", 0
				  	  		);
		goto RETAIN_END;
	}

	onlineVarRetainChgInfoPtr = &onlineVarInfoChgMngrInfoPtr->chgVarRetainInfo[0];

	for(segVarChgLoop = 0; segVarChgLoop < totalRetainNum; segVarChgLoop++,onlineVarRetainChgInfoPtr++) {
		varRetainKind = onlineVarRetainChgInfoPtr->memKind;
		varRetainIndex = onlineVarRetainChgInfoPtr->varRetainIndex;
		retainData = onlineVarRetainChgInfoPtr->retainData;

		
		if( (varRetainKind != AUTO_VAR)&&(varRetainKind != MMEM_VAR) ) {
			status = VARIABLE_KIND_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        		"onlineDwlFbCodeVarInfoChgFuncInComTask",status,
									"Variable Kind",varRetainKind,
									"Not Usting", 0, "Not Usting", 0
					  	  		);
			continue;
		}

		
		if ( (varRetainKind == AUTO_VAR)&&(varRetainIndex > MAX_MPU_AUTOVAR_NUM) ){
			 status = VARIABLE_INDEX_ERR;
		}
		
		else if( (varRetainKind == MMEM_VAR) &&(varRetainIndex > MAX_MPU_MMEM_NUM) )  {
			status = VARIABLE_INDEX_ERR;
		}
		if(status != NO_ERROR) 	{
			setErrorCodeWithVal( 	__FILE__,__LINE__,
          	    "onlineDwlFbCodeVarInfoChgFuncInComTask",status,
				"Variable Index",varRetainIndex,
				"Not Usting", 0, "Not Usting", 0
			);
			 continue;
		}

		
		if( (retainData != TRUE) && (retainData != FALSE) ) {
			 status = VARIABLE_RETAIN_ERR;
			 setErrorCodeWithVal( 	__FILE__,__LINE__,
          	    "onlineDwlFbCodeVarInfoChgFuncInComTask",status,
				"Recv Retain Data",retainData,
				"Not Usting", 0, "Not Usting", 0
			);
			continue;
		}
	}

	
	if(status != NO_ERROR)
		goto RETAIN_END;
	
		
	onlineVarRetainChgInfoPtr = &onlineVarInfoChgMngrInfoPtr->chgVarRetainInfo[0];

	for(segVarChgLoop = 0; segVarChgLoop < totalRetainNum; segVarChgLoop++,onlineVarRetainChgInfoPtr++) {
		varRetainKind = onlineVarRetainChgInfoPtr->memKind;
		varRetainIndex = onlineVarRetainChgInfoPtr->varRetainIndex;
		retainData = onlineVarRetainChgInfoPtr->retainData;

		status = localVariableRetainInfoWrite(varRetainKind, varRetainIndex, (uint8)retainData);
		if(status!=NO_ERROR) {
			setErrorCode(	__FILE__,__LINE__,
          		 "onlineDwlFbCodeVarInfoChgFuncInComTask",status);
			continue;
		}
		status = busVariableRetainInfoWrite(varRetainKind, varRetainIndex, (uint8)retainData);
		if(status!=NO_ERROR) {
			setErrorCode(	__FILE__,__LINE__,
          		 "onlineDwlFbCodeVarInfoChgFuncInComTask",status);
			continue;
		}

		
		status = fbSysExeVarInfoRead(varRetainKind,&sysVarExeInfo);
		if((status == NO_ERROR)) {
			if(sysVarExeInfo.curMaxVarRetainInfo < (varRetainIndex/32 +1 ) ) {
				sysVarExeInfo.curMaxVarRetainInfo =  (varRetainIndex/32 +1) ;
				status = fbSysExeVarInfoWrite(varRetainKind,&sysVarExeInfo);
			}
			if(sysVarExeInfo.curMaxVarAddr < varRetainIndex){
				sysVarExeInfo.curMaxVarAddr =  varRetainIndex;
				status = fbSysExeVarInfoWrite(varRetainKind,&sysVarExeInfo);
			}
		}
		
		if(status!=NO_ERROR){
			setErrorCode(	__FILE__,__LINE__,
          		 "onlineDwlFbCodeVarInfoChgFuncInComTask",status);
			continue;
		}
	}

RETAIN_END:

	onlineVarInitChgInfoPtr = &onlineVarInfoChgMngrInfoPtr->chgVarInitInfo[0];

	for(segVarChgLoop = 0; segVarChgLoop < totalVarInitNum; segVarChgLoop++,onlineVarInitChgInfoPtr++) {
		varInitType = onlineVarInitChgInfoPtr->varInitType;
		varInitIndex = onlineVarInitChgInfoPtr->varInitIndex;

		intlVarSize = onlineVarInitChgInfoPtr->intlVarSize;
		varInitData[0] = onlineVarInitChgInfoPtr->varInitData[0];
		varInitData[1] = onlineVarInitChgInfoPtr->varInitData[1];
		
		status = specActionDataCheck(&varInitType, varInitIndex ) ; 
		if(status!= NO_ERROR) {
			setErrorCode(	__FILE__,__LINE__,
          						 "onlineDwlFbCodeVarInfoChgFuncInComTask",status);
			continue;
		}

		varActionData = (strFbSpecActionInfo*)((void*)&varInitType);
		if( (varActionData->memoryType != M_MEMORY)&&(varActionData->memoryType != A_MEMORY)&&
			(varActionData->memoryType !=S_MEMORY) 
			) {
				status = VARIABLE_KIND_ERR; 
				setErrorCode(	__FILE__,__LINE__,
          						 "onlineDwlFbCodeVarInfoChgFuncInComTask",status);

				continue;
		}
	}

	onlineVarInitChgInfoPtr = &onlineVarInfoChgMngrInfoPtr->chgVarInitInfo[0];
	for(segVarChgLoop = 0; segVarChgLoop < totalVarInitNum; segVarChgLoop++,onlineVarInitChgInfoPtr++) {
		varInitType = onlineVarInitChgInfoPtr->varInitType;
		varInitIndex = onlineVarInitChgInfoPtr->varInitIndex;
		intlVarSize = onlineVarInitChgInfoPtr->intlVarSize;
		varInitData[0] = onlineVarInitChgInfoPtr->varInitData[0];
		varInitData[1] = onlineVarInitChgInfoPtr->varInitData[1];
		
		varActionData = (strFbSpecActionInfo*)((void*)&varInitType);

		status = specActionDataCheck(&varInitType, varInitIndex ) ; 
		if(status!=NO_ERROR) {
			setErrorCode(	__FILE__,__LINE__,
          		 "onlineDwlFbCodeVarInfoChgFuncInComTask",status);
			continue;
		}

		
		status = refDataCopyToMem(varInitData,varInitIndex, *varActionData, intlVarSize);
		if(status!=NO_ERROR) {
			setErrorCode(	__FILE__,__LINE__,
          		 "onlineDwlFbCodeVarInfoChgFuncInComTask",status);
			continue;
		}

		
		status = refDataCopyToBusMem(varInitData,varInitIndex,*varActionData, intlVarSize);
		if(status!=NO_ERROR) {
			setErrorCode(	__FILE__,__LINE__,
          		 "onlineDwlFbCodeVarInfoChgFuncInComTask",status);
			continue;
		}
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
		
		status = refDatacopyToInitVarBusMem(varInitData,varInitIndex,*varActionData, intlVarSize);
		if(status!=NO_ERROR) {
			setErrorCode(	__FILE__,__LINE__,
          		 "onlineDwlFbCodeVarInfoChgFuncInComTask",status);
			continue;
		}
#endif 		
		
		status = fbSysExeVarInfoRead(varActionData->memoryType,&sysVarExeInfo);
		if((status == NO_ERROR)) {
			
			if(varActionData->memoryType == A_MEMORY) 	varInitAddr = varInitIndex>>8;
			else if( (varActionData->memoryType == M_MEMORY) ||
					 (varActionData->memoryType == S_MEMORY) )
			{
				switch(varActionData->dataSize){
					case SPEC_SIZE_BIT:
						varInitAddr = (varInitIndex>>8) /32;
					break;
					case SPEC_SIZE_BYTE	:
						varInitAddr = (varInitIndex>>8) /4;
					break;
					case SPEC_SIZE_WORD	:
						varInitAddr = (varInitIndex>>8) /2;
					break;
					case SPEC_SIZE_DWORD:
						varInitAddr = (varInitIndex>>8) /1;
					break;
					case SPEC_SIZE_LWORD:
						varInitAddr = (varInitIndex>>8) *2;
					break;
					default:
						status = FB_SPEC_ACTION_SIZE_ERR;
					break;
				}
			}
			else status = VARIABLE_KIND_ERR;

			if(status != NO_ERROR)
			{
				setErrorCodeWithVal( 	__FILE__,__LINE__,
          							   "onlineDwlFbCodeVarInfoChgFuncInComTask",status,
									   "VAR KIND", varActionData->memoryType,
									   "DATA SIZE", varActionData->dataSize,
									   "Not Using", 0
									   );
			}

			if(sysVarExeInfo.curMaxVarAddr < varInitAddr) {
				sysVarExeInfo.curMaxVarAddr =  varInitAddr;
				status = fbSysExeVarInfoWrite(varActionData->memoryType,&sysVarExeInfo);
			}
		}
		else{
			setErrorCode(	__FILE__,__LINE__,
          		 "onlineDwlFbCodeVarInfoChgFuncInComTask",status);
			continue;
		}
	}

	return(status);
}


static uint32 onlineDwlFbCodeTrsDataChgFuncInFbTask(strOnlineDwlTaskComReqInfo *argPtr)
{
	uint32	status = NO_ERROR;
	strOnlineDwlChgTrsDataMngrInfo	*onlineTrsChgMngrInfoPtr = &sOnlineDwlTrsChgInfo;
	strOnlineDwlChgTrsSegInfo		*onlineSegChgInfoPtr = onlineTrsChgMngrInfoPtr->chgTrsInfoPtr;
	strOnlineDwlChgTrsInfo			*onlineTrsChgInfoPtr;

	strFbTaskTblMemInfo segTblInfo;
	uint32 totalTrsChgNum, segLoop, loop;
	strTransitionInfo *trsInfoPtr,*trsBusInfoPtr;
	strSfcMemPointerInfo sfcMemPtr, sfcMemBusPtr;

	
	if(onlineTrsChgMngrInfoPtr->dwlTotalFrameNum != onlineTrsChgMngrInfoPtr->dwlCurFrameNum) {
		status = ONLINE_DWL_FRAME_NUM_ERR; 
		setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeTrsDataChgFuncInFbTask",status,
								"Total Frame Number", onlineTrsChgMngrInfoPtr->dwlTotalFrameNum,
								"Current Frame Number",onlineTrsChgMngrInfoPtr->dwlCurFrameNum,
								"Not Using", 0
							 );
		return(status);							 							
	}
	
	
	if( (onlineTrsChgMngrInfoPtr->numOfTotalSegment == 0) ||
		(onlineTrsChgMngrInfoPtr->numOfTotalSegment > MAX_ONLINE_DWL_SEG_ADD_NUM)
	){
		status = ONLINE_DWL_SEG_NUM_ERR; 
		setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeTrsDataChgFuncInFbTask",status,
								"Total Add Segment Number", onlineTrsChgMngrInfoPtr->numOfTotalSegment,
								"Max Add Segment Number",MAX_ONLINE_DWL_SEG_ADD_NUM,
								"Not Using", 0
							 );
		return(status);							 					
	}
	
	
	if( (onlineTrsChgMngrInfoPtr->numOfTotalTrs == 0) ||
		(onlineTrsChgMngrInfoPtr->numOfTotalTrs > MAX_ONLINE_DWL_TRS_CHG_NUM)
	){
		status = FB_NUM_EXCEED_ERR; 
		setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeTrsDataChgFuncInFbTask",status,
								"Total Add FB Number", onlineTrsChgMngrInfoPtr->numOfTotalTrs,
								"Max Add FB Number",MAX_ONLINE_DWL_TRS_CHG_NUM,
								"Not Using", 0
							 );
		return(status);							 					
	}

	
	
	
	totalTrsChgNum = 0;

	for(segLoop = 0; segLoop < onlineTrsChgMngrInfoPtr->numOfTotalSegment; 
		segLoop++,onlineSegChgInfoPtr++){

		
		for(loop = 1; loop <= MAX_LOGIC_TASK_NUM; loop++) {
			memoryClear( (uint8 *)&segTblInfo, sizeof(strFbTaskTblMemInfo));
		
   			(void)getFbTaskTblInfo( 
								LOGIC_ID,loop,
								(strFbTaskTblMemInfo	*) &segTblInfo
								);
			if(segTblInfo.taskId == onlineSegChgInfoPtr->taskId) break;
		}
			
		
		if(	(onlineSegChgInfoPtr->taskId == 0x0) || 
			(onlineSegChgInfoPtr->taskId > MAX_LOGIC_TASK_NUM) ||
			(segTblInfo.taskId != onlineSegChgInfoPtr->taskId)
		){
			status = L_SEG_ID_ERR;
			setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeTrsDataChgFuncInFbTask",status,
									"Segmnet ID", onlineSegChgInfoPtr->taskId,
									"Not Using",0,
									"Not Using", 0
							 	);
			return(status);							 					
		}
		
		
		totalTrsChgNum += onlineSegChgInfoPtr->numOfTransition;	
		
	} 
	
	
	if(totalTrsChgNum != onlineTrsChgMngrInfoPtr->numOfTotalTrs) {
		status = ONLINE_FB_CODE_NUM_MISMATCH_ERR; 
		setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeTrsDataChgFuncInFbTask",status,
								"Total Chg TRS", onlineTrsChgMngrInfoPtr->numOfTotalTrs,
								"Calcu Chg TRS",totalTrsChgNum,
								"Not Using", 0
							 );
		return(status);							 					
	}


	
	onlineSegChgInfoPtr = onlineTrsChgMngrInfoPtr->chgTrsInfoPtr;

	status = sfcMemPointerGet(LOCAL_MEM_ACC, &sfcMemPtr);
	if(status != NO_ERROR){
		setErrorCode( (uint8 *)__FILE__,(uint32)__LINE__,
				(uint8 *)"onlineDwlFbCodeTrsDataChgFuncInComTask",status
				);
		return(status);
	}

	status = sfcMemPointerGet(BUS_MEM_ACC, &sfcMemBusPtr);
	if(status != NO_ERROR){
		setErrorCode( (uint8 *)__FILE__,(uint32)__LINE__,
				(uint8 *)"onlineDwlFbCodeTrsDataChgFuncInComTask",status
				);
		return(status);
	}

	for(segLoop = 0; segLoop < onlineTrsChgMngrInfoPtr->numOfTotalSegment; 
		segLoop++,onlineSegChgInfoPtr++)
	{

		onlineTrsChgInfoPtr  = &onlineSegChgInfoPtr->chgTrsDataInfo[0];

		for(loop = 0; loop <onlineSegChgInfoPtr->numOfTransition; loop++, onlineTrsChgInfoPtr++){
			
			if(	(onlineTrsChgInfoPtr->trsID < 1) ||
				(onlineTrsChgInfoPtr->trsID > MAX_TRANSITOIN_NUM)
			){
				status = SEG_BLK_ADDR_EXCEED_ERR;
				setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeTrsDataChgFuncInFbTask",status,
								"Task ID",onlineSegChgInfoPtr->taskId,
								"TRS ID", onlineTrsChgInfoPtr->trsID,
								"MAX TRS ID", MAX_TRANSITOIN_NUM
							 );
				return(status);							 					
			}

			trsInfoPtr = (strTransitionInfo*)sfcMemPtr.trsMemPtr;
			trsInfoPtr += (onlineTrsChgInfoPtr->trsID-1);

			if(trsInfoPtr->transitionID != onlineTrsChgInfoPtr->trsID )
			{
				status = SEG_BLK_ADDR_EXCEED_ERR;
				setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeTrsDataChgFuncInFbTask",status,
								"Recv TRS ID",onlineTrsChgInfoPtr->trsID,
								"Set TRS ID", trsInfoPtr->transitionID,
								"Not Using", 0
							 );
				return(status);							 					
			}

			if(trsInfoPtr->taskID != onlineSegChgInfoPtr->taskId)
			{
				status = SEG_BLK_ADDR_EXCEED_ERR;
				setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeTrsDataChgFuncInFbTask",status,
								"Trs Task ID",onlineSegChgInfoPtr->taskId,
								"Set Task ID", trsInfoPtr->taskID,
								"Not Using", 0
							 );
				return(status);							 					
			}

			
			trsInfoPtr->setActiveHeader = onlineTrsChgInfoPtr->setActiveHeader;
			trsInfoPtr->setActiveData   = onlineTrsChgInfoPtr->setActiveData;

			
			trsBusInfoPtr = (strTransitionInfo*)sfcMemBusPtr.trsMemPtr;
			trsBusInfoPtr += (onlineTrsChgInfoPtr->trsID-1);

			trsBusInfoPtr->setActiveHeader = onlineTrsChgInfoPtr->setActiveHeader;
			trsBusInfoPtr->setActiveData   = onlineTrsChgInfoPtr->setActiveData;

		}
	}

	return(status);
}




static uint32	fbOutputUnusedMemoryQueInit(void)
{
	uint32	status = NO_ERROR;
	uint32	loop;

    
    sFbOutputUnusedMemQMngrInfo.freeList = NULL;
    sFbOutputUnusedMemQMngrInfo.usedList = NULL;

    for( loop = 0; loop < MAX_FB_SPEC_FREE_MEM_MNGR_NUM; loop++) {
		memoryClear (   (char *) &sFbOutputUnusedMemDataInfo[loop],
                         sizeof(strFbSpecUnusedMemInfo)
                    );
        status = nodePutOnLast	(	(strDoubleLink **) &sFbOutputUnusedMemQMngrInfo.freeList,
                                	(strDoubleLink *)  &sFbOutputUnusedMemDataInfo[loop]
                            	);
		if(status != NO_ERROR)
			break;                            
    }
	
	return(status);
}


uint32 fbOutputUnusedMemoryInfoClear( uint32 moduleId)
{
	uint32	status = NO_ERROR;
	strFbSpecUnusedMemInfo	*curDataPtr;
		
	
	if( moduleId != LOGIC_ID){
		status = L_MODULE_ID_ERR;
        setErrorCode( __FILE__, __LINE__, "fbOutputUnusedMemoryInfoClear",status);
		return(status);
	}	

	
	if(sFbOutputUnusedMemQMngrInfo.usedList == NULL) return(status);

	
    status = semaphoreGet(sFbOutputMemUnusedQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,
						__LINE__,
						"fbOutputUnusedMemoryInfoClear",
						status
					  );
		return(status);
	}

    while( sFbOutputUnusedMemQMngrInfo.usedList!= NULL) {
        
   		curDataPtr = (strFbSpecUnusedMemInfo *)nodeGetAndUnlink(
                    		(strDoubleLink **)&sFbOutputUnusedMemQMngrInfo.usedList
																);
        if(curDataPtr != NULL){
           	
           	memoryClear ( (uint8 *) curDataPtr, sizeof(strFbSpecUnusedMemInfo));
           	(void)nodePutOnLast( (strDoubleLink **) &sFbOutputUnusedMemQMngrInfo.freeList,
               	                 (strDoubleLink *)curDataPtr
                   	           );
        }
    }

	
#ifdef VXWORKS
	semaphoreRelease (sFbOutputMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbOutputMemUnusedQSemaId);	
#endif
	return(status);
}


uint32 fbOutputUnusedMemoryInfoSet( uint32 moduleId)
{
	uint32	status = NO_ERROR, blockAddr = 0, usedMemSize;
	strFbMemInfo 		fbMemInfo;
	strFbSpecUnusedMemInfo	*curDataPtr,*tmpDataPtr;
	uint32	prevStartUnusedOutputMemIndex,prevEndUnusedOutputMemIndex;
	uint32 saveEndIndex, curEndOutputMemIndex; 

	strModuleExecInfo sysExeInfo;
	strFbTaskTblMemInfo  segTblInfo;
	
	
	if( moduleId != LOGIC_ID){
		status = L_MODULE_ID_ERR;
        setErrorCode( __FILE__, __LINE__, "fbSpecUnusedMemoryInfoSet",status);
		return(status);
	}	

	memoryClear( (uint8 *)&sysExeInfo, sizeof(strModuleExecInfo));
	status = fbSysExeModuleInfoRead(moduleId, &sysExeInfo);
	if(status!=NO_ERROR){
		setErrorCode((uint8 *)__FILE__, __LINE__, (uint8 *)"fbOutputUnusedMemoryInfoSet", status);
		return(status);
	}
	
	
    status = semaphoreGet(sFbOutputMemUnusedQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,__LINE__,
						"fbOutputUnusedMemoryInfoSet",status
					  );
		return(status);
	}

	
    if(	(MAX_MPU_BLOCK_ADDR - sysExeInfo.curfbMaxBlockAddr) > MAX_MPU_BLOCK_ADDR){
		status = L_MEM_SIZE_ERR;
        setErrorCodeWithVal( 	__FILE__, __LINE__, 
        						"fbOutputUnusedMemoryInfoSet",	status,
        						"Calcu FB PARA SIZE(BYTE)",sysExeInfo.curfbMaxBlockAddr,
        						"Recv FB PARA SIZE(BYTE)",MAX_MPU_BLOCK_ADDR ,
								"NOT Using", 0
        						);
		goto MODULE_END;    	
    }

	
   	curDataPtr = (strFbSpecUnusedMemInfo *)nodeGetAndUnlink (
                           		(strDoubleLink **)&sFbOutputUnusedMemQMngrInfo.freeList
            	                                    		);
	if(curDataPtr == NULL) {
		status = NO_EXIST_NODE_ERR;
   		setErrorCode( __FILE__, __LINE__, "fbOutputUnusedMemoryInfoSet",status);
		goto MODULE_END;	
	}
	else {
		
		curDataPtr->unUsedMemStartIndex	= 0; 

		
		curDataPtr->unUsedMemEndIndex	= MAX_MPU_OUTPUT_NUM-1;
		 
		curDataPtr->freeMemSize	= curDataPtr->unUsedMemEndIndex - curDataPtr->unUsedMemStartIndex + 1;
		curDataPtr->freeMemSize	*= 4; 

		curDataPtr->usableMemSize = curDataPtr->freeMemSize;
		
		
		status = nodePutOnLast( (strDoubleLink **) &sFbOutputUnusedMemQMngrInfo.usedList,
       		            		(strDoubleLink *)curDataPtr
           		       		  );
		if(status) {
        	setErrorCode( __FILE__, __LINE__, "fbOutputUnusedMemoryInfoSet",status);
			goto MODULE_END;	
		}
	}
	
	
	for(	blockAddr = 1; 
				blockAddr <= sysExeInfo.curfbMaxBlockAddr; blockAddr++){  

		
    	if( (status = fbCodeMemInfoRead	(	LOGIC_ID,	blockAddr, (strFbMemInfo *) &fbMemInfo )) != NO_ERROR)
		{
       		setErrorCode( __FILE__, __LINE__, "fbOutputUnusedMemoryInfoSet",status);
			goto MODULE_END;
		}

		if( fbMemInfo.fbId == 0x0) continue;
			
		
		status = getFbTaskTblInfo(LOGIC_ID,
           						fbMemInfo.fbTaskId,
								(strFbTaskTblMemInfo	*) &segTblInfo);
			
  		if((status != NO_ERROR)||(segTblInfo.taskMode != PCS_RUN))
			continue;
		
		usedMemSize = fbMemInfo.outputSize * 4;
    		
		
		
    	
    	curDataPtr = sFbOutputUnusedMemQMngrInfo.usedList;
    	
    	if(curDataPtr == NULL) goto MODULE_END;
    	
    	while(curDataPtr != NULL) {
    		
			
			prevStartUnusedOutputMemIndex 	= curDataPtr->unUsedMemStartIndex;
			prevEndUnusedOutputMemIndex		= curDataPtr->unUsedMemEndIndex;
	
    		
    		
    		if(fbMemInfo.outputIndex == prevStartUnusedOutputMemIndex){

    			if(curDataPtr->freeMemSize == usedMemSize){
    				
					
   	        		status = nodeUnlink( 	
   	        					(strDoubleLink **)&sFbOutputUnusedMemQMngrInfo.usedList,
   								(strDoubleLink *)curDataPtr
                						);
		                            		
					if(status != NO_ERROR){
   						setErrorCode( __FILE__, __LINE__, "fbOutputUnusedMemoryInfoSet",status);
						goto MODULE_END;	
					}
					
					status = nodePutOnLast( (strDoubleLink **) &sFbOutputUnusedMemQMngrInfo.freeList,
       		        						(strDoubleLink *)curDataPtr
           		       		  			  );
					if(status != NO_ERROR){
   						setErrorCode( __FILE__, __LINE__, "fbOutputUnusedMemoryInfoSet",status);
						goto MODULE_END;	
					}
					
    				break;	
				}
				else if(curDataPtr->freeMemSize > usedMemSize){
    				
					curDataPtr->unUsedMemStartIndex		+= usedMemSize/4;
					curDataPtr->freeMemSize				-= usedMemSize; 
					curDataPtr->usableMemSize 			= curDataPtr->freeMemSize;
	   				break;						
				}
				else {
					status = FB_OUTPUT_MEM_EXCEED_ERR;
					setErrorCodeWithVal(	__FILE__, __LINE__,
											"fbOutputUnusedMemoryInfoSet",status,
											"Block Addr",blockAddr,
											"FB Code", fbMemInfo.fbId,
											"Free Mem Size",curDataPtr->freeMemSize
										 );
					goto MODULE_END;  
				}
    		}
    		
    		else if(fbMemInfo.outputIndex > prevStartUnusedOutputMemIndex){
				curEndOutputMemIndex = fbMemInfo.outputIndex + (usedMemSize/4) - 1;
				
				if(curEndOutputMemIndex < prevEndUnusedOutputMemIndex){
    					
    				
    				
    				saveEndIndex = curDataPtr->unUsedMemEndIndex;
					curDataPtr->unUsedMemEndIndex	= fbMemInfo.outputIndex - 1;
					curDataPtr->freeMemSize			= (curDataPtr->unUsedMemEndIndex - curDataPtr->unUsedMemStartIndex + 1) * 4;     				
    				curDataPtr->usableMemSize 		= curDataPtr->freeMemSize;

    						 		
				   	tmpDataPtr = (strFbSpecUnusedMemInfo *)nodeGetAndUnlink (
                		           		(strDoubleLink **)&sFbOutputUnusedMemQMngrInfo.freeList
            	        				                            		);
					if(tmpDataPtr == NULL) {
						status = NO_EXIST_NODE_ERR;
   						setErrorCode( __FILE__, __LINE__, "fbOutputUnusedMemoryInfoSet",status);
						goto MODULE_END;	
					}
					
					tmpDataPtr->unUsedMemStartIndex			= fbMemInfo.outputIndex + usedMemSize/4;

					tmpDataPtr->unUsedMemEndIndex			= saveEndIndex;
					tmpDataPtr->freeMemSize					= tmpDataPtr->unUsedMemEndIndex - tmpDataPtr->unUsedMemStartIndex + 1;
					tmpDataPtr->freeMemSize					*= 4; 
					tmpDataPtr->usableMemSize 				= tmpDataPtr->freeMemSize;

					
					status = nodePutOnLast( (strDoubleLink **) &sFbOutputUnusedMemQMngrInfo.usedList,
       		        								    		(strDoubleLink *)tmpDataPtr
           		       		  			  );
					if(status) {
        				setErrorCode( __FILE__, __LINE__, "fbOutputUnusedMemoryInfoSet",status);
						goto MODULE_END;	
					}
					
    				break;	
    			}
				else if(curEndOutputMemIndex == prevEndUnusedOutputMemIndex){
    					
    				
    				saveEndIndex = curDataPtr->unUsedMemEndIndex;
					curDataPtr->unUsedMemEndIndex		= fbMemInfo.outputIndex - 1;
					curDataPtr->freeMemSize				= (curDataPtr->unUsedMemEndIndex - curDataPtr->unUsedMemStartIndex + 1) * 4;     				
			 		curDataPtr->usableMemSize 			= curDataPtr->freeMemSize;

 					
 					
    				break;	
  				}
    			else if(curEndOutputMemIndex > prevEndUnusedOutputMemIndex)
    				goto NEXT_LOOP;	
    		} 
NEXT_LOOP:
	        curDataPtr = (strFbSpecUnusedMemInfo *)nextNodeRef ( 	
	        						(strDoubleLink *)sFbOutputUnusedMemQMngrInfo.usedList,
	                        		(strDoubleLink *)curDataPtr		
	                        									);
    	} 
    	
		if(curDataPtr == NULL) {
			status = NO_EXIST_NODE_ERR;
       		setErrorCode( __FILE__, __LINE__, "fbutputUnusedMemoryInfoSet",status);
			goto MODULE_END;	
		}
	} 

MODULE_END:
	
#ifdef VXWORKS
	semaphoreRelease (sFbOutputMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbOutputMemUnusedQSemaId);	
#endif
	return(status);
}



uint32 fbOutputMemorySpaceCheck ( uint32 numOfMem)
{
	uint32	status = NO_ERROR , retVal = NO_ERROR;
	strFbSpecUnusedMemInfo	*curNodePtr, *saveNodePtr = NULL;
	uint32	usingMemSize = 0, unUsedMemSize = 0;
	uint32 memGap,maxMemGap = 0;
	
		
	status = semaphoreGet(sFbOutputMemUnusedQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "fbOutputMemorySpaceCheck",status);
		return(status);
	}
	
	
	
	retVal = onlineDwlDelFbMemoryOutputCheck(numOfMem,FIND_SAME_MEM_SIZE);	
	if(retVal == NO_ERROR) {
		
		goto MODULE_END;
	}

	
	usingMemSize = numOfMem * 4;
	
	saveNodePtr = NULL;
	memGap = 0;
	maxMemGap = 0;
	
	curNodePtr = (strFbSpecUnusedMemInfo *)sFbOutputUnusedMemQMngrInfo.usedList;

	while(curNodePtr != NULL) {
		
		if(usingMemSize == curNodePtr->usableMemSize){ 
			saveNodePtr = curNodePtr;	
			break;
		}
		
		
		if(curNodePtr->usableMemSize > usingMemSize) {
			memGap = curNodePtr->usableMemSize - usingMemSize;	
			
			if(memGap > maxMemGap){
				maxMemGap = memGap;
				saveNodePtr = curNodePtr;
			}
		} 
		
		curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbOutputUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
                          									);	
	}		
	
	if(saveNodePtr != NULL) {
		
		saveNodePtr->usableMemSize -= usingMemSize;
		
		goto MODULE_END;	
	}	

	
	if ((curNodePtr == NULL) && (saveNodePtr == NULL)) {
		unUsedMemSize = 0;
		curNodePtr = (strFbSpecUnusedMemInfo *)sFbOutputUnusedMemQMngrInfo.usedList;
		
		while(curNodePtr != NULL) {
			unUsedMemSize += curNodePtr->usableMemSize;
		
			curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbOutputUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
                          									);	
		}		
	
		if( unUsedMemSize >= usingMemSize ){
			
				status =  FB_OUTPUT_MEM_EXCEED_ERR;
				setErrorCodeWithVal(	__FILE__, __LINE__, 
										"fbOutputMemorySpaceCheck", status,
										"Unused Output MEM Size", unUsedMemSize,
										"Request Output MEM Size", usingMemSize,
										"Not Using",0
										);
		}
		else {
			
			retVal = onlineDwlDelFbMemoryOutputCheck(numOfMem,FIND_GREATER_MEM_SIZE);	
			if(retVal != NO_ERROR) {
				
				
				
				status = SEG_MEM_EXCEED_ERR;
				setErrorCodeWithVal (	__FILE__,__LINE__,"fbOutputMemorySpaceCheck", status,
										"Request Output MEM Size", usingMemSize,
										"Unused Output MEM Size", unUsedMemSize,
										"Not Using",0
									   );
			}
		}
	}
	
MODULE_END:	

#ifdef VXWORKS
	semaphoreRelease (sFbOutputMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbOutputMemUnusedQSemaId);	
#endif
	return(status);
}



static uint32 fbOutputMemorySpaceGet (	uint32	numOfMem, 
										strFbSpecUnusedMemInfo *retNodePtr
									)
{
	uint32	status = NO_ERROR;
	strFbSpecUnusedMemInfo	*curNodePtr, *saveNodePtr = NULL;
	uint32	usingMemSize;
	uint32 memGap,maxMemGap = 0;
	
	status = semaphoreGet(sFbOutputMemUnusedQSemaId, NOWAIT);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "fbOutputMemorySpaceGet",status);
		return(status);
	}
	
	
	curNodePtr = (strFbSpecUnusedMemInfo *)sFbOutputUnusedMemQMngrInfo.usedList;
	
	if(curNodePtr == NULL) {
			
		status = SEG_MEM_EXCEED_ERR;
		setErrorCode(	__FILE__,__LINE__,"fbOutputMemorySpaceGet", status);
		goto MODULE_END;	
	}

	usingMemSize = numOfMem * 4;

	saveNodePtr = NULL;
	memGap = 0;
	maxMemGap = 0;
	
	while(curNodePtr != NULL) {
		
		if(usingMemSize == curNodePtr->freeMemSize){
			
			memoryCopy ( (uint8 *)retNodePtr,
						 (uint8 *)curNodePtr,
						 sizeof(strFbSpecUnusedMemInfo)
						);
						
			
       		status = nodeUnlink( 	
   	        					(strDoubleLink **)&sFbOutputUnusedMemQMngrInfo.usedList,
   								(strDoubleLink *)curNodePtr
           						);
		                            		
			if(status != NO_ERROR)
				setErrorCode( __FILE__, __LINE__, "fbOutputMemorySpaceGet",status);
			else {
				
				status = nodePutOnLast( (strDoubleLink **) &sFbOutputUnusedMemQMngrInfo.freeList,
       	       							(strDoubleLink *)curNodePtr
               		  			 	 );
				if(status != NO_ERROR)
					setErrorCode( __FILE__, __LINE__, "fbOutputMemorySpaceGet",status);
			}
			goto MODULE_END;
		}
		else {
			
			if(curNodePtr->freeMemSize > usingMemSize) {
				memGap = curNodePtr->freeMemSize - usingMemSize;	
			
				if(memGap > maxMemGap){
					maxMemGap = memGap;
					saveNodePtr = curNodePtr;
				}
			} 
		}
		
		curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbOutputUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
                          									);	
	}		
	
	
	if(saveNodePtr != NULL) {
		
		retNodePtr->unUsedMemStartIndex	= saveNodePtr->unUsedMemStartIndex;
		retNodePtr->unUsedMemEndIndex	= saveNodePtr->unUsedMemStartIndex + numOfMem - 1; 

		retNodePtr->usableMemSize		= usingMemSize;
		retNodePtr->freeMemSize			= usingMemSize;
		
		
		saveNodePtr->unUsedMemStartIndex	+= numOfMem;

		saveNodePtr->usableMemSize			-= usingMemSize;
		saveNodePtr->freeMemSize			-= usingMemSize;
	}	
	else {
			
		
		
		setErrorCode(	__FILE__,__LINE__,"fbOutputMemorySpaceGet", NO_EXIST_NODE_ERR);
		status = FB_OUTPUT_MEM_EXCEED_ERR;
	}
MODULE_END:	

#ifdef VXWORKS
	semaphoreRelease (sFbOutputMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbOutputMemUnusedQSemaId);	
#endif
	return(status);
}



static uint32 onlineDwlDelFbMemoryOutputCheck(uint32	numOfMem, uint32 findFlag)
{
	uint32	 fbCodeLoop; 
	strOnlineDwlDelSegMemInfo	*delSegMemInfoPtr;
	strOnlineDwlDelFbMemInfo	*delFbCodeMemInfoPtr;

	delSegMemInfoPtr	= &sOnlineDwlFbDelMemInfo.delSegMemInfo;

	delFbCodeMemInfoPtr	= &delSegMemInfoPtr->delFbMemInfo[0];

	
	if(findFlag == FIND_SAME_MEM_SIZE) {
		for(fbCodeLoop = 0; fbCodeLoop < delSegMemInfoPtr->numOfFbCode;
			fbCodeLoop++,delFbCodeMemInfoPtr++) {
				
			if(delFbCodeMemInfoPtr->usableOutputMemNum == numOfMem) {
				delFbCodeMemInfoPtr->usedOutputMemNum 	+= numOfMem;
				delFbCodeMemInfoPtr->usableOutputMemNum	-= numOfMem;
				
				return(NO_ERROR);
			}
		}
	}
	else {
		
		delFbCodeMemInfoPtr	= &delSegMemInfoPtr->delFbMemInfo[0];
		for(fbCodeLoop = 0; fbCodeLoop < delSegMemInfoPtr->numOfFbCode;
			fbCodeLoop++,delFbCodeMemInfoPtr++) {
				
			if(delFbCodeMemInfoPtr->usableOutputMemNum > numOfMem) {
				delFbCodeMemInfoPtr->usedOutputMemNum 	+= numOfMem;
				delFbCodeMemInfoPtr->usableOutputMemNum	-= numOfMem;
				
				return(NO_ERROR);
			}
		}
	}

	return(1);

}


static uint32	fbOutputFreeMemReallocCheck(void)
{
	uint32 status = NO_ERROR, freeSpaceNum = 0;
	strFbSpecUnusedMemInfo	*curNodePtr;
	
	status = semaphoreGet(sFbOutputMemUnusedQSemaId, NOWAIT);

	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "fbOutputFreeMemReallocCheck",status);
		return(status);
	}
	
	curNodePtr = (strFbSpecUnusedMemInfo *)sFbOutputUnusedMemQMngrInfo.usedList;
	
	if(curNodePtr == NULL){
		 
		status = OUTPUT_FREE_SPECE_NULL_ERR;
		goto MODULE_END;
	}
	
	freeSpaceNum = 0;
	while(curNodePtr != NULL) {
		freeSpaceNum++;
		
		curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbOutputUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
                          									);	
	}		
	
		
	if(freeSpaceNum == 0x1) 
		status = FB_PARA_MEM_REALLOC_NO_NEED;
		
MODULE_END:

#ifdef VXWORKS
	semaphoreRelease (sFbOutputMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbOutputMemUnusedQSemaId);	
#endif
	return(status);
}



uint32	fbOutputUsableMemSet(void)
{
	uint32 status = NO_ERROR;
	strFbSpecUnusedMemInfo	*curNodePtr;
	
	status = semaphoreGet(sFbOutputMemUnusedQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "fbOutputUsableMemSet",status);
		return(status);
	}
	
	curNodePtr = (strFbSpecUnusedMemInfo *)sFbOutputUnusedMemQMngrInfo.usedList;
	
	if(curNodePtr == NULL) goto MODULE_END;

	while(curNodePtr != NULL) {
		curNodePtr->usableMemSize = curNodePtr->freeMemSize;
		
		curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbOutputUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
                          									);	
	}		
	
MODULE_END:

#ifdef VXWORKS
	semaphoreRelease (sFbOutputMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbOutputMemUnusedQSemaId);	
#endif
	return(status);
}




static uint32	fbOutputMemRealloc(void)
{
	uint32 status = NO_ERROR, freeSpaceNum, tmpStatus;
	strFbSpecUnusedMemInfo	*curNodePtr, *miniNodePtr, *nextNodePtr = NULL;
	uint32	loop, loopCount;
	uint32	blockAddr, specReallocEnable = 0;
    strFbMemInfo *fbMemInfoPtr;
   	uint32 offSet, calcuStartIndex, calcuEndIndex, copyNum;
	uint32	*srcDataPtr, *destDataPtr;
	uint32	numOfCheckFbCode, checkCount = 0;
	uint16 checkFbBlkAddrList[MAX_FB_LIST_NUM+1];
	strModuleExecInfo sysExeInfo;
	uint32 taskId;
	strFbTaskTblMemInfo segTblInfo;
	uint32	checkSumCalcuNeedFlag = 0;
	
	status = semaphoreGet(sFbOutputMemUnusedQSemaId, NOWAIT);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "fbSpecMemRealloc",status);
		return(status);
	}
	
	
	
	numOfCheckFbCode = 0;
	memoryClear ( (uint8 *) &checkFbBlkAddrList[0], MAX_FB_LIST_NUM*2);
	
	numOfCheckFbCode = 0;
	for(blockAddr = 1; blockAddr <= MAX_MPU_BLOCK_ADDR; blockAddr++)
	{

		fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (blockAddr-1); 		
		
		if(fbMemInfoPtr->fbId == 0x0) continue;
	
		checkFbBlkAddrList[numOfCheckFbCode] = blockAddr;	
		numOfCheckFbCode += 1;
				
		
		if((numOfCheckFbCode >= MAX_FB_LIST_NUM) || (numOfCheckFbCode == 0x0)){
			status = DB_INDEX_EXCEED_ERR;
			setErrorCode (__FILE__, __LINE__, "fbOutputMemRealloc", status);
			goto MODULE_END;	
		}		
	}

	for(;;) 
	{
 		sFbCodeAddrListInfo.numOfFbCode 	= 0;
 		sFbCodeAddrListInfo.startParaIndex 	= 0;
 		sFbCodeAddrListInfo.endParaIndex 	= 0;

		specReallocEnable = 0;
		curNodePtr = (strFbSpecUnusedMemInfo *)sFbOutputUnusedMemQMngrInfo.usedList;
	
		if(curNodePtr == NULL){
			 
			tmpStatus = OUTPUT_FREE_SPECE_NULL_ERR;
			setErrorCode(	__FILE__,__LINE__, "fbOutputMemRealloc", tmpStatus);
			goto MODULE_END;
		}
	
		freeSpaceNum = 0;

		miniNodePtr = curNodePtr;
		
		while(curNodePtr != NULL) {
			curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbOutputUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
          	                									);	
			
			if(curNodePtr == NULL) break;
			
			
			if(miniNodePtr->unUsedMemEndIndex > curNodePtr->unUsedMemEndIndex)
				miniNodePtr = curNodePtr;	
			
			freeSpaceNum++;
		}		
	
		
			
		if(freeSpaceNum == 0x0){
			setErrorCodeWithVal (	__FILE__,__LINE__, "fbOutputMemRealloc", FB_PARA_MEM_REALLOC_NO_NEED,
								"Mini Start Index",miniNodePtr->unUsedMemStartIndex,
								"Mini End Index",miniNodePtr->unUsedMemEndIndex,
								"NOT Using",0
								  );
			goto MODULE_END;							  
		}

		
		curNodePtr	= (strFbSpecUnusedMemInfo *)sFbOutputUnusedMemQMngrInfo.usedList;
		nextNodePtr = NULL;
		
		while(curNodePtr != NULL) {
			if(curNodePtr != miniNodePtr) {
				if(nextNodePtr == NULL)
					nextNodePtr = curNodePtr;
				else if( nextNodePtr->unUsedMemEndIndex > curNodePtr->unUsedMemEndIndex)
					nextNodePtr = curNodePtr;	
			}
			curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbOutputUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
          	                									);	
		}		

		if(nextNodePtr == NULL){
			tmpStatus = FB_PARA_MEM_REALLOC_NO_NEED;
			setErrorCodeWithVal (	__FILE__,__LINE__, "fbOutputMemRealloc", tmpStatus,
								"Mini Start Index",miniNodePtr->unUsedMemStartIndex,
								"Mini End Index",miniNodePtr->unUsedMemEndIndex,
								"NOT Using",0
								  );
			goto MODULE_END;							  
		}

		
							
		loopCount = 0;	
		for(blockAddr = 1; blockAddr<=MAX_MPU_BLOCK_ADDR; blockAddr++){
			
			fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (blockAddr-1); 		
		
			if(fbMemInfoPtr->fbId == 0x0) continue;
		
			if((miniNodePtr->unUsedMemEndIndex+1) == fbMemInfoPtr->outputIndex){
				sFbCodeAddrListInfo.startParaIndex = fbMemInfoPtr->outputIndex;
				loopCount = 1;
				break;
			}
		}	
	
		if(loopCount == 0){
			tmpStatus = FB_PARA_MEM_REALLOC_NO_NEED;
			setErrorCodeWithVal (	__FILE__,__LINE__, "fbSpecMemRealloc", tmpStatus,
									"Mini Start Index",miniNodePtr->unUsedMemStartIndex,
									"Mini End Index",miniNodePtr->unUsedMemEndIndex,
									"NOT Using",0
								  );
			goto MODULE_END;							  
		}
	
							
		loopCount = 0;
		for(blockAddr = 1; blockAddr<=MAX_MPU_BLOCK_ADDR; blockAddr++){
			
			fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (blockAddr-1); 		
		
			if(fbMemInfoPtr->fbId == 0x0) continue;
			if(nextNodePtr->unUsedMemStartIndex == (fbMemInfoPtr->outputIndex+fbMemInfoPtr->outputSize)){
				sFbCodeAddrListInfo.endParaIndex = nextNodePtr->unUsedMemStartIndex - 1;
				loopCount = 1;
				break;
			}			
		}

		if(loopCount == 0){
			tmpStatus = FB_PARA_MEM_REALLOC_NO_NEED;
			setErrorCodeWithVal (	__FILE__,__LINE__, "fbOutputMemRealloc", tmpStatus,
									"Mini Start Index",miniNodePtr->unUsedMemStartIndex,
									"Mini End Index",miniNodePtr->unUsedMemEndIndex,
									"NOT Using",0
								  );
			goto MODULE_END;							  
		}

								
		offSet 						= 0;
		calcuStartIndex 			= miniNodePtr->unUsedMemEndIndex + 1;
		calcuEndIndex				= nextNodePtr->unUsedMemStartIndex;

		checkCount = 0;

		
		
		while(numOfCheckFbCode != 0x0) {
			
			blockAddr = checkFbBlkAddrList[checkCount];
			
			fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (blockAddr-1); 		
						
			if(fbMemInfoPtr->fbId == 0x0){
				if(checkCount++ > MAX_FB_LIST_NUM)
					checkCount = 0;
					
			 	continue;
			}
			if((calcuStartIndex == fbMemInfoPtr->outputIndex) && (blockAddr != 0x0) ){
				
				
				checkFbBlkAddrList[checkCount] = 0x0;
				numOfCheckFbCode -= 1;
				
				
				sFbCodeAddrListInfo.fbCodeBlkAddr[sFbCodeAddrListInfo.numOfFbCode] = blockAddr;
				sFbCodeAddrListInfo.numOfFbCode += 1;

				
				if(sFbCodeAddrListInfo.numOfFbCode >= MAX_FB_LIST_NUM) {
					status = DB_INDEX_EXCEED_ERR;
					setErrorCode (__FILE__, __LINE__, "fbOutputMemRealloc", status);
					goto MODULE_END;	
				}

				offSet = fbMemInfoPtr->outputSize;
				calcuStartIndex = fbMemInfoPtr->outputIndex + offSet;
			
				
				if(calcuEndIndex == (fbMemInfoPtr->outputIndex + offSet)){
					specReallocEnable = 1;
					
					
					checkSumCalcuNeedFlag = 1;
					break;
				}
			} 

			if(checkCount++ > MAX_FB_LIST_NUM) {
				checkCount = 0;
				continue;
			}
		} 

		if(specReallocEnable == 1) {
			
			nextNodePtr->unUsedMemStartIndex 	-= miniNodePtr->freeMemSize/4;
			nextNodePtr->freeMemSize 			+= miniNodePtr->freeMemSize;
			nextNodePtr->usableMemSize			= nextNodePtr->freeMemSize;
			
			
   	    	status = nodeUnlink( 	
   	        					(strDoubleLink **)&sFbOutputUnusedMemQMngrInfo.usedList,
   								(strDoubleLink *)miniNodePtr
           						);
		                            		
			if(status != NO_ERROR){
   				setErrorCode( __FILE__, __LINE__, "fbOutputMemRealloc",status);
				goto MODULE_END;	
			}
			
			status = nodePutOnLast( (strDoubleLink **) &sFbOutputUnusedMemQMngrInfo.freeList,
       	       						(strDoubleLink *)miniNodePtr
               		  			  );
			if(status != NO_ERROR){
   				setErrorCode( __FILE__, __LINE__, "fbOutputMemRealloc",status);
				goto MODULE_END;	
			}
			
			
			
			
			srcDataPtr 	= (uint32 *)((uint32 *)gMpuOutputDataLocalPtr + sFbCodeAddrListInfo.startParaIndex);
			destDataPtr = (uint32 *)((uint32 *)gMpuOutputDataLocalPtr + miniNodePtr->unUsedMemStartIndex);
			
			copyNum = sFbCodeAddrListInfo.endParaIndex - sFbCodeAddrListInfo.startParaIndex + 1;
			
			for(loop = 0; loop < copyNum; loop++)
				*destDataPtr++ = *srcDataPtr++;
				
			
			srcDataPtr 	= (uint32 *)((uint32 *)gMpuOutputDataBusPtr + sFbCodeAddrListInfo.startParaIndex);
			destDataPtr = (uint32 *)((uint32 *)gMpuOutputDataBusPtr + miniNodePtr->unUsedMemStartIndex);
			
			copyNum = sFbCodeAddrListInfo.endParaIndex - sFbCodeAddrListInfo.startParaIndex + 1;
			
			for(loop = 0; loop < copyNum; loop++)
				*destDataPtr++ = *srcDataPtr++;
				
				
			for(loop = 0; loop < sFbCodeAddrListInfo.numOfFbCode; loop++) {
				blockAddr = sFbCodeAddrListInfo.fbCodeBlkAddr[loop];

				
				fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (blockAddr-1);			
				fbMemInfoPtr->outputIndex -= miniNodePtr->freeMemSize/4;

				
				fbMemInfoPtr = gMpuFbMemInfoBusPtr + (blockAddr-1);
				fbMemInfoPtr->outputIndex -= miniNodePtr->freeMemSize/4;
			}
			
			
			specReallocEnable = 0;
			
			sFbCodeAddrListInfo.numOfFbCode 	= 0;
 			sFbCodeAddrListInfo.startParaIndex 	= 0;
 			sFbCodeAddrListInfo.endParaIndex 	= 0;
		}
		else {
			
			
			status = FB_OUTPUT_MEM_REALLOC_ERR;
			setErrorCode (__FILE__, __LINE__, "fbOutputMemRealloc", status);
			goto MODULE_END;	
		}
	}

MODULE_END:
	
	
	if( checkSumCalcuNeedFlag ) {
		memoryClear( (uint8 *)&sysExeInfo, sizeof(strModuleExecInfo));
		
		status = fbSysExeModuleInfoRead(LOGIC_ID, &sysExeInfo);
		if(status!=NO_ERROR){
			setErrorCode((uint8 *)__FILE__, __LINE__, (uint8 *)"fbOutputMemRealloc", status);
			goto MODULE_END;	
		}
		
		for(taskId=1 ;taskId<=sysExeInfo.curMaxSegmentId ;taskId++)
		{
			if( (getFbTaskTblInfo(LOGIC_ID,taskId,&segTblInfo) == NO_ERROR) 
					&& (segTblInfo.taskId == taskId) )	{
				(void)onlineDwlSegmentCheckSumCalcu(taskId,ONLINE_FB_PARA_REALLOC, 0,0);
			}
			else continue;
		}
	}

#ifdef VXWORKS
	semaphoreRelease (sFbOutputMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbSpecMemUnusedQSemaId);	
#endif
	
	return(status);
}



static uint32 fbOutputMemorySpaceRelease (	uint32	fbId, 
											uint32	fbBlkAddr
										)
{
	uint32	status = NO_ERROR;
	strFbSpecUnusedMemInfo	*curNodePtr, *reqNodePtr, *saveNodePtr;
	uint32	unUsedMemByteNum;
	strFbMemInfo	*fbMemInfoPtr;	
	
	
	fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (fbBlkAddr-1);
	
	status = semaphoreGet(sFbOutputMemUnusedQSemaId, NOWAIT);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "fbOutputMemorySpaceRelease",status);
		return(status);
	}
	
	
	curNodePtr = (strFbSpecUnusedMemInfo *)sFbOutputUnusedMemQMngrInfo.usedList;
	
	if(curNodePtr == NULL) {
			
		reqNodePtr = (strFbSpecUnusedMemInfo *)nodeGetAndUnlink(
                    		(strDoubleLink **)&sFbOutputUnusedMemQMngrInfo.freeList
																);
		if(reqNodePtr == NULL){
			status = NO_EXIST_NODE_ERR;
			setErrorCode(	__FILE__,__LINE__,"fbOutputMemorySpaceRelease", status);
			goto MODULE_END;
		}
		unUsedMemByteNum = (fbMemInfoPtr->outputSize) * 4;
		
		
		reqNodePtr->usableMemSize			= unUsedMemByteNum;	
		reqNodePtr->unUsedMemStartIndex		= fbMemInfoPtr->outputIndex;
		reqNodePtr->unUsedMemEndIndex		= fbMemInfoPtr->outputIndex + (unUsedMemByteNum/4) - 1;
		reqNodePtr->freeMemSize				= unUsedMemByteNum;	
		
		status = nodePutOnLast( (strDoubleLink **) &sFbOutputUnusedMemQMngrInfo.usedList,
       							(strDoubleLink *)reqNodePtr
        	  			 	 );
		if(status != NO_ERROR)
			setErrorCode(	__FILE__,__LINE__,"fbOutputMemorySpaceRelease", status);

		goto MODULE_END;	
	}
	else {
			
		
		
		saveNodePtr = NULL;

		unUsedMemByteNum = (fbMemInfoPtr->outputSize) * 4;

		while(curNodePtr != NULL){
			if( 
				((curNodePtr->unUsedMemEndIndex + 1) == fbMemInfoPtr->outputIndex) ||
				(curNodePtr->unUsedMemStartIndex == (fbMemInfoPtr->outputIndex + (unUsedMemByteNum/4)))
			){
				saveNodePtr = curNodePtr;
				break;	
			}
	
			curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbOutputUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
    	                      									);	
		}
		
		if(curNodePtr != NULL) {
			
			if( (curNodePtr->unUsedMemEndIndex + 1) == fbMemInfoPtr->outputIndex){
					
				
				saveNodePtr->usableMemSize			+= unUsedMemByteNum;	
				saveNodePtr->freeMemSize			+= unUsedMemByteNum;	
				saveNodePtr->unUsedMemEndIndex		+= (unUsedMemByteNum/4);
			}
			else {
					
				
				saveNodePtr->usableMemSize			+= unUsedMemByteNum;	
				saveNodePtr->freeMemSize			+= unUsedMemByteNum;	
				saveNodePtr->unUsedMemStartIndex	= fbMemInfoPtr->outputIndex;
			}

			
			
			curNodePtr = (strFbSpecUnusedMemInfo *)sFbOutputUnusedMemQMngrInfo.usedList;
			while(curNodePtr != NULL) {
				if( 
					((saveNodePtr->unUsedMemEndIndex + 1) == curNodePtr->unUsedMemStartIndex) ||
					(saveNodePtr->unUsedMemStartIndex == (curNodePtr->unUsedMemEndIndex + 1))
				){
					break;	
				}
				
				curNodePtr = (strFbSpecUnusedMemInfo *) nextNodeRef(  
													(strDoubleLink *)sFbOutputUnusedMemQMngrInfo.usedList,
                              						(strDoubleLink *)curNodePtr
    	                      										);	
			}
			if(curNodePtr == NULL) goto MODULE_END;
				
			if( (saveNodePtr->unUsedMemEndIndex + 1) == curNodePtr->unUsedMemStartIndex){
					
				
				saveNodePtr->usableMemSize			+= curNodePtr->usableMemSize;	
				saveNodePtr->freeMemSize			+= curNodePtr->freeMemSize;	
				saveNodePtr->unUsedMemEndIndex		= curNodePtr->unUsedMemEndIndex;
			}
			else {
					
				
				saveNodePtr->usableMemSize			+= curNodePtr->usableMemSize;	
				saveNodePtr->freeMemSize			+= curNodePtr->freeMemSize;	
				saveNodePtr->unUsedMemStartIndex	= curNodePtr->unUsedMemStartIndex;
			}
			
			
   	        status = nodeUnlink( 	
   	        					(strDoubleLink **)&sFbOutputUnusedMemQMngrInfo.usedList,
   								(strDoubleLink *)curNodePtr
            					);
		                            		
			if(status != NO_ERROR){
   				setErrorCode( __FILE__, __LINE__, "fbOutputMemorySpaceRelease",status);
				goto MODULE_END;	
			}
			
			status = nodePutOnLast( (strDoubleLink **) &sFbOutputUnusedMemQMngrInfo.freeList,
       		   						(strDoubleLink *)curNodePtr
           	   		  			  );
			if(status != NO_ERROR){
   				setErrorCode( __FILE__, __LINE__, "fbOutputMemorySpaceRelease",status);
				goto MODULE_END;	
			}
		} 
		else if((curNodePtr == NULL)) { 
			
			
			reqNodePtr = (strFbSpecUnusedMemInfo *)nodeGetAndUnlink(
                    			(strDoubleLink **)&sFbOutputUnusedMemQMngrInfo.freeList
																	);
			if(reqNodePtr == NULL){
				status = NO_EXIST_NODE_ERR;
				setErrorCode(	__FILE__,__LINE__,"fbOutputMemorySpaceRelease", status);
				goto MODULE_END;
			}
			unUsedMemByteNum = (fbMemInfoPtr->outputSize) * 4;
		
			
			reqNodePtr->usableMemSize			= unUsedMemByteNum;	
			reqNodePtr->unUsedMemStartIndex		= fbMemInfoPtr->outputIndex;
			reqNodePtr->unUsedMemEndIndex		= fbMemInfoPtr->outputIndex + (unUsedMemByteNum/4) - 1;
			reqNodePtr->freeMemSize				= unUsedMemByteNum;	
		
			status = nodePutOnLast( (strDoubleLink **) &sFbOutputUnusedMemQMngrInfo.usedList,
       								(strDoubleLink *)reqNodePtr
        	  			 	 	  );
        	  			 	 	  
			if(status != NO_ERROR)
				setErrorCode(	__FILE__,__LINE__,"fbOutputMemorySpaceRelease", status);

			goto MODULE_END;	
		}
	} 
	
MODULE_END:	

#ifdef VXWORKS
	semaphoreRelease (sFbOutputMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbOutputMemUnusedQSemaId);	
#endif
	return(status);
}



uint32 fbUnusedOutputMemInfoRead(strFbSpecMemStatusReadRespInfo *respInfoPtr)
{
	uint32 status = NO_ERROR, loop;
	strFbSpecUnusedMemInfo	*nodePtr;

	
    status = semaphoreGet(sFbOutputMemUnusedQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);;
	}

 	nodePtr = sFbOutputUnusedMemQMngrInfo.usedList;
    if(nodePtr == NULL) {
    	respInfoPtr->fragmentMemNum = 0;
    	goto MODULE_END;
    }
    
    loop = 0;
   	while(nodePtr != NULL) {
    	respInfoPtr->fragmentMemNum++;
		
		if(respInfoPtr->fragmentMemNum > MAX_FB_SPEC_FRAGMENT_NUM) {
			status = FB_OUTPUT_MEM_EXCEED_ERR ;
			setErrorCodeWithVal(	__FILE__, __LINE__,
									"fbUnusedOutputMemInfoRead",status,
									"Fragment MEM NUM", respInfoPtr->fragmentMemNum,
									"MAX MEM NUM", MAX_FB_SPEC_FRAGMENT_NUM,
									"NOT USING", 0
						  		);
			break;
		}
		
		
		respInfoPtr->fbParaMemStatusInfo[loop].fragmentMemEndIndex		= nodePtr->unUsedMemEndIndex;
		respInfoPtr->fbParaMemStatusInfo[loop].fragmentMemStartIndex	= nodePtr->unUsedMemStartIndex;
		
		
		respInfoPtr->freeMemSize += (nodePtr->freeMemSize/4);
		loop++;
		
       	nodePtr = (strFbSpecUnusedMemInfo *)nextNodeRef ( 	
       								(strDoubleLink *)sFbOutputUnusedMemQMngrInfo.usedList,
	       	                		(strDoubleLink *)nodePtr
							  							);
	}

MODULE_END:
	
#ifdef VXWORKS
	semaphoreRelease (sFbOutputMemUnusedQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFbOutputMemUnusedQSemaId);	
#endif
	return(status);
}


uint32 onlineDwlFbInfoChgVariableClear(void)
{
	uint32	status = NO_ERROR;
	
	
	sOnlineDwlFbInfoChgInfo.dwlCurFrameNum = 0;
	sOnlineDwlFbInfoChgInfo.dwlTotalFbCodeNum = 0;
	sOnlineDwlFbInfoChgInfo.dwlTotalFrameNum = 0;
	sOnlineDwlFbInfoChgInfo.numOfTotalFbCode = 0;
	sOnlineDwlFbInfoChgInfo.numOfTotalSegment = 0;
	memoryClear( (uint8 *)sOnlineDwlFbInfoChgInfo.fbInfoChgSegInfoPtr,
					 sizeof(strOnlineDwlFbInfoChgSegInfo)*MAX_ONLINE_DWL_SEG_FBINFO_CHANGE_NUM);


	
	status = onlineDwlTaskReqQueClear(ONLINE_DWL_FBCODE_CHANGE);
	if(status != NO_ERROR) 
		setErrorCode (__FILE__, __LINE__, "onlineDwlFbInfoChgVariableClear", status);
		
	return(status);
}


uint32 onlineDwlFbCodeInfoChgFuncInComTask(	uint32 	*reqDataPtr, 
												uint32	*respDataPtr,
												uint32	*respRetLenPtr
											)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32	numOfSegment, numOfFbInfoChgFbCode, numOfTotalFbInfoChgFbCode,segmentId,fbId, sumOfFbCodeNum;	
	uint32	*saveReqDataPtr;
	uint32	fbBlkAddr , numOfTotalSegment;
	strFbTaskTblMemInfo	segTblInfo;
	strFbMemInfo	fbMemInfo;
	uint32	maxSegId, loop, fbInfoChgLoop, segFbInfoChgLoop;
	uint32	totalFrameNum,curFrameId,checkLoop, prevNumOfFbCode;
	strOnlineDwlFbInfoChgFbCodeMngrInfo	*onlineFbInfoChgMngrInfoPtr = &sOnlineDwlFbInfoChgInfo;
	strOnlineDwlFbInfoChgSegInfo		*onlineSegFbInfoChgInfoPtr = onlineFbInfoChgMngrInfoPtr->fbInfoChgSegInfoPtr;
	strOnlineDwlFbInfoChgFbCodeInfo		*onlineFbInfoChgInfoPtr;

	uint32 taskLogicType = 0;
	uint32 outputNegate[2];

	memoryClear((uint8*)&outputNegate[2],LONG_SIZE);
						
	*respRetLenPtr	= 0;

	
	totalFrameNum	= (*reqDataPtr & 0xffff0000) >> 16;
	curFrameId 		= *reqDataPtr++ & 0xffff;
	
	
	saveReqDataPtr 	= reqDataPtr;
	
	
	
	numOfTotalSegment	= (*reqDataPtr & 0xffff0000) >> 16;
	numOfSegment 		= (*reqDataPtr++ & 0xffff);

	
	*respDataPtr++ 	= numOfSegment;
	*respRetLenPtr 	+= 4;

		
    if( (numOfTotalSegment == 0) || (numOfTotalSegment > MAX_ONLINE_DWL_SEG_FBINFO_CHANGE_NUM)){
		status = ONLINE_DWL_SEG_NUM_ERR;
       	setErrorCodeWithVal( 	__FILE__,	__LINE__,
               	        		"onlineDwlFbCodeInfoChgFuncInComTask", status,
                   	    		"Total Segment Chg Number",numOfTotalSegment,
                   	    		"MAX Segment Chg Number", MAX_ONLINE_DWL_SEG_FBINFO_CHANGE_NUM,
                   	    		"NOT Using", 0 
				  	  		);
		goto MODULE_END;
	}
	
    else if( (numOfSegment == 0) || (numOfSegment > MAX_ONLINE_DWL_SEG_FBINFO_CHANGE_NUM)){
		status = ONLINE_DWL_SEG_NUM_ERR;
       	setErrorCodeWithVal( 	__FILE__,	__LINE__,
               	        		"onlineDwlFbCodeInfoChgFuncInComTask",status,
                   	    		"Current Segment Number",numOfSegment,
                   	    		"Prev Segment Number", onlineFbInfoChgMngrInfoPtr->numOfTotalFbCode,
                   	    		"MAX FB Chg Number", MAX_ONLINE_DWL_SEG_FBINFO_CHANGE_NUM
				  	  		);
		goto MODULE_END;
	}

	
	numOfTotalFbInfoChgFbCode = *reqDataPtr++;
	
	if(	(numOfTotalFbInfoChgFbCode > MAX_ONLINE_DWL_FB_INFO_CHANGE_NUM)||
		(numOfTotalFbInfoChgFbCode == 0x0)
	){
		status = FB_NUM_EXCEED_ERR;
    	setErrorCodeWithVal( 	__FILE__, __LINE__,
           		        		"onlineDwlFbCodeInfoChgFuncInComTask",status,
               	    			"Total FB Chg Number", numOfTotalFbInfoChgFbCode,
               	    			"MAX FB Chg Number", MAX_ONLINE_DWL_FB_INFO_CHANGE_NUM,
               	    			"NOT Using",0
			  	  			);
		goto MODULE_END;
	}		
	
	
	if(curFrameId == 0x1) {
		onlineFbInfoChgMngrInfoPtr->numOfTotalSegment 	= numOfTotalSegment;
		
		
		onlineFbInfoChgMngrInfoPtr->dwlTotalFbCodeNum 	= numOfTotalFbInfoChgFbCode;	
		onlineFbInfoChgMngrInfoPtr->dwlTotalFrameNum 	= totalFrameNum;	
	}
	
	onlineFbInfoChgMngrInfoPtr->dwlCurFrameNum 	= curFrameId;	
	
	
	sumOfFbCodeNum = 0;
	for(segFbInfoChgLoop = 0; segFbInfoChgLoop < numOfSegment; segFbInfoChgLoop++) {

		
		taskLogicType   = (*reqDataPtr>> 31) & 0x1;
		segmentId 		= *reqDataPtr++ & 0xffff;

	
		if((segmentId == 0) || (segmentId > MAX_LOGIC_TASK_NUM)) {
			status = L_SEG_ID_ERR;
       		setErrorCodeWithVal( 	__FILE__, __LINE__,
          	     	        		"onlineDwlFbCodeInfoChgFuncInComTask",status,
           	        	    		"Segment ID",segmentId,
           	        	    		"MAX Segment ID", MAX_LOGIC_TASK_NUM,
           	        	    		"NOT Using", 0 
					  	  		);
			goto MODULE_END;
		}

		
		maxSegId = 0;
		for(loop = 1; loop <= MAX_LOGIC_TASK_NUM; loop++) {
			memoryClear( (uint8 *)&segTblInfo, sizeof(strFbTaskTblMemInfo));
		
   			(void)getFbTaskTblInfo( 
								LOGIC_ID,
           						loop,
								(strFbTaskTblMemInfo	*) &segTblInfo
								);
			if(segTblInfo.taskId > maxSegId)
				maxSegId = segTblInfo.taskId;
			
			if(segTblInfo.taskId == segmentId) break;
		}

		if(segTblInfo.taskId == 0) {
			status = ONLINE_DWL_SEG_EXIST_ERR;
       		setErrorCodeWithVal( 	__FILE__, __LINE__,
           	    	        		"onlineDwlFbCodeInfoChgFuncInComTask", status,
                	   	    		"Seg ID",segmentId,
                 	  	    		"Cur Max Seg Id", maxSegId,
                  	 	    		"NOT Using", 0 
				  		  		);
			goto MODULE_END;		
		}

		if(segTblInfo.logicType != taskLogicType) {
			status = L_LOGIC_ID_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,
          	     	        		"onlineDwlFbCodeInfoChgFuncInComTask",status,
									"SegID",segmentId,
           	        	    		"Curr Logic Type",segTblInfo.logicType,
           	        	    		"Recv Logic Type",taskLogicType
					  	  		);
			goto MODULE_END;
		}


			
			
			numOfTotalFbInfoChgFbCode	= (*reqDataPtr & 0xffff0000) >> 16;	 
			numOfFbInfoChgFbCode		= *reqDataPtr++ & 0xffff;
			sumOfFbCodeNum			+= 	numOfFbInfoChgFbCode;

			
			if( (numOfTotalFbInfoChgFbCode > MAX_ONLINE_DWL_FB_INFO_CHANGE_NUM) ||
				(numOfFbInfoChgFbCode > MAX_ONLINE_DWL_FB_INFO_CHANGE_NUM) ||
				(numOfTotalFbInfoChgFbCode < numOfFbInfoChgFbCode) ||
				(numOfTotalFbInfoChgFbCode == 0x0) ||
				(numOfFbInfoChgFbCode == 0x0)
			) {
				status = FB_NUM_EXCEED_ERR;
    			setErrorCodeWithVal( 	__FILE__, __LINE__,
           			        			"onlineDwlFbCodeInfoChgFuncInComTask", status,
               	    					"Total FB Chg  Number", numOfTotalFbInfoChgFbCode,
               	    					"Fb Chg Number",numOfFbInfoChgFbCode,
               	    					"MAX FB Code Chg Number", MAX_ONLINE_DWL_FB_INFO_CHANGE_NUM
			  	  					);
				goto MODULE_END;
			}
		
			for(fbInfoChgLoop = 0; fbInfoChgLoop < numOfFbInfoChgFbCode; fbInfoChgLoop++) {
				
				fbBlkAddr	= (*reqDataPtr >> 16) & 0xffff;
				fbId 		= *reqDataPtr++ & 0xffff;
				
				
				memoryCopy((uint8*)&outputNegate[0],(uint8*)reqDataPtr++,LONG_SIZE);
				reqDataPtr++;

				if(
					(  (gFbCodeSortingTableInfo[fbId].outputNo <=32 ) &&
					   ((outputNegate[0] >> gFbCodeSortingTableInfo[fbId].outputNo) != 0x0) )
													||
					(  (gFbCodeSortingTableInfo[fbId].outputNo>32)&&
					   ((outputNegate[1] >> gFbCodeSortingTableInfo[fbId].outputNo) != 0x0) ) 
				){
					status = FB_CODE_OUTPUT_ACTION_ERR;
					setErrorCodeWithVal(	(uint8 *)__FILE__, (uint32)__LINE__, 
											(uint8 *)"onlineDwlFbCodeInfoChgFuncInComTask", status,
											"FB Code", fbId,
											"Output Action(MSB)", outputNegate[0],
											"Output Action(LSB)", outputNegate[1]
		  		  						);
					break;
				}

    			(void)fbCodeMemInfoRead	(
											LOGIC_ID,
       										fbBlkAddr,
       										(strFbMemInfo *) &fbMemInfo
       									);
       									
				
				if( (fbBlkAddr < 1) ||
					(fbBlkAddr > MAX_MPU_BLOCK_ADDR)
				){
					status = SEG_BLK_ADDR_EXCEED_ERR;
	       			setErrorCodeWithVal( 	__FILE__,__LINE__,
        	   	    	        			"onlineDwlFbCodeInfoChgFuncInComTask",status,
                		   	    			"FB Addr",fbBlkAddr,
                 		  	    			"MAX FB Addr", MAX_MPU_BLOCK_ADDR,
                  	 		    			"Not Using", 0
				  		  				);
				}   
				
				else if(fbId > MAX_FB_CODE_ID){
					status = L_FB_CODE_ERR;
	       			setErrorCodeWithVal( 	__FILE__, __LINE__,
        	   	    	        			"onlineDwlFbCodeInfoChgFuncInComTask", status,
                		   	    			"FB Code",fbId,
                 		  	    			"Max FB Code ", MAX_FB_CODE_ID,
                  	 		    			"Not Using", 0 
				  		  				);
				}				
				
				else if( fbId != gFbCodeSortingTableInfo[fbId].fbId){
					status = L_FB_CODE_ERR;
	       			setErrorCodeWithVal( 	__FILE__, __LINE__,
        	   	    	        			"onlineDwlFbCodeInfoChgFuncInComTask", status,
                		   	    			"FB Code",fbId,
                 		  	    			"Defined Fb Code ", gFbCodeSortingTableInfo[fbId].fbId,
                  	 		    			"Not Using", 0 
				  		  				);
				}				
			
				
				else if(fbId != fbMemInfo.fbId) {
					status = ONLINE_FB_CODE_MISMATCH_ERR;
	       			setErrorCodeWithVal( 	__FILE__, __LINE__,
        	   	   		        			"onlineDwlFbCodeInfoChgFuncInComTask", status,
                	   	    				"Block Addr",fbBlkAddr,
                 	  	    				"Request FB Code ", fbId,
                  		    				"Read FB Code", fbMemInfo.fbId 
				 	  					);
				}

				if(status != NO_ERROR) goto MAKE_RESP;
				
		}	

		
		
			
MAKE_RESP:
		*respDataPtr 	= (status & 0xffff) << 16;
		*respDataPtr++	|= segmentId;
		*respRetLenPtr 	+= 4;	
		
		if(status != NO_ERROR) goto MODULE_END;
	} 
	
	
	onlineFbInfoChgMngrInfoPtr->numOfTotalFbCode 	+= sumOfFbCodeNum;

	
	if(totalFrameNum == curFrameId) {
		 if((onlineFbInfoChgMngrInfoPtr->numOfTotalFbCode > MAX_ONLINE_DWL_FB_SPEC_CHANGE_NUM)||
		 	(onlineFbInfoChgMngrInfoPtr->numOfTotalFbCode == 0x0)||
			(onlineFbInfoChgMngrInfoPtr->dwlTotalFbCodeNum != onlineFbInfoChgMngrInfoPtr->numOfTotalFbCode)
		 ){
			status = FB_NUM_EXCEED_ERR;
       		setErrorCodeWithVal( 	__FILE__, __LINE__,
               		        		"onlineDwlFbCodeInfoChgFuncInComTask",	status,
                   	    			"SUM Fb Code ADD Number",sumOfFbCodeNum,
                   	    			"Total FB   Number", onlineFbInfoChgMngrInfoPtr->numOfTotalFbCode,
                   	    			"Total FB Set  Number", onlineFbInfoChgMngrInfoPtr->dwlTotalFbCodeNum
				  	  			);
			goto MODULE_END;
		}		
	}		

		
	
	reqDataPtr = saveReqDataPtr;

	
	numOfSegment = *reqDataPtr++&0xffff;

	
	numOfTotalFbInfoChgFbCode = *reqDataPtr++;

	for(segFbInfoChgLoop = 0; segFbInfoChgLoop < numOfSegment; segFbInfoChgLoop++){
		
		
		taskLogicType   = (*reqDataPtr>> 31) & 0x1;
		segmentId 		= *reqDataPtr++ & 0xffff;

		
		for(checkLoop = 0; checkLoop < MAX_ONLINE_DWL_SEG_FBINFO_CHANGE_NUM; checkLoop++) {
			if((onlineSegFbInfoChgInfoPtr->taskId == 0) || (onlineSegFbInfoChgInfoPtr->taskId == segmentId))
				break;
			
			onlineSegFbInfoChgInfoPtr++;	
		}
		if(checkLoop == MAX_ONLINE_DWL_SEG_FBINFO_CHANGE_NUM) {
			status = FB_ONLINE_DOWNLOAD_ERR;
			setErrorCode (__FILE__,__LINE__,"onlineDwlFbCodeInfoChgFuncInComTask", status);
			goto MODULE_END;	
		}
		
			
			numOfFbInfoChgFbCode	= *reqDataPtr++ & 0xffff;
			prevNumOfFbCode	= onlineSegFbInfoChgInfoPtr->numOfFbCode;
			
			onlineSegFbInfoChgInfoPtr->taskId 			= segmentId;
			onlineSegFbInfoChgInfoPtr->numOfFbCode 	+= numOfFbInfoChgFbCode;	
			onlineSegFbInfoChgInfoPtr->logicType		= taskLogicType;

			if(onlineSegFbInfoChgInfoPtr->numOfFbCode  > MAX_ONLINE_DWL_FB_INFO_CHANGE_NUM) {
				status = FB_NUM_EXCEED_ERR;
				setErrorCode (__FILE__,__LINE__,"onlineDwlFbCodeInfoChgFuncInComTask", status);
				goto MODULE_END;	
			}

			
			
			onlineFbInfoChgInfoPtr = onlineSegFbInfoChgInfoPtr->fbInfoChgFbCodeInfo + prevNumOfFbCode;

			for(fbInfoChgLoop = 0; fbInfoChgLoop < numOfFbInfoChgFbCode; fbInfoChgLoop++,onlineFbInfoChgInfoPtr++){
				
				fbBlkAddr = (*reqDataPtr >> 16) & 0xffff;
				fbId = *reqDataPtr++ & 0xffff;	

				
				memoryCopy((uint8*)&outputNegate[0],(uint8*)reqDataPtr++,LONG_SIZE);
				reqDataPtr++;

				memoryCopy(((uint8*)&onlineFbInfoChgInfoPtr->outputAction),
								(uint8*)&outputNegate[0], LONG_SIZE);

				
				onlineFbInfoChgInfoPtr->fbBlkAddr 	= (uint32)fbBlkAddr;
				onlineFbInfoChgInfoPtr->fbId		= (uint32)fbId;


			}	
	

	} 
	
	
	retVal = onlineDwlReqDataSendToFbTask(ONLINE_DWL_FBCODE_CHANGE,NULL, NULL);	

	if(retVal != NO_ERROR){
		setErrorCode (__FILE__, __LINE__, "onlineDwlFbCodeInfoChgFuncInComTask", retVal);
		status = FB_ONLINE_DOWNLOAD_ERR;
	}

MODULE_END:
	return(status);
}


static uint32 onlineDwlFbCodeInfoChgFuncInFbTask(strOnlineDwlTaskComReqInfo *argPtr)
{
	uint32	status = NO_ERROR;
	uint32	numOfSegment, numOfFbInfoChgFbCode, segmentId,fbId, sumOfFbCodeNum, totalFbCodeNum;	
	uint32	fbBlkAddr;
	strFbTaskTblMemInfo	segTblInfo, *segTblInfoPtr;
	strFbMemInfo	fbMemInfo;
	uint32	maxSegId, loop, fbInfoChgLoop, segFbInfoChgLoop;
	strOnlineDwlFbInfoChgFbCodeMngrInfo	*onlineFbInfoChgMngrInfoPtr = &sOnlineDwlFbInfoChgInfo;
	strOnlineDwlFbInfoChgSegInfo		*onlineSegFbInfoChgInfoPtr = onlineFbInfoChgMngrInfoPtr->fbInfoChgSegInfoPtr;
	strOnlineDwlFbInfoChgFbCodeInfo		*onlineFbInfoChgInfoPtr;
	
			
	
	if(onlineFbInfoChgMngrInfoPtr->dwlTotalFrameNum != onlineFbInfoChgMngrInfoPtr->dwlCurFrameNum) {
		status = ONLINE_DWL_FRAME_NUM_ERR; 
		setErrorCodeWithVal(	__FILE__, __LINE__,"onlineDwlFbCodeInfoChgFuncInFbTask",status,
								"Total Frame Number", onlineFbInfoChgMngrInfoPtr->dwlTotalFrameNum,
								"Current Frame Number",onlineFbInfoChgMngrInfoPtr->dwlCurFrameNum,
								"Not Using", 0
							 );
		return(status);							 							
	}
	
	numOfSegment = onlineFbInfoChgMngrInfoPtr->numOfTotalSegment;
	totalFbCodeNum = onlineFbInfoChgMngrInfoPtr->numOfTotalFbCode;

    if( (numOfSegment == 0) || (numOfSegment > MAX_ONLINE_DWL_SEG_FBINFO_CHANGE_NUM)){
		status = FB_NUM_EXCEED_ERR;
       	setErrorCodeWithVal( 	__FILE__, __LINE__,
               	        		"onlineDwlFbCodeInfoChgFuncInFbTask", status,
                   	    		"Total Segment Blk Chg Number",numOfSegment,
                   	    		"MAX Segment Blk Chg Number", MAX_ONLINE_DWL_SEG_FBINFO_CHANGE_NUM,
                   	    		"NOT Using", 0 
				  	  		);
		goto MODULE_END;
	}
    else if( (totalFbCodeNum == 0) || (totalFbCodeNum > MAX_ONLINE_DWL_FB_INFO_CHANGE_NUM)){
		status = FB_NUM_EXCEED_ERR;
       	setErrorCodeWithVal( 	__FILE__, __LINE__,
               	        		"onlineDwlFbCodeInfoChgFuncInFbTask", status,
                   	    		"Total Fb Code Blk Chg Number",totalFbCodeNum,
                   	    		"MAX FB Code Blk Chg Number", MAX_ONLINE_DWL_FB_INFO_CHANGE_NUM,
                   	    		"NOT Using", 0 
				  	  		);
		goto MODULE_END;
	}
	
	
	sumOfFbCodeNum = 0;
	for(segFbInfoChgLoop = 0; segFbInfoChgLoop < numOfSegment; segFbInfoChgLoop++,onlineSegFbInfoChgInfoPtr++) {
		numOfFbInfoChgFbCode = onlineSegFbInfoChgInfoPtr->numOfFbCode;
		
		sumOfFbCodeNum += numOfFbInfoChgFbCode;
		
		segmentId = onlineSegFbInfoChgInfoPtr->taskId;
		
		if(sumOfFbCodeNum > totalFbCodeNum) {
			status = ONLINE_FB_CODE_NUM_MISMATCH_ERR;
			setErrorCodeWithVal (	__FILE__,__LINE__, "onlineDwlFbCodeInfoChgFuncInFbTask", status,
									"Total FB Code NUM",totalFbCodeNum,
									"Sum Of Fb Code NUM",sumOfFbCodeNum,
									"NOT USING", 0
								   );
			goto MODULE_END;  
		}
		
		if((segmentId == 0) || (segmentId > MAX_LOGIC_TASK_NUM)) {
			status = L_SEG_ID_ERR;
       		setErrorCodeWithVal( 	__FILE__,	__LINE__,
          	     	        		"onlineDwlFbCodeInfoChgFuncInFbTask", status,
           	        	    		"Segment ID",segmentId,
           	        	    		"MAX Segment ID", MAX_LOGIC_TASK_NUM,
           	        	    		"NOT Using", 0 
					  	  		);
			goto MODULE_END;
		}

		
		maxSegId = 0;
		for(loop = 1; loop <= MAX_LOGIC_TASK_NUM; loop++) {
			memoryClear( (uint8 *)&segTblInfo, sizeof(strFbTaskTblMemInfo));
		
   			(void)getFbTaskTblInfo( 
								LOGIC_ID,
           						loop,
								(strFbTaskTblMemInfo	*) &segTblInfo
								);
			if(segTblInfo.taskId > maxSegId)
				maxSegId = segTblInfo.taskId;
			
			if(segTblInfo.taskId == segmentId) break;
		}

		if(segTblInfo.taskId == 0) {
			status = ONLINE_DWL_SEG_EXIST_ERR;
       		setErrorCodeWithVal( 	__FILE__, __LINE__,
           	    	        		"onlineDwlFbCodeInfoChgFuncInFbTask", status,
                	   	    		"Segment ID",segmentId,
                 	  	    		"Current Max Seg Id", maxSegId,
                  	 	    		"NOT Using", 0 
				  		  		);
			goto MODULE_END;		
		}
		
		numOfFbInfoChgFbCode = onlineSegFbInfoChgInfoPtr->numOfFbCode;
		
		onlineFbInfoChgInfoPtr = &onlineSegFbInfoChgInfoPtr->fbInfoChgFbCodeInfo[0];
		
		for(fbInfoChgLoop = 0; fbInfoChgLoop < numOfFbInfoChgFbCode; fbInfoChgLoop++,onlineFbInfoChgInfoPtr++) {
			fbId 		= onlineFbInfoChgInfoPtr->fbId;
			fbBlkAddr	= onlineFbInfoChgInfoPtr->fbBlkAddr;
			
    		(void)fbCodeMemInfoRead	(
										LOGIC_ID,
       									fbBlkAddr,
       									(strFbMemInfo *) &fbMemInfo
       								);
       								
			
			if( (fbBlkAddr < 1) ||
				(fbBlkAddr > MAX_MPU_BLOCK_ADDR)
			){
				status = SEG_BLK_ADDR_EXCEED_ERR;
	       		setErrorCodeWithVal( 	__FILE__, __LINE__,
        	   	    	        		"onlineDwlFbCodeInfoChgFuncInFbTask",status,
                		   	    		"FB Addr",fbBlkAddr,
                 		  	    		"MAX FB Addr", MAX_MPU_BLOCK_ADDR,
                  	 		    		"Not Using", 0 
				  		  			);
							  		  				
			}   
			else if(fbId > MAX_FB_CODE_ID){
				status = L_FB_CODE_ERR;
	       		setErrorCodeWithVal( 	__FILE__, __LINE__,
        	   	    	        		"onlineDwlFbCodeInfoChgFuncInFbTask", status,
                		   	    		"FB Code",fbId,
                 		  	    		"Max FB Code ", MAX_FB_CODE_ID,
                  	 		    		"Not Using", 0 
				  		  			);
			}				
			
			else if( fbId != gFbCodeSortingTableInfo[fbId].fbId){
				status = L_FB_CODE_ERR;
	       		setErrorCodeWithVal( 	__FILE__,	__LINE__,
        	   	    	        		"onlineDwlFbCodeInfoChgFuncInFbTask", status,
                		   	    		"FB Code",fbId,
                 		  	    		"Defined Fb Code ", gFbCodeSortingTableInfo[fbId].fbId,
                  	 		    		"Not Using", 0 
				  		  			);
			}				

			
			else if(fbId != fbMemInfo.fbId) {
				status = ONLINE_FB_CODE_MISMATCH_ERR;
	       		setErrorCodeWithVal( 	__FILE__, __LINE__,
        	   	   		        		"onlineDwlFbCodeInfoChgFuncInFbTask", status,
                	   	    			"Block Addr",fbBlkAddr,
                 	  	    			"Request FB Code ", fbId,
                  		    			"Read FB Code", fbMemInfo.fbId 
				 	  				);
			}
		}	
	} 
	
	
	if(status != NO_ERROR)
		goto MODULE_END;
	
	if(sumOfFbCodeNum != totalFbCodeNum) {
		status = ONLINE_FB_CODE_NUM_MISMATCH_ERR;
		setErrorCodeWithVal (	__FILE__,__LINE__, "onlineDwlFbCodeInfoChgFuncInFbTask", status,
								"Total FB Code NUM",totalFbCodeNum,
								"Sum Of Fb Code NUM",sumOfFbCodeNum,
								"NOT USING", 0
							   );
		goto MODULE_END;  
	}
		
	onlineSegFbInfoChgInfoPtr = onlineFbInfoChgMngrInfoPtr->fbInfoChgSegInfoPtr;
	
	for(segFbInfoChgLoop = 0; segFbInfoChgLoop < numOfSegment; segFbInfoChgLoop++, onlineSegFbInfoChgInfoPtr++){
		 
		status = segTblPointerGet( LOGIC_ID,
           						onlineSegFbInfoChgInfoPtr->taskId,
								(strFbTaskTblMemInfo	**) &segTblInfoPtr
								);
		if(status == NO_ERROR) {
			segTblInfoPtr->loopStatus = 0;
		}

		numOfFbInfoChgFbCode	= onlineSegFbInfoChgInfoPtr->numOfFbCode;
		onlineFbInfoChgInfoPtr = &onlineSegFbInfoChgInfoPtr->fbInfoChgFbCodeInfo[0];
		
		for(fbInfoChgLoop = 0; fbInfoChgLoop < numOfFbInfoChgFbCode; fbInfoChgLoop++,onlineFbInfoChgInfoPtr++){
			
			(void)onlineDwlFbOutputActionChgInLocalAndBusMemory(	onlineFbInfoChgInfoPtr->fbId,
																onlineFbInfoChgInfoPtr->fbBlkAddr,
																&onlineFbInfoChgInfoPtr->outputAction[0]
															  );
			
		}	
		
	} 

MODULE_END:

	return(status);
}



static uint32 onlineDwlFbOutputActionChgInLocalAndBusMemory(	uint32 fbId, 
															uint32 fbBlkAddr,
															uint32 *outputAction
														)
{
	uint32 status = NO_ERROR;
	strFbMemInfo	*fbMemInfoPtr;	
	
	
	fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (fbBlkAddr-1);
	fbMemInfoPtr->outputAction[0] = *outputAction;
	fbMemInfoPtr->outputAction[1] = *(outputAction+1);
	
	fbMemInfoPtr = gMpuFbMemInfoBusPtr + (fbBlkAddr-1);
	memoryCopy((uint8*)&fbMemInfoPtr->outputAction[0],
			  (uint8*)outputAction,LONG_SIZE);
	
	
	return(status);
}

uint32 onlineDownloadVarInit()
{
	uint32 status = NO_ERROR;

	sOnlineDwlFbAddInfo.addSegInfoPtr = 
			(strOnlineDwlAddSegInfo *)malloc(sizeof(strOnlineDwlAddSegInfo) * MAX_ONLINE_DWL_SEG_ADD_NUM);

	sOnlineDwlFbDelInfo.delSegInfoPtr = 
			(strOnlineDwlDelSegInfo *)malloc(sizeof(strOnlineDwlDelSegInfo) * MAX_ONLINE_DWL_SEG_DEL_NUM);

	sOnlineDwlFbSpecChgInfo.specChgSegInfoPtr = 
			(strOnlineDwlSpecChgSegInfo *)malloc(sizeof(strOnlineDwlSpecChgSegInfo) * MAX_ONLINE_DWL_SEG_SPEC_CHANGE_NUM);

	sOnlineDwlFbInfoChgInfo.fbInfoChgSegInfoPtr = 
			(strOnlineDwlFbInfoChgSegInfo *)malloc(sizeof(strOnlineDwlFbInfoChgSegInfo) * MAX_ONLINE_DWL_SEG_FBINFO_CHANGE_NUM);

	sOnlineDwlTrsChgInfo.chgTrsInfoPtr = 
			(strOnlineDwlChgTrsSegInfo *)malloc(sizeof(strOnlineDwlChgTrsSegInfo) * MAX_ONLINE_DWL_SEG_TRS_CHANGE_NUM);

	return(status);
}


static uint32 onlineDwlIOCodeSpecChgInLocalAndBusMemory(	uint32 fbId, 
															uint32 fbBlkAddr,
															strOnlineDwlChgSpecInfo	*specInfoPtr
														)
{
	uint32 status = NO_ERROR;
	uint32	chgSpecIndex, *specStartPtr;		
	strFbMemInfo	*fbMemInfoPtr;	
	
	
	
	fbMemInfoPtr = gDiuFbMemInfoLocalPtr + (fbBlkAddr-1);

	

	chgSpecIndex = fbMemInfoPtr->specIndex + specInfoPtr->specIndex - 1;
	specStartPtr = (uint32 *)gDiuSpecAndVarDataLocalPtr + chgSpecIndex;
	*specStartPtr = specInfoPtr->specData[0];

	if(GET_TYPE_SIZE(specInfoPtr->specType) == SIZE64_TYPE) {
		 
		 *(++specStartPtr) = specInfoPtr->specData[1];
	}
	
	fbMemInfoPtr = gDiuFbMemInfoBusPtr + (fbBlkAddr-1);

	

	chgSpecIndex = fbMemInfoPtr->specIndex + specInfoPtr->specIndex - 1;
	specStartPtr = (uint32 *)gDiuSpecAndVarDataBusPtr + chgSpecIndex;
	*specStartPtr = specInfoPtr->specData[0];

	if(GET_TYPE_SIZE(specInfoPtr->specType) == SIZE64_TYPE) {
		 
		 *(++specStartPtr) = specInfoPtr->specData[1];
	}

	return(status);
}


void onlineDwlVarChangeInfoDisplay(void)
{
	strOnlineDwlVarInfoChgMngrInfo	*onlineVarChgMemMngrInfoPtr = &sOnlineDwlVarInfoChgInfo;
	strOnlineDwlVarRetainInfo		*onlineVarRetainInfoPtr = onlineVarChgMemMngrInfoPtr->chgVarRetainInfo;
	strOnlineDwlVarInitInfo 		*onlineVarInitInfoPtr = onlineVarChgMemMngrInfoPtr->chgVarInitInfo;
	uint32	varLoop;
	
	printf( "************************************************************\n");	
	printf( "**		Var Retain/Init Value Info Display\n");
	printf( "************************************************************\n");
	
	for(varLoop = 0; varLoop < onlineVarChgMemMngrInfoPtr->dwlnumOfTotalRetainNum; varLoop++,onlineVarRetainInfoPtr++) {
		printf( "Num OF Retain Info : %d\n",onlineVarChgMemMngrInfoPtr->dwlnumOfTotalRetainNum);
		printf("\n Mem Kind[%d] - Retain Data[%d], Reatain Index[%d]",	
			onlineVarRetainInfoPtr->memKind, 
			onlineVarRetainInfoPtr->retainData, 
			onlineVarRetainInfoPtr->varRetainIndex);
		printf( "\n");		
	} 	 	
	for(varLoop = 0; varLoop < onlineVarChgMemMngrInfoPtr->dwlnumOfTotalInitNum; varLoop++,onlineVarInitInfoPtr++) {
		printf( "Num OF Var Init Info : %d\n",onlineVarChgMemMngrInfoPtr->dwlnumOfTotalInitNum);
		printf("\n Mem Kind[%d] - Var Data[%d], VarIndex[%d], Var Size[%d]",	
			onlineVarInitInfoPtr->varInitType, onlineVarInitInfoPtr->varInitData[0], 
			onlineVarInitInfoPtr->varInitIndex,onlineVarInitInfoPtr->intlVarSize);
		printf( "\n");		
	}
}
