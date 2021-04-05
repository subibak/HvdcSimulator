/**************************************************************************
** Zone1_27_ABUV(154kV AC Bus UV)(FB0701)
**************************************************************************/
#ifndef FCZ1_27_ABUV_H
#define FCZ1_27_ABUV_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ1_27_ABUVInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ1_27_ABUVRunFunc(uint32, uint32);

/*********************************************************************
**	Zone1_27_ABUV(154kV AC Bus UV) FB 	
**********************************************************************/
#define	FC0701_INIT				0
#define	FC0701_RUN				1

#define	FC0701_CODE				701
#define	FC0701_SPEC_NUM			9
#define	FC0701_VAR_NUM			6	
#define	FC0701_OUTPUT_NUM		6

//FB Variable 
typedef struct {
    // Input
    int32 			enable;				// Logic Enable
	int32 			reset;				// Reset
	STF_3P 			vrms;				// Vrms
	float			threshold1;			// Threshold 1
	float			threshold2;			// Threshold 2
	float			setT1;				// Threshold1 Time Set 1
	float			setT2;				// Threshold1 Time Set 2
	
    // Inner variable
	STF_3P 			t1;  				// Time 1 
	STF_3P 			t2;  				// Time 2 
		
    // Output
    STU_3P			o1_alarm;			// Alarm
    STU_3P  		o2_conBlk;			// Converter Block
} strFC0701Info;

#endif
