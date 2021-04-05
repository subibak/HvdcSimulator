

#include <string.h>
#include "DebugCmdExe.h"


uint32_t ulCmdResult = CMD_EXEC_ERROR;
uint32_t ulCmdResultLen = CMD_RESULT_LENGTH_NOMSG;

uint8_t		abPrompt[12];
uint32_t	ulPromptLen;


void DebugCmdExeInit(void)
{

	switch(ulCfgSystemCfg)
	{
		case SYSTEM_MES:
			ulPromptLen = sprintf((char *)abPrompt, "%s_%s","MES",PROMPT_STR);
			break;
		case SYSTEM_CCS:
			ulPromptLen = sprintf((char *)abPrompt, "%s_%s","CCS",PROMPT_STR);
			break;
		case SYSTEM_VBC:
			ulPromptLen = sprintf((char *)abPrompt, "%s_%s","VBC",PROMPT_STR);
			break;
		default:
			ulPromptLen = sprintf((char *)abPrompt, "%s_%s","DSP",PROMPT_STR);
			break;
	}

}

	

//void DebugCmdExeRun(TokenType *pToken)
//void DebugCmdExeRun(CmdTokenType *pToken)
uint32_t DebugCmdExeRun(CmdTokenType *pToken, int32_t lCmdIdx)
{

//	uint32_t	ulLoop;
//	uint16_t	wCmdFound;
//	char		*pbCmdline = pToken->abWorkingCmdLine;
//	uint8_t		wArgIndex=0;
//	int32_t		lCmpResult = 0;


	DebugCmdEntryType	*pCmdEntry = &(DebugCmdTable[lCmdIdx]);

#if 0
	// Step 1: Find Cmd in Table and Execute Handler
	for(ulLoop=0;ulLoop < DEBUG_CMD_ITEM_NUM - 1;ulLoop++)
	{
		lCmpResult = strcmp((const char *)pCmdEntry->strName, (const char *)pToken->pName);
		if(lCmpResult == 0)
		{
			pCmdEntry->DebugCmdProcessFunc(pToken, ulLoop);
			break;
		}
		pCmdEntry++;
	}

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
	// Execute Handler
	return pCmdEntry->DebugCmdProcessFunc(pToken, lCmdIdx);


}

void DebugCmdExeFinalize(CmdTokenType *pToken, int32_t lCmdIdx)
{

	// Step 2: Print Result And Prompt
	DebugCmdExeResult(pToken->abWorkingCmdLine, ulCmdResultLen, ulCmdResult);



	ulCmdResult = CMD_EXEC_OK;
	ulCmdResultLen = CMD_RESULT_LENGTH_NOMSG;
}

void DebugCmdExeResult(char *pcMsg, uint32_t ulLen, uint32_t ulResult)
{
	uint8_t	*pbMsg = (uint8_t *)pcMsg;

	if(ulResult == CMD_EXEC_OK)
	{
		IalDebugWrite(pbMsg, ulLen);
		IalDebugWrite((uint8_t *)"OK\r\n", CMD_EXEC_OK_LEN);
	}
	else
	{
		IalDebugWrite((uint8_t *)"ERROR\r\n", CMD_EXEC_ERROR_LEN);
	}

	IalDebugWrite((uint8_t *)abPrompt, ulPromptLen);

}



