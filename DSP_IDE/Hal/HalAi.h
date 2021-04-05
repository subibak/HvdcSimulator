/**
 * HalAi.h
 * HalAi Device Driver Module Definition
 */

#ifndef _HALAI_H_
#define _HALAI_H_

#include "HalIfDef.h"

#include "Hwdef/Adc.h"

/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{
#define HALAI_CH_NUM	16			///< 채널 개수
///@}
 

/** @name Type Definition
 * Type Definition
 */
///@{
struct stAiConfType {				///< Configuration Type 정의
	float	 afAiData[HALAI_CH_NUM];
	uint32_t aulAiData[HALAI_CH_NUM];
	uint32_t ulAiConfig;
};

typedef struct stAiConfType ST_AI_CONF_TYPE;
///@}

extern ST_AI_CONF_TYPE	t_stAiConfigData;

/**
 * @brief	초기화 함수
 * @details	Hal 기능을 초기화 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalAiInit(void);

/**
 * @brief	Analog Input 값을 읽는 함수
 * @details	각 채널별로 입력값을 읽는다. 
 * @param	ulChNum : Channel 번호
 * @return	채널값
 * @date	2018-11-24
 * @version	0.0.1
 */
float HalAiRead(uint32_t	ulChNum);

/**
 * @brief	Analog Input 값을 전체를 읽는 함수
 * @details	모든 채널의 입력값을 한 번에 읽는다. 
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalAiHandle(void);



#endif
