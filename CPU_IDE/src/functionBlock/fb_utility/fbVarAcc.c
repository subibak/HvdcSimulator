
#include	<stdio.h>
#include	<stdLib.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include	"usrDefBuf.h"
#include	"fbMem.h"
#include	"memDef.h"
#include	"usrErrCode.h"
#include    "sysConfig.h"

#include	"netProtocol.h"
#include	"segScheduler.h"

#include	"protocolBuf.h"
#include	"systemDiag.h"
#include	"network.h"

#include	"usrQueData.h"
#include	"msgQueData.h"
#include	"sysDual.h"

#include	"debugInfo.h"

/*[V107] : M 메모리 매핑때문에 16K-->MEM_MPU_MMEM_NUM */  
#define	M_MEMORY_MAX_ADDR_DWORD		MEM_MPU_MMEM_NUM
#define	M_MEMORY_MAX_ADDR_BYTE		M_MEMORY_MAX_ADDR_DWORD*4
#define	M_MEMORY_MAX_ADDR_WORD		M_MEMORY_MAX_ADDR_DWORD*2
#define	M_MEMORY_MAX_ADDR_LWORD		M_MEMORY_MAX_ADDR_DWORD/2

#define	S_MEMORY_MAX_ADDR_DWORD		MAX_SHARED_MEM_NUM			
#define	S_MEMORY_MAX_ADDR_BYTE		S_MEMORY_MAX_ADDR_DWORD*4
#define S_MEMORY_MAX_ADDR_WORD		S_MEMORY_MAX_ADDR_DWORD*2
#define	S_MEMORY_MAX_ADDR_LWORD		S_MEMORY_MAX_ADDR_DWORD/2

#define	MAX_BITPOSITION_BYTE		8
#define	MAX_BITPOSITION_WORD		16
#define	MAX_BITPOSITION_DWORD		32
#define	MAX_BITPOSITION_LWORD		64

#define INT32_TO_REAL32		((INT_TYPE<<4) | (REAL_TYPE))
#define INT32_TO_INT64		((INT_TYPE<<4) | (LINT_TYPE))
#define INT32_TO_UINT64		((INT_TYPE<<4) | (ULINT_TYPE))
#define INT32_TO_REAL64		((INT_TYPE<<4) | (LREAL_TYPE))

#define UINT32_TO_INT32		((UINT_TYPE<<4) | (INT_TYPE))
#define UINT32_TO_REAL32	((UINT_TYPE<<4) | (REAL_TYPE))
#define UINT32_TO_INT64		((UINT_TYPE<<4) | (LINT_TYPE))
#define UINT32_TO_UINT64	((UINT_TYPE<<4) | (ULINT_TYPE))
#define UINT32_TO_REAL64	((UINT_TYPE<<4) | (LREAL_TYPE))

#define REAL32_TO_REAL64	((REAL_TYPE<<4) | (LREAL_TYPE))
#define INT64_TO_REAL64		((LINT_TYPE<<4) | (LREAL_TYPE))
#define UINT64_TO_REAL64	((ULINT_TYPE<<4) | (LREAL_TYPE))


#define TIME_TO_INT32		((TIME32_TYPE<<4) | (INT_TYPE))
#define TIME_TO_REAL32		((TIME32_TYPE<<4) | (REAL_TYPE))
#define TIME_TO_INT64		((TIME32_TYPE<<4) | (LINT_TYPE))
#define TIME_TO_UINT64		((TIME32_TYPE<<4) | (ULINT_TYPE))
#define TIME_TO_REAL64		((TIME32_TYPE<<4) | (LREAL_TYPE))
#define TIME_TO_UINT32		((TIME32_TYPE<<4) | (UINT_TYPE))
#define INT32_TO_TIME		((INT_TYPE<<4) | (TIME32_TYPE))
#define INT32_TO_UINT32		((INT_TYPE<<4) | (UINT_TYPE))
#define UINT32_TO_TIME		((UINT_TYPE<<4) | (TIME32_TYPE))

extern void		*gMpuOutputDataLocalPtr,*gMpuOutputDataBusPtr;
extern void		*gAutoVariableLocalPtr, *gAutoVariableBusPtr;
extern void		*gMmemVariableLocalPtr, *gMmemVariableBusPtr;
extern uint32	*gSTSharedMemDataLocalPtr,*gSTSharedMemDataBusPtr;
extern strSysConfigInfo	gSysCnfgInfo;
extern uint8	*gBusMpuFbStartAddr;

extern void		*gDiuOutputDataLocalPtr, *gDiuOutputDataBusPtr;

extern int32	gDebugPortFd;

extern uint32	*gSTSharedMemDataBusPtr;
extern strSysRunningConfigInfo		*gSysRunningInfoPtr;
extern strMpuDiuRunningStatusInfo 	*gDiuRasInfoPtr;
extern	void	*gFieldbusOutputDataLocalPtr, *gFieldbusOutputDataBusPtr;
extern	void	*gTmtcOutputDataLocalPtr, *gTmtcOutputDataBusPtr;

uint32	variableDataRead(uint32 *,uint32,uint32,uint32);
uint32  variableDataWrite(uint32 *,uint32,uint32, uint32);
uint32  busVariableDataWrite(uint32 *,uint32,uint32, uint32,uint32);
uint32  rmtVariableDataWrite(uint32 *,uint32,uint32, uint32,uint32);

uint32  localVariableRetainInfoWrite(uint32, uint32, uint8);
uint32  localVariableRetainInfoRead(uint32, uint32, uint8*);
uint32  busVariableRetainInfoWrite(uint32, uint32, uint8);
uint32  busVariableRetainInfoRead(uint32, uint32, uint8*);
uint32  varRetainInfoReadwithRef(uint32, strFbSpecActionInfo, uint32*);

uint32  refDataCopyFromMem(uint32*, uint32, strFbSpecActionInfo, uint32, uint32);
uint32  refDataCopyToMem(uint32*, uint32, strFbSpecActionInfo, uint32);
uint32  refDataCopyToBusMem(uint32*, uint32, strFbSpecActionInfo, uint32);
uint32  refDataCopyToRmtMem(uint32*, uint32, strFbSpecActionInfo, uint32);

static void autoMemoryDisplay(uint32 , uint32 );
static void sharedMemoryDisplay(uint32 , uint32 );
static void flagMemoryDisplay(uint32 , uint32 );
static void variableDataDisplay(uint32 , uint32 );
static void variableInMemoryDisplay(uint32, uint32);
void   variableMemoryDataDisplay(uint32, uint32);
uint32 specActionDataCheck(uint32 *, uint32  );
uint32 varMemoryClear(void);

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
uint32 myRmtRefRunDataWrite(strAccumBusInfo* ) ;
#endif

uint32 rmtSharedVarNumWrite(uint32 , uint32 , uint32 , uint32 *);
uint32 rmtFlagVarNumWrite(uint32, uint32, uint32, uint32, uint32 *);


extern  void	memoryCopy ( uint8 *, uint8 *, uint32 );
extern  void	memoryClear ( uint8 *, uint32 );
extern	void	fourBytesDataCopy ( uint32 *, uint32 *, uint32 );
extern  uint32	fbCodeParaTypePointerGet(uint32	,strFbCodeTypePointerInfo	*);
extern  uint32	fbMemPointerGet(uint32 , strMemPointerInfo *);
extern  uint32	busFbMemPointerGet(uint32,strMemPointerInfo	*);
extern uint32	rmtbusFbMemPointerGet(uint32,strMemPointerInfo	*);
extern  uint32	myBusDataAcc(uint8 *, uint32,uint32	, uint32 ,uint8 *, uint8 *);
extern void		setErrorCode(uint8 *, uint32, uint8 *, uint32);
extern void		setErrorCodeWithVal(uint8 *, uint32, uint8 *, uint32,
							uint8 *,uint32,uint8 *,uint32,uint8 *,uint32);
extern uint32	fbSysExeVarInfoRead(uint32 , strVariableExecInfo* );
extern int32	ioRead(int32 , int8 *, int32 );
extern void		spaceSkip(int8 **);

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
extern uint32   myRmtDataAcc(uint8 *, uint32, uint32, uint32, int8 *, int8 *);
extern uint32	myRmtAllTypeFlagDataWrite(uint32, uint32, uint32,int8*, int8*);
#endif

extern uint32	systemCnfgInfoRead( strSysConfigInfo *);
extern uint32   fbCodeMemPointerGet(uint32, uint32, strFbMemInfo **);
extern uint32	getFbTaskTblInfo(uint32 , uint32 , strFbTaskTblMemInfo *);
extern uint32	fbMemInfoRead(uint32, uint32, strFbMemInfo *);

extern void		screenClear(void);
extern int32    keyHit(void);
extern void 	cursorPositionSet(void);
extern void		sysTaskDelay (int32 );
extern int32	systemClockRateGet(void);

extern void		eightByteConversion( uint32 *, uint32 *, uint8 );
extern void		mySwap(void *, void *, uint32);
extern uint32 	fbSysExeInfoPointerGet(strSysExecInfo** ,uint32 );


uint32 refDataCopyFromMem	(	uint32	*retSpecDataPtr,
								uint32	specData,
								strFbSpecActionInfo specActionData,
								uint32	specType, 
								uint32  tcType
							)
{
	uint32 status = NO_ERROR;
	uint32 *outputStartPtr;
	uint32 memoryAddress,index;
	
	strFbCodeTypePointerInfo readFcodeTypePInfo ;
	uint32  outputBlockAddr, sindex;
	uint8 booldata = 0;
	uint8* srcPtr = NULL;

	uint32	kindTypeConversion = 0;
	uint32	kindSizeConversion = 0;

	int32	int32Buf;
#ifdef USED_BIT64
	int64	int64Buf;
	uint64	uint64Buf;
	double	doubleBuf;
#endif
	float	floatBuf;
	int8	int8Buf;
	int16   int16Buf;
	uint32  uint32Buf;

	strFbMemInfo *fbMemInfoPtr;
    strFbTaskTblMemInfo	segInfo;
	uint32			fbBlkAddress = 0;
	strFbMemInfo	fbMemInfo;

	uint32*	uint32SrcPtr = NULL; 	
	strSysConfigInfo 	sysCnfgInfo;
	int32 anaPointStartPosition, ioPointDigAnaKind;

	if(specActionData.refType != SPEC_TYPE_BYREF) 
	{
		status = FB_SPEC_ACTION_REF_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		
		return(status);
	}

	systemCnfgInfoRead( &sysCnfgInfo);
	
	memoryAddress = specData>>8;
	index = (specData>>2)&0x3f; 

	memoryClear((uint8*)retSpecDataPtr, DOUBLE_SIZE);

	switch(specActionData.memoryType) 
	{
		case F_MEMORY:	
			if( (memoryAddress<1) || 
				(memoryAddress>MAX_MPU_BLOCK_ADDR) ||
				(index == 0x0))	
			{
				status = FB_SPEC_ACTION_ADDR_ERR;
				setErrorCodeWithVal(	__FILE__,	__LINE__,"refDataCopyFromMem",status,
										"Mem Addr", memoryAddress,
										"Output Index", index,
										"Not Using",0
								);
				break;
			}
			else 
			{
				
				status = fbCodeMemPointerGet(	LOGIC_ID, memoryAddress, &fbMemInfoPtr);
				if(status) 
				{
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				}

				status = fbCodeParaTypePointerGet	(
											fbMemInfoPtr->fbId, 
											&readFcodeTypePInfo	);
				if(status) 
				{
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				}
			}

			if( (index > fbMemInfoPtr->outputNo) || (index == 0) ) 
			{
				status = FB_SPEC_ACTION_ADDR_ERR;
				setErrorCodeWithVal(	__FILE__, __LINE__, "refDataCopyFromMem", status,
										"FB CODE", fbMemInfoPtr->fbId,
										"MAX Output Index", fbMemInfoPtr->outputNo,
										"Revc Index", index
							     	);
				break;
			}
			
			
			sindex = *((uint32 *)readFcodeTypePInfo.outputSizeStartAddr + (index-1));

			outputStartPtr		= (uint32 *)gMpuOutputDataLocalPtr;
			outputBlockAddr 	= (uint32)fbMemInfoPtr->outputIndex + sindex;

			srcPtr =  (uint8*)((uint32 *)outputStartPtr + outputBlockAddr);
			
			if(GET_TYPE_SIZE(specType)==SIZE32_TYPE) 
			{	
				memoryCopy ((uint8*)retSpecDataPtr, (uint8*)srcPtr, 4);
			} 
#ifdef USED_BIT64
			else if(GET_TYPE_SIZE(specType)==SIZE64_TYPE) 
			{	
				if(specActionData.dataSize==SPEC_SIZE_LWORD)
					memoryCopy ((uint8*)retSpecDataPtr, (uint8*)srcPtr, 8);
				else
				{ 
					memoryCopy ((uint8*)retSpecDataPtr, (uint8*)srcPtr, 4);
					*(retSpecDataPtr+1) = 0;
				}
			} 
#endif
			else 
			{
				status = FB_SPEC_TYPE_SIZE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				break;
			}

		break;

		case A_MEMORY:	
			if( (memoryAddress<1) || 
				(memoryAddress>MAX_MPU_AUTOVAR_NUM) || 
				(index != 0x0))	
			{
				status = FB_SPEC_ACTION_ADDR_ERR;
				setErrorCodeWithVal(	__FILE__, __LINE__, __FUNCTION__, status,
									"MEM ADDR", memoryAddress,
									"INDEX", index,
									"MEM TYPE",specActionData.memoryType									
								);
				break;
			}
			
			srcPtr = (uint8*)((uint32 *)gAutoVariableLocalPtr + (memoryAddress - 1) );

			if(GET_TYPE_SIZE(specType)==SIZE32_TYPE)
			{	
				memoryCopy ((uint8*)retSpecDataPtr, (uint8*)srcPtr, 4);
			} 
#ifdef USED_BIT64
			else if(GET_TYPE_SIZE(specType)==SIZE64_TYPE)
			{	
				if(specActionData.dataSize==SPEC_SIZE_LWORD)
					memoryCopy ((uint8*)retSpecDataPtr, (uint8*)srcPtr, 8);
				else
				{ 
					memoryCopy ((uint8*)retSpecDataPtr, (uint8*)srcPtr, 4);
					*(retSpecDataPtr+1) = 0;
				}
			}
#endif
			else 
			{
				status = FB_SPEC_TYPE_SIZE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				break;
			}
		break;

		case S_MEMORY:	
		case M_MEMORY:	
			
			if(specActionData.memoryType == S_MEMORY)
			{
				
				srcPtr = (uint8*)gSTSharedMemDataLocalPtr;
			}
					
			else if(specActionData.memoryType == M_MEMORY)
			{
				
				srcPtr = (uint8*)gMmemVariableLocalPtr;
			}
			
			if(GET_TYPE_SIZE(specType)==SIZE32_TYPE)
			{	
				memoryClear ((uint8*)retSpecDataPtr, 4);
			} 
#ifdef USED_BIT64
			else if(GET_TYPE_SIZE(specType)==SIZE64_TYPE)
			{	
				memoryClear ((uint8*)retSpecDataPtr,  8);
			} 
#endif
			else 
			{
				status = FB_SPEC_TYPE_SIZE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				break;
			}
			
			if(specActionData.bitposFlag==1) 
			{
				if((specActionData.bitPosition < 0)) 
				{
					status = FB_SPEC_ACTION_BITPOS_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				}
				else if((specActionData.dataSize==SPEC_SIZE_BYTE) && (specActionData.bitPosition < 8) )
				{
					srcPtr += memoryAddress * 1 ; 
					booldata = (uint8)((*(uint8 *)srcPtr)>>specActionData.bitPosition)&0x01;
					int32Buf = (uint32)booldata;
					fourBytesDataCopy((uint32*)retSpecDataPtr, (uint32*)&int32Buf ,1);
				} 
				else if((specActionData.dataSize==SPEC_SIZE_WORD) && (specActionData.bitPosition < 16 ))
				{
					if(memoryAddress>M_MEMORY_MAX_ADDR_WORD) 
					{
						status = FB_SPEC_ACTION_ADDR_ERR;
						setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
											"Memory Addr", memoryAddress,
											"MAX Addr", M_MEMORY_MAX_ADDR_WORD,
											"DATA SIZE", specActionData.dataSize
										);
						return (status);
					}

					srcPtr += memoryAddress * 2; 

					
					booldata = (uint8)((*(uint16 *)srcPtr)>>specActionData.bitPosition)&0x01;

					int32Buf = (uint32)booldata;
					fourBytesDataCopy((uint32*)retSpecDataPtr, (uint32*)&int32Buf ,1);
				} 
				else if((specActionData.dataSize==SPEC_SIZE_DWORD) && (specActionData.bitPosition < 32 ) )
				{

					if(memoryAddress>M_MEMORY_MAX_ADDR_DWORD) 
					{
						status = FB_SPEC_ACTION_ADDR_ERR;
						setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
											"Memory Addr", memoryAddress,
											"MAX Addr", M_MEMORY_MAX_ADDR_DWORD,
											"DATA SIZE", specActionData.dataSize
										);
						return (status);
					}
					srcPtr += memoryAddress * 4 ; 

					
					booldata = (uint8)((*(uint32 *)srcPtr)>>specActionData.bitPosition)&0x01;

					int32Buf = (uint32)booldata;
					fourBytesDataCopy((uint32*)retSpecDataPtr, (uint32*)&int32Buf ,1);
				} 
#ifdef USED_BIT64
				else if((specActionData.dataSize==SPEC_SIZE_LWORD) && (specActionData.bitPosition < 64 ) )
				{
					if(memoryAddress>M_MEMORY_MAX_ADDR_LWORD) 
					{
						status = FB_SPEC_ACTION_ADDR_ERR;
						setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
											"Memory Addr", memoryAddress,
											"MAX Addr", M_MEMORY_MAX_ADDR_LWORD,
											"DATA SIZE", specActionData.dataSize
										);
						return (status);
					}
					
					srcPtr += memoryAddress * 8 ; 
					booldata = (uint8)((*(uint64 *)srcPtr)>>specActionData.bitPosition)&0x01;
					int32Buf = (uint32)booldata;
					fourBytesDataCopy((uint32*)retSpecDataPtr, (uint32*)&int32Buf ,1);
				}
