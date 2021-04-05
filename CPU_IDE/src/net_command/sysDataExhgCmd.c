
#include	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include	"sysConfig.h"
#include	"systemDiag.h"
#include	"fbMem.h"
#include	"netProtocol.h"

#include	"segScheduler.h"


#include	"protocolBuf.h"

#include	"usrErrCode.h"
#include	"network.h"
#include	"msgQueData.h"
#include	"usrQueData.h"
#include	"msgQue.h"
#include	"usrDefBuf.h"

#include	"sysDual.h"
#include	"rmt_comm\rmtDevUtil.h"



extern strSysRunningConfigInfo		*gSysRunningInfoPtr;


uint32 sysRmtDataReadReqAndRespFunc (strNewNetProtocolInfo *, strBufInfo*);
uint32 sysRmtDataWriteReqAndRespFunc (strNewNetProtocolInfo *, strBufInfo*);

uint32 sysRmtDataReadRespFunc (	strNewNetProtocolInfo *);
uint32 sysRmtDataWriteRespFunc (strNewNetProtocolInfo *);
uint32 sysRmtDataBroadCastFunc (strNewNetProtocolInfo *);

static uint32 sysRmtDataReadInSharedMemAndMakeResp(strNewNetProtocolInfo *, strBufInfo *);
static uint32 sysRmtDataReadRespWriteToSharedMem(strNewNetProtocolInfo	*);
static uint32 sysRmtDataWriteToSharedMemqAndMakeResp(strNewNetProtocolInfo *, strBufInfo *);
static uint32 sysRmtDataWriteRespFromSharedMem(strNewNetProtocolInfo	*);


extern void memoryClear(uint8 *, uint32);
extern void memoryCopy (uint8 *, uint8 *, uint32);
extern void setErrorCode(uint8 *, uint32, uint8 *, uint32);
extern uint32 optionCodeCmdNullFunc(strNewNetProtocolInfo *,strBufInfo *);
extern uint32 optionCodeCmdNullFunc2(strNewNetProtocolInfo *);

extern uint32 fbMemInfoRead(uint32, uint32, strFbMemInfo *);
extern uint32 fbMemPointerGet(uint32 , strMemPointerInfo *);
extern uint32 ethRmtFbComSeqNoChkFunc(uint32 , uint32 , uint32 , uint32 *);
extern uint32 busFbMemPointerGet(uint32 , strMemPointerInfo *);

extern uint32  refDataCopyToMem(uint32*, uint32, strFbSpecActionInfo, uint32);
extern uint32  refDataCopyFromMem(uint32*, uint32, strFbSpecActionInfo, uint32,uint32);
extern uint32  refDataCopyToBusMem(uint32*, uint32, strFbSpecActionInfo, uint32);

extern uint32 rmtSharedVarNumWrite(uint32 , uint32 , uint32 , uint32 *);
extern uint32 dualFlagCheckInRunShelf(void);

extern void setErrorCodeWithVal(uint8 *,uint32,uint8 *,uint32, uint8 *,uint32,
							uint8 *,uint32,uint8 *,uint32);
extern uint32	busFbMemPointerGet(uint32 , strMemPointerInfo *);
extern uint32	fbDataWriteToRemoteVmeMemBoard(uint32,uint32,uint32,uint32,uint32,strFbMemInfo	*);


uint32	(*sysRmtDataAccReqCmdOPFuncList[]) 
			(strNewNetProtocolInfo	*,strBufInfo *)
= {
	optionCodeCmdNullFunc,				
	sysRmtDataReadReqAndRespFunc,	
	sysRmtDataWriteReqAndRespFunc,	
	optionCodeCmdNullFunc				
};

uint32	(*sysRmtDataAccRespCmdOPFuncList[]) 
			(strNewNetProtocolInfo	*)
= {
	optionCodeCmdNullFunc2,				
	sysRmtDataReadRespFunc,			
	sysRmtDataWriteRespFunc,		
	sysRmtDataBroadCastFunc			
};


uint32 sysRmtDataReadReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo				*respMsgPtr
		)
{
	uint32	status = NO_ERROR;
	strEthDataReadReqInfo	*reqInfoPtr;
	
	
    reqInfoPtr = (strEthDataReadReqInfo *)&reqMsgPtr->dataBuff[4];

    
	status = sysRmtDataReadInSharedMemAndMakeResp (	reqMsgPtr,
					            					respMsgPtr
												  );
	if(status != NO_ERROR)
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status); 

    return(status);
}


