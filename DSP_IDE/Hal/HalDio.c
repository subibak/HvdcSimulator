
/**
 * HalDio.c
 * HalDio Device Driver Module Body
 */


#include "HalDio.h"

ST_DIO_CONFIG_TYPE	t_stDioConfig = {
	{{HALDIO_OUT_DIR,	HWREG_DIO_CTRL_BANK0_ENA_YES}
	,{HALDIO_OUT_DIR,	HWREG_DIO_CTRL_BANK1_ENA_YES}
	,{HALDIO_IN_DIR,	HWREG_DIO_CTRL_BANK2_ENA_YES}
	,{HALDIO_IN_DIR,	HWREG_DIO_CTRL_BANK3_ENA_YES}}
	,HALDIO_OUT_DEFAULT		// Out
	,0x0					// In
	,CORE_ERROR_NONE		// Out Default
	,0x0					// CtrlReg
	,0x0					// CFGReg
};

void HalDioInit(void)
{

	uint32_t	ulRegValue;
	ST_DIO_CONFIG_TYPE	*pstDioConfig = &t_stDioConfig;
	uint32_t i;
	

	ReadReg(HWREG_DIO_CTRL_ADDR, &(pstDioConfig->ulCtrlReg));

	pstDioConfig->ulCtrlReg =	pstDioConfig->ulCtrlReg | (HWREG_DIO_CTRL_MEZZ_ENA_YES
								|HWREG_DIO_CTRL_TERM_DIR_IN|HWREG_DIO_CTRL_TERM_ENA_YES
								|HWREG_DIO_CTRL_TERM_DIOENA_YES);

	for(i=0;i < HALDIO_BANK_NUM;i++)
	{
		pstDioConfig->ulCtrlReg = 	pstDioConfig->ulCtrlReg | (pstDioConfig->astBank[i].aulBankDir << i) | (pstDioConfig->astBank[i].aulBankEn);
	}

	WriteReg(HWREG_DIO_CTRL_ADDR,	pstDioConfig->ulCtrlReg);


	ReadReg(HWREG_DIO_CFG_ADDR, &(pstDioConfig->ulCfgReg));

	// Mezzanine Board Check
	if(((pstDioConfig->ulCfgReg) & HWREG_DIO_CFG_MEZ_INS_MASK) == HWREG_DIO_CFG_MEZ_INS_NO)
	{
		// Mezanine Board Not inserted
		pstDioConfig->ulErrStatus = pstDioConfig->ulErrStatus | CORE_ERROR_MEZZANINE_DISCON;
//		SetErrorCause(&ulErrorCause, CORE_ERROR_MEZZANINE_DISCON)
	}

	// Terminal Board Check
	if(((pstDioConfig->ulCfgReg) & HWREG_DIO_CFG_TERM_INS_MASK) == HWREG_DIO_CFG_TERM_INS_YES)
	{
		// Terminal Board Not inserted
		pstDioConfig->ulErrStatus = pstDioConfig->ulErrStatus | CORE_ERROR_TERMINAL_DISCON;
//		SetErrorCause(&ulErrorCause, CORE_ERROR_TERMINAL_DISCON)
	}

	pstDioConfig->ulCtrlReg = pstDioConfig->ulCtrlReg | (HWREG_DIO_CTRL_TERM_DIR_OUT |HWREG_DIO_CTRL_TERM_ENA_YES|HWREG_DIO_CTRL_MEZZ_ENA_YES);

	WriteReg(HWREG_DIO_CTRL_ADDR,	pstDioConfig->ulCtrlReg);



	WriteReg(HWREG_DO_DATA_ADDR,	pstDioConfig->ulOutValue);

	// Init Dio Block
	pstDioConfig->ulCtrlReg =	pstDioConfig->ulCtrlReg | (HWREG_DIO_CTRL_TERM_DIR_OUT
								|HWREG_DIO_CTRL_TERM_BLUELED_ON|HWREG_DIO_CTRL_TERM_GREENLED_OFF);

	WriteReg(HWREG_DIO_CTRL_ADDR, pstDioConfig->ulCtrlReg);
		
}

