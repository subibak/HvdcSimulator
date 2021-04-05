#include 	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include 	"pcsDefs.h"
#include 	"sysConfig.h"
#include 	"modbustcp\mbtServer.h"
#include 	"modbustcp\mbtServerProto.h"
#include 	"lkv120Board.h"
#include 	"rmt_Comm\ethModbus.h"


extern int32 gDebugPortFd;
extern strMBClntAndSlvDevInfo	gMBClntAndSlvDevInfo; 


extern int32   ioRead(int32 , int8 *, int32 );


static void mbtsDebug_HelpCommand(void)
{
    char **pMsg;
    char *helpMsg[] =
    {
        "[?,h,H]",  "- Print below command list.",
        "[q,Q]",	"- Exit ModBus-Tcp-Server Debugger",
		"[cc,CC]",  "- MBTS Configuration Change",
		"[cd,CD]",  "- MBTS Configuration Display",
		"[id,ID]",  "- MBTS Information Display",
		"[ps,PS]",  "- MBTS Packet Display Set",
		"[pr,PR]",  "- MBTS Packet Display Reset",
		NULL
    };

    fdprintf(gDebugPortFd, "\n");

    for(pMsg = helpMsg; *pMsg != NULL; pMsg += 2)
        fdprintf(gDebugPortFd, "\t%-15s %s\n", *pMsg, *(pMsg+1));
	
	return;
}



static void mbtcDebug_HelpCommand(void)
{
    char **pMsg;
    char *helpMsg[] =
    {
        "[?,h,H]",  "- Print below command list.",
        "[q,Q]",	"- Exit ModBus-Tcp-Client Debugger",
		"[cd,CD]",  "- MBTC Configuration Display",
		"[ld,LD]",  "- MBTC Network Live Count Display",
		NULL
    };

    fdprintf(gDebugPortFd, "\n");

    for(pMsg = helpMsg; *pMsg != NULL; pMsg += 2)
        fdprintf(gDebugPortFd, "\t%-15s %s\n", *pMsg, *(pMsg+1));
	
	return;
}


static void mbtsDebug_DisplayConfig (CFG_MBT_SERVER_ST *pCfg)
{
	int index;
	CFG_DEV_MBTS_ST *pC;

	fdprintf (gDebugPortFd,
			"\n************* MBTS Configuration *************\n\n");
	fdprintf (gDebugPortFd,
			"  Number of MBTS : %d\n", pCfg->iNumDevMbts);
	for (index=0; index < pCfg->iNumDevMbts; index++)
	{
		pC = &pCfg->arDevMbts[index];
		fdprintf (gDebugPortFd, "\n"
			"    MBTS Number       : %d\n", index+1);
		fdprintf (gDebugPortFd,
			"    MAX Channel       : %d\n", pC->iChannelMax);
		fdprintf (gDebugPortFd,
			"    TCP Port          : %d\n", pC->uTcpPort);
		fdprintf (gDebugPortFd,
			"    RX  Frame Timeout : %d milliseconds\n", pC->iRxFrameTimeout);
		fdprintf (gDebugPortFd,
			"    Comm Idle Timeout : %d seconds\n\n", pC->iMaxCommIdleTime);
	}
}



static void mbtcDebug_DisplayConfig (strMBClntAndSlvDevInfo *pCfg)
{
	int index;
	uint32 stationId = 0; 
	uint32 netIdx = 0;
	strModbusTcpDevInfo *pC;
	uint8 strIPAddr[32];
	static uint8    *lineKind[] =
	{
		(uint8 *)"MASTER A Line", 
		(uint8 *)"MASTER B Line", 
		(uint8 *)"SLAVE A Line",
		(uint8 *)"SLAVE B Line",  
		NULL
	};

	fdprintf (gDebugPortFd,
			"\n************* MBTC Configuration *************\n\n");
	fdprintf (gDebugPortFd,
		"  Number of MBTC : %d\n", pCfg->numOfSlave);

	for (index=0; index < MODBUS_TCP_MAX_SLAVE_NUM; index++)
	{
		pC = &pCfg->slaveDeviceListInfo[index];

		stationId = pC->slaveDeviceId;
		if( (stationId == 0) || (stationId > MODBUS_TCP_MAX_SLAVE_NUM))
		{
				continue;
		}

		fdprintf (gDebugPortFd, "\n"
			"    MBTC Station ID         : %d\n", index+1);

		for(netIdx=0; netIdx< MODBUS_TCP_MAX_CNET; netIdx++) 
		{
			if(pC->ipAddrInfoList[netIdx] == 0) continue;

			memset(strIPAddr, 0x0, sizeof(strIPAddr)); 
			ipAddrHexToString((int8 *)strIPAddr, (uint32)pC->ipAddrInfoList[netIdx]);

			fdprintf (gDebugPortFd,
				"    [%s] IP Addr : %s \n", lineKind[netIdx], strIPAddr);
			fdprintf (gDebugPortFd,
				"                 TCP Socket : %d \n", pC->tcpClntSock[netIdx]);
			fdprintf (gDebugPortFd,
				"                 Live Count : %d \n", pC->dataRecvLiveCount[netIdx]);
			fdprintf (gDebugPortFd,
				"               Is Connected : %d \n", pC->isConnected[netIdx]);
			fdprintf (gDebugPortFd,
				"                 Check Time : %d \n\n", pC->lastStatCheckTime[netIdx]);
		}
	}
}



