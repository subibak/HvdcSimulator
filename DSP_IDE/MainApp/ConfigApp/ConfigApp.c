

#include <stdio.h>


#include "ConfigApp.h"
#include "Ial/IalConfig.h"

#include "MainApp/DebugApp/DebugApp.h"

void ConfigAppInit(void (*func)(int))
{

	/* Step 1: Init App Variables */


	/* Step 2: Init Ial Layer Accronging to App variables */
	/* Set Default Variables */
	IalConfigInit(func);
#if 0
	/* Step 3: Change Ial Variables as User Setting */
	IalConfigSet(IALCONFIG_MASTERSLAVE_IDX,	SYNC_MASTER);
	IalConfigSet(IALCONFIG_TIMEPERIOD_IDX,	DEFAULT_TICK);
#endif



	/* Step 4:  */


	

}


void ConfigAppRun(void)
{

	/* Step 1: Process IO Read */

	/* Step 2: Parse Cmd according to Result of Step 1 */

	/* Step 3: Handle Command */

	/* Step 4: Flush Read Buffer */




}


uint32_t DebugCFGINITCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	
	uint32_t ulValue;
	uint32_t uli;

	if(pToken->wOp == (NA))
	{
		IalConfigSet(IALCONFIG_FACTORYINIT_DONE_IDX, 0x0, IALCONFIG_SAVE);
		ulCmdResultLen = sprintf((char *)pToken->abWorkingCmdLine, "Reset Required\n\r", ulDebugMode);		
		ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "\n");
	}	
	else
	{
		// Cmd Error
//		DebugCmdExeResult(pToken->abWorkingCmdLine, CMD_RESULT_LENGTH_NOMSG, CMD_EXEC_ERROR);
		ulCmdResult = CMD_EXEC_ERROR;
	}

	return ulCmdResult;
}



uint32_t DebugPRDCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{

	uint32_t ulValue;

	if(pToken->wOp == (NA|QU))
	{
		
		// Read Control Period via Ial
		IalConfigGet(&ulValue, IALCONFIG_TIMEPERIOD_IDX, IAL_DEFAULT_SIZE);
		ulCmdResultLen = sprintf((char *)pToken->abWorkingCmdLine, "%d\n\n\r", ulValue/TICK1US);

		//ulCmdResultLen = strlen((char *)pToken->abWorkingCmdLine);		

	}
	else if(pToken->wOp == (NA|EQ|AR))
	{
		// Change Control Period via Ial
		ulValue = atol((char *)pToken->pArg[0]);

		ulValue = ulValue * TICK1US;

		IalConfigSet(IALCONFIG_TIMEPERIOD_IDX, ulValue, IALCONFIG_SAVE);
//		DebugCmdExeResult(pToken->abWorkingCmdLine, CMD_RESULT_LENGTH_NOMSG, CMD_EXEC_OK);

	}
	else
	{
		// Cmd Error
//		DebugCmdExeResult(pToken->abWorkingCmdLine, CMD_RESULT_LENGTH_NOMSG, CMD_EXEC_ERROR);
		ulCmdResult = CMD_EXEC_ERROR;
	}
	return ulCmdResult;

}




uint32_t DebugFACTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulValue;

	if(pToken->wOp == (NA|QU))
	{
		
		// Read Control Period via Ial
		IalConfigGet(&ulValue, IALCONFIG_FACTORYINIT_DONE_IDX, IAL_DEFAULT_SIZE);
		ulCmdResultLen = sprintf((char *)pToken->abWorkingCmdLine, "%d\n\n\r", ulValue);
		
//		DebugCmdExeResult(pToken->abWorkingCmdLine, strlen((char *)pToken->abWorkingCmdLine), CMD_EXEC_OK);
		

	}
	else if(pToken->wOp == (NA|EQ|AR))
	{
		// Change Control Period via Ial
		ulValue = atol((char *)pToken->pArg[0]);

		IalConfigSet(IALCONFIG_FACTORYINIT_DONE_IDX, ulValue, IALCONFIG_SAVE);
//		DebugCmdExeResult(pToken->abWorkingCmdLine, CMD_RESULT_LENGTH_NOMSG, CMD_EXEC_OK);
	}
	else
	{
		// Cmd Error
//		DebugCmdExeResult(pToken->abWorkingCmdLine, CMD_RESULT_LENGTH_NOMSG, CMD_EXEC_ERROR);
		ulCmdResult = CMD_EXEC_ERROR;
	}
	return ulCmdResult;
}

