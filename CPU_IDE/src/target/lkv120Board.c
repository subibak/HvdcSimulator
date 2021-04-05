#include	<stdio.h>
#include	"usrTypes.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include	"sysConfig.h"
#include	"lkv120Board.h"
#include	"usrDefBuf.h"

#include	"osDepend.h"
#include	"usrErrCode.h"
#include	"network.h"

#include	"systemDiag.h"
#include	"vmeDef.h"

#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))

typedef struct 
    {
    int32  	century;               
    int32  	year;                  
    int32  	month;                 
    int32  	day_of_month;          
    int32  	day_of_week;           
    int32  	hour;                  
    int32  	minute;                
    int32  	second;                
    int32 	clock;                 
    int32 	pm;                    
    } strMVME3100RTCInfo;

#endif


uint32 nvRamWrite(int8 *, int32 , int32 );
uint32 nvRamRead(int8 *, int32 , int32 );
uint32 timeOfDayGet(void);
void boardTimeDataRead(strTimeDataInfo *);
uint32 boardTimeDataWrite(strTimeDataInfo *);

uint32 sysAuxTickCountGet(void);
void boardLedBlink (uint8 	, int32	);

uint32	boardEthernetDeviceInit (void);
uint32	networkDeviceInit (uint32 *, strUserBootParaInfo *);
int32	debugConsolePortInit (int32	*portFdPtr);

uint32	dualEthernetDeviceInit (int32 , uint8 *);


extern void HexToBcd(int32 , uint8 *);
#if (BOARD_TYPE == MVME2100_BOARD)
	extern uint8 sysNvRead(uint32);
	extern void sysNvWrite(uint32, uint8 );

#elif ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))
	extern uint8 sysNvRead(uint32, uint32);
	extern void sysNvWrite(uint32, uint8, uint32 );
	
	
	extern int32 sysDs1375RtcGet (strMVME3100RTCInfo * );
	extern int32 sysDs1375RtcSet (strMVME3100RTCInfo * );
	
	
	extern void sysUserLedSet (uint8 , int32);
#elif (BOARD_TYPE == LKV120_BOARD)
    extern STATUS sysRtcGet(struct tm *tp);
    extern STATUS sysRtcSet(struct tm *tp);
    extern void ledOn(int led);
    extern void ledOff(int led);

    extern STATUS sysFramRead(UINT32	offset,char *	buf,UINT32	length);
    extern STATUS sysFramWrite(UINT32	offset,char *	buf,UINT32	length);
#elif (BOARD_TYPE == X86_PC_BOARD)
	extern uint32	nvRamFileRead(uint8*, uint32 , uint32 );
	extern uint32	nvRamFileWrite(uint8*, uint32 , uint32 );
#endif

extern void setErrorCode(uint8 *, uint32, uint8 *, uint32);
extern uint32 ipAddrSet(int8 *, int8 *);
extern uint32 networkDeviceAttach(int32 , int8 *);
extern int32 excConnect(voidfunc_ptr *,voidfunc_ptr);
extern void systemInitErr(uint8 *,uint32,uint8 *,uint32);
extern uint32 BcdToHex (void *, void *, int );
extern void memoryCopy (uint8 *, uint8 *, uint32);
extern int32   ioSysCtrlFunc(int32 , int32 , int32 );
extern void errorCodeSetInISR(uint8 *, uint32, uint8 *, uint32);
extern int32   keyHit(void);
extern void sysTaskDelay (int32 );
extern uint32 stringCompareWithLen(uint8 *, uint8 *, uint32);
extern int32   ioRead(int32 , int8 *, int32 );
extern void memoryClear ( uint8 *, uint32 );


extern void usrBootLineInit( int32);  
extern int32 usrNetInit (int8 *); 
extern void spaceSkip(int8 **);

extern uint32	checkPMCModule(uint32, int32 );
extern int32	systemClockRateGet(void);
extern uint32	systemCnfgInfoRead( strSysConfigInfo *);

#if ((BOARD_TYPE == LKV120_BOARD))
extern uint32	EnetAttach(int unit, char *ipBuf, int netmask);
#endif 

#ifdef _WIN32
extern uint64 GetTickLongCount(void); 
extern time_t mktime (struct tm *);
extern int32 ceDebugConsolPortInit(int32*);

extern char* SYS_ENET_BOOT_LINE_ADDR;
extern char* SYS_ENET_DUAL_LINE_ADDR;
#endif

extern uint32 gEthDluModuleID;

uint8	*DUAL_NET_DEV_NAME;

extern int32	consoleFd;
extern int32	gDebugPortFd;

extern strSystemRunningStatusInfo	gSysRunningStatusInfo;


extern uint32	gSysAuxIntrCount; 

#if (defined(VXWORKS))
extern uint32 gMvme3100Tsec1QHaltErrCount;
extern uint32 gMpc85xxTsec1THaltErrCount;	
extern uint32 gMpc85xxTsec1PrevRecvPackets;

extern uint32 gMvme4100Tsec1LinkReloadCount;
extern uint32 gMvme4100Tsec1LinkReloadErrCount;

extern uint32 gMv4100PortErrFlag;
#endif



