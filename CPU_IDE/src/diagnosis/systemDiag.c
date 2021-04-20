#include	<stdio.h>
#include	<stdlib.h>

#include	"usrTypes.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include	"segScheduler.h"
#include	"netProtocol.h"
#include	"cmdList.h"
#include	"usrErrCode.h"

#include	"systemDiag.h"
#include	"sysDual.h"

#include	"sysConfig.h"
#include	"protocolBuf.h"

#include	"taskDefs.h"
#include	"osDepend.h"
#include	"usrDefBuf.h"
#include	"usrQueData.h"

#include	"fbMem.h"

#ifdef VXWORKS
#include    "..\MHCAP\project_def.h"
#endif

/* [V105] : SM의 CAP. 전압 및 상태 관련 정의 함수 */
#define	DSP_BOARD_START_VME_ADDR			0x82B00000
#define	DSP_BOARD_VME_ADDR_OFFSET			0x00010000

#define	SUB_MODULE_CAP_VLTG_START_INDEX		0x040
#define	SUB_MODULE_STATUS_START_INDEX		0x118

#define	MAX_SUB_MODULE_NUM_PER_ARM			108

#define	M_REGION_SM_CAP_VLTG_START_INDEX	40000	/* A Phase */
#define	M_REGION_SM_STATUS_START_INDEX		42000	/* A Phase */

strSystemRunningStatusInfo	gSysRunningStatusInfo;
strSysRunningConfigInfo		*gSysRunningInfoPtr =
							&gSysRunningStatusInfo.sysRunningConfigInfo;

strMpuDiuRunningStatusInfo *gMpuRasInfoPtr =
							&gSysRunningStatusInfo.sysMpuRunningStatusInfo;
strMpuDiuRunningStatusInfo *gDiuRasInfoPtr =
							&gSysRunningStatusInfo.sysDiuRunningStatusInfo;
strNetRunningStatusInfo	   *gNetRasInfoPtr = 	
							&gSysRunningStatusInfo.sysNetRunningStatusInfo;

static uint32	sWDCRasSeqNum = 0;
static uint32	sEWSRasSeqNum = 0;

static uint32	gPrevNetCommCount = 0;

static uint32	gPrevRmtShelfNetCommCount = 0;

static uint32 sModuleLiveCount 	= 0;
static uint32 sIncreIndex 		= 0;

strBaseShelfIoInfo gBio;

static uint32 sStbyShelfNetErrPrevStatus = NO_ERROR;
static uint32 sRunShelfNetErrPrevStatus = NO_ERROR;
static uint32 sStbyShelfNetErrCnt = 0;
static uint32 sRunShelfNetErrCnt = 0;
extern int32 gDebugPortFd;

extern uint32 romSwVersion;
extern uint32 releaseDate;

extern uint32 gCurNetCommCount;

extern uint32 gRmtShelfCurNetCommCount;

extern strDiuSegSchedulerInfo	*gDiuSegSchdInfoPtr;

extern strSysConfigInfo	gSysCnfgInfo;

extern uint32	gMv4100PortErrFlag;

extern unnCpuRasInfo gunHvdcCpuRasInfo;

void 	systemRunningStatusInfoInit(void);
uint32	systemRunningStatusChange(uint32 , uint32 , uint32 *);
uint32	rasSummaryGen(strNewNetProtocolInfo	*, uint32);
void 	moduleRasSummaryBitMake(uint32, uint32 *);
void 	dualStatusBitMake(uint32 *);
void	systemRasInfoSet(uint32 , uint32 );
static void moduleSwVersionAndRevDateSet(void);
static void moduleErrDisplay(uint32 );
uint32 dualInfoStatusSet(void);
uint32 dualFlagCheckInRunShelf(void);

void networkCommErrCheck(void);
void rmtNetworkCommErrCheck(void);
static uint32 usrTaskStatusCheck(void);

void	systemRunningStatusInfoDisplay(uint32 , uint32);
uint32 dualFlagCheckInStbyShelf(void);

uint32  rasHeadInfoMake(strRasHeadInfo *);
uint32	mpuRasSummaryMake(strMpuRasSummaryInfo *);

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
uint32 liveCountSet(void);
#endif

void hvdcDiCardBaseIoCheck(void);
void hvdcDoCardBaseIoCheck(void);
void hvdcCcbBaseIoCheck(void);
void hvdcVcbBaseIoCheck(void);
void hvdcAiCardBaseIoCheck(void);

extern void memoryClear( uint8 *, uint32);
extern void memoryCopy ( uint8 *, uint8 *, uint32);
extern void setErrorCode( uint8 *, uint32, uint8 *, uint32);
extern uint32 systemCnfgInfoRead(strSysConfigInfo *);
extern uint32 netMsgChecksumCalcu	(uint32, uint32 *, uint32 *);
extern uint32 remoteNodePTFFClear(void);
extern void logMessageSet (int8 *, uint32, int8 *, uint32, uint32);
extern int32   sysTaskIdListGet (int32 *, int32 );
extern uint32   sysTaskInfoGet(int32 , void *);
extern void boardReboot();
extern void busMemFbDataCopyToLocalFbMem(void);
extern void setErrorCodeWithVal(uint8 *, uint32, uint8 *, uint32,
	uint8 *, uint32 ,uint8 *, uint32 ,uint8 *, uint32 );

extern void mpuDiuSimStatusCheck( uint32 *, uint32 *);

extern void dualStatusBitMake(uint32 *);

extern uint32	systemTimeRead(strTimeDataInfo *);
extern uint32	segmentStatusRead(uint32, uint32, strSegStatusInfo *);

extern void		fourBytesDataCopy(uint32 *, uint32 *, uint32);
extern int32	keyHit(void);
extern void		screenClear(void);
extern void		lineClear();
extern void		cursorPositionSet(void);

extern int32	ioRead(int32, int8 *, int32);
extern void		spaceSkip(int8 **);
extern uint32   sfcCurExecInfoSet(uint32);
extern uint32   BcdToHex (void *, void *, int );
extern uint32   sysTaskSuspendedCheck(int32);
extern uint32 	taskNameGet(int32 , int8 **);
extern uint32	segmentLocalDataChecksumCheck(uint32);

extern int32   ioSysCtrlFunc(int32 , int32 , int32 );
extern int32 systemTickGet(void);
extern void fbcCommErrCheck(void);
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
extern uint32 rmtShelfLiveCntUpdateAndCheck (void);
#endif

/*extern uint32	stdPMC502_RD___RunFunc_test	(void);*/

#ifdef VXWORKS
IMPORT uint	vme_start_flag;
IMPORT VME_DATA			gstVmeData[VME_MAX_SLOT];
IMPORT VME_CONTROL		gstVmeCtrl[VME_MAX_SLOT];

//IMPORT uint	link_delay_time;
IMPORT int vme_debug_flag;

static STATUS mhcap_vme_DoReadProc1 (int slotId);
static STATUS mhcap_vme_DiProc1 (int slotId);
STATUS mhcap_vme_AioProc1 (int32 slotId);

#endif

void systemRasInfoGet(uint32 , uint32 *);

extern void longDataConversion( uint32 *, uint32 *, uint32 );
extern uint32 fbMemPointerGet(uint32, strMemPointerInfo*);
static uint32 subModuleInfoReadAndWriteToMregion(uint32);

void moduleLiveCountAndFbcStatusCheck(void)
{
	strSysConfigInfo	*sysCnfgInfoPtr = &gSysCnfgInfo;
	uint32 factor = 10;
	
	sIncreIndex++;
	
   	if(((sIncreIndex)%(factor)) == 0){ 
		sModuleLiveCount += 1;

		
#if (SYSTEM_DATA_SYNC_BOARD == BUS_BRIDGE_BOARD)
		rmtShelfLiveCntUpdateAndCheck();
#endif
   	}

  	if(sIncreIndex >= (10 * factor))
       	sIncreIndex = 0;
	
	if(sModuleLiveCount >= 0xffff)
    	sModuleLiveCount = 1;

    gSysRunningStatusInfo.sysMpuRunningStatusInfo.moduleLiveCount 
    											= sModuleLiveCount;
    gSysRunningStatusInfo.sysDiuRunningStatusInfo.moduleLiveCount
    											= sModuleLiveCount;
    gSysRunningStatusInfo.sysNetRunningStatusInfo.moduleLiveCount
    											= sModuleLiveCount;
 
 	return;   											
}


static void moduleSwVersionAndRevDateSet(void)
{
	uint16	yearBcd, year;
	uint8	monthBcd, month;
	uint8	dayBcd, day;
	uint32	yearMonthDay;

	
	yearBcd  = (uint16)(releaseDate>>16)&0xFFFF;
	monthBcd = (uint8)(releaseDate>>8)&0xFF;
	dayBcd   = (uint8)(releaseDate)&0xFF;

	BcdToHex((void *)&yearBcd, (void *)&year, 2);
	BcdToHex((void *)&monthBcd, (void *)&month, 1);
	BcdToHex((void *)&dayBcd, (void *)&day, 1);
	yearMonthDay = year<<16 | month<<8 | day;

	
    
    gSysRunningStatusInfo.sysMpuRunningStatusInfo.moduleSwVersion 
    											= romSwVersion;
    gSysRunningStatusInfo.sysMpuRunningStatusInfo.moduleVersionDate 
												= yearMonthDay;

    gSysRunningStatusInfo.sysDiuRunningStatusInfo.moduleSwVersion
    											= romSwVersion;
    gSysRunningStatusInfo.sysDiuRunningStatusInfo.moduleVersionDate
												= yearMonthDay;

    gSysRunningStatusInfo.sysNetRunningStatusInfo.moduleSwVersion
    											= romSwVersion;
	gSysRunningStatusInfo.sysNetRunningStatusInfo.moduleVersionDate 
    											= yearMonthDay;

#if (MAXIOSHELF_CONFIG != MAX12IOSHELF)
	gSysRunningStatusInfo.sysFMasterRunningStatusInfo.moduleVersionDate 
    											= yearMonthDay;
#else
	gSysRunningStatusInfo.sysFMasterRunningStatusInfo.masterRomVersion[0] 
    											= yearMonthDay;
#endif

}


void systemRunningStatusInfoInit(void)
{

	memoryClear ( 	(uint8 *) &gSysRunningStatusInfo,
    				sizeof(strSystemRunningStatusInfo)
				);

	
	moduleSwVersionAndRevDateSet();
	return;
}



