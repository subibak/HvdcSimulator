
/**
 * Core.c
 * Core Module Implementation
 */

#include "Core.h"

void SetState(CORE_STATE *ePrevState, CORE_STATE *eState, CORE_STATE eNextState)
{
	*ePrevState = *eState;
	*eState = eNextState;
}


//--------------------------------------------------------------------------//
// Function:	Delay
//
// Parameters:	ulMS - number of miliseconds to sleep
//
// Return:		None
//
// Description:	Delay for a fixed number of Ms, blocks
//--------------------------------------------------------------------------//
volatile void DelayUs(const uint32_t ulUs)
{

    int i = 0;
    
    for(i=0;i < ulUs;i++)
	{
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
//		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
//		asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");asm("nop;nop;nop;nop;;");
	}

}

