#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_standard\fcBitshift.h"

/**************************************************************************
**		외부 함수 
**************************************************************************/
extern uint32 readRuntimeFbData(uint32,uint32,uint32,uint32 *);
extern uint32 writeRuntimeFbData(uint32, uint32,uint32, uint32 *);
extern void	  setErrorCode(uint8 *, uint32, uint8 *, uint32);

extern uint32 fbInOutputTypeCasting (uint32 , uint32 , uint32 , uint32 *);

/*********************************************************************
**	
**	Bit Rotation Left FB 정의
**
**********************************************************************/
uint32	stdRolInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= ROL_CODE;
    paraInfoPtr->inputNo		= ROL_SPEC_NUM;
    paraInfoPtr->intlVarNo		= ROL_VAR_NUM;
    paraInfoPtr->outputNo	= ROL_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;
	*specTypePtr++			= ANY_BIT_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= ANY_BIT_TYPE | SIZE32_TYPE;
	
	return (status);
}	

uint32	stdRolRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint32	outputTypeAndSize = 0;
	uint32	outputSize = 0;

	strFC0048Info fd;

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
    
	status = fbInOutputTypeCasting(LOGIC_ID, fbBlockAddr, ROL_REF_SINDEX, &outputTypeAndSize);

	if(status) 
	{
    	setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	outputSize = GET_TYPE_DATA_SIZE(outputTypeAndSize);
	
	/* 알고리즘 연산부 */
	switch(outputSize) 
	{
		case SIZE01_TYPE:
			if( (fd.bitRotationCnt < 0) || (fd.bitRotationCnt > 1) ) 
			{
				status = FB_INPUT_RANGE_ERR;
		    	setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
				return(status);
			}
		
			fd.output = (0x1&(fd.input<<fd.bitRotationCnt)) | ((0x1&fd.input)>>(1-fd.bitRotationCnt));
		break;
		
		case SIZE08_TYPE:
			if( (fd.bitRotationCnt<0) || (fd.bitRotationCnt>8) ) 
			{
				status = FB_INPUT_RANGE_ERR;
		    	setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
				return(status);
			}
		
			fd.output = (0xFF&(fd.input<<fd.bitRotationCnt)) | ((0xFF&fd.input)>>(8-fd.bitRotationCnt));
		break;
		
		case SIZE16_TYPE:
			if( (fd.bitRotationCnt<0) || (fd.bitRotationCnt>16) ) 
			{
				status = FB_INPUT_RANGE_ERR;
		    	setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
				return(status);
			}
		
			fd.output = (0xFFFF&(fd.input<<fd.bitRotationCnt)) | ((0xFFFF&fd.input)>>(16-fd.bitRotationCnt));
		break;
		
		case SIZE32_TYPE:
			if( (fd.bitRotationCnt<0) || (fd.bitRotationCnt>32) ) 
			{
				status = FB_INPUT_RANGE_ERR;
		    	setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
				return(status);
			}
			
			fd.output = (0xFFFFFFFF&(fd.input<<fd.bitRotationCnt)) | ((0xFFFFFFFF&fd.input)>>(32-fd.bitRotationCnt));
		
		break;
		
		default:
			status = FB_OUTPUT_DATA_SIZE_ERR;
	    	setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
			
		break;
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
**	Bit Rotation Right FB 정의
**
**********************************************************************/
uint32	stdRorInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)	
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= ROR_CODE;
    paraInfoPtr->inputNo		= ROR_SPEC_NUM;
    paraInfoPtr->intlVarNo		= ROR_VAR_NUM;
    paraInfoPtr->outputNo	= ROR_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;
	*specTypePtr++			= ANY_BIT_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= ANY_BIT_TYPE | SIZE32_TYPE;
	
	return (status);
}	 	 	

uint32	stdRorRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint32	outputTypeAndSize = 0;
	uint32	outputSize = 0;

	strFC0049Info fd;

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

	status = fbInOutputTypeCasting(LOGIC_ID, fbBlockAddr, ROR_REF_SINDEX, &outputTypeAndSize);

    if(status) 
    {
    	setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
    	return(status);
    }

	outputSize = GET_TYPE_DATA_SIZE(outputTypeAndSize);
	
	/* 알고리즘 연산부 */

	switch(outputSize) 
	{
		case SIZE01_TYPE:
			if( (fd.bitRotationCnt<0) || (fd.bitRotationCnt>1) ) 
			{
				status = FB_INPUT_RANGE_ERR;
				setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
				return(status);
			}
	
			fd.output = (0x1&(fd.input>>fd.bitRotationCnt)) | ((0x1&fd.input)<<(1-fd.bitRotationCnt));
		break;
	
		case SIZE08_TYPE:
			if( (fd.bitRotationCnt<0) || (fd.bitRotationCnt>8) ) 
			{
				status = FB_INPUT_RANGE_ERR;
				setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
				return(status);
	    	}
	
			fd.output = (0xFF&(fd.input>>fd.bitRotationCnt)) | ((0xFF&fd.input)<<(8-fd.bitRotationCnt));
		break;
	
		case SIZE16_TYPE:
			if( (fd.bitRotationCnt<0) || (fd.bitRotationCnt>16) ) 
			{
				status = FB_INPUT_RANGE_ERR;
				setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
				return(status);
			}
	
			fd.output = (0xFFFF&(fd.input>>fd.bitRotationCnt)) | ((0xFFFF&fd.input)<<(16-fd.bitRotationCnt));
		break;
	
		case SIZE32_TYPE:
			if( (fd.bitRotationCnt<0) || (fd.bitRotationCnt>32) ) 
			{
				status = FB_INPUT_RANGE_ERR;
				setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
				return(status);
			}
	
			fd.output = (0xFFFFFFFF&(fd.input>>fd.bitRotationCnt)) | ((0xFFFFFFFF&fd.input)<<(32-fd.bitRotationCnt));
	
		break;
	
		default:
			status = FB_OUTPUT_DATA_SIZE_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
	
			break;
	}

    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(taskId,
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
**	Bit Shift Left FB 정의
**
**********************************************************************/
uint32	stdShlInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)	
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= SHL_CODE;
    paraInfoPtr->inputNo		= SHL_SPEC_NUM;
    paraInfoPtr->intlVarNo		= SHL_VAR_NUM;
    paraInfoPtr->outputNo	= SHL_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;
	*specTypePtr++			= ANY_BIT_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= ANY_BIT_TYPE | SIZE32_TYPE;
	
	return (status);
}	 	 	

uint32	stdShlRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 

	strFC0050Info fd;
	
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
	if((fd.bitShiftCnt<0) || (fd.bitShiftCnt>32)) 
	{
		status = FB_INPUT_RANGE_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	fd.output = fd.input<<fd.bitShiftCnt;

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
**	Bit Shift Right FB 정의
**
**********************************************************************/
uint32	stdShrInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)	
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= SHR_CODE;
    paraInfoPtr->inputNo		= SHR_SPEC_NUM;
    paraInfoPtr->intlVarNo		= SHR_VAR_NUM;
    paraInfoPtr->outputNo	= SHR_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;
	*specTypePtr++			= ANY_BIT_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= ANY_BIT_TYPE | SIZE32_TYPE;
	
	return (status);
}	 	 	

uint32	stdShrRunFunc	(uint32 taskId, uint32 fbBlockAddr) 
{
	uint32 	status = NO_ERROR; 
	
	strFC0051Info fd;

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

	if((fd.bitShiftCnt<0) || (fd.bitShiftCnt>32)) 
	{
		status = FB_INPUT_RANGE_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	fd.output = fd.input>>fd.bitShiftCnt;

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
