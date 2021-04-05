
#ifndef SYSCONFIG_H
#define SYSCONFIG_H


#define	ETHERNET_TYPE	(0x1)


#define	CTRL_NET_LINE_SINGLE	(0x1)
#define	CTRL_NET_LINE_DUAL		(0x2)


#define	NETWORK_A_LINE	(0x1)
#define	NETWORK_B_LINE	(0x2)

#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)
#define	DLU_SYS_LINE 	(0x3)
#define	DLU_RMT_LINE	(0x4)
#endif



#define	MANUAL_IPADDR_MAKE		(0x1)
#define	AUTO_IPADDR_MAKE		(0x2)


#if 0
#define	PMC_1_MODULE_USING		(0x1)
#define	PMC_2_MODULE_USING		(0x2)
#endif


#define USING_LKP410_GEI    (0x1)
#define USING_LKP420_GEI    (0x2)
#define USING_LKP428_GEI    (0x3)
#define USING_LKP414_RTG    (0x4)


#define DLU_IF_AUTO     (0x1)
#define DLU_IF_PMC_ETH  (0x2)
#define DLU_IF_VME_DLU  (0x3)
            

#define MASTER_SHELF    (0x1)
#define SLAVE_SHELF     (0x2)


#define	BOARD_DISABLE	(0x1)
#define	BOARD_ENABLE	(0x2)


#define FIELDBUS_IF_NONE		(0x1)
#define FIELDBUS_IF_FMC			(0x2)
#define FIELDBUS_IF_PROFI_SHELF	(0x3)
#define	INTR_ACC_DISABLE	(0x1)
#define	INTR_ACC_ENABLE		(0x2)


#define	FBC_NONE		(0x1)
#define	FBC_311_BOARD	(0x2)
#define FBC_351_BOARD	(0x3)
#define FBC_601_BOARD	(0x4)
#define FBC_352_BOARD	(0x5)
#define PROFIBUS_BOARD	(0x6) 
#define MAX_FBC_BOARD   PROFIBUS_BOARD


#define	MEM_BOARD_NONE	(0x0)
#define	MEM_611_BOARD	(0x1)
#define	MEM_612_BOARD	(0x2)
#define	MEM_613_BOARD	(0x3)	
#define	MEM_4MB_MRAM	(0x4)


#define	MFM_BOARD_NONE	(0x0)
#define	MFM_BOARD_902	(0x1)
#define	MFM_BOARD_HMEM	(0x2)


#define	SIO_VME_BOARD		(0x1) 	
#define	PC_MIP_COM_MODULE	(0x2)	


#define	ONE100US_SCHEDULE_CYCLE	 (0x1)
#define	ONEMS_SCHEDULE_CYCLE	 (0x2)


typedef struct {
	uint32	ctrlNetKind;
    uint32	ctrlNetDualStatus;
    uint32	myIpAddr[2]; 
	uint32	rmtIpAddr[2]; 
}strControlNetConfigInfo;


#define	MAX_ROUTE_NUM	3

typedef struct {
	uint32	destIp;
    uint32	gatewayIp;
}strRouteTblInfo;

#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)

typedef struct {
	uint32	ctrlNetKind;
    uint32	ctrlNetDualStatus;
    uint32	myDluIpAddrHex;
	uint8	macAddr[32];
	uint32	rmtDluIpAddrHex; 
    uint32	rmtNetDualStatus;
}strRedundancyNetConfigInfo;
#endif 


typedef struct {
	uint32	stationId;
    uint32	shelfType;		
    uint32	mpuEnable;

    uint32  dluIfKind;
    uint32	mbSlaveMapIf;

    uint32	ipMakingFlag;
    uint32	IOIfBoardKind; 
	strControlNetConfigInfo	netCnfgInfo;
	
	uint32	memoryBoardType;
	
	uint32  sysSchedlueType;
	
	uint32	routeIndex;		
    strRouteTblInfo	routeTbl [MAX_ROUTE_NUM];
	
	uint32	tmtcComBrdType;	
	
	
	uint32	hvdcCpuType;
    uint32  hvdcDiConfig;
    uint32  hvdcDoConfig;
    uint32  hvdcCcbConfig;
    uint32  hvdcVcbConfig;
    uint32  hvdcAiConfig;
    uint32  hvdcOdmConfig;
#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)
	
	strRedundancyNetConfigInfo redNetCnfgInfo;
	uint32  dluEnable;
#endif
}strSysConfigInfo;


#define	NVRAM_READ		(0x1)
#define	EXT_VAR_READ	(0x2)

#if ((BOARD_TYPE == MVME2100_BOARD)||(BOARD_TYPE == X86_PC_BOARD))
	#define NET_DEV_LEN        20  
	#define NET_HOST_LEN       20  
	#define NET_ADDR_LEN   	   30  
	#define NET_TGT_ADDR_LEN   30  
	#define NET_FILE_LEN       80  
	#define NET_USR_LEN        20  
	#define NET_PASSWORD_LEN   20  
	#define NET_OTHER_LEN      80  
	
	#define NET_FIELD_LEN      80  
