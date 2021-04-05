
#include	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"cmdList.h"
#include	"segScheduler.h"
#include	"netProtocol.h"
#include	"usrDefBuf.h"
#include	"memDef.h"
#include	"pcsDefs.h"
#include	"fbMem.h"
#include	"fbDef.h"
#include	"usrErrCode.h"


#include	"protocolBuf.h"
#include	"fbForcing.h"
#include	"sysDual.h"
#include	"usrQueData.h"

extern uint32 optionCodeCmdNullFunc(strNewNetProtocolInfo *,strBufInfo *);
uint32 forcingFlagWriteReqAndRespFunc (strNewNetProtocolInfo	*, strBufInfo *);
uint32 forcingFlagStationReqAndRespFunc (strNewNetProtocolInfo	*,strBufInfo *);
uint32 forcingStatusReadReadAndRespFunc(strNewNetProtocolInfo	*,strBufInfo *);
uint32 forcingFlagTaskReqAndRespFunc(strNewNetProtocolInfo	*,strBufInfo *);


extern uint32	fbSysExeModuleInfoRead( uint32, strModuleExecInfo* );

extern void memoryClear( uint8 *, uint32);
extern void memoryCopy(uint8 *, uint8 *, uint32);
extern void setErrorCode(uint8 *, uint32, uint8 *, uint32);
extern void setErrorCodeWithVal(uint8 *, uint32, uint8 *, uint32,
								uint8 *, uint32,uint8 *, uint32,uint8 *, uint32
								);

extern uint32 simFlagSegmentCheck(uint32 ,uint32,uint32);
extern uint32 simFlagMpuSegmentWrite(uint32,uint32,uint32,uint32*);
extern uint32 simFlagDiuSegmentWrite(uint32,uint32,uint32,uint32*);
extern uint32 forcingFlagWrite(strFBforcingInfo *);

extern uint32	moduleInfoRead(uint32 , strModuleInfo	*);
extern uint32	getFbTaskTblInfo(uint32 , uint32 , strFbTaskTblMemInfo *);
extern uint32   fbMemInfoRead(uint32, uint32, strFbMemInfo *);
extern uint32	fbCodeParaTypePointerGet(uint32	,strFbCodeTypePointerInfo	*);

extern uint32	mpufbSimFbNumGet(uint32, uint32);
extern uint32	mpuSimFbStatusSet(uint32	, strFbSimStatusInfo *, uint32, uint32 );
extern uint32	dualFlagCheckInRunShelf(void);



uint32	(*forcingCmdOPFuncList[]) 
			(strNewNetProtocolInfo	*,strBufInfo *)
= {
	optionCodeCmdNullFunc,								
 	forcingFlagWriteReqAndRespFunc,					 
 	forcingFlagStationReqAndRespFunc,				
	forcingStatusReadReadAndRespFunc,				
	optionCodeCmdNullFunc							
  };