uint32	systemRunningStatusChange
					(
				uint32	flag,
                uint32	data,
                uint32	*retVal
                	)
{
	uint32	status = NO_ERROR;
	uint32	dualCmd = 0;
	uint32	curRunningMode = 0;
	strSysDualInfo	dualMsgInfo;
	strSysConfigInfo	sysCnfgInfo;
	uint32 taskId = 0;
	
	systemCnfgInfoRead(&sysCnfgInfo);
	
    switch(flag) {
    	case SYS_OPERATION_MODE: 
			
			if(data == SYS_RAS_SYSTEM_MANUAL_MODE)
            	dualCmd = CHG_AUTO_TO_MANUAL_CMD;
            else if(data == SYS_RAS_SYSTEM_AUTO_MODE)
            	dualCmd = CHG_MANUAL_TO_AUTO_CMD;
            else
            	status = SYS_OPERATION_MODE_ERR;

			if(status == NO_ERROR) {
                dualMsgInfo.dualCmd = dualCmd;

                
				{ 
					if(dualCmd == CHG_AUTO_TO_MANUAL_CMD) {
						gSysRunningStatusInfo.sysRunningConfigInfo.sysOperationMode
            				= data;
						*retVal = 
							gSysRunningStatusInfo.sysRunningConfigInfo.sysOperationMode;
					}
					else if(dualCmd == CHG_MANUAL_TO_AUTO_CMD){
						
												
						*retVal = data;	
						
						
						if(gDiuRasInfoPtr->redundancyStatus == SYS_REDUNDANCY_ERR)
							gSysRunningStatusInfo.sysRunningConfigInfo.sysOperationMode
            					= data; 
					}
	            }
	        }
        break;
        case SYS_RUNNING_MODE:	
        	curRunningMode = gSysRunningStatusInfo.sysRunningConfigInfo.sysRunningMode;
			
            if(curRunningMode == SYS_RAS_SYSTEM_RUN) {
        		gSysRunningStatusInfo.sysRunningConfigInfo.sysRunningMode
                		= data;
			} 
			
            
            
            
            
            else if(curRunningMode == SYS_RAS_SYSTEM_STOP) {
				
				dualCmd = CHG_RUN_TO_STANDBY_CMD;
            	
				if(data == SYS_RAS_SYSTEM_RUN) {				
					for(taskId=1;taskId<=MAX_LOGIC_TASK_NUM;taskId++) {
						status = sfcCurExecInfoSet(taskId);
						if(status!=NO_ERROR) {
							setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
						}
					}
                    
				    systemRasInfoSet(RAS_REDUNDANCY_STATUS, SYS_RAS_REDUNDANCY_RUN);

            	}
				
        		gSysRunningStatusInfo.sysRunningConfigInfo.sysRunningMode
                		= data;
			} 
			else {
            	status = SYS_RUNNING_MODE_ERR; 
				setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
				return(status);
            }
			*retVal = 
				gSysRunningStatusInfo.sysRunningConfigInfo.sysRunningMode;
        break;
        default:
        	status = SYS_STATUS_FLAG_ERR;
		break;
	}

    return(status);
}


void systemRasInfoSet(uint32 flag, uint32 val)
{
	uint32	status = NO_ERROR;

	strSysRunningConfigInfo	*sysRasPtr 
				= &gSysRunningStatusInfo.sysRunningConfigInfo;
	strMpuDiuRunningStatusInfo	*mpuRasPtr
				= &gSysRunningStatusInfo.sysMpuRunningStatusInfo;
	strMpuDiuRunningStatusInfo	*diuRasPtr
				= &gSysRunningStatusInfo.sysDiuRunningStatusInfo;
	strNetRunningStatusInfo		*netRasPtr
				= &gSysRunningStatusInfo.sysNetRunningStatusInfo;
    			
	switch(flag) {
		case RAS_SYSTEM_STATUS:
        	sysRasPtr->sysRunningMode = val;
        break;
		case RAS_SHELF_STATUS:
        	sysRasPtr->sysRedundancyRunningStatus = val;
        break;
		case RAS_REDUNDANCY_STATUS:
        	sysRasPtr->sysRedundancyStatus = val;
        break;
		case RAS_SYSTEM_MODE:
        	sysRasPtr->sysOperationMode = val;
        break;
		case RAS_SHELF_ID:
        	sysRasPtr->sysShelfId = val;
        break;
        case RAS_MPU_CHECKSUM_STATUS:
        	mpuRasPtr->moduleChecksumStatus = val;
        break;
        case RAS_DIU_CHECKSUM_STATUS:
        	diuRasPtr->moduleChecksumStatus = val;
        break;
        case RAS_REDUNDANCY_ERR_STATUS:
        	diuRasPtr->redundancyStatus = val;
        break;
        case RAS_MPU_EXEC_OVERTIME_STATUS:
        	mpuRasPtr->moduleFbOvertimeStatus = val;
        break;
        case RAS_DIU_EXEC_OVERTIME_STATUS:
        	diuRasPtr->moduleFbOvertimeStatus = val;
		break;
        case RAS_NET_COM_STATUS:
        	netRasPtr->networkStatus = val;
		break;
        case RAS_RMT_NET_COM_STATUS:
        	netRasPtr->rmtNetworkStatus = val;
        break;
        case RAS_MPU_DUAL_COPY_STATUS:
			mpuRasPtr->dualCopyErrStatus	= val;
        break;
        case RAS_DIU_DUAL_COPY_STATUS:
			diuRasPtr->dualCopyErrStatus	= val;
        break;
		case RAS_MPU_EXEC_LOOP_STATUS:
			mpuRasPtr->moduleFBLoopingStatus = val;
        break;
		default:
		break;
    }
	return;
}


void systemRasInfoGet(uint32 flag, uint32 *val)
{
	strSysRunningConfigInfo	*sysRasPtr 
				= &gSysRunningStatusInfo.sysRunningConfigInfo;
	strMpuDiuRunningStatusInfo	*mpuRasPtr
				= &gSysRunningStatusInfo.sysMpuRunningStatusInfo;
	strMpuDiuRunningStatusInfo	*diuRasPtr
				= &gSysRunningStatusInfo.sysDiuRunningStatusInfo;
	strNetRunningStatusInfo		*netRasPtr
				= &gSysRunningStatusInfo.sysNetRunningStatusInfo;
    			
	switch(flag) {
		case RAS_SYSTEM_STATUS:
        	*val = sysRasPtr->sysRunningMode;
        break;
		case RAS_SHELF_STATUS:
        	*val = sysRasPtr->sysRedundancyRunningStatus;
        break;
		case RAS_REDUNDANCY_STATUS:
        	*val = sysRasPtr->sysRedundancyStatus;
        break;
		case RAS_SYSTEM_MODE:
         	*val = sysRasPtr->sysOperationMode;
        break;
		case RAS_SHELF_ID:
         	*val = sysRasPtr->sysShelfId;
        break;
        case RAS_MPU_CHECKSUM_STATUS:
         	*val = mpuRasPtr->moduleChecksumStatus;
        break;
        case RAS_DIU_CHECKSUM_STATUS:
          	*val = diuRasPtr->moduleChecksumStatus;
       break;
        case RAS_REDUNDANCY_ERR_STATUS:
           	*val = diuRasPtr->redundancyStatus;
        break;
        case RAS_MPU_EXEC_OVERTIME_STATUS:
           	*val = mpuRasPtr->moduleFbOvertimeStatus;
        break;
        case RAS_DIU_EXEC_OVERTIME_STATUS:
           	*val = diuRasPtr->moduleFbOvertimeStatus;
		break;
        case RAS_NET_COM_STATUS:
           	*val = netRasPtr->networkStatus;
        break;
        case RAS_RMT_NET_COM_STATUS:
           	*val = netRasPtr->rmtNetworkStatus;
        break;
        case RAS_MPU_DUAL_COPY_STATUS:
           	*val = mpuRasPtr->dualCopyErrStatus;
        break;
        case RAS_DIU_DUAL_COPY_STATUS:
           	*val = diuRasPtr->dualCopyErrStatus;
        break;
		case RAS_MPU_EXEC_LOOP_STATUS:
           	*val = mpuRasPtr->moduleFBLoopingStatus;
        break;
		default:
		break;
    }
	return;
}


uint32	rasSummaryGen
					(
                strNewNetProtocolInfo	*msgPtr, uint32 rasType
					)
{
	uint32	status = NO_ERROR, checkSum = 0;
	strSysConfigInfo	sysCnfgInfo;
	uint32  rasSeqNum = 0;
	strNewRasSummaryInfo	*rasDataPtr = NULL;

    memoryClear ( (uint8 *)msgPtr, sizeof(strNewNetProtocolInfo));
    memoryClear ( (uint8 *) &sysCnfgInfo, sizeof(strSysConfigInfo));
    
    (void)systemCnfgInfoRead( &sysCnfgInfo);

    
	if (rasType == WDC_RAS ) 
	{
		if(++sWDCRasSeqNum > 0x7fffffff) 
			sWDCRasSeqNum = 1;
			
		rasSeqNum = sWDCRasSeqNum;
	}
	else if(rasType == EWS_RAS ) 
	{
		if(++sEWSRasSeqNum > 0x7fffffff) 
			sEWSRasSeqNum = 1;
			
		rasSeqNum = sEWSRasSeqNum ; 
	}
	else 
	{
		status = RAS_TYPE_ERR;
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
	}

	memoryCopy(msgPtr->companyID, COMPANY_ID,8);
	msgPtr->loopID				= 0;
    msgPtr->sourceID			= sysCnfgInfo.stationId; 
    msgPtr->destinationID		= 0x0; 
    msgPtr->messageType			= BROADCAST;
	msgPtr->sequenceID			= rasSeqNum;
	msgPtr->dualStatus			= sysCnfgInfo.shelfType;

#if (MAXIOSHELF_CONFIG == MAX16IOSHELF)
    msgPtr->dataLength			= sizeof(strNewRasSummaryInfo)- (sizeof(strIoShelfProfibusRasSummaryInfo)*2);
#elif ((MAXIOSHELF_CONFIG == MAX8IOSHELF)||(MAXIOSHELF_CONFIG == MAX12IOSHELF))
    msgPtr->dataLength			= sizeof(strNewRasSummaryInfo);
#else 
	#error "You Should Define MAXIOSHELF_CONFIG(8 or 16)!!!!"
#endif

	
    rasDataPtr = (strNewRasSummaryInfo *)msgPtr->dataBuff;

	status = rasHeadInfoMake(&rasDataPtr->rasheadInfo);
	if(status != NO_ERROR)
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);

	status = mpuRasSummaryMake(&rasDataPtr->mpuRasInfo);
	if(status != NO_ERROR)
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
		
    /* HVDC RAS */
	if(sysCnfgInfo.hvdcCpuType == HVDC_CPU_P_TYPE)
		gunHvdcCpuRasInfo.cpuC.cpuType = HVDC_CPU_P_TYPE;
	else
		gunHvdcCpuRasInfo.cpuC.cpuType = sysCnfgInfo.hvdcCpuType;
	
	if((gSysCnfgInfo.hvdcCpuType == HVDC_CPU_C_TYPE)||(gSysCnfgInfo.hvdcCpuType == HVDC_CPU_P_TYPE))
	{
#ifndef _WIN32
	    if( Lkp502CableCheck(0)==NO_ERROR ) {
	        gunHvdcCpuRasInfo.cpuC.fiberCommStat &= (~(0x1>0));
	    }
	    else {
	        gunHvdcCpuRasInfo.cpuC.fiberCommStat |= (0x1>0);
	    }
#endif

		hvdcDiCardBaseIoCheck();
		hvdcDoCardBaseIoCheck();
	}
	
	if((gSysCnfgInfo.hvdcCpuType == HVDC_CPU_C_TYPE))
	{
		hvdcCcbBaseIoCheck();
	}
	if((gSysCnfgInfo.hvdcCpuType == HVDC_CPU_V_TYPE))
	{
		hvdcVcbBaseIoCheck();
	}
	if((gSysCnfgInfo.hvdcCpuType == HVDC_CPU_M_TYPE))
	{
		hvdcAiCardBaseIoCheck();
	}
	
	fourBytesDataCopy((uint32*)&rasDataPtr->rasheadInfo.YearMonthDay, (uint32*)&gunHvdcCpuRasInfo,((msgPtr->dataLength)/4)-1/*sizeof(unnCpuRasInfo)*/); /* OP CODE 코드 부분을 뺌 */

    netMsgChecksumCalcu	(
    				msgPtr->dataLength,
                    (uint32 *)msgPtr->dataBuff,
                    (uint32 *) &checkSum
                    	);
	msgPtr->checkSum = checkSum;

	return(status);
}

