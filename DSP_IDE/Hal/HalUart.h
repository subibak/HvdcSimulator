
/**
 * HalUart.h
 * HalUart Device Driver Module Definition
 */

#ifndef _HALUART_H_
#define _HALUART_H_

#include "HalIfDef.h"

#include "Core/Queue.h"
#include "HwDef/Uart.h"

/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{
#define HALUART_BUFFER_SIZE	400//256	///< Hal Layer Tx/Rx Buffer size


#define UART_TX_SIZE	10

///@}

/** @name HALUART Error Cause
 * HALUART Error
 */
///@{
#if 0
typedef enum {
	 HALUART_NOERR
	,HALUART_TXBUFFER_FULL
	,HALUART_RXBUFFER_FULL

	,HALUART_ERR_NUM

} HALUART_ERROR;
#endif
///@}







/** @name Type Definition
 * Type Definition
 */
///@{
//struct stUartBufferType			///< Buffer Type 정의 
//{
//	uint32_t ulHead;
//	uint32_t ulTail;
//	uint32_t ulSize;
//	uint32_t ulUnusedSize;
//	uint32_t ulUsedSize;
//	uint8_t aBuffer[HALUART_BUFFER_SIZE];
//};
//typedef struct stUartBufferType ST_BUFFER_TYPE ;
///@}

/**
 * @brief	초기화 함수
 * @details	Hal 기능을 초기화 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
HAL_ERROR HalUartInit(void);

/**
 * @brief	Uart 수신 값을 읽는 함수
 * @details	Uart 수신 버퍼에 남아 있는 데이터를 읽는다. 
 * @param	psData : 데이터 반환을 위한 버퍼 포인터
 * @param	ulLen  : 반환가능한 최대 데이터 양
 * @return	uint32_t : API Error Cause
 * @date	2018-11-24
 * @version	0.0.1
 */
HAL_ERROR HalUartRead(uint8_t *pbData, uint32_t ulLen);

/**
 * @brief	Uart 데이터의 송신을 요청하는 함수
 * @details	Uart 송신 버퍼에 데이터를 전달한다.
 * @param	psData : 송신 요청할 데이터 포인터
 * @param	ulLen  : 송신 요청한 데이터의 양
 * @return	채널값
 * @date	2018-11-24
 * @version	0.0.1
 */
HAL_ERROR HalUartWrite(uint8_t *pbData, uint32_t ulLen);

HAL_ERROR HalUartSyncWrite(uint8_t *pbData, uint32_t ulLen);

/**
 * @brief	Uart Tx,Rx 버퍼를 동시 처리하는 함수
 * @details	Rx, Tx FIFO 를 확인하고 가능한 만큼 데이터를 처리한다. 
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
//uint32_t HalUartHandle(ST_BUFFER_TYPE *psTxData, ST_BUFFER_TYPE *psRxData);
HAL_ERROR HalUartHandle();//ST_BUFFER_TYPE *psTxData, ST_BUFFER_TYPE *psRxData)

HAL_ERROR HalUartGetTxUnusedSize(uint32_t *ulSize);



#endif
