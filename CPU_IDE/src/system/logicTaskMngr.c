
#include	<stdio.h>
#include	"usrTypes.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include	"segScheduler.h"
#include	"fbMem.h"
#include	"usrErrCode.h"


#include	"network.h"
#include	"usrQueData.h"
#include	"msgQueData.h"
#include	"osDepend.h"

#include	"debugInfo.h"
#include	"systemDiag.h"
#include	"sysConfig.h"

#include	"sysDual.h"
		
#include	"sfcMem.h"
#include	"usrDefBuf.h"
#include    "usrSema.h"


#include	"netProtocol.h"
#include	"protocolBuf.h"


strSystemSchedulerInfo	sSysSegSCHDInfo;
strMpuSegSchedulerInfo	*gMpuSegSchdInfoPtr = 
							&sSysSegSCHDInfo.mpuSchdInfo;
strDiuSegSchedulerInfo	*gDiuSegSchdInfoPtr = 
							&sSysSegSCHDInfo.diuSchdInfo;


static uint32	sXrTaskSchdCount = 0;


strSysTaskSchedDebugInfo	sSchdDebugInfo;
strFBTaskSchdDebugInfo		*sFbSchdDbgInfoPtr;	
strIOScanTaskSchdDebugInfo	*sIOSchdDbgInfoPtr;
strXRTaskSchdDebugInfo		*sXRSchdDbgInfoPtr;


static uint32 sMaxTotalSCHDTime = 0;


uint32	gOne100usecData = 0;


uint32	gSysAuxIntrCount = 0;

strSchdPriorityInfo  sExecTaskId[MAX_MPU_CYCLIC_SEG_ID];

uint32 SCHD_TIME_BASE 				= 0;
uint32 XRTASK_SCHD_TIME 			= 0;
uint32 CYCLIC_FB_EXEC_TIME_BASE 	= 0;
uint32 NON_CYCLIC_FB_EXEC_TIME_BASE = 0;

#ifdef _WIN32
uint32 gWdgIsrShcdCount = 0;
#endif


uint32	gMpuOverTimeCheckFlag = TASK_OVERTIME_CHK_DISABLE;


extern int32	gCyclicFbTaskSemaId; 	
extern int32	gNonCyclicFbTaskSemaId;	

extern uint32	gStartTimer;
extern uint32   gEndTimer;

extern strSysRunningConfigInfo		*gSysRunningInfoPtr;
extern strSystemRunningStatusInfo	gSysRunningStatusInfo;

extern int32	gDebugPortFd;
extern uint32 gOutputScanFirstDelayDone;

#ifdef SYS_AUX_CLK_USED
void	one100MicroSecondISR(void);
void	systemUsrDefinedAuxClockSet(void);
#endif

void	segSchedulerISR(void);
uint32	segSchedulerInfoSet(uint32, uint32 );
uint32	segSchedulerInfoDelete(uint32, uint32, uint32);
void	schedulerDebugVarInit(void);
void	schedulerExecInfoDisplay(uint32, uint32);
void	schedulerStartFlagSet(uint32, uint32 , uint32 );
uint32	schedulerModeChange(uint32, uint32);
void	minumumCycleTimeRead(float *, uint32);
uint32	getTotalTaskNumber(uint32,uint32, uint32*);
uint32 	mpusegSchedulerStartAddrSet(uint32 taskId);

static void		mpuSegScheduleFunc(strMpuSegSchedulerInfo 	*);
static uint32 	mpuSegSchedulerInfoSet(uint32, strMpuSegSchedulerInfo *);
static uint32	mpuSegSchedulerInfoDelete(uint32, strMpuSegSchedulerInfo *);
static uint32	cyclicAndNonCyclicSegScheduleFunc(strMpuSegSchedulerInfo 	*);
static uint32	nonCyclicSegScheduleFunc (strMpuSegSchedulerInfo	*);
static uint32	nonCyclicFbSegScheduleSyncFunc(strMpuSegExecInfo	*);


extern void		memoryClear(uint8 *, uint32);
extern void		memoryCopy(uint8 *, uint8 *, uint32);
extern void		errorCodeSetInISR(uint8 *, uint32, uint8 *, uint32);
#ifdef _WIN32
extern int32	semaphoreRelease(int32, int32);
extern int32	semaphoreRelease2(uint8 *, uint32, int32, int32);
#else
extern int32	semaphoreRelease(int32);
#endif
extern uint32	getFbTaskTblInfo(uint32, uint32, strFbTaskTblMemInfo *);
extern uint32	readRuntimeFbData(uint32,uint32, uint32,uint32 *);
extern void		setErrorCodeWithVal(uint8 *, uint32,uint8 *, uint32,
					uint8 *, uint32 ,uint8 *,uint32,uint8 *,uint32);
extern uint32	fbTaskVarInit(void);

extern int32	keyHit(void);
extern void		errorStringGet(uint32, uint8 *);

extern void		debugCountSet(uint32 *);
extern void		debugValSet(uint32 *, uint32 );
extern void		systemClockRateSet(int32 );
extern void		moduleLiveCountAndFbcStatusCheck(void);
extern void		systemRasInfoSet(uint32,uint32);
extern uint32	fbCodeMemInfoRead(uint32, uint32, strFbMemInfo *);
extern void		setErrorCode(uint8 *,uint32,uint8 *,uint32);
extern uint32	systemCnfgInfoRead( strSysConfigInfo *sysCnfgInfo);
extern uint32	systemCnfgInfoRead( strSysConfigInfo *);

extern void		systemClockIntrSet(func_ptr , int32 );
extern void		systemClockEnable(void);
extern void		systemClockDisable(void);
extern int32	systemClockRateGet(void);

extern void		systemAuxClockIntrSet(func_ptr , int32 );
extern void		systemAuxClockEnable(void);
extern void		systemAuxClockDisable(void);
extern void		systemAuxClockRateSet(int32 );
extern int32	systemAuxClockRateGet(void);
extern uint32	onlineDwlAccessFlagCheck(uint32);

#ifdef _WIN32
extern void		wdgCyclicMngrISR(int32);
#endif
extern uint32	moduleInfoRead(uint32, strModuleInfo	*);
extern void		mySwap(void *, void *, uint32);
extern uint32	fbSysExeModuleInfoRead( uint32, strModuleExecInfo* );
extern strFbExecutionInfo	*fbExecIdStartMemInfoGet(uint32 );


#ifdef _WIN32
void CALLBACK clockIntr(UINT, UINT, DWORD, DWORD, DWORD);
void CALLBACK wdtIntr(UINT, UINT, DWORD, DWORD, DWORD);
extern void rasTaskExecCheck (void);
#endif
extern uint32 systemClockTimeSet();


uint32 segSchedulerInfoInit(uint32 clearFlag)
{
	uint32 status = NO_ERROR;
	uint32	taskId;
	
	if(clearFlag == MPU_DIU_DUAL_ACC){
		memoryClear ( 	(uint8 *)&sSysSegSCHDInfo,
    					sizeof(strSystemSchedulerInfo)
					);
	}
	else if(clearFlag == MPU_ONLY_DUAL_ACC) {
		memoryClear (	(uint8 *)&sSysSegSCHDInfo.mpuSchdInfo,
						sizeof(strMpuSegSchedulerInfo)
					);
	}
    else {
    	status = DUAL_STATUS_FLAG_ERR;
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
    }

	if((clearFlag == MPU_DIU_DUAL_ACC) || (clearFlag == MPU_ONLY_DUAL_ACC)) {

		
		for(taskId = 1; taskId <= MAX_MPU_CYCLIC_SEG_ID; taskId++) { 
			memoryClear( (uint8*)&sExecTaskId[taskId-1], sizeof(strSchdPriorityInfo));
		}

		
		for(taskId = 1; taskId <= MAX_MPU_SCHD_SEG_ID; taskId++) { 
			status = segSchedulerInfoSet(LOGIC_ID, taskId);

			if(status != NO_ERROR) {
        		printf( "[ERROR]-SEG_ID[%d]\n", taskId);

			}
		}
		if(clearFlag == MPU_DIU_DUAL_ACC)	
   			printf( "MPU Seg Scheuler Information Set SUCCESS !!!\n");
	}

	return (status);
}