uint32 forcingFlagWriteReqAndRespFunc 
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32	respDataLen = 0, dualRetStatus;
    uint32	*reqDataPtr, *respDataPtr;
	uint32	i, dataNum, simFbCodeNum = 0;
	uint32	taskId, numOfData, simflag;
	uint32  fbBlockAddr = 0, fbId=0;	
    strTotalSegDataInfo	segData;
    strFbMemInfo	fbMemInfo;
	strFbTaskTblMemInfo	segInfo;
	strModuleInfo		moduleInfo;
	strSysDualInfo 	sysDualCmdInfo;

	strFbDataGrHeaderInfo *reqHeaderptr;
	uint16 fbRetVal = 0;

	uint32 cmdCode, moduleId, segmemtNum;
	uint32 pointId;
	strFBforcingInfo forcingInfo;
	
	reqDataPtr = (uint32*)&reqMsgPtr->dataBuff[0];
	respDataPtr = (uint32*)&respMsgPtr->dataBuf[0];
	
	
	reqHeaderptr = (strFbDataGrHeaderInfo *)reqMsgPtr->dataBuff;

	
	cmdCode = *reqDataPtr++;
	*respDataPtr++ = cmdCode;
	respDataLen = DOUBLE_SIZE;

	
	moduleId = (*reqDataPtr>>16)&0xffff;
	segmemtNum = (*reqDataPtr++) &0xffff;

	*respDataPtr = (moduleId << 16);
	*respDataPtr++ |= segmemtNum;
	respDataLen += DOUBLE_SIZE;
	
    
    if( (segmemtNum == 0) || (segmemtNum > MAX_LOGIC_TASK_NUM) ) {
		status = L_ITEM_NO_ERR;
        setErrorCodeWithVal( 	__FILE__,__LINE__,
								"forcingFlagWriteReqAndRespFunc", status,
        						"Item Number", segmemtNum,
        						"Not Using", 0,
								"Not Using", 0 
					  		);
		goto MODULE_END;
	} 

    
	retVal = moduleInfoRead(moduleId, &moduleInfo);

    if(retVal) {
    	status = L_MODULE_ID_ERR;
		setErrorCode(	(uint8 *)__FILE__, 
						(uint32)__LINE__, 
						(uint8 *)"forcingFlagWriteReqAndRespFunc",
                   		retVal
			  		  );
		goto MODULE_END;
    }

	reqDataPtr = (uint32 *)&reqMsgPtr->dataBuff[8];

    
    memoryClear((uint8 *)&segData, sizeof(strTotalSegDataInfo));

    segData.totalSegNum = segmemtNum;

    for(i = 0; i < segmemtNum; i++) {
		segData.segInfo[i].startDataPtr = reqDataPtr;

		
		taskId = *reqDataPtr ++;	

        segData.segInfo[i].taskId 	= taskId;
		segData.segInfo[i].status 	= NO_ERROR;

        
        if(	(taskId < 1) || (taskId > moduleInfo.maxSegmentNum)) {
			segData.segInfo[i].status = L_SEG_ID_ERR & 0xff;
			retVal = L_SEG_ID_ERR;
			status = L_SEG_ID_ERR;			
			setErrorCodeWithVal(	(uint8 *)__FILE__, 
									(uint32)__LINE__, 
									(uint8 *)"forcingFlagWriteReqAndRespFunc",
                        			retVal,
                        			(uint8 *)"Recv Seg Id", taskId,
                        			(uint8 *)"Max Seg Id", moduleInfo.maxSegmentNum,
                        			(uint8 *)"Not Used", 0
			  		  			);
			
            numOfData = *reqDataPtr++;
 
			for(dataNum = 0; dataNum < numOfData; dataNum++) {
				fbBlockAddr = *reqDataPtr++;
                fbId		= *reqDataPtr++;

				pointId		= (*reqDataPtr >> 16)&0xffff;	
                simflag		= *reqDataPtr++ & 0xffff;
            }
			
            continue;
        }

        
        memoryClear ( (uint8 *) &segInfo, sizeof(strFbTaskTblMemInfo));
		retVal = getFbTaskTblInfo ( 	moduleInfo.moduleId,
           							taskId,
                                    (strFbTaskTblMemInfo *)&segInfo
								);
		if(retVal)
			setErrorCode(	
							(uint8 *)__FILE__, 
							(uint32)__LINE__, 
							(uint8 *)"forcingFlagWriteReqAndRespFunc",
             	            retVal
						);

        
		 
        numOfData = *reqDataPtr++ & 0xffff;

		forcingInfo.taskId = taskId;
        for(dataNum = 0; dataNum < numOfData; dataNum++) {
			fbBlockAddr = *reqDataPtr++;
            fbId		= *reqDataPtr++;
				
			pointId     = (*reqDataPtr >> 16)&0xffff;	
            simflag		= *reqDataPtr++ & 0xffff;

           if( 	(fbBlockAddr < 0) ||
                   	(fbBlockAddr > moduleInfo.maxFbBlockAddr)
			) {
                retVal = L_FB_CODE_ERR;
                status = L_FB_CODE_ERR;
   				segData.segInfo[i].status = L_FB_ADDR_ERR & 0xff;
				setErrorCodeWithVal	(	
								(uint8 *)__FILE__, (uint32)__LINE__, 
								(uint8 *)"forcingFlagWriteReqAndRespFunc",retVal,
                                (uint8 *)"Recv Block Addr", fbBlockAddr,
                                (uint8 *)"Start Block Addr",0,
                                (uint8 *)"End Block Addr", moduleInfo.maxFbBlockAddr
               	    		 			);
				goto CHECK_END;
            }

            
            memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
			retVal = fbMemInfoRead (	moduleInfo.moduleId,
            							fbBlockAddr,
                                        (strFbMemInfo *) &fbMemInfo
									);
			if(retVal) {
            	segData.segInfo[i].status = L_FB_CODE_ERR & 0xff;
                status = L_FB_CODE_ERR;
				setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
								(uint8 *)"forcingFlagWriteReqAndRespFunc",retVal
                	    	);
				goto CHECK_END;
			}
			
			
            if(fbId != fbMemInfo.fbId) {
               	retVal = L_FB_CODE_ERR;
                status = L_FB_CODE_ERR;
	           	segData.segInfo[i].status = L_FB_CODE_ERR & 0xff;
				setErrorCodeWithVal(	(uint8 *)__FILE__, (uint32)__LINE__, 
										(uint8 *)"forcingFlagWriteReqAndRespFunc",retVal,
                                        (uint8 *)"Recv Fb Code", fbId,
                                        (uint8 *)"Read Fb Code",fbMemInfo.fbId,
										(uint8 *)"Recv Block Addr", fbBlockAddr 
               	    		 		);
            }

			if((moduleId == LOGIC_ID) && (pointId != 0)) {
               	retVal = SIM_IO_POINT_ERR;
				status = SIM_IO_POINT_ERR;
				setErrorCodeWithVal(	(uint8 *)__FILE__, (uint32)__LINE__, 
						(uint8 *)"forcingFlagWriteReqAndRespFunc",retVal,
                        (uint8 *)"Recv Fb Code", fbId,
						(uint8 *)"Recv Block Addr", fbBlockAddr ,
                        (uint8 *)"Read Point",pointId
               	    );
			}
			
			if( (simflag  != 0) && (simflag  != 1) ) {
               	retVal = L_SIM_FLAG_ERR;
				status = L_SIM_FLAG_ERR;
				setErrorCodeWithVal(	(uint8 *)__FILE__, (uint32)__LINE__, 
						(uint8 *)"forcingFlagWriteReqAndRespFunc",retVal,
                        (uint8 *)"Recv Fb Code", fbId,
						(uint8 *)"Recv Block Addr", fbBlockAddr ,
                        (uint8 *)"Recv SimFlag",simflag
               	    );
			}
            
CHECK_END:
			continue;
        }
    }

	simFbCodeNum = 0;
	memoryClear ( (uint8 *) &sysDualCmdInfo, sizeof(strSysDualInfo));
	
    
   	respDataPtr = (uint32 *)&respMsgPtr->dataBuf[8];
	forcingInfo.moduleId =  moduleId;

    for(i = 0; i < segmemtNum; i++) {
		reqDataPtr = segData.segInfo[i].startDataPtr;

		taskId = *reqDataPtr++;	
        numOfData = *reqDataPtr++;
		
        *respDataPtr++ = taskId ;		
		*respDataPtr++ = numOfData ;
        respDataLen += DOUBLE_SIZE*2;	    

		if(segData.segInfo[i].status != NO_ERROR) continue;

		forcingInfo.taskId = taskId;
        for(dataNum = 0; dataNum < numOfData; dataNum++) {
			forcingInfo.FBAddr = *reqDataPtr++;
            forcingInfo.FBCode = *reqDataPtr++;

			forcingInfo.pointId	= (*reqDataPtr >> 16)&0xffff;	
            forcingInfo.forcingFlag	= *reqDataPtr++ & 0xffff;

            
            memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));

            
            fbMemInfo.fbRunStatus = 1;

			retVal = fbMemInfoRead (	moduleInfo.moduleId,
            							fbBlockAddr,
                                        (strFbMemInfo *) &fbMemInfo
									);
			if(retVal)
				setErrorCode(	(uint8 *)__FILE__, 
								(uint32)__LINE__, 
								(uint8 *)"simFlagWriteReqAndRespFunc",
                                retVal
                	    	);
                	    	
			
			retVal = simFlagSegmentCheck(moduleId, taskId, fbBlockAddr);
			if(retVal == NO_ERROR){
				retVal = forcingFlagWrite( &forcingInfo);
				if(retVal != NO_ERROR)
					setErrorCode(	(uint8 *)__FILE__, 
									(uint32)__LINE__, 
									(uint8 *)"simFlagWriteReqAndRespFunc",
	                                retVal
	                	    	);
				else {					
					
					sysDualCmdInfo.u.sysDualSimInfo.moduleId 					
									= forcingInfo.moduleId;
					sysDualCmdInfo.u.sysDualSimInfo.taskId 					
									= forcingInfo.taskId;
					sysDualCmdInfo.u.sysDualSimInfo.FBCode[simFbCodeNum] 		
									= forcingInfo.FBCode;
					sysDualCmdInfo.u.sysDualSimInfo.FBAddr[simFbCodeNum] 		
									= forcingInfo.FBAddr;
					sysDualCmdInfo.u.sysDualSimInfo.pointId[simFbCodeNum] 	
									= (uint32)forcingInfo.pointId;
					sysDualCmdInfo.u.sysDualSimInfo.forcingFlag[simFbCodeNum] 
									= (uint32)forcingInfo.forcingFlag;
						
										
					sysDualCmdInfo.u.sysDualSimInfo.simFbCodeNum = ++simFbCodeNum;
					
					
						
					if(simFbCodeNum > MAX_DUAL_SIM_INFO_FBCODE_NUM){
						simFbCodeNum--;
							
						setErrorCode( __FILE__,
										__LINE__,
										"simFlagWriteReqAndRespFunc",
										L_SIM_CHECK_ERR
									  );
					}				
				}
			}
				
			
			if(fbMemInfo.fbRunStatus != NO_ERROR) fbRetVal = 1;
			else fbRetVal = 0;

			*respDataPtr++ = fbBlockAddr | (fbRetVal << 31);
			*respDataPtr++ = fbId;
  			*respDataPtr++ = retVal;
			respDataLen += DOUBLE_SIZE*3;
	    }
    }

	
	if(	simFbCodeNum != 0){
		
	}

