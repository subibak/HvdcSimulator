
#include	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"cmdList.h"
#include	"usrDefBuf.h"
#include	"netProtocol.h"

#include	"pcsDefs.h"
#include	"segScheduler.h"


#include	"protocolBuf.h"
#include	"usrErrCode.h"

#include	"fbMem.h"
#include	"fbDef.h"
#include	"memDef.h"
#include    "sysConfig.h"
#include	"segScheduler.h"

#include	"usrQueData.h"
#include	"sfcMem.h"



strTotalSegDownInfo		gSegDownloadData;

extern strProgDwlParaInfo	gMpuProgDwlPara;
extern strProgDwlParaInfo	gDiuProgDwlPara;
extern strProgDwlParaInfo	gMpuProgDwlBusPara;
extern strProgDwlParaInfo	gDiuProgDwlBusPara;

extern strFbMemInfo		*gMpuFbMemInfoLocalPtr,*gMpuFbMemInfoBusPtr;

extern strFbExecDbInfo	gFbCodeSortingTableInfo[MAX_FB_CODE_ID+1];


uint32 fbSegmentDownloadReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32 fbCodeDownloadReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32 fbSequenceTblDownloadReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32 fbCodeOutputReadReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32 fbCodeOutputWriteReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32 fbCodeParameterReadReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32 fbCodeParameterWriteReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32 fbCodeStatusReadReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);


extern uint32	optionCodeCmdNullFunc(strNewNetProtocolInfo *,strBufInfo *);

extern void		memoryClear( uint8 *, uint32);
extern void		memoryCopy(uint8 *, uint8 *, uint32);
extern void		setErrorCode(uint8 *, uint32, uint8 *, uint32);
extern void		setErrorCodeWithVal(uint8 *, uint32, uint8 *, uint32,
								uint8 *, uint32,uint8 *, uint32,uint8 *, uint32 );
extern uint32	moduleInfoRead(uint32 , strModuleInfo	*);
extern uint32   systemCnfgInfoRead( strSysConfigInfo *);

extern uint32	getFbTaskTblInfo(uint32 , uint32 , strFbTaskTblMemInfo *);
extern uint32	fbDbTableSortingInfoRead(uint32, strFbExecDbInfo *);
extern uint32	fbMemPointerGet(uint32 , strMemPointerInfo *);
extern uint32	segTblPointerGet(uint32 ,uint32	,strFbTaskTblMemInfo	**);
extern uint32	fbMemPointerGet(uint32 , strMemPointerInfo *);
extern uint32	fbMemInfoRead(uint32, uint32, strFbMemInfo *);
extern uint32	fbCodeMemInfoRead(uint32, uint32, strFbMemInfo *);
extern uint32	fbCodeParaTypePointerGet(uint32	,strFbCodeTypePointerInfo	*);

extern uint32	segTblInfoWrite(uint32 , uint32 , strFbTaskTblMemInfo *);
extern uint32	segTblInfoClear(uint32, uint32);
extern uint32	busSegTblInfoClear(uint32, uint32);
extern uint32	busSegTblInfoRead(uint32 , uint32 , strFbTaskTblMemInfo *);
extern uint32	busFbMemPointerGet(uint32 , strMemPointerInfo *);
extern uint32	busSegTblPointerGet(uint32 ,uint32	,strFbTaskTblMemInfo	**);
extern uint32	busSegTblInfoWrite(uint32 , uint32 , strFbTaskTblMemInfo *);
extern uint32   onlineDwlFbCodeSeqIdVarSet(uint32,uint32,uint32);


extern uint32	fbSysExeModuleInfoRead( uint32, strModuleExecInfo* );
extern uint32	fbSysExeModuleInfoWrite( uint32, strModuleExecInfo* );

extern uint32	fbInOutputTypeCasting(uint32 , uint32 , uint32 , uint32 *);

static uint32	localFbSegTblWriteReqAndResp(strNewNetProtocolInfo	*,strBufInfo*);
static uint32	busFbSegTblWriteReqAndResp(strNewNetProtocolInfo	*,strBufInfo*);
static uint32   localFbProgWriteReqAndResp	(strNewNetProtocolInfo	*, strBufInfo *);
static uint32   busFbProgWriteReqAndResp	(strNewNetProtocolInfo	*, strBufInfo *);

extern uint32	fbCodeOutputDataReadWithNumber(uint32, uint32, uint32, uint32, uint32, uint32 * ,uint32 *);
extern uint32	fbCodeSpecDataReadWithNumber(uint32, uint32, uint32, uint32, uint32,  uint32*, uint32*, uint32*);

extern uint32	fbCodeOutputDataWriteWithType(uint32,uint32,uint32,uint32,uint32,uint32,uint32*);
extern uint32	fbCodeSpecDataWriteWithType(uint32,uint32,uint32,uint32,uint32,uint32*);
extern uint32	fbCodeSimFlagCheckRead(uint32, uint32 *);

extern uint32	segmentModeChange ( uint32 , uint32 ,uint32 );

extern uint32 specActionDataCheck(uint32 *, uint32 );
extern uint32 sfcInfoDownloadReqAndrespFunc(strNewNetProtocolInfo*, strBufInfo*);
extern uint32 sfcLinkDownloadReqAndrespFunc(strNewNetProtocolInfo*, strBufInfo*);
extern uint32 sfcActCodeDownloadReqAndrespFunc(strNewNetProtocolInfo*, strBufInfo*);
extern uint32 sfcTrsCodeDownloadReqAndrespFunc(strNewNetProtocolInfo*, strBufInfo*);
extern uint32 sfcStatusReadReqAndrespFunc(strNewNetProtocolInfo*, strBufInfo*);
extern uint32 sfcStepJumpReqAndrespFunc(strNewNetProtocolInfo*, strBufInfo*);

extern uint32 jmpFbCodeCheck(uint32);
extern uint32 jmpFbAddressCheck(uint32,uint32,strFbMemInfo*);
extern void		eightByteConversion( uint32 *, uint32 *, uint8 );
extern void		mySwap(void *, void *, uint32);



uint32	(*CodeAccessCmdOPFuncList[]) 
			(strNewNetProtocolInfo	*,strBufInfo *)
= {
	optionCodeCmdNullFunc,							
	fbSegmentDownloadReqAndRespFunc,				
 	fbCodeDownloadReqAndRespFunc,					 
	fbSequenceTblDownloadReqAndRespFunc,			
  	fbCodeOutputReadReqAndRespFunc,					 
	fbCodeOutputWriteReqAndRespFunc,				 
	fbCodeParameterReadReqAndRespFunc,				 
 	fbCodeParameterWriteReqAndRespFunc,				
	fbCodeStatusReadReqAndRespFunc,						

	optionCodeCmdNullFunc						
};



uint32 fbSegmentDownloadReqAndRespFunc 
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR;

    status = localFbSegTblWriteReqAndResp(reqMsgPtr, respMsgPtr);
	if(status != NO_ERROR)
		return(status);
				 
    status = busFbSegTblWriteReqAndResp(reqMsgPtr, respMsgPtr);
    
    return(status);
}


uint32 fbCodeDownloadReqAndRespFunc 
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR;
	status = localFbProgWriteReqAndResp	(
										reqMsgPtr,
            							respMsgPtr
										);
	if(status != NO_ERROR)
    	return(status);

	status = busFbProgWriteReqAndResp	(
										reqMsgPtr,
            							respMsgPtr
										);
    return(status);
}


uint32 fbSequenceTblDownloadReqAndRespFunc 
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR, errStatus = NO_ERROR, checkStatus = NO_ERROR;
	uint32	respDataLen = 0;
	uint32  checkLoop, segLoop,fbBlkAddrLoop, setLoop,fbCodeLoop;
	uint32	*reqDataPtr 	= (uint32 *)&reqMsgPtr->dataBuff[12];
	uint32	*respDataPtr	= (uint32 *)&respMsgPtr->dataBuf[12];
	uint32	*saveReqDataPtr;
	uint32	reqSegId, maxSegId,	totalFbCodeNum, curFbCodeNum, calcuTotalFbCodeNum;
	uint32	reqExcutionId, reqFbBlkAddr, reqFbCode;
	strFbTaskTblMemInfo	segTblInfo;
	strFbMemInfo	fbMemInfo, *fbMemInfoPtr;

	strFbDataGrHeaderInfo *reqHeaderptr;
	strModuleExecInfo sysExeInfo;
	strFbMemInfo jumpAddrInfo;

	
	reqHeaderptr = (strFbDataGrHeaderInfo *)reqMsgPtr->dataBuff;
    memoryCopy((uint8*)respMsgPtr-> dataBuf,(uint8*)reqHeaderptr,
										sizeof(strFbDataGrHeaderInfo) );
	respDataLen = sizeof(strFbDataGrHeaderInfo);   
	
	 
    if(reqHeaderptr->memoryKind != LOGIC_ID)
    {
		status = L_MODULE_ID_ERR;
        setErrorCodeWithVal( 	__FILE__,__LINE__,__FUNCTION__,status,
        						"Memory Kind",reqHeaderptr->memoryKind,
        						"Not Using",0,
        						"Not Using",0 
					  		);
		goto MODULE_END;
	}
	
    
    if( (reqHeaderptr->segmentNum == 0)|| (reqHeaderptr->segmentNum > MAX_LOGIC_TASK_NUM))
    {
		status = L_ITEM_NO_ERR;
        setErrorCodeWithVal( 	__FILE__,__LINE__,__FUNCTION__,status,
        						"Item Number",reqHeaderptr->segmentNum,
        						"Not Using",0,
        						"Not Using",0 
					  		);
		goto MODULE_END;
	}

	saveReqDataPtr = reqDataPtr;
	
	memoryClear( (uint8 *)&sysExeInfo, sizeof(strModuleExecInfo));
	status = fbSysExeModuleInfoRead(reqHeaderptr->memoryKind, &sysExeInfo );
	if(status != NO_ERROR) 
	{
		checkStatus =1;
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
	}

	for(checkLoop = 0; checkLoop < reqHeaderptr->segmentNum; checkLoop++) 
	{
		
		reqSegId = *reqDataPtr++;	
	
		if(reqSegId > MAX_LOGIC_TASK_NUM) 
		{
			status = L_SEG_ID_ERR;
        	setErrorCodeWithVal( 	__FILE__,__LINE__,__FUNCTION__,status,
        							"Req Task Id",reqSegId,
        							"Not Using",0,
        							"Not Using",0 
					  		);
			goto MODULE_END;				
		}
		
		
		maxSegId = 0;
		for(segLoop = 1; segLoop <= MAX_LOGIC_TASK_NUM; segLoop++) 
		{
			memoryClear( (uint8 *)&segTblInfo, sizeof(strFbTaskTblMemInfo));
		
   			(void)getFbTaskTblInfo( 
								LOGIC_ID,
           						segLoop,
								(strFbTaskTblMemInfo	*) &segTblInfo
								);

			if(segTblInfo.taskId > maxSegId)
				maxSegId = segTblInfo.taskId;
			
			if(segTblInfo.taskId == reqSegId) 
				break;
		}

		if((segTblInfo.taskId == 0) || (segLoop > MAX_LOGIC_TASK_NUM))
		{
			status = ONLINE_DWL_SEG_EXIST_ERR;
       		setErrorCodeWithVal( 	__FILE__,
       								__LINE__,
           	    	        		__FUNCTION__,
              	     	    		status,
                	   	    		"Req Segment ID",reqSegId,
                 	  	    		"Current Max Task Id", maxSegId,
                  	 	    		"NOT Using", 0 
				  		  		);
		}

		
		else if(segTblInfo.taskMode != PCS_DOWNLOAD) 
		{
			status = L_PCS_MODE_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__, __FUNCTION__,status,
                	   	    		"Req Segment ID",reqSegId,
                 	  	    		"Seg Mode", segTblInfo.taskMode,
                  	 	    		"NOT Using", 0 
				  		  		);
		}

		 
		else if(segTblInfo.recvMemSize != segTblInfo.memInputSize) 
		{
			status = FB_PARA_MISMATCH_ERR;
       		setErrorCodeWithVal( 	__FILE__,__LINE__,__FUNCTION__,status,
								    (uint8 *)"RECV Input Size", segTblInfo.recvMemSize,
								 	(uint8 *)"CALCU Input Size",  segTblInfo.memInputSize,
									(uint8 *)"TASK ID",  segTblInfo.taskId
				  		  		);
		}

		if(status != NO_ERROR) 
		{
			*respDataPtr 	= reqHeaderptr->segmentNum; 
			*respDataPtr++ 	= reqSegId ; 
			*respDataPtr++ 	= status ;
			respDataLen 	+= DOUBLE_SIZE*3;
			
			goto MODULE_END;
		}
		
		totalFbCodeNum	= (*reqDataPtr >> 16) & 0xffff;
		curFbCodeNum	= *reqDataPtr++ & 0xffff;

		calcuTotalFbCodeNum = 0;
		
		for(fbBlkAddrLoop = 1; fbBlkAddrLoop <= sysExeInfo.curfbMaxBlockAddr; fbBlkAddrLoop++) 
		{

   			(void)fbCodeMemInfoRead	(
										LOGIC_ID,
       									fbBlkAddrLoop,
       									(strFbMemInfo *) &fbMemInfo
   									);
			if(fbMemInfo.fbId == 0x0) 
				continue;
				
			if(fbMemInfo.fbTaskId != reqSegId) 
				continue;			
		
			calcuTotalFbCodeNum++;				
		} 
		
		if(calcuTotalFbCodeNum != totalFbCodeNum) 
		{
			status = ONLINE_FB_CODE_NUM_MISMATCH_ERR;
       		setErrorCodeWithVal( 	__FILE__,
       								__LINE__,
           	    	        		__FUNCTION__,
              	     	    		status,
                	   	    		"Req Segment ID",reqSegId,
                 	  	    		"Req Total FB Num", totalFbCodeNum,
                  	 	    		"Calcu Total FB Num", calcuTotalFbCodeNum 
				  		  		);
		}

		
		if(curFbCodeNum > totalFbCodeNum)
		{
			status = ONLINE_FB_CODE_NUM_MISMATCH_ERR;
       		setErrorCodeWithVal( 	__FILE__,
       								__LINE__,
           	    	        		__FUNCTION__,
              	     	    		status,
                	   	    		"Req Segment ID",reqSegId,
                 	  	    		"Req Total FB Num", totalFbCodeNum,
                  	 	    		"Req Cur FB Num", curFbCodeNum 
				  		  		);
		}

		if(status != NO_ERROR) 
		{
			*respDataPtr 	= reqHeaderptr->segmentNum; 
			*respDataPtr++ 	= reqSegId ; 
			*respDataPtr++ 	= status ;
			respDataLen 	+= DOUBLE_SIZE*3;
			
			goto MODULE_END;
		}
		
		
		onlineDwlFbCodeSeqIdVarSet(reqSegId, curFbCodeNum,totalFbCodeNum);

		for(fbCodeLoop = 0; fbCodeLoop < curFbCodeNum; fbCodeLoop++) 
		{
			errStatus = 0;
			
			reqFbCode		= (*reqDataPtr >> 16)&0xffff;
			reqFbBlkAddr 	= *reqDataPtr++ & 0xffff;
			
			reqExcutionId	= (*reqDataPtr++);
			
			if(reqExcutionId == 0x0) 
			{
				errStatus = FB_SEQ_ID_NULL_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, errStatus);	
			}
			
			else if((reqFbBlkAddr > MAX_MPU_BLOCK_ADDR) || (reqFbBlkAddr < 0)) 
			{
				errStatus = SEG_BLK_ADDR_EXCEED_ERR;	
   				setErrorCodeWithVal( 	__FILE__,
   										__LINE__,
       	    	        				__FUNCTION__,
           	     	    				errStatus,
              	   	    				"Req Segment ID",reqSegId,
               	  	    				"Req Fb Addr", reqFbBlkAddr,
               	 	    				"End Blk Addr", MAX_MPU_BLOCK_ADDR 
			  		  				);			
			}
			
			else 
			{
  				(void)fbCodeMemInfoRead	(
										LOGIC_ID,
       									reqFbBlkAddr,
       									(strFbMemInfo *) &fbMemInfo
   										);
				
				if(fbMemInfo.fbId != reqFbCode) 
				{
					errStatus = ONLINE_FB_CODE_MISMATCH_ERR;	
       				setErrorCodeWithVal( 	__FILE__,
       										__LINE__,
           	    	        				__FUNCTION__,
              	     	    				errStatus,
                 	  	    				"Req Fb Addr", reqFbBlkAddr,
                  	 	    				"Req Fb Code", reqFbCode,
                  	 	    				"Read Fb Code", fbMemInfo.fbId 
				  		  				);
				}
				else
				{
					
					if(jmpFbCodeCheck(fbMemInfo.fbId) == IS_JUMP_CODE)
					{
						memoryClear((uint8*)&jumpAddrInfo, sizeof(strFbMemInfo));
						errStatus = jmpFbAddressCheck(fbMemInfo.fbId,
												   fbMemInfo.fbFuncAddr,
												   &jumpAddrInfo);
						if(errStatus != NO_ERROR)
						{
							checkStatus = 1;
							break; 
						}
					}
				}
				
			}
			
			
			if(errStatus != NO_ERROR)
			{
				checkStatus = 1;
				break; 
			}
		}
		
		*respDataPtr = reqHeaderptr->segmentNum; 
		*respDataPtr++ = reqSegId ; 
		*respDataPtr++ = errStatus ;
		respDataLen += DOUBLE_SIZE*3;

	}
	
	
	if( (status != NO_ERROR) || (checkStatus != NO_ERROR))
		goto MODULE_END;
	
	
	reqDataPtr = saveReqDataPtr;

	for(setLoop = 0; setLoop < reqHeaderptr->segmentNum; setLoop++)
	{
		
		reqSegId = *reqDataPtr++;
		
		totalFbCodeNum	= (*reqDataPtr >> 16) & 0xffff;
		curFbCodeNum	= *reqDataPtr++ & 0xffff;
		
		for(fbCodeLoop = 0; fbCodeLoop < curFbCodeNum; fbCodeLoop++)
		{

			
			reqFbCode 		= (*reqDataPtr >> 16) & 0xffff;
			reqFbBlkAddr	= *reqDataPtr++ & 0xffff;
			
			reqExcutionId	= (*reqDataPtr++);
			
			fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (reqFbBlkAddr-1);			
			fbMemInfoPtr->fbExecSequenceId = reqExcutionId;
											
			
			fbMemInfoPtr = gMpuFbMemInfoBusPtr + (reqFbBlkAddr-1);			
			fbMemInfoPtr->fbExecSequenceId = reqExcutionId;			
		}
	}
			
MODULE_END:

	respMsgPtr->dataLen = respDataLen;
	
	return(status);
}


uint32 fbCodeOutputReadReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32	respDataLen = 0;
	uint32	*reqDataPtr	;
	uint32	*respDataPtr ;

	uint32  i, j,  dataNum, outputSize;
	uint32	taskId, codeNumber, fbCodeId, fbBlockAddr;
