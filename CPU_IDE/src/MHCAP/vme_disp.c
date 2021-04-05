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
 * Filename      : vme_disp.c
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

/*
 ******************************************************************************
 *                             INCLUDE FILES
 ******************************************************************************
 */

#include "project_def.h"


/*
 ******************************************************************************
 *                             DEFINES
 ******************************************************************************
 */

IMPORT char	*lineStr;
IMPORT uint	slave_bd_exist_count;
IMPORT uint	vme_start_flag;
IMPORT VME_CONTROL		gstVmeCtrl[VME_MAX_SLOT];


/*
 ******************************************************************************
 *                             GLOBAL VARIABLES
 ******************************************************************************
 */

const char *lineVmeMain = "+------------------------------------------------------------------+";
const char *lineVmeDio  = "+------------------------------------------------------------------------------------+";
const char *lineVmeAio  = "+--------------------------------------------------------------------------------------+";


/*
 ******************************************************************************
 *                             FUNCTIONS
 ******************************************************************************
 */

LOCAL void printDioBarStr (int count)
{
	int	ch;
	printf ("+----+");
	for (ch = 0; ch < count; ch ++)
		printf ("----+");
	printf ("\n");
}

LOCAL void printDioChanNum (int sChan, int eChan)
{
	int	ch;
	printf ("| CH |");
	for (ch = sChan; ch <= eChan; ch ++)
		printf (" %2d |", ch + 1);
	printf ("\n");
}

LOCAL void printDioData (uint u32_data, int sChan, int eChan)
{
	uint	ch, data;
	uint	*addr = (uint *)&u32_data;
	
	printf ("|DATA|");
	for (ch = sChan; ch <= eChan; ch ++) {
		data = (*(addr + (ch / 8)) >> (ch % 8)) & 0x01;
		printf("%s", data ? " ON |" : " OFF|");
	}
	printf ("\n");
}

LOCAL void printAioBarStr (int count)
{
	int	ch;
	printf ("+------+");
	for (ch = 0; ch < count; ch ++)
		printf ("---------+");
	printf ("\n");
}

LOCAL void printAioChanNum (int sChan, int eChan)
{
	int	ch;
	printf ("|  CH  |");
	for (ch = sChan; ch <= eChan; ch ++)
		printf ("    %2d   |", ch + 1);
	printf ("\n");
}

LOCAL void printAioData (int slotId, int sChan, int eChan)
{
	uint	ch, addr;
	
	addr = mhcap_GetAIOVmeAddr (slotId) + (0x40 + (sChan * 2));
	printf ("| DATA |");
	for (ch = sChan; ch <= eChan; ch ++, addr += 2)
		printf(" %7.4f ", *(ADDRESS16)addr);
	printf ("\n");
}

LOCAL void printMainBarStr (int count)
{
	int	ch;
	printf ("+----------+");
	for (ch = 0; ch < count; ch ++)
		printf  ("------+");
	printf  ("\n");
}

LOCAL void printMainSlotNum (int sSlot, int eSlot)
{
	int	ch;
	printf  ("| Slot No  |");
	for (ch = sSlot; ch <= eSlot; ch ++)
		printf  ("  %2d  |", ch + 1);
	printf  ("\n");
}

LOCAL void printMainBdName (int sSlot, int eSlot)
{
	int	i, OnOff;
	printf  ("|Board Name|");
	for (i = sSlot; i <= eSlot; i ++) {
		if (gstVmeCtrl[i].slave_board_type == DI_BOARD_ID)
			printf ("  DI  |");
		else if (gstVmeCtrl[i].slave_board_type == DO_BOARD_ID)
			printf ("  DO  |");
		else if (gstVmeCtrl[i].slave_board_type == AIO_BOARD_ID)
			printf (" AIO  |");
		else
			printf (" NONE |");
	}
	printf  ("\n");
}

LOCAL void mhcap_vme_DispTestResult (int slotId)
{
	uint	board_type = gstVmeCtrl[slotId].slave_board_type;
	printf ("SLOT#%02d : Size = %d, Test Count = %d\n", slotId,
			gstVmeCtrl[slotId].size, gstVmeCtrl[slotId].vme_loop_count);
	if (board_type == DI_BOARD_ID)
		printf ("  r_chg_count=0x%08X rd_data=0x%08X\n",
				gstVmeCtrl[slotId].vme_rd_chg_count, gstVmeCtrl[slotId].vme_rd_data);
	else
		printf ("  w_chg_count=0x%08X wr_data=0x%08X r_chg_count=0x%08X rd_data=0x%08X\n",
				gstVmeCtrl[slotId].vme_wr_chg_count, gstVmeCtrl[slotId].vme_wr_data,
				gstVmeCtrl[slotId].vme_rd_chg_count, gstVmeCtrl[slotId].vme_rd_data);

	printf( "  Slave init flag OK = %d ERROR = %d. Slave board ID OK = %d ERROR = %d\n",
				gstVmeCtrl[slotId].stat.slave_init_flag_ok,
				gstVmeCtrl[slotId].stat.slave_init_flag_err,
				gstVmeCtrl[slotId].stat.slave_board_id_ok,
				gstVmeCtrl[slotId].stat.slave_board_id_err);
}

