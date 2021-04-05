
/**
 * HalTS201.c
 * HalTS201 Device Driver Module Body
 */

asm("#include <HwDef/cache_macros.h>");
asm("#include <defts201.h>");

#include <sysreg.h>
#include <defts201.h>

#include "HalTS201.h"



void InitDmaInt(void)
{
	volatile __builtin_quad tcb_clr;

	// disable and clear linkports
	__builtin_sysreg_write(__LTCTL0, 0x0);
	__builtin_sysreg_write(__LTCTL1, 0x0);
	__builtin_sysreg_write(__LTCTL2, 0x0);
	__builtin_sysreg_write(__LTCTL3, 0x0);

	__builtin_sysreg_write(__LRCTL0, 0x0);
	__builtin_sysreg_write(__LRCTL1, 0x0);
	__builtin_sysreg_write(__LRCTL2, 0x0);
	__builtin_sysreg_write(__LRCTL3, 0x0);

	// DMA TCB Register Clear
	tcb_clr = __builtin_compose_128((long long)TCB_DISABLE << 32, (long long)TCB_DISABLE << 32);
	__builtin_sysreg_write4(__DC4, tcb_clr);
	__builtin_sysreg_write4(__DC5, tcb_clr);
	__builtin_sysreg_write4(__DC6, tcb_clr);
	__builtin_sysreg_write4(__DC7, tcb_clr);
	__builtin_sysreg_write4(__DC8, tcb_clr);
	__builtin_sysreg_write4(__DC9, tcb_clr);
	__builtin_sysreg_write4(__DC10, tcb_clr);
	__builtin_sysreg_write4(__DC11, tcb_clr);
}


void SystemHealth(void)
{
	uint32_t	data, cnt=0, mode = 0;
	uint32_t ulReg, ulReg2;

	CLI();
	ReadReg(XCA_VEND_ID_RO, &ulReg);
	if (ulReg != 0x4C4B3131) {
		Reboot(0, 1);
		FOREVER;
	}
	
	ReadReg(XCB_VEND_ID_RO, &ulReg);
	if (ulReg != 0x4C4B3131) {
		Reboot(0, 1);
		FOREVER;
	}

	/* 0x03 */
	ReadReg(XCA_SYS_CLK_RO, &ulReg);

	/* 0x07 */
	ReadReg(XCB_SYS_CLK_RO, &ulReg2);
	ulReg = ulReg + ulReg2;


	if (ulReg != 10)	mode = 1; 

	while (mode) {
		DelayUs(1000);

		ReadReg(XCA_SYS_CLK_RO, &ulReg);

		/* 0x07 */
		ReadReg(XCB_SYS_CLK_RO, &ulReg2);
		ulReg = ulReg + ulReg2;


		if (ulReg == 10)	mode = 0;

		cnt++;
		if (cnt >= 1000) {
			Reboot(0, 1);
			FOREVER;
		}
	}
	STI();

}

void Reboot(int time, int pwr_mode)
{
	int i;
	
	
	if ((time < 0) || (time > 10))	time = 10;

	WdtDisable();

	for (i = time; i > 0; i--) {
		DelayUs(1000000);
//		delay_msec(1000);
	}

	WdtEnable();
	
	FOREVER;

}


void WdtEnable(void)
{
	uint32_t ulReg;

	ReadReg(XCA_WDT_STAT_REG_ADDR, &ulReg);
	BitSet(ulReg, WDT_ENABLE);
	WriteReg(XCA_WDT_STAT_REG_ADDR, ulReg);

}

void WdtDisable(void)
{
	uint32_t ulReg;

	ReadReg(XCA_WDT_STAT_REG_ADDR, &ulReg);
	BitClear(ulReg, WDT_ENABLE);
	WriteReg(XCA_WDT_STAT_REG_ADDR, ulReg);

}

void HalTS0201MPEnable(void)
{
	CLI();
	__builtin_sysreg_write(__BMAX, 50); 
	STI();
}

