/**************************************************************************
** Zone2_50/51_TOCP(Transformer OverCurrent Protection)(FB0708)
**************************************************************************/
#ifndef FCZ2_5051_TOCP_H
#define FCZ2_5051_TOCP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ2_5051_TOCPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ2_5051_TOCPRunFunc(uint32, uint32);

/*********************************************************************
**	Zone2_50/51_TOCP(Transformer OverCurrent Protection) FB  	
**********************************************************************/
#define	FC0708_INIT				0
#define	FC0708_RUN				1

#define	FC0708_CODE				708
#define	FC0708_SPEC_NUM			8
#define	FC0708_VAR_NUM			6	
#define	FC0708_OUTPUT_NUM		6

//FB Variable 
typedef struct {
    // Input
    int32 			enable;				// Logic Enable
	int32 			reset;				// Reset
	STF_3P 			itr_1st;			// ipcc
	
	float			threshold1;			// Threshold1
	float			threshold2;			// Threshold2
	float			setT;				// Threshold1,2 Time Set
	
    // Inner variable
	STF_3P 			t1;  				// Time 1 
	STF_3P 			t2;  				// Time 2 
		
    // Output
    STU_3P  		o1_alarm;			// Out1 Alarm
    STU_3P			o2_conBlk;			// Out2 conBlk
} strFC0708Info;

#endif
