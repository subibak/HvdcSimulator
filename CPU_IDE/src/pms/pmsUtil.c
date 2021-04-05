#include	<stdio.h>
#include	"usrTypes.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include	"sysConfig.h"
#include	"lkv120Board.h"
#include	"usrErrCode.h"
#include    "fbMem.h"
#include	"vmeDef.h"
#include	"pmsUtil.h"
#include	"systemDiag.h"
#include	"sysDual.h"

uint32	M_MEMORY_AREA_START_OFFSET = 0;
uint32	M_MEMORY_AREA_MAX_SIZE = 0;
uint32	S_MEMORY_AREA_START_OFFSET = 0;
uint32	S_MEMORY_AREA_MAX_SIZE = 0;


extern strSysRunningConfigInfo		*gSysRunningInfoPtr;

extern uint32	systemCnfgInfoRead( strSysConfigInfo *sysCnfgInfo);
extern uint32	fbMemPointerGet
					(
				uint32	moduleId,
                strMemPointerInfo	*memInfoPtr
                	);

int32 mMemReadPoint(uint32 mdbsAddr, uint32 *valuePtr);
int32 mMemWordReadPoint(uint32 mdbsAddr, uint16 *valuePtr);
int32 mMemReadBlock(uint32 mdbsAddr, uint32 *valuePtr, uint32 valueNum);
int32 mMemWritePoint(uint32 mdbsAddr, uint32 *valuePtr);
int32 mMemWordWritePoint(uint32 mdbsAddr, uint16 *valuePtr);


int32 mMemReadPoint(uint32 mdbsAddr, uint32 *valuePtr)
{
	uint32 status = NO_ERROR;
	uint32 objType = 0;
	uint32 rAddr, bitPosition;

    strSysConfigInfo configData;
    strMemPointerInfo	memPointer;
	uint8	*memDataPtr;
    strSysRunningConfigInfo		*sysStatusInfoPtr = gSysRunningInfoPtr;

    systemCnfgInfoRead((strSysConfigInfo *)&configData);
	
	if(valuePtr == NULL)
		return (-1);

	if(mdbsAddr < (MAX_MB_BO_ADDR + 1))
		objType = MODBUS_BO;
	else if(mdbsAddr < (MAX_MB_BI_ADDR + 1))
		objType = MODBUS_BI;
	else if(mdbsAddr < 30000)
		return (status);
	else if(mdbsAddr < (MAX_MB_AI_ADDR + 1))
		objType = MODBUS_AI;
	else if(mdbsAddr < (MAX_MB_AO_ADDR + 1))
		objType = MODBUS_AO;
	else 
		return (-1);

	switch(objType)
	{
		case MODBUS_AI:
			if((mdbsAddr%2 != 0)) return (status);
			
			rAddr = mdbsAddr - MIN_MB_AI_ADDR;
			rAddr = rAddr/2;

            if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN){
            	status = fbMemPointerGet(LOGIC_ID, &memPointer);
            }
            else{
            	status = busFbMemPointerGet(LOGIC_ID, &memPointer);
            }
			memDataPtr = (uint8 *)memPointer.flagMemPtr;
            memDataPtr += (MODBUS_AI_MMEM_START_INDEX + rAddr)*4;
           *valuePtr = *(volatile uint32*)memDataPtr;
		break;
		case MODBUS_AO:
			if((mdbsAddr%2 != 0)) return (status);
			
			rAddr = mdbsAddr - MIN_MB_AO_ADDR;
			rAddr = rAddr/2;

            if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN){
            	status = fbMemPointerGet(LOGIC_ID, &memPointer);
            }
            else{
            	status = busFbMemPointerGet(LOGIC_ID, &memPointer);
            }
			memDataPtr = (uint8 *)memPointer.flagMemPtr;
            memDataPtr += (MODBUS_AO_MMEM_START_INDEX + rAddr)*4;
           *valuePtr = *(volatile uint32*)memDataPtr;
			
			

		break;
		case MODBUS_BI:
			rAddr = mdbsAddr - MIN_MB_BI_ADDR;
			bitPosition = rAddr%32;
			rAddr = rAddr/32;

            if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN){
            	status = fbMemPointerGet(LOGIC_ID, &memPointer);
            }
            else{
            	status = busFbMemPointerGet(LOGIC_ID, &memPointer);
            }
			memDataPtr = (uint8 *)memPointer.flagMemPtr;
			memDataPtr += (MODBUS_BI_MMEM_START_INDEX + rAddr)*4;
           *valuePtr = (*(volatile uint32*)memDataPtr >> bitPosition) &0x1;
		break;
		case MODBUS_BO:
			rAddr = mdbsAddr - MIN_MB_BO_ADDR;
			bitPosition = rAddr%32;
			rAddr = rAddr/32;

            if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN){
            	status = fbMemPointerGet(LOGIC_ID, &memPointer);
            }
            else{
            	status = busFbMemPointerGet(LOGIC_ID, &memPointer);
            }
			memDataPtr = (uint8 *)memPointer.flagMemPtr;
			memDataPtr += (MODBUS_BO_MMEM_START_INDEX + rAddr)*4;
           *valuePtr = (*(volatile uint32*)memDataPtr >> bitPosition) &0x1;

		break;
		default:
			return (-1);
		break;
	}
	
	return (status);
}


