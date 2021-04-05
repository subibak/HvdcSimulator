
#include	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"cmdList.h"

#include	"pcsDefs.h"
#include	"netProtocol.h"
#include	"usrDefBuf.h"

#include	"segScheduler.h"


#include	"protocolBuf.h"
#include	"fbMem.h"
#include	"usrErrCode.h"
#include	"sysConfig.h"

#include	"segScheduler.h"

#include	"memDef.h"
#include	"fbDef.h"
#include	"systemDiag.h"
#include	"usrQueData.h"
#include	"sysDual.h"


extern uint32 romSwVersion;
extern strSysRunningConfigInfo		*gSysRunningInfoPtr;
extern strSystemRunningStatusInfo	gSysRunningStatusInfo;

uint32	systemHistoryReqAndRespFunc(strNewNetProtocolInfo	*, strBufInfo *);
uint32	systemTaskReadReqAndRespFunc(strNewNetProtocolInfo	*, strBufInfo *);
uint32	systemInfoReadReqAndRespFunc(strNewNetProtocolInfo	*, strBufInfo *);
uint32	systemDataWirteReqAndRespFunc(strNewNetProtocolInfo	*, strBufInfo *);

extern uint32	optionCodeCmdNullFunc(strNewNetProtocolInfo *,strBufInfo *);
extern void		memoryClear( uint8 *, uint32);
extern uint32	sysHistoryFatalErrMsgGet(strSysHistoryInfo*);
extern void		memoryCopy ( uint8 *, uint8 *, uint32);
extern uint32	fbSysExeModuleInfoRead(uint32 , strModuleExecInfo* );
extern uint32	fbSysExeVarInfoRead(uint32 , strVariableExecInfo* );
extern void		setErrorCode(uint8 *, uint32, uint8 *, uint32);
extern uint32	getTotalTaskNumber(uint32,uint32, uint32*);
extern uint32	getTaskExecTime(uint32, uint32,strExecTimeInfo *);
extern uint32	getFbTaskTblInfo(uint32 , uint32 , strFbTaskTblMemInfo *);
extern uint32	systemCnfgInfoRead( strSysConfigInfo *);

extern uint32	fbSysMaxJmpLoopInfo(uint32);
extern uint32	fbMemInfoRead(uint32, uint32, strFbMemInfo *);
extern uint32   jmpFbCodeCheck(uint32);
extern uint32	fbCodeSpecDataReadWithType(strFbDataReadInfo*, uint32*, uint32);
extern uint32	jumpCountVarCheck(uint32 , uint32, uint32*);
extern void		setErrorCodeWithVal(uint8 *, uint32 , uint8 *, uint32,
									  uint8 *, uint32,uint8 *, uint32,uint8 *, uint32
								  	 );
extern void		systemRasInfoSet(uint32, uint32);
extern void		logMessageSet (int8 *, uint32, int8 *, uint32, uint32 );



uint32	(*sysStatusCmdOPFuncList[]) 
			(strNewNetProtocolInfo	*,strBufInfo *)
= {
	optionCodeCmdNullFunc,								
 	optionCodeCmdNullFunc,							 
 	systemHistoryReqAndRespFunc,					
	systemTaskReadReqAndRespFunc,					
	systemInfoReadReqAndRespFunc,					
	optionCodeCmdNullFunc
  };



uint32 systemHistoryReqAndRespFunc
				(
		strNewNetProtocolInfo 	*reqMsgPtr,
        strBufInfo			*respMsgPtr
    		)
{
	uint32	status = NO_ERROR, retVal;
	uint32	respDataLen = 0, cmdCode;
    uint32 *reqDataPtr = (uint32 *)reqMsgPtr->dataBuff;
    uint32 *respDataPtr = (uint32 *)&respMsgPtr->dataBuf[0];
	strSysHistoryInfo *systemHistory;

	cmdCode = *reqDataPtr++;
	memoryCopy((uint8*)respDataPtr,(uint8*)&cmdCode,DOUBLE_SIZE);
	respDataLen = DOUBLE_SIZE;

	systemHistory = (strSysHistoryInfo *) &respMsgPtr->dataBuf[8];
	retVal = sysHistoryFatalErrMsgGet(systemHistory);

	memoryCopy((uint8*)&respMsgPtr->dataBuf[4],(uint8*)&retVal, DOUBLE_SIZE);
	respDataLen += DOUBLE_SIZE;

	if(retVal == NO_ERROR){
		respDataLen += DOUBLE_SIZE + systemHistory->historyNum * sizeof(strSysHistoryData);
	}

	respMsgPtr->dataLen = respDataLen;
	
	return(status);


}


