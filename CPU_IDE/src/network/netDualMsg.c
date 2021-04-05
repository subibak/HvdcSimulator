
#include	<stdio.h>

#include    "usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include    "netProtocol.h"
#include	"usrSema.h"
#include	"cmdList.h"
#include	"network.h"
#include	"usrQueData.h"
#include	"msgQueData.h"
#include	"osDepend.h"
#include	"usrErrCode.h"

#include	"segScheduler.h"


#include	"protocolBuf.h"

#include	"rmt_Comm\ethModbus.h"

static int32   sDLMsgSemaId;
strDualLineMsgInfo  sDLMsgData[MAX_DLM_QUE_NUM];
static strDLMsgInfoMngr    sDLMsgMngrQue;

strPortInfo  sDLPortData[MAX_DLM_QUE_NUM];
static strPortInfoMngr  sDLPortInfoMngrQue;


extern uint32 gUniEWSSendSockFd_A;
extern uint32 gUniEWSSendSockFd_B;

extern uint32 gUniWDCSendSockFd_A;
extern uint32 gUniWDCSendSockFd_B;


extern uint32 gUniHSTSendSockFd_A;
extern uint32 gUniHSTSendSockFd_B;

extern uint32 gBroadSendRecvSockFd_A;
extern uint32 gBroadSendRecvSockFd_B;

extern uint32 gBroadEWSSendRecvSockFd_A;
extern uint32 gBroadEWSSendRecvSockFd_B;

extern uint32 gUniSendRecvSockFd_A;
extern uint32 gUniSendRecvSockFd_B;

extern strMBClntAndSlvDevInfo	gMBClntAndSlvDevInfo; 

extern int32	gDebugPortFd;

#ifdef _WIN32
	extern uint32 sSendAPortBind;
	extern uint32 sSendBPortBind;
#endif

uint32  dLMsgMngrInit(void);
uint32  dLMsgCheck (strMsgQueData *);
static void dLMsgQueLiveCountCheck (void);
void dLMsgQueCountDisplay(void);
void dLMsgQueDataDisplay(void);
uint32  dualLinePortInfoSet( strMsgQueData  *);
uint32 dualLinePortInfoGet(strinetAddr  *,strinetAddr  *,strNewNetProtocolInfo *);
static void dualLinePortInfoQueCheck(void);
uint32	comPortInfoGet(uint32,strinetAddr *, strinetAddr *);
void dlMsgQueueCountDisplay(void);

uint32 ethernetDataShareComInfoGet  (  	strinetAddr  *,
	        	         				strNewNetProtocolInfo *,
										uint32	
           				    		);


extern void memoryClear(uint8 *, uint32);
extern void memoryCopy(uint8 *, uint8 *, uint32);
extern int32 nodePutOnLast(strDoubleLink **,strDoubleLink *);
extern int32 nodeUnlink(strDoubleLink **,strDoubleLink *);
extern strDoubleLink *nextNodeRef(strDoubleLink *,strDoubleLink *);
extern strDoubleLink *nodeGetAndUnlink(strDoubleLink **);
extern int32 keyHit(void);
extern void ipAddrHexToString(int8 *, uint32 );
extern void setErrorCode(uint8 *, uint32, uint8 *, uint32);
extern int32 semaphoreCreate (strSemaInfo *, int32 *);
extern int32 semaphoreGet (int32, int32);
#ifdef VXWORKS
extern int32 semaphoreRelease (int32);
#else
extern int32 semaphoreRelease(int32,int32);
extern int32	semaphoreRelease2(uint8 *, uint32, int32, int32);
#endif
extern void sysTaskDelay(int32);
extern int32 systemClockRateGet(void);
extern uint32 sysBroadcastAddrGet(uint32, uint32 *);
extern uint32  remoteShelfAddrGet(uint32, uint32 *);


extern uint32 rmtDualComMsgCheck(strNewNetProtocolInfo *);


uint32  dLMsgMngrInit(void)
{
    uint32  status = NO_ERROR;
    uint32  loop = 0;
    strSemaInfo     sema;

    
#ifdef VXWORKS
    
    sema.semaType       = MUTEX_SEMA;
    sema.semaOptions    = MYSEM_Q_PRIORITY|
                          MYSEM_DELETE_SAFE|
                          MYSEM_INVERSION_SAFE;
#else
    sema.semaType       = MUTEX_SEMA;
	sema.semaInitVal	= FALSE;
	sema.semaName		= "DLMsg";

#endif
    
	status = semaphoreCreate(   (strSemaInfo *) &sema,
                                (int32 *)&sDLMsgSemaId );	

    if(status == NO_ERROR) {
        sDLMsgMngrQue.freeList = NULL;
        sDLMsgMngrQue.usedList = NULL;

        for( loop = 0; loop < MAX_DLM_QUE_NUM; loop++) {
            memoryClear (   (uint8 *) &sDLMsgData[loop],
                            sizeof(strDualLineMsgInfo)
                        );
            (void) nodePutOnLast(   (strDoubleLink **) &sDLMsgMngrQue.freeList,
                                    (strDoubleLink *) &sDLMsgData[loop]
                                );
        }
    }

    
    if(status == NO_ERROR) {
        sDLPortInfoMngrQue.freeList = NULL;
        sDLPortInfoMngrQue.usedList = NULL;

        for( loop = 0; loop < MAX_DLM_QUE_NUM; loop++) {
            memoryClear (   (uint8 *) &sDLPortData[loop],
                            sizeof(strPortInfo)
                        );
            (void) nodePutOnLast(   (strDoubleLink **) &sDLPortInfoMngrQue.freeList,
                                    (strDoubleLink *) &sDLPortData[loop]
                                );
        }
    }
    return(status);
}


