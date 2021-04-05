#include "IalDebug.h"






ST_BUFFER_TYPE	t_stConsoleTxBuffer;
ST_BUFFER_TYPE	t_stConsoleRxBuffer;


char	bOutboundBuffer[OUTBOUND_BUFFER_SIZE];		/// Connected to Console Output
char	bInboundBuffer[INBOUND_BUFFER_SIZE];		/// Connected to Console Input


uint32_t ulInboundIdx = 0;
uint32_t ulInboundSts = INBOUND_STS_GET;
uint32_t ulInboundChk = 5;


uint32_t	ulDebugMode = IALDEBUG_MODE_NORMAL;

uint32_t	ulIoDelay = 0;



#ifdef FEATURE_DEBUG_VARIABLE

#ifdef FEATURE_IAL_BUFFERINTEGRATION
extern uint32_t	ulInput[];
extern uint32_t	ulOutput[];
#endif

extern uint32_t ulIntStart;
extern uint32_t ulIntEnd;
extern uint32_t ulControlStartCnt;
extern uint32_t ulControlEndCnt;

uint32_t	ulTest = 0x80;
float fTest1 = 0.1;
float fTest2 = 1.5;
float fTest12 = 11.5;

uint32_t	ulDbgTblSize;

ST_DBG_TYPE	astDbgTbl[] = {
//	{"MngrCnt",		(void *) &ulIntStart,			HEX_TYPE},		/*	0  */	
//	{"CtrlCnt",		(void *) &ulControlStartCnt,	HEX_TYPE},		/*	1  */	
	{"ErrCause",	(void *) &ulDiagSt,				HEX_TYPE},		/*	0  */
	{"VMEOUT00",	(void *) &ulOutput[0],			HEX_TYPE},		/*	1  */
	{"VMEOUT01",	(void *) &ulOutput[1],			HEX_TYPE},		/*	2  */
	{"VMEOUT02",	(void *) &ulOutput[2],			HEX_TYPE},		/*	3  */
	{"VMEOUT03",	(void *) &ulOutput[3],			HEX_TYPE},		/*	4  */
	{"VMEOUT04",	(void *) &ulOutput[4],			HEX_TYPE},		/*	5  */
	{"VMEOUT05",	(void *) &ulOutput[5],			HEX_TYPE},		/*	6  */
	{"VMEOUT06",	(void *) &ulOutput[6],			HEX_TYPE},		/*	7  */
	{"VMEOUT07",	(void *) &ulOutput[7],			HEX_TYPE},		/*	8  */
	{"VMEOUT08",	(void *) &ulOutput[8],			HEX_TYPE},		/*	9  */
	{"VMEOUT09",	(void *) &ulOutput[9],			HEX_TYPE},		/*	10 */

 	{"VMEIN00",		(void *) &ulInput[0],			HEX_TYPE},		/*	11 */
	{"VMEIN01",		(void *) &ulInput[1],			HEX_TYPE},		/*	12 */
	{"VMEIN02",		(void *) &ulInput[2],			HEX_TYPE},		/*	13 */
	{"VMEIN03",		(void *) &ulInput[3],			HEX_TYPE},		/*	14  */
	{"VMEIN04",		(void *) &ulInput[4],			HEX_TYPE},		/*	15  */
	{"VMEIN05",		(void *) &ulInput[5],			HEX_TYPE},		/*	16  */
	{"VMEIN06",		(void *) &ulInput[6],			HEX_TYPE},		/*	17  */
	{"VMEIN07",		(void *) &ulInput[7],			HEX_TYPE},		/*	18  */
	{"VMEIN08",		(void *) &ulInput[8],			HEX_TYPE},		/*	19  */
	{"VMEIN09",		(void *) &ulInput[9],			HEX_TYPE},		/*	20 */	



	{"Va",		(void *) &fTest1,			FLOAT_TYPE}, 	/*	2 */	
	{"Vb",		(void *) &fTest2,			FLOAT_TYPE},	/*	3 */	
	{"Vc",		(void *) &ulTest,			HEX_TYPE},		/*	4 */	
	{"Vab",		(void *) &fTest1,			FLOAT_TYPE}, 	/*	5 */	
	{"Vbc",		(void *) &fTest12,			FLOAT_TYPE},	/*	6 */	
	{"Vca",		(void *) &ulTest,			HEX_TYPE},		/*	7 */	
	{"Va2b",	(void *) &fTest12,			FLOAT_TYPE}, 	/*	8 */	
	{"Vb2c",	(void *) &fTest2,			FLOAT_TYPE},	/*	9 */	
	{"Vc2a",	(void *) &ulTest,			HEX_TYPE},		/*	10*/	



	{"\0",		(void *) NULL,				FLOAT_TYPE}		/*	Last: Dummy */	

};
#endif

