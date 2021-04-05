

/**
 * VbcApp.h
 * VbcApp Main Module Definition
 */

#ifndef _VBCLAPP_H_
#define _VBCLAPP_H_

#include "Core/Core.h"
#include "Ial/IalControl/IalVbc.h"


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

void SetVbcInputBuffer(uint32_t *pfInput, uint32_t ulInputSize);
void SetVbcOutputBuffer(uint32_t *pfOutput, uint32_t ulOutputSize);

void VbcAppIOHandle(void);


/**
 * @brief	App �ʱ�ȭ �Լ�
 * @details App ����� �ʱ�ȭ �Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void VbcAppInit(void);

/**
 * @brief	App Main Run �Լ�
 * @details App Main Function�� �����Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void VbcAppRun(void);

#endif
