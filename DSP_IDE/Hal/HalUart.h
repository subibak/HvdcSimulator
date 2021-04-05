
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
//struct stUartBufferType			///< Buffer Type ���� 
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
 * @brief	�ʱ�ȭ �Լ�
 * @details	Hal ����� �ʱ�ȭ �Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
HAL_ERROR HalUartInit(void);

/**
 * @brief	Uart ���� ���� �д� �Լ�
 * @details	Uart ���� ���ۿ� ���� �ִ� �����͸� �д´�. 
 * @param	psData : ������ ��ȯ�� ���� ���� ������
 * @param	ulLen  : ��ȯ������ �ִ� ������ ��
 * @return	uint32_t : API Error Cause
 * @date	2018-11-24
 * @version	0.0.1
 */
HAL_ERROR HalUartRead(uint8_t *pbData, uint32_t ulLen);

/**
 * @brief	Uart �������� �۽��� ��û�ϴ� �Լ�
 * @details	Uart �۽� ���ۿ� �����͸� �����Ѵ�.
 * @param	psData : �۽� ��û�� ������ ������
 * @param	ulLen  : �۽� ��û�� �������� ��
 * @return	ä�ΰ�
 * @date	2018-11-24
 * @version	0.0.1
 */
HAL_ERROR HalUartWrite(uint8_t *pbData, uint32_t ulLen);

HAL_ERROR HalUartSyncWrite(uint8_t *pbData, uint32_t ulLen);

/**
 * @brief	Uart Tx,Rx ���۸� ���� ó���ϴ� �Լ�
 * @details	Rx, Tx FIFO �� Ȯ���ϰ� ������ ��ŭ �����͸� ó���Ѵ�. 
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
//uint32_t HalUartHandle(ST_BUFFER_TYPE *psTxData, ST_BUFFER_TYPE *psRxData);
HAL_ERROR HalUartHandle();//ST_BUFFER_TYPE *psTxData, ST_BUFFER_TYPE *psRxData)

HAL_ERROR HalUartGetTxUnusedSize(uint32_t *ulSize);



#endif