uint32	ewsRasSummaryGen
					(
                strNewNetProtocolInfo	*msgPtr
					)
{
	uint32	status = NO_ERROR, checkSum = 0;
	uint32  rasSeqNum = 0;
	
    
	if(++sEWSRasSeqNum > 0x7fffffff) 
		sEWSRasSeqNum = 1;
	
	rasSeqNum 			= sEWSRasSeqNum ;
	
	msgPtr->sequenceID	= rasSeqNum;

	
    netMsgChecksumCalcu	(
    				msgPtr->dataLength,
                    (uint32 *)msgPtr->dataBuff,
                    (uint32 *) &checkSum
                    	);
	msgPtr->checkSum = checkSum;

	return(status);
}




uint32  rasHeadInfoMake(strRasHeadInfo *mRasHeadInfo)
{
	uint32 status = NO_ERROR;
	strTimeDataInfo		sysTime;
	uint32 mpuSimStatus, diuSimStatus;
	strSysConfigInfo sysCnfgInfo;
	uint32 rasProtocolType;

	memoryClear((uint8*)&sysTime,sizeof(strTimeDataInfo));

	status = systemTimeRead(&sysTime);

	if((status != NO_ERROR))	return(status);

	mRasHeadInfo->opCommand 		= SYS_STATUS_CMD << 16;
	mRasHeadInfo->opCommand 		|= SYS_DIAG_STATUS & 0xffff ;

	mRasHeadInfo->YearMonthDay 		=  (sysTime.year&0xffff)  << 16;
	mRasHeadInfo->YearMonthDay 		|= (sysTime.month&0xff) << 8;
	mRasHeadInfo->YearMonthDay 		|= (sysTime.date  &0xff);
	
	gunHvdcCpuRasInfo.cpuC.date = (((sysTime.year-2000)&0xff) << 24) | ((sysTime.month&0xff) << 16) | ((sysTime.date&0xff) << 8) | ((sysTime.hour&0xff));

	mRasHeadInfo->HourMinSecmSec 	=  (sysTime.hour&0xff)  << 24;
	mRasHeadInfo->HourMinSecmSec 	|= (sysTime.minute&0xff) << 16;
	mRasHeadInfo->HourMinSecmSec 	|= (sysTime.second&0xff) << 8;
	mRasHeadInfo->HourMinSecmSec 	|= (sysTime.mSec/10 &0xff);
	
	gunHvdcCpuRasInfo.cpuC.time = ((sysTime.minute&0xff) << 24) | ((sysTime.second&0xff) << 16) | ((sysTime.mSec&0xffff));

	dualStatusBitMake (&mRasHeadInfo->dualStatus);

	mpuDiuSimStatusCheck( &mpuSimStatus, &diuSimStatus);
	
	mRasHeadInfo->forcingStatus =  (LOGIC_ID&0xff)  << 24;
	mRasHeadInfo->forcingStatus |= (mpuSimStatus&0xff) << 16;
	

#if 0
{
	strSysRunningConfigInfo			*dualInfoPtr;

	dualInfoPtr = &gSysRunningStatusInfo.sysRunningConfigInfo;

    *dataPtr  = dualInfoPtr->sysRunningMode;
    *dataPtr |= dualInfoPtr->sysRedundancyRunningStatus << 2;
    *dataPtr |= dualInfoPtr->sysRedundancyStatus 		<< 4;
    *dataPtr |= dualInfoPtr->sysOperationMode			<< 6;
    *dataPtr |= dualInfoPtr->sysShelfId 				<< 8;
}
    *dataPtr |= dualInfoPtr->sysShelfId 				<< 8;
    *dataPtr |= dualInfoPtr->sysOperationMode			<< 6;
    *dataPtr |= dualInfoPtr->sysRedundancyStatus 		<< 4;
    *dataPtr  = dualInfoPtr->sysRunningMode;
    
    *dataPtr |= dualInfoPtr->sysRedundancyRunningStatus << 2;
#endif

#if 1
gunHvdcCpuRasInfo.cpuC.dualSideId = (((gSysRunningStatusInfo.sysRunningConfigInfo.sysRunningMode)&0x3)<<30) |
                                    (((gSysRunningStatusInfo.sysRunningConfigInfo.sysRedundancyStatus)&0x3)<<28) |
                                    (((gSysRunningStatusInfo.sysRunningConfigInfo.sysOperationMode)&0x3)<<26) |
                                    (((gSysRunningStatusInfo.sysRunningConfigInfo.sysShelfId)&0x3)<<24) | (mpuSimStatus&0xFF); 
#else
    gunHvdcCpuRasInfo.cpuC.dualSideId = 0x12345678;
#endif
	
	mRasHeadInfo->lineCount = 0;
	
	systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);
	
	mRasHeadInfo->rcsHwCnfg = (BOARD_DISABLE & 0xffff) <<16;

#if (MAXIOSHELF_CONFIG == MAX16IOSHELF)
	rasProtocolType = RAS_PROTOCOL_MP5000NEW;
#elif (MAXIOSHELF_CONFIG == MAX8IOSHELF)
	rasProtocolType = RAS_PROTOCOL_MP5000;
#elif (MAXIOSHELF_CONFIG == MAX12IOSHELF)
	rasProtocolType = RAS_PROTOCOL_MP3000AT;
#else 
	#error "You Should Define MAXIOSHELF_CONFIG(8 or 16)!!!!"

#endif
	mRasHeadInfo->rcsHwCnfg |= rasProtocolType; 

    return(status);
}


uint32	mpuRasSummaryMake(strMpuRasSummaryInfo *mMpuRasInfo)
{
	uint32 status = NO_ERROR;
	strMpuDiuRunningStatusInfo *mpuStatusInfoPtr = gMpuRasInfoPtr;
	strSegStatusInfo segStatus;
	uint32 taskId;
	uint32	*segStatusPtr, i, j;
    
	
	moduleRasSummaryBitMake( 
						LOGIC_ID, 
						(uint32 *)&mMpuRasInfo->mpuRasSummary.rasSummary
                        	);
	mMpuRasInfo->mpuRasSummary.liveCount = mpuStatusInfoPtr->moduleLiveCount;
	mMpuRasInfo->mpuRasSummary.romVersion = mpuStatusInfoPtr->moduleSwVersion;
	
	gunHvdcCpuRasInfo.cpuC.cpuRomVersion = (((mMpuRasInfo->mpuRasSummary.romVersion - (mMpuRasInfo->mpuRasSummary.romVersion%100))/100)<<8)|(mMpuRasInfo->mpuRasSummary.romVersion%100);
	
	mMpuRasInfo->mpuRasSummary.releaseDate = mpuStatusInfoPtr->moduleVersionDate; 

	
	segStatusPtr = &mMpuRasInfo->mpuTaskStatus[0];
	for(i = 0; i < MAX_LOGIC_TASK_NUM/8; i++, segStatusPtr++) {
    	*segStatusPtr = 0;

		for(j = 8; j > 0; j--) {
			taskId = j + (i * 8);
			status = segmentStatusRead (LOGIC_ID, taskId, &segStatus);
			if(status == NO_ERROR) {
				*segStatusPtr |= segStatus.taskMode << (4*((j-1)%8));
				if((gSysCnfgInfo.hvdcCpuType == HVDC_CPU_C_TYPE)||(gSysCnfgInfo.hvdcCpuType == HVDC_CPU_P_TYPE))
				{
	                if(i==0) {
	                    gunHvdcCpuRasInfo.cpuC.taskStatus = *segStatusPtr;
	                }
            	}
			}
        }
    }

	return(status);
}

void moduleRasSummaryBitMake
				(
			uint32	moduleId,
			uint32	*dataPtr
            	)
{
	strMpuDiuRunningStatusInfo		*mpuRasPtr;
	strMpuDiuRunningStatusInfo		*diuRasPtr;
	strNetRunningStatusInfo			*netRasPtr;
	strFBCRunningStatusInfo			*fbcRasPtr;
	uint32 cpuErrSummary = 0, retVal;

    switch(moduleId) 
    {
    	case LOGIC_ID:
			mpuRasPtr = &gSysRunningStatusInfo.sysMpuRunningStatusInfo;										
			diuRasPtr = &gSysRunningStatusInfo.sysDiuRunningStatusInfo;

            *dataPtr	= mpuRasPtr->moduleChecksumStatus;			
			*dataPtr	|= mpuRasPtr->moduleFbRunningStatus	<< 1;	
	        *dataPtr	|= mpuRasPtr->moduleFbOvertimeStatus<< 2;	
            *dataPtr	|= diuRasPtr->redundancyStatus		<< 3; 	
            *dataPtr	|= mpuRasPtr->moduleFBLoopingStatus << 4; 	
            *dataPtr	|= mpuRasPtr->dualCopyErrStatus 	<< 5; 	
            
            /*gunHvdcCpuRasInfo.cpuC.dualSideId = gSysRunningStatusInfo.sysRunningConfigInfo.sysShelfId;*/
            cpuErrSummary = 0;
            cpuErrSummary |= mpuRasPtr->moduleChecksumStatus<<0;
	        cpuErrSummary |= mpuRasPtr->moduleFbOvertimeStatus<<1;	  			
	        cpuErrSummary |= mpuRasPtr->moduleFBLoopingStatus<<2;
	        	
	        retVal = usrTaskStatusCheck();	
	        
	        if(retVal == (uint32)(APP_TASK_SUSPEND_ERR))
	        {
	            cpuErrSummary	|= 1<< 3;
	        }
	        else
	        {
	            cpuErrSummary	&= ~(1<< 3); 
	        }
            
            if((gSysCnfgInfo.hvdcCpuType == HVDC_CPU_C_TYPE)||(gSysCnfgInfo.hvdcCpuType == HVDC_CPU_P_TYPE))
            {
	            if(     gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[0]
	                ||  gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[1]
	                ||  gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[2]
	                ||  gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[3]
	                ||  gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[4]
	            )
	                cpuErrSummary |= 1<<5;
	                
	            if(     gunHvdcCpuRasInfo.cpuC.aioBoardErrStat != 0
	            ) {
	                cpuErrSummary |= 1<<6;
	            }
	                
            }
            else if((gSysCnfgInfo.hvdcCpuType == HVDC_CPU_V_TYPE))
            {
	            if(     gunHvdcCpuRasInfo.cpuV.aioBoardErrStat[0]
	                ||  gunHvdcCpuRasInfo.cpuV.aioBoardErrStat[1]
	                ||  gunHvdcCpuRasInfo.cpuV.aioBoardErrStat[2]
	                ||  gunHvdcCpuRasInfo.cpuV.aioBoardErrStat[3]
	                ||  gunHvdcCpuRasInfo.cpuV.aioBoardErrStat[4]
	                ||  gunHvdcCpuRasInfo.cpuV.aioBoardErrStat[5]
	            )
	                cpuErrSummary |= 1<<5;
            }
            else if((gSysCnfgInfo.hvdcCpuType == HVDC_CPU_M_TYPE))
            {
	            if(     gunHvdcCpuRasInfo.cpuM.aioBoardErrStat[0]
	                ||  gunHvdcCpuRasInfo.cpuM.aioBoardErrStat[1]
	                ||  gunHvdcCpuRasInfo.cpuM.aioBoardErrStat[2]
	                ||  gunHvdcCpuRasInfo.cpuM.aioBoardErrStat[3]
	                ||  gunHvdcCpuRasInfo.cpuM.aioBoardErrStat[4]
	                ||  gunHvdcCpuRasInfo.cpuM.aioBoardErrStat[5]
	            )
	                cpuErrSummary |= 1<<5;
	                
            }
            
        	 
            gunHvdcCpuRasInfo.cpuC.cpuErrSummary = cpuErrSummary;
               
		break;

        case NET_ID:
			netRasPtr = &gSysRunningStatusInfo.sysNetRunningStatusInfo;

            *dataPtr	= netRasPtr->moduleReadyErr;
            *dataPtr	|= netRasPtr->moduleLiveCountStatus << 1;
            *dataPtr	|= netRasPtr->networkStatus			<< 9;	
            *dataPtr	|= moduleId 						<< 16;
  		break;
	}
}


