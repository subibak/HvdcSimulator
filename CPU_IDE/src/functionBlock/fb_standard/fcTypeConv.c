#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_standard\fcTypeConv.h"

/**************************************************************************
**		�ܺ� �Լ� 
**************************************************************************/
extern uint32 readRuntimeFbData(uint32,uint32,uint32,uint32 *);
extern uint32 writeRuntimeFbData(uint32, uint32,uint32, uint32 *);
extern void	  setErrorCode(uint8 *, uint32, uint8 *, uint32);

/*********************************************************************
**	
**	Bool To Rea1 Type ��ȯ FB ����
**
**********************************************************************/
uint32	stdB2RInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr) 
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= BOOL2REAL_CODE;
    paraInfoPtr->inputNo		= BOOL2REAL_SPEC_NUM;
    paraInfoPtr->intlVarNo		= BOOL2REAL_VAR_NUM;
    paraInfoPtr->outputNo	= BOOL2REAL_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - ?�� */
	*outputTypePtr++		= REAL_TYPE | SIZE32_TYPE;
	
	return (status);
}	 		 	

uint32	stdB2RRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = 0; 
	strFC0131Info fd;

    /* ����� ���κ��� �Ķ���� �б� */
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
	
    /* ��� �Ķ���� ���� */
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
**	Real To Bool Type ��ȯ FB ����
**
**********************************************************************/
uint32	stdR2BInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= REAL2BOOL_CODE;
    paraInfoPtr->inputNo		= REAL2BOOL_SPEC_NUM;
    paraInfoPtr->intlVarNo		= REAL2BOOL_VAR_NUM;
    paraInfoPtr->outputNo	= REAL2BOOL_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++			= REAL_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - ?�� */
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;
	return (status);
}	 		 		 	

uint32	stdR2BRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = 0; 
	strFC0132Info fd;

    /* ����� ���κ��� �Ķ���� �б� */
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
	
    /* ��� �Ķ���� ���� */
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
**	Bool To Int32 Type ��ȯ FB ����
**
**********************************************************************/
uint32	stdB2DInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= BOOL2DINT_CODE;
    paraInfoPtr->inputNo		= BOOL2DINT_SPEC_NUM;
    paraInfoPtr->intlVarNo		= BOOL2DINT_VAR_NUM;
    paraInfoPtr->outputNo	= BOOL2DINT_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - ?�� */
	*outputTypePtr++		= INT_TYPE | SIZE32_TYPE;
	return (status);
}	 		 		 		 	

uint32	stdB2DRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	strFC0133Info fd;

    /* ����� ���κ��� �Ķ���� �б� */
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
	
    /* ��� �Ķ���� ���� */
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
**	Int32 TO Bool Type ��ȯ FB ����
**
**********************************************************************/
uint32	stdD2BInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= DINT2BOOL_CODE;
    paraInfoPtr->inputNo		= DINT2BOOL_SPEC_NUM;
    paraInfoPtr->intlVarNo		= DINT2BOOL_VAR_NUM;
    paraInfoPtr->outputNo	= DINT2BOOL_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - ?�� */
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}	

uint32	stdD2BRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	strFC0134Info fd;
	
    /* ����� ���κ��� �Ķ���� �б� */
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
	

    /* ��� �Ķ���� ���� */
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
**	Int32 TO Real Type ��ȯ FB ����
**
**********************************************************************/
uint32	stdD2RInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= DINT2REAL_CODE;
    paraInfoPtr->inputNo		= DINT2REAL_SPEC_NUM;
    paraInfoPtr->intlVarNo		= DINT2REAL_VAR_NUM;
    paraInfoPtr->outputNo	= DINT2REAL_OUTPUT_NUM;

	/* �Է� - 1�� */
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - 1�� */
	*outputTypePtr++		= REAL_TYPE | SIZE32_TYPE;
	
	return (status);
}		 	

uint32	stdD2RRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	strFC0135Info fd;
	
    /* ����� ���κ��� �Ķ���� �б� */
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
	

    /* ��� �Ķ���� ���� */
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
**	Real TO Int32 Type ��ȯ FB ����
**
**********************************************************************/
uint32	stdR2DInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= REAL2DINT_CODE;
    paraInfoPtr->inputNo		= REAL2DINT_SPEC_NUM;
    paraInfoPtr->intlVarNo		= REAL2DINT_VAR_NUM;
    paraInfoPtr->outputNo	= REAL2DINT_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++			= REAL_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - ?�� */
	*outputTypePtr++		= INT_TYPE | SIZE32_TYPE;
	return (status);
}		 	

uint32	stdR2DRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = 0; 
	strFC0136Info fd;
	
    /* ����� ���κ��� �Ķ���� �б� */
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
	

    /* ��� �Ķ���� ���� */
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
**	Bool TO Int32 Type ��ȯ FB ����
**
**********************************************************************/
uint32	stdMb2DInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= M_BOOL_TO_DINT_CODE;
    paraInfoPtr->inputNo		= M_BOOL_TO_DINT_SPEC_NUM;
    paraInfoPtr->intlVarNo		= M_BOOL_TO_DINT_VAR_NUM;
    paraInfoPtr->outputNo	= M_BOOL_TO_DINT_OUTPUT_NUM;

	/* �Է� - 32�� */
	for(iLoop = 0; iLoop < M_BOOL_TO_DINT_SPEC_NUM; iLoop++)
		*specTypePtr++	= BOOL_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - 1�� */
	*outputTypePtr++	= INT_TYPE | SIZE32_TYPE;
	
	return (status);
}		 	

