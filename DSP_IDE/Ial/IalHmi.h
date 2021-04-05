

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
 * @brief	Ial Moduel 을 초기화하는 함수 
 * @details	Ial 변수 및 관련 Hal의 초기화 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void IalHmiInit(void);

void IalHmiUpdate(void);


#endif


