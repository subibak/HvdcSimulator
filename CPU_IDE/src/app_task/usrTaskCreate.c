
#include	<stdio.h>
#include	<string.h>
#include	"usrTypes.h"
#include	"sysDefs.h"
#include	"taskDefs.h"
#include	"osDepend.h"
#include	"sysConfig.h"
#include	"usrErrCode.h"
#ifdef  _MBTS_PROGRAM
#include	"lkv120Board.h"
#endif


#ifdef VXWORKS
/*[V101] : FB Over time 때문에 Priority 변경 : 110-->40 */
#define	CYCLIC_FB_TASK_PRI			(40) 
#define	NON_CYCLIC_FB_TASK_PRI		(120)

#define	DATA_RECV_TASK_PRI			(140) 	
#define	TCP_TASK_PRI			    (140)

#define	DATA_SEND_TASK_PRI			(140)
#define	CMD_PROC_TASK_PRI			(140)

#define	RAS_TASK_PRI				(120)	
#define	XR_TASK_PRI					(120)
#define	DBG_TASK_PRI				(100)

#define	DUAL_PROC_TASK_PRI			(115) 

#define	DUAL_COM_LINE_A_TASK_PRI	(110) 
#define	DUAL_COM_LINE_B_TASK_PRI	(110) 
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
#define	DUAL_RECV_TASK_PRI			(120)		
#define	DUAL_SEND_TASK_PRI			(120)	 
#define	DUAL_CHK_TASK_PRI			(115)	
#endif

#define	MBTS_TASK_PRI				(140)

#endif 

#ifdef _WINNT
#define	CYCLIC_FB_TASK_PRI			(THREAD_PRIORITY_TIME_CRITICAL)
#define	NON_CYCLIC_FB_TASK_PRI		(THREAD_PRIORITY_ABOVE_NORMAL)

#define	DATA_RECV_TASK_PRI			(THREAD_PRIORITY_NORMAL) 	
#define	TCP_TASK_PRI			      (THREAD_PRIORITY_NORMAL)

#define	DATA_SEND_TASK_PRI			(THREAD_PRIORITY_NORMAL)
#define	CMD_PROC_TASK_PRI			(THREAD_PRIORITY_NORMAL)

#define	RAS_TASK_PRI				(THREAD_PRIORITY_HIGHEST)	
#define	XR_TASK_PRI					(THREAD_PRIORITY_ABOVE_NORMAL)
#define	DBG_TASK_PRI				(THREAD_PRIORITY_HIGHEST)

#define	DUAL_PROC_TASK_PRI			(THREAD_PRIORITY_HIGHEST) 

#define	DUAL_COM_LINE_A_TASK_PRI	(THREAD_PRIORITY_ABOVE_NORMAL) 
#define	DUAL_COM_LINE_B_TASK_PRI	(THREAD_PRIORITY_ABOVE_NORMAL) 
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
#define	DUAL_RECV_TASK_PRI			(THREAD_PRIORITY_ABOVE_NORMAL)		
#define	DUAL_SEND_TASK_PRI			(THREAD_PRIORITY_ABOVE_NORMAL)	 
#define	DUAL_CHK_TASK_PRI			(THREAD_PRIORITY_ABOVE_NORMAL)	
#endif	

	#ifdef  _MBTS_PROGRAM
	#define	MBTS_TASK_PRI				THREAD_PRIORITY_NORMAL
	#endif
#endif 

#define	CYCLIC_FB_TASK_STACK_SIZE			(0x8000)
#define	NON_CYCLIC_FB_TASK_STACK_SIZE		(0x8000)
#define	DATA_RECV_TASK_STACK_SIZE			(0x8000)
#define	DATA_SEND_TASK_STACK_SIZE			(0x8000)   
#define	CMD_PROC_TASK_STACK_SIZE			(0x20000)  
#define	RAS_TASK_STACK_SIZE					(0x4000)
#define	XR_TASK_STACK_SIZE					(0x5000)
#define	DBG_TASK_STACK_SIZE					(0x4000)
#define	DUAL_PROC_TASK_STACK_SIZE			(0x20000)
#define	DUAL_COM_TASK_STACK_SIZE			(0x5000)

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
#define	DUAL_CHK_TASK_STACK_SIZE			(0x8000)
#define	DUAL_RECV_TASK_STACK_SIZE			(0x8000)
#define	DUAL_SEND_TASK_STACK_SIZE			(0x8000)
#endif

