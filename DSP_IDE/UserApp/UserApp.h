
/**
 * UserApp.h
 * UserApp Module Definition
 */

#ifndef _USERAPP_H_
#define _USERAPP_H_

#include "Core/Core.h"
#include "Ial/IalControl/IalControl.h"



void MesAppDefault(void);



void CcsAppDefault(void);


void VbcAppDefault(void);


#if 0
void VbcLAppDefault(void);
#endif

/**
 * @brief	Default User App Entry 함수 
 * @details	알고리즘 등 사용자 기능의 디폴트 함수를 정의한다. 기능 변경 시 재정의 해서 사용해야 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void UserAppDefault(void);

#endif