void dualStatusBitMake 
					(
			uint32	*dataPtr
					) 
{
	strSysRunningConfigInfo			*dualInfoPtr;

	dualInfoPtr = &gSysRunningStatusInfo.sysRunningConfigInfo;

    *dataPtr  = dualInfoPtr->sysRunningMode;
    *dataPtr |= dualInfoPtr->sysRedundancyRunningStatus << 2;
    *dataPtr |= dualInfoPtr->sysRedundancyStatus 		<< 4;
    *dataPtr |= dualInfoPtr->sysOperationMode			<< 6;
    *dataPtr |= dualInfoPtr->sysShelfId 				<< 8;
}



void systemRunningStatusInfoDisplay(uint32 arg1, uint32 arg2)
{
	strSysConfigInfo sysCnfgInfo;
	
	strSysRunningConfigInfo	*sysModeInfoPtr 
						= &gSysRunningStatusInfo.sysRunningConfigInfo;
	strMpuDiuRunningStatusInfo	*diuRasPtr
				= &gSysRunningStatusInfo.sysDiuRunningStatusInfo;

	int32	strIndex = 0;

	int8 *shelfTypeStr[] =
    {
		"UNKNOWN", "MASTER SHELF", "SLAVE SHELF"
    };
    int8 *operationModeStr[] =
    {
    	"UNKNOWN MODE", "AUTO MODE", "MANUAL MODE", "UNKNOWN MODE"
	};
	int8 *redundancyModeStr[] =
    {
		 "UNKNOWN", "RUN", "STAND BY", "UNKNOWN"
    };
	int8 *redundancyRunningStatusStr[] =
    {
    	"UNKNOWN", "MASTER RUN", "SLAVE RUN "
    };
	int8 *sysRunningStatusStr[] =
    {
    	"UNKNOWN", "SYSTEM RUN ", "SYSTEM STOP"
    };
	int8 *redundancyErrStr[] =
    {
    	"DUAL  ", "SINGLE", "UNKNOWN"
    };
    int8 *dualFlagStr[] = 
    {
    	"ENABLE ", "DISABLE"
    };

    systemCnfgInfoRead( &sysCnfgInfo);

	printf( "\n");
	printf( "\t********* System Status Info **********\n");

    
    strIndex = sysModeInfoPtr->sysShelfId;
    if( (strIndex < SYS_RAS_MASTER_SHELF_ID) || 
		(strIndex > SYS_RAS_SLAVE_SHELF_ID)
	)
    	strIndex = 0;
	printf( "\t          Shelf Type : %s\n",shelfTypeStr[strIndex]);

		  	
	strIndex = sysModeInfoPtr->sysRunningMode;
    if( (strIndex < SYS_RAS_SYSTEM_RUN) ||
    	(strIndex > SYS_RAS_SYSTEM_STOP)
	)
    	strIndex = 0;
	printf( "\tShelf Running Status : %s\n",sysRunningStatusStr[strIndex]);

	
	strIndex = sysModeInfoPtr->sysOperationMode;
    if( (strIndex < SYS_RAS_SYSTEM_AUTO_MODE) ||
    	(strIndex > SYS_RAS_SYSTEM_MANUAL_MODE)
	)
    	strIndex = 2;
	printf( "\tShelf Operation Mode : %s\n",operationModeStr[strIndex]);

	 
	strIndex = sysModeInfoPtr->sysRedundancyStatus;
    if( (strIndex < SYS_RAS_REDUNDANCY_RUN) ||
    	(strIndex > SYS_RAS_REDUNDANCY_STANDBY)
	)
    	strIndex = 0;
	printf( "\t   Shelf Dual Status : %s\n",redundancyModeStr[strIndex]);
	
		  	
	strIndex = sysModeInfoPtr->sysRedundancyRunningStatus;
    if( (strIndex < SYS_RAS_MASTER_RUN) ||
    	(strIndex > SYS_RAS_SLAVE_RUN)
	)
    	strIndex = 0;
	printf( "\t Shelf Dual Run Mode : %s\n",redundancyRunningStatusStr[strIndex]);

    
	strIndex = diuRasPtr->redundancyStatus;
    if( (strIndex < NO_ERROR) ||
    	(strIndex > SYS_REDUNDANCY_ERR)
	)
    	strIndex = 3;
	printf( "\t   Redundancy Status : %s\n",redundancyErrStr[strIndex]);
    
    printf( "\n");
	printf( "\t* Cpu Live Count : %6d\n",
    		    gSysRunningStatusInfo.sysMpuRunningStatusInfo.moduleLiveCount 
			);
	lineClear();
	moduleErrDisplay(LOGIC_ID);
	
MODULE_END:

	return;
}

static void moduleErrDisplay(uint32 moduleId)
{
	strMpuDiuRunningStatusInfo	*dataPtr = NULL;
	strFBCRunningStatusInfo		*fbcPtr = NULL;
	strNetRunningStatusInfo	*netPtr	= NULL;
	strFMasterRunningStatusInfo		*fmcPtr = NULL;	

	if(moduleId == LOGIC_ID)
		dataPtr = gMpuRasInfoPtr;
	else if(moduleId == NET_ID)
		netPtr = gNetRasInfoPtr;

	if((moduleId == NET_ID) && (netPtr->networkStatus != NO_ERROR)){
		printf("\t<NetworkErr>");
		goto MODULE_END;
	}

	if( ((moduleId == LOGIC_ID)) && (dataPtr->moduleReadyErr != NO_ERROR))
    	printf("\t<ModuleReadyErr>");

	if(((moduleId == LOGIC_ID)) && (dataPtr->moduleSysCallErr != NO_ERROR))
		printf("\t<SysCallErr>");

	if(((moduleId == LOGIC_ID)) && (dataPtr->dualCopyErrStatus != NO_ERROR))
		printf("\t<DualCopyErr>");

	if(((moduleId == LOGIC_ID)) && (dataPtr->moduleLiveCountStatus != NO_ERROR))
		printf("\t<LiveCountErr>");

	if(((moduleId == LOGIC_ID)) && (dataPtr->moduleChecksumStatus != NO_ERROR))
		printf("\t<ChecksumErr>");

	if(((moduleId == LOGIC_ID)) && (dataPtr->moduleFbRunningStatus != NO_ERROR))
		printf("\t<FbRunningErr>");

	if(((moduleId == LOGIC_ID)) && (dataPtr->moduleFbOvertimeStatus != NO_ERROR))
		printf("\t<FbOverTimeErr>");

	if((moduleId == LOGIC_ID) && (dataPtr->moduleFBLoopingStatus != NO_ERROR))
		printf("\t<JumpLoopingErr>");

MODULE_END:
	printf("\n");
}

uint32 dualInfoStatusSet(void)
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	strNetRunningStatusInfo		*netStatusInfoPtr;
    strMpuDiuRunningStatusInfo	*mpuDiuStatusInfoPtr;
	strSysRunningConfigInfo		*sysStatusInfoPtr;
	uint32	errorSummary = 0;
	strFBCRunningStatusInfo		*fbcStatusInfoPtr = NULL;
	strFMasterRunningStatusInfo		*fmcStatusInfoPtr = NULL;	
	strIOShelfRunningStatusInfo	*shelfInfoPtr;

	sysStatusInfoPtr = &gSysRunningStatusInfo.sysRunningConfigInfo;

	
	
	mpuDiuStatusInfoPtr = &gSysRunningStatusInfo.sysDiuRunningStatusInfo;

	shelfInfoPtr = &gSysRunningStatusInfo.sysIOShelfStatusInfo[0];

	
	fbcStatusInfoPtr = &gSysRunningStatusInfo.sysFbcRunningStatusInfo;

	
	fmcStatusInfoPtr = &gSysRunningStatusInfo.sysFMasterRunningStatusInfo;	
	
	if(fbcStatusInfoPtr->moduleLiveCountStatus != NO_ERROR) 
		errorSummary |= DUAL_FBC_LIVECNT_ERR;

	
		
	if(fmcStatusInfoPtr->moduleLiveCountStatus != NO_ERROR)
		errorSummary |= DUAL_FMC_LIVECNT_ERR;	
	else  	
		errorSummary &= ~DUAL_FMC_LIVECNT_ERR;
	
	if(mpuDiuStatusInfoPtr->fmcIOCommStatus != NO_ERROR)
		errorSummary |= DUAL_FMC_IO_COMM_ERR; 
	else  	
		errorSummary &= ~DUAL_FMC_IO_COMM_ERR;

	
	netStatusInfoPtr = &gSysRunningStatusInfo.sysNetRunningStatusInfo;								

	if(netStatusInfoPtr->networkStatus != NO_ERROR)
		errorSummary 	|= DUAL_CTRL_NET_ERR;
	else
    	errorSummary	&= ~DUAL_CTRL_NET_ERR;

	
	retVal = usrTaskStatusCheck();

	if(retVal == (uint32)(APP_TASK_SUSPEND_ERR)){
		errorSummary 	|= USR_TASK_SUSPEND_ERR;
		
		
    	if(	(mpuDiuStatusInfoPtr->redundancyStatus != NO_ERROR) ||
    		(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_STANDBY)
    	){
			logMessageSet(	__FILE__, 
							__LINE__,
							"dualInfoStatusSet", 
							SYS_FORCED_REBOOT,0
							);
    		boardReboot();
    	}
    }
    
	
									
	if( dualFlagCheckInRunShelf() == DUAL_CHECK_ENABLE){
	    
    }

    return(status);
}



