/**************************************************************************
** Zone3_27_UV(Under Voltage)(FB714)
**************************************************************************/
#ifndef FCZ3_27_UV_H
#define FCZ3_27_UV_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ3_27_UVInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ3_27_UVRunFunc(uint32, uint32);

/*********************************************************************
**	Zone3_27UV(Under Voltage) FB 	
**********************************************************************/
#define	FC0714_INIT				0
#define	FC0714_RUN				1

#define	FC0714_CODE				714
#define	FC0714_SPEC_NUM			9
#define	FC0714_VAR_NUM			6	
#define	FC0714_OUTPUT_NUM		6


//FB Variable 
typedef struct {
    // Input
    int32 			enable;				// Logic Enable
	int32 			reset;				// Reset
	STF_3P			v2nd;				// V2nd
	float			threshold1;			// Threshold 1
	float			threshold2;			// Threshold 2
	float			setT1;				// Threshold1 Time Set 1
	float			setT2;				// Threshold1 Time Set 2
	
    // Inner variable
	STF_3P 			t1;  				// Time 1 
	STF_3P 			t2;  				// Time 2 
		
    // Output
    STU_3P			o1_alarm;			// Out1 Alarm
    STU_3P 			o2_conBlk;			// Out2 Converter Block

} strFC0714Info;

#endif
