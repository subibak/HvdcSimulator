
#include	<stdio.h>
#include	<stdlib.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include	"fbMem.h"
#include	"usrDefBuf.h"
#include	"fbDef.h"
#include	"osDepend.h"
#include	"usrErrCode.h"
#include	"segScheduler.h"

#include	"systemDiag.h"

#include	"fbForcing.h"
#include	"sysConfig.h"	

#include	"network.h"
#include	"usrQueData.h"
#include	"msgQueData.h"
#include	"debugInfo.h"

/**************************************************************************
**		사용자 정의
**************************************************************************/
#define	MAX_SUPPORT_TYPE_NUM	18
#define	CHK_VALID_TYPE_INDEX(typeIndex)	( ( ( ( (typeIndex) >0 ) && ( (typeIndex) <= (MAX_SUPPORT_TYPE_NUM) ) ) ) ? 1 : 0)

/**************************************************************************
**		내부 변수
**************************************************************************/
strFbExecDbInfo	gFbCodeSortingTableInfo[MAX_FB_CODE_ID+1];
strFbCodeTypeInfo	gFbTypeTbl;
uint32	*gSpecTypePtr, *gVarTypePtr, *gOutputTypePtr;
uint32	*gSpecSizePtr,*gVarSizePtr, *gOutputSizePtr;


/**************************************************************************
**		외부 변수 
**************************************************************************/
extern int32 gDebugPortFd;
extern strSysRunningConfigInfo		*gSysRunningInfoPtr;
extern strFbExecDbInfo	gStandardFbDbTableInfo[MAX_FB_NUM + 1];
extern strFbExecDbInfo	gUsrDefFbDbTableInfo[MAX_FB_NUM + 1];

/**************************************************************************
**		내부함수 Prototype
**************************************************************************/
uint32		fbDbTableTypeInfoInit (void);
uint32 		fbDbTableSorting(void);
uint32		fbDbTableSortingInfoRead(uint32, strFbExecDbInfo *);
void		variableMemoryTest(void);
void		fbParaInMemDisplay(uint32, uint32);

static void fbCodeDefinedInfoDisplay(uint32, uint32 );
static void fbTaskInfoDisplay(uint32 , uint32 );

static void fbCodeInMemDisplay(uint32, uint32);
static void fbCodeInTaskDisplay(uint32, uint32);

static void fbMemDataDisplay(uint32, uint32);
static void fbCodeMemInfoDisplay(uint32, uint32 );

static void simulMemInfoDisplay(uint32, uint32);
static void simulMemInfoClearAll(uint32, uint32);
static void simulCodeInfoDisplay(uint32, uint32);

static void variableMemoryReadDisplay(void);
static void variableMemoryWriteDisplay(void);

static void fbInputHeaderInfoDisplay(uint32, uint32);

/**************************************************************************
**		외부함수 Prototype
**************************************************************************/
extern uint32	fbCodeSpecDataReadAll(strFbDataReadInfo*, uint32*, uint32);
extern uint32	fbCodeSpecDataReadWithType(strFbDataReadInfo*, uint32*, uint32);
extern uint32	fbCodeVarDataReadWithType(strFbDataReadInfo*, uint32*, uint32);
extern uint32	fbCodeOutputDataReadWithType(strFbDataReadInfo*, uint32*, uint32);

extern uint32	pcsModeChange(uint32,uint32,uint32, uint32, uint32	*);
extern uint32	refDataCopyFromMem(uint32*, uint32, strFbSpecActionInfo, uint32,uint32);
extern uint32	refDataCopyToMem(uint32*, uint32, strFbSpecActionInfo, uint32);
extern uint32	specDataNegated(strFbSpecActionInfo*, uint32*);
extern uint32	systemCnfgInfoRead( strSysConfigInfo *);	

extern void		systemInitErrWithVal(uint8 *,uint32,uint8 *,uint32, uint8 *,uint32,
							uint8 *,uint32,uint8 *,uint32);
extern void		screenClear(void);
extern int32	keyHit(void);
extern uint32	fbCodeSpecActionRead(strFbDataReadInfo*, strFbSpecActionInfo*, uint32);
extern void		cursorPositionSet(void);
extern void		sysTaskDelay (int32 );
extern uint32	fbCodeVarDataRead(uint32 ,uint32 ,uint32 ,uint32 ,uint32 *);
extern int32	systemClockRateGet(void);
extern int32	 ioSysCtrlFunc(int32 , int32 , int32 );
extern uint32	getFbTaskTblInfo(uint32 , uint32 , strFbTaskTblMemInfo *);

extern void 	*memoryAlloc(int32 );
extern void		systemInitErr(uint8 *,uint32,uint8 *,uint32);
extern void		setErrorCodeWithVal(uint8 *,uint32,uint8 *,uint32, uint8 *,uint32,
							uint8 *,uint32,uint8 *,uint32);
extern void		memoryClear ( uint8 *, uint32 );
extern void		fourBytesDataCopy ( uint32 *, uint32 *, uint32 );
extern void		memoryCopy ( uint8 *, uint8 *, uint32 );
extern int32	ioRead(int32 , int8 *, int32 );
extern void		spaceSkip(int8 **);
extern uint32	moduleInfoRead(uint32 ,strModuleInfo	*);
extern uint32	fbCodeMemInfoRead(uint32, uint32, strFbMemInfo *);
extern uint32	fbCodeParaTypePointerGet(uint32	,strFbCodeTypePointerInfo	*);
extern uint32   varRetainInfoReadwithRef(uint32, strFbSpecActionInfo, uint32*);
extern uint32   refDataCopyToBusMem(uint32*, uint32, strFbSpecActionInfo, uint32);
extern uint32   refDataCopyToRmtMem(uint32*, uint32, strFbSpecActionInfo, uint32);
extern uint32   fbMemPointerGet(uint32, strMemPointerInfo *);
extern uint32	fbSysExeModuleInfoRead( uint32, strModuleExecInfo* );

extern	uint32  busFbCodeMemInfoRead(uint32	, uint32, strFbMemInfo*);

