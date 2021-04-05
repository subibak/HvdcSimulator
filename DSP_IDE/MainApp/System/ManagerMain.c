
#include "ManagerMain.h"
#include "Hal/HalTS201.h"
#include "Hal/HalFComm.h"
#include "Hal/HalLed.h"
#include "Hal/HalWdt.h"

static void StateBootFunc(void);
static void StateInitFunc(void);
static void StateWaitInitFunc(void);
static void StateInitDoneFunc(void);

static void StateNormalFunc(void);
static void StateDloadFunc(void);
static void StateFaultFunc(void);


/* Definition of External Variable */
extern uint32_t ulSignalQofControl;

/* Definition of System Variable */
//uint32_t ulMasterSlave = CORE_MASTER; // Move to IalConfig.c
uint32_t ulSignalQofManager = SIGNAL_NONE;


uint32_t *pulMySignalQ 		= &ulSignalQofManager;
uint32_t *pulOtherSignalQ	= &ulSignalQofControl;



uint32_t ulPeriod = TIMEOUT_PERIOD;


CORE_STATE eManagerState 		= STATE_BOOT;
CORE_STATE ePrevManagerState	= STATE_BOOT;
CORE_STATE *pePrevState = &ePrevManagerState;
CORE_STATE *peState		= &eManagerState;

uint32_t ulIntStart = 0;
uint32_t ulIntEnd = 0;

uint32_t ulSyncSnd = 0;
uint32_t ulSyncRcv = 0;


uint32_t ulTickCount = 0;
uint32_t ulSecCount = 0;

FuncType	cbFunc;
	
uint32_t ulTempValue;
uint32_t ulCtrlDiagSt;


// Error Counts
uint32_t	ulSyncLostCnt = 0;
uint32_t	ulTaskOverTimecnt = 0;

uint32_t	ulState2LedColor = HALLED_LED_BLUE;


void ManagerInit(void (*func)(int))
{


	/* Initialize System Variable */
	eManagerState		= STATE_INIT;
	ePrevManagerState	= STATE_INIT;

	ulIntStart = 0;
	ulIntEnd = 0;

	ulSyncSnd = 0;
	ulSyncRcv = 0;

	ulCtrlDiagSt = 0;

	/* Initialize System Variable */
	ConfigAppInit(func);
	HmiAppInit();
	DebugAppInit();
	DiagAppInit();
#if 1
	if(ulMasterSlave == SYNC_MASTER)
	{
		cbFunc = PeriodRoutine;
		ulState2LedColor = HALLED_LED_BLUE;		
	}
	else
	{
		cbFunc = SetSyncLost;
		ulState2LedColor = HALLED_LED_GREEN;		
	}
#endif
	return;
}

#define BOOT_MAX_CNT	0x1000

uint32_t	ulBootCnt = 0;
void ManagerMain(void)
{

	CORE_STATE eNextState;
	uint32_t	ulReg;
	static uint32_t 	ulSeqCnt;
	

	/* Initialize System Variable */
	//CORE_STATE eState = STATE_INIT;

	switch(eManagerState)
	{
		case STATE_BOOT:
			StateBootFunc();
			break;
		case STATE_INIT:///////////////////// 0
			StateInitFunc();
			/* System Initialize */
			break;
		case STATE_WAIT_INIT:///////////////////// 1
			/* Wait Control System Initialize */
			StateWaitInitFunc();
			break;
		case STATE_INIT_DONE:
			StateInitDoneFunc();
			break;
		case STATE_MASTER_NORMAL:
			// Do Nothing
			break;
		case STATE_SLAVE_NORMAL:
			// Slave Sync Tracking
			// And then Trigger Control
#ifdef FEATURE_SYNCRONIZATION
			// FComm 채널로 동기를 변경 필요함.
			if(HAL_NOERR == HalFCommReadSync(&ulSeqCnt))
			{
				HalTimerReStart(ulTimerPeriod + ulSyncDelay);

	 			if(ulSyncLostCnt != 0) {ulSyncLostCnt--;}
	 			else {BitClear(ulCtrlDiagSt, IALDIAG_ERR_LOST_SYNC);ulState2LedColor = HALLED_LED_GREEN;}

				PeriodRoutine();
			}
#endif					
			break;
		case STATE_FAULT:
		case STATE_DOWNLOAD:
//			HalTimerStop();
			DiagAppRun(ulCtrlDiagSt);
			// WORK : Error Cause 에 정의된 모든 Fault 를 확인하여 Error Cause 를 업데이트 한다.
			//      : No Error 시에는 Init 상태로 천이한다.
			break;
		default:
			break;
	}


	DebugAppRun();
	HmiAppRun();
	eNextState = ProcessSignal(pulMySignalQ, *peState);
	if(eNextState != *peState)
	{
		SetState(pePrevState, peState, eNextState);
	}


}

