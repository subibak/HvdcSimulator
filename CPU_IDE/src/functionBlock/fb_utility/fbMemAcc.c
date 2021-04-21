
#include	<stdio.h>
#include	<stdLib.h>
#include	<math.h>
#include	<time.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include	"memDef.h"
#include	"fbMem.h"
#include	"segScheduler.h"
#include	"usrDefBuf.h"
#include	"fbDef.h"
#include	"lkv120Board.h"
#include	"sysDual.h"
#include	"sysConfig.h"

#include	"usrErrCode.h"
#include	"fb_code\\fb_common\fbRunningErrCode.h"
#include	"systemDiag.h"
#include	"usrQueData.h"
#include	"osDepend.h"
#include	"usrSema.h"

#include    "sfcMem.h"
#include	"fb_code\\fb_standard\fcExtra.h"
	
#include	"network.h"

#include	"usrQueData.h"
#include	"msgQueData.h"
#include	"debugInfo.h"

#define	WITH_VAL	(0)
#define	WITH_ADDR	(1)

#define	DO_COPY		(0)
#define	DONT_COPY	(1)

#define PARA_COPY    (0x80000000)
#define	GET_PARA_COPY_FLAG(paraType) (((paraType)&PARA_COPY) == PARA_COPY ? 1 : 0)

uint8		*gMpuFbStartAddr,	*gBusMpuFbStartAddr;
uint8		*gDiufbStartAddr,	*gBusDiufbStartAddr;
uint8		*gTmtcfbStartAddr,	*gBusTmtcfbStartAddr;

uint8		*gFieldbusfbStartAddr,	*gBusFieldbusfbStartAddr;	

strFbMemInfo		*gMpuFbMemInfoLocalPtr = NULL, *gMpuFbMemInfoBusPtr = NULL;
strFbMemInfo		*gDiuFbMemInfoLocalPtr = NULL, *gDiuFbMemInfoBusPtr = NULL;
strFbMemInfo		*gTmtcFbMemInfoLocalPtr = NULL, *gTmtcFbMemInfoBusPtr = NULL;

strFbMemInfo		*gFieldbusFbMemInfoLocalPtr = NULL, *gFieldbusFbMemInfoBusPtr = NULL;	


void		*gMpuSpecAndVarDataLocalPtr,*gMpuSpecAndVarDataBusPtr;
void		*gDiuSpecAndVarDataLocalPtr,*gDiuSpecAndVarDataBusPtr;
void		*gTmtcSpecAndVarDataLocalPtr,*gTmtcSpecAndVarDataBusPtr;

void		*gFieldbusSpecAndVarDataLocalPtr,*gFieldbusSpecAndVarDataBusPtr;


void		*gMpuOutputDataLocalPtr,*gMpuOutputDataBusPtr;
void		*gDiuOutputDataLocalPtr,*gDiuOutputDataBusPtr;
void		*gTmtcOutputDataLocalPtr,*gTmtcOutputDataBusPtr;

void		*gFieldbusOutputDataLocalPtr,*gFieldbusOutputDataBusPtr;


strFbTaskTblMemInfo	*gMpuFbSegTblInfoLocalPtr,*gMpuFbSegTblInfoBusPtr;
strFbTaskTblMemInfo	*gDiuFbSegTblInfoLocalPtr,*gDiuFbSegTblInfoBusPtr;
strFbTaskTblMemInfo	*gTmtcFbSegTblInfoLocalPtr,*gTmtcFbSegTblInfoBusPtr;

strFbTaskTblMemInfo	*gFieldbusFbSegTblInfoLocalPtr,*gFieldbusFbSegTblInfoBusPtr;


uint32	*gSTSharedMemDataLocalPtr,*gSTSharedMemDataBusPtr;


uint32 *gDualDataBusPtr,*gUserEndBusPtr;



void *gRetainVarInfoLocalPtr, *gRetainVarInfoBusPtr;
void *gAutoVariableLocalPtr, *gAutoVariableBusPtr;
void *gMmemVariableLocalPtr, *gMmemVariableBusPtr;
strSysExecInfo *gSysExeInfoLocalPtr, *gSysExeInfoBusPtr;


strStepInfo *gSFCStepInfoLocalPtr, *gSFCStepInfoBusPtr;
strTransitionInfo *gSFCTransitionLocalPtr, *gSFCTransitionBusPtr;
strActionInfo *gSFCActionInfoLocalPtr, *gSFCActionInfoBusPtr;

#if	(SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
strAccumBusInfo sAccumSimData;
uint32 gVarAreaOffset,gSFCAreaOffset,gSysExeOffset;
uint32 gDiuAreaOffset,gDualAreaOffset,gSysNvRamAreaOffset, gUserEndOffset;
#else

void *gAutoVarInitLocalPtr, *gAutoVarInitBusPtr;
void *gMmemVarInitLocalPtr, *gMmemVarInitBusPtr;
#endif


void *gEngFileDataBusPtr;
void *gEngFileInfoBusPtr;

extern strFbCodeTypeInfo	gFbTypeTbl; 
extern uint32	*gSpecTypePtr, *gVarTypePtr, *gOutputTypePtr;

extern uint32	*gSpecSizePtr, *gVarSizePtr, *gOutputSizePtr;


extern int32 gDebugPortFd;
extern strFbExecDbInfo	gFbCodeSortingTableInfo[MAX_FB_CODE_ID+1]; 

extern strMpuSegSchedulerInfo	*gMpuSegSchdInfoPtr;
extern strDiuSegSchedulerInfo	*gDiuSegSchdInfoPtr;

extern strSysRunningConfigInfo		*gSysRunningInfoPtr;
extern strMpuDiuRunningStatusInfo 	*gDiuRasInfoPtr;

extern strSysConfigInfo	gSysCnfgInfo;

extern strFbExecutionInfo* gFBExeSequenceQueuePtr[MAX_FB_EXEC_SEQ_ID_NUM];


extern uint32	gDualMpuDataCopyFlag;
extern uint32	gDualDiuDataCopyFlag;


static uint32	localFbMemoryAddrInit(uint32	);
static uint32	busFbMemoryAddrInit(uint32	);

uint32			fbMemoryClear(uint32, uint32, strFbTaskTblMemInfo*, uint32);
static uint32	localFbMemoryClear(uint32, uint32, strFbTaskTblMemInfo *, uint32);
static uint32	busFbMemoryClear(uint32, uint32, strFbTaskTblMemInfo *, uint32);

static uint32	fbParaCopyFromMem(uint32 ,uint32, uint32, uint32 *,strFbMemInfo *, uint32);

uint32	fbMemoryAddrInit(uint32	);
uint32	segmentStatusRead(uint32, uint32, strSegStatusInfo *);
uint32	segTblInfoWrite(uint32 , uint32 , strFbTaskTblMemInfo *);
uint32	busSegTblInfoWrite(uint32 , uint32 , strFbTaskTblMemInfo *);
uint32	getFbTaskTblInfo(uint32 , uint32 , strFbTaskTblMemInfo *);
uint32	busSegTblInfoRead(uint32 , uint32 , strFbTaskTblMemInfo *);
uint32	segTblPointerGet(uint32 ,uint32	,strFbTaskTblMemInfo	**);
uint32	busSegTblPointerGet(uint32 ,uint32	,strFbTaskTblMemInfo	**);

uint32	fbMemInfoValidCheck(uint32, strFbMemInfo *);
uint32	fbMemInfoRead(uint32, uint32, strFbMemInfo *);
uint32	busFbMemInfoRead(uint32, uint32, strFbMemInfo *);
uint32	fbCodeMemInfoRead(uint32, uint32, strFbMemInfo *);
uint32  fbCodeMemPointerGet(uint32, uint32, strFbMemInfo **);
uint32  busFbCodeMemPointerGet(uint32, uint32, strFbMemInfo **);

uint32	busFbCodeMemInfoRead(uint32, uint32, strFbMemInfo *);
uint32	fbMemPointerGet(uint32 , strMemPointerInfo *);
uint32	busFbMemPointerGet(uint32 , strMemPointerInfo *);
uint32	fbCodeParaTypePointerGet(uint32	,strFbCodeTypePointerInfo	*);

void	fbMemoryClearCheck(void);
void	moduleFbMemoryClear(uint32,uint32);
uint32	busMemoryDataCopyToLocalMem(uint32);
void	busMemFbDataCopyToLocalFbMem(void);

uint32	segTblInfoClear(uint32, uint32);
uint32	busSegTblInfoClear(uint32, uint32);
uint32	getLogicTaskCycletime(uint32 , uint32 , float *);

uint32	rmtbusFbMemPointerGet(uint32,strMemPointerInfo	*);
void	fbRunningErrorInfoDisplay(uint32, uint32);
void	cyclicSegExecTimeCalcu(uint32, strExecTimeInfo *);

uint32	segmentBusDataChecksumCheck(uint32 );
uint32	fbDataWriteToRemoteVmeMemBoard(uint32,uint32,uint32,uint32,uint32,strFbMemInfo	*);

void	minFreeTimeCalcu(uint32, uint32, uint32);
       	
uint32	readRuntimeFbData(uint32, uint32, uint32, uint32 *);
uint32	writeRuntimeFbData(uint32, uint32, uint32, uint32 *);

uint32	fbInOutputTypeCasting(uint32, uint32, uint32, uint32 *);

uint32 jmpFbCodeCheck(uint32);
uint32 jmpFbAddressCheck(uint32,uint32,strFbMemInfo*);

uint32 specDataNegated(strFbSpecActionInfo*, uint32*);
uint32 segSchedulerCountRead(uint32 , uint32 *);
uint32 fbSysMaxJmpLoopInfo(uint32);

uint32  segTblTickCountWrite(uint32,uint32,strFbTaskTblMemInfo	*);
uint32	allSegmentModeErrSet(uint32);
uint32	allSegmentModeStopSet(uint32);

uint32 fbCodeVarWriteEnoRead(uint32, uint32, uint32*);
static uint32 fbDataWriteToRemoteLocalMemory(uint32,uint32,uint32,uint32,uint32,strFbMemInfo *);

uint32	fbCodeBlockAddrReadWithInput(uint32, uint32, uint32, uint32 *);		
static uint32	fbAddrCopyFromMem(uint32, uint32, uint32, uint32 *, strFbMemInfo *, uint32);


extern void		memoryCopy(uint8 *, uint8 *, uint32);
extern void		memoryClear(uint8 *, uint32);
extern uint32	myBusDataAcc (uint8 *, uint32, uint32,uint32,uint8 *, uint8 *);
extern uint32	fbDbTableSortingInfoRead(uint32, strFbExecDbInfo *);
extern uint32	systemCnfgInfoRead( strSysConfigInfo *);
extern void		setErrorCode(uint8 *,uint32,uint8 *,uint32);
extern uint32	segmentChecksumCalcu (strFbTaskTblMemInfo *, strFbMemInfo *,uint32	*, uint32);
extern void		sysTaskDelay (int32 );
extern int32	systemClockRateGet(void);
extern void		setErrorCodeWithVal(uint8 *,uint32,uint8 *,uint32, uint8 *,uint32,
							uint8 *,uint32,uint8 *,uint32);
extern int32	ioRead(int32 , int8 *, int32 );
extern uint32	dualFlagCheckInRunShelf(void);
extern int32	systemAuxClockRateGet();
extern void		fourBytesDataCopy ( uint32 *, uint32 *, uint32 );

extern strFbExecutionInfo *fbExecIdStartMemInfoGet(uint32 );
extern strFbExecutionInfo *fbExecIdNextMemInfoGet(uint32 , strFbExecutionInfo *);

extern strFbExecutionInfo *actExecIdStartMemInfoGet(uint32);
extern strFbExecutionInfo *trsExecIdStartMemInfoGet(uint32);

extern strFbExecutionInfo *trsExecIdNextMemInfoGet(uint32 , strFbExecutionInfo *);
extern strFbExecutionInfo *actExecIdNextMemInfoGet(uint32 , strFbExecutionInfo *);

extern uint32   varRetainInfoReadwithRef(uint32, strFbSpecActionInfo, uint32*);
extern uint32  refDataCopyToBusMem(uint32*, uint32, strFbSpecActionInfo, uint32);
extern uint32  refDataCopyToRmtMem(uint32*, uint32, strFbSpecActionInfo, uint32);

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
extern uint32 myRmtDataAcc(uint8 *, uint32, uint32, uint32, int8 *, int8 *);
extern int32	myRmtFourByteDataWrite(	uint32, int8 *, int8 *);
#endif
extern char		*ctime (const time_t *tp);

extern  uint32  refDataCopyFromMem(uint32*, uint32, strFbSpecActionInfo, uint32,uint32);
extern  uint32  refDataCopyFromBusMem(uint32*, uint32, strFbSpecActionInfo, uint32,uint32);
extern  uint32  refDataCopyToMem(uint32*, uint32, strFbSpecActionInfo, uint32);
extern void		systemRasInfoSet(uint32 , uint32 );

extern  strTrsLinkInfo *trsLinkNextMemInfoGet(uint32 , strTrsLinkInfo *);
extern  strTrsLinkInfo *trsLinkStartMemInfoGet(uint32 );
extern strActionLinkInfo *actLinkNextMemInfoGet(uint32 , strActionLinkInfo *);
extern strActionLinkInfo *actLinkStartMemInfoGet(uint32);


uint32	fbSysExeModuleInfoRead( uint32, strModuleExecInfo* );
uint32	fbSysExeModuleInfoWrite( uint32, strModuleExecInfo* );
uint32  fbSysExeVarInfoWrite(uint32, strVariableExecInfo* );
uint32  fbSysExeVarInfoRead(uint32 , strVariableExecInfo* );
uint32  fbSysExeSfcInfoRead(uint32,uint32*);
uint32  fbSysExeSfcInfoWrite(uint32,uint32);

uint32  fbSysExeInfoPointerGet(strSysExecInfo** ,uint32 );

uint32	moduleInfoRead(uint32, strModuleInfo	*);

uint32	fbCodeSpecActionRead(strFbDataReadInfo*, strFbSpecActionInfo*, uint32);
uint32	fbCodeSpecDataReadAll(strFbDataReadInfo*, uint32*, uint32);
uint32	fbCodeSpecDataReadWithNumber(uint32, uint32, uint32, uint32, uint32,  uint32*, uint32*, uint32*);
uint32	fbCodeOutputDataReadWithNumber(uint32, uint32, uint32, uint32, uint32, uint32 * ,uint32 *);

uint32	fbCodeSpecDataReadWithType(strFbDataReadInfo*, uint32*, uint32);
uint32	fbCodeVarDataReadWithType(strFbDataReadInfo*, uint32*, uint32);
uint32	fbCodeOutputDataReadWithType(strFbDataReadInfo*, uint32*, uint32);

uint32	fbCodeSpecDataWriteWithType(uint32,uint32,uint32,uint32,uint32,uint32*);
uint32	fbCodeOutputDataWriteWithType(uint32,uint32,uint32,uint32,uint32,uint32,uint32*);
uint32	busFbCodeOutputDataWriteWithType(uint32, uint32, uint32, uint32, uint32,uint32,uint32 *);

uint32	fbCodeParaReadWithAddr(uint32, uint32, uint32, uint32 *);
uint32  fbCodeParaWriteWithVar(uint32, uint32, uint32, uint32 *, uint8*);

uint32	fbCodeVarDataWriteWithType(uint32,uint32,uint32,uint32,uint32, uint32*);	
void	sysExeInfoMemoryClear(uint32);
uint32  localFbCodeMemClear(uint32 , uint32 , uint32 );
uint32  busFbCodeMemClear(uint32 , uint32 , uint32 );
extern uint32	sysAuxTickCountGet(void);
extern uint32	nvRamRead(int8 *, int32 , int32 );

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
uint32  myRmtRunDataWrite(strAccumBusInfo *);
extern uint32	myRmtAllTypeDataWrite(	uint32,int8 *, int8 *, int8 *);
#endif


uint32	fbMemoryDef( uint32 memoryBoardType)
{
	uint32	status = NO_ERROR;

	switch(memoryBoardType) {
		case MEM_611_BOARD: 
			VME_MEMORY_BAORD_SIZE = 0x200000;
	
			
			MAX_MPU_BLOCK_ADDR	= 5000;      
			
			
			MAX_DIU_BLOCK_ADDR	= MAX_DIU_BLOCK_ADDR_NUM;		

			
			MAX_MPU_SPEC_NUM	= MAX_MPU_BLOCK_ADDR * 10;	
			MAX_MPU_OUTPUT_NUM	= MAX_MPU_BLOCK_ADDR * 4;	
			MAX_DIU_SPEC_NUM	= 150000;	
			MAX_DIU_OUTPUT_NUM	= MAX_IO_OUTPUT_NUM + MAX_TMTC_OUTPUT_NUM;

			
			MAX_MPU_AUTOVAR_NUM = MEM_MPU_AUTOVAR_NUM;	
			MAX_MPU_MMEM_NUM 	= MEM_MPU_MMEM_NUM;		
			MAX_MPU_RETAIN_NUM 	= MAX_MPU_AUTOVAR_NUM + MAX_MPU_MMEM_NUM;
			MAX_MPU_RETAIN_FLAG_MEM_SIZE = (MAX_MPU_RETAIN_NUM/8);	

			MAX_MPU_VAR_SIZE =  MAX_MPU_RETAIN_FLAG_MEM_SIZE +	
								(MAX_MPU_AUTOVAR_NUM+MAX_MPU_MMEM_NUM)*4 +	
								(MAX_MPU_AUTOVAR_NUM+MAX_MPU_MMEM_NUM)*4 +  
								 MAX_SHARED_MEM_SIZE;

			
			MAX_MPU_MEM_SIZE	= MAX_LOGIC_TASK_NUM*sizeof(strFbTaskTblMemInfo) +	
								  MAX_MPU_BLOCK_ADDR*sizeof(strFbMemInfo) +		
								  MAX_MPU_SPEC_NUM*sizeof(uint32) +				
								  MAX_MPU_OUTPUT_NUM*sizeof(uint32) +			
								  MAX_MPU_VAR_SIZE +							
								  MAX_STEP_NUM*sizeof(strStepInfo) +			
								  MAX_TRANSITOIN_NUM*sizeof(strTransitionInfo) +	
								  MAX_ACTION_NUM*sizeof(strActionInfo) +		
								  MAX_SYS_MEM_SIZE; 							

			MAX_MPU_DATA_SIZE	= MAX_MPU_MEM_SIZE - MAX_SYS_MEM_SIZE;
			
			MAX_DIU_MEM_SIZE	= MAX_DIU_SEG_NUM*sizeof(strFbTaskTblMemInfo) +	
								  MAX_DIU_BLOCK_ADDR*sizeof(strFbMemInfo) +		
								  MAX_DIU_SPEC_NUM*sizeof(uint32) +				
								  MAX_DIU_OUTPUT_NUM*sizeof(uint32) +			
								  sizeof(strFMCEngFileDwlInfo) + MAX_ENG_FILE_SIZE;	
		break;
		 
#if ((BOARD_TYPE == MVME2100_BOARD) || (BOARD_TYPE == X86_PC_BOARD))
		case MEM_BOARD_NONE:
#endif
#if (BOARD_TYPE == LKV120_BOARD)
		case MEM_4MB_MRAM:
#endif
		case MEM_612_BOARD: 
			VME_MEMORY_BAORD_SIZE = 0x400000;

			
			MAX_MPU_BLOCK_ADDR	= 20000;

			
			MAX_DIU_BLOCK_ADDR	= MAX_DIU_BLOCK_ADDR_NUM;		
		
			
			MAX_MPU_SPEC_NUM	= MAX_MPU_BLOCK_ADDR * 10;	
			MAX_MPU_OUTPUT_NUM	= MAX_MPU_BLOCK_ADDR * 4;	

			
			MAX_DIU_SPEC_NUM	= 285000;	
			MAX_DIU_OUTPUT_NUM	= MAX_IO_OUTPUT_NUM + MAX_TMTC_OUTPUT_NUM;

			
			MAX_MPU_AUTOVAR_NUM 	= MEM_MPU_AUTOVAR_NUM;
			MAX_MPU_MMEM_NUM 		= MEM_MPU_MMEM_NUM;
			MAX_MPU_RETAIN_NUM 		= MAX_MPU_AUTOVAR_NUM + MAX_MPU_MMEM_NUM;
			MAX_MPU_RETAIN_FLAG_MEM_SIZE = (MAX_MPU_RETAIN_NUM/8);	

			MAX_MPU_VAR_SIZE =  MAX_MPU_RETAIN_FLAG_MEM_SIZE +
								(MAX_MPU_AUTOVAR_NUM+MAX_MPU_MMEM_NUM)*4 +
								(MAX_MPU_AUTOVAR_NUM+MAX_MPU_MMEM_NUM)*4 +  
								 MAX_SHARED_MEM_SIZE;

			
			
			MAX_MPU_MEM_SIZE	= MAX_LOGIC_TASK_NUM*sizeof(strFbTaskTblMemInfo) +	
								  MAX_MPU_BLOCK_ADDR*sizeof(strFbMemInfo) +		
								  MAX_MPU_SPEC_NUM*sizeof(uint32) +				
								  MAX_MPU_OUTPUT_NUM*sizeof(uint32) +			
								  MAX_MPU_VAR_SIZE +							
								  MAX_STEP_NUM*sizeof(strStepInfo) +			
								  MAX_TRANSITOIN_NUM*sizeof(strTransitionInfo) +	
								  MAX_ACTION_NUM*sizeof(strActionInfo) +		
								  MAX_SYS_MEM_SIZE; 							
								  
			MAX_MPU_DATA_SIZE	= MAX_MPU_MEM_SIZE - MAX_SYS_MEM_SIZE;
			
			MAX_DIU_MEM_SIZE	= MAX_DIU_SEG_NUM*sizeof(strFbTaskTblMemInfo) +	
								  MAX_DIU_BLOCK_ADDR*sizeof(strFbMemInfo) +		
								  MAX_DIU_SPEC_NUM*sizeof(uint32) +				
								  MAX_DIU_OUTPUT_NUM*sizeof(uint32) +			
								  sizeof(strFMCEngFileDwlInfo) + MAX_ENG_FILE_SIZE;				
		break;
		
#if ((BOARD_TYPE == MVME3100_BOARD)||(BOARD_TYPE == MVME4100_BOARD)||(BOARD_TYPE == MVME2500_BOARD)||(BOARD_TYPE == LKV120_BOARD))
		case MEM_BOARD_NONE:
		case MEM_613_BOARD: 
			VME_MEMORY_BAORD_SIZE = 0x800000;

			
			MAX_MPU_BLOCK_ADDR	= 40000;

			
			MAX_DIU_BLOCK_ADDR	= MAX_DIU_BLOCK_ADDR_NUM;		
		
			
			MAX_MPU_SPEC_NUM	= MAX_MPU_BLOCK_ADDR * 10;	
			MAX_MPU_OUTPUT_NUM	= MAX_MPU_BLOCK_ADDR * 4;	

			
			MAX_DIU_SPEC_NUM	= 285000;	
			MAX_DIU_OUTPUT_NUM	= MAX_IO_OUTPUT_NUM + MAX_TMTC_OUTPUT_NUM;

			
			MAX_MPU_AUTOVAR_NUM 	= MEM_MPU_AUTOVAR_NUM;
			MAX_MPU_MMEM_NUM 		= MEM_MPU_MMEM_NUM;
			MAX_MPU_RETAIN_NUM 		= MAX_MPU_AUTOVAR_NUM + MAX_MPU_MMEM_NUM;
			MAX_MPU_RETAIN_FLAG_MEM_SIZE = (MAX_MPU_RETAIN_NUM/8);	

			MAX_MPU_VAR_SIZE =  MAX_MPU_RETAIN_FLAG_MEM_SIZE +
								(MAX_MPU_AUTOVAR_NUM+MAX_MPU_MMEM_NUM)*4 +
								(MAX_MPU_AUTOVAR_NUM+MAX_MPU_MMEM_NUM)*4 +  
								 MAX_SHARED_MEM_SIZE;

			
			
			MAX_MPU_MEM_SIZE	= MAX_LOGIC_TASK_NUM*sizeof(strFbTaskTblMemInfo) +	
								  MAX_MPU_BLOCK_ADDR*sizeof(strFbMemInfo) +		
								  MAX_MPU_SPEC_NUM*sizeof(uint32) +				
								  MAX_MPU_OUTPUT_NUM*sizeof(uint32) +			
								  MAX_MPU_VAR_SIZE +							
								  MAX_STEP_NUM*sizeof(strStepInfo) +			
								  MAX_TRANSITOIN_NUM*sizeof(strTransitionInfo) +	
								  MAX_ACTION_NUM*sizeof(strActionInfo) +		
								  MAX_SYS_MEM_SIZE; 							
								  
			MAX_MPU_DATA_SIZE	= MAX_MPU_MEM_SIZE - MAX_SYS_MEM_SIZE;
			
			MAX_DIU_MEM_SIZE	= MAX_DIU_SEG_NUM*sizeof(strFbTaskTblMemInfo) +	
								  MAX_DIU_BLOCK_ADDR*sizeof(strFbMemInfo) +		
								  MAX_DIU_SPEC_NUM*sizeof(uint32) +				
								  MAX_DIU_OUTPUT_NUM*sizeof(uint32) +			
								  sizeof(strFMCEngFileDwlInfo) + MAX_ENG_FILE_SIZE;				
		break;
#endif
		default:
			printf( "[ERROR] - %s(%d) Memory Board Type Err !!\n", __FILE__, __LINE__);
			return(MEMORY_BOARD_TYPE_ERR);
		break;
	}
	MAX_MEMORY_SIZE  = VME_MEMORY_BAORD_SIZE;
	MAX_USR_MEM_SIZE = MAX_MPU_MEM_SIZE + MAX_DIU_MEM_SIZE;

	
	MAX_USABLE_MPU_MEM_SIZE	= MAX_MPU_MEM_SIZE - (MAX_MPU_VAR_SIZE+MAX_SYS_MEM_SIZE);
	MAX_USABLE_DIU_MEM_SIZE = MAX_DIU_MEM_SIZE;
    MAX_USABLE_TMTC_MEM_SIZE = MAX_DIU_MEM_SIZE;
	MAX_USABLE_FIELDBUS_MEM_SIZE = MAX_DIU_MEM_SIZE;	
	
	
	MEMORY_BOARD_LOCAL_ADDR	= (uint8 *)malloc(MAX_USR_MEM_SIZE);
	
	if(MEMORY_BOARD_LOCAL_ADDR == NULL) {
		status = MEMORY_ALLOC_ERR;
		printf( "[ERROR] - %s(%d) Memory Alloc Err !!\n", __FILE__, __LINE__);
		return(status);	
	}
	else
		memoryClear( (uint8 *)MEMORY_BOARD_LOCAL_ADDR, MAX_USR_MEM_SIZE);
		
	if(		(MEMORY_BOARD_BUS_EMUL_ADDR == 0)
#if (BOARD_TYPE == LKV120_BOARD)
		&&	((memoryBoardType==MEM_BOARD_NONE)||(memoryBoardType==MEM_4MB_MRAM))	)
#else
		&&	((memoryBoardType==MEM_BOARD_NONE))	)
#endif
	{
		if(memoryBoardType == MEM_BOARD_NONE)
		{
			MEMORY_BOARD_BUS_EMUL_ADDR = (uint8 *)malloc(MEM613BOARD_END_OFFSET+1);
			memoryClear( (uint8 *)MEMORY_BOARD_BUS_EMUL_ADDR, (MEM613BOARD_END_OFFSET+1));
		}
#if (BOARD_TYPE == LKV120_BOARD)
		else if(memoryBoardType == MEM_4MB_MRAM)
		{
			MEMORY_BOARD_BUS_EMUL_ADDR = (uint8 *)MRAM_START_ADDR;
		}
#endif
	}
		
#if (BOARD_TYPE == LKV120_BOARD)
	if((memoryBoardType==MEM_BOARD_NONE)||(memoryBoardType==MEM_4MB_MRAM)){	
#else
	if(memoryBoardType==MEM_BOARD_NONE){	
#endif
		MPU_FB_MEMORY_START_BUS_ADDR	  = MEMORY_BOARD_BUS_EMUL_ADDR;
		DIU_FB_MEMORY_START_BUS_ADDR	  = MEMORY_BOARD_BUS_EMUL_ADDR + MAX_MPU_MEM_SIZE;
	}
	else {
		MPU_FB_MEMORY_START_BUS_ADDR	  = MEMORY_BOARD_BUS_ADDR;
		DIU_FB_MEMORY_START_BUS_ADDR	  = MEMORY_BOARD_BUS_ADDR + MAX_MPU_MEM_SIZE;
	}

	
	
	return(status);
}


uint32 fbMemoryAddrInit(uint32 moduleId)
{
	uint32 status = NO_ERROR;

    status = localFbMemoryAddrInit(moduleId);
    if(status != NO_ERROR) {
		printf( "[ERROR] - local Memory Init Error !!\n");
        return(status);
    }

    status = busFbMemoryAddrInit(moduleId);
    if(status != NO_ERROR) {
		printf( "[ERROR] - Bus Memory Init Error !!\n");
    }
    return(status);
}


static uint32	localFbMemoryAddrInit(uint32 moduleId)
{
	uint32	status = NO_ERROR;
	uint32	fbStartAddr, fbSegTblStartAddr ,fbCodeInfoAddr ;
	uint32	fbSpecAndVarStartAddr, fbOutputStartAddr;
	uint32	sharedMemStartAddr;
	uint32  retainVarStartAddr, autoVarStartAddr, mMemVarStartAddr,sysExeInfoStartAddr;
	uint32  sfcStepStartAddr, sfcTransitionStartAddr, sfcActionStartAddr;
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
	uint32  autoVarInitAddr, mMemVarInitAddr;
#endif

	uint32 size1, size2;

    switch(moduleId) {
		case LOGIC_ID:
			fbStartAddr 	=  (uint32)MEMORY_BOARD_LOCAL_ADDR;
			gMpuFbStartAddr = (uint8 *)fbStartAddr;
			
			
			fbSegTblStartAddr 			= (uint32)fbStartAddr;
            gMpuFbSegTblInfoLocalPtr 	= (strFbTaskTblMemInfo *)fbSegTblStartAddr;

			
			fbCodeInfoAddr 			= fbSegTblStartAddr + sizeof(strFbTaskTblMemInfo) * MAX_LOGIC_TASK_NUM;
        	gMpuFbMemInfoLocalPtr 	= (strFbMemInfo *)fbCodeInfoAddr;
		
			
			fbSpecAndVarStartAddr 		= fbCodeInfoAddr + sizeof(strFbMemInfo) * MAX_MPU_BLOCK_ADDR;            	
 			gMpuSpecAndVarDataLocalPtr 	= (void *)fbSpecAndVarStartAddr;

			
			fbOutputStartAddr 		= fbSpecAndVarStartAddr + (MAX_MPU_SPEC_NUM * 4);
			gMpuOutputDataLocalPtr 	= (void *)fbOutputStartAddr;

            
			sharedMemStartAddr 			= fbOutputStartAddr + (MAX_MPU_OUTPUT_NUM * 4);
			gSTSharedMemDataLocalPtr 	= (uint32 *) sharedMemStartAddr;

			
			retainVarStartAddr 		= sharedMemStartAddr + MAX_SHARED_MEM_SIZE;
			gRetainVarInfoLocalPtr 	= (void *) retainVarStartAddr;

			autoVarStartAddr 		= retainVarStartAddr + MAX_MPU_RETAIN_FLAG_MEM_SIZE;
			gAutoVariableLocalPtr 	= (void *) autoVarStartAddr;

			mMemVarStartAddr 		= autoVarStartAddr + (MAX_MPU_AUTOVAR_NUM * 4 );
			gMmemVariableLocalPtr 	= (void *) mMemVarStartAddr;

#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
			
			autoVarInitAddr 		= mMemVarStartAddr +  (MAX_MPU_MMEM_NUM * 4 );
			gAutoVarInitLocalPtr 	= (void *) autoVarInitAddr;

			mMemVarInitAddr 		= autoVarInitAddr + (MAX_MPU_AUTOVAR_NUM * 4 );
			gMmemVarInitLocalPtr 	= (void *) mMemVarInitAddr;

			
			sfcStepStartAddr 		= mMemVarInitAddr + (MAX_MPU_MMEM_NUM * 4 );
#else
			sfcStepStartAddr = mMemVarStartAddr + (MAX_MPU_MMEM_NUM * 4 );
#endif
			gSFCStepInfoLocalPtr 	= (strStepInfo *)sfcStepStartAddr;

			sfcTransitionStartAddr 	= sfcStepStartAddr + sizeof(strStepInfo) * MAX_STEP_NUM;
			gSFCTransitionLocalPtr 	= (strTransitionInfo *)sfcTransitionStartAddr;

			sfcActionStartAddr 		= sfcTransitionStartAddr+ sizeof(strTransitionInfo) * MAX_TRANSITOIN_NUM;
			gSFCActionInfoLocalPtr 	= (strActionInfo *)sfcActionStartAddr;

			sysExeInfoStartAddr 	= sfcActionStartAddr + sizeof(strActionInfo) * MAX_ACTION_NUM;
			gSysExeInfoLocalPtr  	= (void *)sysExeInfoStartAddr;

            
            
			size1 = sysExeInfoStartAddr + MAX_SYS_MEM_SIZE;

			size2 = fbStartAddr + MAX_MPU_MEM_SIZE;

            if( size1 > size2 )
            	status = MPU_MEM_SIZE_EXCEED_ERR;

        break;

        default:
        	status = L_MODULE_ID_ERR;
		break;
	}

    return(status);
}

