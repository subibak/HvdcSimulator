/**************************************************************************
** Zone1_64_OVG(OVG)(FB0703)
**************************************************************************/
#ifndef FCZ1_64_OVG_H
#define FCZ1_64_OVG_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ1_64_OVGInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ1_64_OVGRunFunc(uint32, uint32);

/*********************************************************************
**	Zone1_64_OVG(OVG) FB 	
**********************************************************************/
#define	FC0703_INIT				0
#define	FC0703_RUN				1

#define	FC0703_CODE				703
#define	FC0703_SPEC_NUM			5
#define	FC0703_VAR_NUM			1	
#define	FC0703_OUTPUT_NUM		1

//FB Variable 
typedef struct {
    // Input
    int32 					enable;				// Logic Enable
	int32 					reset;				// Reset
	float 		 			vrms0;				// vrms_0
	float					threshold1;			// Threshold1
	float					setT1;				// Threshold1 Time Set
		
    // Inner variable
	float					t1;  				// Time 1 
		
    // Output
    uint32		 			o1_alarm;			// Out1 Alarm	
} strFC0703Info;

#endif
