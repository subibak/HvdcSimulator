
#ifndef LOCALBUF_H
#define LOCALBUF_H

#define		MAX_DATA_BUF_LEN	MAX_PROTOCOL_DATA_LEN 

typedef struct {
	uint32	dataLen;
    uint8	dataBuf[MAX_DATA_BUF_LEN];
}strBufInfo;


typedef struct {
	uint32	moduleId;
	uint32	maxSegmentNum;
    uint32	maxFbBlockAddr;
    uint32	maxFbParaSize;
    uint32	maxMemorySize;
	uint32	maxFbParaNum;
	uint32	maxFbOutputNum;
}strModuleInfo;


typedef struct {
	uint32	taskId;
	uint32	logicCheckSumStatus;
    uint32	taskMode;
	uint32  segLoopStatus;
}strSegStatusInfo;

typedef struct {
	uint32	taskId;
    uint32	status;
    uint32	*startDataPtr;
    uint32	taskMode;
}strSegModeInfo;

typedef struct {
	uint32	totalSegNum;
    strSegModeInfo	segInfo[256];
}strTotalSegDataInfo;

typedef struct {
	uint32	totalSegNum;
    uint32	taskId[256];
}strTotalSegDownInfo;

typedef struct {
	uint32	curParaIndex;
    uint32	curOutputIndex;
}strProgDwlParaInfo;

typedef struct {
	uint32	specTypeStartAddr;
    uint32	varTypeStartAddr;
    uint32	outputTypeStartAddr;

	uint32  specSizeStartAddr;
	uint32	varSizeStartAddr;
	uint32	outputSizeStartAddr;
}strFbCodeTypePointerInfo;


typedef struct {
	uint32	year;
    uint32	month;
    uint32	date;
    uint32	day;
    uint32	hour;
    uint32	minute;
    uint32	second;
    uint32	mSec;
}strTimeDataInfo;


typedef struct {
	uint16	segmentId;
	uint16	status;
}strFBOnlineSpecReallocRespInfo;


typedef struct {
	uint16	dwlKind;
	uint16	status;
}strFBOnlineLogicUpdateRespInfo;


#define		MAX_SFC_DOWN_SEG_NUM	(16)

typedef struct {
    uint32	stepTrId;
    uint32	status;
    uint32	*startDataPtr;
}strSFCStatusInfo;

typedef struct {
	uint32	taskId;
    uint32	*segDataPtr;
	uint32  *trsStartPtr;
	uint32  *stepStartPtr;
	strSFCStatusInfo trsData[256];
	strSFCStatusInfo stepData[256];
}strSFCSegDataInfo;

typedef struct {
	uint32	totalSegNum;
    strSFCSegDataInfo segInfo[MAX_SFC_DOWN_SEG_NUM]; 
	
}strTotalSFCSegDataInfo;



typedef struct {
    uint32  totalSecTime;
    uint32  fatalErrorNum;
}strSysHistoryData;

typedef struct {
    uint32  historyNum;
    strSysHistoryData sysHistory[128];
}strSysHistoryInfo;

typedef struct {
	uint32  moduleId;
    uint32  totTaksNo;
    uint32  curRemaintaskNo;
	uint32  nextTaskId;
}strSysTaskNumData;

typedef struct {
	uint32 X;
	uint32 T;
}strStepXTInfo;



typedef struct {
	
	uint32 	s; 				
	uint32	r1; 			

	

    
    uint32	q1;				
}strRSFlipFlopInfo;

typedef struct {
	
	uint32 	in_clk; 		
	uint32 	pt; 			

	
	uint32 	in_mem; 		

    
    uint32	q;			
    uint32	et;			
}strTONtimerInfo;


#define		MAX_FILE_NAME_LENGTH	(16)
typedef struct {
	uint32 	fileSize; 		
	uint32  fileStatus;
	uint32 	fileNameLen;
	uint8   fileName[MAX_FILE_NAME_LENGTH];
	uint32  fileCheckSum;
}strFMCEngFileDwlInfo;

#endif 
