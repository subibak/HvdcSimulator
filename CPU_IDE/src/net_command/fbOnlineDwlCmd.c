
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

#include	"usrQueData.h"
#include	"sysDual.h"
#include	"sysConfig.h"
#include	"systemDiag.h"



extern strSysRunningConfigInfo		*gSysRunningInfoPtr;

extern strMpuDiuRunningStatusInfo 	*gDiuRasInfoPtr;
extern strFbMemInfo		*gMpuFbMemInfoLocalPtr,*gMpuFbMemInfoBusPtr;
extern uint32 (*onlineDownloadAccessFuncInComTask[])(uint32 *,uint32 *, uint32 *);
extern uint32 optionCodeCmdNullFunc(strNewNetProtocolInfo *,strBufInfo *);



uint32	fbOnlineDwlFbAddReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32	fbOnlineDwlFbDelReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32	fbOnlineDwlFbSpecChgReqAndRespFunc 	(strNewNetProtocolInfo*, strBufInfo*);
uint32	fbOnlineDwlFbSeqIdAddAndChgReqAndRespFunc (strNewNetProtocolInfo*, strBufInfo*);

uint32	fbOnlineDwlFbVarInfoChgReqAndRespFunc 	(strNewNetProtocolInfo*, strBufInfo*);
uint32	fbOnlineDwlFbTrsDataChgReqAndRespFunc 	(strNewNetProtocolInfo*, strBufInfo*);

uint32	fbOnlineDwlParaMemReallocReqAndRespFunc (strNewNetProtocolInfo*, strBufInfo*);
uint32	fbOnlineDwlUpdateOrCancelReqRespFunc (strNewNetProtocolInfo*, strBufInfo*);

static uint32	fbLogicVersionInfoWriteReqRespFunc (strNewNetProtocolInfo*, strBufInfo*);
static uint32	fbLogicVersionInfoReadReqRespFunc(strNewNetProtocolInfo*, strBufInfo*);
static uint32	fbParaMemoryStatusInfoReadReqRespFunc (strNewNetProtocolInfo*, strBufInfo*);
uint32	fbOnlineDwlFbChgReqAndRespFunc(strNewNetProtocolInfo*, strBufInfo*);
uint32	ioOnlineDwlFbSpecChgReqAndRespFunc (strNewNetProtocolInfo*, strBufInfo*);

uint32 (*fbOnlineDwnCmdOPFuncList[])(strNewNetProtocolInfo*, strBufInfo*)
={
	optionCodeCmdNullFunc,							
	fbOnlineDwlFbAddReqAndRespFunc,				
	fbOnlineDwlFbDelReqAndRespFunc,				
	fbOnlineDwlFbSpecChgReqAndRespFunc,			
	fbOnlineDwlFbSeqIdAddAndChgReqAndRespFunc,	

	fbOnlineDwlFbVarInfoChgReqAndRespFunc,		
	fbOnlineDwlFbTrsDataChgReqAndRespFunc,		

	fbOnlineDwlParaMemReallocReqAndRespFunc,	
	fbOnlineDwlUpdateOrCancelReqRespFunc,		
	fbLogicVersionInfoWriteReqRespFunc,				
	fbLogicVersionInfoReadReqRespFunc,			
	fbParaMemoryStatusInfoReadReqRespFunc,		
    fbOnlineDwlFbChgReqAndRespFunc,				
	ioOnlineDwlFbSpecChgReqAndRespFunc			
};



extern  void	memoryCopy (uint8 *, uint8 *, uint32);
extern void		memoryClear (int8 *, uint32 );
extern void		setErrorCode(uint8 *,uint32,uint8 *,uint32);
extern void		setErrorCodeWithVal(uint8 *, uint32 , uint8 *, uint32,
								uint8 *, uint32,uint8 *, uint32,uint8 *, uint32
									);

extern uint32	systemCnfgInfoRead( strSysConfigInfo *);
extern void		systemRasInfoSet(uint32 , uint32 );
extern void		logMessageSet (int8 *, uint32, int8 *, uint32,uint32 );
extern uint32	readRuntimeFbData(uint32, uint32, uint32, uint32 *, uint32 *);
extern uint32	fbMemInfoRead(uint32, uint32, strFbMemInfo *);

extern void		sysRmtIpAddrSetToSysConfigMemory(uint32, uint32 );

extern uint32	fbSysExeInfoPointerGet(strSysExecInfo** ,uint32 );
extern uint32	fbSysExeModuleInfoRead(uint32, strModuleExecInfo* );

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	extern  uint32  myRmtDataAcc(uint8 *, uint32, uint32, uint32, uint8 *, uint8 *);
#endif

extern uint32	fbUnusedSpecMemInfoRead(strFbSpecMemStatusReadRespInfo *);
extern uint32	fbUnusedOutputMemInfoRead(strFbSpecMemStatusReadRespInfo *);

extern void		onlineDwlVariableClear(void);
extern uint32	onlineDwlFbCodeSeqIdVarSet(	uint32 , uint32, uint32); 
extern uint32	onlineDwlReqDataSendToFbTask(uint32 , uint32 *, uint32 *);
extern uint32	onlineDwlFbAddVariableClear(void);
extern uint32	onlineDwlFbDelVariableClear(void);
extern uint32	onlineDwlFbSpecChgVariableClear(void);
extern uint32	onlineDwlFbSeqIdAddAndChgVariableClear(void);
extern uint32	onlineDwlVarInfoChgVariableClear(void);
extern uint32	onlineDwlTrsDataChgVariableClear(void);
extern uint32   onlineDwlFbInfoChgVariableClear(void);

extern uint32  rmtBusCopyCheck(void);
extern void mpuDiufbSimFbNumSetInDualInfo (void);


