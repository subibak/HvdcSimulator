/**************************************************************************
** Zone3_87ST_TTDP(Tr2nd-Terminal Differential Protection)(FB720)
**************************************************************************/
#ifndef FCZ3_87ST_TTDP_H
#define FCZ3_87ST_TTDP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ3_87ST_TTDPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ3_87ST_TTDPRunFunc(uint32, uint32);

/*********************************************************************
** Zone3_87ST_TTDP(Tr2nd-Terminal Differential Protection) FB	
**********************************************************************/
#define	FC0720_INIT				0
#define	FC0720_RUN				1

#define	FC0720_CODE				720
#define	FC0720_SPEC_NUM			9
#define	FC0720_VAR_NUM			0	
#define	FC0720_OUTPUT_NUM		3

//FB Variable 
typedef struct {
    // Input
    int32 			enable;				// Logic Enable
	int32 			reset;				// Reset
	STF_3P			itr_2nd;			// igr
	STF_3P			iterminal;			// iterminal
	float			delta;				// Delta

    // Inner variable
		
    // Output
    STU_3P			o1_emst;			// Out1 EMST
} strFC0720Info;

#endif