static uint32	busFbMemoryAddrInit(uint32 moduleId)
{
	uint32	status = NO_ERROR;
	uint32	fbStartAddr, fbSegTblStartAddr , fbCodeInfoAddr;
	uint32	fbSpecAndVarStartAddr, fbOutputStartAddr;
	uint32	sharedMemStartAddr;

	uint32  retainVarStartAddr, autoVarStartAddr, mMemVarStartAddr, sysExeInfoStartAddr;
	uint32  sfcStepStartAddr, sfcTransitionStartAddr, sfcActionStartAddr;
	
	uint32 size1, size2;
	uint32  engFileStartAddr;
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
    uint32  autoVarInitAddr, mMemVarInitAddr;
#endif
	
    switch(moduleId) {
		case LOGIC_ID:
            
            fbStartAddr = (uint32)MPU_FB_MEMORY_START_BUS_ADDR;
			gBusMpuFbStartAddr = (uint8 *)fbStartAddr;

			
			fbSegTblStartAddr = (uint32)fbStartAddr;
            gMpuFbSegTblInfoBusPtr = (strFbTaskTblMemInfo *)fbSegTblStartAddr;

			
			fbCodeInfoAddr = fbSegTblStartAddr + sizeof(strFbTaskTblMemInfo) * MAX_LOGIC_TASK_NUM;
        	gMpuFbMemInfoBusPtr = (strFbMemInfo *)fbCodeInfoAddr;
		
			
			fbSpecAndVarStartAddr = fbCodeInfoAddr + sizeof(strFbMemInfo) *MAX_MPU_BLOCK_ADDR;            	
			gMpuSpecAndVarDataBusPtr = (void *)fbSpecAndVarStartAddr;

			
			fbOutputStartAddr = fbSpecAndVarStartAddr + (MAX_MPU_SPEC_NUM * 4);
			gMpuOutputDataBusPtr = (void *)fbOutputStartAddr;

            
			sharedMemStartAddr = fbOutputStartAddr + (MAX_MPU_OUTPUT_NUM * 4);
			gSTSharedMemDataBusPtr = (uint32 *) sharedMemStartAddr;

			
			retainVarStartAddr = sharedMemStartAddr + MAX_SHARED_MEM_SIZE;
			gRetainVarInfoBusPtr = (void *) retainVarStartAddr;

			autoVarStartAddr = retainVarStartAddr + MAX_MPU_RETAIN_FLAG_MEM_SIZE;
			gAutoVariableBusPtr = (void *) autoVarStartAddr;

			mMemVarStartAddr = autoVarStartAddr + ( MAX_MPU_AUTOVAR_NUM * 4 );
			gMmemVariableBusPtr = (void *) mMemVarStartAddr;

#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
			
			autoVarInitAddr = mMemVarStartAddr +  (MAX_MPU_MMEM_NUM * 4 );
			gAutoVarInitBusPtr = (void *) autoVarInitAddr;

			mMemVarInitAddr = autoVarInitAddr + (MAX_MPU_AUTOVAR_NUM * 4 );
			gMmemVarInitBusPtr = (void *) mMemVarInitAddr;

			
			sfcStepStartAddr = mMemVarInitAddr + (MAX_MPU_MMEM_NUM * 4 );
#else
			sfcStepStartAddr = mMemVarStartAddr + (MAX_MPU_MMEM_NUM * 4 );
#endif
			gSFCStepInfoBusPtr = (strStepInfo *)sfcStepStartAddr;

			sfcTransitionStartAddr = sfcStepStartAddr + sizeof(strStepInfo) * MAX_STEP_NUM;
			gSFCTransitionBusPtr = (strTransitionInfo *)sfcTransitionStartAddr;

			sfcActionStartAddr = sfcTransitionStartAddr+ sizeof(strTransitionInfo) * MAX_TRANSITOIN_NUM;
			gSFCActionInfoBusPtr = (strActionInfo *)sfcActionStartAddr;

			sysExeInfoStartAddr = sfcActionStartAddr + sizeof(strActionInfo) * MAX_ACTION_NUM;
			gSysExeInfoBusPtr  = (void *)sysExeInfoStartAddr;

            
            
			size1 = sysExeInfoStartAddr + MAX_SYS_MEM_SIZE;
			size2 = fbStartAddr + MAX_MPU_MEM_SIZE;

            if( size1 > size2 )
            	status = MPU_MEM_SIZE_EXCEED_ERR;

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			else{
 				gVarAreaOffset = ((uint32)gSTSharedMemDataBusPtr-(uint32)gBusMpuFbStartAddr)&0x00ffffff;
				gSFCAreaOffset = ((uint32)gSFCStepInfoBusPtr-(uint32)gBusMpuFbStartAddr)&0x00ffffff;
				gSysExeOffset = ((uint32)gSysExeInfoBusPtr-(uint32)gBusMpuFbStartAddr)&0x00ffffff;
			}
#endif
			/* eng 파일 할당 시작 주소 */
			engFileStartAddr = (uint32)(gBusMpuFbStartAddr + MAX_MPU_MEM_SIZE);
			gEngFileInfoBusPtr = (void *)engFileStartAddr;
			gEngFileDataBusPtr = (void *)(engFileStartAddr + sizeof(strFMCEngFileDwlInfo));
			
			/* Fatal Error Log 메세지 할당 시작 주소 */
			FIRST_FATAL_ERR_MNGR_OFFSET = (uint32)gEngFileDataBusPtr + MAX_ENG_FILE_SIZE;
			FATAL_MSG_START_OFFSET   	= FIRST_FATAL_ERR_MNGR_OFFSET + sizeof(strFatalErrLogInfo);

            /* 이중화 관련 데이터를 위한 메모리 할당 시작 주소*/
            /* 메모리 보드의 맨 끝 부분 1K byte를 할당 한다 */
            gDualDataBusPtr 	= (uint32 *)MPU_FB_MEMORY_START_BUS_ADDR;
            gDualDataBusPtr 	+= VME_MEMORY_BAORD_SIZE/4; 
            gDualDataBusPtr		-= MAX_DUAL_MEM_SIZE/4;

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			/* 2005/07/07 : USER MAX 메모리 포인터 */
			gUserEndBusPtr = (uint32 *)((uint32)gDualDataBusPtr + MAX_DUAL_MEM_SIZE);
#endif
			/* 위에서 계산된 메모리 주소가 DIU가 사용할 수 있는 주소를 */
            /* 초과하는 지 Check */
			size1 = FIRST_FATAL_ERR_MNGR_OFFSET + FATAL_ERR_LOG_SIZE;

            if( size1 >= (uint32)(gDualDataBusPtr))
            	status = DIU_MEM_SIZE_EXCEED_ERR;	
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			else{
				gDiuAreaOffset = ((uint32)gBusDiufbStartAddr-(uint32)gBusMpuFbStartAddr)&0x00ffffff;
				gSysNvRamAreaOffset = (FIRST_FATAL_ERR_MNGR_OFFSET-(uint32)gBusMpuFbStartAddr)&0x00ffffff;
				gDualAreaOffset = ((uint32)gDualDataBusPtr-(uint32)gBusMpuFbStartAddr)&0x00ffffff;

				gUserEndOffset = (uint32)gUserEndBusPtr;
			}
#endif
        break;

        default:
        	status = L_MODULE_ID_ERR;
		break;
	}

    return(status);
}


uint32 segmentStatusRead
				(
		uint32 	moduleId,
        uint32	taskId,
        strSegStatusInfo	*segStatusPtr
        		)
{
	uint32	status = NO_ERROR;
	strFbTaskTblMemInfo	segTblInfo;

    memoryClear ( (uint8 *)&segTblInfo, sizeof(strFbTaskTblMemInfo));

    status = getFbTaskTblInfo(moduleId, taskId, &segTblInfo); 	
    if(status == NO_ERROR) {
    	segStatusPtr->taskId 			= segTblInfo.taskId;
        segStatusPtr->logicCheckSumStatus = segTblInfo.logicCheckSumStatus;
        segStatusPtr->taskMode			= segTblInfo.taskMode;
		segStatusPtr->segLoopStatus		= segTblInfo.loopStatus;
    }
    return(status);
}


uint32 segTblInfoWrite
				( 
			uint32 moduleId,
            uint32	taskId,
			strFbTaskTblMemInfo	*segTblPtr
				)
{
	uint32	status = NO_ERROR;
	strFbTaskTblMemInfo	*tmpPtr;

	status = segTblPointerGet(moduleId, taskId, &tmpPtr);

	if(status == NO_ERROR)
    	memoryCopy ( 	(uint8 *)tmpPtr,
        				(uint8 *)segTblPtr,
                        sizeof(strFbTaskTblMemInfo)
					);

    return(status);
}


uint32 busSegTblInfoWrite
				( 
			uint32 moduleId,
            uint32	taskId,
			strFbTaskTblMemInfo	*segTblPtr
				)
{
	uint32	status = NO_ERROR;
	strFbTaskTblMemInfo	*tmpPtr;

	status = busSegTblPointerGet(moduleId, taskId, &tmpPtr);

	if(status == NO_ERROR)
    	memoryCopy ( 	(uint8 *)tmpPtr,
        				(uint8 *)segTblPtr,
                        sizeof(strFbTaskTblMemInfo)
					);

    return(status);
}


uint32 segTblTickCountWrite
				( 
			uint32 moduleId,
            uint32	taskId,
			strFbTaskTblMemInfo	*segTblPtr
				)
{
	uint32	status = NO_ERROR;
	strFbTaskTblMemInfo *busSegTblPtr;

	strFbTaskTblMemInfo	*destPtr;
	uint32  segTblAddr;
	uint32	offSet, bitMask;
	strSysConfigInfo	sysCnfgInfo; 

	 
	status = busSegTblPointerGet(moduleId, taskId, &busSegTblPtr);
	if(status == NO_ERROR) {
		busSegTblPtr->taskTickCnt = segTblPtr->taskTickCnt;
		if(dualFlagCheckInRunShelf() != DUAL_CHECK_ENABLE) 
			return(status);
	}
	else{
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

    (void)systemCnfgInfoRead(&sysCnfgInfo);

    if(sysCnfgInfo.shelfType == MASTER_SHELF)
       	offSet = MEM_BOARD_BUS_ADDR_ON_SLAVE;
    else if(sysCnfgInfo.shelfType == SLAVE_SHELF) 
       	offSet = MEM_BOARD_BUS_ADDR_ON_MASTER;
    else {
		status = STATION_SHELF_CONFIG_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
        return(status);
    }

    bitMask = 0x00ffffff;

#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
	segTblAddr = ((uint32)gMpuFbSegTblInfoBusPtr + (uint32)( (taskId-1)*sizeof(strFbTaskTblMemInfo)) );
#else
	segTblAddr = ((uint32)gMpuFbSegTblInfoBusPtr- (uint32)gBusMpuFbStartAddr 
						+ (uint32)( (taskId-1)*sizeof(strFbTaskTblMemInfo)) );
#endif
	segTblAddr &= bitMask;
	segTblAddr += offSet;
	destPtr 	= (strFbTaskTblMemInfo*)segTblAddr;
	
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
	if((status == NO_ERROR)) {
		status = myBusDataAcc(	__FILE__,
								__LINE__,
								BUS_LONG_ACC, 
								BUS_DATA_WRITE, 
								(uint8*)&destPtr->taskTickCnt,
								(uint8*)&segTblPtr->taskTickCnt
							);
#else
	if(status == NO_ERROR) {
		status = myRmtDataAcc(__FILE__,__LINE__,BUS_LONG_ACC, BUS_DATA_WRITE, 
								(uint8*)&destPtr->taskTickCnt,
								(uint8*)&segTblPtr->taskTickCnt);
#endif
		
		if(status)
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
	}

    return(status);
}


uint32 busSegTblInfoClear
				( 
			uint32 moduleId,
            uint32	taskId
				)
{
	uint32	status = NO_ERROR;
	strFbTaskTblMemInfo	*tmpPtr;

	status = busSegTblPointerGet(moduleId, taskId, &tmpPtr);

	if(status == NO_ERROR)
    	memoryClear ( (uint8 *)tmpPtr, sizeof(strFbTaskTblMemInfo));

    return(status);
}


uint32 segTblInfoClear
				( 
			uint32 moduleId,
            uint32	taskId
				)
{
	uint32	status = NO_ERROR;
	strFbTaskTblMemInfo	*tmpPtr;

	status = segTblPointerGet(moduleId, taskId, &tmpPtr);

	if(status == NO_ERROR)
    	memoryClear ( (uint8 *)tmpPtr, sizeof(strFbTaskTblMemInfo));

    return(status);
}
	

uint32 getFbTaskTblInfo
				( 
			uint32 moduleId,
            uint32	taskId,
			strFbTaskTblMemInfo	*segTblPtr
				)
{
	uint32	status = NO_ERROR;

	switch(moduleId) {
    	case LOGIC_ID:
        	if( (taskId >= 1) && (taskId <= MAX_LOGIC_TASK_NUM))
            	memoryCopy ( 
						(uint8 *) segTblPtr,
   						(uint8 *)(gMpuFbSegTblInfoLocalPtr + (taskId - 1)),
						sizeof(strFbTaskTblMemInfo)
							);
			else
				status = L_SEG_ID_ERR; 
        break;

        default:
        	status = L_MODULE_ID_ERR;
		break;
    }

	return(status);
}



uint32 busSegTblInfoRead
				( 
			uint32 moduleId,
            uint32	taskId,
			strFbTaskTblMemInfo	*segTblPtr
				)
{
	uint32	status = NO_ERROR;

	switch(moduleId) {
    	case LOGIC_ID:
        	if( (taskId >= 1) && (taskId <= MAX_LOGIC_TASK_NUM))
            	memoryCopy ( 
						(uint8 *) segTblPtr,
   						(uint8 *)(gMpuFbSegTblInfoBusPtr + (taskId - 1)),
						sizeof(strFbTaskTblMemInfo)
							);
			else
				status = L_SEG_ID_ERR; 
        break;

        default:
        	status = L_MODULE_ID_ERR;
		break;
    }

	return(status);
}


uint32 segTblPointerGet
				( 
			uint32 moduleId,
            uint32	taskId,
			strFbTaskTblMemInfo	**segTblPtr
				)
{
	uint32	status = NO_ERROR;

	switch(moduleId) {
    	case LOGIC_ID:
        	if( (taskId >= 1) && (taskId <= MAX_LOGIC_TASK_NUM))
   				*segTblPtr = gMpuFbSegTblInfoLocalPtr + (taskId - 1);				
			else
				status = L_SEG_ID_ERR; 
        break;

        default:
        	status = L_MODULE_ID_ERR;
		break;
    }

	return(status);
}


uint32 busSegTblPointerGet
				( 
			uint32 moduleId,
            uint32	taskId,
			strFbTaskTblMemInfo	**segTblPtr
				)
{
	uint32	status = NO_ERROR;

	switch(moduleId) {
    	case LOGIC_ID:
        	if( (taskId >= 1) && (taskId <= MAX_LOGIC_TASK_NUM))
   				*segTblPtr = gMpuFbSegTblInfoBusPtr + (taskId - 1);				
			else
				status = L_SEG_ID_ERR; 
        break;

        default:
        	status = L_MODULE_ID_ERR;
		break;
    }

	return(status);
}
  

uint32	readRuntimeFbData
				( 
			uint32 	taskId,
			uint32	moduleId, 
			uint32	fbBlockAddr, 
			uint32 	*fbInfoPtr
				)
{
	uint32 status = NO_ERROR;
	strFbMemInfo	*fbMemInfoPtr;
	
    if(moduleId == LOGIC_ID) 
    { 
    	fbMemInfoPtr = gMpuFbMemInfoLocalPtr; 
    }
	else 
		return(L_MODULE_ID_ERR);
 
   	
   	fbMemInfoPtr += (fbBlockAddr-1);

 	
    status = fbParaCopyFromMem (	taskId, 
									moduleId,
									fbBlockAddr, 
									(uint32 *)fbInfoPtr, 
									fbMemInfoPtr, 
									WITH_VAL
								);
    return(status);
}


uint32	fbCodeParaReadWithAddr
				( 
			uint32 	taskId,
			uint32	moduleId, 
			uint32	fbBlockAddr, 
			uint32 	*fbInfoPtr
				)
{
	uint32 status = NO_ERROR;
	strFbMemInfo	*fbMemInfoPtr;
	
    if(moduleId == LOGIC_ID)  
    	fbMemInfoPtr = gMpuFbMemInfoLocalPtr;
	else 
		return(L_MODULE_ID_ERR);
 
   	
   	fbMemInfoPtr += (fbBlockAddr-1);

	
    status = fbParaCopyFromMem (	taskId, 
									moduleId,
									fbBlockAddr, 
									(uint32 *)fbInfoPtr, 
									fbMemInfoPtr, 
									WITH_ADDR
								);

    return(status);
}


uint32	writeRuntimeFbData
				( 
			uint32 	taskId,
			uint32	moduleId, 
			uint32	fbBlockAddr, 
			uint32 	*fbInfoPtr
				)
{
	strFbMemInfo	*fbMemPtr, *busFbMemPtr;
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32	*specTypePtr, *tmpPtr, *varTypePtr;
	uint32	*specStartPtr, *varStartPtr, *outputStartPtr;
	uint32	*busSpecStartPtr = NULL, *busVarStartPtr= NULL, *busOutputStartPtr= NULL;
	uint32	*rmtbusSpecStartPtr= NULL, *rmtbusVarStartPtr= NULL, *rmtBusOutputStartPtr= NULL;

	uint32	i = 0, localBusCopyFlag = DATA_SYNC_DISABLE,rmtBusCopyFlag = DATA_SYNC_DISABLE;
	uint32	offSet, bitMask;

	uint32	*specActionStartPtr = NULL;
	strFbSpecActionInfo *specActionDataPtr= NULL;
	strFbCodeTypePointerInfo typePInfo;
	uint32  *outputTypePtr, highBitFlag = FALSE;
	uint32	*dualCopyCheckSpecTypePtr,*dualCopyVarTypePtr,*dualCopyOutputTypePtr;
	uint32  paraDualCopyFlag = DATA_SYNC_DISABLE, fbDualCopyFlag = DATA_SYNC_DISABLE, outputNegated;

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	strAccumBusInfo sAccumFBData;
#endif

    if(gSysCnfgInfo.shelfType == MASTER_SHELF)
       	offSet = MEM_BOARD_BUS_ADDR_ON_SLAVE;
    else if(gSysCnfgInfo.shelfType == SLAVE_SHELF) 
       	offSet = MEM_BOARD_BUS_ADDR_ON_MASTER;
    else {
		status = STATION_SHELF_CONFIG_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
        return(status);
    }
   
    bitMask = 0x00ffffff;

   	if(moduleId == LOGIC_ID)
   	{ 
		
		fbMemPtr 		= gMpuFbMemInfoLocalPtr;
	
		specStartPtr 	= (uint32 *)gMpuSpecAndVarDataLocalPtr;
		varStartPtr 	= (uint32 *)gMpuSpecAndVarDataLocalPtr;
		outputStartPtr 	= (uint32 *)gMpuOutputDataLocalPtr;
		specActionStartPtr= (uint32 *)gMpuSpecAndVarDataLocalPtr;

		
		busFbMemPtr 		= gMpuFbMemInfoBusPtr;
		
		busSpecStartPtr 	= (uint32 *)gMpuSpecAndVarDataBusPtr;
		busVarStartPtr 		= (uint32 *)gMpuSpecAndVarDataBusPtr;
		busOutputStartPtr 	= (uint32 *)gMpuOutputDataBusPtr;	
		
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
		rmtbusSpecStartPtr 		= (uint32 *)(((uint32)gMpuSpecAndVarDataBusPtr & bitMask)+offSet);
		rmtbusVarStartPtr 		= (uint32 *)(((uint32)gMpuSpecAndVarDataBusPtr & bitMask)+offSet);
		rmtBusOutputStartPtr 	= (uint32 *)(((uint32)gMpuOutputDataBusPtr & bitMask)+offSet);
#else 
		rmtbusSpecStartPtr 		= (uint32 *)((((uint32)gMpuSpecAndVarDataBusPtr-(uint32)gBusMpuFbStartAddr) & bitMask)+offSet);
		rmtbusVarStartPtr 		= (uint32 *)((((uint32)gMpuSpecAndVarDataBusPtr-(uint32)gBusMpuFbStartAddr) & bitMask)+offSet);
		rmtBusOutputStartPtr 	= (uint32 *)((((uint32)gMpuOutputDataBusPtr-(uint32)gBusMpuFbStartAddr) & bitMask)+offSet);
#endif
	}
	else return(L_MODULE_ID_ERR);

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	
	sAccumFBData.accDataNum = 0;
#endif
   	
   	fbMemPtr 		+= (fbBlockAddr-1);
	specStartPtr	+= fbMemPtr->specIndex;
	varStartPtr		+= fbMemPtr->varIndex;
	outputStartPtr	+= fbMemPtr->outputIndex;

	if(moduleId == LOGIC_ID) 
	{
		specActionStartPtr	+= fbMemPtr->specActionIndex;
		specActionDataPtr	= (strFbSpecActionInfo *)specActionStartPtr;
	}
 
	
	busFbMemPtr 		+= (fbBlockAddr-1);
	busSpecStartPtr		+= fbMemPtr->specIndex;
	busVarStartPtr		+= fbMemPtr->varIndex;
	busOutputStartPtr	+= fbMemPtr->outputIndex;

	
	rmtbusSpecStartPtr		+= fbMemPtr->specIndex;
	rmtbusVarStartPtr		+= fbMemPtr->varIndex;
	rmtBusOutputStartPtr	+= fbMemPtr->outputIndex;

	
	status = fbCodeParaTypePointerGet(
								fbMemPtr->fbId,
								(strFbCodeTypePointerInfo *)&typePInfo
								);
	if(status != NO_ERROR) return(status);

	specTypePtr 	= (uint32 *)typePInfo.specTypeStartAddr;
	varTypePtr 		= (uint32 *)typePInfo.varTypeStartAddr;
	outputTypePtr 	=  (uint32 *)typePInfo.outputTypeStartAddr;

	
	if( (gSysRunningInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN) &&
		(gSysRunningInfoPtr->sysOperationMode == SYS_RAS_SYSTEM_AUTO_MODE) &&
		(gDiuRasInfoPtr->redundancyStatus == NO_ERROR)
	){
		
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
		localBusCopyFlag 	= DATA_SYNC_ENABLE;
		rmtBusCopyFlag 		= DATA_SYNC_ENABLE;
#elif (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
		localBusCopyFlag 	= 0;
		rmtBusCopyFlag 		= 0;
#endif
	}
	else 
	{ 
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
		
		localBusCopyFlag 	= DATA_SYNC_ENABLE;
		rmtBusCopyFlag 		= DATA_SYNC_DISABLE;
#elif (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
		rmtBusCopyFlag = 1;
#endif
	}
	
	/* Default '1' */
	fbDualCopyFlag = DATA_SYNC_ENABLE;

	tmpPtr = fbInfoPtr;

    
	for(i=0; i < fbMemPtr->inputSize; i++,tmpPtr++, specStartPtr++,busSpecStartPtr++, rmtbusSpecStartPtr++)
	{	
		
		dualCopyCheckSpecTypePtr = specTypePtr;

		if (moduleId == LOGIC_ID)
		{
			
			if(specActionDataPtr->refType==SPEC_TYPE_BYVAL) {
				
				if( CHK_IS_BIT_TYPE(*specTypePtr) &&(specActionDataPtr->negated ==0x1) ) 
					retVal= specDataNegated(specActionDataPtr, tmpPtr);
					if(retVal) 
						setErrorCode( __FILE__, __LINE__, __FUNCTION__, retVal);

				if(GET_TYPE_SIZE(*specTypePtr)==SIZE32_TYPE) { 
					specTypePtr++;
					specActionDataPtr++;
				}
				else if( (GET_TYPE_SIZE(*specTypePtr)==SIZE64_TYPE)&&(highBitFlag==FALSE) ) { 
					highBitFlag =TRUE;
				}
				else if( (GET_TYPE_SIZE(*specTypePtr)==SIZE64_TYPE)&&(highBitFlag==TRUE) ) { 
					highBitFlag =FALSE;
					specTypePtr++;
					specActionDataPtr++;
				}
			}
			else { 
				if(GET_TYPE_SIZE(*specTypePtr)==SIZE32_TYPE) { 
					specTypePtr++;
					specActionDataPtr++;
				}
				else if( (GET_TYPE_SIZE(*specTypePtr)==SIZE64_TYPE)&&(highBitFlag==FALSE) ) { 
					highBitFlag =TRUE;
				}
				else if( (GET_TYPE_SIZE(*specTypePtr)==SIZE64_TYPE)&&(highBitFlag==TRUE) ) { 
					highBitFlag =FALSE;
					specTypePtr++;
					specActionDataPtr++;
				}
				
				continue;
			}
		}
		else{
			specTypePtr++;
		}
	
		
		if(*specStartPtr == *tmpPtr){
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
			
			if(	((moduleId == LOGIC_ID) && (gDualMpuDataCopyFlag == DATA_SYNC_ENABLE))
			){				
				*specStartPtr = *tmpPtr;
				
				if (fbDualCopyFlag == DATA_SYNC_ENABLE)
					paraDualCopyFlag = GET_PARA_COPY_FLAG(*dualCopyCheckSpecTypePtr);

				goto RMT_PARA_COPY;
			}
			else
				continue;
#elif (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			continue;
#endif
		}
		
		 
		*specStartPtr = *tmpPtr;
	
		if (fbDualCopyFlag == DATA_SYNC_ENABLE)
			paraDualCopyFlag = GET_PARA_COPY_FLAG(*dualCopyCheckSpecTypePtr);
			
		
		if((localBusCopyFlag == DATA_SYNC_ENABLE)&&(paraDualCopyFlag == DATA_SYNC_ENABLE)) {
			retVal = myBusDataAcc(	__FILE__,
								__LINE__,	
								BUS_LONG_ACC,  
								BUS_DATA_WRITE ,
								(uint8 *)busSpecStartPtr, 
								(uint8 *)tmpPtr
					 		);
			if(retVal)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, retVal);
		}
	
RMT_PARA_COPY:
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
		if((rmtBusCopyFlag == DATA_SYNC_ENABLE) && (paraDualCopyFlag == DATA_SYNC_ENABLE)) {
#elif (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
		if((rmtBusCopyFlag == NO_ERROR) &&(paraDualCopyFlag == DATA_SYNC_ENABLE)) 
		{
#endif
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			sAccumFBData.accumPtr[sAccumFBData.accDataNum] = rmtbusSpecStartPtr;
			sAccumFBData.accumData[sAccumFBData.accDataNum] = *tmpPtr;
			sAccumFBData.accDataNum++;
#else
			retVal = myBusDataAcc(	__FILE__,
									__LINE__,	
									BUS_LONG_ACC,  
									BUS_DATA_WRITE ,
									(uint8 *)rmtbusSpecStartPtr, 
									(uint8 *)tmpPtr
						 		);
#endif
			if(retVal)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, retVal);
		}
	} 

    tmpPtr = fbInfoPtr + fbMemPtr->inputSize;
    for(i = 0; i < fbMemPtr->intlVarSize; i++,varStartPtr++,busVarStartPtr++, rmtbusVarStartPtr++,
    																		tmpPtr++ ) {
		
		
		dualCopyVarTypePtr = varTypePtr;

		
		varTypePtr++;

		
		if( *varStartPtr == *tmpPtr){
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
			
			if(	((moduleId == LOGIC_ID) && (gDualMpuDataCopyFlag == DATA_SYNC_ENABLE))
			){
				*varStartPtr = *tmpPtr;  
		
				if (fbDualCopyFlag == DATA_SYNC_ENABLE)
					paraDualCopyFlag = GET_PARA_COPY_FLAG(*dualCopyVarTypePtr);

				goto RMT_VAR_COPY;
			}
			else
				continue;
#elif (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			continue;
#endif
		}

		 
		*varStartPtr = *tmpPtr;  

		if (fbDualCopyFlag == DATA_SYNC_ENABLE)
			paraDualCopyFlag = GET_PARA_COPY_FLAG(*dualCopyVarTypePtr);

		
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
		if((localBusCopyFlag == DATA_SYNC_ENABLE)&&(paraDualCopyFlag == DATA_SYNC_ENABLE)){
#elif (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
		if((localBusCopyFlag == NO_ERROR)&&(paraDualCopyFlag == DATA_SYNC_ENABLE)){
#endif
			retVal = myBusDataAcc(	__FILE__,
									__LINE__,	
									BUS_LONG_ACC, 
									BUS_DATA_WRITE ,
									(uint8 *)busVarStartPtr, 
									(uint8 *)tmpPtr
						 		);
			if(retVal)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, retVal);
				
		}

		
		
RMT_VAR_COPY:
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
		if((rmtBusCopyFlag == DATA_SYNC_ENABLE) &&(paraDualCopyFlag == DATA_SYNC_ENABLE)) {
#elif (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
		if((rmtBusCopyFlag == NO_ERROR) &&(paraDualCopyFlag == DATA_SYNC_ENABLE)) {
#endif

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			
			sAccumFBData.accumPtr[sAccumFBData.accDataNum] = rmtbusVarStartPtr;
			sAccumFBData.accumData[sAccumFBData.accDataNum] = *tmpPtr;
			sAccumFBData.accDataNum++;
#else
			retVal = myBusDataAcc(	__FILE__,
									__LINE__,	
									BUS_LONG_ACC,  
									BUS_DATA_WRITE ,
									(uint8 *)rmtbusVarStartPtr, 
									(uint8 *)tmpPtr
						 		);
#endif
			if(retVal)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, retVal);
		}
    }
	
	highBitFlag = FALSE;
	tmpPtr = fbInfoPtr + fbMemPtr->inputSize + fbMemPtr->intlVarSize;
    for(i = 0; i < fbMemPtr->outputSize; i++,outputStartPtr++,busOutputStartPtr++, rmtBusOutputStartPtr++,
    																		tmpPtr++) {
				
		dualCopyOutputTypePtr = outputTypePtr;

		
		if( (i!=0)&&(CHK_IS_ANY_TYPE(*outputTypePtr)) ){
			if( (GET_BASIC_DATA_TYPE(fbMemPtr->paraRunType)==UINT_TYPE) ||
				(GET_BASIC_DATA_TYPE(fbMemPtr->paraRunType)==INT_TYPE))
			{
				switch(GET_TYPE_DATA_SIZE(fbMemPtr->paraRunType))
				{
					case SIZE01_TYPE:
						*tmpPtr = *tmpPtr&0x1;
					break;
					case SIZE08_TYPE:
						*tmpPtr = *tmpPtr&0xff;
					break;
					case SIZE16_TYPE:
						*tmpPtr = *tmpPtr&0xffff;
					break;
					
					case SIZE32_TYPE:
					case SIZE64_TYPE:
						*tmpPtr = *tmpPtr&0xffffffff;
					break;
					default:
						status = FB_SPEC_TYPE_SIZE_ERR;
						setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
											"FB Code",fbMemPtr->fbId,
											"FB Addr",fbMemPtr->fbFuncAddr,
											"Output Index", i+1
										   );
						return(status);
					break;
				}	
			}
		}

		
		outputNegated = (fbMemPtr->outputAction[i/32]>>(i%32))&0x1;

		if( CHK_IS_BIT_TYPE(*outputTypePtr) && (outputNegated == 0x1 ) )
		{
			
			if(IS_ANY_TYPE_FB_CODE(fbMemPtr->fbId)) {
				switch(GET_TYPE_DATA_SIZE(fbMemPtr->paraRunType))
				{
					case SIZE01_TYPE:
						*tmpPtr = (!(*tmpPtr)) & 0x1;
					break;
					case SIZE08_TYPE:
						*tmpPtr = (~(*tmpPtr)) & 0xff;
					break;
					case SIZE16_TYPE:
						*tmpPtr = (~(*tmpPtr)) & 0xffff;
					break;
					case SIZE32_TYPE:
					case SIZE64_TYPE:
						*tmpPtr = (~(*tmpPtr)) & 0xffffffff;
					break;
					default:
						status = ANY_FUNC_OUT_SIZE_ERR;
						setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
											"FB Code",fbMemPtr->fbId,
											"FB Addr",fbMemPtr->fbFuncAddr,
											"Output Index", i+1
										   );
						return(status);
					break;
				}
			}
			
			else {
				
	
				if(GET_DATA_TYPE(*outputTypePtr) == BOOL_TYPE) {
					*tmpPtr = (!(*tmpPtr)) & 0x1;
				} 
				else {
					status = FB_OUTPUT_NEGATE_FLAG_ERR;
					setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
											"FB Code",fbMemPtr->fbId,
											"FB Addr",fbMemPtr->fbFuncAddr,
											"Output Index", i+1
										   );
					return(status);										   
				}
			}
		}

		if(GET_TYPE_SIZE(*outputTypePtr)==SIZE32_TYPE) { 
			outputTypePtr++;
		}
		else if( (GET_TYPE_SIZE(*outputTypePtr)==SIZE64_TYPE)&&(highBitFlag==FALSE) ) { 
			highBitFlag =TRUE;
		}
		else if( (GET_TYPE_SIZE(*outputTypePtr)==SIZE64_TYPE)&&(highBitFlag==TRUE) ) { 
			highBitFlag =FALSE;
			outputTypePtr++;
		}

		
		if( *outputStartPtr == *tmpPtr){
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
			
			if(	((moduleId == LOGIC_ID) && (gDualMpuDataCopyFlag == DATA_SYNC_ENABLE))
			){
				*outputStartPtr = *tmpPtr;  

				if (fbDualCopyFlag == DATA_SYNC_ENABLE)
					paraDualCopyFlag = GET_PARA_COPY_FLAG(*dualCopyOutputTypePtr);

				goto RMT_OUTPUT_COPY;
			}
			else
				continue;
#elif (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			continue;
#endif
		}

		 
		*outputStartPtr = *tmpPtr;  

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
		if (fbDualCopyFlag == DATA_SYNC_ENABLE)
			paraDualCopyFlag = GET_PARA_COPY_FLAG(*dualCopyOutputTypePtr) ;
