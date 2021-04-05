

#ifndef _DEBUGCMDPARSER_H_
#define _DEBUGCMDPARSER_H_

#include "Ial/IalDebug.h"
#include "DebugCmdTable.h"


#define NA		1                 /*  Name field found      */
#define EQ		2                 /*  <=> found             */
#define QU		4                 /*  <?> found             */
#define AR		8                 /*  Argument field found  */


#define PARSER_CMD_FOUND		0 


typedef enum {
	PARSER_STATE_NAMEHUNT,
	PARSER_STATE_ARGSHUNT,
	
	PARSER_STATE_NUM	
} PARSER_STATE;




void DebugCmdParserInit(void);


void DebugCmdParserRun(uint8_t *pCmdLine, uint32_t ulCmdLineLen, CmdTokenType *pToken);



#endif
