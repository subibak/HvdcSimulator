/**
 * Queue.c
 * Queue Interface Module Definition
 */

#include "Queue.h"

#include "Hal/HalTS201.h"


QRESULT QWrite(ST_BUFFER_TYPE *pBuffer, uint8_t *pbData, uint32_t ulLen)
{
//	if(UARTHAL_BUFFER_SIZE - 
//	ST_BUFFER_TYPE *pBuffer = &t_stConsoleTxBuffer;
	QRESULT		eResult = NO_QERROR;
	uint32_t	ulLoop;


	if(ulLen == 0)
	{
		eResult = NO_QERROR;
	}

	else if(ulLen > pBuffer->ulUnusedSize)
	{
		// Buffer Overflow
		eResult = OVERFLOW_QERROR;

	}
	else
	{
		CLI();

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
			uint32_t ulUnusedBack	= pBuffer->ulSize - pBuffer->ulTail;
			uint32_t ulUnusedfront	= pBuffer->ulHead;

			if(ulLen < ulUnusedBack )
			{
				// copy data From the Tail of Buffer
				for(ulLoop = 0 ; ulLoop < ulLen ; ulLoop++)
				{
					pBuffer->uBody.p8Buffer[pBuffer->ulTail + ulLoop] = pbData[ulLoop];
				}
				pBuffer->ulTail			= pBuffer->ulTail + ulLen;

			}
			else
			{
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
				pBuffer->ulTail			= ulLen - ulUnusedBack;
			}
			

			// Move Head Position
			pBuffer->ulUnusedSize	= pBuffer->ulUnusedSize - ulLen;
			pBuffer->ulUsedSize		= pBuffer->ulUsedSize + ulLen;

		}

		STI();

//		pBuffer->ulTail = pBuffer->ulTail % pBuffer->ulSize;

	}

	return eResult;

}



QRESULT QRead(ST_BUFFER_TYPE *pBuffer, uint8_t *pbData, uint32_t ulLen)
{
//	if(UARTHAL_BUFFER_SIZE - 
//	ST_BUFFER_TYPE *pBuffer = &t_stUartTxBuffer;
	QRESULT		eResult = NO_QERROR;
	uint32_t	ulLoop;

	if(ulLen == 0)
	{
		eResult = NO_QERROR;
	}
	else if(ulLen > pBuffer->ulUsedSize)
	{
		// Buffer Overflow
		eResult = UNDERFLOW_QERROR;

	}
	else
	{
		CLI();

		if(pBuffer->ulTail > pBuffer->ulHead)
		{
			// copy data From Tail of Buffer
			for(ulLoop = 0 ; ulLoop < ulLen ; ulLoop++)
			{
				pbData[ulLoop] = pBuffer->uBody.p8Buffer[pBuffer->ulHead + ulLoop];
			}

			// Move Head Position
			pBuffer->ulHead			= pBuffer->ulHead + ulLen;
			pBuffer->ulUnusedSize	= pBuffer->ulUnusedSize + ulLen;
			pBuffer->ulUsedSize		= pBuffer->ulUsedSize - ulLen;
		}
		else
		{
			uint32_t ulUsedBack		= pBuffer->ulSize - pBuffer->ulHead;
			uint32_t ulUsedfront	= pBuffer->ulTail;

			if(ulLen < ulUsedBack)
			{
				for(ulLoop = 0 ; ulLoop < ulLen ; ulLoop++)
				{
					pbData[ulLoop] = pBuffer->uBody.p8Buffer[pBuffer->ulHead + ulLoop];
				}
				pBuffer->ulHead			= pBuffer->ulHead + ulLen;				
			}
			else
			{
				// copy data From the Tail of Buffer
				for(ulLoop = 0 ; ulLoop < ulUsedBack ; ulLoop++)
				{
					pbData[ulLoop] = pBuffer->uBody.p8Buffer[pBuffer->ulHead + ulLoop];
				}
				// copy data From the Start of Buffer
				for(ulLoop = 0 ; ulLoop < ulLen - ulUsedBack ; ulLoop++)
				{
					pbData[ulUsedBack + ulLoop]	= pBuffer->uBody.p8Buffer[ulLoop];
				}
				pBuffer->ulHead			= ulLen - ulUsedBack;				
			}

			// Move Head Position
			pBuffer->ulUnusedSize	= pBuffer->ulUnusedSize + ulLen;
			pBuffer->ulUsedSize		= pBuffer->ulUsedSize - ulLen;

		}
//		pBuffer->ulHead = pBuffer->ulHead % pBuffer->ulSize;

		STI();

	}

	return eResult;

}

QRESULT QFlush(ST_BUFFER_TYPE *pBuffer)
{
//	if(UARTHAL_BUFFER_SIZE - 
//	ST_BUFFER_TYPE *pBuffer = &t_stUartTxBuffer;
	QRESULT		eResult = NO_QERROR;
	uint32_t	ulLoop;

	pBuffer->ulHead = 0;
	pBuffer->ulTail = 0;
	pBuffer->ulUsedSize = 0;
	pBuffer->ulUnusedSize = pBuffer->ulSize;
	
	return eResult;

}


QRESULT QRollBack(ST_BUFFER_TYPE *pBuffer)
{
//	if(UARTHAL_BUFFER_SIZE - 
//	ST_BUFFER_TYPE *pBuffer = &t_stUartTxBuffer;
	QRESULT		eResult = NO_QERROR;

	CLI();

	if(pBuffer->ulUsedSize > 0)
	{

		// Move Head Position
		if(pBuffer->ulTail == 0)
		{
			pBuffer->ulTail = pBuffer->ulSize - 1;
		}
		else
		{
			pBuffer->ulTail--;
		}
		
		pBuffer->ulUnusedSize++;
		pBuffer->ulUsedSize--;
	}
	else
	{
		eResult = EMPTY_QERROR;
	}


	STI();

	return eResult;

}




