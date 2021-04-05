
#include	<stdio.h>
#include	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"

#include	"systemDiag.h"
#include	"usrDefBuf.h"
#include	"segScheduler.h"


#include	"fbDef.h"
#include	"fbMem.h"
#include	"usrErrCode.h"
#include	"sysDual.h"
#include	"sysConfig.h"

#include	"usrQueData.h"
#include	"osDepend.h"
#include	"usrSema.h"
#include	"sfcMem.h"

uint32	gStartTimer = 0;
uint32	gEndTimer = 0;

strExecTimeInfo gTrsExecTime[MAX_TRANSITOIN_NUM];
strExecTimeInfo gActExecTime[MAX_ACTION_NUM];
strSfcStepJumpInfo gSFCStepJumpData[MAX_LOGIC_TASK_NUM];


extern strMpuSegSchedulerInfo	*gMpuSegSchdInfoPtr; 

extern strFbMemInfo		*gMpuFbMemInfoLocalPtr;
extern strFbExecDbInfo	gFbCodeSortingTableInfo[MAX_FB_CODE_ID+1];

extern int32 gFbExecSeqIdQSemaId;

extern uint32	gMpuTaskSchdCount[MAX_LOGIC_TASK_NUM];
extern uint32	gSystemInitFlag[MAX_LOGIC_TASK_NUM];	


extern strTransitionInfo *gSFCTransitionLocalPtr;
extern strActionInfo *gSFCActionInfoLocalPtr;


uint32	taskLogicCtrlRunFunc(uint32);

uint32	fbSegCtrlRunFunc(strFbTaskTblMemInfo*);
uint32	sfcSegCtrlRunFunc(strFbTaskTblMemInfo *);

uint32	sfcTrsFbcodeRunFunc(uint32,strMpuSegExecInfo*, uint32*);
uint32	sfcActFbcodeRunFunc(uint32,strMpuSegExecInfo*, uint32*);


extern void   memoryClear(uint8 *, uint32);
extern void   memoryCopy(uint8 *, uint8 *, uint32);

extern uint32 readRuntimeFbData(uint32,uint32,uint32,uint32 *);
extern void	  setErrorCode(uint8 *, uint32, uint8 *, uint32);
extern void   setErrorCodeWithVal(uint8 *, uint32, uint8 *, uint32,
				uint8 *, uint32, uint8 *, uint32, uint8 *, uint32);
extern uint32 systemTickGet(void);
extern uint32 fbCodeMemInfoRead(uint32, uint32, strFbMemInfo *);
extern uint32 writeRuntimeFbData(uint32, uint32,uint32, uint32 *);
extern uint32 systemClockRateGet(void);

extern void	  cyclicSegExecTimeCalcu(int32, strExecTimeInfo *);

extern uint32 sysAuxTickCountGet();

extern int32  semaphoreGet(int32 , int32 );
#ifdef VXWORKS
extern int32  semaphoreRelease( int32);
#else
extern int32  semaphoreRelease(int32, int32);
extern int32	semaphoreRelease2(uint8 *, uint32, int32, int32);
#endif

extern uint32 getFbTaskTblInfo (uint32, uint32, strFbTaskTblMemInfo *);
extern uint32 segTblInfoWrite(uint32 , uint32 , strFbTaskTblMemInfo *);


extern strFbExecutionInfo *fbExecIdStartMemInfoGet(uint32 );
extern strFbExecutionInfo *fbExecIdNextMemInfoGet(uint32 , strFbExecutionInfo *);
extern strFbExecutionInfo *trsExecIdNextMemInfoGet(uint32 , strFbExecutionInfo *);
extern strFbExecutionInfo *actExecIdNextMemInfoGet(uint32 , strFbExecutionInfo *);


extern uint32 jmpFbCodeCheck(uint32);
extern strFbExecutionInfo* jmpFbExecIdNextMemInfoGet(uint32, strFbExecutionInfo *);
extern uint32 jumpCountVarCheck(uint32 , uint32, uint32*);

extern strFbExecutionInfo *nonCyclicFbExecIdStartMemInfoGet(uint32 , uint32 );


extern strSfcSequcerInfo *sfcSequencerStartMemInfoGet(uint32);
extern strSfcSequcerInfo *sfcSequencerNextMemInfoGet(uint32, strSfcSequcerInfo *);

extern strTrsLinkInfo *trsLinkStartMemInfoGet(uint32);
extern strTrsLinkInfo *trsLinkNextMemInfoGet(uint32, strTrsLinkInfo *);
extern strActionLinkInfo *actLinkNextMemInfoGet(uint32 , strActionLinkInfo *);
extern strActionLinkInfo *actLinkStartMemInfoGet(uint32);
extern strActionLinkInfo *actCurNextMemInfoGet(uint32 , strActionLinkInfo *);
extern strActionLinkInfo *actCurStartMemInfoGet(uint32);

extern strFbExecutionInfo *trsExecIdStartMemInfoGet(uint32);
extern strFbExecutionInfo *actExecIdStartMemInfoGet(uint32);

extern strSfcExecutionInfo *sfcCurrencExecIdStartMemInfoGet(uint32 );
extern strSfcExecutionInfo *sfcCurrentExecIdNextMemInfoGet(uint32 , strSfcExecutionInfo *);

extern uint32 sfcExecCurrentInfoUnlink(uint32, strSfcSequcerInfo *);
extern uint32 sfcExecCurrentInfoPut(uint32, strSfcSequcerInfo *);
extern strSfcExecutionInfo* sfcExecCurrentInfoGet(uint32);

extern uint32 trsActiveStatusRead(uint32, uint32*);
extern uint32 actActiveStatusRead(uint32, uint32*);

extern uint32 sfcStepOnRunFunc(strSfcSequcerInfo *,uint32);
extern uint32 sfcStep0ffRunFunc(strSfcSequcerInfo *);
extern uint32 sfcActionCtrlRunning(uint32 );

extern uint32 sfcStepCurretStatusGet(strSfcSequcerInfo *,uint32 *);
extern uint32 sfcStepActiveStatusGet(strSfcSequcerInfo *,uint32 *);
extern uint32 sfcStepCurretStatusSet(strSfcSequcerInfo *, uint32 );

extern uint32 sfcPrevStepCXAllOff(strSfcAdjacentInfo*,uint32);
extern strSfcSequcerInfo* sfcNextTransitionCheck(strSfcAdjacentInfo*, uint32* );