uint32_t DebugMSTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulValue;

	if(pToken->wOp == (NA|QU))
	{
		
		// Read Control Period via Ial
		IalConfigGet(&ulValue, IALCONFIG_MASTERSLAVE_IDX, IAL_DEFAULT_SIZE);
		ulCmdResultLen = sprintf((char *)pToken->abWorkingCmdLine, "%d\n\n\r", ulValue);
		
//		DebugCmdExeResult(pToken->abWorkingCmdLine, strlen((char *)pToken->abWorkingCmdLine), CMD_EXEC_OK);
		

	}
	else if(pToken->wOp == (NA|EQ|AR))
	{
		// Change Control Period via Ial
		ulValue = atol((char *)pToken->pArg[0]);

		IalConfigSet(IALCONFIG_MASTERSLAVE_IDX, ulValue, IALCONFIG_SAVE);
//		DebugCmdExeResult(pToken->abWorkingCmdLine, CMD_RESULT_LENGTH_NOMSG, CMD_EXEC_OK);
	}
	else
	{
		// Cmd Error
//		DebugCmdExeResult(pToken->abWorkingCmdLine, CMD_RESULT_LENGTH_NOMSG, CMD_EXEC_ERROR);
		ulCmdResult = CMD_EXEC_ERROR;
	}
	return ulCmdResult;
}

