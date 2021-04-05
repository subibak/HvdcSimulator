
/**
 * Utility.h
 * Utility Module Definition
 */
 
#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "Core.h"

typedef struct {
	uint32_t	ulData;
	char		*pString;
} STR_MAP_TYPE;

uint32_t mV2Hex(float s_max, float s_min, int32_t h_max, int32_t h_min, float mV);

float Hex2mV(float s_max, float s_min, int32_t h_max, int32_t h_min, uint32_t hex);

uint32_t Str2ULong(char *pbString);

uint32_t EndianSwap32(uint32_t ulVal);

#define UPCASE(x) ((x)>='a' && (x)<='z')?(x)-('z'-'Z'):x

#endif

