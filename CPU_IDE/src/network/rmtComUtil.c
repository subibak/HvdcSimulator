
#include	<stdio.h>

#include    "usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include	"taskDefs.h"
#include    "network.h"
#include	"fbMem.h"
#include	"msgQueData.h"
#include	"usrQueData.h"
#include	"msgQue.h"
#include	"usrErrCode.h"
#include    "lkv120Board.h"
#include	"usrSema.h"
#include	"cmdList.h"
#include	"netProtocol.h"

#include	"segScheduler.h"

#include	"protocolBuf.h"

#include    "debugInfo.h"
#include    "osDepend.h"

#include	"rmt_comm\rmtDevUtil.h"

#include	"segScheduler.h"

#include	"rmt_comm\ethModbus.h"

#define	SEMA_GET_TIMEOUT FOREVERWAIT


static uint32   sRmtComQSemaId;
static strRmtDevSendMsgInfo  sRmtComDataInfo[MAX_RMT_COM_Q_COUNT];
static strRmtComQInfo    sRmtComQue;


static uint32   sRmtDualComQSemaId;
static strEthComDualMsgInfo  sRmtDualComDataInfo[MAX_RMT_COM_Q_COUNT];
static strRmtDualComQInfo    sRmtDualComQue;


uint32	sRmtComCountSetFlag = 0;
strRmtComCountDisplayInfo gRmtComCountInfo;


strEthRmtSeqNumChkDBInfo	sEthRmtComSeqNumChkDBInfo[MAX_LOGIC_TASK_NUM];

static uint32 	sRemoteDevStatusDisplayFlag = DEBUG_DISABLE;
static uint32 	sPlcStatusRespDisplayFlag  = DEBUG_DISABLE;


int32 gTcpSocketSemaId;


extern int32	gDebugPortFd;

extern uint32 gStatusReadSize;
extern uint32 gStatusReadAddr;
	

uint32  rmtComMngrInit(void);

void rmtCommQueCountDisplay(void);

void	rmtDevMsgDisplay  (uint8 *, uint8 *, uint32, strinetAddr *);
uint32  rmtDeviceTypeGet(strNewNetProtocolInfo *,uint32 *);					
void	longDataConversion( uint32 *, uint32 *, uint32 );
void	shortDataConversion( uint16 *, uint16 *, uint32 );

void rmtRCSComCountSet(	strDataBuf *);
void rmtComCountDebugVarInit(void);
void rmtComCountSet(uint32 *);
void rmtComCountDisplay(uint32, uint32);

uint32	ethRmtFbComSeqNoChkFunc(uint32 , uint32 , uint32 , uint32 *);
void rmtEthComSeqNumChkDBClear(uint32 );
uint32	rmtEthComSeqNumChkDBSet(uint32 );
void ethRmtFbComSeqNoChkDBDisplay (uint32, uint32); 
void remoteDevStatusDisplay(uint32, uint32);
void plcStatusRespDisplay(void);

uint32 rmtDevProtocolEncode (strNewNetProtocolInfo *,strDataBuf *);			
uint32 rmtDevMsgInfoSet	(strDataBuf *,strNewNetProtocolInfo *);				
void rmtEthComSeqNumChkDBClear(uint32 );
uint32  rmtComInfoQueClear(void);
uint32  rmtDualComInfoQueClear(void);

static void rmtDualComQueLiveCountCheck (void);

#ifdef VXWORKS
void	curLongDataConversion( uint32 *, uint32);
#endif


extern uint32 getFbTaskTblInfo(uint32 , uint32 , strFbTaskTblMemInfo *);
extern uint32 fbCodeMemInfoRead(uint32, uint32, strFbMemInfo *);
extern uint32 fbMemInfoRead(uint32, uint32, strFbMemInfo *);
extern uint32	readRuntimeFbData(uint32, uint32, uint32, uint32 *);

extern void ipAddrHexToString(int8 *, uint32 );
extern uint32 netMsgChecksumCalcu(uint32  ,uint32 *,uint32    *);

extern strDoubleLink *nextNodeRef(strDoubleLink *,strDoubleLink *);
extern int32 nodePutOnLast(strDoubleLink **,strDoubleLink *);
extern int32 nodeUnlink( strDoubleLink **,strDoubleLink *);
extern strDoubleLink *nodeGetAndUnlink(strDoubleLink **);
extern int32 	semaphoreCreate(strSemaInfo *, int32 *);
extern int32 	semaphoreGet(int32 , int32 );
#ifdef _WIN32
extern int32	semaphoreRelease(int32, int32);
extern int32	semaphoreRelease2(uint8 *, uint32, int32, int32);
#else
extern int32	semaphoreRelease(int32);
#endif

extern void memoryClear ( uint8 *, uint32 );
extern void memoryCopy ( uint8 *, uint8 *, uint32 );
extern void dataDisplay(uint8 *, uint8 *, uint16 );
extern int32   keyHit(void); 
extern void	  sysTaskDelay (int32 );
extern int32  systemClockRateGet(void);
extern void setErrorCode(uint8 *,uint32,uint8 *,uint32);
extern void setErrorCodeWithVal(uint8 *, uint32, uint8 *, uint32,
				uint8 *, uint32, uint8 *, uint32, uint8 *, uint32);

extern void byteConversionOnShort(uint16 *, uint32 *);
extern uint32  localPlcProtocolDecode(strDataInfo *, strDataInfo *);
extern uint32 stringCompareWithLen(uint8 *, uint8 *, uint32);
extern uint32	fbSysExeModuleInfoRead( uint32, strModuleExecInfo* );

extern uint32	getMemAddrFromBinNumberAndOffSet(uint8 , uint16 , uint32 *);
extern void		mySwap(void *, void *, uint32);
extern uint32  modbusTcpProtocolDecode(strDataInfo *, strDataInfo *);

