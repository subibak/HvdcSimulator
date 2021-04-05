

#include	<stdio.h>
#include	"usrTypes.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include	"segScheduler.h"
#include	"usrSema.h"

#include	"systemDiag.h"
#include	"fbMem.h"
#include	"usrDefBuf.h"
#include	"fbDef.h"
#include	"osDepend.h"
#include	"usrErrCode.h"

#include	"network.h"
#include	"usrQueData.h"
#include	"msgQueData.h"
#include	"debugInfo.h"

#include	"sysDual.h"
#include	"sfcMem.h"

#include	"sysConfig.h"

int32	gCyclicFbTaskSemaId;
int32	gNonCyclicFbTaskSemaId;


static int32	sCyclicFbTaskElpTime = 0;
static int32	sCyclicFbTaskStartTime = 0, sCyclicFbTaskEndTime = 0;
static int32	sNonCyclicFbTaskElpTime = 0;
static int32	sNonCyclicFbTaskStartTime = 0, sNonCyclicFbTaskEndTime = 0;

static uint32	sMpuSegmentPrevTime[MAX_LOGIC_TASK_NUM];
static uint32	sMpuSegmentCurTime[MAX_LOGIC_TASK_NUM];

uint32	gMpuTaskSchdCount[MAX_LOGIC_TASK_NUM];
uint32	gSystemInitFlag[MAX_LOGIC_TASK_NUM];	
uint32 	gOutputScanFirstDelayDone = 0;

extern strMpuSegSchedulerInfo	*gMpuSegSchdInfoPtr;
extern strSystemRunningStatusInfo	gSysRunningStatusInfo; 


extern uint32  gUsrTaskSchedulingFlag;
extern uint32	gMpuSegmentSchedulingFlag;

extern strSchdPriorityInfo  sExecTaskId[MAX_MPU_CYCLIC_SEG_ID];


extern int32	gDebugPortFd;


uint32	fbTaskVarInit(void);
void usrFbExecTask(int32,int32,int32,int32,int32,
						int32,int32,int32,int32,int32); 
void nonCyclicFbSegRunTask(	int32,int32,int32,int32,int32,
							int32,int32,int32,int32,int32); 
static uint32 fbSegCodeRunFunc(strMpuSegExecInfo *);


extern uint32	semaphoreCreate(strSemaInfo *, int32 *);
extern uint32	semaphoreGet(int32, int32);
extern void		setErrorCode(uint8 *, uint32, uint8 *, uint32);
extern void		setErrorCodeWithVal(uint8 *, uint32, uint8 *, uint32,
						uint8 *,uint32,uint8 *,uint32,uint8 *,uint32);
extern uint32	fbCodeMemInfoRead(uint32, uint32, strFbMemInfo *);
extern uint32	taskLogicCtrlRunFunc(uint32);
extern int32	systemTickGet(void);
extern int32	systemClockRateGet(void);
extern void		memoryClear ( uint8 *, uint32 );
extern void		minFreeTimeCalcu(uint32, uint32, uint32);
extern uint32	getFbTaskTblInfo(uint32 , uint32 , strFbTaskTblMemInfo *);
extern void		onlineDownloadFlagCheckAndUpdate(uint32);
extern uint32	sysAuxTickCountGet(void);
extern int32	systemAuxClockRateGet(void);
extern uint32	readRuntimeFbData(uint32,uint32,uint32,uint32 *);
extern uint32	fbSysExeModuleInfoRead( uint32, strModuleExecInfo* );

#ifdef _WIN32
extern uint32	exceptFilterFunc(LPEXCEPTION_POINTERS, uint8*,uint32);
extern uint32   taskSuspendToListStatus(uint32);
#endif


