/**************************************************************************
** Zone4_87DPN_DCDP(DC Current Differential Protection)(FB730)
**************************************************************************/
#ifndef FCZ4_87DPN_DCDP_H
#define FCZ4_87DPN_DCDP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ4_87DPN_DCDPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ4_87DPN_DCDPRunFunc(uint32, uint32);

/*********************************************************************
** Zone4_87DPN_DCDP(Arm DC Current Differential Protection) FB 	
**********************************************************************/
#define	FC0730_INIT				0
#define	FC0730_RUN				1

#define	FC0730_CODE				730
#define	FC0730_SPEC_NUM			6
#define	FC0730_VAR_NUM			1	
#define	FC0730_OUTPUT_NUM		1

//FB Variable 
typedef struct {
    // Input
    int32 					enable;				// Logic Enable
	int32 					reset;				// Reset
	float					idc_up;				// idc_up
	float					idc_low;			// idc_low
	float					threshold1;			// Threshold 1
	float					setT1;				// Threshold1 Time Set 1
		
    // Inner variable
	float		 			t1; 				// Time 1
		
    // Output
    uint32  				o1_emst;			// Out1 EMST
} strFC0730Info;

#endif