void HalDioCheckSubBoard(void)
{
	uint32_t	ulCtrlRegValue, ulCfgRegValue;
	uint32_t			*pstState = &(t_stDioConfig.ulErrStatus);
	
	ReadReg(HWREG_DIO_CTRL_ADDR, &ulCtrlRegValue);
	ulCtrlRegValue = ulCtrlRegValue & (~HWREG_DIO_CTRL_TERM_DIR_OUT);
	WriteReg(HWREG_DIO_CTRL_ADDR, ulCtrlRegValue);

	ReadReg(HWREG_DIO_CFG_ADDR, &ulCfgRegValue);

	// Mezzanine Board Check
	if((ulCfgRegValue & HWREG_DIO_CFG_MEZ_INS_MASK) == HWREG_DIO_CFG_MEZ_INS_NO)
	{
		// Mezanine Board Not inserted
		*pstState = *pstState | CORE_ERROR_MEZZANINE_DISCON;
	}

	// Terminal Board Check
	if((ulCfgRegValue & HWREG_DIO_CFG_TERM_INS_MASK) == HWREG_DIO_CFG_TERM_INS_YES)
	{
		// Terminal Board Not inserted
		*pstState = *pstState | CORE_ERROR_TERMINAL_DISCON;
	}

	ulCtrlRegValue = ulCtrlRegValue | HWREG_DIO_CTRL_TERM_DIR_OUT;
	WriteReg(HWREG_DIO_CTRL_ADDR, ulCtrlRegValue);

}

uint32_t HalDiRead(uint32_t	ulChNum)	// ulChNum: 1~16 => 16~31
{
	uint32_t ulChShift = HALDI_CH_START + ulChNum - HALDIO_CH_OFFSET;
	uint32_t	*pulDiData = &(t_stDioConfig.ulInValue);

#ifdef FEATURE_DI_SEPERATE_ACCESS
	ReadReg(HWREG_DI_DATA_ADDR, pulDiData);
#endif

	return (*pulDiData & (HWREG_DI_DATA_CH1_MASK<<ulChShift)) >> ulChShift;
}

uint32_t HalDoRead(uint32_t	ulChNum)	// ulChNum: 1~16 => 0~15
{
	uint32_t ulChShift = HALDO_CH_START + ulChNum - HALDIO_CH_OFFSET;
	uint32_t	*pulDoData = &(t_stDioConfig.ulOutValue);

#ifdef FEATURE_DI_SEPERATE_ACCESS
	ReadReg(HWREG_DO_DATA_ADDR, pulDoData);
#endif

	return (*pulDoData & (HWREG_DO_DATA_CH1_MASK<<ulChShift)) >> ulChShift;
}

void HalDoWrite(uint32_t	ulChNum, uint32_t ulOutValue)	// ulChNum: 1~16
{
	uint32_t	ulChShift = HALDO_CH_START + ulChNum - HALDIO_CH_OFFSET;
	uint32_t	*pulDoData = &(t_stDioConfig.ulOutValue);

	if(ulOutValue == HALDIO_ON)
	{
		*pulDoData = *pulDoData | (HALDIO_ON << ulChShift);
	}
	else 
	{
		*pulDoData = *pulDoData & ~(HALDIO_ON << ulChShift);
	}
#ifdef FEATURE_DI_SEPERATE_ACCESS
	WriteReg(HWREG_DO_DATA_ADDR, *pulDoData);
#endif
}


bool IsMbdConnect(void)
{

	uint32_t	ulReg;
	bool		blResult;

	ReadReg(HWREG_DIO_CFG_ADDR, &ulReg);

	if((ulReg & HWREG_DIO_CFG_MEZ_INS_MASK) == HWREG_DIO_CFG_MEZ_INS_YES)
	{
		blResult = true;
	}
	else
	{
		blResult = false;
	}

	return blResult;

}





