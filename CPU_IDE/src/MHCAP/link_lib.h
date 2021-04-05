/*
 ******************************************************************************
 *              LK11 MHCAP Project
 *
 *              (c) Copyright 2019 ~ 2020 LK11 Co., Ltd.
 *
 * All rights reserved.  Protected by international copyright laws.
 * Knowledge of the source code may NOT be used to develop a similar product.
 * Please help us continue to provide the Embedded community with the finest
 * software available.  Your honesty is greatly appreciated.
 ******************************************************************************
 */

/*
 ******************************************************************************
 *
 *                  NXP QuoIQ P2020
 *                     on the
 *                  LK11's LKV-120 VME Master Board
 *
 * Filename      : link_lib.h
 * Version       : V1.00
 * Programmer(s) : shjung
 ******************************************************************************
 */

/*
 ******************************************************************************
 REVISION HISTORY :
 ------------------------------------------------------------------------------
 Date          Ver   Name                     Description
 ------------------------------------------------------------------------------
 21/Oct/2019   1.0   SeungHo Jeong (shjung)   Modified from LKV-440(Phase Control Board)
 ******************************************************************************
 */

#ifndef __LINK_LIB_H__
#define __LINK_LIB_H__


#define MAX_LINK_CNT			GTX_CH_NUM
#define	MODULE_MAX_LINK			GTX_CH_NUM
#define LP_0					0		/* Port 1 */
#define LP_1					1		/* Port 2 */


#define	LP_TX_MAX_SIZE			256		/* �ܺ� LINK���� ������ �ִ� ��Ű�� �� */
#define	LP_TX_MIN_SIZE			2		/* �ܺ� LINK���� ������ �ּ� ��Ű�� �� */
#define	LP_TX_SIZE				16		/* �ܺ� LINK�� �⺻ ��Ű�� �� (1 ~ 256) */

#define	LP_TX_ELE_SIZE			16		/* ��Ű���� 16���� ������(32 Bit) */

#define	GTX_TX_MAX_SIZE			(LP_TX_MAX_SIZE * LP_TX_ELE_SIZE)
#define	GTX_RX_MAX_SIZE			GTX_TX_MAX_SIZE

#define	LP_TX_BUF_SIZE			(LP_TX_SIZE * LP_TX_ELE_SIZE)
#define	LP_TX_REAL_LEN			(LP_TX_BUF_SIZE - 2)

#define	CHECK_LP_RX_FAIL		0x00000004

#define	MAX_PACKET_CNT			11
#define	MAX_PACKET_DATA			((LP_TX_MAX_SIZE/16) * LP_TX_ELE_SIZE)

#define	PACKET_INIT_DATA		0xA1A2A3A4
#define PACKET_OFFSET(p_inx)	(MAX_PACKET_DATA * p_inx)



typedef struct {
	uint	ltx_set_env;			/* Link Tx Mode ȯ�� ���� */
	uint	tx_cnt;					/* Tx �õ� Ƚ�� */
	uint	tx_ok_cnt;				/* ���� Tx Ƚ�� */
	uint	tx_err_cnt;				/* ���� Tx Ƚ�� */

	uint	lrx_set_env;			/* Link Rx Mode ȯ�� ���� */
	uint	rx_cnt;					/* Rx �߻� ���� */
	uint	rx_ok_cnt;				/* ���� Rx Ƚ�� */
	uint	rx_overflow;			/* Rx�� ������ �����͸� ó������ ���� ���¿��� ������ ������ ���� */
	uint	rx_check_sum_err;		/* Rx ���Ž� check sum ���� Ƚ�� */
#if 1
	uint	rx_seq_no_err;			/* Rx sequence number ���� Ƚ�� */
#endif
	uint	rx_int_proc_flag;		/* Rx Int���� ���� */

	uint	rx_read_cnt;			/* rx_buf�� ���� �����͸� ���� Ƚ�� */

	uint	rx_pack_buf[GTX_RX_MAX_SIZE];
	uint	tx_seq_num;
	uint	rx_seq_num;

	uint	rcv_check_sum;			/* for debug */
	uint	mk_check_sum;			/* for debug */
} LP_ENV_DATA;

typedef struct {
	int	packet_init;
	int	packet_cnt;
	int	err_status;				/* ���� �����Ϳ� ������ check sum�� ���� ��� 1�� ���� */
	int packet_len[MAX_PACKET_CNT];
} PACK_INFO;


#endif	/* __LINK_LIB_H__ */


