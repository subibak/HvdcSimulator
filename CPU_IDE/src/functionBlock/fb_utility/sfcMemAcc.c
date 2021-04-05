
#include <stdio.h>
#include <stdlib.h>

#include 	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include 	"usrQueData.h"
#include 	"fbMem.h"
#include 	"sfcMem.h"

#include 	"pcsDefs.h"
#include 	"usrSema.h"
#include 	"usrErrCode.h"
#include 	"sysConfig.h"

#include 	"sysDual.h"
#include 	"systemDiag.h"

#include 	"netProtocol.h"

#include	"segScheduler.h"


#include	"protocolBuf.h"

#include 	"osDepend.h"
#include 	"segScheduler.h"
#include 	"usrDefBuf.h"


static strTrsExecQueInfo	sTrsExecQMngrInfo;
static strFbExecutionInfo	sTrsExecDataInfo[MAX_FB_EXEC_SEQ_ID_NUM];
static strActExecQueInfo	sActExecQMngrInfo;
static strFbExecutionInfo	sActExecDataInfo[MAX_FB_EXEC_SEQ_ID_NUM];

static strTrsLinkQueInfo 	sTrsLinkQMngrInfo;
static strTrsLinkInfo		sTrsLinkDataInfo[MAX_TRANSITOIN_NUM];

static strActionLinkQueInfo	sActLinkQMngrInfo;
static strActionLinkInfo	sActLinkDataInfo[MAX_ACTION_NUM];
static strActionLinkQueInfo	sActCurQMngrInfo;
static strActionLinkInfo	sActCurDataInfo[MAX_ACTION_NUM];

static strStepLinkQueInfo   sStepLinkQMngrInfo;
static strStepLinkInfo		sStepLinkDataInfo[MAX_SFC_EXEC_NUM];	

static strSfcSequencerQueInfo sSfcSequencerQMngrInfo;
static strSfcSequcerInfo	sSfcSequencerDataInfo[MAX_VERTEX_NUM];  

static strSfcAdjQueInfo		sSfcAdjacentQMngrInfo;
static strSfcAdjacentInfo	sSfcAdjacentDataInfo[MAX_SFC_EXEC_NUM*2]; 

strSfcExecQueInfo    sSfcExecQMngrInfo;
strSfcExecutionInfo	sSfcExecDataInfo[MAX_VERTEX_NUM];			    

strActionCtrlInfo    sSfcActionFlagInfo[MAX_ACTION_NUM];
strActCurDebugInfo   sActCurDbgData;




static uint32	sPrevSfcExecId = 0;

char* strType[4] = {"NONE", "TR" ,"AC", "ST"};
char* strActFlag[11] = { "NULL","NONE","N","R","S","L","D","P","SD","DS","SL"};
#define	SFC_SEQ_SEMA_GET_TIME_OUT	(systemClockRateGet()/10)	


extern strMpuDiuRunningStatusInfo 	*gDiuRasInfoPtr;
extern strSysRunningConfigInfo		*gSysRunningInfoPtr;

extern strStepInfo *gSFCStepInfoLocalPtr, *gSFCStepInfoBusPtr;
extern strTransitionInfo *gSFCTransitionLocalPtr, *gSFCTransitionBusPtr;
extern strActionInfo *gSFCActionInfoLocalPtr, *gSFCActionInfoBusPtr;

extern int32 gFbExecSeqIdQSemaId,gSFCSequncerQSemaId;
extern int32 gDebugPortFd;

extern strFbExecutionInfo* gFBExeSequenceQueuePtr[MAX_FB_EXEC_SEQ_ID_NUM];

extern strExecTimeInfo gTrsExecTime[MAX_TRANSITOIN_NUM];
extern strExecTimeInfo gActExecTime[MAX_ACTION_NUM];


uint32 sfcMemoryClear(uint32, uint32);

uint32	sfcExecIdQueInit(void);
uint32	sfcExecIdInfoClear(uint32,uint32);
uint32	sfcExecIdInfoSet(uint32,uint32);


static uint32  transitionExecIdQueInit(void);
static uint32  trsExecIdInfoClear(uint32);
static uint32  trsExecIdInfoSet(uint32);
uint32 trsExecIdInfoDelete(uint32, uint32, uint32);


static uint32  actionExecIdQueInit(void);
static uint32  actExecIdInfoClear(uint32);
static uint32  actExecIdInfoSet(uint32);
uint32 actExecIdInfoDelete(uint32, uint32, uint32);


static uint32  transitionLinkQueInit(void);
static uint32  trsLinkInfoClear(uint32);
static uint32  trsLinkInfoSet(uint32);


static uint32  actionLinkQueInit(void);
static uint32  actLinkInfoClear(uint32);
static uint32  actLinkInfoSet(uint32);

static uint32  actionCurQueInit(void);
static uint32  actCurInfoSet(uint32 , uint32 );
static uint32  actCurInfoClear(uint32);
uint32 actionCurInfoUnlink(uint32 , strActionLinkInfo * );


static uint32  stepLinkActQueInit(void);
static uint32  stepLinkActInfoClear(uint32);
static uint32  stepLinkActSet(uint32);


static uint32  sfcAdjacentQueInit(void);
static uint32  sfcAdjacentInfoClear(uint32);
static uint32  sfcAdjacentInfoSet(uint32);


static uint32  sfcSequncerQueInit(void);
static uint32  sfcSequencerInfoClear(uint32);
static uint32  sfcSequencerInfoSet(uint32);


static uint32  sfcExecutionInfoClear(uint32);
static uint32  sfcExecutionQueInit(void);



strFbExecutionInfo	*trsExecIdNextMemInfoGet(uint32 , strFbExecutionInfo *);
strFbExecutionInfo	*trsExecIdStartMemInfoGet(uint32);
strTrsLinkInfo		*trsLinkNextMemInfoGet(uint32, strTrsLinkInfo *);
strTrsLinkInfo		*trsLinkStartMemInfoGet(uint32);

strFbExecutionInfo	*actExecIdNextMemInfoGet(uint32 , strFbExecutionInfo *);
strFbExecutionInfo	*actExecIdStartMemInfoGet(uint32);
strActionLinkInfo	*actLinkNextMemInfoGet(uint32 , strActionLinkInfo *);
strActionLinkInfo	*actLinkStartMemInfoGet(uint32);
strActionLinkInfo	*actCurNextMemInfoGet(uint32 , strActionLinkInfo *);
strActionLinkInfo	*actCurStartMemInfoGet(uint32);

strStepLinkInfo		*stepLinkNextMemInfoGet(uint32, strStepLinkInfo *);
strStepLinkInfo		*stepLinkStartMemInfoGet(uint32);
										 
strSfcSequcerInfo   *sfcSequencerStartMemInfoGet(uint32 );
strSfcSequcerInfo   *sfcSequencerNextMemInfoGet(uint32, strSfcSequcerInfo *);

strSfcExecutionInfo *sfcCurrencExecIdStartMemInfoGet(uint32 );
strSfcExecutionInfo *sfcCurrentExecIdNextMemInfoGet(uint32, strSfcExecutionInfo *);
strSfcExecutionInfo* sfcExecCurrentInfoGet(uint32);
uint32 sfcExecCurrentInfoPut(uint32, strSfcSequcerInfo *);
uint32 sfcExecCurrentInfoUnlink(uint32, strSfcSequcerInfo *);


uint32 sfcExecNextToCurrentInfoSet(uint32);
uint32 sfcStepOnRunFunc(strSfcSequcerInfo *, uint32);
uint32 sfcStep0ffRunFunc(strSfcSequcerInfo *);
uint32 sfcActionCtrlRunning(uint32 );

uint32 sfcStepCurretStatusGet(strSfcSequcerInfo *,uint32 *);
uint32 sfcStepActiveStatusGet(strSfcSequcerInfo *,uint32 *);
uint32 sfcStepCurretStatusSet(strSfcSequcerInfo *, uint32 );

strSfcSequcerInfo *getSequencerId(uint32,  uint32 , uint32);
uint32 sfcSequencerCheck(uint32);
uint32 trsActiveStatusRead(uint32, uint32*);
uint32 actActiveStatusRead(uint32, uint32*);

uint32 sfcPrevStepCXAllOff(strSfcAdjacentInfo*,uint32);
strSfcSequcerInfo* sfcNextTransitionCheck(strSfcAdjacentInfo*, uint32* );
uint32 sfcCurExecInfoSet(uint32);


void sfcDataDisplay();
uint32	sfcMemPointerGet(uint32, strSfcMemPointerInfo *);
static void sfcRunningStepInfoDisplay(uint32);
static void sfcRunningSequencerInfoDisplay(uint32);
static void sfcSequencerLinkQueueDisplay(uint32);
static void sfcActionInfoDisplay(uint32);
static void sfcTransitionInfoDisplay(uint32);
static void sfcStepInfoDisplay(uint32);
static void sfcFbCodeInMemDisplay();

static void sfcRunngingMemoryDisplay();
static void sfcRunningStepMemDisplay(void);
static void sfcRunningTrsMemDisplay(void);
static void sfcRunningActMemDisplay(void);

uint32 sfcStepInfoRead(uint32, strStepInfo*);
uint32 sfcTrsInfoRead(uint32, strTransitionInfo*);
uint32 sfcActInfoRead(uint32, strActionInfo*);
void trsExecIdQueCountInfoDisplay(void);
void actExecIdQueCountInfoDisplay(void);

static void sfcDataInMemoryDisplay();
static void sfcActionMemoryDisplay(void);
static void sfcTransitionMemoryDisplay(void);
static void sfcStepMemoryDisplay(void);

uint32 sfcLoopStatusWrite(uint32,uint32,uint32,uint32); 


void sfcQueueCountDisplay(void);

extern int32  nodePutOnFirst(   	strDoubleLink **, strDoubleLink *);
extern uint32 nodePutOnLast(strDoubleLink **, strDoubleLink *);
extern int32  nodePutOnPrev(strDoubleLink **,strDoubleLink *, strDoubleLink *);
extern strDoubleLink *nodeGetAndUnlink(strDoubleLink **);
extern strDoubleLink *nextNodeRef(strDoubleLink *,strDoubleLink *);
extern int32  nodeUnlink( strDoubleLink **,strDoubleLink *);

extern uint32 semaphoreGet(int32, int32);
#ifdef VXWORKS
extern int32  semaphoreRelease(int32);
#else
extern int32  semaphoreRelease(int32, int32);
extern int32	semaphoreRelease2(uint8 *, uint32, int32, int32);
#endif

extern uint32 getFbTaskTblInfo(uint32 , uint32 , strFbTaskTblMemInfo *);
extern uint32 segTblPointerGet(uint32 ,uint32	,strFbTaskTblMemInfo	**);

extern uint32 fbSysExeModuleInfoWrite( uint32, strModuleExecInfo* );
extern uint32 fbCodeMemInfoRead(uint32, uint32, strFbMemInfo *);
extern uint32 fbSysExeModuleInfoRead( uint32, strModuleExecInfo* );
extern uint32 systemCnfgInfoRead (strSysConfigInfo *);
extern uint32 rmtBusCopyCheck(void);

extern void   setErrorCode(uint8 *, uint32,uint8 *,uint32);
extern void	  setErrorCodeWithVal(uint8 *,uint32,uint8 *,uint32, uint8 *,uint32,
							uint8 *,uint32,uint8 *,uint32);
extern int32  systemClockRateGet();
extern int32  ioRead(int32 , int8 *, int32 );

extern void	  memoryClear ( uint8 *, uint32 );
extern void	  memoryCopy (uint8 *, uint8 *, uint32);
extern void	  screenClear(void);
extern void   spaceSkip(int8 **);
extern int32  keyHit(void); 
extern void	  cursorPositionSet(void);  


extern uint32  refDataCopyToMem(uint32*, uint32, strFbSpecActionInfo, uint32);
extern uint32  refDataCopyToBusMem(uint32*, uint32, strFbSpecActionInfo, uint32);
extern uint32  refDataCopyToRmtMem(uint32*, uint32, strFbSpecActionInfo, uint32);

extern uint32  fbSysExeSfcInfoRead(uint32,uint32*);
extern void	   errorStringDisplay(uint8 *,uint32,uint8 *,uint32);
extern void		sysTaskDelay (int32 );
extern uint32	myBusDataAcc(uint8 *, uint32, uint32	, uint32 ,uint8 *, uint8 *);
extern uint32	dualFlagCheckInRunShelf(void);
extern uint32	busSegTblPointerGet(uint32 ,uint32	,strFbTaskTblMemInfo	**);
extern void		lineClear();
extern uint32 segmentStatusRead(uint32, uint32, strSegStatusInfo *);


uint32	sfcExecIdQueInit(void)
{
	uint32	status = NO_ERROR;

	
    status =  transitionExecIdQueInit();
	if(status == NO_ERROR) 
		transitionLinkQueInit();

	
	if(status == NO_ERROR)	
		status = actionExecIdQueInit();
	if(status == NO_ERROR)	
		status = actionLinkQueInit();
	if(status == NO_ERROR)	
		status = actionCurQueInit();


	
	if(status == NO_ERROR) 	
		status = stepLinkActQueInit();

	if(status == NO_ERROR)
	{
		
		status =sfcSequncerQueInit();
		if(status == NO_ERROR){
			
		   status = sfcAdjacentQueInit();
		}
	}

	
	if(status == NO_ERROR) status =	sfcExecutionQueInit();

	return(status);
}


uint32	sfcExecIdInfoSet(uint32  taskId, uint32 moduleId)
{
	uint32	status = NO_ERROR;
	strFbTaskTblMemInfo *segTblInfoPtr;

	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
		status = L_SEG_ID_ERR;
        setErrorCode( __FILE__, __LINE__, "sfcExecIdInfoSet",status);
		return(status);
	}

	
	if( moduleId != LOGIC_ID){
		status = L_MODULE_ID_ERR;
        setErrorCode( __FILE__, __LINE__, "sfcExecIdInfoSet",status);
		return(status);
	}	

	status = segTblPointerGet(LOGIC_ID, taskId,
								(strFbTaskTblMemInfo	**) &segTblInfoPtr);

	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "sfcExecIdInfoSet",status);
		return(status);		
	}

	 
	if( (segTblInfoPtr->logicType != SFC_LOGIC_TYPE)|| (segTblInfoPtr->taskId != taskId) ) 
		 return(status);
	else{
		segTblInfoPtr->loopStatus = 0;
	}

	
	status = trsLinkInfoSet(taskId);		
	if(status != NO_ERROR) {
		printf( "[ERROR] - trsLinkInfoSet() Call Error In #%d SegId ID\n",taskId);
		return(status); 	
	}	

	
	status = actLinkInfoSet(taskId);		
	if(status != NO_ERROR) {
		printf( "[ERROR] - actLinkInfoSet() Call Error In #%d SegId ID\n",taskId);
		return(status); 	
	}	

	
	
	status = sfcSequencerInfoSet(taskId);  
	if(status != NO_ERROR) {
		printf( "[ERROR] - sfcSequencerInfoSet() Call Error In #%d SegId\n", taskId);
		return(status); 	
	}	
	else{
		
		
		status = sfcAdjacentInfoSet(taskId);
		if(status != NO_ERROR) {
			printf( "[ERROR] - sfcAdjacentInfoSet() Call Error In #%d SegId\n", taskId);
			return(status); 	
		}	
	}

	return(status);
}


uint32	sfcExecIdInfoClear(uint32  taskId, uint32 moduleId)
{
	uint32	status = NO_ERROR;

	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
		status = L_SEG_ID_ERR;
        setErrorCode( __FILE__, __LINE__, "sfcExecIdInfoSet",status);
		return(status);
	}

	
	if( moduleId != LOGIC_ID){
		status = L_MODULE_ID_ERR;
        setErrorCode( __FILE__, __LINE__, "sfcExecIdInfoSet",status);
		return(status);
	}	
	
	
	status = trsLinkInfoClear(taskId);
	if(status != NO_ERROR) {
		printf( "[ERROR] - trsLinkInfoClear() Call Error In #%d SegId ID\n", taskId);
		return(status); 	
	}	

	
	status = actLinkInfoClear(taskId);
	if(status != NO_ERROR) {
		printf( "[ERROR] - actLinkInfoClear() Call Error In #%d SegId ID\n", taskId);
		return(status); 	
	}	
	status = actCurInfoClear(taskId);
	if(status != NO_ERROR) {
		printf( "[ERROR] - actLinkInfoClear() Call Error In #%d SegId ID\n", taskId);
		return(status); 	
	}	
	
	
	
	status = sfcAdjacentInfoClear(taskId);
	if(status != NO_ERROR) {
		printf( "[ERROR] - sfcAdjacentInfoClear() Call Error In #%d SegId ID\n", taskId);
		return(status); 	
	}	

	status = sfcSequencerInfoClear(taskId); 
	if(status != NO_ERROR) {
		printf( "[ERROR] - sfcSequencerInfoClear() Call Error In #%d SegId ID\n", taskId);
		return(status); 	
	}	

	return(status);
}


uint32	sfcRuntimeInfoClear(void)
{
	uint32	status = NO_ERROR;
	uint32  actId, stpId;
	strActionInfo *actInfoPtr;
	strStepInfo *stepInfoPtr;
	strSfcMemPointerInfo sfcMemPtr,sfcBusMemPtr;

	extern strMpuDiuRunningStatusInfo 	*gDiuRasInfoPtr;

	
	if(  (gDiuRasInfoPtr->redundancyStatus != SYS_REDUNDANCY_ERR) &&
		     (gSysRunningInfoPtr->sysOperationMode == SYS_RAS_SYSTEM_AUTO_MODE) 
	  )
		return(status);

	status=sfcMemPointerGet(LOCAL_MEM_ACC, &sfcMemPtr);
	if(status != NO_ERROR){
			setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
							(uint8 *)"sfcRuntimeInfoClear",status
			  		  		);
			return(status);
	}

	status=sfcMemPointerGet(BUS_MEM_ACC, &sfcBusMemPtr);
	if(status != NO_ERROR){
			setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
							(uint8 *)"sfcRuntimeInfoClear",status
			  		  		);
			return(status);
	}

	
	for(actId = 1; actId <= MAX_ACTION_NUM; actId++) {
		
		actInfoPtr = (strActionInfo *)sfcMemPtr.actMemPtr;
		actInfoPtr += (actId -1);

		if(actInfoPtr->actionID != actId) continue;

		actInfoPtr->elapsedTime = 0;
		actInfoPtr->setCurTime = 0;
		actInfoPtr->actCtrlData =0;

		
		actInfoPtr = (strActionInfo *)sfcBusMemPtr.actMemPtr;
		actInfoPtr += (actId -1);
		actInfoPtr->elapsedTime = 0;
		actInfoPtr->setCurTime = 0;
		actInfoPtr->actCtrlData =0;
	}

	
	for(stpId = 1; stpId <= MAX_STEP_NUM; stpId++) {
		
		stepInfoPtr = (strStepInfo *)sfcMemPtr.stepMemPtr;
		stepInfoPtr += (stpId -1);

		if(stepInfoPtr->stepID != stpId) continue;

		stepInfoPtr->T =0;
		if(stepInfoPtr->initStep == START_STEP) stepInfoPtr->CX = ACTIVE_FLAG;
		else stepInfoPtr->CX =0;
		stepInfoPtr->X=0;

		
		stepInfoPtr = (strStepInfo *)sfcBusMemPtr.stepMemPtr;
		stepInfoPtr += (stpId -1);
		stepInfoPtr->T =0;
		if(stepInfoPtr->initStep == START_STEP) stepInfoPtr->CX = ACTIVE_FLAG;
		else stepInfoPtr->CX =0;
		stepInfoPtr->X=0;
	}

	return(status);
}


static uint32 sfcSequencerInfoSet(uint32 taskId)
{
	uint32	status = NO_ERROR;
	uint32  stpId, trsId, initStepNum = 0, linkTrsNum = 0;

	strFbTaskTblMemInfo segTblInfo;

	strStepInfo *stepInfoPtr;
	strTransitionInfo *trsInfoPtr;
	strSfcSequcerInfo *curDataPtr;


	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
			status = L_SEG_ID_ERR;
			setErrorCode( __FILE__, __LINE__, "sfcSequencerInfoSet",status);
			return(status);
	}

	status = getFbTaskTblInfo	( 
								LOGIC_ID,
           						taskId,
								(strFbTaskTblMemInfo	*) &segTblInfo
							);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "sfcSequencerInfoSet",status);
		return(status);		
	}

	
	if(segTblInfo.logicType != SFC_LOGIC_TYPE) return(status);

	
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,
						__LINE__,
						"trsExecIdInfoSet",
						status
					  );
		return(status);
	}

	
	for( stpId = 1; stpId <= MAX_STEP_NUM; stpId++) 
	{
		stepInfoPtr = gSFCStepInfoLocalPtr;
		stepInfoPtr += (stpId -1);

		if(stepInfoPtr->stepID == 0) continue;
		else if(stepInfoPtr->taskID != taskId) continue;

	    curDataPtr = (strSfcSequcerInfo *)nodeGetAndUnlink (
                            	(strDoubleLink **)&sSfcSequencerQMngrInfo.freeList
            	                                    	);

		if(curDataPtr == NULL) {
			status = NO_EXIST_NODE_ERR;
        	setErrorCode( __FILE__, __LINE__, "sfcSequencerInfoSet",status);
			goto MODULE_END;	
		}
		else {
			curDataPtr->taskId = stepInfoPtr->taskID;
			curDataPtr->stepTrId = stepInfoPtr->stepID;
			curDataPtr->nodeType = STEP_TYPE;
			curDataPtr->nextNodeNum = stepInfoPtr->nextTrNum;
			curDataPtr->prevNodeNum = stepInfoPtr->preTrNum;
			curDataPtr->visitFlag = INACTIVE_FLAG;
	
			
			if(stepInfoPtr->initStep == START_STEP) {
				initStepNum++;
				curDataPtr->initStep = START_STEP;
		   		status = nodePutOnFirst( (strDoubleLink **) &sSfcSequencerQMngrInfo.usedList[taskId - 1],
									(strDoubleLink *)curDataPtr
				 		       	  );
			}
			else{
	    		status = nodePutOnLast( (strDoubleLink **) &sSfcSequencerQMngrInfo.usedList[taskId - 1],
             		            	(strDoubleLink *)curDataPtr
                		       	  );
			}
			if(status) {
		        setErrorCode( __FILE__, __LINE__, "sfcSequencerInfoSet",status);
				goto MODULE_END;	
			}

		}

		
		status = stepLinkActSet(stpId);
		if(status != NO_ERROR) {
			printf( "[ERROR] - stepLinkActSet() Call Error In #%d Step ID\n",stpId);
			return(status); 	
		}	
	}

	if( initStepNum != 1 ){
		status = SFC_INIT_STEP_ERR;
	    setErrorCodeWithVal( __FILE__, __LINE__, 
							  "sfcSequencerInfoSet",status,
							  "Init STEP Num", initStepNum,
							  "TASK ID", taskId,
							  "Not Using", 0 );

		return(status); 	
	}
	
	
	for( trsId = 1; trsId <= MAX_TRANSITOIN_NUM; trsId++) 
	{
		trsInfoPtr = gSFCTransitionLocalPtr;
		trsInfoPtr += (trsId -1);

		if(trsInfoPtr->transitionID == 0) continue;
		else if(trsInfoPtr->taskID != taskId) continue;

	    curDataPtr = (strSfcSequcerInfo *)nodeGetAndUnlink (
                            	(strDoubleLink **)&sSfcSequencerQMngrInfo.freeList
            	                                    	);

		if(curDataPtr == NULL) {
			status = NO_EXIST_NODE_ERR;
        	setErrorCode( __FILE__, __LINE__, "sfcSequencerInfoSet",status);
			goto MODULE_END;	
		}
		else {
			curDataPtr->taskId = trsInfoPtr->taskID;
			curDataPtr->stepTrId = trsInfoPtr->transitionID;
			curDataPtr->nodeType = TRANSITION_TYPE;
			curDataPtr->nextNodeNum = trsInfoPtr->NextStepNum;
			curDataPtr->prevNodeNum = trsInfoPtr->preStepNum;
			curDataPtr->visitFlag = INACTIVE_FLAG;

			linkTrsNum++;						
    		status = nodePutOnLast( (strDoubleLink **) &sSfcSequencerQMngrInfo.usedList[taskId - 1],
             		            	(strDoubleLink *)curDataPtr
                		       	  );
			if(status) {
		        setErrorCode( __FILE__, __LINE__, "sfcSequencerInfoSet",status);
				goto MODULE_END;	
			}

		}
	}

	if( linkTrsNum == 0 ){
		status = SFC_LINK_TRANSITION_ERR;
	    setErrorCodeWithVal( __FILE__, __LINE__, 
							  "sfcSequencerInfoSet",status,
							  "Iink Trs Num", linkTrsNum,
							  "TASK ID", taskId,
							  "Not Using", 0 );

		return(status); 	
	}

MODULE_END:
	
#ifdef VXWORKS
	semaphoreRelease (gSFCSequncerQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif
	return(status);
}


static uint32 sfcAdjacentInfoSet(uint32 taskId)
{
	uint32	status = NO_ERROR;
	uint32  i, vertexNum;

	strFbTaskTblMemInfo segTblInfo;

	strStepInfo *stepInfoPtr;
	strTransitionInfo *trsInfoPtr;

	strSfcSequcerInfo   *sfcExecIdInfoPtr;
	strSfcAdjacentInfo	*curLinkPtr;


	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
			status = L_SEG_ID_ERR;
			setErrorCode( __FILE__, __LINE__, "sfcAdjacentInfoSet",status);
			return(status);
	}

	status = getFbTaskTblInfo	( 
								LOGIC_ID,taskId,
								(strFbTaskTblMemInfo	*) &segTblInfo
							);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "sfcAdjacentInfoSet",status);
		return(status);		
	}

	
	if(segTblInfo.logicType != SFC_LOGIC_TYPE) return(status);

	
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,__LINE__,
						"sfcAdjacentInfoSet",status
					  );
		return(status);
	}


	
	sfcExecIdInfoPtr = sfcSequencerStartMemInfoGet(taskId);
	while(sfcExecIdInfoPtr!=NULL) 
	{
		if(sfcExecIdInfoPtr->nodeType == STEP_TYPE)
		{
			stepInfoPtr = gSFCStepInfoLocalPtr;
			stepInfoPtr += (sfcExecIdInfoPtr->stepTrId -1);

			vertexNum = (sfcExecIdInfoPtr->stepTrId -1);

			for(i=0;i<sfcExecIdInfoPtr->nextNodeNum;i++) 
			{
				curLinkPtr = (strSfcAdjacentInfo *)nodeGetAndUnlink (
								(strDoubleLink **)&sSfcAdjacentQMngrInfo.freeList);
				if(curLinkPtr != NULL) {
					
					curLinkPtr->sequencerId = getSequencerId( stepInfoPtr->nextTrList[i], taskId, TRANSITION_TYPE)  ;
					curLinkPtr->nodeType = TRANSITION_TYPE;
					
					status = nodePutOnLast( (strDoubleLink **) &sSfcAdjacentQMngrInfo.nextList[vertexNum],
             								(strDoubleLink *)curLinkPtr
                						  );
					if(status) {
						setErrorCode( __FILE__, __LINE__, "sfcSequencerInfoSet",status);
						goto MODULE_END;	
					}
					sfcExecIdInfoPtr->nextDblink = (strDoubleLink *)sSfcAdjacentQMngrInfo.nextList[vertexNum];
				}

			}

			for(i=0;i<sfcExecIdInfoPtr->prevNodeNum;i++) 
			{
				curLinkPtr = (strSfcAdjacentInfo *)nodeGetAndUnlink (
								(strDoubleLink **)&sSfcAdjacentQMngrInfo.freeList);
				if(curLinkPtr != NULL) {
					
					curLinkPtr->sequencerId = getSequencerId( stepInfoPtr->preTrList[i], taskId, TRANSITION_TYPE)  ;
					curLinkPtr->nodeType = TRANSITION_TYPE;
					
					status = nodePutOnLast( (strDoubleLink **) &sSfcAdjacentQMngrInfo.prevList[vertexNum],
             								(strDoubleLink *)curLinkPtr
                						  );
					if(status) {
						setErrorCode( __FILE__, __LINE__, "sfcSequencerInfoSet",status);
						goto MODULE_END;	
					}
					sfcExecIdInfoPtr->prevDblink = (strDoubleLink *)sSfcAdjacentQMngrInfo.prevList[vertexNum];
				}

			}
		}
		else if(sfcExecIdInfoPtr->nodeType == TRANSITION_TYPE)
		{
			trsInfoPtr = gSFCTransitionLocalPtr;
			trsInfoPtr += (sfcExecIdInfoPtr->stepTrId -1);
			
			vertexNum = MAX_STEP_NUM + (sfcExecIdInfoPtr->stepTrId -1);

			for(i=0;i<sfcExecIdInfoPtr->nextNodeNum;i++) 
			{
				curLinkPtr = (strSfcAdjacentInfo *)nodeGetAndUnlink (
								(strDoubleLink **)&sSfcAdjacentQMngrInfo.freeList);
				if(curLinkPtr != NULL) {
					
					curLinkPtr->sequencerId = getSequencerId( trsInfoPtr->nextStepList[i], taskId, STEP_TYPE)  ;
					curLinkPtr->nodeType = STEP_TYPE;
					
					status = nodePutOnLast( (strDoubleLink **) &sSfcAdjacentQMngrInfo.nextList[vertexNum],
             								(strDoubleLink *)curLinkPtr
                						  );
					if(status) {
						setErrorCode( __FILE__, __LINE__, "sfcSequencerInfoSet",status);
						goto MODULE_END;	
					}
					sfcExecIdInfoPtr->nextDblink = (strDoubleLink *)sSfcAdjacentQMngrInfo.nextList[vertexNum];
				}
			}
			
			for(i=0;i<sfcExecIdInfoPtr->prevNodeNum;i++) 
			{
				curLinkPtr = (strSfcAdjacentInfo *)nodeGetAndUnlink (
								(strDoubleLink **)&sSfcAdjacentQMngrInfo.freeList);
				if(curLinkPtr != NULL) {
					
					curLinkPtr->sequencerId = getSequencerId( trsInfoPtr->preStepList[i], taskId, STEP_TYPE)  ;
					curLinkPtr->nodeType = STEP_TYPE;
					
					status = nodePutOnLast( (strDoubleLink **) &sSfcAdjacentQMngrInfo.prevList[vertexNum],
             								(strDoubleLink *)curLinkPtr
                						  );
					if(status) {
						setErrorCode( __FILE__, __LINE__, "sfcSequencerInfoSet",status);
						goto MODULE_END;	
					}
					sfcExecIdInfoPtr->prevDblink = (strDoubleLink *)sSfcAdjacentQMngrInfo.prevList[vertexNum];
				}
			}
		}
	
		
		sfcExecIdInfoPtr = sfcSequencerNextMemInfoGet(taskId, sfcExecIdInfoPtr);
	}

	
	
	status = sfcSequencerCheck(taskId);
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,__LINE__,
						"sfcAdjacentInfoSet",status
					  );
	}
