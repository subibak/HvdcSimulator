/**************************************************************************
** Zone4_51Dharm(FB733)
**************************************************************************/
#ifndef FCZ4_51DHARM_H
#define FCZ3_51DHARM_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ4_51DharmInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ4_51DharmRunFunc(uint32, uint32);

/*********************************************************************
** Zone4_51Aharm FB	
**********************************************************************/
#define	FC0733_INIT				0
#define	FC0733_RUN				1

#define	FC0733_CODE				733
#define	FC0733_SPEC_NUM			16
#define	FC0733_VAR_NUM			12	
#define	FC0733_OUTPUT_NUM		12

//FB Variable 
typedef struct {
    // Input
    int32 			enable;				// Logic Enable
	int32 			reset;				// Reset
	STF_3P 			ithd;				// ithd
	float			threshold1;			// ithd Threshold1
	float			threshold2;			// ithd Threshold2
	float			setT1;				// ithd Threshold1 Time Set
	float			setT2;				// ithd Threshold2 Time Set
	STF_3P 			ih;					// ih
	float			threshold_h1;		// ih Threshold1
	float			threshold_h2;		// ih Threshold2
	float			setT_h1;			// ih Threshold1 Time Set
	float			setT_h2;			// ih Threshold2 Time Set

    // Inner variable
	STF_3P 			t1;  				// Time 1 
	STF_3P 			t2;	  				// Time 2
	STF_3P 			t_h1;			 	// Time_h1
	STF_3P 			t_h2;			   	// Time_h2

    // Output
    STU_3P  		o1_alarm;			// ithd Out1 Alarm
    STU_3P  		o2_conBlk;			// ithd Out2 Converter Block
	STU_3P  		o1_alarm_h;			// ih Out1 Alarm
    STU_3P  		o2_conBlk_h;		// ih Out2 Converter Block	
    
} strFC0733Info;

#endif
