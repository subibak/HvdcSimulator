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
#define HALAO_CH_NUM	8							///< ä�� ����
///@}
 

/** @name Type Definition
 * Type Definition
 */
///@{
struct stAoConfType {								///< Configuration Type ����
	float		afAoData[HALAO_CH_NUM];
	uint32_t	ulAoCtrlReg;
	uint32_t	ulAoCfgReg;
};

typedef struct stAoConfType ST_AO_CONF_TYPE;
///@}

extern ST_AO_CONF_TYPE	t_stAoConfigData;



/**
 * @brief	�ʱ�ȭ �Լ�
 * @details	Hal ����� �ʱ�ȭ �Ѵ�.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalAoInit(void);

/**
 * @brief	Analog Output ���� ���� �Լ�
 * @details	�� ä�κ��� ���� ����Ѵ�. 
 * @param	ulChNum : Channel ��ȣ
 * @return	ä�ΰ�
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalAoWrite(uint32_t ulChNum, float fValue);

/**
 * @brief	Analog Output ���� ��ü�� ���� �Լ�
 * @details	��� ä���� ���� �� ���� ����Ѵ�. 
 * @param	ulChNum : Channel ��ȣ
 * @param	ulValue : ��°�
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalAoHandle(void);

float HalAoRead(uint32_t ulChNum);

void HalAoWrite(uint32_t ulChNum, float fValue);


#endif