void IalDebugInit(void)
{



	/* Step 0: Init Ial Layer */
	t_stConsoleTxBuffer.ulSize			= OUTBOUND_BUFFER_SIZE;
	t_stConsoleTxBuffer.ulHead			= 0;
	t_stConsoleTxBuffer.ulTail			= 0;
	t_stConsoleTxBuffer.ulUnusedSize	= OUTBOUND_BUFFER_SIZE;
	t_stConsoleTxBuffer.ulUsedSize		= 0;
	t_stConsoleTxBuffer.uBody.pcBuffer	= bOutboundBuffer;

	t_stConsoleRxBuffer.ulSize			= INBOUND_BUFFER_SIZE;
	t_stConsoleRxBuffer.ulHead			= 0;
	t_stConsoleRxBuffer.ulTail			= 0;
	t_stConsoleRxBuffer.ulUnusedSize	= INBOUND_BUFFER_SIZE;
	t_stConsoleRxBuffer.ulUsedSize		= 0;
	t_stConsoleRxBuffer.uBody.pcBuffer	= bInboundBuffer;

	ulDebugMode = IALDEBUG_MODE_NORMAL;
	ulInboundIdx = 0;
	ulInboundChk = 5;
	
	ulDbgTblSize = sizeof(astDbgTbl)/sizeof(ST_DBG_TYPE) - 1;

	/* Step 1: Init Hal Layer */
	HalUartInit();

	/* Step 2:  */


	/* Step 3:  */


	/* Step 4:  */





}

/**
 * @brief	Debug 수신 값을 읽는 함수
 * @details	IalDebug 수신 버퍼에서 필요한 데이터를 읽는다. 
 * @param	psData : 데이터 반환을 위한 버퍼 포인터
 * @param	ulLen  : 반환가능한 최대 데이터 양
 * @return	uint32_t : API Error Cause
 * @date	2018-11-24
 * @version	0.0.1
 */
IALDEBUG_RESULT IalDebugRead(char *pcData, uint32_t *pulLen)
{

	IALDEBUG_RESULT	eIalResult = IALDEBUG_NOERR;
	ST_BUFFER_TYPE	*pBuffer = &t_stConsoleRxBuffer;
	uint8_t	*pbData = (uint8_t *)pcData;
	
	
	if(ulInboundSts == INBOUND_STS_LINE)
	{
		*pulLen = pBuffer->ulUsedSize;
		QRead(pBuffer, pbData, pBuffer->ulUsedSize);
	}
	else
	{
		eIalResult = IALDEBUG_NOCMDLINE;
	}

	return eIalResult;
}
/**
 * @brief	Debug 데이터의 송신을 요청하는 함수
 * @details	IalDebug 송신 버퍼에 데이터를 전달한다.
 * @param	psData : 송신 요청할 데이터 포인터
 * @param	ulLen  : 송신 요청한 데이터의 양
 * @return	채널값
 * @date	2018-11-24
 * @version	0.0.1
 */
