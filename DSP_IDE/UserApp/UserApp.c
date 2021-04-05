#include "UserApp.h"







void MesAppDefault(void)
{


#ifdef FEATURE_SYSTEM_CONFIGURABLE


	// 0. Tx Fcomm(Ch 2,3,4) Data and Sync Signal
//	HalFCommTrigger();
//	HalDioHandle();

	// 1.	Read Analog
	HalAiHandle();

	// 2.	Preprocess Analog Data
	// MesAlgorithm();
	
	//	2.1	Tx 1st Proprocessed Data
	//	2.2 Rx 1st Preprocessed Data
	// 3.	Tx All Data through All Channel
	
#endif
	return;
}


void CcsAppDefault(void)
{


#ifdef FEATURE_SYSTEM_CONFIGURABLE
	// 1.	Sync to MES, Tx Previous Out Data (to 3xVBC)
	// 2.	Rx Input Data 
	//	2.1	Rx MES Data
	//	2.2 Rx 3xVBC Data
	// 3.	Run CCS Algorithm
	
#endif
	return;
}


void VbcAppDefault(void)
{


#ifdef FEATURE_SYSTEM_CONFIGURABLE
	// 1.	Sync to MES, Tx Previous Out Data (to CCS, VBCL)
	// 2.	Rx Input Data 
	//	2.1	Rx MES Data
	//	2.2 Rx CCS Data
	//	2.2 Rx VBCL Data
	// 3.	Run VBC Algorithm
	
#endif
	return;
}

void UserAppDefault(void)
{


	return;
}

