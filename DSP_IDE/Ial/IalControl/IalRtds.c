
/**
 * IalRtds.c
 * IalRtds Implementation
 */


#include "IalRtds.h"
#include "Hal/HalFcomm.h"

/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{

#define MAX_LEG_NUM	6

// FComm 3,4 에 RTDS 연결. 1,2 는 Reserved 처리.
//uint32_t	t_ulLagNum[HALFCOMM_CH_NUM];
uint32_t	t_ulSMNum[MAX_LEG_NUM] = {SM_LEVEL,SM_LEVEL,SM_LEVEL,SM_LEVEL,SM_LEVEL,SM_LEVEL};
uint32_t	t_ulRxLegNum[MAX_LEG_NUM];


// Control Input, Output 은 채널별로 인덱스가 정의되어 있다.
// 기본 정의에 맞게 Idx를 정의하여야 한다.
// 0          ~ 15         : Analog Input
// 16         ~ 15 + 200*1 : FComm #1
// 16 + 200*1 ~ 15 + 200*2 : FComm #1
// 16 + 200*2 ~ 15 + 200*3 : FComm #1
// 16 + 200*3 ~ 15 + 200*4 : FComm #1
//	#define IALRTDS_FCOMM_DUMMY_SIZE		5


//  31  28                                 0
//	LagNo/XXXX/XXXX/XXXX/XXXX/XXXX/XXXX/XXXX
//  Firing-04 /Firing-03/Firing-02/Firing-01
//  Firing-08 /Firing-07/Firing-06/Firing-05
//  ......
//  Firing-100/Firing-99/Firing-98/Firing-97

// Compact Mode
IAL_ERROR IalRtdsEncode(uint32_t	*pulSrc, uint32_t	*pulFpType, uint32_t	ulLegNum)
{
	IAL_ERROR eResult = IAL_NOERR;

	if(ulLegNum != t_ulRxLegNum[ulLegNum])
	{
		// Leg Num 불일치 에러. 
		eResult = IAL_ERR_LEGNUM_MISMATCH;
	}
	else
	{
		pulFpType[0] = (t_ulRxLegNum[ulLegNum] & IALRTDS_TX_HEADER_LEGNUM_MASK) << IALRTDS_TX_HEADER_LEGNUM_SHIFT;
		memcpy(&(pulFpType[IALRTDS_HEADER_SIZE]), pulSrc, ulNumOfSm/4);
	}

	return IAL_NOERR;
}

// Type A
//  31  28                                 0
//	xxxx/xxxx/xxxx/Type/XXXX/XXXX/LegN/SeqNo
//  Firing-04 /Firing-03/Firing-02/Firing-01
//  Firing-08 /Firing-07/Firing-06/Firing-05
//  ......
//  Firing-100/Firing-99/Firing-98/Firing-97

// Type B
//	Sub /Modu/ Num/Type/XXXX/XXXX/LegN/SeqNo


// Type C
//	Sub /Modu/ Num/Type/XXXX/XXXX/LegN/SeqNo
// Compact Mode

// Type A Offset : 0	(Size 13)
// Type B Offset : 13	(Size 1 + SubModuleLevel)
// Type C Offset : (13+1+SubModuleLevel/2)	(Size 1 + SubModuleLevel/2)


IAL_ERROR IalRtdsDecode(uint32_t	*pulSrc, uint32_t	*pulTypeA, uint32_t *pulTypeB, uint32_t *pulTypeC, uint32_t	ulLegNum)
{

	IAL_ERROR eResult = IAL_NOERR;

	t_ulRxLegNum[ulLegNum]	= (pulSrc[IALRTDS_FCOMM_TYPEA_OFFSET] & IALRTDS_RX_HEADER_LEGNUM_MASK) >> IALRTDS_RX_HEADER_LEGNUM_SHIFT;
	t_ulSMNum[ulLegNum]		= (pulSrc[IALRTDS_FCOMM_TYPEA_OFFSET] & IALRTDS_RX_HEADER_SMNUM_MASK) >> IALRTDS_RX_HEADER_SMNUM_SHIFT;

	if(ulLegNum != t_ulRxLegNum[ulLegNum])
	{
		// Leg Num 불일치 에러. 
		eResult = IAL_ERR_LEGNUM_MISMATCH;
	}
	else
	{
		memcpy(pulTypeA, &(pulSrc[IALRTDS_FCOMM_TYPEA_BODY_OFFSET]), (TYPEA_SIZE-IALRTDS_HEADER_SIZE)); // Header 제외.
		memcpy(pulTypeB, &(pulSrc[IALRTDS_FCOMM_TYPEB_BODY_OFFSET]), t_ulRxLegNum[ulLegNum]); // Header 제외.
		memcpy(pulTypeC, &(pulSrc[IALRTDS_FCOMM_TYPEC_BODY_OFFSET]), (t_ulRxLegNum[ulLegNum] + IALRTDS_HEADER_SIZE)/2); // Header 제외.
	}

	return eResult;
}







