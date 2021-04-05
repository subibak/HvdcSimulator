
#include	<stdio.h>
#include	<stdLib.h>
#include	"usrTypes.h"
#include	"sysDefs.h"
#include	"memDef.h"
#include	"fbForcing.h"
#include	"pcsDefs.h"  
#include	"usrErrCode.h"
#include	"fbMem.h"
#include	"usrDefBuf.h"
#include	"fbDef.h"
#include	"segScheduler.h"
#include	"sysDefs.h"
#include	"netProtocol.h"



#include	"protocolBuf.h"


#include	"usrQueData.h"
#include	"sfcMem.h"
#include	"sysDual.h"







#include	"sysConfig.h"


extern strFbMemInfo		*gMpuFbMemInfoLocalPtr,gMpuFbMemInfoBusPtr;

extern strFbExecDbInfo	gFbCodeSortingTableInfo[MAX_FB_CODE_ID+1];
extern strActionInfo *gSFCActionInfoLocalPtr;
extern int32	gDebugPortFd;


uint32	forcingFlagWrite(strFBforcingInfo *);
uint32	simFlagSegmentCheck(uint32,uint32,uint32);
uint32  simFlagMpuSegmentWrite(uint32 , uint32,uint32 , uint32 *);     


uint32	mpufbSimFbNumGet(uint32, uint32);
uint32	mpuSimFbStatusSet(uint32, strFbSimStatusInfo *, uint32, uint32 );
uint32	forcingFlagMemWrite(strFBforcingInfo *);

void mpuDiufbSimFbNumSetInDualInfo (void);


extern void		memoryClear(uint8 *, uint32);
extern void		*memoryAlloc(int32 );
extern void		systemInitErr(uint8 *,uint32,uint8 *,uint32);
extern uint32	fbCodeMemInfoRead(uint32, uint32, strFbMemInfo *);
extern void		setErrorCodeWithVal(uint8 *, uint32, uint8 *, uint32,
					uint8 *, uint32 ,uint8 *, uint32 ,uint8 *, uint32 );
extern uint32	getFbTaskTblInfo(uint32 , uint32 , strFbTaskTblMemInfo *);
extern uint32	memoryCopy ( uint8 *, uint8 *, uint32);
extern uint32	readRuntimeFbData(uint32, uint32, uint32, uint32 *);
extern void		setErrorCode(uint8 *, uint32, uint8 *, uint32);
extern uint32	fbSysExeModuleInfoRead( uint32, strModuleExecInfo* );
extern uint32	fbCodeMemPointerGet(uint32, uint32, strFbMemInfo **);
extern uint32   fbCodeMemPointerGet(uint32, uint32, strFbMemInfo **);
extern uint32   busFbCodeMemPointerGet(uint32, uint32, strFbMemInfo **);
extern uint32   fbMemInfoRead(uint32, uint32, strFbMemInfo *);
extern uint32   systemCnfgInfoRead(strSysConfigInfo *);
extern uint32	forcingDataWriteToRemoteShelf(uint32, uint32,uint32);
extern uint32	fbSysExeInfoPointerGet(strSysExecInfo** ,uint32 );
extern uint32	dualFlagCheckInRunShelf(void);

extern strFbExecutionInfo	*fbExecIdStartMemInfoGet(uint32 );
extern strFbExecutionInfo	*fbExecIdNextMemInfoGet(uint32,strFbExecutionInfo* );
extern strFbExecutionInfo	*trsExecIdNextMemInfoGet(uint32 , strFbExecutionInfo *);
extern strFbExecutionInfo	*trsExecIdStartMemInfoGet(uint32);
extern strTrsLinkInfo		*trsLinkNextMemInfoGet(uint32, strTrsLinkInfo *);
extern strTrsLinkInfo		*trsLinkStartMemInfoGet(uint32);
extern strFbExecutionInfo	*actExecIdNextMemInfoGet(uint32 , strFbExecutionInfo *);
extern strFbExecutionInfo	*actExecIdStartMemInfoGet(uint32);

