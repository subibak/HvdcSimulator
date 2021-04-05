

/**
 * HmiApp.h
 * HmiApp Main Module Definition
 */

#ifndef _HMIAPP_H_
#define _HMIAPP_H_

#include "Ial/IalHmi.h"


/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{

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
void HmiAppInit(void);

/**
 * @brief	App Main Run 함수
 * @details App Main Function을 수행한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HmiAppRun(void);

#endif
