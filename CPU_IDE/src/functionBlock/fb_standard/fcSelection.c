#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_standard\fcSelection.h"

/**************************************************************************
**		�ܺ� �Լ� 
**************************************************************************/
extern uint32 readRuntimeFbData(uint32,uint32,uint32,uint32 *);
extern uint32 writeRuntimeFbData(uint32, uint32,uint32, uint32 *);
extern void	  setErrorCode(uint8 *, uint32, uint8 *, uint32);

/*********************************************************************
**	
**	Limit FB ����
**
**********************************************************************/
uint32	stdLimitInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= LIMIT_CODE;
    paraInfoPtr->inputNo		= LIMIT_SPEC_NUM;
    paraInfoPtr->intlVarNo		= LIMIT_VAR_NUM;
    paraInfoPtr->outputNo	= LIMIT_OUTPUT_NUM;

	/* �Է� - 3�� */
	for(iLoop = 0; iLoop < LIMIT_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* ���κ��� - ?�� */	


    /* ��� - ?�� */
	*outputTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	return (status);	 	
} 	

uint32	stdLimitRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint32	paraRunType = 0;

	strFC0231Info fd;

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
	

	/* MIN �Է°��� MAX �Է°��� ���ų� ���� Ŭ ��� ������ ó�� */
	if(fd.min > fd.max) 
	{
		status = FB_INPUT_DATA_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		
		return(status);
	}

	if(fd.input <= fd.min) 
	{
		fd.output = fd.min;
	} 
	else if (fd.input >= fd.max) 
	{
		fd.output = fd.max;
	} 
	else 
	{
		fd.output = fd.input;
	}

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

    return(status);
}

/*********************************************************************
**	
**	2�� �Է¿� ���� Max �� ���� FB ����
**
**********************************************************************/
uint32	stdMax2InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= MAX2_CODE;
    paraInfoPtr->inputNo		= MAX2_SPEC_NUM;
    paraInfoPtr->intlVarNo		= MAX2_VAR_NUM;
    paraInfoPtr->outputNo	= MAX2_OUTPUT_NUM;

	/* �Է� - ?�� */
	for(iLoop = 0; iLoop < MAX2_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* ���κ��� - ?�� */	


    /* ��� - ?�� */
	*outputTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	return (status);	 	
} 		 	

uint32	stdMax2RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i; 
	float 	std32TempData, std32MaxData;
	
	strFC0232Info fd;

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
	

	/* �˰��� ����� */

	std32TempData = fd.in[0];
	
	for(i = 0;i < (MAX2_SPEC_NUM-1); i++)
	{
		std32MaxData 	= MY_MAX(std32TempData,fd.in[i+1]);
		std32TempData 	= std32MaxData;
	}
	
	fd.out = std32MaxData;


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

    return(status);
}

/*********************************************************************
**	
**	4�� �Է¿� ���� Max �� ���� FB ����
**
**********************************************************************/
uint32	stdMax4InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= MAX4_CODE;
    paraInfoPtr->inputNo		= MAX4_SPEC_NUM;
    paraInfoPtr->intlVarNo		= MAX4_VAR_NUM;
    paraInfoPtr->outputNo	= MAX4_OUTPUT_NUM;

	/* �Է� - ?�� */
	for(iLoop = 0; iLoop < MAX4_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* ���κ��� - ?�� */	


    /* ��� - ?�� */
	*outputTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	return (status);	 	
} 	 	

uint32	stdMax4RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i;
	float 	std32TempData, std32MaxData;
	
	strFC0234Info fd;

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
	
	std32TempData = fd.in[0];

	for(i = 0;i < (MAX4_SPEC_NUM-1); i++)
	{
		std32MaxData 	= MY_MAX(std32TempData,fd.in[i+1]);
		std32TempData 	= std32MaxData;
	}
	
	fd.out = std32MaxData;

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

    return(status);
}

/*********************************************************************
**	
**	8�� �Է¿� ���� Max �� ���� FB ����
**
**********************************************************************/
uint32	stdMax8InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= MAX8_CODE;
    paraInfoPtr->inputNo		= MAX8_SPEC_NUM;
    paraInfoPtr->intlVarNo		= MAX8_VAR_NUM;
    paraInfoPtr->outputNo	= MAX8_OUTPUT_NUM;

	/* �Է� - ?�� */
	for(iLoop = 0; iLoop < MAX8_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* ���κ��� - ?�� */	


    /* ��� - ?�� */
	*outputTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	return (status);	 	
}		 	

