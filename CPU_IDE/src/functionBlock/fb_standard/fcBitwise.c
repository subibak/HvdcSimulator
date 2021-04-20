#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_standard\fcBitwise.h"

/**************************************************************************
**		외부 함수 
**************************************************************************/
extern uint32 readRuntimeFbData(uint32,uint32,uint32,uint32 *);
extern uint32 writeRuntimeFbData(uint32, uint32,uint32, uint32 *);
extern void	  setErrorCode(uint8 *, uint32, uint8 *, uint32);

extern uint32 fbInOutputTypeCasting (uint32 , uint32 , uint32 , uint32 *);

/*********************************************************************
**	
**	2개 입력 AND FB 정의
**
**********************************************************************/
uint32	stdAnd2InitFunc
				(
			uint32	*specTypePtr,			
            uint32	*varTypePtr,			
            uint32	*outputTypePtr,			 
            strFbDefInfo	*paraInfoPtr	
				)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= AND2_CODE;
    paraInfoPtr->inputNo		= AND2_SPEC_NUM;
    paraInfoPtr->intlVarNo		= AND2_VAR_NUM;
    paraInfoPtr->outputNo	= AND2_OUTPUT_NUM;

	/* 스펙 - 2개 */
	for(iLoop = 0; iLoop < AND2_SPEC_NUM; iLoop++)
		*specTypePtr++		= ANY_BIT_TYPE | SIZE32_TYPE;	

	/* 내부변수 - 0개 */		

    /* 출력 - 1개 */
	*outputTypePtr++		= ANY_BIT_TYPE | SIZE32_TYPE;	/* N01 */
	
    return(status);
}

uint32	stdAnd2RunFunc
				(
			uint32 taskId,		
			uint32 fbBlockAddr	
				)
{
	uint32	status = NO_ERROR;	
	uint32	iLoop;
	
	strFC0031Info	fd;			

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
    fd.output = 1;
    
    for(iLoop = 0; iLoop < AND2_SPEC_NUM; iLoop++)
		fd.output &= fd.input[iLoop];

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
**	4개 입력 AND FB 정의
**
**********************************************************************/
uint32	stdAnd4InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)	
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= AND4_CODE;
    paraInfoPtr->inputNo		= AND4_SPEC_NUM;
    paraInfoPtr->intlVarNo		= AND4_VAR_NUM;
    paraInfoPtr->outputNo	= AND4_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < AND4_SPEC_NUM; iLoop++)
		*specTypePtr++		= ANY_BIT_TYPE | SIZE32_TYPE;	

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= ANY_BIT_TYPE | SIZE32_TYPE;
	
	return (status);
}		 	

uint32	stdAnd4RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint32	iLoop;

	strFC0033Info fd;
	

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
    fd.output = 1;
    
    for(iLoop = 0; iLoop < AND4_SPEC_NUM; iLoop++)
		fd.output &= fd.input[iLoop];
		
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
**	8개 입력 AND FB 정의
**
**********************************************************************/
uint32	stdAnd8InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)	
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= AND8_CODE;
    paraInfoPtr->inputNo		= AND8_SPEC_NUM;
    paraInfoPtr->intlVarNo		= AND8_VAR_NUM;
    paraInfoPtr->outputNo	= AND8_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < AND8_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_BIT_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}	 		 	 	

uint32	stdAnd8RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint32	iLoop;
	
	strFC0037Info fd;

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
    fd.output = 1;
    
    for(iLoop = 0; iLoop < AND8_SPEC_NUM; iLoop++)
		fd.output &= fd.input[iLoop];
    	
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
**	2개 입력 OR FB 정의
**
**********************************************************************/
uint32	stdOr2InitFunc
				(
			uint32	*specTypePtr,			
            uint32	*varTypePtr,			
            uint32	*outputTypePtr,			 
            strFbDefInfo	*paraInfoPtr	
				)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= OR2_CODE;
    paraInfoPtr->inputNo	= OR2_SPEC_NUM;
    paraInfoPtr->intlVarNo	= OR2_VAR_NUM;
    paraInfoPtr->outputNo	= OR2_OUTPUT_NUM;

	/* 스펙 - 2개 */
	for(iLoop = 0; iLoop < OR2_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_BIT_TYPE | SIZE32_TYPE;	

	/* 내부변수 - 0개 */		

    /* 출력 - 1개 */
	*outputTypePtr++	= ANY_BIT_TYPE | SIZE32_TYPE;	/* N01 */
	
    return(status);
}