uint32	fbOnlineDwlFbAddReqAndRespFunc 	(
										strNewNetProtocolInfo	*reqMsgPtr,
            							strBufInfo			*respMsgPtr
										)
{
	uint32	status = NO_ERROR;
	uint32	respDataLen = 0, respRetLen;
	uint32	totalFrameNum = 0, curFrameNum = 0;
	uint32	onlineDwlKind = 0;
	uint32	*onlineDwlReqDataPtr 	= (uint32 *)&reqMsgPtr->dataBuff[4];
	uint32	*onlineDwlRespDataPtr 	= (uint32 *)&respMsgPtr->dataBuf[4];
	uint32  *cmdCode;
	
	cmdCode = (uint32*)&reqMsgPtr->dataBuff[0];
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0],(uint8*)cmdCode,DOUBLE_SIZE);
	respDataLen = DOUBLE_SIZE;

	totalFrameNum 	= (*onlineDwlReqDataPtr >> 16) & 0xffff;
	curFrameNum		= *onlineDwlReqDataPtr & 0xffff;

	
	if( (curFrameNum == 0) ||(curFrameNum > totalFrameNum)){
		status = ONLINE_DWL_FRAME_NUM_ERR;
        setErrorCodeWithVal( 	__FILE__,__LINE__,
                        		"fbOnlineDwlFbAddReqAndRespFunc",status,
                        		"Total Frame Num",totalFrameNum,
                        		"Current Frame Num", curFrameNum,
                        		"Not Using", 0 
					  		);
		goto MODULE_END;		
	}

	
	*onlineDwlRespDataPtr++ = *onlineDwlReqDataPtr;
	respDataLen += 4;

	
	
	
	if(curFrameNum == 1){
		status = onlineDwlFbAddVariableClear();
		if(status != NO_ERROR) {
        	setErrorCode( __FILE__,__LINE__,
              		        "fbOnlineDwlFbAddReqAndRespFunc",status
					  	);
			status = FB_ONLINE_DOWNLOAD_ERR;
			goto MODULE_END;
			
		}
	}
	
	onlineDwlKind 		= ONLINE_DWL_FBCODE_ADD;
		
	
	respRetLen	= 0;

	status = onlineDownloadAccessFuncInComTask[onlineDwlKind-1] (	onlineDwlReqDataPtr,
																	onlineDwlRespDataPtr,
																	&respRetLen
																);
	
	respDataLen += respRetLen;

	if(status != NO_ERROR)
        setErrorCode( __FILE__,
   	    				__LINE__,
       	                "fbOnlineDwlFbAddReqAndRespFunc",
           	            status
					  );
		
MODULE_END:
	respMsgPtr->dataLen = respDataLen;

	return(status);
}


uint32	fbOnlineDwlFbDelReqAndRespFunc 	(
										strNewNetProtocolInfo	*reqMsgPtr,
            							strBufInfo			*respMsgPtr
										)
{
	uint32	status = NO_ERROR;
	uint32	respDataLen = 0, respRetLen;
	uint32	totalFrameNum = 0, curFrameNum = 0;
	uint32	onlineDwlKind = 0;
	uint32	*onlineDwlReqDataPtr 	= (uint32 *)&reqMsgPtr->dataBuff[4];
	uint32	*onlineDwlRespDataPtr 	= (uint32 *)&respMsgPtr->dataBuf[4];
	
	uint32  *cmdCode;

	cmdCode = (uint32*)&reqMsgPtr->dataBuff[0];
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0],(uint8*)cmdCode,DOUBLE_SIZE);
	respDataLen = DOUBLE_SIZE;

	totalFrameNum 	= (*onlineDwlReqDataPtr >> 16) & 0xffff;
	curFrameNum		= *onlineDwlReqDataPtr & 0xffff;

	
	if( (curFrameNum == 0) ||(curFrameNum > totalFrameNum)){
		status = ONLINE_DWL_FRAME_NUM_ERR;
        setErrorCodeWithVal( 	__FILE__,__LINE__,
                        		"fbOnlineDwlFbDelReqAndRespFunc",status,
                        		"Total Frame Num",totalFrameNum,
                        		"Current Frame Num", curFrameNum,
                        		"Not Using", 0 
					  		);
		goto MODULE_END;		
	}

	
	*onlineDwlRespDataPtr++ = *onlineDwlReqDataPtr;
	respDataLen += 4;

	
	
	
	if(curFrameNum == 1){
		status = onlineDwlFbDelVariableClear();
		if(status != NO_ERROR) {
        	setErrorCode( __FILE__,__LINE__,
              		        "fbOnlineDwlFbDelReqAndRespFunc", status
					  	);
			status = FB_ONLINE_DOWNLOAD_ERR;
			goto MODULE_END;
			
		}
	}

	onlineDwlKind 		= ONLINE_DWL_FBCODE_DEL;
		
	
	respRetLen	= 0;

	status = onlineDownloadAccessFuncInComTask[onlineDwlKind-1] (	onlineDwlReqDataPtr,
																	onlineDwlRespDataPtr,
																	&respRetLen
																);
	
	respDataLen += respRetLen;

	if(status != NO_ERROR) {
        setErrorCode( __FILE__,
   	    				__LINE__,
       	                "fbOnlineDwlFbDelReqAndRespFunc",
           	            status
					  );
	}

		
MODULE_END:
	respMsgPtr->dataLen = respDataLen;
	
	return(status);
}


