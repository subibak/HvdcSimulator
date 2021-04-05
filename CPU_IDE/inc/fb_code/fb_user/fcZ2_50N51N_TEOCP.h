/**************************************************************************
** Zone2_50N/51N_TEOCP(Transformer Earth OverCurrent Protection)(FB0709)
**************************************************************************/
#ifndef FCZ2_50N51N_TEOCP_H
#define FCZ2_50N51N_TEOCP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ2_50N51N_TEOCPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ2_50N51N_TEOCPRunFunc(uint32, uint32);

/*********************************************************************
**	Zone2_50N/51N_TEOCP(Transformer Earth OverCurrent Protection) FB 	
**********************************************************************/
#define	FC0709_INIT				0
#define	FC0709_RUN				1

#define	FC0709_CODE				709
#define	FC0709_SPEC_NUM			6
#define	FC0709_VAR_NUM			2	
#define	FC0709_OUTPUT_NUM		2

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
    float 					o1_alarm;			// Out1 Alarm
    float					o2_conBlk;			// Out2 conBlk
} strFC0709Info;

#endif
