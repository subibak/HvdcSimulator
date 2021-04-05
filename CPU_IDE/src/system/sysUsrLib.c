

#ifdef _WINNT
#include	"sysNTlib.h"
#if 0
#include	<windows.h>
#include	<time.h>
#endif
#endif 

#ifdef _WINCE
#include	<winsock2.h>
#include	<msgqueue.h>
#include	<time.h>
#include	<pkfuncs.h>
#include	<pc.h>
#endif

#include	<stdio.h>
#include	<ctype.h>
#include	"usrTypes.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include	"taskDefs.h"
#include	"lkv120Board.h"
#include	"msgQue.h"
#include	"usrSema.h"
#include	"usrDefBuf.h"
#include	"osDepend.h"
#include	"usrErrCode.h"
#include	"network.h"
#include	"sysConfig.h"
#include	"msgQueData.h"

#include    "netProtocol.h"
#include	"segScheduler.h"


#include	"protocolBuf.h"


#ifdef VXWORKS
#ifdef BOARD_TYPE


char    *gSerialDevName[BOARD_SERIAL_PORT_NUM] = {
        "/tyCo/0"
};

#endif
#endif 
#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD) || (BOARD_TYPE == LKV120_BOARD))
	time_t gSysClockTime = 0;
	
	static char sDays[] = {				
	    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
	static int sYdays[] = {					
	    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
	};
#elif (BOARD_TYPE == MVME2500_BOARD)

#endif

#ifdef _WIN32
#define DELAY_COUNT		1000
#define ONEKB			1024
#define MAX_THREADS		1024
#define DATA_SIZE		256	
#endif

extern int32	gDebugPortFd;

#ifdef _WIN32
extern HANDLE gHSharedMMF;
extern uint32 gShmIdSharedMMF;
extern char*  SYS_BUS_ADDR;
extern int32  gShmSize;
#endif

#ifdef _WINCE
#define	MAX_SYSTEM_QUEUE_ID		2048
uint32	gMessageQueueCount;
void*	gReadQueueId[MAX_SYSTEM_QUEUE_ID+1];
void*	gWriteQueueId[MAX_SYSTEM_QUEUE_ID+1];
extern	CRITICAL_SECTION _csIncrementLock;
#endif

#ifdef _WIN32
extern strTaskListInfo gTaskList[MAX_USR_TASK_NUM];
#endif

int32 	usrTaskCreate(strTaskInfo *, int32 *);
int32 	usrTaskDelete(int32 );
int32 	semaphoreCreate(strSemaInfo *, int32 *);
int32 	semaphoreGet(int32, int32);
#ifdef _WIN32
int32	semaphoreRelease(int32, int32);
int32	semaphoreRelease2(uint8 *, uint32, int32, int32);
#else
int32	semaphoreRelease(int32);
#endif
int32   messageQueueCreate(strMsgQueInfo *, void **);
int32   messageQueueSend(void *, int8 *, int32,int32, int32);
int32   messageQueueRecv(void *, int8 *, int32,int32, int32 *);
#ifdef _WINCE
int32	messageQueueCountGet(void *, int32 *);
#endif
void 	*memoryAlloc(int32);
void 	memoryFree(void *);
int32 	sysSwIntrSet(int32, voidfunc_ptr);
uint32  sysHwIntrSet(uint32, voidfunc_ptr, int32);
int32	systemTickGet(void);
int32	wdgTimerCreate(void);
int32	wdgTimerStart(int32 , int32 , func_ptr ,int32);
int32	inOutMultiplex( int32 , void *, void *,void *, void *);
void	inOutInit(void *);
void	inOutSet(int32 , void *);
void	inOutClear(int32 , void *);
int32	inOutCheck(int32 , void *);
int32   ioOpen(int8 *, int32 , int32 );
int32   ioRead(int32 , int8 *, int32 );
int32   ioWrite(int32 , int8 *, int32 );
int32   ioSysCtrlFunc(int32 , int32 , int32 );
void	spaceSkip(int8 **);
int32	sysMsgLogInit(void);
void	boardReboot(void);
int32   sysTaskIdListGet (int32 *, int32 );
uint32	taskNameGet(int32 , int8 **);
uint32  sysTaskInfoGet(int32 , void *);
uint32	sysPing(int8 *);
void	sysTaskDelay (int32 );
int32	sysErrNoGet(void);
int32	sysErrStringFindByVal(int32,  int8 *,int32 *, uint8 *);
void	sysInetHostAdd( int8 *, int8 *);
uint32	sysKernelTimeSlice(int32 );
uint32  sysBroadcastAddrGet(uint32, uint32  *);
uint32	sysSubNetMaskAddrGet(int8 *, int32 *);
uint32	sysSubNetMaskAddrSet(int8 *, int32 );
int32   sysMsgQueNumGet(void *);
uint32	curTimeGetInSecond(int32 *);
void	curTimeGetInString(uint32 *, int8 *);
uint32	systemTimeRead (strTimeDataInfo *);
uint32	systemTimeWrite (strTimeDataInfo *);
uint32	routeTblInfoDelete( int8 *, int8 *);
uint32	routeTblInfoAdd( int8 *, int8 *);
uint32	networkDeviceAttach(int32 , int8 *);
uint32  sysTaskStatusChangeToSuspend(int32 );
uint32  sysTaskStatusChangeToResume(int32 );

int32	systemClockRateGet(void);
void	systemClockRateSet(int32 );
void	systemClockIntrSet(func_ptr , int32 );
void	systemClockEnable(void);
void	systemClockDisable(void);

int32	systemAuxClockRateGet(void);
void	systemAuxClockRateSet(int32 );
void	systemAuxClockIntrSet(func_ptr , int32 );
void	systemAuxClockEnable(void);
void	systemAuxClockDisable(void);

#ifdef VXWORKS
uint32	vmeBusProbe(int8 *);
uint32  tasCommandSendOnVmeBus(int8  *);
#endif 

#ifdef _WIN32
uint32	exceptFilterFunc(LPEXCEPTION_POINTERS , uint8*,uint32 );
uint32  taskSuspendToListStatus(uint32);
uint32  taskResumeToListStatus(uint32);
#else
#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD) || (BOARD_TYPE == LKV120_BOARD))
	static uint32	curTimeGetInTimeData(strTimeDataInfo *);
	static time_t timeDataGetIncurTime (strTimeDataInfo );
	uint32 	systemClockTimeSet();
	static int getyday (int , int );
	static int getleaps (int );
	static int isleap (int , int );
	static int getmday (int , int );
#elif (BOARD_TYPE == MVME2500_BOARD)

#endif
#endif 

#ifdef _WIN32
uint32 periodicExecutionCheck(uint32 *, uint32, uint32 *);
#endif


extern void		boardTimeDataRead(strTimeDataInfo *);
extern uint32	boardTimeDataWrite(strTimeDataInfo *);
extern uint32	systemCnfgInfoRead( strSysConfigInfo *sysCnfgInfo);
extern uint32	stringCompare(int8 *s1, int8 *s2);

#ifdef VXWORKS

extern int32	sysBusProbe(int8 *, int32, int32, int8 *);
extern  uint32  ipAttach(int32, int8*);
#if (BOARD_TYPE == LKV120_BOARD)
extern STATUS VmeAccess (char *adrs, int mode, int length, char *pVal);
#endif
#endif 

#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))
    extern void 	sysBrdHardReset(void);
#elif (BOARD_TYPE == MVME2500_BOARD)
	extern void sysHwReboot (void);
#elif (BOARD_TYPE == LKV120_BOARD)
    extern sysHwReboot();
#endif    


#ifdef _WIN32
extern void		PortWrite (BYTE,int);
extern  BYTE	PortRead (int);
extern uint32	ping(char *,int32,uint32);
extern int64	GetLongClockRate(void);
extern char		*ctime (const time_t *tp);
extern uint32	routeAdd(int8 *, int8 *);
extern uint32	routeDelete(int8 *, int8 *);

#ifdef _WINNT
extern  typSTATUS	myMsgQCreate(typMSG_Q_ID*, uint32, uint32);
extern  typSTATUS	myMsgQDelete(typMSG_Q_ID*);
extern  typSTATUS	myMsgQOpen(typMSG_Q_ID*, uint16);
extern  typSTATUS	myMsgQClose(typMSG_Q_ID*);
extern  typSTATUS	myMsgQSend(typMSG_Q_ID*, uchar*, uint32, uint32, uchar);
extern  int32		myMsgQReceive(typMSG_Q_ID*, uchar*, uint32, uint32);
extern  typSTATUS   myMsgQCount( typMSG_Q_ID*  , uint32 * );
#endif

#ifdef _WINCE
extern  uint32	wdCreate(void);
extern  uint32	wdStart(uint32 ,uint32);
extern  uint32	ifMaskGet(int8 *, int32 *);
#endif

#endif 

extern void setErrorCode(	uint8 *, uint32, uint8 *, uint32);
#ifdef _WINCE
extern void resetFbcModule(void);
#endif
extern void debugStringDisplay(uint8 *, uint8 *, uint32,uint32);
extern void logMessageSet (int8 *, uint32, int8 *, uint32,uint32 );
extern void setErrorCodeWithVal(uint8 *, uint32, uint8 *, uint32,
						uint8 *, uint32, uint8 *, uint32, uint8 *, uint32);


int32 usrTaskCreate(strTaskInfo *task_info_ptr, int32 *ret_ptr)
{
    int32 status = NO_ERROR;
#ifdef _WIN32
	HANDLE hThread;
#endif

    if(task_info_ptr == (strTaskInfo *)NULL) {
        return (INPUT_NULL_POINT_ERR);
	}

#ifdef _WIN32

#ifdef _WINCE
	hThread = CreateThread(	NULL,
							 task_info_ptr->taskStackSize,     
							(LPTHREAD_START_ROUTINE)task_info_ptr->taskEntry,
							NULL,
							STACK_SIZE_PARAM_IS_A_RESERVATION, 
							ret_ptr);


	CeSetThreadPriority((HANDLE)hThread,
							task_info_ptr->taskPriority);


	
	
	
	CeSetThreadQuantum((HANDLE)hThread,5);

#else
	hThread = CreateThread(	NULL,
							task_info_ptr->taskStackSize,     
							(LPTHREAD_START_ROUTINE)task_info_ptr->taskEntry,
							(LPVOID)task_info_ptr->taskArgs,
							0,
							ret_ptr);

	SetThreadPriority((HANDLE)hThread,
							task_info_ptr->taskPriority);
#endif

	task_info_ptr->taskHandle = hThread;
	task_info_ptr->taskId = *ret_ptr;
	CloseHandle(hThread);
#endif 


#ifdef VXWORKS
    *ret_ptr = taskSpawn (task_info_ptr->taskName,
                        task_info_ptr->taskPriority,
                        task_info_ptr->taskOptions,
                        task_info_ptr->taskStackSize,
                        task_info_ptr->taskEntry,
                        task_info_ptr->taskArgs[0], task_info_ptr->taskArgs[1],
                        task_info_ptr->taskArgs[2], task_info_ptr->taskArgs[3],
                        task_info_ptr->taskArgs[4], task_info_ptr->taskArgs[5],
                        task_info_ptr->taskArgs[6], task_info_ptr->taskArgs[7],
                        task_info_ptr->taskArgs[8], task_info_ptr->taskArgs[9]
                       );
	task_info_ptr->taskId = *ret_ptr;
#endif
    return(status);
}


int32 usrTaskDelete(int32 taskId)
{
    int32 status = NO_ERROR;

#ifdef VXWORKS
	if(taskDelete(taskId) != NO_ERROR)
    	status = USR_TASK_DELETE_ERR;
#endif
#ifdef _WIN32
	if( SuspendThread(GetCurrentThread()) == 0xffffffff)
		status = USR_TASK_DELETE_ERR;
#endif
    return(status);
}


int32 semaphoreCreate(strSemaInfo   *sema_info_ptr, int32 *sema_id_ptr)
{
    int32   status = NO_ERROR;

    if(sema_info_ptr == (strSemaInfo *)NULL)
        return (INPUT_NULL_POINT_ERR);

#ifdef VXWORKS
    switch(sema_info_ptr->semaType) {
        case    BINARY_SEMA: 
            *sema_id_ptr = (int32)semBCreate(
                        sema_info_ptr->semaOptions,
                        sema_info_ptr->semaInitVal 
                                 );
        break;
        case    COUNT_SEMA:  
            *sema_id_ptr = (int32)semCCreate(sema_info_ptr->semaOptions,
                                      sema_info_ptr->semaInitVal
                                     );
        break;
        case    MUTEX_SEMA:  
            *sema_id_ptr = (int32)semMCreate (sema_info_ptr->semaOptions);
                        
                        
                        
        break;
        default:
            status = INVALID_SEMA_TYPE_ERR;
        break;
    }
#endif
#ifdef _WIN32
	   switch(sema_info_ptr->semaType) {
        case    BINARY_SEMA: 
			*sema_id_ptr = (int32)CreateSemaphore(	NULL,
								(LONG)sema_info_ptr->semaInitVal,
								(LONG)1,
								(LPCTSTR)&sema_info_ptr->semaName[0]);
        break;
        case    COUNT_SEMA:  
			*sema_id_ptr = (int32)CreateSemaphore(	NULL,
								(LONG)sema_info_ptr->semaInitVal,
								(LONG)sema_info_ptr->semaMaxVal,
								(LPCTSTR)&sema_info_ptr->semaName[0]);
        break;
        case    MUTEX_SEMA:  
			*sema_id_ptr = (int32)CreateMutex(	NULL,
								(BOOL)sema_info_ptr->semaInitVal,
								(LPCTSTR)&sema_info_ptr->semaName[0]);
        break;
        default:
            status = INVALID_SEMA_TYPE_ERR;
        break;
		
    }
#endif
    return(status);
}




int32 semaphoreGet(int32 sema_id, int32 time_out)
{
    int32 status = NO_ERROR;
    int32 timeTick = 0;
#ifdef VXWORKS
    SEM_ID  id;

    id = (SEM_ID) sema_id;

    if(time_out == NOWAIT)
        timeTick = NO_WAIT;
    else if(time_out == FOREVERWAIT)
        timeTick = WAIT_FOREVER;
    else timeTick = time_out;

    status = (int32)semTake(id, timeTick);
	if(status != NO_ERROR)
		status = SEMA_TIMEOUT_ERR;

#endif
#ifdef _WIN32
	DWORD dwWait;
    DWORD dwMilliseconds;

    if(time_out == NOWAIT) { dwMilliseconds = 0; }
    else if(time_out == FOREVERWAIT) { dwMilliseconds = INFINITE; }
    else { dwMilliseconds = time_out; }

	dwWait = WaitForSingleObject((HANDLE)sema_id, dwMilliseconds);
	
	switch (dwWait)
	{
		case WAIT_OBJECT_0:
			status = NO_ERROR;
			break;

		default:
		case WAIT_TIMEOUT:
			status = SEMA_TIMEOUT_ERR;
			break;

		case WAIT_ABANDONED:
			status = SEMA_TIMEOUT_ERR;
			setErrorCode(__FILE__,__LINE__,"semaphoreGet:SEMA_TIMEOUT_ERR",dwWait);
			break;
		}

#endif

    return(status);
}


#ifdef _WIN32
int32 semaphoreRelease(int32 semaType, int32 sema_id)
#else
int32 semaphoreRelease(int32 sema_id)
#endif
{
    int32 status = NO_ERROR;
#ifdef _WIN32
	uint32	previousCount = 0;
#endif

#ifdef VXWORKS
    SEM_ID  id;

    id = (SEM_ID) sema_id;

    status = semGive (id);
#endif
#ifdef _WIN32
	BOOL Result = TRUE;

    switch(semaType) {
        case    BINARY_SEMA: 
			Result = ReleaseSemaphore((HANDLE)sema_id,1, &previousCount);
			if (FALSE==Result) {status = SEMA_RELEASE_ERR;}
			else {
				if(previousCount != 0)
					setErrorCode(__FILE__,__LINE__,"semaphoreRelease",previousCount);
			}
        break;
        case    COUNT_SEMA:  
			Result = ReleaseSemaphore((HANDLE)sema_id,1,NULL);
			if (FALSE==Result) {status = SEMA_RELEASE_ERR;}
        break;
        case    MUTEX_SEMA:  
			Result = ReleaseMutex((HANDLE)sema_id);
			if (FALSE==Result) {status = SEMA_RELEASE_ERR;}
        break;
        default:
            status = INVALID_SEMA_TYPE_ERR;
        break;
    }
#endif
	if(status != NO_ERROR) {
		setErrorCodeWithVal(	__FILE__, __LINE__,
								"semaphoreRelease", status,
								"SemaID", sema_id,
								"Not Using", 0,
								"Not Using", 0);
	}

    return(status);
}

#ifdef _WIN32
int32 semaphoreRelease2(uint8 *fileName,uint32 line, int32 semaType, int32 sema_id)
{
    int32 status = NO_ERROR;
	uint32	previousCount = 0;

#ifdef VXWORKS
    SEM_ID  id;

    id = (SEM_ID) sema_id;

    status = semGive (id);
#endif
#ifdef _WIN32
	BOOL Result = TRUE;

    switch(semaType) {
        case    BINARY_SEMA: 
			Result = ReleaseSemaphore((HANDLE)sema_id,1, &previousCount);
			if (FALSE==Result) {status = SEMA_RELEASE_ERR;}
			else {
				if(previousCount != 0)
					setErrorCode(__FILE__,__LINE__,"semaphoreRelease2",previousCount);
			}
        break;
        case    COUNT_SEMA:  
			Result = ReleaseSemaphore((HANDLE)sema_id,1,NULL);
			if (FALSE==Result) {status = SEMA_RELEASE_ERR;}
        break;
        case    MUTEX_SEMA:  
			Result = ReleaseMutex((HANDLE)sema_id);
			if (FALSE==Result) {status = SEMA_RELEASE_ERR;}
        break;
        default:
            status = INVALID_SEMA_TYPE_ERR;
        break;
    }
#endif
	if(status != NO_ERROR) {
		setErrorCodeWithVal(	fileName, line,
								"semaphoreRelease2", status,
								"SemaID", sema_id,
								"SemaType", semaType,
								"Not Using", 0);
	}

    return(status);
}
#endif


int32   messageQueueCreate(strMsgQueInfo *msg_que_info_ptr, void **que_id_ptr)
{
    int32   status = NO_ERROR;

#ifdef VXWORKS
    if(msg_que_info_ptr == (strMsgQueInfo *) NULL)
        return(INPUT_NULL_POINT_ERR);
    *que_id_ptr = (void *) msgQCreate (  msg_que_info_ptr->queueCount,
                                msg_que_info_ptr->queueSize,
                                msg_que_info_ptr->queueOptions
                             );
#endif
#ifdef _WIN32
#ifdef _WINCE
	MSGQUEUEOPTIONS lpOptions;
	if(msg_que_info_ptr == (strMsgQueInfo *) NULL)
        return(INPUT_NULL_POINT_ERR);

	
	memset(&lpOptions, 0, sizeof(lpOptions));
    lpOptions.dwSize = sizeof(MSGQUEUEOPTIONS);
    
	if(msg_que_info_ptr->queueOptions==MSGQUEUE_NOPRECOMMIT) {
		lpOptions.dwFlags = MSGQUEUE_NOPRECOMMIT;
	} else {
    lpOptions.dwFlags = MSGQUEUE_ALLOW_BROKEN;
	}
    lpOptions.dwMaxMessages = msg_que_info_ptr->queueCount;
    lpOptions.cbMaxMessage = msg_que_info_ptr->queueSize;
    lpOptions.bReadAccess = TRUE;

	*que_id_ptr = (void*)CreateMsgQueue(NULL,&lpOptions);

#else
	*que_id_ptr=malloc(sizeof(typMSG_Q_ID));
	status = myMsgQCreate((typMSG_Q_ID *)*que_id_ptr,
						                 msg_que_info_ptr->queueCount,
										 msg_que_info_ptr->queueSize);
#endif

#endif
    if(*que_id_ptr == NULL)
		status = MSG_QUE_CREATE_ERR;
    return(status);
}


int32   messageQueueSend(   void *que_id, int8 *data_ptr, int32 numOfBytes,
                            int32 time_out, int32 options)
{
    int32   status = NO_ERROR;
#ifdef VXWORKS
    MSG_Q_ID    id;
#endif

#ifdef _WINCE
	MSGQUEUEOPTIONS	sOptions;
	HANDLE wr_que_id;
	DWORD	dwFlags = 0;
	DWORD	dwTimeout;
#endif

    if(que_id == NULL) {
			printf( "INPUT NULL POINT ERR [0x%x]\n",(uint32)que_id);
        return(INPUT_NULL_POINT_ERR);
	}

#ifdef VXWORKS
    id = (MSG_Q_ID) que_id;

    if( msgQSend( id, data_ptr,numOfBytes,time_out,options))
		status = MSG_QUE_SEND_ERR;
#endif

#ifdef _WIN32
#ifdef _WINCE

	sOptions.dwSize = sizeof(MSGQUEUEOPTIONS);
	sOptions.bReadAccess = FALSE;

	wr_que_id = OpenMsgQueue(GetCurrentProcess(),
							 (HANDLE)que_id,
							 &sOptions);


	if (time_out == FOREVERWAIT) {
		dwTimeout = INFINITE;
	} else {
		dwTimeout = time_out;
	}

	if(FALSE == WriteMsgQueue(wr_que_id,
							  (LPVOID)data_ptr,
							  (DWORD)numOfBytes,
							  (DWORD)time_out,
							  (DWORD)0)) {
		status = MSG_QUE_SEND_ERR;
	}

	CloseMsgQueue(wr_que_id);

#else
	
	if (time_out == FOREVERWAIT) {
		status = myMsgQSend((typMSG_Q_ID *)que_id,(uint8 *)data_ptr,numOfBytes,ISA_WAITFOREVER,ISA_MSG_PRINORMAL);
	} else {
		status = myMsgQSend((typMSG_Q_ID *)que_id,(uint8 *)data_ptr,numOfBytes,time_out+10,ISA_MSG_PRINORMAL);
	}

	if(status != NO_ERROR) status = MSG_QUE_SEND_ERR;
#endif
#endif
    return(status);
}


int32   messageQueueRecv (void *que_id, int8 *data_ptr,int32 numOfBytes,
                            int32 timeout, int32 *retVal)
{
    int32 status = NO_ERROR;
    int32 read_len;
#ifdef VXWORKS
    MSG_Q_ID    id;
#endif

#ifdef _WINCE
	DWORD	*pdwFlags = 0;
	DWORD  dwWait, dwTimeout;
#endif

    if(que_id == NULL)
        return(INPUT_NULL_POINT_ERR);

    
    read_len = 0;

#ifdef VXWORKS
    id = (MSG_Q_ID) que_id;

    read_len = msgQReceive (  id,
                            data_ptr,
                            numOfBytes,
                            timeout
                           );

    if(read_len > 0 ) 
        *retVal = read_len;
    else { 
        *retVal = 0;
		status = MSG_QUE_RECV_ERR;
    }
#endif
	
#ifdef _WIN32
#ifdef _WINCE

	if (timeout == FOREVERWAIT) {
		dwTimeout = INFINITE;
	} else {
		dwTimeout = timeout;
	}

    dwWait = WaitForSingleObject((HANDLE)que_id, dwTimeout);

	if(FALSE == ReadMsgQueue(	(HANDLE)que_id,
								(LPVOID)data_ptr,
								(DWORD)numOfBytes,
								(LPDWORD)retVal,
								(DWORD)dwTimeout,
								(DWORD*)&pdwFlags) ||
		(dwWait != WAIT_OBJECT_0)) {
		status = MSG_QUE_RECV_ERR;
	}
#else
	if (timeout == FOREVERWAIT) {
		read_len = myMsgQReceive((typMSG_Q_ID *)que_id,(uint8 *)data_ptr,numOfBytes,ISA_WAITFOREVER); 
		*retVal = read_len;
		if((read_len==-1) || (read_len<0)) {
			status = MSG_QUE_RECV_ERR;
		}
	} else {
		read_len= myMsgQReceive((typMSG_Q_ID *)que_id,(uint8 *)data_ptr,numOfBytes,timeout);  
		*retVal = read_len;
		if((read_len==-1) || (read_len<0)) {
			status = MSG_QUE_RECV_ERR;
		}
	}
#endif
#endif

    return(status);
}

#ifdef _WINCE

int32   messageQueueCountGet (void *que_id, int32 *que_count)
{
    int32 status = NO_ERROR;

	MSGQUEUEINFO	queInfo;

    if(que_id == NULL)
        return(INPUT_NULL_POINT_ERR);

	if(TRUE == GetMsgQueueInfo((HANDLE)que_id, &queInfo)) 
	{
		*que_count = queInfo.dwCurrentMessages;

	} else {
		status = MSG_QUE_INFO_READ_ERR;
	}


    return(status);
}
#endif


void *memoryAlloc(int32 num_of_bytes)
{
    void    * mem_ptr = (void *)NULL;
    mem_ptr = malloc((size_t)num_of_bytes);
    return(mem_ptr);
}


void memoryFree(void *ptr)
{
    free(ptr);
}


int32 sysSwIntrSet(int32 sig_num, void (*pHandler)())
{
    int32 status = NO_ERROR;

#ifdef VXWORKS
    if(signal(sig_num, pHandler) == SIG_ERR)
        status = SIG_SET_ERR;
#endif
    return(status);
}


uint32  sysHwIntrSet( uint32 intrNum, voidfunc_ptr intrRoutine, int32 intrArg)
{
    uint32  status = NO_ERROR;

#ifdef VXWORKS
    if(intConnect( INUM_TO_IVEC(intrNum),intrRoutine, intrArg))
        status = HW_INTERRUPT_SET_ERR;
#endif
    return(status);
}


int32 systemTickGet(void)
{
    int32 tickCounts=0;

#ifdef VXWORKS
    tickCounts = (int32)tickGet();
#endif
#ifdef _WIN32
	tickCounts = (int32)GetTickCount();			
#endif	    

    return(tickCounts);
}


int32 systemClockRateGet()
{
    int32 tick_counts=0;
#ifdef VXWORKS
    tick_counts = sysClkRateGet();
#endif
#ifdef _WIN32
	tick_counts = 1000;
#endif 
    return(tick_counts);
}



void systemClockRateSet(int32 clock_rate)
{
#ifdef VXWORKS
    sysClkRateSet(clock_rate);
#endif
#ifdef _WIN32
	timeBeginPeriod(clock_rate);
#endif 

	return;
}


void systemClockDisable(void)
{
#ifdef VXWORKS
	sysClkDisable();
#endif	

	return;
}


void systemClockEnable(void)
{
#ifdef VXWORKS
	sysClkEnable();
#endif	
	
	return;
}


void systemClockIntrSet(func_ptr intrRtn, int32 arg)
{
#ifdef VXWORKS
	sysClkConnect(intrRtn, arg);
#endif	

	return;
}


int32 systemAuxClockRateGet()
{
    int32 tick_counts=0;
#ifdef VXWORKS
#if ((BOARD_TYPE == MVME2100_BOARD)||(BOARD_TYPE == MVME3100_BOARD)||(BOARD_TYPE == MVME2500_BOARD)||(BOARD_TYPE == LKV120_BOARD))
    tick_counts = sysAuxClkRateGet();
#endif
#endif
#ifdef _WIN32
	tick_counts = (uint32)GetLongClockRate();
#endif 
    return(tick_counts);
}


void systemAuxClockRateSet(int32 clock_rate)
{
#ifdef VXWORKS
#if ((BOARD_TYPE == MVME2100_BOARD)||(BOARD_TYPE == MVME3100_BOARD)|| (BOARD_TYPE == MVME2500_BOARD)||(BOARD_TYPE == LKV120_BOARD))
    sysAuxClkRateSet(clock_rate);
#endif    
#endif

	return;
}


void systemAuxClockDisable(void)
{
#ifdef VXWORKS
#if ((BOARD_TYPE == MVME2100_BOARD)||(BOARD_TYPE == MVME3100_BOARD)|| (BOARD_TYPE == MVME2500_BOARD)||(BOARD_TYPE == LKV120_BOARD))
	sysAuxClkDisable();
#endif
#endif	

	return;
}


void systemAuxClockEnable(void)
{
#ifdef VXWORKS
#if ((BOARD_TYPE == MVME2100_BOARD)||(BOARD_TYPE == MVME3100_BOARD)|| (BOARD_TYPE == MVME2500_BOARD)||(BOARD_TYPE == LKV120_BOARD))
	sysAuxClkEnable();
#endif
#endif	

	return;
}


void systemAuxClockIntrSet(func_ptr intrRtn, int32 arg)
{
#ifdef VXWORKS
#if ((BOARD_TYPE == MVME2100_BOARD)||(BOARD_TYPE == MVME3100_BOARD)|| (BOARD_TYPE == MVME2500_BOARD)||(BOARD_TYPE == LKV120_BOARD))
	sysAuxClkConnect(intrRtn, arg);
#endif
#endif

	return;	
}


int32 wdgTimerCreate()
{
    int32 id = 1;
#ifdef VXWORKS
    id = (int32)wdCreate();
#endif
    return(id);
}


int32 wdgTimerStart(int32 wdgId, int32 timeDelay, func_ptr pRoutine,
                    int32 pRoutineArg)
{
    int32 status = NO_ERROR;
#ifdef VXWORKS
    status = wdStart(   (WDOG_ID)wdgId,
                        timeDelay,
                        (FUNCPTR)pRoutine,
                        pRoutineArg
                    );
#endif
#ifdef _WINCE
	status = wdStart(  (uint32)wdgId,
                       (uint32)WATCHDOG_SEC);
#endif
    if(status)
        status = WDG_START_ERR;

    return(status);
}


void sysTaskDelay (int32 ticks)
{

#ifdef VXWORKS
    taskDelay(ticks);
#else
	Sleep(ticks);
#endif

	return;
}


uint32 networkDeviceAttach(int32 unitNum, int8 *devName)
{
	uint32	status = NO_ERROR;
#ifdef VXWORKS
    status = (uint32)ipAttach(unitNum, devName);	
#endif
    return(status);
}


uint32	routeTblInfoAdd( int8 *destInetAddr, int8 *gateInetAddr)
{
	uint32	status = NO_ERROR;
	status = routeAdd(destInetAddr, gateInetAddr);
	return(status);
}


void spaceSkip(int8 **strPtr)
{
    while( isspace( (int) **strPtr) )
        ++*strPtr;
}


uint32	routeTblInfoDelete(int8 *destInetAddr, int8 *gateInetAddr)
{
	uint32	status = NO_ERROR;
	status = routeDelete(destInetAddr, gateInetAddr);
	return(status);
}



int32 inOutMultiplex( int32 maxFds, void *readFds, void *writeFds,
                      void *exceptFds, void *timeVal)
{
    int32   status = NO_ERROR;

    if( select(  maxFds,
				(fd_set *)readFds,
                (fd_set *)writeFds,
				(fd_set *)exceptFds,
                (struct timeval *)timeVal ) <= 0) {
        status = IO_MUX_ERR;
	}
    return(status);
}


void inOutInit(void *fdVar)
{
	FD_ZERO((fd_set *)fdVar);
}


void inOutSet(int32 fd, void *fdSet)
{
    FD_SET((uint32)fd, (fd_set *)fdSet);
}


void inOutClear(int32 fd, void *fdSet)
{
    FD_CLR((uint32)fd, (fd_set *)fdSet);
}


int32 inOutCheck(int32 fd, void *fdSet)
{
    return(FD_ISSET((uint32)fd, (fd_set *)fdSet));
}


int32   ioOpen(int8 *ioName, int32  flags, int32 mode)
{
    int32   ioFd=0;
#ifdef _WINCE
	ioFd =	(int32)CreateFile (	(const unsigned short *)ioName,
							flags, mode,           
							NULL, OPEN_EXISTING,
							0, 	NULL);
#endif

#ifdef _WINNT
	ioFd =	(int32)CreateFile (	(const char *)ioName,
							flags, mode,           
							NULL, OPEN_EXISTING,
							0, 	NULL);
#endif
	
#ifdef VXWORKS
	  ioFd = open(ioName, flags, mode);
#endif
    return(ioFd);
}


int32   ioRead(int32 fd, int8 *buf, int32 maxBytes)
{
    int32   retVal=0;
#ifdef _WIN32
	int i = 0;
	while(i<maxBytes)
	{
#ifdef _WINNT
		buf[i] = getchar();
#else
		buf[i] = PortRead(fd);
		if((i==0)&&(buf[i]=='\b')) continue;

		PortWrite (buf[i],fd);	
		if(buf[i]=='\b'){
			PortWrite(' ',fd);
			PortWrite ('\b',fd);
			i--;
			continue;
		}
		else if(buf[i]=='\r')    PortWrite('\n',fd);
#endif
		if(buf[i]==0x0D || buf[i]==0x0A) {
			buf[i] = '\0';
			break;
		}
		if(i==(maxBytes-1)) {
			buf[i] = '\0';	
			fdprintf(fd,"\nCommand shoud be in %d characters.\n", maxBytes);
		}

		i++;
	}
#endif

#ifdef VXWORKS
    retVal = fioRdString(fd, buf, maxBytes);
#endif
    return(retVal);
}


int32   ioWrite(int32 fd, int8 *buf, int32 maxBytes)
{
    int32   retVal=0;

#ifndef  VXWORKS
	int i;
	for(i=0;i<maxBytes;i++) {
#ifdef  _WINCE
		PortWrite (buf[i],fd);	
#else
		putchar(buf[i]);
#endif		
	}

#else
    retVal = write(fd, buf, maxBytes);
#endif
    return(retVal);
}


int32   ioSysCtrlFunc(int32 fd, int32 ctrlFunc , int32 options)
{
    int32   retVal=0;
#ifdef VXWORKS
    retVal = ioctl(fd, ctrlFunc, options);
#endif
#ifdef _WIN32
    retVal = ioctlsocket(fd, ctrlFunc, options);
#endif
    return(retVal);
}

#ifdef VXWORKS

int32 sysMsgLogInit()
{
    int32   status = NO_ERROR;
    int32   logFd=0;
#ifdef INCLUDE_LOGGING

    

    if(logFdDelete(consoleFd))
        return(LOG_FD_DEL_ERR);

#endif 

    
    logFd = ioOpen(gSerialDevName[MSG_LOG_PORT_ID-1], O_RDWR, 0);

    if(logFd < 0)
        return(IO_OPEN_ERR);


    

    

    if(ioSysCtrlFunc(logFd, MYIOBAUDRATE, MYCONSOLE_BAUD_RATE) < 0)
        return(IO_CTRL_ERR);

    if(ioSysCtrlFunc(logFd, MYIOSETOPTIONS,
                    MYOPT_ECHO | MYOPT_CRMOD | MYOPT_TANDEM | MYOPT_7_BIT) < 0)
        return(IO_CTRL_ERR);

    

    if(ioSysCtrlFunc (logFd, MYIOFLUSH, 0) < 0)
        return(IO_CTRL_ERR);

    
    if(ioSysCtrlFunc(logFd, MYIOSETOPTIONS, MYOPT_TERMINAL) < 0)
        return(IO_CTRL_ERR);

    
    if(logInit(logFd, MAX_SYS_LOG_MSG_NUM))
        status = LOG_INIT_ERR;

    return(status);
}
#endif 


#ifdef VXWORKS
#define sysMsgLogSet    logMsg
#endif


void boardReboot()
{
	printf("Board Reboot!!!\n");
#if ((BOARD_TYPE == MVME2100_BOARD) || (BOARD_TYPE == MVME2500_BOARD))
	reboot(BOOT_COLD);
#elif ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))
	sysBrdHardReset ();
#elif ((BOARD_TYPE == LKV120_BOARD))
	sysHwReboot();
#elif (BOARD_TYPE == X86_PC_BOARD)
 #ifdef _WINNT
	ExitProcess(0);
 #endif 
 #ifdef _WINCE

	resetFbcModule();
	_outp(0xCF9, 0x06);    
 #endif 	
#endif    
}


uint32   sysTaskStatusChangeToSuspend(int32 taskId)
{
	uint32	status = NO_ERROR;
	int32	retVal = NO_ERROR;

#ifdef  VXWORKS
	retVal = taskSuspend(taskId);
	if(retVal != NO_ERROR)
		status = TASK_STATUS_CHANGE_ERR;
#endif	
#ifdef _WIN32
	
	retVal = SuspendThread( (HANDLE) taskId);
	if(retVal == 0xFFFFFFFF )
		status = TASK_STATUS_CHANGE_ERR;
	else{
		taskSuspendToListStatus((uint32)taskId);
	}

#endif

	return(status);
}


uint32   sysTaskStatusChangeToResume(int32 taskId)
{
	uint32	status = NO_ERROR;
	int32	retVal = NO_ERROR;

#ifdef  VXWORKS
	retVal = taskResume(taskId);
	if(retVal != NO_ERROR)
		status = TASK_STATUS_CHANGE_ERR;
#endif	
#ifdef _WIN32
	
	retVal = ResumeThread( (HANDLE) taskId);
	if(retVal == 0xFFFFFFFF )
		status = TASK_STATUS_CHANGE_ERR;
	else{
		taskResumeToListStatus((uint32)taskId);
	}
#endif

	return(status);
}


uint32   sysTaskSuspendedCheck(int32 taskId)
{
	uint32	status = NO_ERROR;
	int32	retVal = NO_ERROR;

#ifdef  VXWORKS
	retVal = taskIsSuspended(taskId);
	if(retVal != NO_ERROR)
		status = TASK_SUSPENDED_ERR;
#endif	

	return(status);
}


int32   sysTaskIdListGet (int32 *idPtr, int32 maxTaskNum)
{
    int32 idNum=0;
#ifdef VXWORKS
    idNum = taskIdListGet(idPtr, maxTaskNum);
#endif

#ifdef _WIN32
	uint32 i = 0; 
	if(maxTaskNum == MAX_USR_TASK_NUM) {
		for(i=0;i<MAX_USR_TASK_NUM;i++) 
			idPtr[i] = gTaskList[i].TaskId;

		idNum = MAX_TASK_LIST;
	}
	else idNum = 0;
#endif
    return(idNum);
}


uint32 taskNameGet(int32 taskId, int8 **namePtr)
{
    uint32 status = NO_ERROR;
#ifdef VXWORKS
    *namePtr = taskName(taskId);

    if(*namePtr == (int8 *)NULL)	{
        status = TASK_NAME_GET_ERR;
	}
#endif
    return(status);
}


uint32   sysTaskInfoGet(int32 taskId, void *infoPtr)
{
    uint32   status = NO_ERROR;
#ifdef  VXWORKS
    TASK_DESC   *taskInfoPtr = (TASK_DESC *)infoPtr;

    if(taskInfoGet(taskId, taskInfoPtr)) {
        status = TASK_INFO_GET_ERR;
	}
#else
	uint32 i = 0, taskIdFlag =0; 
    strTaskListInfo   *taskInfoPtr = (strTaskListInfo *)infoPtr;

	for(i=0; i< MAX_TASK_LIST;i++) {
		if(gTaskList[i].TaskId == taskId) {
			taskInfoPtr->td_status = gTaskList[i].td_status;
			taskInfoPtr->td_name = gTaskList[i].td_name;
			taskIdFlag = 1;
			break;
		}
	}

	if(taskIdFlag != 1) {
		taskInfoPtr->td_status = 0; 
        status = TASK_INFO_GET_ERR;
	}
#endif
	
	return(status);
}

#ifdef _WIN32
uint32   usrTaskInfoStatusGet(int32 taskId, void *infoPtr)
{
    uint32   status = NO_ERROR;
	uint32 i = 0, taskIdFlag =0; 
    strTaskListInfo   *taskInfoPtr = (strTaskListInfo *)infoPtr;

	for(i=0; i< MAX_TASK_LIST;i++) {
		if(gTaskList[i].TaskId == taskId) {
			taskInfoPtr->td_status = gTaskList[i].td_status;
			taskInfoPtr->td_name = gTaskList[i].td_name;
			taskIdFlag = 1;
			break;
		}
	}

	if(taskIdFlag != 1) {
		taskInfoPtr->td_status = 0; 
		taskInfoPtr->td_name = "tSysTask";
	}
    return(status);
}
#endif


uint32 sysPing(int8 *ipAddr)
{
    uint32  status = NO_ERROR;

    int32   numPackets; 
    uint32  options;

    numPackets = 5;
    options = 0;

    status = ping(ipAddr,numPackets, options);
    return(status);
}


int32 sysErrNoGet(void)
{
    int32 eNum = 0;
#ifdef VXWORKS
    eNum = errnoGet();
#endif
    return(eNum);
}


int32 sysErrStringFindByVal(int32 eNum, int8 *errString,
                              int32 *retErrNum, uint8 *retErrType)
{
    int32 status = 0;
#ifdef VXWORKS
    if(statSymTbl == NULL)
        status = -1;
    else
        symFindByValue( statSymTbl, eNum, errString, retErrNum, retErrType);
#endif
    return(status);
}


void sysInetHostAdd( int8 *hName, int8 *inetAddr)
{
#ifdef VXWORKS
    hostAdd (hName, inetAddr);
#endif
}


uint32 sysKernelTimeSlice(int32 ticks)
{
    uint32 status = NO_ERROR;

#ifdef VXWORKS
    if(kernelTimeSlice(ticks))
        status = TIMESLICE_ENABLE_ERR;
#endif
    return(status);
}


uint32  sysBroadcastAddrGet(uint32 lineKind, uint32  *addrPtr)
{
    uint32  status = NO_ERROR;
    strSysConfigInfo	sysCnfgInfo;
    uint32	ipAddr, netAddr;
    
    systemCnfgInfoRead(&sysCnfgInfo);
    
    
    if(lineKind == NET_LINE_LINK_A) {	
    	ipAddr = sysCnfgInfo.netCnfgInfo.myIpAddr[0];
	}  	
    else if(lineKind == NET_LINE_LINK_B) {	
    	ipAddr = sysCnfgInfo.netCnfgInfo.myIpAddr[1];
	}
	else return(BROADCAST_ADDR_GET_ERR);
	
	
#ifdef _WIN32
	netAddr = (ipAddr&0x000000FF);
	if((netAddr > 0) && (netAddr <= MAX_IP_A_CLASS_NUM)) {
		*addrPtr = (ipAddr & 0x000000FF) | (0xFFFFFF00);
	}
	else if((netAddr > MAX_IP_A_CLASS_NUM) && (netAddr <= MAX_IP_B_CLASS_NUM)) {
		*addrPtr = (ipAddr & 0x0000FFFF) | (0xFFFF0000);
	}
	else if((netAddr > MAX_IP_B_CLASS_NUM) && (netAddr <= MAX_IP_C_CLASS_NUM)) {
		*addrPtr = (ipAddr & 0x00FFFFFF) | (0xFF000000);
	}
	else {
        status = IP_ADDR_INVALID_ERR;
	}
#endif
#ifdef VXWORKS
	netAddr = (ipAddr&0xff000000) >> 24;
	
	if((netAddr > 0) && (netAddr <= MAX_IP_A_CLASS_NUM))
		*addrPtr = (ipAddr & 0xff000000) | (0xffffff);	
	else if((netAddr > MAX_IP_A_CLASS_NUM) && (netAddr <= MAX_IP_B_CLASS_NUM))
		*addrPtr = (ipAddr & 0xffff0000) | (0xffff);	
	else if((netAddr > MAX_IP_B_CLASS_NUM) && (netAddr <= MAX_IP_C_CLASS_NUM))
		*addrPtr = (ipAddr & 0xffffff00) | (0xff);	
	else
        status = IP_ADDR_INVALID_ERR;
#endif

    return(status);
}


uint32 sysSubNetMaskAddrGet(int8 *ifName, int32 *addrPtr)
{
    uint32  status = NO_ERROR;

#ifdef  VXWORKS
    if(ifMaskGet(ifName, addrPtr)) status = SUB_NET_MASK_GET_ERR;
#endif
    return(status);
}


uint32 sysSubNetMaskAddrSet(int8 *ifName, int32 subAddr)
{
    uint32  status = NO_ERROR;
#ifdef  VXWORKS
    status = ifMaskSet(ifName, subAddr);
#endif
    return(status);
}


int32   sysMsgQueNumGet(void *msgQueIdPtr)
{
    int32  msgNum = 0;

#ifdef VXWORKS
    msgNum = msgQNumMsgs( (MSG_Q_ID)msgQueIdPtr );
#endif
#ifdef _WINCE
	MSGQUEUEINFO	queInfo;

    if(msgQueIdPtr == NULL)
        return(INPUT_NULL_POINT_ERR);

	if(TRUE == GetMsgQueueInfo((HANDLE)msgQueIdPtr, &queInfo)) 
	{
		msgNum = queInfo.dwCurrentMessages;

	} else {
		msgNum = 0;
	}
#endif
#ifdef _WINNT
    if(myMsgQCount( msgQueIdPtr, &msgNum)!=NO_ERROR)
		msgNum = 0;		
#endif
    return(msgNum);
}


uint32	curTimeGetInSecond(int32 *secPtr)
{
	uint32	status = NO_ERROR;

#ifdef VXWORKS
	if(time( (time_t *) secPtr) == (time_t)-1)
		status = TIME_SYSCALL_ERR;
#endif	

#ifdef _WINNT
	time( (time_t *) secPtr);
#endif

	return(status);
}


void curTimeGetInString(uint32 *secPtr, int8 *strPtr)
{
    sprintf((char *)strPtr, "%s",ctime((time_t *)secPtr));
}


uint32	systemTimeRead (strTimeDataInfo *dataPtr)
{
	uint32	status = NO_ERROR;
	
#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD) || (BOARD_TYPE == LKV120_BOARD))
	status = curTimeGetInTimeData(dataPtr); 
#elif (BOARD_TYPE == MVME2500_BOARD)

#else 
	boardTimeDataRead(dataPtr);    
#endif
    return(status);
}


uint32	systemTimeWrite (strTimeDataInfo *dataPtr)
{
	uint32	status = NO_ERROR;

	status = boardTimeDataWrite(dataPtr);

#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD) || (BOARD_TYPE == LKV120_BOARD))
	if(status == NO_ERROR) {
		status = systemClockTimeSet();
	}
#elif (BOARD_TYPE == MVME2500_BOARD)

#endif
    return(status);
}

#ifdef VXWORKS

uint32 vmeBusProbe(int8 *vmeAddr)
{
	int8 val = 0;
	uint32	status = 1;
	status = (uint32)sysBusProbe( 	vmeAddr, 
    								0,			
                    				1,			
									&val
								);
#if (BOARD_TYPE == LKV120_BOARD)
#if 0
  
	status = (uint32)sysBusProbe( 	vmeAddr, 
    								1,			
                    				1,			
									&val
								);
#else
  
	status = (uint32)VmeAccess( 	vmeAddr, 
    								1,			
                    				1,			
									&val
								);
#endif
#endif 
	return( status);
}


uint32  tasCommandSendOnVmeBus(int8  *adrs)
{
    uint32  status = NO_ERROR;
#if !(BOARD_TYPE == LKV120_BOARD)
    if(!sysBusTas(adrs))    
                            
        status = BUS_ACCESS_PERMIT_ERR;
#endif
    return(status);
}
#endif 

#ifdef _WIN32
uint32 exceptFilterFunc(LPEXCEPTION_POINTERS pExceptInfo, uint8* fileName, uint32 lineNum)
{
	uint32 nRetCode = 0;
	int nCode;
	uint32 exceptionAddr = 0; 
	uint8 errString[32];

	if(pExceptInfo != NULL)  {
		 nCode = pExceptInfo->ExceptionRecord->ExceptionCode;
		switch(nCode){
		case EXCEPTION_DATATYPE_MISALIGNMENT:
			nRetCode = 1;
			sprintf(errString,"DATATYPE_MISALIG");
			break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			nRetCode = 1;
			sprintf(errString,"ARRAY_BOUNDS_EXC");
			break;
		case EXCEPTION_FLT_DENORMAL_OPERAND:
			nRetCode = 1;
			sprintf(errString,"FLT_DENORMAL_OPE");
			break;
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			nRetCode = 1;
			sprintf(errString,"FLT_DIVIDE_BY_ZER");
			break;
		case EXCEPTION_FLT_INEXACT_RESULT:
			nRetCode = 1;
			sprintf(errString,"FLT_INEXACT_RESUL");
			break;
		case EXCEPTION_FLT_INVALID_OPERATION:
			nRetCode = 1;
			sprintf(errString,"FLT_INVALID_OPERA");
			break;
		case EXCEPTION_FLT_OVERFLOW:
			nRetCode = 1;
			sprintf(errString,"FLT_OVERFLOW");
			break;

		case EXCEPTION_FLT_UNDERFLOW:
			nRetCode = 1;
			sprintf(errString,"FLT_UNDERFLOW");
			break;
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			nRetCode = 1;
			sprintf(errString,"INT_DIVIDE_BY_ZERO");
			break;
		case EXCEPTION_INT_OVERFLOW :
			nRetCode = 1;
			sprintf(errString,"INT_OVERFLOW");
			break;
		case EXCEPTION_PRIV_INSTRUCTION:
			nRetCode = 1;
			sprintf(errString,"PRIV_INSTRUCTION");
			break;
		case EXCEPTION_IN_PAGE_ERROR:
			nRetCode = 1;
			sprintf(errString,"IN_PAGE_ERROR");
			break;
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			nRetCode = 1;
			sprintf(errString,"ILLEGAL_INSTRUCTI");
			break;
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			nRetCode = 1;
			sprintf(errString,"NONCONTINUABLE_EX");
			break;
		case EXCEPTION_INVALID_DISPOSITION:
			nRetCode = 1;
			sprintf(errString,"INVALID_DISPOSITI");
			break;
		case EXCEPTION_GUARD_PAGE:
			nRetCode = 1;
			sprintf(errString,"GUARD_PAGE");
			break;
		case EXCEPTION_INVALID_HANDLE:
			nRetCode = 1;
			sprintf(errString,"INVALID_HANDLE");
			break;

		case EXCEPTION_ACCESS_VIOLATION:
			nRetCode = 1;
			sprintf(errString,"ACCESS_VIOLATION");
			break;
		case EXCEPTION_STACK_OVERFLOW:
			nRetCode = 1;
			sprintf(errString,"STACK_OVERFLOW");
			break;

			default:
				
				sprintf(errString, "EXCT:0x%08x", nCode);
				break;
		}

		logMessageSet(fileName,lineNum,errString,SYSTEM_EXCEPTION_ERR,0);
	}
	else{
		debugStringDisplay("exceptFilterFunc","Memory Address NULL Err",0,0);
	}

	return nRetCode;
}

uint32   taskSuspendToListStatus(uint32 threadId)
{
	uint32 status = NO_ERROR;
	uint32 i, retTaskNum;
	uint32 taskIdFlag = 0;
	uint32 threadIdList[MAX_USR_TASK_NUM];
	
	retTaskNum = sysTaskIdListGet( (int32 *)threadIdList, MAX_USR_TASK_NUM);

	for(i=0;i<retTaskNum;i++) {
		if ((int32)threadId == (int32)threadIdList[i]) {
			taskIdFlag = 1;
			break;
		}
	}

	if(taskIdFlag == 1) {
		gTaskList[i].td_status = TASK_SUSPEND;
	}
	return(status);
}


uint32   taskResumeToListStatus(uint32 threadId)
{
	uint32 status = NO_ERROR;
	uint32 i, retTaskNum;
	uint32 taskIdFlag = 0;
	uint32 threadIdList[MAX_USR_TASK_NUM];
	
	retTaskNum = sysTaskIdListGet( (int32 *)threadIdList, MAX_USR_TASK_NUM);

	for(i=0;i<retTaskNum;i++) {
		if ((int32)threadId == (int32)threadIdList[i]) {
			taskIdFlag = 1;
			break;
		}
	}

	if(taskIdFlag == 1) {
		gTaskList[i].td_status = 0;
	}
	return(status);
}
#endif 


#ifdef VXWORKS
#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD) || (BOARD_TYPE == LKV120_BOARD))

static int getmday (int month, int year)
{
	int retVal, addVal;
	
	if(		(month == 1) 
		&& 	(		((year % 400) == 0)
			   || 	(((year % 4) == 0) && ((year % 100) != 0))
			)
	) {
		addVal = 1;
	}
	else {
		addVal = 0;
	}
	
	retVal = sDays[month] + addVal;
	
    return retVal;
}
static int isleap (int months, int year)
{
	int retVal;
	
	if(		(months == 2) 
		&&	(		((year % 400) == 0)
				|| 	(((year % 4) == 0) && ((year % 100) != 0))
			)
	) {
		retVal = 1;
	}
	else {
		retVal = 0;
	}
	
    return retVal;
}

static int getleaps (int year)
{
    int y1, y2, y3, retVal;
    year--;
    y1 = year - 1968;
    y2 = year - 1900;
    y3 = year - 1600;
    
    retVal = (y1/4) - (y2/100) + (y3/400);
    
    return retVal;
}
static int getyday (int month, int year)
{
	int retVal, addVal;
	
	if(		(month >= 2) 
		&&	(		((year % 400) == 0) 
				|| 	(((year % 4) == 0) && ((year % 100) != 0))
			)
	) {
		addVal = 1;
	}
	else {
		addVal = 0;
	}
		
	retVal = sYdays[month] + addVal;
	
    return retVal;
}


static uint32	curTimeGetInTimeData(strTimeDataInfo *timePtr)
{
	uint32	status = NO_ERROR;
	int32 clockTime;
	int year, mon, day, curMonthDays;
	time_t tick;
	time_t	second, minute, hour, date, days;

	status = curTimeGetInSecond(&clockTime);
	if(status != NO_ERROR){
		setErrorCode(	(uint8 *)__FILE__,
    					(uint32)__LINE__,
        				(uint8 *)"timeOfDayGet",
	        			 SYSTEM_TIME_READ_ERR
						);
	}
    tick = gSysClockTime + clockTime ; 

	second = tick;
    timePtr->second = second % 60;
    minute = second/60;
    timePtr->minute = minute % 60;
    hour = minute/60;
    timePtr->hour = hour % 24;
    days = hour/24;
	
    timePtr->day = (days + 4) % 7;

    year = 1970;
    mon  = 0;

	
	curMonthDays = getmday (mon, year);
    while (days >= ((time_t)curMonthDays)) 
	{
		days -= curMonthDays;
		++mon;
		if (mon >= 12) {
			mon = 0;
			year++;
		}
		curMonthDays = getmday (mon, year);
    }

    timePtr->year   = year;  
    timePtr->month  = mon + 1; 
    timePtr->date   = days + 1;
	timePtr->mSec   = 0;
	return(status);
}


static time_t timeDataGetIncurTime (strTimeDataInfo timeData)
{
    int y, m, d, s;
    long long ts, temp = 0xffffffff;
    uint32 retval;

    m = (timeData.year-1900) * 12 + (timeData.month-1); 
    y = m / 12 + 1900;
    m = m % 12;
    d = 365 * (y - 1970) + 
    	getleaps (y) +
		getyday (m, y) +
		(timeData.date - 1);
    s = 3600 * timeData.hour + 60 * timeData.minute + timeData.second;
    ts = (long long) d * 86400 + (long long) s;	
    if (	(ts < 0)
    	||	(ts > (long long) temp)	) 
    {
		return 0;
    }

    retval = (time_t) ts; 
    return retval;
}


uint32 systemClockTimeSet()
{
	uint32 status = NO_ERROR;
	strTimeDataInfo systemTime;
	time_t  st,ct,retTime=0;

	status = curTimeGetInSecond((int32*)&ct);	
	if(status != NO_ERROR){
		status = TIME_SYSCALL_ERR;
	}
	
	boardTimeDataRead(&systemTime); 

	
	st = timeDataGetIncurTime(systemTime);	
	
	retTime = st-ct;
	if((retTime<0)&&(st==0)) {
		gSysClockTime = 0;
		status = TIME_SYSCALL_ERR;
	}
	else {
		gSysClockTime = retTime;
	}

	return(status);
}


time_t getSysClockTime (void)
{
	return gSysClockTime;
}

#elif (BOARD_TYPE == MVME2500_BOARD)

#endif
#endif



uint32	checkPMCModule(uint32 pmcModuleId, int32 unitNum)
{
	uint32	status = NO_ERROR;
#if ((BOARD_TYPE == MVME2100_BOARD)||(BOARD_TYPE == MVME3100_BOARD)||(BOARD_TYPE == MVME4100_BOARD)||(BOARD_TYPE == MVME2500_BOARD))
	int32	busNo, slotNo, funcNo;
	
	if( pmcModuleId == PMC661J_END)
		status = pciFindDevice(	(uint32)PMC661J_VENDOR_ID,
								(uint32)PMC661J_DEVICE_ID,
								(uint32)unitNum,
								(int32 *)&busNo,
								(int32 *)&slotNo,
								(int32 *)&funcNo
							   );
	else if(pmcModuleId == PMC682E_END)
		status = pciFindDevice(	(uint32)PMC682E_VENDOR_ID,
								(uint32)PMC682E_DEVICE_ID,
								(uint32)unitNum,
								(int32 *)&busNo,
								(int32 *)&slotNo,
								(int32 *)&funcNo
							   );
	
	else if(pmcModuleId == PMC676TX_END) {
		status = pciFindDevice(	(uint32)PMC676TX_VENDOR_ID,
								(uint32)PMC676RCTX_DEVICE_ID,
								(uint32)0, 
								(int32 *)&busNo,
								(int32 *)&slotNo,
								(int32 *)&funcNo
							   );
		if(status != NO_ERROR)
			status = pciFindDevice(	(uint32)PMC676TX_VENDOR_ID,
									(uint32)PMC676TX_DEVICE_ID,
									(uint32)0, 
									(int32 *)&busNo,
									(int32 *)&slotNo,
									(int32 *)&funcNo
								   );
	}
#endif
		
	return(status);
}

#ifdef _WIN32
uint32 periodicExecutionCheck(uint32 *prevTickCountPtr, uint32 msecPeriod, uint32 *deltaTickPtr) 
{
	uint32 status = NO_ERROR;
	uint32 deltaTick = 0;
	uint32 curTickCount;
	uint32 highLimitDeltaTick, lowLimitDeltaTick;

	curTickCount = GetTickCount()&0x7FFFFFFF;

	if(*prevTickCountPtr != 0x80000000) {
	
		if(curTickCount < *prevTickCountPtr)
			deltaTick = 0x80000000 - (*prevTickCountPtr - curTickCount);
		else
			deltaTick = curTickCount - *prevTickCountPtr;

		highLimitDeltaTick = (uint32)(msecPeriod*1.5);
		lowLimitDeltaTick = msecPeriod>>1;

		if(deltaTick > highLimitDeltaTick)
			status = PERIODIC_EXEC_OVERTIME_ERR;
		else if(deltaTick < lowLimitDeltaTick)
			status = PERIODIC_EXEC_UNDERTIME_ERR;
		else
			status = NO_ERROR;
	}

	*prevTickCountPtr = curTickCount;
	*deltaTickPtr = deltaTick;

	return (status);
}



size_t getSharedMemorySize ()
{
	size_t	memSize = 0;
	memSize = 0x400000;	
	return (memSize);
}


uint32 createSharedMemory (size_t shmSize)
{
	uint32 status = NO_ERROR;
	uint32	shmId = 0;
	HANDLE	hMapFile;

	gHSharedMMF = CreateFile (	FLASH_FILE,
								GENERIC_READ | GENERIC_WRITE,
								0, 
								NULL,
								OPEN_ALWAYS, 
								FILE_ATTRIBUTE_NORMAL,
								NULL
								);

	if (gHSharedMMF == INVALID_HANDLE_VALUE)
	{
		
		status = SHARED_MEM_MMF_ERR;
		printf("*** FAIL - CreateFile\n");

		shmId = -1;
		return (shmId);	
	}

	hMapFile = CreateFileMapping (	gHSharedMMF,
									NULL,
									PAGE_READWRITE,
									0,
									shmSize,
									FLASH_FILE);

	if (hMapFile == NULL)
	{
		status = SHARED_MEM_MMF_ERR;
		printf("*** FAIL - CreateFile\n");

		shmId = -1;
	}
	else if ((hMapFile != NULL) && (GetLastError () == ERROR_ALREADY_EXISTS))
	{
		CloseHandle (hMapFile);
		hMapFile = NULL;

		shmId = -1;
	}
	else
	{
		shmId = (int)hMapFile;
	}

	return (shmId);
}



uint32 destroyBackupSharedMMF ()
{
	uint32 status = NO_ERROR;
	uint32 retVal = 0;

	retVal = UnmapViewOfFile ((LPCVOID)SYS_BUS_ADDR);

	if (retVal == 0){
		status = SHARED_MEM_MMF_ERR;
		printf("*** FAIL - detach shared memory [gSoeInfoTbl]\n");
	}
	else
	{
		retVal = CloseHandle ((HANDLE)gShmIdSharedMMF);

		if (retVal == 0){
			status = SHARED_MEM_MMF_ERR;
			printf("*** FAIL - remove shared memory [gSoeInfoTbl]\n");
		}
		else{

			retVal = CloseHandle ((HANDLE)gHSharedMMF);

			if (retVal == 0){	
				status = SHARED_MEM_MMF_ERR;
				printf("*** FAIL - remove shared memory [gSoeInfoTbl]\n");
			}
			else{
				printf ("*** SUCCESS - remove shared memory [gSoeInfoTbl]\n");
			}
		}
	}

	return (status);
}



uint32 initBackupSharedMMF ()
{
	uint32 status = NO_ERROR;
	size_t	shmSize;
	

	shmSize = getSharedMemorySize ();

	if (shmSize != 0)
	{
		gShmIdSharedMMF = createSharedMemory (shmSize);

		if(gShmIdSharedMMF == -1){
			status = SHARED_MEM_MMF_ERR;
			return (status);
		}

		if ((SYS_BUS_ADDR = (char *)MapViewOfFile ((LPVOID)gShmIdSharedMMF,
														FILE_MAP_ALL_ACCESS,
														0,
														0,
														0)) == NULL)
		{
			status = SHARED_MEM_MMF_ERR;		
			return (status);
		}
		{
			gShmSize = shmSize;
		}
	}
	else
	{
		status = SHARED_MEM_MMF_ERR;
		printf("INVALID - shared memory size\n");
		return (status);
	}

	return status;
}
#endif
