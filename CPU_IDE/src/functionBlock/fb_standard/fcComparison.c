#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_standard\fcComparison.h"

/**************************************************************************
**		외부 함수 
**************************************************************************/
extern uint32 readRuntimeFbData(uint32,uint32,uint32,uint32 *);
extern uint32 writeRuntimeFbData(uint32, uint32,uint32, uint32 *);
extern void	  setErrorCode(uint8 *, uint32, uint8 *, uint32);

/*********************************************************************
**	
**	2개 입력 equal 비교 FB 정의
**
**********************************************************************/
uint32	stdEq2InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= EQ2_CODE;
    paraInfoPtr->inputNo		= EQ2_SPEC_NUM;
    paraInfoPtr->intlVarNo		= EQ2_VAR_NUM;
    paraInfoPtr->outputNo	= EQ2_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < EQ2_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;
	return (status);
}		 		 		 	

uint32	stdEq2RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	strFC0081Info fd;
	
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
    
	if(fd.in[0] != fd.in[1]) 
	{
		fd.out = 0;
	} 
	else 
	{
		fd.out = 1;
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
**	4개 입력 equal 비교 FB 정의
**
**********************************************************************/
uint32	stdEq4InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= EQ4_CODE;
    paraInfoPtr->inputNo		= EQ4_SPEC_NUM;
    paraInfoPtr->intlVarNo		= EQ4_VAR_NUM;
    paraInfoPtr->outputNo	= EQ4_OUTPUT_NUM;

	/* 입력 - 4개 */
	for(iLoop = 0; iLoop < EQ4_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}		 	 	

uint32	stdEq4RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint32 	i;
	uint32	tempOutput = 1;

	strFC0083Info fd;

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
    
   	for(i = 0; i < (EQ4_SPEC_NUM - 1); i++)
	{
		if(fd.in[i] !=  fd.in[i+1])
		{	
			tempOutput = 0;
			break;
		}
	}
	
	fd.out = tempOutput;

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
**	8개 입력 equal 비교 FB 정의
**
**********************************************************************/
uint32	stdEq8InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= EQ8_CODE;
    paraInfoPtr->inputNo		= EQ8_SPEC_NUM;
    paraInfoPtr->intlVarNo		= EQ8_VAR_NUM;
    paraInfoPtr->outputNo	= EQ8_OUTPUT_NUM;

	/* 입력 - 8개 */
	for(iLoop = 0; iLoop < EQ8_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - 1개 */
	*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}	 	 	

uint32	stdEq8RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	uint32 i;
	uint32	tempOutput = 1;
	
	strFC0087Info fd;

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
    
   	for(i = 0; i < (EQ8_SPEC_NUM - 1); i++)
	{
		if(fd.in[i] !=  fd.in[i+1])
		{	
			tempOutput = 0;
			break;
		}
	}
	
	fd.out = tempOutput;

	
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
**	2개 입력 greater equal 비교 FB 정의
**
**********************************************************************/
uint32	stdGe2InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= GE2_CODE;
    paraInfoPtr->inputNo		= GE2_SPEC_NUM;
    paraInfoPtr->intlVarNo		= GE2_VAR_NUM;
    paraInfoPtr->outputNo	= GE2_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= ANY_NUM_TYPE | SIZE32_TYPE;
	*specTypePtr++			= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;
	return (status);
}	 	

uint32	stdGe2RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR;
	uint32 i;
	uint32 tempOutput = 1;

	strFC0089Info fd;
	
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
    
   	for(i = 0; i < (GE2_SPEC_NUM-1); i++)
	{
		if(fd.in[i] <  fd.in[i+1])
		{	
			tempOutput = 0;
			break;
		}
	}
	
	fd.out = tempOutput;
	

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
**	4개 입력 greater equal 비교 FB 정의
**
**********************************************************************/
uint32	stdGe4InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= GE4_CODE;
    paraInfoPtr->inputNo		= GE4_SPEC_NUM;
    paraInfoPtr->intlVarNo		= GE4_VAR_NUM;
    paraInfoPtr->outputNo	= GE4_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < GE4_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}		 	

uint32	stdGe4RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i; 
	uint32 	tempOutput = 1;
	
	strFC0091Info fd;
	
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
    
   	for(i = 0; i < (GE4_SPEC_NUM-1); i++)
	{
		if(fd.in[i]<fd.in[i+1])
		{	
			tempOutput = 0;
			break;
		}
	}
	
	fd.out = tempOutput;
	

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
**	8개 입력 greater equal 비교 FB 정의
**
**********************************************************************/
uint32	stdGe8InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= GE8_CODE;
    paraInfoPtr->inputNo		= GE8_SPEC_NUM;
    paraInfoPtr->intlVarNo		= GE8_VAR_NUM;
    paraInfoPtr->outputNo	= GE8_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < GE8_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}	 	