#ifdef SYS_AUX_CLK_USED
void one100MicroSecondISR(void)
{
	gSysAuxIntrCount++;
	gOne100usecData++;
	gOne100usecData %= BOARD_AUX_CLK_RATE; 
	
#if (BOARD_TYPE == LKV120_BOARD)
	#if 1
	if(gOne100usecData%2 == 0)
	    ledOff(1);  /* S2_LED_BLUE_BIT */
    else 
        ledOn(1);  /* S2_LED_BLUE_BIT */
    #endif
#endif
    
	return;
}		
#endif


void segSchedulerISR(void)
{
	strSysRunningConfigInfo *sysStatusInfoPtr = gSysRunningInfoPtr;
	strSysConfigInfo sysCnfgInfo;
	
	moduleLiveCountAndFbcStatusCheck();
	
#ifdef VXWORKS
	if(	(sysStatusInfoPtr->sysRedundancyStatus 	== SYS_RAS_REDUNDANCY_RUN) && 
		(sysStatusInfoPtr->sysRunningMode 		== SYS_RAS_SYSTEM_RUN))
	{
		mpuSegScheduleFunc(&sSysSegSCHDInfo.mpuSchdInfo);
    }
    else if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_STANDBY)
    {
		
    	systemRasInfoSet(RAS_MPU_EXEC_OVERTIME_STATUS, 0);
        systemRasInfoSet(RAS_DIU_EXEC_OVERTIME_STATUS, 0);

        
		systemRasInfoSet(RAS_MPU_EXEC_LOOP_STATUS, 0);
	}
#else
	if(	(sysStatusInfoPtr->sysRedundancyStatus 	== SYS_RAS_REDUNDANCY_RUN) && 
		(sysStatusInfoPtr->sysRunningMode 		== SYS_RAS_SYSTEM_RUN)){
		mpuSegScheduleFunc(&sSysSegSCHDInfo.mpuSchdInfo);
	}
    else if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_STANDBY){
		
    	systemRasInfoSet(RAS_MPU_EXEC_OVERTIME_STATUS, 0);
        systemRasInfoSet(RAS_DIU_EXEC_OVERTIME_STATUS, 0);
	    
		systemRasInfoSet(RAS_MPU_EXEC_LOOP_STATUS, 0);

	}

#endif	
	systemCnfgInfoRead(&sysCnfgInfo);

#ifdef _WIN32
	if(++gWdgIsrShcdCount >= (RAS_TASK_SCHD_TIME/SCHD_TIME_BASE_ONEMS)) {
		wdgCyclicMngrISR(0);
		gWdgIsrShcdCount = 0;
	}

	
	
	rasTaskExecCheck ();
#endif
    return;
}



static void mpuSegScheduleFunc
			( 
		strMpuSegSchedulerInfo 	*schdInfoPtr
        	)
{
	uint32	i;
	
	strMpuSegExecInfo		*cyclicExecInfoPtr, *nonCyclicExecInfoPtr;

  	nonCyclicExecInfoPtr 	= (strMpuSegExecInfo *)&schdInfoPtr->nonCyclicSeg;

	
    if(	schdInfoPtr->schedulerMode == SCHEDULER_ENABLE)	
    {
    	
        if(	((schdInfoPtr->cyclicSegNum + schdInfoPtr->eventTaskNum) > MAX_MPU_CYCLIC_SEG_ID) ||
			(schdInfoPtr->nonCyclicSegNum 	> MAX_MPU_NONCYCLIC_SEG_ID))
		{   
	     	errorCodeSetInISR (	__FILE__,
            					__LINE__,
								(uint8 *)"mpuSegScheduleFunc",
								SEG_GROUP_NUM_EXCEED_ERR
								);
        }
        
        else if( (schdInfoPtr->cyclicSegNum != 0) || (schdInfoPtr->eventTaskNum != 0) )
        {
			cyclicAndNonCyclicSegScheduleFunc(schdInfoPtr);
        }
          
		else if(schdInfoPtr->nonCyclicSegNum != 0)
		{
			nonCyclicSegScheduleFunc (schdInfoPtr);
		}
        
		else 
		{
			gOutputScanFirstDelayDone = 1;
			nonCyclicExecInfoPtr->schedulingCount = 0;
			
            for(i = 0; i <= MAX_MPU_CYCLIC_SEG_ID; i++) 
            {
				cyclicExecInfoPtr = &schdInfoPtr->cyclicSeg[i];
				cyclicExecInfoPtr->schedulingCount = 0;				 
            }

            
			gStartTimer = 0;
       		gEndTimer	= 0;

			
            sMaxTotalSCHDTime = 0;

            
            systemRasInfoSet(RAS_MPU_EXEC_OVERTIME_STATUS, 0);
			
			
			systemRasInfoSet(RAS_MPU_EXEC_LOOP_STATUS, 0);
        }
	}
	return;
}


static uint32 nonCyclicSegScheduleFunc(strMpuSegSchedulerInfo 	*schdInfoPtr)
{
	uint32 status = NO_ERROR;
	uint32 retVal = NO_ERROR;
	uint32	totalSCHDTime = 0;
	strMpuSegExecInfo	*nonCyclicExecInfoPtr;

  	nonCyclicExecInfoPtr 	= (strMpuSegExecInfo *)&schdInfoPtr->nonCyclicSeg;

    
    
	if (nonCyclicExecInfoPtr->schedulingEnableFlag == SCHEDULING_FLAG_ENABLE){
		
				
		totalSCHDTime = nonCyclicExecInfoPtr->schedulingCount * SCHD_TIME_BASE_ONE100US;

		if(totalSCHDTime >= (schdInfoPtr->nonCyclicSeg.cyclicAndNonCyclicTimeId * NON_CYCLIC_FB_EXEC_TIME_BASE)) {
	        if(nonCyclicExecInfoPtr->segStatus == FB_RUNNING_DONE){ 

       	     	
        	    debugCountSet((uint32 *) &sFbSchdDbgInfoPtr->nonCyclicSchdCount);

				nonCyclicExecInfoPtr->schedulingMode = SCHEDULING_RUNNING;

                
				nonCyclicExecInfoPtr->segStatus = FB_RUNNING_READY;
				
				
				 nonCyclicExecInfoPtr->mpuSegPhaseRunFlag = SCHEDULING_FLAG_ENABLE;
				
			   	
#ifdef _WIN32
				retVal = semaphoreRelease2(__FILE__,__LINE__,BINARY_SEMA, gNonCyclicFbTaskSemaId);
				if(retVal)
				   sysMsgLogSet(__FILE__,__LINE__,"nonCyclicSegScheduleFunc",SYSTEM_ISR_ERR,
							  "SemaRelErr",retVal);
#else
				semaphoreRelease(gNonCyclicFbTaskSemaId);  
#endif
			}
			
            
           	nonCyclicExecInfoPtr->schedulingCount = 1;
		}
        else {
           	
            
			if( nonCyclicExecInfoPtr->schedulingCount == 0) {
				nonCyclicExecInfoPtr->schedulingMode = SCHEDULING_RUNNING;

	            
				nonCyclicExecInfoPtr->segStatus = FB_RUNNING_READY;

				
				 nonCyclicExecInfoPtr->mpuSegPhaseRunFlag = SCHEDULING_FLAG_ENABLE;

#ifdef _WIN32
				retVal = semaphoreRelease2(__FILE__,__LINE__,BINARY_SEMA, gNonCyclicFbTaskSemaId);
				if(retVal)
				   sysMsgLogSet(__FILE__,__LINE__,"nonCyclicSegScheduleFunc",SYSTEM_ISR_ERR,
							  "SemaRelErr",retVal);
#else
				semaphoreRelease(gNonCyclicFbTaskSemaId);  
#endif
			}

           	nonCyclicExecInfoPtr->schedulingCount += 1;
		}
    }
    return(status);
}


