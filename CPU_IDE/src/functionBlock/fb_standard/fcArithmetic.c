#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_standard\fcArithmetic.h"

/**************************************************************************
**		외부 함수 
**************************************************************************/
extern uint32 readRuntimeFbData(uint32,uint32,uint32,uint32 *);
extern uint32 writeRuntimeFbData(uint32, uint32,uint32, uint32 *);
extern void	  setErrorCode(uint8 *, uint32, uint8 *, uint32);
extern void   setErrorCodeWithVal(uint8 *, uint32, uint8 *, uint32,
				uint8 *, uint32, uint8 *, uint32, uint8 *, uint32);

extern uint32 fbInOutputTypeCasting (uint32 , uint32 , uint32 , uint32 *);

/*********************************************************************
**	
**	Bit Rotation Left FB 정의
**
**********************************************************************/
uint32	stdMoveInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= MOVE_CODE;
    paraInfoPtr->inputNo	= MOVE_SPEC_NUM;
    paraInfoPtr->intlVarNo	= MOVE_VAR_NUM;
    paraInfoPtr->outputNo	= MOVE_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= ANY_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= ANY_TYPE | SIZE32_TYPE;
	
	return (status);
}

uint32	stdMoveRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	
	strFC0013Info	fd;			

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
	fourBytesDataCopy((uint32 *)&fd.output,(uint32 *)&fd.input,1);

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
**	2개 입력 ADD FB 정의
**
**********************************************************************/
uint32	stdAdd2InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= ADD2_CODE;
    paraInfoPtr->inputNo	= ADD2_SPEC_NUM;
    paraInfoPtr->intlVarNo	= ADD2_VAR_NUM;
    paraInfoPtr->outputNo	= ADD2_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < ADD2_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	return (status);
}

uint32	stdAdd2RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	paraRunType = 0;

	strFC0002Info fd;

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

	status = fbInOutputTypeCasting(LOGIC_ID, fbBlockAddr, ADD2_REF_SINDEX, &paraRunType);
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	paraRunType = GET_BASIC_DATA_TYPE(paraRunType);
	
	if(paraRunType == REAL_TYPE) 
	{
		fd.forReal.output = fd.forReal.input+fd.forReal.input2;
	}
	
	else if(paraRunType == INT_TYPE) 
	{
		fd.forInt.output = fd.forInt.input+fd.forInt.input2;
	}
	
	else if(paraRunType == UINT_TYPE) 
	{
		fd.forUint.output = fd.forUint.input+fd.forUint.input2;
	}
	else {
		status = FB_INPUT_TYPE_ERR;
		setErrorCodeWithVal(	__FILE__,
								__LINE__,
								"FC0002: FB_INPUT_TYPE_ERR", status,
								"taskId", taskId,
								"fbBlockAddr", fbBlockAddr,
								"paraRunType", paraRunType);
		return(status);
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
**	4개 입력 ADD FB 정의
**
**********************************************************************/
uint32	stdAdd4InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= ADD4_CODE;
    paraInfoPtr->inputNo		= ADD4_SPEC_NUM;
    paraInfoPtr->intlVarNo		= ADD4_VAR_NUM;
    paraInfoPtr->outputNo	= ADD4_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < ADD4_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;
	
	return (status);
}

