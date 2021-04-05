#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_standard\fcNumerical.h"

/**************************************************************************
**		�ܺ� �Լ� 
**************************************************************************/
extern uint32 readRuntimeFbData(uint32,uint32,uint32,uint32 *);
extern uint32 fbCodeParaWriteWithVar(uint32,uint32, uint32, uint32 *,uint8 *);
extern void	  setErrorCode(uint8 *, uint32, uint8 *, uint32);

extern uint32 fbInOutputTypeCasting(uint32, uint32, uint32, uint32 *);
extern uint32 fbCodeVarWriteEnoRead(uint32 , uint32 , uint32* );

/*********************************************************************
**	
**	ABS FB ����
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

	/* �Է� - 1�� */
	*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;		

	/* ���κ��� - 0�� */		

    /* ��� - 1�� */
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
	
    /* ����� ���κ��� �Ķ���� �б� */
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

	/* �˰��� ����� */
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
		/* INT32 Ÿ���� ��� -2147483648�� ���밪�� INT32 Ÿ������ ǥ���� �� ����. */
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

    /* ��� �Ķ���� ���� */
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
**	SQRT FB ����
**
**********************************************************************/
uint32	stdSqrtInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)	
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= SQRT_CODE;
    paraInfoPtr->inputNo		= SQRT_SPEC_NUM;
    paraInfoPtr->intlVarNo		= SQRT_VAR_NUM;
    paraInfoPtr->outputNo	= SQRT_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - ?�� */
	*outputTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	return (status);
}	 		 	

uint32	stdSqrtRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status 	= NO_ERROR; 

	strFC0023Info fd;

    /* ����� ���κ��� �Ķ���� �б� */
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
    
	/* Square Root ��� */
	if(fd.input < 0.0)  
	{
		status = FB_INPUT_RANGE_ERR;
    	setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
    	return(status);
	} 
	else 
		fd.output = (float)sqrt((double)fd.input);
		
    	
    /* ��� �Ķ���� ���� */
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
**	exponential FB ����
**
**********************************************************************/
uint32	stdExpInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= EXP_CODE;
    paraInfoPtr->inputNo		= EXP_SPEC_NUM;
    paraInfoPtr->intlVarNo		= EXP_VAR_NUM;
    paraInfoPtr->outputNo	= EXP_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - ?�� */
	*outputTypePtr++	= REAL_TYPE | SIZE32_TYPE;
	return (status);
}
	
uint32	stdExpRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
    uint32 status = NO_ERROR; 
    
    strFC0025Info fd;

    /* ����� ���κ��� �Ķ���� �б� */
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
		
    	
    /* ��� �Ķ���� ���� */
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
**	�ڿ��α� FB ����(���� e)
**
**********************************************************************/
uint32	stdLnInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= LN_CODE;
    paraInfoPtr->inputNo		= LN_SPEC_NUM;
    paraInfoPtr->intlVarNo		= LN_VAR_NUM;
    paraInfoPtr->outputNo	= LN_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - ?�� */
	*outputTypePtr++	= REAL_TYPE | SIZE32_TYPE;
	return (status);
}	

uint32	stdLnRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
    uint32 status = NO_ERROR; 
    
    strFC0026Info fd;

    /* ����� ���κ��� �Ķ���� �б� */
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
		
    	
    /* ��� �Ķ���� ���� */
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
**	���α� FB ����(���� 10)
**
**********************************************************************/
uint32	stdLogInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= LOG_CODE;
    paraInfoPtr->inputNo		= LOG_SPEC_NUM;
    paraInfoPtr->intlVarNo		= LOG_VAR_NUM;
    paraInfoPtr->outputNo	= LOG_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - ?�� */
	*outputTypePtr++	= REAL_TYPE | SIZE32_TYPE;
	return (status);
}	

