
#ifndef _HALFLASH_H_
#define _HALFLASH_H_

#include "HalIfDef.h"

#include "HwDef/Flash.h"




#define HALFLASH_STCTOR_START	0x0
#define HALFLASH_STCTOR_END		(HWREG_FLASH_SIZE/HWREG_FLASH_SECTOR_SIZE - 1)
#define HALFLASH_STCTOR_NUM		(HWREG_FLASH_SIZE/HWREG_FLASH_SECTOR_SIZE)

#define HALFLASH_TIMEOUT		10000



void HalFlashInit(void);

HAL_ERROR HalFlashEraseSector(uint32_t	ulSectorNum);

// Sector Number: 0 ~ 7
HAL_ERROR HalFlashEraseChip(void);


HAL_ERROR HalFlasSwUpgrade(uint32_t	*pulData, uint32_t ulLen);

HAL_ERROR HalFlashWriteBuffer(uint32_t ulAddr, uint32_t	*pulData, uint32_t ulLen);


void HalFlashWpEnable(void);

void HalFlashWpDisable(void);







#endif