#endif
		
		
		
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
		if(localBusCopyFlag == DATA_SYNC_ENABLE){ 
#elif (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
		if(localBusCopyFlag == NO_ERROR){
#endif
			retVal = myBusDataAcc(	__FILE__,
									__LINE__,	
									BUS_LONG_ACC,  
									BUS_DATA_WRITE ,
									(uint8 *)busOutputStartPtr, 
									(uint8 *)tmpPtr
						 		);
			if(retVal)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, retVal);
				
		}
		
RMT_OUTPUT_COPY:
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
		if(rmtBusCopyFlag == DATA_SYNC_ENABLE){
#elif (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
		if(rmtBusCopyFlag == NO_ERROR){
#endif
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			
			sAccumFBData.accumPtr[sAccumFBData.accDataNum] = rmtBusOutputStartPtr;
			sAccumFBData.accumData[sAccumFBData.accDataNum] = *tmpPtr;
			sAccumFBData.accDataNum++;
#else 
			retVal = myBusDataAcc(	__FILE__,
									__LINE__,	
									BUS_LONG_ACC, 
									BUS_DATA_WRITE ,
									(uint8 *)rmtBusOutputStartPtr, 
									(uint8 *)tmpPtr
						 		);
#endif 
			if(retVal)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, retVal);
		}
    }

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD) 
	if( (rmtBusCopyFlag == NO_ERROR)&&(sAccumFBData.accDataNum >0 ) ) {
		status = myRmtRunDataWrite((strAccumBusInfo*)&sAccumFBData);
		if(status)
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
	}
#endif
    return(status);
}

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)

uint32 myRmtRunDataWrite(strAccumBusInfo* rmtRumDataInfo) 
{
	uint32 status = NO_ERROR;
	uint32 sendPackLoop, rmtSendPackNum, namergeSendPackNum;
	uint32 sendDataIndex;

	
	if(rmtRumDataInfo->accDataNum >= MAX_DUAL_COPY_DATA_NUM)
	{
		rmtSendPackNum = (uint32)floor((double)(rmtRumDataInfo->accDataNum/MAX_DUAL_COPY_DATA_NUM));

		for(sendPackLoop=0;sendPackLoop < rmtSendPackNum ;sendPackLoop++) {
			sendDataIndex = sendPackLoop*MAX_DUAL_COPY_DATA_NUM;
			status = myRmtFourByteDataWrite( MAX_DUAL_COPY_DATA_NUM,
											 (uint8*)&rmtRumDataInfo->accumPtr[sendDataIndex], 
											 (uint8*)&rmtRumDataInfo->accumData[sendDataIndex] 
										   );
			if(status != NO_ERROR){
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
				return(status);
			}
		}
		
		namergeSendPackNum = rmtRumDataInfo->accDataNum % MAX_DUAL_COPY_DATA_NUM;
		if(namergeSendPackNum)
		{
			sendDataIndex = rmtSendPackNum * MAX_DUAL_COPY_DATA_NUM;

			status = myRmtFourByteDataWrite( namergeSendPackNum,
											 (uint8*)&rmtRumDataInfo->accumPtr[sendDataIndex], 
											 (uint8*)&rmtRumDataInfo->accumData[sendDataIndex] 
										   );
			if(status != NO_ERROR){
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
				return(status);
		}
	}

		rmtRumDataInfo->accDataNum = 0;

	}
	
	else if(rmtRumDataInfo->accDataNum !=0 ) {
		status = myRmtFourByteDataWrite( rmtRumDataInfo->accDataNum,
										 (uint8*)&rmtRumDataInfo->accumPtr[0],
										 (uint8*)&rmtRumDataInfo->accumData[0]
									   );
		if(status != NO_ERROR){
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}
		rmtRumDataInfo->accDataNum = 0;
	}

	return(status);
}
#endif 


uint32	fbCodeParaWriteWithVar
				( 
			uint32 	taskId,
			uint32	moduleId, 
			uint32	fbBlockAddr, 
			uint32 	*fbInfoPtr,
			uint8   *varFlag
				)
{
	strFbMemInfo	*fbMemPtr, *busFbMemPtr;
	uint32	status = NO_ERROR;
	uint32	retVal = NO_ERROR;
	uint32	*specTypePtr, *tmpPtr;
	uint32	*specStartPtr = NULL;
	uint32	*busSpecStartPtr = NULL;
	uint32	*rmtbusSpecStartPtr= NULL;
	uint32	i = 0;
	uint32	localBusCopyFlag = DO_COPY;
	uint32	rmtBusCopyFlag = DO_COPY;
	uint32	*specActionStartPtr = NULL; 
	uint32	*busSpecActionStartPtr = NULL;
	uint32	*rmtbusSpecActionStartPtr = NULL;
	strFbSpecActionInfo *specActionDataPtr = NULL;
	strFbSpecActionInfo *busSpecActionDataPtr = NULL;
	strFbSpecActionInfo *rmtbusSpecActionDataPtr = NULL;
	strFbCodeTypePointerInfo typePInfo;
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	uint32 retainInfo;
#endif

	
    if(moduleId == LOGIC_ID) { 
		
		fbMemPtr 		= gMpuFbMemInfoLocalPtr;
	
		specStartPtr 	= (uint32 *)gMpuSpecAndVarDataLocalPtr;
		specActionStartPtr = (uint32 *)gMpuSpecAndVarDataLocalPtr;

		
		busFbMemPtr 		= gMpuFbMemInfoBusPtr;
		
		busSpecStartPtr 	= (uint32 *)gMpuSpecAndVarDataBusPtr;
		busSpecActionStartPtr 	= (uint32 *)gMpuSpecAndVarDataBusPtr;
		
	} else {
		return (L_MODULE_ID_ERR);
	}

	
   	
   	fbMemPtr 		+= (fbBlockAddr-1);
	specStartPtr	+= fbMemPtr->specIndex;

	specActionStartPtr	+= fbMemPtr->specActionIndex;
	specActionDataPtr	= (strFbSpecActionInfo *)specActionStartPtr;
 
	
	busFbMemPtr 		+= (fbBlockAddr-1);
	busSpecStartPtr		+= fbMemPtr->specIndex;

	busSpecActionStartPtr	+= fbMemPtr->specActionIndex;
	busSpecActionDataPtr	= (strFbSpecActionInfo *)busSpecActionStartPtr;

	
	rmtbusSpecStartPtr		+= fbMemPtr->specIndex;
	rmtbusSpecActionStartPtr+= fbMemPtr->specActionIndex;
	rmtbusSpecActionDataPtr	= (strFbSpecActionInfo *)rmtbusSpecActionStartPtr;

	
	status = fbCodeParaTypePointerGet(
								fbMemPtr->fbId,
								(strFbCodeTypePointerInfo *)&typePInfo
								);
	if(status != NO_ERROR) {
		return(status);
	}

	specTypePtr = (uint32 *)typePInfo.specTypeStartAddr;

	
	if( (gSysRunningInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN) &&
		(gSysRunningInfoPtr->sysOperationMode == SYS_RAS_SYSTEM_AUTO_MODE) &&
		(gDiuRasInfoPtr->redundancyStatus == NO_ERROR)
	){
		
		localBusCopyFlag = DO_COPY;
	}
	else { 
		rmtBusCopyFlag = DONT_COPY;
	}

	
 	
	

	tmpPtr = fbInfoPtr;

    
	for(i=0; i < fbMemPtr->inputNo; i++,
						tmpPtr++, specStartPtr++,specActionDataPtr++,
						busSpecStartPtr++,busSpecActionDataPtr++)

	{	

		
		if ( (specActionDataPtr->refType == SPEC_TYPE_BYREF) &&
			 (specActionDataPtr->memoryType != F_MEMORY) &&
			 (varFlag[i] == TRUE) 
		   )
		{

			 
			retVal = refDataCopyToMem(	tmpPtr, 
										*specStartPtr, 
										*specActionDataPtr, 
										GET_TYPE_SIZE(*specTypePtr)
									);

			
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			retainInfo = NO_RETAIN; 
			if(retVal == NO_ERROR ) {
				retVal = varRetainInfoReadwithRef(	*busSpecStartPtr, 
													*busSpecActionDataPtr,
													(uint32*)&retainInfo
												);
			}
#endif			
			
			if( (localBusCopyFlag == DO_COPY) &&
				(retVal == NO_ERROR) 
#if 0 
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
				 &&(retainInfo == EN_RETAIN)
#endif
#endif
			){
				retVal = refDataCopyToBusMem(	tmpPtr, 
												*busSpecStartPtr, 
												*busSpecActionDataPtr,
												GET_TYPE_SIZE(*specTypePtr)
											);
				if(retVal) {
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, retVal);
				}
						
			}
			
			
			if( (rmtBusCopyFlag == DO_COPY) &&
				(retVal == NO_ERROR) 
			){
				retVal = refDataCopyToRmtMem(	tmpPtr, 
											*busSpecStartPtr, 
											*busSpecActionDataPtr,
											GET_TYPE_SIZE(*specTypePtr)
										);
				if(retVal) {
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, retVal);
				}
			}
		}

		if(GET_TYPE_SIZE(*specTypePtr) == SIZE64_TYPE) 
		{ 
			specStartPtr++;
			
			busSpecStartPtr++;
			tmpPtr++;
		}
	}

    status = writeRuntimeFbData(	taskId, 
								moduleId, 
								fbBlockAddr, 
								fbInfoPtr
							);

	return(status);
}


static uint32	fbParaCopyFromMem 
				(
			 uint32	taskId,
			 uint32	moduleId,
             uint32	fbBlockAddr,
			 uint32	*destPtr,
             strFbMemInfo	*fbMemPtr,
             uint32	flag
             	)
{
	uint32	status = NO_ERROR;
	uint32	*specTypePtr, *outputTypePtr;
	uint32	*specStartPtr, *varStartPtr, *outputStartPtr, *outputDataPtr;
	uint32	i = 0;
	uint32	*specActionStartPtr = NULL;
	strFbSpecActionInfo *specActionDataPtr= NULL;
	void	*srcPtr;
	strFbCodeTypePointerInfo typePInfo;

	
	if(moduleId == LOGIC_ID) 
	{
		specActionStartPtr 	= (uint32 *)gMpuSpecAndVarDataLocalPtr;
		specStartPtr 		= (uint32 *)gMpuSpecAndVarDataLocalPtr;
		varStartPtr 		= (uint32 *)gMpuSpecAndVarDataLocalPtr;
		outputStartPtr 		= (uint32 *)gMpuOutputDataLocalPtr;
	}  	
	else 
	{
		status = L_MODULE_ID_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		
		return(status);
	}

	
	if(moduleId == LOGIC_ID) 
	{
		specActionStartPtr	+= fbMemPtr->specActionIndex;
		specActionDataPtr	= (strFbSpecActionInfo *)specActionStartPtr;
	}
	
    specStartPtr		+= fbMemPtr->specIndex;
	varStartPtr			+= fbMemPtr->varIndex;
	outputDataPtr		= outputStartPtr + fbMemPtr->outputIndex;

	
	if(moduleId != LOGIC_ID) 
	{
	    fourBytesDataCopy (	(uint32 *)destPtr,
    						(uint32 *)specStartPtr,
                    		fbMemPtr->inputNo 
						);

		destPtr += fbMemPtr->inputNo;
		fourBytesDataCopy (	(uint32 *)destPtr,
    						(uint32 *)varStartPtr,
                    		fbMemPtr->intlVarNo
						);

		destPtr += fbMemPtr->intlVarNo;
		fourBytesDataCopy (	(uint32 *)destPtr,
    						(uint32 *)outputDataPtr,
							fbMemPtr->outputNo
						);
		return(status);
	}

	status = fbCodeParaTypePointerGet(
								fbMemPtr->fbId,
								(strFbCodeTypePointerInfo *)&typePInfo
								);
	if(status) 
	{
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	specTypePtr = (uint32 *)typePInfo.specTypeStartAddr;		
	outputTypePtr = (uint32 *)typePInfo.outputTypeStartAddr;	

		
	if( IS_ANY_TYPE_FB_CODE(fbMemPtr->fbId) &&
		CHK_IS_ANY_TYPE(*(outputTypePtr + 1))
	) {
		if(fbMemPtr->paraRunType == 0) 
		{
			status = NO_RUN_PARA_TYPE_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"FB Code",fbMemPtr->fbId,
								"FB Addr",fbMemPtr->fbFuncAddr,
								"Run Type",fbMemPtr->paraRunType);
			return(status);
		}
	}
   
	for(i=0; i < fbMemPtr->inputNo; i++,
				destPtr++, specStartPtr++, specTypePtr++, specActionDataPtr++)
	{	
		if(moduleId == LOGIC_ID)
		{
				
			if(specActionDataPtr->refType == SPEC_TYPE_BYREF) 
			{
				if(flag == WITH_VAL)
				{
					
					if(GET_BASIC_SIZE(specActionDataPtr->dataType) == SIZE64_TYPE)
						specStartPtr++;	
																
					if(CHK_IS_ANY_TYPE(*specTypePtr)) 
					{
						status = refDataCopyFromMem	(	destPtr,
														*specStartPtr,
														*specActionDataPtr,
														
														( ((fbMemPtr->paraRunType)&0xf00f) | ((*specTypePtr)&0xffff0ff0) ),
														TYPECASTING_ENABLE
													);
					} 
					else 
					{
						status = refDataCopyFromMem	(	destPtr,
														*specStartPtr,
														*specActionDataPtr,
														*specTypePtr,
														0
													);
					} 

					if(status) 
					{
						setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
											"FB CODE", fbMemPtr->fbId,
											"Fb Blk Addr", fbBlockAddr,
											"INPUT INDEX", i+1
											);
						return(status);
					}
				}

				else if(flag == WITH_ADDR) 
				{
					memoryCopy((uint8*)destPtr, (uint8*)specStartPtr,DOUBLE_SIZE) ;
				}		
			}

			else 
			{
				srcPtr = specStartPtr;
				
				if((*specTypePtr&0xf000) == SIZE32_TYPE) 
				{	
					memoryCopy ((uint8*)destPtr, (uint8*)srcPtr, 4);
				} 
				else if((*specTypePtr&0xf000) == SIZE64_TYPE) 
				{	
					memoryCopy ((uint8*)destPtr, (uint8*)srcPtr, 8);
				}
			}
		} 
		else 
		{
			srcPtr = specStartPtr;
			
			if((*specTypePtr&0xf000) == SIZE32_TYPE) 
			{	
				memoryCopy ((uint8*)destPtr, (uint8*)srcPtr, 4);
			} 
			else if((*specTypePtr&0xf000) == SIZE64_TYPE) 
			{	
				memoryCopy ((uint8*)destPtr, (uint8*)srcPtr, 8);
			}
		}
	
		
		if( (moduleId==LOGIC_ID)&& CHK_IS_BIT_TYPE(*specTypePtr) && (specActionDataPtr->negated == 0x1) ) 
		{
			status= specDataNegated(specActionDataPtr, destPtr);
			if(status) 
			{ 
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
				return(status);
			}
		}

		if(GET_TYPE_SIZE(*specTypePtr) == SIZE64_TYPE) 
		{
			specStartPtr++;
			destPtr++;	
			
			if((moduleId == LOGIC_ID)&& CHK_IS_BIT_TYPE(*specTypePtr) &&(specActionDataPtr->negated == 0x1)) 
			{
				status = specDataNegated(specActionDataPtr, destPtr);
				if(status) 
				{
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
					return(status);
				}
			}
		}
	}
   
    fourBytesDataCopy (	(uint32 *)destPtr,
    					(uint32 *)varStartPtr,
                    	fbMemPtr->intlVarSize 
					);

    
    destPtr += fbMemPtr->intlVarSize;
    fourBytesDataCopy (	(uint32 *)destPtr,
    					(uint32 *)outputDataPtr,
						fbMemPtr->outputSize
					);

    return(status);
}


uint32	fbMemInfoValidCheck
					(
				uint32 moduleId,
				strFbMemInfo *fbMemPtr
                	)
{
	uint32	status = NO_ERROR;
	strFbExecDbInfo	fbId;

    status = fbDbTableSortingInfoRead(fbMemPtr->fbId, &fbId);

    if(status == NO_ERROR) {
	    
        if( fbMemPtr->inputNo != fbId.inputNo){
        	status = FC_SPEC_NUM_ERR;
		}
		else if (fbMemPtr->intlVarNo != fbId.intlVarNo)
        	status = FC_VAR_NUM_ERR;
		else if(fbMemPtr->outputNo != fbId.outputNo)
        	status = FC_OUTPUT_NUM_ERR;
    }
    return(status);
}


uint32 fbMemInfoRead
				(
			uint32	moduleId,
            uint32	fbBlockAddr,
            strFbMemInfo	*fbMemPtr
            	)
{
	uint32	status = NO_ERROR;
	strMemPointerInfo	memPointer;
	strFbMemInfo	*dataPtr;
	strModuleInfo		moduleInfo;
	
	status = fbMemPointerGet(moduleId, &memPointer);

	status = moduleInfoRead(moduleId, &moduleInfo);
	if( (fbBlockAddr < 1) || (fbBlockAddr> moduleInfo.maxFbBlockAddr) ) {
		status = FB_BLOCK_ADDR_RANGE_ERR;
		return status;
	}

    if(status == NO_ERROR) {
		dataPtr = (strFbMemInfo *) memPointer.fbMemPtr;

    	
		dataPtr += (fbBlockAddr-1);
	  	memoryCopy ((uint8 *) fbMemPtr,	(uint8 *) dataPtr, sizeof(strFbMemInfo)	);
	}
    return(status);
}


uint32 busFbMemInfoRead
				(
			uint32	moduleId,
            uint32	fbBlockAddr,
            strFbMemInfo	*fbMemPtr
            	)
{
	uint32	status = NO_ERROR;
	strMemPointerInfo	memPointer;
	strFbMemInfo	*dataPtr;

	status = busFbMemPointerGet(moduleId, &memPointer);

    if(status == NO_ERROR) {
		dataPtr = (strFbMemInfo *) memPointer.fbMemPtr;

    	
    	dataPtr += (fbBlockAddr-1);
 
        
        
        

        
#if NOT_USED
		if(dataPtr->fbCodeIndex > 0x7FFFF){
			status = MEMORY_CRASH_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Module ID", moduleId,
								"FB Block Addr",fbBlockAddr,
								"FB Code", dataPtr->fbId
								 );
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Fb Index", dataPtr->fbCodeIndex,
								"VME Start Addr", memPointer.fbMemPtr,
								"NOT USING", 0
								 );
								  
			return(MEMORY_CRASH_ERR); 
		}	        
		dataPtr = dataPtr - dataPtr->fbCodeIndex;
#endif
    	memoryCopy ((uint8 *) fbMemPtr,
        			(uint8 *) dataPtr,
        			sizeof(strFbMemInfo)
        			);
	}
    return(status);
}


uint32 fbCodeMemInfoRead
				(
			uint32	moduleId,
            uint32	fbBlockAddr,
            strFbMemInfo	*fbMemPtr
            	)
{
	uint32	status = NO_ERROR;
	strFbMemInfo	*dataPtr;
	uint32	i, *srcPtr, *destPtr, numOfData;
	
	if(fbBlockAddr < 1) return(L_FB_ADDR_ERR);

    if(moduleId == LOGIC_ID) 
    	dataPtr = gMpuFbMemInfoLocalPtr; 
	else 
		return(L_MODULE_ID_ERR);
 
   	
   	dataPtr += (fbBlockAddr-1);
 	
    srcPtr 		= (uint32 *)dataPtr;
    destPtr 	= (uint32 *)fbMemPtr;
	numOfData	= sizeof(strFbMemInfo)/4;

    for(i = 0; i < numOfData; i++, srcPtr++, destPtr++) 
		*destPtr = *srcPtr;

    return(status);
}


uint32 busFbCodeMemInfoRead
				(
			uint32	moduleId,
            uint32	fbBlockAddr,
            strFbMemInfo	*fbMemPtr
            	)
{
	uint32	status = NO_ERROR;
	strFbMemInfo	*dataPtr;
	uint32	i, *srcPtr, *destPtr, numOfData;
	
	if(fbBlockAddr < 1) 
		return(L_FB_ADDR_ERR);

    if(moduleId == LOGIC_ID) 
    	dataPtr = gMpuFbMemInfoBusPtr;
	else 
		return(L_MODULE_ID_ERR);
 
   	
   	dataPtr += (fbBlockAddr-1);
 	
    srcPtr 		= (uint32 *)dataPtr;
    destPtr 	= (uint32 *)fbMemPtr;
	numOfData	= sizeof(strFbMemInfo)/4;

    for(i = 0; i < numOfData; i++, srcPtr++, destPtr++) 
		*destPtr = *srcPtr;

    return(status);
}


uint32	fbMemPointerGet
					(
				uint32	moduleId,
                strMemPointerInfo	*memInfoPtr
                	)
{
	uint32	status = NO_ERROR;
	strSysConfigInfo *sysCnfgInfoPtr = &gSysCnfgInfo;

    switch(moduleId) {
  		case LOGIC_ID:
			memInfoPtr->fbMemPtr 		= (uint32)gMpuFbMemInfoLocalPtr;
			memInfoPtr->paraMemPtr 		= (uint32)gMpuSpecAndVarDataLocalPtr;
			memInfoPtr->outputMemPtr 	= (uint32)gMpuOutputDataLocalPtr;
			memInfoPtr->segTblMemPtr 	= (uint32)gMpuFbSegTblInfoLocalPtr;
			
			memInfoPtr->stSharedMemPtr = (uint32)gSTSharedMemDataLocalPtr;

			memInfoPtr->retainMemPtr 	= (uint32)gRetainVarInfoLocalPtr;
			memInfoPtr->autoMemPtr 		= (uint32)gAutoVariableLocalPtr;
			
			memInfoPtr->flagMemPtr 		= (uint32)gMmemVariableLocalPtr;

        break;

        default:
        	status = L_MODULE_ID_ERR;
        break;
	}

    return(status);
}


uint32	busFbMemPointerGet
					(
				uint32	moduleId,
                strMemPointerInfo	*memInfoPtr
                	)
{
	uint32	status = NO_ERROR;

    switch(moduleId) {
  		case LOGIC_ID:
			memInfoPtr->fbMemPtr = (uint32)gMpuFbMemInfoBusPtr;
			memInfoPtr->paraMemPtr = (uint32)gMpuSpecAndVarDataBusPtr;
			memInfoPtr->outputMemPtr = (uint32)gMpuOutputDataBusPtr;
			memInfoPtr->segTblMemPtr = (uint32)gMpuFbSegTblInfoBusPtr;
			memInfoPtr->stSharedMemPtr = (uint32)gSTSharedMemDataBusPtr;
        	
			memInfoPtr->retainMemPtr = (uint32)gRetainVarInfoBusPtr;
			memInfoPtr->autoMemPtr = (uint32)gAutoVariableBusPtr;
			memInfoPtr->flagMemPtr = (uint32)gMmemVariableBusPtr;
        	
        break;

        default:
        	status = L_MODULE_ID_ERR;
        break;
	}

    return(status);
}


uint32	rmtbusFbMemPointerGet
					(
				uint32	moduleId,
                strMemPointerInfo	*memInfoPtr
                	)
{
	uint32	status = NO_ERROR;
	uint32	offSet, bitMask;
	strSysConfigInfo	sysCnfgInfo;

    
    systemCnfgInfoRead(&sysCnfgInfo);

    if(sysCnfgInfo.shelfType == MASTER_SHELF)
       	offSet = MEM_BOARD_BUS_ADDR_ON_SLAVE;
    else if(sysCnfgInfo.shelfType == SLAVE_SHELF) 
       	offSet = MEM_BOARD_BUS_ADDR_ON_MASTER;
    else {
		status = STATION_SHELF_CONFIG_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
        return(status);
    }

    bitMask = 0x00ffffff;

#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
    switch(moduleId) {
  		case LOGIC_ID:
			memInfoPtr->fbMemPtr = (uint32)gMpuFbMemInfoBusPtr;
			memInfoPtr->fbMemPtr &= bitMask;
			memInfoPtr->fbMemPtr += offSet;

			memInfoPtr->paraMemPtr = (uint32)gMpuSpecAndVarDataBusPtr;
			memInfoPtr->paraMemPtr &= bitMask;
			memInfoPtr->paraMemPtr += offSet;

			memInfoPtr->outputMemPtr = (uint32)gMpuOutputDataBusPtr;
			memInfoPtr->outputMemPtr &= bitMask;
			memInfoPtr->outputMemPtr += offSet;

			memInfoPtr->segTblMemPtr = (uint32)gMpuFbSegTblInfoBusPtr;
			memInfoPtr->segTblMemPtr &= bitMask;
			memInfoPtr->segTblMemPtr += offSet;

			memInfoPtr->stSharedMemPtr = (uint32)gSTSharedMemDataBusPtr;
			memInfoPtr->stSharedMemPtr &= bitMask;
			memInfoPtr->stSharedMemPtr += offSet;
        	
        break;

        default:
        	status = L_MODULE_ID_ERR;
        break;
	}
#elif (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
    switch(moduleId) {
  		case LOGIC_ID:
			memInfoPtr->fbMemPtr = ((uint32)gMpuFbMemInfoBusPtr- (uint32)gBusMpuFbStartAddr);
			memInfoPtr->fbMemPtr &= bitMask;
			memInfoPtr->fbMemPtr += offSet;

			memInfoPtr->paraMemPtr = ((uint32)gMpuSpecAndVarDataBusPtr- (uint32)gBusMpuFbStartAddr);
			memInfoPtr->paraMemPtr &= bitMask;
			memInfoPtr->paraMemPtr += offSet;

			memInfoPtr->outputMemPtr = ((uint32)gMpuOutputDataBusPtr- (uint32)gBusMpuFbStartAddr);
			memInfoPtr->outputMemPtr &= bitMask;
			memInfoPtr->outputMemPtr += offSet;

			memInfoPtr->segTblMemPtr = ((uint32)gMpuFbSegTblInfoBusPtr- (uint32)gBusMpuFbStartAddr);
			memInfoPtr->segTblMemPtr &= bitMask;
			memInfoPtr->segTblMemPtr += offSet;

			memInfoPtr->stSharedMemPtr = ((uint32)gSTSharedMemDataBusPtr- (uint32)gBusMpuFbStartAddr);
			memInfoPtr->stSharedMemPtr &= bitMask;
			memInfoPtr->stSharedMemPtr += offSet;
			
			
			memInfoPtr->flagMemPtr = ((uint32)gMmemVariableBusPtr - (uint32)gBusMpuFbStartAddr);
			memInfoPtr->flagMemPtr &= bitMask;
			memInfoPtr->flagMemPtr += offSet;

			memInfoPtr->autoMemPtr = ((uint32)gAutoVariableBusPtr - (uint32)gBusMpuFbStartAddr);
			memInfoPtr->autoMemPtr &= bitMask;
			memInfoPtr->autoMemPtr += offSet;

			memInfoPtr->retainMemPtr = ((uint32)gRetainVarInfoBusPtr - (uint32)gBusMpuFbStartAddr);
			memInfoPtr->retainMemPtr &= bitMask;
			memInfoPtr->retainMemPtr += offSet;
        break;

        default:
        	status = L_MODULE_ID_ERR;
        break;
	}
#endif

    return(status);
}


uint32 fbMemoryClear
				(
			uint32	moduleId,
            uint32	taskId,
            strFbTaskTblMemInfo	*segInfoPtr,
            uint32	dwlKind	
            	)
{
	uint32 status = NO_ERROR;

	status = localFbMemoryClear(moduleId, taskId, segInfoPtr, dwlKind);
    if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
    	return(status);
	}

	status = busFbMemoryClear(moduleId, taskId, segInfoPtr, dwlKind);
    if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
	}
    return(status);
}


uint32 localFbCodeMemClear(uint32 moduleId, uint32 blockAddr, uint32 taskId)
{
	uint32 status = NO_ERROR;
	strFbMemInfo fbMemInfo;
	uint32	*clearPtr;
	strMemPointerInfo memInfo;

	memoryClear((uint8 *)&fbMemInfo, sizeof(strFbMemInfo));
    status = fbMemInfoRead(moduleId, blockAddr, &fbMemInfo);
	if(status != NO_ERROR) {
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	if( (fbMemInfo.fbId == 0) || (fbMemInfo.fbTaskId != taskId) )
		return(status);

	status = fbMemPointerGet(moduleId, &memInfo);
	if(status) {
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);

		return(status);
	}

	
	clearPtr 	= (uint32 *)memInfo.fbMemPtr + (blockAddr-1) * (sizeof(strFbMemInfo) / 4);
	memoryClear	( 	(uint8 *) clearPtr,
       			sizeof(strFbMemInfo)
			);	
	
	clearPtr 	= (uint32 *)memInfo.paraMemPtr + fbMemInfo.specActionIndex;
	memoryClear	( 	(uint8 *) clearPtr,
       			4 * (fbMemInfo.inputNo + fbMemInfo.inputSize + fbMemInfo.intlVarSize)
			);	

	
	clearPtr 	= (uint32 *)memInfo.outputMemPtr + fbMemInfo.outputIndex;
	memoryClear	( 	(uint8 *) clearPtr,
       			4 * (fbMemInfo.outputSize)
	  		);	
	
	if(moduleId == LOGIC_ID)
		gFBExeSequenceQueuePtr[blockAddr-1] =  NULL;

	return(status);
}


uint32 busFbCodeMemClear(uint32 moduleId, uint32 blockAddr, uint32 taskId)
{
	uint32 status = NO_ERROR;
	strFbMemInfo fbMemInfo;
	uint32	*clearPtr;
	strMemPointerInfo memInfo;

	memoryClear((uint8 *)&fbMemInfo, sizeof(strFbMemInfo));
    status = busFbMemInfoRead(moduleId, blockAddr, &fbMemInfo);
	if(status != NO_ERROR) {
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	if( (fbMemInfo.fbId == 0) || (fbMemInfo.fbTaskId != taskId) )
		return(status);

	status = busFbMemPointerGet(moduleId, &memInfo);
	if(status) {
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);

		return(status);
	}

	
	clearPtr 	= (uint32 *)memInfo.fbMemPtr + (blockAddr-1) * (sizeof(strFbMemInfo) / 4);
	memoryClear	( 	(uint8 *) clearPtr,
       			sizeof(strFbMemInfo)
			);	
	
	clearPtr 	= (uint32 *)memInfo.paraMemPtr + fbMemInfo.specActionIndex;
	memoryClear	( 	(uint8 *) clearPtr,
       			4 * (fbMemInfo.inputNo + fbMemInfo.inputSize + fbMemInfo.intlVarSize)
			);	

	
	clearPtr 	= (uint32 *)memInfo.outputMemPtr + fbMemInfo.outputIndex;
	memoryClear	( 	(uint8 *) clearPtr,
       			4 * (fbMemInfo.outputSize)
	  		);	
	return(status);
}


