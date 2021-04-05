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
 * Filename      : project_def.h
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

#ifndef __PROJECT_DEF_H__
#define __PROJECT_DEF_H__


/* VxWorks Header Files */
#include <vxWorks.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <intLib.h>
#include <ttyLib.h>
#include <unistd.h>
#include <types.h>
#include <socket.h>
#include <in.h>
#include <inetLib.h>
#include <sockLib.h>
#include <socket.h>
#include <wrn/coreip/netinet/tcp.h>
#include <usrLib.h>
#include <netLib.h>
#include <hostLib.h>
#include <logLib.h>
#include <sysLib.h>
#include <taskLib.h>
#include <tickLib.h>
#include <semLib.h>
#include <drv/pci/pciConfigLib.h>
#include <drv/pci/pciIntLib.h>
#include <sioLib.h>
#include <ioLib.h>
#include <tyLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/vxbus/vxbPciLib.h>
#include <vxBusLib.h>


/* Project Header Files */
#include "usrTypes.h"
#include "mhcap_types.h"
#include "vme.h"
#include "lkp502.h"
#include "link_lib.h"
#include "mhcap_inc.h"

/*
 ******************************************************************************
 *                             DEFINES
 ******************************************************************************
 */

#define PROJECT_NAME		"MHCAP"
#define SW_VER_INFO			"1.0.0"


#define PROJ_RUN_TICK		1000			/* 1 tick per 1msec */
#define MAX_TICK_CNT		86400000		/* 1 day, 1msec tick */
#define TICK_PER_MS_SECOND	1


#define PRT_PROMPT			printf("%s> ", PromptStr)


#define DI_TASK_NAME		"tDiTask"
#define DO_TASK_NAME		"tDoTask"
#define AIO_TASK_NAME		"tAioTask"

#define V_TASK_STACK_SIZE	0x10000
#define V_TASK_PRIORITY		100
#define V_TASK_OPTION		(VX_FP_TASK|VX_SUPERVISOR_MODE|VX_STDIO)



#endif	/* __PROJECT_DEF_H__ */


