#include	<stdio.h>
#include 	<stdlib.h>

#include	"usrTypes.h"
#include	"fb_code\fb_user\fbProtection.h"

uint32 ftOverDetect(float val, float th, float set, float t, float Cycletime)
{
	uint32	flag;
	
	if(t >= set)
	{
		flag = 1;
	}
	else
	{
		if(val > th)
			t += Cycletime;
		else
			t = 0;		
		
		flag = 0;			
	}
	
	return flag; 	
}

uint32 ftUnderDetect(float val, float th, float set, float t, float Cycletime)
{
	uint32	flag;
	
	if(t >= set)
	{
		flag = 1;
	}
	else
	{
		if(val < th)
			t += Cycletime;
		else
			t = 0;		
		
		flag = 0;			
	}
	
	return flag; 	
}