static uint32 localFbMemoryClear
				(
			uint32	moduleId,
            uint32	taskId,
            strFbTaskTblMemInfo	*segInfoPtr,
            uint32	dwlKind
            	)
{
	uint32	status = NO_ERROR;
	strMemPointerInfo	memInfo;
	uint32	paraSize, outputSize, segTblSize, fbSize;
	strFbTaskTblMemInfo	*segStartPtr;
	strFbMemInfo	fbMemInfo;
	uint32	blockAddr, startParaIndex, startOutputIndex;
	uint32	*clearPtr;

	strFbExecutionInfo	*fbExecIdInfoPtr;
	strModuleExecInfo  sysExeInfo;

	strTrsLinkInfo *trsLinkInfoPtr;
	strActionLinkInfo *actLinkInfoPtr;
    strSysExecInfo *sysBusExeInfoPtr;

    paraSize 	= 0;
    outputSize 	= 0;
    fbSize 		= 0;
    segTblSize 	= 0;
	startParaIndex 		= 0;
	startOutputIndex 	= 0;

	if(moduleId == LOGIC_ID) {
		
		if( (segInfoPtr->taskId == 0) || (segInfoPtr->taskId > MAX_LOGIC_TASK_NUM) )
			fbExecIdInfoPtr = NULL;
		else fbExecIdInfoPtr = fbExecIdStartMemInfoGet(segInfoPtr->taskId);

		status = fbMemPointerGet(moduleId, &memInfo);
		if(status) {
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);

			return(status);
		}
		 
		if( (fbExecIdInfoPtr == NULL) && (segInfoPtr->fbUsedNum ==0) )	return(status);
		
		else if((fbExecIdInfoPtr == NULL) && (segInfoPtr->fbUsedNum !=0) )
		{
			if(segInfoPtr->logicType == SFC_LOGIC_TYPE)
			{
				trsLinkInfoPtr = trsLinkStartMemInfoGet(taskId);
				while(trsLinkInfoPtr!=NULL)
				{
					if(trsLinkInfoPtr->taskId == taskId)
					{
						fbExecIdInfoPtr = trsExecIdStartMemInfoGet(trsLinkInfoPtr->transitionId);
						while(fbExecIdInfoPtr !=NULL)
						{
							blockAddr = fbExecIdInfoPtr->fbBlockAddr;
							status = localFbCodeMemClear(moduleId,blockAddr,taskId);
							if(status != NO_ERROR)
								setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
							
							fbExecIdInfoPtr = trsExecIdNextMemInfoGet(trsLinkInfoPtr->transitionId, fbExecIdInfoPtr);
						} 
					}
					trsLinkInfoPtr = trsLinkNextMemInfoGet(taskId, trsLinkInfoPtr);
				}
				
				actLinkInfoPtr = actLinkStartMemInfoGet(taskId);
				while(actLinkInfoPtr!=NULL)
				{
					if(actLinkInfoPtr->taskId == taskId)
					{
						fbExecIdInfoPtr = actExecIdStartMemInfoGet(actLinkInfoPtr->actionId);
						while(fbExecIdInfoPtr !=NULL)
						{
							blockAddr = fbExecIdInfoPtr->fbBlockAddr;
							status = localFbCodeMemClear(moduleId,blockAddr,taskId);
							if(status != NO_ERROR)
								setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
							
							fbExecIdInfoPtr = actExecIdNextMemInfoGet(actLinkInfoPtr->actionId, fbExecIdInfoPtr);
						} 
					}
					actLinkInfoPtr = actLinkNextMemInfoGet(taskId, actLinkInfoPtr);
				}	
				
				
				return(status);
			} 

			else{
				memoryClear( (uint8 *)&sysExeInfo, sizeof(strModuleExecInfo));
				status = fbSysExeModuleInfoRead( moduleId, &sysExeInfo );
				if(status != NO_ERROR){
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
					return(status);
				}

				if( (taskId <= sysExeInfo.curMaxSegmentId) &&	
					(sysExeInfo.curfbMaxBlockAddr != 0) &&
					(sysExeInfo.curfbMaxBlockAddr <= MAX_MPU_BLOCK_ADDR) )
				{
					for(blockAddr=1;blockAddr<=sysExeInfo.curfbMaxBlockAddr;blockAddr++)
					{
						status = localFbCodeMemClear(moduleId,blockAddr,taskId);
						if(status != NO_ERROR)
							setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
					}
				}
			}
		}

		while(fbExecIdInfoPtr != NULL)
		{ 
			blockAddr = fbExecIdInfoPtr->fbBlockAddr;
			status = localFbCodeMemClear(moduleId,blockAddr,taskId);
			if(status != NO_ERROR)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);

			fbExecIdInfoPtr = fbExecIdNextMemInfoGet(taskId, fbExecIdInfoPtr);
		}

	} 
	else {
		
		
		if( ((segInfoPtr->u1.fbStartBlockAddr == 0) && (segInfoPtr->u2.fbEndBlockAddr == 0))
											||
			(segInfoPtr->u1.fbStartBlockAddr > segInfoPtr->u2.fbEndBlockAddr))
			return(status);

		for	( blockAddr = segInfoPtr->u1.fbStartBlockAddr; 
			 blockAddr <= segInfoPtr->u2.fbEndBlockAddr; blockAddr++) 
		{

			status = fbMemInfoRead(moduleId, blockAddr, &fbMemInfo);
			if(status != NO_ERROR) {
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
				continue;
			}
			
			
			if((fbMemInfo.fbId == 0) || (fbMemInfo.fbId > MAX_FB_CODE_ID)) continue;

			if(	blockAddr == segInfoPtr->u2.fbEndBlockAddr) {
				
				paraSize = fbMemInfo.specIndex - startParaIndex;
	            paraSize += fbMemInfo.inputNo + fbMemInfo.intlVarNo;
				paraSize *= 4; 
    
				
				outputSize = fbMemInfo.outputIndex - startOutputIndex;
				outputSize += fbMemInfo.outputNo;
				outputSize *= 4;
			}	
		}

		status = fbMemPointerGet(moduleId, &memInfo);
		if(status) {
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}

   		fbSize		= sizeof(strFbMemInfo) * (segInfoPtr->u2.fbEndBlockAddr - segInfoPtr->u1.fbStartBlockAddr + 1);
 		segTblSize	= sizeof(strFbTaskTblMemInfo);

		
		clearPtr 	= (uint32 *)memInfo.fbMemPtr;
		clearPtr 	+= (segInfoPtr->u1.fbStartBlockAddr - 1) *(sizeof(strFbMemInfo) / 4);
		memoryClear	( 	(uint8 *) clearPtr, fbSize );	

		
		clearPtr 	= (uint32 *)memInfo.paraMemPtr + startParaIndex;
		memoryClear	( (uint8 *) clearPtr,	paraSize );	

		
		clearPtr 	= (uint32 *)memInfo.outputMemPtr + startOutputIndex;
		memoryClear	( (uint8 *) clearPtr,	outputSize );	
	}

	
    
  	
   	
    segTblSize	= sizeof(strFbTaskTblMemInfo);
    segStartPtr = (strFbTaskTblMemInfo *)memInfo.segTblMemPtr;

	segStartPtr += taskId - 1;

	 
    if(dwlKind == SEG_DWL) {
    	segStartPtr->memInputSize 			= 0;
    	segStartPtr->memOutputSize 			= 0;
		segStartPtr->fbUsedNum 				= 0;
		segStartPtr->taskElpTime 	= 0;
		segStartPtr->logicCheckSum 			= 0;

		
		status =  fbSysExeInfoPointerGet(&sysBusExeInfoPtr, LOCAL_MEM_ACC);
		if(status != NO_ERROR){
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}

		memoryClear ( (uint8 *) gSTSharedMemDataLocalPtr, MAX_SHARED_MEM_SIZE);
		memoryClear ( (uint8 *) gRetainVarInfoLocalPtr,  MAX_MPU_RETAIN_FLAG_MEM_SIZE);
		memoryClear ( (uint8 *) gAutoVariableLocalPtr, sysBusExeInfoPtr->autovarSysExeInfo.curMaxVarAddr*4);
		memoryClear ( (uint8 *) gMmemVariableLocalPtr, sysBusExeInfoPtr->flagvarSysExeInfo.curMaxVarAddr*4);
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
		memoryClear ( (uint8 *) gAutoVarInitLocalPtr, sysBusExeInfoPtr->autovarSysExeInfo.curMaxVarAddr*4);
		memoryClear ( (uint8 *) gMmemVarInitLocalPtr, sysBusExeInfoPtr->flagvarSysExeInfo.curMaxVarAddr*4);
#endif
	}
    else {
		memoryClear ( (uint8 *) segStartPtr, segTblSize);
    }

    return(status);
}


static uint32 busFbMemoryClear
				(
			uint32	moduleId,
            uint32	taskId,
            strFbTaskTblMemInfo	*segInfoPtr,
            uint32	dwlKind
            	)
{
	uint32	status = NO_ERROR;
	strMemPointerInfo	memInfo;
	uint32	paraSize, outputSize, segTblSize, fbSize;
	strFbTaskTblMemInfo	*segStartPtr;
	strFbMemInfo	fbMemInfo;
	uint32	blockAddr, startParaIndex, startOutputIndex;
	uint32	*clearPtr;

	strFbExecutionInfo	*fbExecIdInfoPtr;
	strModuleExecInfo	sysExeInfo;

	strTrsLinkInfo *trsLinkInfoPtr;
	strActionLinkInfo *actLinkInfoPtr;
    strSysExecInfo *sysBusExeInfoPtr;

    paraSize 	= 0;
    outputSize 	= 0;
    fbSize 		= 0;
    segTblSize 	= 0;
	startParaIndex 		= 0;
	startOutputIndex 	= 0;

	if(moduleId == LOGIC_ID) {
		
		if( (segInfoPtr->taskId == 0) || (segInfoPtr->taskId > MAX_LOGIC_TASK_NUM) )
			fbExecIdInfoPtr = NULL;
		else fbExecIdInfoPtr = fbExecIdStartMemInfoGet(segInfoPtr->taskId);

		status = busFbMemPointerGet(moduleId, &memInfo);
		if(status) {
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);

			return(status);
		}
		 
		if( (fbExecIdInfoPtr == NULL) && (segInfoPtr->fbUsedNum ==0) )	return(status);
		else if( (fbExecIdInfoPtr == NULL) && (segInfoPtr->fbUsedNum !=0) ) 
		{
			if(segInfoPtr->logicType == SFC_LOGIC_TYPE)
			{
				trsLinkInfoPtr = trsLinkStartMemInfoGet(taskId);
				while(trsLinkInfoPtr!=NULL)
				{
					if(trsLinkInfoPtr->taskId == taskId)
					{
						fbExecIdInfoPtr = trsExecIdStartMemInfoGet(trsLinkInfoPtr->transitionId);
						while(fbExecIdInfoPtr !=NULL)
						{
							blockAddr = fbExecIdInfoPtr->fbBlockAddr;
							status = busFbCodeMemClear(moduleId,blockAddr,taskId);
							if(status != NO_ERROR)
								setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
							
							fbExecIdInfoPtr = trsExecIdNextMemInfoGet(trsLinkInfoPtr->transitionId, fbExecIdInfoPtr);
						} 
					}
					trsLinkInfoPtr = trsLinkNextMemInfoGet(taskId, trsLinkInfoPtr);
				}
				
				actLinkInfoPtr = actLinkStartMemInfoGet(taskId);
				while(actLinkInfoPtr!=NULL)
				{
					if(actLinkInfoPtr->taskId == taskId)
					{
						fbExecIdInfoPtr = actExecIdStartMemInfoGet(actLinkInfoPtr->actionId);
						while(fbExecIdInfoPtr !=NULL)
						{
							blockAddr = fbExecIdInfoPtr->fbBlockAddr;
							status = busFbCodeMemClear(moduleId,blockAddr,taskId);
							if(status != NO_ERROR)
								setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
							
							fbExecIdInfoPtr = actExecIdNextMemInfoGet(actLinkInfoPtr->actionId, fbExecIdInfoPtr);
						} 
					}
					actLinkInfoPtr = actLinkNextMemInfoGet(taskId, actLinkInfoPtr);
				}	

				
				return(status);
			} 
					
			else{
				memoryClear( (uint8 *)&sysExeInfo, sizeof(strModuleExecInfo));
				status = fbSysExeModuleInfoRead( moduleId, &sysExeInfo );
				if(status != NO_ERROR){
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
					return(status);
				}

				if( (taskId <= sysExeInfo.curMaxSegmentId) &&	
					(sysExeInfo.curfbMaxBlockAddr != 0) &&
					(sysExeInfo.curfbMaxBlockAddr <= MAX_MPU_BLOCK_ADDR) )
				{
					for(blockAddr=1;blockAddr<=sysExeInfo.curfbMaxBlockAddr;blockAddr++)
					{
						memoryClear((uint8 *)&fbMemInfo, sizeof(strFbMemInfo));
									status = busFbCodeMemClear(moduleId,blockAddr,taskId);
						if(status != NO_ERROR)
							setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
					}
				}
			}
		}

		while(fbExecIdInfoPtr != NULL)
		{ 
			blockAddr = fbExecIdInfoPtr->fbBlockAddr;
			status = busFbCodeMemClear(moduleId,blockAddr,taskId);
			if(status != NO_ERROR)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);

			fbExecIdInfoPtr = fbExecIdNextMemInfoGet(taskId, fbExecIdInfoPtr);
		}
	}
	else {
		
		
		
		if( ((segInfoPtr->u1.fbStartBlockAddr == 0) && (segInfoPtr->u2.fbEndBlockAddr == 0))
											||
			(segInfoPtr->u1.fbStartBlockAddr > segInfoPtr->u2.fbEndBlockAddr))
			return(status);

		for	(	blockAddr = segInfoPtr->u1.fbStartBlockAddr; 
				blockAddr <= segInfoPtr->u2.fbEndBlockAddr;blockAddr++) 
		{
			status = busFbMemInfoRead(moduleId, blockAddr, &fbMemInfo);
			if(status != NO_ERROR) {
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
				continue;
			}
			
			
			if((fbMemInfo.fbId == 0) || (fbMemInfo.fbId > MAX_FB_CODE_ID)) continue;

			if(	blockAddr == segInfoPtr->u2.fbEndBlockAddr) {
				
				paraSize = fbMemInfo.specIndex - startParaIndex;
	            paraSize += fbMemInfo.inputNo + fbMemInfo.intlVarNo;
				paraSize *= 4; 
    
				
				outputSize = fbMemInfo.outputIndex - startOutputIndex;
				outputSize += fbMemInfo.outputNo;
				outputSize *= 4;
			}	
		}

		status = busFbMemPointerGet(moduleId, &memInfo);
		if(status) {
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}

   		fbSize		= sizeof(strFbMemInfo) * (segInfoPtr->u2.fbEndBlockAddr - segInfoPtr->u1.fbStartBlockAddr + 1);
		segTblSize	= sizeof(strFbTaskTblMemInfo);

		
		clearPtr 	= (uint32 *)memInfo.fbMemPtr;
		clearPtr 	+= (segInfoPtr->u1.fbStartBlockAddr - 1) *(sizeof(strFbMemInfo) / 4);
		memoryClear	( 	(uint8 *) clearPtr,fbSize);	

		
		clearPtr 	= (uint32 *)memInfo.paraMemPtr + startParaIndex;
		memoryClear	( 	(uint8 *) clearPtr,paraSize);	

		
		clearPtr 	= (uint32 *)memInfo.outputMemPtr + startOutputIndex;
		memoryClear	( 	(uint8 *) clearPtr,	outputSize);	
	}
   	
    segTblSize	= sizeof(strFbTaskTblMemInfo);
    segStartPtr = (strFbTaskTblMemInfo *)memInfo.segTblMemPtr;

	segStartPtr += taskId - 1;

	 
    if(dwlKind == SEG_DWL) {
    	segStartPtr->memInputSize = 0;
    	segStartPtr->memOutputSize = 0;
		segStartPtr->fbUsedNum = 0;
		segStartPtr->taskElpTime = 0;
		segStartPtr->logicCheckSum = 0;

		
		status =  fbSysExeInfoPointerGet(&sysBusExeInfoPtr, BUS_MEM_ACC);
		if(status != NO_ERROR){
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}

		memoryClear ( (uint8 *) gSTSharedMemDataBusPtr, MAX_SHARED_MEM_SIZE);
		memoryClear ( (uint8 *) gRetainVarInfoBusPtr,  MAX_MPU_RETAIN_FLAG_MEM_SIZE);
		memoryClear ( (uint8 *) gAutoVariableBusPtr, sysBusExeInfoPtr->autovarSysExeInfo.curMaxVarAddr*4);
		memoryClear ( (uint8 *) gMmemVariableBusPtr, sysBusExeInfoPtr->flagvarSysExeInfo.curMaxVarAddr*4);
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
		memoryClear ( (uint8 *) gAutoVarInitBusPtr, sysBusExeInfoPtr->autovarSysExeInfo.curMaxVarAddr*4);
		memoryClear ( (uint8 *) gMmemVarInitBusPtr, sysBusExeInfoPtr->flagvarSysExeInfo.curMaxVarAddr*4);
#endif
	}
    else {
		memoryClear ( (uint8 *) segStartPtr, segTblSize);
    }
	
    return(status);
}


uint32	fbCodeParaTypePointerGet
				(
			uint32	fbId,
			strFbCodeTypePointerInfo	*fbTypePtr 
				)
{
	uint32	status = NO_ERROR;
	strFbTypeIndexInfo	*indexPtr;

	indexPtr = (strFbTypeIndexInfo *)gFbCodeSortingTableInfo[fbId].fbCodeTypeIndexPtr;

    
	if(indexPtr == NULL) { 
		status = MEM_ADDR_NULL_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Req FB Code ID",fbId,
							"Not Using", 0,
							"Not Using", 0
							);

		return(status);
	}

	if(indexPtr->fbInfo.fbId != fbId) {
		status = FB_TYPE_TABLE_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Read FB Code ID",indexPtr->fbInfo.fbId,
							"Req FB Code ID",fbId,
							"Not Using", 0
							);
		return(status);
	}

	
    fbTypePtr->specTypeStartAddr	= (uint32)(gSpecTypePtr + indexPtr->specTypeIndex);
   	fbTypePtr->varTypeStartAddr		= (uint32)(gVarTypePtr + indexPtr->varTypeIndex);
   	fbTypePtr->outputTypeStartAddr	= (uint32)(gOutputTypePtr + indexPtr->outputTypeIndex);
  
   	fbTypePtr->specSizeStartAddr	= (uint32)(gSpecSizePtr + indexPtr->specTypeIndex);
	fbTypePtr->varSizeStartAddr		= (uint32)(gVarSizePtr + indexPtr->varTypeIndex);
   	fbTypePtr->outputSizeStartAddr	= (uint32)(gOutputSizePtr + indexPtr->outputTypeIndex);

    return(status);
}



uint32	busFbCodeOutputDataWriteWithType
				(
			uint32	moduleId,
			uint32	taskId,
			uint32	fbId,
            uint32	fbBlockAddr,
			uint32  writeIndex,     
			uint32  writeType,
            uint32	*dataPtr
				)
{
	uint32	status = NO_ERROR;
	strFbTaskTblMemInfo	segInfo;
	strMemPointerInfo	memInfo;
	uint32	*outputStartPtr, *outputDataPtr;
	strFbMemInfo	fbMemInfo;

	strFbCodeTypePointerInfo	ParaTypeInfo;
	uint32 *outputSizePtr ,outputWriteSize;
	uint32 *outputTypePtr;

    
    memoryClear ( (uint8 *) &segInfo, sizeof(strFbTaskTblMemInfo));

	if( (status = busSegTblInfoRead
					( 
				moduleId,
            	taskId,
				(strFbTaskTblMemInfo *) &segInfo
					)
		) != NO_ERROR
	)
    	return(status);

	
   if ( 	(fbBlockAddr < 0) ||
    		(fbBlockAddr > MAX_MPU_BLOCK_ADDR)
		)
 		return(L_FB_ADDR_ERR);

	
    memoryClear( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
	if( (status = busFbMemInfoRead (	moduleId,
    								fbBlockAddr,
                                    (strFbMemInfo *) &fbMemInfo
								)
		) != NO_ERROR
	)
    	return(status);

	if( fbMemInfo.fbId != fbId)
    	return(FB_CODE_MISMATCH_ERR);

	else if( fbMemInfo.fbTaskId != taskId)
    	return(FB_TASKID_MISMATCH_ERR);

	else if( ( writeIndex < 1) || ( writeIndex > fbMemInfo.outputNo ) )
		return(FB_CODE_OUTPUT_INDEX_ERR);

	
	status = fbCodeParaTypePointerGet 
						(
					fbId,           								
				(strFbCodeTypePointerInfo	*)&ParaTypeInfo 
						);
    if(status != NO_ERROR) return(status);	
	
	outputSizePtr = (uint32 *)ParaTypeInfo.outputSizeStartAddr;
	outputSizePtr += (writeIndex - 1);

	
	outputTypePtr = (uint32 *)ParaTypeInfo.outputTypeStartAddr;
	outputTypePtr += (writeIndex - 1);
	
	if(CHK_IS_ANY_TYPE(*outputTypePtr)) {
		if((uint32)(GET_BASIC_DATA_TYPE(fbMemInfo.paraRunType)) != writeType)
			return(FB_CODE_OUTPUT_TYPE_ERR);
	} else {
		if((uint32)(GET_BASIC_DATA_TYPE(*outputTypePtr)) != writeType)
		return(FB_CODE_OUTPUT_TYPE_ERR);
	}

	
	if(GET_TYPE_SIZE(*outputTypePtr)==SIZE64_TYPE) outputWriteSize = 2;
	else outputWriteSize = 1;


	
    memoryClear ( (uint8 *) &memInfo, sizeof(strMemPointerInfo));
	if( (status = busFbMemPointerGet
						(
						moduleId,
                		(strMemPointerInfo	*) &memInfo
                		)
		) != NO_ERROR
	)
    	return(status);

	
	outputStartPtr = (uint32 *)memInfo.outputMemPtr;

    
	outputDataPtr = outputStartPtr + fbMemInfo.outputIndex + (*outputSizePtr);	

	memoryCopy ( 	(uint8 *)outputDataPtr,
					(uint8 *)dataPtr,
					DOUBLE_SIZE * outputWriteSize
				); 
	return(status);
}



uint32	segmentBusDataChecksumCheck(uint32 moduleId)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	strModuleInfo	moduleInfo;
    strFbTaskTblMemInfo	segTbl;
	uint32	taskId, segChecksum;
	strMemPointerInfo	memP;
	strFbMemInfo		*fbMemStartPtr;
	uint32 temp1 = 0, temp2 =0;
	
    (void)busFbMemPointerGet(moduleId,(strMemPointerInfo	*)&memP);
	fbMemStartPtr = (strFbMemInfo *)memP.fbMemPtr;
 
	
    memoryClear( (uint8 *) &moduleInfo, sizeof(strModuleInfo));
    moduleInfoRead(moduleId, &moduleInfo);
	
	for(taskId = 1; taskId <= moduleInfo.maxSegmentNum; taskId++) {
    	memoryClear( (uint8 *)&segTbl, sizeof(strFbTaskTblMemInfo));
		retVal = busSegTblInfoRead	(	moduleId,
            							taskId,
										(strFbTaskTblMemInfo	*) &segTbl
									);
		if(retVal != NO_ERROR){
        	printf( "[%s(%d)]-[%s(taskId::%d)]-[0x%x]\n",
            			(uint8 *)__FILE__, __LINE__,
                        "segmentBusDataChecksumCheck",taskId,retVal);
			continue;
		}
		
        
        if(segTbl.taskMode != PCS_RUN) continue;

		segChecksum = 0;
		status = segmentChecksumCalcu(
								&segTbl, 
								fbMemStartPtr, 
								&segChecksum,
								moduleId
							);
		if( (segChecksum != segTbl.logicCheckSum) || (status != NO_ERROR) ) {
        	printf( "[MODULE_ID : 0x%x]-", moduleId);
			printf( "[BUS CHECKSUM ERROR]-[SEG_ID : %d]-"\
				"[Calcu Checksum :: 0x%x]-[Read Checksum :: 0x%x]\n",
                taskId, segChecksum,segTbl.logicCheckSum
					);

			status = APP_SEG_CHECKSUM_ERR;

			
            
			
			if(moduleId == LOGIC_ID){
				temp1 = segTbl.fbUsedNum;
				temp2 = segTbl.logicType;
			}
			else{
				temp1 = segTbl.u1.fbStartBlockAddr;
				temp2 = segTbl.u2.fbEndBlockAddr;
			}
	    	memoryClear( (uint8 *)&segTbl, sizeof(strFbTaskTblMemInfo));
		    segTbl.logicCheckSumStatus 	= SEG_CHECKSUM_ERR;
    		segTbl.taskMode				= PCS_ERR;
			if(moduleId == LOGIC_ID){
				segTbl.fbUsedNum			= temp1; 
				segTbl.logicType			= temp2;
			}
			else{
				segTbl.u1.fbStartBlockAddr 	= temp1;
				segTbl.u2.fbEndBlockAddr 	= temp2;
			}

    		retVal = busSegTblInfoWrite( moduleId, taskId, &segTbl);
			if(retVal != NO_ERROR){
    	    	printf( "[%s(%d)]-[%s(taskId::%d)]-[0x%x]\n",
				(uint8 *)__FILE__, __LINE__, "busSegTblInfoWrite",taskId,retVal);
			}	
		}
    }

    return(status);
}


uint32	segmentLocalDataChecksumCheck(uint32 moduleId)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	strModuleInfo	moduleInfo;
    strFbTaskTblMemInfo	segTbl;
	uint32	taskId, segChecksum;
	strMemPointerInfo	memP;
	strFbMemInfo		*fbMemStartPtr;
	
    (void)fbMemPointerGet(moduleId,(strMemPointerInfo	*)&memP);

	fbMemStartPtr = (strFbMemInfo *)memP.fbMemPtr;
 
	
    moduleInfoRead(moduleId, &moduleInfo);
	
	for(taskId = 1; taskId <= moduleInfo.maxSegmentNum; taskId++) {
    	memoryClear( (uint8 *)&segTbl, sizeof(strFbTaskTblMemInfo));
		retVal = getFbTaskTblInfo	(	moduleId,
            						taskId,
									(strFbTaskTblMemInfo	*) &segTbl
								);
		if(retVal != NO_ERROR){
        	printf( "[%s(%d)]-[%s(taskId::%d)]-[0x%x]\n",
            			(uint8 *)__FILE__, __LINE__,
                        "segmentLocalDataChecksumCheck",taskId,retVal);
			continue;
		}
		
        
        if(segTbl.taskMode != PCS_RUN) continue;

		segChecksum = 0;
		status = segmentChecksumCalcu(
								&segTbl, 
								fbMemStartPtr, 
								&segChecksum,
								moduleId
							);
		if( (segChecksum != segTbl.logicCheckSum) || (status != NO_ERROR) ) {
        	printf( "[MODULE_ID : 0x%x]-", moduleId);
			printf( "[LOCAL CHECKSUM ERROR]-[SEG_ID : %d]-" \
				"[Calcu Checksum :: 0x%x]-[Read Checksum :: 0x%x]\n",
                taskId, segChecksum,segTbl.logicCheckSum
					);

			status = APP_SEG_CHECKSUM_ERR;

		    segTbl.logicCheckSumStatus 	= SEG_CHECKSUM_ERR;
    		segTbl.taskMode				= PCS_ERR;

    		retVal = segTblInfoWrite( moduleId, taskId, &segTbl);
			if(retVal != NO_ERROR){
    	    	printf( "[%s(%d)]-[%s(taskId::%d)]-[0x%x]\n",
				(uint8 *)__FILE__, __LINE__, "segTblInfoWrite",taskId,retVal);
			}	
		}
    }

    return(status);
}

void fbMemoryClearCheck(void)
{
#ifdef VXWORKS
#if (BOARD_TYPE == MVME2100_BOARD)
	uint8 *addrPtr = (uint8 *)ABORT_SWITCH_ADDR;
	uint8 data;
	uint8	*memStartPtr;
    
	data = (*addrPtr >> ABORT_SWITCH_BIT) & 0x1;

    memStartPtr = (uint8 *)gMpuFbStartAddr;

    if( data == ABORT_SWITCH_PUSH_VAL) {
		for(;;) {
			memoryClear( memStartPtr, MAX_MPU_MEM_SIZE+MAX_DIU_MEM_SIZE);

        	printf( "** MPU, DIU Downloading Data Clear !!\007\n");
			sysTaskDelay(systemClockRateGet() * 2);
        }
	}  
#endif
#endif
}


void moduleFbMemoryClear(uint32 moduleId, uint32 printFlag)
{
    switch(moduleId) 
    {
		case LOGIC_ID:
			sysExeInfoMemoryClear(LOGIC_ID);

	    	memoryClear( (uint8 *)gBusMpuFbStartAddr, MAX_MPU_DATA_SIZE);

            if(printFlag == PRINT_FLAG)
				printf("MPU Memory Clear !!\n");
        break;
		case ALL_ID:
			sysExeInfoMemoryClear(ALL_ID);
			
	    	memoryClear( (uint8 *)gBusMpuFbStartAddr, MAX_MPU_DATA_SIZE);

            if(printFlag == PRINT_FLAG)
				printf("ALL(MPU, DIU) Memory Clear !!\n");
        break;
        default:
        	printf( "[ERROR] - moduleFbMemoryClear() Module ID Err(0x%x) !!\n", moduleId);
        return;
    }

    return;
}


void sysExeInfoMemoryClear(uint32 moduleId)
{
    uint32	status = NO_ERROR;
	uint8 	*startMemPtr, clearData = 0;
    uint32	dataBytes = 0, loop;
	
	strSysExecInfo *sysExeInfoPtr;

	status =  fbSysExeInfoPointerGet(&sysExeInfoPtr, BUS_MEM_ACC);
	if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		return;
	}

    switch(moduleId) {
		case ALL_ID:
			startMemPtr = (uint8 *)sysExeInfoPtr;
			dataBytes	= sizeof(strSysExecInfo);
			for(loop = 0; loop < dataBytes; loop++, startMemPtr++) {
				status = myBusDataAcc(	__FILE__,
										__LINE__, 
										BUS_BYTE_ACC, 
										BUS_DATA_WRITE, 
										(uint8 *) startMemPtr, 
										(uint8 *) &clearData
									  );
				if(status)
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			}			
		break;
		case LOGIC_ID:
			startMemPtr = (uint8 *)&sysExeInfoPtr->mpuSysExeInfo;
			dataBytes	= sizeof(strModuleExecInfo);

			for(loop = 0; loop < dataBytes; loop++, startMemPtr++) {
				status = myBusDataAcc(	__FILE__,
										__LINE__, 
										BUS_BYTE_ACC, 
										BUS_DATA_WRITE, 
										(uint8 *) startMemPtr, 
										(uint8 *) &clearData
									  );
				if(status)
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			}			

			startMemPtr = (uint8 *)&sysExeInfoPtr->autovarSysExeInfo;
			dataBytes	= sizeof(strVariableExecInfo);
			for(loop = 0; loop < dataBytes; loop++, startMemPtr++) {
				status = myBusDataAcc(	__FILE__,
										__LINE__, 
										BUS_BYTE_ACC, 
										BUS_DATA_WRITE, 
										(uint8 *) startMemPtr, 
										(uint8 *) &clearData
									  );
				if(status)
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			}			

			startMemPtr = (uint8 *)&sysExeInfoPtr->flagvarSysExeInfo;
			dataBytes	= sizeof(strVariableExecInfo);
			for(loop = 0; loop < dataBytes; loop++, startMemPtr++) {
				status = myBusDataAcc(	__FILE__,
										__LINE__, 
										BUS_BYTE_ACC, 
										BUS_DATA_WRITE, 
										(uint8 *) startMemPtr, 
										(uint8 *) &clearData
									  );
				if(status)
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			}			

			startMemPtr = (uint8 *)&sysExeInfoPtr->sfcSysExeInfo;
			dataBytes	= sizeof(strSfcMaxExecInfo);
			for(loop = 0; loop < dataBytes; loop++, startMemPtr++) {
				status = myBusDataAcc(	__FILE__,
										__LINE__, 
										BUS_BYTE_ACC, 
										BUS_DATA_WRITE, 
										(uint8 *) startMemPtr, 
										(uint8 *) &clearData
									  );
				if(status)
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			}			

        break;

        default:
        	printf( "[ERROR] - sysExeInfoMemoryClear() MODULE ID Err(0x%x) !!\n", moduleId);
        return;
    }

	status =  fbSysExeInfoPointerGet(&sysExeInfoPtr, LOCAL_MEM_ACC);
	if(status != NO_ERROR){
      	printf( "[ERROR] - LOCAL SYS_EXEC_INFO READ Err(0x%x) !!\n", moduleId);
		return;
	}
    switch(moduleId) {
		case ALL_ID:
			startMemPtr = (uint8 *)sysExeInfoPtr;
			dataBytes	= sizeof(strSysExecInfo);
			memoryClear( (uint8 *)startMemPtr, dataBytes);
		break;

		case LOGIC_ID:
			startMemPtr = (uint8 *)&sysExeInfoPtr->mpuSysExeInfo;
			dataBytes	= sizeof(strModuleExecInfo);
			memoryClear( (uint8 *)startMemPtr, dataBytes);

			startMemPtr = (uint8 *)&sysExeInfoPtr->autovarSysExeInfo;
			dataBytes	= sizeof(strVariableExecInfo);
			memoryClear( (uint8 *)startMemPtr, dataBytes);

			startMemPtr = (uint8 *)&sysExeInfoPtr->flagvarSysExeInfo;
			dataBytes	= sizeof(strVariableExecInfo);
			memoryClear( (uint8 *)startMemPtr, dataBytes);

			startMemPtr = (uint8 *)&sysExeInfoPtr->sfcSysExeInfo;
			dataBytes	= sizeof(strSfcMaxExecInfo);
			memoryClear( (uint8 *)startMemPtr, dataBytes);
        break;

        default:
        	printf( "[ERROR] - sysExeInfoMemoryClear() MODULE ID Err(0x%x) !!\n", moduleId);
        return;
    }
}


