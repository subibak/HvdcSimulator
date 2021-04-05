

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
#define	IALCONTROL_INPUT_SIZE		64			///< Real + ����� ��Ŷ �ִ� ���������ϱ��ƴ�.
#define	IALCONTROL_OUTPUT_SIZE		64			///< Real + ����� ��Ŷ �ִ� ���������ϱ��ƴ�.


#define IALCONTROL_CH_NUM			4


/****************************************************************************/
/* MES, CCS, VBC Analog I/O Idx ����                                        */
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

///> MES FComm Tx 1�� ��ó�� �����Ͱ��������� MES 1, 2 �ǵ����Ͱ� ���� ����.
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
/* MES System Input Variable Idx ����                                       */
/****************************************************************************/

///> MES FComm Tx 1�� ��ó�� �����Ͱ��������� MES 1, 2 �ǵ����Ͱ� ���� ����.
#define MES_FTX1_HEAD		16
#define MES_FTX1_CRC		17
#define MES_FTX1_VOLT_A		18
#define MES_FTX1_VOLT_B		19
#define MES_FTX1_VOLT_C		20
#define MES_FTX1_CURR_A		21
#define MES_FTX1_CURR_B		22
#define MES_FTX1_CURR_C		23
// �߰� �׸��� ������ ��. Tx 1�� mis 1,2 ���� �� ������ ��ȯ �뵵�� ���� ������ �ʿ���.

/****************************************************************************/
/* MES System Output Variable Idx ����                                       */
/****************************************************************************/
///> MES FComm Tx 2,3,4 �� ������ �����ϴ�.
#define MES_FTX2_HEAD		(16+2*HALFCOMM_BUFFER_SIZE)
#define MES_FTX2_CRC		(17+2*HALFCOMM_BUFFER_SIZE)
#define MES_FTX2_VOLT_A		(18+2*HALFCOMM_BUFFER_SIZE)
#define MES_FTX2_VOLT_B		(19+2*HALFCOMM_BUFFER_SIZE)
#define MES_FTX2_VOLT_C		(20+2*HALFCOMM_BUFFER_SIZE)
#define MES_FTX2_CURR_A		(21+2*HALFCOMM_BUFFER_SIZE)
#define MES_FTX2_CURR_B		(22+2*HALFCOMM_BUFFER_SIZE)
#define MES_FTX2_CURR_C		(23+2*HALFCOMM_BUFFER_SIZE)
// Tx Channel 2 �� ������ �ε��� ����

#define MES_FTX3_HEAD		(16+3*HALFCOMM_BUFFER_SIZE)
#define MES_FTX3_CRC		(17+3*HALFCOMM_BUFFER_SIZE)
#define MES_FTX3_VOLT_A		(18+3*HALFCOMM_BUFFER_SIZE)
#define MES_FTX3_VOLT_B		(19+3*HALFCOMM_BUFFER_SIZE)
#define MES_FTX3_VOLT_C		(20+3*HALFCOMM_BUFFER_SIZE)
#define MES_FTX3_CURR_A		(21+3*HALFCOMM_BUFFER_SIZE)
#define MES_FTX3_CURR_B		(22+3*HALFCOMM_BUFFER_SIZE)
#define MES_FTX3_CURR_C		(23+3*HALFCOMM_BUFFER_SIZE)
// Tx Channel 3 �� ������ �ε��� ����

#define MES_FTX4_HEAD		(16+4*HALFCOMM_BUFFER_SIZE)
#define MES_FTX4_CRC		(17+4*HALFCOMM_BUFFER_SIZE)
#define MES_FTX4_VOLT_A		(18+4*HALFCOMM_BUFFER_SIZE)
#define MES_FTX4_VOLT_B		(19+4*HALFCOMM_BUFFER_SIZE)
#define MES_FTX4_VOLT_C		(20+4*HALFCOMM_BUFFER_SIZE)
#define MES_FTX4_CURR_A		(21+4*HALFCOMM_BUFFER_SIZE)
#define MES_FTX4_CURR_B		(22+4*HALFCOMM_BUFFER_SIZE)
#define MES_FTX4_CURR_C		(23+4*HALFCOMM_BUFFER_SIZE)
// Tx Channel 4 �� ������ �ε��� ����













/****************************************************************************/
/* CCS System Input Variable Idx ����                                       */
/****************************************************************************/
///> CCS FComm Tx 1�� Mes �� �����͸� ó���ϹǷ� Mes Tx Idx �� �����ϴ�.
#define CCS_FRX1_HEAD		(16+HALFCOMM_BUFFER_SIZE)
#define CCS_FRX1_CRC		(17+HALFCOMM_BUFFER_SIZE)
#define CCS_FRX1_VOLT_A		(18+HALFCOMM_BUFFER_SIZE)
#define CCS_FRX1_VOLT_B		(19+HALFCOMM_BUFFER_SIZE)
#define CCS_FRX1_VOLT_C		(20+HALFCOMM_BUFFER_SIZE)
#define CCS_FRX1_CURR_A		(21+HALFCOMM_BUFFER_SIZE)
#define CCS_FRX1_CURR_B		(22+HALFCOMM_BUFFER_SIZE)
#define CCS_FRX1_CURR_C		(23+HALFCOMM_BUFFER_SIZE)
// �߰� �׸��� ������ ��. Tx 1�� mis 1,2 ���� �� ������ ��ȯ �뵵�� ���� ������ �ʿ���.

///> CCS FComm Tx 2,3,4 �� ������ �����ϴ�. �� Arm �� VBC ����.
#define CCS_FRX2_HEAD		(16+2*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX2_CRC		(17+2*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX2_ARMVOLTU	(18+2*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX2_ARMCURRU	(19+2*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX2_ARMVOLTL	(20+2*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX2_ARMCURRL	(21+2*HALFCOMM_BUFFER_SIZE)
// Tx Channel 2 �� ������ �ε��� ����

#define CCS_FRX3_HEAD		(16+3*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX3_CRC		(17+3*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX3_ARMVOLT	(18+3*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX3_ARMCURR	(19+3*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX3_ARMVOLTL	(20+3*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX3_ARMCURRL	(21+3*HALFCOMM_BUFFER_SIZE)
// Tx Channel 3 �� ������ �ε��� ����

#define CCS_FRX4_HEAD		(16+4*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX4_CRC		(17+4*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX4_ARMVOLT	(18+4*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX4_ARMCURR	(19+4*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX4_ARMVOLTL	(20+4*HALFCOMM_BUFFER_SIZE)
#define CCS_FRX4_ARMCURRL	(21+4*HALFCOMM_BUFFER_SIZE)
// Tx Channel 4 �� ������ �ε��� ����



/****************************************************************************/
/* CCS System Output Variable Idx ����                                       */
/****************************************************************************/
///> CCS FComm Tx 1�� Mes ����̹Ƿ� No Data
#define CCS_FTX1_HEAD		(16+HALFCOMM_BUFFER_SIZE)
#define CCS_FTX1_CRC		(17+HALFCOMM_BUFFER_SIZE)

///> CCS FComm Tx 2,3,4 �� ������ �����ϴ�. �� Arm �� VBC ����.
#define CCS_FTX2_HEAD			(16+2*HALFCOMM_BUFFER_SIZE)
#define CCS_FTX2_CRC			(17+2*HALFCOMM_BUFFER_SIZE)
#define CCS_FTX2_ARMVOLT_REFU	(18+2*HALFCOMM_BUFFER_SIZE)
#define CCS_FTX2_ARMVOLT_REFL	(19+2*HALFCOMM_BUFFER_SIZE)
// Tx Channel 2 �� ������ �ε��� ����

#define CCS_FTX3_HEAD			(16+3*HALFCOMM_BUFFER_SIZE)
#define CCS_FTX3_CRC			(17+3*HALFCOMM_BUFFER_SIZE)
#define CCS_FTX3_ARMVOLT_REFU	(18+3*HALFCOMM_BUFFER_SIZE)
#define CCS_FTX3_ARMVOLT_REFL	(19+3*HALFCOMM_BUFFER_SIZE)
// Tx Channel 3 �� ������ �ε��� ����

#define CCS_FTX4_HEAD			(16+4*HALFCOMM_BUFFER_SIZE)
#define CCS_FTX4_CRC			(17+4*HALFCOMM_BUFFER_SIZE)
#define CCS_FTX4_ARMVOLT_REFU	(18+4*HALFCOMM_BUFFER_SIZE)
#define CCS_FTX4_ARMVOLT_REFL	(19+4*HALFCOMM_BUFFER_SIZE)
// Tx Channel 4 �� ������ �ε��� ����



/****************************************************************************/
/* VBC System Input Variable Idx ����                                       */
/****************************************************************************/
///> VBC FComm Tx 1�� Mes �� �����͸� ó���ϹǷ� Mes Tx Idx �� �����ϴ�.
#define VBC_FRX1_HEAD		(16+HALFCOMM_BUFFER_SIZE)
#define VBC_FRX1_CRC		(17+HALFCOMM_BUFFER_SIZE)
#define VBC_FRX1_VOLT_A		(18+HALFCOMM_BUFFER_SIZE)
#define VBC_FRX1_VOLT_B		(19+HALFCOMM_BUFFER_SIZE)
#define VBC_FRX1_VOLT_C		(20+HALFCOMM_BUFFER_SIZE)
#define VBC_FRX1_CURR_A		(21+HALFCOMM_BUFFER_SIZE)
#define VBC_FRX1_CURR_B		(22+HALFCOMM_BUFFER_SIZE)
#define VBC_FRX1_CURR_C		(23+HALFCOMM_BUFFER_SIZE)
// �߰� �׸��� ������ ��. Tx 1�� mis 1,2 ���� �� ������ ��ȯ �뵵�� ���� ������ �ʿ���.
 
#define VBC_FRX2_HEAD			(16+2*HALFCOMM_BUFFER_SIZE)
#define VBC_FRX2_CRC			(17+2*HALFCOMM_BUFFER_SIZE)
#define VBC_FRX2_ARMVOLT_REFU	(18+2*HALFCOMM_BUFFER_SIZE)
#define VBC_FRX2_ARMVOLT_REFL	(19+2*HALFCOMM_BUFFER_SIZE)
// Tx Channel 2 �� ������ �ε��� ���� (From CCS)

#define VBC_FRX3_HEAD		(16+3*HALFCOMM_BUFFER_SIZE)
#define VBC_FRX3_CRC		(17+3*HALFCOMM_BUFFER_SIZE)
#define VBC_FRX3_ARMVOLTL	(18+3*HALFCOMM_BUFFER_SIZE)
#define VBC_FRX3_ARMCURRL	(19+3*HALFCOMM_BUFFER_SIZE)
// Tx Channel 3 �� ������ �ε��� ���� (From VBCL)

// Tx Channel 4 �� ������ �ε��� ����



/****************************************************************************/
/* VBC System Output Variable Idx ����                                       */
/****************************************************************************/
///> VBC FComm Tx 1�� Mes ����̹Ƿ� No Data
#define VBC_FTX1_HEAD		(16+HALFCOMM_BUFFER_SIZE)
#define VBC_FTX1_CRC		(17+HALFCOMM_BUFFER_SIZE)

///> VBC FComm Tx 2,3,4 �� ������ �����ϴ�. �� Arm �� VBC ����.
#define VBC_FTX2_HEAD		(16+2*HALFCOMM_BUFFER_SIZE)
#define VBC_FTX2_CRC		(17+2*HALFCOMM_BUFFER_SIZE)
#define VBC_FTX2_ARMVOLTU	(18+2*HALFCOMM_BUFFER_SIZE)
#define VBC_FTX2_ARMCURRU	(19+2*HALFCOMM_BUFFER_SIZE)
#define VBC_FTX2_ARMVOLTL	(20+2*HALFCOMM_BUFFER_SIZE)
#define VBC_FTX2_ARMCURRL	(21+2*HALFCOMM_BUFFER_SIZE)
// Tx Channel 2 �� ������ �ε��� ����

#define VBC_FTX3_HEAD			(16+3*HALFCOMM_BUFFER_SIZE)
#define VBC_FTX3_CRC			(17+3*HALFCOMM_BUFFER_SIZE)
#define VBC_FTX3_ARMVOLT_REFU	(18+3*HALFCOMM_BUFFER_SIZE)
#define VBC_FTX3_ARMVOLT_REFL	(19+3*HALFCOMM_BUFFER_SIZE)
// Tx Channel 3 �� ������ �ε��� ����

 // Tx Channel 4 �� ������ �ε��� ����






/****************************************************************************/
/* VBCL System Input Variable Idx ����                                       */
/****************************************************************************/
///> VBCL FComm Tx 1�� Mes �� �����͸� ó���ϹǷ� Mes Tx Idx �� �����ϴ�.
#define VBCL_FRX1_HEAD		(16+HALFCOMM_BUFFER_SIZE)
#define VBCL_FRX1_CRC		(17+HALFCOMM_BUFFER_SIZE)
#define VBCL_FRX1_VOLT_A		(18+HALFCOMM_BUFFER_SIZE)
#define VBCL_FRX1_VOLT_B		(19+HALFCOMM_BUFFER_SIZE)
#define VBCL_FRX1_VOLT_C		(20+HALFCOMM_BUFFER_SIZE)
#define VBCL_FRX1_CURR_A		(21+HALFCOMM_BUFFER_SIZE)
#define VBCL_FRX1_CURR_B		(22+HALFCOMM_BUFFER_SIZE)
#define VBCL_FRX1_CURR_C		(23+HALFCOMM_BUFFER_SIZE)
// �߰� �׸��� ������ ��. Tx 1�� mis 1,2 ���� �� ������ ��ȯ �뵵�� ���� ������ �ʿ���.
 
#define VBCL_FRX2_HEAD			(16+2*HALFCOMM_BUFFER_SIZE)
#define VBCL_FRX2_CRC			(17+2*HALFCOMM_BUFFER_SIZE)
#define VBCL_FTX2_ARMVOLT_REFU	(18+2*HALFCOMM_BUFFER_SIZE)
#define VBCL_FTX2_ARMVOLT_REFL	(19+2*HALFCOMM_BUFFER_SIZE)
// Tx Channel 2 �� ������ �ε��� ���� (From CCS)

// Tx Channel 3 �� ������ �ε��� ���� (From VBCL)

// Tx Channel 4 �� ������ �ε��� ����



/****************************************************************************/
/* VBCL System Output Variable Idx ����                                       */
/****************************************************************************/
///> VBCL FComm Tx 1�� Mes ����̹Ƿ� No Data
#define VBCL_FTX1_HEAD		(16+HALFCOMM_BUFFER_SIZE)
#define VBCL_FTX1_CRC		(17+HALFCOMM_BUFFER_SIZE)

///> VBCL FComm Tx 2,3,4 �� ������ �����ϴ�. �� Arm �� VBC ����.
#define VBCL_FTX2_HEAD		(16+2*HALFCOMM_BUFFER_SIZE)
#define VBCL_FTX2_CRC		(17+2*HALFCOMM_BUFFER_SIZE)
#define VBCL_FTX2_ARMVOLTL	(18+2*HALFCOMM_BUFFER_SIZE)
#define VBCL_FTX2_ARMCURRL	(19+2*HALFCOMM_BUFFER_SIZE)
// Tx Channel 2 �� ������ �ε��� ����

// Tx Channel 3 �� ������ �ε��� ����

// Tx Channel 4 �� ������ �ε��� ����


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
 * @brief	Ial Moduel �� �ʱ�ȭ�ϴ� �Լ� 
 * @details	Ial ���� �� ���� Hal�� �ʱ�ȭ �Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void IalControlInit(void);
