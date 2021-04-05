
#ifndef SYSDEFS_H
#define SYSDEFS_H


#define	ROM_RELEASE_TO_USER		1


#define	EVALUATION_TIMEOUT_1DAY		(86400)
#define	EVALUATION_TIMEOUT_1HOUR	(3600)
#define	EVALUATION_TIMEOUT_5MIN		(300)

#define	EVALUATION_TIMEOUT	(EVALUATION_TIMEOUT_1DAY)

#if 0
	#define EVALUATION_USE_BUILD
#else
	#undef EVALUATION_USE_BUILD
#endif


#if ( (!defined(_WIN32)) && (!defined(LINUX)) )
#ifndef	VXWORKS
	#define	VXWORKS
#endif
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <tlhelp32.h>
#include <time.h>
#include <tchar.h>
#include <signal.h>

#ifdef _WINCE
	#include <pkfuncs.h>
#endif

#define _MBTS_PROGRAM
#define _PMS_PROGRAM
#undef _SINGLE_CONTROLLER_PROGRAM
#define _USR_C_TASK_PROGRAM

#endif


#if (defined(VXWORKS))

#elif (defined(_WIN32))
#define	_MEMORY_MAP_FILE	(1)
#define	_IPC_INTERFACE		(1)

#define CPU_TYPE_MVMEPPC	1
#define CPU_TYPE_CPCIX86    2
#endif

#define	MVME2100_BOARD	1
#define	MVME3100_BOARD	2
#define	MVME4100_BOARD	3
#define	MVME2500_BOARD	4
#define	LKV120_BOARD	5
#define X86_PC_BOARD  	6
#define LINUX_PC_BOARD	7
#define LINUX_XU3_BOARD 8

#define X86_PC_CPCI_BOARD	(1)
#define	X86_PC_GENERAL		(2)

#ifdef _WIN32
#define	X86_PC_TYPE		(X86_PC_GENERAL)
#else
#define	X86_PC_TYPE		(0)
#endif

#if (defined(VXWORKS))

#elif (defined(_WIN32))
	#define	BOARD_TYPE	(X86_PC_BOARD)
#elif (defined(LINUX))
	#define	BOARD_TYPE	(LINUX_PC_BOARD)
#endif

#undef 	BUCHEON_SPECIFIC_121009
#define	PMS_SPECIFIC


#define	MB_SLAVE_MAP_DPMS		(1)
#define	MB_SLAVE_MAP_SAMSUNG	(2)	
#define	MB_SLAVE_MAP_DIPP		(2)
#define	MB_SLAVE_MAP_GOCHANG	(3)

#define	MB_SLAVE_MAP_TYPE	(MB_SLAVE_MAP_SAMSUNG)

#if (MB_SLAVE_MAP_TYPE == MB_SLAVE_MAP_SAMSUNG)
    #undef _SINGLE_CONTROLLER_PROGRAM
#else
#endif


#define	ETH3_BOARD			(1)
#define	BUS_BRIDGE_BOARD	(2)

#ifdef VXWORKS
    #if (BOARD_TYPE == LKV120_BOARD)
	    #define	SYSTEM_DATA_SYNC_BOARD	(ETH3_BOARD)
    #else
	    #define	SYSTEM_DATA_SYNC_BOARD	(BUS_BRIDGE_BOARD)
    #endif
#else
    #define	SYSTEM_DATA_SYNC_BOARD	(ETH3_BOARD)
#endif

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
#ifdef	VXWORKS
	#ifndef	BIT3_BOARD
		#define	BIT3_BOARD
	#endif
#endif
#endif


#if (BOARD_TYPE == MVME2100_BOARD)
	#define	INCLUDE_PMC661J_END
#elif ((BOARD_TYPE == MVME3100_BOARD)||(BOARD_TYPE == MVME4100_BOARD)||(BOARD_TYPE == MVME2500_BOARD))
	
	#define	INCLUDE_TSEC_END

	
	#ifdef INCLUDE_FEC_END
		#undef INCLUDE_FEC_END
	#endif	
#endif


#if ((BOARD_TYPE == MVME4100_BOARD)||(BOARD_TYPE == MVME2500_BOARD))
	#undef SYS_AUX_CLK_USED
#elif ((BOARD_TYPE == LKV120_BOARD))
	#define SYS_AUX_CLK_USED
#else
	#define SYS_AUX_CLK_USED
#endif


#define	BOARD_PARAMETER_NOT_USE 0


#ifdef VXWORKS
	#if (BOARD_TYPE == MVME2100_BOARD)
		#define	INCLUDE_XRSIO	1
		#define	XRSIO_DRV_DEBUG 1
	#endif
#endif 


#define	MAX16IOSHELF	(1)
#define	MAX8IOSHELF		(2)
#define	MAX12IOSHELF	(3)

#ifdef VXWORKS

#else
#define	MAXIOSHELF_CONFIG	(MAX16IOSHELF)
#endif


#define	INCLUDE_NET_INIT_IN_USR_APP

#ifdef VXWORKS
#define myBusDataAcc myVmeDataAcc
#endif

#define	MAX_PROTOCOL_DATA_LEN (3072) 

#define	MB_SLV_IF_DWORD_FIX		(1)
#define	MB_SLV_IF_WORD			(2)
#define	MB_SLV_IF_DWORD_WSWAP	(3)

#define	MB_SLAVE_INTERFACE	(MB_SLV_IF_WORD)

#ifndef MB_SLAVE_INTERFACE
	#error "You Should Define MB_SLAVE_INTERFACE!!!"
#endif

#endif 