uint32	stdOr2RunFunc
				(
			uint32 taskId,		
			uint32 fbBlockAddr	
				)
{
	uint32	status = NO_ERROR;	
	uint32	iLoop;
	
	strFC0040Info fd;

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
	fd.output = 0;
	
	for(iLoop = 0; iLoop < OR2_SPEC_NUM; iLoop++)
		fd.output |= fd.input[iLoop];


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
**	4개 입력 OR FB 정의
**
**********************************************************************/
uint32	stdOr4InitFunc
				(
			uint32	*specTypePtr,			
            uint32	*varTypePtr,			
            uint32	*outputTypePtr,			 
            strFbDefInfo	*paraInfoPtr	
				)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= OR4_CODE;
    paraInfoPtr->inputNo	= OR4_SPEC_NUM;
    paraInfoPtr->intlVarNo	= OR4_VAR_NUM;
    paraInfoPtr->outputNo	= OR4_OUTPUT_NUM;

	/* 스펙 - 4개 */
	for(iLoop = 0; iLoop < OR4_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_BIT_TYPE | SIZE32_TYPE;	

	/* 내부변수 - 0개 */		

    /* 출력 - 1개 */
	*outputTypePtr++	= ANY_BIT_TYPE | SIZE32_TYPE;	/* N01 */
	
    return(status);
}

uint32	stdOr4RunFunc
				(
			uint32 taskId,		
			uint32 fbBlockAddr	
				)
{
	uint32	status = NO_ERROR;	
	uint32	iLoop;

	strFC0042Info fd;

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
	fd.output = 0;
	
	for(iLoop = 0; iLoop < OR4_SPEC_NUM; iLoop++)
		fd.output |= fd.input[iLoop];

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
**	8개 입력 OR FB 정의
**
**********************************************************************/
uint32	stdOr8InitFunc
				(
			uint32	*specTypePtr,			
            uint32	*varTypePtr,			
            uint32	*outputTypePtr,			 
            strFbDefInfo	*paraInfoPtr	
				)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= OR8_CODE;
    paraInfoPtr->inputNo	= OR8_SPEC_NUM;
    paraInfoPtr->intlVarNo	= OR8_VAR_NUM;
    paraInfoPtr->outputNo	= OR8_OUTPUT_NUM;

	/* 스펙 - 8개 */
	for(iLoop = 0; iLoop < OR8_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_BIT_TYPE | SIZE32_TYPE;	


	/* 내부변수 - 0개 */		

    /* 출력 - 1개 */
	*outputTypePtr++	= ANY_BIT_TYPE | SIZE32_TYPE;	/* N01 */
	
    return(status);
}

uint32	stdOr8RunFunc
				(
			uint32 taskId,		
			uint32 fbBlockAddr	
				)
{
	uint32	status = NO_ERROR;	
	uint32	iLoop;

	strFC0046Info fd;

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
	fd.output = 0;
	
	for(iLoop = 0; iLoop < OR8_SPEC_NUM; iLoop++)
		fd.output |= fd.input[iLoop];


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
**	2개 입력 XOR FB 정의
**
**********************************************************************/
uint32	stdXor2InitFunc
				(
			uint32	*specTypePtr,			
            uint32	*varTypePtr,			
            uint32	*outputTypePtr,			 
            strFbDefInfo	*paraInfoPtr	
				)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= XOR2_CODE;
    paraInfoPtr->inputNo	= XOR2_SPEC_NUM;
    paraInfoPtr->intlVarNo	= XOR2_VAR_NUM;
    paraInfoPtr->outputNo	= XOR2_OUTPUT_NUM;

	/* 스펙 - 2개 */
	for(iLoop = 0; iLoop < XOR2_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_BIT_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - 1개 */
	*outputTypePtr++	= ANY_BIT_TYPE | SIZE32_TYPE;	/* N01 */
	
    return(status);
}

uint32	stdXor2RunFunc
				(
			uint32 taskId,		
			uint32 fbBlockAddr	
				)
{
	uint32	status = NO_ERROR;	
	
	strFC0052Info fd;

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
	fd.output = fd.input ^ fd.input2;

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
**	4개 입력 XOR FB 정의
**
**********************************************************************/
uint32	stdXor4InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= XOR4_CODE;
    paraInfoPtr->inputNo		= XOR4_SPEC_NUM;
    paraInfoPtr->intlVarNo		= XOR4_VAR_NUM;
    paraInfoPtr->outputNo	= XOR4_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < XOR4_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_BIT_TYPE | SIZE32_TYPE;


	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= ANY_BIT_TYPE | SIZE32_TYPE;
	
	return (status);
}		 	

uint32	stdXor4RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	
	strFC0054Info fd;

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
	fd.out =fd.in[0]^fd.in[1]^fd.in[2]^fd.in[3];

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
**	8개 입력 XOR FB 정의
**
**********************************************************************/
uint32	stdXor8InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)	
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= XOR8_CODE;
    paraInfoPtr->inputNo		= XOR8_SPEC_NUM;
    paraInfoPtr->intlVarNo		= XOR8_VAR_NUM;
    paraInfoPtr->outputNo	= XOR8_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < XOR8_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_BIT_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++	= ANY_BIT_TYPE | SIZE32_TYPE;
	
	return (status);
}

uint32	stdXor8RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	 
	strFC0058Info fd;

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
	fd.out =fd.in[0]^fd.in[1]^fd.in[2]^fd.in[3]^fd.in[4]^fd.in[5]^fd.in[6]^fd.in[7];

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
**	Not FB 정의
**
**********************************************************************/
uint32	stdNotInitFunc
				(
			uint32	*specTypePtr,			
            uint32	*varTypePtr,			
            uint32	*outputTypePtr,			 
            strFbDefInfo	*paraInfoPtr	
				)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= NOT_CODE;
    paraInfoPtr->inputNo		= NOT_SPEC_NUM;
    paraInfoPtr->intlVarNo		= NOT_VAR_NUM;
    paraInfoPtr->outputNo	= NOT_OUTPUT_NUM;

	/* 스펙 - 1개 */
	*specTypePtr++	= ANY_BIT_TYPE | SIZE32_TYPE;	/* S01 */

	/* 내부변수 - 0개 */		

    /* 출력 - 1개 */
	*outputTypePtr++	= ANY_BIT_TYPE | SIZE32_TYPE;	/* N01 */
	
    return(status);
}

