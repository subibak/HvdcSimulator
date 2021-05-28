/**************************************************************************
** Zone4_27D_DUP(DC Undervoltage Protection)(FB724)
**************************************************************************/
#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_user\fcPrtCommon.h"
#include	"fb_code\fb_user\fcZ4_27D_DUP.h"

/**************************************************************************
**		외부 함수 
**************************************************************************/
extern uint32	readRuntimeFbData(uint32, uint32, uint32, uint32 *);
extern uint32	writeRuntimeFbData(uint32, uint32, uint32, uint32 *);
extern void 	setErrorCode(uint8 *, uint32,uint8 *,uint32);
extern uint32 	getLogicTaskCycletime(uint32 , uint32 , float *);
extern uint32	ftUnderDetect(float, float, float, float *, float);

/*********************************************************************
**	
**	Zone4_27D_DUP(DC Undervoltage Protection) FB 	
**
**********************************************************************/
uint32	stdZ4_27D_DUPInitFunc
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
	fbInfoPtr->fbId			= FC0724_CODE;
    fbInfoPtr->inputNo		= FC0724_SPEC_NUM;
    fbInfoPtr->intlVarNo	= FC0724_VAR_NUM;
    fbInfoPtr->outputNo		= FC0724_OUTPUT_NUM;

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

	/**************************************************
	**	FB 내부 변수 타입 정의
	**************************************************/
	*intlVarTypePtr++ 		= REAL_TYPE|SIZE32_TYPE;
	*intlVarTypePtr++ 		= REAL_TYPE|SIZE32_TYPE;
	*intlVarTypePtr++ 		= REAL_TYPE|SIZE32_TYPE;
	*intlVarTypePtr++ 		= REAL_TYPE|SIZE32_TYPE;

	/**************************************************
	**	FB 출력 타입 정의
	**************************************************/
	*outputTypePtr++		= INT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= INT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= INT_TYPE|SIZE32_TYPE;	
	*outputTypePtr++		= INT_TYPE|SIZE32_TYPE;	
		
    return(status);
}

uint32	stdZ4_27D_DUPRunFunc(uint32 taskId, uint32 fbMemAddr)
{
	uint32	status = NO_ERROR;
	float	cycleTimeSec;
	float	reset;
	float	enable;
	float	abs_vdc_up;
	float	abs_vdc_low;

	strFC0724Info	fbInfo;
	
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
		fbInfo.t1_up 		 = 0;
		fbInfo.t2_up 		 = 0;
		fbInfo.t1_low 		 = 0;
		fbInfo.t2_low		 = 0;
					
		fbInfo.o1_conBlk_up	 = NORMAL;		
		fbInfo.o2_emst_up	 = NORMAL;
		fbInfo.o1_conBlk_low = NORMAL;		
		fbInfo.o2_emst_low	 = NORMAL;				
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
					
			/* Fault Detection */
			 abs_vdc_up  = ABSt(fbInfo.vdc_up);
			 abs_vdc_low = ABSt(fbInfo.vdc_low);

			fbInfo.o1_conBlk_up	 = ftUnderDetect(abs_vdc_up,  fbInfo.threshold1, fbInfo.setT1, &fbInfo.t1_up,  cycleTimeSec);
			fbInfo.o2_emst_up	 = ftUnderDetect(abs_vdc_up,  fbInfo.threshold2, fbInfo.setT2, &fbInfo.t2_up,  cycleTimeSec);
			fbInfo.o1_conBlk_low = ftUnderDetect(abs_vdc_low, fbInfo.threshold1, fbInfo.setT1, &fbInfo.t1_low, cycleTimeSec);
			fbInfo.o2_emst_low	 = ftUnderDetect(abs_vdc_low, fbInfo.threshold2, fbInfo.setT2, &fbInfo.t2_low, cycleTimeSec);
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