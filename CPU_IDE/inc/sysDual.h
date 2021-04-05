
#ifndef SYSDUAL_H
#define SYSDUAL_H



#define	MAX_DUAL_ACC_ERR_PERMISSION	(2)

#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)
#define	MAX_DUAL_LIVE_COUNT_ERR		(10) 
										 
#define MPU_LIVE_TIME_IN_MSEC		(MAX_DUAL_LIVE_COUNT_ERR*100)
#endif


#define	DUAL_CHECK_DISABLE	(0x1)
#define	DUAL_CHECK_ENABLE	(0x2)

#define	DUAL_DATA_COPY_DISABLE	(0x1)
#define	DUAL_DATA_COPY_ENABLE	(0x2)


#define STANDBY_SHELF_STATUS_CHECK_DISABLE 	(1)
#define STANDBY_SHELF_STATUS_CHECK_ENABLE 	(2)


#define RUN_SHELF_STATUS_CHECK_DISABLE 	(1)
#define RUN_SHELF_STATUS_CHECK_ENABLE 	(2)


#define	RMT_STATUS_CHECK_CMD_SEND_ENABLE	(1)
#define	RMT_STATUS_CHECK_CMD_SEND_DISABLE	(2)


#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)
	#define RUN_SHELF_LIVECNT_SEND_DISABLE  (1)
	#define RUN_SHELF_LIVECNT_SEND_ENABLE   (2)
#endif


#define	REMOTE_SHELF_CONNECT	(0)
#define	REMOTE_SHELF_DISCONNECT	(1)


#define	DUAL_SPEC_WRITE		(1)
#define	DUAL_VAR_WRITE		(2)
#define	DUAL_OUTPUT_WRITE	(3)




#define	REDUNDANCY_ERR_CLR_CMD	(0x1)


#define	REMOTE_RUN_CMD	(0x2)



#define	CHG_AUTO_TO_MANUAL_CMD	(0x3)



#define	CHG_MANUAL_TO_AUTO_CMD	(0x4)



#define	CHG_RUN_TO_STANDBY_CMD	(0x5)






#define	SYS_DUAL_OPERATION_START	(0x6)



#define	ONLINE_DWL_CHG_MANUAL_TO_AUTO_CMD	(0x7)


#define	ONLINE_DWL_REMOTE_RUN_CMD		(0x8)


#define	RMT_FBCODE_FORCING_ON_OFF_CMD	(0x9)


#define	RMT_FBCODE_SPEC_DATA_WRITE_CMD	(0xa)



#define	RMT_POWER_OFF_CMD		(0xb)



#define	RMT_POWER_OFF_AND_LOCAL_RUN	(0xc)




#define	ONLINE_DWL_IOCHG_MANUAL_TO_AUTO_CMD	(0xd)


#define	RMT_RESET_CMD			(0xe)


#define	RMT_LIVE_CNT_ERR_CMD			(0xf)


#define	DUAL_CMD_ACC				(1)
#define	DUAL_SHELF_MODE_ACC			(2)
#define	DUAL_LIVE_CNT_ACC			(3)
#define	DUAL_MY_ERR_SUMMARY_ACC		(4)
#define	DUAL_SHELF_TYPE_ACC			(5)
#define	DUAL_STATION_ID_ACC			(6)
#define	DUAL_RMT_LIVE_CNT_ACC		(7)
#define	DUAL_IO_INIT_FLAG_ACC		(8)
#define	DUAL_IP_ADDR_A_ACC			(9)
#define	DUAL_IP_ADDR_B_ACC			(10)
#define	DUAL_RMT_ERR_SUMMARY_ACC	(11)
#define	DUAL_FMC_GLV_ERR_SUM_ACC	(12)	
#define	DUAL_FORCING_MPU_CNT_ACC	(13)	
#define	DUAL_FORCING_DIU_CNT_ACC	(14)	
#define	DUAL_MEM_BRD_TYPE_ACC		(15)	
#define	DUAL_SW_ENGINE_TYPE_ACC		(16)
#define	DUAL_FMC_DUAL_ENABLE_ACC	(17)
#define	DUAL_PCU_SHELF_ID_ACC		(18)


#define	MAX_DUAL_SIM_INFO_FBCODE_NUM	32

