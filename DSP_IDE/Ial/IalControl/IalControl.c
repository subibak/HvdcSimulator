
#include "IalControl.h"

//	#include "IalMes.h"
//	#include "IalCcs.h"
//	#include "IalVbc.h"
//#include "IalVbcl.h"


//	#include "../IalHmi.h"

#include "Hal/HalAi.h"
//#include "Hal/HalAo.h"
//#include "Hal/HalDio.h"

#include "../IalDiag.h"


#if 1
// HalFComm Module 로 옮길 것인가? => 옮겨선 안된다.
//	uint32_t	ulFCommEnable[HALFCOMM_CH_NUM] = {
//													 HALFCOMM_ENABLE
//													,HALFCOMM_ENABLE
//													,HALFCOMM_ENABLE
//													,HALFCOMM_ENABLE
//												};
uint32_t	ulFCommEnable[HALFCOMM_CH_NUM] = {
												 HALFCOMM_DISABLE
												,HALFCOMM_DISABLE
												,HALFCOMM_DISABLE
												,HALFCOMM_DISABLE
											};
											
uint32_t	ulFCommRxLen[HALFCOMM_CH_NUM] = {
												 HALFCOMM_BUFFER_SIZE
												,HALFCOMM_BUFFER_SIZE
												,HALFCOMM_BUFFER_SIZE
												,HALFCOMM_BUFFER_SIZE
											};


uint32_t	ulFCommTxLen[HALFCOMM_CH_NUM] = {
												 HALFCOMM_BUFFER_SIZE
												,HALFCOMM_BUFFER_SIZE
												,HALFCOMM_BUFFER_SIZE
												,HALFCOMM_BUFFER_SIZE
											};


// Hw Frame length : 1이 16 word 0 => 16*1, 7=> 16*8
uint32_t	ulFCommFrameNum[HALFCOMM_CH_NUM] = {
												 HALFCOMM_LARGFRAME_NUM
												,HALFCOMM_SMALFRAME_NUM
												,HALFCOMM_SMALFRAME_NUM
												,HALFCOMM_SMALFRAME_NUM
											};

uint32_t		ulFcommOkCnt[HALFCOMM_CH_NUM];
uint32_t		ulFcommCrcErrCnt[HALFCOMM_CH_NUM];
uint32_t		ulFcommNoPktErrCnt[HALFCOMM_CH_NUM];

#endif

// IO 에 대한 시작 위치는 고정
// AI 위치, FCOMM 1TX, 1RX 2Tx, 2Rx 3Tx, 3Rx 4Tx, 4Rx, 기타 IO
// 시스템별로 각IO 의 길이 En.Dis 설정
// 각 IO의 논리적인 위치는 각 시스템 App 에서 정의 가능
// 공통에해당하는 부분만 IalControl.h 로 가져오도록 한다.

// 항상 Tx 시스템의 Enum 과 Rx 시스템의 Enum 이 채널별로 일치해야함.
// 혼동 가능성이 높다. 방법이 없는가?

#ifdef FEATURE_SYSTEM_CONFIGURABLE
uint32_t	ulSystemCfg = SYSTEM_MES;
uint32_t		t_ulControlMst = SYNC_MASTER;

#endif

/************************** Control Buffer *******************************/
U_IALBUFFER_TYPE	uInputBuffer;
U_IALBUFFER_TYPE	uOutputBuffer;

#ifdef FEATURE_IAL_BUFFERINTEGRATION
uint32_t	ulInput[IALCONTROL_INPUT_SIZE];
uint32_t	ulOutput[IALCONTROL_OUTPUT_SIZE];
#endif
/*****************************************************************************************/

