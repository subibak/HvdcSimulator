#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_standard\fcEdgeDect.h"

/**************************************************************************
**		외부 함수 
**************************************************************************/
extern uint32 readRuntimeFbData(uint32,uint32,uint32,uint32 *);
extern uint32 writeRuntimeFbData(uint32, uint32,uint32, uint32 *);
extern void	  setErrorCode(uint8 *, uint32, uint8 *, uint32);

/*********************************************************************
**	
**	Falling Edge Trigger FB 정의
**
**********************************************************************/
uint32	stdFtrigInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= FTRIG_CODE;
    paraInfoPtr->inputNo	= FTRIG_SPEC_NUM;
    paraInfoPtr->intlVarNo	= FTRIG_VAR_NUM;
    paraInfoPtr->outputNo	= FTRIG_OUTPUT_NUM;

	/* 입력 - 1개 */
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 1개 */	
	*varTypePtr++			= BOOL_TYPE | SIZE32_TYPE;

    /* 출력 - 1개 */
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;

	return (status);
}	 	
	 	
uint32	stdFtrigRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	strFC0211Info fd;
	
    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	if( (fd.input == 0 ) && (fd.prevInput == 1) ) 
	{
		fd.output = 1;
	}
	else 
	{
		fd.output = 0;
	}

	fd.prevInput = fd.input;

    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

    return(status);
}

/*********************************************************************
**	
**	rising Edge Trigger FB 정의
**
**********************************************************************/
uint32	stdRtrigInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= RTRIG_CODE;
    paraInfoPtr->inputNo	= RTRIG_SPEC_NUM;
    paraInfoPtr->intlVarNo	= RTRIG_VAR_NUM;
    paraInfoPtr->outputNo	= RTRIG_OUTPUT_NUM;

	/* 입력 - 1개 */
	*specTypePtr++			= BOOL_TYPE | SIZE32_TYPE;

	/* 내부변수 - 1개 */	
	*varTypePtr++			= BOOL_TYPE | SIZE32_TYPE;

    /* 출력 - 1개 */
	*outputTypePtr++		= BOOL_TYPE | SIZE32_TYPE;

	return (status);	 	
} 	

uint32	stdRtrigRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	strFC0213Info fd;
	
    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	/* 알고리즘 연산부 */
	if((fd.input) && (!fd.prevInput)) 
	{
		fd.output = 1;
	} 
	else 
	{
		fd.output = 0;
	}
	
	fd.prevInput = fd.input;

    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

    return(status);
}