static uint32 cyclicAndNonCyclicSegScheduleFunc (strMpuSegSchedulerInfo 	*schdInfoPtr)
{
	uint32 status = NO_ERROR, segIndex,semaReleaseFlag = 0;
#ifdef _WIN32
	uint32 retVal = NO_ERROR;
#endif
    uint32 	nonCyclicRunningFlag = SCHEDULING_FLAG_ENABLE;
    uint32	cycleExecTime;
	strMpuSegExecInfo		*cyclicExecInfoPtr, *nonCyclicExecInfoPtr;
	strSystemRunningStatusInfo	*sysInfoPtr 
							= (strSystemRunningStatusInfo *)&gSysRunningStatusInfo; 

 	nonCyclicExecInfoPtr 	= (strMpuSegExecInfo *)&schdInfoPtr->nonCyclicSeg;

	
	cycleExecTime = (schdInfoPtr->schedulingCount+1) * SCHD_TIME_BASE_ONE100US;

    
    debugValSet((uint32 *) &sFbSchdDbgInfoPtr->cyclicElapsedTime,
        		cycleExecTime
			   );

	if(cycleExecTime >= CYCLIC_FB_EXEC_TIME_BASE)
	{	
		schdInfoPtr->schedulingCount = 0;
		
		semaReleaseFlag = 0;
		
    	for(segIndex = 0; segIndex <= MAX_MPU_CYCLIC_SEG_ID; segIndex++) 
    	{

	  		cyclicExecInfoPtr 	= (strMpuSegExecInfo *)&schdInfoPtr->cyclicSeg[segIndex];
	  		
			if(	(cyclicExecInfoPtr->schedulingEnableFlag == SCHEDULING_FLAG_ENABLE) &&
				(cyclicExecInfoPtr->taskId != 0x0) 
			){
				if(cyclicExecInfoPtr->phaseSchedulingCount == cyclicExecInfoPtr->cyclicAndNonCyclicTimeId) 
				{
	   				
					debugCountSet((uint32 *) &sFbSchdDbgInfoPtr->cyclicSchdCount);
						
					cyclicExecInfoPtr->mpuSegPhaseRunFlag 	= SCHEDULING_FLAG_ENABLE;	
					cyclicExecInfoPtr->phaseSchedulingCount = 1;
											 
					if(semaReleaseFlag == 0x0){
#ifdef _WIN32
						if((retVal = semaphoreRelease2(__FILE__,__LINE__,BINARY_SEMA, gCyclicFbTaskSemaId))!=NO_ERROR)
#else
						if(semaphoreRelease(gCyclicFbTaskSemaId)!= NO_ERROR)
#endif
						{
							schdInfoPtr->cyclicsemaReleaseErrCnt++;
#ifdef _WIN32
							sysMsgLogSet(__FILE__,__LINE__,"cyclicAndNonCyclicSegScheduleFunc",SYSTEM_ISR_ERR,
									  "SemaRelErr",retVal);
#endif
						}
						semaReleaseFlag++;
					} 

					
					if(cyclicExecInfoPtr->segSchedulingStartFlag == SEG_SCHEDULING_FIRST) 
					{
						cyclicExecInfoPtr->segSchedulingStartFlag = SEG_SCHEDULING_DOING;
						continue;						
					}
									
					if(	(cyclicExecInfoPtr->segStatus != FB_RUNNING_DONE) 
										&&
						(sysInfoPtr->sysRunningConfigInfo.sysRedundancyStatus != SYS_RAS_REDUNDANCY_STANDBY)
										&&
						(gMpuOverTimeCheckFlag == TASK_OVERTIME_CHK_DISABLE)
					){
 	     				errorCodeSetInISR (	__FILE__,
               								__LINE__,
											(uint8 *)"cyclicAndNonCyclicSegScheduleFunc",
											FB_RUNNING_OVERTIME_ERR
										);

						schdInfoPtr->overTimeFlag = SEG_EXEC_OVERTIME;

		   				
						debugCountSet((uint32 *) &sFbSchdDbgInfoPtr->cyclicOvertimeCount);
            			
            			if(onlineDwlAccessFlagCheck(LOGIC_ID) == NO_ERROR)
            				systemRasInfoSet(RAS_MPU_EXEC_OVERTIME_STATUS, 1);

          			} 
					else 
					{
           				schdInfoPtr->overTimeFlag = SEG_EXEC_INTIME;

            			
            			systemRasInfoSet(RAS_MPU_EXEC_OVERTIME_STATUS, 0);
					} 
					
					
					cyclicExecInfoPtr->segStatus		= FB_RUNNING_READY;
                	cyclicExecInfoPtr->schedulingMode 	= SCHEDULING_RUNNING;

                	nonCyclicExecInfoPtr->schedulingMode = SCHEDULING_WAITTING;

				} 
				else 
				{
					cyclicExecInfoPtr->phaseSchedulingCount++;
				}
			}
		} 
	} 
	else 
	{ 
		schdInfoPtr->schedulingCount++;	
		
       
		if(schdInfoPtr->nonCyclicSegNum != 0x0) 
		{
 			
    		for(segIndex = 0; segIndex <= MAX_MPU_CYCLIC_SEG_ID; segIndex++) 
    		{
	  			cyclicExecInfoPtr 	= (strMpuSegExecInfo *)&schdInfoPtr->cyclicSeg[segIndex];
    			if(	(cyclicExecInfoPtr->schedulingEnableFlag == SCHEDULING_FLAG_ENABLE) &&
					(cyclicExecInfoPtr->taskId != 0x0) 
				){
					
					
					if( cyclicExecInfoPtr->mpuSegPhaseRunFlag == SCHEDULING_FLAG_ENABLE){
						
						nonCyclicRunningFlag = SCHEDULING_FLAG_DISABLE;	
						break;
					}
				} 			
 			} 
 			
 			if(nonCyclicRunningFlag == SCHEDULING_FLAG_ENABLE) 
 			{
                if(nonCyclicFbSegScheduleSyncFunc(nonCyclicExecInfoPtr) 
					== SCHEDULING_RUNNING
				){
            		
                	debugCountSet((uint32 *) &sFbSchdDbgInfoPtr->nonCyclicSchdCount);

   	            	nonCyclicExecInfoPtr->schedulingMode 		= SCHEDULING_RUNNING;
					nonCyclicExecInfoPtr->mpuSegPhaseRunFlag	= SCHEDULING_FLAG_ENABLE;
   		            
   		            
       		        nonCyclicExecInfoPtr->segStatus = FB_RUNNING_READY;
#ifdef _WIN32
					retVal = semaphoreRelease2(__FILE__,__LINE__,BINARY_SEMA, gNonCyclicFbTaskSemaId);  
					if(retVal)
					   sysMsgLogSet(__FILE__,__LINE__,"cyclicAndNonCyclicSegScheduleFunc",SYSTEM_ISR_ERR,
								  "SemaRelErr",retVal);
#else
					semaphoreRelease(gNonCyclicFbTaskSemaId);  
#endif
 				}
                else 
                {
    	            nonCyclicExecInfoPtr->schedulingMode = SCHEDULING_WAITTING;
	
   		            
       		        nonCyclicExecInfoPtr->segStatus = FB_RUNNING_READY;
 					nonCyclicExecInfoPtr->mpuSegPhaseRunFlag	= SCHEDULING_FLAG_DISABLE;
               }
 			} 
 		} 
	}
	
	return(status);
} 