uint32	stdNotRunFunc
				(
			uint32 taskId,		
			uint32 fbBlockAddr	
				)
{
	uint32	status = NO_ERROR;	
	uint32	paraRunType	=0;
	uint32  outputSize 	= 0;

	strFC0039Info	fd;			
	
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

	status = fbInOutputTypeCasting(LOGIC_ID, fbBlockAddr, NOT_REF_SINDEX, &paraRunType);
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	outputSize  = GET_TYPE_DATA_SIZE(paraRunType);

	/* 알고리즘 연산부 */
	switch(outputSize) 
	{
		case SIZE01_TYPE:
			fd.output = ( (~fd.input)&(0xffffffff >>31) );  /* INPUT SIZE가 BIT인 경우 */
		break;
		case SIZE08_TYPE:
			fd.output = ( (~fd.input)&(0xffffffff >>24) );  /* INPUT SIZE가 BYTE인 경우 */ 
		break;
		case SIZE16_TYPE:
			fd.output = ( (~fd.input)&(0xffffffff >>16) );  /* INPUT SIZE가 WORD인 경우 */
		break;
		case SIZE32_TYPE:
			fd.output = (~fd.input);						/* INPUT SIZE가 DWORD인 경우 */
		break;
		default:
			fd.output = 0;
			status = FB_OUTPUT_DATA_SIZE_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
			goto PARA_WRITE;
		break;
	}

PARA_WRITE:
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
**	Bit Get FB 정의
**
**********************************************************************/
uint32	stdBgetInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)	
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= BGET_CODE;
    paraInfoPtr->inputNo		= BGET_SPEC_NUM;
    paraInfoPtr->intlVarNo		= BGET_VAR_NUM;
    paraInfoPtr->outputNo	= BGET_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;
	*specTypePtr++			= WORD_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}		 	

uint32	stdBgetRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	
	strFC0061Info fd;

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
	if((fd.bitIndex < 0)||(fd.bitIndex > 31))
	{
		status = FB_INPUT_RANGE_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}
	else 
	{
		fd.output = (fd.input >> fd.bitIndex)&0x1;
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
**	Bit And FB 정의
**
**********************************************************************/
uint32	stdBitAndInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)	
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= BITAND_CODE;
    paraInfoPtr->inputNo		= BITAND_SPEC_NUM;
    paraInfoPtr->intlVarNo		= BITAND_VAR_NUM;
    paraInfoPtr->outputNo	= BITAND_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= WORD_TYPE | SIZE32_TYPE;
	*specTypePtr++			= WORD_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= WORD_TYPE | SIZE32_TYPE;
	return (status);
}		 		 	

