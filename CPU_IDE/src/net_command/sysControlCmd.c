
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

#include	"systemDiag.h"
#include	"sysDual.h"
#include	"fbMem.h"
#include    "sysConfig.h"

#include	"usrQueData.h"
#include	"sfcMem.h"
#include	"fbForcing.h"


strProgDwlParaInfo	gMpuProgDwlPara;
strProgDwlParaInfo	gDiuProgDwlPara;
strProgDwlParaInfo	gMpuProgDwlBusPara;
strProgDwlParaInfo	gDiuProgDwlBusPara;

extern strSysRunningConfigInfo		*gSysRunningInfoPtr;

extern uint32	gMpuTaskSchdCount[MAX_LOGIC_TASK_NUM];
extern uint32	gSystemInitFlag[MAX_LOGIC_TASK_NUM];	

extern strSystemRunningStatusInfo	gSysRunningStatusInfo;

#ifdef _WIN32
extern char*  SYS_BUS_ADDR;
extern int32  gShmSize;
#endif


uint32	pcsModeChange(uint32,uint32,uint32, uint32, uint32	*);
uint32	segmentModeChange ( uint32 , uint32 ,uint32 ); 
uint32	pcsInitModeSet(uint32,uint32,uint32, strFbTaskTblMemInfo *);
uint32	pcsDowndoneModeSet(uint32,uint32,strFbTaskTblMemInfo *);
uint32 	pcsRunModeSet(uint32,uint32, strFbTaskTblMemInfo *);
uint32	localSegmentModeChange ( uint32 , uint32 ,uint32 );
uint32	busSegmentModeChange ( uint32 , uint32 ,uint32 );

static uint32 	localPcsRunModeSet(uint32,uint32, strFbTaskTblMemInfo *);
static uint32 	busPcsRunModeSet(uint32,uint32, strFbTaskTblMemInfo *);


extern uint32	systemRunningStatusChange	(uint32, uint32, uint32 *);
extern uint32   systemCnfgInfoRead( strSysConfigInfo *);
extern void		systemRasInfoSet(uint32 , uint32 );
extern void		memoryClear( uint8 *, uint32);
extern void		memoryCopy(uint8 *, uint8 *, uint32);
extern void		setErrorCode(uint8 *, uint32, uint8 *, uint32);
extern void		setErrorCodeWithVal(uint8 *, uint32, uint8 *, uint32,
									uint8 *, uint32,uint8 *, uint32,uint8 *, uint32	);
extern uint32	moduleInfoRead(uint32 , strModuleInfo *);
extern uint32	segmentStatusRead(uint32, uint32, strSegStatusInfo *);
extern uint32	getFbTaskTblInfo(uint32 , uint32 , strFbTaskTblMemInfo *);
extern uint32	fbMemoryClear(uint32, uint32, strFbTaskTblMemInfo *, uint32);
extern uint32	fbMemPointerGet(uint32 , strMemPointerInfo *);
extern void		segmentChecksumCalcu (strFbTaskTblMemInfo *, strFbMemInfo *,uint32	*, uint32);
extern uint32	segTblInfoWrite(uint32 , uint32 , strFbTaskTblMemInfo *);
extern void		defaultFbMemoryClear(uint32 );
extern void		schedulerStartFlagSet(uint32 , uint32 , uint32 );
extern uint32	segSchedulerInfoDelete(uint32, uint32, uint32);
extern uint32	segTblPointerGet(uint32 ,uint32	,strFbTaskTblMemInfo	**);
extern uint32	busSegTblPointerGet(uint32 ,uint32	,strFbTaskTblMemInfo	**);
extern void		rmtEthComSeqNumChkDBClear(uint32 );
extern uint32	rmtEthComSeqNumChkDBSet(uint32 );
extern uint32	segSchedulerInfoSet(uint32 , uint32 );
extern uint32	busFbMemPointerGet(uint32 , strMemPointerInfo *);
extern uint32	busSegTblInfoWrite(uint32 , uint32 , strFbTaskTblMemInfo *);
extern uint32	rmtComInfoQueClear(void);
extern uint32	rmtDualComInfoQueClear(void);
extern uint32	fbExecIdInfoClear(uint32 , uint32 );
extern uint32	fbExecIdInfoSet(uint32 , uint32 );
extern uint32	sfcExecIdInfoClear(uint32 , uint32 );
extern uint32	sfcExecIdInfoSet(uint32 , uint32 );
extern uint32	fbSpecUnusedMemoryInfoClear( uint32 );
extern uint32	fbOutputUnusedMemoryInfoClear( uint32 );
extern uint32	fbSpecUnusedMemoryInfoSet( uint32 );
extern uint32	fbOutputUnusedMemoryInfoSet(uint32);
extern uint32	onlineDwlTaskQueClear(void);
extern uint32	onlineDwlFbCodeSeqIdVarInit(uint32,uint32);
extern uint32	onlineDwlFbCodeSeqIdVarCompare(uint32);
extern void		sysExeInfoMemoryClear(uint32);

extern uint32	dualFlagCheckInRunShelf(void);

uint32 ModeSegmentReadReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32 ModeSegmentWriteReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32 ModeStationCtrlReadReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32 ModeStationCtrlWriteReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);

extern uint32	optionCodeCmdNullFunc(strNewNetProtocolInfo *,strBufInfo *);
extern uint32	fbDataGrHeaderCheck(strFbDataGrHeaderInfo *,strSysConfigInfo *);
extern void		fbExeSequenceQueuePtrClear(uint32);
extern uint32	varMemoryClear(void);
extern uint32	sfcMemoryClear(uint32, uint32);
extern uint32	sfcSegTblClear(uint32, uint32);



uint32	(*CtrlModeAccessCmdOPFuncList[]) 
			(strNewNetProtocolInfo	*,strBufInfo *)
= {
	optionCodeCmdNullFunc,							
	ModeSegmentReadReqAndRespFunc,					
 	ModeSegmentWriteReqAndRespFunc,					 
	ModeStationCtrlReadReqAndRespFunc,				
  	ModeStationCtrlWriteReqAndRespFunc				 
};


