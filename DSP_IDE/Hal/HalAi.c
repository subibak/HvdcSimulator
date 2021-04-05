

#include "HalAi.h"


/***
 * Data Definition
 */
ST_AI_CONF_TYPE	t_stAiConfigData;

void HalAiInit(void)
{
	uint32_t ulLoop;
	ST_AI_CONF_TYPE	*pstAiConfigData = &t_stAiConfigData;	

	for(ulLoop=0;ulLoop<HALAI_CH_NUM;ulLoop++)
	{
		pstAiConfigData->afAiData[ulLoop] = 0;
	}


	ReadReg(HWREG_ADC_CTRL_ADDR, &(pstAiConfigData->ulAiConfig));

	pstAiConfigData->ulAiConfig = pstAiConfigData->ulAiConfig | HWREG_ADC_CTRL_RST_YES;

	pstAiConfigData->ulAiConfig = pstAiConfigData->ulAiConfig & (~HWREG_ADC_CTRL_RST_MASK);
	pstAiConfigData->ulAiConfig = pstAiConfigData->ulAiConfig & (~HWREG_ADC_CTRL_RANGE_5V);

	WriteReg(HWREG_ADC_CTRL_ADDR, pstAiConfigData->ulAiConfig);
	
}

float HalAiRead(uint32_t	ulChNum)
{

	float		*pfAiRead = t_stAiConfigData.afAiData;

#ifdef FEATURE_AI_SEPERATE_ACCESS
	uint32_t	*pulAiRead = t_stAiConfigData.aulAiData; 

	ReadReg(HWREG_ADC_INPUT_ADDR + ulChNum, pulAiRead);
	pfAiRead[ulChNum] = Hex2mV(ADC_INPUT_MAX, ADC_INPUT_MIN, ADC_INPUT_MAX_HEX, ADC_INPUT_MIN_HEX, *pulAiRead);
//	ReadReg(HWREG_ADC_INPUT_ADDR + ulChNum - 1, pulAiRead);
//	pfAiRead[ulChNum - 1] = Hex2mV(ADC_INPUT_MAX, ADC_INPUT_MIN, ADC_INPUT_MAX_HEX, ADC_INPUT_MIN_HEX, *pulAiRead);
#endif

	return pfAiRead[ulChNum];
//	return pfAiRead[ulChNum - 1];

}


void HalAiHandle(void)
{
	float		*pfAiRead = t_stAiConfigData.afAiData;
	uint32_t	*pulAiRead = t_stAiConfigData.aulAiData; 
	uint32_t	ulChNum;

	for(ulChNum=0;ulChNum<HALAI_CH_NUM;ulChNum++)
	{
		ReadReg(HWREG_ADC_INPUT_ADDR + ulChNum, &(pfAiRead[ulChNum]));
		pfAiRead[ulChNum] = Hex2mV(ADC_INPUT_MAX, ADC_INPUT_MIN, ADC_INPUT_MAX_HEX, ADC_INPUT_MIN_HEX, pfAiRead[ulChNum]);
	}
}