uint32 systemTaskReadReqAndRespFunc
				(
		strNewNetProtocolInfo 	*reqMsgPtr,
        strBufInfo			*respMsgPtr
    		)
{
	uint32	status = NO_ERROR;
	uint32	respDataLen = 0, *cmdCode;
	strModuleExecInfo sysExeInfo;
	uint32 *respPtr;
	uint32 segLoop, taskNum;
	strFbTaskTblMemInfo segTblInfo;
	strExecTimeInfo  execTimeInfo;
	strTaskStatusReadInfo *taskStatusPtr,*saveStatusPtr;
	uint32 moduleId =0, recvTaskId=0, numOfTotalTask=0;

	strSysTaskNumData *taskNumInfo;
	uint32 numOfTaskCurSend = 0, numOfRemainTask =0;

	
    cmdCode = (uint32*)(&reqMsgPtr->dataBuff[0]);
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0],(uint8*)cmdCode,DOUBLE_SIZE);
	respDataLen = DOUBLE_SIZE;

	memoryCopy((uint8*)&moduleId,(uint8*)&reqMsgPtr->dataBuff[4],DOUBLE_SIZE);
	memoryCopy((uint8*)&recvTaskId,(uint8*)&reqMsgPtr->dataBuff[8],DOUBLE_SIZE);

	taskNumInfo = (strSysTaskNumData*)&respMsgPtr->dataBuf[4];
	respPtr =     (uint32*)&respMsgPtr->dataBuf[20];

	
	status = getTotalTaskNumber(moduleId, recvTaskId,  &numOfTotalTask);
	if(status != NO_ERROR){
		numOfTotalTask = 0;
		setErrorCode(__FILE__,__LINE__,"systemTaskReadReqAndRespFunc",status);
		goto MODULE_END;
	}

	respDataLen += sizeof(strSysTaskNumData);
	taskNumInfo->moduleId  = moduleId;
	taskNumInfo->totTaksNo = numOfTotalTask;
	
	
	taskNum = 0;
	taskNumInfo->nextTaskId = 0;
	taskStatusPtr = (strTaskStatusReadInfo*)respPtr;
	saveStatusPtr = (strTaskStatusReadInfo*)taskStatusPtr;

	status = fbSysExeModuleInfoRead(moduleId, &sysExeInfo);
	if(status != NO_ERROR){
		setErrorCode(__FILE__,__LINE__,"systemTaskReadReqAndRespFunc", status);
		goto MODULE_END;
	}
	
	for(segLoop=recvTaskId; segLoop<= sysExeInfo.curMaxSegmentId; segLoop++,taskStatusPtr++)
	{
		memoryClear( (uint8 *)&segTblInfo, sizeof(strFbTaskTblMemInfo));
	
   		(void)getFbTaskTblInfo(	moduleId, 
   								segLoop,
								(strFbTaskTblMemInfo	*) &segTblInfo
							);

		if(segTblInfo.taskId != segLoop) continue;

		taskNum++;
		if(taskNum > MAX_TASK_INFO_NUM) {
			taskNumInfo->nextTaskId = segTblInfo.taskId;
			numOfTaskCurSend = MAX_TASK_INFO_NUM;
			break;
		}
		else {		
			taskNumInfo->nextTaskId = 0;
			numOfTaskCurSend = taskNum;
		}

		memoryClear( (uint8 *)&execTimeInfo, sizeof(strExecTimeInfo));

		
		status = getTaskExecTime(moduleId, segTblInfo.taskId, &execTimeInfo);
		if(status != NO_ERROR){
			setErrorCodeWithVal(__FILE__,__LINE__,
								  "systemTaskReadReqAndRespFunc", status,
								  "MODULE ID ",moduleId ,
								  "TASK ID ",segTblInfo.taskId,
								  "Not Using",0);
			goto MODULE_END;
		}

		taskStatusPtr->taskID = segTblInfo.taskId;

		taskStatusPtr->taskRunMode = (segTblInfo.loopStatus) << 16;
		taskStatusPtr->taskRunMode |= (segTblInfo.taskMode)& 0xffff;

		taskStatusPtr->maxExecTime = (uint32)execTimeInfo.maxExecTime;
		taskStatusPtr->curExecTime = (uint32)execTimeInfo.curExecTime;
		taskStatusPtr->usedFbCodeNum = segTblInfo.fbUsedNum;
		taskStatusPtr->usedInputMem =  segTblInfo.memInputSize;
		taskStatusPtr->usedOutputMem = segTblInfo.memOutputSize;
		respDataLen += sizeof(strTaskStatusReadInfo);
	}

	
	numOfRemainTask = 0;
	if( (taskNumInfo->nextTaskId != 0) && (moduleId == LOGIC_ID) &&
		(taskNumInfo->nextTaskId <= sysExeInfo.curMaxSegmentId)
	) {
		for(segLoop=taskNumInfo->nextTaskId; segLoop<= sysExeInfo.curMaxSegmentId; segLoop++)
		{
			memoryClear( (uint8 *)&segTblInfo, sizeof(strFbTaskTblMemInfo));
		
   			(void)getFbTaskTblInfo( moduleId,segLoop,
								(strFbTaskTblMemInfo	*) &segTblInfo
								);
			if(segTblInfo.taskId != segLoop) continue;
			numOfRemainTask++;
		}
	}
	
    taskNumInfo->curRemaintaskNo = (numOfRemainTask & 0xffff);
	taskNumInfo->curRemaintaskNo |= numOfTaskCurSend <<16;

