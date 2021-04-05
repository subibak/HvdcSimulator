/**************************************************************************
** Zone1_59_ABOV(154kV AC Bus OV)(FB0702)
**************************************************************************/
#ifndef FCZ1_59_ABOV_H
#define FCZ1_59_ABOV_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ1_59_ABOVInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ1_59_ABOVRunFunc(uint32, uint32);

/*********************************************************************
**	Zone1_59_ABOV(154kV AC Bus OV) FB 	
**********************************************************************/
#define	FC0702_INIT				0
#define	FC0702_RUN				1

#define	FC0702_CODE				702
#define	FC0702_SPEC_NUM			9
#define	FC0702_VAR_NUM			6	
#define	FC0702_OUTPUT_NUM		6


//FB Variable 
typedef struct {
    // Input
    int32 			enable;				// Logic Enable
	int32 			reset;				// Reset
	STF_3P 			vrms;				// vrms
	float			threshold1;			// Threshold1
	float			threshold2;			// Threshold2
	float			setT1;				// Threshold1 Time Set
	float			setT2;				// Threshold2 Time Set

    // Inner variable
	STF_3P 			t1;  				// Time 1 
	STF_3P 			t2;  				// Time 2 
		
    // Output
    STU_3P			o1_alarm;			// Out1 Alarm
    STU_3P  		o2_nmst;			// Out2 NMST
} strFC0702Info;

#endif
