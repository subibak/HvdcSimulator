
#include "Signal.h"

/**
 * @brief	Signal Clear �Լ� 
 * @details	���ŵ� Signal �� Clear �Ѵ�.
 * @param	pulSignalQ: Signal ť ������
 *			ulSignalMask: ������ Signal Mask
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
 * @brief	Signal ���� �Լ� 
 * @details	���ŵ� Signal �� ���� �Ѵ�.
 * @param	pulSignalQ: Signal ť ������
 *			ulSignalMask: ������ Signal Mask
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
 * @brief	Signal Check �Լ� 
 * @details	���ŵ� Signal �� Ȯ���Ѵ�.
 * @param	pulSignalQ: Signal ť ������
 *			ulSignalMask: Ȯ���� Signal Mask
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
 * @brief	Signal �� ó���ϴ� �Լ� 
 * @details	���ŵ� Signal �� Ȯ�� �ϰ� �׿� �´� ó���� �����Ѵ�.
 * @param	pulSignalQ: Signal ť ������
 *			eState: ���� ����� ����
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

