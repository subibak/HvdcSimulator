/*
 ***************************************************************************
 * Copyright (c) LK Eleven Co., Ltd. 2008~2013 All Rights Reserved.
 * Design Units  : NCS-1000
 * Compile Tools : VisualDSP++ 5.0
 ***************************************************************************
 */
#ifndef _CRC_H_
#define _CRC_H_


#include "Core.h"



#define CRC_OK		0x1
#define CRC_ERROR	0x0


//extern	uint16_t	Crc16Ccitt(char *buf, uint len);
//extern	int		CheckCrc16 (int crc, uchar *buf, int sz);
//extern	void	MakeCrc(uchar *buf, uint len);
//extern	STATUS	CheckCrc(uchar *buf, uint len);
//extern	uint32_t	MakeCrc32(uchar *buf, int len);




//extern	uint16_t	crc16_ccitt(uint16_t *buf, uint32_t len);
//extern	int32_t		check_crc16 (int32_t crc, uchar *buf, int32_t sz);
//extern	void	make_crc(uint8_t *buf, uint32_t len);
//extern	STATUS	check_crc(uint8_t *buf, uint32_t len);
//extern	uint	make_crc32(uint8_t *buf, int32_t len);




/* 송신할 데이터의 CRC를 만든다.
// Return - CRC 값
*/

/**
 * @brief	Uart 데이터의 송신을 요청하는 함수
 * @details	Uart 송신 버퍼에 데이터를 전달한다.
 * @param	psData : 송신 요청할 데이터 포인터
 * @param	ulLen  : 송신 요청한 데이터의 양
 * @return	채널값
 * @date	2018-11-24
 * @version	0.0.1
 */
uint16_t Crc16Ccitt(uint8_t *bBuf, uint32_t ulLen);

/**
 * @brief	16비트 Crc값을 계산하는 함수.
 * @details	주어진 데이터를 사용하여 Crc 데이터를 계산한다.
 * @param	pBuff : 데이터 스트림
 * @param	ulLen  : 데이터 스르림 길이
 * @return	Crc값
 * @date	2018-11-24
 * @version	0.0.1
 */
uint16_t MakeCrc16(uint16_t *bBuf, uint32_t ulLen);



// 수신한 데이터의 CRC를 읽어서 데이터가 정확한지 검사한다.
// Return 0 : Fail
// Return 1 : Success
int32_t CheckCrc16(uint32_t ulCrc, uint8_t *bBuf, int32_t sz);

uint32_t MakeCrc32(const uint32_t *pulBuf, int len);

uint32_t CheckCrc32(uint16_t *bBuf, uint32_t ulLen, uint32_t ulRcvCrc);

#endif

