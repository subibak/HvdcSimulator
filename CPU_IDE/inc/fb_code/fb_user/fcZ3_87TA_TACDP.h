/**************************************************************************
** Zone3_87TA_TACDP(Terminal - Arm CT Differential Protection)(FB723)
**************************************************************************/
#ifndef FCZ3_87TA_TACDP_H
#define FCZ3_87TA_TACDP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ3_87TA_TACDPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ3_87TA_TACDPRunFunc(uint32, uint32);

/*********************************************************************
**	Zone3_60T_PCI(Phase Current Imbalance) FB 	
**********************************************************************/
#define	FC0723_INIT				0
#define	FC0723_RUN				1

#define	FC0723_CODE				723
#define	FC0723_SPEC_NUM			12
#define	FC0723_VAR_NUM			0	
#define	FC0723_OUTPUT_NUM		3

//FB Variable 
typedef struct {
    // Input
    int32 			enable;				// Logic Enable
	int32 			reset;				// Reset
	STF_3P			iterminal;			// iterminal
	STF_3P			iup;				// iup
	STF_3P			ilow;				// ilow
	float			delta;				// Delta

    // Inner variable
		
    // Output
    STU_3P  		o1_emst;			// Out1 EMST
} strFC0723Info;

#endif