extern strActionLinkInfo	*actLinkNextMemInfoGet(uint32 , strActionLinkInfo *);
extern strActionLinkInfo	*actLinkStartMemInfoGet(uint32);

void mpuDiufbSimFbNumSetInDualInfo (void)
{
	uint32 	status = NO_ERROR;
	uint32	count = 0;
	
	
	count = mpufbSimFbNumGet (1, SIM_FB_CHECK_ASCENDING);

	status = dualMsgWriteToMyShelf(DUAL_FORCING_MPU_CNT_ACC, count);
	if(status != NO_ERROR)
		setErrorCode( __FILE__,__LINE__, __FUNCTION__, status);
	
	return;
}


uint32 mpufbSimFbNumGet(uint32	startFbBlkAddr, uint32 accMethod)
{
	uint32	blockAddr = 0, count = 0;
	int32	fbId = 0;
	strFbMemInfo fbMemInfo;
	uint32  status = NO_ERROR;
	strModuleExecInfo sysExeInfo;
	strSysExecInfo *sysExeInfoLocalPtr;
	
	status = fbSysExeModuleInfoRead(LOGIC_ID, &sysExeInfo);
	if(status != NO_ERROR)
	{
		setErrorCode( __FILE__,__LINE__, __FUNCTION__, status);
		return(0);
	}

	status =  fbSysExeInfoPointerGet(&sysExeInfoLocalPtr, LOCAL_MEM_ACC);
	if(status != NO_ERROR)
	{
		setErrorCode( __FILE__,__LINE__, __FUNCTION__, status);
		return(0);
	}

	 
	
	if ( startFbBlkAddr == 0x0) 
	{
		status = dualMsgReadFromMyShelf(DUAL_FORCING_MPU_CNT_ACC, &count);
		if(status != NO_ERROR) 
			setErrorCode( __FILE__,__LINE__, __FUNCTION__, status);
	}
	else 
	{
		count = 0;
		if((accMethod == SIM_FB_CHECK_ASCENDING)&&(status == NO_ERROR))
		{
			for(blockAddr = startFbBlkAddr; blockAddr <= sysExeInfo.curfbMaxBlockAddr; blockAddr++)
			{
				memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
    			status = fbCodeMemInfoRead (	LOGIC_ID,
            									blockAddr,
												(strFbMemInfo *) &fbMemInfo
											);
				if(status != NO_ERROR) continue;
					
				if(	(fbMemInfo.fbId == 0)||
					(fbMemInfo.fbId > MAX_FB_CODE_ID)
				  )
					 continue;

				fbId 	= fbMemInfo.fbId;
				if(fbMemInfo.forcingFlag == FORCING_ENABLE) 
				{
						count++;
				}
			}
		}
		else if (accMethod == SIM_FB_CHECK_DESCENDING)
		{
			count = 0;
			for(blockAddr = startFbBlkAddr; blockAddr > 0; blockAddr--)
			{
				memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
    			status = fbCodeMemInfoRead (	LOGIC_ID,
            									blockAddr,
												(strFbMemInfo *) &fbMemInfo
											);
				if(status != NO_ERROR) continue;
				if(	(fbMemInfo.fbId == 0)||
					(fbMemInfo.fbId > MAX_FB_NUM)
				  )
					 continue;

				fbId 	= fbMemInfo.fbId;
				if(fbMemInfo.forcingFlag == FORCING_ENABLE)
				{
						count++;
				}
			}
		}			
		else 
		{
			setErrorCode( __FILE__,__LINE__, __FUNCTION__, L_SIM_ACC_METHOD_ERR);
			count = 0;
		}
	}
	return(count);
}   

