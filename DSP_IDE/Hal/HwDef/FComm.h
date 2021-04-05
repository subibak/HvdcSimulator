/**
 * FComm.h
 * Fiber Communication Device Module
 */

#ifndef _FCOMM_H_
#define _FCOMM_H_



/** 
 *  @name FComm Basic Definiction
 */
///@{
#define 	FCOMM_CH_NUM				0x4						///< Channel Num

#define 	FCOMM_CH1					0x0						///< Channel 1
#define 	FCOMM_CH2					0x1						///< Channel 2
#define 	FCOMM_CH3					0x2						///< Channel 3
#define 	FCOMM_CH4					0x3						///< Channel 4

#define		FCOMM_CH_OFFSET(ch)			(ch*0x4000)	///< Channel Offset
//#define		FCOMM_CH_OFFSET				0x4000					///< Channel Offset

///@}


/** @name SFP Module Control Register Definition
 *  SFP Module Control Register Definition
 */
///@{
#define		HWREG_FCOMM_CTRLREG_ADDR			0x80000005			///< Register Address
#define		HWREG_FCOMM_CTRLREG_RST_MASK		0x0000000F			///< D0 ~ D3 : Ch4 ~ Ch1
#define		HWREG_FCOMM_CTRLREG_TXINTEN_MASK		0x00000F00			///<  D0 ~ D3  : Ch1 ~ Ch4
#define		HWREG_FCOMM_CTRLREG_RXINTEN_MASK		0x00000F00			///<  D8 ~ D11 : Ch1 ~ Ch4
#define		HWREG_FCOMM_CTRLREG_INT_MASK			0x0000F000			///< D12 ~ D15 : Ch1 ~ Ch4


// Tx Disable Field 0x95000002 >> 0xF LowActive 동작하지 않음.
#define		HWREG_SFP_TX_DIS_ADDR			0x95000002			///< Register Address
#define		HWREG_SFP_TX_DIS_CH1_MASK		0x01				///< Ch1 Tx_Dis Mask
#define		HWREG_SFP_TX_DIS_CH2_MASK		0x02				///< Ch2 Tx_Dis Mask
#define		HWREG_SFP_TX_DIS_CH3_MASK		0x04				///< Ch3 Tx_Dis Mask
#define		HWREG_SFP_TX_DIS_CH4_MASK		0x08				///< Ch4 Tx_Dis Mask
///@}

/** @name Buffer Register Definition
 *  Buffer Register Definition
 */
///@{

		
#define		HWREG_FCOMM_BASE_ADDR				0xC0000000		///< Ch1 Tx Buffer Address
//#define		HWREG_FCOMM_TXB_ADDR				HWREG_FCOMM_BASE_ADDR		///< Ch1 Tx Buffer Address
//#define		HWREG_FCOMM_RXB_ADDR				(HWREG_FCOMM_BASE_ADDR + 0x400)		///< Ch1 Rx Buffer Address

#define		HWREG_FCOMM_CH_OFFSET				0x4000			///< Ch Offset
#define		HWREG_FCOMM_TX_OFFSET				0x0000			///< Ch Offset
#define		HWREG_FCOMM_RX_OFFSET				0x1000			///< Ch Offset
#define		HWREG_FCOMM_STAT_OFFSET				0x2000			///< Ch Offset

#define		HWREG_FCOMM_TXB_ADDR(ch)			(HWREG_FCOMM_BASE_ADDR + ch*HWREG_FCOMM_CH_OFFSET + HWREG_FCOMM_TX_OFFSET)
#define		HWREG_FCOMM_RXB_ADDR(ch)			(HWREG_FCOMM_BASE_ADDR + ch*HWREG_FCOMM_CH_OFFSET + HWREG_FCOMM_RX_OFFSET)

#define		HWREG_FCOMM_STAT_ADDR(ch)			(HWREG_FCOMM_BASE_ADDR + ch*HWREG_FCOMM_CH_OFFSET + HWREG_FCOMM_STAT_OFFSET)
#define		HWREG_FCOMM_STAT_SIZE_MASK				0xFF0			///< Ch Offset
#define		HWREG_FCOMM_STAT_SIZE_SHIFT				0x4				///< Ch Offset

#define		HWREG_FCOMM_STAT2_ADDR(ch)			(HWREG_FCOMM_BASE_ADDR + ch*HWREG_FCOMM_CH_OFFSET + HWREG_FCOMM_STAT_OFFSET + 5)
#define		HWREG_FCOMM_STAT2_RXLOSS_MASK		0x00000004			///< Rx Signal Lost
#define		HWREG_FCOMM_STAT2_CHBOND_MASK		0x00000010			///< Ch Ready // Bond Function 무관해야함.

#define		TXSIZE(ulSize)						((ulSize - 1)/HWREG_FCOMM_TXSTS_TXSIZE_UNIT)


///@}


/** @name Tx Status Register Definition
 *  Tx Status Register Definition
 */
///@{
#define		HWREG_FCOMM_TXSTS_ADDR				(HWREG_FCOMM_BASE_ADDR + 0x800)		///< Tx Status Register Address

#define		HWREG_FCOMM_TXSTS_TXCOMP_MASK		0x1				///< Tx Status Complete Mask
#define		HWREG_FCOMM_TXSTS_TX_MASK			0x2				///< Tx Data Mask



#define		HWREG_FCOMM_TXSTS_TXSIZE_UNIT		0x10				///< Tx Status Complete Mask Shift
#define		HWREG_FCOMM_TXSTS_TXSIZE_MASK		0x3F0			///< Tx Status Complete Mask
#define		HWREG_FCOMM_TXSTS_TXSIZE_SHIFT		0x4				///< Tx Status Complete Mask Shift

#define		HWREG_FCOMM_TXSTS_RXCOMP_MASK		0x10000			///< Tx Status Complete Mask
 
#endif

