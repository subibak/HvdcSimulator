/**************************************************************************
** Zone2_87_TDP(Transformer Differential Protection)(FB0713)
**************************************************************************/
#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_user\fcPrtCommon.h"
#include	"fb_code\fb_user\fcZ2_87_TDP.h"

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
**	Zone2_87_TDP(Transformer Differential Protection) FB 	
**
**********************************************************************/
uint32	stdZ2_87_TDPInitFunc
				(
			uint32			*inputTypePtr,
            uint32			*intlVarTypePtr,
            uint32			*outputTypePtr,
            strFbDefInfo	*fbInfoPtr
            	)
{
	uint32	status = NO_ERROR;
	
	fbInfoPtr->fbId			= FC0713_CODE;
    fbInfoPtr->inputNo		= FC0713_SPEC_NUM;
    fbInfoPtr->intlVarNo	= FC0713_VAR_NUM;
    fbInfoPtr->outputNo		= FC0713_OUTPUT_NUM;

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
	*inputTypePtr++			= INT_TYPE|SIZE32_TYPE;
	
	*inputTypePtr++			= INT_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= INT_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= INT_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= INT_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= INT_TYPE|SIZE32_TYPE;
	
	*inputTypePtr++			= INT_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= REAL_TYPE|SIZE32_TYPE;		
	*inputTypePtr			= REAL_TYPE|SIZE32_TYPE;

	/**************************************************
	**	FB 내부 변수 타입 정의
	**************************************************/
	*intlVarTypePtr++ 		= REAL_TYPE|SIZE32_TYPE;
	*intlVarTypePtr++ 		= REAL_TYPE|SIZE32_TYPE;
	*intlVarTypePtr 		= REAL_TYPE|SIZE32_TYPE;
	
	/**************************************************
	**	FB 출력 타입 정의
	**************************************************/
	*outputTypePtr++		= INT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= INT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= INT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= INT_TYPE|SIZE32_TYPE;
	*outputTypePtr			= INT_TYPE|SIZE32_TYPE;
		
    return(status);
}

uint32	stdZ2_87_TDPRunFunc(uint32 taskId, uint32 fbMemAddr)
{
	uint32	status = NO_ERROR;
	float	cycleTimeSec;
	float	reset;
	float	enable;
	STF_3P 	idiff_tr;
	float	alarm_s;
	float	emst_s;
	
	strFC0713Info	fbInfo;
	
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
		fbInfo.t1.a			= 0;
		fbInfo.t1.b			= 0;
		fbInfo.t1.c			= 0;

		fbInfo.o1_alarm 	= 0;
		fbInfo.o2_emst.a 	= 0;
		fbInfo.o2_emst.b	= 0;
		fbInfo.o2_emst.c	= 0;
		fbInfo.o2_emst.s	= 0;
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
							
			/* Fault Calculation */
			idiff_tr.a = fbInfo.ipcc.a - fbInfo.ig;	
			idiff_tr.b = fbInfo.ipcc.b - fbInfo.ig;
			idiff_tr.c = fbInfo.ipcc.c - fbInfo.ig;
			
			alarm_s = fbInfo.a26W + fbInfo.a96B1;
			emst_s 	= fbInfo.t26W + fbInfo.t96P + fbInfo.t96D + fbInfo.t96B2 + fbInfo.t96T;

			/* Fault Detection */	
			if(alarm_s >= 1)
				fbInfo.o1_alarm = FAULT;
			else
				fbInfo.o1_alarm = NORMAL;
			
			fbInfo.o2_emst.a 	= ftOverDetect(idiff_tr.a, fbInfo.delta, fbInfo.setT, &fbInfo.t1.a, cycleTimeSec);
			fbInfo.o2_emst.b 	= ftOverDetect(idiff_tr.b, fbInfo.delta, fbInfo.setT, &fbInfo.t1.b, cycleTimeSec);
			fbInfo.o2_emst.c 	= ftOverDetect(idiff_tr.c, fbInfo.delta, fbInfo.setT, &fbInfo.t1.c, cycleTimeSec);
			
			if(emst_s >= 1)
				fbInfo.o2_emst.s = FAULT;
			else
				fbInfo.o2_emst.s = NORMAL;	
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