MODULE_END:
	
#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif
	return(status);
}

uint32 sfcSequencerCheck(uint32 taskId)
{
	uint32 status = NO_ERROR;
	strFbTaskTblMemInfo segTblInfo;
	strSfcSequcerInfo *sfcExecIdInfoPtr;
	strSfcExecutionInfo *curDataPtr,*curLinkPtr;
	strSfcAdjacentInfo *nodePtr, *nodeHeadPtr;
	strSfcAdjacentInfo *nextNodePtr, *nextNodeHeadPtr, *preNodePtr, *preNodeHeadPtr;
	
	uint32 vertexNum = 0, preLinkFlag, nextLinkFlag;
	uint32 sequencerCount = 0;

	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
			status = L_SEG_ID_ERR;
			setErrorCode( __FILE__, __LINE__, "sfcSequencerInfoSet",status);
			return(status);
	}

	status = getFbTaskTblInfo	( 
								LOGIC_ID,
           						taskId,
								(strFbTaskTblMemInfo	*) &segTblInfo
							);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "sfcSequencerInfoSet",status);
		return(status);		
	}

	
	if(segTblInfo.logicType != SFC_LOGIC_TYPE) return(status);

	
	vertexNum = 0;
	sfcExecIdInfoPtr = sfcSequencerStartMemInfoGet(taskId);
	while(sfcExecIdInfoPtr!=NULL) 
	{
		nodePtr = (strSfcAdjacentInfo *)sfcExecIdInfoPtr->nextDblink;
		nodeHeadPtr = nodePtr;
		while(nodePtr)
		{
			if(nodePtr->sequencerId != NULL) {
				
				
				preLinkFlag = INACTIVE_FLAG;
				preNodePtr =  (strSfcAdjacentInfo *)nodePtr->sequencerId->prevDblink;
				preNodeHeadPtr = preNodePtr;
				while(preNodePtr != NULL)
				{
					if(preNodePtr->sequencerId != NULL) {
						if(sfcExecIdInfoPtr == preNodePtr->sequencerId) {
							preLinkFlag = ACTIVE_FLAG;
							break;
						}
						preNodePtr = (strSfcAdjacentInfo *)nextNodeRef(  
							(strDoubleLink *)preNodeHeadPtr,
							(strDoubleLink *)preNodePtr);
					}
					else {
						status = SFC_SEQUENCER_LINK_ERR;
						setErrorCode( __FILE__, __LINE__, "sfcSequencerCheck",status);
						return(status);
					}
				}
				if(preLinkFlag != ACTIVE_FLAG) {
					status = SFC_SEQUENCER_LINK_ERR;
					setErrorCode( __FILE__, __LINE__, "sfcSequencerCheck",status);
					return(status);
				}
				
				nodePtr = (strSfcAdjacentInfo *)nextNodeRef(  
						(strDoubleLink *)nodeHeadPtr,
						(strDoubleLink *)nodePtr);
			}
			else {
				status = SFC_SEQUENCER_LINK_ERR;
				setErrorCode( __FILE__, __LINE__, "sfcSequencerCheck",status);
				return(status);
			}
		}

		nodePtr = (strSfcAdjacentInfo *)sfcExecIdInfoPtr->prevDblink;
		nodeHeadPtr = nodePtr;

		while(nodePtr)
		{
			if(nodePtr->sequencerId != NULL) {
				
				
				nextLinkFlag = INACTIVE_FLAG;
				nextNodePtr =  (strSfcAdjacentInfo *)nodePtr->sequencerId->nextDblink;
				nextNodeHeadPtr = nextNodePtr;
				while(nextNodePtr != NULL)
				{
					if(nextNodePtr->sequencerId != NULL) {
						if(sfcExecIdInfoPtr == nextNodePtr->sequencerId) {
							nextLinkFlag = ACTIVE_FLAG;
							break;
						}
						nextNodePtr = (strSfcAdjacentInfo *)nextNodeRef(  
							(strDoubleLink *)nextNodeHeadPtr,
							(strDoubleLink *)nextNodePtr);
					}
					else {
						status = SFC_SEQUENCER_LINK_ERR;
						setErrorCode( __FILE__, __LINE__, "sfcSequencerCheck",status);
						return(status);
					}
				}
				if(nextLinkFlag != ACTIVE_FLAG) {
					status = SFC_SEQUENCER_LINK_ERR;
					setErrorCode( __FILE__, __LINE__, "sfcSequencerCheck",status);
					return(status);
				}
				
				nodePtr = (strSfcAdjacentInfo *)nextNodeRef(  
					(strDoubleLink *)nodeHeadPtr,
					(strDoubleLink *)nodePtr);
			}
			else {
				status = SFC_SEQUENCER_LINK_ERR;
				setErrorCode( __FILE__, __LINE__, "sfcSequencerCheck",status);
				return(status);
			}
		}

		
		vertexNum++;
		sfcExecIdInfoPtr = sfcSequencerNextMemInfoGet(taskId, sfcExecIdInfoPtr);
	}

	sfcExecIdInfoPtr = sfcSequencerStartMemInfoGet(taskId);
	
	while(sfcExecIdInfoPtr!=NULL) 
	{
		
		if(sfcExecIdInfoPtr-> visitFlag == INACTIVE_FLAG)
		{
			sequencerCount++;
			curDataPtr = (strSfcExecutionInfo *)nodeGetAndUnlink (
							(strDoubleLink **)&sSfcExecQMngrInfo.freeList
            										);
			curDataPtr->sequencerId = sfcExecIdInfoPtr;
			
			status = nodePutOnLast( (strDoubleLink **) &sSfcExecQMngrInfo.curExeList[taskId - 1],
             		            		(strDoubleLink *)curDataPtr
                		       		  );
			if(status) {
				setErrorCode( __FILE__, __LINE__, "sfcSequencerCheck",status);
				return(status);	
			}
			
			sfcExecIdInfoPtr->visitFlag = ACTIVE_FLAG;					

			 
			while(sSfcExecQMngrInfo.curExeList[taskId - 1]!=NULL)
			{
				
				curLinkPtr = (strSfcExecutionInfo *)nodeGetAndUnlink
					((strDoubleLink **) &sSfcExecQMngrInfo.curExeList[taskId - 1]);
				status = nodePutOnLast( (strDoubleLink **) &sSfcExecQMngrInfo.freeList,
             		            		(strDoubleLink *)curLinkPtr
                		       		  );
				nodePtr = (strSfcAdjacentInfo *)curLinkPtr->sequencerId->nextDblink;

				
				while(nodePtr)
				{
					
					if(nodePtr->sequencerId != NULL) {
						if (nodePtr->sequencerId->visitFlag == INACTIVE_FLAG)		
						{
							
							curDataPtr = (strSfcExecutionInfo *)nodeGetAndUnlink (
										 (strDoubleLink **)&sSfcExecQMngrInfo.freeList
            							 );
							curDataPtr->sequencerId = nodePtr->sequencerId;

							nodePutOnLast( (strDoubleLink **) &sSfcExecQMngrInfo.curExeList[taskId - 1],
             		            			(strDoubleLink *)curDataPtr
                		       			  );
							nodePtr->sequencerId->visitFlag = ACTIVE_FLAG;		
						}
					}
					nodePtr = (strSfcAdjacentInfo *)nextNodeRef( 
						(strDoubleLink *)curLinkPtr->sequencerId->nextDblink,
						(strDoubleLink *)nodePtr);
				}

			}
		}
		
		sfcExecIdInfoPtr = sfcSequencerNextMemInfoGet(taskId, sfcExecIdInfoPtr);
	}

	
	if (sequencerCount != 1)  {
		status = SFC_SEQUENCER_LINK_ERR;
		setErrorCodeWithVal( __FILE__, __LINE__,
								"sfcCurExecInfoSet",status,
								"SFC COUNT", sequencerCount,
								"TASK ID",taskId,
								"NOT Using",0
							 );
	}

	
	status = sfcCurExecInfoSet(taskId);
	if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, "sfcSequencerCheck",status);
	}

	return(status);
}


uint32 sfcCurExecInfoSet(uint32 taskId)
{
	uint32 status = NO_ERROR;
	strSfcSequcerInfo*  sfcExecIdInfoPtr;
	strStepInfo*  stepInfoPtr;
	uint32 currExeStepNum =0 ;
	strFbTaskTblMemInfo segTblInfo;

	strActionLinkInfo* actLinkInfoPtr;
	strActionCtrlInfo* actCtrlPtr;
	strActionInfo *actStartInfoPtr, *actInfoPtr;
	
	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
			status = L_SEG_ID_ERR;
			setErrorCode( __FILE__, __LINE__, "sfcCurExecInfoSet",status);
			return(status);
	}

	status = getFbTaskTblInfo	( 
								LOGIC_ID,	taskId,
								(strFbTaskTblMemInfo	*) &segTblInfo
							);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "sfcCurExecInfoSet",status);
		return(status);		
	}

	
	if(segTblInfo.logicType != SFC_LOGIC_TYPE) return(status);

	
	status = sfcExecutionInfoClear(taskId);
	if(status != NO_ERROR) {
		setErrorCode( __FILE__, __LINE__, "sfcCurExecInfoSet",status);
		return(status);
	}

	sfcExecIdInfoPtr = sfcSequencerStartMemInfoGet(taskId);
	while(sfcExecIdInfoPtr!=NULL) 
	{
		
		
		
		
		if (sfcExecIdInfoPtr->nodeType == STEP_TYPE)
		{
			stepInfoPtr = gSFCStepInfoLocalPtr;
			stepInfoPtr += (sfcExecIdInfoPtr->stepTrId -1);

			if(stepInfoPtr->CX == ACTIVE_FLAG) {
				status = sfcExecCurrentInfoPut(taskId, sfcExecIdInfoPtr);
				if(status != NO_ERROR) {
					setErrorCode( __FILE__, __LINE__, "sfcCurExecInfoSet",status);
				}
				currExeStepNum++;
			}
		
		}
		sfcExecIdInfoPtr->visitFlag = INACTIVE_FLAG ;
		
		
		sfcExecIdInfoPtr = sfcSequencerNextMemInfoGet(taskId, sfcExecIdInfoPtr);
	}

	
	if( currExeStepNum <= 0 ) {
		status = SFC_SEQUENCER_LINK_ERR;
		setErrorCodeWithVal( __FILE__, __LINE__,
								"sfcCurExecInfoSet",status,
								"RUN STEP NUM",currExeStepNum,
								"TASK ID",taskId,
								"NOT Using",0
							 );

	}

	
	actStartInfoPtr = gSFCActionInfoLocalPtr;

	status = actCurInfoClear(taskId);
	if(status != NO_ERROR) {
		setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
				(uint8 *)"sfcStepOnRunFunc",status
			  	);
	}

	actLinkInfoPtr = actLinkStartMemInfoGet(taskId);
	while(actLinkInfoPtr!=NULL)
	{
		if(actLinkInfoPtr->taskId == taskId)
		{
			actInfoPtr = actStartInfoPtr + (actLinkInfoPtr->actionId-1) ;
			actCtrlPtr = (strActionCtrlInfo*)&actInfoPtr->actCtrlData;

			
			actCtrlPtr->actCX = 0;
			if(actCtrlPtr ->actCtrlQ == 1) {

				
				status = actCurInfoSet(actLinkInfoPtr->taskId,actInfoPtr->actionID);
				if(status != NO_ERROR) {
					setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
							(uint8 *)"sfcCurExecInfoSet",status
			  		  		);
				}
			}

		}
		actLinkInfoPtr = actLinkNextMemInfoGet(taskId, actLinkInfoPtr);
	}	

	return(status);
}


strSfcSequcerInfo *getSequencerId(uint32 StepTrId,  uint32 taskId, uint32 nodeType)
{
	strSfcSequcerInfo *curSequencerPtr;

	curSequencerPtr = sfcSequencerStartMemInfoGet(taskId);
	while(curSequencerPtr!=NULL)
	{
		if( (curSequencerPtr->stepTrId == StepTrId)&&
				(curSequencerPtr->nodeType == nodeType) ) break;

		curSequencerPtr = sfcSequencerNextMemInfoGet(taskId, curSequencerPtr);
	}

	return((strSfcSequcerInfo *)curSequencerPtr);	
}



static uint32 stepLinkActSet(uint32 stpId)
{
	uint32	status = NO_ERROR;
	uint32  i, actId;
	strStepInfo *stepInfoPtr;
	strActionInfo *actionInfoPtr;
	
	strStepLinkInfo * curActionPtr;

	if((stpId == 0) || (stpId > MAX_STEP_NUM)){
			status = L_STEP_ID_ERR;
			setErrorCode( __FILE__, __LINE__, "stepLinkActSet",status);
			return(status);
	}

	
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,
						__LINE__,
						"stepLinkActSet",
						status
					  );
		return(status);
	}

	stepInfoPtr = gSFCStepInfoLocalPtr;
	stepInfoPtr += (stpId -1);

	for( i = 0; i < stepInfoPtr->linkActionNum; i++) 
	{

		actId = stepInfoPtr->actionList[i];
		actionInfoPtr = gSFCActionInfoLocalPtr;
		actionInfoPtr += (actId -1);

		if(actId != actionInfoPtr->actionID) continue;
		else if(stepInfoPtr->taskID != actionInfoPtr->taskID) continue;

		curActionPtr = (strStepLinkInfo *)nodeGetAndUnlink (
								(strDoubleLink **)&sStepLinkQMngrInfo.freeList
            											);

		if(curActionPtr == NULL) {
			status = NO_EXIST_NODE_ERR;
			setErrorCode( __FILE__, __LINE__, "stepLinkActSet",status);
			goto MODULE_END;	
		}
		else {
			curActionPtr->taskId = actionInfoPtr->taskID;
			curActionPtr->actionId = actionInfoPtr->actionID;
			curActionPtr->actionFlag = stepInfoPtr->actionFlag[i];
			curActionPtr->actionTime  = stepInfoPtr->actionTime[i];
			curActionPtr->startFbExcution = sActExecQMngrInfo.usedList[actId - 1];

			status = nodePutOnLast( (strDoubleLink **) &sStepLinkQMngrInfo.usedList[stpId - 1],
             		            	(strDoubleLink *)curActionPtr
                		       	  );
			if(status) {
		        setErrorCode( __FILE__, __LINE__, "stepLinkActSet",status);
				goto MODULE_END;	
			}
		}
	}

MODULE_END:
	
#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif
	return(status);
}



static uint32 trsLinkInfoSet(uint32 taskId)
{
	uint32	status = NO_ERROR;
	uint32  trsId;
	strTransitionInfo *trsInfoPtr, *trsStartInfoPtr;
	strTrsLinkInfo * curTrsLinkPtr;

	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
			status = L_SEG_ID_ERR;
			setErrorCode( __FILE__, __LINE__, "trsLinkInfoSet",status);
			return(status);
	}

	
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,
						__LINE__,
						"stepLinkActSet",
						status
					  );
		return(status);
	}

	trsStartInfoPtr = gSFCTransitionLocalPtr;
	for( trsId = 1; trsId <= MAX_TRANSITOIN_NUM; trsId++) 
	{
		trsInfoPtr = trsStartInfoPtr + (trsId-1) ;
		if(trsInfoPtr->taskID != taskId) continue;

		curTrsLinkPtr = (strTrsLinkInfo *)nodeGetAndUnlink (
								(strDoubleLink **)&sTrsLinkQMngrInfo.freeList
            											);

		if(curTrsLinkPtr == NULL) {
			status = NO_EXIST_NODE_ERR;
			setErrorCode( __FILE__, __LINE__, "trsLinkInfoSet",status);
			goto MODULE_END;	
		}
		else {
			curTrsLinkPtr->taskId = trsInfoPtr->taskID;
			curTrsLinkPtr->transitionId = trsInfoPtr->transitionID;
			curTrsLinkPtr->setActiveData =trsInfoPtr->setActiveData;
			curTrsLinkPtr->setActiveHeader = trsInfoPtr->setActiveHeader;

			status = nodePutOnLast( (strDoubleLink **) &sTrsLinkQMngrInfo.usedList[taskId - 1],
             		            	(strDoubleLink *)curTrsLinkPtr
                		       	  );
			if(status) {
		        setErrorCode( __FILE__, __LINE__, "trsLinkInfoSet",status);
				goto MODULE_END;	
			}
		}
	}

MODULE_END:
	
#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif

	
	trsStartInfoPtr = gSFCTransitionLocalPtr;
	for( trsId = 1; trsId <= MAX_TRANSITOIN_NUM; trsId++) 
	{
		trsInfoPtr = trsStartInfoPtr + (trsId-1) ;
		if(trsInfoPtr->taskID != taskId) continue;
		
		trsInfoPtr->trsStatus = NO_ERROR;

		
		status = trsExecIdInfoSet(trsId);
		if(status != NO_ERROR) {
			printf( "[ERROR] - trsExecIdInfoSet() Call Error In #%d Transiton ID\n",trsId);
			return(status); 	
		}	
	}

	return(status);
}


static uint32 actLinkInfoSet(uint32 taskId)
{
	uint32	status = NO_ERROR;
	uint32  actId;
	strActionInfo *actInfoPtr, *actStartInfoPtr;
	strActionLinkInfo * curActLinkPtr;
	strActionCtrlInfo *actCtrlInfoPtr;

	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
			status = L_SEG_ID_ERR;
			setErrorCode( __FILE__, __LINE__, "actLinkInfoSet",status);
			return(status);
	}

	
    status = semaphoreGet(gFbExecSeqIdQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,
						__LINE__,
						"stepLinkActSet",
						status
					  );
		return(status);
	}

	actStartInfoPtr = gSFCActionInfoLocalPtr;
	for( actId = 1; actId <= MAX_ACTION_NUM; actId++) 
	{
		actInfoPtr = actStartInfoPtr + (actId-1) ;
		if(actInfoPtr->taskID != taskId) continue;

		curActLinkPtr = (strActionLinkInfo *)nodeGetAndUnlink (
								(strDoubleLink **)&sActLinkQMngrInfo.freeList
            											);

		if(curActLinkPtr == NULL) {
			status = NO_EXIST_NODE_ERR;
			setErrorCode( __FILE__, __LINE__, "actLinkInfoSet",status);
			goto MODULE_END;	
		}
		else {
			curActLinkPtr->taskId = actInfoPtr->taskID;
			curActLinkPtr->actionId = actInfoPtr->actionID;

			status = nodePutOnLast( (strDoubleLink **) &sActLinkQMngrInfo.usedList[taskId - 1],
             		            	(strDoubleLink *)curActLinkPtr
                		       	  );
			if(status) {
		        setErrorCode( __FILE__, __LINE__, "actLinkInfoSet",status);
				goto MODULE_END;	
			}
		}

	}

MODULE_END:
	
#ifdef VXWORKS
	semaphoreRelease (gFbExecSeqIdQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gFbExecSeqIdQSemaId);	
#endif

	actStartInfoPtr = gSFCActionInfoLocalPtr;
	for( actId = 1; actId <= MAX_ACTION_NUM; actId++) 
	{
		actInfoPtr = actStartInfoPtr + (actId-1) ;
		if(actInfoPtr->taskID != taskId) continue;
		
		
		actCtrlInfoPtr =(strActionCtrlInfo*)&actInfoPtr->actCtrlData;
		if(actCtrlInfoPtr != NULL)
			actCtrlInfoPtr->actStatus = NO_ERROR;

		
		status = actExecIdInfoSet(actId);
		if(status != NO_ERROR) {
			printf( "[ERROR] - actExecIdInfoSet() Call Error In #%d Action ID\n", actId);
			return(status); 	
		}
	}

	return(status);
}



static uint32 trsExecIdInfoSet(uint32 trsId)
{
	uint32	status = NO_ERROR, blockAddr = 0;
	strFbMemInfo 		fbMemInfo;
	strFbExecutionInfo	*curDataPtr, *nextDataPtr = NULL;

	strModuleExecInfo sysExeInfo;

	memoryClear( (uint8 *)&sysExeInfo, sizeof(strModuleExecInfo));
	status = fbSysExeModuleInfoRead(LOGIC_ID, &sysExeInfo);
	if(status != NO_ERROR){
		setErrorCode (	__FILE__,__LINE__,"trsExecIdInfoSet", status );
		return(status);
	}
		
	if((trsId == 0) || (trsId > MAX_TRANSITOIN_NUM)){
			status = L_TRANSITION_ID_ERR;
			setErrorCode( __FILE__, __LINE__, "trsExecIdInfoSet",status);
			return(status);
	}

	if (sysExeInfo.curfbMaxBlockAddr >  MAX_MPU_BLOCK_ADDR) 
	{	
		setErrorCode(	__FILE__,
						__LINE__,
						"trsExecIdInfoSet",
						status
					  );
		return(status);
	}

	
    status = semaphoreGet(gFbExecSeqIdQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,
						__LINE__,
						"trsExecIdInfoSet",
						status
					  );
		return(status);
	}
	
	
	
	sPrevSfcExecId = 0;
	
		
	

  	

	for(	blockAddr = 1; 
			blockAddr <= sysExeInfo.curfbMaxBlockAddr; blockAddr++) {
		
	    if( (status = fbCodeMemInfoRead	(
											LOGIC_ID,
           									blockAddr,
           									(strFbMemInfo *) &fbMemInfo
           								)
			) != NO_ERROR
		){
        	setErrorCode( __FILE__, __LINE__, "trsExecIdInfoSet",status);
			goto MODULE_END;
		}

		if( fbMemInfo.fbId == 0x0) continue;
		else if(fbMemInfo.logicType != SFC_LOGIC_TYPE) continue;
		else if(fbMemInfo.usingType != TRANSITION_TYPE) continue;
		else if(fbMemInfo.TransitionID  != trsId) continue;

		
		if(	fbMemInfo.fbExecSequenceId == 0x0) {
			status = FB_SEQ_ID_NULL_ERR;
			setErrorCodeWithVal(	__FILE__,__LINE__,"trsExecIdInfoSet",status,
									"Fb Blk Addr",blockAddr,
									"Fb Code", fbMemInfo.fbId,
									"Seq Id",fbMemInfo.fbExecSequenceId
								  );  
			goto MODULE_END;
		}
		
		
		else if (fbMemInfo.fbExecSequenceId > MAX_FB_EXEC_SEQ_ID){
			status = FB_SEQ_ID_MAX_EXCEED_ERR;
			setErrorCodeWithVal(	__FILE__,__LINE__,"trsExecIdInfoSet",status,
									"Fb Blk Addr",blockAddr,
									"Fb Code", fbMemInfo.fbId,
									"Seq Id",fbMemInfo.fbExecSequenceId
								  );  
			goto MODULE_END;
		}
		
		
	    
	    curDataPtr = (strFbExecutionInfo *)nodeGetAndUnlink (
                            	(strDoubleLink **)&sTrsExecQMngrInfo.freeList
            	                                    	);
		if(curDataPtr == NULL) {
			status = NO_EXIST_NODE_ERR;
        	setErrorCode( __FILE__, __LINE__, "trsExecIdInfoSet",status);
			goto MODULE_END;	
		}
		else {
				
			curDataPtr->fbExecId 		= fbMemInfo.fbExecSequenceId;
			curDataPtr->taskId			= fbMemInfo.fbTaskId;
			curDataPtr->fbId			= fbMemInfo.fbId;
			curDataPtr->fbBlockAddr		= blockAddr;
			
	
			
			gFBExeSequenceQueuePtr[blockAddr-1]= (strFbExecutionInfo*)curDataPtr;
		
			
			if(sysExeInfo.curfbMaxBlockAddr < blockAddr) 
					sysExeInfo.curfbMaxBlockAddr = blockAddr;

			if(fbMemInfo.fbExecSequenceId >= sPrevSfcExecId){
				
				
				nextDataPtr = NULL;					
				
				
				
				
				sPrevSfcExecId = fbMemInfo.fbExecSequenceId;
			}
			else if(fbMemInfo.fbExecSequenceId < sPrevSfcExecId){
					
				
				
				nextDataPtr = sTrsExecQMngrInfo.usedList[trsId - 1];
			
				while(nextDataPtr != NULL) {	
					
					
					if(curDataPtr->fbExecId < nextDataPtr->fbExecId)
						break;
					
					nextDataPtr = (strFbExecutionInfo *)nextNodeRef ( 	
			        						(strDoubleLink *)sTrsExecQMngrInfo.usedList[trsId - 1],
			                	        	(strDoubleLink *)nextDataPtr
											  				);

				} 
			}

			
			
			
			if(nextDataPtr == NULL) {
				
 				status = nodePutOnLast( (strDoubleLink **) &sTrsExecQMngrInfo.usedList[trsId - 1],
             			            (strDoubleLink *)curDataPtr
                			      );

				if(status) {
		        	setErrorCode( __FILE__, __LINE__, "trsExecIdInfoSet",status);
					goto MODULE_END;
				}						
			} 
			else {
				
				status = nodePutOnPrev	(	
							(strDoubleLink **)&sTrsExecQMngrInfo.usedList[trsId - 1],
							(strDoubleLink *)nextDataPtr,
							(strDoubleLink *)curDataPtr
									);

				if(status) {
		        	setErrorCode( __FILE__, __LINE__, "trsExecIdInfoSet",status);
					goto MODULE_END;
				}						
			} 
		} 
		
	} 		

	
	status = fbSysExeModuleInfoWrite(LOGIC_ID, &sysExeInfo);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "trsExecIdInfoSet",status);
		return(status);		
	}

MODULE_END:
	
#ifdef VXWORKS
	semaphoreRelease (gFbExecSeqIdQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gFbExecSeqIdQSemaId);	
#endif
	return(status);
}	


static uint32 actLinkInfoClear(uint32 taskId)
{
	uint32	status = NO_ERROR;
	strActionLinkInfo	*curDataPtr;
		
	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
		status = L_SEG_ID_ERR;
        setErrorCode( __FILE__, __LINE__, "actLinkInfoClear",status);
		return(status);
	}

	
	if(sActLinkQMngrInfo.usedList[taskId - 1] == NULL) return(status);

	
    status = semaphoreGet(gFbExecSeqIdQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,
						__LINE__,
						"actLinkInfoClear",
						status
					  );
		return(status);
	}
	
    while( sActLinkQMngrInfo.usedList[taskId - 1]!= NULL) {
        
   		curDataPtr = (strActionLinkInfo *)nodeGetAndUnlink(
                    				(strDoubleLink **)&sActLinkQMngrInfo.usedList[taskId - 1]
															);
       	if(curDataPtr != NULL){
			status = actExecIdInfoClear(curDataPtr->actionId);
			if(status != NO_ERROR) {
				printf( "[ERROR] - actExecIdInfoClear() Call Error In #%d Action ID\n", 
										curDataPtr->actionId);
			}	
           	
           	memoryClear( (uint8 *)curDataPtr, sizeof(strActionLinkInfo));
           	(void)nodePutOnLast( (strDoubleLink **) &sActLinkQMngrInfo.freeList,
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



static uint32 actExecIdInfoSet(uint32 actId)
{
	uint32	status = NO_ERROR, blockAddr = 0;
	strFbMemInfo 		fbMemInfo;
	strFbExecutionInfo	*curDataPtr, *nextDataPtr = NULL;

	strModuleExecInfo sysExeInfo;

	memoryClear( (uint8 *)&sysExeInfo, sizeof(strModuleExecInfo));
	status = fbSysExeModuleInfoRead(LOGIC_ID, &sysExeInfo);
	if(status != NO_ERROR){
		setErrorCode (	__FILE__,__LINE__,"actExecIdInfoSet", status );
		return(status);
	}

	if((actId == 0) || (actId > MAX_ACTION_NUM)){
		status = L_ACTION_ID_ERR;
		setErrorCode( __FILE__, __LINE__, "actExecIdInfoSet",status);
		return(status);
	}

	if (sysExeInfo.curfbMaxBlockAddr >  MAX_MPU_BLOCK_ADDR) 
	{	
		setErrorCode(	__FILE__,
						__LINE__,
						"actExecIdInfoSet",
						status
					  );
		return(status);
	}

	
    status = semaphoreGet(gFbExecSeqIdQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,
						__LINE__,
						"actExecIdInfoSet",
						status
					  );
		return(status);
	}
	
	
	
	sPrevSfcExecId = 0;
	
		
	

  	

	for(	blockAddr = 1; 
			blockAddr <= sysExeInfo.curfbMaxBlockAddr; blockAddr++) {
		
	    if( (status = fbCodeMemInfoRead	(
											LOGIC_ID,
           									blockAddr,
           									(strFbMemInfo *) &fbMemInfo
           								)
			) != NO_ERROR
		){
        	setErrorCode( __FILE__, __LINE__, "actExecIdInfoSet",status);
			goto MODULE_END;
		}

		if( fbMemInfo.fbId == 0x0) continue;
		else if( fbMemInfo.logicType != SFC_LOGIC_TYPE) continue;
		else if (fbMemInfo.usingType != ACTION_TYPE) continue;
		else if (fbMemInfo.ActionID  != actId)  continue;

		
		if(	fbMemInfo.fbExecSequenceId == 0x0) {
			status = FB_SEQ_ID_NULL_ERR;
			setErrorCodeWithVal(	__FILE__,__LINE__,"actExecIdInfoSet",status,
									"Fb Blk Addr",blockAddr,
									"Fb Code", fbMemInfo.fbId,
									"Seq Id",fbMemInfo.fbExecSequenceId
								  );  
			goto MODULE_END;
		}
		
		
		else if (fbMemInfo.fbExecSequenceId > MAX_FB_EXEC_SEQ_ID){
			status = FB_SEQ_ID_MAX_EXCEED_ERR;
			setErrorCodeWithVal(	__FILE__,__LINE__,"actExecIdInfoSet",status,
									"Fb Blk Addr",blockAddr,
									"Fb Code", fbMemInfo.fbId,
									"Seq Id",fbMemInfo.fbExecSequenceId
								  );  
			goto MODULE_END;
		}
		
		
	    
	    curDataPtr = (strFbExecutionInfo *)nodeGetAndUnlink (
                            	(strDoubleLink **)&sActExecQMngrInfo.freeList
            	                                    	);
		if(curDataPtr == NULL) {
			status = NO_EXIST_NODE_ERR;
        	setErrorCode( __FILE__, __LINE__, "actExecIdInfoSet",status);
			goto MODULE_END;	
		}
		else {
				
			curDataPtr->fbExecId 		= fbMemInfo.fbExecSequenceId;
			curDataPtr->taskId			= fbMemInfo.fbTaskId;
			curDataPtr->fbId			= fbMemInfo.fbId;
			curDataPtr->fbBlockAddr		= blockAddr;
			

			
			gFBExeSequenceQueuePtr[blockAddr-1]= (strFbExecutionInfo*)curDataPtr;
		
			
			if(sysExeInfo.curfbMaxBlockAddr < blockAddr) 
					sysExeInfo.curfbMaxBlockAddr = blockAddr;

			if(fbMemInfo.fbExecSequenceId >= sPrevSfcExecId){
				
				
				nextDataPtr = NULL;					
				
				
				
				
				sPrevSfcExecId = fbMemInfo.fbExecSequenceId;
			}
			else if(fbMemInfo.fbExecSequenceId < sPrevSfcExecId){
					
				
				
				nextDataPtr = sActExecQMngrInfo.usedList[actId - 1];
			
				while(nextDataPtr != NULL) {	
					
					
					if(curDataPtr->fbExecId < nextDataPtr->fbExecId)
						break;
					
					nextDataPtr = (strFbExecutionInfo *)nextNodeRef ( 	
			        						(strDoubleLink *)sActExecQMngrInfo.usedList[actId - 1],
			                	        	(strDoubleLink *)nextDataPtr
											  				);

				} 
			}

			
			
			
			if(nextDataPtr == NULL) {
				
	   			status = nodePutOnLast( (strDoubleLink **) &sActExecQMngrInfo.usedList[actId - 1],
             			        (strDoubleLink *)curDataPtr
                			  );
				if(status) {
		        	setErrorCode( __FILE__, __LINE__, "actExecIdInfoSet",status);
					goto MODULE_END;
				}						
			} 
			else {
				
				status = nodePutOnPrev	(	
							(strDoubleLink **)&sActExecQMngrInfo.usedList[actId - 1],
							(strDoubleLink *)nextDataPtr,
							(strDoubleLink *)curDataPtr
									);

				if(status) {
		        	setErrorCode( __FILE__, __LINE__, "actExecIdInfoSet",status);
					goto MODULE_END;
				}						
			} 
		} 
		
	} 		

	
	status = fbSysExeModuleInfoWrite(LOGIC_ID, &sysExeInfo);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "actExecIdInfoSet",status);
		return(status);		
	}

MODULE_END:
	
#ifdef VXWORKS
	semaphoreRelease (gFbExecSeqIdQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gFbExecSeqIdQSemaId);	
#endif
	return(status);
}	