static uint32 nonCyclicFbSegScheduleSyncFunc(strMpuSegExecInfo	*nonCyclicExecInfoPtr
											)
{
	uint32	retVal = 0;
	
    if(!(nonCyclicExecInfoPtr->nonCyclicSyncCount%2)) {
		retVal = SCHEDULING_RUNNING;
    }
    else {
    	retVal = SCHEDULING_WAITTING;
    	nonCyclicExecInfoPtr->nonCyclicSyncCount++;
    	if(nonCyclicExecInfoPtr->nonCyclicSyncCount > 0x7fffff)
    		nonCyclicExecInfoPtr->nonCyclicSyncCount = 0;	
	}

    return(retVal);
}

uint32	segSchedulerInfoSet(uint32 moduleId, uint32 taskId)
{
	uint32	status = NO_ERROR;
	
    if(moduleId == LOGIC_ID)
    	status = mpuSegSchedulerInfoSet(taskId, &sSysSegSCHDInfo.mpuSchdInfo);
	else
    	status = L_MODULE_ID_ERR;

	return(status);
}


uint32  mpuTaskExecInfoQuicksort(	strSchdPriorityInfo taskPriorityInfo[], 
									int32 left, 
									int32 right
								)
{
	uint32 status = NO_ERROR;
	int32  i, j;
	uint32 priorityPivot;

	if ( (left > MAX_MPU_CYCLIC_SEG_ID) || (right > MAX_MPU_CYCLIC_SEG_ID) ) {
		status = L_SEG_ID_ERR;
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
		return(status);
	}

	if(left < right)
	{
		i = left; j = right +1;
		priorityPivot = taskPriorityInfo[left].priority;
		if( (taskPriorityInfo[left].priority < 0) || 
			(taskPriorityInfo[left].priority > MAX_SEG_PRIORITY) )
		{
			status = SEG_PRIORITY_ERR;
			setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
			return(status);
		}
		
		do{
			
			do {
				i++;
				if( (taskPriorityInfo[i].priority < 0) || 
					(taskPriorityInfo[i].priority > MAX_SEG_PRIORITY) )
				{
					status = SEG_PRIORITY_ERR;
					setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
					return(status);
				}
				if(i>=right) break;
			} while(taskPriorityInfo[i].priority < priorityPivot);

			do {
				j--;
				if( (taskPriorityInfo[i].priority < 0) || 
					(taskPriorityInfo[i].priority > MAX_SEG_PRIORITY) )
				{
					status = SEG_PRIORITY_ERR;
					setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
					return(status);
				}
				if(j<=left) break;
			} while(taskPriorityInfo[j].priority > priorityPivot);
			
			
			if(i<j)
				mySwap(&taskPriorityInfo[i],&taskPriorityInfo[j],sizeof(strSchdPriorityInfo));
		}while(i<j);

		mySwap(&taskPriorityInfo[left],&taskPriorityInfo[j],sizeof(strSchdPriorityInfo));
		
		status = mpuTaskExecInfoQuicksort(taskPriorityInfo,left, j-1);
		if(status == NO_ERROR)
			status = mpuTaskExecInfoQuicksort(taskPriorityInfo,j+1, right);
	}
	return(status);
}


