
#ifndef SFCMEM_H
#define SFCMEM_H

#define MAX_SFCLINK_NUM		16
#define MAX_SFC_EXEC_NUM   1000

#define MAX_VERTEX_NUM (MAX_STEP_NUM + MAX_TRANSITOIN_NUM)

#define START_STEP 1

#define SFC_LOGIC_TYPE  0x1
#define FB_LOGIC_TYPE   0x0
#define NOT_LOGIC_TYPE  0xf


#define TRANSITION_TYPE		1
#define ACTION_TYPE			2
#define STEP_TYPE			3

#define AF_NONE			1
#define AF_NONSTROE		2
#define AF_RESET		3
#define AF_SET			4

#define AF_LIMIT		5
#define AF_DELAY		6
#define AF_PULSE		7

#define AF_SET_DELAY	8
#define AF_DELAY_SET	9
#define AF_SET_LIMIT	10

#define MAX_ACTION_FLAG  10

#define ACTIVE_FLAG		1
#define INACTIVE_FLAG	0


#define ACTIVE_STEP_KIND 0
#define ONE_STEP_KIND	 1





typedef struct {
	uint16 stepID;
	uint16 taskID; 
	
	uint8  initStep;
	uint8  linkActionNum;
	uint8  preTrNum;
	uint8  nextTrNum;

	uint8  actionFlag[MAX_SFCLINK_NUM];
	uint32 actionTime[MAX_SFCLINK_NUM];
	uint16 actionList[MAX_SFCLINK_NUM];
	
	uint16 preTrList[MAX_SFCLINK_NUM]; 
	uint16 nextTrList[MAX_SFCLINK_NUM];

	uint32 T;
	uint32 X;		
	uint32 CX;		
}strStepInfo;


typedef struct {
	uint16 transitionID;
	uint16 taskID; 

	uint16 preStepList[MAX_SFCLINK_NUM]; 
	uint16 nextStepList[MAX_SFCLINK_NUM];

	uint32 setActiveHeader;
	uint32 setActiveData;

	uint8  preStepNum;
	uint8  NextStepNum;
	uint8  activeStatus; 
	uint8  trsStatus;
}strTransitionInfo;


typedef struct {
	uint16 actionID;
	uint16 taskID; 

	
	uint32 varActiveHeader;			
	uint32 varActiveData;

	
	uint32 actCtrlData;
	uint32 setCurTime;				
	uint32 elapsedTime;				
}strActionInfo;





 
typedef struct {
    strFbExecutionInfo     *freeList;
    strFbExecutionInfo     *usedList[MAX_TRANSITOIN_NUM];
}strTrsExecQueInfo;

typedef struct {
    strDoubleLink   link;
	uint32 taskId;
    uint32 transitionId;
	uint32 setActiveHeader;
	uint32 setActiveData;
}strTrsLinkInfo;

typedef struct {
    strTrsLinkInfo     *freeList;
    strTrsLinkInfo     *usedList[MAX_LOGIC_TASK_NUM];
}strTrsLinkQueInfo;


 
typedef struct {
    strFbExecutionInfo     *freeList;
    strFbExecutionInfo     *usedList[MAX_ACTION_NUM];
}strActExecQueInfo;

typedef struct {
    strDoubleLink   link;
    uint32 actionId;
	uint32 taskId;
}strActionLinkInfo;

typedef struct {
    strActionLinkInfo     *freeList;
    strActionLinkInfo     *usedList[MAX_LOGIC_TASK_NUM];
}strActionLinkQueInfo;


typedef struct {
    strDoubleLink   link;
	uint32 actionId;
	uint32 taskId;
	uint32 actionFlag;
	uint32 actionTime;
	strFbExecutionInfo *startFbExcution;
}strStepLinkInfo;

typedef struct {
    strStepLinkInfo     *freeList;
    strStepLinkInfo     *usedList[MAX_STEP_NUM];
}strStepLinkQueInfo;


typedef struct {
	uint32 actCtrlQ		:2;
	uint32 actStatus    :1;
	uint32 actCX	    :1;
	uint32 reserved2	:6;

	uint32 Timer_Mem	:1;
	uint32 Pulse_Mem	:1;
	uint32 SL_RS_Q		:1;
	uint32 DS_RS_Q		:1;
	uint32 SD_RS_Q		:1;
    uint32 Set_RS_Q		:1;

	uint32 reserved1	:3;
	uint32 ActiveTFlag	:4;
	uint32 DelaySet		:1;
	uint32 SetLimit		:1;
	uint32 SetDelay		:1;
	uint32 Limit		:1;
	uint32 Delay		:1;				
	uint32 Pulse		:1;
	uint32 Set			:1; 
	uint32 Reset		:1; 
	uint32 Nonstore		:1;
}strActionCtrlInfo;


typedef struct
{
	strDoubleLink   link;
	uint32 nextNodeNum;
	uint32 prevNodeNum;
	
	uint32 taskId;
	uint32 nodeType;
	uint32 stepTrId;
	uint32 initStep;

	uint32 visitFlag;

	strDoubleLink *prevDblink;
	strDoubleLink *nextDblink;
} strSfcSequcerInfo;

typedef struct {
    strSfcSequcerInfo     *freeList;
    strSfcSequcerInfo     *usedList[MAX_LOGIC_TASK_NUM]; 
}strSfcSequencerQueInfo;


typedef struct {
    strDoubleLink   link;
	strSfcSequcerInfo* sequencerId;
	uint32  nodeType;
}strSfcAdjacentInfo;

typedef struct {
    strSfcAdjacentInfo     *freeList;
    strSfcAdjacentInfo     *nextList[MAX_VERTEX_NUM];
    strSfcAdjacentInfo     *prevList[MAX_VERTEX_NUM];
}strSfcAdjQueInfo;


 
typedef struct {
    strDoubleLink   link;
	strSfcSequcerInfo* sequencerId;
}strSfcExecutionInfo;

typedef struct {
    strSfcExecutionInfo     *freeList;
    strSfcExecutionInfo     *curExeList[MAX_LOGIC_TASK_NUM]; 
}strSfcExecQueInfo;										   


typedef struct {
	uint32 stepMemPtr;
	uint32 trsMemPtr;
	uint32 actMemPtr;
}strSfcMemPointerInfo;

typedef struct {
	uint32 curSegId;
	uint32 curActNum;
	uint32 curActId[MAX_ACTION_NUM];
}strActCurDebugInfo;

#endif 