uint32  rmtComMngrInit(void)
{
    uint32  status = NO_ERROR;
    uint32  loop = 0, taskId;
    strSemaInfo     sema;

    
    
#ifdef VXWORKS	
    sema.semaType       = MUTEX_SEMA;
    sema.semaOptions    = MYSEM_Q_PRIORITY|
                          MYSEM_DELETE_SAFE|
                          MYSEM_INVERSION_SAFE;
#else			
    sema.semaType       = MUTEX_SEMA;
	sema.semaInitVal	= FALSE;
	sema.semaName		= "RmtComQ";
#endif

    
    status = semaphoreCreate(   (strSemaInfo *) &sema,
                                (int32 *) &sRmtComQSemaId
                            );

    
    if(status == NO_ERROR) {
        sRmtComQue.freeList = NULL;
        sRmtComQue.usedList = NULL;

        for( loop = 0; loop < MAX_RMT_COM_Q_COUNT; loop++) {
            memoryClear (   (int8 *) &sRmtComDataInfo[loop],
                            sizeof(strRmtDevSendMsgInfo)
                        );
            (void) nodePutOnLast(   (strDoubleLink **) &sRmtComQue.freeList,
                                    (strDoubleLink *) &sRmtComDataInfo[loop]
                                );
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
	sema.semaName		= "RmtDualComQ";
#endif
    
    status = semaphoreCreate(   (strSemaInfo *) &sema,
                                (int32 *) &sRmtDualComQSemaId
                            );



	
    

#ifdef VXWORKS	
    sema.semaType       = MUTEX_SEMA;
    sema.semaOptions    = MYSEM_Q_PRIORITY|
                          MYSEM_DELETE_SAFE|
                          MYSEM_INVERSION_SAFE;
#else			
    sema.semaType       = MUTEX_SEMA;
	sema.semaInitVal	= FALSE;
	sema.semaName		= "RmtTcpInfo";
#endif
    
    status = semaphoreCreate(   (strSemaInfo *) &sema,
                                (int32 *) &gTcpSocketSemaId
                            );

    
    if(status == NO_ERROR) {
        sRmtDualComQue.freeList = NULL;
        sRmtDualComQue.usedList = NULL;

        for( loop = 0; loop < MAX_RMT_COM_Q_COUNT; loop++) {
            memoryClear (   (int8 *) &sRmtDualComDataInfo[loop],
                            sizeof(strEthComDualMsgInfo)
                        );
            (void) nodePutOnLast(   (strDoubleLink **) &sRmtDualComQue.freeList,
                                    (strDoubleLink *) &sRmtDualComDataInfo[loop]
                                );
        }
    }
	
    
    for(taskId = 1; taskId <= MAX_LOGIC_TASK_NUM; taskId++) {
		status = rmtEthComSeqNumChkDBSet(taskId);
        if(status != NO_ERROR) break;
	}
    return(status);
}


void rmtDevMsgDisplay ( 	uint8 	*stringPtr,
						uint8 	*dataPtr,
                        uint32	dataLen,
                        strinetAddr	*peerAddr
					)
{
	uint8	ipAddr[32];

	printf("\n");
	printf("****************[%s Port Info]**************\n", 
					stringPtr
			);

	memoryClear (ipAddr, 32);

	ipAddrHexToString (ipAddr, peerAddr->iAddr);

	printf("\t	 Dest Ip Address : %s\n", ipAddr);
	printf("\t	Dest Port Number : %d\n", peerAddr->port);

    dataDisplay ( stringPtr, dataPtr, (uint16)dataLen);

    return;
}


void rmtComCountDisplay(uint32 arg1, uint32 arg2)
{
	uint8 *titleStr[] = {
        "DATA R-SEND", "DATA R-RECV",
        "DATA W-SEND", "DATA W-RECV",
        NULL
	};

    uint8 **pPtr;

    printf( "\n");
    printf( "\t------------------------------------------------------\n");
    printf( "\t**         Remote Communication Count  \n");
    printf( "\t------------------------------------------------------\n");

    for (pPtr = titleStr; *pPtr != NULL; pPtr++)
    	printf("%12s", *pPtr);

	printf("\n");

    for (pPtr = titleStr; *pPtr != NULL; pPtr++)
    	printf("%12s", "-----------");
	
   	printf("\n");

	if(sRmtComCountSetFlag == DEBUG_DISABLE)
    	sRmtComCountSetFlag = DEBUG_ENABLE;
	else 
    	sRmtComCountSetFlag = DEBUG_DISABLE;

    while( !keyHit()) {
		printf("%12d%12d%12d%12d%12d%12d\r",
					gRmtComCountInfo.rmtDevDataReadComSendCount,
					gRmtComCountInfo.rmtDevDataReadComRecvCount,
					gRmtComCountInfo.rmtDevDataWriteComSendCount,
					gRmtComCountInfo.rmtDevDataWriteComRecvCount
				);
		sysTaskDelay(systemClockRateGet());
	}

	return;
}

void rmtComCountDebugVarInit(void)
{
	memoryClear ( 	(uint8 *) &gRmtComCountInfo,
					sizeof(strRmtComCountDisplayInfo)
				); 
}

void rmtComCountSet(uint32 *dataPtr)
{
	if ((*dataPtr)++ > 0x7fffffff)
    	*dataPtr = 1;

	return;
}

void rmtRCSComCountSet(	strDataBuf *rcvDataPtr)
{
	strNewNetProtocolInfo    *dataPtr;
	uint32 opCode, optionCode, msgType, *cmdCode;
	
    dataPtr = (strNewNetProtocolInfo *)rcvDataPtr->data_buf;

	cmdCode = (uint32*)(dataPtr->dataBuff);
 	opCode = (*cmdCode)>>16;
	optionCode = (*cmdCode)&0xffff;

	msgType		= dataPtr->messageType;
	
    switch(opCode) {
		case ETHERNET_COM_PATH_OP:
        	if(msgType == REQUEST) {
                
                if(sRmtComCountSetFlag)
                	rmtComCountSet(&gRmtComCountInfo.rmtDevStatusReadComSendCount);
			}
            else if(msgType == RESPONSE) {
                
                if(sRmtComCountSetFlag)
                	rmtComCountSet(&gRmtComCountInfo.rmtDevStatusReadComRecvCount);
            }
        break;
        case ETHERNET_DATA_ACC_OP:
        	switch(optionCode) {
				case ETHERNET_DATA_READ:
		        	if(msgType == REQUEST) {
        		        
                		if(sRmtComCountSetFlag)
                			rmtComCountSet(&gRmtComCountInfo.rmtDevDataReadComSendCount);
					}
            		else if(msgType == RESPONSE) {
                		
                		if(sRmtComCountSetFlag)
                		rmtComCountSet(&gRmtComCountInfo.rmtDevDataReadComRecvCount);
            		}
                break;

				case ETHERNET_DATA_WRITE:
		        	if(msgType == REQUEST) {
        		        
                		if(sRmtComCountSetFlag)
                			rmtComCountSet(&gRmtComCountInfo.rmtDevDataWriteComSendCount);
					}
            		else if(msgType == RESPONSE) {
                		
                		if(sRmtComCountSetFlag)
                		rmtComCountSet(&gRmtComCountInfo.rmtDevDataWriteComRecvCount);
            		}
                break;
            }
        break;
    }
}

void longDataConversion( uint32 *destPtr, uint32 *srcPtr, uint32 dataLen)
{
    uint32 i;
    uint8 *ucDp, *ucSp;

    for( i = 0; i < (dataLen/4); i++, destPtr++, srcPtr++){
        ucDp = (uint8 *)destPtr;
        ucSp = (uint8 *)srcPtr;
        COPYL(ucDp, ucSp);
    }
	
	if( (dataLen%4 == 2 ) ){
		ucDp = (uint8 *)destPtr;
        ucSp = (uint8 *)srcPtr;
        COPYS(ucDp, ucSp);
	}
}
									

void shortDataConversion( uint16 *destPtr, uint16 *srcPtr, uint32 dataLen)
{
    uint32 i;
    uint8 *ucDp, *ucSp;

    for( i = 0; i < (dataLen/2) ; i ++, destPtr++, srcPtr++){
        ucDp = (uint8 *)destPtr;
        ucSp = (uint8 *)srcPtr;
        COPYS(ucDp, ucSp);
    }
}

#ifdef VXWORKS

void curLongDataConversion( uint32 *destPtr, uint32 dataLen)
{
    uint32 i;
    uint8 *ucDp, *ucSp;
	uint8  data_buf[MAX_DATA_LEN];
	uint32  *srcPtr = (uint32*)&data_buf[0];

	if(dataLen > MAX_DATA_LEN) 
	{
		setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__, L_NET_MSG_LEN_ERR,
							"dataLen", dataLen,
							"MAX_DATA_LEN", MAX_DATA_LEN,
							"NOT Using",0);
		return;
	}
	else if(destPtr == NULL) 
		return;

	memoryCopy((uint8*)&data_buf[0],(uint8*)destPtr,dataLen);

    for( i = 0; i < (dataLen/4); i++, destPtr++, srcPtr++){
        ucDp = (uint8 *)destPtr;
        ucSp = (uint8 *)srcPtr;
        COPYL(ucDp, ucSp);
    }

	if( (dataLen%4 == 2 ) ){
		ucDp = (uint8 *)destPtr;
        ucSp = (uint8 *)srcPtr;
        COPYS(ucDp, ucSp);
	}
}
#endif 


uint32	rmtEthComSeqNumChkDBSet(uint32 taskId)
{
	uint32	status = NO_ERROR, i, index = 0;
	strFbTaskTblMemInfo	segTblInfo;
	strFbMemInfo		fbMemInfo;
	strEthRmtSeqNumChkDBInfo	*dbInfoPtr;
	strModuleExecInfo sysExecInfo;

	if( (taskId < 1)||(taskId>MAX_LOGIC_TASK_NUM) ) {
		status = L_SEG_ID_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

    dbInfoPtr = &sEthRmtComSeqNumChkDBInfo[taskId - 1];

	status = getFbTaskTblInfo (LOGIC_ID, taskId, &segTblInfo);
	if(status != NO_ERROR) {
		setErrorCodeWithVal (	__FILE__,
        						__LINE__,
                                "rmtEthComSeqNumChkDBSet",
                                status,
                                "Seg ID", taskId,
                                "Unuse", 0,
                                "Unuse", 0
							  );
		return(status);
    }

	memoryClear((uint8*)&sysExecInfo,sizeof(strModuleExecInfo));
	status = fbSysExeModuleInfoRead(LOGIC_ID, (strModuleExecInfo*) &sysExecInfo);
	if(status != NO_ERROR){
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);		
	}

	for(i = 1; i <= sysExecInfo.curfbMaxBlockAddr; i++) {
		status = fbCodeMemInfoRead (LOGIC_ID, i, &fbMemInfo);
		if(status != NO_ERROR) {
			setErrorCodeWithVal (	__FILE__,
    	    						__LINE__,
        	                        "rmtEthComSeqNumChkDBSet",
            	                    status,
                	                "Block Addr", i,
                    	            "Unuse", 0,
                        	        "Unuse", 0
								  );
			return(status);
		}

		if(fbMemInfo.fbTaskId!=taskId) continue;
		
		if( (fbMemInfo.fbId == ETH_RMT_STATUS_READ_FB_CODE) ||
			(fbMemInfo.fbId == ETH_RMT_DATA_SEND_FB_CODE) ||
			(fbMemInfo.fbId == MBT_RMT_DATA_SEND_FB_CODE) ||
			(fbMemInfo.fbId == ETH_RMT_DATA_BROAD_FB_CODE) ||
			(fbMemInfo.fbId == ETH_RMT_DATA_READ_FB_CODE_FOR_REPLACE) ||
			(fbMemInfo.fbId == ETH_RMT_DATA_WRITE_FB_CODE_FOR_REPLACE)
		){
			dbInfoPtr->fbCodeBlockAddr[index] 	= i;
            dbInfoPtr->fbId[index]			= fbMemInfo.fbId;
            dbInfoPtr->prevRcvSeqNum[index]		= 0;
            dbInfoPtr->rcvCount[index]			= 0;
            dbInfoPtr->seqErrCount[index]		= 0;
			
	        index++;
            if(index == MAX_ETH_COM_FB_CODE_NUM) {
				status = EXCEED_ETH_COM_FB_CODE_NUM_ERR;
	            setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				return(status);
            }
        }
    }
    return(status);
}

uint32  rmtDualComMsgCheck (strNewNetProtocolInfo    *msgDataInfoPtr)
{
	strEthComDualMsgInfo	rmtDualMsgInfo, *rmtDualQDataInfoPtr = NULL;
	uint32	opCode = 0, optionCode = 0, *cmdCode;
	uint32	rmtDevType = 0,blockAddr = 0;
	uint32 status = NO_ERROR;
	uint32	dualMsgFlag = 0, queueCount = 0;

	strEthDataReadReqInfo		*readReqInfoPtr = NULL;
	strEthDataWriteReqInfo		*writeReqInfoPtr = NULL;
	strEthCommPathReadReqInfo	*statusReadReqInfoPtr = NULL;

    
    if(msgDataInfoPtr->messageType != RESPONSE)
    	return(status);

 	cmdCode = (uint32*)(msgDataInfoPtr->dataBuff);
 	opCode = (*cmdCode)>>16;
	optionCode = (*cmdCode)&0xffff;

    switch(opCode) {
		case ETHERNET_DATA_ACC_OP:
        	if(optionCode == ETHERNET_DATA_READ) {
				readReqInfoPtr = (strEthDataReadReqInfo *)&msgDataInfoPtr->dataBuff[4];
                blockAddr 	= readReqInfoPtr->srcFbBlockAddr;
                rmtDevType 	= readReqInfoPtr->deviceType;
            }
            else if(optionCode == ETHERNET_DATA_WRITE) {
				writeReqInfoPtr = (strEthDataWriteReqInfo *)&msgDataInfoPtr->dataBuff[4];
                blockAddr 	= writeReqInfoPtr->srcFbBlockAddr;
                rmtDevType 	= writeReqInfoPtr->deviceType;
            }
            else {
				status = L_OPTION_CODE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
                return(status);
            }
        break;
        case ETHERNET_COM_PATH_OP:
			if(optionCode == ETHERNET_DEV_STATUS_READ) {
				statusReadReqInfoPtr = (strEthCommPathReadReqInfo *)&msgDataInfoPtr->dataBuff[4];

                blockAddr 	= statusReadReqInfoPtr->srcFbBlockAddr;
                rmtDevType 	= statusReadReqInfoPtr->deviceType;
			}
            else {
				status = L_OPTION_CODE_ERR;
	            setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
                return(status);
            }
        break;
        default:
        	return(NO_ERROR);
    }

    
    rmtDualMsgInfo.liveCount        = 0;
    rmtDualMsgInfo.seqNum           = msgDataInfoPtr->sequenceID;
    rmtDualMsgInfo.opCode           = opCode;
    rmtDualMsgInfo.optionCode       = optionCode;
    rmtDualMsgInfo.rmtDevType       = rmtDevType;
    rmtDualMsgInfo.fbBlockAddr      = blockAddr;

    
    if( (
            status = semaphoreGet(sRmtDualComQSemaId, SEMA_GET_TIMEOUT)
        ) > 0
      ) {
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
    }

    else if(status == NO_ERROR) {
        rmtDualQDataInfoPtr = (strEthComDualMsgInfo *) sRmtDualComQue.usedList;

        while(rmtDualQDataInfoPtr != NULL) {

            if  (
                    (rmtDualMsgInfo.seqNum == rmtDualQDataInfoPtr->seqNum)
                                        &&
                    (rmtDualMsgInfo.opCode == rmtDualQDataInfoPtr->opCode)
                                        &&
                    (rmtDualMsgInfo.optionCode == rmtDualQDataInfoPtr->optionCode)
                                        &&
                    (rmtDualMsgInfo.rmtDevType == rmtDualQDataInfoPtr->rmtDevType)
                                        &&
                    (rmtDualMsgInfo.fbBlockAddr == rmtDualQDataInfoPtr->fbBlockAddr)
                ){
                
                if  (   (
                        status = nodeUnlink (
                                (strDoubleLink **) &sRmtDualComQue.usedList,
                                (strDoubleLink *)rmtDualQDataInfoPtr
                                            )
                        ) == NO_ERROR
                    ) {
	
                    memoryClear ( 	(int8 *) rmtDualQDataInfoPtr,
                    				sizeof(strEthComDualMsgInfo)
								);
                    status = nodePutOnLast  (
                                (strDoubleLink **) &sRmtDualComQue.freeList,
                                (strDoubleLink *) rmtDualQDataInfoPtr
                                            );
                    if(status == NO_ERROR) {
                        status = DUAL_SAME_MSG_ERR;
                        dualMsgFlag = 1;
					}
                    else 
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
                }

                
                break;
            }
			
			if(++queueCount > MAX_RMT_COM_Q_COUNT) {
            	status = QUEUE_CRASH_ERR;
	            setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
                break;
			} 			
            rmtDualQDataInfoPtr = (strEthComDualMsgInfo *) nextNodeRef  (
                                    (strDoubleLink *)sRmtDualComQue.usedList,
                                    (strDoubleLink *)rmtDualQDataInfoPtr
                                                            );
        } 

        if((rmtDualQDataInfoPtr == NULL) && (status == NO_ERROR)){
            
            
            

            rmtDualQDataInfoPtr = (strEthComDualMsgInfo *) nodeGetAndUnlink(
                            (strDoubleLink **)&sRmtDualComQue.freeList
                                                                );
            if( rmtDualQDataInfoPtr != NULL) {
    			rmtDualQDataInfoPtr->liveCount        = 0;
    			rmtDualQDataInfoPtr->seqNum           = msgDataInfoPtr->sequenceID;
    			rmtDualQDataInfoPtr->opCode           = opCode;
    			rmtDualQDataInfoPtr->optionCode       = optionCode;
    			rmtDualQDataInfoPtr->rmtDevType       = rmtDevType;
    			rmtDualQDataInfoPtr->fbBlockAddr      = blockAddr;

                (void)nodePutOnLast  (
                         (strDoubleLink **) &sRmtDualComQue.usedList,
                         (strDoubleLink *) rmtDualQDataInfoPtr
                                     );
            }
            else {
                status = QUEUE_FULL_ERR;
	            setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			}
        }
        
        (void)rmtDualComQueLiveCountCheck ();

        
#ifdef VXWORKS
		semaphoreRelease( sRmtDualComQSemaId);  
#else
		semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sRmtDualComQSemaId); 
#endif 
	}

    return(status);
}

