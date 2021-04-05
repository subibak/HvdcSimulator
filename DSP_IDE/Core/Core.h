

/**
 * Core.h
 * Core Module Definition
 */


#ifndef _CORE_H_
#define _CORE_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <sysreg.h>
#include <builtins.h>
#include <defts201.h>
#include <Signal.h>

#define TS_CHAR_BIT_8	8
#define TS_CHAR_BIT_32	32



#if _BITS_BYTE == TS_CHAR_BIT_32

typedef uint32_t	uint8_t;
typedef uint32_t	uint16_t;

typedef int32_t		int8_t;
typedef int32_t		int16_t;

#endif


#include "Utility.h"

#define HW_VER 3

#define FEATURE_SYNCRONIZATION
//#define FEATURE_PERI_TEST
//#define FEATURE_DELAYUS

//#define TARGET_EVKIT
#define	FEATURE_TIMING_TEST
//#define FEATURE_PERIPHERAL_TEST
//#define FEATURE_TEMP_TEST

#define FEATURE_DI_SEPERATE_ACCESS
#define FEATURE_AI_SEPERATE_ACCESS

#define FEATURE_DEBUG_VARIABLE

#define FEATURE_IAL_BUFFERINTEGRATION



#define FEATURE_SYSTEM_CONFIGURABLE


#define FEATURE_AO_SEPERATE_ACCESS

#define FEATURE_SDLY_SAVE





/** @name Constant
 * Constant
 */
///@{
#define CORE_BOOTUP_READYCODE		0xB1B2B3B4	


/** @name Core Configuration
 * Core Configuration
 */
///@{
#define CORE_MASTER							0			///< Master 시스템 정의
#define CORE_SLAVE							1			///< Slave 시스템 정의
///@}

/** @name Core Error Cause
 * Core Error Cause
 */
///@{
#define CORE_COUNT_MAX	0xFFFF0000




#define SYSTEM_MES		0x0	// Low Speed
#define SYSTEM_CCS		0x1
#define SYSTEM_VBC		0x2
#define SYSTEM_MES_HS	0x3 // SYSINIT 명령 파라미터로만 사용되며 실제 저장은 SYSTEM_MES 로 한다.
#define SYSTEM_UNKNOWN	0x4

#define SYNC_MASTER				0x0			///< Sync Master
#define SYNC_SLAVE				0x1			///< Sync Master


///@}

/** @name Type Definition
 * Type Definition
 */
///@{
typedef enum {					///< State Type 정의
	STATE_BOOT = 0
	,STATE_INIT
	,STATE_WAIT_INIT	
	,STATE_INIT_DONE	
	,STATE_MASTER_NORMAL
	,STATE_SLAVE_NORMAL
	,STATE_DOWNLOAD
	,STATE_FAULT			

} CORE_STATE;

typedef void (*FuncType)(void);
///@}


void SetState(CORE_STATE *ePrevState, CORE_STATE *eState, CORE_STATE eNextState);


//--------------------------------------------------------------------------//
// Function:	Delay
//
// Parameters:	ulMS - number of miliseconds to sleep
//
// Return:		None
//
// Description:	Delay for a fixed number of Ms, blocks
//--------------------------------------------------------------------------//
volatile void DelayUs(const uint32_t ulUs);


#endif /* _CORE_H_ */
