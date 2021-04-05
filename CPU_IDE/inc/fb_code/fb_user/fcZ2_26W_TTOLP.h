/**************************************************************************
** Zone2_26W_TTOLP(Transformer Thermal OverLoad Protection)(FB0711)
**************************************************************************/
#ifndef FCZ2_26W_TTOLP_H
#define FCZ2_26W_TTOLP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ2_26W_TTOLPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ2_26W_TTOLPRunFunc(uint32, uint32);

/*********************************************************************
**	Zone2_26W_TTOLP(Transformer Thermal OverLoad Protection) FB 	
**********************************************************************/
#define	FC0711_INIT				0
#define	FC0711_RUN				1

#define	FC0711_CODE				711
#define	FC0711_SPEC_NUM			9
#define	FC0711_VAR_NUM			6	
#define	FC0711_OUTPUT_NUM		6

//FB Variable 
typedef struct {
    // Input
    int32 			enable;				// Logic Enable
	int32 			reset;				// Reset
	STF_3P 			ittr;				// Tr thermal Overload  
	float			threshold1;			// Threshold 1
	float			threshold2;			// Threshold 2
	float			setT1;				// Threshold Time Set 1
	float			setT2;				// Threshold Time Set 2
	
    // Inner variable
	STF_3P  		t1;  				// Time 1 
	STF_3P  		t2;  				// Time 2 
		
    // Output
    STU_3P			o1_alarm;			// Out1 Alarm
    STU_3P   		o2_trip;			// Out2 Trip
} strFC0711Info;

#endif