uint32 ModeSegmentReadReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR, retVal;
	uint32	numOfSegment, taskId = 0;
    uint32	*reqDataPtr, *respDataPtr, respDataLen = 0;
	strModuleInfo		moduleInfo;
	strSegStatusInfo	segStatusInfo;
	strTotalSegDataInfo		segData;
	uint32	segNo = 0, i;
	
	uint32  itemNum=0;
	strSysConfigInfo   sysCnfgInfo;
	strFbDataGrHeaderInfo *reqHeaderptr,*respHeaderptr;

	
    systemCnfgInfoRead( (strSysConfigInfo   *) &sysCnfgInfo);
	reqHeaderptr = (strFbDataGrHeaderInfo *)&reqMsgPtr->dataBuff[0];
	respHeaderptr = (strFbDataGrHeaderInfo *)&respMsgPtr->dataBuf[0];

	
	
    memoryCopy	(	(uint8*)&respMsgPtr->dataBuf[0],
					(uint8*)reqHeaderptr,
					DOUBLE_SIZE*3 
				);
	respDataLen	= sizeof(strFbDataGrHeaderInfo);

	
	retVal = fbDataGrHeaderCheck(reqHeaderptr, &sysCnfgInfo);
	if(retVal) {
    	status = L_MODULE_ID_ERR;
        setErrorCode( (uint8 *)__FILE__,
        				(uint32)__LINE__,
                        (uint8 *)"localFbSegTblWriteReqAndResp",
                        retVal
						);

		
		respHeaderptr->segmentNum = 0;
		goto MODULE_END;
    }

    
    memoryClear( (uint8 *)&moduleInfo, sizeof(strModuleInfo));

	retVal = moduleInfoRead(reqHeaderptr->memoryKind, &moduleInfo);
    if(retVal) {
    	status = L_MODULE_ID_ERR;
        setErrorCode( (uint8 *)__FILE__,
        				(uint32)__LINE__,
                        (uint8 *)"pcsModeReadReqAndRespFunc",
                        retVal
						);
		
		respHeaderptr->segmentNum = 0;
		goto MODULE_END;
    }

    
	
    if( (reqHeaderptr->segmentNum < 1) || 
		(reqHeaderptr->segmentNum > moduleInfo.maxSegmentNum)
	){
		status = L_ITEM_NO_ERR;
        retVal = L_ITEM_NO_ERR;
        setErrorCode( (uint8 *)__FILE__,
        				(uint32)__LINE__,
                        (uint8 *)"pcsModeReadReqAndRespFunc",
                        retVal
						);
		
		respHeaderptr->segmentNum = 0;
		goto MODULE_END;
	}

    
	reqDataPtr = (uint32 *) &reqMsgPtr->dataBuff[12];
	taskId 		= *reqDataPtr;

	
    if( (reqHeaderptr->segmentNum == 1) && (taskId == 0)) {
		numOfSegment = moduleInfo.maxSegmentNum;

		
        itemNum		= 0;
		respDataPtr = (uint32 *) &respMsgPtr->dataBuf[12];

		for(segNo = 0; segNo < numOfSegment; segNo++) {
        	taskId = segNo + 1;
        	
            memoryClear ( (uint8 *)&segStatusInfo, sizeof(strSegStatusInfo));
 
        	retVal = segmentStatusRead	(	moduleInfo.moduleId,
        								taskId,
        								(strSegStatusInfo *)&segStatusInfo
        								);
			if(retVal) {
				status = retVal;
		        setErrorCode( (uint8 *)__FILE__,
        						(uint32)__LINE__,
                		        (uint8 *)"pcsModeReadReqAndRespFunc",
                        		retVal
								);
			}
			
			
			if((segStatusInfo.taskId == 0) || (segStatusInfo.taskId != taskId)) { 
           		retVal = L_SEG_ID_ERR;
#if 0
		        setErrorCodeWithVal(	(uint8 *)__FILE__, (uint32)__LINE__,
                						(uint8 *)"pcsModeReadReqAndRespFunc", retVal,
										"Module ID", moduleInfo.moduleId,
										"Seg ID", taskId,
										"Read Seg ID", segStatusInfo.taskId
										);
#endif
			}
            	
            if(retVal == NO_ERROR) {
				*respDataPtr = taskId<<16;	
    	        *respDataPtr++ |= segStatusInfo.taskMode; 
    	        respDataLen += DOUBLE_SIZE;	
    	        itemNum += 1;	
			}
            else {
				*respDataPtr = taskId<<16  ;
				*respDataPtr++ |= PCS_NO_MODE;
    	        respDataLen += DOUBLE_SIZE;
    	        itemNum += 1;
			}
		}
  
	    
        

		
		respHeaderptr->segmentNum = itemNum;
		goto MODULE_END; 
	}

	reqDataPtr = (uint32 *)&reqMsgPtr->dataBuff[12];

	memoryClear( (uint8 *)&segData, sizeof(strTotalSegDataInfo));
   	segData.totalSegNum = reqHeaderptr->segmentNum;

	for(i = 0; i < reqHeaderptr->segmentNum; i++) {
		segData.segInfo[i].startDataPtr = reqDataPtr;
		segData.segInfo[i].status	= NO_ERROR;

        taskId 		= *reqDataPtr++;
		segData.segInfo[i].taskId	= taskId;

        
       	if( (taskId < 1) ||
           	(taskId > moduleInfo.maxSegmentNum)
		) {
           	retVal = L_SEG_ID_ERR;
           	status = L_SEG_ID_ERR;
           	segData.segInfo[i].status = L_SEG_ID_ERR;
	        setErrorCodeWithVal( 	(uint8 *)__FILE__,
    	    						(uint32)__LINE__,
        	                		(uint8 *)"pcsModeReadReqAndRespFunc",
            	            		retVal,
                                    (uint8 *)"Recv Seg Id", taskId,
                                    (uint8 *)"Max Seg Id", moduleInfo.maxSegmentNum,
                                    (uint8 *)"Not Used" , 0
								);
			
			respHeaderptr->segmentNum = 0;
			goto MODULE_END; 
		}

       	
        memoryClear ( (uint8 *)&segStatusInfo, sizeof(strSegStatusInfo));
 
       	retVal = segmentStatusRead(
									moduleInfo.moduleId,
       								taskId,
       								(strSegStatusInfo *)&segStatusInfo
   								);
 
		if(retVal != NO_ERROR)
		{
	        setErrorCode( (uint8 *)__FILE__,
       						(uint32)__LINE__,
               		        (uint8 *)"pcsModeReadReqAndRespFunc",
                       		retVal
						);
			
			respHeaderptr->segmentNum = 0;
			goto MODULE_END; 
		}
		
		
		if((segStatusInfo.taskId == 0) || (segStatusInfo.taskId != taskId)) {
           	retVal = L_SEG_ID_ERR;
           	status = L_SEG_ID_ERR;
           	segData.segInfo[i].status = L_SEG_ID_ERR;
			setErrorCode( (uint8 *)__FILE__,
        					(uint32)__LINE__,
                	        (uint8 *)"pcsModeReadReqAndRespFunc",
                       		retVal
						);
		}

        segData.segInfo[i].taskMode = segStatusInfo.taskMode;
	}

	
	respDataPtr = (uint32 *) &respMsgPtr->dataBuf[12];

	for(i = 0; i < reqHeaderptr->segmentNum; i++) {
    	reqDataPtr = segData.segInfo[i].startDataPtr;

        taskId = *reqDataPtr++;

        
        if (segData.segInfo[i].status != NO_ERROR) {
			setErrorCode( (uint8 *)__FILE__,
        					(uint32)__LINE__,
                	        (uint8 *)"pcsModeReadReqAndRespFunc",
                       		segData.segInfo[i].status
						);
			
			*respDataPtr++ = taskId <<16 | PCS_NO_MODE;
			respDataLen += DOUBLE_SIZE;
		} 
		else {
		
        
		*respDataPtr++ = taskId <<16 | (segData.segInfo[i].taskMode);
        respDataLen += DOUBLE_SIZE;
		}

 	}  

MODULE_END:

	
    respMsgPtr->dataLen = respDataLen;

    return(status);
}


