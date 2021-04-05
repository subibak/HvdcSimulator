
#ifndef FBMEM_H
#define FBMEM_H

#include "memDef.h"

typedef struct {
    uint16	fbTaskId;			
	uint16	fbId;
	uint16	inputNo;
    uint16 	intlVarNo;
    uint16	outputNo;
	uint16  inputSize;
	uint16  intlVarSize;
	uint16  outputSize;

    uint16	fbFuncAddr;
	uint16	fbRunStatus;		
	uint16	paraRunType;		

	uint16  logicType;			
								
	uint16  usingType;			
	uint16  ActionID;
	uint16  TransitionID;
	uint16  fbExecSequenceId;	

	uint32	specActionIndex;	
	uint32	specIndex;			
    uint32	varIndex;			
    uint32	outputIndex;
	uint32  forcingFlag;			 
	
	uint32  outputAction[2];		
}strFbMemInfo;


typedef struct {
	uint32	taskId;
    uint32	taskMode;			
	uint32	logicCheckSum;
    uint32	logicCheckSumStatus;

	uint32  recvMemSize;		
    uint32	memInputSize;		
	uint32  memOutputSize;		

	uint32  loopStatus;			
	uint32  taskCycleTime;		
	uint32  execMode;			

	union {
		uint32  evtActFbStartAddr;  
		uint32	fbStartBlockAddr;	
	}u1;
	union {
		uint32  evtSetFbEndAddr;   	
		uint32	fbEndBlockAddr;		
	}u2;
	
	uint32  taskPriority;		
	
	uint32  fbUsedNum;			

	uint32  taskTickCnt;		
	uint32  taskElpTime;
	uint32  taskOvertimeRunCnt;		

	uint32  logicType;			

	uint32  sfcStepKind;		

	
	uint32  reserved;

}strFbTaskTblMemInfo;


union multiDataType64{
	int				Bool_t;
    int				Int_t;
    float			Float_t;
    unsigned int	Hex_t;
	uint32			Uint_t;

	double			LReal_t;
#ifdef USED_BIT64
	int64			LInt_t;
	uint64			Ulint_t;
#endif
	uint32			halfUlint_t[2];
};

union multiDataType {
	int				Bool_t;
    int				Int_t;
    float			Float_t;
    unsigned int	Hex_t;
	uint32			Uint_t;
};


typedef struct {
	uint32	fbMemPtr;
    uint32	paraMemPtr;
	uint32	outputMemPtr;
	uint32	segTblMemPtr;
    uint32	stSharedMemPtr;
	uint32	retainMemPtr;
	uint32	autoMemPtr;
	uint32	flagMemPtr;
}strMemPointerInfo;


typedef struct {
	uint32	fbLogicCompileTime;
	uint32  curMaxSegmentId;
	uint32  curfbMaxBlockAddr;
	uint32  curfbMaxParaIndex;
	uint32  curfbMaxOutIndex;
	float	minFreeTime;
}strModuleExecInfo; 

typedef struct {
	uint32  curMaxPBSegmentId;
	uint32  curMaxTMSegmentId;
	uint32  curfbMaxPBTMOutIndex;
	float	minPBFreeTime;
	float	minTMFreeTime;
}strfbtmModuleExecInfo; 

typedef struct {
	uint32  curMaxVarRetainInfo;
	uint32  curMaxVarAddr;
	uint32  curMaxVarRetainNum;
	uint32  curMaxVarInitNum;
}strVariableExecInfo; 

typedef struct {
	uint32  curMaxStepId;
	uint32  curMaxTrsId;
	uint32  curMaxActId;
}strSfcMaxExecInfo; 

typedef struct {
	strModuleExecInfo	mpuSysExeInfo;
	strModuleExecInfo	diuSysExeInfo;
	strfbtmModuleExecInfo fbtmSysExeInfo;
	strVariableExecInfo autovarSysExeInfo;
	strVariableExecInfo flagvarSysExeInfo;
	strSfcMaxExecInfo   sfcSysExeInfo;
	uint32  maxJumpLoopCnt;
}strSysExecInfo; 


typedef struct {
	uint32 	moduleId; 		
	uint32 	blockAddr; 		
	uint32  fbId;
	uint32  index;
}strFbDataReadInfo;

#define SPEC_SIZE_BIT		1
#define SPEC_SIZE_BYTE		2
#define SPEC_SIZE_WORD		3
#define SPEC_SIZE_DWORD		4
#define SPEC_SIZE_LWORD		5

#define SPEC_DATA_TYPE_INT		1
#define SPEC_DATA_TYPE_UINT		2

#define SPEC_DATA_TYPE_LINT		3
#define SPEC_DATA_TYPE_ULINT	4
#define SPEC_DATA_TYPE_REAL		5
#define SPEC_DATA_TYPE_LREAL	6
#define SPEC_DATA_TYPE_TIME		7
#define	SPEC_DATA_TYPE_DATE		8
#define	SPEC_DATA_TYPE_TOD		9
#define	SPEC_DATA_TYPE_DT		10

#define SPEC_TYPE_BYREF		0x00
#define	SPEC_TYPE_BYVAL		0x01

#define M_MEMORY	1
#define A_MEMORY	2
#define	I_MEMORY	3
#define	Q_MEMORY	4
#define	S_MEMORY	5
#define	F_MEMORY	6
#define	T_MEMORY	7
#define	P_MEMORY	8

#define MAX_MEMTYPE P_MEMORY

typedef struct  {
#ifdef _WIN32
	uint32	reserved	: 3;
	uint32	taskID		: 8;
	uint32  bitposFlag	: 1;
	uint32	dataType	: 4;
	uint32	bitPosition	: 6;
	uint32	dataSize	: 4;
	uint32	memoryType	: 4;
	uint32	negated		: 1;
	uint32	refType		: 1;
#else
	uint32	refType		: 1;
	uint32	negated		: 1;
	uint32	memoryType	: 4;
	uint32	dataSize	: 4;
	uint32	bitPosition	: 6;
	uint32	dataType	: 4;
	uint32  bitposFlag	: 1;
	uint32	taskID		: 8;
	uint32	reserved	: 3;
#endif
} strFbSpecActionInfo; 


#define INPUT_PARA_TYPE  0x01
#define OUTPUT_PARA_TYPE 0x02
#define ALL_FB_PARA_TYPE 0x03

#define TYPECASTING_ENABLE 0x1


typedef struct {
	uint32  taskID;
	uint32 	jmpStepID; 		
	uint32 	jmpStepFlag; 
	uint32  jmpStepTime;
	uint32  jmpStepStatus;
}strSfcStepJumpInfo;


#define IS_ANY_TYPE_FB_CODE(fbcode)	( (fbcode == 1000) ? 1 : 0)


#define MIN_TYPE_CONV_FB_CODE 	(DINT_TO_TIME_CODE)
#define	MAX_TYPE_CONV_FB_CODE 	(LINT2L_BCD_CODE)

#define IS_ENABLE_CHK_FB_CODE(fbcode)	(0)

#endif 



