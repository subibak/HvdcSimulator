

#include "DebugCmdParser.h"



#define 	PARSE_NUM_ONCE	5


	




void DebugCmdParserInit(void)
{


}



void DebugCmdParserRun(uint8_t *pCmdLine, uint32_t ulCmdLineLen, CmdTokenType *pToken)
{

	uint32_t	iLoop;
	uint16_t	wCmdFound;
	uint8_t		*pbCmdline = (uint8_t *)pToken->abWorkingCmdLine;
//	uint8_t		wArgIndex=0;

	if(ulCmdLineLen == 0)
	{
  		sprintf(pToken->abWorkingCmdLine, "DUMMY");
  		pToken->pName = (uint8_t *)pToken->abWorkingCmdLine;
		pToken->wOp = NA;
		pToken->wArgsFound = 0;
	}
	else
	{
		// Initialize Token struct
  		pToken->pName = (uint8_t *)pToken->abWorkingCmdLine;
		pToken->wOp = NA;
		pToken->wArgsFound = 0;

		for(iLoop = 0; iLoop < ulCmdLineLen; iLoop++, pbCmdline++)
		{

			switch(*(pbCmdline))
			{
				case '=':							// Command with Arguments
					pToken->wOp |= EQ;
				case ',':							// Command with Arguments
					*(pbCmdline) = 0; 					// NULL Termination
					pToken->pArg[pToken->wArgsFound] = pbCmdline+1;
					if(*pToken->pArg[pToken->wArgsFound] == '\0')
					{
						;
					}
					else
					{
						pToken->wOp |= AR;
						pToken->wArgsFound++;
					}
					break;
				case '?':							// Command with Arguments
					*(pbCmdline) = 0; 					// NULL Termination
					pToken->wOp |= QU;
					break;
				case 0x0:							// End of Command
				default:
					// Normal Characters
					break;
	        				
			}

		}

	}

}