static uint32 	mpuSegSchedulerInfoSet (uint32	taskId,strMpuSegSchedulerInfo	*segSCHDInfoPtr)
{
	uint32	status = NO_ERROR;
    uint32	cycleTimeId = 0, cycleEventtaskNum;
	strMpuSegExecInfo		*segExecInfoPtr;
	strFbTaskTblMemInfo	segTblInfo;
	strFbExecutionInfo  *fbExecIdInfoPtr;

    
    if( (taskId <= 0) || (taskId > MAX_MPU_SCHD_SEG_ID)) {
    	status = L_SEG_ID_ERR;
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
	}
	else {
    	status = getFbTaskTblInfo	( 
									LOGIC_ID,
            						taskId,
									(strFbTaskTblMemInfo	*) &segTblInfo
								);
		if(status != NO_ERROR){
			setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
			goto MODULE_END;
		}
		
		if((segTblInfo.taskId == 0) 
			|| (segTblInfo.taskId > MAX_LOGIC_TASK_NUM)
			|| (segTblInfo.taskId != taskId) 
		)
			goto MODULE_END;


		 
		cycleEventtaskNum = segSCHDInfoPtr->cyclicSegNum + segSCHDInfoPtr->eventTaskNum;

		if(segTblInfo.execMode == CYCLIC_RUNNING_MODE) { 
			cycleTimeId = segTblInfo.taskCycleTime;

        	if(	(cycleTimeId < MIN_CYCLIC_TIME_ID ) || 
				(cycleTimeId > MAX_CYCLIC_TIME_ID)
			) {
				status =  CYCLIC_TIME_ID_ERR;
				setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
				goto MODULE_END;
			}

    	    segSCHDInfoPtr->cyclicSeg[taskId - 1].cyclicAndNonCyclicTimeId = cycleTimeId;

            
            
            if(segSCHDInfoPtr->cyclicSegNum >= MAX_MPU_CYCLIC_SEG_ID) {
				status = L_SEG_ID_ERR;
				setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
				goto MODULE_END;
            }
			segSCHDInfoPtr->cyclicSegNum += 1;
            
	       	
			segExecInfoPtr = (strMpuSegExecInfo *)&segSCHDInfoPtr->cyclicSeg[taskId - 1]; 

    	    segExecInfoPtr->taskId 				= taskId;
    	    segExecInfoPtr->schedulingMode		= SCHEDULING_RUNNING;
    	    segExecInfoPtr->segStatus			= FB_RUNNING_READY;
    	    segExecInfoPtr->segExecMode			= CYCLIC_RUNNING_MODE;
            segExecInfoPtr->nonCyclicSyncCount	= 0;
			
			

			segExecInfoPtr->startFbBlockAddr	= 0;
			segExecInfoPtr->nextExecFbBlockAddr	= 0;
            
            
            segExecInfoPtr->phaseSchedulingCount	= 1;
           	segExecInfoPtr->segSchedulingStartFlag 	= SEG_SCHEDULING_FIRST;
            segExecInfoPtr->mpuSegPhaseRunFlag		= SCHEDULING_FLAG_DISABLE;
			segExecInfoPtr->logicType = segTblInfo.logicType;
		}
		else if(segTblInfo.execMode == NON_CYCLIC_RUNNING_MODE) { 
			cycleTimeId = segTblInfo.taskCycleTime;

            
            
            if(segSCHDInfoPtr->nonCyclicSegNum != 0) {
				status = L_SEG_ID_ERR;
				setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
				goto MODULE_END;
            }
			segSCHDInfoPtr->nonCyclicSegNum = 1;

    	    segSCHDInfoPtr->nonCyclicSeg.cyclicAndNonCyclicTimeId	= 1;

	       	
			segExecInfoPtr = (strMpuSegExecInfo *)
					&segSCHDInfoPtr->nonCyclicSeg; 

    	    segExecInfoPtr->taskId 				= taskId;
    	    segExecInfoPtr->segExecMode			= NON_CYCLIC_RUNNING_MODE;
    	    segExecInfoPtr->segStatus			= FB_RUNNING_READY;
	        segExecInfoPtr->nonCyclicSyncCount	= 0;

			
			fbExecIdInfoPtr = fbExecIdStartMemInfoGet(taskId);
			if(fbExecIdInfoPtr != NULL ) {
				segExecInfoPtr->startFbBlockAddr	= fbExecIdInfoPtr->fbBlockAddr;
				segExecInfoPtr->nextExecFbBlockAddr	= fbExecIdInfoPtr->fbBlockAddr;
 			}
			else{
				segExecInfoPtr->startFbBlockAddr = 0 ;
				segExecInfoPtr->nextExecFbBlockAddr = 0;
			}
			segExecInfoPtr->logicType = segTblInfo.logicType;
		}
		else if(segTblInfo.execMode == 	EVENT_RUNNING_MODE) { 
			cycleTimeId = segTblInfo.taskCycleTime;

            
            if(segSCHDInfoPtr->eventTaskNum >= MAX_MPU_CYCLIC_SEG_ID) {
				status = L_SEG_ID_ERR;
				setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
				goto MODULE_END;
            }
			segSCHDInfoPtr->eventTaskNum += 1;
            
	       	
			segExecInfoPtr = (strMpuSegExecInfo *)&segSCHDInfoPtr->cyclicSeg[taskId-1]; 

    	    segExecInfoPtr->taskId 				= taskId;
    	    segExecInfoPtr->schedulingMode		= SCHEDULING_RUNNING;
    	    segExecInfoPtr->segStatus			= FB_RUNNING_READY;
    	    segExecInfoPtr->segExecMode			= EVENT_RUNNING_MODE;
            segExecInfoPtr->nonCyclicSyncCount	= 0;
			
			
			segExecInfoPtr->startFbBlockAddr	= 0;
			segExecInfoPtr->nextExecFbBlockAddr	= 0;
            
            
            segExecInfoPtr->phaseSchedulingCount	= 1;
           	segExecInfoPtr->segSchedulingStartFlag 	= SEG_SCHEDULING_FIRST;
            segExecInfoPtr->mpuSegPhaseRunFlag		= SCHEDULING_FLAG_DISABLE;

			
			segExecInfoPtr->eventTaskRunFlag = EVENT_FLAG_DISABLE;
			segExecInfoPtr->eventPreVar = 0;
			segExecInfoPtr->eventVarType = (segTblInfo.u2.evtSetFbEndAddr & 0x1);

			
			if(segExecInfoPtr->eventVarType == EVENT_LEVEL_TYPE)
				segExecInfoPtr->cyclicAndNonCyclicTimeId = cycleTimeId; 
			else segExecInfoPtr->cyclicAndNonCyclicTimeId = 1;

			segExecInfoPtr->logicType = segTblInfo.logicType;
		}
        else {
        	status = SEG_EXEC_MODE_ERR; 
        	setErrorCodeWithVal(	__FILE__, __LINE__,__FUNCTION__, status,
                                    "Req seg Id", taskId,
                                    "Seg Exec Mode", segTblInfo.execMode,
                                    "Block Addr", 0
								);
		}

		
		if( (segTblInfo.execMode == EVENT_RUNNING_MODE) ||
			(segTblInfo.execMode == CYCLIC_RUNNING_MODE) ) 
		{ 
			sExecTaskId[cycleEventtaskNum].priority = segTblInfo.taskPriority;
			sExecTaskId[cycleEventtaskNum].taskId 	= taskId;
			mpuTaskExecInfoQuicksort(sExecTaskId,0,cycleEventtaskNum);
		}
	} 
MODULE_END:
  	return(status);
}

uint32 segSchedulerInfoDelete(uint32 moduleId, uint32 taskId, uint32 dwlKind)
{
	uint32	status = NO_ERROR;

	if(moduleId == LOGIC_ID) 
		status = mpuSegSchedulerInfoDelete(taskId,&sSysSegSCHDInfo.mpuSchdInfo);
	else 
    	status = L_MODULE_ID_ERR;

	return(status);
}
	

static uint32	mpuSegSchedulerInfoDelete
					(
			uint32	taskId,
			strMpuSegSchedulerInfo	*segSchdInfoPtr
					)
{
	uint32	status = NO_ERROR, segIndex;
    strMpuSegExecInfo		*segExecInfoPtr;

    if(taskId == 0)
    { 
    	status = L_SEG_ID_ERR;
        goto MODULE_END;
	}

	if( taskId > MAX_MPU_SCHD_SEG_ID)
	{
		
        goto MODULE_END;
    }
    
    if( (segSchdInfoPtr->cyclicSegNum 		== 0) &&
    	(segSchdInfoPtr->nonCyclicSegNum	== 0) &&
		(segSchdInfoPtr->eventTaskNum 		== 0)
	)
    	goto MODULE_END;


	
    if(segSchdInfoPtr->nonCyclicSegNum	!= 0) 
    {
		segExecInfoPtr = &segSchdInfoPtr->nonCyclicSeg;

        if(segExecInfoPtr->taskId == taskId) 
        {
			segSchdInfoPtr->nonCyclicSegNum = 0;
            memoryClear	( 	(uint8 *)&segSchdInfoPtr->nonCyclicSeg,
							sizeof(strMpuSegExecInfo)
						);
			goto MODULE_END; 
        }
    }

	
    if(segSchdInfoPtr->cyclicSegNum	!= 0) 
    {
    	for(segIndex = 0; segIndex <= MAX_MPU_CYCLIC_SEG_ID ; segIndex++) 
    	{
			segExecInfoPtr = &segSchdInfoPtr->cyclicSeg[segIndex];

	        if( (segExecInfoPtr->taskId == taskId) &&
			    (segExecInfoPtr->segExecMode == CYCLIC_RUNNING_MODE) ) 
			{
				segSchdInfoPtr->cyclicSegNum -= 1;
        	    memoryClear	( 	(uint8 *)segExecInfoPtr,
								sizeof(strMpuSegExecInfo)
							);
				goto MODULE_END;
			} 
        }

		
		for(segIndex = 0; segIndex < MAX_MPU_CYCLIC_SEG_ID; segIndex++) 
		{
			if(sExecTaskId[segIndex].taskId == taskId) 
			{
				memoryClear( (uint8*)&sExecTaskId[segIndex], sizeof(strSchdPriorityInfo));
				break;
			}
		}
    }
	
    if(segSchdInfoPtr->eventTaskNum	!= 0) 
    {
    	for(segIndex = 0; segIndex <= MAX_MPU_CYCLIC_SEG_ID; segIndex++) 
    	{
			segExecInfoPtr = &segSchdInfoPtr->cyclicSeg[segIndex];
	        if( (segExecInfoPtr->taskId == taskId) && 
				(segExecInfoPtr->segExecMode == EVENT_RUNNING_MODE) ) 
			{
				segSchdInfoPtr->eventTaskNum -= 1;
        	    memoryClear	( 	(uint8 *)segExecInfoPtr,
								sizeof(strMpuSegExecInfo)
							);
				goto MODULE_END;
			} 
        }
    }
    
MODULE_END:
    return(status);
}