static void rmtDualComQueLiveCountCheck (void)
{
    uint32  status = NO_ERROR;
    strEthComDualMsgInfo  *rmtDualComQPtr      = NULL;
    uint32  unlinkTbl[MAX_RMT_COM_Q_COUNT];
    uint32  unlinkQueNum = 0, index;
	uint32	queueCount = 0, liveCount;

    rmtDualComQPtr = (strEthComDualMsgInfo *) sRmtDualComQue.usedList;

    while(rmtDualComQPtr != NULL) {
    	liveCount = ++rmtDualComQPtr->liveCount;

        if(liveCount > MAX_RMT_Q_LIVE_COUNT) {
            unlinkTbl[unlinkQueNum++] = (uint32)rmtDualComQPtr;
		}

        if(++queueCount > MAX_RMT_COM_Q_COUNT) 
        	break;

        rmtDualComQPtr = (strEthComDualMsgInfo *) nextNodeRef  (
                                    (strDoubleLink *)sRmtDualComQue.usedList,
                                    (strDoubleLink *)rmtDualComQPtr
                                                         );   
	}

    for(index = 0; index < unlinkQueNum; index++) {
        
        if  (   (
                status = nodeUnlink (
                                (strDoubleLink **) &sRmtDualComQue.usedList,
                                (strDoubleLink *)unlinkTbl[index]
                                     )
                ) == NO_ERROR
        ){
            nodePutOnLast  (
                            (strDoubleLink **) &sRmtDualComQue.freeList,
                            (strDoubleLink *) unlinkTbl[index]
                            );

            
            status = RMT_COM_Q_LIVE_COUNT_ERR;
#if NOT_USED
            setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
#endif
        } 
    }
}

									
void rmtCommQueCountDisplay(void)
{
    uint32 freeListCount = 0, usedListCount = 0;
    strRmtDevSendMsgInfo  *rmtComQPtr      = NULL;
    strEthComDualMsgInfo         *dLPortPtr      = NULL;
    uint32  status = NO_ERROR;

    status = semaphoreGet(sRmtComQSemaId, FOREVERWAIT);

    if(status == NO_ERROR) 
    {
        rmtComQPtr = (strRmtDevSendMsgInfo *)sRmtComQue.freeList;

        while(rmtComQPtr != NULL) 
        {
            freeListCount++;
            rmtComQPtr = (strRmtDevSendMsgInfo *) nextNodeRef  (
                                        (strDoubleLink *)sRmtComQue.freeList,
                                        (strDoubleLink *)rmtComQPtr
                                                             );
        }

        rmtComQPtr = (strRmtDevSendMsgInfo *)sRmtComQue.usedList;

        while(rmtComQPtr != NULL) 
        {
            usedListCount++;
            rmtComQPtr = (strRmtDevSendMsgInfo *) nextNodeRef  (
                                        (strDoubleLink *)sRmtComQue.usedList,
                                        (strDoubleLink *)rmtComQPtr
                                                             );
        }

#ifdef VXWORKS
		semaphoreRelease( sRmtComQSemaId);  
#else
		semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sRmtComQSemaId);  
#endif
    	printf( "\n\t************[System Comm. Queue : %d]************\n",MAX_RMT_COM_Q_COUNT);
		printf( "\tFree List :%d     Used List : %d\n", freeListCount, usedListCount);

    }

    freeListCount = 0;
    usedListCount = 0;

    status = semaphoreGet(sRmtDualComQSemaId, FOREVERWAIT);

    if(status == NO_ERROR) 
    {
        dLPortPtr = (strEthComDualMsgInfo *)sRmtDualComQue.freeList;

		while(dLPortPtr != NULL) 
		{
            freeListCount++;
            dLPortPtr = (strEthComDualMsgInfo *) nextNodeRef  (
                                        (strDoubleLink *)sRmtDualComQue.freeList,
                                        (strDoubleLink *)dLPortPtr
                                                             );
        }

        dLPortPtr = (strEthComDualMsgInfo *)sRmtDualComQue.usedList;

        while(dLPortPtr != NULL) 
        {
            usedListCount++;
            dLPortPtr = (strEthComDualMsgInfo *) nextNodeRef  (
                                        (strDoubleLink *)sRmtDualComQue.usedList,
                                        (strDoubleLink *)dLPortPtr
                                                             );
        }
        
#ifdef VXWORKS
		semaphoreRelease( sRmtDualComQSemaId);  
#else
		semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sRmtDualComQSemaId);  
