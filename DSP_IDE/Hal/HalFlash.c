


#include "HalFlash.h"
#include "HalUart.h"

uint32_t	ulFlashBaseAddr	= HWREG_FLASH_BASE_ADDR;
uint32_t			ulFlashSize		= HWREG_FLASH_SIZE;
uint32_t			ulTimeOut		= 0;				// ms

void HalFlashInit(void)
{
	ulFlashBaseAddr	= HWREG_FLASH_BASE_ADDR;
	ulFlashSize		= HWREG_FLASH_SIZE;

	HalFlashWpEnable();
}



// Sector Number: 0 ~ 7
HAL_ERROR HalFlashEraseSector(uint32_t	ulSectorNum)
{

	uint8_t	*pulAddr;
	HAL_ERROR	eResult = HAL_NOERR;

	if( ulSectorNum < HALFLASH_STCTOR_NUM )
	{

		// Erase Sector

		pulAddr = (uint8_t	*)(ulFlashBaseAddr + (HWREG_FLASH_SECTOR_SIZE * ulSectorNum));
	 			
		pulAddr[HWREG_FLASH_RA1_OFFSET]	= 0xAA;
		pulAddr[HWREG_FLASH_RA2_OFFSET]	= 0x55;
		pulAddr[HWREG_FLASH_RA1_OFFSET]	= 0x80;
		pulAddr[HWREG_FLASH_RA1_OFFSET]	= 0xAA;
		pulAddr[HWREG_FLASH_RA2_OFFSET]	= 0x55;
		pulAddr[HWREG_FLASH_SA_OFFSET]	= 0x30;

		DelayUs(1000);
		ulTimeOut = 0;

		// Wait Erase Ack
		while ((pulAddr[HWREG_FLASH_SA_OFFSET] & 0x80) != 0x80) 
		{
			DelayUs(1000);
			ulTimeOut++;

			if(ulTimeOut > HALFLASH_TIMEOUT)
			{
				// Error
				ulTimeOut = 0;
				eResult = HALFLASH_ERASE_ERR;
				break;
			}

		}
			
		/* reset sector to read mode */
		pulAddr[HWREG_FLASH_RA1_OFFSET] = 0xAA;
		pulAddr[HWREG_FLASH_RA2_OFFSET] = 0x55;
		pulAddr[HWREG_FLASH_RA1_OFFSET] = 0xF0;
		
	}
	else
	{
		// Sector error
		eResult = HALFLASH_ERASE_ERR;
	}

	return eResult;
	
}


// Sector Number: 0 ~ 7
HAL_ERROR HalFlashEraseChip(void)
{

	uint8_t	*pulAddr;

	// Erase Sector
	pulAddr = (uint8_t	*)(ulFlashBaseAddr);
	 			
	pulAddr[HWREG_FLASH_RA1_OFFSET]	= 0xAA;
	pulAddr[HWREG_FLASH_RA2_OFFSET]	= 0x55;
	pulAddr[HWREG_FLASH_RA1_OFFSET]	= 0x80;
	pulAddr[HWREG_FLASH_RA1_OFFSET]	= 0xAA;
	pulAddr[HWREG_FLASH_RA2_OFFSET]	= 0x55;
	pulAddr[HWREG_FLASH_RA1_OFFSET]	= 0x10;

	DelayUs(1000);
	ulTimeOut = 0;

	// Wait Erase Ack
	while ((pulAddr[HWREG_FLASH_SA_OFFSET] & 0x80) != 0x80) 
	{
		DelayUs(1000);
		ulTimeOut++;

		if(ulTimeOut > HALFLASH_TIMEOUT)
		{
			// Error
			ulTimeOut = 0;
			return HALFLASH_ERASE_ERR;
		}

	}
			
	/* reset sector to read mode */
	pulAddr[HWREG_FLASH_RA1_OFFSET] = 0xAA;
	pulAddr[HWREG_FLASH_RA2_OFFSET] = 0x55;
	pulAddr[HWREG_FLASH_RA1_OFFSET] = 0xF0;

	DelayUs(5000);

	return HAL_NOERR;
		
}


HAL_ERROR HalFlasSwUpgrade(uint32_t	*pulData, uint32_t ulLen)
{

	uint32_t	ulLoop;
	uint32_t	ulData;
	uint32_t	ulTimeoutCnt;

//	HalUartWrite((uint8_t *)"\r\n", 2);

	for(ulLoop = 0; ulLoop < ulLen; ulLoop++)
	{
		WriteReg((ulFlashBaseAddr + HWREG_FLASH_RA1_OFFSET), 0xAA);
		WriteReg((ulFlashBaseAddr + HWREG_FLASH_RA2_OFFSET), 0x55);
		WriteReg((ulFlashBaseAddr + HWREG_FLASH_RA1_OFFSET), 0xA0);

		WriteReg((ulFlashBaseAddr + ulLoop), (*(pulData + ulLoop))&0xFF);

		ulTimeoutCnt = 0;

		do
		{
			ReadReg((ulFlashBaseAddr + ulLoop), &ulData);
			DelayUs(100);
			ulTimeoutCnt++;
			if(ulTimeoutCnt > 100)
			{
				// Error
				HalUartWrite((uint8_t *)"\r\nUpdate Fail!\n\r", 16);
				return HALFLASH_WRITE_ERR;
			}

		} while((ulData&0xFF) != ((*(pulData + ulLoop))&0xFF) );

		if((ulLoop % 10000) == 0)
		{
			HalUartWrite((uint8_t *)".", 1);
		}

	}



	HalUartWrite((uint8_t *)"\r\nUpdate Done!\n\r", 16);

	return HAL_NOERR;


}

HAL_ERROR HalFlashWriteBuffer(uint32_t ulAddr, uint32_t	*pulData, uint32_t ulLen)
{

	uint32_t	ulLoop;
	uint32_t	ulData;
	uint32_t	ulTimeoutCnt;

	for(ulLoop = 0; ulLoop < ulLen; ulLoop++)
	{
		WriteReg((ulAddr + HWREG_FLASH_RA1_OFFSET), 0xAA);
		WriteReg((ulAddr + HWREG_FLASH_RA2_OFFSET), 0x55);
		WriteReg((ulAddr + HWREG_FLASH_RA1_OFFSET), 0xA0);

		WriteReg((ulAddr + ulLoop), (*(pulData + ulLoop))&0xFF);

		ulTimeoutCnt = 0;

		do
		{
			ReadReg((ulAddr + ulLoop), &ulData);
			DelayUs(100);
			ulTimeoutCnt++;
			if(ulTimeoutCnt > 100)
			{
				// Error
				HalUartWrite((uint8_t *)"\r\nWrite Error!\n\r", 16);
				return HALFLASH_WRITE_ERR;
				
			}

		} while((ulData&0xFF) != ((*(pulData + ulLoop))&0xFF) );



	}

	return HAL_NOERR;


}


void HalFlashWpEnable(void)
{
	uint32_t	ulRegValue;

	ReadReg(HWREG_FLASH_WP_ADDR, &ulRegValue);
	BitSet(ulRegValue, HWREG_FLASH_WP_MASK);
	WriteReg(HWREG_FLASH_WP_ADDR, ulRegValue);
	DelayUs(100);
}

void HalFlashWpDisable(void)
{
	uint32_t	ulRegValue;

	ReadReg(HWREG_FLASH_WP_ADDR, &ulRegValue);
	BitClear(ulRegValue, HWREG_FLASH_WP_MASK);
	WriteReg(HWREG_FLASH_WP_ADDR, ulRegValue);
	DelayUs(100);

}