uint32 dualFlagCheckInRunShelf(void)
{
	uint32	status = DUAL_CHECK_DISABLE;
	strSysRunningConfigInfo		*sysCnfgInfoPtr;
	strMpuDiuRunningStatusInfo	*diuStatusInfoPtr;

	sysCnfgInfoPtr = (strSysRunningConfigInfo *)
							&gSysRunningStatusInfo.sysRunningConfigInfo;
	diuStatusInfoPtr = (strMpuDiuRunningStatusInfo *)
							&gSysRunningStatusInfo.sysDiuRunningStatusInfo;

    
    
     
	if ( 
		(sysCnfgInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN)
           				&&
		(sysCnfgInfoPtr->sysOperationMode == SYS_RAS_SYSTEM_AUTO_MODE)
           				&&
		(diuStatusInfoPtr->redundancyStatus == NO_ERROR)
	)

	   	status = DUAL_CHECK_ENABLE;

    return(status);
}


uint32 dualFlagCheckInStbyShelf(void)
{
	uint32	status = DUAL_CHECK_DISABLE;
	strSysRunningConfigInfo		*sysCnfgInfoPtr;
	strMpuDiuRunningStatusInfo	*diuStatusInfoPtr;

	sysCnfgInfoPtr = (strSysRunningConfigInfo *)
							&gSysRunningStatusInfo.sysRunningConfigInfo;
	diuStatusInfoPtr = (strMpuDiuRunningStatusInfo *)
							&gSysRunningStatusInfo.sysDiuRunningStatusInfo;

    
    
     
	if ( 
		(sysCnfgInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_STANDBY)
           				&&
		(sysCnfgInfoPtr->sysOperationMode == SYS_RAS_SYSTEM_AUTO_MODE)
           				&&
		(diuStatusInfoPtr->redundancyStatus == NO_ERROR)
	)
	   	status = DUAL_CHECK_ENABLE;

    return(status);
}


void networkCommErrCheck(void)
{
	uint32	netErrStatus = NO_ERROR;
	strMpuDiuRunningStatusInfo	*diuStatusInfoPtr;
	strSysRunningConfigInfo		*sysStatusInfoPtr;
	
 	    
	sysStatusInfoPtr = &gSysRunningStatusInfo.sysRunningConfigInfo;
	diuStatusInfoPtr = &gSysRunningStatusInfo.sysDiuRunningStatusInfo;

    
    if(diuStatusInfoPtr->redundancyStatus == NO_ERROR){ 
    
    	
    	if(gSysRunningInfoPtr->sysOperationMode == SYS_RAS_SYSTEM_AUTO_MODE) {
			if(gCurNetCommCount == gPrevNetCommCount) 
			{
				netErrStatus = 1;
			}
		}
		
		if(		(netErrStatus != 1)
			&&	(gMv4100PortErrFlag != 0)	) 
		{
			netErrStatus = 1;
		}

        if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_STANDBY) {
        	
        	if(sStbyShelfNetErrPrevStatus == NO_ERROR) {
				systemRasInfoSet(RAS_NET_COM_STATUS, netErrStatus);			
				sStbyShelfNetErrPrevStatus = netErrStatus;	
				sStbyShelfNetErrCnt = 0;
        	}
        	else { 
        		
        		if(netErrStatus == NO_ERROR) {
	        		if(sStbyShelfNetErrCnt++ >= 1) {
						systemRasInfoSet(RAS_NET_COM_STATUS, netErrStatus);	
						sStbyShelfNetErrPrevStatus = netErrStatus;	
						sStbyShelfNetErrCnt = 0;        			
	
						if(netErrStatus)
							logMessageSet(	__FILE__, 
											__LINE__,
											"networkCommErrCheck", 
											CTRL_NET_COM_ERR,0
											);
	        		}
				}
        	}			
        }
        else { 
        	
        	if((sRunShelfNetErrPrevStatus == NO_ERROR) && (netErrStatus)){
        		if(sRunShelfNetErrCnt++ >= 1) {
					systemRasInfoSet(RAS_NET_COM_STATUS, netErrStatus);		
					sRunShelfNetErrPrevStatus = netErrStatus;	
					sRunShelfNetErrCnt = 0;        			

					if(netErrStatus)
						logMessageSet(	__FILE__, 
										__LINE__,
										"networkCommErrCheck", 
										CTRL_NET_COM_ERR,0
										);
        		}        		
        	}
        	else {
        			
				systemRasInfoSet(RAS_NET_COM_STATUS, netErrStatus);			
				sRunShelfNetErrPrevStatus = netErrStatus;	
				sRunShelfNetErrCnt = 0;        		
        	}
        }
        
		gPrevNetCommCount = gCurNetCommCount;
	}
	else {
    	
		systemRasInfoSet(RAS_NET_COM_STATUS, NO_ERROR);						

		sStbyShelfNetErrPrevStatus 	= NO_ERROR;
		sRunShelfNetErrPrevStatus 	= NO_ERROR;
		sStbyShelfNetErrCnt 		= 0;
		sRunShelfNetErrCnt 			= 0;
       
        
         
		gCurNetCommCount++;
    }
    
	return;                             
}


void rmtNetworkCommErrCheck(void)
{
	uint32	netErrStatus = NO_ERROR;
	strMpuDiuRunningStatusInfo	*diuStatusInfoPtr;
	strSysRunningConfigInfo		*sysStatusInfoPtr;

 	    
	sysStatusInfoPtr = &gSysRunningStatusInfo.sysRunningConfigInfo;
	diuStatusInfoPtr = &gSysRunningStatusInfo.sysDiuRunningStatusInfo;
	
	netErrStatus = 0;
	                     
	if(sysStatusInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_STANDBY){
		 
    	if( (gRmtShelfCurNetCommCount == gPrevRmtShelfNetCommCount)&&
		    (sysStatusInfoPtr->sysOperationMode == SYS_RAS_SYSTEM_AUTO_MODE)
		){
    		netErrStatus = 1;
			setErrorCode(__FILE__,__LINE__,__FUNCTION__, RMT_CTRL_NET_COM_ERR);
		}
		else 
			netErrStatus = 0;

		systemRasInfoSet(RAS_RMT_NET_COM_STATUS, netErrStatus);

  		gPrevRmtShelfNetCommCount = gRmtShelfCurNetCommCount;
	}
	else {
		systemRasInfoSet(RAS_RMT_NET_COM_STATUS, NO_ERROR);
        
         
		gRmtShelfCurNetCommCount++;
	} 

	return;                             
}


#ifdef VXWORKS
static uint32 usrTaskStatusCheck(void)
{
    int32  idList[MAX_USR_TASK_NUM];
    int32  retTaskNum, index;
	uint32	status = NO_ERROR;
    int8    *taskNamePtr = NULL; 
	
	memoryClear( (uint8 *)idList, MAX_USR_TASK_NUM * 4);

    
    retTaskNum = sysTaskIdListGet( (int32 *)idList, MAX_USR_TASK_NUM);


    if(retTaskNum == 0){
    	status = TASK_NUMBER_ERR;
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
        goto MODULE_END;
    }

    for(index = 0; index < retTaskNum; index++){
        if((status = sysTaskSuspendedCheck(idList[index]))!= NO_ERROR){
			status = APP_TASK_SUSPEND_ERR;
			taskNameGet(idList[index], &taskNamePtr);
			logMessageSet(	__FILE__, 
							__LINE__,
							taskNamePtr, 
							status, 0
						);
			return(status);
		}
    }

MODULE_END:

	return(status);
}
#else
extern uint32   usrTaskInfoStatusGet(int32 , void *);
static uint32 usrTaskStatusCheck(void)
{
	uint32	status = NO_ERROR;
	int32  idList[MAX_USR_TASK_NUM];
	int32  retTaskNum, index;
	strTaskListInfo taskInfo[MAX_USR_TASK_NUM];
	
	memoryClear( (uint8 *)taskInfo, sizeof(strTaskListInfo));
	memoryClear( (uint8 *)idList, MAX_USR_TASK_NUM * 4);

    
    retTaskNum = sysTaskIdListGet( (int32 *)idList, MAX_USR_TASK_NUM);
    if(retTaskNum == 0){
        printf( "\n[ERROR] - No available Task Id List\n");
        goto MODULE_END;
    }
    for(index = 0; index < retTaskNum; index++){
        if((usrTaskInfoStatusGet(idList[index], (void *)&taskInfo[index]))!= NO_ERROR){
			setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
        }

#ifdef VXWORKS
		if(taskIsSuspended(idList[index]) != NO_ERROR) {
#else
		if(taskInfo[index].td_status == TASK_SUSPEND) {
#endif
			
			status = APP_TASK_SUSPEND_ERR;
			logMessageSet(	__FILE__, 
							__LINE__,
							taskInfo[index].td_name, 
							status, 0
						);
			return(status);
		}

    }

MODULE_END:
	return(status);
}
#endif


uint32 rmtBusCopyCheck(void)
{
	uint32 rmtBusCopyFlag;
	if( (gSysRunningInfoPtr->sysRedundancyStatus == SYS_RAS_REDUNDANCY_RUN) &&
		(gSysRunningInfoPtr->sysOperationMode == SYS_RAS_SYSTEM_AUTO_MODE) &&
		(gDiuRasInfoPtr->redundancyStatus == NO_ERROR))
	{
		rmtBusCopyFlag = 0;  
	}
	else 
	{ 
		rmtBusCopyFlag = 1;
	}
	
	return(rmtBusCopyFlag);
}

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)

uint32 liveCountSet(void)
{
	uint32	status = NO_ERROR;
    strMpuDiuRunningStatusInfo	*mpuDiuStatusInfoPtr;

	mpuDiuStatusInfoPtr = &gSysRunningStatusInfo.sysDiuRunningStatusInfo;

	return(status);
}
#endif

