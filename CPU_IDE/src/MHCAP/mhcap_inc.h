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
 * Filename      : mhcap_inc.h
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


#ifndef __MHCAP_INC_H__
#define __MHCAP_INC_H__


/*
 ******************************************************************************
 *                             INCLUDE FILES
 ******************************************************************************
 */

/* cmd_proc.c */
IMPORT void usage (int argc, char **argv);
IMPORT void set_local_mode (int argc, char **argv);
IMPORT void set_print_mode(int argc, char **argv);
IMPORT void prog_stop(int argc, char **argv);


/* common.c */
IMPORT ULONG crc32(ULONG crc, const uchar *buf, int len);
IMPORT uint duration_tick (ULONG tick);
#if (PROJ_RUN_TICK == 10000)
IMPORT uint duration_100usec (uint usec);
IMPORT void wait_100us (uint us);
#endif
IMPORT uint duration_msec (uint msec);
IMPORT uint duration_sec (uint sec);
IMPORT void nothing (void);
IMPORT void delay_nothing (int max);
IMPORT void msDelay (uint msec);
IMPORT void taskMsDelay (uint msec);
IMPORT int getYesNo (char *msg, char yes_flag, char no_flag);
IMPORT int getNumber (char *msg, int MinNo, int MaxNo);
IMPORT uint swap32 (uint data);
IMPORT uint swap16to32 (ushort data0, ushort data1);
IMPORT void swap32to16 (uint data, ushort *data0, ushort *data1);
IMPORT int str2int (char *str);
IMPORT uint str2uint (char *str);
IMPORT float get_mbps_data (uint snd_size, uint p_cnt, uint p_msec, int bits);


/* link_lib.c */
IMPORT void linkEnable(int lp_inx, int onoff);
IMPORT LP_ENV_DATA *getLpEnvData(int lp_inx);
IMPORT void initLpEnvData(void);
IMPORT uint *getLpEnvPackBuf(int lp_inx);
IMPORT void makePackInfo(uint *tx_buf);
IMPORT int appendLinkPacket(int packet_inx, uint *to_pack_buf, uint *fr_buf, int len);
IMPORT int appendLinkPackBuf(int packet_inx, uint *to_pack_buf, uint *fr_pack_buf);
IMPORT int linkSendProc(int lp_inx, uint *tx_pack_buf);
IMPORT int linkWriteProc(int lp_index, uint *tx_pack_buf);
IMPORT int linkWrite(int tx_lp_index, uint *in_data, int len, int *inc_lp_index);
IMPORT int linkReadProc(int lp_inx, uint *rcv_pack_buf);
IMPORT int linkRead(int lp_inx);
IMPORT int linkReadPacket(int lp_inx, int packet_inx, uint *buf);
IMPORT void showGtxData(uint *gtx_buf);


#if 1	/* INCLUDE_DEBUG_ENV */
IMPORT int link_test_set (int mode, int lp_no);
IMPORT void link_show (int argc, char **argv);
IMPORT void link_show1 (void);
IMPORT void link_test_end (void);
#if 0
IMPORT void link_test_proc (void);
#endif
IMPORT void link_test (int argc, char **argv);
IMPORT void link_test1 (void);
IMPORT void link_req_test(int argc, char **argv);
IMPORT void link_led_test(int argc, char **argv);
#endif	/* INCLUDE_DEBUG_ENV */


/* lkp520.c */
IMPORT STATUS Lkp502Init (void);
IMPORT STATUS Lkp502PciShow (void);
IMPORT STATUS Lkp502InfoShow (void);
IMPORT STATUS Lkp502GetSFPAbsenceStatus (int port, uchar *val);
IMPORT STATUS Lkp502Read (int port, uint32 *pBuf, uint32 size);
IMPORT STATUS Lkp502Write (int port, uint32 *pBuf, uint32 size);
IMPORT STATUS Lkp502GetRxErrorStatus (int port, uchar *val);
IMPORT STATUS Lkp502GetTxErrorStatus (int port, uchar *val);
IMPORT uint32 Lkp502SetTxSize (int port, uint32 size);
IMPORT STATUS Lkp502CableCheck (int port);
IMPORT STATUS Lkp502SetGtxRstCtrl (int port, int reset);
IMPORT STATUS Lkp502SetSfpTxCtrl (int port, int OnOff);
IMPORT void Lkp502SetSLedAutoMode (int mode);
IMPORT STATUS Lkp502SetSLedCtrl (byte value);
IMPORT void Lkp502SetCLedAutoMode (int auto_mode);
IMPORT STATUS Lkp502GetCLedAutoMode (void);
IMPORT STATUS Lkp502SetRxLed (int port, int ctrl);
IMPORT STATUS Lkp502SetTxLed (int port, int ctrl);
IMPORT STATUS Lkp502SetTxInt (int port, int enable);
IMPORT STATUS Lkp502SetRxInt (int port, int enable);

/* mhcap_main.c */
IMPORT void mhcap_Logo (int argc, char **argv);
IMPORT void mhcap_stop (void);
IMPORT void mhcap_main_task (void);

/* mhcap_task.c */
IMPORT void mhcap_basic_task_end (void);
IMPORT void vmeBoardInit (void);


/* vme.c */
IMPORT void mhcap_vme_GetPeerBd (int flag);
IMPORT uint mhcap_GetAIOVmeAddr (int slotId);
IMPORT uint mhcap_GetSlaveVmeAddr (int board_type, int slotId);
IMPORT void vme_stop (void);
IMPORT void setVmeEnv (void);
IMPORT void vme_set_data_size (uint size);
IMPORT STATUS mhcap_GetVmeAddr (int	slotId);
IMPORT STATUS mhcap_vme_DiProc (int slotId);
IMPORT STATUS mhcap_vme_DoProc (int slotId);
IMPORT STATUS mhcap_vme_AioProc (int slotId);


/* vme_disp.c */
IMPORT STATUS mhcap_vme_DioDispProc (int slotId);
IMPORT STATUS mhcap_vme_AioDispProc (int slotId);
IMPORT void mhcap_vme_DispProc (void);
IMPORT void mhcap_vme_DispMain (void);


/* vme_test.c */
IMPORT uint mvme_std_read (uint vme_addr);
IMPORT void mvme_std_write (uint vme_addr, uint data);
IMPORT void vme_mtest_end (void);
IMPORT void vme_test (int argc, char **argv);
IMPORT void vme_disp (int argc, char **argv);


#endif	/* __MHCAP_INC_H__ */


