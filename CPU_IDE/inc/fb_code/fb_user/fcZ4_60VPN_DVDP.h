/**************************************************************************
** Zone4_60VPN_DVDP(DC Voltage Differential Protection)(FB726)
**************************************************************************/
#ifndef FCZ4_60VPN_DVDP_H
#define FCZ4_60VPN_DVDP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ4_60VPN_DVDPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ4_60VPN_DVDPRunFunc(uint32, uint32);

/*********************************************************************
** Zone4_60VPN_DVDP(DC Voltage Differential Protection) FB 	
**********************************************************************/
#define	FC0726_INIT				0
#define	FC0726_RUN				1

#define	FC0726_CODE				726
#define	FC0726_SPEC_NUM			8
#define	FC0726_VAR_NUM			2	
#define	FC0726_OUTPUT_NUM		2

//FB Variable 
typedef struct {
    // Input
    int32 					enable;				// Logic Enable
	int32 					reset;				// Reset
	float					vdc_up;				// Vdc_up
	float					vdc_low;			// Vdc_low
	float					threshold1;			// Threshold 1
	float					threshold2;			// Threshold 2
	float					setT1;				// Threshold1 Time Set 1
	float					setT2;				// Threshold1 Time Set 2
	
    // Inner variable
	float		 			t1; 				// Time 1
	float 					t2; 				// Time 2
	
    // Output
    uint32  				o1_alarm;			// Out1 Alarm   
    uint32  				o2_conBlk;			// Out2 conBlk
 } strFC0726Info;

#endif
