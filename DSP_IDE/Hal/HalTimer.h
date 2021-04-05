

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
 * @brief	Timer Module 초기화 함수 
 * @details	Timer 동작을 초기화 한다. 호출 뒤, Timer 를 다시 시작해야 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalTimerInit(void (*func)(int));

/**
 * @brief	Timer 설정값을 입력하는 함수
 * @details	Timer 동작 주기(uS) 를 설정한다. 
 * @param	ulPeriod: 동작 주기 를 uS 단위로 입력한다.
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalTimerConfigure(uint32_t ulPeriod);

/**
 * @brief	Timer 모둘의 인터럽트 설정 함수
 * @details	인터럽트 동작 여부와 핸들러를 설정한다.
 * @param	func: 핸들러 함수
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalTimerIntInit(void (*func)(int));

/**
 * @brief	Timer 를 새 주기로 재시작하는 함수
 * @details	입력된 주기를 로딩하여 Timer 모듈을 재시작한다
 * @param	ulPeriod: 동작 주기를 uS 단위로 입력한다.
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalTimerReStart(uint32_t ulPeriod);

/**
 * @brief	Timer 를 새 주기로 시작하는 함수
 * @details	입력된 주기를 로딩하여 Timer 모듈을 시작한다
 * @param	ulPeriod: 동작 주기를 uS 단위로 입력한다.
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalTimerStart(uint32_t ulPeriod);


void HalTimerStop(void);

#endif
