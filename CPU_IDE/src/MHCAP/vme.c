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
 * Filename      : vme.c
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
#define VME_SLAVE_CH		0x5A
#define VME_MASTER_CH		0xA5

#define VME_MASTER_ERR_TIME		3000
#define VME_SLAVE_ERR_TIME		3000
#define VME_SLAVE_CHK_TIME		3000
#define DISP_STATUS_TIME		5000


/*
 ******************************************************************************
 *                             GLOBAL VARIABLES
 ******************************************************************************
 */
uint	vme_r_data			= 0;
uint	vme_w_data			= 0;
uint	vme_w_chg_cnt		= 0;
uint	vme_r_chg_cnt		= 0;
uint	vme_rw_err_cnt		= 0;
uint	vme_loop_cnt		= 0;
uint	vme_tick			= 0;

uint	vme_data_size       = 0;

uint	vme_start_flag		= FALSE;	/* TRUE : test start		*/
uint	vme_debug_flag		= FALSE;	/* TRUE : debug start		*/
uint	vme_test_size		= VME_DIO_MIN_SIZE;
uint	vme_remaind_test	= FALSE;	/* 추가적인 VME 영역 테스트			*/
uint	vme_data_start		= FALSE;
uint	vme_proc_start		= FALSE;

uchar	DoDataBuf[VME_DATA_MAX_SIZE];
uchar	DiDataBuf[VME_DATA_MAX_SIZE];


VME_HEADER		*vme_system_header;
VME_DATA		gstVmeData[VME_MAX_SLOT];
VME_CONTROL		gstVmeCtrl[VME_MAX_SLOT];

char	*lineStr = "================================================================================";

IMPORT uint	delay_time;


/*
 ******************************************************************************
 *                             FUNCTIONS
 ******************************************************************************
 */

void vme_debug (uint flag)
{
	if (flag != FALSE)
		vme_debug_flag = TRUE;
	else
		vme_debug_flag = FALSE;
}

/* VME에서 DIO의 나머지 영역에서 테스트할 경우 사용하는 함수 */
void vme_ExtDebug (uint flag)
{
	if (flag != FALSE)
		vme_remaind_test = TRUE;
	else
		vme_remaind_test = FALSE;
}

/* CPU 보드에서 vme test를 종료하려 하는 경우 사용하는 함수	*/
/* 사용 예 : vme_stop									*/
void vme_stop (void)
{
	int	i;
	vme_start_flag = FALSE;
	for (i = 0; i < VME_MAX_SLOT; i ++) {
		if (gstVmeCtrl[i].using_semaphore != TRUE)	continue;
		
		if (gstVmeCtrl[i].semDoSync)
			semGive (gstVmeCtrl[i].semDoSync);
		if (gstVmeCtrl[i].semDiSync)
			semGive (gstVmeCtrl[i].semDiSync);
	}
}

void vme_delay (uint32 vme_delay)
{
	if (vme_delay)
		delay_time = vme_delay;
}

/* CPU 보드에서 변경 사항을 reset하려 하는 경우 사용하는 함수	*/
/* 사용 예 : vme_reset										*/
void vme_reset (void)
{
	vme_w_chg_cnt	= 0;
	vme_r_chg_cnt	= 0;
	vme_rw_err_cnt	= 0;

	memset ((char *)DoDataBuf, 0x00, VME_DATA_MAX_SIZE);
	memset ((char *)DiDataBuf, 0x00, VME_DATA_MAX_SIZE);
}

void vme_set_data_size (uint size)
{
	if (size > VME_DIO_DATA_SIZE)		size = VME_DIO_DATA_SIZE;
	else if (size < VME_DIO_MIN_SIZE)	size = VME_DIO_MIN_SIZE;
	vme_data_size = size;
}

