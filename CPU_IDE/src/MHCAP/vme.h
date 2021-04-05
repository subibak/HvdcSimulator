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
 * Filename      : vme.h
 * Version       : V1.00
 * Programmer(s) : shjung
 ******************************************************************************
 */

/*
 ******************************************************************************
 REVISION HISTORY :
 ------------------------------------------------------------------------------
 Date          Ver   Name                     Description
 --------------------------------------------------------------------------------
 17/Oct/2019   1.0   SeungHo Jeong (shjung)   Created
 ******************************************************************************
 */

/*
** VME Test는 반드시 LKV-120 HW v1.2 이상에서 수행해야 함.
*/

#ifndef __VME_H__
#define __VME_H__

#define VME_STD_START_ADDR		0x82000000		/* VME Address		*/
#define VME_STD_SIZE			0x00010000		/* 64KB				*/
#define VME_MAX_SLOT			16				/* 16 IO boards		*/

#define VME_AIO_BASE_ADDR		0x00B00000		/* AIO Base Address	*/
#define VME_AIO_MIN_SIZE		16				/* 16 channel		*/

#define VME_DIO_BASE_ADDR		0x00A00000		/* DIO Base Address	*/
#define VME_DIO_OFFSET_ADDR		0x1000
#define VME_DIO_DATA_ADDR		0xF000


#define VME_DIO_DATA_SIZE		0x1000			/* 4KB				*/
#define VME_DATA_MAX_SIZE		VME_DIO_DATA_SIZE
#define VME_DIO_MIN_SIZE		4
#define VME_DIO_CHAN_NO			32				/* 32 channel		*/


/* Master에서 VME 영역은 Standard Mode로만 access할 수 있다 */
typedef struct {
	uchar	vme_slave_init_flag[4];
	uchar	vme_board_id[4];
	uchar	vme_slave_run_count[4];
	uchar	reserved1[5][4];
	uchar	vme_master_init_flag[4];
	uchar	vme_master_run_count[4];
	uchar	reserved2[6][4];
} VME_HEADER;		/* 64bytes. 0x40 */

typedef struct {
	uint	slave_init_flag_ok;
	uint	slave_init_flag_err;
	uint	slave_board_id_ok;
	uint	slave_board_id_err;
} VME_HEADER_STAT;

#define VME_MASTER_INIT_FLAG	0x51525354
#define VME_SLAVE_INIT_FLAG		0xA1A2A3A4


typedef enum {
	UNKNOWN_ID      = 0,
	DI_BOARD_ID		= 1,
	DO_BOARD_ID		= 2,
	AIO_BOARD_ID	= 3
} VME_IO_BOARD_ID;


union IO_DATA {
	ushort	aio_data[VME_AIO_MIN_SIZE];		/* 16 channel */
	uchar	di_data [VME_DIO_MIN_SIZE];		/* 32 channel */
	uchar	do_data [VME_DIO_MIN_SIZE];		/* 32 channel */
};

typedef struct {
	VME_HEADER		header;
	union IO_DATA	iodata;
} VME_DATA;

typedef struct {
	VME_DATA	*vme_data;			/* VME Area Data			*/
	uint		slave_board_type;	/* VME Slave board type		*/
	uint		vme_addr;			/* VME Address				*/
	uint		slave_run_count;	/* VME Slave Run Counter	*/
	uint		slave_run_tick;		/* VME Slave Run Tick		*/
	uint		slave_status;		/* VME Status				*/
#if 0
	uint		vme_start_flag;		/* VME test start flag		*/
#endif
	uint		master_run_count;	/* VME Master Run Counter	*/

	uint		vme_comm_stat;
	uint		vme_wr_data;
	uint		vme_wr_chg_count;
	uint		vme_rd_data;
	uint		vme_rd_chg_count;
	uint		vme_rw_err_count;
	uint		vme_loop_count;
	uint		vme_run_tick;

	int32		tid;				/* Task ID */
	uint		using_semaphore;	/* Using semaphre ? */
	uint		peer_board;			/* Semaphore peer */
	int32		semDoSync;			/* Semaphore for DO			*/
	int32		semDiSync;			/* Semaphore for DI			*/

	VME_HEADER_STAT	stat;			/* VME statistic information */
	uint		size;				/* Test Size */
} VME_CONTROL;


#define VME_TEST_DISP_TIME		2000				/* 2 sec				*/

#endif	/* __VME_H__ */

