/**************************************************************************
** Zone4_51Dharm(FB733)
**************************************************************************/
#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_user\fcPrtCommon.h"
#include	"fb_code\fb_user\fcZ4_51Dharm.h"

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
** Zone4_51Dharm FB	
**
**********************************************************************/
uint32	stdZ4_51DharmInitFunc
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
	fbInfoPtr->fbId			= FC0733_CODE;
    fbInfoPtr->inputNo		= FC0733_SPEC_NUM;
    fbInfoPtr->intlVarNo	= FC0733_VAR_NUM;
    fbInfoPtr->outputNo		= FC0733_OUTPUT_NUM;

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
	*intlVarTypePtr++ 		= REAL_TYPE|SIZE32_TYPE;
	*intlVarTypePtr++ 		= REAL_TYPE|SIZE32_TYPE;
	*intlVarTypePtr++ 		= REAL_TYPE|SIZE32_TYPE;
	*intlVarTypePtr++ 		= REAL_TYPE|SIZE32_TYPE;
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
	*outputTypePtr++		= INT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= INT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= INT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= INT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= INT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= INT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= INT_TYPE|SIZE32_TYPE;	
	*outputTypePtr++		= INT_TYPE|SIZE32_TYPE;	
		
    return(status);
}

uint32	stdZ4_51DharmRunFunc(uint32 taskId, uint32 fbMemAddr)
{
	uint32	status = NO_ERROR;
	float	cycleTimeSec;
	float	reset;
	float	enable;
		
	strFC0733Info	fbInfo;
	
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
    	fbInfo.t1.a 			= 0;
    	fbInfo.t1.b 			= 0;
    	fbInfo.t1.c 			= 0;
    	fbInfo.t2.a 			= 0;
    	fbInfo.t2.b 			= 0;
    	fbInfo.t2.c 			= 0;
    	fbInfo.t_h1.a 			= 0;
    	fbInfo.t_h1.b 			= 0;
    	fbInfo.t_h1.c 			= 0;
    	fbInfo.t_h2.a 			= 0;
    	fbInfo.t_h2.b 			= 0;
    	fbInfo.t_h2.c 			= 0;

		fbInfo.o1_alarm.a 		= NORMAL;		
		fbInfo.o1_alarm.b 		= NORMAL;		
		fbInfo.o1_alarm.c 		= NORMAL;		
		fbInfo.o2_conBlk.a 		= NORMAL;
		fbInfo.o2_conBlk.b 		= NORMAL;
		fbInfo.o2_conBlk.c 		= NORMAL;
		fbInfo.o1_alarm_h.a 	= NORMAL;		
		fbInfo.o1_alarm_h.b 	= NORMAL;		
		fbInfo.o1_alarm_h.c 	= NORMAL;		
		fbInfo.o2_conBlk_h.a	= NORMAL;
		fbInfo.o2_conBlk_h.b	= NORMAL;
		fbInfo.o2_conBlk_h.c	= NORMAL;
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
			fbInfo.o1_alarm.a 		= ftOverDetect(fbInfo.ithd.a, fbInfo.threshold1, fbInfo.setT1, &fbInfo.t1.a, cycleTimeSec);		
			fbInfo.o1_alarm.b 		= ftOverDetect(fbInfo.ithd.b, fbInfo.threshold1, fbInfo.setT1, &fbInfo.t1.b, cycleTimeSec);		
			fbInfo.o1_alarm.c 		= ftOverDetect(fbInfo.ithd.c, fbInfo.threshold1, fbInfo.setT1, &fbInfo.t1.c, cycleTimeSec);		
			fbInfo.o2_conBlk.a 		= ftOverDetect(fbInfo.ithd.a, fbInfo.threshold2, fbInfo.setT2, &fbInfo.t2.a, cycleTimeSec);
			fbInfo.o2_conBlk.b 		= ftOverDetect(fbInfo.ithd.b, fbInfo.threshold2, fbInfo.setT2, &fbInfo.t2.b, cycleTimeSec);
			fbInfo.o2_conBlk.c 		= ftOverDetect(fbInfo.ithd.c, fbInfo.threshold2, fbInfo.setT2, &fbInfo.t2.c, cycleTimeSec);
			fbInfo.o1_alarm_h.a 	= ftOverDetect(fbInfo.ih.a, fbInfo.threshold_h1, fbInfo.setT_h1, &fbInfo.t_h1.a, cycleTimeSec);		
			fbInfo.o1_alarm_h.b 	= ftOverDetect(fbInfo.ih.b, fbInfo.threshold_h1, fbInfo.setT_h1, &fbInfo.t_h1.b, cycleTimeSec);		
			fbInfo.o1_alarm_h.c 	= ftOverDetect(fbInfo.ih.c, fbInfo.threshold_h1, fbInfo.setT_h1, &fbInfo.t_h1.c, cycleTimeSec);		
			fbInfo.o2_conBlk_h.a	= ftOverDetect(fbInfo.ih.a, fbInfo.threshold_h2, fbInfo.setT_h2, &fbInfo.t_h2.a, cycleTimeSec);
			fbInfo.o2_conBlk_h.b	= ftOverDetect(fbInfo.ih.b, fbInfo.threshold_h2, fbInfo.setT_h2, &fbInfo.t_h2.b, cycleTimeSec);
			fbInfo.o2_conBlk_h.c	= ftOverDetect(fbInfo.ih.c, fbInfo.threshold_h2, fbInfo.setT_h2, &fbInfo.t_h2.c, cycleTimeSec);
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