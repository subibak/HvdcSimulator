

/**
 * DiagApp.h
 * DiagApp Main Module Definition
 */

#ifndef _DIAGAPP_H_
#define _DIAGAPP_H_

#include "Ial/IalDiag.h"


/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{
#define DIAGAPP_FALT_RESET	1234
///@}
 

/** @name Type Definition
 * Type Definition
 */
///@{

///@}


/**
 * @brief	App 초기화 함수
 * @details App 기능을 초기화 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void DiagAppInit(void);

/**
 * @brief	App Main Run 함수
 * @details App Main Function을 수행한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void DiagAppRun(uint32_t ulState);

#endif
