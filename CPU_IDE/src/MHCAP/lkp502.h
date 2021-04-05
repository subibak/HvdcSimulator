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
 * Filename      : lkp502.h
 * Version       : V1.00
 * Programmer(s) : shjung
 ******************************************************************************
 */

/*
 ******************************************************************************
 REVISION HISTORY :
 ------------------------------------------------------------------------------
 Date          Ver     Name                     Description
 ------------------------------------------------------------------------------
 12/Jun/2017   1.0.0   Yoon, Tae-Ku (tgyoon)    Created by dinosaur (tgyoon)
 29/Jun/2017   1.0.1   Yoon, Tae-Ku (tgyoon)    Created by dinosaur (tgyoon)
 12/Oct/2019   2.0.1   Yoon, Tae-Ku (tgyoon)    Created by dinosaur (tgyoon)
 21/Nov/2019   2.0.2   Jeong, SeungHo (shjung)  Modified
 ******************************************************************************
 */

#ifndef __LKP502_H__
#define __LKP502_H__


typedef VXB_DEVICE_ID			PCI_DEV;


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/**
 * Twincity v1.0.0 configure
 */
#define	PCI_DEV_NUM				1
#define	PCI_DEV_MEM				0x40

#define	GTX_CH_NUM				2		/* support 2 fiber optic port */

#define	PCI_ADD_NUM				3

#define	PMC_ICR_BASE			0x1EC	/* Interrupt Control Register Address */
#define	PMC_ISR_OFFSET			0x04	/* Interrupt Service Register Address */
#define	PMC_INT_MASK			0x01	/* Interrupt Mask */

#define	DRV_PMC_BUS_NO			0x1
#define	PCI_INST_NAME			"m85xxPci"

#if 1
#define	LK11_ORTHROS			1		/* If on LKV-120 board */
#define	VXB_PCI_DEV				1
#else
#define	MVME2500				1		/* If on MVME-2502 board */
#define	VXB_PCI_DEV				1
#endif

#define	SWIFT_BD_NAME			"LKP-502 PMC Board"
#define	SWIFT_DRV_VER			"2.0.1"

#ifdef	LK11_ORTHROS
#define	swiInByte(addr)			sysInByte(addr)
#define	swiInWord(addr)			sysInWord(addr)
#define	swiInLong(addr)			sysInLong(addr)

#define	swiOutByte(addr,val)	sysOutByte(addr,val)
#define	swiOutWord(addr,val)	sysOutWord(addr,val)
#define	swiOutLong(addr,val)	sysOutLong(addr,val)
#endif	/* LK11_ORTHROS */

#ifdef MVME2500
#undef	DRV_PMC_BUS_NO
#define	DRV_PMC_BUS_NO		0x0
#endif /* MVME2500 Board */

#ifdef VP717
#undef	PCI_INST_NAME
#define	PCI_INST_NAME		"pentiumPci"
#undef	DRV_PMC_BUS_NO
#define	DRV_PMC_BUS_NO		0x0
#endif /* VP-717 Board */

#define	VENDOR_ID			0x1357			/* LK11 Vendor ID */
#define	ETH_VEN_ID			0x8086			/* Intel Vendor ID */
#define	ETH_DEV_ID			0x107A			/* 82546 Fiber Device ID */

#define	SWIFT_DEV_ID		0x1105			/* LKP-500 Device ID */
#define	VESTI_DEV_ID		0x1106			/* HPMC-100 Device ID */
#define	HERIT_DEV_ID		0x1107			/* HPMC-101 Device ID */
#define	TWINC_DEV_ID		0x1108			/* LKP-428 Device ID */
#define	SWI_2_DEV_ID		0x1109			/* LKP-502 Device ID */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

#define	MAX_TX_BUF			0x4000			/* GTX Port TX Buffer Size */
#define	MAX_RX_BUF			0x4000			/* GTX Port RX Buffer Size */

#define	GTX_PORT_OFFSET		0x10000			/* Port Offset Address : 64kB */
#define	RX_ADD_OFFSET		0x4000
#define	GTX_4B_MAX_SIZE		(MAX_TX_BUF/4)

#define	TX_1B_MIN_SIZE		0x10			/* 64 Byte size */
#define	TX_1B_MAX_SIZE		MAX_TX_BUF		/* 16 Kbyte size */

#define PORT_STAT_REG_ADDR	0x8000			/* Port Status Register Address */

#define	ALMOST_COMPLETE		(TX_1B_MIN_SIZE-0x0C)	/* Rx size : 4 */

typedef struct {
	uint32	cfgBaseAddr;
	uint32	regBaseAddr;
	uint32	gt0BaseAddr;
	uint32	gt0StatAddr;
	uint32	gt1BaseAddr;
	uint32	gt1StatAddr;
} SWI_BASE_REG;

typedef struct {
	PCI_DEV	busCtrlId;
	uint32	baseAddr[PCI_ADD_NUM];		/* Base Address Register */
	int		pciBus;
	int		pciDevice;
	int		pciFunc;
	uint8	intLine;
	uint8	intPin;
	uint8	classCode;
	uint8	intFlag;
	uint8	memMask[PCI_ADD_NUM];
	uint8	reserved[8 - PCI_ADD_NUM];
} SWI_PCI_INFO;

typedef struct {
	uint32	txBuf[GTX_4B_MAX_SIZE];
	uint32	rxBuf[GTX_4B_MAX_SIZE];
	uint32	tx4BSize;
	uint32	rx4BSize;
	uint32	txSum;
	uint32	rxSum;
	uint32	intLoopCnt;
	uint32	intRxErrCnt;
} SWI_GTX_TXRX;

