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
	*inputTypePtr++			= UINT_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= UINT_TYPE|SIZE32_TYPE;
	*inputTypePtr++			= UINT_TYPE|SIZE32_TYPE;
	*inputTypePtr			= UINT_TYPE|SIZE32_TYPE;

	/**************************************************
	**	FB 내부 변수 타입 정의
	**************************************************/
	*intlVarTypePtr++ 		= UINT_TYPE|SIZE32_TYPE;
	*intlVarTypePtr++ 		= UINT_TYPE|SIZE32_TYPE;
	*intlVarTypePtr++ 		= UINT_TYPE|SIZE32_TYPE;
	*intlVarTypePtr++ 		= UINT_TYPE|SIZE32_TYPE;
	*intlVarTypePtr 		= UINT_TYPE|SIZE32_TYPE;
	
	/**************************************************
	**	FB 출력 타입 정의
	**************************************************/
	*outputTypePtr++		= UINT_TYPE|SIZE32_TYPE;
	*outputTypePtr++		= UINT_TYPE|SIZE32_TYPE;
	*outputTypePtr			= UINT_TYPE|SIZE32_TYPE;		
		
    return(status);
}

uint32	stdZ4_SwiYardSeqRunFunc(uint32 taskId, uint32 fbMemAddr)
{
	uint32	status = NO_ERROR;
	uint32	sysStatus;

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


	if(fb.sysMode.bit.status == SM_STS_UNREADY | fb.sysMode.bit.status == SM_STS_ERROR)
   	{
	   	/* System Initialization */
	   	// Step Initialization - Seq0000, 0200 ...
	   	fb.PrevStep = Seq0000;
	   	fb.CurrStep = Seq0000;
	   	fb.NextStep	= Seq0000;

    	// HMI SEQ Initialization
	   	fb.seqSts1.all = SEQ_COMM_ALL_RESET;
	   	fb.seqSts2.all = SEQ_COMM_ALL_RESET;

    	// Yard Switch Command  
	   	fb.yardSwiComm.bit.cbx01 = YARD_SWITCH_COMM_OPEN;
	   	fb.yardSwiComm.bit.cbx91 = YARD_SWITCH_COMM_OPEN;
	   	fb.yardSwiComm.bit.dsx01 = YARD_SWITCH_COMM_OPEN;
	   	fb.yardSwiComm.bit.dsx02 = YARD_SWITCH_COMM_OPEN;
	    fb.yardSwiComm.bit.ds3x1 = YARD_SWITCH_COMM_OPEN;

	   	fb.yardSwiComm.bit.esx01_02 = YARD_SWITCH_COMM_CLOSE;
	   	fb.yardSwiComm.bit.esx11_21 = YARD_SWITCH_COMM_CLOSE;
	   	fb.yardSwiComm.bit.esx12_22 = YARD_SWITCH_COMM_CLOSE;

	   	// Flag Sequence Process Initialization
#if 0/*임시 */
	   	fb.flagSeqStatus = FLAG_SEQ_UNCOMPLETE;

	   	// Flag Sequence Opeation Initialization 
	   	fb.flagSeqOp = FLAG_SEQ_STOP;
#endif
	}
	else if(fb.sysMode.bit.status == SM_STS_READY)
	{
		// fb.CurrStep = fb.NextStep;
		// if(fb.sysMode.bit.seqOp == SM_SEQOP_START)
		// {
		// 	// Active Power Mode(P Mode, Vdc Mode) 
		// 	if(fb.sysMode.bit.actPwr == SM_ACTPWR_P) // P Mode
		// 	{
		// 		// Sequence Direction(Start, Stop, Hold)
		// 		if(fb.sysMode.bit.seqDir == SM_SEQDIR_START)
		// 		{
		// 			// Opposite Converter Status
		// 			if(fb.oSeqSts2.bit.seq0800 == SEQ_COMM_COMPLETE)
		// 				fb.CurrStep = fb.NextStep;
		// 			else
		// 				fb.CurrStep = Seq0000;
		// 		}
		// 		else if(fb.sysMode.bit.seqDir == SM_SEQDIR_STOP)
					
		// 		else if(fb.sysMode.bit.seqDir == SM_SEQDIR_NODIR)
		// 		else
		// 			fb.CurrStep = Seq0000;
				
		// 	}	
		// 	else // Vdc Mode
		// 	{
		// 		// Sequence Direction(Start, Stop, Hold)
		// 		if(fb.sysMode.bit.seqDir == SM_SEQDIR_START)
		// 		{
		// 			fb.CurrStep = fb.NextStep;
		// 		}
		// 		else if(fb.sysMode.bit.seqDir == SM_SEQDIR_STOP)
					
		// 		else if(fb.sysMode.bit.seqDir == SM_SEQDIR_NODIR)
		// 		else
		// 	}

		// System Mode Check
		if(fb.sysMode.bit.operation == SM_OP_AUTOMATIC)
		{
			// Automatic
			
		}
		else
		{
			// Manual

		}

		// Sequence Operation START/STOP/HOLD
		if(fb.sysMode.bit.seqOp == SM_SEQOP_START)		// Sequence: Start
		{
			// System Mode Check
			if(fb.sysMode.bit.operation == SM_OP_MANUAL)// Manual Mode
			{			
				if(fb.flagSeqComlete == FLAG_SEQ_COMPLETE)
					fb.flagSeqOp = FLAG_SEQ_STOP;
				else
					fb.flagSeqOp = FLAG_SEQ_START;
			}
			else 										// Automatic Mode
				fb.flagSeqOp = FLAG_SEQ_START;			
		}

		else if(fb.sysMode.bit.seqOp == SM_SEQOP_STOP)	// Sequence: Stop
			fb.flagSeqOp = FLAG_SEQ_STOP;
		if(fb.flagSeqOp == FLAG_SEQ_START)
			fb.CurrStep = fb.NextStep;
		else if(fb.flagSeqOp == FLAG_SEQ_STOP)
			fb.CurrStep = fb.PrevStep;

		switch(fb.CurrStep)
		{
			case Seq0000 : 	// Undefined
				funcSeq0000(&fb);	break;
			case Seq0200 : 	// 	
				funcSeq0200(&fb);	break;
			case Seq0210 :	//	
				funcSeq0210(&fb);	break;
			case Seq0220 :	//	
				funcSeq0220(&fb);	break;
			case Seq0230 : 	// 
				funcSeq0230(&fb);	break;
			case Seq0240 : 	// 	
				funcSeq0240(&fb);	break;
			case Seq0250 :	//	
				funcSeq0250(&fb);	break;
			case Seq0260 :	//	
				funcSeq0260(&fb);	break;
			case Seq0320 : 	// 	
				funcSeq0320(&fb);	break;
			case Seq0330 :	//	
				funcSeq0330(&fb);	break;
			case Seq0340 :	//	
				funcSeq0340(&fb);	break;
			case Seq0350 : 	// 
				funcSeq0350(&fb);	break;
			case Seq0360 : 	// 	
				funcSeq0360(&fb);	break;
			case Seq0400 :	//	
				funcSeq0400(&fb);	break;
			case Seq0410 :	//	
				funcSeq0410(&fb);	break;	
			case Seq0510 : 	// 	
				funcSeq0510(&fb);	break;
			case Seq0520 :	//	
				funcSeq0520(&fb);	break;
			case Seq0600 :	//	
				funcSeq0600(&fb);	break;
			case Seq0610 : 	// 
				funcSeq0610(&fb);	break;
			case Seq0620 : 	// 	
				funcSeq0620(&fb);	break;
			case Seq0630 :	//	
				funcSeq0630(&fb);	break;
			case Seq0640 :	//	
				funcSeq0640(&fb);	break;
			case Seq0650 : 	// 	
				funcSeq0650(&fb);	break;
			case Seq0660 :	//	
				funcSeq0660(&fb);	break;
			case Seq0670 :	//	
				funcSeq0670(&fb);	break;	
			case Seq0720 : 	// 	
				funcSeq0720(&fb);	break;
			case Seq0730 :	//	
				funcSeq0730(&fb);	break;
			case Seq0740 :	//	
				funcSeq0740(&fb);	break;
			case Seq0760 : 	// 
				funcSeq0760(&fb);	break;
			case Seq0770 : 	// 	
				funcSeq0770(&fb);	break;
			case Seq0800 :	//	
				funcSeq0800(&fb);	break;	
			case SeqBypass :		break;														
			default : 				break;
		}
		fb.PrevStep = fb.CurrStep;		
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