#define	MAX_FB_OUT_READ_NUM	(128)
	uint32  outputIndex, outputNum, outputData[MAX_FB_OUT_READ_NUM];  
	uint32	accumCodeNum;

	strTotalSegDataInfo	segData;
	strFbMemInfo	fbMemInfo;
	strModuleInfo		moduleInfo;
	strFbTaskTblMemInfo	segInfo;

	strFbDataGrHeaderInfo *reqHeaderptr;
	strFbCodeTypePointerInfo	fbParaTypeInfo;
	uint32	forcingChkFlag ,respType, k;
	uint16  fbRetVal = 0;
	uint32	*outputTypePtr = NULL;

	
	reqHeaderptr = (strFbDataGrHeaderInfo *)reqMsgPtr->dataBuff;
	
    memoryCopy((uint8*)respMsgPtr-> dataBuf,(uint8*)reqHeaderptr,
										sizeof(strFbDataGrHeaderInfo) );
										
	respDataLen = sizeof(strFbDataGrHeaderInfo);         
	
	
	retVal = moduleInfoRead(reqHeaderptr->memoryKind, &moduleInfo);

    if(retVal) 
    {
    	status = L_MODULE_ID_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, retVal);
		
		goto MODULE_END;
    }

    
    if( (reqHeaderptr->segmentNum == 0)|| (reqHeaderptr->segmentNum > moduleInfo.maxSegmentNum))
    {
		status = L_ITEM_NO_ERR;
        retVal = L_ITEM_NO_ERR;
		setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__, retVal,
	                        "Recv Item Num", reqHeaderptr->segmentNum,
	                        "Max Item Num", moduleInfo.maxSegmentNum,
	                        "Not Used", 0
				  		  );
			  		  
		goto MODULE_END;
	}
	
	reqDataPtr	= (uint32 *)&reqMsgPtr->dataBuff[12];
	
	accumCodeNum = 0;
	
    for(i = 0; i < reqHeaderptr->segmentNum; i++) 
	{
		segData.segInfo[i].startDataPtr = reqDataPtr;

		taskId = (*reqDataPtr >> 16)&0xffff;	

        segData.segInfo[i].taskId 	= taskId;
		segData.segInfo[i].status 	= NO_ERROR;

        
        if(	(taskId < 1) || (taskId > moduleInfo.maxSegmentNum)) 
        {
			segData.segInfo[i].status = L_SEG_ID_ERR & 0xff;
			retVal = L_SEG_ID_ERR;
			status = L_SEG_ID_ERR;			
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__,retVal,
                    			"Recv Task Id", taskId,
                    			"Max Task Id", moduleInfo.maxSegmentNum,
                    			"Not Used", 0
			  		  			);
			
            codeNumber = *reqDataPtr++ & 0xffff;
            
			
            accumCodeNum += codeNumber;
			if(accumCodeNum > MAX_FB_OUT_READ_NUM)
			{
				status = L_ITEM_NO_ERR;
		        retVal = L_ITEM_NO_ERR;
				setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
			                        "Total Code Num", accumCodeNum,
			                        "MAX_FB_CODE_OUTPUT_NUM", MAX_FB_OUT_READ_NUM,
			                        "Not Used", 0
						  		  );
				goto MODULE_END;

			}

			for(dataNum = 0; dataNum < codeNumber; dataNum++) 
			{
                fbCodeId  	= (*reqDataPtr >> 16)&0xffff;
				fbBlockAddr = *reqDataPtr++& 0xffff;
				outputIndex = (*reqDataPtr >> 16)&0xffff;
				outputNum	 = *reqDataPtr++& 0xffff;;
            }
			
            continue;
        }
		
        
        memoryClear ( (uint8 *) &segInfo, sizeof(strFbTaskTblMemInfo));
		retVal = getFbTaskTblInfo ( 	moduleInfo.moduleId,
           							taskId,
                                    (strFbTaskTblMemInfo *)&segInfo
								);
		if(retVal)
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, retVal);

        codeNumber = *reqDataPtr++ & 0xffff;
		
		
		accumCodeNum += codeNumber;
		if(accumCodeNum > MAX_FB_OUT_READ_NUM)
		{
			status = L_ITEM_NO_ERR;
	        retVal = L_ITEM_NO_ERR;
				setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
			                        "Total Code Num", accumCodeNum,
			                        "MAX_FB_CODE_OUTPUT_NUM", MAX_FB_OUT_READ_NUM,
			                        "Not Used", 0
						  		  );
				goto MODULE_END;
		}

	    for(dataNum = 0; dataNum < codeNumber; dataNum++) 
	    {
            fbCodeId	= (*reqDataPtr >> 16) & 0xffff;
			fbBlockAddr = *reqDataPtr++ & 0xffff;
			outputIndex = (*reqDataPtr >> 16)&0xffff;
			outputNum = *reqDataPtr++& 0xffff;;
				
			if( (fbBlockAddr < 0) || (fbBlockAddr > moduleInfo.maxFbBlockAddr) ) 
			{
                retVal = L_FB_CODE_ERR;
                status = L_FB_CODE_ERR;
   				segData.segInfo[i].status = L_FB_ADDR_ERR & 0xff;
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,	
	                                "Recv Block Addr", fbBlockAddr,
	                                "Start Block Addr",0,
	                                "End Block Addr", moduleInfo.maxFbBlockAddr
									);
				goto CHECK_END;
            }
			
            
            memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
			retVal = fbMemInfoRead (	moduleInfo.moduleId,
            							fbBlockAddr,
                                        (strFbMemInfo *) &fbMemInfo
									);
			if(retVal) 
			{
            	segData.segInfo[i].status = L_FB_CODE_ERR & 0xff;
                status = L_FB_CODE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				
				goto CHECK_END;
			}
			
			
            if(taskId != fbMemInfo.fbTaskId) 
            {
               	retVal = L_SEG_ID_ERR;
                status = L_SEG_ID_ERR;
	           	segData.segInfo[i].status = L_SEG_ID_ERR & 0xff;
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,	
                                    "Recv Task Id", taskId,
                                    "Read Task Id",fbMemInfo.fbTaskId,
									"Recv Block Addr", fbBlockAddr 
               	    		 		);
				goto CHECK_END;
            }

			
            if(fbCodeId != fbMemInfo.fbId) 
            {
               	retVal = L_FB_CODE_ERR;
                status = L_FB_CODE_ERR;
	           	segData.segInfo[i].status = L_FB_CODE_ERR & 0xff;
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,	
                                    "Recv Fb Code", fbCodeId,
                                    "Read Fb Code",fbMemInfo.fbId,
									"Recv Block Addr", fbBlockAddr 
               	    		 		);
				goto CHECK_END;
            }

			
			if( 
				( (outputIndex + outputNum -1) > fbMemInfo.outputNo) ||
				( outputIndex < 1 ) || ( outputIndex > fbMemInfo.outputNo ) ||
				( outputNum > fbMemInfo.outputNo))
			{
               	retVal = FB_OUTPUT_NUM_ERR;
                status = FB_OUTPUT_NUM_ERR;
	           	segData.segInfo[i].status = FB_OUTPUT_NUM_ERR & 0xff;
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,	
                                    "Recv Output Num", outputNum,
                                    "Read Output Num",fbMemInfo.outputNo,
									"Recv Fb Code", fbCodeId 
               	    		 		);
            }
CHECK_END:
			continue;
		}

	}

	reqDataPtr	= (uint32 *)&reqMsgPtr->dataBuff[12];
   	respDataPtr = (uint32 *)&respMsgPtr->dataBuf[12];
	
    for(i = 0; i < reqHeaderptr->segmentNum; i++) 
	{
		taskId 		= (*reqDataPtr >> 16)&0xffff;	
        codeNumber	 = *reqDataPtr++ & 0xffff;	
		
        *respDataPtr 	= taskId << 16;		
		*respDataPtr++ 	|= codeNumber & 0xffff;
        respDataLen 	+= DOUBLE_SIZE;	    
	
        for(dataNum = 0; dataNum < codeNumber; dataNum++) 
		{
            fbCodeId	= (*reqDataPtr >> 16) & 0xffff;
			fbBlockAddr = *reqDataPtr++ & 0xffff;

			outputIndex = (*reqDataPtr >> 16) & 0xffff;
			outputNum   = *reqDataPtr++ & 0xffff;

            
            memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
		
            
            fbMemInfo.fbRunStatus = 1;
			retVal = fbMemInfoRead (	moduleInfo.moduleId,
            							fbBlockAddr,
                                        (strFbMemInfo *) &fbMemInfo
									);
			if(retVal)
				setErrorCode(__FILE__, __LINE__, __FUNCTION__,retVal);

			retVal = fbCodeParaTypePointerGet ( fbCodeId,  
								(strFbCodeTypePointerInfo*)&fbParaTypeInfo 	);
			if(retVal != NO_ERROR) 
			{
				outputTypePtr = NULL;
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,	
									"Req Fb Code ID", fbCodeId,
									"Not Using", 0,
									"Not Using", 0
		                	    	);
			}
			else
			{
				outputTypePtr = (uint32 *)fbParaTypeInfo.outputTypeStartAddr;
			}

			
			if(fbMemInfo.fbRunStatus != NO_ERROR)	fbRetVal = 1;
			else fbRetVal = 0;

			*respDataPtr   	= fbCodeId << 16;
			*respDataPtr++ 	|= ( fbBlockAddr | (fbRetVal << 15) );
   			respDataLen 	+= DOUBLE_SIZE;	    
		
			outputSize = 0; 
			if(retVal == NO_ERROR) 
			{
				
				if( 
					( (outputIndex + outputNum -1) > fbMemInfo.outputNo) ||
					( outputIndex < 1 ) || ( outputIndex > fbMemInfo.outputNo ) ||
					( outputNum > fbMemInfo.outputNo)
				) 
					retVal = FB_OUTPUT_NUM_ERR;

				
				if(retVal==NO_ERROR)
				{
					retVal = fbCodeOutputDataReadWithNumber(	
									            				moduleInfo.moduleId,
																fbBlockAddr, fbCodeId,
																outputIndex, outputNum,
																(uint32*)&outputSize, (uint32*)outputData
															);
				}
				
				if(retVal)
				{
					setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,	
										"Task Id", 			taskId,
										"Recv Block Addr", fbBlockAddr,
										"Recv Fb Code", 	fbCodeId
                                   		);
				}
			}

						
			if(retVal != NO_ERROR) 
				continue; 

			/* 모드 Fb Code는 Default로 forcing Enable이므로 '1'로 Set */
			forcingChkFlag = FORCING_ENABLE;
			
			if(reqHeaderptr->memoryKind == LOGIC_ID) 
			{
				forcingChkFlag |= ((fbMemInfo.forcingFlag) & 0x1)<<1;
			}

			*respDataPtr 	= forcingChkFlag <<16;
			*respDataPtr++ 	|= retVal;
			respDataLen 	+= DOUBLE_SIZE;	    

			*respDataPtr 	= outputNum << 16;		
			*respDataPtr++ 	|= outputSize & 0xffff;
			respDataLen 	+= DOUBLE_SIZE;	    


			
			k = 0; 
			for(j=0 ; j < outputNum ; j++ )
			{
				
				if( (moduleInfo.moduleId == LOGIC_ID) ) 
				{
					*respDataPtr = ((outputIndex + j)<<16); 
				}
				
				
				if(outputTypePtr != NULL )
				{
					if( CHK_IS_ANY_TYPE(*(outputTypePtr + outputIndex + j - 1)) ) 
					{
					   respType = fbMemInfo.paraRunType;
					}
					else 
					{
						respType =  ( GET_BASIC_DATA_TYPE(*(outputTypePtr + outputIndex+ j - 1)));
					}
				}
				else 
					respType = 0;

				*respDataPtr++ 	|=  respType&0xf;
				respDataLen 	+= DOUBLE_SIZE;	    

				
				if(outputTypePtr != NULL )
				{
					
					if ( GET_TYPE_SIZE(*(outputTypePtr + outputIndex + j - 1))  == SIZE64_TYPE ) 
					{
						eightByteConversion((uint32*)respDataPtr,(uint32*)&outputData[k++],0);
						k++;
						respDataPtr++;
						respDataLen += DOUBLE_SIZE;	    
					}
					else 
					{
						memoryCopy ( (uint8 *) respDataPtr, 
            					 (uint8 *) &outputData[k++],
								 DOUBLE_SIZE
							   );	
					}
					
					respDataPtr++;
					respDataLen += DOUBLE_SIZE;	   						
				}
			}
        } 
    } 

MODULE_END:
	
	respMsgPtr->dataLen = respDataLen;
	
	return(status);
}


uint32 fbCodeOutputWriteReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32	respDataLen = 0;
	uint32	*reqDataPtr	;
	uint32	*respDataPtr ;

	uint32  i, dataNum, j;
	uint32	taskId, codeNumber, fbCodeId, fbBlockAddr;
#define MAX_FB_OUT_WRITE_CODE_NUM	(150)
	uint32  outputNum, outputIndex, outputData[2];  
	uint32	accumCodeNum;
	
	strTotalSegDataInfo	segData;
	strFbMemInfo	fbMemInfo;
	strModuleInfo		moduleInfo;
	strFbTaskTblMemInfo	segInfo;

	strFbDataGrHeaderInfo *reqHeaderptr;
	strFbCodeTypePointerInfo	fbParaTypeInfo;
	uint32	outputType, *outputTypePtr=NULL ;
	uint32  forcingFlag;

	
	reqHeaderptr = (strFbDataGrHeaderInfo *)reqMsgPtr->dataBuff;
    memoryCopy((uint8*)respMsgPtr-> dataBuf,(uint8*)reqHeaderptr,
										sizeof(strFbDataGrHeaderInfo) );
	respDataLen = sizeof(strFbDataGrHeaderInfo);         
	
	
	retVal = moduleInfoRead(reqHeaderptr->memoryKind, &moduleInfo);

    if(retVal) 
    {
    	status = L_MODULE_ID_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, retVal);
		goto MODULE_END;
    }

    
    if( (reqHeaderptr->segmentNum == 0)|| (reqHeaderptr->segmentNum > MAX_LOGIC_TASK_NUM))
    {
		status = L_ITEM_NO_ERR;
        retVal = L_ITEM_NO_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,	
		                    "Recv Item Num", reqHeaderptr->segmentNum,
		                    "Max Item Num", moduleInfo.maxSegmentNum,
		                    "Not Used", 0
				  		  );
		goto MODULE_END;
	}
	
	reqDataPtr	= (uint32 *)&reqMsgPtr->dataBuff[12];
	
	accumCodeNum = 0;
	
    for(i = 0; i < reqHeaderptr->segmentNum; i++) 
	{
		segData.segInfo[i].startDataPtr = reqDataPtr;

		taskId = (*reqDataPtr >> 16)&0xffff;	

        segData.segInfo[i].taskId 	= taskId;
		segData.segInfo[i].status 	= NO_ERROR;

        
        if(	(taskId < 1) || (taskId > moduleInfo.maxSegmentNum)) 
        {
			segData.segInfo[i].status = L_SEG_ID_ERR & 0xff;
			retVal = L_SEG_ID_ERR;
			status = L_SEG_ID_ERR;			
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,	
                    			"Recv Task Id", taskId,
                    			"Max Task Id", moduleInfo.maxSegmentNum,
                    			"Not Used", 0
			  		  			);
			
            codeNumber = *reqDataPtr++ & 0xffff;

			
			accumCodeNum += codeNumber;
			
			if(accumCodeNum > MAX_FB_OUT_WRITE_CODE_NUM)
			{
				status = L_ITEM_NO_ERR;
		        retVal = L_ITEM_NO_ERR;
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,	
			                        "Total Code Output Num", accumCodeNum,
			                        "MAX_FB_OUT_WRITE_CODE_NUM", MAX_FB_OUT_WRITE_CODE_NUM,
			                        "Not Used", 0
						  		  );
				goto MODULE_END;
			}
			
			for(dataNum = 0; dataNum < codeNumber; dataNum++) 
			{
                fbCodeId  = (*reqDataPtr >> 16)&0xffff;
				fbBlockAddr = *reqDataPtr++& 0xffff;
				outputNum = *reqDataPtr++;

            }
			
            continue;
        }
		
        
        memoryClear ( (uint8 *) &segInfo, sizeof(strFbTaskTblMemInfo));
		retVal = getFbTaskTblInfo ( 	moduleInfo.moduleId,
           							taskId,
                                    (strFbTaskTblMemInfo *)&segInfo
								);
		if(retVal)
			setErrorCode(	__FILE__,__LINE__, __FUNCTION__, retVal);
		
        codeNumber = *reqDataPtr++ & 0xffff;
    		
		accumCodeNum += codeNumber;
		if(accumCodeNum > MAX_FB_OUT_WRITE_CODE_NUM)
		{
			status = L_ITEM_NO_ERR;
	        retVal = L_ITEM_NO_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,	
		                        "Total Code Output Num", accumCodeNum,
		                        "MAX_FB_OUT_WRITE_CODE_NUM", MAX_FB_OUT_WRITE_CODE_NUM,
		                        "Not Used", 0
					  		  );
			goto MODULE_END;
		}
			
	    for(dataNum = 0; dataNum < codeNumber; dataNum++) 
	    {
            fbCodeId	= (*reqDataPtr >> 16) & 0xffff;
			fbBlockAddr = *reqDataPtr++ & 0xffff;
			outputNum   = *reqDataPtr++ ;
				
			if( (fbBlockAddr < 0) || (fbBlockAddr > MAX_MPU_BLOCK_ADDR) ) 
			{
                retVal = L_FB_CODE_ERR;
                status = L_FB_CODE_ERR;
   				segData.segInfo[i].status = L_FB_ADDR_ERR & 0xff;
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,	
	                                "Recv Block Addr", fbBlockAddr,
	                                "Start Block Addr",0,
	                                "End Block Addr", MAX_MPU_BLOCK_ADDR
           	    		 			);
				goto CHECK_END;
            }
			
            
            memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
			retVal = fbMemInfoRead (	moduleInfo.moduleId,
            							fbBlockAddr,
                                        (strFbMemInfo *) &fbMemInfo
									);
			if(retVal) 
			{
            	segData.segInfo[i].status = L_FB_CODE_ERR & 0xff;
                status = L_FB_CODE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, retVal);
				
				goto CHECK_END;
			}
			
			
            if(taskId != fbMemInfo.fbTaskId) 
            {
               	retVal = L_SEG_ID_ERR;
                status = L_SEG_ID_ERR;
	           	segData.segInfo[i].status = L_SEG_ID_ERR & 0xff;
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,	
                                    "Recv Task Id", taskId,
                                    "Read Task Id",fbMemInfo.fbTaskId,
									"Recv Block Addr", fbBlockAddr 
               	    		 		);
				goto CHECK_END;
            }

			
            if(fbCodeId != fbMemInfo.fbId) 
            {
               	retVal = L_FB_CODE_ERR;
                status = L_FB_CODE_ERR;
	           	segData.segInfo[i].status = L_FB_CODE_ERR & 0xff;
				setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__, retVal,
                                    "Recv Fb Code", fbCodeId,
                                    "Read Fb Code",fbMemInfo.fbId,
									"Recv Block Addr", fbBlockAddr 
               	    		 		);
				goto CHECK_END;
            }
			
			if(outputNum > fbMemInfo.outputNo) 
			{
               	retVal = FB_OUTPUT_NUM_ERR;
                status = FB_OUTPUT_NUM_ERR;
	           	segData.segInfo[i].status = FB_OUTPUT_NUM_ERR & 0xff;
				setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__, retVal,
                                    "Recv Output Num", outputNum,
                                    "Read Output Num",fbMemInfo.outputNo,
									"Recv Fb Code", fbCodeId 
               	    		 		);
				goto CHECK_END;
            }

			retVal = fbCodeParaTypePointerGet ( fbCodeId,  (strFbCodeTypePointerInfo	*)&fbParaTypeInfo 	);
			if(retVal != NO_ERROR) 
			{
				outputTypePtr = NULL;
		        segData.segInfo[i].status = FB_CODE_OUTPUT_TYPE_ERR & 0xff;
				setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__, retVal,
            	            		"fb Code", fbCodeId,
                	            	"Not Using", 0,
                    				"Not Using", 0
									);
			}
			else
			{
				outputTypePtr = (uint32 *)fbParaTypeInfo.outputTypeStartAddr;
			}
			
			for(j=0; j < outputNum ; j++ )
			{
				outputIndex  =  (*reqDataPtr >> 16) & 0xffff;
				outputType   = *reqDataPtr++ & 0xffff;
			
				
				if( (outputIndex < 1)||(outputIndex > fbMemInfo.outputNo) ) 
				{
               		retVal = FB_CODE_OUTPUT_INDEX_ERR;
					status = FB_CODE_OUTPUT_INDEX_ERR;
	           		segData.segInfo[i].status = FB_CODE_OUTPUT_INDEX_ERR & 0xff;
					setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__, retVal,
										"Recv Output Index", outputIndex,
										"MAX Output Index",fbMemInfo.outputNo,
										"Recv Fb Code", fbCodeId 
	           	    		 			);
					goto CHECK_END;
				}

				
				if(outputTypePtr != NULL) 
				{
					if(!CHK_IS_ANY_TYPE(*(outputTypePtr + outputIndex - 1)))  
					{
						if( outputType != ( GET_BASIC_DATA_TYPE(*(outputTypePtr + outputIndex - 1)) ) ) 
						{
							segData.segInfo[i].status = FB_CODE_OUTPUT_TYPE_ERR & 0xff;
							setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__, FB_CODE_OUTPUT_TYPE_ERR,
			                					"fb Code", fbCodeId,
			                    				"Recv Type", outputType,
												"Read Type", 
												GET_BASIC_DATA_TYPE(*(outputTypePtr + outputIndex - 1))
												);
						}
					}
				}
				
				if(GET_BASIC_SIZE(outputType) == SIZE64_TYPE ) 
				{
					eightByteConversion((uint32*)&outputData[0],(uint32*)reqDataPtr++,0);
					reqDataPtr++;
				}
				else 
				{
					outputData[0] = *reqDataPtr++;
					outputData[1] = 0;
				}

				
				if(CHK_IS_ANY_TYPE(*(outputTypePtr + outputIndex - 1)))  
				{
					switch(GET_TYPE_DATA_SIZE(fbMemInfo.paraRunType))
					{
						case SIZE08_TYPE:
							if(outputData[0] > 0xff) 
							{
								status = ANY_FUNC_OUT_SIZE_ERR;
							    retVal = ANY_FUNC_OUT_SIZE_ERR;
								segData.segInfo[i].status = ANY_FUNC_OUT_SIZE_ERR & 0xff;
								setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__, ANY_FUNC_OUT_SIZE_ERR,
				                					"fb Code", fbCodeId,
				                    				"Run Type", GET_TYPE_DATA_SIZE(fbMemInfo.paraRunType)>>12,
													"Recv Data", outputData[0] );
							}

						break;
						case SIZE16_TYPE:
							if(outputData[0] > 0xffff)
							{
								status = ANY_FUNC_OUT_SIZE_ERR;
							    retVal = ANY_FUNC_OUT_SIZE_ERR;
								segData.segInfo[i].status = ANY_FUNC_OUT_SIZE_ERR & 0xff;
								setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__, ANY_FUNC_OUT_SIZE_ERR,
				                					"fb Code", fbCodeId,
				                    				"Run Type", GET_TYPE_DATA_SIZE(fbMemInfo.paraRunType)>>12,
													"Recv Data", outputData[0] );
							}

						break;
						case SIZE32_TYPE:
							if(outputData[1] > 0x0) 
							{
								status = ANY_FUNC_OUT_SIZE_ERR;
							    retVal = ANY_FUNC_OUT_SIZE_ERR;
								segData.segInfo[i].status = ANY_FUNC_OUT_SIZE_ERR & 0xff;
								setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__, ANY_FUNC_OUT_SIZE_ERR,
				                					"fb Code", fbCodeId,
				                    				"Run Type", GET_TYPE_DATA_SIZE(fbMemInfo.paraRunType)>>12,
													"Recv Data", outputData[0] );
							}
						break;	
						
					}
					
					if(status != NO_ERROR) 
						goto CHECK_END;
				}				
			}  
			
