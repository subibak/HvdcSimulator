
#include	<string.h>
#include	"usrTypes.h"
#include	"sysDefs.h"
#include	"usrErrCode.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"


void errorStringGet(uint32 , uint8 *);

extern void memoryCopy ( uint8 *, uint8 *, uint32 );


static uint8 unKnownErrCode[] = "Unknown Error Code";
static uint8 unKnownErrType[] = "Unknown Error Type";


#define	MAX_PROTOCOL_ERR_NUM	(0xA3)
static uint8 *protcolErrString[MAX_PROTOCOL_ERR_NUM+1] = {
(uint8 *)"NO Error",
(uint8 *)"L_NET_MSG_LEN_ERR",					
(uint8 *)"L_SRC_STATION_ID_ERR",				
(uint8 *)"L_DEST_STATION_ID_ERR",				
(uint8 *)"L_MSG_TYPE_ERR",						
(uint8 *)"L_DUAL_ID_ERR",						
(uint8 *)"L_MODULE_ID_ERR",						
(uint8 *)"L_OP_CODE_ERR",						
(uint8 *)"L_OPTION_CODE_ERR",					
(uint8 *)"L_ITEM_NO_ERR",						
(uint8 *)"L_FB_ADDR_ERR",						
(uint8 *)"L_FB_CODE_ERR",						
(uint8 *)"L_PARA_NO_ERR",						
(uint8 *)"L_PARA_TUNE_ERR",						
(uint8 *)"L_PCS_MODE_ERR",						
(uint8 *)"L_SEG_ID_ERR",						
(uint8 *)"L_MEM_SIZE_ERR",						
(uint8 *)"L_TIME_ERR",							
(uint8 *)"L_COM_CHECKSUM_ERR",					
(uint8 *)"L_PATH_ERR",							
(uint8 *)"SEG_MEM_EXCEED_ERR",					
(uint8 *)"SEG_BLK_ADDR_EXCEED_ERR",				
(uint8 *)"FB_SPEC_MEM_EXCEED_ERR",				
(uint8 *)"FB_NUM_EXCEED_ERR",					
(uint8 *)"FB_ADD_PERMISSON_ERR",				
(uint8 *)"ONLINE_FB_CODE_MISMATCH_ERR",			
(uint8 *)"FB_DEL_PERMISSION_ERR",				
(uint8 *)"MAX_FB_BLK_ADDR_EXCEED_ERR",			
(uint8 *)"FB_SEQ_ID_MISMATCH_ERR",				
(uint8 *)"FB_SEQ_ID_OVERLAP_ERR",				
(uint8 *)"FB_SEQ_ID_MAX_EXCEED_ERR",			
(uint8 *)"FB_CODE_NUM_MISMATCH_ERR",			
(uint8 *)"L_SIM_SEGID_ERR",						
(uint8 *)"L_SIM_FBADDR_ERR",					
(uint8 *)"L_SIM_FBCODE_ERR",					
(uint8 *)"L_SIM_CHECK_ERR",						
(uint8 *)"L_SIM_FLAG_ERR",						
(uint8 *)"L_SIM_FB_CODE_NUM_ERR",				
(uint8 *)"L_SIM_ACC_METHOD_ERR",				
(uint8 *)"ONLINE_DWL_FRAME_NUM_ERR",			
(uint8 *)"ONLINE_DWL_KIND_ERR",					
(uint8 *)"ONLINE_DWL_SEG_EXIST_ERR",			
(uint8 *)"L_MISMATCH_WITH_REMOTE_DEVICE",		
(uint8 *)"L_NO_PERMIT_REMOTE_DOWNLOAD",			
(uint8 *)"L_NO_MAPPING_DEVICE",					
(uint8 *)"L_UNKNOWN_REMOTE_DEVICE",				
(uint8 *)"L_FILE_SIZE_EXCEED_ERR",				
(uint8 *)"L_MISMATCH_CH_ID_ERR",				
(uint8 *)"L_TIME_TBL_DATA_ERR",					
(uint8 *)"L_EVENT_QUEUE_FULL_ERR",				
(uint8 *)"L_GET_SEMA_ERR",						
(uint8 *)"L_REMOTE_DOWNLOAD_ERR",				
(uint8 *)"L_DATA_ACC_ERR",						
(uint8 *)"L_TIME_TBL_DWL_ERR",					
(uint8 *)"L_CH_CONFIG_ERR",						
(uint8 *)"ONLINE_FB_CODE_NUM_MISMATCH_ERR",		
(uint8 *)"SPEC_FREE_SPECE_NULL_ERR",			
(uint8 *)"FB_SPEC_MEM_REALLOC_ERR",				
(uint8 *)"FB_SEQ_ID_NULL_ERR",					
(uint8 *)"FB_LOGIC_UPDATE_FLAG_ERR",			
(uint8 *)"FB_LOGIC_UPDATE_ERR",					
(uint8 *)"FB_ONLINE_DOWNLOAD_ERR",				
(uint8 *)"ONLINE_DWL_SEG_NUM_ERR",				
(uint8 *)"ON_DWL_FRAME_NUM_MISMATCH_ERR",	
(uint8 *)"FB_SPEC_CHG_PERMISSION_ERR",			
(uint8 *)"REQ_DATA_ERR",						
(uint8 *)"VERSION_INFO_DATA_ERR",				
(uint8 *)"VERSION_INFO_WRITE_ERR",				
(uint8 *)"VERSION_INFO_READ_ERR",				
(uint8 *)"SEG_EXECUTION_MODE_ERR",				
(uint8 *)"SEG_EXECUTION_ID_ERR",				
(uint8 *)"FB_OUTPUT_NUM_ERR",					
(uint8 *)"SEG_PRIORITY_ERR",					
(uint8 *)"EVENT_TASK_SET_ERR",					
(uint8 *)"FB_SPEC_NUM_ERR",						
(uint8 *)"MSG_COMPANY_ID_ERR",					
(uint8 *)"FBC_NOT_INSTALLED_ERR",				
(uint8 *)"NOT_SUPPORT_COMMAND_ERR",				
(uint8 *)"FBC_NOT_READY_ERR",					
(uint8 *)"INVALID_CARD_DETECTED_ERR",			
(uint8 *)"L_ACTION_ID_ERR",						
(uint8 *)"L_TRANSITION_ID_ERR",					
(uint8 *)"L_STEP_ID_ERR",						
(uint8 *)"L_LOGIC_ID_ERR",						
(uint8 *)"L_ACTION_FLAG_ID_ERR",				
(uint8 *)"L_SFC_LINK_ID_ERR",					
(uint8 *)"L_MAX_FATAL_ID_ERR",					
(uint8 *)"L_EXEINFO_EXCEED_ERR",				
(uint8 *)"L_PARA_TYPE_ERR",						
(uint8 *)"FB_OUTPUT_MEM_EXCEED_ERR",			
(uint8 *)"OUTPUT_FREE_SPECE_NULL_ERR",			
(uint8 *)"FB_OUTPUT_MEM_REALLOC_ERR",			
(uint8 *)"ONLINE_DWL_VAR_NUM_ERR",				
(uint8 *)"VAR_NUM_EXCEED_ERR",					
(uint8 *)"TRS_NUM_EXCEED_ERR",					
(uint8 *)"ONLINE_VAR_RETAIN_MISMATCH_ERR",		
(uint8 *)"ONLINE_VAR_INIT_MISMATCH_ERR",		
(uint8 *)"ONLINE_SEG_CHECKSUM_ERR",				
(uint8 *)"SEG_NUM_EXCEED_ERR",					
(uint8 *)"SIM_IO_POINT_ERR",					
(uint8 *)"FB_SPEC_ACTION_REF_ERR",				
(uint8 *)"FB_SPEC_ACTION_TYPE_ERR",				
(uint8 *)"FB_SPEC_ACTION_SIZE_ERR",				
(uint8 *)"FB_SPEC_ACTION_ADDR_ERR",				
(uint8 *)"FB_SPEC_ACTION_BITPOS_ERR",			
(uint8 *)"FB_SPEC_ACTION_TASKID_ERR",			
(uint8 *)"FB_SPEC_TYPE_SIZE_ERR",				
(uint8 *)"FB_TASKID_MISMATCH_ERR",				
(uint8 *)"FB_CODE_OUTPUT_INDEX_ERR",			
(uint8 *)"FB_CODE_OUTPUT_TYPE_ERR",				
(uint8 *)"FB_CODE_OUTPUT_ACTION_ERR",			
(uint8 *)"FB_CODE_SPEC_TYPE_ERR",				
(uint8 *)"VARIABLE_INDEX_ERR",					
(uint8 *)"VARIABLE_KIND_ERR",					
(uint8 *)"VARIABLE_RETAIN_ERR",					
(uint8 *)"VARIABLE_TYPE_ERR",					
(uint8 *)"VARIABLE_ADDR_RANGE_ERR",				
(uint8 *)"ANY_FUNC_OUT_SIZE_ERR",				
(uint8 *)"ANY_FUNC_OUT_TYPE_ERR",				
(uint8 *)"FB_SPEC_ACTION_INDEX_PTR_ERR",		
(uint8 *)"FB_SPEC_INDEX_PTR_ERR",				
(uint8 *)"FB_VAR_INDEX_PTR_ERR",				
(uint8 *)"FB_OUTPUT_INDEX_PTR_ERR",				
(uint8 *)"L_ACTION_VAR_SET_ERR",				
(uint8 *)"FRAME_KIND_ERR",						
(uint8 *)"INSUFFICENT_FREE_MEMORY_ERR",			
(uint8 *)"SRC_DWL_INDEX_ERR",					
(uint8 *)"BUSY_ACCESSING_ERR",					
(uint8 *)"FILE_NAME_LENGTH_ERR",				
(uint8 *)"FILE_EXIST_ERR",						
(uint8 *)"FILE_CREATE_ERR",						
(uint8 *)"DISK_FREE_SIZE_GET_ERR",				
(uint8 *)"FILE_WRITE_ERR",						
(uint8 *)"FILE_READ_ERR",						
(uint8 *)"FILE_DELETE_ERR",						
(uint8 *)"FILE_CLOSE_ERR",						
(uint8 *)"SYS_MAX_EXCCEED_ERR",					
(uint8 *)"SIZE_BITPOS_MISMATCH_ERR",			
(uint8 *)"ROM_UPDATE_MODE_ERR",					
(uint8 *)"HART_DEVICE_ID_ERR",					
(uint8 *)"HART_DEVICE_STATUS_ERR",				
(uint8 *)"DUAL_CMD_PERMISSION_ERR",				
(uint8 *)"HART_DATA_LENG_ERR",					
(uint8 *)"FB_CODE_SUPPORT_ERR",					
(uint8 *)"MAX_HST_NUM_EXCEED_ERR",				
(uint8 *)"REQ_STAION_ID_MISMATCH_ERR",			
(uint8 *)"PROFI_SHELF_NUM_ERR",					
(uint8 *)"FMC_CARD_SLOT_ERR",					
(uint8 *)"FMC_SHARED_MEM_ADDR_ERR",				
(uint8 *)"PROFI_SCAN_WORD_TYPE_ERR",			
(uint8 *)"PROFI_SCAN_WORD_NUM_ERR",				
(uint8 *)"FIELDBUS_FB_CODE_ID_ERR",				
(uint8 *)"OUT_OF_RANGE_ERR",					
(uint8 *)"MULTI_PROFI_CNFG_INFO_CRASH_ERR",		
(uint8 *)"MULTI_PROFI_FLAG_SET_ERR",			
(uint8 *)"RESP_ITEM_NUM_ERR",					
(uint8 *)"REQ_SHELF_ID_MISMATCH_ERR",			
(uint8 *)"FMC_DUAL_CFG_DISABLE_ERR",			
(uint8 *)"PCU_NOT_IN_RUN_ERR",					
(uint8 *)"FMC_CTRL_PERMISSION_ERR",				
(uint8 *)"PCU_IP_UNDEFINE_ERR",					
(uint8 *)"FMC_SCANBLK_NOT_UPDATE_ERR",			
(uint8 *)"FMC_ACCESS_PERMIT_ERR",				
(uint8 *)"PROFI_SHELF_PACKET_ID_ERR"			
};