static uint32	transitionExecIdQueInit(void)
{
	uint32	status = NO_ERROR;
	uint32	loop;

    

    sTrsExecQMngrInfo.freeList = NULL;
	
	for(loop = 0; loop < MAX_LOGIC_TASK_NUM; loop++)
    	sTrsExecQMngrInfo.usedList[loop] = NULL;

    for( loop = 0; loop < MAX_FB_EXEC_SEQ_ID_NUM; loop++) {
		memoryClear (   (char *) &sTrsExecDataInfo[loop],
                         sizeof(strFbExecutionInfo)
                    );
        status = nodePutOnLast	(	(strDoubleLink **) &sTrsExecQMngrInfo.freeList,
                                	(strDoubleLink *)  &sTrsExecDataInfo[loop]
                            	);
		if(status != NO_ERROR)
			break;                            
    }
	
	return(status);
}


static uint32	transitionLinkQueInit(void)
{
	uint32	status = NO_ERROR;
	uint32	loop;

    

    sTrsLinkQMngrInfo.freeList = NULL;
	
	for(loop = 0; loop < MAX_LOGIC_TASK_NUM; loop++)
    	sTrsLinkQMngrInfo.usedList[loop] = NULL;

    for( loop = 0; loop < MAX_TRANSITOIN_NUM; loop++) {
		memoryClear (   (char *) &sTrsLinkDataInfo[loop],
                         sizeof(strTrsLinkInfo)
                    );
        status = nodePutOnLast	(	(strDoubleLink **) &sTrsLinkQMngrInfo.freeList,
                                	(strDoubleLink *)  &sTrsLinkDataInfo[loop]
                            	);
		if(status != NO_ERROR)
			break;                            
    }
	
	return(status);
}


static uint32	actionLinkQueInit(void)
{

	uint32	status = NO_ERROR;
	uint32	loop;

    
    sActLinkQMngrInfo.freeList = NULL;
	for(loop = 0; loop < MAX_LOGIC_TASK_NUM; loop++) 
    	sActLinkQMngrInfo.usedList[loop] = NULL;

    for( loop = 0; loop < MAX_ACTION_NUM; loop++) {
		memoryClear (   (char *) &sActLinkDataInfo[loop],
                         sizeof(strActionLinkInfo)
                    );
        status = nodePutOnLast	(	(strDoubleLink **) &sActLinkQMngrInfo.freeList,
                                	(strDoubleLink *)  &sActLinkDataInfo[loop]
                            	);
		if(status != NO_ERROR)
			break;                            
    }
	
	return(status);
}


static uint32	actionExecIdQueInit(void)
{
	uint32	status = NO_ERROR;
	uint32	loop;

    

    sActExecQMngrInfo.freeList = NULL;
	
	for(loop = 0; loop < MAX_ACTION_NUM; loop++)
    	sActExecQMngrInfo.usedList[loop] = NULL;

    for( loop = 0; loop < MAX_FB_EXEC_SEQ_ID_NUM; loop++) {
		memoryClear (   (char *) &sActExecDataInfo[loop],
                         sizeof(strFbExecutionInfo)
                    );
        status = nodePutOnLast	(	(strDoubleLink **) &sActExecQMngrInfo.freeList,
                                	(strDoubleLink *)  &sActExecDataInfo[loop]
                            	);
		if(status != NO_ERROR)
			break;                            
    }
	
	return(status);
}


static uint32	stepLinkActQueInit(void)
{
	uint32	status = NO_ERROR;
	uint32	loop;

    

    sStepLinkQMngrInfo.freeList = NULL;
	
	for(loop = 0; loop < MAX_STEP_NUM; loop++)
    	sStepLinkQMngrInfo.usedList[loop] = NULL;

    for( loop = 0; loop < MAX_SFC_EXEC_NUM; loop++) {
		memoryClear (   (char *) &sStepLinkDataInfo[loop],
                         sizeof(strFbExecutionInfo)
                    );
        status = nodePutOnLast	(	(strDoubleLink **) &sStepLinkQMngrInfo.freeList,
                                	(strDoubleLink *)  &sStepLinkDataInfo[loop]
                            	);
		if(status != NO_ERROR)
			break;                            
    }
	
	return(status);
}

static uint32	sfcAdjacentQueInit(void)
{
	uint32	status = NO_ERROR;
	uint32	loop;

    
    sSfcAdjacentQMngrInfo.freeList = NULL;
	
	for(loop = 0; loop <  MAX_VERTEX_NUM ; loop++){
    	sSfcAdjacentQMngrInfo.nextList[loop] = NULL;
    	sSfcAdjacentQMngrInfo.prevList[loop] = NULL;
	}

    for( loop = 0; loop < MAX_SFC_EXEC_NUM*2; loop++) {
		memoryClear (   (char *) &sSfcAdjacentDataInfo[loop],
                         sizeof(strSfcAdjacentInfo)
                    );
        status = nodePutOnLast	(	(strDoubleLink **) &sSfcAdjacentQMngrInfo.freeList,
                                	(strDoubleLink *)  &sSfcAdjacentDataInfo[loop]
                            	);
		if(status != NO_ERROR)
			break;                            
    }
	
	return(status);
}


static uint32	sfcSequncerQueInit(void)
{
	uint32	status = NO_ERROR;
	uint32	loop;

    
    sSfcSequencerQMngrInfo.freeList = NULL;
	
	for(loop = 0; loop < MAX_LOGIC_TASK_NUM; loop++){
    	sSfcSequencerQMngrInfo.usedList[loop] = NULL;
	}

	
    for( loop = 0; loop < MAX_VERTEX_NUM; loop++) {
		memoryClear (   (char *) &sSfcSequencerDataInfo[loop], 
                         sizeof(strSfcAdjacentInfo)
                    );
        status = nodePutOnLast	(	(strDoubleLink **) &sSfcSequencerQMngrInfo.freeList,
                                	(strDoubleLink *)  &sSfcSequencerDataInfo[loop]
                            	);
		if(status != NO_ERROR)
			break;                            
    }
	
	return(status);
}


static uint32	sfcExecutionQueInit(void)
{
	uint32	status = NO_ERROR;
	uint32	loop;

    
    sSfcExecQMngrInfo.freeList = NULL;
	
	for(loop = 0; loop < MAX_LOGIC_TASK_NUM ; loop++){
    	sSfcExecQMngrInfo.curExeList[loop] = NULL;
	}

    for( loop = 0; loop < MAX_VERTEX_NUM; loop++) {
		memoryClear (   (char *) &sSfcExecDataInfo[loop], 
                         sizeof(strSfcExecutionInfo)
                    );
        status = nodePutOnLast	(	(strDoubleLink **) &sSfcExecQMngrInfo.freeList,
                                	(strDoubleLink *)  &sSfcExecDataInfo[loop]
                            	);
		if(status != NO_ERROR)
			break;                            
    }
	
	return(status);
}

static uint32 trsExecIdInfoClear(uint32 trsId)
{
	uint32	status = NO_ERROR;
	strFbExecutionInfo	*curDataPtr;
		
	if((trsId == 0) || (trsId > MAX_TRANSITOIN_NUM)){
		status = L_TRANSITION_ID_ERR;
        setErrorCode( __FILE__, __LINE__, "trsExecIdInfoClear",status);
		return(status);
	}

	
	if(sTrsExecQMngrInfo.usedList[trsId - 1] == NULL) return(status);

	
    status = semaphoreGet(gFbExecSeqIdQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,
						__LINE__,
						"trsExecIdInfoClear",
						status
					  );
		return(status);
	}
	
    while( sTrsExecQMngrInfo.usedList[trsId - 1]!= NULL) {
        
   		curDataPtr = (strFbExecutionInfo *)nodeGetAndUnlink(
                    				(strDoubleLink **)&sTrsExecQMngrInfo.usedList[trsId - 1]
															);
       	if(curDataPtr != NULL){
			if( (curDataPtr->fbBlockAddr >0) && (curDataPtr->fbBlockAddr<= MAX_MPU_BLOCK_ADDR) )
				gFBExeSequenceQueuePtr[curDataPtr->fbBlockAddr -1] = NULL; 
           	
           	memoryClear( (uint8 *)curDataPtr, sizeof(strFbExecutionInfo));
           	(void)nodePutOnLast( (strDoubleLink **) &sTrsExecQMngrInfo.freeList,
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


uint32 trsExecIdInfoDelete(uint32 trsId, uint32 fbId, uint32 fbBlkAddr)
{
	uint32	status = NO_ERROR;
	strFbExecutionInfo	*nodeDataPtr;
	uint32 delListNum,listLoop;
	uint32 delList[MAX_ONLINE_DWL_FB_CODE_DEL_NUM];

	status = semaphoreGet(gFbExecSeqIdQSemaId, NOWAIT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,__LINE__,"trsExecIdInfoDelete",status);
		return(status);
	}
	
	nodeDataPtr = sTrsExecQMngrInfo.usedList[trsId - 1];

	if(nodeDataPtr == NULL) {
		status = NO_EXIST_NODE_ERR;
		setErrorCode(	__FILE__,__LINE__,"trsExecIdInfoDelete",status);
		goto MODULE_END;		
	}
	
	delListNum = 0;
	while(nodeDataPtr != NULL) {
		if(nodeDataPtr->fbBlockAddr == fbBlkAddr){
			
			if(nodeDataPtr->fbId != fbId){
				status = L_FB_CODE_ERR;
				setErrorCodeWithVal(	__FILE__,__LINE__,"trsExecIdInfoDelete", status,
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
			        							(strDoubleLink *)sTrsExecQMngrInfo.usedList[trsId - 1],
			                        			(strDoubleLink *)nodeDataPtr
										  						);
	}
	
	if(delListNum == 0) {
		status = L_FB_CODE_ERR;
		setErrorCodeWithVal(	__FILE__,__LINE__,"trsExecIdInfoDelete", status,
								"Req Blk Addr", fbBlkAddr,
								"Req Fb Code", fbId,
								"Req TRS ID", trsId
							  );
		goto MODULE_END;	
	}
	
	
	for(listLoop = 0; listLoop < delListNum; listLoop++){
   		status = nodeUnlink( 	
        					(strDoubleLink **)&sTrsExecQMngrInfo.usedList[trsId - 1],
							(strDoubleLink *)delList[listLoop]
      						);
		                            		
		if(status != NO_ERROR){
   			setErrorCode( __FILE__, __LINE__, "fbExecIdInfoDelete",status);
			goto MODULE_END;	
		}
		
		status = nodePutOnLast( (strDoubleLink **) &sTrsExecQMngrInfo.freeList,
       	   						(strDoubleLink *)delList[listLoop]
           		  			  );
		if(status != NO_ERROR){
   			setErrorCode( __FILE__, __LINE__, "trsExecIdInfoDelete",status);
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


static uint32 trsLinkInfoClear(uint32 taskId)
{
	uint32	status = NO_ERROR;
	strTrsLinkInfo	*curDataPtr;
		
	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
		status = L_SEG_ID_ERR;
        setErrorCode( __FILE__, __LINE__, "trsLinkInfoClear",status);
		return(status);
	}

	
	if(sTrsLinkQMngrInfo.usedList[taskId - 1] == NULL) return(status);

	
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
 	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,__LINE__,
						"trsLinkInfoClear",	status
					  );
		return(status);
	}
	
    while( sTrsLinkQMngrInfo.usedList[taskId - 1]!= NULL) {
        
   		curDataPtr = (strTrsLinkInfo *)nodeGetAndUnlink(
                    				(strDoubleLink **)&sTrsLinkQMngrInfo.usedList[taskId - 1]
															);
	 	if(curDataPtr != NULL){
			
			status = trsExecIdInfoClear(curDataPtr->transitionId);
			if(status != NO_ERROR) {
				printf( "[ERROR] - trsExecIdInfoClear() Call Error In #%d Transiton ID\n",
					curDataPtr->transitionId);
			}	

           	
           	memoryClear( (uint8 *)curDataPtr, sizeof(strTrsLinkInfo));
           	(void)nodePutOnLast( (strDoubleLink **) &sTrsLinkQMngrInfo.freeList,
               	                 (strDoubleLink *)curDataPtr
                   	           );
        }
    }

	
#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif
	return(status);
}



static uint32 actExecIdInfoClear(uint32 actId)
{
	uint32	status = NO_ERROR;
	strFbExecutionInfo	*curDataPtr;
		
	if((actId == 0) || (actId > MAX_ACTION_NUM)){
		status = L_ACTION_ID_ERR;
        setErrorCode( __FILE__, __LINE__, "actExecIdInfoClear",status);
		return(status);
	}

	
	if(sActExecQMngrInfo.usedList[actId - 1] == NULL) return(status);

	
    status = semaphoreGet(gFbExecSeqIdQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,
						__LINE__,
						"actExecIdInfoClear",
						status
					  );
		return(status);
	}
	
    while( sActExecQMngrInfo.usedList[actId - 1]!= NULL) {
        
   		curDataPtr = (strFbExecutionInfo *)nodeGetAndUnlink(
                    				(strDoubleLink **)&sActExecQMngrInfo.usedList[actId - 1]
															);
       	if(curDataPtr != NULL){
			if( (curDataPtr->fbBlockAddr >0) && (curDataPtr->fbBlockAddr<= MAX_MPU_BLOCK_ADDR) )
				gFBExeSequenceQueuePtr[curDataPtr->fbBlockAddr -1] = NULL; 

           	
           	memoryClear( (uint8 *)curDataPtr, sizeof(strFbExecutionInfo));
           	(void)nodePutOnLast( (strDoubleLink **) &sActExecQMngrInfo.freeList,
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


uint32 actExecIdInfoDelete(uint32 actId, uint32 fbId, uint32 fbBlkAddr)
{
	uint32	status = NO_ERROR;
	strFbExecutionInfo	*nodeDataPtr;
	uint32 delListNum,listLoop;
	uint32 delList[MAX_ONLINE_DWL_FB_CODE_DEL_NUM];

	status = semaphoreGet(gFbExecSeqIdQSemaId, NOWAIT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,__LINE__,"actExecIdInfoDelete",status);
		return(status);
	}
	
	nodeDataPtr = sActExecQMngrInfo.usedList[actId - 1];

	if(nodeDataPtr == NULL) {
		status = NO_EXIST_NODE_ERR;
		setErrorCode(	__FILE__,__LINE__,"actExecIdInfoDelete",status);
		goto MODULE_END;		
	}
	
	delListNum = 0;
	while(nodeDataPtr != NULL) {
		if(nodeDataPtr->fbBlockAddr == fbBlkAddr){
			
			if(nodeDataPtr->fbId != fbId){
				status = L_FB_CODE_ERR;
				setErrorCodeWithVal(	__FILE__,__LINE__,"actExecIdInfoDelete", status,
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
			        							(strDoubleLink *)sActExecQMngrInfo.usedList[actId - 1],
			                        			(strDoubleLink *)nodeDataPtr
										  						);
	}
	
	if(delListNum == 0) {
		status = L_FB_CODE_ERR;
		setErrorCodeWithVal(	__FILE__,__LINE__,"actExecIdInfoDelete", status,
								"Req Blk Addr", fbBlkAddr,
								"Req Fb Code", fbId,
								"Req ACT ID", actId
							  );
		goto MODULE_END;	
	}
	
	
	for(listLoop = 0; listLoop < delListNum; listLoop++){
   		status = nodeUnlink( 	
        					(strDoubleLink **)&sActExecQMngrInfo.usedList[actId - 1],
							(strDoubleLink *)delList[listLoop]
      						);
		                            		
		if(status != NO_ERROR){
   			setErrorCode( __FILE__, __LINE__, "actExecIdInfoDelete",status);
			goto MODULE_END;	
		}
		
		status = nodePutOnLast( (strDoubleLink **) &sActExecQMngrInfo.freeList,
       	   						(strDoubleLink *)delList[listLoop]
           		  			  );
		if(status != NO_ERROR){
   			setErrorCode( __FILE__, __LINE__, "actExecIdInfoDelete",status);
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

static uint32 stepLinkActInfoClear(uint32 stpId)
{
	uint32	status = NO_ERROR;
	strStepLinkInfo	*curDataPtr;
		
	if((stpId == 0) || (stpId > MAX_STEP_NUM)){
		status = L_STEP_ID_ERR;
        setErrorCode( __FILE__, __LINE__, "stepLinkActInfoClear",status);
		return(status);
	}

	
	if(sStepLinkQMngrInfo.usedList[stpId - 1] == NULL) return(status);

	
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,
						__LINE__,
						"stepLinkActInfoClear",	status
					  );
		return(status);
	}
	
    while( sStepLinkQMngrInfo.usedList[stpId - 1]!= NULL) {
        
   		curDataPtr = (strStepLinkInfo *)nodeGetAndUnlink(
                    				(strDoubleLink **)&sStepLinkQMngrInfo.usedList[stpId - 1]
															);
       	if(curDataPtr != NULL){
           	
           	memoryClear( (uint8 *)curDataPtr, sizeof(strStepLinkInfo));
           	(void)nodePutOnLast( (strDoubleLink **) &sStepLinkQMngrInfo.freeList,
               	                 (strDoubleLink *)curDataPtr
                   	           );
        }
    }

	
#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif
	return(status);
}


static uint32 sfcExecutionInfoClear(uint32 taskId)
{
	uint32	status = NO_ERROR;
	strSfcExecutionInfo *curDataPtr;
		
	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
		status = L_SEG_ID_ERR;
        setErrorCode( __FILE__, __LINE__, "sfcExecutionInfoClear",status);
		return(status);
	}

	
	if(sSfcExecQMngrInfo.curExeList[taskId - 1] == NULL) 
		return(status);

	
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,
						__LINE__,
						"sfcExecutionInfoClear",
						status
					  );
		return(status);
	}
	
    while( sSfcExecQMngrInfo.curExeList[taskId - 1]!= NULL) {
        
   		curDataPtr = (strSfcExecutionInfo *)nodeGetAndUnlink(
                    				(strDoubleLink **)&sSfcExecQMngrInfo.curExeList[taskId - 1]
															);
       	if(curDataPtr != NULL){
           	
           	memoryClear( (uint8 *)curDataPtr, sizeof(strSfcExecutionInfo));
           	(void)nodePutOnLast( (strDoubleLink **) &sSfcExecQMngrInfo.freeList,
               	                 (strDoubleLink *)curDataPtr
                   	           );
        }
    }

	
#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif
	return(status);
}

static uint32 sfcSequencerInfoClear(uint32 taskId)
{
	uint32	status = NO_ERROR;
	strSfcSequcerInfo *curDataPtr;
		
	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
		status = L_SEG_ID_ERR;
        setErrorCode( __FILE__, __LINE__, "sfcSequencerInfoClear",status);
		return(status);
	}

	
	if(sSfcSequencerQMngrInfo.usedList[taskId - 1] == NULL) return(status);

	
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,
						__LINE__,
						"sfcSequencerInfoClear",
						status
					  );
		return(status);
	}
	
    while( sSfcSequencerQMngrInfo.usedList[taskId - 1]!= NULL) {
        
   		curDataPtr = (strSfcSequcerInfo *)nodeGetAndUnlink(
                    				(strDoubleLink **)&sSfcSequencerQMngrInfo.usedList[taskId - 1]
															);
       	if(curDataPtr != NULL){
			
			if(curDataPtr->nodeType == STEP_TYPE) {
				status = stepLinkActInfoClear(curDataPtr->stepTrId);
				if(status != NO_ERROR) {
					printf( "[ERROR] - stepLinkActInfoClear() Call Error In #%d Step ID\n",
						curDataPtr->stepTrId);
				}	
			}

           	
           	memoryClear( (uint8 *)curDataPtr, sizeof(strSfcSequcerInfo));
           	(void)nodePutOnLast( (strDoubleLink **) &sSfcSequencerQMngrInfo.freeList,
               	                 (strDoubleLink *)curDataPtr
                   	           );
        }
    }

	
#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif
	return(status);
}


static uint32 sfcAdjacentInfoClear(uint32 taskId)
{
	uint32	status = NO_ERROR;
	strFbTaskTblMemInfo segTblInfo;
	strSfcSequcerInfo   *sfcExecIdInfoPtr;
	strSfcAdjacentInfo *curDataPtr;
	uint32 vertexId = 0;
	
	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
			status = L_SEG_ID_ERR;
			setErrorCode( __FILE__, __LINE__, "sfcAdjacentInfoSet",status);
			return(status);
	}

	status = getFbTaskTblInfo	( 
								LOGIC_ID,taskId,
								(strFbTaskTblMemInfo	*) &segTblInfo
							);
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "sfcAdjacentInfoSet",status);
		return(status);		
	}
	
	if(segTblInfo.logicType != SFC_LOGIC_TYPE) return(status);

	
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,__LINE__,
						"sfcAdjacentInfoSet",status
					  );
		return(status);
	}
	
	
	sfcExecIdInfoPtr = sfcSequencerStartMemInfoGet(taskId);
	while(sfcExecIdInfoPtr!=NULL) 
	{
		if(sfcExecIdInfoPtr->nodeType == STEP_TYPE)
				vertexId = (sfcExecIdInfoPtr->stepTrId - 1); 
		else if(sfcExecIdInfoPtr->nodeType == TRANSITION_TYPE)
				vertexId = (sfcExecIdInfoPtr->stepTrId -1 ) + MAX_STEP_NUM;

		while( sSfcAdjacentQMngrInfo.prevList[vertexId]!= NULL) {
			
   			curDataPtr = (strSfcAdjacentInfo *)nodeGetAndUnlink(
                    					(strDoubleLink **)&sSfcAdjacentQMngrInfo.prevList[vertexId]
																);
       		if(curDataPtr != NULL){
           		
           		memoryClear( (uint8 *)curDataPtr, sizeof(strSfcAdjacentInfo));
           		(void)nodePutOnLast( (strDoubleLink **) &sSfcAdjacentQMngrInfo.freeList,
               						 (strDoubleLink *)curDataPtr
                   				   );
			}
		}

		while( sSfcAdjacentQMngrInfo.nextList[vertexId]!= NULL) {
			
   			curDataPtr = (strSfcAdjacentInfo *)nodeGetAndUnlink(
                    					(strDoubleLink **)&sSfcAdjacentQMngrInfo.nextList[vertexId]
																);
       		if(curDataPtr != NULL){
           		
           		memoryClear( (uint8 *)curDataPtr, sizeof(strSfcAdjacentInfo));
           		(void)nodePutOnLast( (strDoubleLink **) &sSfcAdjacentQMngrInfo.freeList,
               						 (strDoubleLink *)curDataPtr
                   				   );
			}
		}
		sfcExecIdInfoPtr = sfcSequencerNextMemInfoGet(taskId,sfcExecIdInfoPtr );
	}


	
#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif
	return(status);
}

strFbExecutionInfo *trsExecIdStartMemInfoGet(uint32 trsId)
{
	if( (trsId == 0) || (trsId >MAX_TRANSITOIN_NUM) )
		return(NULL);
	else 
	    return((strFbExecutionInfo *)sTrsExecQMngrInfo.usedList[trsId - 1]);	
}



strFbExecutionInfo *trsExecIdNextMemInfoGet(uint32 trsId, strFbExecutionInfo *dataPtr)
{
	strDoubleLink *head_ptr = (strDoubleLink *)sTrsExecQMngrInfo.usedList[trsId - 1];
    strDoubleLink *cur_node = (strDoubleLink *)dataPtr;
    strDoubleLink *next_node = NULL;

    if( (head_ptr == NULL) || (cur_node == NULL))
        next_node = NULL;
	else if(cur_node == cur_node->next)
		next_node = NULL;
    else if(cur_node->next != head_ptr)
        next_node = cur_node->next;
    else 
        next_node = NULL;

    return((strFbExecutionInfo *)next_node);
}



strFbExecutionInfo *actExecIdStartMemInfoGet(uint32 actId)
{
	if( (actId == 0) || (actId >MAX_ACTION_NUM) )
		return(NULL);
	else  
	    return((strFbExecutionInfo *)sActExecQMngrInfo.usedList[actId - 1]);	
}



strFbExecutionInfo *actExecIdNextMemInfoGet(uint32 actId, strFbExecutionInfo *dataPtr)
{
	strDoubleLink *head_ptr = (strDoubleLink *)sActExecQMngrInfo.usedList[actId - 1];
    strDoubleLink *cur_node = (strDoubleLink *)dataPtr;
    strDoubleLink *next_node = NULL;

    if( (head_ptr == NULL) || (cur_node == NULL))
        next_node = NULL;
	else if(cur_node == cur_node->next)
		next_node = NULL;
    else if(cur_node->next != head_ptr)
        next_node = cur_node->next;
    else 
        next_node = NULL;

    return((strFbExecutionInfo *)next_node);
}


strSfcExecutionInfo *sfcCurrencExecIdStartMemInfoGet(uint32 taskId)
{
	if( (taskId == 0) || (taskId >MAX_LOGIC_TASK_NUM) )
		return(NULL);
	else  
		return((strSfcExecutionInfo *)sSfcExecQMngrInfo.curExeList[taskId - 1]);	
}



strSfcExecutionInfo *sfcCurrentExecIdNextMemInfoGet(uint32 taskId, strSfcExecutionInfo *dataPtr)
{
	strDoubleLink *head_ptr = (strDoubleLink *)sSfcExecQMngrInfo.curExeList[taskId - 1];
    strDoubleLink *cur_node = (strDoubleLink *)dataPtr;
    strDoubleLink *next_node = NULL;

    if( (head_ptr == NULL) || (cur_node == NULL))
        next_node = NULL;
    else if(cur_node->next != head_ptr)
        next_node = cur_node->next;
    else 
        next_node = NULL;

    return((strSfcExecutionInfo *)next_node);
}


uint32 sfcExecCurrentInfoPut(uint32 taskId, strSfcSequcerInfo *sequencerPtr)
{
	uint32 status = NO_ERROR;
	strSfcExecutionInfo *dataPtr =NULL;
	dataPtr = (strSfcExecutionInfo *)nodeGetAndUnlink (
								 (strDoubleLink **)&sSfcExecQMngrInfo.freeList
            					 );
	if(dataPtr != NULL) {
		dataPtr->sequencerId = sequencerPtr;
		status = nodePutOnLast( (strDoubleLink **) &sSfcExecQMngrInfo.curExeList[taskId - 1],
             		            			(strDoubleLink *)dataPtr );
	}
    return(status);
}


strSfcExecutionInfo* sfcExecCurrentInfoGet(uint32 taskId)
{
	uint32 status = NO_ERROR;

	strSfcExecutionInfo *dataPtr =NULL;

	dataPtr =  (strSfcExecutionInfo *)nodeGetAndUnlink(
				(strDoubleLink **)&sSfcExecQMngrInfo.curExeList[taskId - 1]);

	if(dataPtr != NULL){
		status = nodePutOnLast( (strDoubleLink **) &sSfcExecQMngrInfo.freeList,
             						(strDoubleLink *)dataPtr
                				  );
	}

    return((strSfcExecutionInfo *)dataPtr);
}


uint32 sfcExecCurrentInfoUnlink(uint32 taskId, strSfcSequcerInfo *unlinkData)
{
	uint32 status = NO_ERROR;
	strSfcExecutionInfo *dataPtr ;
	dataPtr = sSfcExecQMngrInfo.curExeList[taskId - 1];
	while(dataPtr!=NULL) 
	{
		if(dataPtr ->sequencerId == unlinkData){
		  nodeUnlink( (strDoubleLink **)&sSfcExecQMngrInfo.curExeList[taskId - 1],
                  (strDoubleLink *)dataPtr
                );
		  status = nodePutOnLast( (strDoubleLink **) &sSfcExecQMngrInfo.freeList,
             						(strDoubleLink *)dataPtr
                				  );
		  break;
		}
		 dataPtr = sfcCurrentExecIdNextMemInfoGet(taskId,dataPtr);
	}
    return(status);
}