void setVmeEnv (void)
{
	int		i;
	for (i = 0; i < VME_MAX_SLOT; i ++) {
		if (i == 0) {
			gstVmeCtrl[i].using_semaphore = TRUE;
			gstVmeCtrl[i].peer_board	  = 2;
		} else if (i == 2) {
			gstVmeCtrl[i].using_semaphore = TRUE;
			gstVmeCtrl[i].peer_board	  = 0;
		} else {
			gstVmeCtrl[i].using_semaphore = FALSE;
			gstVmeCtrl[i].size = VME_DIO_MIN_SIZE;
			gstVmeCtrl[i].peer_board	  = 0;
		} 
	}
}

uint mhcap_GetSlaveBoardCount (void)
{
	int		i;
	uint	count = 0;

	/* 각 slot을 확인하고 VME 주소를 획득한다 */
	for (i = 0; i < VME_MAX_SLOT; i ++) {
		mhcap_GetVmeAddr (i);
		if (gstVmeCtrl[i].vme_addr == 0)	continue;
	
		if (gstVmeCtrl[i].slave_board_type == DI_BOARD_ID ||
			gstVmeCtrl[i].slave_board_type == DO_BOARD_ID ||
			gstVmeCtrl[i].slave_board_type == AIO_BOARD_ID)		count ++;
	}
	return count;
}

uint mhcap_GetDIOVmeAddr (int slotId)
{
#if 0
	return (VME_STD_START_ADDR + VME_DIO_BASE_ADDR + 
			(VME_STD_SIZE * slotId) + VME_DIO_DATA_ADDR);
#else
	return (VME_STD_START_ADDR + VME_DIO_BASE_ADDR + (VME_STD_SIZE * slotId));
#endif
}

uint mhcap_GetAIOVmeAddr (int slotId)
{
	return (VME_STD_START_ADDR + VME_AIO_BASE_ADDR + (VME_STD_SIZE * slotId));
}

uint mhcap_GetSlaveVmeAddr (int board_type, int slotId)
{
	if (board_type == DI_BOARD_ID ||
		board_type == DO_BOARD_ID)
		return (mhcap_GetDIOVmeAddr (slotId));
	else if (board_type == AIO_BOARD_ID)
		return (mhcap_GetAIOVmeAddr (slotId));
	else
		return 0;
}

/* LKV_440 or LKV-550 보드의 init_flag를 읽는다. */
LOCAL STATUS mhcap_GetSlaveInitFlag (int slotId)
{
	uint	addr, init_flag;
	/* uint id, dump, i;*/
	ushort	u16_data[2];

	addr = mhcap_GetSlaveVmeAddr (gstVmeCtrl[slotId].slave_board_type, slotId);
	u16_data[0] = *((ADDRESS16) addr);
	u16_data[1] = *((ADDRESS16) (addr+2));
	init_flag = (u16_data[0] << 16) + u16_data[1];

#if 0
	u16_data[0] = *((ADDRESS16) (addr+4));
	u16_data[1] = *((ADDRESS16) (addr+6));
	id = (u16_data[0] << 16) + u16_data[1];
#endif
	
#if 0
if((gstVmeCtrl[slotId].slave_board_type == AIO_BOARD_ID)&&(slotId == 2))
{
		printf ("init_flag=0x%08X, id=0x%08X\n", init_flag, id);
		printf ("\n");
		for(i=0;i<38;i+=2)
		{
        	u16_data[0] = *((ADDRESS16) (addr+i));
        	u16_data[1] = *((ADDRESS16) (addr+i+2));
        	dump = (u16_data[0] << 16) + u16_data[1];
        	printf ("%08x ", dump);
        	if(i%16 == 0) printf ("[%08x]",i);
        	else if(i%16 == 12) printf ("\n");
    	}
}
#endif	
	
	/*if(gstVmeCtrl[slotId].slave_board_type != AIO_BOARD_ID)*/
	{
	if (init_flag != VME_SLAVE_INIT_FLAG) {
#if 0
		logMsg ("[ERROR] VME Slave[%d] init flag failed !!! init_flag=0x%08X\n", slotId, init_flag,3,4,5,6);
#endif
		gstVmeCtrl[slotId].stat.slave_init_flag_err ++;
		return ERROR;
	}
	}

	gstVmeCtrl[slotId].stat.slave_init_flag_ok ++;
	return OK;
}