static void mbtcDebug_DisplayLiveCnt (strMBClntAndSlvDevInfo *pCfg)
{
	int index;
	uint32 stationId = 0; 
	strModbusTcpDevInfo *pC;
	static uint8    *lineKind[] =
	{
		(uint8 *)"MASTER A Line", 
		(uint8 *)"MASTER B Line", 
		(uint8 *)"SLAVE A Line",
		(uint8 *)"SLAVE B Line",  
		NULL
	};

	int8	line[16], *cPtr;

RETRY:
	fdprintf (gDebugPortFd, "\t Input Station ID [1 ~ %d] : ", MODBUS_TCP_MAX_SLAVE_NUM);

	ioRead(gDebugPortFd, line, sizeof(line));
    cPtr = line;
    spaceSkip(&cPtr);

	stationId = atoi((const char *)cPtr); 
    
    if( (stationId == 0) || (stationId > MODBUS_TCP_MAX_SLAVE_NUM)) 
	{
		fdprintf (gDebugPortFd,"\t [Error] - Station ID!!! \n ");
		goto RETRY;
	}
	
	for (index=0; index < MODBUS_TCP_MAX_SLAVE_NUM; index++)
	{
		pC = &pCfg->slaveDeviceListInfo[index];

		if(stationId == pC->slaveDeviceId)
			break;
	}

	while(!keyHit())
	{
		fdprintf (gDebugPortFd,"\t***************************************************************\n");
		fdprintf (gDebugPortFd,"\t**	       SLAVE COMMUNCATION STATUS \n");
		fdprintf (gDebugPortFd,"\t***************************************************************\n");
		fdprintf (gDebugPortFd,"\t*                     STATION ID : %d\n", pC->slaveDeviceId);

		fdprintf (gDebugPortFd,"\t*   %s Response Count : %d\n", lineKind[0], pC->dataRecvLiveCount[0]);
		fdprintf (gDebugPortFd,"\t*   %s Response Count : %d\n", lineKind[1], pC->dataRecvLiveCount[1]);
		fdprintf (gDebugPortFd,"\t*    %s Response Count : %d\n", lineKind[2], pC->dataRecvLiveCount[2]);
		fdprintf (gDebugPortFd,"\t*    %s Response Count : %d\n", lineKind[3], pC->dataRecvLiveCount[3]);

		sysTaskDelay(systemClockRateGet()/1);
	}
}



