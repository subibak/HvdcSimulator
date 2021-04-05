/**
 * HalUart.c
 * HalUart Device Driver Module Implementation
 */

#include "HalUart.h"

ST_BUFFER_TYPE	t_stUartTxBuffer;
ST_BUFFER_TYPE	t_stUartRxBuffer;

char			abTxBuffer[HALUART_BUFFER_SIZE];
char			abRxBuffer[HALUART_BUFFER_SIZE];

char	cTxData[100];
char	cRxData[20];

HAL_ERROR HalUartInit(void)
{
	WriteReg(HWREG_UART_LCR, (HWREG_UART_LCR_DLAB_1));
	WriteReg(HWREG_UART_DLL, (HWREG_UART_DLL_115200)); 
	WriteReg(HWREG_UART_DLM, (HWREG_UART_DLM_0));

	WriteReg(HWREG_UART_LCR, (HWREG_UART_LCR_DATALEN_8|HWREG_UART_LCR_STOPBIT_1|HWREG_UART_LCR_PARITY_NONE));
	WriteReg(HWREG_UART_FCR, (HWREG_UART_FCR_RCVFIFO_TRIG_14|HWREG_UART_FCR_CLR_TXFIFO_CLR|HWREG_UART_FCR_CLR_RXFIFO_CLR|HWREG_UART_FCR_ENFIFO_EN));
	WriteReg(HWREG_UART_IER, (HWREG_UART_IER_INT_DISABLE));
	WriteReg(HWREG_UART_FCR, (HWREG_UART_FCR_RCVFIFO_TRIG_14|HWREG_UART_FCR_ENFIFO_EN));		

	t_stUartTxBuffer.ulSize			= HALUART_BUFFER_SIZE;
	t_stUartTxBuffer.ulHead			= 0;
	t_stUartTxBuffer.ulTail			= 0;
	t_stUartTxBuffer.ulUnusedSize	= HALUART_BUFFER_SIZE;
	t_stUartTxBuffer.ulUsedSize		= 0;
	t_stUartTxBuffer.uBody.pcBuffer = abTxBuffer;

	t_stUartRxBuffer.ulSize			= HALUART_BUFFER_SIZE;
	t_stUartRxBuffer.ulHead			= 0;
	t_stUartRxBuffer.ulTail			= 0;
	t_stUartRxBuffer.ulUnusedSize	= HALUART_BUFFER_SIZE;
	t_stUartRxBuffer.ulUsedSize		= 0;
	t_stUartRxBuffer.uBody.pcBuffer	= abRxBuffer;

	return HAL_NOERR;
	
}

/* Write Data to Tx Buffer */
HAL_ERROR HalUartWrite(uint8_t *pbData, uint32_t ulLen)
{
	ST_BUFFER_TYPE *pBuffer = &t_stUartTxBuffer;
	HAL_ERROR	eResult = HAL_NOERR;
	uint32_t	ulStsReg;

	QWrite(pBuffer, pbData, ulLen);

	ReadReg(HWREG_UART_LSR, &ulStsReg);

	if((ulStsReg & HWREG_UART_LSR_TXBUFF_EMPTY_MASK) == HWREG_UART_LSR_TXBUFF_EMPTY_YES)
	{
		uint32_t	ulLoop;
		
		if(pBuffer->ulUsedSize > UART_TX_SIZE)
		{
			ulLen = UART_TX_SIZE;
		}
		else
		{
			ulLen = pBuffer->ulUsedSize;
		}

		QRead(pBuffer, (uint8_t *) cTxData, ulLen);
		for(ulLoop = 0;ulLoop < ulLen;ulLoop++)
		{
			WriteReg(HWREG_UART_THR, (cTxData[ulLoop] & 0xFF));
		}
		
	}

	return eResult;

}