MODULE_END:

	
	respMsgPtr->dataLen = respDataLen;

    return(status);
}	


uint32 forcingFlagStationReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR,retVal = NO_ERROR;
	uint32	respDataLen = 0;
    uint32	*reqDataPtr, *respDataPtr, cmdCode;
	uint32	taskId,simflag,moduleId;
	uint32  NumberOfSetFlag = 0, totalSetFlagNum = 0;

	strFbTaskTblMemInfo segInfo;
	strModuleExecInfo sysExeInfo;

	reqDataPtr = (uint32*)&reqMsgPtr->dataBuff[0];
	respDataPtr = (uint32*)&respMsgPtr->dataBuf[0];

	cmdCode = *reqDataPtr++;
	memoryCopy((uint8*)respDataPtr,(uint8*)&cmdCode,DOUBLE_SIZE);
	respDataPtr++;
	respDataLen = DOUBLE_SIZE;

	moduleId = *reqDataPtr++;
	memoryCopy((uint8*)respDataPtr,(uint8*)&moduleId,DOUBLE_SIZE);
	respDataPtr++;
	respDataLen += DOUBLE_SIZE;

    
    simflag  = *reqDataPtr++;
    
    
	if(simflag != FORCING_DISABLE){
	  	retVal = L_SIM_FLAG_ERR;
		setErrorCodeWithVal(	__FILE__, 
								__LINE__, 
								"forcingFlagStationReqAndRespFunc",retVal,
				                "Recv moduleId", moduleId,
				                "Recv SimFlag",simflag,
								"Not Using",0
				            );
		goto MODULE_END;
	}
    
    
   	respDataPtr = (uint32 *)&respMsgPtr->dataBuf[8];
  	
   	
	if((moduleId == LOGIC_ID)||(moduleId == ALL_ID)){
		memoryClear((uint8*)&sysExeInfo, sizeof(strModuleExecInfo));
		status = fbSysExeModuleInfoRead(LOGIC_ID, &sysExeInfo);
		if(status != NO_ERROR){
			setErrorCode(__FILE__,__LINE__,"forcingFlagStationReqAndRespFunc", status);
			goto MODULE_END;
		}
		
  		for(taskId = 1; taskId <= sysExeInfo.curMaxSegmentId; taskId++) {
		    memoryClear ( (uint8 *) &segInfo, sizeof(strFbTaskTblMemInfo));
			retVal = getFbTaskTblInfo ( LOGIC_ID,taskId,
									(strFbTaskTblMemInfo *)&segInfo);
			if(retVal != NO_ERROR){ 
				setErrorCode(__FILE__,__LINE__,"forcingFlagStationReqAndRespFunc", retVal);
				goto MODULE_END;
			}
			
			if(taskId != segInfo.taskId) continue;

			retVal = simFlagMpuSegmentWrite(LOGIC_ID,taskId,simflag,&NumberOfSetFlag);   	
			if(retVal == NO_ERROR) 
				totalSetFlagNum += NumberOfSetFlag;
			else { 
				setErrorCode(__FILE__,__LINE__,"forcingFlagStationReqAndRespFunc", retVal);
				goto MODULE_END;
			}
		}
	}

	if(moduleId > LOGIC_ID) {
		retVal = L_MODULE_ID_ERR;
		setErrorCodeWithVal(	__FILE__, 
								__LINE__, 
								"forcingFlagStationReqAndRespFunc",
								retVal,
				                "Recv moduleId", moduleId,
								"Not Using",0,
								"Not Using",0
				            );
	}

