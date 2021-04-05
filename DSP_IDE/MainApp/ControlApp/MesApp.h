

/**
 * MesApp.h
 * MesApp Main Module Definition
 */

#ifndef _MESAPP_H_
#define _MESAPP_H_

#include "Core/Core.h"
#include "Ial/IalControl/IalMes.h"


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

void MesAppIOHandle(void);

void SetMesInputBuffer(uint32_t * pulInput, uint32_t ulInputSize);
void SetMesOutputBuffer(uint32_t * pulOutput, uint32_t ulOutputSize);
//void SetMesOutputBuffer(float * pfOutput, uint32_t ulOutputSize);

/**
 * @brief	App �ʱ�ȭ �Լ�
 * @details App ����� �ʱ�ȭ �Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void MesAppInit(void);

/**
 * @brief	App Main Run �Լ�
 * @details App Main Function�� �����Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void MesAppRun(void);

#endif
