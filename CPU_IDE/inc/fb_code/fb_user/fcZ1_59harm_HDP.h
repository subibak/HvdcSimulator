/**************************************************************************
//Zone1_59_harm(Harmonic distortion Protection)
**************************************************************************/
#ifndef FCZ1_59HARM_HDP_H
#define FCZ1_59HARM_HDP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**		함수 Prototype
**************************************************************************/
extern uint32	stdZ1_59harm_HDPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ1_59harm_HDPRunFunc(uint32, uint32);

/*********************************************************************
**	Zone1_59harm_Harmonic Distortion Protection FB 	
**********************************************************************/
#define	FC0700_INIT				0
#define	FC0700_RUN				1

#define	FC0700_CODE				700
#define	FC0700_SPEC_NUM			16
#define	FC0700_VAR_NUM			12	
#define	FC0700_OUTPUT_NUM		12


//FB Variable 
typedef struct {
    // Input
    int32 			enable;				// Logic Enable
	int32 			reset;				// Reset
	STF_3P 			vthd;				// vthd
	float			threshold1;			// vthd Threshold1
	float			threshold2;			// vthd Threshold2
	float			setT1;				// vthd Threshold1 Time Set
	float			setT2;				// vthd Threshold2 Time Set
	STF_3P 			vh;					// vh
	float			threshold_h1;		// vh Threshold1
	float			threshold_h2;		// vh Threshold2
	float			setT_h1;			// vh Threshold1 Time Set
	float			setT_h2;			// vh Threshold2 Time Set

    // Inner variable
	STF_3P 			t1;  				// Time 1 
	STF_3P 			t2;	  				// Time 2
	STF_3P 			t_h1;			 	// Time_h1
	STF_3P 			t_h2;			   	// Time_h2

    // Output
    STU_3P  		o1_alarm;			// vthd Out1 Alarm
    STU_3P  		o2_conBlk;			// vthd Out2 Converter Block
	STU_3P  		o1_alarm_h;			// vh Out1 Alarm
    STU_3P  		o2_conBlk_h;		// vh Out2 Converter Block	

} strFC0700Info;

#endif
