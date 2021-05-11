
#include	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"cmdList.h"

#include	"netProtocol.h"
#include	"usrDefBuf.h"

#include	"pcsDefs.h"

#include	"segScheduler.h"


#include	"protocolBuf.h"
#include	"usrErrCode.h"

#include	"fbMem.h"
#include	"sysDual.h"

#define		MAX_ALLVAR_MEMREAD_NUM 	(64) 
#define		MAX_ITEM_MEMREAD_NUM 	(100) 
#define		MAX_VAR_ACC_NUM 		(400)

extern strTotalSegDownInfo	gSegDownloadData;
extern void *gRetainVarInfoLocalPtr, *gRetainVarInfoBusPtr;


extern uint32	variableDataRead(uint32 *,uint32,uint32, uint32);
extern uint32	variableDataWrite(uint32 *,uint32,uint32, uint32);
extern void		memoryClear ( uint8 *, uint32 );
extern uint32	getFbTaskTblInfo(uint32 , uint32 , strFbTaskTblMemInfo *);
extern void		setErrorCode(uint8 *, uint32, uint8 *, uint32);
extern void		setErrorCodeWithVal(uint8 *, uint32, uint8 *, uint32,
					uint8   *, uint32,uint8 *, uint32,uint8 *, uint32);
extern uint32	localVariableRetainInfoWrite(uint32, uint32, uint8);
extern uint32	busVariableRetainInfoWrite(uint32, uint32, uint8);
extern uint32	fbSysExeVarInfoWrite(uint32, strVariableExecInfo* );
extern uint32	fbSysExeVarInfoRead(uint32 , strVariableExecInfo* );
extern uint32	refDataCopyToMem(uint32*, uint32, strFbSpecActionInfo, uint32);
extern uint32	refDataCopyFromMem(uint32*, uint32, strFbSpecActionInfo, uint32,uint32);
extern uint32	refDataCopyToBusMem(uint32*, uint32, strFbSpecActionInfo, uint32);
extern uint32   refDataCopyToRmtMem(uint32*, uint32, strFbSpecActionInfo, uint32);

extern uint32  busVariableDataWrite(uint32 *,uint32,uint32, uint32,uint32);
extern uint32  rmtVariableDataWrite(uint32 *,uint32,uint32, uint32,uint32);

extern uint32	specActionDataCheck(uint32 *, uint32 );
extern void		memoryCopy ( uint8 *, uint8 *, uint32);
extern	uint32 dualFlagCheckInRunShelf(void);
extern void  eightByteConversion( uint32 *, uint32 *, uint8 );

#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
extern uint32 refDatacopyToInitVarBusMem(uint32 *,uint32 , strFbSpecActionInfo , uint32);
#endif


extern uint32 optionCodeCmdNullFunc(strNewNetProtocolInfo *,strBufInfo *);

uint32 VarBlockReadReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32 VarItemReadReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32 VarBlockWriteReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32 VarItemWriteReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32 VarInfoDownloadReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32 VarAllMemReadReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32 VarAllMemWriteReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);



uint32	(*VariableAccessCmdOPFuncList[]) 
			(strNewNetProtocolInfo	*,strBufInfo *)
= {
	optionCodeCmdNullFunc,							
	VarBlockReadReqAndRespFunc,						
 	VarItemReadReqAndRespFunc,						 
	VarBlockWriteReqAndRespFunc,					
  	VarItemWriteReqAndRespFunc,						 
  	VarInfoDownloadReqAndRespFunc,					 
 	VarAllMemReadReqAndRespFunc,					 
	VarAllMemWriteReqAndRespFunc					 
};


