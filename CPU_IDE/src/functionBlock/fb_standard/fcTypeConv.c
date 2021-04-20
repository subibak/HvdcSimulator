#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_standard\fcTypeConv.h"

/**************************************************************************
**		외부 함수 
**************************************************************************/
extern uint32 readRuntimeFbData(uint32,uint32,uint32,uint32 *);
extern uint32 writeRuntimeFbData(uint32, uint32,uint32, uint32 *);
extern void	  setErrorCode(uint8 *, uint32, uint8 *, uint32);

/*********************************************************************
**	
**	Bool To Rea1 Type 변환 FB 정의
**
**********************************************************************/
uint32	stdB2RInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr) 
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= B2R_CODE;
    paraInfoPtr->inputNo	= B2R_SPEC_NUM;
    paraInfoPtr->intlVarNo	= B2R_VAR_NUM;
    paraInfoPtr->outputNo	= B2R_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= REAL_TYPE | SIZE32_TYPE;
	
	return (status);
}	 		 	

uint32	stdB2RRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = 0; 
	strFC0131Info fd;

    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }
    
	fd.out = (float)(fd.in&0x1);
	
    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }

    return(status);
}

/*********************************************************************
**	
**	Real To Bool Type 변환 FB 정의
**
**********************************************************************/
uint32	stdR2BInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= R2B_CODE;
    paraInfoPtr->inputNo	= R2B_SPEC_NUM;
    paraInfoPtr->intlVarNo	= R2B_VAR_NUM;
    paraInfoPtr->outputNo	= R2B_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= REAL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;
	return (status);
}	 		 		 	

uint32	stdR2BRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = 0; 
	strFC0132Info fd;

    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }
    
	fd.out = 0x1&(uint32)fd.in;
	
    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }

    return(status);
}

/*********************************************************************
**	
**	Bool To Int32 Type 변환 FB 정의
**
**********************************************************************/
uint32	stdB2DInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= B2DI_CODE;
    paraInfoPtr->inputNo	= B2DI_SPEC_NUM;
    paraInfoPtr->intlVarNo	= B2DI_VAR_NUM;
    paraInfoPtr->outputNo	= B2DI_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= INT_TYPE | SIZE32_TYPE;
	return (status);
}	 		 		 		 	

uint32	stdB2DRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	strFC0133Info fd;

    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }
    
	fd.out = (int32)fd.in;
	
    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }

    return(status);
}

/*********************************************************************
**	
**	Int32 TO Bool Type 변환 FB 정의
**
**********************************************************************/
uint32	stdD2BInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= DI2B_CODE;
    paraInfoPtr->inputNo	= DI2B_SPEC_NUM;
    paraInfoPtr->intlVarNo	= DI2B_VAR_NUM;
    paraInfoPtr->outputNo	= DI2B_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}	

uint32	stdD2BRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	strFC0134Info fd;
	
    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }
    
	fd.out = 0x1&(uint32)fd.in;
	

    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }

    return(status);
}

/*********************************************************************
**	
**	Int32 TO Real Type 변환 FB 정의
**
**********************************************************************/
uint32	stdD2RInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= DI2R_CODE;
    paraInfoPtr->inputNo	= DI2R_SPEC_NUM;
    paraInfoPtr->intlVarNo	= DI2R_VAR_NUM;
    paraInfoPtr->outputNo	= DI2R_OUTPUT_NUM;

	/* 입력 - 1개 */
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - 1개 */
	*outputTypePtr++		= REAL_TYPE | SIZE32_TYPE;
	
	return (status);
}		 	

uint32	stdD2RRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	strFC0135Info fd;
	
    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }
    
	fd.out =(float)fd.in;
	

    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }

    return(status);
}

/*********************************************************************
**	
**	Real TO Int32 Type 변환 FB 정의
**
**********************************************************************/
uint32	stdR2DInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= R2DI_CODE;
    paraInfoPtr->inputNo	= R2DI_SPEC_NUM;
    paraInfoPtr->intlVarNo	= R2DI_VAR_NUM;
    paraInfoPtr->outputNo	= R2DI_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= REAL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= INT_TYPE | SIZE32_TYPE;
	return (status);
}		 	

uint32	stdR2DRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = 0; 
	strFC0136Info fd;
	
    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }
    
	fd.out = (int32)fd.in;
	

    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }

    return(status);
}

/*********************************************************************
**	
**	Bool TO Int32 Type 변환 FB 정의
**
**********************************************************************/
uint32	stdMb2DInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= MB2DI_CODE;
    paraInfoPtr->inputNo	= MB2DI_SPEC_NUM;
    paraInfoPtr->intlVarNo	= MB2DI_VAR_NUM;
    paraInfoPtr->outputNo	= MB2DI_OUTPUT_NUM;

	/* 입력 - 32개 */
	for(iLoop = 0; iLoop < MB2DI_SPEC_NUM; iLoop++)
		*specTypePtr++	= BOOL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - 1개 */
	*outputTypePtr++	= INT_TYPE | SIZE32_TYPE;
	
	return (status);
}		 	