uint32	allSegmentModeErrSet(uint32 moduleId)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	strModuleInfo	moduleInfo;
    strFbTaskTblMemInfo	segTbl;
	uint32	taskId;
	
	
    moduleInfoRead(moduleId, &moduleInfo);
	
	for(taskId = 1; taskId <= moduleInfo.maxSegmentNum; taskId++) {
    	memoryClear( (uint8 *)&segTbl, sizeof(strFbTaskTblMemInfo));
		retVal = busSegTblInfoRead	(	moduleId,
            						taskId,
									(strFbTaskTblMemInfo	*) &segTbl
								);
		if(retVal != NO_ERROR){
        	printf( "[%s(%d)]-[%s(taskId::%d)]-[0x%x]\n",
            			(uint8 *)__FILE__, __LINE__,
                        "segmentLocalDataChecksumCheck",taskId,retVal);
			continue;
		}
		
        
        if(segTbl.taskMode != PCS_RUN) continue;

   		segTbl.taskMode				= PCS_ERR;

		
   		retVal = segTblInfoWrite( moduleId, taskId, &segTbl);
		if(retVal != NO_ERROR){
   	    	printf( "[%s(%d)]-[%s(taskId::%d)]-[0x%x]\n",
			(uint8 *)__FILE__, __LINE__, "segTblInfoWrite",taskId,retVal);
		}	

		
   		retVal = busSegTblInfoWrite( moduleId, taskId, &segTbl);
		if(retVal != NO_ERROR){
   	    	printf( "[%s(%d)]-[%s(taskId::%d)]-[0x%x]\n",
			(uint8 *)__FILE__, __LINE__, "segTblInfoWrite",taskId,retVal);
		}	
    }

    return(status);
}


uint32	allSegmentModeStopSet(uint32 moduleId)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	strModuleInfo	moduleInfo;
    strFbTaskTblMemInfo	segTbl;
	uint32	taskId;
	strMemPointerInfo	memP;
	strFbMemInfo		*fbMemStartPtr;
	
    (void)fbMemPointerGet(moduleId,(strMemPointerInfo	*)&memP);

	fbMemStartPtr = (strFbMemInfo *)memP.fbMemPtr;
 
	
    moduleInfoRead(moduleId, &moduleInfo);
	
	for(taskId = 1; taskId <= moduleInfo.maxSegmentNum; taskId++) {
    	memoryClear( (uint8 *)&segTbl, sizeof(strFbTaskTblMemInfo));
		retVal = getFbTaskTblInfo	(	moduleId,
            						taskId,
									(strFbTaskTblMemInfo	*) &segTbl
								);
		if(retVal != NO_ERROR){
        	printf( "[%s(%d)]-[%s(taskId::%d)]-[0x%x]\n",
            			(uint8 *)__FILE__, __LINE__,
                        "allSegmentModeStopSet",taskId,retVal);
			continue;
		}
		
        
        if(segTbl.taskMode != PCS_RUN) continue;

   		segTbl.taskMode				= PCS_STOP;

   		retVal = segTblInfoWrite( moduleId, taskId, &segTbl);
		if(retVal != NO_ERROR){
   	    	printf( "[%s(%d)]-[%s(taskId::%d)]-[0x%x]\n",
			(uint8 *)__FILE__, __LINE__, "allSegmentModeStopSet",taskId,retVal);
		}	

		retVal = busSegTblInfoWrite( moduleId, taskId, &segTbl);
		if(retVal != NO_ERROR){
   	    	printf( "[%s(%d)]-[%s(taskId::%d)]-[0x%x]\n",
			(uint8 *)__FILE__, __LINE__, "allSegmentModeStopSet",taskId,retVal);
		}	
    }

    return(status);
}


void busMemFbDataCopyToLocalFbMem(void)
{
	uint32	status, loop;
	uint32	*busMemPtr, *localMemPtr;

    strSysExecInfo *sysBusExeInfoPtr;
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	uint8 retainFlag;
#elif (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
	uint32	copyNum;
	uint32 	curfbMaxBlockAddr, curfbMaxOutIndex;
	uint32	curMaxStepId, curMaxActId, curfbMaxParaIndex, curfbMaxPBTMOutIndex;
	strFbMemInfo	*fbMemInfoPtr;
#endif
	strStepInfo *busStepMemPtr, *localStepMemPtr; 
	strActionInfo *busActMemPtr, *localActMemPtr;
	
	
	
	status =  fbSysExeInfoPointerGet(&sysBusExeInfoPtr, BUS_MEM_ACC);
	if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		return;
	}
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)

	
	localMemPtr = (uint32 *)gSysExeInfoLocalPtr;
	busMemPtr 	= (uint32 *)gSysExeInfoBusPtr;

	fourBytesDataCopy (localMemPtr, busMemPtr,MAX_SYS_MEM_SIZE/4);

	
	
	
	fbMemInfoPtr = gMpuFbMemInfoLocalPtr;
	curfbMaxBlockAddr = sysBusExeInfoPtr->mpuSysExeInfo.curfbMaxBlockAddr;
	
	for(loop = 0; loop < curfbMaxBlockAddr; loop++,fbMemInfoPtr++) {
		if(	fbMemInfoPtr->fbId == 0)continue;
		
		if(fbMemInfoPtr->fbId > MAX_FB_CODE_ID){
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, L_FB_CODE_ERR);
			continue;
		}
				
		if(gFbCodeSortingTableInfo[fbMemInfoPtr->fbId].dualCopyFlag == DATA_SYNC_DISABLE)	 
			continue;
		
		
		localMemPtr = (uint32 *)gMpuSpecAndVarDataLocalPtr;
		busMemPtr 	= (uint32 *)gMpuSpecAndVarDataBusPtr;

		localMemPtr += fbMemInfoPtr->specIndex;
		busMemPtr	+= fbMemInfoPtr->specIndex;

		copyNum 	= fbMemInfoPtr->inputNo + fbMemInfoPtr->intlVarNo;

		fourBytesDataCopy (localMemPtr, busMemPtr,copyNum);
	}

	
	busMemPtr 	= (uint32 *)gMpuOutputDataBusPtr;
	localMemPtr = (uint32 *)gMpuOutputDataLocalPtr;
	
	curfbMaxOutIndex = sysBusExeInfoPtr->mpuSysExeInfo.curfbMaxOutIndex;
	
	fourBytesDataCopy (localMemPtr, busMemPtr,curfbMaxOutIndex);

	
	
	localMemPtr = (uint32 *)gRetainVarInfoLocalPtr;
	busMemPtr 	= (uint32 *)gRetainVarInfoBusPtr;

	fourBytesDataCopy (localMemPtr, busMemPtr,MAX_MPU_RETAIN_NUM/32);

	
	localMemPtr = (uint32 *)gAutoVariableLocalPtr;
	busMemPtr 	= (uint32 *)gAutoVariableBusPtr;

	fourBytesDataCopy (localMemPtr, busMemPtr,MAX_MPU_AUTOVAR_NUM);
		
	
	localMemPtr = (uint32 *)gMmemVariableLocalPtr;
	busMemPtr 	= (uint32 *)gMmemVariableBusPtr;

	fourBytesDataCopy (localMemPtr, busMemPtr,MAX_MPU_MMEM_NUM);

	
	localMemPtr = (uint32 *)gSTSharedMemDataLocalPtr;
	busMemPtr 	= (uint32 *)gSTSharedMemDataBusPtr;

	fourBytesDataCopy (localMemPtr, busMemPtr,MAX_SHARED_MEM_NUM);
	
	
	localStepMemPtr	= (strStepInfo *)gSFCStepInfoLocalPtr;
	busStepMemPtr 	= (strStepInfo *)gSFCStepInfoBusPtr;
	
	curMaxStepId = sysBusExeInfoPtr->sfcSysExeInfo.curMaxStepId;
	for(loop = 0; loop < curMaxStepId; loop++,busStepMemPtr++,localStepMemPtr++) {
		status = myBusDataAcc(	__FILE__,
								__LINE__,	
								BUS_LONG_ACC, 
								BUS_DATA_READ,
								(uint8 *)&busStepMemPtr->T, 
								(uint8 *)&localStepMemPtr->T 
							);

		if(status == NO_ERROR)
			status = myBusDataAcc(	__FILE__,
									__LINE__,	
									BUS_LONG_ACC, 
									BUS_DATA_READ,
									(uint8 *)&busStepMemPtr->X, 
									(uint8 *)&localStepMemPtr->X 
								);

		if(status == NO_ERROR)
			status = myBusDataAcc(	__FILE__,
									__LINE__,	
									BUS_LONG_ACC, 
									BUS_DATA_READ,
									(uint8 *)&busStepMemPtr->CX, 
									(uint8 *)&localStepMemPtr->CX 
								);
		if(status != NO_ERROR) 
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
	}
	
	localActMemPtr 	= (strActionInfo *)gSFCActionInfoLocalPtr;
	busActMemPtr	= (strActionInfo *)gSFCActionInfoBusPtr;

	curMaxActId = sysBusExeInfoPtr->sfcSysExeInfo.curMaxActId;
	for(loop = 0; loop < curMaxActId; loop++,busActMemPtr++,localActMemPtr++) {
		status = myBusDataAcc(	__FILE__,
								__LINE__,	
								BUS_LONG_ACC, 
								BUS_DATA_READ,
								(uint8 *)&busActMemPtr->actCtrlData, 
								(uint8 *)&localActMemPtr->actCtrlData 
							);
		if(status == NO_ERROR)
			status = myBusDataAcc(	__FILE__,
									__LINE__,	
									BUS_LONG_ACC, 
									BUS_DATA_READ,
									(uint8 *)&busActMemPtr->elapsedTime, 
									(uint8 *)&localActMemPtr->elapsedTime 
								);
		if(status != NO_ERROR)
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
	}
	
	localMemPtr = (uint32 *)gDiuSpecAndVarDataLocalPtr;
	busMemPtr 	= (uint32 *)gDiuSpecAndVarDataBusPtr;

	curfbMaxParaIndex = sysBusExeInfoPtr->diuSysExeInfo.curfbMaxParaIndex;
	fourBytesDataCopy (	localMemPtr, 
						busMemPtr,
						curfbMaxParaIndex
					);
	
	
	localMemPtr = (uint32 *)gDiuOutputDataLocalPtr;
	busMemPtr 	= (uint32 *)gDiuOutputDataBusPtr;

	curfbMaxOutIndex = sysBusExeInfoPtr->diuSysExeInfo.curfbMaxOutIndex;

	fourBytesDataCopy (	localMemPtr, 
						busMemPtr,
						curfbMaxOutIndex
					);

	
	localMemPtr = (uint32 *)gFieldbusOutputDataLocalPtr;
	busMemPtr 	= (uint32 *)gFieldbusOutputDataBusPtr;

	curfbMaxPBTMOutIndex = sysBusExeInfoPtr->fbtmSysExeInfo.curfbMaxPBTMOutIndex;
	fourBytesDataCopy (	localMemPtr, 
						busMemPtr,
						curfbMaxPBTMOutIndex
					);
#endif

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)

	
	localMemPtr = (uint32 *)gMpuSpecAndVarDataLocalPtr;
	busMemPtr 	= (uint32 *)gMpuSpecAndVarDataBusPtr;

	for(loop = 0; loop < (sysBusExeInfoPtr->mpuSysExeInfo.curfbMaxParaIndex); loop++,busMemPtr++,localMemPtr++) {
		status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
								BUS_DATA_READ ,
								(uint8 *)busMemPtr, 
								(uint8 *)localMemPtr
							 );
		if(status != NO_ERROR) 
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
	}
	
	busMemPtr 	= (uint32 *)gMpuOutputDataBusPtr;
	localMemPtr = (uint32 *)gMpuOutputDataLocalPtr;

	for(loop = 0; loop < (sysBusExeInfoPtr->mpuSysExeInfo.curfbMaxOutIndex); loop++,busMemPtr++,localMemPtr++) {
		status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
								BUS_DATA_READ,
								(uint8 *)busMemPtr, 
								(uint8 *)localMemPtr
							 );
		if(status != NO_ERROR) 
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
	}
	
	localMemPtr = (uint32 *)gRetainVarInfoLocalPtr;
	busMemPtr 	= (uint32 *)gRetainVarInfoBusPtr;

	for(loop = 1; loop <= MAX_MPU_RETAIN_NUM/32; loop++,busMemPtr++,localMemPtr++) 
	{
		status = myBusDataAcc(__FILE__,__LINE__, BUS_LONG_ACC, BUS_DATA_READ,
								(uint8 *)busMemPtr, (uint8 *)localMemPtr);
		if(status != NO_ERROR) 
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
	}
	
	
	localMemPtr = (uint32 *)gAutoVariableLocalPtr;
	busMemPtr 	= (uint32 *)gAutoVariableBusPtr;

	for(loop = 1; loop <= MAX_MPU_AUTOVAR_NUM; loop++,busMemPtr++,localMemPtr++) 
	{
		status = busVariableRetainInfoRead(AUTO_VAR, loop, &retainFlag);
		if((status == NO_ERROR) && (retainFlag == EN_RETAIN)) 
	{
			status = myBusDataAcc(__FILE__,__LINE__, BUS_LONG_ACC, BUS_DATA_READ,
									(uint8 *)busMemPtr, (uint8 *)localMemPtr);
			if(status != NO_ERROR) 
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		}
	}

	
	localMemPtr = (uint32 *)gMmemVariableLocalPtr;
	busMemPtr 	= (uint32 *)gMmemVariableBusPtr;

	for(loop = 0; loop < MAX_MPU_MMEM_NUM; loop++,busMemPtr++,localMemPtr++) 
	{
		status = busVariableRetainInfoRead(MMEM_VAR, loop, &retainFlag);
		if((status == NO_ERROR) && (retainFlag == EN_RETAIN)) {
			status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
									BUS_DATA_READ,
									(uint8 *)busMemPtr, 
									(uint8 *)localMemPtr
								 );
			if(status != NO_ERROR) 
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		}
	}

	
	localMemPtr = (uint32 *)gSTSharedMemDataLocalPtr;
	busMemPtr 	= (uint32 *)gSTSharedMemDataBusPtr;

	for(loop = 0; loop < MAX_SHARED_MEM_NUM ; loop++,busMemPtr++,localMemPtr++) 
	{
			status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
									BUS_DATA_READ,
									(uint8 *)busMemPtr, 
									(uint8 *)localMemPtr
								 );
			if(status != NO_ERROR) 
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
	}

	
	localStepMemPtr	= (strStepInfo *)gSFCStepInfoLocalPtr;
	busStepMemPtr = (strStepInfo *)gSFCStepInfoBusPtr;

	for(loop = 0; loop < (sysBusExeInfoPtr->sfcSysExeInfo.curMaxStepId); loop++,busStepMemPtr++,localStepMemPtr++) 
	{
		status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, BUS_DATA_READ,
								(uint8 *)&busStepMemPtr->T, (uint8 *)&localStepMemPtr->T );
		if(status == NO_ERROR) 
		{
			status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, BUS_DATA_READ,
								(uint8 *)&busStepMemPtr->X, (uint8 *)&localStepMemPtr->X );
		}
		if(status == NO_ERROR) 
		{
			status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, BUS_DATA_READ,
								(uint8 *)&busStepMemPtr->CX, (uint8 *)&localStepMemPtr->CX );
		}
		if(status != NO_ERROR) 
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
	}
	
	localActMemPtr 	= (strActionInfo *)gSFCActionInfoLocalPtr;
	busActMemPtr= (strActionInfo *)gSFCActionInfoBusPtr;

	for(loop = 0; loop < (sysBusExeInfoPtr->sfcSysExeInfo.curMaxActId); loop++,busActMemPtr++,localActMemPtr++) 
	{
		status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, BUS_DATA_READ,
								(uint8 *)&busActMemPtr->actCtrlData, (uint8 *)&localActMemPtr->actCtrlData );
		if(status == NO_ERROR) 
		{
			status = myBusDataAcc(__FILE__,__LINE__, BUS_LONG_ACC, BUS_DATA_READ,
								(uint8 *)&busActMemPtr->elapsedTime, (uint8 *)&localActMemPtr->elapsedTime );
		}
		
		if(status != NO_ERROR) 
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
	}
		
	localMemPtr = (uint32 *)gDiuSpecAndVarDataLocalPtr;
	busMemPtr 	= (uint32 *)gDiuSpecAndVarDataBusPtr;

	for(loop = 0; loop < (sysBusExeInfoPtr->diuSysExeInfo.curfbMaxParaIndex); loop++,busMemPtr++,localMemPtr++) 
	{
		status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC,BUS_DATA_READ,
								(uint8 *)busMemPtr, (uint8 *)localMemPtr);
							 
		if(status != NO_ERROR) 
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
	}
	
	localMemPtr = (uint32 *)gDiuOutputDataLocalPtr;
	busMemPtr 	= (uint32 *)gDiuOutputDataBusPtr;

	for(loop = 0; loop < (sysBusExeInfoPtr->diuSysExeInfo.curfbMaxOutIndex); loop++,busMemPtr++,localMemPtr++) 
	{
		status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, BUS_DATA_READ,
								(uint8 *)busMemPtr, (uint8 *)localMemPtr);
		if(status != NO_ERROR) 
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
	}

	
	localMemPtr = (uint32 *)gFieldbusOutputDataLocalPtr;
	busMemPtr 	= (uint32 *)gFieldbusOutputDataBusPtr;

	for(loop = 0; loop < (sysBusExeInfoPtr->fbtmSysExeInfo.curfbMaxPBTMOutIndex); loop++,busMemPtr++,localMemPtr++) 
	{
		status = myBusDataAcc(__FILE__,__LINE__, BUS_LONG_ACC, BUS_DATA_READ,
								(uint8 *)busMemPtr, (uint8 *)localMemPtr);
		if(status != NO_ERROR) 
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
	}
#endif
	return;
}


uint32 busMemoryDataCopyToLocalMem(uint32	copyFlag)
{
	uint32 *srcPtr, *destPtr;
	uint32	dataNum, status = NO_ERROR;

	if(copyFlag == MPU_DIU_DUAL_ACC) {	
    	
	    srcPtr 		= (uint32 *)gBusMpuFbStartAddr;
		destPtr 	= (uint32 *)gMpuFbStartAddr;
   		dataNum		= MAX_MPU_MEM_SIZE / 4;
    	fourBytesDataCopy (destPtr, srcPtr, dataNum);

    	
		srcPtr 		= (uint32 *)gBusDiufbStartAddr;
		destPtr 	= (uint32 *)gDiufbStartAddr;
   		dataNum		= MAX_DIU_MEM_SIZE / 4;
    	fourBytesDataCopy (destPtr, srcPtr, dataNum);
    }
    else if(copyFlag == MPU_ONLY_DUAL_ACC) {
    	
    	srcPtr 		= (uint32 *)gBusMpuFbStartAddr; 
    	destPtr 	= (uint32 *)gMpuFbStartAddr; 
   		dataNum		= MAX_MPU_MEM_SIZE / 4;

    	fourBytesDataCopy (destPtr, srcPtr, dataNum);
    }
    else {
    	status = DUAL_STATUS_FLAG_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
    }
	
	return(status);
}