void IalControlInit(void)
{
	uint32_t	uli;

	/* Step 0: Init Ial Layer */
	uInputBuffer.pulBuffer	= ulInput;
	uOutputBuffer.pulBuffer	= ulOutput;

	/* Step 1: Init Hal Layer */
	HalAiInit();
	HalFCommInit();
	HalVmeInit();

	HalFCommEna(HALFCOMM_CH_1, HALFCOMM_ENABLE);
	HalFCommEna(HALFCOMM_CH_2, HALFCOMM_ENABLE);
	HalFCommEna(HALFCOMM_CH_3, HALFCOMM_ENABLE);
	HalFCommEna(HALFCOMM_CH_4, HALFCOMM_ENABLE);

	HalFCommSetFrameSize(HALFCOMM_CH_1, ulFCommTxLen[HALFCOMM_CH_1], ulFCommRxLen[HALFCOMM_CH_1]);
	HalFCommSetFrameSize(HALFCOMM_CH_2, ulFCommTxLen[HALFCOMM_CH_2], ulFCommRxLen[HALFCOMM_CH_2]);
	HalFCommSetFrameSize(HALFCOMM_CH_3, ulFCommTxLen[HALFCOMM_CH_3], ulFCommRxLen[HALFCOMM_CH_3]);
	HalFCommSetFrameSize(HALFCOMM_CH_4, ulFCommTxLen[HALFCOMM_CH_4], ulFCommRxLen[HALFCOMM_CH_4]);

  	// Default 설정
	HalFCommSetBuffer(HALFCOMM_CH_1, &(ulOutput[IALCONTROL_FCOMM1_OFFSET]), &(ulInput[IALCONTROL_FCOMM1_OFFSET]));
	HalFCommSetBuffer(HALFCOMM_CH_2, &(ulOutput[IALCONTROL_FCOMM2_OFFSET]), &(ulInput[IALCONTROL_FCOMM2_OFFSET]));
	HalFCommSetBuffer(HALFCOMM_CH_3, &(ulOutput[IALCONTROL_FCOMM3_OFFSET]), &(ulInput[IALCONTROL_FCOMM3_OFFSET]));
	HalFCommSetBuffer(HALFCOMM_CH_4, &(ulOutput[IALCONTROL_FCOMM4_OFFSET]), &(ulInput[IALCONTROL_FCOMM4_OFFSET]));

#if 0
	/* Step 2: 시스템별로 설정 변경 가능. */
#ifdef FEATURE_SYSTEM_CONFIGURABLE
	switch(ulSystemCfg)
	{
		case SYSTEM_MES:
			// CH1 : MES 1,2간의 데이터공유, CH2,3,4 : 전체 데이터의 전달
				HalFCommSetBuffer(HALFCOMM_CH_1, &(ulOutput[IALMES_FCOMM_CH1_START]), &(ulInput[IALMES_FCOMM_CH1_START]));
				HalFCommSetBuffer(HALFCOMM_CH_2, &(ulOutput[IALMES_FCOMM_CH2_START]), &(ulInput[IALMES_FCOMM_CH2_START]));
				HalFCommSetBuffer(HALFCOMM_CH_3, &(ulOutput[IALMES_FCOMM_CH3_START]), &(ulInput[IALMES_FCOMM_CH3_START]));
				HalFCommSetBuffer(HALFCOMM_CH_4, &(ulOutput[IALMES_FCOMM_CH4_START]), &(ulInput[IALMES_FCOMM_CH4_START]));
			break;
		case SYSTEM_CCS:
			// CH1: Mes Data 수신 , 			CH2~4: VBCU를 통해 VBCU와 L 의 데이터 송수신.
			// CH1: Mes 데잍터전체크기(34) 		CH2~4: 기본 Ref 또는 Meas 값과 HDR 만포함.
//				HalFCommSetBuffer(HALFCOMM_CH_1, &(ulOutput[HALAI_CH_NUM]), &(ulInput[HALAI_CH_NUM]));
//				HalFCommSetBuffer(HALFCOMM_CH_2, &(ulOutput[HALAI_CH_NUM]), &(ulInput[HALAI_CH_NUM+HALFCOMM_CH_2*HALFCOMM_BUFFER_SIZE]));
//				HalFCommSetBuffer(HALFCOMM_CH_3, &(ulOutput[HALAI_CH_NUM]), &(ulInput[HALAI_CH_NUM+HALFCOMM_CH_3*HALFCOMM_BUFFER_SIZE]));
//				HalFCommSetBuffer(HALFCOMM_CH_4, &(ulOutput[HALAI_CH_NUM]), &(ulInput[HALAI_CH_NUM+HALFCOMM_CH_4*HALFCOMM_BUFFER_SIZE]));
 			break;
		case SYSTEM_VBC:
//				HalFCommSetBuffer(HALFCOMM_CH_1, &(ulOutput[HALAI_CH_NUM]), &(ulInput[HALAI_CH_NUM]));
//				HalFCommSetBuffer(HALFCOMM_CH_2, &(ulOutput[HALAI_CH_NUM]), &(ulInput[HALAI_CH_NUM+HALFCOMM_CH_2*HALFCOMM_BUFFER_SIZE]));
//				HalFCommSetBuffer(HALFCOMM_CH_3, &(ulOutput[HALAI_CH_NUM]), &(ulInput[HALAI_CH_NUM+HALFCOMM_CH_3*HALFCOMM_BUFFER_SIZE]));
//				HalFCommSetBuffer(HALFCOMM_CH_4, &(ulOutput[HALAI_CH_NUM]), &(ulInput[HALAI_CH_NUM+HALFCOMM_CH_4*HALFCOMM_BUFFER_SIZE]));

			break;
		default:

			break;
	}
	
#endif
#endif
	/* Step 3:  */

	// 절대 지우지 말것.
	// 출력 데이터가 모두 0인 경우에 CRC 생성 에러 발생함.
	for(uli=0;uli < IALCONTROL_OUTPUT_SIZE; uli++)
	{
		ulOutput[uli] = uli;	
	}
	/* Step 4:  */


}

