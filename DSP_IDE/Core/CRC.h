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




/* �۽��� �������� CRC�� �����.
// Return - CRC ��
*/

/**
 * @brief	Uart �������� �۽��� ��û�ϴ� �Լ�
 * @details	Uart �۽� ���ۿ� �����͸� �����Ѵ�.
 * @param	psData : �۽� ��û�� ������ ������
 * @param	ulLen  : �۽� ��û�� �������� ��
 * @return	ä�ΰ�
 * @date	2018-11-24
 * @version	0.0.1
 */
uint16_t Crc16Ccitt(uint8_t *bBuf, uint32_t ulLen);

/**
 * @brief	16��Ʈ Crc���� ����ϴ� �Լ�.
 * @details	�־��� �����͸� ����Ͽ� Crc �����͸� ����Ѵ�.
 * @param	pBuff : ������ ��Ʈ��
 * @param	ulLen  : ������ ������ ����
 * @return	Crc��
 * @date	2018-11-24
 * @version	0.0.1
 */
uint16_t MakeCrc16(uint16_t *bBuf, uint32_t ulLen);



// ������ �������� CRC�� �о �����Ͱ� ��Ȯ���� �˻��Ѵ�.
// Return 0 : Fail
// Return 1 : Success
int32_t CheckCrc16(uint32_t ulCrc, uint8_t *bBuf, int32_t sz);

uint32_t MakeCrc32(const uint32_t *pulBuf, int len);

uint32_t CheckCrc32(uint16_t *bBuf, uint32_t ulLen, uint32_t ulRcvCrc);

#endif

