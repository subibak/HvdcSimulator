
#ifndef RMTDEVUTIL_H
#define RMTDEVUTIL_H


#define BT(a) (*((uint8 *)a))


#define COPYS(d, s) BT(d)=BT(s+1);\
                    BT(d+1)=BT(s)


#define COPYL(d, s) BT(d)=BT(s+3);BT(d+1)=BT(s+2);\
                    BT(d+2)=BT(s+1);BT(d+3)=BT(s)


#define	MAX_RMT_COM_Q_COUNT	(1024)


#define	MAX_RMT_Q_LIVE_COUNT	(254) 



#define	RMT_MASTER_SHELF	(0x0)
#define	RMT_SLAVE_SHELF		(0x1)


#define	RMT_CPU_OK		(0x0)
#define	RMT_CPU_FAIL	(0x1)


#define	RMT_GLOFA_SYS_STOP		(0x2)
#define	RMT_GLOFA_SYS_RUN		(0x4)
#define	RMT_GLOFA_SYS_PAUSE		(0x8)
#define	RMT_GLOFA_SYS_DEBUG		(0xa)

#define	RMT_XGT_SYS_RUN			(0x1)
#define	RMT_XGT_SYS_STOP		(0x2)
#define	RMT_XGT_SYS_ERROR		(0x4)
#define	RMT_XGT_RUN_ERROR		(0x5)
#define	RMT_XGT_STOP_ERROR		(0x6)
#define	RMT_XGT_SYS_DEBUG		(0x8)
#define	RMT_XGT_DEBUG_ERROR		(0xc)

#define MAX_VAR_NAME_LEN        16
#define MAX_VAR_NUM             16
#define MAX_DATA_BYTE_NUM       1400 
#define MAX_VAR_DATA_LEN        128   


#define READ_REQ       0x0054
#define READ_RESP      0x0055

#define WRITE_REQ      0x0058
#define WRITE_RESP     0x0059

#define STATUS_READ_REQ     0x00B0
#define STATUS_READ_RESP    0x00B1


typedef struct {
    uint16  command;
    uint16  dataType;
    uint16  memoryType;
    uint16  dataNum;
    uint16  memoryAddr[MAX_VAR_NUM];
    uint16  byteNum[MAX_VAR_NUM];
    uint32 data[256]; 
}strDataAccReqInfo;

typedef struct {
    uint32  dataLen;
    uint8   data[1024]; 
}strDataInfo;



#define ETH_GLOFA_BOOL_TYPE               0x00
#define ETH_GLOFA_BYTE_TYPE               0x01
#define ETH_GLOFA_WORD_TYPE               0x02
#define ETH_GLOFA_DWORD_TYPE              0x03
#define ETH_GLOFA_LWORD_TYPE              0x04
#define ETH_GLOFA_SINT_TYPE               0x05
#define ETH_GLOFA_INT_TYPE                0x06
#define ETH_GLOFA_DLINT_TYPE              0x07
#define ETH_GLOFA_LINT_TYPE               0x07
#define ETH_GLOFA_USINT_TYPE              0x08
#define ETH_GLOFA_UINT_TYPE               0x09
#define ETH_GLOFA_UDINT_TYPE              0x0a
#define ETH_GLOFA_ULINT_TYPE              0x0b
#define ETH_GLOFA_REAL_TYPE               0x0c
#define ETH_GLOFA_LREAL_TYPE              0x0d
#define ETH_GLOFA_TIME_TYPE               0x0f
#define ETH_GLOFA_DATE_TYPE               0x10
#define ETH_GLOFA_TIME_OF_DAY_TYPE        0x11
#define ETH_GLOFA_DATE_AND_TIME_TYPE      0x12
#define ETH_GLOFA_BLOCK_TYPE              0x14

typedef struct {
    uint32      numOfTotalBytes; 
    uint32      seqNum;
    uint32      srcAddr;
    uint32      destAddr;
    uint32      msgType;
    uint32      msgStatus;
    uint32      dualId;
    uint32      moduleId;
    uint32      opCode;
    uint32      optionCode;
    uint32      itemNum;
}strLocalProtocolHeadInfo;


typedef struct {
    strDoubleLink   link;
    uint32  liveCount;
    uint32	sendDeviceSeqNum;
    
    
    uint32	rmtReqMemAddr;
    strEthCommonInfo			dataComInfo;
	strEthDataAccCommonInfo		dataAccInfo;
	strLocalProtocolHeadInfo	localProtocolHeadInfo;
}strRmtDevSendMsgInfo;

typedef struct {
    strRmtDevSendMsgInfo     *freeList;
    strRmtDevSendMsgInfo     *usedList;
}strRmtComQInfo;


typedef struct {
    strDoubleLink   link;
	uint32	liveCount;
	uint32	seqNum;
	uint32	opCode;
    uint32	optionCode;
    uint32	rmtDevType;
    uint32	fbBlockAddr;
}strEthComDualMsgInfo;

typedef struct {
    strEthComDualMsgInfo     *freeList;
    strEthComDualMsgInfo     *usedList;
}strRmtDualComQInfo;



#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD) || (BOARD_TYPE == MVME2500_BOARD) || (BOARD_TYPE == LKV120_BOARD))
	#define	MAX_ETH_COM_FB_CODE_NUM	(512)
#elif (BOARD_TYPE == MVME2100_BOARD)
	#define	MAX_ETH_COM_FB_CODE_NUM	(128)
#else
	#define	MAX_ETH_COM_FB_CODE_NUM	(512)
#endif

typedef struct {
	uint32	fbCodeBlockAddr[MAX_ETH_COM_FB_CODE_NUM];
    uint32	fbId[MAX_ETH_COM_FB_CODE_NUM];
    uint32	prevRcvSeqNum[MAX_ETH_COM_FB_CODE_NUM];
    uint32	rcvCount[MAX_ETH_COM_FB_CODE_NUM];
    uint32	seqErrCount[MAX_ETH_COM_FB_CODE_NUM];
}strEthRmtSeqNumChkDBInfo;


#define	ETH_RMT_STATUS_READ_FB_CODE	(301)
#define	ETH_RMT_DATA_SEND_FB_CODE	(302)
#define	ETH_RMT_DATA_BROAD_FB_CODE	(303)
#define	MBT_RMT_DATA_SEND_FB_CODE	(316)


#define	ETH_RMT_DATA_READ_FB_CODE_FOR_REPLACE	(1531)
#define	ETH_RMT_DATA_WRITE_FB_CODE_FOR_REPLACE	(1532)

#endif 
