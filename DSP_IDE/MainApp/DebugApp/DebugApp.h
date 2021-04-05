

/**
 * DebugApp.h
 * DebugApp Main Module Implementation
 */

#ifndef _DEBUGAPP_H_
#define _DEBUGAPP_H_



#include "Ial/IalDebug.h"

#include "DebugCmdTable.h"
#include "DebugCmdParser.h"
#include "DebugCmdExe.h"



#if 0
#include "Hal/HalUart.h"
#include "Hal/HalAi.h"
#include "Hal/HalAo.h"
#include "Hal/HalDio.h"
#include "Hal/HalFComm.h"
#include "Hal/HalNvRam.h"
#include "Hal/HalTimer.h"
#include "Hal/HalVme.h"
#endif



/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{
#define DEBUGAPP_STATE_READY	0x0
#define DEBUGAPP_STATE_PARSE	0x1
#define DEBUGAPP_STATE_SEARCH	0x2
#define DEBUGAPP_STATE_EXECUTE	0x3
#define DEBUGAPP_STATE_FINAL	0x4


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
void DebugAppInit(void);

/**
 * @brief	App Main Run �Լ�
 * @details App Main Function�� �����Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void DebugAppRun(void);

#endif
