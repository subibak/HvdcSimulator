
#ifndef MEMDEF_H
#define MEMDEF_H



#include	"vmeDef.h"


#define		MAX_LOGIC_TASK_NUM			(16)

#if (MAXIOSHELF_CONFIG == MAX16IOSHELF)
#define		MAX_IO_SEG_NUM			(16)
#elif (MAXIOSHELF_CONFIG == MAX8IOSHELF)
#define		MAX_IO_SEG_NUM			(8)
#elif (MAXIOSHELF_CONFIG == MAX12IOSHELF)
#define		MAX_IO_SEG_NUM			(12)
#else 
	#error "You Should Define MAXIOSHELF_CONFIG(8 or 16)!!!!"
#endif
#define		MAX_TMTC_SEG_NUM		(15)
#define		MAX_FIELDBUS_SEG_NUM	(4)
#define		MAX_DIU_SEG_NUM			(MAX_IO_SEG_NUM+MAX_TMTC_SEG_NUM+MAX_FIELDBUS_SEG_NUM)


#define		MAX_DUAL_MEM_SIZE	(0x400)			
#define		MAX_SYS_MEM_SIZE	(0x400)		

#define		MEM_MPU_AUTOVAR_NUM		(16*1024) 	
#define 	MEM_MPU_MMEM_NUM		(16*1024)  


uint32  MAX_MEMORY_SIZE;
uint32	MAX_MPU_SPEC_NUM;
uint32  MAX_MPU_OUTPUT_NUM;

uint32	MAX_DIU_SPEC_NUM;
uint32	MAX_DIU_OUTPUT_NUM;

uint32	MAX_MPU_BLOCK_ADDR;


#define	MAX_DIU_BLOCK_ADDR_NUM	(2000)

uint32	MAX_DIU_BLOCK_ADDR;

uint32	VME_MEMORY_BAORD_SIZE;

uint32	MAX_MPU_MEM_SIZE;
uint32  MAX_MPU_DATA_SIZE;
uint32	MAX_DIU_MEM_SIZE;
uint32	MAX_USR_MEM_SIZE;

uint32	MAX_USABLE_MPU_MEM_SIZE;	
uint32	MAX_USABLE_DIU_MEM_SIZE;
uint32	MAX_USABLE_TMTC_MEM_SIZE;
uint32	MAX_USABLE_FIELDBUS_MEM_SIZE;
	
uint8	*MEMORY_BOARD_LOCAL_ADDR;
uint8	*MEMORY_BOARD_BUS_EMUL_ADDR;
	
uint32	MPU_FB_MEMORY_START_BUS_ADDR;
uint32	DIU_FB_MEMORY_START_BUS_ADDR;
#ifdef _WIN32
uint8	*DEBUG_LOCAL_MEMORY_ADDR_1;
uint8	*DEBUG_LOCAL_MEMORY_ADDR_2;
#endif

uint32 FIRST_FATAL_ERR_MNGR_OFFSET;
uint32 FATAL_MSG_START_OFFSET;



#define		MAX_DIU_SEG_CTRL_FB_OUTPUT_NUM	(16)
#define		MAX_TMTC_COMM_BLOCK_NUM			(40)	


#define		MAX_IO_OUTPUT_NUM		(	(MAX_DIU_SEG_CTRL_FB_OUTPUT_NUM*MAX_IO_SEG_NUM) + \
										(MAX_IO_SEG_NUM*MAX_IO_CARD_NUM*MAX_IO_POINT_NUM)	)
#define		MAX_TMTC_OUTPUT_NUM		(	(MAX_DIU_SEG_CTRL_FB_OUTPUT_NUM*MAX_TMTC_SEG_NUM) + \
										(MAX_TMTC_SEG_NUM*MAX_TMTC_COMM_BLOCK_NUM*MAX_IO_POINT_NUM) )   


#define		MAX_FIELDBUS_OUTPUT_NUM	(50000)	


#define	SHARED_MEMORY_MAX_BLOCK_SIZE		(256)
#define	SHARED_MEMORY_FIX_MAX_BLOCK_NUM		(32)
#define	SHARED_MEMORY_EXT_MAX_BLOCK_NUM		(4)

#define	SHARED_MEMORY_MAX_BLOCK_NUM			(SHARED_MEMORY_FIX_MAX_BLOCK_NUM+SHARED_MEMORY_EXT_MAX_BLOCK_NUM) 
#define	SHARED_MEM_MAX_BLOCK_ID				SHARED_MEMORY_MAX_BLOCK_NUM
#define	SHARED_MEM_BLOCK_BYTES				(SHARED_MEMORY_MAX_BLOCK_SIZE * 4)

#define	MAX_RCS_SHARED_MEM_NUM		(SHARED_MEMORY_FIX_MAX_BLOCK_NUM * SHARED_MEMORY_MAX_BLOCK_SIZE)	
#define	MAX_RCS_SHARED_EXTMEM_NUM	(SHARED_MEMORY_EXT_MAX_BLOCK_NUM * SHARED_MEMORY_MAX_BLOCK_SIZE)			
#define	MAX_SHARED_MEM_NUM	 (MAX_RCS_SHARED_MEM_NUM +MAX_RCS_SHARED_EXTMEM_NUM )			
													

#define	MAX_RCS_SHARED_MEM_SIZE (MAX_RCS_SHARED_MEM_NUM*4)		
													
#define	MAX_RCS_SHARED_EXTMEM_SIZE (MAX_RCS_SHARED_EXTMEM_NUM*4)
													
#define	MAX_SHARED_MEM_SIZE	\
		 (MAX_RCS_SHARED_MEM_SIZE+MAX_RCS_SHARED_EXTMEM_SIZE)		
													


uint32		MAX_MPU_AUTOVAR_NUM;
uint32		MAX_MPU_MMEM_NUM;
uint32		MAX_MPU_RETAIN_NUM;
uint32		MAX_MPU_RETAIN_FLAG_MEM_SIZE;
uint32      MAX_MPU_VAR_SIZE;


#define	LOCAL_MEM_ACC	0x1
#define	BUS_MEM_ACC		0x2
#define	RMT_MEM_ACC		0x3


#define MAX_STEP_NUM		(128)
#define MAX_TRANSITOIN_NUM  (128)
#define MAX_ACTION_NUM		(128)

#if (BOARD_TYPE == MVME2100_BOARD)
#define	MAX_FB_EXEC_SEQ_ID_NUM	(20000)   
#else
#define	MAX_FB_EXEC_SEQ_ID_NUM	(40000)   
#endif
#define	MAX_FB_EXEC_SEQ_ID	MAX_FB_EXEC_SEQ_ID_NUM 	
	
	


#define	COMPARE_CANCEL							(0)
#define	COMPARE_BETWEEN_LOCAL_RAM_AND_LOCAL_BOARD		(1)
#define	COMPARE_BETWEEN_LOCAL_RAM_AND_REMOTE_BOARD	(2)
#define	COMPARE_BETWEEN_LOCAL_BOARD_AND_REMOTE_BOARD	(3)

#define	LOCAL_DEBUGGING_MEMORY_REGION	(1)
#define	REMOTE_DEBUGGING_MEMORY_REGION	(2)


#define MAX_ENG_FILE_SIZE		(1024*20)		
#endif 
