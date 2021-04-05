

/**
 * ControlApp.h
 * ControlApp Main Module Definition
 */

#ifndef _CONTROLAPP_H_
#define _CONTROLAPP_H_

#include "Ial/IalControl/IalControl.h"
#include "MesApp.h"
#include "CcsApp.h"
#include "VbcApp.h"


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

void ControlAppIOHandle(void);


/**
 * @brief	App 초기화 함수
 * @details App 기능을 초기화 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void ControlAppInit(void);

/**
 * @brief	App Main Run 함수
 * @details App Main Function을 수행한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void ControlAppRun(void);

#endif
