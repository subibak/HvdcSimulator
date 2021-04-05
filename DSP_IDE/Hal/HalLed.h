/**
 * HalLed.h
 * Led Control
 */

#ifndef _HALLED_H_
#define _HALLED_H_

#include "HalIfDef.h"

#include "Hwdef/Led.h"

#define HALLED_ON		0x1
#define HALLED_OFF		0x0

#define HALLED_MODE_AUTO	0x0
#define HALLED_MODE_MANUAL	0x1


#define HALLED_RUNLED	0x1
#define HALLED_VMELED	0x2
#define HALLED_STA1LED	0x3
#define HALLED_STA2LED	0x6
#define HALLED_DSTALED	0x9
#define HALLED_DFAILLED	0xA


#define HALLED_LED_RED		0x1
#define HALLED_LED_GREEN	0x2
#define HALLED_LED_BLUE		0x4




void HalLedModeSet(uint32_t	ulCh, uint32_t ulMode);


void HalLedCtrlSet(uint32_t ulCh, uint32_t ulOnOff, uint32_t ulColor);



#endif