uint32	schedulerModeChange(uint32 moduleId, uint32	mode)
{
	uint32	status = NO_ERROR;
	uint32	taskId = 0;

    if(moduleId == LOGIC_ID){
		sSysSegSCHDInfo.mpuSchdInfo.schedulerMode = mode;
        for(taskId = 1; taskId <= MAX_MPU_SCHD_SEG_ID; taskId++)
			schedulerStartFlagSet(LOGIC_ID, taskId ,SCHEDULING_FLAG_ENABLE);
	}
	else
    	status = L_MODULE_ID_ERR;

	return(status);
}	


void schedulerStartFlagSet(uint32 moduleId, uint32 taskId, uint32 val)
{
	strMpuSegExecInfo	*mpuExecInfoPtr;
	strDiuSegExecInfo	*diuExecInfoPtr;
	uint32 diuSegId; 
	
	if(moduleId == LOGIC_ID) {
    	mpuExecInfoPtr = &gMpuSegSchdInfoPtr->cyclicSeg[taskId - 1];
        if(mpuExecInfoPtr->taskId == taskId) {
			mpuExecInfoPtr->schedulingEnableFlag = val;
			
			mpuExecInfoPtr->segStatus = FB_RUNNING_DONE;
            return;
        }

    	mpuExecInfoPtr = &gMpuSegSchdInfoPtr->nonCyclicSeg;
        if(mpuExecInfoPtr->taskId == taskId) {
			mpuExecInfoPtr->schedulingEnableFlag = val;
            return;
        }
	}
}


#ifdef VXWORKS
void clockIntr()
{
	tickAnnounce ();	
#if 0
	segSchedulerISR();
#endif
}
#endif

#ifdef _WIN32
void CALLBACK clockIntr(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	segSchedulerISR();
}
void CALLBACK wdtIntr(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	wdgCyclicMngrISR(0);
}
#endif


#ifdef SYS_AUX_CLK_USED
void auxClockIntr()
{
#ifdef VXWORKS
    segSchedulerISR();
#endif
	one100MicroSecondISR();	
}
#endif

uint32 usrTaskSyncVarInit(void)
{
	uint32 status = NO_ERROR;

#ifdef _WIN32
	TIMECAPS tc;
    uint32 resolution;
	uint32 schdTime = SCHD_TIME_BASE_ONEMS;
#endif
	
	
	status = fbTaskVarInit();
    if(status != NO_ERROR) {return(status);}

    if ((SCHD_TIME_BASE != SCHD_TIME_BASE_ONE100US) && 
        (SCHD_TIME_BASE != SCHD_TIME_BASE_ONEMS) && 
    	(SCHD_TIME_BASE != SCHD_TIME_BASE_TENMS))
	{
		status = SCHDULER_TYPE_ERR;
    	return (status);
	}

#ifdef _WIN32
	timeGetDevCaps(&tc, sizeof(TIMECAPS));
    resolution = min(max(tc.wPeriodMin, 1), tc.wPeriodMax);
	timeBeginPeriod(resolution);    
    
	
	timeSetEvent(schdTime, resolution, clockIntr, 0, TIME_PERIODIC);  
#endif

#ifdef VXWORKS
	
    systemClockDisable();

    systemClockIntrSet ((func_ptr) clockIntr, 0);	
    
     
	systemClockRateSet((int32)BOARD_CLK_RATE);
	
	systemClockEnable();

#ifdef SYS_AUX_CLK_USED
	systemUsrDefinedAuxClockSet();
#endif

#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD) || (BOARD_TYPE == LKV120_BOARD))
    (void)systemClockTimeSet();
#elif (BOARD_TYPE == MVME2500_BOARD)

#endif
#endif

   return(status);
}


#ifdef SYS_AUX_CLK_USED
void systemUsrDefinedAuxClockSet(void)
{
	
    systemAuxClockDisable();

    systemAuxClockIntrSet ((func_ptr) auxClockIntr, 0);	
	systemAuxClockRateSet(BOARD_AUX_CLK_RATE);
	
	systemAuxClockEnable();
}
#endif

void schedulerDebugVarInit(void)
{
	strSysConfigInfo sysCnfgInfo;
	uint32	ioScanTime;
		
	memoryClear	( 	(uint8 *) &sSchdDebugInfo,
    				sizeof(strSysTaskSchedDebugInfo)
                );

	sFbSchdDbgInfoPtr	= &sSchdDebugInfo.fbTaskSchdDebugInfo;	
	sIOSchdDbgInfoPtr	= &sSchdDebugInfo.ioScanTaskSchdDebugInfo;
	sXRSchdDbgInfoPtr	= &sSchdDebugInfo.xrTaskSchdDebugInfo;
	
	sSchdDebugInfo.schdTimeBase 			= SCHD_TIME_BASE_ONE100US;
	sXRSchdDbgInfoPtr->maxSchdTime 			= XRTASK_SCHD_TIME;
	sFbSchdDbgInfoPtr->cyclicMaxSchdTime 	= CYCLIC_FB_EXEC_TIME_BASE;

	systemCnfgInfoRead( (strSysConfigInfo *)&sysCnfgInfo);

	switch(sysCnfgInfo.IOIfBoardKind) {
		case FBC_311_BOARD:
			ioScanTime = FBC311_IO_SCAN_TIME_BASE;
		break;	
		case FBC_351_BOARD:
		case FBC_352_BOARD:
#ifdef BUCHEON_SPECIFIC_121009
			ioScanTime = SCHD_TIME_BASE*IO_SCAN_TIME_BASE_CONST*2;
#else
			ioScanTime = SCHD_TIME_BASE*IO_SCAN_TIME_BASE_CONST;
#endif
		break;	
		case FBC_601_BOARD:
			ioScanTime = FBC601_IO_SCAN_TIME_BASE;
		break;	
		case FBC_NONE:
		default:
			ioScanTime = FBCNONE_IO_SCAN_TIME_BASE;
		break;	
	}
    sIOSchdDbgInfoPtr->maxSchdTime = ioScanTime;
}


void minumumCycleTimeRead(float *dataPtr, uint32 taskId)
{
	float	cycleTime;
	uint32	segIndex, miniCycleTimeId = 0, saveSegId = 0;
	strMpuSegExecInfo		*cyclicExecInfoPtr;

	cycleTime = 0.0;
	
	if( gMpuSegSchdInfoPtr->cyclicSegNum != 0){
    	for(segIndex = 0; segIndex < MAX_MPU_CYCLIC_SEG_ID; segIndex++) {
	  		cyclicExecInfoPtr 	= (strMpuSegExecInfo *)&gMpuSegSchdInfoPtr->cyclicSeg[segIndex];

    		if(	(cyclicExecInfoPtr->schedulingEnableFlag == SCHEDULING_FLAG_ENABLE) &&
				(cyclicExecInfoPtr->taskId != 0x0)
				){
				if(	(miniCycleTimeId == 0x0) ||
					(miniCycleTimeId > cyclicExecInfoPtr->cyclicAndNonCyclicTimeId)
				){
					miniCycleTimeId = cyclicExecInfoPtr->cyclicAndNonCyclicTimeId;
					saveSegId = segIndex + 1;
				}
			}
		}

		
		
		
  		cyclicExecInfoPtr 	= (strMpuSegExecInfo *)&gMpuSegSchdInfoPtr->cyclicSeg[saveSegId-1];
		if( (taskId == saveSegId) &&
			(cyclicExecInfoPtr->mpuSegPhaseRunFlag == SCHEDULING_FLAG_ENABLE)
		){
			cycleTime = (float) (CYCLIC_FB_EXEC_TIME_BASE * 0.001);
			cycleTime *= miniCycleTimeId;
		}
		else {
			cycleTime = 0.0;
		}
	}
	else if(gMpuSegSchdInfoPtr->nonCyclicSegNum != 0) {
		
		cycleTime = (float) (CYCLIC_FB_EXEC_TIME_BASE * 0.001);
		cycleTime *= gMpuSegSchdInfoPtr->nonCyclicSeg.cyclicAndNonCyclicTimeId;
	}
	
	*dataPtr = cycleTime;
} 


