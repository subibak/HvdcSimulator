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
 * Filename      : lkp502.c
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

/*
 ******************************************************************************
 *                             INCLUDE FILES
 ******************************************************************************
 */

#if 0
#include "lkp502.h"
#else
#include "project_def.h"
#endif


/*
 ******************************************************************************
 *                             DEFINES
 ******************************************************************************
 */

/*
 ******************************************************************************
 *                             GLOBAL VARIABLES
 ******************************************************************************
 */

LOCAL int		initCnt = 0;

LOCAL SWI_PCI_INFO	PmcInfo[PCI_DEV_NUM];
LOCAL SWI_BASE_REG	SwiBaseAddr;
LOCAL SWI_GT_PORT	swiGtPort [GTX_CH_NUM];

SWI_PCI_INFO	*pPmcInfo = NULL;
SWI_BASE_REG	*pSwiBaseAddr = NULL;
SWI_GT_PORT		*pGtPort = NULL;

#if PMC_INT_USED
LOCAL SWI_GTX_TXRX	SwiTxRxBuf[GTX_CH_NUM];
SWI_GTX_TXRX	*pSwiTxRx = NULL;
#endif

const char *lkp502lStr = "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";


/*
 ******************************************************************************
 *                             FUNCTIONS
 ******************************************************************************
 */

/***
 * @brief  GTX Tx/Rx LED mode setting
 * @param  Manual(1) & Automode(0)
 * @retval None.
 */
void Lkp502SetCLedAutoMode (int auto_mode)
{
	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;
	if (auto_mode)	/* auto mode */
		swiOutByte (pSwiBaseAddr->regBaseAddr + GTX_LED_AT_RW, GTX_LED_MODE_AUTO);
	else			/* manual mode */
		swiOutByte (pSwiBaseAddr->regBaseAddr + GTX_LED_AT_RW, GTX_LED_MODE_MANUAL);
}

/***
 * @brief  Get GTX Tx/Rx LED mode status
 * @param  None.
 * @retval Manual(ERROR) & Automode(OK)
 */
STATUS Lkp502GetCLedAutoMode (void)
{
	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;
	if (swiInByte (pSwiBaseAddr->regBaseAddr + GTX_LED_AT_RW) != 0)
		return ERROR;	/* manual mode */
	else
		return OK;		/* auto mode */
}

/***
 * @brief  Rx LED ON/OFF - This function can only be executed in manual mode.
 * @param  Port number
 * @param  ON(1) & OFF(0)
 * @retval None.
 */
STATUS Lkp502SetRxLed (int port, int ctrl)
{
	byte     data;

	if (port >= GTX_CH_NUM)		return ERROR;
	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;

	/* Check CLEDSR */
	if (Lkp502GetCLedAutoMode() == OK)		return ERROR;

	data = swiInByte(pSwiBaseAddr->regBaseAddr);
	if (!port) {
		if (ctrl == LED_ON)	data |= 0x01;
		else				data &= 0xFE;
	} else {
		if (ctrl == LED_ON)	data |= 0x04;
		else				data &= 0xFB;
	}

	swiOutByte(pSwiBaseAddr->regBaseAddr, data);
	return OK;
}

/***
 * @brief  Tx LED ON/OFF - This function can only be executed in manual mode.
 * @param  Port number
 * @param  ON(1) & OFF(0)
 * @retval None.
 */
STATUS Lkp502SetTxLed (int port, int ctrl)
{
	byte     data;

	if (port >= GTX_CH_NUM)		return ERROR;
	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;

	/* Check CLEDSR */
	if (Lkp502GetCLedAutoMode() == OK)		return ERROR;

	data = swiInByte(pSwiBaseAddr->regBaseAddr);
	if (!port) {
		if (ctrl == LED_ON)	data |= 0x02;
		else				data &= 0xFD;
	} else {
		if (ctrl == LED_ON)	data |= 0x08;
		else				data &= 0xF7;
	}
	
	swiOutByte(pSwiBaseAddr->regBaseAddr, data);
	return OK;
}

/***
 * @brief  Status LED setting
 * @param  Manual(1) & Automode(0)
 * @retval  None.
 */
void Lkp502SetSLedAutoMode (int mode)
{
	byte     data;

	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;
	data = swiInByte(pSwiBaseAddr->regBaseAddr + STA_LED_AT_RW);
	
	if (mode)	data &= 0xFE;	/* auto mode */
	else		data |= 0x01;	/* manual mode */
	
	swiOutByte(pSwiBaseAddr->regBaseAddr + STA_LED_AT_RW, data);
}