#endif
				else if((specActionData.dataSize==SPEC_SIZE_BIT)
						&& (specActionData.bitPosition == 0 ) )
				{
					srcPtr += memoryAddress/8 ; 
					booldata = (uint8)((*(uint8 *)srcPtr)>>memoryAddress%8)&0x01;
					int32Buf = (uint32)booldata;
					fourBytesDataCopy((uint32*)retSpecDataPtr, (uint32*)&int32Buf ,1);
				}
				else 
				{
					status = FB_SPEC_ACTION_BITPOS_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				}
			}			
			else
			{
				if(specActionData.dataSize==SPEC_SIZE_BYTE) 
				{
					srcPtr += memoryAddress * 1 ; 

					int32Buf =(uint32)(*(uint8*)srcPtr);
					fourBytesDataCopy((uint32*)retSpecDataPtr, (uint32*)&int32Buf ,1);
				} 
				else if(specActionData.dataSize==SPEC_SIZE_WORD) 
				{
					srcPtr += memoryAddress * 2; 
					
					int32Buf =(uint32)(*(uint16*)srcPtr);
					fourBytesDataCopy((uint32*)retSpecDataPtr, (uint32*)&int32Buf ,1);
				} 
				else if(specActionData.dataSize==SPEC_SIZE_DWORD) 
				{
					srcPtr += memoryAddress * 4 ; 
					memoryCopy ((uint8*)retSpecDataPtr, (uint8*)srcPtr, 4);
				} 
#ifdef USED_BIT64
				else if(specActionData.dataSize==SPEC_SIZE_LWORD) 
				{
					srcPtr += memoryAddress * 8 ; 
					memoryCopy ((uint8*)retSpecDataPtr, (uint8*)srcPtr, 8);
				} 
#endif
				else if(specActionData.dataSize==SPEC_SIZE_BIT) 
				{
					srcPtr += memoryAddress/8 ;  
					booldata = (*srcPtr>>memoryAddress%8)&0x01;
					int32Buf =(uint32)booldata;
					fourBytesDataCopy((uint32*)retSpecDataPtr, (uint32*)&int32Buf ,1);
				} 
				else 
				{
					status = FB_SPEC_ACTION_SIZE_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				}
			}

		break;

		default:
			status = VARIABLE_KIND_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		break;
	}

	if(status != NO_ERROR)
		return(status);
		
	
	if( (CHK_IS_ANY_TYPE(specType) ) && (tcType==TYPECASTING_ENABLE) &&
		(GET_BASIC_DATA_TYPE(specActionData.dataType)==INT_TYPE) &&
	    ( (GET_TYPE_DATA_SIZE(specType) >>12) > specActionData.dataSize)) 
	{
		kindSizeConversion = GET_TYPE_DATA_SIZE(specType);
			
		if(GET_TYPE_DATA_SIZE(specActionData.dataSize<<12)==SIZE08_TYPE) 
		{
			int8Buf = (int8)(*(int32 *)retSpecDataPtr);
			switch(kindSizeConversion)
			{
				case SIZE16_TYPE:
					int16Buf = (int16)int8Buf;
					memoryCopy((uint8*)retSpecDataPtr, (uint8*)&int16Buf,WORD_SIZE);
#ifdef VXWORKS
					memoryCopy((uint8*)(((uint8*)retSpecDataPtr)+2), (uint8*)&int16Buf,WORD_SIZE);
#endif 
				break;
				case SIZE32_TYPE:
					*retSpecDataPtr = (int32)int8Buf;
				break;
#ifdef USED_BIT64
				case SIZE64_TYPE:
					int8Buf = (int8)(*(int32 *)retSpecDataPtr);
					int64Buf = (int64)(int8Buf);
					memoryCopy((uint8*)retSpecDataPtr, (uint8*)&int64Buf,LONG_SIZE);
				break;
#endif
				default:
					
					status = ANY_FUNC_OUT_SIZE_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				break;
			}
		}
		else if(GET_TYPE_DATA_SIZE(specActionData.dataSize<<12)==SIZE16_TYPE) 
		{
			switch(kindSizeConversion)
			{
				case SIZE32_TYPE:
					int16Buf = (int16)(*(int32 *)retSpecDataPtr);
					*retSpecDataPtr = (int32)int16Buf;
				break;
#ifdef USED_BIT64
				case SIZE64_TYPE:
					int16Buf = (int16)(*(int32 *)retSpecDataPtr);
					int64Buf = (int64)(int16Buf);
					memoryCopy((uint8*)retSpecDataPtr, (uint8*)&int64Buf,LONG_SIZE);
				break;
#endif
				default:
					
					status = ANY_FUNC_OUT_SIZE_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				break;
			}
		}
		else if(GET_TYPE_DATA_SIZE(specActionData.dataSize<<12)==SIZE32_TYPE) 
		{
			switch(kindSizeConversion)
			{
#ifdef USED_BIT64
				case SIZE64_TYPE:
					int64Buf = (int64)(*(int32 *)retSpecDataPtr);
					memoryCopy((uint8*)retSpecDataPtr, (uint8*)&int64Buf,LONG_SIZE);
				break;
#endif
				default:					
					status = ANY_FUNC_OUT_SIZE_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				break;
			}
		}
	}


	if(status != NO_ERROR)
		return(status);

	if( CHK_IS_ANY_TYPE(specType) && (tcType==TYPECASTING_ENABLE) 
						&&
		( (GET_BASIC_DATA_TYPE(specActionData.dataType) != GET_BASIC_DATA_TYPE(specType)) 
#ifdef USED_BIT64
			||
		  ((specActionData.bitposFlag==1)&&(GET_TYPE_SIZE(specType)==SIZE64_TYPE)) 
#endif
		))
	{	
#ifdef USED_BIT64
			
		if( (specActionData.dataSize==SPEC_SIZE_LWORD) &&
			(GET_TYPE_SIZE(specType)==SIZE64_TYPE) &&
			(specActionData.bitposFlag==0))
		{
			status = FB_SPEC_ACTION_SIZE_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}
#endif

		
#ifdef USED_BIT64
		if( (specActionData.bitposFlag==1) &&(GET_TYPE_SIZE(specType)==SIZE64_TYPE) )
			kindTypeConversion = (UINT_TYPE<<4) | GET_BASIC_DATA_TYPE(specType);
 		else 
			kindTypeConversion = ((GET_BASIC_DATA_TYPE(specActionData.dataType))<<4) | GET_BASIC_DATA_TYPE(specType);
#else
		kindTypeConversion = ((GET_BASIC_DATA_TYPE(specActionData.dataType))<<4) | GET_BASIC_DATA_TYPE(specType);
#endif
		
		switch(kindTypeConversion) 
		{
			case INT32_TO_REAL32:
				floatBuf = (float)(*(int32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&floatBuf),1);
			break;
			
			case UINT32_TO_INT32: case TIME_TO_INT32: 
				int32Buf = (int32)(*(uint32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)&int32Buf,1);
			break;
			
			case UINT32_TO_REAL32: case TIME_TO_REAL32:
				floatBuf = (float)(*(uint32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&floatBuf),1);
			break;
			
			case INT32_TO_UINT32: case INT32_TO_TIME:
				uint32Buf = (uint32)(*(int32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)&uint32Buf,1);				
			break;
			
			case TIME_TO_UINT32: 
			case UINT32_TO_TIME:
			break;
#ifdef USED_BIT64
			
			case INT32_TO_INT64:
				mySwap((uint32*)retSpecDataPtr,(uint32*)retSpecDataPtr+1,DOUBLE_SIZE);
				int64Buf = (int64)(*(int32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&int64Buf),2);
			break;
			
			case INT32_TO_UINT64:
				mySwap((uint32*)retSpecDataPtr,(uint32*)retSpecDataPtr+1,DOUBLE_SIZE);
				uint64Buf = (uint64)(*(int32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&uint64Buf),2);
			break;
			
			case INT32_TO_REAL64:
				mySwap((uint32*)retSpecDataPtr,(uint32*)retSpecDataPtr+1,DOUBLE_SIZE);
				doubleBuf = (double)(*(int32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&doubleBuf),2);
			break;
			
			case UINT32_TO_INT64: case TIME_TO_INT64:
				int64Buf = (int64)(*(uint32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&int64Buf),2);
			break;
			
			case UINT32_TO_UINT64: case TIME_TO_UINT64:
				uint64Buf = (uint64)(*(uint32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&uint64Buf),2);
			break;
			
			case UINT32_TO_REAL64: case TIME_TO_REAL64:
				doubleBuf = (double)(*(uint32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&doubleBuf),2);
			break;
			
			case REAL32_TO_REAL64:
				doubleBuf = (double)(*(float *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&doubleBuf),2);
			break;
			
			case INT64_TO_REAL64:
				doubleBuf = (double)(*(int64 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&doubleBuf),2);
				break;
#endif
			default:
				status = FB_SPEC_ACTION_TYPE_ERR;
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"Type-Conversion From", GET_BASIC_DATA_TYPE(specActionData.dataType),
									"Type-Conversion To", GET_BASIC_DATA_TYPE(specType),
									"kindTypeConversion", kindTypeConversion
								);
			break;
		}
	}


	return(status);
}


uint32 refDataCopyToMem(uint32 *setSpecDataPtr,uint32 specData,
						 strFbSpecActionInfo specActionData, uint32	specType)
{
	uint32 status = NO_ERROR;
	uint32 memoryAddress,index;
	uint32 booldata = 0;
	uint8* destPtr= NULL;

    strFbTaskTblMemInfo	segInfo;
	uint32			fbBlkAddress = 0;
	strFbMemInfo	fbMemInfo;

	uint32 *uint32DestPtr=NULL;
	uint8 uint8Data; uint16 uint16Data; 
	uint16	wordData;
	uint32	longData;
#ifdef USED_BIT64
	uint64	dLongData;
#endif
	strSysConfigInfo 	sysCnfgInfo;
	int32 anaPointStartPosition, ioPointDigAnaKind;
	
	if(specActionData.refType !=SPEC_TYPE_BYREF )
	{
		status = FB_SPEC_ACTION_REF_ERR;
		return(status);
	}

	systemCnfgInfoRead( &sysCnfgInfo);
	
	memoryAddress = specData>>8;
	index = (specData>>2)&0x3f; 
	
	switch(specActionData.memoryType) 
	{
		case F_MEMORY:
			
		break;

		case A_MEMORY:
			
			destPtr = (uint8*)((uint32 *)gAutoVariableLocalPtr + (memoryAddress-1));

			if(GET_TYPE_SIZE(specType)==SIZE32_TYPE) 
			{			
				memoryCopy ( (uint8*)destPtr, (uint8*)setSpecDataPtr, 4);
			} 
			else if(GET_TYPE_SIZE(specType)==SIZE64_TYPE) 
			{	
				memoryCopy ( (uint8*)destPtr, (uint8*)setSpecDataPtr, 8);
			}
			else
			{
				status = FB_SPEC_TYPE_SIZE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				break;
			}
		break;

		case S_MEMORY:
		case M_MEMORY:
			
			if(specActionData.memoryType == S_MEMORY)
			{
				
				destPtr = (uint8*)gSTSharedMemDataLocalPtr;
			}
			
			
			else if(specActionData.memoryType == M_MEMORY)
			{
				
				destPtr = (uint8*)gMmemVariableLocalPtr;
			}
			
		
			if(specActionData.bitposFlag == 1)
			{
				if((specActionData.bitPosition < 0)) 
				{
					status = FB_SPEC_ACTION_BITPOS_ERR;
					break;
				} 
				else if((specActionData.dataSize==SPEC_SIZE_BYTE) && (specActionData.bitPosition < 8) )	
				{
					destPtr += memoryAddress * 1 ; 
					booldata = (uint8)(*setSpecDataPtr)&0x01;

					
					if(booldata ==1) 
						*destPtr |= (0x1 << specActionData.bitPosition);
					else if(booldata ==0) 
						*destPtr &= (~(0x1<< specActionData.bitPosition));
				} 
				else if((specActionData.dataSize==SPEC_SIZE_WORD) && (specActionData.bitPosition < 16 )) 
				{
					destPtr += memoryAddress * 2; 
					booldata = *setSpecDataPtr&0x01;
					
					wordData = *(uint16 *)destPtr;
					
					
					if(booldata ==1) 
						wordData |= (0x1 << specActionData.bitPosition);
					else if(booldata ==0) 
						wordData &= (~(0x1<< specActionData.bitPosition));
					
					memoryCopy( destPtr, (uint8 *)&wordData, 2);
				}
				else if((specActionData.dataSize==SPEC_SIZE_DWORD) && (specActionData.bitPosition < 32 )) 
				{
					destPtr += memoryAddress * 4 ; 
					booldata = *setSpecDataPtr&0x01;
					longData = *(uint32 *)destPtr;
					
					
					if(booldata ==1) 
						longData |= (0x1 << specActionData.bitPosition);
					else if(booldata ==0) 
						longData &= (~(0x1<< specActionData.bitPosition));
					
					memoryCopy( destPtr, (uint8 *)&longData, 4);
				}
#ifdef USED_BIT64
				else if((specActionData.dataSize==SPEC_SIZE_LWORD) && (specActionData.bitPosition < 64 )) 
				{
					destPtr += memoryAddress * 8 ; 
					booldata = *setSpecDataPtr&0x01;
					dLongData = *(uint64 *)destPtr;
					
					
					if(booldata ==1) 
						dLongData |= (0x1 << specActionData.bitPosition);
					else if(booldata ==0) 
						dLongData &= (~(0x1<< specActionData.bitPosition));
					
					memoryCopy( destPtr, (uint8 *)&dLongData, 8);
				} 
#endif
				else if((specActionData.dataSize==SPEC_SIZE_BIT) && (specActionData.bitPosition == 0 ) ) 
				{
					destPtr += memoryAddress/8 ; 
					booldata = *setSpecDataPtr&0x01;
					
					if(booldata ==1) 
						*destPtr |= (0x1 << specActionData.bitPosition);
					else if(booldata ==0) 
						*destPtr &= (~(0x1<< specActionData.bitPosition));
				} 
				else 
				{
					status = FB_SPEC_ACTION_BITPOS_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				}
			}

			else 
			{
				if(specActionData.dataSize == SPEC_SIZE_BYTE) 
				{
					destPtr += memoryAddress * 1 ; 
					uint8Data = (uint8)(*setSpecDataPtr & 0xff);
					memoryCopy ((uint8*)destPtr, (uint8*)&uint8Data, 1);
				} 
				else if(specActionData.dataSize == SPEC_SIZE_WORD) 
				{
					destPtr += memoryAddress * 2; 
					uint16Data = (uint16)(*setSpecDataPtr & 0xffff);
					memoryCopy ((uint8*)destPtr, (uint8*)&uint16Data, 2);
				} 
				else if(specActionData.dataSize == SPEC_SIZE_DWORD) 
				{
					destPtr += memoryAddress * 4 ; 
					memoryCopy ((uint8*)destPtr, (uint8*)setSpecDataPtr, 4);
				} 
				else if(specActionData.dataSize == SPEC_SIZE_LWORD) 
				{
					destPtr += memoryAddress * 8 ; 
					memoryCopy ((uint8*)destPtr, (uint8*)setSpecDataPtr, 8);
				} 
				
				else if(specActionData.dataSize == SPEC_SIZE_BIT) 
				{
					destPtr += memoryAddress/8 ;  
					
					booldata = (*setSpecDataPtr)&0x01; 

					if(booldata ==1) 
						*destPtr |= (0x1 << memoryAddress%8);
					else if(booldata ==0) 
						*destPtr &= (~(0x1 << memoryAddress%8));
				} 
				else 
				{
					status = FB_SPEC_ACTION_SIZE_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				}
			}
		break;

		default:
			status = VARIABLE_KIND_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Recv Memory Type", specActionData.memoryType,
								"Not Using",0,
								"Not Using", 0
							);
		break;
	}

	
	return(status);
}



uint32 refDataCopyToBusMem(	uint32 *setSpecDataPtr,
							uint32 specData,
						 	strFbSpecActionInfo specActionData, 
						 	uint32	specType
						 )
{
	uint32 status = NO_ERROR;
	uint32 memoryAddress,index;
	uint32 booldata = 0;
	uint8* destPtr= NULL, tempPtr;

    strFbTaskTblMemInfo	segInfo;
	uint32			fbBlkAddress = 0;
	strFbMemInfo	fbMemInfo;
	
	uint32 *uint32DestPtr = NULL;
	uint8 uint8Data; uint16 uint16Data; 
	uint8 	byteData;
	uint16	wordData;
	uint32	longData;
	uint64	dLongData;
	strSysConfigInfo 	sysCnfgInfo;
	int32 anaPointStartPosition, ioPointDigAnaKind;
	
	if(specActionData.refType != SPEC_TYPE_BYREF )
	{
		status = FB_SPEC_ACTION_REF_ERR;
		return(status);
	}

	systemCnfgInfoRead( &sysCnfgInfo);

	memoryAddress = specData>>8;
	index = (specData>>2)&0x3f; 

	switch(specActionData.memoryType) 
	{
		case F_MEMORY:
			
		break;

		case A_MEMORY:
			
			destPtr = (uint8*)((uint32 *)gAutoVariableBusPtr + (memoryAddress - 1) );

			if(GET_TYPE_SIZE(specType)==SIZE32_TYPE) 
			{			
				status = myBusDataAcc(__FILE__,__LINE__,BUS_LONG_ACC, 	BUS_DATA_WRITE ,(uint8*)destPtr, (uint8*)setSpecDataPtr );
				if(status) 
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			} 
			else if(GET_TYPE_SIZE(specType)==SIZE64_TYPE) 
			{	
				status = myBusDataAcc(__FILE__,__LINE__,BUS_DLONG_ACC, 	BUS_DATA_WRITE ,(uint8*)destPtr, (uint8*)setSpecDataPtr );
				if(status) 
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			} 
			else 
			{
				status = FB_SPEC_TYPE_SIZE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			}
		break;

		case S_MEMORY:
		case M_MEMORY:
			
			if(specActionData.memoryType==S_MEMORY)
				destPtr = (uint8*)gSTSharedMemDataBusPtr;
						
			else if(specActionData.memoryType==M_MEMORY) 
				destPtr = (uint8*)gMmemVariableBusPtr;
						
			if(specActionData.bitposFlag==1) 
			{
				if((specActionData.bitPosition < 0)) 
				{
					status = FB_SPEC_ACTION_BITPOS_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				} 
				else if((specActionData.dataSize==SPEC_SIZE_BYTE)
						&& (specActionData.bitPosition < 8) )
				{
					destPtr += memoryAddress * 1 ; 
					
					
					booldata = *setSpecDataPtr&0x01;

					
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_BYTE_ACC, 
											BUS_DATA_READ ,
											(uint8*)destPtr, 
											(uint8*)&byteData 
										);
					if(status) 
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						break;						
					}										
					if(booldata == 1) 
						byteData |= (0x1 << specActionData.bitPosition);
					else if(booldata == 0) 
						byteData &= (~(0x1<< specActionData.bitPosition));
						
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_BYTE_ACC, 
											BUS_DATA_WRITE,
											(uint8*)destPtr, 
											(uint8*)&byteData 
										 );
					if(status) 
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						break;						
					}
				} 
				else if((specActionData.dataSize==SPEC_SIZE_WORD)
						&& (specActionData.bitPosition < 16 ))
				{
					destPtr += memoryAddress * 2; 
					
					booldata = *setSpecDataPtr&0x01;

					
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_WORD_ACC, 
											BUS_DATA_READ ,
											(uint8*)destPtr, 
											(uint8*)&wordData 
										);
					if(status) 
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						break;						
					}										
					if(booldata == 1) 
						wordData |= (0x1 << specActionData.bitPosition);
					else if(booldata == 0) 
						wordData &= (~(0x1<< specActionData.bitPosition));
						
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_WORD_ACC, 
											BUS_DATA_WRITE,
											(uint8*)destPtr, 
											(uint8*)&wordData 
										 );
					if(status) 
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						break;						
					}
				} 
				else if((specActionData.dataSize==SPEC_SIZE_DWORD)
						&& (specActionData.bitPosition < 32 ) )
				{
					destPtr += memoryAddress * 4 ; 
					
					booldata = *setSpecDataPtr&0x01;

					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_LONG_ACC, 
											BUS_DATA_READ ,
											(uint8*)destPtr, 
											(uint8*)&longData 
										);
					if(status) 
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						break;						
					}	
														
					if(booldata == 1) 
						longData |= (0x1 << specActionData.bitPosition);
					else if(booldata == 0) 
						longData &= (~(0x1<< specActionData.bitPosition));
						
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_LONG_ACC, 
											BUS_DATA_WRITE,
											(uint8*)destPtr, 
											(uint8*)&longData 
										 );
					if(status) 
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						break;						
					}
				} 
				else if((specActionData.dataSize==SPEC_SIZE_LWORD)
						&& (specActionData.bitPosition < 64 ) )
				{
					destPtr += memoryAddress * 8 ; 
					
					booldata = *setSpecDataPtr&0x01;

					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_DLONG_ACC, 
											BUS_DATA_READ ,
											(uint8*)destPtr, 
											(uint8*)&dLongData 
										);
					if(status) 
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						break;						
					}	
														
					if(booldata == 1) 
						dLongData |= (0x1 << specActionData.bitPosition);
					else if(booldata == 0) 
						dLongData &= (~(0x1<< specActionData.bitPosition));
						
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_DLONG_ACC, 
											BUS_DATA_WRITE,
											(uint8*)destPtr, 
											(uint8*)&dLongData 
										 );
					if(status) 
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						break;						
					}
				} 
				else if((specActionData.dataSize==SPEC_SIZE_BIT)
						&& (specActionData.bitPosition == 0 ) )
				{
					destPtr += memoryAddress/8 ; 
					booldata = (uint8)(*setSpecDataPtr)&0x01;
					
					booldata = *setSpecDataPtr&0x01;

					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_BYTE_ACC, 
											BUS_DATA_READ ,
											(uint8*)destPtr, 
											(uint8*)&byteData 
										);
					if(status) 
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						break;						
					}	
														
					if(booldata == 1) 
						byteData |= (0x1 << specActionData.bitPosition);
					else if(booldata == 0) 
						byteData &= (~(0x1<< specActionData.bitPosition));
						
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_BYTE_ACC, 
											BUS_DATA_WRITE,
											(uint8*)destPtr, 
											(uint8*)&byteData 
										 );
					if(status) 
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						break;						
					}
				} 
				else 
				{
					status = FB_SPEC_ACTION_BITPOS_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				}
			}

			else
			{
				if(specActionData.dataSize==SPEC_SIZE_BYTE) 
				{
					destPtr 	+= memoryAddress * 1 ; 
					uint8Data 	= (uint8)(*setSpecDataPtr & 0xff);
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_BYTE_ACC, 	
											BUS_DATA_WRITE ,
											(uint8*)destPtr, 
											(uint8*)&uint8Data
										 );
					if(status) 
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						break;						
					}										
			
				} 
				else if(specActionData.dataSize==SPEC_SIZE_WORD) 
				{
					destPtr += memoryAddress * 2; 
					uint16Data = (uint16)(*setSpecDataPtr & 0xffff);
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_WORD_ACC, 	
											BUS_DATA_WRITE ,
											(uint8*)destPtr, 
											(uint8*)&uint16Data 
										);
					if(status) 
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						break;						
					}										
				} 
				else if(specActionData.dataSize==SPEC_SIZE_DWORD) 
				{
					destPtr += memoryAddress * 4 ; 
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_LONG_ACC, 	
											BUS_DATA_WRITE ,
											(uint8*)destPtr, 
											(uint8*)setSpecDataPtr 
										);
					if(status) 
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						break;						
					}										
				} 
				else if(specActionData.dataSize==SPEC_SIZE_LWORD) 
				{
					destPtr += memoryAddress * 8 ; 
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_DLONG_ACC, 	
											BUS_DATA_WRITE ,
											(uint8*)destPtr, 
											(uint8*)setSpecDataPtr 
										);
					if(status) 
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						break;						
					}										
				} 				
				else if(specActionData.dataSize==SPEC_SIZE_BIT) 
				{
					destPtr += memoryAddress/8 ;  
					
					booldata = (*setSpecDataPtr)&0x01; 
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_BYTE_ACC, 	
											BUS_DATA_READ ,
											(uint8*)destPtr, 
											(uint8*)&tempPtr 
										);
					if(status) 
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						break;						
					}										
					
					if(booldata == 1) 
						tempPtr |= (0x1 <<memoryAddress%8);
					else if(booldata == 0) 
						tempPtr &= (~(0x1<< memoryAddress%8));
					
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_BYTE_ACC, 	
											BUS_DATA_WRITE ,
											(uint8*)destPtr, 
											(uint8*)&tempPtr 
										);
					if(status) 
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						break;						
					}										

				} 
				else 
				{
					status = FB_SPEC_ACTION_SIZE_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				}
			}

		break;

		default:
			status = VARIABLE_KIND_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		break;
	}

	return(status);
}