uint32_t DebugFCOMMCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulValue1, ulValue2, ulValue3, ulValue4;

	if(pToken->wOp == (NA|QU))
	{
		uint32_t ulLoop;
		
		// Read Control Period via Ial
		pToken->abWorkingCmdLine[0] = 0x0;

//		IalConfigGet(&ulValue2, IALCONFIG_FCOMM_CH1_ENABLE_IDX, IAL_DEFAULT_SIZE);
		IalConfigGet(&ulValue3, IALCONFIG_FCOMM_CH1_TXLEN_IDX, IAL_DEFAULT_SIZE);
		IalConfigGet(&ulValue4, IALCONFIG_FCOMM_CH1_RXLEN_IDX, IAL_DEFAULT_SIZE);
		ulCmdResultLen = sprintf((char *)pToken->abWorkingCmdLine + strlen((char*)(pToken->abWorkingCmdLine)), "CH1: %d, %d\n\r", ulValue3, ulValue4);
//		ulCmdResultLen = sprintf((char *)pToken->abWorkingCmdLine + strlen((char*)(pToken->abWorkingCmdLine)), "CH1: %d, %d, %d\n\r", ulValue2, ulValue3, ulValue4);

//		IalConfigGet(&ulValue2, IALCONFIG_FCOMM_CH2_ENABLE_IDX, IAL_DEFAULT_SIZE);
		IalConfigGet(&ulValue3, IALCONFIG_FCOMM_CH2_TXLEN_IDX, IAL_DEFAULT_SIZE);
		IalConfigGet(&ulValue4, IALCONFIG_FCOMM_CH2_RXLEN_IDX, IAL_DEFAULT_SIZE);
		ulCmdResultLen += sprintf((char *)pToken->abWorkingCmdLine + strlen((char*)(pToken->abWorkingCmdLine)), "CH2: %d, %d\n\r", ulValue3, ulValue4);
//		ulCmdResultLen += sprintf((char *)pToken->abWorkingCmdLine + strlen((char*)(pToken->abWorkingCmdLine)), "CH2: %d, %d, %d\n\r", ulValue2, ulValue3, ulValue4);

//		IalConfigGet(&ulValue2, IALCONFIG_FCOMM_CH3_ENABLE_IDX, IAL_DEFAULT_SIZE);
		IalConfigGet(&ulValue3, IALCONFIG_FCOMM_CH3_TXLEN_IDX, IAL_DEFAULT_SIZE);
		IalConfigGet(&ulValue4, IALCONFIG_FCOMM_CH3_RXLEN_IDX, IAL_DEFAULT_SIZE);
		ulCmdResultLen += sprintf((char *)pToken->abWorkingCmdLine + strlen((char*)(pToken->abWorkingCmdLine)), "CH3: %d, %d\n\r", ulValue3, ulValue4);
//		ulCmdResultLen += sprintf((char *)pToken->abWorkingCmdLine + strlen((char*)(pToken->abWorkingCmdLine)), "CH3: %d, %d, %d\n\r", ulValue2, ulValue3, ulValue4);

//		IalConfigGet(&ulValue2, IALCONFIG_FCOMM_CH4_ENABLE_IDX, IAL_DEFAULT_SIZE);
		IalConfigGet(&ulValue3, IALCONFIG_FCOMM_CH4_TXLEN_IDX, IAL_DEFAULT_SIZE);
		IalConfigGet(&ulValue4, IALCONFIG_FCOMM_CH4_RXLEN_IDX, IAL_DEFAULT_SIZE);
		ulCmdResultLen += sprintf((char *)pToken->abWorkingCmdLine + strlen((char*)(pToken->abWorkingCmdLine)), "CH4: %d, %d\n\n\r", ulValue3, ulValue4);
//		ulCmdResultLen += sprintf((char *)pToken->abWorkingCmdLine + strlen((char*)(pToken->abWorkingCmdLine)), "CH4: %d, %d, %d\n\n\r", ulValue2, ulValue3, ulValue4);

//		ulCmdResultLen = strlen((char *)pToken->abWorkingCmdLine);				
	}
	else if(pToken->wOp == (NA|EQ|AR))
	{

		if(pToken->wArgsFound == 3)
		{
			// Change Control Period via Ial
			ulValue1 = atol((char *)pToken->pArg[0]);
//				ulValue2 = atol((char *)pToken->pArg[1]);
			ulValue3 = atol((char *)pToken->pArg[1]);
			ulValue4 = atol((char *)pToken->pArg[2]);
//				ulValue3 = (ulValue3 == 0) ? ulValue3 : ulValue3 + PROTOCOL_HEADER_LEN;
//				ulValue4 = (ulValue3 == 0) ? ulValue4 : ulValue4 + PROTOCOL_HEADER_LEN;	

			switch(ulValue1)
			{
				case 1:
//						IalConfigSet(IALCONFIG_FCOMM_CH1_ENABLE_IDX, ulValue2, IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH1_TXLEN_IDX, ulValue3, IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH1_RXLEN_IDX, ulValue4, IALCONFIG_SAVE);
					break;

				case 2:
//						IalConfigSet(IALCONFIG_FCOMM_CH2_ENABLE_IDX, ulValue2, IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH2_TXLEN_IDX, ulValue3, IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH2_RXLEN_IDX, ulValue4, IALCONFIG_SAVE);
					break;


				case 3:
//						IalConfigSet(IALCONFIG_FCOMM_CH3_ENABLE_IDX, ulValue2, IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH3_TXLEN_IDX, ulValue3, IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH3_RXLEN_IDX, ulValue4, IALCONFIG_SAVE);
					break;

				case 4:
//						IalConfigSet(IALCONFIG_FCOMM_CH4_ENABLE_IDX, ulValue2, IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH4_TXLEN_IDX, ulValue3, IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH4_RXLEN_IDX, ulValue4, IALCONFIG_SAVE);
					break;

				default:
					ulCmdResult = CMD_EXEC_ERROR;
					break;				
			}
		}
//	
//			else if(pToken->wArgsFound == 2)
//			{
//				// Change Control Period via Ial
//				ulValue1 = atol((char *)pToken->pArg[0]);
//				ulValue2 = atol((char *)pToken->pArg[1]);
//	
//	 			switch(ulValue1)
//				{
//					case 1: // Channel Number
//						IalConfigSet(IALCONFIG_FCOMM_CH1_ENABLE_IDX, ulValue2, IALCONFIG_SAVE);
//						break;
//	
//					case 2:
//						IalConfigSet(IALCONFIG_FCOMM_CH2_ENABLE_IDX, ulValue2, IALCONFIG_SAVE);
//						break;
//	
//					case 3:
//						IalConfigSet(IALCONFIG_FCOMM_CH3_ENABLE_IDX, ulValue2, IALCONFIG_SAVE);
//						break;
//	
//					case 4:
//						IalConfigSet(IALCONFIG_FCOMM_CH4_ENABLE_IDX, ulValue2, IALCONFIG_SAVE);
//						break;
//	
//					default:
//						ulCmdResult = CMD_EXEC_ERROR;
//						break;				
//				}
//			}		
#if 0
		else if(pToken->wArgsFound == 1)
		{
			ulValue1 = atol((char *)pToken->pArg[0]);

			switch(ulValue1)
			{
				case 1:
					IalConfigGet(&ulValue2, IALCONFIG_FCOMM_CH1_ENABLE_IDX, IAL_DEFAULT_SIZE);
					IalConfigGet(&ulValue3, IALCONFIG_FCOMM_CH1_LENGTH_IDX, IAL_DEFAULT_SIZE);
					ulCmdResultLen = sprintf((char *)pToken->abWorkingCmdLine, "CH1: %d, %d\n\n\r", ulValue2, ulValue3);
					break;

				case 2:
					IalConfigGet(&ulValue2, IALCONFIG_FCOMM_CH2_ENABLE_IDX, IAL_DEFAULT_SIZE);
					IalConfigGet(&ulValue3, IALCONFIG_FCOMM_CH2_LENGTH_IDX, IAL_DEFAULT_SIZE);
					ulCmdResultLen = sprintf((char *)pToken->abWorkingCmdLine, "CH2: %d, %d\n\n\r", ulValue2, ulValue3);
					break;


				case 3:
					IalConfigGet(&ulValue2, IALCONFIG_FCOMM_CH3_ENABLE_IDX, IAL_DEFAULT_SIZE);
					IalConfigGet(&ulValue3, IALCONFIG_FCOMM_CH3_LENGTH_IDX, IAL_DEFAULT_SIZE);
					ulCmdResultLen = sprintf((char *)pToken->abWorkingCmdLine, "CH3: %d, %d\n\n\r", ulValue2, ulValue3);
					break;

				case 4:
					IalConfigGet(&ulValue2, IALCONFIG_FCOMM_CH4_ENABLE_IDX, IAL_DEFAULT_SIZE);
					IalConfigGet(&ulValue3, IALCONFIG_FCOMM_CH4_LENGTH_IDX, IAL_DEFAULT_SIZE);
					ulCmdResultLen = sprintf((char *)pToken->abWorkingCmdLine, "CH4: %d, %d\n\n\r", ulValue2, ulValue3);
					break;

				default:
					ulCmdResult = CMD_EXEC_ERROR;
					break;				
			}

		}
#endif
		else
		{
			ulCmdResult = CMD_EXEC_ERROR;
		}
	}
	else
	{
		// Cmd Error
		ulCmdResult = CMD_EXEC_ERROR;
	}
	return ulCmdResult;