#define	MAX_FUNC_ARGU_ERR_NUM 0x10
static uint8 *funcArguErrString[MAX_FUNC_ARGU_ERR_NUM+1] = {
(uint8 *)"NO Error",
(uint8 *)"SYS_STATUS_FLAG_ERR",						
(uint8 *)"SH_MEM_BLOCK_ID_ERR",			
(uint8 *)"IO_CARD_KIND_ERR",			
(uint8 *)"IO_SHELF_NUM_ERR",			
(uint8 *)"IO_CARD_NUM_ERR",				
(uint8 *)"IO_POINT_NUM_ID_ERR",			
(uint8 *)"IO_CARD_NUM_MISMATCH_ERR",	
(uint8 *)"IO_SHELF_NUM_MISMATCH_ERR",	
(uint8 *)"DATA_LEN_EXCEED_ERR",			
(uint8 *)"STATUS_READ_ID_ERR",			
(uint8 *)"COM_PORT_NUM_ERR",			
(uint8 *)"FB_CODE_ID_ERR",				
(uint8 *)"BCD_CONV_LEN_ERR",			
(uint8 *)"DUAL_STATUS_FLAG_ERR",		
(uint8 *)"XR_DATA_KIND_ERR",			
(uint8 *)"DATA_INVALID_ERR"				
};