static void mbtsDebug_ChangeConfig (void)
{
    int8 line[128];
	int iVal, iMin, iMax;
	CFG_MBT_SERVER_ST tmpCfg;
	int index;
	CFG_DEV_MBTS_ST *pC;

	fdprintf (gDebugPortFd,
			"\n*************[MBTS Configuration Change]*************\n\n");
	memcpy (&tmpCfg, &g_MbtServerCfg, sizeof(tmpCfg));

__CHANGE_CONFIG_iNumDevMbts:
	fdprintf (gDebugPortFd,
		"  Number of MBTS : %d ? [0 ~ %d] : ", tmpCfg.iNumDevMbts, MAX_DEV_MBTS);
	ioRead (gDebugPortFd, line, sizeof(line));
	if (line[0] == 'q' || line[0] == 'Q')
		goto __CHANGE_CONFIG_QUIT;
	if (line[0] != ENDOFSTR)
	{
		if ((sscanf ((char *)line, "%d", &iVal) == 1) &&
			(iVal >= 0 && iVal <= MAX_DEV_MBTS))
		{
			
			tmpCfg.iNumDevMbts = iVal;
		}
		else
		{
			fdprintf (gDebugPortFd, "\tInvalid Input !!\n");
			goto __CHANGE_CONFIG_iNumDevMbts;
		}
	}
	for (index=0; index < tmpCfg.iNumDevMbts; index++)
	{
		pC = &tmpCfg.arDevMbts[index];
__CHANGE_CONFIG_iChannelMax:
		if (pC->iChannelMax <= 0 || pC->iChannelMax > MAX_CHANNEL_MBTS)
			pC->iChannelMax = 1;	
		fdprintf (gDebugPortFd,
			"\n    MBTS#%d MAX Channel       : %d ? [1 ~ %d] : ",
			index+1, pC->iChannelMax, MAX_CHANNEL_MBTS);
		ioRead (gDebugPortFd, line, sizeof(line));
		if (line[0] == 'q' || line[0] == 'Q')
			goto __CHANGE_CONFIG_QUIT;
		if (line[0] != ENDOFSTR)
		{
			if ((sscanf ((char *)line, "%d", &iVal) == 1) &&
				(iVal >= 1 && iVal <= MAX_CHANNEL_MBTS))
			{
				
				pC->iChannelMax = iVal;
			}
			else
			{
				fdprintf (gDebugPortFd, "\tInvalid Input !!\n");
				goto __CHANGE_CONFIG_iChannelMax;
			}
		}
__CHANGE_CONFIG_uTcpPort:
		iMin = 1;
		iMax = 65535;
		if (pC->uTcpPort < iMin || pC->uTcpPort > iMax)
			pC->uTcpPort = MBT_TCP_PORT;	
		fdprintf (gDebugPortFd,
			"    MBTS#%d TCP Port          : %d ? [%d ~ %d] : ",
			index+1, pC->uTcpPort, iMin, iMax);
		ioRead (gDebugPortFd, line, sizeof(line));
		if (line[0] == 'q' || line[0] == 'Q')
			goto __CHANGE_CONFIG_QUIT;
		if (line[0] != ENDOFSTR)
		{
			if ((sscanf ((char *)line, "%d", &iVal) == 1) &&
				(iVal >= iMin && iVal <= iMax))
			{
				
				pC->uTcpPort = iVal;
			}
			else
			{
				fdprintf (gDebugPortFd, "\tInvalid Input !!\n");
				goto __CHANGE_CONFIG_uTcpPort;
			}
		}
__CHANGE_CONFIG_iRxFrameTimeout:
		iMin = 1000;
		iMax = 10000;
		if (pC->iRxFrameTimeout < iMin || pC->iRxFrameTimeout > iMax)
			pC->iRxFrameTimeout = 3000;	
		fdprintf (gDebugPortFd,
			"    MBTS#%d RX  Frame Timeout : %d milliseconds ? [%d ~ %d] : ",
			index+1, pC->iRxFrameTimeout, iMin, iMax);
		ioRead (gDebugPortFd, line, sizeof(line));
		if (line[0] == 'q' || line[0] == 'Q')
			goto __CHANGE_CONFIG_QUIT;
		if (line[0] != ENDOFSTR)
		{
			if ((sscanf ((char *)line, "%d", &iVal) == 1) &&
				(iVal >= iMin && iVal <= iMax))
			{
				
				pC->iRxFrameTimeout = iVal;
			}
			else
			{
				fdprintf (gDebugPortFd, "\tInvalid Input !!\n");
				goto __CHANGE_CONFIG_iRxFrameTimeout;
			}
		}
__CHANGE_CONFIG_iMaxCommIdleTime:
		iMin = 1; 
		iMax = 86400;
		if (pC->iMaxCommIdleTime < iMin || pC->iMaxCommIdleTime > iMax)
			pC->iMaxCommIdleTime = 3;	
		fdprintf (gDebugPortFd,
			"    MBTS#%d Comm Idle Timeout : %d seconds ? [%d ~ %d] : ",
			index+1, pC->iMaxCommIdleTime, iMin, iMax);
		ioRead (gDebugPortFd, line, sizeof(line));
		if (line[0] == 'q' || line[0] == 'Q')
			goto __CHANGE_CONFIG_QUIT;
		if (line[0] != ENDOFSTR)
		{
			if ((sscanf ((char *)line, "%d", &iVal) == 1) &&
				(iVal >= iMin && iVal <= iMax))
			{
				
				pC->iMaxCommIdleTime = iVal;
			}
			else
			{
				fdprintf (gDebugPortFd, "\tInvalid Input !!\n");
				goto __CHANGE_CONFIG_iMaxCommIdleTime;
			}
		}
	}	

	if (memcmp (&tmpCfg, &g_MbtServerCfg, sizeof(tmpCfg)))
	{
		mbtsDebug_DisplayConfig (&tmpCfg);
		fdprintf (gDebugPortFd, "\tDo you really change 'MBTS Configuration' ? [Y/N] : ");
		ioRead (gDebugPortFd, line, sizeof(line));
		if (line[0] == 'y' || line[0] == 'Y')
		{
			
			if (nvRamWrite ((int8 *) &tmpCfg,
							USR_MBTS_CONFIG_OFFSET, sizeof(tmpCfg)) == NO_ERROR)
			{
				fdprintf (gDebugPortFd, "\t'MBTS Configuration' is saved at NV-RAM  !!\n");
				fdprintf (gDebugPortFd, "\tTo apply the changes, you should reboot the board  !!\n");
			}
			else
				fdprintf (gDebugPortFd, "\t'MBTS Configuration' is NOT saved at NV-RAM  !!\n");
		}
		else
			goto __CHANGE_CONFIG_QUIT;
	}
	else
		fdprintf (gDebugPortFd, "\t'MBTS Configuration' is not changed  !!\n");
	return;
__CHANGE_CONFIG_QUIT:
	fdprintf (gDebugPortFd, "\t'MBTS Configuration Change' is canceled  !!\n");
	return;
}