uint32 getLogicTaskCycletime(uint32 moduleId, uint32 taskId, float *timePtr)
{
	uint32	status = NO_ERROR;
	strFbTaskTblMemInfo	segTblInfo;

    memoryClear( 	(uint8 *) &segTblInfo,
    				sizeof(strFbTaskTblMemInfo)
				);

 	status = getFbTaskTblInfo (	moduleId,
    							taskId,
                                &segTblInfo
							);
	if(status != NO_ERROR) 
	{
		setErrorCode	(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	if(moduleId == LOGIC_ID) 
	{
    	*timePtr = (float) (segTblInfo.taskCycleTime * CYCLIC_FB_EXEC_TIME_BASE);
#if 0
        *timePtr *= (float)0.001; 
#else
        *timePtr *= (float)0.0001; 
#endif
	}
	
    return(status);
}


typedef struct {
   	uint32	taskId;
    uint32	checksumStatus;
    uint32	taskMode;
	uint32  segLogic;
}strSegDisplayInfo;

void segmentDebugInfoRead(uint32 moduleId, uint32 taskId,strSegDisplayInfo *dataPtr)
{
	uint32	status = NO_ERROR;
	strFbTaskTblMemInfo	segTblInfo;

    memoryClear ( (uint8 *)&segTblInfo, sizeof(strFbTaskTblMemInfo));
    status = getFbTaskTblInfo(moduleId, taskId, &segTblInfo); 

	
   	if(status == NO_ERROR) {
   		dataPtr->taskId 			= segTblInfo.taskId;
       	dataPtr->checksumStatus = segTblInfo.logicCheckSumStatus;
       	dataPtr->taskMode		= segTblInfo.taskMode;
		if((moduleId == LOGIC_ID) && (taskId == segTblInfo.taskId))
			dataPtr->segLogic	= segTblInfo.logicType;
		else 
			dataPtr->segLogic	= NOT_LOGIC_TYPE;
   	}
    else {

    	printf( "[ERROR] - Segment Tbl Info Read Err[0x%x]\n",
        					status);

	}
}

void segmentStatusPrint(uint32 factor,strSegDisplayInfo *dataPtr)
{
	int8 *segModeStr[] =
    {
		"NONE(0)","INIT(1)","DWL(2)",
		"DWLDN(3)","STOP(4)","RUN(5)","DEBUG(6)","ERROR(7)", "INVAL"
    };
	int8 *checksumStr[] =
    {
    	(int8 *)"OK", (int8 *)"ERR"
	};
	int32	taskId, i;
	int32	strIndex = 0;
	strSegDisplayInfo *tmpPtr;

	printf( "\n");
    for(i = 1; i <= 10; i++) {
    	printf( "%6s","*******");
	}

	printf( "\n");
    printf( "%s", "TSK ID   *");
 
   for(i = 1; i <= 8; i++) {
    	taskId = i + (factor * 8);
    	printf("%7d",taskId);
    }

	printf( "\n");
	
   for(i = 1; i <= 10; i++) {
    	printf( "%6s","*******");
   }

	printf( "\n");

    printf( "%10s","TSK MODE *");
	
    tmpPtr = dataPtr;
    for(i = 1; i <= 8; i++, tmpPtr++) {
		if( (tmpPtr->taskMode >= PCS_INIT) &&
        	(tmpPtr->taskMode <= PCS_ERR)
			) {
			strIndex = tmpPtr->taskMode;}
		else if(tmpPtr->taskMode == 0) {
			strIndex = 0;}
		else {
			strIndex = 8;}
		printf( "%7s", segModeStr[strIndex]);
    }

    printf( "\n");

    printf( "%10s","CHECKSUM *");

    tmpPtr = dataPtr;
    for(i = 1; i <= 8;i++, tmpPtr++) {
		if(tmpPtr->checksumStatus == NO_ERROR) {
			strIndex = 0;}
		else {
			strIndex = 1;}
		printf( "%7s", checksumStr[strIndex]);
	}
	printf( "\n"); 
}

void segLogicTypePrint(uint32 factor,strSegDisplayInfo *dataPtr)
{
	uint32 i, strIndex = 0;
	strSegDisplayInfo *tmpPtr;

	int8 *segModeStr[] =
    {
		"FBD(0)","SFC(1)","NONE"
    };

    printf( "%10s","LOGIC   *");
    
    tmpPtr = dataPtr;

    for(i = 1; i <= 8;i++, tmpPtr++) 
    {
		if(tmpPtr->segLogic == FB_LOGIC_TYPE) 
			strIndex = 0;
		else if(tmpPtr->segLogic == SFC_LOGIC_TYPE) 
			strIndex = 1;
		else 
			strIndex = 2;
			
		printf( "%7s", segModeStr[strIndex]);
	}

	printf( "\n\n"); 
}
 
void segmentStatusDisplay(uint32 moduleId, uint32 keyHitEnable)
{
    int8 *moduleStr[] =
    {
    	(int8 *)"LOGIC", (int8 *)"DIU", (int8 *)"TMTC", (int8 *)"FIELDBUS"
	};
	uint32 	strIndex = 0;
	uint32	maxLoopCount = 0, i,j, taskId;
    strSegDisplayInfo	segInfo[8];

	int8 line[16], *pLine;
    int8 string[] =
    {
    	"type <q>,<Q> to Stop or <RET> to Continue ? "
    };

    if(moduleId == LOGIC_ID)
    {
		strIndex = 0;
        maxLoopCount = (uint32)ceil((double)MAX_LOGIC_TASK_NUM/8.0);
	} 
	else 
	{
		return;
	}

    printf( "\n");
    printf( "\t************** << %s Task Status >>*************\n",
							moduleStr[strIndex]);

	for( i = 0; i < maxLoopCount; i++) 
	{

    	memoryClear( (uint8 *) &segInfo[0], sizeof(strSegDisplayInfo) * 8);

		for(j = 1; j <= 8; j++) 
		{
			taskId = j + (i * 8);
			if ((moduleId == LOGIC_ID && taskId >= 1 && taskId <= MAX_LOGIC_TASK_NUM))
			{
				segmentDebugInfoRead( moduleId, taskId, &segInfo[j-1]);
			}
		}

		segmentStatusPrint(i, &segInfo[0]);
		
		if(moduleId == LOGIC_ID)
			segLogicTypePrint(i, &segInfo[0]);

        if(i == (maxLoopCount - 1))
        	break;

		if(keyHitEnable)
		{
	        printf((const char *)string);
	
	        ioRead(gDebugPortFd, line, sizeof(line));
	
	        pLine = line;
			
	        if( (*pLine == 'q') || (*pLine == 'Q'))
	        	break;
		}
	}	
	
	return;
}


void fbRunningErrorInfoDisplay(uint32 arg1, uint32 arg2)
{
	uint32  status = NO_ERROR;
	uint32	moduleId = 0, bIndex, maxSegNum;	
	uint32	fbBlockAddr;
 	strFbMemInfo	fbMemInfo;
	strModuleExecInfo sysExeInfo;
	uint32  runErrCnt = 0, runFBCnt = 0;
	
	memoryClear( (int8 *)&sysExeInfo, sizeof(strModuleExecInfo));

	moduleId 	= LOGIC_ID;
	maxSegNum 	= MAX_MPU_SCHD_SEG_ID;
			
	(void)fdprintf(gDebugPortFd, "Waitting for  second ......!!\n");

	if( (fbSysExeModuleInfoRead(moduleId, &sysExeInfo)) == NO_ERROR ) 
	{
		for(fbBlockAddr = 1; fbBlockAddr <= sysExeInfo.curfbMaxBlockAddr; fbBlockAddr++)
		{
			status = fbCodeMemInfoRead(moduleId, fbBlockAddr,&fbMemInfo); 
			
			if( (status != NO_ERROR) || (fbMemInfo.fbId == 0) )
			{
				continue;
			}
				
			runFBCnt++;
			if(fbMemInfo.fbRunStatus != NO_ERROR) 
			{
				runErrCnt++;
				(void)fdprintf(gDebugPortFd, "[ERROR]-[BLOCK ADDR :: %05d]-[FB CODE :: %03d]-[Error:: %0x%04x]\n",
							fbBlockAddr, fbMemInfo.fbId, fbMemInfo.fbRunStatus
						);

			}
		}
		sysTaskDelay(systemClockRateGet()/ 10);			
	}
	else 
	{
		(void)fdprintf(gDebugPortFd,"[ERROR]-fbSysExeModuleInfoRead() Call Error !!\n");
	}

	(void)fdprintf(gDebugPortFd, " * Current MAX FB Address : %d\n", sysExeInfo.curfbMaxBlockAddr);
	(void)fdprintf(gDebugPortFd, " * Current Running FB Num : %d\n", runFBCnt);
	(void)fdprintf(gDebugPortFd, " * FB Running Error Count : %d\n", runErrCnt);
	
	return;
}


void	cyclicSegExecTimeCalcu	(	
									uint32	clockCount, 
									strExecTimeInfo *timePtr
								)
{
	float	execMiliSec, fData1,fData2;
	    
    fData1 = (float)(clockCount*1000); 
#ifdef SYS_AUX_CLK_USED
    fData2 = (float)systemAuxClockRateGet();
#else
    fData2 = (float)systemClockRateGet();
#endif
    execMiliSec = fData1 / fData2;

    timePtr->curExecTime	= execMiliSec;

	if(timePtr->minExecTime == 0.0)
    	timePtr->minExecTime = execMiliSec;	
	else if(timePtr->minExecTime > execMiliSec)
  		timePtr->minExecTime = execMiliSec;	
	
	if(timePtr->maxExecTime == 0.0)
    	timePtr->maxExecTime = execMiliSec;	
	else if(timePtr->maxExecTime < execMiliSec)
  		timePtr->maxExecTime = execMiliSec;	

	return;
}


void	minFreeTimeCalcu	(	uint32	moduleId,
								uint32	taskId,	
								uint32	clockCount 
							)
{
	float	execMiliSec, fData1,fData2;
	float	*outputMemStartPtr = NULL, minFreeTime;
	float	refCycleTime, diffTime;
	uint32  cycleTimeId;
	float	curFbExecTime;

	float	*outputMemBusStartPtr = NULL;		

	strModuleExecInfo sysExeInfo;		
	uint32  status = NO_ERROR;
	
	
	if((clockCount < 0)) return;
	
	if(moduleId == LOGIC_ID)
	{
		cycleTimeId = gMpuSegSchdInfoPtr->cyclicSeg[taskId - 1].cyclicAndNonCyclicTimeId;
		refCycleTime = (float) (cycleTimeId * CYCLIC_FB_EXEC_TIME_BASE);
	}

#ifdef SYS_AUX_CLK_USED
    fData1 = (float)(clockCount * 1000); 
    fData2 = (float)systemAuxClockRateGet();
#else
    fData1 = (float)(clockCount);		
    fData2 = (float)systemClockRateGet();
#endif

    execMiliSec = fData1 / fData2;
    
   
    curFbExecTime = execMiliSec;
    
    diffTime = refCycleTime - execMiliSec;

	if(diffTime < 0.0) return;
	
	if((moduleId == LOGIC_ID))
	{
		status = fbSysExeModuleInfoRead(moduleId, &sysExeInfo); 
		if(status == NO_ERROR){
			
			minFreeTime = sysExeInfo.minFreeTime;
			
			if(minFreeTime < 0.0) 
				sysExeInfo.minFreeTime = diffTime;

			if((minFreeTime == 0.0) || (minFreeTime > diffTime)){
				sysExeInfo.minFreeTime = diffTime;
			}
			
			if(moduleId == LOGIC_ID)
				gSysExeInfoLocalPtr->mpuSysExeInfo.minFreeTime = sysExeInfo.minFreeTime;

		}
		
		else 
		{
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		}
	}
	return;
}



uint32 moduleInfoRead
			(
		uint32 	moduleId,
        strModuleInfo	*infoPtr
        	)
{
	uint32	status = NO_ERROR;

	switch(moduleId) {
    	case LOGIC_ID:
        	infoPtr->moduleId		= LOGIC_ID ; 
        	infoPtr->maxSegmentNum 	= MAX_LOGIC_TASK_NUM;
            infoPtr->maxFbBlockAddr = MAX_MPU_BLOCK_ADDR;
            infoPtr->maxFbParaSize	= MAX_MPU_SPEC_NUM * 4;
            infoPtr->maxMemorySize	= MAX_USABLE_MPU_MEM_SIZE;
			infoPtr->maxFbParaNum   = MAX_MPU_SPEC_NUM;
			infoPtr->maxFbOutputNum = MAX_MPU_OUTPUT_NUM;
		break;

        default:
        	status = L_MODULE_ID_ERR;
		break;
	}
    return(status);
}



uint32	fbCodeSpecDataReadAll
	(strFbDataReadInfo* readDataInfo, uint32* retSpecDataPtr, uint32 memType)
{
	uint32	status = NO_ERROR;
    uint32	*specStartPtr, *specDataPtr;
	strFbMemInfo	fbMemInfo;
	strMemPointerInfo	memP;

    memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
    memoryClear ( (uint8 *) &memP, sizeof(strMemPointerInfo));

     
	if(memType==LOCAL_MEM_ACC) {
		status = fbMemInfoRead	( readDataInfo->moduleId, readDataInfo->blockAddr,
            					(strFbMemInfo	*) &fbMemInfo
            				);
	}
	else if(memType==BUS_MEM_ACC) {
		status = busFbMemInfoRead( readDataInfo->moduleId, readDataInfo->blockAddr,
            					(strFbMemInfo	*) &fbMemInfo
            				);
	}
	else status = MEM_ACC_TYPE_ERR;

	if(status != NO_ERROR) goto MODULE_END;

    
    if( readDataInfo->fbId != fbMemInfo.fbId) {
    	status = FB_CODE_MISMATCH_ERR;
        goto MODULE_END;
	}

	if(memType==LOCAL_MEM_ACC) {
		status = fbMemPointerGet(	readDataInfo->moduleId,
               						(strMemPointerInfo	*)&memP
			                	);
	}
	else if(memType==BUS_MEM_ACC) {
		status = busFbMemPointerGet( readDataInfo->moduleId,
               						(strMemPointerInfo	*)&memP
			                	);
	}

	if(status != NO_ERROR) goto MODULE_END;

	specStartPtr = (uint32 *)memP.paraMemPtr;
	specDataPtr = (uint32 *)(specStartPtr + fbMemInfo.specIndex);

    
    memoryCopy ( (uint8 *) retSpecDataPtr,
    			 (uint8 *) specDataPtr,
                 fbMemInfo.inputSize * DOUBLE_SIZE
				);
	
MODULE_END:
    return(status);
}


uint32	fbCodeOutputDataWriteWithType
				(
			uint32	moduleId,
			uint32	taskId,
			uint32	fbId,
            uint32	fbBlockAddr,
			uint32  writeIndex,     
			uint32  writeType,
            uint32	*dataPtr
				)
{
	uint32	status = NO_ERROR;
	strFbTaskTblMemInfo	segInfo;
	strMemPointerInfo	memInfo;
	uint32	*outputStartPtr, *outputDataPtr;
	strFbMemInfo	fbMemInfo;
	
	strFbCodeTypePointerInfo	ParaTypeInfo;
	uint32 *outputSizePtr ,outputWriteSize;
	uint32 *outputTypePtr;

    
    memoryClear ( (uint8 *) &segInfo, sizeof(strFbTaskTblMemInfo));

	if( (status = getFbTaskTblInfo
					( 
				moduleId,
            	taskId,
				(strFbTaskTblMemInfo *) &segInfo
					)
		) != NO_ERROR
	){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		
    	return(status);
	}
	
	
    if ( 	(fbBlockAddr < 0) ||
    		(fbBlockAddr > MAX_MPU_BLOCK_ADDR)
		){
		
		status = L_FB_ADDR_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			
 		return(status);
	}
	
    memoryClear( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
	if( (status = fbMemInfoRead (	moduleId,
    								fbBlockAddr,
                                    (strFbMemInfo *) &fbMemInfo
								)
		) != NO_ERROR
	){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
    	return(status);
	}
	if( fbMemInfo.fbId != fbId) 
	{
		status = FB_CODE_MISMATCH_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
    	return(status);
	}
	else if( fbMemInfo.fbTaskId != taskId){
		status = FB_TASKID_MISMATCH_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
    	return(status);
	}
	else if( ( writeIndex < 1 ) || (writeIndex > fbMemInfo.outputNo) ){
		status = FB_CODE_OUTPUT_INDEX_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
    	return(status);
	}

	
	status = fbCodeParaTypePointerGet 
						(
					fbId,           								
				(strFbCodeTypePointerInfo	*)&ParaTypeInfo 
						);
    if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
    	return(status);	
	}
		
	outputSizePtr = (uint32 *)ParaTypeInfo.outputSizeStartAddr;
	outputSizePtr += (writeIndex - 1);

	
	outputTypePtr = (uint32 *)ParaTypeInfo.outputTypeStartAddr;
	outputTypePtr += (writeIndex - 1);
	
	if(CHK_IS_ANY_TYPE(*outputTypePtr)) {
		if((uint32)(GET_BASIC_DATA_TYPE(fbMemInfo.paraRunType)) != writeType) {
			status = FB_CODE_OUTPUT_TYPE_ERR;
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}
	} 
	else {
    	if(GET_BASIC_DATA_TYPE(*outputTypePtr) != writeType) {
			status = FB_CODE_OUTPUT_TYPE_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"req Type", writeType,
								"Basic Type",GET_BASIC_DATA_TYPE(*outputTypePtr),
								"NOT USING", 0 
							   );							 

			return(status);
		}
	}

	
	if(GET_TYPE_SIZE(*outputTypePtr)==SIZE64_TYPE) outputWriteSize = 2;
	else outputWriteSize = 1;

	
    memoryClear ( (uint8 *) &memInfo, sizeof(strMemPointerInfo));
	if( (status = fbMemPointerGet
						(
						moduleId,
                		(strMemPointerInfo	*) &memInfo
                		)
		) != NO_ERROR
	){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
    	return(status);
	}
	
	outputStartPtr = (uint32 *)memInfo.outputMemPtr;

    
	outputDataPtr = outputStartPtr + fbMemInfo.outputIndex + (*outputSizePtr);

	
	memoryCopy ( 	(uint8 *)outputDataPtr,
					(uint8 *)dataPtr,
					DOUBLE_SIZE * outputWriteSize
				); 

	
	status = busFbCodeOutputDataWriteWithType	(	moduleId,
													taskId,
													fbId,
													fbBlockAddr,
					 								writeIndex, 
					 								writeType, 
					 								dataPtr
					 							);
	
	if(status == NO_ERROR){
		
		status = fbDataWriteToRemoteVmeMemBoard(	moduleId,
													fbBlockAddr, 
													writeIndex,
													*dataPtr,
													DUAL_OUTPUT_WRITE, 
													&fbMemInfo
												);
		if(status)
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
	}
	else
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		
	return(status);
}

uint32	fbCodeSpecDataWriteWithType
					(
				uint32	moduleId,
                uint32	fbId,
				uint32	fbAddr,
                uint32	specIndex, 
				uint32  specType,
                uint32	*specData
                	)
{
	uint32	status = NO_ERROR;
    uint32	*specStartPtr, *specDataPtr;
	strFbMemInfo	fbMemInfo, busFbMemInfo;
	strMemPointerInfo	memP, busMemAddrInfo;
	uint32	*specTypePtr, *specSizePtr, specWriteSize;
	strFbCodeTypePointerInfo	typePInfo;
	strFbSpecActionInfo *specActionPtr;
	uint32	rmtSpecDataCopyFlag = 0;

    memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
    memoryClear ( (uint8 *) &memP, sizeof(strMemPointerInfo));

     
	status = fbMemInfoRead	(
								moduleId,
            					fbAddr,
            					(strFbMemInfo *)&fbMemInfo
            				);
	if(status != NO_ERROR) goto MODULE_END;

    
    if(fbId != fbMemInfo.fbId) {
    	status = FB_CODE_MISMATCH_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
                            "Block Addr",	fbAddr,
                            "Recv FB Code",	fbId,
                            "Read FB Code",	fbMemInfo.fbId
						  );
        goto MODULE_END;
	}

    
    if( (specIndex < 1) || (specIndex > fbMemInfo.inputNo) ){
    	status = FB_CODE_SPEC_INDEX_ERR; 
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
                            "Block Addr",	fbAddr,
                            "FB Code",		fbId,
                            "Spec Index",	specIndex
						  );
        goto MODULE_END;
	}

	status = fbCodeParaTypePointerGet(
								fbId,
								(strFbCodeTypePointerInfo *) &typePInfo
									);
	if(status != NO_ERROR) {
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	
    specTypePtr = (uint32 *)typePInfo.specTypeStartAddr;
	specTypePtr += (specIndex - 1);

	specSizePtr = (uint32 *)typePInfo.specSizeStartAddr;
	specSizePtr += (specIndex - 1);

	
	
	if( !(CHK_IS_ANY_TYPE(*specTypePtr)) && 
		(GET_BASIC_DATA_TYPE(*specTypePtr) != specType)) {
		status = FB_CODE_SPEC_TYPE_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	status = fbMemPointerGet	(
									moduleId,
               						(strMemPointerInfo	*)&memP
			                	);
	if(status != NO_ERROR) {
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

	specStartPtr = (uint32 *)memP.paraMemPtr;

    
	specDataPtr = (uint32 *)(specStartPtr + fbMemInfo.specIndex + (*specSizePtr));

    
	if(moduleId==LOGIC_ID) {
		specActionPtr = (strFbSpecActionInfo *)(specStartPtr + fbMemInfo.specActionIndex + (specIndex - 1));
		if(specActionPtr->refType == SPEC_TYPE_BYREF) {
			status = L_PARA_TUNE_ERR; 
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}
		else 
		{
			rmtSpecDataCopyFlag = 1;
		}
	}

	
	if(GET_TYPE_SIZE(*specTypePtr)==SIZE64_TYPE) specWriteSize = 2;
	else specWriteSize = 1;

    
    memoryCopy ( (uint8 *) specDataPtr,
    			 (uint8 *) specData,
                 DOUBLE_SIZE * specWriteSize
				);
	
	status = busFbMemInfoRead	(
								moduleId,
            					fbAddr,
            					(strFbMemInfo	*) &busFbMemInfo
            				);
	if(status != NO_ERROR) 
		goto MODULE_END;
	
   	
   	if( fbId != busFbMemInfo.fbId) {
   		status = FB_CODE_MISMATCH_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
                            "Block Addr", fbAddr,
                            "Local Fb Code", fbId,
                            "MemBoard Fb Code", busFbMemInfo.fbId
						  );
       	goto MODULE_END;
	}

   	
   	if(specIndex > busFbMemInfo.inputNo) {
   		status = FB_CODE_SPEC_INDEX_ERR; 
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
                            "fb Code", fbId,
                            "Req Spec Index", specIndex,
                            "MemBoard Spec No", busFbMemInfo.inputNo
						  );
       	goto MODULE_END;
	}

	status = busFbMemPointerGet	(moduleId, &busMemAddrInfo);
	if(status != NO_ERROR) goto MODULE_END;

	specStartPtr = (uint32 *)busMemAddrInfo.paraMemPtr;

   	
	specDataPtr = (uint32 *)(specStartPtr + busFbMemInfo.specIndex);
   	specDataPtr += specIndex - 1;

	status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
							BUS_DATA_WRITE ,
							(uint8 *)specDataPtr, 
							(uint8 *)specData
						 );
	if(status)
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
	else {
		
		status = fbDataWriteToRemoteVmeMemBoard(	moduleId,
													fbAddr, 
													specIndex,
													*specData,
													DUAL_SPEC_WRITE, 
													&fbMemInfo
												);
		if(status)
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		
		if((rmtSpecDataCopyFlag == 1)|| (moduleId != LOGIC_ID))
		{
			status = fbDataWriteToRemoteLocalMemory(	moduleId,
														fbAddr, 
														specIndex,
														*specData,
														specType, 
														&fbMemInfo
													);
			if(status)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		}
	}

MODULE_END:
	
    return(status);
}


uint32	fbCodeSpecDataReadWithType
				(strFbDataReadInfo *inputInfoPtr, uint32 *dataPtr, uint32 memType)
{
	uint32	status = NO_ERROR;
    uint32	*specStartPtr, *specDataPtr;
	strMemPointerInfo	memInfo;
	uint32  *specTypePtr,*specSizePtr,TypeSize;
	strFbSpecActionInfo *specActionDataPtr;

	strFbCodeTypePointerInfo typePInfo;
	strFbMemInfo	fbMemInfo;
	
    memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));

	 
	if(memType == LOCAL_MEM_ACC) {
		status = fbMemInfoRead ( inputInfoPtr->moduleId,
							 inputInfoPtr->blockAddr, 
							 (strFbMemInfo *) &fbMemInfo);
	}
	else if(memType == BUS_MEM_ACC) {
		status = busFbMemInfoRead  ( inputInfoPtr->moduleId,
								 inputInfoPtr->blockAddr, 
								 (strFbMemInfo *) &fbMemInfo);
	}
	else{
		status = MEM_ACC_TYPE_ERR;
	}

	if(status != NO_ERROR){
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"MODULE ID" ,inputInfoPtr->moduleId,
							"MEM TYPE " ,memType,
							"FB ADDR", inputInfoPtr->blockAddr );
		goto MODULE_END;
	}

    
    if( inputInfoPtr->fbId != fbMemInfo.fbId) {
    	status = FB_CODE_MISMATCH_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"FB Addr", inputInfoPtr->blockAddr,
							"Read FB Code",inputInfoPtr->fbId,
							"Set FB Code",fbMemInfo.fbId);
        goto MODULE_END;
	}

    
    if(inputInfoPtr->index > fbMemInfo.inputNo) {
    	status = FB_CODE_SPEC_INDEX_ERR; 
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
					"FB FB Code", inputInfoPtr->blockAddr,
					"Read Input Index",inputInfoPtr->index,
					"Set Input Index",fbMemInfo.inputNo);
					
        goto MODULE_END;
	}

	status = fbCodeParaTypePointerGet(
								inputInfoPtr->fbId,
								(strFbCodeTypePointerInfo *)&typePInfo
								);
	if(status != NO_ERROR) {
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

    specTypePtr = (uint32 *)typePInfo.specTypeStartAddr;
	specTypePtr += (inputInfoPtr->index - 1);

	specSizePtr = (uint32 *)typePInfo.specSizeStartAddr;
	specSizePtr += (inputInfoPtr->index - 1);


	
    memoryClear ( (uint8 *) &memInfo, sizeof(strMemPointerInfo));
	if(memType == LOCAL_MEM_ACC) {
		status = fbMemPointerGet(inputInfoPtr->moduleId, (strMemPointerInfo*) &memInfo);
	}
	else {
		status = busFbMemPointerGet( inputInfoPtr->moduleId, (strMemPointerInfo*) &memInfo);
	}

	if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

	specStartPtr = (uint32 *)memInfo.paraMemPtr;

	if(inputInfoPtr->moduleId == LOGIC_ID) {
		
		specDataPtr = (uint32 *)(specStartPtr + fbMemInfo.specIndex + (*specSizePtr));

		
		specActionDataPtr = (strFbSpecActionInfo *)(specStartPtr + fbMemInfo.specActionIndex + 
														(inputInfoPtr->index - 1));

		
		if(GET_TYPE_SIZE(*specTypePtr)==SIZE64_TYPE) TypeSize = 2;
		else TypeSize = 1;

			
		if(specActionDataPtr->refType==SPEC_TYPE_BYREF) {

			
			if(CHK_IS_ANY_TYPE(*specTypePtr)) {
				if(memType == LOCAL_MEM_ACC){
					status =  refDataCopyFromMem(	dataPtr, 
												*specDataPtr, 
												*specActionDataPtr,
												
												( ((fbMemInfo.paraRunType)&0xf00f) | ((*specTypePtr)&0xffff0ff0) ),
												TYPECASTING_ENABLE
											);
				}
				else 
				{
					status =  refDataCopyFromBusMem(	dataPtr, 
												*specDataPtr, 
												*specActionDataPtr,
												
												( ((fbMemInfo.paraRunType)&0xf00f) | ((*specTypePtr)&0xffff0ff0) ),
												TYPECASTING_ENABLE
											);
				}
				if(status)
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			} 
			else {
				if(memType == LOCAL_MEM_ACC){
					status = refDataCopyFromMem	(	dataPtr,
													*specDataPtr,
													*specActionDataPtr,
													*specTypePtr,
													0
												);
				}
				else{
					status = refDataCopyFromBusMem	(	dataPtr,
													*specDataPtr,
													*specActionDataPtr,
													*specTypePtr,
													0
												);
				}
				if(status)
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			}
		}

		
		else
			memoryCopy ((uint8*)dataPtr, (uint8*)specDataPtr,DOUBLE_SIZE *TypeSize);
	}
	else {
			
			specDataPtr = (uint32 *)(specStartPtr + fbMemInfo.specIndex + inputInfoPtr->index - 1);
		
			memoryCopy ((uint8*)dataPtr, (uint8*)specDataPtr,DOUBLE_SIZE);
	}

MODULE_END:
    return(status);
}


uint32	fbCodeVarDataReadWithType
				(strFbDataReadInfo *varInfoPtr, uint32 *dataPtr, uint32 memType)
{
	uint32	status = NO_ERROR;
	strMemPointerInfo	memInfo;
    strFbMemInfo	fbMemInfo;
	uint32	*varStartPtr, *varDataPtr;
	uint32  *varSizePtr, *varTypePtr;
	uint32  TypeSize;
	strFbCodeTypePointerInfo typePInfo;

	
    memoryClear( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
	memoryClear ( (uint8 *) &memInfo, sizeof(strMemPointerInfo));
	memoryClear ( (uint8 *) &typePInfo, sizeof(strFbCodeTypePointerInfo));

	if(memType == LOCAL_MEM_ACC) {
		status = fbMemInfoRead ( varInfoPtr->moduleId,
							 varInfoPtr->blockAddr, 
							 (strFbMemInfo *) &fbMemInfo);
	}
	else if(memType == BUS_MEM_ACC) {
		status = busFbMemInfoRead  ( varInfoPtr->moduleId,
								 varInfoPtr->blockAddr, 
								 (strFbMemInfo *) &fbMemInfo);
	}
	else{
		status = MEM_ACC_TYPE_ERR;
	}

	if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
    	return(status);
	}

	if( fbMemInfo.fbId != varInfoPtr->fbId) {
		status = FB_CODE_MISMATCH_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
    	return(status);
	}

	status = fbCodeParaTypePointerGet(
								varInfoPtr->fbId,
								(strFbCodeTypePointerInfo *)&typePInfo
								);
	if(status != NO_ERROR) {
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}
	
    varTypePtr = (uint32 *)typePInfo.varTypeStartAddr;
	varTypePtr += (varInfoPtr->index - 1);

	varSizePtr = (uint32 *)typePInfo.varSizeStartAddr;
	varSizePtr += (varInfoPtr->index - 1);

	
    memoryClear ( (uint8 *) &memInfo, sizeof(strMemPointerInfo));
	if(memType == LOCAL_MEM_ACC) {
		status = fbMemPointerGet(varInfoPtr->moduleId, (strMemPointerInfo*) &memInfo);
	}
	else {
		status = busFbMemPointerGet( varInfoPtr->moduleId, (strMemPointerInfo*) &memInfo);
	}

	if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
    	return(status);
	}

    varStartPtr = (uint32 *) memInfo.paraMemPtr;
    varDataPtr = (uint32 *)(varStartPtr + fbMemInfo.varIndex + (*varSizePtr));

	
	if(GET_TYPE_SIZE(*varTypePtr)==SIZE64_TYPE) TypeSize = 2;
	else TypeSize = 1;

	
	memoryCopy (	(uint8 *) dataPtr,
 					(uint8 *)varDataPtr,
					DOUBLE_SIZE*TypeSize
				); 

	return(status);
}


uint32	fbCodeOutputDataReadWithType
				(strFbDataReadInfo *outputInfoPtr, uint32 *dataPtr, uint32 memType)
{
	uint32	status = NO_ERROR;
	strMemPointerInfo	memInfo;
	uint32	*outputStartPtr, *outputDataPtr;
	uint32  *outputSizePtr, *outputTypePtr;
	strFbMemInfo	fbMemInfo;
	uint32  TypeSize;
	strFbCodeTypePointerInfo typePInfo;


	
    memoryClear( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));

	if(memType == LOCAL_MEM_ACC) {
		status = fbMemInfoRead ( outputInfoPtr->moduleId,
								 outputInfoPtr->blockAddr, 
								 (strFbMemInfo *) &fbMemInfo);
	}
	else if(memType == BUS_MEM_ACC) {
		status = busFbMemInfoRead  ( outputInfoPtr->moduleId,
								 outputInfoPtr->blockAddr, 
								 (strFbMemInfo *) &fbMemInfo);
	}
	else{
		status = MEM_ACC_TYPE_ERR;
	}

	if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
    	return(status);
	}

	if( fbMemInfo.fbId != outputInfoPtr->fbId) {
		status = FB_CODE_MISMATCH_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
    	return(status);
	}

	status = fbCodeParaTypePointerGet( outputInfoPtr->fbId, (strFbCodeTypePointerInfo *)&typePInfo);
	if(status != NO_ERROR) return(status);

    outputTypePtr = (uint32 *)typePInfo.outputTypeStartAddr;
	outputTypePtr += (outputInfoPtr->index - 1);

	outputSizePtr = (uint32 *)typePInfo.outputSizeStartAddr;
	outputSizePtr += (outputInfoPtr->index - 1);

	
    memoryClear ( (uint8 *) &memInfo, sizeof(strMemPointerInfo));
	if(memType == LOCAL_MEM_ACC) {
		status = fbMemPointerGet(outputInfoPtr->moduleId, (strMemPointerInfo*) &memInfo);
	}
	else {
		status = busFbMemPointerGet( outputInfoPtr->moduleId, (strMemPointerInfo*) &memInfo);
	}

	if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
    	return(status);
	}

	
	outputStartPtr = (uint32 *)memInfo.outputMemPtr;
    outputDataPtr = (uint32 *)(outputStartPtr + fbMemInfo.outputIndex + (*outputSizePtr));

	
	if(GET_TYPE_SIZE(*outputTypePtr)==SIZE64_TYPE) TypeSize = 2;
	else TypeSize = 1;

	
	memoryCopy ( (uint8 *)dataPtr, (uint8 *)outputDataPtr, DOUBLE_SIZE*TypeSize); 

	return(status);
}


