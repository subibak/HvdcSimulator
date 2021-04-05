

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
 * @brief	Ial Moduel �� �ʱ�ȭ�ϴ� �Լ� 
 * @details	Ial ���� �� ���� Hal�� �ʱ�ȭ �Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void IalDebugInit(void);

/**
 * @brief	Console ä���� �Է��� Cmd ������ �д´�.
 * @details	Console ���� ���ۿ� ���� �ִ� �����͸� �д´�. 
 * @param	psData : ������ ��ȯ�� ���� ���� ������
 * @param	ulLen  : ��ȯ������ �ִ� ������ ��
 * @return	uint32_t : API Error Cause
 * @date	2018-11-24
 * @version	0.0.1
 */
IALDEBUG_RESULT IalDebugConsoleRead(uint32_t ulLen);

/**
 * @brief	Console �������� �۽��� ��û�ϴ� �Լ�
 * @details	Console ä�η� �۽� ������ �����͸� �����Ѵ�.
 * @param	psData : �۽� ��û�� ������ ������
 * @param	ulLen  : �۽� ��û�� �������� ��
 * @return	ä�ΰ�
 * @date	2018-11-24
 * @version	0.0.1
 */
IALDEBUG_RESULT IalDebugConsoleWrite(uint32_t ulLen);

/**
 * @brief	Console Tx,Rx ���۸� ���� ó���ϴ� �Լ�
 * @details	Rx, Tx FIFO �� Ȯ���ϰ� ������ ��ŭ �����͸� ó���Ѵ�. 
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
//uint32_t IalDebugHandle(ST_BUFFER_TYPE *psTxData, ST_BUFFER_TYPE *psRxData);
uint32_t IalDebugHandle(void);


/**
 * @brief	Debug ���� ���� �д� �Լ�
 * @details	IalDebug ���� ���ۿ��� �ʿ��� �����͸� �д´�. 
 * @param	pcData : ������ ��ȯ�� ���� ���� ������
 * @param	ulLen  : ��ȯ������ �ִ� ������ ��
 * @return	uint32_t : API Error Cause
 * @date	2018-11-24
 * @version	0.0.1
 */
IALDEBUG_RESULT IalDebugRead(char *pcData, uint32_t *pulLen);

/**
 * @brief	Debug �������� �۽��� ��û�ϴ� �Լ�
 * @details	IalDebug �۽� ���ۿ� �����͸� �����Ѵ�.
 * @param	psData : �۽� ��û�� ������ ������
 * @param	ulLen  : �۽� ��û�� �������� ��
 * @return	ä�ΰ�
 * @date	2018-11-24
 * @version	0.0.1
 */
IALDEBUG_RESULT IalDebugWrite(uint8_t *pbData, uint16_t ulLen);



extern uint32_t ulInboundSts;

extern uint32_t	ulDebugMode;

extern uint32_t	ulIoDelay;


extern uint32_t	ulDbgTblSize;


#endif


