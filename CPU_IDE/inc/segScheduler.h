
#ifndef SEGSCHEDULER_H
#define SEGSCHEDULER_H


#define	SEG_EXEC_INTIME		(0)
#define	SEG_EXEC_OVERTIME	(1)


#define	BOARD_CLK_RATE		(1000)     /* Sys Clock 1msec */


#define	BOARD_AUX_CLK_RATE	(10000)     /* Aux Clock 1msec --> 100usec */


#define	SCHD_TIME_BASE_ONE100US		(1)	
#define	SCHD_TIME_BASE_ONEMS		(10)	
#define	SCHD_TIME_BASE_TENMS		(100)	
#define	XRTASK_SCHD_TIME_CONST		(10)	
#ifdef _WIN32
#define	RAS_TASK_SCHD_TIME	(100)
#endif



#define	CYCLIC_FB_EXEC_TIME_BASE_CONST		(1)		
#define	NON_CYCLIC_FB_EXEC_TIME_BASE_CONST	(1)	

extern uint32 CYCLIC_FB_EXEC_TIME_BASE;
extern uint32 NON_CYCLIC_FB_EXEC_TIME_BASE;

#define	FBCNONE_IO_SCAN_TIME_BASE		(100)	/* m second */
#define	FBC311_IO_SCAN_TIME_BASE		(50)	/* m second */
#define	FBC601_IO_SCAN_TIME_BASE		(100)	/* m second */

#define	IO_SCAN_TIME_BASE_CONST			(1)		
#define	TMTC_SCAN_TIME_BASE_CONST		(5)	


#define	MAX_MPU_CYCLIC_SEG_ID		(15)
#define	MAX_MPU_NONCYCLIC_SEG_ID	(1)
#define	MAX_MPU_SCHD_SEG_ID			(MAX_MPU_CYCLIC_SEG_ID + \
									MAX_MPU_NONCYCLIC_SEG_ID) 


#if (MAXIOSHELF_CONFIG == MAX16IOSHELF)
	#define	MAX_IO_SCHD_SEG_ID	(16)
#elif (MAXIOSHELF_CONFIG == MAX8IOSHELF)
	#define	MAX_IO_SCHD_SEG_ID	(8)
#elif (MAXIOSHELF_CONFIG == MAX12IOSHELF)
	#define	MAX_IO_SCHD_SEG_ID	(12)
#else 
	#error "You Should Define MAXIOSHELF_CONFIG(8 or 16)!!!!"
#endif
#define	MAX_TMTC_SCHD_SEG_ID		15
#define MAX_FIELDBUS_SCHD_SEG_ID	4

#define	MAX_DIU_SCHD_SEG_ID	(MAX_IO_SCHD_SEG_ID+MAX_TMTC_SCHD_SEG_ID+MAX_FIELDBUS_SCHD_SEG_ID) 


#define	MIN_CYCLIC_TIME_ID		(1)
#define	MAX_CYCLIC_TIME_ID		(1000) 

#define MAX_SEG_PRIORITY		(256)


#define	FB_TASK_SYNC_ENABLE		(1)
#define	FB_TASK_SYNC_DISABLE	(2)


#define	TASK_OVERTIME_CHK_ENABLE	(1)
#define	TASK_OVERTIME_CHK_DISABLE	(2)


typedef struct {
	uint32	errorCode;
	uint32	fbId;
	uint32  fbAddr;
}strFbRunningErrInfo;



#define	SCHEDULING_RUNNING		(1)
#define	SCHEDULING_WAITTING		(2)
		

#define	CYCLIC_RUNNING_MODE		(1)
#define	NON_CYCLIC_RUNNING_MODE	(2)
#define	EVENT_RUNNING_MODE		(3)


#define	FB_RUNNING_READY		(1)	
#define	FB_RUNNING_DONE			(2)	

#define	FB_LOOPING_ERR			(1)	

#define	MAX_FB_ERR_CODE_NUM		(128)


#define	SCHEDULING_FLAG_DISABLE	(0)
#define	SCHEDULING_FLAG_ENABLE	(1)


#define	SEG_SCHEDULING_FIRST	(0)
#define	SEG_SCHEDULING_DOING	(1)


#define SYS_INIT_FLAG_CLEAR		(0)
#define	SYS_INIT_FLAG_DONE		(1)