#endif
	    printf( "\n\t************[System Com. Dual Line Info Queue : %d]************\n",MAX_RMT_COM_Q_COUNT);
    	printf( "\tFree List :%d     Used List : %d\n\n", freeListCount, usedListCount);
    }
	
    return;
}


uint32  rmtDeviceTypeGet(	strNewNetProtocolInfo    *msgDataInfoPtr,
							uint32 *rmtDevTypePtr
					  	)
{
    uint32  status = NO_ERROR;
	uint32	opCode, optionCode,*cmdCode;
	uint32	rmtDevType = 0,blockAddr;

	strEthDataReadReqInfo		*readReqInfoPtr;
	strEthDataWriteReqInfo		*writeReqInfoPtr;
	strEthCommPathReadReqInfo	*statusReadReqInfoPtr;

   	cmdCode = (uint32*)(msgDataInfoPtr->dataBuff);
 	opCode = (*cmdCode)>>16;
	optionCode = (*cmdCode)&0xffff;

    switch(opCode) {
		case ETHERNET_DATA_ACC_OP:
        	if(optionCode == ETHERNET_DATA_READ) {
				readReqInfoPtr = (strEthDataReadReqInfo *)&msgDataInfoPtr->dataBuff[4];
                blockAddr 	= readReqInfoPtr->srcFbBlockAddr;
                rmtDevType 	= readReqInfoPtr->deviceType;
            }
            else if(optionCode == ETHERNET_DATA_WRITE) {
				writeReqInfoPtr = (strEthDataWriteReqInfo *)&msgDataInfoPtr->dataBuff[4];
                blockAddr 	= writeReqInfoPtr->srcFbBlockAddr;
                rmtDevType 	= writeReqInfoPtr->deviceType;
            }
            else {
				status = L_OPTION_CODE_ERR;
	            setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
                return(status);
            }
        break;
        case ETHERNET_COM_PATH_OP:
			if(optionCode == ETHERNET_DEV_STATUS_READ) {
				statusReadReqInfoPtr = (strEthCommPathReadReqInfo *)&msgDataInfoPtr->dataBuff[4];

                blockAddr 	= statusReadReqInfoPtr->srcFbBlockAddr;
                rmtDevType 	= statusReadReqInfoPtr->deviceType;
			}
            else {
				status = L_OPTION_CODE_ERR;
	            setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
                return(status);
            }
        break;
        default:
        	status = MSG_OPCODE_ERR;
            setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		break;
    }

    *rmtDevTypePtr = rmtDevType;

    return(status);
}



