#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_standard\fcCounter.h"

/**************************************************************************
**		외부 함수 
**************************************************************************/
extern uint32 readRuntimeFbData(uint32,uint32,uint32,uint32 *);
extern uint32 writeRuntimeFbData(uint32, uint32,uint32, uint32 *);
extern void	  setErrorCode(uint8 *, uint32, uint8 *, uint32);

/*********************************************************************
**	
**	Down Counter FB 정의
**
**********************************************************************/
uint32	stdCtdInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= CTD_CODE;
    paraInfoPtr->inputNo		= CTD_SPEC_NUM;
    paraInfoPtr->intlVarNo		= CTD_VAR_NUM;
    paraInfoPtr->outputNo	= CTD_OUTPUT_NUM;

	/* 입력 - 3개 */
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;

	/* 내부변수 - 1개 */	
	*varTypePtr++			= BOOL_TYPE | SIZE32_TYPE;

    /* 출력 - 2개 */
	*outputTypePtr++		= INT_TYPE | SIZE32_TYPE;
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;

	return (status);
}	 	

uint32	stdCtdRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	uint32	cd_q;

	strFC0201Info fd;
	
    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	/* clkInput이 Rising Edge 일때만 카운트를 감소시킴 */ 
	cd_q 			= fd.clkInput && !(fd.prevInput);

	fd.prevInput 	= fd.clkInput;

	/* resetCmd == TRUE이면 downCountVal = setVal */
	if(fd.loadCmd) 
	{
		fd.downCountVal = fd.loadVal;
	}
	else if( (cd_q) && (fd.loadVal > MIN_INT32_VALUE))
	{
		/* cv가 -32768 ~ 32768 사이에 있을 때만 감소 */
		fd.downCountVal = fd.downCountVal - 1;
	}

	/* q는 downCountVal<0일때 1 */
	if(fd.downCountVal <= 0) 
	{
		fd.q = 1;
	}
	else 
	{
		fd.q = 0;
	}

    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	return (status);
}

/*********************************************************************
**	
**	Up Counter FB 정의
**
**********************************************************************/
uint32	stdCtuInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= CTU_CODE;
    paraInfoPtr->inputNo		= CTU_SPEC_NUM;
    paraInfoPtr->intlVarNo		= CTU_VAR_NUM;
    paraInfoPtr->outputNo	= CTU_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++	= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++	= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++	= INT_TYPE | SIZE32_TYPE;

	/* 내부변수 - ?개 */	
	*varTypePtr++	= BOOL_TYPE | SIZE32_TYPE;

    /* 출력 - ?개 */
	*outputTypePtr++	= INT_TYPE | SIZE32_TYPE;
	*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;

	return (status);
}	 	

uint32	stdCtuRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	uint32	cu_q;

	strFC0202Info fd;
	
    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	/* 알고리즘 연산부 */
	cu_q 			= fd.clkInput && !(fd.prevInput);
	fd.prevInput 	= fd.clkInput;

	if(fd.resetCmd) 
	{
		fd.upCountVal = 0;
	}
	else if(cu_q && (fd.limitVal < MAX_INT32_VALUE))
	{
		fd.upCountVal = fd.upCountVal + 1;
	}

	/* q는 downCountVal<0일때 1 */
	if(fd.upCountVal >= fd.limitVal) 
	{
		fd.q = 1;
	}
	else 
	{
		fd.q = 0;
	}
	
    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

    return(status);
}

/*********************************************************************
**	
**	Up-Down Counter FB 정의
**
**********************************************************************/
uint32	stdCtudInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr){
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= CTUD_CODE;
    paraInfoPtr->inputNo		= CTUD_SPEC_NUM;
    paraInfoPtr->intlVarNo		= CTUD_VAR_NUM;
    paraInfoPtr->outputNo	= CTUD_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;

	/* 내부변수 - ?개 */	
	*varTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*varTypePtr++			= BOOL_TYPE | SIZE32_TYPE;

    /* 출력 - ?개 */
	*outputTypePtr++		= INT_TYPE | SIZE32_TYPE;
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;

	return (status);
}	 	

uint32	stdCtudRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	uint32	cu_q, cd_q;

	strFC0203Info fd;

	
    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }


	/* 알고리즘 연산부 */
	cu_q 				= fd.upClkInput && !(fd.prevUpClkInput);
	fd.prevUpClkInput 	= fd.upClkInput;

	cd_q 				= fd.downClkInput && !(fd.prevDownClkInput);
	fd.prevDownClkInput = fd.downClkInput;

	if(fd.resetCmd) 
	{
		fd.upDownCountVal = 0;
	} 
	else if(fd.loadCmd) 
	{
		fd.upDownCountVal = fd.loadVal;
	} 
	else if(cu_q && (fd.upDownCountVal < MAX_INT32_VALUE)) 
	{
		fd.upDownCountVal = fd.upDownCountVal + 1;
	}
	else if(cd_q && (fd.upDownCountVal > MIN_INT32_VALUE)) 
	{
		fd.upDownCountVal = fd.upDownCountVal - 1;
	}

	if(fd.upDownCountVal > fd.loadVal) 
	{
		fd.qu = 1;
	} 
	else 
	{
		fd.qu = 0;
	}

	if(fd.upDownCountVal <= 0) 
	{
		fd.qd = 1;
	} 
	else 
	{
		fd.qd = 0;
	}

    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

    return(status);
}
