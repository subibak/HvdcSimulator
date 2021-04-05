

/**
 * HalFcomm.c
 * HalFcomm Device Driver Module Body
 */


#include "HalFComm.h"

// Buffer Instance
ST_FCOMM_TYPE	t_stFCommTx[HALFCOMM_CH_NUM];
ST_FCOMM_TYPE	t_stFCommRx[HALFCOMM_CH_NUM];

uint32_t		ulTempBuffer[HALFCOMM_BUFFER_SIZE];

//  Buffer Raw Data for Handling in Hal Layer
//uint32_t		aulTxBuffer[HALFCOMM_BUFFER_SIZE];		
//uint32_t		aulRxBuffer[HALFCOMM_BUFFER_SIZE];		

uint32_t		*aulTxBuffer;
uint32_t		*aulRxBuffer;


extern uint32_t ulFcommOkCnt[];
extern uint32_t ulFcommCrcErrCnt[];
extern uint32_t ulFcommNoPktErrCnt[];




HAL_ERROR HalFCommInit(void)
{

	uint32_t ulLoop;

	uint32_t	ulRegValue;
	uint32_t	ulRegAddr;

	ST_BUFFER_TYPE	*pTxBuffer;
	ST_BUFFER_TYPE	*pRxBuffer;

	for(ulLoop = HALFCOMM_CH_1; ulLoop < HALFCOMM_CH_NUM; ulLoop++)
	{
		pTxBuffer = &(t_stFCommTx[ulLoop].stBuffer);
		pRxBuffer = &(t_stFCommRx[ulLoop].stBuffer);

		// Tx Part
		t_stFCommTx[ulLoop].ulEnable	= HALFCOMM_ENABLE;
		t_stFCommTx[ulLoop].ulSize		= HALFCOMM_BUFFER_SIZE;

		pTxBuffer->ulSize			= HALFCOMM_BUFFER_SIZE;
		pTxBuffer->ulHead			= 0x0;
		pTxBuffer->ulTail			= 0x0;
		pTxBuffer->ulUnusedSize		= HALFCOMM_BUFFER_SIZE;
		pTxBuffer->ulUsedSize		= 0x0;
//		pTxBuffer->uBody.p32Buffer	= aulTxBuffer;

		// Rx Part
		t_stFCommRx[ulLoop].ulEnable	= HALFCOMM_ENABLE;
		t_stFCommRx[ulLoop].ulSize		= HALFCOMM_BUFFER_SIZE;

		pRxBuffer->ulSize			= HALFCOMM_BUFFER_SIZE;
		pRxBuffer->ulHead			= 0x0;
		pRxBuffer->ulTail			= 0x0;
		pRxBuffer->ulUnusedSize		= HALFCOMM_BUFFER_SIZE;
		pRxBuffer->ulUsedSize		= 0x0;
//		pRxBuffer->uBody.p32Buffer	= aulRxBuffer;

		// Channel Length	
		HalFCommSetSize(ulLoop, TXSIZE(HALFCOMM_BUFFER_SIZE));

		ulFcommOkCnt[ulLoop] = 0;
		ulFcommCrcErrCnt[ulLoop] = 0;
		ulFcommNoPktErrCnt[ulLoop] = 0;

	}

	// Channel Enable/Disable
	ReadReg(HWREG_SFP_TX_DIS_ADDR, &ulRegValue);
	BitClear(ulRegValue, (HWREG_SFP_TX_DIS_CH1_MASK | HWREG_SFP_TX_DIS_CH2_MASK | HWREG_SFP_TX_DIS_CH3_MASK | HWREG_SFP_TX_DIS_CH4_MASK));
	WriteReg(HWREG_SFP_TX_DIS_ADDR, ulRegValue);

	return HAL_NOERR;
	
}

HAL_ERROR HalFCommEna(uint32_t ulChNum, uint32_t ulEnable)
{

	uint32_t	ulReg = 0, ulSftReg = 0;
	ReadReg(HWREG_FCOMM_CTRLREG_ADDR, &ulReg);
	ReadReg(HWREG_SFP_TX_DIS_ADDR, &ulSftReg);
	
	if(ulEnable == HALFCOMM_ENABLE)
	{
		BitClear(ulReg, (0x1<<(HALFCOMM_CH_4 - ulChNum)));
		BitClear(ulSftReg, (0x1<<ulChNum));
	}
	else
	{
		BitSet(ulReg, (0x1<<(HALFCOMM_CH_4 - ulChNum)));
		BitSet(ulSftReg, (0x1<<ulChNum));
	}

	WriteReg(HWREG_FCOMM_CTRLREG_ADDR, ulReg);
	WriteReg(HWREG_SFP_TX_DIS_ADDR, ulSftReg);
	
	return HAL_NOERR;
}

