
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
/*		fbName, 			  fb ID   		  INP   				VAR     			OUT  				Init Func    		Run Func  		   	Type Pnt
        ------   			---------  		  ----  				---  	  			---   				-------------       -------------   	--------*/
	{(uint8 *)"ABS",		ABS_CODE,		ABS_SPEC_NUM,		ABS_VAR_NUM,		ABS_OUTPUT_NUM,			stdAbsInitFunc,		stdAbsRunFunc,		NULL},

	{(uint8 *)"ADD2",		ADD2_CODE,		ADD2_SPEC_NUM,		ADD2_VAR_NUM,		ADD2_OUTPUT_NUM,		stdAdd2InitFunc,	stdAdd2RunFunc,		NULL},
	{(uint8 *)"ADD4",		ADD4_CODE,		ADD4_SPEC_NUM,		ADD4_VAR_NUM,		ADD4_OUTPUT_NUM,		stdAdd4InitFunc,	stdAdd4RunFunc,		NULL},
	{(uint8 *)"ADD8",		ADD8_CODE,		ADD8_SPEC_NUM,		ADD8_VAR_NUM,		ADD8_OUTPUT_NUM,		stdAdd8InitFunc,	stdAdd8RunFunc,		NULL},

	{(uint8 *)"DIV",		DIV_CODE,		DIV_SPEC_NUM,		DIV_VAR_NUM,		DIV_OUTPUT_NUM,			stdDivInitFunc,		stdDivRunFunc,		NULL},
	{(uint8 *)"EXPT",		EXPT_CODE,		EXPT_SPEC_NUM,		EXPT_VAR_NUM,		EXPT_OUTPUT_NUM,		stdExptInitFunc,	stdExptRunFunc,		NULL},
	{(uint8 *)"MOD",		MOD_CODE,		MOD_SPEC_NUM,		MOD_VAR_NUM,		MOD_OUTPUT_NUM,			stdModInitFunc,		stdModRunFunc,		NULL},
	{(uint8 *)"MOVE",		MOVE_CODE,		MOVE_SPEC_NUM,		MOVE_VAR_NUM,		MOVE_OUTPUT_NUM,		stdMoveInitFunc,	stdMoveRunFunc,		NULL},

	{(uint8 *)"MUL2",		MUL2_CODE,		MUL2_SPEC_NUM,		MUL2_VAR_NUM,		MUL2_OUTPUT_NUM,		stdMul2InitFunc,	stdMul2RunFunc,		NULL},
	{(uint8 *)"MUL4",		MUL4_CODE,		MUL4_SPEC_NUM,		MUL4_VAR_NUM,		MUL4_OUTPUT_NUM,		stdMul4InitFunc,	stdMul4RunFunc,		NULL},
	{(uint8 *)"MUL8",		MUL8_CODE,		MUL8_SPEC_NUM,		MUL8_VAR_NUM,		MUL8_OUTPUT_NUM,		stdMul8InitFunc,	stdMul8RunFunc,		NULL},

	{(uint8 *)"MULDIV",		MULDIV_CODE,	MULDIV_SPEC_NUM,	MULDIV_VAR_NUM,		MULDIV_OUTPUT_NUM,		stdMulDivInitFunc,	stdMulDivRunFunc,	NULL},
	{(uint8 *)"SQRT",		SQRT_CODE,		SQRT_SPEC_NUM,		SQRT_VAR_NUM,		SQRT_OUTPUT_NUM,		stdSqrtInitFunc,	stdSqrtRunFunc,		NULL},
	{(uint8 *)"SUB",		SUB_CODE,		SUB_SPEC_NUM,		SUB_VAR_NUM,		SUB_OUTPUT_NUM,			stdSubInitFunc,		stdSubRunFunc,		NULL},
	{(uint8 *)"EXP",		EXP_CODE,		EXP_SPEC_NUM,		EXP_VAR_NUM,		EXP_OUTPUT_NUM,			stdExpInitFunc,		stdExpRunFunc,		NULL},
	{(uint8 *)"LN",			LN_CODE,		LN_SPEC_NUM,		LN_VAR_NUM,			LN_OUTPUT_NUM,			stdLnInitFunc,		stdLnRunFunc,		NULL},
	{(uint8 *)"LOG",		LOG_CODE,		LOG_SPEC_NUM,		LOG_VAR_NUM,		LOG_OUTPUT_NUM,			stdLogInitFunc,		stdLogRunFunc,		NULL},
	
	{(uint8 *)"AND2",	    AND2_CODE,		AND2_SPEC_NUM,		AND2_VAR_NUM,		AND2_OUTPUT_NUM,		stdAnd2InitFunc,	stdAnd2RunFunc,		NULL},
	{(uint8 *)"AND4",		AND4_CODE,		AND4_SPEC_NUM,		AND4_VAR_NUM,		AND4_OUTPUT_NUM,		stdAnd4InitFunc,	stdAnd4RunFunc,		NULL},
	{(uint8 *)"AND8",		AND8_CODE,		AND8_SPEC_NUM,		AND8_VAR_NUM,		AND8_OUTPUT_NUM,		stdAnd8InitFunc,	stdAnd8RunFunc,		NULL},

	{(uint8 *)"NOT",	    NOT_CODE,		NOT_SPEC_NUM,		NOT_VAR_NUM,		NOT_OUTPUT_NUM,			stdNotInitFunc,		stdNotRunFunc,		NULL},
	
	{(uint8 *)"OR2",	    OR2_CODE,		OR2_SPEC_NUM,		OR2_VAR_NUM,		OR2_OUTPUT_NUM,			stdOr2InitFunc,		stdOr2RunFunc,		NULL},
	{(uint8 *)"OR4",	    OR4_CODE,		OR4_SPEC_NUM,		OR4_VAR_NUM,		OR4_OUTPUT_NUM,			stdOr4InitFunc,		stdOr4RunFunc,		NULL},
	{(uint8 *)"OR8",	    OR8_CODE,		OR8_SPEC_NUM,		OR8_VAR_NUM,		OR8_OUTPUT_NUM,			stdOr8InitFunc,		stdOr8RunFunc,		NULL},

	{(uint8 *)"ROL",		ROL_CODE,		ROL_SPEC_NUM,		ROL_VAR_NUM,		ROL_OUTPUT_NUM,			stdRolInitFunc,		stdRolRunFunc,		NULL},
	{(uint8 *)"ROR",		ROR_CODE,		ROR_SPEC_NUM,		ROR_VAR_NUM,		ROR_OUTPUT_NUM,			stdRorInitFunc,		stdRorRunFunc,		NULL},
	{(uint8 *)"SHL",		SHL_CODE,		SHL_SPEC_NUM,		SHL_VAR_NUM,		SHL_OUTPUT_NUM,			stdShlInitFunc,		stdShlRunFunc,		NULL},
	{(uint8 *)"SHR",		SHR_CODE,		SHR_SPEC_NUM,		SHR_VAR_NUM,		SHR_OUTPUT_NUM,			stdShrInitFunc,		stdShrRunFunc,		NULL},

	{(uint8 *)"XOR2",	    XOR2_CODE, 		XOR2_SPEC_NUM,		XOR2_VAR_NUM,		XOR2_OUTPUT_NUM,		stdXor2InitFunc,	stdXor2RunFunc,		NULL},
	{(uint8 *)"XOR4",		XOR4_CODE, 		XOR4_SPEC_NUM,		XOR4_VAR_NUM,		XOR4_OUTPUT_NUM,		stdXor4InitFunc,	stdXor4RunFunc,		NULL},
	{(uint8 *)"XOR8",		XOR8_CODE, 		XOR8_SPEC_NUM,		XOR8_VAR_NUM,		XOR8_OUTPUT_NUM,		stdXor8InitFunc,	stdXor8RunFunc,		NULL},										
	
	{(uint8 *)"BGET",		BGET_CODE,		BGET_SPEC_NUM,		BGET_VAR_NUM,		BGET_OUTPUT_NUM,		stdBgetInitFunc,	stdBgetRunFunc	,	NULL},
	{(uint8 *)"BITAND",		BITAND_CODE,	BITAND_SPEC_NUM,	BITAND_VAR_NUM,		BITAND_OUTPUT_NUM,		stdBitAndInitFunc,	stdBitAndRunFunc,	NULL},
	{(uint8 *)"BITOR",		BITOR_CODE,		BITOR_SPEC_NUM,		BITOR_VAR_NUM,		BITOR_OUTPUT_NUM,		stdBitOrInitFunc,	stdBitOrRunFunc	,	NULL},
	{(uint8 *)"BSET",		BSET_CODE,		BSET_SPEC_NUM,		BSET_VAR_NUM,		BSET_OUTPUT_NUM,		stdBitSetInitFunc,	stdBitSetRunFunc,	NULL},
	{(uint8 *)"REG",		REG_CODE,		REG_SPEC_NUM,		REG_VAR_NUM,		REG_OUTPUT_NUM,			stdRegInitFunc,		stdRegRunFunc,		NULL},
	{(uint8 *)"SRFF",		SRFF_CODE,		SRFF_SPEC_NUM,		SRFF_VAR_NUM,		SRFF_OUTPUT_NUM,		stdSrFfInitFunc,	stdSrFfRunFunc,		NULL},										
    
    {(uint8 *)"ACOS",		ACOS_CODE,		ACOS_SPEC_NUM,		ACOS_VAR_NUM,		ACOS_OUTPUT_NUM,		stdACosInitFunc,	stdACosRunFunc,		NULL},
    {(uint8 *)"ASIN",		ASIN_CODE,		ASIN_SPEC_NUM,		ASIN_VAR_NUM,		ASIN_OUTPUT_NUM,		stdAsinInitFunc,	stdAsinRunFunc,		NULL},
    {(uint8 *)"ATAN ",		ATAN_CODE,		ATAN_SPEC_NUM,		ATAN_VAR_NUM,		ATAN_OUTPUT_NUM,		stdAtanInitFunc,	stdAtanRunFunc,		NULL},
    {(uint8 *)"COS",		COS_CODE,		COS_SPEC_NUM,		COS_VAR_NUM,		COS_OUTPUT_NUM,			stdCosInitFunc,		stdCosRunFunc,		NULL},
    {(uint8 *)"SIN",		SIN_CODE,		SIN_SPEC_NUM,		SIN_VAR_NUM,		SIN_OUTPUT_NUM,			stdSinInitFunc,		stdSinRunFunc,		NULL},
    {(uint8 *)"TAN",		TAN_CODE,		TAN_SPEC_NUM,		TAN_VAR_NUM,		TAN_OUTPUT_NUM,			stdTanInitFunc,		stdTanRunFunc,		NULL},									
	
	{(uint8 *)"EQ2",		EQ2_CODE,		EQ2_SPEC_NUM,		EQ2_VAR_NUM,		EQ2_OUTPUT_NUM,			stdEq2InitFunc,		stdEq2RunFunc,		NULL},
	{(uint8 *)"EQ4",		EQ4_CODE,		EQ4_SPEC_NUM,		EQ4_VAR_NUM,		EQ4_OUTPUT_NUM,			stdEq4InitFunc,		stdEq4RunFunc,		NULL},
	{(uint8 *)"EQ8",		EQ8_CODE,		EQ8_SPEC_NUM,		EQ8_VAR_NUM,		EQ8_OUTPUT_NUM,			stdEq8InitFunc,		stdEq8RunFunc,		NULL},	

	{(uint8 *)"GE2",		GE2_CODE,		GE2_SPEC_NUM,		GE2_VAR_NUM,		GE2_OUTPUT_NUM,			stdGe2InitFunc,		stdGe2RunFunc,		NULL},
	{(uint8 *)"GE4",		GE4_CODE,		GE4_SPEC_NUM,		GE4_VAR_NUM,		GE4_OUTPUT_NUM,			stdGe4InitFunc,		stdGe4RunFunc,		NULL},
	{(uint8 *)"GE8",		GE8_CODE,		GE8_SPEC_NUM,		GE8_VAR_NUM,		GE8_OUTPUT_NUM,			stdGe8InitFunc,		stdGe8RunFunc,		NULL},	

	{(uint8 *)"GT2",		GT2_CODE,		GT2_SPEC_NUM,		GT2_VAR_NUM,		GT2_OUTPUT_NUM,			stdGt2InitFunc,		stdGt2RunFunc,		NULL},
	{(uint8 *)"GT4",		GT4_CODE,		GT4_SPEC_NUM,		GT4_VAR_NUM,		GT4_OUTPUT_NUM,			stdGt4InitFunc,		stdGt4RunFunc,		NULL},
	{(uint8 *)"GT8",		GT8_CODE,		GT8_SPEC_NUM,		GT8_VAR_NUM,		GT8_OUTPUT_NUM,			stdGt8InitFunc,		stdGt8RunFunc,		NULL},	

	{(uint8 *)"LE2",		LE2_CODE,		LE2_SPEC_NUM,		LE2_VAR_NUM,		LE2_OUTPUT_NUM,			stdLe2InitFunc,		stdLe2RunFunc,		NULL},
	{(uint8 *)"LE4",		LE4_CODE,		LE4_SPEC_NUM,		LE4_VAR_NUM,		LE4_OUTPUT_NUM,			stdLe4InitFunc,		stdLe4RunFunc,		NULL},
	{(uint8 *)"LE8",		LE8_CODE,		LE8_SPEC_NUM,		LE8_VAR_NUM,		LE8_OUTPUT_NUM,			stdLe8InitFunc,		stdLe8RunFunc,		NULL},

	{(uint8 *)"LT2",		LT2_CODE,		LT2_SPEC_NUM,		LT2_VAR_NUM,		LT2_OUTPUT_NUM,			stdLt2InitFunc,		stdLt2RunFunc,		NULL},
	{(uint8 *)"LT4",		LT4_CODE,		LT4_SPEC_NUM,		LT4_VAR_NUM,		LT4_OUTPUT_NUM,			stdLt4InitFunc,		stdLt4RunFunc,		NULL},
	{(uint8 *)"LT8",		LT8_CODE,		LT8_SPEC_NUM,		LT8_VAR_NUM,		LT8_OUTPUT_NUM,			stdLt8InitFunc,		stdLt8RunFunc,		NULL},	
	
	{(uint8 *)"NE2",		NE2_CODE,		NE2_SPEC_NUM,		NE2_VAR_NUM,		NE2_OUTPUT_NUM,			stdNe2InitFunc,		stdNe2RunFunc,		NULL},
	{(uint8 *)"NE4",		NE4_CODE,		NE4_SPEC_NUM,		NE4_VAR_NUM,		NE4_OUTPUT_NUM,			stdNe4InitFunc,		stdNe4RunFunc,		NULL},
	{(uint8 *)"NE8",		NE8_CODE,		NE8_SPEC_NUM,		NE8_VAR_NUM,		NE8_OUTPUT_NUM,			stdNe8InitFunc,		stdNe8RunFunc,		NULL},										
	
	{(uint8 *)"B2R",		B2R_CODE,		B2R_SPEC_NUM,		B2R_VAR_NUM,		B2R_OUTPUT_NUM,			stdB2RInitFunc,		stdB2RRunFunc,		NULL},
	{(uint8 *)"R2B",		R2B_CODE,		R2B_SPEC_NUM,		R2B_VAR_NUM,		R2B_OUTPUT_NUM,			stdR2BInitFunc,		stdR2BRunFunc,		NULL},
	{(uint8 *)"B2DI",		B2DI_CODE,		B2DI_SPEC_NUM,		B2DI_VAR_NUM,		B2DI_OUTPUT_NUM,		stdB2DInitFunc,		stdB2DRunFunc,		NULL},
	{(uint8 *)"DI2B",		DI2B_CODE,		DI2B_SPEC_NUM,		DI2B_VAR_NUM,		DI2B_OUTPUT_NUM,		stdD2BInitFunc,		stdD2BRunFunc,		NULL},	
	{(uint8 *)"DI2R",		DI2R_CODE,		DI2R_SPEC_NUM,		DI2R_VAR_NUM,		DI2R_OUTPUT_NUM,		stdD2RInitFunc,		stdD2RRunFunc,		NULL},
	{(uint8 *)"R2DI",		R2DI_CODE,		R2DI_SPEC_NUM,		R2DI_VAR_NUM,		R2DI_OUTPUT_NUM,		stdR2DInitFunc,		stdR2DRunFunc,		NULL},
	{(uint8 *)"MB2DI",		MB2DI_CODE,		MB2DI_SPEC_NUM,		MB2DI_VAR_NUM,		MB2DI_OUTPUT_NUM,		stdMb2DInitFunc,	stdMb2DRunFunc,		NULL},
	{(uint8 *)"B2I",		B2I_CODE,		B2I_SPEC_NUM,		B2I_VAR_NUM,		B2I_OUTPUT_NUM,			stdB2IInitFunc,		stdB2IRunFunc,		NULL},
	{(uint8 *)"DI2MB",		DI2MB_CODE,		DI2MB_SPEC_NUM,		DI2MB_VAR_NUM,		DI2MB_OUTPUT_NUM,		stdD2MbInitFunc,	stdD2MbRunFunc,		NULL},
	
	{(uint8 *)"DI2I",		DI2I_CODE,		DI2I_SPEC_NUM,		DI2I_VAR_NUM,		DI2I_OUTPUT_NUM,		stdD2IInifFunc,		stdD2IRunFunc,		NULL},
	{(uint8 *)"I2B",		I2B_CODE,		I2B_SPEC_NUM,		I2B_VAR_NUM,		I2B_OUTPUT_NUM,			stdI2BInitFunc,		stdI2BRunFunc,		NULL},
	{(uint8 *)"I2DI",		I2DI_CODE,		I2DI_SPEC_NUM,		I2DI_VAR_NUM,		I2DI_OUTPUT_NUM,		stdI2DInitFunc,		stdI2DRunFunc,		NULL},										
	
	{(uint8 *)"CTD",		CTD_CODE,		CTD_SPEC_NUM,		CTD_VAR_NUM,		CTD_OUTPUT_NUM,			stdCtdInitFunc,		stdCtdRunFunc,		NULL},
	{(uint8 *)"CTU",		CTU_CODE,		CTU_SPEC_NUM,		CTU_VAR_NUM,		CTU_OUTPUT_NUM,			stdCtuInitFunc,		stdCtuRunFunc,		NULL},
	{(uint8 *)"CTUD",		CTUD_CODE,		CTUD_SPEC_NUM,		CTUD_VAR_NUM,		CTUD_OUTPUT_NUM,		stdCtudInitFunc,	stdCtudRunFunc,		NULL},										
	
	{(uint8 *)"FTRIG",		FTRIG_CODE,		FTRIG_SPEC_NUM,		FTRIG_VAR_NUM,		FTRIG_OUTPUT_NUM,		stdFtrigInitFunc,	stdFtrigRunFunc,	NULL},
	{(uint8 *)"RS",			RS_CODE,		RS_SPEC_NUM,		RS_VAR_NUM,			RS_OUTPUT_NUM,			stdRsInitFunc,		stdRsRunFunc,		NULL},
	{(uint8 *)"RTRIG",		RTRIG_CODE,		RTRIG_SPEC_NUM,		RTRIG_VAR_NUM,		RTRIG_OUTPUT_NUM,		stdRtrigInitFunc,	stdRtrigRunFunc,	NULL},
	{(uint8 *)"SR",			SR_CODE,		SR_SPEC_NUM,		SR_VAR_NUM,			SR_OUTPUT_NUM,			stdSrInitFunc,		stdSrRunFunc,		NULL},									
	{(uint8 *)"LIMIT",		LIMIT_CODE,		LIMIT_SPEC_NUM,		LIMIT_VAR_NUM,		LIMIT_OUTPUT_NUM,		stdLimitInitFunc,	stdLimitRunFunc,	NULL},

	{(uint8 *)"MAX2",		MAX2_CODE,		MAX2_SPEC_NUM,		MAX2_VAR_NUM,		MAX2_OUTPUT_NUM,		stdMax2InitFunc,	stdMax2RunFunc,		NULL},
	{(uint8 *)"MAX4",		MAX4_CODE,		MAX4_SPEC_NUM,		MAX4_VAR_NUM,		MAX4_OUTPUT_NUM,		stdMax4InitFunc,	stdMax4RunFunc,		NULL},
	{(uint8 *)"MAX8",		MAX8_CODE,		MAX8_SPEC_NUM,		MAX8_VAR_NUM,		MAX8_OUTPUT_NUM,		stdMax8InitFunc,	stdMax8RunFunc,		NULL},	
	
	{(uint8 *)"MIN2",		MIN2_CODE,		MIN2_SPEC_NUM,		MIN2_VAR_NUM,		MIN2_OUTPUT_NUM,		stdMin2InitFunc,	stdMin2RunFunc,		NULL},
	{(uint8 *)"MIN4",		MIN4_CODE,		MIN4_SPEC_NUM,		MIN4_VAR_NUM,		MIN4_OUTPUT_NUM,		stdMin4InitFunc,	stdMin4RunFunc,		NULL},
	{(uint8 *)"MIN8",		MIN8_CODE,		MIN8_SPEC_NUM,		MIN8_VAR_NUM,		MIN8_OUTPUT_NUM,		stdMin8InitFunc,	stdMin8RunFunc,		NULL},	

	{(uint8 *)"MUX2",		MUX2_CODE,		MUX2_SPEC_NUM,		MUX2_VAR_NUM,		MUX2_OUTPUT_NUM,		stdMux2InitFunc,	stdMux2RunFunc,		NULL},
	{(uint8 *)"MUX4",		MUX4_CODE,		MUX4_SPEC_NUM,		MUX4_VAR_NUM,		MUX4_OUTPUT_NUM,		stdMux4InitFunc,	stdMux4RunFunc,		NULL},
	{(uint8 *)"MUX8",		MUX8_CODE,		MUX8_SPEC_NUM,		MUX8_VAR_NUM,		MUX8_OUTPUT_NUM,		stdMux8InitFunc,	stdMux8RunFunc,		NULL},	

	{(uint8 *)"SEL",		SEL_CODE,		SEL_SPEC_NUM,		SEL_VAR_NUM,		SEL_OUTPUT_NUM,			stdSelInitFunc,		stdSelRunFunc,		NULL},										
	{(uint8 *)"TOF",		TOF_CODE,		TOF_SPEC_NUM,		TOF_VAR_NUM,		TOF_OUTPUT_NUM,			stdToffInitFunc,	stdToffRunFunc,		NULL},
	{(uint8 *)"TON",		TON_CODE,		TON_SPEC_NUM,		TON_VAR_NUM,		TON_OUTPUT_NUM,			stdTonInitFunc,		stdTonRunFunc,		NULL},
	{(uint8 *)"TP",			TP_CODE,		TP_SPEC_NUM,		TP_VAR_NUM,			TP_OUTPUT_NUM,			stdTpInitFunc,		stdTpRunFunc,		NULL},
	
	{(uint8 *)"ETH_SEND",	ETH_SEND_CODE,	ETH_SEND_SPEC_NUM, 	ETH_SEND_VAR_NUM,  	ETH_SEND_OUTPUT_NUM,	ethDSndInitFunc, 	ethDSndRunFunc,		NULL},
	{(uint8 *)"ETH_BROD",	ETH_BROAD_CODE,	ETH_BROAD_SPEC_NUM, ETH_BROAD_VAR_NUM,  ETH_BROAD_OUTPUT_NUM,	ethDBrdInitFunc, 	ethDBrdRunFunc,		NULL},

	{(uint8 *)"PMC502_RD",	PMC502_RD_CODE,	PMC502_RD_SPEC_NUM, PMC502_RD_VAR_NUM,  PMC502_RD_OUTPUT_NUM,	pmc502RdInitFunc,	pmc502RdRunFunc,	NULL},
	
	{(uint8 *)"IOB_DI",		IOB_DI_CODE,	IOB_DI_SPEC_NUM, 	IOB_DI_VAR_NUM,  	IOB_DI_OUTPUT_NUM,		diRdInitFunc,		diRdRunFunc,		NULL},
	{(uint8 *)"IOB_DO",		IOB_DO_CODE,	IOB_DO_SPEC_NUM, 	IOB_DO_VAR_NUM,  	IOB_DO_OUTPUT_NUM,		doWrInitFunc,		doWrRunFunc,		NULL},
	{(uint8 *)"IOB_CC",		IOB_CC_CODE,	IOB_CC_SPEC_NUM, 	IOB_CC_VAR_NUM,  	IOB_CC_OUTPUT_NUM,		dspRwInitFunc,		dspRwRunFunc,		NULL},

	{(uint8 *)"SYSCFG",		SYSCFG_CODE,	SYSCFG_SPEC_NUM,	SYSCFG_VAR_NUM,		SYSCFG_OUTPUT_NUM,		sysStaCpInitFunc,	sysStaCpRunFunc,	NULL},

	{(uint8 *)"WRVAR",	   	WRVAR_CODE,		WRVAR_SPEC_NUM,		WRVAR_VAR_NUM,		WRVAR_OUTPUT_NUM,		wrVarInitFunc,		wrVarRunFunc,		NULL}
};
