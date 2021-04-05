/**************************************************************************
** Zone1_27_ABUV(154kV AC Bus UV)(FB0701)
*************************************************************************/
#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_user\fcPrtCommon.h"
#include	"fb_code\fb_user\fcZ1_27_ABUV.h"

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
**	Zone1_27_ABUV(154kV AC Bus UV) FB  	
**
**********************************************************************/
uint32	stdZ1_27_ABUVInitFunc
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
	fbInfoPtr->fbId			= FC0701_CODE;
    fbInfoPtr->inputNo		= FC0701_SPEC_NUM;
    fbInfoPtr->intlVarNo	= FC0701_VAR_NUM;
    fbInfoPtr->outputNo		= FC0701_OUTPUT_NUM;

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
	*intlVarTypePtr++ 		= REAL_TYPE|SIZE32_TYPE;
	*intlVarTypePtr++ 		= REAL_TYPE|SIZE32_TYPE;
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
	*outputTypePtr++		= INT_TYPE|SIZE32_TYPE;
	*outputTypePtr			= INT_TYPE|SIZE32_TYPE;	
		
    return(status);
}

uint32	stdZ1_27_ABUVRunFunc(uint32 taskId, uint32 fbMemAddr)
{
	uint32	status = NO_ERROR;
	float	cycleTimeSec;
	float	reset;
	float	enable;

	strFC0701Info	fbInfo;
	
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
		fbInfo.t1.a 		= 0;
		fbInfo.t1.b 		= 0;
		fbInfo.t1.c 		= 0;
		fbInfo.t2.a 		= 0;
		fbInfo.t2.b 		= 0;
		fbInfo.t2.c 		= 0;
			
		fbInfo.o1_alarm.a 	= NORMAL;		
		fbInfo.o1_alarm.b 	= NORMAL;		
		fbInfo.o1_alarm.c 	= NORMAL;		
		fbInfo.o2_conBlk.a 	= NORMAL;	
		fbInfo.o2_conBlk.b	= NORMAL;	
		fbInfo.o2_conBlk.c 	= NORMAL;	
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
			fbInfo.o1_alarm.a 	= ftUnderDetect(fbInfo.vrms.a, fbInfo.threshold1, fbInfo.setT1, &fbInfo.t1.a, cycleTimeSec);
			fbInfo.o1_alarm.b 	= ftUnderDetect(fbInfo.vrms.b, fbInfo.threshold1, fbInfo.setT1, &fbInfo.t1.b, cycleTimeSec);
			fbInfo.o1_alarm.c 	= ftUnderDetect(fbInfo.vrms.c, fbInfo.threshold1, fbInfo.setT1, &fbInfo.t1.c, cycleTimeSec);
			fbInfo.o2_conBlk.a	= ftUnderDetect(fbInfo.vrms.a, fbInfo.threshold2, fbInfo.setT2, &fbInfo.t2.a, cycleTimeSec);
			fbInfo.o2_conBlk.b	= ftUnderDetect(fbInfo.vrms.b, fbInfo.threshold2, fbInfo.setT2, &fbInfo.t2.b, cycleTimeSec);
			fbInfo.o2_conBlk.c	= ftUnderDetect(fbInfo.vrms.c, fbInfo.threshold2, fbInfo.setT2, &fbInfo.t2.c, cycleTimeSec);
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