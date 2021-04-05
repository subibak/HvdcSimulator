
#include "Protocol.h"
#include "Core/Crc.h"



uint32_t ulTxSeqNum[MAX_CHANNEL_NUM] = {PROTOCOL_SEQ_MIN,PROTOCOL_SEQ_MIN,PROTOCOL_SEQ_MIN,PROTOCOL_SEQ_MIN,};
uint32_t ulRxSeqNum[MAX_CHANNEL_NUM] = {PROTOCOL_SEQ_MAX,PROTOCOL_SEQ_MAX,PROTOCOL_SEQ_MAX,PROTOCOL_SEQ_MAX,};
//uint32_t ulSeqNum[FCOMM_CH_NUM];



void ProtocolInit(void)
{
	uint32_t	ulLoop;

	// Do Init Action.
	for(ulLoop = 0; ulLoop < MAX_CHANNEL_NUM; ulLoop++)
	{
		ulTxSeqNum[ulLoop] = PROTOCOL_SEQ_MIN;
		ulRxSeqNum[ulLoop] = PROTOCOL_SEQ_MAX;
	}

}

uint32_t	ulFCommTest = 0;

void ProtocolEncode(uint32_t ulChNum, uint32_t *pulData, uint32_t ulLen)
{

	/* Step 0: Calculate Sequence Num */
//	pulData[PROTOCOL_SEQ_IDX] = (ulTxSeqNum[ulChNum] | (ulLen << PROTOCOL_LEN_SHIFT));
	pulData[PROTOCOL_STARTCODE_IDX] = PROTOCOL_STARTCODE;
	pulData[PROTOCOL_SEQ_IDX] = (ulTxSeqNum[ulChNum]) & PROTOCOL_SEQ_MASK;
	pulData[PROTOCOL_LEN_IDX] = ulLen - PROTOCOL_HEADER_LEN;
	pulData[PROTOCOL_CRC_IDX] = MakeCrc32(&(pulData[PROTOCOL_BODY_IDX]), ulLen - PROTOCOL_HEADER_LEN);

	if(ulTxSeqNum[ulChNum] > PROTOCOL_SEQ_MAX) ulTxSeqNum[ulChNum] = PROTOCOL_SEQ_MIN;

	if(ulFCommTest == 2)
	{
		// Same Sequence Test
		// It will be detected in Rx Side
	}
	else
	{
		ulTxSeqNum[ulChNum]++;
	}

	if(ulTxSeqNum[ulChNum] > PROTOCOL_SEQ_MAX)
	{
		ulTxSeqNum[ulChNum] = PROTOCOL_SEQ_MIN;
	}


}

uint32_t ProtocolDecode(uint32_t ulChNum, uint32_t *pulData, uint32_t ulLen)
{

	uint32_t	ulResult = PROTOCOL_OK;
	uint32_t	ulCrc, ulSeqNum;

	ulSeqNum = pulData[PROTOCOL_SEQ_IDX];

	/* Step 1: Calculate Crc Number */
	ulCrc = MakeCrc32(&(pulData[PROTOCOL_BODY_IDX]), ulLen);
	ulCrc += ulFCommTest;

	if(ulCrc == pulData[PROTOCOL_CRC_IDX])
	{
		// No Error
		ulRxSeqNum[ulChNum] = ulSeqNum;
	}
	else
	{
		// Crc Error
		ulResult = PROTOCOL_CRC_ERROR;
	}
	
	/* Step 0: Init Ial Layer */
	// 상위에서 체크를 하는 데 한번 더 볼 필요 없을 듯.
	return ulResult;

}

uint32_t GetChSeqNum(uint32_t ulChNum)
{
	return ulRxSeqNum[ulChNum];
}