uint32	fbOnlineDwlFbSpecChgReqAndRespFunc 	(
										strNewNetProtocolInfo	*reqMsgPtr,
            							strBufInfo			*respMsgPtr
											)
{
	uint32	status = NO_ERROR;
	uint32	respDataLen = 0, respRetLen;
	uint32	totalFrameNum = 0, curFrameNum = 0;
	uint32	onlineDwlKind = 0;
	uint32	*onlineDwlReqDataPtr 	= (uint32 *)&reqMsgPtr->dataBuff[4];
	uint32	*onlineDwlRespDataPtr 	= (uint32 *)&respMsgPtr->dataBuf[4];
	
	uint32  *cmdCode;

	cmdCode = (uint32*)&reqMsgPtr->dataBuff[0];
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0],(uint8*)cmdCode,DOUBLE_SIZE);
	respDataLen = DOUBLE_SIZE;

	totalFrameNum 	= (*onlineDwlReqDataPtr >> 16) & 0xffff;
	curFrameNum		= *onlineDwlReqDataPtr & 0xffff;

	
	if( (curFrameNum == 0) ||(curFrameNum > totalFrameNum)){
		status = ONLINE_DWL_FRAME_NUM_ERR;
        setErrorCodeWithVal( 	__FILE__,
        						__LINE__,
                        		"fbOnlineDwlFbSpecChgReqAndRespFunc",
                        		status,
                        		"Total Frame Num",totalFrameNum,
                        		"Current Frame Num", curFrameNum,
                        		"Not Using", 0 
					  		);
		goto MODULE_END;		
	}

	
	*onlineDwlRespDataPtr++ = *onlineDwlReqDataPtr;
	respDataLen += 4;

	
	
	
	if(curFrameNum == 1){
		status = onlineDwlFbSpecChgVariableClear();
		if(status != NO_ERROR) {
        	setErrorCode( __FILE__,
        					__LINE__,
              		        "fbOnlineDwlFbSpecChgReqAndRespFunc",
                        	status
					  	);
			status = FB_ONLINE_DOWNLOAD_ERR;
			goto MODULE_END;
			
		}
	}

	onlineDwlKind 		= ONLINE_DWL_FBCODE_SPEC_CHANGE;
		
	
	respRetLen	= 0;

	status = onlineDownloadAccessFuncInComTask[onlineDwlKind-1] (	onlineDwlReqDataPtr,
																	onlineDwlRespDataPtr,
																	&respRetLen
																);
	
	respDataLen += respRetLen;

	if(status != NO_ERROR) {
        setErrorCode( __FILE__,
   	    				__LINE__,
       	                "fbOnlineDwlFbSpecChgReqAndRespFunc",
           	            status
					  );
	}


MODULE_END:
	respMsgPtr->dataLen = respDataLen;

	return(status);
}


uint32	fbOnlineDwlFbSeqIdAddAndChgReqAndRespFunc 	(
										strNewNetProtocolInfo	*reqMsgPtr,
            							strBufInfo			*respMsgPtr
												)
{
	uint32	status = NO_ERROR;
	uint32	respDataLen = 0, respRetLen;
	uint32	totalFrameNum = 0, curFrameNum = 0;
	uint32	onlineDwlKind = 0;
	uint32	*onlineDwlReqDataPtr 	= (uint32 *)&reqMsgPtr->dataBuff[4];
	uint32	*onlineDwlRespDataPtr 	= (uint32 *)&respMsgPtr->dataBuf[4];

	uint32  *cmdCode;

	cmdCode = (uint32*)&reqMsgPtr->dataBuff[0];
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0],(uint8*)cmdCode,DOUBLE_SIZE);
	respDataLen = DOUBLE_SIZE;

	totalFrameNum 	= (*onlineDwlReqDataPtr >> 16) & 0xffff;
	curFrameNum		= *onlineDwlReqDataPtr & 0xffff;

	
	if( (curFrameNum == 0) ||(curFrameNum > totalFrameNum)){
		status = ONLINE_DWL_FRAME_NUM_ERR;
        setErrorCodeWithVal( 	__FILE__,
        						__LINE__,
                        		"fbOnlineDwlFbSeqIdAddAndChgReqAndRespFunc",
                        		status,
                        		"Total Frame Num",totalFrameNum,
                        		"Current Frame Num", curFrameNum,
                        		"Not Using", 0 
					  		);
		goto MODULE_END;		
	}

	
	*onlineDwlRespDataPtr++ = *onlineDwlReqDataPtr;
	respDataLen += 4;

	
	
	
	if(curFrameNum == 1){
		status = onlineDwlFbSeqIdAddAndChgVariableClear();
		if(status != NO_ERROR) {
        	setErrorCode( __FILE__,
        					__LINE__,
              		        "fbOnlineDwlFbSeqIdAddAndChgReqAndRespFunc",
                        	status
					  	);
			status = FB_ONLINE_DOWNLOAD_ERR;
			goto MODULE_END;
			
		}
	}
	
	onlineDwlKind 		= ONLINE_DWL_FBCODE_SEQ_ID_ADD_AND_CHG;
		
	
	respRetLen	= 0;

	status = onlineDownloadAccessFuncInComTask[onlineDwlKind-1] (	onlineDwlReqDataPtr,
																	onlineDwlRespDataPtr,
																	&respRetLen
																);
	
	respDataLen += respRetLen;

	if(status != NO_ERROR) {
        setErrorCode( __FILE__,
   	    				__LINE__,
       	                "fbOnlineDwlFbSeqIdAddAndChgReqAndRespFunc",
           	            status
					  );
	}

	
MODULE_END:
	respMsgPtr->dataLen = respDataLen;
	
	return(status);
}