void SetSyncLost(void)
{
	static uint32_t ulCapture = 0;

	ulSyncLostCnt = ulSyncLostCnt + 1;
	if(ulSyncLostCnt % 10000 == 1 && ((ulIntStart - ulCapture) > 10000))
	{
		ulCapture = ulIntStart;
//			ulSyncLostCnt = 10000;
//			ulCtrlDiagSt |= IALDIAG_ERR_LOST_SYNC;
		IalDebugWrite((uint8_t *)"\r\nSync Lost!\r\n", 14);
		if(ulSyncLostCnt > 20000)
			ulSyncLostCnt = 1;
	}
	BitSet(ulCtrlDiagSt, IALDIAG_ERR_LOST_SYNC);
	PeriodRoutine();
	ulState2LedColor = HALLED_LED_RED;

	
}

void PeriodRoutine(void)
{
	uint32_t ulCnt = 0;

#if 1//def FEATURE_PERIPHERAL_TEST
#if 0//def FEATURE_PERIPHERAL_TEST			
	static	uint32_t	ulOkCnt		= 0;
	static	uint32_t	ulErrCnt	= 0;
	static	uint32_t	ulTestCnt	= 0;
	static	uint8_t		abResult[100];
	uint32_t	ulLen;
	uint32_t	ulRetOkCnt =0;
	uint32_t	ulRetErrCnt=0;
#endif
//		HalLedCtrlSet(HALLED_STA2LED, HALLED_ON, ulState2LedColor);

	DiagAppRun(ulCtrlDiagSt);

	ulIntStart++;
	switch(ulDebugMode)
	{
		case IALDEBUG_MODE_NORMAL:
//			ulSyncSnd++;
			SetSignal(pulOtherSignalQ, SIGNAL_SYNC_DETECTED);
			while(CheckSignal(pulMySignalQ, SIGNAL_RELEASE_BUS) == false)
			{
				// Control DSP Fault 발생으로 뭉
				DelayUs(1);
				ulCnt++;
				if(ulCnt > 90)
				{
					IalDiagSet(IALDIAG_ERR_TOOMUCH_IODLY);
//					SetState(pePrevState, peState, STATE_FAULT);
//					SetErrorCause(&ulErrorCause, CORE_ERROR_CONTROLDSP_NORSP);
					break;
				}
			}
			ClearSignal(pulMySignalQ, SIGNAL_RELEASE_BUS);
			if(ulIoDelay < ulCnt)
			{
				ulIoDelay = ulCnt;
			}
			
			break;
#ifdef FEATURE_PERIPHERAL_TEST			
		case IALDEBUG_MODE_FCOMM_TEST:
			HalFCommLoopBackTest(&ulRetOkCnt, &ulRetErrCnt, ulTestCnt);
			if(ulTestCnt == 0)
			{
				// No Count
			}
			else
			{
				ulOkCnt = ulOkCnt + ulRetOkCnt;
				ulErrCnt = ulErrCnt + ulRetErrCnt;
				if(ulTestCnt % 30000 == 0)
				{
					ulLen = sprintf((char *)abResult, "\n\rFCOMM :%d\tOk:%08d\tErr:%08d", HalFCommGetTestCh(), ulOkCnt, ulErrCnt);
					IalDebugWrite(abResult, ulLen);
				}
				
			}

			if(ulTestCnt >=300000)
			{
				ulLen = sprintf((char *)abResult, "\n\rTest Done!\n\r\n\r");
				IalDebugWrite(abResult, ulLen);
				ulOkCnt		= 0;
				ulErrCnt	= 0;
				ulTestCnt	= 0;
				
				ulDebugMode = IALDEBUG_MODE_NORMAL;
			}
			else
			{
				ulTestCnt++;
			}

			
			break;
		case IALDEBUG_MODE_VME_TEST:
			HalVmeLoopBackTest(&ulRetOkCnt, &ulRetErrCnt, ulTestCnt);
			if(ulTestCnt == 0)
			{
				// No Count

			}
			else
			{
				ulOkCnt = ulOkCnt + ulRetOkCnt;
				ulErrCnt = ulErrCnt + ulRetErrCnt;
				if(ulTestCnt % 30000 == 0)
				{
					ulLen = sprintf((char *)abResult, "\n\rVME Test Size: 0x%08x\tOk:0x%08x\tErr:0x%08x",ulOkCnt+ulErrCnt, ulOkCnt, ulErrCnt);
					IalDebugWrite(abResult, ulLen);
				}
				
			}

			if(ulTestCnt >= 300000)
			{
				ulLen = sprintf((char *)abResult, "\n\rTest Done!\n\r\n\r");
				IalDebugWrite(abResult, ulLen);

				ulOkCnt		= 0;
				ulErrCnt	= 0;
				ulTestCnt	= 0;
				
				ulDebugMode = IALDEBUG_MODE_NORMAL;
			}
			else
			{
				ulTestCnt++;
			}
			break;
#endif			
		case IALDEBUG_MODE_DLOAD:
		case IALDEBUG_MODE_XMODEM:
		default:
			break;
			

	}

#else
	ulIntStart++;
	if(ulDebugMode == IALDEBUG_MODE_NORMAL)
	{
		ulSyncSnd++;
		SetSignal(pulOtherSignalQ, SIGNAL_SYNC_DETECTED);
		while(CheckSignal(pulMySignalQ, SIGNAL_RELEASE_BUS) == false)
		{
			;
		}
		ClearSignal(pulMySignalQ, SIGNAL_RELEASE_BUS);
		ulSyncRcv++;
	}
#endif

	ulIntEnd++;
//		HalLedCtrlSet(HALLED_STA2LED, HALLED_OFF, ulState2LedColor);

}

