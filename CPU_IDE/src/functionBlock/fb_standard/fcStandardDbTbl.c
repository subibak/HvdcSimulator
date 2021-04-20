
#include	<stdio.h>
#include	<stdlib.h>

#include	"usrTypes.h"
#include	"fbDef.h"

#include	"fb_code\fb_standard\fcArithmetic.h"
#include	"fb_code\fb_standard\fcBitshift.h"
#include	"fb_code\fb_standard\fcBitStableEle.h"
#include	"fb_code\fb_standard\fcBitwise.h"
#include	"fb_code\fb_standard\fcComparison.h"
#include	"fb_code\fb_standard\fcCounter.h"
#include	"fb_code\fb_standard\fcEdgeDect.h"
#include	"fb_code\fb_standard\fcExtra.h"
#include	"fb_code\fb_standard\fcMiscellary.h"
#include	"fb_code\fb_standard\fcNumerical.h"

#include	"fb_code\fb_standard\fcSelection.h"
#include	"fb_code\fb_standard\fcTimer.h"
#include	"fb_code\fb_standard\fcTypeConv.h"
#include	"fb_code\fb_standard\fcVmeBrdAcc.h"

/*********************************************************************
**				
**	IEC61131-3 Standard Function Code Á¤ÀÇ
**
*********************************************************************/
strFbExecDbInfo	gStandardFbDbTableInfo[MAX_FB_NUM + 1] =
{
/*			fbName, 		fb ID   INP   VAR     OUT  		Init Func    		Run Func  		    Type Pnt 
            ------   		------  ----  ---  	  ---   	-------------       -------------       -------- */
	{(uint8 *)"ABS",		1,		1,		0,		1,		stdAbsInitFunc,		stdAbsRunFunc,		NULL},
	{(uint8 *)"ADD2",		2,		2,		0,		1,		stdAdd2InitFunc,	stdAdd2RunFunc,		NULL},
	{(uint8 *)"ADD4",		4,		4,		0,		1,		stdAdd4InitFunc,	stdAdd4RunFunc,		NULL},
	{(uint8 *)"ADD8",		8,		8,		0,		1,		stdAdd8InitFunc,	stdAdd8RunFunc,		NULL},
	{(uint8 *)"DIV",		10,		2,		0,		1,		stdDivInitFunc,		stdDivRunFunc,		NULL},
	{(uint8 *)"EXPT",		11,		2,		0,		1,		stdExptInitFunc,	stdExptRunFunc,		NULL},
	{(uint8 *)"MOD",		12,		2,		0,		1,		stdModInitFunc,		stdModRunFunc,		NULL},
	{(uint8 *)"MOVE",		13,		1,		0,		1,		stdMoveInitFunc,	stdMoveRunFunc,		NULL},
	{(uint8 *)"MUL2",		14,		2,		0,		1,		stdMul2InitFunc,	stdMul2RunFunc,		NULL},
	{(uint8 *)"MUL4",		16,		4,		0,		1,		stdMul4InitFunc,	stdMul4RunFunc,		NULL},
	
	{(uint8 *)"MUL8",		20,		8,		0,		1,		stdMul8InitFunc,	stdMul8RunFunc,		NULL},
	{(uint8 *)"MULDIV",		22,		3,		0,		2,		stdMulDivInitFunc,	stdMulDivRunFunc,	NULL},
	{(uint8 *)"SQRT",		23,		1,		0,		1,		stdSqrtInitFunc,	stdSqrtRunFunc,		NULL},
	{(uint8 *)"SUB",		24,		2,		0,		1,		stdSubInitFunc,		stdSubRunFunc,		NULL},
	{(uint8 *)"EXP",		25,		1,		0,		1,		stdExpInitFunc,		stdExpRunFunc,		NULL},
	{(uint8 *)"LN",			26,		1,		0,		1,		stdLnInitFunc,		stdLnRunFunc,		NULL},
	{(uint8 *)"LOG",		27,		1,		0,		1,		stdLogInitFunc,		stdLogRunFunc,		NULL},
	
	{(uint8 *)"AND2",	    31,		2,		0,		1,		stdAnd2InitFunc,	stdAnd2RunFunc,		NULL},
	{(uint8 *)"AND4",		33,		4,		0,		1,		stdAnd4InitFunc,	stdAnd4RunFunc,		NULL},
	{(uint8 *)"AND8",		37,		8,		0,		1,		stdAnd8InitFunc,	stdAnd8RunFunc,		NULL},
	{(uint8 *)"NOT",	    39,		1,		0,		1,		stdNotInitFunc,		stdNotRunFunc,		NULL},
	
	{(uint8 *)"OR2",	    40,		2,		0,		1,		stdOr2InitFunc,		stdOr2RunFunc,		NULL},
	{(uint8 *)"OR4",	    42,		4,		0,		1,		stdOr4InitFunc,		stdOr4RunFunc,		NULL},
	{(uint8 *)"OR8",	    46,		8,		0,		1,		stdOr8InitFunc,		stdOr8RunFunc,		NULL},
	{(uint8 *)"ROL",		48,		2,		0,		1,		stdRolInitFunc,		stdRolRunFunc,		NULL},
	{(uint8 *)"ROR",		49,		2,		0,		1,		stdRorInitFunc,		stdRorRunFunc,		NULL},
	
	{(uint8 *)"SHL",		50,		2,		0,		1,		stdShlInitFunc,		stdShlRunFunc,		NULL},
	{(uint8 *)"SHR",		51,		2,		0,		1,		stdShrInitFunc,		stdShrRunFunc,		NULL},
	{(uint8 *)"XOR2",	    52, 	2,		0,		1,		stdXor2InitFunc,	stdXor2RunFunc,		NULL},
	{(uint8 *)"XOR4",		54,		4,		0,		1,		stdXor4InitFunc,	stdXor4RunFunc,		NULL},
	{(uint8 *)"XOR8",		58,		8,		0,		1,		stdXor8InitFunc,	stdXor8RunFunc,		NULL},										
	
	{(uint8 *)"BGET",		61,		2,		0,		1,		stdBgetInitFunc,	stdBgetRunFunc	,	NULL},
	{(uint8 *)"BITAND",		62,		2,		0,		1,		stdBitAndInitFunc,	stdBitAndRunFunc,	NULL},
	{(uint8 *)"BITOR",		63,		2,		0,		1,		stdBitOrInitFunc,	stdBitOrRunFunc	,	NULL},
	{(uint8 *)"BSET",		64,		4,		0,		1,		stdBitSetInitFunc,	stdBitSetRunFunc,	NULL},
	{(uint8 *)"REG",		65,		4,		1,		1,		stdRegInitFunc,		stdRegRunFunc,		NULL},
	{(uint8 *)"SRFF",		66,		4,		1,		1,		stdSrFfInitFunc,	stdSrFfRunFunc,		NULL},										
    
    {(uint8 *)"ACOS",		71,		1,		0,		1,		stdACosInitFunc,	stdACosRunFunc,		NULL},
    {(uint8 *)"ASIN",		72,		1,		0,		1,		stdAsinInitFunc,	stdAsinRunFunc,		NULL},
    {(uint8 *)"ATAN ",		73,		1,		0,		1,		stdAtanInitFunc,	stdAtanRunFunc,		NULL},
    {(uint8 *)"COS",		74,		1,		0,		1,		stdCosInitFunc,		stdCosRunFunc,		NULL},
    {(uint8 *)"SIN",		75,		1,		0,		1,		stdSinInitFunc,		stdSinRunFunc,		NULL},
    {(uint8 *)"TAN",		76,		1,		0,		1,		stdTanInitFunc,		stdTanRunFunc,		NULL},									
	
	{(uint8 *)"EQ2",		81,		2,		0,		1,		stdEq2InitFunc,		stdEq2RunFunc,		NULL},
	{(uint8 *)"EQ4",		83,		4,		0,		1,		stdEq4InitFunc,		stdEq4RunFunc,		NULL},
	{(uint8 *)"EQ8",		87,		8,		0,		1,		stdEq8InitFunc,		stdEq8RunFunc,		NULL},	
	{(uint8 *)"GE2",		89,		2,		0,		1,		stdGe2InitFunc,		stdGe2RunFunc,		NULL},
	
	{(uint8 *)"GE4",		91,		4,		0,		1,		stdGe4InitFunc,		stdGe4RunFunc,		NULL},
	{(uint8 *)"GE8",		95,		8,		0,		1,		stdGe8InitFunc,		stdGe8RunFunc,		NULL},	
	{(uint8 *)"GT2",		97,		2,		0,		1,		stdGt2InitFunc,		stdGt2RunFunc,		NULL},
	{(uint8 *)"GT4",		99,		4,		0,		1,		stdGt4InitFunc,		stdGt4RunFunc,		NULL},
	
	{(uint8 *)"GT8",		103,	8,		0,		1,		stdGt8InitFunc,		stdGt8RunFunc,		NULL},	
	{(uint8 *)"LE2",		105,	2,		0,		1,		stdLe2InitFunc,		stdLe2RunFunc,		NULL},
	{(uint8 *)"LE4",		107,	4,		0,		1,		stdLe4InitFunc,		stdLe4RunFunc,		NULL},
	
	{(uint8 *)"LE8",		111,	8,		0,		1,		stdLe8InitFunc,		stdLe8RunFunc,		NULL},
	{(uint8 *)"LT2",		113,	2,		0,		1,		stdLt2InitFunc,		stdLt2RunFunc,		NULL},
	{(uint8 *)"LT4",		115,	4,		0,		1,		stdLt4InitFunc,		stdLt4RunFunc,		NULL},
	{(uint8 *)"LT8",		119,	8,		0,		1,		stdLt8InitFunc,		stdLt8RunFunc,		NULL},	
	
	{(uint8 *)"NE2",		121,	2,		0,		1,		stdNe2InitFunc,		stdNe2RunFunc,		NULL},
	{(uint8 *)"NE4",		123,	4,		0,		1,		stdNe4InitFunc,		stdNe4RunFunc,		NULL},
	{(uint8 *)"NE8",		127,	8,		0,		1,		stdNe8InitFunc,		stdNe8RunFunc,		NULL},										
	
	{(uint8 *)"B2R",		131,	1,		0,		1,		stdB2RInitFunc,		stdB2RRunFunc,		NULL},
	{(uint8 *)"R2B",		132,	1,		0,		1,		stdR2BInitFunc,		stdR2BRunFunc,		NULL},
	{(uint8 *)"B2DI",		133,	1,		0,		1,		stdB2DInitFunc,		stdB2DRunFunc,		NULL},
	{(uint8 *)"DI2B",		134,	1,		0,		1,		stdD2BInitFunc,		stdD2BRunFunc,		NULL},	
	{(uint8 *)"DI2R",		135,	1,		0,		1,		stdD2RInitFunc,		stdD2RRunFunc,		NULL},
	{(uint8 *)"R2DI",		136,	1,		0,		1,		stdR2DInitFunc,		stdR2DRunFunc,		NULL},
	{(uint8 *)"MB2DI",		137,	32,		0,		1,		stdMb2DInitFunc,	stdMb2DRunFunc,		NULL},
	{(uint8 *)"B2I",		138,	16,		0,		1,		stdB2IInitFunc,		stdB2IRunFunc,		NULL},
	{(uint8 *)"DI2B",		139,	1,		0,		32,		stdD2MbInitFunc,	stdD2MbRunFunc,		NULL},
	
	{(uint8 *)"DI2I",		140,	1,		0,		1,		stdD2IInifFunc,		stdD2IRunFunc,		NULL},
	{(uint8 *)"I2B",		141,	1,		0,		16,		stdI2BInitFunc,		stdI2BRunFunc,		NULL},
	{(uint8 *)"I2DI",		142,	1,		0,		1,		stdI2DInitFunc,		stdI2DRunFunc,		NULL},										
	
	{(uint8 *)"CTD",		201,	3,		1,		2,		stdCtdInitFunc,		stdCtdRunFunc,		NULL},
	{(uint8 *)"CTU",		202,	3,		1,		2,		stdCtuInitFunc,		stdCtuRunFunc,		NULL},
	{(uint8 *)"CTUD",		203,	5,		2,		3,		stdCtudInitFunc,	stdCtudRunFunc,		NULL},										
	
	{(uint8 *)"FTRIG",		211,	1,		1,		1,		stdFtrigInitFunc,	stdFtrigRunFunc,	NULL},
	{(uint8 *)"RS",			212,	2,		0,		1,		stdRsInitFunc,		stdRsRunFunc,		NULL},
	{(uint8 *)"RTRIG",		213,	1,		1,		1,		stdRtrigInitFunc,	stdRtrigRunFunc,	NULL},
	{(uint8 *)"SR",			214,	2,		0,		1,		stdSrInitFunc,		stdSrRunFunc,		NULL},									
	
	{(uint8 *)"LIMIT",		231,	3,		0,		1,		stdLimitInitFunc,	stdLimitRunFunc,	NULL},
	{(uint8 *)"MAX2",		232,	2,		0,		1,		stdMax2InitFunc,	stdMax2RunFunc,		NULL},
	{(uint8 *)"MAX4",		234,	4,		0,		1,		stdMax4InitFunc,	stdMax4RunFunc,		NULL},
	{(uint8 *)"MAX8",		238,	8,		0,		1,		stdMax8InitFunc,	stdMax8RunFunc,		NULL},	
	
	{(uint8 *)"MIN2",		240,	2,		0,		1,		stdMin2InitFunc,	stdMin2RunFunc,		NULL},
	{(uint8 *)"MIN4",		242,	4,		0,		1,		stdMin4InitFunc,	stdMin4RunFunc,		NULL},
	{(uint8 *)"MIN8",		246,	8,		0,		1,		stdMin8InitFunc,	stdMin8RunFunc,		NULL},	
	{(uint8 *)"MUX2",		248,	3,		0,		1,		stdMux2InitFunc,	stdMux2RunFunc,		NULL},
	
	{(uint8 *)"MUX4",		250,	5,		0,		1,		stdMux4InitFunc,	stdMux4RunFunc,		NULL},
	{(uint8 *)"MUX8",		254,	9,		0,		1,		stdMux8InitFunc,	stdMux8RunFunc,		NULL},	
	{(uint8 *)"SEL",		256,	3,		0,		1,		stdSelInitFunc,		stdSelRunFunc,		NULL},										
	
	{(uint8 *)"TOFF",		261,	2,		2,		2,		stdToffInitFunc,	stdToffRunFunc,		NULL},
	{(uint8 *)"TON",		262,	2,		2,		2,		stdTonInitFunc,		stdTonRunFunc,		NULL},
	{(uint8 *)"TP",			263,	2,		2,		2,		stdTpInitFunc,		stdTpRunFunc,		NULL},
	
	{(uint8 *)"ETH_SEND",	302,	20, 	9,  	9,		ethDSndInitFunc, 	ethDSndRunFunc,		NULL},
	{(uint8 *)"ETH_BROD",	303,	12, 	3,  	4,		ethDBrdInitFunc, 	ethDBrdRunFunc,		NULL},
	{(uint8 *)"PMC502_RD",	304,	5,		0,		2,		pmc502RdInitFunc,	pmc502RdRunFunc,	NULL},
	
	{(uint8 *)"IOB_DI",		311,	1,		2,		2,		diRdInitFunc,		diRdRunFunc,		NULL},
	{(uint8 *)"IOB_DO",		312,	2,		2,		2,		doWrInitFunc,		doWrRunFunc,		NULL},
	{(uint8 *)"IOB_CC",		314,	8,		2,		1,		dspRwInitFunc,		dspRwRunFunc,		NULL},

	{(uint8 *)"SYSCFG",		401,	8,		0,		8,		sysStaCpInitFunc,	sysStaCpRunFunc,	NULL},

	{(uint8 *)"WRVAR",	   	681,	2,		0,		1,		wrVarInitFunc,		wrVarRunFunc,		NULL}
};