#if (defined(VXWORKS))
#ifdef  _MBTS_PROGRAM
#define	MBTS_TASK_STACK_SIZE				(0x8000)
#endif	
#elif (defined(_WIN32))
#ifdef  _MBTS_PROGRAM
#define	MBTS_TASK_STACK_SIZE				(0x8000)
#endif	
#endif

#ifdef _WIN32
strTaskListInfo gTaskList[MAX_USR_TASK_NUM];
static int	sTaskCount = 0;
#endif

extern void memoryClear(uint8 *, uint32);
extern void memoryCopy(uint8 *,uint8 *, uint32);
extern uint32 usrTaskCreate(strTaskInfo *,int32 *);
extern uint32 systemCnfgInfoRead( strSysConfigInfo *);

extern void usrFbExecTask(	int32,int32,int32,int32,int32,
								int32,int32,int32,int32,int32
							  ); 
extern void nonCyclicFbSegRunTask(	int32,int32,int32,int32,int32,
									int32,int32,int32,int32,int32
								); 
extern void usrNetRTask(	int32,int32,int32,int32,int32,
						int32,int32,int32,int32,int32
					  );
extern void netTcpSocketMngrTask(	int32,int32,int32,int32,int32,
						int32,int32,int32,int32,int32
						);
extern void netTcpStatusCheckTask(	int32,int32,int32,int32,int32,
						int32,int32,int32,int32,int32
						);
#ifdef _WIN32
#ifdef _IPC_INTERFACE
extern void	ipcDataSendTask(int32, int32,int32,int32,int32, int32, 
						int32,int32,int32,int32 );

#endif 
#if 111
extern void	procStatusCheckTask(int32, int32,int32,int32,int32, int32, 
						int32,int32,int32,int32 );
#endif
#endif 

extern void usrNCmdTask(	int32,int32,int32,int32,int32,
						int32,int32,int32,int32,int32
					  );
extern void usrNetSTask(	int32,int32,int32,int32,int32,
						int32,int32,int32,int32,int32
					  );
extern void uDiagnosisTask(	int32,int32,int32,int32,int32,
						int32,int32,int32,int32,int32
					  );
extern void xrTask(	int32,int32,int32,int32,int32,
						int32,int32,int32,int32,int32
					  );
extern void usrDbgTask(	int32,int32,int32,int32,int32,
						int32,int32,int32,int32,int32
					  );

extern void usrDualTask(	int32,int32,int32,int32,int32,
							int32,int32,int32,int32,int32
						);

extern void usrDC_ATask(	int32,int32,int32,int32,int32,
								int32,int32,int32,int32,int32
					  		);
extern void usrDC_BTask(	int32,int32,int32,int32,int32,
								int32,int32,int32,int32,int32
					  		);

#ifdef _USR_C_TASK_PROGRAM
extern void usrCProgTask_1(	int32,int32,int32,int32,int32,
								int32,int32,int32,int32,int32
					  		);
#endif 

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
extern void dualDataRecvTask(	int32,int32,int32,int32,int32,
						int32,int32,int32,int32,int32
					  );
extern void usrDualSTask(	int32,int32,int32,int32,int32,
						int32,int32,int32,int32,int32
					  );

extern void usrDualChkTask(	int32,int32,int32,int32,int32,
								int32,int32,int32,int32,int32
					  		);
#endif

#if (defined(VXWORKS))

#include <string.h>
#include "lkv120Board.h"

	#ifdef  _MBTS_PROGRAM	
	#include  "modbustcp\mbtServerProto.h"
	#endif	

#elif (defined(_WIN32))
	#ifdef  _MBTS_PROGRAM	
	#include  "modbustcp\mbtServerProto.h"
	#endif	

#endif


