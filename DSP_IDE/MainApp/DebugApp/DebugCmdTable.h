

#ifndef _DEBUGCMDTABLE_H_
#define _DEBUGCMDTABLE_H_



#include <string.h>

#include <stdint.h>



#define MAX_DEBUG_CMD_LEN	20

#define MAX_ARG_NUM			10
#define MAX_LINE_SIZE		2048//100



#define DEBUGCMDTABLE_FIND_ERROR	-1
#define DEBUGCMDTABLE_NOCMD_ERROR	-2




#include "Core/Core.h"

typedef struct
{
	char	abWorkingCmdLine[MAX_LINE_SIZE];
	uint8_t *pName;
	uint16_t wOp;
	uint8_t *pArg[MAX_ARG_NUM];
	uint16_t wArgsFound;
} CmdTokenType;


typedef struct 
{

  char  strName[MAX_DEBUG_CMD_LEN];	// Command Name 
  uint16_t  wNameLen;                   // Command Length
  uint32_t  (*DebugCmdProcessFunc)(			// Command Handler Entry
    CmdTokenType	*pToken,					// Token
    int32_t		lMatchIndex				// 명령어 이름 길이
  );
  char	*pcHelp;
  char	*pcUsage;
} DebugCmdEntryType;


extern DebugCmdEntryType DebugCmdTable[];




void DebugCmdTableInit(void);
	


#if 1
int32_t DebugCmdTableRun(CmdTokenType *pToken,uint32_t ulStartIdx, uint32_t ulIdxNum);
#else
uint32_t DebugCmdTableRun(void);
#endif



#endif
