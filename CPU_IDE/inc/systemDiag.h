
#ifndef SYSRAS_H
#define SYSRAS_H


#define	SYS_STATUS_INFO		(0x01)
#define	MPU_STATUS_INFO		(0x02)
#define	DIU_STATUS_INFO		(0x03)	
#define	NET_STATUS_INFO		(0x04)	
#define	FBC_STATUS_INFO		(0x05)	
#define	IO_STATUS_INFO		(0x06)	
#define	DPU_STATUS_INFO		(0x07)	
#define	PORT_STATUS_INFO	(0x08)	


#define	SYS_OPERATION_MODE		(0x1) 	
#define	SYS_RUNNING_MODE		(0x2)	
#define	SYS_DUAL_COPY_MODE		(0x3)   
#define	SYS_DUAL_RUNNING_MODE	(0x4)   


#define	RAS_SYSTEM_STATUS		(0x1)
#define	RAS_SHELF_STATUS		(0x2)
#define	RAS_REDUNDANCY_STATUS	(0x3)
#define	RAS_SYSTEM_MODE			(0x4)
#define	RAS_SHELF_ID			(0x5)
#define	RAS_MPU_CHECKSUM_STATUS		(0x6)
#define	RAS_DIU_CHECKSUM_STATUS		(0x7)
#define	RAS_REDUNDANCY_ERR_STATUS	(0x8)
#define	RAS_MPU_EXEC_OVERTIME_STATUS	(0x9)
#define	RAS_DIU_EXEC_OVERTIME_STATUS	(0xa)
#define	RAS_NET_COM_STATUS			(0xb)
#define	RAS_RMT_NET_COM_STATUS		(0xc)
#define	RAS_MPU_DUAL_COPY_STATUS		(0xd)
#define	RAS_DIU_DUAL_COPY_STATUS		(0xe)
#define	RAS_MPU_EXEC_LOOP_STATUS		(0xf)



#define	SYS_RAS_SYSTEM_RUN		(0x01)
#define	SYS_RAS_SYSTEM_STOP		(0x02)


#define	SYS_RAS_MASTER_RUN		(0x01)
#define	SYS_RAS_SLAVE_RUN		(0x02)


#define	SYS_RAS_REDUNDANCY_RUN		(0x01)
#define	SYS_RAS_REDUNDANCY_STANDBY	(0x02)


#define	SYS_RAS_SYSTEM_AUTO_MODE	(0x01)
#define	SYS_RAS_SYSTEM_MANUAL_MODE	(0x02)


#define	SYS_RAS_MASTER_SHELF_ID		(0x01)
#define	SYS_RAS_SLAVE_SHELF_ID		(0x02)

typedef struct {
	uint32	sysRunningMode;				
    uint32	sysRedundancyRunningStatus; 
    uint32	sysRedundancyStatus;		
    uint32	sysOperationMode;			
    uint32	sysShelfId;					
}strSysRunningConfigInfo;


typedef struct {
	uint32	moduleReadyErr;
    uint32	moduleLiveCount;
    uint32	DCUErr;
    uint32	IOCommStatus;
    uint32	fmcIOCommStatus;		
    uint32	redundancyStatus;
    uint32	maxErrorStatus;
    uint32	dualCopyErrStatus;
    uint32	interProcMsgStatus;
    uint32	moduleSysCallErr;
    uint32	moduleDiagonisticErr;
	uint32	moduleSwVersion;
    uint32	moduleVersionDate;
    uint32	moduleLiveCountStatus; 
	uint32	moduleChecksumStatus;
	uint32	moduleFbRunningStatus;
	uint32	moduleFbOvertimeStatus;
	uint32  moduleFBLoopingStatus;
}strMpuDiuRunningStatusInfo;


typedef struct {
	uint32	moduleReadyErr;
    uint32	moduleLiveCount;
    uint32	moduleLiveCountStatus;
	uint32	networkStatus;
	uint32	networkMsgStatus; 
	uint32	moduleSwVersion;
    uint32	moduleVersionDate;
    uint32	Port_A_IPAddr;
    uint32	Port_B_IPAddr;
	uint32	msgReqCount;
    uint32	msgRespCount;
    uint32	broadcastRecvCount;
    uint32	broadcastSendCount;
    uint32	msgLostCount;	
    uint32	rmtNetworkStatus;	
	uint32  line_A_Count;   
	uint32  line_B_Count;
}strNetRunningStatusInfo;