void hvdcDiCardBaseIoCheck(void)
{
	uint32	slotIndex = 0;
	strSysConfigInfo	sysCnfgInfo;

    memoryClear ( (uint8 *) &sysCnfgInfo, sizeof(strSysConfigInfo));
    (void)systemCnfgInfoRead( &sysCnfgInfo);
	
	/* 알고리즘 연산부 시작 */
    for(slotIndex=0;slotIndex<5;slotIndex++)
    {
	    if((sysCnfgInfo.hvdcDiConfig>>slotIndex)&0x1)
	    {
	#ifndef _WIN32
			if((slotIndex>=0)&&(slotIndex<=4))
			{
			    
		        mhcap_vme_DiProc1(slotIndex);
		
		        if(gstVmeCtrl[slotIndex].slave_status != TRUE) 
		            gBio.diCard[slotIndex].errCode = 1; // 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
		        
		        else if(gstVmeCtrl[slotIndex].slave_board_type != DI_BOARD_ID) 
		            gBio.diCard[slotIndex].errCode = 2; // 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
		        
		        else if(gstVmeCtrl[slotIndex].stat.slave_init_flag_ok == 0)
		            gBio.diCard[slotIndex].errCode = 3; // 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
		        
		        else
		            gBio.diCard[slotIndex].errCode = 0;
		            
				if((gBio.diCard[slotIndex].errCode==0)&&(gBio.diCard[slotIndex].liveCnt == gstVmeCtrl[slotIndex].slave_run_count))
				{
		    		gBio.diCard[slotIndex].errCode = 4;	// 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)		
		    		gstVmeCtrl[slotIndex].slave_status = FALSE;
		    	}
		    		
		        gBio.diCard[slotIndex].liveCnt = gstVmeCtrl[slotIndex].slave_run_count;
			}
			else
			{
				gBio.diCard[slotIndex].errCode = 0;
			}
			
			switch(gunHvdcCpuRasInfo.cpuC.cpuType)
			{
			    case HVDC_CPU_C_TYPE:
			        if(     (slotIndex>=0)
			            &&  (slotIndex<MAX_CPU_C_DIO_BOARD_NUM)   )
			        {
		        	    gunHvdcCpuRasInfo.cpuC.cpuType;
		        	    gunHvdcCpuRasInfo.cpuC.dioBoardCfgStat &= ~(0x3<<(slotIndex*2));
		        	    gunHvdcCpuRasInfo.cpuC.dioBoardCfgStat |= (DIO_DI_TYPE<<(slotIndex*2));
		        	    switch(gBio.diCard[slotIndex].errCode)  /*(입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)*/
		        	    {
		        	        case 0: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 0<<0; break; 
		        	        case 1: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 1<<0; break;
		        	        case 2: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 1<<2; break;
		        	        case 3: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 1<<1; break;
		        	        case 4: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 1<<1; break;
		        	    }
		            }
			        break;
			    case HVDC_CPU_P_TYPE:
			        if(     (slotIndex>=0)
			            &&  (slotIndex<MAX_CPU_P_DIO_BOARD_NUM)   )
			        {
		        	    gunHvdcCpuRasInfo.cpuP.cpuType;
		        	    gunHvdcCpuRasInfo.cpuP.dioBoardCfgStat &= ~(0x3<<(slotIndex*2));
		        	    gunHvdcCpuRasInfo.cpuP.dioBoardCfgStat |= (DIO_DI_TYPE<<(slotIndex*2));
		        	    switch(gBio.diCard[slotIndex].errCode)  /*(입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)*/
		        	    {
		        	        case 0: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 0<<0; break; 
		        	        case 1: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 1<<0; break;
		        	        case 2: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 1<<2; break;
		        	        case 3: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 1<<1; break;
		        	        case 4: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 1<<1; break;
		        	    }
		            }
			        break;
			    case HVDC_CPU_V_TYPE:
			        gunHvdcCpuRasInfo.cpuV.cpuType;
			        break;
			    case HVDC_CPU_M_TYPE:
		    	    gunHvdcCpuRasInfo.cpuM.cpuType;
			        break;
			    default:
			        break;
			}
	#endif
	    }
	    else if(!(sysCnfgInfo.hvdcDoConfig>>slotIndex)&0x1)
	    {
	    	gunHvdcCpuRasInfo.cpuC.dioBoardCfgStat &= ~(0x3<<(slotIndex*2));
	    }
    }
    /* 알고리즘 연산부 끝 */
}

void hvdcDoCardBaseIoCheck(void)
{
	uint32	slotIndex = 0;
	strSysConfigInfo	sysCnfgInfo;

    memoryClear ( (uint8 *) &sysCnfgInfo, sizeof(strSysConfigInfo));
    (void)systemCnfgInfoRead( &sysCnfgInfo);
    
	/* 알고리즘 연산부 시작 */
    for(slotIndex=0;slotIndex<5;slotIndex++)
    {
	    if((sysCnfgInfo.hvdcDoConfig>>slotIndex)&0x1)
	    {
#ifndef _WIN32
			if((slotIndex>=0)&&(slotIndex<=4))
			{
		        mhcap_vme_DoReadProc1(slotIndex);
		
		        if(gstVmeCtrl[slotIndex].slave_status != TRUE) 
		            gBio.doCard[slotIndex].errCode = 1; // 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
		        else if(gstVmeCtrl[slotIndex].slave_board_type != DO_BOARD_ID) 
		            gBio.doCard[slotIndex].errCode = 2; // 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
		        else if(gstVmeCtrl[slotIndex].stat.slave_init_flag_ok == 0)
		            gBio.doCard[slotIndex].errCode = 3; // 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
		        else
		            gBio.doCard[slotIndex].errCode = 0;
				if((gBio.doCard[slotIndex].errCode==0)&&(gBio.doCard[slotIndex].liveCnt ==  gstVmeCtrl[slotIndex].slave_run_count))
				{
		    		gBio.doCard[slotIndex].errCode = 4;	// 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
		    	    gstVmeCtrl[slotIndex].slave_status = FALSE;
		    	}
		    		
		        gBio.doCard[slotIndex].liveCnt = gstVmeCtrl[slotIndex].slave_run_count;
			}
			else
			{
				gBio.doCard[slotIndex].errCode = 0;
			}
			
			switch(gunHvdcCpuRasInfo.cpuC.cpuType)
			{
			    case HVDC_CPU_C_TYPE:
			        if(     (slotIndex>=0)
			            &&  (slotIndex<MAX_CPU_C_DIO_BOARD_NUM)   )
			        {
		        	    gunHvdcCpuRasInfo.cpuC.cpuType;
		        	    gunHvdcCpuRasInfo.cpuC.dioBoardCfgStat &= ~(0x3<<(slotIndex*2));
		        	    gunHvdcCpuRasInfo.cpuC.dioBoardCfgStat |= (DIO_DO_TYPE<<(slotIndex*2)); 
		        	    switch(gBio.doCard[slotIndex].errCode)  /*(입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)*/
		        	    {
		        	        case 0: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 0<<0; break; 
		        	        case 1: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 1<<0; break;
		        	        case 2: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 1<<2; break;
		        	        case 3: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 1<<1; break;
		        	        case 4: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 1<<1; break;
		        	    }
		            }
			        break;
			    case HVDC_CPU_P_TYPE:
			        if(     (slotIndex>=0)
			            &&  (slotIndex<MAX_CPU_P_DIO_BOARD_NUM)   )
			        {
		        	    gunHvdcCpuRasInfo.cpuP.cpuType;
		        	    gunHvdcCpuRasInfo.cpuP.dioBoardCfgStat &= ~(0x3<<(slotIndex*2));
		        	    gunHvdcCpuRasInfo.cpuP.dioBoardCfgStat |= (DIO_DO_TYPE<<(slotIndex*2));
		        	    switch(gBio.doCard[slotIndex].errCode)  /*(입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)*/
		        	    {
		        	        case 0: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 0<<0; break; 
		        	        case 1: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 1<<0; break;
		        	        case 2: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 1<<2; break;
		        	        case 3: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 1<<1; break;
		        	        case 4: gunHvdcCpuRasInfo.cpuC.dioBoardErrStat[slotIndex] = 1<<1; break;
		        	    }
		            }
			        break;
			    case HVDC_CPU_V_TYPE:
			        gunHvdcCpuRasInfo.cpuV.cpuType;
			        break;
			    case HVDC_CPU_M_TYPE:
		    	    gunHvdcCpuRasInfo.cpuM.cpuType;
			        break;
			    default:
			        break;
			}
			
#endif
	    }
	    else if(!(sysCnfgInfo.hvdcDiConfig>>slotIndex)&0x1)
	    {
			gunHvdcCpuRasInfo.cpuC.dioBoardCfgStat &= ~(0x3<<(slotIndex*2));
	    }
    }
    /* 알고리즘 연산부 끝 */
}

void hvdcCcbBaseIoCheck(void)
{
	uint32	slotIndex = 0;
	strSysConfigInfo	sysCnfgInfo;

    memoryClear ( (uint8 *) &sysCnfgInfo, sizeof(strSysConfigInfo));
    (void)systemCnfgInfoRead( &sysCnfgInfo);
    
    slotIndex = 2;

    if(sysCnfgInfo.hvdcCcbConfig == 0x1)
    {
#ifndef _WIN32
		if((slotIndex>=0)&&(slotIndex<=4))
		{
	        mhcap_vme_AioProc1(slotIndex);
	
	        if(gstVmeCtrl[slotIndex].slave_status != TRUE) 
	        {
	            gBio.ccb.errCode = 1; // 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
		        setErrorCodeWithVal( 	__FILE__,__LINE__,__FUNCTION__,1/*status*/,
										"slotIndex",slotIndex,
										"Not Using",0,
										"Not Using",0 
							  		);
	        }
	        else if(gstVmeCtrl[slotIndex].slave_board_type != AIO_BOARD_ID) 
	        {
	            gBio.ccb.errCode = 2; // 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
	        }
	        else if(gstVmeCtrl[slotIndex].stat.slave_init_flag_ok == 0)
	            gBio.ccb.errCode = 3; // 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
	        else
	            gBio.ccb.errCode = 0;
			if((gBio.ccb.errCode==0)&&(gBio.ccb.liveCnt ==  gstVmeCtrl[slotIndex].slave_run_count))
			{
	    		gBio.ccb.errCode = 4;	// 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)		
	    	    gstVmeCtrl[slotIndex].slave_status = FALSE;
	    	}
	    		
	        gBio.ccb.liveCnt = gstVmeCtrl[slotIndex].slave_run_count;
		}
		else
		{
			gBio.ccb.errCode = 0;
		}
		
		switch(gunHvdcCpuRasInfo.cpuC.cpuType)
		{
		    case HVDC_CPU_C_TYPE:
		        if(     (slotIndex>=0)
		            &&  (slotIndex<MAX_CPU_C_DIO_BOARD_NUM)   )
		        {
	        	    gunHvdcCpuRasInfo.cpuC.cpuType;
	        	    gunHvdcCpuRasInfo.cpuC.aioBoardCfgStat &= ~(0x1);
	        	    gunHvdcCpuRasInfo.cpuC.aioBoardCfgStat |= (0x1);
	
	    		    if((gBio.ccb.errCode == 1)||(gBio.ccb.errCode == 2))
	    		        gunHvdcCpuRasInfo.cpuC.aioBoardErrStat = 0x1|(gunHvdcCpuRasInfo.cpuC.aioBoardErrStat&0xFFFC);
	    		    else if((gBio.ccb.errCode == 0)&&((gunHvdcCpuRasInfo.cpuC.aioBoardErrStat&0xFFFC) != 0))
	    		        gunHvdcCpuRasInfo.cpuC.aioBoardErrStat &= 0xFFFC;
	    		    else if(gBio.ccb.errCode == 0)
	        		    gunHvdcCpuRasInfo.cpuC.aioBoardErrStat = 0;
	    		    else /*if((gBio.ccb.errCode == 3)||(gBio.ccb.errCode == 4))*/ 
	    		        gunHvdcCpuRasInfo.cpuC.aioBoardErrStat = 0x2|(gunHvdcCpuRasInfo.cpuC.aioBoardErrStat&0xFFFC);
	            }
		        break;
		    case HVDC_CPU_P_TYPE:
		        break;
		    case HVDC_CPU_V_TYPE:
		        gunHvdcCpuRasInfo.cpuV.cpuType;
		        break;
		    case HVDC_CPU_M_TYPE:
	    	    gunHvdcCpuRasInfo.cpuM.cpuType;
		        break;
		    default:
		        break;
		}
		
		gunHvdcCpuRasInfo.cpuC.aioBoardErrStat = ((gstVmeCtrl[slotIndex].vme_comm_stat&0xFFF)<<2)
		                                        |(gunHvdcCpuRasInfo.cpuC.aioBoardErrStat&0x3);
		
#endif
    }
    else 
    {
    	gunHvdcCpuRasInfo.cpuC.aioBoardCfgStat &= ~(0x1);
    }

	return;
}