MODULE_END:
   	  	
   	*respDataPtr++ 	= retVal;
   	*respDataPtr++ 	= 0; 

	
    respDataLen += DOUBLE_SIZE*2;

	
	respMsgPtr->dataLen = respDataLen;
	
    return(status);
}	


uint32 forcingStatusReadReadAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR, retVal;
	strFbSimStatusRespInfo	*respInfoPtr;
	uint32	respDataLen = 0;
	uint32 	totalSimFbCodeNum = 0;
	uint32 	replyFbCodeNum = 0;
	uint32	remainSimFbCodeNum = 0;
	int32	startBlockAddr = 0;

	strModuleInfo  moduleInfo;
	strModuleExecInfo sysExeInfo;
	
    uint32 *reqDataPtr = (uint32 *) reqMsgPtr->dataBuff;
    uint32 *respDataPtr = (uint32 *) respMsgPtr->dataBuf;
	uint32 cmdCode, memoryKind, reqFbBlockAddr, numOfFbCode;
	uint32 reqAccessMethod = SIM_FB_CHECK_ASCENDING; 

    cmdCode = *reqDataPtr++;
	*respDataPtr++ = cmdCode ;
	respDataLen = DOUBLE_SIZE;        

	memoryKind = *reqDataPtr++;
	*respDataPtr++ = memoryKind ;
	respDataLen += DOUBLE_SIZE;        


	
	if(	(memoryKind != LOGIC_ID)){
		status = L_MODULE_ID_ERR;
        retVal = COMMAND_SUPPORT_ERR;
		setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
						(uint8 *)"forcingStatusReadReadAndRespFunc ",retVal
			  		  );
		goto MODULE_END;
    }
    
    memoryClear( (uint8 *)&moduleInfo, sizeof(strModuleInfo));
	retVal = moduleInfoRead(memoryKind, &moduleInfo);

    if(retVal) {
    	status = L_MODULE_ID_ERR;
        setErrorCode( (uint8 *)__FILE__,(uint32)__LINE__,
                        (uint8 *)"pcsModeWriteReqAndRespFunc",retVal
						);
		goto MODULE_END;
    }

	reqFbBlockAddr 	= (*reqDataPtr>>16)&0xffff;
	numOfFbCode 	= *reqDataPtr++& 0xffff;
	respInfoPtr		= (strFbSimStatusRespInfo *) &respMsgPtr->dataBuf[8];
	
	
	if( (memoryKind == LOGIC_ID) && 
		(reqFbBlockAddr > MAX_MPU_BLOCK_ADDR)
	){
		status = L_SIM_FBADDR_ERR;
        retVal = L_SIM_FBADDR_ERR;
		setErrorCodeWithVal(	
						(uint8 *)__FILE__, (uint32)__LINE__, 
						(uint8 *)"forcingStatusReadReadAndRespFunc",retVal,
                        (uint8 *)"Recv Blk Addr", reqFbBlockAddr,
                        (uint8 *)"Max Blk Addr", MAX_MPU_BLOCK_ADDR,
                        (uint8 *)"Not Used", 0
			  		  );
		goto MODULE_END;
	}

	
	if(	(numOfFbCode == 0x0) ||
		(numOfFbCode > MAX_FB_SIM_STATUS_REQ_NUM)
	){
		status = L_SIM_FB_CODE_NUM_ERR;
        retVal = L_SIM_FB_CODE_NUM_ERR;
		setErrorCodeWithVal(	
						(uint8 *)__FILE__, 
						(uint32)__LINE__, 
						(uint8 *)"forcingStatusReadReadAndRespFunc",
                        retVal,
                        (uint8 *)"Recv Fb Number", numOfFbCode,
                        (uint8 *)"Max Fb Code Number", MAX_FB_SIM_STATUS_REQ_NUM,
                        (uint8 *)"Not Used", 0
			  		  );
		goto MODULE_END;		
	}
	
	status =  fbSysExeModuleInfoRead(memoryKind,&sysExeInfo);
	if(status != NO_ERROR)
	{
		setErrorCode(	
						(uint8 *)__FILE__, 
						(uint32)__LINE__, 
						(uint8 *)"forcingStatusReadReadAndRespFunc", status
			  		  );
		goto MODULE_END;		
	}
	
	if( reqAccessMethod == SIM_FB_CHECK_ASCENDING){
		startBlockAddr = reqFbBlockAddr;
	}
	else if(reqAccessMethod == SIM_FB_CHECK_DESCENDING){
		
		
		if(memoryKind == LOGIC_ID){
			if(reqFbBlockAddr == MAX_MPU_BLOCK_ADDR){
				startBlockAddr = sysExeInfo.curfbMaxBlockAddr;
			}
			else 
				startBlockAddr = reqFbBlockAddr;
		}
	}
	else{
		status = L_SIM_ACC_METHOD_ERR;
        retVal = L_SIM_ACC_METHOD_ERR;
		setErrorCodeWithVal(	
						(uint8 *)__FILE__, 
						(uint32)__LINE__, 
						(uint8 *)"forcingStatusReadReadAndRespFunc",
                        retVal,
                        (uint8 *)"Recv Blk Addr", reqAccessMethod,
                        (uint8 *)"ASCENDING Method", SIM_FB_CHECK_ASCENDING,
                        (uint8 *)"ASCENDING Method", SIM_FB_CHECK_DESCENDING
			  		  );
		goto MODULE_END;		
	}
	
	
	if(memoryKind == LOGIC_ID){
		
		
		totalSimFbCodeNum = mpufbSimFbNumGet(0, reqAccessMethod); 
		
		respInfoPtr->numOfSimFbCode = totalSimFbCodeNum;
		
		 
		respDataLen += DOUBLE_SIZE;
		
		
		if(totalSimFbCodeNum == 0x0) {
			respInfoPtr->numOfReplyFbCode 	= 0x0;
			respInfoPtr->numOfRemainFbCode	= 0x0;
			
			respDataLen += DOUBLE_SIZE;
			
			goto MODULE_END;
		}
		
		
		
		
		if(startBlockAddr == 0x1){ 
			if(totalSimFbCodeNum > MAX_FB_SIM_STATUS_REQ_NUM) {
				respInfoPtr->numOfReplyFbCode 	= (uint16)MAX_FB_SIM_STATUS_REQ_NUM;	
				respInfoPtr->numOfRemainFbCode	= (uint16)(totalSimFbCodeNum - MAX_FB_SIM_STATUS_REQ_NUM);
			}
			else {
				respInfoPtr->numOfReplyFbCode 	= (uint16)totalSimFbCodeNum;	
				respInfoPtr->numOfRemainFbCode	= 0;
			}
		
			
			respDataLen += DOUBLE_SIZE;
		}
		else { 
			remainSimFbCodeNum = mpufbSimFbNumGet(startBlockAddr,reqAccessMethod);
			if(remainSimFbCodeNum > MAX_FB_SIM_STATUS_REQ_NUM) {
				respInfoPtr->numOfReplyFbCode 	= (uint16)MAX_FB_SIM_STATUS_REQ_NUM;	
				respInfoPtr->numOfRemainFbCode	= (uint16)(remainSimFbCodeNum - MAX_FB_SIM_STATUS_REQ_NUM);
			}
			else {
				respInfoPtr->numOfReplyFbCode 	= (uint16)remainSimFbCodeNum;	
				respInfoPtr->numOfRemainFbCode	= 0;
			}

			
			respDataLen += DOUBLE_SIZE;
		}
		
		
		replyFbCodeNum = mpuSimFbStatusSet(	respInfoPtr->numOfReplyFbCode,
											&respInfoPtr->fbSimInfo[0],
											startBlockAddr,
											reqAccessMethod
										  );
			
		if(respInfoPtr->numOfReplyFbCode != replyFbCodeNum) {
			setErrorCodeWithVal(	
							(uint8 *)__FILE__, (uint32)__LINE__, 
							(uint8 *)"forcingStatusReadReadAndRespFunc",SIM_FB_NUM_MISMATCH_ERR,
                 	       	(uint8 *)"Req FB Code Num", respInfoPtr->numOfReplyFbCode,
                  	      	(uint8 *)"Resp Fb Code Num", replyFbCodeNum,
                    	    (uint8 *)"Not Used", 0
			  		  			);
			 
			respInfoPtr->numOfReplyFbCode 	= (uint16)replyFbCodeNum;	

			if(respInfoPtr->numOfRemainFbCode != 0x0)
				respInfoPtr->numOfRemainFbCode	+= (uint16)(MAX_FB_SIM_STATUS_REQ_NUM - replyFbCodeNum);
		} 
		
		
		respDataLen +=DOUBLE_SIZE * respInfoPtr->numOfReplyFbCode;
	}
	
