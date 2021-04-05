
#ifndef NETPROTOCOL_H
#define NETPROTOCOL_H

#define	MAX_NET_MSG_LEN  MAX_PROTOCOL_DATA_LEN
#define	MAX_NEW_FRAME_HEAD_LEN	(48)

typedef struct {
	uint8	companyID[8];			
	uint32	appType;
    uint32	loopID;					
    uint32	sourceID;				 
    uint32	destinationID;			 
    uint32	messageType;			  
    uint32	sequenceID;				
	uint32	deviceInfo;
	uint32  dualStatus;				
    uint32	dataLength;				
    uint32	checkSum;		  		
    
    uint8	dataBuff[MAX_NET_MSG_LEN - MAX_NEW_FRAME_HEAD_LEN];
}strNewNetProtocolInfo;

#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)
#define	MAX_DUAL_FRAME_HEAD_LEN	(8)
#define	DUAL_FRAME_FB_EXE_INDEX	(MAX_DUAL_FRAME_HEAD_LEN + 4)	

typedef struct {
    uint8	messageType;			  
	uint8	dualDataArea;			
    uint16	sequenceID;				

    uint16	dataLength;				
    uint16	checkSum;		  		

    uint8	dataBuff[MAX_NET_MSG_LEN - MAX_DUAL_FRAME_HEAD_LEN];
}strDualProtocolInfo;
#endif


#define	CTRL_NET_PATH	(0x1)
#define	SERI_NET_PATH	(0x2)


#define	MAX_PUMP_TIME_TBL_DATA_LEN	(192)
#define	MAX_PUMP_DATA_TRANS_PATH	(4)
#define	MAX_PUMP_TIME_TBL_DATA		(48)


#ifdef _WIN32
#define WRITE_KIND_ROM_UPD			1
#define WRITE_KIND_MEM_CLR			2
#define WRITE_KIND_RCS_RST			3
#endif

#if (defined(_WIN32) && (MB_SLAVE_MAP_TYPE == MB_SLAVE_MAP_DIPP))

#define DEFS_TRUE    (0==0)
#define DEFS_FALSE   (0==1)

#define DEFS_OK      (0)
#define DEFS_ERROR   (-1)

#define REG_PRO_MAIN	0
#define REG_PRO_END		10

#define MAX_TASK_NAME_LEN 50
#define MAX_NAMELENGTH	  50

typedef struct _PROCESS_REG_INFO 
{
	int8 processName [MAX_TASK_NAME_LEN];
	int8 processArgument [MAX_TASK_NAME_LEN];
	int8 processDescription [MAX_TASK_NAME_LEN];
	uint32 procRunFlag;

} PROCESS_REG_INFO, PROCREG_t;
#endif 

#endif 
