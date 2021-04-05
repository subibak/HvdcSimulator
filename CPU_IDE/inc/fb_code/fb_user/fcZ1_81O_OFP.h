/**************************************************************************
** Zone1_81O_OFP(Over Frequency Protection)(FB0704)
**************************************************************************/
#ifndef FCZ1_81O_OFP_H
#define FCZ1_81O_OFP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ1_81O_OFPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ1_81O_OFPRunFunc(uint32, uint32);

/*********************************************************************
**	Zone1_81O_OFP(Over Frequency Protection) FB 	
**********************************************************************/
#define	FC0704_INIT				0
#define	FC0704_RUN				1

#define	FC0704_CODE				704
#define	FC0704_SPEC_NUM			7
#define	FC0704_VAR_NUM			2	
#define	FC0704_OUTPUT_NUM		2


//FB Variable 
typedef struct {
    // Input
    int32 					enable;				// Logic Enable
	int32 					reset;				// Reset
	float 		 			freq;				// Frequency
	float					threshold1;			// Threshold1
	float					threshold2;			// Threshold2
	
	float					setT1;				// Threshold1 Time Set
	float					setT2;				// Threshold2 Time Set
		
    // Inner variable
	float					t1;  				// Time 1 
	float					t2;  				// Time 2
		
    // Output
    float		 			o1_alarm;			// Out1 Alarm	
    float 		 			o2_conBlk;			// Out2 conBlk
} strFC0704Info;

#endif