uint32	stdBitAndRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	
	strFC0062Info fd;

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
	fd.output = fd.input[0] & fd.input[1];

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
**	Bit OR FB 정의
**
**********************************************************************/
uint32	stdBitOrInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= BITOR_CODE;
    paraInfoPtr->inputNo		= BITOR_SPEC_NUM;
    paraInfoPtr->intlVarNo		= BITOR_VAR_NUM;
    paraInfoPtr->outputNo	= BITOR_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= WORD_TYPE | SIZE32_TYPE;
	*specTypePtr++			= WORD_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= WORD_TYPE | SIZE32_TYPE;
	return (status);
}		 		 		 	

uint32	stdBitOrRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	
	strFC0063Info fd;

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
	fd.output = fd.input[0] | fd.input[1];

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
**	Bit Set FB 정의
**
**********************************************************************/
uint32	stdBitSetInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)	
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= BSET_CODE;
    paraInfoPtr->inputNo		= BSET_SPEC_NUM;
    paraInfoPtr->intlVarNo		= BSET_VAR_NUM;
    paraInfoPtr->outputNo	= BSET_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= WORD_TYPE | SIZE32_TYPE;
	*specTypePtr++			= WORD_TYPE | SIZE32_TYPE;
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= WORD_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= WORD_TYPE | SIZE32_TYPE;
	
	return (status);
}		 		 	

uint32	stdBitSetRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint32 	temp;

	strFC0064Info fd;
	
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
	if(fd.enable == 1)
	{
		if((fd.bitIndex < 0)||(fd.bitIndex > 31))
		{
			status = FB_INPUT_RANGE_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}
		else 
		{
		    temp = (~(0x1 << fd.bitIndex)) & fd.input;
		    
			fd.output = ((0x1 & fd.bitValue) << fd.bitIndex)|temp;
		}
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
**	Bit Register FB 정의
**
**********************************************************************/
uint32	stdRegInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= REG_CODE;
    paraInfoPtr->inputNo		= REG_SPEC_NUM;
    paraInfoPtr->intlVarNo		= REG_VAR_NUM;
    paraInfoPtr->outputNo	= REG_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= REAL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		
	*varTypePtr++			= REAL_TYPE | SIZE32_TYPE;

    /* 출력 - ?개 */
	*outputTypePtr++		= REAL_TYPE | SIZE32_TYPE;
	return (status);
}		 	

uint32	stdRegRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 

	strFC0065Info fd;
	
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

    if(fd.reset == 1) 			/* Reset 최우선 */
    	fd.output = 0.0;
    	
    else if(fd.set == 1)
    	fd.output = fd.input;

    else
    {
   		if(fd.load == 0)
   			fd.output = fd.prevOutput;	
		else
			fd.output = fd.input;	
    } 
 
    fd.prevOutput = fd.output;

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
**	SR Flip flop FB 정의
**
**********************************************************************/
uint32	stdSrFfInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= SRFF_CODE;
    paraInfoPtr->inputNo		= SRFF_SPEC_NUM;
    paraInfoPtr->intlVarNo		= SRFF_VAR_NUM;
    paraInfoPtr->outputNo	= SRFF_OUTPUT_NUM;

	/* 입력 - ?개 */
	for(iLoop = 0; iLoop < SRFF_SPEC_NUM; iLoop++)
		*specTypePtr++	= BOOL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		
	*varTypePtr++	= BOOL_TYPE | SIZE32_TYPE;

    /* 출력 - ?개 */
	*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;

	return (status);
}		 		 	

uint32	stdSrFfRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint32 output;

	strFC0066Info	fd;
		
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

    if(fd.resetInput == 1) 	/* 최우선 */
    	fd.output = 0;
  
  	else if(fd.setInput == 1)
  	{
  		if(fd.clockInput == 0)
  		{
  			fd.output = 1;	/* Set Value */
  		}
  		else
  		{
  			fd.output = fd.dataInput;	
  		}
  	}
  	else
  	{
  		if(fd.clockInput == 0)
  		{
  			fd.output = fd.prevOutput;	
  		}
  		else
  		{
  			fd.output = fd.dataInput;	
  		}
  		
  	}
    	
    fd.prevOutput = fd.output;

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
