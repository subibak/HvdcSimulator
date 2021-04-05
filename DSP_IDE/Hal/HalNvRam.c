

/**
 * HalNvRam.c
 * HalNvRam Device Driver Module Implementation
 */


#include "HalNvRam.h"

uint32_t	NvRamBaseAddr;
uint32_t	ulNvRamSize;

HAL_ERROR HalNvRamInit(void)
{
	NvRamBaseAddr	= HWREG_MRAM_BASE;
	ulNvRamSize		= MRAM_SIZE;

	return HAL_NOERR;	
}

HAL_ERROR HalNvRamWpDis(void)
{
	uint32_t	ulReg;

	ReadReg(HWREG_MRAM_WP_ADDR, &ulReg);
	BitClear(ulReg, HWREG_MRAM_WP_MASK);
	WriteReg(HWREG_MRAM_WP_ADDR, ulReg);

	return HAL_NOERR;

}

HAL_ERROR HalNvRamWpEn(void)
{
	uint32_t	ulReg;

	ReadReg(HWREG_MRAM_WP_ADDR, &ulReg);
	BitSet(ulReg, HWREG_MRAM_WP_MASK);
	WriteReg(HWREG_MRAM_WP_ADDR, ulReg);

	return HAL_NOERR;

}

/* Write Data to Tx Buffer */
HAL_ERROR HalNvRamWrite(uint32_t *psData, uint32_t ulOffset, uint32_t uslLen)
{

	uint32_t ulLoop;

	HalNvRamWpDis();

	for(ulLoop = 0; ulLoop < uslLen; ulLoop++)
	{
		WriteReg((NvRamBaseAddr + ulOffset + ulLoop), *(psData + ulLoop));
	}

	HalNvRamWpEn();

	return HAL_NOERR;

}


/* Read Data from Rx Buffer until '\r\n'*/
HAL_ERROR HalNvRamRead(uint32_t *psData, uint32_t ulOffset, uint32_t ulLen)
{
	uint32_t ulLoop;
	uint32_t ulValue;

	for(ulLoop = 0; ulLoop < ulLen; ulLoop++)
	{
		ReadReg(NvRamBaseAddr + ulOffset + ulLoop, &ulValue);
		*(psData + ulLoop) = ulValue;
	}

	return HAL_NOERR;

}

/* Operation between NvRam Buffer and NvRam Hw*/
HAL_ERROR HalNvRamHandle(ST_BUFFER_TYPE *psTxData, ST_BUFFER_TYPE *psRxData)
{
	return HAL_NOERR;
}




