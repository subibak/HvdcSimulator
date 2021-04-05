

/**
 * IalConfig.c
 * IalConfig Interface Module Implementation
 */


#include "IalControl/IalControl.h"
#include "IalConfig.h"



#include "Hal/HalNvRam.h"
#include "Hal/HalFComm.h"
#include "Hal/HalTimer.h"


// Define Config Variables
uint32_t	ulMasterSlave;
uint32_t	ulIalFactoryInit;
uint32_t	ulFltOption = IALCONFIG_FAULT_RECOVERY_AUTO;


// HalTimer Module 로 옮길 것인가?
uint32_t	ulTimerPeriod;

// HalFComm Module 로 옮길 것인가?
extern uint32_t	ulFCommEnable[HALFCOMM_CH_NUM];
extern uint32_t	ulFCommRxLen[HALFCOMM_CH_NUM];
extern uint32_t	ulFCommTxLen[HALFCOMM_CH_NUM];

uint32_t	ulCfgFCommEnable[HALFCOMM_CH_NUM];											
uint32_t	ulCfgFCommRxLen[HALFCOMM_CH_NUM];
uint32_t	ulCfgFCommTxLen[HALFCOMM_CH_NUM];

extern uint32_t	ulSystemCfg;
extern uint32_t		t_ulControlMst;

uint32_t	ulCfgSystemCfg;

uint32_t	ulSyncDelay = TIMER_SYNC_DELAY;


extern uint32_t	ulMesMode;
extern uint32_t	ulMesId;

uint32_t	ulCfgMesMode = IALMES_HIGH_SPEED;
uint32_t	ulCfgMesId 	 = IALMES_ID_START;


//	uint32_t	ulCfgDefault[] = {
//	/* 0x00 ~ 0x03 */		SYNC_MASTER,			TIMEOUT_PERIOD,			HALFCOMM_ENABLE,				HALFCOMM_ENABLE,
//	/* 0x04 ~ 0x07 */		HALFCOMM_ENABLE,		HALFCOMM_ENABLE,		HALFCOMM_BUFFER_SIZE,			HALFCOMM_BUFFER_SIZE,
//	/* 0x08 ~ 0x0B */		HALFCOMM_BUFFER_SIZE,	HALFCOMM_BUFFER_SIZE,	HALFCOMM_BUFFER_SIZE,			HALFCOMM_BUFFER_SIZE,
//	/* 0x0C ~ 0x0F */		HALFCOMM_BUFFER_SIZE,	HALFCOMM_BUFFER_SIZE,	IALCONFIG_FACTORY_INIT_DONE,	SYSTEM_MES,
//	/* 0x10 ~ 0x13 */		TIMER_SYNC_DELAY
//	};