uint32	stdAdd4RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	paraRunType = 0;

	strFC0004Info fd;
	
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

	status = fbInOutputTypeCasting(LOGIC_ID, fbBlockAddr, ADD4_REF_SINDEX, &paraRunType);
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	paraRunType = GET_BASIC_DATA_TYPE(paraRunType);
	
	if(paraRunType==REAL_TYPE) 
	{
		fd.forReal.output = fd.forReal.input+fd.forReal.input2+fd.forReal.input3+fd.forReal.input4;
	}
	else if(paraRunType==INT_TYPE) 
	{
		fd.forInt.output = fd.forInt.input+fd.forInt.input2+fd.forInt.input3+fd.forInt.input4;
	}
	else if(paraRunType==UINT_TYPE) 
	{
		fd.forUint.output = fd.forUint.input+fd.forUint.input2+fd.forUint.input3+fd.forUint.input4;
	}
	else 
	{
		status = FB_INPUT_TYPE_ERR;
		setErrorCodeWithVal(	__FILE__,
								__LINE__,
								"FC0004: FB_INPUT_TYPE_ERR", status,
								"taskId", taskId,
								"fbBlockAddr", fbBlockAddr,
								"paraRunType", paraRunType);
		return(status);
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
**	8개 입력 ADD FB 정의
**
**********************************************************************/
uint32	stdAdd8InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= ADD8_CODE;
    paraInfoPtr->inputNo		= ADD8_SPEC_NUM;
    paraInfoPtr->intlVarNo		= ADD8_VAR_NUM;
    paraInfoPtr->outputNo	= ADD8_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < ADD8_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;
	
	return (status);
}

uint32	stdAdd8RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	paraRunType = 0;

	strFC0008Info	fd;
	
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

	status = fbInOutputTypeCasting(LOGIC_ID, fbBlockAddr, ADD8_REF_SINDEX, &paraRunType);
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	paraRunType = GET_BASIC_DATA_TYPE(paraRunType);
	
	if(paraRunType == REAL_TYPE) 
	{
		fd.forReal.output = fd.forReal.input+fd.forReal.input2+fd.forReal.input3+fd.forReal.input4+fd.forReal.input5+fd.forReal.input6+fd.forReal.input7+fd.forReal.input8;
	}
	else if(paraRunType == INT_TYPE) 
	{
		fd.forInt.output = fd.forInt.input+fd.forInt.input2+fd.forInt.input3+fd.forInt.input4+fd.forInt.input5+fd.forInt.input6+fd.forInt.input7+fd.forInt.input8;
	}
	else if(paraRunType == UINT_TYPE) 
	{
		fd.forUint.output = fd.forUint.input+fd.forUint.input2+fd.forUint.input3+fd.forUint.input4+fd.forUint.input5+fd.forUint.input6+fd.forInt.input7+fd.forInt.input8;
	}
	else 
	{
		status = FB_INPUT_TYPE_ERR;
		setErrorCodeWithVal(	__FILE__,
								__LINE__,
								"FC0008: FB_INPUT_TYPE_ERR", status,
								"taskId", taskId,
								"fbBlockAddr", fbBlockAddr,
								"paraRunType", paraRunType);
		return(status);
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
**	2개 입력 Multiply FB 정의
**
**********************************************************************/
uint32	stdMul2InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= MUL2_CODE;
    paraInfoPtr->inputNo		= MUL2_SPEC_NUM;
    paraInfoPtr->intlVarNo		= MUL2_VAR_NUM;
    paraInfoPtr->outputNo	= MUL2_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < MUL2_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;
	
	return (status);
}	 	

uint32	stdMul2RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint32	paraRunType = 0;
	
	strFC0014Info	fd;
	
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

	status = fbInOutputTypeCasting(	LOGIC_ID, fbBlockAddr, MUL2_REF_SINEX, &paraRunType);
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	paraRunType = GET_BASIC_DATA_TYPE(paraRunType);

	/* 알고리즘 연산부 */
	if(paraRunType == REAL_TYPE) 
	{
		fd.forReal.output = fd.forReal.input*fd.forReal.input2;
	} 
	else if(paraRunType == INT_TYPE) 
	{

		fd.forInt.output = fd.forInt.input*fd.forInt.input2;
	} 
	else if(paraRunType == UINT_TYPE) 
	{
		fd.forUint.output = fd.forUint.input*fd.forUint.input2;
	} 
	else 
	{
		status = FB_INPUT_TYPE_ERR;
		setErrorCodeWithVal(	__FILE__,
								__LINE__,
								"FC0014: FB_INPUT_TYPE_ERR", status,
								"taskId", taskId,
								"fbBlockAddr", fbBlockAddr,
								"paraRunType", paraRunType);

		return(status);
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
**	4개 입력 Multiply FB 정의
**
**********************************************************************/
uint32	stdMul4InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= MUL4_CODE;
    paraInfoPtr->inputNo		= MUL4_SPEC_NUM;
    paraInfoPtr->intlVarNo		= MUL4_VAR_NUM;
    paraInfoPtr->outputNo	= MUL4_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < MUL4_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;


	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;
	
	return (status);
}	 	