#define	MAX_APP_ACCESS_ERR_NUM	0x8d
static uint8 *appAccessErrString[MAX_APP_ACCESS_ERR_NUM+1] = {
(uint8 *)"NO Error",
(uint8 *)"SEG_GROUP_NUM_EXCEED_ERR",					
(uint8 *)"SEG_GROUP_MODE_ERR",						
(uint8 *)"FC_SPEC_NUM_ERR",							
(uint8 *)"FC_VAR_NUM_ERR",							
(uint8 *)"FC_OUTPUT_NUM_ERR",						
(uint8 *)"FC_CODE_EXIST_ERR",						
(uint8 *)"MPU_MEM_SIZE_EXCEED_ERR",					
(uint8 *)"DIU_MEM_SIZE_EXCEED_ERR",					
(uint8 *)"PARA_SIZE_EXCEED_ERR",					
(uint8 *)"OUTPUT_SIZE_EXCEED_ERR",					
(uint8 *)"FB_PARA_DEFINE_ERR",						
(uint8 *)"FB_INIT_FUNC_CALL_ERR",					
(uint8 *)"FB_PARA_INDEX_ERR",						
(uint8 *)"FB_CODE_MISMATCH_ERR",					 
(uint8 *)"FB_CODE_SPEC_INDEX_ERR",					 
(uint8 *)"COMMAND_SUPPORT_ERR",						
(uint8 *)"SYS_OPERATION_MODE_ERR",					
(uint8 *)"MEM_ADDR_NULL_ERR",						
(uint8 *)"FB_TYPE_TABLE_ERR",						
(uint8 *)"TIME_DATA_ERR",							
(uint8 *)"DATA_LEN_ERR",							
(uint8 *)"PUMP_DWL_PATH_ERR",						
(uint8 *)"PUMP_DWL_PATH_COUNTER_ERR",				
(uint8 *)"QUEUE_FULL_ERR",							
(uint8 *)"SEG_EXEC_MODE_ERR",						
(uint8 *)"SCHD_INFO_DEL_ERR",						
(uint8 *)"CYCLIC_TIME_ID_ERR",						
(uint8 *)"FB_RUNNING_OVERTIME_ERR",					
(uint8 *)"FB_CODE_DEFINE_ERR",						
(uint8 *)"INPUT_NULL_POINT_ERR",					
(uint8 *)"NO_EXIST_NODE_ERR",						
(uint8 *)"FB_BLOCK_ADDR_RANGE_ERR",					
(uint8 *)"DUAL_SAME_MSG_ERR",						
(uint8 *)"DLM_LIVE_COUNT_ERR",						
(uint8 *)"DUAL_LINE_PORT_INFO_ERR",					
(uint8 *)"DUAL_LINE_QUEUE_CONTENT_ERR",				
(uint8 *)"MULTICAST_SUPPORT_ERR",					
(uint8 *)"NETWORK_LINE_KIND_ERR",					
(uint8 *)"STATION_ID_RANGE_ERR",					
(uint8 *)"IP_ADDR_SET_ERR",							
(uint8 *)"IP_ADDR_GET_ERR",							
(uint8 *)"SOCK_FD_ERR",								
(uint8 *)"SOCK_PARA_ERR",							
(uint8 *)"SOCK_OPTSET_ERR",							
(uint8 *)"GATEWAY_ADD_ERR",							
(uint8 *)"SOCK_ACCEPT_ERR",							
(uint8 *)"SOCK_OPEN_ERR  ",							
(uint8 *)"SOCK_CONNECT_ERR  ",						
(uint8 *)"SOCK_BIND_ERR  ",							
(uint8 *)"ETH_PROT_TYPE_ERR ",						
(uint8 *)"SOCK_SEND_ERR",							
(uint8 *)"SOCK_SEND_LEN_ERR ",						
(uint8 *)"SOCK_LISTEN_ERR ",						
(uint8 *)"SOCK_RECV_ERR",							
(uint8 *)"IP_ADDR_INVALID_ERR",						
(uint8 *)"QUE_DATA_LEN_ERR",						
(uint8 *)"MSG_GARBAGE_DATA",						
(uint8 *)"MSG_MY_BROADCAST_DATA",					
(uint8 *)"MSG_LEN_EXCEED_ERR",						
(uint8 *)"MSG_LEN_INVALID_ERR",						
(uint8 *)"MSG_TYPE_ERR",							
(uint8 *)"MSG_MODULE_ID_ERR",						
(uint8 *)"MSG_CHECKSUM_ERR",						
(uint8 *)"MSG_OPCODE_ERR",							
(uint8 *)"FB_START_ADDR_NULL_ERR",					
(uint8 *)"CYCLE_TIME_ID_ERR",						
(uint8 *)"FB_PARA_MISMATCH_ERR",					
(uint8 *)"QUEUE_CRASH_ERR",							
(uint8 *)"BUS_DATA_ACC_TYPE_ERR",					
(uint8 *)"BUS_ACC_TYPE_ERR",						
(uint8 *)"BUS_DATA_ACC_ERR",						
(uint8 *)"PCI_ADDR_ERR",							
(uint8 *)"SYS_DUAL_CMD_ERR",						
(uint8 *)"SYS_RUNNING_MODE_ERR",					
(uint8 *)"DUAL_COPY_STOP",							
(uint8 *)"IP_ADDR_NULL_ERR",						
(uint8 *)"DB_INDEX_EXCEED_ERR",						
(uint8 *)"MEMORY_CRASH_ERR",						
(uint8 *)"STBY_LIVE_COUNT_ERR",						
(uint8 *)"RUN_LIVE_COUNT_ERR",						
(uint8 *)"CTRL_NET_COM_ERR",						
(uint8 *)"FB_IO_LOOP_NUM_ERR",						
(uint8 *)"RMT_CTRL_NET_COM_ERR",					
(uint8 *)"APP_TASK_SUSPEND_ERR",					
(uint8 *)"MFM_DATA_ACC_ERR",						
(uint8 *)"MFM_LIVECOUNT_ERR",						
(uint8 *)"SIM_FB_NUM_MISMATCH_ERR",					
(uint8 *)"SIM_FB_NUM_EXCEED_ERR",					
(uint8 *)"FB_PARA_MEM_REALLOC_NO_NEED",				
(uint8 *)"FB_ONLINE_OP_CODE_ERR",			
(uint8 *)"FB_UPDATE_RESP_TIMEOUT_ERR",	
(uint8 *)"FB_MEM_REALLOC_TIMEOUT_ERR",	
(uint8 *)"PUMP_TBL_FILE_ID_NULL_ERR",				
(uint8 *)"APP_SEG_CHECKSUM_ERR",					
(uint8 *)"TMTC_MEM_SIZE_EXCEED_ERR",				
(uint8 *)"DUAL_MEM_SIZE_EXCEED_ERR",				
(uint8 *)"FIELDBUS_MEM_SIZE_OVER_ERR",			
(uint8 *)"MEM_ACC_TYPE_ERR",						
(uint8 *)"MAX_MEM_SIZE_EXCEED_ERR",					
(uint8 *)"DUAL_MSG_PROCESS_TYPE_ERR",				
(uint8 *)"FB_SPEC_DATA_RANGE_ERR",					
(uint8 *)"VARIABLE_SIZE_ERR",						 
(uint8 *)"JUMP_FB_CODE_ERR",						
(uint8 *)"PUMP_NUM_MISMATCH_ERR",					
(uint8 *)"NO_RUN_PARA_TYPE_ERR",					
(uint8 *)"FILE_DATA_ACCESS_ERR",					
(uint8 *)"TRS_SEQUENCER_ERR",						
(uint8 *)"STEP_SEQUENCER_ERR",						
(uint8 *)"ACTION_TIME_FLAG_ERR",					
(uint8 *)"SFC_MEM_GET_ERR",							
(uint8 *)"SFC_ACTIVE_FLAG_ERR",						
(uint8 *)"SFC_INIT_STEP_ERR",						
(uint8 *)"SFC_VERTEX_ID_ERR	",						
(uint8 *)"SFC_SEQUENCER_LINK_ERR",					
(uint8 *)"SFC_TYPE_ID_ERR",							
(uint8 *)"SFC_ACTION_FLAG_ID_ERR",					
(uint8 *)"SFC_LINK_TRANSITION_ERR",					
(uint8 *)"TRS_ACTIVESTATUS_ERR",					
(uint8 *)"RAS_TYPE_ERR",							
(uint8 *)"SYS_DATA_WRITE_ERR",						
(uint8 *)"JUMP_DEST_ADDR_ERR",						
(uint8 *)"WDG_TIME_OUT_ERR",						
(uint8 *)"CTRL_NET_COM_ERR_RECOVER",				
(uint8 *)"SYSTEM_EXCEPTION_ERR",					
(uint8 *)"HART_RESP_TIMEOUT_ERR",					
(uint8 *)"PERIODIC_EXEC_OVERTIME_ERR",				
(uint8 *)"PERIODIC_EXEC_UNDERTIME_ERR",				
(uint8 *)"FMC_HOST_REDAY_ERR",						
(uint8 *)"FMC_IO_COMM_ERR",							
(uint8 *)"FMC_LIVECNT_ERR",							
(uint8 *)"NETWORKS_LINK_ERR",						
(uint8 *)"HART_BRD_COMM_ERR	",						
(uint8 *)"RMT_SHELF_COMM_CHK_ERR",					
(uint8 *)"RMT_SHELF_ALIVE_BY_COMM_CHK",				
(uint8 *)"CMD_NOT_EXEC",							
(uint8 *)"PEER_IP_ADDR_ERR",
(uint8 *)"FB_CODE_DUAL_DEFINE_ERR",
(uint8 *)"IO_BOARD_ACCESS_ERR",
(uint8 *)"IO_BOARD_MISMATCH_ERR",
(uint8 *)"IO_BOARD_READY_ERR",
(uint8 *)"IO_BOARD_LIVE_COUNT_ERR"				
};


