
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"sysConfig.h"
#include	"pcsDefs.h"
#include	"network.h"
#include	"lkv120Board.h"
#include	"usrErrCode.h"
#include	"osDepend.h"
#include	"sysConfig.h"
#include	"systemDiag.h"

#include	"usrQueData.h"
#include	"msgQueData.h"

#include	"debugInfo.h"

extern int32	gDebugPortFd;
extern int8 *verDate;

extern uint32 gEthDluModuleID;
extern uint8 *gBusMpuFbStartAddr;

void bootParaSequence (strSystemBootParaInfo *);
uint32 bootParaCheck(strSystemBootParaInfo *);
int32 bootConfigWait(int32 );
void bootParaConfig(void);
void routeTblInfoAllDelete(uint32, uint32);
static void bootParaChange(uint32, uint32);
void usgMsg(int8 **, int8 **, int32 );
void sysBootParameterUpdate(int8 *);
void bootParaDisplay(uint32, uint32	);
static void bootCmdHelp(uint32, uint32	);
static void ipAddressDisplay(uint32, uint32);
void bootLogoPrint(strSystemBootParaInfo *);


extern uint32	nvRamRead (int8 *, int32 , int32);
extern uint32	nvRamWrite (int8 *, int32 , int32);
extern void		memoryClear (uint8 *, uint32);
extern void		memoryCopy (uint8 *, uint8 *, uint32);
extern int32    keyHit(void);
extern void		systemInitErr (uint8 *,uint32,uint8 *,uint32);
extern void		errorStringDisplay (uint8 *,uint32,uint8 *,uint32);
extern int32	ioSysCtrlFunc (int32 , int32 , int32);
extern int32	systemTickGet (void);
extern int32	systemClockRateGet (void);
extern void		spaceSkip (int8 **);
extern void		boardReboot (void);
extern uint32	sysSubNetMaskAddrGet (int8 *, int32 *);
extern int32	ioRead (int32 , int8 *, int32);
extern int32	ioSysCtrlFuncW();
extern uint32	ipAddrMake (uint8 *, uint32, uint32, uint32);
extern uint32	ipAddrValidCheck (uint8 *);
extern void		ipAddrHexToString (int8 *, uint32);
extern void		ipAddrStrToHex (uint32 *, uint8 *);
extern uint32	ipAddrGet (int8 *, int8 *);
extern void		systemCnfgDebugInfoSet (strSysConfigInfo *,strSystemBootParaInfo *);
extern uint32	systemCnfgInfoRead (strSysConfigInfo *);
extern void		moduleFbMemoryClear (uint32,uint32);


extern uint32 fbMemoryDef(uint32);
extern uint32 fbMemoryAddrInit(uint32);

extern void memoryBoardParameterDisplay(uint32, uint32);
extern void fbCodeInBusMemDisplay(uint32, uint32);

#ifdef _WIN32
extern SYSTEMTIME asctosystime (const char *dt);
extern char* systoasctime(SYSTEMTIME *st);
extern int _isleap (int months, int year);
static void boardTimeSet(void);
extern uint32 stringCompareWithLen(uint8 *, uint8 *, uint32);
extern uint32 nvRamCreate();
extern uint32  BusMemDataCopyToFileMemory(void);
extern uint32  FileMemoryDataCopyToBusMem(void);
#ifdef _WINCE
extern	int32	_keyhit(int);
extern uint32 getOSVersionInfo(uint32*);
#endif 
#endif

#ifdef VXWORKS

#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD) || (BOARD_TYPE == MVME2500_BOARD) || (BOARD_TYPE == LKV120_BOARD))
extern STATUS	usrBootLineCrack (int8 * , strNetBootParaInfo * pParams);
#endif
#endif

extern void cpuBrdRebootByManual(uint32 , uint32 );

static void fbLogicMemoryMemoryClear(uint32 , uint32 );


void bootParaSequence (strSystemBootParaInfo *bootPara)
{
	uint32	status = NO_ERROR;
    int32	consoleWaitTime;

	
#ifdef _WIN32
   
	if(nvRamCreate()==NO_ERROR)
		printf("NVRAM File Creation!!!");
#endif

    status  = nvRamRead((int8 *)bootPara,
						USR_ENET_CONFIG_PARA_OFFSET, 
						USR_ENET_CONFIG_PARA_SIZE
					   );
	if(status != NO_ERROR)
		systemInitErr(__FILE__, __LINE__, __FUNCTION__, status);

	bootLogoPrint(bootPara);

    printf( "\n");

    
    if(bootPara->usrBootParaInfo.ipMakingFlag == AUTO_IPADDR_MAKE) 
    {
    	printf( "IP Address Making Flag is Auto !!\n");

		
		if( (status = ipAddrMake((uint8 *)bootPara->netBootParaInfo.netLineAIpAddr,
								bootPara->usrBootParaInfo.stationNum,
								NETWORK_A_LINE,
								bootPara->usrBootParaInfo.shelfType
								)
			))
		{
        	printf( "LINE A IP Address Make ERROR !!!\n");
			goto BOOT_PARA_CONFIG_MODE;
		}
        else
        {
        	printf( "LINE A IP Address Make SUCCESS !!!\n");
		}

		
		if( (status = ipAddrMake(bootPara->usrBootParaInfo.netLineBIpAddr,
								bootPara->usrBootParaInfo.stationNum,
								NETWORK_B_LINE,
								bootPara->usrBootParaInfo.shelfType
								)
			)
		){
        	printf( "LINE B IP Address Make ERROR !!!\n");
			goto BOOT_PARA_CONFIG_MODE;
		}
        else
        {
        	printf( "LINE B IP Address Make SUCCESS !!!\n");
		}
		
#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)
		
		if( (status = ipAddrMake(bootPara->usrBootParaInfo.dualLinkIpAddr,
								bootPara->usrBootParaInfo.stationNum,
								DLU_SYS_LINE,
								bootPara->usrBootParaInfo.shelfType
								)
			)
		){
        	printf( "DUAL NET IP Address Make ERROR !!!\n");
			goto BOOT_PARA_CONFIG_MODE;
		}
        else
        {
        	printf( "DUAL NET IP Address Make SUCCESS !!!\n");
		}
#endif

		sysBootParameterUpdate(bootPara->netBootParaInfo.netLineAIpAddr);
    
		
        (void)nvRamWrite(  (int8 *)bootPara,
							USR_ENET_CONFIG_PARA_OFFSET, 
							USR_ENET_CONFIG_PARA_SIZE
					    );
    }
    else if(bootPara->usrBootParaInfo.ipMakingFlag == MANUAL_IPADDR_MAKE) 
    {

#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)
		
		if( (status = ipAddrMake(bootPara->usrBootParaInfo.dualLinkIpAddr,
								bootPara->usrBootParaInfo.stationNum,
								DLU_SYS_LINE,
								bootPara->usrBootParaInfo.shelfType
								)
			)
		){
        	printf( "DUAL NET IP Address Make ERROR !!!\n");
			goto BOOT_PARA_CONFIG_MODE;
		}
        else
        {
        	printf( "DUAL NET IP Address Make SUCCESS !!!\n");
		}
    
		
        (void)nvRamWrite(  (int8 *)bootPara,
							USR_ENET_CONFIG_PARA_OFFSET, 
							USR_ENET_CONFIG_PARA_SIZE
					    );
#endif
    	
    	printf( "IP Address Making Flag is Manual !!\n");
    }
    else 
    {
    	printf( "IP Address Making Flag Error !!\n");
	} 
	
#ifdef _WIN32
	SYS_ENET_BOOT_LINE_ADDR =  (char*) &bootPara->netBootParaInfo.netLineAIpAddr; 
#endif
	status = bootParaCheck(bootPara);

	if(status != NO_ERROR) 
	{ 	
        errorStringDisplay(	__FILE__, __LINE__, __FUNCTION__, status);

BOOT_PARA_CONFIG_MODE:

		bootParaConfig();
    }

    else 
    {		
        consoleWaitTime = 5;

        if(bootConfigWait(consoleWaitTime))
        { 
		   	
#ifdef _WINCE
			status = fbMemoryDef (MEM_612_BOARD);
#else			
			status = fbMemoryDef (bootPara->usrBootParaInfo.memoryBoardType);
#endif
		    if(status != NO_ERROR)
				systemInitErr(__FILE__, __LINE__, __FUNCTION__, status);
			else 
				printf( "Memory MAP INIT SUCCESS !!!\n");

		   	
			status = fbMemoryAddrInit(LOGIC_ID);
			
		    if(status != NO_ERROR)
				printf	(			"systemInitErr:[%s(%d)]-[%s]-[0x%08x]\n",
									__FILE__,
		        					__LINE__,
		                            (uint8 *)"fbMemoryAddrInit(LOGIC_ID)",
		                            status
								);
			else								
				printf("MPU FB Memory Address Init SUCCESS !!!\n");
            
            bootParaConfig();
        }
    }
    
    return;
}


