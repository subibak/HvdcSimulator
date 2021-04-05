/*
 ******************************************************************************
 *              LK11 MHCAP Project
 *
 *              (c) Copyright 2019 ~ 2020 LK11 Co., Ltd.
 *
 * All rights reserved.  Protected by international copyright laws.
 * Knowledge of the source code may NOT be used to develop a similar product.
 * Please help us continue to provide the Embedded community with the finest
 * software available.  Your honesty is greatly appreciated.
 ******************************************************************************
 */

/*
 ******************************************************************************
 *
 *                  NXP QuoIQ P2020
 *                     on the
 *                  LK11's LKV-120 VME Master Board
 *
 * Filename      : mhcap_types.h
 * Version       : V1.00
 * Programmer(s) : shjung
 ******************************************************************************
 */

/*
 ******************************************************************************
 REVISION HISTORY :
 ------------------------------------------------------------------------------
 Date          Ver   Name                     Description
 --------------------------------------------------------------------------------
 17/Oct/2019   1.0   SeungHo Jeong (shjung)   Created
 ******************************************************************************
 */


#ifndef __MHCAP_TYPES_H__
#define __MHCAP_TYPES_H__


/* -----------------------type definition ---------------------- */
typedef char		 				int8;
typedef short			 			int16;
typedef	int							int32;

typedef unsigned char		 		uint8;
typedef unsigned char		 		uint08;
typedef unsigned char		 		UINT08;
typedef unsigned char				uchar;

typedef unsigned short				uint16;
typedef unsigned short				UINT16;
typedef unsigned short				ushort;

typedef	unsigned int				uint32;
typedef	unsigned int				UINT32;
typedef unsigned int				uint;

typedef unsigned char		 		byte;
typedef unsigned short				word;

typedef volatile char				vint8;
typedef volatile short				vint16;
typedef volatile int				vint32;

typedef long long					sLONG;
typedef unsigned long long			uLONG;


typedef volatile unsigned char		vuint8;
typedef volatile unsigned char		vuchar;
typedef volatile unsigned char		vbyte;
typedef volatile unsigned char		vuint08;
typedef volatile unsigned short		vword;
typedef volatile unsigned short		vuint16;
typedef volatile unsigned short		vushort;
typedef	volatile unsigned int		vuint;
typedef	volatile unsigned int		vuint32;

typedef volatile unsigned char *	ADDRESS08;
typedef volatile unsigned short *	ADDRESS16;
typedef volatile unsigned int * 	ADDRESS;
typedef volatile unsigned int * 	ADDRESS32;


/* -----------------------MIN & MAX Macro ---------------------- */
#define MAX(a, b)	(((a) > (b)) ? (a) : (b))
#define MIN(a, b)	(((a) < (b)) ? (a) : (b))


/* -----------------------BIN Macro ---------------------- */
#define _2BL(a)		(Int8U)(a),(Int8U)(a>>8)
#define _2BB(a)		(Int8U)(a>>8),(Int8U)(a),
#define _3BL(a)		(Int8U)(a),(Int8U)(a>>8),(Int8U)(a>>16)
#define _3BB(a)		(Int8U)(a>>16),(Int8U)(a>>8),(Int8U)(a)
#define _4BL(a)		(Int8U)(a),(Int8U)(a>>8),(Int8U)(a>>16),(Int8U)(a>>24)
#define _4BB(a)		(Int8U)(a>>24),(Int8U)(a>>16),(Int8U)(a>>8),(Int8U)(a)

#define LongToBin(n) (((n >> 21) & 0x80) | \
                      ((n >> 18) & 0x40) | \
                      ((n >> 15) & 0x20) | \
                      ((n >> 12) & 0x10) | \
                      ((n >>  9) & 0x08) | \
                      ((n >>  6) & 0x04) | \
                      ((n >>  3) & 0x02) | \
                      ((n      ) & 0x01))

#define __BIN(n) LongToBin(0x##n##l)

#define BIN8(n)                       __BIN(n)
#define BIN(n)                        __BIN(n)
#define BIN16(b1,b2)        ((        __BIN(b1)  <<  8UL) + \
                                      __BIN(b2))
#define BIN32(b1,b2,b3,b4)	((((Int32U)__BIN(b1)) << 24UL) + \
                            (((Int32U)__BIN(b2)) << 16UL) + \
                            (((Int32U)__BIN(b3)) <<  8UL) + \
                              (Int32U)__BIN(b4))


/* -----------------------SWAP Macro ---------------------- */

/* swap the MSB with the LSB of a 16 bit integer */
#define VMSB(x)			(((x) >> 8) & 0xff)			/* most signif byte of 2-byte integer */
#define VLSB(x)			(((x) & 0xff) << 8)			/* least signif byte of 2-byte integer*/
#define BYTE_SWAP(x)	(VMSB(x) | VLSB(x))

/* swap the MSW with the LSW of a 32 bit integer */
#define VMSW(x)			(((x) >> 16) & 0xffff)		/* most signif word of 2-word integer */
#define VLSW(x)			(((x) & 0xffff) << 16)		/* least signif byte of 2-word integer*/
#define WORD_SWAP(x)	(VMSW(x) | VLSW(x))

/* 32bit word byte/word swap macros */
#define LLSB(x)			((x) & 0xff)
#define LNLSB(x)		(((x) >> 8) & 0xff)
#define LNMSB(x)		(((x) >> 16) & 0xff)
#define LMSB(x)			(((x) >> 24) & 0xff)
#define LONGSWAP(x) 	((LLSB(x) << 24) | (LNLSB(x) << 16)| (LNMSB(x) << 8) | (LMSB(x)))


/* -----------------------COMMON Macro ---------------------- */
#ifndef TRUE
#define TRUE	(1 == 1)
#endif

#ifndef FALSE
#define FALSE	(1 == 0)
#endif

#ifndef ON
#define ON		(1 == 1)
#endif

#ifndef OFF
#define OFF		(1 == 0)
#endif


#endif	/* __MHCAP_TYPES_H__ */

