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

#define SIGNAL_DO_INIT			0x1			///< 초기화 실행 Signal 
#define SIGNAL_INIT_DONE		0x2			///< 초기화 완료 Signal



#define SIGNAL_RELEASE_BUS		0x10		///< 버스 릴리즈 Signal
#define SIGNAL_REQUEST_BUS		0x20		///< 버스 릴리즈 요청 Signal
#define SIGNAL_SYNC_DETECTED	0x40		///< Sync 신호 검출 Signal

#define SIGNAL_TIMER_EXPIRED	0x100		///< Timer 만료 Signal



#define SIGNAL_CORE_RESET		0x10000000			///< 시스템 리셋 Signal
#define SIGNAL_CORE_RESET_DONE	0x20000000			///< 시스템 리셋 Signal


///@}

void ClearSignal(uint32_t *pulSignalQ, uint32_t ulSignalMask);

/**
 * @brief	Signal 설정 함수 
 * @details	수신된 Signal 을 설정 한다.
 * @param	pulSignalQ: Signal 큐 포인터
 *			ulSignalMask: 설정할 Signal Mask
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void SetSignal(uint32_t *pulSignalQ, uint32_t ulSignalMask);


/**
 * @brief	Signal Check 함수 
 * @details	수신된 Signal 을 확인한다.
 * @param	pulSignalQ: Signal 큐 포인터
 *			ulSignalMask: 확인할 Signal Mask
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
bool CheckSignal(uint32_t *pulSignalQ, uint32_t ulSignalMask);

/**
 * @brief	Signal 을 처리하는 함수 
 * @details	수신된 Signal 을 확인 하고 그에 맞는 처리를 진행한다.
 * @param	pulSignalQ: Signal 큐 포인터
 *			eState: 현재 모듈의 상태
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
CORE_STATE ProcessSignal(uint32_t *pulSignalQ, CORE_STATE eState);

#endif


