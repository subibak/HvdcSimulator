

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
 * @brief	App 초기화 함수
 * @details App 기능을 초기화 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void CcsAppInit(void);

/**
 * @brief	App Main Run 함수
 * @details App Main Function을 수행한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void CcsAppRun(void);

#endif
