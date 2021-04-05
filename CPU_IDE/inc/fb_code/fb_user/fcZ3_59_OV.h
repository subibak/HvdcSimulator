/**************************************************************************
** Zone3_59_OV(Over Voltage)(FB715)
**************************************************************************/
#ifndef FCZ3_59_OV_H
#define FCZ3_59_OV_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ3_59_OVInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ3_59_OVRunFunc(uint32, uint32);

/*********************************************************************
**	Zone3_59_OV(Over Voltage) FB 	
**********************************************************************/
#define	FC0715_INIT				0
#define	FC0715_RUN				1

#define	FC0715_CODE				715
#define	FC0715_SPEC_NUM			9
#define	FC0715_VAR_NUM			6	
#define	FC0715_OUTPUT_NUM		6

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
    STU_3P  		o1_conBlk;			// Out1 Converter Block
    STU_3P			o2_emst;			// Out2 EMST
} strFC0715Info;

#endif
