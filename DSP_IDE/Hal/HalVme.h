
/**
 * HalVme.h
 * HalVme Device Driver Module Definition
 */

#ifndef _HALVME_H_
#define _HALVME_H_

#include "HalIfDef.h"

#include "Hwdef/Vme.h"

/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{
#define HALVME_BUFFER_SIZE	0x2000	///< Hal Layer Tx/Rx Buffer size
#define HALVME_TXRX_OFFSET	0x1000	///< Hal Layer Tx/Rx Buffer size

///< Head + Body = 512 
///< Rx: 512, Tx: 512 로 정의함.
#define HALVME_HEAD_SIZE	1
#define HALVME_BODY_SIZE	511

#define HALVME_USERDATA_SIZE	256



///@}

/** @name HALVME Error Cause
 * HALVME Error
 */
///@{
#if 0
typedef enum {
	 HALVME_NOERR
	,HALVME_TXBUFFER_FULL
	,HALVME_RXBUFFER_FULL

	,HALVME_ERR_NUM

} HALVME_ERROR;
#endif
///@}







/** @name Type Definition
 * Type Definition
 */
///@{

typedef union HalVmeBody {
	uint32_t	ulData[HALVME_BODY_SIZE];
	float		fData[HALVME_BODY_SIZE];
} HALVME_BODY_TYPE;


typedef struct HalVmeDataType {
	uint32_t			ulLiveCnt;			///> 
	HALVME_BODY_TYPE	uBody;
} HALVME_DATA_TYPE;


typedef struct {
	uint32_t	ulSlaveReady;
	uint32_t	ulBoardId;
	uint32_t	ulSlaveLiveCnt;
	uint32_t	ulReserved1[5];
	uint32_t	ulMasterInitflag;
	uint32_t	ulMasterLiveCnt;
	uint32_t	ulReserved2[6];
} HALVME_SYSTEM_HEADER;

typedef struct {
	uint32_t	aulRxReserved[HALVME_USERDATA_SIZE];
	uint32_t	aulTxReserved[HALVME_USERDATA_SIZE];
} HALVME_USERDATA_TYPE;


typedef struct {
	uint32_t	ulDiag;
	uint32_t	ulReserved[10];
} HALVME_DIAG_INFO;

///@}

/**
 * @brief	초기화 함수
 * @details	Hal 기능을 초기화 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
HAL_ERROR HalVmeInit(void);

/**
 * @brief	Vme 수신 값을 읽는 함수
 * @details	Vme 수신 버퍼에 남아 있는 데이터를 읽는다. 
 * @param	psData : 데이터 반환을 위한 버퍼 포인터
 * @param	ulLen  : 반환가능한 최대 데이터 양
 * @return	uint32_t : API Error Cause
 * @date	2018-11-24
 * @version	0.0.1
 */
HAL_ERROR HalVmeRead(uint32_t ulOffset, uint32_t *psData, uint32_t ulLen);

/**
 * @brief	Vme 데이터의 송신을 요청하는 함수
 * @details	Vme 송신 버퍼에 데이터를 전달한다.
 * @param	psData : 송신 요청할 데이터 포인터
 * @param	ulLen  : 송신 요청한 데이터의 양
 * @return	채널값
 * @date	2018-11-24
 * @version	0.0.1
 */
//HAL_ERROR HalVmeWrite(ST_BUFFER_TYPE *psData);
HAL_ERROR HalVmeWrite(uint32_t ulOffset, uint32_t *psData, uint32_t ulLen);

/**
 * @brief	Vme Tx,Rx 버퍼를 동시 처리하는 함수
 * @details	Rx, Tx FIFO 를 확인하고 가능한 만큼 데이터를 처리한다. 
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
//uint32_t HalVmeHandle(ST_BUFFER_TYPE *psTxData, ST_BUFFER_TYPE *psRxData);
uint32_t HalVmeHandle(void);


uint32_t HalVmeGetUserBase(void);


#if 1
HAL_ERROR HalVmeWriteUserDate(uint32_t ulOffset, uint32_t psData[], uint32_t ulLen);
HAL_ERROR HalVmeReadUserDate(uint32_t ulOffset, uint32_t psData[], uint32_t ulLen);
#else
HAL_ERROR HalVmeWriteUserDate(uint32_t ulOffset, uint32_t *psData[], uint32_t ulLen);
HAL_ERROR HalVmeReadUserDate(uint32_t ulOffset, uint32_t *psData[], uint32_t ulLen);
#endif

HAL_ERROR HalVmeSetRBuffer(uint32_t *pReadBuffer,uint32_t ulLen);
HAL_ERROR HalVmeSetWBuffer(uint32_t *pWriteBuffer,uint32_t ulLen);



#endif
