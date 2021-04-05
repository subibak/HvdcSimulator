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
#define HALAI_CH_NUM	16			///< ä�� ����
///@}
 

/** @name Type Definition
 * Type Definition
 */
///@{
struct stAiConfType {				///< Configuration Type ����
	float	 afAiData[HALAI_CH_NUM];
	uint32_t aulAiData[HALAI_CH_NUM];
	uint32_t ulAiConfig;
};

typedef struct stAiConfType ST_AI_CONF_TYPE;
///@}

extern ST_AI_CONF_TYPE	t_stAiConfigData;

/**
 * @brief	�ʱ�ȭ �Լ�
 * @details	Hal ����� �ʱ�ȭ �Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalAiInit(void);

/**
 * @brief	Analog Input ���� �д� �Լ�
 * @details	�� ä�κ��� �Է°��� �д´�. 
 * @param	ulChNum : Channel ��ȣ
 * @return	ä�ΰ�
 * @date	2018-11-24
 * @version	0.0.1
 */
float HalAiRead(uint32_t	ulChNum);

/**
 * @brief	Analog Input ���� ��ü�� �д� �Լ�
 * @details	��� ä���� �Է°��� �� ���� �д´�. 
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalAiHandle(void);



#endif
