#include "IalHmi.h"
#include "IalControl/IalControl.h"
//uint32_t	*pTxBaseAddr;
//uint32_t	*pRxBaseAddr;

//uint32_t	ulUpdateCnt;



#ifdef FEATURE_IAL_BUFFERINTEGRATION
extern uint32_t	ulInput[];
extern uint32_t	ulOutput[];
#endif

extern uint32_t	ulCfgSystemCfg;

uint32_t	ulHmiDummy;		// 유효한 등록 개수를 4의 배수로 맞추기 위한 Dummy

//	uint32_t	*pulToHmi[HALVME_USERDATA_SIZE] = {
//		&ulOutput[0], 	&ulOutput[1],	&ulOutput[2],	&ulOutput[3], 	&ulOutput[4],	&ulOutput[5], 	&ulOutput[6],	&ulOutput[7],	// 8
//	
//	
//	};

uint32_t	*pulToHmiStart = &ulOutput[IALCONTROL_VME_OFFSET];
uint32_t	ulToHmiLen	= IALHMI_INTERFACE_LEN;	// Active Length
uint32_t	ulToIdx		= 0;


//	uint32_t	*pulFromHmi[HALVME_USERDATA_SIZE] = {
//		&ulInput[0], 	&ulInput[1],	&ulInput[2],	&ulInput[3], 	&ulInput[4],	&ulInput[5], 	&ulInput[6],	&ulInput[7],	// 8
//	
//	};
uint32_t	*pulFromHmiStart = &ulInput[IALCONTROL_VME_OFFSET];
uint32_t	ulFromHmiLen	= IALHMI_INTERFACE_LEN;	// Active Length
uint32_t	ulFromIdx		= 0;


void IalHmiInit(void)
{


	/* Step 0: Init Ial Layer */
	ulToHmiLen = IALHMI_INTERFACE_LEN;			// Active Length
	ulToIdx	= 0;

	ulFromHmiLen	= IALHMI_INTERFACE_LEN;	// Active Length
	ulFromIdx	= 0;

	switch(ulCfgSystemCfg)
	{
		case SYSTEM_CCS:
//			pulToHmiStart
			break;
		case SYSTEM_VBC:
			break;
		case SYSTEM_MES:
		default:
			break;
	}


	/* Step 1: Init Hal Layer */
	HalVmeInit();


	/* Step 2:  */


	/* Step 3:  */


	/* Step 4:  */


}

// Update Hmi Date with only predefine size
void IalHmiUpdate(void)
{
#if 1
	/* Step 0: Output Part */
	if(HAL_NOERR == HalVmeWriteUserDate(ulToIdx, pulToHmiStart,  IALHMI_PROC_UNIT))
	{
		ulToIdx += IALHMI_PROC_UNIT;
		if(ulToIdx >= ulToHmiLen)
		{
			ulToIdx = 0;
		}
	}
	
	/* Step 1: Input Part */
	if(HAL_NOERR == HalVmeReadUserDate(ulFromIdx, pulFromHmiStart,  IALHMI_PROC_UNIT))
	{
		ulFromIdx+= IALHMI_PROC_UNIT;
		if(ulFromIdx >= ulFromHmiLen)
		{
			ulFromIdx = 0;
		}
	}
#else
	/* Step 0: Output Part */
	if(HAL_NOERR == HalVmeWriteUserDate(ulToIdx, pulToHmi,  IALHMI_PROC_UNIT))
	{
		ulToIdx += IALHMI_PROC_UNIT;
		if(ulToIdx >= ulToHmiLen)
		{
			ulToIdx = 0;
		}
	}
	
	/* Step 1: Input Part */
	if(HAL_NOERR == HalVmeReadUserDate(ulFromIdx, pulFromHmi,  IALHMI_PROC_UNIT))
	{
		ulFromIdx+= IALHMI_PROC_UNIT;
		if(ulFromIdx >= ulFromHmiLen)
		{
			ulFromIdx = 0;
		}
	}

#endif
}



void IalHmiRun(void)
{


	/* Step 0: Init Ial Layer */



	/* Step 1: Init Hal Layer */



	/* Step 2:  */


	/* Step 3:  */


	/* Step 4:  */


}



