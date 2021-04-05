/**
 * Led.h
 * LedControl
 */

#ifndef _LED_H_
#define _LED_H_

#define		RUN_LED_MASK			0x00000001
#define		RUN_LED_SHIFT			0x0
#define		VME_LED_MASK			0x00000002
#define		VME_LED_SHIFT			0x1

#define		SLED_MASK				0x000000FC
#define		STA1_LED_MASK			0x0000001C
#define		STA1_LED_SHIFT			0x2
#define		STA2_LED_MASK			0x000000E0
#define		STA2_LED_SHIFT			0x5

#define		RED_COLOR				0x1
#define		GREEN_COLOR				0x2
#define		BLUE_COLOR				0x4

#define		FAIL_LED_MASK			0x00000100
#define		FAIL_LED_SHIFT			0x8
#define		STA_LED_MASK			0x00000200
#define		STA_LED_SHIFT			0x9


#define		OLED_MASK				0xFF000000



/** 
   Basic Define of ADC Module.
*/
#define LED_ON	0x1
#define LED_OFF	0x0

/** @name Oled Control Register Definition
 *  Oled Control Register Definition
 */
///@{
#if 0 // not for Hw Ver 3.x
#define	HWREG_OLED_CTRL_ADDR			0x80000000		///< Optic Led Control Register
#define HWREG_OLED_CTRL_TXLED_MASK		0x000F				///< Led Status Mask R/W 1-On, 0-Off
#define HWREG_OLED_CTRL_RXLED_MASK		0x00F0				///< Led Status Mask R/W 1-On, 0-Off

#define HWREG_OLED_CTRL_TXLEDMODE_MASK		0x0F00				///< Optic Led Tx Mode
#define HWREG_OLED_CTRL_TXLEDMODE_AUTO		0x0000				///< Auto
#define HWREG_OLED_CTRL_TXLEDMODE_MANUAL	0x0F00				///< Manual

#define HWREG_OLED_CTRL_RXLEDMODE_MASK		0xF000				///< Optic Led Rx Mode
#define HWREG_OLED_CTRL_TXLEDMODE_AUTO		0x0000				///< Auto
#define HWREG_OLED_CTRL_TXLEDMODE_MANUAL	0xF000				///< Manual
#endif
///@}

/** @name Ctrl Register Definition
 *   Ctrl Register Definition.
 */
///@{

#if 1
#define	HWREG_LED_CTRL_ADDR			0x95000000		///< Led Control Register
#define HWREG_LED_CTRL_RUNLED_MASK		0x0001				///< SLed Upper Left (Blue)
#define HWREG_LED_CTRL_VMELED_MASK		0x0002				///< SLed Upper Right (Green)

#define HWREG_LED_CTRL_SLED1R_MASK		0x0004				///< SLed Lower Left (Red)
#define HWREG_LED_CTRL_SLED1G_MASK		0x0008				///< SLed Lower Left (Green)
#define HWREG_LED_CTRL_SLED1B_MASK		0x0010				///< SLed Lower Left (Blue)

#define HWREG_LED_CTRL_SLED2R_MASK		0x0020				///< SLed Lower Right (Red)
#define HWREG_LED_CTRL_SLED2G_MASK		0x0040				///< SLed Lower Right (Green)
#define HWREG_LED_CTRL_SLED2B_MASK		0x0080				///< SLed Lower Right (Blue)

#define HWREG_LED_CTRL_DLEDULB_MASK		0x0100				///< DLed Upper Left (Blue)
#define HWREG_LED_CTRL_DLEDURG_MASK		0x0200				///< DLed Upper Right (Green)

#define HWREG_LED_CTRL_OLED_MASK		0xFF000000				///< OLED Upper Right (Green)



#define	HWREG_LED_MODE_ADDR			0x95000001		///< Led Mode Register
#define HWREG_LED_MODE_SLEDULB_MASK		0x0001				///< SLed Upper Left (Blue)
#define HWREG_LED_MODE_SLEDURG_MASK		0x0002				///< SLed Upper Right (Green)

