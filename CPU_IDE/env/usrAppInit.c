/* usrAppInit.c - stub application initialization routine */

/* Copyright (c) 1998,2006,2011 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,26may11,pcs  Add stubs corresponding to the boot sequence hook callout
                 mechanism.
01b,16mar06,jmt  Add header file to find USER_APPL_INIT define
01a,02jun98,ms   written
*/

/*
DESCRIPTION
Initialize user application code.
*/ 

#include <vxWorks.h>
#if defined(PRJ_BUILD)
#include "prjParams.h"
#endif /* defined PRJ_BUILD */

#if	1
#include "taskLib.h"
#include "sysLib.h"

/* ����� application��	���� �Լ� */
IMPORT void	usrInitTask(void);
#endif

/******************************************************************************
*
* usrAppInit - initialize the users	application
*/

void usrAppInit	(void)
	{
#ifdef	USER_APPL_INIT
	USER_APPL_INIT;		/* for backwards compatibility */
#endif

	/* add application specific	code here */
#if	1
	taskSpawn("userInitTask", 100, VX_FP_TASK, 0x10000,
			(FUNCPTR)usrInitTask, 0,0,0,0,0,0,0,0,0,0);

	taskDelay(sysClkRateGet()/10);
#endif
    }

#ifdef INCLUDE_USER_PRE_KERNEL_APPL_INIT
/******************************************************************************
*
* usrPreKernelAppInit - initialize the users pre-kernel application code
*/ 

void usrPreKernelAppInit (void)
    {

    /*
     * Add application specific code here.
     * No kernel feature is available at this point.
     */

    }
#endif

#ifdef INCLUDE_USER_POST_KERNEL_APPL_INIT
/******************************************************************************
*
* usrPostKernelAppInit - initialize the users post-kernel application code
*/ 

void usrPostKernelAppInit (void)
    {

    /*
     * Add application specific code here.
     * Core kernel feature is available at this point.
     * IO system and Network is not available.
     * kernel features dependent on system clock or timer not available.
     */

    }
#endif


#ifdef INCLUDE_USER_PRE_NETWORK_APPL_INIT
/******************************************************************************
*
* usrPreNetworkAppInit - initialize the users pre-network application code
*/ 

void usrPreNetworkAppInit (void)
    {

    /*
     * Add application specific code here.
     * Core kernel feature and IO system is available at this point.
     * Network is not available.
     */

    }
#endif


