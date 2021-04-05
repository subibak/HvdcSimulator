
/**
 * HwIfDef.h
 * HwIfDef Interface Module Definition
 */

#ifndef _HWIFDEF_H_
#define _HWIFDEF_H_

#include "Core/Core.h"

// Memory Map �� ���� ������ ���� ����.
#define XCA_BASE_ADDR	0x95000000
#define XCB_BASE_ADDR	0x80000000
#define XCC_BASE_ADDR	0xC0000000


#define XCA_WDT_STAT_REG_ADDR	(XCA_BASE_ADDR+4)
#define XCA_FAIL_REG_ADDR		(XCA_BASE_ADDR+7)

#define XCA_VEND_ID_RO			(XCA_BASE_ADDR+0x8)
#define XCB_VEND_ID_RO			(XCB_BASE_ADDR+0x10)

#define XCA_SYS_CLK_RO			(XCA_BASE_ADDR+0x0C)
#define XCB_SYS_CLK_RO			(XCB_BASE_ADDR+20)

#define WDT_ENABLE	0x1

#define		FOREVER				while(1)



/** @name Macro Definition
 * Macro Definition
 */
///@{

///< ������Ʈ ���� �Լ�
#define WriteReg(pAddr, ulData)			*((volatile uint32_t *)(pAddr)) = ulData

///< ������Ʈ �б� �Լ�
#define ReadReg(pAddr, pulData)			(*pulData) = *((volatile uint32_t *)(pAddr))

///< ������Ʈ ���� �Լ�
#define BitSet(ulData, ulMask)			ulData = (ulData) | (ulMask)

///< ������Ʈ �б� �Լ�
#define BitClear(ulData, ulMask)		ulData = (ulData) & ~(ulMask)

///@}

#endif


