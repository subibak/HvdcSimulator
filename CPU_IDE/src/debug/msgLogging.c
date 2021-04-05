
#include	<stdio.h>
#include	<string.h>
#include    "usrTypes.h"
#include	"sysDefs.h"
#include    "pcsDefs.h"
#include	"sysConfig.h"
#include	"network.h"
#include	"usrQueData.h"
#include	"msgQueData.h"
#include	"segScheduler.h"


#include	"debugInfo.h"
#include	"usrErrCode.h"
#include	"lkv120Board.h"
#include	"usrSema.h"
#include	"osDepend.h"

#include	"usrDefBuf.h"



#define MAX_FATAL_ERR_NUM      128



#define NOW_USING_RAM       0xad


static uint32   sFatalErrMsgSemaId;


extern int32 gDebugPortFd;
extern uint32 *gDualDataBusPtr;


uint32  fatalErrorLogInit(void);
void fatalErrMsgClear (uint32, uint32);
void logMessageSet (int8 *, uint32, int8 *, uint32 , uint32);
static void fatalErrMsgRefresh(int8 *, uint32,int8 *, uint32, uint32);
void fatalErrMsgDisplay(uint32, uint32);
void fatalErrQueCountDisplay(void);

uint32  sysHistoryFatalErrMsgGet(strSysHistoryInfo*);


extern  void fatalErrStringDisplay(int8 *, uint32,uint8 *, uint32 , uint32, uint8 *, uint32 );
extern void errorStringDisplay(uint8 *,uint32,uint8 *,uint32);
extern int32 	semaphoreCreate(strSemaInfo *, int32 *);
extern int32 	semaphoreGet(int32 , int32 );
#ifdef VXWORKS
extern int32	semaphoreRelease(int32);
#else
extern int32	semaphoreRelease(int32, int32);
extern int32	semaphoreRelease2(uint8 *, uint32, int32, int32);
#endif

extern void memoryClear ( uint8 *, uint32 );
extern void memoryCopy ( uint8 *, uint8 *, uint32 );
extern void curTimeGetInString(uint32 *, int8 *);
extern uint32 timeOfDayGet(void);
uint8* getSourceFileName(uint8* ,uint32*);
extern void spaceSkip(int8 **);
extern int32   ioRead(int32 , int8 *, int32 );
extern uint32	myBusDataAcc(uint8 *, int32 ,int32	, int32 ,uint8 *, uint8 *);
extern void setErrorCode(uint8 *,uint32,uint8 *,uint32);
extern void boardLedBlink (uint8 	, int32	);
extern void fourBytesDataCopy ( uint32 *, uint32 *, uint32 );


uint32  fatalErrorLogInit(void)
{
    uint32  status = NO_ERROR;
    uint32   fatalErrVmeAddr;
    int8    timeString[32];
    strSemaInfo     sema;
    strFatalErrLogInfo  flagInfo;
    uint32	loop, dataNum, *vmeDataPtr, *localDataPtr;

    
#ifdef VXWORKS
    sema.semaType       = MUTEX_SEMA;
    sema.semaOptions    = MYSEM_Q_PRIORITY|
                          MYSEM_DELETE_SAFE|
                          MYSEM_INVERSION_SAFE;
#else
	sema.semaType       = MUTEX_SEMA;
	sema.semaInitVal	= FALSE;
	sema.semaName		= "FatalErrMsgSema";
#endif
    
 
    status = semaphoreCreate(   (strSemaInfo *) &sema,
                                (int32 *)&sFatalErrMsgSemaId
                            );


    if(status) goto MODULE_END;

	
    fatalErrVmeAddr = (uint32)FATAL_MSG_START_OFFSET;
	fatalErrVmeAddr += sizeof(strFatalErrMsgInfo) * MAX_FATAL_ERR_NUM;

    if(fatalErrVmeAddr > (uint32)gDualDataBusPtr) 
    {
		status = NVRAM_ADDR_EXCEED_ERR;
        return(status);
    }

    
    vmeDataPtr 		= (uint32 *)FIRST_FATAL_ERR_MNGR_OFFSET;
    localDataPtr	= (uint32 *)&flagInfo;
    
    dataNum = sizeof(strFatalErrLogInfo)/4;
    
    for(loop = 0; loop < dataNum; loop++, vmeDataPtr++, localDataPtr++) 
    {
		status = myBusDataAcc(	__FILE__,
								__LINE__, 
								BUS_LONG_ACC, 
								BUS_DATA_READ,
								(uint8 *)vmeDataPtr, 
								(uint8 *)localDataPtr
					 		);
					 				
		if(status != NO_ERROR)
    		return(status);
    }

    
    if(flagInfo.firstFlag == NOW_USING_RAM)
    {
        memoryClear( timeString, 32);
        curTimeGetInString((uint32 *) &flagInfo.firstSecTime, timeString);
        printf( "\n[LOG MESSAGE CREATION DATE] - %s\n", timeString);
    }
    else
    { 
        flagInfo.firstFlag 		= NOW_USING_RAM;
       	flagInfo.firstSecTime 	= timeOfDayGet();
        flagInfo.curIndex 		= 0;

		
		vmeDataPtr 		= (uint32 *)FIRST_FATAL_ERR_MNGR_OFFSET;
		localDataPtr	= (uint32 *)&flagInfo;
	
		for(loop = 0; loop < dataNum; loop++, vmeDataPtr++, localDataPtr++) 
		{
			status = myBusDataAcc(	__FILE__,
									__LINE__, 
									BUS_LONG_ACC, 
									BUS_DATA_WRITE,
									(uint8 *)vmeDataPtr, 
									(uint8 *)localDataPtr
						 		);			
			if(status != NO_ERROR)				
				return(status);
			
		}
    }

MODULE_END:
    return(status);
}