extern uint32_t ulCtrlDiagSt;


// ulStartCh: 0~15, pulBuff: Read Buffer Pointer, ulLen 채널개수.
IAL_ERROR IalControlAiRead(uint32_t ulStartCh, float pfBuf[], uint32_t	ulLen)
{

	IAL_ERROR	eResult = IAL_NOERR;
	uint32_t	uli, ulLsatIdx;

	ulLsatIdx = ulStartCh + ulLen;

	if(ulLsatIdx < HALAI_CH_NUM)
	{
		// Size 1 이 32bits 기반으로 copy 됨.
		memcpy(pfBuf, uInputBuffer.pulBuffer, ulLen);

//			if(ulLen > ulFCommRxLen[ulStartCh])
//			{
//				eResult = IAL_ERR_PARAMETER_RANGE;
//			}
//			else
//			{
//				for(uli = ulStartCh ; uli < ulLsatIdx; uli++)
//				{
//					pfBuf[uli] = HalAiRead(uli);
//				}
//			}
	}
	else
	{
		eResult = IAL_ERR_PARAMETER_RANGE;
	}
	return eResult;
}


// ulStartCh: 0~3, pulBuff: Read Buffer Pointer, ulLen 수신 데이터 개수 (0~설정값), 설정 Max=124.
IAL_ERROR IalControlFiberRead(uint32_t ulCh, uint32_t pulBuf[], uint32_t	ulLen)
{

	IAL_ERROR	eResult = IAL_NOERR;
//	uint32_t	uli;

	if(ulCh < HALFCOMM_CH_NUM)
	{
		ulFCommEnable[ulCh] = HALFCOMM_ENABLE;
//			HalFCommEna(ulCh, HALFCOMM_ENABLE);
		switch(ulCh)
		{
			case HALFCOMM_CH_1:
				memcpy(pulBuf, &(ulInput[IALCONTROL_FCOMM1_OFFSET + PROTOCOL_BODY_IDX]), ulLen);
				break;
			case HALFCOMM_CH_2:
//					HalFCommEna(ulCh, HALFCOMM_ENABLE);
				memcpy(pulBuf, &(ulInput[IALCONTROL_FCOMM2_OFFSET + PROTOCOL_BODY_IDX]), ulLen);
				break;
			case HALFCOMM_CH_3:
//					HalFCommEna(ulCh, HALFCOMM_ENABLE);
				if(ulSystemCfg == SYSTEM_VBC)
				{
					// IalRtdsDecode();
					memcpy(pulBuf, &(ulInput[IALCONTROL_FCOMM3_OFFSET]), ulLen);
				}
				else
				{
					memcpy(pulBuf, &(ulInput[IALCONTROL_FCOMM3_OFFSET + PROTOCOL_BODY_IDX]), ulLen);
				}
				// VBC 인 경우에는 헤더에서 중요한 부분은 Sm number 와 lag no. 뿐이다.
				break;
			case HALFCOMM_CH_4:
//					HalFCommEna(ulCh, HALFCOMM_ENABLE);
				if(ulSystemCfg == SYSTEM_VBC)
				{
					// IalRtdsDecode();
					memcpy(pulBuf, &(ulInput[IALCONTROL_FCOMM4_OFFSET]), ulLen);
				}
				else
				{
					memcpy(pulBuf, &(ulInput[IALCONTROL_FCOMM4_OFFSET + PROTOCOL_BODY_IDX]), ulLen);
				}
				// VBC 인 경우에는 헤더에서 중요한 부분은 Sm number 와 lag no. 뿐이다.
				break;
			default:
				eResult = IAL_ERR_PARAMETER_RANGE;
				break;
		}
		
	}
	else
	{
		eResult = IAL_ERR_PARAMETER_RANGE;
	}
	return eResult;
}


