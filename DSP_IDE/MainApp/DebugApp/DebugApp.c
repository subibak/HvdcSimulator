
/**
 * DebugApp.c
 * DebugApp Main Module Definition
 */


#include "DebugApp.h"
#include "Hal/HalFlash.h"

uint32_t	ulDebugAppState = DEBUGAPP_STATE_READY;


void DebugAppInit(void)
{

	/* Step 1: Init App Variables */
	DebugCmdTableInit();
	DebugCmdParserInit();
	DebugCmdExeInit();

	/* Step 2: Init Ial Layer */
	IalDebugInit();


	/* Step 3:  */
	ulDebugAppState = DEBUGAPP_STATE_READY;
	ulInboundSts = INBOUND_STS_GET;


	/* Step 4:  */	

}


#define DEBUGAPP_CMD_SEARCH_NUM		2


CmdTokenType	stCmdToken;
uint32_t		ulStartIdx;
int32_t		lCmdIdx;



void DebugAppRun(void)
{

	uint32_t		ulLen = MAX_LINE_SIZE;
	CmdTokenType		*pToken = &stCmdToken;
	uint32_t			ulLoop;

	uint32_t ulExeResult;
	

	/* Step 2: Debug Console Data Processing routine */
	switch(ulDebugAppState)
	{
		case DEBUGAPP_STATE_READY:
//			ulInboundSts = INBOUND_STS_GET;
			if(ulInboundSts == INBOUND_STS_LINE)
			{
				// Step #1: Read Cmd Line
				IalDebugRead(pToken->abWorkingCmdLine, &ulLen);
				ulDebugAppState = DEBUGAPP_STATE_PARSE;
				ulStartIdx		= 0;
			}
			break;
		case DEBUGAPP_STATE_PARSE:
			// Step #2: Parse Cmd Line to token structs
			ulLen = strlen(pToken->abWorkingCmdLine);
			DebugCmdParserRun((uint8_t *)pToken->abWorkingCmdLine, ulLen, pToken);
			if((pToken->wOp & NA) == NA)
			{
				ulDebugAppState = DEBUGAPP_STATE_SEARCH;
				
			}
			else
			{
				ulDebugAppState = DEBUGAPP_STATE_READY;
			}
			
			break;
		case DEBUGAPP_STATE_SEARCH:
			lCmdIdx = DebugCmdTableRun(pToken, ulStartIdx, DEBUGAPP_CMD_SEARCH_NUM);
			if(lCmdIdx ==  DEBUGCMDTABLE_FIND_ERROR)
			{
				ulStartIdx = ulStartIdx + DEBUGAPP_CMD_SEARCH_NUM;
			}
			else if(lCmdIdx ==  DEBUGCMDTABLE_NOCMD_ERROR)
			{
				ulDebugAppState = DEBUGAPP_STATE_FINAL;
				ulCmdResult = CMD_EXEC_ERROR;
			}
			else
			{
				ulDebugAppState = DEBUGAPP_STATE_EXECUTE;
			}
			break;
		case DEBUGAPP_STATE_EXECUTE:
			// Step #2: Parse Cmd Line to token struct
			ulExeResult = DebugCmdExeRun(pToken, lCmdIdx);
			if(ulExeResult == CMD_EXEC_PENDING)
			{
				;
			}
			else
			{
				//		IalDebugWrite("CMD LINE DETECTED!\r\n", 20);
	  			ulDebugAppState = DEBUGAPP_STATE_FINAL;
			}
			break;

		case DEBUGAPP_STATE_FINAL:
			DebugCmdExeFinalize(pToken, lCmdIdx);
			ulInboundSts = INBOUND_STS_GET;
			
  			ulDebugAppState = DEBUGAPP_STATE_READY;
			ulStartIdx		= 0;
			break;

		default:
  			ulDebugAppState = DEBUGAPP_STATE_READY;
			break;

	}

	IalDebugHandle();

	
	/* Step 1: Process Ial Data Process */
//	IalDebugHandle(ST_BUFFER_TYPE *psTxData, ST_BUFFER_TYPE *psRxData);

	/* Step 2: Ial Inbound Data */


	/* Step 3: Handle Command */
	// Dispatch Command and Execute Handler



	/* Step 4: Flush Read Buffer */




}

#include "Core/Crc.h"

uint32_t ulTest[500];

extern unsigned char	*dest;
extern int32_t			ulNewImgLen;


