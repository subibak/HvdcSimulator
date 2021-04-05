

#include "CcsApp.h"
#include "UserApp/UserApp.h"
#include "Ial/IalControl/IalControl.h"



static float *t_pfCcsInput;
static float *t_pfCcsOutput;
static uint32_t *t_pulCcsInput;
static uint32_t *t_pulCcsOutput;


static uint32_t	t_CcsInputSize;
static uint32_t	t_CcsOutputSize;


extern uint32_t	ulInput[];
extern uint32_t	ulOutput[];


// Input
ST_CCSRX_MES_TYPE *t_pstCcsMesRx;
ST_CCSRX_TYPE *t_pstCcsRxPhA, *t_pstCcsRxPhB, *t_pstCcsRxPhC;

// Output
ST_CCSTX_TYPE *t_pstCcsTxPhA, *t_pstCcsTxPhB, *t_pstCcsTxPhC;

// Hmi
ST_CCSCPURX_TYPE	*t_stCcsCpuRx;
ST_CCSCPUTX_TYPE	*t_stCcsCpuTx;

void SetCcsInputBuffer(uint32_t *pfInput, uint32_t ulInputSize)
{
	t_pfCcsInput = (float *)pfInput;
	t_pulCcsInput = pfInput;
	t_CcsInputSize = ulInputSize;

	t_pstCcsRxPhA = (ST_CCSRX_TYPE*) (t_pulCcsInput + IALCCS_VBC_PHA_START);
	t_pstCcsRxPhB = (ST_CCSRX_TYPE*) (t_pulCcsInput + IALCCS_VBC_PHB_START);
	t_pstCcsRxPhC = (ST_CCSRX_TYPE*) (t_pulCcsInput + IALCCS_VBC_PHC_START);
	t_pstCcsMesRx	= (ST_CCSRX_MES_TYPE *) (t_pulCcsInput + IALCCS_MES_START);
}

void SetCcsOutputBuffer(uint32_t  *pfOutput, uint32_t ulOutputSize)
{
	t_pfCcsOutput = (float *)pfOutput;
	t_pulCcsOutput = pfOutput;
	t_CcsOutputSize = ulOutputSize;

	t_pstCcsTxPhA = (ST_CCSTX_TYPE*) (t_pulCcsOutput + IALCCS_VBC_PHA_START);
	t_pstCcsTxPhB = (ST_CCSTX_TYPE*) (t_pulCcsOutput + IALCCS_VBC_PHB_START);
	t_pstCcsTxPhC = (ST_CCSTX_TYPE*) (t_pulCcsOutput + IALCCS_VBC_PHC_START);
}


void CcsAppInit(void)
{
	uint32_t	uli;


	t_stCcsCpuRx	= (ST_CCSCPURX_TYPE*)(t_pulCcsInput + IALVBC_HMI_START);;
	t_stCcsCpuTx	= (ST_CCSCPUTX_TYPE*)(t_pulCcsOutput + IALVBC_HMI_START);;


	/* Step 1: Init App Variables */

//		for(uli=0;uli < IALCONTROL_FCOMM3_OFFSET; uli++)
//		{
//			t_pulCcsOutput[uli] = uli;
//			
//		}
	

	/* Step 2: Init Ial Layer */

	/* Step 3:  */


	/* Step 4:  */



	

}

void CcsAppIOHandle(void)
{

	/* Step 1: Process IO Read */
//	IalCcsWrite();
//	IalCcsRead();

}


#define VME_COMM_SIZE	40

#define VME_VBC1_OFFSET	0
#define VME_VBC2_OFFSET 40
#define VME_VBC3_OFFSET 80

void CcsAppRun(void)
{



}

