/**************************************************************************
** Zone3_60T_PCI(Phase Current Imbalance)(FB722)
**************************************************************************/
#ifndef FCZ3_60T_PCI_H
#define FCZ3_60T_PCI_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ3_60T_PCIInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ3_60T_PCIRunFunc(uint32, uint32);

/*********************************************************************
**	Zone3_60T_PCI(Phase Current Imbalance) FB 	
**********************************************************************/
#define	FC0722_INIT				0
#define	FC0722_RUN				1

#define	FC0722_CODE				722
#define	FC0722_SPEC_NUM			9
#define	FC0722_VAR_NUM			2	
#define	FC0722_OUTPUT_NUM		2

//FB Variable 
typedef struct {
    // Input
    int32 			enable;				// Logic Enable
	int32 			reset;				// Reset
	STF_3P			itrm_rms;			// itrm_rms
	float			threshold1;			// Threshold 1
	float			threshold2;			// Threshold 2
	float			setT1;				// Threshold1 Time Set 1
	float			setT2;				// Threshold1 Time Set 2
	
    // Inner variable
	float  			t1;  				// Time 1 
	float  			t2;  				// Time 2 
		
    // Output
    uint32  		o1_alarm;			// Out1 Alarm
    uint32 			o2_conBlk;			// Out2 Converter Block
} strFC0722Info;

#endif
