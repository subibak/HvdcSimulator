/**************************************************************************
** Zone4_76D_DOCP(DC Over Current Protection)(FB728)
**************************************************************************/
#ifndef FCZ4_76A_DOCP_H
#define FCZ4_76A_DOCP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ4_76A_DOCPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ4_76A_DOCPRunFunc(uint32, uint32);

/*********************************************************************
** Zone4_76D_DOCP(DC Over Current Protection) FB 	
**********************************************************************/
#define	FC0728_INIT				0
#define	FC0728_RUN				1

#define	FC0728_CODE				728
#define	FC0728_SPEC_NUM			6
#define	FC0728_VAR_NUM			2	
#define	FC0728_OUTPUT_NUM		2

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
	float		 			t1_up; 				// Time 1 Up
	float		 			t1_low;				// Time 1 Low
	
    // Output
    uint32  				o1_emst_up;			// Out1 EMST Up
    uint32  				o1_emst_low;		// Out1 EMST Low
} strFC0728Info;

#endif