uint32	fbCodeOutputDataReadWithNumber
				(
			uint32	moduleId,
            uint32	fbBlockAddr,
			uint32	fbId,
			uint32  outputIndex,
			uint32  outputNumber,

			uint32  *outputByte,
            uint32	*dataPtr
				)
{
	uint32	i,status = NO_ERROR;
	strMemPointerInfo	memInfo;
	uint32	*outputStartPtr, *outputDataPtr;
	uint32  *outputSizePtr, *outputTypePtr;
	strFbMemInfo	fbMemInfo;
	uint32  TypeSize;
	strFbCodeTypePointerInfo typePInfo;

	
    memoryClear( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
	if( (status = fbMemInfoRead (	moduleId,
    								fbBlockAddr,
                                    (strFbMemInfo *) &fbMemInfo
								)
		) != NO_ERROR
	) {
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
    	return(status);
	}

	if( fbMemInfo.fbId != fbId){
		status = FB_CODE_MISMATCH_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	status = fbCodeParaTypePointerGet(
								fbId,
								(strFbCodeTypePointerInfo *)&typePInfo
								);
	if(status != NO_ERROR) {
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	
    memoryClear ( (uint8 *) &memInfo, sizeof(strMemPointerInfo));
	if( (status = fbMemPointerGet
						(
						moduleId,
                		(strMemPointerInfo	*) &memInfo
                		)
		) != NO_ERROR
	){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
    	return(status);
	}
	outputStartPtr = (uint32 *)memInfo.outputMemPtr;
	*outputByte = 0;

	
	if( (outputIndex<1) || ( (outputIndex-1 + outputNumber) > fbMemInfo.outputNo) ) {
		status = FB_CODE_OUTPUT_INDEX_ERR;
		return(status);
	}

	
	for(i=(outputIndex-1);i<(outputIndex-1)+outputNumber;i++)
	{
	    outputTypePtr = (uint32 *)typePInfo.outputTypeStartAddr;
		outputSizePtr = (uint32 *)typePInfo.outputSizeStartAddr;

		outputTypePtr += i;
		outputSizePtr += i;
		
		
		outputDataPtr = (uint32 *)(outputStartPtr + fbMemInfo.outputIndex  + (*outputSizePtr));

		
		if(GET_TYPE_SIZE(*outputTypePtr)==SIZE64_TYPE) TypeSize = 2;
		else TypeSize = 1;

		
		memoryCopy ( 	(uint8 *)dataPtr,
						(uint8 *)outputDataPtr,
						DOUBLE_SIZE*TypeSize
					); 
		dataPtr += TypeSize;
		*outputByte += DOUBLE_SIZE*TypeSize;

	}
	return(status);
}


uint32	fbCodeSpecDataReadWithNumber
					(
				uint32	moduleId,
				uint32	fbAddr,
				uint32	fbId,
				uint32  specIndex,
				uint32  specNumber,

				uint32  *specByte,
                uint32	*retRefSpecDataPtr,
                uint32	*retSpecDataPtr
                	)
{
	uint32	status = NO_ERROR;
    uint32	*specStartPtr, *specDataPtr;
	strFbMemInfo	fbMemInfo;
	strMemPointerInfo	memP;
	strFbSpecActionInfo *specActionDataPtr;
	strFbCodeTypePointerInfo typePInfo;
	uint32 i, TypeSize, *specTypePtr,*specSizePtr ;

    memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
    memoryClear ( (uint8 *) &memP, sizeof(strMemPointerInfo));
	memoryClear ( (uint8 *) &memP, sizeof(strMemPointerInfo));

     
	status = fbMemInfoRead	(
								moduleId,
            					fbAddr,
            					(strFbMemInfo	*) &fbMemInfo
            				);
	if(status != NO_ERROR) {
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

    
    if( fbId != fbMemInfo.fbId) {
    	status = FB_CODE_MISMATCH_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
        goto MODULE_END;
	}

	status = fbMemPointerGet	(
									moduleId,
               						(strMemPointerInfo	*)&memP
			                	);

	if(status != NO_ERROR) {
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

	specStartPtr = (uint32 *)memP.paraMemPtr;

	specDataPtr = (uint32 *)(specStartPtr + fbMemInfo.specIndex);
	

	status = fbCodeParaTypePointerGet(
								fbId,
								(strFbCodeTypePointerInfo *)&typePInfo
								);
	if(status != NO_ERROR) {
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}
	
	if( (specIndex<1) || ( (specIndex-1 + specNumber) > fbMemInfo.inputNo) ) {
		status = FB_CODE_SPEC_INDEX_ERR;
		return(status);
	}

	*specByte = 0;
	
	for(i = (specIndex-1); i < (specIndex-1)+specNumber; i++) 
	{
		specTypePtr = (uint32 *)typePInfo.specTypeStartAddr;
		specSizePtr = (uint32 *)typePInfo.specSizeStartAddr;
		
		specTypePtr+= i;
		specSizePtr+= i;

		
		specActionDataPtr = (strFbSpecActionInfo *)(specStartPtr + fbMemInfo.specActionIndex + i );
		specDataPtr = (uint32 *)(specStartPtr + fbMemInfo.specIndex  + (*specSizePtr));

		
		if(GET_TYPE_SIZE(*specTypePtr)==SIZE64_TYPE) TypeSize = 2;
		else TypeSize = 1;

		
		memoryCopy ( 	(uint8 *)retRefSpecDataPtr,
						(uint8 *)specDataPtr,
						DOUBLE_SIZE*TypeSize
					); 
		retRefSpecDataPtr += TypeSize;
		*specByte += DOUBLE_SIZE*TypeSize;

		
			
		if((moduleId==LOGIC_ID) &&(specActionDataPtr->refType==SPEC_TYPE_BYREF)) {

			
			if(GET_BASIC_SIZE(specActionDataPtr->dataType) == SIZE64_TYPE)
				specDataPtr++;

			
			if(CHK_IS_ANY_TYPE(*specTypePtr)) {
				status =  refDataCopyFromMem(	retSpecDataPtr, 
												*specDataPtr, 
												*specActionDataPtr,
												
												((fbMemInfo.paraRunType)&0xf00f)|((*specTypePtr)&0xffff0ff0),
												0
											);
				if(status)
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			} else {
				status = refDataCopyFromMem	(	retSpecDataPtr,
												*specDataPtr,
												*specActionDataPtr,
												*specTypePtr,
												0
											);
				if(status)
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			}

		}

		
		else
			memoryCopy ((uint8*) (retSpecDataPtr), (uint8*)specDataPtr,DOUBLE_SIZE *TypeSize);

		
		if( (moduleId==LOGIC_ID)&&CHK_IS_BIT_TYPE(*specTypePtr) &&(specActionDataPtr->negated ==0x1) ) {
			status= specDataNegated(specActionDataPtr, retSpecDataPtr);
			if(status) 
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		}

		retSpecDataPtr++;
		if(GET_TYPE_SIZE(*specTypePtr)==SIZE64_TYPE) {
			if( (moduleId==LOGIC_ID)&& CHK_IS_BIT_TYPE(*specTypePtr) &&(specActionDataPtr->negated ==0x1) ) {
				status= specDataNegated(specActionDataPtr, retSpecDataPtr);
				if(status) 
					setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			}
			retSpecDataPtr++;
		}

	}
MODULE_END:
    return(status);
}


uint32 fbInOutputTypeCasting
			(
		uint32 moduleId, 
		uint32 fbBlockAddr, 
		uint32 specSindex, 
		uint32 *paraRunTypePtr
			)
{
	uint32 status = NO_ERROR;
	strFbMemInfo	*fbMemInfoPtr;
	uint32	*specActionStartPtr;
	strFbSpecActionInfo *specActionDataPtr;
	
    if(moduleId == LOGIC_ID) {
		fbMemInfoPtr = gMpuFbMemInfoLocalPtr;
	}
	else {
		return(L_MODULE_ID_ERR);
	}
 
   	
   	fbMemInfoPtr += (fbBlockAddr - 1);

	specActionStartPtr 	= (uint32 *)gMpuSpecAndVarDataLocalPtr;
    specActionStartPtr	+= fbMemInfoPtr->specActionIndex;
	specActionDataPtr	= (strFbSpecActionInfo *)specActionStartPtr;
	specActionDataPtr	+= (specSindex - 1);

#ifdef TYPE_DECISION_BY_RCS
	*paraRunTypePtr = specActionDataPtr->dataType;
	fbMemInfoPtr->paraRunType = specActionDataPtr->dataType;
#else
	
	if( (fbMemInfoPtr->fbId == WRVAR_CODE)
		&& (GET_BASIC_DATA_TYPE(fbMemInfoPtr->paraRunType) ==  0)
	) {
		*paraRunTypePtr = specActionDataPtr->dataType;
		fbMemInfoPtr->paraRunType = specActionDataPtr->dataType;
	} 
	
	
	if(GET_BASIC_DATA_TYPE(fbMemInfoPtr->paraRunType)==TIME32_TYPE)
		*paraRunTypePtr = SIZE32_TYPE|UINT_TYPE;
	else 
		*paraRunTypePtr = fbMemInfoPtr->paraRunType;
#endif

	return(status);
}


uint32 fbCodeMemPointerGet
				(
			uint32	moduleId,
            uint32	fbBlockAddr,
            strFbMemInfo	**fbMemPtr
            	)
{
	uint32	status = NO_ERROR;

    switch(moduleId) 
	{
	    case LOGIC_ID:
			*fbMemPtr  = gMpuFbMemInfoLocalPtr;
        break;

		default:
			return(L_MODULE_ID_ERR);
		break;
	}
 
   	
   	*fbMemPtr += (fbBlockAddr-1);
 	
    return(status);
}



uint32 busFbCodeMemPointerGet
				(
			uint32	moduleId,
            uint32	fbBlockAddr,
            strFbMemInfo	**fbMemPtr
            	)
{
	uint32	status = NO_ERROR;

    switch(moduleId) 
	{
	    case LOGIC_ID:
			*fbMemPtr  = gMpuFbMemInfoBusPtr;
        break;

		default:
			return(L_MODULE_ID_ERR);
		break;
	}
 
   	
   	*fbMemPtr += (fbBlockAddr-1);
 	
    return(status);
}



uint32 fbSysExeModuleInfoRead(uint32 moduleID, strModuleExecInfo* sysExeInfo)
{
	uint32 status = NO_ERROR;
    strModuleInfo	moduleInfo;

	switch(moduleID){
		case LOGIC_ID:
			memoryCopy ((uint8*)sysExeInfo, 
						(uint8*)&gSysExeInfoLocalPtr->mpuSysExeInfo, 
						sizeof(strModuleExecInfo) 
					   ); 
		break;

		default:
			status = L_MODULE_ID_ERR ;
		break;

	}

	if(status == NO_ERROR)
		status = moduleInfoRead(moduleID, &moduleInfo);
	
	if(status == NO_ERROR)
	{
		if(sysExeInfo->curfbMaxBlockAddr > moduleInfo.maxFbBlockAddr)
			status = L_FB_ADDR_ERR;
			
		else if(sysExeInfo->curfbMaxOutIndex > moduleInfo.maxFbOutputNum)
			status = OUTPUT_SIZE_EXCEED_ERR;
			
		else if(sysExeInfo->curfbMaxParaIndex > moduleInfo.maxFbParaNum)
			status = PARA_SIZE_EXCEED_ERR;
	}
	
	return(status);
}



uint32 fbSysExeModuleInfoWrite(uint32 moduleID, strModuleExecInfo* sysExeInfo)
{
	uint32 status = NO_ERROR;

	switch(moduleID){
		case LOGIC_ID:
			if( (sysExeInfo->curMaxSegmentId > MAX_LOGIC_TASK_NUM) ||
				(sysExeInfo->curfbMaxBlockAddr > MAX_MPU_BLOCK_ADDR) ||
				(sysExeInfo->curfbMaxParaIndex > MAX_MPU_SPEC_NUM) ||
				(sysExeInfo->curfbMaxOutIndex > MAX_MPU_OUTPUT_NUM) ){
				status = SYS_MAX_EXCCEED_ERR ;
		
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
						(uint8 *)"curMaxSegmentId", sysExeInfo->curMaxSegmentId,
						(uint8 *)"curfbMaxBlockAddr", sysExeInfo->curfbMaxBlockAddr,
						(uint8 *)"curfbMaxParaIndex",  sysExeInfo->curfbMaxParaIndex
	  			);
        
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
						(uint8 *)"curfbMaxOutIndex", sysExeInfo->curfbMaxOutIndex,
						(uint8 *)"MAX_LOGIC_TASK_NUM", MAX_LOGIC_TASK_NUM,
						(uint8 *)"MAX_MPU_BLOCK_ADDR",  MAX_MPU_BLOCK_ADDR
	  			);
	  			
   				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
						(uint8 *)"MAX_MPU_SPEC_NUM", MAX_MPU_SPEC_NUM,
						(uint8 *)"MAX_MPU_OUTPUT_NUM", MAX_MPU_OUTPUT_NUM,
						(uint8 *)"Not Using",  0
	  			);	  						
			}
			else {
				memoryCopy ((uint8*)&gSysExeInfoLocalPtr->mpuSysExeInfo, (uint8*)sysExeInfo, 
						sizeof(strModuleExecInfo) );
				memoryCopy ((uint8*)&gSysExeInfoBusPtr->mpuSysExeInfo, (uint8*)sysExeInfo, 
						sizeof(strModuleExecInfo) );
			}
		break;

		default:
			status = L_MODULE_ID_ERR ;
		break;

	}

	return(status);
}


uint32 fbSysExeVarInfoRead(uint32 variableID, strVariableExecInfo* sysVarExeInfo)
{
	uint32 status = NO_ERROR;

	switch(variableID){
		case AUTO_VAR:
			memoryCopy ((uint8*)sysVarExeInfo, (uint8*)&gSysExeInfoLocalPtr->autovarSysExeInfo, 
				sizeof(strVariableExecInfo) ); 
		break;
		case MMEM_VAR:
			memoryCopy ((uint8*)sysVarExeInfo, (uint8*)&gSysExeInfoLocalPtr->flagvarSysExeInfo, 
				sizeof(strVariableExecInfo) ); 
		break;
		case SMEM_VAR:
			
		break;
		default:
			status = VARIABLE_KIND_ERR ;
		break;

	}
	
	return(status);
}



uint32 fbSysExeVarInfoWrite(uint32 variableID, strVariableExecInfo* sysVarExeInfo)
{
	uint32 status = NO_ERROR;

	switch(variableID){
		case AUTO_VAR:
			if( (sysVarExeInfo->curMaxVarAddr > MAX_MPU_AUTOVAR_NUM) ||
				(sysVarExeInfo->curMaxVarInitNum > MAX_MPU_AUTOVAR_NUM) ||
				(sysVarExeInfo->curMaxVarRetainInfo > MAX_MPU_RETAIN_NUM) ||
				(sysVarExeInfo->curMaxVarRetainNum > MAX_MPU_RETAIN_NUM) )
				status = VARIABLE_ADDR_RANGE_ERR;
			else {
				memoryCopy ((uint8*)&gSysExeInfoLocalPtr->autovarSysExeInfo, (uint8*)sysVarExeInfo, 
						sizeof(strVariableExecInfo) );
				memoryCopy ((uint8*)&gSysExeInfoBusPtr->autovarSysExeInfo, (uint8*)sysVarExeInfo, 
						sizeof(strVariableExecInfo) );
			}
		break;
		case MMEM_VAR:
			if( (sysVarExeInfo->curMaxVarAddr > MAX_MPU_MMEM_NUM) ||
				(sysVarExeInfo->curMaxVarInitNum > MAX_MPU_MMEM_NUM) ||
				(sysVarExeInfo->curMaxVarRetainInfo > MAX_MPU_RETAIN_NUM) ||
				(sysVarExeInfo->curMaxVarRetainNum > MAX_MPU_RETAIN_NUM) )
				status = VARIABLE_ADDR_RANGE_ERR;
			else {
				memoryCopy ((uint8*)&gSysExeInfoLocalPtr->flagvarSysExeInfo, (uint8*)sysVarExeInfo, 
						sizeof(strVariableExecInfo) );
				memoryCopy ((uint8*)&gSysExeInfoBusPtr->flagvarSysExeInfo, (uint8*)sysVarExeInfo, 
						sizeof(strVariableExecInfo) );
			}
		break;
		case SMEM_VAR:
			
		break;

		default:
			status = VARIABLE_KIND_ERR ;
		break;

	}

	return(status);
}



uint32 fbSysExeSfcInfoRead(uint32 sfcType, uint32* typeId)
{
	uint32 status = NO_ERROR;

	switch(sfcType){
		case STEP_TYPE:
			memoryCopy ((uint8*)typeId, (uint8*)&gSysExeInfoLocalPtr->sfcSysExeInfo.curMaxStepId, 
				DOUBLE_SIZE ); 
			if((*typeId<0) || (*typeId>MAX_STEP_NUM))  status = L_STEP_ID_ERR;
		break;
		case TRANSITION_TYPE:
			memoryCopy ((uint8*)typeId, (uint8*)&gSysExeInfoLocalPtr->sfcSysExeInfo.curMaxTrsId, 
				DOUBLE_SIZE ); 
			if((*typeId<0) || (*typeId>MAX_TRANSITOIN_NUM))  status = L_TRANSITION_ID_ERR;
		break;
		case ACTION_TYPE:
			memoryCopy ((uint8*)typeId, (uint8*)&gSysExeInfoLocalPtr->sfcSysExeInfo.curMaxActId, 
				DOUBLE_SIZE); 
			if((*typeId<0) || (*typeId>MAX_ACTION_NUM))  status = L_ACTION_ID_ERR;
		break;

		default:
			status = SFC_TYPE_ID_ERR ;
		break;

	}
	if(status != NO_ERROR) *typeId = 0;
	
	return(status);
}


uint32 fbSysExeSfcInfoWrite(uint32 sfcType, uint32 typeId)
{
	uint32 status = NO_ERROR;

	switch(sfcType){
		case STEP_TYPE:
			if(typeId<1 || typeId>MAX_STEP_NUM)  status = L_STEP_ID_ERR;
			else {
				memoryCopy ( (uint8*)&gSysExeInfoLocalPtr->sfcSysExeInfo.curMaxStepId, (uint8*)&typeId,
					DOUBLE_SIZE ); 
				memoryCopy ( (uint8*)&gSysExeInfoBusPtr->sfcSysExeInfo.curMaxStepId, (uint8*)&typeId,
					DOUBLE_SIZE ); 
			}
		break;
		case TRANSITION_TYPE:
			if(typeId<1 || typeId>MAX_TRANSITOIN_NUM)  status = L_TRANSITION_ID_ERR;
			else {
				memoryCopy ( (uint8*)&gSysExeInfoLocalPtr->sfcSysExeInfo.curMaxTrsId, (uint8*)&typeId,
					DOUBLE_SIZE ); 
				memoryCopy ( (uint8*)&gSysExeInfoBusPtr->sfcSysExeInfo.curMaxTrsId, (uint8*)&typeId,
					DOUBLE_SIZE ); 
			}
		break;
		case ACTION_TYPE:
			if(typeId<1 || typeId>MAX_ACTION_NUM)  status = L_ACTION_ID_ERR;
			else {
				memoryCopy ( (uint8*)&gSysExeInfoLocalPtr->sfcSysExeInfo.curMaxActId, (uint8*)&typeId,
					DOUBLE_SIZE); 
				memoryCopy ( (uint8*)&gSysExeInfoBusPtr->sfcSysExeInfo.curMaxActId, (uint8*)&typeId,
					DOUBLE_SIZE ); 
			}

		break;

		default:
			status = SFC_TYPE_ID_ERR ;
		break;

	}

	return(status);
}


uint32 fbSysMaxJmpLoopInfo(uint32 maxCnt)
{
	uint32 jmpCnt = MAX_JUMP_LOOP_CNT;
	
	if((gSysExeInfoLocalPtr != NULL) && (gSysExeInfoBusPtr != NULL)) {

		if(gSysExeInfoLocalPtr->maxJumpLoopCnt < MAX_JUMP_LOOP_CNT ) {
			gSysExeInfoLocalPtr->maxJumpLoopCnt = MAX_JUMP_LOOP_CNT;
			gSysExeInfoBusPtr->maxJumpLoopCnt = MAX_JUMP_LOOP_CNT;
		}	
		else if( gSysExeInfoBusPtr->maxJumpLoopCnt < maxCnt ) {
			gSysExeInfoBusPtr->maxJumpLoopCnt = maxCnt;
		}
		
		jmpCnt = gSysExeInfoLocalPtr->maxJumpLoopCnt;
	}
	return(jmpCnt);
}


uint32 fbSysExeInfoPointerGet(strSysExecInfo** sysExeInfo,uint32 memType)
{
	uint32 status = NO_ERROR;

	strSysConfigInfo sysCnfgInfo;
	uint32 offSet = 0,bitMask;
	uint32 tempPtr;

	bitMask = 0x00ffffff;

	if(memType==LOCAL_MEM_ACC)
		*sysExeInfo = gSysExeInfoLocalPtr; 
	else if(memType==BUS_MEM_ACC)
		*sysExeInfo = gSysExeInfoBusPtr; 
	else if(memType==RMT_MEM_ACC) {
		status = systemCnfgInfoRead(&sysCnfgInfo);
		if(status != NO_ERROR){
			return(status);
		}

		if(sysCnfgInfo.shelfType == MASTER_SHELF) 
       		offSet = MEM_BOARD_BUS_ADDR_ON_SLAVE;
		else if(sysCnfgInfo.shelfType == SLAVE_SHELF) 
       		offSet = MEM_BOARD_BUS_ADDR_ON_MASTER;
		else {
			status = STATION_SHELF_CONFIG_ERR;
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}
		
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
		tempPtr = (uint32)gSysExeInfoBusPtr;
#elif (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
		tempPtr = (uint32)gSysExeInfoBusPtr - (uint32)gBusMpuFbStartAddr; 
#endif
		tempPtr &=  bitMask;
		tempPtr += offSet;
		*sysExeInfo = (strSysExecInfo*)tempPtr;
	}
	else status = MEM_ACC_TYPE_ERR;

	return(status);
}


uint32	fbCodeSpecActionRead
	(strFbDataReadInfo *inputInfoPtr, strFbSpecActionInfo *retSpecActionPtr, uint32 memType)
{
	uint32	status = NO_ERROR;
    uint32	*specStartPtr, *specDataPtr;
	strFbMemInfo	fbMemInfo;
	strMemPointerInfo	memP;

	if(inputInfoPtr->moduleId != LOGIC_ID) return(L_MODULE_ID_ERR);

    memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
    memoryClear ( (uint8 *) retSpecActionPtr,sizeof(strFbSpecActionInfo) );

	 
	if(memType == LOCAL_MEM_ACC) {
		status = fbMemInfoRead ( inputInfoPtr->moduleId,
							 inputInfoPtr->blockAddr, 
							 (strFbMemInfo *) &fbMemInfo);
	}
	else if(memType == BUS_MEM_ACC) {
		status = busFbMemInfoRead  ( inputInfoPtr->moduleId,
								 inputInfoPtr->blockAddr, 
								 (strFbMemInfo *) &fbMemInfo);
	}
	else{
		status = MEM_ACC_TYPE_ERR;
	}

	if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

    
    if( inputInfoPtr->fbId != fbMemInfo.fbId) {
    	status = FB_CODE_MISMATCH_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"FB Addr", inputInfoPtr->blockAddr,
							"Read FB Code",inputInfoPtr->fbId,
							"Set FB Code",fbMemInfo.fbId);
        goto MODULE_END;
	}

    
    if(inputInfoPtr->index > fbMemInfo.inputNo) {
    	status = FB_CODE_SPEC_INDEX_ERR; 
        goto MODULE_END;
	}

	
    memoryClear ( (uint8 *) &memP, sizeof(strMemPointerInfo));
	if(memType == LOCAL_MEM_ACC) {
		status = fbMemPointerGet(inputInfoPtr->moduleId, (strMemPointerInfo*) &memP);
	}
	else {
		status = busFbMemPointerGet( inputInfoPtr->moduleId, (strMemPointerInfo*) &memP);
	}

	if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

	specStartPtr = (uint32 *)memP.paraMemPtr;

    
	specDataPtr = (uint32 *)(specStartPtr + fbMemInfo.specActionIndex);
    specDataPtr += inputInfoPtr->index - 1;

    
    memoryCopy ( (uint8 *) retSpecActionPtr,
    			 (uint8 *) specDataPtr,
                 DOUBLE_SIZE
				);
MODULE_END:
    return(status);
}

uint32 fbDataWriteToRemoteVmeMemBoard(	uint32 	moduleId,
									uint32 	fbAddr, 
									uint32 	writeIndex,
									uint32	writeData,
									uint32	writeType, 
									strFbMemInfo	*fbMemInfoPtr
								)
{
	uint32	status = NO_ERROR;
    uint32	*srcPtr, *destPtr;
	strMemPointerInfo	rmtbusMemAddrInfo;
	strSysRunningConfigInfo			*sysRunningInfoPtr = gSysRunningInfoPtr;
	strMpuDiuRunningStatusInfo		*diuRunningInfoPtr = gDiuRasInfoPtr;

	
	
	if( (sysRunningInfoPtr->sysRedundancyStatus != SYS_RAS_REDUNDANCY_RUN) ||
		(sysRunningInfoPtr->sysOperationMode 	!= SYS_RAS_SYSTEM_AUTO_MODE) ||
		(diuRunningInfoPtr->redundancyStatus 	!= NO_ERROR)
	){
		return(status);
	}

 	status = rmtbusFbMemPointerGet(moduleId, &rmtbusMemAddrInfo);
 	
	if(status != NO_ERROR) return(status);

	srcPtr = &writeData;
	
	switch(writeType) {
		case DUAL_SPEC_WRITE:
			destPtr = (uint32 *)rmtbusMemAddrInfo.paraMemPtr;

   			
			destPtr = (uint32 *)(destPtr + fbMemInfoPtr->specIndex);
   			destPtr += (writeIndex - 1);
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
			status = myBusDataAcc(	__FILE__,
									__LINE__,	
									BUS_LONG_ACC, 
									BUS_DATA_WRITE ,
									(uint8 *)destPtr, 
									(uint8 *)srcPtr
							 	);
#else
			status = myRmtDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
									BUS_DATA_WRITE ,
									(uint8 *)destPtr, 
									(uint8 *)srcPtr
							 	);
#endif
			if(status)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		break;
		case DUAL_VAR_WRITE:
			destPtr = (uint32 *)rmtbusMemAddrInfo.paraMemPtr;

			
			destPtr = (uint32 *)(destPtr + fbMemInfoPtr->varIndex);
			destPtr += writeIndex - 1;
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
			status = myBusDataAcc(	__FILE__,
									__LINE__,	
									BUS_LONG_ACC, 
									BUS_DATA_WRITE ,
									(uint8 *)destPtr, 
									(uint8 *)srcPtr
								 );
#else
			status = myRmtDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
									BUS_DATA_WRITE ,
									(uint8 *)destPtr, 
									(uint8 *)srcPtr
								 );
#endif
			if(status)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		break;
		case DUAL_OUTPUT_WRITE:
			
			destPtr = (uint32 *)rmtbusMemAddrInfo.outputMemPtr;
		
			
			destPtr = (uint32 *)(destPtr + fbMemInfoPtr->outputIndex);
			destPtr += writeIndex - 1;
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
			status = myBusDataAcc(	__FILE__,
									__LINE__,	
									BUS_LONG_ACC, 
									BUS_DATA_WRITE ,
									(uint8 *)destPtr, 
									(uint8 *)srcPtr
								 );
#else
			status = myRmtDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
									BUS_DATA_WRITE ,
									(uint8 *)destPtr, 
									(uint8 *)srcPtr
								 );
#endif
			if(status)
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		break;
		default:
			status = SYS_DUAL_CMD_ERR;
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		break;
	}
		
	return(status);
}


static uint32 fbDataWriteToRemoteLocalMemory(	uint32 	moduleId,
												uint32 	fbAddr, 
												uint32 	writeIndex,
												uint32	writeData,
												uint32	writeSpecType, 
												strFbMemInfo	*fbMemInfoPtr
											)
{
	uint32	status = NO_ERROR;
	strSysRunningConfigInfo			*sysRunningInfoPtr = gSysRunningInfoPtr;
	strMpuDiuRunningStatusInfo		*diuRunningInfoPtr = gDiuRasInfoPtr;

	if( (sysRunningInfoPtr->sysRedundancyStatus != SYS_RAS_REDUNDANCY_RUN) ||
		(sysRunningInfoPtr->sysOperationMode 	!= SYS_RAS_SYSTEM_AUTO_MODE) ||
		(diuRunningInfoPtr->redundancyStatus 	!= NO_ERROR)
	){
		return(status);
	}

	if ((fbMemInfoPtr->fbId < 1) || (fbMemInfoPtr->fbId > MAX_FB_CODE_ID)){
        status = L_FB_CODE_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Block Addr", fbAddr,
							"Fb Code Id", fbMemInfoPtr->fbId,
							"Not Using", 0
  						);
		return(status);					
	}
	
	
	return(status);
}


uint32 forcingDataWriteToRemoteShelf(	uint32 	moduleId,
									uint32 	fbAddr, 
									uint32	writeData
								)
{
	uint32	status = NO_ERROR;
    uint32	*srcPtr, *destPtr;
	strFbMemInfo *fbMemInfoPtr;
	strMemPointerInfo	rmtbusMemAddrInfo;
	strSysRunningConfigInfo			*sysRunningInfoPtr = gSysRunningInfoPtr;
	strMpuDiuRunningStatusInfo		*diuRunningInfoPtr = gDiuRasInfoPtr;

	
	
	if( (sysRunningInfoPtr->sysRedundancyStatus != SYS_RAS_REDUNDANCY_RUN) ||
		(sysRunningInfoPtr->sysOperationMode 	!= SYS_RAS_SYSTEM_AUTO_MODE) ||
		(diuRunningInfoPtr->redundancyStatus 	!= NO_ERROR)
	){
		return(status);
	}

 	status = rmtbusFbMemPointerGet	(moduleId, &rmtbusMemAddrInfo);
 	
	if(status != NO_ERROR) return(status);

	srcPtr = &writeData;
	
	fbMemInfoPtr = (strFbMemInfo *)rmtbusMemAddrInfo.fbMemPtr;
	fbMemInfoPtr = (strFbMemInfo *)(fbMemInfoPtr + fbAddr - 1);

	destPtr = &fbMemInfoPtr->forcingFlag;

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
		sAccumSimData.accumPtr[sAccumSimData.accDataNum]= (uint32*)destPtr;
		sAccumSimData.accumData[sAccumSimData.accDataNum]= (uint32)writeData;
		sAccumSimData.accDataNum++;

		if(sAccumSimData.accDataNum >= MAX_DUAL_COPY_DATA_NUM)
			status = myRmtRunDataWrite((strAccumBusInfo*)&sAccumSimData);
#elif (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
	status = myBusDataAcc(	__FILE__,
							__LINE__,	
							BUS_LONG_ACC, 
							BUS_DATA_WRITE ,
							(uint8 *)destPtr, 
							(uint8 *)srcPtr
						);
#endif
	if(status)
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
	
	return(status);
}



uint32 specDataNegated(strFbSpecActionInfo* specActionDataPtr, uint32* tmpPtr)
{
	uint32 status = NO_ERROR;
	
	if(specActionDataPtr->negated == TRUE)
	{
		if( (specActionDataPtr->dataSize == SPEC_SIZE_BIT)||
			(specActionDataPtr->bitposFlag == TRUE) )
		{
			*tmpPtr ^= 0x1;
		}
		else if(specActionDataPtr->dataSize == SPEC_SIZE_BYTE ) {
			*tmpPtr ^= 0xff;
		}
		else if(specActionDataPtr->dataSize == SPEC_SIZE_WORD ) {
			*tmpPtr ^= 0xffff;
		}
		else if(specActionDataPtr->dataSize == SPEC_SIZE_DWORD ) {
			*tmpPtr ^= 0xffffffff;
		}
		
		else if(specActionDataPtr->dataSize == SPEC_SIZE_LWORD ) {
			*tmpPtr ^= 0xffffffff;
		}
	}
	
	return(status);
}


uint32 jmpFbCodeCheck(uint32 fbId)
{
	uint32 status = NO_ERROR;
	if( (fbId < 1) || (fbId > MAX_FB_CODE_ID) ){												
		status = L_FB_CODE_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
            		   	    "FB Code",fbId,
             		  	    "Max FB Code ", MAX_FB_CODE_ID,
              	 		    "Not Using", 0 
			  		  	);	
		return(0);
	}

#if 0
	컴파일 에러 때문에 comment
	if(	(fbId == JMP_CODE)||(fbId == JMPC_CODE)||
		(fbId == RET_CODE)||(fbId == RETC_CODE) ) 
		return(IS_JUMP_CODE);
	
	else return(0);
#endif

	return(0);
}


uint32 jumpCountVarCheck(uint32 fbAddr, uint32 taskId, uint32 *jmpErr)
{
	uint32 status = NO_ERROR;
	strFbMemInfo *fbMemInfoPtr;
	strMemPointerInfo memInfo;
	uint32 *varStartPtr; 
	strFbDataReadInfo readFbInputData;

	if( (fbAddr < 1) || (fbAddr > MAX_MPU_BLOCK_ADDR) ){												
		status = L_FB_ADDR_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
            		   	    "FB Addr",fbAddr,
             		  	    "Max FB Addr", MAX_MPU_BLOCK_ADDR,
              	 		    "TASK ID", taskId 
			  		  	);	
		return(status);
	}

	if( (taskId < 1) || (taskId > MAX_LOGIC_TASK_NUM) ){												
		status = L_SEG_ID_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
            		   	    "TASK ID",taskId,
             		  	    "Max TASK NUM", MAX_LOGIC_TASK_NUM,
              	 		    "NOT USING", 0
			  		  	);	
		return(status);
	}

	
	if( (status = fbCodeMemPointerGet	(LOGIC_ID, fbAddr,(strFbMemInfo **) &fbMemInfoPtr) ) 
		!= NO_ERROR)
	{
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
						  "FB Addr",fbAddr,
						  "TASK ID",taskId,
						  "NOT USING",0
						  );
		return(status);
	}

	if( jmpFbCodeCheck(fbMemInfoPtr->fbId) == IS_JUMP_CODE) {
		
		
		if( (status = fbMemPointerGet
							( LOGIC_ID, (strMemPointerInfo	*) &memInfo)
			) != NO_ERROR
		) {
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
    		return(status);
		}

	    varStartPtr = (uint32 *)memInfo.paraMemPtr;
		
		varStartPtr = (uint32 *)(varStartPtr + fbMemInfoPtr->varIndex);

		 
		readFbInputData.moduleId = LOGIC_ID;
		readFbInputData.blockAddr =fbMemInfoPtr->fbFuncAddr;
		readFbInputData.fbId = fbMemInfoPtr->fbId;
		readFbInputData.index = 2; 
		
		status = fbCodeOutputDataReadWithType( (strFbDataReadInfo*)&readFbInputData, 
											 (uint32*)jmpErr, LOCAL_MEM_ACC);

		if(status != NO_ERROR)
		{
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		}
		
		if( *jmpErr!=0) 
			memoryClear( (uint8 *)varStartPtr, DOUBLE_SIZE ); 

	}
	
	return(status);

}


uint32 jmpFbAddressCheck(uint32 fbId, uint32 fbAddr, strFbMemInfo* jmpFbMemInfo)
{
	uint32 status = NO_ERROR;
	strFbDataReadInfo readFbInputData;
	strFbMemInfo curFbMemInfo;
	uint32 jmpAddr = 0;

#if 0	
	컴파일 에러 때문에 Comment
	if(	(fbId != JMP_CODE)&&(fbId != JMPC_CODE)&&
		(fbId != RET_CODE)&&(fbId != RETC_CODE) ) {
		status = JUMP_FB_CODE_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							  "FB CODE",fbId,
							  "FB ADDR",fbAddr,
							  "NOT USING",0
							  );
		return(status);
	}
#endif
	
	memoryClear( (uint8 *)&curFbMemInfo, sizeof(strFbMemInfo));
	if( (status = fbCodeMemInfoRead	(LOGIC_ID, fbAddr,(strFbMemInfo *) &curFbMemInfo) ) 
		!= NO_ERROR)
	{
		status = JUMP_FB_CODE_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							  "Jump FB Addr",curFbMemInfo.fbFuncAddr,
							  "TASK ID",curFbMemInfo.fbTaskId  ,
							  "NOT USING",0
							  );
		return(status);
	}


	memoryClear((uint8*)&readFbInputData,sizeof(strFbDataReadInfo));
	readFbInputData.moduleId = LOGIC_ID;
	readFbInputData.blockAddr = fbAddr;
	readFbInputData.fbId = fbId;
	readFbInputData.index = 2; 

	
	status = fbCodeSpecDataReadWithType((strFbDataReadInfo*)&readFbInputData,
										(uint32*)&jmpAddr, LOCAL_MEM_ACC);

	if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	 
	if( (status = fbCodeMemInfoRead	(LOGIC_ID, jmpAddr,(strFbMemInfo *) jmpFbMemInfo) ) 
		!= NO_ERROR)
	{
		status = JUMP_DEST_ADDR_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							  "Jump FB Addr",curFbMemInfo.fbFuncAddr,
							  "Read FB Addr",jmpFbMemInfo->fbFuncAddr,
							  "Read FB Code",jmpFbMemInfo->fbId
							  );
		return(status);
	}
	
   if (jmpFbMemInfo->fbFuncAddr !=  jmpAddr ){
		status = JUMP_DEST_ADDR_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Task ID ",curFbMemInfo.fbTaskId,
								"Read FB Addr ",jmpFbMemInfo->fbFuncAddr,
								"Dest FB Addr",jmpAddr);
		return(status);
	}


	if( (jmpFbMemInfo->fbTaskId != curFbMemInfo.fbTaskId) ||
		(jmpFbMemInfo->fbFuncAddr == 0) || (jmpFbMemInfo->fbFuncAddr > MAX_MPU_BLOCK_ADDR) 
		) 
	{
		status = JUMP_DEST_ADDR_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Read Task ID ",jmpFbMemInfo->fbTaskId,
								"Current Task ID ",curFbMemInfo.fbTaskId,
								"Dest FB Addr",jmpAddr);
		return(status);
	}

	return(status);

}


strFbExecutionInfo* jmpFbExecIdNextMemInfoGet(uint32 IdNum, strFbExecutionInfo *dataPtr)
{
	uint32 status = NO_ERROR;
	strFbMemInfo jmpFbMemInfo, curFbMemInfo;
	strFbExecutionInfo *retFbExecIdInfoPtr = NULL, *startFbExecIdInfoPtr = NULL;
	uint32 jmpFlag = 0, jmpFbAddr = 0, jmpOutput = 0, jmpErr = 0;
	uint32 nextMemFlag = FALSE;
	strFbDataReadInfo readFbInputData;
	strFbTaskTblMemInfo  *segTblInfoPtr;

	if(dataPtr == NULL) {
		status = JUMP_FB_CODE_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		return(NULL);
	}

	status = segTblPointerGet(LOGIC_ID, dataPtr->taskId,
								(strFbTaskTblMemInfo	**) &segTblInfoPtr);
			
  	if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		return(NULL);	
	}

	
	memoryClear( (uint8 *)&curFbMemInfo, sizeof(strFbMemInfo));
	if( (status = fbCodeMemInfoRead	(LOGIC_ID, dataPtr->fbBlockAddr,(strFbMemInfo *) &curFbMemInfo) ) 
		!= NO_ERROR)
	{
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}
	
	
	if(segTblInfoPtr->logicType != curFbMemInfo.logicType ) {
		status = L_LOGIC_ID_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

	if(segTblInfoPtr->logicType == FB_LOGIC_TYPE){
		if( (IdNum != segTblInfoPtr->taskId ) ||  ( dataPtr->taskId != segTblInfoPtr->taskId) ) {
			status = L_SEG_ID_ERR;
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			goto MODULE_END;
		}
	}
	else if(segTblInfoPtr->logicType == SFC_LOGIC_TYPE ){
		if(curFbMemInfo.usingType == TRANSITION_TYPE) {
			if(curFbMemInfo.TransitionID != IdNum) {
				status = L_TRANSITION_ID_ERR;
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			}
		}
		else if(curFbMemInfo.usingType == ACTION_TYPE) {
			if(curFbMemInfo.ActionID != IdNum) {
				status = L_ACTION_ID_ERR;
				setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
			}
		}
		else {
			status = SFC_TYPE_ID_ERR;
			setErrorCode( __FILE__, __LINE__, __FUNCTION__, status);
		}

		if(status != NO_ERROR) 	goto MODULE_END;
	}

	memoryClear( (uint8 *)&readFbInputData, sizeof(strFbDataReadInfo));
	memoryClear( (uint8 *)&jmpFbMemInfo, sizeof(strFbMemInfo));

#if 0
컴파일 에러 때문에 Comment
	if( (dataPtr->fbId == JMP_CODE)||(dataPtr->fbId == JMPC_CODE)||
		  (dataPtr->fbId == RET_CODE)||(dataPtr->fbId == RETC_CODE) )
	{
		readFbInputData.moduleId = LOGIC_ID;
		readFbInputData.blockAddr = dataPtr->fbBlockAddr;
		readFbInputData.fbId = dataPtr->fbId;
		readFbInputData.index = 1; 

		 
		status = fbCodeSpecDataReadWithType( (strFbDataReadInfo*)&readFbInputData, 
											 (uint32*)&jmpFlag, LOCAL_MEM_ACC);
		if(status != NO_ERROR){
			setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
			goto MODULE_END;
		}

		 
		status = fbCodeOutputDataReadWithType( (strFbDataReadInfo*)&readFbInputData, 
											 (uint32*)&jmpOutput, LOCAL_MEM_ACC);
		if(status != NO_ERROR){
			setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
			goto MODULE_END;
		}

		
		readFbInputData.index = 2; 
		status = fbCodeOutputDataReadWithType( (strFbDataReadInfo*)&readFbInputData, 
											 (uint32*)&jmpErr, LOCAL_MEM_ACC);
		if(status != NO_ERROR){
			setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
			goto MODULE_END;
		}

		if((jmpFlag == TRUE)&&(jmpOutput == TRUE)&&(jmpErr==FALSE)) {
			 
			memoryClear( (uint8 *)&jmpFbMemInfo, sizeof(strFbMemInfo));
			status = jmpFbAddressCheck(dataPtr->fbId,dataPtr->fbBlockAddr,&jmpFbMemInfo);
			if(status == NO_ERROR) {
				nextMemFlag = TRUE;
				jmpFbAddr = jmpFbMemInfo.fbFuncAddr;
			}
			else {
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"FB ADDR",dataPtr->fbBlockAddr ,
								"FB CODE",dataPtr->fbId ,
								"TASK ID",dataPtr->taskId );
				goto MODULE_END;
			}

		}
		
		else if((jmpFlag == TRUE)&&(jmpOutput == TRUE)&&(jmpErr==TRUE)) {
			return(NULL);
		}
		else { 	
			switch(curFbMemInfo.usingType)	{
				case FB_LOGIC_TYPE:
					return(fbExecIdNextMemInfoGet(IdNum, dataPtr));
				break;
				case TRANSITION_TYPE:
					return(trsExecIdNextMemInfoGet(IdNum, dataPtr));
				break;
				case ACTION_TYPE:
					return(actExecIdNextMemInfoGet(IdNum, dataPtr));
				break;
				default:
					status = L_LOGIC_ID_ERR;
					setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
					return(NULL);
				break;
			}
		}
	}

	
	else {
 		status = JUMP_FB_CODE_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"FB ADDR",dataPtr->fbBlockAddr ,
								"FB CODE",dataPtr->fbId ,
								"TASK ID",dataPtr->taskId );
		return(NULL);
	}