typedef struct {
	uint32	moduleReadyErr;
    uint32	moduleCurLiveCount;
    uint32	modulePrevLiveCount;
	uint32	moduleSwVersion;
    uint32	moduleVersionDate;
    uint32	moduleLiveCountStatus; 
	uint32	IOCommStatus;
}strFBCRunningStatusInfo;


#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
#define	DCU_MASTER	(0x1)
#define	DCU_SLAVE	(0x2)
#else
#define	DCU_MASTER	(0x81)
#define	DCU_SLAVE	(0x82)
#endif

typedef struct {
	uint32	memoryStatus;
    uint32	hwStatus;
    uint32	configStatus;
    uint32	commStatus;
#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
    uint32	scanOvertime;	
    uint32	dualStatus;		
#endif
    uint32	cardId;	

}strDCURunningStatusInfo;

typedef struct {
	uint32	pointErrStatus;		
	uint32	failSafeDataSetErr;	
	uint32	ioIfErr;	
#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
	uint32	ioParaSetErr;
#endif
}strIOCardPointStatusInfo;

#define	IO_CARD_RUN		(0x1)
#define	IO_CARD_AUTO	(0x0)
#define	IO_CARD_MANUAL	(0x1)


#define	IO_DUAL_RUN		(0x0)
#define	IO_DUAL_STANDBY	(0x1)
#define	IO_DUAL_FAULT	(0x2)

typedef struct {
	uint32	ioCardType;
	uint32	ioCardPointId;
    uint32	ioCardBusStatus;
	uint32	ioCardInstallStatus;
	uint32	ioCardHWStatus;
	uint32	ioCardConfigStatus;
	uint32	ioCardSetErr;		
	uint32	ioCardIFErr;		
	uint32	dualCmdStatus;
	uint32	dualStatus; 
	uint32	comBrdCommErrStatus;
									
	uint32	ioPointErrStatus;	
	uint32	failSafeDataSetErr;	
#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
	uint32	hartSlaveDevErr;	
	uint32	diagPntIndex;		
	uint32	diagPntEvtCode;		
#endif
	strIOCardPointStatusInfo ioPointStatusInfo[MAX_IO_POINT_NUM];
}strIOCardRunningStatusInfo;


#define	DCU_BOARD_DUAL		(0x0)
#define	DCU_BOARD_SINGLE	(0x1)

typedef struct {
    uint32	ioShelfScanStatus;	
    uint32	dcuDualStatus;			
	strDCURunningStatusInfo		dcuStatusInfo[2];
	strIOCardRunningStatusInfo	ioCardStatusInfo[MAX_IO_CARD_NUM];
	
	uint32	ioConfigStatus;		
#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
	uint32	ioBoardDualSummary;
#endif
}strIOShelfRunningStatusInfo;

#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)

#define	IO_RAS_UPDATE_SKIP	(0)
#define	IO_RAS_UPDATE_DOIT	(1)



typedef struct {
	uint32 dcuErrBitFlags;
	uint32 boardErrBitFlags;
	uint32 pointErrBitFlags;
}strIoShelfRasChangeFlagInfo;
#endif


typedef struct {
	uint32	moduleReadyErr;			
    uint32	moduleLiveCount;		
	uint32	moduleLiveCountStatus;  	
	uint32	fmcBusMSState;		
    uint32	fmcIOCommStatus;		
	uint32	masterRasSummary;
    uint32	fieldbusErrDetail;
#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
	uint32	masterRomVersion;	
	uint32  moduleVersionDate;
	uint32  fmsStatus[16];
#else
	uint32	masterRomVersion[4];		
#endif
}strFMasterRunningStatusInfo;


typedef struct {
	uint16	slaveState;
}strFSlaveRunningStatusInfo;