uint32 refDataCopyToRmtMem(	uint32 *setSpecDataPtr,
							uint32 specData,
						 	strFbSpecActionInfo specActionData, 
						 	uint32	specType
						 )
{
	uint32 status = NO_ERROR;
	uint32 memoryAddress,index;
	uint32 booldata = 0;
	uint8 *destPtr= NULL, *BusPtr= NULL, tempData = 0;
	uint32	offSet, bitMask;

    strFbTaskTblMemInfo	segInfo;
	uint32			fbBlkAddress = 0;
	strFbMemInfo	fbMemInfo;
	
	uint32 *uint32DestPtr=NULL;
	uint8 uint8Data; uint16 uint16Data; 
	uint8 *writeSrcPtr=NULL;
	uint32 writeType =0;
	uint8	byteData;
	uint16	wordData;
	uint32	longData;
	uint64	dLongData;
	strSysConfigInfo 	sysCnfgInfo;
	int32 anaPointStartPosition, ioPointDigAnaKind;

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	uint32 retainInfo=0,backupFlag;
	strAccumBusInfo sAccumRefData;
	
	sAccumRefData.accDataNum = 0;
#endif	
	
	if(gSysCnfgInfo.shelfType == MASTER_SHELF)
       	offSet = MEM_BOARD_BUS_ADDR_ON_SLAVE;
    else if(gSysCnfgInfo.shelfType == SLAVE_SHELF) 
       	offSet = MEM_BOARD_BUS_ADDR_ON_MASTER;
    else 
    {
		status = STATION_SHELF_CONFIG_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
        return(status);
    }

	if(specActionData.refType !=SPEC_TYPE_BYREF )
	{
		status = FB_SPEC_ACTION_REF_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	systemCnfgInfoRead( &sysCnfgInfo);
	
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	backupFlag = (EN_SAVE<<16);
	if( (specActionData.memoryType == A_MEMORY) || (specActionData.memoryType==M_MEMORY) )
	{
		status = varRetainInfoReadwithRef(specData, specActionData, (uint32*)&retainInfo);
#if 0 
		if((status==NO_ERROR)&&(retainInfo==NO_RETAIN))
			backupFlag = (NO_SAVE<<16);
#endif
	}
#endif	
	memoryAddress = specData>>8;
	index = (specData>>2)&0x3f; 

    
    
    
    bitMask = 0x00ffffff;
	
	switch(specActionData.memoryType) 
	{
		case A_MEMORY:
			
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			destPtr = (uint8*)  ( ( ( ((uint32)gAutoVariableBusPtr - (uint32)gBusMpuFbStartAddr) 
													+ (memoryAddress-1)*DOUBLE_SIZE ) & bitMask )+offSet 
								); 
#endif
			if(GET_TYPE_SIZE(specType)==SIZE32_TYPE) 
			{			
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
				sAccumRefData.accumPtr[sAccumRefData.accDataNum] =  (uint32*)destPtr;
				sAccumRefData.accumData[sAccumRefData.accDataNum] = (uint32)*setSpecDataPtr;
				sAccumRefData.accumType[sAccumRefData.accDataNum] = backupFlag|BUS_LONG_ACC;
				sAccumRefData.accDataNum++;
#endif
				if(status)
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			} 
			else if(GET_TYPE_SIZE(specType)==SIZE64_TYPE) 
			{	
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
				sAccumRefData.accumPtr[sAccumRefData.accDataNum] =  (uint32*)destPtr;
				sAccumRefData.accumData[sAccumRefData.accDataNum] = (uint32)*setSpecDataPtr;
				sAccumRefData.accumType[sAccumRefData.accDataNum] = backupFlag|BUS_LONG_ACC;
				sAccumRefData.accDataNum++;

				sAccumRefData.accumPtr[sAccumRefData.accDataNum] =  (uint32*)(destPtr+4);
				sAccumRefData.accumData[sAccumRefData.accDataNum] = (uint32)(*(++setSpecDataPtr));
				sAccumRefData.accumType[sAccumRefData.accDataNum] = backupFlag|BUS_LONG_ACC;
				sAccumRefData.accDataNum++;

#endif
				if(status)
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

			} 
			else 
			{
				status = FB_SPEC_TYPE_SIZE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				break;
			}
		break;

		case S_MEMORY:
		case M_MEMORY:
			if(specActionData.memoryType == S_MEMORY) 
			{
				
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
				destPtr = (uint8*)(( ((uint32)gSTSharedMemDataBusPtr -(uint32)gBusMpuFbStartAddr )&bitMask)+offSet );
#endif
				BusPtr = (uint8*)gSTSharedMemDataBusPtr;
			}
			else if(specActionData.memoryType == M_MEMORY) 
			{
				
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
				destPtr = (uint8*)(( ((uint32)gMmemVariableBusPtr -(uint32)gBusMpuFbStartAddr )&bitMask)+offSet );
#endif
				BusPtr = (uint8*)gMmemVariableBusPtr;
			}			
			
			if(specActionData.bitposFlag==1) 
			{
				if((specActionData.bitPosition < 0)) 
				{
					status = FB_SPEC_ACTION_BITPOS_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				} 
				else if((specActionData.dataSize==SPEC_SIZE_BYTE)
						&& (specActionData.bitPosition < 8) )
				{
					BusPtr += memoryAddress * 1 ;
					destPtr += memoryAddress * 1 ; 

					
					booldata = *setSpecDataPtr&0x01;
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_BYTE_ACC, 	
											BUS_DATA_READ ,
											(uint8*)BusPtr, 
											(uint8*)&byteData 
										 );
					if(status)
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

					if(booldata ==1) 
						byteData |= (0x1 << specActionData.bitPosition);
					else if(booldata ==0) 
						byteData &= (~(0x1<< specActionData.bitPosition));

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
					writeSrcPtr = (uint8*)&byteData;

					writeType = BUS_BYTE_ACC;
					sAccumRefData.accumPtr[sAccumRefData.accDataNum] = (uint32*)destPtr;
					memoryCopy((uint8*)&sAccumRefData.accumData[sAccumRefData.accDataNum],
						(uint8*)writeSrcPtr, writeType );
					sAccumRefData.accumType[sAccumRefData.accDataNum] = backupFlag|writeType;
					sAccumRefData.accDataNum++;

					if( writeType == BUS_DLONG_ACC)
					 {
						writeType = BUS_LONG_ACC;
						sAccumRefData.accumType[sAccumRefData.accDataNum] = backupFlag|writeType;
						sAccumRefData.accDataNum++;
						sAccumRefData.accumPtr[sAccumRefData.accDataNum] = (uint32*)(destPtr+4);
						memoryCopy((uint8*)&sAccumRefData.accumData[sAccumRefData.accDataNum],
							(uint8*)(writeSrcPtr+4), writeType );
					}
#endif
				} 
				else if((specActionData.dataSize==SPEC_SIZE_WORD)
						&& (specActionData.bitPosition < 16 ))
				{
					BusPtr += memoryAddress * 2 ;
					destPtr += memoryAddress * 2; 
					
					booldata = *setSpecDataPtr&0x01;
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_WORD_ACC, 	
											BUS_DATA_READ ,
											(uint8*)BusPtr, 
											(uint8*)&wordData 
										 );
					if(status)
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

					if(booldata ==1) 
						wordData |= (0x1 << specActionData.bitPosition);
					else if(booldata ==0) 
						wordData &= (~(0x1<< specActionData.bitPosition));

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
					writeSrcPtr = (uint8*)&wordData;

					writeType = BUS_WORD_ACC;
					sAccumRefData.accumPtr[sAccumRefData.accDataNum] = (uint32*)destPtr;
					memoryCopy((uint8*)&sAccumRefData.accumData[sAccumRefData.accDataNum],
						(uint8*)writeSrcPtr, writeType );
					sAccumRefData.accumType[sAccumRefData.accDataNum] = backupFlag|writeType;
					sAccumRefData.accDataNum++;

					if( writeType == BUS_DLONG_ACC) 
					{
						writeType = BUS_LONG_ACC;
						sAccumRefData.accumType[sAccumRefData.accDataNum] = backupFlag|writeType;
						sAccumRefData.accDataNum++;
						sAccumRefData.accumPtr[sAccumRefData.accDataNum] = (uint32*)(destPtr+4);
						memoryCopy((uint8*)&sAccumRefData.accumData[sAccumRefData.accDataNum],
							(uint8*)(writeSrcPtr+4), writeType );
					}
#endif
				} 
				else if((specActionData.dataSize==SPEC_SIZE_DWORD)
						&& (specActionData.bitPosition < 32 ) )
				{
					BusPtr += memoryAddress * 4 ;
					destPtr += memoryAddress * 4 ; 
					
					
					booldata = *setSpecDataPtr&0x01;
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_LONG_ACC, 	
											BUS_DATA_READ ,
											(uint8*)BusPtr, 
											(uint8*)&longData 
										 );
					if(status)
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

					if(booldata ==1) 
						longData |= (0x1 << specActionData.bitPosition);
					else if(booldata ==0) 
						longData &= (~(0x1<< specActionData.bitPosition));

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
					writeSrcPtr = (uint8*)&longData;

					writeType = BUS_LONG_ACC;
					sAccumRefData.accumPtr[sAccumRefData.accDataNum] = (uint32*)destPtr;
					memoryCopy((uint8*)&sAccumRefData.accumData[sAccumRefData.accDataNum],
						(uint8*)writeSrcPtr, writeType );
					sAccumRefData.accumType[sAccumRefData.accDataNum] = backupFlag|writeType;
					sAccumRefData.accDataNum++;

					if( writeType == BUS_DLONG_ACC) 
					{
						writeType = BUS_LONG_ACC;
						sAccumRefData.accumType[sAccumRefData.accDataNum] = backupFlag|writeType;
						sAccumRefData.accDataNum++;
						sAccumRefData.accumPtr[sAccumRefData.accDataNum] = (uint32*)(destPtr+4);
						memoryCopy((uint8*)&sAccumRefData.accumData[sAccumRefData.accDataNum],
							(uint8*)(writeSrcPtr+4), writeType );
					}
#endif
				} 
				else if((specActionData.dataSize==SPEC_SIZE_LWORD)
						&& (specActionData.bitPosition < 64 ) )
				{
					BusPtr += memoryAddress * 8 ;
					destPtr += memoryAddress * 8 ; 
					
					booldata = *setSpecDataPtr&0x01;
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_DLONG_ACC, 	
											BUS_DATA_READ ,
											(uint8*)BusPtr, 
											(uint8*)&dLongData 
										 );
					if(status)
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

					if(booldata ==1) 
						dLongData |= (0x1 << specActionData.bitPosition);
					else if(booldata ==0) 
						dLongData &= (~(0x1<< specActionData.bitPosition));

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
					writeSrcPtr = (uint8*)&dLongData;

					writeType = BUS_DLONG_ACC;
					sAccumRefData.accumPtr[sAccumRefData.accDataNum] = (uint32*)destPtr;
					
					memoryCopy((uint8*)&sAccumRefData.accumData[sAccumRefData.accDataNum],
						(uint8*)writeSrcPtr, writeType );
					
					sAccumRefData.accumType[sAccumRefData.accDataNum] = backupFlag|writeType;
					
					sAccumRefData.accDataNum++;

					if( writeType == BUS_DLONG_ACC) 
					{
						writeType = BUS_LONG_ACC;
						sAccumRefData.accumType[sAccumRefData.accDataNum] = backupFlag|writeType;
					
						sAccumRefData.accDataNum++;
					
						sAccumRefData.accumPtr[sAccumRefData.accDataNum] = (uint32*)(destPtr+4);
					
						memoryCopy((uint8*)&sAccumRefData.accumData[sAccumRefData.accDataNum],
							(uint8*)(writeSrcPtr+4), writeType );
					}
#endif
				} 
				else if((specActionData.dataSize==SPEC_SIZE_BIT)
						&& (specActionData.bitPosition == 0 ) )
				{
					BusPtr += memoryAddress/8 ;
					destPtr += memoryAddress/8 ; 
					booldata = (*setSpecDataPtr)>>memoryAddress&0x01;

					
					booldata = *setSpecDataPtr&0x01;
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_BYTE_ACC, 	
											BUS_DATA_READ ,
											(uint8*)BusPtr, 
											(uint8*)&byteData 
										 );
					if(status)
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

					if(booldata ==1) 
						byteData |= (0x1 << specActionData.bitPosition);
					else if(booldata ==0) 
						byteData &= (~(0x1<< specActionData.bitPosition));
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
					writeSrcPtr = (uint8*)&byteData;

					writeType = BUS_BYTE_ACC;
					sAccumRefData.accumPtr[sAccumRefData.accDataNum] = (uint32*)destPtr;
					memoryCopy((uint8*)&sAccumRefData.accumData[sAccumRefData.accDataNum],
						(uint8*)writeSrcPtr, writeType );
					sAccumRefData.accumType[sAccumRefData.accDataNum] = backupFlag|writeType;
					sAccumRefData.accDataNum++;

					if( writeType == BUS_DLONG_ACC) 
					{
						writeType = BUS_LONG_ACC;
						sAccumRefData.accumType[sAccumRefData.accDataNum] = backupFlag|writeType;
						sAccumRefData.accDataNum++;
						sAccumRefData.accumPtr[sAccumRefData.accDataNum] = (uint32*)(destPtr+4);
						memoryCopy((uint8*)&sAccumRefData.accumData[sAccumRefData.accDataNum],
							(uint8*)(writeSrcPtr+4), writeType );
					}
#endif
				} 
				else 
				{
					status = FB_SPEC_ACTION_BITPOS_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				}
			}

			else
			{
				if(specActionData.dataSize==SPEC_SIZE_BYTE) 
				{
					destPtr += memoryAddress * 1 ; 
					uint8Data = (uint8)(*setSpecDataPtr & 0xff);
					writeSrcPtr = (uint8*)&uint8Data;
					writeType = BUS_BYTE_ACC;
				} 
				else if(specActionData.dataSize==SPEC_SIZE_WORD) 
				{
					destPtr += memoryAddress * 2; 
					uint16Data = (uint16)(*setSpecDataPtr & 0xffff);
					writeSrcPtr = (uint8*)&uint16Data;
					writeType = BUS_WORD_ACC;
				} 
				else if(specActionData.dataSize==SPEC_SIZE_DWORD)
				{
					destPtr += memoryAddress * 4 ; 
					writeSrcPtr = (uint8*)setSpecDataPtr;
					writeType = BUS_LONG_ACC;
				} 
				else if(specActionData.dataSize==SPEC_SIZE_LWORD) 
				{
					destPtr += memoryAddress * 8 ; 
					writeSrcPtr = (uint8*)setSpecDataPtr;
					writeType = BUS_DLONG_ACC;
				} 
				
				else if(specActionData.dataSize==SPEC_SIZE_BIT) 
				{
					BusPtr += memoryAddress/8 ;
					destPtr += memoryAddress/8 ;  
					
					booldata = (*setSpecDataPtr)&0x01; 

					status = myBusDataAcc(__FILE__,__LINE__,BUS_BYTE_ACC, 	BUS_DATA_READ ,(uint8*)BusPtr, (uint8*)&tempData );
				
					if(status)
					{
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
						break;
					}
					
					if(booldata ==1) 
						tempData |= (0x1 <<memoryAddress%8);
					
					else if(booldata ==0) 
						tempData &= (~(0x1<< memoryAddress%8));
					
					writeSrcPtr = (uint8*)&tempData;
					writeType = BUS_BYTE_ACC;
				} 
				else 
				{
					status = FB_SPEC_ACTION_SIZE_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				}

				if(status == NO_ERROR)
				{
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
					sAccumRefData.accumPtr[sAccumRefData.accDataNum] = (uint32*)destPtr;
					memoryCopy((uint8*)&sAccumRefData.accumData[sAccumRefData.accDataNum],
						(uint8*)writeSrcPtr, writeType );
					sAccumRefData.accumType[sAccumRefData.accDataNum] = backupFlag|writeType;
					sAccumRefData.accDataNum++;

					if( writeType == BUS_DLONG_ACC) 
					{
						writeType = BUS_LONG_ACC;
						sAccumRefData.accumType[sAccumRefData.accDataNum] = backupFlag|writeType;
						sAccumRefData.accDataNum++;
						sAccumRefData.accumPtr[sAccumRefData.accDataNum] = (uint32*)(destPtr+4);
						memoryCopy((uint8*)&sAccumRefData.accumData[sAccumRefData.accDataNum],
							(uint8*)(writeSrcPtr+4), writeType );
					}
#endif				
				}
			}

		break;
		
		case F_MEMORY:
			
		default:
			status = VARIABLE_KIND_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		break;
	}

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	
	status = myRmtRefRunDataWrite((strAccumBusInfo*)&sAccumRefData);
	if(status != NO_ERROR)
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
#endif

	return(status);
}


uint32  localVariableRetainInfoWrite(uint32 variableKind, uint32 variableIndex, uint8 flag)
{
	uint32	status = NO_ERROR;
	uint32   *retainIndexPtr = NULL;
	strMemPointerInfo	memPointer;

	status = fbMemPointerGet(LOGIC_ID, &memPointer);
    if(status != NO_ERROR) 
    {
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	if(variableKind == AUTO_VAR) 
	{
		/* 자동 변수인 경우는 Index가 '1'부터 시작 */
		variableIndex = variableIndex-1; 
		retainIndexPtr = (uint32*)memPointer.retainMemPtr;

		if (variableIndex > MAX_MPU_AUTOVAR_NUM) 
		{
			status = VARIABLE_INDEX_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Recv variable Index",variableIndex,
								"MAX AUTO VAR NUM", MAX_MPU_AUTOVAR_NUM,
								"Not Using", 0
							  );
		}
	}
	else if(variableKind == MMEM_VAR) 
	{
		
		retainIndexPtr = (uint32*)memPointer.retainMemPtr + MAX_MPU_AUTOVAR_NUM / 32 ; 
		if  ( (variableIndex < 0 ) ||(variableIndex > MAX_MPU_MMEM_NUM) ) 
		{
			status = VARIABLE_INDEX_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Recv variable Index",variableIndex,
								"MAX M_VAR NUM", MAX_MPU_MMEM_NUM,
								"Not Using", 0
								);
		}
	}
	else 
	{		
		status = VARIABLE_KIND_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Recv variable Kind",variableIndex,
							"Define AUTO_VAR ", AUTO_VAR,
							"Define MMEM_VAR", MMEM_VAR
						  );
	}
	
	if (status != NO_ERROR) return(status);

	if (flag ==EN_RETAIN )
	{
		retainIndexPtr = retainIndexPtr + variableIndex/32  ;
		*retainIndexPtr |= 0x1<<variableIndex%32  ; 
	}
	else if(flag ==NO_RETAIN)
	{
		retainIndexPtr = retainIndexPtr + variableIndex/32  ;
		*retainIndexPtr &= (~(0x1<<variableIndex%32) ) ; 
	}
	else 
	{
		status = VARIABLE_RETAIN_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Var Kind", variableKind,
							"Var Index",variableIndex,
							"Retain Flag", flag
						);
	}

    return(status);
}


