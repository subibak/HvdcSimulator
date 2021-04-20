#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_standard\fcBitStableEle.h"

/**************************************************************************
**		�ܺ� �Լ� 
**************************************************************************/
extern uint32 readRuntimeFbData(uint32,uint32,uint32,uint32 *);
extern uint32 writeRuntimeFbData(uint32, uint32,uint32, uint32 *);
extern void	  setErrorCode(uint8 *, uint32, uint8 *, uint32);

/*********************************************************************
**	
**	Reset dominant Bitstable FB ����
**
**********************************************************************/
uint32	stdRsInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= RS_CODE;
    paraInfoPtr->inputNo	= RS_SPEC_NUM;
    paraInfoPtr->intlVarNo	= RS_VAR_NUM;
    paraInfoPtr->outputNo	= RS_OUTPUT_NUM;

	/* �Է� - 2�� */
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */	

    /* ��� - 1�� */
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;

	return (status);	 	
}

uint32	stdRsRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = 0; 
	strFC0212Info fd;
	
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

	if(fd.set == 0) 
	{
		/* ���� �� ���� */
		if(fd.resetDominant == 0) 
		{
			fd.q1 = fd.q1 * 1;
		} 
		/* ��� ���� */
		else if (fd.resetDominant == 1) 
		{
			fd.q1 = 0;
		} 
		else 
		{
			status = FB_INPUT_DATA_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}
	}
	else if (fd.set == 1) 
	{
		/* ��� ��Ʈ */
		if(fd.resetDominant == 0) 
		{
			fd.q1 = 1;
		} 
		/* ��� ����(���� �켱) */
		else if (fd.resetDominant == 1) 
		{
			fd.q1 = 0;
		} 
		else 
		{
			status = FB_INPUT_DATA_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}

	}
	else 
	{
		status = FB_INPUT_DATA_ERR;
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
**	set dominant Bitstable FB ����
**
**********************************************************************/
uint32	stdSrInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= SR_CODE;
    paraInfoPtr->inputNo	= SR_SPEC_NUM;
    paraInfoPtr->intlVarNo	= SR_VAR_NUM;
    paraInfoPtr->outputNo	= SR_OUTPUT_NUM;

	/* �Է� - 2�� */
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;

	/* ���κ��� - 0�� */	

    /* ��� - 1�� */
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;

	return (status);	 	
} 	

uint32	stdSrRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	strFC0214Info fd;
	
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

	if(fd.setDominant == 0) 
	{

		/* ���� �� ���� */
		if(fd.reset == 0 ) 
		{
			fd.q1 = fd.q1 * 1;
		} 
		/* ��� ���� */
		else if (fd.reset == 1) 
		{
			fd.q1 = 0;
		} 
		else 
		{
			status = FB_INPUT_DATA_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}
	}
	else if (fd.setDominant == 1) 
	{
		/* ��� ��Ʈ */
		if((fd.reset == 0) || (fd.reset == 1))
		{
			fd.q1 = 1;
		} 
		else 
		{
			status = FB_INPUT_DATA_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}

	}
	else 
	{
		status = FB_INPUT_DATA_ERR;
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