uint32 mpuSimFbStatusSet(	uint32	numOfFbCode, strFbSimStatusInfo *simInfoPtr, 
							uint32 startBlkAddr, uint32	accMethod)
{
	uint32	blockAddr = 0, count = 0, copyIndex = 0;
	int32	fbId = 0;
	strFbSimStatusInfo	tmpSimInfo[MAX_FB_SIM_STATUS_REQ_NUM],*tmpSimInfoPtr;
	strFbMemInfo fbMemInfo;
	uint32  status = NO_ERROR;
	strModuleExecInfo sysExeInfo;

	status = fbSysExeModuleInfoRead( LOGIC_ID, &sysExeInfo);
	if(status!=NO_ERROR)
	{
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}
	count = 0;

	if((accMethod == SIM_FB_CHECK_ASCENDING) && (status == NO_ERROR))
	{
		for(blockAddr = startBlkAddr; blockAddr <= sysExeInfo.curfbMaxBlockAddr;blockAddr++)
		{
			
			memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
    		status = fbCodeMemInfoRead (	LOGIC_ID,
            								blockAddr,
											(strFbMemInfo *) &fbMemInfo
										);
			if(status != NO_ERROR) continue;

			fbId 	= fbMemInfo.fbId;
			if(fbMemInfo.forcingFlag == FORCING_ENABLE)
			{
					simInfoPtr->simFbCode		= fbId;
					simInfoPtr->simFbBlockAddr	= blockAddr;
						
					if(++count >= numOfFbCode) break;
					simInfoPtr++;
			}
		}
	}
	
	else if (accMethod == SIM_FB_CHECK_DESCENDING)
	{
		memoryClear ( (uint8 *)&tmpSimInfo, sizeof(strFbSimStatusInfo)*MAX_FB_SIM_STATUS_REQ_NUM);
		tmpSimInfoPtr = &tmpSimInfo[0];
		 
		for(blockAddr = startBlkAddr; blockAddr > 0; blockAddr--)
		{
			
			memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
    		status = fbCodeMemInfoRead (	LOGIC_ID,
            								blockAddr,
											(strFbMemInfo *) &fbMemInfo
										);
			if(status != NO_ERROR) continue;

			fbId 	= fbMemInfo.fbId;

			if(fbMemInfo.forcingFlag == FORCING_ENABLE)
			{
					tmpSimInfoPtr->simFbCode		= fbId;
					tmpSimInfoPtr->simFbBlockAddr	= blockAddr;
				
					if(++count >= numOfFbCode) break;

					tmpSimInfoPtr++;
			}
		}
		
		
		for(copyIndex = 0; copyIndex < count; copyIndex++, simInfoPtr++, tmpSimInfoPtr--)
			memoryCopy ( 	(uint8 *) simInfoPtr,
							(uint8 *) tmpSimInfoPtr,
							sizeof(strFbSimStatusInfo)
						);
	}
	else 
	{
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, L_SIM_ACC_METHOD_ERR);
		count = 0;
	}

	return(count);
}   