CHECK_END:
		continue;
		}

	}
		
	reqDataPtr	= (uint32 *)&reqMsgPtr->dataBuff[12];
   	respDataPtr = (uint32 *)&respMsgPtr->dataBuf[12];
	
    for(i = 0; i < reqHeaderptr->segmentNum; i++) 
	{
		taskId = (*reqDataPtr >> 16)&0xffff;	
        codeNumber = *reqDataPtr++ & 0xffff;
	
        *respDataPtr = taskId << 16;		
		*respDataPtr++ |= codeNumber & 0xffff;
        respDataLen += DOUBLE_SIZE;	    

        for(dataNum = 0; dataNum < codeNumber; dataNum++) 
		{
            fbCodeId	= (*reqDataPtr >> 16) & 0xffff;
			fbBlockAddr = *reqDataPtr++ & 0xffff;
			outputNum   = *reqDataPtr++;

  			
			*respDataPtr   = fbCodeId << 16;
			*respDataPtr++ |= fbBlockAddr & 0xffff ;
			respDataLen += DOUBLE_SIZE;	    

			*respDataPtr++  = outputNum;
			respDataLen += DOUBLE_SIZE;	    

			retVal = fbCodeParaTypePointerGet ( fbCodeId,  
						(strFbCodeTypePointerInfo*)&fbParaTypeInfo 	);
		
			
			for(j=0; j < outputNum ; j++ )
			{
				outputIndex  =  (*reqDataPtr >> 16) & 0xffff;
				outputType   = *reqDataPtr++ & 0xffff;

				
				if( (outputIndex < 1)||(outputIndex > fbMemInfo.outputNo) ) 
				{
               		retVal = FB_CODE_OUTPUT_INDEX_ERR;
               		status = FB_CODE_OUTPUT_INDEX_ERR;
				}
				
				if(GET_BASIC_SIZE(outputType) == SIZE64_TYPE ) 
				{
					eightByteConversion((uint32*)&outputData[0],(uint32*)reqDataPtr++,0);
					reqDataPtr++;
				}
				else 
				{
					outputData[0] = *reqDataPtr++;
					outputData[1] = 0;
				}
				
				
				if( (segData.segInfo[i].status == NO_ERROR)&&(status== NO_ERROR) )
				{					
					retVal = fbCodeOutputDataWriteWithType(	
							            					moduleInfo.moduleId, 	taskId,
															fbCodeId, 	fbBlockAddr ,
															outputIndex,  outputType,
															(uint32*) outputData );

					if(retVal)
					{
						setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__, retVal,
											"Task Id", 			taskId,
											"Recv Block Addr",	fbBlockAddr,
											"Recv Fb Code", 	fbCodeId
                               				);
					}
				}
				else 
					retVal = segData.segInfo[i].status; 

				*respDataPtr   	= outputIndex << 16;
				*respDataPtr++ 	|= ( outputType & 0xffff);
				respDataLen 	+= DOUBLE_SIZE;	    

				*respDataPtr++ 	=  retVal;
				respDataLen 	+= DOUBLE_SIZE;	    
			}  
        } 
    } 

MODULE_END:
	
	respMsgPtr->dataLen = respDataLen;
	
	return(status);
}


uint32 fbCodeParameterReadReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32	respDataLen = 0;
	uint32	*reqDataPtr	;
	uint32	*respDataPtr ;

	uint32  i, dataNum, j, parameterSize;
	uint32	taskId, codeNumber, fbCodeId, fbBlockAddr;
#define	MAX_FB_IN_READ_NUM	(128)
	uint32  parameterIndex, parameterNum, parameterData[MAX_FB_IN_READ_NUM], paraRefData[MAX_FB_IN_READ_NUM];  
	uint32	accumCodeNum;
	
	strTotalSegDataInfo	segData;
	strFbMemInfo	fbMemInfo;
	strModuleInfo		moduleInfo;
	strFbTaskTblMemInfo	segInfo;

	strFbDataGrHeaderInfo *reqHeaderptr;
	strFbCodeTypePointerInfo	fbParaTypeInfo;
	uint32	respType, k;
	uint16 fbRetVal = 0;
	uint32	*paraTypePtr = NULL;

	uint32 *fbParaPtr;
	strMemPointerInfo fbMem;
	uint32	taskNumber, logicCompareFlag;
	uint32	*fbCodeAndBlkAddrPtr;

	strFbSpecActionInfo specActionParaTypeDefInfo;
	
	memoryClear( (uint8*)&parameterData[0], sizeof(uint32)*MAX_FB_IN_READ_NUM);
	memoryClear( (uint8*)&paraRefData[0], sizeof(uint32)*MAX_FB_IN_READ_NUM);
	memoryClear( (uint8*)&specActionParaTypeDefInfo, sizeof(strFbSpecActionInfo));

	
	reqHeaderptr = (strFbDataGrHeaderInfo *)reqMsgPtr->dataBuff;
    memoryCopy((uint8*)respMsgPtr-> dataBuf,(uint8*)reqHeaderptr,
										sizeof(strFbDataGrHeaderInfo) );
	respDataLen = sizeof(strFbDataGrHeaderInfo);         
	
	
	retVal = moduleInfoRead(reqHeaderptr->memoryKind, &moduleInfo);

    if(retVal) 
    {
    	status = L_MODULE_ID_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, retVal);
		goto MODULE_END;
    }

    
    taskNumber = (reqHeaderptr->segmentNum&0x7FFFFFFF);

    
    logicCompareFlag = (reqHeaderptr->segmentNum>>31)&0x1;

	if( (taskNumber == 0)|| (taskNumber > moduleInfo.maxSegmentNum))
	{
		status = L_ITEM_NO_ERR;
        retVal = L_ITEM_NO_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal, 
	                        "Recv Item Num", taskNumber,
	                        "Max Item Num", moduleInfo.maxSegmentNum,
	                        "Not Used", 0
				  		  );
		goto MODULE_END;
	}
	
	
	reqDataPtr	= (uint32 *)&reqMsgPtr->dataBuff[12];
	
	accumCodeNum = 0;
    for(i = 0; i < taskNumber; i++) 
	{
		segData.segInfo[i].startDataPtr = reqDataPtr;

		taskId = (*reqDataPtr >> 16)&0xffff;	

        segData.segInfo[i].taskId 	= taskId;
		segData.segInfo[i].status 	= NO_ERROR;

        
        if(	(taskId < 1) || (taskId > moduleInfo.maxSegmentNum)) 
        {
			segData.segInfo[i].status = L_SEG_ID_ERR & 0xff;
			retVal = L_SEG_ID_ERR;
			status = L_SEG_ID_ERR;			
			setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
                    			"Recv Task Id", taskId,
                    			"Max Task Id", moduleInfo.maxSegmentNum,
                    			"Not Used", 0
			  		  			);
			
            codeNumber = *reqDataPtr++ & 0xffff;

			
			accumCodeNum += codeNumber;
			if(accumCodeNum > MAX_FB_IN_READ_NUM)
			{
				status = L_ITEM_NO_ERR;
		        retVal = L_ITEM_NO_ERR;
				setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
			                        "Total Code Output Num", accumCodeNum,
			                        "MAX_FB_IN_READ_NUM", MAX_FB_IN_READ_NUM,
			                        "Not Used", 0
						  		  );
				goto MODULE_END;
			}
			
			for(dataNum = 0; dataNum < codeNumber; dataNum++) 
			{
                fbCodeId  = (*reqDataPtr >> 16)&0xffff;
				fbBlockAddr = *reqDataPtr++& 0xffff;

				parameterIndex = (*reqDataPtr >> 16)&0xffff;
				parameterNum = *reqDataPtr++& 0xffff;

            }
			
            continue;
        }
		
        
        memoryClear ( (uint8 *) &segInfo, sizeof(strFbTaskTblMemInfo));
		retVal = getFbTaskTblInfo ( 	moduleInfo.moduleId,
           							taskId,
                                    (strFbTaskTblMemInfo *)&segInfo
								);
		if(retVal)
			setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
		
        codeNumber = *reqDataPtr++ & 0xffff;

		
		accumCodeNum += codeNumber;
		if(accumCodeNum > MAX_FB_IN_READ_NUM)
		{
			status = L_ITEM_NO_ERR;
	        retVal = L_ITEM_NO_ERR;
			setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
		                        "Total Code Output Num", accumCodeNum,
		                        "MAX_FB_IN_READ_NUM", MAX_FB_IN_READ_NUM,
		                        "Not Used", 0
					  		  );
			goto MODULE_END;
		}

	    for(dataNum = 0; dataNum < codeNumber; dataNum++) 
	    {
	    	
	    	fbCodeAndBlkAddrPtr = reqDataPtr;
	    	
            fbCodeId	= (*reqDataPtr >> 16) & 0xffff;
			fbBlockAddr = *reqDataPtr++ & 0xffff;
			
			parameterIndex 	= (*reqDataPtr >> 16)&0xffff;
			parameterNum 	= *reqDataPtr++& 0xffff;
				
			
			if(logicCompareFlag == 1)  		 		
				*fbCodeAndBlkAddrPtr = (0 << 16) | fbBlockAddr;
				
			if( (fbBlockAddr < 0) || (fbBlockAddr > moduleInfo.maxFbBlockAddr) ) 
			{
                retVal = L_FB_CODE_ERR;
                status = L_FB_CODE_ERR;
   				segData.segInfo[i].status = L_FB_ADDR_ERR & 0xff;
				setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
	                                "Recv Block Addr", fbBlockAddr,
	                                "Start Block Addr",0,
	                                "End Block Addr", moduleInfo.maxFbBlockAddr
									);
				
				if(logicCompareFlag == 1)  		 		
					*fbCodeAndBlkAddrPtr = (0<<16) | fbBlockAddr;
					
				goto CHECK_END;
            }
			
            
            memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
			retVal = fbMemInfoRead (	moduleInfo.moduleId,
            							fbBlockAddr,
                                        (strFbMemInfo *) &fbMemInfo
									);
			if(retVal) 
			{
            	segData.segInfo[i].status = L_FB_CODE_ERR & 0xff;
                status = L_FB_CODE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, retVal);
				
				goto CHECK_END;
			}
			
			
			if(logicCompareFlag == 1)  		 		
				*fbCodeAndBlkAddrPtr = (fbMemInfo.fbId<<16) | fbBlockAddr;
			
			
            if(taskId != fbMemInfo.fbTaskId) 
            {
               	retVal = L_SEG_ID_ERR;
                status = L_SEG_ID_ERR;
	           	segData.segInfo[i].status = L_SEG_ID_ERR & 0xff;
				setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
                                    "Recv Task Id", taskId,
                                    "Read Task Id",fbMemInfo.fbTaskId,
									"Recv Block Addr", fbBlockAddr 
               	    		 		);
				goto CHECK_END;
            }

			
            if(fbCodeId != fbMemInfo.fbId) 
            {
               	retVal = L_FB_CODE_ERR;
                status = L_FB_CODE_ERR;
	           	segData.segInfo[i].status = L_FB_CODE_ERR & 0xff;
				setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
	                                "Recv Fb Code", fbCodeId,
	                                "Read Fb Code",fbMemInfo.fbId,
									"Recv Block Addr", fbBlockAddr 
	       	    		 		);
				goto CHECK_END;
            }


			
			if( ( (parameterIndex + parameterNum -1) > fbMemInfo.inputNo) ||
				( parameterIndex < 1 ) || ( parameterIndex > fbMemInfo.inputNo ) ||
				( parameterNum > fbMemInfo.inputNo))
			{
               	retVal = L_PARA_NO_ERR;
                status = L_PARA_NO_ERR;
	           	segData.segInfo[i].status = L_PARA_NO_ERR & 0xff;
				setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
                                    "Recv Spec Num", parameterNum,
                                    "Read Spec Num",fbMemInfo.inputNo,
									"Recv Fb Code", fbCodeId 
               	    		 		);
            }	
