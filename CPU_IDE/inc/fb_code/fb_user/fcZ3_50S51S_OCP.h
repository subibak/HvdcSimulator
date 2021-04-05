/**************************************************************************
** Zone3_50S51S_OCP(Over Current Protection)(FB717)
**************************************************************************/
#ifndef FCZ3_50S51S_OCP_H
#define FCZ3_50S51S_OCP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ3_50S51S_OCPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ3_50S51S_OCPRunFunc(uint32, uint32);

/*********************************************************************
**	Zone3_50S51S_OCP(Over Current Protection) FB 	
**********************************************************************/
#define	FC0717_INIT				0
#define	FC0717_RUN				1

#define	FC0717_CODE				717
#define	FC0717_SPEC_NUM			8
#define	FC0717_VAR_NUM			6	
#define	FC0717_OUTPUT_NUM		6

//FB Variable 
typedef struct {
    // Input
    int32 			enable;				// Logic Enable
	int32 			reset;				// Reset
	STF_3P			itr_2nd;			// V2nd
	float			threshold1;			// Threshold 1
	float			threshold2;			// Threshold 2
	float			setT;				// Threshold1 & 2 Time Set 1
		
    // Inner variable
	STF_3P 			t1;  				// Time 1 
	STF_3P 			t2;  				// Time 2 
		
    // Output
    STU_3P  		o1_emst;			// Out1 EMST
    STU_3P			o2_emst;			// Out2 EMST
} strFC0717Info;

#endif
