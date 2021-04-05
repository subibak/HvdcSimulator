


#include "HalLed.h"


void HalLedModeSet(uint32_t	ulCh, uint32_t ulMode)
{

	uint32_t	ulReg = 0;
	uint32_t	ulMask = 0;
	uint32_t	ulValue = 0;

	ReadReg(HWREG_LED_MODE_ADDR, &ulReg);

	switch(ulCh)
	{
		case HALLED_RUNLED:
			ulMask = RUN_LED_MASK;
			ulValue = ulMode << RUN_LED_SHIFT;
			break;
		case HALLED_VMELED:
			ulMask = VME_LED_MASK;
			ulValue = ulMode << VME_LED_SHIFT;
			break;
		case HALLED_STA1LED:
			ulMask = STA1_LED_MASK;
			ulValue = (ulMode << STA1_LED_SHIFT) | (ulMode << (STA1_LED_SHIFT+1))|(ulMode << (STA1_LED_SHIFT+2));
			break;
		case HALLED_STA2LED:
			ulMask = STA2_LED_MASK;
			ulValue = (ulMode << STA2_LED_SHIFT) | (ulMode << (STA2_LED_SHIFT+1))|(ulMode << (STA2_LED_SHIFT+2));
			break;
		case HALLED_DSTALED:
			ulMask = STA_LED_MASK;
			ulValue = ulMode << STA_LED_SHIFT;
			break;
		case HALLED_DFAILLED:
			ulMask = FAIL_LED_MASK;
			ulValue = ulMode << FAIL_LED_SHIFT;
			break;
		default:
			ulMask = RUN_LED_MASK;
			ulValue = ulMode << RUN_LED_SHIFT;
			break;
	}

	BitClear(ulReg, ulMask);
	BitSet(ulReg, (ulValue & ulMask));

	WriteReg(HWREG_LED_MODE_ADDR, ulReg);

}


void HalLedCtrlSet(uint32_t ulCh, uint32_t ulOnOff, uint32_t ulColor)
{

	uint32_t	ulReg = 0;
	uint32_t	ulMask = 0;
	uint32_t	ulValue = 0;

	ReadReg(HWREG_LED_CTRL_ADDR, &ulReg);

	switch(ulCh)
	{
		case HALLED_RUNLED:
			ulMask = RUN_LED_MASK;
			ulValue = ulOnOff << RUN_LED_SHIFT;
			break;
		case HALLED_VMELED:
			ulMask = VME_LED_MASK;
			ulValue = ulOnOff << VME_LED_SHIFT;
			break;
		// need to process Color
		case HALLED_STA1LED:
			ulMask = STA1_LED_MASK;
			if(ulOnOff == HALLED_ON)
			{
				ulValue = ulColor << STA1_LED_SHIFT;			
			}
			else
			{
				ulValue = HALLED_OFF;			
			}
			break;
		case HALLED_STA2LED:
			ulMask = STA2_LED_MASK;
			if(ulOnOff == HALLED_ON)
			{
				ulValue = ulColor << STA2_LED_SHIFT;			
			}
			else
			{
				ulValue = HALLED_OFF;			
			}
			break;

		case HALLED_DSTALED:
			ulMask = STA_LED_MASK;
			ulValue = ulOnOff << STA_LED_SHIFT;
			break;
		case HALLED_DFAILLED:
			ulMask = FAIL_LED_MASK;
			ulValue = ulOnOff << FAIL_LED_SHIFT;
			break;
		default:
			ulMask = RUN_LED_MASK;
			ulValue = ulOnOff << RUN_LED_SHIFT;
			break;
	}

	BitClear(ulReg, ulMask);
	BitSet(ulReg, (ulValue & ulMask));

	WriteReg(HWREG_LED_CTRL_ADDR, ulReg);	

}