void fatalErrMsgClear (uint32 arg1, uint32 arg2)
{
    uint32  status = NO_ERROR;
    strFatalErrLogInfo  flagInfo;
    uint32	loop, dataNum, *vmeDataPtr, *localDataPtr,clearData;

    
    status = semaphoreGet(sFatalErrMsgSemaId, FOREVERWAIT);
	if(status != NO_ERROR) {
		printf( "\n  ** Fatal Error Message Clear Semaphore Get Error!!!\n");
		return;
	}
	
	
	dataNum = (sizeof(strFatalErrMsgInfo)/4) * MAX_FATAL_ERR_NUM;
	vmeDataPtr 	= (uint32 *)FATAL_MSG_START_OFFSET;
	clearData	= 0;
    for(loop = 0; loop < dataNum; loop++, vmeDataPtr++) {
		status = myBusDataAcc(	__FILE__,
								__LINE__, 
								BUS_LONG_ACC, 
								BUS_DATA_WRITE,
								(uint8 *)vmeDataPtr, 
								(uint8 *)&clearData
					 		);
					 		
		if(status != NO_ERROR) {
			printf( "[ERROR]- Fatal MESSAGE Clear Error[%d-0x%x]\n",  
					loop+1, (uint32)vmeDataPtr);
			goto MODULE_END;
		}
	}

	
    vmeDataPtr 		= (uint32 *)FIRST_FATAL_ERR_MNGR_OFFSET;
    localDataPtr	= (uint32 *)&flagInfo;
    
    dataNum = sizeof(strFatalErrLogInfo)/4;
    for(loop = 0; loop < dataNum; loop++, vmeDataPtr++, localDataPtr++) {
		status = myBusDataAcc(	__FILE__,
								__LINE__, 
								BUS_LONG_ACC, 
								BUS_DATA_READ,
								(uint8 *)vmeDataPtr, 
								(uint8 *)localDataPtr
					 		);
					 		
		if(status != NO_ERROR) {
			printf( "[ERROR]- Fatal Mngr Info Read Error[%d-0x%x]\n",  
					loop+1, (uint32)vmeDataPtr);
			goto MODULE_END;
		}
    }

	flagInfo.curIndex	= 0;
    vmeDataPtr 		= (uint32 *)FIRST_FATAL_ERR_MNGR_OFFSET;
    localDataPtr	= (uint32 *)&flagInfo;
    
    for(loop = 0; loop < dataNum; loop++, vmeDataPtr++, localDataPtr++) {
		status = myBusDataAcc(	__FILE__,
								__LINE__, 
								BUS_LONG_ACC, 
								BUS_DATA_WRITE,
								(uint8 *)vmeDataPtr, 
								(uint8 *)localDataPtr
					 		);
					 		
		if(status != NO_ERROR) {
			printf( "[ERROR]- Fatal Mngr Info Write Error[%d-0x%x]\n",  
					loop+1, (uint32)vmeDataPtr);
			goto MODULE_END;
		}
    }

MODULE_END:
#ifdef VXWORKS
	    semaphoreRelease(sFatalErrMsgSemaId);
#else
	    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFatalErrMsgSemaId);
