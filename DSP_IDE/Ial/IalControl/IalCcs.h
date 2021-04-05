
/**
 * IalCcs.h
 * IalCcs Interface Module Definition
 */

#ifndef _IALCCS_H_
#define _IALCCS_H_

#include "Hal/HalAi.h"
#include "Hal/HalFComm.h"

#include "IalMes.h"
#include "IalVbc.h"

/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{

// Control Input, Output 은 채널별로 인덱스가 정의되어 있다.
// 기본 정의에 맞게 Idx를 정의하여야 한다.
// 0          ~ 15         : Analog Input
// 16         ~ 15 + 200*1 : FComm #1
// 16 + 200*1 ~ 15 + 200*2 : FComm #1
// 16 + 200*2 ~ 15 + 200*3 : FComm #1
// 16 + 200*3 ~ 15 + 200*4 : FComm #1

#define IALCCS_FCOMM_CH1_START		HALAI_CH_NUM										// to MES with only HDR
#define IALCCS_FCOMM_CH2_START		(IALCCS_FCOMM_CH1_START + IALRTDS_FCOMM_SIZE)		// to VBCU A with small Data
#define IALCCS_FCOMM_CH3_START		(IALCCS_FCOMM_CH2_START + IALRTDS_FCOMM_SIZE)		// to VBCU B with small Data
#define IALCCS_FCOMM_CH4_START		(IALCCS_FCOMM_CH3_START + IALRTDS_FCOMM_SIZE)		// to VBCU C with small Data

#define IALCCS_HMI_START			(IALCCS_FCOMM_CH4_START + IALRTDS_FCOMM_SIZE)		// to VBCU C with small Data

#define IALCCS_FCOMM_HDR_OFFSET		PROTOCOL_STARTCODE
#define IALCCS_FCOMM_CRC_OFFSET		PROTOCOL_CRC_IDX
#define IALCCS_FCOMM_SEQ_OFFSET		PROTOCOL_SEQ_IDX
#define IALCCS_FCOMM_LEN_OFFSET		PROTOCOL_LEN_IDX

// Data Body
#define IALCCS_FCOMM_VBCUREF_OFFSET		PROTOCOL_BODY_IDX
#define IALCCS_FCOMM_VBCLREF_OFFSET		(PROTOCOL_BODY_IDX + 1)

#define IALCCS_FCOMM_LAST_IDX		IALCCS_FCOMM_VBCLREF_OFFSET
#define IALCCS_FCOMM_SIZE			(IALCCS_FCOMM_LAST_IDX + 1)
//	#define IALCCS_FCOMM_DUMMY_SIZE		5



/****************************************************************************/
/* CCS 와 VBC 간 채널 연결이 변경되면 아래의 정의를 변경해야함.             */
/****************************************************************************/
#define IALCCS_MES_START			IALCCS_FCOMM_CH1_START

#define IALCCS_VBC_PHA_START		IALCCS_FCOMM_CH2_START
#define IALCCS_VBC_PHB_START		IALCCS_FCOMM_CH3_START
#define IALCCS_VBC_PHC_START		IALCCS_FCOMM_CH4_START

// Hmi In/Out Data : HmiApp 과 연결해야 하며, 아래의 정의는 사용되지 않음.
#define IALCCS_HMI_START			(IALCCS_FCOMM_CH4_START + IALRTDS_FCOMM_SIZE)		// to VBCU C with small Data



/****************************************************************************/
/* CCS 의 송신 데이터로 수신부에서는 아래의 Idx를 사용해야 한다.(IalVbc.h 의 정의를 사용해야함.            */
/****************************************************************************/
// FComm Ch1 - MES 연결 채널로 송신 데이타 없음.
// 				수신 데이터는 IalMes.h 의 내용 참조.


// FComm Ch2 - VBC Phase A
#define IALCCS_VBCREF_PHA_UPPER_IDX (IALCCS_VBC_PHA_START + IALVBC_FCOMM_VBCUREF_OFFSET)
#define IALCCS_VBCREF_PHA_LoWER_IDX (IALCCS_VBC_PHA_START + IALVBC_FCOMM_VBCLREF_OFFSET)
#define IALCCS_VBCMEAS1_PHA_IDX 	(IALCCS_VBC_PHA_START + IALVBC_FCOMM_MEAS1_OFFSET)

// FComm Ch3 - VBC Phase B 
#define IALCCS_VBCREF_PHB_UPPER_IDX (IALCCS_VBC_PHB_START + IALVBC_FCOMM_VBCUREF_OFFSET)
#define IALCCS_VBCREF_PHB_LOWER_IDX (IALCCS_VBC_PHB_START + IALVBC_FCOMM_VBCLREF_OFFSET)
#define IALCCS_VBCMEAS1_PHB_IDX 	(IALCCS_VBC_PHB_START + IALVBC_FCOMM_MEAS1_OFFSET)

// FComm Ch4 - VBC Phase C
#define IALCCS_VBCREF_PHC_UPPER_IDX (IALCCS_VBC_PHC_START + IALVBC_FCOMM_VBCUREF_OFFSET)
#define IALCCS_VBCREF_PHC_LOWER_IDX (IALCCS_VBC_PHC_START + IALVBC_FCOMM_VBCLREF_OFFSET)
#define IALCCS_VBCMEAS1_PHC_IDX 	(IALCCS_VBC_PHC_START + IALVBC_FCOMM_MEAS1_OFFSET)

/****************************************************************************/
/* CCS 의 수신 데이터 정의(IalVbc.h 의 정의를 사용해야 함.          */
/****************************************************************************/

// FComm Ch2 - VBC Phase A
#define IALCCS_VBCMEAS_PHA_UPPER_IDX (IALCCS_VBC_PHA_START + IALVBC_FCOMM_VBCUMEAS_OFFSET)
#define IALCCS_VBCMEAS_PHA_LoWER_IDX (IALCCS_VBC_PHA_START + IALVBC_FCOMM_VBCLMEAS_OFFSET)

// FComm Ch3 - VBC Phase B 
#define IALCCS_VBCMEAS_PHB_UPPER_IDX (IALCCS_VBC_PHB_START + IALVBC_FCOMM_VBCUMEAS_OFFSET)
#define IALCCS_VBCMEAS_PHB_LOWER_IDX (IALCCS_VBC_PHB_START + IALVBC_FCOMM_VBCLMEAS_OFFSET)

// FComm Ch4 - VBC Phase C
#define IALCCS_VBCMEAS_PHC_UPPER_IDX (IALCCS_VBC_PHC_START + IALVBC_FCOMM_VBCUMEAS_OFFSET)
#define IALCCS_VBCMEAS_PHC_LOWER_IDX (IALCCS_VBC_PHC_START + IALVBC_FCOMM_VBCLMEAS_OFFSET)


#define IALCCS_TX_DEFAULT_SIZE IALVBC_RX_DEFAULT_SIZE 
#define IALCCS_RX_DEFAULT_SIZE IALVBC_TX_DEFAULT_SIZE 




typedef ST_VBCRX_TYPE ST_CCSTX_TYPE;
typedef ST_VBCTX_TYPE ST_CCSRX_TYPE;

typedef ST_MESTX_HS_TYPE ST_CCSRX_MES_TYPE;

typedef struct stCcsCpuTxtype {
	U_IALDATA_TYPE	uCurr_Meas;
	U_IALDATA_TYPE	uDigitalSts;
	// 모니터링 데이터 추가

 	U_IALDATA_TYPE	uReserved[IALHMI_INTERFACE_LEN - 2];
} ST_CCSCPUTX_TYPE;

typedef struct stCcsCpuRxtype {
	U_IALDATA_TYPE	uCurr_Ref;
	U_IALDATA_TYPE	uDigitalCmd;
	// 추가 지령 데이터 추가

	U_IALDATA_TYPE	uReserved[IALHMI_INTERFACE_LEN - 2];
} ST_CCSCPURX_TYPE;



#endif


