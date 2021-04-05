
/**
 * HalFcomm.h
 * HalFcomm Device Driver Module Definition
 */

#ifndef _HALFCOMM_H_
#define _HALFCOMM_H_

#include "HalIfDef.h"

#include "Core/Queue.h"

#include "HwDef/FComm.h"

#include "Protocol/Protocol.h"


/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{

#define HALFCOMM_BASE_SIZE		0x10
#define HALFCOMM_BUFFER_SIZE	128	///< Hal Layer Tx/Rx Buffer size

#define HALFCOMM_LARGFRAME_NUM	(128/HALFCOMM_BASE_SIZE-1)	///< Hal Layer Tx/Rx Buffer size
#define HALFCOMM_SMALFRAME_NUM	(32/HALFCOMM_BASE_SIZE-1)	///< Hal Layer Tx/Rx Buffer size

#define HALFCOMM_DISABLE		0x0
#define HALFCOMM_ENABLE			0x1

#define HALFCOMM_RESET			0x1
#define HALFCOMM_SET			0x0


#define HALFCOMM_ERR_NONE		0x0
#define HALFCOMM_ERR_CRC		0x1
#define HALFCOMM_ERR_SEQNUM		0x2
#define HALFCOMM_ERR_NOPKT		0x8

#define HALFCOMM_ERRCNT_MAX		0x5

///@}

/** @name HALFCOMM Error Cause
 * HALFCOMM Error
 */
///@{

///@}


/** @name HALFCOMM Error Cause
 * HALFCOMM Error
 */
///@{
typedef enum {
	
	 HALFCOMM_CH_1
	,HALFCOMM_CH_2
	,HALFCOMM_CH_3
	,HALFCOMM_CH_4
	,HALFCOMM_CH_NUM

} HALFCOMM_CH;
///@}







/** @name Type Definition
 * Type Definition
 */
///@{
struct stFcommBufferType						///< Buffer Type 정의 
{
	uint32_t ulChannel;							///< Channel Enum
	uint32_t ulHead;							///< Channel Buffer Q Head
	uint32_t ulTail;							///< Channel Buffer Q Head
	uint32_t ulSize;							///< Channel Buffer Fixed Length
	uint32_t ulUnusedSize;						///< Channel Buffer Q Head
	uint32_t ulUsedSize;						///< Channel Buffer Q Head
	uint8_t aBuffer[HALFCOMM_BUFFER_SIZE];			///< Channel Buffer
};
typedef struct stFcommBufferType ST_FCOMM_BUFFER_TYPE ;

struct stFcommType								///< FComm Type 정의 
{
	uint32_t		ulEnable;					///< Channel Enable
	uint32_t		ulSize;						///< Channel Buffer Fixed Length
	uint32_t		ulErrCause;

	ST_BUFFER_TYPE	stBuffer;
};
typedef struct stFcommType 	ST_FCOMM_TYPE ;



///@}

/**
 * @brief	초기화 함수
 * @details	Hal 기능을 초기화 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
HAL_ERROR HalFCommInit(void);

/**
 * @brief	Fiber Comm 수신 값을 읽는 함수
 * @details	Fiber Comm 수신 버퍼에 남아 있는 데이터를 읽는다. 
 * @param	ulChNum	: 채널 인덱스
 * @param	psData : 데이터 반환을 위한 버퍼 포인터
 * @param	ulLen  : 반환가능한 최대 데이터 양
 * @return	uint32_t : API Error Cause
 * @date	2018-11-24
 * @version	0.0.1
 */
HAL_ERROR HalFCommRead(uint32_t ulChNum, uint32_t	*pulErr);

/**
 * @brief	Fiber Comm 데이터의 송신을 요청하는 함수
 * @details	Fiber Comm 송신 버퍼에 데이터를 전달한다.
 * @param	ulChNum	: 채널 인덱스
 * @param	psData : 송신 요청할 데이터 포인터
 * @param	ulLen  : 송신 요청한 데이터의 양
 * @return	채널값
 * @date	2018-11-24
 * @version	0.0.1
 */
//HAL_ERROR HalFCommWrite(uint32_t ulChNum, ST_FCOMM_TYPE *psData);
//HAL_ERROR HalFCommWrite(uint32_t ulChNum, uint32_t *psData);
HAL_ERROR HalFCommWrite(uint32_t ulChNum);
//HAL_ERROR HalFCommWrite(uint32_t ulChNum, uint32_t pulData[], uint32_t ulLen);

/**
 * @brief	Fiber Comm Tx,Rx 버퍼를 동시 처리하는 함수
 * @details	Full Length Buffer 를 가지고 있으며, 채널을 구분하여 처리한다.
 * @param	ulChNum	: 채널 인덱스
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
uint32_t HalFCommHandle(uint32_t ulChNum);



/**
 * @brief	Fiber Comm Tx,Rx 버퍼를 동시 처리하는 함수
 * @details	Full Length Buffer 를 가지고 있으며, 채널을 구분하여 처리한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
uint32_t HalFCommSet(uint32_t ulChNum, uint32_t ulEnable, uint32_t ulSize);

HAL_ERROR HalFCommSetBuffer(uint32_t ulChNum, uint32_t *pulTxBuffer, uint32_t *pulRxBuffer);
HAL_ERROR HalFCommSetFrameSize(uint32_t ulChNum, uint32_t ulTxSize, uint32_t ulRxSize);

HAL_ERROR HalFCommReadSync(uint32_t	*pulOldSeq);
HAL_ERROR HalFCommEna(uint32_t ulChNum, uint32_t ulEnable);
HAL_ERROR HalFCommStsCheck(uint32_t ulChNum);
HAL_ERROR HalFCommSetSize(uint32_t ulChNum, uint32_t	ulSize);



/* Write Data to Tx Buffer */
HAL_ERROR HalFCommWriteNoEnc(uint32_t ulChNum);

/* Read Data from Rx Buffer until '\r\n'*/
HAL_ERROR HalFCommRead(uint32_t ulChNum, uint32_t	*pulErr);

/* Read Data from Rx Buffer until '\r\n'*/
HAL_ERROR HalFCommReadNoDec(uint32_t ulChNum, uint32_t	*pulErr);

#endif