static uint32 sysRmtDataReadInSharedMemAndMakeResp
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo				*respMsgPtr
		)
{
	uint32	status = NO_ERROR;
	uint32	bitMask, i, *dataPtr;
	strEthDataReadReqInfo	*reqInfoPtr;
	strEthDataReadRespInfo	respInfo;
	strFbSpecActionInfo specActionData;
	strSysRunningConfigInfo		*sysStatusInfoPtr = gSysRunningInfoPtr;

	uint32 *cmdCode,retData,specData,maxShrardMem=0;

	
    cmdCode = (uint32*)(&reqMsgPtr->dataBuff[0]);

	
    reqInfoPtr = (strEthDataReadReqInfo *)&reqMsgPtr->dataBuff[4];

	if(reqInfoPtr->srcFbBlockAddr == 0) 
	{
    	status = FB_ADDR_NULL_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
    }
    
	
    if(reqInfoPtr->deviceType != LK11_CPU)
	{ 
    	status = RMT_DEVICE_TYPE_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
    }

	if(reqInfoPtr->dataAccType == RMT_LONG_ACC) 
		maxShrardMem = MAX_SHARED_MEM_NUM;
	else if(reqInfoPtr->dataAccType== RMT_WORD_ACC) 
		maxShrardMem = MAX_SHARED_MEM_NUM *2;
	else if(reqInfoPtr->dataAccType== RMT_BYTE_ACC) 
		maxShrardMem = MAX_SHARED_MEM_NUM *4;
	else 
	{
	   	status = RMT_DATA_ACC_TYPE_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

    if(reqInfoPtr->numOfData > MAX_RMT_ST_ACC_DATA_NUM)
    {
    	status = RMT_DATA_ACC_NUM_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
    }

    
	if( (reqInfoPtr->destFbBlockAddr<0) || (reqInfoPtr->destFbBlockAddr >= maxShrardMem) )
	{
		status = VARIABLE_ADDR_RANGE_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

    
	if(sysStatusInfoPtr->sysRedundancyStatus != SYS_RAS_REDUNDANCY_RUN){
		status = RMT_ETH_DEV_REDUNDANCY_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

	
    if(reqInfoPtr->dataAccType == RMT_BYTE_ACC)
		bitMask = 0xff;
    else if(reqInfoPtr->dataAccType == RMT_WORD_ACC)
		bitMask = 0xffff;
    else
		bitMask = 0xffffffff;

	memoryClear((uint8*)&specActionData,sizeof(strFbSpecActionInfo)) ;
	
	specActionData.dataSize 	= (reqInfoPtr->dataAccType + 1);
	specActionData.refType 		= 0x00; 
	specActionData.memoryType 	= S_MEMORY;
	specActionData.bitPosition 	= 0;
	specActionData.bitposFlag 	= 0;

	dataPtr = (uint32 *)respInfo.readData;
 	retData = 0;
	for(i = 0; i < reqInfoPtr->numOfData; i++,  dataPtr++)
	{
		
		specData = 0;
		specData = (reqInfoPtr->destFbBlockAddr +i)<<8;
		status = refDataCopyFromMem(&retData,specData, specActionData, SIZE32_TYPE,0);
		*dataPtr = retData& bitMask;
	}

MODULE_END:
    
	
    memoryCopy ( 	(uint8 *)&respInfo,
    				(uint8 *)reqInfoPtr,
                    sizeof(strEthDataReadReqInfo)
				);

	if(status != NO_ERROR) {
	    
		respMsgPtr->dataLen = sizeof(strEthDataReadRespInfo);
		respMsgPtr->dataLen -= MAX_RMT_ST_ACC_DATA_NUM * 4;
		 
		respInfo.numOfData = 0;
	}
	else {
	    
		respMsgPtr->dataLen = sizeof(strEthDataReadRespInfo);
		respMsgPtr->dataLen -= MAX_RMT_ST_ACC_DATA_NUM * 4;
		respMsgPtr->dataLen += reqInfoPtr->numOfData * 4;
	}	
	
    memoryCopy ( (uint8 *) &respMsgPtr->dataBuf[4],
    			 (uint8 *) &respInfo,
                 respMsgPtr->dataLen
			  );
	
	
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0], (uint8*)cmdCode, DOUBLE_SIZE);
	respMsgPtr->dataLen += DOUBLE_SIZE;

    return(status);
}


uint32 sysRmtDataReadRespFunc(strNewNetProtocolInfo	*respMsgPtr)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	strFbMemInfo	fbMemInfo;
	strEthDataReadRespInfo	*respInfoPtr;	
	
	respInfoPtr = (	strEthDataReadRespInfo *)&respMsgPtr->dataBuff[4];
		
	retVal = fbMemInfoRead(	LOGIC_ID, 
							respInfoPtr->srcFbBlockAddr,
							&fbMemInfo
						  );
	if(retVal != NO_ERROR)
	{
		status = retVal;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
    }	
	
   	if((fbMemInfo.fbId == ETH_RMT_DATA_SEND_FB_CODE) || (fbMemInfo.fbId == MBT_RMT_DATA_SEND_FB_CODE))
		status = sysRmtDataReadRespWriteToSharedMem (respMsgPtr);
	
	else
		status = FB_CODE_ID_ERR;
	
	if(status != NO_ERROR)
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);

