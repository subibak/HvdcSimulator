
#ifndef _mbtServer_h
#define _mbtServer_h

#ifdef __cplusplus
extern "C" {
#endif


#include "pmsCommon.h"
#include "modbustcp\modbustcp.h"

#define MAX_DEV_MBTS				2		
#define MAX_CHANNEL_MBTS			20		


typedef struct
{
	TMWTYPES_INT		iRxFrameTimeout;	
			
	TMWTYPES_INT		iMaxCommIdleTime;	
			
	TMWTYPES_USHORT 	uTcpPort;   	 	
	TMWTYPES_INT		iChannelMax;		
}	CFG_DEV_MBTS_ST;

typedef struct
{
	TMWTYPES_INT		iNumDevMbts;			
	CFG_DEV_MBTS_ST		arDevMbts[MAX_DEV_MBTS];	
}	CFG_MBT_SERVER_ST;

extern CFG_MBT_SERVER_ST g_MbtServerCfg;




typedef struct _inf_channel_mbts_st
{
#if 0   
	TMWTYPES_CHAR			szName[16];		
#endif	
	TMWTYPES_INT			iIndex;
	CFG_DEV_MBTS_ST			*pC;			
	struct _inf_dev_mbts_st	*pI;			

	
#if 0   
	CH_STS_ENUM				eChStatus;		
	DEV_STS_ENUM			eStatus;		
	CH_STAGE_ENUM			eStage;
#endif	

#if 0   
	TMWTYPES_BOOL			bConnected;		
	TMWTYPES_BOOL			bOnline;		
	TMWTYPES_BYTE			openTryCount;	
											
	TMWTYPES_MILLISECONDS	msTimeConnect;	
											
	CH_STATS_ST				tStats;			
	
	TMWTYPES_ULONG			scanFlag;		
	TMWTYPES_ULONG			sendFlag;		
	TMWTYPES_BYTE			scanAllIndex;	
	TMWTYPES_BYTE			scanPollIndex;	
	DNP_CONTROL_BO_ST		scanControlBo;	
	DNP_CONTROL_AO_ST		scanControlAo;	
	TMWTYPES_BYTE			responseFailCount;	
	TMWTYPES_MILLISECONDS	msTimeLastSlaveSend;
#endif	

	int						commFd;			
	char					peerIp[32];
	MB_DIAGNOSTICS_ST		diag;			
	MBT_MSG_DATA_ST			msg;			
	MBT_ERR_DATA_ST			rxError;
	
	MBT_COMM_STAT_ST		commStat;
}	INF_CHANNEL_MBTS_ST;



typedef struct _inf_dev_mbts_st
{
	TMWTYPES_INT		iIndex;
	
	
	
	int					listenFd;		

	
	TMWTYPES_BOOL		bRun;			
#if 0   
	DEV_STS_ENUM		eStatus;		
#endif	
	int					channelCount;	
	INF_CHANNEL_MBTS_ST	tChan[MAX_CHANNEL_MBTS];
} INF_DEV_MBTS_ST;


extern INF_DEV_MBTS_ST g_MbtsInf[MAX_DEV_MBTS];



#ifdef __cplusplus
}
#endif

#endif 