uint32	appTaskCreate(uint8 *tNamePtr)
{
    uint32				status = NO_ERROR;
    strTaskInfo     taskInfo;
    int32				tId;
	strSysConfigInfo	sysCnfgInfo;
	uint32				portNum =0;

#if (defined(VXWORKS))

	int index;

	#ifdef  _MBTS_PROGRAM	
	int iNumDevMbts;
	#endif	

#elif (defined(_WIN32))

#ifdef  _MBTS_PROGRAM	
	int index;
	int iNumDevMbts;
#endif	
#endif

    memoryClear ( (uint8 *) &sysCnfgInfo, sizeof(strSysConfigInfo));

	systemCnfgInfoRead( &sysCnfgInfo);
	
    memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));

    memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"uFbTask", strlen("uFbTask"));

    taskInfo.taskPriority   = CYCLIC_FB_TASK_PRI;
    taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
    taskInfo.taskStackSize  = CYCLIC_FB_TASK_STACK_SIZE;
    taskInfo.taskEntry      = (func_ptr)usrFbExecTask;

    taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
    taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
    taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
    taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
    taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;

    status = usrTaskCreate(&taskInfo,&tId);

    if(status)
    {
        memoryCopy(     (uint8 *)tNamePtr,
                        (uint8 *)"uFbTask",
                        strlen("uFbTask")
                  );
        return(status);
    }

#ifdef _WIN32
	if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) 
	{
		gTaskList[sTaskCount].TaskHandle 	= taskInfo.taskHandle;
		gTaskList[sTaskCount].TaskId 		= taskInfo.taskId;
		gTaskList[sTaskCount].td_name 		= taskInfo.taskName;
		gTaskList[sTaskCount].TaskKind 		= DEBUG_MPUTASK;
		gTaskList[sTaskCount].td_status 	= USR_TASK_RUN;
		sTaskCount++;
	}
#endif
	
#if NOT_USED
	/* 필요 없음 */
    memoryClear((uint8 *)&taskInfo, sizeof(strTaskInfo));

    memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"tNCFbRun", strlen("tNCFbRun"));
    taskInfo.taskPriority   = NON_CYCLIC_FB_TASK_PRI;
    taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
    taskInfo.taskStackSize  = NON_CYCLIC_FB_TASK_STACK_SIZE;
    taskInfo.taskEntry      = (func_ptr)nonCyclicFbSegRunTask;

    taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
    taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
    taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
    taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
    taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;

    status = usrTaskCreate(&taskInfo,&tId);

    if(status){
        memoryCopy(     (uint8 *)tNamePtr,
                        (uint8 *)"tNCFbRun",
                        strlen("tNCFbRun")
                  );
        return(status);
    }
#endif /* End of NOT_USED */
    
#ifdef _WIN32
	if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
		gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
		gTaskList[sTaskCount].TaskId = taskInfo.taskId;
		gTaskList[sTaskCount].td_name = taskInfo.taskName;
		gTaskList[sTaskCount].TaskKind = DEBUG_MPUTASK;
		gTaskList[sTaskCount].td_status = USR_TASK_RUN;
		sTaskCount++;
	}
#endif

#ifdef  _MBTS_PROGRAM	
	iNumDevMbts = MBTS_LoadConfig ();
	if (iNumDevMbts < 0)
	{
#if 0   
		return status;
#endif	
	}
	printf("\n");
	printf("-----------------------------------------------\n");
    printf("------ Create %d ModBus Tcp Server Task !!!!\n", iNumDevMbts);
	printf("-----------------------------------------------\n");
	
	for (index = 0; index < iNumDevMbts; index++)
	{
		memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));

		sprintf (taskInfo.taskName, "tMbts%d", index+1);

		taskInfo.taskPriority   = MBTS_TASK_PRI;
		taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
		taskInfo.taskStackSize  = MBTS_TASK_STACK_SIZE;
		taskInfo.taskEntry      = (func_ptr)MBTS_CommTask;

		taskInfo.taskArgs[0] = index;
		taskInfo.taskArgs[1] = 0;
		taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
		taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
		taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
		taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;

		status = usrTaskCreate(&taskInfo,&tId);

		if(status){
			memoryCopy(     tNamePtr,
							taskInfo.taskName,
							strlen(taskInfo.taskName)
					  );
			return(status);
		}
		
