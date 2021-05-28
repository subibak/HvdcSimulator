/**************************************************************************
** Protection Common Header
**************************************************************************/
#ifndef FCPRTCOMMON_H
#define FCPRTCOMMON_H

#define FAULT					1
#define NORMAL					0
#define ZERO					0

#define ABSt(var)				(var > 0 ? var : -var) 

/*********************************************************************
**	
**	structure definition for Protection(zone 1~4)	
**
**********************************************************************/
typedef struct {
	float 			a;				// Phase-A
	float 			b;				// Phase-B
	float 		 	c;				// Phase-C
} STF_3P;

typedef struct {
	uint32 			a;				// Phase-A
	uint32 			b;				// Phase-B
	uint32 		 	c;				// Phase-C
} STU_3P;

typedef struct {
	float 			a;				// Phase-A
	float 			b;				// Phase-B
	float 		 	c;				// Phase-C
	float			s;				// Single Varaible
} STF_4V;

typedef struct {
	uint32 			a;				// Phase-A
	uint32 			b;				// Phase-B
	uint32 		 	c;				// Phase-C
	uint32			s;				// Single Varaible
} STU_4V;

/*
#define ENABLE					0x1
#define DISALBE					0x0

#define RESET					0x2
#define NORST					0x0

#define FAULT_PHA				0x1
#define NOFLT_PHA				0x0

#define	FAULT_PHB				0x2
#define NOFLT_PHB				0x0

#define FAULT_PHC				0x4
#define NOFLT_PHC				0x0

#define FAULT_SINGLE			0x8
#define NOFLT_SINGLE			0x0

typedef union {
	uint32			all;
	struct {
		uint32		EN:1;			// Enable(1: Enable, 0: Disable)
		uint32		RST:1;			// Reset(1: Reset, 0: No Reset)
		uint32		rsvd:30;		// Reserved
	} bit;
} ST_PRT_IN;

typedef union {
	uint32			all;
	struct {
		uint32		a:1;			// Phase-A Out
		uint32		b:1;			// Phase-B Out
		uint32		c:1;			// Phase-C Out
		uint32		s:1;			// Single Variable Out
		uint32		rsvd:28;		// Reserved
	} bit;
} ST_PRT_OUT;
*/

#endif
