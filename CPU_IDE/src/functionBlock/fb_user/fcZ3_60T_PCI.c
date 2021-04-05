/**************************************************************************
** Zone3_60T_PCI(Phase Current Imbalance)(FB722)
**************************************************************************/
#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_user\fcPrtCommon.h"
#include	"fb_code\fb_user\fcZ3_60T_PCI.h"

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
**	Zone3_60T_PCI(Phase Current Imbalance) FB	
**
**********************************************************************/
uint32	stdZ3_60T_PCIInitFunc
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
	fbInfoPtr->fbId			= FC0722_CODE;
    fbInfoPtr->inputNo		= FC0722_SPEC_NUM;
    fbInfoPtr->intlVarNo	= FC0722_VAR_NUM;
    fbInfoPtr->outputNo		= FC0722_OUTPUT_NUM;

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

uint32	stdZ3_60T_PCIRunFunc(uint32 taskId, uint32 fbMemAddr)
{
	uint32	status = NO_ERROR;
	float	cycleTimeSec;
	float	reset;
	float	enable;
	float	imax;
	float	imin;
	float	diff;
	
	strFC0722Info	fbInfo;
	
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
		fbInfo.t1 	 		= 0;
		fbInfo.t2 	 		= 0;
			
		fbInfo.o1_alarm		= NORMAL;		
		fbInfo.o2_conBlk	= NORMAL;		
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
				
			/* Min-Max & Difference Calculation */
			if(fbInfo.itrm_rms.a > fbInfo.itrm_rms.b)
			{
				imax = fbInfo.itrm_rms.a;
				imin = fbInfo.itrm_rms.b;
			}
			else
			{
				imax = fbInfo.itrm_rms.b;
				imin = fbInfo.itrm_rms.a;				
			}

			if(fbInfo.itrm_rms.c > imax)
				imax = fbInfo.itrm_rms.c;
		
			if(fbInfo.itrm_rms.c < imin)
				imin = fbInfo.itrm_rms.c;

			diff = imax - imin;
			
			/* Fault Detection */
			fbInfo.o1_alarm 	= ftOverDetect(diff, fbInfo.threshold1, fbInfo.setT1, &fbInfo.t1, cycleTimeSec);		
			fbInfo.o2_conBlk	= ftOverDetect(diff, fbInfo.threshold2, fbInfo.setT2, &fbInfo.t2, cycleTimeSec);		
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