uint32	fbOnlineDwlParaMemReallocReqAndRespFunc 	(
										strNewNetProtocolInfo	*reqMsgPtr,
            							strBufInfo			*respMsgPtr
													)
{
	uint32	status = NO_ERROR, errStatus = NO_ERROR, retVal = NO_ERROR;
	uint32	respDataLen = 0, loop;
	uint32	segmentId, itemNum = 0;
	uint32	*respDataPtr 	= (uint32 *)&respMsgPtr->dataBuf[4];
	uint32	*reqDataPtr		= (uint32 *)&reqMsgPtr->dataBuff[4];
	strOnlineDwlTaskComReqInfo	taskComReqInfo;
	strOnlineDwlTaskComRespInfo	taskComRespInfo;
	strFBOnlineSpecReallocRespInfo	*specReallocRespInfoPtr;
	strSysDualInfo	dualMsgInfo;
	uint32	reallocErrStatus = NO_ERROR;
	
	uint32  *cmdCode;

	cmdCode = (uint32*)&reqMsgPtr->dataBuff[0];
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0],(uint8*)cmdCode,DOUBLE_SIZE);
	respDataLen = DOUBLE_SIZE;
	
	if((status != NO_ERROR)) goto MODULE_END;


	memoryClear ((uint8 *)&taskComReqInfo, sizeof(strOnlineDwlTaskComReqInfo));
	memoryClear ((uint8 *)&taskComRespInfo, sizeof(strOnlineDwlTaskComRespInfo));
	

	taskComReqInfo.reqDataNum = 1;
	taskComReqInfo.reqDataBuf[0] = *reqDataPtr;
	
	
    dualMsgInfo.dualCmd = CHG_AUTO_TO_MANUAL_CMD;
	
	systemRasInfoSet(	RAS_SYSTEM_MODE, 
						SYS_RAS_SYSTEM_MANUAL_MODE
					);	
	
	
	errStatus = onlineDwlReqDataSendToFbTask(	ONLINE_FB_PARA_REALLOC,
												(uint32 *)&taskComReqInfo,
												(uint32 *)&taskComRespInfo
											);	

	if(errStatus != NO_ERROR) {
		status = FB_SPEC_MEM_REALLOC_ERR;
		setErrorCode(	__FILE__, 
						__LINE__, 
						"fbOnlineDwlParaMemReallocReqAndRespFunc",
						status
					  );
	}
	if(status == NO_ERROR) {
		specReallocRespInfoPtr = (strFBOnlineSpecReallocRespInfo *) &taskComRespInfo.respDataBuf[0];

		for(loop = 0; loop < taskComRespInfo.respDataNum; loop++,specReallocRespInfoPtr++) {
			segmentId 	= specReallocRespInfoPtr->segmentId;
			errStatus		= specReallocRespInfoPtr->status;
			
			
			
			*respDataPtr++ = errStatus ;
			respDataLen += 4;
		
			itemNum += 1;
			
			if(errStatus != NO_ERROR)
				reallocErrStatus = 1;
		}		
	
		if(reallocErrStatus != NO_ERROR) goto MODULE_END;
		
		
		
		
		
    	dualMsgInfo.dualCmd = CHG_MANUAL_TO_AUTO_CMD;

	}	

MODULE_END:

	respMsgPtr->dataLen = respDataLen;
	
	return(status);
}