uint32	stdMb2DRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR;
	uint32 i; 
	
	strFC0137Info fd;
	
    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }
    
    fd.out = 0;

    for(i= 0;i<(MB2DI_SPEC_NUM-1); i++)
        fd.out |= (fd.in[i]&0x1)<<i;
        
	if(fd.sign == 1)
	    fd.out = (-1)*fd.out;
	

    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }

    return(status);
}

/*********************************************************************
**	
**	Bool TO Int16 Type 변환 FB 정의
**
**********************************************************************/
uint32	stdB2IInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr) 
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= B2I_CODE;
    paraInfoPtr->inputNo	= B2I_SPEC_NUM;
    paraInfoPtr->intlVarNo	= B2I_VAR_NUM;
    paraInfoPtr->outputNo	= B2I_OUTPUT_NUM;

	/* 입력 - 16개 */
	for(iLoop = 0; iLoop < B2I_SPEC_NUM; iLoop++)
		*specTypePtr++	= BOOL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - 1개 */
	*outputTypePtr++	= INT_TYPE | SIZE32_TYPE;
	
	return (status);
}		 		 	

uint32	stdB2IRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR;
	uint32 i; 
	strFC0138Info fd;
	
    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,	__FUNCTION__, status);
		return(status);
    }
    
    fd.out = 0;
    for(i=0;i<(B2I_SPEC_NUM-1);i++)
        fd.out |= (fd.in[i]&0x1)<<i;
        
	if(fd.sign == 1)
	    fd.out = (-1)*fd.out;
	

    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,	__FUNCTION__, status);
		return(status);
    }

    return(status);
}

/*********************************************************************
**	
**	Int32 TO Bool Type 변환 FB 정의
**
**********************************************************************/
uint32	stdD2MbInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= DI2MB_CODE;
    paraInfoPtr->inputNo	= DI2MB_SPEC_NUM;
    paraInfoPtr->intlVarNo	= DI2MB_VAR_NUM;
    paraInfoPtr->outputNo	= DI2MB_OUTPUT_NUM;

	/* 입력 - 1개 */
	*specTypePtr++	= INT_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - 32개 */
    for(iLoop = 0; iLoop < DI2MB_OUTPUT_NUM; iLoop++)
		*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}		 		 	

uint32	stdD2MbRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR;
	uint32 i; 
	strFC0139Info fd;
	
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
    
    for(i=0;i<(DI2MB_OUTPUT_NUM);i++)
        fd.out[i] = (fd.in>>i)&0x1;
	

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
**	Int32 TO int16 Type 변환 FB 정의
**
**********************************************************************/
uint32	stdD2IInifFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= DI2I_CODE;
    paraInfoPtr->inputNo	= DI2I_SPEC_NUM;
    paraInfoPtr->intlVarNo	= DI2I_VAR_NUM;
    paraInfoPtr->outputNo	= DI2I_OUTPUT_NUM;

	/* 입력 - 1개 */
	*specTypePtr++	= INT_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - 1개 */
	*outputTypePtr++	= INT_TYPE | SIZE32_TYPE;

	return (status);
}	 	

uint32	stdD2IRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint16	uint16Data;
	
	strFC0140Info fd;
	
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

	uint16Data = fd.in & 0xffff;    
	
    fd.out = (int32)uint16Data;
	

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
**	Int16 TO Bool Type 변환 FB 정의
**
**********************************************************************/
uint32	stdI2BInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= I2B_CODE;
    paraInfoPtr->inputNo	= I2B_SPEC_NUM;
    paraInfoPtr->intlVarNo	= I2B_VAR_NUM;
    paraInfoPtr->outputNo	= I2B_OUTPUT_NUM;

	/* 입력 - 1개 */
	*specTypePtr++	= INT_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
    for(iLoop = 0; iLoop < I2B_OUTPUT_NUM; iLoop++)
		*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;

	return (status);
}	 		 	

uint32	stdI2BRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i; 
	
	strFC0141Info fd;
	
    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }
    
    for(i = 0;i < I2B_OUTPUT_NUM; i++)
        fd.out[i] = (fd.in>>i)&0x1;
	

    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }

    return(status);
}

/*********************************************************************
**	
**	Int16 TO Int32 Type 변환 FB 정의
**
**********************************************************************/
uint32	stdI2DInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= I2DI_CODE;
    paraInfoPtr->inputNo	= I2DI_SPEC_NUM;
    paraInfoPtr->intlVarNo	= I2DI_VAR_NUM;
    paraInfoPtr->outputNo	= I2DI_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;

	/* 내부변수 - 0개 */		

    /* 출력 - ?개 */
	*outputTypePtr++		= INT_TYPE | SIZE32_TYPE;

	return (status);
}

uint32	stdI2DRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint16 	uint16Data;

	strFC0142Info fd;
	
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
    
    uint16Data 	= fd.in&0xFFFF;
 
    fd.out 		= (int32)uint16Data;
	

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
