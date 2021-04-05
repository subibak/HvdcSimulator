/**
 * DIO.h
 * Digital IO Device Module Definition
 */

#ifndef _DIO_H_
#define _DIO_H_



/** 
   Digital Output Register
*/

#if (HW_VER == 3)
#define	HWREG_DO_DATA_ADDR						0xD0000000
#else
#define	HWREG_DO_DATA_ADDR						0xC0000000
#endif

#define HWREG_DO_DATA_CH1_MASK					0x00000001
#define HWREG_DO_DATA_CH1_BASE					0x00000000
#define HWREG_DO_DATA_DEFAULT_VALUE				0x00000000


/** 
   Digital IO Ctrl Register
*/
#if (HW_VER == 3)
#define	HWREG_DIO_CTRL_ADDR						0xD0000001
#else
#define	HWREG_DIO_CTRL_ADDR						0xC0000001
#endif

/** @name Bank Dir Field
 *  Direction Configure Field
 */
///@{
#define	HWREG_DIO_CTRL_BANK0_DIR_MASK			0x00000001	///< Bank0 Dir Field Mask
#define	HWREG_DIO_CTRL_BANK0_DIR_OUT			0x00000001	///< Bank0 Dir Option 1
#define	HWREG_DIO_CTRL_BANK0_DIR_IN				0x00000000	///< Bank0 Dir Option 2
#define	HWREG_DIO_CTRL_BANK1_DIR_MASK			0x00000002	///< Bank1 Dir Field Mask
#define	HWREG_DIO_CTRL_BANK1_DIR_OUT			0x00000002	///< Bank1 Dir Option 1
#define	HWREG_DIO_CTRL_BANK1_DIR_IN				0x00000000	///< Bank1 Dir Option 2
#define	HWREG_DIO_CTRL_BANK2_DIR_MASK			0x00000004	///< Bank2 Dir Field Mask
#define	HWREG_DIO_CTRL_BANK2_DIR_OUT			0x00000004	///< Bank2 Dir Option 1
#define	HWREG_DIO_CTRL_BANK2_DIR_IN				0x00000000	///< Bank2 Dir Option 2
#define	HWREG_DIO_CTRL_BANK3_DIR_MASK			0x00000008	///< Bank3 Dir Field Mask
#define	HWREG_DIO_CTRL_BANK3_DIR_OUT			0x00000008	///< Bank3 Dir Option 1
#define	HWREG_DIO_CTRL_BANK3_DIR_IN				0x00000000	///< Bank3 Dir Option 2
///@}

/** @name Bank Enable Field
 *	Bank Enable Field
 */
///@{
#define	HWREG_DIO_CTRL_BANK0_ENA_MASK			0x00000010
#define	HWREG_DIO_CTRL_BANK0_ENA_YES			0x00000010
#define	HWREG_DIO_CTRL_BANK0_ENA_NO				0x00000000
#define	HWREG_DIO_CTRL_BANK1_ENA_MASK			0x00000020
#define	HWREG_DIO_CTRL_BANK1_ENA_YES			0x00000020
#define	HWREG_DIO_CTRL_BANK1_ENA_NO				0x00000000
#define	HWREG_DIO_CTRL_BANK2_ENA_MASK			0x00000040
#define	HWREG_DIO_CTRL_BANK2_ENA_YES			0x00000040
#define	HWREG_DIO_CTRL_BANK2_ENA_NO				0x00000000
#define	HWREG_DIO_CTRL_BANK3_ENA_MASK			0x00000080
#define	HWREG_DIO_CTRL_BANK3_ENA_YES			0x00000080
#define	HWREG_DIO_CTRL_BANK3_ENA_NO				0x00000000
///@}

/** @name Bank Enable Field
 *	Bank Enable Field
 */
///@{
#define	HWREG_DIO_CTRL_XCC_GOOD_MASK			0x00000100
#define	HWREG_DIO_CTRL_XCC_GOOD_YES				0x00000100
#define	HWREG_DIO_CTRL_XCC_GOOD_NO				0x00000000
///@}

/** @name Bank Enable Field
 *	Bank Enable Field
 */
///@{
#define	HWREG_DIO_CTRL_MEZZ_ENA_MASK			0x00000200
#define	HWREG_DIO_CTRL_MEZZ_ENA_YES				0x00000200
#define	HWREG_DIO_CTRL_MEZZ_ENA_NO				0x00000000
///@}

/** @name Bank Enable Field
 *	Bank Enable Field
 */
///@{
#define	HWREG_DIO_CTRL_TERM_DIR_MASK			0x00000400
#define	HWREG_DIO_CTRL_TERM_DIR_OUT				0x00000400
#define	HWREG_DIO_CTRL_TERM_DIR_IN				0x00000000
///@}