#define	IN_TIME		(0)
#define	OVERTIME	(1)

void schedulerInfoDisplay(uint32 arg1, uint32 arg2)
{
	int8 *schdModeString[] =
    {
    	"UNKNOWN_MODE","SCHEDULER_DISABLE","SCHEDULER_ENABLE"
	};
	int8 *segStatusString[] =
    {
		"UNKNOWN_STATUS", "FB_RUNNING_READY","FB_RUNNING_DONE"
    };
	int8 *overTimeString[] =
    {
    	"IN TIME", "OVERTIME", "UNKNOWN"
	};
    int8 *segSchdModeStr[] =
    {
    	"UNKNOWN", "SCHEDULING_RUNNING", "SCHEDULING_WAITTING"
	};
    int8 *segExecMode[] =
    {
		"UNKNOWN","CYCLIC_RUNNING_MODE", "NON_CYCLIC_RUNNING_MODE"
	  , "EVENT_RUNNING_MODE"
    };
	int8 *flagStr[] =
    {
    	"DISABLE", "ENABLE", "UNKNOWN"
	};
	uint8 errorString[128];

	uint32 indexString, segNum, errNum;

	strMpuSegSchedulerInfo	*mpuSchdInfoPtr = &sSysSegSCHDInfo.mpuSchdInfo;
	strDiuSegSchedulerInfo	*diuSchdInfoPtr = &sSysSegSCHDInfo.diuSchdInfo;
	strExecTimeInfo		*execTimePtr;
	strFbRunningErrInfo	*errInfoPtr;
	strDiuSegExecInfo	*diuSegExecInfoPtr;
	strMpuSegExecInfo	*mpuSegExecInfoPtr;

    printf( "\n\t**********************************************\n");
    printf( "\t	FB Logic SCHEDULER INFO\n");
    printf( "\t**********************************************\n");

	if(	(mpuSchdInfoPtr->schedulerMode != SCHEDULER_ENABLE) &&
		(mpuSchdInfoPtr->schedulerMode != SCHEDULER_DISABLE)
	  )
      	indexString = 0;
	else
    	indexString = mpuSchdInfoPtr->schedulerMode;

	printf("\t           Scheduler Mode : %s\n",schdModeString[indexString]);
	printf("\t      FB Logic Tsk Number : %d\n",mpuSchdInfoPtr->cyclicSegNum);
	printf("\tFB Logic Sema Rel Err Cnt : %d\n",mpuSchdInfoPtr->cyclicsemaReleaseErrCnt);

	if(mpuSchdInfoPtr->cyclicSegNum != 0) 
	{
		printf( "\t     Scheduling Time Base : %d * 100us\n",CYCLIC_FB_EXEC_TIME_BASE);

	    if(	(mpuSchdInfoPtr->overTimeFlag != IN_TIME) &&
			(mpuSchdInfoPtr->overTimeFlag != OVERTIME))
      		indexString = 2;
		else
    		indexString = mpuSchdInfoPtr->overTimeFlag;

		printf( "\t FB Logic OverTime Status : %s\n",overTimeString[indexString]);
	}

	printf("\n\n");
	
	for(segNum = 0; segNum <= MAX_MPU_CYCLIC_SEG_ID; segNum++) 
	{
		mpuSegExecInfoPtr = &mpuSchdInfoPtr->cyclicSeg[segNum];
		
		if(mpuSegExecInfoPtr->taskId == 0x0) continue;
		
		printf( "\t             FB Logic Task Id : %d\n", mpuSegExecInfoPtr->taskId);
		printf( "\t      FB Logic Cyclic Time Id : %d\n",mpuSegExecInfoPtr->cyclicAndNonCyclicTimeId);

		if(	(mpuSegExecInfoPtr->schedulingMode != SCHEDULING_RUNNING) &&
			(mpuSegExecInfoPtr->schedulingMode != SCHEDULING_WAITTING)
		)
			indexString = 0;
		else
			indexString = mpuSegExecInfoPtr->schedulingMode;
		
		printf( "\t         Taks Scheduling Mode : %s\n",segSchdModeStr[indexString]);

        if( (mpuSegExecInfoPtr->schedulingEnableFlag == SCHEDULING_FLAG_ENABLE) ||
        	(mpuSegExecInfoPtr->schedulingEnableFlag == SCHEDULING_FLAG_DISABLE)
		)
        	indexString = mpuSegExecInfoPtr->schedulingEnableFlag;
		else
        	indexString = 2;

		printf( "\t         Task Scheduling Flag : %s\n",flagStr[indexString]);

        if(	(mpuSegExecInfoPtr->segStatus != FB_RUNNING_READY) &&
        	(mpuSegExecInfoPtr->segStatus != FB_RUNNING_DONE)
		)
        	indexString = 0;
		else
        	indexString = mpuSegExecInfoPtr->segStatus;

        printf( "\t       Task Scheduling Status : %s\n",segStatusString[indexString]);

        if(	(mpuSegExecInfoPtr->segExecMode != CYCLIC_RUNNING_MODE) &&
        	(mpuSegExecInfoPtr->segExecMode != NON_CYCLIC_RUNNING_MODE) &&
	       	(mpuSegExecInfoPtr->segExecMode != EVENT_RUNNING_MODE)
		)
        	indexString = 0;
		else
        	indexString = mpuSegExecInfoPtr->segExecMode;

		printf( "\t        Task Scheduling Count : %d\n",mpuSegExecInfoPtr->schedulingCount);  	

		execTimePtr = &mpuSegExecInfoPtr->segExecTime;

		printf( "\t       Task Exec Min Time[ms] : %f\n", execTimePtr->minExecTime);        
		printf( "\t       Task Exec Cur Time[ms] : %f\n", execTimePtr->curExecTime);        
		printf( "\t       Task Exec Max Time[ms] : %f\n", execTimePtr->maxExecTime); 

		printf( "\n"); 
	}  

	return;	
}


void schedulerExecInfoDisplay(uint32 arg1, uint32 arg2)
{
	int8 *titleHelp[] =
    {
    	"SCTB	- Scheduler Time Base (100micro Sec)",
    	"FBSC	- FB Scheduling Count",
    	"FBOT	- FB Scheduling Overtime Count",
        "FBET	- FB Elapsed Time(100micro Sec)",
        "FBMT	- FB Max Time Base(100micro Sec)",
		"SAIC	- System Aux Clock Interrupt Count",
 		NULL
    };
    
	int8 *stringTitle[] =
    {
    	"SCTB","FBSC","FBOT", "FBET","FBMT","SAIC",	NULL
    };
    
	int8 **tMsg;

    printf( "\t\t [Title Message Help List]\n");

    for(tMsg = titleHelp; *tMsg != NULL; tMsg++)
        printf( "\t%s\n", *tMsg);

	printf("\n");
	
    printf( "\tBOARD CLOCK RATE     :: %d\n", (uint32)systemClockRateGet());
    printf( "\tBOARD AUX CLOCK RATE :: %d\n", (uint32)systemAuxClockRateGet());

    printf( "\n\n");

    printf( "\t");
    for(tMsg = stringTitle; *tMsg != NULL; tMsg++)
        printf( "%5s", *tMsg);

    printf( "\n");

    printf( "\t");
    for(tMsg = stringTitle; *tMsg != NULL; tMsg++)
        printf( "%5s", "----");

    printf( "\n");

   	while(!keyHit()) 
   	{
        printf( "\t%5d%5d%5d%5d%5d%5d\r",
        			sSchdDebugInfo.schdTimeBase,
                    sFbSchdDbgInfoPtr->cyclicSchdCount%9999,
                    sFbSchdDbgInfoPtr->cyclicOvertimeCount%9999,
        			sFbSchdDbgInfoPtr->cyclicElapsedTime,
                    sFbSchdDbgInfoPtr->cyclicMaxSchdTime,
					gOne100usecData
				);
	} 
	
	printf("\n\n");
}