uint32	stdMul4RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	paraRunType = 0;
	
	strFC0016Info fd;
	
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
	
	status = fbInOutputTypeCasting(	LOGIC_ID, fbBlockAddr, MUL4_REF_SINEX, &paraRunType);
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	paraRunType = GET_BASIC_DATA_TYPE(paraRunType);

	/* 알고리즘 연산부 */
	if(paraRunType == REAL_TYPE) 
	{
		fd.forReal.output = fd.forReal.input*fd.forReal.input2*fd.forReal.input3*fd.forReal.input4;
	} 
	else if(paraRunType == INT_TYPE) 
	{
		fd.forInt.output = fd.forInt.input*fd.forInt.input2*fd.forInt.input3*fd.forInt.input4;
	} 
	else if(paraRunType == UINT_TYPE) 
	{
		fd.forUint.output = fd.forUint.input*fd.forUint.input2*fd.forUint.input3*fd.forUint.input4;
	} 
	else 
	{
		status = FB_INPUT_TYPE_ERR;
		setErrorCodeWithVal(	__FILE__,
								__LINE__,
								"FC0016: FB_INPUT_TYPE_ERR", status,
								"taskId", taskId,
								"fbBlockAddr", fbBlockAddr,
								"paraRunType", paraRunType);

		return(status);
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
**	8개 입력 Multiply FB 정의
**
**********************************************************************/
uint32	stdMul8InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= MUL8_CODE;
    paraInfoPtr->inputNo		= MUL8_SPEC_NUM;
    paraInfoPtr->intlVarNo		= MUL8_VAR_NUM;
    paraInfoPtr->outputNo	= MUL8_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < MUL8_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;
	
	return (status);
}	 		 	

uint32	stdMul8RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	paraRunType = 0;
	
	strFC0020Info fd;
	
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
	
	status = fbInOutputTypeCasting(	LOGIC_ID, fbBlockAddr, MUL8_REF_SINEX, &paraRunType);
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	paraRunType = GET_BASIC_DATA_TYPE(paraRunType);

	/* 알고리즘 연산부 */
	if(paraRunType == REAL_TYPE) 
	{
		fd.forReal.output = fd.forReal.input*fd.forReal.input2*fd.forReal.input3*fd.forReal.input4
							*fd.forReal.input5*fd.forReal.input6*fd.forReal.input7*fd.forReal.input8;
	} 
	else if(paraRunType == INT_TYPE) 
	{
		fd.forInt.output = fd.forInt.input*fd.forInt.input2*fd.forInt.input3*fd.forInt.input4
							*fd.forInt.input5*fd.forInt.input6*fd.forInt.input7*fd.forInt.input8;
	} 
	else if(paraRunType == UINT_TYPE) 
	{
		fd.forUint.output = fd.forUint.input*fd.forUint.input2*fd.forUint.input3*fd.forUint.input4
							*fd.forUint.input5*fd.forUint.input6*fd.forUint.input7*fd.forUint.input8;
	} 
	else 
	{
		status = FB_INPUT_TYPE_ERR;
		setErrorCodeWithVal(	__FILE__,
								__LINE__,
								"FC0020: FB_INPUT_TYPE_ERR", status,
								"taskId", taskId,
								"fbBlockAddr", fbBlockAddr,
								"paraRunType", paraRunType);

		return(status);
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


uint32	stdMulDivInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)	
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= MULDIV_CODE;
    paraInfoPtr->inputNo		= MULDIV_SPEC_NUM;
    paraInfoPtr->intlVarNo		= MULDIV_VAR_NUM;
    paraInfoPtr->outputNo	= MULDIV_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < MULDIV_SPEC_NUM; iLoop++)
		*specTypePtr++	= INT_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	for(iLoop = 0; iLoop < MULDIV_OUTPUT_NUM; iLoop++)
	*outputTypePtr++	= INT_TYPE | SIZE32_TYPE;

	return (status);
}	 		 	