uint32 VarBlockReadReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR;
	uint32 i;
	uint32 cmdCode,  variableRegion, memoryAddress, readType , dataNum;
    uint32 *reqDataPtr = (uint32 *) reqMsgPtr->dataBuff;
    uint32 *respDataPtr = (uint32 *) respMsgPtr->dataBuf;

	uint32   *varDataPtr = (uint32*) &respMsgPtr->dataBuf[16];
	uint32  readDataLen = 0 ;
	uint32  endMemAddr = 0;
 
    cmdCode = *reqDataPtr++;
	variableRegion = *reqDataPtr >> 16;
	
	
	if( (variableRegion < AUTO_VAR)&&(variableRegion> SMEM_VAR) ) {
		status = VARIABLE_KIND_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Curr variableRegion" ,variableRegion,
							"Max Var Region",SMEM_VAR, 	
							"Not Using",0
						);

		goto MODULE_END;
	}
    
	memoryAddress = *reqDataPtr++ &0xffff;
	readType = *reqDataPtr>>16;
	dataNum  = *reqDataPtr++ &0xffff;

	
	if(readType==SPEC_SIZE_LWORD ) {
		if( ( 4*DOUBLE_SIZE + (dataNum * LONG_SIZE) ) >
			(MAX_NET_MSG_LEN - MAX_NEW_FRAME_HEAD_LEN) ) 
		{
			status = MSG_LEN_EXCEED_ERR;
 			goto CHECK_END;
		}
	}
	else {
		if( ( 4*DOUBLE_SIZE + (dataNum * DOUBLE_SIZE) ) > 
			(MAX_NET_MSG_LEN - MAX_NEW_FRAME_HEAD_LEN) ) 
		{
 			 status = MSG_LEN_EXCEED_ERR;
			 goto CHECK_END;
		}
	}

	
	if(variableRegion == AUTO_VAR) 	endMemAddr = memoryAddress-1 + dataNum;
	else endMemAddr = memoryAddress + dataNum;
	
	if(readType == SPEC_SIZE_BIT) endMemAddr = endMemAddr/32;
	else if(readType == SPEC_SIZE_BYTE) endMemAddr = endMemAddr/4;
	else if(readType == SPEC_SIZE_WORD) endMemAddr = endMemAddr/2;
	else if(readType == SPEC_SIZE_DWORD) endMemAddr = endMemAddr*1;
	else if(readType == SPEC_SIZE_LWORD) endMemAddr = endMemAddr*2;
	else {
		status = VARIABLE_TYPE_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Curr variableRegion" ,variableRegion,
							"Start Var Addr",memoryAddress,
							"Var Type",readType);
		goto CHECK_END;
	}

	
	if (variableRegion == AUTO_VAR) {
		if ( (endMemAddr > MAX_MPU_AUTOVAR_NUM) || (memoryAddress <1 ) )
		 status = VARIABLE_INDEX_ERR;
	}
	else if (variableRegion == MMEM_VAR)  {
		if (endMemAddr > MAX_MPU_MMEM_NUM) 
		 status = VARIABLE_INDEX_ERR;
	}
	else if (variableRegion == SMEM_VAR)  {
		if (endMemAddr > MAX_SHARED_MEM_NUM) 
		 status = VARIABLE_INDEX_ERR;
	}
	else status = VARIABLE_KIND_ERR;
	
	if(status != NO_ERROR) {
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Curr variableRegion" ,variableRegion,
							"Start Var Addr",memoryAddress,
							"End Var Addr",endMemAddr);
		goto CHECK_END;
	}

CHECK_END:
	*respDataPtr++ = cmdCode ;
	*respDataPtr    = memoryAddress;
	*respDataPtr++ |= (variableRegion<<16) ;

	*respDataPtr    = dataNum;
	*respDataPtr++ |= readType << 16 ;
	respMsgPtr->dataLen =  DOUBLE_SIZE*3 ;
	
	
	respMsgPtr->dataLen +=  DOUBLE_SIZE;

	varDataPtr = (uint32 *) &respMsgPtr->dataBuf[respMsgPtr->dataLen];

	readDataLen = 0;
	if(status == NO_ERROR) {
		for (i=0; i< dataNum ; i++, varDataPtr++)
		{
			
			status = variableDataRead(varDataPtr,variableRegion, memoryAddress+ i, readType) ; 
			if(status != NO_ERROR) {
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Var Region" ,variableRegion,
							"Start Var Addr",memoryAddress,
							"Curr Var Addr", memoryAddress+i);

				goto MODULE_END;
			}
			readDataLen +=  DOUBLE_SIZE;

			
			if(readType == SPEC_SIZE_LWORD) {
				varDataPtr++;
				readDataLen +=  DOUBLE_SIZE;
			}
		}
	}

MODULE_END:
	*respDataPtr = status;
	if(status != NO_ERROR) readDataLen =0;
	respMsgPtr->dataLen += readDataLen ;
	
    return(status);
}


uint32 VarItemReadReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR;
	uint32 j, i;
	uint32 cmdCode,  itemNum, variableRegion;
	uint32 memoryAddress =0 , readType = 0, dataNum =0;
    uint32 *reqDataPtr = (uint32 *) reqMsgPtr->dataBuff;
    uint32 *respDataPtr = (uint32 *) respMsgPtr->dataBuf;

	uint32  readDataLen = 0,  *varDataPtr;
	uint32  endMemAddr=0 ;

    cmdCode = *reqDataPtr++;
	itemNum = *reqDataPtr++;

	*respDataPtr++ = cmdCode ;
	*respDataPtr++ = itemNum;
	respMsgPtr->dataLen =  DOUBLE_SIZE*2 ;

	if(itemNum> MAX_ITEM_MEMREAD_NUM ) {
	    status = L_ITEM_NO_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Curr ITEM Num", itemNum,
							"MAX ITEM Num", MAX_ITEM_MEMREAD_NUM,
							"Not Using", 0 );
	    return(status);
	}


	
	for(j=0;j<itemNum;j++)
	{
		variableRegion = *reqDataPtr >> 16;
		
		if( (variableRegion < AUTO_VAR)&&(variableRegion> SMEM_VAR) ) {
			status = VARIABLE_KIND_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Curr variableRegion" ,variableRegion,
								"Max Var Region",SMEM_VAR, 	
								"Not Using",0
							);
			goto CHECK_END;
		}
    
		memoryAddress = *reqDataPtr++ &0xffff;

		readType = *reqDataPtr>>16;
		dataNum  = *reqDataPtr++ &0xffff;

		
		if(variableRegion == AUTO_VAR) 	endMemAddr = memoryAddress-1 + dataNum;
		else endMemAddr = memoryAddress + dataNum;
		
		if(readType == SPEC_SIZE_BIT) endMemAddr = endMemAddr/32;
		else if(readType == SPEC_SIZE_BYTE) endMemAddr = endMemAddr/4;
		else if(readType == SPEC_SIZE_WORD) endMemAddr = endMemAddr/2;
		else if(readType == SPEC_SIZE_DWORD) endMemAddr = endMemAddr*1;
		else if(readType == SPEC_SIZE_LWORD) endMemAddr = endMemAddr*2;
		else {
			status = VARIABLE_TYPE_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Curr variableRegion" ,variableRegion,
								"Start Var Addr",memoryAddress,
								"Var Type",readType);
			goto CHECK_END;
		}

		
		if (variableRegion == AUTO_VAR) {
			if ( (endMemAddr > MAX_MPU_AUTOVAR_NUM ) || (memoryAddress <1 ) )
			 status = VARIABLE_INDEX_ERR;
		}
		else if (variableRegion == MMEM_VAR)  {
			if (endMemAddr > MAX_MPU_MMEM_NUM) 
			 status = VARIABLE_INDEX_ERR;
		}
		else if (variableRegion == SMEM_VAR)  {
			if ( endMemAddr > MAX_SHARED_MEM_NUM) 
			 status = VARIABLE_INDEX_ERR;
		}
		else status = VARIABLE_KIND_ERR;

		if(status != NO_ERROR) 	{
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Curr variableRegion" ,variableRegion,
								"Start Var Addr",memoryAddress,
								"End Var Addr",endMemAddr);
			goto CHECK_END;
		}