uint32 ModeSegmentWriteReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR;
	uint32	retVal = NO_ERROR;
	strModuleInfo	moduleInfo;
	strTotalSegDataInfo	segData;
	uint32	respDataLen =0, i,j;
	uint32	respFlag = NO_ERROR;
	strSysRunningConfigInfo	*sysInfoPtr = gSysRunningInfoPtr;
	strFbDataGrHeaderInfo *reqHeaderptr,*respHeaderptr;
	strSysConfigInfo  sysCnfgInfo;
	uint32 taskId, taskMode = 0;
	uint32	*reqDataPtr, *respDataPtr;

	
    systemCnfgInfoRead( (strSysConfigInfo   *) &sysCnfgInfo);
	reqHeaderptr = (strFbDataGrHeaderInfo *)&reqMsgPtr->dataBuff[0];
	respHeaderptr = (strFbDataGrHeaderInfo *)&respMsgPtr->dataBuf[0];

	retVal = fbDataGrHeaderCheck(reqHeaderptr, &sysCnfgInfo);
	
	memoryCopy((uint8*)respMsgPtr->dataBuf,(uint8*)reqHeaderptr,
										sizeof(strFbDataGrHeaderInfo) );
    respDataLen = sizeof(strFbDataGrHeaderInfo);

	if(retVal) {
    	status = L_MODULE_ID_ERR;
        setErrorCode( (uint8 *)__FILE__,
        				(uint32)__LINE__,
                        (uint8 *)"localFbSegTblWriteReqAndResp",
                        retVal
						);
		
		respHeaderptr->segmentNum = 0;
		goto MODULE_END;
    }

    
    memoryClear( (uint8 *)&moduleInfo, sizeof(strModuleInfo));
	retVal = moduleInfoRead(reqHeaderptr->memoryKind, &moduleInfo);
    if(retVal) {
    	status = L_MODULE_ID_ERR;
        setErrorCode( (uint8 *)__FILE__,
        				(uint32)__LINE__,
                        (uint8 *)"pcsModeWriteReqAndRespFunc",
                        retVal
						);
		
		respHeaderptr->segmentNum = 0;
		goto MODULE_END;
    }

    
    if( (reqHeaderptr->segmentNum < 1) || 
		(reqHeaderptr->segmentNum > moduleInfo.maxSegmentNum)
	){
		status = L_ITEM_NO_ERR;
        retVal = L_ITEM_NO_ERR;
        setErrorCode( (uint8 *)__FILE__,
        				(uint32)__LINE__,
                        (uint8 *)"localFbSegTblWriteReqAndResp",
                        retVal
						);
		
		respHeaderptr->segmentNum = 0;
		goto MODULE_END;
	}

    
	if(sysInfoPtr->sysOperationMode != SYS_RAS_SYSTEM_MANUAL_MODE){

		
        status = L_PCS_MODE_ERR;
        setErrorCodeWithVal( (uint8 *)__FILE__,
        				(uint32)__LINE__,
                        (uint8 *)"pcsModeWriteReqAndRespFunc",
                        status,
                        "Cur System Mode", sysInfoPtr->sysOperationMode,
                        "Requested System Mode", SYS_RAS_SYSTEM_MANUAL_MODE,
                        "Not Using", 0
						);
		
		respHeaderptr->segmentNum = 0;
		goto MODULE_END;
	} 	 

	reqDataPtr = (uint32 *)&reqMsgPtr->dataBuff[12];

	memoryClear( (uint8 *)&segData, sizeof(strTotalSegDataInfo));

    
	taskId  	= (*reqDataPtr >>16);

    if( (reqHeaderptr->segmentNum == 1) && (taskId == 0)) 
    { 
   		segData.totalSegNum = moduleInfo.maxSegmentNum;
		segData.segInfo[0].startDataPtr = reqDataPtr;
		segData.segInfo[0].status	= NO_ERROR;

		taskMode  	= *reqDataPtr++ & 0xffff;
	}
	else 
	{ 	
   		segData.totalSegNum = reqHeaderptr->segmentNum;

		for(i = 0; i < reqHeaderptr->segmentNum; i++) {
			segData.segInfo[i].startDataPtr = reqDataPtr;
			segData.segInfo[i].status	= NO_ERROR;

			taskId  	= (*reqDataPtr >>16);
			segData.segInfo[i].taskId	= taskId;

	        
    	   	if( (taskId < 1) ||
        	   	(taskId > moduleInfo.maxSegmentNum)
			) {
           		retVal = L_SEG_ID_ERR;
           		segData.segInfo[i].status = L_SEG_ID_ERR;
	        	setErrorCodeWithVal( 	(uint8 *)__FILE__,
    	    							(uint32)__LINE__,
        	        	        		(uint8 *)"pcsModeReadReqAndRespFuncc",
            	        	    		retVal,
                            	        (uint8 *)"Recv Seg Id", taskId,
                                	    (uint8 *)"Max Seg Id", moduleInfo.maxSegmentNum,
                                    	(uint8 *)"Not Used" , 0
									);
			}

			taskMode  	= *reqDataPtr++ & 0xffff;
		}
	}

    
    respDataPtr = (uint32 *) &respMsgPtr->dataBuf[12];

    for(i = 0; i < reqHeaderptr->segmentNum; i++) 
    {
    	reqDataPtr = segData.segInfo[i].startDataPtr;
	
		taskId		= (*reqDataPtr >> 16)&0xffff;	
		taskMode  	= *reqDataPtr++ & 0xffff;

        
        if(segData.segInfo[i].status != NO_ERROR) 
        {
	        *respDataPtr++ 	= ( taskId<<16  | ( (segData.segInfo[i].status & 0xff) ) );
            respDataLen 	+= DOUBLE_SIZE;
            continue;
        }

        if( (taskId == 0) && (reqHeaderptr->segmentNum == 1) ) 
        {
			respHeaderptr->segmentNum = 0;
        	for(j = 0; j < segData.totalSegNum; j++) 
        	{
				taskId = j+1;
        		
				status = pcsModeChange(	moduleInfo.moduleId, 
										taskId, 
										taskMode,
										TOTAL_DWL, 
										&retVal
									); 
			
				if(status != NO_ERROR) {
                	respFlag = 1;

           			setErrorCode(	(uint8 *)__FILE__, 
									(uint32)__LINE__, 
									(uint8 *)"pcsModeWriteReqAndRespFunc",
               						status
		  		  		  			);

					
					respHeaderptr->segmentNum += 1;
			        *respDataPtr++ = ( (taskId<<16) | (status & 0xff) );
					respDataLen += sizeof(uint32); 
				}
			}

            
            
            if(respFlag == NO_ERROR) {
				
				respHeaderptr->segmentNum = 1;
            	*respDataPtr = 0;
                respDataLen += DOUBLE_SIZE;

				
				
				if(taskMode == PCS_RUN) {
		        	for(j = 0; j < segData.totalSegNum; j++) {
						taskId = j+1;
						schedulerStartFlagSet(moduleInfo.moduleId, taskId ,SCHEDULING_FLAG_ENABLE);
					}
				}
            }

			
            
            if(taskMode == PCS_INIT)
			{
	
				if(moduleInfo.moduleId == LOGIC_ID) {	
					
					status = varMemoryClear();
					if(status != NO_ERROR) {
           				setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__, 
										(uint8 *)"pcsModeWriteReqAndRespFunc",
               							status
		  		  		  				);
					}
					
	     			systemRasInfoSet(RAS_MPU_DUAL_COPY_STATUS, NO_ERROR);
				}
				
				
				sysExeInfoMemoryClear(moduleInfo.moduleId);

				if(moduleInfo.moduleId==LOGIC_ID)  {
					
					(void)fbExeSequenceQueuePtrClear(TOTAL_DWL);

					
					retVal = fbSpecUnusedMemoryInfoClear(LOGIC_ID);
					if(retVal != NO_ERROR){
						setErrorCode (	__FILE__,__LINE__,
											"ModeSegmentWriteReqAndRespFunc",retVal
										);
					}

					
					retVal = fbOutputUnusedMemoryInfoClear(LOGIC_ID);
					if(retVal != NO_ERROR){
						setErrorCode (	__FILE__,__LINE__,
											"ModeSegmentWriteReqAndRespFunc",retVal
										);
					}
				}
			}
			
			
			else if(taskMode == PCS_RUN)
			{
				if(moduleInfo.moduleId==LOGIC_ID)  {
					 
					retVal = fbSpecUnusedMemoryInfoSet(LOGIC_ID);
					if(retVal != NO_ERROR){
						setErrorCode (	__FILE__,__LINE__,
											"ModeSegmentWriteReqAndRespFunc",retVal
										);
					}

					retVal = fbOutputUnusedMemoryInfoSet(LOGIC_ID);
					if(retVal != NO_ERROR){
						setErrorCode (	__FILE__,__LINE__,
											"ModeSegmentWriteReqAndRespFunc",retVal
										);
					}
				}
			}

		}
        else {  
      		
			status = pcsModeChange(	moduleInfo.moduleId, 
									taskId, 
									taskMode,
									SEG_DWL,								
									&retVal
    							); 

			if(status != NO_ERROR) {
       			setErrorCodeWithVal(	(uint8 *)__FILE__, 
										(uint32)__LINE__, 
										(uint8 *)"pcsModeWriteReqAndRespFunc",
           								status,
										"seg ID", taskId,
										"REQ Mode", taskMode,
										"NOT USING", 0
	  		  		  			);
			}
			else {
				
				
				if(taskMode == PCS_RUN) {
		        	schedulerStartFlagSet(moduleInfo.moduleId, taskId ,SCHEDULING_FLAG_ENABLE);
				}
			}
			
    	    *respDataPtr++ = (taskId<< 16 | ( (status & 0xff) ) );
			respDataLen += DOUBLE_SIZE; 
		}
    }

	
	if( (segData.totalSegNum == reqHeaderptr->segmentNum) &&
		(taskMode == PCS_RUN) 
	  )
	{
		if(moduleInfo.moduleId==LOGIC_ID)  {
			 
			retVal = fbSpecUnusedMemoryInfoSet(LOGIC_ID);
			if(retVal != NO_ERROR){
				setErrorCode (	__FILE__,__LINE__,
									"ModeSegmentWriteReqAndRespFunc",retVal
								);
			}

			retVal = fbOutputUnusedMemoryInfoSet(LOGIC_ID);
			if(retVal != NO_ERROR){
				setErrorCode (	__FILE__,__LINE__,
									"ModeSegmentWriteReqAndRespFunc",retVal
								);
			}
		}
	}
	

