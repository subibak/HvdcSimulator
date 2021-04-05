
#include "Signal.h"

/**
 * @brief	Signal Clear 함수 
 * @details	수신된 Signal 을 Clear 한다.
 * @param	pulSignalQ: Signal 큐 포인터
 *			ulSignalMask: 해제할 Signal Mask
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void ClearSignal(uint32_t *pulSignalQ, uint32_t ulSignalMask)
{
	*pulSignalQ = *pulSignalQ & (~ulSignalMask);

	return;
}

/**
 * @brief	Signal 설정 함수 
 * @details	수신된 Signal 을 설정 한다.
 * @param	pulSignalQ: Signal 큐 포인터
 *			ulSignalMask: 설정할 Signal Mask
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void SetSignal(uint32_t *pulSignalQ, uint32_t ulSignalMask)
{
	*pulSignalQ = *pulSignalQ | ulSignalMask;
//	__builtin_sysreg_write(__BUSLK, 1);
//	__builtin_sysreg_write(__BUSLK, 0);

}


/**
 * @brief	Signal Check 함수 
 * @details	수신된 Signal 을 확인한다.
 * @param	pulSignalQ: Signal 큐 포인터
 *			ulSignalMask: 확인할 Signal Mask
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
bool CheckSignal(uint32_t *pulSignalQ, uint32_t ulSignalMask)
{
	bool blSignalRcv = true;
	
	if((*pulSignalQ & ulSignalMask) == SIGNAL_NONE)
	{
		blSignalRcv = false;
	}

	return blSignalRcv;

}

/**
 * @brief	Signal 을 처리하는 함수 
 * @details	수신된 Signal 을 확인 하고 그에 맞는 처리를 진행한다.
 * @param	pulSignalQ: Signal 큐 포인터
 *			eState: 현재 모듈의 상태
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
CORE_STATE ProcessSignal(uint32_t *pulSignalQ, CORE_STATE eState)
{

	CORE_STATE eNextState = eState;

	if((*pulSignalQ & SIGNAL_CORE_RESET) == SIGNAL_CORE_RESET){

		ClearSignal(pulSignalQ, SIGNAL_CORE_RESET);
					eNextState = STATE_BOOT;
	}
	else 
	{
		;
	}

	return eNextState;

}

