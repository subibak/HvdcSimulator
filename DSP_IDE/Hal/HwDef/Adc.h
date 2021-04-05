/**
 * ADC.h
 * ADC Device Module Definition
 */

#ifndef _ADC_H_
#define _ADC_H_


/** 
   Basic Define of ADC Module.
*/
#define ADC_CHANNEL_NUM 16

#define ADC_INPUT_MAX	10000.0		///< Analog Input Max in mV Unit
#define ADC_INPUT_MIN	-10000.0	///< Analog Input Min in mV Unit

#define ADC_INPUT_MAX_HEX	0x00007FFF	///< Analog Input Max in Hex
#define ADC_INPUT_MIN_HEX	0xFFFF8000	///< Analog Input Min in Hex



#if (HW_VER == 3)

/** @name Data Register Definition
 *  Data Register Definition
 */
///@{
#define	HWREG_ADC_INPUT_ADDR		0xD1000000	///< Analog Input Data Register Base Address
#define HWREG_ADC_INPUT_MASK		0xFFFF		///< Analog Input Data Register Data Field Mask
///@}

/** @name Ctrl Register Definition
 *   Ctrl Register Definition.
 */
///@{
#define	HWREG_ADC_CTRL_ADDR			0xD1000011	///< Analog Input Ctrl Register Base Address
#define HWREG_ADC_CTRL_RST_MASK		0x00000001	///< Analog Input Ctrl Register Reset Field Mask
#define HWREG_ADC_CTRL_RST_YES		0x00000001	///< Analog Input Ctrl Register Reset Option 1
#define HWREG_ADC_CTRL_RST_NO		0x00000000	///< Analog Input Ctrl Register Reset Option 2
#define HWREG_ADC_CTRL_RANGE_MASK	0x00000300	///< Analog Input Ctrl Register Range Field Mask
#define HWREG_ADC_CTRL_RANGE_5V		0x00000100	///< Analog Input Ctrl Register Range Option 1
#define HWREG_ADC_CTRL_RANGE_10V	0x00000000	///< Analog Input Ctrl Register Range Option 2
///@}


#else
/** @name Data Register Definition
 *  Data Register Definition
 */
///@{
#define	HWREG_ADC_INPUT_ADDR		0xC1000000	///< Analog Input Data Register Base Address
#define HWREG_ADC_INPUT_MASK		0xFFFF		///< Analog Input Data Register Data Field Mask
///@}

/** @name Ctrl Register Definition
 *   Ctrl Register Definition.
 */
///@{
#define	HWREG_ADC_CTRL_ADDR			0xC1000011	///< Analog Input Ctrl Register Base Address
#define HWREG_ADC_CTRL_RST_MASK		0x00000001	///< Analog Input Ctrl Register Reset Field Mask
#define HWREG_ADC_CTRL_RST_YES		0x00000001	///< Analog Input Ctrl Register Reset Option 1
#define HWREG_ADC_CTRL_RST_NO		0x00000000	///< Analog Input Ctrl Register Reset Option 2
#define HWREG_ADC_CTRL_RANGE_MASK	0x00000300	///< Analog Input Ctrl Register Range Field Mask
#define HWREG_ADC_CTRL_RANGE_5V		0x00000100	///< Analog Input Ctrl Register Range Option 1
#define HWREG_ADC_CTRL_RANGE_10V	0x00000000	///< Analog Input Ctrl Register Range Option 2
///@}
#endif
#endif