typedef struct {
#ifdef _WIN32
#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
	uint8 reserved		:3;
	uint8 netCommErr	:1;
	uint8 jmploopErr	:1;
	uint8 fbOverTimeErr	:1;
	uint8 fbRunningErr	:1;
	uint8 checkSumErr	:1;
#else
	uint8 checkSumErr	:1;
	uint8 fbRunningErr	:1;
	uint8 fbOverTimeErr	:1;
	uint8 jmploopErr	:1;
	uint8 netCommErr	:1;
	uint8 reserved		:3;
#endif
#else
#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
	uint8 checkSumErr	:1;
	uint8 fbRunningErr	:1;
	uint8 fbOverTimeErr	:1;
	uint8 jmploopErr	:1;
	uint8 netCommErr	:1;
	uint8 reserved		:3;
#else
	uint8 reserved		:3;
	uint8 netCommErr	:1;
	uint8 jmploopErr	:1;
	uint8 fbOverTimeErr	:1;
	uint8 fbRunningErr	:1;
	uint8 checkSumErr	:1;
#endif
#endif
}strMpuMFMErrStringInfo;

typedef struct {
#ifdef _WIN32
#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
	uint8 reserved		:2;
	uint8 ioCommErr		:1;
	uint8 liveCntErr	:1;
	uint8 readyErr		:1;
	uint8 fbOverTimeErr	:1;
	uint8 fbRunningErr	:1;
	uint8 checkSumErr	:1;
#else
	uint8 checkSumErr	:1;
	uint8 fbRunningErr	:1;
	uint8 fbOverTimeErr	:1;
	uint8 readyErr		:1;
	uint8 liveCntErr	:1;
	uint8 ioCommErr		:1;
	uint8 reserved		:2;
#endif
#else
#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
	uint8 checkSumErr	:1;
	uint8 fbRunningErr	:1;
	uint8 fbOverTimeErr	:1;
	uint8 readyErr		:1;
	uint8 liveCntErr	:1;
	uint8 ioCommErr		:1;
	uint8 reserved		:2;
#else
	uint8 reserved		:2;
	uint8 ioCommErr		:1;
	uint8 liveCntErr	:1;
	uint8 readyErr		:1;
	uint8 fbOverTimeErr	:1;
	uint8 fbRunningErr	:1;
	uint8 checkSumErr	:1;
#endif
#endif
}strFbcMFMErrStringInfo;

typedef struct {
#ifdef _WIN32
#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
	uint8 commErr2		:1;
	uint8 configErr2	:1;
	uint8 hwErr2		:1;
	uint8 memErr2		:1;
	uint8 commErr1		:1;
	uint8 configErr1	:1;
	uint8 hwErr1		:1;
	uint8 memErr1		:1;

#else
	uint8 memErr1		:1;
	uint8 hwErr1		:1;
	uint8 configErr1	:1;
	uint8 commErr1		:1;
	uint8 memErr2		:1;
	uint8 hwErr2		:1;
	uint8 configErr2	:1;
	uint8 commErr2		:1;
#endif
#else
#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
	uint8 memErr1		:1;
	uint8 hwErr1		:1;
	uint8 configErr1	:1;
	uint8 commErr1		:1;
	uint8 memErr2		:1;
	uint8 hwErr2		:1;
	uint8 configErr2	:1;
	uint8 commErr2		:1;

#else
	uint8 commErr2		:1;
	uint8 configErr2	:1;
	uint8 hwErr2		:1;
	uint8 memErr2		:1;
	uint8 commErr1		:1;
	uint8 configErr1	:1;
	uint8 hwErr1		:1;
	uint8 memErr1		:1;
#endif
#endif
}strDcuMFMErrStringInfo;

typedef struct {
#ifdef _WIN32
#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
	uint8 reserved		:4;
	uint8 fmcIoCommErr	:1;
	uint8 memAccErr		:1;
	uint8 fmcReadyErr	:1;
	uint8 dpuCommErr	:1;
#else
	uint8 dpuCommErr	:1;
	uint8 fmcReadyErr	:1;
	uint8 memAccErr		:1;
	uint8 fmcIoCommErr	:1;
	uint8 reserved		:4;
#endif
#else
#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
	uint8 dpuCommErr	:1;
	uint8 fmcReadyErr	:1;
	uint8 memAccErr		:1;
	uint8 fmcIoCommErr	:1;
	uint8 reserved		:4;
#else
	uint8 reserved		:4;
	uint8 fmcIoCommErr	:1;
	uint8 memAccErr		:1;
	uint8 fmcReadyErr	:1;
	uint8 dpuCommErr	:1;
#endif
#endif
}strTmFmcMFMErrStringInfo;