uint32  busVariableRetainInfoWrite(uint32 variableKind, uint32 variableIndex, uint8 flag)
{
	uint32	status = NO_ERROR;
	uint32   *retainIndexPtr = NULL;
	strMemPointerInfo	memPointer;

	status = busFbMemPointerGet(LOGIC_ID, &memPointer);
    if(status != NO_ERROR) 
    {
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	if(variableKind == AUTO_VAR) 
	{
		
		variableIndex = variableIndex-1; 
		retainIndexPtr = (uint32*)memPointer.retainMemPtr;
		if ( (variableIndex < 0 ) || (variableIndex >= MAX_MPU_AUTOVAR_NUM) )
		{
			status = VARIABLE_INDEX_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Recv variable Index",variableIndex,
								"MAX AUTO VAR NUM", MAX_MPU_AUTOVAR_NUM,
								"Not Using", 0
							  );
		}
	}
	else if(variableKind == MMEM_VAR) 
	{	
		retainIndexPtr = (uint32*)memPointer.retainMemPtr + MAX_MPU_AUTOVAR_NUM / 32 ; 
		
		if ( (variableIndex < 0 ) ||(variableIndex > MAX_MPU_MMEM_NUM))  
		{
			status = VARIABLE_INDEX_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Recv variable Index",variableIndex,
								"MAX M_VAR NUM", MAX_MPU_MMEM_NUM,
								"Not Using", 0
							  );
		}
	}
	else 
	{
		
		status = VARIABLE_KIND_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Recv variable Kind",variableIndex,
							"Define AUTO_VAR ", AUTO_VAR,
							"Define MMEM_VAR", MMEM_VAR
						  );
	}

	if (status != NO_ERROR) 
		return(status);

	if (flag ==EN_RETAIN )
	{
		retainIndexPtr = retainIndexPtr + variableIndex/32  ;
		*retainIndexPtr |= 0x1<<variableIndex%32  ; 
	}
	else if(flag ==NO_RETAIN)
	{
		retainIndexPtr = retainIndexPtr + variableIndex/32  ;
		*retainIndexPtr ^= 0x1<<variableIndex%32  ; 
	}
	else 
	{
		status = VARIABLE_RETAIN_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Var Kind", variableKind,
							"Var Index",variableIndex,
							"Retain Flag", flag
						  );
	}

    return(status);
}


uint32  busVariableRetainInfoRead(uint32 variableKind, uint32 variableIndex, uint8* flag)
{
	uint32	status = NO_ERROR;
	uint32   *retainIndexPtr = NULL;
	strMemPointerInfo	memPointer;

	*flag = NO_RETAIN;

	status = busFbMemPointerGet(LOGIC_ID, &memPointer);
    if(status != NO_ERROR) 
    {
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	if(variableKind == AUTO_VAR) 
	{
		
		variableIndex = variableIndex - 1;
		retainIndexPtr = (uint32*)memPointer.retainMemPtr;
		if (variableIndex > MAX_MPU_AUTOVAR_NUM) {
			status = VARIABLE_INDEX_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		}
	}
	else if(variableKind == MMEM_VAR) 
	{
		retainIndexPtr = (uint32*)memPointer.retainMemPtr + MAX_MPU_AUTOVAR_NUM / 32 ; 
		if (variableIndex > MAX_MPU_MMEM_NUM)
		{
			status = VARIABLE_INDEX_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		}
	}
	else 
	{
		status = VARIABLE_KIND_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
	}
	
	if (status != NO_ERROR) 
		return(status);

	retainIndexPtr = retainIndexPtr + variableIndex/32  ;
	*flag = ( (*retainIndexPtr) >> variableIndex%32 ) &0x1;

	if((*flag != EN_RETAIN) && (*flag != NO_RETAIN)) 
	{
		 status = VARIABLE_RETAIN_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
	}

    return(status);
}


uint32  localVariableRetainInfoRead(uint32 variableKind, uint32 variableIndex, uint8* flag)
{
	uint32	status = NO_ERROR;
	uint32   *retainIndexPtr=NULL;
	strMemPointerInfo	memPointer;

	*flag = NO_RETAIN;

	status = fbMemPointerGet(LOGIC_ID, &memPointer);
    if(status != NO_ERROR) 
    {
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	if(variableKind == AUTO_VAR) 
	{
		
		retainIndexPtr = (uint32*)memPointer.retainMemPtr;
		if (variableIndex > MAX_MPU_AUTOVAR_NUM)  
		{
			status = VARIABLE_INDEX_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		}
	}
	else if(variableKind == MMEM_VAR) 
	{
		retainIndexPtr = (uint32*)memPointer.retainMemPtr + MAX_MPU_AUTOVAR_NUM / 32 ; 
		if (variableIndex > MAX_MPU_MMEM_NUM)
		{
			status = VARIABLE_INDEX_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		}
	}
	else 
	{
		status = VARIABLE_KIND_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
	}

	if (status != NO_ERROR) 
	{
		return(status);
	}

	retainIndexPtr = retainIndexPtr + variableIndex/32  ;
	*flag = ( (*retainIndexPtr) >> variableIndex%32 ) &0x1;

	if((*flag !=EN_RETAIN) && (*flag !=NO_RETAIN)) 
	{
		status = VARIABLE_RETAIN_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
	}

    return(status);
}


uint32  varRetainInfoReadwithRef(uint32 specData, strFbSpecActionInfo specActionData,
						  uint32* retainFlag)
{
	uint32 status = NO_ERROR;
	uint32 memoryAddress,index;
    uint32 *retainIndexPtr;
	strMemPointerInfo	memPointer;
	uint32 specType;

	*retainFlag = NO_RETAIN;

	if(specActionData.refType !=SPEC_TYPE_BYREF )
	{
		status = FB_SPEC_ACTION_REF_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	if( (specActionData.dataType == SPEC_DATA_TYPE_LINT) || (specActionData.dataType == SPEC_DATA_TYPE_LREAL) || 
		(specActionData.dataType ==SPEC_DATA_TYPE_ULINT) || (specActionData.dataSize == SPEC_SIZE_LWORD) ||
		(specActionData.dataType ==SPEC_DATA_TYPE_DT)) 
	{
		specType = SIZE64_TYPE;
	}
	else 
		specType = SIZE32_TYPE;

	memoryAddress = specData>>8;
	index = (specData>>2)&0x3f; 

	status = fbMemPointerGet(LOGIC_ID, &memPointer);
    if(status != NO_ERROR) 
    {
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}
	
	switch(specActionData.memoryType) 
	{	
		case A_MEMORY:
			retainIndexPtr = (uint32*)memPointer.retainMemPtr;
			if((memoryAddress<1)|| (memoryAddress>MAX_MPU_AUTOVAR_NUM) || (index != 0x0) )
			{
				status = FB_SPEC_ACTION_ADDR_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				break;
			}
			
			if(GET_TYPE_SIZE(specType)==SIZE32_TYPE) 
			{			
				retainIndexPtr += (memoryAddress/32); 
				*retainFlag = (uint8)( (*retainIndexPtr >> ((memoryAddress-1)%32))& 0x1) ;

			} 
			else if(GET_TYPE_SIZE(specType)==SIZE64_TYPE) 
			{	
				retainIndexPtr += (memoryAddress/16); 
				*retainFlag = (uint8)( ( (*retainIndexPtr >> ((memoryAddress-1)%16))& 0x1 << 4 ) ) ;
				*retainFlag |= (uint8) ( (*retainIndexPtr >> (((memoryAddress-1)%16)+1))& 0x1)   ;
			} 
			else 
			{
				status = FB_SPEC_TYPE_SIZE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				break;
			}
		break;

		
		case M_MEMORY:
			retainIndexPtr = (uint32*)memPointer.retainMemPtr + MAX_MPU_AUTOVAR_NUM / 32 ; 
			
			if(specActionData.dataSize==SPEC_SIZE_BYTE) 
			{
				retainIndexPtr += memoryAddress /(32*4) ;			
				*retainFlag = (uint8)( (*retainIndexPtr >> ((memoryAddress/4)%32))& 0x1) ;

			} 
			else if(specActionData.dataSize==SPEC_SIZE_WORD) 
			{
				retainIndexPtr += memoryAddress /(32*2);			
				*retainFlag = (uint8)( (*retainIndexPtr >> ((memoryAddress/2)%32))& 0x1) ;

			} 
			else if(specActionData.dataSize==SPEC_SIZE_DWORD) 
			{
				retainIndexPtr += memoryAddress/32 ;				
				*retainFlag = (uint8)( (*retainIndexPtr >> (memoryAddress%32))& 0x1) ;

			} 
			else if(specActionData.dataSize==SPEC_SIZE_LWORD) 
			{
				retainIndexPtr += (memoryAddress/(32/2));			
				*retainFlag = (uint8)( ( (*retainIndexPtr >> (memoryAddress%16))& 0x1 << 4 ) ) ;
				*retainFlag |= (uint8) ( (*retainIndexPtr >> ((memoryAddress%16)+1))& 0x1 )  ;
			} 
			else if(specActionData.dataSize==SPEC_SIZE_BIT) 
			{
				retainIndexPtr += memoryAddress/(32*4*8) ;			
				*retainFlag = (uint8)( (*retainIndexPtr >> ((memoryAddress/32)%32))& 0x1) ;

			} 
			else 
			{
				status = FB_SPEC_ACTION_SIZE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				break;
			}

		break;

		case S_MEMORY:
		case Q_MEMORY:
		case T_MEMORY:				 
		case P_MEMORY:				 
			*retainFlag = EN_RETAIN;  
		break;
	
		case I_MEMORY:	
			
		case F_MEMORY:
		default:
			status = VARIABLE_KIND_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			*retainFlag = NO_RETAIN;
		break;
	}

	return(status);
}



uint32 variableDataRead(uint32 *retVarDataPtr,uint32 varKind,
						 uint32 varAddress, uint32 varType)
{
	uint32 status = NO_ERROR;
	strMemPointerInfo	memPointer;
	uint8* srcPtr = NULL;
	uint32 int32Buf;

	status = fbMemPointerGet(LOGIC_ID, &memPointer);

    if(status != NO_ERROR) 
    {
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	switch(varKind) 
	{
		case AUTO_VAR:
			if(varAddress >= 1) 
			{
				srcPtr = ((uint8 *)memPointer.autoMemPtr);
				varAddress = varAddress - 1;
			}								
			else if( (varType != SPEC_SIZE_DWORD) || (varType != SPEC_SIZE_LWORD) ) 
			{
				status = VARIABLE_TYPE_ERR;
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"Var Region" ,varKind,
									"Var Type",varType, 
									"NOT Using",0
								   );	
				return(status);
			}

		break;

		case MMEM_VAR:		
			srcPtr = ((uint8 *)memPointer.flagMemPtr);
		break;

		case SMEM_VAR:
			srcPtr = ((uint8 *)memPointer.stSharedMemPtr);
		break;

		default:
			status = VARIABLE_KIND_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Var Region" ,varKind,
								"Var Addr",varAddress, 
								"Var Type",varType
							);	
			return(status);
		break;
	}

	switch(varType) 
	{
		case SPEC_SIZE_BIT:
			srcPtr += (varAddress/8) * 1 ; 
			int32Buf =(uint32)(*(uint8*)srcPtr);
			int32Buf = int32Buf >> (varAddress%8);
			fourBytesDataCopy((uint32*)retVarDataPtr, (uint32*)&int32Buf ,1);
		break;


		case SPEC_SIZE_BYTE:
			srcPtr += varAddress * 1 ; 
			int32Buf =(uint32)(*(uint8*)srcPtr);
			fourBytesDataCopy((uint32*)retVarDataPtr, (uint32*)&int32Buf ,1);
		break;

		case SPEC_SIZE_WORD:		
			srcPtr += varAddress * 2; 
			
			int32Buf =(uint32)(*(uint16*)srcPtr);
			fourBytesDataCopy((uint32*)retVarDataPtr, (uint32*)&int32Buf ,1);
		break;

		case SPEC_SIZE_DWORD:
			srcPtr += varAddress * 4 ; 
			memoryCopy ((uint8*)retVarDataPtr, (uint8*)srcPtr, 4);
		break;

		case SPEC_SIZE_LWORD:
			srcPtr += varAddress * 8 ; 
			eightByteConversion((uint32*)retVarDataPtr,(uint32*)srcPtr,0);
		break;

		default:
			status = VARIABLE_KIND_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Var Region" ,varKind,
								"Var Addr",varAddress, 
								"Var Type",varType
							);	
			return(status);
		break;
	}

	return(status);
}


uint32 variableDataWrite(uint32 *setVarDataPtr,uint32 varKind,
						 uint32 varAddress, uint32 varType)
{
	uint32 status = NO_ERROR;
	strMemPointerInfo	memPointer;
	uint8* destPtr = NULL, uint8Data ;
	uint16 uint16Data; 

	status = fbMemPointerGet(LOGIC_ID, &memPointer);

    if(status != NO_ERROR) 
    {
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	switch(varKind) 
	{
		case AUTO_VAR:
			if(varAddress >= 1) 
			{
				
				destPtr = ((uint8 *)memPointer.autoMemPtr);
				varAddress = varAddress - 1;
			}
			else if( (varType != SPEC_SIZE_DWORD) || (varType != SPEC_SIZE_LWORD) ) 
			{
				status = VARIABLE_TYPE_ERR;
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"Var Region" ,varKind,
									"Var Type",varType, 
									"NOT Using",0
									);	
				return(status);
			}
		break;

		case MMEM_VAR:		
			destPtr = ((uint8 *)memPointer.flagMemPtr);
		break;

		case SMEM_VAR:
			destPtr = ((uint8 *)memPointer.stSharedMemPtr);
		break;

		default:
			status = VARIABLE_KIND_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Var Region" ,varKind,
								"Var Addr",varAddress, 
								"Var Type",varType
							);	
			return(status);
		break;
	}

	switch(varType) 
	{
		case SPEC_SIZE_BIT:
			destPtr += (varAddress/8) * 1 ; 
			uint8Data = (uint8)(*setVarDataPtr & 0x1);
			if(uint8Data == 1) *destPtr |= (0x1 << (varAddress%8)); 
			else  *destPtr &= (~(0x1 << (varAddress%8)));
		break;

		case SPEC_SIZE_BYTE:
			destPtr += varAddress * 1 ; 
			uint8Data = (uint8)(*setVarDataPtr & 0xff);
			memoryCopy ((uint8*)destPtr, (uint8*)&uint8Data, 1);
		break;

		case SPEC_SIZE_WORD:		
			destPtr += varAddress * 2; 
			
			uint16Data = (uint16)(*setVarDataPtr & 0xffff);
			memoryCopy ((uint8*)destPtr, (uint8*)&uint16Data, 2);
		break;

		case SPEC_SIZE_DWORD:
			destPtr += varAddress * 4 ; 
			memoryCopy ((uint8*)destPtr, (uint8*)setVarDataPtr, 4);
		break;

		case SPEC_SIZE_LWORD:
			destPtr += varAddress * 8 ; 
			eightByteConversion((uint32*)destPtr,(uint32*)setVarDataPtr,0);
		break;


		default:
			status = VARIABLE_KIND_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Var Region" ,varKind,
								"Var Addr",varAddress, 
								"Var Type",varType
							);	
			return(status);
		break;
	}

	return(status);
}


uint32 busVariableDataWrite(uint32 *setVarDataPtr,uint32 varKind,
						 uint32 varAddress, uint32 varType, uint32 retainCheck)
{
	uint32 status = NO_ERROR;
	strMemPointerInfo	memPointer;
	uint8* destPtr = NULL, uint8Data, retainFlag ;
	uint16 uint16Data; 
	uint32 retainVarAddr = 0;

	status = busFbMemPointerGet(LOGIC_ID, &memPointer);

    if(status != NO_ERROR) {
		setErrorCode(__FILE__,__LINE__, "busVariableDataWrite", status );
		return(status);
	}

	switch(varKind) 
	{
		case AUTO_VAR:
			if(varAddress >= 1) 
			{			
				destPtr = ((uint8 *)memPointer.autoMemPtr);
				varAddress = varAddress - 1;
			}
			else if( (varType != SPEC_SIZE_DWORD) || (varType != SPEC_SIZE_LWORD) ) 
			{
				status = VARIABLE_TYPE_ERR;
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"Var Region" ,varKind,
									"Var Type",varType, "NOT Using",0
								);	
				return(status);
			}
		break;

		case MMEM_VAR:		
			destPtr = ((uint8 *)memPointer.flagMemPtr);
		break;

		case SMEM_VAR:
			destPtr = ((uint8 *)memPointer.stSharedMemPtr);
		break;

		default:
			status = VARIABLE_KIND_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Var Region" ,varKind,
								"Var Addr",varAddress, 
								"Var Type",varType
							);	
			return(status);
		break;
	}


	if(varType == SPEC_SIZE_BIT) 
		retainVarAddr = varAddress/32;
	else if(varType == SPEC_SIZE_BYTE) 
		retainVarAddr = varAddress/4;
	else if(varType == SPEC_SIZE_WORD) 
		retainVarAddr = varAddress/2;
	else if(varType == SPEC_SIZE_DWORD) 
		retainVarAddr = varAddress;
	else if(varType == SPEC_SIZE_LWORD) 
		retainVarAddr = varAddress*2;
	else
	{	
		status = VARIABLE_KIND_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Var Region" ,varKind,
							"Var Addr",varAddress, 
							"Var Type",varType
						);	
		return(status);
	}

	/* EN_RETAIN 일 때만 이중화 복사하지 않고 항상 이중화 복사하도록 수정 */	
#if 0
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	
	if(retainCheck == EN_RETAIN_CHECK) 
	{
		status = localVariableRetainInfoRead(varKind,retainVarAddr, &retainFlag);
		if(status != NO_ERROR)
		{
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
			
		}
		else if(retainFlag == NO_RETAIN) 
			return(status);	
	}
#endif
#endif
	switch(varType) 
	{
		case SPEC_SIZE_BIT:
			destPtr += (varAddress/8) * 1 ; 
			uint8Data = (uint8)(*setVarDataPtr & 0x1);
			if(uint8Data == 1) *destPtr |= (0x1 << (varAddress%8)); 
			else  *destPtr &= (~(0x1 << (varAddress%8)));
		break;

		case SPEC_SIZE_BYTE:
			destPtr += varAddress * 1 ; 
			uint8Data = (uint8)(*setVarDataPtr & 0xff);
			memoryCopy ((uint8*)destPtr, (uint8*)&uint8Data, 1);
		break;

		case SPEC_SIZE_WORD:		
			destPtr += varAddress * 2; 
			
			uint16Data = (uint16)(*setVarDataPtr & 0xffff);
			memoryCopy ((uint8*)destPtr, (uint8*)&uint16Data, 2);
		break;

		case SPEC_SIZE_DWORD:
			destPtr += varAddress * 4 ; 
			memoryCopy ((uint8*)destPtr, (uint8*)setVarDataPtr, 4);
		break;

		case SPEC_SIZE_LWORD:
			destPtr += varAddress * 8 ; 
			eightByteConversion((uint32*)destPtr,(uint32*)setVarDataPtr,0);
		break;
		
		default:
			status = VARIABLE_KIND_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Var Region" ,varKind,
								"Var Addr",varAddress, 
								"Var Type",varType
							);	
			return(status);
		break;
	}

	return(status);
}


uint32 rmtVariableDataWrite(uint32 *setVarDataPtr,uint32 varKind,
						 uint32 varAddress, uint32 varType, uint32 retainCheck)
{
	uint32 status = NO_ERROR;
	strMemPointerInfo	memPointer;
	uint8* destPtr=NULL, uint8Data, retainFlag ;
	uint16 uint16Data; 
	uint32 retainVarAddr = 0;
	uint8 *BusPtr=NULL, tempPtr;


	status = rmtbusFbMemPointerGet(LOGIC_ID, &memPointer);

    if(status != NO_ERROR) 
    {
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	switch(varKind) 
	{
		case AUTO_VAR:
			if(varAddress >= 1) 
			{
				
				destPtr = ((uint8 *)memPointer.autoMemPtr);
				varAddress = varAddress - 1;
			}
			else if( (varType != SPEC_SIZE_DWORD) || (varType != SPEC_SIZE_LWORD) ) 
			{
				status = VARIABLE_TYPE_ERR;
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
									"Var Region" ,varKind,
									"Var Type",varType, "NOT Using",0
								);	
				return(status);
			}
		break;

		case MMEM_VAR:		
			destPtr = ((uint8 *)memPointer.flagMemPtr);
			BusPtr = (uint8*)gMmemVariableBusPtr;
		break;

		case SMEM_VAR:
			destPtr = ((uint8 *)memPointer.stSharedMemPtr);
			BusPtr = (uint8*)gSTSharedMemDataBusPtr;
		break;

		default:
			status = VARIABLE_KIND_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Var Region" ,varKind,
								"Var Addr",varAddress, 
								"Var Type",varType
							);	
			return(status);
		break;
	}

	if(varType == SPEC_SIZE_BIT) 
		retainVarAddr = varAddress/32;
	else if(varType == SPEC_SIZE_BYTE) 
		retainVarAddr = varAddress/4;
	else if(varType == SPEC_SIZE_WORD) 
		retainVarAddr = varAddress/2;
	else if(varType == SPEC_SIZE_DWORD) 
		retainVarAddr = varAddress;
	else if(varType == SPEC_SIZE_LWORD) 
		retainVarAddr = varAddress*2;
	else
	{	
		status = VARIABLE_KIND_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							"Var Region" ,varKind,
							"Var Addr",varAddress, 
							"Var Type",varType
						);	
		return(status);
	}

	
	status = localVariableRetainInfoRead(varKind,retainVarAddr, &retainFlag);
	if(status != NO_ERROR)
	{
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	
	switch(varType) 
	{
		case SPEC_SIZE_BIT:
			if( (varKind == MMEM_VAR ) || (varKind == SMEM_VAR ) ) 
			{
				destPtr += (varAddress/8) * 1 ; 
				BusPtr += (varAddress/8) * 1 ; 
				uint8Data = (uint8)(*setVarDataPtr & 0x1);
		
				status = myBusDataAcc(__FILE__,__LINE__,BUS_BYTE_ACC, 	BUS_DATA_READ ,(uint8*)BusPtr, (uint8*)&tempPtr );
				if(status)
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				
				if(uint8Data ==1) 
					tempPtr |= (1 << (varAddress%8));
				else 
					tempPtr &= (~(1<< (varAddress%8)));
					
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
				status = myRmtDataAcc(__FILE__,__LINE__,BUS_BYTE_ACC, 	BUS_DATA_WRITE ,(uint8*)destPtr, (uint8*)&tempPtr );
				if(status)
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
#endif
			}
			else {
				status = VARIABLE_KIND_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				return(status);
			}
		break;

		case SPEC_SIZE_BYTE:
			destPtr += varAddress * 1 ; 
			uint8Data = (uint8)(*setVarDataPtr & 0xff);
			
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			status = myRmtDataAcc(__FILE__,__LINE__,	BUS_BYTE_ACC, BUS_DATA_WRITE ,
									(uint8 *)destPtr, (uint8 *)&uint8Data
							 	);
#endif
			if(status)
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		break;

		case SPEC_SIZE_WORD:		
			destPtr += varAddress * 2; 
			
			uint16Data = (uint16)(*setVarDataPtr & 0xffff);
			
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			status = myRmtDataAcc(__FILE__,__LINE__,	BUS_WORD_ACC, BUS_DATA_WRITE ,
									(uint8 *)destPtr, (uint8 *)&uint16Data
							 	);
#endif
			if(status)
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		break;

		case SPEC_SIZE_DWORD:
			destPtr += varAddress * 4 ; 
			
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			status = myRmtDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, BUS_DATA_WRITE ,
									(uint8 *)destPtr, (uint8 *)setVarDataPtr
							 	);
#endif
			if(status)
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		break;

		case SPEC_SIZE_LWORD:
			destPtr += varAddress * 8 ; 
			
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			status = myRmtDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, BUS_DATA_WRITE ,
									(uint8 *)destPtr, (uint8 *)setVarDataPtr
							 	);
