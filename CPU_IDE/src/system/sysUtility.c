
#include	"usrTypes.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include	"usrDefBuf.h"
#include	"sysConfig.h"
#include	"lkv120Board.h"
#include	"systemDiag.h"
#include	"usrErrCode.h"


strSysConfigInfo	gSysCnfgInfo;


extern uint32 gNetLineCnfg;


extern void systemRasInfoSet(uint32 , uint32 );
extern void memoryClear ( uint8 *, uint32 );
extern void memoryCopy ( uint8 *, uint8 *, uint32 );
extern void ipAddrStrToHex (uint32 *, uint8 *);
extern uint32 schedulerTimeBaseSet();
extern void		setErrorCode(uint8 *,uint32,uint8 *,uint32);


uint32	systemCnfgInfoRead( strSysConfigInfo *sysCnfgInfo)
{
	uint32	status = NO_ERROR;

    memoryCopy ( (uint8 *) sysCnfgInfo,
    			 (uint8 *) &gSysCnfgInfo,
				 sizeof(strSysConfigInfo)
				);		
    return(status);
}



uint32 systemCnfgInfoSet	(
						strSystemBootParaInfo 	*bootParaPtr 
							)
{
	uint32	status = NO_ERROR;
    uint32 hexData;
    uint32 rasData = 0;

	strNetBootParaInfo	*netParaPtr;
    strUserBootParaInfo	*usrParaPtr;
	strSysConfigInfo	sysCnfgInfo;

    netParaPtr = &bootParaPtr->netBootParaInfo;
    usrParaPtr = &bootParaPtr->usrBootParaInfo;

    hexData = 0;
	
    memoryClear ( (uint8 *) &sysCnfgInfo,
    				sizeof(strSysConfigInfo)
				);

    
    sysCnfgInfo.shelfType = usrParaPtr->shelfType;

    
    sysCnfgInfo.mpuEnable = usrParaPtr->mpuEnable;

    sysCnfgInfo.dluIfKind = usrParaPtr->dluIfKind;
    
    sysCnfgInfo.mbSlaveMapIf = usrParaPtr->mbSlaveMapIf;
    
    sysCnfgInfo.IOIfBoardKind = usrParaPtr->ioShelfType;
    
    
    sysCnfgInfo.memoryBoardType = usrParaPtr->memoryBoardType; 
   
	
    sysCnfgInfo.tmtcComBrdType = usrParaPtr->tmtcComBrdType; 

       
	sysCnfgInfo.sysSchedlueType  = usrParaPtr->sysSchedlueType;

	
	
    sysCnfgInfo.hvdcCpuType = usrParaPtr->hvdcCpuType; 
    sysCnfgInfo.hvdcDiConfig = usrParaPtr->hvdcDiConfig;
    sysCnfgInfo.hvdcDoConfig = usrParaPtr->hvdcDoConfig;
    sysCnfgInfo.hvdcCcbConfig = usrParaPtr->hvdcCcbConfig;
    sysCnfgInfo.hvdcVcbConfig = usrParaPtr->hvdcVcbConfig;
    sysCnfgInfo.hvdcAiConfig = usrParaPtr->hvdcAiConfig;
    sysCnfgInfo.hvdcOdmConfig = usrParaPtr->hvdcOdmConfig;
    
	sysCnfgInfo.routeIndex = usrParaPtr->routeIndex;
	memoryCopy ( (uint8 *)&sysCnfgInfo.routeTbl,(uint8*)usrParaPtr->routeTbl,
    			 sizeof(strRouteTblInfo)*MAX_ROUTE_NUM
				);
 
   
	ipAddrStrToHex(&hexData, netParaPtr->netLineAIpAddr);

	sysCnfgInfo.netCnfgInfo.myIpAddr[0] = hexData;

    
    ipAddrStrToHex(&hexData, usrParaPtr->netLineBIpAddr);
    sysCnfgInfo.netCnfgInfo.myIpAddr[1] = hexData;

	
    
    sysCnfgInfo.netCnfgInfo.ctrlNetDualStatus = gNetLineCnfg;

    sysCnfgInfo.stationId = usrParaPtr->stationNum;

    
    sysCnfgInfo.ipMakingFlag = usrParaPtr->ipMakingFlag; 
	
	
    memoryCopy ( (uint8 *) &gSysCnfgInfo,
    			 (uint8 *) &sysCnfgInfo,
				 sizeof(strSysConfigInfo)
				);

    
	if(sysCnfgInfo.shelfType == MASTER_SHELF)
		rasData = SYS_RAS_MASTER_SHELF_ID;
	else if(sysCnfgInfo.shelfType == SLAVE_SHELF)
		rasData = SYS_RAS_SLAVE_SHELF_ID;
  
	else rasData = 0;

    systemRasInfoSet(RAS_SHELF_ID, rasData);

	
	status = schedulerTimeBaseSet();
	
	return(status);
}