uint32 sysAuxTickCountGet(void)
{
	uint32	tickCount = 0;
#if (defined(BOARD_TYPE) && defined(VXWORKS))
	tickCount = gSysAuxIntrCount;
#elif	((BOARD_TYPE==X86_PC_BOARD) && defined(_WIN32))
	tickCount = (uint32)GetTickLongCount();
#endif
	return(tickCount);
}


uint32 nvRamWrite(int8 *dataPtr, int32 offSet, int32 bytesLen)
{
    uint32 status = NO_ERROR;
#if (defined(BOARD_TYPE) && defined(VXWORKS))
	int32	i;
    uint8 	data;

    
    if( (offSet < 0)    ||
        (bytesLen <= 0) ||
        ((offSet+BOOT_PARA_START_ADDR) > NVRAM_END_ADDR)
      )
        status = NVRAM_PARA_ERR;
    else {
    	for(i = 0; i < bytesLen; i++, dataPtr++){
        	data = *dataPtr;

#if (BOARD_TYPE == MVME2100_BOARD)
			sysNvWrite(	(uint32)(offSet + i),
						data
					  );
#elif ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))
			sysNvWrite(	(uint32)(offSet + i),
						data,
						BRD_NVRAM_BASE_ADDR
					  );
#elif ((BOARD_TYPE == MVME2500_BOARD))
			MRAMWrite (	(uint32)(offSet + i),
						data
					  );
#elif ((BOARD_TYPE == LKV120_BOARD))
            if(sysFramWrite((uint32)(offSet + i),&data,1) != OK)
            {
                printf("NVRAM write fail\n");
            }
#endif
		}		
	}
#elif	((BOARD_TYPE==X86_PC_BOARD) && defined(_WIN32))
	status = nvRamFileWrite((uint8*)dataPtr,offSet, bytesLen);
#endif 

	if(status != NO_ERROR)
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);

	return(status);
}


uint32 nvRamRead(int8 *dataPtr, int32 offSet, int32 bytesLen)
{
    uint32 status = NO_ERROR;
#if (defined(BOARD_TYPE) && defined(VXWORKS))
    int32	i;
	uint8	data;

    
    if( (offSet < 0) ||
        (bytesLen <= 0) ||
        ((BOOT_PARA_START_ADDR + offSet) > NVRAM_END_ADDR)
      ){
        status = NVRAM_PARA_ERR;
    }
    else{
		for(i = 0; i < bytesLen; i++, dataPtr++) {
#if (BOARD_TYPE == MVME2100_BOARD)
			data = sysNvRead( (uint32)(offSet + i));
#elif ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))
			data = sysNvRead( (uint32)(offSet + i), BRD_NVRAM_BASE_ADDR);
#elif ((BOARD_TYPE == MVME2500_BOARD))
			data = MRAMRead( (uint32)(offSet + i));
#elif ((BOARD_TYPE == LKV120_BOARD))
            if(sysFramRead((uint32)(offSet + i),&data,1) != OK)
            {
                printf("NVRAM read fail\n");
            }

#endif
            *dataPtr = data;
		}		
    }
#elif	((BOARD_TYPE==X86_PC_BOARD) && defined(_WIN32))
    status  = nvRamFileRead((int8 *)dataPtr,
						(uint32)offSet, 
						(uint32)bytesLen
					   );

#endif
	if(status != NO_ERROR)
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);

    return(status);
}


int32 debugConsolePortInit(int32* portFdPtr)
{
    int32   status = NO_ERROR;
    int32	portFd = 0;
#if (defined(BOARD_TYPE) && defined(VXWORKS))
    portFd = consoleFd; 

    
    if(ioSysCtrlFunc(portFd, MYIOSETOPTIONS, MYOPT_TERMINAL) < 0)
        return(IO_CTRL_ERR);
#elif	((BOARD_TYPE==X86_PC_BOARD) && defined(_WIN32))
    ceDebugConsolPortInit(&portFd);
#endif

	*portFdPtr = portFd;

    return(status);
}