CHECK_END:
		*respDataPtr    = memoryAddress;
		*respDataPtr++ |= (variableRegion <<16) ;
		respMsgPtr->dataLen +=  DOUBLE_SIZE ;

		*respDataPtr    = dataNum;
		*respDataPtr++ |= readType << 16 ;
		respMsgPtr->dataLen +=  DOUBLE_SIZE ;

		
		respMsgPtr->dataLen += DOUBLE_SIZE;

		varDataPtr = (uint32 *) &respMsgPtr->dataBuf[respMsgPtr->dataLen];
		
		readDataLen = 0 ;
		if(status == NO_ERROR) {
			for (i=0; i< dataNum ; i++, varDataPtr++)
			{
				
				status = variableDataRead(varDataPtr,variableRegion, memoryAddress+ i, readType) ; 
				if(status != NO_ERROR) {
					setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
										"Var Region" ,variableRegion,
										"Start Var Addr",memoryAddress,
										"Curr Var Addr", memoryAddress+i);

					goto MODULE_END;
				}
				readDataLen +=  DOUBLE_SIZE;
				
				
				if(readType == SPEC_SIZE_LWORD) {
					varDataPtr++;
					readDataLen +=  DOUBLE_SIZE;
				}
			}
		}

MODULE_END:
		*respDataPtr++ = status;
		if(status != NO_ERROR) 	readDataLen =0;
		else respDataPtr = (uint32*)((uint8*)respDataPtr + readDataLen);
		respMsgPtr->dataLen += readDataLen ;

		if(respMsgPtr->dataLen > (MAX_NET_MSG_LEN - MAX_NEW_FRAME_HEAD_LEN) ) {
			status = DATA_LEN_EXCEED_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

			break;
		}
	}

    return(status);
}


uint32 VarBlockWriteReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR;
	uint32 i;
	uint32 cmdCode,  variableRegion, memoryAddress, writeType , dataNum;
    uint32 *reqDataPtr = (uint32 *) reqMsgPtr->dataBuff;
    uint32 *respDataPtr = (uint32 *) respMsgPtr->dataBuf;

	uint32  *varDataPtr, readDataLen, endMemAddr =0 ;
 
    cmdCode = *reqDataPtr++;
	variableRegion = *reqDataPtr >> 16;
	
	
	if( (variableRegion < AUTO_VAR)&&(variableRegion> SMEM_VAR) ) {
		status = VARIABLE_KIND_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Curr Var Region" ,variableRegion,
							"Max Var Region",SMEM_VAR, 	
							"Not Using",0
					);
		goto MODULE_END;
	}
    
	memoryAddress = *reqDataPtr++ &0xffff;
	writeType = *reqDataPtr>>16;
	dataNum  = *reqDataPtr++ &0xffff;

	
	if(variableRegion == AUTO_VAR) 	endMemAddr = memoryAddress-1 + dataNum;
	else endMemAddr = memoryAddress + dataNum;

	if(writeType == SPEC_SIZE_BIT) endMemAddr = endMemAddr/32;
	else if(writeType == SPEC_SIZE_BYTE) endMemAddr = endMemAddr/4;
	else if(writeType == SPEC_SIZE_WORD) endMemAddr = endMemAddr/2;
	else if(writeType == SPEC_SIZE_DWORD) endMemAddr = endMemAddr*1;
	else if(writeType == SPEC_SIZE_LWORD) endMemAddr = endMemAddr*2;
	else {
		status = VARIABLE_TYPE_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Curr variableRegion" ,variableRegion,
							"Start Var Addr",memoryAddress,
							"Var Type",writeType);
		goto CHECK_END;
	}

	
	if (variableRegion == AUTO_VAR) {
		if ((endMemAddr > MAX_MPU_AUTOVAR_NUM )|| (memoryAddress < 1) ) 
		 status = VARIABLE_INDEX_ERR;
	}
	else if (variableRegion == MMEM_VAR)  {
		if ( (endMemAddr > MAX_MPU_MMEM_NUM)  || (memoryAddress < 0) )
		 status = VARIABLE_INDEX_ERR;
	}
	else if (variableRegion == SMEM_VAR)  {
		if ( (endMemAddr > MAX_SHARED_MEM_NUM) || (memoryAddress < 0) )
		 status = VARIABLE_INDEX_ERR;
	}
	else status = VARIABLE_KIND_ERR;

	if(status != NO_ERROR) {
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Curr variableRegion" ,variableRegion,
							"Start Var Addr",memoryAddress,
							"End Var Addr",endMemAddr);
		goto CHECK_END;
	}