// ulStartCh: 0~3, pulBuff: Read Buffer Pointer, ulLen 수신 데이터 개수 (0~설정값), 설정 Max=124.
IAL_ERROR IalControlFiberWrite(uint32_t ulCh, uint32_t pulBuf[], uint32_t	ulLen)
{

	IAL_ERROR	eResult = IAL_NOERR;

	if(ulCh < HALFCOMM_CH_NUM)
	{
		ulFCommEnable[ulCh] = HALFCOMM_ENABLE;
//			HalFCommEna(ulCh, HALFCOMM_ENABLE);
		switch(ulCh)
		{
			case HALFCOMM_CH_1:
				memcpy( &(ulOutput[IALCONTROL_FCOMM1_OFFSET+PROTOCOL_BODY_IDX]), pulBuf, ulLen);
				break;
			case HALFCOMM_CH_2:
				memcpy( &(ulOutput[IALCONTROL_FCOMM2_OFFSET+PROTOCOL_BODY_IDX]), pulBuf, ulLen);
				break;
			case HALFCOMM_CH_3:
				if(ulSystemCfg == SYSTEM_VBC)
				{
					// RTDS 규격에 맞도록 패키징 필요함. (Compact Mode)
					// IalRtdsEncode();
					memcpy( &(ulOutput[IALCONTROL_FCOMM3_OFFSET]), pulBuf, ulLen);
				}
				else
				{
					memcpy( &(ulOutput[IALCONTROL_FCOMM3_OFFSET+PROTOCOL_BODY_IDX]), pulBuf, ulLen);
				}
				break;
			case HALFCOMM_CH_4:
				if(ulSystemCfg == SYSTEM_VBC)
				{
					// RTDS 규격에 맞도록 패키징 필요함. (Compact Mode)
					// IalRtdsEncode();
					memcpy( &(ulOutput[IALCONTROL_FCOMM4_OFFSET]), pulBuf, ulLen);
				}
				else
				{
					memcpy( &(ulOutput[IALCONTROL_FCOMM4_OFFSET+PROTOCOL_BODY_IDX]), pulBuf, ulLen);
				}
				break;
			default:
				eResult = IAL_ERR_PARAMETER_RANGE;
				break;
		}
	}
	else
	{
		eResult = IAL_ERR_PARAMETER_RANGE;
	}
	return eResult;
}

// ulStartCh: 0~3, pulBuff: Read Buffer Pointer, ulLen 수신 데이터 개수 (0~설정값), 설정 Max=124.
IAL_ERROR IalControlVmeRead(uint32_t ulStartIdx, uint32_t pulBuf[], uint32_t	ulLen)
{

	IAL_ERROR	eResult = IAL_NOERR;
	uint32_t	ulLsatIdx;

	ulLsatIdx = ulStartIdx + ulLen;
	if(ulLsatIdx < IALCONTROL_VME_SIZE_MAX)
	{
		memcpy(pulBuf, &(ulOutput[IALCONTROL_VME_OFFSET + ulStartIdx]), ulLen);
	}
	else
	{
		eResult = IAL_ERR_PARAMETER_RANGE;
	}
	return eResult;
}