void mpuDiuSimStatusCheck( uint32 *mpuSimFlagPtr, uint32 *diuSimFlagPtr)
{
	uint32 	dualInfoForcingCnt;

	*mpuSimFlagPtr	= 0;
	*diuSimFlagPtr	= 0;
	
	dualInfoForcingCnt = 0; 
	(void) dualMsgReadFromMyShelf(DUAL_FORCING_MPU_CNT_ACC, &dualInfoForcingCnt);
	
	if(dualInfoForcingCnt == 0) 
		*mpuSimFlagPtr	= 0;
	else 
		*mpuSimFlagPtr	= 1;

	return;
}

                 
uint32 simFlagSegmentCheck(uint32 moduleId, uint32 taskId,uint32 blockAddr)                 
{  
	uint32	status = NO_ERROR;                                                         
 	strFbTaskTblMemInfo segInfo;   
	strFbMemInfo fbMemInfo;

	memoryClear( (uint8 *) &segInfo, sizeof(strFbTaskTblMemInfo));                        
                                                                                       
	status = getFbTaskTblInfo (moduleId, taskId, &segInfo);                                 
	if(status != NO_ERROR) 
	{                                                           
		printf( "[ERROR] - segment Table Info Read Err !!\n");          
        return(status);                                                                
    }             
   
    
    if(segInfo.taskMode == PCS_RUN) 
    {
		 memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
    	 status = fbCodeMemInfoRead (	moduleId,
            							blockAddr,
                                        (strFbMemInfo *) &fbMemInfo
									);
		 if(status==NO_ERROR) 
		 {
			 if(fbMemInfo.fbTaskId != taskId) 
				status = L_SIM_SEGID_ERR;        
		 }
	}
	else 
		status= L_PCS_MODE_ERR ;    
	
	return(status);	
}

                 
uint32 simFlagMpuSegmentWrite(uint32 moduleId, uint32 taskId,uint32 forcingFlag, uint32 *totalSimFbCodeNum)                 
{  
	uint32 status = NO_ERROR; 
	uint32 blockAddr = 0, NumberOfSetFlag = 0;                                                      
 	strFbTaskTblMemInfo segInfo;                                                      
	strFbMemInfo	fbMemInfo;	
	strFbExecutionInfo	*fbExecIdInfoPtr;
	strTrsLinkInfo		*trsLinkInfoPtr;
	strFBforcingInfo	forcingInfo;

	uint32 actId = 0;
	strActionInfo *actInfoPtr, *actStartInfoPtr;
	actStartInfoPtr = gSFCActionInfoLocalPtr;

	memoryClear( (uint8 *) &segInfo, sizeof(strFbTaskTblMemInfo));                        
                                                                                       
	status = getFbTaskTblInfo (moduleId, taskId, &segInfo);                                 
	if(status != NO_ERROR) 
	{                                                           
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
        return(status);                                                                
    }                
	
    if(segInfo.taskMode == PCS_RUN ) 
    {
		if(segInfo.logicType  == FB_LOGIC_TYPE) 
		{
			fbExecIdInfoPtr = fbExecIdStartMemInfoGet(taskId);
			while(fbExecIdInfoPtr != NULL)
			{ 
				blockAddr = fbExecIdInfoPtr->fbBlockAddr;
    			memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
    			status = fbCodeMemInfoRead (	moduleId,blockAddr,
												(strFbMemInfo *) &fbMemInfo
											);
				if(fbMemInfo.fbTaskId != taskId) continue;

   				
				if(status == NO_ERROR)
				{				
					forcingInfo.moduleId 	= moduleId;
					forcingInfo.taskId 		= taskId;
					forcingInfo.FBAddr 		= blockAddr;
					forcingInfo.FBCode 		= fbMemInfo.fbId;
					forcingInfo.forcingFlag = forcingFlag;    
					forcingInfo.pointId 	= 0;

      				
					status = forcingFlagWrite( &forcingInfo);
					if(status == NO_ERROR) 
						NumberOfSetFlag++;
						
					else
					{
						setErrorCodeWithVal (__FILE__,__LINE__, __FUNCTION__, status,
											"MODULE",forcingInfo.moduleId,
											"TASK ID",forcingInfo.taskId,
											"FB Addr",forcingInfo.FBAddr);
						setErrorCodeWithVal (__FILE__,__LINE__, __FUNCTION__, status,
											"FB Code",forcingInfo.FBCode,
											"FLAG",forcingInfo.forcingFlag,
											"POINT",forcingInfo.pointId);

					}
				}
				fbExecIdInfoPtr = fbExecIdNextMemInfoGet(taskId, fbExecIdInfoPtr);
			}
		}
		else if(segInfo.logicType  == SFC_LOGIC_TYPE) 
		{
			
			if(moduleId != LOGIC_ID)
			{
				status = L_MODULE_ID_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				return(status);                                                                

			}
			
			for(actId = 1; actId<=MAX_ACTION_NUM ; actId++) 
			{
				actInfoPtr = actStartInfoPtr + (actId-1) ;
				if(actInfoPtr->taskID != taskId) continue;

				fbExecIdInfoPtr = actExecIdStartMemInfoGet(actId);
				
				while(fbExecIdInfoPtr != NULL)
				{ 
					blockAddr = fbExecIdInfoPtr->fbBlockAddr;
    				memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
    				status = fbCodeMemInfoRead (	LOGIC_ID,
            									blockAddr,
												(strFbMemInfo *) &fbMemInfo
											);
					if(fbMemInfo.fbTaskId != taskId) 
						continue;
    			
   					
					if(status == NO_ERROR)
					{
						forcingInfo.moduleId 	= moduleId;
						forcingInfo.taskId 		= taskId;
						forcingInfo.FBAddr 		= blockAddr;
						forcingInfo.FBCode 		= fbMemInfo.fbId;
						forcingInfo.pointId 	= 0;
						forcingInfo.forcingFlag = forcingFlag;
      					
						status = forcingFlagWrite( &forcingInfo);
						
						if(status == NO_ERROR) 
							NumberOfSetFlag++;
						else 
						{
							setErrorCodeWithVal (__FILE__,__LINE__, __FUNCTION__, status,
												"MODULE",forcingInfo.moduleId,
												"TASK ID",forcingInfo.taskId,
												"FB Addr",forcingInfo.FBAddr);
							setErrorCodeWithVal (__FILE__,__LINE__, __FUNCTION__, status,
												"FB Code",forcingInfo.FBCode,
												"FLAG",forcingInfo.forcingFlag,
												"POINT",forcingInfo.pointId);
						}
					}
					fbExecIdInfoPtr = actExecIdNextMemInfoGet(actId, fbExecIdInfoPtr);
				}
			}
						
			trsLinkInfoPtr = trsLinkStartMemInfoGet(taskId);
			
			while(trsLinkInfoPtr != NULL)
			{ 
				fbExecIdInfoPtr = trsExecIdStartMemInfoGet(trsLinkInfoPtr->transitionId);
				
				while(fbExecIdInfoPtr != NULL)
				{ 
					blockAddr = fbExecIdInfoPtr->fbBlockAddr;
    				memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
    				status = fbCodeMemInfoRead (	LOGIC_ID,
            									blockAddr,
												(strFbMemInfo *) &fbMemInfo
											);
					if(fbMemInfo.fbTaskId != taskId) 
						continue;

   					
					if(status == NO_ERROR) 
					{
						forcingInfo.moduleId 	= moduleId;
						forcingInfo.taskId 		= taskId;
						forcingInfo.FBAddr 		= blockAddr;
						forcingInfo.FBCode 		= fbMemInfo.fbId;
						forcingInfo.pointId 	= 0;
						forcingInfo.forcingFlag = forcingFlag;
      					
						status = forcingFlagWrite( &forcingInfo);

						if(status == NO_ERROR) 
							NumberOfSetFlag++;

						else
						{
							setErrorCodeWithVal (__FILE__,__LINE__, __FUNCTION__, status,
												"MODULE",forcingInfo.moduleId,
												"TASK ID",forcingInfo.taskId,
												"FB Addr",forcingInfo.FBAddr);
							setErrorCodeWithVal (__FILE__,__LINE__, __FUNCTION__, status,
												"FB Code",forcingInfo.FBCode,
												"FLAG",forcingInfo.forcingFlag,
												"POINT",forcingInfo.pointId);
						}
					}
					fbExecIdInfoPtr = trsExecIdNextMemInfoGet(trsLinkInfoPtr->transitionId, fbExecIdInfoPtr);
				}

				trsLinkInfoPtr = trsLinkNextMemInfoGet(taskId,trsLinkInfoPtr);
			}
		}
		
		if( NumberOfSetFlag > 0)  
		{
			*totalSimFbCodeNum =  NumberOfSetFlag;
		}
		else 
			status = L_SIM_FBCODE_ERR ; 
	}
	else 
		status = L_PCS_MODE_ERR ;  
	
	return(status);	
}

                 
uint32 simFlagDiuSegmentWrite(uint32 moduleId, uint32 taskId,uint32 forcingFlag, uint32 *totalSimFbCodeNum)                 
{  
	uint32 status = NO_ERROR; 
	uint32 blockAddr = 0, NumberOfSetFlag = 0;                                                      
 	strFbTaskTblMemInfo segInfo;                                                      
	strFbMemInfo	fbMemInfo;	
	strFBforcingInfo	forcingInfo;

	memoryClear( (uint8 *) &segInfo, sizeof(strFbTaskTblMemInfo));                        
                                                                                       
	status = getFbTaskTblInfo (moduleId, taskId, &segInfo);                                 
	if(status != NO_ERROR) 
	{                                                           
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
        return(status);                                                                
    }                

    if(segInfo.taskMode == PCS_RUN ) 
    {
		for(blockAddr = segInfo.u1.fbStartBlockAddr; blockAddr <= segInfo.u2.fbEndBlockAddr; blockAddr++) 
		{
			status = fbCodeMemInfoRead (	moduleId,
											blockAddr,
											(strFbMemInfo *) &fbMemInfo
										);
			if(status != NO_ERROR) 
			{
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		        return(status);                                                                
			}
														
			if(fbMemInfo.forcingFlag != 0x0) 
			{				
				forcingInfo.moduleId 	= moduleId;
				forcingInfo.taskId 		= taskId;
				forcingInfo.FBAddr 		= blockAddr;
				forcingInfo.FBCode 		= fbMemInfo.fbId;
				forcingInfo.forcingFlag = forcingFlag;    
				forcingInfo.pointId 	= 0;
  				
				status = forcingFlagWrite(&forcingInfo);

				if(status == NO_ERROR) 
					NumberOfSetFlag++;
					
				else
				{
					setErrorCodeWithVal (__FILE__,__LINE__, __FUNCTION__, status,
											"MODULE",forcingInfo.moduleId,
											"TASK ID",forcingInfo.taskId,
											"FB Addr",forcingInfo.FBAddr);
					setErrorCodeWithVal (__FILE__,__LINE__, __FUNCTION__, status,
											"FB Code",forcingInfo.FBCode,
											"FLAG",forcingInfo.forcingFlag,
											"POINT",forcingInfo.pointId);

				}
			}
		}
		
		if( NumberOfSetFlag > 0)
			*totalSimFbCodeNum =  NumberOfSetFlag;
			
		else 
			status = L_SIM_FBCODE_ERR ; 
	}
	else 
		status = L_PCS_MODE_ERR ;  
	
	return(status);	
}