int32 mMemWordReadPoint(uint32 mdbsAddr, uint16 *valuePtr)
{
	uint32 status = NO_ERROR;
	uint32 objType = 0;
	uint32 rAddr, bitPosition;

    strSysConfigInfo configData;
    strMemPointerInfo	memPointer;
	uint8	*memDataPtr;
    strSysRunningConfigInfo		*sysStatusInfoPtr = gSysRunningInfoPtr;
    
    systemCnfgInfoRead((strSysConfigInfo *)&configData);
	
	if(valuePtr == NULL)
		return (-1);

	if(mdbsAddr < (MAX_MB_BO_ADDR + 1))
		objType = MODBUS_BO;
	else if(mdbsAddr < (MAX_MB_BI_ADDR + 1))
		objType = MODBUS_BI;
	else if(mdbsAddr < 30000)
		return (status);
	else if(mdbsAddr < (MAX_MB_AI_ADDR + 1))
		objType = MODBUS_AI;
	else if(mdbsAddr < (MAX_MB_AO_ADDR + 1))
		objType = MODBUS_AO;
	else 
		return (-1);

	switch(objType)
	{
		case MODBUS_AI:	
			rAddr = mdbsAddr - MIN_MB_AI_ADDR;

		    if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN){
				status = fbMemPointerGet(LOGIC_ID, &memPointer);
			}
            else{
            	status = busFbMemPointerGet(LOGIC_ID, &memPointer);
            }
			memDataPtr = (uint8 *)memPointer.flagMemPtr;
			memDataPtr += ((MODBUS_AI_MMEM_START_INDEX*2) + rAddr)*2;
			*valuePtr = *(volatile uint16*)memDataPtr;

		break;
		case MODBUS_AO:
			rAddr = mdbsAddr - MIN_MB_AO_ADDR;

            if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN){
            	status = fbMemPointerGet(LOGIC_ID, &memPointer);
            }
            else{
            	status = busFbMemPointerGet(LOGIC_ID, &memPointer);
            }
			memDataPtr = (uint8 *)memPointer.flagMemPtr;
            memDataPtr += ((MODBUS_AO_MMEM_START_INDEX*2) + rAddr)*2;
			*valuePtr = *(volatile uint16*)memDataPtr;

		break;
		default:
			return (-1);
		break;
	}
	
	return (status);
}