void IalConfigInit(void (*func)(int))
{

	uint32_t ulLoop;

	/* Step 0: Init Ial Variables */
	ulMasterSlave = SYNC_MASTER;
	ulTimerPeriod = DEFAULT_TICK;

	/* Step 1: Init Hal Layer as Default */
	HalNvRamInit();
	HalTimerInit(func);

	IalConfigLoad(&ulIalFactoryInit,							IALCONFIG_FACTORYINIT_DONE_IDX,	IAL_DEFAULT_SIZE);

	/* Step 2: Check and Load Ial Config Value */
	if(ulIalFactoryInit == IALCONFIG_FACTORY_INIT_DONE)
	{
		/* Step 2.1: Load Ial Config Value */
		IalConfigLoad(&ulMasterSlave,					IALCONFIG_MASTERSLAVE_IDX,		IAL_DEFAULT_SIZE);
		t_ulControlMst = ulMasterSlave;

		IalConfigLoad(&ulTimerPeriod,					IALCONFIG_TIMEPERIOD_IDX,		IAL_DEFAULT_SIZE);

//			IalConfigLoad(&ulCfgFCommEnable[HALFCOMM_CH_1],	IALCONFIG_FCOMM_CH1_ENABLE_IDX, IAL_DEFAULT_SIZE);
//			IalConfigLoad(&ulCfgFCommEnable[HALFCOMM_CH_2],	IALCONFIG_FCOMM_CH2_ENABLE_IDX, IAL_DEFAULT_SIZE);
//			IalConfigLoad(&ulCfgFCommEnable[HALFCOMM_CH_3],	IALCONFIG_FCOMM_CH3_ENABLE_IDX, IAL_DEFAULT_SIZE);
//			IalConfigLoad(&ulCfgFCommEnable[HALFCOMM_CH_4],	IALCONFIG_FCOMM_CH4_ENABLE_IDX, IAL_DEFAULT_SIZE);
		IalConfigLoad(&ulCfgFCommTxLen[HALFCOMM_CH_1],	IALCONFIG_FCOMM_CH1_TXLEN_IDX, IAL_DEFAULT_SIZE);
		IalConfigLoad(&ulCfgFCommTxLen[HALFCOMM_CH_2],	IALCONFIG_FCOMM_CH2_TXLEN_IDX, IAL_DEFAULT_SIZE);
		IalConfigLoad(&ulCfgFCommTxLen[HALFCOMM_CH_3],	IALCONFIG_FCOMM_CH3_TXLEN_IDX, IAL_DEFAULT_SIZE);
		IalConfigLoad(&ulCfgFCommTxLen[HALFCOMM_CH_4],	IALCONFIG_FCOMM_CH4_TXLEN_IDX, IAL_DEFAULT_SIZE);
		IalConfigLoad(&ulCfgFCommRxLen[HALFCOMM_CH_1],	IALCONFIG_FCOMM_CH1_RXLEN_IDX, IAL_DEFAULT_SIZE);
		IalConfigLoad(&ulCfgFCommRxLen[HALFCOMM_CH_2],	IALCONFIG_FCOMM_CH2_RXLEN_IDX, IAL_DEFAULT_SIZE);
		IalConfigLoad(&ulCfgFCommRxLen[HALFCOMM_CH_3],	IALCONFIG_FCOMM_CH3_RXLEN_IDX, IAL_DEFAULT_SIZE);
		IalConfigLoad(&ulCfgFCommRxLen[HALFCOMM_CH_4],	IALCONFIG_FCOMM_CH4_RXLEN_IDX, IAL_DEFAULT_SIZE);

//			memcpy(ulFCommEnable, ulCfgFCommEnable, sizeof(ulFCommEnable));
		memcpy(ulFCommTxLen, ulCfgFCommTxLen, sizeof(ulFCommTxLen));
		memcpy(ulFCommRxLen, ulCfgFCommRxLen, sizeof(ulCfgFCommRxLen));

		IalConfigLoad(&ulCfgSystemCfg,					IALCONFIG_SYSTEMCFG_IDX, 		IAL_DEFAULT_SIZE);
		ulSystemCfg = ulCfgSystemCfg;

#ifdef FEATURE_SDLY_SAVE
		IalConfigLoad(&ulSyncDelay,						IALCONFIG_SDLY_IDX, 			IAL_DEFAULT_SIZE);
#endif
		IalConfigLoad(&ulCfgMesMode,						IALCONFIG_MESMODE_IDX, 			IAL_DEFAULT_SIZE);
		ulMesMode = ulCfgMesMode;

	}
	else
	{
		IalConfigLoad(&ulCfgSystemCfg,					IALCONFIG_SYSTEMCFG_IDX, 		IAL_DEFAULT_SIZE);

		/* Step 2.2: Set/Save Ial Config Value as default */
		switch(ulCfgSystemCfg)
		{
			case SYSTEM_CCS:
				IalConfigSet(IALCONFIG_SYSTEMCFG_IDX,			SYSTEM_CCS,			IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_MASTERSLAVE_IDX, 		SYNC_SLAVE, 			IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH1_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH2_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH3_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH4_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH1_TXLEN_IDX, 	0,						IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH2_TXLEN_IDX, 	IALCCS_TX_DEFAULT_SIZE,	IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH3_TXLEN_IDX, 	IALCCS_TX_DEFAULT_SIZE,	IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH4_TXLEN_IDX, 	IALCCS_TX_DEFAULT_SIZE, IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH1_RXLEN_IDX, 	IALMES_FCOMM_SIZE, 		IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH2_RXLEN_IDX, 	IALCCS_RX_DEFAULT_SIZE, 	IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH3_RXLEN_IDX, 	IALCCS_RX_DEFAULT_SIZE, 	IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH4_RXLEN_IDX, 	IALCCS_RX_DEFAULT_SIZE, 	IALCONFIG_SAVE);
				break;
			case SYSTEM_VBC:
				IalConfigSet(IALCONFIG_SYSTEMCFG_IDX,			SYSTEM_VBC,			IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_MASTERSLAVE_IDX, 		SYNC_SLAVE, 			IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH1_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH2_ENABLE_IDX, 	HALFCOMM_DISABLE,		IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH3_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH4_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH1_TXLEN_IDX, 	IALVBC_TX_DEFAULT_SIZE,	IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH2_TXLEN_IDX, 	IALVBC_TX_DEFAULT_SIZE,	IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH3_TXLEN_IDX, 	IALVBC_RTDS_TX_SIZE,	IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH4_TXLEN_IDX, 	IALVBC_RTDS_TX_SIZE, IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH1_RXLEN_IDX, 	IALVBC_RX_DEFAULT_SIZE, 	IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH2_RXLEN_IDX, 	IALVBC_RX_DEFAULT_SIZE, 	IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH3_RXLEN_IDX, 	IALVBC_RTDS_RX_SIZE, 	IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH4_RXLEN_IDX, 	IALVBC_RTDS_RX_SIZE, 	IALCONFIG_SAVE);
				break;
			case SYSTEM_MES:
			default:
				IalConfigSet(IALCONFIG_SYSTEMCFG_IDX,			SYSTEM_MES,					IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_MASTERSLAVE_IDX, 		SYNC_MASTER, 			IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH1_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH2_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH3_ENABLE_IDX, 	HALFCOMM_DISABLE,		IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH4_ENABLE_IDX, 	HALFCOMM_DISABLE,		IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH1_TXLEN_IDX, 	0,	IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH2_TXLEN_IDX, 	HALFCOMM_BUFFER_SIZE,	IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH3_TXLEN_IDX, 	0,	IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH4_TXLEN_IDX, 	0, 	IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH1_RXLEN_IDX, 	HALFCOMM_BUFFER_SIZE, 	IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH2_RXLEN_IDX, 	0, 	IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH3_RXLEN_IDX, 	0, 	IALCONFIG_SAVE);
				IalConfigSet(IALCONFIG_FCOMM_CH4_RXLEN_IDX, 	0, 	IALCONFIG_SAVE);
				break;
		}

#ifdef FEATURE_SDLY_SAVE
		IalConfigSet(IALCONFIG_SDLY_IDX,				TIMER_SYNC_DELAY, 				IALCONFIG_SAVE);
#endif
		IalConfigSet(IALCONFIG_TIMEPERIOD_IDX,			TIMEOUT_PERIOD, 			IALCONFIG_SAVE);
		IalConfigSet(IALCONFIG_FACTORYINIT_DONE_IDX,	IALCONFIG_FACTORY_INIT_DONE,	IALCONFIG_SAVE);
		IalConfigSet(IALCONFIG_MESMODE_IDX,				IALMES_HIGH_SPEED, 				IALCONFIG_SAVE);
	//	IalConfigSet(IALCONFIG_MESID_IDX,				SYSTEM_MES, 				IALCONFIG_SAVE);
	}
	
	/* Step 3: Init Hal Layer according to Ial variables */
	HalTimerConfigure(ulTimerPeriod);
//		for(ulLoop = HALFCOMM_CH_1; ulLoop < HALFCOMM_CH_NUM;ulLoop++)
//		{
//			;//HalFCommSet(ulLoop, ulFCommEnable[ulLoop], ulFCommLength[ulLoop]);
//		}

	/* Step 4:  */


}