MODULE_END:

	
	respMsgPtr->dataLen = respDataLen;

    return(status);
}


uint32 ModeStationCtrlReadReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR;
	uint32	*reqDataPtr, *respDataPtr, respDataLen;
	uint32  opCommand = 0;

	uint32	curSysOperMode;				
	uint32	curSysRunningMode;			
	uint32	curSysShelfId;				
	uint32	curSysDualStatus;			
	strSysRunningConfigInfo	*sysRunningConfigInfoPtr = gSysRunningInfoPtr;
	
	curSysOperMode			= sysRunningConfigInfoPtr->sysOperationMode;	
	curSysRunningMode		= sysRunningConfigInfoPtr->sysRunningMode;		

	curSysShelfId			= sysRunningConfigInfoPtr->sysShelfId;			
	curSysDualStatus		= sysRunningConfigInfoPtr->sysRedundancyStatus;	

    respDataLen = 0;
	respDataPtr = (uint32 *)respMsgPtr->dataBuf;
 	reqDataPtr = (uint32 *)reqMsgPtr->dataBuff;
 	opCommand = *reqDataPtr++ ; 

	curSysOperMode			= sysRunningConfigInfoPtr->sysOperationMode;
	if ( (curSysOperMode != SYS_RAS_SYSTEM_AUTO_MODE) &&
		(curSysOperMode != SYS_RAS_SYSTEM_MANUAL_MODE) )
	{
		status = SYS_OPERATION_MODE_ERR;
        setErrorCodeWithVal(	(uint8 *)__FILE__, (uint32)__LINE__,
                                    (uint8 *)"sysCtrlReqAndRespFunc",status,
                                    (uint8 *)"System Operation Mode",curSysOperMode,
                                    (uint8 *)"Not Using", 0,
                                    (uint8 *)"Not Using", 0
								);
	}
 
	 
	*respDataPtr++ = opCommand;
	*respDataPtr	= status >>16;				

	*respDataPtr 	|= (curSysRunningMode);		
	*respDataPtr	|= (curSysOperMode << 2);   
	*respDataPtr	|= (curSysDualStatus << 4); 
	*respDataPtr	|= (curSysShelfId << 6);	


		
	respDataLen = LONG_SIZE;
  	
	respMsgPtr->dataLen = respDataLen;

    return(status);
}