typedef struct {
	strMpuMFMErrStringInfo		errStr1;
	strFbcMFMErrStringInfo		errStr2;
	strDcuMFMErrStringInfo		errStr3;
	strTmFmcMFMErrStringInfo	errStr4;
}strMFMRunningStatusInfo;


typedef struct {
	uint32		profiShelfRasSummary;
	uint32		scanCmdStatus;
	uint32		pcuStatus;
	uint32		fmcBoardStatus[6];
	uint32		fmcDualRunInfo;
	uint32		slvDiagCount[12];
	uint32		fmcSlvErrSummary[12][4];
}strProfiShelfRunningStatusInfo;

typedef struct {
	strProfiShelfRunningStatusInfo	profiShelfRasInfo[4];
}strMultiProfiRunningStatusInfo;


typedef struct {
	strSysRunningConfigInfo			sysRunningConfigInfo;
	strMpuDiuRunningStatusInfo		sysMpuRunningStatusInfo;
	strMpuDiuRunningStatusInfo		sysDiuRunningStatusInfo;
	strNetRunningStatusInfo			sysNetRunningStatusInfo;
	strFBCRunningStatusInfo			sysFbcRunningStatusInfo;
	strIOShelfRunningStatusInfo		sysIOShelfStatusInfo[MAX_IO_SHELF_NUM];
#if (MAXIOSHELF_CONFIG == MAX12IOSHELF)
	strETOSRunningStatusInfo		sysEtosRunningStatusInfo;
#endif
	strFMasterRunningStatusInfo		sysFMasterRunningStatusInfo;						 
	strFSlaveRunningStatusInfo		sysFSlaveStatusInfo[MAX_FIELDBUS_SLAVE_NUM]; 
	strMFMRunningStatusInfo			sysMFMRunningErrStringInfo;
#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
	strMultiProfiRunningStatusInfo	sysMultiProfiRunningStatusInfo;
#endif
}strSystemRunningStatusInfo;

#define WDC_RAS		(0x01)
#define EWS_RAS		(0x02)

/* 2019.12.06 HVDC 알고리즘 검증 시스템 RAS 정의 */
typedef struct {
	uint32 slfRasSummary; 
	uint32 scanCmdScanStat;     /* scan cmd(16bytes) | scan stat(16bytes) */
	uint32 ioAdaptBrdStat;      /* B_IO_Adapt(16bytes) | A_IO_Adapt(16bytes) */
#define MAX_EXT_IO_BOARD_NUM	(12)
	uint32 ioBrdStat[MAX_EXT_IO_BOARD_NUM/2];   /* Second IO Board(16bytes) | First IO Board(16bytes) */
} strIoShelfRasInfo;