//	DebugCmdExeResult(pToken->abWorkingCmdLine, ulCmdResultLen, ulCmdResult);
}

uint32_t DebugSYSINITCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulArg;

	if(pToken->wOp == (NA|EQ|AR))
	{
		if(pToken->wArgsFound == 1)
		{
			ulArg = Str2ULong((char *) pToken->pArg[0]);

			switch(ulArg)
			{

				case SYSTEM_MES:
					IalConfigSet(IALCONFIG_SYSTEMCFG_IDX,			SYSTEM_MES,			IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_MASTERSLAVE_IDX, 		SYNC_MASTER, 			IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH1_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH2_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH3_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH4_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH1_TXLEN_IDX, 	0,						IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH2_TXLEN_IDX,	IALMES_LOW_SPEED_FRAMESIZE,	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH3_TXLEN_IDX, 	IALMES_LOW_SPEED_FRAMESIZE,	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH4_TXLEN_IDX, 	IALMES_LOW_SPEED_FRAMESIZE, 	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH1_RXLEN_IDX, 	IALMES_LOW_SPEED_FRAMESIZE, 	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH2_RXLEN_IDX, 	0, 						IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH3_RXLEN_IDX, 	0, 						IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH4_RXLEN_IDX, 	0, 						IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_SDLY_IDX,				TIMER_SYNC_DELAY, 		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_TIMEPERIOD_IDX,			TIMEOUT_PERIOD, 			IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FACTORYINIT_DONE_IDX,		IALCONFIG_FACTORY_INIT_DONE,	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_MESMODE_IDX,				IALMES_LOW_SPEED, 				IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_MESID_IDX,					IALMES_ID_START, 				IALCONFIG_SAVE);
					break;
				case SYSTEM_CCS:
					IalConfigSet(IALCONFIG_SYSTEMCFG_IDX,			SYSTEM_CCS,				IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_MASTERSLAVE_IDX, 			SYNC_SLAVE, 			IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH1_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH2_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH3_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH4_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH1_TXLEN_IDX, 		0,						IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH2_TXLEN_IDX, 		IALCCS_TX_DEFAULT_SIZE,	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH3_TXLEN_IDX, 		IALCCS_TX_DEFAULT_SIZE,	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH4_TXLEN_IDX, 		IALCCS_TX_DEFAULT_SIZE, IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH1_RXLEN_IDX,	 	IALMES_HIGH_SPEED_FRAMESIZE, 		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH2_RXLEN_IDX, 		IALCCS_RX_DEFAULT_SIZE, 	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH3_RXLEN_IDX, 		IALCCS_RX_DEFAULT_SIZE, 	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH4_RXLEN_IDX, 		IALCCS_RX_DEFAULT_SIZE, 	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_SDLY_IDX,					TIMER_SYNC_DELAY, 		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_TIMEPERIOD_IDX,			TIMEOUT_PERIOD, 			IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FACTORYINIT_DONE_IDX,		IALCONFIG_FACTORY_INIT_DONE,	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_MESMODE_IDX,				IALMES_HIGH_SPEED, 				IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_MESID_IDX,					IALMES_ID_START, 				IALCONFIG_SAVE);
					break;
				case SYSTEM_VBC:
					IalConfigSet(IALCONFIG_SYSTEMCFG_IDX,			SYSTEM_VBC,				IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_MASTERSLAVE_IDX, 			SYNC_SLAVE, 			IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH1_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH2_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH3_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH4_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH1_TXLEN_IDX, 		IALVBC_TX_DEFAULT_SIZE,	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH2_TXLEN_IDX,	 	IALVBC_TX_DEFAULT_SIZE,	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH3_TXLEN_IDX,	 	IALVBC_RTDS_TX_SIZE,	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH4_TXLEN_IDX,	 	IALVBC_RTDS_TX_SIZE, 	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH1_RXLEN_IDX, 		IALMES_HIGH_SPEED_FRAMESIZE, 		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH2_RXLEN_IDX, 		IALVBC_RX_DEFAULT_SIZE, 	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH3_RXLEN_IDX, 		IALVBC_RTDS_RX_SIZE, 	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH4_RXLEN_IDX, 		IALVBC_RTDS_RX_SIZE, 	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_SDLY_IDX,					TIMER_SYNC_DELAY, 		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_TIMEPERIOD_IDX,			TIMEOUT_PERIOD, 			IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FACTORYINIT_DONE_IDX,		IALCONFIG_FACTORY_INIT_DONE,	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_MESMODE_IDX,				IALMES_HIGH_SPEED, 				IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_MESID_IDX,					IALMES_ID_START, 				IALCONFIG_SAVE);
					break;
				case SYSTEM_MES_HS:
					IalConfigSet(IALCONFIG_SYSTEMCFG_IDX,			SYSTEM_MES,			IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_MASTERSLAVE_IDX, 			SYNC_MASTER, 			IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH1_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH2_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH3_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH4_ENABLE_IDX, 	HALFCOMM_ENABLE,		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH1_TXLEN_IDX, 	0,	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH2_TXLEN_IDX,	 	IALMES_HIGH_SPEED_FRAMESIZE,	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH3_TXLEN_IDX,	 	IALMES_HIGH_SPEED_FRAMESIZE,	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH4_TXLEN_IDX,	 	IALMES_HIGH_SPEED_FRAMESIZE, 	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH1_RXLEN_IDX, 	IALMES_HIGH_SPEED_FRAMESIZE, 	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH2_RXLEN_IDX, 	0, 	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH3_RXLEN_IDX, 	0, 	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FCOMM_CH4_RXLEN_IDX, 	0, 	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_SDLY_IDX,				TIMER_SYNC_DELAY, 		IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_TIMEPERIOD_IDX,			TIMEOUT_PERIOD, 			IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_FACTORYINIT_DONE_IDX,	IALCONFIG_FACTORY_INIT_DONE,	IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_MESMODE_IDX,				IALMES_HIGH_SPEED, 				IALCONFIG_SAVE);
					IalConfigSet(IALCONFIG_MESID_IDX,				IALMES_ID_START, 				IALCONFIG_SAVE);
					break;
				default:
					ulCmdResult = CMD_EXEC_ERROR;
					break;

			}

		}
		else
		{
			ulCmdResult = CMD_EXEC_ERROR;
		}

	}
	else
	{
		// Cmd Error
		ulCmdResult = CMD_EXEC_ERROR;
	}
	
	return ulCmdResult;
}