MODULE_END:
	respMsgPtr->dataLen = respDataLen;
	return(status);


}

uint32 systemInfoReadReqAndRespFunc
				(
		strNewNetProtocolInfo 	*reqMsgPtr,
        strBufInfo			*respMsgPtr
    		)
{
	uint32	status = NO_ERROR;
	uint32	respDataLen = 0, *cmdCode;
	strModuleExecInfo sysExeInfo;
	uint32 *respPtr, zeroData = 0;
	uint32 initNum =0, retainNum=0;
	strVariableExecInfo sysVarExeInfo;

	uint32 loopingFBNum = 0, i; 
	strMpuDiuRunningStatusInfo	*mpuRasPtr;
	uint32 mpuMaxSegId = 0, mpuMaxFBaddr = 0;
	uint32 jmpErr = 0, jmpAddr = 0;
	strFbTaskTblMemInfo  segTblInfo;
	strFbMemInfo fbMemInfo;
	uint32 loopStatus[MAX_LOGIC_TASK_NUM];
	uint32 loopFBAddr[MAX_LOGIC_TASK_NUM];
	strFbDataReadInfo readFbInputData;

    cmdCode = (uint32*)(&reqMsgPtr->dataBuff[0]);
	memoryCopy((uint8*)&respMsgPtr->dataBuf[0],(uint8*)cmdCode,DOUBLE_SIZE);
	respDataLen=DOUBLE_SIZE;

	respPtr = (uint32*)&respMsgPtr->dataBuf[4];
	
	*respPtr++ = 0x1;
	respDataLen += DOUBLE_SIZE;

	*respPtr++ =romSwVersion;
	respDataLen += DOUBLE_SIZE;


	
	memoryClear((uint8*)&sysExeInfo,sizeof(strModuleExecInfo) );
	status = fbSysExeModuleInfoRead(LOGIC_ID, &sysExeInfo);
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__, __LINE__,
				"systemInfoReadReqAndRespFunc",status
			 );
		memoryCopy((uint8*)respPtr,(uint8*)&zeroData,DOUBLE_SIZE*4);
		respPtr += 4;
	}
	else{
		*respPtr++ = sysExeInfo.curMaxSegmentId;
		*respPtr++ = sysExeInfo.curfbMaxBlockAddr;
		*respPtr++ = sysExeInfo.curfbMaxParaIndex;
		*respPtr++ = sysExeInfo.curfbMaxOutIndex;

		mpuMaxSegId = sysExeInfo.curMaxSegmentId;
		mpuMaxFBaddr = sysExeInfo.curfbMaxBlockAddr;
	}
	respDataLen += DOUBLE_SIZE*4;
	
    memoryCopy((uint8*)respPtr,(uint8*)&zeroData,DOUBLE_SIZE);
    respPtr ++;
	respDataLen += DOUBLE_SIZE;
	
	memoryCopy((uint8*)respPtr,(uint8*)&zeroData,DOUBLE_SIZE);
	respPtr ++;
	respDataLen += DOUBLE_SIZE;
	
	memoryCopy((uint8*)respPtr,(uint8*)&zeroData,DOUBLE_SIZE);
	respPtr ++;
	respDataLen += DOUBLE_SIZE;
	
	memoryClear((uint8*)&sysExeInfo,sizeof(strModuleExecInfo) );
    memoryCopy((uint8*)respPtr,(uint8*)&zeroData,DOUBLE_SIZE*3);
    respPtr += 3;
	respDataLen += DOUBLE_SIZE*3;
	
	memoryClear((uint8*)&sysExeInfo,sizeof(strModuleExecInfo) );
	memoryCopy((uint8*)respPtr,(uint8*)&zeroData,DOUBLE_SIZE);
	respPtr += 1;
	respDataLen += DOUBLE_SIZE;
	
	memoryClear((uint8*)&sysVarExeInfo,sizeof(strVariableExecInfo) );
	status = fbSysExeVarInfoRead(AUTO_VAR,&sysVarExeInfo);
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__, __LINE__,
				"systemInfoReadReqAndRespFunc",status
			 );
		*respPtr++ = 0;
	}
	else {
		initNum = sysVarExeInfo.curMaxVarInitNum;
		retainNum = sysVarExeInfo.curMaxVarRetainNum;
		*respPtr++ = sysVarExeInfo.curMaxVarAddr;
	}
	respDataLen += DOUBLE_SIZE;

	
	memoryClear((uint8*)&sysVarExeInfo,sizeof(strVariableExecInfo) );
	status = fbSysExeVarInfoRead(MMEM_VAR,&sysVarExeInfo);
	if(status != NO_ERROR) {
		setErrorCode(	__FILE__, __LINE__,
				"systemInfoReadReqAndRespFunc",status
			 );
		*respPtr++ = 0;
	}
	else {
		initNum += sysVarExeInfo.curMaxVarInitNum;
		retainNum += sysVarExeInfo.curMaxVarRetainNum;
		*respPtr++ = sysVarExeInfo.curMaxVarAddr;
	}
	respDataLen += DOUBLE_SIZE;

	
	*respPtr++ = initNum;
	*respPtr++ = retainNum;
	respDataLen += DOUBLE_SIZE*2;

	
	*respPtr++ = 0;
	*respPtr++ = 0;
	*respPtr++ = 0;
	*respPtr++ = 0;
	respDataLen += DOUBLE_SIZE*4;

	 
	*respPtr++ = fbSysMaxJmpLoopInfo(MAX_MPU_BLOCK_ADDR);
	respDataLen += DOUBLE_SIZE;
	
	
	mpuRasPtr = &gSysRunningStatusInfo.sysMpuRunningStatusInfo;

	if(mpuRasPtr->moduleFBLoopingStatus!=NO_ERROR) {

		for(i=0; i<mpuMaxSegId; i++) {
			status = getFbTaskTblInfo(LOGIC_ID,i+1,&segTblInfo);
			if(status!= NO_ERROR) {
				setErrorCode(__FILE__,__LINE__,"systemInfoReadReqAndRespFunc",status);
			}
			else if(segTblInfo.loopStatus == FB_LOOPING_ERR) {
				loopStatus[i] = FB_LOOPING_ERR;			
			}
			else loopStatus[i] = NO_ERROR;	
		}

		for(i=0; i<mpuMaxFBaddr; i++) {
			status = fbMemInfoRead(LOGIC_ID,i+1,&fbMemInfo);
			if(status != NO_ERROR)
					setErrorCode(__FILE__,__LINE__,"systemInfoReadReqAndRespFunc",status);

			if( (fbMemInfo.fbTaskId > mpuMaxSegId) || 
				(fbMemInfo.fbFuncAddr != i+1) ||
				(loopStatus[fbMemInfo.fbTaskId-1] == NO_ERROR) ||
				(jmpFbCodeCheck(fbMemInfo.fbId) != IS_JUMP_CODE)
			  )
				continue;

			else if (loopStatus[fbMemInfo.fbTaskId-1] == FB_LOOPING_ERR) {
				status = jumpCountVarCheck(fbMemInfo.fbFuncAddr ,
											fbMemInfo.fbTaskId, &jmpErr);
				if(status != NO_ERROR)
					setErrorCode(__FILE__,__LINE__,"systemInfoReadReqAndRespFunc",status);

				if( (status == NO_ERROR)&&(jmpErr == FB_LOOPING_ERR) ) {
					loopFBAddr[loopingFBNum] = fbMemInfo.fbFuncAddr;
					loopingFBNum++;
				}
			}
		}
	}

	if(loopingFBNum > MAX_LOGIC_TASK_NUM){
		status = SEG_NUM_EXCEED_ERR;
		setErrorCode(__FILE__,__LINE__,"systemInfoReadReqAndRespFunc",status);

		*respPtr++ = loopingFBNum;
		respDataLen += DOUBLE_SIZE;
	}
	else {
		
		*respPtr++ = loopingFBNum;
		respDataLen += DOUBLE_SIZE;

		for(i=0; i<loopingFBNum; i++) {
			status = fbMemInfoRead(LOGIC_ID,loopFBAddr[i],&fbMemInfo);
			if(status != NO_ERROR)
				setErrorCode(__FILE__,__LINE__,"systemInfoReadReqAndRespFunc",status);

			if( (status == NO_ERROR) && 
				(jmpFbCodeCheck(fbMemInfo.fbId) == IS_JUMP_CODE)
			) {
				
				*respPtr = loopFBAddr[i] << 16;
				*respPtr++ |= (fbMemInfo.fbId & 0xffff);
				respDataLen += DOUBLE_SIZE;

				readFbInputData.moduleId = LOGIC_ID;
				readFbInputData.blockAddr = fbMemInfo.fbFuncAddr;
				readFbInputData.fbId = fbMemInfo.fbId;
				readFbInputData.index = 2; 

				 
				status = fbCodeSpecDataReadWithType( (strFbDataReadInfo*)&readFbInputData, 
													 (uint32*)&jmpAddr, LOCAL_MEM_ACC);

				if(status != NO_ERROR) {
					setErrorCode(__FILE__,__LINE__,"systemInfoReadReqAndRespFunc",status);
					*respPtr++ = 0;
					respDataLen += DOUBLE_SIZE;
				}
				else {
					
					*respPtr = fbMemInfo.fbTaskId << 16;
					*respPtr++ |= (jmpAddr & 0xffff);
					respDataLen += DOUBLE_SIZE;
				}
			}
			else{
				*respPtr++ = 0;
				*respPtr++ = 0;
				respDataLen += DOUBLE_SIZE*2;
			}
		}
	}

	respMsgPtr->dataLen = respDataLen;
	return(status);
}