void IalConfigSet(uint32_t ulIalIdx, uint32_t ulValue, uint32_t	ulSave)
{

	switch(ulIalIdx)
	{
		case IALCONFIG_FACTORYINIT_DONE_IDX:
			ulIalFactoryInit = ulValue;
			break;
		case IALCONFIG_MASTERSLAVE_IDX:
			ulMasterSlave	= ulValue;
			t_ulControlMst = ulMasterSlave;
			break;
		case IALCONFIG_TIMEPERIOD_IDX:
			// Error
			ulTimerPeriod	= ulValue;
			break;
		case IALCONFIG_FCOMM_CH1_ENABLE_IDX:
			ulCfgFCommEnable[HALFCOMM_CH_1]	= ulValue;
			ulFCommEnable[HALFCOMM_CH_1]	= ulValue;
			break;
		case IALCONFIG_FCOMM_CH2_ENABLE_IDX:
			ulCfgFCommEnable[HALFCOMM_CH_2]	= ulValue;
			ulFCommEnable[HALFCOMM_CH_2]	= ulValue;
			break;
		case IALCONFIG_FCOMM_CH3_ENABLE_IDX:
			ulCfgFCommEnable[HALFCOMM_CH_3]	= ulValue;
			ulFCommEnable[HALFCOMM_CH_3]	= ulValue;
			break;
		case IALCONFIG_FCOMM_CH4_ENABLE_IDX:
			ulCfgFCommEnable[HALFCOMM_CH_4]	= ulValue;
			ulFCommEnable[HALFCOMM_CH_4]	= ulValue;
			break;
		case IALCONFIG_FCOMM_CH1_TXLEN_IDX:
			ulCfgFCommTxLen[HALFCOMM_CH_1]	= ulValue;
			ulFCommTxLen[HALFCOMM_CH_1]	= ulValue;
			break;
		case IALCONFIG_FCOMM_CH2_TXLEN_IDX:
			ulCfgFCommTxLen[HALFCOMM_CH_2]	= ulValue;
			ulFCommTxLen[HALFCOMM_CH_2]	= ulValue;
			break;
		case IALCONFIG_FCOMM_CH3_TXLEN_IDX:
			ulCfgFCommTxLen[HALFCOMM_CH_3]	= ulValue;
			ulFCommTxLen[HALFCOMM_CH_3]	= ulValue;
			break;
		case IALCONFIG_FCOMM_CH4_TXLEN_IDX:
			ulCfgFCommTxLen[HALFCOMM_CH_4]	= ulValue;
			ulFCommTxLen[HALFCOMM_CH_4]	= ulValue;
			break;			
		case IALCONFIG_FCOMM_CH1_RXLEN_IDX:
			ulCfgFCommRxLen[HALFCOMM_CH_1]	= ulValue;
			ulFCommRxLen[HALFCOMM_CH_1]	= ulValue;
			break;
		case IALCONFIG_FCOMM_CH2_RXLEN_IDX:
			ulCfgFCommRxLen[HALFCOMM_CH_2]	= ulValue;
			ulFCommRxLen[HALFCOMM_CH_2]	= ulValue;
			break;
		case IALCONFIG_FCOMM_CH3_RXLEN_IDX:
			ulCfgFCommRxLen[HALFCOMM_CH_3]	= ulValue;
			ulFCommRxLen[HALFCOMM_CH_3]	= ulValue;
			break;
		case IALCONFIG_FCOMM_CH4_RXLEN_IDX:
			ulCfgFCommRxLen[HALFCOMM_CH_4]	= ulValue;
			ulFCommRxLen[HALFCOMM_CH_4]	= ulValue;
			break;			
 		case IALCONFIG_SYSTEMCFG_IDX:
			ulCfgSystemCfg = ulValue;
			ulSystemCfg = ulValue;
			break;			
#ifdef FEATURE_SDLY_SAVE
 		case IALCONFIG_SDLY_IDX:
			ulSyncDelay = ulValue;
			break;
#endif
 		case IALCONFIG_MESID_IDX:
			ulCfgMesId = ulValue;
			ulMesId = ulValue;
			break;			
 		case IALCONFIG_MESMODE_IDX:
			ulCfgMesMode = ulValue;
			ulMesMode = ulValue;
			break;			
		default:
			// Error
			return;
	}

	if(ulSave == IALCONFIG_SAVE)
	{
		HalNvRamWrite(&ulValue, ulIalIdx, IAL_DEFAULT_SIZE);
	}

}