#endif

	if(status == NO_ERROR)
		printf( "\n  ** Fatal Error Message Clear !!!\n");

	return;
}


void logMessageSet (int8 *fileName, uint32 line, int8 *moduleName, uint32 errorNum, uint32 data)
{
    uint32  status = NO_ERROR;
    strFatalErrMsgInfo  logInfo;
	uint32	nameLen;
    strFatalErrLogInfo  *flagInfoVmePtr;
    uint32	dataNum, *vmeDataPtr, *localDataPtr;

    memoryClear( (uint8 *) &logInfo, sizeof(strFatalErrMsgInfo));

    
    status = semaphoreGet(sFatalErrMsgSemaId, FOREVERWAIT);

    if(status) {
      setErrorCode ( 	__FILE__,
   						__LINE__,
						(uint8 *)"logMessageSet",
                       	status
                      );
        return;
    }

    
    flagInfoVmePtr 		= (strFatalErrLogInfo *)FIRST_FATAL_ERR_MNGR_OFFSET;

	if(flagInfoVmePtr->curIndex > (MAX_FATAL_ERR_NUM - 1)) {
		fatalErrMsgRefresh (fileName, line, moduleName, errorNum,data);
        goto MODULE_END;
    }

    nameLen = strlen(fileName);
    
	
	fileName = getSourceFileName(fileName,&nameLen);

	if(nameLen > FALTA_FILE_NAME_LEN)
    	 nameLen = FALTA_FILE_NAME_LEN - 1;

    memoryCopy ( (uint8 *)logInfo.fileName,
                 (uint8 *)fileName,
                 nameLen
               );

	logInfo.lineNum = line;

    nameLen = strlen(moduleName);

	if(nameLen > FATAL_MODULE_NAME_LEN)
    	nameLen = FATAL_MODULE_NAME_LEN - 1;

    memoryCopy ( (uint8 *)logInfo.moduleName,
                 (uint8 *)moduleName,
                 nameLen
               );

   	logInfo.fatalErrorNum 	= errorNum;
   	logInfo.totalSecTime 	= timeOfDayGet();
	logInfo.vmeAddr			= data;
	
    
    dataNum 		= sizeof(strFatalErrMsgInfo)/4;
    
    vmeDataPtr 		= (uint32 *)FATAL_MSG_START_OFFSET;
    vmeDataPtr		+= dataNum * flagInfoVmePtr->curIndex; 
    
    localDataPtr	= (uint32 *)&logInfo;

    fourBytesDataCopy(vmeDataPtr, localDataPtr,dataNum);

    
	flagInfoVmePtr->curIndex	+= 1;
	
MODULE_END:
#ifdef VXWORKS
	semaphoreRelease( sFatalErrMsgSemaId);  
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sFatalErrMsgSemaId);  
#endif

	return;
}


static void fatalErrMsgRefresh(	int8 *fileName, 
								uint32 line,
								int8 *moduleName, 
								uint32 errorNum, 
								uint32 data
							)
{
    strFatalErrMsgInfo  logInfo;
    strFatalErrMsgInfo  readLogInfo[MAX_FATAL_ERR_NUM];
	uint32	nameLen;
    uint32	dataNum, *vmeDataPtr, *localDataPtr;

    
    dataNum 		= (sizeof(strFatalErrMsgInfo)/4) * MAX_FATAL_ERR_NUM;
    vmeDataPtr 		= (uint32 *)FATAL_MSG_START_OFFSET;
    localDataPtr	= (uint32 *)&readLogInfo[0];
    
	fourBytesDataCopy ( localDataPtr, vmeDataPtr, dataNum);

    
    dataNum 		= (sizeof(strFatalErrMsgInfo)/4) * (MAX_FATAL_ERR_NUM-1);
    vmeDataPtr 		= (uint32 *)FATAL_MSG_START_OFFSET;
    localDataPtr	= (uint32 *)&readLogInfo[1];
    
	fourBytesDataCopy ( vmeDataPtr, localDataPtr, dataNum);

    
    nameLen = strlen(fileName);
    
	
	fileName = getSourceFileName(fileName,&nameLen);

	if(nameLen > FALTA_FILE_NAME_LEN)
    	nameLen = FALTA_FILE_NAME_LEN -1;

	memoryClear((uint8*)&logInfo.fileName,sizeof(logInfo.fileName));
    memoryCopy ( (uint8 *)logInfo.fileName,
                 (uint8 *)fileName,
                 nameLen
               );

	logInfo.lineNum = line;

    nameLen = strlen(moduleName);

    if(nameLen >= FATAL_MODULE_NAME_LEN) 
	   	nameLen = FATAL_MODULE_NAME_LEN -1 ;

    
	memoryClear((uint8*)&logInfo.moduleName,sizeof(logInfo.moduleName));
    memoryCopy ( logInfo.moduleName,
                 moduleName,
                 nameLen
               );
    logInfo.fatalErrorNum 	= errorNum;
    logInfo.totalSecTime 	= timeOfDayGet();
	logInfo.vmeAddr			= data;

    dataNum 		= sizeof(strFatalErrMsgInfo)/4;
    vmeDataPtr 		= (uint32 *)FATAL_MSG_START_OFFSET;
	vmeDataPtr 		+= (sizeof(strFatalErrMsgInfo)/4) * (MAX_FATAL_ERR_NUM - 1);
    localDataPtr	= (uint32 *)&logInfo;
    
	fourBytesDataCopy ( vmeDataPtr, localDataPtr, dataNum);

	return;
}