uint32	boardEthernetDeviceInit(void)
{
	uint32	status = NO_ERROR;
	strSysConfigInfo	configData;
    strSystemBootParaInfo	bootData;
    char	buf[64];
    uint8    devName[32], ipAddr[32];
    int32 unitNum;

#if	(BOARD_TYPE != LKV120_BOARD)
	usrBootLineInit(BOOT_COLD);
	if(usrNetInit((int8 *)SYS_ENET_BOOT_LINE_ADDR) != NO_ERROR) {
    	status = BOARD_ETH_DEV_INIT_ERR;
    }
#else
#if 0
	status = EnetAttach(0, sysBootParaInfoPtr->netBootParaInfo.targetIpAddr, 0xffffff00);
    if(status)
    	printf("networkDevice(%s, %d unit) attach Error !!\n", 
    										"motetsec", 0);
#endif
#endif 


#if (BOARD_TYPE == MVME2500_BOARD)
	
	status = nvRamRead((int8 *)&bootData,
						USR_ENET_CONFIG_PARA_OFFSET, 
						USR_ENET_CONFIG_PARA_SIZE
					   );
	if(status){
    	printf("[boardEthernetDeviceInit(%d)] - NVRAM Read Err\n",__LINE__);
	}
    else {
       	systemCnfgDebugInfoSet(&configData, &bootData);
	} 
	
	ipAddrHexToString(ipAddr, configData.netCnfgInfo.myIpAddr[0]);
	unitNum = 0;
	status = networkDeviceAttach( 	unitNum, 
    								(int8*)NET_DEV_NAME
                                );

    if(status)
    	printf("networkDevice(%s, %d unit) attach Error !!\n", 
    										NET_DEV_NAME, unitNum);
	if(status == NO_ERROR){
		memoryClear( (uint8 *)devName, 32);
		sprintf((int8*)devName, "%s%d", NET_DEV_NAME, unitNum);
    	status = ipAddrSet(	devName, 
        					ipAddr
						  );
		if(status)
			printf("ipAddrSet() Error !!  devName(%s%d), %s\n", NET_DEV_NAME, unitNum, ipAddr);
	}

    sprintf (buf,"%s%d inet up add %s netmask %3.3d.%3.3d.%3.3d.%3.3d",
             NET_DEV_NAME,
             unitNum,
             ipAddr,
             ((0xFFFFFF00 & 0xFF000000) >> 24),
             ((0xFFFFFF00 & 0xFF0000) >> 16),
             ((0xFFFFFF00 & 0xFF00) >> 8),
              (0xFFFFFF00 & 0xFF));
    if (ifconfig (buf) == ERROR)
       printf ("Failed to configure %s%d for IPv4 TCP/IP stack.\n", NET_DEV_NAME, unitNum);
#endif

    return(status);
}


uint32	dualEthernetDeviceInit(	int32 unitNum, 
								uint8 *ipAddrPtr
							  )
{
	uint32	status = 1;
#if (defined(BOARD_TYPE) && defined(VXWORKS))
	int8	devName[32];
	uint32 retVal = NO_ERROR;
    char	buf[64];
    
#if ((BOARD_TYPE == MVME2100_BOARD) || (BOARD_TYPE == MVME3100_BOARD))
	retVal = checkPMCModule (PMC661J_END, unitNum);
	if(retVal == NO_ERROR) {
		DUAL_NET_DEV_NAME 	= PMC661J_DEV_NAME;
		printf("PMC661J_END Unit #%d Found !!\n", unitNum);
	}
	else {
		printf("PMC661J_END Unit #%d Not Found !!\n", unitNum);
		retVal = checkPMCModule (PMC682E_END, unitNum);
		if(retVal == NO_ERROR){
			DUAL_NET_DEV_NAME 	= PMC682E_DEV_NAME;

			printf("PMC682E_END Unit #%d Found !!\n", unitNum);
		}
		else {
			printf("PMC682E_END Unit #%d Not Found !!\n", unitNum);

			retVal = checkPMCModule (PMC676TX_END, unitNum);
			if(retVal == NO_ERROR){
				DUAL_NET_DEV_NAME 	= PMC676TX_DEV_NAME;
	
				printf("PMC676TX_END Unit #%d Found !!\n", unitNum);
			}
			else {
				printf("PMC676TX_END Unit #%d Not Found !!\n", unitNum);
				return (retVal);
			}
		}
	}
#elif ((BOARD_TYPE == MVME4100_BOARD) || (BOARD_TYPE == MVME2500_BOARD) || (BOARD_TYPE == LKV120_BOARD))

	DUAL_NET_DEV_NAME = "motetsec";
	unitNum = 1;
#endif

	status = networkDeviceAttach( 	unitNum, 
    								(int8*)DUAL_NET_DEV_NAME
                                );

    if(status)
    	printf("networkDevice(%s, %d unit) attach Error !!\n", 
    										DUAL_NET_DEV_NAME, unitNum);
	if(status == NO_ERROR){
		memoryClear( (uint8 *)devName, 32);
		sprintf((int8*)devName, "%s%d", DUAL_NET_DEV_NAME, unitNum);
    	status = ipAddrSet(	devName, 
        					ipAddrPtr
						  );
		if(status)
			printf("ipAddrSet() Error !!  devName(%s), %s\n", devName, ipAddrPtr);
	}

#if ((BOARD_TYPE == MVME4100_BOARD) || (BOARD_TYPE == MVME2500_BOARD) || (BOARD_TYPE == LKV120_BOARD))
    sprintf (buf,"%s%d inet up add %s netmask %3.3d.%3.3d.%3.3d.%3.3d",
             DUAL_NET_DEV_NAME,
             unitNum,
             ipAddrPtr,
             ((0xFFFFFF00 & 0xFF000000) >> 24),
             ((0xFFFFFF00 & 0xFF0000) >> 16),
             ((0xFFFFFF00 & 0xFF00) >> 8),
              (0xFFFFFF00 & 0xFF));
    if (ifconfig (buf) == ERROR)
       printf ("Failed to configure %s%d for IPv4 TCP/IP stack.\n", DUAL_NET_DEV_NAME, unitNum);
#endif

#elif	((X86_PC_TYPE==X86_PC_CPCI_BOARD) && defined(_WIN32))

	if(status == NO_ERROR)
    	status = ipAddrSet(	DUAL_NET_DEV_NAME0, 
        					ipAddrPtr
						  );
#elif	((X86_PC_TYPE==X86_PC_GENERAL) && defined(_WIN32))
	status = NO_ERROR;
#endif 
    return(status);
}