uint32 bootParaCheck(strSystemBootParaInfo *sysParaPtr)
{
    uint32 status = NO_ERROR;
	strUserBootParaInfo	*usrParaPtr = &sysParaPtr->usrBootParaInfo;
    strNetBootParaInfo	*netParaPtr = &sysParaPtr->netBootParaInfo;
	
    
    if( (usrParaPtr->shelfType != MASTER_SHELF)
                    &&
        (usrParaPtr->shelfType != SLAVE_SHELF))
        status = STATION_SHELF_CONFIG_ERR;

    else if((usrParaPtr->dluIfKind != DLU_IF_AUTO)
                    &&
            (usrParaPtr->dluIfKind != DLU_IF_PMC_ETH)
                    &&
            (usrParaPtr->dluIfKind != DLU_IF_VME_DLU))
        status = DLU_IF_TYPE_CONFIG_ERR;
        
    else if((usrParaPtr->mbSlaveMapIf != MB_SLV_IF_DWORD_FIX)
                    &&
            (usrParaPtr->mbSlaveMapIf != MB_SLV_IF_WORD)
                    &&
            (usrParaPtr->mbSlaveMapIf != MB_SLV_IF_DWORD_WSWAP))
        status = MB_SLV_IF_CONFIG_ERR;

    else if((usrParaPtr->ioShelfType != FBC_NONE)
                    &&
            (usrParaPtr->ioShelfType != PROFIBUS_BOARD)
            		&&
            (usrParaPtr->ioShelfType != FBC_311_BOARD)
            		&&
            (usrParaPtr->ioShelfType != FBC_351_BOARD)
            		&&
            (usrParaPtr->ioShelfType != FBC_601_BOARD)
            		&&
            (usrParaPtr->ioShelfType != FBC_352_BOARD))
        status = IO_SHELF_CONFIG_ERR;

	
	else if((usrParaPtr->hvdcCpuType < HVDC_CPU_C_TYPE) 
								&&
			(usrParaPtr->hvdcCpuType > HVDC_CPU_M_TYPE))
        status = HVDC_CPU_TYPE_CONFIG_ERR;

	
    else if(
    		
    		(
    			(*(netParaPtr->netLineAIpAddr) == ENDOFSTR)
                    ||
            	(ipAddrValidCheck(netParaPtr->netLineAIpAddr) != NO_ERROR)
            ))
        status = TARGET_IP_ADDR_ERR;

    
    else if((*(usrParaPtr->netLineBIpAddr) == ENDOFSTR)
                    ||
            (ipAddrValidCheck(usrParaPtr->netLineBIpAddr) != NO_ERROR))
        status = TARGET_IP_ADDR_ERR;

		
    else if(
            (gEthDluModuleID != 0)
    				&&
    		(
    			(*(usrParaPtr->dualLinkIpAddr) == ENDOFSTR)
                    ||
            	(ipAddrValidCheck(usrParaPtr->dualLinkIpAddr) != NO_ERROR)
            ))
        status = TARGET_IP_ADDR_ERR;

	
    else if( (usrParaPtr->ipMakingFlag != MANUAL_IPADDR_MAKE) &&
    		 (usrParaPtr->ipMakingFlag != AUTO_IPADDR_MAKE)
			)
		status = IP_ADDR_MAKING_FLAG_ERR; 	
	
   
    
    else if( (usrParaPtr->memoryBoardType != MEM_611_BOARD) &&
             (usrParaPtr->memoryBoardType != MEM_612_BOARD) &&
#if ((BOARD_TYPE == MVME3100_BOARD)||(BOARD_TYPE == MVME4100_BOARD)||(BOARD_TYPE == MVME2500_BOARD)||(BOARD_TYPE == LKV120_BOARD))
             (usrParaPtr->memoryBoardType != MEM_613_BOARD) &&
#endif
#if (BOARD_TYPE == LKV120_BOARD)
             (usrParaPtr->memoryBoardType != MEM_4MB_MRAM) &&
#endif
             (usrParaPtr->memoryBoardType != MEM_BOARD_NONE)
              )
          status = MEMORY_BOARD_TYPE_ERR;

    /* 12. 스케쥴링 주기 BASE : 100us, 1ms */
    else if( (usrParaPtr->sysSchedlueType != ONE100US_SCHEDULE_CYCLE) &&
             (usrParaPtr->sysSchedlueType != ONEMS_SCHEDULE_CYCLE)
              )
          status = SCHDULER_TYPE_ERR;

	
    else if((*(usrParaPtr->rmtTargetIpAddr) == ENDOFSTR)
                    ||
            (ipAddrValidCheck(usrParaPtr->rmtTargetIpAddr) != NO_ERROR))
        status = TARGET_IP_ADDR_ERR;

    
    else if((*(usrParaPtr->rmtModuleIpAddr) == ENDOFSTR)
                    ||
            (ipAddrValidCheck(usrParaPtr->rmtModuleIpAddr) != NO_ERROR))
        status = TARGET_IP_ADDR_ERR;

    else if( (usrParaPtr->dluIfKind != DLU_IF_AUTO) &&
             (usrParaPtr->dluIfKind != DLU_IF_PMC_ETH) &&
             (usrParaPtr->dluIfKind != DLU_IF_VME_DLU)
              )
        status = DLU_IF_TYPE_CONFIG_ERR;
    else if( (usrParaPtr->mbSlaveMapIf != MB_SLV_IF_DWORD_FIX) &&
             (usrParaPtr->mbSlaveMapIf != MB_SLV_IF_WORD) &&
             (usrParaPtr->mbSlaveMapIf != MB_SLV_IF_DWORD_WSWAP)
              )
        status = MB_SLV_IF_CONFIG_ERR;
	
	return(status);
}


int32 bootConfigWait(int32 timeout)
{
    int32 bootTime;
    int32 timeLeft;
    int32 timeMarker;
	int32 byteRead = 0;
#ifdef VXWORKS
	int8    key;
#endif
    int32 retVal = 0;

    if(timeout > 0) 
    {
        printf( "\nPRESS RETURN KEY TO CHANGE BOOT PARAMETER[ %d Seconds]\n", timeout);

#ifdef VXWORKS
        bootTime  = systemTickGet();
        bootTime += systemClockRateGet() * timeout;

        timeMarker = systemTickGet() + systemClockRateGet();

        timeLeft = timeout;

        printf( "Time Left : %2d\r",timeLeft);

        while( (systemTickGet() < bootTime) && (byteRead == 0)) 
        {
            (void) ioSysCtrlFunc(gDebugPortFd, MYIONREAD, (int32)&byteRead); 
            if( systemTickGet() == timeMarker) 
            {
                timeMarker = systemTickGet() + systemClockRateGet();
                printf( "Time Left : %2d\r",--timeLeft);
			}
	    } 
#else
        timeLeft = timeout;

        printf( "Time Left : %2d\r",timeLeft);

        while( (0 < timeLeft) && (byteRead == 0)) 
        {
			byteRead = keyHit();
			printf( "Time Left : %2d\r",timeLeft--);
			sysTaskDelay(systemClockRateGet());
	    }
#endif

        if(byteRead == 0)
        {
#ifdef VXWORKS
		    
            (void) ioSysCtrlFunc(gDebugPortFd, MYIOSETOPTIONS, MYOPT_TERMINAL);
#endif
            retVal = 0;

            printf( "\nSTART BOOTING ............\n");
        }
        else 
        { 

#ifdef _WIN32
			printf( "\n");
#endif
            
#ifdef VXWORKS
		    ioRead(gDebugPortFd, &key, 1);
#endif
            retVal = 1;
        }
    }
    else 
    {
        printf( "\nBooting timeout value is negative\n");
        retVal = 1;
    }

    return(retVal);
}