uint32_t DebugSWUPDATECmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulValue;
	uint32_t ulLoop;

	if(pToken->wOp == (NA))
	{
		ulDebugMode = IALDEBUG_MODE_DLOAD;

		HalFlashWpDisable();

		HalFlashEraseChip();
		HalFlasSwUpgrade((uint32_t *)dest, ulNewImgLen);

		HalFlashWpEnable();

		ulDebugMode = IALDEBUG_MODE_NORMAL;

		
//		IalDebugWrite("", );
//		ulCmdResultLen = sprintf((char *)pToken->abWorkingCmdLine, "%d\n\r", ulDebugMode);		
//		ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "\n");
	}	
	else if(pToken->wOp == (NA|EQ))
	{
		ulDebugMode = IALDEBUG_MODE_DLOAD;

		HalFlashWpDisable();

		HalFlashEraseChip();

		HalFlashWpEnable();

		ulDebugMode = IALDEBUG_MODE_NORMAL;

	}	
	else if(pToken->wOp == (NA|EQ|AR))
	{
		ulValue = atol((char *)pToken->pArg[0]);

		if(ulValue == IALDEBUG_MODE_IOTEST)
		{
			ulDebugMode = IALDEBUG_MODE_IOTEST;
		}
		else if(ulValue == IALDEBUG_MODE_NORMAL)
		{
			
			ulDebugMode = IALDEBUG_MODE_NORMAL;
		}
		else
		{
			ulCmdResult = CMD_EXEC_ERROR;
		}
		

	}	
	else
	{
		// Cmd Error
//		DebugCmdExeResult(pToken->abWorkingCmdLine, CMD_RESULT_LENGTH_NOMSG, CMD_EXEC_ERROR);
		ulCmdResult = CMD_EXEC_ERROR;
	}
	return ulCmdResult;

}


#define LINE_NUM	4
uint32_t DebugDCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulArg1, ulArg2;
	uint32_t i;
	uint32_t iPrint;

	if(pToken->wOp == (NA))
	{
		IalDebugWrite((uint8_t *)"Usage: D=<addr>,<len>\r\n", 23);
		ulCmdResultLen = sprintf((char *)pToken->abWorkingCmdLine, "%d\n\r", ulDebugMode);		
		ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "\n");

	}	
	else if(pToken->wOp == (NA|EQ|AR))
	{


		if(pToken->wArgsFound == 2)
		{
			ulArg1 = Str2ULong((char *) pToken->pArg[0]);
			ulArg2 = Str2ULong((char *) pToken->pArg[1]);
			ulCmdResultLen = 0;
			iPrint=0;


			if(ulArg2 <= 0x20)
			{
				for(i=0;i<ulArg2;i++)
				{
					if(iPrint%LINE_NUM == 0)
					{
						ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "%02d: ",iPrint);
					}
					ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "%08x ", *((uint32_t *)(ulArg1+i)));
					iPrint++;
					if(iPrint%LINE_NUM == 0)
					{
						//iPrintf = 0;
						ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "\n\r");
					}

				}

			}
			else
			{
				ulCmdResult = CMD_EXEC_ERROR;
			}

			ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "\r\n\n");
			
			
		}
		else
		{
			ulCmdResult = CMD_EXEC_ERROR;
		}
		

	}	
	else
	{
		// Cmd Error
//		DebugCmdExeResult(pToken->abWorkingCmdLine, CMD_RESULT_LENGTH_NOMSG, CMD_EXEC_ERROR);
		ulCmdResult = CMD_EXEC_ERROR;
	}
	return ulCmdResult;

}

