

/**
 * HalVme.c
 * HalVme Device Driver Module Body
 */


#include "HalVme.h"

HALVME_SYSTEM_HEADER	*pstVmeSystemHeader;
//HALVME_USERDATA_TYPE		*pstVmeUserData;

ST_BUFFER_TYPE	t_stVmeTxBuffer;
ST_BUFFER_TYPE	t_stVmeRxBuffer;

uint32_t	*pulTxData = (uint32_t *) (HWREG_VMES_BASE + HWREG_VMES_WUSER_OFFSET);
uint32_t	*pulRxData = (uint32_t *) (HWREG_VMES_BASE + HWREG_VMES_RUSER_OFFSET);
uint32_t	ulSlotId;


HAL_ERROR HalVmeSysCfgInit(void)
{
	uint32_t	ulCfg = 0;

	ulCfg = ulCfg | HWREG_VME_SCON_MREAD_16BIT;
	WriteReg(HWREG_VME_SCON, ulCfg);


	ReadReg(HWREG_VME_SCTRL,&ulSlotId);
	ulSlotId = (ulSlotId & HWREG_VME_SCTRL_SLOTID_MASK);
	ulSlotId = ulSlotId >> 24;

	return HAL_NOERR;
	
}


HAL_ERROR HalVmeMasterInit(void)
{
	uint32_t	ulCfg = 0;

	ulCfg = ulCfg | HWREG_VME_MCFG_EXTMADD_DEF;
	ulCfg = ulCfg | HWREG_VME_MCFG_STDMADD_DEF;
	ulCfg = ulCfg | HWREG_VME_MCFG_EXTMAM_NPRIV;
	ulCfg = ulCfg | HWREG_VME_MCFG_STDMAM_NPRIV;

	WriteReg(HWREG_VME_MCFG, ulCfg);

	return HAL_NOERR;
	
}

HAL_ERROR HalVmeSlaveInit(void)
{
	uint32_t	ulCfg = 0;

	ulCfg = ulCfg | HWREG_VME_SCFG_EXTADD_DEF;
	ulCfg = ulCfg | HWREG_VME_SCFG_STDADD_DEF;
	ulCfg = ulCfg | HWREG_VME_SCFG_AUTOSLOT_MODE;
	ulCfg = ulCfg | (ulSlotId<<16);	

	WriteReg(HWREG_VME_SCFG, ulCfg);

	pstVmeSystemHeader	= (HALVME_SYSTEM_HEADER *)HWREG_VMES_BASE;
//	pstVmeUserData		= (HALVME_USERDATA_TYPE *)HWREG_VMES_USERDATA_BASE;
//	pulVmeDiagInfo		= (HALVME_DIAG_INFO *)HWREG_VMES_BASE;

	return HAL_NOERR;
	
}


uint32_t HalVmeGetUserBase(void)
{
	return HWREG_VMES_RUSERDATA_BASE;
}


HAL_ERROR HalVmeInit(void)
{

	t_stVmeTxBuffer.ulSize	= HALVME_USERDATA_SIZE;
	t_stVmeTxBuffer.ulHead	= 0;
	t_stVmeTxBuffer.ulTail	= 0;

	t_stVmeRxBuffer.ulSize	= HALVME_USERDATA_SIZE;
	t_stVmeRxBuffer.ulHead	= 0;
	t_stVmeRxBuffer.ulTail	= 0;

	HalVmeSysCfgInit();
	HalVmeMasterInit();
	HalVmeSlaveInit();

	return HAL_NOERR;
	
}

/* Write Data to Tx Buffer */
//HALVME_ERROR HalVmeWrite(ST_VME_BUFFER_TYPE *psData)
HAL_ERROR HalVmeWrite(uint32_t ulOffset, uint32_t *psData, uint32_t ulLen)
{
	HAL_ERROR	eResult = HAL_NOERR;
	uint32_t	ulLoop;

	CLI();

	for(ulLoop = 0; ulLoop < ulLen;ulLoop++)
	{
		WriteReg(HWREG_VMES_BASE+ulOffset+ulLoop, (psData[ulLoop]));
	}

	STI();

	return eResult;

}

HAL_ERROR HalVmeWriteUserDate(uint32_t ulOffset, uint32_t psData[], uint32_t ulLen)
{
	HAL_ERROR	eResult = HAL_NOERR;
	uint32_t	ulLoop;

	CLI();

	for(ulLoop = 0; ulLoop < ulLen; ulLoop++)
	{
		WriteReg(HWREG_VMES_WUSERDATA_BASE+ulOffset+ulLoop, (psData[ulOffset + ulLoop]));
	}

	STI();

	return eResult;

}


HAL_ERROR HalVmeReadUserDate(uint32_t ulOffset, uint32_t psData[], uint32_t ulLen)
{
	HAL_ERROR	eResult = HAL_NOERR;
	uint32_t	ulLoop;

	CLI();

	for(ulLoop = 0; ulLoop < ulLen; ulLoop++)
	{
		ReadReg(HWREG_VMES_RUSERDATA_BASE+ulOffset+ulLoop, &(psData[ulOffset + ulLoop]));
	}

	STI();

	return eResult;

}

