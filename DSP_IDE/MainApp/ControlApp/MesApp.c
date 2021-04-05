

#include "MesApp.h"
//#include "UserApp/UserApp.h"
//#include "Ial/IalControl/IalControl.h"

#include "Hal/HalAi.h"
#include "Ial/IalControl/IalControl.h"
#include "Hal/HalFComm.h"


extern uint32_t	ulInput[];
extern uint32_t	ulOutput[];


static float *t_pfMesInput;
static float *t_pfMesOutput;

static uint32_t *t_pulMesInput;
static uint32_t *t_pulMesOutput;

static uint32_t	t_MesInputSize;
static uint32_t	t_MesOutputSize;

//uint32_t	t_BrdId;

uint32_t	ulMesMode;
uint32_t	ulMesId 	 = IALMES_ID_START;

// 입력 버퍼
ST_AIDATA_TYPE		*pstMesAiData;
ST_MESRX_LS_TYPE	*pstMesRxLsData;

// 출력 버퍼
ST_MESTX_LS_TYPE	*pstMesTxLsData;
ST_MESTX_HS_TYPE	*pstMesTxHsData;

// Hmi
ST_MESCPURX_TYPE	*t_stMesCpuRx;
ST_MESCPUTX_TYPE	*t_stMesCpuTx;

void SetMesInputBuffer(uint32_t *pulInput, uint32_t ulInputSize)
{
	t_pfMesInput = (float *) pulInput;
	t_pulMesInput = pulInput;
	t_MesInputSize = ulInputSize;

	pstMesAiData = (ST_AIDATA_TYPE *)pulInput;
//	pstMesTxHsData = (ST_MESTX_HS_TYPE *)pulInput;
//	pstMesTxLsData = (ST_MESTX_LS_TYPE *)pulInput;
}

//void SetMesOutputBuffer(float *pfOutput, uint32_t ulOutputSize)
void SetMesOutputBuffer(uint32_t *pulOutput, uint32_t ulOutputSize)
{
	t_pfMesOutput = (float *)pulOutput;
	t_MesOutputSize = ulOutputSize;
	t_pulMesOutput = (uint32_t *) pulOutput;

	pstMesAiData	= (ST_AIDATA_TYPE *)&(t_pulMesInput[IALMES_AI_START]);
	pstMesRxLsData	= (ST_MESRX_LS_TYPE *)&(t_pulMesInput[IALMES_FCOMM_CH1_START]);

	pstMesTxHsData	= (ST_MESTX_HS_TYPE *)&(t_pulMesOutput[IALMES_FCOMM_CH1_START]);
	pstMesTxLsData	= (ST_MESTX_LS_TYPE *)&(t_pulMesOutput[IALMES_FCOMM_CH1_START]);
}

void MesAppInit(void)
{
	uint32_t uli;

	t_stMesCpuRx	= (ST_MESCPURX_TYPE*)(t_pulMesInput + IALMES_HMI_START);;
	t_stMesCpuTx	= (ST_MESCPUTX_TYPE*)(t_pulMesOutput + IALMES_HMI_START);;

	

}

void MesAppIOHandle(void)
{

	/* Step 1: Process IO Read */
//	IalMesWrite();
//	IalMesRead();

}

#define CurrentScale	1000
#define VoltageScale	1000


void MesAppRun(void)
{


	uint32_t	abData[10];


	// Ai Read



	if(ulMesId == IALMES_ID_START)
	{
		// Do Nothing
	}
	else
	{
		// Read Optical Relay Channel(Rx)
	}
	IalControlFiberRead(HALFCOMM_CH_1, abData, 1);






//	
//	
//	
//	
//		if(ulMesMode == IALMES_HIGH_SPEED)
//		{
//			// Analog Data 변환 후 송신만 하므로
//			// 아날로그 입력 값을 위치에 맞게 송신 프레임 위치로 복사한다.
//			pstMesTxHsData->Ia_t.fData = pstMesAiData->Ch1.fData * CurrentScale;
//			pstMesTxHsData->Ib_t.fData = pstMesAiData->Ch2.fData * CurrentScale;
//			pstMesTxHsData->Ic_t.fData = pstMesAiData->Ch3.fData * CurrentScale;
//			pstMesTxHsData->Ia_P.fData = pstMesAiData->Ch4.fData * CurrentScale;
//			pstMesTxHsData->Ib_P.fData = pstMesAiData->Ch5.fData * CurrentScale;
//			pstMesTxHsData->Ic_P.fData = pstMesAiData->Ch6.fData * CurrentScale;
//			pstMesTxHsData->Ia_N.fData = pstMesAiData->Ch7.fData * CurrentScale;
//			pstMesTxHsData->Ib_N.fData = pstMesAiData->Ch8.fData * CurrentScale;
//			pstMesTxHsData->Ic_N.fData = pstMesAiData->Ch9.fData * CurrentScale;
//		}
//		else		// LOW SPEED Mode
//		{
//	
//			// Analog Data 변환 후, Preprocessing 
//			// Board ID 별로 알고리즘 및 동작이 다름.
//	
//			// Step 1. 순시 값을적정 Buffer 에 복사하고
//			pstMesTxLsData->Ia_t.fData = pstMesAiData->Ch1.fData * CurrentScale;
//			pstMesTxLsData->Ib_t.fData = pstMesAiData->Ch2.fData * CurrentScale;
//			pstMesTxLsData->Ic_t.fData = pstMesAiData->Ch3.fData * CurrentScale;
//			pstMesTxLsData->Ia_P.fData = pstMesAiData->Ch4.fData * CurrentScale;
//	
//	
//			// Step 2. 알고리즘을 순서대로 수행하며, 버퍼를 채운다.
//			
//			
//		}

}