/*********************************************************************
**	함수명		: fbDbTableTypeInfoInit
**********************************************************************/
uint32	fbDbTableTypeInfoInit (void)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32	i, memSize, fbId, inputNo, intlVarNo, outputNo;
	uint32  j, inputSize, intlVarSize, outputSize;
    uint32	specTypeIndex, varTypeIndex, outputTypeIndex;
	uint32	specType[1024], varType[1024], outputType[1024];

	uint32	specTypeSize[1024], varTypeSize[1024], outputTypeSize[1024];
   	strFbExecDbInfo	fbCodeInfo, *fbCodePtr;
	strFbDefInfo	fbParaInfo;
	uint32	numOfSpec, numOfVar, numOfOutput;
	uint32	*tmpPtr;
	strFbTypeIndexInfo	*indexPtr;

	memSize = sizeof(strFbTypeIndexInfo) * MAX_FB_NUM;

    gFbTypeTbl.indexPtr = (strFbTypeIndexInfo *) memoryAlloc(memSize);

    indexPtr = gFbTypeTbl.indexPtr;

    if(indexPtr == (strFbTypeIndexInfo *)NULL) 
    {
    	status = MEMORY_ALLOC_ERR;
        systemInitErr (__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}

    
	memoryClear ( (uint8 *) indexPtr, memSize);
	gFbTypeTbl.totalFbCodeNum	= 0;

	
    numOfSpec 	= 0;
    numOfVar	= 0;
    numOfOutput = 0;

	for(i = 1; i < MAX_FB_CODE_ID+1; i++) 
	{
    	memoryClear	((uint8 *) &fbCodeInfo, sizeof(strFbExecDbInfo));

    	retVal = fbDbTableSortingInfoRead(i, &fbCodeInfo);

        if(retVal != NO_ERROR) continue;

	   	numOfSpec 	+= fbCodeInfo.inputNo;
   		numOfVar 	+= fbCodeInfo.intlVarNo;
   		numOfOutput	+= fbCodeInfo.outputNo;
	}	
	
	memSize = DOUBLE_SIZE * numOfSpec;
	gSpecTypePtr = (uint32 *)memoryAlloc(memSize);
	
	
    if(gSpecTypePtr == (uint32 *)NULL) 
    {
    	status = MEMORY_ALLOC_ERR;
        systemInitErr (__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}
	memoryClear ( (uint8 *) gSpecTypePtr, memSize);

	if(numOfVar != 0) 
	{
		memSize = DOUBLE_SIZE * numOfVar;
		gVarTypePtr = (uint32 *)memoryAlloc(memSize);
		
	    if(gVarTypePtr == (uint32 *)NULL) 
	    {
    		status = MEMORY_ALLOC_ERR;
	        systemInitErr (__FILE__, __LINE__, __FUNCTION__, status);
			goto MODULE_END;
		}
       	memoryClear ( (uint8 *) gVarTypePtr, memSize);

	}	
	memSize = DOUBLE_SIZE * numOfOutput;
	gOutputTypePtr = (uint32 *)memoryAlloc(memSize);
	
    if(gOutputTypePtr == (uint32 *)NULL) 
    {
    	status = MEMORY_ALLOC_ERR;
        systemInitErr (__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}
   	memoryClear ( (uint8 *) gOutputTypePtr, memSize);


	memSize = DOUBLE_SIZE * numOfSpec;
	gSpecSizePtr = (uint32 *)memoryAlloc(memSize);
	
    if(gSpecSizePtr == (uint32 *)NULL) 
    {
    	status = MEMORY_ALLOC_ERR;
        systemInitErr (__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}
	memoryClear ( (uint8 *) gSpecSizePtr, memSize);

	memSize = DOUBLE_SIZE * numOfVar;
	if(numOfVar != 0) 
	{
		gVarSizePtr = (uint32 *)memoryAlloc(memSize);
		
	    if(gVarSizePtr == (uint32 *)NULL) 
	    {
    		status = MEMORY_ALLOC_ERR;
	        systemInitErr (__FILE__, __LINE__, __FUNCTION__, status);
			goto MODULE_END;
		}
       	memoryClear ( (uint8 *) gVarSizePtr, memSize);

	}	

	memSize = DOUBLE_SIZE * numOfOutput;
	gOutputSizePtr = (uint32 *)memoryAlloc(memSize);
	
    if(gOutputSizePtr == (uint32 *)NULL) 
    {
    	status = MEMORY_ALLOC_ERR;
        systemInitErr (__FILE__, __LINE__, __FUNCTION__, status);
		goto MODULE_END;
	}
   	memoryClear ( (uint8 *) gOutputSizePtr, memSize);

    specTypeIndex 	= 0;
    varTypeIndex	= 0;
    outputTypeIndex	= 0;

    for(i = 1; i < MAX_FB_CODE_ID+1; i++ )
    {
		memoryClear (	(uint8 *)&fbParaInfo, sizeof(strFbDefInfo));
	
    	retVal = fbDbTableSortingInfoRead(i, &fbCodeInfo);

        if((retVal != NO_ERROR) || (fbCodeInfo.fbId == 0)) continue;
		
	    memoryClear ( 	(uint8 *)&specType[0], 1024*4);
		memoryClear ( 	(uint8 *)&varType[0], 1024*4);
		memoryClear ( 	(uint8 *)&outputType[0], 1024*4);

    	fbId 		= fbCodeInfo.fbId;
	   	inputNo 		= fbCodeInfo.inputNo;
   		intlVarNo 		= fbCodeInfo.intlVarNo;
   		outputNo 	= fbCodeInfo.outputNo;

		  
		if(	(fbId 	== 0) || 
			(inputNo 	== 0) ||
			(outputNo 	== 0)
		) {
			status = FB_PARA_DEFINE_ERR;
	        systemInitErrWithVal (	__FILE__,__LINE__,__FUNCTION__, status,
	                                (uint8 *)"FB CODE ID", 	fbId,
	                                (uint8 *)"SPEC NO", 	inputNo,
	                                (uint8 *)"OUTPUT NO", 	outputNo
							);
 			goto MODULE_END;
        }
        
    	status = fbCodeInfo.fbInitFunc( 	&specType[0],
        										&varType[0],
                               					&outputType[0],
                                                &fbParaInfo
											);
		if(status != NO_ERROR) {
			status = FB_INIT_FUNC_CALL_ERR;
	        systemInitErr (__FILE__, __LINE__, __FUNCTION__, status);
			goto MODULE_END;
        }

        
        
        if( (fbId 	!= fbParaInfo.fbId) 	|| 
        	(inputNo 	!= fbParaInfo.inputNo) 	||
        	(intlVarNo 		!= fbParaInfo.intlVarNo)	||	 		
        	(outputNo 	!= fbParaInfo.outputNo) 
		) {
			status = FB_PARA_DEFINE_ERR;
	        systemInitErrWithVal (	__FILE__,__LINE__,__FUNCTION__, status,
			                        (uint8 *)"INDEX", 	i,
			                        (uint8 *)"FB CODE ID", 	fbParaInfo.fbId,
			                        (uint8 *)"SPEC NO", 	fbParaInfo.inputNo
                       
					);

			goto MODULE_END;
        }
        
        
		tmpPtr = gSpecTypePtr + specTypeIndex;
        memoryCopy (	(uint8 *) tmpPtr,
        				(uint8 *) &specType[0],
                        inputNo * 4
					);

		tmpPtr = gVarTypePtr + varTypeIndex;
        memoryCopy (	(uint8 *) tmpPtr,
        				(uint8 *) &varType[0],
                        intlVarNo * 4
					);

		tmpPtr = gOutputTypePtr + outputTypeIndex;
        memoryCopy (	(uint8 *) tmpPtr,
        				(uint8 *) &outputType[0],
                        outputNo * 4
					);
        
        
        indexPtr->fbInfo.fbId 	= fbId;
        indexPtr->fbInfo.inputNo 	= inputNo;
        indexPtr->fbInfo.intlVarNo 		= intlVarNo;
        indexPtr->fbInfo.outputNo 	= outputNo;

        indexPtr->specTypeIndex 	= specTypeIndex;
        indexPtr->varTypeIndex 		= varTypeIndex;
        indexPtr->outputTypeIndex 	= outputTypeIndex;

		gFbTypeTbl.totalFbCodeNum++;

		
        
		fbCodePtr = &gFbCodeSortingTableInfo[fbId];
		fbCodePtr->fbCodeTypeIndexPtr = (uint32 *)indexPtr;

		memoryClear ((uint8 *)&specTypeSize[0], 1024*4);
		memoryClear ((uint8 *)&varTypeSize[0], 1024*4);
		memoryClear ((uint8 *)&outputTypeSize[0], 1024*4);

		inputSize = 0;
		for (j=0;j<inputNo;j++)
		{	
			specTypeSize[j] = inputSize;
			inputSize ++;
			
			if(GET_TYPE_SIZE(specType[j])==SIZE64_TYPE) inputSize++;

		}
		fbCodePtr->inputSize = inputSize;

		intlVarSize = 0;
		for (j=0;j<intlVarNo;j++)
		{	
			varTypeSize[j] = intlVarSize;
			intlVarSize ++;
			
			if(GET_TYPE_SIZE(varType[j])==SIZE64_TYPE) intlVarSize++;
		}
		fbCodePtr->intlVarSize = intlVarSize;

		outputSize = 0;
		for (j=0;j<outputNo;j++)
		{	
			outputTypeSize[j] = outputSize;
			outputSize ++;
			
			if(GET_TYPE_SIZE(outputType[j])==SIZE64_TYPE) outputSize++;
		}
		fbCodePtr->outputSize = outputSize;

		tmpPtr = gSpecSizePtr + specTypeIndex;
        memoryCopy (	(uint8 *) tmpPtr,
        				(uint8 *) &specTypeSize[0],
                        inputNo * 4
					);
		
		tmpPtr = gVarSizePtr + varTypeIndex;
        memoryCopy (	(uint8 *) tmpPtr,
        				(uint8 *) &varTypeSize[0],
                        intlVarNo * 4
					);

		tmpPtr = gOutputSizePtr + outputTypeIndex;
        memoryCopy (	(uint8 *) tmpPtr,
        				(uint8 *) &outputTypeSize[0],
                        outputNo * 4
					);

        indexPtr++;
       
        specTypeIndex 	+= inputNo;
        varTypeIndex	+= intlVarNo;
        outputTypeIndex	+= outputNo;

        if(	(specTypeIndex > numOfSpec) ||
        	(varTypeIndex > numOfVar) ||
            (outputTypeIndex > numOfOutput)
		) {
        	status = FB_PARA_INDEX_ERR;
	        systemInitErrWithVal (	__FILE__,__LINE__,__FUNCTION__, status,
	                                (uint8 *)"Total VAR NO", 	numOfVar,
	                                (uint8 *)"Total SPEC NO", 	numOfSpec,
	                                (uint8 *)"Total OUTPUT NO",	numOfOutput
								);
	        systemInitErrWithVal (	__FILE__,__LINE__,__FUNCTION__, status,
	                                (uint8 *)"Calcu VAR Index", 	varTypeIndex,
	                                (uint8 *)"Calcu SPEC Index", 	specTypeIndex,
	                                (uint8 *)"Calcu OUTPUT Index",	outputTypeIndex
								);

			goto MODULE_END;
        }
	}

MODULE_END:
	
    return(status);
}

uint32 fbDbTableSorting(void)
{
	uint32	status = NO_ERROR;
	uint32	index;
	strFbExecDbInfo	*srcDataPtr, *destDataPtr;

    memoryClear ( 	(uint8 *) &gFbCodeSortingTableInfo[0],
    				(uint32) sizeof(strFbExecDbInfo) * (MAX_FB_CODE_ID+1)
				);

	/********************************************************************
	**	IEC61131-3	Standard FB Sorting
	********************************************************************/				
    for(index = 0; index < (MAX_FB_NUM+1); index++) 
    {
		srcDataPtr = &gStandardFbDbTableInfo[index];
    
        if( srcDataPtr->fbId == 0) continue;
    
        if(srcDataPtr->fbId > MAX_FB_CODE_ID) 
        {
        	status = FB_CODE_DEFINE_ERR;
    		systemInitErrWithVal (	__FILE__, __LINE__, __FUNCTION__, status,
            	                    (uint8 *)"Fb Code Id", 	srcDataPtr->fbId,
                	                (uint8 *)"MAX FB CODE ID", 	MAX_FB_CODE_ID,
                    	            (uint8 *)"NOT USING",	0
								);
            goto MODULE_END;
		}
		
        destDataPtr = &gFbCodeSortingTableInfo[srcDataPtr->fbId];
        
        /* FB Code ID가 중복되었는지  Check */
        if(destDataPtr->fbId == srcDataPtr->fbId)
        {
        	status = FB_CODE_DUAL_DEFINE_ERR;
    		systemInitErrWithVal (	__FILE__, __LINE__, __FUNCTION__, status,
            	                    (uint8 *)"DEST Fb Code Id", destDataPtr->fbId,
            	                    (uint8 *)"SRC Fb Code Id", 	srcDataPtr->fbId,
                    	            (uint8 *)"NOT USING",	0
								);
            goto MODULE_END;
        	
        }
        else
        {
			memoryCopy((uint8 *) destDataPtr, (uint8 *) srcDataPtr,sizeof(strFbExecDbInfo));
		}
    }

	/********************************************************************
	**	사용자 정의 FB Sorting
	********************************************************************/				
    for(index = 0; index < (MAX_FB_NUM+1); index++) 
    {
		srcDataPtr = &gUsrDefFbDbTableInfo[index];
    
        if( srcDataPtr->fbId == 0) continue;
    
        if(srcDataPtr->fbId > MAX_FB_CODE_ID) 
        {
        	status = FB_CODE_DEFINE_ERR;
    		systemInitErrWithVal (	__FILE__, __LINE__, __FUNCTION__, status,
            	                    (uint8 *)"Fb Code Id", 	srcDataPtr->fbId,
                	                (uint8 *)"MAX FB CODE ID", 	MAX_FB_CODE_ID,
                    	            (uint8 *)"NOT USING",	0
								);
            goto MODULE_END;
		}
		
        destDataPtr = &gFbCodeSortingTableInfo[srcDataPtr->fbId];
        
        /* FB Code ID가 중복되었는지  Check */
        if(destDataPtr->fbId == srcDataPtr->fbId)
        {
        	status = FB_CODE_DUAL_DEFINE_ERR;
    		systemInitErrWithVal (	__FILE__, __LINE__, __FUNCTION__, status,
            	                    (uint8 *)"DEST Fb Code Id", destDataPtr->fbId,
            	                    (uint8 *)"SRC Fb Code Id", 	srcDataPtr->fbId,
                    	            (uint8 *)"NOT USING",	0
								);
            goto MODULE_END;
        	
        }
        else
        {
			memoryCopy((uint8 *) destDataPtr, (uint8 *) srcDataPtr,sizeof(strFbExecDbInfo));
		}
    }
    
MODULE_END:
	
    return(status);
}

uint32	fbDbTableSortingInfoRead
					(
			uint32	fbCodeNum,
            strFbExecDbInfo	*fbCodePtr
            		)
{
	uint32	status = NO_ERROR;
	strFbExecDbInfo	*dataPtr;

    if(fbCodeNum > MAX_FB_CODE_ID) 
    {
    	status = FB_CODE_ID_ERR;
        goto MODULE_END;
	}

    dataPtr = &gFbCodeSortingTableInfo[fbCodeNum];

    if( dataPtr->fbId == fbCodeNum)
    { 
    	fourBytesDataCopy ( (uint32 *) fbCodePtr,
        			 		(uint32 *) dataPtr,
                     		sizeof(strFbExecDbInfo)/4
						 );
	} 
    else
		status = FC_CODE_EXIST_ERR;

MODULE_END:
	
    return(status);
}

void usrDefineFbCodeInfoDisplay(uint32 moduleId, uint32 arg1)
{
	int8 line[16], *pLine, **pStr;
    int8    defaultStr[] = "\n\t *Unrecognized Command !!!\n";

	strDbgCmdFuncInfo fbDebugCmdFunc[] =
	{
		{"fdd",		NULL, 	NULL,	&fbCodeDefinedInfoDisplay},
		{"ffc",		NULL, 	NULL,	&simulMemInfoClearAll},
		{"ffd",		NULL, 	NULL,	&simulMemInfoDisplay},
		{"fid",		NULL, 	NULL,	&fbInputHeaderInfoDisplay},
		{"fmd",		NULL, 	NULL,	&fbCodeInMemDisplay},
		{"ftd",		NULL, 	NULL,	&fbCodeInTaskDisplay},
		{"rfd",		NULL, 	NULL,	&fbMemDataDisplay},
		{"rid",		NULL, 	NULL,	&simulCodeInfoDisplay},
		{"rmd",		NULL, 	NULL,	&fbCodeMemInfoDisplay},
		{"tid",		NULL, 	NULL,	&fbTaskInfoDisplay},
		{"NULL",	NULL, 		NULL,	NULL} 	 	
	};

	strDbgCmdFuncInfo *cmdFuncPtr;

    int8 *helpString[] = {
	    "*************************************",
		"* [fdd] - FB Code Define Info Display",
		"* [ffc] - FB Code Forcing Flag Clear All",
		"* [ffd] - FB Code Forcing Flag Display",
		"* [fid] - FB Input Data Display",
	    "* [fmd] - FB Code In Memory Display",
	    "* [ftd] - FB Code In Task Display",
		"* [rfd] - RunTime Fb Data Display",
	    "* [rid] - RunTime FB Code Info Display",    
	    "* [rmd] - Runtime FB Code Memory Info Display",
	    "* [tid] - Task Info Display",
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
			for(cmdFuncPtr = fbDebugCmdFunc; cmdFuncPtr->dbgFunc != NULL; cmdFuncPtr++)
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

static void fbCodeMemInfoDisplay(uint32 moduleId, uint32 arg1)
{
	uint32	status;
	int8	line[16], *cPtr;
	int32	blockAddr;
	uint32	maxBlockAddr;
    strModuleInfo	moduleInfo;
    strFbMemInfo	fbMemInfo;
	uint32  outputNegate[2];
	strSysRunningConfigInfo		*sysStatusInfoPtr = gSysRunningInfoPtr;

    status = moduleInfoRead(moduleId, &moduleInfo);
	if(status != NO_ERROR) {
		printf( "[ERROR] - moduleInfoRead() Call Err !!\n");
        return;
    }

    maxBlockAddr = moduleInfo.maxFbBlockAddr;

    printf("\tFb Block Addr To Display : ");
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    blockAddr = atoi((const char *)cPtr);

    if((blockAddr < 0) || (blockAddr > (int32)maxBlockAddr)) {
		printf("\t[ERROR] - Block Addr(%d) Err\n", blockAddr);
        return;
    }

   	memoryClear( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));

	
	
	if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN){
   		status = fbCodeMemInfoRead(	moduleId, blockAddr, &fbMemInfo);
		if(status != NO_ERROR) {
			printf("[ERROR] - fbCodeMemInfoRead() Call Err !!\n");
			return;
		}
	}
	else{
		status = busFbCodeMemInfoRead( moduleId, blockAddr, &fbMemInfo );
		if(status != NO_ERROR) {
			printf("[ERROR] - busFbCodeMemInfoRead() Call Err !!\n");
			return;
		}
	}

    if(fbMemInfo.fbId == 0) {
		printf("\tFunction Code is NULL In Block Addr(%d) !!!\n",
        						blockAddr
				);
		return;
    }

	memoryCopy((uint8*)&outputNegate[0],(uint8*)&fbMemInfo.outputAction,LONG_SIZE);

    printf( "\n************** << BlockAddr[%d] >> ********************************\n",
    							 blockAddr
				);
	printf( "   * Fb Segment ID     : %3d\n", fbMemInfo.fbTaskId);
	printf( "   * Fb Code           : %3d\n", fbMemInfo.fbId);
	printf( "   * Number Of Spec    : %3d\n", fbMemInfo.inputNo);
	printf( "   * Number Of Var     : %3d\n", fbMemInfo.intlVarNo);
	printf( "   * Number Of Output  : %3d\n", fbMemInfo.outputNo);
	printf( "   * InputHeader Index : %3d\n", fbMemInfo.specActionIndex);
	printf( "   * Input Index       : %3d\n", fbMemInfo.specIndex);
	printf( "   * Var Index         : %3d\n", fbMemInfo.varIndex);
	printf( "   * Output Index      : %3d\n", fbMemInfo.outputIndex);
	printf( "   * Simulation Flag   : 0x%x\n", fbMemInfo.forcingFlag );
	printf( "   * Status            : 0x%x\n", fbMemInfo.fbRunStatus);
	printf( "   * Input Size        : %3d\n", fbMemInfo.inputSize);
	printf( "   * Var Size          : %3d\n", fbMemInfo.intlVarSize);
	printf( "   * Output Size       : %3d\n", fbMemInfo.outputSize);
	printf( "   * Run Para Type     : 0x%x\n", fbMemInfo.paraRunType);
	printf( "   * Logic Type        : 0x%x\n", fbMemInfo.logicType);
	printf( "   * Using Type        : 0x%x\n", fbMemInfo.usingType);
	printf( "   * Action ID         : 0x%x\n", fbMemInfo.ActionID);
	printf( "   * Transition ID     : 0x%x\n", fbMemInfo.TransitionID);
	printf( "   * Exec Seq ID       : %3d\n", fbMemInfo.fbExecSequenceId);
#ifdef _WIN32
	printf( "   * Output Action     : 0x%08x%08x\n", outputNegate[1],outputNegate[0]);
#else
	printf( "   * Output Action     : 0x%08x%08x\n", outputNegate[0],outputNegate[1]);
#endif
    printf( "*****************************************************************\n");
}


static void fbInputHeaderInfoDisplay(uint32 moduleId, uint32 arg1)
{
	uint32	status;
	int8	line[16], *cPtr;
	int32	blockAddr;
	uint32	maxBlockAddr, inputIndex;
    strModuleInfo	moduleInfo;
    strFbMemInfo	fbMemInfo;

	uint32  specData[1024]; 
	strFbSpecActionInfo specActionData;
	strFbCodeTypePointerInfo typePInfo;
	uint32 *paraTypePtr,sIndex = 0;
#ifdef USED_BIT64
	union multiDataType64	data1;
#endif
	union multiDataType		data2;

	strFbDataReadInfo inputHeaderInfo;
	strSysRunningConfigInfo		*sysStatusInfoPtr = gSysRunningInfoPtr;
	
	uint8 *inputTypeStr[] = {
		(uint8 *)"ADDRESS",(uint8 *)"VALUE", (uint8 *)"NONE"
	};

	uint8 *memTypeStr[] = {
		(uint8 *)"NONE",(uint8 *)"FLAG(%M)", (uint8 *)"AUTO(%A)", (uint8 *)"INPUT(%I)",
		(uint8 *)"OUTPUT(%Q)", (uint8 *)"SHARED(%S)", (uint8 *)"FB/FU(%F)", (uint8 *)"TMTC(%T)",
		(uint8 *)"NONE"
	};

	uint8 *dataSizeStr[] = {
		(uint8 *)"NOBIT",(uint8 *)"BIT(X)", (uint8 *)"BYTE(B)", (uint8 *)"2 BYTE(W)",
		(uint8 *)"4 BYTE(D)", (uint8 *)"8 BYTE(L)",
		(uint8 *)"NONE"
	};
	uint8 *dataTypeStr[] = {
		(uint8 *)"NONE",(uint8 *)"INT32", (uint8 *)"UINT32", (uint8 *)"INT64",
		(uint8 *)"UINT64", (uint8 *)"REAL32", (uint8 *)"REAL64",
		(uint8 *)"NONE"
	};

	if(moduleId != LOGIC_ID) 
	{
		printf("\t[ERROR] - This Command Support In FB Logic !!!\n");
		return;	
	}
	
    status = moduleInfoRead(moduleId, &moduleInfo);
	if(status != NO_ERROR) {
		printf( "[ERROR] - moduleInfoRead() Call Err !!\n");
        return;
    }

    maxBlockAddr = moduleInfo.maxFbBlockAddr;

    printf("\tFb Block Addr To Input Display : ");
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    blockAddr = atoi((const char *)cPtr);

    if((blockAddr < 0) || (blockAddr > (int32)maxBlockAddr)) {
		printf("\t[ERROR] - Block Addr(%d) Err\n", blockAddr);
        return;
    }

   	memoryClear( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
   	status = fbCodeMemInfoRead(	moduleId,
       					    	blockAddr,
                                &fbMemInfo
							  );

	if(status != NO_ERROR) {
		printf("[ERROR] - fbCodeMemInfoRead() Call Err !!\n");
        return;
    }

    if(fbMemInfo.fbId == 0) {
		printf("\tFunction Code is NULL In Block Addr(%d) !!!\n",
        						blockAddr
				);
		return;
    }

	printf("\tInput Index To Display(1-%d) :  ",fbMemInfo.inputNo);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    inputIndex = atoi((const char *)cPtr);
	if((inputIndex <1) || (inputIndex >fbMemInfo.inputNo)){
		printf("\tMax Input Index Invalid!!!\n");
		return;
	}

	inputHeaderInfo.moduleId 	= moduleId;
	inputHeaderInfo.index 		= inputIndex;
	inputHeaderInfo.blockAddr 	= blockAddr;
	inputHeaderInfo.fbId 		= fbMemInfo.fbId;
	
	if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN)
	{
		fbCodeSpecActionRead(&inputHeaderInfo, &specActionData, LOCAL_MEM_ACC);
		fbCodeSpecDataReadAll(&inputHeaderInfo, (uint32 *)specData, LOCAL_MEM_ACC);

	}
	else
	{
		fbCodeSpecActionRead(&inputHeaderInfo, &specActionData, BUS_MEM_ACC);
		fbCodeSpecDataReadAll(&inputHeaderInfo, (uint32 *)specData, BUS_MEM_ACC);
	}

	if( fbCodeParaTypePointerGet(fbMemInfo.fbId, (strFbCodeTypePointerInfo *)&typePInfo) == NO_ERROR) 
	{
		sIndex = *((uint32 *)typePInfo.specSizeStartAddr + (inputIndex-1) );
		paraTypePtr = ( (uint32 *)typePInfo.specTypeStartAddr + (inputIndex-1) );
	}
	else 
		return;

    printf("\n");
    printf( "************** << BlockAddr[%d] >> ****************\n",blockAddr);
	printf( "   * Fb Code ID         : %3d\n",fbMemInfo.fbId);
	printf( "   * Fb Code Input      : %3d\n",inputIndex);
	printf( "   * Input Header Index : %3d\n",fbMemInfo.specActionIndex+(inputIndex-1));
	printf( "\n");

	printf( "   * Input Ref Type     : %s\n", inputTypeStr[specActionData.refType]);
	printf( "   * Negate Set Flag    : %3d\n",specActionData.negated);
	printf( "   * Referrence Type    : %s\n", memTypeStr[specActionData.memoryType]);
	printf( "   * Input Data Type    : %s\n", dataTypeStr[specActionData.dataType]);
	printf( "   * Input Data Size    : %s\n", dataSizeStr[specActionData.dataSize]);
	printf( "   * Bit Position Flag	: %3d\n", specActionData.bitposFlag);
	printf( "   * Bit Position(0-63)	: %3d\n", specActionData.bitPosition);
	printf( "   * Task ID            : %3d\n", specActionData.taskID);
	printf( "\n");

	if(specActionData.refType == SPEC_TYPE_BYREF) {
		printf( "   * Address Data       :   %d\n", (specData[sIndex] >> 8) & 0xffffff );
		printf( "   * Ref Index          :   %d\n", (specData[sIndex] >> 2) & 0x3f );
	}
	else {
		if( GET_TYPE_SIZE(*(paraTypePtr)) != SIZE64_TYPE ){
			memoryCopy( (uint8*)&data2, (uint8*)&specData[sIndex],DOUBLE_SIZE);
			
			printf( "   * Input Data(Hex)    : 0x%08x\n", data2.Hex_t);
			printf( "               (Int)    : %d\n",   data2.Int_t);
			printf( "               (UInt)   : %u\n", data2.Uint_t);
			printf( "               (Real)   : %f\n", data2.Float_t);
		}
#ifdef USED_BIT64
		else{
			memoryCopy( (uint8*)&data1, (uint8*)&specData[sIndex],LONG_SIZE);
#ifdef _WIN32
			printf( "   * Input Data(Hex)    : 0x%08x %08x\n", data1.halfUlint_t[1],data1.halfUlint_t[0]);
#else
			printf( "   * Input Data(Hex)    : 0x%08x %08x\n", data1.halfUlint_t[0],data1.halfUlint_t[1]);
#endif
			printf( "               (LInt)   : %u\n",(uint64)data1.LInt_t);
			printf( "               (ULInt)  : %u\n", (uint64)data1.Ulint_t);
			printf( "               (LReal)  : %f\n", data1.LReal_t);
		}
#endif
	}

    printf( "**************************************************\n");
}


static void fbMemDataDisplay(uint32 moduleId, uint32 arg1)
{
	uint32	status, i;
	int8	line[16], *cPtr;
	int32	blockAddr;
	uint32	maxBlockAddr;
    strModuleInfo	moduleInfo;
    strFbMemInfo	fbMemInfo;
    strFbExecDbInfo	fbCodeInfo;
	strFbCodeTypePointerInfo typeInfo;
	strSysRunningConfigInfo		*sysStatusInfoPtr = gSysRunningInfoPtr;

	strFbDataReadInfo readDataInfo;

	uint8 *typeStr[] = {
		(uint8 *)"N",(uint8 *)"I", (uint8 *)"UI",(uint8 *)"LI",(uint8 *)"ULI",
		(uint8 *)"R",(uint8 *)"LR",(uint8 *)"T", (uint8 *)"D", (uint8 *)"TD",
		(uint8 *)"DT",(uint8 *)"B",(uint8 *)"W",(uint8 *)"LW",
		(uint8 *)"A",(uint8 *)"AB",(uint8 *)"AN",(uint8 *)"AI",(uint8 *)"AR"
	};
	
	uint8 *memTypeStr[] = {
		(uint8 *)"N",(uint8 *)"M", (uint8 *)"A",(uint8 *)"I",(uint8 *)"Q",
		(uint8 *)"S",(uint8 *)"F",(uint8 *)"T"
	};

	uint8 *memSizeStr[] = {
		(uint8 *)"N",(uint8 *)"X",(uint8 *)"B", (uint8 *)"W",(uint8 *)"D",(uint8 *)"L"
	};

	uint8 *automemSizeStr[] = {
		(uint8 *)"N",(uint8 *)"D", (uint8 *)"D",(uint8 *)"L",(uint8 *)"L",
		(uint8 *)"D",(uint8 *)"L"
	};

	uint32	*specTypePtr, *saveSpecTypePtr,*specSizePtr, *saveSpecSizePtr;
	uint32	*varTypePtr, *saveVarTypePtr , *saveVarSizePtr;
	uint32	*outputTypePtr, *saveOutputTypePtr,*saveOutputSizePtr;
    uint32	typeIndex, factor = 2;
	union multiDataType64	data;
	uint32	paraType, specData[1024]; 
	strFbSpecActionInfo specActionData;
	uint32 outputNegated = 0;

	if(moduleId==LOGIC_ID) {
		factor = 2;
	} else {
		factor = 3;
	}

    status = moduleInfoRead(moduleId, &moduleInfo);
	if(status != NO_ERROR) {
		printf( "[ERROR] - moduleInfoRead() Call Err !!\n");
        return;
    }


    maxBlockAddr = moduleInfo.maxFbBlockAddr;

    printf("\tFb Block Addr To Display : ");
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    blockAddr = atoi((const char *)cPtr);

    if((blockAddr < 0) || (blockAddr > (int32)maxBlockAddr)) {
		printf("\t[ERROR] - Block Addr(%d) Err\n", blockAddr);
        return;
    }

   	memoryClear( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));

	
	
	if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN){
   		status = fbCodeMemInfoRead(	moduleId, blockAddr, &fbMemInfo);
		if(status != NO_ERROR) {
			printf("[ERROR] - fbCodeMemInfoRead() Call Err !!\n");
			return;
		}
	}
	else{
		status = busFbCodeMemInfoRead( moduleId, blockAddr, &fbMemInfo );
		if(status != NO_ERROR) {
			printf("[ERROR] - busFbCodeMemInfoRead() Call Err !!\n");
			return;
		}
	}


    if(fbMemInfo.fbId == 0) {
		printf("\tFunction Code is NULL In Block Addr(%d) !!!\n",
        						blockAddr
				);
		return;
    }

    status = fbDbTableSortingInfoRead (fbMemInfo.fbId, &fbCodeInfo);
	if(status != NO_ERROR) {
		printf("[ERROR] - fbDbTableSortingInfoRead() Call Err !!\n");
        return;
    }

	status = fbCodeParaTypePointerGet (fbMemInfo.fbId, &typeInfo);
	if(status != NO_ERROR) {
		printf("[ERROR] - fbCodeParaTypePointerGet() Call Err !!\n");
        return;
    }
    saveSpecTypePtr 	= (uint32 *)typeInfo.specTypeStartAddr;
    saveVarTypePtr 		= (uint32 *)typeInfo.varTypeStartAddr;
    saveOutputTypePtr 	= (uint32 *)typeInfo.outputTypeStartAddr;
   
	saveSpecSizePtr 	= (uint32 *)typeInfo.specSizeStartAddr;
    saveVarSizePtr 		= (uint32 *)typeInfo.varSizeStartAddr;
	saveOutputSizePtr 	= (uint32 *)typeInfo.outputSizeStartAddr;

    screenClear();

    while(!keyHit()) {
#ifdef _WINCE
		if((fbCodeInfo.inputNo + fbCodeInfo.intlVarNo + fbCodeInfo.outputNo)/factor > 20) 
			screenClear();
#endif
    	cursorPositionSet();
	    printf( "*************** << %7s[%03d] - BlockAddr[%d] >> *****************\n",
    							fbCodeInfo.fbNamePtr,fbMemInfo.fbId,
								blockAddr
				);
		printf( "   * Number Of Spec    : %3d", fbCodeInfo.inputNo);
		printf( "   * Size Of Spec      : %3d\n", fbCodeInfo.inputSize);

		printf( "   * Number Of Var     : %3d", fbCodeInfo.intlVarNo);
		printf( "   * Size Of Var       : %3d\n", fbCodeInfo.intlVarSize);

		printf( "   * Number Of Output  : %3d", fbCodeInfo.outputNo);
		printf( "   * Size Of Output    : %3d\n", fbCodeInfo.outputSize);

	    printf( "\n*********************************************************************\n");

        specTypePtr = saveSpecTypePtr;
        specSizePtr = saveSpecSizePtr;

		readDataInfo.moduleId = moduleId;
		readDataInfo.blockAddr = blockAddr;
		readDataInfo.fbId = fbMemInfo.fbId;
		readDataInfo.index = 0;

		
		if(moduleId == LOGIC_ID){
			if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN){
				status = fbCodeSpecDataReadAll(&readDataInfo, (uint32 *)specData, LOCAL_MEM_ACC);
			}
			else{
				status = fbCodeSpecDataReadAll(&readDataInfo, (uint32 *)specData, BUS_MEM_ACC);
			}
		}
		
		for(i = 1; i <= fbCodeInfo.inputNo; i++, specSizePtr++,specTypePtr++) {
			typeIndex = GET_TYPE_INDEX(*specTypePtr) + GET_ANY_TYPE_INDEX(*specTypePtr);
			paraType = GET_DATA_TYPE(*specTypePtr);

            if(CHK_VALID_TYPE_INDEX(typeIndex)==0) {typeIndex = 0;}

			if(moduleId==LOGIC_ID) {
				readDataInfo.index = i;
				if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN){
					status = fbCodeSpecActionRead(&readDataInfo,
											(strFbSpecActionInfo *)&specActionData, LOCAL_MEM_ACC);
				}
				else{
					status = fbCodeSpecActionRead(&readDataInfo,
											(strFbSpecActionInfo *)&specActionData, BUS_MEM_ACC);
				}

				if( CHK_IS_BIT_TYPE(*specTypePtr) && (specActionData.negated ==0x1) ) {
					
					printf("[S%03d %s] (N)", i, typeStr[typeIndex]);
				}
				else printf("[S%03d %s] ", i, typeStr[typeIndex]);	
			} 
			else {
				printf("[S%03d %s] ", i, typeStr[typeIndex]);
			}


			readDataInfo.index = i;
			if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN){
				status = fbCodeSpecDataReadWithType(&readDataInfo,
														(uint32 *)&data, LOCAL_MEM_ACC);
			}
			else{
				status = fbCodeSpecDataReadWithType(&readDataInfo,
													   (uint32 *)&data, BUS_MEM_ACC);
			}

			if(status) {
				printf(" ERR");
				goto END_OF_ONE_SPEC_READ;
			}

			
			if(moduleId==LOGIC_ID){
				if( (specActionData.negated ==0x1)&& CHK_IS_BIT_TYPE(*specTypePtr) ) {
					specDataNegated(&specActionData, &data.halfUlint_t[0]);
					specDataNegated(&specActionData, &data.halfUlint_t[1]);
				}
			}

			
			switch(CHK_IS_ANY_TYPE(*specTypePtr) ? GET_BASIC_DATA_TYPE(fbMemInfo.paraRunType) : paraType) 
			{
				case BOOL_TYPE:
					printf(" %8x ",data.Uint_t);	
					break;
				case UINT_TYPE:
				case WORD_TYPE:
					printf(" 0x%08x ",data.Uint_t);	
					break;
#ifdef USED_BIT64
				case ULINT_TYPE:
				case LWORD_TYPE:
#ifdef _WIN32
					printf(" 0x%8x%08x ",data.halfUlint_t[1],data.halfUlint_t[0]);	
#else
					printf(" 0x%8x%08x ",data.halfUlint_t[0],data.halfUlint_t[1]);
#endif 
					break;
#endif
				case REAL_TYPE:
					printf(" %6.5f ",data.Float_t);
			   		break;
				case LREAL_TYPE:
					printf(" %6.7f ",data.LReal_t);
			   		break;
				case INT_TYPE:
					printf(" %8d ",data.Int_t);	
					break;
#ifdef USED_BIT64
				case LINT_TYPE:
					printf(" %20d ",(uint64)data.LInt_t);
			   		break;
#endif
				default:
					if( GET_TYPE_SIZE(*specTypePtr) == SIZE32_TYPE ) {
						printf(" %9d ",data.Int_t);	
					}
#ifdef USED_BIT64
					else if( GET_TYPE_SIZE(*specTypePtr) == SIZE64_TYPE ) {
						printf(" %9d ",(uint64)data.LInt_t);	
					}
#endif
					else {
						printf(" TERR");
					}
					break;
			}
			
			if((specActionData.refType == SPEC_TYPE_BYREF) && (moduleId == LOGIC_ID))
			{
				if(specActionData.memoryType==F_MEMORY)
				{
					printf("(%%%s%s%d.%d)  ",
						memTypeStr[specActionData.memoryType],automemSizeStr[specActionData.dataType],
						specData[*specSizePtr]>>8, (specData[*specSizePtr]& 0xf)>>2);

				}
				else if(specActionData.memoryType==A_MEMORY)
				{
					printf("(%%%s%s%d)  ",
						memTypeStr[specActionData.memoryType],automemSizeStr[specActionData.dataType],
						specData[*specSizePtr]>>8);

				}
				else if(specActionData.memoryType==M_MEMORY)
				{
					if(specActionData.bitposFlag ==1) 
					{
						printf("(%%%s%s%d.%d)  ",
							memTypeStr[specActionData.memoryType],memSizeStr[specActionData.dataSize],
							specData[*specSizePtr]>>8, specActionData.bitPosition);
					}
					else
					{
 						printf("(%%%s%s%d)    ",
								memTypeStr[specActionData.memoryType],memSizeStr[specActionData.dataSize],
								specData[*specSizePtr]>>8);
					}
				}

				else 
				{
					if(specActionData.bitposFlag ==1) 
					{
						printf("(%%%s%s%d.%d.%d) ",
							memTypeStr[specActionData.memoryType],memSizeStr[specActionData.dataSize],
							specData[*specSizePtr]>>8, (specData[*specSizePtr]>>2)&0x3f ,
							specActionData.bitPosition);
					}
					else
					{
 						printf("(%%%s%s%d.%d.%d) ",
							memTypeStr[specActionData.memoryType],memSizeStr[specActionData.dataSize],
							specData[*specSizePtr]>>8, (specData[*specSizePtr]>>2)&0x3f ,
							specActionData.bitPosition);
					}
				}

			}
			else {
				if(moduleId==LOGIC_ID) {
					printf("\t\t");
				}
				else {
					printf("\t");
				}
			}
			
END_OF_ONE_SPEC_READ:

            
			if(!(i%factor)) printf("\n");
        }	

		if( ((fbCodeInfo.inputNo > factor) && (fbCodeInfo.inputNo % factor)) ||
        	 (fbCodeInfo.inputNo < factor)
		  )
		{
           	printf( "\n=====================================================================\n");
		} else {
	       	printf(   "=====================================================================\n");
		}

		
			
		
        varTypePtr = saveVarTypePtr;

		for(i = 1; i <= fbCodeInfo.intlVarNo; i++, varTypePtr++) {
			typeIndex = GET_TYPE_INDEX(*varTypePtr) + GET_ANY_TYPE_INDEX(*varTypePtr);
			paraType = GET_DATA_TYPE(*varTypePtr);

            if(CHK_VALID_TYPE_INDEX(typeIndex)==0) {typeIndex = 0;}
			printf("[V%03d %s] ", i, typeStr[typeIndex]);

			readDataInfo.index = i;
			if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN){
				status = fbCodeVarDataReadWithType(&readDataInfo,
														(uint32 *)&data, LOCAL_MEM_ACC);
			}
			else{
				status = fbCodeVarDataReadWithType(&readDataInfo,
													   (uint32 *)&data, BUS_MEM_ACC);
			}

			if(status) {
				printf(" ERR");
				goto END_OF_ONE_VAR_READ;
			}

			switch(CHK_IS_ANY_TYPE(*varTypePtr) ? fbMemInfo.paraRunType : paraType) 
			{
				case BOOL_TYPE:
					printf(" %8x ",data.Uint_t);	
					break;
				case UINT_TYPE:
				case WORD_TYPE:
					printf(" 0x%08x",data.Uint_t);	
					break;
#ifdef USED_BIT64
				case ULINT_TYPE:
				case LWORD_TYPE:
#ifdef _WIN32
					printf(" %8x%8x ",data.halfUlint_t[1],data.halfUlint_t[0]);	
#else
					printf(" %8x%8x ",data.halfUlint_t[0],data.halfUlint_t[1]);	
#endif
					break;
#endif
				case REAL_TYPE:
					printf(" %6.5f ",data.Float_t);
			   		break;
				case LREAL_TYPE:
					printf(" %6.13f ",data.LReal_t);
			   		break;
				case INT_TYPE:
					printf(" %8d ",data.Int_t);	
					break;
#ifdef USED_BIT64
				case LINT_TYPE:
					printf(" %20d ",(uint64)data.LInt_t);
			   		break;
#endif
            	default:
					
					if( GET_TYPE_SIZE(*specTypePtr) == SIZE32_TYPE ) 
						printf(" %9d ",data.Int_t);	
#ifdef USED_BIT64
					else if( GET_TYPE_SIZE(*specTypePtr) == SIZE64_TYPE ) 
						printf(" %9d ",(uint64)data.LInt_t);	
#endif
                	else 	printf(" TERR");
					break;
			}
			if(moduleId==LOGIC_ID) {
				printf("\t\t");
			}
			else {
				printf("\t");
			}
			