uint32	stdMax8RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i; 
	float 	std32TempData, std32MaxData;
	
	strFC0238Info fd;

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

	/* �˰��� ����� */

	std32TempData = fd.in[0];
	
	for(i = 0;i < (MAX8_SPEC_NUM-1); i++)
	{
		std32MaxData 	= MY_MAX(std32TempData,fd.in[i+1]);
		std32TempData 	= std32MaxData;
	}
	
	fd.out = std32MaxData;

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

    return(status);
}

/*********************************************************************
**	
**	2�� �Է¿� ���� Min. �� ���� FB ����
**
**********************************************************************/
uint32	stdMin2InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= MIN2_CODE;
    paraInfoPtr->inputNo		= MIN2_SPEC_NUM;
    paraInfoPtr->intlVarNo		= MIN2_VAR_NUM;
    paraInfoPtr->outputNo	= MIN2_OUTPUT_NUM;

	/* �Է� - ?�� */
	for(iLoop = 0; iLoop < MIN2_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* ���κ��� - ?�� */	


    /* ��� - ?�� */
	*outputTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	return (status);	 	
} 		 	

uint32	stdMin2RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i; 
	float 	std32TempData, std32MinData;

	strFC0240Info	fd;
	
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

	/* �˰��� ����� */
	std32TempData = fd.in[0];

	for(i = 0;i < (MIN2_SPEC_NUM-1); i++)
	{
		std32MinData 	= MY_MIN(std32TempData,fd.in[i+1]);
		std32TempData 	= std32MinData;
	}
	
	fd.out = std32MinData;

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

    return(status);
}

/*********************************************************************
**	
**	4�� �Է¿� ���� Min. �� ���� FB ����
**
**********************************************************************/
uint32	stdMin4InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= MIN4_CODE;
    paraInfoPtr->inputNo		= MIN4_SPEC_NUM;
    paraInfoPtr->intlVarNo		= MIN4_VAR_NUM;
    paraInfoPtr->outputNo	= MIN4_OUTPUT_NUM;

	/* �Է� - ?�� */
	for(iLoop = 0; iLoop < MIN4_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* ���κ��� - ?�� */	


    /* ��� - ?�� */
	*outputTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	return (status);	 	
} 	 	

uint32	stdMin4RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i; 
	float 	std32TempData, std32MinData;

	strFC0242Info	fd;

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

	/* �˰��� ����� */
	std32TempData = fd.in[0];

	for(i = 0;i < (MIN4_SPEC_NUM-1); i++)
	{
		std32MinData 	= MY_MIN(std32TempData,fd.in[i+1]);
		std32TempData 	= std32MinData;
	}
	
	fd.out = std32MinData;

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

    return(status);
}

/*********************************************************************
**	
**	8�� �Է¿� ���� Min. �� ���� FB ����
**
**********************************************************************/
uint32	stdMin8InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= MIN8_CODE;
    paraInfoPtr->inputNo		= MIN8_SPEC_NUM;
    paraInfoPtr->intlVarNo		= MIN8_VAR_NUM;
    paraInfoPtr->outputNo	= MIN8_OUTPUT_NUM;

	/* �Է� - ?�� */
	for(iLoop = 0; iLoop < MIN8_SPEC_NUM; iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* ���κ��� - ?�� */	


    /* ��� - ?�� */
	*outputTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	return (status);	 	
}		 	

uint32	stdMin8RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i; 
	float 	std32TempData, std32MinData;

	strFC0246Info	fd;

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

	/* �˰��� ����� */
	std32TempData = fd.in[0];

	for(i = 0; i < (MIN8_SPEC_NUM-1); i++)
	{
		std32MinData 	= MY_MIN(std32TempData,fd.in[i+1]);
		std32TempData 	= std32MinData;
	}
	
	fd.out = std32MinData;

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

    return(status);
}

/*********************************************************************
**	
**	2�� �Է¿� ���� index�� ���� ���� ���� FB ����
**
**********************************************************************/
uint32	stdMux2InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= MUX2_CODE;
    paraInfoPtr->inputNo		= MUX2_SPEC_NUM;
    paraInfoPtr->intlVarNo		= MUX2_VAR_NUM;
    paraInfoPtr->outputNo	= MUX2_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++	= INT_TYPE | SIZE32_TYPE;

	*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;
	*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* ���κ��� - ?�� */	


    /* ��� - ?�� */
	*outputTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	return (status);	 	
} 		 		 	