#ifdef _WIN32
		if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
			gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
			gTaskList[sTaskCount].TaskId = taskInfo.taskId;
			gTaskList[sTaskCount].td_name = taskInfo.taskName;
			gTaskList[sTaskCount].TaskKind = DEBUG_TMTCTASK;
			gTaskList[sTaskCount].td_status = USR_TASK_RUN;
			sTaskCount++;
		}
#endif

#ifdef _WIN32
		sysTaskDelay(systemClockRateGet()/10); 
#endif 

	}

#endif	

	
    memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));

    memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"uDbgTask", strlen("uDbgTask"));
    taskInfo.taskPriority   = DBG_TASK_PRI;
    taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
    taskInfo.taskStackSize  = DBG_TASK_STACK_SIZE;
    taskInfo.taskEntry      = (func_ptr)usrDbgTask;

    taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
    taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
    taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
    taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
    taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;

    status = usrTaskCreate(&taskInfo,&tId);

    if(status){
        memoryCopy(     (uint8 *)tNamePtr,
                        (uint8 *)"uDbgTask",
                        strlen("uDbgTask")
                  );
        return(status);
    }

#ifdef _WIN32
	if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
		gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
		gTaskList[sTaskCount].TaskId = taskInfo.taskId;
		gTaskList[sTaskCount].td_name = taskInfo.taskName;
		gTaskList[sTaskCount].TaskKind = DEBUG_SYSTASK;
		gTaskList[sTaskCount].td_status = USR_TASK_RUN;
		sTaskCount++;
	}
#endif
	
    memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));

    memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"uNRTask", strlen("uNRTask"));
    taskInfo.taskPriority   = DATA_RECV_TASK_PRI;
    taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
    taskInfo.taskStackSize  = DATA_RECV_TASK_STACK_SIZE;
    taskInfo.taskEntry      = (func_ptr)usrNetRTask;

    taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
    taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
    taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
    taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
    taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;

    status = usrTaskCreate(&taskInfo,&tId);

    if(status){
        memoryCopy(     (uint8 *)tNamePtr,
                        (uint8 *)"uNRTask",
                        strlen("uNRTask")
                  );
        return(status);
    }

#ifdef _WIN32
	if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
		gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
		gTaskList[sTaskCount].TaskId = taskInfo.taskId;
		gTaskList[sTaskCount].td_name = taskInfo.taskName;
		gTaskList[sTaskCount].TaskKind = DEBUG_NETTASK;
		gTaskList[sTaskCount].td_status = USR_TASK_RUN;
		sTaskCount++;
	}
#endif
	
	
    memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));

    memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"tTcpMngr", strlen("tTcpMngr"));
    taskInfo.taskPriority   = TCP_TASK_PRI;
    taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
    taskInfo.taskStackSize  = DATA_RECV_TASK_STACK_SIZE;
    taskInfo.taskEntry      = (func_ptr)netTcpSocketMngrTask;

    taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
    taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
    taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
    taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
    taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;

    status = usrTaskCreate(&taskInfo,&tId);

    if(status){
        memoryCopy(     (uint8 *)tNamePtr,
                        (uint8 *)"tTcpMngr",
                        strlen("tTcpMngr")
                  );
        return(status);
    }

#ifdef _WIN32
	if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
		gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
		gTaskList[sTaskCount].TaskId = taskInfo.taskId;
		gTaskList[sTaskCount].td_name = taskInfo.taskName;
		gTaskList[sTaskCount].TaskKind = DEBUG_NETTASK;
		gTaskList[sTaskCount].td_status = USR_TASK_RUN;
		sTaskCount++;
	}
#endif

#ifdef _WIN32
	if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
		gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
		gTaskList[sTaskCount].TaskId = taskInfo.taskId;
		gTaskList[sTaskCount].td_name = taskInfo.taskName;
		gTaskList[sTaskCount].TaskKind = DEBUG_NETTASK;
		gTaskList[sTaskCount].td_status = USR_TASK_RUN;
		sTaskCount++;
	}