CHECK_END:
		continue;
		}

	}
	
	reqDataPtr	= (uint32 *)&reqMsgPtr->dataBuff[12];
   	respDataPtr = (uint32 *)&respMsgPtr->dataBuf[12];
	
    for(i = 0; i < taskNumber; i++) 
	{
		taskId 		= (*reqDataPtr >> 16)&0xffff;	
        codeNumber 	= *reqDataPtr++ & 0xffff;
		
        *respDataPtr 	= taskId << 16;		
		*respDataPtr++ 	|= codeNumber & 0xffff;
        respDataLen 	+= DOUBLE_SIZE;	    

        for(dataNum = 0; dataNum < codeNumber; dataNum++) 
		{
            fbCodeId	= (*reqDataPtr >> 16) & 0xffff;
			fbBlockAddr = *reqDataPtr++ & 0xffff;

			parameterIndex = (*reqDataPtr >> 16)&0xffff;
			parameterNum = *reqDataPtr++& 0xffff;

            
            memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
		
            
            fbMemInfo.fbRunStatus = 1;
			retVal = fbMemInfoRead (	moduleInfo.moduleId,fbBlockAddr,
                                        (strFbMemInfo *) &fbMemInfo
									);
			if(retVal)
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, retVal);


			retVal = fbCodeParaTypePointerGet ( fbCodeId,  (strFbCodeTypePointerInfo	*)&fbParaTypeInfo 	);
			if(retVal != NO_ERROR) 
			{
				paraTypePtr = NULL;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, retVal);
			}
			else
			{
				paraTypePtr = (uint32 *)fbParaTypeInfo.specTypeStartAddr;
			}
			
			if(fbMemInfo.fbRunStatus != NO_ERROR) 
				fbRetVal = 1;
				
			else 
				fbRetVal = 0;

			*respDataPtr   	= fbCodeId << 16;
			*respDataPtr++ 	|= ( fbBlockAddr | (fbRetVal << 15) );
   			respDataLen 	+= DOUBLE_SIZE;	    
			
       		parameterSize  = 0 ; 

			if(retVal == NO_ERROR) 
			{
				
				if( 
					( (parameterIndex + parameterNum -1) > fbMemInfo.inputNo) ||
					( parameterIndex < 1 ) || ( parameterIndex > fbMemInfo.inputNo ) ||
					( parameterNum > fbMemInfo.inputNo)
				)
					retVal = FB_OUTPUT_NUM_ERR;

				
				if(retVal == NO_ERROR)
				{
					retVal = fbCodeSpecDataReadWithNumber(	
            					moduleInfo.moduleId,
								fbBlockAddr ,fbCodeId,
								parameterIndex,parameterNum,
								&parameterSize,
								(uint32*) paraRefData,			
								(uint32*) parameterData			
							);
				}

				if(retVal)
				{
					setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
										"Task Id", 			taskId,
										"Recv Block Addr", 	fbBlockAddr,
										"Recv Fb Code", 	fbCodeId
	                               		);
					parameterSize = 0; 
				}
			}

			*respDataPtr++ 	= retVal;
			respDataLen 	+= DOUBLE_SIZE;	    

			*respDataPtr 	= parameterNum << 16;		
			*respDataPtr++ 	|= parameterSize & 0xffff;
			respDataLen 	+= DOUBLE_SIZE;	    

			
			if(retVal != NO_ERROR) continue; 	

			
			k = 0;
			for(j=0; j < parameterNum ; j++ )
			{
				
				*respDataPtr = (parameterIndex + j)<<16; 

				
				
				if(paraTypePtr != NULL) 
				{
					if( CHK_IS_ANY_TYPE(*(paraTypePtr + parameterIndex + j - 1)) ) 
					{
						respType = fbMemInfo.paraRunType;
					}
					else 
					{
						respType = (GET_BASIC_DATA_TYPE(*(paraTypePtr + parameterIndex + j - 1)))  ;
					}
				}
				else 
					respType = 0;

			   	*respDataPtr++ |=  respType & 0xf ; 
				respDataLen += DOUBLE_SIZE;	    
#if NOT_USED
				
				if(paraTypePtr != NULL )
				{
					
					
					if (GET_TYPE_SIZE(*(paraTypePtr + parameterIndex + j - 1))  == SIZE64_TYPE ) 
					{
						eightByteConversion((uint32*)respDataPtr,(uint32*)&parameterData[k++],0);
						k++;
						respDataPtr++;
						respDataLen += DOUBLE_SIZE;	    
					}
					else
					{
						memoryCopy ( (uint8 *) respDataPtr, (uint8 *) &parameterData[k++],DOUBLE_SIZE );	
					}
					
					respDataPtr++;
					respDataLen += DOUBLE_SIZE;	    
				}

#endif 
				if(moduleInfo.moduleId == LOGIC_ID) 
				{
						
					memoryClear( 	(uint8 *)&fbMem,
        							sizeof(strMemPointerInfo)
								);

					if( (retVal = fbMemPointerGet	( moduleInfo.moduleId, &fbMem)) != NO_ERROR )
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, retVal);
						goto MODULE_END;
					}

					fbParaPtr = (uint32 *)fbMem.paraMemPtr + fbMemInfo.specActionIndex;
					specActionParaTypeDefInfo = *(strFbSpecActionInfo *)(fbParaPtr + parameterIndex + j - 1);
				}
	
				memoryCopy ( (uint8 *) respDataPtr, (uint8 *) &parameterData[k++],DOUBLE_SIZE );	
				respDataPtr++;
				respDataLen += DOUBLE_SIZE;	    

				
				if(paraTypePtr != NULL )
				{
					
					if ( (GET_TYPE_SIZE(*(paraTypePtr + parameterIndex + j - 1))  == SIZE64_TYPE ) &&
						 (GET_BASIC_SIZE(specActionParaTypeDefInfo.dataType) == SIZE64_TYPE) )
					{
						memoryCopy ( (uint8 *) respDataPtr,
            						 (uint8 *) &parameterData[k++],
							 		DOUBLE_SIZE
								   );	
#ifdef VXWORKS
						mySwap((uint32*)respDataPtr,(uint32*)(respDataPtr-1),DOUBLE_SIZE);
#endif
						respDataPtr++;
						respDataLen += DOUBLE_SIZE;	    
					}
					else if( (GET_TYPE_SIZE(*(paraTypePtr + parameterIndex + j - 1))==SIZE64_TYPE) && 
							 (CHK_IS_ANY_TYPE(*(paraTypePtr + parameterIndex + j - 1)))			&&
					 		 
							 (GET_BASIC_SIZE(specActionParaTypeDefInfo.dataType) == SIZE32_TYPE))
					{
						
						
						if(specActionParaTypeDefInfo.refType == SPEC_TYPE_BYREF)
						{
							memoryCopy ( (uint8 *) respDataPtr,
            							 (uint8 *) &parameterData[k++],
							 			DOUBLE_SIZE
									   );	
							respDataPtr++;
						}
						else
						{
							*respDataPtr++ = 0;
							k++;
						}
						
						respDataLen += DOUBLE_SIZE;	    
					}
				}
			}
        } 
    } 


MODULE_END:
	
	respMsgPtr->dataLen = respDataLen;
	
	return(status);
}


uint32 fbCodeParameterWriteReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32	respDataLen = 0;
	uint32	*reqDataPtr	;
	uint32	*respDataPtr ;

	uint32  i, dataNum, j;
	uint32	taskId, codeNumber, fbCodeId, fbBlockAddr;
#define MAX_FB_IN_WRITE_CODE_NUM	(150)
	uint32  specNum, specIndex, specData[2];  
	uint32	accumCodeNum;
	
	strTotalSegDataInfo	segData;
	strFbMemInfo	fbMemInfo;
	strModuleInfo		moduleInfo;
	strFbTaskTblMemInfo	segInfo;

	strFbDataGrHeaderInfo *reqHeaderptr;
	strFbCodeTypePointerInfo	fbParaTypeInfo;
	uint32	specType, *specTypePtr=NULL ;

    memoryClear((uint8*)&specData[0], DOUBLE_SIZE);

	
	reqHeaderptr = (strFbDataGrHeaderInfo *)reqMsgPtr->dataBuff;
    memoryCopy((uint8*)respMsgPtr-> dataBuf,(uint8*)reqHeaderptr,
										sizeof(strFbDataGrHeaderInfo) );
	respDataLen = sizeof(strFbDataGrHeaderInfo);         
	
	reqDataPtr	= (uint32 *)&reqMsgPtr->dataBuff[12];
   	respDataPtr = (uint32 *)&respMsgPtr->dataBuf[12];

	
	retVal = moduleInfoRead(reqHeaderptr->memoryKind, &moduleInfo);

    if(retVal) 
    {
    	status = L_MODULE_ID_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, retVal);
		
		goto MODULE_END;
    }

    
    if( (reqHeaderptr->segmentNum == 0)|| (reqHeaderptr->segmentNum > MAX_LOGIC_TASK_NUM))
    {
		status = L_ITEM_NO_ERR;
        retVal = L_ITEM_NO_ERR;
		setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
	                        "Recv Item Num", reqHeaderptr->segmentNum,
	                        "Max Item Num", moduleInfo.maxSegmentNum,
	                        "Not Used", 0
				  		  );
		goto MODULE_END;
	}
	
	accumCodeNum = 0;
	
    for(i = 0; i < reqHeaderptr->segmentNum; i++) 
	{
		segData.segInfo[i].startDataPtr = reqDataPtr;

		taskId = (*reqDataPtr >> 16)&0xffff;	

        segData.segInfo[i].taskId 	= taskId;
		segData.segInfo[i].status 	= NO_ERROR;

        
        if(	(taskId < 1) || (taskId > moduleInfo.maxSegmentNum)) 
        {
			segData.segInfo[i].status = L_SEG_ID_ERR & 0xff;
			retVal = L_SEG_ID_ERR;
			status = L_SEG_ID_ERR;			
			setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
                    			"Recv Task Id", taskId,
                    			"Max Task Id", moduleInfo.maxSegmentNum,
                    			"Not Used", 0
			  		  			);
			
            codeNumber = *reqDataPtr++ & 0xffff;

			
			accumCodeNum += codeNumber;
			if(accumCodeNum > MAX_FB_OUT_WRITE_CODE_NUM)
			{
				status = L_ITEM_NO_ERR;
		        retVal = L_ITEM_NO_ERR;
				setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
			                        "Total Code Output Num", accumCodeNum,
			                        "MAX_FB_IN_WRITE_CODE_NUM", MAX_FB_IN_WRITE_CODE_NUM,
			                        "Not Used", 0
						  		  );
				goto MODULE_END;
			}

			for(dataNum = 0; dataNum < codeNumber; dataNum++) 
			{
                fbCodeId  = (*reqDataPtr >> 16)&0xffff;
				fbBlockAddr = *reqDataPtr++& 0xffff;
				specNum = *reqDataPtr++;

            }
			
            continue;
        }
		
        
        memoryClear ( (uint8 *) &segInfo, sizeof(strFbTaskTblMemInfo));
		retVal = getFbTaskTblInfo ( 	moduleInfo.moduleId,taskId,
                                    (strFbTaskTblMemInfo *)&segInfo
								);
		if(retVal)
			setErrorCode(	__FILE__,__LINE__, __FUNCTION__, retVal);
	
		
        codeNumber = *reqDataPtr++ & 0xffff;

		
		accumCodeNum += codeNumber;
		if(accumCodeNum > MAX_FB_OUT_WRITE_CODE_NUM)
		{
			status = L_ITEM_NO_ERR;
	        retVal = L_ITEM_NO_ERR;
			setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
		                        "Total Code Output Num", accumCodeNum,
		                        "MAX_FB_IN_WRITE_CODE_NUM", MAX_FB_IN_WRITE_CODE_NUM,
		                        "Not Used", 0
					  		  );
			goto MODULE_END;
		}

	    for(dataNum = 0; dataNum < codeNumber; dataNum++) 
	    {
            fbCodeId	= (*reqDataPtr >> 16) & 0xffff;
			fbBlockAddr = *reqDataPtr++ & 0xffff;
			specNum   = *reqDataPtr++ ;
				
			if( (fbBlockAddr < 0) || (fbBlockAddr > MAX_MPU_BLOCK_ADDR) ) 
			{
                retVal = L_FB_CODE_ERR;
                status = L_FB_CODE_ERR;
   				segData.segInfo[i].status = L_FB_ADDR_ERR & 0xff;
				setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
	                                "Recv Block Addr", fbBlockAddr,
	                                "Start Block Addr",0,
	                                "End Block Addr", MAX_MPU_BLOCK_ADDR
           	    		 			);
				goto CHECK_END;
            }
			
            
            memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
			retVal = fbMemInfoRead (	moduleInfo.moduleId,
            							fbBlockAddr,
                                        (strFbMemInfo *) &fbMemInfo
									);
			if(retVal) 
			{
            	segData.segInfo[i].status = L_FB_CODE_ERR & 0xff;
                status = L_FB_CODE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, retVal);
				
				goto CHECK_END;
			}
			
			
            if(taskId != fbMemInfo.fbTaskId) 
            {
               	retVal = L_SEG_ID_ERR;
                status = L_SEG_ID_ERR;
	           	segData.segInfo[i].status = L_SEG_ID_ERR & 0xff;
				setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
                                    "Recv Task Id", taskId,
                                    "Read Task Id",fbMemInfo.fbTaskId,
									"Recv Block Addr", fbBlockAddr 
               	    		 		);
				goto CHECK_END;
            }

			
            if(fbCodeId != fbMemInfo.fbId) 
            {
               	retVal = L_FB_CODE_ERR;
                status = L_FB_CODE_ERR;
	           	segData.segInfo[i].status = L_FB_CODE_ERR & 0xff;
				setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
                                    "Recv Fb Code", fbCodeId,
                                    "Read Fb Code",fbMemInfo.fbId,
									"Recv Block Addr", fbBlockAddr 
               	    		 		);
				goto CHECK_END;
            }


			
			if(specNum > fbMemInfo.inputNo) 
			{
               	retVal = FB_SPEC_NUM_ERR;
                status = FB_SPEC_NUM_ERR;
	           	segData.segInfo[i].status = FB_SPEC_NUM_ERR & 0xff;
				setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
	                                "Recv Spec Num", specNum,
	                                "Read Spec Num",fbMemInfo.inputNo,
									"Recv Fb Code", fbCodeId 
	           	    		 		);
				goto CHECK_END;
            }
			retVal = fbCodeParaTypePointerGet 
								(
							fbCodeId,           								
						(strFbCodeTypePointerInfo	*)&fbParaTypeInfo 
								);
			if(retVal != NO_ERROR) 
			{
				specTypePtr = NULL;
		        segData.segInfo[i].status = (FB_CODE_SPEC_TYPE_ERR)& 0xff;
				setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
            	            		"fb Code", fbCodeId,
                	            	"Not Using", 0,
                    				"Not Using", 0
									);
			}
			else
			{
				specTypePtr = (uint32 *)fbParaTypeInfo.specTypeStartAddr;
			}

			
			for(j=0; j < specNum ; j++ )
			{
				specIndex  =  (*reqDataPtr >> 16) & 0xffff;
				specType   = *reqDataPtr++ & 0xffff;

				
				if( (specIndex > fbMemInfo.inputNo)||(specIndex <1) ) 
				{
               		retVal = FB_PARA_INDEX_ERR;
					status = FB_PARA_INDEX_ERR;
	           		segData.segInfo[i].status = (FB_PARA_INDEX_ERR) & 0xff;
					setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
										"Recv Input Index", specIndex,
										"MAX Input Index",fbMemInfo.inputNo,
										"Recv Fb Code", fbCodeId 
               	    		 			);
					goto CHECK_END;
				}


				
				if(specTypePtr != NULL) 
				{
					
					if( !CHK_IS_ANY_TYPE(*(specTypePtr + specIndex - 1)) ) 	
					{
						 if( specType != GET_BASIC_DATA_TYPE(*(specTypePtr + specIndex - 1)) ) 
						 {
							segData.segInfo[i].status = FB_CODE_SPEC_TYPE_ERR & 0xff;
							setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,FB_CODE_SPEC_TYPE_ERR,
				                				"Input Index", specIndex,
				                    			"Recv Type", specType,
				                        		"Read Type", GET_BASIC_DATA_TYPE(*(specTypePtr + specIndex - 1))
												);
						 }
					}
					
					if( (reqHeaderptr->memoryKind != LOGIC_ID ) && 
						(GET_NON_TUNNABLE_TYPE(*(specTypePtr + specIndex - 1))))
					{
						segData.segInfo[i].status = L_PARA_TUNE_ERR & 0xff;
						setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,L_PARA_TUNE_ERR,
				                			"Input Index", specIndex,
				                    		"Recv Type", specType,
				                        	"Read Type", (*(specTypePtr + specIndex - 1))
											);
					
					}
				}	
				
				if(GET_BASIC_SIZE(specType) == SIZE64_TYPE ) 
				{
					eightByteConversion((uint32*)&specData[0],(uint32*)reqDataPtr++,0);
					reqDataPtr++;
				}
				else 
				{
					specData[0] = *reqDataPtr++;
					specData[1] = 0;
				}
			}  
			
CHECK_END:
		continue;
		}

	}
	
	reqDataPtr	= (uint32 *)&reqMsgPtr->dataBuff[12];
   	respDataPtr = (uint32 *)&respMsgPtr->dataBuf[12];
	
    for(i = 0; i < reqHeaderptr->segmentNum; i++) 
	{
		taskId = (*reqDataPtr >> 16)&0xffff;	
        codeNumber = *reqDataPtr++ & 0xffff;
	
        *respDataPtr = taskId << 16;		
		*respDataPtr++ |= (codeNumber & 0xffff);
        respDataLen += DOUBLE_SIZE;	    

        for(dataNum = 0; dataNum < codeNumber; dataNum++) 
		{
            fbCodeId	= (*reqDataPtr >> 16) & 0xffff;
			fbBlockAddr = *reqDataPtr++ & 0xffff;
			specNum   = *reqDataPtr++;

			
			*respDataPtr   = fbCodeId << 16;
			*respDataPtr++ |=  (fbBlockAddr & 0xffff) ;
   			respDataLen += DOUBLE_SIZE;	    

			*respDataPtr++  = specNum;
			respDataLen += DOUBLE_SIZE;	    

			
			for(j=0; j < specNum ; j++ )
			{
				specIndex  =  (*reqDataPtr >> 16) & 0xffff;
				specType   = *reqDataPtr++ & 0xffff;

				
				if( (specIndex < 1)||(specIndex > fbMemInfo.inputNo) ) 
				{
               		retVal = FB_PARA_INDEX_ERR;
					status = FB_PARA_INDEX_ERR;
				}
				
				if(GET_BASIC_SIZE(specType) == SIZE64_TYPE ) 
				{
					eightByteConversion((uint32*)&specData[0],(uint32*)reqDataPtr++,0);
					reqDataPtr++;
				}
				else 
				{
					specData[0] = *reqDataPtr++;
					specData[1] = 0;
				}	

				
				if( (segData.segInfo[i].status == NO_ERROR)&&(status== NO_ERROR) )
				{
					
					retVal = fbCodeSpecDataWriteWithType(	
							            					moduleInfo.moduleId, 
															fbCodeId, 	fbBlockAddr ,
															specIndex,  specType,
															(uint32*) specData 
														);
					

					if(retVal)
					{
						setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__,retVal,
											"Task Id", 			taskId,
											"Recv Block Addr", 	fbBlockAddr,
											"Recv Fb Code", 	fbCodeId
                                   			);
					}
				}
				else 
					retVal = segData.segInfo[i].status; 

				*respDataPtr   	= specIndex << 16;
				*respDataPtr++ 	|= (specType  & 0xffff);
				respDataLen 	+= DOUBLE_SIZE;	    

				*respDataPtr++ 	=  retVal & 0xffff;
				respDataLen 	+= DOUBLE_SIZE;	    
			}  
        } 
    } 

MODULE_END:
	
	respMsgPtr->dataLen = respDataLen;
	
	return(status);
}




