

/**
 * IalHmi.h
 * IalHmi Interface Module Definition
 */

#ifndef _IALHMI_H_
#define _IALHMI_H_

#include "IalIfDef.h"

#include "Hal/HalVme.h"


/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{
//#define SIGNAL_NONE				0x0			///< No Signal

#define	IALHMI_PROC_UNIT		0x1
#define IALHMI_INTERFACE_LEN	128

///@}


//#define SIGNAL_REQUEST_BUS	0x8


/**
 * @brief	Ial Moduel �� �ʱ�ȭ�ϴ� �Լ� 
 * @details	Ial ���� �� ���� Hal�� �ʱ�ȭ �Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void IalHmiInit(void);

void IalHmiUpdate(void);


#endif