static void mbtsDebug_DisplayInfo (void)
{
	int index, j;

	INF_DEV_MBTS_ST *pI;
	INF_CHANNEL_MBTS_ST *pCh;
	TMWTYPES_BOOL bActive = IsActiveSystem ();

	fdprintf (gDebugPortFd,
			"\n************* MBTS Information *************\n\n");
	fdprintf (gDebugPortFd,
			"  System Status  : %s\n", bActive ? "ACTIVE" : "STANDBY");
	fdprintf (gDebugPortFd,
			"  Number of MBTS : %d\n", g_MbtServerCfg.iNumDevMbts);
	for (index=0; index < g_MbtServerCfg.iNumDevMbts; index++)
	{

		pI = &g_MbtsInf[index];
		fdprintf (gDebugPortFd, "\n"
			"    MBTS Number             : %d\n", pI->iIndex+1);
		fdprintf (gDebugPortFd,
			"    Listen Socket FD        : %d %s\n",
			pI->listenFd,
			(pI->listenFd <= 0 && bActive) ? "<ERROT>" : "");
		fdprintf (gDebugPortFd,
			"    Connected Channel Count : %d\n", pI->channelCount);

		pCh = &pI->tChan[0];
		for (j=0; j<MAX_CHANNEL_MBTS; j++, pCh++)
		{
			if (pCh->commFd <= 0)
				continue;
			fdprintf (gDebugPortFd, "\n"
				"      Channel Number         : %d\n", pCh->iIndex+1);
			fdprintf (gDebugPortFd,
				"      Peer IP                : %s\n", pCh->peerIp);
			fdprintf (gDebugPortFd,
				"      Packet Display Flag    : %c\n",
				pCh->diag.logDispFlag ? 'Y' : 'N');
			fdprintf (gDebugPortFd,
				"      Frame Sent Count       : %d\n", pCh->commStat.frameSentCount);
			fdprintf (gDebugPortFd,
				"      Frame Received Count   : %d\n", pCh->commStat.frameReceivedCount);
			fdprintf (gDebugPortFd,
				"      Byte  Sent Count       : %d\n", pCh->commStat.byteSentCount);
			fdprintf (gDebugPortFd,
				"      Byte  Received Count   : %d\n", pCh->commStat.byteReceivedCount);
			fdprintf (gDebugPortFd,
				"      Byte  Recv Error Count : %d\n", pCh->commStat.byteReceivedErrorCount);
		}
	}
	fdprintf (gDebugPortFd, "\n");
}