uint32 getTotalTaskNumber(uint32 moduleId, uint32 taskId, uint32* totalTaskNum)
{
	uint32 status = NO_ERROR;
	strSysConfigInfo sysCnfgInfo;
	strModuleExecInfo  sysExeInfo;

	memoryClear((uint8*)&sysCnfgInfo,sizeof(strSysConfigInfo));
	memoryClear((uint8*)&sysExeInfo,sizeof(strModuleExecInfo));
	
	systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);
	if((status != NO_ERROR)){
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
	}
	status = fbSysExeModuleInfoRead(moduleId, &sysExeInfo);
	if(status != NO_ERROR){
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
	}

	switch(moduleId){
		case LOGIC_ID:
			*totalTaskNum = gMpuSegSchdInfoPtr->cyclicSegNum + gMpuSegSchdInfoPtr->nonCyclicSegNum;

			if( (gMpuSegSchdInfoPtr->cyclicSegNum >MAX_LOGIC_TASK_NUM) ||
		    	(gMpuSegSchdInfoPtr->nonCyclicSegNum > 1) ||
				(*totalTaskNum > sysExeInfo.curMaxSegmentId )
			){
				status = SEG_NUM_EXCEED_ERR;
				setErrorCodeWithVal(__FILE__,__LINE__,__FUNCTION__,status,
									  "Module ID" , moduleId,
									  "Total Task Num " , *totalTaskNum, 
									  "Cur Max Task" , sysExeInfo.curMaxSegmentId
									);

				*totalTaskNum= 0;
			}
		break;

		default:
			status = L_MODULE_ID_ERR;
		break;
	}

	return(status);
}


uint32 getTaskExecTime(uint32 moduleId, uint32 taskId,strExecTimeInfo *taskTimeInfo)
{
	uint32 status = NO_ERROR;
	strMpuSegExecInfo *mpuExecInfoPtr = NULL;
	strDiuSegExecInfo *diuExecInfoPtr;
	strFbTaskTblMemInfo  segTblInfo;
	uint32 diuSegId = 0;

	switch(moduleId)
	{
	case  LOGIC_ID:
		status = getFbTaskTblInfo(moduleId , taskId , &segTblInfo);
		if(status == NO_ERROR)
		{
			if((segTblInfo.execMode ==CYCLIC_RUNNING_MODE) || (segTblInfo.execMode ==EVENT_RUNNING_MODE)){
				mpuExecInfoPtr = (strMpuSegExecInfo *)&gMpuSegSchdInfoPtr->cyclicSeg[segTblInfo.taskId- 1];
			}
			else if (segTblInfo.execMode == NON_CYCLIC_RUNNING_MODE) {
				mpuExecInfoPtr = (strMpuSegExecInfo *)&gMpuSegSchdInfoPtr->nonCyclicSeg;
			}
			else {
				status = SEG_EXEC_MODE_ERR;
				setErrorCodeWithVal(__FILE__,__LINE__ ,__FUNCTION__,status,
									 "MODULE ID ", moduleId,
									 "TASK ID", taskId,
									 "EXEC MODE",segTblInfo.execMode);
			}
			
			if(mpuExecInfoPtr!=NULL) {
				taskTimeInfo->minExecTime = mpuExecInfoPtr->segExecTime.minExecTime;
				taskTimeInfo->curExecTime = mpuExecInfoPtr->segExecTime.curExecTime;
				taskTimeInfo->maxExecTime = mpuExecInfoPtr->segExecTime.maxExecTime;
			}
		}


	break;

	default:
		status = L_MODULE_ID_ERR ;
	break;
	}

	if(status != NO_ERROR){
		taskTimeInfo->minExecTime = 0;
		taskTimeInfo->curExecTime = 0;
		taskTimeInfo->maxExecTime = 0;
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
	}

	return(status);
}



uint32 	mpusegSchedulerStartAddrSet(uint32 taskId)
{
	uint32	status = NO_ERROR;
	strFbTaskTblMemInfo segTblInfo;
	strMpuSegExecInfo *nonCyclicExecInfoPtr;
	strFbExecutionInfo *startDataPtr;
	strMpuSegSchedulerInfo	*mpuSchdInfoPtr = &sSysSegSCHDInfo.mpuSchdInfo;

	memoryClear((uint8*)&segTblInfo,sizeof(strFbTaskTblMemInfo));
   	status = getFbTaskTblInfo	( LOGIC_ID, taskId, (strFbTaskTblMemInfo	*) &segTblInfo);
	if(status != NO_ERROR){
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
		return(status);		
	}

	if( (segTblInfo.logicType == FB_LOGIC_TYPE) && 
		(segTblInfo.execMode == NON_CYCLIC_RUNNING_MODE) )
	{
		nonCyclicExecInfoPtr 	= (strMpuSegExecInfo *)&mpuSchdInfoPtr->nonCyclicSeg;
		startDataPtr = fbExecIdStartMemInfoGet(taskId);

		if(startDataPtr != NULL)
			nonCyclicExecInfoPtr->startFbBlockAddr = startDataPtr->fbBlockAddr;
		else
			nonCyclicExecInfoPtr->startFbBlockAddr = 0;
	}
	return(status);
}


uint32 schedulerTimeBaseSet()
{
	uint32 status = NO_ERROR;
	strSysConfigInfo configData;
	
	status = systemCnfgInfoRead((strSysConfigInfo *)&configData);
	
	if(status != NO_ERROR) 
		return(status);

	if(configData.sysSchedlueType == ONE100US_SCHEDULE_CYCLE)
		SCHD_TIME_BASE = SCHD_TIME_BASE_ONE100US;
		
	else if(configData.sysSchedlueType == ONEMS_SCHEDULE_CYCLE)
		SCHD_TIME_BASE = SCHD_TIME_BASE_ONEMS;
		
	else
	{
		status = SCHDULER_TYPE_ERR;
		return(status);
	}

#ifdef _WIN32
	XRTASK_SCHD_TIME 				= SCHD_TIME_BASE*XRTASK_SCHD_TIME_CONST*2;
#else
	XRTASK_SCHD_TIME 				= SCHD_TIME_BASE*XRTASK_SCHD_TIME_CONST;
#endif

	CYCLIC_FB_EXEC_TIME_BASE 		= SCHD_TIME_BASE*CYCLIC_FB_EXEC_TIME_BASE_CONST;
	NON_CYCLIC_FB_EXEC_TIME_BASE 	= SCHD_TIME_BASE*NON_CYCLIC_FB_EXEC_TIME_BASE_CONST;

	return(status);
}