uint32	fbTaskVarInit(void)
{
	uint32	status = NO_ERROR;
	strSemaInfo	semaInfo;

    
    semaInfo.semaType       =   BINARY_SEMA; 

    semaInfo.semaOptions    =   MYSEM_Q_PRIORITY;
    semaInfo.semaInitVal    =   SEM_EMPTY;
#ifdef _WIN32
	semaInfo.semaName		=	"CYCLICFBSema";
#endif
    status = semaphoreCreate(   (strSemaInfo *) &semaInfo,
                                (int32 *) &gCyclicFbTaskSemaId
                             );
	if(status) return(status);
#ifdef _WIN32
	semaInfo.semaName		=	"NONCYCLICFBSema";
#endif
    status = semaphoreCreate(   (strSemaInfo *) &semaInfo,
                                (int32 *) &gNonCyclicFbTaskSemaId
                             );
    return(status);
}


void usrFbExecTask (int a,int b,int c,int d,int e,int f,int g,int h,int ii,int j)
{
	strSystemRunningStatusInfo	*sysInfoPtr 
							= (strSystemRunningStatusInfo *)&gSysRunningStatusInfo; 
	strMpuSegSchedulerInfo	*schdInfoPtr = gMpuSegSchdInfoPtr;
					
	uint32	status = NO_ERROR, segIndex = 0, taskId;
	int32	startTime = 0, timeFactor;

#ifdef _WIN32  
__try {
#endif

	memoryClear((uint8 *)&gMpuTaskSchdCount[0],MAX_LOGIC_TASK_NUM*sizeof(uint32));

	
	memoryClear((uint8 *)&gSystemInitFlag[0],MAX_LOGIC_TASK_NUM*sizeof(uint32));

	for(;;) {
    	if(gUsrTaskSchedulingFlag == DEBUG_ENABLE){
#ifdef SYS_AUX_CLK_USED
	    	startTime = sysAuxTickCountGet();
#else
    		startTime = systemTickGet();
#endif
		}


		if((status = semaphoreGet(gCyclicFbTaskSemaId, FOREVERWAIT)) == NO_ERROR)
		{

       		if(	(sysInfoPtr->sysRunningConfigInfo.sysRunningMode 
						== SYS_RAS_SYSTEM_RUN) &&
                (sysInfoPtr->sysRunningConfigInfo.sysRedundancyStatus 
						== SYS_RAS_REDUNDANCY_RUN) &&
				(schdInfoPtr->schedulerMode == SCHEDULER_ENABLE) &&
                ( (schdInfoPtr->cyclicSegNum != 0) || (schdInfoPtr->eventTaskNum != 0) )
			){	               
                for(segIndex = 0; segIndex < MAX_MPU_CYCLIC_SEG_ID; segIndex++) 
                {
					
					taskId = sExecTaskId[segIndex].taskId;
					
					if( (taskId == 0) || (taskId > MAX_LOGIC_TASK_NUM)) continue;

					
					if(schdInfoPtr->cyclicSeg[taskId-1].schedulingEnableFlag == 0) 
					{
                    	continue;
					}

					status = fbSegCodeRunFunc(&schdInfoPtr->cyclicSeg[taskId-1]);

	    			schdInfoPtr->cyclicSeg[taskId-1].segStatus = FB_RUNNING_DONE;
				} 
				gOutputScanFirstDelayDone = 1;
			}
			else
			{
				gOutputScanFirstDelayDone = 1;
			}
        }
		else {
			setErrorCode(	(uint8 *)__FILE__,
							__LINE__,
							(uint8 *)"usrFbExecTask",
							status
						   );
		}

    	if(gUsrTaskSchedulingFlag == DEBUG_ENABLE) {
#ifdef SYS_AUX_CLK_USED
	    	sCyclicFbTaskEndTime = sysAuxTickCountGet();
	    	timeFactor = systemAuxClockRateGet();
#else
    		sCyclicFbTaskEndTime = systemTickGet();
    		timeFactor	= systemClockRateGet();
#endif
			sCyclicFbTaskStartTime = startTime;

			sCyclicFbTaskElpTime = (sCyclicFbTaskEndTime - sCyclicFbTaskStartTime);
			sCyclicFbTaskElpTime *= 1000;
			sCyclicFbTaskElpTime /= timeFactor;
		}	
		
		
		
		onlineDownloadFlagCheckAndUpdate(LOGIC_ID);
    }	

#ifdef _WIN32
  }
__except( exceptFilterFunc(GetExceptionInformation(), __FILE__,__LINE__ ) ) {
	taskSuspendToListStatus(GetCurrentThreadId());
}
#endif 
}