MODULE_END:
    return(status);
}

static uint32 sysRmtDataReadRespWriteToSharedMem(strNewNetProtocolInfo	*respMsgPtr)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	strFbMemInfo	fbMemInfo;	
	strEthDataReadRespInfo	*respInfoPtr;
	strMemPointerInfo	localMemInfo, vmeMemInfo;
	uint32	*saveVarPtr, *varPtr, *outputPtr, *dataPtr, i;	
	uint32 *cmdCode, specData, retData;
	strFbSpecActionInfo specActionData, *specActionPtr;
	strMemPointerInfo	memP;
	uint32 *specStartPtr,*specDataPtr, srcSharedMemAddr = 0;
	uint32	copyFlag = 0;
	uint32 dword;
	
    cmdCode 	= (uint32*)(&respMsgPtr->dataBuff[0]);
	respInfoPtr = (	strEthDataReadRespInfo *)&respMsgPtr->dataBuff[4];
	
	
    if(respInfoPtr->deviceType != LK11_CPU) 
	{
    	status = RMT_DEVICE_TYPE_ERR;
		setErrorCode (	__FILE__, __LINE__, __FUNCTION__, status);
    }

	if(	(respInfoPtr->dataAccType != RMT_BYTE_ACC) &&
		(respInfoPtr->dataAccType != RMT_WORD_ACC) &&
		(respInfoPtr->dataAccType != RMT_LONG_ACC) 
	){
		status = RMT_DATA_ACC_TYPE_ERR;
		setErrorCode (	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

    if(respInfoPtr->numOfData > MAX_RMT_ST_ACC_DATA_NUM)
    {
    	status = RMT_DATA_ACC_NUM_ERR;
		setErrorCode (	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
    }

	
	retVal = fbMemInfoRead(	LOGIC_ID, 
							respInfoPtr->srcFbBlockAddr,
							&fbMemInfo
						  );
	if(retVal != NO_ERROR)
	{
		status = retVal;
		setErrorCode (	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
    }

	if( (fbMemInfo.fbId != ETH_RMT_DATA_SEND_FB_CODE)
	    && (fbMemInfo.fbId != MBT_RMT_DATA_SEND_FB_CODE)   )
	{
		status = FB_CODE_MISMATCH_ERR;
		setErrorCode (	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}
	
	retVal = fbMemPointerGet(LOGIC_ID, (strMemPointerInfo	*)&memP);
	if(retVal != NO_ERROR){ 
		setErrorCode (	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}
	
	specStartPtr = (uint32 *)memP.paraMemPtr;

	
	specActionPtr = (strFbSpecActionInfo *)(specStartPtr + fbMemInfo.specActionIndex + 13);
	specDataPtr = (uint32 *)(specStartPtr + fbMemInfo.specIndex + 13);
	
	if(specActionPtr->refType == SPEC_TYPE_BYREF) 
	{
		retVal = refDataCopyFromMem(&retData,*specDataPtr, *specActionPtr, SIZE32_TYPE,0);
		if(retVal != NO_ERROR) {
			setErrorCode (	__FILE__, __LINE__, __FUNCTION__, status);
			goto MODULE_END;	
		}
		
		srcSharedMemAddr = retData;
	}
	else 
		srcSharedMemAddr = *specDataPtr;
	
	
	if(  ( respMsgPtr->dataLength 
		   - (sizeof(strEthDataReadReqInfo)+DOUBLE_SIZE) 
		 )
		 != (respInfoPtr->numOfData * DOUBLE_SIZE) 
	) {
		status = RMT_DATA_ACC_NUM_ERR; 
		setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;	
	}

	
	fbMemPointerGet(LOGIC_ID, &localMemInfo);
	busFbMemPointerGet(LOGIC_ID , &vmeMemInfo);

	
	varPtr 		= (uint32 *)localMemInfo.paraMemPtr;
	varPtr 		+= fbMemInfo.varIndex;
 	saveVarPtr 	= varPtr;

    
	varPtr 		= saveVarPtr;
    *varPtr 	= ETH_RMT_COM_RCV_DONE; 

    
    if(status != NO_ERROR)
	{
		varPtr 	= saveVarPtr + 1;
        *varPtr += 1;
	}
    else 
	{
		dataPtr = (uint32 *)respInfoPtr->readData;
   	
        varPtr = saveVarPtr;
 	
 		memoryClear((uint8*)&specActionData,sizeof(strFbSpecActionInfo)) ;
				
		specActionData.dataSize 	= (respInfoPtr->dataAccType + 1);
		specActionData.refType 		= 0x00; 
		specActionData.memoryType 	= S_MEMORY;
		specActionData.bitPosition 	= 0;
		specActionData.bitposFlag 	= 0;
		
        varPtr 		= saveVarPtr + 3;
        copyFlag 	= *varPtr;		

		for(i = 0; i < respInfoPtr->numOfData; i++, dataPtr++)
		{
			
			specData = (srcSharedMemAddr +i)<<8;

			retData = 0;
			retVal = refDataCopyFromMem(&retData,specData, specActionData, SIZE32_TYPE,0);
			if ( retVal) {
				setErrorCode (	__FILE__, __LINE__, __FUNCTION__, retVal);
				continue;			
			}

			if((retData != *dataPtr) || (copyFlag == 0)) 
			{
				
				retVal = refDataCopyToMem(dataPtr, specData, specActionData, SIZE32_TYPE);
				if(retVal)
					setErrorCode (	__FILE__, __LINE__, __FUNCTION__, retVal);

				
				else {
					retVal = refDataCopyToBusMem(dataPtr, specData, specActionData, SIZE32_TYPE);
			
					if(retVal)
						setErrorCode (	__FILE__, __LINE__, __FUNCTION__, retVal);
					else {
							
						if(dualFlagCheckInRunShelf() == DUAL_CHECK_ENABLE)
						{
							retVal = rmtSharedVarNumWrite(	respInfoPtr->dataAccType,
															1, 
															srcSharedMemAddr+i, 
															(uint32 *)dataPtr
														);
							if(retVal)
								setErrorCode (	__FILE__, __LINE__, __FUNCTION__, retVal);
						}
					}
				}
    		}
    	}

		
        varPtr = saveVarPtr + 2 ;
        *varPtr = respInfoPtr->numOfData;
	}

    
	outputPtr = (uint32 *)localMemInfo.outputMemPtr;
	
	/* 출력 : 수신 count */
    outputPtr += (fbMemInfo.outputIndex + 5); /* 수정 주의 */

    retVal = ethRmtFbComSeqNoChkFunc(	respInfoPtr->srcFbBlockAddr, 
										fbMemInfo.fbId, 
										respMsgPtr->sequenceID, 
										outputPtr
						   			);
	if(retVal != NO_ERROR) 
	{
		/* 내부 변수 : sequence 에러 카운터 */
        varPtr = saveVarPtr + 4;/* 수정 주의 */
        *varPtr += 1;
    }

	
	if(dualFlagCheckInRunShelf() == DUAL_CHECK_ENABLE) 
	{
		
        varPtr = saveVarPtr + 3; /* 수정 주의 */
        *varPtr = 1;		
	}
MODULE_END:
    return(status);
}


uint32 sysRmtDataWriteReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR;
	strEthDataWriteReqInfo	*reqInfoPtr;
	
    reqInfoPtr = (strEthDataWriteReqInfo *)&reqMsgPtr->dataBuff[4];

    
	status = sysRmtDataWriteToSharedMemqAndMakeResp (reqMsgPtr,	respMsgPtr);
	if(status != NO_ERROR)
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status); 

    return(status);
}


static uint32 sysRmtDataWriteToSharedMemqAndMakeResp
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR;
	uint32	bitMask, i, *dataPtr;
	strEthDataWriteReqInfo	*reqInfoPtr;
	strEthDataWriteRespInfo	respInfo;
	strFbSpecActionInfo specActionData;
	strSysRunningConfigInfo		*sysStatusInfoPtr = gSysRunningInfoPtr;
	uint32 *cmdCode, specData, retVal=0, maxShrardMem=0;
	uint32	sharedMemAddr, retData;
	
	
    cmdCode = (uint32*)(&reqMsgPtr->dataBuff[0]);

	
    reqInfoPtr = (strEthDataWriteReqInfo *)&reqMsgPtr->dataBuff[4];

	if(reqInfoPtr->srcFbBlockAddr == 0) 
	{
    	status = FB_ADDR_NULL_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
    }
    
	
    if(reqInfoPtr->deviceType != LK11_CPU)
	{ 
    	status = RMT_DEVICE_TYPE_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
    }


 	if(reqInfoPtr->dataAccType == RMT_LONG_ACC) 
 		maxShrardMem = MAX_SHARED_MEM_NUM;

	else if(reqInfoPtr->dataAccType == RMT_WORD_ACC) 
		maxShrardMem = MAX_SHARED_MEM_NUM *2;

	else if(reqInfoPtr->dataAccType == RMT_BYTE_ACC) 
		maxShrardMem = MAX_SHARED_MEM_NUM *4;

	else 
		{
	   	status = RMT_DATA_ACC_TYPE_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

    if(reqInfoPtr->numOfData > MAX_RMT_ST_ACC_DATA_NUM)
    {
    	status = RMT_DATA_ACC_NUM_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
    }

    
	if( (reqInfoPtr->destFbBlockAddr<0) || (reqInfoPtr->destFbBlockAddr >= maxShrardMem) )
	{
		status = VARIABLE_ADDR_RANGE_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

    
	if(sysStatusInfoPtr->sysRedundancyStatus != SYS_RAS_REDUNDANCY_RUN)
	{
		status = RMT_ETH_DEV_REDUNDANCY_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}
	
	
    if(reqInfoPtr->dataAccType == RMT_BYTE_ACC)
		bitMask = 0xff;
    else if(reqInfoPtr->dataAccType == RMT_WORD_ACC)
		bitMask = 0xffff;
    else
		bitMask = 0xffffffff;

	memoryClear((uint8*)&specActionData,sizeof(strFbSpecActionInfo)) ;
	
	specActionData.dataSize 	= (reqInfoPtr->dataAccType + 1);
	specActionData.refType 		= 0x00; 
	specActionData.memoryType 	= S_MEMORY;
	specActionData.bitPosition 	= 0;
	specActionData.bitposFlag 	= 0;

	dataPtr 	= (uint32 *)reqInfoPtr->writeData;

	for(i = 0; i < reqInfoPtr->numOfData; i++, dataPtr++) 
	{
    	*dataPtr 		= (*dataPtr & bitMask);
    	sharedMemAddr	= reqInfoPtr->destFbBlockAddr +i;
		specData 		= sharedMemAddr<<8;

		
		retData = 0;
		retVal = refDataCopyFromMem(&retData,specData, specActionData, SIZE32_TYPE,0);
		if ( retVal) {
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, retVal);
			continue;			
		}

		if(retData != *dataPtr) {
			
			retVal = refDataCopyToMem(dataPtr,specData, specActionData, SIZE32_TYPE);
			
			if(retVal)
				setErrorCode(	__FILE__, __LINE__, __FUNCTION__, retVal);
			
			else {
				retVal = refDataCopyToBusMem(dataPtr, specData, specActionData, SIZE32_TYPE);
	
				if(retVal)
					setErrorCode(	__FILE__, __LINE__, __FUNCTION__, retVal);
				else 
				{
						
					if( dualFlagCheckInRunShelf() == DUAL_CHECK_ENABLE)
					{
						retVal = rmtSharedVarNumWrite(	reqInfoPtr->dataAccType,
														1, 
														sharedMemAddr, 
														(uint32 *)dataPtr
													);
						if(retVal)
							setErrorCode(	__FILE__, __LINE__, __FUNCTION__, retVal);
					}		
				}							 
			}
		}
	}

MODULE_END:

    memoryCopy ( 	(uint8 *)&respInfo,
    				(uint8 *)reqInfoPtr,
                    sizeof(strEthDataWriteRespInfo)
				);

	respInfo.accStatus = status;

    
	respMsgPtr->dataLen = sizeof(strEthDataWriteRespInfo);
	
    memoryCopy ( (uint8 *) &respMsgPtr->dataBuf[4],
    			 (uint8 *) &respInfo,
                 respMsgPtr->dataLen
			  );

	
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0], (uint8*)cmdCode, DOUBLE_SIZE);
	respMsgPtr->dataLen += DOUBLE_SIZE;

    return(status);
}


uint32 sysRmtDataWriteRespFunc(strNewNetProtocolInfo	*respMsgPtr)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	strFbMemInfo	fbMemInfo;	
	strEthDataWriteRespInfo	*respInfoPtr;
	
	respInfoPtr = (	strEthDataWriteRespInfo *)&respMsgPtr->dataBuff[4];

		
	
	retVal = fbMemInfoRead(	LOGIC_ID, 
							respInfoPtr->srcFbBlockAddr,
							&fbMemInfo
						  );
	if(retVal != NO_ERROR){
		status = retVal;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
    }
    
	if((fbMemInfo.fbId == ETH_RMT_DATA_SEND_FB_CODE) || (fbMemInfo.fbId == MBT_RMT_DATA_SEND_FB_CODE))
		status = sysRmtDataWriteRespFromSharedMem (respMsgPtr);

	else
		status = FB_CODE_ID_ERR;
	
	if(status != NO_ERROR)
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);