int32 mMemWritePoint(uint32 mdbsAddr, uint32 *valuePtr)
{
	uint32 status = NO_ERROR;
	uint32 objType = 0;
	uint32 rAddr, bitPosition, bitVal;

    strSysConfigInfo configData;
    strMemPointerInfo	memPointer;
	uint8	*memDataPtr;
    strSysRunningConfigInfo		*sysStatusInfoPtr = gSysRunningInfoPtr;

    systemCnfgInfoRead((strSysConfigInfo *)&configData);
	
	if(valuePtr == NULL)
		return (-1);

	if(mdbsAddr < (MAX_MB_BO_ADDR + 1))
		objType = MODBUS_BO;
	else if(mdbsAddr < (MAX_MB_BI_ADDR + 1))
		objType = MODBUS_BI;
	else if(mdbsAddr < 30000)
		return (-1);
	else if(mdbsAddr < (MAX_MB_AI_ADDR + 1))
		objType = MODBUS_AI;
	else if(mdbsAddr < (MAX_MB_AO_ADDR + 1))
		objType = MODBUS_AO;
	else 
		return (-1);

	/* 2019.01.20 이동일 : Write는 SYS_RAS_REDUNDANCY_RUN일 때만 허용도록 함. */
	if(sysStatusInfoPtr->sysRedundancyStatus != SYS_RAS_REDUNDANCY_RUN){
		
		return (-1);
	}

	switch(objType)
	{
		case MODBUS_AO:
			if((mdbsAddr%2 != 0)) return (status);

			rAddr = mdbsAddr - MIN_MB_AO_ADDR;
			rAddr = rAddr/2;

            status = fbMemPointerGet(LOGIC_ID, &memPointer);
			memDataPtr = (uint8 *)memPointer.flagMemPtr;
            memDataPtr += (MODBUS_AO_MMEM_START_INDEX + rAddr)*4;
           *(volatile uint32*)memDataPtr = *valuePtr;

			if(status == NO_ERROR) {
				status = busVariableDataWrite(valuePtr,MMEM_VAR,
											  (MODBUS_AO_MMEM_START_INDEX + rAddr), SPEC_SIZE_DWORD/*SPEC_SIZE_WORD*/, EN_RETAIN_CHECK) ; 
			}
			
			if(status == NO_ERROR) {
				if( dualFlagCheckInRunShelf() == DUAL_CHECK_ENABLE)
					status = rmtVariableDataWrite(valuePtr,MMEM_VAR,
											  (MODBUS_AO_MMEM_START_INDEX + rAddr), SPEC_SIZE_DWORD/*SPEC_SIZE_WORD*/, EN_RETAIN_CHECK) ; 
			}
            
		break;
		case MODBUS_BO:
			rAddr = mdbsAddr - MIN_MB_BO_ADDR;
			bitPosition = rAddr%32;
			rAddr = rAddr/32;

			bitVal = ((*valuePtr)&0x1)<<bitPosition;

			if(bitVal)
			{
                status = fbMemPointerGet(LOGIC_ID, &memPointer);
				memDataPtr = (uint8 *)memPointer.flagMemPtr;
                memDataPtr += (MODBUS_BO_MMEM_START_INDEX + rAddr)*4;
               *(volatile uint32*)memDataPtr |= bitVal;
			}
			else
			{
				
                status = fbMemPointerGet(LOGIC_ID, &memPointer);
				memDataPtr = (uint8 *)memPointer.flagMemPtr;
                memDataPtr += (MODBUS_BO_MMEM_START_INDEX + rAddr)*4;
               *(volatile uint32*)memDataPtr &= (~(1<<bitPosition));
			}

			if(status == NO_ERROR) {
				status = busVariableDataWrite(valuePtr,MMEM_VAR,
											  (MODBUS_BO_MMEM_START_INDEX + rAddr), SPEC_SIZE_DWORD/*SPEC_SIZE_WORD*/, EN_RETAIN_CHECK) ; 
			}
			
			if(status == NO_ERROR) {
				if( dualFlagCheckInRunShelf() == DUAL_CHECK_ENABLE)
					status = rmtVariableDataWrite(valuePtr,MMEM_VAR,
											  (MODBUS_BO_MMEM_START_INDEX + rAddr), SPEC_SIZE_DWORD/*SPEC_SIZE_WORD*/, EN_RETAIN_CHECK) ; 
			}

		break;
		default:
			return (-1);
		break;
	}
	
	return (status);
}



