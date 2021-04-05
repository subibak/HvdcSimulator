
#ifndef TASKDEF_H
#define TASKDEF_H


#define	MAX_USR_TASK_NUM	60
#define MAX_ARG_NUM     	10
#define	MAX_TASK_NAME_LEN	32

typedef struct task_info{
    char    taskName[MAX_TASK_NAME_LEN]; 
    int32   taskPriority;           
    int32   taskOptions;            
    int32   taskStackSize;          
    func_ptr    taskEntry;          
    int32   taskArgs[MAX_ARG_NUM];  
    int32   taskStatus;             
    int32   taskSliceTime;          
                                    
    void	*taskHandle;
	int32	taskId;	
}strTaskInfo;

typedef struct {
	void*	TaskHandle;
	int32	TaskId;   
	char*	td_name;  
	uint32  TaskKind;
	uint32  td_status;
} strTaskListInfo;

#ifndef TASK_SUSPEND
	#define	TASK_SUSPEND	1
#endif

#ifdef _WIN32
#ifndef USR_TASK_RUN
	#define	USR_TASK_RUN		1234
#endif
#endif

#define	DEBUG_DISABLE		(0x0)
#define	DEBUG_SYSTASK		(0x1)
#define	DEBUG_MPUTASK		(0x2)
#define	DEBUG_DIUTASK		(0x3)
#define	DEBUG_NETTASK		(0x4)
#define	DEBUG_DLUTASK		(0x5)
#define	DEBUG_TMTCTASK		(0x6)

#ifdef VXWORKS
#define MAX_TASK_LIST		(15)
#else
#define MAX_TASK_LIST		(36)
#endif

#endif 
