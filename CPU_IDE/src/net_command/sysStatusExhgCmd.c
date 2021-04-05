
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
#include	"rmt_comm\rmtDevUtil.h"


extern strSysRunningConfigInfo		*gSysRunningInfoPtr;
extern strMpuDiuRunningStatusInfo 	*gDiuRasInfoPtr;


uint32 sysRmtStatusReqAndRespFunc (	strNewNetProtocolInfo *,
            						strBufInfo	*,
            						strinetAddr *
									);
uint32 sysRmtStatusRespFunc(strNewNetProtocolInfo	*);

static uint32	optionCodeCmdNullFunc1(
				strNewNetProtocolInfo	*reqMsgPtr,
				strBufInfo *respMsgPtr, 
                strinetAddr	*iaddr
					)
{
	uint32	status = NO_ERROR;

    respMsgPtr->dataLen = 0;

    status = L_OPTION_CODE_ERR & 0xffff;

	return(status); 
}

uint32	optionCodeCmdNullFunc2(
				strNewNetProtocolInfo	*reqMsgPtr
					)
{
	uint32	status = NO_ERROR;

    status = L_OPTION_CODE_ERR & 0xffff;

	return(status); 
}

extern void memoryClear(uint8 *, uint32);
extern void memoryCopy (uint8 *, uint8 *, uint32);
extern void setErrorCode(uint8 *, uint32, uint8 *, uint32);

extern uint32 systemCnfgInfoRead(strSysConfigInfo *);
extern uint32 fbMemInfoRead(uint32, uint32, strFbMemInfo *);
extern uint32	fbMemPointerGet(uint32 , strMemPointerInfo *);
extern uint32 ethRmtFbComSeqNoChkFunc(uint32, uint32, uint32, uint32 *);


uint32	(*sysRmtStatusReqCmdOPFuncList[]) 
			(strNewNetProtocolInfo	*,strBufInfo *, strinetAddr *)
= {
	optionCodeCmdNullFunc1,			
	sysRmtStatusReqAndRespFunc	
};

uint32	(*sysRmtStatusRespCmdOPFuncList[]) 
			(strNewNetProtocolInfo	*)
= {
	optionCodeCmdNullFunc2,		
	sysRmtStatusRespFunc	
};


uint32 sysRmtStatusReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr,
            strinetAddr *peerAddr
		)
{
	uint32	status = NO_ERROR;

	strEthCommPathReadReqInfo	*reqInfoPtr;
	strEthCommPathReadRespInfo	*respInfoPtr;
	strSysConfigInfo  sysCnfgInfo;
	strMpuDiuRunningStatusInfo	*diuStatusInfoPtr = gDiuRasInfoPtr;
	strSysRunningConfigInfo		*sysStatusInfoPtr = gSysRunningInfoPtr;
	uint32 *cmdCode;

	
    cmdCode = (uint32*)(&reqMsgPtr->dataBuff[0]);

	
    reqInfoPtr = (strEthCommPathReadReqInfo *)&reqMsgPtr->dataBuff[4];

  	
    if(reqInfoPtr->deviceType != LK11_CPU) 
	{ 
    	status = RMT_DEVICE_TYPE_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
    }
    
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0], (uint8*)cmdCode, DOUBLE_SIZE);
	respInfoPtr = (strEthCommPathReadRespInfo *)&respMsgPtr->dataBuf[4];
	
    
	respMsgPtr->dataLen = sizeof(strEthCommPathReadRespInfo) + DOUBLE_SIZE;

    respInfoPtr->destLineAIpAddr = reqInfoPtr->destLineAIpAddr;
    respInfoPtr->destLineBIpAddr = reqInfoPtr->destLineBIpAddr;

    respInfoPtr->srcFbBlockAddr = reqInfoPtr->srcFbBlockAddr;
    respInfoPtr->deviceType 	= reqInfoPtr->deviceType;

	systemCnfgInfoRead(&sysCnfgInfo);

    if(sysCnfgInfo.netCnfgInfo.ctrlNetDualStatus == CTRL_NET_LINE_DUAL)
    {
    	respInfoPtr->lineStatus 	= NET_DUAL_LINE;
	    respInfoPtr->commLine 		= NET_LINE_LINK_DUAL;
	}
	else 
	{
    	respInfoPtr->lineStatus 	= NET_SINGLE_LINE;
	
		if ((peerAddr->iAddr & 0xffffff00) == 
			(sysCnfgInfo.netCnfgInfo.myIpAddr[0] & 0xffffff00))
		    respInfoPtr->commLine 	= NET_LINE_LINK_A;
 
		else if ( 	(peerAddr->iAddr & 0xffffff00) == 
					(sysCnfgInfo.netCnfgInfo.myIpAddr[1] & 0xffffff00))
		    respInfoPtr->commLine 	= NET_LINE_LINK_B;
		else 
		{
	    	status = IP_ADDR_INVALID_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		}
	}

    if(diuStatusInfoPtr->redundancyStatus == NO_ERROR)
    	respInfoPtr->redundancyStatus 	= SYSTEM_SHELF_DUAL_STATUS;
    else
    	respInfoPtr->redundancyStatus 	= SYSTEM_SHELF_SINGLE_STATUS;
    

    if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN) 
    { 
    	if (sysCnfgInfo.shelfType == MASTER_SHELF)
			respInfoPtr->dualStatus		= RMT_DEV_MASTER_RUN;
		else
			respInfoPtr->dualStatus		= RMT_DEV_SLAVE_RUN;
	}
    else 
    { 
    	if (sysCnfgInfo.shelfType == MASTER_SHELF)
			respInfoPtr->dualStatus		= RMT_DEV_SLAVE_RUN;
		else
			respInfoPtr->dualStatus		= RMT_DEV_MASTER_RUN;
	}

    return(status);
}