uint32	stdMux2RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint32	*srcPtr, *destPtr;

	strFC0248Info fd;

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

	/* �˰��� ����� */
	if(	(fd.choiceIndex > 0) && (fd.choiceIndex <= (MUX2_SPEC_NUM - 1))) 
	{
		srcPtr 		= &fd.in[0];
		srcPtr 		+= (fd.choiceIndex - 1);
		destPtr 	= &fd.out;
		
		fourBytesDataCopy(destPtr, srcPtr, 1);
	}
	else
	{ 
		status = FB_INPUT_RANGE_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		
		return(status);
	}
	
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

    return(status);
}

/*********************************************************************
**	
**	4�� �Է¿� ���� index�� ���� ���� ���� FB ����
**
**********************************************************************/
uint32	stdMux4InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= MUX4_CODE;
    paraInfoPtr->inputNo		= MUX4_SPEC_NUM;
    paraInfoPtr->intlVarNo		= MUX4_VAR_NUM;
    paraInfoPtr->outputNo	= MUX4_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++	= INT_TYPE | SIZE32_TYPE;
	
	for(iLoop = 0; iLoop < (MUX4_SPEC_NUM - 1); iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* ���κ��� - ?�� */	


    /* ��� - ?�� */
	*outputTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	return (status);	 	
} 		 		 		 	

uint32	stdMux4RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint32	*srcPtr, *destPtr;

	strFC0250Info fd;

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

	/* �˰��� ����� */
	if(	(fd.choiceIndex > 0) && (fd.choiceIndex <= (MUX4_SPEC_NUM - 1))) 
	{
		srcPtr 		= &fd.in[0];
		srcPtr 		+= (fd.choiceIndex - 1);
		destPtr 	= &fd.out;
		
		fourBytesDataCopy(destPtr, srcPtr, 1);
	}
	else
	{ 
		status = FB_INPUT_RANGE_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		
		return(status);
	}

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

    return(status);
}

/*********************************************************************
**	
**	8�� �Է¿� ���� index�� ���� ���� ���� FB ����
**
**********************************************************************/
uint32	stdMux8InitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= MUX8_CODE;
    paraInfoPtr->inputNo		= MUX8_SPEC_NUM;
    paraInfoPtr->intlVarNo		= MUX8_VAR_NUM;
    paraInfoPtr->outputNo	= MUX8_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++	= INT_TYPE | SIZE32_TYPE;
	
	for(iLoop = 0; iLoop < (MUX8_SPEC_NUM - 1); iLoop++)
		*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* ���κ��� - ?�� */	


    /* ��� - ?�� */
	*outputTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	return (status);	 	
} 		 		 		 	

uint32	stdMux8RunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint32	*srcPtr, *destPtr;

	strFC0254Info fd;

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

	/* �˰��� ����� */
	/* �˰��� ����� */
	if(	(fd.choiceIndex > 0) && (fd.choiceIndex <= (MUX8_SPEC_NUM - 1))) 
	{
		srcPtr 		= &fd.in[0];
		srcPtr 		+= (fd.choiceIndex - 1);
		destPtr 	= &fd.out;
		
		fourBytesDataCopy(destPtr, srcPtr, 1);
	}
	else
	{ 
		status = FB_INPUT_RANGE_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		
		return(status);
	}

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

    return(status);
}

/*********************************************************************
**	
**	2�� �Է¿� ���� index�� ���� ���� ���� FB ����
**
**********************************************************************/
uint32	stdSelInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= SEL_CODE;
    paraInfoPtr->inputNo		= SEL_SPEC_NUM;
    paraInfoPtr->intlVarNo		= SEL_VAR_NUM;
    paraInfoPtr->outputNo	= SEL_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++	= BOOL_TYPE | SIZE32_TYPE;

	*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;
	*specTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	/* ���κ��� - ?�� */	


    /* ��� - ?�� */
	*outputTypePtr++	= ANY_NUM_TYPE | SIZE32_TYPE;

	return (status);	 	
} 		 		 		 

uint32	stdSelRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	
	strFC0256Info fd;

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

	/* �˰��� ����� */
	if(fd.choiceIndex == 0) 
	{
		fourBytesDataCopy(&fd.out, &fd.ina, 1);
	} 
	else if(fd.choiceIndex == 1) 
	{
		fourBytesDataCopy(&fd.out, &fd.inb, 1);
	}
	else
	{ 
		status = FB_INPUT_RANGE_ERR;
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		
		return(status);
	}

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

    return(status);
}



