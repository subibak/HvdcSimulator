
/**
 * HalTimer.c
 * HalTimer Device Driver Module Body
 */

#include "HalTimer.h"

void HalTimerInit(void (*func)(int))
{
	__builtin_sysreg_write(__TMRIN0H, 0);
	__builtin_sysreg_write(__TMRIN0L, 0);	
	HalTimerIntInit(func);
}

void HalTimerConfigure(uint32_t ulPeriod)
{
	__builtin_sysreg_write(__TMRIN0H, 0);	
	__builtin_sysreg_write(__TMRIN0L, ulPeriod);	
}

void HalTimerIntInit(void (*func)(int))
{
	volatile int temp;

	interruptf(SIGTIMER0LP, func);
	
	temp = __builtin_sysreg_read(__IMASKL);
	temp = temp | INT_TIMER0L;
	__builtin_sysreg_write(__IMASKL, temp);

}

void HalTimerReStart(uint32_t ulPeriod)
{

	HalTimerStop();
	HalTimerStart(ulPeriod);
#if 0	
	volatile int temp;

	temp = __builtin_sysreg_read(__INTCTL);
	temp = temp & (~INTCTL_TMR0RN);	/* Timer0 run */
	__builtin_sysreg_write(__INTCTL, temp);

	__builtin_sysreg_write(__TMRIN0L, 0);	
	__builtin_sysreg_write(__TMRIN0L, ulPeriod);	

	temp = __builtin_sysreg_read(__INTCTL);
	temp = temp | INTCTL_TMR0RN;	/* Timer0 run */
	__builtin_sysreg_write(__INTCTL, temp);
#endif
}

void HalTimerStart(uint32_t ulPeriod)
{

	volatile int temp;

	HalTimerConfigure(ulPeriod);

	temp = __builtin_sysreg_read(__INTCTL);
	temp = temp | INTCTL_TMR0RN;	/* Timer0 run */
	__builtin_sysreg_write(__INTCTL, temp);

}

void HalTimerStop(void)
{

	volatile int temp;

	temp = __builtin_sysreg_read(__INTCTL);
	temp = temp & ~INTCTL_TMR0RN;	/* Timer0 run */
	__builtin_sysreg_write(__INTCTL, temp);

}