uint32	stdGe8RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i;  
	uint32 	tempOutput = 1;

	strFC0095Info fd;
	
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
    
   	for(i = 0;i < (GE8_SPEC_NUM-1); i++)
	{
		if(fd.in[i] < fd.in[i+1])
		{	
			tempOutput = 0;
			break;
		}
	}
	
	fd.out = tempOutput;
	

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
**	2개 입력 greater than 비교 FB 정의
**
**********************************************************************/
uint32	stdGt2InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= GT2_CODE;
    paraInfoPtr->inputNo		= GT2_SPEC_NUM;
    paraInfoPtr->intlVarNo		= GT2_VAR_NUM;
    paraInfoPtr->outputNo	= GT2_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= ANY_NUM_TYPE | SIZE32_TYPE;
	*specTypePtr++			= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}	 	

uint32	stdGt2RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i; 
	uint32 	tempOutput = 1;

	strFC0097Info fd;
	
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
    
   	for(i = 0;i < (GT2_SPEC_NUM-1); i++)
	{
		if(fd.in[i] <= fd.in[i+1])
		{	
			tempOutput = 0;
			break;
		}
	}
	
	fd.out = tempOutput;
	

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
**	4개 입력 greater than 비교 FB 정의
**
**********************************************************************/
uint32	stdGt4InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= GT4_CODE;
    paraInfoPtr->inputNo		= GT4_SPEC_NUM;
    paraInfoPtr->intlVarNo		= GT4_VAR_NUM;
    paraInfoPtr->outputNo	= GT4_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < GT4_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}		

uint32	stdGt4RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i; 
	uint32 	tempOutput = 1;

	strFC0099Info fd;
	
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
    
   	for(i = 0;i < (GT4_SPEC_NUM-1); i++)
	{
		if(fd.in[i] <= fd.in[i+1])
		{	
			tempOutput = 0;
			break;
		}
	}
	
	fd.out = tempOutput;
	
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

uint32	stdGt8InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= GT8_CODE;
    paraInfoPtr->inputNo		= GT8_SPEC_NUM;
    paraInfoPtr->intlVarNo		= GT8_VAR_NUM;
    paraInfoPtr->outputNo	= GT8_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < GT8_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}	 	

uint32	stdGt8RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i; 
	uint32 	tempOutput = 1;

	strFC0103Info fd;
	
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
    
   	for(i = 0;i < (GT8_SPEC_NUM-1); i++)
	{
		if(fd.in[i] <= fd.in[i+1])
		{	
			tempOutput = 0;
			break;
		}
	}
	
	fd.out = tempOutput;
	
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
**	2개 입력 Less or Equal 비교 FB 정의
**
**********************************************************************/
uint32	stdLe2InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= LE2_CODE;
    paraInfoPtr->inputNo		= LE2_SPEC_NUM;
    paraInfoPtr->intlVarNo		= LE2_VAR_NUM;
    paraInfoPtr->outputNo	= LE2_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;
	return (status);
}	 	

uint32	stdLe2RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i; 
	uint32 	tempOutput = 1;

	strFC0105Info fd;
	
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
    
   	for(i = 0;i < (LE2_SPEC_NUM-1); i++)
	{
		if(fd.in[i] > fd.in[i+1])
		{	
			tempOutput = 0;
			break;
		}
	}
	
	fd.out = tempOutput;
	
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


uint32	stdLe4InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= LE4_CODE;
    paraInfoPtr->inputNo		= LE4_SPEC_NUM;
    paraInfoPtr->intlVarNo		= LE4_VAR_NUM;
    paraInfoPtr->outputNo	= LE4_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < LE4_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}		 	

uint32	stdLe4RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i; 
	uint32 	tempOutput = 1;

	strFC0107Info fd;
	
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
    
   	for(i = 0;i < (LE4_SPEC_NUM-1); i++)
	{
		if(fd.in[i] > fd.in[i+1])
		{	
			tempOutput = 0;
			break;
		}
	}
	
	fd.out = tempOutput;
	
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
**	8개 입력 Less or Equal 비교 FB 정의
**
**********************************************************************/
uint32	stdLe8InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= LE8_CODE;
    paraInfoPtr->inputNo		= LE8_SPEC_NUM;
    paraInfoPtr->intlVarNo		= LE8_VAR_NUM;
    paraInfoPtr->outputNo	= LE8_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < LE8_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}	 	

uint32	stdLe8RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i; 
	uint32 	tempOutput = 1;

	strFC0111Info fd;
	
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
    
   	for(i = 0;i < (LE8_SPEC_NUM-1); i++)
	{
		if(fd.in[i] > fd.in[i+1])
		{	
			tempOutput = 0;
			break;
		}
	}
	
	fd.out = tempOutput;

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
**	2개 입력 Less than 비교 FB 정의
**
**********************************************************************/
uint32	stdLt2InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= LT2_CODE;
    paraInfoPtr->inputNo		= LT2_SPEC_NUM;
    paraInfoPtr->intlVarNo		= LT2_VAR_NUM;
    paraInfoPtr->outputNo	= LT2_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}	