uint32	fbOnlineDwlUpdateOrCancelReqRespFunc (
									strNewNetProtocolInfo	*reqMsgPtr,
            						strBufInfo			*respMsgPtr
											)
{
	uint32	status = NO_ERROR, errStatus = NO_ERROR, retVal = NO_ERROR,updateErrStatus = NO_ERROR;
	uint32	respDataLen = 0,  loop;
	uint32	*reqDataPtr ;
	uint32	*respDataPtr;
	uint32	updateFlag = 0;
	strOnlineDwlTaskComRespInfo	taskComRespInfo;
	strFBOnlineLogicUpdateRespInfo	*onlineLogicUpdateRespInfoPtr;
	strSysDualInfo	dualMsgInfo;
	strSysDualInfo dualCmdInfo;
	strMpuDiuRunningStatusInfo *diuStatusInfoPtr = gDiuRasInfoPtr;

	uint32 *cmdCode, *dwlKindNum;

	cmdCode = (uint32*)&reqMsgPtr->dataBuff[0];
	reqDataPtr = (uint32 *)&reqMsgPtr->dataBuff[4];

	memoryCopy((uint8*)&respMsgPtr->dataBuf[0],(uint8*)cmdCode,DOUBLE_SIZE);
	respDataLen = DOUBLE_SIZE;

	dwlKindNum = (uint32 *)&respMsgPtr->dataBuf[4];
	*dwlKindNum = 0;
	respDataLen += DOUBLE_SIZE;

	respDataPtr= (uint32 *)&respMsgPtr->dataBuf[8];

	
	updateFlag = *reqDataPtr;
	
	if( (updateFlag != FB_ONLINE_LOGIC_UPDATE) && (updateFlag != FB_ONLINE_LOGIC_CANCEL)
		 && (updateFlag != IO_ONLINE_LOGIC_UPDATE)&&(updateFlag != IO_ONLINE_LOGIC_CANCEL)
	){
		status = FB_LOGIC_UPDATE_FLAG_ERR;
        setErrorCodeWithVal( 	__FILE__,__LINE__,"fbOnlineDwlUpdateOrCancelReqRespFunc",status,
        						"Item Number",updateFlag,
        						"Not Using",0,
        						"Not Using",0 
					  		);
		goto MODULE_END;		
	}

	if( (updateFlag == FB_ONLINE_LOGIC_UPDATE)||(updateFlag == IO_ONLINE_LOGIC_UPDATE) )  {
		
        dualMsgInfo.dualCmd = CHG_AUTO_TO_MANUAL_CMD;
        
		systemRasInfoSet(	RAS_SYSTEM_MODE, 
							SYS_RAS_SYSTEM_MANUAL_MODE
						);	

		
		memoryClear ( (uint8 *)&taskComRespInfo, sizeof(strOnlineDwlTaskComRespInfo));	
	
		if(updateFlag == FB_ONLINE_LOGIC_UPDATE)
			errStatus =  onlineDwlReqDataSendToFbTask(	ONLINE_FB_LOGIC_UPDATE , 
														NULL,
														(uint32 *)&taskComRespInfo
														);

		if(errStatus != NO_ERROR) {
			setErrorCode (__FILE__,
							__LINE__,
							"fbOnlineDwlUpdateOrCancelReqRespFunc",
							errStatus
							);
										
			if(diuStatusInfoPtr->redundancyStatus != NO_ERROR) {
				
		    	systemRasInfoSet(RAS_SYSTEM_STATUS, SYS_RAS_SYSTEM_STOP);
				
				logMessageSet(	__FILE__, 
								__LINE__,
								"fbOnlineDwlUpdateOrCancelReqRespFunc", 
								LOCAL_STOP_BY_ONLINE_DWL_UPDATE_ERR,0
				  			  );			
			}
			else {
				 
		    	systemRasInfoSet(RAS_SYSTEM_STATUS, SYS_RAS_SYSTEM_STOP);
			    systemRasInfoSet(RAS_REDUNDANCY_STATUS, SYS_RAS_REDUNDANCY_STANDBY);			    
			    
	            dualMsgInfo.dualCmd = ONLINE_DWL_REMOTE_RUN_CMD;
				
				logMessageSet(	__FILE__, 
								__LINE__,
								"fbOnlineDwlUpdateOrCancelReqRespFunc", 
								LOCAL_STANDBY_BY_ONLINE_DWL_UPDATE_ERR,0
				  			  );			
			}
			status = FB_LOGIC_UPDATE_ERR;
		}
		if(status == NO_ERROR) {
			onlineLogicUpdateRespInfoPtr = (strFBOnlineLogicUpdateRespInfo	*)&taskComRespInfo.respDataBuf[0];
			*dwlKindNum = 0;
			for(loop = 0; loop < taskComRespInfo.respDataNum; loop++,onlineLogicUpdateRespInfoPtr++){

				
				*respDataPtr = (onlineLogicUpdateRespInfoPtr->dwlKind << 16) & 0xffff0000;
				*respDataPtr++ |= (onlineLogicUpdateRespInfoPtr->status & 0xffff);
				respDataLen += DOUBLE_SIZE;
				*dwlKindNum += 1;
				
				if(onlineLogicUpdateRespInfoPtr->status != NO_ERROR)
					updateErrStatus  = 1;
			}
			
			
			if(updateErrStatus == NO_ERROR) {

				
				mpuDiufbSimFbNumSetInDualInfo ();
				
				
				
				
				
				 
				if(updateFlag == FB_ONLINE_LOGIC_UPDATE)
	       			dualMsgInfo.dualCmd = ONLINE_DWL_CHG_MANUAL_TO_AUTO_CMD;
				
				else if(updateFlag == IO_ONLINE_LOGIC_UPDATE)
					dualMsgInfo.dualCmd = ONLINE_DWL_IOCHG_MANUAL_TO_AUTO_CMD;
				else {
					status = FB_LOGIC_UPDATE_FLAG_ERR;
				    setErrorCode(	__FILE__, __LINE__,
           	      	  "fbOnlineDwlUpdateOrCancelReqRespFunc",
               	    	status
					);
				}
			}
		}
		else {
			*dwlKindNum = 1;
		}
	}
	else if( (updateFlag == FB_ONLINE_LOGIC_CANCEL) || (updateFlag == IO_ONLINE_LOGIC_CANCEL) ){
		*dwlKindNum = 1;
		

		*respDataPtr = status;
		respDataLen += DOUBLE_SIZE;
	}
MODULE_END:

	
	onlineDwlVariableClear();
		
	respMsgPtr->dataLen = respDataLen;
	
	return(status);
}


static uint32	fbLogicVersionInfoWriteReqRespFunc (
									strNewNetProtocolInfo	*reqMsgPtr,
            						strBufInfo			*respMsgPtr
											)
{
	uint32	status = NO_ERROR, retVal;
	uint32	respDataLen = 0;
	strFbLogicVersionInfoWriteReqInfo	*reqInfoPtr;

	uint32	*respDataPtr	= (uint32 *)&respMsgPtr->dataBuf[4];
	uint32 *cmdCode, *memoryKind;
	strSysExecInfo *sysExeInfoPtr;	

	cmdCode = (uint32*)&reqMsgPtr->dataBuff[0];
	memoryKind = (uint32*)&reqMsgPtr->dataBuff[4];
	
	
	if((*memoryKind != LOGIC_ID)) {
		status = L_MODULE_ID_ERR;
        setErrorCodeWithVal( 	__FILE__,__LINE__,"fbLogicVersionInfoWriteReqRespFunc",status,
        						"Module ID",*memoryKind,
        						"Not Using",0,	"Not Using",0 
					  		);
		goto MODULE_END;		
	}

	
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0],(uint8*)cmdCode,DOUBLE_SIZE);
	respDataLen = DOUBLE_SIZE;
		
	
	reqInfoPtr = (strFbLogicVersionInfoWriteReqInfo *)&reqMsgPtr->dataBuff[8];
	
	if(reqInfoPtr->fbLogicCompileTime == 0x0) {
		status = VERSION_INFO_DATA_ERR;
        setErrorCodeWithVal( 	__FILE__,__LINE__,"fbLogicVersionInfoWriteReqRespFunc",status,
        						"Compile Time",reqInfoPtr->fbLogicCompileTime,
        						"Not Using",0,"Not Using",0 
					  		);
		goto MODULE_END;
	}

	
	retVal =  fbSysExeInfoPointerGet(&sysExeInfoPtr, LOCAL_MEM_ACC);
	if(retVal == NO_ERROR){
		if(*memoryKind == LOGIC_ID)
			sysExeInfoPtr->mpuSysExeInfo.fbLogicCompileTime 
								= reqInfoPtr->fbLogicCompileTime;
		else
			sysExeInfoPtr->diuSysExeInfo.fbLogicCompileTime 
								= reqInfoPtr->fbLogicCompileTime;
	}
	
	retVal =  fbSysExeInfoPointerGet(&sysExeInfoPtr, BUS_MEM_ACC);
	if(retVal == NO_ERROR){
		if(*memoryKind == LOGIC_ID){
			
			sysExeInfoPtr->mpuSysExeInfo.fbLogicCompileTime 
						= reqInfoPtr->fbLogicCompileTime;
		}
		else
			sysExeInfoPtr->diuSysExeInfo.fbLogicCompileTime 
								= reqInfoPtr->fbLogicCompileTime;
	}

	

	if(retVal != NO_ERROR) {
        setErrorCode( __FILE__,__LINE__,
        				"fbLogicVersionInfoWriteReqRespFunc",retVal
        			  );
		status = VERSION_INFO_WRITE_ERR;
		goto MODULE_END;
	}