#elif ((BOARD_TYPE == MVME3100_BOARD)||(BOARD_TYPE == MVME4100_BOARD)||(BOARD_TYPE == MVME2500_BOARD)||(BOARD_TYPE == LKV120_BOARD))
	#define NET_DEV_LEN        40  
	#define NET_HOST_LEN       20  
	#define NET_ADDR_LEN       30  
	#define NET_TGT_ADDR_LEN   50  
	#define NET_FILE_LEN       160 
	#define NET_USR_LEN        20  
	#define NET_PASSWORD_LEN   20  
	#define NET_OTHER_LEN      80  
	
	#define NET_FIELD_LEN      160 
#endif

typedef struct {
    uint8   netLineBIpAddr[NET_TGT_ADDR_LEN];
    uint8 	dualLinkIpAddr[NET_TGT_ADDR_LEN]; 
    int32  	stationNum;
    int32  	shelfType;
    int32  	mpuEnable;

    uint32  dluIfKind;
    int32	mbSlaveMapIf;

    int32	ioShelfType;	
    uint32	ipMakingFlag;	
	uint32	routeIndex;		
    strRouteTblInfo	routeTbl [MAX_ROUTE_NUM];
	uint32	memoryBoardType;
	uint32  sysSchedlueType;
	
	uint8	rmtTargetIpAddr[NET_TGT_ADDR_LEN];
	uint8	rmtModuleIpAddr[NET_TGT_ADDR_LEN];
	uint32	rmtDualLinkIpHex;
	
	uint32	tmtcComBrdType;	
	
	
	uint32	hvdcCpuType;
    uint32  hvdcDiConfig;
    uint32  hvdcDoConfig;
    uint32  hvdcCcbConfig;
    uint32  hvdcVcbConfig;
    uint32  hvdcAiConfig;
    uint32  hvdcOdmConfig;
}strUserBootParaInfo;

typedef struct
    {
    int8 bootDev [NET_DEV_LEN];    
    int8 hostName [NET_HOST_LEN];  
    int8 targetName [NET_HOST_LEN];    
    int8 netLineAIpAddr [NET_TGT_ADDR_LEN];       
    int8 bad[NET_TGT_ADDR_LEN];       
    int8 had [NET_ADDR_LEN];       
    int8 gad [NET_ADDR_LEN];       
    int8 bootFile [NET_FILE_LEN];  
    int8 startupScript [NET_FILE_LEN]; 
    int8 usr [NET_USR_LEN];        
    int8 passwd [NET_PASSWORD_LEN];    
    int8 other [NET_OTHER_LEN];    
    int32  procNum;           
    int32  flags;             
#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD) || (BOARD_TYPE == MVME2500_BOARD))
    int32  unitNum;                       
#endif
}strNetBootParaInfo;

typedef struct {
	strNetBootParaInfo	netBootParaInfo;
    strUserBootParaInfo	usrBootParaInfo;
}strSystemBootParaInfo;



#define	FILE_KIND_TXTFILE		0x3
#define	FILE_KIND_ENGFILE		0xf
#define ENG_FILE_NAME			"*.dp1"
#define TXT_FILE_NAME			"*.txt"

#ifdef _WIN32
#define	FILE_KIND_ENGSRC		(0x1)
#define	FILE_KIND_ROMIMG		(0x2)
#define	FILE_KIND_PBENG_FILE	(0xf)

#ifdef _WINCE
#define ROOT_DIR_NAME			"\\Hard Disk\\"
#define NVRAM_FILE_NAME			"\\Hard Disk\\NVRam.txt"
#define BACKUP_FILE_NAME		"\\Hard Disk\\FLASH.bak"
#define PBENG_FILE_NAME			"\\Hard Disk\\PEng.dbm"
#define ZIP_FILE_NAME			"\\Hard Disk\\*.zip"
#define APPEND_FILE_NAME		"\\Hard Disk\\%S"
#define ROM_FILE_NAME			"\\Hard Disk\\cPCI3720AT.*"
#define EXE_FILE_NAME			"cPCI3720AT.*"
#define ALL_PBENG_FILE_NAME		"\\Hard Disk\\*.dbm"	
#endif

#ifdef _WINNT
#define ROOT_DIR_NAME			"..\\"

#define PBENG_FILE_NAME			"PEng.dbm"
#define ZIP_FILE_NAME			"*.zip"

#define NVRAM_FILE_NAME			"NVRAM.BIN"
#define FLASH_FILE				"MEM.BIN"
#define BACKUP_FILE_NAME		"MEM.BAK"

#define ROM_FILE_NAME			"cPCI3720NT.*"
#define EXE_FILE_NAME			"cPCI3720NT.*"
#define ALL_PBENG_FILE_NAME		"*.dbm"					
#endif

#define	MAX_FILE_NAME_LENGTH	30
#endif	

#define	MAX_FILE_DIR_LENGTH		60

#define MAX_JUMP_LOOP_CNT		20000
#endif 





