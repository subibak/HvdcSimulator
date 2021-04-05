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
 * Filename      : mhcap_task.c
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
 22/Nov/2019   1.0   SeungHo Jeong (shjung)   Created
 ******************************************************************************
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

#define DI_TASK_NAME		"tDiTask"
#define DO_TASK_NAME		"tDoTask"
#define AIO_TASK_NAME		"tAioTask"
#define BASIC_TASK_NAME		"tBasicTask"

#define CONSOLE_TASK_NAME	"tConsole"

/*
 ******************************************************************************
 *                             GLOBAL VARIABLES
 ******************************************************************************
 */

uint	slave_bd_exist_count = 0;
uint	delay_time = 1;
TASK_ID	basic_task_id = ERROR;

IMPORT uint	vme_start_flag;
IMPORT VME_DATA			gstVmeData[VME_MAX_SLOT];
IMPORT VME_CONTROL		gstVmeCtrl[VME_MAX_SLOT];


/*
 ******************************************************************************
 *                             FUNCTIONS
 ******************************************************************************
 */

void mhcap_basic_task_end (void)
{
	if (basic_task_id != ERROR)
		taskDelete (basic_task_id);
}

void vmeBoardInit (void)
{
	int		i;

	vme_start_flag = TRUE;

	memset ((char *)gstVmeCtrl, 0x00, sizeof (VME_CONTROL) * VME_MAX_SLOT);
	memset ((char *)gstVmeData, 0x00, sizeof (VME_DATA) * VME_MAX_SLOT);

	/* Set VME environment - semaphore & peer board */
	/*
	setVmeEnv ();
	*/
	
	/* 각 slot을 확인하고 VME 주소를 획득한다 */
	for (i = 0; i < VME_MAX_SLOT; i ++) 
	{
		mhcap_GetVmeAddr (i);
	
		if (gstVmeCtrl[i].vme_addr != 0) 
		{
			/* VME slave와 통신을 수행하는 task 생성 */
			if (gstVmeCtrl[i].slave_board_type == DI_BOARD_ID) 
			{
				gstVmeCtrl[i].size = VME_DIO_MIN_SIZE;	//VME_DATA_MAX_SIZE
				slave_bd_exist_count ++;
			}
			else if (gstVmeCtrl[i].slave_board_type == DO_BOARD_ID) 
			{
				gstVmeCtrl[i].size = VME_DIO_MIN_SIZE;	//VME_DATA_MAX_SIZE;
				slave_bd_exist_count ++;
			}
			else if (gstVmeCtrl[i].slave_board_type == AIO_BOARD_ID) 
			{
				gstVmeCtrl[i].size = VME_AIO_MIN_SIZE;
				slave_bd_exist_count ++;
			}
			else 
			{
				continue;
			}

			taskDelay (sysClkRateGet()/10); /* 100msec 마다 실행 */
		}
	}

	return;
}