void fatalErrMsgDisplay(uint32 arg1, uint32 arg2)
{
    uint32  status = NO_ERROR;
    strFatalErrMsgInfo  logInfo[MAX_FATAL_ERR_NUM], *readLogInfoPtr;
    strFatalErrLogInfo  flagInfo;
    int8    timeString[32];
    uint32  index = 0;
    uint32	loop, dataNum, *vmeDataPtr, *localDataPtr;

    
    status = semaphoreGet(sFatalErrMsgSemaId, FOREVERWAIT);

    if(status) {
    	status = L_GET_SEMA_ERR;
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
        return;
    }

    
    vmeDataPtr 		= (uint32 *)FIRST_FATAL_ERR_MNGR_OFFSET;
    localDataPtr	= (uint32 *)&flagInfo;
    
    dataNum = sizeof(strFatalErrLogInfo)/4;
    for(loop = 0; loop < dataNum; loop++, vmeDataPtr++, localDataPtr++) {
		status = myBusDataAcc(	__FILE__,
								__LINE__, 
								BUS_LONG_ACC, 
								BUS_DATA_READ,
								(uint8 *)vmeDataPtr, 
								(uint8 *)localDataPtr
					 		);
					 		
		if(status != NO_ERROR) {
	        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
#ifdef VXWORKS
    		semaphoreRelease(sFatalErrMsgSemaId);
#else
    		semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFatalErrMsgSemaId);
#endif
			return;
    	}
    }

   
    dataNum 		= (sizeof(strFatalErrMsgInfo)/4) * flagInfo.curIndex;
    vmeDataPtr 		= (uint32 *)FATAL_MSG_START_OFFSET;
    localDataPtr	= (uint32 *)&logInfo[0];
    
    for(loop = 0; loop < dataNum; loop++, vmeDataPtr++, localDataPtr++) {
		status = myBusDataAcc(	__FILE__,
								__LINE__, 
								BUS_LONG_ACC, 
								BUS_DATA_READ,
								(uint8 *)vmeDataPtr, 
								(uint8 *)localDataPtr
					 		);
		if(status != NO_ERROR) {
	        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
#ifdef VXWORKS
    		semaphoreRelease(sFatalErrMsgSemaId);
#else
    		semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFatalErrMsgSemaId);
#endif
			return;
    	}
	}

#ifdef VXWORKS
    semaphoreRelease(sFatalErrMsgSemaId);
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sFatalErrMsgSemaId);
#endif

	readLogInfoPtr = logInfo;
		
	for(index = 0; index < flagInfo.curIndex; index++, readLogInfoPtr++) {
		memoryClear( (uint8 *)timeString, 32);
        curTimeGetInString(	(uint32 *) &readLogInfoPtr->totalSecTime, 
							timeString
							);
		fatalErrStringDisplay(	readLogInfoPtr->fileName,
        						readLogInfoPtr->lineNum,
								readLogInfoPtr->moduleName, 
								readLogInfoPtr->fatalErrorNum,
								readLogInfoPtr->vmeAddr,
                            	timeString, 
								index+1
                        );
    }
    
    return;
}