MODULE_END:
	respDataLen += DOUBLE_SIZE; 
	respMsgPtr->dataLen = respDataLen;
	
	*respDataPtr = status;
	
	return(status);
}


static uint32	fbLogicVersionInfoReadReqRespFunc (
									strNewNetProtocolInfo	*reqMsgPtr,
            						strBufInfo			*respMsgPtr
									)
{
	uint32	status = NO_ERROR, retVal;
	uint32	respDataLen = 0;
	strFbLogicVersionInfoReadRespInfo	*respInfoPtr;
	uint32 *cmdCode, *memoryKind;
	strModuleExecInfo mpuExeInfo;
	
	
	cmdCode = (uint32*)&reqMsgPtr->dataBuff[0];
	memoryKind = (uint32*)&reqMsgPtr->dataBuff[4];
	
	
	if((*memoryKind != LOGIC_ID)) {
		status = L_MODULE_ID_ERR;
        setErrorCodeWithVal( 	__FILE__,__LINE__,"fbLogicVersionInfoWriteReqRespFunc",status,
        						"Module ID",*memoryKind,
        						"Not Using",0,	"Not Using",0 
					  		);
		goto MODULE_END;		
	}
	
	
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0],(uint8*)cmdCode,DOUBLE_SIZE);
	respDataLen = DOUBLE_SIZE;
	
		
	respInfoPtr = (strFbLogicVersionInfoReadRespInfo *)&respMsgPtr->dataBuf[4];
	memoryClear ( (uint8 *)respInfoPtr, sizeof(strFbLogicVersionInfoReadRespInfo));
	
	memoryClear ( (uint8 *)&mpuExeInfo, sizeof(strModuleExecInfo));
	retVal = fbSysExeModuleInfoRead(*memoryKind, &mpuExeInfo);
	if(retVal != NO_ERROR) {
        setErrorCode( __FILE__,
        				__LINE__,
        				"fbLogicVersionInfoReadReqRespFunc",
        				retVal
        			  );
		goto MAKE_RESP;
	}
	respDataLen += sizeof(strFbLogicVersionInfoReadRespInfo);


MAKE_RESP:	

	
	
	if(retVal != NO_ERROR)
		respInfoPtr->fbLogicCompileTime 		= 0;
	else
		respInfoPtr->fbLogicCompileTime 		= mpuExeInfo.fbLogicCompileTime;
	
MODULE_END:

	respMsgPtr->dataLen = respDataLen;
	
	return(status);
}


static uint32	fbParaMemoryStatusInfoReadReqRespFunc (
									strNewNetProtocolInfo	*reqMsgPtr,
            						strBufInfo			*respMsgPtr
									)
{
	uint32	status = NO_ERROR;
	uint32	respDataLen = 0;
	strFbSpecMemStatusReadRespInfo	*respInfoPtr = NULL;

	uint32 *cmdCode, *inOutType;
	
	cmdCode = (uint32*)&reqMsgPtr->dataBuff[0];
	inOutType = (uint32*)&reqMsgPtr->dataBuff[4];

	
	if( (*inOutType != INPUT_PARA_TYPE)&&(*inOutType != OUTPUT_PARA_TYPE) ) {
		status = L_PARA_TYPE_ERR;
        setErrorCodeWithVal( 	__FILE__,__LINE__,"fbParaMemoryStatusInfoReadReqRespFunc",status,
        						"PARA TYPE",*inOutType,
        						"Not Using",0,"Not Using",0 
					  		);
		goto MODULE_END;		
	}

	
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0],(uint8*)cmdCode,DOUBLE_SIZE);
	respDataLen = DOUBLE_SIZE;

	
	respInfoPtr = (strFbSpecMemStatusReadRespInfo *)&respMsgPtr->dataBuf[4];

	memoryClear ( (uint8 *)respInfoPtr, sizeof(strFbSpecMemStatusReadRespInfo));
	respInfoPtr->inoutType = *inOutType;

	
	if(*inOutType == INPUT_PARA_TYPE)
		(void) fbUnusedSpecMemInfoRead(respInfoPtr);

	else if(*inOutType == OUTPUT_PARA_TYPE)
		(void) fbUnusedOutputMemInfoRead(respInfoPtr);

	respDataLen += sizeof(strFbSpecMemStatusReadRespInfo);
	respDataLen -= (MAX_FB_SPEC_FRAGMENT_NUM * 4);
	respDataLen += (respInfoPtr->fragmentMemNum * 4);