CHECK_END:
	*respDataPtr++ = cmdCode ;
	*respDataPtr    = memoryAddress;
	*respDataPtr++ |= variableRegion <<16 ;

	*respDataPtr    = dataNum;
	*respDataPtr++ |= writeType << 16 ;

	respMsgPtr->dataLen =  DOUBLE_SIZE*3 ;

	varDataPtr = (uint32 *) &reqMsgPtr->dataBuff[DOUBLE_SIZE*3];

	readDataLen = 0;

	if(status == NO_ERROR) {
		for (i=0; i< dataNum ; i++, varDataPtr++)
		{
			status = variableDataWrite(varDataPtr,variableRegion, memoryAddress+i, writeType) ; 
			if(status == NO_ERROR) {
				status = busVariableDataWrite(varDataPtr,variableRegion, 
											  memoryAddress+i, writeType, EN_RETAIN_CHECK) ; 
			}

			if(status == NO_ERROR) {
				if( dualFlagCheckInRunShelf() == DUAL_CHECK_ENABLE)
					status = rmtVariableDataWrite(varDataPtr,variableRegion, 
											  memoryAddress+i, writeType, EN_RETAIN_CHECK) ; 
			}

			if(status != NO_ERROR) {
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"Var Region" ,variableRegion,
									"Start Var Addr",memoryAddress,
									"Curr Var Addr", memoryAddress+i);

				goto MODULE_END;
			}	

			
			if(writeType == SPEC_SIZE_LWORD) varDataPtr++;
		}
	}
MODULE_END:
	*respDataPtr = status;
	respMsgPtr->dataLen +=  DOUBLE_SIZE;
    return(status);
}


uint32 VarItemWriteReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR;
	uint32 j, i;
	uint32 cmdCode,  itemNum, variableRegion;
	uint32  memoryAddress=0, writeType= 0 , dataNum= 0;
    uint32 *reqDataPtr = (uint32 *) reqMsgPtr->dataBuff;
    uint32 *respDataPtr = (uint32 *) respMsgPtr->dataBuf;

	uint32  writeDataLen, endMemAddr =0 ;
	uint32   *varDataPtr ;

    cmdCode = *reqDataPtr++;
	itemNum = *reqDataPtr++;
	
	*respDataPtr++ = cmdCode ;
	*respDataPtr++ = itemNum;
	respMsgPtr->dataLen =  DOUBLE_SIZE*2 ;
	writeDataLen = DOUBLE_SIZE*2 ;

	if(itemNum> MAX_ITEM_MEMREAD_NUM ) {
	    status = L_ITEM_NO_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Curr ITEM Num", itemNum,
							"MAX ITEM Num", MAX_ITEM_MEMREAD_NUM,
							"Not Using", 0 );
	    return(status);
	}

	for(j=0;j<itemNum;j++)
	{
		variableRegion = *reqDataPtr >> 16;
		
		if( (variableRegion < AUTO_VAR)&&(variableRegion> SMEM_VAR) ) {
			status = VARIABLE_KIND_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Curr variableRegion" ,variableRegion,
								"Max Var Region",SMEM_VAR,
								"Err Var Num", j+1 
			);
			goto CHECK_END;
		}
    
		memoryAddress = *reqDataPtr++ &0xffff;
		writeType = *reqDataPtr>>16;
		dataNum  = *reqDataPtr++ &0xffff;

		
		if(variableRegion == AUTO_VAR) 	endMemAddr = memoryAddress-1 + dataNum;
		else endMemAddr = memoryAddress + dataNum;
		
		if(writeType == SPEC_SIZE_BIT) endMemAddr = endMemAddr/32;
		else if(writeType == SPEC_SIZE_BYTE) endMemAddr = endMemAddr/4;
		else if(writeType == SPEC_SIZE_WORD) endMemAddr = endMemAddr/2;
		else if(writeType == SPEC_SIZE_DWORD) endMemAddr = endMemAddr*1;
		else if(writeType == SPEC_SIZE_LWORD) endMemAddr = endMemAddr*2;
		else {
			status = VARIABLE_TYPE_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Curr variableRegion" ,variableRegion,
								"Start Var Addr",memoryAddress,
								"Var Type",writeType);
			goto CHECK_END;
		}

		
		if (variableRegion == AUTO_VAR) {
			if ( (endMemAddr > MAX_MPU_AUTOVAR_NUM ) || (memoryAddress < 1) )
			 status = VARIABLE_INDEX_ERR;
		}
		else if (variableRegion == MMEM_VAR) {
			if( (endMemAddr > MAX_MPU_MMEM_NUM)  || (memoryAddress < 0) )
			 status = VARIABLE_INDEX_ERR;
		}
		else if (variableRegion == SMEM_VAR)  {
			if ( (endMemAddr > MAX_SHARED_MEM_NUM) || (memoryAddress < 0) )
			 status = VARIABLE_INDEX_ERR;
		}
		else status = VARIABLE_KIND_ERR;
		if(status != NO_ERROR) {
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Curr variableRegion" ,variableRegion,
								"Start Var Addr",memoryAddress,
								"End Var Addr",endMemAddr);
			goto CHECK_END;
		}

