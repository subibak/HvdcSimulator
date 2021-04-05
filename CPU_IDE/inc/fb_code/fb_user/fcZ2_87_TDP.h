/**************************************************************************
** Zone2_87_TDP(Transformer Differential Protection)(FB0713)
**************************************************************************/
#ifndef FCZ2_87_TDP_H
#define FCZ2_87_TDP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ2_87_TDPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ2_87_TDPRunFunc(uint32, uint32);

/*********************************************************************
**	Zone2_87_TDP(Transformer Differential Protection) FB 	
**********************************************************************/
#define	FC0713_INIT				0
#define	FC0713_RUN				1

#define	FC0713_CODE				713
#define	FC0713_SPEC_NUM			18
#define	FC0713_VAR_NUM			3	
#define	FC0713_OUTPUT_NUM		5

//FB Variable 
typedef struct {
    // Input
    int32 			enable;				// Logic Enable
	int32 			reset;				// Reset
	STF_3P			ipcc;				// PCC Current
	
	STF_3P			isec;				// Transformer Secondary Current
	float 		 	ig;					// Transformer Earth Current
	int32			a26W;				// 26W 	Alarm
	
	int32			t26W;				// 26W 	Trip
	int32			t96P;				// 96P 	Trip
	int32			t96D;				// 96D 	Trip
	int32			a96B1;				// 96B1 Alarm
	int32			t96B2;				// 96B2 Trip
	
	int32			t96T;				// 96T 	Trip
	float			delta;				// Delta
	float			setT;				// Set
	
    // Inner variable
	STF_3P			t1;  				// Time 1 
		
    // Output
    float			o1_alarm;			// Out1 Alarm
    STU_4V			o2_emst;			// Out2 EMST
} strFC0713Info;

#endif
