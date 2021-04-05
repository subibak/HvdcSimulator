/**
 * HalAo.h
 * HalTimer Device Driver Module Definition
 */

#ifndef _HALAO_H_
#define _HALAO_H_

#include "HalIfDef.h"

#include "HwDef/Dac.h"

/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{
#define HALAO_CH_NUM	8							///< 채널 개수
///@}
 

/** @name Type Definition
 * Type Definition
 */
///@{
struct stAoConfType {								///< Configuration Type 정의
	float		afAoData[HALAO_CH_NUM];
	uint32_t	ulAoCtrlReg;
	uint32_t	ulAoCfgReg;
};

typedef struct stAoConfType ST_AO_CONF_TYPE;
///@}

extern ST_AO_CONF_TYPE	t_stAoConfigData;



/**
 * @brief	초기화 함수
 * @details	Hal 기능을 초기화 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalAoInit(void);

/**
 * @brief	Analog Output 값을 쓰는 함수
 * @details	각 채널별로 값을 출력한다. 
 * @param	ulChNum : Channel 번호
 * @return	채널값
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalAoWrite(uint32_t ulChNum, float fValue);

/**
 * @brief	Analog Output 값을 전체를 쓰는 함수
 * @details	모든 채널의 값을 한 번에 출력한다. 
 * @param	ulChNum : Channel 번호
 * @param	ulValue : 출력값
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalAoHandle(void);

float HalAoRead(uint32_t ulChNum);

void HalAoWrite(uint32_t ulChNum, float fValue);


#endif
