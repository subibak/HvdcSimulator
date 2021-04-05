
/**
 * HalDio.h
 * HalDio Device Driver Module Definition
 */

#ifndef _HALDIO_H_
#define _HALDIO_H_

#include "HalIfDef.h"

#include "HwDef/Dio.h"

/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{
#define HALDIO_ERR_MEZZANINE_BOARD		0x1		///<Mezzanine Board Error
#define HALDIO_ERR_TERMINAL_BOARD		0x2		///<Terminal Board Error

#define HALDIO_BANK_NUM					4		///<Bank 개수
#define HALDIO_BANK_SIZE				8
#define HALDIO_CH_NUM					32		///<채널 개수(0~31)

#define HALDIO_ON						0x1		///<채널 On 상태
#define HALDIO_OFF						0x0		///<채널 Off 상태

#define HALDIO_OUT_DEFAULT				0x0		///<채널 출력 초기값

#define HALDIO_CH_MASK(x)				(0x1<<x)///<채널 Mask




#define HALDIO_IN_DIR					0x0
#define HALDIO_OUT_DIR					0x1

#define HALDIO_DISABLE					0x0
#define HALDIO_ENABLE					0x1

#define HALDO_CH_NUM				16
#define HALDO_CH_START				1
#define HALDO_CH_END				16

#define HALDO_CH_1					0x1



#define HALDI_CH_NUM				16
#define HALDI_CH_START				17
#define HALDI_CH_MAX				32

#define HALDI_CH_1					0x1


#define HALDIO_CH_OFFSET	2




///@}


/** @name Type Definition
 * Type Definition
 */
///@{
struct stDioBankType {		///< Bank Type 정의
	uint32_t aulBankDir;
	uint32_t aulBankEn;
};
typedef struct stDioBankType ST_DIO_BANK_TYPE;

struct stDioConfigType {	///< IO Configuration 정의
	ST_DIO_BANK_TYPE	astBank[HALDIO_BANK_NUM];
	uint32_t			ulOutValue;
	uint32_t			ulInValue;
	uint32_t			ulErrStatus;
	uint32_t			ulCtrlReg;
	uint32_t			ulCfgReg;	
};
typedef struct stDioConfigType ST_DIO_CONFIG_TYPE;

struct stDioDataType {	///< IO Configuration 정의
	uint32_t	ulDiData;
	uint32_t	ulDoData;
};
typedef struct stDioDataType ST_DIO_DATA_TYPE;
///@}


extern ST_DIO_CONFIG_TYPE	t_stDioConfig;


/**
 * @brief	Digital 값을 읽는 함수
 * @details	각 채널별로 In/Out 상태와 무관하게 값을 읽는다. 
 * @param	ulChNum : Channel 번호
 * @return	채널값
 * @date	2018-11-24
 * @version	0.0.1
 */
inline uint32_t HalDioRead(uint32_t ulChNum)
{

	uint32_t	*pstDioRead = &(t_stDioConfig.ulInValue);

#ifdef FEATURE_DIO_SEPERATE_ACCESS
	ReadReg(HWREG_DI_DATA_ADDR, pstDioRead);
#endif

	return (*pstDioRead & HALDIO_CH_MASK(ulChNum))>>ulChNum;
	
}

/**
 * @brief	Digital Output을 쓰는 함수
 * @details	각 채널별로 출력값을 쓴다(In 으로 설정되어 있으면 실제 신호에는 반영되지 않음)
 * @param	ulChNum : Channel 번호
 * 			ulOnOff : On/Off 상태
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
inline void HalDioWrite(uint32_t ulChNum, uint32_t ulOnOff)
{

	uint32_t ulRegTemp;
	uint32_t	*pstDioWrite = &(t_stDioConfig.ulOutValue);

	*pstDioWrite = *pstDioWrite & ~(HALDIO_CH_MASK(ulChNum));
	*pstDioWrite = *pstDioWrite | (ulOnOff << ulChNum);

#ifdef FEATURE_DIO_SEPERATE_ACCESS
	WriteReg(HWREG_DO_DATA_ADDR, *pstDioWrite);
#endif	
}

/**
 * @brief	Digital Input 값을 전체를 읽고 쓰는 함수
 * @details	모든 채널의 입력값과 출력값을 한 번에 처리한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */

inline void HalDioHandle(void)
{
	uint32_t	*pstDioWrite = &(t_stDioConfig.ulOutValue);
	uint32_t	*pstDioRead = &(t_stDioConfig.ulInValue);

	WriteReg(HWREG_DO_DATA_ADDR, *pstDioWrite);
	ReadReg(HWREG_DI_DATA_ADDR, pstDioRead);

}

/**
 * @brief	LED 출력을 Toggle 하는 함수
 * @details	보드의 상태 LED를 Toggle 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
inline void HalDioToggleLed(void)
{

	uint32_t	*pstDioCtrlReg =  &(t_stDioConfig.ulCtrlReg);

	ReadReg(HWREG_DIO_CTRL_ADDR, pstDioCtrlReg);

	*pstDioCtrlReg = *pstDioCtrlReg ^ (HWREG_DIO_CTRL_TERM_BLUELED_MASK|HWREG_DIO_CTRL_TERM_GREENLED_MASK);

	WriteReg(HWREG_DIO_CTRL_ADDR, *pstDioCtrlReg);

}

/**
 * @brief	초기화 함수
 * @details	Hal 기능을 초기화 한다. 모든 채널의 초기 설정으로 바꾼다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalDioInit(void);

/**
 * @brief	Mezzanine 보드와 Terminal 보드의 상태를 확인하는 함수
 * @details	Mezzanine 보드와 Terminal 보드의 상태를 확인하는 함수.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void HalDioCheckSubBoard(void);


uint32_t HalDiRead(uint32_t	ulChNum);	// ulChNum: 1~16


uint32_t HalDoRead(uint32_t	ulChNum);	// ulChNum: 1~16


void HalDoWrite(uint32_t	ulChNum, uint32_t ulOutValue);	// ulChNum: 1~16

#endif