CHECK_END:
		*respDataPtr    = memoryAddress;
		*respDataPtr++ |= (variableRegion<<16) ;
		respMsgPtr->dataLen +=  DOUBLE_SIZE ;

		*respDataPtr    = dataNum;
		*respDataPtr++ |= writeType << 16 ;
		respMsgPtr->dataLen +=  DOUBLE_SIZE ;

		writeDataLen += DOUBLE_SIZE*2 ;
		varDataPtr = (uint32 *) &reqMsgPtr->dataBuff[writeDataLen];
		writeDataLen += dataNum;

		if(status == NO_ERROR) {
			for (i=0; i<  dataNum; i++, varDataPtr++)
			{
				
				status = variableDataWrite(varDataPtr,variableRegion, memoryAddress+i, writeType) ; 
				if(status == NO_ERROR) {
					status = busVariableDataWrite(varDataPtr,variableRegion,
												  memoryAddress+i, writeType, EN_RETAIN_CHECK) ; 
				}
				
				if(status == NO_ERROR) {
					if( dualFlagCheckInRunShelf() == DUAL_CHECK_ENABLE)
						status = rmtVariableDataWrite(varDataPtr,variableRegion,
												  memoryAddress+i, writeType, EN_RETAIN_CHECK) ; 
				}

				if(status != NO_ERROR) {
					setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
										"Curr variableRegion" ,variableRegion,
										"Start Var Addr",memoryAddress,
										"Var Byte Addr", i);

					goto MODULE_END;
				}
			
				
				if(writeType == SPEC_SIZE_LWORD) varDataPtr++;
			}
		}

MODULE_END:
		*respDataPtr++ = status;
	    reqDataPtr = (uint32*)(reqDataPtr + dataNum);
		respMsgPtr->dataLen += DOUBLE_SIZE;
	}

    return(status);
}




uint32 VarInfoDownloadReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	strFbTaskTblMemInfo	segTbl;
	uint32 i;
	uint32 cmdCode, memoryKind, variableKind, variableIndexNum, variableIndex;
    uint32 *reqDataPtr = (uint32 *) reqMsgPtr->dataBuff;
    uint32 *respDataPtr = (uint32 *) respMsgPtr->dataBuf;

	uint32 initVarNum, varData[2],  specData, varType;
	strFbSpecActionInfo *specAction;
	strVariableExecInfo sysVarExeInfo;
	uint32 memoryAddress =0;

	memoryClear((uint8*)&sysVarExeInfo, sizeof(strVariableExecInfo )) ;

    cmdCode 			= *reqDataPtr++;
	memoryKind 			= *reqDataPtr++;
	variableKind 		= *reqDataPtr++;
	variableIndexNum 	= *reqDataPtr++;

	
	
	for(i=0; i<gSegDownloadData.totalSegNum; i++)
	{
		status = getFbTaskTblInfo(LOGIC_ID, gSegDownloadData.taskId[i], &segTbl);
		if (segTbl.taskMode != PCS_DOWNLOAD) {
			status = L_PCS_MODE_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Curr Task Mode" ,segTbl.taskMode,
								"Set Task Mode",PCS_DOWNLOAD,
								"Not Using",0);
			return(status);
		}
	}
	
 	
	if(memoryKind != LOGIC_ID) {
		status = L_MODULE_ID_ERR;
        retVal = COMMAND_SUPPORT_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Module Kind", memoryKind,
							"Set Module Kind",LOGIC_ID,
							"Not Using",0 
				  		  );
		goto RETAIN_END;
    }
	
	
	if( (variableKind != AUTO_VAR)&&(variableKind != MMEM_VAR) ) {
		status = VARIABLE_KIND_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Recv variable Kind",variableKind,
							"Auto Memory Define", AUTO_VAR,
							"M Memory Define", MMEM_VAR
							);
		goto RETAIN_END;
	}
	status = fbSysExeVarInfoRead(variableKind,&sysVarExeInfo);
	if(status != NO_ERROR){
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
	}

	for(i=0; i<variableIndexNum; i++)
	{
		variableIndex = *reqDataPtr++;

		
		if (variableKind == AUTO_VAR) 
		{
			if (variableIndex > MAX_MPU_AUTOVAR_NUM) 
			{
			 	status = VARIABLE_INDEX_ERR;
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"Var Kind",variableKind,
									"Var Index",variableIndex,
									"Max Index",MAX_MPU_AUTOVAR_NUM);
			}
		}
		
		else if (variableKind == MMEM_VAR)  
		{
			if (variableIndex > MAX_MPU_MMEM_NUM) 
			{
				status = VARIABLE_INDEX_ERR;
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"Var Kind",variableKind,
									"Var Index",variableIndex,
									"Max Index",MAX_MPU_MMEM_NUM);
			}
		}
		
		if(status != NO_ERROR) 	goto RETAIN_END;

		status = localVariableRetainInfoWrite(variableKind, variableIndex, EN_RETAIN);
		if(status != NO_ERROR)
		{ 
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Recv variable Index Num",variableIndexNum,
								"Error Variable Index", i+1,
								"Not Using", 0
							);
			break;
		}
		else 
		{
			status = busVariableRetainInfoWrite(variableKind, variableIndex, EN_RETAIN);
			if(status != NO_ERROR) 
			{
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"Recv variable Index Num",variableIndexNum,
									"Error Variable Index", i+1,
									"Not Using", 0
								);
				break;
			}
		}

		
		if((status == NO_ERROR)) {
			if(sysVarExeInfo.curMaxVarRetainInfo < (variableIndex/32 +1 ) )
				sysVarExeInfo.curMaxVarRetainInfo =  (variableIndex/32 +1) ;
			if(sysVarExeInfo.curMaxVarAddr < variableIndex)
				sysVarExeInfo.curMaxVarAddr =  variableIndex;
		}


	}
	
	if(sysVarExeInfo.curMaxVarRetainNum < variableIndexNum) {
		sysVarExeInfo.curMaxVarRetainNum +=  variableIndexNum;
	}
	status = fbSysExeVarInfoWrite(variableKind,&sysVarExeInfo);
	if(status != NO_ERROR){
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
	}

