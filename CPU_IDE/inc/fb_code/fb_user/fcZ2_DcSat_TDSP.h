/**************************************************************************
** Zone2_DcSat_TDSP(Transformer DC Saturation Protection)(FB0712)
**************************************************************************/
#ifndef FCZ2_DCSAT_TDSP_H
#define FCZ2_DCSAT_TDSP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ2_DcSat_TDSPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ2_DcSat_TDSPRunFunc(uint32, uint32);

/*********************************************************************
**	Zone2_DcSat_TDSP(Transformer DC Saturation Protection) FB 	
**********************************************************************/
#define	FC0712_INIT				0
#define	FC0712_RUN				1

#define	FC0712_CODE				712
#define	FC0712_SPEC_NUM			5
#define	FC0712_VAR_NUM			1	
#define	FC0712_OUTPUT_NUM		1

//FB Variable 
typedef struct {
    // Input
    int32 					enable;				// Logic Enable
	int32 					reset;				// Reset
	float 		 			idc_tr;				// idc_tr 
	float					threshold;			// Threshold
	float					setT;				// Threshold Time Set
	
    // Inner variable
	float 					t1;  				// Time 1 
		
    // Output
    float  					o1_emst;			// Transformer DC Current EMST
} strFC0712Info;

#endif