uint32 networkDeviceInit(uint32 *retLineCnfg, strUserBootParaInfo *usrParaInfoPtr)
{
	uint32	status = NO_ERROR;

	
	status = boardEthernetDeviceInit();
    if(status != NO_ERROR)
		systemInitErr	(	(uint8 *)__FILE__,
        					__LINE__,
                            (uint8 *)"networkDeviceInit",
                            status
						);

	status = dualEthernetDeviceInit	( 
		
#if ((BOARD_TYPE == MVME2100_BOARD) || (BOARD_TYPE == XP86_PC_BOARD))
										1, 
#else
										0,
#endif									
										(uint8 *)usrParaInfoPtr->netLineBIpAddr
									);

	if(status != NO_ERROR) {
    	printf( "Dual Ethernet Device Init Error !!\n");

		
        *retLineCnfg = CTRL_NET_LINE_SINGLE;
	}
    else {
		
        *retLineCnfg = CTRL_NET_LINE_DUAL;
	}

#if ((SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD) && defined(VXWORKS))

#if (BOARD_TYPE != LKV120_BOARD) 
	status = dualEthernetDeviceInit	( 	1, 
										(uint8 *)usrParaInfoPtr->dualLinkIpAddr
									);

	if(status != NO_ERROR) {
    	printf( "DLU Ethernet Device Init Error !!\n");
	}
#else
	if(status == NO_ERROR)
	{
	    if(gEthDluModuleID == USING_LKP410_GEI)
	    {
    		status = EnetAttach(4, usrParaInfoPtr->dualLinkIpAddr, 0xffffff00);
    	    if(status)
    	    {
    	    	printf("networkDevice(%s, %d unit) attach Error !!\n", 
    	    										"gei", 0);
    	    	ledOn(4);  
    	    }
    
    		status = EnetAttach(5, "188.9.212.1", 0xffffff00);
    	    if(status)
    	    	printf("networkDevice(%s, %d unit) attach Error !!\n", 
    	    										"gei", 1);
        }
	    else if(gEthDluModuleID == USING_LKP414_RTG)
	    {
    		status = EnetAttach(6, usrParaInfoPtr->dualLinkIpAddr, 0xffffff00);
    	    if(status)
    	    {
    	    	printf("networkDevice(%s, %d unit) attach Error !!\n", 
    	    										"rtg", 0);
    	        ledOn(4);  
    	    }
    
    		status = EnetAttach(7, "188.9.212.1", 0xffffff00);
    	    if(status)
    	    	printf("networkDevice(%s, %d unit) attach Error !!\n", 
    	    										"rtg", 1);
        }
    	status = 0;
	}
#endif
#endif 

    return(status);
}


uint32 timeOfDayGet(void)
{
   uint32  retVal=0;
   struct  tm  timeData;
#if (defined(VXWORKS))
#if (BOARD_TYPE == MVME2100_BOARD)
    uint8	buf[8];
    strBoardTimeInfo    *tPtr = (strBoardTimeInfo *)&buf[0];
    int8    tmp;

    tPtr->sec = sysNvRead(BRD_TIME_ADR_OFFSET + 1);
    BcdToHex( (void *) &tPtr->sec, (void *)&tmp, 1);
    timeData.tm_sec = (int32)tmp;

    tPtr->min = sysNvRead(BRD_TIME_ADR_OFFSET + 2);
    BcdToHex( (void *) &tPtr->min, (void *)&tmp, 1);
    timeData.tm_min = (int32)tmp;

    tPtr->hour = sysNvRead(BRD_TIME_ADR_OFFSET + 3);
    BcdToHex( (void *) &tPtr->hour, (void *)&tmp, 1);
    timeData.tm_hour = (int32)tmp;

    tPtr->day = sysNvRead(BRD_TIME_ADR_OFFSET + 4);
    BcdToHex( (void *) &tPtr->day, (void *)&tmp, 1);
    
    timeData.tm_wday = (int32)(tmp - 1);

    tPtr->date = sysNvRead(BRD_TIME_ADR_OFFSET + 5);
    BcdToHex( (void *) &tPtr->date, (void *)&tmp, 1);
    timeData.tm_mday = (int32)tmp;

    tPtr->month = sysNvRead(BRD_TIME_ADR_OFFSET + 6);
    BcdToHex( (void *) &tPtr->month, (void *)&tmp, 1);
    
    timeData.tm_mon = (int32)(tmp - 1);

    tPtr->year = sysNvRead(BRD_TIME_ADR_OFFSET + 7);
    BcdToHex( (void *) &tPtr->year, (void *)&tmp, 1);

    if( tmp > 50 )  
    	timeData.tm_year = (int32)tmp; 
	else 
		timeData.tm_year = (int32)(tmp + 100); 

#elif ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))
	int32	status;
	strMVME3100RTCInfo	rtcInfo;
	
	status = sysDs1375RtcGet(&rtcInfo);
	if(status != NO_ERROR)
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, status);
	
    timeData.tm_sec 	= rtcInfo.second;
    timeData.tm_min 	= rtcInfo.minute;
    timeData.tm_hour 	= rtcInfo.hour;
    timeData.tm_wday 	= rtcInfo.day_of_week - 1;
    timeData.tm_mday 	= rtcInfo.day_of_month;
    timeData.tm_mon 	= rtcInfo.month - 1;
    timeData.tm_year 	= rtcInfo.year + 100;