#endif
	
	switch(curFbMemInfo.usingType)
	{
		case FB_LOGIC_TYPE:
			if(IdNum==0 || IdNum > MAX_LOGIC_TASK_NUM){
				status = L_SEG_ID_ERR;
				setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
				goto MODULE_END;
			}
			if( (nextMemFlag == TRUE) && (jmpFbMemInfo.fbTaskId != 0) ){
				startFbExecIdInfoPtr = fbExecIdStartMemInfoGet(jmpFbMemInfo.fbTaskId);
				retFbExecIdInfoPtr = gFBExeSequenceQueuePtr[jmpFbAddr-1];
		
				if( (startFbExecIdInfoPtr == retFbExecIdInfoPtr) ||
					(startFbExecIdInfoPtr == NULL) )
				{
					 retFbExecIdInfoPtr = NULL;
				}

			}
			else retFbExecIdInfoPtr = NULL;
		break;
		
		case TRANSITION_TYPE:
			if(IdNum==0 || IdNum > MAX_TRANSITOIN_NUM){
				status = L_TRANSITION_ID_ERR;
				setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
				goto MODULE_END;
			}
			if((nextMemFlag == TRUE)) {
				if ( (jmpFbMemInfo.logicType == SFC_LOGIC_TYPE) &&
					 (jmpFbMemInfo.usingType== TRANSITION_TYPE ) &&
					 (jmpFbMemInfo.TransitionID == IdNum)
					 ) 
				{
	  			    startFbExecIdInfoPtr = trsExecIdStartMemInfoGet(jmpFbMemInfo.TransitionID);
					retFbExecIdInfoPtr = gFBExeSequenceQueuePtr[jmpFbAddr-1];

					if( (startFbExecIdInfoPtr == retFbExecIdInfoPtr) &&
						(startFbExecIdInfoPtr == NULL) )
					{
						 retFbExecIdInfoPtr = NULL;
					}
				}

				else {
					status =JUMP_FB_CODE_ERR;
					setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Cur TRS Id", IdNum,
								"JMP TRS Id",jmpFbMemInfo.TransitionID,
								"JMP FB Addr", jmpFbAddr );
					goto MODULE_END;
				}
			}
			else retFbExecIdInfoPtr = NULL;
		break;

		case ACTION_TYPE:
			if(IdNum==0 || IdNum > MAX_ACTION_NUM){
				status = L_ACTION_ID_ERR;
				setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
				goto MODULE_END;
			}
			if((nextMemFlag == TRUE)) {
				if ( (jmpFbMemInfo.logicType == SFC_LOGIC_TYPE) &&
					 (jmpFbMemInfo.usingType== ACTION_TYPE ) &&
					 (jmpFbMemInfo.ActionID == IdNum)
					 )
				{
					startFbExecIdInfoPtr = actExecIdStartMemInfoGet(jmpFbMemInfo.ActionID);
					retFbExecIdInfoPtr = gFBExeSequenceQueuePtr[jmpFbAddr-1];

					if( (startFbExecIdInfoPtr == retFbExecIdInfoPtr) &&
						(startFbExecIdInfoPtr == NULL) )
					{
						 retFbExecIdInfoPtr = NULL;
					}

				} 
				else {
					status = JUMP_FB_CODE_ERR;
					setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Cur ACT Id", IdNum,
								"JMP ACT Id",jmpFbMemInfo.ActionID ,
								"JMP FB Addr", jmpFbAddr );
					goto MODULE_END;
				}
			}
			else retFbExecIdInfoPtr = NULL;
		break;
		
		default:
			retFbExecIdInfoPtr = NULL;
			status = L_LOGIC_ID_ERR;
			setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		break;
	}
MODULE_END:
	if(status!= NO_ERROR) retFbExecIdInfoPtr = NULL;
	else if(jmpFbAddr<=0 || jmpFbAddr > MAX_MPU_BLOCK_ADDR) retFbExecIdInfoPtr = NULL;

	return(retFbExecIdInfoPtr);
}



void sysExecInfoDisplay(uint32 arg1, uint32 arg2)
{
	uint32	index;	
    int8    line[16], *pLine;

	strModuleExecInfo  sysExeInfo;
	strVariableExecInfo sysVarInfo;
	uint32 curMaxSfcId =0;

RETRY:	
    printf( "\t* Choice ?( 1:LOGIC, 2:VARIABLE ) ");
    ioRead(gDebugPortFd, line, sizeof(line));
	pLine = line;
	
	index = atoi(pLine);
	switch(index)
	{
		case 0:
			printf("\t Cancel System Execution Memory Display!!!\n");
			return; 
		break;	
		case 1:
			fbSysExeModuleInfoRead(LOGIC_ID, &sysExeInfo);
			
			printf("\n\t*******LOGIC EXEC INFO******* \n");
			printf("\t  Max LOGIC Task : %d\n", sysExeInfo.curMaxSegmentId);
			printf("\t  Max LOGIC Addr : %d\n", sysExeInfo.curfbMaxBlockAddr);
			printf("\t Max Input Index : %d\n", sysExeInfo.curfbMaxParaIndex);
			printf("\tMax Output Index : %d\n", sysExeInfo.curfbMaxOutIndex);

		break;

		case 2:
			fbSysExeVarInfoRead(AUTO_VAR, &sysVarInfo);
			
		    printf("\n\t*******AUTO VAR MAX ADDRESS******* \n");
		    printf("\ Retain & Init Var Max Addr : %d\n", sysVarInfo.curMaxVarAddr);
		    printf("\             Retain Max Num : %d\n", sysVarInfo.curMaxVarRetainNum);
		    printf("\       Retain Info Max Addr : %d\n", sysVarInfo.curMaxVarRetainInfo);
		    printf("\           Init Var Max Num : %d\n", sysVarInfo.curMaxVarInitNum);

			fbSysExeVarInfoRead(MMEM_VAR, &sysVarInfo);
			
		    printf("\n\t*******FLAG VAR MAX ADDRESS******* \n");
		    printf("\ Retain & Init Var Max Addr : %d\n", sysVarInfo.curMaxVarAddr);
		    printf("\             Retain Max Num : %d\n", sysVarInfo.curMaxVarRetainNum);
		    printf("\       Retain Info Max Addr : %d\n", sysVarInfo.curMaxVarRetainInfo);
		    printf("\           Init Var Max Num : %d\n", sysVarInfo.curMaxVarInitNum);
		break;

		default:
			printf("\tInvalid Choice\n");
			goto RETRY;
		break;
	}
    printf( "\n");
}



uint32 segSchedulerCountRead(uint32 taskId, uint32 *schCount)
{
	uint32 status = NO_ERROR;
	strFbTaskTblMemInfo		*segTblInfoPtr;

	status = segTblPointerGet(LOGIC_ID, taskId,
								(strFbTaskTblMemInfo	**) &segTblInfoPtr);
			
  	if(status != NO_ERROR){
		setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);		
	}

	if(taskId != segTblInfoPtr->taskId) {
		status = L_SEG_ID_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);		
	}

	if(schCount!= NULL){
		*schCount = segTblInfoPtr->taskTickCnt;
	}

	return(status);
} 


uint32 fbCodeVarWriteEnoRead(uint32 taskId, uint32 fbBlockAddr, uint32* enoPtr)
{
	uint32 status =NO_ERROR;
	strFbMemInfo	*fbMemInfoPtr, *refFbInfoPtr;	
	strFbSpecActionInfo *specActionDataPtr;
	uint32 *specDataPtr;
	uint32 enoSpecData, memoryAddress;

	*enoPtr = 1;
	fbMemInfoPtr = gMpuFbMemInfoLocalPtr + (fbBlockAddr-1);

	if( fbMemInfoPtr->fbId != WRVAR_CODE )
	{
		status = FB_CODE_ID_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}

	specActionDataPtr 	= (strFbSpecActionInfo *)gMpuSpecAndVarDataLocalPtr;
	specDataPtr 	= (uint32 *)gMpuSpecAndVarDataLocalPtr;

	specActionDataPtr	+= fbMemInfoPtr->specActionIndex;
    specDataPtr		+= fbMemInfoPtr->specIndex;

	if(specActionDataPtr->refType != SPEC_TYPE_BYREF){
		status = FB_SPEC_ACTION_REF_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}
	else if(specActionDataPtr->memoryType != F_MEMORY){
		status = VARIABLE_KIND_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}

	memoryAddress = (*specDataPtr)>>8;
	refFbInfoPtr = gMpuFbMemInfoLocalPtr + (memoryAddress-1);

	if(taskId != refFbInfoPtr->fbTaskId) {
		status = L_SEG_ID_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}

	return(status);
}

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)

uint32 myRmtRefRunDataWrite(strAccumBusInfo* rmtRumDataInfo) 
{
	uint32 status = NO_ERROR;
	uint32 i =0, sendNum = 0;

	if(rmtRumDataInfo->accDataNum > MAX_RMT_COPY_BUF_NUM){
		status = DATA_LEN_ERR ; 
		setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
	}
	else if(rmtRumDataInfo->accDataNum >= MAX_DUAL_COPY_TYPE_NUM)
	{
		for(i=0;i<MAX_DUAL_COPY_SEND_NUM;i++)
		{
			if(rmtRumDataInfo->accDataNum >= MAX_DUAL_COPY_TYPE_NUM )
				sendNum = MAX_DUAL_COPY_TYPE_NUM;
			else sendNum = rmtRumDataInfo->accDataNum;

			status = myRmtAllTypeDataWrite( sendNum,
											 (uint8*)&rmtRumDataInfo->accumPtr[i*MAX_DUAL_COPY_TYPE_NUM], 
											 (uint8*)&rmtRumDataInfo->accumData[i*MAX_DUAL_COPY_TYPE_NUM],
											 (uint8*)&rmtRumDataInfo->accumType[i*MAX_DUAL_COPY_TYPE_NUM]
										   );
			if(status != NO_ERROR){
				setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
				return(status);
			}

			rmtRumDataInfo->accDataNum -= sendNum;

			if( (rmtRumDataInfo->accDataNum == 0) || 
				(rmtRumDataInfo->accDataNum > MAX_RMT_COPY_BUF_NUM)
			){
				rmtRumDataInfo->accDataNum = 0;
				break;
			}
		}
	}

	
	if(rmtRumDataInfo->accDataNum > 0 ){
		status = myRmtAllTypeDataWrite( rmtRumDataInfo->accDataNum,
										 (uint8*)&rmtRumDataInfo->accumPtr[0],
										 (uint8*)&rmtRumDataInfo->accumData[0],
										 (uint8*)&rmtRumDataInfo->accumType[0]
									   );
		if(status != NO_ERROR){
			setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
			return(status);
		}
		rmtRumDataInfo->accDataNum = 0;
	}

	return(status);
}
#endif 



void memoryBoardParameterDisplay(uint32 whereToRead, uint32 arg2)
{
    strSystemBootParaInfo	bootData;
	strSysConfigInfo configData;
	uint32 memoryBoardType = 0;
	uint32 zoneSize;

    int8 *memBoardStr[] =
    {
    	"MEM_NONE", "MEM611(2M Bytes)","MEM612(4M Bytes)","MEM613(8M Bytes)","MRAM(4M Bytes)",
    	"UNKNOWN"
	};
	int32	index;
	uint32 status;

	
	if(whereToRead == NVRAM_READ) 
	{
		status = nvRamRead((int8 *)&bootData,
							USR_ENET_CONFIG_PARA_OFFSET, 
							USR_ENET_CONFIG_PARA_SIZE
						   );
		if(status){
    		printf("%s[%d] - NVRAM Read Err\n",__FILE__, __LINE__);
    		return;
		}
		memoryBoardType = bootData.usrBootParaInfo.memoryBoardType;
	}

    else {
    	systemCnfgInfoRead((strSysConfigInfo *)&configData);
		memoryBoardType = configData.memoryBoardType;
    }

	
    if( (memoryBoardType < MEM_BOARD_NONE) || 
#if (BOARD_TYPE == LKV120_BOARD)
    	(memoryBoardType > MEM_4MB_MRAM))
#else
    	(memoryBoardType > MEM_613_BOARD))
#endif
        index = 5;
    else
        index = memoryBoardType;

	printf("\n");
	printf( "\t ************************************************\n");
	printf( "\t **   MEMORY Map Table Info Display\n");
	printf( "\t ************************************************\n");
    printf( "\t        Memory Board Config : %s\n\n", memBoardStr[index]);

	printf( "\t    LOGIC MAX. VIRTUAL ADDR : %7d\n",MAX_MPU_BLOCK_ADDR);
	printf( "\t       IO MAX. VIRTUAL ADDR : %7d\n",MAX_DIU_BLOCK_ADDR);
	printf( "\t       LOGIC MAX. INPUT NUM : %7d\n",MAX_MPU_SPEC_NUM);
	printf( "\t          IO MAX. INPUT NUM : %7d\n",MAX_DIU_SPEC_NUM);
	printf( "\t      LOGIC MAX. OUTPUT NUM : %7d\n",MAX_MPU_OUTPUT_NUM);
	printf( "\t         IO MAX. OUTPUT NUM : %7d\n",MAX_DIU_OUTPUT_NUM);
#if 0
	printf( "\t      SFC MAX STEP NUM : %7d\n",MAX_STEP_NUM);
	printf( "\tSFC MAX TRANSITOIN NUM : %7d\n",MAX_TRANSITOIN_NUM);
	printf( "\t    SFC MAX ACTION NUM : %7d\n\n",MAX_ACTION_NUM);
#endif

	printf( "\t        LOGIC MAX. MEM SIZE : %7d (%4.1fK)\n",MAX_MPU_MEM_SIZE,((float)MAX_MPU_MEM_SIZE)/1024.0);
	printf( "\t           IO MAX. MEM SIZE : %7d (%4.1fK)\n\n",MAX_DIU_MEM_SIZE,((float)MAX_DIU_MEM_SIZE)/1024.0);
	
	printf( "\t         SHARED VAR MAX NUM : %7d (%4.1fK)\n",MAX_SHARED_MEM_SIZE/4,((float)MAX_SHARED_MEM_SIZE)/1024.0);
	printf( "\t         M-REG. VAR MAX NUM : %7d (%4.1fK)\n",MAX_MPU_MMEM_NUM,((float)MAX_MPU_MMEM_NUM*4)/1024.0);
	printf( "\t           AUTO VAR MAX NUM : %7d (%4.1fK)\n\n",MAX_MPU_AUTOVAR_NUM,((float)MAX_MPU_AUTOVAR_NUM*4)/1024.0);
	
	printf( "\t           USR MAX MEM SIZE : 0x%06x (%4.1fK)\n",MAX_USR_MEM_SIZE,((float)MAX_USR_MEM_SIZE)/1024.0);
	zoneSize = (uint32)gDualDataBusPtr - (uint32)FIRST_FATAL_ERR_MNGR_OFFSET + FATAL_ERR_LOG_SIZE - FATAL_ERR_LOG_SIZE;
	printf( "\t            UNUSED MEM SIZE : 0x%06x (%4.1fK)\n",zoneSize,((float)zoneSize)/1024.0);
	printf( "\t      AVAIL. LOGIC MEM SIZE : 0x%06x (%4.1fK, without VAR and EXE INFO)\n",MAX_USABLE_MPU_MEM_SIZE,((float)MAX_USABLE_MPU_MEM_SIZE)/1024.0);
	printf( "\t         AVAIL. IO MEM SIZE : 0x%06x (%4.1fK)\n\n",MAX_USABLE_DIU_MEM_SIZE,((float)MAX_USABLE_DIU_MEM_SIZE)/1024.0);

	printf( "\t             MEM LOCAL ADDR : 0x%08x\n",(uint32)MEMORY_BOARD_LOCAL_ADDR);

	zoneSize = (uint32)gMpuFbMemInfoBusPtr - (uint32)gMpuFbSegTblInfoBusPtr;
	printf( "\t      LOGIC TASK START ADDR : 0x%08x (%7d Bytes)\n",(uint32)gMpuFbSegTblInfoBusPtr, zoneSize);
	zoneSize = (uint32)gMpuSpecAndVarDataBusPtr - (uint32)gMpuFbMemInfoBusPtr;
	printf( "\t        LOGIC FB START ADDR : 0x%08x (%7d Bytes)\n",(uint32)gMpuFbMemInfoBusPtr, zoneSize);
	zoneSize = (uint32)gMpuOutputDataBusPtr - (uint32)gMpuSpecAndVarDataBusPtr;
	printf( "\t     LOGIC INPUT START ADDR : 0x%08x (%7d Bytes)\n",(uint32)gMpuSpecAndVarDataBusPtr, zoneSize);
	zoneSize = (uint32)gSTSharedMemDataBusPtr - (uint32)gMpuOutputDataBusPtr;
	printf( "\t    LOGIC OUTPUT START ADDR : 0x%08x (%7d Bytes)\n\n",(uint32)gMpuOutputDataBusPtr, zoneSize);

	zoneSize = (uint32)gRetainVarInfoBusPtr - (uint32)gSTSharedMemDataBusPtr;
	printf( "\t      SHARED VAR START ADDR : 0x%08x (%7d Bytes)\n",(uint32)gSTSharedMemDataBusPtr, zoneSize);
	zoneSize = (uint32)gAutoVariableBusPtr - (uint32)gRetainVarInfoBusPtr;
	printf( "\t     RETAIN INFO START ADDR : 0x%08x (%7d Bytes)\n",(uint32)gRetainVarInfoBusPtr, zoneSize);
	zoneSize = (uint32)gMmemVariableBusPtr - (uint32)gAutoVariableBusPtr;
	printf( "\t        AUTO VAR START ADDR : 0x%08x (%7d Bytes)\n",(uint32)gAutoVariableBusPtr, zoneSize);

#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
	zoneSize = (uint32)gAutoVarInitBusPtr - (uint32)gMmemVariableBusPtr;
	printf( "\t   FLAG VAR START ADDR : 0x%08x (%7d Bytes)\n",(uint32)gMmemVariableBusPtr, zoneSize);
	zoneSize = (uint32)gMmemVarInitBusPtr - (uint32)gAutoVarInitBusPtr;
	printf( "\t    AUTO VAR INIT ADDR : 0x%08x (%7d Bytes)\n",(uint32)gAutoVarInitBusPtr, zoneSize);
	zoneSize = (uint32)gSFCStepInfoBusPtr - (uint32)gMmemVarInitBusPtr;
	printf( "\t    FLAG VAR INIT ADDR : 0x%08x (%7d Bytes)\n\n",(uint32)gMmemVarInitBusPtr, zoneSize);
#else
	zoneSize = (uint32)gSFCStepInfoBusPtr - (uint32)gMmemVariableBusPtr;
	printf( "\t      M_REG. VAR START ADDR : 0x%08x (%7d Bytes)\n",(uint32)gMmemVariableBusPtr, zoneSize);
#endif

#if 0
	zoneSize = (uint32)gSFCTransitionBusPtr - (uint32)gSFCStepInfoBusPtr;
	printf( "\t   SFC STEP START ADDR : 0x%08x (%7d Bytes)\n",(uint32)gSFCStepInfoBusPtr, zoneSize);
	zoneSize = (uint32)gSFCActionInfoBusPtr - (uint32)gSFCTransitionBusPtr;
	printf( "\t TRANSITION START ADDR : 0x%08x (%7d Bytes)\n",(uint32)gSFCTransitionBusPtr, zoneSize);
	zoneSize = (uint32)gSysExeInfoBusPtr - (uint32)gSFCActionInfoBusPtr;
	printf( "\t     ACTION START ADDR : 0x%08x (%7d Bytes)\n\n",(uint32)gSFCActionInfoBusPtr, zoneSize);
#endif
	
	zoneSize = (uint32)gDiuFbSegTblInfoBusPtr - (uint32)gSysExeInfoBusPtr;
	printf( "\t        EXE INFO START ADDR : 0x%08x (%7d Bytes)\n\n",(uint32)gSysExeInfoBusPtr, zoneSize);

	zoneSize = (uint32)gTmtcFbSegTblInfoBusPtr - (uint32)gDiuFbSegTblInfoBusPtr;
	printf( "\t         IO TASK START ADDR : 0x%08x (%7d Bytes)\n",(uint32)gDiuFbSegTblInfoBusPtr, zoneSize);

#if 0
	zoneSize = (uint32)gFieldbusFbSegTblInfoBusPtr - (uint32)gTmtcFbSegTblInfoBusPtr;
	printf( "\t  TMTC SEGT START ADDR : 0x%08x (%7d Bytes)\n",(uint32)gTmtcFbSegTblInfoBusPtr, zoneSize);
	zoneSize = (uint32)gDiuFbMemInfoBusPtr - (uint32)gFieldbusFbSegTblInfoBusPtr;
	printf( "\tFIELDB SEGT START ADDR : 0x%08x (%7d Bytes)\n",(uint32)gFieldbusFbSegTblInfoBusPtr, zoneSize);
#endif
	zoneSize = (uint32)gDiuSpecAndVarDataBusPtr - (uint32)gDiuFbMemInfoBusPtr;
	printf( "\t           IO FB START ADDR : 0x%08x (%7d Bytes)\n",(uint32)gDiuFbMemInfoBusPtr, zoneSize);
	zoneSize = (uint32)gDiuOutputDataBusPtr - (uint32)gDiuSpecAndVarDataBusPtr;
	printf( "\t        IO INPUT START ADDR : 0x%08x (%7d Bytes)\n",(uint32)gDiuSpecAndVarDataBusPtr, zoneSize);
	zoneSize = (uint32)gTmtcOutputDataBusPtr - (uint32)gDiuOutputDataBusPtr;
	printf( "\t       IO OUTPUT START ADDR : 0x%08x (%7d Bytes)\n",(uint32)gDiuOutputDataBusPtr, zoneSize);

#if 0
	zoneSize = (uint32)gEngFileInfoBusPtr - (uint32)gTmtcOutputDataBusPtr;
	printf( "\tTMTC OUTPUT START ADDR : 0x%08x (%7d Bytes)\n\n",(uint32)gTmtcOutputDataBusPtr, zoneSize);
	zoneSize = (uint32)FIRST_FATAL_ERR_MNGR_OFFSET - (uint32)gEngFileInfoBusPtr;
	printf( "\t    FMC ENG START ADDR : 0x%08x (%7d Bytes)\n",(uint32)gEngFileInfoBusPtr, zoneSize);
#endif

	printf( "\t    LOGGING MSG. START ADDR : 0x%08x (%7d Bytes)\n",FIRST_FATAL_ERR_MNGR_OFFSET, FATAL_ERR_LOG_SIZE);

	zoneSize = (uint32)gDualDataBusPtr - (uint32)FIRST_FATAL_ERR_MNGR_OFFSET + FATAL_ERR_LOG_SIZE - FATAL_ERR_LOG_SIZE;

	printf( "\t        FREE MEM START ADDR : 0x%08x (%7d Bytes)\n",FIRST_FATAL_ERR_MNGR_OFFSET + FATAL_ERR_LOG_SIZE, zoneSize);
	printf( "\t      DUAL INFO. START ADDR : 0x%08x (%7d Bytes)\n",(uint32)gDualDataBusPtr, MAX_DUAL_MEM_SIZE);

  return;	
}

uint32	fbCodeVarDataWriteWithType
				(
			uint32	moduleId,
			uint32	fbId,
            uint32	fbAddr,
			uint32  varIndex,     
			uint32  varType,
            uint32	*varData
				)
{
	uint32	status = NO_ERROR;
	strMemPointerInfo	memP, busMemAddrInfo;
	uint32	*varStartPtr, *varDataPtr;
	strFbMemInfo	fbMemInfo,  busFbMemInfo;
	
	strFbCodeTypePointerInfo	typePInfo;
	uint32 *varSizePtr, varWriteSize;
	uint32 *varTypePtr;
	
    memoryClear ( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
    memoryClear ( (uint8 *) &memP, sizeof(strMemPointerInfo));

     
	status = fbMemInfoRead	(
								moduleId,
            					fbAddr,
            					(strFbMemInfo *)&fbMemInfo
            				);
	if(status != NO_ERROR) goto MODULE_END;

    
    if(fbId != fbMemInfo.fbId) {
    	status = FB_CODE_MISMATCH_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
                                "Block Addr",	fbAddr,
                                "Recv FB Code",	fbId,
                                "Read FB Code",	fbMemInfo.fbId
							  );
        goto MODULE_END;
	}

    
	if( (varIndex < 1 ) || (varIndex > fbMemInfo.intlVarNo) ) {
    	status = VARIABLE_INDEX_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
                                "Block Addr",	fbAddr,
                                "FB Code",		fbId,
                                "Var Index",	varIndex
							  );					      	
        goto MODULE_END;
     }

		
	status = fbCodeParaTypePointerGet (
										fbId,           								
										(strFbCodeTypePointerInfo	*)&typePInfo 
									);
    if(status != NO_ERROR) {
		setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
    	goto MODULE_END;	
    }
		
	varSizePtr = (uint32 *)typePInfo.varSizeStartAddr;
	varSizePtr += (varIndex - 1);

	
	varTypePtr = (uint32 *)typePInfo.varTypeStartAddr;
	varTypePtr += (varIndex - 1);

	
	status = fbMemPointerGet(moduleId, 
							(strMemPointerInfo	*) &memP
							);
	if(status != NO_ERROR ) {
		setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
    	goto MODULE_END;
	}

	
	varStartPtr = (uint32 *)memP.paraMemPtr;

    
	varDataPtr = (uint32 *) (varStartPtr + fbMemInfo.varIndex + (*varSizePtr));

	
	if(GET_TYPE_SIZE(*varTypePtr)==SIZE64_TYPE) 
		varWriteSize = 2;
	else 
		varWriteSize = 1;

	
	memoryCopy ( 	(uint8 *)varDataPtr,
					(uint8 *)varData,
					DOUBLE_SIZE * varWriteSize
				); 

	
	status = busFbMemInfoRead	(
								moduleId,
            					fbAddr,
            					(strFbMemInfo	*) &busFbMemInfo
            				);
	if(status != NO_ERROR) goto MODULE_END;
	
   	
   	if( fbId != fbMemInfo.fbId) {
   		status = FB_CODE_MISMATCH_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
                                "Block Addr", fbAddr,
                                "Local Fb Code", fbId,
                                "MemBoard Fb Code", fbMemInfo.fbId
							  );
       	goto MODULE_END;
	}

   	
   	if(varIndex > busFbMemInfo.intlVarNo) {
   		status = VARIABLE_INDEX_ERR; 
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
                                "fb Code", fbId,
                                "Req Var Index", varIndex,
                                "MemBoard Var No", fbMemInfo.intlVarNo
							  );
       	goto MODULE_END;
	}

	status = busFbMemPointerGet	(moduleId, &busMemAddrInfo);
	if(status != NO_ERROR) goto MODULE_END;

	varStartPtr = (uint32 *)busMemAddrInfo.paraMemPtr;

   	
	varDataPtr = (uint32 *)(varStartPtr + busFbMemInfo.varIndex);
   	varDataPtr += varIndex - 1;

	status = myBusDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, 
							BUS_DATA_WRITE ,
							(uint8 *)varDataPtr, 
							(uint8 *)varData
						 );
	if(status)
		setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
	else 
	{
		
		status = fbDataWriteToRemoteVmeMemBoard(	moduleId,
													fbAddr, 
													varIndex,
													*varData,
													DUAL_VAR_WRITE, 
													&fbMemInfo
												);
		if(status)
			setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
	}	
	
MODULE_END:					   							 
	return(status);
}


uint32	fbCodeBlockAddrReadWithInput
				( 
			uint32 	taskId,
			uint32	moduleId, 
			uint32	fbBlockAddr, 
			uint32 	*inBlkAddrStartPtr
				)
{
	uint32 status = NO_ERROR;
	strFbMemInfo	*fbMemInfoPtr;
	
    if(moduleId == LOGIC_ID)  fbMemInfoPtr = gMpuFbMemInfoLocalPtr;	
	else return(L_MODULE_ID_ERR);
 
   	
   	fbMemInfoPtr += (fbBlockAddr-1);   	

	
    status = fbAddrCopyFromMem (	taskId, 
									moduleId,
									fbBlockAddr, 
									(uint32 *)inBlkAddrStartPtr, 
									fbMemInfoPtr, 
									WITH_ADDR
								);

    return(status);
}


static uint32	fbAddrCopyFromMem 
				(
			 uint32	taskId,
			 uint32	moduleId,
             uint32	fbBlockAddr,
			 uint32	*destPtr,
             strFbMemInfo	*fbMemPtr,
             uint32	flag
             	)
{
	uint32	status = NO_ERROR;
	uint32	*specTypePtr, *outputTypePtr;
	uint32	*specStartPtr;
	uint32	i = 0, fbAddr;
	uint32	*specActionStartPtr = NULL;
	strFbSpecActionInfo *specActionDataPtr= NULL;
	strFbCodeTypePointerInfo typePInfo;

	
	if(moduleId != LOGIC_ID) {
		status = L_MODULE_ID_ERR;
		setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}

	specActionStartPtr 	= (uint32 *)gMpuSpecAndVarDataLocalPtr;
	specActionStartPtr	+= fbMemPtr->specActionIndex;

	specActionDataPtr	= (strFbSpecActionInfo *)specActionStartPtr;

	specStartPtr 		= (uint32 *)gMpuSpecAndVarDataLocalPtr;
    specStartPtr		+= fbMemPtr->specIndex;

	status = fbCodeParaTypePointerGet(
								fbMemPtr->fbId,
								(strFbCodeTypePointerInfo *)&typePInfo
									);
	if(status) {
		setErrorCode( __FILE__, __LINE__, __FUNCTION__,status);
		return(status);
	}

	specTypePtr 	= (uint32 *)typePInfo.specTypeStartAddr;		
	outputTypePtr 	= (uint32 *)typePInfo.outputTypeStartAddr;	

	
	
	if( IS_ANY_TYPE_FB_CODE(fbMemPtr->fbId) &&	
		CHK_IS_ANY_TYPE(*(outputTypePtr + 1))
	) {
		if(fbMemPtr->paraRunType == 0) {
			status = NO_RUN_PARA_TYPE_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"FB Code",fbMemPtr->fbId,
							"FB Addr",fbMemPtr->fbFuncAddr,
							"Run Type",fbMemPtr->paraRunType);
			return(status);
		}
	}

	
	for(i=0; i < fbMemPtr->inputNo; i++,
				destPtr++, specStartPtr++, specTypePtr++, specActionDataPtr++)
	{	
		if(moduleId==LOGIC_ID) {

				
			if(specActionDataPtr->refType==SPEC_TYPE_BYREF) {
				if(flag == WITH_ADDR) {
					fbAddr = *specStartPtr >> 8;	
					
					if((fbAddr < 1) || (fbAddr > MAX_MPU_BLOCK_ADDR)) {
						status = FB_SPEC_ACTION_ADDR_ERR;
						setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
											"Mem Addr", fbAddr,
											"Not Using", 0,
											"Not Using",0
											);
					}

					memoryCopy((uint8*)destPtr, (uint8*)&fbAddr, DOUBLE_SIZE) ;

					if(status != NO_ERROR) 
						return(status);
				}
			}
		} 
	}

    return(status);
}