uint32_t DebugFCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulArg1, ulArg2, ulArg3;
	uint32_t i;
	uint32_t iPrint;

	if(pToken->wOp == (NA))
	{
		IalDebugWrite((uint8_t *)"Usage: F=<addr>,<data>,<len>\r\n", 23);
		ulCmdResultLen = sprintf((char *)pToken->abWorkingCmdLine, "%d\n\r", ulDebugMode);		
		ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "\n");

	}	
	else if(pToken->wOp == (NA|EQ|AR))
	{


		if(pToken->wArgsFound == 3)
		{
			ulArg1 = Str2ULong((char *) pToken->pArg[0]);
			ulArg2 = Str2ULong((char *) pToken->pArg[1]);
			ulArg3 = Str2ULong((char *) pToken->pArg[2]);
			ulCmdResultLen = 0;
			iPrint=0;


			for(i=0;i<ulArg3;i++)
			{

				*((uint32_t *)(ulArg1+i)) = ulArg2;

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
//		DebugCmdExeResult(pToken->abWorkingCmdLine, CMD_RESULT_LENGTH_NOMSG, CMD_EXEC_ERROR);
		ulCmdResult = CMD_EXEC_ERROR;
	}
	return ulCmdResult;

}


#ifdef FEATURE_DEBUG_VARIABLE

#define DBG_MODE_READY		0x0
#define DBG_MODE_PENDING	0x1
#define ASCII_ESC		27
uint32_t DebugDBGRCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
//	uint32_t ulArg1, ulArg2;


	uint32_t ulLoop;
	static uint32_t	ulStart, ulEnd, ulCurr;
	static uint32_t ulMode = DBG_MODE_READY;
	uint8_t		aulBuffer[30];
	uint32_t	ulLen = 0;

	switch(ulMode)
	{
		case DBG_MODE_PENDING:
			// 이어지는 출력.
			// IalDebugWrite("");
			// 종료 조건 필요.
			if(HAL_NOERR == HalUartRead(aulBuffer, 1))
			{
				ulCmdResult = CMD_EXEC_OK;
				ulMode = DBG_MODE_READY;// 종료.
			}
			else
			{
				if(ulStart == ulCurr)
				{
//					ulLen = sprintf((char *)aulBuffer,"\f");


					ulLen = sprintf((char *)aulBuffer, "%c[2J%c[0;0H", ASCII_ESC,ASCII_ESC );
//					ulLen += sprintf((char *)aulBuffer, "%c[1;1H", ASCII_ESC );
//					ulLen = sprintf( "%c", ASCII_ESC );
//					ulLen += sprintf( "%c", ASCII_ESC );
				}

				{
					ST_DBG_TYPE *pstTbl;

					pstTbl = &(astDbgTbl[ulCurr]);

					if(pstTbl->ulType == FLOAT_TYPE)
					{
						ulLen += sprintf((char *)(aulBuffer+ulLen),"%8s: %4.5f", (char *)(pstTbl->pbName), *((float *)(pstTbl->pValue)));
					}
					else if(astDbgTbl[ulCurr].ulType == HEX_TYPE)
					{
						ulLen += sprintf((char *)(aulBuffer+ulLen),"%8s: 0x%8X", (char *)(pstTbl->pbName), *((uint32_t *)(pstTbl->pValue)));
					}
					else
					{
						ulLen += sprintf((char *)(aulBuffer+ulLen),"%8s: 10d", (char *)(pstTbl->pbName), *((uint32_t *)(pstTbl->pValue)));
					}

					if((ulCurr - ulStart) %3 == 2)
					{
						ulLen += sprintf((char *)(aulBuffer+ulLen),"\r\n");
					}
					else 
					{
						ulLen += sprintf((char *)(aulBuffer+ulLen),"\t");
					}

					if(IALDEBUG_OUTBOUND_FULL == IalDebugWrite(aulBuffer, ulLen))
					{
						;
					}
					else
					{
						ulCurr++;
					}

					if(ulCurr > ulEnd)
					{
						ulCurr = ulStart;
					}
				}
			
				;// 상태 지속.
			}
			break;
		default:
			// 최초 Cmd 처리
			if(pToken->wOp == (NA|EQ|AR))
			{
				if(pToken->wArgsFound == 2)
				{
					ulStart = atol((char *)pToken->pArg[0]);
					ulEnd = atol((char *)pToken->pArg[1]);
					ulCurr = ulStart;
					if(ulDbgTblSize > ulEnd)
					{
						ulCmdResult = CMD_EXEC_PENDING;
						ulMode = DBG_MODE_PENDING;
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
		//		DebugCmdExeResult(pToken->abWorkingCmdLine, CMD_RESULT_LENGTH_NOMSG, CMD_EXEC_ERROR);
				ulCmdResult = CMD_EXEC_ERROR;
			}
			break;

	}
	return ulCmdResult;

}

uint32_t DebugDBGWCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
//	uint32_t ulArg1, ulArg2;


	ST_DBG_TYPE *pstTbl;
	uint8_t		aulBuffer[30];
	uint32_t	ulArg1, ulArg2;
	uint32_t	ulLen = 0;

	// 최초 Cmd 처리
	if(pToken->wOp == (NA|EQ|AR))
	{

		if(pToken->wArgsFound == 2)
		{
			ulArg1 = Str2ULong((char *) pToken->pArg[0]);
			ulArg2 = Str2ULong((char *) pToken->pArg[1]);


			if(ulDbgTblSize > ulArg1)
			{
				pstTbl = &(astDbgTbl[ulArg1]);
				*((uint32_t *)(pstTbl->pValue)) = ulArg2;

				ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "%s: 0x%x\r\n", pstTbl->pbName, ulArg2);
				ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "\n");

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
//		DebugCmdExeResult(pToken->abWorkingCmdLine, CMD_RESULT_LENGTH_NOMSG, CMD_EXEC_ERROR);
		ulCmdResult = CMD_EXEC_ERROR;
	}

	return ulCmdResult;

}


#endif


uint32_t DebugIODLYCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulArg1;
	uint32_t ulLoop;

	if(pToken->wOp == (NA|QU))
	{
		// Read Control Period via Ial
		ulCmdResultLen = sprintf((char *)pToken->abWorkingCmdLine, "%d\n\n\r", ulIoDelay);

	}		
	else if(pToken->wOp == (NA|EQ|AR))
	{

		if(pToken->wArgsFound == 1)
		{
			ulArg1 = Str2ULong((char *) pToken->pArg[0]);
			ulIoDelay = ulArg1; 			
		}
		else 
		{
			ulCmdResult = CMD_EXEC_ERROR;

		}
	}	
	else
	{
		// Cmd Error
//		DebugCmdExeResult(pToken->abWorkingCmdLine, CMD_RESULT_LENGTH_NOMSG, CMD_EXEC_ERROR);
		ulCmdResult = CMD_EXEC_ERROR;
	}
	return ulCmdResult;

}