#endif
			if(status)
			{
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				break;
			}
			
			destPtr += (varAddress * 8)+ 4; 
			
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
			status = myRmtDataAcc(__FILE__,__LINE__,	BUS_LONG_ACC, BUS_DATA_WRITE ,
									(uint8 *)destPtr, (uint8 *)(setVarDataPtr++)
							 	);
#endif
			if(status)
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		break;

		default:
			status = VARIABLE_KIND_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								"Var Region" ,varKind,
								"Var Addr",varAddress, 
								"Var Type",varType
							);	
			return(status);
		break;
	}

	return(status);
}


static void flagMemoryDisplay(uint32 arg1, uint32 arg2)
{
	uint32 startMemId = 0, endMemId = 0;
	uint32	status,  index, i;
	int8	line[16], *cPtr;
	strMemPointerInfo	memPointer;
	uint32  *varMemPtr;
	strSysRunningConfigInfo		*sysStatusInfoPtr = gSysRunningInfoPtr;

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	status = fbMemPointerGet(LOGIC_ID, &memPointer);
#endif
	
	printf("\tStart M Memory Number %%MD(0-%d) : ", MAX_MPU_MMEM_NUM-1);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    startMemId = atoi((const char *)cPtr);

    printf("\tEnd M Memory Number %%MD(%d-%d)  : ",startMemId+1,MAX_MPU_MMEM_NUM-1);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    endMemId = atoi((const char *)cPtr);

	if( ((endMemId - startMemId) <= 0) ||
    	(startMemId >= endMemId) ||  (startMemId < 0) ||
        (endMemId >= MAX_MPU_MMEM_NUM))
	{
    	printf( "\n");
		printf( "\t[ERROR] - Invalid %%M Mem Id (Start : %d,End : %d)\n",
        				startMemId, endMemId);
		return;
    }

	screenClear();

    while(!keyHit()) 
    {
	   	cursorPositionSet();
	    printf( " ********************* << Run Time FLAG(%%M) Memory >> *******************\n");
		printf( "   * START FLAG MEM# : [%%MD%05d], END FLAG MEM# : [%%MD%05d]\n",startMemId, endMemId);
		printf( "   * [M00000] 0x12345678 : [VAR_ADDRESS] VAR_DATA \n");
	    printf( " ************************************************************************\n");

			
		printf( " +================+================+================+================+\n");
		printf( " | %%MD# |  DATA   | %%MD# |  DATA   | %%MD# |  DATA   | %%MD# |  DATA   |\n");
	    printf( " +================+================+================+================+\n");

		index = 1;
		for(i = startMemId; i <= endMemId; i++) 
		{
			varMemPtr = (uint32*)memPointer.flagMemPtr;
			varMemPtr += i;

			printf(" [M%05d]", i);
		    printf("%08x", *varMemPtr);

			if( !(index % 4)) 
				printf( "\n");
				
			index++;
		}
		
		if(((index-1) % 4)) 
			printf( "\n");
			
	    printf( " +================+================+================+================+\n");
	    
		sysTaskDelay(systemClockRateGet()/2);

	}
}


static void autoMemoryDisplay(uint32 arg1, uint32 arg2)
{
	uint32 startMemId = 1, endMemId = 1;
	uint32	status,  index, i;
	int8	line[16], *cPtr;
	strMemPointerInfo	memPointer;
	uint32  *varMemPtr;

	strSysRunningConfigInfo		*sysStatusInfoPtr = gSysRunningInfoPtr;

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	status = fbMemPointerGet(LOGIC_ID, &memPointer);
#endif

	printf("\tStart Auto Memory Number %%AD(1-%d) : ", MAX_MPU_AUTOVAR_NUM);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    startMemId = atoi((const char *)cPtr);

    printf("\tEnd A Memory Number %%AD(%d-%d)  : ",startMemId+1,MAX_MPU_AUTOVAR_NUM);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    endMemId = atoi((const char *)cPtr);

	if( ((endMemId - startMemId) <= 0) ||
    	(startMemId >= endMemId) ||  (startMemId <= 0) ||
        (endMemId > MAX_MPU_AUTOVAR_NUM))
	{
    	printf( "\n");
		printf( "\t[ERROR] - Invalid %%A Mem Id (Start : %d,End : %d)\n",
        				startMemId, endMemId);
		return;
    }

	screenClear();

    while(!keyHit()) 
	{
	   	cursorPositionSet();
		printf( " ********************* << Run Time AUTO(%%A) Memory >> *******************\n");
		printf( "   * START AUTO MEM# : [%%AD%05d], END AUTO MEM# : [%%AD%05d]\n",startMemId, endMemId);
		printf( "   * [A00000] 0x12345678 : [VAR_ADDRESS] VAR_DATA \n");
	    printf( " ************************************************************************\n");

			
		printf( " +================+================+================+================+\n");
		printf( " | %%AD# |  DATA   | %%AD# |  DATA   | %%AD# |  DATA   | %%AD# |  DATA   |\n");
		printf( " +================+================+================+================+\n");

		index = 1;
		for(i = startMemId; i <= endMemId; i++) 
		{
			varMemPtr = (uint32*)memPointer.autoMemPtr;
			varMemPtr += (i-1);
			printf(" [A%05d]", i);
			printf("%08x", *varMemPtr);

			if( !(index % 4)) 
				printf( "\n");
				
			index++;
		}
		if(((index-1) % 4)) 
			printf( "\n");
			
		printf( " +================+================+================+================+\n");
		
		sysTaskDelay(systemClockRateGet()/2);
	}

}


static void sharedMemoryDisplay(uint32 arg1, uint32 arg2)
{
	uint32 startMemId = 0, endMemId = 0;
	uint32	status,  index, i;
	int8	line[16], *cPtr;
	strMemPointerInfo	memPointer;
	uint32  *varMemPtr;
	strSysRunningConfigInfo		*sysStatusInfoPtr = gSysRunningInfoPtr;

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	status = fbMemPointerGet(LOGIC_ID, &memPointer);
#endif

	printf("\tStart Auto Memory Number %%SD(0-%d) : ", MAX_SHARED_MEM_NUM-1);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    startMemId = atoi((const char *)cPtr);

    printf("\tEnd A Memory Number %%SD(%d-%d)  : ",startMemId+1,MAX_SHARED_MEM_NUM-1);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    endMemId = atoi((const char *)cPtr);


	if( ((endMemId - startMemId) <= 0) ||
    	(startMemId >= endMemId) ||  (startMemId < 0) ||
        (endMemId >= MAX_SHARED_MEM_NUM))
	{
    	printf( "\n");
		printf( "\t[ERROR] - Invalid %%S Mem Id (Start : %d,End : %d)\n",
        				startMemId, endMemId);
		return;
    }

	screenClear();

    while(!keyHit()) 
	{
	   	cursorPositionSet();
	   	
		printf( " ******************* << Run Time Shared(%%S) Memory >> ****************\n");
		printf( "   * START Shared MEM# : [%%SD%05d], END Shared MEM# : [%%SD%05d]\n",startMemId, endMemId);
		printf( "   * [S00000] 0x12345678 : [VAR_ADDRESS] VAR_DATA \n");
	    printf( " *********************************************************************\n");

			
		printf( " +================+================+================+================+\n");
		printf( " | %%SD# |  DATA   | %%SD# |  DATA   | %%SD# |  DATA   | %%SD# |  DATA   |\n");
	    printf( " +================+================+================+================+\n");

		index = 1;
		for(i = startMemId; i <= endMemId; i++) 
		{
			varMemPtr = (uint32*)memPointer.stSharedMemPtr;
			varMemPtr += i;

			printf(" [S%05d]", i);
			printf("%08x", *varMemPtr);

			if( !(index % 4)) 
				printf( "\n");
				
			index++;
		}
		if(((index-1) % 4)) 
			printf( "\n");
			
	    printf( " +================+================+================+================+\n");
	    
		sysTaskDelay(systemClockRateGet()/2);
	}
}

void variableMemoryDataDisplay(uint32 moduleId, uint32 arg1)
{
	int8 line[16], *pLine, **pStr;
    int8    defaultStr[] = "\n\t *Unrecognized Command !!!\n";

	strDbgCmdFuncInfo varDebugCmdFunc[] =
	{
		{"add",		NULL, 	NULL,	&autoMemoryDisplay},
		{"mdd",		NULL, 	NULL,	&flagMemoryDisplay},
		{"sdd",		NULL, 	NULL,	&sharedMemoryDisplay},
		{"vdd",		NULL, 	NULL,	&variableInMemoryDisplay},
		{"vdr",		NULL, 	NULL,	&variableDataDisplay},
		{"NULL",	NULL, 	NULL,	NULL} 	 	
	};

	strDbgCmdFuncInfo *cmdFuncPtr;

    int8 *helpString[] = {
	    "*************************************",
		"* [add] - A Region Data Display",
		"* [mdd] - M Region Data Display",
	    "* [sdd] - S Region Data Display",
		"* [vdd] - Variable Data Display",
		"* [vdr] - Variable Data Read",
	    "* [q]   - Exit",
	    "*************************************",
	    NULL
    };

	for(;;) 
	{
    	printf( "\n");

		for(pStr = helpString; *pStr != NULL; pStr++)
        	printf( "\t%s\n", *pStr);

        printf("\n\tEnter Command : ");

        ioRead(gDebugPortFd, line, sizeof(line));
		pLine = line;
		spaceSkip(&pLine);

		if(*pLine == 'q')
		{
			break;
		}

		else if(*pLine != ENDOFSTR) 
		{
			for(cmdFuncPtr = varDebugCmdFunc; cmdFuncPtr->dbgFunc != NULL; cmdFuncPtr++)
			{
				if(stringCompare(pLine,cmdFuncPtr->dbgCmdStr) == NO_ERROR)
				{
					cmdFuncPtr->dbgFunc(moduleId,cmdFuncPtr->arg2);
	                printf("\n");
					break;	
				} 	
			}
			
			if(cmdFuncPtr->dbgFunc == NULL)
			{
				printf("%s", defaultStr);
			}
		}
    }

	return;
}

static void variableInMemoryDisplay(uint32 initType, uint32 arg1)
{
	uint32 startMemId, endMemId;
	uint32	status,  index, i,variableRegion;
	int8	line[16], *cPtr;
	strMemPointerInfo	memPointer;
	void  *varMemPtr = NULL;
	uint8* strVarRegion[] = { "NULL","M","A","I","Q","S","NULL","NULL"};
	uint8* strVarType[] = { "NULL","X","B","W","D","L"};

	uint32 maxMemAddress = 0,variableSize=0; 
	union multiDataType64	data;
	uint8 retainInfo = 0, uint8data;
	uint16 uint16data;
	strSysRunningConfigInfo		*sysStatusInfoPtr = gSysRunningInfoPtr;

	if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN){
		status = fbMemPointerGet(LOGIC_ID, &memPointer);
	}
	else{	
		status = busFbMemPointerGet(LOGIC_ID, &memPointer);
	}	
    printf("\n\t================VARIABLE MEMEORY DISPLAY=================");

VARIABLE_KIND:
    if(initType == 0)
		printf("\n\tEnter the Choice Variable Kind[M,A,S]? ");
	else
		printf("\n\tEnter the Choice Variable Kind[M,A]? ");

	ioRead(gDebugPortFd, line, sizeof(line));
	cPtr = line;
	spaceSkip(&cPtr);
	switch(*cPtr){
        case 'm': case 'M':
			variableRegion = MMEM_VAR;
			maxMemAddress = MAX_MPU_MMEM_NUM;
			if(initType == 0) 
				varMemPtr = (void*)memPointer.flagMemPtr;
		break;
		case 'a': case 'A':
			variableRegion 	= AUTO_VAR;
			maxMemAddress 	= MAX_MPU_AUTOVAR_NUM;

			if(initType == 0) 
				varMemPtr = (void*)memPointer.autoMemPtr;
		break;

		case 's': case 'S':
			if(initType == 0) {
				variableRegion = SMEM_VAR;
				maxMemAddress = MAX_SHARED_MEM_NUM;
				varMemPtr = (void*)memPointer.stSharedMemPtr;
			}
			else {
				printf( "Invalid Variable Kind!!!\n");
				goto VARIABLE_KIND;
			}
		break;

		case '.':
			goto MODULE_END;
		break;

		default :
            printf( "Invalid Variable Kind!!!\n");
			goto VARIABLE_KIND;
		break;
	}

VARIABLE_TYPE:
	if( variableRegion == AUTO_VAR) {
		variableSize = SPEC_SIZE_DWORD;
		maxMemAddress *= 1;
	}
	else {

		printf("\n\tEnter the Choice Variable Type[X,B,W,D,L]? ");
		ioRead(gDebugPortFd, line, sizeof(line));
		cPtr = line;
		spaceSkip(&cPtr);

		switch(*cPtr){
			case 'x': case 'X':
				variableSize = SPEC_SIZE_BIT;
				maxMemAddress *= 32;
			break;
			case 'b': case 'B':
				variableSize = SPEC_SIZE_BYTE;
				maxMemAddress *= 4;
			break;
			case 'w': case 'W':
				variableSize = SPEC_SIZE_WORD;
				maxMemAddress *= 2;
			break;
			case 'd': case 'D':
				variableSize = SPEC_SIZE_DWORD;
				maxMemAddress *= 1;
			break;
			case 'l': case 'L':
				variableSize = SPEC_SIZE_LWORD;
				maxMemAddress /= 2;
			break;

			case '.':
				goto MODULE_END;
			break;

			default :
				printf( "Invalid Variable Type!!!\n");
				goto VARIABLE_TYPE;
			break;
		}
	}

	if( variableRegion != AUTO_VAR) {
		printf("\n\tStart Memory Address %%%s%s(0-%d) : ",
			strVarRegion[variableRegion],strVarType[variableSize] ,maxMemAddress-1);
	}
	else {
		printf("\n\tStart Memory Address %%%s%s(1-%d) : ",
			strVarRegion[variableRegion],strVarType[variableSize] ,maxMemAddress);
	}


	ioRead(gDebugPortFd, line, sizeof(line));
	cPtr = line;
	spaceSkip(&cPtr);
	startMemId = atoi((const char *)cPtr);


	if( variableRegion != AUTO_VAR) {
		startMemId = atoi((const char *)cPtr);
		printf("\tEnd Memory Address  %%%s%s(%d-%d) : ",
			strVarRegion[variableRegion],strVarType[variableSize] ,startMemId+1,maxMemAddress-1);
	}
	else{
		printf("\tEnd Memory Address  %%%s%s(%d-%d) : ",
		strVarRegion[variableRegion],strVarType[variableSize] ,startMemId+1,maxMemAddress);
	}

	ioRead(gDebugPortFd, line, sizeof(line));
	cPtr = line;
	spaceSkip(&cPtr);

	endMemId = atoi((const char *)cPtr);

	if( ((endMemId - startMemId) <= 0) ||
    	(startMemId >= endMemId) ||  (startMemId < 0) )
		
	{
		if(  (( variableRegion != AUTO_VAR) && (endMemId >= maxMemAddress) ) ||
		 	 (( variableRegion == AUTO_VAR) && (endMemId > maxMemAddress) ) 
	    ){
    		printf( "\n");
			printf( "\t[ERROR] - Invalid Memory Address(Start : %d,End : %d)\n",
        					startMemId, endMemId);
			return;
		}
	}

	index = 1;
	printf(" \n [VAR_ADDRESS] VAR_DATA(HEX):RETAIN_INFO \n");
   	printf( "\n");

	memoryClear((uint8*)&data,sizeof(LONG_SIZE));
	if(variableSize == SPEC_SIZE_DWORD) {
		for(i = startMemId; i <= endMemId; i++) {
			
			if( variableRegion == AUTO_VAR ) {
				memoryCopy((uint8*)&data.Hex_t,(uint8*)((uint32*)varMemPtr + (i-1) ), DOUBLE_SIZE);
				(void) localVariableRetainInfoRead(variableRegion, (i-1), &retainInfo);
			}
			else {
				memoryCopy((uint8*)&data.Hex_t,(uint8*)((uint32*)varMemPtr + i), DOUBLE_SIZE);
				(void) localVariableRetainInfoRead(variableRegion, i, &retainInfo);
			}
			printf(" [%05d]", i);
			printf("%08x", data.Hex_t);
			printf(":%d", retainInfo);


			if( !(index % 4)) printf( "\n");
			index++;
		}
	}
	else if(variableSize == SPEC_SIZE_WORD) {
		for(i = startMemId; i <= endMemId; i++) {
			memoryCopy((uint8*)&uint16data,(uint8*)((uint16*)varMemPtr + i), WORD_SIZE);
			data.Hex_t = (uint32)uint16data;

			printf(" [%05d]", i);
			printf("%04x", data.Hex_t);

			if(variableRegion != AUTO_VAR) {
				(void) localVariableRetainInfoRead(variableRegion, i/2, &retainInfo);
				printf(":%d", retainInfo);
			}

			if( !(index % 5)) printf( "\n");
			index++;
		}
	}

	else if(variableSize == SPEC_SIZE_BYTE) {
		for(i = startMemId; i <= endMemId; i++) {
			memoryCopy((uint8*)&uint8data,(uint8*)((uint8*)varMemPtr + i), BYTE_SIZE);
			data.Hex_t = (uint32)uint8data;

			printf(" [%05d]", i);
			printf("%02x", data.Hex_t);

			if( variableRegion != AUTO_VAR ) {
				(void) localVariableRetainInfoRead(variableRegion, i/4, &retainInfo);
				printf(":%d", retainInfo);
			}

			if( !(index % 6)) printf( "\n");
			index++;
		}
	}

	else if(variableSize == SPEC_SIZE_BIT) {
		for(i = startMemId; i <= endMemId; i++) {
			memoryCopy((uint8*)&uint8data,(uint8*)((uint8*)varMemPtr + i/8), BYTE_SIZE);
			data.Hex_t = (uint32)uint8data;

			if( !((index % 8)-1)) printf(" [%05d]", i);
			printf("%01x", (data.Hex_t>>(i%8))&0x1);

			if( !(index % 4)) printf( " ");

			if( !(index % 32)) printf( "\n");
			index++;
		}
	}
#ifdef USED_BIT64
	else if(variableSize == SPEC_SIZE_LWORD) {
		for(i = startMemId; i <= endMemId; i++) {
			memoryCopy((uint8*)& data.Ulint_t,(uint8*)((uint64*)varMemPtr + i), DOUBLE_SIZE*2);
			printf(" [%04d]", i);
			printf("%016x", (uint64)data.Ulint_t);

			if( variableRegion != AUTO_VAR ) {
				(void) localVariableRetainInfoRead(variableRegion, i*2, &retainInfo);
				printf(":%d", retainInfo);
			}

			if( !(index % 3)) printf( "\n");
			index++;
		}
	}