#elif (BOARD_TYPE == LKV120_BOARD)

    int32	status;
    status = sysRtcGet(&timeData);
    if (status != NO_ERROR) {
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, SYSTEM_TIME_READ_ERR);
    }

    timeData.tm_isdst = 0;

#endif
    retVal = mktime(&timeData);
#elif (defined(_WIN32))

   SYSTEMTIME st;

   GetLocalTime(&st);  
   timeData.tm_year = st.wYear - 1900;
   timeData.tm_mon = st.wMonth - 1 ;
   timeData.tm_wday = st.wDayOfWeek;
   timeData.tm_mday = st.wDay;
   timeData.tm_hour = st.wHour;

   timeData.tm_min = st.wMinute;
   timeData.tm_sec = st.wSecond;
 
   timeData.tm_isdst = 0;
   timeData.tm_yday = 0;

   retVal = (uint32)mktime(&timeData);  

#endif
    return(retVal);
}


void boardTimeDataRead(strTimeDataInfo *timePtr)
{  
#if (defined(VXWORKS))
#if (BOARD_TYPE == MVME2100_BOARD)
	uint8	buf[8];
    int8    tmp;
    strBoardTimeInfo    *tPtr = (strBoardTimeInfo *)&buf[0];

#if NOT_USED
    tPtr->msec = sysNvRead(BRD_TIME_ADR_OFFSET + 0);
    BcdToHex( (void *) &tPtr->msec, (void *)&tmp, 1);
#endif
    timePtr->mSec = (uint32)0;

    tPtr->sec = sysNvRead(BRD_TIME_ADR_OFFSET + 1);
    BcdToHex( (void *) &tPtr->sec, (void *)&tmp, 1);
    timePtr->second = (uint32)tmp;

    tPtr->min = sysNvRead(BRD_TIME_ADR_OFFSET + 2);
    BcdToHex( (void *) &tPtr->min, (void *)&tmp, 1);
    timePtr->minute = (uint32)tmp;

    tPtr->hour = sysNvRead(BRD_TIME_ADR_OFFSET + 3);
    BcdToHex( (void *) &tPtr->hour, (void *)&tmp, 1);
    timePtr->hour = (uint32)tmp;

    tPtr->day = sysNvRead(BRD_TIME_ADR_OFFSET + 4);
    BcdToHex( (void *) &tPtr->day, (void *)&tmp, 1);
    
    
    if(tmp == 0)
    	timePtr->day = 0;
    else
    	timePtr->day = (uint32)tmp - 1;

    tPtr->date = sysNvRead(BRD_TIME_ADR_OFFSET + 5);
    BcdToHex( (void *) &tPtr->date, (void *)&tmp, 1);
    timePtr->date = (uint32)tmp;

    tPtr->month = sysNvRead(BRD_TIME_ADR_OFFSET + 6);
    BcdToHex( (void *) &tPtr->month, (void *)&tmp, 1);
    
    timePtr->month = (int32)tmp;

    tPtr->year = sysNvRead(BRD_TIME_ADR_OFFSET + 7);
    BcdToHex( (void *) &tPtr->year, (void *)&tmp, 1);

    if( tmp > 50 )  
    	timePtr->year = (int32)tmp + 1900; 
	else 
		timePtr->year = (int32)(tmp + 2000); 

#elif ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))
	int32	status;
	strMVME3100RTCInfo	rtcInfo;
	
	status = sysDs1375RtcGet(&rtcInfo);
	if(status != NO_ERROR)
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, SYSTEM_TIME_READ_ERR);
	
    timePtr->second 	= rtcInfo.second;
    timePtr->minute 	= rtcInfo.minute;
    timePtr->hour 		= rtcInfo.hour;
    
    if(rtcInfo.day_of_week == 0)
    	timePtr->day		= 0;
    else
    	timePtr->day 		= rtcInfo.day_of_week - 1;
    	
    timePtr->date 		= rtcInfo.day_of_month;
    timePtr->month 		= rtcInfo.month;
    timePtr->year 		= rtcInfo.year + 2000;
#elif (BOARD_TYPE == LKV120_BOARD)

    int32	status;
    struct tm tm;
    status = sysRtcGet(&tm);
    if (status != NO_ERROR) {
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, SYSTEM_TIME_READ_ERR);
    }

    timePtr->hour = tm.tm_hour;
    timePtr->minute = tm.tm_min;
    timePtr->second = tm.tm_sec;
    timePtr->month = tm.tm_mon +  1;
    timePtr->date = tm.tm_mday;
    timePtr->year = tm.tm_year + 1900;
    timePtr->day = tm.tm_wday;