void dlMsgQueueCountDisplay(void)
{
	uint32	freeListCount = 0;
    uint32	usedListCount = 0;
	uint32	freeListCount1 = 0;
    uint32	usedListCount1 = 0;
	uint32	status = NO_ERROR;
	strDualLineMsgInfo	*nodePtr;
	strPortInfo			*nodePtr1;

    status = semaphoreGet(sDLMsgSemaId, FOREVERWAIT);
	if(status != NO_ERROR) {
		printf( "[DL MSG Queue] - [Get Semaphore Error]\n");
        goto MODULE_END;
    }

	
	nodePtr = sDLMsgMngrQue.freeList;

    while(nodePtr != NULL) 
    {
    	freeListCount++;
        nodePtr = (strDualLineMsgInfo *)nextNodeRef(
								(strDoubleLink *)sDLMsgMngrQue.freeList,
	                        	(strDoubleLink *)nodePtr);
	}

	nodePtr = sDLMsgMngrQue.usedList;
    while(nodePtr != NULL) 
    {
    	usedListCount++;
        nodePtr = (strDualLineMsgInfo *)nextNodeRef( 
								(strDoubleLink *)sDLMsgMngrQue.usedList,
                                (strDoubleLink *)nodePtr);	
	} 

	
	nodePtr1 = sDLPortInfoMngrQue.freeList;

    while(nodePtr1 != NULL) 
    {
    	freeListCount1++;
        nodePtr1 = (strPortInfo *)nextNodeRef( 
								(strDoubleLink *)sDLPortInfoMngrQue.freeList,
	                        	(strDoubleLink *)nodePtr1);
	}

	nodePtr1 = sDLPortInfoMngrQue.usedList;
    while(nodePtr1 != NULL) 
    {
    	usedListCount1++;
        nodePtr1 = (strPortInfo *)nextNodeRef( 
								(strDoubleLink *)sDLPortInfoMngrQue.usedList,
                                (strDoubleLink *)nodePtr1);	
	} 

#ifdef VXWORKS
	semaphoreRelease( sDLMsgSemaId);
#else
	semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sDLMsgSemaId);
#endif
  	
    printf( "\t************[Dual Line Msg. Queue : %d]************\n",MAX_DLM_QUE_NUM);
	printf( "\tFreeList : %d		UsedList : %d\n",freeListCount, usedListCount);

	printf("\n");
	
    printf( "\t************[Dual Line Port Info. Queue : %d]*******\n",MAX_DLM_QUE_NUM);
	printf( "\tFreeList : %d		UsedList : %d\n",freeListCount1, usedListCount1);

MODULE_END:
	return;
}