strSfcSequcerInfo *sfcSequencerStartMemInfoGet(uint32 taskId)
{
	if( (taskId == 0) || (taskId >MAX_LOGIC_TASK_NUM) )
		return(NULL);
	else  
		return((strSfcSequcerInfo *)sSfcSequencerQMngrInfo.usedList[taskId - 1]);	
}


strSfcSequcerInfo *sfcSequencerNextMemInfoGet(uint32 taskId, strSfcSequcerInfo *dataPtr)
{
	strDoubleLink *head_ptr = (strDoubleLink *)sSfcSequencerQMngrInfo.usedList[taskId - 1];
    strDoubleLink *cur_node = (strDoubleLink *)dataPtr;
    strDoubleLink *next_node = NULL;

    if( (head_ptr == NULL) || (cur_node == NULL))
        next_node = NULL;
    else if(cur_node->next != head_ptr)
        next_node = cur_node->next;
    else 
        next_node = NULL;

    return((strSfcSequcerInfo *)next_node);
}




strActionLinkInfo *actLinkStartMemInfoGet(uint32 taskId)
{
	if( (taskId == 0) || (taskId >MAX_LOGIC_TASK_NUM) )
		return(NULL);
	else  
		return((strActionLinkInfo *)sActLinkQMngrInfo.usedList[taskId - 1]);	
}



strActionLinkInfo *actLinkNextMemInfoGet(uint32 taskId, strActionLinkInfo *dataPtr)
{
	strDoubleLink *head_ptr = (strDoubleLink *)sActLinkQMngrInfo.usedList[taskId - 1];
    strDoubleLink *cur_node = (strDoubleLink *)dataPtr;
    strDoubleLink *next_node = NULL;

    if( (head_ptr == NULL) || (cur_node == NULL))
        next_node = NULL;
    else if(cur_node->next != head_ptr)
        next_node = cur_node->next;
    else 
        next_node = NULL;

    return((strActionLinkInfo *)next_node);
}



strActionLinkInfo *actCurStartMemInfoGet(uint32 taskId)
{
	if( (taskId == 0) || (taskId >MAX_LOGIC_TASK_NUM) )
		return(NULL);
	else  
		return((strActionLinkInfo *)sActCurQMngrInfo.usedList[taskId - 1]);	
}



strActionLinkInfo *actCurNextMemInfoGet(uint32 taskId, strActionLinkInfo *dataPtr)
{
	strDoubleLink *head_ptr = (strDoubleLink *)sActCurQMngrInfo.usedList[taskId - 1];
    strDoubleLink *cur_node = (strDoubleLink *)dataPtr;
    strDoubleLink *next_node = NULL;

    if( (head_ptr == NULL) || (cur_node == NULL))
        next_node = NULL;
    else if(cur_node->next != head_ptr)
        next_node = cur_node->next;
    else 
        next_node = NULL;

    return((strActionLinkInfo *)next_node);
}


strTrsLinkInfo *trsLinkStartMemInfoGet(uint32 taskId)
{
	if( (taskId == 0) || (taskId >MAX_LOGIC_TASK_NUM) )
		return(NULL);
	else  
	    return((strTrsLinkInfo *)sTrsLinkQMngrInfo.usedList[taskId - 1]);	
}



strTrsLinkInfo *trsLinkNextMemInfoGet(uint32 taskId, strTrsLinkInfo *dataPtr)
{
	strDoubleLink *head_ptr = (strDoubleLink *)sTrsLinkQMngrInfo.usedList[taskId - 1];
    strDoubleLink *cur_node = (strDoubleLink *)dataPtr;
    strDoubleLink *next_node = NULL;

    if( (head_ptr == NULL) || (cur_node == NULL))
        next_node = NULL;
    else if(cur_node->next != head_ptr)
        next_node = cur_node->next;
    else 
        next_node = NULL;

    return((strTrsLinkInfo*)next_node);
}


strStepLinkInfo *stepLinkStartMemInfoGet(uint32 stepId)
{
	if( (stepId == 0) || (stepId >MAX_STEP_NUM) )
		return(NULL);
	else  
		return((strStepLinkInfo *)sStepLinkQMngrInfo.usedList[stepId - 1]);	
}



strStepLinkInfo *stepLinkNextMemInfoGet(uint32 stepId, strStepLinkInfo *dataPtr)
{
	strDoubleLink *head_ptr = (strDoubleLink *)sStepLinkQMngrInfo.usedList[stepId - 1];
    strDoubleLink *cur_node = (strDoubleLink *)dataPtr;
    strDoubleLink *next_node = NULL;

    if( (head_ptr == NULL) || (cur_node == NULL))
        next_node = NULL;
    else if(cur_node->next != head_ptr)
        next_node = cur_node->next;
    else 
        next_node = NULL;

    return((strStepLinkInfo *)next_node);
}


uint32 sfcActionControlSet(uint32 actionId, uint8 flagType, uint8 flagData)
{
	uint32 status = NO_ERROR;
	if((actionId<1)||(actionId>MAX_ACTION_NUM))
		status = L_ACTION_ID_ERR;
	else{
		switch(flagType){
		case AF_NONSTROE:
			sSfcActionFlagInfo[actionId-1].Nonstore = flagData;
			break;
		case AF_RESET:
			sSfcActionFlagInfo[actionId-1].Reset = flagData;
			break;
		case AF_SET:
			sSfcActionFlagInfo[actionId-1].Set = flagData;
			break;
		case AF_PULSE:
			sSfcActionFlagInfo[actionId-1].Pulse = flagData;
			break;

		
		case AF_DELAY:
			sSfcActionFlagInfo[actionId-1].Delay = flagData;
			break;
		case AF_LIMIT:
			sSfcActionFlagInfo[actionId-1].Limit = flagData;
			break;
		case AF_SET_DELAY:
			sSfcActionFlagInfo[actionId-1].SetDelay = flagData;
			break;
		case AF_SET_LIMIT:
			sSfcActionFlagInfo[actionId-1].SetLimit = flagData;
			break;
		case AF_DELAY_SET:
			sSfcActionFlagInfo[actionId-1].DelaySet = flagData;
			break;
		default:
			status = SFC_ACTION_FLAG_ID_ERR;
			break;
		}
	}

	
	if(status == NO_ERROR)
	{

	}

	return(status);
}


uint32	sfcMemPointerGet(uint32 memType, strSfcMemPointerInfo	*memInfoPtr)
{
	uint32	status = NO_ERROR, bitMask, offSet;
	strSysConfigInfo	sysCnfgInfo;
	
    switch(memType) {
  		case LOCAL_MEM_ACC:
 		memInfoPtr->stepMemPtr = (uint32)gSFCStepInfoLocalPtr;
		memInfoPtr->trsMemPtr = (uint32)gSFCTransitionLocalPtr;
		memInfoPtr->actMemPtr = (uint32)gSFCActionInfoLocalPtr;
		break;
		 
		case BUS_MEM_ACC:
 		memInfoPtr->stepMemPtr = (uint32)gSFCStepInfoBusPtr;
		memInfoPtr->trsMemPtr = (uint32)gSFCTransitionBusPtr;
		memInfoPtr->actMemPtr = (uint32)gSFCActionInfoBusPtr;
		break;
	
		case RMT_MEM_ACC:
	    
		systemCnfgInfoRead(&sysCnfgInfo);

		if(sysCnfgInfo.shelfType == MASTER_SHELF)
       		offSet = MEM_BOARD_BUS_ADDR_ON_SLAVE;
		else if(sysCnfgInfo.shelfType == SLAVE_SHELF) 
       		offSet = MEM_BOARD_BUS_ADDR_ON_MASTER;
		else {
			status = STATION_SHELF_CONFIG_ERR;
			setErrorCode(	(uint8 *)__FILE__,
        					__LINE__,
							(uint8 *)"sfcMemPointerGet", 
							status
						  );
			return(status);
		}
		bitMask = 0x00ffffff;
 		memInfoPtr->stepMemPtr= (uint32)gSFCStepInfoBusPtr;
		memInfoPtr->stepMemPtr &= bitMask;
		memInfoPtr->stepMemPtr += offSet;
		
		memInfoPtr->trsMemPtr = (uint32)gSFCTransitionBusPtr;
		memInfoPtr->trsMemPtr &= bitMask;
		memInfoPtr->trsMemPtr += offSet;

		memInfoPtr->actMemPtr = (uint32)gSFCActionInfoBusPtr;
		memInfoPtr->actMemPtr &= bitMask;
		memInfoPtr->actMemPtr += offSet;

		break;

		default:
		status = MEM_ACC_TYPE_ERR;
		break;
	}

    return(status);
}


uint32 sfcLoopStatusWrite(uint32 taskId,uint32 trsActId,
						  uint32 usingType, uint32 runStatus) 
{
	uint32 status = NO_ERROR;
	strTransitionInfo *trsInfoPtr;
	strActionInfo *actInfoPtr;
	strActionCtrlInfo *actCtrlInfoPtr;

	if((runStatus == FB_LOOPING_ERR) || (runStatus == NO_ERROR) ) {
		if(usingType == TRANSITION_TYPE) {
			if(trsActId < 1 || trsActId >MAX_TRANSITOIN_NUM) {
				status = L_TRANSITION_ID_ERR;
				setErrorCode(__FILE__,__LINE__,"sfcLoopStatusWrite",status);
				return(status);
			}
		
			trsInfoPtr = gSFCTransitionLocalPtr;
			trsInfoPtr += (trsActId-1);

			if(taskId != trsInfoPtr->taskID) {
				status = L_SEG_ID_ERR;
				setErrorCode(__FILE__,__LINE__,"sfcLoopStatusWrite",status);
				return(status);
			}

			if(trsInfoPtr->transitionID==trsActId){
				trsInfoPtr->trsStatus = runStatus;
			}
			else{
				status = L_TRANSITION_ID_ERR;
				setErrorCode(__FILE__,__LINE__,"sfcLoopStatusWrite",status);
				return(status);
			}
		}
		else if(usingType == ACTION_TYPE) {
			if(trsActId < 1 || trsActId >MAX_ACTION_NUM) {
				status = L_ACTION_ID_ERR;
				setErrorCode(__FILE__,__LINE__,"sfcLoopStatusWrite",status);
				return(status);
			}
			actInfoPtr = gSFCActionInfoLocalPtr;
			actInfoPtr += (trsActId-1);
			if(taskId != actInfoPtr->taskID) {
				status = L_SEG_ID_ERR;
				setErrorCode(__FILE__,__LINE__,"sfcLoopStatusWrite",status);
				return(status);
			}

			if(actInfoPtr->actionID==trsActId){
				actCtrlInfoPtr = (strActionCtrlInfo*)&actInfoPtr->actCtrlData;
				actCtrlInfoPtr->actStatus = runStatus;
			}
			else{
				status = L_ACTION_ID_ERR;
				setErrorCode(__FILE__,__LINE__,"sfcLoopStatusWrite",status);
				return(status);
			}
		}
	}

    return(status);
}




static void sfcStepInfoDisplay(uint32 stepId)
{
	strStepInfo *stepInfoPtr;
	char* strStepType[3] = { "NOMAL STEP","INIT STEP","NONE"};

	uint32 i;


	if(stepId<1 || stepId >MAX_STEP_NUM) {
		printf( "Invalid Step Id!!!\n");
		return;
	}
	stepInfoPtr = gSFCStepInfoLocalPtr;

	stepInfoPtr += (stepId - 1);
	printf( "\n");
	printf( "\t*********** << STEP%3d >> ************\n", stepId);
	if(stepId != stepInfoPtr->stepID ) {
		printf( "\t\n Invalid Step ID\n");
		return;
	}
	printf( "\t Sequencer Task ID : %d\n",stepInfoPtr->taskID );

	printf( "\t Step Type : %s\n",strStepType[stepInfoPtr->initStep] );

	printf( "\t Previous Transion Num : %d\n",stepInfoPtr->preTrNum );
	for(i=0;i<stepInfoPtr->preTrNum;i++)
	{
		printf( "\t      * Previous %dth TR : [T%03d]\n",i+1,stepInfoPtr->preTrList[i]);
	}

	printf( "\t Next Transion Num : %d\n",stepInfoPtr->nextTrNum );
	for(i=0;i<stepInfoPtr->nextTrNum;i++)
	{
		printf( "\t      * Next %dth TR : [T%03d]\n",i+1,stepInfoPtr->nextTrList[i]);
	}

	printf( "\t Link Action Num : %d\n",stepInfoPtr->linkActionNum );
	for(i=0;i<stepInfoPtr->linkActionNum;i++)
	{
		printf( "\t      * Link %dth ACT : [A%03d]",i+1, stepInfoPtr->actionList[i]);
		printf( " - %s", strActFlag[stepInfoPtr->actionFlag[i]]);
		if(stepInfoPtr->actionFlag[i] >= AF_LIMIT && stepInfoPtr->actionFlag[i] <= MAX_ACTION_FLAG)
		{
			if(stepInfoPtr->actionFlag[i] != AF_PULSE )
				printf( " (T#%4d[ms])\n", stepInfoPtr->actionTime[i]);
		}
		else 	printf( "\n");
	}
}



uint8 *inputTypeStr[] = {
	(uint8 *)"ADDRESS",(uint8 *)"VALUE", (uint8 *)"NONE"
};

uint8 *memTypeStr[] = {
	(uint8 *)"NONE",(uint8 *)"FLAG(%M)", (uint8 *)"AUTO(%A)", (uint8 *)"INPUT(%I)",
	(uint8 *)"OUTPUT(%Q)", (uint8 *)"SHARED(%S)", (uint8 *)"FB/FU(%F)", (uint8 *)"TMTC(%T)",
	(uint8 *)"FIELDBUS(%P)", (uint8 *)"NONE"
};

uint8 *dataSizeStr[] = {
	(uint8 *)"NOBIT",(uint8 *)"BIT(X)", (uint8 *)"BYTE(X)", (uint8 *)"2 BYTE(W)",
	(uint8 *)"4 BYTE(D)", (uint8 *)"8 BYTE(L)",
	(uint8 *)"NONE"
};
uint8 *dataTypeStr[] = {
	(uint8 *)"NONE",(uint8 *)"INT32", (uint8 *)"UINT32", (uint8 *)"INT64",
	(uint8 *)"UINT64", (uint8 *)"REAL32", (uint8 *)"REAL64",
	(uint8 *)"NONE"
};
char* strStatus[3] = { "FALSE","TRUE ","NULL "};

static void sfcTransitionInfoDisplay(uint32 trsId)
{
	strTransitionInfo *trsInfoPtr;
	strFbSpecActionInfo *specActionData;
	uint32 i;


	if( trsId<1 ||  trsId >MAX_TRANSITOIN_NUM){
		printf( "Invalid Transition Id!!!\n");
		return;
	}

	trsInfoPtr = gSFCTransitionLocalPtr;
	trsInfoPtr += (trsId -1);
	printf( "\n");
	printf( "\t*********** << TRANSTOIN%3d >> ************\n", trsId);

	if(trsId != trsInfoPtr->transitionID ) {
		printf( "\t\n Invalid Transtion ID\n");
		return;
	}
	printf( "\t Sequencer Task ID : %d\n",trsInfoPtr->taskID );

	printf( "\t Previous Step Num : %d\n",trsInfoPtr->preStepNum);
	for(i=0;i<trsInfoPtr->preStepNum;i++)
	{
		printf( "\t      * Previous %dth STEP : [S%03d]\n",i+1,trsInfoPtr->preStepList[i]);
	}

	printf( "\t Next Step Num : %d\n",trsInfoPtr->NextStepNum);
	for(i=0;i<trsInfoPtr->NextStepNum;i++)
	{
		printf( "\t      * Next %dth STEP : [S%03d]\n",i+1,trsInfoPtr->nextStepList[i]);
	}
	
	specActionData = (strFbSpecActionInfo*)&trsInfoPtr->setActiveHeader;
	printf( "\t Set Active Data Status : %s\n",strStatus[trsInfoPtr->activeStatus]);
	printf( "\t      * Input Ref Type     : %s\n", inputTypeStr[specActionData->refType]);
	printf( "\t      * Negate Set Flag    : %3d\n",specActionData->negated);
	printf( "\t      * Referrence Type    : %s\n", memTypeStr[specActionData->memoryType]);
	printf( "\t      * Input Data Type    : %s\n", dataTypeStr[specActionData->dataType]);
	printf( "\t      * Bit Data Size      : %s\n", dataSizeStr[specActionData->dataSize]);
	
	printf( "\t      * Bit Position Flag  : %3d\n", specActionData->bitposFlag);
	printf( "\t      * Bit Position(0-63) : %3d\n", specActionData->bitPosition);
	printf( "\t      * Task ID            : %3d\n", specActionData->taskID);

	printf( "\n\t      * Address Data       : %3d\n", (trsInfoPtr->setActiveData >> 8) & 0xffffff );
	printf( "\t      * Index Data         : %3d\n", (trsInfoPtr->setActiveData >> 2) & 0x3f );

}


static void sfcActionInfoDisplay(uint32 actId)
{
	strActionInfo	 *actInfoPtr;
	strActionCtrlInfo actCtrlInfo;
	strFbSpecActionInfo *specActionData;
	
	if( actId<1 || actId >MAX_ACTION_NUM) 
	{
		printf( "Invalid Action Id!!!\n");
		return;
	}
	actInfoPtr = gSFCActionInfoLocalPtr;
	actInfoPtr += (actId -1);
	printf( "\n");
	printf( "\t*********** << ACTOIN%3d >> ************\n", actId);

	if(actId != actInfoPtr->actionID ) {
		printf( "\t\n Invalid Action ID\n");
		return;
	}
	printf( "\t Sequencer Task ID : %d\n",actInfoPtr->taskID );

	specActionData = (strFbSpecActionInfo*)&actInfoPtr->varActiveHeader;
	memoryCopy((uint8*)&actCtrlInfo,(uint8*)&actInfoPtr->actCtrlData,sizeof(strActionCtrlInfo));
	printf( "\t Current Action Status : %s\n",strStatus[actCtrlInfo.actCtrlQ]);

	if(actInfoPtr->varActiveHeader != 0 ) {
		printf( "\t Using Action Varialbe \n");
		printf( "\t      * Input Ref Type     : %s\n", inputTypeStr[specActionData->refType]);
		printf( "\t      * Negate Set Flag    : %3d\n",specActionData->negated);
		printf( "\t      * Referrence Type    : %s\n", memTypeStr[specActionData->memoryType]);
		printf( "\t      * Input Data Type    : %s\n", dataTypeStr[specActionData->dataType]);
		printf( "\t      * Bit Data Size      : %s\n", dataSizeStr[specActionData->dataSize]);

		printf( "\t      * Bit Position Flag  : %3d\n", specActionData->bitposFlag);
		printf( "\t      * Bit Position(0-63) : %3d\n", specActionData->bitPosition);
		printf( "\t      * Task ID            : %3d\n", specActionData->taskID);

		printf( "\n");
		printf( "\t      * Address Data       : %3d\n", (actInfoPtr->varActiveData >> 8) & 0xffffff );
		printf( "\t      * Index Data         : %3d\n", (actInfoPtr->varActiveData >> 2) & 0x3f );
	}
	else {
		printf( "\t Using Action FBD \n");
	}


}


static void sfcSequencerLinkQueueDisplay(uint32 taskId)
{
   int vertexNum, nodeNum;
	uint32 status = NO_ERROR;
	strSfcSequcerInfo *sfcExecIdInfoPtr;
	strSfcAdjacentInfo *nodePtr;
	strStepLinkInfo *stepLinkInfoPtr;

	strFbTaskTblMemInfo segTblInfo;

	printf( "\n");
	printf( "\t*********** << TASK_ID #%03d >> ************\n",taskId);
	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
		printf( " Segment ID Exceed!!\n");
		return;	
	}
	status = getFbTaskTblInfo	( 
								LOGIC_ID,
           						taskId,
								(strFbTaskTblMemInfo	*) &segTblInfo
							);
	if(status != NO_ERROR) return;
	
	if(segTblInfo.logicType != SFC_LOGIC_TYPE) 
	{
		printf( "%d SegId is not SFC TYPE\n",taskId);
		return;	
	}

	
	printf( "\tNext Link Queue Display \n");
	vertexNum = 0;

	
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR) {
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
		return;
	}
	sfcExecIdInfoPtr = sfcSequencerStartMemInfoGet(taskId);
	while(sfcExecIdInfoPtr!=NULL) 
	{
		if(sfcExecIdInfoPtr->nodeType == STEP_TYPE) {
			vertexNum = (sfcExecIdInfoPtr->stepTrId -1);
		}
		else if(sfcExecIdInfoPtr->nodeType == TRANSITION_TYPE) {
			vertexNum = MAX_STEP_NUM + (sfcExecIdInfoPtr->stepTrId -1);
		}

		printf("\t * [%s:%2d] ->", strType[sfcExecIdInfoPtr->nodeType],
									sfcExecIdInfoPtr->stepTrId);

		nodeNum=0;
		nodePtr = (strSfcAdjacentInfo *)sfcExecIdInfoPtr->nextDblink;
		while(nodePtr)
		{
			if(nodePtr->sequencerId != NULL) {
				nodeNum++;
				if(nodeNum>5) printf("\n\t\t       ");

				printf(" [%s:%2d] ", strType[nodePtr->nodeType],
							nodePtr->sequencerId->stepTrId);
				nodePtr = (strSfcAdjacentInfo *)nextNodeRef(  
						(strDoubleLink *)sSfcAdjacentQMngrInfo.nextList[vertexNum],
						(strDoubleLink *)nodePtr);
			}
			else break;

		}
		
		
		sfcExecIdInfoPtr = sfcSequencerNextMemInfoGet(taskId, sfcExecIdInfoPtr);
		printf( "\n");
	}
	printf( "\n");

	
	printf( "\tPrevious Link Queue Display \n");
	vertexNum = 0;
	sfcExecIdInfoPtr = sfcSequencerStartMemInfoGet(taskId);
	while(sfcExecIdInfoPtr!=NULL) 
	{
		if(sfcExecIdInfoPtr->nodeType == STEP_TYPE) {
			vertexNum = (sfcExecIdInfoPtr->stepTrId -1);
		}
		else if(sfcExecIdInfoPtr->nodeType == TRANSITION_TYPE) {
			vertexNum = MAX_STEP_NUM + (sfcExecIdInfoPtr->stepTrId -1);
		}

		printf("\t * [%s:%2d] <-", strType[sfcExecIdInfoPtr->nodeType],
									sfcExecIdInfoPtr->stepTrId);

		nodeNum=0;
		nodePtr = (strSfcAdjacentInfo *)sfcExecIdInfoPtr->prevDblink;
		while(nodePtr)
		{
			if(nodePtr->sequencerId != NULL) {
				nodeNum++;
				if(nodeNum>5) printf("\n\t\t       ");

				printf(" [%s:%2d] ",strType[nodePtr->nodeType],
							nodePtr->sequencerId->stepTrId);
				nodePtr = (strSfcAdjacentInfo *)nextNodeRef(  
					(strDoubleLink *)sSfcAdjacentQMngrInfo.prevList[vertexNum],
					(strDoubleLink *)nodePtr);
			}
			else break;
		}
		
		sfcExecIdInfoPtr = sfcSequencerNextMemInfoGet(taskId, sfcExecIdInfoPtr);
		printf( "\n");
	}
	printf( "\n");

	
	printf( "\tLinked Action Queue Display \n");
	vertexNum = 0;
	sfcExecIdInfoPtr = sfcSequencerStartMemInfoGet(taskId);
	while(sfcExecIdInfoPtr!=NULL) 
	{
		if(sfcExecIdInfoPtr->nodeType == STEP_TYPE)
		{
			vertexNum = (sfcExecIdInfoPtr->stepTrId -1);
			printf("\t * [%s:%2d] -|", strType[sfcExecIdInfoPtr->nodeType],
										sfcExecIdInfoPtr->stepTrId);
			nodeNum = 0;
			stepLinkInfoPtr = stepLinkStartMemInfoGet(sfcExecIdInfoPtr->stepTrId);
			while(stepLinkInfoPtr!=NULL)
			{
				nodeNum++;
				if(nodeNum>4) printf("\n\t\t       ");
				printf( " [AT:%2d(%s)]",stepLinkInfoPtr->actionId,
										strActFlag[stepLinkInfoPtr->actionFlag]);
				stepLinkInfoPtr = stepLinkNextMemInfoGet(sfcExecIdInfoPtr->stepTrId, stepLinkInfoPtr);
			}
			printf( "\n");
		}
		
		sfcExecIdInfoPtr = sfcSequencerNextMemInfoGet(taskId, sfcExecIdInfoPtr);
	}
	printf( "\n");
	
#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif
}


static void sfcRunningSequencerInfoDisplay(uint32 taskId)
{
	uint32 status = NO_ERROR;
	strSfcSequcerInfo *sfcExecIdInfoPtr;
	strFbTaskTblMemInfo segTblInfo;

	strStepInfo *stepInfoPtr;
	strTransitionInfo *trsInfoPtr;
	strActionInfo *actInfoPtr;
	strActionCtrlInfo *actCtrlInfoPtr;
	strSfcMemPointerInfo sfcMemPtr;

	strSfcExecutionInfo *curDataPtr,* curLinkPtr, *debugExecList = NULL;
	strSfcAdjacentInfo *nodePtr;
	uint32 visitFlag[MAX_STEP_NUM+MAX_TRANSITOIN_NUM], visitIndex;
	uint32 debugStepId[MAX_STEP_NUM], debugStepNum = 0;
	uint32 i,j,k;
	char* strCurStatus[3] = { " ","*","N"};

	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
		printf( " Segment ID Exceed!!\n");
		return;	
	}
	status = getFbTaskTblInfo	(   LOGIC_ID,	taskId,
								(strFbTaskTblMemInfo	*) &segTblInfo
						 	);
	if(status != NO_ERROR) return;
	sActCurDbgData.curSegId = taskId;

	
	if(segTblInfo.logicType != SFC_LOGIC_TYPE) {
		printf( "%d SegId is not SFC TYPE\n",taskId);
		return;	
	}

	for(i=0; i<MAX_STEP_NUM+MAX_TRANSITOIN_NUM;i++) {
		visitFlag[i]=0;
	}
	memoryClear( (uint8 *) &sfcMemPtr, sizeof(strSfcMemPointerInfo));
	status = sfcMemPointerGet(LOCAL_MEM_ACC, &sfcMemPtr);

 	
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR) {
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
		return;
	}

	sfcExecIdInfoPtr = sfcSequencerStartMemInfoGet(taskId);
	while(sfcExecIdInfoPtr!=NULL) 
	{
		if(sfcExecIdInfoPtr->nodeType == STEP_TYPE) 
			visitIndex = sfcExecIdInfoPtr->stepTrId - 1;
		else visitIndex = MAX_STEP_NUM + sfcExecIdInfoPtr->stepTrId - 1;

		
		if (visitFlag[visitIndex] == INACTIVE_FLAG) 
		{
			curDataPtr = (strSfcExecutionInfo *)nodeGetAndUnlink (
							(strDoubleLink **)&sSfcExecQMngrInfo.freeList
            										);
			curDataPtr->sequencerId = sfcExecIdInfoPtr;
			
			status = nodePutOnLast( (strDoubleLink **) ((void*)&debugExecList),
             		            		(strDoubleLink *)curDataPtr
                		       		  );
			if(status) {
				setErrorCode( __FILE__, __LINE__, "sfcRunningSequencerInfoDisplay",status);
				return;	
			}
			
			visitFlag[visitIndex] = ACTIVE_FLAG;	
			
			 
			while(debugExecList!=NULL)
			{
				
				curLinkPtr = (strSfcExecutionInfo *)nodeGetAndUnlink
					((strDoubleLink **) ((void*)&debugExecList));
				status = nodePutOnLast( (strDoubleLink **) &sSfcExecQMngrInfo.freeList,
             		            		(strDoubleLink *)curLinkPtr
                		       		  );

				nodePtr = (strSfcAdjacentInfo *)curLinkPtr->sequencerId->nextDblink;
				
				if(curLinkPtr->sequencerId->nodeType == STEP_TYPE) {
					debugStepId[debugStepNum++] = curLinkPtr->sequencerId->stepTrId;
				}

				
				while(nodePtr)
				{
					
					if(nodePtr->sequencerId != NULL) {
						if(nodePtr->sequencerId->nodeType == STEP_TYPE)  
							visitIndex = nodePtr->sequencerId->stepTrId - 1 ;
						else visitIndex = MAX_STEP_NUM + nodePtr->sequencerId->stepTrId - 1;

						if (visitFlag[visitIndex] == INACTIVE_FLAG)
						{
							
							curDataPtr = (strSfcExecutionInfo *)nodeGetAndUnlink (
										 (strDoubleLink **)&sSfcExecQMngrInfo.freeList
            							 );
							curDataPtr->sequencerId = nodePtr->sequencerId;

							nodePutOnLast( (strDoubleLink **) ((void*)&debugExecList),
             		            			(strDoubleLink *)curDataPtr
                		       			  );
							visitFlag[visitIndex] = ACTIVE_FLAG;	
						}
					}
					nodePtr = (strSfcAdjacentInfo *)nextNodeRef( 
						(strDoubleLink *)curLinkPtr->sequencerId->nextDblink,
						(strDoubleLink *)nodePtr);
				}

			}
		}
		
		sfcExecIdInfoPtr = sfcSequencerNextMemInfoGet(taskId, sfcExecIdInfoPtr);
	}