static uint32	localFbSegTblWriteReqAndResp
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR;
	uint32	retVal = 0;
	uint32	taskId = 0;
	uint32	i = 0;
	strModuleInfo			moduleInfo;		
	strSegTblWriteReqInfo	*reqDataPtr;	
    strSegTblWriteRespInfo	*respDataPtr;	
	
	strFbTaskTblMemInfo		segTbl;			
	strTotalSegDataInfo		segData;		
	uint32	respDataLen = 0;				
	uint32	usedMemSize = 0;				

	strFbDataGrHeaderInfo *reqHeaderptr;	
	strModuleExecInfo sysExeInfo;
	uint32 totalTaskNum = 0 , currTaskNum = 0;

	
	reqHeaderptr = (strFbDataGrHeaderInfo *)reqMsgPtr->dataBuff;
    memoryCopy((uint8*)respMsgPtr-> dataBuf,(uint8*)reqHeaderptr,
										sizeof(strFbDataGrHeaderInfo) );
										
	
	respDataLen = sizeof(strFbDataGrHeaderInfo);

    
    memoryClear( (uint8 *)&moduleInfo, sizeof(strModuleInfo));
	retVal = moduleInfoRead(reqHeaderptr->memoryKind, &moduleInfo);

    if(retVal)
    {
    	status = L_MODULE_ID_ERR;
        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
        
		goto MODULE_END;
    }

	totalTaskNum = (reqHeaderptr->segmentNum >> 16);
	currTaskNum = (reqHeaderptr->segmentNum & 0xffff);

	if(totalTaskNum == 0) totalTaskNum = currTaskNum;

    
    if( (currTaskNum < 1) || ( currTaskNum > totalTaskNum) ||
		(totalTaskNum > moduleInfo.maxSegmentNum))
	{
		status = L_ITEM_NO_ERR;
        retVal = L_ITEM_NO_ERR;
        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
		goto MODULE_END;
	}

	reqDataPtr = (strSegTblWriteReqInfo *)&reqMsgPtr->dataBuff[12];
	
	
	memoryClear( (uint8 *)&sysExeInfo, sizeof(strModuleExecInfo));
	status = fbSysExeModuleInfoRead(moduleInfo.moduleId, &sysExeInfo); 
	if(status != NO_ERROR) 
	{
        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
	}

	memoryClear( (uint8 *)&segData, sizeof(strTotalSegDataInfo));
   	segData.totalSegNum = currTaskNum;
	
	usedMemSize = 0;

	for(i = 0; i < currTaskNum; i++, reqDataPtr++) 
	{
		segData.segInfo[i].startDataPtr = (void *)reqDataPtr;

       	segData.segInfo[i].taskId = reqDataPtr->taskId;

       	
       	if( (reqDataPtr->taskId < 1) ||
           	(reqDataPtr->taskId > moduleInfo.maxSegmentNum))
		{
			retVal = L_SEG_ID_ERR;
           	status = L_SEG_ID_ERR;
           	segData.segInfo[i].status = L_SEG_ID_ERR & 0xff;	
	        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
                                "Recv Task Id", reqDataPtr->taskId,
                                "Max Task Id", moduleInfo.maxSegmentNum,
                                "Not Used" , 0
								);
			goto CHECK_END;
        }
		
		memoryClear( (uint8 *)&segTbl, sizeof(strFbTaskTblMemInfo));
		if ( (retVal = getFbTaskTblInfo	(
            				moduleInfo.moduleId,
							reqDataPtr->taskId,
                            &segTbl
                           				)
			 ) != NO_ERROR)
		{	
			if(retVal == L_MODULE_ID_ERR)
			{
               	segData.segInfo[i].status = L_MODULE_ID_ERR ;
               	
                status = L_MODULE_ID_ERR;
            }
			else 
			{
                segData.segInfo[i].status = L_SEG_ID_ERR;
                
                status = L_SEG_ID_ERR;
            }
            
	        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
	        
			goto CHECK_END;
        }

		
        if (segTbl.taskMode != PCS_INIT) 
		{
			retVal = L_PCS_MODE_ERR;
            status = L_PCS_MODE_ERR;
            segData.segInfo[i].status = L_PCS_MODE_ERR;
	        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
                                "Cur Seg Mode", segTbl.taskMode,
                                "Cur Task Id", 	segData.segInfo[i].taskId,
                                "Read Task Id" , segTbl.taskId
								);
			goto CHECK_END;
        }

		
		if( ((reqDataPtr->excutionMode != CYCLIC_RUNNING_MODE) &&
			(reqDataPtr->excutionMode != NON_CYCLIC_RUNNING_MODE) &&
			(reqDataPtr->excutionMode !=EVENT_RUNNING_MODE)) 
			||
			((reqHeaderptr->memoryKind != LOGIC_ID) &&			
			(reqDataPtr->excutionMode != CYCLIC_RUNNING_MODE))
		  )
		{
			retVal = SEG_EXECUTION_MODE_ERR;
			status = SEG_EXECUTION_MODE_ERR;
            segData.segInfo[i].status = SEG_EXECUTION_MODE_ERR;            
	        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
                                "Task Id", i+1,
                                "Execution Mode", reqDataPtr->excutionMode,
                                "Not Used" , 0
								);
			goto CHECK_END;
		}

		
		if( (reqDataPtr->excutionMode == CYCLIC_RUNNING_MODE) &&
			( (reqDataPtr->excutionID < MIN_CYCLIC_TIME_ID) ||
			(reqDataPtr->excutionID > MAX_CYCLIC_TIME_ID) ) )
		{
			retVal = SEG_EXECUTION_ID_ERR;
			status = SEG_EXECUTION_ID_ERR;
            segData.segInfo[i].status = SEG_EXECUTION_ID_ERR;            
	        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
                                "Task Id", i+1,
                                "Execution ID", reqDataPtr->excutionID,
                                "Not Used" , 0
								);
			goto CHECK_END;
		}

		
		if(reqHeaderptr->memoryKind == LOGIC_ID) 
		{
			if( ((reqDataPtr->excutionMode ==EVENT_RUNNING_MODE) && (reqDataPtr->taskPriority != 0)) 
				|| 
				((reqDataPtr->excutionMode ==CYCLIC_RUNNING_MODE) && (reqDataPtr->taskPriority < 1)) 
				|| 
				(reqDataPtr->taskPriority>MAX_SEG_PRIORITY) )
			{
				retVal = SEG_PRIORITY_ERR;
				status = SEG_PRIORITY_ERR;
				segData.segInfo[i].status = SEG_PRIORITY_ERR;            
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
									"Task Id", i+1,
									"Seg Priority", reqDataPtr->taskPriority,
									"Not Used" , 0
									);
				goto CHECK_END;
			}
		}
		
		
		if( (reqHeaderptr->memoryKind == LOGIC_ID) && (reqDataPtr->excutionMode != EVENT_RUNNING_MODE) &&
			( (reqDataPtr->eventTasksetAction != 0) || (reqDataPtr->eventTasksetAddress != 0) )
		  )
		{
			retVal = EVENT_TASK_SET_ERR;
			status = EVENT_TASK_SET_ERR;
            segData.segInfo[i].status = SEG_PRIORITY_ERR;            
	        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
                                "Task Id", i+1,
                                "EventTask SetAction" , reqDataPtr->eventTasksetAction,
								"EventTask SetAddress", reqDataPtr->eventTasksetAddress
								);
			goto CHECK_END;
		}

        
        
        usedMemSize += reqDataPtr->paraMemSize;
        if( (usedMemSize > moduleInfo.maxFbParaSize) ||
        	(reqDataPtr->paraMemSize == 0))
		{
			retVal = L_MEM_SIZE_ERR;
			status = L_MEM_SIZE_ERR;
            segData.segInfo[i].status = L_MEM_SIZE_ERR;            
	        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
                                "Task Id", i+1,
                                "Recv Mem Size", usedMemSize,
                                "Max Mem Size",  moduleInfo.maxFbParaSize
								);
			goto CHECK_END;
		}
		
CHECK_END:
	
		continue;
	}

	respDataPtr = (strSegTblWriteRespInfo *)&respMsgPtr->dataBuf[12];
	gSegDownloadData.totalSegNum = totalTaskNum; 

	for(i = 0; i < currTaskNum; i++, respDataPtr++) 
	{
		gSegDownloadData.taskId[i] = segData.segInfo[i].taskId;

		reqDataPtr = (strSegTblWriteReqInfo *)segData.segInfo[i].startDataPtr;

		respDataPtr->taskId 	= segData.segInfo[i].taskId;
		respDataPtr->status 	= segData.segInfo[i].status;

		
		if(segData.segInfo[i].status) 
			continue; 

		
		if( (retVal = getFbTaskTblInfo (
								moduleInfo.moduleId,
								segData.segInfo[i].taskId,
								&segTbl
                               	)
			) != NO_ERROR)
		{
	        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
		}
		else 
		{           	
            segTbl.logicCheckSum			= 0;
            segTbl.logicCheckSumStatus	= 0;
            segTbl.memInputSize			= 0;
			segTbl.memOutputSize		= 0;

			segTbl.loopStatus			= 0;
         	segTbl.taskTickCnt		= 0;
			segTbl.taskElpTime	= 0;
			segTbl.taskOvertimeRunCnt		= 0;

			segTbl.taskMode				= PCS_DOWNLOAD;

			segTbl.taskId				= reqDataPtr->taskId;
			segTbl.recvMemSize			= reqDataPtr->paraMemSize;
			segTbl.execMode				= reqDataPtr->excutionMode;
			segTbl.taskCycleTime			= reqDataPtr->excutionID;

			segTbl.taskPriority			= reqDataPtr->taskPriority;

			
			if (moduleInfo.moduleId == LOGIC_ID) 
			{
				segTbl.u1.evtActFbStartAddr    = reqDataPtr->eventTasksetAction;
				segTbl.u2.evtSetFbEndAddr      = reqDataPtr->eventTasksetAddress;
			}
			else 
			{ 
				segTbl.u1.fbStartBlockAddr    = reqDataPtr->eventTasksetAction;
				segTbl.u2.fbEndBlockAddr      = reqDataPtr->eventTasksetAddress;				
			}
			
			segTbl.fbUsedNum			= 0; 
			segTbl.logicType			= 0;
			
			if( (reqHeaderptr->memoryKind==LOGIC_ID)&&(reqDataPtr->excutionMode ==CYCLIC_RUNNING_MODE))
				segTbl.sfcStepKind = reqDataPtr->eventTasksetAction;
			else 
				segTbl.sfcStepKind = 0;
				
			if( (retVal = segTblInfoWrite(
								moduleInfo.moduleId,
								segData.segInfo[i].taskId,
								&segTbl
										)
				) != NO_ERROR)
			{
		        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
			}

		}		
	} 
	
	respDataLen += currTaskNum * sizeof(strSegTblWriteRespInfo);
    
	sysExeInfo.curMaxSegmentId += currTaskNum;

	status = fbSysExeModuleInfoWrite(moduleInfo.moduleId, &sysExeInfo); 

	if(status != NO_ERROR) 
	{
        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
	}
	
MODULE_END:

	if(sysExeInfo.curMaxSegmentId == gSegDownloadData.totalSegNum ) 
	{
		for(i = 0; i < moduleInfo.maxSegmentNum; i++) 
		{
			memoryClear( (uint8 *) &segTbl, sizeof(strFbTaskTblMemInfo));

       		taskId = i+1;

			if( (retVal = getFbTaskTblInfo (
									moduleInfo.moduleId,
									taskId,
									&segTbl
                               		)
				) != NO_ERROR)
			{
		        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
		        
				continue;		
			}
			
			
			if(	(segTbl.taskMode != PCS_NO_MODE) && (segTbl.recvMemSize == 0) ) 
			{
				segTblInfoClear(moduleInfo.moduleId,taskId);
			}
		}
	}
	
 	respMsgPtr->dataLen = respDataLen;

    return(status);
}


static uint32	busFbSegTblWriteReqAndResp
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR;
	uint32	retVal = 0;
	uint32	taskId = 0;
	uint32	i = 0;
	strModuleInfo			moduleInfo;		
	strSegTblWriteReqInfo	*reqDataPtr;	
    strSegTblWriteRespInfo	*respDataPtr;	
	
	strFbTaskTblMemInfo		segTbl;			
	strTotalSegDataInfo		segData;		
	uint32	respDataLen = 0;				
	uint32	usedMemSize = 0;				

	strFbDataGrHeaderInfo *reqHeaderptr;	
	uint32 totalTaskNum = 0 , currTaskNum = 0;
	strModuleExecInfo sysExeInfo;

	
	reqHeaderptr = (strFbDataGrHeaderInfo *)reqMsgPtr->dataBuff;
    memoryCopy((uint8*)respMsgPtr-> dataBuf,(uint8*)reqHeaderptr,
										sizeof(strFbDataGrHeaderInfo) );
	
	respDataLen = sizeof(strFbDataGrHeaderInfo);         

    
    memoryClear( (uint8 *)&moduleInfo, sizeof(strModuleInfo));
	retVal = moduleInfoRead(reqHeaderptr->memoryKind, &moduleInfo);

    if(retVal) 
    {
    	status = L_MODULE_ID_ERR;
        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
		goto MODULE_END;
    }

	totalTaskNum = (reqHeaderptr->segmentNum >> 16);
	currTaskNum = (reqHeaderptr->segmentNum & 0xffff);

	if(totalTaskNum == 0) totalTaskNum = currTaskNum;

    
    if( (currTaskNum < 1) || ( currTaskNum > totalTaskNum) ||
		(totalTaskNum > moduleInfo.maxSegmentNum))
	{
		status = L_ITEM_NO_ERR;
        retVal = L_ITEM_NO_ERR;
        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
        
		goto MODULE_END;
	}
	
	reqDataPtr = (strSegTblWriteReqInfo *)&reqMsgPtr->dataBuff[12];

	memoryClear( (uint8 *)&segData, sizeof(strTotalSegDataInfo));
   	segData.totalSegNum = currTaskNum;
		
	usedMemSize = 0;

	for(i = 0; i < currTaskNum; i++, reqDataPtr++) 
	{
		segData.segInfo[i].startDataPtr = (void *)reqDataPtr;

       	segData.segInfo[i].taskId = reqDataPtr->taskId;

       	
       	if( (reqDataPtr->taskId < 1) ||
           	(reqDataPtr->taskId > moduleInfo.maxSegmentNum))
		{
           	retVal = L_SEG_ID_ERR;
           	status = L_SEG_ID_ERR;
           	segData.segInfo[i].status = L_SEG_ID_ERR & 0xff;
	        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
                                "Recv Task Id", reqDataPtr->taskId,
                                "Max Task Id", moduleInfo.maxSegmentNum,
                                "Not Used" , 0
								);
			goto CHECK_END;
        }
		
		memoryClear( (uint8 *)&segTbl, sizeof(strFbTaskTblMemInfo));
		if ( (retVal = busSegTblInfoRead	(
            				moduleInfo.moduleId,
							reqDataPtr->taskId,
                            &segTbl
                           				)
			 ) != NO_ERROR)
		{	
			if(retVal == L_MODULE_ID_ERR)
			{
               	segData.segInfo[i].status = L_MODULE_ID_ERR & 0xff;
                status = L_MODULE_ID_ERR;
            }
			else 
			{
                segData.segInfo[i].status = L_SEG_ID_ERR & 0xff;
                status = L_SEG_ID_ERR;
            }
            
	        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
	        
			goto CHECK_END;
        }

		
       if (segTbl.taskMode != PCS_INIT)  
		{
			retVal = L_PCS_MODE_ERR;
            status = L_PCS_MODE_ERR;
            segData.segInfo[i].status = L_PCS_MODE_ERR & 0xff;
	        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
                                "Cur Seg Mode", segTbl.taskMode,
                                "Cur Task Id", 	segData.segInfo[i].taskId,
                                "Read Task Id" , segTbl.taskId
								);
			goto CHECK_END;
        }

		
		if( ((reqDataPtr->excutionMode != CYCLIC_RUNNING_MODE) &&
			(reqDataPtr->excutionMode != NON_CYCLIC_RUNNING_MODE) &&
			(reqDataPtr->excutionMode !=EVENT_RUNNING_MODE) ) 
			||
			((reqHeaderptr->memoryKind != LOGIC_ID) &&			
			(reqDataPtr->excutionMode != CYCLIC_RUNNING_MODE))
		  )
		{
			retVal = SEG_EXECUTION_MODE_ERR;
			status = SEG_EXECUTION_MODE_ERR;
            segData.segInfo[i].status = SEG_EXECUTION_MODE_ERR & 0xff;            
	        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
                                "Task Id", i+1,
                                "Execution Mode", reqDataPtr->excutionMode,
                                "Not Used" , 0
								);
			goto CHECK_END;
		}

		
		if( (reqDataPtr->excutionMode == CYCLIC_RUNNING_MODE) &&
			( (reqDataPtr->excutionID < MIN_CYCLIC_TIME_ID) ||
			(reqDataPtr->excutionID > MAX_CYCLIC_TIME_ID) ) )
		{
			retVal = SEG_EXECUTION_ID_ERR;
			status = SEG_EXECUTION_ID_ERR;
            segData.segInfo[i].status = SEG_EXECUTION_ID_ERR & 0xff;            
	        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
                                "Task Id", i+1,
                                "Execution ID", reqDataPtr->excutionID,
                                "Not Used" , 0
								);
			goto CHECK_END;
		}

		
		if(reqHeaderptr->memoryKind == LOGIC_ID) 
		{
			if( ((reqDataPtr->excutionMode ==EVENT_RUNNING_MODE) && (reqDataPtr->taskPriority != 0)) 
				|| 
				((reqDataPtr->excutionMode ==CYCLIC_RUNNING_MODE) && (reqDataPtr->taskPriority < 1)) 
				|| 
				(reqDataPtr->taskPriority>MAX_SEG_PRIORITY) )
			{
				retVal = SEG_PRIORITY_ERR;
				status = SEG_PRIORITY_ERR;
				segData.segInfo[i].status = SEG_PRIORITY_ERR;            
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
									"Task Id", i+1,
									"Seg Priority", reqDataPtr->taskPriority,
									"Not Used" , 0
									);
				goto CHECK_END;
			}
		}

		
		if( (reqHeaderptr->memoryKind == LOGIC_ID) &&  (reqDataPtr->excutionMode != EVENT_RUNNING_MODE) &&
			( (reqDataPtr->eventTasksetAction != 0) || (reqDataPtr->eventTasksetAddress != 0) )
		  )
		{
			retVal = EVENT_TASK_SET_ERR;
			status = EVENT_TASK_SET_ERR;
            segData.segInfo[i].status = SEG_PRIORITY_ERR & 0xff;            
	        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
                                "Task Id", i+1,
                                "EventTask SetAction" , reqDataPtr->eventTasksetAction,
								"EventTask SetAddress", reqDataPtr->eventTasksetAddress
								);
			goto CHECK_END;
		}
	    
        
        usedMemSize += reqDataPtr->paraMemSize;
        if( (usedMemSize > moduleInfo.maxFbParaSize) ||
        	(reqDataPtr->paraMemSize == 0))
		{
			retVal = L_MEM_SIZE_ERR;
			status = L_MEM_SIZE_ERR;
            segData.segInfo[i].status = L_MEM_SIZE_ERR & 0xff;            
	        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
                                "Task Id", i+1,
                                "Total Mem Size", usedMemSize,
                                "Max Mem Size",  moduleInfo.maxFbParaSize
								);
		}
		