IALDEBUG_RESULT IalDebugWrite(uint8_t *pbData, uint16_t ulLen)
{
//	if(UARTHAL_BUFFER_SIZE - 
	ST_BUFFER_TYPE *pBuffer = &t_stConsoleTxBuffer;
	IALDEBUG_RESULT	eResult = IALDEBUG_NOERR;
	uint32_t ulLoop;
	QRESULT			eQResult = NO_QERROR;


	if(ulLen > pBuffer->ulUnusedSize)
	{
		// Buffer Overflow
		eResult = IALDEBUG_OUTBOUND_FULL;

	}
	else
	{
		CLI();

#if 1
		eQResult = QWrite(pBuffer, pbData, ulLen);
		if(eQResult != NO_QERROR)
		{
			eResult = IALDEBUG_OUTBOUND_FULL;
		}

#else


		if(pBuffer->ulTail < pBuffer->ulHead)
		{
			// copy data From Tail of Buffer
			for(ulLoop = 0 ; ulLoop < ulLen ; ulLoop++)
			{
				pBuffer->uBody.p8Buffer[pBuffer->ulTail + ulLoop] = pbData[ulLoop];
			}

			// Move Head Position
			pBuffer->ulTail			= pBuffer->ulTail + ulLen;
			pBuffer->ulUnusedSize	= pBuffer->ulUnusedSize - ulLen;
			pBuffer->ulUsedSize		= pBuffer->ulUsedSize + ulLen;
		}
		else
		{
			uint32_t i;
			uint32_t ulUnusedBack	= pBuffer->ulSize - pBuffer->ulTail;
			uint32_t ulUnusedfront	= pBuffer->ulHead;
			
			// copy data From the Tail of Buffer
			for(ulLoop = 0 ; ulLoop < ulUnusedBack ; ulLoop++)
			{
				pBuffer->uBody.p8Buffer[pBuffer->ulTail + ulLoop] = pbData[ulLoop];
			}
			// copy data From the Start of Buffer
			for(ulLoop = 0 ; ulLoop < ulLen - ulUnusedBack ; ulLoop++)
			{
				pBuffer->uBody.p8Buffer[ulLoop] = pbData[ulUnusedBack + ulLoop];
			}

			// Move Head Position
			pBuffer->ulTail = ulLen - ulUnusedBack;
			pBuffer->ulUnusedSize = pBuffer->ulUnusedSize - ulLen;
			pBuffer->ulUsedSize = pBuffer->ulUsedSize + ulLen;

		}
#endif
		STI();

	}

	return eResult;

}


/**
 * @brief	Uart 수신 값을 읽는 함수
 * @details	Uart 수신 버퍼에 남아 있는 데이터를 읽는다. 
 * @param	ulLen  : Hw에서 1회에 수신할 최대 데이터 양
 * @return	uint32_t : API Error Cause
 * @date	2018-11-24
 * @version	0.0.1
 */