MODULE_END:

	respMsgPtr->dataLen = respDataLen;
		
	return(status);
}




uint32	fbOnlineDwlFbVarInfoChgReqAndRespFunc (
									strNewNetProtocolInfo	*reqMsgPtr,
            						strBufInfo			*respMsgPtr
									)
{
	uint32	status = NO_ERROR;
	uint32	respDataLen = 0, respRetLen;
	uint32	totalFrameNum = 0, curFrameNum = 0;
	uint32	onlineDwlKind = 0;
	uint32	*onlineDwlReqDataPtr 	= (uint32 *)&reqMsgPtr->dataBuff[4];
	uint32	*onlineDwlRespDataPtr 	= (uint32 *)&respMsgPtr->dataBuf[4];
	
	uint32  *cmdCode;

	cmdCode = (uint32*)&reqMsgPtr->dataBuff[0];
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0],(uint8*)cmdCode,DOUBLE_SIZE);
	respDataLen = DOUBLE_SIZE;

	totalFrameNum 	= (*onlineDwlReqDataPtr >> 16) & 0xffff;
	curFrameNum		= *onlineDwlReqDataPtr & 0xffff;

	
	if( (curFrameNum == 0) ||(curFrameNum > totalFrameNum)){
		status = ONLINE_DWL_FRAME_NUM_ERR;
        setErrorCodeWithVal( 	__FILE__,__LINE__,
                        		"fbOnlineDwlVarInfoChgReqAndRespFunc",status,
                        		"Total Frame Num",totalFrameNum,
                        		"Current Frame Num", curFrameNum,
                        		"Not Using", 0 
					  		);
		goto MODULE_END;		
	}

	
	*onlineDwlRespDataPtr++ = *onlineDwlReqDataPtr;
	respDataLen += 4;

	
	
	
	if(curFrameNum == 1){
		status = onlineDwlVarInfoChgVariableClear();
		if(status != NO_ERROR) {
        	setErrorCode( __FILE__,__LINE__,
              		        "fbOnlineDwlVarInfoChgReqAndRespFunc",status
					  	);
			status = FB_ONLINE_DOWNLOAD_ERR;
			goto MODULE_END;
			
		}
	}

	onlineDwlKind = ONLINE_DWL_VAR_INFO_CHANGE;

	
	respRetLen	= 0;

	status = onlineDownloadAccessFuncInComTask[onlineDwlKind-1] (	onlineDwlReqDataPtr,
																	onlineDwlRespDataPtr,
																	&respRetLen
																);
	
	respDataLen += respRetLen;

	if(status != NO_ERROR) {
        setErrorCode( __FILE__,__LINE__,
       	                "fbOnlineDwlFbVarInfoChgReqAndRespFunc",status
					  );
	}


MODULE_END:
	respMsgPtr->dataLen = respDataLen;
	return(status);
}


uint32	fbOnlineDwlFbTrsDataChgReqAndRespFunc (
									strNewNetProtocolInfo	*reqMsgPtr,
            						strBufInfo			*respMsgPtr
									)
{
	uint32	status = NO_ERROR;
	uint32	respDataLen = 0, respRetLen;
	uint32	totalFrameNum = 0, curFrameNum = 0;
	uint32	onlineDwlKind = 0;
	uint32	*onlineDwlReqDataPtr 	= (uint32 *)&reqMsgPtr->dataBuff[4];
	uint32	*onlineDwlRespDataPtr 	= (uint32 *)&respMsgPtr->dataBuf[4];
	uint32  *cmdCode;
	
	cmdCode = (uint32*)&reqMsgPtr->dataBuff[0];
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0],(uint8*)cmdCode,DOUBLE_SIZE);
	respDataLen = DOUBLE_SIZE;

	totalFrameNum 	= (*onlineDwlReqDataPtr >> 16) & 0xffff;
	curFrameNum		= *onlineDwlReqDataPtr & 0xffff;

	
	if( (curFrameNum == 0) ||(curFrameNum > totalFrameNum)){
		status = ONLINE_DWL_FRAME_NUM_ERR;
        setErrorCodeWithVal( 	__FILE__,__LINE__,
                        		"fbOnlineDwlFbTrsDataChgReqAndRespFunc",status,
                        		"Total Frame Num",totalFrameNum,
                        		"Current Frame Num", curFrameNum,
                        		"Not Using", 0 
					  		);
		goto MODULE_END;		
	}

	
	*onlineDwlRespDataPtr++ = *onlineDwlReqDataPtr;
	respDataLen += 4;

	
	
	
	if(curFrameNum == 1){
		status = onlineDwlTrsDataChgVariableClear();
		if(status != NO_ERROR) {
        	setErrorCode( __FILE__,__LINE__,
              		        "fbOnlineDwlFbAddReqAndRespFunc",status
					  	);
			status = FB_ONLINE_DOWNLOAD_ERR;
			goto MODULE_END;
			
		}
	}
	
	onlineDwlKind 		= ONLINE_DWL_TRS_DATA_CHANGE;
		
	
	respRetLen	= 0;

	status = onlineDownloadAccessFuncInComTask[onlineDwlKind-1] (	onlineDwlReqDataPtr,
																	onlineDwlRespDataPtr,
																	&respRetLen
																);
	
	respDataLen += respRetLen;

	if(status != NO_ERROR) {
        setErrorCode( __FILE__,__LINE__,
       	                "fbOnlineDwlFbTrsDataChgReqAndRespFunc",
           	            status
					  );
	}

		