#define	MAX_SYS_CALL_ERR_NUM	0x20
static uint8 *sysCallErrString[MAX_SYS_CALL_ERR_NUM+1] = {
(uint8 *)"NO Error",
(uint8 *)"MEMORY_ALLOC_ERR",			
(uint8 *)"SYSTEM_TIME_READ_ERR",		
(uint8 *)"SYSTEM_TIME_WRITE_ERR",		
(uint8 *)"INVALID_SEMA_TYPE_ERR",		
(uint8 *)"MSG_QUE_CREATE_ERR",			
(uint8 *)"MSG_QUE_SEND_ERR",			
(uint8 *)"MSG_QUE_RECV_ERR",			
(uint8 *)"SIG_SET_ERR",					
(uint8 *)"HW_INTERRUPT_SET_ERR",		
(uint8 *)"WDG_START_ERR",				
(uint8 *)"IO_MUX_ERR",					
(uint8 *)"MSG_LOG_PORT_ERR",			
(uint8 *)"IO_OPEN_ERR",					
(uint8 *)"LOG_INIT_ERR",				
(uint8 *)"TASK_NAME_GET_ERR",			
(uint8 *)"TASK_INFO_GET_ERR",			
(uint8 *)"TIMESLICE_ENABLE_ERR",		
(uint8 *)"BROADCAST_ADDR_GET_ERR",		
(uint8 *)"SUB_NET_MASK_GET_ERR",		
(uint8 *)"BUS_ACCESS_PERMIT_ERR",		
(uint8 *)"TIME_SYSCALL_ERR",			
(uint8 *)"IP_PACK_INSTALL_ERR",			
(uint8 *)"IO_CTRL_ERR",					
(uint8 *)"WDG_CREATE_ERR",				
(uint8 *)"USR_TASK_DELETE_ERR",			
(uint8 *)"SEMA_TIMEOUT_ERR",			
(uint8 *)"TASK_STATUS_CHANGE_ERR",		
(uint8 *)"MSG_QUE_INFO_READ_ERR",		
(uint8 *)"SYSTEM_ISR_ERR",				
(uint8 *)"SEMA_RELEASE_ERR",			
(uint8 *)"TASK_SUSPENDED_ERR",			
(uint8 *)"TASK_NUMBER_ERR"				
};


