#ifndef FB_RUNNING_ERR_CODE_H
#define FB_RUNNING_ERR_CODE_H

#if ( (!defined(NO_ERROR)) || (NO_ERROR != 0) )
#undef	NO_ERROR
#define	NO_ERROR	(0)
#endif

/**************************************************************************
**		FB ����� �߻��Ǵ� ���� ����
**************************************************************************/
#define	FB_EXEC_ERR_GROUP			0x00000000

#define	FB_INPUT_DATA_ERR				(FB_EXEC_ERR_GROUP|0x0001)
#define	FB_INPUT_TYPE_ERR				(FB_EXEC_ERR_GROUP|0x0002)
#define	FB_INPUT_RANGE_ERR				(FB_EXEC_ERR_GROUP|0x0003)
#define	FB_OUTPUT_NEGATE_FLAG_ERR		(FB_EXEC_ERR_GROUP|0x0004)
#define	FB_OUTPUT_DATA_SIZE_ERR			(FB_EXEC_ERR_GROUP|0x0005)

/**************************************************************************
**		������� ���� ���� ��Ʈ�� ����
**
[���� ����]
	- ���� �ڵ� �߰� �� ���� �Ʒ����� �߰�
**************************************************************************/
#define	MAX_FB_EXEC_ERR_NUM	0x05

static uint8 *fbExecErrStringForDbg[MAX_FB_EXEC_ERR_NUM+1] = {
	(uint8 *)"NO Error",
	(uint8 *)"FB_INPUT_DATA_ERR",		
	(uint8 *)"FB_INPUT_TYPE_ERR",		
	(uint8 *)"FB_INPUT_RANGE_ERR",		
	(uint8 *)"FB_OUTPUT_NEGATE_FLAG_ERR",	
	(uint8 *)"FB_OUTPUT_DATA_SIZE_ERR"	
};

#endif

