

/**
 * ConfigApp.h
 * ConfigApp Main Module Definition
 */

#ifndef _CONFIGAPP_H_
#define _CONFIGAPP_H_

#include "Ial/IalConfig.h"


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
void ConfigAppInit(void (*func)(int));
/**
 * @brief	App Main Run 함수
 * @details App Main Function을 수행한다.
 * 			주기적인 기능의 수행이 필요하지 않은 경우, 생략가능 
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void ConfigAppRun(void);

#endif