extern strDoubleLink *nodeGetAndUnlink(strDoubleLink **);
extern int32 nodePutOnLast(strDoubleLink **,strDoubleLink *);
extern strDoubleLink *nextNodeRef(strDoubleLink *,strDoubleLink *);
extern uint32  refDataCopyFromMem(uint32*, uint32, strFbSpecActionInfo, uint32, uint32);
extern void   systemRasInfoSet(uint32 , uint32 );

extern uint32 segTblTickCountWrite(uint32,uint32,strFbTaskTblMemInfo	*);
extern uint32 allSegmentModeStopSet(uint32 );


extern uint32 actionCurInfoUnlink(uint32 , strActionLinkInfo * );
extern uint32 actionCurrentDebugSet(uint32 taskId);

extern uint32 sfcStepJumpCheckAndRun(uint32 ,uint32 , uint32* );


uint32	taskLogicCtrlRunFunc(uint32 taskId)
{
	
	uint32 status = NO_ERROR;
	strMpuSegSchedulerInfo	*schdInfoPtr = gMpuSegSchdInfoPtr; 
	strFbTaskTblMemInfo	segTblInfo;

	if(schdInfoPtr->schedulerMode != SCHEDULER_ENABLE)
    	return(status);

    
    if( (taskId <= 0) || (taskId > MAX_MPU_SCHD_SEG_ID)) 
    {
    	status = L_SEG_ID_ERR;
        setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
	}
    else 
    {
    	memoryClear ( (uint8 *) &segTblInfo, sizeof(strFbTaskTblMemInfo));
    	status = getFbTaskTblInfo( LOGIC_ID, taskId, (strFbTaskTblMemInfo	*) &segTblInfo);
		if (status != NO_ERROR) 
		{
	        setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
 			goto MODULE_END;
		}

		 
	    if( ( (segTblInfo.taskId <= 0) || (segTblInfo.taskId> MAX_MPU_SCHD_SEG_ID) ) &&
			(segTblInfo.execMode == NON_CYCLIC_RUNNING_MODE)) 
		{
 			goto MODULE_END;
		}
	}

	
	
	if(segTblInfo.logicType == SFC_LOGIC_TYPE) 	{
		status = sfcSegCtrlRunFunc(&segTblInfo);
	}
	else {
		status = fbSegCtrlRunFunc(&segTblInfo);
	}
MODULE_END:

	return(status);
}


