#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_standard\fcNumerical.h"

/**************************************************************************
**		외부 함수 
**************************************************************************/
extern uint32 readRuntimeFbData(uint32,uint32,uint32,uint32 *);
extern uint32 fbCodeParaWriteWithVar(uint32,uint32, uint32, uint32 *,uint8 *);
extern void	  setErrorCode(uint8 *, uint32, uint8 *, uint32);

extern uint32 fbInOutputTypeCasting(uint32, uint32, uint32, uint32 *);
extern uint32 fbCodeVarWriteEnoRead(uint32 , uint32 , uint32* );

/*********************************************************************
**	
**	ABS FB 정의
**
**********************************************************************/
uint32	stdAbsInitFunc
				(
			uint32	*specTypePtr,			
            uint32	*varTypePtr,			
            uint32	*outputTypePtr,			 
            strFbDefInfo	*paraInfoPtr	
				)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= ABS_CODE;
    paraInfoPtr->inputNo	= ABS_SPEC_NUM;
    paraInfoPtr->intlVarNo	= ABS_VAR_NUM;
    paraInfoPtr->outputNo	= ABS_OUTPUT_NUM;

	/* 입력 - 1개 */
	*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;		

	/* 내부변수 - 0개 */		

    /* 출력 - 1개 */
	*outputTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;	

    return(status);
}

uint32	stdAbsRunFunc
				(
			uint32 taskId,		
			uint32 fbBlockAddr	
				)
{
	uint32	status 		= NO_ERROR;	
	uint32	paraRunType = 0;

	strFC0001Info fd;
	
    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (void *)&fd
						   );
	if(status != NO_ERROR) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }

	status = fbInOutputTypeCasting(LOGIC_ID, fbBlockAddr, ABS_REF_SINDEX, &paraRunType);
	if(status != NO_ERROR) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }

	paraRunType = GET_BASIC_DATA_TYPE(paraRunType);

	/* 알고리즘 연산부 */
	if(paraRunType == REAL_TYPE) 
	{
		if(fd.forReal.input >= 0.0) 
		{
			fd.forReal.output = fd.forReal.input;
		} 
		else 
		{
	 		fd.forReal.output = fd.forReal.input * (-1.0);
		}
	} 
	else if(paraRunType == INT_TYPE) 
	{
		/* INT32 타입일 경우 -2147483648의 절대값을 INT32 타입으로 표현할 수 없다. */
		if(fd.forInt.input == MIN_INT32_VALUE) 
		{
			fd.forInt.output = 0;
			
			status = FB_INPUT_RANGE_ERR;
			
			setErrorCodeWithVal(	__FILE__,__LINE__,__FUNCTION__, status,
									"taskId", taskId,
									"fbBlockAddr", fbBlockAddr,
									"NOT USING", 0);
			return(status);
		}
		else if(fd.forInt.input >= 0) 
		{
			fd.forInt.output = fd.forInt.input;
		} 
		else 
		{
	 		fd.forInt.output = fd.forInt.input * (-1.0);
		}
	} 
	else if(paraRunType == UINT_TYPE) 
	{
		fd.forUint.output = fd.forUint.input;
	} 
	else 
	{
		status = FB_INPUT_TYPE_ERR;
		setErrorCodeWithVal(	__FILE__,__LINE__,__FUNCTION__, status,
								"taskId", taskId,
								"fbBlockAddr", fbBlockAddr,
								"paraRunType", paraRunType);
		return (status);
	}

    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (void *)&fd
						     );
	if(status != NO_ERROR) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
    }

    return (status);
}

/*********************************************************************
**	
**	SQRT FB 정의
**
**********************************************************************/
uint32	stdSqrtInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)	
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= SQRT_CODE;
    paraInfoPtr->inputNo		= SQRT_SPEC_NUM;
    paraInfoPtr->intlVarNo		= SQRT_VAR_NUM;
    paraInfoPtr->outputNo	= SQRT_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	return (status);
}	 		 	

uint32	stdSqrtRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status 	= NO_ERROR; 

	strFC0023Info fd;

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
    
	/* Square Root 계산 */
	if(fd.input < 0.0)  
	{
		status = FB_INPUT_RANGE_ERR;
    	setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
    	return(status);
	} 
	else 
		fd.output = (float)sqrt((double)fd.input);
		
    	
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
**	exponential FB 정의
**
**********************************************************************/
uint32	stdExpInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= EXP_CODE;
    paraInfoPtr->inputNo		= EXP_SPEC_NUM;
    paraInfoPtr->intlVarNo		= EXP_VAR_NUM;
    paraInfoPtr->outputNo	= EXP_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= REAL_TYPE | SIZE32_TYPE;
	return (status);
}
	
