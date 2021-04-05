

#include "ControlApp.h"
#include "UserApp/UserApp.h"

//	#include "MesApp.h"
//	#include "CcsApp.h"
//	#include "VbcApp.h"
//#include "VbclApp.h"





void ControlAppInit(void)
{
	
	/* Step 1: Init App Variables */


	/* Step 2: Init Ial Layer */
	IalControlInit();

	/* Step 3:  */


	/* Step 4:  */
	SetMesInputBuffer(uInputBuffer.pulBuffer, IALCONTROL_INPUT_SIZE);
	SetMesOutputBuffer(uOutputBuffer.pulBuffer, IALCONTROL_OUTPUT_SIZE);

	SetCcsInputBuffer(uInputBuffer.pulBuffer, IALCONTROL_INPUT_SIZE);
	SetCcsOutputBuffer(uOutputBuffer.pulBuffer, IALCONTROL_OUTPUT_SIZE);

	SetVbcInputBuffer(uInputBuffer.pulBuffer, IALCONTROL_INPUT_SIZE);
	SetVbcOutputBuffer(uOutputBuffer.pulBuffer, IALCONTROL_OUTPUT_SIZE);


#ifdef FEATURE_SYSTEM_CONFIGURABLE
	switch(ulSystemCfg)
	{
		case SYSTEM_MES:
			MesAppInit();
			break;
		case SYSTEM_CCS:
			CcsAppInit();
			break;
		case SYSTEM_VBC:
			VbcAppInit();
			break;
		default:
			DelayUs(10);
			break;
	}
#endif



}

void ControlAppIOHandle(void)
{

	/* Step 1: Process IO Read */
	IalControlSend();
	IalControlReceive();
//	HalVmeHandle();
	

}

uint32_t ulAlgorithmPeriod = 80;

void ControlAppRun(void)
{

	/* Step 1: Process IO Read */
//	UserAppDefault();

//	ControlAppIOHandle();

#ifdef FEATURE_SYSTEM_CONFIGURABLE
	switch(ulSystemCfg)
	{
		case SYSTEM_MES:
			MesAppRun();
			break;
		case SYSTEM_CCS:
			CcsAppRun();
			break;
		case SYSTEM_VBC:
			VbcAppRun();
			break;
		default:
			DelayUs(ulAlgorithmPeriod);
			break;
	}
#endif




	/* Step 2: Parse Cmd according to Result of Step 1 */




	/* Step 3: Handle Command */
	// Dispatch Command and Execute Handler



	/* Step 4: Flush Read Buffer */




}