uint32	stdMulDivRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	 
	strFC0022Info fd;

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
	
	if(fd.dividor == 0)
	{
   		status = FB_INPUT_RANGE_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}
	else
	{
        fd.quotient = (fd.input * fd.multiplier) / fd.dividor;
        fd.remainder = (fd.input * fd.multiplier) % fd.dividor;
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
**	 Subtraction FB 정의
**
**********************************************************************/
uint32	stdSubInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)	
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= SUB_CODE;
    paraInfoPtr->inputNo		= SUB_SPEC_NUM;
    paraInfoPtr->intlVarNo		= SUB_VAR_NUM;
    paraInfoPtr->outputNo	= SUB_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= ANY_NUM_TYPE | SIZE32_TYPE;
	*specTypePtr++			= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= ANY_NUM_TYPE | SIZE32_TYPE;
	
	return (status);
}	 		 	

uint32	stdSubRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint32	paraRunType = 0;

	strFC0024Info fd;

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
	
	status = fbInOutputTypeCasting(LOGIC_ID, fbBlockAddr, SUB_REF_SINDEX, &paraRunType);
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	paraRunType = GET_BASIC_DATA_TYPE(paraRunType);

	/* 알고리즘 연산부 */
	if(paraRunType == REAL_TYPE) 
	{
		fd.forReal.output = fd.forReal.input-fd.forReal.input2;
	}
	else if(paraRunType == INT_TYPE) 
	{
		fd.forInt.output = fd.forInt.input-fd.forInt.input2;
	}
	else if(paraRunType == UINT_TYPE) 
	{
		fd.forUint.output = fd.forUint.input-fd.forUint.input2;
	}
	else 
	{
		status = FB_INPUT_TYPE_ERR;
		setErrorCodeWithVal(	__FILE__,
								__LINE__,
								"FC0024: FB_INPUT_TYPE_ERR", status,
								"taskId", taskId,
								"fbBlockAddr", fbBlockAddr,
								"paraRunType", paraRunType);

		return(status);
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
**	 Division FB 정의
**
**********************************************************************/
uint32	stdDivInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= DIV_CODE;
    paraInfoPtr->inputNo		= DIV_SPEC_NUM;
    paraInfoPtr->intlVarNo		= DIV_VAR_NUM;
    paraInfoPtr->outputNo	= DIV_OUTPUT_NUM;

	/* 스펙 - 2개 */
	*specTypePtr++			= ANY_NUM_TYPE | SIZE32_TYPE;	/* S01 */
	*specTypePtr++			= ANY_NUM_TYPE | SIZE32_TYPE;	/* S02 */


	/* 내부변수 - 0개 */		

    /* 출력 - 1개 */
	*outputTypePtr++		= ANY_NUM_TYPE | SIZE32_TYPE;	/* N01 */

	return (status);
}