MODULE_END:
	
    return(status);
}


static uint32 sysRmtDataWriteRespFromSharedMem(strNewNetProtocolInfo	*respMsgPtr)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	strFbMemInfo	fbMemInfo;	
	strEthDataWriteRespInfo	*respInfoPtr;
	strMemPointerInfo	memInfo;
	uint32	*saveVarPtr, *varPtr, *outputPtr;

	uint32 *cmdCode;

	
    cmdCode = (uint32*)(&respMsgPtr->dataBuff[0]);

	respInfoPtr = (strEthDataWriteRespInfo *)&respMsgPtr->dataBuff[4];

	
	if(respInfoPtr->accStatus != NO_ERROR) 
		{
    	status = respInfoPtr->accStatus;
		setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
    }
    
    if(respInfoPtr->deviceType != LK11_CPU) 
    {
    	status = RMT_DEVICE_TYPE_ERR;
		setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
   		goto MODULE_END;
    }

	if(	(respInfoPtr->dataAccType != RMT_BYTE_ACC) &&
		(respInfoPtr->dataAccType != RMT_WORD_ACC) &&
		(respInfoPtr->dataAccType != RMT_LONG_ACC) 
	){
		status = RMT_DATA_ACC_TYPE_ERR;
		setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
}

    if(respInfoPtr->numOfData > MAX_RMT_ST_ACC_DATA_NUM)
    {
    	status = RMT_DATA_ACC_NUM_ERR;
		setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
    	goto MODULE_END;
    }
	
	retVal = fbMemInfoRead(	LOGIC_ID, 
							respInfoPtr->srcFbBlockAddr,
							&fbMemInfo
						  );

	if(retVal != NO_ERROR)
	{
		status = retVal;
		setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);

		goto MODULE_END;
    }
	if( (fbMemInfo.fbId != ETH_RMT_DATA_SEND_FB_CODE)
        && (fbMemInfo.fbId != MBT_RMT_DATA_SEND_FB_CODE)    )
	{
		status = FB_CODE_MISMATCH_ERR;
		status = retVal;

		goto MODULE_END;
	}

	
	fbMemPointerGet(LOGIC_ID, &memInfo);

	varPtr 	= (uint32 *)memInfo.paraMemPtr;
	varPtr += fbMemInfo.varIndex;

 	saveVarPtr = varPtr;

    
	varPtr = saveVarPtr;
    *varPtr = ETH_RMT_COM_RCV_DONE; 

    
    if(status != NO_ERROR){
		varPtr = saveVarPtr +1;
        *varPtr += 1;
	}
    else {
		
        varPtr = saveVarPtr + 2;
        *varPtr = respInfoPtr->numOfData;
	}

    
	outputPtr = (uint32 *)memInfo.outputMemPtr;
    outputPtr += (fbMemInfo.outputIndex + 5); /* 수정 주의 */

    retVal = ethRmtFbComSeqNoChkFunc(	respInfoPtr->srcFbBlockAddr, 
										fbMemInfo.fbId, 
										respMsgPtr->sequenceID, 
										outputPtr
						   			);
	if(retVal != NO_ERROR) {
		
        varPtr = saveVarPtr + 4 ;
        *varPtr += 1;
    }