#endif
#elif (defined(_WIN32))
   uint32 status = NO_ERROR;
   SYSTEMTIME st;

   GetLocalTime(&st);  
   timePtr->year = st.wYear;
   timePtr->month = st.wMonth ;
   timePtr->date = st.wDay;
   timePtr->day =  st.wDayOfWeek;
   timePtr->hour = st.wHour;
   timePtr->minute = st.wMinute;
   timePtr->second = st.wSecond;
   timePtr->mSec = st.wMilliseconds;
#endif 
		
	return;
}


uint32 boardTimeDataWrite(strTimeDataInfo *timePtr)
{
	uint32 status = NO_ERROR;

#if (defined(VXWORKS))
#if (BOARD_TYPE == MVME2100_BOARD)
	uint8 tmp;
	uint8 controlData;
    strTimeDataInfo	timeInfo;
    
    controlData = 0x80;

	memoryCopy ((uint8 *)&timeInfo, (uint8 *)timePtr, sizeof(strTimeDataInfo));

	
    sysNvWrite(BRD_TIME_ADR_OFFSET + 0,controlData);
 
    HexToBcd( timeInfo.second, &tmp);
    sysNvWrite(BRD_TIME_ADR_OFFSET + 1, tmp);

    HexToBcd(timeInfo.minute, &tmp);
    sysNvWrite(BRD_TIME_ADR_OFFSET + 2, tmp);

    HexToBcd(timeInfo.hour, &tmp);
    sysNvWrite(BRD_TIME_ADR_OFFSET + 3, tmp);

    
    HexToBcd(timeInfo.day, &tmp);
    sysNvWrite(BRD_TIME_ADR_OFFSET + 4, tmp);

    HexToBcd(timeInfo.date, &tmp);
    sysNvWrite(BRD_TIME_ADR_OFFSET + 5, tmp);

    
    HexToBcd(timeInfo.month, &tmp);
    sysNvWrite(BRD_TIME_ADR_OFFSET + 6, tmp);

    if( timeInfo.year < 2000 )  
    	timeInfo.year -= 1900; 
	else 
		timeInfo.year -= 2000;
 
    HexToBcd(timeInfo.year, &tmp);
    sysNvWrite(BRD_TIME_ADR_OFFSET + 7, tmp);

	
    controlData = 0x0;
    sysNvWrite(BRD_TIME_ADR_OFFSET + 0,controlData);

#elif ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))
	strMVME3100RTCInfo	rtcInfo;
	
    rtcInfo.century		= 21;      

    if( timePtr->year < 2000 )  
    	rtcInfo.year = timePtr->year - 1900; 
	else 
		rtcInfo.year = timePtr->year - 2000;
    
    rtcInfo.month	= timePtr->month;
    
    rtcInfo.day_of_month	= timePtr->date;
    rtcInfo.day_of_week		= timePtr->day; 
    rtcInfo.hour			= timePtr->hour;
    rtcInfo.minute			= timePtr->minute;
    rtcInfo.second			= timePtr->second;
    rtcInfo.clock			= 1;
    rtcInfo.pm				= 0;  

	status = sysDs1375RtcSet(&rtcInfo);
	if(status != NO_ERROR)
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, SYSTEM_TIME_WRITE_ERR);
						
#elif (BOARD_TYPE == LKV120_BOARD)
    struct tm tm;
    tm.tm_hour 	= timePtr->hour;
    tm.tm_min 	= timePtr->minute;
    tm.tm_sec 	=  timePtr->second;
    tm.tm_mon 	= timePtr->month - 1;
    tm.tm_mday 	= timePtr->date;
    tm.tm_year 	= timePtr->year - 1900;
    tm.tm_wday 	= timePtr->day;
    tm.tm_isdst = 0;        

    status = sysRtcSet(&tm);
    if (status != NO_ERROR) 
    {
		setErrorCode(__FILE__,__LINE__,__FUNCTION__, SYSTEM_TIME_WRITE_ERR);
    }
#endif
#elif (defined(_WIN32))
	SYSTEMTIME setTime;
	setTime.wYear = timePtr->year;
	setTime.wMonth = timePtr->month;
	setTime.wDayOfWeek = timePtr->day;
	setTime.wDay = timePtr->date;
	setTime.wHour = timePtr->hour;
	setTime.wMinute = timePtr->minute;
	setTime.wSecond = timePtr->second;
	setTime.wMilliseconds = timePtr->mSec;

	if(SetLocalTime(&setTime)==0)
	{
		status = SYSTEM_TIME_WRITE_ERR;
	}
#endif 
	return(status);
}

void boardLedBlink (uint8 	whichLed, int32	setOnOff)
{
#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))
	sysUserLedSet (
				    whichLed,  
				    setOnOff   
			    );
#endif

	return;
}