void fatalErrQueCountDisplay(void)
{
    strFatalErrLogInfo  flagInfo;
	uint32	status;
	uint32 loop, dataNum, *vmeDataPtr, *localDataPtr;

    
    vmeDataPtr 		= (uint32 *)FIRST_FATAL_ERR_MNGR_OFFSET;
    localDataPtr	= (uint32 *)&flagInfo;
    
    dataNum = sizeof(strFatalErrLogInfo)/4;
    for(loop = 0; loop < dataNum; loop++, vmeDataPtr++, localDataPtr++) 
    {
		status = myBusDataAcc(	__FILE__,
								__LINE__, 
								BUS_LONG_ACC, 
								BUS_DATA_READ,
								(uint8 *)vmeDataPtr, 
								(uint8 *)localDataPtr
					 		);
					 		
		if(status != NO_ERROR) {
	        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
    		return;
    	}
    }

    printf( "\n\t************[Logging Message Queue : %d]************\n",MAX_FATAL_ERR_NUM);

    printf( "\tCurrent Logging Msg. Num :%d\n", flagInfo.curIndex);
	printf( "\n");

	return;
}


uint8* getSourceFileName(uint8* fileName,uint32 *fileLength)
{
	uint32 dirLength=0;
	uint8 *pdest;
#ifdef _WIN32
	pdest = (uint8 *)strrchr(fileName,'\\');
#else 
	pdest = (uint8 *)strrchr(fileName,'/');
#endif
 	dirLength = pdest - fileName +1 ;
	*fileLength = strlen(fileName+dirLength);
	
	return(fileName+dirLength);
}


uint32  sysHistoryFatalErrMsgGet(strSysHistoryInfo* sysHistory)
{
    uint32  status = NO_ERROR;
    strFatalErrMsgInfo  logInfo[MAX_FATAL_ERR_NUM], *readLogInfoPtr;
    strFatalErrLogInfo  flagInfo;
    uint32  index = 0;
	uint32 loop, dataNum, *vmeDataPtr, *localDataPtr;

    
    status = semaphoreGet(sFatalErrMsgSemaId, FOREVERWAIT);
    if(status) {
    	status = L_GET_SEMA_ERR;
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
        return(status);
    }

    
    vmeDataPtr 		= (uint32 *)FIRST_FATAL_ERR_MNGR_OFFSET;
    localDataPtr	= (uint32 *)&flagInfo;
    
    dataNum = sizeof(strFatalErrLogInfo)/4;
    for(loop = 0; loop < dataNum; loop++, vmeDataPtr++, localDataPtr++) {
		status = myBusDataAcc(	__FILE__,
								__LINE__, 
								BUS_LONG_ACC, 
								BUS_DATA_READ,
								(uint8 *)vmeDataPtr, 
								(uint8 *)localDataPtr
					 		);
					 		
		if(status != NO_ERROR) {
	        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
			goto MODULE_END;
    	}
    }

	if(flagInfo.curIndex > MAX_FATAL_ERR_NUM)
	{
		status = L_MAX_FATAL_ID_ERR;
		goto MODULE_END;
	}
	else sysHistory->historyNum = flagInfo.curIndex;
	
   
    dataNum 		= (sizeof(strFatalErrMsgInfo)/4) * flagInfo.curIndex;
    vmeDataPtr 		= (uint32 *)FATAL_MSG_START_OFFSET;
    localDataPtr	= (uint32 *)&logInfo[0];
    
    for(loop = 0; loop < dataNum; loop++, vmeDataPtr++, localDataPtr++) {
		status = myBusDataAcc(	__FILE__,
								__LINE__, 
								BUS_LONG_ACC, 
								BUS_DATA_READ,
								(uint8 *)vmeDataPtr, 
								(uint8 *)localDataPtr
					 		);
		if(status != NO_ERROR) {
	        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);
			goto MODULE_END;
    	}
	}

	readLogInfoPtr = logInfo;
		
	for(index = 0; index < flagInfo.curIndex; index++, readLogInfoPtr++) {
		sysHistory->sysHistory[index].totalSecTime = readLogInfoPtr->totalSecTime;
		sysHistory->sysHistory[index].fatalErrorNum = readLogInfoPtr->fatalErrorNum;
    }
    

MODULE_END:
#ifdef VXWORKS
	semaphoreRelease( sFatalErrMsgSemaId);  
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sFatalErrMsgSemaId);  
#endif

	return(status);
}


