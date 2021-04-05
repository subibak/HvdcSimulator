

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
 * @brief	App �ʱ�ȭ �Լ�
 * @details App ����� �ʱ�ȭ �Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void ConfigAppInit(void (*func)(int));
/**
 * @brief	App Main Run �Լ�
 * @details App Main Function�� �����Ѵ�.
 * 			�ֱ����� ����� ������ �ʿ����� ���� ���, �������� 
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void ConfigAppRun(void);

#endif