uint32	fbSegCtrlRunFunc(strFbTaskTblMemInfo* segTblInfo)
{
	strMpuSegSchedulerInfo	*schdInfoPtr = gMpuSegSchdInfoPtr; 
	strMpuSegExecInfo	*execInfoPtr;
	strExecTimeInfo     mpuSegmentTime;

	strFbMemInfo		*fbInfoPtr;
    strFbExecDbInfo		*fbCodeInfoPtr;
	uint32	blockAddr = 0;
    uint32	status = NO_ERROR;
	int32	startTimer = 0, endTimer = 0, execTime = 0 ;
	strFbExecutionInfo	*fbExecIdInfoPtr = NULL;
	uint32 taskId;

	strFbSpecActionInfo *specActionDataPtr = NULL;
	uint32 eventVar;
	uint32 jmpErr =0;

	 
	taskId = segTblInfo->taskId;

    if( (taskId <= 0) || (taskId > MAX_MPU_SCHD_SEG_ID)) 
    {
    	status = L_SEG_ID_ERR;
        setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	if( (segTblInfo->execMode == CYCLIC_RUNNING_MODE) ||
		(segTblInfo->execMode == EVENT_RUNNING_MODE) ) 
	{
		execInfoPtr = (strMpuSegExecInfo *)&schdInfoPtr->cyclicSeg[taskId-1];

		
	    if(segTblInfo->execMode == EVENT_RUNNING_MODE) 
		{
			specActionDataPtr =(strFbSpecActionInfo *)&segTblInfo->u1.evtActFbStartAddr;
			if(specActionDataPtr->refType == SPEC_TYPE_BYREF)
			{
				status =  refDataCopyFromMem(	&eventVar, 
												segTblInfo->u2.evtSetFbEndAddr,
												*specActionDataPtr,
												SIZE32_TYPE,
												0
											);

				if(status != NO_ERROR ) 
				{
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					return(status);
				}
				
				if( (eventVar == ACTIVE_FLAG) && (execInfoPtr->eventPreVar == INACTIVE_FLAG) )
			 		 execInfoPtr->eventTaskRunFlag = EVENT_FLAG_ENABLE; 
				else execInfoPtr->eventTaskRunFlag = EVENT_FLAG_DISABLE; 

				
				if(execInfoPtr->eventVarType == EVENT_EDGE_TYPE) 
					 execInfoPtr->eventPreVar = eventVar;
				else if(execInfoPtr->eventVarType == EVENT_LEVEL_TYPE)
					 execInfoPtr->eventPreVar = INACTIVE_FLAG;
				
				if(execInfoPtr->eventTaskRunFlag == EVENT_FLAG_DISABLE)
					goto MODULE_END;

			}
			else 
			{
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				goto MODULE_END;
			}
		}
		
	   	
#ifdef SYS_AUX_CLK_USED
	    startTimer = sysAuxTickCountGet();
#else
    	startTimer = systemTickGet();
#endif
	}
	else if(segTblInfo->execMode == NON_CYCLIC_RUNNING_MODE) {
		execInfoPtr = (strMpuSegExecInfo *)&schdInfoPtr->nonCyclicSeg;
	   	
        
        if(gStartTimer == 0)
#ifdef SYS_AUX_CLK_USED
			gStartTimer = sysAuxTickCountGet();
#else
    		gStartTimer = systemTickGet();
#endif
	}
	else {
    	status = SEG_EXEC_MODE_ERR;
        setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
       	return(status);
    }

	
	memoryCopy ( 	(uint8 *) &mpuSegmentTime,
					(uint8 *) &execInfoPtr->segExecTime,
					sizeof(strExecTimeInfo)
				); 
	
    
    
    if(segTblInfo->taskMode != PCS_RUN) {goto MODULE_END;}

 	
    status = semaphoreGet(gFbExecSeqIdQSemaId, NOWAIT);
	if(status != NO_ERROR) {
		setErrorCodeWithVal(	__FILE__,__LINE__,
						"fbSegCtrlRunFunc",status,
						"TASK ID",execInfoPtr->taskId,
                        "TASK MODE", segTblInfo->execMode,
						"NOT USING",0
					  	);
		goto MODULE_END;
	}
	
	
	
	if(segTblInfo->execMode == CYCLIC_RUNNING_MODE) {
		fbExecIdInfoPtr = fbExecIdStartMemInfoGet(taskId);
	}
	else if(segTblInfo->execMode == NON_CYCLIC_RUNNING_MODE) {
			
		fbExecIdInfoPtr	= nonCyclicFbExecIdStartMemInfoGet(
															taskId, 
															execInfoPtr->nextExecFbBlockAddr
														  );		
	}
	else if(segTblInfo->execMode == EVENT_RUNNING_MODE){
		if(execInfoPtr->eventTaskRunFlag == EVENT_FLAG_ENABLE) 
		   fbExecIdInfoPtr = fbExecIdStartMemInfoGet(taskId);
	}		
	else {
		status = SEG_EXEC_MODE_ERR;
		setErrorCodeWithVal (	__FILE__,__LINE__,
								"fbSegCtrlRunFunc",status,
								"Read Task Mode", segTblInfo->execMode,
								"Min Task Mode",CYCLIC_RUNNING_MODE,
								"Max Task Mode", EVENT_RUNNING_MODE
						  	);
		
		goto SEMA_RELEASE;	
	}

	
	execInfoPtr->runErrCount = 0;
	

	while(fbExecIdInfoPtr != NULL){ 
		if(	(execInfoPtr->schedulingMode 		== SCHEDULING_RUNNING) &&
			(execInfoPtr->segStatus 			== FB_RUNNING_READY)   &&
            (execInfoPtr->schedulingEnableFlag 	== SCHEDULING_FLAG_ENABLE) 
		){
			
			if(fbExecIdInfoPtr->taskId != taskId) {
				status = L_SEG_ID_ERR;	
				setErrorCodeWithVal (	__FILE__,__LINE__,
										"fbSegCtrlRunFunc", status,
										"Read SegId", fbExecIdInfoPtr->taskId,
										"Cur Seg ID",taskId,
										"Block Addr", fbExecIdInfoPtr->fbBlockAddr
							  );
				goto SEMA_RELEASE;	
							  
			}
			
			if( (fbExecIdInfoPtr->fbBlockAddr > MAX_MPU_BLOCK_ADDR) ||
				(fbExecIdInfoPtr->fbBlockAddr == 0)	)
			{												
				status = L_FB_ADDR_ERR;	
				setErrorCodeWithVal (	__FILE__,	__LINE__,
										"fbSegCtrlRunFunc", status,
										"Read Block Addr", fbExecIdInfoPtr->fbBlockAddr,
										"TASK ID",fbExecIdInfoPtr->taskId,
										"NOT USING", 0
							  			);
				goto SEMA_RELEASE;						  
			}
			blockAddr = fbExecIdInfoPtr->fbBlockAddr;

			fbInfoPtr = gMpuFbMemInfoLocalPtr;
            fbInfoPtr += (blockAddr-1);

			
			
			if( (fbInfoPtr->fbId == 0)
			){ 
				status = L_FB_CODE_ERR;
				setErrorCodeWithVal (	__FILE__,__LINE__,
										"fbSegCtrlRunFunc",	status,
										"Read Block Addr", fbExecIdInfoPtr->fbBlockAddr,
										"Read Fb Code",fbExecIdInfoPtr->fbId,
										"Cur FB Code", fbInfoPtr->fbId
							  			);
				goto NEXT_FB_INFO_GET;
			}

			
			if(fbInfoPtr->forcingFlag == FORCING_ENABLE) goto NEXT_FB_INFO_GET;

			fbCodeInfoPtr = &gFbCodeSortingTableInfo[fbInfoPtr->fbId];

			if(fbCodeInfoPtr->fbId != fbInfoPtr->fbId){
               	status = FC_CODE_EXIST_ERR;
				setErrorCodeWithVal (	__FILE__,__LINE__,
										"fbSegCtrlRunFunc", status,
										"Block Addr", blockAddr,
										"Fb Code",fbInfoPtr->fbId,
										"Not Using", 0
									  );
				goto NEXT_FB_INFO_GET;
			}
			else { 
				status = fbCodeInfoPtr->fbActFunc(taskId, blockAddr); 
				
                if(status != NO_ERROR) {
					
					fbInfoPtr->fbRunStatus = (status & 0xffff);	
                }
                else  
                	fbInfoPtr->fbRunStatus = 0;

			} 	
NEXT_FB_INFO_GET:	
			if(jmpFbCodeCheck(fbExecIdInfoPtr->fbId) == IS_JUMP_CODE){
				status = jumpCountVarCheck(fbExecIdInfoPtr->fbBlockAddr ,
											fbExecIdInfoPtr->taskId, &jmpErr);
				if (status != NO_ERROR)
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);	

				if( jmpErr != NO_ERROR)  
				{	
					segTblInfo->loopStatus = FB_LOOPING_ERR;
					segTblInfo->taskMode = PCS_STOP;
				}
				fbExecIdInfoPtr = jmpFbExecIdNextMemInfoGet(taskId, fbExecIdInfoPtr);
			}
			else 
				fbExecIdInfoPtr = fbExecIdNextMemInfoGet(taskId, fbExecIdInfoPtr);

        } 

        else { 			 
			if(segTblInfo->execMode == NON_CYCLIC_RUNNING_MODE) 
   				execInfoPtr->nextExecFbBlockAddr = blockAddr;
   				
			
           	goto SEMA_RELEASE;
        }
	} 

    if( (segTblInfo->execMode == CYCLIC_RUNNING_MODE) ||
		(segTblInfo->execMode == EVENT_RUNNING_MODE) )
	{
#ifdef SYS_AUX_CLK_USED
	    endTimer = sysAuxTickCountGet();
#else
    	endTimer = systemTickGet();
#endif
        execTime = endTimer - startTimer;
	}
	else {
#ifdef SYS_AUX_CLK_USED
	    gEndTimer = sysAuxTickCountGet();
#else
    	gEndTimer = systemTickGet();
#endif
        execTime = gEndTimer - gStartTimer;
        
        gStartTimer = 0;
        gEndTimer	= 0;
	}

	
	if(gMpuTaskSchdCount[execInfoPtr->taskId - 1] > 0)
		cyclicSegExecTimeCalcu((uint32)execTime, &mpuSegmentTime);

    
	if(++segTblInfo->taskTickCnt > MAX_SEG_EXEC_COUNT)
		segTblInfo->taskTickCnt = 1;

  	
	 
   	execInfoPtr->nextExecFbBlockAddr = execInfoPtr->startFbBlockAddr;
	execInfoPtr->segStatus = FB_RUNNING_DONE;

    
	if(mpuSegmentTime.maxExecTime  > execInfoPtr->segExecTime.maxExecTime)
		execInfoPtr->maxTimeSchdCnt = gMpuTaskSchdCount[execInfoPtr->taskId - 1] + 1;
		
    
	memoryCopy ( 	(uint8 *) &execInfoPtr->segExecTime,
					(uint8 *) &mpuSegmentTime,
					sizeof(strExecTimeInfo)
				); 

	
	
	
	status = segTblTickCountWrite (LOGIC_ID,taskId, segTblInfo); 
	if(status != NO_ERROR)
        setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					  
	segTblInfo->taskElpTime = (int32)mpuSegmentTime.curExecTime;  

	status = segTblInfoWrite(LOGIC_ID, taskId, segTblInfo);
	if(status != NO_ERROR) 
        setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

		
	gMpuTaskSchdCount[execInfoPtr->taskId - 1] =  (++gMpuTaskSchdCount[execInfoPtr->taskId - 1]) % 0x7fffff;

	
	if(gSystemInitFlag[execInfoPtr->taskId - 1] == SYS_INIT_FLAG_CLEAR) 
		gSystemInitFlag[execInfoPtr->taskId - 1] = SYS_INIT_FLAG_DONE;

SEMA_RELEASE:
	 
#ifdef VXWORKS
	semaphoreRelease( gFbExecSeqIdQSemaId); 
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, gFbExecSeqIdQSemaId);  
#endif