MODULE_END:
	respMsgPtr->dataLen = respDataLen;

	return(status);
}



uint32	fbOnlineDwlFbChgReqAndRespFunc 	(
										strNewNetProtocolInfo	*reqMsgPtr,
            							strBufInfo			*respMsgPtr
										)
{
	uint32	status = NO_ERROR;
	uint32	respDataLen = 0, respRetLen;
	uint32	totalFrameNum = 0, curFrameNum = 0;
	uint32	onlineDwlKind = 0;
	uint32	*onlineDwlReqDataPtr 	= (uint32 *)&reqMsgPtr->dataBuff[4];
	uint32	*onlineDwlRespDataPtr 	= (uint32 *)&respMsgPtr->dataBuf[4];

	uint32  *cmdCode;

	cmdCode = (uint32*)&reqMsgPtr->dataBuff[0];
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0],(uint8*)cmdCode,DOUBLE_SIZE);
	respDataLen = DOUBLE_SIZE;

	totalFrameNum 	= (*onlineDwlReqDataPtr >> 16) & 0xffff;
	curFrameNum		= *onlineDwlReqDataPtr & 0xffff;

	
	if( (curFrameNum == 0) ||(curFrameNum > totalFrameNum)){
		status = ONLINE_DWL_FRAME_NUM_ERR;
        setErrorCodeWithVal( 	__FILE__,
        						__LINE__,
                        		"fbOnlineDwlFbChgReqAndRespFunc",
                        		status,
                        		"Total Frame Num",totalFrameNum,
                        		"Current Frame Num", curFrameNum,
                        		"Not Using", 0 
					  		);
		goto MODULE_END;		
	}

	
	*onlineDwlRespDataPtr++ = *onlineDwlReqDataPtr;
	respDataLen += 4;

	
	
	
	if(curFrameNum == 1){
		status = onlineDwlFbInfoChgVariableClear();
		if(status != NO_ERROR) {
        	setErrorCode( __FILE__,
        					__LINE__,
              		        "fbOnlineDwlFbChgReqAndRespFunc",
                        	status
					  	);
			status = FB_ONLINE_DOWNLOAD_ERR;
			goto MODULE_END;
			
		}
	}
	
	onlineDwlKind 		= ONLINE_DWL_FBCODE_CHANGE;
		
	
	respRetLen	= 0;

	status = onlineDownloadAccessFuncInComTask[onlineDwlKind-1] (	onlineDwlReqDataPtr,
																	onlineDwlRespDataPtr,
																	&respRetLen
																);
	
	respDataLen += respRetLen;

	if(status != NO_ERROR) {
        setErrorCode( __FILE__,
   	    				__LINE__,
       	                "fbOnlineDwlFbChgReqAndRespFunc",
           	            status
					  );
	}

	
MODULE_END:
	respMsgPtr->dataLen = respDataLen;
	
	return(status);
}



uint32	ioOnlineDwlFbSpecChgReqAndRespFunc 	(
										strNewNetProtocolInfo	*reqMsgPtr,
            							strBufInfo			*respMsgPtr
											)
{
	uint32	status = NO_ERROR;
	uint32	respDataLen = 0, respRetLen;
	uint32	totalFrameNum = 0, curFrameNum = 0;
	uint32	onlineDwlKind = 0;
	uint32	*onlineDwlReqDataPtr 	= (uint32 *)&reqMsgPtr->dataBuff[4];
	uint32	*onlineDwlRespDataPtr 	= (uint32 *)&respMsgPtr->dataBuf[4];
	
	uint32  *cmdCode;

	cmdCode = (uint32*)&reqMsgPtr->dataBuff[0];
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0],(uint8*)cmdCode,DOUBLE_SIZE);
	respDataLen = DOUBLE_SIZE;

	totalFrameNum 	= (*onlineDwlReqDataPtr >> 16) & 0xffff;
	curFrameNum		= *onlineDwlReqDataPtr & 0xffff;

	
	if( (curFrameNum == 0) ||(curFrameNum > totalFrameNum)){
		status = ONLINE_DWL_FRAME_NUM_ERR;
        setErrorCodeWithVal( 	__FILE__,
        						__LINE__,
                        		"ioOnlineDwlFbSpecChgReqAndRespFunc",
                        		status,
                        		"Total Frame Num",totalFrameNum,
                        		"Current Frame Num", curFrameNum,
                        		"Not Using", 0 
					  		);
		goto MODULE_END;		
	}

	
	*onlineDwlRespDataPtr++ = *onlineDwlReqDataPtr;
	respDataLen += 4;

	
	
	
	if(curFrameNum == 1){
		status = onlineDwlFbSpecChgVariableClear();
		if(status != NO_ERROR) {
        	setErrorCode( __FILE__,
        					__LINE__,
              		        "ioOnlineDwlFbSpecChgReqAndRespFunc",
                        	status
					  	);
			status = FB_ONLINE_DOWNLOAD_ERR;
			goto MODULE_END;
			
		}
	}

	onlineDwlKind 		= ONLINE_DWL_IO_SPEC_CHANGE;
		
	
	respRetLen	= 0;

	status = onlineDownloadAccessFuncInComTask[onlineDwlKind-1] (	onlineDwlReqDataPtr,
																	onlineDwlRespDataPtr,
																	&respRetLen
																);
	
	respDataLen += respRetLen;

	if(status != NO_ERROR) {
        setErrorCode( __FILE__,
   	    				__LINE__,
       	                "ioOnlineDwlFbSpecChgReqAndRespFunc",
           	            status
					  );
	}


MODULE_END:
	respMsgPtr->dataLen = respDataLen;

	return(status);
}
