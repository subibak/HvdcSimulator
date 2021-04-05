
/**
 * HalTimer.c
 * HalTimer Device Driver Module Body
 */

#include "HalAo.h"

ST_AO_CONF_TYPE	t_stAoConfigData;

void HalAoInit(void)
{
	uint32_t i;
	ST_AO_CONF_TYPE	*pstAoConfigData = &t_stAoConfigData;
	

	for(i=0;i<HALAO_CH_NUM;i++)
	{
		pstAoConfigData->afAoData[i] = 0;
	}


	ReadReg(HWREG_DAC_CTRL_ADDR, &(pstAoConfigData->ulAoCtrlReg));
	ReadReg(HWREG_DAC_CFG_ADDR, &(pstAoConfigData->ulAoCfgReg));

	pstAoConfigData->ulAoCtrlReg	= pstAoConfigData->ulAoCtrlReg | (HWREG_DAC_CTRL_RSTSEL_0V|HWREG_DAC_CTRL_BTC_2COML);
	pstAoConfigData->ulAoCfgReg		= pstAoConfigData->ulAoCfgReg | (HWREG_DAC_CFG_SCE_ENA|HWREG_DAC_CFG_GAIN_4);

	WriteReg(HWREG_DAC_CTRL_ADDR, pstAoConfigData->ulAoCtrlReg);
	WriteReg(HWREG_DAC_CFG_ADDR,  pstAoConfigData->ulAoCfgReg);

}

void HalAoWrite(uint32_t ulChNum, float fValue)	// 1~
{
	float	*pfAoWrite = t_stAoConfigData.afAoData;

#ifdef FEATURE_AO_SEPERATE_ACCESS
	uint32_t	ulHex;

	ulHex = mV2Hex(DAC_OUTPUT_MAX, DAC_OUTPUT_MIN, DAC_OUTPUT_MAX_HEX, DAC_OUTPUT_MIN_HEX, fValue);
	WriteReg(HWREG_DAC_OUTPUT_ADDR + ulChNum - 1, ulHex);
#endif

	pfAoWrite[ulChNum - 1] = fValue;

//	return pstAoWrite[ulChNum];

}

float HalAoRead(uint32_t ulChNum)	// 1 ~
{
	float		*pfAoWrite = t_stAoConfigData.afAoData;

#ifdef FEATURE_AO_SEPERATE_ACCESS
	uint32_t	ulHex;

	ReadReg(HWREG_DAC_OUTPUT_ADDR + ulChNum - 1, &ulHex);

	pfAoWrite[ulChNum - 1] = Hex2mV(DAC_OUTPUT_MAX, DAC_OUTPUT_MIN, DAC_OUTPUT_MAX_HEX, DAC_OUTPUT_MIN_HEX, ulHex);
#endif

	return pfAoWrite[ulChNum - 1];
}


void HalAoHandle(void)
{
	float		*pfAoWrite = t_stAoConfigData.afAoData;
	uint32_t	ulChNum;
	uint32_t	ulHex;

	for(ulChNum=0;ulChNum<HALAO_CH_NUM;ulChNum++)
	{
		ulHex = mV2Hex(DAC_OUTPUT_MAX, DAC_OUTPUT_MIN, DAC_OUTPUT_MAX_HEX, DAC_OUTPUT_MIN_HEX, pfAoWrite[ulChNum]);
		WriteReg(HWREG_DAC_OUTPUT_ADDR + ulChNum, ulHex);
	}

}