void bootParaConfig(void)
{
    int8    line[16], *pLine;
    
    int8    debugPrompt[32];
    int8	dbgKind[] = "Board Boot";
    
    int8    defaultStr[] = "Unrecognized Command. Type '?','h','H' for Help\n";

	strDbgCmdFuncInfo mpuDebugCmdFunc[] =
	{
		{"?", 		NULL, 		NULL,	&bootCmdHelp},
		{"h",		NULL,		NULL,	&bootCmdHelp},
		{"cbp", 	NULL, 		NULL,	&bootParaChange},
		{"dbp",		NVRAM_READ,	NULL,	&bootParaDisplay},
		{"ipd", 	NULL, 		NULL,	&ipAddressDisplay},
		{"fid", 	LOGIC_ID,	NULL,	&fbCodeInBusMemDisplay},
		{"flc", 	LOGIC_ID,	NULL,	&fbLogicMemoryMemoryClear},
		{"brt", 	NULL, 		NULL,	&cpuBrdRebootByManual},
		{"NULL",	NULL, 		NULL,	NULL} 	 	
	};

	strDbgCmdFuncInfo *cmdFuncPtr;
	
	memoryClear((uint8 *)debugPrompt, 32);

  	(void)sprintf(debugPrompt, "[%s]> ", dbgKind);

	for(;;) 
	{
	    (void)printf(debugPrompt);

        (void)ioRead(gDebugPortFd, line, sizeof(line));
		
        pLine = line;
        
	    spaceSkip(&pLine);
		
		upperToLowerCaseConversion( pLine, strlen(pLine));
		
		if(*pLine != ENDOFSTR) 
		{
			for(cmdFuncPtr = mpuDebugCmdFunc; cmdFuncPtr->dbgFunc != NULL; cmdFuncPtr++)
			{
				if(stringCompare(pLine,cmdFuncPtr->dbgCmdStr) == NO_ERROR)
				{
					cmdFuncPtr->dbgFunc(cmdFuncPtr->arg1,cmdFuncPtr->arg2);
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
}

void routeTblInfoAllDelete(uint32 arg1, uint32 arg2)
{
	strSystemBootParaInfo	paraData;
    uint32	status = 0;

    memoryClear((uint8 *) &paraData, sizeof(strSystemBootParaInfo));

    status  = nvRamRead((int8 *)&paraData,
						USR_ENET_CONFIG_PARA_OFFSET, 
						USR_ENET_CONFIG_PARA_SIZE
					   );
	if(status)
	{
    	printf("NV-RAM Read Error !!\n");
        return;
	}

	paraData.usrBootParaInfo.routeIndex = 0x0;

   	
   	status = nvRamWrite(  (int8 *)&paraData,
       	               USR_ENET_CONFIG_PARA_OFFSET,
           	           USR_ENET_CONFIG_PARA_SIZE
               	    );
	if(status == NO_ERROR)
		 printf("routeTblInfoAllDelete OK!!\n");

	return;
}


static void bootParaChange(uint32 arg1, uint32 arg2)
{
	uint32	status;
    int8    line[32];
    int8    ipAddr[20];
    uint32  diHexSetup, doHexSetup, aiHexSetup, dspHexSetup, odmUseSetup;    
    int8    *cPtr;
    int8    **strPtr;
    int8    targetName[32];
    static  int8 *exitMsg[] =
    {
        " ", "['.'] - exit without saving",
        NULL
    };
    static  int8 *defaultMsg[] =
    {
        "Type following format", "- [XXX.XXX.XXX.XXX]",
        "Type following character","- ['y' or 'Y'] - UPDATE",
        " ",                       "- ['n' or 'N'] - CANCEL",
        NULL
    };

    strSystemBootParaInfo sysBootParaInfo;
   	strNetBootParaInfo	*netParaPtr;
    strUserBootParaInfo	*usrParaPtr;

    memoryClear ( (uint8 *)&sysBootParaInfo, sizeof(strSystemBootParaInfo));
    memoryClear ( (uint8 *)ipAddr, 20);
    memoryClear ( (uint8 *)targetName, 32);

    netParaPtr = &sysBootParaInfo.netBootParaInfo;
    usrParaPtr = &sysBootParaInfo.usrBootParaInfo;

    status  = nvRamRead((int8 *)&sysBootParaInfo,
						USR_ENET_CONFIG_PARA_OFFSET, 
						USR_ENET_CONFIG_PARA_SIZE
					   );
					   
	if(status != NO_ERROR)
		printf("[ERROR] - Boot Parameter Read Error[0x%x]\n", status);
		
    printf( "\n*****************[BOOT PARAMETER CHANGE]*****************\n");

    for(;;)
    {
HVDC_CPU_TYPE_SETUP:
        printf( "HVDC CPU Type Set Up? [1:CPU-C,2:CPU-P,3:CPU-V,4:CPU-M]: ");

        ioRead(gDebugPortFd, line, sizeof(line));
        cPtr = line;
        spaceSkip(&cPtr);

        switch(*cPtr) 
        {
            case '1': 
            case ENDOFSTR:
                usrParaPtr->hvdcCpuType = HVDC_CPU_C_TYPE;
            break;
            case '2':
                usrParaPtr->hvdcCpuType = HVDC_CPU_P_TYPE;
            break;
            case '3':
                usrParaPtr->hvdcCpuType = HVDC_CPU_V_TYPE;
            break;
            case '4':
                usrParaPtr->hvdcCpuType = HVDC_CPU_M_TYPE;
            break;
            case '.': 
                goto MODULE_END;
            break;
            default :
                printf( "\t** Modbus Slave Mapping Interface is invalid(0 ~ 2) !!\n");
                goto HVDC_CPU_TYPE_SETUP;
            break;
        }	
        

IP_ADDR_MAKING_FLAG_SETUP:
		printf( "IP ADDRESS Make Auto[A] or Manual[M] ?[A or M] ");
	
        ioRead(gDebugPortFd, line, sizeof(line));
        cPtr = line;
        spaceSkip(&cPtr);

        switch(*cPtr) 
        {
            case 'a': case 'A':
                usrParaPtr->ipMakingFlag = AUTO_IPADDR_MAKE;
            break;
            
            case 'm': case 'M':
                usrParaPtr->ipMakingFlag = MANUAL_IPADDR_MAKE;
            break;
            
            case '.': 
                goto MODULE_END;
            break;
            
            default :
            	printf( "**********[HELP]************\n");
				printf( "'a, A' - IP Addr Making AUTO\n");
				printf( "'m, M' - IP Addr Making MANUAL. ");
				printf( "Type 'a(A)' or 'm(M)'\n");
				goto IP_ADDR_MAKING_FLAG_SETUP;
            break;
        }

STATION_NUM_SETUP:
        printf( "Station Number Set (1 ~ 64) ? ");

        ioRead(gDebugPortFd, line, sizeof(line));
        cPtr = line;
        spaceSkip(&cPtr);
        usrParaPtr->stationNum = atoi(cPtr);

        if( (usrParaPtr->stationNum < MIN_STATION_ID) ||
            (usrParaPtr->stationNum > MAX_STATION_ID)) 
		{
            printf( "\t** Station Number is invalid(1 ~ 64) !!\n");
            goto STATION_NUM_SETUP;
        }
        
    	usrParaPtr->shelfType = MASTER_SHELF;
		
        usrParaPtr->mpuEnable = BOARD_ENABLE;
        
DLU_IF_TYPE_SETUP:
        usrParaPtr->dluIfKind = DLU_IF_AUTO;	
        
  		if( (usrParaPtr->hvdcCpuType == HVDC_CPU_C_TYPE)
      		||  (usrParaPtr->hvdcCpuType == HVDC_CPU_P_TYPE)    )
		{
MB_SLAVE_IF_SETUP:
	        printf( "Modbus Slave Mapping Interface Set Up? [1:DWORD_FIX,2:WORD,3:DWORD_WSWAP]: ");
	
	        ioRead(gDebugPortFd, line, sizeof(line));
	        cPtr = line;
	        spaceSkip(&cPtr);
	
	        switch(*cPtr) 
	        {
	            case '1': 
	                usrParaPtr->mbSlaveMapIf = MB_SLV_IF_DWORD_FIX;
	            break;
	            case ENDOFSTR:
	                printf( "\t** Default setting was selected!!(2:WORD)\n");
	            case '2':
	                usrParaPtr->mbSlaveMapIf = MB_SLV_IF_WORD;
	            break;
	            case '3':
	                usrParaPtr->mbSlaveMapIf = MB_SLV_IF_DWORD_WSWAP;
	            break;
	            case '.': 
	                goto MODULE_END;
	            break;
	            default :
	                printf( "\t** Modbus Slave Mapping Interface is invalid(0 ~ 2) !!\n");
	                goto MB_SLAVE_IF_SETUP;
	            break;
	        }			
	
SCHDULER_SETUP:
	  		
	        printf( "Scheduler Base Time ? [1: 100us, 2: 1ms] : ");
	
	        ioRead(gDebugPortFd, line, sizeof(line));
	        cPtr = line;
	        spaceSkip(&cPtr);
	
	        switch(*cPtr) 
	        {
	            case '1':
	                usrParaPtr->sysSchedlueType = ONE100US_SCHEDULE_CYCLE;
	            break;
	            case ENDOFSTR:
	                printf( "\t** Default setting was selected!!(2: 1ms)\n");
	            case '2':
	                usrParaPtr->sysSchedlueType = ONEMS_SCHEDULE_CYCLE;
	            break;
	            case '.': 
	                goto MODULE_END;
	            break;
	            default :
	            	printf( "\t** Control Target Selection is invalid(1 or 2) !!\n");
	                goto SCHDULER_SETUP;
	            break;
	        }
		}
        else
        {
            usrParaPtr->mbSlaveMapIf 	= MB_SLV_IF_WORD;
            usrParaPtr->sysSchedlueType = ONEMS_SCHEDULE_CYCLE;
        }

IOSHELF_SETUP:
        usrParaPtr->ioShelfType = FBC_NONE;

#define DIO_SLOT_BITS_MASK  (0x1F)
#define VCB_SLOT_BITS_MASK  (0x3F)
#define AI_SLOT_BITS_MASK   (0x3F)

	    if((usrParaPtr->hvdcCpuType == HVDC_CPU_C_TYPE) || (usrParaPtr->hvdcCpuType == HVDC_CPU_P_TYPE))
	    {
	DI_SLOT_SETUP:
	        printf( "Enter slot bits setting for using DI.(0x0~0x1F[Bit4~0] Default:0x1): 0x");
	
	        scanf("%x", &diHexSetup);
	        if(diHexSetup&(~DIO_SLOT_BITS_MASK))
	            printf( "\t** Hex value 0x%x will be selected!!\n",diHexSetup);
	        diHexSetup = diHexSetup&DIO_SLOT_BITS_MASK; /* mask value */
	        usrParaPtr->hvdcDiConfig = diHexSetup;		
	
	DO_SLOT_SETUP:
	        printf( "Enter slot bits setting for using DO.(0x0~0x1F[Bit4~0] Default:0x2): 0x");
	
	        scanf("%x", &doHexSetup);
	        if(doHexSetup&(~DIO_SLOT_BITS_MASK))
	            printf( "\t** Hex value 0x%x will be selected!!\n",doHexSetup);
	        doHexSetup = doHexSetup&DIO_SLOT_BITS_MASK; /* mask value */
	        
	        if(diHexSetup&doHexSetup)
	        {
	            printf( "\t** [ERROR]Config slot was overlaped. Retry!! (DI:0x%02x, DO:0x%02x)\n", diHexSetup, doHexSetup);
	            goto DO_SLOT_SETUP;
	        }
	        usrParaPtr->hvdcDoConfig = doHexSetup;
	    }
	    else
	    {
	        usrParaPtr->hvdcDiConfig = 0;	
	        usrParaPtr->hvdcDoConfig = 0;	
	    }
        
	    if((usrParaPtr->hvdcCpuType == HVDC_CPU_C_TYPE))
		{
	CCB_SLOT_SETUP:
		    printf( "Use Current Control Board ? [Y/N] : ");
		
		    ioRead(gDebugPortFd, line, sizeof(line));
		    cPtr = line;
		    spaceSkip(&cPtr);
		
		    switch(*cPtr) {
		        case ENDOFSTR:
		            printf( "\t** Default setting was selected!!(Y: Use CCB)\n");
		        case 'y': case 'Y':
		            usrParaPtr->hvdcCcbConfig = 0x1;
		        break;
		        case 'n': case 'N':
		            usrParaPtr->hvdcCcbConfig = 0x0;
		        break;
		        default :
		        	printf( "\t** Setting value is invalid(Y or N) !!\n");
		            goto CCB_SLOT_SETUP;
		        break;
		    }
		}
	    else
	    {
	        usrParaPtr->hvdcCcbConfig = 0x0;
	    }
    
	    if((usrParaPtr->hvdcCpuType == HVDC_CPU_C_TYPE))
	    {
	        if(     (usrParaPtr->hvdcDiConfig != 0x1)
	            ||  (usrParaPtr->hvdcDoConfig != 0x2)
	            ||  (usrParaPtr->hvdcCcbConfig != 0x1))
	        {
	            printf( "Your setting is 0x%02x/0x%02x/0x%01x[DI/DO/CCB(Default:0x01/0x02/0x1)].\n",
	                    usrParaPtr->hvdcDiConfig,usrParaPtr->hvdcDoConfig,usrParaPtr->hvdcCcbConfig);
	            printf( "Are you sure this setting? [Y/N] : ");
	        
	            ioRead(gDebugPortFd, line, sizeof(line));
	            cPtr = line;
	            spaceSkip(&cPtr);
	        
	            switch(*cPtr) 
	            {
	                case ENDOFSTR:
	                case 'y': case 'Y':
	                break;
	                case 'n': case 'N':
	                    goto DI_SLOT_SETUP;
	                break;
	            }  
	        }
	    }
	    
	    if((usrParaPtr->hvdcCpuType == HVDC_CPU_P_TYPE))
	    {
	        if(     (usrParaPtr->hvdcDiConfig != 0x1)
	            ||  (usrParaPtr->hvdcDoConfig != 0x2))
	        {
	            printf( "Your setting is 0x%02x/0x%02x/0x%01x[DI/DO(Default:0x01/0x02)].\n",
	                    usrParaPtr->hvdcDiConfig,usrParaPtr->hvdcDoConfig);
	            printf( "Are you sure this setting? [Y/N] : ");
	        
	            ioRead(gDebugPortFd, line, sizeof(line));
	            cPtr = line;
	            spaceSkip(&cPtr);
	        
	            switch(*cPtr) {
	                case ENDOFSTR:
	                case 'y': case 'Y':
	                break;
	                case 'n': case 'N':
	                    goto DI_SLOT_SETUP;
	                break;
	            }  
	        }
	    }

	    if((usrParaPtr->hvdcCpuType == HVDC_CPU_V_TYPE))
	    {
	VCB_SLOT_SETUP:
	        printf( "Enter slot bits setting for using VCB.(0x0~0x3F[Bit5~0] Default:0x3F): 0x");
	
	        scanf("%x", &dspHexSetup);
	        if(dspHexSetup&(~VCB_SLOT_BITS_MASK))
	            printf( "\t** Hex value 0x%x will be selected!!\n",dspHexSetup);
	        dspHexSetup = dspHexSetup&VCB_SLOT_BITS_MASK; /* mask value */
	        usrParaPtr->hvdcVcbConfig = dspHexSetup;	    
	    }
	    else
	    {
	        usrParaPtr->hvdcVcbConfig = 0x0;
	    }
    
	    if((usrParaPtr->hvdcCpuType == HVDC_CPU_V_TYPE))
	    {
	        if(usrParaPtr->hvdcVcbConfig != VCB_SLOT_BITS_MASK)
	        {
	            printf( "Your setting is 0x%02x[VCB(Default:0x3F)].\n",
	                    usrParaPtr->hvdcVcbConfig);
	            printf( "Are you sure this setting? [Y/N] : ");
	        
	            ioRead(gDebugPortFd, line, sizeof(line));
	            cPtr = line;
	            spaceSkip(&cPtr);
	        
	            switch(*cPtr) 
	            {
	                case ENDOFSTR:
	                case 'y': case 'Y':
	                break;
	                case 'n': case 'N':
	                    goto VCB_SLOT_SETUP;
	                break;
	            }  
	        }
	    }
	
	    if((usrParaPtr->hvdcCpuType == HVDC_CPU_M_TYPE))
	    {
	AI_SLOT_SETUP:
	        printf( "Enter slot bits setting for using AI.(0x0~0x3F[Bit5~0] Default:0x3F): 0x");
	
	        scanf("%x", &dspHexSetup);
	        if(dspHexSetup&(~AI_SLOT_BITS_MASK))
	            printf( "\t** Hex value 0x%x will be selected!!\n",dspHexSetup);
	            
	        dspHexSetup = dspHexSetup&AI_SLOT_BITS_MASK; /* mask value */
	        usrParaPtr->hvdcAiConfig = dspHexSetup;	
	
	ODM_SLOT_SETUP:
	    printf( "Use ODM Board ? [Y/N] : ");
	
	    ioRead(gDebugPortFd, line, sizeof(line));
	    cPtr = line;
	    spaceSkip(&cPtr);
	
	    switch(*cPtr) 
	    {
	        case ENDOFSTR:
	            printf( "\t** Default setting was selected!!(Y: Use ODM)\n");
	        case 'y': case 'Y':
	            usrParaPtr->hvdcOdmConfig = 0x1;
	        break;
	        case 'n': case 'N':
	            usrParaPtr->hvdcOdmConfig = 0x0;
	        break;
	        default :
	        	printf( "\t** Setting value is invalid(Y or N) !!\n");
	            goto ODM_SLOT_SETUP;
	        break;
	    }     
	    }
	    else
	    {
	        usrParaPtr->hvdcAiConfig = 0x0;
	        usrParaPtr->hvdcOdmConfig = 0x0;
	    }       
        
	    if((usrParaPtr->hvdcCpuType == HVDC_CPU_M_TYPE))
	    {
	        if(     (usrParaPtr->hvdcAiConfig != AI_SLOT_BITS_MASK)
	            ||  (usrParaPtr->hvdcOdmConfig != 0x1))
	        {
	            printf( "Your setting is 0x%02x/0x%01x[AI/ODM(Default:0x3F/0x1)].\n",
	                    usrParaPtr->hvdcAiConfig, usrParaPtr->hvdcOdmConfig);
	            printf( "Are you sure this setting? [Y/N] : ");
	        
	            ioRead(gDebugPortFd, line, sizeof(line));
	            cPtr = line;
	            spaceSkip(&cPtr);
	        
	            switch(*cPtr) 
	            {
	                case ENDOFSTR:
	                case 'y': case 'Y':
	                break;
	                case 'n': case 'N':
	                    goto AI_SLOT_SETUP;
	                break;
	            }  
	        }
	    }
		
		/* default로 보드에 부착된 M 메모리 사용 */        
        usrParaPtr->memoryBoardType = MEM_4MB_MRAM;
		
		if(usrParaPtr->ipMakingFlag == AUTO_IPADDR_MAKE)
		{
			memoryClear( (uint8 *)netParaPtr->netLineAIpAddr,NET_TGT_ADDR_LEN);
			ipAddrMake(	netParaPtr->netLineAIpAddr,
					   	usrParaPtr->stationNum,
						NETWORK_A_LINE,
						usrParaPtr->shelfType
					  );
			memoryClear( (uint8 *)usrParaPtr->netLineBIpAddr,NET_TGT_ADDR_LEN);
			ipAddrMake(usrParaPtr->netLineBIpAddr,
						usrParaPtr->stationNum,
						NETWORK_B_LINE,
						usrParaPtr->shelfType
						  );			
#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)
			
			memoryClear( (uint8 *)usrParaPtr->dualLinkIpAddr,NET_TGT_ADDR_LEN);
			ipAddrMake(usrParaPtr->dualLinkIpAddr,
						usrParaPtr->stationNum,
						DLU_SYS_LINE,
						usrParaPtr->shelfType
						  );
#endif
			
			if(usrParaPtr->shelfType == MASTER_SHELF) {
				memoryClear( (uint8 *)usrParaPtr->rmtTargetIpAddr,NET_TGT_ADDR_LEN);
				ipAddrMake(	usrParaPtr->rmtTargetIpAddr,
						   	usrParaPtr->stationNum,
							NETWORK_A_LINE,
							SLAVE_SHELF
						  );
				memoryClear( (uint8 *)usrParaPtr->rmtModuleIpAddr,NET_TGT_ADDR_LEN);
				ipAddrMake(usrParaPtr->rmtModuleIpAddr,
							usrParaPtr->stationNum,
							NETWORK_B_LINE,
							SLAVE_SHELF
						  );			
			}
			else {
				memoryClear( (uint8 *)usrParaPtr->rmtTargetIpAddr,NET_TGT_ADDR_LEN);
				ipAddrMake(	usrParaPtr->rmtTargetIpAddr,
						   	usrParaPtr->stationNum,
							NETWORK_A_LINE,
							MASTER_SHELF
						  );
				memoryClear( (uint8 *)usrParaPtr->rmtModuleIpAddr,NET_TGT_ADDR_LEN);
				ipAddrMake(usrParaPtr->rmtModuleIpAddr,
							usrParaPtr->stationNum,
							NETWORK_B_LINE,
							MASTER_SHELF
						  );
			}
			
			goto BOOT_PARA_SETUP;
		}

IP_ADDR_SETUP:
        printf( "Ctrl Net LINE-A Ip Address Enter ? (XXX.XXX.XXX.XXX) : ");
        ioRead(gDebugPortFd, ipAddr, sizeof(ipAddr));

        cPtr = ipAddr;
        spaceSkip(&cPtr);

        if( (*cPtr != ENDOFSTR) && (ipAddrValidCheck((uint8 *)cPtr) == NO_ERROR ))
        {
			memoryClear( (uint8 *)netParaPtr->netLineAIpAddr,NET_TGT_ADDR_LEN);
            memoryCopy( (int8 *)netParaPtr->netLineAIpAddr,
                        (int8 *)ipAddr,
                        (uint32) strlen((int8*)ipAddr)
                      );
        }
        else if ( *cPtr == '.') 
        	goto MODULE_END;

        else 
        {
            strPtr = defaultMsg;
            strPtr += 20;

            usgMsg(strPtr, exitMsg, 2);
            goto IP_ADDR_SETUP;
        }
        
IP_ADDR_SETUP1:
        memoryClear ( (int8 *)ipAddr, 20);
        printf( "Ctrl Net LINE-B Ip Address Enter ? (XXX.XXX.XXX.XXX) : ");
        ioRead(gDebugPortFd, ipAddr, sizeof(ipAddr));

        cPtr = ipAddr;
        spaceSkip(&cPtr);

        if( (*cPtr != ENDOFSTR) && (ipAddrValidCheck((uint8 *)cPtr) == NO_ERROR ))
        {
			memoryClear( (uint8 *)usrParaPtr->netLineBIpAddr,NET_TGT_ADDR_LEN);
            memoryCopy( (int8 *)usrParaPtr->netLineBIpAddr,
                        (int8 *)ipAddr,
                        (uint32) strlen(ipAddr)
                      );
        }
        else if ( *cPtr == '.') 
        	goto MODULE_END;

        else 
        {
            strPtr = defaultMsg;
            strPtr += 20;

            usgMsg(strPtr, exitMsg, 2);
            goto IP_ADDR_SETUP1;
        }
			
IP_ADDR_SETUP2:
        printf( "Remote Ctrl Net Line-A IP Address Enter ? (XXX.XXX.XXX.XXX) : ");
        ioRead(gDebugPortFd, ipAddr, sizeof(ipAddr));

        cPtr = ipAddr;
        spaceSkip(&cPtr);

        if( (*cPtr != ENDOFSTR) && (ipAddrValidCheck((uint8 *)cPtr) == NO_ERROR ))
        {
			memoryClear( (uint8 *)usrParaPtr->rmtTargetIpAddr,NET_TGT_ADDR_LEN);
            memoryCopy( (int8 *)usrParaPtr->rmtTargetIpAddr,
                        (int8 *)ipAddr,
                        (uint32) strlen((int8*)ipAddr)
                      );
        }
        else if ( *cPtr == '.') 
        	goto MODULE_END;

        else 
        {
            strPtr = defaultMsg;
            strPtr += 20;

            usgMsg(strPtr, exitMsg, 2);
            goto IP_ADDR_SETUP2;
        }
        
IP_ADDR_SETUP3:
        memoryClear ( (int8 *)ipAddr, 20);
        printf( "Remote Ctrl Net Line-B IP Address Enter ? (XXX.XXX.XXX.XXX) : ");
        ioRead(gDebugPortFd, ipAddr, sizeof(ipAddr));

        cPtr = ipAddr;
        spaceSkip(&cPtr);

        if( (*cPtr != ENDOFSTR) && (ipAddrValidCheck((uint8 *)cPtr) == NO_ERROR ))
        {
			memoryClear( (uint8 *)usrParaPtr->rmtModuleIpAddr,NET_TGT_ADDR_LEN);
            memoryCopy( (int8 *)usrParaPtr->rmtModuleIpAddr,
                        (int8 *)ipAddr,
                        (uint32) strlen(ipAddr)
                      );
        }
        else if ( *cPtr == '.') 
        	goto MODULE_END;

        else 
        {
            strPtr = defaultMsg;
            strPtr += 20;

            usgMsg(strPtr, exitMsg, 2);
            goto IP_ADDR_SETUP3;
        }

        break; 
    } 

BOOT_PARA_SETUP:
    
    memoryCopy( (int8 *)netParaPtr->bootDev,
                (int8 *)NET_DEV_NAME,
                strlen(NET_DEV_NAME)
              );
    
    sprintf(targetName, "%s%d", "STATION-", usrParaPtr->stationNum);
    memoryCopy((int8 *)netParaPtr->targetName,targetName, strlen(targetName));


PARA_UPDATE:
    
    printf( "Update Boot Parameter ? [Y/N] : ");

    ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

    switch(*cPtr) 
    {
        case 'y': case 'Y':
            
			sysBootParameterUpdate(netParaPtr->netLineAIpAddr);
			
            
            (void)nvRamWrite(  (int8 *)&sysBootParaInfo,
                                USR_ENET_CONFIG_PARA_OFFSET,
                                USR_ENET_CONFIG_PARA_SIZE
                            );

			status = fbMemoryDef (usrParaPtr->memoryBoardType);
			if(status != NO_ERROR)
					systemInitErr(__FILE__, __LINE__, __FUNCTION__, status);
			else 
				printf( "Memory MAP INIT SUCCESS !!!\n");
				
			status = fbMemoryAddrInit(LOGIC_ID);
			if(status != NO_ERROR)
				printf	(			"systemInitErr:[%s(%d)]-[%s]-[0x%08x]\n",
									__FILE__,
			    					__LINE__,
			                        (uint8 *)"fbMemoryAddrInit(LOGIC_ID)",
			                        status
								);
			else								
				printf("MPU FB Memory Address Init SUCCESS !!!\n");

            printf( "\nUpdating Boot parameter SUCCESS !!\n");
        break;
        
        case 'n': case 'N':
            printf( "\nUpdating Boot parameter CANCEL !!\n");
        break;
        
        default:
            strPtr = defaultMsg;
            strPtr += 24;
            usgMsg(strPtr,exitMsg, 2);
            goto PARA_UPDATE;
        break;
    }

MODULE_END:
	
	return;
}

void usgMsg(int8 **defaultMsg, int8 **exitMsg, int32 count)
{
    int8    **strPtr = NULL;
    int32     i;

    printf( "\n");
    for(strPtr = defaultMsg,i = 0; i < count; strPtr += 2, i++)
        printf( "\t%-25s %s\n", *strPtr, *(strPtr+1));

    printf( "\t%-25s %s\n", *exitMsg, *(exitMsg+1));
}


void sysBootParameterUpdate(int8 *ipAddrPtr)
{
    uint8    *tmpAddr = NULL;
    uint8    tAddrStr[32];
    static uint8 sysBootLine[SYS_ENET_BOOT_PARA_SIZE];
    
#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD) || (BOARD_TYPE == MVME2500_BOARD)|| (BOARD_TYPE == LKV120_BOARD))
	uint32 status = NO_ERROR;
	uint8 	readBootLine[SYS_ENET_BOOT_PARA_SIZE];
	strNetBootParaInfo	netBootParaInfo;
#endif
	
    memoryClear ( (uint8 *)sysBootLine, SYS_ENET_BOOT_PARA_SIZE);
    memoryClear ( (uint8 *)tAddrStr, 32);

#if ((BOARD_TYPE == MVME2100_BOARD)|| (BOARD_TYPE == X86_PC_BOARD))
    
    memoryCopy( tAddrStr,
                (uint8 *)" e=",
                strlen(" e=")
              );
    memoryCopy( (uint8 *)(tAddrStr+strlen(" e=")),
                (uint8 *)ipAddrPtr,
                strlen((const char *)ipAddrPtr)
                );

    
    tmpAddr = sysBootLine;
    memoryCopy( tmpAddr,
                (uint8 *)SYS_ENET_BOOT_PARA1,
                strlen(SYS_ENET_BOOT_PARA1)
              );
    tmpAddr += strlen(SYS_ENET_BOOT_PARA1);

    memoryCopy( tmpAddr,
                tAddrStr,
                strlen((const char *)tAddrStr)
              );
    tmpAddr += strlen((const char *)tAddrStr);
    memoryCopy( tmpAddr,
                (uint8 *)SYS_ENET_BOOT_PARA2,
                strlen(SYS_ENET_BOOT_PARA2)
               );
	(void)nvRamWrite(   (int8 *)sysBootLine,
                        SYS_ENET_BOOT_PARA_OFFSET,
                        SYS_ENET_BOOT_PARA_SIZE
                    );
#elif ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD) || (BOARD_TYPE == MVME2500_BOARD)|| (BOARD_TYPE == LKV120_BOARD))
	memoryClear ( 	(uint8 *)&netBootParaInfo, 
					sizeof(strNetBootParaInfo)
				);
				
    tmpAddr = sysBootLine;

	
	memoryCopy(	tmpAddr,
				(uint8 *)SYS_ENET_BOOT_PARA1,
				strlen(SYS_ENET_BOOT_PARA1)
			  );
			  
	
	tmpAddr += strlen(SYS_ENET_BOOT_PARA1);

    status  = nvRamRead((uint8 *)readBootLine,
						SYS_ENET_BOOT_PARA_OFFSET, 
						SYS_ENET_BOOT_PARA_SIZE
					   );
	if(status != NO_ERROR)
		systemInitErr(__FILE__, __LINE__, __FUNCTION__, status);

#ifdef VXWORKS
	if(usrBootLineCrack(readBootLine, &netBootParaInfo) != NO_ERROR)
	{
#if (BOARD_TYPE == MVME3100_BOARD)
		systemInitErr(__FILE__, __LINE__, __FUNCTION__, NVRAM_PARA_ERR);

#elif ((BOARD_TYPE == MVME4100_BOARD)||(BOARD_TYPE == MVME2500_BOARD)|| (BOARD_TYPE == LKV120_BOARD))
		printf	(	__FILE__,
        			__LINE__,
                    (uint8 *)"sysBootParameterUpdate",
                    "NVRAM_PARA_ERR",
					"[%s(%d)]-[%s]-[%s]\n");
#endif
	}
#endif 
	if(netBootParaInfo.bootFile[0] != ENDOFSTR)
	{
		memoryCopy(	tmpAddr,
					(uint8 *)netBootParaInfo.bootFile,
					strlen(netBootParaInfo.bootFile)
				  );
		tmpAddr += strlen(netBootParaInfo.bootFile);
	}
	else 
	{
		memoryCopy(	tmpAddr,
					(uint8 *)SYS_DEFAULT_BOOTFILE,
					strlen(SYS_DEFAULT_BOOTFILE)
				  );
		tmpAddr += strlen(SYS_DEFAULT_BOOTFILE);
	}
	
	
    memoryCopy( tAddrStr,
                (uint8 *)" e=",
                strlen(" e=")
              );
    memoryCopy( (uint8 *)(tAddrStr+strlen(" e=")),
                (uint8 *)ipAddrPtr,
                strlen(ipAddrPtr)
                );

	memoryCopy(	tmpAddr,
				tAddrStr,
				strlen(tAddrStr)
			   );	

	
	tmpAddr += strlen(tAddrStr);
	memoryCopy(	tmpAddr,
				(uint8 *)SYS_DEFAULT_HOSTNAME,
				strlen(SYS_DEFAULT_HOSTNAME)
			  );

	
	tmpAddr += strlen(SYS_DEFAULT_HOSTNAME);
	memoryCopy(	tmpAddr,
				(uint8 *)SYS_ENET_BOOT_PARA2,
				strlen(SYS_ENET_BOOT_PARA2)
			  );

	
    (void)nvRamWrite(   sysBootLine,
                        SYS_ENET_BOOT_PARA_OFFSET,
                        SYS_ENET_BOOT_PARA_SIZE
                    );
#if ((BOARD_TYPE == MVME4100_BOARD) || (BOARD_TYPE == MVME2500_BOARD)|| (BOARD_TYPE == LKV120_BOARD))
    
	if(usrBootLineCrack(readBootLine, &netBootParaInfo) != NO_ERROR)
		systemInitErr(__FILE__, __LINE__, __FUNCTION__, NVRAM_PARA_ERR);
#endif
#else
	#error "[ERROR]-You Must Define BOARD_TYPE in sysDefs.h) !!!"
#endif
}


void bootParaDisplay(uint32	whereToRead, uint32 arg2)
{
    int32 index;
    uint8    ipAddr[32], subNet[32];
	strSysConfigInfo	configData;
    strSystemBootParaInfo	bootData;
	uint32	status;
	
    int8 *configMsg[] =
    {
        (int8 *)"NULL",(int8 *)"MASTER",(int8 *)"SLAVE",(int8 *)"UNKNOWN"
    };

    int8 *boardConfigMsg[] =
    {
        (int8 *)"NULL", (int8 *)"NOT CONFIG", (int8 *)"CONFIG", (int8 *)"UNKNOWN"
    };
	int8 *ipMakingFlagStr[] =
    {
		(int8 *)"NULL",(int8 *)"MANUAL", (int8 *)"AUTO", (int8 *)"UNKNOWN"
    };
    int8 *fbcBoardStr[] =
    {
    	"NULL", "NONE", "FBC311", "FBC351", "FBC601", "FBC352",
    	"UNKNOWN"
	};
    int8 *memBoardStr[] =
    {
    	"MEM_NONE", "MEM611(2M Bytes)","MEM612(4M Bytes)","MEM613(8M Bytes)","MRAM(4M Bytes)",
    	"UNKNOWN"
	};
	int8 *schedulerStr[] =
    {
    	"NULL", "100us","1ms",
    	"UNKNOWN"
	};
    int8 *hvdcCpuTypeStr[] =
    {
    	"UNKNOWN", "CPU-C", "CPU-P", "CPU-V", "CPU-M"
	};
	int8 *fieldbusIfKindMsg[] =
    {
        "NULL", "NOT CONFIG", "FMC CONFIG", "PROFI-SHELF CONFIG", "UNKNOWN"
    };
    int8 *mbSlvMapIfStr[] =
    {
    	"NULL", "DWORD_FIX", "WORD","DWORD_WSWAP",
    	"UNKNOWN"
	};
	int8 *dluIfKindMsg[] =
    {
        "NULL", "AUTO CONFIG", "ETH_PMC CONFIG", "VME_DLU CONFIG", "UNKNOWN"
    };

    memoryClear( (uint8 *)ipAddr, 32);
    memoryClear( (uint8 *)subNet, 32);
	memoryClear( (uint8 *) &configData,sizeof(strSysConfigInfo));
	memoryClear( (uint8 *) &bootData, USR_ENET_CONFIG_PARA_SIZE);
 
    if(whereToRead == NVRAM_READ) 
    {
 		
    	status = nvRamRead((int8 *)&bootData,
							USR_ENET_CONFIG_PARA_OFFSET, 
							USR_ENET_CONFIG_PARA_SIZE
						   );
		if(status)
        	printf("[BootParaDisplay(%d)] - NVRAM Read Err\n",__LINE__);
        else 
	       	systemCnfgDebugInfoSet(&configData, &bootData);
    }
    else 
    	systemCnfgInfoRead((strSysConfigInfo *)&configData);

    index = 0;

    printf("\n\t**********[BOOTING PARAMETER DISPLAY]************\n");

    if( (configData.hvdcCpuType < HVDC_CPU_C_TYPE) || (configData.hvdcCpuType > HVDC_CPU_M_TYPE))
        index = 0;
    else
        index = configData.hvdcCpuType;
    	
    printf( "\t         HVDC CPU TYPE : %s\n", hvdcCpuTypeStr[index]);
    
    if(configData.ipMakingFlag > AUTO_IPADDR_MAKE) index = 3;
    else index = configData.ipMakingFlag;

    printf( "\t             SYSTEM ID : %d\n", configData.stationId);

    if( (configData.shelfType > SLAVE_SHELF) || (configData.shelfType < MASTER_SHELF))
        index = 3;
    else
        index = configData.shelfType;

    printf( "\t           SYSTEM TYPE : %s\n", configMsg[index]);

    if( (configData.mbSlaveMapIf < MB_SLV_IF_DWORD_FIX) || (configData.mbSlaveMapIf > MB_SLV_IF_DWORD_WSWAP))
        index = 4;
    else
        index = configData.mbSlaveMapIf;

    printf( "\tModbus Slave If Config : %s\n", mbSlvMapIfStr[index]);

    if( (configData.IOIfBoardKind < FBC_NONE) || (configData.IOIfBoardKind > FBC_352_BOARD))
        index = 0;
    else
        index = configData.IOIfBoardKind;

    printf( "\t       IO Shelf Config : %s\n", fbcBoardStr[index]);
    printf( "\t             DI Config : 0x%x\n", configData.hvdcDiConfig);
    printf( "\t             DO Config : 0x%x\n", configData.hvdcDoConfig);
    printf( "\t            CCB Config : 0x%x\n", configData.hvdcCcbConfig);
    printf( "\t            VCB Config : 0x%x\n", configData.hvdcVcbConfig);
    printf( "\t             AI Config : 0x%x\n", configData.hvdcAiConfig);
    printf( "\t            ODM Config : 0x%x\n", configData.hvdcOdmConfig);

#ifndef _WINCE
#if (BOARD_TYPE == LKV120_BOARD)
    if( (configData.memoryBoardType < MEM_BOARD_NONE) || (configData.memoryBoardType > MEM_4MB_MRAM))
#else
    if( (configData.memoryBoardType < MEM_BOARD_NONE) || (configData.memoryBoardType > MEM_613_BOARD))
#endif
        index = 5;
    else
        index = configData.memoryBoardType;

    printf( "\t   Memory Board Config : %s\n", memBoardStr[index]);
#endif 

	if( (configData.sysSchedlueType < ONE100US_SCHEDULE_CYCLE) || (configData.sysSchedlueType > ONEMS_SCHEDULE_CYCLE))
        index = 3;
    else
        index = configData.sysSchedlueType;

    printf( "\t   Scheduler Time BASE : %s\n", schedulerStr[index]);

	if(configData.netCnfgInfo.ctrlNetDualStatus == CTRL_NET_LINE_DUAL)
        printf( "\t   Network Line Config : %s\n", "DUAL LINE NETWORK");
    else
        printf( "\t   Network Line Config : %s\n", "SINGLE LINE NETWORK");

	printf("\n");

    printf( "\t        IP ADDR CONFIG : %s\n", ipMakingFlagStr[index]);

    ipAddrHexToString(ipAddr, configData.netCnfgInfo.myIpAddr[0]);
    printf( "\t       MAIN IP ADDRESS : %s\n", ipAddr);

 	printf("\n");

	return;
}

static void bootCmdHelp(uint32 arg1, uint32 arg2)
{
	int8    **pMsg;
    
    int8 *bootHelpMsg[] =
    {
        "[?,h,H]",		"- Print below command list",
        "[cbp,CBP]",	"- Change Board's boot parameters",
        "[dbp,DBP]",	"- Display Board's boot parameters",
        "[ipd,IPD]",	"- Board Ip Address display",
        "[fid,FID]",	"- FB Logic Info Display",
        "[flc,FLD]",	"- FB Logic Clear",
        "[brt,BRT]",	"- Board Rebooting",
        NULL
    };

    printf( "\n");

    for(pMsg = bootHelpMsg; *pMsg != NULL; pMsg += 2)
        printf( "\t%-15s %s\n", *pMsg, *(pMsg+1));
}


static void ipAddressDisplay(uint32 arg1, uint32 arg2)
{
    int8    ipAddr[20];
    uint32  status;

    status = ipAddrGet(NET_DEV_NAME0,ipAddr);
    if(status)
        printf( "Ip Address Get Err\n");
    else
        printf( "\n\t Board IP Address : %s\n", ipAddr);
}


void bootLogoPrint(strSystemBootParaInfo *paraPtr)
{
    uint32 index = 0;

    int8 *configMsg[] =
    {
        "NULL","MASTER","SLAVE","UNKNOWN"
    };

    int8 *boardConfigMsg[] =
    {
        "NULL", "NOT CONFIG", "CONFIG", "UNKNOWN"
    };

    int8 *ipAddrMakingFlagStr[] = 
    {
    	"NULL", "MANUAL", "AUTO", "UNKNOWN"  
    };
    int8 *fbcBoardStr[] =
    {
    	"NULL", "NONE", "FBC311", "FBC351", "FBC601", "FBC352",
    	"UNKNOWN"
	};
    int8 *memBoardStr[] =
    {
    	"MEM_NONE", "MEM611(2M Bytes)","MEM612(4M Bytes)","MEM613(8M Bytes)","MRAM(4M Bytes)",
    	"UNKNOWN"
	};
    int8 *scheTimeStr[] =
    {
    	"NULL", "100us","1ms",
    	"UNKNOWN"
	};
    int8 *hvdcCpuTypeStr[] =
    {
    	"UNKNOWN", "CPU-C", "CPU-P", "CPU-V", "CPU-M"
	};

    int8 *fieldbusIfKindMsg[] =
    {
        "NULL", "NOT CONFIG", "FMC CONFIG", "PROFI-SHELF CONFIG", "UNKNOWN"
    };
    
    int8 *mbSlvMapIfStr[] =
    {
    	"NULL", "DWORD_FIX", "WORD","DWORD_WSWAP",
    	"UNKNOWN"
	};
	
    int8 *dluIfKindMsg[] =
    {
        "NULL", "AUTO CONFIG", "PMC_ETH CONFIG", "VME_DLU CONFIG", "UNKNOWN"
    };
    
    printf( "\n\n");
    printf( "%20s%s", "","=================== Systme Booting Start ====================\n");
    printf( "%20s%s", "", "     Copyright(c) HVDC Simulator Systems, LK11 Co., Ltd.\n\n");

    printf( "\n");
    printf( "%20s%s%s","","             Compile time : ", verDate);
    printf( "\n");

	
    if( (paraPtr->usrBootParaInfo.hvdcCpuType < HVDC_CPU_C_TYPE) || (paraPtr->usrBootParaInfo.hvdcCpuType > HVDC_CPU_M_TYPE)) 
        index = 0;
    else 
        index = paraPtr->usrBootParaInfo.hvdcCpuType;
    	
    printf( "%20s%s%s","","            HVDC CPU TYPE : ",hvdcCpuTypeStr[index]);
    printf( "\n");

    if( (paraPtr->usrBootParaInfo.stationNum >= MIN_STATION_ID) &&
        (paraPtr->usrBootParaInfo.stationNum <= MAX_STATION_ID)
    )
        printf( "%20s%s%d","","                System ID : ",paraPtr->usrBootParaInfo.stationNum);
    else
        printf( "%20s%s%d[%s]","","            System ID : ",
                                paraPtr->usrBootParaInfo.stationNum,
                                "INVALID");

     printf( "\n");

   	if(paraPtr->usrBootParaInfo.ipMakingFlag > AUTO_IPADDR_MAKE) 
   		index = 3;
    else 
    	index = paraPtr->usrBootParaInfo.ipMakingFlag;

    printf( "%20s%s%s","","   System IP Addr. Making : ",ipAddrMakingFlagStr[index]);
    printf( "\n");

    {
	    printf( "%20s%s%s","","          MAIN IP Address : ",paraPtr->netBootParaInfo.netLineAIpAddr);
	    printf( "\n");
	    
#if ( (BOARD_TYPE == X86_PC_BOARD) || (BOARD_TYPE == LKV120_BOARD) )

#if ((BOARD_TYPE == LKV120_BOARD))
		if(gEthDluModuleID != 0) 
		{
#endif
		    printf( "%20s%s%s","","     Dual Link Ip Address : ",paraPtr->usrBootParaInfo.dualLinkIpAddr);
		    printf( "\n");
#if ((BOARD_TYPE == LKV120_BOARD))
        }
#endif
#endif

	}

    if( (paraPtr->usrBootParaInfo.shelfType > SLAVE_SHELF) || (paraPtr->usrBootParaInfo.shelfType < MASTER_SHELF))
        index = 3;
    else
        index = paraPtr->usrBootParaInfo.shelfType;
    printf( "%20s%s%s","","               Shelf Type : ", configMsg[index]);
    printf( "\n");
    
    if( (paraPtr->usrBootParaInfo.dluIfKind < DLU_IF_AUTO) || (paraPtr->usrBootParaInfo.dluIfKind > DLU_IF_VME_DLU))
        index = 4;
    else
        index = paraPtr->usrBootParaInfo.dluIfKind;
    
    printf( "%20s%s%s","","       DUAL LINK I/F Type : ", dluIfKindMsg[index]);
    printf( "\n");
    
	
    if( (paraPtr->usrBootParaInfo.mbSlaveMapIf < MB_SLV_IF_DWORD_FIX) || (paraPtr->usrBootParaInfo.mbSlaveMapIf > MB_SLV_IF_DWORD_WSWAP))
        index = 4;
    else
        index = paraPtr->usrBootParaInfo.mbSlaveMapIf;

    printf( "%20s%s%s","","  Modbus Slave Map Config : ", mbSlvMapIfStr[index]);
    printf( "\n");

    if( (paraPtr->usrBootParaInfo.ioShelfType < FBC_NONE) || (paraPtr->usrBootParaInfo.ioShelfType > FBC_352_BOARD))
        index = 6;
    else
        index = paraPtr->usrBootParaInfo.ioShelfType;

    printf( "%20s%s%s","","          IO Shelf Config : ", fbcBoardStr[index]);
    printf( "\n");

    printf( "%20s%s0x%x\n","","                DI Config : ", paraPtr->usrBootParaInfo.hvdcDiConfig);
    printf( "%20s%s0x%x\n","","                DO Config : ", paraPtr->usrBootParaInfo.hvdcDoConfig);
    printf( "%20s%s0x%x\n","","               CCB Config : ", paraPtr->usrBootParaInfo.hvdcCcbConfig);
    printf( "%20s%s0x%x\n","","               VCB Config : ", paraPtr->usrBootParaInfo.hvdcVcbConfig);
    printf( "%20s%s0x%x\n","","                AI Config : ", paraPtr->usrBootParaInfo.hvdcAiConfig);
    printf( "%20s%s0x%x\n","","               ODM Config : ", paraPtr->usrBootParaInfo.hvdcOdmConfig);

#ifndef _WINCE
#if (BOARD_TYPE == LKV120_BOARD)
    if( (paraPtr->usrBootParaInfo.memoryBoardType < MEM_BOARD_NONE) || (paraPtr->usrBootParaInfo.memoryBoardType > MEM_4MB_MRAM))
#else
    if( (paraPtr->usrBootParaInfo.memoryBoardType < MEM_BOARD_NONE) || (paraPtr->usrBootParaInfo.memoryBoardType > MEM_613_BOARD))
#endif
        index = 5;
    else
        index = paraPtr->usrBootParaInfo.memoryBoardType;

    printf( "%20s%s%s","","         MEM Board Config : ", memBoardStr[index]);
    printf( "\n");
#endif 
    
    if( (paraPtr->usrBootParaInfo.sysSchedlueType < ONE100US_SCHEDULE_CYCLE)
		|| (paraPtr->usrBootParaInfo.sysSchedlueType > ONEMS_SCHEDULE_CYCLE))
        index = 3;
    else
        index = paraPtr->usrBootParaInfo.sysSchedlueType;

    printf( "%20s%s%s","","      Scheduler Time Base : ", scheTimeStr[index]);
    printf( "\n");

    return;
}

#ifdef _WIN32

static void boardTimeSet(void)
{
    int8    line[32];
    int8    *cPtr;
    SYSTEMTIME st, vt, setTime;
	char* timeString;
	static int days[] = 
	{				
		31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
	uint32 day = 0;

    printf( "\n*****************[SYSTEM TIME SET]*****************\n");
	GetLocalTime(&st);				
	timeString = systoasctime(&st);
    printf( "Current System Time :  %s", timeString);
	
	vt = asctosystime (verDate);    
    printf( "Creation Date Time  :  %s\n\n", verDate);

	while(1L) {
		memoryClear((uint8*)&setTime, sizeof(SYSTEMTIME));
	
		printf(" Enter the Setting YEAR[%d - 2100] ? ",vt.wYear);
		ioRead(gDebugPortFd,line, sizeof(line));
		cPtr = line;
		spaceSkip(&cPtr);
		if(*cPtr == '.')   goto MODULE_END;
		setTime.wYear = atoi((const char *)cPtr);
		if( (setTime.wYear < vt.wYear) || (setTime.wYear > 2100) ) goto TIME_RESET2;

		printf(" Enter the Setting MONTH[1 - 12] ? ");
		ioRead(gDebugPortFd,line, sizeof(line));
		cPtr = line;
		spaceSkip(&cPtr);
		if(*cPtr == '.')   goto MODULE_END;
		setTime.wMonth = atoi((const char *)cPtr);
		if( (setTime.wMonth < 1)||( setTime.wMonth > 12) )  goto TIME_RESET2;
		else if( (setTime.wYear == vt.wYear) &&(setTime.wMonth < vt.wMonth) ) goto TIME_RESET1;

		day =  days[setTime.wMonth-1] + _isleap (setTime.wMonth, setTime.wYear);
		printf(" Enter the Setting DAY[1 - %d] ? ", day	);
		ioRead(gDebugPortFd,line, sizeof(line));
		cPtr = line;
		spaceSkip(&cPtr);
		if(*cPtr == '.')   goto MODULE_END;
		setTime.wDay = atoi((const char *)cPtr);
		if( (setTime.wDay < 1)||( setTime.wDay > day) )  goto TIME_RESET2;
		else if( (setTime.wYear == vt.wYear) &&(setTime.wMonth == vt.wMonth)
			&&(setTime.wDay < vt.wDay) ) 
			goto TIME_RESET1;

		printf(" Enter the Setting HOUR[0 - 23] ? ");
		ioRead(gDebugPortFd,line, sizeof(line));
		cPtr = line;
		spaceSkip(&cPtr);
		if(*cPtr == '.')   goto MODULE_END;
		setTime.wHour = atoi((const char *)cPtr);
		if( (setTime.wHour < 0)||( setTime.wHour > 23) )  goto TIME_RESET2;
		else if( (setTime.wYear == vt.wYear) &&(setTime.wMonth == vt.wMonth)
			&&(setTime.wDay == vt.wDay) &&(setTime.wHour < vt.wHour)) 
			goto TIME_RESET1;

		printf(" Enter the Setting MINUTE[0 - 59] ? ");
		ioRead(gDebugPortFd,line, sizeof(line));
		cPtr = line;
		spaceSkip(&cPtr);
		if(*cPtr == '.')   goto MODULE_END;
		setTime.wMinute = atoi((const char *)cPtr);
		if( (setTime.wMinute < 0)||( setTime.wMinute > 59) )  goto TIME_RESET2;
		else if( (setTime.wYear == vt.wYear) &&(setTime.wMonth == vt.wMonth)
			&&(setTime.wDay == vt.wDay) &&(setTime.wHour == vt.wHour) 
			&& (setTime.wMinute < vt.wMinute)) 
			goto TIME_RESET1;

		printf(" Enter the Setting SECOND[0 - 59] ? ");
		ioRead(gDebugPortFd,line, sizeof(line));
		cPtr = line;
		spaceSkip(&cPtr);
		if(*cPtr == '.')   goto MODULE_END;
		setTime.wSecond = atoi((const char *)cPtr);
		if( (setTime.wSecond < 0)||( setTime.wSecond > 59) )  goto TIME_RESET2;
		else if( (setTime.wYear == vt.wYear) &&(setTime.wMonth == vt.wMonth)
			&&(setTime.wDay == vt.wDay) &&(setTime.wHour == vt.wHour) 
			&& (setTime.wMinute == vt.wMinute)&& (setTime.wSecond < vt.wSecond) ) 
			goto TIME_RESET1;

		if(SetLocalTime(&setTime)) {		
			GetLocalTime(&st);				
			timeString = systoasctime(&st);

			printf( "\nSYSTEM TIME SET SUCCESS!!!\n", verDate);
			printf( "Set System Time :  %s", timeString);
		}
		else goto TIME_RESET2;

		break;
TIME_RESET1:
		 printf( "\nYou Should Change Later Creation Date!!!\n");
TIME_RESET2:
		 printf( "INVALID TIME SET!!!\n\n", verDate);
    } 

MODULE_END:
	return;
}

#endif

static void fbLogicMemoryMemoryClear(uint32 moduleId, uint32 arg1)
{
    int8    line[16], YorN[2];
    int8    *pLine;

	printf(" Are you sure to clear FB Logic ?[Y/N] : ");

	ioRead(gDebugPortFd, line, sizeof(line));

    pLine = line;
    
    spaceSkip(&pLine);
	
	upperToLowerCaseConversion( pLine, strlen(pLine));
	
	if(*pLine == 'n')
	{
		printf("\n\t FB Logic Memory Clear CANCEL !!!\n");
		return;	
	}
	else
	{	
		sysExeInfoMemoryClear(LOGIC_ID);
	
		memoryClear( (uint8 *)gBusMpuFbStartAddr, MAX_MPU_DATA_SIZE);
	
		printf("\n\t FB Logic Memory Clear !!!\n");
	}	

    return;
}