MODULE_END:
	
	if(segTblInfo->loopStatus == FB_LOOPING_ERR) {
		allSegmentModeStopSet(LOGIC_ID);
		systemRasInfoSet(RAS_MPU_EXEC_LOOP_STATUS, 1);	
	}

    
    
	if( (segTblInfo->execMode == CYCLIC_RUNNING_MODE)||
		(segTblInfo->execMode == EVENT_RUNNING_MODE) ) {
    	execInfoPtr->segStatus 			= FB_RUNNING_DONE;
    	
    	execInfoPtr->mpuSegPhaseRunFlag = SCHEDULING_FLAG_DISABLE;
	}

    
    
	if(	(segTblInfo->execMode == NON_CYCLIC_RUNNING_MODE) && 
    	(segTblInfo->taskMode != PCS_RUN)
	) {
		gStartTimer = 0;
        gEndTimer	= 0;
    }

	return(status);	    
}


uint32	sfcSegCtrlRunFunc(strFbTaskTblMemInfo* segTblInfoPtr)
{
	strMpuSegSchedulerInfo	*schdInfoPtr = gMpuSegSchdInfoPtr; 
	strMpuSegExecInfo	*execInfoPtr;


    uint32	status = NO_ERROR;
	int32	startTimer = 0, endTimer = 0, execTime = 0;

	strExecTimeInfo     mpuSegmentTime;

	strSfcExecutionInfo *sfcExecInfoPtr; 

	strSfcAdjacentInfo  *nextTrsPtr ,*nextTrsHeadPtr;
	strSfcAdjacentInfo  *nextStepPtr,*nextStepHeadPtr;

	strSfcSequcerInfo   *curSeqStepPtr, *nextSeqTrsPtr;
	strSfcSequcerInfo   *nextSeqStepPtr;

	strTrsLinkInfo		*trsLinkInfoPtr;
	strActionLinkInfo	*actLinkInfoPtr, *curLinkInfoPtr;

	uint32 taskId =0, currFlag;
	uint32 nextTrStatus = INACTIVE_FLAG;
	uint32 actActiveFlag = 0, loopStatus = 0;
	uint32 stepJmpFlag = 0;
	
	if(schdInfoPtr->schedulerMode != SCHEDULER_ENABLE)
    	return(status);

	taskId = segTblInfoPtr->taskId;
	 
    if( (taskId <= 0) || (taskId > MAX_MPU_SCHD_SEG_ID)) 
    {
    	status = L_SEG_ID_ERR;
        setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	
	if(segTblInfoPtr->logicType != SFC_LOGIC_TYPE) 	
	{
		status = L_LOGIC_ID_ERR;
		setErrorCodeWithVal(	__FILE__, __LINE__, 
							"sfcSegCtrlRunFunc", status,
                            "Segment Id", taskId,
                            "Logic Type", segTblInfoPtr->logicType,
                            "Not Using", 0
					  	);
       	return(status);
	}
	

	if(segTblInfoPtr->execMode ==CYCLIC_RUNNING_MODE) {
		execInfoPtr = (strMpuSegExecInfo *)&schdInfoPtr->cyclicSeg[taskId-1];
	   	
#ifdef SYS_AUX_CLK_USED
	    startTimer = sysAuxTickCountGet();
#else
    	startTimer = systemTickGet();
#endif
	}
	else {
		
    	status = SEG_EXEC_MODE_ERR;
		setErrorCodeWithVal(	__FILE__, __LINE__, 
								"sfcSegCtrlRunFunc", status,
                                "Segment Id", taskId,
                                "Exec Mode", segTblInfoPtr->execMode,
                                "Not Using", 0
					  		);
       	return(status);
    }

	
	memoryCopy ( 	(uint8 *) &mpuSegmentTime,
					(uint8 *) &execInfoPtr->segExecTime,
					sizeof(strExecTimeInfo)
				); 
	
    
    
    if(segTblInfoPtr->taskMode != PCS_RUN) {goto MODULE_END;}

 	
    status = semaphoreGet(gFbExecSeqIdQSemaId, NOWAIT);
	
	if(status != NO_ERROR) 
	{
        setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

	
	
	
	status = sfcStepJumpCheckAndRun(taskId, segTblInfoPtr->taskCycleTime,&stepJmpFlag);
	if(status != NO_ERROR) {
		setErrorCodeWithVal (	__FILE__,__LINE__,
				"sfcSegCtrlRunFunc",status,
				"Seg Id", taskId,
				"Jump Flag", stepJmpFlag,
				"Not Using", 0
			  );
	}	
	else if(stepJmpFlag ==1){
		goto STEP_END;
	}	
	
	
	
	
	trsLinkInfoPtr = trsLinkStartMemInfoGet(taskId);
	while(trsLinkInfoPtr!=NULL)
	{
		if(trsLinkInfoPtr->taskId == taskId)
		{
			status = sfcTrsFbcodeRunFunc(trsLinkInfoPtr->transitionId, execInfoPtr, &loopStatus);
			if(status != NO_ERROR)
			{
				setErrorCodeWithVal (	__FILE__,__LINE__,
						"sfcSegCtrlRunFunc",status,
						"Seg Id", trsLinkInfoPtr->taskId,
						"Trs Id",trsLinkInfoPtr->transitionId,
						"Not Using", 0
					  );
			}
			else {
				segTblInfoPtr->loopStatus |= loopStatus;
			}
		}
		else
		{
			
			status = L_SEG_ID_ERR;	
			setErrorCodeWithVal (	__FILE__, __LINE__,
									"sfcSegCtrlRunFunc",status,
									"Read SegId", trsLinkInfoPtr->taskId,
									"Cur Seg ID",taskId,
									"Not Using", 0
						  );
			goto SEMA_RELEASE;	
		}

		trsLinkInfoPtr = trsLinkNextMemInfoGet(taskId, trsLinkInfoPtr);
	}

	
	
	
	
	sfcExecInfoPtr = sfcCurrencExecIdStartMemInfoGet(taskId);
	while(sfcExecInfoPtr!=NULL) 
	{
		curSeqStepPtr =sfcExecInfoPtr->sequencerId;
		
		sfcExecInfoPtr = sfcCurrentExecIdNextMemInfoGet(taskId,sfcExecInfoPtr);
		if(curSeqStepPtr!=NULL) 
		{
			status = sfcStepCurretStatusGet(curSeqStepPtr,&currFlag);
			if(status!=NO_ERROR)	
			{
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			}

			else if(currFlag == INACTIVE_FLAG) 
			{ 
				status = sfcStep0ffRunFunc(curSeqStepPtr);
				
				if(status!=NO_ERROR)	
				{
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				}
				sfcExecCurrentInfoUnlink(taskId,curSeqStepPtr);
			}
			curSeqStepPtr->visitFlag = INACTIVE_FLAG;
		}
	}
	
	sfcExecInfoPtr = sfcCurrencExecIdStartMemInfoGet(taskId);
	while(sfcExecInfoPtr!=NULL) 
	{
		
		curSeqStepPtr = sfcExecInfoPtr->sequencerId;

		if(curSeqStepPtr!=NULL){
			
			if( (curSeqStepPtr->initStep == START_STEP)&&(curSeqStepPtr->nodeType == STEP_TYPE)
				 &&(curSeqStepPtr->visitFlag == INACTIVE_FLAG)
			){
				status = sfcStepOnRunFunc(curSeqStepPtr,segTblInfoPtr->taskCycleTime);
				if(status!=NO_ERROR)	{
					setErrorCodeWithVal(	__FILE__, __LINE__,	
										  "sfcSegCtrlRunFunc", status,
										  "STEP ID", curSeqStepPtr->stepTrId,
										  "TASK Cycle Time", segTblInfoPtr->taskCycleTime,
										  "TASK ID",curSeqStepPtr->taskId);
				}
			}
			else if(segTblInfoPtr->sfcStepKind==ONE_STEP_KIND){
				if(curSeqStepPtr->visitFlag == ACTIVE_FLAG) {
					sfcExecInfoPtr = sfcCurrentExecIdNextMemInfoGet(taskId,sfcExecInfoPtr);
					continue;
				}
			}
			
			nextTrsHeadPtr = (strSfcAdjacentInfo *)curSeqStepPtr->nextDblink;
			nextTrsPtr = nextTrsHeadPtr;
		}
		else
		{
			status = STEP_SEQUENCER_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);	
			goto SEMA_RELEASE;
		}

		
		status = sfcStepCurretStatusGet(curSeqStepPtr,&currFlag);
		if(status!=NO_ERROR)	
		{
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		}

		else if(currFlag == ACTIVE_FLAG) 
		{
			nextSeqTrsPtr = sfcNextTransitionCheck(nextTrsHeadPtr, &nextTrStatus);

			if(nextTrStatus == ACTIVE_FLAG)
			{
				status = sfcPrevStepCXAllOff((strSfcAdjacentInfo*)nextSeqTrsPtr->prevDblink,
									segTblInfoPtr->sfcStepKind);
				if(status!=NO_ERROR)	
				{
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status  );
				}

				nextStepHeadPtr = (strSfcAdjacentInfo *)nextSeqTrsPtr->nextDblink;
				nextStepPtr = nextStepHeadPtr;
				while(nextStepPtr!=NULL) 
				{
					nextSeqStepPtr = nextStepPtr->sequencerId;
					if(nextSeqStepPtr != NULL) 
					{
						if(nextSeqStepPtr->visitFlag==INACTIVE_FLAG) 
						{
							
							status = sfcExecCurrentInfoPut(taskId, nextSeqStepPtr);
							if(status!=NO_ERROR)	
								setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						}
						
						status = sfcStepOnRunFunc(nextSeqStepPtr,segTblInfoPtr->taskCycleTime);
						if(status!=NO_ERROR)	
								setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);				
						
						if(segTblInfoPtr->sfcStepKind==ONE_STEP_KIND){
							status = sfcStepCurretStatusSet(nextSeqStepPtr,ACTIVE_FLAG);
							nextSeqStepPtr->visitFlag = ACTIVE_FLAG;
						}
					}
					nextStepPtr = (strSfcAdjacentInfo *)nextNodeRef(  (strDoubleLink *)nextStepHeadPtr,
																(strDoubleLink *)nextStepPtr);
				}
			}
			
			
			else if(nextTrStatus == INACTIVE_FLAG)	
			{
				
				if(curSeqStepPtr->visitFlag == INACTIVE_FLAG)	
				{
					status = sfcStepOnRunFunc(curSeqStepPtr,segTblInfoPtr->taskCycleTime);
					if(status!=NO_ERROR)	
							setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				}
			}
		}
		
		
		sfcExecInfoPtr = sfcCurrentExecIdNextMemInfoGet(taskId,sfcExecInfoPtr);
	}
