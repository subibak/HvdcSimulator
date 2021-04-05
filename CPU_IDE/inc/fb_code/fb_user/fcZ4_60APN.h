/**************************************************************************
** Zone4_60APN(FB731)
**************************************************************************/
#ifndef FCZ4_60APN_H
#define FCZ4_60APN_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ4_60APNInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ4_60APNRunFunc(uint32, uint32);

/*********************************************************************
** Zone4_60APN FB 	
**********************************************************************/
#define	FC0731_INIT				0
#define	FC0731_RUN				1

#define	FC0731_CODE				731
#define	FC0731_SPEC_NUM			12
#define	FC0731_VAR_NUM			2	
#define	FC0731_OUTPUT_NUM		2

//FB Variable 
typedef struct {
    // Input
    int32 			enable;				// Logic Enable
	int32 			reset;				// Reset
	STF_3P			iarm_rms_up;		// iarm rms up
	STF_3P			iarm_rms_low; 		// iarm rms low
	float			threshold1;			// Threshold 1
	float			threshold2;			// Threshold 2
	float			setT1;				// Threshold1 Time Set 1
	float			setT2;				// Threshold1 Time Set 2
		
    // Inner variable
	float			t1; 				// Time 1
	float			t2; 				// Time 2
		
    // Output
    float  			o1_alarm;			// Out1 alarm
    float  			o2_conBlk;			// Out2 conBlk
} strFC0731Info;

#endif
