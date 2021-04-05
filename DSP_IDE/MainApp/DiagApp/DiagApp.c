

#include "Core/Signal.h"
#include "DiagApp.h"

#include "MainApp/DebugApp/DebugApp.h"

#include "Hal/HalAi.h"


extern uint32_t ulIntStart;
extern uint32_t ulIntEnd;

extern uint32_t ulSyncSnd;
extern uint32_t ulSyncRcv;

extern uint32_t ulControlStartCnt;
extern uint32_t ulControlEndCnt;

extern CORE_STATE eManagerState;
extern CORE_STATE eControlState;
extern CORE_STATE ePrevManagerState;
extern CORE_STATE ePrevControlState;

extern uint32_t ulSignalQofManager;
extern uint32_t ulSignalQofControl;


void DiagAppInit(void)
{

	/* Step 1: Init App Variables */

	/* Step 2: Init Ial Layer */
	IalDiagInit();


	/* Step 3:  */


	/* Step 4:  */




	

}

uint32_t	ulCntTemp;

#if 0
uint32_t	ulReadZero=0, ulReadZeroMax=0, ulReadNormal = 0;
uint32_t	ulReadZeroFlag = 0;
#endif

int32_t	lMaxOvrTimeCnt = 0;
int32_t lOvrTimeCnt = 0;

void DiagAppRun(uint32_t ulState)
{
	uint32_t	ulDiag = IALDIAG_ERR_NONE;

	/* Step 1: Update Live Cnt */
	IalDiagUpdateLiveCnt();

	// Master의 경우, CRC, NoPkt 에러는 확인하지 않음
	if(ulMasterSlave == SYNC_MASTER)
	{
		BitClear(ulState, 0xDB6);
	}
	else
	{
		BitSet(ulState, (ulState&IALDIAG_ERR_LOST_SYNC)>>13);
	}

	/* Step 2: Run Diag */
	ulDiag = ulState;
	
	// Check Task OverTime
	ulCntTemp = ulControlEndCnt;
	if(ulCntTemp != 0)
	{
		int32_t	lDiff;
		lDiff = ulIntStart - ulCntTemp;
//		if(ulIntStart > ulCntTemp)
		if(lDiff != 0)
		{
			lOvrTimeCnt++;
			if(lOvrTimeCnt > lMaxOvrTimeCnt)
			{
				lMaxOvrTimeCnt = lOvrTimeCnt;
				ulDiag = ulDiag | IALDIAG_ERR_TASK_OVERTIME;
				
			}
		}
		else
		{
			lOvrTimeCnt = 0;
		}
//		ulReadZero = 0;
//		ulReadNormal++;ulReadZeroFlag = 0;
	}
#if 0
	else
	{
		ulReadZero++;
		ulReadZeroFlag++;
		if(ulReadZeroFlag > 0)
		{
			ulReadZeroMax = ulReadZeroFlag;
		}

	}
#endif
	// Check FComm Status // 광통신 HDL Fix 후 수정 필요함.
	ulDiag = ulDiag | IalDiagFCommCheck();

	/* Step 3: Write Diag State */
	IalDiagSet(ulDiag);

}


uint32_t DebugAICmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{

	uint32_t ulValue;
	uint32_t ulArg1;
	uint32_t ulLoop;
	float fValue;
	static float fTempValue[16] = {998.32,4997.263,9999.23,-4999.1,-996.371,0.135,0.325,0.4125,0.44425,0.1125,0.7775,0.3455,0.3455,0.345,0.5545};
	static float bais = 0;

	if(pToken->wOp == (NA|QU))
	{
		
		// Read All of AI Input Values
		for(ulLoop = 0; ulLoop < HALAI_CH_NUM; ulLoop++)
		{

#if 0
			ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine+ulCmdResultLen), "%d:\t0x%04X\t%4.4f\n\r"
											,ulLoop, mV2Hex(ADC_INPUT_MAX, ADC_INPUT_MIN, ADC_INPUT_MAX_HEX, ADC_INPUT_MIN_HEX, fTempValue[ulLoop-1])&0xFFFF, fTempValue[ulLoop-1]);
#else
			fValue = HalAiRead(ulLoop);

			ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine+ulCmdResultLen), "%d:\t0x%04X\t%1.4f\n\r"
											,ulLoop, mV2Hex(ADC_INPUT_MAX, ADC_INPUT_MIN, ADC_INPUT_MAX_HEX, ADC_INPUT_MIN_HEX, fValue)&0xFFFF, fValue);
