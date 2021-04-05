/**************************************************************************
** Zone4_76A_AOCP(Arm Over Current Protection)(FB727)
**************************************************************************/
#ifndef FCZ4_76A_AOCP_H
#define FCZ4_76A_AOCP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ4_76A_AOCPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ4_76A_AOCPRunFunc(uint32, uint32);

/*********************************************************************
** Zone4_76A_AOCP(Arm Over Current Protection) FB 	
**********************************************************************/
#define	FC0727_INIT				0
#define	FC0727_RUN				1

#define	FC0727_CODE				727
#define	FC0727_SPEC_NUM			7
#define	FC0727_VAR_NUM			3	
#define	FC0727_OUTPUT_NUM		3

//FB Variable 
typedef struct {
    // Input
    int32 			enable;				// Logic Enable
	int32 			reset;				// Reset
	STF_3P			iarm;				// iarm
	float			threshold1;			// Threshold 1
	float			setT1;				// Threshold1 Time Set 1
	
    // Inner variable
	STF_3P 			t1; 				// Time 1

    // Output
    STU_3P  		o1_emst;			// Out1 EMST   
} strFC0727Info;

#endif