void nonCyclicFbSegRunTask(int a,int b,int c,int d,int e,int f,int g,int h,int i,int j)
{
	strSystemRunningStatusInfo	*sysInfoPtr 
							= (strSystemRunningStatusInfo *)&gSysRunningStatusInfo; 
	strMpuSegSchedulerInfo	*schdInfoPtr = gMpuSegSchdInfoPtr;
	uint32	status;
	int32	startTime = 0, timeFactor;
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

    	if((status = semaphoreGet(gNonCyclicFbTaskSemaId, FOREVERWAIT)) == NO_ERROR) {
       		if(	(sysInfoPtr->sysRunningConfigInfo.sysRunningMode 
						== SYS_RAS_SYSTEM_RUN) &&
                (sysInfoPtr->sysRunningConfigInfo.sysRedundancyStatus 
						== SYS_RAS_REDUNDANCY_RUN) &&
				(schdInfoPtr->schedulerMode == SCHEDULER_ENABLE) &&
                (schdInfoPtr->nonCyclicSegNum != 0)
			){
  				fbSegCodeRunFunc(&schdInfoPtr->nonCyclicSeg);
  				gOutputScanFirstDelayDone = 1;
			}
			else
			{
				gOutputScanFirstDelayDone = 1;
			}
        }
		else {
			setErrorCode(	(uint8 *)__FILE__,
							__LINE__,
							(uint8 *)"nonCyclicFbSegRunTask",
							status
						   );
		}

    	if(gUsrTaskSchedulingFlag == DEBUG_ENABLE) {
#ifdef SYS_AUX_CLK_USED
	    	sNonCyclicFbTaskEndTime = sysAuxTickCountGet();
	    	timeFactor = systemAuxClockRateGet();
#else
    		sNonCyclicFbTaskEndTime = systemTickGet();
    		timeFactor	= systemClockRateGet();
#endif
			sNonCyclicFbTaskStartTime = startTime;

			sNonCyclicFbTaskElpTime = (sNonCyclicFbTaskEndTime - sNonCyclicFbTaskStartTime);
			sNonCyclicFbTaskElpTime *= 1000;
			sNonCyclicFbTaskElpTime /= timeFactor;
		}
    }	
	
#ifdef _WIN32
  }
__except( exceptFilterFunc(GetExceptionInformation(), __FILE__,__LINE__) ) {
	taskSuspendToListStatus(GetCurrentThreadId());
}
#endif 
}


static uint32 fbSegCodeRunFunc(strMpuSegExecInfo *execInfoPtr)
{
	uint32	status = NO_ERROR;
	int32	fbStartTime, fbEndTime;
		
	if(	(execInfoPtr->taskId == 0) || (execInfoPtr->mpuSegPhaseRunFlag == SCHEDULING_FLAG_DISABLE))
		return(status);

#ifdef SYS_AUX_CLK_USED
	fbStartTime = sysAuxTickCountGet();
#else
	fbStartTime = systemTickGet();
#endif

	if(	(gMpuSegmentSchedulingFlag == DEBUG_ENABLE) && (execInfoPtr->taskId != 0)){
		sMpuSegmentPrevTime[execInfoPtr->taskId - 1] = sMpuSegmentCurTime[execInfoPtr->taskId - 1];
		sMpuSegmentCurTime[execInfoPtr->taskId - 1]  = fbStartTime;
	}

    status = taskLogicCtrlRunFunc(execInfoPtr->taskId); 

#ifdef SYS_AUX_CLK_USED
	fbEndTime = sysAuxTickCountGet();
#else
	fbEndTime = systemTickGet();
#endif
            
    minFreeTimeCalcu(LOGIC_ID, (uint32)execInfoPtr->taskId, fbEndTime - fbStartTime);

    return(status);
}