/* Read Data from Rx Buffer until '\r\n'*/
HAL_ERROR HalVmeRead(uint32_t ulOffset, uint32_t *psData, uint32_t ulLen)
{
	HAL_ERROR	eResult = HAL_NOERR;
	uint32_t	ulLoop;
	uint32_t	ulBaseAddr = HWREG_VMES_BASE;

	CLI();

	for(ulLoop = 0; ulLoop < ulLen;ulLoop++)
	{
		ReadReg(HWREG_VMES_BASE+ulOffset+ulLoop, &(psData[ulLoop]));
	}

	STI();

	return eResult;

}

/* Operation between Vme Buffer and Vme Hw*/
uint32_t HalVmeHandle(void)
{
	HAL_ERROR	eResult = HAL_NOERR;
	uint32_t uli, ulStart;


	// Data Read
	ulStart = t_stVmeRxBuffer.ulHead;
	for(uli = 0;uli < 4; uli++)
	{
		if(ulStart + uli >= t_stVmeRxBuffer.ulSize)
		{
			ulStart = 0; uli = 0;
			break;
		}
		t_stVmeRxBuffer.uBody.p32Buffer[uli + ulStart] = pulRxData[uli + ulStart];
		
	}
	t_stVmeRxBuffer.ulHead = (ulStart + uli);

	// Data Write
	ulStart = t_stVmeTxBuffer.ulHead;
	for(uli = 0;uli < 4; uli++)
	{
		if(ulStart + uli >= t_stVmeTxBuffer.ulSize)
		{
			ulStart = 0; uli = 0;
			break;
		}
		pulTxData[uli + ulStart] = t_stVmeTxBuffer.uBody.p32Buffer[uli + ulStart];
	}
	t_stVmeTxBuffer.ulHead = (ulStart + uli);
	

	

	return eResult;

}

HAL_ERROR HalVmeSetRBuffer(uint32_t *pReadBuffer,uint32_t ulLen)
{
	HAL_ERROR	eResult = HAL_NOERR;

	t_stVmeRxBuffer.uBody.p32Buffer = pReadBuffer;
	t_stVmeRxBuffer.ulSize 			= ulLen;

	return eResult;

}

HAL_ERROR HalVmeSetWBuffer(uint32_t *pWriteBuffer,uint32_t ulLen)
{
	HAL_ERROR	eResult = HAL_NOERR;

	t_stVmeTxBuffer.uBody.p32Buffer = pWriteBuffer;
	t_stVmeTxBuffer.ulSize 			= ulLen;

	return eResult;

}

#ifdef FEATURE_PERIPHERAL_TEST

#define VME_TEST_FRACTION_SIZE	256
#define VME_TEST_1ITERATION		(HWREG_VMES_SIZE/VME_TEST_FRACTION_SIZE)


HAL_ERROR HalVmeLoopBackTest(uint32_t *pulOkCnt, uint32_t *pulErrCnt)
{
	ST_BUFFER_TYPE	*pTxBuffer = &(t_stVmeTxBuffer);
	ST_BUFFER_TYPE	*pRxBuffer = &(t_stVmeRxBuffer);
	static uint32_t	ulTxData[VME_TEST_FRACTION_SIZE];
	static uint32_t	ulRxData[VME_TEST_FRACTION_SIZE];

	uint32_t ulTxRegAddr;
	uint32_t ulRxRegAddr;
	uint32_t ulLoop = 0;
	static uint32_t ulFraction = 0;

	uint32_t	ulOkCnt		= 0;
	uint32_t	ulErrCnt	= 0;


	ulTxRegAddr = HWREG_VMES_BASE + (ulFraction * VME_TEST_FRACTION_SIZE);
	ulRxRegAddr = HWREG_VMES_BASE + (ulFraction * VME_TEST_FRACTION_SIZE);

	// RxData 1st
	HalVmeRead((VME_TEST_FRACTION_SIZE*ulFraction), ulRxData, VME_TEST_FRACTION_SIZE);

	// Check Data
	for(ulLoop = 0; ulLoop < VME_TEST_FRACTION_SIZE; ulLoop++)
	{
		if(ulRxData[ulLoop] == ulTxData[ulLoop])
		{
			ulOkCnt++;
		}
		else
		{
			ulErrCnt++;
		}
	}

	// Refresh Tx Data
	for(ulLoop = 0; ulLoop < VME_TEST_FRACTION_SIZE; ulLoop++)
	{
		ulTxData[ulLoop] = ulTxData[ulLoop] + ulLoop;
	}

	ulFraction++;
	if(ulFraction == VME_TEST_1ITERATION)
	{
		ulFraction = 0;
	}
	HalVmeWrite((VME_TEST_FRACTION_SIZE*ulFraction), ulTxData, VME_TEST_FRACTION_SIZE);

	// Tx Data
	*pulOkCnt = ulOkCnt;
	*pulErrCnt = ulErrCnt;


	return HAL_NOERR;

}

#endif