#define HWREG_LED_MODE_SLEDLLR_MASK		0x0004				///< SLed Lower Left (Red)    -NA
#define HWREG_LED_MODE_SLEDLLG_MASK		0x0008				///< SLed Lower Left (Green)  -NA
#define HWREG_LED_MODE_SLEDLLB_MASK		0x0010				///< SLed Lower Left (Blue)   -NA

#define HWREG_LED_MODE_SLEDLRR_MASK		0x0020				///< SLed Lower Right (Red)   -NA
#define HWREG_LED_MODE_SLEDLRG_MASK		0x0040				///< SLed Lower Right (Green) -NA
#define HWREG_LED_MODE_SLEDLRB_MASK		0x0080				///< SLed Lower Right (Blue)  -NA

#define HWREG_LED_MODE_DLEDULB_MASK		0x0100				///< DLed Upper Left (Blue)
#define HWREG_LED_MODE_DLEDURG_MASK		0x0200				///< DLed Upper Right (Green)

#define HWREG_LED_CTRL_OLED_MASK		0xFF000000				///< OLED Upper Right (Green)

#else
#define	HWREG_LED_CTRL_ADDR			0x95000000		///< Led Control Register
#define HWREG_LED_CTRL_SLEDULB_MASK		0x0001				///< SLed Upper Left (Blue)
#define HWREG_LED_CTRL_SLEDURG_MASK		0x0002				///< SLed Upper Right (Green)

#define HWREG_LED_CTRL_SLEDLLR_MASK		0x0004				///< SLed Lower Left (Red)
#define HWREG_LED_CTRL_SLEDLLG_MASK		0x0008				///< SLed Lower Left (Green)
#define HWREG_LED_CTRL_SLEDLLB_MASK		0x0010				///< SLed Lower Left (Blue)

#define HWREG_LED_CTRL_SLEDLRR_MASK		0x0020				///< SLed Lower Right (Red)
#define HWREG_LED_CTRL_SLEDLRG_MASK		0x0040				///< SLed Lower Right (Green)
#define HWREG_LED_CTRL_SLEDLRB_MASK		0x0080				///< SLed Lower Right (Blue)

#define HWREG_LED_CTRL_DLEDULB_MASK		0x0100				///< DLed Upper Left (Blue)
#define HWREG_LED_CTRL_DLEDURG_MASK		0x0200				///< DLed Upper Right (Green)



#define	HWREG_LED_MODE_ADDR			0x95000001		///< Led Mode Register
#define HWREG_LED_MODE_SLEDULB_MASK		0x0001				///< SLed Upper Left (Blue)
#define HWREG_LED_MODE_SLEDURG_MASK		0x0002				///< SLed Upper Right (Green)

#define HWREG_LED_MODE_SLEDLLR_MASK		0x0004				///< SLed Lower Left (Red)    -NA
#define HWREG_LED_MODE_SLEDLLG_MASK		0x0008				///< SLed Lower Left (Green)  -NA
#define HWREG_LED_MODE_SLEDLLB_MASK		0x0010				///< SLed Lower Left (Blue)   -NA

#define HWREG_LED_MODE_SLEDLRR_MASK		0x0020				///< SLed Lower Right (Red)   -NA
#define HWREG_LED_MODE_SLEDLRG_MASK		0x0040				///< SLed Lower Right (Green) -NA
#define HWREG_LED_MODE_SLEDLRB_MASK		0x0080				///< SLed Lower Right (Blue)  -NA

#define HWREG_LED_MODE_DLEDULB_MASK		0x0100				///< DLed Upper Left (Blue)
#define HWREG_LED_MODE_DLEDURG_MASK		0x0200				///< DLed Upper Right (Green)
#endif

///@}







#endif





