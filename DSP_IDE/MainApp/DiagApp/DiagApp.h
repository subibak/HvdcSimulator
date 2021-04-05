

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
 * @brief	App �ʱ�ȭ �Լ�
 * @details App ����� �ʱ�ȭ �Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void DiagAppInit(void);

/**
 * @brief	App Main Run �Լ�
 * @details App Main Function�� �����Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void DiagAppRun(uint32_t ulState);

#endif