MODULE_END:	
	
	respMsgPtr->dataLen = respDataLen;

	return(status);
}


uint32 forcingFlagTaskReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR,retVal = NO_ERROR;
	uint32	respDataLen = 0;
    uint32	*reqDataPtr, *respDataPtr, cmdCode;
	uint32	i,moduleId, taskNum, taskID,  simflag;
	uint32  NumberOfSetFlag = 0;

	strFbTaskTblMemInfo segInfo;

	reqDataPtr = (uint32*)&reqMsgPtr->dataBuff[0];
	respDataPtr = (uint32*)&reqMsgPtr->dataBuff[0];

	cmdCode = *reqDataPtr++;
	memoryCopy((uint8*)respDataPtr,(uint8*)&cmdCode,DOUBLE_SIZE);
	respDataPtr++;
	respDataLen = DOUBLE_SIZE;

	moduleId = *reqDataPtr++;
	memoryCopy((uint8*)respDataPtr,(uint8*)&moduleId,DOUBLE_SIZE);
	respDataPtr++;
	respDataLen += DOUBLE_SIZE;

	taskNum = *reqDataPtr++;
	memoryCopy((uint8*)respDataPtr,(uint8*)&taskNum,DOUBLE_SIZE);
	respDataPtr++;
	respDataLen += DOUBLE_SIZE;

	switch(moduleId){
		case LOGIC_ID:
			
			if( (taskNum == 0) || (taskNum > MAX_LOGIC_TASK_NUM/2 ) ) {
				taskNum = 0;
				status = L_SEG_ID_ERR;
			}
		break;
		default:
			status = L_MODULE_ID_ERR;
		break;
	}
	if(status!=NO_ERROR) {
		setErrorCodeWithVal(__FILE__, __LINE__, 
			"forcingFlagStationReqAndRespFunc",retVal,
			"MODULE ID", moduleId,
			"RECV TASK NUM",taskNum,
			"Not Using",0
		);
		goto MODULE_END;
	}

    
    
   	respDataPtr = (uint32 *)&respMsgPtr->dataBuf[12];

  	for(i = 0; i < taskNum; i++) {
		
		taskID = *reqDataPtr++;
		*respDataPtr = taskID;
		respDataLen += DOUBLE_SIZE;
		switch(moduleId){
			case LOGIC_ID:
				if( (taskID == 0) || (taskID > MAX_LOGIC_TASK_NUM ) ) {
					status = L_SEG_ID_ERR;
				}
			break;
			default:
				status = L_MODULE_ID_ERR;
			break;
		}
		if(status!=NO_ERROR) {
			setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
						(uint8 *)"forcingFlagTaskReqAndRespFunc ",status);
			goto FORCING_END;
		}

		
		simflag  = *reqDataPtr++;
		if( (simflag != 0) && (simflag != 1) ) {
	  		retVal = L_SIM_FLAG_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, 
				"forcingFlagStationReqAndRespFunc",retVal,
				"MODULE ID", moduleId,
				"RECV TASK ID",taskID,
				"FORCING FLAG",simflag
			);
			goto FORCING_END;
		}

		memoryClear ( (uint8 *) &segInfo, sizeof(strFbTaskTblMemInfo));
		retVal = getFbTaskTblInfo ( moduleId, taskID,
								(strFbTaskTblMemInfo *)&segInfo);

		if((retVal != NO_ERROR) || (taskID != segInfo.taskId)){
			retVal = L_SEG_ID_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, 
					"forcingFlagStationReqAndRespFunc",retVal,
					"MODULE ID", moduleId,
					"RECV TASK ID",taskID,
					"SET TASK ID",segInfo.taskId
				);
			goto FORCING_END;
		}
		else {
			retVal = simFlagMpuSegmentWrite(moduleId,taskID,simflag,&NumberOfSetFlag);   	
			if(retVal != NO_ERROR){
				setErrorCodeWithVal(__FILE__, __LINE__, 
						"forcingFlagStationReqAndRespFunc",retVal,
						"MODULE ID", moduleId,
						"RECV TASK ID",taskID,
						"FORCONG NUM",NumberOfSetFlag
					);
			}
		}
FORCING_END:
	   	*respDataPtr++ 	= retVal;
   		*respDataPtr++ 	= NumberOfSetFlag;
		respDataLen += DOUBLE_SIZE*2;
	}

MODULE_END:

	
	respMsgPtr->dataLen = respDataLen;

    return(status);
}	