uint32_t DebugSYSCFGCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulArg;

	if(pToken->wOp == (NA|QU))
	{
		ulCmdResultLen += sprintf((char *)pToken->abWorkingCmdLine, "%d\n\r", ulCfgSystemCfg);
		ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "\n");
	}	
	else if(pToken->wOp == (NA|EQ|AR))
	{
		if(pToken->wArgsFound == 1)
		{
			ulArg = Str2ULong((char *) pToken->pArg[0]);
			if(ulArg > SYSTEM_UNKNOWN)
			{
				ulCmdResult = CMD_EXEC_ERROR;
			}
			else
			{
				IalConfigSet(IALCONFIG_SYSTEMCFG_IDX, ulArg, IALCONFIG_SAVE);
				//ulSystemCfg = ulArg;
			}			

		}
		else
		{
			ulCmdResult = CMD_EXEC_ERROR;
		}

	}
	else
	{
		// Cmd Error
		ulCmdResult = CMD_EXEC_ERROR;
	}
	
	return ulCmdResult;
}

uint32_t DebugMESIDCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulArg;

	if(pToken->wOp == (NA|QU))
	{
		ulCmdResultLen += sprintf((char *)pToken->abWorkingCmdLine, "%d\n\r", ulCfgMesId);
		ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "\n");
	}	
	else if(pToken->wOp == (NA|EQ|AR))
	{
		if(pToken->wArgsFound == 1)
		{

			if(ulCfgSystemCfg == SYSTEM_MES)
			{
				ulArg = Str2ULong((char *) pToken->pArg[0]);
				if(ulArg > IALMES_ID_MAX)
				{
					ulCmdResult = CMD_EXEC_ERROR;
				}
				else 
				{
					IalConfigSet(IALCONFIG_MESID_IDX, ulArg, IALCONFIG_SAVE);
					if(ulArg == IALMES_ID_START)
					{
						IalConfigSet(IALCONFIG_MASTERSLAVE_IDX, SYNC_MASTER, IALCONFIG_SAVE);
					}
					else
					{
						IalConfigSet(IALCONFIG_MASTERSLAVE_IDX, SYNC_SLAVE, IALCONFIG_SAVE);
					}
				}
			}
			else
			{
				ulCmdResult = CMD_EXEC_ERROR;
			}	
		}
		else
		{
			ulCmdResult = CMD_EXEC_ERROR;
		}

	}
	else
	{
		// Cmd Error
		ulCmdResult = CMD_EXEC_ERROR;
	}
	
	return ulCmdResult;
}