#endif
		}


		ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "\n");
	}
	else if(pToken->wOp == (NA|EQ|AR))
	{

		if(pToken->wArgsFound == 1)
		{
		
			ulArg1 = atol((char *)pToken->pArg[0]);

			if(ulArg1 < HALAI_CH_NUM)
			{
#if 0
				ulCmdResultLen = sprintf((char *)(pToken->abWorkingCmdLine+ulCmdResultLen), "%d:\t0x%04X\t%4.4f\n\r"
												,ulArg1, mV2Hex(ADC_INPUT_MAX, ADC_INPUT_MIN, ADC_INPUT_MAX_HEX, ADC_INPUT_MIN_HEX, fTempValue[ulArg1-1])&0xFFFF, fTempValue[ulArg1-1]);
#else
				// Read AI Input Values of Channel #
				ulValue = HalAiRead(ulArg1);

				ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine+ulCmdResultLen), "%d:\t0x%04X\t%1.4f\n\r"
												,ulArg1, mV2Hex(ADC_INPUT_MAX, ADC_INPUT_MIN, ADC_INPUT_MAX_HEX, ADC_INPUT_MIN_HEX, fValue)&0xFFFF, fValue);
#endif
				ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "\n");
			}
			else
			{
				ulCmdResult = CMD_EXEC_ERROR;
			}
		}
		else
		{
			ulCmdResult = CMD_EXEC_ERROR;
		}
	}
	else
	{
		// Cmd Error
		ulCmdResult = CMD_EXEC_ERROR;
	}
	return ulCmdResult;

}


//	STR_MAP_TYPE	stStateString[] = {
//		{STATE_INIT,			"INIT"},
//		{STATE_WAIT_INIT,		"Wait Control INIT"},
//		{STATE_INIT_DONE,		"INIT DONE"},
//		{STATE_MASTER_NORMAL,	"READY"},
//		{STATE_SLAVE_NORMAL,	"READY"},
//		{STATE_DOWNLOAD, 		"DLOAD"},
//		{STATE_FAULT,			"FAULT"},
//		{0xFFFF,				"Unknown"}
//	};
//	uint32_t	ulStateStrLen;
//	
//	STR_MAP_TYPE	stCauseString[] = {
//		{IALDIAG_ERR_FCOM1_LINK,			"FCom1 Discon"},
//		{IALDIAG_ERR_FCOM1_TIMEOUT,			"FCom1 NoRsp"},
//		{IALDIAG_ERR_FCOM1_FRAME,			"FCom1 CRCErr"},
//		{IALDIAG_ERR_FCOM2_LINK,			"FCom2 Discon"},
//		{IALDIAG_ERR_FCOM2_TIMEOUT,			"FCom2 NoRsp"},
//		{IALDIAG_ERR_FCOM2_FRAME,			"FCom2 CRCErr"},
//		{IALDIAG_ERR_FCOM3_LINK,			"FCom3 Discon"},
//		{IALDIAG_ERR_FCOM3_TIMEOUT,			"FCom3 NoRsp"},
//		{IALDIAG_ERR_FCOM3_FRAME,			"FCom3 CRCErr"},
//		{IALDIAG_ERR_FCOM4_LINK,			"FCom4 Discon"},
//		{IALDIAG_ERR_FCOM4_TIMEOUT,			"FCom4 NoRsp"},
//		{IALDIAG_ERR_FCOM4_FRAME,			"FCom4 CRCErr"},
//	
//		{IALDIAG_ERR_TASK_OVERTIME,			"Task OverTime"},
//		{IALDIAG_ERR_MBRD_DISC, 			"MBrd Discon"},
//		{IALDIAG_ERR_NONE, 					"No Error"},
//	
//		{0xFFFF, "Unknown"}
//	};
//	uint32_t	ulCauseStrLen;