uint32	ethRmtFbComSeqNoChkFunc(uint32 fbBlockAddr, uint32 fbId, 
								uint32	curSeqNum, uint32 *rcvCntPtr
							   )
{
	uint32	status = NO_ERROR;
	uint32	dbFbCode,dbFbBlkAddr, dbPrevSeqNum, taskId, fbNum;
	strEthRmtSeqNumChkDBInfo	*dbInfoPtr;
	strModuleExecInfo sysExecInfo;

	
	memoryClear((uint8*)&sysExecInfo,sizeof(strModuleExecInfo));
	status = fbSysExeModuleInfoRead(LOGIC_ID, (strModuleExecInfo*) &sysExecInfo);

	if(status != NO_ERROR)
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);		
	}

    dbInfoPtr = &sEthRmtComSeqNumChkDBInfo[0];
 
	for(taskId = 0; taskId < sysExecInfo.curMaxSegmentId; taskId++, dbInfoPtr++) 
	{
    	for(fbNum = 0; fbNum < MAX_ETH_COM_FB_CODE_NUM; fbNum++) 
		{
			dbFbCode 		= dbInfoPtr->fbId[fbNum];
			dbFbBlkAddr 	= dbInfoPtr->fbCodeBlockAddr[fbNum];
			dbPrevSeqNum 	= dbInfoPtr->prevRcvSeqNum[fbNum];

            
            if( (dbFbCode == fbId) && (dbFbBlkAddr == fbBlockAddr)) 
			{
				if( (dbPrevSeqNum == 0) || ((dbPrevSeqNum + 1) == curSeqNum)	)
				{
					*rcvCntPtr += 1;
                }
                else 
				{
                	
                	dbInfoPtr->seqErrCount[fbNum] += 1;

					status = RMT_ETH_COM_SEQ_NUM_ERR;
                    setErrorCodeWithVal (	__FILE__, __LINE__, __FUNCTION__, status,
                                            "PREV SEQ NUM",dbPrevSeqNum,
                                            "CUR SEQ NUM", curSeqNum,
                                            "FB ADDR", fbBlockAddr
										);
				}

				dbInfoPtr->prevRcvSeqNum[fbNum] = curSeqNum;
				dbInfoPtr->rcvCount[fbNum] 		+= 1;
				goto MODULE_END; 
            }
        }
	} 

    if(taskId == MAX_LOGIC_TASK_NUM) 
    	setErrorCode (__FILE__, __LINE__, __FUNCTION__, ETH_COM_DB_MISMATCH_ERR);