uint32 ModeStationCtrlWriteReqAndRespFunc
		(
			strNewNetProtocolInfo	*reqMsgPtr,
            strBufInfo			*respMsgPtr
		)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32	*reqDataPtr, *respDataPtr, respDataLen;
	uint32	reqSysOperMode;
	uint32	reqSysRunningMode;

	uint32	curSysOperMode;				
	uint32	curSysRunningMode;			
	uint32	curSysShelfId;				
	uint32	curSysDualStatus;			
	strSysRunningConfigInfo	*sysRunningConfigInfoPtr = gSysRunningInfoPtr;
	strMpuDiuRunningStatusInfo	*diuRasPtr
							= &gSysRunningStatusInfo.sysDiuRunningStatusInfo;
	uint32  opCommand = 0;
	
    respDataLen = 0;
	respDataPtr = (uint32 *)respMsgPtr->dataBuf;
 	reqDataPtr = (uint32 *)reqMsgPtr->dataBuff;
	
	opCommand = *reqDataPtr++ ; 

	
	*reqDataPtr |= *reqDataPtr & 0x0000000f;

	 
	reqSysRunningMode		= (*reqDataPtr >> 0) & 0x03;
	
    reqSysOperMode			= (*reqDataPtr >> 2) & 0x03; 	
   
	curSysOperMode			= sysRunningConfigInfoPtr->sysOperationMode;
	curSysRunningMode		= sysRunningConfigInfoPtr->sysRunningMode;

	curSysShelfId			= sysRunningConfigInfoPtr->sysShelfId;
	curSysDualStatus		= sysRunningConfigInfoPtr->sysRedundancyStatus;

	  
	if(reqMsgPtr->dataLength != 8) {
		retVal = DATA_LEN_ERR;
	    setErrorCode(	(uint8 *)__FILE__, (uint32)__LINE__,
        	        	(uint8 *)"sysCtrlReqAndRespFunc",
            	         retVal
						);
		goto MODULE_END;
	} 

	
	if( ( (curSysDualStatus == SYS_RAS_REDUNDANCY_STANDBY) && (diuRasPtr->redundancyStatus == NO_ERROR) )
													  &&
		( (reqSysOperMode == SYS_RAS_SYSTEM_AUTO_MODE) || (reqSysOperMode == SYS_RAS_SYSTEM_MANUAL_MODE) )
	){
		retVal = SYS_OPERATION_MODE_ERR;
	    setErrorCodeWithVal(	(uint8 *)__FILE__,	(uint32)__LINE__,
        	                        (uint8 *)"sysCtrlReqAndRespFunc", SYS_OPERATION_MODE_ERR,
                	                (uint8 *)"System Operation Mode",curSysOperMode,
                    	            (uint8 *)"REQ Operation Mode", reqSysOperMode,
                        	        (uint8 *)"Dual Status", SYS_RAS_REDUNDANCY_STANDBY
								);			
		goto MODULE_END;
	}

    switch(curSysOperMode) {
		case SYS_RAS_SYSTEM_AUTO_MODE: 
            
			if(reqSysOperMode == SYS_RAS_SYSTEM_AUTO_MODE) {
				goto MODULE_END;
			} 
			else if(reqSysOperMode == SYS_RAS_SYSTEM_MANUAL_MODE) {	 

				if( (retVal = systemRunningStatusChange
											( 	
										SYS_OPERATION_MODE, 	
										reqSysOperMode,
                                        (uint32 *) &curSysOperMode
											)
					) != NO_ERROR
				) {							
	        		setErrorCode(	(uint8 *)__FILE__,
    	        					(uint32)__LINE__,
        	        	            (uint8 *)"sysCtrlReqAndRespFunc",
            	        	         retVal
									);
				}
				else {

				}
			}
			else {													
	        	setErrorCodeWithVal(	(uint8 *)__FILE__,	(uint32)__LINE__,
        	                                (uint8 *)"sysCtrlReqAndRespFunc", SYS_OPERATION_MODE_ERR,
                	                        (uint8 *)"System Operation Mode",curSysOperMode,
                    	                    (uint8 *)"REQ Operation Mode", reqSysOperMode,
                        	                (uint8 *)"Not Using", 0
										);
			}

			
		break;

		case SYS_RAS_SYSTEM_MANUAL_MODE: 

        	
            
            if( (reqSysRunningMode == SYS_RAS_SYSTEM_RUN)  
            			||
				(reqSysRunningMode == SYS_RAS_SYSTEM_STOP) 
			){
				if( (retVal = systemRunningStatusChange
											( 	
										SYS_RUNNING_MODE, 	
										reqSysRunningMode,
                                        (uint32 *) &curSysRunningMode
											)
					) != NO_ERROR
				) {							
	        		setErrorCode(	(uint8 *)__FILE__,
    	        					(uint32)__LINE__,
        	        	            (uint8 *)"sysCtrlReqAndRespFunc",
            	        	         retVal
									);
				}
			} 

			else if(reqSysOperMode == SYS_RAS_SYSTEM_MANUAL_MODE) {			
				goto MODULE_END;
			 
			

			} else if(reqSysOperMode == SYS_RAS_SYSTEM_AUTO_MODE) {		 

				if( (retVal = systemRunningStatusChange
											( 	
										SYS_OPERATION_MODE, 	
										reqSysOperMode,
                                        (uint32 *) &curSysOperMode
											)
					) != NO_ERROR
				) {							
	        		setErrorCode(	(uint8 *)__FILE__,
    	        					(uint32)__LINE__,
        	        	            (uint8 *)"sysCtrlReqAndRespFunc",
            	        	         retVal
									);
				}
            } 
			else {													
	        	setErrorCodeWithVal(	(uint8 *)__FILE__,	(uint32)__LINE__,
                                        (uint8 *)"sysCtrlReqAndRespFunc",retVal,
                                        (uint8 *)"System Operation Mode",curSysOperMode,
                    	                (uint8 *)"REQ Operation Mode", reqSysOperMode,
                                        (uint8 *)"REQ Running Mode", reqSysRunningMode
									);
			}
            
		break;
		default:
        	retVal = SYS_OPERATION_MODE_ERR;
        	setErrorCodeWithVal(	(uint8 *)__FILE__,
            							(uint32)__LINE__,
                                        (uint8 *)"sysCtrlReqAndRespFunc",
                                        retVal,
                                        (uint8 *)"System Operation Mode",curSysOperMode,
                                        (uint8 *)"Not Using", 0,
                                        (uint8 *)"Not Using", 0
									);
		break;	
    }
MODULE_END:
	 
	*respDataPtr++ = opCommand;
	*respDataPtr = retVal<<16;						 

	*respDataPtr 	|= (curSysRunningMode);		
	*respDataPtr	|= (curSysOperMode << 2);   
	*respDataPtr	|= (curSysDualStatus << 4); 
	*respDataPtr	|= (curSysShelfId << 6);	

		
	respDataLen = LONG_SIZE;
  	
	respMsgPtr->dataLen = respDataLen;

    return(status);
}