uint32	stdDivRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint32	paraRunType = 0;

	strFC0010Info fd;
	
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

	status = fbInOutputTypeCasting(LOGIC_ID, fbBlockAddr, DIV_REF_SINDEX, &paraRunType);
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	paraRunType = GET_BASIC_DATA_TYPE(paraRunType);

	/* 알고리즘 연산부 */
	if(paraRunType == REAL_TYPE) 
	{
		if(fd.forReal.dividor == 0.0) 
		{
			status = FB_INPUT_RANGE_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}

		fd.forReal.quotient = fd.forReal.input/fd.forReal.dividor;
	} 
	else if(paraRunType == INT_TYPE) 
	{
		if(fd.forInt.dividor == 0) 
		{
			status = FB_INPUT_RANGE_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}

		fd.forInt.quotient = fd.forInt.input/fd.forInt.dividor;
	}
	else if(paraRunType == UINT_TYPE) 
	{
		if(fd.forUint.dividor == 0) 
		{
			status = FB_INPUT_RANGE_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}

		fd.forUint.quotient = fd.forUint.input/fd.forUint.dividor;
	} 
	else 
	{
		status = FB_INPUT_TYPE_ERR;
		setErrorCodeWithVal(	__FILE__,
								__LINE__,
								"HVDCFC0010: FB_INPUT_TYPE_ERR", status,
								"taskId", taskId,
								"fbBlockAddr", fbBlockAddr,
								"paraRunType", paraRunType);
		return(status);
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
**	 MOdulo FB 정의
**
**********************************************************************/
uint32	stdModInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= MOD_CODE;
    paraInfoPtr->inputNo	= MOD_SPEC_NUM;
    paraInfoPtr->intlVarNo	= MOD_VAR_NUM;
    paraInfoPtr->outputNo	= MOD_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= ANY_INT_TYPE | SIZE32_TYPE;
	*specTypePtr++			= ANY_INT_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= ANY_INT_TYPE | SIZE32_TYPE;
	
	return (status);
}

uint32	stdModRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint32	paraRunType = 0;

	strFC0012Info fd;
	
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

	status = fbInOutputTypeCasting(LOGIC_ID, fbBlockAddr, MOD_REF_SINDEX, &paraRunType);

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	paraRunType = GET_BASIC_DATA_TYPE(paraRunType);

	/* 알고리즘 연산부 */
	if(paraRunType == INT_TYPE) 
	{

		if(fd.forInt.dividor == 0) 
		{
			status = FB_INPUT_RANGE_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}

		fd.forInt.remainder = fd.forInt.input%fd.forInt.dividor;
	}
	else if(paraRunType == UINT_TYPE) 
	{

		if(fd.forUint.dividor == 0) 
		{
			status = FB_INPUT_RANGE_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}

		fd.forUint.remainder = fd.forUint.input%fd.forUint.dividor;
	}
	else 
	{
		status = FB_INPUT_TYPE_ERR;
		setErrorCodeWithVal(	__FILE__, __LINE__, __FUNCTION__, status,
								"taskId", taskId,
								"fbBlockAddr", fbBlockAddr,
								"paraRunType", paraRunType);

		return(status);
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
**	 MOdulo FB 정의
**
**********************************************************************/
uint32	stdExptInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= EXPT_CODE;
    paraInfoPtr->inputNo	= EXPT_SPEC_NUM;
    paraInfoPtr->intlVarNo	= EXPT_VAR_NUM;
    paraInfoPtr->outputNo	= EXPT_OUTPUT_NUM;

	/* 입력 - 2개 */
	*specTypePtr++			= REAL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= ANY_NUM_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - 2개 */
	*outputTypePtr++		= REAL_TYPE | SIZE32_TYPE;
	
	return (status);
}

uint32	stdExptRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint32	paraRunType = 0;

	strFC0011Info fd;
	
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

	status = fbInOutputTypeCasting(LOGIC_ID, fbBlockAddr, EXPT_REF_SINDEX, &paraRunType);

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	paraRunType = GET_BASIC_DATA_TYPE(paraRunType);

	/* 알고리즘 연산부 */
	if(paraRunType == REAL_TYPE) 
	{
		fd.forReal.output = (float)pow((double)fd.forReal.input,(double)fd.forReal.input2);
	}
	else if(paraRunType == INT_TYPE) 
	{
		fd.forInt.output = (float)pow((double)fd.forInt.input,(double)fd.forInt.input2);
	}
	else if(paraRunType == UINT_TYPE) 
	{
		fd.forUint.output = (float)pow((double)fd.forUint.input,(double)fd.forUint.input2);
	}
	else {
		status = FB_INPUT_TYPE_ERR;
		setErrorCodeWithVal(	__FILE__,
								__LINE__,
								"FC0011: FB_INPUT_TYPE_ERR", status,
								"taskId", taskId,
								"fbBlockAddr", fbBlockAddr,
								"paraRunType", paraRunType);

		return(status);
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
