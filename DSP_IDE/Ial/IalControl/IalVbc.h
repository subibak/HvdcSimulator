
/**
 * IalVbc.h
 * IalVbc Interface Module Definition
 */

#ifndef _IALVBC_H_
#define _IALVBC_H_
#include "Hal/HalAi.h"
#include "Hal/HalFComm.h"

#include "IalRtds.h"
#include "IalMes.h"

/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{

// Control Input, Output 은 채널별로 인덱스가 정의되어 있다.
// 기본 정의에 맞게 Idx를 정의하여야 한다.
// 0          ~ 15         : Analog Input
// 16         ~ 15 + 200*1 : FComm #1
// 16 + 200*1 ~ 15 + 200*2 : FComm #2
// 16 + 200*2 ~ 15 + 200*3 : FComm #3
// 16 + 200*3 ~ 15 + 200*4 : FComm #4


#define IALVBC_FCOMM_CH1_START		HALAI_CH_NUM										// to MES with only HDR
#define IALVBC_FCOMM_CH2_START		(IALVBC_FCOMM_CH1_START + IALRTDS_FCOMM_SIZE)		// to VBCU A with small Data
#define IALVBC_FCOMM_CH3_START		(IALVBC_FCOMM_CH2_START + IALRTDS_FCOMM_SIZE)		// to VBCU B with small Data
#define IALVBC_FCOMM_CH4_START		(IALVBC_FCOMM_CH3_START + IALRTDS_FCOMM_SIZE)		// to VBCU C with small Data

// Hmi In/Out Data : HmiApp 과 연결해야 함.
#define IALVBC_HMI_START			(IALVBC_FCOMM_CH4_START + IALRTDS_FCOMM_SIZE)		// to VBCU C with small Data

/****************************************************************************/
/* VBC 의 송신 데이터로 수신부에서는 아래의 Idx를 사용해야 한다.            */
/****************************************************************************/

// 시스템 간의 채널 연결 정의
#define IALVBC_CCS_COMM_START			IALVBC_FCOMM_CH1_START
#define IALVBC_RTDSU_COMM_START			IALVBC_FCOMM_CH3_START
#define IALVBC_RTDSL_COMM_START			IALVBC_FCOMM_CH4_START

///////////////////////////////////////// FComm Ch1 - CCS
#define IALVBC_FCOMM_HDR_OFFSET		(IALVBC_CCS_COMM_START+PROTOCOL_STARTCODE_IDX)
#define IALVBC_FCOMM_CRC_OFFSET		(IALVBC_CCS_COMM_START+PROTOCOL_CRC_IDX)
#define IALVBC_FCOMM_SEQ_OFFSET		(IALVBC_CCS_COMM_START+PROTOCOL_SEQ_IDX)
#define IALVBC_FCOMM_LEN_OFFSET		(IALVBC_CCS_COMM_START+PROTOCOL_LEN_IDX)

// Data Body : Tx
#define IALVBC_FCOMM_VBCUMEAS_OFFSET		(IALVBC_CCS_COMM_START+PROTOCOL_BODY_IDX)
#define IALVBC_FCOMM_VBCLMEAS_OFFSET		(IALVBC_FCOMM_VBCUMEAS_OFFSET + 1)

#define IALVBC_FCOMM_TX_LAST_IDX		IALVBC_FCOMM_VBCLMEAS_OFFSET
#define IALVBC_FCOMM_TX_SIZE			(IALVBC_FCOMM_LAST_IDX + 1)

// Data Body : Rx
#define IALVBC_FCOMM_VBCUREF_OFFSET		(IALVBC_CCS_COMM_START+PROTOCOL_BODY_IDX)
#define IALVBC_FCOMM_VBCLREF_OFFSET		(IALVBC_FCOMM_VBCUREF_OFFSET + 1)
#define IALVBC_FCOMM_MEAS1_OFFSET		(IALVBC_FCOMM_VBCUREF_OFFSET + 2)

#define IALVBC_FCOMM_RX_LAST_IDX		IALVBC_FCOMM_MEAS1_OFFSET
#define IALVBC_FCOMM_RX_SIZE			(IALVBC_FCOMM_LAST_IDX + 1)



///////////////////////////////////////// FComm Ch3, Ch4 - RTDS
// Tx, Rx, 상세 Frame의 Idx는 IalRtds.h 의 내용을 참조한다.
//	// Real Data Idx
//	#define IALVBC_VBCMEAS_PHA_UPPER_IDX (IALVBC_FCOMM_CH2_START + IALVBC_FCOMM_VBCUMEAS_OFFSET)
//	#define IALVBC_VBCMEAS_PHA_LoWER_IDX (IALVBC_FCOMM_CH2_START + IALVBC_FCOMM_VBCLMEAS_OFFSET)
//	
//	///////////////////////////////////////// FComm Ch3 - RTDS
//	#define IALVBC_VBCMEAS_PHB_UPPER_IDX (IALVBC_FCOMM_CH3_START + IALVBC_FCOMM_VBCUMEAS_OFFSET)
//	#define IALVBC_VBCMEAS_PHB_LOWER_IDX (IALVBC_FCOMM_CH3_START + IALVBC_FCOMM_VBCLMEAS_OFFSET)
//	
//	///////////////////////////////////////// FComm Ch4 - RTDS
//	#define IALVBC_VBCMEAS_PHC_UPPER_IDX (IALVBC_FCOMM_CH4_START + IALVBC_FCOMM_VBCUMEAS_OFFSET)
//	#define IALVBC_VBCMEAS_PHC_LOWER_IDX (IALVBC_FCOMM_CH4_START + IALVBC_FCOMM_VBCLMEAS_OFFSET)
//	
//	#define IALVBC_MONITORING_DATA_01_IDX		IALVBC_HMI_START

/****************************************************************************/
/* VBC 의 수신신 데이터는 연결된 시스템의 송신 Idx를 사용해야 한다.         */
/****************************************************************************/

#define IALVBC_TX_DEFAULT_SIZE	16
#define IALVBC_RX_DEFAULT_SIZE	16

#define IALVBC_RTDS_TX_SIZE		IALRTDS_RX_SIZE
#define IALVBC_RTDS_RX_SIZE		IALRTDS_TX_SIZE

typedef struct stVbcRxtype {
	U_IALDATA_TYPE	uHeader[PROTOCOL_HEADER_LEN];
	U_IALDATA_TYPE	uUpperRef; // Each Fp have Fp datas of 4 SM.
	U_IALDATA_TYPE	uLowerRef; // Each Fp have Fp datas of 4 SM.
	U_IALDATA_TYPE	uMeas1; // Each Fp have Fp datas of 4 SM.
	U_IALDATA_TYPE	uMeas2; // Each Fp have Fp datas of 4 SM.
} ST_VBCRX_TYPE;

typedef struct stVbcTxtype {
	U_IALDATA_TYPE	uHeader[PROTOCOL_HEADER_LEN];
	U_IALDATA_TYPE	uUpperMeas; // Each Fp have Fp datas of 4 SM.
	U_IALDATA_TYPE	uLowerMeas; // Each Fp have Fp datas of 4 SM.
} ST_VBCTX_TYPE;

typedef struct stVbcCpuTxtype {
	U_IALDATA_TYPE	uSubModuleSts[SM_LEVEL/4];
	// 모니터링 데이터 추가

	U_IALDATA_TYPE	uReserved[IALHMI_INTERFACE_LEN - SM_LEVEL/4];
} ST_VBCCPUTX_TYPE;

typedef struct stVbcCpuRxtype {
	// 기타 CPU 보드 입력 데이터 추가
	U_IALDATA_TYPE	uReserved[IALHMI_INTERFACE_LEN];
} ST_VBCCPURX_TYPE;

typedef ST_MESTX_HS_TYPE ST_VBCRX_MES_TYPE;


#endif


