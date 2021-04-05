
#include "IalDiag.h"
#include "IalConfig.h"

#include "Hal/HalFComm.h"
#include "Hal/HalVme.h"
#include "Hal/HalLed.h"
//#include "Hal/HalDio.h"


volatile uint32_t	ulDiagSt;
//uint32_t	ulFcommSeq[HALFCOMM_CH_NUM];
uint32_t	ulBPLiveCnt;
uint32_t	ulFaultState;

IALDIAG_HEADER	*pstDiagHeader;
IALDIAG_INFO	*pstDiagInfo;
uint32_t	ulCnt1Sec = TICK1MS*500;


#define DSP_BOARD_TYPE	3
#define DSP_BOARD_INIT_FLAG	0xA1A2A3A4



void IalDiagInit(void)
{

	/* Step 0: Init Ial Layer */
	ulDiagSt = IALDIAG_ERR_NONE;
	ulBPLiveCnt = 0;
	ulFaultState = IALDIAG_FAULT_NONE;

	pstDiagHeader	= (IALDIAG_HEADER *) HWREG_VMES_BASE;
	pstDiagInfo		= (IALDIAG_INFO *) HWREG_VMES_DIAG_BASE;

	memset((char *)pstDiagHeader, 0x0, sizeof(IALDIAG_HEADER));
	memset(pstDiagInfo, 0x0, sizeof(IALDIAG_INFO));

	ulCnt1Sec = TICK1MS*500/ulTimerPeriod;


	/* Step 1: Init Hal Layer */
//	HalWdtDisable();




	/* Step 2:  */


	/* Step 3:  */


	/* Step 4:  */
	pstDiagHeader->ulBoardId		= EndianSwap32(DSP_BOARD_TYPE);
	pstDiagHeader->ulSlaveReadyFlag	= EndianSwap32(DSP_BOARD_INIT_FLAG);

}


void IalDiagRun(void)
{

}

extern uint32_t	ulCfgFCommEnable[HALFCOMM_CH_NUM];		
extern uint32_t	ulFCommEnable[HALFCOMM_CH_NUM];


uint32_t IalDiagFCommCheck(void)
{
	uint32_t	ulState = 0;
	uint32_t	uli;
	HAL_ERROR	eHalError;

	for(uli=HALFCOMM_CH_1; uli<HALFCOMM_CH_NUM;uli++)
	{
		if(ulCfgFCommEnable[uli] == HALFCOMM_ENABLE)
		{
			eHalError = HalFCommStsCheck(uli);
			if(eHalError == HAL_NOERR)
			{
				// CRC, NoRsp Error 를 부가적으로 확인하여 진단 데이터 생성.
			}
#if 1
			else if(eHalError == HALFCOMM_DISABLED_ERROR)
			{
				// Disable 이므로 진단 결과는 No err 로 통일.
				
			}
#endif
			else
			{
				// 채널 Lost 로 진단 정보 설정.
				ulState = ulState | IALDIAG_ERR(uli, IALDIAG_ERR_YES);
			}
		}
		else
		{
			ulCfgFCommEnable[uli] = ulFCommEnable[uli];
		}

	}

	return ulState;



}

void IalDiagBPCheck(void)
{

}

void IalDiagBrdCheck(void)
{

}


extern CORE_STATE eManagerState;
extern CORE_STATE eControlState;
extern CORE_STATE ePrevManagerState;
extern CORE_STATE ePrevControlState;


extern uint32_t	ulTimerPeriod;

#if 1
void IalDiagSet(uint32_t ulSts)
{
	static uint32_t	ulCurrCnt = 0;
	static uint32_t	ulLedColor;
	static uint32_t	ulLedOnOff;

	ulDiagSt = ulDiagSt & ulSts;
	BitSet(ulDiagSt, ulSts);
	pstDiagInfo->ulDiag = EndianSwap32(ulDiagSt);

	if(ulDiagSt == IALDIAG_ERR_NONE)
	{
//			HalLedCtrlSet(HALLED_STA1LED, HALLED_ON, HALLED_LED_BLUE);
		ulLedColor = HALLED_LED_GREEN;
		
	}
	else
	{
//	Fault Latch 제거. 결정 필요함.
//	
//			if(ulMasterSlave == SYNC_SLAVE)
//			{
//				SetState(&ePrevManagerState, &eManagerState, STATE_FAULT);	
//				SetState(&ePrevControlState, &eControlState, STATE_FAULT);
//			}
//			HalLedCtrlSet(HALLED_STA1LED, HALLED_ON, HALLED_LED_RED);
		ulLedColor = HALLED_LED_RED;
	}

	ulCurrCnt++;
	if(ulCurrCnt == ulCnt1Sec)
	{
		ulCurrCnt = 0;
		ulLedOnOff = (ulLedOnOff ^ HALLED_ON);
		// LED Set
		HalLedCtrlSet(HALLED_STA1LED, ulLedOnOff, ulLedColor);
	}

}

uint32_t IalDiagGet(void)
{
	return EndianSwap32(pstDiagInfo->ulDiag);
}

#else

void IalDiagSet(uint32_t ulIalIdx, uint32_t ulValue)
{

	switch(ulIalIdx)
	{
		case IALDIAG_DIAG_STATE_IDX:
			ulDiagSt		|= ulValue;
			break;
		case IALDIAG_BACKPLANE_LIVECNT_IDX:
			ulBPLiveCnt		= ulValue;
			break;
		case IALDIAG_FAULT_STATE_IDX:
			// Error
			ulFaultState	= ulValue;
			break;
		default:
			// Error
			break;
	}
}
#endif

#if 1
void IalDiagUpdateLiveCnt(void)
{
	ulBPLiveCnt++;
	pstDiagHeader->ulSlaveLiveCnt = EndianSwap32(ulBPLiveCnt++);
	if(ulBPLiveCnt > IALDIAG_LIVECNT_MAX)
	{
		ulBPLiveCnt = 0;
	}

}
#else
void IalDiagGet(uint32_t *ulValue, uint32_t ulIalIdx, uint32_t ulLen)
{

	switch(ulIalIdx)
	{
		case IALDIAG_DIAG_STATE_IDX:
			*ulValue = ulDiagSt;
			break;
		case IALDIAG_BACKPLANE_LIVECNT_IDX:
			*ulValue = ulBPLiveCnt;
			break;
		case IALDIAG_FAULT_STATE_IDX:
			// Error
			*ulValue = ulFaultState;
			break;		
		default:
			// Error
			break;
	}

}
#endif