// ulStartCh: 0~3, pulBuff: Read Buffer Pointer, ulLen 수신 데이터 개수 (0~설정값), 설정 Max=124.
IAL_ERROR IalControlVmeWrite(uint32_t ulStartIdx, uint32_t pulBuf[], uint32_t	ulLen)
{

	IAL_ERROR	eResult = IAL_NOERR;
	uint32_t	ulLsatIdx;

	ulLsatIdx = ulStartIdx + ulLen;
	if(ulLsatIdx < IALCONTROL_VME_SIZE_MAX)
	{
		memcpy(&(ulOutput[IALCONTROL_VME_OFFSET + ulStartIdx]), pulBuf, ulLen);
	}
	else
	{
		eResult = IAL_ERR_PARAMETER_RANGE;
	}
	return eResult;
}


void IalControlSend(void)
{

	uint32_t ulLoop;
	/* Step 0: Ial Write Optic Data */
	switch(ulSystemCfg)
	{
		case SYSTEM_MES:
		case SYSTEM_CCS:
			// MES CH1 : MES 1,2간의 데이터공유, CH2,3,4 : 전체 데이터의 전달
			// CCS CH1 : MES, CH2,3,4 : VBC 데이터 
			if(ulFCommEnable[HALFCOMM_CH_1] == HALFCOMM_ENABLE)
			{
				if(ulFCommTxLen[HALFCOMM_CH_1] > 0)
				{
					HalFCommWrite(HALFCOMM_CH_1);
				}
			}

			if(ulFCommEnable[HALFCOMM_CH_2] == HALFCOMM_ENABLE)
			{
				if(ulFCommTxLen[HALFCOMM_CH_2] > 0)
				{
					HalFCommWrite(HALFCOMM_CH_2);
				}
			}
			if(ulFCommEnable[HALFCOMM_CH_3] == HALFCOMM_ENABLE)
			{
				if(ulFCommTxLen[HALFCOMM_CH_3] > 0)
				{
					HalFCommWrite(HALFCOMM_CH_3);
				}
			}

			if(ulFCommEnable[HALFCOMM_CH_4] == HALFCOMM_ENABLE)
			{
				if(ulFCommTxLen[HALFCOMM_CH_4] > 0)
				{
					HalFCommWrite(HALFCOMM_CH_4);
				}
			}
			break;

		case SYSTEM_VBC:
			// VBC CH1 : MES, CH2 : CCS, CH3,4 : RTDS 데이터
			if(ulFCommEnable[HALFCOMM_CH_1] == HALFCOMM_ENABLE)
			{
				if(ulFCommTxLen[HALFCOMM_CH_1] > 0)
				{
					HalFCommWrite(HALFCOMM_CH_1);
				}
			}

			if(ulFCommEnable[HALFCOMM_CH_2] == HALFCOMM_ENABLE)
			{
				if(ulFCommTxLen[HALFCOMM_CH_2] > 0)
				{
					HalFCommWrite(HALFCOMM_CH_2);
				}
			}
			if(ulFCommEnable[HALFCOMM_CH_3] == HALFCOMM_ENABLE)
			{
				if(ulFCommTxLen[HALFCOMM_CH_3] > 0)
				{
					HalFCommWriteNoEnc(HALFCOMM_CH_3);
				}
			}

			if(ulFCommEnable[HALFCOMM_CH_4] == HALFCOMM_ENABLE)
			{
				if(ulFCommTxLen[HALFCOMM_CH_4] > 0)
				{
					HalFCommWriteNoEnc(HALFCOMM_CH_4);
				}
			}
			break;

		default:
			if(ulFCommEnable[HALFCOMM_CH_1] == HALFCOMM_ENABLE)
			{
				HalFCommWrite(HALFCOMM_CH_1);
			}

			break;
	}


}



