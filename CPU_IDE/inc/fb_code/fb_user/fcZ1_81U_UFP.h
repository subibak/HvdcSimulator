/**************************************************************************
** Zone1_81U_UFP(Under Frequency Protection)(FB0705)
**************************************************************************/
#ifndef FCZ1_81U_UFP_H
#define FCZ1_81U_UFP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ1_81U_UFPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ1_81U_UFPRunFunc(uint32, uint32);

/*********************************************************************
**	Zone1_81U_UFP(Under Frequency Protection) FB 	
**********************************************************************/
#define	FC0705_INIT				0
#define	FC0705_RUN				1

#define	FC0705_CODE				705
#define	FC0705_SPEC_NUM			7
#define	FC0705_VAR_NUM			2	
#define	FC0705_OUTPUT_NUM		2

//FB Variable 
typedef struct {
    // Input
    int32 					enable;				// Logic Enable
	int32 					reset;				// Reset
	float 		 			freq;				// Frequency
	float					threshold1;			// Frequency Threshold1
	float					threshold2;			// Frequency Threshold2
	
	float					setT1;				// Frequency Threshold1 Time Set
	float					setT2;				// Frequency Threshold2 Time Set
		
    // Inner variable
	float					t1;  				// Time 1 
	float					t2;  				// Time 2
		
    // Output
    uint32		 			o1_alarm;			// Out1 Alarm	
    uint32 		 			o2_conBlk;			// Out2 conBlk
} strFC0705Info;

#endif