CHECK_END:
	
		continue;
	}

    
    memoryCopy((uint8*)respMsgPtr-> dataBuf,(uint8*)reqHeaderptr,
										sizeof(strFbDataGrHeaderInfo) );
	
	respDataPtr = (strSegTblWriteRespInfo *)&respMsgPtr->dataBuf[12];
	
	for(i = 0; i < currTaskNum; i++, respDataPtr++) 
	{
		reqDataPtr = (strSegTblWriteReqInfo *)segData.segInfo[i].startDataPtr;

		respDataPtr->taskId 	= segData.segInfo[i].taskId;
		respDataPtr->status = segData.segInfo[i].status;

		
		if(segData.segInfo[i].status) 
			continue; 

		
		if( (retVal = busSegTblInfoRead (
								moduleInfo.moduleId,
								segData.segInfo[i].taskId,
								&segTbl
                               	)
			) != NO_ERROR)
		{
	        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
		}
		else 
		{
           	
            segTbl.logicCheckSum		= 0;
            segTbl.logicCheckSumStatus	= 0;
            segTbl.memInputSize			= 0;
            segTbl.memOutputSize		= 0;

			segTbl.loopStatus			= 0;
         	segTbl.taskTickCnt			= 0;
			segTbl.taskElpTime			= 0;
			segTbl.taskOvertimeRunCnt	= 0;
			segTbl.taskMode				= PCS_DOWNLOAD;

			segTbl.taskId				= reqDataPtr->taskId;
			segTbl.recvMemSize			= reqDataPtr->paraMemSize;
			segTbl.execMode				= reqDataPtr->excutionMode;
			segTbl.taskCycleTime		= reqDataPtr->excutionID;

			segTbl.taskPriority			= reqDataPtr->taskPriority;
			
			
			
			if (moduleInfo.moduleId == LOGIC_ID) 
			{
				segTbl.u1.evtActFbStartAddr    = reqDataPtr->eventTasksetAction;
				segTbl.u2.evtSetFbEndAddr      = reqDataPtr->eventTasksetAddress;
			}
			else 
			{ 
				segTbl.u1.fbStartBlockAddr    = reqDataPtr->eventTasksetAction;
				segTbl.u2.fbEndBlockAddr      = reqDataPtr->eventTasksetAddress;				
			}

			segTbl.fbUsedNum			= 0; 
			segTbl.logicType			= 0;

			if( (reqHeaderptr->memoryKind==LOGIC_ID)&&(reqDataPtr->excutionMode ==CYCLIC_RUNNING_MODE))
				segTbl.sfcStepKind = reqDataPtr->eventTasksetAction;
			else segTbl.sfcStepKind = 0;

			if( (retVal = busSegTblInfoWrite(
								moduleInfo.moduleId,
								segData.segInfo[i].taskId,
								&segTbl
										)
				) != NO_ERROR)
			{
		        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
			}
		}		
	} 
	
	respDataLen +=currTaskNum * sizeof(strSegTblWriteRespInfo);
	
MODULE_END:

	memoryClear( (uint8 *)&sysExeInfo, sizeof(strModuleExecInfo));
	status = fbSysExeModuleInfoRead(moduleInfo.moduleId, &sysExeInfo); 
	if(status != NO_ERROR) 
	{
        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
	}

	if(sysExeInfo.curMaxSegmentId == gSegDownloadData.totalSegNum ) 
	{
		for(i = 0; i < moduleInfo.maxSegmentNum; i++) 
		{
			memoryClear( (uint8 *) &segTbl, sizeof(strFbTaskTblMemInfo));

       		taskId = i+1;

			if( (retVal = busSegTblInfoRead (
									moduleInfo.moduleId,
									taskId,
									&segTbl
                               		)
				) != NO_ERROR)
			{
		        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
				continue;		
			}
			
			
			if(	(segTbl.taskMode != PCS_NO_MODE) && (segTbl.recvMemSize == 0) ) 
			{
				busSegTblInfoClear(moduleInfo.moduleId,taskId);
			}
		}
	}

 	respMsgPtr->dataLen = respDataLen;

    return(status);
}



static uint32 localFbProgWriteReqAndResp 
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR;
	uint32	retVal = NO_ERROR;
	uint32	respDataLen = 0;
	uint32	*reqDataPtr	;					
	uint32	*respDataPtr ;					

	uint32  i, j;
	uint32	dataNum;
	uint32	taskId;
	uint32	codeNumber;
	uint32	fbId;
	uint32	fbBlockAddr;
	uint32  specNum;
	uint32	specSizeNum;
	uint32	specData;
	uint32	sIndex;  
	uint32  outputNegate[2];
	strFbSpecActionInfo *specActionInfoPtr;
	uint32	anyFuncOutDataSize;
	uint32	anyFuncOutDataType;


	strTotalSegDataInfo	segData;
	strModuleInfo		moduleInfo;
	strFbTaskTblMemInfo	segTbl, *segTblPtr;
	strProgDwlParaInfo	*paraIndexPtr;

	strFbDataGrHeaderInfo *reqHeaderptr;
    strFbExecDbInfo fbCodeInfo;
	strMemPointerInfo	fbMem;

	strFbMemInfo	*fbCodePtr, *firstFbCodePtr;
    uint32	*fbParaPtr,*firstFbParaPtr, *tmpPtr , *tmpPtr2;
	uint32	*fbOutputPtr, *firstFbOutputPtr;

	strModuleExecInfo sysExeInfo;
	strFbCodeTypePointerInfo typePInfo;
	uint32 *paraTypePtr;

	strFbSpecActionInfo specActionParaTypeDefInfo;
	uint32	fbParaTypeDef;
	uint32	exceptNumber;

	memoryClear( (uint8 *)&moduleInfo, sizeof(strModuleInfo));
	memoryClear( (uint8 *)&segData, sizeof(strTotalSegDataInfo));
	memoryClear( (uint8 *)&typePInfo, sizeof(strFbCodeTypePointerInfo));
	memoryClear( (uint8 *)&outputNegate[0], LONG_SIZE);

	
	reqHeaderptr = (strFbDataGrHeaderInfo *)reqMsgPtr->dataBuff;
    memoryCopy((uint8*)respMsgPtr->dataBuf,(uint8*)reqHeaderptr,
										sizeof(strFbDataGrHeaderInfo) );
	
	respDataLen = sizeof(strFbDataGrHeaderInfo);         
	
	reqDataPtr	= (uint32 *)&reqMsgPtr->dataBuff[12];
	
   	respDataPtr = (uint32 *)&respMsgPtr->dataBuf[12];
	
	memoryClear( (uint8 *)&sysExeInfo, sizeof(strModuleExecInfo));
	
	status = fbSysExeModuleInfoRead( reqHeaderptr->memoryKind, &sysExeInfo );
	if(status != NO_ERROR) 
	{
        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
	    goto MODULE_END;
	}

	
	memoryClear( (uint8 *)&moduleInfo, sizeof(strModuleInfo));
	retVal = moduleInfoRead(reqHeaderptr->memoryKind, &moduleInfo);

	if(retVal) 
	{
    	status = L_MODULE_ID_ERR;
        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
        
		goto MODULE_END;
    }

    
    if( (reqHeaderptr->segmentNum < 1) || (reqHeaderptr->segmentNum > moduleInfo.maxSegmentNum))
    {
		status = L_ITEM_NO_ERR;
        retVal = L_ITEM_NO_ERR;
        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
        
		goto MODULE_END;
	}
	
    if(moduleInfo.moduleId == LOGIC_ID)
    	paraIndexPtr = &gMpuProgDwlPara;
   	else
    	paraIndexPtr = &gDiuProgDwlPara;

   	segData.totalSegNum = reqHeaderptr->segmentNum;

	
    for(i = 0; i < reqHeaderptr->segmentNum; i++) 
	{
		segData.segInfo[i].startDataPtr = reqDataPtr;

		taskId = (*reqDataPtr >> 16)&0xffff;	
		codeNumber = *reqDataPtr++ & 0xffff;

		segData.segInfo[i].taskId 	= taskId;
		segData.segInfo[i].status 	= NO_ERROR;

		
        memoryClear ( (uint8 *) &segTbl, sizeof(strFbTaskTblMemInfo));
        
        if ( (retVal = getFbTaskTblInfo	(
            				moduleInfo.moduleId,
							taskId,
                    	    &segTbl
                            			)
			 ) != NO_ERROR)
		 {
			status = retVal;
           	segData.segInfo[i].status = retVal & 0xff;
	        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
	        
			goto CHECK_END;
        }

		
        if(	(taskId < 1) || (taskId > moduleInfo.maxSegmentNum))
        {
			segData.segInfo[i].status = L_SEG_ID_ERR & 0xff;
			retVal = L_SEG_ID_ERR;
			status = L_SEG_ID_ERR;			
	        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
                    			"Recv Task Id", taskId,
                    			"Max Task Id", moduleInfo.maxSegmentNum,
                    			"Not Used", 0
			  		  			);
			

			for(dataNum = 0; dataNum < codeNumber; dataNum++) 
			{
                fbId  = (*reqDataPtr >> 16)&0xffff;
				fbBlockAddr = *reqDataPtr++& 0xffff;

				anyFuncOutDataSize = (*reqDataPtr >> 16)&0xffff;
				anyFuncOutDataType = *reqDataPtr++& 0xffff;
				
				memoryCopy((uint8*)&outputNegate[0],(uint8*)reqDataPtr++,LONG_SIZE);
				reqDataPtr++;

				specNum = (*reqDataPtr >> 16)&0xffff;
				specSizeNum = *reqDataPtr++& 0xffff;

				reqDataPtr += specNum;
				reqDataPtr += specSizeNum;

            }
			
            goto CHECK_END;;
        }

		
        if(segTbl.taskMode != PCS_DOWNLOAD) 
        {
            retVal = L_PCS_MODE_ERR;
            status = L_PCS_MODE_ERR;
	        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
								"Task Id", taskId,
                                "Seg Mode", segTbl.taskMode,
                                "NOT USING", 0
			  		  			);
			goto CHECK_END;
		} 

	    if( (moduleInfo.moduleId==LOGIC_ID) && (segTbl.fbUsedNum > MAX_MPU_BLOCK_ADDR) )
		{
            retVal = FB_NUM_EXCEED_ERR;
            status = FB_NUM_EXCEED_ERR;
	        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, retVal,
								"Task Id", taskId,
                                "Seg FB Used Num", segTbl.fbUsedNum,
                                "MAX FB Num", MAX_MPU_BLOCK_ADDR
			  		  			);
			goto CHECK_END;
		} 
       
		
		for(dataNum = 0; dataNum < codeNumber; dataNum++) 
		{
            fbId	= (*reqDataPtr >> 16) & 0xffff;
			fbBlockAddr = *reqDataPtr++ & 0xffff;

			anyFuncOutDataSize = (*reqDataPtr >> 16)&0xffff;
			anyFuncOutDataType = *reqDataPtr++& 0xffff;

			if( (fbBlockAddr < 0) || (fbBlockAddr > moduleInfo.maxFbBlockAddr) ) 
			{	
				status = L_FB_ADDR_ERR;
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"FB Block Addr",fbBlockAddr,
									"Cur MAX Block Addr", moduleInfo.maxFbBlockAddr,
									"FB Code ID", fbId
				  		  			);
				goto CHECK_END;
			}				
			
			
			if( (fbId == 0x0) || (fbId > MAX_FB_CODE_ID))
			{
                status = L_FB_CODE_ERR;
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"Task Id", taskId,
									"Block Addr", fbBlockAddr,
									"Fb Code Id", fbId
			  						);
				goto CHECK_END;			
			}

			if(IS_ANY_TYPE_FB_CODE(fbId)) 
			{
				
				if( (anyFuncOutDataSize < 1) || (anyFuncOutDataSize > 5))
				{
					status = ANY_FUNC_OUT_SIZE_ERR;
			        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
										"Block Addr", fbBlockAddr,
										"FB Code Id",fbId,
										"Out Data Size", anyFuncOutDataSize
		  								);
					goto CHECK_END;			
				}

				
				if( (anyFuncOutDataType < 1) || (anyFuncOutDataType > 10))
				{
					status = ANY_FUNC_OUT_TYPE_ERR;
			        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
										"Block Addr", fbBlockAddr,
										"FB Code Id",fbId,
										"Out Data Type", anyFuncOutDataType
						  				);
					goto CHECK_END;			
				}
			}

			
            memoryClear( 	(uint8 *) &fbCodeInfo,
                			sizeof(strFbExecDbInfo)
						);
			if ((retVal = fbDbTableSortingInfoRead (	
										fbId,
										&fbCodeInfo
									)
				) != NO_ERROR)
			{
	            status = L_FB_CODE_ERR;
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"FB Code", fbId,
									"Not Using", 0,
									"Not Using", 0
		  		  					);
				goto CHECK_END;                	
			}
			
			memoryCopy((uint8*)&outputNegate[0],(uint8*)reqDataPtr++,LONG_SIZE);
			reqDataPtr++;
			
			if(moduleInfo.moduleId == LOGIC_ID) 
			{
				if(
					((fbCodeInfo.outputNo<=32)&&( (outputNegate[0] >> fbCodeInfo.outputNo) != 0x0) ) ||
					((fbCodeInfo.outputNo>32)&&( (outputNegate[1] >> fbCodeInfo.outputNo) != 0x0) )) 
				{
					status = FB_CODE_OUTPUT_ACTION_ERR;
			        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
										"FB Code", fbId,
										"Output Action(MSB)", outputNegate[0],
										"Output Action(LSB)", outputNegate[1]
		  		  						);
					goto CHECK_END;
				}
			}

			specNum = (*reqDataPtr >> 16)&0xffff;
			specSizeNum = *reqDataPtr++& 0xffff;
			
			
			
			if(specNum != fbCodeInfo.inputNo)
			{
                retVal = L_PARA_NO_ERR;
	            status = L_PARA_NO_ERR;

		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"FB CODE", fbId,
									"RECV PARA NUM",specNum,
									"DEF PARA NUM",fbCodeInfo.inputNo
				  		  			);
				goto CHECK_END;                	
			}

			
			else if(specSizeNum > fbCodeInfo.inputSize) 
			{
                retVal = L_PARA_NO_ERR;
	            status = L_PARA_NO_ERR;

		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"FB CODE", fbId,
									"RECV PARA SIZE",specSizeNum,
									"DEF PARA SIZE",fbCodeInfo.inputSize
				  		  			);
				goto CHECK_END;                	
			}

			if(moduleInfo.moduleId == LOGIC_ID) 
			{	
				
				retVal= fbCodeParaTypePointerGet(fbId, (strFbCodeTypePointerInfo *)&typePInfo);
				if(retVal != NO_ERROR) 
				{
					status = retVal;
			        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
					goto CHECK_END;
				}
				else
				{
					paraTypePtr = (uint32 *)typePInfo.specTypeStartAddr;
				}
				
				tmpPtr = reqDataPtr;

				exceptNumber = 0;
				for(j = 0; j < specNum; j++) 
				{
					specActionInfoPtr = (strFbSpecActionInfo*)reqDataPtr++;
					
					sIndex = *((uint32 *)typePInfo.specSizeStartAddr + j - exceptNumber);
					
					if (sIndex >= specSizeNum) 
					{
						status = FB_SPEC_TYPE_SIZE_ERR;
				        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
				        
						goto CHECK_END;
					}
					
					
					if(GET_BASIC_SIZE(specActionInfoPtr->dataType) == SIZE64_TYPE)
						specData = *(tmpPtr + specNum + sIndex + 1);
					else
						specData = *(tmpPtr + specNum + sIndex) ;
					
					
					status = specActionDataCheck((uint32*)specActionInfoPtr, specData);
					if(status != NO_ERROR) 	
					{
						setErrorCodeWithVal(__FILE__, __LINE__,__FUNCTION__,status,
											"FB Block Addr",fbBlockAddr,
											"FB Code",fbId,
											"Input Index",j+1
											);
						goto CHECK_END;								
					}

					
					if( ( (specActionInfoPtr->dataSize == SPEC_SIZE_LWORD)&&(specActionInfoPtr->bitposFlag == 0x1) ) ||
						(specActionInfoPtr->dataType == SPEC_DATA_TYPE_LINT) ||
						(specActionInfoPtr->dataType == SPEC_DATA_TYPE_ULINT) ||
						(specActionInfoPtr->dataType == SPEC_DATA_TYPE_LREAL) ||
						(specActionInfoPtr->dataType == SPEC_DATA_TYPE_DT))
					{
						if (GET_TYPE_SIZE(*(paraTypePtr + j))  == SIZE32_TYPE )
						{
							status = FB_SPEC_TYPE_SIZE_ERR;
							setErrorCodeWithVal( 	__FILE__,__LINE__, __FUNCTION__,status,
													"Input Size", specActionInfoPtr->dataSize,
													"Input Type", specActionInfoPtr->dataType,
													"Cur FB Size", SIZE32_TYPE); 
							goto CHECK_END;
						}
					}
					
					if(specActionInfoPtr->taskID != taskId) 
					{
						status = FB_SPEC_ACTION_TASKID_ERR;
				        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
				        
						goto CHECK_END;
					}
					
					else if(specActionInfoPtr->dataType > SPEC_DATA_TYPE_DT)	
					{
						status = FB_SPEC_ACTION_TYPE_ERR;
				        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
				        
						goto CHECK_END;
					}

					
					if(!CHK_IS_ANY_TYPE(*(paraTypePtr + j )) ) 
					{
						if( specActionInfoPtr->dataType != GET_BASIC_DATA_TYPE(*(paraTypePtr + j)) )
						{
							status = FB_CODE_SPEC_TYPE_ERR ;
					        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
				            					"Input Index", j + 1,
				                				"Recv Type", specActionInfoPtr->dataType,
				                    			"Read Type", GET_BASIC_DATA_TYPE(*(paraTypePtr + j))
												);
					        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
				            					"FB ADDR", fbBlockAddr,
				                				"FB CODE", fbId,
				                    			"TASK ID", taskId
												);
							goto CHECK_END;
						}
					}
				} 
			} 
			else 
			{	
				reqDataPtr += specNum;
			}

			reqDataPtr += specSizeNum;
			
			if(status != NO_ERROR) 
			{
		        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
				goto CHECK_END;
			}
		} 
	} 