void IsrManagerTimer0(int lArg)
{
	CLI();
	HalTimerReStart(ulTimerPeriod);

//		HalLedCtrlSet(HALLED_STA2LED, HALLED_ON, ulState2LedColor);
	cbFunc();
//		HalLedCtrlSet(HALLED_STA2LED, HALLED_ON, ulState2LedColor);
	
#if 0
	if(ulMasterSlave == SYNC_SLAVE)
	{
		DiagAppRun(ulSyncLost);
	}

	ulTickCount++;

	if(ulTickCount >= T1SEC_COUNT)
	{
		ulSecCount++;
		ulTickCount = 0;	
	}
#endif

	STI();

}

void main(void)
{
	volatile bool	blLoop;

	blLoop = true;

//	strcpy((char *)abPrompt, PROMPT_STR);
//	ulPromptLen = strlen((char *)abPrompt);

	while(blLoop)
	{
		ManagerMain();
	}
	
}


static void StateBootFunc(void)
{
	uint32_t	ulReg;
	uint32_t	uli;
	
	InitTS201();
	ClearSignal(&ulSignalQofManager,		0xFFFFFFFF);
		
	SystemHealth();
	ulBootCnt++;
	CLI();
	ReadReg(XCA_FAIL_REG_ADDR, &ulReg);
	BitClear(ulReg, 0x1);
	WriteReg(XCA_FAIL_REG_ADDR, ulReg);

	ReadReg(0x30000001, &ulReg);
	HalUartInit();
	STI();

	if(ulReg == CORE_BOOTUP_READYCODE)
	{
//		ReadReg(XCA_WDT_STAT_REG_ADDR, &ulReg);
//		BitClear(ulReg, WDT_ENABLE);
//		WriteReg(XCA_WDT_STAT_REG_ADDR, ulReg);
		WriteReg(0x30000002, CORE_BOOTUP_READYCODE);
		WriteReg(0x30000001, 0x0);
		SetState(pePrevState, peState, STATE_INIT);
		HalUartSyncWrite((uint8_t *)"Dsp Boot complete!\r\n", strlen("Dsp Boot complete!\r\n"));

	}
#if 0	
	else
	{
		if(ulBootCnt > BOOT_MAX_CNT)
		{
			// Reset
			ulBootCnt = 0;
			ReadReg(XCA_WDT_STAT_REG_ADDR, &ulReg);
			BitSet(ulReg, WDT_ENABLE);
			WriteReg(XCA_WDT_STAT_REG_ADDR, ulReg);
		}
	}
#endif
}

