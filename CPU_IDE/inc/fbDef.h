#ifndef FBDEF_H
#define FBDEF_H

/*********************************************************************
**	FB ID
**********************************************************************/
#ifndef LOGIC_ID
	#define	LOGIC_ID	(0x10)
#endif

/*********************************************************************
**	����� FB ID
**********************************************************************/
#define	MAX_FB_CODE_ID	(2000)
#define	MAX_FB_NUM		MAX_FB_CODE_ID

/*********************************************************************
**	
**	����� FB ����(�Է�, ���κ���, ���)���� Type�� �����ϴ� ����ü 	
**
**********************************************************************/
typedef struct {
	uint32			fbId;
    uint32			inputNo;
    uint32			intlVarNo;
    uint32			outputNo;
}strFbDefInfo;

typedef struct {
	strFbDefInfo	fbInfo;
    uint32			specTypeIndex;
    uint32			varTypeIndex;
    uint32			outputTypeIndex;
}strFbTypeIndexInfo;

typedef struct {
	uint32				totalFbCodeNum; 
    strFbTypeIndexInfo	*indexPtr;
}strFbCodeTypeInfo;

/*********************************************************************
**	
**	����� FB�� �����ϴ� Database ����ü 	
**
**********************************************************************/
typedef struct {
	uint8	*fbNamePtr;
	uint32	fbId;
	uint32	inputNo;
    uint32	intlVarNo;
    uint32	outputNo;
    uint32	(* fbInitFunc)(	uint32 *,uint32 *,uint32 *,strFbDefInfo	*);
    uint32	(* fbActFunc)(uint32, uint32); 
    uint32	*fbCodeTypeIndexPtr; 
	
	uint32	inputSize;
    uint32	intlVarSize;
    uint32	outputSize;
}strFbExecDbInfo;

#endif 