/***
 * @brief  Get status LED mode status
 * @param  None.
 * @retval Manual(ERROR) & Automode(OK)
 */
STATUS Lkp502GetSLedAutoMode (void)
{
	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;
	if (swiInByte (pSwiBaseAddr->regBaseAddr + STA_LED_AT_RW) != 0)
		return ERROR;	/* manual mode */
	else
		return OK;		/* auto mode */
}

/***
 * @brief  Set Status LED control
 * @param  ON(1)/OFF(0) => R(PCB),G(PCB),B(L),B(R),G(L),G(R),R(L),R(R)
 * @retval None.
 */
STATUS Lkp502SetSLedCtrl (byte value)
{
	byte     data;

	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;

	/* Check SLEDSR */
	if (Lkp502GetSLedAutoMode() == OK)	return ERROR;

	swiOutByte(pSwiBaseAddr->regBaseAddr + STA_LED_ON_RW, value);

	return OK;
}

/***
 * @brief  En/Disable SFP TX
 * @param  Port number
 * @param  Tx Enable (1) & Tx Disable (0)
 * @retval None.
 */
STATUS Lkp502SetSfpTxCtrl (int port, int enable)
{
	byte     data;

	if (port >= GTX_CH_NUM)		return ERROR;
	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;

	data = swiInByte(pSwiBaseAddr->regBaseAddr + GTX_TX_EN_RW);

	if (!port) {
		if (enable)		data |= GTX_TX_PORT1_ENABLE;
		else			data &= 0xFE;
	} else {
		if (enable)		data |= GTX_TX_PORT2_ENABLE;
		else			data &= 0xFD;
	}

	swiOutByte(pSwiBaseAddr->regBaseAddr + GTX_TX_EN_RW, data);

	return OK;
}

/***
 * @brief  GTX reset ON/OFF
 * @param  Port number
 * @param  Set(0)/Reset(1)
 * @retval None.
 */
STATUS Lkp502SetGtxRstCtrl (int port, int reset)
{
	byte     data;

	if (port >= GTX_CH_NUM)		return ERROR;
	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;
	
	data = swiInByte(pSwiBaseAddr->regBaseAddr + GTX_RST_RW);

	if (!port) {
		if (reset == GTX_RESET)	data |= GTX_PORT1_RESET;
		else					data &= 0xFE;
	} else {
		if (reset == GTX_RESET)	data |= GTX_PORT2_RESET;
		else					data &= 0xFD;
	}		

	swiOutByte(pSwiBaseAddr->regBaseAddr + GTX_RST_RW, data);

	return OK;
}

/***
 * @brief  En/Disable GT Tx Interrupt Register
 * @param  Enable (1)/Disble (0)
 * @retval None.
 */
STATUS Lkp502SetTxInt (int port, int enable)
{
	byte     data;

	if (port >= GTX_CH_NUM)		return ERROR;
	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;
	
	data = swiInByte(pSwiBaseAddr->regBaseAddr + TX_INT_EN_RW);

	if (!port) {
		if (enable)		data |= TX_INT_PORT1_ENABLE;
		else			data &= 0xFE;
	} else {
		if (enable)		data |= TX_INT_PORT2_ENABLE;
		else			data &= 0xFD;
	}		

	swiOutByte(pSwiBaseAddr->regBaseAddr + TX_INT_EN_RW, data);

	return OK;
}

/***
 * @brief  En/Disable GT Rx Interrupt Register
 * @param  Enable (1)/Disble (0)
 * @retval None.
 */
STATUS Lkp502SetRxInt (int port, int enable)
{
	byte     data;

	if (port >= GTX_CH_NUM)		return ERROR;
	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;
	
	data = swiInByte(pSwiBaseAddr->regBaseAddr + RX_INT_EN_RW);

	if (!port) {
		if (enable)		data |= RX_INT_PORT1_ENABLE;
		else			data &= 0xFE;
	} else {
		if (enable)		data |= RX_INT_PORT2_ENABLE;
		else			data &= 0xFD;
	}		

	swiOutByte(pSwiBaseAddr->regBaseAddr + RX_INT_EN_RW, data);

	return OK;
}