uint32_t DebugFLTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)

{
	uint32_t ulValue;

	if(pToken->wOp == (NA|QU))
	{
		
		ulCmdResultLen = sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "STATE: %d, %d\n\r", eManagerState, eControlState);
		ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "CAUSE: 0x%08x\n\r", IalDiagGet());

		ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "\n");

	}	
	else if(pToken->wOp == (NA|EQ|AR))
	{
		ulValue = atol((char *)pToken->pArg[0]);
		
		if(ulValue == DIAGAPP_FALT_RESET)
		{
			SetState(&ePrevControlState, &eControlState, STATE_INIT);
			SetState(&ePrevManagerState, &eManagerState, STATE_INIT);
		}

	}	
	else
	{
		// Cmd Error
		ulCmdResult = CMD_EXEC_ERROR;
	}
	return ulCmdResult;
}

uint32_t DebugFLTRCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulValue;

	if(pToken->wOp == (NA))
	{
		ulCtrlDiagSt = IALDIAG_ERR_NONE;
#if 0
		// Read Control Period via Ial
		HalTimerStop();
		IalDiagSet(IALDIAG_ERR_NONE);
		SetSignal(&ulSignalQofControl, SIGNAL_CORE_RESET);
		SetSignal(&ulSignalQofManager, SIGNAL_CORE_RESET);
#endif
	}
	else if(pToken->wOp == (NA|EQ|AR))
	{		
		// Read Control Period via Ial
		ulValue = atol((char *)pToken->pArg[0]);
		
		if(ulValue == 1)
		{
			SetSignal(&ulSignalQofControl, SIGNAL_CORE_RESET);
		}
		else
		{
			SetSignal(&ulSignalQofManager, SIGNAL_CORE_RESET);
		}

	}
	else
	{
		// Cmd Error
		ulCmdResult = CMD_EXEC_ERROR;
	}
	return ulCmdResult;
}


extern uint32_t	ulFCommTest;


uint32_t DebugFCOMMTESTmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulValue;

	if(pToken->wOp == (NA|QU))
	{
		
		ulCmdResultLen = sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "%d\n\r", ulFCommTest);
 		ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "\n");

	}	
	else if(pToken->wOp == (NA|EQ|AR))
	{
		ulValue = atol((char *)pToken->pArg[0]);

		if(ulValue == 1) 	// CRC Test
		{
			ulFCommTest = 1;
		}
		else if(ulValue == 2) 	// CRC Test
		{
			ulFCommTest = 2;
		}
		else				// No Rsp Test
		{
			ulFCommTest = 0;
		}

	}	
	else
	{
		// Cmd Error
		ulCmdResult = CMD_EXEC_ERROR;
	}

	return ulCmdResult;

}


extern uint32_t ulFcommOkCnt[];
extern uint32_t ulFcommCrcErrCnt[];
extern uint32_t ulFcommNoPktErrCnt[];

