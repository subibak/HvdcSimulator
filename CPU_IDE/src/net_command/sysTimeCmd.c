
#include	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include	"segScheduler.h"

#include	"cmdList.h"
#include	"netProtocol.h"
#include	"protocolBuf.h"
#include	"usrDefBuf.h"
#include	"sysConfig.h"
#include	"usrErrCode.h"


uint32 pcsTimeWriteReqAndRespFunc
				(strNewNetProtocolInfo *,strBufInfo *);
uint32 pcsTimeReadReqAndRespFunc
				(strNewNetProtocolInfo *,strBufInfo *);
uint32 pcsTimeSyncReqAndRespFunc(strNewNetProtocolInfo 	*,strBufInfo *);


extern void memoryClear( uint8 *, uint32);
extern void memoryCopy(uint8 *, uint8 *, uint32);
extern void setErrorCode(uint8 *, uint32, uint8 *, uint32);
extern uint32 optionCodeCmdNullFunc(strNewNetProtocolInfo *,strBufInfo *);

extern uint32 timeDataCheck (strTimeDataInfo *);
extern uint32 systemTimeWrite( strTimeDataInfo *);
extern uint32 systemTimeRead (strTimeDataInfo *);
extern uint32 systemCnfgInfoRead(strSysConfigInfo *);
extern uint32 timeDataBroadcastToAllPort(strTimeDataInfo *);


uint32	(*sysTimeCmdOPFuncList[]) 
			(strNewNetProtocolInfo	*,strBufInfo *)
= {
	optionCodeCmdNullFunc,			
	pcsTimeSyncReqAndRespFunc,	
	pcsTimeReadReqAndRespFunc,	
	pcsTimeWriteReqAndRespFunc	
};


uint32 pcsTimeSyncReqAndRespFunc
				(
		strNewNetProtocolInfo 	*reqMsgPtr,
		strBufInfo			*respMsgPtr
	    		)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	strSysConfigInfo	sysCnfgInfo;
	strTimeDataInfo	*reqDataPtr;
	strTimeDataInfo	reqTimeDataInfo;

	
    reqDataPtr = (strTimeDataInfo *)&reqMsgPtr->dataBuff[4];
    
	
	memoryCopy((uint8 *)&reqTimeDataInfo,(uint8 *)reqDataPtr,(sizeof(strTimeDataInfo)-4));
	reqTimeDataInfo.mSec = 0;

    
    retVal = timeDataCheck ( &reqTimeDataInfo);

    if(retVal != NO_ERROR)
        setErrorCode ( 	(uint8 *)__FILE__,
        					(uint32)__LINE__,
                            (uint8 *)"timeSyncBroadcastCmd",
                            retVal
						);

	
	if(retVal == NO_ERROR) {
		retVal = systemTimeWrite( (strTimeDataInfo *)&reqTimeDataInfo);

		if(retVal != NO_ERROR)
	        setErrorCode ( 	(uint8 *)__FILE__,
    	    					(uint32)__LINE__,
        	                    (uint8 *)"timeSyncBroadcastCmd",
            	                retVal
							);
	}

	
    
    memoryClear( (uint8 *) &sysCnfgInfo, sizeof(strSysConfigInfo));

    systemCnfgInfoRead( (strSysConfigInfo *) &sysCnfgInfo);

    return(status);
}




uint32 pcsTimeReadReqAndRespFunc
				(
		strNewNetProtocolInfo 	*reqMsgPtr,
        strBufInfo			*respMsgPtr
    		)
{
	uint32	status = NO_ERROR, retVal;
	strTimeDataInfo	 *respDataPtr;	
	uint32	respDataLen = 0, cmdCode;
    uint32 *reqDataPtr = (uint32 *)reqMsgPtr->dataBuff;

    cmdCode = *reqDataPtr++;
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0],(uint8*)&cmdCode,DOUBLE_SIZE);

    
    respDataPtr = (strTimeDataInfo *)&respMsgPtr->dataBuf[8];
	respDataLen = DOUBLE_SIZE *2;

	retVal = systemTimeRead ( (strTimeDataInfo *)respDataPtr);
	if(retVal != NO_ERROR)
	{
        setErrorCode ( 	(uint8 *)__FILE__,
   	    					(uint32)__LINE__,
       	                    (uint8 *)"pcsTimeReadReqAndRespFunc",
           	                status
						);
	}
	else respDataLen += sizeof(strTimeDataInfo);
    memoryCopy((uint8*)&respMsgPtr->dataBuf[4],(uint8*)&retVal,DOUBLE_SIZE);
	

	respMsgPtr->dataLen = respDataLen;
	
    return(status);
}


uint32 pcsTimeWriteReqAndRespFunc
				(
		strNewNetProtocolInfo 	*reqMsgPtr,
        strBufInfo			*respMsgPtr
    		)
{
	uint32	status = NO_ERROR, retVal;
	strTimeDataInfo	*reqDataPtr;	
	uint32	respDataLen = 0;
	strTimeDataInfo	reqTimeDataInfo;

	memoryCopy((uint8*)&respMsgPtr->dataBuf[0],(uint8*)&reqMsgPtr->dataBuff[0],DOUBLE_SIZE);
	respDataLen = DOUBLE_SIZE;

    reqDataPtr = (strTimeDataInfo *)&reqMsgPtr->dataBuff[4];
	
	memoryCopy((uint8 *)&reqTimeDataInfo,(uint8 *)reqDataPtr,(sizeof(strTimeDataInfo)-4));
	reqTimeDataInfo.mSec = 0;

    
    retVal = timeDataCheck ( &reqTimeDataInfo);

    if(retVal != NO_ERROR)
        setErrorCode ( 	(uint8 *)__FILE__,
        					(uint32)__LINE__,
                            (uint8 *)"pcsTimeWriteReqAndRespFunc",
                            retVal
						);

	
	if(retVal == NO_ERROR) {
		retVal = systemTimeWrite( (strTimeDataInfo *)&reqTimeDataInfo);

		if(retVal != NO_ERROR)
	        setErrorCode ( 	(uint8 *)__FILE__,
    	    					(uint32)__LINE__,
        	                    (uint8 *)"pcsTimeWriteReqAndRespFunc",
            	                retVal
							);
	}

    memoryCopy((uint8*)&respMsgPtr->dataBuf[4],(uint8*)&retVal,DOUBLE_SIZE);
	respDataLen += DOUBLE_SIZE;

	respMsgPtr->dataLen = respDataLen;

    return(status);
}