void	boardEthernetRegisterCheckAndErrClear(void)
{
#if ((BOARD_TYPE == MVME3100_BOARD))
	uint32 regVal = 0;
	strSysConfigInfo	sysCnfgInfo;

	#define	ETH_RSTAT_ERR_VAL	0x00800000
	#define	ETH_RSTAT_ERR_CLEAR	0x00800000
		
	systemCnfgInfoRead (&sysCnfgInfo);
   		
	regVal = CCSR_READ32 (CCSBAR, CCSR_TSEC1_RSTAT);
	
	if( (regVal & ETH_RSTAT_ERR_VAL) == ETH_RSTAT_ERR_VAL) {
		
		gMvme3100Tsec1QHaltErrCount++;
		CCSR_WRITE32 (CCSBAR, CCSR_TSEC1_RSTAT, ETH_RSTAT_ERR_CLEAR);
		CCSR_WRITE32 (CCSBAR, CCSR_TSEC1_RSTAT, 0x0);
	}
#elif ((BOARD_TYPE == MVME4100_BOARD))
	uint32 regVal = 0;
	uint32 recvPackets = 0;
    char	buf[64];
    	
	#define	MPC8548_ETH_RSTAT_ERR_VAL	0x00FF0000
	#define	MPC8548_ETH_RSTAT_ERR_CLEAR	0x00FF0000
   		
	regVal = CCSR_READ32 (CCSBAR, CCSR_TSEC1_RSTAT);
	
	if( (regVal & MPC8548_ETH_RSTAT_ERR_VAL) != 0) {
		
		gMvme3100Tsec1QHaltErrCount++;
		CCSR_WRITE32 (CCSBAR, CCSR_TSEC1_RSTAT, ((regVal)|(MPC8548_ETH_RSTAT_ERR_CLEAR)));
	}

	#define	MPC8548_ETH_TSTAT_ERR_VAL	0xFF000000
	#define	MPC8548_ETH_TSTAT_ERR_CLEAR	0xFF000000
	
	regVal = CCSR_READ32 (CCSBAR, CCSR_TSEC1_TSTAT);
	recvPackets = CCSR_READ32 (CCSBAR, CCSR_TSEC1_RPKT);
	if( 	((regVal & MPC8548_ETH_TSTAT_ERR_VAL) == 0)
		&&	(recvPackets != gMpc85xxTsec1PrevRecvPackets)) 
	{
		
		gMpc85xxTsec1THaltErrCount++;
		gMv4100PortErrFlag = 1;
	}
	else if (((regVal & MPC8548_ETH_TSTAT_ERR_VAL) != 0)) 
	{
		gMv4100PortErrFlag = 0;
	}
	
	gMpc85xxTsec1PrevRecvPackets = recvPackets;
	
	if(gMpc85xxTsec1THaltErrCount != 0)
	{
		memoryClear( (uint8 *)buf, 64);




		sprintf (buf,"%s%d down", NET_DEV_NAME,0);
		if (ifconfig (buf) == ERROR)
		{
			printf ("Failed to bring %s down.\n",buf);
			gMvme4100Tsec1LinkReloadErrCount++;
		}
		else
		{
			sprintf (buf,"%s%d up", NET_DEV_NAME,0);
			if (ifconfig (buf) == ERROR)
			{
				printf ("Failed to bring %s up.\n",buf);
				gMvme4100Tsec1LinkReloadErrCount++;
			}
			else
			{
				printf ("Success %s re-up\n", buf);
				gMvme4100Tsec1LinkReloadCount++;
				gMpc85xxTsec1THaltErrCount = 0;
			}
		}
	}

#endif
}

#ifdef VXWORKS
void mpc8540Tsec1ErrCntDisplay (void)
{
	printf("\t***********************************************\n");
	printf("\t***    MPC85xx TSEC Register Error Count    ***\n");	
	printf("\t***********************************************\n");
	printf("\tQHLT Count = %d\n", gMvme3100Tsec1QHaltErrCount);
	printf("\tTHLT Count = %d\n", gMpc85xxTsec1THaltErrCount);
	printf("\tTSEC1 Reload Count = %d\n", gMvme4100Tsec1LinkReloadCount);
	printf("\tTSEC1 Reload Error Count = %d\n", gMvme4100Tsec1LinkReloadErrCount);
}
#endif 