typedef struct {
	uint32 date;                /* Year(8bytes) | Month(8bytes) | Day(8bytes) | Hour(8bytes) */
	uint32 time;                /* minute(8bytes) | second(8bytes) | milisecond(16bytes) */
	uint32 dualSideId;

#define HVDC_CPU_C_TYPE	(1)	/* Control */
#define HVDC_CPU_P_TYPE	(2)	/* Protection */
#define HVDC_CPU_V_TYPE	(3)	/* VBE */
#define HVDC_CPU_M_TYPE	(4)	/* Measurement */
	uint32 cpuType;
	uint32 cpuErrSummary;
	uint32 interStnCommStat;
	uint32 fiberCommStat;
	uint32 cpuRomVersion;       /* Reserved(16bytes) | major ver.(8bytes) | minor ver.(8bytes) */
	uint32 taskStatus;          /* Task#8(4bytes) ~ Task#1(4bytes) */

#define DIO_NONE_TYPE	(0)
#define DIO_DI_TYPE	    (1)
#define DIO_DO_TYPE	    (2)
	uint32 dioBoardCfgStat;

#define MAX_CPU_C_DIO_BOARD_NUM	(5)
#define MAX_CPU_P_DIO_BOARD_NUM	(MAX_CPU_C_DIO_BOARD_NUM)
	uint32 dioBoardErrStat[MAX_CPU_C_DIO_BOARD_NUM];
	uint32 aioBoardCfgStat;

#define MAX_CPU_C_AIO_BOARD_NUM	(1)
#define MAX_CPU_P_AIO_BOARD_NUM	(0)
	uint32 aioBoardErrStat;

#define MAX_CPU_C_ODM_BOARD_NUM	(0)
#define MAX_CPU_P_ODM_BOARD_NUM	(0)
	uint32 ioComBrdStatSummary;
	uint32 ioComBrdLiveCnt;
	uint32 ioComBrdRomVersion;

#define MAX_EXT_IO_SHELF_NUM	(8)
	strIoShelfRasInfo ioShelfRasInfo[MAX_EXT_IO_SHELF_NUM];
	
} strCnPTypeCpuRasInfo;

typedef struct {
	uint32 date;                /* Year(8bytes) | Month(8bytes) | Day(8bytes) | Hour(8bytes) */
	uint32 time;                /* minute(8bytes) | second(8bytes) | milisecond(16bytes) */
	uint32 dualSideId;
	uint32 cpuType;
	uint32 cpuErrSummary;
	uint32 unusedPad1;
	uint32 unusedPad2;
	uint32 cpuRomVersion;       /* Reserved(16bytes) | major ver.(8bytes) | minor ver.(8bytes) */
	uint32 unusedPad3;

#define MAX_CPU_V_DIO_BOARD_NUM	(0)
	uint32 aioBoardCfgStat;

#define MAX_CPU_V_AIO_BOARD_NUM	(6)
	uint32 aioBoardErrStat[MAX_CPU_V_AIO_BOARD_NUM];

#define MAX_CPU_V_ODM_BOARD_NUM	(0)

#if 0
[V105] 진단 정보 제외 : m영역에 서버 모듈 진단 정보 및 전압 Write 함
#define MAX_PHASE_SUB_MODULE_NUM	(768)
	uint32 phaseModule[MAX_PHASE_SUB_MODULE_NUM];
#endif
} strVTypeCpuRasInfo;

typedef struct {
	uint32 date;                /* Year(8bytes) | Month(8bytes) | Day(8bytes) | Hour(8bytes) */
	uint32 time;                /* minute(8bytes) | second(8bytes) | milisecond(16bytes) */
	uint32 dualSideId;
	uint32 cpuType;
	uint32 cpuErrSummary;
	uint32 cFiberCommStat;
	uint32 pFiberCommStat;
	uint32 cpuRomVersion;       /* Reserved(16bytes) | major ver.(8bytes) | minor ver.(8bytes) */
	uint32 unusedPad3;

#define MAX_CPU_M_DIO_BOARD_NUM	(0)
	uint32 aioBoardCfgStat;

#define MAX_CPU_M_AIO_BOARD_NUM	(6)
	uint32 aioBoardErrStat[MAX_CPU_M_AIO_BOARD_NUM];

#if 0
[V105] 사용 안함
#define MAX_CPU_M_ODM_BOARD_NUM	(0)
	uint32 odmBoardCfgStat;
	uint32 odmBoardErrStat;
#endif
} strMTypeCpuRasInfo;

typedef union {
	strCnPTypeCpuRasInfo 	cpuC;
	strCnPTypeCpuRasInfo 	cpuP;
	strVTypeCpuRasInfo		cpuV;
	strMTypeCpuRasInfo		cpuM;
} unnCpuRasInfo;


typedef struct {
	uint32	config;
	uint32	errCode;
	uint32	liveCnt;
} strBaseIoInfo;

typedef struct {
	strBaseIoInfo diCard[5];
	strBaseIoInfo doCard[5];
	strBaseIoInfo ccb;
	strBaseIoInfo vcb[6];
	strBaseIoInfo aiCard[6];
	strBaseIoInfo odm;
}strBaseShelfIoInfo;

#endif 