#ifdef _WIN32

#define MAX_SYS_WRITE_DATA   8

#ifdef _WIN32
extern uint32	systemRunningStatusChange(uint32 , uint32 , uint32 *);
uint32 romImgUpdate();
#endif

uint32 systemDataWriteReqAndRespFunc
				(
		strNewNetProtocolInfo 	*reqMsgPtr,
        strBufInfo			*respMsgPtr
    		)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32	respDataLen = 0, *cmdCode;
	uint32  *writeKind, *reqPtr, i;
	uint32  recvData[MAX_SYS_WRITE_DATA]; 

	memoryClear((uint8*)&recvData, DOUBLE_SIZE*MAX_SYS_WRITE_DATA);

	
    cmdCode = (uint32*)(&reqMsgPtr->dataBuff[0]);
    writeKind = (uint32*)(&reqMsgPtr->dataBuff[4]);

	if(reqMsgPtr->messageType == REQUEST) {
		memoryCopy((uint8*)&respMsgPtr->dataBuf[0],(uint8*)cmdCode,DOUBLE_SIZE);
		respDataLen = DOUBLE_SIZE;
		memoryCopy((uint8*)&respMsgPtr->dataBuf[4],(uint8*)writeKind,DOUBLE_SIZE);
		respDataLen += DOUBLE_SIZE;
	}

	reqPtr = (uint32*)&reqMsgPtr->dataBuff[8];
	if(reqMsgPtr->messageType == REQUEST) {
		for(i=0;i<MAX_SYS_WRITE_DATA;i++)
		recvData[i] = *reqPtr++;
	}
	else if(reqMsgPtr->messageType == RESPONSE){
		recvData[0] = *reqPtr;
	}
	
	switch(*writeKind) {
#ifdef _WIN32
		case WRITE_KIND_ROM_UPD:
			if (recvData[0] == 1) {
				retVal = romImgUpdate(); 
			}
			else {
				retVal = SYS_DATA_WRITE_ERR;
				setErrorCode(__FILE__,__LINE__," systemTaskReadReqAndRespFunc", retVal);
			}
		break;
#endif
		case WRITE_KIND_MEM_CLR :
			if(recvData[0] == 0) {
				if(recvData[1] == 1 ){
					moduleFbMemoryClear(LOGIC_ID,0);
				}
				else if(recvData[1] == 3) {
					moduleFbMemoryClear(LOGIC_ID,0);
				}	
				else retVal = SYS_DATA_WRITE_ERR;
			}
			else retVal = SYS_DATA_WRITE_ERR;
			
			
			if(retVal == NO_ERROR){
				printf("\nIt will be Board Reboot!!!\n\n");
				logMessageSet(__FILE__, __LINE__,"MEM CLEAR", LOCAL_POWER_OFF, 0);
				boardReboot();
			}
			else {
				setErrorCode(__FILE__,__LINE__," systemDataWriteReqAndRespFunc", retVal);
			}
		break;

		case WRITE_KIND_RCS_RST:
			if( (recvData[0] == 0)&&(recvData[1] == 0)&&(recvData[2] == 1) ) {
				logMessageSet(__FILE__, __LINE__,"RCS RESET", LOCAL_POWER_OFF, 0);
				boardReboot();
			}
			else {
				retVal = SYS_DATA_WRITE_ERR;
				setErrorCode(__FILE__,__LINE__," systemDataWriteReqAndRespFunc", retVal);
			}
		break;

		default:
			retVal = SYS_DATA_WRITE_ERR;
			setErrorCode(__FILE__,__LINE__," systemDataWriteReqAndRespFunc", retVal);
		break;
	}
	
	if(reqMsgPtr->messageType == REQUEST) {
		memoryCopy((uint8*)&respMsgPtr->dataBuf[4],(uint8*)writeKind,DOUBLE_SIZE);
		respDataLen += DOUBLE_SIZE;

		memoryCopy((uint8*)&respMsgPtr->dataBuf[8],(uint8*)&retVal,DOUBLE_SIZE);
		respDataLen += DOUBLE_SIZE;
		respMsgPtr->dataLen = respDataLen;
	}

	return(status);


}
#endif