END_OF_ONE_VAR_READ:

            if( !(i % factor))
            	printf("\n");
        }

		if(((fbCodeInfo.intlVarNo > factor) && (fbCodeInfo.intlVarNo % factor)) ||
        	(fbCodeInfo.intlVarNo < factor))
           	printf( "\n=====================================================================\n");
		else
	       	printf( "======================================================================\n");

		
			
		
        outputTypePtr = saveOutputTypePtr;

		for(i = 1; i <= fbCodeInfo.outputNo; i++, outputTypePtr++) {
			typeIndex = GET_TYPE_INDEX(*outputTypePtr) + GET_ANY_TYPE_INDEX(*outputTypePtr);
			paraType = GET_DATA_TYPE(*outputTypePtr);

            if(CHK_VALID_TYPE_INDEX(typeIndex)==0) {typeIndex = 0;}
			
			outputNegated = (fbMemInfo.outputAction[(i-1)/32] >> ((i-1)%32))&0x1;

			if( CHK_IS_BIT_TYPE(*outputTypePtr) && ( outputNegated ==0x1 ) 
			) {
					printf("[N%03d %s] (N)", i, typeStr[typeIndex]);
			}
			else printf("[N%03d %s] ", i, typeStr[typeIndex]);


			readDataInfo.index = i;
			if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN){
				status = fbCodeOutputDataReadWithType(&readDataInfo,
														(uint32 *)&data, LOCAL_MEM_ACC);
			}
			else{
				status = fbCodeOutputDataReadWithType(&readDataInfo,
													   (uint32 *)&data, BUS_MEM_ACC);
			}

	
			if(status) {
				printf(" ERR");
				goto END_OF_ONE_OUTPUT_READ;
			}
			switch(CHK_IS_ANY_TYPE(*outputTypePtr) ? GET_BASIC_DATA_TYPE(fbMemInfo.paraRunType) : paraType) 
			{
				case BOOL_TYPE:
					printf(" %8x ",data.Uint_t);	
					break;
				case UINT_TYPE:
				case WORD_TYPE:
					printf(" 0x%08x ",data.Uint_t);	
					break;
#ifdef USED_BIT64
				case ULINT_TYPE:
				case LWORD_TYPE:
#ifdef _WIN32
					printf(" %8x%8x ",data.halfUlint_t[1],data.halfUlint_t[0]);	
#else
					printf(" %8x%8x ",data.halfUlint_t[0],data.halfUlint_t[1]);	
#endif
					break;
#endif
				case REAL_TYPE:
					printf(" %6.5f",data.Float_t);
			   		break;
				case LREAL_TYPE:
					printf(" %6.13f ",data.LReal_t);
			   		break;
				case INT_TYPE:
					printf(" %8d ",data.Int_t);	
					break;
#ifdef USED_BIT64
				case LINT_TYPE:
					printf(" %20d ",(uint64)data.LInt_t);
			   		break;
#endif
				default:
					if( GET_TYPE_SIZE(*specTypePtr) == SIZE32_TYPE ) 
						printf(" %8d ",data.Int_t);	
#ifdef USED_BIT64
					else if( GET_TYPE_SIZE(*specTypePtr) == SIZE64_TYPE ) 
						printf(" %8d ",(uint64)data.LInt_t);	
#endif
                	else 
                	printf(" TERR");
					break;
			}
			if(moduleId==LOGIC_ID) {
				printf("\t\t");
			}
			else {
				printf("\t");
			}