uint32  dLMsgCheck (strMsgQueData   *recvQueDataPtr)
{
    uint32  status = NO_ERROR, dualMsgFlag = 0;
    uint32  ipAddr = 0;
    strDualLineMsgInfo  dLMsg;
    strDualLineMsgInfo  *dLMQuePtr      = NULL;
    strNewNetProtocolInfo    *protPtr    = NULL;
	uint32	queueCount = 0;
	uint32  opCmd, opCode, optionCode;

    ipAddr = recvQueDataPtr->peerAddr.iAddr;
    protPtr = (strNewNetProtocolInfo *)recvQueDataPtr->protoData.msgData;

	memoryClear((uint8*)&dLMsg,sizeof(strDualLineMsgInfo));
    memoryCopy((uint8*)&opCmd,(uint8*)&protPtr->dataBuff[0],DOUBLE_SIZE);

	opCode = opCmd>>16;
	optionCode = opCmd&0xffff;

    
    if( (opCode  == ETHERNET_DATA_ACC_OP) || (opCode  == ETHERNET_COM_PATH_OP)
	) {
		status = rmtDualComMsgCheck(protPtr);
		return(status);
	} 
    
    
    dLMsg.dLMsgLiveCount        = 0;
    dLMsg.dLMsgIpAddr           = ipAddr;
    dLMsg.dLMsgSeqNum           = protPtr->sequenceID;
    dLMsg.dLMsgSrcId            = protPtr->sourceID;
    dLMsg.dLMsgDestId           = protPtr->destinationID;

    dLMsg.dLMsgOpCode           = opCode;
    dLMsg.dLMsgOptionCode       = optionCode;

    
    if( ( status = semaphoreGet(sDLMsgSemaId, FOREVERWAIT) ) > 0) {
        setErrorCode( (uint8 *)__FILE__, __LINE__,(uint8 *)"dLMsgCheck", status);
    }

    else if(status == NO_ERROR) {
        
        

        dLMQuePtr = (strDualLineMsgInfo *) sDLMsgMngrQue.usedList;

        while(dLMQuePtr != NULL) {

            if  (
                    (dLMsg.dLMsgSeqNum == dLMQuePtr->dLMsgSeqNum)
                                        &&
                    (dLMsg.dLMsgSrcId == dLMQuePtr->dLMsgSrcId)
                                        &&
                    (dLMsg.dLMsgDestId == dLMQuePtr->dLMsgDestId)
                                        &&
                    (dLMsg.dLMsgOpCode == dLMQuePtr->dLMsgOpCode)
                                        &&
                    (dLMsg.dLMsgOptionCode == dLMQuePtr->dLMsgOptionCode)
                                        &&
                    (dLMsg.dLMsgIpAddr != dLMQuePtr->dLMsgIpAddr) 
					

                ){
                
                if  (   (
                        status = nodeUnlink (
                                (strDoubleLink **) &sDLMsgMngrQue.usedList,
                                (strDoubleLink *)dLMQuePtr
                                            )
                        ) == NO_ERROR
                    ) {
                    status = nodePutOnLast  (
                                (strDoubleLink **) &sDLMsgMngrQue.freeList,
                                (strDoubleLink *) dLMQuePtr
                                            );
                    if(status == NO_ERROR) {
                        status = DUAL_SAME_MSG_ERR;
                        dualMsgFlag = 1;
					}
                }

                
                break;
            }
			if(++queueCount > MAX_DLM_QUE_NUM) {
            	status = QUEUE_CRASH_ERR;
                break;
			} 			

            dLMQuePtr = (strDualLineMsgInfo *) nextNodeRef  (
                                    (strDoubleLink *)sDLMsgMngrQue.usedList,
                                    (strDoubleLink *)dLMQuePtr
                                                            );
        } 

        if((dLMQuePtr == NULL) && (status == NO_ERROR)){
            
            
            
            dLMQuePtr = (strDualLineMsgInfo *) nodeGetAndUnlink(
                            (strDoubleLink **)&sDLMsgMngrQue.freeList
                                                                );
            memoryCopy ( (uint8 *)dLMQuePtr,
                         (uint8 *)&dLMsg,
                         sizeof(strDualLineMsgInfo)
                        );
            if( dLMQuePtr != NULL) {
                (void)nodePutOnLast  (
                         (strDoubleLink **) &sDLMsgMngrQue.usedList,
                         (strDoubleLink *) dLMQuePtr
                                     );
            }
            else
                status = QUEUE_FULL_ERR;
        }
        else if((dLMQuePtr != NULL) && (dualMsgFlag == 1)){
            
            
            

            if(protPtr->messageType == REQUEST){
                (void)dualLinePortInfoSet(recvQueDataPtr);
            }
        }
        
        (void)dLMsgQueLiveCountCheck();

        
#ifdef VXWORKS
        semaphoreRelease( sDLMsgSemaId );
#else
        semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA, sDLMsgSemaId );
#endif
    }

    return(status);
}


static void dLMsgQueLiveCountCheck (void)
{
    uint32  status = NO_ERROR;
    strDualLineMsgInfo  *dLMQuePtr      = NULL;
    uint32  unlinkTbl[MAX_DLM_QUE_NUM];
    uint32  unlinkQueNum = 0, index;

    dLMQuePtr = (strDualLineMsgInfo *) sDLMsgMngrQue.usedList;

    while(dLMQuePtr != NULL) {
        if(++dLMQuePtr->dLMsgLiveCount > MAX_DLM_LIVE_COUNT)
            unlinkTbl[unlinkQueNum++] = (uint32)dLMQuePtr;

        dLMQuePtr = (strDualLineMsgInfo *) nextNodeRef  (
                                    (strDoubleLink *)sDLMsgMngrQue.usedList,
                                    (strDoubleLink *)dLMQuePtr
                                                         );
    }

    for(index = 0; index < unlinkQueNum; index++) {
        
        if  (   (
                status = nodeUnlink (
                                (strDoubleLink **) &sDLMsgMngrQue.usedList,
                                (strDoubleLink *)unlinkTbl[index]
                                     )
                ) == NO_ERROR
        ){
            nodePutOnLast  (
                            (strDoubleLink **) &sDLMsgMngrQue.freeList,
                            (strDoubleLink *) unlinkTbl[index]
                            );

            
            status = DLM_LIVE_COUNT_ERR;
#if NOT_USED
            setErrorCode(__FILE__, __LINE__, (uint8 *) "dLMsgQUeLiveCountCheck", status);
#endif
        } 
    }
}