uint32_t DebugFCOMMCNTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulValue;

	static uint32_t		ulDiagOkCnt[HALFCOMM_CH_NUM];
	static uint32_t		ulDiagErrCnt[HALFCOMM_CH_NUM];
	static uint32_t		ulDiagNoPktErrCnt[HALFCOMM_CH_NUM];

	uint32_t ulTemp;

	if(pToken->wOp == (NA))
	{
		uint32_t uli;
		
		// Read Control Period via Ial
		for(uli=HALFCOMM_CH_1; uli<HALFCOMM_CH_NUM; uli++)
		{
			ulFcommOkCnt[uli] = 0;
			ulFcommCrcErrCnt[uli] = 0;
			ulFcommNoPktErrCnt[uli] = 0;

			ulDiagOkCnt[uli] = 0;
			ulDiagErrCnt[uli] = 0;
			ulDiagNoPktErrCnt[uli] = 0;			
		}

	}
	else if(pToken->wOp == (NA|QU))
	{
		uint32_t uli;
		
		// Read Control Period via Ial
		ulCmdResultLen = sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "       OK          CrcErr      NoPkt\n\r");
		for(uli=HALFCOMM_CH_1; uli<HALFCOMM_CH_NUM; uli++)
		{
			ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "CH%d: 0x%08x, 0x%08x, 0x%08x\n\r", uli+1, ulDiagOkCnt[uli], ulDiagErrCnt[uli], ulDiagNoPktErrCnt[uli]);

			ulTemp = ulFcommOkCnt[uli];
			if(ulTemp != 0) ulDiagOkCnt[uli] = ulTemp;
			ulTemp = ulFcommCrcErrCnt[uli];
			if(ulTemp != 0) ulDiagErrCnt[uli] = ulTemp;
			ulTemp = ulFcommNoPktErrCnt[uli];
			if(ulTemp != 0) ulDiagNoPktErrCnt[uli] = ulTemp;
			
		}
 		ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "\n");

	}
	else if(pToken->wOp == (NA|EQ|AR))
	{
		if(pToken->wArgsFound == 1)
		{
			ulValue = Str2ULong((char *)pToken->pArg[0]);
			if(ulValue <= HALFCOMM_CH_NUM)
			{
				ulCmdResultLen = sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "       OK          CrcErr      NoPkt\n\r");
				ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "CH%d: 0x%08x, 0x%08x, 0x%08x\n\r", ulValue, ulDiagOkCnt[ulValue], ulDiagErrCnt[ulValue], ulDiagNoPktErrCnt[ulValue]);
			}
			else
			{
				// Cmd Error
				ulCmdResult = CMD_EXEC_ERROR;
			}

		}
		else
		{
			// Cmd Error
			ulCmdResult = CMD_EXEC_ERROR;
		}
	}
	else
	{
		// Cmd Error
		ulCmdResult = CMD_EXEC_ERROR;
	}
	return ulCmdResult;

//	DebugCmdExeResult(pToken->abWorkingCmdLine, ulCmdResultLen, ulCmdResult);
}



#ifdef FEATURE_PERIPHERAL_TEST

extern uint32_t	ulTestCh;


void DebugFCOMMTESTmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulValue;

	if(pToken->wOp == (NA|EQ|AR))
	{
		ulValue = atol((char *)pToken->pArg[0]);

		ulTestCh = ulValue;
		ulDebugMode = IALDEBUG_MODE_FCOMM_TEST;
	}	
	else
	{
		// Cmd Error
		ulCmdResult = CMD_EXEC_ERROR;
	}

	return ulCmdResult;

}


uint32_t DebugVMETESTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulValue;

	if(pToken->wOp == (NA))
	{
		ulDebugMode = IALDEBUG_MODE_VME_TEST;
	}	
	else
	{
		// Cmd Error
		ulCmdResult = CMD_EXEC_ERROR;
	}

	return ulCmdResult;
}


#endif


uint32_t DebugDIAGTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{

	uint32_t ulValue;
	uint32_t ulArg1;
	uint32_t ulLoop;
	float fValue;
	if(pToken->wOp == (NA|QU))
	{
		ulCmdResultLen = sprintf((char *)(pToken->abWorkingCmdLine), "OvrTimeCnt: %d(Max:%d)\n\r",lOvrTimeCnt, lMaxOvrTimeCnt);
		ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "ErrCause: 0x%08x\n\r",ulDiagSt);
		ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "\n");
	}
	else
	{
		// Cmd Error
		ulCmdResult = CMD_EXEC_ERROR;
	}
	return ulCmdResult;

}