uint32	stdExpRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
    uint32 status = NO_ERROR; 
    
    strFC0025Info fd;

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
    
	fd.output = (float)exp((double)fd.input);
		
    	
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
**	자연로그 FB 정의(밑이 e)
**
**********************************************************************/
uint32	stdLnInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= LN_CODE;
    paraInfoPtr->inputNo		= LN_SPEC_NUM;
    paraInfoPtr->intlVarNo		= LN_VAR_NUM;
    paraInfoPtr->outputNo	= LN_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= REAL_TYPE | SIZE32_TYPE;
	return (status);
}	

uint32	stdLnRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
    uint32 status = NO_ERROR; 
    
    strFC0026Info fd;

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
    
	fd.output = (float)log((double)fd.input);
		
    	
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
**	상용로그 FB 정의(밑이 10)
**
**********************************************************************/
uint32	stdLogInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= LOG_CODE;
    paraInfoPtr->inputNo		= LOG_SPEC_NUM;
    paraInfoPtr->intlVarNo		= LOG_VAR_NUM;
    paraInfoPtr->outputNo	= LOG_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= REAL_TYPE | SIZE32_TYPE;
	return (status);
}	

uint32	stdLogRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
    uint32 status = NO_ERROR; 
    
    strFC0027Info fd;

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
    
	fd.output = (float)log10((double)fd.input);
		
    	
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
**	arccosine FB 정의
**
**********************************************************************/
uint32	stdACosInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= ACOS_CODE;
    paraInfoPtr->inputNo		= ACOS_SPEC_NUM;
    paraInfoPtr->intlVarNo		= ACOS_VAR_NUM;
    paraInfoPtr->outputNo	= ACOS_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	return (status);
}	

uint32	stdACosRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
    uint32 status = NO_ERROR; 
    
    strFC0071Info fd;

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
	
    
	fd.output = (float)acos((double)fd.input);
		
    	
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
**	arcsine FB 정의
**
**********************************************************************/
uint32	stdAsinInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr){
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= ASIN_CODE;
    paraInfoPtr->inputNo		= ASIN_SPEC_NUM;
    paraInfoPtr->intlVarNo		= ASIN_VAR_NUM;
    paraInfoPtr->outputNo	= ASIN_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= REAL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= REAL_TYPE | SIZE32_TYPE;

	return (status);
}	

uint32	stdAsinRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
    uint32 status = NO_ERROR; 
    
    strFC0072Info fd;

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
    
	fd.output = (float)asin((double)fd.input);
		
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
**	arctangent FB 정의
**
**********************************************************************/
uint32	stdAtanInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= ATAN_CODE;
    paraInfoPtr->inputNo		= ATAN_SPEC_NUM;
    paraInfoPtr->intlVarNo		= ATAN_VAR_NUM;
    paraInfoPtr->outputNo	= ATAN_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	return (status);
}	

uint32	stdAtanRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
    uint32 status = NO_ERROR; 
    
    strFC0073Info fd;

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
    
	fd.output = (float)atan((double)fd.input);
		
    	
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
**	cosine FB 정의
**
**********************************************************************/
uint32	stdCosInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= COS_CODE;
    paraInfoPtr->inputNo		= COS_SPEC_NUM;
    paraInfoPtr->intlVarNo		= COS_VAR_NUM;
    paraInfoPtr->outputNo	= COS_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= REAL_TYPE | SIZE32_TYPE;
	return (status);
}	

uint32	stdCosRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
    uint32 status = NO_ERROR; 
    
    strFC0074Info fd;

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
    
	fd.output = (float)cos((double)fd.input);
		
    	
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
**	sine FB 정의
**
**********************************************************************/
uint32	stdSinInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= SIN_CODE;
    paraInfoPtr->inputNo		= SIN_SPEC_NUM;
    paraInfoPtr->intlVarNo		= SIN_VAR_NUM;
    paraInfoPtr->outputNo	= SIN_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= REAL_TYPE | SIZE32_TYPE;
	
	return (status);
}	
uint32	stdSinRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
    uint32 status = NO_ERROR;
    
    strFC0075Info fd; 

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
    
	fd.output = (float)sin((double)fd.input);
		
    	
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
**	tangent FB 정의
**
**********************************************************************/
uint32	stdTanInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= TAN_CODE;
    paraInfoPtr->inputNo		= TAN_SPEC_NUM;
    paraInfoPtr->intlVarNo		= TAN_VAR_NUM;
    paraInfoPtr->outputNo	= TAN_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= REAL_TYPE | SIZE32_TYPE;
	
	return (status);
}	

uint32	stdTanRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
    uint32 status = NO_ERROR; 
    
    strFC0076Info fd;

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
    
	fd.output = (float)tan((double)fd.input);
		
    	
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