uint32 forcingFlagWrite(strFBforcingInfo *fbForingInfoPtr)
{
	uint32	status = NO_ERROR; 
	strFbMemInfo fbMemInfo;
	uint32  outputNum;

	memoryClear((uint8*)&fbMemInfo,sizeof(strFbMemInfo));
	status = fbMemInfoRead(fbForingInfoPtr->moduleId, fbForingInfoPtr->FBAddr, &fbMemInfo);
	if(status) 
	{
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	if( (fbMemInfo.fbFuncAddr 	!= fbForingInfoPtr->FBAddr) &&
		(fbMemInfo.fbId != fbForingInfoPtr->FBCode) &&
		(fbMemInfo.fbTaskId 	!= fbForingInfoPtr->taskId))
	{
		status = SIM_FLAG_CONFIG_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}
	
	
	if( ((fbForingInfoPtr->moduleId == LOGIC_ID) && (fbForingInfoPtr->pointId == 0)))
	{
		if(fbForingInfoPtr->pointId <= fbMemInfo.outputNo) 
		{
			status = forcingFlagMemWrite(fbForingInfoPtr);
			if(status)
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		}
		else 
		{
			status = FC_OUTPUT_NUM_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			return(status);			
		}
	}
	
	return(status);	
}


uint32 forcingFlagMemWrite(strFBforcingInfo *fbForingInfoPtr)
{
	uint32	status = NO_ERROR;  
	uint32	forcingSetChanged = 0, cmd;
	strFbMemInfo *fbMemInfoPtr, *fbBusMemInfoPtr;
	uint32  forcingPointFlag, prviousFlag;
	uint32  forcingCount = 0, forcingChgFlag = 0;

	 
	if(fbForingInfoPtr->moduleId != LOGIC_ID) 
	{
		status = L_MODULE_ID_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}
	
	else if((
				(fbForingInfoPtr->moduleId == LOGIC_ID) && (fbForingInfoPtr->pointId != 0)
			)
			|| 
			(fbForingInfoPtr->pointId > 32))
	{
		status = SIM_IO_POINT_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	

	status = fbCodeMemPointerGet(	fbForingInfoPtr->moduleId, 
									fbForingInfoPtr->FBAddr, 
									&fbMemInfoPtr
								);
	if(status) 
	{
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	status = busFbCodeMemPointerGet(fbForingInfoPtr->moduleId, 
									fbForingInfoPtr->FBAddr, 
									&fbBusMemInfoPtr
									);
	if(status) 
	{
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}
	
	if(fbMemInfoPtr->fbId != fbForingInfoPtr->FBCode) 
	{
		status = L_SIM_FBCODE_ERR ;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	if( (fbForingInfoPtr->forcingFlag !=0) && (fbForingInfoPtr->forcingFlag	!=1))
	{
		status =  L_SIM_FLAG_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	if(fbForingInfoPtr->forcingFlag == FORCING_ENABLE) 
	{
		if(fbForingInfoPtr->moduleId == LOGIC_ID) 
		{
			forcingPointFlag = 1;
		}
		else 
		{
			if(fbForingInfoPtr->pointId == 0) 
			{
				forcingPointFlag = 0xFFFFFFFF;
			} 
			else
			{
				forcingPointFlag = fbMemInfoPtr->forcingFlag | (0x1<<(fbForingInfoPtr->pointId-1));
			}
		}
	}
	else 
	{	
		if(fbForingInfoPtr->moduleId == LOGIC_ID) 
		{
			forcingPointFlag = 0;
		}
		else 
		{
			if(fbForingInfoPtr->pointId == 0) 
			{
				forcingPointFlag = 0x00000000;
			} 
			else 
			{
				forcingPointFlag = fbMemInfoPtr->forcingFlag & (~(0x1<<(fbForingInfoPtr->pointId-1)));
			}
		}
	}
	
	if(	fbMemInfoPtr->forcingFlag != forcingPointFlag)
	{
		forcingSetChanged = 1;
	}

	
	prviousFlag = fbMemInfoPtr->forcingFlag;
	fbMemInfoPtr->forcingFlag = forcingPointFlag;

	
	fbBusMemInfoPtr->forcingFlag = fbMemInfoPtr->forcingFlag;

#if NOT_USED	
	if( dualFlagCheckInRunShelf() == DUAL_CHECK_ENABLE)
	{
		forcingDataWriteToRemoteShelf(FBForingInfo.moduleId,
								  FBForingInfo.FBAddr,
								  fbMemInfoPtr->forcingFlag);
	}
#endif

	
	if(forcingSetChanged == 1) 
	{
		if(fbForingInfoPtr->moduleId == LOGIC_ID)
			cmd = DUAL_FORCING_MPU_CNT_ACC;
			
		else 
			cmd = DUAL_FORCING_DIU_CNT_ACC; 
			
		status = dualMsgReadFromMyShelf(cmd, &forcingCount);
		if(status != NO_ERROR)
		{
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			return(status);	
		}
		
		if((fbForingInfoPtr->forcingFlag == FORCING_ENABLE) && (prviousFlag==0x0)) 
		{
			forcingCount++;
			forcingChgFlag = ACTIVE_FLAG; 
		}
		
		else if( (fbForingInfoPtr->forcingFlag == FORCING_DISABLE)	&& (fbMemInfoPtr->forcingFlag == 0x0) )
		{ 	
			if(	forcingCount > 0 )
			{
				forcingCount--;
				forcingChgFlag = ACTIVE_FLAG; 
			}
		}

		if(forcingChgFlag == ACTIVE_FLAG) 
		{
			status = dualMsgWriteToMyShelf(cmd, forcingCount);
			if(status != NO_ERROR) 
			{
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				return(status);	
			}

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD) 
			if( dualFlagCheckInRunShelf() == DUAL_CHECK_ENABLE )
			{
				status = dualMsgWriteToRmtShelf(cmd, forcingCount);
				if(status != NO_ERROR) 
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			}
#endif
		}
	}
				
	return(status);	
}