void dLMsgQueDataDisplay(void)
{
    strDualLineMsgInfo     *dLMsgInfoPtr = NULL;
    uint32  status = NO_ERROR;
    uint8   tmp[32];

    memoryClear( (uint8 *) tmp, 32);

    status = semaphoreGet(sDLMsgSemaId, FOREVERWAIT);

    if(status != NO_ERROR) goto MODULE_END;

    dLMsgInfoPtr = (strDualLineMsgInfo *)sDLMsgMngrQue.usedList;

    if(dLMsgInfoPtr!= NULL)
        printf( "\n\t*************** DL MSG INFO QUEUE DATA ******************\n");
    else
        printf( "\n\t***************DL MSG INFO QUEUE DATA  NONE !!******************\n");

    while((dLMsgInfoPtr != NULL) && !keyHit()) {
        printf( "\t      DL Msg Live Count : %d\n", dLMsgInfoPtr->dLMsgLiveCount);

        ipAddrHexToString((int8 *)tmp, dLMsgInfoPtr->dLMsgIpAddr);
        printf( "\t         DL Msg Ip Addr : %s\n", tmp);
        printf( "\t         DL Msg Op Code : %d\n", dLMsgInfoPtr->dLMsgOpCode);
        printf( "\t     DL Msg Option Code : %d\n", dLMsgInfoPtr->dLMsgOptionCode);
        printf( "\t     DL MsgSequence Num : 0x%x\n\n", dLMsgInfoPtr->dLMsgSeqNum);

        
        sysTaskDelay(systemClockRateGet()/100);

        dLMsgInfoPtr = (strDualLineMsgInfo *)nextNodeRef    (
                                    (strDoubleLink *)sDLMsgMngrQue.usedList,
                                    (strDoubleLink *)dLMsgInfoPtr
                                                             );
    }

#ifdef VXWORKS
    semaphoreRelease(sDLMsgSemaId);
#else
    semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sDLMsgSemaId);
#endif

MODULE_END:
	return;
}


uint32  dualLinePortInfoSet( strMsgQueData  *msgPtr)
{

    uint32  status = NO_ERROR;
    strPortInfo     *portInfoPtr;
    strNewNetProtocolInfo    *dataPtr;
    strinetAddr             *addrPtr;
	uint32 *cmdCode, opCode, optionCode;

    dataPtr = (strNewNetProtocolInfo *)msgPtr->protoData.msgData;
    addrPtr = (strinetAddr *)&msgPtr->peerAddr;

    if(sDLPortInfoMngrQue.freeList == NULL) {
        status = QUEUE_FULL_ERR;
        setErrorCode((uint8 *)__FILE__, __LINE__, (uint8 *)"dualLinePortInfoSet", status);
        goto MODULE_END;
    }
    else if( (htons(addrPtr->port) != UNI_EWS_UDP_RECV_PORT_A) &&
             (htons(addrPtr->port) != UNI_EWS_UDP_RECV_PORT_B) &&
			 (htons(addrPtr->port) != UNI_WDC_UDP_RECV_PORT_A) &&
             (htons(addrPtr->port) != UNI_WDC_UDP_RECV_PORT_B) &&
             
			 (htons(addrPtr->port) != UNI_HST_UDP_RECV_PORT_A) &&
             (htons(addrPtr->port) != UNI_HST_UDP_RECV_PORT_B)
           )
        goto MODULE_END;

	
    cmdCode 	= (uint32*)(dataPtr->dataBuff);
 	opCode 		= (*cmdCode)>>16;
	optionCode	= (*cmdCode)&0xffff;

    if((status == NO_ERROR)) {
        
        portInfoPtr = (strPortInfo *)nodeGetAndUnlink (
                            (strDoubleLink **)&sDLPortInfoMngrQue.freeList
                                                    );

        if(portInfoPtr != NULL) {
            
            portInfoPtr->mngrCount      = 0;

            if(htons(addrPtr->port) == UNI_EWS_UDP_RECV_PORT_A){
                portInfoPtr->recvSockFd     = gUniEWSSendSockFd_A;
                portInfoPtr->recvPortNum    = (uint32)UNI_EWS_UDP_SEND_PORT_A;
            }
            else if(htons(addrPtr->port) == UNI_EWS_UDP_RECV_PORT_B){
                portInfoPtr->recvSockFd     = gUniEWSSendSockFd_B;
                portInfoPtr->recvPortNum    = (uint32)UNI_EWS_UDP_SEND_PORT_B;
            }
			else if(htons(addrPtr->port) == UNI_WDC_UDP_RECV_PORT_A){
                portInfoPtr->recvSockFd     = gUniWDCSendSockFd_A;
                portInfoPtr->recvPortNum    = (uint32)UNI_WDC_UDP_SEND_PORT_A;
            }
            else if(htons(addrPtr->port) == UNI_WDC_UDP_RECV_PORT_B){
                portInfoPtr->recvSockFd     = gUniWDCSendSockFd_B;
                portInfoPtr->recvPortNum    = (uint32)UNI_WDC_UDP_SEND_PORT_B;
            }
            
			else if(htons(addrPtr->port) == UNI_HST_UDP_RECV_PORT_A){
                portInfoPtr->recvSockFd     = gUniHSTSendSockFd_A;
                portInfoPtr->recvPortNum    = (uint32)UNI_HST_UDP_SEND_PORT_A;
            }
            else if(htons(addrPtr->port) == UNI_HST_UDP_RECV_PORT_B){
                portInfoPtr->recvSockFd     = gUniHSTSendSockFd_B;
                portInfoPtr->recvPortNum    = (uint32)UNI_HST_UDP_SEND_PORT_B;
            }
			
            portInfoPtr->recvIpAddr     = addrPtr->iAddr; 
            

            
            portInfoPtr->recvOpCode     = opCode;
            portInfoPtr->recvOptionCode = optionCode;
            portInfoPtr->recvSeqNum     = dataPtr->sequenceID;

            
            (void)nodePutOnLast( (strDoubleLink **) &sDLPortInfoMngrQue.usedList,
                                 (strDoubleLink *)portInfoPtr
                               );
        }
        else {
            status = QUEUE_FULL_ERR;
            setErrorCode((uint8 *)__FILE__, __LINE__, (uint8 *)"dualLinePortInfoSet", status);
        }
    }
MODULE_END:
    return(status);
}


