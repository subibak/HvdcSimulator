/**************************************************************************
** Zone2_87N_REFP(Restricted Earth Fault Protection)(FB0710)
**************************************************************************/
#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_user\fcPrtCommon.h"
#include	"fb_code\fb_user\fcZ2_87N_REFP.h"

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
**	Zone2_87N_REFP(Restricted Earth Fault Protection) FB 	
**
**********************************************************************/
uint32	stdZ2_87N_REFPInitFunc
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
	fbInfoPtr->fbId			= FC0710_CODE;
    fbInfoPtr->inputNo		= FC0710_SPEC_NUM;
    fbInfoPtr->intlVarNo	= FC0710_VAR_NUM;
    fbInfoPtr->outputNo		= FC0710_OUTPUT_NUM;

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
	*outputTypePtr			= INT_TYPE|SIZE32_TYPE;	
		
    return(status);
}

uint32	stdZ2_87N_REFPRunFunc(uint32 taskId, uint32 fbMemAddr)
{
	uint32	status = NO_ERROR;
	float	cycleTimeSec;
	float	reset;
	float	enable;
	STF_3P	idiffRef;

	strFC0710Info	fbInfo;
	
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
						
		fbInfo.o1_emst.a	= 0;
		fbInfo.o1_emst.b	= 0;
		fbInfo.o1_emst.c	= 0;
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
					
			/* idiff_ref calculation  */		
			idiffRef.a = fbInfo.ip.a - fbInfo.in;
			idiffRef.b = fbInfo.ip.b - fbInfo.in;
			idiffRef.c = fbInfo.ip.c - fbInfo.in;	

			/* Fault Detection */			
			fbInfo.o1_emst.a = ftOverDetect(idiffRef.a, fbInfo.delta, fbInfo.setT, &fbInfo.t1.a, cycleTimeSec);
			fbInfo.o1_emst.b = ftOverDetect(idiffRef.b, fbInfo.delta, fbInfo.setT, &fbInfo.t1.b, cycleTimeSec);
			fbInfo.o1_emst.c = ftOverDetect(idiffRef.c, fbInfo.delta, fbInfo.setT, &fbInfo.t1.c, cycleTimeSec);
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