#ifndef FCEXTRA_H
#define FCEXTRA_H

/*********************************************************************
**	Function Type
**********************************************************************/
extern uint32	wrVarInitFunc 	(uint32 *, uint32 *, uint32 *, strFbDefInfo *);
extern uint32	wrVarRunFunc	(uint32 , uint32 );

/*********************************************************************
**	
**	기존 FB의 출력에 변수를 연결할 때 editor 컴파일 시에 FB가 생성됨 
**
**********************************************************************/
#define	WRVAR_CODE					681
#define	WRVAR_SPEC_NUM				2
#define	WRVAR_VAR_NUM				0	
#define	WRVAR_OUTPUT_NUM			1
#define	WRVAR_REF_SINDEX			1

typedef struct {
	
	uint32	in1;			
	uint32	in2; 			

	

    
	uint32	out;
} strFC0681Info;

#endif