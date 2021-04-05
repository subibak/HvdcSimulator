

/**
 * IalConfig.h
 * IalConfig Interface Module Definition
 */

#ifndef _IALCONFIG_H_
#define _IALCONFIG_H_

#include "IalIfDef.h"

#include "Hal/HalTimer.h"
#include "Hal/HalFComm.h"


/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{
#define IALCONFIG_FACTORY_INIT_DONE		0x1

#define IALCONFIG_FAULT_RECOVERY_MANUAL	0x0
#define IALCONFIG_FAULT_RECOVERY_AUTO	0x1



//#define IAL_DEFAULT_SIZE			0x1			///< Sync Master


#define IALCONFIG_FACTORYINIT_DONE_IDX	0x0
#define IALCONFIG_MASTERSLAVE_IDX		0x1
#define IALCONFIG_TIMEPERIOD_IDX		0x2
#define IALCONFIG_FCOMM_CH1_ENABLE_IDX	0x3
#define IALCONFIG_FCOMM_CH2_ENABLE_IDX	0x4
#define IALCONFIG_FCOMM_CH3_ENABLE_IDX	0x5
#define IALCONFIG_FCOMM_CH4_ENABLE_IDX	0x6
#define IALCONFIG_FCOMM_CH1_TXLEN_IDX	0x7
#define IALCONFIG_FCOMM_CH2_TXLEN_IDX	0x8
#define IALCONFIG_FCOMM_CH3_TXLEN_IDX	0x9
#define IALCONFIG_FCOMM_CH4_TXLEN_IDX	0xA
#define IALCONFIG_FCOMM_CH1_RXLEN_IDX	0xB
#define IALCONFIG_FCOMM_CH2_RXLEN_IDX	0xC
#define IALCONFIG_FCOMM_CH3_RXLEN_IDX	0xD
#define IALCONFIG_FCOMM_CH4_RXLEN_IDX	0xE
#define IALCONFIG_FAULTRECOVERY_IDX		0xF

#define IALCONFIG_SYSTEMCFG_IDX			0x10
#define IALCONFIG_SDLY_IDX				0x11
#define IALCONFIG_MESID_IDX				0x12
#define IALCONFIG_MESMODE_IDX				0x13



#define IALCONFIG_SAVE					0x1
#define IALCONFIG_NOSAVE				0x0

///@}


//#define SIGNAL_REQUEST_BUS	0x8


/**
 * @brief	Ial Moduel 을 초기화하는 함수 
 * @details	Ial 변수 및 관련 Hal의 초기화 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void IalConfigInit(void (*func)(int));

void IalConfigGet(uint32_t *ulValue, uint32_t ulIalIdx, uint32_t ulLen);

void IalConfigSet(uint32_t ulIalIdx, uint32_t ulValue, uint32_t	ulSave);

void IalConfigLoad(uint32_t *ulValue, uint32_t ulIalIdx, uint32_t ulLen);


extern uint32_t	ulMasterSlave;
extern uint32_t	ulTimerPeriod;
extern uint32_t	ulCfgSystemCfg;
extern uint32_t	ulSyncDelay;
extern uint32_t	ulCfgMesId;


#endif