#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif

	screenClear();
    while(!keyHit()) {
    	cursorPositionSet();
		printf( " *********************** << TASK_ID #%03d >> *********************** \n",taskId);
		printf( "  * [STEP_ID#](*/ ) -| [ACT_ID#(FLAG)](*/ ) -> [TRS_ID#](*/ ) \n");
		printf( "  * Initial Step ID#: %03d            * Total Step Number: %d  \n",
								debugStepId[0], debugStepNum);
		printf( " ****************************************************************** \n");

		if(debugStepNum > MAX_STEP_NUM) {
			printf( "\nCurrent Task Step Number is Exceeded(%d)!!", debugStepNum);
			break;
		}


		for(i=0;i<debugStepNum; i++){
			stepInfoPtr = (strStepInfo*)sfcMemPtr.stepMemPtr;
			stepInfoPtr += (debugStepId[i] - 1);

			printf("  [S%03d]%s -|", 
									debugStepId[i], strCurStatus[stepInfoPtr->X]);

			if( (stepInfoPtr->linkActionNum > MAX_ACTION_NUM)) {
				printf( "\nLinked Action Number is Exceeded(%d)!!", stepInfoPtr->linkActionNum);
				break;
			}

			for(j=0;j<stepInfoPtr->linkActionNum; j++){
				if((j+1)%5==0) printf("\n\t    ");
			
				actInfoPtr = (strActionInfo*)sfcMemPtr.actMemPtr;
				actInfoPtr += (stepInfoPtr->actionList[j] - 1);
				actCtrlInfoPtr = (strActionCtrlInfo *)&actInfoPtr->actCtrlData;
				printf( " [A%03d(%s)]%s",actInfoPtr->actionID,
													strActFlag[stepInfoPtr->actionFlag[j]]
													,strCurStatus[actCtrlInfoPtr->actCtrlQ]);
			}

			if( (stepInfoPtr->nextTrNum > MAX_TRANSITOIN_NUM)) {
				printf( "\nNext Transition Number is Exceeded(%d)!!", stepInfoPtr->nextTrNum);
				break;
			}

			printf(" ->");
			for(k=0;k<stepInfoPtr->nextTrNum; k++){
				if((j+k+1)%5==0) printf("\n\t    ");
				trsInfoPtr = (strTransitionInfo*)sfcMemPtr.trsMemPtr;
				trsInfoPtr += (stepInfoPtr->nextTrList[k] - 1);
				printf( " [T%03d]%s",trsInfoPtr->transitionID,
													strCurStatus[trsInfoPtr->activeStatus]);
			}

			printf( "\n");
		}

		printf( " ================================================================== \n");
		printf( "  * Current Running Action Queue Data(%d)\n",sActCurDbgData.curActNum);
		lineClear();

		for(i=0;i<sActCurDbgData.curActNum; i++){
			if(sActCurDbgData.curActNum>MAX_ACTION_NUM) break;
			printf( "  [A%03d]", sActCurDbgData.curActId[i]);
			if((i+1)%7==0) {
				printf("\n");
				lineClear();
			}
		}

		sysTaskDelay(systemClockRateGet()/2);
	}

	sActCurDbgData.curSegId = 0;
}


static void sfcRunningStepInfoDisplay(uint32 stepId)
{
	strStepInfo *stepInfoPtr;
	strTransitionInfo *trsInfoPtr;
	strActionInfo *actInfoPtr;
	strActionCtrlInfo actCtrlData;

	uint32 factor = 2, i;

	if( stepId<1 || stepId >MAX_STEP_NUM) 
	{
		printf( "Invalid Step Id!!!\n");
		return;
	}
	stepInfoPtr = gSFCStepInfoLocalPtr;
	stepInfoPtr += (stepId - 1);

	if(stepInfoPtr->stepID !=stepId || stepInfoPtr->taskID ==0)
	{
		printf( "Not Used Step!!!\n");
		return;
	}
    screenClear();

    while(!keyHit()) {
    	cursorPositionSet();
	    printf( "****************** << STEP[%2d] - SEGID[%d] >> ******************\n",
    							stepId, stepInfoPtr->taskID
				);
	
		printf( "   * Num Of Prev TR : %3d", stepInfoPtr->preTrNum);
		printf( "   * Num Of Next TR : %3d\n", stepInfoPtr->nextTrNum);
		printf( "   * Num Of Link AT : %3d", stepInfoPtr->linkActionNum);
		printf( "   * Init Step Type : %3d\n", stepInfoPtr->initStep);

	    printf( "*****************************************************************\n");
		
		
		for(i = 0; i < stepInfoPtr->preTrNum; i++) {
			printf("[PTR%03d]", i+1);

			trsInfoPtr = gSFCTransitionLocalPtr;
			trsInfoPtr += (stepInfoPtr->preTrList[i] -1);
			
			printf("\t   %d",trsInfoPtr->activeStatus);
			printf("\t\t");

            
			if(!((i+1)%factor)) printf("\n");
        }	
		if( ((stepInfoPtr->preTrNum > factor) && (stepInfoPtr->preTrNum % factor)) ||
        	 (stepInfoPtr->preTrNum < factor)
		  )
		{
           	printf( "\n=================================================================\n");
		} else {
	       	printf(   "=================================================================\n");
		}

		
		for(i = 0; i < stepInfoPtr->nextTrNum; i++) {
			printf("[NTR%03d]", stepInfoPtr->nextTrList[i]);

			trsInfoPtr = gSFCTransitionLocalPtr;
			trsInfoPtr += (stepInfoPtr->nextTrList[i] -1);
			
			printf("\t   %d",trsInfoPtr->activeStatus);
			printf("\t\t");

            
			if(!((i+1)%factor)) printf("\n");
        }	
		if( ((stepInfoPtr->nextTrNum > factor) && (stepInfoPtr->nextTrNum % factor)) ||
        	 (stepInfoPtr->nextTrNum< factor)
		  )
		{
           	printf( "\n=================================================================\n");
		} else {
	       	printf(   "=================================================================\n");
		}

		
		for(i = 0; i < stepInfoPtr->linkActionNum; i++) {
			printf("[ACT%03d]", stepInfoPtr->actionList[i]);


			actInfoPtr = gSFCActionInfoLocalPtr;
			actInfoPtr += (stepInfoPtr->actionList[i] -1);
			memoryCopy((uint8*) &actCtrlData,(uint8*)&actInfoPtr->actCtrlData, sizeof(strActionCtrlInfo));
			printf("\t %d",actCtrlData.actCtrlQ);
			printf("(%s",	strActFlag[stepInfoPtr->actionFlag[i]]);

			if(stepInfoPtr->actionFlag[i] >= AF_DELAY && stepInfoPtr->actionFlag[i] <= AF_DELAY_SET)
			{
				printf( "-T#%4d[ms])    ", stepInfoPtr->actionTime[i]);
			}
			else printf(")    \t");

            
			if(!((i+1)%factor)) printf("\n");
        }	

		if( ((stepInfoPtr->linkActionNum > factor) && (stepInfoPtr->linkActionNum % factor)) ||
        	 (stepInfoPtr->linkActionNum< factor)
		  )
		{
           	printf( "\n=================================================================\n");
		} else {
	       	printf(   "=================================================================\n");
		}

		
		printf("[S%03d.X]", stepInfoPtr->stepID);
		printf("\t   %1d",stepInfoPtr->X);
		printf("\t\t");

		printf("[S%03d.T]", stepInfoPtr->stepID);
		printf("\t   %7d[ms]",stepInfoPtr->T);
      	printf(   "\n=================================================================\n");

		sysTaskDelay(systemClockRateGet()/2);
	}

}



static void sfcRunningActionInfoDisplay(uint32 actId)
{
	strActionInfo *actInfoPtr;
	strActionCtrlInfo actCtrlInfo;

	if( actId<1 || actId >MAX_ACTION_NUM) 
	{
		printf( "Invalid Action Id!!!\n");
		return;
	}
	actInfoPtr = gSFCActionInfoLocalPtr;
	actInfoPtr += (actId - 1);

	if(actInfoPtr->actionID !=actId || actInfoPtr->taskID ==0)
	{
		printf( "Not Used Action!!!\n");
		return;
	}
    screenClear();

    while(!keyHit()) {
    	cursorPositionSet();
	    printf( "*************** << ACTION[%2d] : TASK_ID #[%03d]>> ***************\n",
    							actId, actInfoPtr->taskID);
		printf( "   * Var SetAction Header  : 0x%08x \n", actInfoPtr->varActiveHeader);
		printf( "   * Var SetAction Data    : 0x%08x \n", actInfoPtr->varActiveData);
		printf( "*****************************************************************\n");

		memoryCopy((uint8*)&actCtrlInfo,(uint8*)&actInfoPtr->actCtrlData,sizeof(strActionCtrlInfo));
      	printf(   "=================================================================\n");
		printf( "   * Action Ctrl Data	   : 0x%08x \n", actInfoPtr->actCtrlData);
		printf( "   * Current Action Status : %s\n",strStatus[actCtrlInfo.actCtrlQ]);

      	printf(   "=================================================================\n");
		printf( "   * NonStore(N)  : %x\n",actCtrlInfo.Nonstore);
		printf( "   * Reset(R)     : %x\n",actCtrlInfo.Reset);
		printf( "   * Set(S)       : %x (RS F/F:%x)\n",
														actCtrlInfo.Set, actCtrlInfo.Set_RS_Q);
		printf( "   * Pulse(P)     : %x\n",actCtrlInfo.Pulse);
		printf( "   * Delay(D)     : %x\n",actCtrlInfo.Delay);
		printf( "   * Limit(L)     : %x\n",actCtrlInfo.Limit);
		printf( "   * SetDelay(SD) : %x (RS F/F:%x)\n"
														,actCtrlInfo.SetDelay,actCtrlInfo.SD_RS_Q);
		printf( "   * SetLimit(SL) : %x (RS F/F:%x)\n",
														actCtrlInfo.SetLimit,actCtrlInfo.SL_RS_Q);
		printf( "   * DelaySet(DS) : %x (RS F/F:%x)\n",
													actCtrlInfo.DelaySet,actCtrlInfo.DS_RS_Q);
		printf( "   * Timer Memory : %x\n", actCtrlInfo.Timer_Mem);
		printf( "   * Pulse Memory : %x\n", actCtrlInfo
			.Pulse_Mem);

		
      	printf( "=================================================================\n");
		printf( "   * Set Current Time Flag : %4s\n",strActFlag[actCtrlInfo.ActiveTFlag]);
		printf( "   * Set Current Time : %4d[mSec]\n",actInfoPtr->setCurTime);
		printf( "   * Action Elapsed Time : %4d[mSec]\n",actInfoPtr->elapsedTime);

      	printf( "=================================================================\n");
	    printf( "\n");
		sysTaskDelay(systemClockRateGet()/2);
	}

}

static void sfcRunningStepMemDisplay(void)
{
	uint32 startStepId, endStepId;
	uint32	status,  index, i;
	int8	line[16], *cPtr;
	strSfcMemPointerInfo sfcMemPtr;
	strStepInfo *stepInfoPtr;
	char* strStepX[3] = {" ", "*", "N"};
	uint32 runStepNum = 0;

	printf("\tStart Step ID (1-%d) : ", MAX_STEP_NUM);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    startStepId = atoi((const char *)cPtr);

    printf("\tEnd Step ID (%d-%d)  : ",startStepId+1,MAX_STEP_NUM);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    endStepId = atoi((const char *)cPtr);


	if( ((endStepId - startStepId) <= 0) ||
    	(startStepId >= endStepId) ||  (startStepId < 1) ||
        (endStepId > MAX_STEP_NUM)
	) {
    	printf( "\n");
		printf( "\t[ERROR] - Invalid Step Id (Start : %d,End : %d)\n",
        				startStepId, endStepId);
		return;
    }

	screenClear();
    while(!keyHit()) {
    	cursorPositionSet();
		
	    printf( " ********************** << Run Time STEP STATUS/TIME >> *********************\n");
		printf( "   * Start STEP ID# : [#S%03d], End STEP ID# : [#S%03d]\n",startStepId, endStepId);
		printf( "   * [STEP_ID#]STEP.X:STEP.T (STATUS:ELAPSED TIME[SEC])\n");
	    printf( " ****************************************************************************\n");
 
		
		printf( " +====+=========+====+=========+====+=========+====+=========+====+=========+\n");
		printf( " | ID |E_Time[s]| ID |E_Time[s]| ID |E_Time[s]| ID |E_Time[s]| ID |E_Time[s]|\n");
	    printf( " +====+=========+====+=========+====+=========+====+=========+====+=========+\n");
		
		index = 1;
		runStepNum = 0;
		for(i = startStepId; i <= endStepId; i++) {
			memoryClear( (uint8 *) &sfcMemPtr, sizeof(strSfcMemPointerInfo));

			status = sfcMemPointerGet(LOCAL_MEM_ACC, &sfcMemPtr);
			if(status != NO_ERROR) {
				printf("[ERROR] - sfcMemPointerGet() Call Err !!\n");
				return;
			}
			stepInfoPtr = (strStepInfo*)sfcMemPtr.stepMemPtr;
			stepInfoPtr += (i - 1);

			printf(" [S%03d]", i);
  
			if(stepInfoPtr->stepID != 0) {
				printf("%1s:", strStepX[stepInfoPtr->X] );
				if((stepInfoPtr->T/1000) < 1000) {
					printf("%03d", (stepInfoPtr->T/1000) );
					printf(".%02d", (stepInfoPtr->T%1000)/10 );
				}
				else {
					printf("%04d", (stepInfoPtr->T/1000) );
					printf(".%01d", (stepInfoPtr->T%1000)/100 );
				}

			}
  			else
				printf("        ");

			if( !(index % 5)) printf( "\n");
			index++;
			if(stepInfoPtr->X == 1) runStepNum++;
				
		}
		if(((index-1) % 5)) printf( "\n");
		printf( " +====+=========+====+=========+====+=========+====+=========+====+=========+\n");
		printf( "   * Current Running STEP Num : %d", runStepNum);
	
		sysTaskDelay(systemClockRateGet()/2);
	}
}


static void sfcRunningTrsMemDisplay(void)
{
	uint32 startTrsId, endTrsId;
	uint32	status,  index, i;
	int8	line[16], *cPtr;
	strSfcMemPointerInfo sfcMemPtr;
	strTransitionInfo *trsInfoPtr;
	char* strTrsStatus[3] = {" ", "*", "N"};
	uint32 runTrsNum = 0;

	printf("\tStart TRANSITION ID (1-%d) : ", MAX_TRANSITOIN_NUM);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    startTrsId = atoi((const char *)cPtr);

    printf("\tEnd TRANSITION ID (%d-%d)  : ",startTrsId+1,MAX_TRANSITOIN_NUM);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    endTrsId = atoi((const char *)cPtr);


	if( ((endTrsId - startTrsId) <= 0) ||
    	(startTrsId >= endTrsId) ||  (startTrsId < 1) ||
        (endTrsId > MAX_TRANSITOIN_NUM)
	) {
    	printf( "\n");
		printf( "\t[ERROR] - Invalid TRANSITION Id (Start : %d,End : %d)\n",
        				startTrsId, endTrsId);
		return;
    }

	screenClear();
    while(!keyHit()) {
    	cursorPositionSet();
		
	    printf( " ******************** << Run Time TRANSITION STATUS/TIME >> *****************\n");
		printf( "   * Start TRANSISTION ID# : [#S%03d], End TRANSISTION ID# : [#T%03d]\n",startTrsId, endTrsId);
		printf( "   * [TRS_ID#]TRS Status:TRS FB Running Time (ON/OFF : FB Running TIME[mSEC])\n");
	    printf( " ****************************************************************************\n");
 
		
		printf( " +====+=========+====+=========+====+=========+====+=========+====+=========+\n");
		printf( " | ID |RTime[ms]| ID |RTime[ms]| ID |RTime[ms]| ID |RTime[ms]| ID |RTime[ms]|\n");
	    printf( " +====+=========+====+=========+====+=========+====+=========+====+=========+\n");
		
		index = 1;
		runTrsNum = 0;
		for(i = startTrsId; i <= endTrsId; i++) {
			memoryClear( (uint8 *) &sfcMemPtr, sizeof(strSfcMemPointerInfo));

			status = sfcMemPointerGet(LOCAL_MEM_ACC, &sfcMemPtr);
			if(status != NO_ERROR) {
				printf("[ERROR] - sfcMemPointerGet() Call Err !!\n");
				return;
			}
			trsInfoPtr = (strTransitionInfo*)sfcMemPtr.trsMemPtr;
			trsInfoPtr += (i - 1);

			printf(" [T%03d]", i);
  
			if(trsInfoPtr->transitionID == i) {
				printf("%1s:", strTrsStatus[trsInfoPtr->activeStatus] );
				if(gTrsExecTime[i-1].curExecTime < 100.0)
					printf("%06.3f", gTrsExecTime[i-1].curExecTime );
				else 
					printf("%06.2f", gTrsExecTime[i-1].curExecTime );
			}
  			else
				printf("        ");

			if( !(index % 5)) printf( "\n");
			index++;
			if(trsInfoPtr->activeStatus == 1) runTrsNum++;
				
		}
		if( ((index-1) % 5)) printf( "\n");
		printf( " +====+=========+====+=========+====+=========+====+=========+====+=========+\n");
		printf( "   * Current ON TRS Num : %d", runTrsNum);

		sysTaskDelay(systemClockRateGet()/2);
	}
}


static void sfcRunningActMemDisplay(void)
{
	uint32 startActId, endActId;
	uint32	status,  index, i;
	int8	line[16], *cPtr;
	strSfcMemPointerInfo sfcMemPtr;
	strActionInfo *actInfoPtr;
	char* strActStatus[3] = {" ", "*", "N"};
	uint32 runActNum = 0;
	strActionCtrlInfo *actCtrlInfo;

	printf("\tStart ACTION ID (1-%d) : ", MAX_ACTION_NUM);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    startActId = atoi((const char *)cPtr);

    printf("\tEnd ACTION ID (%d-%d)  : ",startActId+1,MAX_ACTION_NUM);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    endActId = atoi((const char *)cPtr);


	if( ((endActId - startActId) <= 0) ||
    	(startActId >= endActId) ||  (startActId < 1) ||
        (endActId > MAX_ACTION_NUM)
	) {
    	printf( "\n");
		printf( "\t[ERROR] - Invalid ACTION Id (Start : %d,End : %d)\n",
        				startActId, endActId);
		return;
    }

	screenClear();
    while(!keyHit()) {
    	cursorPositionSet();
		
	    printf( " ********************** << Run Time ACTION STATUS/TIME >> *******************\n");
		printf( "   * Start ACTION ID# : [#S%03d], End ACTION ID# : [#T%03d]\n",startActId, endActId);
		printf( "   * [ACT_ID#]ACT Status:ACT FB Running Time (ON/OFF : FB Running TIME[mSEC])\n");
	    printf( " ****************************************************************************\n");
 
		
		printf( " +====+=========+====+=========+====+=========+====+=========+====+=========+\n");
		printf( " | ID |RTime[ms]| ID |RTime[ms]| ID |RTime[ms]| ID |RTime[ms]| ID |RTime[ms]|\n");
	    printf( " +====+=========+====+=========+====+=========+====+=========+====+=========+\n");
		
		index = 1;
		runActNum = 0;
		for(i = startActId; i <= endActId; i++) {
			memoryClear( (uint8 *) &sfcMemPtr, sizeof(strSfcMemPointerInfo));

			status = sfcMemPointerGet(LOCAL_MEM_ACC, &sfcMemPtr);
			if(status != NO_ERROR) {
				printf("[ERROR] - sfcMemPointerGet() Call Err !!\n");
				return;
			}
			actInfoPtr = (strActionInfo*)sfcMemPtr.actMemPtr;
			actInfoPtr += (i - 1);

			printf(" [A%03d]", i);

			actCtrlInfo = (strActionCtrlInfo*)&actInfoPtr->actCtrlData;
			if(actInfoPtr->actionID == i) {
				printf("%1s:", strActStatus[actCtrlInfo->actCtrlQ] );
				if(gActExecTime[i-1].curExecTime < 100.0)
					printf("%06.3f", gActExecTime[i-1].curExecTime );
				else 
					printf("%06.2f", gActExecTime[i-1].curExecTime );
			}
  			else
				printf("        ");

			if( !(index % 5)) printf( "\n");
			index++;
			if(actCtrlInfo->actCtrlQ == 1) runActNum++;
				
		}
		if( ((index-1) % 5)) printf( "\n");
		printf( " +====+=========+====+=========+====+=========+====+=========+====+=========+\n");
		printf( "   * Current ON ACT Num : %d", runActNum);

		sysTaskDelay(systemClockRateGet()/2);
	}
}



static void sfcSettingInfoDisplay()
{
	int8 line[16], *pLine;
	uint32 sfcId = 0;

    printf("\n\tEnter the Choice? [1:STEP, 2: TRANSITION, 3:ACTION] ");

    ioRead(gDebugPortFd, line, sizeof(line));
	pLine = line;
	spaceSkip(&pLine);

	switch( *pLine) {
	case '1':
        pLine++;
        if( *pLine != ENDOFSTR) {
	        printf("\n\t[Error]-Unknown Number !!!\n");
        }
		else {
            printf("\n");
            printf("\tEnter The Step Number ? ");
            ioRead(gDebugPortFd,line, sizeof(line));
            pLine = line;
            spaceSkip( &pLine);
		
			sfcId = atoi((const char *)pLine);
			sfcStepInfoDisplay(sfcId);	
		}
    break;
	case '2':
        pLine++;
        if( *pLine != ENDOFSTR) {
	        printf("\n\t[Error]-Unknown Number !!!\n");
        }
		else {
            printf("\n");
            printf("\tEnter The Transtion Number ? ");
            ioRead(gDebugPortFd,line, sizeof(line));
            pLine = line;
            spaceSkip( &pLine);
		
			sfcId = atoi((const char *)pLine);
			sfcTransitionInfoDisplay(sfcId);
		}
    break;
	case '3':
        pLine++;
        if( *pLine != ENDOFSTR) {
	        printf("\n\t[Error]-Unknown Number !!!\n");
        }
		else {
            printf("\n");
            printf("\tEnter The Action Number ? ");
            ioRead(gDebugPortFd,line, sizeof(line));
            pLine = line;
            spaceSkip( &pLine);
		
			sfcId = atoi((const char *)pLine);
			sfcActionInfoDisplay(sfcId);
		}
    break;
    default:
        printf("\n\t[Error]-Unknown Number !!!\n");
	break;
	}

}



static void sfcRunngingMemoryDisplay()
{
	int8 line[16], *pLine;

    printf("\n\tEnter the Choice? [1:STEP, 2: TRANSITION, 3:ACTION] ");

    ioRead(gDebugPortFd, line, sizeof(line));
	pLine = line;
	spaceSkip(&pLine);

	switch( *pLine) {
	case '1':
        pLine++;
        if( *pLine != ENDOFSTR) {
	        printf("\n\t[Error]-Unknown Number !!!\n");
        }
		else {
			sfcRunningStepMemDisplay();
		}
    break;
	case '2':
        pLine++;
        if( *pLine != ENDOFSTR) {
	        printf("\n\t[Error]-Unknown Number !!!\n");
        }
		else {
			sfcRunningTrsMemDisplay();
		}
    break;
	case '3':
        pLine++;
        if( *pLine != ENDOFSTR) {
	        printf("\n\t[Error]-Unknown Number !!!\n");
        }
		else {
			sfcRunningActMemDisplay();
		}
    break;
    default:
        printf("\n\t[Error]-Unknown Number !!!\n");
	break;
	}
}


void sfcDataDisplay()
{
	int8 line[16], *pLine;

    int8 *helpString[] = {
    (int8 *)"*************************************",
    (int8 *)"* 1.SFC Setting Info Display",
	(int8 *)"* 2.SFC Data In Memory Display",
    (int8 *)"* 3.RunTime SFC Data Display",
    (int8 *)"* 4.RunTime STEP Detail Info Display",
    (int8 *)"* 5.RunTime ACTION Detail Info Display",
 	(int8 *)"* 6.Current Running Sequencer Display",
	(int8 *)"* 7.Sequencer Link Queue Display",
	(int8 *)"* 8.SFC FB Code In Memory Display",
	(int8 *)"* 9.SFC Queue info Display",
    (int8 *)"* q.Exit",
    (int8 *)"*************************************",
    NULL
    };
    int8 **pStr;
	uint32	stepId, taskId, actId;

	for(;;){
    	printf( "\n");

		for(pStr = helpString; *pStr != NULL; pStr++)
        	printf( "\t%s\n", *pStr);

        printf("\n\tChoice ? ");

        ioRead(gDebugPortFd, line, sizeof(line));
		pLine = line;
		spaceSkip(&pLine);

		switch( *pLine) {
			case '1':
            	pLine++;
                if( *pLine != ENDOFSTR) {
	            	printf("\n\t[Error]-Unknown Number !!!\n");
                }
				else {
					sfcSettingInfoDisplay();
				}
			break;

			case '2':
            	pLine++;
                if( *pLine != ENDOFSTR) {
	            	printf("\n\t[Error]-Unknown Number !!!\n");
                }
				else {
					sfcDataInMemoryDisplay();
				}
            break;
			case '3':
            	pLine++;
                if( *pLine != ENDOFSTR) {
	            	printf("\n\t[Error]-Unknown Number !!!\n");
                }
				else {
					sfcRunngingMemoryDisplay();
				}
            break;
	
			case '4':
            	pLine++;
                if( *pLine != ENDOFSTR) {
	            	printf("\n\t[Error]-Unknown Number !!!\n");
                }
				else {
            		printf("\n");

            		printf("\tEnter The Step Number ? ");
                	ioRead(gDebugPortFd,line, sizeof(line));
                	pLine = line;
                	spaceSkip( &pLine);
				
					stepId = atoi((const char *)pLine);
					sfcRunningStepInfoDisplay(stepId);
				}
            break;

			case '5':
            	pLine++;
                if( *pLine != ENDOFSTR) {
	            	printf("\n\t[Error]-Unknown Number !!!\n");
                }
				else {
            		printf("\n");
            		printf("\tEnter The Action Number ? ");
                	ioRead(gDebugPortFd,line, sizeof(line));
                	pLine = line;
                	spaceSkip( &pLine);
				
					actId = atoi((const char *)pLine);
					sfcRunningActionInfoDisplay(actId);
				}
            break;

			case '6':
            	pLine++;
                if( *pLine != ENDOFSTR) {
	            	printf("\n\t[Error]-Unknown Number !!!\n");
                }
				else {
            		printf("\n");
            		printf("\tEnter The Segment Number ? ");
                	ioRead(gDebugPortFd,line, sizeof(line));
                	pLine = line;
                	spaceSkip( &pLine);
				
					taskId = atoi((const char *)pLine);
					sfcRunningSequencerInfoDisplay(taskId);
				}
            break;
			case '7':
            	pLine++;
                if( *pLine != ENDOFSTR) {
	            	printf("\n\t[Error]-Unknown Number !!!\n");
                }
				else {
            		printf("\n");
            		printf("\tEnter The Segment Number ? ");
                	ioRead(gDebugPortFd,line, sizeof(line));
                	pLine = line;
                	spaceSkip( &pLine);
				
					taskId = atoi((const char *)pLine);
					sfcSequencerLinkQueueDisplay(taskId);
				}
            break;
			case '8':
			    pLine++;
                if( *pLine != ENDOFSTR) {
	            	printf("\n\t[Error]-Unknown Number !!!\n");
                }
				else {
					sfcFbCodeInMemDisplay();
				}
            break;
			case '9':
			    pLine++;
                if( *pLine != ENDOFSTR) {
	            	printf("\n\t[Error]-Unknown Number !!!\n");
                }
				else {
					sfcQueueCountDisplay();
				}
            break;

            case 'q':case 'Q':
            	goto MODULE_END;
            break;
            default:
            	printf("\n\t[Error]-Unknown Number !!!\n");
			break;
		}		
    }
MODULE_END:
	return;

}



static void sfcFbCodeInMemDisplay()
{
    strFbMemInfo	fbMemInfo;
	uint32	status,  index;
	int8	line[16], *cPtr;
	int32  startBlockAddr, endBlockAddr, i;
	uint32	maxBlockAddr;
	strModuleExecInfo sysExeInfo;
	uint32 sfcType =0 , sfcTypeId =0 , maxId = 0;

	status = fbSysExeModuleInfoRead(LOGIC_ID, &sysExeInfo);
	if(status != NO_ERROR) {
			printf("[ERROR] - fbSysExeModuleInfoRead() Call Err !!\n");
	}

	printf("\tEnter The SFC Sequence Type[0.All 1.TRS 2.ACT ] : ");
	ioRead(gDebugPortFd, line, sizeof(line));
	cPtr = line;
	spaceSkip(&cPtr);

	if(*cPtr==ENDOFSTR) sfcType = 0;
	else sfcType = atoi((const char *)cPtr);

	if(sfcType == TRANSITION_TYPE) {
			status = fbSysExeSfcInfoRead(TRANSITION_TYPE, &maxId);
			printf("\tEnter The Transition Number[1-%d] : ",maxId );
	}
	else if(sfcType == ACTION_TYPE) {
			status = fbSysExeSfcInfoRead(ACTION_TYPE, &maxId);
			printf("\tEnter The Action Number[1-%d] : ",maxId);
	}
	else{
			maxId = sysExeInfo.curMaxSegmentId;
			printf("\tEnter The Task Number[1-%d] : ",maxId );
	}

	ioRead(gDebugPortFd, line, sizeof(line));
	cPtr = line;
	spaceSkip(&cPtr);

	if(*cPtr==ENDOFSTR) sfcTypeId = 1;
	else sfcTypeId = atoi((const char *)cPtr);

	if( (sfcTypeId < 1 ) || (sfcTypeId >maxId ) ) {
    	printf( "\n");
		printf( "\t[ERROR] - Invalid Number : %d(Start : 0,End : %d)\n",
        				sfcTypeId, maxId);
		return;
	}

	maxBlockAddr = sysExeInfo.curfbMaxBlockAddr;

    printf("\tStart Fb Block Addr[1 - %d(Cur MAX)] : ",maxBlockAddr);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

	if(*cPtr==ENDOFSTR)  startBlockAddr = 1;
    else startBlockAddr = atoi((const char *)cPtr);

    printf("\tEnd Fb Block Addr[%d - %d(Cur MAX)] : ",startBlockAddr+1,maxBlockAddr);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);
    printf( "\n");

	if(*cPtr==ENDOFSTR) endBlockAddr = maxBlockAddr;
    else endBlockAddr = atoi((const char *)cPtr);

    if( ((endBlockAddr - startBlockAddr) <= 0) ||
    	(startBlockAddr >= (int32)maxBlockAddr) ||
        (endBlockAddr > (int32)maxBlockAddr)
	) {
    	printf( "\n");
		printf( "\t[ERROR] - Invalid Block Addr (Start : %d,End : %d)\n",
        				startBlockAddr, endBlockAddr);
		return;
    }

    index = 1;
    for(i = startBlockAddr; i <= endBlockAddr; i++) {
    	memoryClear( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
    	status = fbCodeMemInfoRead(	LOGIC_ID,	i, &fbMemInfo);

		if(status != NO_ERROR) {
			printf("[ERROR] - fbCodeMemInfoRead() Call Err !!\n");
            return;
        }

		if(sfcType == TRANSITION_TYPE) {
			if( (fbMemInfo.usingType != TRANSITION_TYPE)||
				( fbMemInfo.TransitionID != sfcTypeId ) ) continue;

	        printf(" [%05d]", i);
			if(fbMemInfo.fbId != 0)
				printf("%04d", fbMemInfo.fbId);
  			else
        		printf("    ");

			if( !(index % 6)) printf( "\n");
			index++;
		}

		else if(sfcType == ACTION_TYPE) {
			if( ( fbMemInfo.ActionID != sfcTypeId )||
			   (fbMemInfo.usingType != ACTION_TYPE) )continue;
			 
			printf(" [%05d]", i);
			if(fbMemInfo.fbId != 0)
				printf("%04d", fbMemInfo.fbId);
  			else
        		printf("    ");

			if( !(index % 6)) printf( "\n");
			index++;
		}

		else {
			
			if(fbMemInfo.fbTaskId != sfcTypeId) continue;

			if(fbMemInfo.usingType == ACTION_TYPE) {
				printf(" [%05d:AC%03d]", i,fbMemInfo.ActionID);
			}
			else if(fbMemInfo.usingType == TRANSITION_TYPE) {
				printf(" [%05d:TR%03d]", i,fbMemInfo.TransitionID);
			}
			else 
				printf(" [%05d:0000]", i);
			
			if(fbMemInfo.fbId != 0)
				printf("%04d", fbMemInfo.fbId);
  			else
				printf("    ");

			if( !(index % 4)) printf( "\n");
			index++;
		}
	}
   	printf( "\n");
}