void sysRmtIpAddrSetToSysConfigMemory(uint32 ipAddr_A, uint32 ipAddr_B)
{
	gSysCnfgInfo.netCnfgInfo.rmtIpAddr[0] = ipAddr_A;
	gSysCnfgInfo.netCnfgInfo.rmtIpAddr[1] = ipAddr_B;
}


void systemCnfgDebugInfoSet	(
						strSysConfigInfo	*dataPtr,
						strSystemBootParaInfo 	*bootParaPtr 
						)
{

    uint32 hexData;
	strNetBootParaInfo	*netParaPtr;
    strUserBootParaInfo	*usrParaPtr;

    netParaPtr = &bootParaPtr->netBootParaInfo;
    usrParaPtr = &bootParaPtr->usrBootParaInfo;

    hexData = 0;

    
    dataPtr->shelfType = usrParaPtr->shelfType;

    
    dataPtr->mpuEnable = usrParaPtr->mpuEnable;

    dataPtr->dluIfKind = usrParaPtr->dluIfKind;
    
    dataPtr->mbSlaveMapIf = usrParaPtr->mbSlaveMapIf;
    
    dataPtr->IOIfBoardKind = usrParaPtr->ioShelfType;
    
    
    ipAddrStrToHex(&hexData, netParaPtr->netLineAIpAddr);
    dataPtr->netCnfgInfo.myIpAddr[0] = hexData;

    
    ipAddrStrToHex(&hexData, usrParaPtr->netLineBIpAddr);
    dataPtr->netCnfgInfo.myIpAddr[1] = hexData;
	
	
    
    ipAddrStrToHex(&hexData, usrParaPtr->rmtTargetIpAddr);
    dataPtr->netCnfgInfo.rmtIpAddr[0] = hexData;

    
    ipAddrStrToHex(&hexData, usrParaPtr->rmtModuleIpAddr);
    dataPtr->netCnfgInfo.rmtIpAddr[1] = hexData;
	
	
    
    dataPtr->netCnfgInfo.ctrlNetDualStatus = gNetLineCnfg;

    dataPtr->stationId = usrParaPtr->stationNum;

    
    dataPtr->ipMakingFlag = usrParaPtr->ipMakingFlag; 
    
    
    dataPtr->memoryBoardType = usrParaPtr->memoryBoardType; 
	
    
	dataPtr->sysSchedlueType  = usrParaPtr->sysSchedlueType;
	
    
    dataPtr->tmtcComBrdType = usrParaPtr->tmtcComBrdType; 
	
	
    dataPtr->hvdcCpuType = usrParaPtr->hvdcCpuType; 
    dataPtr->hvdcDiConfig = usrParaPtr->hvdcDiConfig;
    dataPtr->hvdcDoConfig = usrParaPtr->hvdcDoConfig;
    dataPtr->hvdcCcbConfig = usrParaPtr->hvdcCcbConfig;
    dataPtr->hvdcVcbConfig = usrParaPtr->hvdcVcbConfig;
    dataPtr->hvdcAiConfig = usrParaPtr->hvdcAiConfig;
    dataPtr->hvdcOdmConfig = usrParaPtr->hvdcOdmConfig;

	return;
}