RETAIN_END:

	*respDataPtr++ = cmdCode ;
	*respDataPtr++ = memoryKind ;
	*respDataPtr++ = variableKind ;
	*respDataPtr++ = variableIndexNum ;
	*respDataPtr++ = status;
	respMsgPtr->dataLen =  DOUBLE_SIZE*5 ;
	
	status = fbSysExeVarInfoRead(variableKind,&sysVarExeInfo);
	if(status != NO_ERROR){
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
	}
	
	initVarNum = *reqDataPtr++;
	for(i=0;i<initVarNum;i++)
	{
		specAction = (strFbSpecActionInfo *) reqDataPtr++;
		if((specAction->dataSize == SPEC_SIZE_LWORD)&&(specAction->bitposFlag != 1 ))
			varType = SIZE64_TYPE;
		else varType = SIZE32_TYPE;

		specData = *reqDataPtr++;

		
		if(varType == SIZE64_TYPE) {
			eightByteConversion((uint32*)&varData[0],(uint32*)reqDataPtr++,0);
			reqDataPtr++;
		}
		else {
			varData[0] = *reqDataPtr++;
			varData[1] = 0;
		}	

		status = specActionDataCheck((uint32*)specAction, specData);
		if(status != NO_ERROR) 	{
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		}
			
		
		status = refDataCopyToMem(varData, specData, *specAction, varType);
		if(status!=NO_ERROR)
		{
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Recv variable Index Num",variableIndexNum,
								"Error Variable Index", i+1,
								"Recv Memory Type", specAction->memoryType
							);
			goto INIT_END;
		}
		
		status = refDataCopyToBusMem(varData,specData, *specAction, varType);
		if(status!=NO_ERROR)
		{
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Recv variable Index Num",variableIndexNum,
								"Error Variable Index", i+1,
								"Not Using", 0
							);
			goto INIT_END;
		}
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)		
		
		status = refDatacopyToInitVarBusMem(varData,specData, *specAction, varType);
		if(status!=NO_ERROR)
		{
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Recv variable Index Num",variableIndexNum,
								"Error Variable Index", i+1,
								"Not Using", 0
							);
			goto INIT_END;
		}

#endif
		
		if(specAction->memoryType == A_MEMORY) 	memoryAddress = specData>>8;
		else if( (specAction->memoryType == M_MEMORY) ||
				 (specAction->memoryType == S_MEMORY) )
		{
			switch(specAction->dataSize){
				case SPEC_SIZE_BIT:
					memoryAddress = (specData>>8) /32;
				break;
				case SPEC_SIZE_BYTE	:
					memoryAddress = (specData>>8) /4;
				break;
				case SPEC_SIZE_WORD	:
					memoryAddress = (specData>>8) /2;
				break;
				case SPEC_SIZE_DWORD:
					memoryAddress = (specData>>8) /1;
				break;
				case SPEC_SIZE_LWORD:
					memoryAddress = (specData>>8) *2;
				break;
				default:
					status = FB_SPEC_ACTION_SIZE_ERR;
				break;
			}
		}
		else status = VARIABLE_KIND_ERR;

		if(status != NO_ERROR)
		{
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"VAR KIND", specAction->memoryType,
								"DATA SIZE", specAction->dataSize,
								"Not Using", 0
							);
		}

		if(sysVarExeInfo.curMaxVarAddr < memoryAddress)
				sysVarExeInfo.curMaxVarAddr =  memoryAddress;
	}
	
	
	if(sysVarExeInfo.curMaxVarInitNum < initVarNum) {
		sysVarExeInfo.curMaxVarInitNum +=  initVarNum;
	}
	status = fbSysExeVarInfoWrite(variableKind,&sysVarExeInfo);
	
	if(status != NO_ERROR)
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Recv variable Index Num",variableIndexNum,
							"Error Variable Index", i+1,
							"Not Using", 0
							);

