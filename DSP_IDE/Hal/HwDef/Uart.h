
/**
 * UART.h
 * UART Device Driver Module Definition
 */

#ifndef _UART_H_
#define _UART_H_

#include "HwIfDef.h"

/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{
#define		UART_FIFO_SIZE 16		///> Level 16
///@}


#define		HWREG_UART_BASE			0x94000000

/** @name Register RBR, THR
 * Reciever Buffer Register, Transmitter Holding Register
 */
///@{
#define		HWREG_UART_RBR							((volatile unsigned int *)(HWREG_UART_BASE+0))		///< Receiver Buffer Register(Read Only, DLAB = 0)
#define		HWREG_UART_THR							((volatile unsigned int *)(HWREG_UART_BASE+0))		///< Transmitter Holding Register(Write Only, DLAB = 0)
//#define		HWREG_UART_RBR							(HWREG_UART_BASE+0)		///< Receiver Buffer Register(Read Only, DLAB = 0)
//#define		HWREG_UART_THR							(HWREG_UART_BASE+0)		///< Transmitter Holding Register(Write Only, DLAB = 0)
///@}

/** @name Register DLL, DLM
 * Divisor Latch Register
 */
///@{
#define		HWREG_UART_DLL							((volatile unsigned int *)(HWREG_UART_BASE+0))		///< Divisor Latch(LS,DLAB = 1)
#define		HWREG_UART_DLL_115200					0x36												///< Baud 115200, 100MHz Base
#define		HWREG_UART_DLM							((volatile unsigned int *)(HWREG_UART_BASE+1))		///< Divisor Latch(MS,DLAB = 1)
#define		HWREG_UART_DLM_0						0x0
//#define		HWREG_UART_DLL							(HWREG_UART_BASE+0)		///< Divisor Latch(LS,DLAB = 1)
//#define		HWREG_UART_DLM							(HWREG_UART_BASE+1)		///< Divisor Latch(MS,DLAB = 1)
///@}

/** @name Register IER
 * Interrupt Enable Register Definition.
 */
///@{
#define		HWREG_UART_IER							((volatile unsigned int *)(HWREG_UART_BASE+1))		///< Interrupt Enable Register
//#define		HWREG_UART_IER							(HWREG_UART_BASE+1)		///< Interrupt Enable Register
#define		HWREG_UART_IER_LPM_MASK					0x20
#define		HWREG_UART_IER_SLEEPMODE_MASK			0x10
#define		HWREG_UART_IER_MODEMSTS_MASK			0x08
#define		HWREG_UART_IER_RCVLINESTS_MASK			0x04
#define		HWREG_UART_IER_TXEMPTY_MASK				0x02
#define		HWREG_UART_IER_RCVAVAIL_MASK			0x01
#define		HWREG_UART_IER_INT_DISABLE				0x00
///@}

/** @name Register IIR
 * Interrupt Indentification Register Definition.
 */
///@{
#define		HWREG_UART_IIR							((volatile unsigned int *)(HWREG_UART_BASE+2))		/* Interrupt Ident. Register(Read Only) */
//#define		HWREG_UART_IIR							(HWREG_UART_BASE+2)		/* Interrupt Ident. Register(Read Only) */
#define		HWREG_UART_IIR_FIFOSTS_MASK				0xC0
#define		HWREG_UART_IIR_FIFOSTS_EN				0xC0
#define		HWREG_UART_IIR_FIFOSTS_DIS				0x00

#define		HWREG_UART_IIR_TOUTPEND_MASK			0x80
#define		HWREG_UART_IIR_TOUTEND_YES				0x80
#define		HWREG_UART_IIR_TOUTEND_NO				0x00

#define		HWREG_UART_IIR_INTTYPE_MASK				0x60
#define		HWREG_UART_IIR_INTTYPE_RCVLINE			0x60
#define		HWREG_UART_IIR_INTTYPE_RCVDATA			0x40
#define		HWREG_UART_IIR_INTTYPE_TXEMPTY			0x20
#define		HWREG_UART_IIR_INTTYPE_MODEMSTS			0x00

#define		HWREG_UART_IIR_INTPEND_MASK				0x01
#define		HWREG_UART_IIR_INTPEND_YES				0x00
#define		HWREG_UART_IIR_INTPEND_NO				0x01
///@}



/** @name Register FCR
 * FIFO Control Register Definition.
 */
///@{
#define		HWREG_UART_FCR							((volatile unsigned int *)(HWREG_UART_BASE+2))		/* FIFO Control Register(Write Only) */
//#define		HWREG_UART_FCR							(HWREG_UART_BASE+2)		/* FIFO Control Register(Write Only) */
#define		HWREG_UART_FCR_RCVFIFO_TRIG_MASK		0xC0												///< Default Level, Level 14
#define		HWREG_UART_FCR_RCVFIFO_TRIG_14			0xC0												///< Level 14
#define		HWREG_UART_FCR_RCVFIFO_TRIG_8			0x80												///< Level 8
#define		HWREG_UART_FCR_RCVFIFO_TRIG_4			0x40												///< Level 4
#define		HWREG_UART_FCR_RCVFIFO_TRIG_1			0x00												///< Level 1

#define		HWREG_UART_FCR_CLR_TXFIFO_MASK			0x04
#define		HWREG_UART_FCR_CLR_TXFIFO_CLR			0x04

#define		HWREG_UART_FCR_CLR_RXFIFO_MASK			0x02
#define		HWREG_UART_FCR_CLR_RXFIFO_CLR			0x02

#define		HWREG_UART_FCR_ENFIFO_MASK				0x01
#define		HWREG_UART_FCR_ENFIFO_EN				0x01
///@}

/** @name Register LCR
 * LINE Control Register Definition.
 */
///@{
#define		HWREG_UART_LCR							((volatile unsigned int *)(HWREG_UART_BASE+3))		/* Line Control Register */
//#define		HWREG_UART_LCR							(HWREG_UART_BASE+3)		/* Line Control Register */
#define		HWREG_UART_LCR_DLAB_MASK				0x80
#define		HWREG_UART_LCR_DLAB_1					0x80
#define		HWREG_UART_LCR_DLAB_0					0x00

#define		HWREG_UART_LCR_PARITY_MASK				0x38
#define		HWREG_UART_LCR_PARITY_LOW				0x38
#define		HWREG_UART_LCR_PARITY_HIGH				0x28
#define		HWREG_UART_LCR_PARITY_EVEN				0x18
#define		HWREG_UART_LCR_PARITY_ODD				0x08
#define		HWREG_UART_LCR_PARITY_NONE				0x00

#define		HWREG_UART_LCR_STOPBIT_MASK				0x04
#define		HWREG_UART_LCR_STOPBIT_2				0x04
#define		HWREG_UART_LCR_STOPBIT_1				0x00

#define		HWREG_UART_LCR_DATALEN_MASK				0x03
#define		HWREG_UART_LCR_DATALEN_8				0x03
#define		HWREG_UART_LCR_DATALEN_7				0x02
#define		HWREG_UART_LCR_DATALEN_6				0x01
#define		HWREG_UART_LCR_DATALEN_5				0x00
///@}



/** @name Register MCR
 * Modem Control Register Definition.
 */
///@{
#define		HWREG_UART_MCR							((volatile unsigned int *)(HWREG_UART_BASE+4))		/* Modem Control Register */
//#define		HWREG_UART_MCR							(HWREG_UART_BASE+4)		/* Modem Control Register */
///@}

/** @name Register LSR
 * Line Status Register Definition.
 */
///@{
#define		HWREG_UART_LSR							((volatile unsigned int *)(HWREG_UART_BASE+5))		/* Line Control Register */
//#define		HWREG_UART_LSR							(HWREG_UART_BASE+5)		/* Line Control Register */
#define		HWREG_UART_LSR_RXFIFO_ERR_MASK			0x80
#define		HWREG_UART_LSR_RXFIFO_ERR_ERR			0x80
#define		HWREG_UART_LSR_RXFIFO_ERR_NOERR			0x00
#define		HWREG_UART_LSR_RXBUFF_EMPTY_MASK		0x40
#define		HWREG_UART_LSR_RXBUFF_EMPTY_YES			0x40
#define		HWREG_UART_LSR_RXBUFF_EMPTY_NO			0x00
#define		HWREG_UART_LSR_TXBUFF_EMPTY_MASK		0x20
#define		HWREG_UART_LSR_TXBUFF_EMPTY_YES			0x20
#define		HWREG_UART_LSR_TXBUFF_EMPTY_NO			0x00
#define		HWREG_UART_LSR_RX_BREAK_MASK			0x10
#define		HWREG_UART_LSR_RX_BREAK_YES				0x10
#define		HWREG_UART_LSR_RX_BREAK_NO				0x00
#define		HWREG_UART_LSR_FRAME_ERR_MASK			0x08
#define		HWREG_UART_LSR_FRAME_ERR_ERR			0x08
#define		HWREG_UART_LSR_FRAME_ERR_NOERR			0x00
#define		HWREG_UART_LSR_PARITY_ERR_MASK			0x04
#define		HWREG_UART_LSR_PARITY_ERR_ERR			0x04
#define		HWREG_UART_LSR_PARITY_ERR_NOERR			0x00
#define		HWREG_UART_LSR_OVRRUN_ERR_MASK			0x02
#define		HWREG_UART_LSR_OVRRUN_ERR_ERR			0x02
#define		HWREG_UART_LSR_OVRRUN_ERR_NOERR			0x00
#define		HWREG_UART_LSR_DATA_RDY_MASK			0x01
#define		HWREG_UART_LSR_DATA_RDY_YES				0x01
#define		HWREG_UART_LSR_DATA_RDY_NO				0x00
///@}


/** @name Register MSR
 * Modem Status Register Definition.
 */
///@{
#define		HWREG_UART_MSR					((volatile unsigned int *)(HWREG_UART_BASE+6))		/* MODEM Control Register */
//#define		HWREG_UART_MSR					(HWREG_UART_BASE+6)		/* MODEM Control Register */
///@}



/** @name Register SCR
 * Scratch Register Definition.
 */
///@{
#define		HWREG_UART_SCR					((volatile unsigned int *)(HWREG_UART_BASE+7))		/* Scratch Register */
//#define		HWREG_UART_SCR					(HWREG_UART_BASE+7)		/* Scratch Register */
///@}

#endif

