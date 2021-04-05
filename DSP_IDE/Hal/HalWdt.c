/**
 * HalWdt.c
 * WatchDog 
 */

#include "HwDef/Wdt.h"
#include "HalWdt.h"


// Board 기능 재정의 필요함.





void HalWdtReboot(void)
{
	HalWdtDisable();

	HalWdtEnable();
	
	FOREVER;
}


void HalWdtEnable(void)
{
	uint32_t	ulReg;

	ReadReg(HWREG_WDT_STAT_ADDR, &ulReg);
	BitSet(ulReg, WDT_ENABLE);
	WriteReg(HWREG_WDT_STAT_ADDR, ulReg);
}

void HalWdtDisable(void)
{
	uint32_t	ulReg;

	ReadReg(HWREG_WDT_STAT_ADDR, &ulReg);
	BitClear(ulReg, HWREG_WDT_STAT_ENABLE_MASK);
	WriteReg(HWREG_WDT_STAT_ADDR, ulReg);
}

#if 0

#define	WDT_BASE_MSEC		10	// 10.24msec

uint32_t HalWdtSetTime(int32_t base)
{
	uint32_t	set_val;

	if (base > 10000)	base = 10000;	// max. 10sec
	set_val = (100 * base) << 8;		// 1.024msec step

	return set_val;
}

void wdt_enable(int wdt_msec)
{
	uint	data;
	ADDRESS	addr;
	
	wdt_settime(wdt_msec);
	wdtEnable = TRUE;
	addr = (ADDRESS)WDT_STAT_REG_RW;

	data = *addr;
	data |= WDT_ENABLE;
	*addr = data; 
}


void
HalWdtSetTime(void)
{
	uint	data;
	ADDRESS	addr;

	if (wdtEnable == FALSE)	return;
	
	addr = (ADDRESS)WDT_CTRL_REG_RW;
	data = *addr;

	if (data & WDT_TOGGLE)	data &= ~WDT_TOGGLE;
	else					data |= WDT_TOGGLE;
	
	*addr = data;
}


void
wdt_test(int argc, char **argv)
{
	if (argc == 2) {
		if (strcmp(*(argv+1), "on") == OK)			wdt_enable(WDT_BASE_MSEC);
		else if (strcmp(*(argv+1), "off") == OK)	wdt_disable();
	}
	PRINTF("watch dog : ");
	if (wdtEnable)	PRINTF("ON\n");
	else			PRINTF("OFF\n");
}


/***
 * Power Restart
 * Keep a fatal error, this function only !!!!!!!!!!!!!!!!!!!!
 */
#if 0
void
pwr_restart(void)
{
	/* Power Restart */
	ADDRESS addr;
	addr = (ADDRESS)DSPB_CPLD_REG_RW;
	*addr |= XCB_CPLD_0;
	*addr &= ~XCB_CPLD_1;

	addr = (ADDRESS)DSPA_CPLD_REG_RW;
	*addr |= XCA_CPLD_0;

	FOREVER;
}
#endif

void
reboot(int time, int pwr_mode)
{
	int i;
	
	if ((time < 0) || (time > 10))	time = 10;

	if (pwr_mode != FALSE)		PRINTF("POWER RESTART ... ");
	else						PRINTF("REBOOT ... ");
	
	wdt_disable();

	for (i = time; i > 0; i--) {
		PRINTF("%2d.", i);
		delay_msec(1000);
	}
	PRINTF("\n");

#if 0	// YET
	if (pwr_mode != FALSE) {
		pwr_restart();
		return;
	}
#endif

#if (TIMER_INCLUDE_WDTS == YES)
	reboot_proc_flag = TRUE;
#endif	// (TIMER_INCLUDE_WDTS == YES)
	wdt_enable(WDT_BASE_MSEC);
	
	FOREVER;
}

void
reboot_cmd(int argc, char **argv)
{
	int		pwr_mode = FALSE;
	int		time = 1, i;
	char	*cp;

	for (i = 1; i < argc; i++) {
		cp = *(argv+i);
		if (strcmp(cp, "pwr") == OK)		pwr_mode = TRUE;
		else if (*cp >= '0' && *cp <= '9')	time = str2int(cp);
	}

	reboot(time, pwr_mode);
}


#endif


