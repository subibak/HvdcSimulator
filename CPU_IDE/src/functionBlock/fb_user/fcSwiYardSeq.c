/**************************************************************************
** Switch Yard Sequence(FB0740)
**************************************************************************/
#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_user\fcSwiYardSeq.h"

/**************************************************************************
**		외부 함수 
**************************************************************************/
extern uint32	readRuntimeFbData(uint32, uint32, uint32, uint32 *);
extern uint32	writeRuntimeFbData(uint32, uint32, uint32, uint32 *);
extern void 	setErrorCode(uint8 *, uint32,uint8 *,uint32);
extern uint32 	getLogicTaskCycletime(uint32 , uint32 , float *);

/*********************************************************************
**	
**	Zone1_59harm_Harmonic Distortion Protection FB 	
**
**********************************************************************/
uint32	stdZ4_SwiYardSeqInitFunc
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
	fbInfoPtr->fbId			= FC0740_CODE;
    fbInfoPtr->inputNo		= FC0740_SPEC_NUM;
    fbInfoPtr->intlVarNo	= FC0740_VAR_NUM;
    fbInfoPtr->outputNo		= FC0740_OUTPUT_NUM;

	/**************************************************
	**	FB 입력 타입 정의
	**************************************************/
	*inputTypePtr++			= UINT_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= UINT_TYPE|SIZE32_TYPE;
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
	*inputTypePtr			= REAL_TYPE|SIZE32_TYPE;

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
	*intlVarTypePtr 		= REAL_TYPE|SIZE32_TYPE;
	
	/**************************************************
	**	FB 출력 타입 정의
	**************************************************/
	*outputTypePtr++		= UINT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= UINT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= UINT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= UINT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= UINT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= UINT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= UINT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= UINT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= UINT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= UINT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= UINT_TYPE|SIZE32_TYPE;	
	*outputTypePtr			= UINT_TYPE|SIZE32_TYPE;		
		
    return(status);
}

uint32	stdZ4_SwiYardSeqRunFunc(uint32 taskId, uint32 fbMemAddr)
{
	uint32	status = NO_ERROR;
	float	cycleTimeSec;
	uint32	sysStatus;
	float	reset;
	float	enable;

	strFC0740Info	fb;
	
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbMemAddr,
                                (uint32 *)&fb
						   );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }

	/* Yard Switch Sequcenc Algorithm */
	sysStatus = 0x3 & fb.sysMode.bit.status;

	if(sysStatus == SM_STS_ERROR)
	{

	} 
	else if(sysStatus == SM_STS_UNREADY)
   	{
	   	/* System Initialization */
	   	// State Initialization - Earth, Standy ... 
	   	fb.PrevState = Seq0000;
	   	fb.CurrState = Seq0000;
	   	fb.NextState = Seq0000;

	   	// Step Initialization - Seq0000, 0200 ...
	   	fb.PrevStep = Seq0000;
	   	fb.CurrStep = Seq0000;
	   	fb.NextStep	= Seq0000;

    	// HMI SEQ Initialization
	   	fb.seqSts1.all = SEQ_COMM_ALL_RESET;
	   	fb.seqSts2.all = SEQ_COMM_ALL_RESET;

    	// Yard Switch Command  
	   	fb.yardSwiComm.bit.cbx01 = YARD_SWITCH_COMM_OPEN;
	   	fb.yardSwiComm.bit.dsx01 = YARD_SWITCH_COMM_OPEN;
	   	fb.yardSwiComm.bit.ds311 = YARD_SWITCH_COMM_OPEN;
	   	fb.yardSwiComm.bit.ds321 = YARD_SWITCH_COMM_OPEN;
	    fb.yardSwiComm.bit.dsx02 = YARD_SWITCH_COMM_OPEN;

	   	fb.yardSwiComm.bit.esx01 = YARD_SWITCH_COMM_CLOSE;
	   	fb.yardSwiComm.bit.esx02 = YARD_SWITCH_COMM_CLOSE;
	   	fb.yardSwiComm.bit.esx11 = YARD_SWITCH_COMM_CLOSE;
	   	fb.yardSwiComm.bit.esx12 = YARD_SWITCH_COMM_CLOSE;
	   	fb.yardSwiComm.bit.esx21 = YARD_SWITCH_COMM_CLOSE;
	   	fb.yardSwiComm.bit.esx22 = YARD_SWITCH_COMM_CLOSE;
	}
	else if(sysStatus == SM_STS_READY)
	{
		
		fb.CurrStep = fb.NextStep;
		if(fb.seqOp == SO_SEQOP_START)
		{
			switch(CurrStep)
			{
				case Seq0000 : 	// Undefined
					funcSeq0000(&fb);	break;
				case Seq0200 : 	// 	
					funcSeq0200(&fb);	break;
				case Seq0210 :	//	
					funcSeq0210(&fb);	break;
				case Seq0220 :	//	
					funcSeq0220(&fb);	break;
				default : 				break;
			}
		}
	}



	/*  */
    
    status = writeRuntimeFbData(taskId,
    							LOGIC_ID,
                                fbMemAddr,
                                (uint32 *)&fb
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }
	
	return(status);
}