uint32 pcsModeChange
					(
				uint32 	moduleId,
                uint32 	segmentId,
                uint32	ctrlSegMode,
                uint32	dwlKind,
                uint32	*retValPtr
                	)
{
	uint32	status = NO_ERROR, curSegMode = 0;
	strFbTaskTblMemInfo	segTbl;

	status = getFbTaskTblInfo(moduleId, segmentId, &segTbl);

    if(status == NO_ERROR) {
    	
		curSegMode = segTbl.taskMode;
   		switch(curSegMode) {
        	case PCS_NO_MODE:
        	default:         		
            	
                switch(ctrlSegMode) {
                    case PCS_INIT:
						
    					if(moduleId == LOGIC_ID){
                        	
    						memoryClear ( 	(uint8 *) &gMpuProgDwlPara,
        									sizeof(strProgDwlParaInfo)
										);
    						memoryClear ( 	(uint8 *) &gMpuProgDwlBusPara,
        									sizeof(strProgDwlParaInfo)
										);
						}
						else {
							
       						memoryClear ( (uint8 *) &gDiuProgDwlPara,
        									sizeof(strProgDwlParaInfo)
										);
       						memoryClear ( (uint8 *) &gDiuProgDwlBusPara,
        									sizeof(strProgDwlParaInfo)
										);
						}
                                              
                    	*retValPtr = pcsInitModeSet(moduleId, segmentId, dwlKind, &segTbl);
                        if(*retValPtr != NO_ERROR) {
			        		setErrorCode(	(uint8 *)__FILE__, 
			        						(uint32)__LINE__, 
											(uint8 *)"pcsModeChange", 
											*retValPtr
			  		  		  			  );
							status = *retValPtr;
						}
                    break;

					case PCS_STOP:
						
					break;

                    default:
                  		status		= L_PCS_MODE_ERR;
                        *retValPtr	= L_PCS_MODE_ERR;       
		        		setErrorCode(	(uint8 *)__FILE__, 
		        						(uint32)__LINE__, 
										(uint8 *)"pcsModeChange", 
										*retValPtr
		  		  		  			  );
					break;       
                }
            break;

            case PCS_INIT:
				
				switch( ctrlSegMode) {
                	case PCS_INIT:
						
						if(dwlKind == TOTAL_DWL) {
                    		*retValPtr = pcsInitModeSet(moduleId, segmentId, dwlKind, &segTbl);
							if(*retValPtr != NO_ERROR) {
			        			setErrorCode(	(uint8 *)__FILE__, 
			        							(uint32)__LINE__, 
												(uint8 *)"pcsModeChange", 
												*retValPtr
			  		  		  				  );
								status = *retValPtr;
							}
						}
					break;
					case PCS_DOWNLOAD:
						
                        *retValPtr = segmentModeChange(moduleId, segmentId,PCS_DOWNLOAD);
                        if(*retValPtr != NO_ERROR) {
			        		setErrorCode(	(uint8 *)__FILE__, 	
			        						(uint32)__LINE__, 
											(uint8 *)"pcsModeChange", 
											*retValPtr
			  		  		  			  );
							status = *retValPtr;
						}
						
						
						schedulerStartFlagSet(moduleId, segmentId, SCHEDULING_FLAG_DISABLE);

					break;
					case PCS_STOP:
						
					break;
                    default:
                  		status		= L_PCS_MODE_ERR;
                        *retValPtr	= L_PCS_MODE_ERR;       
		        		setErrorCode(	(uint8 *)__FILE__, 
		        						(uint32)__LINE__, 
										(uint8 *)"pcsModeChange", 
										*retValPtr
		  		  		  			  );
                	break;
				}
            break;
			
			case PCS_DOWNLOAD:
				
				switch( ctrlSegMode) {
                	case PCS_DOWNLOAD:
                    	
					break;
                    case PCS_INIT:
                    	*retValPtr = pcsInitModeSet(moduleId, segmentId, dwlKind, &segTbl);

                        if(*retValPtr != NO_ERROR) {
			        		setErrorCode(	(uint8 *)__FILE__, 
			        						(uint32)__LINE__, 
											(uint8 *)"pcsModeChange", 
											*retValPtr
			  		  		  			  );
							status = *retValPtr;
						}
                    break;
					case PCS_DOWNLOAD_DONE:
                    	*retValPtr = pcsDowndoneModeSet(moduleId, segmentId, &segTbl);
                        if(*retValPtr != NO_ERROR) {
			        		setErrorCode(	(uint8 *)__FILE__, 
			        						(uint32)__LINE__, 
											(uint8 *)"pcsModeChange", 
											*retValPtr
			  		  		  			  );
							status = *retValPtr;
						}
                    break;

					case PCS_ERR:
						
                        *retValPtr = segmentModeChange(moduleId, segmentId,PCS_ERR);
                        if(*retValPtr != NO_ERROR) {
			        		setErrorCode(	(uint8 *)__FILE__, 
											(uint32)__LINE__, 
											(uint8 *)"pcsModeChange",
                   							*retValPtr
			  		  		  			  );
							status = *retValPtr;
						}
					break;
                    default:
                 		status = L_PCS_MODE_ERR;
                        *retValPtr = L_PCS_MODE_ERR;       
		        		setErrorCode(	(uint8 *)__FILE__, 
		        						(uint32)__LINE__, 
										(uint8 *)"pcsModeChange", 
										*retValPtr
		  		  		  			  );
                    break;
                }
            break;

			case PCS_DOWNLOAD_DONE:
				
				switch( ctrlSegMode) {
                	case PCS_DOWNLOAD_DONE:
                    	
					break;
                    case PCS_INIT:
                    	*retValPtr = pcsInitModeSet(moduleId, segmentId, dwlKind, &segTbl);

                        if(*retValPtr != NO_ERROR) {
			        		setErrorCode(	(uint8 *)__FILE__, 
			        						(uint32)__LINE__, 
											(uint8 *)"pcsModeChange", 
											*retValPtr
			  		  		  			  );
							status = *retValPtr;
						}
                    break;

					case PCS_STOP:
						
                        *retValPtr = segmentModeChange(moduleId, segmentId,PCS_STOP);
                        if(*retValPtr != NO_ERROR) {
			        		setErrorCode(	(uint8 *)__FILE__, 
			        						(uint32)__LINE__, 
											(uint8 *)"pcsModeChange", 
											*retValPtr
			  		  		  			  );
							status = *retValPtr;
						}
                    break;

                    case PCS_RUN:
							*retValPtr = pcsRunModeSet(moduleId, segmentId, &segTbl);
							if(*retValPtr != NO_ERROR) {
			        			setErrorCode(	(uint8 *)__FILE__, 
			        							(uint32)__LINE__, 
												(uint8 *)"pcsModeChange", 
												*retValPtr
			  		  		  				  );
								status = *retValPtr;
							}
				
                    break;

					case PCS_ERR:
						
                        *retValPtr = segmentModeChange(moduleId, segmentId,PCS_ERR);
                        if(*retValPtr != NO_ERROR) {
			        		setErrorCode(	(uint8 *)__FILE__, 
			        						(uint32)__LINE__, 
											(uint8 *)"pcsModeChange", 
											*retValPtr
			  		  		  			  );
							status = *retValPtr;
						}
					break;
                    default:
                 		status = L_PCS_MODE_ERR;
                        *retValPtr = L_PCS_MODE_ERR;       
		        		setErrorCode(	(uint8 *)__FILE__, 
		        						(uint32)__LINE__, 
										(uint8 *)"pcsModeChange", 
										*retValPtr
		  		  		  			  );
                    break;
                }
            break;

            case PCS_STOP:
	           	
                switch( ctrlSegMode) {
                	case PCS_STOP:
                    	
                    break;
					case PCS_INIT:
						 
                		*retValPtr = pcsInitModeSet(moduleId, segmentId, dwlKind, &segTbl);
						if(*retValPtr != NO_ERROR) {
		        			setErrorCode(	(uint8 *)__FILE__, 
		        							(uint32)__LINE__, 
											(uint8 *)"pcsModeChange", 
											*retValPtr
		  		  		  				  );
							status = *retValPtr;
						}
                    break;
                    case PCS_RUN:
                    	*retValPtr = segmentModeChange(moduleId, segmentId,PCS_RUN);
                        if(*retValPtr != NO_ERROR) {
			        		setErrorCode(	(uint8 *)__FILE__, 	
			        						(uint32)__LINE__, 
											(uint8 *)"pcsModeChange", 
											*retValPtr
			  		  		  			  );
							status = *retValPtr;
						}
						
                    break;
				    case PCS_DEBUG:
						
                        *retValPtr = segmentModeChange(moduleId, segmentId,PCS_DEBUG);
                        if(*retValPtr != NO_ERROR) {
			        		setErrorCode(	(uint8 *)__FILE__, 
			        						(uint32)__LINE__, 
											(uint8 *)"pcsModeChange", 
											*retValPtr
			  		  		  			  );
							status = *retValPtr;
						}
                    break;
                    default:
                 		status = L_PCS_MODE_ERR;
                        *retValPtr = L_PCS_MODE_ERR;       
		        		setErrorCode(	(uint8 *)__FILE__, 
		        						(uint32)__LINE__, 
										(uint8 *)"pcsModeChange", 
										*retValPtr
		  		  		  			  );
                    break;
                }
            break;

			case PCS_RUN:
            	
                switch( ctrlSegMode) {
                	case PCS_RUN:
                    	
                    break;
                    case PCS_STOP:
						
                        *retValPtr = segmentModeChange(moduleId, segmentId,PCS_STOP);
                        if(*retValPtr != NO_ERROR) {
			        		setErrorCode(	(uint8 *)__FILE__, 
			        						(uint32)__LINE__, 
											(uint8 *)"pcsModeChange", 
											*retValPtr
			  		  		  			  );
							status = *retValPtr;
						}
                    break;
				    case PCS_DEBUG:
						
                        *retValPtr = segmentModeChange(moduleId, segmentId,PCS_DEBUG);
                        if(*retValPtr != NO_ERROR) {
			        		setErrorCode(	(uint8 *)__FILE__, 
			        						(uint32)__LINE__, 
											(uint8 *)"pcsModeChange", 
											*retValPtr
			  		  		  			  );
							status = *retValPtr;
						}
                    break;
					case PCS_ERR:
						
                        *retValPtr = segmentModeChange(moduleId, segmentId,PCS_ERR);
                        if(*retValPtr != NO_ERROR) {
			        		setErrorCode(	(uint8 *)__FILE__, 
			        						(uint32)__LINE__, 
											(uint8 *)"pcsModeChange", 
											*retValPtr
			  		  		  			  );
							status = *retValPtr;
						}
					break;
                    default:
                 		status = L_PCS_MODE_ERR;
                        *retValPtr = L_PCS_MODE_ERR;       
		        		setErrorCode(	(uint8 *)__FILE__, 
		        						(uint32)__LINE__, 
										(uint8 *)"pcsModeChange", 
										*retValPtr
		  		  		  			  );
                    break;
                }
            break;
		    case PCS_DEBUG:
          	
                switch( ctrlSegMode) {
                	case PCS_DEBUG:
                    	
                    break;
                    case PCS_STOP:
						
                        *retValPtr = segmentModeChange(moduleId, segmentId,PCS_STOP);
                        if(*retValPtr != NO_ERROR) {
			        		setErrorCode(	(uint8 *)__FILE__, 
			        						(uint32)__LINE__, 
											(uint8 *)"pcsModeChange", 
											*retValPtr
			  		  		  			  );
							status = *retValPtr;
						}
                  break;
                    case PCS_RUN:
                    	*retValPtr = segmentModeChange(moduleId, segmentId,PCS_RUN);

                        if(*retValPtr != NO_ERROR) {
			        		setErrorCode(	(uint8 *)__FILE__, 
			        						(uint32)__LINE__, 
											(uint8 *)"pcsModeChange", 
											*retValPtr
			  		  		  			  );
							status = *retValPtr;
						}
                    break;
					case PCS_ERR:
						
                        *retValPtr = segmentModeChange(moduleId, segmentId,PCS_ERR);
                        if(*retValPtr != NO_ERROR) {
			        		setErrorCode(	(uint8 *)__FILE__, 
			        						(uint32)__LINE__, 
											(uint8 *)"pcsModeChange", 
											*retValPtr
			  		  		  			  );
							status = *retValPtr;
						}
					break;
                    default:
                 		status = L_PCS_MODE_ERR;
                        *retValPtr = L_PCS_MODE_ERR;       
		        		setErrorCode(	(uint8 *)__FILE__, 
		        						(uint32)__LINE__, 
										(uint8 *)"pcsModeChange", 
										*retValPtr
		  		  		  			  );
                    break;
                }           
			break;
            case PCS_ERR:
				
				switch( ctrlSegMode) {
					case PCS_ERR:
                    	
                    break;
					case PCS_INIT:
                    	*retValPtr = pcsInitModeSet(moduleId, segmentId, dwlKind, &segTbl);

                        if(*retValPtr != NO_ERROR) {
			        		setErrorCode(	(uint8 *)__FILE__, 
			        						(uint32)__LINE__, 
											(uint8 *)"pcsModeChange", 
											*retValPtr
			  		  		  			  );
							status = *retValPtr;
						}
                    break;

                    default:
                  		status = L_PCS_MODE_ERR;
                        *retValPtr = L_PCS_MODE_ERR;       
		        		setErrorCode(	(uint8 *)__FILE__, 
		        						(uint32)__LINE__, 
										(uint8 *)"pcsModeChange", 
										*retValPtr
		  		  		  			  );
                	break;
				}
            break;
        }
    }
   
	if(status)
		setErrorCodeWithVal(	__FILE__, __LINE__,
								"pcsModeChange", status,
								"seg ID", segmentId,
								"REQ MODE", ctrlSegMode,
								"CUR MODE", curSegMode
						 	 );
    return(status);
}