uint32 dualLinePortInfoGet  (   strinetAddr  *tempAddrPtr,
							    strinetAddr  *peerAddrPtr,
                                strNewNetProtocolInfo *lgProtPtr
                            )
{
    uint32  status = NO_ERROR, broadAddr;
    strPortInfo     *portInfoPtr = NULL;
    uint32  *cmdCode, opCode,optionCode;

	
    cmdCode = (uint32*)(lgProtPtr->dataBuff);
 	opCode = (*cmdCode)>>16;
	optionCode = (*cmdCode)&0xffff;

    if(lgProtPtr->messageType == RESPONSE) {
        
        
        
        if(sDLPortInfoMngrQue.usedList == NULL) {
			status = DUAL_LINE_PORT_INFO_ERR;
			goto MODULE_END;
        }

		status = semaphoreGet(sDLMsgSemaId, FOREVERWAIT);
        if(status != NO_ERROR){
            setErrorCode((uint8 *)__FILE__, __LINE__, (uint8 *)"dualLinePortInfoGet", status);
            goto MODULE_END;
        }

        
        portInfoPtr = (strPortInfo *)sDLPortInfoMngrQue.usedList;

        while(portInfoPtr != NULL) {
            if (portInfoPtr->recvSeqNum == lgProtPtr->sequenceID){
                if(     (portInfoPtr->recvOpCode == opCode)
                                        &&
                        (portInfoPtr->recvOptionCode == optionCode)
                  ) {
                    
                    peerAddrPtr->type   = AF_INET;
                    peerAddrPtr->port   = htons((uint16)portInfoPtr->recvPortNum);
                    peerAddrPtr->iAddr  = portInfoPtr->recvIpAddr;
                    peerAddrPtr->sockFd = portInfoPtr->recvSockFd;

                    
                    if( (status = nodeUnlink(
                                    (strDoubleLink **)&sDLPortInfoMngrQue.usedList,
                                    (strDoubleLink *)portInfoPtr
                                            )
                         ) == NO_ERROR
                       ) {
                        status = nodePutOnLast (
                                    (strDoubleLink **) &sDLPortInfoMngrQue.freeList,
                                    (strDoubleLink *)portInfoPtr
                                              );
                    }
                    
                    break;
                }
                else { 
                    status = DUAL_LINE_QUEUE_CONTENT_ERR;
                    
                    break;
                }
            }

            portInfoPtr = (strPortInfo *)nextNodeRef (
                                (strDoubleLink *)sDLPortInfoMngrQue.usedList,
                                (strDoubleLink *)portInfoPtr
                                                      );
        }

        
        
        if(portInfoPtr == NULL) {
				status = 1;
        }
        
#ifdef VXWORKS
        semaphoreRelease (sDLMsgSemaId);
#else
        semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sDLMsgSemaId);
#endif

    }


    
    else if(lgProtPtr->messageType == REQUEST) {
		
   	    
		
		if(	(opCode == ETHERNET_DATA_ACC_OP) || (opCode == ETHERNET_COM_PATH_OP) )
			status = ethernetDataShareComInfoGet(	peerAddrPtr,
           											lgProtPtr,
                   	                                NET_LINE_LINK_B
												);
		else 
			status = MSG_OPCODE_ERR;

		if(	(status != NO_ERROR) && 
			( (opCode != ETHERNET_DATA_ACC_OP) && 
			  (opCode != ETHERNET_COM_PATH_OP) ) 
   	    ){
	        setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		}
    }
    else { 
        if(lgProtPtr->messageType == BROADCAST){ 
            peerAddrPtr->type   = AF_INET;
            
			if(tempAddrPtr->port == htons(BROAD_UDP_SEND_RECV_PORT_A) ){
				peerAddrPtr->port   = htons(BROAD_UDP_SEND_RECV_PORT_B); 
				peerAddrPtr->sockFd = gBroadSendRecvSockFd_B;
			}
			else if(tempAddrPtr->port == htons(BROAD_EWS_SEND_RECV_PORT_A) ){ 
				peerAddrPtr->port   = htons(BROAD_EWS_SEND_RECV_PORT_B); 
				peerAddrPtr->sockFd = gBroadEWSSendRecvSockFd_B;
			}

            if( (
                    status = sysBroadcastAddrGet(NET_LINE_LINK_B,
                                                  (uint32 *)&broadAddr
                                                )
                ) > 0
              ) {
                setErrorCode(__FILE__, __LINE__,  __FUNCTION__, status);
                goto MODULE_END;
            }

            peerAddrPtr->iAddr  = broadAddr;
        }

        else {
            status = 1;
        }
    }