#endif

    memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));

    memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"tTcpChk", strlen("tTcpChk"));
    taskInfo.taskPriority   = TCP_TASK_PRI;
    taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
    taskInfo.taskStackSize  = DATA_RECV_TASK_STACK_SIZE;
    taskInfo.taskEntry      = (func_ptr)netTcpStatusCheckTask;

    taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
    taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
    taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
    taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
    taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;

    status = usrTaskCreate(&taskInfo,&tId);

    if(status){
        memoryCopy(     (uint8 *)tNamePtr,
                        (uint8 *)"tTcpChk",
                        strlen("tTcpChk")
						);
        return(status);
    }

#ifdef _WIN32
	if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
		gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
		gTaskList[sTaskCount].TaskId = taskInfo.taskId;
		gTaskList[sTaskCount].td_name = taskInfo.taskName;
		gTaskList[sTaskCount].TaskKind = DEBUG_NETTASK;
		gTaskList[sTaskCount].td_status = USR_TASK_RUN;
		sTaskCount++;
	}
#endif


#ifdef _IPC_INTERFACE
	
	
	memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));

	memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"tIpcDataSend", strlen("tIpcDataSend"));
	taskInfo.taskPriority   = TCP_TASK_PRI;
	taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
	taskInfo.taskStackSize  = DATA_RECV_TASK_STACK_SIZE;
	taskInfo.taskEntry      = (func_ptr)ipcDataSendTask;

	taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
	taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
	taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
	taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
	taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;

	status = usrTaskCreate(&taskInfo,&tId);

	if(status){
		memoryCopy( (uint8 *)tNamePtr,
					(uint8 *)"tIpcDataSend",
					strlen("tIpcDataSend")
					);
		return(status);
	}

#ifdef _WIN32
	if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
		gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
		gTaskList[sTaskCount].TaskId = taskInfo.taskId;
		gTaskList[sTaskCount].td_name = taskInfo.taskName;
		gTaskList[sTaskCount].TaskKind = DEBUG_NETTASK;
		gTaskList[sTaskCount].td_status = USR_TASK_RUN;
		sTaskCount++;
	}
#endif



#if 111
	
	
	memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));

	memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"tprocStatusCheck", strlen("tprocStatusCheck"));
	taskInfo.taskPriority   = TCP_TASK_PRI;
	taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
	taskInfo.taskStackSize  = DATA_RECV_TASK_STACK_SIZE;
	taskInfo.taskEntry      = (func_ptr)procStatusCheckTask;

	taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
	taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
	taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
	taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
	taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;

	status = usrTaskCreate(&taskInfo,&tId);

	if(status){
		memoryCopy( (uint8 *)tNamePtr,
					(uint8 *)"tprocStatusCheck",
					strlen("tprocStatusCheck")
					);
		return(status);
	}

#ifdef _WIN32
	if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
		gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
		gTaskList[sTaskCount].TaskId = taskInfo.taskId;
		gTaskList[sTaskCount].td_name = taskInfo.taskName;
		gTaskList[sTaskCount].TaskKind = DEBUG_NETTASK;
		gTaskList[sTaskCount].td_status = USR_TASK_RUN;
		sTaskCount++;
	}
#endif

#endif 

#endif 
	
    memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));

    memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"uNCmdTask", strlen("uNCmdTask"));
    taskInfo.taskPriority   = CMD_PROC_TASK_PRI;
    taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
    taskInfo.taskStackSize  = CMD_PROC_TASK_STACK_SIZE;
    taskInfo.taskEntry      = (func_ptr)usrNCmdTask;

    taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
    taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
    taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
    taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
    taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;

    status = usrTaskCreate(&taskInfo,&tId);

    if(status){
        memoryCopy(     (uint8 *)tNamePtr,
                        (uint8 *)"uNCmdTask",
                        strlen("uNCmdTask")
                  );
        return(status);
    }

