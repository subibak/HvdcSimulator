/**************************************************************************
** Zone3_49_PROP(Pre-insertion Resistor Overload Protection)(FB718)
**************************************************************************/
#ifndef FCZ3_49_PROP_H
#define FCZ3_49_PROP_H

#include	"fb_code\fb_user\fcPrtCommon.h"

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ3_49_PROPInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ3_49_PROPRunFunc(uint32, uint32);

/*********************************************************************
** Zone3_49_PROP(Pre-insertion Resistor Overload Protection) FB	
**********************************************************************/
#define	FC0718_INIT				0
#define	FC0718_RUN				1

#define	FC0718_CODE				718
#define	FC0718_SPEC_NUM			5
#define	FC0718_VAR_NUM			0	
#define	FC0718_OUTPUT_NUM		2

//FB Variable 
typedef struct {
    // Input
    int32 					enable;				// Logic Enable
	int32 					reset;				// Reset
	float					temp;				// Temperature
	float					threshold1;			// Threshold 1
	float					threshold2;			// Threshold 2	
			
    // Output
    uint32		  			o1_alarm;			// Out1 Alarm
    uint32		  			o2_emst;			// Out2 EMST
} strFC0718Info;

#endif