uint32	segmentModeChange ( uint32 moduleId, uint32 taskId,uint32 taskMode)
{
	uint32	status = NO_ERROR;

	
	status = localSegmentModeChange(moduleId, taskId, taskMode);
    if(status != NO_ERROR){
    	setErrorCode(__FILE__, __LINE__, "localSegmentModeChange", status);
        return(status);
	}

    
	status = busSegmentModeChange(moduleId, taskId, taskMode);
    if(status != NO_ERROR){
    	setErrorCode(__FILE__, __LINE__, "busSegmentModeChange ", status);
	}

    return(status);
}


uint32	localSegmentModeChange ( uint32 moduleId, uint32 taskId,uint32 taskMode)
{
	strFbTaskTblMemInfo	*segPtr;
	uint32	status = NO_ERROR;

    
	status = segTblPointerGet(moduleId, taskId, &segPtr);
    if(status == NO_ERROR){
		segPtr->taskId	= taskId; 
    	segPtr->taskMode = taskMode;
	}
    else 
		setErrorCode(__FILE__, __LINE__, "segTblPointerGet",status);

    return(status);
}


uint32	busSegmentModeChange ( uint32 moduleId, uint32 taskId,uint32 taskMode)
{
	strFbTaskTblMemInfo	*segPtr;
	uint32	status = NO_ERROR;

    
	status = busSegTblPointerGet(moduleId, taskId, &segPtr);
    if(status == NO_ERROR){
		segPtr->taskId	= taskId; 
    	segPtr->taskMode = taskMode;
	}
    else {
		setErrorCode(__FILE__, __LINE__, "busSegTblPointerGet",status);
    }

    return(status);
}



