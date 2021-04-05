

#include "VbcApp.h"
#include "Ial/IalControl/IalControl.h"


extern uint32_t	ulInput[];
extern uint32_t	ulOutput[];

extern uint32_t	ulFCommEnable[HALFCOMM_CH_NUM];
extern uint32_t	ulFCommRxLen[HALFCOMM_CH_NUM];
extern uint32_t	ulFCommTxLen[HALFCOMM_CH_NUM];


static float *t_pfVbcInput;
static float *t_pfVbcOutput;
static uint32_t *t_pulVbcInput;
static uint32_t *t_pulVbcOutput;

static uint32_t	t_VbcInputSize;
static uint32_t	t_VbcOutputSize;

// Input
ST_COMPACT_TYPE	*t_pstRtds_RxUpper;
ST_COMPACT_TYPE	*t_pstRtds_RxLower;
ST_VBCRX_MES_TYPE	*t_stVbcMesRx;
ST_VBCRX_TYPE		*t_stVbcRx;
ST_VBCCPURX_TYPE	*t_stVbcCpuRx;

// Output
ST_VBCTX_TYPE	*t_stVbcTx;;
ST_FP_TYPE		*t_pstRtds_FpUpper;
ST_FP_TYPE		*t_pstRtds_FpLower;

// Hmi
ST_VBCCPURX_TYPE	*t_stVbcCpuRx;
ST_VBCCPUTX_TYPE	*t_stVbcCpuTx;


void SetVbcInputBuffer(uint32_t *pfInput, uint32_t ulInputSize)
{
	t_pfVbcInput = (float *)pfInput;
	t_pulVbcInput = pfInput;
	t_VbcInputSize = ulInputSize;
}

void SetVbcOutputBuffer(uint32_t *pfOutput, uint32_t ulOutputSize)
{
	t_pfVbcOutput = (float *)pfOutput;
	t_pulVbcOutput = pfOutput;
	t_VbcOutputSize = ulOutputSize;
}


void VbcAppInit(void)
{

	/* Step 1: Init App Variables */
	t_pstRtds_RxUpper = (ST_COMPACT_TYPE*)(t_pulVbcInput + IALVBC_RTDSU_COMM_START);
	t_pstRtds_RxLower = (ST_COMPACT_TYPE*)(t_pulVbcInput + IALVBC_RTDSL_COMM_START);

	t_pstRtds_FpUpper = (ST_FP_TYPE*)(t_pulVbcOutput + IALVBC_RTDSU_COMM_START);
	t_pstRtds_FpLower = (ST_FP_TYPE*)(t_pulVbcOutput + IALVBC_RTDSL_COMM_START);

	t_stVbcCpuRx	= (ST_VBCCPURX_TYPE*)(t_pulVbcInput + IALVBC_HMI_START);;
	t_stVbcCpuTx	= (ST_VBCCPUTX_TYPE*)(t_pulVbcOutput + IALVBC_HMI_START);;


	/* Step 2: Init Ial Layer */

	/* Step 3:  */


	/* Step 4:  */



	

}

void VbcAppIOHandle(void)
{

	/* Step 1: Process IO Read */
//	IalVbcWrite();
//	IalVbcRead();

}


void VbcAppRun(void)
{

	/* Step 1: Upper Arm Algorithm */

	/* Step 2: Low Arm Algorithm */

}