void InitTS201(void)
{


	CLI();
#if 1
	/* System Control Register */
	__builtin_sysreg_write(__SYSCON, SYSCON_MP_WID64	|	\
	                                 SYSCON_MEM_WID64	|	\

	                                 SYSCON_MSH_SLOW	|	\
									 SYSCON_MSH_WT3		|	\
									 SYSCON_MSH_IDLE	|	\
		                             
									 SYSCON_MS1_SLOW	|	\
									 SYSCON_MS1_WT3		|	\
									 SYSCON_MS1_IDLE	|	\
																	 
									 SYSCON_MS0_SLOW	|	\
									 SYSCON_MS0_WT2		|	\
									 SYSCON_MS0_IDLE);

	/* SDRAM Control Register Disable */
	__builtin_sysreg_write(__SDRCON, SDRAM_DISABLE);
	__builtin_sysreg_write(__BMAX, 50); 
//	__builtin_sysreg_write(__BMAX, 150); 
	__builtin_sysreg_write(__FLAGREG, 0x0f);	/* all output */	
	
#else
	__builtin_sysreg_write(__SYSCON, SYSCON_MP_WID64 |       
                                     SYSCON_MEM_WID64 |
	                                 SYSCON_MSH_PIPE2 |
                                     SYSCON_MSH_WT0 |  
                                     SYSCON_MSH_IDLE | 
		                             SYSCON_MS1_PIPE1 | 
                                     SYSCON_MS1_WT0 |  
                                     SYSCON_MS1_IDLE | 
		                             SYSCON_MS0_SLOW  | 
                                     SYSCON_MS0_WT3 |
                                     SYSCON_MS0_IDLE); 

	__builtin_sysreg_write(__SDRCON, 0);
	/* System Control Register */
	__builtin_sysreg_write(__SYSCON, SYSCON_MSH_SLOW	|
									 SYSCON_MSH_WT3		|
									 SYSCON_MSH_IDLE	|
		                             
									 SYSCON_MS1_SLOW	|
									 SYSCON_MS1_WT3		|
									 SYSCON_MS1_IDLE	|
																	 
									 SYSCON_MS0_SLOW	|
									 SYSCON_MS0_WT2		|
									 SYSCON_MS0_IDLE);

	/* SDRAM Control Register Disable */
	__builtin_sysreg_write(__SDRCON, SDRAM_DISABLE);
    /* Flag Control Register */
	__builtin_sysreg_write(__FLAGREG, (FLAG1_OUTPUT | FLAG2_OUTPUT | FLAG3_OUTPUT | FLAG4_OUTPUT));	/* all output */	
	asm("cache_enable(900);");				// Enable cache for NCS1000
#endif


	InitDmaInt();
	
	STI();


	/* 	for TS201, at the beginning of the program the
   		cache must be enabled. The procedure is contained in the
		cache_enable macro that uses the refresh rate as input parameter
		-if CCLK=500MHz, refresh_rate=750
		-if CCLK=400MHz, refresh_rate=600
		-if CCLK=300MHz, refresh_rate=450
		-if CCLK=250MHz, refresh_rate=375 */
}



/****
 * Description:	Clear global interrupts
 */
void CLI(void)
{
	volatile unsigned long temp;
	
	temp = __builtin_sysreg_read(__SQCTL);
	temp &= ~(SQCTL_GIE | SQCTL_NMOD);
	__builtin_sysreg_write(__SQCTL, temp);
}

/****
 * Description:	Set global interrupts
 */
void STI(void)
{
	volatile unsigned long temp;
	
	temp = __builtin_sysreg_read(__SQCTL);
    temp |= (SQCTL_GIE | SQCTL_NMOD);
    __builtin_sysreg_write(__SQCTL, temp);
}


void HalTS201ExtInt(void)
{
	volatile int temp;

	/* Level Sensitive = 1, Edge Sensitive = 0 */
	temp = __builtin_sysreg_read(__INTCTL);
#if 1	// CMS_YET : 왜 변경되었나?????
	temp = temp | INTCTL_IRQ0_EDGE | INTCTL_IRQ1_EDGE | INTCTL_IRQ3_EDGE;	// IRQ0, IRQ1, IRQ3
#else	// VER1.0
	temp = temp | 0x0B;	/* IRQ3 */
#endif	// CMS_YET
	__builtin_sysreg_write(__INTCTL, temp);

	/* Interrupt MASK */
	temp = __builtin_sysreg_read(__IMASKH);
	temp &= ~INT_IRQ2;	// LPI : ???
	temp = temp | INT_IRQ0 | INT_IRQ1 | INT_IRQ3;	/* IRQ0, 1, 3 */

	__builtin_sysreg_write(__IMASKH, temp);

}


