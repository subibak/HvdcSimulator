

/**
 * CcsApp.h
 * CcsApp Main Module Definition
 */

#ifndef _CCSAPP_H_
#define _CCSAPP_H_

#include "Core/Core.h"
#include "Ial/IalControl/IalCcs.h"


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


void SetCcsInputBuffer(uint32_t *pfInput, uint32_t ulInputSize);
void SetCcsOutputBuffer(uint32_t  *pfOutput, uint32_t ulOutputSize);

void CcsAppIOHandle(void);


/**
 * @brief	App �ʱ�ȭ �Լ�
 * @details App ����� �ʱ�ȭ �Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void CcsAppInit(void);

/**
 * @brief	App Main Run �Լ�
 * @details App Main Function�� �����Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void CcsAppRun(void);

#endif
