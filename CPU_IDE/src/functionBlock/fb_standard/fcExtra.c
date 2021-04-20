#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fb_code\fb_standard\fcExtra.h"

/**************************************************************************
**		외부 함수 
**************************************************************************/
extern uint32 readRuntimeFbData(uint32,uint32,uint32,uint32 *);
extern uint32 fbCodeParaWriteWithVar(uint32,uint32, uint32, uint32 *,uint8 *);
extern void	  setErrorCode(uint8 *, uint32, uint8 *, uint32);

extern uint32 fbInOutputTypeCasting(uint32, uint32, uint32, uint32 *);
extern uint32 fbCodeVarWriteEnoRead(uint32 , uint32 , uint32* );

/*********************************************************************
**	
**	기존 FB의 출력에 변수를 연결할 때 editor 컴파일 시에 FB가 생성됨 
**
**********************************************************************/
uint32	wrVarInitFunc
				(
			uint32	*specTypePtr,			
            uint32	*varTypePtr,			
            uint32	*outputTypePtr,			 
            strFbDefInfo	*paraInfoPtr	
				)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= WRVAR_CODE;
    paraInfoPtr->inputNo	= WRVAR_SPEC_NUM;
    paraInfoPtr->intlVarNo	= WRVAR_VAR_NUM;
    paraInfoPtr->outputNo	= WRVAR_OUTPUT_NUM;

	
	*specTypePtr++			= ANY_TYPE | SIZE32_TYPE ;	
	*specTypePtr++			= ANY_TYPE | SIZE32_TYPE ;	

			

    
	*outputTypePtr++		= ANY_TYPE | SIZE32_TYPE ;	

    return(status);
};


uint32	wrVarRunFunc
				(
			uint32 taskId,		
			uint32 fbBlockAddr	
				)
{
	uint32	status 		= NO_ERROR;	
	uint32  wrVarEno 	= 1; 
	uint32	i;
	uint8   fv[WRVAR_SPEC_NUM];

	strFC0681Info	fd;			

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

	for(i = 0; i < WRVAR_SPEC_NUM; i++)
		fv[i]=0;
	
    status = fbCodeVarWriteEnoRead(	taskId, 
    								fbBlockAddr,
                                	(uint32 *)&wrVarEno
						   		);
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

	if(fd.in1 != fd.in2) 
	{
		fv[1] = TRUE;
	}		
	else 
	{
		fv[1] = FALSE;
	}

	fd.in2 = fd.in1;
	fd.out = fd.in2;

    status = fbCodeParaWriteWithVar(	taskId,
		    							LOGIC_ID,
		                                fbBlockAddr,
		                                (uint32 *)&fd,
										(uint8  *)&fv
								     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }

    return(status);
}