/**************************************************************************
** Zone4_60APN(FB731)
**************************************************************************/
#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_user\fcPrtCommon.h"
#include	"fb_code\fb_user\fcZ4_60APN.h"

/**************************************************************************
**		외부 함수 
**************************************************************************/
extern uint32	readRuntimeFbData(uint32, uint32, uint32, uint32 *);
extern uint32	writeRuntimeFbData(uint32, uint32, uint32, uint32 *);
extern void 	setErrorCode(uint8 *, uint32,uint8 *,uint32);
extern uint32 	getLogicTaskCycletime(uint32 , uint32 , float *);
extern uint32	ftOverDetect(float, float, float, float *, float);

/*********************************************************************
**	
** Zone4_60APN FB 	
**
**********************************************************************/
uint32	stdZ4_60APNInitFunc
				(
			uint32			*inputTypePtr,
            uint32			*intlVarTypePtr,
            uint32			*outputTypePtr,
            strFbDefInfo	*fbInfoPtr
            	)
{
	uint32	status = NO_ERROR;
	
	/**************************************************
	**	FB 정보 정의
	**************************************************/
	fbInfoPtr->fbId			= FC0731_CODE;
    fbInfoPtr->inputNo		= FC0731_SPEC_NUM;
    fbInfoPtr->intlVarNo	= FC0731_VAR_NUM;
    fbInfoPtr->outputNo		= FC0731_OUTPUT_NUM;

	/**************************************************
	**	FB 입력 타입 정의
	**************************************************/
	*inputTypePtr++			= INT_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= INT_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= REAL_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= REAL_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= REAL_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= REAL_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= REAL_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= REAL_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= REAL_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= REAL_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= REAL_TYPE|SIZE32_TYPE;
	*inputTypePtr			= REAL_TYPE|SIZE32_TYPE;

	/**************************************************
	**	FB 내부 변수 타입 정의
	**************************************************/
	*intlVarTypePtr++ 		= REAL_TYPE|SIZE32_TYPE;
	*intlVarTypePtr 		= REAL_TYPE|SIZE32_TYPE;

	/**************************************************
	**	FB 출력 타입 정의
	**************************************************/
	*outputTypePtr++		= INT_TYPE|SIZE32_TYPE;	
	*outputTypePtr			= INT_TYPE|SIZE32_TYPE;	
		
    return(status);
}

uint32	stdZ4_60APNRunFunc(uint32 taskId, uint32 fbMemAddr)
{
	uint32	status = NO_ERROR;
	float	cycleTimeSec;
	float	reset;
	float	enable;
	float 	abs_max;
	float	abs_diff_a;
	float	abs_diff_b;
	float	abs_diff_c;

	strFC0731Info	fbInfo;
	
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

	reset 	= fbInfo.reset & 0x1;
	enable 	= fbInfo.enable & 0x1;

    if (reset == 1)
    {
		fbInfo.t1 		 = 0;
		fbInfo.t2 		 = 0;
								
		fbInfo.o1_alarm	 = NORMAL;		
		fbInfo.o2_conBlk = NORMAL;		
	}
    else
    {
    	if(enable == 1)
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

			/* Difference & Max Calculation */
			abs_diff_a  = abs(fbInfo.iarm_rms_up.a - fbInfo.iarm_rms_low.a);
			abs_diff_b  = abs(fbInfo.iarm_rms_up.b - fbInfo.iarm_rms_low.b);
			abs_diff_c  = abs(fbInfo.iarm_rms_up.c - fbInfo.iarm_rms_low.c);

			if(abs_diff_a > abs_diff_b)
			{
				abs_max = abs_diff_a;
			}
			else
			{
				abs_max = abs_diff_b;
			}

			if(abs_diff_c > abs_max)
				abs_max = abs_diff_c;
			
			/* Fault Detection */
			fbInfo.o1_alarm	 = ftOverDetect(abs_max, fbInfo.threshold1, fbInfo.setT1, &fbInfo.t1, cycleTimeSec);
			fbInfo.o2_conBlk = ftOverDetect(abs_max, fbInfo.threshold2, fbInfo.setT2, &fbInfo.t2, cycleTimeSec);
    	}
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