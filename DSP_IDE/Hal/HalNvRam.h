
/**
 * HalNvRam.h
 * HalNvRam Device Driver Module Definition
 */

#ifndef _HALNVRAM_H_
#define _HALNVRAM_H_

#include "HalIfDef.h"

#include "HwDef/NvRam.h"
//#include "Core/Queue.h"

/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{
#define HALNVRAM_DEFAULT_LENGTH		10
#define HALNVRAM_DATA_MASK			0xFFFF
///@}

/** @name HALVME Error Cause
 * HALVME Error
 */
///@{

///@}







/** @name Type Definition
 * Type Definition
 */
///@{


///@}

/**
 * @brief	초기화 함수
 * @details	Hal 기능을 초기화 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
HAL_ERROR HalNvRamInit(void);

/**
 * @brief	NvRam 데이터의 송신을 요청하는 함수
 * @details	NvRam 송신 버퍼에 데이터를 전달한다.
 * @param	psData	: 송신 요청할 데이터 포인터
 * @param	ulOffset: 주소 Offset
 * @param	ulLen	: 송신 요청한 데이터의 양
 * @param	Error 여부 
 * @date	2018-11-24
 * @version	0.0.1
 */
HAL_ERROR HalNvRamWrite(uint32_t *psData, uint32_t ulOffset, uint32_t uslLen);

/**
 * @brief	NvRam 수신 값을 읽는 함수
 * @details	NvRam 수신 버퍼에 남아 있는 데이터를 읽는다. 
 * @param	psData : 데이터 반환을 위한 버퍼 포인터
 * @param	ulOffset: 주소 Offset
 * @param	Error 여부 
 * @return	uint32_t : API Error Cause
 * @date	2018-11-24
 * @version	0.0.1
 */
//HAL_ERROR HalNvRamRead(uint8_t *psData, uint32_t ulOffset, uint32_t ulLen);
HAL_ERROR HalNvRamRead(uint32_t *psData, uint32_t ulOffset, uint32_t ulLen);

/**
 * @brief	NvRam Tx,Rx 버퍼를 동시 처리하는 함수
 * @details	Rx, Tx FIFO 를 확인하고 가능한 만큼 데이터를 처리한다. 
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
HAL_ERROR HalNvRamHandle(ST_BUFFER_TYPE *psTxData, ST_BUFFER_TYPE *psRxData);




#endif
