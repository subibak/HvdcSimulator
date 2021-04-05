#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_standard\fcCounter.h"

/**************************************************************************
**		�ܺ� �Լ� 
**************************************************************************/
extern uint32 readRuntimeFbData(uint32,uint32,uint32,uint32 *);
extern uint32 writeRuntimeFbData(uint32, uint32,uint32, uint32 *);
extern void	  setErrorCode(uint8 *, uint32, uint8 *, uint32);

/*********************************************************************
**	
**	Down Counter FB ����
**
**********************************************************************/
uint32	stdCtdInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= CTD_CODE;
    paraInfoPtr->inputNo		= CTD_SPEC_NUM;
    paraInfoPtr->intlVarNo		= CTD_VAR_NUM;
    paraInfoPtr->outputNo	= CTD_OUTPUT_NUM;

	/* �Է� - 3�� */
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;

	/* ���κ��� - 1�� */	
	*varTypePtr++			= BOOL_TYPE | SIZE32_TYPE;

    /* ��� - 2�� */
	*outputTypePtr++		= INT_TYPE | SIZE32_TYPE;
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;

	return (status);
}	 	

uint32	stdCtdRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	uint32	cd_q;

	strFC0201Info fd;
	
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

	/* clkInput�� Rising Edge �϶��� ī��Ʈ�� ���ҽ�Ŵ */ 
	cd_q 			= fd.clkInput && !(fd.prevInput);

	fd.prevInput 	= fd.clkInput;

	/* resetCmd == TRUE�̸� downCountVal = setVal */
	if(fd.loadCmd) 
	{
		fd.downCountVal = fd.loadVal;
	}
	else if( (cd_q) && (fd.loadVal > MIN_INT32_VALUE))
	{
		/* cv�� -32768 ~ 32768 ���̿� ���� ���� ���� */
		fd.downCountVal = fd.downCountVal - 1;
	}

	/* q�� downCountVal<0�϶� 1 */
	if(fd.downCountVal <= 0) 
	{
		fd.q = 1;
	}
	else 
	{
		fd.q = 0;
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

	return (status);
}

/*********************************************************************
**	
**	Up Counter FB ����
**
**********************************************************************/
uint32	stdCtuInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= CTU_CODE;
    paraInfoPtr->inputNo		= CTU_SPEC_NUM;
    paraInfoPtr->intlVarNo		= CTU_VAR_NUM;
    paraInfoPtr->outputNo	= CTU_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++	= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++	= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++	= INT_TYPE | SIZE32_TYPE;

	/* ���κ��� - ?�� */	
	*varTypePtr++	= BOOL_TYPE | SIZE32_TYPE;

    /* ��� - ?�� */
	*outputTypePtr++	= INT_TYPE | SIZE32_TYPE;
	*outputTypePtr++	= BOOL_TYPE | SIZE32_TYPE;

	return (status);
}	 	

uint32	stdCtuRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	uint32	cu_q;

	strFC0202Info fd;
	
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
	cu_q 			= fd.clkInput && !(fd.prevInput);
	fd.prevInput 	= fd.clkInput;

	if(fd.resetCmd) 
	{
		fd.upCountVal = 0;
	}
	else if(cu_q && (fd.limitVal < MAX_INT32_VALUE))
	{
		fd.upCountVal = fd.upCountVal + 1;
	}

	/* q�� downCountVal<0�϶� 1 */
	if(fd.upCountVal >= fd.limitVal) 
	{
		fd.q = 1;
	}
	else 
	{
		fd.q = 0;
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
**	Up-Down Counter FB ����
**
**********************************************************************/
uint32	stdCtudInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr){
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= CTUD_CODE;
    paraInfoPtr->inputNo		= CTUD_SPEC_NUM;
    paraInfoPtr->intlVarNo		= CTUD_VAR_NUM;
    paraInfoPtr->outputNo	= CTUD_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;

	/* ���κ��� - ?�� */	
	*varTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*varTypePtr++			= BOOL_TYPE | SIZE32_TYPE;

    /* ��� - ?�� */
	*outputTypePtr++		= INT_TYPE | SIZE32_TYPE;
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;

	return (status);
}	 	

uint32	stdCtudRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	uint32	cu_q, cd_q;

	strFC0203Info fd;

	
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
	cu_q 				= fd.upClkInput && !(fd.prevUpClkInput);
	fd.prevUpClkInput 	= fd.upClkInput;

	cd_q 				= fd.downClkInput && !(fd.prevDownClkInput);
	fd.prevDownClkInput = fd.downClkInput;

	if(fd.resetCmd) 
	{
		fd.upDownCountVal = 0;
	} 
	else if(fd.loadCmd) 
	{
		fd.upDownCountVal = fd.loadVal;
	} 
	else if(cu_q && (fd.upDownCountVal < MAX_INT32_VALUE)) 
	{
		fd.upDownCountVal = fd.upDownCountVal + 1;
	}
	else if(cd_q && (fd.upDownCountVal > MIN_INT32_VALUE)) 
	{
		fd.upDownCountVal = fd.upDownCountVal - 1;
	}

	if(fd.upDownCountVal > fd.loadVal) 
	{
		fd.qu = 1;
	} 
	else 
	{
		fd.qu = 0;
	}

	if(fd.upDownCountVal <= 0) 
	{
		fd.qd = 1;
	} 
	else 
	{
		fd.qd = 0;
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