#define	MAX_SYS_BOOT_ERR_NUM	0x22
static uint8 *sysBootingErrString[MAX_SYS_BOOT_ERR_NUM+1] = {
(uint8 *)"NO Error",
(uint8 *)"STATION_SHELF_CONFIG_ERR",	
(uint8 *)"SOE_BOARD_CONFIG_ERR",		
(uint8 *)"MPU_BOARD_CONFIG_ERR",		
(uint8 *)"TMTC_BOARD_CONFIG_ERR",		
(uint8 *)"IO_SHELF_CONFIG_ERR",			
(uint8 *)"TARGET_IP_ADDR_ERR",			
(uint8 *)"IP_ADDR_MAKING_FLAG_ERR",			
(uint8 *)"MPU_SOE_ENABLE_ERR",			
(uint8 *)"NVRAM_PARA_ERR",				
(uint8 *)"BOARD_ETH_DEV_INIT_ERR",		
(uint8 *)"NVRAM_ADDR_EXCEED_ERR",		
(uint8 *)"NVRAM_SIZE_EXCEED_ERR",		
(uint8 *)"BOARD_POWER_ON_RESET",		
(uint8 *)"MEMORY_BOARD_INSTALL_ERR",	
(uint8 *)"DUAL_PARA_CONFIG_ERR",		
(uint8 *)"VME_BOARD_PROBE_ERR",			
(uint8 *)"SIM_FLAG_CONFIG_ERR",			
(uint8 *)"SYS_FORCED_REBOOT",			
(uint8 *)"ONLINE_DWL_FLAG_CONFIG_ERR",	
(uint8 *)"MEMORY_BOARD_TYPE_ERR",		
(uint8 *)"FIELDBUS_TYPE_CONFIG_ERR",			
(uint8 *)"FIELDBUS_CNFG_DWL_FLAG_ERR",		
(uint8 *)"DLU_BOARD_CONFIG_ERR",		
(uint8 *)"MPU_CPU_TYPE_CONFIG_ERR",		
(uint8 *)"SCHDULER_TYPE_ERR",			
(uint8 *)"FMC_BOARD_INSTALL_ERR",		
(uint8 *)"TMTC_SERIAL_BRD_CONFIG_ERR",	
(uint8 *)"TMTC_SIO_BOARD_INSTALL_ERR",	
(uint8 *)"FMC_BOARD_CONFIG_ERR",		
(uint8 *)"HVDC_CPU_TYPE_CONFIG_ERR",	
(uint8 *)"NSIO_MODUE_CONFIG_ERR",   	
(uint8 *)"MB_SLV_IF_CONFIG_ERR",	    
(uint8 *)"DLU_IF_TYPE_CONFIG_ERR",	    
(uint8 *)"DLU_INSTALL_ERR"	            
};

