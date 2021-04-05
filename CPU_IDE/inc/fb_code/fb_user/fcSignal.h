#ifndef FCSIGNAL_H
#define FCSIGNAL_H

/**************************************************************************
**		함수 Prototype
**************************************************************************/
extern uint32	rampInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	rampRunFunc(uint32, uint32);

/*********************************************************************
**	RAMP FB 	
**********************************************************************/
#define	RAMP_INIT		0
#define	RAMP_RUN		1

#define	RAMP_CODE				450
#define	RAMP_SPEC_NUM			4
#define	RAMP_VAR_NUM			0	
#define	RAMP_OUTPUT_NUM			2

typedef struct {
	/* 입력 */
	uint32	rampRun;			/* 0:rampInitVal, 1: Ramp to rampSetVal */
	float	rampInitVal;
	float	rampSetVal;
	float	rampRateValPerSec;		/* ramp rate per second */
	
	/* 내부 변수 */
	
	/* 출력 */
	uint32	rampBusy;			/* 1 : ramp duration Time 기동, 즉 rampRun = 1때 */
	float	rampOut;			/* rampRun == 0 일때 initVal 그외는 ramp 연산값 */  
}strFC0450Info;

#endif