void IalControlSend(void);
void IalControlReceive(void);

/**
 * @brief	Ial Control ���μ� Ai ���� �д� �Լ�.
 * @details	Ai ä�� 0~15�� �Է����� �� ä���� �Է� ���а��� ��ȯ�ϴ� �Լ�.
 * @param	ulStartCh	: ���� ���� ä���� ���� (0~15)
 *			pfBuf		: ���� ������ float type Buffer ������
 *			ulLen		: ulStartCh ���� ���� ä�� ���� (1~16)
 * @return	���� ���
 * @date	2018-11-24
 * @version	0.0.1
 */
IAL_ERROR IalControlAiRead(uint32_t ulStartCh, float pfBuf[], uint32_t	ulLen);

/**
 * @brief	Ial Control ���μ� ��ä�� ���� �����͸� �д� �Լ�.
 * @details	������ �����ʹ� �ý��ۿ� ���� �̹� ���� ó���ǵǾ�����, �����͸� �ش� Layer ���� �д� �Լ�.
 * @param	ulCh		: ���� ���� ä�� ��ȣ (0~3)
 *			pulBuf		: ���� ������ uint32_t type Buffer ������
 *			ulLen		: ulStartCh ���� ���� ä�� ���� (1~������) : ���� �ִ밪(124)
 * @return	���� ���
 * @date	2018-11-24
 * @version	0.0.1
 */
IAL_ERROR IalControlFiberRead(uint32_t ulCh, uint32_t pulBuf[], uint32_t	ulLen);

/**
 * @brief	Ial Control ���μ� ��ä�� ���� �����͸� �۽��ϴ� �Լ�.
 * @details	�۽��� �����͸� �ش� Layer �� �����ϸ�, ���� �۽��� �ý��ۿ� ���� �۽ŵ�.
 * @param	ulCh		: ���� �۽��� ä�� ��ȣ (0~3)
 *			pulBuf		: �۽��� uint32_t type Buffer ������
 *			ulLen		: �۽��� �������� ���� (1~������) : ���� �ִ밪(124)
 * @return	���� ���
 * @date	2018-11-24
 * @version	0.0.1
 */
IAL_ERROR IalControlFiberWrite(uint32_t ulCh, uint32_t pulBuf[], uint32_t	ulLen);

/**
 * @brief	Ial Control ���μ� VME ���� �����͸� �д� �Լ�.
 * @details	������ �����ʹ� �ý��ۿ� ���� �ֱ������� ���� ó����, �����͸� �ش� Layer ���� �д� �Լ�.
 * @param	ulStartIdx	: ���� ���� ä�� Index ��ȣ (0~127)
 *			pulBuf		: ���� ������ uint32_t type Buffer ������
 *			ulLen		: ulStartIdx ���� ���� ������ ���� �ִ밪(128)
 * @return	���� ���
 * @date	2018-11-24
 * @version	0.0.1
 */
IAL_ERROR IalControlVmeRead(uint32_t ulStartIdx, uint32_t pulBuf[], uint32_t	ulLen);


/**
 * @brief	Ial Control ���μ� VME �����͸� �۽��ϴ� �Լ�.
 * @details	�۽��� �����͸� �ش� Layer �� �����ϸ�, ���� �۽��� �ý��ۿ� ���� �۽ŵ�.
 * @param	ulStartIdx	: ���� �۽��� ������ index ��ȣ (0~127)
 *			pulBuf		: �۽��� uint32_t type Buffer ������
 *			ulLen		: ulStartIdx ���� �۽��� �������� ���� �ִ밪(128)
 * @return	���� ���
 * @date	2018-11-24
 * @version	0.0.1
 */
IAL_ERROR IalControlVmeWrite(uint32_t ulStartIdx, uint32_t pulBuf[], uint32_t	ulLen);


extern U_IALBUFFER_TYPE	uInputBuffer;
extern U_IALBUFFER_TYPE	uOutputBuffer;

extern uint32_t	ulFCommRxLen[HALFCOMM_CH_NUM];


#endif