uint32 trsActiveStatusRead(uint32 trsId, uint32* boolData)
{
	uint32 status = NO_ERROR;
    strTransitionInfo *trsInfoPtr = NULL;
	*boolData = INACTIVE_FLAG;

	if((trsId == 0) || (trsId > MAX_TRANSITOIN_NUM)) {
		status = L_TRANSITION_ID_ERR;
		setErrorCode(__FILE__,__LINE__,"trsActiveStatusRead",status);
	}
	else{
		trsInfoPtr = gSFCTransitionLocalPtr;
		trsInfoPtr += (trsId-1);
		if(trsInfoPtr->transitionID == trsId) {
			if( (trsInfoPtr->activeStatus == ACTIVE_FLAG)||
				(trsInfoPtr->activeStatus == INACTIVE_FLAG) ) {
					*boolData = trsInfoPtr->activeStatus;
			}
			else {
				status = TRS_ACTIVESTATUS_ERR;
				setErrorCode(__FILE__,__LINE__,"trsActiveStatusRead",status);
			}
		}
		else{
			status = L_TRANSITION_ID_ERR;
			setErrorCode(__FILE__,__LINE__,"trsActiveStatusRead",status);
		}
	}
	return(status);
}


uint32 actActiveStatusRead(uint32 actId, uint32* boolData)
{
	uint32 status = NO_ERROR;
    strActionInfo *actInfoPtr = NULL;
	strActionCtrlInfo *actCtrlPtr;

	*boolData = INACTIVE_FLAG;
	if((actId == 0) || (actId > MAX_ACTION_NUM))status = L_ACTION_ID_ERR;
	else{
		actInfoPtr = gSFCActionInfoLocalPtr;
		actInfoPtr += (actId-1);

		actCtrlPtr = (strActionCtrlInfo *)&actInfoPtr->actCtrlData;
		*boolData = actCtrlPtr->actCtrlQ;
	}
	return(status);
}


uint32 sfcStepCurretStatusGet(strSfcSequcerInfo *stepSeqData,uint32 *setFlag)
{
	uint32 status = NO_ERROR;

	strStepInfo	*stepInfoPtr;
	strSfcMemPointerInfo sfcLocalMemPtr;

	status=sfcMemPointerGet(LOCAL_MEM_ACC, &sfcLocalMemPtr);
	if(status != NO_ERROR){
			setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
							(uint8 *)"sfcStepCurretStatusGet",status
			  		  		);
			goto MODULE_END;
	}
	
	if(stepSeqData!=NULL)
	{
		if(stepSeqData->nodeType == STEP_TYPE)
		{
			stepInfoPtr = (strStepInfo*)sfcLocalMemPtr.stepMemPtr;  
			stepInfoPtr += (stepSeqData->stepTrId -1);

			if (stepSeqData->stepTrId != stepInfoPtr->stepID) {
				status = L_STEP_ID_ERR;
				goto MODULE_END;
			}
			

			if(stepInfoPtr->CX== ACTIVE_FLAG || stepInfoPtr->CX == INACTIVE_FLAG ) {
				
				*setFlag = stepInfoPtr->CX;
			}
			else{
				*setFlag = 0;
				status = SFC_ACTIVE_FLAG_ERR;
			}

		}
		else status = SFC_TYPE_ID_ERR;

	}
	else status = STEP_SEQUENCER_ERR;


MODULE_END:
	return(status);
}


uint32 sfcStepCurretStatusSet(strSfcSequcerInfo *stepSeqData, uint32 setFlag)
{
	uint32 status = NO_ERROR;

	strStepInfo	*stepInfoPtr,*stepInfoBusPtr, *stepInfoRmtPtr;
	strSfcMemPointerInfo sfcRmtMemPtr;

	uint32 rmtBusCopyFlag ;
	
	status=sfcMemPointerGet(RMT_MEM_ACC, &sfcRmtMemPtr);
	if(status != NO_ERROR){
			setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
							(uint8 *)"sfcStepOnRunFunc",status
			  		  		);
			goto MODULE_END;
	}

	if(stepSeqData!=NULL)
	{
		if(stepSeqData->nodeType == STEP_TYPE)
		{
			stepInfoPtr = (strStepInfo*)gSFCStepInfoLocalPtr;
			stepInfoPtr += (stepSeqData->stepTrId -1);

			if (stepSeqData->stepTrId != stepInfoPtr->stepID) {
				status = L_STEP_ID_ERR;
				goto MODULE_END;
			}
			stepInfoPtr->CX = setFlag;

			
			rmtBusCopyFlag = rmtBusCopyCheck();

			stepInfoBusPtr = (strStepInfo*)gSFCStepInfoBusPtr;   
			stepInfoBusPtr += (stepSeqData->stepTrId -1);
			stepInfoBusPtr->CX = stepInfoPtr->CX;


			
			if(rmtBusCopyFlag == NO_ERROR){
				stepInfoRmtPtr = (strStepInfo*)sfcRmtMemPtr.stepMemPtr;   
				stepInfoRmtPtr += (stepSeqData->stepTrId -1);

				status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, BUS_DATA_WRITE ,
										(uint8 *)&stepInfoRmtPtr->CX, (uint8 *)&stepInfoPtr->CX	);
				if(status != NO_ERROR)
						setErrorCode(	(uint8 *)__FILE__, 	__LINE__, (uint8 *)"sfcStepCurretStatusSet", status );


			}
		}
	}
MODULE_END:
	return(status);
}


uint32 sfcStepActiveStatusGet(strSfcSequcerInfo *stepSeqData,uint32 *setFlag)
{
	uint32 status = NO_ERROR;

	strStepInfo	*stepInfoPtr;
	strSfcMemPointerInfo sfcLocalMemPtr;

	status=sfcMemPointerGet(LOCAL_MEM_ACC, &sfcLocalMemPtr);
	if(status != NO_ERROR){
			setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
							(uint8 *)"sfcStepCurretStatusGet",status
			  		  		);
			goto MODULE_END;
	}
	
	if(stepSeqData!=NULL)
	{
		if(stepSeqData->nodeType == STEP_TYPE)
		{
			stepInfoPtr = (strStepInfo*)sfcLocalMemPtr.stepMemPtr;  
			stepInfoPtr += (stepSeqData->stepTrId -1);

			if (stepSeqData->stepTrId != stepInfoPtr->stepID) {
				status = L_STEP_ID_ERR;
				goto MODULE_END;
			}
			
			if(stepInfoPtr->X== ACTIVE_FLAG || stepInfoPtr->X == INACTIVE_FLAG ) {
				
				*setFlag = stepInfoPtr->X;
			}
			else{
				*setFlag = 0;
				status = SFC_ACTIVE_FLAG_ERR;
			}

		}
		else status = SFC_TYPE_ID_ERR;

	}
	else status = STEP_SEQUENCER_ERR;


MODULE_END:
	return(status);
}


uint32 sfcStepStatusAndTimeGet(uint32 taskId, uint32 stepId, strStepXTInfo *step)
{
	uint32 status = NO_ERROR;

	strStepInfo	*stepInfoPtr;
	strSfcMemPointerInfo sfcLocalMemPtr;

	status=sfcMemPointerGet(LOCAL_MEM_ACC, &sfcLocalMemPtr);
	if(status != NO_ERROR){
			setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
							(uint8 *)"sfcStepStatusAndTimeGet",status
			  		  		);
			goto MODULE_END;
	}
	
	stepInfoPtr = (strStepInfo*)sfcLocalMemPtr.stepMemPtr;  
	stepInfoPtr += (stepId -1);

	if (taskId!= stepInfoPtr->taskID) {
		status = L_SEG_ID_ERR;
		goto MODULE_END;
	}

	if (stepId!= stepInfoPtr->stepID) {
		status = L_STEP_ID_ERR;
		goto MODULE_END;
	}
	
	if(stepInfoPtr->CX== ACTIVE_FLAG || stepInfoPtr->CX == INACTIVE_FLAG ) {
		
		step->X = stepInfoPtr->X;
		step->T = stepInfoPtr->T;
	}
	else{
		step->X =0 ;  step->T= 0;
		status = SFC_ACTIVE_FLAG_ERR;
	}

MODULE_END:
	return(status);
}


uint32 sfcStepOnRunFunc(strSfcSequcerInfo *stepSeqData, uint32 cycleTime)
{
	uint32 status = NO_ERROR;

	strStepInfo	*stepInfoPtr,*stepInfoBusPtr, *stepInfoRmtPtr;
	strActionInfo  *actInfoPtr,*actInfoBusPtr, *actInfoRmtPtr;
	strStepLinkInfo *stepLinkPtr;
	strActionCtrlInfo *actCtrlPtr;

	strSfcMemPointerInfo sfcRmtMemPtr;

	uint32 rmtBusCopyFlag ;
	uint32 execCycle = cycleTime * CYCLIC_FB_EXEC_TIME_BASE;  
	
	status=sfcMemPointerGet(RMT_MEM_ACC, &sfcRmtMemPtr);
	if(status != NO_ERROR){
			setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
							(uint8 *)"sfcStepOnRunFunc",status
			  		  		);
			goto MODULE_END;
	}

	if(stepSeqData!=NULL)
	{
		if(stepSeqData->nodeType == STEP_TYPE)
		{
			stepInfoPtr = (strStepInfo*)gSFCStepInfoLocalPtr;
			stepInfoPtr += (stepSeqData->stepTrId -1);

			if (stepSeqData->stepTrId != stepInfoPtr->stepID) {
				status = L_STEP_ID_ERR;
				goto MODULE_END;
			}
			
			if(stepSeqData->visitFlag == INACTIVE_FLAG){
				stepInfoPtr->X = ACTIVE_FLAG;
				stepInfoPtr->T += execCycle;
			}
			stepInfoPtr->CX = ACTIVE_FLAG;

			
			rmtBusCopyFlag = rmtBusCopyCheck();

			stepInfoBusPtr = (strStepInfo*)gSFCStepInfoBusPtr;   
			stepInfoBusPtr += (stepSeqData->stepTrId -1);
			if(stepSeqData->visitFlag == INACTIVE_FLAG) {
				stepInfoBusPtr->T = stepInfoPtr->T;
				stepInfoBusPtr->X = stepInfoPtr->X;
			}
			stepInfoBusPtr->CX = stepInfoPtr->CX;


			
			if(rmtBusCopyFlag == NO_ERROR){
				stepInfoRmtPtr = (strStepInfo*)sfcRmtMemPtr.stepMemPtr;   
				stepInfoRmtPtr += (stepSeqData->stepTrId -1);

				
					status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, BUS_DATA_WRITE ,
											(uint8 *)&stepInfoRmtPtr->T, (uint8 *)&stepInfoPtr->T	);
					if(status != NO_ERROR)
							setErrorCode(	(uint8 *)__FILE__, 	__LINE__, (uint8 *)"sfcStepOnRunFunc", status );

					status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, BUS_DATA_WRITE ,
											(uint8 *)&stepInfoRmtPtr->X, (uint8 *)&stepInfoPtr->X	);
					if(status != NO_ERROR)
							setErrorCode(	(uint8 *)__FILE__, 	__LINE__, (uint8 *)"sfcStepOnRunFunc", status );

					status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, BUS_DATA_WRITE ,
											(uint8 *)&stepInfoRmtPtr->CX, (uint8 *)&stepInfoPtr->CX	);
					if(status != NO_ERROR)
							setErrorCode(	(uint8 *)__FILE__, 	__LINE__, (uint8 *)"sfcStepOnRunFunc", status );

			}

			stepLinkPtr = stepLinkStartMemInfoGet(stepInfoPtr->stepID);
			
			while((stepLinkPtr!=NULL) && (stepSeqData->visitFlag == INACTIVE_FLAG))
			{
				if(stepLinkPtr->taskId != stepInfoPtr->taskID) {
					status = L_SEG_ID_ERR;
					goto MODULE_END;
				}
				actInfoPtr = (strActionInfo*)gSFCActionInfoLocalPtr;
				actInfoPtr += (stepLinkPtr->actionId - 1);

				if(actInfoPtr->actionID!= stepLinkPtr->actionId){
					status = L_ACTION_ID_ERR;
					goto MODULE_END;
				}

				
				status = actCurInfoSet(stepLinkPtr->taskId,actInfoPtr->actionID);
				if(status != NO_ERROR) {
					setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
									(uint8 *)"sfcStepOnRunFunc",status
			  		  				);
				}

				actCtrlPtr = (strActionCtrlInfo*)&actInfoPtr->actCtrlData;

				
				switch(stepLinkPtr->actionFlag)
				{
					case AF_NONE:
					case AF_NONSTROE:
						actCtrlPtr->Nonstore = ACTIVE_FLAG;
					break;

					case AF_RESET:
						actCtrlPtr->Reset = ACTIVE_FLAG;
					break;

					case AF_SET:
						actCtrlPtr->Set = ACTIVE_FLAG;
					break;

					case AF_PULSE:
						actCtrlPtr->Pulse = ACTIVE_FLAG;
					break;
					
					case AF_DELAY:
						actCtrlPtr->Delay = ACTIVE_FLAG;
						if(actCtrlPtr->ActiveTFlag != AF_DELAY)	{
							actCtrlPtr->ActiveTFlag = AF_DELAY;
							actInfoPtr ->setCurTime = stepLinkPtr->actionTime;
							actCtrlPtr->Timer_Mem = 0;
							actInfoPtr->elapsedTime = 0;
						}
					break;

					case AF_LIMIT:
						actCtrlPtr->Limit = ACTIVE_FLAG; 
						if(actCtrlPtr->ActiveTFlag != AF_LIMIT)	{
							actCtrlPtr->ActiveTFlag = AF_LIMIT;
							actInfoPtr ->setCurTime = stepLinkPtr->actionTime;
							actCtrlPtr->Timer_Mem = 0;
							actInfoPtr->elapsedTime = 0;
						}
					break;

					case AF_SET_DELAY:
						actCtrlPtr->SetDelay = ACTIVE_FLAG; 
						if(actCtrlPtr->ActiveTFlag != AF_SET_DELAY)	{
							actCtrlPtr->ActiveTFlag=AF_SET_DELAY;
							actInfoPtr ->setCurTime = stepLinkPtr->actionTime;
							actInfoPtr->elapsedTime = 0;
						}
					break;

					case AF_SET_LIMIT:
						actCtrlPtr->SetLimit = ACTIVE_FLAG;
						if(actCtrlPtr->ActiveTFlag != AF_SET_LIMIT)	{
							actCtrlPtr->ActiveTFlag=AF_SET_LIMIT;
							actInfoPtr ->setCurTime= stepLinkPtr->actionTime;
							actInfoPtr->elapsedTime = 0;
						}

					break;

					case AF_DELAY_SET:
						actCtrlPtr->DelaySet = ACTIVE_FLAG;
						if(actCtrlPtr->ActiveTFlag != AF_DELAY_SET)	{
							actCtrlPtr->ActiveTFlag = AF_DELAY_SET;
							actInfoPtr ->setCurTime = stepLinkPtr->actionTime;
							actCtrlPtr->Timer_Mem = 0;
							actInfoPtr->elapsedTime = 0;
						}
					break;

					default:
						actCtrlPtr->ActiveTFlag = INACTIVE_FLAG;
						actInfoPtr ->setCurTime = 0;
						actInfoPtr->elapsedTime = 0;
						status = SFC_ACTION_FLAG_ID_ERR;
					break;
				}
			
				
				actInfoBusPtr = (strActionInfo*)gSFCActionInfoBusPtr;
				actInfoBusPtr += (stepLinkPtr->actionId - 1);
				actInfoBusPtr->actCtrlData = actInfoPtr->actCtrlData;

				actInfoBusPtr->elapsedTime = actInfoPtr->elapsedTime;
				actInfoBusPtr->setCurTime = actInfoPtr ->setCurTime;
				
				
				if(rmtBusCopyFlag == NO_ERROR){
					actInfoRmtPtr = (strActionInfo*)sfcRmtMemPtr.actMemPtr;
					actInfoRmtPtr += (stepLinkPtr->actionId - 1);

					
					status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
											BUS_DATA_WRITE ,
											(uint8 *)&actInfoRmtPtr->elapsedTime, 
											(uint8 *)&actInfoPtr->elapsedTime
					);
					if(status != NO_ERROR)
							setErrorCode(	(uint8 *)__FILE__, 	__LINE__, (uint8 *)"sfcStepOnRunFunc", status );

					status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
											BUS_DATA_WRITE ,
											(uint8 *)&actInfoRmtPtr->actCtrlData, 
											(uint8 *)&actInfoPtr->actCtrlData
					);
					if(status != NO_ERROR)
							setErrorCode(	(uint8 *)__FILE__, 	__LINE__, (uint8 *)"sfcStepOnRunFunc", status );

				}
				
				stepLinkPtr = stepLinkNextMemInfoGet(stepInfoPtr->stepID,stepLinkPtr);
			}
		}
		else status = SFC_TYPE_ID_ERR;

		
		stepSeqData->visitFlag = ACTIVE_FLAG;
	}
	else status = STEP_SEQUENCER_ERR;


MODULE_END:
	return(status);
}



uint32 sfcStep0ffRunFunc(strSfcSequcerInfo *stepSeqData)
{
	uint32 status = NO_ERROR;

	strStepInfo	*stepInfoPtr, *stepInfoBusPtr,*stepInfoRmtPtr;
	strActionInfo  *actInfoPtr, *actInfoBusPtr,*actInfoRmtPtr;
	strStepLinkInfo *stepLinkPtr;
	strActionCtrlInfo *actCtrlPtr;
	strSfcMemPointerInfo sfcRmtMemPtr;
	uint32	rmtBusCopyFlag;

	status=sfcMemPointerGet(RMT_MEM_ACC, &sfcRmtMemPtr);
	if(status != NO_ERROR){
			setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
							(uint8 *)"sfcStep0ffRunFunc",
                   			status
			  		  		);
			goto MODULE_END;
	}

	rmtBusCopyFlag = rmtBusCopyCheck();

	if(stepSeqData!=NULL)
	{
		if(stepSeqData->nodeType == STEP_TYPE)
		{
			stepInfoPtr = (strStepInfo*)gSFCStepInfoLocalPtr;  
			stepInfoPtr += (stepSeqData->stepTrId -1);

			if (stepSeqData->stepTrId != stepInfoPtr->stepID) {
				status = L_STEP_ID_ERR;
				goto MODULE_END;
			}

			
			stepInfoPtr->X = INACTIVE_FLAG;
			stepInfoPtr->T = 0;

			
			stepInfoBusPtr = (strStepInfo*)gSFCStepInfoBusPtr;   
			stepInfoBusPtr += (stepSeqData->stepTrId -1);
			stepInfoBusPtr->X = stepInfoPtr->X;
			stepInfoBusPtr->T = stepInfoPtr->T;

			
			if(rmtBusCopyFlag == NO_ERROR){
				stepInfoRmtPtr = (strStepInfo*)sfcRmtMemPtr.stepMemPtr;   
				stepInfoRmtPtr += (stepSeqData->stepTrId -1);

				
				status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 	BUS_DATA_WRITE ,
										(uint8 *)&stepInfoRmtPtr->T, (uint8 *)&stepInfoPtr->T
				);
				if(status != NO_ERROR)
					setErrorCode(	(uint8 *)__FILE__, 	__LINE__, (uint8 *)"sfcStep0ffRunFunc", status );

				status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 	BUS_DATA_WRITE ,
										(uint8 *)&stepInfoRmtPtr->X, (uint8 *)&stepInfoPtr->X
				);
				if(status != NO_ERROR)
					setErrorCode(	(uint8 *)__FILE__, 	__LINE__, (uint8 *)"sfcStep0ffRunFunc", status );
			}


			stepLinkPtr = stepLinkStartMemInfoGet(stepInfoPtr->stepID);
			while(stepLinkPtr!=NULL)
			{
				if(stepLinkPtr->taskId != stepInfoPtr->taskID) {
					status = L_SEG_ID_ERR;
					goto MODULE_END;
				}
				actInfoPtr = (strActionInfo*)gSFCActionInfoLocalPtr;
				actInfoPtr += (stepLinkPtr->actionId - 1);

				if(actInfoPtr->actionID!= stepLinkPtr->actionId){
					status = L_ACTION_ID_ERR;
					goto MODULE_END;
				}
				actCtrlPtr = (strActionCtrlInfo*)&actInfoPtr->actCtrlData;

				
				switch(stepLinkPtr->actionFlag)
				{
					case AF_NONE:
					case AF_NONSTROE:
						actCtrlPtr->Nonstore = INACTIVE_FLAG;
					break;

					case AF_RESET:
						actCtrlPtr->Reset = INACTIVE_FLAG;
					break;

					case AF_SET:
						actCtrlPtr->Set = INACTIVE_FLAG;
					break;

					case AF_PULSE:
						actCtrlPtr->Pulse = INACTIVE_FLAG;
						actCtrlPtr->Pulse_Mem = INACTIVE_FLAG;
						actInfoPtr->elapsedTime = 0;
					break;
					
					case AF_DELAY:
						actCtrlPtr->Delay = INACTIVE_FLAG;
						actCtrlPtr->Timer_Mem = INACTIVE_FLAG;
						actInfoPtr->elapsedTime = 0;
						actCtrlPtr->ActiveTFlag = INACTIVE_FLAG;
					break;

					case AF_LIMIT:
						actCtrlPtr->Limit = INACTIVE_FLAG;
						actCtrlPtr->Timer_Mem = INACTIVE_FLAG;
						actInfoPtr->elapsedTime = 0;
						actCtrlPtr->ActiveTFlag = INACTIVE_FLAG;
					break;

					case AF_DELAY_SET:
						actCtrlPtr->DelaySet = INACTIVE_FLAG;
						actCtrlPtr->Timer_Mem = INACTIVE_FLAG;
						actInfoPtr->elapsedTime = 0;
						actCtrlPtr->ActiveTFlag = INACTIVE_FLAG;
					break;

					case AF_SET_DELAY:
						actCtrlPtr->SetDelay = INACTIVE_FLAG;
					break;

					case AF_SET_LIMIT:
						actCtrlPtr->SetLimit = INACTIVE_FLAG;
					break;


					default:
						status = SFC_ACTION_FLAG_ID_ERR;
					break;
				}

				
				actInfoBusPtr = (strActionInfo*)gSFCActionInfoBusPtr;
				actInfoBusPtr += (stepLinkPtr->actionId - 1);
				actInfoBusPtr->actCtrlData = actInfoPtr->actCtrlData;
				actInfoBusPtr->elapsedTime = actInfoPtr->elapsedTime;

				
				if(rmtBusCopyFlag == NO_ERROR){
					actInfoRmtPtr = (strActionInfo*)sfcRmtMemPtr.actMemPtr;
					actInfoRmtPtr += (stepLinkPtr->actionId - 1);

					
					status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
											BUS_DATA_WRITE ,
											(uint8 *)&actInfoRmtPtr->actCtrlData, 
											(uint8 *)&actInfoPtr->actCtrlData
					);
					if(status != NO_ERROR)
						setErrorCode(	(uint8 *)__FILE__, 	__LINE__, (uint8 *)"sfcStep0ffRunFunc", status );

					status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
											BUS_DATA_WRITE ,
											(uint8 *)&actInfoRmtPtr->elapsedTime, 
											(uint8 *)&actInfoPtr->elapsedTime
					);
					if(status != NO_ERROR)
						setErrorCode(	(uint8 *)__FILE__, 	__LINE__, (uint8 *)"sfcStep0ffRunFunc", status );
				}

				stepLinkPtr = stepLinkNextMemInfoGet(stepInfoPtr->stepID,stepLinkPtr);
			}
		}
		else status = SFC_TYPE_ID_ERR;
	}
	else status = STEP_SEQUENCER_ERR;

MODULE_END:
	return(status);
}



uint32 sfcPrevStepCXAllOff(strSfcAdjacentInfo* prevStepHeadPtr, uint32 stepKind)
{
	uint32 status = NO_ERROR;
	strSfcAdjacentInfo* prevStepPtr;
	strStepInfo	*stepInfoPtr,*stepInfoBusPtr , *stepInfoRmtPtr;
	strSfcMemPointerInfo sfcRmtMemPtr;
	uint32 stepId = 0, rmtBusCopyFlag;

	status=sfcMemPointerGet(RMT_MEM_ACC, &sfcRmtMemPtr);
	if(status != NO_ERROR){
			setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
							(uint8 *)"sfcMemPointerGet",status
			  		  		);
			goto MODULE_END;
	}

	rmtBusCopyFlag = rmtBusCopyCheck();

	prevStepPtr = prevStepHeadPtr;
	while(prevStepPtr!=NULL) 
	{
		if(prevStepPtr->sequencerId != NULL){
			if(prevStepPtr->sequencerId->nodeType == STEP_TYPE) {
				
				if(stepKind == ONE_STEP_KIND)
					status = sfcStep0ffRunFunc(prevStepPtr->sequencerId);

				stepId = prevStepPtr->sequencerId->stepTrId;
				stepInfoPtr = (strStepInfo*)gSFCStepInfoLocalPtr;  
				stepInfoPtr += ( stepId -1);
				
				stepInfoPtr->CX = INACTIVE_FLAG;

				
				if(stepKind == ONE_STEP_KIND) stepInfoPtr->X = INACTIVE_FLAG;

				
				stepInfoBusPtr = (strStepInfo*)gSFCStepInfoBusPtr;   
				stepInfoBusPtr += (stepId -1);
				stepInfoBusPtr->CX = stepInfoPtr->CX;

				
				if(stepKind == ONE_STEP_KIND) 
					stepInfoBusPtr->X = stepInfoPtr->X;

				
				if(rmtBusCopyFlag == NO_ERROR) {
					stepInfoRmtPtr = (strStepInfo*)sfcRmtMemPtr.stepMemPtr;   
					stepInfoRmtPtr += (stepId -1);

					status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
											BUS_DATA_WRITE ,
											(uint8 *)&stepInfoRmtPtr->CX, 
											(uint8 *)&stepInfoPtr->CX
					);
					if(status != NO_ERROR)
						setErrorCode(	(uint8 *)__FILE__, 	__LINE__, (uint8 *)"sfcPrevStepCXAllOff", status );

					
					if(stepKind == ONE_STEP_KIND) {
						status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
												BUS_DATA_WRITE ,
												(uint8 *)&stepInfoRmtPtr->X, 
												(uint8 *)&stepInfoPtr->X
						);
						if(status != NO_ERROR)
							setErrorCode(	(uint8 *)__FILE__, 	__LINE__, (uint8 *)"sfcPrevStepCXAllOff", status );
					}
				}
			}
			else status = SFC_TYPE_ID_ERR;
			
			prevStepPtr = (strSfcAdjacentInfo *)nextNodeRef(  (strDoubleLink *)prevStepHeadPtr,
													(strDoubleLink *)prevStepPtr);
		}
		else status = STEP_SEQUENCER_ERR;
	}	
MODULE_END:
	return (status);
}



strSfcSequcerInfo* sfcNextTransitionCheck(strSfcAdjacentInfo* nextTrsHeadPtr, uint32* nextTrStatus)
{
	uint32 status = NO_ERROR;
	strSfcAdjacentInfo *nextTrsPtr, *prevStepHeadPtr, *prevStepPtr;
	strSfcSequcerInfo *nextSeqTrsPtr= NULL, *preSeqStepPtr;
	uint32 trsActiveFlag = 0, currFlag = 0;
	
	nextTrsPtr = nextTrsHeadPtr;

	while( (nextTrsPtr!=NULL)&&(nextTrsPtr->nodeType == TRANSITION_TYPE) ) 
	{

		nextSeqTrsPtr = nextTrsPtr->sequencerId;
		if(nextSeqTrsPtr != NULL) {
			
			status = trsActiveStatusRead(nextSeqTrsPtr->stepTrId, &trsActiveFlag);
			if(status != NO_ERROR) {
				setErrorCode(__FILE__,__LINE__,
							"sfcNextTransitionCheck",status); 
			}

 			
			*nextTrStatus = INACTIVE_FLAG;
			if((status == NO_ERROR)&& (trsActiveFlag == ACTIVE_FLAG))
			{
				prevStepHeadPtr = (strSfcAdjacentInfo *)nextSeqTrsPtr->prevDblink;
				prevStepPtr = prevStepHeadPtr;
				
				
				while(prevStepPtr!=NULL)  {
					preSeqStepPtr= prevStepPtr->sequencerId;
					
					if(preSeqStepPtr->nodeType == STEP_TYPE) {
						status = sfcStepCurretStatusGet(preSeqStepPtr,&currFlag);
						if(status != NO_ERROR) {
							setErrorCode(__FILE__,__LINE__,
										"sfcNextTransitionCheck",status);
						}
					}

					if((currFlag != ACTIVE_FLAG) || (preSeqStepPtr->nodeType != STEP_TYPE))
					{
						*nextTrStatus = INACTIVE_FLAG;
						break;
					}
					else *nextTrStatus = ACTIVE_FLAG;

					prevStepPtr = (strSfcAdjacentInfo *)nextNodeRef(  (strDoubleLink *)prevStepHeadPtr,
																(strDoubleLink *)prevStepPtr);
				}

				
				if(*nextTrStatus== ACTIVE_FLAG) break;
			}

		}
		nextTrsPtr = (strSfcAdjacentInfo *)nextNodeRef(  (strDoubleLink *)nextTrsHeadPtr,
														(strDoubleLink *)nextTrsPtr);
	}

	return((strSfcSequcerInfo*) nextSeqTrsPtr);
}