CHECK_END:
	
	if(status) 
	{
		segData.segInfo[i].status	= status & 0xffff;
        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

    
	
	reqDataPtr	= (uint32 *)&reqMsgPtr->dataBuff[12];
	
   	respDataPtr = (uint32 *)&respMsgPtr->dataBuf[12];
	
	
    memoryClear( 	(uint8 *)&fbMem,
        			sizeof(strMemPointerInfo)
				);

    if( (retVal = fbMemPointerGet	(
										moduleInfo.moduleId,
										&fbMem
                        			)
		) != NO_ERROR)
	{
		status = retVal;
        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
		goto MODULE_END;
    }

	
	firstFbCodePtr = (strFbMemInfo *) fbMem.fbMemPtr;
	firstFbParaPtr = (uint32 *)fbMem.paraMemPtr;
	firstFbOutputPtr = (uint32 *)fbMem.outputMemPtr;

	
	for(i = 0; i < reqHeaderptr->segmentNum; i++) 
	{
		taskId = (*reqDataPtr >> 16)&0xffff;	
        codeNumber = *reqDataPtr++ & 0xffff;
  
		if( (retVal = segTblPointerGet( 	moduleInfo.moduleId,
		            						taskId,
        		                            &segTblPtr
									  )
			) != NO_ERROR)
		{
			status = retVal;
	        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
			goto MAKE_ERR_RESP;
		}       

		segTblPtr->fbUsedNum += codeNumber;
		
		if(moduleInfo.moduleId==LOGIC_ID) 
		{
			paraIndexPtr->curParaIndex = gMpuProgDwlPara.curParaIndex;
			paraIndexPtr->curOutputIndex = gMpuProgDwlPara.curOutputIndex;
		} 
		else 
		{
			paraIndexPtr->curParaIndex = gDiuProgDwlPara.curParaIndex;
			paraIndexPtr->curOutputIndex = gDiuProgDwlPara.curOutputIndex;
		}

		for(dataNum = 0; dataNum < codeNumber; dataNum ++) 
		{
            fbCodePtr = firstFbCodePtr;
			fbParaPtr = firstFbParaPtr;
            fbOutputPtr = firstFbOutputPtr;

            fbId	= (*reqDataPtr >> 16) & 0xffff;
			fbBlockAddr = *reqDataPtr++ & 0xffff;
			
			anyFuncOutDataSize = (*reqDataPtr >> 16)&0xffff;
			anyFuncOutDataType = *reqDataPtr++& 0xffff;
				
			
			memoryCopy((uint8*)&outputNegate[0],(uint8*)reqDataPtr++,LONG_SIZE);
			reqDataPtr++;

			specNum = (*reqDataPtr >> 16)&0xffff;
			specSizeNum = *reqDataPtr++& 0xffff;

			
			memoryClear( 	(uint8 *) &fbCodeInfo,
                			sizeof(strFbExecDbInfo)
						);
			if( (retVal = fbDbTableSortingInfoRead (	
										fbId,
										&fbCodeInfo
									)
				) != NO_ERROR)
			{
	            status = L_FB_CODE_ERR;
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
			                        "FB CODE", fbId,
			                        "RECV PARA NUM", specNum,
			                        "DEF PARA NUM", fbCodeInfo.inputNo
		  		  					);
				goto MAKE_ERR_RESP;                	
			}

			if(fbBlockAddr > 0) 
			{
				fbCodePtr += fbBlockAddr - 1;
			} 
			else
			{
				status = L_FB_ADDR_ERR;
		        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
		        
				goto MAKE_ERR_RESP;                	
			}
			
			fbCodePtr->fbTaskId		= taskId ;
			fbCodePtr->fbFuncAddr   = fbBlockAddr;
            fbCodePtr->fbId			= fbId;
            fbCodePtr->inputNo		= fbCodeInfo.inputNo;
            fbCodePtr->intlVarNo	= fbCodeInfo.intlVarNo;
            fbCodePtr->outputNo		= fbCodeInfo.outputNo;
			
			fbCodePtr->inputSize	= fbCodeInfo.inputSize;
            fbCodePtr->intlVarSize	= fbCodeInfo.intlVarSize;
            fbCodePtr->outputSize	= fbCodeInfo.outputSize;

			fbCodePtr->fbRunStatus	= 0;
			fbCodePtr->paraRunType  = (anyFuncOutDataType&0xF) | ((anyFuncOutDataSize&0xF)<<12);
			fbCodePtr->usingType	= 0;

			if(moduleInfo.moduleId == LOGIC_ID) 
			{
				fbCodePtr->specActionIndex 	= paraIndexPtr->curParaIndex;
				fbCodePtr->specIndex 		= paraIndexPtr->curParaIndex + fbCodeInfo.inputNo ;
				fbCodePtr->varIndex	 		= paraIndexPtr->curParaIndex 
											+ fbCodeInfo.inputNo    
											+ fbCodeInfo.inputSize; 
				 
				fbCodePtr->logicType = 0;

				fbCodePtr->outputIndex 	= paraIndexPtr->curOutputIndex;
			}
			else
			{
				status = L_MODULE_ID_ERR;
		        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
		        
				goto MAKE_ERR_RESP;                				
			}			
			
			memoryCopy((uint8*)&fbCodePtr->outputAction[0],(uint8*)&outputNegate[0], LONG_SIZE );

			if(moduleInfo.moduleId == LOGIC_ID) 
			{
				fbParaPtr += fbCodePtr->specActionIndex;
				tmpPtr = fbParaPtr;   
             
				for(j = 0; j < specNum; j++) 
				{
					
					specActionInfoPtr = (strFbSpecActionInfo*)reqDataPtr++;
					memoryCopy ( (uint8 *) tmpPtr,(uint8 *) specActionInfoPtr, 4 );
					tmpPtr++;
				} 

			} 
			else 
			{

				fbParaPtr += fbCodePtr->specIndex;
				tmpPtr = fbParaPtr;
				for(j = 0; j < specNum; j++) 
				{
					reqDataPtr++;
				} 
			}

			
			if ( tmpPtr != (firstFbParaPtr + fbCodePtr->specIndex) ) 
			{
				status = FB_SPEC_INDEX_PTR_ERR;
		        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
		        
				goto MAKE_ERR_RESP;
			}
			
			retVal= fbCodeParaTypePointerGet(fbCodePtr->fbId, (strFbCodeTypePointerInfo *)&typePInfo);
			if(retVal != NO_ERROR) 
			{
				status = retVal;
			    setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
				goto MAKE_ERR_RESP;
			} 
			else 
			{
				paraTypePtr = (uint32 *)typePInfo.specTypeStartAddr;
			}
				
			
			for(j = 0; j < specNum; j++) 
			{			
				
				specData = *reqDataPtr++;
                memoryCopy ( (uint8 *) tmpPtr,(uint8 *) &specData, 4 );
				tmpPtr++;

				specActionParaTypeDefInfo = *(strFbSpecActionInfo *)(fbParaPtr + j);
				fbParaTypeDef = *(uint32 *)(paraTypePtr + j);

				if( (GET_TYPE_SIZE(fbParaTypeDef)==SIZE64_TYPE) && 
					
					(GET_BASIC_SIZE(specActionParaTypeDefInfo.dataType) == SIZE64_TYPE))
				{
					
					specData = *reqDataPtr++;
					memoryCopy ( (uint8 *) tmpPtr,(uint8 *) &specData, 4 );
#ifdef VXWORKS
					
					if(specActionParaTypeDefInfo.refType == SPEC_TYPE_BYVAL)
						mySwap((uint32*)(tmpPtr-1),(uint32*)tmpPtr,DOUBLE_SIZE);
#endif 
					tmpPtr++;
				}
				else if( (GET_TYPE_SIZE(fbParaTypeDef)==SIZE64_TYPE) && 
						 (CHK_IS_ANY_TYPE(fbParaTypeDef))			&&
					 	 
						 (GET_BASIC_SIZE(specActionParaTypeDefInfo.dataType) == SIZE32_TYPE))
				{	
					specData = 0;
					memoryCopy ( (uint8 *) tmpPtr,(uint8 *) &specData, 4 );
					tmpPtr++;
				}
			} 

			if ( tmpPtr != (firstFbParaPtr + fbCodePtr->varIndex) ) 
			{
				status = FB_VAR_INDEX_PTR_ERR;
		        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
		        
				goto MAKE_ERR_RESP;
			}
			
			
            for(j = 0; j < fbCodePtr->intlVarSize; j++) 
            {
                memoryClear (	(uint8 *) tmpPtr, 
                    			4 
							);
                tmpPtr++;
			} 

			
            fbOutputPtr += fbCodePtr->outputIndex;
			tmpPtr2 = fbOutputPtr;
			
            for(j = 0; j < fbCodeInfo.outputSize; j++) 
            {
                memoryClear ((uint8 *) tmpPtr2, 4);
                tmpPtr2++;
			}

			
			if(moduleInfo.moduleId == LOGIC_ID) 
			{
				paraIndexPtr->curParaIndex += specNum + fbCodePtr->inputSize + fbCodePtr->intlVarSize;
			} 
			else 
			{
				paraIndexPtr->curParaIndex += fbCodePtr->inputSize + fbCodePtr->intlVarSize;
			}

			if(moduleInfo.moduleId == LOGIC_ID)
			{
				paraIndexPtr->curOutputIndex += fbCodePtr->outputSize;
			}

			if (tmpPtr!= firstFbParaPtr+paraIndexPtr->curParaIndex) 
			{
				status = FB_PARA_INDEX_ERR;
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
			                        "Cur Para Index",paraIndexPtr->curParaIndex,
			                        "Para Index Mismatch",0,
			                        "Module Id",  moduleInfo.moduleId
					  		  		);
				goto MAKE_ERR_RESP;
			}

			if(moduleInfo.moduleId==LOGIC_ID) 
			{
				if (tmpPtr2!= (firstFbOutputPtr+ paraIndexPtr->curOutputIndex)) 
				{
					status = FB_CODE_OUTPUT_INDEX_ERR;
			        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
										"Cur Out Index",paraIndexPtr->curOutputIndex,
										"Para Index Mismatch:FB Code ID=",fbCodePtr->fbId,
										"module Id",  moduleInfo.moduleId
					  		  			);
					goto MAKE_ERR_RESP;
				}
			}
			
			if(sysExeInfo.curfbMaxBlockAddr < fbBlockAddr) 
				sysExeInfo.curfbMaxBlockAddr = fbBlockAddr;
				
			if(sysExeInfo.curfbMaxParaIndex < paraIndexPtr->curParaIndex) 
				sysExeInfo.curfbMaxParaIndex = paraIndexPtr->curParaIndex;
			
			if(moduleInfo.moduleId == LOGIC_ID) 
			{
				if(sysExeInfo.curfbMaxOutIndex < paraIndexPtr->curOutputIndex) 
					sysExeInfo.curfbMaxOutIndex = paraIndexPtr->curOutputIndex;
			}
			else 
			{
				if( sysExeInfo.curfbMaxOutIndex < paraIndexPtr->curOutputIndex ) 
					sysExeInfo.curfbMaxOutIndex = paraIndexPtr->curOutputIndex;
			}
            
            if( paraIndexPtr->curParaIndex > (moduleInfo.maxFbParaNum + 1) )
            {
                retVal = PARA_SIZE_EXCEED_ERR;
                status = PARA_SIZE_EXCEED_ERR;
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
			                        "Cur Para Index", paraIndexPtr->curParaIndex,
			                        "MAX Para Size", moduleInfo.maxFbParaNum,
			                        "Block Addr",  fbCodePtr->fbFuncAddr
					  		  		);
				goto MAKE_ERR_RESP;
			}
			
            if( paraIndexPtr->curOutputIndex > (moduleInfo.maxFbOutputNum + 1) )
            {
                retVal = L_FB_ADDR_ERR;
                status = L_FB_ADDR_ERR;
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
			                        "Cur Output Index", paraIndexPtr->curOutputIndex,
			                        "MAX Output Size",  moduleInfo.maxFbOutputNum,
			                        "Not Using",  0
							  		);
				goto MAKE_ERR_RESP;
            }

            
			if(moduleInfo.moduleId == LOGIC_ID) 
			{
				segTblPtr->memInputSize += (fbCodePtr->inputNo + fbCodePtr->inputSize+ fbCodePtr->intlVarSize  );
				segTblPtr->memOutputSize += fbCodePtr->outputSize;
			} 
			else
			{
				segTblPtr->memInputSize += (fbCodePtr->inputSize+ fbCodePtr->intlVarSize) ;
				segTblPtr->memOutputSize += fbCodePtr->outputSize;
			}
		} 		
	} 

	status = fbSysExeModuleInfoWrite(reqHeaderptr->memoryKind, &sysExeInfo);
	if(status != NO_ERROR)
        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);

MAKE_ERR_RESP:
	
	if(status != NO_ERROR) 
	{
	 	for(i = 0; i <  reqHeaderptr->segmentNum; i++) 
	 	{
			segData.segInfo[i].status = status & 0xffff;
		}				
	}
	
MODULE_END:

 	for(i = 0; i <  reqHeaderptr->segmentNum; i++) 
 	{
 		*respDataPtr++ = segData.segInfo[i].taskId;
		*respDataPtr++ = segData.segInfo[i].status ;
	    respDataLen += DOUBLE_SIZE*2;	    
 	}
 	
 	respMsgPtr->dataLen = respDataLen;

    return(status);    
}



static uint32 busFbProgWriteReqAndResp 
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR;
	uint32	retVal = NO_ERROR;
	uint32	respDataLen = 0;
	uint32	*reqDataPtr	;					
	uint32	*respDataPtr ;					

	uint32  i, j;
	uint32	dataNum;
	uint32	taskId;
	uint32	codeNumber;
	uint32	fbId;
	uint32	fbBlockAddr;
	uint32  specNum;
	uint32	specSizeNum;
	uint32	specData; 
	uint32  outputNegate[2];
	uint32	specAction;
	uint32	anyFuncOutDataSize;
	uint32	anyFuncOutDataType;


	strTotalSegDataInfo	segData;
	strModuleInfo		moduleInfo;
	strFbTaskTblMemInfo	segTbl, *segTblPtr;
	strProgDwlParaInfo	*paraIndexPtr;

	strFbDataGrHeaderInfo *reqHeaderptr;
    strFbExecDbInfo fbCodeInfo;
	strMemPointerInfo	fbMem;

	strFbMemInfo	*fbCodePtr, *firstFbCodePtr;
    uint32	*fbParaPtr,*firstFbParaPtr, *tmpPtr , *tmpPtr2;
	uint32	*fbOutputPtr, *firstFbOutputPtr;

	strFbCodeTypePointerInfo typePInfo;
	uint32 *paraTypePtr;

	strFbSpecActionInfo specActionParaTypeDefInfo;
	uint32	fbParaTypeDef;
	uint32	compareAddr;

	memoryClear( (uint8 *)&moduleInfo, sizeof(strModuleInfo));
	memoryClear( (uint8 *)&segData, sizeof(strTotalSegDataInfo));
	memoryClear( (uint8 *)&typePInfo, sizeof(strFbCodeTypePointerInfo));
	memoryClear( (uint8 *)&outputNegate[0], LONG_SIZE);

	
	reqHeaderptr = (strFbDataGrHeaderInfo *)reqMsgPtr->dataBuff;
    memoryCopy((uint8*)respMsgPtr->dataBuf,(uint8*)reqHeaderptr,
										sizeof(strFbDataGrHeaderInfo) );
	
	respDataLen = sizeof(strFbDataGrHeaderInfo);         

    
	
	reqDataPtr	= (uint32 *)&reqMsgPtr->dataBuff[12];
	
	
   	respDataPtr = (uint32 *)&respMsgPtr->dataBuf[12];

	
	memoryClear( (uint8 *)&moduleInfo, sizeof(strModuleInfo));
	retVal = moduleInfoRead(reqHeaderptr->memoryKind, &moduleInfo);

	if(retVal) 
	{
    	status = L_MODULE_ID_ERR;
        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
		goto MODULE_END;
    }

    
    if( (reqHeaderptr->segmentNum < 1) || (reqHeaderptr->segmentNum > moduleInfo.maxSegmentNum))
    {
		status = L_ITEM_NO_ERR;
        retVal = L_ITEM_NO_ERR;
        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
        
		goto MODULE_END;
	}
    
    if(moduleInfo.moduleId == LOGIC_ID)
    	paraIndexPtr = &gMpuProgDwlBusPara;
   	else
    	paraIndexPtr = &gDiuProgDwlBusPara;

   	segData.totalSegNum = reqHeaderptr->segmentNum;

	
    for(i = 0; i < reqHeaderptr->segmentNum; i++) 
	{
		segData.segInfo[i].startDataPtr = reqDataPtr;

		taskId = (*reqDataPtr >> 16)&0xffff;	
		codeNumber = *reqDataPtr++ & 0xffff;

		segData.segInfo[i].taskId 	= taskId;
		segData.segInfo[i].status 	= NO_ERROR;

		
        memoryClear ( (uint8 *) &segTbl, sizeof(strFbTaskTblMemInfo));
        
        if ( (retVal = busSegTblInfoRead	(
            				moduleInfo.moduleId,
							taskId,
                    	    &segTbl
                            			)
			 ) != NO_ERROR)
		{
			status = retVal;
           	segData.segInfo[i].status = retVal & 0xff;
	        setErrorCode(__FILE__,__LINE__, __FUNCTION__, retVal);
	        
			goto CHECK_END;
        }

		
        if(	(taskId < 1) || (taskId > moduleInfo.maxSegmentNum)) 
        {
			segData.segInfo[i].status = L_SEG_ID_ERR & 0xff;
			retVal = L_SEG_ID_ERR;
			status = L_SEG_ID_ERR;			
	        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
                    			"Recv Task Id", taskId,
                    			"Max Task Id", moduleInfo.maxSegmentNum,
                    			"Not Used", 0
			  		  			);
			

			for(dataNum = 0; dataNum < codeNumber; dataNum++) 
			{
                fbId  = (*reqDataPtr >> 16)&0xffff;
				fbBlockAddr = *reqDataPtr++& 0xffff;

				anyFuncOutDataSize = (*reqDataPtr >> 16)&0xffff;
				anyFuncOutDataType = *reqDataPtr++& 0xffff;
				
				memoryCopy((uint8*)&outputNegate[0],(uint8*)reqDataPtr++,LONG_SIZE);
				reqDataPtr++;

				specNum = (*reqDataPtr >> 16)&0xffff;
				specSizeNum = *reqDataPtr++& 0xffff;

				reqDataPtr += specNum;
				reqDataPtr += specSizeNum;

            }
			
            goto CHECK_END;;
        }

		
        if(segTbl.taskMode != PCS_DOWNLOAD) 
        {
            retVal = L_PCS_MODE_ERR;
            status = L_PCS_MODE_ERR;
	        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Task Id", taskId,
                                "Seg Mode", segTbl.taskMode,
                                "NOT USING", 0
			  		  			);
			goto CHECK_END;
		} 

	    if( (moduleInfo.moduleId==LOGIC_ID) && (segTbl.fbUsedNum > MAX_MPU_BLOCK_ADDR) )
		{
            retVal = FB_NUM_EXCEED_ERR;
            status = FB_NUM_EXCEED_ERR;
	        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Task Id", taskId,
                                "Seg FB Used Num", segTbl.fbUsedNum,
                                "MAX FB Num", MAX_MPU_BLOCK_ADDR
			  		  			);
			goto CHECK_END;
		} 
       
		
		for(dataNum = 0; dataNum < codeNumber; dataNum++) 
		{
            fbId	= (*reqDataPtr >> 16) & 0xffff;
			fbBlockAddr = *reqDataPtr++ & 0xffff;

			anyFuncOutDataSize = (*reqDataPtr >> 16)&0xffff;
			anyFuncOutDataType = *reqDataPtr++& 0xffff;

			if( (fbBlockAddr < 0) || (fbBlockAddr > moduleInfo.maxFbBlockAddr) ) 
			{
				status = L_FB_ADDR_ERR;
		        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
				goto CHECK_END;
			}				
			
			
			if( (fbId == 0x0) || (fbId > MAX_FB_CODE_ID))
			{
                status = L_FB_CODE_ERR;
		        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
				goto CHECK_END;			
			}

			if(IS_ANY_TYPE_FB_CODE(fbId)) 
			{
				
				if( (anyFuncOutDataSize < 1) || (anyFuncOutDataSize > 5))
				{
					status = ANY_FUNC_OUT_SIZE_ERR;
			        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
										"Task Id", taskId,
										"Block Addr", fbBlockAddr,
										"Out Data Size", anyFuncOutDataSize
			  							);
					goto CHECK_END;			
				}

				
				if( (anyFuncOutDataType < 1) || (anyFuncOutDataType > 10))
				{
					status = ANY_FUNC_OUT_TYPE_ERR;
			        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
										"Task Id", taskId,
										"Block Addr", fbBlockAddr,
										"Out Data Type", anyFuncOutDataType
			  							);
					goto CHECK_END;			
				}
			}

			
            memoryClear( 	(uint8 *) &fbCodeInfo,
                			sizeof(strFbExecDbInfo)
						);
			if ((retVal = fbDbTableSortingInfoRead (	
										fbId,
										&fbCodeInfo
									)
				) != NO_ERROR)
			{
	            status = L_FB_CODE_ERR;
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"FB Code", fbId,
									"Not Using", 0,
									"Not Using", 0
		  		  					);
				goto CHECK_END;                	
			}
			
			memoryCopy((uint8*)&outputNegate[0],(uint8*)reqDataPtr++,LONG_SIZE);
			reqDataPtr++;

			
			
			if(moduleInfo.moduleId == LOGIC_ID) 
			{
				if(
					((fbCodeInfo.outputNo<=32)&&( (outputNegate[0] >> fbCodeInfo.outputNo) != 0x0) ) ||
					((fbCodeInfo.outputNo>32)&&( (outputNegate[1] >> fbCodeInfo.outputNo) != 0x0) ) )
				{
					status = FB_CODE_OUTPUT_ACTION_ERR;
			        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
										"FB Code", fbId,
										"Output Action(MSB)", outputNegate[0],
										"Output Action(LSB)", outputNegate[1]
		  		  						);
					goto CHECK_END;
				}
			}

			specNum 	= (*reqDataPtr >> 16)&0xffff;
			specSizeNum = *reqDataPtr++& 0xffff;
			
			
			
			if(specNum != fbCodeInfo.inputNo)
			{
                retVal = L_PARA_NO_ERR;
	            status = L_PARA_NO_ERR;

		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
			                        "FB CODE", fbId,
			                        "RECV PARA NUM", specNum,
			                        "DEF PARA NUM", fbCodeInfo.inputNo
			  		  				);
				goto CHECK_END;                	
			}

			
			if( (specSizeNum > fbCodeInfo.inputSize) ) 
			{
                retVal = L_PARA_NO_ERR;
	            status = L_PARA_NO_ERR;

		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
			                        "FB CODE", fbId,
			                        "RECV PARA NUM", specNum,
			                        "DEF PARA NUM", fbCodeInfo.inputNo
			  		  				);
				goto CHECK_END;                	
			}

			reqDataPtr += specNum;
			reqDataPtr += specSizeNum;
			if(status != NO_ERROR) 
			{
		        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
		        
				goto CHECK_END;
			}
		} 
	} 