#if PMC_INT_USED
#if TX_INT_USED
/***
 * @brief  En/Disable GT Tx Interrupt Register
 * @param  Enable (1)/Disble (0)
 * @retval None.
 */
void Lkp502SetAllTxInt (int enable)
{
	int		i;
	for (i = 0; i < GTX_CH_NUM; i ++)
		Lkp502SetTxInt (i, enable);
}
#endif	/* TX_INT_USED */

#if RX_INT_USED
/***
 * @brief  En/Disable GT Rx Interrupt Register
 * @param  Enable (1)/Disble (0)
 * @retval None.
 */
void Lkp502SetAllRxInt (int enable)
{
	int		i;
	for (i = 0; i < GTX_CH_NUM; i ++)
		Lkp502SetRxInt (i, enable);
}
#endif	/* RX_INT_USED */
#endif	/* PMC_INT_USED */

/***
 * @brief  GTX Tx size setting
 * @param  GTX port number
 * @param  GTX tx size(bytes)
 * @retval Tx 4bytes size.
 */
uint32 Lkp502SetTxSize (int port, uint32 size)
{
	uint32   data;

	if (size <= TX_1B_MIN_SIZE)		size = TX_1B_MIN_SIZE;
	if (size >= TX_1B_MAX_SIZE)		size = TX_1B_MAX_SIZE;

	/* 4bytes size */
	size = ((size / 4) - 1) & 0x0FF0;

	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;
	if (!port) {
		data = (swiInLong(pSwiBaseAddr->gt0StatAddr) & 0xFFFFF00F) | size;
		swiOutLong(pSwiBaseAddr->gt0StatAddr, data);
	} else {
		data = (swiInLong(pSwiBaseAddr->gt1StatAddr) & 0xFFFFF00F) | size;
		swiOutLong(pSwiBaseAddr->gt1StatAddr, data);
	}

	data += 0x10;
	return (data);
}

/***
 * @brief  LKP-502 board optical cable connection check
 * @param  port number
 * @retval ERROR or OK.
 */
STATUS Lkp502CableCheck (int port)
{
	byte     data;

	if (port >= GTX_CH_NUM)		return ERROR;
	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;
	if (!port)
		data = swiInByte(pSwiBaseAddr->gt0StatAddr + 0x14) & 0x04;
	else
		data = swiInByte(pSwiBaseAddr->gt1StatAddr + 0x14) & 0x04;

	if (data)	return ERROR;

	return OK;
}

/***
 * @brief  check Tx falut status of LKP-502 board
 * @param  port number
 * @retval ERROR or OK.
 */
STATUS Lkp502GetTxErrorStatus (int port, uchar *val)
{
	if (port >= GTX_CH_NUM)		return ERROR;
	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;
	
	if (!port)
		*val = (swiInByte (pSwiBaseAddr->regBaseAddr + TRX_ERROR_DET_RO) >> 4) & 0x01;
	else
		*val = (swiInByte (pSwiBaseAddr->regBaseAddr + TRX_ERROR_DET_RO) >> 5) & 0x01;

	return OK;
}

/***
 * @brief  check Rx loss status of LKP-502 board
 * @param  port number
 * @retval ERROR or OK.
 */
STATUS Lkp502GetRxErrorStatus (int port, uchar *val)
{
	if (port >= GTX_CH_NUM)		return ERROR;
	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;
	
	if (!port)
		*val = (swiInByte (pSwiBaseAddr->regBaseAddr + TRX_ERROR_DET_RO) >> 0) & 0x01;
	else
		*val = (swiInByte (pSwiBaseAddr->regBaseAddr + TRX_ERROR_DET_RO) >> 1) & 0x01;

	return OK;
}

/***
 * @brief  check Rx loss status of LKP-502 board
 * @param  port number
 * @retval ERROR or OK.
 */
STATUS Lkp502GetSFPAbsenceStatus (int port, uchar *val)
{
	if (port >= GTX_CH_NUM)		return ERROR;
	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;

	if (!port)
		*val = (swiInByte (pSwiBaseAddr->regBaseAddr + SFP_ABSENCE_RO) >> 0) & 0x01;
	else
		*val = (swiInByte (pSwiBaseAddr->regBaseAddr + SFP_ABSENCE_RO) >> 1) & 0x01;

	return OK;
}

/***
 * @brief  GTX Rx read
 * @param  GTX port number
 * @param  Rx buffer address
 * @param  Rx buffer size
 * @retval None.
 */