MODULE_END:
   return(status);
}



void rmtEthComSeqNumChkDBClear(uint32 taskId)
{
	strEthRmtSeqNumChkDBInfo	*dbInfoPtr;

    dbInfoPtr = &sEthRmtComSeqNumChkDBInfo[taskId - 1];

    memoryClear ( 	(uint8 *) dbInfoPtr,
    				sizeof(strEthRmtSeqNumChkDBInfo)
				);
}



uint32  rmtComInfoQueClear(void)
{
    uint32  status = NO_ERROR;
	strRmtDevSendMsgInfo		*comDataInfoPtr = NULL;

    if(sRmtComQue.usedList == NULL) {
        goto MODULE_END;
    }

    
    if( (
            status = semaphoreGet(sRmtComQSemaId, SEMA_GET_TIMEOUT)
        ) > 0
      ) {
        setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
        goto MODULE_END;
    }

    while(sRmtComQue.usedList != NULL) {
        
        comDataInfoPtr = (strRmtDevSendMsgInfo *)nodeGetAndUnlink (
                            (strDoubleLink **)&sRmtComQue.usedList
                                                    );

        if(comDataInfoPtr != NULL) {
			memoryClear ( 	(uint8 *)comDataInfoPtr,
							sizeof(strRmtDevSendMsgInfo)
						); 
            
            (void)nodePutOnLast( (strDoubleLink **) &sRmtComQue.freeList,
                                 (strDoubleLink *)comDataInfoPtr
                               );
        }
     }
     
#ifdef VXWORKS
	semaphoreRelease( sRmtComQSemaId);  
#else     
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sRmtComQSemaId);  
#endif
MODULE_END:
    return(status);
}