void hvdcVcbBaseIoCheck(void)
{
	uint32	status = NO_ERROR;
	
	uint32	slotIndex = 0;
	strSysConfigInfo	sysCnfgInfo;

    memoryClear ( (uint8 *) &sysCnfgInfo, sizeof(strSysConfigInfo));
    
    (void)systemCnfgInfoRead( &sysCnfgInfo);
    
    for(slotIndex = 0;slotIndex < 6; slotIndex++)
    {
	    if(((sysCnfgInfo.hvdcVcbConfig>>slotIndex)&0x1) == 0x1)
	    {
#ifndef _WIN32

			/****************************************************************** 
			** 보드 자체의 진단 정보 Set : 광통신 상태, 보드 정상동작 여부 등
			*******************************************************************/
	        mhcap_vme_AioProc1(slotIndex);
	
	        if(gstVmeCtrl[slotIndex].slave_status != TRUE) 
	            gBio.vcb[slotIndex].errCode = 1; // (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
	        else if(gstVmeCtrl[slotIndex].slave_board_type != AIO_BOARD_ID) 
	            gBio.vcb[slotIndex].errCode = 2; // (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
	        else if(gstVmeCtrl[slotIndex].stat.slave_init_flag_ok == 0)
	            gBio.vcb[slotIndex].errCode = 3; // (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
	        else
	            gBio.vcb[slotIndex].errCode = 0;
	            
			if((gBio.vcb[slotIndex].errCode==0)&&(gBio.vcb[slotIndex].liveCnt == gstVmeCtrl[slotIndex].slave_run_count))
			{
	    		gBio.vcb[slotIndex].errCode = 4;	// (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)	
	    		gstVmeCtrl[slotIndex].slave_status = FALSE;
	    	}	
	    		
	        gBio.vcb[slotIndex].liveCnt = gstVmeCtrl[slotIndex].slave_run_count;

		
    	    gunHvdcCpuRasInfo.cpuV.aioBoardCfgStat &= ~(0x1<<slotIndex);
    	    gunHvdcCpuRasInfo.cpuV.aioBoardCfgStat |= (0x1<<slotIndex);

		    if((gBio.vcb[slotIndex].errCode == 1)||(gBio.vcb[slotIndex].errCode == 2))
		        gunHvdcCpuRasInfo.cpuV.aioBoardErrStat[slotIndex] = 0x1|(gunHvdcCpuRasInfo.cpuV.aioBoardErrStat[slotIndex]&0xFFFC);

		    else if((gBio.vcb[slotIndex].errCode == 0)&&((gunHvdcCpuRasInfo.cpuV.aioBoardErrStat[slotIndex]&0xFFFC) != 0))
		        gunHvdcCpuRasInfo.cpuV.aioBoardErrStat[slotIndex] &= 0xFFFC;

		    else if(gBio.vcb[slotIndex].errCode == 0)
		        gunHvdcCpuRasInfo.cpuV.aioBoardErrStat[slotIndex] = 0;

		    else /*if((gBio.vcb[slotIndex].errCode == 3)||(gBio.vcb[slotIndex].errCode == 4))*/ 
		        gunHvdcCpuRasInfo.cpuV.aioBoardErrStat[slotIndex] = 0x2|(gunHvdcCpuRasInfo.cpuV.aioBoardErrStat[slotIndex]&0xFFFC);

			/*
			gunHvdcCpuRasInfo.cpuV.aioBoardErrStat[slotIndex] = (((gstVmeCtrl[slotIndex].vme_comm_stat>>9)&0x7)<<8)|
																((gstVmeCtrl[slotIndex].vme_comm_stat&0x3F)<<2)|
																(gunHvdcCpuRasInfo.cpuV.aioBoardErrStat[slotIndex]&0x3);
			*/
			gunHvdcCpuRasInfo.cpuV.aioBoardErrStat[slotIndex] = ((gstVmeCtrl[slotIndex].vme_comm_stat&0x3FFF)<<2)
			                                                    |(gunHvdcCpuRasInfo.cpuV.aioBoardErrStat[slotIndex]&0x3);
		
			/****************************************************************** 
			** [V105] RTDS에서 통신한 서버 모듈의의 CAP. 전압 및 상태를 M영역에 Write
			*******************************************************************/
			status = subModuleInfoReadAndWriteToMregion(slotIndex);
			if(status != NO_ERROR)
		        setErrorCodeWithVal(__FILE__,__LINE__,__FUNCTION__,status,
									"slotIndex",slotIndex,
									"Not Using",0,
									"Not Using",0 
							  	);

#endif
	    }
	    else
	    {
			gunHvdcCpuRasInfo.cpuV.aioBoardCfgStat &= ~(0x1<<slotIndex);
	    }
    }
    
    return;
}

static uint32 subModuleInfoReadAndWriteToMregion(uint32 slotIndex)
{
	uint32	status = NO_ERROR;
	strMemPointerInfo	memInfo;
	
	uint8	*vmeDataPtr;
	uint32	*mRegionPtr;
	float	readSMCapVltgData[MAX_SUB_MODULE_NUM_PER_ARM * 2]; 	/* upper and lower ARM */
	uint32	readSMStatusData[MAX_SUB_MODULE_NUM_PER_ARM]; 		/* upper and lower ARM */

#if 1 /* 시험 용도 */
	uint32 	loop = 0;
	uint32	tmpData = 0;
	
	/******************************************************************
	**	1. SM Voltage Write
	******************************************************************/
	memoryClear(	(uint8 *)readSMCapVltgData,
					MAX_SUB_MODULE_NUM_PER_ARM * 2 * 4
				);

	(void)fbMemPointerGet(LOGIC_ID, &memInfo);

	mRegionPtr  = (uint32 *)memInfo.flagMemPtr;
	mRegionPtr += M_REGION_SM_CAP_VLTG_START_INDEX;
	
	/* A, B, C상 차례로 Upper, Lower ARM의 CAP. 전압이 위치 함 */
	mRegionPtr += (MAX_SUB_MODULE_NUM_PER_ARM * 2 * slotIndex);
	
	for(loop = 0; loop < (MAX_SUB_MODULE_NUM_PER_ARM * 2); loop++, mRegionPtr++)
	{
		readSMCapVltgData[loop] = (float)((1000 * slotIndex) + loop);	
		
		memoryCopy ( 	(uint8 *) mRegionPtr,
						(uint8 *) &readSMCapVltgData[loop],
						4
					);
	}

	/******************************************************************
	**	2. SM Status Write
	******************************************************************/
	memoryClear(	(uint8 *)readSMStatusData,
					MAX_SUB_MODULE_NUM_PER_ARM  * 4
				);
	
	(void)fbMemPointerGet(LOGIC_ID, &memInfo);

	mRegionPtr  = (uint32 *)memInfo.flagMemPtr;
	mRegionPtr += M_REGION_SM_STATUS_START_INDEX;
	
	/* A, B, C상 차례로 Upper, Lower ARM의 Status(2bytes/SM) 위치 함 */
	mRegionPtr += (MAX_SUB_MODULE_NUM_PER_ARM * slotIndex);
	
	for(loop = 0; loop < MAX_SUB_MODULE_NUM_PER_ARM; loop++, mRegionPtr++)
	{
		readSMStatusData[loop]  = (uint32)((slotIndex << 12) | tmpData++);	
		readSMStatusData[loop] |= (uint32)((slotIndex << 28) | ((tmpData++)<<16));	
		
		memoryCopy ( 	(uint8 *) mRegionPtr,
						(uint8 *) &readSMStatusData[loop],
						4
					);
	}
	
#endif	


#if 0

	/******************************************************************
	**	0. 해당 보드가 정상일 경우만 Access 한다
	******************************************************************/
	if(gBio.vcb[slotIndex].errCode != NO_ERROR)
	{
		status = DSP_BOARD_READY_ERR;
        setErrorCodeWithVal(__FILE__,__LINE__,__FUNCTION__,status,
							"slotIndex",slotIndex,
							"Not Using",0,
							"Not Using",0 
					  	);
		
		return (status);
	}
		
	/******************************************************************
	**	1. Read upper/lower Sub Module Capacitor Voltage from DSP 보드
	******************************************************************/
	memoryClear(	(uint8 *)readSMCapVltgData,
					MAX_SUB_MODULE_NUM_PER_ARM * 2 * 4
				);
				 
	/* Slot Index 는 '0' 부터 시작 */
	vmeDataPtr  = (uint8 *)DSP_BOARD_START_VME_ADDR;
	vmeDataPtr += (DSP_BOARD_VME_ADDR_OFFSET * slotIndex);
	vmeDataPtr += SUB_MODULE_CAP_VLTG_START_INDEX;
	
	/* VME Read */
	memoryCopy( (uint8 *) readSMCapVltgData,
				(uint8 *) vmeDataPtr,
				MAX_SUB_MODULE_NUM_PER_ARM * 2 * 4
			  )
				 		
	/******************************************************************
	**	2. Write Sub Module Capacitor Voltage to M region
	**
	**	[주의 사항] M-RAM(즉, NVRAM)에 Write할 필요가 있는지 검토
	**			 현재는 Local RAM에만 Write 함
	******************************************************************/
	(void)fbMemPointerGet(LOGIC_ID, &memInfo);

	mRegionPtr  = (uint32 *)memInfo.flagMemPtr;
	mRegionPtr += M_REGION_SM_CAP_VLTG_START_INDEX;
	
	/* A, B, C상 차례로 Upper, Lower ARM의 CAP. 전압이 위치 함 */
	mRegionPtr += (MAX_SUB_MODULE_NUM_PER_ARM * 2 * slotIndex);
	
	/* Swapping 함, 4바이트 단위로 */
	longDataConversion(	mRegionPtr, 
						readSMCapVltgData,
						MAX_SUB_MODULE_NUM_PER_ARM * 2
					  );
	
	/******************************************************************
	**	3. Read Sub module Status : 2byte/Submodule
	******************************************************************/
	memoryClear(	(uint8 *)readSMStatusData,
					MAX_SUB_MODULE_NUM_PER_ARM  * 4
				);
				 
	/* Slot Index 는 '0' 부터 시작 */
	vmeDataPtr = (uint8 *)DSP_BOARD_START_VME_ADDR;
	vmeDataPtr += (DSP_BOARD_VME_ADDR_OFFSET * slotIndex);
	vmeDataPtr += SUB_MODULE_STATUS_START_INDEX;
	
	memoryCopy( (uint8 *) readSMStatusData,
				(uint8 *) vmeDataPtr,
				MAX_SUB_MODULE_NUM_PER_ARM * 4
			  )
	
	/******************************************************************
	**	4. Write Sub Module Status to M region
	**
	**	[주의 사항] M-RAM(즉, NVRAM)에 Write할 필요가 있는지 검토
	**			 현재는 Local RAM에만 Write 함
	******************************************************************/
	(void)fbMemPointerGet(LOGIC_ID, &memInfo);

	mRegionPtr  = (uint32 *)memInfo.flagMemPtr;
	mRegionPtr += M_REGION_SM_STATUS_START_INDEX;
	
	/* A, B, C상 차례로 Upper, Lower ARM의 Status(2bytes/SM) 위치 함 */
	mRegionPtr += (MAX_SUB_MODULE_NUM_PER_ARM * slotIndex);
	
	/* Swapping 함, 4바이트 단위로 */
	shortDataConversion((uint16 *)mRegionPtr, 
						(uint16 *)readSMStatusData,
						MAX_SUB_MODULE_NUM_PER_ARM * 2
					   );
#endif
	 
	return(status);
}	