HAL_ERROR HalFCommSetSize(uint32_t ulChNum, uint32_t	ulSize)
{

	uint32_t	uli, ulReg = 0;
	uint32_t	ulRegAddr, ulSizeConv;

	ulRegAddr = HWREG_FCOMM_STAT_ADDR(ulChNum);
	ReadReg(ulRegAddr, &ulReg);
	BitClear(ulReg, HWREG_FCOMM_STAT_SIZE_MASK);
	BitSet(ulReg, ulSize<<HWREG_FCOMM_STAT_SIZE_SHIFT);
	WriteReg(ulRegAddr, ulReg);

	return HAL_NOERR;
}


HAL_ERROR HalFCommStsCheck(uint32_t ulChNum)
{

	uint32_t	ulReg = 0, ulEnable;
	uint32_t	ulRegAddr;
	HAL_ERROR	eReturn = HAL_NOERR;

	ulRegAddr = HWREG_FCOMM_STAT2_ADDR(ulChNum);
	ReadReg(ulRegAddr, &ulReg);
#if 0
	ReadReg(HWREG_FCOMM_CTRLREG_ADDR, &ulEnable);

	if((ulEnable&(HALFCOMM_RESET<<(HALFCOMM_CH_4 - ulChNum))) == HALFCOMM_RESET)
	{
		eReturn = HALFCOMM_DISABLED_ERROR;
	}
	else
	{
#endif

		if((ulReg&HWREG_FCOMM_STAT2_RXLOSS_MASK) == HWREG_FCOMM_STAT2_RXLOSS_MASK)
		{
			// Rx Loss : Cable Disconnect
			eReturn = HALFCOMM_RXLOSS_ERROR;
		}

//	}

	return eReturn;
}

HAL_ERROR HalFCommReadSeq(uint32_t ulChNum, uint32_t *pulData)
{
	ReadReg(HWREG_FCOMM_RXB_ADDR(ulChNum) + PROTOCOL_SEQ_IDX, pulData);
	return HAL_NOERR;
}


HAL_ERROR HalFCommReadSync(uint32_t	*pulOldSeq)
{
	uint32_t	ulSyncData, ulStartCode;
	HAL_ERROR	eResult = HAL_NOERR;
	ReadReg(HWREG_FCOMM_RXB_ADDR(HALFCOMM_CH_1) + PROTOCOL_STARTCODE_IDX, &ulStartCode);
	ReadReg(HWREG_FCOMM_RXB_ADDR(HALFCOMM_CH_1) + PROTOCOL_SEQ_IDX, &ulSyncData);
	ulSyncData = ulSyncData & PROTOCOL_SEQ_MASK;
	static uint32_t	ulFlag = false;

	if(ulStartCode == PROTOCOL_STARTCODE)
	{
		if((ulSyncData == *pulOldSeq)) // + 1))
		{
//			*pulOldSeq = ulSyncData;
			eResult = HALFCOMM_NOSYNC_ERROR;

		}
		else
		{
			if(*pulOldSeq + 1 == ulSyncData)
			{
				*pulOldSeq = ulSyncData;
			}
			else
			{
				if(ulFlag == false)
				{
					ulFlag = true;
					*pulOldSeq = ulSyncData;
				}
				else
				{
					*pulOldSeq = ulSyncData;
				}
//				eResult = HALFCOMM_NOSYNC_ERROR;
			}
//			HalUartWrite("");
		}
	}
	else
	{
		eResult = HALFCOMM_NOSYNC_ERROR;
	}	
	return eResult;
}

extern uint32_t	ulFCommRxLen[HALFCOMM_CH_NUM];
extern uint32_t	ulFCommTxLen[HALFCOMM_CH_NUM];


/* Write Data to Tx Buffer */
//HAL_ERROR HalFCommWrite(uint32_t ulChNum, uint32_t pulData[], uint32_t ulLen)
HAL_ERROR HalFCommWrite(uint32_t ulChNum) //, uint32_t pulData[], uint32_t ulLen)
{

	HAL_ERROR		eResult = HAL_NOERR;
	uint32_t		ulLoop;
	uint32_t		pulTxBufferAddr = HWREG_FCOMM_TXB_ADDR(ulChNum);//HWREG_FCOMM_TXB_ADDR + FCOMM_CH_OFFSET(ulChNum);
//	uint32_t	*pBuffer = pulData;

	ST_FCOMM_TYPE *pTxBuffer = &t_stFCommTx[ulChNum];
	uint32_t	*pBuffer = pTxBuffer->stBuffer.uBody.p32Buffer;
	uint32_t 	ulLen = ulFCommTxLen[ulChNum];

//	CLI();
	// psData 의 내용을 Encode 하여 psData 의 정적 위치에 Head만 Write 하고 모두 HW Write 함.

	ProtocolEncode(ulChNum, pBuffer, ulLen);
#if 1
	memcpy((uint32_t *)pulTxBufferAddr, pBuffer, ulLen);
#else
	for(ulLoop=0; ulLoop < ulLen;ulLoop++)
	{
		WriteReg(pulTxBufferAddr+ulLoop, *(pBuffer + ulLoop));
	}
#endif
//	STI();

	return eResult;

}

/* Write Data to Tx Buffer */
HAL_ERROR HalFCommWriteNoEnc(uint32_t ulChNum) //, uint32_t pulData[], uint32_t ulLen)
{

	HAL_ERROR		eResult = HAL_NOERR;
	uint32_t		ulLoop;
	uint32_t		pulTxBufferAddr = HWREG_FCOMM_TXB_ADDR(ulChNum);//HWREG_FCOMM_TXB_ADDR + FCOMM_CH_OFFSET(ulChNum);

	ST_FCOMM_TYPE *pTxBuffer = &t_stFCommTx[ulChNum];
	uint32_t	*pBuffer = pTxBuffer->stBuffer.uBody.p32Buffer;
	uint32_t 	ulLen = ulFCommTxLen[ulChNum];

	memcpy((uint32_t *)pulTxBufferAddr, pBuffer, ulLen);

	return eResult;

}

/* Read Data from Rx Buffer until '\r\n'*/
//HAL_ERROR HalFcommRead(ST_FCOMM_BUFFER_TYPE *psData)
//HAL_ERROR HalFCommRead(uint32_t ulChNum, uint8_t *psData, uint32_t ulLen)
HAL_ERROR HalFCommRead(uint32_t ulChNum, uint32_t	*pulErr)
{

	ST_FCOMM_TYPE *pRxBuffer = &t_stFCommRx[ulChNum];
	static	uint32_t	ulCrcErrCnt[HALFCOMM_CH_NUM];
	static	uint32_t	ulSeqErrCnt[HALFCOMM_CH_NUM];

#if 1
	uint32_t	*pBuffer = pRxBuffer->stBuffer.uBody.p32Buffer;
	
	HAL_ERROR	eResult = HAL_NOERR;
	uint32_t	ulLoop;
	uint32_t	pulRxBufferAddr = HWREG_FCOMM_RXB_ADDR(ulChNum);//HWREG_FCOMM_RXB_ADDR + FCOMM_CH_OFFSET(ulChNum);
	uint32_t	ulProtocol;
	uint32_t	ulNewSeq, ulNewLen;
	uint32_t	ulStartCode;
#endif

	CLI();

	// 데이터를 Manager 로 전달
//	HalFCommReadSeq(ulChNum, &ulNewSeq);

//	ulNewLen = (ulNewSeq >> 0x10) & 0xFFFF;
//	ulNewSeq = ulNewSeq & PROTOCOL_SEQ_MASK;
	ReadReg(pulRxBufferAddr + PROTOCOL_STARTCODE_IDX, &ulStartCode);
	ReadReg(pulRxBufferAddr + PROTOCOL_SEQ_IDX, &ulNewSeq);
	ReadReg(pulRxBufferAddr + PROTOCOL_LEN_IDX , &ulNewLen);


	if(pRxBuffer->ulSize == 0)
	{
		BitClear(pRxBuffer->ulErrCause, 0xF);
	}
	else
	{

		if(ulStartCode == PROTOCOL_STARTCODE)
		{

			if(ulNewLen > 0 && ulNewLen <= HALFCOMM_BUFFER_SIZE)
			{

				if(LastRcvSeq(ulChNum) == ulNewSeq)
				{
					// No New Frame
					ulSeqErrCnt[ulChNum] = ulSeqErrCnt[ulChNum] + 0x1;
					if((ulSeqErrCnt[ulChNum] & 0xFFFF) > HALFCOMM_ERRCNT_MAX)
					{
						eResult = HALFCOMM_SEQNUM_ERROR;
						BitSet(pRxBuffer->ulErrCause, PROTOCOL_SEQ_SAME);
						ulSeqErrCnt[ulChNum] = 0x0;
					}
					ulFcommNoPktErrCnt[ulChNum]++;
			 	}
				else
				{
					// New Frame Arrived
		//			BitClear(pRxBuffer->ulErrCause, PROTOCOL_SEQ_SAME|PROTOCOL_CRC_ERROR);
					memcpy(ulTempBuffer, (uint32_t *)(pulRxBufferAddr), ulNewLen + PROTOCOL_HEADER_LEN);

					ulProtocol = ProtocolDecode(ulChNum, (ulTempBuffer), ulNewLen);		
					if(ulProtocol == PROTOCOL_CRC_ERROR)
					{
						ulCrcErrCnt[ulChNum] = ulCrcErrCnt[ulChNum] + 0x1000;
						if(((ulCrcErrCnt[ulChNum]>>16) & 0xFFFF) > HALFCOMM_ERRCNT_MAX)
						{
							BitSet(pRxBuffer->ulErrCause, PROTOCOL_CRC_ERROR);
							eResult = HALFCOMM_CRC_ERROR;
							ulCrcErrCnt[ulChNum] = 0x0;
						}
						ulFcommCrcErrCnt[ulChNum]++;

					}
					else	// No Error
					{
						ulRxSeqNum[ulChNum] = ulNewSeq;
						ulCrcErrCnt[ulChNum] = 0x0;ulSeqErrCnt[ulChNum] = 0x0;
						BitClear(pRxBuffer->ulErrCause, (PROTOCOL_CRC_ERROR | PROTOCOL_SEQ_SAME));
						memcpy(pBuffer, ulTempBuffer, ulNewLen + PROTOCOL_HEADER_LEN);
						ulFcommOkCnt[ulChNum]++;
						
						
						
					}
				}
			}
		}
		else
		{
			ulSeqErrCnt[ulChNum]++;
			if((ulSeqErrCnt[ulChNum] & 0xFFFF) > HALFCOMM_ERRCNT_MAX)
			{
				eResult = HALFCOMM_SEQNUM_ERROR;
				BitSet(pRxBuffer->ulErrCause, PROTOCOL_SEQ_SAME);
				ulSeqErrCnt[ulChNum] = 0x0;
			}
			ulFcommNoPktErrCnt[ulChNum]++;
		}


	}
	*pulErr = pRxBuffer->ulErrCause;

	return eResult;

}

/* Read Data from Rx Buffer until '\r\n'*/
HAL_ERROR HalFCommReadNoDec(uint32_t ulChNum, uint32_t	*pulErr)
{

	HAL_ERROR	eResult = HAL_NOERR;

	ST_FCOMM_TYPE *pRxBuffer = &t_stFCommRx[ulChNum];
	uint32_t	*pBuffer = pRxBuffer->stBuffer.uBody.p32Buffer;	
	uint32_t	pulRxBufferAddr = HWREG_FCOMM_RXB_ADDR(ulChNum);//HWREG_FCOMM_RXB_ADDR + FCOMM_CH_OFFSET(ulChNum);
	uint32_t 	ulLen = ulFCommRxLen[ulChNum];

	// New Frame Arrived
	memcpy(pBuffer, (uint32_t *)(pulRxBufferAddr), ulLen);

	*pulErr = PROTOCOL_OK;

	return eResult;

}


HAL_ERROR HalFCommSet(uint32_t ulChNum, uint32_t ulEnable, uint32_t ulSize)
{
	ST_BUFFER_TYPE	*pTxBuffer = &(t_stFCommTx[ulChNum].stBuffer);
	ST_BUFFER_TYPE	*pRxBuffer = &(t_stFCommRx[ulChNum].stBuffer);

	uint32_t ulRegAddr;
	uint32_t ulRegValue;

	t_stFCommTx[ulChNum].ulEnable = ulEnable;
	pTxBuffer->ulSize	= ulSize;
	t_stFCommRx[ulChNum].ulEnable = ulEnable;
	pTxBuffer->ulSize	= ulSize;

	ReadReg(HWREG_SFP_TX_DIS_ADDR, &ulRegValue);

	// Channel Enable/Disable
	ulRegAddr	= HWREG_SFP_TX_DIS_ADDR;
	if(ulEnable == HALFCOMM_ENABLE)
	{
		WriteReg(ulRegAddr, (ulRegValue | (HWREG_SFP_TX_DIS_CH1_MASK<<ulChNum)));
	}
	else
	{
		WriteReg(ulRegAddr, (ulRegValue & ~(HWREG_SFP_TX_DIS_CH1_MASK<<ulChNum)));
	}

	// Channel Length	
	HalFCommSetFrameSize(ulChNum, ulSize, ulSize);

	return HAL_NOERR;

}

HAL_ERROR HalFCommSetBuffer(uint32_t ulChNum, uint32_t *pulTxBuffer, uint32_t *pulRxBuffer)
{
	t_stFCommTx[ulChNum].stBuffer.uBody.p32Buffer = pulTxBuffer;
	t_stFCommRx[ulChNum].stBuffer.uBody.p32Buffer = pulRxBuffer;

	return HAL_NOERR;

}


HAL_ERROR HalFCommSetFrameSize(uint32_t ulChNum, uint32_t ulTxSize, uint32_t ulRxSize)
{
	uint32_t	ulSize;

	t_stFCommTx[ulChNum].stBuffer.ulSize = ulTxSize;
	t_stFCommRx[ulChNum].stBuffer.ulSize = ulRxSize;

	if(ulTxSize > ulRxSize)
	{
		ulSize = TXSIZE(ulTxSize);
 	}
	else
	{
		ulSize = TXSIZE(ulRxSize); 
	}

	HalFCommSetSize(ulChNum, ulSize);

	return HAL_NOERR;

}

#ifdef FEATURE_PERIPHERAL_TEST

uint32_t	ulTestCh = HALFCOMM_CH_1;

uint32_t	HalFCommGetTestCh(void)
{
	return ulTestCh;
}


HAL_ERROR HalFCommLoopBackTest(uint32_t *pulOkCnt, uint32_t *pulErrCnt)
{
	ST_BUFFER_TYPE	*pTxBuffer = &(t_stFCommTx[ulTestCh].stBuffer);
	ST_BUFFER_TYPE	*pRxBuffer = &(t_stFCommRx[ulTestCh].stBuffer);
	static uint32_t	ulTxData[HALFCOMM_BUFFER_SIZE];
	static uint32_t	ulRxData[HALFCOMM_BUFFER_SIZE];

	uint32_t ulTxRegAddr = HWREG_FCOMM_TXB_ADDR;
	uint32_t ulRxRegAddr = HWREG_FCOMM_RXB_ADDR;
	uint32_t ulLoop;

	uint32_t	ulOkCnt		= 0;
	uint32_t	ulErrCnt	= 0;


	

	// RxData 1st

	for(ulLoop = 0; ulLoop < HALFCOMM_BUFFER_SIZE; ulLoop++)
	{
		ReadReg(ulRxRegAddr+ulLoop,&ulRxData[ulLoop]);
	}

	// Check Data
	for(ulLoop = 0; ulLoop < HALFCOMM_BUFFER_SIZE; ulLoop++)
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
	for(ulLoop = 0; ulLoop < HALFCOMM_BUFFER_SIZE; ulLoop++)
	{
		ulTxData[ulLoop] = ulTxData[ulLoop] + ulLoop;
	}

	// Tx Data
	for(ulLoop = 0; ulLoop < HALFCOMM_BUFFER_SIZE; ulLoop++)
	{
		WriteReg(ulTxRegAddr+ulLoop,ulTxData[ulLoop]);
	}

	*pulOkCnt = ulOkCnt;
	*pulErrCnt = ulErrCnt;

	return HAL_NOERR;

}

#endif


