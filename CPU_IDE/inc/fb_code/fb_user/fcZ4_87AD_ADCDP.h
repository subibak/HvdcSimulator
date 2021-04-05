/**************************************************************************
** Zone4_87AD_ADCDP(Arm DC Current Differential Protection)(FB729)
**************************************************************************/
#ifndef FCZ4_87AD_ADCDP_H
#define FCZ4_87AD_ADCDP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ4_87AD_ADCDPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ4_87AD_ADCDPRunFunc(uint32, uint32);

/*********************************************************************
** Zone4_87AD_ADCP(Arm DC Current Differential Protection) FB 	
**********************************************************************/
#define	FC0729_INIT				0
#define	FC0729_RUN				1

#define	FC0729_CODE				729
#define	FC0729_SPEC_NUM			8
#define	FC0729_VAR_NUM			1	
#define	FC0729_OUTPUT_NUM		1

//FB Variable 
typedef struct {
    // Input
    int32 			enable;				// Logic Enable
	int32 			reset;				// Reset
	float			idc;				// idc
	STF_3P			iarm;				// iarm
	float			threshold1;			// Threshold 1
	float			setT1;				// Threshold1 Time Set 1
		
    // Inner variable
	float			t1; 				// Time 1
		
    // Output
    float  			o1_emst;			// Out1 EMST
} strFC0729Info;

#endif