END_OF_ONE_OUTPUT_READ:

            if( !(i % factor))
            	printf("\n");
        }

		if(((fbCodeInfo.outputNo > factor) && (fbCodeInfo.outputNo % factor)) ||
        	(fbCodeInfo.outputNo < factor))
           	printf( "\n=====================================================================\n");
		else
	       	printf( "=====================================================================\n");
        
		sysTaskDelay(systemClockRateGet()/2);
    }
    
#ifdef VXWORKS
    (void)ioSysCtrlFunc (gDebugPortFd, MYIOFLUSH, 0);
#endif
}


static void fbCodeInMemDisplay(uint32 moduleId, uint32 arg1)
{
    strFbMemInfo	fbMemInfo;
    strModuleInfo	moduleInfo;
	uint32	status,  index;
	int8	line[16], *cPtr;
	int32  startBlockAddr, endBlockAddr, i;
	uint32	maxBlockAddr;

    status = moduleInfoRead(moduleId, &moduleInfo);
	if(status != NO_ERROR) {
		printf( "[ERROR] - moduleInfoRead() Call Err !!\n");
        return;
    }

    maxBlockAddr = moduleInfo.maxFbBlockAddr;

    printf("\tStart Fb Block Addr[1 - %d] : ",maxBlockAddr);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    startBlockAddr = atoi((const char *)cPtr);

    printf("\tEnd Fb Block Addr[%d - %d] : ",startBlockAddr+1,maxBlockAddr);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    endBlockAddr = atoi((const char *)cPtr);

    if( ((endBlockAddr - startBlockAddr) <= 0) ||
    	(startBlockAddr >= (int32)maxBlockAddr) ||
        (endBlockAddr > (int32)maxBlockAddr)
	) {
    	printf( "\n");
		printf( "\t[ERROR] - Invalid Block Addr (Start : %d,End : %d)\n",
        				startBlockAddr, endBlockAddr);
		return;
    }

    printf("\n");

    index = 1;
    for(i = startBlockAddr; i <= endBlockAddr; i++) {
    	memoryClear( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
    	status = fbCodeMemInfoRead(	moduleId,
        					    	i,
                                    &fbMemInfo
								  );

		if(status != NO_ERROR) {
			printf("[ERROR] - fbCodeMemInfoRead() Call Err !!\n");
            return;
        }

        printf(" [%05d]", i);
      
        if(fbMemInfo.fbId != 0)
	        printf("%04d", fbMemInfo.fbId);
  		else
        	printf("    ");

        if( !(index % 6)) printf( "\n");
        index++;
	}
}


void fbParaInMemDisplay(uint32 moduleId, uint32 arg1)
{
    strModuleInfo	moduleInfo;
	uint32	status,  index;
	int8	line[16], *cPtr;
	uint32  startParaIndex, endParaIndex, i;
	uint32	maxParaIndex;
	strMemPointerInfo	memInfoPtr;
	uint32  *paraData;

    status = moduleInfoRead(moduleId, &moduleInfo);
	if(status != NO_ERROR) {
		printf( "[ERROR] - moduleInfoRead() Call Err !!\n");
        return;
    }

    maxParaIndex = (moduleInfo.maxFbParaNum -1);

    printf("\tStart Fb Input Index[0 - %d] : ",maxParaIndex);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

	if(*cPtr==ENDOFSTR) startParaIndex = 0;
    else startParaIndex = atoi((const char *)cPtr);

    printf("\tEnd Fb Input Index [%d - %d]  : ",startParaIndex+1,maxParaIndex);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

	if(*cPtr==ENDOFSTR) endParaIndex = maxParaIndex;
    else endParaIndex = atoi((const char *)cPtr);

	if( ((endParaIndex - startParaIndex) <= 0) ||
    	(startParaIndex >= (int32)maxParaIndex) ||
        (endParaIndex > (int32)maxParaIndex)
	) {
    	printf( "\n");
		printf( "\t[ERROR] - Invalid  Para Index (Start : %d,End : %d)\n",
        				startParaIndex, endParaIndex);
		return;
    }

   	memoryClear( (uint8 *) &memInfoPtr, sizeof(strMemPointerInfo));
	status = fbMemPointerGet(moduleId,  &memInfoPtr);
	if(status != NO_ERROR) {
		printf("[ERROR] - fbMemPointerGet() Call Err !!\n");
        return;
    }

    index = 1;
    for(i = startParaIndex; i <= endParaIndex; i++) {

		paraData = (uint32*)(memInfoPtr.paraMemPtr) + i ;
			
        printf(" [%06d]", i);
        printf("0x%08x",*paraData);
      
        if( !(index % 4)) printf( "\n");
        index++;
	}
	printf( "\n\n");

}


static void fbCodeInTaskDisplay(uint32 moduleId, uint32 arg1)
{
    strFbMemInfo	fbMemInfo;
	uint32	status,  index, taskId;
	int8	line[16], *cPtr;
	int32  startBlockAddr, endBlockAddr, i;
	uint32	maxBlockAddr;
	strModuleExecInfo sysExeInfo;

	status = fbSysExeModuleInfoRead(LOGIC_ID, &sysExeInfo);
	if(status != NO_ERROR) {
		printf("[ERROR] - fbSysExeModuleInfoRead() Call Err !!\n");
	}
    maxBlockAddr = sysExeInfo.curfbMaxBlockAddr;

	printf("\n\tEnter Segment Id ? ");
	ioRead(gDebugPortFd,line, sizeof(line));
	cPtr = line;
	spaceSkip( &cPtr);

	taskId = atoi((const char *)cPtr);

    printf("\tStart Fb Block Addr[1 - %d(Cur MAX)] : ",maxBlockAddr);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    startBlockAddr = atoi((const char *)cPtr);

    printf("\tEnd Fb Block Addr[%d - %d(Cur MAX)] : ",startBlockAddr+1,maxBlockAddr);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);
    printf( "\n");

    endBlockAddr = atoi((const char *)cPtr);

    if( ((endBlockAddr - startBlockAddr) <= 0) ||
    	(startBlockAddr >= (int32)maxBlockAddr) ||
        (endBlockAddr > (int32)maxBlockAddr)
	) {
    	printf( "\n");
		printf( "\t[ERROR] - Invalid Block Addr (Start : %d,End : %d)\n",
        				startBlockAddr, endBlockAddr);
		return;
    }

    index = 1;
    for(i = startBlockAddr; i <= endBlockAddr; i++) {
    	memoryClear( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
    	status = fbCodeMemInfoRead(	moduleId,
        					    	i,
                                    &fbMemInfo
								  );

		if(status != NO_ERROR) {
			printf("[ERROR] - fbCodeMemInfoRead() Call Err !!\n");
            return;
        }

		
		if(fbMemInfo.fbTaskId != taskId) continue;

        printf(" [%05d]", i);
      
        if(fbMemInfo.fbId != 0)
	        printf("%04d", fbMemInfo.fbId);
  		else
        	printf("    ");

        if( !(index % 6)) printf( "\n");
        index++;
	}
   	printf( "\n");
}

void fbCodeInBusMemDisplay(uint32 moduleId, uint32 arg1)
{
    strFbMemInfo	fbMemInfo;
    strModuleInfo	moduleInfo;
	uint32	status, i, index;
	int8	line[16], *cPtr;
	int32 startBlockAddr, endBlockAddr;
	uint32	maxBlockAddr;

    status = moduleInfoRead(moduleId, &moduleInfo);
    
	if(status != NO_ERROR) 
	{
		printf( "[ERROR] - moduleInfoRead() Call Err !!\n");
        return;
    }

    maxBlockAddr = moduleInfo.maxFbBlockAddr;

    printf("\tStart Fb Virtual Addr(1~Max) : ");
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    startBlockAddr = atoi(cPtr);

    printf("\tEnd Fb Virtual Addr(1~Max)   : ");
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    endBlockAddr = atoi(cPtr);

    if( ((endBlockAddr - startBlockAddr) < 1) ||
    	(startBlockAddr >= maxBlockAddr) ||
        (endBlockAddr > maxBlockAddr))
	{
    	printf( "\n");
		printf( "\t[ERROR] - Invalid Block Addr (Start : %d,End : %d)\n",
        				startBlockAddr, endBlockAddr);
		return;
    }

    index = 1;
    for(i = startBlockAddr; i <= endBlockAddr; i++) 
    {
    	memoryClear( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
    	status = busFbCodeMemInfoRead(	moduleId,
        					    		i,
                                    	&fbMemInfo
								  	);

		if(status != NO_ERROR) 
		{
			printf("[ERROR] - vmeFbCodeMemInfoRead() Call Err !!\n");
            return;
        }
        
        printf(" [%05d]", i);

        if(fbMemInfo.fbId != 0)
	        printf("%03d", fbMemInfo.fbId);
		else
        	printf("   ");

        if( !(index % 7)) 
        	printf( "\n");
        	
        index++;
	}
	
	return;
}

static void fbTaskInfoDisplay(uint32 moduleId, uint32 arg1)
{
	uint32 status = 0, strIndex = 0;
    strFbTaskTblMemInfo	segInfo;
	int8 *moduleStr[] =
    {
		(int8 *)"NULL", (int8 *)"MPU", (int8 *)"DIU", (int8 *)"TMTC", (int8 *)"FIELDBUS"
    };
    int8 *checksumStr[] =
    {
    	(int8 *)"OK", (int8 *)"ERROR"
    };
	int8 *segModeStr[] =
    {
		(int8 *)"NO_MODE(0)",(int8 *)"INIT(1)",(int8 *)"DWN_LD(2)",
		(int8 *)"DWN_DNE(3)",(int8 *)"STOP(4)",(int8 *)"RUN(5)",(int8 *)"DEBUG(6)",
		(int8 *)"ERROR(7)", (int8 *)"INVAL"
    };
    int8 *logicTypestr[] =
    {
    	(int8 *)"FBD", (int8 *)"SFC", (int8 *)"INVAL"
    };
	int8 *strLoopStatus[] =
	{
		"NO_ERROR","FB_LOOPING_ERR"
	};

	int8	line[16], *pLine;
	
	uint32 fbTaskId;

	printf("\n");
	printf("\tEnter FB Task Id ? ");
	
	ioRead(gDebugPortFd,line, sizeof(line));
	pLine = line;
	spaceSkip( &pLine);
	
	fbTaskId = atoi((const char *)pLine);
	
	switch(moduleId) {
		case LOGIC_ID:
        	if( (fbTaskId < 1) || (fbTaskId > MAX_MPU_SCHD_SEG_ID))
            	status = 1;
			strIndex = 1;
        break;
        default:
        	status = 1;
        break;
    }

    if(status != NO_ERROR)  {
		printf( "\tInvalid Task Id[%d] !!!\n", fbTaskId);
        return;
    }
	
	printf( "\n********** << %s TASK Info >>***********\n",moduleStr[strIndex]);

	memoryClear( (uint8 *) &segInfo, sizeof(strFbTaskTblMemInfo));

	status = getFbTaskTblInfo ( moduleId, fbTaskId, &segInfo);
	if(status != NO_ERROR) {
		printf( "[ERROR] - Task Table Info Read Err !!\n");
        return;
    }

	printf(" * Task Id                  : %d\n", segInfo.taskId );

    if( (segInfo.taskMode >= PCS_INIT) &&
        (segInfo.taskMode <= PCS_ERR)
	)
      	strIndex = segInfo.taskMode;
	else if(segInfo.taskMode == 0)
        strIndex = 0;
	else
       	strIndex = 8;
	printf(" * Task Mode                : %s\n",segModeStr[strIndex]);
	printf(" * Task CheckSum            : 0x%x\n", segInfo.logicCheckSum);

    if( segInfo.logicCheckSumStatus == NO_ERROR)
    	strIndex = 0;
	else
    	strIndex = 1;
    	
	printf(" * Task Checksum Status     : %s\n", checksumStr[strIndex]);

	printf(" * Memory Recv Size         : %d\n", segInfo.recvMemSize);
	printf(" * Memory Used Input Size   : %d\n", segInfo.memInputSize);
	printf(" * Memory Used Output Size  : %d\n", segInfo.memOutputSize);
	printf(" * Used FB Code Number      : %d\n", segInfo.fbUsedNum);
    
	if(moduleId != LOGIC_ID) 
	{
		printf(" * FB Start Blk Addr        : %d\n", segInfo.u1.fbStartBlockAddr);
		printf(" * FB End Blk Addr          : %d\n", segInfo.u2.fbEndBlockAddr);
	}

	printf(" * TaskTickCounter          : %d\n", segInfo.taskTickCnt);
	printf(" * TaskCycleElapsedTime     : %d\n", segInfo.taskElpTime);
	printf(" * TaskTimeOverRun          : %d\n", segInfo.taskOvertimeRunCnt);
	printf(" * TaskCycleTime            : %d\n", segInfo.taskCycleTime);
	printf(" * execMode(Cyclic MODE)    : %d\n", segInfo.execMode);
	printf(" * LOGIC Type               : %s\n", logicTypestr[segInfo.logicType]);
	printf(" * Task Loop Status         : %s\n", strLoopStatus[segInfo.loopStatus]);
}

static void fbCodeDefinedInfoDisplay(uint32 arg1, uint32 arg2)
{
	strFbExecDbInfo	*fbInfoPtr = &gFbCodeSortingTableInfo[0];
	strFbCodeTypePointerInfo typeInfo;
	uint32 status, *typePtr, i;
	uint32 strIndex, type, size;
    int8 line[16], *pLine;
	uint32 fbCodeId;
	
	uint8 *typeStr[] = {
		(uint8 *)"NONE",
		(uint8 *)"INT", (uint8 *)"UINT",(uint8 *)"LINT",(uint8 *)"ULINT",(uint8 *)"REAL",
		(uint8 *)"LREAL",(uint8 *)"TIME",(uint8 *)"DATE",(uint8 *)"TIME_OF_DATE",(uint8 *)"DATE_AND_TIME",
		(uint8 *)"BOOL", (uint8 *)"WORD", (uint8 *)"LWORD",(uint8 *)"ANY",(uint8 *)"ANY_BIT",
		(uint8 *)"ANY_NUM",(uint8 *)"ANY_INT",(uint8 *)"ANY_REAL",
	};

	int8 *inputStr[] =
    {
		(int8 *)"NONE", (int8 *)"SIZE32", (int8 *)"SIZE64"
    };


	printf("\n");
	printf("\tEnter Fb Code Number ? ");
	ioRead(gDebugPortFd,line, sizeof(line));
	pLine = line;
	spaceSkip( &pLine);

	fbCodeId = atoi((const char *)pLine);

	if( (fbCodeId < 1) || (fbCodeId > MAX_FB_CODE_ID)) {
		printf( "\t[ERROR] - Fb Code Range(1 ~ %d) Error !!\n\n",
        				(uint32) MAX_FB_CODE_ID);
		return;
    }

    fbInfoPtr += fbCodeId;

    if(fbInfoPtr->fbId == 0) {
    	printf( "\t Not Define !!\n\n");
        return;
	}

    printf( "\n");
	printf("\t*********** << %s[%03d] >> *************\n",
    					fbInfoPtr->fbNamePtr, fbInfoPtr->fbId);
	printf("\t\t Input Number/Size : %3d/%d\n",fbInfoPtr->inputNo,fbInfoPtr->inputSize);
	printf("\t\t   Var Number/Size : %3d/%d\n",fbInfoPtr->intlVarNo,fbInfoPtr->intlVarSize);
	printf("\t\tOutput Number/Size : %3d/%d\n",fbInfoPtr->outputNo,fbInfoPtr->outputSize);
	
	printf( "\t Hit <RET> To Continue !!\n");
    ioRead(gDebugPortFd,line, sizeof(line));
	printf( "\n");

	status = fbCodeParaTypePointerGet (fbCodeId, &typeInfo);
				
	if(status != NO_ERROR) {
		printf("\t[ERROR] - Type Get Error !!\n");
        return;
    }
	printf( "\n");
	printf( "******************** << Input Info Display >> ********************\n");
    typePtr = (uint32 *)typeInfo.specTypeStartAddr;
	
    for(i = 1; i <= fbInfoPtr->inputNo; i++, typePtr++){
		type = GET_TYPE_INDEX(*typePtr)+ GET_ANY_TYPE_INDEX(*typePtr);
		size = GET_TYPE_SIZE(*typePtr);

        if(CHK_VALID_TYPE_INDEX(type)==0) {strIndex = 0;}
		else {strIndex = type;}

   
    	printf("    *INPUT[%03d] : %5s|",i, typeStr[strIndex]);

	    if( (size < SIZE32_TYPE) || (size > SIZE64_TYPE))
        	strIndex = 0;
		else {
        	if( size == SIZE32_TYPE)           	strIndex = 1;
            else if( (size == SIZE64_TYPE) )	strIndex = 2;
			else				            	strIndex = 0;
		}

		printf("%4s", inputStr[strIndex]);

        if((i != 0) && !(i % 2)) printf( "\n");
        if((i == 0) || (i % 60)) continue;
  
		printf( "\n");
		printf( "\t Hit <RET> To Continue !!\n");
        ioRead(gDebugPortFd,line, sizeof(line));
		printf( "\n");
	}

    printf( "\n");
	printf( "\t Hit <RET> To Continue !!\n");
    ioRead(gDebugPortFd,line, sizeof(line));
	printf( "\n");

	printf( "******************** << Var Info Display >> ********************\n");
	if(fbInfoPtr->intlVarNo == 0)
    	printf( "\t Variable Is NONE !!\n");

    typePtr = (uint32 *)typeInfo.varTypeStartAddr;
    for(i = 1; i <= fbInfoPtr->intlVarNo; i++, typePtr++){
		type = GET_TYPE_INDEX(*typePtr)+ GET_ANY_TYPE_INDEX(*typePtr);
		size = GET_TYPE_SIZE(*typePtr);

        if(CHK_VALID_TYPE_INDEX(type)==0) {strIndex = 0;}
		else {strIndex = type;}
   
    	printf("     *VAR[%03d] : %5s|",i, typeStr[strIndex]);

	    if( (size < SIZE32_TYPE) || (size > SIZE64_TYPE))
        	strIndex = 0;
		else {
        	if( size == SIZE32_TYPE)           	strIndex = 1;
            else if( (size == SIZE64_TYPE) )	strIndex = 2;
			else				            	strIndex = 0;
		}
		
        if( (i != 0) && !(i % 2)) printf( "\n");
        if((i == 0) || (i % 60)) continue;
		printf( "\n");  
		printf( "\t Hit <RET> To Continue !!\n");
        ioRead(gDebugPortFd,line, sizeof(line));
		printf( "\n");
	}

    printf( "\n");
	printf( "\t Hit <RET> To Continue !!\n");
    ioRead(gDebugPortFd,line, sizeof(line));
    printf( "\n");
	printf( "******************** << Output Info Display >> ********************\n");

    typePtr = (uint32 *)typeInfo.outputTypeStartAddr;
    for(i = 1; i <= fbInfoPtr->outputNo; i++, typePtr++){
		type = GET_TYPE_INDEX(*typePtr)+ GET_ANY_TYPE_INDEX(*typePtr);
		size = GET_TYPE_SIZE(*typePtr);

        if(CHK_VALID_TYPE_INDEX(type)==0) {strIndex = 0;}
		else {strIndex = type;}
   
    	printf("    *OUTPUT[%03d] : %5s|",i, typeStr[strIndex]);

	    if( (size < SIZE32_TYPE) || (size > SIZE64_TYPE))
        	strIndex = 0;
		else {
        	if( size == SIZE32_TYPE)           	strIndex = 1;
            else if( (size == SIZE64_TYPE) )	strIndex = 2;
			else				            	strIndex = 0;
		}
		printf("%4s", inputStr[strIndex]);
        if( (i != 0) && !(i % 2)) printf( "\n");
        if((i == 0) || (i % 60)) continue;
		
		printf( "\n");  
		printf( "\t Hit <RET> To Continue !!\n");
        ioRead(gDebugPortFd,line, sizeof(line));
        printf( "\n");
	}
}


static void simulMemInfoDisplay(uint32 moduleId, uint32 arg1)
{
	strFbMemInfo	fbMemInfo;
    strModuleInfo	moduleInfo;
	uint32	status, index;
	int8	line[16], *cPtr;
	int32 i, startBlockAddr, endBlockAddr;
	uint32	maxBlockAddr;
	strSysRunningConfigInfo		*sysStatusInfoPtr = gSysRunningInfoPtr;

    status = moduleInfoRead(moduleId, &moduleInfo);
	if(status != NO_ERROR) {
		printf( "[ERROR] - moduleInfoRead() Call Err !!\n");
        return;
    }

    maxBlockAddr = moduleInfo.maxFbBlockAddr;

	printf("\tStart Fb Block Addr[1 - %d] : ",maxBlockAddr);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    startBlockAddr = atoi((const char *)cPtr);

	printf("\tEnd Fb Block Addr[%d - %d] : ",startBlockAddr+1,maxBlockAddr);  
	ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    endBlockAddr = atoi((const char *)cPtr);

    if( ((endBlockAddr - startBlockAddr) <= 0) ||
    	(startBlockAddr >= (int32)maxBlockAddr) ||
        (endBlockAddr > (int32)maxBlockAddr)
	) {
    	printf( "\n");
		printf( "\t[ERROR] - Invalid Block Addr (Start : %d,End : %d)\n",
        				startBlockAddr, endBlockAddr);
		return;
    }

    printf("\n");
    
    index = 1;
    
    for(i = startBlockAddr; i <= endBlockAddr; i++) {
    	memoryClear( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
		if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN)
		{
    		status = fbCodeMemInfoRead(	moduleId, i,&fbMemInfo);
		}
		else
		{
    		status = busFbCodeMemInfoRead(	moduleId,i,&fbMemInfo);
		}

		if(status != NO_ERROR) 
		{
			printf("[ERROR] - fbCodeMemInfoRead() Call Err !!\n");
            return;
        }

        printf("[%05d]", i);
      
		if(fbMemInfo.fbId != 0) 
		{
	       	printf("%04d:%02d ", fbMemInfo.fbId, fbMemInfo.forcingFlag);   
		}
		
		else printf("        ");
        	
        if( !(index % 5)) printf( "\n");
        index++;     
       
	}
	
}	


static void simulMemInfoClearAll(uint32 moduleId, uint32 arg1)
{
	strFbMemInfo	fbMemInfo;
    strModuleInfo	moduleInfo;
	uint32	status;
	int8	line[16], *cPtr;
	int32 i, startBlockAddr, endBlockAddr;
	uint32	maxBlockAddr;
	strSysRunningConfigInfo		*sysStatusInfoPtr = gSysRunningInfoPtr;
	strFBforcingInfo	forcingInfo;
	uint32 retVal = NO_ERROR;

    status = moduleInfoRead(moduleId, &moduleInfo);
	if(status != NO_ERROR) {
		printf( "[ERROR] - moduleInfoRead() Call Err !!\n");
        return;
    }

    maxBlockAddr = moduleInfo.maxFbBlockAddr;

	printf("\tStart Fb Block Addr[1 - %d] : ",maxBlockAddr);
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    startBlockAddr = atoi((const char *)cPtr);

	printf("\tEnd Fb Block Addr[%d - %d] : ",startBlockAddr+1,maxBlockAddr);  
	ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    endBlockAddr = atoi((const char *)cPtr);

    if( ((endBlockAddr - startBlockAddr) <= 0) ||
    	(startBlockAddr >= (int32)maxBlockAddr) ||
        (endBlockAddr > (int32)maxBlockAddr)
	) {
    	printf( "\n");
		printf( "\t[ERROR] - Invalid Block Addr (Start : %d,End : %d)\n",
        				startBlockAddr, endBlockAddr);
		return;
    }

	printf("\n");
	printf("\t======================================\n");
	printf("\t* Focing Clear List\n");
	printf("\t======================================\n");

    for(i = startBlockAddr; i <= endBlockAddr; i++) 
    {
    	memoryClear( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
    	
		if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN)
		{
    		status = fbCodeMemInfoRead(	moduleId, i, &fbMemInfo);
		}
		else{
    		status = busFbCodeMemInfoRead(	moduleId, i, &fbMemInfo);
		}

		if(status != NO_ERROR) 
		{
			printf("[ERROR] - fbCodeMemInfoRead() Call Err !!\n");
            return;
        }
      
		if(fbMemInfo.fbId != 0) 
		{
			if(fbMemInfo.forcingFlag != 0)
			{
				forcingInfo.moduleId 	= moduleId;
				forcingInfo.taskId 		= fbMemInfo.fbTaskId;
				forcingInfo.FBCode 		= fbMemInfo.fbId;
				forcingInfo.FBAddr 		= i;
				forcingInfo.pointId 	= 0;
				forcingInfo.forcingFlag = FORCING_DISABLE;
				
				retVal = forcingFlagWrite(&forcingInfo);
				if(retVal != NO_ERROR)
					printf("[ERROR] - forcingFlagWrite() Call Err !!\n");
					
	            printf("\n\t Block Addr:%05d, Fb Code:%04d\n",forcingInfo.FBAddr,forcingInfo.FBCode);
			}
		}
	}
	
	return;
}

static void simulCodeInfoDisplay(uint32 moduleId, uint32 arg1)
{
	uint32	status;
	int8	line[16], *cPtr;
	int32	blockAddr;
	uint32	maxBlockAddr;
    strModuleInfo	moduleInfo;
    strFbMemInfo	fbMemInfo;

    status = moduleInfoRead(moduleId, &moduleInfo);
	if(status != NO_ERROR) {
		printf( "[ERROR] - moduleInfoRead() Call Err !!\n");
        return;
    }

    maxBlockAddr = moduleInfo.maxFbBlockAddr;

    printf("\tFb Block Addr To Display : ");
    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    blockAddr = atoi((const char *)cPtr);

    if((blockAddr < 0) || (blockAddr > (int32)maxBlockAddr)) {
		printf("\t[ERROR] - Block Addr(%d) Err\n", blockAddr);
        return;
    }

   	memoryClear( (uint8 *) &fbMemInfo, sizeof(strFbMemInfo));
   	status = fbCodeMemInfoRead(	moduleId,
       					    	blockAddr,
                                &fbMemInfo
							  );

	if(status != NO_ERROR) {
		printf("[ERROR] - fbCodeMemInfoRead() Call Err !!\n");
        return;
    }

    if(fbMemInfo.fbId == 0) {
		printf("\tFunction Code is NULL In Block Addr(%d) !!!\n",
        						blockAddr
				);
		return;
    }

    printf( "\n************** << BlockAddr[%d] >> ******************************\n",
    							 blockAddr
				);
	printf( "   * Seg ID             : %3d\n",fbMemInfo.fbTaskId );
	printf( "   * Fb Code            : %3d\n",fbMemInfo.fbId);
	printf( "   * Fb Address         : %3d\n",fbMemInfo.fbFuncAddr);
	printf( "   * Forcing Flag       : %3d\n",fbMemInfo.forcingFlag);
	printf( "   * Exe Sequence Order : %3d\n",fbMemInfo.fbExecSequenceId);

    printf( "*****************************************************************\n");
}


void variableMemoryTest(void)
{
	int8 line[16], *pLine;

    int8 *helpString[] = {
    (int8 *)"*************************************",
	(int8 *)"* 1.Variable Memory Read Display",
    (int8 *)"* 2.Variable Memory Write Display",
    (int8 *)"* q.Exit",
    (int8 *)"*************************************",
    NULL
    };
    int8 **pStr;

	for(;;){
    	printf( "\n");

		for(pStr = helpString; *pStr != NULL; pStr++)
        	printf( "\t%s\n", *pStr);

        printf("\n\tChoice ? ");

        ioRead(gDebugPortFd, line, sizeof(line));
		pLine = line;
		spaceSkip(&pLine);

		switch( *pLine) {
			case '1':
            	pLine++;
                if( *pLine != ENDOFSTR) {
	            	printf("\n\t[Error]-Unknown Number !!!\n");
                }
				else {
    	        	variableMemoryReadDisplay();
				}
            break;
            case '2':
            	pLine++;
                if( *pLine != ENDOFSTR) {
	            	printf("\n\t[Error]-Unknown Number !!!\n");
                }
                else {
             		variableMemoryWriteDisplay();
				} 
            break;

            case 'q':
            	goto MODULE_END;
            break;
            default:
            	printf("\n\t[Error]-Unknown Number !!!\n");
			break;
		}		
    }
MODULE_END:
	return;
}


static void variableMemoryReadDisplay(void)
{
	uint32	status, i;
	int8	line[16], *pLine,*pTempChar;
	uint32  variableRegion, variableSize,bitPosition=0, bitposFlag=0;
	uint32	memAddress =0, memIndex=0;
	uint32 specData=0;
	strFbSpecActionInfo specActionData;
	uint32	specType;
	uint32 retSpecDataPtr[2];
	uint8  retainInfo = 0;
	
	memoryClear((uint8*)&specActionData,sizeof(strFbSpecActionInfo));

	printf( "\t**************** << Variable Read >> ****************\n");
	printf("\tFLAG(M) Variable   : %%MW3.10 or %%MW3\n");
	printf("\tShared(S) Variable : %%SW11.10 or %%SW11\n");
	printf("\tAuto(A) Variable   : %%AD3 or %%AL3 (Size prefix : D, L)\n");
	printf("\tIO(I,Q) Variable   : %%IX1.1.1 or %%QW1.4.1 (Size prefix : X,B,W,D)\n");


    printf("\n\tVariable Memory prefix and Data: %%");
    ioRead(gDebugPortFd, line, sizeof(line));
	pLine = line;
    spaceSkip(&pLine);

    if( (*pLine == 'm')|| (*pLine == 'M')) {
		variableRegion = M_MEMORY;
		pLine++;
		if((*pLine == 'b')|| (*pLine == 'B'))	variableSize = SPEC_SIZE_BYTE;
		else if((*pLine == 'w')|| (*pLine == 'W'))	variableSize = SPEC_SIZE_WORD;
		else if((*pLine == 'd')|| (*pLine == 'D'))	variableSize = SPEC_SIZE_DWORD;
		else if((*pLine == 'l')|| (*pLine == 'L'))	variableSize = SPEC_SIZE_LWORD;
		else if((*pLine == 'x')|| (*pLine == 'X'))		variableSize = SPEC_SIZE_BIT;
		else {
			printf( "\tInvalid Flag Memory[%%%s] !!!\n",line);
			return;
		}
		
		i=0;pLine++;
		pTempChar = pLine;
		while( (*pLine !='.')&&(*pLine !='\0') ) {
			if (i>5) break;
			i++;pLine++;
		}
		if(i<5) memAddress = atoi((const char*)pTempChar);
		else {
			printf( "\tInvalid Flag Memory[%%%s] !!!\n",line);
			return;
		}

		if(*pLine != '\0') {
			i=0;pLine++;
			pTempChar = pLine;
			while(*pLine != '\0') {
				if ((i>5) || (*pLine =='.')) break;
				i++;pLine++;
			}
			if( (i<5)&&(*pLine !='.')){
				bitPosition = atoi((const char*)pTempChar);
				bitposFlag =1;
			}
			else{
				printf( "\tInvalid Flag Memory[%%%s] !!!\n",line);
				return;
			}
		}

	}

    else if( (*pLine == 's')|| (*pLine == 'S')) {
		variableRegion = S_MEMORY;
		pLine++;
		if((*pLine == 'b')|| (*pLine == 'B'))	variableSize = SPEC_SIZE_BYTE;
		else if((*pLine == 'w')|| (*pLine == 'W'))	variableSize = SPEC_SIZE_WORD;
		else if((*pLine == 'd')|| (*pLine == 'D'))	variableSize = SPEC_SIZE_DWORD;
		else if((*pLine == 'l')|| (*pLine == 'L'))	variableSize = SPEC_SIZE_LWORD;
		else if((*pLine == 'x')|| (*pLine == 'X'))		variableSize = SPEC_SIZE_BIT;
		else {
			printf( "\tInvalid Shared Memory[%%%s] !!!\n",line);
			return;
		}
		
		i=0;pLine++;
		pTempChar = pLine;
		while( (*pLine !='.')&&(*pLine !='\0') ) {
			if (i>5) break;
			i++;pLine++;
		}
		if(i<5) memAddress = atoi((const char*)pTempChar);
		else {
			printf( "\tInvalid Shared Memory[%%%s] !!!\n",line);
			return;
		}

		if(*pLine != '\0') {
			i=0;pLine++;
			pTempChar = pLine;
			while(*pLine != '\0') {
				if ((i>5) || (*pLine =='.')) break;
				i++;pLine++;
			}
			if( (i<5)&&(*pLine !='.')){
				bitPosition = atoi((const char*)pTempChar);
				bitposFlag =1;
			}
			else{
				printf( "\tInvalid Flag Memory[%%%s] !!!\n",line);
				return;
			}
		}
	}

	else if( (*pLine == 'a')|| (*pLine == 'A')) {
		variableRegion = A_MEMORY;
		pLine++;
		if((*pLine == 'd')|| (*pLine == 'D'))	variableSize = SPEC_SIZE_DWORD;
		else if((*pLine == 'l')|| (*pLine == 'L'))	variableSize = SPEC_SIZE_LWORD;
		else {
			printf( "\tInvalid Auto Memory[%%%s] !!!\n",line);
			return;
		}
		
		i=0;pLine++;
		pTempChar = pLine;
		while( (*pLine !='.')&&(*pLine !='\0') ) {
			if (i>5) break;
			i++;pLine++;
		}
		if ( (i<5)&&(*pLine !='.')) memAddress = atoi((const char*)pTempChar);
		else {
			printf( "\tInvalid Auto Memory[%%%s] !!!\n",line);
			return;
		}

	}
    else if( (*pLine == 'i')|| (*pLine == 'I')) {	
		variableRegion = I_MEMORY;
		pLine++;
		if((*pLine == 'b')|| (*pLine == 'B'))	variableSize = SPEC_SIZE_BYTE;
		else if((*pLine == 'w')|| (*pLine == 'W'))	variableSize = SPEC_SIZE_WORD;
		else if((*pLine == 'd')|| (*pLine == 'D'))	variableSize = SPEC_SIZE_DWORD;
		else if((*pLine == 'x')|| (*pLine == 'X')|| (*pLine =='\0')) variableSize = SPEC_SIZE_BIT;
		else {
			printf( "\tInvalid Input Memory[%%%s] !!!\n",line);
			return;
		}
		
		pLine++;i=0;
		pTempChar = pLine;
		while(*pLine !='.'&&(*pLine !='\0')) {
			if (i>2) break;
			i++;pLine++;
		}
		if(i<2) memAddress = atoi((const char*)pTempChar);
		else {
			printf( "\tInvalid Input Memory[%%%s] !!!\n",line);
			return;
		}

		pLine++;i=0;
		pTempChar = pLine;
		while( (*pLine !='.')&&(*pLine !='\0') ) {
			if (i>2) break;
			i++;pLine++;
		}
		if(i<2) memIndex = atoi((const char*)pTempChar);
		else{
			printf( "\tInvalid Input Memory[%%%s] !!!\n",line);
			return;
		}

		i=0;pLine++;
		pTempChar = pLine;
		while( (*pLine !='\0') ) {
			if (i>2) break;
			i++;pLine++;
		}
		if(i<2) {
			bitPosition = atoi((const char*)pTempChar);
			bitposFlag = 1;
		}
		else{
			printf( "\tInvalid Input Memory[%%%s] !!!\n",line);
			return;
		}


	}
   else if( (*pLine == 'q')|| (*pLine == 'Q')) {	
		variableRegion = Q_MEMORY;
		pLine++;
		if((*pLine == 'b')|| (*pLine == 'B'))	variableSize = SPEC_SIZE_BYTE;
		else if((*pLine == 'w')|| (*pLine == 'W'))	variableSize = SPEC_SIZE_WORD;
		else if((*pLine == 'd')|| (*pLine == 'D'))	variableSize = SPEC_SIZE_DWORD;
		else if((*pLine == 'x')|| (*pLine == 'X')|| (*pLine =='\0')) variableSize = SPEC_SIZE_BIT;
		else {
			printf( "\tInvalid Output Memory[%%%s] !!!\n",line);
			return;
		}
		
		pLine++;i=0;
		pTempChar = pLine;
		while(*pLine !='.'&&(*pLine !='\0')) {
			if (i>3) break;
			i++;pLine++;
		}
		if(i<3) memAddress = atoi((const char*)pTempChar);

		if((memAddress>MAX_IO_SHELF_NUM) || (i>=3)) {
			printf( "\tInvalid Output Memory(SHELF)[%%%s] !!!\n",line);
			return;
		}

		pLine++;i=0;
		pTempChar = pLine;
		while( (*pLine !='.')&&(*pLine !='\0') ) {
			if (i>3) break;
			i++;pLine++;
		}
		if(i<3)  memIndex = atoi((const char*)pTempChar);

		if((memAddress>MAX_IO_CARD_NUM) || (i>=3)) {
			printf( "\tInvalid Output Memory(CARD)[%%%s] !!!\n",line);
			return;
		}

		i=0;pLine++;
		pTempChar = pLine;
		while( (*pLine !='\0') ) {
			if (i>3) break;
			i++;pLine++;
		}
		if(i<3) {
			bitPosition = atoi((const char*)pTempChar);
			bitposFlag = 1;
		}

		if((memAddress>MAX_IO_POINT_NUM) || (i>=3)) {
			printf( "\tInvalid Output Memory(POINT)[%%%s] !!!\n",line);
			return;
		}


	}
	else{
		printf( "\tInvalid Memory Region[%%%s] !!!\n",line);
        return;
	}

	specData =	memAddress<<8;
	specData |= (memIndex &0x3f)<<2; 
	specActionData.dataSize = variableSize;
	specActionData.refType = SPEC_TYPE_BYREF;
	specActionData.memoryType = variableRegion;
	specActionData.bitPosition = bitPosition;
	specActionData.bitposFlag =bitposFlag;
	if(variableSize == SPEC_SIZE_LWORD) specType = SIZE64_TYPE;
	else specType = SIZE32_TYPE;

	retSpecDataPtr[0]=0;	retSpecDataPtr[1]=0;
	if(refDataCopyFromMem(retSpecDataPtr,specData, specActionData, specType,0)==NO_ERROR)
	{
		if (bitposFlag== 1)  printf("\tVariable Bit Data : 0x%.1x\n",retSpecDataPtr[0]);	
		else {
			if(variableSize == SPEC_SIZE_BYTE) printf("\tVariable Data : 0x%.2x\n",retSpecDataPtr[0]);
			else if(variableSize == SPEC_SIZE_WORD) printf("\tVariable Data : 0x%.4x\n ",retSpecDataPtr[0]);	
			else if(variableSize == SPEC_SIZE_DWORD) printf("\tVariable Data : 0x%.8x\n ",retSpecDataPtr[0]);	
			else if(variableSize == SPEC_SIZE_BIT) printf("\tVariable Data : 0x%.1x\n ",retSpecDataPtr[0]);	
			else if(variableSize == SPEC_SIZE_LWORD)
			{
				printf("\tVariable Data : 0x%.8x ",retSpecDataPtr[0]);	
				printf(" 0x%.8x ",retSpecDataPtr[1]);
			}
			printf("\n");	
		}
		status = varRetainInfoReadwithRef(specData, specActionData, (uint32*)&retainInfo);
		printf("\tVariable Retain Info: 0x%.2x\n",retainInfo);
	}

	else printf( "\tInvalid variable Data[%%%s] !!!\n",line);

	return;

}


static void variableMemoryWriteDisplay(void)
{
	uint32	i;
	int8	line[16], *pLine,*pTempChar;
	uint32  variableRegion, variableSize,bitPosition=0, bitposFlag=0;
	uint32	memAddress =0, memIndex=0;
	uint32 specData=0;
	strFbSpecActionInfo specActionData;
	uint32	specType,retainInfo;
	uint32 setSpecDataPtr[2];
	uint32 status = NO_ERROR;

	memoryClear((uint8*)&specActionData,sizeof(strFbSpecActionInfo));

	printf( "\t**************** << Variable Write >> ****************\n");
	printf("\tFLAG(M) Variable   : %%MW3.10 or %%MW3\n");
	printf("\tShared(S) Variable : %%SW11.10 or %%SW11\n");
	printf("\tAuto(A) Variable   : %%AD3 or %%AL3 (Size prefix : D, L)\n");
	printf("\tIO(I,Q) Variable   : %%IX1.1.1 or %%QW1.4.1 (Size prefix : X,B,W,D)\n");


    printf("\n\tVariable Memory prefix and Data: %%");
    ioRead(gDebugPortFd, line, sizeof(line));
	pLine = line;
    spaceSkip(&pLine);

    if( (*pLine == 'm')|| (*pLine == 'M')) {
		variableRegion = M_MEMORY;
		pLine++;
		if((*pLine == 'b')|| (*pLine == 'B'))	variableSize = SPEC_SIZE_BYTE;
		else if((*pLine == 'w')|| (*pLine == 'W'))	variableSize = SPEC_SIZE_WORD;
		else if((*pLine == 'd')|| (*pLine == 'D'))	variableSize = SPEC_SIZE_DWORD;
		else if((*pLine == 'l')|| (*pLine == 'L'))	variableSize = SPEC_SIZE_LWORD;
		else if((*pLine == 'x')|| (*pLine == 'X'))		variableSize = SPEC_SIZE_BIT;
		else {
			printf( "\tInvalid Flag Memory[%%%s] !!!\n",line);
			return;
		}
		
		i=0;pLine++;
		pTempChar = pLine;
		while( (*pLine !='.')&&(*pLine !='\0') ) {
			if (i>5) break;
			i++;pLine++;
		}
		if(i<5) memAddress = atoi((const char*)pTempChar);
		else {
			printf( "\tInvalid Flag Memory[%%%s] !!!\n",line);
			return;
		}

		if(*pLine != '\0') {
			i=0;pLine++;
			pTempChar = pLine;
			while(*pLine != '\0') {
				if ((i>5) || (*pLine =='.')) break;
				i++;pLine++;
			}
			if( (i<5)&&(*pLine !='.')){
				bitPosition = atoi((const char*)pTempChar);
				bitposFlag =1;
			}
			else{
				printf( "\tInvalid Flag Memory[%%%s] !!!\n",line);
				return;
			}
		}

	}

    else if( (*pLine == 's')|| (*pLine == 'S')) {
		variableRegion = S_MEMORY;
		pLine++;
		if((*pLine == 'b')|| (*pLine == 'B'))	variableSize = SPEC_SIZE_BYTE;
		else if((*pLine == 'w')|| (*pLine == 'W'))	variableSize = SPEC_SIZE_WORD;
		else if((*pLine == 'd')|| (*pLine == 'D'))	variableSize = SPEC_SIZE_DWORD;
		else if((*pLine == 'l')|| (*pLine == 'L'))	variableSize = SPEC_SIZE_LWORD;
		else if((*pLine == 'x')|| (*pLine == 'X'))		variableSize = SPEC_SIZE_BIT;
		else {
			printf( "\tInvalid Shared Memory[%%%s] !!!\n",line);
			return;
		}
		
		i=0;pLine++;
		pTempChar = pLine;
		while( (*pLine !='.')&&(*pLine !='\0') ) {
			if (i>5) break;
			i++;pLine++;
		}
		if(i<5) memAddress = atoi((const char*)pTempChar);
		else {
			printf( "\tInvalid Shared Memory[%%%s] !!!\n",line);
			return;
		}

		if(*pLine != '\0') {
			i=0;pLine++;
			pTempChar = pLine;
			while(*pLine != '\0') {
				if ((i>5) || (*pLine =='.')) break;
				i++;pLine++;
			}
			if( (i<5)&&(*pLine !='.')){
				bitPosition = atoi((const char*)pTempChar);
				bitposFlag =1;
			}
			else{
				printf( "\tInvalid Shared Memory[%%%s] !!!\n",line);
				return;
			}
		}

	}

	else if( (*pLine == 'a')|| (*pLine == 'A')) {
		variableRegion = A_MEMORY;
		pLine++;
		if((*pLine == 'd')|| (*pLine == 'D'))	variableSize = SPEC_SIZE_DWORD;
		else if((*pLine == 'l')|| (*pLine == 'L'))	variableSize = SPEC_SIZE_LWORD;
		else {
			printf( "\tInvalid Auto Memory[%%%s] !!!\n",line);
			return;
		}
		
		i=0;pLine++;
		pTempChar = pLine;
		while( (*pLine !='.')&&(*pLine !='\0') ) {
			if (i>5) break;
			i++;pLine++;
		}
		if ( (i<5)&&(*pLine !='.')) memAddress = atoi((const char*)pTempChar);
		else {
			printf( "\tInvalid Auto Memory[%%%s] !!!\n",line);
			return;
		}

	}
    else if( (*pLine == 'i')|| (*pLine == 'I')) {	
		variableRegion = I_MEMORY;
		pLine++;
		if((*pLine == 'b')|| (*pLine == 'B'))	variableSize = SPEC_SIZE_BYTE;
		else if((*pLine == 'w')|| (*pLine == 'W'))	variableSize = SPEC_SIZE_WORD;
		else if((*pLine == 'd')|| (*pLine == 'D'))	variableSize = SPEC_SIZE_DWORD;
		else if((*pLine == 'x')|| (*pLine == 'X')|| (*pLine =='\0')) variableSize = SPEC_SIZE_BIT;
		else {
			printf( "\tInvalid Input Memory[%%%s] !!!\n",line);
			return;
		}
		
		pLine++;i=0;
		pTempChar = pLine;
		while(*pLine !='.'&&(*pLine !='\0')) {
			if (i>2) break;
			i++;pLine++;
		}
		if(i<2) memAddress = atoi((const char*)pTempChar);
		else {
			printf( "\tInvalid Input Memory[%%%s] !!!\n",line);
			return;
		}

		pLine++;i=0;
		pTempChar = pLine;
		while( (*pLine !='.')&&(*pLine !='\0') ) {
			if (i>2) break;
			i++;pLine++;
		}
		if(i<2) memIndex = atoi((const char*)pTempChar);
		else{
			printf( "\tInvalid Input Memory[%%%s] !!!\n",line);
			return;
		}

		i=0;pLine++;
		pTempChar = pLine;
		while( (*pLine !='\0') ) {
			if (i>2) break;
			i++;pLine++;
		}
		if(i<2) {
			bitPosition = atoi((const char*)pTempChar);
			bitposFlag = 1;
		}
		else{
			printf( "\tInvalid Input Memory[%%%s] !!!\n",line);
			return;
		}


	}
   else if( (*pLine == 'q')|| (*pLine == 'Q')) {	
		variableRegion = Q_MEMORY;
		pLine++;
		if((*pLine == 'b')|| (*pLine == 'B'))	variableSize = SPEC_SIZE_BYTE;
		else if((*pLine == 'w')|| (*pLine == 'W'))	variableSize = SPEC_SIZE_WORD;
		else if((*pLine == 'd')|| (*pLine == 'D'))	variableSize = SPEC_SIZE_DWORD;
		else if((*pLine == 'x')|| (*pLine == 'X')|| (*pLine =='\0')) variableSize = SPEC_SIZE_BIT;
		else {
			printf( "\tInvalid Output Memory[%%%s] !!!\n",line);
			return;
		}
		
		pLine++;i=0;
		pTempChar = pLine;
		while(*pLine !='.'&&(*pLine !='\0')) {
			if (i>2) break;
			i++;pLine++;
		}
		if(i<2) memAddress = atoi((const char*)pTempChar);
		else {
			printf( "\tInvalid Output Memory[%%%s] !!!\n",line);
			return;
		}

		pLine++;i=0;
		pTempChar = pLine;
		while( (*pLine !='.')&&(*pLine !='\0') ) {
			if (i>2) break;
			i++;pLine++;
		}
		if(i<2) memIndex = atoi((const char*)pTempChar);
		else{
			printf( "\tInvalid Output Memory[%%%s] !!!\n",line);
			return;
		}

		i=0;pLine++;
		pTempChar = pLine;
		while( (*pLine !='\0') ) {
			if (i>2) break;
			i++;pLine++;
		}
		if(i<2) {
			bitPosition = atoi((const char*)pTempChar);
			bitposFlag = 1;
		}
		else{
			printf( "\tInvalid Output Memory[%%%s] !!!\n",line);
			return;
		}


	}
	else{
		printf( "\tInvalid Memory Region[%%%s] !!!\n",line);
        return;
	}

	specData =	memAddress<<8;
	specData |= (memIndex &0x3f)<<2; 
	specActionData.dataSize = variableSize;
	specActionData.refType = 0x00; 
	specActionData.memoryType = variableRegion;
	specActionData.bitPosition = bitPosition;
	specActionData.bitposFlag =bitposFlag;

	if((variableSize == SPEC_SIZE_LWORD)&&(bitposFlag != 1 )) specType = SIZE64_TYPE;
	else specType = SIZE32_TYPE;

	printf("\tVariable Data(32BIT:HEX) : 0x");
    ioRead(gDebugPortFd, line, sizeof(line));
	pLine = line;
    spaceSkip(&pLine);
  
	setSpecDataPtr[0] = 0;
	setSpecDataPtr[1] = 0;
	
	setSpecDataPtr[0] = strtoul(pLine,(char**)&pLine,16);

	if(specType == SIZE64_TYPE)
	{
		printf("\tVariable Data(64BIT:HEX) : 0x");
		ioRead(gDebugPortFd, line, sizeof(line));
		pLine = line;
		spaceSkip(&pLine);
		setSpecDataPtr[1] = strtoul(pLine,(char**)&pLine,16);
	}

	if(refDataCopyToMem(setSpecDataPtr,specData, specActionData, specType)==NO_ERROR)
	{
		if (bitposFlag== 1)  printf("\tVariable Bit Data : 0x%.1x\n",setSpecDataPtr[0]);	
		else {
			if(variableSize == SPEC_SIZE_BYTE) printf("\tVariable Data : 0x%.2x\n",setSpecDataPtr[0]);
			else if(variableSize == SPEC_SIZE_WORD) printf("\tVariable Data : 0x%.4x\n ",setSpecDataPtr[0]);	
			else if(variableSize == SPEC_SIZE_DWORD) printf("\tVariable Data : 0x%.8x\n ",setSpecDataPtr[0]);	
			else if(variableSize == SPEC_SIZE_BIT) printf("\tVariable Data : 0x%.1x\n ",setSpecDataPtr[0]);	
			else if(variableSize == SPEC_SIZE_LWORD)
			{
				printf("\tVariable Data Write : 0x%.8x ",setSpecDataPtr[0]);	
				printf(" 0x%.8x ",setSpecDataPtr[1]);
			}
			printf("\n");	
		}
	}
	else printf( "\tInvalid variable Data[%%%s] !!!\n",line);

	
	retainInfo = 0; 
	if( varRetainInfoReadwithRef(specData, specActionData, (uint32*)&retainInfo)  == NO_ERROR)
	{
		if( retainInfo!=0) {
			if( refDataCopyToBusMem(setSpecDataPtr, specData, specActionData, specType ) ==NO_ERROR)
				printf( "\tRetain variable Data BUS Write !!!\n");
		}
	}

	retainInfo = 0; 
	if( varRetainInfoReadwithRef(specData, specActionData, (uint32*)&retainInfo)  == NO_ERROR)
	{
		if( retainInfo!=0) {
				status = refDataCopyToRmtMem(setSpecDataPtr, specData, 
												specActionData, specType );
			if( status ==NO_ERROR)
				printf( "\tRetain variable Data Remote Write !!!\n");
		}
	}

	return;

}