void hvdcAiCardBaseIoCheck(void)
{
	uint32	slotIndex = 0;
	strSysConfigInfo	sysCnfgInfo;

    memoryClear ( (uint8 *) &sysCnfgInfo, sizeof(strSysConfigInfo));
    (void)systemCnfgInfoRead( &sysCnfgInfo);
    
	/* 알고리즘 연산부 시작 */
    for(slotIndex=0;slotIndex<6;slotIndex++)
    {
	    if(((sysCnfgInfo.hvdcAiConfig>>slotIndex)&0x1)==0x1)
	    {
#ifndef _WIN32
			if((slotIndex>=0)&&(slotIndex<=5))
			{
		        mhcap_vme_AioProc1(slotIndex);
		
		        if(gstVmeCtrl[slotIndex].slave_status != TRUE) 
		        {
		            gBio.aiCard[slotIndex].errCode = 1; // 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
			        setErrorCodeWithVal( 	__FILE__,__LINE__,__FUNCTION__,IO_BOARD_ACCESS_ERR,
									"slotIndex",slotIndex+1,
									"Not Using",0,
									"Not Using",0 
						  		);
		        }
		        else if(gstVmeCtrl[slotIndex].slave_board_type != AIO_BOARD_ID) 
		        {
		            gBio.aiCard[slotIndex].errCode = 2; // 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
			        setErrorCodeWithVal( 	__FILE__,__LINE__,__FUNCTION__,IO_BOARD_MISMATCH_ERR,
											"slotIndex",slotIndex+1,
											"slave_board_type",gstVmeCtrl[slotIndex].slave_board_type,
											"Not Using",0 
								  		);
		        }
		        else if(gstVmeCtrl[slotIndex].stat.slave_init_flag_ok == 0)
		        {
		            gBio.aiCard[slotIndex].errCode = 3; // 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
			        setErrorCodeWithVal( 	__FILE__,__LINE__,__FUNCTION__,IO_BOARD_READY_ERR,
											"slotIndex",slotIndex+1,
											"Not Using",0,
											"Not Using",0 
								  		);
		        }
		        else
		            gBio.aiCard[slotIndex].errCode = 0;
		            
				if((gBio.aiCard[slotIndex].errCode == NO_ERROR)&&(gBio.aiCard[slotIndex].liveCnt == gstVmeCtrl[slotIndex].slave_run_count))
				{
		    		gBio.aiCard[slotIndex].errCode = 4;	// 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)		
			        setErrorCodeWithVal( 	__FILE__,__LINE__,__FUNCTION__,IO_BOARD_LIVE_COUNT_ERR,
											"slotIndex",slotIndex+1,
											"Not Using",0,
											"Not Using",0 
								  		);
					gstVmeCtrl[slotIndex].slave_status = FALSE;
		        }
		    		
		        gBio.aiCard[slotIndex].liveCnt = gstVmeCtrl[slotIndex].slave_run_count;
			}
			else
			{
				gBio.aiCard[slotIndex].errCode = 0;
			}
			
			switch(gunHvdcCpuRasInfo.cpuM.cpuType)
			{
			    case HVDC_CPU_C_TYPE:
			        break;
			    case HVDC_CPU_P_TYPE:
			        break;
			    case HVDC_CPU_V_TYPE:
			        break;
			    case HVDC_CPU_M_TYPE:
			        if((slotIndex >= 0) && (slotIndex < MAX_CPU_M_AIO_BOARD_NUM))
			        {
		        	    gunHvdcCpuRasInfo.cpuM.aioBoardCfgStat &= ~(0x1<<slotIndex);
		        	    gunHvdcCpuRasInfo.cpuM.aioBoardCfgStat |= (0x1<<slotIndex);
		
		    		    if((gBio.aiCard[slotIndex].errCode == 1)||(gBio.aiCard[slotIndex].errCode == 2))
		    		    {
		    		        gunHvdcCpuRasInfo.cpuM.aioBoardErrStat[slotIndex] = 0x1|(gunHvdcCpuRasInfo.cpuM.aioBoardErrStat[slotIndex]&0xFFFC);
		    		        
					        setErrorCodeWithVal( 	__FILE__,__LINE__,__FUNCTION__,IO_BOARD_ACCESS_ERR,
					        						"slotIndex",slotIndex+1,
					        						"Not Using",0,
					        						"Not Using",0 
										  		);
		    		    }
		    		    else if((gBio.aiCard[slotIndex].errCode == 3)||(gBio.aiCard[slotIndex].errCode == 4))
		    		    { 
		    		        gunHvdcCpuRasInfo.cpuM.aioBoardErrStat[slotIndex] = 0x2|(gunHvdcCpuRasInfo.cpuM.aioBoardErrStat[slotIndex]&0xFFFC);
					     
					        setErrorCodeWithVal( 	__FILE__,__LINE__,__FUNCTION__,IO_BOARD_LIVE_COUNT_ERR,
					        						"slotIndex",slotIndex+1,
					        						"Not Using",0,
					        						"Not Using",0 
										  		);
		    		    }
		    		    
		    		    else if(gBio.aiCard[slotIndex].errCode == 0)
		    		    {
		    		    	if((gunHvdcCpuRasInfo.cpuM.aioBoardErrStat[slotIndex]&0xFFFC) != 0)
		    		        	gunHvdcCpuRasInfo.cpuM.aioBoardErrStat[slotIndex] &= 0xFFFC;
		    		        
		    		    	else 
		    		        	gunHvdcCpuRasInfo.cpuM.aioBoardErrStat[slotIndex] = 0;
						}
		            }
			        break;
			    default:
			        break;
			}
			
			gunHvdcCpuRasInfo.cpuM.aioBoardErrStat[slotIndex] = ((gstVmeCtrl[slotIndex].vme_comm_stat&0x3FFF)<<2)
			                                                    |(gunHvdcCpuRasInfo.cpuM.aioBoardErrStat[slotIndex]&0x3);			
#endif
	    }
	    else
	    {
			gunHvdcCpuRasInfo.cpuM.aioBoardCfgStat &= ~(0x1<<slotIndex);
	    }
    }
	
	return;
}

static STATUS mhcap_vme_DoReadProc1 (int slotId)
{
	vuchar	data;
	uint	vme_user_addr, size;
	uint	peer, tick = 0;

	vme_user_addr = gstVmeCtrl[slotId].vme_addr + sizeof (VME_HEADER);
	size = gstVmeCtrl[slotId].size;

	gstVmeCtrl[slotId].vme_data = (VME_DATA *)gstVmeCtrl[slotId].vme_addr;

	mhcap_checkVmeSlaveStatus (slotId, TRUE);


	while (vme_start_flag)
	{
		mhcap_setVmeMasterStatus (slotId, TRUE);
		
		if (mhcap_checkVmeSlaveStatus (slotId, FALSE) == OK) 
		{
		    /* 보드가 있을 경우 */
		    break;
		}

		if ((tick == 0) || (duration_msec (tick) >= 3000)) 
		{
			tick = tickGet ();
		}

		break;
	}
	
	data = *(vuchar *)vme_user_addr;
	gstVmeCtrl[slotId].vme_rd_data = data;
	gstVmeData[slotId].iodata.do_data[0] = data;

	/*gstVmeCtrl[slotId].vme_wr_data = 0;*/ /* 최초 초기화시 클리어 필요*/


	gstVmeCtrl[slotId].vme_run_tick = tickGet ();

#if 0
	필요 없는것으로 판단 : 검토 필요
	mhcap_vme_DoDataReadProc1 (slotId, size);
#endif
	return OK;
}

static STATUS mhcap_vme_DiProc1 (int slotId)
{
	vuchar	data;
	uint	vme_user_addr, size;
	uint	peer, tick = 0;

	vme_user_addr = gstVmeCtrl[slotId].vme_addr + sizeof (VME_HEADER);
	size = gstVmeCtrl[slotId].size;

	gstVmeCtrl[slotId].vme_data = (VME_DATA *)gstVmeCtrl[slotId].vme_addr;

	mhcap_checkVmeSlaveStatus (slotId, TRUE);

	while (vme_start_flag) 
	{
		mhcap_setVmeMasterStatus (slotId, TRUE);
	
		if (mhcap_checkVmeSlaveStatus (slotId, FALSE) == OK)		
			break;
			
		if ((tick == 0) || (duration_msec (tick) >= 3000)) 
		{
			tick = tickGet ();
		}

		break;
	}
	
#if 0
	data = *(vuchar *)vme_user_addr;    /* 하위 1바이트 입력 데이터 스캔 */
	gstVmeCtrl[slotId].vme_rd_data = data;
	gstVmeData[slotId].iodata.di_data[0] = data; /* 2020.06.06 */
#endif 

	gstVmeCtrl[slotId].vme_run_tick = tickGet ();
	
#if 0
	필요없는것으로 판단됨
	mhcap_vme_DiDataProc1 (slotId, size);
#endif
	
	return (NO_ERROR);
}

STATUS mhcap_vme_AioProc1 (int32 slotId)
{
	uint16	data;
	uint32	vme_user_addr, size;
	uint32	tick = 0;

	vme_user_addr 	= gstVmeCtrl[slotId].vme_addr + sizeof (VME_HEADER);
	size 			= gstVmeCtrl[slotId].size;
	
	gstVmeCtrl[slotId].vme_data = (VME_DATA *)gstVmeCtrl[slotId].vme_addr;

	if(mhcap_checkVmeSlaveStatus (slotId, TRUE) == OK)
	{
        gstVmeCtrl[slotId].vme_comm_stat = 	(*(uint8*)(gstVmeCtrl[slotId].vme_addr + 0x840))<<24;
        gstVmeCtrl[slotId].vme_comm_stat |= (*(uint8*)(gstVmeCtrl[slotId].vme_addr + 0x841))<<16;
        gstVmeCtrl[slotId].vme_comm_stat |= (*(uint8*)(gstVmeCtrl[slotId].vme_addr + 0x842))<<8;
        gstVmeCtrl[slotId].vme_comm_stat |= (*(uint8*)(gstVmeCtrl[slotId].vme_addr + 0x843))<<0;
        /*
        if(slotId == 0) {
            printf("[4]vme_comm_stat=%x\n",gstVmeCtrl[slotId].vme_comm_stat);
            printf("vme_comm_stat_addr=0x%08x\n", (uint32)(gstVmeCtrl[slotId].vme_addr + 0x840));
        }*/
    }

	while (vme_start_flag)
	{
		mhcap_setVmeMasterStatus (slotId, TRUE);
		
		if (mhcap_checkVmeSlaveStatus (slotId, FALSE) == OK)		
			break;
			
		if ((tick == 0) || (duration_msec (tick) >= 3000)) 
		{
			tick = tickGet ();
		}
		break;
	}

#if 0
	필요 없는 것으로 예상 됨 : 검토 필요
	data = *(vuchar *)vme_user_addr;
	gstVmeCtrl[slotId].vme_rd_data = data;
	gstVmeData[slotId].iodata.aio_data[0] = data;
#endif

	gstVmeCtrl[slotId].vme_run_tick = tickGet ();

#if 0
	필요 없는 것으로 예상 됨 : 검토 필요
    mhcap_vme_AioDataProc1 (slotId, size);
#endif	

	return OK;
}