static void StateInitFunc(void)
{
	ManagerInit(IsrManagerTimer0);	

	SetSignal(pulOtherSignalQ, SIGNAL_DO_INIT);
	SetState(pePrevState, peState, STATE_WAIT_INIT);
}

static void StateWaitInitFunc(void)
{
	if(CheckSignal(pulMySignalQ, SIGNAL_INIT_DONE) == true)
	{
		ClearSignal(pulMySignalQ, SIGNAL_INIT_DONE);
		SetState(pePrevState, peState, STATE_INIT_DONE);
		HalUartSyncWrite((uint8_t *)"Initialize complete!\r\n", strlen("Initialize complete!\r\n"));
	}
}

static void StateInitDoneFunc(void)
{
	IalConfigGet(&ulTempValue, IALCONFIG_MASTERSLAVE_IDX, IAL_DEFAULT_SIZE);
	if(ulTempValue == CORE_MASTER)
	{
		SetState(pePrevState, peState, STATE_MASTER_NORMAL);
	}
	else
	{
		SetState(pePrevState, peState, STATE_SLAVE_NORMAL);
	}
				
	HalTimerStart(ulTimerPeriod);
}



static void StateDloadFunc()
{

}
static void StateFaultFunc()
{

}


uint32_t DebugRESETCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	
	uint32_t ulValue;

	if(pToken->wOp == (NA))
	{
//		SetState(&ePrevControlState, &eControlState, STATE_BOOT);
		SetSignal(pulOtherSignalQ, SIGNAL_CORE_RESET);
		SetSignal(pulMySignalQ, SIGNAL_CORE_RESET);
//		SetState(&ePrevManagerState, &eManagerState, STATE_BOOT);

//		HalUartWrite((uint8_t *)"\r\nOK\r\n", 6);

	}	
	else
	{
		// Cmd Error
//		DebugCmdExeResult(pToken->abWorkingCmdLine, CMD_RESULT_LENGTH_NOMSG, CMD_EXEC_ERROR);
		ulCmdResult = CMD_EXEC_ERROR;
	}

	return ulCmdResult;
	
}


uint32_t DebugREBOOTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	
	uint32_t ulValue;

	if(pToken->wOp == (NA))
	{
		HalWdtReboot();
	}	
	else
	{
		// Cmd Error
		ulCmdResult = CMD_EXEC_ERROR;
	}

	return ulCmdResult;
	
}



uint32_t DebugSLVSTSCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	
	uint32_t ulArg1;

	if(pToken->wOp == (NA | QU))
	{
		ulCmdResultLen = sprintf((char *)(pToken->abWorkingCmdLine), "%d\r\n", ulSyncLostCnt);
	}	
	else if(pToken->wOp == (NA|EQ|AR))
	{

		if(pToken->wArgsFound == 1)
		{
			ulArg1 = Str2ULong((char *) pToken->pArg[0]);
			ulSyncLostCnt = ulArg1;
			BitClear(ulCtrlDiagSt, IALDIAG_ERR_LOST_SYNC);
		}
		else 
		{
			ulCmdResult = CMD_EXEC_ERROR;
		}
	}	
	else
	{
		// Cmd Error
//		DebugCmdExeResult(pToken->abWorkingCmdLine, CMD_RESULT_LENGTH_NOMSG, CMD_EXEC_ERROR);
		ulCmdResult = CMD_EXEC_ERROR;
	}

	return ulCmdResult;
	
}


uint32_t DebugSDLYCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	
	uint32_t ulArg1;

	if(pToken->wOp == (NA|QU))
	{
		ulCmdResultLen = sprintf((char *)(pToken->abWorkingCmdLine), "%d\r\n", ulSyncDelay/TICK1US);
	}	
	else if(pToken->wOp == (NA|EQ|AR))
	{
		if(pToken->wArgsFound == 1)
		{
			ulArg1 = Str2ULong((char *) pToken->pArg[0]);
			ulSyncDelay = ulArg1 * TICK1US;
			IalConfigSet(IALCONFIG_SDLY_IDX, ulSyncDelay, IALCONFIG_SAVE);

		}
		else 
		{
			ulCmdResult = CMD_EXEC_ERROR;
		}
	}	
	else
	{
		// Cmd Error
//		DebugCmdExeResult(pToken->abWorkingCmdLine, CMD_RESULT_LENGTH_NOMSG, CMD_EXEC_ERROR);
		ulCmdResult = CMD_EXEC_ERROR;
	}

	return ulCmdResult;
	
}