typedef struct {
	vuint32	pTxSpace;
	vuint32	pRxSpace;
	vuint32	pRwStatReg;
	vuint32	pRoStatReg;
} SWI_GT_PORT;


extern SWI_BASE_REG *pSwiBaseAddr;
extern SWI_PCI_INFO *pPciInfo;
extern SWI_GTX_TXRX	*pSwiTxRx;
extern SWI_GT_PORT  *pGtPort;


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

#define	RX_MAX_SIZE			MAX_RX_BUF		/* Rx buffer size */
#define	TX_MAX_SIZE			MAX_TX_BUF		/* Tx buffer size */

#define	TEST_SIZE			8192

#define	BD_MAGIC_NUM		0x4C4B3131		/* LK11 */

#define	LED_ON				0x01
#define	LED_OFF				0x00

#define	LED_LEFT			0x01
#define	LED_RIGHT			0x00

#define	GTX_RESET			0x01

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/*
** LKP-502 registers
*/

/* Communication LED Control Register(CLEDCR/RW) */
#define	GTX_LED_ON_RW		0x00
# define GTX_LED_P2_GREEN		0x08
# define GTX_LED_P2_BLUE		0x04
# define GTX_LED_P1_GREEN		0x02
# define GTX_LED_P1_BLUE		0x01


/* Communication LED Status Register(CLEDSR/RW) */
#define	GTX_LED_AT_RW		0x01
# define GTX_LED_MODE_MANUAL	0x01
# define GTX_LED_MODE_AUTO		0x00


/* Status LED Control Register(SLEDCR/RW) */
#define	STA_LED_ON_RW		0x02
# define STA_LED_PCB_RED		0x80
# define STA_LED_PCB_GREEN		0x40
# define S1_LED_BLUE			0x20
# define S2_LED_BLUE			0x10
# define S1_LED_GREEN			0x08
# define S2_LED_GREEN			0x04
# define S1_LED_RED				0x02
# define S2_LED_RED				0x01


/* Status LED Status Register(SLEDSR/RW)  */
#define	STA_LED_AT_RW		0x03
# define STA_LED_MODE_MANUAL	0x01
# define STA_LED_MODE_AUTO		0x00


/* SFP Tx Enable Register(SFPTXEN/RW) */
#define	GTX_TX_EN_RW		0x04
# define GTX_TX_PORT2_ENABLE	0x02
# define GTX_TX_PORT1_ENABLE	0x01

/* GT Reset Control Register(GTRSTEN/RW) */
#define	GTX_RST_RW			0x05
# define GTX_PORT2_RESET		0x02
# define GTX_PORT1_RESET		0x01

/* GT Tx Interrupt Enable Register(TXINTEN/RW) */
#define	TX_INT_EN_RW		0x08
# define TX_INT_PORT2_ENABLE	0x02
# define TX_INT_PORT1_ENABLE	0x01
# define TX_INT_ALL_ENABLE		(TX_INT_PORT1_ENABLE | TX_INT_PORT2_ENABLE)
# define TX_INT_ALL_DISABLE		~(TX_INT_PORT1_ENABLE | TX_INT_PORT2_ENABLE)

/* GT Rx Interrupt Enable Register(RXINTEN/RW) */
#define	RX_INT_EN_RW		0x09
# define RX_INT_PORT2_ENABLE	0x02
# define RX_INT_PORT1_ENABLE	0x01
# define RX_INT_ALL_ENABLE		(RX_INT_PORT1_ENABLE | RX_INT_PORT2_ENABLE)
# define RX_INT_ALL_DISABLE		~(RX_INT_PORT1_ENABLE | RX_INT_PORT2_ENABLE)

/* Vendor ID Register(VENDID/RO) */
#define	VENDOR_ID_RO		0x40
# define VENDOR_ID_VAL		0x4C4B3131		/* LK11 */

/* Hardware Version Register(HWVER/RO) */
#define	LOGIC_VER_RO		0x44
#define	MINOR_VER_RO		0x45
#define	MAJOR_VER_RO		0x46
#define	PLLCLK_OK_RO		0x47

/* DIP Switch Read Register(DIPSW/RO)  */
#define	DIP_SWITCH_RO		0x48
# define DIP_SWITCH_MASK		0x0F

/* Tx Fault/Rx Loss Detection Register(RXLOSS/RO) */
#define TRX_ERROR_DET_RO	0x4A
# define PORT2_TX_FAULT			0x20
# define PORT1_TX_FAULT			0x10
# define PORT2_RX_LOSS			0x02
# define PORT1_RX_LOSS			0x01

/* SFP Module Absence Register(MODABS/RO) */
#define	SFP_ABSENCE_RO		0x4B
# define PORT2_SFP_ABSENCE		0x02
# define PORT1_SFP_ABSENCE		0x01

/* Tx Interrupt Status Register(TXINTST/RO) */
#define	TX_INT_STA_RO		0x4C
# define PORT2_TX_INT_ACT		0x02
# define PORT1_TX_INT_ACT		0x01

/* Rx Interrupt Status Register(RXINTST/RO) */
#define	RX_INT_STA_RO		0x4D
# define PORT2_RX_INT_ACT		0x02
# define PORT1_RX_INT_ACT		0x01

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* Interrupt를 사용할 경우에 1로 설정해야 함 */
#define	PMC_INT_USED		0
#define	TX_INT_USED			0
#define	RX_INT_USED			0

#define	MAIL_BOX_USED		0

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */


#endif	/* __LKP502_H__ */


