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


#define	LP_TX_MAX_SIZE			256		/* 외부 LINK에서 가용한 최대 패키지 수 */
#define	LP_TX_MIN_SIZE			2		/* 외부 LINK에서 가용한 최소 패키지 수 */
#define	LP_TX_SIZE				16		/* 외부 LINK의 기본 패키수 수 (1 ~ 256) */

#define	LP_TX_ELE_SIZE			16		/* 패키지당 16개의 데이터(32 Bit) */

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
	uint	ltx_set_env;			/* Link Tx Mode 환경 설정 */
	uint	tx_cnt;					/* Tx 시도 횟수 */
	uint	tx_ok_cnt;				/* 정상 Tx 횟수 */
	uint	tx_err_cnt;				/* 오류 Tx 횟수 */

	uint	lrx_set_env;			/* Link Rx Mode 환경 설정 */
	uint	rx_cnt;					/* Rx 발생 갯수 */
	uint	rx_ok_cnt;				/* 정상 Rx 횟수 */
	uint	rx_overflow;			/* Rx로 수신한 데이터를 처리하지 못한 상태에서 데이터 수신한 숫자 */
	uint	rx_check_sum_err;		/* Rx 수신시 check sum 오류 횟수 */
#if 1
	uint	rx_seq_no_err;			/* Rx sequence number 오류 횟수 */
#endif
	uint	rx_int_proc_flag;		/* Rx Int에서 설정 */

	uint	rx_read_cnt;			/* rx_buf로 부터 데이터를 읽은 횟수 */

	uint	rx_pack_buf[GTX_RX_MAX_SIZE];
	uint	tx_seq_num;
	uint	rx_seq_num;

	uint	rcv_check_sum;			/* for debug */
	uint	mk_check_sum;			/* for debug */
} LP_ENV_DATA;

typedef struct {
	int	packet_init;
	int	packet_cnt;
	int	err_status;				/* 읽은 데이터에 오류나 check sum이 깨진 경우 1로 설정 */
	int packet_len[MAX_PACKET_CNT];
} PACK_INFO;


#endif	/* __LINK_LIB_H__ */


