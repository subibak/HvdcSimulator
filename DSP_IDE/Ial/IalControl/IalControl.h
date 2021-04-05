

/**
 * IalControl.h
 * IalControl Interface Module Definition
 */

#ifndef _IALCONTROL_H_
#define _IALCONTROL_H_

#include "../IalIfDef.h"
#include "Hal/HalAi.h"
#include "Hal/HalFComm.h"
#include "Hal/HalVme.h"
#include "IalMes.h"
#include "IalVbc.h"
#include "IalCcs.h"



/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{


#define IALCONTROL_INPUT_SIZE	(HALAI_CH_NUM + (HALFCOMM_CH_NUM*IALRTDS_FCOMM_SIZE) + HWREG_VMES_RUSER_SIZE)
#define IALCONTROL_OUTPUT_SIZE	(HALAI_CH_NUM + (HALFCOMM_CH_NUM*IALRTDS_FCOMM_SIZE) + HWREG_VMES_RUSER_SIZE)

#define IALCONTROL_AI_OFFSET	0x0

#define IALCONTROL_FCOMM1_OFFSET	(HALAI_CH_NUM)
#define IALCONTROL_FCOMM2_OFFSET	(IALCONTROL_FCOMM1_OFFSET + IALRTDS_FCOMM_SIZE)
#define IALCONTROL_FCOMM3_OFFSET	(IALCONTROL_FCOMM2_OFFSET + IALRTDS_FCOMM_SIZE)
#define IALCONTROL_FCOMM4_OFFSET	(IALCONTROL_FCOMM3_OFFSET + IALRTDS_FCOMM_SIZE)

#define IALCONTROL_VME_OFFSET		(IALCONTROL_FCOMM4_OFFSET + IALRTDS_FCOMM_SIZE)
#define IALCONTROL_VME_SIZE_MAX		(128)

#if 0
#define	IALCONTROL_INPUT_SIZE		64			///< Real + 광통신 패킷 최대 개수를정하기어렵다.
#define	IALCONTROL_OUTPUT_SIZE		64			///< Real + 광통신 패킷 최대 개수를정하기어렵다.


#define IALCONTROL_CH_NUM			4


/****************************************************************************/
/* MES, CCS, VBC Analog I/O Idx 정의                                        */
/****************************************************************************/
///> MES, CCS, VBC Analog Input IDX
#define IALCONTROL_FRXAI01_IDX		0
#define IALCONTROL_FRXAI02_IDX		1
#define IALCONTROL_FRXAI03_IDX		2
#define IALCONTROL_FRXAI04_IDX		3
#define IALCONTROL_FRXAI05_IDX		4
#define IALCONTROL_FRXAI06_IDX		5
#define IALCONTROL_FRXAI07_IDX		6
#define IALCONTROL_FRXAI08_IDX		7
#define IALCONTROL_FRXAI09_IDX		8
#define IALCONTROL_FRXAI10_IDX		9
#define IALCONTROL_FRXAI11_IDX		10
#define IALCONTROL_FRXAI12_IDX		11
#define IALCONTROL_FRXAI13_IDX		12
#define IALCONTROL_FRXAI14_IDX		13
#define IALCONTROL_FRXAI15_IDX		14
#define IALCONTROL_FRXAI16_IDX		15



#define FRXFRX1_IDX		16

///> MES FComm Tx 1은 전처리 데이터공유용으로 MES 1, 2 의데이터가 차이 존재.
#define MES_FRX1_HEAD		16
#define MES_FRX1_CRC		17
#define MES_FRX1_VOLT_A		18
#define MES_FRX1_VOLT_B		19
#define MES_FRX1_VOLT_C		20
#define MES_FRX1_CURR_A		21
#define MES_FRX1_CURR_B		22
#define MES_FRX1_CURR_C		23


///> MES, CCS, VBC Analog Output IDX
#define FRXAO01_IDX		0
#define FRXAO02_IDX		1
#define FRXAO03_IDX		2
#define FRXAO04_IDX		3
#define FRXAO05_IDX		4
#define FRXAO06_IDX		5
#define FRXAO07_IDX		6
#define FRXAO08_IDX		7



/****************************************************************************/
/* MES System Input Variable Idx 정의                                       */
/****************************************************************************/

///> MES FComm Tx 1은 전처리 데이터공유용으로 MES 1, 2 의데이터가 차이 존재.
#define MES_FTX1_HEAD		16
#define MES_FTX1_CRC		17
#define MES_FTX1_VOLT_A		18
#define MES_FTX1_VOLT_B		19
#define MES_FTX1_VOLT_C		20
#define MES_FTX1_CURR_A		21
#define MES_FTX1_CURR_B		22
#define MES_FTX1_CURR_C		23
// 추가 항목을 정의할 것. Tx 1은 mis 1,2 보드 같 데이터 교환 용도로 향후 정리가 필요함.

/****************************************************************************/
/* MES System Output Variable Idx 정의                                       */
/****************************************************************************/
///> MES FComm Tx 2,3,4 는 내용이 동일하다.
#define MES_FTX2_HEAD		(16+2*HALFCOMM_BUFFER_SIZE)
#define MES_FTX2_CRC		(17+2*HALFCOMM_BUFFER_SIZE)
#define MES_FTX2_VOLT_A		(18+2*HALFCOMM_BUFFER_SIZE)
#define MES_FTX2_VOLT_B		(19+2*HALFCOMM_BUFFER_SIZE)
#define MES_FTX2_VOLT_C		(20+2*HALFCOMM_BUFFER_SIZE)
#define MES_FTX2_CURR_A		(21+2*HALFCOMM_BUFFER_SIZE)
#define MES_FTX2_CURR_B		(22+2*HALFCOMM_BUFFER_SIZE)
#define MES_FTX2_CURR_C		(23+2*HALFCOMM_BUFFER_SIZE)
// Tx Channel 2 의 데이터 인덱스 정의

#define MES_FTX3_HEAD		(16+3*HALFCOMM_BUFFER_SIZE)
#define MES_FTX3_CRC		(17+3*HALFCOMM_BUFFER_SIZE)
#define MES_FTX3_VOLT_A		(18+3*HALFCOMM_BUFFER_SIZE)
#define MES_FTX3_VOLT_B		(19+3*HALFCOMM_BUFFER_SIZE)
#define MES_FTX3_VOLT_C		(20+3*HALFCOMM_BUFFER_SIZE)
#define MES_FTX3_CURR_A		(21+3*HALFCOMM_BUFFER_SIZE)
#define MES_FTX3_CURR_B		(22+3*HALFCOMM_BUFFER_SIZE)
#define MES_FTX3_CURR_C		(23+3*HALFCOMM_BUFFER_SIZE)
// Tx Channel 3 의 데이터 인덱스 정의

#define MES_FTX4_HEAD		(16+4*HALFCOMM_BUFFER_SIZE)
#define MES_FTX4_CRC		(17+4*HALFCOMM_BUFFER_SIZE)
#define MES_FTX4_VOLT_A		(18+4*HALFCOMM_BUFFER_SIZE)
#define MES_FTX4_VOLT_B		(19+4*HALFCOMM_BUFFER_SIZE)
#define MES_FTX4_VOLT_C		(20+4*HALFCOMM_BUFFER_SIZE)
#define MES_FTX4_CURR_A		(21+4*HALFCOMM_BUFFER_SIZE)
#define MES_FTX4_CURR_B		(22+4*HALFCOMM_BUFFER_SIZE)
#define MES_FTX4_CURR_C		(23+4*HALFCOMM_BUFFER_SIZE)
// Tx Channel 4 의 데이터 인덱스 정의













/****************************************************************************/
/* CCS System Input Variable Idx 정의                                       */
/****************************************************************************/
///> CCS FComm Tx 1은 Mes 의 데이터를 처리하므로 Mes Tx Idx 와 동일하다.
#define CCS_FRX1_HEAD		(16+HALFCOMM_BUFFER_SIZE)
#define CCS_FRX1_CRC		(17+HALFCOMM_BUFFER_SIZE)
#define CCS_FRX1_VOLT_A		(18+HALFCOMM_BUFFER_SIZE)
#define CCS_FRX1_VOLT_B		(19+HALFCOMM_BUFFER_SIZE)
#define CCS_FRX1_VOLT_C		(20+HALFCOMM_BUFFER_SIZE)
#define CCS_FRX1_CURR_A		(21+HALFCOMM_BUFFER_SIZE)
#define CCS_FRX1_CURR_B		(22+HALFCOMM_BUFFER_SIZE)
#define CCS_FRX1_CURR_C		(23+HALFCOMM_BUFFER_SIZE)
// 추가 항목을 정의할 것. Tx 1은 mis 1,2 보드 같 데이터 교환 용도로 향후 정리가 필요함.

///> CCS FComm Tx 2,3,4 는 내용이 동일하다. 각 Arm 의 VBC 용임.
#define CCS_FRX2_HEAD		(16+2*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX2_CRC		(17+2*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX2_ARMVOLTU	(18+2*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX2_ARMCURRU	(19+2*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX2_ARMVOLTL	(20+2*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX2_ARMCURRL	(21+2*HALFCOMM_BUFFER_SIZE)
// Tx Channel 2 의 데이터 인덱스 정의

#define CCS_FRX3_HEAD		(16+3*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX3_CRC		(17+3*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX3_ARMVOLT	(18+3*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX3_ARMCURR	(19+3*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX3_ARMVOLTL	(20+3*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX3_ARMCURRL	(21+3*HALFCOMM_BUFFER_SIZE)
// Tx Channel 3 의 데이터 인덱스 정의

#define CCS_FRX4_HEAD		(16+4*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX4_CRC		(17+4*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX4_ARMVOLT	(18+4*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX4_ARMCURR	(19+4*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX4_ARMVOLTL	(20+4*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX4_ARMCURRL	(21+4*HALFCOMM_BUFFER_SIZE)
// Tx Channel 4 의 데이터 인덱스 정의



/****************************************************************************/
/* CCS System Output Variable Idx 정의                                       */
/****************************************************************************/
///> CCS FComm Tx 1은 Mes 대상이므로 No Data
#define CCS_FTX1_HEAD		(16+HALFCOMM_BUFFER_SIZE)
#define CCS_FTX1_CRC		(17+HALFCOMM_BUFFER_SIZE)

///> CCS FComm Tx 2,3,4 는 내용이 동일하다. 각 Arm 의 VBC 용임.
#define CCS_FTX2_HEAD			(16+2*HALFCOMM_BUFFER_SIZE)
#define CCS_FTX2_CRC			(17+2*HALFCOMM_BUFFER_SIZE)
#define CCS_FTX2_ARMVOLT_REFU	(18+2*HALFCOMM_BUFFER_SIZE)
#define CCS_FTX2_ARMVOLT_REFL	(19+2*HALFCOMM_BUFFER_SIZE)
// Tx Channel 2 의 데이터 인덱스 정의

#define CCS_FTX3_HEAD			(16+3*HALFCOMM_BUFFER_SIZE)
#define CCS_FTX3_CRC			(17+3*HALFCOMM_BUFFER_SIZE)
#define CCS_FTX3_ARMVOLT_REFU	(18+3*HALFCOMM_BUFFER_SIZE)
#define CCS_FTX3_ARMVOLT_REFL	(19+3*HALFCOMM_BUFFER_SIZE)
// Tx Channel 3 의 데이터 인덱스 정의

#define CCS_FTX4_HEAD			(16+4*HALFCOMM_BUFFER_SIZE)
#define CCS_FTX4_CRC			(17+4*HALFCOMM_BUFFER_SIZE)
#define CCS_FTX4_ARMVOLT_REFU	(18+4*HALFCOMM_BUFFER_SIZE)
#define CCS_FTX4_ARMVOLT_REFL	(19+4*HALFCOMM_BUFFER_SIZE)
// Tx Channel 4 의 데이터 인덱스 정의



/****************************************************************************/
/* VBC System Input Variable Idx 정의                                       */
/****************************************************************************/
///> VBC FComm Tx 1은 Mes 의 데이터를 처리하므로 Mes Tx Idx 와 동일하다.
#define VBC_FRX1_HEAD		(16+HALFCOMM_BUFFER_SIZE)
#define VBC_FRX1_CRC		(17+HALFCOMM_BUFFER_SIZE)
#define VBC_FRX1_VOLT_A		(18+HALFCOMM_BUFFER_SIZE)
#define VBC_FRX1_VOLT_B		(19+HALFCOMM_BUFFER_SIZE)
#define VBC_FRX1_VOLT_C		(20+HALFCOMM_BUFFER_SIZE)
#define VBC_FRX1_CURR_A		(21+HALFCOMM_BUFFER_SIZE)
#define VBC_FRX1_CURR_B		(22+HALFCOMM_BUFFER_SIZE)
#define VBC_FRX1_CURR_C		(23+HALFCOMM_BUFFER_SIZE)
// 추가 항목을 정의할 것. Tx 1은 mis 1,2 보드 같 데이터 교환 용도로 향후 정리가 필요함.
 
#define VBC_FRX2_HEAD			(16+2*HALFCOMM_BUFFER_SIZE)
#define VBC_FRX2_CRC			(17+2*HALFCOMM_BUFFER_SIZE)
#define VBC_FRX2_ARMVOLT_REFU	(18+2*HALFCOMM_BUFFER_SIZE)
#define VBC_FRX2_ARMVOLT_REFL	(19+2*HALFCOMM_BUFFER_SIZE)
// Tx Channel 2 의 데이터 인덱스 정의 (From CCS)

#define VBC_FRX3_HEAD		(16+3*HALFCOMM_BUFFER_SIZE)
#define VBC_FRX3_CRC		(17+3*HALFCOMM_BUFFER_SIZE)
#define VBC_FRX3_ARMVOLTL	(18+3*HALFCOMM_BUFFER_SIZE)
#define VBC_FRX3_ARMCURRL	(19+3*HALFCOMM_BUFFER_SIZE)
// Tx Channel 3 의 데이터 인덱스 정의 (From VBCL)

// Tx Channel 4 의 데이터 인덱스 정의



/****************************************************************************/
/* VBC System Output Variable Idx 정의                                       */
/****************************************************************************/
///> VBC FComm Tx 1은 Mes 대상이므로 No Data
#define VBC_FTX1_HEAD		(16+HALFCOMM_BUFFER_SIZE)
#define VBC_FTX1_CRC		(17+HALFCOMM_BUFFER_SIZE)

///> VBC FComm Tx 2,3,4 는 내용이 동일하다. 각 Arm 의 VBC 용임.
#define VBC_FTX2_HEAD		(16+2*HALFCOMM_BUFFER_SIZE)
#define VBC_FTX2_CRC		(17+2*HALFCOMM_BUFFER_SIZE)
#define VBC_FTX2_ARMVOLTU	(18+2*HALFCOMM_BUFFER_SIZE)
#define VBC_FTX2_ARMCURRU	(19+2*HALFCOMM_BUFFER_SIZE)
#define VBC_FTX2_ARMVOLTL	(20+2*HALFCOMM_BUFFER_SIZE)
#define VBC_FTX2_ARMCURRL	(21+2*HALFCOMM_BUFFER_SIZE)
// Tx Channel 2 의 데이터 인덱스 정의

#define VBC_FTX3_HEAD			(16+3*HALFCOMM_BUFFER_SIZE)
#define VBC_FTX3_CRC			(17+3*HALFCOMM_BUFFER_SIZE)
#define VBC_FTX3_ARMVOLT_REFU	(18+3*HALFCOMM_BUFFER_SIZE)
#define VBC_FTX3_ARMVOLT_REFL	(19+3*HALFCOMM_BUFFER_SIZE)
// Tx Channel 3 의 데이터 인덱스 정의

 // Tx Channel 4 의 데이터 인덱스 정의






/****************************************************************************/
/* VBCL System Input Variable Idx 정의                                       */
/****************************************************************************/
///> VBCL FComm Tx 1은 Mes 의 데이터를 처리하므로 Mes Tx Idx 와 동일하다.
#define VBCL_FRX1_HEAD		(16+HALFCOMM_BUFFER_SIZE)
#define VBCL_FRX1_CRC		(17+HALFCOMM_BUFFER_SIZE)
#define VBCL_FRX1_VOLT_A		(18+HALFCOMM_BUFFER_SIZE)
#define VBCL_FRX1_VOLT_B		(19+HALFCOMM_BUFFER_SIZE)
#define VBCL_FRX1_VOLT_C		(20+HALFCOMM_BUFFER_SIZE)
#define VBCL_FRX1_CURR_A		(21+HALFCOMM_BUFFER_SIZE)
#define VBCL_FRX1_CURR_B		(22+HALFCOMM_BUFFER_SIZE)
#define VBCL_FRX1_CURR_C		(23+HALFCOMM_BUFFER_SIZE)
// 추가 항목을 정의할 것. Tx 1은 mis 1,2 보드 같 데이터 교환 용도로 향후 정리가 필요함.
 
#define VBCL_FRX2_HEAD			(16+2*HALFCOMM_BUFFER_SIZE)
#define VBCL_FRX2_CRC			(17+2*HALFCOMM_BUFFER_SIZE)
#define VBCL_FTX2_ARMVOLT_REFU	(18+2*HALFCOMM_BUFFER_SIZE)
#define VBCL_FTX2_ARMVOLT_REFL	(19+2*HALFCOMM_BUFFER_SIZE)
// Tx Channel 2 의 데이터 인덱스 정의 (From CCS)

// Tx Channel 3 의 데이터 인덱스 정의 (From VBCL)

// Tx Channel 4 의 데이터 인덱스 정의



/****************************************************************************/
/* VBCL System Output Variable Idx 정의                                       */
/****************************************************************************/
///> VBCL FComm Tx 1은 Mes 대상이므로 No Data
#define VBCL_FTX1_HEAD		(16+HALFCOMM_BUFFER_SIZE)
#define VBCL_FTX1_CRC		(17+HALFCOMM_BUFFER_SIZE)

///> VBCL FComm Tx 2,3,4 는 내용이 동일하다. 각 Arm 의 VBC 용임.
#define VBCL_FTX2_HEAD		(16+2*HALFCOMM_BUFFER_SIZE)
#define VBCL_FTX2_CRC		(17+2*HALFCOMM_BUFFER_SIZE)
#define VBCL_FTX2_ARMVOLTL	(18+2*HALFCOMM_BUFFER_SIZE)
#define VBCL_FTX2_ARMCURRL	(19+2*HALFCOMM_BUFFER_SIZE)
// Tx Channel 2 의 데이터 인덱스 정의

// Tx Channel 3 의 데이터 인덱스 정의

// Tx Channel 4 의 데이터 인덱스 정의


#define IALCONTROL_SYNC_CH			0x1
#endif



///@}



/** @name Type Definition
 * Type Definition
 */
///@{


///@}

extern uint32_t	ulSystemCfg;

/**
 * @brief	Ial Moduel 을 초기화하는 함수 
 * @details	Ial 변수 및 관련 Hal의 초기화 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void IalControlInit(void);
void IalControlSend(void);
void IalControlReceive(void);

/**
 * @brief	Ial Control 모듈로서 Ai 값을 읽는 함수.
 * @details	Ai 채널 0~15번 입력으로 각 채널의 입력 전압값을 반환하는 함수.
 * @param	ulStartCh	: 값을 읽을 채널의 시작 (0~15)
 *			pfBuf		: 값을 저장한 float type Buffer 포인터
 *			ulLen		: ulStartCh 부터 읽을 채널 개수 (1~16)
 * @return	실행 결과
 * @date	2018-11-24
 * @version	0.0.1
 */
IAL_ERROR IalControlAiRead(uint32_t ulStartCh, float pfBuf[], uint32_t	ulLen);

/**
 * @brief	Ial Control 모듈로서 광채널 수신 데이터를 읽는 함수.
 * @details	수신할 데이터는 시스템에 의해 이미 수신 처리되되었으며, 데이터를 해당 Layer 에서 읽는 함수.
 * @param	ulCh		: 값을 읽을 채널 번호 (0~3)
 *			pulBuf		: 값을 저장한 uint32_t type Buffer 포인터
 *			ulLen		: ulStartCh 부터 읽을 채널 개수 (1~설정값) : 설정 최대값(124)
 * @return	실행 결과
 * @date	2018-11-24
 * @version	0.0.1
 */
IAL_ERROR IalControlFiberRead(uint32_t ulCh, uint32_t pulBuf[], uint32_t	ulLen);

/**
 * @brief	Ial Control 모듈로서 광채널 수신 데이터를 송신하는 함수.
 * @details	송신할 데이터를 해당 Layer 에 전달하며, 실제 송신의 시스템에 의해 송신됨.
 * @param	ulCh		: 값을 송신할 채널 번호 (0~3)
 *			pulBuf		: 송신할 uint32_t type Buffer 포인터
 *			ulLen		: 송신할 데이터의 개수 (1~설정값) : 설정 최대값(124)
 * @return	실행 결과
 * @date	2018-11-24
 * @version	0.0.1
 */
IAL_ERROR IalControlFiberWrite(uint32_t ulCh, uint32_t pulBuf[], uint32_t	ulLen);

/**
 * @brief	Ial Control 모듈로서 VME 수신 데이터를 읽는 함수.
 * @details	수신할 데이터는 시스템에 의해 주기적으로 수신 처리며, 데이터를 해당 Layer 에서 읽는 함수.
 * @param	ulStartIdx	: 값을 읽을 채널 Index 번호 (0~127)
 *			pulBuf		: 값을 저장한 uint32_t type Buffer 포인터
 *			ulLen		: ulStartIdx 부터 읽을 데이터 개수 최대값(128)
 * @return	실행 결과
 * @date	2018-11-24
 * @version	0.0.1
 */
IAL_ERROR IalControlVmeRead(uint32_t ulStartIdx, uint32_t pulBuf[], uint32_t	ulLen);


/**
 * @brief	Ial Control 모듈로서 VME 데이터를 송신하는 함수.
 * @details	송신할 데이터를 해당 Layer 에 전달하며, 실제 송신의 시스템에 의해 송신됨.
 * @param	ulStartIdx	: 값을 송신할 데이터 index 번호 (0~127)
 *			pulBuf		: 송신할 uint32_t type Buffer 포인터
 *			ulLen		: ulStartIdx 부터 송신할 데이터의 개수 최대값(128)
 * @return	실행 결과
 * @date	2018-11-24
 * @version	0.0.1
 */
IAL_ERROR IalControlVmeWrite(uint32_t ulStartIdx, uint32_t pulBuf[], uint32_t	ulLen);


extern U_IALBUFFER_TYPE	uInputBuffer;
extern U_IALBUFFER_TYPE	uOutputBuffer;

extern uint32_t	ulFCommRxLen[HALFCOMM_CH_NUM];


#endif