IALDEBUG_RESULT IalDebugConsoleRead(uint32_t ulLen)
{


	uint32_t	ulLoop;
	uint8_t		bChar;
	HAL_ERROR	eHalResult = HAL_NOERR;




//	HalUartHandle();

	if(ulInboundSts == INBOUND_STS_GET)
	{
		for(ulLoop = 0; ulLoop < ulInboundChk; ulLoop++)
		{
			eHalResult = HalUartRead(&bChar, INBOUND_CHK_SIZE);

			if(eHalResult == HAL_NOERR)
			{

				// Echo Char self
				if(bChar < '0')
				{
					if(bChar == CHAR_CR)
					{
						// Echo LF Char More
						QWrite(&t_stConsoleTxBuffer, &bChar, INBOUND_CHK_SIZE);
						bChar = CHAR_LF;
	//					HalUartWrite(&bChar, INBOUND_CHK_SIZE);
						QWrite(&t_stConsoleTxBuffer, &bChar, INBOUND_CHK_SIZE);

						// Change Char to NULL and Keep Char Rx Buffer
						bChar = CHAR_NULL;
						QWrite(&t_stConsoleRxBuffer, &bChar, INBOUND_CHK_SIZE);
						
	//					bInboundBuffer[ulInboundIdx++] = CHAR_NULL;
						ulInboundSts = INBOUND_STS_LINE;
					}
					else if(bChar == CHAR_BS)
					{

						if(t_stConsoleRxBuffer.ulUsedSize > 0)
						{
							QWrite(&t_stConsoleTxBuffer, &bChar, INBOUND_CHK_SIZE);
							QRollBack(&t_stConsoleRxBuffer);
						}
						
					}
					else if(bChar == CHAR_COMMA || bChar == CHAR_MINUS)
					{
						QWrite(&t_stConsoleTxBuffer, &bChar, INBOUND_CHK_SIZE);
						QWrite(&t_stConsoleRxBuffer, &bChar, INBOUND_CHK_SIZE);
					}
					else
					{
						;
					}
				}
				else
				{
					QWrite(&t_stConsoleTxBuffer, &bChar, INBOUND_CHK_SIZE);
					bChar = UPCASE(bChar);					
					QWrite(&t_stConsoleRxBuffer, &bChar, INBOUND_CHK_SIZE);
					
				}
				//HalUartWrite(&bChar, INBOUND_CHK_SIZE);
		
			}
			else
			{
				// No Uart Rx Data
				break;
			}			

		}

	}
	else	/// Cmd Line Detected, Wait
	{

	}


	return IALDEBUG_NOERR;

}
/**
 * @brief	Uart 데이터의 송신을 요청하는 함수
 * @details	Uart 송신 버퍼에 데이터를 전달한다.
 * @param	ulLen  : Hw로 1회에 송신할 최대 데이터양
 * @return	채널값
 * @date	2018-11-24
 * @version	0.0.1
 */
IALDEBUG_RESULT IalDebugConsoleWrite(uint32_t ulLen)
{
	ST_BUFFER_TYPE *pBuffer = &t_stConsoleTxBuffer;
//	uint8_t			*pbData	 = &(t_stConsoleTxBuffer.uBody.p8Buffer[t_stConsoleTxBuffer.ulHead]);
	IALDEBUG_RESULT	eResult = IALDEBUG_NOERR;
	uint32_t ulLoop;

	uint8_t			pbData[OUTBOUND_BUFFER_SIZE];
	uint32_t		ulHwUnusedSize;

	ulLen = pBuffer->ulUsedSize;

#if 1
	HalUartGetTxUnusedSize(&ulHwUnusedSize);
	ulLen = ulLen < ulHwUnusedSize?ulLen:ulHwUnusedSize;
	
	// Read From Ial Buffer
	// Write to  Hal Buffer
	QRead(pBuffer, pbData, ulLen);
	HalUartWrite(pbData, ulLen);
	
#else

	if(ulLen > pBuffer->ulUnusedSize)
	{
		// Buffer Overflow
		eResult = IALDEBUG_OUTBOUND_FULL;

	}
	else
	{
		CLI();

		if(pBuffer->ulHead < pBuffer->ulTail)
		{

#if 0		
			HalUartWrite(uint8_t * pbData, uint32_t ulLen)
			// copy data From Tail of Buffer
			for(ulLoop = 0 ; ulLoop < ulLen ; ulLoop++)
			{
				pBuffer->uBody.p8Buffer[pBuffer->ulTail + ulLoop] = pbData[ulLoop];
			}

			// Move Head Position
			pBuffer->ulTail			= pBuffer->ulTail + ulLen;
			pBuffer->ulUnusedSize	= pBuffer->ulUnusedSize - ulLen;
			pBuffer->ulUsedSize		= pBuffer->ulUsedSize + ulLen;
#endif
		}
		else
		{
			uint32_t i;
			uint32_t ulUnusedBack	= pBuffer->ulSize - pBuffer->ulTail;
			uint32_t ulUnusedfront	= pBuffer->ulHead;
#if 0			
			// copy data From the Tail of Buffer
			for(ulLoop= 0 ; ulLoop < ulUnusedBack ; ulLoop++)
			{
				pBuffer->uBody.p8Buffer[pBuffer->ulTail + ulLoop] = pbData[ulLoop];
			}
			// copy data From the Start of Buffer
			for(ulLoop = 0 ; ulLoop < ulLen - ulUnusedBack ; ulLoop++)
			{
				pBuffer->uBody.p8Buffer[ulLoop] = pbData[ulUnusedBack + ulLoop];
			}
#endif
			// Move Head Position
			pBuffer->ulTail = ulLen - ulUnusedBack;
			pBuffer->ulUnusedSize = pBuffer->ulUnusedSize - ulLen;
			pBuffer->ulUsedSize = pBuffer->ulUsedSize + ulLen;

		}

		STI();

	}
#endif
	return eResult;

}




