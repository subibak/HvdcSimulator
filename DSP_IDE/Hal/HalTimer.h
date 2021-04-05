

/**
 * HalTimer.h
 * HalTimer Device Driver Module Definition
 */

#ifndef _HALTIMER_H_
#define _HALTIMER_H_

#include "HalIfDef.h"

/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{
#ifdef TARGET_EVKIT // EVkit
#define 	CCLK			500000000							///< Core Clock
#define 	SCLK			250000000							///< System Clock
#define		TCLK			SCLK								///< Timer Clock
#else
#define 	CCLK			600000000
#define 	SCLK			300000000
#define		TCLK			SCLK
#endif

#define		TICK1US			(TCLK/1000000)						///< 1uS, 1000000Hz
#define		TICK5US			(5 * TICK1US)						///< 1uS, 1000000Hz
#define		TICK100US		(100 * TICK1US)						///< 1uS, 1000000Hz
#define		TICK1MS			(TICK1US*1000)						///< 1mS, 1000Hz


#define		DEFAULT_TICK	TICK100US							///< 1uS, 1000000Hz


//#define 	TIMER_SYNC_DELAY	(TICK1US*5)
#define 	TIMER_SYNC_DELAY	(63 * TICK1US)		// microsec unit



#define		TICK1S			(1000*TICK1MS)						///< 1S, 1000000Hz

#define		TIMEOUT_PERIOD	TICK100US								///< Timer Period

#define		MSEC_COUNT		(TCLK / (1000 * TIMEOUT_PERIOD))	///< 1mS Tick Count
#define		T1SEC_COUNT		(TCLK / TIMEOUT_PERIOD)

#define		MAX_TICK_CNT	86400000							///< 1day = 86400000msec

///@}


/**
 * @brief	Timer Module �ʱ�ȭ �Լ� 
 * @details	Timer ������ �ʱ�ȭ �Ѵ�. ȣ�� ��, Timer �� �ٽ� �����ؾ� �Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalTimerInit(void (*func)(int));

/**
 * @brief	Timer �������� �Է��ϴ� �Լ�
 * @details	Timer ���� �ֱ�(uS) �� �����Ѵ�. 
 * @param	ulPeriod: ���� �ֱ� �� uS ������ �Է��Ѵ�.
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalTimerConfigure(uint32_t ulPeriod);

/**
 * @brief	Timer ����� ���ͷ�Ʈ ���� �Լ�
 * @details	���ͷ�Ʈ ���� ���ο� �ڵ鷯�� �����Ѵ�.
 * @param	func: �ڵ鷯 �Լ�
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalTimerIntInit(void (*func)(int));

/**
 * @brief	Timer �� �� �ֱ�� ������ϴ� �Լ�
 * @details	�Էµ� �ֱ⸦ �ε��Ͽ� Timer ����� ������Ѵ�
 * @param	ulPeriod: ���� �ֱ⸦ uS ������ �Է��Ѵ�.
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalTimerReStart(uint32_t ulPeriod);

/**
 * @brief	Timer �� �� �ֱ�� �����ϴ� �Լ�
 * @details	�Էµ� �ֱ⸦ �ε��Ͽ� Timer ����� �����Ѵ�
 * @param	ulPeriod: ���� �ֱ⸦ uS ������ �Է��Ѵ�.
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalTimerStart(uint32_t ulPeriod);


void HalTimerStop(void);

#endif
