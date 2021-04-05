#include	<stdio.h>
#include 	<stdlib.h>

#include	"usrTypes.h"

#include	"fb_code\fb_user\fcPrtCommon.h"

uint32	ftOverDetect(float, float, float, float *, float);
uint32	ftUnderDetect(float, float, float, float *, float);

uint32 ftOverDetect(float val, float th, float set, float *t, float ct)
{
	uint32	flag;

	if(*t >= set)
		flag = FAULT;
	else
		flag = NORMAL;		

	if(flag == NORMAL)
	{
		if(val >= th)
			*t += ct;
		else
			*t = ZERO;		
	}
	return flag; 	
}

uint32 ftUnderDetect(float val, float th, float set, float *t, float ct)
{
	uint32	flag;

	if(*t >= set)
		flag = FAULT;
	else
		flag = NORMAL;		
			
	if(flag == NORMAL)
	{
		if(val <= th)
			*t += ct;
		else
			*t = ZERO;		
	}
	
	return flag; 	
}