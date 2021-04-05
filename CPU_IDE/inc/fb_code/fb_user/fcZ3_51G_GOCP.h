/**************************************************************************
** Zone3_51G_GOCP(Ground Over Current Protection)(FB719)
**************************************************************************/
#ifndef FCZ3_51G_GOCP_H
#define FCZ3_51G_GOCP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ3_51G_GOCPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ3_51G_GOCPRunFunc(uint32, uint32);

/*********************************************************************
** Zone3_51G_GOCP(Ground Over Current Protection) FB	
**********************************************************************/
#define	FC0719_INIT				0
#define	FC0719_RUN				1

#define	FC0719_CODE				719
#define	FC0719_SPEC_NUM			7
#define	FC0719_VAR_NUM			2	
#define	FC0719_OUTPUT_NUM		2

//FB Variable 
typedef struct {
    // Input
    int32 					enable;				// Logic Enable
	int32 					reset;				// Reset
	float					igr;				// igr
	float					threshold1;			// Threshold 1
	float					threshold2;			// Threshold 2
	float					setT1;				// Threshold1 Time Set 1
	float					setT2;				// Threshold1 Time Set 2

    // Inner variable
	float  					t1;  				// Time 1 
	float		 			t2;  				// Time 2 
			
    // Output
    float		  			o1_emst;			// Out1 EMST
    float		  			o2_emst;			// Out2 EMST
} strFC0719Info;

#endif
