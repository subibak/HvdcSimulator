

/**
 * IalDiag.h
 * Signal Interface Module Definition
 */

#ifndef _IALDIAG_H_
#define _IALDIAG_H_

#include "IalIfDef.h"

/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{
#define IALDIAG_ERR_NONE				0x0

#define IALDIAG_ERR_FCOM1_LINK			0x1
#define IALDIAG_ERR_FCOM1_TIMEOUT		0x2
#define IALDIAG_ERR_FCOM1_FRAME			0x4
#define IALDIAG_ERR_FCOM2_LINK			0x8
#define IALDIAG_ERR_FCOM2_TIMEOUT		0x10
#define IALDIAG_ERR_FCOM2_FRAME			0x20
#define IALDIAG_ERR_FCOM3_LINK			0x40
#define IALDIAG_ERR_FCOM3_TIMEOUT		0x80
#define IALDIAG_ERR_FCOM3_FRAME			0x100
#define IALDIAG_ERR_FCOM4_LINK			0x200
#define IALDIAG_ERR_FCOM4_TIMEOUT		0x400
#define IALDIAG_ERR_FCOM4_FRAME			0x800

#define IALDIAG_ERR_FCOMM_MASK			0xFFF

#define IALDIAG_ERR_TASK_OVERTIME		0x1000			///< Task OverTime
#define IALDIAG_ERR_MBRD_DISC			0x2000			///< Mezzanine Board Disconnect

// 2020. 03.28 : SyncLost 는 Ch1 Timeout 으로 Mapping 하도록 확인함.
// Latch 하지 않고 적용 후, 보드 LED와 연계하고, Console Msg 연계하여 SyncLost 체크
#define IALDIAG_ERR_LOST_SYNC			0x4002			///< Sync Signal Lost

#define IALDIAG_FAULT_NONE				0x0
#define IALDIAG_FAULT_STATE				0x1


#define IALDIAG_ERR_TOOMUCH_IODLY		0x10000000			///< Sync Signal Lost
#define IALDIAG_ERR_FLT_RESET			0x20000000
#define IALDIAG_LIVECNT_MAX				0xFFFF0000


///@}






/** @name Type Definition
 * Type Definition
 */
///@{
#if 0
typedef union HalVmeBody {
	uint32_t	ulData[HALVME_BODY_SIZE];
	float		fData[HALVME_BODY_SIZE];
} HALVME_BODY_TYPE;


typedef struct HalVmeDataType {
	uint32_t			ulLiveCnt;			///> 
	HALVME_BODY_TYPE	uBody;
} HALVME_DATA_TYPE;
#endif

typedef struct {
	uint32_t	ulSlaveReadyFlag;
	uint32_t	ulBoardId;
	uint32_t	ulSlaveLiveCnt;
	uint32_t	ulReserved1[5];
	uint32_t	ulMasterReadyFlag;
	uint32_t	ulMasterLiveCnt;
	uint32_t	ulReserved2[6];
} IALDIAG_HEADER;

#define IALDIAG_CH_1				0
#define IALDIAG_CH_2				1
#define IALDIAG_CH_3				2
#define IALDIAG_CH_4				3

#define IALDIAG_ERR_YES		0x1

#define IALDIAG_CH_OFFSET			3
#define IALDIAG_ERR(ch, err)		(err<<(ch*IALDIAG_CH_OFFSET))

typedef struct {
	uint32_t	ulDiag;
} IALDIAG_INFO;

///@}



//#define SIGNAL_REQUEST_BUS	0x8


/**
 * @brief	Ial Moduel 을 초기화하는 함수 
 * @details	Ial 변수 및 관련 Hal의 초기화 한다.
 * @param	N/A
 * @return	N/A
 * @date	2018-11-24
 * @version	0.0.1
 */
void IalDiagInit(void);
void IalDiagRun(void);

uint32_t IalDiagFCommCheck(void);

void IalDiagBPCheck(void);

void IalDiagBrdCheck(void);

void IalDiagSet(uint32_t ulSts);
uint32_t IalDiagGet(void);
void IalDiagUpdateLiveCnt(void);

extern IALDIAG_HEADER	*pstDiagHeader;
extern IALDIAG_INFO	*pstDiagInfo;

extern uint32_t	ulBPLiveCnt;
extern volatile uint32_t	ulDiagSt;

extern uint32_t ulCtrlDiagSt;


#endif