CHECK_END:
	
	if(status) 
	{
		segData.segInfo[i].status	= status & 0xffff;
        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}
	
	reqDataPtr	= (uint32 *)&reqMsgPtr->dataBuff[12];
	
   	respDataPtr = (uint32 *)&respMsgPtr->dataBuf[12];
	
	
    memoryClear( 	(uint8 *)&fbMem,
        			sizeof(strMemPointerInfo)
				);

    if( (retVal = busFbMemPointerGet	(
										moduleInfo.moduleId,
										&fbMem
                        			)
		) != NO_ERROR)
	{
		status = retVal;
        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
        
		goto MODULE_END;
    }
	
	firstFbCodePtr = (strFbMemInfo *) fbMem.fbMemPtr;
	firstFbParaPtr = (uint32 *)fbMem.paraMemPtr;
	firstFbOutputPtr = (uint32 *)fbMem.outputMemPtr;

	
	for(i = 0; i < reqHeaderptr->segmentNum; i++) 
	{
		taskId = (*reqDataPtr >> 16)&0xffff;	
        codeNumber = *reqDataPtr++ & 0xffff;
	   
		if( (retVal = busSegTblPointerGet( 	moduleInfo.moduleId,
		            						taskId,
        		                            &segTblPtr
									  )
			) != NO_ERROR)
		{
			status = retVal;
	        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
	        
			goto MAKE_ERR_RESP;
		}       

		segTblPtr->fbUsedNum += codeNumber;
		if(moduleInfo.moduleId==LOGIC_ID) 
		{
			paraIndexPtr->curParaIndex = gMpuProgDwlBusPara.curParaIndex;
			paraIndexPtr->curOutputIndex = gMpuProgDwlBusPara.curOutputIndex;
		} 
		else 
		{
			paraIndexPtr->curParaIndex = gDiuProgDwlBusPara.curParaIndex;
			paraIndexPtr->curOutputIndex = gDiuProgDwlBusPara.curOutputIndex;
		}

		for(dataNum = 0; dataNum < codeNumber; dataNum ++) 
		{
            fbCodePtr 	= firstFbCodePtr;
			fbParaPtr 	= firstFbParaPtr;
            fbOutputPtr = firstFbOutputPtr;

            fbId		= (*reqDataPtr >> 16) & 0xffff;
			fbBlockAddr = *reqDataPtr++ & 0xffff;
			
			anyFuncOutDataSize = (*reqDataPtr >> 16)&0xffff;
			anyFuncOutDataType = *reqDataPtr++& 0xffff;
			
			memoryCopy((uint8*)&outputNegate[0],(uint8*)reqDataPtr++,LONG_SIZE);
			reqDataPtr++;

			specNum = (*reqDataPtr >> 16)&0xffff;
			specSizeNum = *reqDataPtr++& 0xffff;

			
			memoryClear( 	(uint8 *) &fbCodeInfo,
                			sizeof(strFbExecDbInfo)
						);
			if( (retVal = fbDbTableSortingInfoRead (	
										fbId,
										&fbCodeInfo
									)
				) != NO_ERROR)
			{
	            status = L_FB_CODE_ERR;
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
			                        "FB CODE",fbId,
			                        "RECV PARA NUM",specNum,
			                        "DEF PARA NUM",fbCodeInfo.inputNo
						  		  	);
				goto MAKE_ERR_RESP;                	
			}

            
			if(fbBlockAddr > 0) 
			{
				fbCodePtr += fbBlockAddr - 1;
			} 
			else
			{
				status = L_FB_ADDR_ERR;
		        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
		        
				goto MAKE_ERR_RESP;                	
			}
			
			fbCodePtr->fbTaskId		= taskId ;
			fbCodePtr->fbFuncAddr   = fbBlockAddr;
            fbCodePtr->fbId			= fbId;
            fbCodePtr->inputNo		= fbCodeInfo.inputNo;
            fbCodePtr->intlVarNo	= fbCodeInfo.intlVarNo;
            fbCodePtr->outputNo		= fbCodeInfo.outputNo;
			
			fbCodePtr->inputSize	= fbCodeInfo.inputSize;
            fbCodePtr->intlVarSize	= fbCodeInfo.intlVarSize;
            fbCodePtr->outputSize	= fbCodeInfo.outputSize;

			fbCodePtr->fbRunStatus	= 0;
			fbCodePtr->paraRunType  = (anyFuncOutDataType&0xF) | ((anyFuncOutDataSize&0xF)<<12);
			fbCodePtr->usingType	= 0;

			if(moduleInfo.moduleId == LOGIC_ID) 
			{
				fbCodePtr->specActionIndex 	= paraIndexPtr->curParaIndex;
				fbCodePtr->specIndex 		= paraIndexPtr->curParaIndex + fbCodeInfo.inputNo ;
				fbCodePtr->varIndex	 		= paraIndexPtr->curParaIndex 
											+ fbCodeInfo.inputNo    
											+ fbCodeInfo.inputSize; 
				 
				fbCodePtr->logicType = 0;

				fbCodePtr->outputIndex 	= paraIndexPtr->curOutputIndex;
			}
			else
			{
				status = L_MODULE_ID_ERR;
		        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
		        
				goto MAKE_ERR_RESP;                				
			}			
			
			memoryCopy((uint8*)&fbCodePtr->outputAction[0],(uint8*)&outputNegate[0], LONG_SIZE );

			if(moduleInfo.moduleId == LOGIC_ID) 
			{
				fbParaPtr += fbCodePtr->specActionIndex;
				tmpPtr = fbParaPtr;   
             
				for(j = 0; j < specNum; j++) 
				{
					
					specAction = *reqDataPtr++;
					memoryCopy ( (uint8 *) tmpPtr,(uint8 *) &specAction, 4 );
					tmpPtr++;
				} 

			} 
			else 
			{

				fbParaPtr += fbCodePtr->specIndex;
				tmpPtr = fbParaPtr;
				for(j = 0; j < specNum; j++) 
				{
					reqDataPtr++;
				} 
			}

			
			compareAddr = (uint32)(firstFbParaPtr + fbCodePtr->specIndex);
			if ( (uint32)tmpPtr != compareAddr ) 
			{
				status = FB_SPEC_INDEX_PTR_ERR;
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"tmpPtr",(uint32)tmpPtr, 
									"FbspecIndexPtr",(uint32)compareAddr, 
									"fbBlockAddr",(uint32)fbBlockAddr
					  		  		);
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"firstFbParaPtr",(uint32)firstFbParaPtr, 
									"specIndex",fbCodePtr->specIndex, 
									"fbBlockAddr",(uint32)fbBlockAddr
					  		  		);
				goto MAKE_ERR_RESP;
			}

			retVal= fbCodeParaTypePointerGet(fbCodePtr->fbId, (strFbCodeTypePointerInfo *)&typePInfo);
			if(retVal != NO_ERROR) 
			{
				status = retVal;
		        setErrorCode(__FILE__,__LINE__, __FUNCTION__, status);
				goto MAKE_ERR_RESP;
			} 
			else 
			{
				paraTypePtr = (uint32 *)typePInfo.specTypeStartAddr;
			}
				
			
			for(j = 0; j < specNum; j++) 
			{			
				
				specData = *reqDataPtr++;
                memoryCopy ( (uint8 *) tmpPtr,(uint8 *) &specData, 4 );
				tmpPtr++;

				specActionParaTypeDefInfo = *(strFbSpecActionInfo *)(fbParaPtr + j);
				fbParaTypeDef = *(uint32 *)(paraTypePtr + j);

				if( (GET_TYPE_SIZE(fbParaTypeDef)==SIZE64_TYPE) && 
					
					(GET_BASIC_SIZE(specActionParaTypeDefInfo.dataType) == SIZE64_TYPE))
				{
					
					specData = *reqDataPtr++;
					memoryCopy ( (uint8 *) tmpPtr,(uint8 *) &specData, 4 );
#ifdef VXWORKS
					
					if(specActionParaTypeDefInfo.refType == SPEC_TYPE_BYVAL)
						mySwap((uint32*)(tmpPtr-1),(uint32*)tmpPtr,DOUBLE_SIZE);
#endif 
					tmpPtr++;
				}
				else if( (GET_TYPE_SIZE(fbParaTypeDef)==SIZE64_TYPE) && 
						 (CHK_IS_ANY_TYPE(fbParaTypeDef))			&&
					 	 
						 (GET_BASIC_SIZE(specActionParaTypeDefInfo.dataType) == SIZE32_TYPE))
				{
					specData = 0;
					memoryCopy ( (uint8 *) tmpPtr,(uint8 *) &specData, 4 );
					tmpPtr++;
				}
			} 
			
			compareAddr = (uint32)(firstFbParaPtr + fbCodePtr->varIndex);
			if ( (uint32)tmpPtr != compareAddr ) 
			{
				status = FB_VAR_INDEX_PTR_ERR;
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"tmpPtr",(uint32)tmpPtr,
									"varIndexPtr",compareAddr,
									"fbBlockAddr",fbBlockAddr
									);
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"firstFbParaPtr",(uint32)firstFbParaPtr,
									"varIndex",(uint32)fbCodePtr->varIndex,
									"fbBlockAddr",fbBlockAddr
									);

				goto MAKE_ERR_RESP;
			}
			
			
            for(j = 0; j < fbCodePtr->intlVarSize; j++) 
            {
                memoryClear (	(uint8 *) tmpPtr, 
                    			4 
							);
                tmpPtr++;
			} 

			
            fbOutputPtr += fbCodePtr->outputIndex;
			tmpPtr2 	= fbOutputPtr;
			
            for(j = 0; j < fbCodeInfo.outputSize; j++) 
            {
                memoryClear ((uint8 *) tmpPtr2, 4);
                tmpPtr2++;
			}

			
			if(moduleInfo.moduleId == LOGIC_ID) 
			{
				paraIndexPtr->curParaIndex += specNum + fbCodePtr->inputSize + fbCodePtr->intlVarSize;
			} 
			else 
			{
				paraIndexPtr->curParaIndex += fbCodePtr->inputSize + fbCodePtr->intlVarSize;
			}

			if(moduleInfo.moduleId == LOGIC_ID)
			{
				paraIndexPtr->curOutputIndex += fbCodePtr->outputSize;
			}

			compareAddr = (uint32)(firstFbParaPtr + paraIndexPtr->curParaIndex);
			if ((uint32)tmpPtr != compareAddr) 
			{
				status = FB_PARA_INDEX_ERR;
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"tmpPtr",(uint32)tmpPtr,
									"curParaIndexPtr",compareAddr,
									"fbBlockAddr",fbBlockAddr
					  		  		);
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"firstFbParaPtr",(uint32)firstFbParaPtr,
									"curParaIndex",paraIndexPtr->curParaIndex,
									"fbBlockAddr",fbBlockAddr
					  		  		);

				goto MAKE_ERR_RESP;
			}

			if(moduleInfo.moduleId==LOGIC_ID) 
			{
				compareAddr = (uint32)(firstFbOutputPtr+ paraIndexPtr->curOutputIndex);
				if ((uint32)tmpPtr2!= compareAddr) 
				{
					status = FB_CODE_OUTPUT_INDEX_ERR;
			        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
										"tmpPtr2",(uint32)tmpPtr2,
										"curOutputIndexPtr",compareAddr,
										"fbBlockAddr",fbBlockAddr
					  		  			);
			        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
										"firstFbOutputPtr",(uint32)firstFbOutputPtr,
										"curOutputIndex",paraIndexPtr->curOutputIndex,
										"fbBlockAddr",fbBlockAddr
					  		  			);
					goto MAKE_ERR_RESP;
				}
			}
			
            
            if( paraIndexPtr->curParaIndex > (moduleInfo.maxFbParaNum + 1) )
            {
                retVal = PARA_SIZE_EXCEED_ERR;
                status = PARA_SIZE_EXCEED_ERR;
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
			                        "Cur Para Index", paraIndexPtr->curParaIndex,
			                        "MAX Para Size", moduleInfo.maxFbParaNum,
			                        "Block Addr",  fbCodePtr->fbFuncAddr
					  		  		);
				goto MAKE_ERR_RESP;
			}
			
            if( paraIndexPtr->curOutputIndex > (moduleInfo.maxFbOutputNum + 1) )
            {
                retVal = L_FB_ADDR_ERR;
                status = L_FB_ADDR_ERR;
		        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
			                        "Cur Output Index", paraIndexPtr->curOutputIndex,
			                        "MAX Output Size",  moduleInfo.maxFbOutputNum,
			                        "Not Using",  0
							  		);
				goto MAKE_ERR_RESP;
            }

            
			if(moduleInfo.moduleId == LOGIC_ID) 
			{
				segTblPtr->memInputSize += (fbCodePtr->inputNo + fbCodePtr->inputSize+ fbCodePtr->intlVarSize  );
				segTblPtr->memOutputSize += fbCodePtr->outputSize;
			} 
			else 
			{
				segTblPtr->memInputSize += (fbCodePtr->inputSize+ fbCodePtr->intlVarSize) ;
				segTblPtr->memOutputSize += fbCodePtr->outputSize;
			}
		} 		
	} 

MAKE_ERR_RESP:
	
	if(status != NO_ERROR) 
	{
	 	for(i = 0; i <  reqHeaderptr->segmentNum; i++) 
	 	{
			segData.segInfo[i].status = status & 0xffff;
		}				
	}
	
MODULE_END:

 	for(i = 0; i <  reqHeaderptr->segmentNum; i++) 
 	{
 		*respDataPtr++ 	= segData.segInfo[i].taskId;
		*respDataPtr++ 	= segData.segInfo[i].status ;
	    respDataLen 	+= DOUBLE_SIZE*2;	    
 	}
 	
 	respMsgPtr->dataLen = respDataLen;

    return(status);    
}


#define MAX_CODE_STATUS_NUM 128 

uint32 fbCodeStatusReadReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR;
    uint32 *reqDataPtr = (uint32 *) &reqMsgPtr->dataBuff[4];
    uint32 *respDataPtr = (uint32 *) &respMsgPtr->dataBuf[8];
	uint32  cmdCode, respDataLen, FBRunErrNum = 0, fbBlockAddr;
    strModuleExecInfo sysExeInfo;
	strFbMemInfo fbMemInfo;

	cmdCode = *reqDataPtr++;
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0] , (uint8*)&cmdCode,DOUBLE_SIZE);
	respDataLen = DOUBLE_SIZE;

	
	if( (status = fbSysExeModuleInfoRead(LOGIC_ID, &sysExeInfo)) == NO_ERROR ) 
	{
		for(fbBlockAddr = 1; fbBlockAddr <= sysExeInfo.curfbMaxBlockAddr; fbBlockAddr++)
		{
			status = fbCodeMemInfoRead(LOGIC_ID, fbBlockAddr,&fbMemInfo); 
			
			if( (status != NO_ERROR) || (fbMemInfo.fbId == 0) )
				continue;
				
			if(fbMemInfo.fbRunStatus != NO_ERROR) 
			{
				FBRunErrNum++;
				*respDataPtr = LOGIC_ID << 16; 
				*respDataPtr++ |= (fbMemInfo.fbTaskId & 0xffff);

				*respDataPtr = fbMemInfo.fbId << 16;
				*respDataPtr++ |= (fbMemInfo.fbFuncAddr & 0xffff);

				*respDataPtr++ = fbMemInfo.fbRunStatus;
				respDataLen += DOUBLE_SIZE*3;

				if(FBRunErrNum >= MAX_CODE_STATUS_NUM) 
					break;
			}
		}
	}
	else 
	{
        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"MODULE ID",  LOGIC_ID,
							"Not Using",  0, 
							"Not Using",  0
					  		);
	}

	if(FBRunErrNum>=MAX_CODE_STATUS_NUM) 
		goto MODULE_END;
	
	memoryCopy((uint8*)&respMsgPtr->dataBuf[4] , (uint8*)&FBRunErrNum,DOUBLE_SIZE);
	
	respDataLen += DOUBLE_SIZE;
	
MODULE_END:
 	respMsgPtr->dataLen = respDataLen;

	return(status);
}