#define	EVENT_FLAG_DISABLE	(0)
#define	EVENT_FLAG_ENABLE	(1)

#define EVENT_EDGE_TYPE (0)
#define EVENT_LEVEL_TYPE (1)

typedef struct {
	float	minExecTime;
	float	curExecTime;
	float	maxExecTime;
}strExecTimeInfo;

typedef struct {
	uint32	taskId;
    uint32	cyclicAndNonCyclicTimeId; 	
	uint32	phaseSchedulingCount;		
	uint32	mpuSegPhaseRunFlag;			
										
										
    uint32	schedulingMode;				
    									
	uint32	schedulingEnableFlag;		
										
										
    uint32	segStatus;		
    						 
    						
                            
                            
                            
                            

    uint32	segExecMode; 	
    uint32	segSchedulingStartFlag;	
    uint32	segFbBlockAddr;
    uint32	startFbBlockAddr;
    uint32	endFbBlockAddr;
    uint32	nextExecFbBlockAddr;	
    uint32	schedulingCount;
	uint32	nonCyclicSyncCount;		
									
									  	
	strExecTimeInfo		segExecTime;
	uint32  maxTimeSchdCnt;

	uint32  runErrCount;
	uint32	fbErrCount;
    strFbRunningErrInfo	errInfoList[MAX_FB_ERR_CODE_NUM];

	uint32  eventTaskRunFlag;
	uint32  eventPreVar;
	uint32  eventVarType;
	uint32  logicType;
}strMpuSegExecInfo;

typedef struct {
	uint32  moduleId;
	uint32	taskId;
    uint32	segStatus;
	uint32	schedulingEnableFlag;	
									
									
    uint32	segFbBlockAddr;
    uint32	startFbBlockAddr;
    uint32	endFbBlockAddr;

    uint32	schedulingCount;

	strExecTimeInfo		segExecTime;

	uint32  runErrCount;
	uint32	fbErrCount;
    strFbRunningErrInfo	errInfoList[MAX_FB_ERR_CODE_NUM];
}strDiuSegExecInfo;

typedef struct {
	uint32	taskId;
    uint32	segStatus;
	uint32	schedulingEnableFlag;	
									
									
    uint32	segFbBlockAddr;
    uint32	startFbBlockAddr;
    uint32	endFbBlockAddr;

    uint32	schedulingCount;

	strExecTimeInfo		segExecTime;

	uint32  runErrCount;
	uint32	fbErrCount;
    strFbRunningErrInfo	errInfoList[MAX_FB_ERR_CODE_NUM];
}strTmtcSegExecInfo;



typedef struct {
	uint32 	schedulerMode;	
    uint32	cyclicSegNum;	 
    uint32	overTimeFlag;	
    uint32	schedulingCount;
	strMpuSegExecInfo	cyclicSeg[MAX_MPU_CYCLIC_SEG_ID+1]; 

    uint32	nonCyclicSegNum;
    strMpuSegExecInfo	nonCyclicSeg;

	uint32	eventTaskNum;	
	uint32  cyclicsemaReleaseErrCnt; 
}strMpuSegSchedulerInfo;




#define IO_SCAN_SEMA_RELEASED		(1)
#define	IO_SCAN_SEMA_NOT_RELEASED	(2)

typedef struct {
	uint32 	schedulerMode;	
    uint32	scanStatus;		
    uint32	overTimeFlag;	
    uint32	cycleTimeId;
    uint32	schedulingCount;
	strExecTimeInfo		ioScanTime;

    uint32	diuSegNum;  
    uint32	ioSegNum; 	 
    uint32	tmtcSegNum;  
    uint32	fieldbusSegNum;  
	strDiuSegExecInfo	diuSeg[MAX_DIU_SCHD_SEG_ID];
}strDiuSegSchedulerInfo;



#define		SCHEDULER_DISABLE	(1)
#define		SCHEDULER_ENABLE	(2)

typedef struct {
	strMpuSegSchedulerInfo	mpuSchdInfo;
	strDiuSegSchedulerInfo	diuSchdInfo;
}strSystemSchedulerInfo;

typedef struct{
	uint32  priority;
	uint32  taskId;
}strSchdPriorityInfo;

#endif 