STATUS mhcap_vme_DioDispProc (int slotId)
{
	int		i, size;
	int		maxChanNo;
	uint	addr, u32_data;
	uint	board_type;
	ushort	u16_data[2];

	board_type = gstVmeCtrl[slotId].slave_board_type;
	if (board_type != DI_BOARD_ID && board_type != DO_BOARD_ID)		return ERROR;

	printf ("\n[VME SLAVE]\n");

	addr = mhcap_GetSlaveVmeAddr (board_type, slotId);
	u16_data[0] = *((ADDRESS16) addr);
	u16_data[1] = *((ADDRESS16) (addr+2));
	u32_data = (u16_data[0] << 16) + u16_data[1];
	printf ("Init flag     : 0x%08X = 0x%08X\n", addr, u32_data);

	addr += 4;
	u16_data[0] = *((ADDRESS16) addr);
	u16_data[1] = *((ADDRESS16) (addr+2));
	u32_data = (u16_data[0] << 16) + u16_data[1];
	printf ("Board ID      : 0x%08X = 0x%08X. %s board.\n", addr, u32_data,
			u32_data == DI_BOARD_ID ? "LKV-551 DI" : u32_data == DO_BOARD_ID ? "LKV-552 DO" : "Unknown");

	addr += 4;
	u16_data[0] = *((ADDRESS16) addr);
	u16_data[1] = *((ADDRESS16) (addr+2));
	u32_data = (u16_data[0] << 16) + u16_data[1];
	printf ("Run Counter   : 0x%08X = 0x%08X\n", addr, u32_data);

	printf ("\n[VME MASTER]\n");
	addr = mhcap_GetSlaveVmeAddr (board_type, slotId) + 0x20;
	u16_data[0] = BYTE_SWAP(*((ADDRESS16) addr));
	u16_data[1] = BYTE_SWAP(*((ADDRESS16) (addr+2)));
	u32_data = (u16_data[1] << 16) + u16_data[0];
	printf ("Init flag     : 0x%08X = 0x%08X\n", addr, u32_data);

	addr += 4;
	u16_data[0] = BYTE_SWAP(*((ADDRESS16) addr));
	u16_data[1] = BYTE_SWAP(*((ADDRESS16) (addr+2)));
	u32_data = (u16_data[1] << 16) + u16_data[0];
	printf ("Run Counter   : 0x%08X = 0x%08X\n", addr, u32_data);

	printf ("\n[%s board]\n", board_type == DI_BOARD_ID ? "DI" : "DO");
	
	addr = mhcap_GetSlaveVmeAddr (board_type, slotId) + 0x40;
	u16_data[0] = *((ADDRESS16) addr);
	u16_data[1] = *((ADDRESS16) (addr+2));
	u32_data = (u16_data[0] << 16) + u16_data[1];
	maxChanNo = VME_DIO_CHAN_NO;
	printf ("Point Status  : 0x%08X = 0x%08X. (ON = On_Line, OFF = Off_Line)\n", addr, u32_data);

	printf ("\n%s\n", lineVmeDio);
	if (board_type == DI_BOARD_ID)
		printf ("|             Digital Input : Point Status Information (DI Menu)                     |\n");
	else
		printf ("|             Digital Output : Point Status Information (DO Menu)                    |\n");		
	printDioBarStr ((maxChanNo/2));
	printDioChanNum (0, (maxChanNo/2) - 1);
	printDioBarStr ((maxChanNo/2));
	printDioData (u32_data, 0, (maxChanNo/2) - 1);

	printDioBarStr ((maxChanNo/2));
	printDioChanNum (maxChanNo/2, maxChanNo - 1);
	printDioBarStr ((maxChanNo/2));
	printDioData (u32_data, maxChanNo/2, maxChanNo - 1);
	printDioBarStr ((maxChanNo/2));

	mhcap_vme_DispTestResult (slotId);
	printf ("%s\n", lineVmeDio);

	return OK;
}