/* LKV_440 or LKV-550 보드의 ID를 찾는다. */
LOCAL STATUS mhcap_GetVmeBoardId (int slotId)
{
	uint	addr, boardId;
	ushort	u16_data[2];

	/* LKV-550 보드 타입을 알려주는 주소 계산. 0x82A0:F000 ~ 0x82AF:F000 */
	addr = mhcap_GetDIOVmeAddr (slotId) + 4;
	u16_data[0] = *((ADDRESS16) addr);
	u16_data[1] = *((ADDRESS16) (addr+2));
	boardId = (u16_data[0] << 16) + u16_data[1];
	if (boardId == DI_BOARD_ID) {
		gstVmeCtrl[slotId].slave_board_type = DI_BOARD_ID;
		gstVmeCtrl[slotId].stat.slave_board_id_ok ++;
		return OK;
	} else if (boardId == DO_BOARD_ID) {
		gstVmeCtrl[slotId].slave_board_type = DO_BOARD_ID;
		gstVmeCtrl[slotId].stat.slave_board_id_ok ++;
		return OK;
	}

	/* LKV-440 보드 타입을 알려주는 주소 계산. 0x82B0:0000 ~ 0x82BF:0000 */
	addr = mhcap_GetAIOVmeAddr (slotId) + 4;
	u16_data[0] = *((ADDRESS16) addr);
	u16_data[1] = *((ADDRESS16) (addr+2));
	boardId = (u16_data[0] << 16) + u16_data[1];
	if (boardId == AIO_BOARD_ID) {
		gstVmeCtrl[slotId].slave_board_type = AIO_BOARD_ID;
		gstVmeCtrl[slotId].stat.slave_board_id_ok ++;
		return OK;
	}

	if (vme_debug_flag) 
		logMsg ("[ERROR] Unknown VME Slave[%d] board ID !!! bd_id=0x%08X\n", slotId, boardId,3,4,5,6);
	gstVmeCtrl[slotId].stat.slave_board_id_err ++;
	return ERROR;
}

/* LKV_440 or LKV-550 보드의 run_count를 읽는다. */
LOCAL void mhcap_GetSlaveRunCount (int slotId, uint *run_count, int start_flag)
{
	uint	addr;
	ushort	u16_data[2];

	addr = mhcap_GetSlaveVmeAddr (gstVmeCtrl[slotId].slave_board_type, slotId) + 8;
	u16_data[0] = *((ADDRESS16) addr);
	u16_data[1] = *((ADDRESS16) (addr+2));
	*run_count = (u16_data[0] << 16) + u16_data[1];

	if (start_flag)
		gstVmeCtrl[slotId].slave_run_count = *run_count;
}

/* LKV_440 or LKV-550 보드의 STD_SADD를 찾는다. */
STATUS mhcap_GetVmeAddr (int	slotId)
{
	uint	addr;

	if (mhcap_GetVmeBoardId (slotId) != OK)		return ERROR;
	addr = mhcap_GetSlaveVmeAddr (gstVmeCtrl[slotId].slave_board_type, slotId);
	if (addr == 0)		return ERROR;
	gstVmeCtrl[slotId].vme_addr = addr;
	gstVmeCtrl[slotId].vme_data = (VME_DATA *)addr;
	return OK;
}

void mhcap_vme_reset (int slotId)
{
	gstVmeCtrl[slotId].vme_wr_chg_count = 0;
	gstVmeCtrl[slotId].vme_rd_chg_count = 0;
	gstVmeCtrl[slotId].vme_rw_err_count = 0;
}

