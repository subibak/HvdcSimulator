

/**
 * IalControl.h
 * IalControl Interface Module Definition
 */

#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "Core/Core.h"
/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{

#define PROTOCOL_STARTCODE_IDX	0x0
#define PROTOCOL_CRC_IDX		0x1
#define PROTOCOL_SEQ_IDX		0x2
#define PROTOCOL_LEN_IDX		0x3
//#define PROTOCOL_CRC_IDX	0x0
//#define PROTOCOL_SEQ_IDX	0x1
#define PROTOCOL_BODY_IDX	0x4

#define PROTOCOL_HEADER_LEN	4

//#define PROTOCOL_STARTCODE	0xC1C2C3C4
#define PROTOCOL_STARTCODE	0xAABBCCDD

#define PROTOCOL_SEQ_MIN	0x0
#define PROTOCOL_SEQ_MAX	0xFFFFFFFF
#define PROTOCOL_SEQ_MASK	0xFFFFFFFF

#define PROTOCOL_LEN_MASK	0xFFFFFFFF
//#define PROTOCOL_LEN_SHIFT	0x10


#define PROTOCOL_OK			0x0

#define PROTOCOL_LINK		0x1
#define PROTOCOL_SEQ_SAME	0x2  // Data Missing 으로 동일 Sequence Number, Start Code Error 를 모두 포함한다.
#define PROTOCOL_CRC_ERROR	0x4

#define LastRcvSeq(ulChNum)	ulRxSeqNum[ulChNum]

#define MAX_CHANNEL_NUM	10



///@}


//#define SIGNAL_REQUEST_BUS	0x8


/**
 * @brief	Ial Moduel 을 초기화하는 함수 
 * @details	Ial 변수 및 관련 Hal의 초기화 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */



void ProtocolInit(void);

void ProtocolEncode(uint32_t ulChNum, uint32_t *pulData, uint32_t ulLen);

uint32_t ProtocolDecode(uint32_t ulChNum, uint32_t *pulData, uint32_t ulLen);

uint32_t GetChSeqNum(uint32_t ulChNum);

extern uint32_t ulTxSeqNum[MAX_CHANNEL_NUM];
extern uint32_t ulRxSeqNum[MAX_CHANNEL_NUM];


#endif