MODULE_END:

    if(	semaphoreGet(sDLMsgSemaId, FOREVERWAIT) == 	NO_ERROR) {
        dualLinePortInfoQueCheck();
#ifdef VXWORKS
		semaphoreRelease (sDLMsgSemaId);
#else
		semaphoreRelease2(__FILE__,__LINE__,MUTEX_SEMA,sDLMsgSemaId);
#endif
    }
    else
		setErrorCode(__FILE__, __LINE__,  __FUNCTION__, status);

   return(status);
}


static void dualLinePortInfoQueCheck(void)
{
    strPortInfo     *portMngrPtr = NULL;
    uint32  status = NO_ERROR;
    uint32  unlinkTbl[MAX_PORTINFO_QUE_NUM];
    uint32  unlinkQueNum = 0, index;

    portMngrPtr = (strPortInfo *)sDLPortInfoMngrQue.usedList;

    while(portMngrPtr != NULL) {
        if(portMngrPtr->mngrCount++ > MAX_DLM_LIVE_COUNT)
            unlinkTbl[unlinkQueNum++] = (uint32)portMngrPtr;

        portMngrPtr = (strPortInfo *)nextNodeRef (
                                (strDoubleLink *)sDLPortInfoMngrQue.usedList,
                                (strDoubleLink *)portMngrPtr
                                                  );
    }

    for(index = 0; index < unlinkQueNum; index++) {
        if( (status = nodeUnlink(
                            (strDoubleLink **)&sDLPortInfoMngrQue.usedList,
                            (strDoubleLink *)unlinkTbl[index]
                                 )
            ) == NO_ERROR
        ){
            status = nodePutOnLast (
                             (strDoubleLink **) &sDLPortInfoMngrQue.freeList,
                             (strDoubleLink *)unlinkTbl[index]
                                    );
        }

        portMngrPtr = (strPortInfo *)unlinkTbl[index];

    }

    if(status)
		setErrorCode(__FILE__, __LINE__,  __FUNCTION__, status);

    return;
}


uint32	comPortInfoGet
					(
				uint32	msgType,
                strinetAddr	*srcAddrPtr,
                strinetAddr	*destAddrPtr
                	)
{
	uint32	status = NO_ERROR;
	uint32	broadAddr;
    
    switch(msgType) {
		case REQUEST:
			status = L_MSG_TYPE_ERR;
       	break;
        case RESPONSE:
        	destAddrPtr->type	= AF_INET;
        	
            if(htons(srcAddrPtr->port) == UNI_EWS_UDP_RECV_PORT_A) 
            {
				destAddrPtr->sockFd = gUniEWSSendSockFd_A;
                destAddrPtr->port	= htons(UNI_EWS_UDP_SEND_PORT_A);
                destAddrPtr->iAddr	= srcAddrPtr->iAddr;  
            }
            else if(htons(srcAddrPtr->port) == UNI_EWS_UDP_RECV_PORT_B)
            {
				destAddrPtr->sockFd = gUniEWSSendSockFd_B;
                destAddrPtr->port	= htons(UNI_EWS_UDP_SEND_PORT_B);
                destAddrPtr->iAddr	= srcAddrPtr->iAddr;  
            }

			else if(htons(srcAddrPtr->port) == UNI_WDC_UDP_RECV_PORT_A) 
			{
				destAddrPtr->sockFd = gUniWDCSendSockFd_A;
                destAddrPtr->port	= htons(UNI_WDC_UDP_SEND_PORT_A);
                destAddrPtr->iAddr	= srcAddrPtr->iAddr;  
            }
            else if(htons(srcAddrPtr->port) == UNI_WDC_UDP_RECV_PORT_B)
            {
				destAddrPtr->sockFd = gUniWDCSendSockFd_B;
                destAddrPtr->port	= htons(UNI_WDC_UDP_SEND_PORT_B);
                destAddrPtr->iAddr	= srcAddrPtr->iAddr;  
            }
			
			else if(htons(srcAddrPtr->port) == UNI_HST_UDP_RECV_PORT_A) 
			{
				destAddrPtr->sockFd = gUniHSTSendSockFd_A;
                destAddrPtr->port	= htons(UNI_HST_UDP_SEND_PORT_A);
                destAddrPtr->iAddr	= srcAddrPtr->iAddr;  
            }
            else if(htons(srcAddrPtr->port) == UNI_HST_UDP_RECV_PORT_B)
            {
				destAddrPtr->sockFd = gUniHSTSendSockFd_B;
                destAddrPtr->port	= htons(UNI_HST_UDP_SEND_PORT_B);
                destAddrPtr->iAddr	= srcAddrPtr->iAddr;  
            }
			else if(htons(srcAddrPtr->port) == UNI_UDP_SEND_RECV_PORT_A) 
			{
				destAddrPtr->sockFd = gUniSendRecvSockFd_A;
                destAddrPtr->port	= htons(UNI_UDP_SEND_RECV_PORT_A);
                destAddrPtr->iAddr	= srcAddrPtr->iAddr;  
            }
            else if(htons(srcAddrPtr->port) == UNI_UDP_SEND_RECV_PORT_B)
            {
				destAddrPtr->sockFd = gUniSendRecvSockFd_B;
                destAddrPtr->port	= htons(UNI_UDP_SEND_RECV_PORT_B);
                destAddrPtr->iAddr	= srcAddrPtr->iAddr;  
            }
            else
				status = COM_PORT_NUM_ERR;
        break;
        case BROADCAST:
			if(srcAddrPtr == NULL)
			{
				destAddrPtr->type   = AF_INET;
				destAddrPtr->port   = htons(BROAD_UDP_SEND_RECV_PORT_A); 
				destAddrPtr->sockFd = gBroadSendRecvSockFd_A;
			}
			else if(htons(srcAddrPtr->port) == BROAD_EWS_SEND_RECV_PORT_A)
			{
				destAddrPtr->type   = AF_INET;
				destAddrPtr->port   = htons(BROAD_EWS_SEND_RECV_PORT_A); 
				destAddrPtr->sockFd = gBroadEWSSendRecvSockFd_A;
			}
    		
            if( (
                    status = sysBroadcastAddrGet(NET_LINE_LINK_A,
                                                  (uint32 *)&broadAddr
                                                )
                ) > 0
              ) {
				setErrorCode(__FILE__, __LINE__,  __FUNCTION__, status);
                goto MODULE_END;
            }
            destAddrPtr->iAddr  = broadAddr;
        break;
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
		case RMTREQST:
        	status = L_MSG_TYPE_ERR;
        break;
#endif
        default:
        	status = L_MSG_TYPE_ERR;
        break;
    }

MODULE_END:

	if(status)
		setErrorCode(__FILE__, __LINE__,  __FUNCTION__, status);
		
    return(status);
}