int32 mMemWordWritePoint(uint32 mdbsAddr, uint16 *valuePtr)
{
	uint32 status = NO_ERROR;
	uint32 objType = 0;
	uint32 rAddr, bitPosition, bitVal;

    strSysConfigInfo configData;
    strMemPointerInfo	memPointer;
	uint8	*memDataPtr;
	uint32	val32;
    strSysRunningConfigInfo		*sysStatusInfoPtr = gSysRunningInfoPtr;

    systemCnfgInfoRead((strSysConfigInfo *)&configData);
	
	if(valuePtr == NULL)
		return (-1);

	if(mdbsAddr < (MAX_MB_BO_ADDR + 1))
		objType = MODBUS_BO;
	else if(mdbsAddr < (MAX_MB_BI_ADDR + 1))
		objType = MODBUS_BI;
	else if(mdbsAddr < 30000)
		return (-1);
	else if(mdbsAddr < (MAX_MB_AI_ADDR + 1))
		objType = MODBUS_AI;
	else if(mdbsAddr < (MAX_MB_AO_ADDR + 1))
		objType = MODBUS_AO;
	else 
		return (-1);

	if(sysStatusInfoPtr->sysRedundancyStatus != SYS_RAS_REDUNDANCY_RUN){
		
		return (-1);
	}

	switch(objType)
	{
		case MODBUS_AO:
			rAddr = mdbsAddr - MIN_MB_AO_ADDR;

			val32 = (*valuePtr);
			status = variableDataWrite((uint32*)&val32, MMEM_VAR, ((MODBUS_AO_MMEM_START_INDEX*2) + rAddr), SPEC_SIZE_WORD) ; 


			if(status == NO_ERROR) {
				status = busVariableDataWrite((uint32*)&val32,MMEM_VAR,
											  ((MODBUS_AO_MMEM_START_INDEX*2) + rAddr), /*SPEC_SIZE_DWORD*/SPEC_SIZE_WORD, EN_RETAIN_CHECK) ; 
			}
			
			if(status == NO_ERROR) {
				if( dualFlagCheckInRunShelf() == DUAL_CHECK_ENABLE)
					status = rmtVariableDataWrite((uint32*)&val32,MMEM_VAR,
											  ((MODBUS_AO_MMEM_START_INDEX*2) + rAddr), /*SPEC_SIZE_DWORD*/SPEC_SIZE_WORD, EN_RETAIN_CHECK) ; 
			}

		break;
		default:
			return (-1);
		break;
	}
	
	return (status);
}

int32 mMemReadBlock(uint32 mdbsAddr, uint32 *valuePtr, uint32 valueNum)
{
	uint32 status = NO_ERROR;
	uint32 objType = 0;
	uint32 rAddr, bitPosition;
	uint32 valueLoop;
	uint32 incMdbsAddr, nextMdbsAddr, *nextValuePtr;
	
	if((valuePtr == NULL) || (valueNum == 0))
		return (-1);
		
	if(mdbsAddr < (MAX_MB_BO_ADDR + 1)) {
		objType = MODBUS_BO;
		incMdbsAddr = 1;
	}
	else if(mdbsAddr < (MAX_MB_BI_ADDR + 1)) {
		objType = MODBUS_BI;
		incMdbsAddr = 1;
	}
	else if(mdbsAddr < 30000)
		return (status);
	else if(mdbsAddr < (MAX_MB_AI_ADDR + 1)) {
		objType = MODBUS_AI;
		incMdbsAddr = 2;
	}
	else if(mdbsAddr < (MAX_MB_AO_ADDR + 1)) {
		objType = MODBUS_AO;
		incMdbsAddr = 2;
	}
	else 
		return (-1);
		
	for(valueLoop=0;valueLoop<valueNum;valueLoop++)
	{
		nextMdbsAddr = mdbsAddr + (valueLoop*incMdbsAddr);
		nextValuePtr = valuePtr + valueLoop;
		status = mMemReadPoint(nextMdbsAddr, nextValuePtr);
		if(status != NO_ERROR) {
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

			return (-1);
		}
	}
	
	return (status);
}