void IalConfigGet(uint32_t *ulValue, uint32_t ulIalIdx, uint32_t ulLen)
{

	switch(ulIalIdx)
	{
		case IALCONFIG_MASTERSLAVE_IDX:
			*ulValue = ulMasterSlave;
			break;
		case IALCONFIG_TIMEPERIOD_IDX:
			// Error
			*ulValue = ulTimerPeriod;
			break;
		case IALCONFIG_FCOMM_CH1_ENABLE_IDX:
			*ulValue = ulCfgFCommEnable[HALFCOMM_CH_1];
			break;
		case IALCONFIG_FCOMM_CH2_ENABLE_IDX:
			*ulValue = ulCfgFCommEnable[HALFCOMM_CH_2];
			break;
		case IALCONFIG_FCOMM_CH3_ENABLE_IDX:
			*ulValue = ulCfgFCommEnable[HALFCOMM_CH_3];
			break;
		case IALCONFIG_FCOMM_CH4_ENABLE_IDX:
			*ulValue = ulCfgFCommEnable[HALFCOMM_CH_4];
			break;
		case IALCONFIG_FCOMM_CH1_TXLEN_IDX:
			*ulValue = ulCfgFCommTxLen[HALFCOMM_CH_1];
			break;
		case IALCONFIG_FCOMM_CH2_TXLEN_IDX:
			*ulValue = ulCfgFCommTxLen[HALFCOMM_CH_2];
			break;
		case IALCONFIG_FCOMM_CH3_TXLEN_IDX:
			*ulValue = ulCfgFCommTxLen[HALFCOMM_CH_3];
			break;
		case IALCONFIG_FCOMM_CH4_TXLEN_IDX:
			*ulValue = ulCfgFCommTxLen[HALFCOMM_CH_4];
			break;			
		case IALCONFIG_FCOMM_CH1_RXLEN_IDX:
			*ulValue = ulCfgFCommRxLen[HALFCOMM_CH_1];
			break;
		case IALCONFIG_FCOMM_CH2_RXLEN_IDX:
			*ulValue = ulCfgFCommRxLen[HALFCOMM_CH_2];
			break;
		case IALCONFIG_FCOMM_CH3_RXLEN_IDX:
			*ulValue = ulCfgFCommRxLen[HALFCOMM_CH_3];
			break;
		case IALCONFIG_FCOMM_CH4_RXLEN_IDX:
			*ulValue = ulCfgFCommRxLen[HALFCOMM_CH_4];
			break;			
		case IALCONFIG_SYSTEMCFG_IDX:
			*ulValue = ulCfgSystemCfg;
			break;			
 		case IALCONFIG_SDLY_IDX:
			*ulValue = ulSyncDelay;
			break;
		case IALCONFIG_MESID_IDX:
			*ulValue = ulCfgMesId;
			break;			
		case IALCONFIG_MESMODE_IDX:
			*ulValue = ulCfgMesMode;
			break;			
		default:
			// Error
			break;
	}

}

void IalConfigLoad(uint32_t *ulValue, uint32_t ulIalIdx, uint32_t ulLen)
{
	HalNvRamRead(ulValue, ulIalIdx, IAL_DEFAULT_SIZE);
}