void IalControlReceive(void)
{

	uint32_t	ulErrCause = IALDIAG_ERR_NONE;
	uint32_t	ulTemp = IALDIAG_ERR_NONE;
	uint32_t	uli = 0;

	/* Step 0: Ial Write Optic Data */
	switch(ulSystemCfg)
	{
		case SYSTEM_MES:
			for(uli=0; uli<HALAI_CH_NUM; uli++)
			{
//				uInputBuffer.pfBuffer[uli] = HalAiRead(uli + 1);
				uInputBuffer.pfBuffer[uli] = HalAiRead(uli);
			}
//			HalAiHandle();
		case SYSTEM_CCS:
			// MES CH1 : MES 1,2간의 데이터공유, CH2,3,4 : 전체 데이터의 전달
			// CCS CH1 : MES, CH2,3,4 : VBC 데이터 
			if(ulFCommEnable[HALFCOMM_CH_1] == HALFCOMM_ENABLE)
			{
				if(ulFCommRxLen[HALFCOMM_CH_1] > 0)
				{
					HalFCommRead(HALFCOMM_CH_1, &ulTemp);
					BitSet(ulErrCause, IALDIAG_ERR(HALFCOMM_CH_1, ulTemp));
				}
			}
			if(ulFCommEnable[HALFCOMM_CH_2] == HALFCOMM_ENABLE)
			{
				if(ulFCommRxLen[HALFCOMM_CH_2] > 0)
				{
					HalFCommRead(HALFCOMM_CH_2, &ulTemp);
					BitSet(ulErrCause, IALDIAG_ERR(HALFCOMM_CH_2, ulTemp));
				}
			}
			if(ulFCommEnable[HALFCOMM_CH_3] == HALFCOMM_ENABLE)
			{
				if(ulFCommRxLen[HALFCOMM_CH_3] > 0)
				{
					HalFCommRead(HALFCOMM_CH_3, &ulTemp);
					BitSet(ulErrCause, IALDIAG_ERR(HALFCOMM_CH_3, ulTemp));
				}
			}
			if(ulFCommEnable[HALFCOMM_CH_4] == HALFCOMM_ENABLE)
			{
				if(ulFCommRxLen[HALFCOMM_CH_4] > 0)
				{
					HalFCommRead(HALFCOMM_CH_4, &ulTemp);
					BitSet(ulErrCause, IALDIAG_ERR(HALFCOMM_CH_4, ulTemp));
				}
			}

			break;
		case SYSTEM_VBC:
			// VBC CH1 : MES, CH2 : CCS, CH3,4 : RTDS 데이터
			if(ulFCommEnable[HALFCOMM_CH_1] == HALFCOMM_ENABLE)
			{
				if(ulFCommRxLen[HALFCOMM_CH_1] > 0)
				{
					HalFCommRead(HALFCOMM_CH_1, &ulTemp);
					BitSet(ulErrCause, IALDIAG_ERR(HALFCOMM_CH_1, ulTemp));
				}
			}
			if(ulFCommEnable[HALFCOMM_CH_2] == HALFCOMM_ENABLE)
			{
				if(ulFCommRxLen[HALFCOMM_CH_2] > 0)
				{
					HalFCommRead(HALFCOMM_CH_2, &ulTemp);
					BitSet(ulErrCause, IALDIAG_ERR(HALFCOMM_CH_2, ulTemp));
				}
			}
			if(ulFCommEnable[HALFCOMM_CH_3] == HALFCOMM_ENABLE)
			{
				if(ulFCommRxLen[HALFCOMM_CH_3] > 0)
				{
					HalFCommReadNoDec(HALFCOMM_CH_3, &ulTemp);
					BitSet(ulErrCause, IALDIAG_ERR(HALFCOMM_CH_3, ulTemp));
				}
			}
			if(ulFCommEnable[HALFCOMM_CH_4] == HALFCOMM_ENABLE)
			{
				if(ulFCommRxLen[HALFCOMM_CH_4] > 0)
				{
					HalFCommReadNoDec(HALFCOMM_CH_4, &ulTemp);
					BitSet(ulErrCause, IALDIAG_ERR(HALFCOMM_CH_4, ulTemp));
				}
			}

			break;
		default:
			if(ulFCommEnable[HALFCOMM_CH_2] == HALFCOMM_ENABLE)
			{
				HalFCommRead(HALFCOMM_CH_2, &ulTemp);
				BitSet(ulErrCause, IALDIAG_ERR(HALFCOMM_CH_2, ulTemp));
			}
			break;
	}

	BitClear(ulCtrlDiagSt, 0xFFF);
	BitSet(ulCtrlDiagSt, ulErrCause);
	
}
 