uint32 RS_FlipFlop(strRSFlipFlopInfo *fd);
uint32 TON_Timer(strTONtimerInfo  *fd, uint32 cycleTime);

uint32 sfcActionCtrlRunning(uint32 taskId)
{
	uint32 status = NO_ERROR;
	strActionInfo  *actInfoPtr, *actInfoBusPtr, *actInfoRmtPtr, *actStartInfoPtr;
	strActionCtrlInfo *actCtrlInfoPtr;
	uint8  Pulse_Q=0, Delay_Q=0, Limit_Q=0, SetDelay_Q=0, DelaySet_Q=0, SetLimit_Q=0;

	strRSFlipFlopInfo Set_RS, SetDelay_RS, DelaySet_RS, SetLimit_RS;
	strTONtimerInfo   TON_L, TON_D, TON_DS, TON_SD, TON_SL;

	strSfcMemPointerInfo sfcRmtMemPtr;
	uint32 actId, execCycleTime;
	strFbTaskTblMemInfo segTblInfo; 

	uint32 boolData = 0;
	strFbSpecActionInfo* specActionPtr;

   	status = getFbTaskTblInfo	(  LOGIC_ID,	taskId, (strFbTaskTblMemInfo	*) &segTblInfo );
	if(status != NO_ERROR){
        setErrorCode( __FILE__, __LINE__, "sfcActionCtrlRunning",status);
		return(status);		
	}
	execCycleTime = segTblInfo.taskCycleTime * CYCLIC_FB_EXEC_TIME_BASE;

	status=sfcMemPointerGet(RMT_MEM_ACC, &sfcRmtMemPtr);
	if(status != NO_ERROR){
		setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
						(uint8 *)"sfcActionCtrlRunning",status
			  		  	);
		goto MODULE_END;
	}


	actStartInfoPtr = gSFCActionInfoLocalPtr;
	for( actId = 1; actId <= MAX_ACTION_NUM; actId++) 
	{
		actInfoPtr = actStartInfoPtr + (actId-1) ;
		if( (actInfoPtr->taskID != taskId) || (actInfoPtr->actionID != actId) ) {
			continue;
		}
		
		memoryClear((uint8*)&TON_D, sizeof(strTONtimerInfo));
		memoryClear((uint8*)&TON_L, sizeof(strTONtimerInfo));
		memoryClear((uint8*)&TON_DS, sizeof(strTONtimerInfo));
		memoryClear((uint8*)&TON_SD, sizeof(strTONtimerInfo));
		memoryClear((uint8*)&TON_SL, sizeof(strTONtimerInfo));

		memoryClear((uint8*)&Set_RS, sizeof(strRSFlipFlopInfo));
		memoryClear((uint8*)&SetDelay_RS, sizeof(strRSFlipFlopInfo));
		memoryClear((uint8*)&DelaySet_RS, sizeof(strRSFlipFlopInfo));
		memoryClear((uint8*)&SetLimit_RS, sizeof(strRSFlipFlopInfo));


		actInfoBusPtr = (strActionInfo  *)gSFCActionInfoBusPtr;
		actInfoRmtPtr = (strActionInfo  *)sfcRmtMemPtr.actMemPtr;

		actInfoBusPtr += (actId-1);
		actCtrlInfoPtr = (strActionCtrlInfo *)&actInfoPtr->actCtrlData;

		
		Set_RS.s = actCtrlInfoPtr->Set;
		Set_RS.r1 = actCtrlInfoPtr->Reset;
		Set_RS.q1 = actCtrlInfoPtr->Set_RS_Q;
		status = RS_FlipFlop(&Set_RS);
		actCtrlInfoPtr->Set_RS_Q = Set_RS.q1;

		
		if((actCtrlInfoPtr->Pulse)&&(!actCtrlInfoPtr->Pulse_Mem)) 	Pulse_Q= ACTIVE_FLAG;
		else  Pulse_Q= INACTIVE_FLAG;
		actCtrlInfoPtr->Pulse_Mem = actCtrlInfoPtr->Pulse;

		
		
		if( (actCtrlInfoPtr->SL_RS_Q != ACTIVE_FLAG) && (actCtrlInfoPtr->SetLimit!=ACTIVE_FLAG) ) {
			SetDelay_RS.s = actCtrlInfoPtr->SetDelay;
			SetDelay_RS.r1 = actCtrlInfoPtr->Reset;
			SetDelay_RS.q1 = actCtrlInfoPtr->SD_RS_Q;
			status = RS_FlipFlop(&SetDelay_RS);
			actCtrlInfoPtr->SD_RS_Q = SetDelay_RS.q1;
		}

		
		if( (actCtrlInfoPtr->SD_RS_Q != ACTIVE_FLAG) && (actCtrlInfoPtr->SetDelay!=ACTIVE_FLAG) ) {
			SetLimit_RS.s = actCtrlInfoPtr->SetLimit;
			SetLimit_RS.r1 = actCtrlInfoPtr->Reset;
			SetLimit_RS.q1 = actCtrlInfoPtr->SL_RS_Q;
			status = RS_FlipFlop(&SetLimit_RS);
			actCtrlInfoPtr->SL_RS_Q = SetLimit_RS.q1;
		}

		switch(actCtrlInfoPtr->ActiveTFlag)
		{
		case AF_DELAY:
			
			if(actCtrlInfoPtr->Delay == ACTIVE_FLAG ) {
				TON_D.et = actInfoPtr->elapsedTime;
				TON_D.pt = actInfoPtr->setCurTime;

				TON_D.in_clk = actCtrlInfoPtr->Delay;
				TON_D.in_mem = actCtrlInfoPtr->Timer_Mem;
				status = TON_Timer(&TON_D,execCycleTime);
				actCtrlInfoPtr->Timer_Mem = TON_D.in_mem;
				actInfoPtr->elapsedTime = TON_D.et;
				Delay_Q = TON_D.q;
			} 
			else Delay_Q = INACTIVE_FLAG;
		break;

		case AF_LIMIT:
			
			if(actCtrlInfoPtr->Limit == ACTIVE_FLAG ) {
				TON_L.et = actInfoPtr->elapsedTime;
				TON_L.pt = actInfoPtr->setCurTime;

				TON_L.in_clk = actCtrlInfoPtr->Limit;
				TON_L.in_mem = actCtrlInfoPtr->Timer_Mem;
				status = TON_Timer(&TON_L,execCycleTime);
				actCtrlInfoPtr->Timer_Mem = TON_L.in_mem;
				actInfoPtr->elapsedTime = TON_L.et;
				Limit_Q = (actCtrlInfoPtr->Limit && !TON_L.q);
			} 
			else Limit_Q = INACTIVE_FLAG;
		break;

		case AF_DELAY_SET:
			
			if(actCtrlInfoPtr->DelaySet == ACTIVE_FLAG){
				TON_DS.et = actInfoPtr->elapsedTime;
				TON_DS.pt = actInfoPtr->setCurTime;

				TON_DS.in_clk = actCtrlInfoPtr->DelaySet;
				TON_DS.in_mem = actCtrlInfoPtr->Timer_Mem;
				status = TON_Timer(&TON_DS,execCycleTime);
				actCtrlInfoPtr->Timer_Mem = TON_DS.in_mem;
				actInfoPtr->elapsedTime = TON_DS.et;
			}
			else TON_DS.q = 0;
		break;

		case AF_SET_DELAY:
			
			if(actCtrlInfoPtr->SD_RS_Q == ACTIVE_FLAG) {
				TON_SD.et = actInfoPtr->elapsedTime;
				TON_SD.pt = actInfoPtr->setCurTime;

				TON_SD.in_clk = SetDelay_RS.q1;
				TON_SD.in_mem = actCtrlInfoPtr->Timer_Mem;
				status = TON_Timer(&TON_SD,execCycleTime);
				actCtrlInfoPtr->Timer_Mem =TON_SD.in_mem;
				actInfoPtr->elapsedTime = TON_SD.et;
				SetDelay_Q = TON_SD.q;
			}
			else {
				SetDelay_Q = INACTIVE_FLAG;
				actCtrlInfoPtr->Timer_Mem = INACTIVE_FLAG;
			}
		break;

		case AF_SET_LIMIT:
			 
			if(actCtrlInfoPtr->SL_RS_Q == ACTIVE_FLAG) {
				TON_SL.et = actInfoPtr->elapsedTime;
				TON_SL.pt = actInfoPtr->setCurTime;

				TON_SL.in_clk = SetLimit_RS.q1;
				TON_SL.in_mem = actCtrlInfoPtr->Timer_Mem;
				status = TON_Timer(&TON_SL,execCycleTime);
				actCtrlInfoPtr->Timer_Mem = TON_SL.in_mem;
				actInfoPtr->elapsedTime = TON_SL.et;
				SetLimit_Q = (SetLimit_RS.q1 && !TON_SL.q);
			}
			else {
				SetLimit_Q = INACTIVE_FLAG;
				actCtrlInfoPtr->Timer_Mem = INACTIVE_FLAG;
			}
		break;

		default:
			Delay_Q = INACTIVE_FLAG;
			Limit_Q = INACTIVE_FLAG;
			SetDelay_Q  = INACTIVE_FLAG;
			DelaySet_Q  = INACTIVE_FLAG;
			SetLimit_Q  = INACTIVE_FLAG;
			actCtrlInfoPtr->Timer_Mem = INACTIVE_FLAG;
		break;
		}

		
		DelaySet_RS.s = TON_DS.q;
		DelaySet_RS.r1 = actCtrlInfoPtr->Reset;
		DelaySet_RS.q1 = actCtrlInfoPtr->DS_RS_Q;
		status = RS_FlipFlop(&DelaySet_RS);
		actCtrlInfoPtr->DS_RS_Q = DelaySet_RS.q1;
		DelaySet_Q = DelaySet_RS.q1;

		
		if(actCtrlInfoPtr->Reset == ACTIVE_FLAG){
			actCtrlInfoPtr->SL_RS_Q  = INACTIVE_FLAG;
			actCtrlInfoPtr->DS_RS_Q  = INACTIVE_FLAG;
			actCtrlInfoPtr->SD_RS_Q  = INACTIVE_FLAG;
			actCtrlInfoPtr->Set_RS_Q = INACTIVE_FLAG;
			actInfoPtr->elapsedTime = 0;
		}

		
		actCtrlInfoPtr->actCtrlQ = ( actCtrlInfoPtr->Nonstore ||actCtrlInfoPtr->Set_RS_Q ||Pulse_Q || 									
									Delay_Q || Limit_Q || SetDelay_Q || DelaySet_Q || SetLimit_Q );

		actCtrlInfoPtr->actCtrlQ = (!(actCtrlInfoPtr->Reset) && actCtrlInfoPtr->actCtrlQ);

					
		
		actInfoBusPtr->elapsedTime = actInfoPtr->elapsedTime;
		actInfoBusPtr->actCtrlData = actInfoPtr->actCtrlData;	

		
		if(rmtBusCopyCheck() == NO_ERROR){

			actInfoRmtPtr = (strActionInfo*)sfcRmtMemPtr.actMemPtr;   
			actInfoRmtPtr += (actId -1);

			status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
									BUS_DATA_WRITE ,
									(uint8 *)&actInfoRmtPtr->elapsedTime, 
									(uint8 *)&actInfoPtr->elapsedTime
			);
			if(status != NO_ERROR)
					setErrorCode(	(uint8 *)__FILE__, 	__LINE__, (uint8 *)"sfcActionCtrlRunning", status );

			status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
									BUS_DATA_WRITE ,
									(uint8 *)&actInfoRmtPtr->actCtrlData, 
									(uint8 *)&actInfoPtr->actCtrlData
			);
			if(status != NO_ERROR)
					setErrorCode(	(uint8 *)__FILE__, 	__LINE__, (uint8 *)"sfcActionCtrlRunning", status );

		}

		
		if( (actExecIdStartMemInfoGet(actId) == NULL ) &&(actInfoPtr->varActiveHeader != 0 ) )
		{
			boolData = actCtrlInfoPtr->actCtrlQ;
			
			specActionPtr = (strFbSpecActionInfo*)&actInfoPtr->varActiveHeader;
			status = refDataCopyToMem(&boolData, actInfoPtr->varActiveData, 
									*specActionPtr, SIZE32_TYPE);
			
			if(status == NO_ERROR) {
				status = refDataCopyToBusMem(&boolData, actInfoPtr->varActiveData, 
											*specActionPtr, SIZE32_TYPE);
			}
			
			if(status == NO_ERROR ) {
				if( dualFlagCheckInRunShelf() == DUAL_CHECK_ENABLE)
					status= refDataCopyToRmtMem(&boolData, actInfoPtr->varActiveData, 
											*specActionPtr, SIZE32_TYPE);
			}
			else {
				setErrorCode(__FILE__,__LINE__,"sfcActionCtrlRunning",status);
			}
		}			
	} 

MODULE_END:

	return(status);
}

uint32 RS_FlipFlop(strRSFlipFlopInfo  *fd)
{
	uint32 status = NO_ERROR;
	fd->q1 = (!fd->r1 && (fd->s || fd->q1)) || ( (!fd->s) &&fd->r1 && (!fd->q1) );
	return(status);
}

uint32 TON_Timer(strTONtimerInfo  *fd, uint32 cycleTime)
{
	uint32 status = NO_ERROR;
	uint32 in_q;
	in_q = (fd->in_clk&0x1);
	
	if(in_q == 0x1) {
		if(fd->in_mem == 0x0){
			fd->et = 0;
			if(fd->et >= fd->pt) {
				fd->q = 1;
			}
			else {
				fd->q = 0;
			}
		}
		else {
			if(fd->et < fd->pt) {
				fd->et 	+= cycleTime;
			}

			if(fd->et >= fd->pt) {
				fd->q = 1;
			}
			else {
				fd->q = 0;
			}
		}
	}
	else {
		fd->et 		= 0;
		fd->q		= 0;	
	}

	fd->in_mem	= in_q;
	return(status);
}


uint32 sfcMemoryClear(uint32 taskId, uint32 logicType)
{
	uint32 status = NO_ERROR;
	uint32 trsNum, actNum, stepNum;
	strStepInfo *stepStartInfoPtr, *stepInfoPtr;
	strTransitionInfo *trsStartInfoPtr, *trsInfoPtr;
	strActionInfo *actStartInfoPtr, *actInfoPtr;

	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
		status = L_SEG_ID_ERR;
        setErrorCode( __FILE__, __LINE__, "sfcMemoryClear",status);
		return(status);
	}

	 
	if(logicType != SFC_LOGIC_TYPE) return(status);

	
	stepStartInfoPtr = gSFCStepInfoLocalPtr;
	for(stepNum=0; stepNum< MAX_STEP_NUM ;stepNum++) {
		stepInfoPtr = stepStartInfoPtr + stepNum ;
		if(stepInfoPtr->taskID != taskId) continue;
		if(stepInfoPtr->stepID != (stepNum+1)) continue;
		memoryClear((uint8*)stepInfoPtr, sizeof(strStepInfo));
	}

	trsStartInfoPtr = gSFCTransitionLocalPtr;
	for(trsNum=0; trsNum< MAX_TRANSITOIN_NUM ;trsNum++) {
		trsInfoPtr = trsStartInfoPtr + trsNum ;
		if(trsInfoPtr->taskID != taskId) continue;
		if(trsInfoPtr->transitionID != (trsNum+1)) continue;
		memoryClear((uint8*)trsInfoPtr, sizeof(strTransitionInfo));
	}

	actStartInfoPtr = gSFCActionInfoLocalPtr;
	for(actNum=0; actNum< MAX_STEP_NUM ;actNum++) {
		actInfoPtr = actStartInfoPtr + actNum ;
		if(actInfoPtr->taskID != taskId) continue;
		if(actInfoPtr->actionID != (actNum+1)) continue;
		memoryClear((uint8*)actInfoPtr, sizeof(strActionInfo));
	}

	
	stepStartInfoPtr = gSFCStepInfoBusPtr;
	for(stepNum=0; stepNum< MAX_STEP_NUM ;stepNum++) {
		stepInfoPtr = stepStartInfoPtr + stepNum ;
		if(stepInfoPtr->taskID != taskId) continue;
		if(stepInfoPtr->stepID != (stepNum+1)) continue;
		memoryClear((uint8*)stepInfoPtr, sizeof(strStepInfo));
	}

	trsStartInfoPtr = gSFCTransitionBusPtr;
	for(trsNum=0; trsNum< MAX_TRANSITOIN_NUM ;trsNum++) {
		trsInfoPtr = trsStartInfoPtr + trsNum ;
		if(trsInfoPtr->taskID != taskId) continue;
		if(trsInfoPtr->transitionID != (trsNum+1)) continue;
		memoryClear((uint8*)trsInfoPtr, sizeof(strTransitionInfo));
	}

	actStartInfoPtr = gSFCActionInfoBusPtr;
	for(actNum=0; actNum< MAX_ACTION_NUM ;actNum++) {
		actInfoPtr = actStartInfoPtr + actNum ;
		if(actInfoPtr->taskID != taskId) continue;
		if(actInfoPtr->actionID != (actNum+1)) continue;
		memoryClear((uint8*)actInfoPtr, sizeof(strActionInfo));
	}
	return(status);
}


uint32 sfcStepInfoRead(uint32 stepId, strStepInfo* stepInfoData)
{
	uint32 status = NO_ERROR;
	strSfcMemPointerInfo sfcMemPtr;
	strStepInfo *StepInfoPtr;

	if((stepId == 0) || (stepId > MAX_STEP_NUM)){
		status = L_STEP_ID_ERR;
		setErrorCode( __FILE__, __LINE__, "sfcStepInfoRead",status);
		return(status);
	}
	
	status = sfcMemPointerGet(LOCAL_MEM_ACC, &sfcMemPtr);
	if(status != NO_ERROR){
		status = SFC_MEM_GET_ERR;
		setErrorCode(	__FILE__, __LINE__, 
					   "sfcStepInfoRead",status
			  		  );
		return(status);
	}

	StepInfoPtr = (strStepInfo*)sfcMemPtr.stepMemPtr;
	StepInfoPtr += (stepId-1) ;	
	memoryCopy((uint8*)stepInfoData ,(uint8*)StepInfoPtr,sizeof(strStepInfo) );

	return(status);
}


uint32 sfcTrsInfoRead(uint32 trsId, strTransitionInfo* trsInfoData)
{
	uint32 status = NO_ERROR;
	strSfcMemPointerInfo sfcMemPtr;
	strTransitionInfo *trsInfoPtr;

	if((trsId == 0) || (trsId > MAX_TRANSITOIN_NUM)){
		status = L_TRANSITION_ID_ERR;
		setErrorCode( __FILE__, __LINE__, "sfcTrsInfoRead",status);
		return(status);
	}
	
	status = sfcMemPointerGet(LOCAL_MEM_ACC, &sfcMemPtr);
	if(status != NO_ERROR){
		status = SFC_MEM_GET_ERR;
		setErrorCode(	__FILE__, __LINE__, 
					   "sfcTrsInfoRead",status
			  		  );
		return(status);
	}

	trsInfoPtr = (strTransitionInfo*)sfcMemPtr.trsMemPtr;
	trsInfoPtr += (trsId-1) ;	
	memoryCopy((uint8*)trsInfoData ,(uint8*)trsInfoPtr,sizeof(strTransitionInfo) );
	return(status);
}

uint32 sfcActInfoRead(uint32 actId, strActionInfo* actInfoData)
{
	uint32 status = NO_ERROR;
	strSfcMemPointerInfo sfcMemPtr;
	strActionInfo *actInfoPtr;

	if((actId == 0) || (actId > MAX_ACTION_NUM)){
		status = L_ACTION_ID_ERR;
		setErrorCode( __FILE__, __LINE__, "sfcActInfoRead",status);
		return(status);
	}
	
	status = sfcMemPointerGet(LOCAL_MEM_ACC, &sfcMemPtr);
	if(status != NO_ERROR){
		status = SFC_MEM_GET_ERR;
		setErrorCode(	__FILE__, __LINE__, 
					   "sfcActInfoRead",status
			  		  );
		return(status);
	}

	actInfoPtr = (strActionInfo*)sfcMemPtr.actMemPtr;
	actInfoPtr += (actId-1) ;	
	memoryCopy((uint8*)actInfoData ,(uint8*)actInfoPtr,sizeof(strActionInfo) );
	return(status);
}


void trsExecIdQueCountInfoDisplay(void)
{
	uint32	freeListCount = 0, usedListCount[MAX_TRANSITOIN_NUM], totalUsedQueCount = 0;
	strFbExecutionInfo	*nodePtr;
	uint32 sfcId = 0, status = NO_ERROR, lastSegId = 0;
	strTransitionInfo  trsDataInfo;

	memoryClear ( (uint8 *)	&usedListCount[0], MAX_TRANSITOIN_NUM*4);
	
	printf( "\t***************************************************\n");
	printf( "\t**          SFC TRS EXEC Queue Count Info        **\n");
	printf( "\t***************************************************\n");
	
	printf( "\tMAX QUEUE COUNT                  : %05d\n",MAX_FB_EXEC_SEQ_ID_NUM);
	
	nodePtr = sTrsExecQMngrInfo.freeList;

	
    status = semaphoreGet(gFbExecSeqIdQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR) {
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
		return;
	}

    while(nodePtr != NULL) {
    	freeListCount++;
        nodePtr = (strFbExecutionInfo *)nextNodeRef ( 	(strDoubleLink *)sTrsExecQMngrInfo.freeList,
	                        							(strDoubleLink *)nodePtr
							  						);
	}

	for(sfcId = 1; sfcId <= MAX_TRANSITOIN_NUM; sfcId++) {
		usedListCount[sfcId - 1] = 0;
	
		status= sfcTrsInfoRead(sfcId, &trsDataInfo);
		if(status != NO_ERROR){
    	    printf( "[ERROR] - TRS ID %d Read Error\n", sfcId); 
			goto MODULE_END;		
		}	
		if( trsDataInfo.transitionID != sfcId) continue;

		nodePtr = sTrsExecQMngrInfo.usedList[sfcId - 1];
    	while(nodePtr != NULL) {
    		usedListCount[sfcId - 1]++;
			lastSegId = sfcId;
        	nodePtr = (strFbExecutionInfo *)nextNodeRef ( 	(strDoubleLink *)sTrsExecQMngrInfo.usedList[sfcId - 1],
	        	                							(strDoubleLink *)nodePtr
								  						);
		}
		totalUsedQueCount += usedListCount[sfcId - 1];						  						
	}

MODULE_END:
	
#ifdef VXWORKS
	semaphoreRelease (gFbExecSeqIdQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gFbExecSeqIdQSemaId);	
#endif

	printf( "\tFree List Queue Count            : %05d\n",freeListCount);
	for(sfcId = 1; sfcId <= lastSegId; sfcId++){
		printf( "\t[TRS_ID ::%03d] Used Queue Count  : %05d\n",sfcId, usedListCount[sfcId - 1]);
	}	
	printf( "\tTotal Used Queue Count           : %05d\n",totalUsedQueCount);

	return;
}



void actExecIdQueCountInfoDisplay(void)
{
	uint32	freeListCount = 0, usedListCount[MAX_ACTION_NUM], totalUsedQueCount = 0;
	strFbExecutionInfo	*nodePtr;
	uint32 sfcId = 0, status = NO_ERROR, lastSegId = 0;
	strActionInfo  actDataInfo;

	memoryClear ( (uint8 *)	&usedListCount[0], MAX_ACTION_NUM*4);
	printf( "\n");
	printf( "\t***************************************************\n");
	printf( "\t**          SFC ACT EXEC Queue Count Info        **\n");
	printf( "\t***************************************************\n");
	
	printf( "\tMAX QUEUE COUNT                  : %05d\n",MAX_FB_EXEC_SEQ_ID_NUM);
	
	nodePtr = sActExecQMngrInfo.freeList;

	
    status = semaphoreGet(gFbExecSeqIdQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR) {
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
		return;
	}

    while(nodePtr != NULL) {
    	freeListCount++;
        nodePtr = (strFbExecutionInfo *)nextNodeRef ( 	(strDoubleLink *)sActExecQMngrInfo.freeList,
	                        							(strDoubleLink *)nodePtr
							  						);
	}

	for(sfcId = 1; sfcId <= MAX_ACTION_NUM; sfcId++) {
		usedListCount[sfcId - 1] = 0;
	
		status= sfcActInfoRead(sfcId, &actDataInfo);
		if(status != NO_ERROR){
    	    printf( "[ERROR] - ACT ID %d Read Error\n", sfcId); 
			goto MODULE_END;		
		}	
		if( actDataInfo.actionID != sfcId) continue;

		nodePtr = sActExecQMngrInfo.usedList[sfcId - 1];
    	while(nodePtr != NULL) {
    		usedListCount[sfcId - 1]++;
			lastSegId = sfcId;
        	nodePtr = (strFbExecutionInfo *)nextNodeRef ( 	(strDoubleLink *)sActExecQMngrInfo.usedList[sfcId - 1],
	        	                							(strDoubleLink *)nodePtr
								  						);
		}
		totalUsedQueCount += usedListCount[sfcId - 1];						  						
	}

MODULE_END:
	
#ifdef VXWORKS
	semaphoreRelease (gFbExecSeqIdQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gFbExecSeqIdQSemaId);	
#endif

	printf( "\tFree List Queue Count            : %05d\n",freeListCount);
	for(sfcId = 1; sfcId <= lastSegId; sfcId++){
		printf( "\t[ACT_ID ::%03d] Used Queue Count  : %05d\n",sfcId, usedListCount[sfcId - 1]);
	}	
	printf( "\tTotal Used Queue Count           : %05d\n",totalUsedQueCount);

	return;
}


#define	MAX_FB_EXEC_SEQ_ID_DISPLAY_NUM 256
void sfcFBExecIdQueInfoDisplay(void)
{
	uint32  startId, endId, printLoop, loopCount = 0, status = NO_ERROR;
	int8	line[16], *cPtr;
	strFbExecutionInfo	*nodePtr, saveData[MAX_FB_EXEC_SEQ_ID_DISPLAY_NUM], *saveDataPtr;
	uint32 sfcType =0 , sfcId;

SFC_TYPE:
	printf("\t Enter the SFC Type[1:TRS, 2:ACT] : ");
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);
    sfcType = atoi(cPtr);

	if(sfcType == TRANSITION_TYPE)	{
		printf("\t Enter Transition ID : ");
	}
	else if(sfcType == ACTION_TYPE)	{
		printf("\t Enter Action ID : ");
	}
	else goto SFC_TYPE;
	
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);
    sfcId = atoi(cPtr);

	if( (sfcType == TRANSITION_TYPE) &&
		( (sfcId == 0) || (sfcId > MAX_TRANSITOIN_NUM) )
	){
		printf( "[ERROR] - Transition ID Error !!\n");
		return;	
	}

	if( (sfcType == ACTION_TYPE) && 
		( (sfcId == 0) || (sfcId > MAX_ACTION_NUM)) 
	){
		printf( "[ERROR] - Action ID Error !!\n");
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
	if(endId == 0) {
		printf( "[ERROR] - End ID must be greater than 0\n");
		return;	
	}
    
    if(startId >= endId) {
		printf( "[ERROR] - End ID must be greater than Start ID !!\n");
		return;	
    }
    
    if( (endId - startId + 1) > MAX_FB_EXEC_SEQ_ID_DISPLAY_NUM){
		printf( "[ERROR] - The Number of Display(%d) is greater than MAX Number[%d]\n",
								(endId - startId + 1), MAX_FB_EXEC_SEQ_ID_DISPLAY_NUM);
		return;	
	}

 	
    status = semaphoreGet(gFbExecSeqIdQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR) {
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
		return;
	}

	if(sfcType == TRANSITION_TYPE)	{
		nodePtr = sTrsExecQMngrInfo.usedList[sfcId - 1];
  		while(nodePtr != NULL) {
			if(nodePtr->fbExecId == startId)
				break;
	   		nodePtr = (strFbExecutionInfo *)nextNodeRef ( 	(strDoubleLink *)sTrsExecQMngrInfo.usedList[sfcId - 1],
        	                								(strDoubleLink *)nodePtr
							  							);
		}

		if(nodePtr == NULL) {
			printf( "[ERROR] - NO FB Code Exist !!\n");
			goto MODULE_END;	
		}

		loopCount = 0;
		while(nodePtr != NULL) {
			
  			memoryCopy( (uint8 *) &saveData[loopCount], 
  						(uint8 *)nodePtr, 
  						sizeof(strFbExecutionInfo)
  					  );
			if( ++loopCount >= (endId-startId +1))
				 break;
			
	   		nodePtr = (strFbExecutionInfo *)nextNodeRef ( 	(strDoubleLink *)sTrsExecQMngrInfo.usedList[sfcId - 1],
        	                								(strDoubleLink *)nodePtr
							  							);
		}
	}
	else if(sfcType == ACTION_TYPE)	{
		nodePtr = sActExecQMngrInfo.usedList[sfcId - 1];
  		while(nodePtr != NULL) {
			if(nodePtr->fbExecId == startId)
				break;
	   		nodePtr = (strFbExecutionInfo *)nextNodeRef ( 	(strDoubleLink *)sActExecQMngrInfo.usedList[sfcId - 1],
        	                								(strDoubleLink *)nodePtr
							  							);
		}

		if(nodePtr == NULL) {
			printf( "[ERROR] - NO FB Code Exist !!\n");
			goto MODULE_END;	
		}

		loopCount = 0;
		while(nodePtr != NULL) {
			
  			memoryCopy( (uint8 *) &saveData[loopCount], 
  						(uint8 *)nodePtr, 
  						sizeof(strFbExecutionInfo)
  					  );
			if( ++loopCount >= (endId-startId +1))
				 break;
			
	   		nodePtr = (strFbExecutionInfo *)nextNodeRef ( 	(strDoubleLink *)sActExecQMngrInfo.usedList[sfcId - 1],
        	                								(strDoubleLink *)nodePtr
							  							);
		}
	}
	
MODULE_END:
	
#ifdef VXWORKS
	semaphoreRelease (gFbExecSeqIdQSemaId);	
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gFbExecSeqIdQSemaId);	
#endif

	printf("+=====+========+========+=========+=====+========+========+=========+\n");
 	printf("| ID# | SEG_ID | FB_ADR | FB CODE | ID# | SEG_ID | FB_ADR | FB CODE |\n");
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