MODULE_END:
	
    return(status);
}


uint32 sysRmtDataBroadCastFunc(strNewNetProtocolInfo	*respMsgPtr)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	strEthDataAccCommonInfo	*respInfoPtr;
	uint32	bitMask, i, *dataPtr,specData;
	strFbSpecActionInfo specActionData;

	uint32 *cmdCode, retData, maxShrardMem = 0;

	
    cmdCode = (uint32*)(&respMsgPtr->dataBuff[0]);

	respInfoPtr = (strEthDataAccCommonInfo *)&respMsgPtr->dataBuff[4];

	
 	if(respInfoPtr->dataAccType == RMT_LONG_ACC) 
 		maxShrardMem = MAX_SHARED_MEM_NUM;
	else if(respInfoPtr->dataAccType == RMT_WORD_ACC) 
		maxShrardMem = MAX_SHARED_MEM_NUM *2;
	else if(respInfoPtr->dataAccType == RMT_BYTE_ACC) 
		maxShrardMem = MAX_SHARED_MEM_NUM *4;
	else {
		maxShrardMem = 0;
	   	status = RMT_DATA_ACC_TYPE_ERR;
		setErrorCode(	__FILE__, 
						__LINE__,
						"sysRmtDataBroadCastFunc", 
						status
					 );
		goto MODULE_END;
	}

    if(respInfoPtr->numOfData > MAX_SHARED_ACC_DATA_NUM){
    	status = RMT_DATA_ACC_NUM_ERR;
		setErrorCode(	__FILE__, 
						__LINE__,
						"sysRmtDataBroadCastFunc", 
						status
					 );
		goto MODULE_END;
    }
    
    
	if((respInfoPtr->destFbBlockAddr < 0) ||
		(respInfoPtr->destFbBlockAddr >= maxShrardMem)
	  ){
		status = VARIABLE_ADDR_RANGE_ERR;
		setErrorCode(	__FILE__, 
						__LINE__,
						"sysRmtDataBroadCastFunc",
						status
					 );
		goto MODULE_END;
	}

	
    if(respInfoPtr->dataAccType == RMT_BYTE_ACC)
		bitMask = 0xff;
    else if(respInfoPtr->dataAccType == RMT_WORD_ACC)
		bitMask = 0xffff;
    else
		bitMask = 0xffffffff;

	memoryClear((uint8*)&specActionData,sizeof(strFbSpecActionInfo));
	
	
	specActionData.dataSize 	= (respInfoPtr->dataAccType + 1);
	specActionData.refType 		= 0x00; 
	specActionData.memoryType 	= S_MEMORY;
	specActionData.bitPosition 	= 0;
	specActionData.bitposFlag 	= 0;

	dataPtr	= (uint32 *)&respMsgPtr->dataBuff[16];

	for(i = 0; i < respInfoPtr->numOfData; i++, dataPtr++) {
    	*dataPtr = (*dataPtr & bitMask);
		specData = 0;
		specData = (respInfoPtr->destFbBlockAddr +i)<<8;

		retVal = refDataCopyFromMem(&retData,specData, specActionData, SIZE32_TYPE,0);
		if(retData != *dataPtr) {
			retVal = refDataCopyToMem(dataPtr,specData, specActionData, SIZE32_TYPE);

			
			if(retVal == NO_ERROR)
				retVal = refDataCopyToBusMem(dataPtr, specData, specActionData, SIZE32_TYPE);

			
			
			if(retVal)
				setErrorCode( __FILE__,
								__LINE__,
								"sysRmtDataBroadCastFunc", 
								retVal
							  );	
		}
	}

	if(status) {
		setErrorCode( __FILE__,
						__LINE__,
						"sysRmtDataBroadCastFunc", 
						retVal
					  );		
	}

MODULE_END:
    return(status);
}