static void mbtsDebug_ResetPacketDisplay (int print)
{
	int index, j;
	INF_DEV_MBTS_ST *pI;
	INF_CHANNEL_MBTS_ST *pCh;

	if (print)
		fdprintf (gDebugPortFd,
				"\n************* Reset Packet Display Flag *************\n\n");
	for (index=0; index < g_MbtServerCfg.iNumDevMbts; index++)
	{
		pI = &g_MbtsInf[index];
		pCh = &pI->tChan[0];
		for (j=0; j<MAX_CHANNEL_MBTS; j++, pCh++)
		{
			if (pCh->diag.logDispFlag)
			{
				if (print)
					fdprintf (gDebugPortFd,
						"\tMBTS#%d-%d Reset Packet Display Flag\n\n",
						pI->iIndex+1, pCh->iIndex+1);
				pCh->diag.logDispFlag = 0;
			}
		}
	}
}



static void mbtsDebug_SetPacketDisplay (void)
{
    int8 line[128];
	int iVal;
	int j, indexMbts, indexCh;
	INF_DEV_MBTS_ST *pI;
	INF_CHANNEL_MBTS_ST *pCh;

	fdprintf (gDebugPortFd,
			"\n************* Set Packet Display Flag *************\n\n");

	indexMbts = indexCh = 0;
	if (g_MbtServerCfg.iNumDevMbts == 0)
	{
		fdprintf (gDebugPortFd, "\tThere is no MBTS !!\n\n");
		return;
	}
	else if (g_MbtServerCfg.iNumDevMbts > 1)
	{
__SET_PACKET_indexMbts:
		fdprintf (gDebugPortFd,
			"  Select MBTS = 1 ? [1 ~ %d] : ", g_MbtServerCfg.iNumDevMbts);
		ioRead (gDebugPortFd, line, sizeof(line));
		if (line[0] == 'q' || line[0] == 'Q')
			goto __SET_PACKET_QUIT;
		if (line[0] != ENDOFSTR)
		{
			if ((sscanf ((char *)line, "%d", &iVal) == 1) &&
				(iVal >= 1 && iVal <= MAX_DEV_MBTS))
			{
				indexMbts = iVal - 1;
			}
			else
			{
				fdprintf (gDebugPortFd, "\tInvalid Input !!\n");
				goto __SET_PACKET_indexMbts;
			}
		}
	}
	pI = &g_MbtsInf[indexMbts];
	if (pI->channelCount == 0)
	{
		fdprintf (gDebugPortFd, "\tThere is no active channel at MBTS#%d !!\n\n", indexMbts+1);
		return;
	}
	else if (pI->channelCount == 1)
	{
		
		pCh = &pI->tChan[0];
		for (j=0; j<MAX_CHANNEL_MBTS; j++, pCh++)
		{
			if (pCh->commFd > 0)
			{
				indexCh = j;
			}
		}
	}
	else
	{
__SET_PACKET_indexCh:
		fdprintf (gDebugPortFd,
			"  Select Chennel of MBTS#%d = 1 ? [1 ~ %d] : ", indexMbts+1, MAX_CHANNEL_MBTS);
		ioRead (gDebugPortFd, line, sizeof(line));
		if (line[0] == 'q' || line[0] == 'Q')
			goto __SET_PACKET_QUIT;
		if (line[0] != ENDOFSTR)
		{
			if ((sscanf ((char *)line, "%d", &iVal) == 1) &&
				(iVal >= 1 && iVal <= MAX_CHANNEL_MBTS))
			{
				indexCh = iVal - 1;
				pCh = &pI->tChan[indexCh];
				if (pCh->commFd <= 0)
				{
					fdprintf (gDebugPortFd, "\tSelected channel is not Active !!\n");
					goto __SET_PACKET_indexCh;
				}
			}
			else
			{
				fdprintf (gDebugPortFd, "\tInvalid Input !!\n");
				goto __SET_PACKET_indexCh;
			}
		}
	}
	
	mbtsDebug_ResetPacketDisplay (0);
	
	pI = &g_MbtsInf[indexMbts];
	pCh = &pI->tChan[indexCh];
	if (pCh->commFd <= 0)
	{
		fdprintf (gDebugPortFd, "\tMBTS#%d-%d channel is not Active !!\n\n", indexMbts+1, indexCh+1);
		return;
	}
	
	fdprintf (gDebugPortFd,
		"\n\tMBTS#%d-%d Set Packet Display Flag\n\n",
		pI->iIndex+1, pCh->iIndex+1);
	pCh->diag.logDispFlag = 1;
	return;
__SET_PACKET_QUIT:
	fdprintf (gDebugPortFd, "\t'Set Packet Display Flag' is canceled  !!\n\n");
	return;
}



