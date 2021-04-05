#ifndef FCSELECTION_H
#define FCSELECTION_H

/*********************************************************************
**	����� ����
**********************************************************************/
#ifndef MY_MIN
	#define MY_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef MY_MAX
	#define MY_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

/*********************************************************************
**	Function Type
**********************************************************************/
extern uint32	stdLimitInitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdLimitRunFunc	(uint32, uint32);

extern uint32	stdMax2InitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdMax2RunFunc	(uint32, uint32);

extern uint32	stdMax4InitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdMax4RunFunc	(uint32, uint32);

extern uint32	stdMax8InitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdMax8RunFunc	(uint32, uint32);

extern uint32	stdMin2InitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdMin2RunFunc	(uint32, uint32);

extern uint32	stdMin4InitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdMin4RunFunc	(uint32, uint32);

extern uint32	stdMin8InitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdMin8RunFunc	(uint32, uint32);

extern uint32	stdMux2InitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdMux2RunFunc	(uint32, uint32);

extern uint32	stdMux4InitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdMux4RunFunc	(uint32, uint32);

extern uint32	stdMux8InitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdMux8RunFunc	(uint32, uint32);

extern uint32	stdSelInitFunc	(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdSelRunFunc	(uint32, uint32);

/*********************************************************************
**	
**	Limit FB ����
**
**********************************************************************/
#define	LIMIT_CODE					231
#define	LIMIT_SPEC_NUM				3
#define	LIMIT_VAR_NUM				0
#define	LIMIT_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */
	uint32 	min; 		/* S01: �Է°� */
	uint32	input; 		/* S02: �Է°� */
	uint32	max; 		/* S03: �Է°� */

	/* ���κ��� - 0�� */

    /* ��� */
    uint32	output;		/* N01: ��°� */
} strFC0231Info;		

/*********************************************************************
**	
**	2�� �Է¿� ���� Max �� ���� FB ����
**
**********************************************************************/
#define	MAX2_CODE				232
#define	MAX2_SPEC_NUM			2
#define	MAX2_VAR_NUM			0
#define	MAX2_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */	
	float	in[2];
		
	/* ���κ��� */	
		
	/* ��� */	
	float	out;
} strFC0232Info;		
	
/*********************************************************************
**	
**	4�� �Է¿� ���� Max �� ���� FB ����
**
**********************************************************************/
#define	MAX4_CODE				234
#define	MAX4_SPEC_NUM			4
#define	MAX4_VAR_NUM			0
#define	MAX4_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */	
	float	in[4];
		
	/* ���κ��� */	
		
	/* ��� */	
	float	out;
} strFC0234Info;		

/*********************************************************************
**	
**	8�� �Է¿� ���� Max �� ���� FB ����
**
**********************************************************************/
#define	MAX8_CODE				238
#define	MAX8_SPEC_NUM			8
#define	MAX8_VAR_NUM			0
#define	MAX8_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */	
	float	in[8];
		
	/* ���κ��� */	
		
	/* ��� */	
	float	out;
} strFC0238Info;		

/*********************************************************************
**	
**	2�� �Է¿� ���� Min. �� ���� FB ����
**
**********************************************************************/
#define	MIN2_CODE				240
#define	MIN2_SPEC_NUM			2
#define	MIN2_VAR_NUM			0
#define	MIN2_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */	
	float	in[2];
		
	/* ���κ��� */	
		
	/* ��� */	
	float	out;
} strFC0240Info;		

/*********************************************************************
**	
**	4�� �Է¿� ���� Min. �� ���� FB ����
**
**********************************************************************/
#define	MIN4_CODE				242
#define	MIN4_SPEC_NUM			4
#define	MIN4_VAR_NUM			0
#define	MIN4_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */	
	float	in[4];
		
	/* ���κ��� */	
		
	/* ��� */	
	float	out;
} strFC0242Info;		

/*********************************************************************
**	
**	8�� �Է¿� ���� Min. �� ���� FB ����
**
**********************************************************************/
#define	MIN8_CODE				246
#define	MIN8_SPEC_NUM			8
#define	MIN8_VAR_NUM			0
#define	MIN8_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */	
	float	in[8];
		
	/* ���κ��� */	
		
	/* ��� */	
	float	out;
} strFC0246Info;		

/*********************************************************************
**	
**	2�� �Է¿� ���� index�� ���� ���� ���� FB ����
**
**********************************************************************/
#define	MUX2_CODE				248
#define	MUX2_SPEC_NUM			3
#define	MUX2_VAR_NUM			0
#define	MUX2_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */	
	int32   choiceIndex;
	float	in[2];
		
	/* ���κ��� */	
		
	/* ��� */	
	float	out;
} strFC0248Info;		

/*********************************************************************
**	
**	4�� �Է¿� ���� index�� ���� ���� ���� FB ����
**
**********************************************************************/
#define	MUX4_CODE				250
#define	MUX4_SPEC_NUM			5
#define	MUX4_VAR_NUM			0
#define	MUX4_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */	
	int32   choiceIndex;
	float	in[4];
		
	/* ���κ��� */	
		
	/* ��� */	
	float	out;
} strFC0250Info;		

/*********************************************************************
**	
**	8�� �Է¿� ���� index�� ���� ���� ���� FB ����
**
**********************************************************************/
#define	MUX8_CODE				254
#define	MUX8_SPEC_NUM			9
#define	MUX8_VAR_NUM			0
#define	MUX8_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */	
	int32   choiceIndex;
	float	in[8];
		
	/* ���κ��� */	
		
	/* ��� */	
	float	out;
} strFC0254Info;		

/*********************************************************************
**	
**	2�� �Է¿� ���� index�� ���� ���� ���� FB ����
**
**********************************************************************/
#define	SEL_CODE				256
#define	SEL_SPEC_NUM			3
#define	SEL_VAR_NUM				0
#define	SEL_OUTPUT_NUM			1
		
typedef struct {		
	/* ���� */	
	uint32  choiceIndex;
	float   ina;
	float   inb;
		
	/* ���κ��� */	
		
	/* ��� */	
	float	out;
} strFC0256Info;		






#endif