STATUS mhcap_checkVmeSlaveStatus (int slotId, int start_flag)
{
	uint	run_count;

	if (mhcap_GetSlaveInitFlag (slotId) != OK) 		return ERROR;

	if (mhcap_GetVmeBoardId (slotId) != OK)			return ERROR;

	mhcap_GetSlaveRunCount (slotId, &run_count, start_flag);

	if (start_flag == TRUE)		return OK;

	if (gstVmeCtrl[slotId].slave_run_count != run_count) 
	{
		if (gstVmeCtrl[slotId].slave_status == FALSE)
			logMsg ("[Slot %02d] VME Slave Recovery ... OK !!!\n", slotId,2,3,4,5,6);
	
		gstVmeCtrl[slotId].slave_run_count = run_count;
		gstVmeCtrl[slotId].slave_status	   = TRUE;
		gstVmeCtrl[slotId].slave_run_tick  = tickGet();
		
		return OK;
	}

	if (gstVmeCtrl[slotId].slave_status) 
	{
#if 0
		if (duration_msec(gstVmeCtrl[slotId].slave_run_tick) >= (VME_MASTER_ERR_TIME*10)) {
			logMsg ("[Slot %02d] VME Slave Down !!!\n", slotId,2,3,4,5,6);
			gstVmeCtrl[slotId].slave_status = FALSE;
			return ERROR;
		}
#endif		
		return OK;
	}
	
	return ERROR;
}

void mhcap_setVmeMasterStatus (uint slotId, int start_flag)
{
	vuchar	*vme_addr;
	int		i;

	/* Set VME Master status */
	vme_addr = (vuchar *)(gstVmeCtrl[slotId].vme_addr + (8 * sizeof(int))); /* offset : 32 */
	if (start_flag) {
		for (i = 0; i < 4; i ++)
			*(vme_addr + i) = (VME_MASTER_INIT_FLAG >> (8 * i)) & 0xFF; /* 바이트(8bits) 단위로 쪼개서 데이터 WRITE */
	} else {
		for (i = 0; i < 4; i ++)
			*(vme_addr + i) = 0x00;
	}

	/* Set RUN counter for DIO */
	gstVmeCtrl[slotId].master_run_count ++;
	vme_addr += 4; /* offset : 36 */
	for (i = 0; i < 4; i ++)
		*(vme_addr + i) = (gstVmeCtrl[slotId].master_run_count >> (8 * i)) & 0xFF; /* 바이트(8bits) 단위로 쪼개서 데이터 WRITE */
}

/* 일정 시간마다 상태를 출력 */
void mhcap_dispVmeStat (int slotId, int size)
{
#if 0
	uint	boardId = gstVmeCtrl[slotId].slave_board_type;
	if (duration_msec (gstVmeCtrl[slotId].vme_run_tick) >= DISP_STATUS_TIME) {
		printf ("================================================================\n");
		printf ("SLOT#%02d : [%s] Size = %d, Test Count = %d\n", slotId,
				((boardId==DI_BOARD_ID)?"DI":(boardId==DO_BOARD_ID)?"DO":"AIO"),
				size, gstVmeCtrl[slotId].vme_loop_count);
		if (boardId == DI_BOARD_ID)
			printf ("  r_chg_count=0x%08X rd_data=0x%08X\n",
				gstVmeCtrl[slotId].vme_rd_chg_count, gstVmeCtrl[slotId].vme_rd_data);
		else if (boardId == DO_BOARD_ID)
			printf ("  w_chg_count=0x%08X wr_data=0x%08X r_chg_count=0x%08X rd_data=0x%08X\n",
					gstVmeCtrl[slotId].vme_wr_chg_count, gstVmeCtrl[slotId].vme_wr_data,
					gstVmeCtrl[slotId].vme_rd_chg_count, gstVmeCtrl[slotId].vme_rd_data);
		else if (boardId == AIO_BOARD_ID)
			printf ("  w_chg_count=0x%08X wr_data=0x%08X r_chg_count=0x%08X rd_data=0x%08X\n",
					gstVmeCtrl[slotId].vme_wr_chg_count, gstVmeCtrl[slotId].vme_wr_data,
					gstVmeCtrl[slotId].vme_rd_chg_count, gstVmeCtrl[slotId].vme_rd_data);
		gstVmeCtrl[slotId].vme_run_tick = tickGet ();
	}
#endif
}



