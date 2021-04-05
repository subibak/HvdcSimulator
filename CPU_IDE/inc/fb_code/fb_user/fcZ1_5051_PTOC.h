/**************************************************************************
** Zone1_50/51_PTOC(Primary Transformer OverCurrent)(FB0706)
**************************************************************************/
#ifndef FCZ1_5051_PTOC_H
#define FCZ1_5051_PTOC_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ1_5051_PTOCInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ1_5051_PTOCRunFunc(uint32, uint32);

/*********************************************************************
**	Zone1_50/51_PTOC(Primary Transformer OverCurrent) FB 	
**********************************************************************/
#define	FC0706_INIT				0
#define	FC0706_RUN				1

#define	FC0706_CODE				706
#define	FC0706_SPEC_NUM			8
#define	FC0706_VAR_NUM			6	
#define	FC0706_OUTPUT_NUM		6


//FB Variable 
typedef struct {
    // Input
    int32 			enable;				// Logic Enable
	int32 			reset;				// Reset
	STF_3P 			ipcc;				// ipcc
	
	float			threshold1;			// Threshold1
	float			threshold2;			// Threshold2
	float			setT;				// Threshold1,2 Time Set
	
    // Inner variable
	STF_3P 			t1;  				// Time 1 
	STF_3P 			t2;  				// Time 2 
		
    // Output
    STU_3P  		o1_emst;			// Out1 EMST
    STU_3P			o2_emst;			// Out2 EMST
} strFC0706Info;

#endif
