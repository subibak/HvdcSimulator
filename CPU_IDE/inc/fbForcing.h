
#ifndef FBSIMUL_H
#define FBSIMUL_H

#include	"memDef.h"

typedef struct {
 	uint32	taskId; 
  	uint32	functionCode;
	uint32	fbFuncAddr;

	uint32	simCheck;
	uint32	simFlag;
	uint32	outputIndex; 
}strSimInfo;


typedef struct {
    uint32  moduleId;
	uint32  taskId;
    uint32  FBCode;
    uint32  FBAddr;
    uint16  pointId;
	uint16  forcingFlag;
}strFBforcingInfo;



#define	SIM_FB_CHECK_ASCENDING	(1)
#define	SIM_FB_CHECK_DESCENDING	(2)

#endif 
