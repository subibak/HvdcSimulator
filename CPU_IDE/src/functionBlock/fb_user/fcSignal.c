#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_user\fcSignal.h"

/**************************************************************************
**		�ܺ� �Լ� 
**************************************************************************/
extern uint32	readRuntimeFbData(uint32, uint32, uint32, uint32 *);
extern uint32	writeRuntimeFbData(uint32, uint32, uint32, uint32 *);
extern void 	setErrorCode(uint8 *, uint32,uint8 *,uint32);
extern uint32 	getLogicTaskCycletime(uint32 , uint32 , float *);

/*********************************************************************
**	
**	RAMP FB 	
**
**********************************************************************/
uint32	rampInitFunc
				(
			uint32			*inputTypePtr,
            uint32			*intlVarTypePtr,
            uint32			*outputTypePtr,
            strFbDefInfo	*fbInfoPtr
            	)
{
	uint32	status = NO_ERROR;
	
	/**************************************************
	**	FB ���� ����
	**************************************************/
	fbInfoPtr->fbId			= RAMP_CODE;
    fbInfoPtr->inputNo		= RAMP_SPEC_NUM;
    fbInfoPtr->intlVarNo	= RAMP_VAR_NUM;
    fbInfoPtr->outputNo		= RAMP_OUTPUT_NUM;

	/**************************************************
	**	FB �Է� Ÿ�� ����
	**************************************************/
	*inputTypePtr++			= BOOL_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= REAL_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= REAL_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= REAL_TYPE|SIZE32_TYPE;
	*inputTypePtr			= REAL_TYPE|SIZE32_TYPE;

	/**************************************************
	**	FB ���� ���� Ÿ�� ����
	**************************************************/
	
	/**************************************************
	**	FB ��� Ÿ�� ����
	**************************************************/
	*outputTypePtr++		= BOOL_TYPE|SIZE32_TYPE;	
	*outputTypePtr			= REAL_TYPE|SIZE32_TYPE;	
		
    return(status);
}


uint32	rampRunFunc(uint32 taskId, uint32 fbMemAddr)
{
	uint32	status = NO_ERROR;
	float	factor;
	float	cycleTimeSec;
	float	rampRateValPerCycle;
	float	delta;
	
	strFC0450Info	fbInfo;
	
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbMemAddr,
                                (uint32 *)&fbInfo
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }

	if(fbInfo.rampRun == RAMP_INIT)	/* �ʱⰪ Set */
	{
		/* ��� �ʱ�ȭ */
		fbInfo.rampBusy		= RAMP_INIT;
		fbInfo.rampOut		= fbInfo.rampInitVal;
		
	}
	else if(fbInfo.rampRun == RAMP_RUN)	/* ramp �⵿ */
	{
		/* Cycle time(second) read */
		status = getLogicTaskCycletime(	LOGIC_ID,
										taskId,
										(float *)&cycleTimeSec
 								   );
		if(status) 
		{
			setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
			return(status);
		}
		
		/* ascending or descending Check */
		if(fbInfo.rampOut > fbInfo.rampSetVal)
			factor = -1.0;
		else
			factor = 1.0; 
		
		/* ramp ���� */
		rampRateValPerCycle = fbInfo.rampRateValPerSec * cycleTimeSec;
		
		delta = (float)fabs((double)(fbInfo.rampOut - fbInfo.rampSetVal));
		
		if(delta >= rampRateValPerCycle)
		{
			fbInfo.rampOut += (rampRateValPerCycle * factor);	
		}
		else
		{
			/* rampRateValPerCycle���� ������ rmapSetval �� Set */
			fbInfo.rampOut = fbInfo.rampSetVal; 	
		}	
		
		fbInfo.rampBusy = RAMP_RUN;
	}
	else
	{
		status = FB_INPUT_RANGE_ERR;
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);		
	}

    status = writeRuntimeFbData(taskId,
    							LOGIC_ID,
                                fbMemAddr,
                                (uint32 *)&fbInfo
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }
	
	return(status);
}