#define	MAX_SYS_HISTORY_ERR_NUM	0x19
static uint8 *sysHistoryErrStrig[MAX_SYS_HISTORY_ERR_NUM+1] = {
(uint8 *)"NO Error",
(uint8 *)"LOCAL_STANDBY",							
(uint8 *)"LOCAL_RUN",								
(uint8 *)"RMT_RESET_LOCAL_RUN",						
(uint8 *)"RMT_POWER_OFF",							
(uint8 *)"RMT_POWEROFF_AND_LOCAL_RUN",				
(uint8 *)"RMT_RUN_BY_LOCAL_ERR",					
(uint8 *)"LOCAL_RUN_BY_RMT_ONLINE_DWL_UPDATE_ERR",	
(uint8 *)"LOCAL_STANDBY_BY_ONLINE_DWL_UPDATE_ERR",	
(uint8 *)"LOCAL_STOP_BY_ONLINE_DWL_UPDATE_ERR",		
(uint8 *)"LOCAL_POWER_OFF",							
(uint8 *)"RMT_DUAL_MEM_COPY_ERR",					
(uint8 *)"MPU_SEG_CHKSUM_ERR",						
(uint8 *)"DIU_SEG_CHKSUM_ERR",						
(uint8 *)"RMT_RESET_AND_SFC_EXE_INFO_SET_ERR",		
(uint8 *)"DUAL_PROC_RECV_ERR",						
(uint8 *)"DUAL_SEND_QUEUE_RECV_ERR",				
(uint8 *)"DUAL_ETH_SEND_ERR",						
(uint8 *)"DUAL_SEND_QUEUE_SEND_ERR",				
(uint8 *)"RMT_DUAL_NET_IP_READ_ERR",				
(uint8 *)"STBY_RESET_REQ_WRITE_ERR",				
(uint8 *)"RMT_SHELF_POWER_OFF",						
(uint8 *)"RMT_SHELF_RESET",							
(uint8 *)"RMT_FMC_BOARD_ERR",						
(uint8 *)"LOCAL_STANDBY_BY_DLU_COM_ERR",			
(uint8 *)"RMT_RESET"								
};