/** @name Bank Enable Field
 *	Bank Enable Field
 */
///@{
#define	HWREG_DIO_CTRL_TERM_ENA_MASK			0x00000800
#define	HWREG_DIO_CTRL_TERM_ENA_YES				0x00000800
#define	HWREG_DIO_CTRL_TERM_ENA_NO				0x00000000
///@}

/** @name Bank Enable Field
 *	Bank Enable Field
 */
///@{
#define	HWREG_DIO_CTRL_TERM_BLUELED_MASK		0x00010000		// TERM_DIR: 1, TERM_EN: 1 required
#define	HWREG_DIO_CTRL_TERM_BLUELED_ON			0x00010000		// TERM_DIR: 1, TERM_EN: 1 required
#define	HWREG_DIO_CTRL_TERM_BLUELED_OFF			0x00000000		// TERM_DIR: 1, TERM_EN: 1 required
///@}

/** @name Bank Enable Field
 *	Bank Enable Field
 */
///@{
#define	HWREG_DIO_CTRL_TERM_GREENLED_MASK		0x00020000		// TERM_DIR: 1, TERM_EN: 1 required
#define	HWREG_DIO_CTRL_TERM_GREENLED_ON			0x00020000		// TERM_DIR: 1, TERM_EN: 1 required
#define	HWREG_DIO_CTRL_TERM_GREENLED_OFF		0x00000000		// TERM_DIR: 1, TERM_EN: 1 required
///@}

/** @name Bank Enable Field
 *	Bank Enable Field
 */
///@{
#define	HWREG_DIO_CTRL_TERM_DIOENA_MASK			0x00C00000
#define	HWREG_DIO_CTRL_TERM_DIOENA_YES			0x00C00000
#define	HWREG_DIO_CTRL_TERM_DIOENA_NO			0x00000000
///@}

/** 
   Digital IO Input Data Register
*/
#if (HW_VER == 3)
#define	HWREG_DI_DATA_ADDR						0xD0000002
#else
#define	HWREG_DI_DATA_ADDR						0xC0000002
#endif

#define HWREG_DI_DATA_CH1_MASK					0x00000001
#define HWREG_DI_DATA_CH1_BASE					16

/** 
   Digital IO Configure Register
*/
#if (HW_VER == 3)
#define	HWREG_DIO_CFG_ADDR						0xD0000003
#else
#define	HWREG_DIO_CFG_ADDR						0xC0000003
#endif
/** @name XCC FPGA Dip Switch Field
 *	Bank XCC FPGA Dip Switch Field
 */
///@{
#define	HWREG_DIO_CFG_XCC_DIP_MASK				0x0000000F		// 4pin dip sw
#define	HWREG_DIO_CFG_XCC_DIP_1					0x00000001		// 4pin dip sw
#define	HWREG_DIO_CFG_XCC_DIP_2					0x00000002		// 4pin dip sw
#define	HWREG_DIO_CFG_XCC_DIP_4					0x00000004		// 4pin dip sw
#define	HWREG_DIO_CFG_XCC_DIP_8					0x00000008		// 4pin dip sw
///@}

/** @name Mezzanine Board Insert Field
 *	Bank Mezzanine Board Insert Field
 */
///@{
#define	HWREG_DIO_CFG_MEZ_INS_MASK				0x00000100		// Mezzanine Card Inserted?
#define	HWREG_DIO_CFG_MEZ_INS_YES				0x00000100		// Mezzanine Card Inserted?
#define	HWREG_DIO_CFG_MEZ_INS_NO				0x00000000		// Mezzanine Card Inserted?
///@}

/** @name XCC PLLLOCK Field
 *	Bank XCC PLLLOCK Field
 */
///@{
#define	HWREG_DIO_CFG_XCC_PLLLOCK_MASK			0x00000200		// PLL Lock Status 0: reset required.
#define	HWREG_DIO_CFG_XCC_PLLLOCK_YES			0x00000200		// PLL Lock Status 0: reset required.
#define	HWREG_DIO_CFG_XCC_PLLLOCK_NO			0x00000200		// PLL Lock Status 0: reset required.
///@}

/** @name Terminal Board Insert Mask
 *	Terminal Board Insert Mask
 */
///@{
#define	HWREG_DIO_CFG_TERM_INS_MASK				0x000F0000		// TERM_DIR: 0, TERM_EN: 1 required
#define	HWREG_DIO_CFG_TERM_INS_YES				0x000C0000		// 0xC: inserted, else: Not inserted
///@}

																
#endif