uint32 sysRmtStatusRespFunc(strNewNetProtocolInfo	*respMsgPtr)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	strFbMemInfo	fbMemInfo;	
	strEthCommPathReadRespInfo	*respInfoPtr;
	strMemPointerInfo	memInfo;
	uint32	*saveVarPtr, *varPtr, *outputPtr;

	respInfoPtr = (	strEthCommPathReadRespInfo *)&respMsgPtr->dataBuff[4];

	
    if(respInfoPtr->deviceType != LK11_CPU)
    {
    	status = RMT_DEVICE_TYPE_ERR;
		setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
		
		goto MODULE_END;
    }

    if ((respInfoPtr->lineStatus != NET_SINGLE_LINE) &&
		(respInfoPtr->lineStatus != NET_DUAL_LINE)
	) {
    	status = RMT_ETH_DEV_LINE_CNFG_ERR;
		setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
	
		goto MODULE_END;
    }

    if( (respInfoPtr->commLine < NET_LINE_LINK_A) ||
    	(respInfoPtr->commLine > NET_LINE_LINK_DUAL)
	){
    	status = RMT_ETH_DEV_LINE_LINK_ERR;
		setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
    }

    if( (respInfoPtr->redundancyStatus < SYSTEM_SHELF_SINGLE_STATUS) ||
    	(respInfoPtr->redundancyStatus > SYSTEM_SHELF_DUAL_STATUS)
	){
    	status = RMT_ETH_DEV_REDUNDANCY_ERR;
		setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
    }

	
    if( respInfoPtr->dualStatus == RMT_DEV_RUN_ERR	){
		status = RMT_ETH_DEV_DUAL_ERR;
		setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
 		goto MODULE_END;
   }

    if( respInfoPtr->dualStatus > RMT_DEV_SLAVE_RUN	){
    	status = RMT_ETH_DEV_DUAL_ERR;
		setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
    }
	
	retVal = fbMemInfoRead(	LOGIC_ID, 
							respInfoPtr->srcFbBlockAddr,
							&fbMemInfo
						  );
	if(retVal != NO_ERROR){
    	status = retVal;
		setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
    }
	if(fbMemInfo.fbId != ETH_RMT_STATUS_READ_FB_CODE){
		status = FB_CODE_MISMATCH_ERR;

		setErrorCode (__FILE__, __LINE__, __FUNCTION__, status);

		goto MODULE_END;
	}
	
	fbMemPointerGet(LOGIC_ID, &memInfo);

	varPtr 	= (uint32 *)memInfo.paraMemPtr;
    varPtr += fbMemInfo.varIndex; 
	
    saveVarPtr = varPtr;

    
    varPtr += 4;
    *varPtr = ETH_RMT_COM_RCV_DONE; 

    
    if(status != NO_ERROR){
		varPtr = saveVarPtr;

        varPtr += 5;
        *varPtr += 1;
	}
    else {
		
        varPtr = saveVarPtr;
        *varPtr = respInfoPtr->lineStatus;

        
		varPtr	+= 1;
		*varPtr = respInfoPtr->commLine;

		
		varPtr	+= 1;
		*varPtr = respInfoPtr->redundancyStatus;

		
		varPtr	+= 1;
		*varPtr = respInfoPtr->dualStatus;
	}	

    
	outputPtr = (uint32 *)memInfo.outputMemPtr;
    outputPtr += (fbMemInfo.outputIndex + 7);

    retVal = ethRmtFbComSeqNoChkFunc(	respInfoPtr->srcFbBlockAddr, 
										ETH_RMT_STATUS_READ_FB_CODE, 
										respMsgPtr->sequenceID, 
										outputPtr
						   			);
	if(retVal != NO_ERROR) {
		
        varPtr = saveVarPtr;
   	    varPtr += 12;
        *varPtr += 1;
    }

MODULE_END:
    return(status);
}