STATUS Lkp502Read (int port, uint32 *pBuf, uint32 size)
{
	int      i;
	uint32   addr;
	uint32   *d_addr = NULL;
	SWI_BASE_REG *pBase = NULL;

	if (port >= GTX_CH_NUM) 	return ERROR;

	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;
	pBase = pSwiBaseAddr;
	d_addr = pBuf;

	if (!port)
		addr = pBase->gt0BaseAddr + RX_ADD_OFFSET;
	else
		addr = pBase->gt1BaseAddr + RX_ADD_OFFSET;

#if 0
	for (i = 0; i < size; i ++) {
#else
	for (i = 0; i < size / 4; i ++) {
#endif
		*d_addr = swiInLong(addr);
		d_addr ++;
		addr += 4;
	}

#if	RX_INT_USED
	if (!port)
		addr  = pBase->gt0StatAddr;
	else
		addr  = pBase->gt1StatAddr;

	/* Rx complete set : Tx interrupt start */
	swiOutByte(addr, (swiInLong(addr) | 0x10000));

	/* Rx complete clear */
	swiOutByte(addr, (swiInLong(addr) & ~0x10000));
#endif	/* RX_INT_USED */

	return OK;
}

/***
 * @brief  GTX Tx write
 * @param  GTX port number
 * @param  Tx buffer address
 * @param  Tx buffer size(4bytes)
 * @retval None.
 */
STATUS Lkp502Write (int port, uint32 *pBuf, uint32 size)
{
	int      i;
	uint32   addr;
	uint32   *s_addr = NULL;
	SWI_BASE_REG *pBase = NULL;
#if	TX_INT_USED
	uint32   reg;
	byte     data;
#endif	/* TX_INT_USED */

	if (port >= GTX_CH_NUM) 	return ERROR;

	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;
	pBase = pSwiBaseAddr;
	s_addr = pBuf;

	if (!port)
		addr = pBase->gt0BaseAddr;
	else
		addr = pBase->gt1BaseAddr;

#if	TX_INT_USED
	if (!port)
		reg  = pBase->gt0StatAddr;
	else
		reg  = pBase->gt1StatAddr;
#endif	/* TX_INT_USED */
	
#if	TX_INT_USED
	/* Tx transfer mode : Rx interrupt enable */
	data = swiInByte(reg) | 0x02;
	swiOutByte(reg, data);
#endif	/* TX_INT_USED */

#if 0
	for (i = 0; i < TX_1B_MIN_SIZE; i ++) {
#else
	for (i = 0; i < TX_1B_MIN_SIZE / 4; i ++) {
#endif
		swiOutLong(addr, *s_addr);
		s_addr ++;
		addr += 4;
	}

#if	TX_INT_USED
	/* Tx complete set : Rx interrupt start */
	data = swiInByte(reg) | 0x01;
	swiOutByte(reg, data);
#endif	/* TX_INT_USED */

#if 0
	for (i = 0; i < size - TX_1B_MIN_SIZE; i++) {
#else
	for (i = 0; i < (size - TX_1B_MIN_SIZE) / 4; i ++) {
#endif
		swiOutLong(addr, *s_addr);
		s_addr ++;
		addr += 4;
	}

#if	TX_INT_USED
	/* Tx complete clear */
	data = swiInByte(reg) & 0xFC;
	swiOutByte(reg, data);
#endif	/* TX_INT_USED */

	return OK;
}

#if PMC_INT_USED
/***
 * @brief  LKP-502 board interrupt handler
 * @param  PCI bus register address.
 * @retval None.
 */
LOCAL void Lkp502ProcInt (void)
{
	byte     mode, data;
	uint32   size, sum;
	uint32   *addr = NULL;
	SWI_BASE_REG *pBase = NULL;
	SWI_GTX_TXRX *pTxRx = NULL;

	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;
	pBase = pSwiBaseAddr;
	pTxRx = pSwiTxRx;

#if TX_INT_USED
	/* Tx interrupt */
	mode = swiInByte(pBase->regBaseAddr + TX_INT_STA_RO);
	if (mode & 0x01) {
		/* Tx interrupt clear */
		data = swiInByte(pBase->gt0StatAddr + 0x02) | 0x02;
		swiOutByte(pBase->gt0StatAddr + 0x02, data);

		addr = &(pTxRx->txBuf[0]);
		size = pTxRx->tx4BSize;
		Lkp502Write (0, addr, size);

		/* Tx interrupt set */
		data = swiInByte(pBase->gt0StatAddr + 0x02) & 0xFD;
		swiOutByte(pBase->gt0StatAddr + 0x02, data);
	}

	if (mode & 0x02) {
		/* Tx interrupt clear */
		data = swiInByte(pBase->gt1StatAddr + 0x02) | 0x02;
		swiOutByte(pBase->gt1StatAddr + 0x02, data);

		pTxRx ++;
		addr = &(pTxRx->txBuf[0]);
		size = pTxRx->tx4BSize;
		Lkp502Write (1, addr, size);
		
		/* Tx interrupt set */
		data = swiInByte(pBase->gt1StatAddr + 0x02) & 0xFD;
		swiOutByte(pBase->gt1StatAddr + 0x02, data);
	}
#endif	/* TX_INT_USED */

#if RX_INT_USED
	/* Rx interrupt */
	mode = swiInByte(pBase->regBaseAddr + RX_INT_STA_RO);
	if (mode & 0x01) {
		data = swiInByte(pBase->regBaseAddr + RX_INT_EN_RW);
		swiOutByte(pBase->regBaseAddr + RX_INT_EN_RW, data & 0xFE);
		addr = &(pTxRx->rxBuf[0]);
		size = pTxRx->rx4BSize;
		Lkp502Read (0, addr, size);
		sum = swiInLong(pBase->gt0StatAddr + 0x1C);
		if (sum != pTxRx->rxSum)	pTxRx->intRxErrCnt++;
		swiOutByte(pBase->regBaseAddr + RX_INT_EN_RW, data | 0x01);
	}

	if (mode & 0x02) {
		data = swiInByte(pBase->regBaseAddr + RX_INT_EN_RW);
		swiOutByte(pBase->regBaseAddr + RX_INT_EN_RW, data & 0xFD);
		pTxRx++;
		addr = &(pTxRx->rxBuf[0]);
		size = pTxRx->rx4BSize;
		Lkp502Read (1, addr, size);
		sum = swiInLong(pBase->gt1StatAddr + 0x1C);
		if (sum != pTxRx->rxSum)	pTxRx->intRxErrCnt++;
		swiOutByte(pBase->regBaseAddr + RX_INT_EN_RW, data | 0x02);
	}
#endif	/* RX_INT_USED */

	pTxRx->intLoopCnt ++;
}

/***
 * @brief  LKP-502 board PCI interrupt routine
 * @param  PCI interrupt control register address
 * @retval None.
 */
void Lkp502IntHandler (uint32 baseAddr)
{
	byte isrFlag, icrFlag;

	isrFlag = swiInByte(baseAddr + PMC_ISR_OFFSET) & PMC_INT_MASK;
	if (isrFlag) {
		/* PCI interrupt disable */
		icrFlag = swiInByte(baseAddr) & 0xFE;
		swiOutByte(baseAddr, icrFlag);

		Lkp502ProcInt ();

		/* PCI interrupt enable */
		swiOutByte(baseAddr, icrFlag | PMC_INT_MASK);
	}
}

/***
 * @brief  LKP-502 board interrupt initialization
 * @param  VxWorks VXB_DEVICE_ID handle
 * @param  SWI_BASE_REG *pBase
 * @param  interrupt number
 * @retval ERROR or OK.
 */
STATUS Lkp503InitInt (PCI_DEV vxbCtrlId, SWI_BASE_REG *pBaseAddr, uint32 intLine)
{
	uint32   intAddr;
	PCI_DEV  bCtrlId = NULL;
	SWI_BASE_REG *pBase = NULL;

	bCtrlId = vxbCtrlId;
	pBase = pBaseAddr;

	/* PCI bus interrupt initialize */
	intAddr = pBase->cfgBaseAddr + PMC_ICR_BASE;
	if (vxbPciIntConnect(bCtrlId, (VOIDFUNCPTR *)intLine, (VOIDFUNCPTR)Lkp502IntHandler, intAddr) == OK) {
		swiOutByte(intAddr, PMC_INT_MASK);
		if (vxbPciIntEnable(bCtrlId, (VOIDFUNCPTR *)intLine, (VOIDFUNCPTR)Lkp502IntHandler, intAddr, intLine) != OK)
			return ERROR;
		
#if TX_INT_USED
		/* Tx interrupt enable */
		Lkp502SetAllTxInt (TRUE);
#endif

#if RX_INT_USED
		/* Rx interrupt enable */
		Lkp502SetAllRxInt (TRUE);
#endif

		return OK;
	}

	return ERROR;
}
#endif	/* PMC_INT_USED */

/***
 * @brief  LKP-502 board PCI device configuration space read
 * @param  None.
 * @retval ERROR or OK.
 */
STATUS Lkp502PciShow (void)
{
	int				i, j;
	byte			*s_data = NULL;
	PCI_DEV			bCtrlId = NULL;
	SWI_PCI_INFO	*bPciInfo = NULL;
	
	if (pPmcInfo == NULL)	pPmcInfo = PmcInfo;
	bPciInfo = pPmcInfo;
	
	for (i = 0; i < PCI_DEV_NUM; i ++) {
		printf ("PCI information for LKP-502 No %d.\n", i+1);
		bCtrlId = bPciInfo->busCtrlId;
		printf ("[0x%08X, %d, %d, %d], [IRQ Num: %d]\n",
				(uint32)bCtrlId, bPciInfo->pciBus, bPciInfo->pciDevice,
				bPciInfo->pciFunc, bPciInfo->intLine);
		for (j = 0; j < PCI_DEV_MEM; j ++) {
			if ((j % 16) == 0)	printf ("%02x : ", j);
			if (vxbPciConfigInByte (bCtrlId, bPciInfo->pciBus, bPciInfo->pciDevice,
									bPciInfo->pciFunc, j, s_data) != OK) {
				printf ("\n[ERROR] PMC board PCI configuration space read error.\n");
				return ERROR;
			}
			printf ("%02x ", *s_data);
			if (((j + 1) % 16) == 0)	printf ("\n");
		}
		printf ("%s\n", lkp502lStr);
		bPciInfo ++;
	}

	return OK;
}

/***
 * @brief  LKP-502 board all register read
 * @param  None.
 * @retval ERROR or OK.
 */

STATUS Lkp502InfoShow (void)
{
	int      i;
	byte     data[3];

	if (pSwiBaseAddr == NULL)	pSwiBaseAddr = &SwiBaseAddr;

	data[0] = swiInByte(pSwiBaseAddr->regBaseAddr + MAJOR_VER_RO);
	data[1] = swiInByte(pSwiBaseAddr->regBaseAddr + MINOR_VER_RO);
	data[2] = swiInByte(pSwiBaseAddr->regBaseAddr + LOGIC_VER_RO);

	printf ("\n%s\n", lkp502lStr);
	printf ("    %s VxWorks Driver\n", SWIFT_BD_NAME);
	printf ("    SW Version   : %s\n", SWIFT_DRV_VER);
	printf ("    HW Version   : %d.%d.%d\n", data[0], data[1], data[2]);
	printf ("    Compile Date : %s, %s\n", __DATE__, __TIME__);
	printf ("%s\n", lkp502lStr);

	printf ("Status registers address : 0x%08x\n", pSwiBaseAddr->regBaseAddr);
	for (i = 0; i < 24; i ++) {
#if 0
		printf ("    Addr_Offset[%02x] data : 0x%08x\n", 4 * i,
				(uint32)swiInLong(pSwiBaseAddr->regBaseAddr + 4 * i));
#else
		if (!(i % 4)) {
			if (i != 0)		printf ("\n");
			printf ("    [0x%08X] ", (pSwiBaseAddr->regBaseAddr + (4 * i)));
		}
		printf ("%08X ", (uint32)swiInLong(pSwiBaseAddr->regBaseAddr + (4 * i)));
#endif
	}
	printf("\n%s\n", lkp502lStr);

	data[0] = swiInByte(pSwiBaseAddr->gt0StatAddr + 0x14) & 0x03;
	printf ("GTX port #%d Tx addr : 0x%08X, Rx Addr : 0x%08X\n",
			data[0], pSwiBaseAddr->gt0BaseAddr, pSwiBaseAddr->gt0BaseAddr + RX_ADD_OFFSET);
	printf ("GTX port #%d stat address : 0x%08x\n", data[0], pSwiBaseAddr->gt0StatAddr);
	for (i = 0; i < 8; i ++) {
#if 0
		printf("    Addr_Offset[%02x] data : 0x%08x\n", 4 * i,
				(uint32)swiInLong(pSwiBaseAddr->gt0StatAddr + (4 * i)));
#else
		if (!(i % 4)) {
			if (i != 0) 	printf ("\n");
			printf ("	 [0x%08X] ", (pSwiBaseAddr->gt0StatAddr + (4 * i)));
		}
		printf ("%08X ", (uint32)swiInLong(pSwiBaseAddr->gt0StatAddr + (4 * i)));
#endif
	}
	printf ("\n%s\n", lkp502lStr);

	data[0] = swiInByte(pSwiBaseAddr->gt1StatAddr + 0x14) & 0x03;
	printf ("GTX port #%d Tx addr : 0x%08X, Rx Addr : 0x%08X\n",
			data[0], pSwiBaseAddr->gt1BaseAddr, pSwiBaseAddr->gt1BaseAddr + RX_ADD_OFFSET);
	printf("GTX port #%d stat address : 0x%08x\n", data[0], pSwiBaseAddr->gt1StatAddr);
	for (i = 0; i < 8; i ++) {
#if 0
		printf("    Addr_Offset[%02x] data : 0x%08x\n", 4 * i,
				(uint32)swiInLong(pSwiBaseAddr->gt1StatAddr + (4 * i)));
#else
		if (!(i % 4)) {
			if (i != 0) 	printf ("\n");
			printf ("	 [0x%08X] ", (pSwiBaseAddr->gt1StatAddr + (4 * i)));
		}
		printf ("%08X ", (uint32)swiInLong(pSwiBaseAddr->gt1StatAddr + (4 * i)));

#endif
	}
	printf("\n%s\n", lkp502lStr);

	/* Show PCI information */
	Lkp502PciShow ();
	
	return OK;
}

/***
 * @brief  LKP-502 board PCI bus initialization
 * @param  PCI bus information
 * @param  Board register address
 * @retval ERROR or OK.
 */
LOCAL STATUS sysLkp502PciInit (void)
{
	int      i;
	int      pciBus, pciDevice, pciFunc;
	byte     classCode, intPin, intLine;
	uint32   baseAddr;
	PCI_DEV  bCtrlId = NULL;
	SWI_PCI_INFO	*pPmc = NULL;
	SWI_BASE_REG	*pBaseAddr = NULL;

	bCtrlId = vxbInstByNameFind (PCI_INST_NAME, DRV_PMC_BUS_NO);
	if (bCtrlId == NULL) {
		printf ("\n[ERROR] PCI device [%s] not found.\n", PCI_INST_NAME);
		return ERROR;
	}

	if (vxbPciFindDevice (bCtrlId, VENDOR_ID, SWI_2_DEV_ID, 0, &pciBus, &pciDevice, &pciFunc) != OK) {
		printf ("\n[ERROR] PMC board is not detected.\n");
		return ERROR;
	}

	pPmc = pPmcInfo;
	pBaseAddr = pSwiBaseAddr;
	for (i = 0; i < PCI_ADD_NUM; i ++) {
		if (vxbPciConfigInLong (bCtrlId, pciBus, pciDevice, pciFunc,
						PCI_CFG_BASE_ADDRESS_0 + (i * 4), &baseAddr) != OK) {
			printf ("\n[ERROR] PMC board initialization failed.\n");
			return ERROR;
		}
		pPmc->baseAddr[i] = baseAddr & 0xFFFFFFF0;
		pPmc->memMask[i]  = baseAddr & 0x01;
	}

	pBaseAddr->cfgBaseAddr = pPmc->baseAddr[0];
	pBaseAddr->regBaseAddr = pPmc->baseAddr[1];
	pBaseAddr->gt0BaseAddr = pPmc->baseAddr[2];
	pBaseAddr->gt1BaseAddr = pPmc->baseAddr[2] + GTX_PORT_OFFSET;
		
	pBaseAddr->gt0StatAddr = pBaseAddr->gt0BaseAddr + PORT_STAT_REG_ADDR;
	pBaseAddr->gt1StatAddr = pBaseAddr->gt1BaseAddr + PORT_STAT_REG_ADDR;

	(void)vxbPciConfigInByte (bCtrlId, pciBus, pciDevice, pciFunc, PCI_CFG_CLASS, &classCode);
	(void)vxbPciConfigInByte (bCtrlId, pciBus, pciDevice, pciFunc, PCI_CFG_DEV_INT_LINE, &intLine);
	(void)vxbPciConfigInByte (bCtrlId, pciBus, pciDevice, pciFunc, PCI_CFG_DEV_INT_PIN, &intPin);
	
	pPmc->busCtrlId = bCtrlId;
	pPmc->pciBus    = pciBus;
	pPmc->pciDevice = pciDevice;
	pPmc->pciFunc   = pciFunc;
	pPmc->intLine   = intLine;
	pPmc->intPin    = intPin;
	pPmc->intFlag   = PMC_INT_MASK;
	pPmc->classCode = classCode;
	
#if PMC_INT_USED
	/* PCI bus interrupt initialize */
	if (Lkp503InitInt (bCtrlId, pBaseAddr, (uint32)intLine) != OK) {
		printf("LKP-502 board interrupt #%d initialization failed !\n", intLine);
		return ERROR;
	}
#endif	/* PMC_INT_USED */

	return OK;
}

/***
 * @brief  Initialize LKP-502 board for PCI Bus
 * @param  None.
 * @retval ERROR or OK.
 */
LOCAL STATUS sysLkp502Init (void)
{
	int		i;
	
	/* PCI device information */
	pPmcInfo = PmcInfo;
	(void)memset((void *)pPmcInfo, 0, (sizeof(SWI_PCI_INFO) * PCI_DEV_NUM));

	/* LKP-502 register information */
	pSwiBaseAddr = &SwiBaseAddr;
	(void)memset((void *)pSwiBaseAddr, 0, sizeof(SWI_BASE_REG));

	/* Initialize PCI bus informatoin for LKP-502 */
	if (sysLkp502PciInit () != OK)
		return ERROR;

#if PMC_INT_USED
	pSwiTxRx = SwiTxRxBuf;
	for (i = 0; i < GTX_CH_NUM; i ++) {
		(void)memset((void *)pSwiTxRx, 0, sizeof(SWI_GTX_TXRX));
		pSwiTxRx->tx4BSize = Lkp502SetTxSize (i, TX_1B_MAX_SIZE);
		pSwiTxRx->rx4BSize = pSwiTxRx->tx4BSize;
		pSwiTxRx ++;
	}
	pSwiTxRx = SwiTxRxBuf;
#endif	/* PMC_INT_USED */

#if 0
	swiGtPort[0].pTxSpace   = (uint32 *) pSwiBaseAddr->gt0BaseAddr;
	swiGtPort[0].pRxSpace   = (uint32 *)(pSwiBaseAddr->gt0BaseAddr + RX_ADD_OFFSET);
	swiGtPort[0].pRwStatReg = (uint32 *) pSwiBaseAddr->gt0StatAddr;
	swiGtPort[0].pRoStatReg = (uint32 *)(pSwiBaseAddr->gt0StatAddr + 0x14);

	swiGtPort[1].pTxSpace   = (uint32 *) pSwiBaseAddr->gt1BaseAddr;
	swiGtPort[1].pRxSpace   = (uint32 *)(pSwiBaseAddr->gt1BaseAddr + RX_ADD_OFFSET);
	swiGtPort[1].pRwStatReg = (uint32 *) pSwiBaseAddr->gt1StatAddr;
	swiGtPort[1].pRoStatReg = (uint32 *)(pSwiBaseAddr->gt1StatAddr + 0x14);
#else
	swiGtPort[0].pTxSpace   = pSwiBaseAddr->gt0BaseAddr;
	swiGtPort[0].pRxSpace   = pSwiBaseAddr->gt0BaseAddr + RX_ADD_OFFSET;
	swiGtPort[0].pRwStatReg = pSwiBaseAddr->gt0StatAddr;
	swiGtPort[0].pRoStatReg = pSwiBaseAddr->gt0StatAddr + 0x14;

	swiGtPort[1].pTxSpace   = pSwiBaseAddr->gt1BaseAddr;
	swiGtPort[1].pRxSpace   = pSwiBaseAddr->gt1BaseAddr + RX_ADD_OFFSET;
	swiGtPort[1].pRwStatReg = pSwiBaseAddr->gt1StatAddr;
	swiGtPort[1].pRoStatReg = pSwiBaseAddr->gt1StatAddr + 0x14;
#endif

	pGtPort = swiGtPort;

	return OK;
}

/***
 * @brief  LKP-502 PMC board init
 * @param  None.
 * @retval ERROR or OK.
 */
STATUS Lkp502Init (void)
{
	if (initCnt > 1)	return OK;

	if (sysLkp502Init () != OK)
		return ERROR;

	initCnt ++;
	return OK;
}
/* end of file */