STEP_END:	
	
	
	
	status = sfcActionCtrlRunning(taskId);
	if(status != NO_ERROR)
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

	
	actionCurrentDebugSet(taskId);

	actLinkInfoPtr = actCurStartMemInfoGet(taskId);
	while(actLinkInfoPtr!=NULL)
	{
		if(actLinkInfoPtr->taskId == taskId) 
		{
			status = actActiveStatusRead(actLinkInfoPtr->actionId, &actActiveFlag);
			if(status != NO_ERROR){
				setErrorCodeWithVal (	__FILE__,__LINE__,
						"sfcSegCtrlRunFunc",status,
						"Seg Id", actLinkInfoPtr->taskId,
						"Act Id",actLinkInfoPtr->actionId,
						"Not Using", 0
					  );
			}
			
			else{  
				if(actActiveFlag ==ACTIVE_FLAG)  
				{
					status = sfcActFbcodeRunFunc(actLinkInfoPtr->actionId, execInfoPtr, &loopStatus);
					if(status != NO_ERROR)
					{
						setErrorCodeWithVal (	__FILE__,__LINE__,
								"sfcSegCtrlRunFunc",status,
								"Seg Id", actLinkInfoPtr->taskId,
								"Act Id",actLinkInfoPtr->actionId,
								"Not Using", 0
							  );

					}
					else segTblInfoPtr->loopStatus |= loopStatus;
				}
			}
		}
		curLinkInfoPtr= actLinkInfoPtr; 
		actLinkInfoPtr = actCurNextMemInfoGet(taskId, actLinkInfoPtr);
		actionCurInfoUnlink(taskId, curLinkInfoPtr);
	}
	
    if(segTblInfoPtr->execMode == CYCLIC_RUNNING_MODE){

#ifdef SYS_AUX_CLK_USED
	    endTimer = sysAuxTickCountGet();
#else
    	endTimer = systemTickGet();
#endif
        execTime = endTimer - startTimer;
	}
	
	if(gMpuTaskSchdCount[execInfoPtr->taskId - 1] > 0)
		cyclicSegExecTimeCalcu((uint32)execTime, &mpuSegmentTime);

    
	if(++segTblInfoPtr->taskTickCnt > MAX_SEG_EXEC_COUNT)
		segTblInfoPtr->taskTickCnt = 1;

  	
	 
   	execInfoPtr->nextExecFbBlockAddr = execInfoPtr->startFbBlockAddr;
	execInfoPtr->segStatus = FB_RUNNING_DONE;

	
	if(mpuSegmentTime.maxExecTime  > execInfoPtr->segExecTime.maxExecTime)
		execInfoPtr->maxTimeSchdCnt = gMpuTaskSchdCount[execInfoPtr->taskId - 1] + 1;

    
	memoryCopy ( 	(uint8 *) &execInfoPtr->segExecTime,
					(uint8 *) &mpuSegmentTime,
					sizeof(strExecTimeInfo)
				); 

	if(segTblInfoPtr->loopStatus == FB_LOOPING_ERR){
		segTblInfoPtr->taskMode = PCS_STOP;
		systemRasInfoSet(RAS_MPU_EXEC_LOOP_STATUS, 1);	
	}

	
	
	
	status = segTblTickCountWrite (LOGIC_ID,taskId, segTblInfoPtr); 
	if(status != NO_ERROR) 
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

	segTblInfoPtr->taskElpTime = (int32)execTime ; 
 	status = segTblInfoWrite(LOGIC_ID, taskId, segTblInfoPtr);

	if(status != NO_ERROR) 
	{
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
	}

		
	gMpuTaskSchdCount[execInfoPtr->taskId - 1] =  (++gMpuTaskSchdCount[execInfoPtr->taskId - 1]) % 0x7fffff;

	
	if(gSystemInitFlag[execInfoPtr->taskId - 1] == SYS_INIT_FLAG_CLEAR) 
		gSystemInitFlag[execInfoPtr->taskId - 1] = SYS_INIT_FLAG_DONE;

