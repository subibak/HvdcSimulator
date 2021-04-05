/**************************************************************************
** Zone2_87N_REFP(Restricted Earth Fault Protection)(FB0710)
**************************************************************************/
#ifndef FCZ2_87N_REFP_H
#define FCZ2_87N_REFP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ2_87N_REFPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ2_87N_REFPRunFunc(uint32, uint32);

/*********************************************************************
**	Zone2_87N_REFP(Restricted Earth Fault Protection) FB 	
**********************************************************************/
#define	FC0710_INIT				0
#define	FC0710_RUN				1

#define	FC0710_CODE				710
#define	FC0710_SPEC_NUM			8
#define	FC0710_VAR_NUM			3	
#define	FC0710_OUTPUT_NUM		3

//FB Variable 
typedef struct {
    // Input
    int32 			enable;				// Logic Enable
	int32 			reset;				// Reset
	float 		 	in;					// in
	STF_3P			ip;					// ip
	float			delta;				// Delta
	float			setT;				// Delta Time Set
	
    // Inner variable
	STF_3P 			t1;  				// Time 1 
		
    // Output
    STU_3P  		o1_emst;			// Out1 EMST

} strFC0710Info;

#endif
