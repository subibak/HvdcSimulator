/**************************************************************************
** Zone1_50N/51N(FB0707)
**************************************************************************/
#ifndef FCZ1_50N51N_H
#define FCZ1_50N51N_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ1_50N51NPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ1_50N51NRunFunc(uint32, uint32);

/*********************************************************************
**	Zone1_50N/51N FB 	
**********************************************************************/
#define	FC0707_INIT				0
#define	FC0707_RUN				1

#define	FC0707_CODE				707
#define	FC0707_SPEC_NUM			6
#define	FC0707_VAR_NUM			2	
#define	FC0707_OUTPUT_NUM		2


//FB Variable 
typedef struct {
    // Input
    int32 					enable;				// Logic Enable
	int32 					reset;				// Reset
	float 		 			itr_n;				// itr_n
	float					threshold1;			// Threshold1
	float					threshold2;			// Threshold2
	
	float					setT;				// Threshold1,2 Time Set
	
      // Inner variable
	float	 				t1;  				// Time 1 
	float 					t2;  				// Time 2 
		
    // Output
    uint32 					o1_emst;			// Out1 EMST
    uint32					o2_emst;			// Out2 EMST
} strFC0707Info;

#endif
