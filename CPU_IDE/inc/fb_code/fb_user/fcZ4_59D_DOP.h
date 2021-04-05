/**************************************************************************
** Zone4_59D_DOP(DC Overvoltage Protection)(FB725)
**************************************************************************/
#ifndef FCZ4_59D_DOP_H
#define FCZ4_59D_DOP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ4_59D_DOPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ4_59D_DOPRunFunc(uint32, uint32);

/*********************************************************************
**	Zone4_27D_DUP(DC Undervoltage Protection) FB 	
**********************************************************************/
#define	FC0725_INIT				0
#define	FC0725_RUN				1

#define	FC0725_CODE				725
#define	FC0725_SPEC_NUM			8
#define	FC0725_VAR_NUM			4	
#define	FC0725_OUTPUT_NUM		4

//FB Variable 
typedef struct {
    // Input
    int32 					enable;				// Logic Enable
	int32 					reset;				// Reset
	float					vdc_up;				// Vdc_up
	float					vdc_low;			// Vdc_low
	float					threshold1;			// Threshold 1
	float					threshold2;			// Threshold 2
	float					setT1;				// V2nd Threshold1 Time Set 1
	float					setT2;				// V2nd Threshold1 Time Set 2
	
    // Inner variable
	float		 			t1_up; 				// Time 1 Up
	float 					t2_up; 				// Time 2 Up
	float		 			t1_low;				// Time 1 Low
	float 					t2_low;				// Time 2 Low
	
    // Output
    float  					o1_conBlk_up;		// Out1 conBlk Up
    float  					o2_nmst_up;			// Out2 NMST Up
    float  					o1_conBlk_low;		// Out1 conBlk Low
    float  					o2_nmst_low;		// Out2 NMST Low
} strFC0725Info;

#endif