typedef struct {
	uint32	simFbCodeNum;

    uint32  moduleId;
	uint32  taskId;
    uint32  FBCode[MAX_DUAL_SIM_INFO_FBCODE_NUM];
    uint32  FBAddr[MAX_DUAL_SIM_INFO_FBCODE_NUM];
    uint32  pointId[MAX_DUAL_SIM_INFO_FBCODE_NUM];
	uint32  forcingFlag[MAX_DUAL_SIM_INFO_FBCODE_NUM];
}strDualFbCodeSimInfo;


typedef struct {
	uint32 	moduleId;
	uint32	fbBlockAddr; 
	uint32	fbId;
	uint32	writeIndex;
	uint32	writeSpecType;
	uint32	writeByteNum;
	uint8	writeData[8];
}strDualFbCodeSpecInfo;


typedef struct {
	uint32	dualCmd;			
    							
    uint32	shelfMode; 			
    uint32	shelfLiveCount;		
    uint32	rmtShelfErrSummary;	
    uint32	myShelfErrSummary;	
    uint32	shelfType;			
    uint32	stationId;			
    uint32	rmtShelfLiveCount;	
    							
                                
	uint32	vmeIOInitFlag;		
								
								
	uint32	myIpAddr_A;											
	uint32	myIpAddr_B;				
	uint32	fmcGblErrSummary;	 
														
	uint32	memoryBrdType;		
	uint32 forcingMpuFbCodeNum;
	uint32 forcingDiuFbCodeNum;
	union {
		strDualFbCodeSimInfo	sysDualSimInfo; 
		strDualFbCodeSpecInfo	sysDualSpecInfo;
	}u;
    uint32	sysSchedlueType;		
#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)
	uint32 shelfRedundancyStatus;  
#endif
}strSysDualInfo;

#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)
typedef struct {
	uint32	rmtLiveCount;		
    uint32	preLiveCount; 		
    uint32	liveCountRecvFlag;	
}strSysDualLiveCountInfo;
#endif


#define	DUAL_CTRL_NET_ERR			(1<<0) 
#define	DUAL_IO_COMM_ERR			(1<<1)
#define	DUAL_FMC_IO_COMM_ERR		(1<<2)		
#define	DUAL_FMC_LIVECNT_ERR		(1<<3)
#define	DUAL_FBC_LIVECNT_ERR		(1<<4)
#define	USR_TASK_SUSPEND_ERR		(1<<5)
#define	DUAL_FBC_BASEOUT_ERR	    (1<<6)


#define	MPU_DIU_DUAL_ACC	(0x1)
#define	MPU_ONLY_DUAL_ACC	(0x2)


#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)
#define		DUAL_ACCESS_CMD			(0x22)
 	#define DUAL_MEMORY_COPY	(0x01)  
 	#define DUAL_CONF_READ		(0x02)	
	#define DUAL_CONF_WRITE		(0x03)	
	#define DUAL_CMD_WRITE		(0x04)	
 	#define DUAL_LIVE_COUNT		(0x05)	
 	#define DUAL_RUNDATA_COPY	(0x06)	
 	#define DUAL_RUNTYPE_COPY	(0x07)	
 	#define DUAL_RUNBUS_COPY	(0x08)	

#define MAX_DUAL_COPY_DATA_NUM   (250) 
#define MAX_DUAL_COPY_TYPE_NUM   (150) 
#define MAX_DUAL_COPY_SEND_NUM   (2)   


#define MAX_DUAL_DATA_ITER   (10)
#define MAX_DUAL_EXE_ITER    ((MAX_DUAL_DATA_ITER)*2)

#define DUAL_DEFAULT_AREA		 (0x00)
#define MPU_FB_AREA				 (0x01)
#define VAR_DATA_AREA			 (0x02)
#define SFC_INFO_AREA			 (0x03)
#define SYS_EXE_AREA			 (0x04)
#define DIU_FB_AREA				 (0x05)
#define DUAL_INFO_AREA			 (0x06)
#define FERR_DATA_AREA			 (0x07)
#endif



#define	RMT_SHELF_CHK_REQUEST	(1)
#define	RMT_SHELF_CHK_RESPONSE	(2)


#define	RMT_SHELF_LIVE_CHK_CMD			(1)
#define	RMT_SHELF_CHK_ENABLE_CMD		(2)

#define	RMT_SHELF_NON_ACTIVE	(1)
#define	RMT_SHELF_ACTIVE		(2)


typedef	struct {
	uint32	msgType;
	uint32	dualMsgCmd;
	uint32	sendShelfType;
	uint32	shelfRunStatus;
}strMpuDualChkByNetInfo;


#endif 