SEMA_RELEASE:
	
#ifdef VXWORKS
	semaphoreRelease( gFbExecSeqIdQSemaId);  
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, gFbExecSeqIdQSemaId);  
#endif

MODULE_END:
	
	if(segTblInfoPtr->loopStatus == FB_LOOPING_ERR) {
		allSegmentModeStopSet(LOGIC_ID);
	}

    
    
	if(segTblInfoPtr->execMode == CYCLIC_RUNNING_MODE) {
    	execInfoPtr->segStatus 			= FB_RUNNING_DONE;
    	
    	execInfoPtr->mpuSegPhaseRunFlag = SCHEDULING_FLAG_DISABLE;
	}
	return(status);	    
}


uint32	sfcTrsFbcodeRunFunc(uint32 trsId,strMpuSegExecInfo	*segExecInfoPtr, uint32 *loopStatus)
{
	uint32 status = NO_ERROR;
	strFbExecutionInfo *fbExecIdInfoPtr;
	strFbMemInfo		*fbInfoPtr;
    strFbExecDbInfo		*fbCodeInfoPtr;
	uint32	blockAddr = 0;
	strTransitionInfo  *trsInfoPtr = NULL;
	uint32 taskId,trsResult;
	strFbSpecActionInfo *specActionDataPtr = NULL;
	uint32 i, startTimer, endTimer, execTime;

	uint32 jmpErr = 0;

	if(trsId<1 || trsId > MAX_TRANSITOIN_NUM)
	{
		status = L_TRANSITION_ID_ERR;
		goto MODULE_END;	
	}
    
	trsInfoPtr = gSFCTransitionLocalPtr;
	trsInfoPtr += (trsId-1);
	taskId = trsInfoPtr->taskID;
	specActionDataPtr =(strFbSpecActionInfo *)&trsInfoPtr->setActiveHeader;

	
#ifdef SYS_AUX_CLK_USED
	    startTimer = sysAuxTickCountGet();
#else
    	startTimer = systemTickGet();
#endif

	if( ( loopStatus != NULL )&&(trsInfoPtr->trsStatus == FB_LOOPING_ERR) ) {
		*loopStatus = FB_LOOPING_ERR ;
		goto MODULE_END;
	}
	
	fbExecIdInfoPtr = trsExecIdStartMemInfoGet(trsId);
	while(fbExecIdInfoPtr!=NULL)
	{
			
			if(fbExecIdInfoPtr->taskId != taskId ) {
				status = L_SEG_ID_ERR;	
				setErrorCodeWithVal (	__FILE__, __LINE__,
										"sfcTrsFbcodeRunFunc",status,
										"Read SegId", fbExecIdInfoPtr->taskId,
										"Cur Seg ID",taskId,
										"Block Addr", fbExecIdInfoPtr->fbBlockAddr
							  );
				goto MODULE_END;	
							  
			}

			if( (fbExecIdInfoPtr->fbBlockAddr < 1)	|| 
				(fbExecIdInfoPtr->fbBlockAddr > MAX_MPU_BLOCK_ADDR)
			){												
				status = L_FB_ADDR_ERR;	
				setErrorCodeWithVal (	__FILE__,__LINE__,
										"sfcTrsFbcodeRunFunc",status,
										"Read Block Addr", fbExecIdInfoPtr->fbBlockAddr,
										"Not Using",0,
										"Not Using",0
							  			);
				goto MODULE_END;						  
			}
			blockAddr = fbExecIdInfoPtr->fbBlockAddr;
			
			fbInfoPtr = gMpuFbMemInfoLocalPtr;
            fbInfoPtr += (blockAddr-1);

			
			
			if( (fbInfoPtr->fbId == 0)
			){ 
				status = L_FB_CODE_ERR;
				setErrorCodeWithVal (	__FILE__,__LINE__,
										"sfcTrsFbcodeRunFunc",status,
										"Read Block Addr", fbExecIdInfoPtr->fbBlockAddr,
										"Read Fb Code",fbExecIdInfoPtr->fbId,
										"Cur FB Code", fbInfoPtr->fbId
							  			);
				goto NEXT_FB_INFO_GET;
			}
			
			
			if(fbInfoPtr->forcingFlag== FORCING_ENABLE) goto NEXT_FB_INFO_GET;

			if(fbInfoPtr->TransitionID != trsId){
               	status = L_TRANSITION_ID_ERR;
				setErrorCodeWithVal (	__FILE__,__LINE__,
										"sfcTrsFbcodeRunFunc",status,
										"Block Addr", blockAddr,
										"Read Trs Id",fbInfoPtr->TransitionID,
										"Curr Trs Id", trsId
									  );
				goto NEXT_FB_INFO_GET;
			}

			fbCodeInfoPtr = &gFbCodeSortingTableInfo[fbInfoPtr->fbId];

			if(fbCodeInfoPtr->fbId != fbInfoPtr->fbId){
               	status = FC_CODE_EXIST_ERR;
				setErrorCodeWithVal (	__FILE__,__LINE__,
										"sfcTrsFbcodeRunFunc",status,
										"Block Addr", blockAddr,
										"Fb Code",fbInfoPtr->fbId,
										"Not Using", 0
									  );
				goto NEXT_FB_INFO_GET;
			}
			else { 
				status = fbCodeInfoPtr->fbActFunc(taskId, blockAddr);
				
                if(status != NO_ERROR) {
					
					fbInfoPtr->fbRunStatus = (status & 0xffff);	
					
					
					segExecInfoPtr->runErrCount++;

					
					if(++segExecInfoPtr->fbErrCount > MAX_FB_ERR_CODE_NUM) {
						segExecInfoPtr->fbErrCount = MAX_FB_ERR_CODE_NUM;
						
						for(i = 0; i < MAX_FB_ERR_CODE_NUM; i++) 
                       		memoryCopy ((uint8 *) &segExecInfoPtr->errInfoList[i],
                           				(uint8 *) &segExecInfoPtr->errInfoList[i+1],
										sizeof(strFbRunningErrInfo)
									   );
					}
					segExecInfoPtr->errInfoList[segExecInfoPtr->fbErrCount - 1].errorCode 
						= status;
					segExecInfoPtr->errInfoList[segExecInfoPtr->fbErrCount - 1].fbId 
						= fbInfoPtr->fbId;
					segExecInfoPtr->errInfoList[segExecInfoPtr->fbErrCount - 1].fbAddr 
						= blockAddr;

                }
                else  
                	fbInfoPtr->fbRunStatus = 0;
                	
			} 	
NEXT_FB_INFO_GET:	
		if(jmpFbCodeCheck(fbExecIdInfoPtr->fbId) == IS_JUMP_CODE){
			status = jumpCountVarCheck(fbExecIdInfoPtr->fbBlockAddr , 
									  fbExecIdInfoPtr->taskId,  &jmpErr);
			if (status != NO_ERROR)
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

			else if( jmpErr != NO_ERROR)  {
				
				trsInfoPtr->trsStatus = FB_LOOPING_ERR;
			}
			fbExecIdInfoPtr = jmpFbExecIdNextMemInfoGet(trsId, fbExecIdInfoPtr);
		}
		else fbExecIdInfoPtr = trsExecIdNextMemInfoGet(trsId, fbExecIdInfoPtr);
	}

#ifdef SYS_AUX_CLK_USED
	endTimer = sysAuxTickCountGet();
#else
    endTimer = systemTickGet();
#endif
    execTime = endTimer - startTimer;
	
	
	if(gMpuTaskSchdCount[segExecInfoPtr->taskId - 1] > 0)
		cyclicSegExecTimeCalcu((uint32)execTime, &gTrsExecTime[trsId-1]);

	
	if(trsInfoPtr->trsStatus != FB_LOOPING_ERR) {
		trsInfoPtr->trsStatus = NO_ERROR;
	}

MODULE_END:
	if(trsInfoPtr->trsStatus == NO_ERROR) {
		if(specActionDataPtr->refType==SPEC_TYPE_BYREF)
		{
			status =  refDataCopyFromMem(&trsResult, trsInfoPtr->setActiveData,
							*specActionDataPtr,SIZE32_TYPE,0);

			if( (status == NO_ERROR)&&(trsResult <= 1) )
				trsInfoPtr->activeStatus = trsResult;
			else {
				trsInfoPtr->activeStatus = 0;
				status = TRS_ACTIVESTATUS_ERR; 
				setErrorCodeWithVal (	__FILE__,__LINE__,
							"sfcTrsFbcodeRunFunc",status,
							"TRS Value", trsResult,
							"TRS ID",trsInfoPtr->transitionID,
							"TASK ID", trsInfoPtr->taskID
						  );
			}
		}
		else if(specActionDataPtr->refType==SPEC_TYPE_BYVAL)
		{
			if(trsInfoPtr->setActiveData <= 1)
				trsInfoPtr->activeStatus = trsInfoPtr->setActiveData;
			else {
				trsInfoPtr->activeStatus = 0;
				status = TRS_ACTIVESTATUS_ERR; 
				setErrorCodeWithVal (	__FILE__,__LINE__,
						"sfcTrsFbcodeRunFunc",status,
						"TRS Value", trsInfoPtr->setActiveData,
						"TRS ID",trsInfoPtr->transitionID,
						"TASK ID", trsInfoPtr->taskID
					  );
			}
		}
		else {
			trsInfoPtr->activeStatus = 0;
			status = TRS_ACTIVESTATUS_ERR; 
			setErrorCode (__FILE__,__LINE__, __FUNCTION__, status );
		}
	}
	else {
		trsInfoPtr->activeStatus = 0;
	}
	
	if(loopStatus!= NULL)
		*loopStatus = trsInfoPtr->trsStatus;

	return(status);
}