/**************************************************************************
**
**  Function 이름   : boardTimeSet
**
**  개요            : board의 시스템 Time을 Set한다. 
**
**  argument        : 없슴
**
**  전역 변수       : 없음
**
**  리턴 값         : 없슴
**
**	[변경이력]
	------	--------	----------------------------------------------	
    변경자 	변경날짜						변경사항
    ------	--------	----------------------------------------------
**************************************************************************/
void boardTimeSet(uint32 arg1, uint32 arg2)
{
    int8    line[32];
    int8    *cPtr;
	static int days[] = {				
		31, 28, 31, 
		30, 31, 30, 
		31, 31, 30, 
		31, 30, 31
	};
	uint32 day = 0;
	strTimeDataInfo setTimeData;
	struct tm tmSetTime;
    uint32	time_tData;
    uint32	status;

    time_tData = (uint32)timeOfDayGet();
	(void)printf( "\n\t* System RTC Time : %s\n", ctime((time_t *)&time_tData));
	

	FOREVER 
	{
		(void)printf(" Enter the Setting YEAR[1900 - 2100] ? ");
		(void)ioRead(gDebugPortFd,line, sizeof(line));
		cPtr = line;
		spaceSkip(&cPtr);
		if(*cPtr == '.') 
		{
			(void)printf("Entering Time Data is Cancelled !!\n");
			return;
		}
		
		setTimeData.year = atoi((const int8 *)cPtr) ;
		if(setTimeData.year > MAX_TIME_DATA_YEAR) 
		{
			(void)printf("[ERROR]-Invalid Time Data Year[%d] > MAX_YEAR[%d]\n",setTimeData.year, MAX_TIME_DATA_YEAR);
			continue; 
		}

		(void)printf(" Enter the Setting MONTH[1 - 12] ? ");
		(void)ioRead(gDebugPortFd,line, sizeof(line));
		cPtr = line;
		spaceSkip(&cPtr);
		if(*cPtr == '.') 
		{
			(void)printf("Entering Time Data is Cancelled !!\n");
			return;
		}
		
		setTimeData.month = atoi((const int8 *)cPtr);
		if( (setTimeData.month < MIN_TIME_DATA_MONTH)||( setTimeData.month > MAX_TIME_DATA_MONTH) )
		{
			(void)printf("[ERROR]-Invalid Time Data month[%d] > MAX_MONTH[%d]\n",setTimeData.month, MAX_TIME_DATA_MONTH);
			continue; 
		}

		day =  days[setTimeData.month-1] + isleap (setTimeData.month, setTimeData.year);
		(void)printf(" Enter the Setting DAY[1 - %d] ? ", day	);
		(void)ioRead(gDebugPortFd,line, sizeof(line));
		cPtr = line;
		spaceSkip(&cPtr);
		if(*cPtr == '.')
		{
			(void)printf("Entering Time Data is Cancelled !!\n");
			return;
		}
		setTimeData.day = atoi((const int8 *)cPtr);
		if( (setTimeData.day < 1)||( setTimeData.day > day) ) 
		{
			(void)printf("[ERROR]-Invalid Time Data day[%d] > MAX_DAY[%d]\n",setTimeData.day, day);
			continue; 
		}

		(void)printf(" Enter the Setting HOUR[0 - 23] ? ");
		(void)ioRead(gDebugPortFd,line, sizeof(line));
		cPtr = line;
		spaceSkip(&cPtr);
		if(*cPtr == '.')
		{
			(void)printf("Entering Time Data is Cancelled !!\n");
			return;
		}
		
		setTimeData.hour = atoi((const int8 *)cPtr);
		if( (setTimeData.hour < MIN_TIME_DATA_HOUR)||( setTimeData.hour > MAX_TIME_DATA_HOUR) )
		{
			(void)printf("[ERROR]-Invalid Time Data Hour[%d] > MAX_HOUR[%d]\n",setTimeData.hour, MAX_TIME_DATA_HOUR);
			continue; 
		}

		(void)printf(" Enter the Setting MINUTE[0 - 59] ? ");
		(void)ioRead(gDebugPortFd,line, sizeof(line));
		cPtr = line;
		spaceSkip(&cPtr);
		if(*cPtr == '.')
		{
			(void)printf("Entering Time Data is Cancelled !!\n");
			return;
		}
		
		setTimeData.minute = atoi((const int8 *)cPtr);
		if( (setTimeData.minute < MIN_TIME_DATA_MIN)||( setTimeData.minute > MAX_TIME_DATA_MIN) )
		{
			(void)printf("[ERROR]-Invalid Time Data Minute[%d] > MAX_MONTH[%d]\n",setTimeData.minute, MAX_TIME_DATA_MIN);
			continue; 
		}

		(void)printf(" Enter the Setting SECOND[0 - 59] ? ");
		(void)ioRead(gDebugPortFd,line, sizeof(line));
		cPtr = line;
		spaceSkip(&cPtr);
		if(*cPtr == '.')
		{
			(void)printf("Entering Time Data is Cancelled !!\n");
			return;
		}
		
		setTimeData.second = atoi((const int8 *)cPtr);
		if( (setTimeData.second < MIN_TIME_DATA_SEC)||( setTimeData.second > MAX_TIME_DATA_SEC) ) 
		{
			(void)printf("[ERROR]-Invalid Time Data second[%d] > MAX_SECOND[%d]\n",setTimeData.second, MAX_TIME_DATA_SEC);
			continue; 
		}

        tmSetTime.tm_sec 	= setTimeData.second;
        tmSetTime.tm_min 	= setTimeData.minute;
        tmSetTime.tm_hour 	= setTimeData.hour;
        tmSetTime.tm_mday 	= setTimeData.day;
        tmSetTime.tm_mon 	= setTimeData.month-1;
        tmSetTime.tm_year 	= setTimeData.year - REF_TIME_DATA_YEAR;
        
#if (BOARD_TYPE == CCVME717_BOARD) 
        status = rtc_time_write ((struct tm *)&tmSetTime);
#endif

#if (BOARD_TYPE == LKV120_BOARD) 
        status = sysRtcSet ((struct tm *)&tmSetTime);
#endif
        if(status == NO_ERROR) 
        {
           (void)systemClockTimeSet();
           time_tData = timeOfDayGet();
           (void)printf( "\n\t* Read System Time : %s\n", ctime((time_t *)&time_tData));          
        }
        else
        {
            (void)printf("[ERROR]- rtc_time_write() Call Error[0x%x]\n", status);
            continue;
        }
		break;
    } /* end of while */

	return;
}