/* Write Data to Tx Buffer */
HAL_ERROR HalUartSyncWrite(uint8_t *pbData, uint32_t ulLen)
{
	uint32_t	uli = 0, ulTempLen;
	uint32_t	ulStsReg;
	HAL_ERROR	eResult = HAL_NOERR;
	

	do
	{
		ReadReg(HWREG_UART_LSR, &ulStsReg);

		if(ulLen > UART_TX_SIZE)
		{
			ulTempLen = UART_TX_SIZE;
		}
		else
		{
			ulTempLen = ulLen;
		}

		if((ulStsReg & HWREG_UART_LSR_TXBUFF_EMPTY_MASK) == HWREG_UART_LSR_TXBUFF_EMPTY_YES)
		{
			uint32_t	ulLoop;

			for(uli = 0;uli < ulTempLen;uli++)
			{
				WriteReg(HWREG_UART_THR, (pbData[uli] & 0xFF));
			}
			pbData = pbData + ulTempLen;
			ulLen = ulLen - ulTempLen;
			DelayUs(1000);

		}
			

	} while (ulLen > 0);

	return eResult;

}


/* Read Data from Rx Buffer until '\r\n'*/
HAL_ERROR HalUartRead(uint8_t *pbData, uint32_t ulLen)
{
	ST_BUFFER_TYPE *pBuffer = &t_stUartRxBuffer;
	HAL_ERROR	eResult = HAL_NOERR;
	QRESULT		eQResult = NO_QERROR;
	uint32_t	ulLSRReg;
	uint32_t	ulRcvLen = 0;

	ReadReg(HWREG_UART_LSR, &ulLSRReg);

	while((ulLSRReg & HWREG_UART_LSR_DATA_RDY_MASK) == HWREG_UART_LSR_DATA_RDY_YES)
	{
		ReadReg(HWREG_UART_RBR, &(cRxData[ulRcvLen++]));
		ReadReg(HWREG_UART_LSR, &ulLSRReg);	
	}

	// Write Data to Hal Buffer
	if(ulRcvLen > 0)
	{
		QWrite(pBuffer, (uint8_t *)cRxData, ulRcvLen);		
	}

	eQResult = QRead(pBuffer, (uint8_t *)pbData, ulLen);		

	if(eQResult == UNDERFLOW_QERROR)
	{
		eResult = HALUART_RXBUFFER_NODATA;

	}	

	return eResult;

}


/* Operation between Uart Buffer and Uart Hw*/
HAL_ERROR HalUartHandle()//ST_BUFFER_TYPE *psTxData, ST_BUFFER_TYPE *psRxData)
{

	ST_BUFFER_TYPE *pTxBuffer = &t_stUartTxBuffer;
	ST_BUFFER_TYPE *pRxBuffer = &t_stUartRxBuffer;
	uint8_t			bTemp = 0;

	uint32_t ulLSRReg;

	ReadReg(HWREG_UART_LSR, &ulLSRReg);

	// Tx Handle Part
	// No Push, but HW Handle
	HalUartWrite(&bTemp, 0);

	// Rx Handle Part
	HalUartRead(&bTemp, 0);

	return HAL_NOERR;
}

HAL_ERROR HalUartFlush(void)
{
	ST_BUFFER_TYPE *pBuffer = &t_stUartRxBuffer;
	HAL_ERROR	eResult = HAL_NOERR;
	QRESULT		eQResult = NO_QERROR;

	uint32_t	ulLSRReg;
	uint32_t	ulRcvLen = 0;
	uint32_t	ulRBRReg;

	ReadReg(HWREG_UART_LSR, &ulLSRReg);
		
	while((ulLSRReg & HWREG_UART_LSR_RXBUFF_EMPTY_MASK) == HWREG_UART_LSR_RXBUFF_EMPTY_NO)
	{
		ReadReg(HWREG_UART_RBR, &ulRBRReg);
		ReadReg(HWREG_UART_LSR, &ulLSRReg);	
	}

	eQResult = QFlush(&t_stUartRxBuffer);		

	if(eQResult == UNDERFLOW_QERROR)
	{
		eResult = HALUART_RXBUFFER_NODATA;

	}	

	return eResult;

}


HAL_ERROR HalUartGetTxUnusedSize(uint32_t *ulSize)
{
	*ulSize = t_stUartTxBuffer.ulUnusedSize;
	return HAL_NOERR;
}