#ifdef _WIN32
	if((sTaskCount>0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
		gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
		gTaskList[sTaskCount].TaskId = taskInfo.taskId;
		gTaskList[sTaskCount].td_name = taskInfo.taskName;
		gTaskList[sTaskCount].TaskKind = DEBUG_NETTASK;
		gTaskList[sTaskCount].td_status = USR_TASK_RUN;
		sTaskCount++;
	}
#endif

	
    memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));

    memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"uNSTask", strlen("uNSTask"));
    taskInfo.taskPriority   = DATA_SEND_TASK_PRI;
    taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
    taskInfo.taskStackSize  = DATA_SEND_TASK_STACK_SIZE;
    taskInfo.taskEntry      = (func_ptr)usrNetSTask;

    taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
    taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
    taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
    taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
    taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;

    status = usrTaskCreate(&taskInfo,&tId);

    if(status){
        memoryCopy(     (uint8 *)tNamePtr,
                        (uint8 *)"uNSTask",
                        strlen("uNSTask")
                  );
        return(status);
    }

#ifdef _WIN32
	if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
		gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
		gTaskList[sTaskCount].TaskId = taskInfo.taskId;
		gTaskList[sTaskCount].td_name = taskInfo.taskName;
		gTaskList[sTaskCount].TaskKind = DEBUG_NETTASK;
		gTaskList[sTaskCount].td_status = USR_TASK_RUN;
		sTaskCount++;
	}
#endif
	
	
    memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));

    memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"uDiagTask", strlen("uDiagTask"));
    taskInfo.taskPriority   = RAS_TASK_PRI;
    taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
    taskInfo.taskStackSize  = RAS_TASK_STACK_SIZE;
    taskInfo.taskEntry      = (func_ptr)uDiagnosisTask;

    taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
    taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
    taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
    taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
    taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;

    status = usrTaskCreate(&taskInfo,&tId);

    if(status){
        memoryCopy(     (uint8 *)tNamePtr,
                        (uint8 *)"uDiagTask",
                        strlen("uDiagTask")
                  );
        return(status);
    }

#ifdef _WIN32
	if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
		gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
		gTaskList[sTaskCount].TaskId = taskInfo.taskId;
		gTaskList[sTaskCount].td_name = taskInfo.taskName;
		gTaskList[sTaskCount].TaskKind = DEBUG_SYSTASK;
		gTaskList[sTaskCount].td_status = USR_TASK_RUN;
		sTaskCount++;
	}
#endif
	
#if NOT_USED
	/* 필요 없음 */
    memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));

    memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"tXr", strlen("tXr"));
    taskInfo.taskPriority   = XR_TASK_PRI;
    taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
    taskInfo.taskStackSize  = XR_TASK_STACK_SIZE;
    taskInfo.taskEntry      = (func_ptr)xrTask;

    taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
    taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
    taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
    taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
    taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;

    status = usrTaskCreate(&taskInfo,&tId);

    if(status){
        memoryCopy(     (uint8 *)tNamePtr,
                        (uint8 *)"tXr",
                        strlen("tXr")
                  );
        return(status);
    }
#endif

#ifdef _WIN32
	if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
		gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
		gTaskList[sTaskCount].TaskId = taskInfo.taskId;
		gTaskList[sTaskCount].td_name = taskInfo.taskName;
		gTaskList[sTaskCount].TaskKind = DEBUG_SYSTASK;
		gTaskList[sTaskCount].td_status = USR_TASK_RUN;
		sTaskCount++;
	}
#endif

	
    memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));

    memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"uDualTask", strlen("uDualTask"));
    taskInfo.taskPriority   = DUAL_PROC_TASK_PRI;
    taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
    taskInfo.taskStackSize  = DUAL_PROC_TASK_STACK_SIZE;
    taskInfo.taskEntry      = (func_ptr)usrDualTask;

    taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
    taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
    taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
    taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
    taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;

    status = usrTaskCreate(&taskInfo,&tId);

    if(status){
        memoryCopy(     (uint8 *)tNamePtr,
                        (uint8 *)"uDualTask",
                        strlen("uDualTask")
                  );
        return(status);
    }

#ifdef _WIN32
	if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
		gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
		gTaskList[sTaskCount].TaskId = taskInfo.taskId;
		gTaskList[sTaskCount].td_name = taskInfo.taskName;
		gTaskList[sTaskCount].TaskKind = DEBUG_DLUTASK;
		gTaskList[sTaskCount].td_status = USR_TASK_RUN;
		sTaskCount++;
	}
