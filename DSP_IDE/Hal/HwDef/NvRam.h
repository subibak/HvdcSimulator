
/**
 * VME.h
 * VME Device Driver Module Definition
 */

#ifndef _NVRAM_H_
#define _NVRAM_H_



/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{
#define MRAM_SIZE						0x100000						///> MRAM Size
#define HWREG_MRAM_BASE					0x92000000						///> Base Address



#define HWREG_MRAM_WP_ADDR			0x95000002

#define HWREG_MRAM_WP_MASK			0x00000200
#define HWREG_MRAM_WP_YES			0x00000200
#define HWREG_MRAM_WP_NO			0x00000000



///@}



#endif

