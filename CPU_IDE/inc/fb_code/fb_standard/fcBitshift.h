#ifndef FCBITSHIFT_H
#define FCBITSHIFT_H

/*********************************************************************
**	Function Type
**********************************************************************/
extern uint32	stdRolInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdRolRunFunc	(uint32 , uint32 );

extern uint32	stdRorInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdRorRunFunc	(uint32 , uint32 );

extern uint32	stdShlInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdShlRunFunc	(uint32 , uint32 );

extern uint32	stdShrInitFunc	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	stdShrRunFunc	(uint32 , uint32 );

/*********************************************************************
**	
**	Bit Rotation Left FB 정의
**
**********************************************************************/
#define	ROL_CODE				48
#define	ROL_SPEC_NUM			2
#define	ROL_VAR_NUM				0
#define	ROL_OUTPUT_NUM			1
#define ROL_REF_SINDEX 			2

typedef struct {		
	/* 스펙 */	
	int32 	bitRotationCnt;
	uint32	input;
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	output;
} strFC0048Info;		

/*********************************************************************
**	
**	Bit Rotation Right FB 정의
**
**********************************************************************/
#define	ROR_CODE			49
#define	ROR_SPEC_NUM		2
#define	ROR_VAR_NUM			0
#define	ROR_OUTPUT_NUM		1
#define	ROR_REF_SINDEX		2
		
typedef struct {		
	/* 스펙 */	
	uint32 	bitRotationCnt;
	uint32	input;
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	output;
} strFC0049Info;		

/*********************************************************************
**	
**	Bit Shift Left FB 정의
**
**********************************************************************/
#define	SHL_CODE			50
#define	SHL_SPEC_NUM		2
#define	SHL_VAR_NUM			0
#define	SHL_OUTPUT_NUM		1
		
typedef struct {		
	/* 스펙 */	
	int32 	bitShiftCnt;
	uint32	input;
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	output;
} strFC0050Info;		

/*********************************************************************
**	
**	Bit Shift Right FB 정의
**
**********************************************************************/
#define	SHR_CODE			51
#define	SHR_SPEC_NUM		2
#define	SHR_VAR_NUM			0
#define	SHR_OUTPUT_NUM		1
		
typedef struct {		
	/* 스펙 */	
	uint32 	bitShiftCnt;
	uint32	input;
		
	/* 내부변수 */	
		
	/* 출력 */	
	uint32	output;
} strFC0051Info;		



#endif