#define	MAX_RMT_COM_ERR_NUM	0x15
static uint8 *rmtComErrString[MAX_RMT_COM_ERR_NUM+1] = {
(uint8 *)"NO Error",
(uint8 *)"RMT_DEVICE_TYPE_ERR",								
(uint8 *)"RMT_COMINFO_QUEUE_MISMATCH_ERR",		
(uint8 *)"RMT_COM_Q_LIVE_COUNT_ERR",				
(uint8 *)"RMT_DEV_UNKNOWN_MODE_ERR",				
(uint8 *)"RMT_DATA_ACC_TYPE_ERR",						
(uint8 *)"RMT_DATA_BYTE_MISMATCH_ERR",				
(uint8 *)"EXCEED_ETH_COM_FB_CODE_NUM_ERR",		
(uint8 *)"ETH_COM_DB_MISMATCH_ERR",						
(uint8 *)"RMT_ETH_COM_SEQ_NUM_ERR",					
(uint8 *)"RMT_COMMAND_ERR",										
(uint8 *)"FB_ADDR_NULL_ERR",								
(uint8 *)"RMT_ETH_DEV_LINE_CNFG_ERR",				
(uint8 *)"RMT_ETH_DEV_LINE_LINK_ERR",				
(uint8 *)"RMT_ETH_DEV_REDUNDANCY_ERR",				
(uint8 *)"RMT_ETH_DEV_DUAL_ERR",						
(uint8 *)"RMT_DATA_ACC_NUM_ERR",					
(uint8 *)"MEM_PERMISSION_ERR",						
(uint8 *)"MEM_ADDR_ERR",							
(uint8 *)"INVALID_BIN_NUMBER_ERR",					
(uint8 *)"RMT_DEV_CHECKSUM_ERR",					
(uint8 *)"RMT_DATA_LEN_EXCEED_ERR"
};