INIT_END:

	*respDataPtr++ = initVarNum;
	*respDataPtr++ = status;
	respMsgPtr->dataLen +=  DOUBLE_SIZE*2 ;


    return(status);
}



uint32 VarAllMemReadReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32 i;
	uint32 cmdCode, memoryKind, readVarNum;
    uint32 *reqDataPtr = (uint32 *) reqMsgPtr->dataBuff;
    uint32 *respDataPtr = (uint32 *) respMsgPtr->dataBuf;

	uint32 varData[2],  specData, varType;
	strFbSpecActionInfo *specAction;
	strVariableExecInfo sysVarExeInfo;
	uint32 varStatus[MAX_ALLVAR_MEMREAD_NUM];

	memoryClear((uint8*)&sysVarExeInfo, sizeof(strVariableExecInfo )) ;

    cmdCode 	= *reqDataPtr++;
	memoryKind 	= *reqDataPtr++;
	readVarNum 	= *reqDataPtr++;
	
	
	if(memoryKind != LOGIC_ID) 
	{
		status = L_MODULE_ID_ERR;
        retVal = COMMAND_SUPPORT_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Module Type", memoryKind,
							"Not Using", 0 ,
							"Not Using", 0 
				  		  );
		goto CHECK_END;
    }

	if(readVarNum > MAX_ALLVAR_MEMREAD_NUM)
	{
		status = L_ITEM_NO_ERR;
	    retVal = L_ITEM_NO_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
						"Curr Var Num", readVarNum,
						"MAX Var Num", MAX_ALLVAR_MEMREAD_NUM,
						"Not Using", 0 
			  		  );
		goto CHECK_END;
    }
	
	for(i = 0; i < readVarNum; i++)
	{
		varStatus[i] = NO_ERROR;
		specAction = (strFbSpecActionInfo*)reqDataPtr++;
		
		if(specAction->refType != SPEC_TYPE_BYREF)
		{
			varStatus[i]  = FB_SPEC_ACTION_REF_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, varStatus[i]);
		}
		else if(specAction->memoryType > MAX_MEMTYPE)
		{
			varStatus[i]  = FB_SPEC_ACTION_TYPE_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, varStatus[i]);
		}
		else if(specAction->dataSize > SPEC_SIZE_LWORD)
		{
			varStatus[i] = FB_SPEC_ACTION_SIZE_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, varStatus[i]);
		}
		
		else if(specAction->dataType > SPEC_DATA_TYPE_DT)
		{
			varStatus[i]  = FB_SPEC_TYPE_SIZE_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, varStatus[i]);
		}
		
		if((specAction->dataSize == SPEC_SIZE_LWORD)&&(specAction->bitposFlag != 1 ))
			varType = SIZE64_TYPE;
		else 
			varType = SIZE32_TYPE;

		specData = *reqDataPtr++;

		
		varStatus[i] = specActionDataCheck((uint32*)specAction, specData);
		
		if(varStatus[i]!=NO_ERROR) 
		{
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, varStatus[i]);
		}
	}
	
CHECK_END:
	
    reqDataPtr 			= (uint32 *) reqMsgPtr->dataBuff;
    cmdCode				= *reqDataPtr++;
	memoryKind 			= *reqDataPtr++;

	*respDataPtr++ 		= cmdCode;
	*respDataPtr++ 		= status;
	respMsgPtr->dataLen =  DOUBLE_SIZE*2;
	
	if(status == NO_ERROR)
	{
		readVarNum 			= *reqDataPtr++;
		*respDataPtr++ 		= readVarNum;
		respMsgPtr->dataLen +=  DOUBLE_SIZE;
		
		for(i = 0; i < readVarNum; i++)
		{
			specAction = (strFbSpecActionInfo*)reqDataPtr++;
			
			memoryCopy((uint8*)respDataPtr,(uint8*)specAction, DOUBLE_SIZE);
			
			respDataPtr++;
			respMsgPtr->dataLen +=  DOUBLE_SIZE;

			specData 			= *reqDataPtr++;
			*respDataPtr++ 		= specData;
			respMsgPtr->dataLen +=  DOUBLE_SIZE;

			if((specAction->dataSize == SPEC_SIZE_LWORD)&&(specAction->bitposFlag != 1 ))
				varType = SIZE64_TYPE;
			else 
				varType = SIZE32_TYPE;

			
			if(varStatus[i] == NO_ERROR)
			{
				varStatus[i] = refDataCopyFromMem(varData,specData, *specAction, varType,0);
			}
			
			*respDataPtr++ 		= varStatus[i] ;
			respMsgPtr->dataLen +=  DOUBLE_SIZE;

			if(varStatus[i] == NO_ERROR) 
			{
				
				if(varType == SIZE64_TYPE) 
				{
					eightByteConversion((uint32*)respDataPtr++,(uint32*)&varData[0],0);
					respMsgPtr->dataLen +=  DOUBLE_SIZE;
				}
				else 
				{
					*respDataPtr = varData[0];
				}	
				
				respDataPtr++;
				respMsgPtr->dataLen +=  DOUBLE_SIZE;
			}
			else 
			{
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, varStatus[i]);
			}
		}
	}

    return(status);
}