/**
 * @brief	Ial Layer 의 Data 를 Hal Layer 로 분할 처리하는 함수
 * @details	Rx, Tx 데이터 모두 분할 처리 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
//uint32_t IalDebugConsoleHandle(ST_BUFFER_TYPE *psTxData, ST_BUFFER_TYPE *psRxData)
uint32_t IalDebugConsoleHandle(void)
{
	
//	IalDebugConsoleRead(5);
//	IalDebugConsoleWrite(5);



#if 1
	ST_BUFFER_TYPE *pTxBuffer = &t_stConsoleTxBuffer;
	ST_BUFFER_TYPE *pRxBuffer = &t_stConsoleRxBuffer;
//	uint8_t			*pbData	 = &(t_stConsoleTxBuffer.uBody.p8Buffer[t_stConsoleTxBuffer.ulHead]);
	IALDEBUG_RESULT	eResult = IALDEBUG_NOERR;

	uint32_t		ulLen = pTxBuffer->ulUsedSize;


	ulLen = pTxBuffer->ulUsedSize;
	eResult = IalDebugConsoleWrite(ulLen);
	eResult = IalDebugConsoleRead(ulLen);


#else
	uint32_t	ulLoop;
	uint8_t		bChar;
	HAL_ERROR	eHalResult = HAL_NOERR;




	HalUartHandle();

	if(ulInboundSts == INBOUND_STS_GET)
	{
		for(ulLoop = 0; ulLoop < ulInboundChk; ulLoop++)
		{
			eHalResult = HalUartRead(&bChar, INBOUND_CHK_SIZE);

			if(eHalResult == HAL_NOERR)
			{
				// Echo Char self
				HalUartWrite(&bChar, INBOUND_CHK_SIZE);
				
				if(bChar == CHAR_CR)
				{
					bChar = CHAR_LF;
					HalUartWrite(&bChar, INBOUND_CHK_SIZE);
					bInboundBuffer[ulInboundIdx++] = 0x0;
					ulInboundSts = INBOUND_STS_LINE;
				}
				else
				{
					bInboundBuffer[ulInboundIdx++];
				}
				
			}
			else
			{
				// No Uart Rx Data
				break;
			}			

		}

	}
	else	/// Cmd Line Detected, Wait
	{

	}
#endif

	return 0;

}


/**
 * @brief	Ial Layer 의 Data 를 Hal Layer 로 분할 처리하는 함수
 * @details	Rx, Tx 데이터 모두 분할 처리 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
//uint32_t IalDebugHandle(ST_BUFFER_TYPE *psTxData, ST_BUFFER_TYPE *psRxData)
uint32_t IalDebugHandle(void)
{

	// Console 의 Read, Write 를 처리하여,  Outbound/Inbound 버퍼를 조정한다.
	IalDebugConsoleHandle();

	// Check Ial Inbound Data and Return 1 line string

	return 0;
}