uint32 ethernetDataShareComInfoGet  (  	strinetAddr  *peerAddrPtr,
	        	         				strNewNetProtocolInfo *lgProtPtr,
										uint32	lineKind
           				    		)
{
    uint32  status = NO_ERROR;
	uint32	deviceType = 0, portNum = 0, ipAddr = 0;
	int32 sockFd = 0;
	strEthCommPathReadReqInfo	*comPathReqInfoPtr;	
	strEthDataReadReqInfo		*dataReadReqInfoPtr;
	strEthDataWriteReqInfo		*dataWriteReqInfoPtr;
	uint32 *cmdCode, opCode, optionCode;
	uint32 stationId = 0;
    uint32 mbStationId, mbSlaveLoop, mbLineLoop;
	strModbusTcpDevInfo * ptrSlaveDeviceListInfo = NULL;

	cmdCode = (uint32*)(lgProtPtr->dataBuff);
 	opCode = (*cmdCode)>>16;
	optionCode = (*cmdCode)&0xffff;

	stationId = (lgProtPtr->destinationID&0xFFFF);
	
	if((opCode == ETHERNET_DATA_ACC_OP) && ((optionCode == ETHERNET_DATA_READ) || (optionCode == ETHERNET_DATA_WRITE)))
	{
		dataReadReqInfoPtr = (strEthDataReadReqInfo *)&lgProtPtr->dataBuff[4];

		deviceType = dataReadReqInfoPtr->deviceType;
	}

	switch(opCode) 
	{
		case ETHERNET_DATA_ACC_OP:
    		if(optionCode == ETHERNET_DATA_READ) 
    		{
				dataReadReqInfoPtr = (strEthDataReadReqInfo *)&lgProtPtr->dataBuff[4];
				deviceType = dataReadReqInfoPtr->deviceType;

                if(lineKind == NET_LINE_LINK_A) 
                {
	                
                	ipAddr = dataReadReqInfoPtr->destLineAIpAddr;
#if NOT_USED
					
					if(ipAddr ==0x0) 
                    	status = IP_ADDR_NULL_ERR;
#endif
					if(deviceType == LK11_CPU)
					{ 
						
        	        	portNum = UNI_UDP_SEND_RECV_PORT_A;

                        
                        sockFd = gUniSendRecvSockFd_A;
					}
                	else
                		status = RMT_DEVICE_TYPE_ERR;
				}
                else if(lineKind == NET_LINE_LINK_B) 
                {
	                
                	ipAddr = dataReadReqInfoPtr->destLineBIpAddr;
#if NOT_USED
					
					if(ipAddr ==0x0) 
                    	status = IP_ADDR_NULL_ERR;
#endif
					if(deviceType == LK11_CPU)
					{ 
						
        	        	portNum = UNI_UDP_SEND_RECV_PORT_B;

                        
                        sockFd = gUniSendRecvSockFd_B;
					}
                	else
                		status = RMT_DEVICE_TYPE_ERR;
				}

			}     	
    		else if(optionCode == ETHERNET_DATA_WRITE) 
    		{
				dataWriteReqInfoPtr = (strEthDataWriteReqInfo *)&lgProtPtr->dataBuff[4];
				deviceType = dataWriteReqInfoPtr->deviceType;

                if(lineKind == NET_LINE_LINK_A) 
                {
	                
                	ipAddr = dataWriteReqInfoPtr->destLineAIpAddr;
#if NOT_USED
					
					if(ipAddr ==0x0) 
                    	status = IP_ADDR_NULL_ERR;
#endif
					if(deviceType == LK11_CPU)
					{ 	
        	        	portNum = UNI_UDP_SEND_RECV_PORT_A;

                        
                        sockFd = gUniSendRecvSockFd_A;
					}
                	else
                		status = RMT_DEVICE_TYPE_ERR;
				}
                else if(lineKind == NET_LINE_LINK_B) 
                {
	                
                	ipAddr = dataWriteReqInfoPtr->destLineBIpAddr;

#if NOT_USED
					
					if(ipAddr ==0x0) 
                    	status = IP_ADDR_NULL_ERR;
#endif
					if(deviceType == LK11_CPU)
					{ 
						
        	        	portNum = UNI_UDP_SEND_RECV_PORT_B;

                        
                        sockFd = gUniSendRecvSockFd_B;
					}
                	else
                		status = RMT_DEVICE_TYPE_ERR;
				}
			
			}
		 	else if(optionCode == ETHERNET_DATA_BROAD) 
		 	{
                
                if(lineKind == NET_LINE_LINK_A) 
                {
					if( ( status = sysBroadcastAddrGet(lineKind,(uint32 *)&ipAddr) ) > 0) 		
					{
						status = IP_ADDR_NULL_ERR;
					}			
					
        	        portNum = BROAD_UDP_SEND_RECV_PORT_A;
                    
                    sockFd = gBroadSendRecvSockFd_A;
				}
                else if(lineKind == NET_LINE_LINK_B) {
					if( ( status = sysBroadcastAddrGet(lineKind,(uint32 *)&ipAddr) ) > 0) 		{
						status = IP_ADDR_NULL_ERR;
					}
					
        	        portNum = BROAD_UDP_SEND_RECV_PORT_B;

                    
                    sockFd = gBroadSendRecvSockFd_B;
				}
			}
			else
				status = L_OPTION_CODE_ERR;     	
        break;
        case ETHERNET_COM_PATH_OP:
    		if(optionCode == ETHERNET_DEV_STATUS_READ) {
				comPathReqInfoPtr = (strEthCommPathReadReqInfo *)&lgProtPtr->dataBuff[4];
				deviceType = comPathReqInfoPtr->deviceType;

                if(lineKind == NET_LINE_LINK_A) {
	                
                	ipAddr = comPathReqInfoPtr->destLineAIpAddr;

#if NOT_USED
					
					if(ipAddr ==0x0) 
                    	status = IP_ADDR_NULL_ERR;
#endif
					if(deviceType == LK11_CPU)
					{ 
						
        	        	portNum = UNI_UDP_SEND_RECV_PORT_A;

                        
                        sockFd = gUniSendRecvSockFd_A;
					}
                	else
                		status = RMT_DEVICE_TYPE_ERR;
				}
                else if(lineKind == NET_LINE_LINK_B) 
                {
	                
                	ipAddr = comPathReqInfoPtr->destLineBIpAddr;

#if NOT_USED
					
					if(ipAddr ==0x0) 
                    	status = IP_ADDR_NULL_ERR;
#endif
					if(deviceType == LK11_CPU)
					{ 
						
        	        	portNum = UNI_UDP_SEND_RECV_PORT_B;

                        
                        sockFd = gUniSendRecvSockFd_B;
					}
                	else
                		status = RMT_DEVICE_TYPE_ERR;
				}
			}
			else
				status = L_OPTION_CODE_ERR;     	
        break;
        default:
        	status = L_OP_CODE_ERR;
		break;
    }
	
    if(status == NO_ERROR) {
		
    	peerAddrPtr->type   = AF_INET;
    	peerAddrPtr->port   = htons((uint16)portNum);
#ifdef _WIN32
		 

		if( (opCode==ETHERNET_DATA_ACC_OP && optionCode == ETHERNET_DATA_BROAD ) ) {
			peerAddrPtr->iAddr  = ipAddr;
		}
    	else longDataConversion(&peerAddrPtr->iAddr,&ipAddr,DOUBLE_SIZE);
#else
    	peerAddrPtr->iAddr  = ipAddr;
#endif
    	peerAddrPtr->sockFd = sockFd;
    	
#ifdef _WIN32
		
		if((lineKind==NET_LINE_LINK_A)&&(sSendAPortBind == NOT_BINDING)){
			unicastSendPortBinding(NET_LINE_LINK_A);
		}
		else if((lineKind==NET_LINE_LINK_B)&&(sSendBPortBind == NOT_BINDING)){
			unicastSendPortBinding(NET_LINE_LINK_B);
		}
#endif
	}

	if(status)
	{
		setErrorCode(__FILE__, __LINE__,  __FUNCTION__, status);
	}

    return(status);
}
















