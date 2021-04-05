/**
 * DAC.h
 * DAC Device Module Definition
 */

#ifndef _DAC_H_
#define _DAC_H_


/** 
   Basic Define of DAC Module.
*/
#define DAC_CHANNEL_NUM 8

#define DAC_OUTPUT_MAX	10000.0		///< Analog Output Max in mV Unit
#define DAC_OUTPUT_MIN	-10000.0	///< Analog Output Min in mV Unit

#define DAC_OUTPUT_MAX_HEX	0x00007FFF	///< Analog Input Max in Hex
#define DAC_OUTPUT_MIN_HEX	0xFFFF8000	///< Analog Input Min in Hex


/** @name Ctrl Register Definition
 *  Ctrl Register Definition
 */
///@{

#if (HW_VER == 3)
#define	HWREG_DAC_CTRL_ADDR				0xD1000010	///< DAC Ctrl Register Base Address
#else
#define	HWREG_DAC_CTRL_ADDR				0xC1000010	///< DAC Ctrl Register Base Address
#endif

#define HWREG_DAC_CTRL_RST_MASK			0x00000001	///< DAC Ctrl Register Reset Field Mask
#define HWREG_DAC_CTRL_RST				0x00000001	///< DAC Ctrl Register Reset Field Option 1

#define HWREG_DAC_CTRL_CLR_MASK			0x00000002	///< DAC Ctrl Register Clear Field Mask
#define HWREG_DAC_CTRL_CLR				0x00000002	///< DAC Ctrl Register Clear Field Option 1

#define HWREG_DAC_CTRL_RSTSEL_MASK		0x00000004	///< DAC Ctrl Register Reset Select Field Mask
#define HWREG_DAC_CTRL_RSTSEL_0V		0x00000004	///< DAC Ctrl Register Reset Select Field Option

#define HWREG_DAC_CTRL_BTC_MASK			0x00000008	///< DAC Ctrl Register BTC Field Mask
#define HWREG_DAC_CTRL_BTC_2COML		0x00000008	///< DAC Ctrl Register BTC Field Option 1
#define HWREG_DAC_CTRL_BTC_SBIN			0x00000000	///< DAC Ctrl Register BTC Field Option 2

#define HWREG_DAC_CTRL_LDAC_MASK		0x00000010	///< DAC Ctrl Register LDAC Field Mask
#define HWREG_DAC_CTRL_LDAC_DIS			0x00000010	///< DAC Ctrl Register LDAC Field Option 1
#define HWREG_DAC_CTRL_LDAC_PLAT		0x00000000	///< DAC Ctrl Register LDAC Field Option 2

#define HWREG_DAC_CTRL_AUTOLDAC_MASK	0x00000020	///< DAC Ctrl Register AUTOLDAC Field Mask
#define HWREG_DAC_CTRL_AUTOLDAC_AUTO	0x00000020	///< DAC Ctrl Register AUTOLDAC Field Option 1
#define HWREG_DAC_CTRL_AUTOLDAC_PLAT	0x00000000	///< DAC Ctrl Register AUTOLDAC Field Option 2
///@}


/** @name Cfg Register Definition
 *  Cfg Register Definition
 */
///@{
#if (HW_VER == 3)
#define	HWREG_DAC_CFG_ADDR				0xD1000020	///< DAC Cfg Register Base Address
#else
#define	HWREG_DAC_CFG_ADDR				0xC1000020	///< DAC Cfg Register Base Address
#endif

#define HWREG_DAC_CFG_GAIN_MASK			0x00000080	///< DAC Cfg Register Gain Field Mask
#define HWREG_DAC_CFG_GAIN_4			0x00000080	///< DAC Cfg Register Gain Field Option 1
#define HWREG_DAC_CFG_GAIN_6			0x00000000	///< DAC Cfg Register Gain Field Option 2

#define HWREG_DAC_CFG_SCE_MASK			0x00000400	///< DAC Cfg Register SCE Field Mask
#define HWREG_DAC_CFG_SCE_ENA			0x00000400	///< DAC Cfg Register SCE Field Option 1
#define HWREG_DAC_CFG_SCE_DIS			0x00000000	///< DAC Cfg Register SCE Field Option 2
///@}

/** @name Data Register Definition
 *  Data Register Definition
 */
///@{
#if (HW_VER == 3)
#define	HWREG_DAC_OUTPUT_ADDR			0xD1000028	///< DAC Data Register Base Address
#else
#define	HWREG_DAC_OUTPUT_ADDR			0xC1000028	///< DAC Data Register Base Address
#endif
#define HWREG_DAC_OUTPUT_MASK			0xFFFF		///< DAC Data Register Data Field Mask
///@}

#endif