uint32  rmtDualComInfoQueClear(void)
{
    uint32  status = NO_ERROR;
	strEthComDualMsgInfo		*comDataInfoPtr = NULL;

    if(sRmtDualComQue.usedList == NULL) {
        goto MODULE_END;
    }

    
    
    if( (
            status = semaphoreGet(sRmtDualComQSemaId, SEMA_GET_TIMEOUT)
        ) > 0
      ) {
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
        goto MODULE_END;
    }

    while(sRmtDualComQue.usedList != NULL) {
        
        comDataInfoPtr = (strEthComDualMsgInfo *)nodeGetAndUnlink (
                            (strDoubleLink **)&sRmtDualComQue.usedList
                                                    );

        if(comDataInfoPtr != NULL) {
			memoryClear ( 	(uint8 *)comDataInfoPtr,
							sizeof(strEthComDualMsgInfo)
						); 
            
            (void)nodePutOnLast( (strDoubleLink **) &sRmtDualComQue.freeList,
                                 (strDoubleLink *)comDataInfoPtr
                               );
        }
     }
     
#ifdef VXWORKS
	semaphoreRelease( sRmtDualComQSemaId);  
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sRmtDualComQSemaId);  
#endif
MODULE_END:
    return(status);
}

void ethRmtFbComSeqNoChkDBDisplay (uint32 arg1, uint32 arg2) 
{
	uint32	dbFbCode,dbFbBlkAddr, dbRcvCnt, taskId, dbSeqErrCnt,fbNum;
	strEthRmtSeqNumChkDBInfo	*dbInfoPtr;

    dbInfoPtr = &sEthRmtComSeqNumChkDBInfo[0];
 
	printf( "\n");
	printf( "----------------------------------------\n");
	printf( "**        RMT COM FB DB DISPLAY\n");
	printf( "----------------------------------------\n");

	for(taskId = 0; taskId < MAX_LOGIC_TASK_NUM; taskId++, dbInfoPtr++) {
    	for(fbNum = 0; fbNum < MAX_ETH_COM_FB_CODE_NUM; fbNum++) {
			dbFbCode 		= dbInfoPtr->fbId[fbNum];
			dbFbBlkAddr 	= dbInfoPtr->fbCodeBlockAddr[fbNum];
			dbRcvCnt 		= dbInfoPtr->rcvCount[fbNum];
			dbSeqErrCnt		= dbInfoPtr->seqErrCount[fbNum];

			if( (dbFbCode != 0) && (dbFbBlkAddr != 0))
				printf( "[TASK ID(%d)]-[FB ADDR(%04d)]-[FB CODE(%d)]-[RCV CNT(%05d)]-[SEQ ERR(%05d)]\n",
						taskId+1,dbFbBlkAddr, dbFbCode,dbRcvCnt, dbSeqErrCnt 
					);
		}
	} 
	printf( "\n");
}


void remoteDevStatusDisplay(uint32 arg1, uint32 arg2)
{
	while( !keyHit() ) {

       	sRemoteDevStatusDisplayFlag = DEBUG_ENABLE;
        sysTaskDelay(systemClockRateGet());
    }
   	sRemoteDevStatusDisplayFlag = DEBUG_DISABLE;
}


void plcStatusRespDisplay(void)
{
	while( !keyHit() ) {

       	sPlcStatusRespDisplayFlag = DEBUG_ENABLE;
        sysTaskDelay(systemClockRateGet());
    }
   	sPlcStatusRespDisplayFlag = DEBUG_DISABLE;
}

uint32 rmtDevProtocolEncode (	strNewNetProtocolInfo    *protInfoPtr,
								strDataBuf      *dataBufInfoPtr
							)
{
	uint32 status = NO_ERROR;
	uint32	opCode = 0, optionCode = 0;
	strEthCommPathReadReqInfo	*statusReadReqInfoPtr =NULL;
	strEthDataReadReqInfo		*dataReadReqInfoPtr = NULL;
	strEthDataWriteReqInfo		*dataWriteReqInfoPtr = NULL;
	uint32	deviceType = 0, blockAddrOffSet = 0;
    uint8 byteDataBuf[256];
	uint16	byteNum = 0, cvtByteNum = 0, i, shortDataBuf[256];

	uint16  convShortDataBuf[256];

	strDataAccReqInfo   reqAccBufInfo;
    strDataInfo         encodeDataBufInfo;
	uint32 cmdCode;

#ifdef _WIN32
	uint32 tempWriteData = 0;
#endif
	strMBDataAccReqInfo	modbusReqAccBufInfo;

	memoryCopy((uint8*)&cmdCode,(uint8*)&protInfoPtr->dataBuff[0],DOUBLE_SIZE);
    opCode 		= cmdCode>>16;
    optionCode	= cmdCode& 0xffff;

    switch(opCode) {
		case ETHERNET_DATA_ACC_OP:
        	if(optionCode == ETHERNET_DATA_READ) {
				dataReadReqInfoPtr = (strEthDataReadReqInfo *) &protInfoPtr->dataBuff[4];
                deviceType = dataReadReqInfoPtr->deviceType;

                
                if(sRmtComCountSetFlag == DEBUG_ENABLE)
                	rmtComCountSet(&gRmtComCountInfo.rmtDevDataReadComSendCount);
            }
            else if(optionCode == ETHERNET_DATA_WRITE) {
            	dataWriteReqInfoPtr = (strEthDataWriteReqInfo *)&protInfoPtr->dataBuff[4];
                deviceType = dataWriteReqInfoPtr->deviceType;
				
                
                if(sRmtComCountSetFlag == DEBUG_ENABLE)
                	rmtComCountSet(&gRmtComCountInfo.rmtDevDataWriteComSendCount);
            }
			else if(optionCode == ETHERNET_DATA_BROAD) 
			{
				
				deviceType = LK11_CPU;
				
                
                if(sRmtComCountSetFlag == DEBUG_ENABLE)
                	rmtComCountSet(&gRmtComCountInfo.rmtDevDataWriteComSendCount);
            }
			else
				status = L_OPTION_CODE_ERR;  
        break;
        case ETHERNET_COM_PATH_OP:

        	if(optionCode == ETHERNET_DEV_STATUS_READ) 
        	{
            	statusReadReqInfoPtr = (strEthCommPathReadReqInfo *)&protInfoPtr->dataBuff[4];
                deviceType = statusReadReqInfoPtr->deviceType;

                
                if(sRmtComCountSetFlag == DEBUG_ENABLE)
                	rmtComCountSet(&gRmtComCountInfo.rmtDevStatusReadComSendCount);
            }
			else
				status = L_OPTION_CODE_ERR; 
        break;
        default:
        	status = L_OP_CODE_ERR;
		break;
    }

    if(status == NO_ERROR) 
	{
		switch(deviceType) 
		{
			case LK11_CPU: 
            	dataBufInfoPtr->data_len = protInfoPtr->dataLength + MAX_NEW_FRAME_HEAD_LEN;
				memoryCopy ( 	(uint8 *)dataBufInfoPtr->data_buf,
								(uint8 *)protInfoPtr,
								 dataBufInfoPtr->data_len
							);
            break;

            default:
            	status = RMT_DEVICE_TYPE_ERR;
	            setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			break;
        }
    }
    return(status);
}