#endif

MODULE_END:
	
	return;
}

static void variableDataDisplay(uint32 arg1, uint32 arg2)
{
	int8 line[16], *pLine;
	uint32 variableRegion, variableSize, bitposFlag =0;
	uint32 maxBitPosition = 0, maxMemAddress =0;
	uint32 bitPosition=0, memAddress= 0, memIndex = 0,specData=0;
	strFbSpecActionInfo specActionData;
	uint32	specType;
	uint8  retainInfo = 0;
	union multiDataType64	data;


	uint8* strVarRegion[] = { "NULL","M","A","I","Q","S","NULL","NULL"};
	uint8* strVarType[] = { "NULL","X","B","W","D","L"};

	printf( "\n\t==================Variable Data Display===================");
    printf("\n\tVARIABLE KIND : M(Flag Memory), A(Auto Memory), S(Shared Memory),");
    printf("\n\tVARIABLE TYPE : X(Bit:1bit), B(BYTE:8bit), W(WORD:16bit)");
	printf("\n\t                D(DWORD:32bit), L(LWORD:64bit)");

    printf( "\n");

VARIABLE_KIND:
    printf("\n\tEnter the Choice Variable Kind[M,A,S]? ");
	ioRead(gDebugPortFd, line, sizeof(line));
	pLine = line;
	spaceSkip(&pLine);
	
	switch(*pLine)
	{
        case 'm': case 'M':
			variableRegion = M_MEMORY;
			maxMemAddress = MAX_MPU_MMEM_NUM;
		break;
		case 'a': case 'A':
			variableRegion = A_MEMORY;
			maxMemAddress = MAX_MPU_AUTOVAR_NUM;
		break;
		case 's': case 'S':
			variableRegion = S_MEMORY;
			maxMemAddress = MAX_SHARED_MEM_NUM;
		break;

		case '.':
			goto MODULE_END;
		break;

		default :
            printf( "Invalid Variable Kind!!!\n");
			goto VARIABLE_KIND;
		break;
	}

VARIABLE_TYPE:
	if(variableRegion == A_MEMORY)
	{
		variableSize = SPEC_SIZE_DWORD;
		maxBitPosition = 0;
		maxMemAddress *= 1;
		bitposFlag = 0;
	}
	else
	{
		printf("\n\tEnter the Choice Variable Type[X,B,W,D,L]? ");
		ioRead(gDebugPortFd, line, sizeof(line));
		pLine = line;
		spaceSkip(&pLine);

		switch(*pLine)
		{
			case 'x': case 'X':
				variableSize = SPEC_SIZE_BIT;
				maxMemAddress *= 32;
			break;
			case 'b': case 'B':
				variableSize = SPEC_SIZE_BYTE;
				maxBitPosition = 7;
				maxMemAddress *= 4;
			break;
			case 'w': case 'W':
				variableSize = SPEC_SIZE_WORD;
				maxBitPosition = 15;
				maxMemAddress *= 2;
			break;
			case 'd': case 'D':
				variableSize = SPEC_SIZE_DWORD;
				maxBitPosition = 31;
				maxMemAddress *= 1;
			break;
			
			case 'l': case 'L':
				variableSize = SPEC_SIZE_LWORD;
				maxBitPosition = 63;
				maxMemAddress /= 2;
			break;

			case '.':
				goto MODULE_END;
			break;

			default :
				printf( "Invalid Variable Type!!!\n");
				goto VARIABLE_TYPE;
			break;
		}
	}
	
MEM_ADDRESS:
	if( (variableRegion == M_MEMORY) || (variableRegion == S_MEMORY) )
	{

		printf("\n\tEnter the Memory Address(0-%d)? ",maxMemAddress-1);
		ioRead(gDebugPortFd, line, sizeof(line));
		pLine = line;
		spaceSkip(&pLine);
		memAddress= atoi(pLine);

		if((memAddress<0) || (memAddress >= maxMemAddress)) {
			 printf( "%d is Invalid Memory Address!!!\n", memAddress);
			 goto MEM_ADDRESS;
		}

		if((variableSize != SPEC_SIZE_BIT) && (variableRegion != A_MEMORY))
		{
			printf("\n\tDoes it have Bit Position?[Y or N] ");
			ioRead(gDebugPortFd, line, sizeof(line));
			pLine = line;
			spaceSkip(&pLine);
			if((*pLine =='Y') ||(*pLine =='y'))
			{
				bitposFlag =1;
BIT_POSITION:
				printf("\n\tEnter the Bit Position?[0-%2d])? ",maxBitPosition);
				ioRead(gDebugPortFd, line, sizeof(line));
				pLine = line;
				spaceSkip(&pLine);
				bitPosition = atoi(pLine);
				if((bitPosition<0) || (bitPosition > maxBitPosition)){
					 printf( "%d is Invalid Bit Position!!!\n", bitPosition);
					 goto BIT_POSITION;
				}
			}
			else bitposFlag =0;
		}
	}
	else if(variableRegion == A_MEMORY) 
	{
		printf("\n\tEnter the Memory Address(1-%d)? ",maxMemAddress);
		ioRead(gDebugPortFd, line, sizeof(line));
		pLine = line;
		spaceSkip(&pLine);
		memAddress= atoi(pLine);

		if(memAddress<1|| memAddress > maxMemAddress) 
		{
			 printf( "%d is Invalid Memory Address!!!\n", memAddress);
			 goto MEM_ADDRESS;
		}
	}

	specData =	memAddress<<8;
	specData |= (memIndex &0x3f)<<2; 
	specActionData.dataSize = variableSize;
	specActionData.refType = SPEC_TYPE_BYREF;
	specActionData.memoryType = variableRegion;
	specActionData.bitPosition = bitPosition;
	specActionData.bitposFlag =bitposFlag;
	
	if(variableSize == SPEC_SIZE_LWORD) 
		specType = SIZE64_TYPE;
	else 
		specType = SIZE32_TYPE;

	if(refDataCopyFromMem((uint32*)&data,specData, specActionData, specType,0)==NO_ERROR)
	{
		printf("\n\tVariable Data : %%%s%s%d",
			strVarRegion[variableRegion],strVarType[variableSize] ,memAddress);

		if (bitposFlag== 1)  
		{
			printf(".%1d",bitPosition);
			printf("\n\t * HEX TYPE(1BIT) : 0x%.1x ",data.Bool_t);	
		}
		else if(variableSize == SPEC_SIZE_BIT) 
			printf("\n\t * HEX TYPE(1BIT) : 0x%.1x ",data.Bool_t);
		
		else if(variableSize == SPEC_SIZE_BYTE) 
		{
			printf("\n\t * HEX TYPE(8BIT) : 0x%.2x",data.Hex_t);
			printf("\n\t *      SINT TYPE : %d",data.Int_t);
			printf("\n\t *     USINT TYPE : %u",data.Int_t);
		}
		else if(variableSize == SPEC_SIZE_WORD) 
		{
			printf("\n\t * HEX TYPE(16BIT) : 0x%.4x ",data.Hex_t);
			printf("\n\t *        INT TYPE : %d ",data.Int_t);
			printf("\n\t *       UINT TYPE : %u ",data.Int_t);
		}
		else if(variableSize == SPEC_SIZE_DWORD) {
			printf("\n\t * HEX TYPE(32BIT) : 0x%.8x ",data.Hex_t);	
			printf("\n\t *       LINT TYPE : %d ",data.Int_t);	
			printf("\n\t *      ULINT TYPE : %u ",data.Int_t);	
			printf("\n\t *       REAL TYPE : %f ",data.Float_t);
		}
#ifdef USED_BIT64
		else if(variableSize == SPEC_SIZE_LWORD)
		{
#ifdef _WIN32
			printf("\n\t* HEX TYPE(64BIT) : 0x%.8x%.8x ",data.halfUlint_t[1],data.halfUlint_t[0]);	
#else
			printf("\n\t* HEX TYPE(64BIT) : 0x%.8x%.8x ",data.halfUlint_t[0],data.halfUlint_t[1]);	
#endif	
			printf("\n\t * DINT TYPE      : %d ",data.LInt_t);	
			printf("\n\t * UDINT TYPE     : %u ",data.LInt_t);	
			printf("\n\t * DOUBLE TYPE    : %6.13f ",data.LReal_t);
		}
#endif
		
		(void) varRetainInfoReadwithRef(specData, specActionData, (uint32*)&retainInfo);
		if( (variableRegion == M_MEMORY)|| (variableRegion == A_MEMORY)  )
			 printf("\n\tRetain Info: 0x%.2x\n",retainInfo);
	}

	else 
		printf( "\tInvalid variable Data[%%%s] !!!\n",line);
		
    printf( "\n");
    
MODULE_END:
	
	return;
}



uint32 specActionDataCheck(uint32 *specActionPtr, uint32 specData )
{
	uint32 status = NO_ERROR;
	uint32 memoryAddress = 0, index =0;
	strFbSpecActionInfo *specActionData;
	uint32 offSet;

	if(specActionPtr == NULL) {
		status = FB_SPEC_ACTION_REF_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}
	
	specActionData = (strFbSpecActionInfo*)specActionPtr;
	 
	
	if( (specActionData->dataType == 0) || 
		(specActionData->dataType > SPEC_DATA_TYPE_DT)
	) {
		status = FB_SPEC_ACTION_TYPE_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
						   "Data Type", specActionData->dataType,
						   "Recv Task Id", specActionData->taskID,
						   "Not Using", 0
						   );
		return(status);
	}

	 
	switch(specActionData->dataSize) 
		{
		case SPEC_SIZE_BIT:
			offSet = 16; 
		break;
		case SPEC_SIZE_BYTE:
			offSet = 8; 
		break;
		case SPEC_SIZE_WORD:
			offSet = 4; 
		break;
		case SPEC_SIZE_DWORD:
			offSet = 2; 
		break;
		case SPEC_SIZE_LWORD:
			offSet = 1; 
		break;		

		default:
			status = FB_SPEC_ACTION_SIZE_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							   "Data Size", specActionData->dataSize,
							   "Recv Task Id", specActionData->taskID,
							   "Not Using", 0
							   );

			offSet = 0; 
		break;
	}

	if(status != NO_ERROR) {
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		goto CHECK_END;
	}


	if(specActionData->refType ==SPEC_TYPE_BYREF ){
		memoryAddress = specData>>8;
		index = (specData>>2)&0x3f;		

		
		switch(specActionData->memoryType) 
		{
		case F_MEMORY:
		    if( (memoryAddress == 0)||(memoryAddress > MAX_MPU_BLOCK_ADDR) )
		    	status = FB_SPEC_ACTION_ADDR_ERR;
		    else if( (index == 0)|| (index > MAX_FB_SPEC_PERMISSION_NUM) )
				status = FB_SPEC_ACTION_ADDR_ERR;
		    else if((specActionData->bitposFlag==TRUE) || (specActionData->bitPosition != 0))
				status = FB_SPEC_ACTION_BITPOS_ERR;
			if(status != NO_ERROR) {
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								   "MEM Address", memoryAddress,
								   "MEM Index", index,
								   "Bit Position", specActionData->bitposFlag
								   );
			}
		break;

		case A_MEMORY: 
			if( (memoryAddress == 0)||(memoryAddress>MAX_MPU_AUTOVAR_NUM) || (index != 0x0) )
				status = FB_SPEC_ACTION_ADDR_ERR;
			else if((specActionData->bitposFlag==TRUE) || (specActionData->bitPosition != 0))
				status = FB_SPEC_ACTION_BITPOS_ERR;
			if(status != NO_ERROR) {
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								   "MEM KIND", specActionData->memoryType,
								   "MEM Address", memoryAddress,
								   "Bit Position", specActionData->bitposFlag
								   );
			}
		break;

		case M_MEMORY:
			if( (memoryAddress < 0)||(memoryAddress>MAX_MPU_MMEM_NUM * offSet/2) || (index != 0x0) )
				status = FB_SPEC_ACTION_ADDR_ERR;

			if(status != NO_ERROR) {
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								   "MEM KIND", specActionData->memoryType,
								   "MEM Address", memoryAddress,
								   "MAX Address", MAX_MPU_MMEM_NUM * offSet/2
								   );
			}
		break;

		case S_MEMORY:
			if( (memoryAddress < 0)||(memoryAddress>MAX_SHARED_MEM_NUM * offSet/2) || (index != 0x0) )
				status = FB_SPEC_ACTION_ADDR_ERR;

			if(status != NO_ERROR) {
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								   "MEM KIND", specActionData->memoryType,
								   "MEM Address", memoryAddress,
								   "MAX Address", MAX_MPU_MMEM_NUM * offSet/2
								   );
			}
		break;

		case I_MEMORY:
		case Q_MEMORY:
			
			if( (memoryAddress < 0) || (memoryAddress > (MAX_IO_SHELF_NUM-1)) ) 
				status = IO_SHELF_NUM_ERR;
			else if ((index < 0) || (index > (MAX_IO_CARD_NUM-1)) ) 
				status = IO_CARD_NUM_ERR;

			if(status != NO_ERROR) {
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								   "MEM KIND", specActionData->memoryType,
								   "MEM Address", memoryAddress,
								   "MEM Index", index
								   );
			}
		break;

		case T_MEMORY:
		case P_MEMORY:
			
		break;

		default:
			status = VARIABLE_KIND_ERR;
			if((status != NO_ERROR)) {
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								   "MEM KIND", specActionData->memoryType,
								   "MEM Address", specActionData->taskID,
								   "Not Using", 0
								   );
			}
		break;
		}
	}
	
	else {
		if(specActionData->memoryType != 0) 
			status = VARIABLE_KIND_ERR;
		else if(specActionData->bitPosition != 0)
			status = FB_SPEC_ACTION_BITPOS_ERR;
		else if(specActionData->bitposFlag != 0)
			status = FB_SPEC_ACTION_BITPOS_ERR;

		if(status != NO_ERROR) {
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							   "VAR KIND",  specActionData->refType ,
							   "MEM KIND", specActionData->memoryType,
							   "Bit Flag", specActionData->bitposFlag
							   );
		}
	}
	
	if(status != NO_ERROR) {
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		goto CHECK_END;
	}
	
	if(specActionData->negated == TRUE) {
		if( (specActionData->dataType != UINT_TYPE) && (specActionData->dataType != UINT_TYPE) ) {
			status = FB_SPEC_ACTION_TYPE_ERR;
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							   "MEM KIND", specActionData->memoryType,
							   "Negated", specActionData->negated,
							   "Data Type", specActionData->dataType
							   );
		}
	}
	
	if(status != NO_ERROR) {
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		goto CHECK_END;
	}

	
	if(specActionData->bitposFlag==FALSE) {
		
		if(specActionData->bitPosition != 0)
			status = FB_SPEC_ACTION_BITPOS_ERR;
		else if(specActionData->dataSize == SPEC_SIZE_LWORD){
			if( (specActionData->dataType != SPEC_DATA_TYPE_LINT) &&
				(specActionData->dataType != SPEC_DATA_TYPE_ULINT) &&
				(specActionData->dataType != SPEC_DATA_TYPE_LREAL) &&
				(specActionData->dataType != SPEC_DATA_TYPE_DT)
			)
				status = FB_SPEC_TYPE_SIZE_ERR;
		}
		if(status != NO_ERROR) {
			setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
							   "Bit Flag", specActionData->bitposFlag,
							   "Bit Position", specActionData->bitPosition,
							   "Data Type", specActionData->dataType
							   );
		}
	}
	else if (specActionData->bitposFlag==TRUE)
	{
		
		if( (specActionData->memoryType != I_MEMORY) &&	
			(specActionData->memoryType != Q_MEMORY) &&
			(specActionData->memoryType != P_MEMORY) &&
			(specActionData->memoryType != T_MEMORY)
		) {
			if( (specActionData->dataType == SPEC_DATA_TYPE_REAL) ||
				(specActionData->dataType == SPEC_DATA_TYPE_LREAL)
			) {
				 status = FB_SPEC_ACTION_TYPE_ERR;
				setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
								   "Bit Flag", specActionData->bitposFlag,
								   "Data Type", specActionData->dataType,
								   "Data Size", specActionData->dataSize
							 );
			}
		}

		switch(specActionData->dataSize) 
		{
			case SPEC_SIZE_BIT:
				if( (specActionData->memoryType == I_MEMORY) ||
					(specActionData->memoryType == Q_MEMORY) ||
					(specActionData->memoryType == T_MEMORY) ||
					(specActionData->memoryType == P_MEMORY)
				) {
					
					if(	(specActionData->bitPosition < 0) || 
						(specActionData->bitPosition > 31) 
					) {
						status = FB_SPEC_ACTION_BITPOS_ERR;
						setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
										   "Bit Flag", specActionData->bitposFlag,
										   "Bit Position", specActionData->bitPosition,
										   "Data Size", specActionData->dataSize
										);
					}
				} else {
					if(specActionData->bitPosition !=  0) {
						status = FB_SPEC_ACTION_BITPOS_ERR;
						setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
										   "Bit Flag", specActionData->bitposFlag,
										   "Bit Position", specActionData->bitPosition,
										   "Data Size", specActionData->dataSize
										);
					}
				}
				
			break;
			case SPEC_SIZE_BYTE:
				if( (specActionData->memoryType == I_MEMORY) ||
					(specActionData->memoryType == Q_MEMORY) ||
					(specActionData->memoryType == T_MEMORY) ||
					(specActionData->memoryType == P_MEMORY)
				) {
					
					if(	(specActionData->bitPosition < 0) || 
						(specActionData->bitPosition > 24) 
					) {
						status = FB_SPEC_ACTION_BITPOS_ERR;
						setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
										   "Bit Flag", specActionData->bitposFlag,
										   "Bit Position", specActionData->bitPosition,
										   "Data Size", specActionData->dataSize
										);
					}
				} else {
					if(specActionData->bitPosition >= 8) {
						status = FB_SPEC_ACTION_BITPOS_ERR;
						setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
										   "Bit Flag", specActionData->bitposFlag,
										   "Bit Position", specActionData->bitPosition,
										   "Data Size", specActionData->dataSize
										);
					}
				}
			break;
			case SPEC_SIZE_WORD:
				if( (specActionData->memoryType == I_MEMORY) ||
					(specActionData->memoryType == Q_MEMORY) ||
					(specActionData->memoryType == T_MEMORY) ||
					(specActionData->memoryType == P_MEMORY)
				) {
					
					if(	(specActionData->bitPosition < 0) || 
						(specActionData->bitPosition > 16) 
					) {
						status = FB_SPEC_ACTION_BITPOS_ERR;
						setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
										   "Bit Flag", specActionData->bitposFlag,
										   "Bit Position", specActionData->bitPosition,
										   "Data Size", specActionData->dataSize
										);
					}
				} else {
					if(specActionData->bitPosition >= 16) {
						status = FB_SPEC_ACTION_BITPOS_ERR;
						setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__, status,
										   "Bit Flag", specActionData->bitposFlag,
										   "Bit Position", specActionData->bitPosition,
										   "Data Size", specActionData->dataSize
										);
					}
				}
			break;
			case SPEC_SIZE_DWORD:
				if( (specActionData->memoryType != I_MEMORY) &&
					(specActionData->memoryType != Q_MEMORY) &&
					(specActionData->memoryType != T_MEMORY) &&
					(specActionData->memoryType != P_MEMORY)) 
				{				
					if(specActionData->bitPosition >= 32) 
					{
						status = FB_SPEC_ACTION_BITPOS_ERR;
						setErrorCodeWithVal(__FILE__,__LINE__,__FUNCTION__, status,
										   "Bit Flag", specActionData->bitposFlag,
										   "Bit Position", specActionData->bitPosition,
										   "Data Size", specActionData->dataSize
											);
					}
				}
			break;
			case SPEC_SIZE_LWORD:
				if( (specActionData->memoryType == I_MEMORY) ||
					(specActionData->memoryType == Q_MEMORY) ||
					(specActionData->memoryType == T_MEMORY) ||
					(specActionData->memoryType == P_MEMORY)
				) {
					status = FB_SPEC_ACTION_BITPOS_ERR;
					setErrorCodeWithVal(__FILE__,__LINE__,__FUNCTION__, status,
									   "Bit Flag", specActionData->bitposFlag,
									   "Bit Position", specActionData->bitPosition,
									   "Data Size", specActionData->dataSize
										);
				} 
				else 
				{			
					if(specActionData->bitPosition >= 64) {
						setErrorCodeWithVal(__FILE__,__LINE__,__FUNCTION__, status,
										   "Bit Flag", specActionData->bitposFlag,
										   "Bit Position", specActionData->bitPosition,
										   "Data Size", specActionData->dataSize
											);
					}
				}
			break;		
			default:
				status = FB_SPEC_ACTION_SIZE_ERR;
				setErrorCodeWithVal(__FILE__,__LINE__,__FUNCTION__, status,
								   "Bit Flag", specActionData->bitposFlag,
								   "Bit Position", specActionData->bitPosition,
								   "Data Size", specActionData->dataSize
									);
			break;
		}

	}