uint32 VarAllMemWriteReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32 i;
	uint32 cmdCode, memoryKind, readVarNum;
    uint32 *reqDataPtr = (uint32 *) reqMsgPtr->dataBuff;
    uint32 *respDataPtr = (uint32 *) respMsgPtr->dataBuf;

	uint32 varData[2],  specData, varType;
	strFbSpecActionInfo *specAction;
	strVariableExecInfo sysVarExeInfo;
	uint32 varStatus[MAX_ALLVAR_MEMREAD_NUM];

	memoryClear((uint8*)&sysVarExeInfo, sizeof(strVariableExecInfo )) ;

    cmdCode = *reqDataPtr++;
	memoryKind = *reqDataPtr++;
	readVarNum = *reqDataPtr++;
	
	
	if(memoryKind != LOGIC_ID) {
		status = L_MODULE_ID_ERR;
        retVal = COMMAND_SUPPORT_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Module Type", memoryKind,
							"Not Using", 0 ,
							"Not Using", 0 
				  		  );
		goto CHECK_END;
    }

	if(readVarNum > MAX_ALLVAR_MEMREAD_NUM)
	{
		status = L_ITEM_NO_ERR;
	    retVal = L_ITEM_NO_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
							"Curr Var Num", readVarNum,
							"MAX Var Num", MAX_ALLVAR_MEMREAD_NUM,
							"Not Using", 0 
				  		  );
		goto CHECK_END;
    }


	
	for(i=0;i<readVarNum;i++)
	{
		varStatus[i] = NO_ERROR;
		specAction = (strFbSpecActionInfo*)reqDataPtr++;
		
		if(specAction->refType != SPEC_TYPE_BYREF){
			varStatus[i]  = FB_SPEC_ACTION_REF_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, varStatus[i]);
		}
		else if(specAction->memoryType > MAX_MEMTYPE){
			varStatus[i]  = FB_SPEC_ACTION_TYPE_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, varStatus[i]);
		}
		else if(specAction->dataSize > SPEC_SIZE_LWORD){
			varStatus[i] = FB_SPEC_ACTION_SIZE_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, varStatus[i]);
		}
		
		else if(specAction->dataType > SPEC_DATA_TYPE_DT){
			varStatus[i]  = FB_SPEC_TYPE_SIZE_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, varStatus[i]);
		}
		
		specData = *reqDataPtr++;
		
		if((specAction->dataSize == SPEC_SIZE_LWORD)&&(specAction->bitposFlag != 1 )){
			eightByteConversion((uint32*)&varData[0],(uint32*)reqDataPtr++,0);
			reqDataPtr++;
		}
		else {
			varData[0] = *reqDataPtr++;
			varData[1] = 0;
		}	

		
		varStatus[i] = specActionDataCheck((uint32*)specAction, specData);
		if(varStatus[i]!=NO_ERROR) {
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, varStatus[i]);
		}

	}
	
CHECK_END:
    reqDataPtr = (uint32 *) reqMsgPtr->dataBuff;
    cmdCode = *reqDataPtr++;
	memoryKind = *reqDataPtr++;

	*respDataPtr++ = cmdCode;
	*respDataPtr++ = status;
	respMsgPtr->dataLen =  DOUBLE_SIZE*2;
	
	if(status == NO_ERROR)
	{
		readVarNum = *reqDataPtr++;
		*respDataPtr++ = readVarNum;
		respMsgPtr->dataLen +=  DOUBLE_SIZE;
		
		for(i=0;i<readVarNum;i++)
		{
			specAction = (strFbSpecActionInfo*)reqDataPtr++;
			memoryCopy((uint8*)respDataPtr,(uint8*)specAction, DOUBLE_SIZE);
			respDataPtr++;
			respMsgPtr->dataLen +=  DOUBLE_SIZE;

			specData = *reqDataPtr++;
			*respDataPtr++ = specData;
			respMsgPtr->dataLen +=  DOUBLE_SIZE;

			if((specAction->dataSize == SPEC_SIZE_LWORD)&&(specAction->bitposFlag != 1 ))
				varType = SIZE64_TYPE;
			else varType = SIZE32_TYPE;

			
			
			if((specAction->dataSize == SPEC_SIZE_LWORD)&&(specAction->bitposFlag != 1 )){
				eightByteConversion((uint32*)&varData[0],(uint32*)reqDataPtr++,0);
				reqDataPtr++;
			}
			else {
				varData[0] = *reqDataPtr++;
				varData[1] = 0;
			}	

			
			if(varStatus[i]==NO_ERROR){
				varStatus[i] = refDataCopyToMem(varData,specData, *specAction, varType);
			}
			*respDataPtr++ = varStatus[i] ;
			respMsgPtr->dataLen +=  DOUBLE_SIZE;

			if(varStatus[i]==NO_ERROR) {
				varStatus[i] = refDataCopyToBusMem(varData,specData, *specAction, varType);
				
				if(varStatus[i]==NO_ERROR) {
					if( dualFlagCheckInRunShelf() == DUAL_CHECK_ENABLE)
						varStatus[i] = refDataCopyToRmtMem(varData,specData, *specAction, varType);
				}	
			}
			
			if(varStatus[i]!=NO_ERROR) { 
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, varStatus[i]);
			}

		}
	}

    return(status);
}