uint32 rmtDevMsgInfoSet	(   strDataBuf      *sendDataInfoPtr,
							strNewNetProtocolInfo    *protInfoPtr
						)
{
	uint32	status = NO_ERROR;
	strEthCommPathReadReqInfo	*statusReadReqInfoPtr;
	strEthDataReadReqInfo		*dataReadReqInfoPtr;
	strEthDataWriteReqInfo		*dataWriteReqInfoPtr;
	uint32	deviceType = 0, opCode, optionCode;
	strModbusTCPHeadInfo *modbusTcpHeadInfoPtr;
	uint16 sendId;
	uint32	lData,opCmd;
		
	memoryCopy((uint8*)&opCmd,(uint8*)&protInfoPtr->dataBuff[0],DOUBLE_SIZE);
	opCode 		= opCmd>>16; 
    optionCode	= opCmd & 0xffff;

    switch(opCode) {
		case ETHERNET_DATA_ACC_OP:
        	if(optionCode == ETHERNET_DATA_READ) {
				dataReadReqInfoPtr = (strEthDataReadReqInfo *) &protInfoPtr->dataBuff[4];
                deviceType = dataReadReqInfoPtr->deviceType;
            }
            else if(optionCode == ETHERNET_DATA_WRITE) {
            	dataWriteReqInfoPtr = (strEthDataWriteReqInfo *)  &protInfoPtr->dataBuff[4];
                deviceType = dataWriteReqInfoPtr->deviceType;
            }
			else
				status = L_OPTION_CODE_ERR;  
        break;
        case ETHERNET_COM_PATH_OP:
        	if(optionCode == ETHERNET_DEV_STATUS_READ) {
            	statusReadReqInfoPtr = (strEthCommPathReadReqInfo *)&protInfoPtr->dataBuff[4];
                deviceType = statusReadReqInfoPtr->deviceType;
            }
			else
				status = L_OPTION_CODE_ERR; 
        break;
        default:
        	status = L_OP_CODE_ERR;
		break;
    }

    return(status);
}


void eightByteConversion( uint32 *destPtr, uint32 *srcPtr, uint8 option)
{
	if(option == 1){ 
		longDataConversion( (uint32*)destPtr, (uint32*)srcPtr, DOUBLE_SIZE);
		longDataConversion( (uint32*)destPtr+1, (uint32*)srcPtr+1, DOUBLE_SIZE);
	}
	else { 
		memoryCopy((uint8*)destPtr,(uint8*)srcPtr,DOUBLE_SIZE);
		memoryCopy((uint8*)(destPtr+1),(uint8*)(srcPtr+1),DOUBLE_SIZE);
	}
#ifdef VXWORKS
	mySwap((uint32*)destPtr,(uint32*)(destPtr+1),DOUBLE_SIZE);
#endif 
}


uint32 rmtDevMsgInfoDel	(   strDataBuf      *sendDataInfoPtr,
							strNewNetProtocolInfo    *protInfoPtr
						)
{
	uint32	status = NO_ERROR;
	strEthCommPathReadReqInfo	*statusReadReqInfoPtr;
	strEthDataReadReqInfo		*dataReadReqInfoPtr;
	strEthDataWriteReqInfo		*dataWriteReqInfoPtr;
	uint32	deviceType = 0, opCode, optionCode;
	strModbusTCPHeadInfo	*modbusTcpHeadInfoPtr;
	uint16 sendId;
	uint32	lData;

    opCode 		= 0;
    optionCode	= 0;

    switch(opCode) {
		case ETHERNET_DATA_ACC_OP:
        	if((optionCode == ETHERNET_DATA_READ)) {
				dataReadReqInfoPtr = (strEthDataReadReqInfo *) protInfoPtr->dataBuff;
                deviceType = dataReadReqInfoPtr->deviceType;
            }
            else if(optionCode == ETHERNET_DATA_WRITE) {
            	dataWriteReqInfoPtr = (strEthDataWriteReqInfo *) protInfoPtr->dataBuff;
                deviceType = dataWriteReqInfoPtr->deviceType;
            }
			else
				status = L_OPTION_CODE_ERR;  
        break;
        case ETHERNET_COM_PATH_OP:
        	if((optionCode == ETHERNET_DEV_STATUS_READ)) {
            	statusReadReqInfoPtr = (strEthCommPathReadReqInfo *)protInfoPtr->dataBuff;
                deviceType = statusReadReqInfoPtr->deviceType;
            }
			else
				status = L_OPTION_CODE_ERR; 
        break;
        default:
        	status = L_OP_CODE_ERR;
		break;
    }

    return(status);
}