void fbTaskSchdInfoDisplay(void)
{
    uint32	startTime;
    uint32	endTime;
	uint32	elpTime;

    
    startTime 	= sCyclicFbTaskStartTime; 
    endTime		= sCyclicFbTaskEndTime;
	elpTime		= sCyclicFbTaskElpTime;

	printf("%18s%18d%18d%18d\n", 
			"FBExecTask", startTime, endTime, elpTime);

	return;    
}


void mpuSchdInfoDisplay(void)
{
	uint32	segIndex, strIndex;
	strFbTaskTblMemInfo	segTblInfo;
    strModuleExecInfo		mpuExe;
	
    strMpuSegExecInfo	*execInfoPtr = NULL;
	strMpuSegSchedulerInfo	*schdInfoPtr = gMpuSegSchdInfoPtr; 
	uint8 *strJmpErr[] = {"NO_ERR", "JMPERR","INVAL"};

	int8 *segModeStr[] =
    {
		"NOMODE","INIT","DWLOAD",
		"DWDONE","STOP","RUN","DEBUG","ERROR", "INVAL"
    };
	strSysConfigInfo configData;
	uint32 factor;
	
	for(segIndex = 1; segIndex <= MAX_LOGIC_TASK_NUM; segIndex++) 
	{
		(void)getFbTaskTblInfo(LOGIC_ID, segIndex, &segTblInfo);
		
		if(segTblInfo.taskId == 0) 
			continue;
		else if(segTblInfo.taskId != segIndex) 
			continue;

		if(segTblInfo.taskMode > PCS_ERR)
			strIndex = 8;
		else
			strIndex = segTblInfo.taskMode;

			
		if( (segTblInfo.execMode ==CYCLIC_RUNNING_MODE) ||
			(segTblInfo.execMode ==EVENT_RUNNING_MODE) ) 
		{
			execInfoPtr = (strMpuSegExecInfo *)&schdInfoPtr->cyclicSeg[segIndex - 1];
		}
		else if (segTblInfo.execMode == NON_CYCLIC_RUNNING_MODE) 
		{
			execInfoPtr = (strMpuSegExecInfo *)&schdInfoPtr->nonCyclicSeg;
		}
		else 
		{
    		strIndex = 8;
		}

		if(execInfoPtr != NULL) 
		{
			printf("%3d:%03d%7s%7d%7x%7.2f%7.2f%7.2f%7d%7s%9x\n", 
								segTblInfo.taskPriority,
								segIndex, 
								segModeStr[strIndex],
								segTblInfo.taskCycleTime,
								gMpuTaskSchdCount[segIndex-1],
								execInfoPtr->segExecTime.minExecTime,
								execInfoPtr->segExecTime.curExecTime,
								execInfoPtr->segExecTime.maxExecTime,
								execInfoPtr->maxTimeSchdCnt, 
								strJmpErr[segTblInfo.loopStatus],
								segTblInfo.taskTickCnt								
				);
		}
	}


	(void)systemCnfgInfoRead((strSysConfigInfo *)&configData);

	/* ms로 표현 함으로 그에 맞게 factor을 정함 */	
	if(configData.sysSchedlueType == ONE100US_SCHEDULE_CYCLE)
		factor = 100;
		
	else if(configData.sysSchedlueType == ONEMS_SCHEDULE_CYCLE)
		factor = 10;
		
	else
	{
		return;
	}
		
	(void)fbSysExeModuleInfoRead(LOGIC_ID, &mpuExe);
	
	printf("\n\n * Min Free Time : %7.3f[ms]\n",	mpuExe.minFreeTime/factor);

	return;
}





