/**************************************************************************
** Zone3_64_GOV(Ground Over Voltage)(FB716)
**************************************************************************/
#ifndef FCZ3_64_GOV_H
#define FCZ3_64_GOV_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ3_64_GOVInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ3_64_GOVRunFunc(uint32, uint32);

/*********************************************************************
** Zone3_64GOV(Ground Over Voltage) FB	
**********************************************************************/
#define	FC0716_INIT				0
#define	FC0716_RUN				1

#define	FC0716_CODE				716
#define	FC0716_SPEC_NUM			5
#define	FC0716_VAR_NUM			1	
#define	FC0716_OUTPUT_NUM		1

#define FAULT					1
#define NORMAL					0

//FB Variable 
typedef struct {
    // Input
    int32 					enable;				// Logic Enable
	int32 					reset;				// Reset
	float					vg;					// Vg
	float					threshold;			// Threshold 
	float					setT;				// Vg Threshold1 Time Set 
	
    // Inner variable
	float					t1;  				// Time 1 
				
    // Output
    uint32		  			o1_alarm;			// Out1 Alarm
} strFC0716Info;

#endif
