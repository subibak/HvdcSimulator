

#include "DebugCmdTable.h"





#include "DebugCmdParser.h"
#include "DebugCmdExe.h"
#include "Hal/HalFlash.h"
#include "Hal/HalWdt.h"

extern uint32_t DebugDEFAULTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugTESTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugPRDCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugFACTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugFLTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugFLTRCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugMSTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugFCOMMCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugFCOMMCNTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugXmodemCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugAICmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugSYSINITCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugSYSCFGCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugMESIDCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugFCOMMTESTmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

#ifdef FEATURE_PERIPHERAL_TEST
extern uint32_t DebugVMETESTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);
#endif

extern uint32_t DebugCFGINITCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugSWUPDATECmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugRESETCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);
extern uint32_t DebugREBOOTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugSLVSTSCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);
extern uint32_t DebugSDLYCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

#ifdef FEATURE_DEBUG_VARIABLE
extern uint32_t DebugDBGRCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);
extern uint32_t DebugDBGWCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);
#endif

uint32_t DebugDIAGTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);


extern uint32_t DebugDCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);
extern uint32_t DebugFCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);
extern uint32_t DebugVERCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);
extern uint32_t DebugDLYTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);
extern uint32_t DebugIODLYCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugMODECmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);

extern uint32_t DebugHELPCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex);


DebugCmdEntryType DebugCmdTable[] =
{
	// Length of Cmd has to be less than 10 (NULL Termination is necessary)
	{"SYSCFG"	,6		,DebugSYSCFGCmdHandler, "System Config (0:MES, 1:CCS, 2:VBC)\r\n",	"USAGE: SYSCFG? or SYSCFG=0|1|2"},	// 10
	{"SYSINIT"	,7		,DebugSYSINITCmdHandler, "System Init (0:MES(LS), 1:CCS, 2:VBC, 3:MES(HS))\r\n",	"USAGE: SYSINIT=0|1|2|3"},	// 10
	{"CFGINIT"	,7		,DebugCFGINITCmdHandler,	"Init Configuration\r\n",				"USAGE: CFGINIT"},
	{"MST"		,3		,DebugMSTCmdHandler,		"Sync Master(0:Master, 1:Slave)\r\n",	"USAGE: MST? or MST=0|1"},
//	{"FACT"		,4		,DebugFACTCmdHandler,		"Factory Init State(0:Not Done, 1:Done)\r\nUSAGE: FACT? or FACT=0|1"},		// 5
	{"MODE"		,4		,DebugMODECmdHandler,		"Measurement System Mode(0:High Speed, 1:Low Speed)\r\n","USAGE: MODE? or MODE=0|1"},

	{"XMODEM"	,6		,DebugXmodemCmdHandler,		"Start Xmodem\r\n","USAGE: XMODEM"},
	{"SWUPDATE"	,8		,DebugSWUPDATECmdHandler, "Sw Update\r\n","USAGE: SWUPDATE"},
	{"REBOOT"	,6		,DebugREBOOTCmdHandler, "System Warm Reboot\r\n","USAGE: REBOOT"},

	{"AI"		,2		,DebugAICmdHandler, "Check Ai Channel\r\n","USAGE: AI? or AI=<ch>"},
	{"FLT"		,3		,DebugFLTCmdHandler, "Fault State\r\n","USAGE: FLT?"},		
//	{"FLTR"		,4		,DebugFLTRCmdHandler, "N/A - Fault Reset\r\nUSAGE: FLTR"},
	{"FCOMM"	,5		,DebugFCOMMCmdHandler, "Fiber Comm Cfg\r\n","USAGE: FCOMM? or FCOMM=<ch>,<en>[,<txsize>,<rxsize>]"},
	{"FCOMMCNT"	,8		,DebugFCOMMCNTCmdHandler, "Fiber Comm Count\r\n","USAGE: FCOMMCNT or FCOMMCNT?"},
	{"MESID"	,5		,DebugMESIDCmdHandler, "Measurement Board Id\r\n","USAGE: MESID=0|1|2|3|4|5"},	// 10


//	{"DLYT"		,4		,DebugDLYTCmdHandler, "N/A"},

//	{"FCOMMTEST",9		,DebugFCOMMTESTmdHandler, "N/A"},
#ifdef FEATURE_PERIPHERAL_TEST
	{"VMETEST"	,7		,DebugVMETESTCmdHandler, "N/A"},
#endif
//	{"RESET"	,5		,DebugRESETCmdHandler, "N/A - Manager Reset\r\nUSAGE: RESET"},
	{"DIAG"		,4		,DebugDIAGTCmdHandler, "Diag Status\r\nUSAGE: DIAG?"},

#ifdef FEATURE_DEBUG_VARIABLE
//	{"DBGR"		,4		,DebugDBGRCmdHandler, "Print Inter Debug Value\r\n","USAGE: DBGR=<startidx>,<endidx>"},
//	{"DBGW"		,4		,DebugDBGWCmdHandler, "N/A"},
#endif

	{"D"		,1		,DebugDCmdHandler, "Dump Memory(max len = 20)\r\n","USAGE: D=<address>,<len>"},	// 15
	{"F"		,1		,DebugFCmdHandler, "Fill Memory\r\n","USAGE: F=<address>,<value>,<len>"},	// 15

//	{"SSTS"		,4		,DebugSLVSTSCmdHandler, "N/A - Sync Lost Status Check\r\nUSAGE: SLVSTS? of SSTS=<cnt>"},	// 15
	{"PRD"		,3		,DebugPRDCmdHandler, "Control Period in uS\r\n","USAGE: PRD? or PRD=<period>"},
	{"IODLY"	,5		,DebugIODLYCmdHandler, "Io Update Time\r\n","USAGE: IODLY? or IODLY=<time>"},
	{"SDLY"		,4		,DebugSDLYCmdHandler, "Slave Sync Delay(uS)\r\n","USAGE: SDLY? of SDLY=<dly>"},	// 15

	{"VER"		,3		,DebugVERCmdHandler, "Sw Version\r\n","USAGE: VER"},	// 15
	{"HELP"		,4		,DebugHELPCmdHandler, "Help\r\n","USAGE: HELP or HELP=<cmd>"},	// 15
	{"DUMMY"	,5		,DebugDEFAULTCmdHandler, "Dummy"}	// 
};

