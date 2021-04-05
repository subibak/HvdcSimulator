/**
 * Signal.h
 * Signal Interface Module Definition
 */

#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#include "Core.h"


/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{
#define SIGNAL_NONE				0x0			///< No Signal

#define SIGNAL_DO_INIT			0x1			///< �ʱ�ȭ ���� Signal 
#define SIGNAL_INIT_DONE		0x2			///< �ʱ�ȭ �Ϸ� Signal



#define SIGNAL_RELEASE_BUS		0x10		///< ���� ������ Signal
#define SIGNAL_REQUEST_BUS		0x20		///< ���� ������ ��û Signal
#define SIGNAL_SYNC_DETECTED	0x40		///< Sync ��ȣ ���� Signal

#define SIGNAL_TIMER_EXPIRED	0x100		///< Timer ���� Signal



#define SIGNAL_CORE_RESET		0x10000000			///< �ý��� ���� Signal
#define SIGNAL_CORE_RESET_DONE	0x20000000			///< �ý��� ���� Signal


///@}

void ClearSignal(uint32_t *pulSignalQ, uint32_t ulSignalMask);

/**
 * @brief	Signal ���� �Լ� 
 * @details	���ŵ� Signal �� ���� �Ѵ�.
 * @param	pulSignalQ: Signal ť ������
 *			ulSignalMask: ������ Signal Mask
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void SetSignal(uint32_t *pulSignalQ, uint32_t ulSignalMask);


/**
 * @brief	Signal Check �Լ� 
 * @details	���ŵ� Signal �� Ȯ���Ѵ�.
 * @param	pulSignalQ: Signal ť ������
 *			ulSignalMask: Ȯ���� Signal Mask
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
bool CheckSignal(uint32_t *pulSignalQ, uint32_t ulSignalMask);

/**
 * @brief	Signal �� ó���ϴ� �Լ� 
 * @details	���ŵ� Signal �� Ȯ�� �ϰ� �׿� �´� ó���� �����Ѵ�.
 * @param	pulSignalQ: Signal ť ������
 *			eState: ���� ����� ����
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
CORE_STATE ProcessSignal(uint32_t *pulSignalQ, CORE_STATE eState);

#endif


