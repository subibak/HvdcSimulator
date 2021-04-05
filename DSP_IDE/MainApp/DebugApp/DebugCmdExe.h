

#ifndef _DEBUGCMDEXE_H_
#define _DEBUGCMDEXE_H_

#include "DebugCmdTable.h"
#include "Ial/IalDebug.h"


#define CMD_EXEC_OK			0x0
#define CMD_EXEC_ERROR		0x1
#define CMD_EXEC_PENDING	0x2

#define CMD_RESULT_LENGTH_NOMSG		0x0

#define CMD_EXEC_OK_LEN		0x4
#define CMD_EXEC_ERROR_LEN	0x7


#define PROMPT_STR	"DSP> "
#define PROMPT_LEN	5

void DebugCmdExeInit(void);

//void DebugCmdExeRun(CmdTokenType *pToken);
uint32_t DebugCmdExeRun(CmdTokenType *pToken, int32_t lCmdIdx);

void DebugCmdExeFinalize(CmdTokenType *pToken, int32_t lCmdIdx);
void DebugCmdExeResult(char *pcMsg, uint32_t ulLen, uint32_t ulResult);


extern uint32_t ulCmdResult;
extern uint32_t ulCmdResultLen;



#endif