uint32 pcsInitModeSet 
					(
				uint32 	moduleId,
                uint32	taskId,
                uint32	dwlKind,
                strFbTaskTblMemInfo *segInfoPtr
                	)
{
	uint32	status = NO_ERROR;
	uint32  forcingCnt = 0; 
	uint32  logicType = segInfoPtr->logicType;

	
	schedulerStartFlagSet(moduleId, taskId, SCHEDULING_FLAG_DISABLE); 

    fbMemoryClear(moduleId, taskId, segInfoPtr, dwlKind);

    if(moduleId == LOGIC_ID){
		
		sfcMemoryClear(taskId, logicType);    
	
		
		rmtEthComSeqNumChkDBClear(taskId);
		if(dwlKind == TOTAL_DWL) {
			rmtComInfoQueClear();
			rmtDualComInfoQueClear();
		}

		
		if(logicType== SFC_LOGIC_TYPE){
			status = sfcExecIdInfoClear(taskId, moduleId);
			if(status == NO_ERROR)
			{
				 
				status = sfcSegTblClear(taskId,dwlKind);
				if(status != NO_ERROR){
					setErrorCode  (__FILE__, __LINE__, __FUNCTION__, status);
					return(status);
				}
			}
		}
		else{
			status = fbExecIdInfoClear(taskId, moduleId);
		}
		
		if(status != NO_ERROR)
		{
			setErrorCode  (__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}

		
		if(dwlKind == TOTAL_DWL)
			status = onlineDwlTaskQueClear();

		if(status != NO_ERROR)
		{
			setErrorCode  (__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}
		
		(void)onlineDwlFbCodeSeqIdVarInit(taskId, dwlKind);
	}

    
	status = segmentModeChange ( moduleId, taskId,PCS_INIT);
	if(status != NO_ERROR) 
	{
		setErrorCode  (__FILE__, __LINE__, __FUNCTION__, status);

		return(status);
	}

	
	forcingCnt = 0 ;

	if(dwlKind != TOTAL_DWL) return(status);

	
    if(moduleId == LOGIC_ID) {
    	memoryClear ( (uint8 *) &gMpuProgDwlPara,
        				sizeof(strProgDwlParaInfo)
					);
    	memoryClear ( (uint8 *) &gMpuProgDwlBusPara,
        				sizeof(strProgDwlParaInfo)
					);
		memoryClear((uint8 *)&gMpuTaskSchdCount[0],
					MAX_LOGIC_TASK_NUM*sizeof(uint32));

		
		memoryClear((uint8 *)&gSystemInitFlag[0],
					MAX_LOGIC_TASK_NUM*sizeof(uint32));

	}
	else {
		
       	memoryClear ( (uint8 *) &gDiuProgDwlPara,
        				sizeof(strProgDwlParaInfo)
					);
       	memoryClear ( (uint8 *) &gDiuProgDwlBusPara,
        				sizeof(strProgDwlParaInfo)
					);
	}
	
	
    status = segSchedulerInfoDelete(moduleId, taskId, dwlKind);
	if(status != NO_ERROR) {
		setErrorCode  (__FILE__, __LINE__, __FUNCTION__, status);

		return(status);
	}

	return(status);
}


uint32 pcsDowndoneModeSet 
					(
				uint32 	moduleId,
                uint32	taskId,
                strFbTaskTblMemInfo *segInfoPtr
                	)
{
	uint32	status = NO_ERROR;
	
	
	if( (moduleId == LOGIC_ID) &&
		((segInfoPtr->logicCheckSumStatus != NO_ERROR) || 
	     (gSysRunningStatusInfo.sysMpuRunningStatusInfo.moduleChecksumStatus))
	){
		systemRasInfoSet(RAS_MPU_CHECKSUM_STATUS, NO_ERROR);
	}

    
    
    

    
	status = segmentModeChange (moduleId, taskId, PCS_DOWNLOAD_DONE);
    if(status != NO_ERROR) return(status);

    return(status);
}


uint32 	pcsRunModeSet
				(
			uint32 	moduleId, 
			uint32	taskId, 
			strFbTaskTblMemInfo *segInfoPtr
				)
{
	uint32	status = NO_ERROR;

    
	status = localPcsRunModeSet(moduleId, taskId, segInfoPtr);
    if(status != NO_ERROR){
    	setErrorCode(__FILE__, __LINE__, "localPcsRunModeSet", status);
        return(status);
	}

    
	status = busPcsRunModeSet(moduleId, taskId, segInfoPtr);
    if(status != NO_ERROR){
    	setErrorCode(__FILE__, __LINE__, "busPcsRunModeSet", status);
	}

#if (defined(_WIN32))
	if(		(SYS_BUS_ADDR != NULL)
		&&	(gShmSize == 0x400000)	)
	{
		FlushViewOfFile(SYS_BUS_ADDR,gShmSize);
	}
#endif 

    return(status);
}


static uint32 	localPcsRunModeSet
				(
			uint32 	moduleId, 
			uint32	taskId, 
			strFbTaskTblMemInfo *segInfoPtr
				)
{
	uint32	status = NO_ERROR;
    uint32	checkSum = 0;
	strMemPointerInfo	memInfo;
	strFbMemInfo		*fbMemStartPtr;
	strSysConfigInfo 	sysCnfgInfo;

	if(moduleId == LOGIC_ID) {
		status = rmtEthComSeqNumChkDBSet(taskId);
    	if(status != NO_ERROR) return(status);
	}
    status = fbMemPointerGet(moduleId,(strMemPointerInfo	*)&memInfo);

	if(status) return(status);

	fbMemStartPtr = (strFbMemInfo *)memInfo.fbMemPtr;
    
    segmentChecksumCalcu(segInfoPtr, fbMemStartPtr, (uint32 *)&checkSum, moduleId);

    segInfoPtr->logicCheckSumStatus 	= NO_ERROR;
    segInfoPtr->logicCheckSum			= checkSum;

    status = segTblInfoWrite( moduleId, taskId, segInfoPtr);
    
	if(status != NO_ERROR){
    	setErrorCode(__FILE__, __LINE__,"localPcsRunModeSet", status); 
 		return(status);
	}

	

	
	
    if(moduleId == LOGIC_ID){
		 
		(void)fbExeSequenceQueuePtrClear(taskId);
		
		if(segInfoPtr->logicType == SFC_LOGIC_TYPE){
			status = sfcExecIdInfoSet(taskId, moduleId);
		}
		else{
			status = fbExecIdInfoSet(taskId, moduleId);
		}

		if(status != NO_ERROR){
			setErrorCode (	__FILE__,
								__LINE__,
								"localPcsRunModeSet",
								status
							);
			return(status);
		}
		
		
		
		
		
		
		status = onlineDwlFbCodeSeqIdVarCompare(taskId);						
		if(status != NO_ERROR){
			setErrorCode (	__FILE__,
								__LINE__,
								"localPcsRunModeSet",
								status
							);
			return(status);
		}
	}
	
	if((status = segSchedulerInfoSet(moduleId, taskId)) != NO_ERROR){
    	setErrorCode(__FILE__, __LINE__,"localPcsRunModeSet", status);
    	return(status);
	}

	
	status = localSegmentModeChange ( moduleId, taskId, PCS_RUN);
	if(status != NO_ERROR){
    	setErrorCode(__FILE__, __LINE__,"localPcsRunModeSet", status);
 		return(status);
	}

	
    if(moduleId == LOGIC_ID){
    	memoryClear ( (uint8 *) &gMpuProgDwlPara,
        				sizeof(strProgDwlParaInfo)
					);
    	memoryClear ( (uint8 *) &gMpuProgDwlBusPara,
        				sizeof(strProgDwlParaInfo)
					);
	}
	else {
		
       	memoryClear ( (uint8 *) &gDiuProgDwlPara,
        				sizeof(strProgDwlParaInfo)
					);
       	memoryClear ( (uint8 *) &gDiuProgDwlBusPara,
        				sizeof(strProgDwlParaInfo)
					);
	}	 
	
    return(status);
}



static uint32 	busPcsRunModeSet
				(
			uint32 	moduleId, 
			uint32	taskId, 
			strFbTaskTblMemInfo *segInfoPtr
				)
{
	uint32	status = NO_ERROR;
    uint32	checkSum = 0;
	strMemPointerInfo	memInfo;
	strFbMemInfo		*fbMemStartPtr;

    status = busFbMemPointerGet(moduleId,(strMemPointerInfo	*)&memInfo);

	if(status) return(status);

	fbMemStartPtr = (strFbMemInfo *)memInfo.fbMemPtr;
 
    
    segmentChecksumCalcu(segInfoPtr, fbMemStartPtr, (uint32 *)&checkSum, moduleId);

    segInfoPtr->logicCheckSumStatus 	= NO_ERROR;
    segInfoPtr->logicCheckSum			= checkSum;

    status = busSegTblInfoWrite( moduleId, taskId, segInfoPtr);
    
	if(status != NO_ERROR){
    	setErrorCode(__FILE__, __LINE__,"busPcsRunModeSet", status); 
 		return(status);
	}

	
	status = busSegmentModeChange ( moduleId, taskId, PCS_RUN);
	if(status != NO_ERROR){
    	setErrorCode(__FILE__, __LINE__,"BusPcsRunModeSet", status);
 		return(status);
	}

    return(status);
}