CHECK_END:
	return(status);
}


uint32 varMemoryClear()
{
	uint32 status = NO_ERROR;

	strMemPointerInfo	memPointer;
	strVariableExecInfo sysVarInfo;
	uint32 autoVarClearSize = 0, flagVarClearSize = 0; 

	
	status = fbMemPointerGet(LOGIC_ID, &memPointer);
    if(status != NO_ERROR) {
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}
	else{
		memoryClear((uint8*)memPointer.autoMemPtr, (MAX_MPU_AUTOVAR_NUM*4));
		memoryClear((uint8*)memPointer.flagMemPtr, (MAX_MPU_MMEM_NUM*4));
		memoryClear((uint8*)memPointer.stSharedMemPtr, MAX_SHARED_MEM_SIZE);
		memoryClear((uint8*)memPointer.retainMemPtr, MAX_MPU_RETAIN_FLAG_MEM_SIZE);
	}

	
	status = busFbMemPointerGet(LOGIC_ID, &memPointer);
    if(status != NO_ERROR) {
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}
	else {
		
		status = fbSysExeVarInfoRead(AUTO_VAR , &sysVarInfo);
	    if(status != NO_ERROR) {
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}
		autoVarClearSize = sysVarInfo.curMaxVarAddr * DOUBLE_SIZE;
		memoryClear((uint8*)memPointer.autoMemPtr, autoVarClearSize);

		
		status = fbSysExeVarInfoRead(MMEM_VAR , &sysVarInfo);
	    if(status != NO_ERROR) {
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}
		flagVarClearSize = sysVarInfo.curMaxVarAddr * DOUBLE_SIZE;
		memoryClear((uint8*)memPointer.flagMemPtr, flagVarClearSize);

		
		memoryClear((uint8*)memPointer.stSharedMemPtr, MAX_SHARED_MEM_SIZE);
		memoryClear((uint8*)memPointer.retainMemPtr, MAX_MPU_RETAIN_FLAG_MEM_SIZE);
	}
	
	return(status);
}