STATUS mhcap_vme_AioDispProc (int slotId)
{
	int		i, size;
	int		maxChanNo;
	uint	addr, u32_data;
	uint	board_type;
	ushort	u16_data[2];

	board_type = gstVmeCtrl[slotId].slave_board_type;
	if (board_type != AIO_BOARD_ID)	return ERROR;

	printf ("\n[VME SLAVE]\n");
	
	addr = mhcap_GetSlaveVmeAddr (board_type, slotId);
	u16_data[0] = *((ADDRESS16) addr);
	u16_data[1] = *((ADDRESS16) (addr+2));
	u32_data = (u16_data[0] << 16) + u16_data[1];
	printf ("Init flag     : 0x%08X = 0x%08X\n", addr, u32_data);

	addr += 4;
	u16_data[0] = *((ADDRESS16) addr);
	u16_data[1] = *((ADDRESS16) (addr+2));
	u32_data = (u16_data[0] << 16) + u16_data[1];
	printf ("Board ID      : 0x%08X = 0x%08X\n", addr, u32_data);
	
	addr += 4;
	u16_data[0] = *((ADDRESS16) addr);
	u16_data[1] = *((ADDRESS16) (addr+2));
	u32_data = (u16_data[0] << 16) + u16_data[1];
	printf ("Run Counter   : 0x%08X = 0x%08X\n", addr, u32_data);

	printf ("\n[VME MASTER]\n");
	
	addr = mhcap_GetSlaveVmeAddr (board_type, slotId) + 0x20;
	u16_data[0] = BYTE_SWAP(*((ADDRESS16) addr));
	u16_data[1] = BYTE_SWAP(*((ADDRESS16) (addr+2)));
	u32_data = (u16_data[1] << 16) + u16_data[0];
	printf ("Init flag     : 0x%08X = 0x%08X\n", addr, u32_data);

	addr += 4;
	u16_data[0] = BYTE_SWAP(*((ADDRESS16) addr));
	u16_data[1] = BYTE_SWAP(*((ADDRESS16) (addr+2)));
	u32_data = (u16_data[1] << 16) + u16_data[0];
	printf ("Run Counter   : 0x%08X = 0x%08X\n", addr, u32_data);

	printf ("\n[AIO board]\n");
	maxChanNo = VME_AIO_MIN_SIZE;

	printf ("\n%s\n", lineVmeAio);
	printf ("|             Analog In/Output : Point Status Information (AIO Menu)                   |\n");
	printAioBarStr ((maxChanNo/2) + 1);
	printAioChanNum (0, (maxChanNo/2) - 1);
	printAioBarStr ((maxChanNo/2) + 1);
	printAioData (slotId, 0, (maxChanNo/2) - 1);
	
	printAioBarStr ((maxChanNo/2) + 1);
	printAioChanNum (maxChanNo/2, maxChanNo - 1);
	printAioBarStr ((maxChanNo/2) + 1);
	printAioData (slotId, maxChanNo/2, maxChanNo - 1);
	printAioBarStr ((maxChanNo/2) + 1);

	mhcap_vme_DispTestResult (slotId);
	printf ("%s\n", lineVmeAio);

	return OK;
}

void mhcap_vme_DispMain (void)
{
	int maxChanNo = VME_MAX_SLOT;

	printf("\n%s\n", lineVmeMain);
	printf("|             VME Slave Board Information (Main Menu)              |\n");
	printMainBarStr ((maxChanNo/2));
	printMainSlotNum (0, (maxChanNo/2) - 1);
	printMainBarStr ((maxChanNo/2));
	printMainBdName (0, (maxChanNo/2) - 1);
	printMainBarStr ((maxChanNo/2));
	printMainSlotNum (maxChanNo/2, maxChanNo - 1);
	printMainBarStr ((maxChanNo/2));
	printMainBdName (maxChanNo/2, maxChanNo - 1);
	printMainBarStr ((maxChanNo/2));
}

void mhcap_vme_DispProc (void)
{
	int		i, size;
	int		maxChanNo, num;

	printf("%s\n", lineStr);
	printf ("[VME] %d slave board is detected !!!\n", slave_bd_exist_count);
	printf("%s\n", lineStr);

	maxChanNo = VME_MAX_SLOT;
	while (vme_start_flag) {
		mhcap_vme_DispMain ();

		num = getNumber("Input number to view detail board information ? [1~16 or q] : ", 1, maxChanNo);
		if (num == 0)			continue;
		else if (num == -1)		return;
		
		switch ((uchar)gstVmeCtrl[num-1].slave_board_type) {
			case DI_BOARD_ID :
			case DO_BOARD_ID :
				mhcap_vme_DioDispProc (num-1);
				break;
			case AIO_BOARD_ID :
				mhcap_vme_AioDispProc (num-1);
				break;
			default :
				printf ("\nUnknown board type !!! type = %d\n", gstVmeCtrl[num-1].slave_board_type);
				break;
		}

		taskDelay (10);
	}
}