volatile char	aDate[] = __DATE__;
volatile char	aTime[] = __TIME__;

uint32_t DebugVERCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	
	uint32_t ulValue;

	if(pToken->wOp == (NA))
	{
		ulCmdResultLen = sprintf((char *)pToken->abWorkingCmdLine, "%s - %s\n\r", aDate, aTime);		
		ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "\n");
	}	
	else
	{
		// Cmd Error
		ulCmdResult = CMD_EXEC_ERROR;
	}

	return ulCmdResult;
}

uint32_t ulTestDelay = 80;
extern uint32_t ulAlgorithmPeriod;
uint32_t DebugDLYTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulValue;
	uint32_t ulLoop;

	if(pToken->wOp == (NA|QU))
	{
		// Read Control Period via Ial
		ulCmdResultLen = sprintf((char *)pToken->abWorkingCmdLine, "%d\n\n\r", ulTestDelay);

	}	
	else if(pToken->wOp == (NA|EQ|AR))
	{
		ulValue = Str2ULong((char *)pToken->pArg[0]);
		ulAlgorithmPeriod = ulValue;
		ulTestDelay	= ulValue;

	}	
	else
	{
		// Cmd Error
//		DebugCmdExeResult(pToken->abWorkingCmdLine, CMD_RESULT_LENGTH_NOMSG, CMD_EXEC_ERROR);
		ulCmdResult = CMD_EXEC_ERROR;
	}
	return ulCmdResult;

}


uint32_t DebugMODECmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulValue;

	if(pToken->wOp == (NA|QU))
	{
		// Read Control Period via Ial
		IalConfigGet(&ulValue, IALCONFIG_MESMODE_IDX, IAL_DEFAULT_SIZE);
		ulCmdResultLen = sprintf((char *)pToken->abWorkingCmdLine, "%d\n\n\r", ulValue);		
	}
	else if(pToken->wOp == (NA|EQ|AR))
	{
		if(ulCfgSystemCfg == SYSTEM_MES)
		{
			// Change Control Period via Ial
			ulValue = atol((char *)pToken->pArg[0]);

			if(ulValue == IALMES_HIGH_SPEED || ulValue == IALMES_LOW_SPEED)
			{
				IalConfigSet(IALCONFIG_MESMODE_IDX, ulValue, IALCONFIG_SAVE);
			}
			else
			{
				ulCmdResult = CMD_EXEC_ERROR;
			}
		}
		else
		{
			ulCmdResult = CMD_EXEC_ERROR;
		}	

	}
	else
	{
		// Cmd Error
		ulCmdResult = CMD_EXEC_ERROR;
	}
	return ulCmdResult;
}