#endif
	
#ifdef _USR_C_TASK_PROGRAM
	
	
	memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));
	
	memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"tUsrCProg", strlen("tUsrCProg"));
	taskInfo.taskPriority   = DUAL_COM_LINE_B_TASK_PRI;
	taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
	taskInfo.taskStackSize  = DUAL_COM_TASK_STACK_SIZE;
	taskInfo.taskEntry      = (func_ptr)usrCProgTask_1;
	
	taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
	taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
	taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
	taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
	taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;
	
	status = usrTaskCreate(&taskInfo,&tId);
	
	if(status){
	    memoryCopy(     (uint8 *)tNamePtr,
	                    (uint8 *)"tUsrCProg",
	                    strlen("tUsrCProg")
	                );
	    return(status);
	}
#endif 
	    
#ifdef _WIN32
	if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
		gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
		gTaskList[sTaskCount].TaskId = taskInfo.taskId;
		gTaskList[sTaskCount].td_name = "tUsrCProg";
		gTaskList[sTaskCount].TaskKind = DEBUG_DLUTASK;
		gTaskList[sTaskCount].td_status = USR_TASK_RUN;
		sTaskCount++;
	}
#endif

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	 
	if(sysCnfgInfo.redNetCnfgInfo.ctrlNetDualStatus == BOARD_ENABLE)
	{
		
	    memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));
	
	    memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"uDualSTask", strlen("uDualSTask"));
	    taskInfo.taskPriority   = DUAL_SEND_TASK_PRI;
	    taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
	    taskInfo.taskStackSize  = DUAL_SEND_TASK_STACK_SIZE;
	    taskInfo.taskEntry      = (func_ptr)usrDualSTask;
	
	    taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
	    taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
	    taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
	    taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
	    taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;
	
	    status = usrTaskCreate(&taskInfo,&tId);
	
	    if(status){
	        memoryCopy(     (uint8 *)tNamePtr,
	                        (uint8 *)"uDualSTask",
	                        strlen("uDualSTask")
	                  );
	        return(status);
	    }
	
#ifdef _WIN32
		if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
			gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
			gTaskList[sTaskCount].TaskId = taskInfo.taskId;
			gTaskList[sTaskCount].td_name = taskInfo.taskName;
			gTaskList[sTaskCount].TaskKind = DEBUG_DLUTASK;
			gTaskList[sTaskCount].td_status = USR_TASK_RUN;
			sTaskCount++;
		}
#endif
		
	    memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));
	
	    memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"uDualRTask", strlen("uDualRTask"));
	    taskInfo.taskPriority   = DUAL_RECV_TASK_PRI;
	    taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
	    taskInfo.taskStackSize  = DUAL_RECV_TASK_STACK_SIZE;
	    taskInfo.taskEntry      = (func_ptr)dualDataRecvTask;
	
	    taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
	    taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
	    taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
	    taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
	    taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;
	
	    status = usrTaskCreate(&taskInfo,&tId);
	
	    if(status){
	        memoryCopy(     (uint8 *)tNamePtr,
	                        (uint8 *)"uDualRTask",
	                        strlen("uDualRTask")
	                  );
	        return(status);
	    }
	    
#ifdef _WIN32
		if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
			gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
			gTaskList[sTaskCount].TaskId = taskInfo.taskId;
			gTaskList[sTaskCount].td_name = taskInfo.taskName;
			gTaskList[sTaskCount].TaskKind = DEBUG_DLUTASK;
			gTaskList[sTaskCount].td_status = USR_TASK_RUN;
			sTaskCount++;
		}
#endif

	}
	
	
    memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));

    memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"uDualChkTask", strlen("uDualChkTask"));
    taskInfo.taskPriority   = DUAL_CHK_TASK_PRI;
    taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
    taskInfo.taskStackSize  = DUAL_CHK_TASK_STACK_SIZE;
    taskInfo.taskEntry      = (func_ptr)usrDualChkTask;

    taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
    taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
    taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
    taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
    taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;

    status = usrTaskCreate(&taskInfo,&tId);

    if(status){
        memoryCopy(     (uint8 *)tNamePtr,
                        (uint8 *)"uDualChkTask",
                        strlen("uDualChkTask")
                  );
        return(status);
    }
    
