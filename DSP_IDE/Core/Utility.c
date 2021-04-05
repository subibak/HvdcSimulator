

/**
 * Utility.c
 * Utility Module Implementation
 */



#include "Utility.h"


uint32_t mV2Hex(float s_max, float s_min, int32_t h_max, int32_t h_min, float mV)
{
	uint32_t hex;
	hex = (uint32_t) (((h_max - h_min) / (s_max - s_min)) * (mV - s_min) + h_min);

#if 0
	PRINTF("\n");
	PRINTF("S_MAX : %f S_MIN : %f\n", s_max, s_min);
	PRINTF("H_REF : %d H_MIN : %d\n", h_max, h_min);
	PRINTF("\n");
#endif
	
	return hex;
}



float Hex2mV(float s_max, float s_min, int32_t h_max, int32_t h_min, uint32_t hex)
{
	float	mV;
	mV = ((s_max - s_min) / (h_max - h_min)) * (hex - h_min) + s_min;
#if 0
	PRINTF("\n");
	PRINTF("S_MAX : %f S_MIN : %f\n", s_max, s_min);
	PRINTF("H_REF : %d H_MIN : %d\n", h_max, h_min);
	PRINTF("\n");
#endif
	return mV;
}



uint32_t Str2ULong(char *pbString)
{
	int32_t lBase = 10;

    if (*pbString == '0' && (UPCASE(*(pbString + 1))  == 'X'))   lBase = 16;

    return ((uint32_t)strtoul(pbString, NULL, lBase));
}	
	
uint32_t EndianSwap32(uint32_t ulVal)
{
	uint32_t	ulCh[4];
	uint32_t	ulRet;
	
	ulCh[0] = ulVal&0xFF;
	ulCh[1] = ulVal&0xFF00;
	ulCh[2] = ulVal&0xFF0000;
	ulCh[3] = ulVal&0xFF000000;

	ulRet = ulCh[0]<<24 | ulCh[1]<<8 | ulCh[2]>>8 | ulCh[3]>>24;

	return ulRet;
}


