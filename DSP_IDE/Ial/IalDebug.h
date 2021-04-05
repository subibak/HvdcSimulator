

/**
 * IalDebug.h
 * IalDebug Interface Module Definition
 */

#ifndef _IALDEBUG_H_
#define _IALDEBUG_H_


#include "IalIfDef.h"
#include "IalControl/IalControl.h"
#include "IalConfig.h"
#include "IalDiag.h"
#include "IalHmi.h"


#include "Hal/HalUart.h"
//#include "Hal/HalAo.h"



/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{
#define	INBOUND_BUFFER_SIZE		2048//HALUART_BUFFER_SIZE
#define	OUTBOUND_BUFFER_SIZE	2048//HALUART_BUFFER_SIZE
#define INBOUND_CHK_SIZE		1

#define CHAR_CR					0xD
#define CHAR_LF					0xA
#define CHAR_BS					0x8
#define CHAR_COMMA				0x2C
#define CHAR_MINUS				0x2D
#define CHAR_NULL				0x0

#define IALDEBUG_MODE_NORMAL		0x0
#define IALDEBUG_MODE_IOTEST		0x1
#define IALDEBUG_MODE_FCOMM_TEST	0x2
#define IALDEBUG_MODE_VME_TEST		0x3
#define IALDEBUG_MODE_XMODEM		0x4
#define IALDEBUG_MODE_DLOAD			0x5

#define IALDEBUG_MODE_TEST		0x2



///@}

/** @name Debug Line State
 * Debug Line State
 */
///@{
#define	INBOUND_STS_GET		0
#define	INBOUND_STS_LINE	1
///@}





/** @name IALDEBUG Error Cause
 * IALDEBUG Error Cause
 */
///@{
typedef enum {
	 IALDEBUG_NOERR
	,IALDEBUG_NOCMDLINE
	,IALDEBUG_INBOUND_FULL
	,IALDEBUG_OUTBOUND_FULL
	,IALDEBUG_ERR_NUM

} IALDEBUG_RESULT;
///@}


#ifdef FEATURE_DEBUG_VARIABLE

#define FLOAT_TYPE		0x0
#define HEX_TYPE		0x1
#define DECIMAL_TYPE	0x2

typedef struct stDbgValueType{
	char 	*pbName;		/// Mapping Pointer Variable
	void		*pValue;			/// Value for -10V (0x8000) Output
	uint32_t	ulType;			/// Value for +10V (0x7FFF) Output
}ST_DBG_TYPE;

ST_DBG_TYPE	astDbgTbl[];


#endif




/**
 * @brief	Ial Moduel 을 초기화하는 함수 
 * @details	Ial 변수 및 관련 Hal의 초기화 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void IalDebugInit(void);

/**
 * @brief	Console 채널의 입력을 Cmd 단위로 읽는다.
 * @details	Console 수신 버퍼에 남아 있는 데이터를 읽는다. 
 * @param	psData : 데이터 반환을 위한 버퍼 포인터
 * @param	ulLen  : 반환가능한 최대 데이터 양
 * @return	uint32_t : API Error Cause
 * @date	2018-11-24
 * @version	0.0.1
 */
IALDEBUG_RESULT IalDebugConsoleRead(uint32_t ulLen);

/**
 * @brief	Console 데이터의 송신을 요청하는 함수
 * @details	Console 채널로 송신 버퍼의 데이터를 전달한다.
 * @param	psData : 송신 요청할 데이터 포인터
 * @param	ulLen  : 송신 요청한 데이터의 양
 * @return	채널값
 * @date	2018-11-24
 * @version	0.0.1
 */
IALDEBUG_RESULT IalDebugConsoleWrite(uint32_t ulLen);

/**
 * @brief	Console Tx,Rx 버퍼를 동시 처리하는 함수
 * @details	Rx, Tx FIFO 를 확인하고 가능한 만큼 데이터를 처리한다. 
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
//uint32_t IalDebugHandle(ST_BUFFER_TYPE *psTxData, ST_BUFFER_TYPE *psRxData);
uint32_t IalDebugHandle(void);


/**
 * @brief	Debug 수신 값을 읽는 함수
 * @details	IalDebug 수신 버퍼에서 필요한 데이터를 읽는다. 
 * @param	pcData : 데이터 반환을 위한 버퍼 포인터
 * @param	ulLen  : 반환가능한 최대 데이터 양
 * @return	uint32_t : API Error Cause
 * @date	2018-11-24
 * @version	0.0.1
 */
IALDEBUG_RESULT IalDebugRead(char *pcData, uint32_t *pulLen);

/**
 * @brief	Debug 데이터의 송신을 요청하는 함수
 * @details	IalDebug 송신 버퍼에 데이터를 전달한다.
 * @param	psData : 송신 요청할 데이터 포인터
 * @param	ulLen  : 송신 요청한 데이터의 양
 * @return	채널값
 * @date	2018-11-24
 * @version	0.0.1
 */
IALDEBUG_RESULT IalDebugWrite(uint8_t *pbData, uint16_t ulLen);



extern uint32_t ulInboundSts;

extern uint32_t	ulDebugMode;

extern uint32_t	ulIoDelay;


extern uint32_t	ulDbgTblSize;


#endif