uint32	stdLt2RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i; 
	uint32 	tempOutput = 1;

	strFC0113Info fd;
	
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
    
   	for(i = 0;i < (LT2_SPEC_NUM-1); i++)
	{
		if(fd.in[i] >= fd.in[i+1])
		{	
			tempOutput = 0;
			break;
		}
	}
	
	fd.out = tempOutput;
	
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
**	4개 입력 Less than 비교 FB 정의
**
**********************************************************************/
uint32	stdLt4InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32 	iLoop;

	paraInfoPtr->fbId		= LT4_CODE;
    paraInfoPtr->inputNo		= LT4_SPEC_NUM;
    paraInfoPtr->intlVarNo		= LT4_VAR_NUM;
    paraInfoPtr->outputNo	= LT4_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < LT4_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}		 	

uint32	stdLt4RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i; 
	uint32 	tempOutput = 1;

	strFC0115Info fd;
	
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
    
   	for(i = 0;i < (LT4_SPEC_NUM-1); i++)
	{
		if(fd.in[i] >= fd.in[i+1])
		{	
			tempOutput = 0;
			break;
		}
	}
	
	fd.out = tempOutput;
	

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
**	8개 입력 Less than 비교 FB 정의
**
**********************************************************************/
uint32	stdLt8InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32 	iLoop;
	
	paraInfoPtr->fbId		= LT8_CODE;
    paraInfoPtr->inputNo		= LT8_SPEC_NUM;
    paraInfoPtr->intlVarNo		= LT8_VAR_NUM;
    paraInfoPtr->outputNo	= LT8_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < LT8_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}	 	

uint32	stdLt8RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i; 
	uint32 	tempOutput = 1;

	strFC0119Info fd;
	
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
    
   	for(i = 0;i < (LT8_SPEC_NUM-1); i++)
	{
		if(fd.in[i] >= fd.in[i+1])
		{	
			tempOutput = 0;
			break;
		}
	}
	
	fd.out = tempOutput;
	
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
**	2개 입력 Not Equal 비교 FB 정의
**
**********************************************************************/
uint32	stdNe2InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= NE2_CODE;
    paraInfoPtr->inputNo		= NE2_SPEC_NUM;
    paraInfoPtr->intlVarNo		= NE2_VAR_NUM;
    paraInfoPtr->outputNo	= NE2_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;
	return (status);
}	 	

uint32	stdNe2RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i,j; 
	uint32 	tempOutput = 1;

	strFC0121Info fd;
	
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
    
	for(i = 0; i < (NE2_SPEC_NUM-1); i++)
	{
		for(j = (i+1); j < NE2_SPEC_NUM; j++) 
		{
			if(fd.in[i] == fd.in[j]) 
			{
				tempOutput = 0;
				break;
			}
		}
	}	
	
	fd.out = tempOutput;
	

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
**	4개 입력 Not Equal 비교 FB 정의
**
**********************************************************************/
uint32	stdNe4InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= NE4_CODE;
    paraInfoPtr->inputNo		= NE4_SPEC_NUM;
    paraInfoPtr->intlVarNo		= NE4_VAR_NUM;
    paraInfoPtr->outputNo	= NE4_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < NE4_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}		 	

uint32	stdNe4RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i,j; 
	uint32 	tempOutput = 1;

	strFC0123Info fd;
	
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
    
	for(i = 0; i < (NE4_SPEC_NUM-1); i++)
	{
		for(j = (i+1); j < NE4_SPEC_NUM; j++) 
		{
			if(fd.in[i] == fd.in[j]) 
			{
				tempOutput = 0;
				break;
			}
		}
	}	
	
	fd.out = tempOutput;
	
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
**	8개 입력 Not Equal 비교 FB 정의
**
**********************************************************************/
uint32	stdNe8InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= NE8_CODE;
    paraInfoPtr->inputNo		= NE8_SPEC_NUM;
    paraInfoPtr->intlVarNo		= NE8_VAR_NUM;
    paraInfoPtr->outputNo	= NE8_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < NE8_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}	 	

uint32	stdNe8RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i,j; 
	uint32 	tempOutput = 1;

	strFC0127Info fd;
	
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
   
	for(i = 0; i < (NE8_SPEC_NUM-1); i++)
	{
		for(j = (i+1); j < NE8_SPEC_NUM; j++) 
		{
			if(fd.in[i] == fd.in[j]) 
			{
				tempOutput = 0;
				break;
			}
		}
	}	
	
	fd.out = tempOutput;
	
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