uint32	sfcActFbcodeRunFunc(uint32 actId, strMpuSegExecInfo	*segExecInfoPtr, uint32 * loopStatus)
{
	uint32 status = NO_ERROR;
	strFbExecutionInfo *fbExecIdInfoPtr;
	strFbMemInfo		*fbInfoPtr;
    strFbExecDbInfo		*fbCodeInfoPtr;
	uint32	blockAddr = 0;
	strActionInfo  *actInfoPtr;
	uint32 taskId, i;
	uint32 startTimer, endTimer, execTime;

	strActionCtrlInfo *actCtrlInfoPtr;
	uint32 jmpErr=0;

	if(actId<1 || actId > MAX_TRANSITOIN_NUM)
	{
		status = L_ACTION_ID_ERR;
		goto MODULE_END;	
	}
    
	actInfoPtr = gSFCActionInfoLocalPtr;
	actInfoPtr += (actId-1);
	taskId = actInfoPtr->taskID;

	
#ifdef SYS_AUX_CLK_USED
	    startTimer = sysAuxTickCountGet();
#else
    	startTimer = systemTickGet();
#endif
	actCtrlInfoPtr = (strActionCtrlInfo*) &actInfoPtr->actCtrlData;
	if( (loopStatus != NULL )&&(actCtrlInfoPtr != NULL) ) {
		if(actCtrlInfoPtr->actStatus == FB_LOOPING_ERR){
			*loopStatus = FB_LOOPING_ERR;
			goto MODULE_END;
		}
	}

	fbExecIdInfoPtr = actExecIdStartMemInfoGet(actId);
	while(fbExecIdInfoPtr!=NULL)
	{
			
			if(fbExecIdInfoPtr->taskId != taskId ) {
				status = L_SEG_ID_ERR;	
				setErrorCodeWithVal (	__FILE__, __LINE__,
										"sfcTrsFbcodeRunFunc",status,
										"Read SegId", fbExecIdInfoPtr->taskId,
										"Cur Seg ID",taskId,
										"Block Addr", fbExecIdInfoPtr->fbBlockAddr
							  );
				goto MODULE_END;	
							  
			}
			if( (fbExecIdInfoPtr->fbBlockAddr < 1)
									|| 
				(fbExecIdInfoPtr->fbBlockAddr > MAX_MPU_BLOCK_ADDR)
			){												
				status = L_FB_ADDR_ERR;	
				setErrorCodeWithVal (	__FILE__,__LINE__,
										"sfcActFbcodeRunFunc",status,
										"Read Block Addr", fbExecIdInfoPtr->fbBlockAddr,
										"Not Using",0,
										"Not Using",0
							  			);
				goto MODULE_END;						  
			}
			blockAddr = fbExecIdInfoPtr->fbBlockAddr;
			
			fbInfoPtr = gMpuFbMemInfoLocalPtr;
            fbInfoPtr += (blockAddr-1);

			
			
			if( (fbInfoPtr->fbId == 0)
			){ 
				status = L_FB_CODE_ERR;
				setErrorCodeWithVal (	__FILE__,__LINE__,
										"sfcSegCtrlRunFunc",status,
										"Read Block Addr", fbExecIdInfoPtr->fbBlockAddr,
										"Read Fb Code",fbExecIdInfoPtr->fbId,
										"Cur FB Code", fbInfoPtr->fbId
							  			);
				goto NEXT_FB_INFO_GET;
			}
			
			
			if(fbInfoPtr->forcingFlag== FORCING_ENABLE) goto NEXT_FB_INFO_GET;

			fbCodeInfoPtr = &gFbCodeSortingTableInfo[fbInfoPtr->fbId];

			if(fbCodeInfoPtr->fbId != fbInfoPtr->fbId){
               	status = FC_CODE_EXIST_ERR;
				setErrorCodeWithVal (	__FILE__,__LINE__,
										"sfcSegCtrlRunFunc",status,
										"Block Addr", blockAddr,
										"Fb Code",fbInfoPtr->fbId,
										"Not Using", 0
									  );
				goto NEXT_FB_INFO_GET;
			}
			else { 
				status = fbCodeInfoPtr->fbActFunc(taskId, blockAddr);
                if(status != NO_ERROR) {
					
					fbInfoPtr->fbRunStatus = (status & 0xffff);	
					
					
					segExecInfoPtr->runErrCount++;
					
					
					if(++segExecInfoPtr->fbErrCount > MAX_FB_ERR_CODE_NUM) {
						segExecInfoPtr->fbErrCount = MAX_FB_ERR_CODE_NUM;
						
						for(i = 0; i < MAX_FB_ERR_CODE_NUM; i++) 
                       		memoryCopy ((uint8 *) &segExecInfoPtr->errInfoList[i],
                           				(uint8 *) &segExecInfoPtr->errInfoList[i+1],
										sizeof(strFbRunningErrInfo)
									   );
					}
					segExecInfoPtr->errInfoList[segExecInfoPtr->fbErrCount - 1].errorCode 
						= status;
					segExecInfoPtr->errInfoList[segExecInfoPtr->fbErrCount - 1].fbId 
						= fbInfoPtr->fbId;
					segExecInfoPtr->errInfoList[segExecInfoPtr->fbErrCount - 1].fbAddr 
						= blockAddr;

                }
                else  
                	fbInfoPtr->fbRunStatus = 0;
               	
			} 	
NEXT_FB_INFO_GET:	
		if(jmpFbCodeCheck(fbExecIdInfoPtr->fbId) == IS_JUMP_CODE){
			status = jumpCountVarCheck(fbExecIdInfoPtr->fbBlockAddr , 
										fbExecIdInfoPtr->taskId, &jmpErr);
			if (status != NO_ERROR)
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

			else if( jmpErr != NO_ERROR)  {
				
				actCtrlInfoPtr->actStatus = FB_LOOPING_ERR;
			}
			fbExecIdInfoPtr = jmpFbExecIdNextMemInfoGet(actId, fbExecIdInfoPtr);
		}
		else fbExecIdInfoPtr =  actExecIdNextMemInfoGet(actId, fbExecIdInfoPtr);
	}


#ifdef SYS_AUX_CLK_USED
	endTimer = sysAuxTickCountGet();
#else
    endTimer = systemTickGet();
#endif
    execTime = endTimer - startTimer;
	
	if(gMpuTaskSchdCount[segExecInfoPtr->taskId - 1] > 0)
		cyclicSegExecTimeCalcu((uint32)execTime, &gActExecTime[actId-1]);

	
	if(actCtrlInfoPtr->actStatus != FB_LOOPING_ERR) {
		actCtrlInfoPtr->actStatus = NO_ERROR;
	}

	if(loopStatus!= NULL)
		*loopStatus = actCtrlInfoPtr->actStatus;

	
MODULE_END:
	return(status);
}