#define	MAX_FIELDBUS_GLOBAL_BIT_ERR1_NUM		0x41
static uint8 *fieldbusIfGblErr1String[MAX_FIELDBUS_GLOBAL_BIT_ERR1_NUM+1] = {
(uint8 *)"NO Error",									
(uint8 *)"FIELDBUS: Unknown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: Master Request Not Support By Slave",
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: Slave No Answer Data",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",					
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: Slave No Response",					
(uint8 *)"FIELDBUS: Master Not Token Ring",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",					
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",					
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: UnKnown Error Code",				
(uint8 *)"FIELDBUS: Usr Task Not Found",				
(uint8 *)"FIELDBUS: No Global Entry In DB",				
(uint8 *)"FIELDBUS: FDL Task Not Found",				
(uint8 *)"FIELDBUS: PLC Task Not Found",				
(uint8 *)"FIELDBUS: None Exsist Master Parameter",		
(uint8 *)"FIELDBUS: Fault Parameter In Master",			
(uint8 *)"FIELDBUS: None Exsist Slave Parameter",		
(uint8 *)"FIELDBUS: Fault Parameter In Slave Data File",
(uint8 *)"FIELDBUS: Slave Address Doubled",				
(uint8 *)"FIELDBUS: Max Output Size(256Bytes)",			
(uint8 *)"FIELDBUS: Max Input Size(256Bytes)",			
(uint8 *)"FIELDBUS: Output Data Area Overlaped",		
(uint8 *)"FIELDBUS: Input Data Area Overlaped",			
(uint8 *)"FIELDBUS: UnKnown Mode of Warm Start",			
(uint8 *)"FIELDBUS: Extended RAM Exeeded",				
(uint8 *)"FIELDBUS: Slave Data Set Fault",				
};
#define	MAX_FIELDBUS_GLOBAL_BIT_ERR2_NUM		0x6
static uint8 *fieldbusIfGblErr2String[MAX_FIELDBUS_GLOBAL_BIT_ERR2_NUM] = {
(uint8 *)"FIELDBUS: User Watchdog Failure",				
(uint8 *)"FIELDBUS: Mode No Acknowledge",				
(uint8 *)"FIELDBUS: Auto Clear Activated",				
(uint8 *)"FIELDBUS: Fatal Error Occured",               
(uint8 *)"FIELDBUS: UnKnown Event Detected",			
(uint8 *)"FIELDBUS: No Segment to Communicate",			
};

void errorStringGet(uint32 errorCode, uint8 *retString)
{
	uint32	errorType;
    uint32	errorNum;
	uint8 	*tmpPtr;

    errorType 	= errorCode & 0xffff0000;
    errorNum	= errorCode & 0x0000ffff;
    switch(errorType) {
		case PROTOCOL_ERR_TYPE:
        	if(errorNum > MAX_PROTOCOL_ERR_NUM) {
                tmpPtr = &unKnownErrCode[0];
            }
            else
            	tmpPtr = protcolErrString[errorNum]; 
        break;
		case FUNC_ARGU_ERR_TYPE:
        	if(errorNum > MAX_FUNC_ARGU_ERR_NUM) {
                tmpPtr = &unKnownErrCode[0];
            }
            else
            	tmpPtr = funcArguErrString[errorNum]; 
        break;
		case APP_ACCESS_ERR_TYPE:
        	if(errorNum > MAX_APP_ACCESS_ERR_NUM) {
                tmpPtr = &unKnownErrCode[0];
            }
            else
            	tmpPtr = appAccessErrString[errorNum]; 

        break;
		case SYSTEM_CALL_ERR_TYPE:
        	if(errorNum > MAX_SYS_CALL_ERR_NUM) {
                tmpPtr = &unKnownErrCode[0];
            }
            else
            	tmpPtr = sysCallErrString[errorNum]; 
        break;

		case BOOT_CONFIG_ERR_TYPE:
        	if(errorNum > MAX_SYS_BOOT_ERR_NUM) {
                tmpPtr = &unKnownErrCode[0];
            }
            else
            	tmpPtr = sysBootingErrString[errorNum]; 
        break;

        case FB_EXEC_ERR_GROUP:
        	if(errorNum > MAX_FB_EXEC_ERR_NUM) {
                tmpPtr = &unKnownErrCode[0];
            }
            else
            	tmpPtr = fbExecErrStringForDbg[errorNum]; 
        break;

        case SYS_HISTORY_TYPE:
        	if(errorNum > MAX_SYS_HISTORY_ERR_NUM) {
                tmpPtr = &unKnownErrCode[0];
            }
            else
            	tmpPtr = sysHistoryErrStrig[errorNum]; 
        break;

        case ETH_RMT_COM_ERR_TYPE:
        	if(errorNum > MAX_RMT_COM_ERR_NUM) {
                tmpPtr = &unKnownErrCode[0];
            }
            else
            	tmpPtr = rmtComErrString[errorNum]; 
        break;

        default:
			tmpPtr 		= &unKnownErrType[0];
		break;
    }

    
	memoryCopy(	retString,
       			tmpPtr,
                strlen((const char *)tmpPtr)
			  );
}