// #define DEBUG_CMD_ITEM_NUM	21

uint32_t	ulTableLen;

void DebugCmdTableInit(void)
{

	ulTableLen = sizeof(DebugCmdTable)/sizeof(DebugCmdEntryType);
}

	
#if 1
int32_t DebugCmdTableRun(CmdTokenType *pToken,uint32_t ulStartIdx, uint32_t ulIdxNum)
{
	uint32_t ulLoop;
	uint32_t ulEndIdx = ulStartIdx + ulIdxNum;
	DebugCmdEntryType	*pCmdEntry;
	int32_t				lCmpResult, lRet = DEBUGCMDTABLE_FIND_ERROR;
	uint32_t i=0;


	while(pToken->pName[i] != '\0')
	{
//		pToken->pName[i] = UPCASE(pToken->pName[i]);
		i++;
	}

	for(ulLoop = ulStartIdx; ulLoop < ulEndIdx; ulLoop++)
	{
		if(ulLoop >= ulTableLen)
		{
			lRet = DEBUGCMDTABLE_NOCMD_ERROR;
			break;
		}

		pCmdEntry = &(DebugCmdTable[ulLoop]);
		lCmpResult = strcmp((const char *)pCmdEntry->strName, (const char *)pToken->pName);

		if(lCmpResult == 0)
		{
			lRet = ulLoop;
//			pCmdEntry->DebugCmdProcessFunc(pToken, ulLoop);
			break;
		}
//		pCmdEntry++;
	}
#if 0
	if(pToken->abWorkingCmdLine[0] == '\0')
	{
		
	}
	else
	{
		if(pCmdEntry->strName[0] == '\0')
		{
			// No Cmd
			ulCmdResult = CMD_EXEC_ERROR;
		}
	}
#endif
	return lRet;
}
#endif

uint32_t DebugTESTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulValue;
	uint32_t ulArg1;
	uint32_t ulLoop;

	if(pToken->wOp == (NA))
	{

	}
	else if(pToken->wOp == (NA|EQ|AR))
	{

		// Sector Erase
		if(pToken->wArgsFound == 1)
		{
		
			ulArg1 = atol((char *)pToken->pArg[0]);

			HalFlashWpDisable();

			HalFlashEraseSector(ulArg1);

			HalFlashWpEnable();

			
		}
		else
		{
			ulCmdResult = CMD_EXEC_ERROR;
		}
	}
	else
	{
		// Cmd Error
//		DebugCmdExeResult(pToken->abWorkingCmdLine, CMD_RESULT_LENGTH_NOMSG, CMD_EXEC_ERROR);
		ulCmdResult = CMD_EXEC_ERROR;
	}
	return ulCmdResult;
}


uint32_t DebugHELPCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{
	uint32_t ulValue;
	uint32_t ulArg1;
	uint32_t uli;

	// Print All Cmd
	if(pToken->wOp == (NA))				// HELP 명령 처리
	{
		ulCmdResultLen = sprintf((char *)pToken->abWorkingCmdLine+ulCmdResultLen, "--- Cmd List ---\r\n");
		for(uli=0;uli<ulTableLen-1;uli++)
		{
			ulCmdResultLen += sprintf((char *)pToken->abWorkingCmdLine+ulCmdResultLen, "%10s --- %s\r\n", DebugCmdTable[uli].strName, DebugCmdTable[uli].pcHelp);
		}
		ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "\r\n");
	}
	else if(pToken->wOp == (NA|EQ|AR))	// HELP=<parameter>
	{
		// Sector Erase
		if(pToken->wArgsFound == 1)
		{
			for(uli=0;uli<ulTableLen-1;uli++)
			{
				if(0 == strcmp((char *)pToken->pArg[0], DebugCmdTable[uli].strName))
				{
					ulCmdResultLen += sprintf((char *)pToken->abWorkingCmdLine+ulCmdResultLen, "%s\n%s", DebugCmdTable[uli].pcHelp, DebugCmdTable[uli].pcUsage);
					break;
				}
			}
			ulCmdResultLen += sprintf((char *)(pToken->abWorkingCmdLine + ulCmdResultLen), "\r\n");
		}
		else
		{
			ulCmdResult = CMD_EXEC_ERROR;
		}
	}
	else								// 정의되지 않은 명령
	{
		// Cmd Error
		ulCmdResult = CMD_EXEC_ERROR;
	}
	return ulCmdResult;
}




uint32_t DebugDEFAULTCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
{

	static uint32_t ulCount;

	ulCount++;

	return ulCmdResult;

}


