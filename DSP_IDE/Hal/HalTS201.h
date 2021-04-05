
/**
 * HalTS201.c
 * HalTS201 Device Driver Module Definition
 */


#ifndef _HALTS201_H_
#define _HALTS201_H_

#include "HalIfDef.h"

/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{
#define FLAG1_OUTPUT 0x1	///< Flag1 Output
#define FLAG2_OUTPUT 0x2	///< Flag2 Output
#define FLAG3_OUTPUT 0x4	///< Flag3 Output
#define FLAG4_OUTPUT 0x8	///< Flag4 Output


#define SDRAM_DISABLE 0		///< SDRAM Disable Configure
#define SDRAM_ENABLE 1		///< SDRAM Enable Configure
///@}


/**
 * @brief	TigerSharc Module �ʱ�ȭ �Լ� 
 * @details	TigerSharc Module�� �ʱ�ȭ �Ѵ�. (Cache, PLL ��)
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void InitTS201(void);

/**
 * @brief	Global Interrupt ���� Clear �Լ� 
 * @details	Global Interrupt �� Clear �Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void CLI(void);

/**
 * @brief	Global Interrupt ���� Set �Լ� 
 * @details	Global Interrupt �� Set �Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void STI(void);


void WdtEnable(void);

void WdtDisable(void);

void HalTS0201MPEnable(void);

void Reboot(int time, int pwr_mode);

void HalTS201ExtInt(void);

void SystemHealth(void);


#endif