uint32	stdMb2DRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR;
	uint32 i; 
	
	strFC0137Info fd;
	
    /* ����� ���κ��� �Ķ���� �б� */
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

    for(i= 0;i<(M_BOOL_TO_DINT_SPEC_NUM-1); i++)
        fd.out |= (fd.in[i]&0x1)<<i;
        
	if(fd.sign == 1)
	    fd.out = (-1)*fd.out;
	

    /* ��� �Ķ���� ���� */
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
**	Bool TO Int16 Type ��ȯ FB ����
**
**********************************************************************/
uint32	stdB2IInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr) 
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= BOOL_TO_INT_CODE;
    paraInfoPtr->inputNo		= BOOL_TO_INT_SPEC_NUM;
    paraInfoPtr->intlVarNo		= BOOL_TO_INT_VAR_NUM;
    paraInfoPtr->outputNo	= BOOL_TO_INT_OUTPUT_NUM;

	/* �Է� - 16�� */
	for(iLoop = 0; iLoop < BOOL_TO_INT_SPEC_NUM; iLoop++)
		*specTypePtr++	= BOOL_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - 1�� */
	*outputTypePtr++	= INT_TYPE | SIZE32_TYPE;
	
	return (status);
}		 		 	

uint32	stdB2IRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR;
	uint32 i; 
	strFC0138Info fd;
	
    /* ����� ���κ��� �Ķ���� �б� */
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
    for(i=0;i<(BOOL_TO_INT_SPEC_NUM-1);i++)
        fd.out |= (fd.in[i]&0x1)<<i;
        
	if(fd.sign == 1)
	    fd.out = (-1)*fd.out;
	

    /* ��� �Ķ���� ���� */
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
**	Int32 TO Bool Type ��ȯ FB ����
**
**********************************************************************/
uint32	stdD2MbInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;
	
	paraInfoPtr->fbId		= DINT_TO_M_BOOL_CODE;
    paraInfoPtr->inputNo		= DINT_TO_M_BOOL_SPEC_NUM;
    paraInfoPtr->intlVarNo		= DINT_TO_M_BOOL_VAR_NUM;
    paraInfoPtr->outputNo	= DINT_TO_M_BOOL_OUTPUT_NUM;

	/* �Է� - 1�� */
	*specTypePtr++	= INT_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - 32�� */
    for(iLoop = 0; iLoop < DINT_TO_M_BOOL_OUTPUT_NUM; iLoop++)
		*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;
	
	return (status);
}		 		 	

uint32	stdD2MbRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR;
	uint32 i; 
	strFC0139Info fd;
	
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
    
    for(i=0;i<(DINT_TO_M_BOOL_OUTPUT_NUM);i++)
        fd.out[i] = (fd.in>>i)&0x1;
	

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
**	Int32 TO int16 Type ��ȯ FB ����
**
**********************************************************************/
uint32	stdD2IInifFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= DINT_TO_INT_CODE;
    paraInfoPtr->inputNo		= DINT_TO_INT_SPEC_NUM;
    paraInfoPtr->intlVarNo		= DINT_TO_INT_VAR_NUM;
    paraInfoPtr->outputNo	= DINT_TO_INT_OUTPUT_NUM;

	/* �Է� - 1�� */
	*specTypePtr++	= INT_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - 1�� */
	*outputTypePtr++	= INT_TYPE | SIZE32_TYPE;

	return (status);
}	 	

uint32	stdD2IRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint16	uint16Data;
	
	strFC0140Info fd;
	
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

	uint16Data = fd.in & 0xffff;    
	
    fd.out = (int32)uint16Data;
	

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
**	Int16 TO Bool Type ��ȯ FB ����
**
**********************************************************************/
uint32	stdI2BInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= INT_TO_BOOL_CODE;
    paraInfoPtr->inputNo		= INT_TO_BOOL_SPEC_NUM;
    paraInfoPtr->intlVarNo		= INT_TO_BOOL_VAR_NUM;
    paraInfoPtr->outputNo	= INT_TO_BOOL_OUTPUT_NUM;

	/* �Է� - 1�� */
	*specTypePtr++	= INT_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - ?�� */
    for(iLoop = 0; iLoop < INT_TO_BOOL_OUTPUT_NUM; iLoop++)
		*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;

	return (status);
}	 		 	

uint32	stdI2BRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR;
	uint32	i; 
	
	strFC0141Info fd;
	
    /* ����� ���κ��� �Ķ���� �б� */
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
    
    for(i = 0;i < INT_TO_BOOL_OUTPUT_NUM; i++)
        fd.out[i] = (fd.in>>i)&0x1;
	

    /* ��� �Ķ���� ���� */
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
**	Int16 TO Int32 Type ��ȯ FB ����
**
**********************************************************************/
uint32	stdI2DInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= INT_TO_DINT_CODE;
    paraInfoPtr->inputNo		= INT_TO_DINT_SPEC_NUM;
    paraInfoPtr->intlVarNo		= INT_TO_DINT_VAR_NUM;
    paraInfoPtr->outputNo	= INT_TO_DINT_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */		

    /* ��� - ?�� */
	*outputTypePtr++		= INT_TYPE | SIZE32_TYPE;

	return (status);
}

uint32	stdI2DRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 	status = NO_ERROR; 
	uint16 	uint16Data;

	strFC0142Info fd;
	
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
    
    uint16Data 	= fd.in&0xFFFF;
 
    fd.out 		= (int32)uint16Data;
	

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