uint32	stdLogRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
    uint32 status = NO_ERROR; 
    
    strFC0027Info fd;

    /* ����� ���κ��� �Ķ���� �б� */
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
		
    	
    /* ��� �Ķ���� ���� */
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
**	arccosine FB ����
**
**********************************************************************/
uint32	stdACosInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= ACOS_CODE;
    paraInfoPtr->inputNo		= ACOS_SPEC_NUM;
    paraInfoPtr->intlVarNo		= ACOS_VAR_NUM;
    paraInfoPtr->outputNo	= ACOS_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - ?�� */
	*outputTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	return (status);
}	

uint32	stdACosRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
    uint32 status = NO_ERROR; 
    
    strFC0071Info fd;

    /* ����� ���κ��� �Ķ���� �б� */
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
		
    	
    /* ��� �Ķ���� ���� */
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
**	arcsine FB ����
**
**********************************************************************/
uint32	stdAsinInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr){
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= ASIN_CODE;
    paraInfoPtr->inputNo		= ASIN_SPEC_NUM;
    paraInfoPtr->intlVarNo		= ASIN_VAR_NUM;
    paraInfoPtr->outputNo	= ASIN_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++			= REAL_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - ?�� */
	*outputTypePtr++		= REAL_TYPE | SIZE32_TYPE;

	return (status);
}	

uint32	stdAsinRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
    uint32 status = NO_ERROR; 
    
    strFC0072Info fd;

    /* ����� ���κ��� �Ķ���� �б� */
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
		
	/* ��� �Ķ���� ���� */
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
**	arctangent FB ����
**
**********************************************************************/
uint32	stdAtanInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= ATAN_CODE;
    paraInfoPtr->inputNo		= ATAN_SPEC_NUM;
    paraInfoPtr->intlVarNo		= ATAN_VAR_NUM;
    paraInfoPtr->outputNo	= ATAN_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - ?�� */
	*outputTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	return (status);
}	

uint32	stdAtanRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
    uint32 status = NO_ERROR; 
    
    strFC0073Info fd;

    /* ����� ���κ��� �Ķ���� �б� */
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
		
    	
    /* ��� �Ķ���� ���� */
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
**	cosine FB ����
**
**********************************************************************/
uint32	stdCosInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= COS_CODE;
    paraInfoPtr->inputNo		= COS_SPEC_NUM;
    paraInfoPtr->intlVarNo		= COS_VAR_NUM;
    paraInfoPtr->outputNo	= COS_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - ?�� */
	*outputTypePtr++	= REAL_TYPE | SIZE32_TYPE;
	return (status);
}	

uint32	stdCosRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
    uint32 status = NO_ERROR; 
    
    strFC0074Info fd;

    /* ����� ���κ��� �Ķ���� �б� */
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
		
    	
    /* ��� �Ķ���� ���� */
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
**	sine FB ����
**
**********************************************************************/
uint32	stdSinInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= SIN_CODE;
    paraInfoPtr->inputNo		= SIN_SPEC_NUM;
    paraInfoPtr->intlVarNo		= SIN_VAR_NUM;
    paraInfoPtr->outputNo	= SIN_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - ?�� */
	*outputTypePtr++	= REAL_TYPE | SIZE32_TYPE;
	
	return (status);
}	
uint32	stdSinRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
    uint32 status = NO_ERROR;
    
    strFC0075Info fd; 

    /* ����� ���κ��� �Ķ���� �б� */
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
		
    	
    /* ��� �Ķ���� ���� */
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
**	tangent FB ����
**
**********************************************************************/
uint32	stdTanInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= TAN_CODE;
    paraInfoPtr->inputNo		= TAN_SPEC_NUM;
    paraInfoPtr->intlVarNo		= TAN_VAR_NUM;
    paraInfoPtr->outputNo	= TAN_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++	= REAL_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - ?�� */
	*outputTypePtr++	= REAL_TYPE | SIZE32_TYPE;
	
	return (status);
}	

uint32	stdTanRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
    uint32 status = NO_ERROR; 
    
    strFC0076Info fd;

    /* ����� ���κ��� �Ķ���� �б� */
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
		
    	
    /* ��� �Ķ���� ���� */
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
