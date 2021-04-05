
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
 * @brief	�ʱ�ȭ �Լ�
 * @details	Hal ����� �ʱ�ȭ �Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
HAL_ERROR HalNvRamInit(void);

/**
 * @brief	NvRam �������� �۽��� ��û�ϴ� �Լ�
 * @details	NvRam �۽� ���ۿ� �����͸� �����Ѵ�.
 * @param	psData	: �۽� ��û�� ������ ������
 * @param	ulOffset: �ּ� Offset
 * @param	ulLen	: �۽� ��û�� �������� ��
 * @param	Error ���� 
 * @date	2018-11-24
 * @version	0.0.1
 */
HAL_ERROR HalNvRamWrite(uint32_t *psData, uint32_t ulOffset, uint32_t uslLen);

/**
 * @brief	NvRam ���� ���� �д� �Լ�
 * @details	NvRam ���� ���ۿ� ���� �ִ� �����͸� �д´�. 
 * @param	psData : ������ ��ȯ�� ���� ���� ������
 * @param	ulOffset: �ּ� Offset
 * @param	Error ���� 
 * @return	uint32_t : API Error Cause
 * @date	2018-11-24
 * @version	0.0.1
 */
//HAL_ERROR HalNvRamRead(uint8_t *psData, uint32_t ulOffset, uint32_t ulLen);
HAL_ERROR HalNvRamRead(uint32_t *psData, uint32_t ulOffset, uint32_t ulLen);

/**
 * @brief	NvRam Tx,Rx ���۸� ���� ó���ϴ� �Լ�
 * @details	Rx, Tx FIFO �� Ȯ���ϰ� ������ ��ŭ �����͸� ó���Ѵ�. 
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
HAL_ERROR HalNvRamHandle(ST_BUFFER_TYPE *psTxData, ST_BUFFER_TYPE *psRxData);




#endif