#ifdef _WIN32
	if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
		gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
		gTaskList[sTaskCount].TaskId = taskInfo.taskId;
		gTaskList[sTaskCount].td_name = taskInfo.taskName;
		gTaskList[sTaskCount].TaskKind = DEBUG_DLUTASK;
		gTaskList[sTaskCount].td_status = USR_TASK_RUN;
		sTaskCount++;
	}
#endif

#endif 

    return(status);
}


uint32	dualChkAppTaskCreate(uint8 *tNamePtr)
{
    uint32				status = NO_ERROR;
    strTaskInfo     taskInfo;
    int32				tId;
	strSysConfigInfo	sysCnfgInfo;

    memoryClear ( (uint8 *) &sysCnfgInfo, sizeof(strSysConfigInfo));

	systemCnfgInfoRead( &sysCnfgInfo);

	
    memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));

    memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"uDC_ATask", strlen("uDC_ATask"));
    taskInfo.taskPriority   = DUAL_COM_LINE_A_TASK_PRI;
    taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
    taskInfo.taskStackSize  = DUAL_COM_TASK_STACK_SIZE;
    taskInfo.taskEntry      = (func_ptr)usrDC_ATask;

    taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
    taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
    taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
    taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
    taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;

    status = usrTaskCreate(&taskInfo,&tId);

    if(status){
        memoryCopy(     (uint8 *)tNamePtr,
                        (uint8 *)"uDC_ATask",
                        strlen("uDC_ATask")
                  );
        return(status);
    }
    
#ifdef _WIN32
	if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
		gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
		gTaskList[sTaskCount].TaskId = taskInfo.taskId;
		gTaskList[sTaskCount].td_name = "uDC_ATask";
		gTaskList[sTaskCount].TaskKind = DEBUG_DLUTASK;
		gTaskList[sTaskCount].td_status = USR_TASK_RUN;
		sTaskCount++;
	}
#endif
	
	
	if(sysCnfgInfo.netCnfgInfo.ctrlNetDualStatus == CTRL_NET_LINE_DUAL) { 
		
	    memoryClear( (uint8 *)&taskInfo, sizeof(strTaskInfo));
	
	    memoryCopy((uint8 *)taskInfo.taskName, (uint8 *)"uDC_BTask", strlen("uDC_BTask"));
	    taskInfo.taskPriority   = DUAL_COM_LINE_B_TASK_PRI;
	    taskInfo.taskOptions    = TASK_OPTION_DEFAULT;
	    taskInfo.taskStackSize  = DUAL_COM_TASK_STACK_SIZE;
	    taskInfo.taskEntry      = (func_ptr)usrDC_BTask;
	
	    taskInfo.taskArgs[0] = 0; taskInfo.taskArgs[1] = 0;
	    taskInfo.taskArgs[2] = 0; taskInfo.taskArgs[3] = 0;
	    taskInfo.taskArgs[4] = 0; taskInfo.taskArgs[5] = 0;
	    taskInfo.taskArgs[6] = 0; taskInfo.taskArgs[7] = 0;
	    taskInfo.taskArgs[8] = 0; taskInfo.taskArgs[9] = 0;
	
	    status = usrTaskCreate(&taskInfo,&tId);
	
	    if(status){
	        memoryCopy(     (uint8 *)tNamePtr,
	                        (uint8 *)"uDC_BTask",
	                        strlen("uDC_BTask")
	                  );
	        return(status);
	    }
	    
#ifdef _WIN32
		if((sTaskCount>=0)&&(sTaskCount<MAX_USR_TASK_NUM)) {
			gTaskList[sTaskCount].TaskHandle = taskInfo.taskHandle;
			gTaskList[sTaskCount].TaskId = taskInfo.taskId;
			gTaskList[sTaskCount].td_name = "uDC_BTask";
			gTaskList[sTaskCount].TaskKind = DEBUG_DLUTASK;
			gTaskList[sTaskCount].td_status = USR_TASK_RUN;
			sTaskCount++;
		}
#endif

	}
    return(status);
}