uint32 rmtFlagVarNumWrite(uint32 dataAccType, uint32 numOfData, uint32 backupFlag, 
							   uint32 destVarAddr, uint32 *writeData)
{
	uint32 status = NO_ERROR;
	uint32	accDataType, byteNum, offSet, maxVarMem, bitMask;
	strSysConfigInfo sysCnfgInfo;
	uint8  *destPtr;
	
	
	 if(dataAccType == SPEC_SIZE_BYTE){
		maxVarMem = MAX_MPU_MMEM_NUM *4;
		accDataType = BUS_BYTE_ACC;
		byteNum = numOfData*BUS_BYTE_ACC;
	}
	else if(dataAccType == SPEC_SIZE_WORD) {
		maxVarMem = MAX_MPU_MMEM_NUM *2;
		accDataType = BUS_WORD_ACC;
		byteNum = numOfData * BUS_WORD_ACC;
	}
	else if(dataAccType == SPEC_SIZE_DWORD) {
		maxVarMem = MAX_MPU_MMEM_NUM;
		accDataType = BUS_LONG_ACC;
		byteNum = numOfData * BUS_LONG_ACC;
	}
	else if(dataAccType == SPEC_SIZE_LWORD){
		maxVarMem = MAX_MPU_MMEM_NUM /2;
		accDataType = BUS_DLONG_ACC;
		byteNum = numOfData*BUS_DLONG_ACC;
	}
	else  {
		maxVarMem = 0;
	   	status = RMT_DATA_ACC_TYPE_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}


    if( numOfData > MAX_SHARED_ACC_DATA_NUM){
    	status = RMT_DATA_ACC_NUM_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
    }

    
	if( (destVarAddr<0) || (destVarAddr >= maxVarMem)){
		status = VARIABLE_ADDR_RANGE_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

	if( (gSysRunningInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN) &&
		(gSysRunningInfoPtr->sysOperationMode == SYS_RAS_SYSTEM_AUTO_MODE) &&
		(gDiuRasInfoPtr->redundancyStatus == NO_ERROR)
	){
		
		systemCnfgInfoRead(&sysCnfgInfo);
		
		if(sysCnfgInfo.shelfType == MASTER_SHELF)
       		offSet = MEM_BOARD_BUS_ADDR_ON_SLAVE;
		else if(sysCnfgInfo.shelfType == SLAVE_SHELF) 
       		offSet = MEM_BOARD_BUS_ADDR_ON_MASTER;
		else {
			status = STATION_SHELF_CONFIG_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}

		bitMask = 0x00ffffff;
		destPtr = (uint8*)(( ((uint32)gMmemVariableBusPtr -(uint32)gBusMpuFbStartAddr )&bitMask)+offSet );
		destPtr += ( destVarAddr * accDataType);
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
		status = myRmtAllTypeFlagDataWrite(accDataType, byteNum, backupFlag,
									(uint8*)destPtr, (uint8*)writeData);
#endif
		if(status) {
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		}
	}
MODULE_END:
	return(status);
}


uint32 rmtSharedVarNumWrite(	uint32 dataAccType, 
								uint32 numOfData, 
								uint32 destVarAddr, 
								uint32 *writeData
							)
{
	uint32 status = NO_ERROR;
	uint32	accDataType, byteNum, offSet, maxVarMem, bitMask, loop;
	strSysConfigInfo sysCnfgInfo;
	uint8  *destPtr, *srcPtr;
	uint8 writeCopyData[4];
	
	if(numOfData > 1) {status = 1; return (status);}
	
	if(dataAccType == RMT_LONG_ACC) {
		maxVarMem 	= MAX_SHARED_MEM_NUM;
		accDataType = BUS_LONG_ACC;
		byteNum 	= numOfData * BUS_LONG_ACC;
		*(uint32*)writeCopyData = *writeData;
	}
	else if(dataAccType == RMT_WORD_ACC) {
		maxVarMem 	= MAX_SHARED_MEM_NUM *2;
		accDataType = BUS_WORD_ACC;
		byteNum 	= numOfData * BUS_WORD_ACC;
		*(uint16*)writeCopyData = *writeData;
	}
	else if(dataAccType == RMT_BYTE_ACC){
		maxVarMem 	= MAX_SHARED_MEM_NUM *4;
		accDataType = BUS_BYTE_ACC;
		byteNum 	= numOfData*BUS_BYTE_ACC;
		*(uint8*)writeCopyData = *writeData;
	}
	else {
		maxVarMem = 0;
	   	status = RMT_DATA_ACC_TYPE_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

    if( numOfData > MAX_SHARED_ACC_DATA_NUM){
    	status = RMT_DATA_ACC_NUM_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
    }

    
	if( (destVarAddr < 0) || (destVarAddr >= maxVarMem)){
		status = VARIABLE_ADDR_RANGE_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

	if( (gSysRunningInfoPtr->sysRedundancyStatus 	== SYS_RAS_REDUNDANCY_RUN) &&
		(gSysRunningInfoPtr->sysOperationMode 		== SYS_RAS_SYSTEM_AUTO_MODE) &&
		(gDiuRasInfoPtr->redundancyStatus 			== NO_ERROR)
	){
		
		systemCnfgInfoRead(&sysCnfgInfo);
		
		if(sysCnfgInfo.shelfType == MASTER_SHELF)
       		offSet = MEM_BOARD_BUS_ADDR_ON_SLAVE;
		else if(sysCnfgInfo.shelfType == SLAVE_SHELF) 
       		offSet = MEM_BOARD_BUS_ADDR_ON_MASTER;
		else {
			status = STATION_SHELF_CONFIG_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
		bitMask = 0x00ffffff;
		destPtr = (uint8*)(( ((uint32)gSTSharedMemDataBusPtr -(uint32)gBusMpuFbStartAddr )&bitMask)+offSet );
		destPtr += ( destVarAddr * accDataType);

		status = myRmtAllTypeFlagDataWrite(accDataType, byteNum, EN_RETAIN,
									(uint8*)destPtr, (uint8*)writeCopyData/*writeData*/);
		if(status) {
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		}
#else
		srcPtr = (uint8 *)writeData;
		
		bitMask = 0x00ffffff;
		destPtr = (uint8 *)((uint32)gSTSharedMemDataBusPtr&bitMask) + offSet;
		destPtr += ( destVarAddr * accDataType);


		
		
		for(loop = 0; loop < byteNum; loop++, destPtr++, srcPtr++) {
			status = myBusDataAcc(	__FILE__,
									__LINE__,	
									BUS_BYTE_ACC, 
									BUS_DATA_WRITE ,
									destPtr, 
									srcPtr
							 	);

			if(status) 
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		}
#endif

	}
MODULE_END:
	
	return(status);
}

uint32 refDataCopyFromBusMem	(	uint32	*retSpecDataPtr,
								uint32	specData,
								strFbSpecActionInfo specActionData,
								uint32	specType, 
								uint32  tcType
							)
{
	uint32 status = NO_ERROR;
	uint32 *outputStartPtr;
	uint32 memoryAddress,index;
	
	strFbCodeTypePointerInfo readFcodeTypePInfo ;
	uint32  outputBlockAddr, sindex;
	uint8 booldata = 0;
	uint8* srcPtr = NULL;

	uint32	kindTypeConversion = 0;
	uint32	kindSizeConversion = 0;

	int32	int32Buf;
#ifdef USED_BIT64
	int64	int64Buf;
	uint64	uint64Buf;
	double	doubleBuf;
#endif
	float	floatBuf;
	int8	int8Buf;
	int16   int16Buf;
	uint32  uint32Buf;

	strFbMemInfo *fbMemInfoPtr;
    strFbTaskTblMemInfo	segInfo;
	uint32			fbBlkAddress = 0;
	strFbMemInfo	fbMemInfo;

	uint32*	uint32SrcPtr = NULL;
	strSysConfigInfo 	sysCnfgInfo; 	
	int32 anaPointStartPosition, ioPointDigAnaKind;

	if(specActionData.refType != SPEC_TYPE_BYREF) {
		status = FB_SPEC_ACTION_REF_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
	}

	systemCnfgInfoRead( &sysCnfgInfo);

	memoryAddress = specData>>8;
	index = (specData>>2)&0x3f; 

	memoryClear((uint8*)retSpecDataPtr, DOUBLE_SIZE);

	switch(specActionData.memoryType) 
	{
		case F_MEMORY:	
			if( (memoryAddress<1) || 
				(memoryAddress>MAX_MPU_BLOCK_ADDR) ||
				(index == 0x0)	
			) {
				status = FB_SPEC_ACTION_ADDR_ERR;
				setErrorCodeWithVal(__FILE__,__LINE__,__FUNCTION__, status,
									"Mem Addr", memoryAddress,
									"Output Index", index,
									"Not Using",0
								);
				break;
			}
			else {
				
				status = fbCodeMemPointerGet(	LOGIC_ID, memoryAddress, &fbMemInfoPtr);
				if(status) {
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				}

				status = fbCodeParaTypePointerGet	(
											fbMemInfoPtr->fbId, 
											&readFcodeTypePInfo	);
				if(status) {
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				}
			}

			if( (index > fbMemInfoPtr->outputNo) || (index == 0) ) {
				status = FB_SPEC_ACTION_ADDR_ERR;
				setErrorCodeWithVal(__FILE__,__LINE__,__FUNCTION__, status,
									"FB CODE", fbMemInfoPtr->fbId,
									"MAX Output Index", fbMemInfoPtr->outputNo,
									"Revc Index", index
							     	);
				break;
			}
			
			
			sindex = *((uint32 *)readFcodeTypePInfo.outputSizeStartAddr + (index-1));

			outputStartPtr		= (uint32 *)gMpuOutputDataBusPtr;
			outputBlockAddr 	= (uint32)fbMemInfoPtr->outputIndex + sindex;

			srcPtr =  (uint8*)((uint32 *)outputStartPtr + outputBlockAddr);
			
			if(GET_TYPE_SIZE(specType)==SIZE32_TYPE) 
			{	
				memoryCopy ((uint8*)retSpecDataPtr, (uint8*)srcPtr, 4);
			} 
#ifdef USED_BIT64
			else if(GET_TYPE_SIZE(specType)==SIZE64_TYPE) 
			{	
				if(specActionData.dataSize==SPEC_SIZE_LWORD)
					memoryCopy ((uint8*)retSpecDataPtr, (uint8*)srcPtr, 8);
				else{
					 
					memoryCopy ((uint8*)retSpecDataPtr, (uint8*)srcPtr, 4);
					*(retSpecDataPtr+1) = 0;
				}
			} 
#endif
			else {
				status = FB_SPEC_TYPE_SIZE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				break;
			}

		break;

		case A_MEMORY:	
			if( (memoryAddress<1) || 
				(memoryAddress>MAX_MPU_AUTOVAR_NUM) || 
				(index != 0x0)	
			) {
				status = FB_SPEC_ACTION_ADDR_ERR;
				setErrorCodeWithVal(__FILE__,__LINE__,__FUNCTION__, status,
									"MEM ADDR", memoryAddress,
									"INDEX", index,
									"MEM TYPE",specActionData.memoryType									
								);
				break;
			}
			
			srcPtr = (uint8*)((uint32 *)gAutoVariableBusPtr + (memoryAddress - 1) );

			if(GET_TYPE_SIZE(specType)==SIZE32_TYPE)
			{	
				memoryCopy ((uint8*)retSpecDataPtr, (uint8*)srcPtr, 4);
			} 
#ifdef USED_BIT64
			else if(GET_TYPE_SIZE(specType)==SIZE64_TYPE)
			{	
				if(specActionData.dataSize==SPEC_SIZE_LWORD)
					memoryCopy ((uint8*)retSpecDataPtr, (uint8*)srcPtr, 8);
				else{
					 
					memoryCopy ((uint8*)retSpecDataPtr, (uint8*)srcPtr, 4);
					*(retSpecDataPtr+1) = 0;
				}
			} 
#endif
			else {
				status = FB_SPEC_TYPE_SIZE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				break;
			}
		break;

		case S_MEMORY:	
		case M_MEMORY:	
			
			if(specActionData.memoryType == S_MEMORY)
				srcPtr = (uint8*)gSTSharedMemDataBusPtr;
			
			
			else if(specActionData.memoryType == M_MEMORY)
				srcPtr = (uint8*)gMmemVariableBusPtr;
			
			if(GET_TYPE_SIZE(specType)==SIZE32_TYPE)
			{	
				memoryClear ((uint8*)retSpecDataPtr, 4);
			} 
#ifdef USED_BIT64
			else if(GET_TYPE_SIZE(specType)==SIZE64_TYPE)
			{	
				memoryClear ((uint8*)retSpecDataPtr,  8);
			} 
#endif
			else {
				status = FB_SPEC_TYPE_SIZE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
				break;
			}
			
			if(specActionData.bitposFlag==1) {
				if((specActionData.bitPosition < 0)) {
					status = FB_SPEC_ACTION_BITPOS_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				}
				else if((specActionData.dataSize==SPEC_SIZE_BYTE) && (specActionData.bitPosition < 8) )
				{
					srcPtr += memoryAddress * 1 ; 
					booldata = (uint8)((*(uint8 *)srcPtr)>>specActionData.bitPosition)&0x01;
					int32Buf = (uint32)booldata;
					fourBytesDataCopy((uint32*)retSpecDataPtr, (uint32*)&int32Buf ,1);
				} 
				else if((specActionData.dataSize==SPEC_SIZE_WORD) && (specActionData.bitPosition < 16 ))
				{
					if(memoryAddress>M_MEMORY_MAX_ADDR_WORD) {
						status = FB_SPEC_ACTION_ADDR_ERR;
						setErrorCodeWithVal(__FILE__,__LINE__,__FUNCTION__, status,
											"Memory Addr", memoryAddress,
											"MAX Addr", M_MEMORY_MAX_ADDR_WORD,
											"DATA SIZE", specActionData.dataSize
											);
						return (status);
					}

					srcPtr += memoryAddress * 2; 
					booldata = (uint8)((*(uint16 *)srcPtr)>>specActionData.bitPosition)&0x01;
					int32Buf = (uint32)booldata;
					fourBytesDataCopy((uint32*)retSpecDataPtr, (uint32*)&int32Buf ,1);
				} 
				else if((specActionData.dataSize==SPEC_SIZE_DWORD) && (specActionData.bitPosition < 32 ) )
				{

					if(memoryAddress>M_MEMORY_MAX_ADDR_DWORD) {
						status = FB_SPEC_ACTION_ADDR_ERR;
						setErrorCodeWithVal(__FILE__,__LINE__,__FUNCTION__, status,
											"Memory Addr", memoryAddress,
											"MAX Addr", M_MEMORY_MAX_ADDR_DWORD,
											"DATA SIZE", specActionData.dataSize
											);
						return (status);
					}
					srcPtr += memoryAddress * 4 ; 
					booldata = (uint8)((*(uint32 *)srcPtr)>>specActionData.bitPosition)&0x01;
					int32Buf = (uint32)booldata;
					fourBytesDataCopy((uint32*)retSpecDataPtr, (uint32*)&int32Buf ,1);
				} 
#ifdef USED_BIT64
				else if((specActionData.dataSize==SPEC_SIZE_LWORD) && (specActionData.bitPosition < 64 ) )
				{
					if(memoryAddress>M_MEMORY_MAX_ADDR_LWORD) {
						status = FB_SPEC_ACTION_ADDR_ERR;
						setErrorCodeWithVal(__FILE__,__LINE__,__FUNCTION__, status,
											"Memory Addr", memoryAddress,
											"MAX Addr", M_MEMORY_MAX_ADDR_LWORD,
											"DATA SIZE", specActionData.dataSize
										);
						return (status);
					}
					srcPtr += memoryAddress * 8 ; 
					booldata = (uint8)((*(uint64 *)srcPtr)>>specActionData.bitPosition)&0x01;
					int32Buf = (uint32)booldata;
					fourBytesDataCopy((uint32*)retSpecDataPtr, (uint32*)&int32Buf ,1);
				}
#endif
				else if((specActionData.dataSize==SPEC_SIZE_BIT)
						&& (specActionData.bitPosition == 0 ) )
				{
					srcPtr += memoryAddress/8 ; 
					booldata = (uint8)((*(uint8 *)srcPtr)>>memoryAddress%8)&0x01;
					int32Buf = (uint32)booldata;
					fourBytesDataCopy((uint32*)retSpecDataPtr, (uint32*)&int32Buf ,1);
				}
				else {
					status = FB_SPEC_ACTION_BITPOS_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				}
			}
			else
			{
				if(specActionData.dataSize==SPEC_SIZE_BYTE) {
					srcPtr += memoryAddress * 1 ; 

					int32Buf =(uint32)(*(uint8*)srcPtr);
					fourBytesDataCopy((uint32*)retSpecDataPtr, (uint32*)&int32Buf ,1);
				} 
				else if(specActionData.dataSize==SPEC_SIZE_WORD) {
					srcPtr += memoryAddress * 2; 
					
					int32Buf =(uint32)(*(uint16*)srcPtr);
					fourBytesDataCopy((uint32*)retSpecDataPtr, (uint32*)&int32Buf ,1);
				} 
				else if(specActionData.dataSize==SPEC_SIZE_DWORD) {
					srcPtr += memoryAddress * 4 ; 
					memoryCopy ((uint8*)retSpecDataPtr, (uint8*)srcPtr, 4);
				} 
#ifdef USED_BIT64
				else if(specActionData.dataSize==SPEC_SIZE_LWORD) {
					srcPtr += memoryAddress * 8 ; 
					memoryCopy ((uint8*)retSpecDataPtr, (uint8*)srcPtr, 8);
				} 
#endif
				else if(specActionData.dataSize==SPEC_SIZE_BIT) {
					srcPtr += memoryAddress/8 ;  
					booldata = (*srcPtr>>memoryAddress%8)&0x01;
					int32Buf =(uint32)booldata;
					fourBytesDataCopy((uint32*)retSpecDataPtr, (uint32*)&int32Buf ,1);
				} 
				else {
					status = FB_SPEC_ACTION_SIZE_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				}
			}

		break;

		default:
			status = VARIABLE_KIND_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		break;
	}

	
	if( (CHK_IS_ANY_TYPE(specType) ) && (tcType==TYPECASTING_ENABLE) &&
		(GET_BASIC_DATA_TYPE(specActionData.dataType)==INT_TYPE) &&
	    ( (GET_TYPE_DATA_SIZE(specType) >>12) > specActionData.dataSize) 
	) 
	{
		kindSizeConversion = GET_TYPE_DATA_SIZE(specType);
			
		if(GET_TYPE_DATA_SIZE(specActionData.dataSize<<12)==SIZE08_TYPE) 
		{
			int8Buf = (int8)(*(int32 *)retSpecDataPtr);
			switch(kindSizeConversion)
			{
			case SIZE16_TYPE:
				int16Buf = (int16)int8Buf;
				memoryCopy((uint8*)retSpecDataPtr, (uint8*)&int16Buf,WORD_SIZE);
#ifdef VXWORKS
				memoryCopy((uint8*)(((uint8*)retSpecDataPtr)+2), (uint8*)&int16Buf,WORD_SIZE);
#endif 
			break;
			case SIZE32_TYPE:
				*retSpecDataPtr = (int32)int8Buf;
			break;
#ifdef USED_BIT64
			case SIZE64_TYPE:
				int8Buf = (int8)(*(int32 *)retSpecDataPtr);
				int64Buf = (int64)(int8Buf);
				memoryCopy((uint8*)retSpecDataPtr, (uint8*)&int64Buf,LONG_SIZE);
			break;
#endif
			default:		
				status = ANY_FUNC_OUT_SIZE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			break;
			}
		}
		else if(GET_TYPE_DATA_SIZE(specActionData.dataSize<<12)==SIZE16_TYPE) 
		{
			switch(kindSizeConversion)
			{
			case SIZE32_TYPE:
				int16Buf = (int16)(*(int32 *)retSpecDataPtr);
				*retSpecDataPtr = (int32)int16Buf;
			break;
#ifdef USED_BIT64
			case SIZE64_TYPE:
				int16Buf = (int16)(*(int32 *)retSpecDataPtr);
				int64Buf = (int64)(int16Buf);
				memoryCopy((uint8*)retSpecDataPtr, (uint8*)&int64Buf,LONG_SIZE);
			break;
#endif
			default:		
				status = ANY_FUNC_OUT_SIZE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			break;
			}
		}
		else if(GET_TYPE_DATA_SIZE(specActionData.dataSize<<12)==SIZE32_TYPE) 
		{
			switch(kindSizeConversion)
			{
#ifdef USED_BIT64
			case SIZE64_TYPE:
				int64Buf = (int64)(*(int32 *)retSpecDataPtr);
				memoryCopy((uint8*)retSpecDataPtr, (uint8*)&int64Buf,LONG_SIZE);
			break;
#endif
			default:		
				status = ANY_FUNC_OUT_SIZE_ERR;
				setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
			break;
			}
		}
	}

	if( CHK_IS_ANY_TYPE(specType) && (tcType==TYPECASTING_ENABLE) 
						&&
		( (GET_BASIC_DATA_TYPE(specActionData.dataType) != GET_BASIC_DATA_TYPE(specType)) ||
		  ((specActionData.bitposFlag==1)&&(GET_TYPE_SIZE(specType)==SIZE64_TYPE)) )
	) {	
		
		if( (specActionData.dataSize==SPEC_SIZE_LWORD) &&
			(GET_TYPE_SIZE(specType)==SIZE64_TYPE) &&
			(specActionData.bitposFlag==0)
		) {
			status = FB_SPEC_ACTION_SIZE_ERR;
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
		}
			
#ifdef USED_BIT64
		if( (specActionData.bitposFlag==1) &&(GET_TYPE_SIZE(specType)==SIZE64_TYPE) )
			kindTypeConversion = (UINT_TYPE<<4) | GET_BASIC_DATA_TYPE(specType);
 		else 
			kindTypeConversion = ((GET_BASIC_DATA_TYPE(specActionData.dataType))<<4) | GET_BASIC_DATA_TYPE(specType);
#else
		kindTypeConversion = ((GET_BASIC_DATA_TYPE(specActionData.dataType))<<4) | GET_BASIC_DATA_TYPE(specType);
#endif

		
		switch(kindTypeConversion) 
		{
			case INT32_TO_REAL32:
				floatBuf = (float)(*(int32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&floatBuf),1);
				break;
			
#ifdef USED_BIT64
			case INT32_TO_INT64:
#ifdef VXWORKS			 
				mySwap((uint32*)retSpecDataPtr,(uint32*)retSpecDataPtr+1,DOUBLE_SIZE);
#endif	
				int64Buf = (int64)(*(int32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&int64Buf),2);
				break;
			case INT32_TO_UINT64:
#ifdef VXWORKS			 
				mySwap((uint32*)retSpecDataPtr,(uint32*)retSpecDataPtr+1,DOUBLE_SIZE);
#endif
				uint64Buf = (uint64)(*(int32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&uint64Buf),2);
				break;
			case INT32_TO_REAL64:
#ifdef VXWORKS			 
				mySwap((uint32*)retSpecDataPtr,(uint32*)retSpecDataPtr+1,DOUBLE_SIZE);
#endif
				doubleBuf = (double)(*(int32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&doubleBuf),2);
				break;
#endif
			case UINT32_TO_INT32: case TIME_TO_INT32: 
				int32Buf = (int32)(*(uint32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)&int32Buf,1);
				break;
			case UINT32_TO_REAL32: case TIME_TO_REAL32:
				floatBuf = (float)(*(uint32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&floatBuf),1);
				break;
#ifdef USED_BIT64
			case UINT32_TO_INT64: case TIME_TO_INT64:
				int64Buf = (int64)(*(uint32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&int64Buf),2);
				break;
			case UINT32_TO_UINT64: case TIME_TO_UINT64:
				uint64Buf = (uint64)(*(uint32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&uint64Buf),2);
				break;
			case UINT32_TO_REAL64: case TIME_TO_REAL64:
				doubleBuf = (double)(*(uint32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&doubleBuf),2);
				break;
			case REAL32_TO_REAL64:
				doubleBuf = (double)(*(float *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&doubleBuf),2);
				break;
			case INT64_TO_REAL64:
				doubleBuf = (double)(*(int64 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)((void*)&doubleBuf),2);
				break;
#endif
			case INT32_TO_UINT32: case INT32_TO_TIME:
				uint32Buf= (uint32)(*(int32 *)retSpecDataPtr);
				fourBytesDataCopy((uint32 *)retSpecDataPtr,(uint32 *)&uint32Buf,1);				
				break;
			case TIME_TO_UINT32: case UINT32_TO_TIME:
				break;
			default:
				status = FB_SPEC_ACTION_TYPE_ERR;
				setErrorCodeWithVal(__FILE__,__LINE__,__FUNCTION__, status,
									"Type-Conversion From", GET_BASIC_DATA_TYPE(specActionData.dataType),
									"Type-Conversion To", GET_BASIC_DATA_TYPE(specType),
									"Not Using", 0
								);
			break;
		}
	}

	return(status);
}

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)

strAccumBusInfo sAccumDirectData;
uint32 directAddrAccumDataToRmtMem(uint32 *setSpecDataPtr,uint32 specData,
						 strFbSpecActionInfo specActionData, uint32	specType)
{
	uint32 status = NO_ERROR;
	uint32 memoryAddress,index;
	uint8 booldata = 0;
	uint8 *destPtr= NULL, *BusPtr= NULL, tempData = 0;
	uint32	offSet, bitMask;

	uint8 uint8Data; uint16 uint16Data; 
	uint8 *writeSrcPtr=NULL;
	uint32 writeType =0;
	uint8 	byteData;
	uint16	wordData;
	uint32	longData;
	uint64	dLongData;
	
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	uint32 retainInfo=0,backupFlag;
	
	sAccumDirectData.accDataNum = 0;
#endif	
	if(gSysCnfgInfo.shelfType == MASTER_SHELF)
       	offSet = MEM_BOARD_BUS_ADDR_ON_SLAVE;
    else if(gSysCnfgInfo.shelfType == SLAVE_SHELF) 
       	offSet = MEM_BOARD_BUS_ADDR_ON_MASTER;
    else {
		status = STATION_SHELF_CONFIG_ERR;
		setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
        return(status);
    }

	if(specActionData.refType !=SPEC_TYPE_BYREF ){
		status = FB_SPEC_ACTION_REF_ERR;
		return(status);
	}
	
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	
	backupFlag = (EN_SAVE<<16);
	if( specActionData.memoryType==M_MEMORY ){
		status = varRetainInfoReadwithRef(specData, specActionData, (uint32*)&retainInfo);
#if 0 
		if((status==NO_ERROR)&&(retainInfo==NO_RETAIN))
			backupFlag = (NO_SAVE<<16);
#endif
	}
	else if(specActionData.memoryType != S_MEMORY ){
		status = VARIABLE_KIND_ERR;
		return(status);
	}
#endif	
	memoryAddress = specData>>8;
	index = (specData>>2)&0x3f; 

    
    
    
    bitMask = 0x00ffffff;
	
	switch(specActionData.memoryType) 
	{
		case S_MEMORY:
		case M_MEMORY:
			if(specActionData.memoryType == S_MEMORY) {
				
				destPtr = (uint8*)(( ((uint32)gSTSharedMemDataBusPtr -(uint32)gBusMpuFbStartAddr )&bitMask)+offSet );
				BusPtr = (uint8*)gSTSharedMemDataBusPtr;
			}
			else if(specActionData.memoryType == M_MEMORY) {
				
				destPtr = (uint8*)(( ((uint32)gMmemVariableBusPtr -(uint32)gBusMpuFbStartAddr )&bitMask)+offSet );
				BusPtr = (uint8*)gMmemVariableBusPtr;
			}			
			
			if(specActionData.bitposFlag==1) {
				if((specActionData.bitPosition < 0)) {
					status = FB_SPEC_ACTION_BITPOS_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				} 
				else if((specActionData.dataSize==SPEC_SIZE_BYTE)
						&& (specActionData.bitPosition < 8) )
				{
					BusPtr += memoryAddress * 1 ;
					destPtr += memoryAddress * 1 ; 

					
					booldata = *setSpecDataPtr&0x01;
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_BYTE_ACC, 	
											BUS_DATA_READ ,
											(uint8*)BusPtr, 
											(uint8*)&byteData 
										 );
					if(status)
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

					if(booldata ==1) 
						byteData |= (0x1 << specActionData.bitPosition);
					else if(booldata ==0) 
						byteData &= (~(0x1<< specActionData.bitPosition));

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
					writeSrcPtr = (uint8*)&byteData;

					writeType = BUS_BYTE_ACC;
					sAccumDirectData.accumPtr[sAccumDirectData.accDataNum] = (uint32*)destPtr;
					memoryCopy((uint8*)&sAccumDirectData.accumData[sAccumDirectData.accDataNum],
						(uint8*)writeSrcPtr, writeType );
					sAccumDirectData.accumType[sAccumDirectData.accDataNum] = backupFlag|writeType;
					sAccumDirectData.accDataNum++;

					if( writeType == BUS_DLONG_ACC) {
						writeType = BUS_LONG_ACC;
						sAccumDirectData.accumType[sAccumDirectData.accDataNum] = backupFlag|writeType;
						sAccumDirectData.accDataNum++;
						sAccumDirectData.accumPtr[sAccumDirectData.accDataNum] = (uint32*)(destPtr+4);
						memoryCopy((uint8*)&sAccumDirectData.accumData[sAccumDirectData.accDataNum],
							(uint8*)(writeSrcPtr+4), writeType );
					}
#endif
				} 
				else if((specActionData.dataSize==SPEC_SIZE_WORD)
						&& (specActionData.bitPosition < 16 ))
				{
					BusPtr += memoryAddress * 2 ;
					destPtr += memoryAddress * 2; 
					
					booldata = *setSpecDataPtr&0x01;
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_WORD_ACC, 	
											BUS_DATA_READ ,
											(uint8*)BusPtr, 
											(uint8*)&wordData 
										 );
					if(status)
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

					if(booldata ==1) 
						wordData |= (0x1 << specActionData.bitPosition);
					else if(booldata ==0) 
						wordData &= (~(0x1<< specActionData.bitPosition));

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
					writeSrcPtr = (uint8*)&wordData;

					writeType = BUS_WORD_ACC;
					sAccumDirectData.accumPtr[sAccumDirectData.accDataNum] = (uint32*)destPtr;
					memoryCopy((uint8*)&sAccumDirectData.accumData[sAccumDirectData.accDataNum],
						(uint8*)writeSrcPtr, writeType );
					sAccumDirectData.accumType[sAccumDirectData.accDataNum] = backupFlag|writeType;
					sAccumDirectData.accDataNum++;

					if( writeType == BUS_DLONG_ACC) {
						writeType = BUS_LONG_ACC;
						sAccumDirectData.accumType[sAccumDirectData.accDataNum] = backupFlag|writeType;
						sAccumDirectData.accDataNum++;
						sAccumDirectData.accumPtr[sAccumDirectData.accDataNum] = (uint32*)(destPtr+4);
						memoryCopy((uint8*)&sAccumDirectData.accumData[sAccumDirectData.accDataNum],
							(uint8*)(writeSrcPtr+4), writeType );
					}
#endif
				} 
				else if((specActionData.dataSize==SPEC_SIZE_DWORD)
						&& (specActionData.bitPosition < 32 ) )
				{
					BusPtr += memoryAddress * 4 ;
					destPtr += memoryAddress * 4 ; 
					
					
					booldata = *setSpecDataPtr&0x01;
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_LONG_ACC, 	
											BUS_DATA_READ ,
											(uint8*)BusPtr, 
											(uint8*)&longData 
										 );
					if(status)
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

					if(booldata ==1) 
						longData |= (0x1 << specActionData.bitPosition);
					else if(booldata ==0) 
						longData &= (~(0x1<< specActionData.bitPosition));

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
					writeSrcPtr = (uint8*)&longData;

					writeType = BUS_LONG_ACC;
					sAccumDirectData.accumPtr[sAccumDirectData.accDataNum] = (uint32*)destPtr;
					memoryCopy((uint8*)&sAccumDirectData.accumData[sAccumDirectData.accDataNum],
						(uint8*)writeSrcPtr, writeType );
					sAccumDirectData.accumType[sAccumDirectData.accDataNum] = backupFlag|writeType;
					sAccumDirectData.accDataNum++;

					if( writeType == BUS_DLONG_ACC) {
						writeType = BUS_LONG_ACC;
						sAccumDirectData.accumType[sAccumDirectData.accDataNum] = backupFlag|writeType;
						sAccumDirectData.accDataNum++;
						sAccumDirectData.accumPtr[sAccumDirectData.accDataNum] = (uint32*)(destPtr+4);
						memoryCopy((uint8*)&sAccumDirectData.accumData[sAccumDirectData.accDataNum],
							(uint8*)(writeSrcPtr+4), writeType );
					}
#endif
				} 
				else if((specActionData.dataSize==SPEC_SIZE_LWORD)
						&& (specActionData.bitPosition < 64 ) )
				{
					BusPtr += memoryAddress * 8 ;
					destPtr += memoryAddress * 8 ; 
					
					booldata = *setSpecDataPtr&0x01;
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_DLONG_ACC, 	
											BUS_DATA_READ ,
											(uint8*)BusPtr, 
											(uint8*)&dLongData 
										 );
					if(status)
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

					if(booldata ==1) 
						dLongData |= (0x1 << specActionData.bitPosition);
					else if(booldata ==0) 
						dLongData &= (~(0x1<< specActionData.bitPosition));

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
					writeSrcPtr = (uint8*)&dLongData;

					writeType = BUS_DLONG_ACC;
					sAccumDirectData.accumPtr[sAccumDirectData.accDataNum] = (uint32*)destPtr;
					memoryCopy((uint8*)&sAccumDirectData.accumData[sAccumDirectData.accDataNum],
						(uint8*)writeSrcPtr, writeType );
					sAccumDirectData.accumType[sAccumDirectData.accDataNum] = backupFlag|writeType;
					sAccumDirectData.accDataNum++;

					if( writeType == BUS_DLONG_ACC) {
						writeType = BUS_LONG_ACC;
						sAccumDirectData.accumType[sAccumDirectData.accDataNum] = backupFlag|writeType;
						sAccumDirectData.accDataNum++;
						sAccumDirectData.accumPtr[sAccumDirectData.accDataNum] = (uint32*)(destPtr+4);
						memoryCopy((uint8*)&sAccumDirectData.accumData[sAccumDirectData.accDataNum],
							(uint8*)(writeSrcPtr+4), writeType );
					}
#endif
				} 
				else if((specActionData.dataSize==SPEC_SIZE_BIT)
						&& (specActionData.bitPosition == 0 ) )
				{
					BusPtr += memoryAddress/8 ;
					destPtr += memoryAddress/8 ; 
					booldata = (*setSpecDataPtr)>>memoryAddress&0x01;

					
					booldata = *setSpecDataPtr&0x01;
					status = myBusDataAcc(	__FILE__,
											__LINE__,
											BUS_BYTE_ACC, 	
											BUS_DATA_READ ,
											(uint8*)BusPtr, 
											(uint8*)&byteData 
										 );
					if(status)
						setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);

					if(booldata ==1) 
						byteData |= (0x1 << specActionData.bitPosition);
					else if(booldata ==0) 
						byteData &= (~(0x1<< specActionData.bitPosition));
						
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
					writeSrcPtr = (uint8*)&byteData;

					writeType = BUS_BYTE_ACC;
					sAccumDirectData.accumPtr[sAccumDirectData.accDataNum] = (uint32*)destPtr;
					memoryCopy((uint8*)&sAccumDirectData.accumData[sAccumDirectData.accDataNum],
						(uint8*)writeSrcPtr, writeType );
					sAccumDirectData.accumType[sAccumDirectData.accDataNum] = backupFlag|writeType;
					sAccumDirectData.accDataNum++;

					if( writeType == BUS_DLONG_ACC) {
						writeType = BUS_LONG_ACC;
						sAccumDirectData.accumType[sAccumDirectData.accDataNum] = backupFlag|writeType;
						sAccumDirectData.accDataNum++;
						sAccumDirectData.accumPtr[sAccumDirectData.accDataNum] = (uint32*)(destPtr+4);
						memoryCopy((uint8*)&sAccumDirectData.accumData[sAccumDirectData.accDataNum],
							(uint8*)(writeSrcPtr+4), writeType );
					}
#endif
				} 
				else {
					status = FB_SPEC_ACTION_BITPOS_ERR;
					setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
					break;
				}
			}

			else
			{
				if(specActionData.dataSize==SPEC_SIZE_BYTE) {
					destPtr += memoryAddress * 1 ; 
					uint8Data = (uint8)(*setSpecDataPtr & 0xff);
					writeSrcPtr = (uint8*)&uint8Data;
					writeType = BUS_BYTE_ACC;
				} else if(specActionData.dataSize==SPEC_SIZE_WORD) {
					destPtr += memoryAddress * 2; 
					uint16Data = (uint16)(*setSpecDataPtr & 0xffff);
					writeSrcPtr = (uint8*)&uint16Data;
					writeType = BUS_WORD_ACC;
				} else if(specActionData.dataSize==SPEC_SIZE_DWORD) {
					destPtr += memoryAddress * 4 ; 
					writeSrcPtr = (uint8*)setSpecDataPtr;
					writeType = BUS_LONG_ACC;
				} else if(specActionData.dataSize==SPEC_SIZE_LWORD) {
					destPtr += memoryAddress * 8 ; 
					writeSrcPtr = (uint8*)setSpecDataPtr;
					writeType = BUS_DLONG_ACC;
				} 
				
				else if(specActionData.dataSize==SPEC_SIZE_BIT) {
					BusPtr += memoryAddress/8 ;
					destPtr += memoryAddress/8 ;  
					
					booldata = (*setSpecDataPtr)&0x01; 

					myBusDataAcc(__FILE__,__LINE__,BUS_BYTE_ACC, 	BUS_DATA_READ ,(uint8*)BusPtr, (uint8*)&tempData );
					if(booldata ==1) tempData |= (0x1 <<memoryAddress%8);
					else if(booldata ==0) tempData &= (~(0x1<< memoryAddress%8));
					writeSrcPtr = (uint8*)&tempData;
					writeType = BUS_BYTE_ACC;
				} 
				else {
					status = FB_SPEC_ACTION_SIZE_ERR;
					break;
				}

				if(status == NO_ERROR)
				{
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
					sAccumDirectData.accumPtr[sAccumDirectData.accDataNum] = (uint32*)destPtr;
					memoryCopy((uint8*)&sAccumDirectData.accumData[sAccumDirectData.accDataNum],
						(uint8*)writeSrcPtr, writeType );
					sAccumDirectData.accumType[sAccumDirectData.accDataNum] = backupFlag|writeType;
					sAccumDirectData.accDataNum++;

					if( writeType == BUS_DLONG_ACC) {
						writeType = BUS_LONG_ACC;
						sAccumDirectData.accumType[sAccumDirectData.accDataNum] = backupFlag|writeType;
						sAccumDirectData.accDataNum++;
						sAccumDirectData.accumPtr[sAccumDirectData.accDataNum] = (uint32*)(destPtr+4);
						memoryCopy((uint8*)&sAccumDirectData.accumData[sAccumDirectData.accDataNum],
							(uint8*)(writeSrcPtr+4), writeType );
					}
#endif				
				}
			}

		break;
		
		default:
			status = VARIABLE_KIND_ERR;
		break;
	}
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	
	if(sAccumDirectData.accDataNum >= MAX_DUAL_COPY_TYPE_NUM) 
	{
		status = myRmtRefRunDataWrite((strAccumBusInfo*)&sAccumDirectData);
		if(status != NO_ERROR)
			setErrorCode(__FILE__, __LINE__, __FUNCTION__, status);
	}
#endif
	
	return(status);
}
#endif 
