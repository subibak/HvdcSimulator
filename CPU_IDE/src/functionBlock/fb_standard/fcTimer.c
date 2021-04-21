#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_standard\fcTimer.h"

/**************************************************************************
**		�ܺ� �Լ� 
**************************************************************************/
extern uint32 readRuntimeFbData(uint32,uint32,uint32,uint32 *);
extern uint32 writeRuntimeFbData(uint32, uint32,uint32, uint32 *);
extern void	  setErrorCode(uint8 *, uint32, uint8 *, uint32);

/*********************************************************************
**	
**	Time Off FB ����
**
**********************************************************************/
uint32	stdToffInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId			= TOF_CODE;
    paraInfoPtr->inputNo		= TOF_SPEC_NUM;
    paraInfoPtr->intlVarNo		= TOF_VAR_NUM;
    paraInfoPtr->outputNo		= TOF_OUTPUT_NUM;

	/* �Է� - 2�� */
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;

	/* ���κ��� - 2�� */	
	*varTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*varTypePtr++			= INT_TYPE | SIZE32_TYPE;

    /* ��� - 2�� */
	*outputTypePtr++		= INT_TYPE | SIZE32_TYPE;
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;

	return (status);	 	
} 		 		

uint32	stdToffRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	strFC0261Info fd;
	uint32	in_q;
	
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
	in_q = (fd.in&0x1);

	if(in_q == 0x1) 
	{
		if(fd.prevValue == 0x0) /* �����Է� */
		{
			fd.elapsedTime	= 0;
			fd.Q 	= 1;
		}
		else if (fd.prevValue == 1) 
		{
			fd.Q = 1;	
		}
	}
	else /* ���� �Է� '0' */
	{
		if(fd.prevValue == 0x1) /* ���� �Է� */
		{
			fd.elapsedTime += 1;
	
			if(fd.elapsedTime >= fd.timeInput) 
			{
				fd.Q 	= 0;
			}	
			else 
			{
				fd.Q = 1;
			}
		}
		else /* ���� �Է� '0' */
		{
			if(fd.Q == 0x1)  /* ���� ��� */
			{
				fd.elapsedTime += 1;
			
				if(fd.elapsedTime >= fd.timeInput) 
				{
					fd.Q 	= 0;	
				}	
				else 
				{
					fd.Q = 1;
				}
			}
			else /* ���� ��� '0' */
			{
				fd.Q = 0;
			}
		}
	}

	fd.prevValue	= in_q;

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
**	Time ON FB ����
**
**********************************************************************/
uint32	stdTonInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= TON_CODE;
    paraInfoPtr->inputNo	= TON_SPEC_NUM;
    paraInfoPtr->intlVarNo	= TON_VAR_NUM;
    paraInfoPtr->outputNo	= TON_OUTPUT_NUM;

	/* �Է� - ?�� */
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;

	/* ���κ��� - ?�� */	
	*varTypePtr++			= INT_TYPE | SIZE32_TYPE;
	*varTypePtr++			= BOOL_TYPE | SIZE32_TYPE;

    /* ��� - ?�� */
	*outputTypePtr++		= INT_TYPE | SIZE32_TYPE;
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;

	return (status);	 	
}

uint32	stdTonRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	strFC0262Info fd;
	uint32	in_q;
	
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

	in_q = (fd.in&0x1);
	
	if(in_q == 0x1) 
	{
		fd.elapsedTime += 1;

		if(fd.elapsedTime >= fd.timeInput) 
		{
			fd.Q 	= 1;
			fd.elapsedTime 	= fd.timeInput;
		}
		else 
		{
			fd.Q = 0;
		}
	}
	else 
	{
		fd.elapsedTime 		= 0;
		fd.Q		= 0;	
	}

	fd.prevValue	= in_q;

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
**	Time PULSE FB ����
**
**********************************************************************/
uint32	stdTpInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= TP_CODE;
    paraInfoPtr->inputNo		= TP_SPEC_NUM;
    paraInfoPtr->intlVarNo		= TP_VAR_NUM;
    paraInfoPtr->outputNo	= TP_OUTPUT_NUM;

	/* �Է� - 2�� */
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;

	/* ���κ��� - 2�� */	
	*varTypePtr++			= INT_TYPE | SIZE32_TYPE;
	*varTypePtr++			= BOOL_TYPE | SIZE32_TYPE;

    /* ��� - 2�� */
	*outputTypePtr++		= INT_TYPE | SIZE32_TYPE;
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;

	return (status);	 	
}

uint32	stdTpRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = 0; 
	strFC0263Info fd;
	uint32			in_q;
	
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

	in_q = (fd.in&0x1);
	
	if(in_q == 0x1) 
	{
		if(fd.prevValue == 0x0)  /* �޽� �Է��� �Ǵ� ��� */
		{
			fd.Q 	= 1;

			fd.elapsedTime += 1;
			
			if(fd.elapsedTime >= fd.timeInput) 
			{
				fd.Q = 0;					
			}
		}
		else /* ���� �Է� '1' */
		{
			if(fd.Q == 0x1) 
			{
				fd.elapsedTime += 1;

				if(fd.elapsedTime >= fd.timeInput)
				{
					fd.Q	= 0;	
				}
			}
			else  /* ���� �Է��� ��� '1'�̵Ǵ� ��� */
			{
				fd.Q	= 0;
			}
		}
	}
	else /* ���� �Է��� 0 */ 
	{
		if(fd.Q == 0x1) 
		{
			fd.elapsedTime += 1;

			if(fd.elapsedTime >= fd.timeInput) 
			{
				fd.Q 	= 0;
				fd.elapsedTime	= 0;	
			}	
		}
		else 
		{
			fd.Q 	= 0;
			fd.elapsedTime	= 0;	
		}
	}
	
	fd.prevValue = in_q;


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