void mbtsDebugger (uint32 arg1, uint32 arg2)
{
    int8    line[16], *pLine;
    static char    debugPrompt[] = "[MBTS_BUG]> ";
    static char    defaultStr[] = "Unrecognized Command. Type '?','h','H' for Help\n";

	
    for(;;)	{
        fdprintf(gDebugPortFd,debugPrompt);
        ioRead(gDebugPortFd, line, sizeof(line));
        pLine = line;

        switch(*(pLine)) {
            case ENDOFSTR:
            break;
            case '?':case 'h':case 'H':
                mbtsDebug_HelpCommand ();
                fdprintf(gDebugPortFd, "\n");
			break;
			case 'q': case 'Q':
            	pLine++;
                if(*pLine == ENDOFSTR){
                	fdprintf(gDebugPortFd,"\nReturn To Main Debugger !!!\n");
					return;
				}
                else
	                fdprintf(gDebugPortFd,"%s",defaultStr);
            break;
			case 'c': case 'C':
            	pLine++;
                if( (*pLine == 'd') || (*pLine == 'D')) {
					mbtsDebug_DisplayConfig (&g_MbtServerCfg);
				}
                else if( (*pLine == 'c') || (*pLine == 'C')){
					mbtsDebug_ChangeConfig ();
				}
                else
	                fdprintf(gDebugPortFd,"%s",defaultStr);
            break;
			case 'i': case 'I':
            	pLine++;
                if( (*pLine == 'd') || (*pLine == 'D')) {
					mbtsDebug_DisplayInfo ();
				}
                else
	                fdprintf(gDebugPortFd,"%s",defaultStr);
            break;
			case 'p': case 'P':
            	pLine++;
                if( (*pLine == 's') || (*pLine == 'S')) {
					mbtsDebug_SetPacketDisplay ();
				}
                else if( (*pLine == 'r') || (*pLine == 'R')){
					mbtsDebug_ResetPacketDisplay (1);
				}
                else
	                fdprintf(gDebugPortFd,"%s",defaultStr);
            break;
			default:
                fdprintf(gDebugPortFd,"%s",defaultStr);
            break;
        }
    }
}



void mbtcDebugger (uint32 arg1, uint32 arg2)
{
    int8    line[16], *pLine;
    static char    debugPrompt[] = "[MBTC_BUG]> ";
    static char    defaultStr[] = "Unrecognized Command. Type '?','h','H' for Help\n";
	
    for(;;)	{
        fdprintf(gDebugPortFd,debugPrompt);
        ioRead(gDebugPortFd, line, sizeof(line));
        pLine = line;

        switch(*(pLine)) {
            case ENDOFSTR:
            break;
            case '?':case 'h':case 'H':
                mbtcDebug_HelpCommand ();
                fdprintf(gDebugPortFd, "\n");
			break;
			case 'q': case 'Q':
            	pLine++;
                if(*pLine == ENDOFSTR){
                	fdprintf(gDebugPortFd,"\nReturn To Main Debugger !!!\n");
					return;
				}
                else
	                fdprintf(gDebugPortFd,"%s",defaultStr);
            break;
			case 'c': case 'C':
            	pLine++;
                if( (*pLine == 'd') || (*pLine == 'D')) {
					mbtcDebug_DisplayConfig (&gMBClntAndSlvDevInfo);
				}
                else
	                fdprintf(gDebugPortFd,"%s",defaultStr);
            break;
			case 'l': case 'L':
            	pLine++;
                if( (*pLine == 'd') || (*pLine == 'D')) {
					mbtcDebug_DisplayLiveCnt (&gMBClntAndSlvDevInfo);
				}
                else
	                fdprintf(gDebugPortFd,"%s",defaultStr);
            break;
			default:
                fdprintf(gDebugPortFd,"%s",defaultStr);
            break;
        }
    }
}