static void sfcDataInMemoryDisplay()
{
	int8 line[16], *pLine;

    printf("\n\tEnter the Choice? [1:STEP, 2: TRANSITION, 3:ACTION] ");

    ioRead(gDebugPortFd, line, sizeof(line));
	pLine = line;
	spaceSkip(&pLine);

	switch( *pLine) {
	case '1':
        pLine++;
        if( *pLine != ENDOFSTR) {
	        printf("\n\t[Error]-Unknown Number !!!\n");
        }
		else {
			sfcStepMemoryDisplay();
		}
    break;
	case '2':
        pLine++;
        if( *pLine != ENDOFSTR) {
	        printf("\n\t[Error]-Unknown Number !!!\n");
        }
		else {
			sfcTransitionMemoryDisplay();
		}
    break;
	case '3':
        pLine++;
        if( *pLine != ENDOFSTR) {
	        printf("\n\t[Error]-Unknown Number !!!\n");
        }
		else {
			sfcActionMemoryDisplay();
		}
    break;
    default:
        printf("\n\t[Error]-Unknown Number !!!\n");
	break;
	}

}



static void sfcStepMemoryDisplay(void)
{
	uint32 startStepId, endStepId;
	uint32	status,  index, i;
	int8	line[16], *cPtr;
	strSfcMemPointerInfo sfcMemPtr;
	strStepInfo *stepInfoPtr;

	printf("\tStart Step ID (1-%d) : ", MAX_STEP_NUM);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    startStepId = atoi((const char *)cPtr);

    printf("\tEnd Step ID (%d-%d)  : ",startStepId+1,MAX_STEP_NUM);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    endStepId = atoi((const char *)cPtr);


	if( ((endStepId - startStepId) <= 0) ||
    	(startStepId >= endStepId) ||  (startStepId < 1) ||
        (endStepId > MAX_STEP_NUM)
	) {
    	printf( "\n");
		printf( "\t[ERROR] - Invalid Step Id (Start : %d,End : %d)\n",
        				startStepId, endStepId);
		return;
    }

    index = 1;
   	printf( "\n");
	printf( " +====+========+====+========+====+========+====+========+====+========+\n");
	printf( " | ID |STP:TASK| ID |STP:TASK| ID |STP:TASK| ID |STP:TASK| ID |STP:TASK|\n");
	printf( " +====+========+====+========+====+========+====+========+====+========+\n");

    for(i = startStepId; i <= endStepId; i++) {
    	memoryClear( (uint8 *) &sfcMemPtr, sizeof(strSfcMemPointerInfo));

		status = sfcMemPointerGet(LOCAL_MEM_ACC, &sfcMemPtr);
		if(status != NO_ERROR) {
			printf("[ERROR] - sfcMemPointerGet() Call Err !!\n");
            return;
        }
		stepInfoPtr = (strStepInfo*)sfcMemPtr.stepMemPtr;
		stepInfoPtr += (i - 1);

        printf(" [S%03d]", i);
      
        if(stepInfoPtr->stepID != 0)
	        printf("%03d:%03d", stepInfoPtr->stepID,  stepInfoPtr->taskID);
  		else
        	printf("       ");

        if( !(index % 5)) printf( "\n");
        index++;
	}
}


static void sfcTransitionMemoryDisplay(void)
{
	uint32 startTrsId, endTrsId;
	uint32	status,  index, i;
	int8	line[16], *cPtr;
	strSfcMemPointerInfo sfcMemPtr;
	strTransitionInfo *trsInfoPtr;

	printf("\tStart Transition ID (1-%d) : ", MAX_TRANSITOIN_NUM);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    startTrsId = atoi((const char *)cPtr);

    printf("\tEnd Transition ID (%d-%d)  : ",startTrsId+1,MAX_TRANSITOIN_NUM);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    endTrsId = atoi((const char *)cPtr);


	if( ((endTrsId - startTrsId) <= 0) ||
    	(startTrsId >= endTrsId) ||  (startTrsId < 1) ||
        (endTrsId > MAX_TRANSITOIN_NUM)
	) {
    	printf( "\n");
		printf( "\t[ERROR] - Invalid Transition Id (Start : %d,End : %d)\n",
        				startTrsId, endTrsId);
		return;
    }

    index = 1;
   	printf( "\n");
	printf( " +====+========+====+========+====+========+====+========+====+========+\n");
	printf( " | ID |TRS:TASK| ID |TRS:TASK| ID |TRS:TASK| ID |TRS:TASK| ID |TRS:TASK|\n");
	printf( " +====+========+====+========+====+========+====+========+====+========+\n");

    for(i = startTrsId; i <= endTrsId; i++) {
    	memoryClear( (uint8 *) &sfcMemPtr, sizeof(strSfcMemPointerInfo));

		status = sfcMemPointerGet(LOCAL_MEM_ACC, &sfcMemPtr);
		if(status != NO_ERROR) {
			printf("[ERROR] - sfcMemPointerGet() Call Err !!\n");
            return;
        }
		trsInfoPtr = (strTransitionInfo*)sfcMemPtr.trsMemPtr;
		trsInfoPtr += (i - 1);

        printf(" [T%03d]", i);
      
        if(trsInfoPtr->transitionID != 0)
	        printf("%03d:%03d", trsInfoPtr->transitionID,  trsInfoPtr->taskID);
  		else
        	printf("       ");

        if( !(index % 5)) printf( "\n");
        index++;
	}
}


static void sfcActionMemoryDisplay(void)
{
	uint32 startActId, endActId;
	uint32	status,  index, i;
	int8	line[16], *cPtr;
	strSfcMemPointerInfo sfcMemPtr;
	strActionInfo *actInfoPtr;
	strActionCtrlInfo actCtrlData;

	printf("\tStart Action ID (1-%d) : ", MAX_ACTION_NUM);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    startActId = atoi((const char *)cPtr);

    printf("\tEnd Action ID (%d-%d)  : ",startActId+1,MAX_ACTION_NUM);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    endActId = atoi((const char *)cPtr);


	if( ((endActId - startActId) <= 0) ||
    	(startActId >= endActId) ||  (startActId < 1) ||
        (endActId > MAX_ACTION_NUM)
	) {
    	printf( "\n");
		printf( "\t[ERROR] - Invalid Action Id (Start : %d,End : %d)\n",
        				startActId, endActId);
		return;
    }

    index = 1;
   	printf( "\n");
	printf( " +====+========+====+========+====+========+====+========+====+========+\n");
	printf( " | ID |ACT:TASK| ID |ACT:TASK| ID |ACT:TASK| ID |ACT:TASK| ID |ACT:TASK|\n");
	printf( " +====+========+====+========+====+========+====+========+====+========+\n");
    for(i = startActId; i <= endActId; i++) {
    	memoryClear( (uint8 *) &sfcMemPtr, sizeof(strSfcMemPointerInfo));

		status = sfcMemPointerGet(LOCAL_MEM_ACC, &sfcMemPtr);
		if(status != NO_ERROR) {
			printf("[ERROR] - sfcMemPointerGet() Call Err !!\n");
            return;
        }
		actInfoPtr = (strActionInfo*)sfcMemPtr.actMemPtr;
		actInfoPtr += (i - 1);

        printf(" [A%03d]", i);
      
        if(actInfoPtr->actionID != 0){
			memoryCopy((uint8*)&actCtrlData,(uint8*)&actInfoPtr->actCtrlData,sizeof(strActionCtrlInfo));
	        printf("%03d:%03d", actInfoPtr->actionID,actInfoPtr->taskID);
        }
  		else
        	printf("       ");

        if( !(index % 5)) printf( "\n");
        index++;
	}
}


static uint32 actCurInfoSet(uint32 taskId, uint32 actId)
{
	uint32	status = NO_ERROR;
	strActionInfo *actInfoPtr, *actStartInfoPtr;
	strActionLinkInfo *curActLinkPtr;
	strActionCtrlInfo *actCtrlInfoPtr;

	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
			status = L_SEG_ID_ERR;
			setErrorCode( __FILE__, __LINE__, "actCurInfoSet",status);
			return(status);
	}

	
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
 	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__, __LINE__,
						"actCurInfoSet", status
					  );
		return(status);
	}

	actStartInfoPtr = gSFCActionInfoLocalPtr;

	actInfoPtr = actStartInfoPtr + (actId-1) ;
	if(actInfoPtr->taskID != taskId) goto MODULE_END;

	
	actCtrlInfoPtr = (strActionCtrlInfo*)&actInfoPtr->actCtrlData;
	if((actCtrlInfoPtr->actCX ==1)||(actCtrlInfoPtr->actCtrlQ==0)) goto MODULE_END;

	

	curActLinkPtr = (strActionLinkInfo *)nodeGetAndUnlink (
							(strDoubleLink **)&sActCurQMngrInfo.freeList
            										);

	if(curActLinkPtr == NULL) {
		status = NO_EXIST_NODE_ERR;
		setErrorCode( __FILE__, __LINE__, "actCurInfoSet",status);
		goto MODULE_END;	
	}
	else {
		curActLinkPtr->taskId = actInfoPtr->taskID;
		curActLinkPtr->actionId = actInfoPtr->actionID;

		status = nodePutOnLast( (strDoubleLink **) &sActCurQMngrInfo.usedList[taskId - 1],
             		            (strDoubleLink *)curActLinkPtr
                		      );
		if(status) {
		    setErrorCode( __FILE__, __LINE__, "actCurInfoSet",status);
			goto MODULE_END;	
		}
		actCtrlInfoPtr->actCX = 1;
	}


MODULE_END:
	
#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif
	return(status);
}



uint32 actionCurInfoUnlink(uint32 taskId, strActionLinkInfo *actLinkInfoPtr)
{
	uint32	status = NO_ERROR;
	strActionInfo *actInfoPtr, *actStartInfoPtr;
	strActionCtrlInfo  *actCtrlDataPtr;

	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
		status = L_SEG_ID_ERR;
		setErrorCode( __FILE__, __LINE__, "actionCurInfoUnlink",status);
		return(status);
	}

	actStartInfoPtr = gSFCActionInfoLocalPtr;
	actInfoPtr = actStartInfoPtr + (actLinkInfoPtr->actionId-1) ;
	if(actLinkInfoPtr->actionId != actInfoPtr->actionID) {
		status = L_ACTION_ID_ERR;
		setErrorCode( __FILE__, __LINE__, "actionCurInfoUnlink",status);
		return(status);
	}

	if(actInfoPtr->taskID != taskId) {
		status = L_SEG_ID_ERR;
		setErrorCode( __FILE__, __LINE__, "actionCurInfoUnlink",status);
		return(status);
	}

	
	actCtrlDataPtr = (strActionCtrlInfo*)&actInfoPtr->actCtrlData;
	if(actCtrlDataPtr->actCtrlQ == 1) return(status);


	
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__, __LINE__,
						"actionCurInfoClear", status
					  );
		return(status);
	}

	status = nodeUnlink( (strDoubleLink **) &sActCurQMngrInfo.usedList[taskId - 1],
      (strDoubleLink *)actLinkInfoPtr);
	if(status != NO_ERROR) {
		setErrorCode( __FILE__, __LINE__, "actionCurInfoUnlink",status);
		goto MODULE_END;	
	}
	
	actCtrlDataPtr->actCX = 0;

	actLinkInfoPtr -> actionId =0;
	actLinkInfoPtr -> taskId =0;
	status = nodePutOnLast( (strDoubleLink **) &sActCurQMngrInfo.freeList,
            (strDoubleLink *)actLinkInfoPtr
          );
	if(status != NO_ERROR) {
		setErrorCode( __FILE__, __LINE__, "actionCurInfoUnlink",status);
		goto MODULE_END;	
	}

MODULE_END:
	
#ifdef VXWORKS
   semaphoreRelease (gSFCSequncerQSemaId);	
#else
   semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif
	return(status);
}


static uint32	actionCurQueInit(void)
{

	uint32	status = NO_ERROR;
	uint32	loop;

    
    sActCurQMngrInfo.freeList = NULL;
	for(loop = 0; loop < MAX_LOGIC_TASK_NUM; loop++) 
    	sActCurQMngrInfo.usedList[loop] = NULL;

    for( loop = 0; loop < MAX_ACTION_NUM; loop++) {
		memoryClear (   (char *) &sActCurDataInfo[loop],
                         sizeof(strActionLinkInfo)
                    );
        status = nodePutOnLast	(	(strDoubleLink **) &sActCurQMngrInfo.freeList,
                                	(strDoubleLink *)  &sActCurDataInfo[loop]
                            	);
		if(status != NO_ERROR)
			break;                            
    }
	
	return(status);
}


static uint32 actCurInfoClear(uint32 taskId)
{
	uint32	status = NO_ERROR;
	strActionLinkInfo	*curDataPtr;
		
	if((taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)){
		status = L_SEG_ID_ERR;
        setErrorCode( __FILE__, __LINE__, "actCurInfoClear",status);
		return(status);
	}

	
	if(sActCurQMngrInfo.usedList[taskId - 1] == NULL) return(status);

	
    status = semaphoreGet(gFbExecSeqIdQSemaId, ONLINE_DWL_SEMA_GET_TIME_OUT);
	
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__,
						__LINE__,
						"actLinkInfoClear",
						status
					  );
		return(status);
	}
	
    while( sActCurQMngrInfo.usedList[taskId - 1]!= NULL) {
        
   		curDataPtr = (strActionLinkInfo *)nodeGetAndUnlink(
                    				(strDoubleLink **)&sActCurQMngrInfo.usedList[taskId - 1]
															);
       	if(curDataPtr != NULL){
           	
           	memoryClear( (uint8 *)curDataPtr, sizeof(strActionLinkInfo));
           	(void)nodePutOnLast( (strDoubleLink **) &sActCurQMngrInfo.freeList,
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


uint32	sfcSegTblClear(uint32 taskId, uint32 dwlKind)
{
	uint32 status = NO_ERROR;
	strFbTaskTblMemInfo* segTblPtr;
	
	status = segTblPointerGet(LOGIC_ID ,taskId	,&segTblPtr);
	if(status == NO_ERROR) {
		if(segTblPtr->logicType != SFC_LOGIC_TYPE) return(status);

		if(dwlKind == SEG_DWL) {
    		segTblPtr->memInputSize = 0;
    		segTblPtr->memOutputSize = 0;
			segTblPtr->fbUsedNum = 0;
			segTblPtr->taskElpTime = 0;
			segTblPtr->logicCheckSum = 0;
		}
		else {
			memoryClear ( (uint8 *) segTblPtr, sizeof(strFbTaskTblMemInfo));
		}
	}
	else{
		setErrorCode(__FILE__,__LINE__,"sfcSegTblClear",status);
		return(status);
	}

	status = busSegTblPointerGet(LOGIC_ID ,taskId	,&segTblPtr);
	if(status == NO_ERROR){
		if(segTblPtr->logicType != SFC_LOGIC_TYPE) return(status);

		if(dwlKind == SEG_DWL) {
    		segTblPtr->memInputSize = 0;
    		segTblPtr->memOutputSize = 0;
			segTblPtr->fbUsedNum = 0;
			segTblPtr->taskElpTime = 0;
			segTblPtr->logicCheckSum = 0;
		}
		else {
			memoryClear ( (uint8 *) segTblPtr, sizeof(strFbTaskTblMemInfo));
		}
	}
	else{
		setErrorCode(__FILE__,__LINE__,"sfcSegTblClear",status);
		return(status);
	}

	return(status);
}


void sfcQueueCountDisplay()
{
	uint32	freeListCount = 0;
    uint32	usedListCount = 0;
	uint32	prevListCount = 0;
	uint32	status = NO_ERROR;
	void	*nodePtr;
	uint32 i = 0;


 	
    
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR) {
		printf( "[SFC TRS Queue] - [Get Semaphore Error]\n");
        goto MODULE_END;
    }
	freeListCount = 0; 
	usedListCount = 0;

	nodePtr = (void*)sTrsLinkQMngrInfo.freeList;
    while(nodePtr != NULL) {
    	freeListCount++;

        nodePtr = (void *)nextNodeRef ( (strDoubleLink *)sTrsLinkQMngrInfo.freeList,
	                        					(strDoubleLink *)nodePtr
							  					);
	}

	for(i=0; i<MAX_LOGIC_TASK_NUM;i++) {
		nodePtr = (void*)sTrsLinkQMngrInfo.usedList[i];
		while(nodePtr != NULL) {
    		usedListCount++;

			nodePtr = (void *)nextNodeRef ( (strDoubleLink *)sTrsLinkQMngrInfo.usedList[i],
                                					(strDoubleLink *)nodePtr
								  					);	
		} 
	}

#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif

    
    printf( "\n\t*******[TRANSITION LINK MAX Queue : %d]******\n",
                MAX_TRANSITOIN_NUM
            );
	printf( "\tFreeList : %d		UsedList : %d\n",
    						freeListCount, usedListCount
			);
	
    
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR) {
		printf( "[SFC ACT Queue] - [Get Semaphore Error]\n");
        goto MODULE_END;
    }
	freeListCount = 0; 
	usedListCount = 0;

	nodePtr = (void *)sStepLinkQMngrInfo.freeList;
    while(nodePtr != NULL) {
    	freeListCount++;

        nodePtr = (void *)nextNodeRef ( (strDoubleLink *)sStepLinkQMngrInfo.freeList,
	                        					(strDoubleLink *)nodePtr
							  					);
	}

	for(i=0; i<MAX_STEP_NUM;i++) {
		nodePtr = (void *)sStepLinkQMngrInfo.usedList[i];
		while(nodePtr != NULL) {
    		usedListCount++;

			nodePtr = (void *)nextNodeRef ( (strDoubleLink *)sStepLinkQMngrInfo.usedList[i],
                                					(strDoubleLink *)nodePtr
								  					);	
		} 
	}

#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif

    
    printf( "\n\t*******[STEP LINK ACTION MAX Queue : %d]******\n",
                MAX_SFC_EXEC_NUM
            );
	printf( "\tFreeList : %d		UsedList : %d\n",
    						freeListCount, usedListCount
			);

	
    
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR) {
		printf( "[SFC EXE Queue] - [Get Semaphore Error]\n");
        goto MODULE_END;
    }

	freeListCount = 0; 
	usedListCount = 0;
	nodePtr = (void *)sSfcExecQMngrInfo.freeList;
    while(nodePtr != NULL) {
    	freeListCount++;

        nodePtr = (void *)nextNodeRef ( (strDoubleLink *)sSfcExecQMngrInfo.freeList,
	                        					(strDoubleLink *)nodePtr
							  					);
	}

	for(i=0; i<MAX_LOGIC_TASK_NUM;i++) {
		nodePtr = (void *)sSfcExecQMngrInfo.curExeList[i];
		while(nodePtr != NULL) {
    		usedListCount++;

			nodePtr = (void *)nextNodeRef ( (strDoubleLink *)sSfcExecQMngrInfo.curExeList[i],
                                					(strDoubleLink *)nodePtr
								  					);	
		} 
	}
#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif

    
    printf( "\n\t*******[SFC EXE MAX Queue : %d]******\n",
                MAX_VERTEX_NUM
            );
	printf( "\tFreeList : %d		UsedList : %d\n",
    						freeListCount, usedListCount
			);

	
    
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR) {
		printf( "[SFC SEQ Queue] - [Get Semaphore Error]\n");
        goto MODULE_END;
    }

	freeListCount = 0; 
	usedListCount = 0;
	nodePtr = (void *)sSfcSequencerQMngrInfo.freeList;
    while(nodePtr != NULL) {
    	freeListCount++;

        nodePtr = (void *)nextNodeRef ( (strDoubleLink *)sSfcSequencerQMngrInfo.freeList,
	                        					(strDoubleLink *)nodePtr
							  					);
	}

	for(i=0; i<MAX_LOGIC_TASK_NUM;i++) {
		nodePtr = (void *)sSfcSequencerQMngrInfo.usedList[i];
		while(nodePtr != NULL) {
    		usedListCount++;

			nodePtr = (void *)nextNodeRef ( (strDoubleLink *)sSfcSequencerQMngrInfo.usedList[i],
                                					(strDoubleLink *)nodePtr
								  					);	
		} 
	}
#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif
    
    printf( "\n\t*******[SFC SEQ MAX Queue : %d]******\n",
                MAX_VERTEX_NUM
            );
	printf( "\tFreeList : %d		UsedList : %d\n",
    						freeListCount, usedListCount
			);
	
    
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR) {
		printf( "[SFC ADJACENT Queue] - [Get Semaphore Error]\n");
        goto MODULE_END;
    }

	freeListCount = 0; 
	usedListCount = 0;
	prevListCount = 0;
	nodePtr = (void *)sSfcAdjacentQMngrInfo.freeList;
    while(nodePtr != NULL) {
    	freeListCount++;

        nodePtr = (void *)nextNodeRef ( (strDoubleLink *)sSfcAdjacentQMngrInfo.freeList,
	                        					(strDoubleLink *)nodePtr
							  					);
	}

	for(i=0; i<MAX_VERTEX_NUM;i++) {
		nodePtr = (void *)sSfcAdjacentQMngrInfo.prevList[i];
		while(nodePtr != NULL) {
    		prevListCount++;

			nodePtr = (void *)nextNodeRef ( (strDoubleLink *)sSfcAdjacentQMngrInfo.prevList[i],
                                					(strDoubleLink *)nodePtr
								  					);	
		} 
	}

	for(i=0; i<MAX_VERTEX_NUM;i++) {
		nodePtr = (void *)sSfcAdjacentQMngrInfo.nextList[i];
		while(nodePtr != NULL) {
    		usedListCount++;

			nodePtr = (void *)nextNodeRef ( (strDoubleLink *)sSfcAdjacentQMngrInfo.nextList[i],
                                					(strDoubleLink *)nodePtr
								  					);	
		} 
	}
#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif
    
    printf( "\n\t*******[SFC ADJACENT MAX Queue : %d]******\n",
                MAX_SFC_EXEC_NUM*2
            );
	printf( "\tFreeList : %d	PrevList : %d  NextList : %d\n",
    						freeListCount, prevListCount, usedListCount
			);
 	
    
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR) {
		printf( "[SFC ACT Queue] - [Get Semaphore Error]\n");
        goto MODULE_END;
    }
	freeListCount = 0;
    usedListCount = 0;

	nodePtr = (void*)sActLinkQMngrInfo.freeList;
    while(nodePtr != NULL) {
    	freeListCount++;

        nodePtr = (void *)nextNodeRef ( (strDoubleLink *)sActLinkQMngrInfo.freeList,
	                        					(strDoubleLink *)nodePtr
							  					);
	}

	for(i=0; i<MAX_LOGIC_TASK_NUM;i++) {
		nodePtr = (void*)sActLinkQMngrInfo.usedList[i];
		while(nodePtr != NULL) {
    		usedListCount++;

			nodePtr = (void *)nextNodeRef ( (strDoubleLink *)sActLinkQMngrInfo.usedList[i],
                                					(strDoubleLink *)nodePtr
								  					);	
		} 
	}

#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif

    
    printf( "\n\t*******[ACT LINK MAX Queue : %d]******\n",
                MAX_ACTION_NUM
            );
	printf( "\tFreeList : %d		UsedList : %d\n",
    						freeListCount, usedListCount
			);

	
    
    status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR) {
		printf( "[CUR ACT Queue] - [Get Semaphore Error]\n");
        goto MODULE_END;
    }
	freeListCount = 0; 
	usedListCount = 0;

	nodePtr = (void*)sActCurQMngrInfo.freeList;
    while(nodePtr != NULL) {
    	freeListCount++;

        nodePtr = (void *)nextNodeRef ( (strDoubleLink *)sActCurQMngrInfo.freeList,
	                        					(strDoubleLink *)nodePtr
							  					);
	}

	for(i=0; i<MAX_LOGIC_TASK_NUM;i++) {
		nodePtr = (void*)sActCurQMngrInfo.usedList[i];
		while(nodePtr != NULL) {
    		usedListCount++;

			nodePtr = (void *)nextNodeRef ( (strDoubleLink *)sActCurQMngrInfo.usedList[i],
                                					(strDoubleLink *)nodePtr
								  					);	
		} 
	}

#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif

    
    printf( "\n\t*******[CUR ACT MAX Queue : %d]******\n",
                MAX_ACTION_NUM
            );
	printf( "\tFreeList : %d		UsedList : %d\n",
    						freeListCount, usedListCount
			);

MODULE_END:
	return;
}

uint32 actionCurrentDebugSet(uint32 taskId)
{
	uint32 status = NO_ERROR;
	strActionLinkInfo *actLinkInfoPtr;

	if(taskId != sActCurDbgData.curSegId) return(status);
	sActCurDbgData.curActNum = 0;
 
	status = semaphoreGet(gSFCSequncerQSemaId, SFC_SEQ_SEMA_GET_TIME_OUT);
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__, __LINE__,
						"actionCurrentDebugSet",status
					  );
		return(status);
    }

	actLinkInfoPtr = actCurStartMemInfoGet(taskId);
	while(actLinkInfoPtr!=NULL)
	{
		if(actLinkInfoPtr->taskId == taskId) 
		{
			sActCurDbgData.curActId[sActCurDbgData.curActNum++] = actLinkInfoPtr->actionId;
			
		}
		actLinkInfoPtr = actCurNextMemInfoGet(taskId, actLinkInfoPtr);
	} 
#ifdef VXWORKS
    semaphoreRelease (gSFCSequncerQSemaId);	
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,gSFCSequncerQSemaId);	
#endif

	return(status);
}

extern strSfcStepJumpInfo gSFCStepJumpData[MAX_LOGIC_TASK_NUM];
uint32 sfcStepJumpDataSet(uint32 taskId,uint32 stepId,uint32* retValPtr)
{
	uint32 status = NO_ERROR;
	strSegStatusInfo	segStatus;
	strSfcMemPointerInfo sfcMemPtr;
	strStepInfo* stepInfoPtr;

	
	if(	(stepId < 1) || (stepId > MAX_STEP_NUM)) {
		status = L_STEP_ID_ERR;			
		setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
								(uint8 *)"sfcStepJumpDataSet", status
			  		  		);
	}

	status=sfcMemPointerGet(LOCAL_MEM_ACC, &sfcMemPtr);
	if(status != NO_ERROR){
		setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
						(uint8 *)"sfcStepJumpDataSet",status
			  		  	);
		return(status);
	}

	
	stepInfoPtr = (strStepInfo *)sfcMemPtr.stepMemPtr;
	stepInfoPtr += (stepId -1);

	if(stepInfoPtr->stepID != stepId) {
		status = L_STEP_ID_ERR;			
		setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
								(uint8 *)"sfcStepJumpDataSet", status
			  		  		);
	}

	
	status = segmentStatusRead (LOGIC_ID, taskId, &segStatus);
	if(status != NO_ERROR) {
		setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
						(uint8 *)"sfcStepJumpDataSet",status
			  		  	);
		return status;
	}

	
	if(segStatus.taskMode == PCS_RUN){
		memoryClear((uint8*)&gSFCStepJumpData[taskId-1],sizeof(strSfcStepJumpInfo));
		gSFCStepJumpData[taskId-1].taskID = taskId;
		gSFCStepJumpData[taskId-1].jmpStepID = stepId;
		gSFCStepJumpData[taskId-1].jmpStepFlag = 1;
	}
	else{
		status = L_PCS_MODE_ERR;
		setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
						(uint8 *)"sfcStepJumpDataSet",status
			  		  	);
		return status;
	}
	return(status);
}


uint32 sfcStepJumpCheckAndRun(uint32 taskId,uint32 cycleTime, uint32* JmpFlagPtr)
{
	uint32 status = NO_ERROR;
	strSfcExecutionInfo* sfcExecInfoPtr;
	strSfcSequcerInfo *curSeqStepPtr,*jmpSeqStepPtr;

	
	if( (taskId != gSFCStepJumpData[taskId-1].taskID) ||
		(gSFCStepJumpData[taskId-1].jmpStepFlag == 0)
	){
		*JmpFlagPtr = 0;
		return(status);
	}

	if(gSFCStepJumpData[taskId-1].jmpStepFlag == 1 )
	{
		sfcExecInfoPtr = sfcCurrencExecIdStartMemInfoGet(taskId);
		while(sfcExecInfoPtr!=NULL) 
		{
			curSeqStepPtr =sfcExecInfoPtr->sequencerId;
			
			sfcExecInfoPtr = sfcCurrentExecIdNextMemInfoGet(taskId,sfcExecInfoPtr);
			if(curSeqStepPtr!=NULL) 
			{
				status = sfcStep0ffRunFunc(curSeqStepPtr);
				if(status!=NO_ERROR)	{
					setErrorCode(	__FILE__, __LINE__,	"sfcStepJumpCheckAndRun", status );
				}
				sfcExecCurrentInfoUnlink(taskId,curSeqStepPtr);

				curSeqStepPtr->visitFlag = INACTIVE_FLAG;
			}
		}

		
		jmpSeqStepPtr = getSequencerId( gSFCStepJumpData[taskId-1].jmpStepID, taskId, STEP_TYPE)  ;
		jmpSeqStepPtr->visitFlag = INACTIVE_FLAG;
		
		status = sfcExecCurrentInfoPut(taskId, jmpSeqStepPtr);
		if(status!=NO_ERROR)	setErrorCode(	__FILE__, __LINE__,	"sfcSegCtrlRunFunc", status);

		status = sfcStepOnRunFunc(jmpSeqStepPtr,cycleTime);
		if(status!=NO_ERROR)	setErrorCode(	__FILE__, __LINE__,	"sfcSegCtrlRunFunc", status  );

		
		if(status != NO_ERROR) gSFCStepJumpData[taskId-1].jmpStepStatus = status;
		else{
			gSFCStepJumpData[taskId-1].jmpStepStatus = NO_ERROR;
		}
		*JmpFlagPtr = 1;
		gSFCStepJumpData[taskId-1].jmpStepFlag = 0;

	}
	
	return(status);
}
