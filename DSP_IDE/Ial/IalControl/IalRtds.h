
/**
 * IalVbeu.h
 * IalVbeu Interface Module Definition
 */

#ifndef _IALRTDS_H_
#define _IALRTDS_H_


#include "Ial/IalIfdef.h"

/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{

// Control Input, Output 은 채널별로 인덱스가 정의되어 있다.
// 기본 정의에 맞게 Idx를 정의하여야 한다.
// 0          ~ 15         : Analog Input
// 16         ~ 15 + 200*1 : FComm #1
// 16 + 200*1 ~ 15 + 200*2 : FComm #1
// 16 + 200*2 ~ 15 + 200*3 : FComm #1
// 16 + 200*3 ~ 15 + 200*4 : FComm #1


//	#define IALRTDS_FCOMM_DUMMY_SIZE		5

#define IALRTDS_FCOMM_SIZE			400

#define IALRTDS_FCOMM_CH1_START		HALAI_CH_NUM										// to MES with only HDR
#define IALRTDS_FCOMM_CH2_START		(IALRTDS_FCOMM_CH1_START + IALRTDS_FCOMM_SIZE)		// to RTDSU A with small Data
#define IALRTDS_FCOMM_CH3_START		(IALRTDS_FCOMM_CH2_START + IALRTDS_FCOMM_SIZE)		// to RTDS Upper
#define IALRTDS_FCOMM_CH4_START		(IALRTDS_FCOMM_CH3_START + IALRTDS_FCOMM_SIZE)		// to RTDS Lower

// 시스템 사양에 맞춰 변경.


#define SM_LEVEL					100				// Fix 해야함.
#define TYPEA_SIZE					13				// Header(1) + 12
#define TYPEB_SIZE					(SM_LEVEL+1)
#define TYPEC_SIZE					(SM_LEVEL/2+1)

#define IALRTDS_FCOMM_TYPEA_OFFSET		0							// Size 
#define IALRTDS_FCOMM_TYPEA_BODY_OFFSET	(IALRTDS_FCOMM_TYPEA_OFFSET+1)							// Size 
#define IALRTDS_FCOMM_TYPEB_OFFSET		TYPEA_SIZE					// Size Submodule 수 + 1: 100 기준 101
#define IALRTDS_FCOMM_TYPEB_BODY_OFFSET	(IALRTDS_FCOMM_TYPEB_OFFSET+1)					// Size Submodule 수 + 1: 100 기준 101
#define IALRTDS_FCOMM_TYPEC_OFFSET		(TYPEA_SIZE+TYPEB_SIZE)		// Size 
#define IALRTDS_FCOMM_TYPEC_BODY_OFFSET	(IALRTDS_FCOMM_TYPEC_OFFSET+1)		// Size 

#define IALRTDS_TX_SIZE			165
#define IALRTDS_RX_SIZE			26

// Compact Type
#define IALRTDS_TX_HEADER_SMNUM_MASK	0xFFF00000		// Submodule 개수 use in Type B, C
#define IALRTDS_TX_HEADER_SMNUM_SHIFT	20

#define IALRTDS_TX_HEADER_PKTTYPE_MASK	0x000F0000		// 0: A, 1: B, 2: C
#define IALRTDS_TX_HEADER_PKTTYPE_SHIFT	16

#define IALRTDS_TX_HEADER_LEGNUM_MASK	0x000000F0		// 1,2 Upper Lower 만 확인 ?
#define IALRTDS_TX_HEADER_LEGNUM_SHIFT	4

#define IALRTDS_TX_HEADER_SEQNUM_MASK	0x0000000F		// always 1 in Compact Mode
#define IALRTDS_TX_HEADER_SEQNUM_SHIFT	0

// Type C, Status Field
#define IALRTDS_STS_MASK				0XFFFF
#define IALRTDS_STS_SHIFT				16
#define IALRTDS_STS_T1OC_MASK			0x0001
#define IALRTDS_STS_T1SC_MASK			0x0002
#define IALRTDS_STS_T2OC_MASK			0x0004
#define IALRTDS_STS_T2SC_MASK			0x0008
#define IALRTDS_STS_T3OC_MASK			0x0010
#define IALRTDS_STS_T3SC_MASK			0x0020
#define IALRTDS_STS_T4OC_MASK			0x0040
#define IALRTDS_STS_T4SC_MASK			0x0080
#define IALRTDS_STS_BRK_MASK			0x0100
#define IALRTDS_STS_DEAC_MASK			0x0200
#define IALRTDS_STS_OV1_MASK			0x0400
#define IALRTDS_STS_OV2_MASK			0x0800
#define IALRTDS_STS_UV_MASK`			0x1000




#if 0
/****************************************************************************/
/* RTDS 의 송신 데이터로 수신부(VBC)에서는 아래의 Idx를 사용해야 한다.            */
/****************************************************************************/

// Type A
#define IALRTDS_UP_A_HEADER			(IALRTDS_FCOMM_CH3_START)
#define IALRTDS_UP_A_V_BRANCH			(IALRTDS_UP_A_HEADER + 1)
#define IALRTDS_UP_A_I_BRANCH			(IALRTDS_UP_A_HEADER + 2)
#define IALRTDS_UP_A_V_VALVES			(IALRTDS_UP_A_HEADER + 3)
#define IALRTDS_UP_A_RESERVED1			(IALRTDS_UP_A_HEADER + 4)
#define IALRTDS_UP_A_RESERVED2			(IALRTDS_UP_A_HEADER + 5)
#define IALRTDS_UP_A_RESERVED3			(IALRTDS_UP_A_HEADER + 6)
#define IALRTDS_UP_A_PASSTHRU1			(IALRTDS_UP_A_HEADER + 7)
#define IALRTDS_UP_A_PASSTHRU2			(IALRTDS_UP_A_HEADER + 8)
#define IALRTDS_UP_A_PASSTHRU3			(IALRTDS_UP_A_HEADER + 9)
#define IALRTDS_UP_A_PASSTHRU4			(IALRTDS_UP_A_HEADER + 10)
#define IALRTDS_UP_A_PASSTHRU5			(IALRTDS_UP_A_HEADER + 11)
#define IALRTDS_UP_A_PASSTHRU6			(IALRTDS_UP_A_HEADER + 12)
#define IALRTDS_UP_A_LAST				IALRTDS_UP_A_PASSTHRU6

// Type B
#define IALRTDS_UP_B_HEADER			(IALRTDS_UP_A_HEADER + 13)
#define IALRTDS_UP_B_CAPV_001			(IALRTDS_UP_A_HEADER + 14)
#define IALRTDS_UP_B_CAPV_002			(IALRTDS_UP_A_HEADER + 15)
#define IALRTDS_UP_B_CAPV_003			(IALRTDS_UP_A_HEADER + 16)
#define IALRTDS_UP_B_CAPV_004			(IALRTDS_UP_A_HEADER + 17)
#define IALRTDS_UP_B_CAPV_005			(IALRTDS_UP_A_HEADER + 18)
#define IALRTDS_UP_B_CAPV_006			(IALRTDS_UP_A_HEADER + 19)
#define IALRTDS_UP_B_CAPV_007			(IALRTDS_UP_A_HEADER + 20)
#define IALRTDS_UP_B_CAPV_008			(IALRTDS_UP_A_HEADER + 21)
#define IALRTDS_UP_B_CAPV_009			(IALRTDS_UP_A_HEADER + 22)
#define IALRTDS_UP_B_CAPV_010			(IALRTDS_UP_A_HEADER + 23)
#define IALRTDS_UP_B_CAPV_011			(IALRTDS_UP_A_HEADER + 24)
#define IALRTDS_UP_B_CAPV_012			(IALRTDS_UP_A_HEADER + 25)
#define IALRTDS_UP_B_CAPV_013			(IALRTDS_UP_A_HEADER + 26)
#define IALRTDS_UP_B_CAPV_014			(IALRTDS_UP_A_HEADER + 27)
#define IALRTDS_UP_B_CAPV_015			(IALRTDS_UP_A_HEADER + 28)
#define IALRTDS_UP_B_CAPV_016			(IALRTDS_UP_A_HEADER + 29)
#define IALRTDS_UP_B_CAPV_017			(IALRTDS_UP_A_HEADER + 30)
#define IALRTDS_UP_B_CAPV_018			(IALRTDS_UP_A_HEADER + 31)
#define IALRTDS_UP_B_CAPV_019			(IALRTDS_UP_A_HEADER + 32)
#define IALRTDS_UP_B_CAPV_020			(IALRTDS_UP_A_HEADER + 33)
#define IALRTDS_UP_B_CAPV_021			(IALRTDS_UP_A_HEADER + 34)
#define IALRTDS_UP_B_CAPV_022			(IALRTDS_UP_A_HEADER + 35)
#define IALRTDS_UP_B_CAPV_023			(IALRTDS_UP_A_HEADER + 36)
#define IALRTDS_UP_B_CAPV_024			(IALRTDS_UP_A_HEADER + 37)
#define IALRTDS_UP_B_CAPV_025			(IALRTDS_UP_A_HEADER + 38)
#define IALRTDS_UP_B_CAPV_026			(IALRTDS_UP_A_HEADER + 39) 
#define IALRTDS_UP_B_CAPV_027			(IALRTDS_UP_A_HEADER + 40)
#define IALRTDS_UP_B_CAPV_028			(IALRTDS_UP_A_HEADER + 41)
#define IALRTDS_UP_B_CAPV_029			(IALRTDS_UP_A_HEADER + 42)
#define IALRTDS_UP_B_CAPV_030			(IALRTDS_UP_A_HEADER + 43)
#define IALRTDS_UP_B_CAPV_031			(IALRTDS_UP_A_HEADER + 44)
#define IALRTDS_UP_B_CAPV_032			(IALRTDS_UP_A_HEADER + 45)
#define IALRTDS_UP_B_CAPV_033			(IALRTDS_UP_A_HEADER + 46)
#define IALRTDS_UP_B_CAPV_034			(IALRTDS_UP_A_HEADER + 47)
#define IALRTDS_UP_B_CAPV_035			(IALRTDS_UP_A_HEADER + 48)
#define IALRTDS_UP_B_CAPV_036			(IALRTDS_UP_A_HEADER + 49) 
#define IALRTDS_UP_B_CAPV_037			(IALRTDS_UP_A_HEADER + 50)
#define IALRTDS_UP_B_CAPV_038			(IALRTDS_UP_A_HEADER + 51)
#define IALRTDS_UP_B_CAPV_039			(IALRTDS_UP_A_HEADER + 52)
#define IALRTDS_UP_B_CAPV_040			(IALRTDS_UP_A_HEADER + 53)
#define IALRTDS_UP_B_CAPV_041			(IALRTDS_UP_A_HEADER + 54)
#define IALRTDS_UP_B_CAPV_042			(IALRTDS_UP_A_HEADER + 55)
#define IALRTDS_UP_B_CAPV_043			(IALRTDS_UP_A_HEADER + 56)
#define IALRTDS_UP_B_CAPV_044			(IALRTDS_UP_A_HEADER + 57)
#define IALRTDS_UP_B_CAPV_045			(IALRTDS_UP_A_HEADER + 58)
#define IALRTDS_UP_B_CAPV_046			(IALRTDS_UP_A_HEADER + 59) 
#define IALRTDS_UP_B_CAPV_047			(IALRTDS_UP_A_HEADER + 60)
#define IALRTDS_UP_B_CAPV_048			(IALRTDS_UP_A_HEADER + 61)
#define IALRTDS_UP_B_CAPV_049			(IALRTDS_UP_A_HEADER + 62)
#define IALRTDS_UP_B_CAPV_050			(IALRTDS_UP_A_HEADER + 63)
#define IALRTDS_UP_B_CAPV_051			(IALRTDS_UP_A_HEADER + 64)
#define IALRTDS_UP_B_CAPV_052			(IALRTDS_UP_A_HEADER + 65)
#define IALRTDS_UP_B_CAPV_053			(IALRTDS_UP_A_HEADER + 66)
#define IALRTDS_UP_B_CAPV_054			(IALRTDS_UP_A_HEADER + 67)
#define IALRTDS_UP_B_CAPV_055			(IALRTDS_UP_A_HEADER + 68)
#define IALRTDS_UP_B_CAPV_056			(IALRTDS_UP_A_HEADER + 69) 
#define IALRTDS_UP_B_CAPV_057			(IALRTDS_UP_A_HEADER + 70)
#define IALRTDS_UP_B_CAPV_058			(IALRTDS_UP_A_HEADER + 71)
#define IALRTDS_UP_B_CAPV_059			(IALRTDS_UP_A_HEADER + 72)
#define IALRTDS_UP_B_CAPV_060			(IALRTDS_UP_A_HEADER + 73)
#define IALRTDS_UP_B_CAPV_061			(IALRTDS_UP_A_HEADER + 74)
#define IALRTDS_UP_B_CAPV_062			(IALRTDS_UP_A_HEADER + 75)
#define IALRTDS_UP_B_CAPV_063			(IALRTDS_UP_A_HEADER + 76)
#define IALRTDS_UP_B_CAPV_064			(IALRTDS_UP_A_HEADER + 77)
#define IALRTDS_UP_B_CAPV_065			(IALRTDS_UP_A_HEADER + 78)
#define IALRTDS_UP_B_CAPV_066			(IALRTDS_UP_A_HEADER + 79) 
#define IALRTDS_UP_B_CAPV_067			(IALRTDS_UP_A_HEADER + 80)
#define IALRTDS_UP_B_CAPV_068			(IALRTDS_UP_A_HEADER + 81)
#define IALRTDS_UP_B_CAPV_069			(IALRTDS_UP_A_HEADER + 82)
#define IALRTDS_UP_B_CAPV_070			(IALRTDS_UP_A_HEADER + 83)
#define IALRTDS_UP_B_CAPV_071			(IALRTDS_UP_A_HEADER + 84)
#define IALRTDS_UP_B_CAPV_072			(IALRTDS_UP_A_HEADER + 85)
#define IALRTDS_UP_B_CAPV_073			(IALRTDS_UP_A_HEADER + 86)
#define IALRTDS_UP_B_CAPV_074			(IALRTDS_UP_A_HEADER + 87)
#define IALRTDS_UP_B_CAPV_075			(IALRTDS_UP_A_HEADER + 88)
#define IALRTDS_UP_B_CAPV_076			(IALRTDS_UP_A_HEADER + 89) 
#define IALRTDS_UP_B_CAPV_077			(IALRTDS_UP_A_HEADER + 90)
#define IALRTDS_UP_B_CAPV_078			(IALRTDS_UP_A_HEADER + 91)
#define IALRTDS_UP_B_CAPV_079			(IALRTDS_UP_A_HEADER + 92)
#define IALRTDS_UP_B_CAPV_080			(IALRTDS_UP_A_HEADER + 93)
#define IALRTDS_UP_B_CAPV_081			(IALRTDS_UP_A_HEADER + 94)
#define IALRTDS_UP_B_CAPV_082			(IALRTDS_UP_A_HEADER + 95)
#define IALRTDS_UP_B_CAPV_083			(IALRTDS_UP_A_HEADER + 96)
#define IALRTDS_UP_B_CAPV_084			(IALRTDS_UP_A_HEADER + 97)
#define IALRTDS_UP_B_CAPV_085			(IALRTDS_UP_A_HEADER + 98)
#define IALRTDS_UP_B_CAPV_086			(IALRTDS_UP_A_HEADER + 99) 
#define IALRTDS_UP_B_CAPV_087			(IALRTDS_UP_A_HEADER + 100)
#define IALRTDS_UP_B_CAPV_088			(IALRTDS_UP_A_HEADER + 101)
#define IALRTDS_UP_B_CAPV_089			(IALRTDS_UP_A_HEADER + 102)
#define IALRTDS_UP_B_CAPV_090			(IALRTDS_UP_A_HEADER + 103)
#define IALRTDS_UP_B_CAPV_091			(IALRTDS_UP_A_HEADER + 104)
#define IALRTDS_UP_B_CAPV_092			(IALRTDS_UP_A_HEADER + 105)
#define IALRTDS_UP_B_CAPV_093			(IALRTDS_UP_A_HEADER + 106)
#define IALRTDS_UP_B_CAPV_094			(IALRTDS_UP_A_HEADER + 107)
#define IALRTDS_UP_B_CAPV_095			(IALRTDS_UP_A_HEADER + 108)
#define IALRTDS_UP_B_CAPV_096			(IALRTDS_UP_A_HEADER + 109)
#define IALRTDS_UP_B_CAPV_097			(IALRTDS_UP_A_HEADER + 110)
#define IALRTDS_UP_B_CAPV_098			(IALRTDS_UP_A_HEADER + 111)
#define IALRTDS_UP_B_CAPV_099			(IALRTDS_UP_A_HEADER + 112)
#define IALRTDS_UP_B_CAPV_100			(IALRTDS_UP_A_HEADER + 113)
#define IALRTDS_UP_B_LAST				IALRTDS_UP_B_CAPV_100

// Type C
#define IALRTDS_UP_C_HEADER			(IALRTDS_UP_B_LAST + 1)
#define IALRTDS_UP_C_002_01_STS		(IALRTDS_UP_C_HEADER + 1)
#define IALRTDS_UP_C_004_03_STS		(IALRTDS_UP_C_HEADER + 2)
#define IALRTDS_UP_C_006_05_STS		(IALRTDS_UP_C_HEADER + 3)
#define IALRTDS_UP_C_008_07_STS		(IALRTDS_UP_C_HEADER + 4)
#define IALRTDS_UP_C_010_09_STS		(IALRTDS_UP_C_HEADER + 5)
#define IALRTDS_UP_C_012_11_STS		(IALRTDS_UP_C_HEADER + 6)
#define IALRTDS_UP_C_014_13_STS		(IALRTDS_UP_C_HEADER + 7)
#define IALRTDS_UP_C_016_15_STS		(IALRTDS_UP_C_HEADER + 8)
#define IALRTDS_UP_C_018_17_STS		(IALRTDS_UP_C_HEADER + 9)
#define IALRTDS_UP_C_020_19_STS		(IALRTDS_UP_C_HEADER + 10)
#define IALRTDS_UP_C_022_21_STS		(IALRTDS_UP_C_HEADER + 11)
#define IALRTDS_UP_C_024_23_STS		(IALRTDS_UP_C_HEADER + 12)
#define IALRTDS_UP_C_026_25_STS		(IALRTDS_UP_C_HEADER + 13)
#define IALRTDS_UP_C_028_27_STS		(IALRTDS_UP_C_HEADER + 14)
#define IALRTDS_UP_C_030_29_STS		(IALRTDS_UP_C_HEADER + 15)
#define IALRTDS_UP_C_032_31_STS		(IALRTDS_UP_C_HEADER + 16)
#define IALRTDS_UP_C_034_33_STS		(IALRTDS_UP_C_HEADER + 17)
#define IALRTDS_UP_C_036_35_STS		(IALRTDS_UP_C_HEADER + 18)
#define IALRTDS_UP_C_038_37_STS		(IALRTDS_UP_C_HEADER + 19)
#define IALRTDS_UP_C_040_39_STS		(IALRTDS_UP_C_HEADER + 20)
#define IALRTDS_UP_C_042_41_STS		(IALRTDS_UP_C_HEADER + 21)
#define IALRTDS_UP_C_044_43_STS		(IALRTDS_UP_C_HEADER + 22)
#define IALRTDS_UP_C_046_45_STS		(IALRTDS_UP_C_HEADER + 23)
#define IALRTDS_UP_C_048_47_STS		(IALRTDS_UP_C_HEADER + 24)
#define IALRTDS_UP_C_050_49_STS		(IALRTDS_UP_C_HEADER + 25)
#define IALRTDS_UP_C_052_51_STS		(IALRTDS_UP_C_HEADER + 26)
#define IALRTDS_UP_C_054_53_STS		(IALRTDS_UP_C_HEADER + 27)
#define IALRTDS_UP_C_056_55_STS		(IALRTDS_UP_C_HEADER + 28)
#define IALRTDS_UP_C_058_57_STS		(IALRTDS_UP_C_HEADER + 29)
#define IALRTDS_UP_C_060_59_STS		(IALRTDS_UP_C_HEADER + 30)
#define IALRTDS_UP_C_062_61_STS		(IALRTDS_UP_C_HEADER + 31)
#define IALRTDS_UP_C_064_63_STS		(IALRTDS_UP_C_HEADER + 32)
#define IALRTDS_UP_C_066_65_STS		(IALRTDS_UP_C_HEADER + 33)
#define IALRTDS_UP_C_068_67_STS		(IALRTDS_UP_C_HEADER + 34)
#define IALRTDS_UP_C_070_69_STS		(IALRTDS_UP_C_HEADER + 35)
#define IALRTDS_UP_C_072_71_STS		(IALRTDS_UP_C_HEADER + 36)
#define IALRTDS_UP_C_074_73_STS		(IALRTDS_UP_C_HEADER + 37)
#define IALRTDS_UP_C_076_75_STS		(IALRTDS_UP_C_HEADER + 38)
#define IALRTDS_UP_C_078_77_STS		(IALRTDS_UP_C_HEADER + 39)
#define IALRTDS_UP_C_080_79_STS		(IALRTDS_UP_C_HEADER + 40)
#define IALRTDS_UP_C_082_81_STS		(IALRTDS_UP_C_HEADER + 41)
#define IALRTDS_UP_C_084_83_STS		(IALRTDS_UP_C_HEADER + 42)
#define IALRTDS_UP_C_086_85_STS		(IALRTDS_UP_C_HEADER + 43)
#define IALRTDS_UP_C_088_87_STS		(IALRTDS_UP_C_HEADER + 44)
#define IALRTDS_UP_C_090_89_STS		(IALRTDS_UP_C_HEADER + 45)
#define IALRTDS_UP_C_092_91_STS		(IALRTDS_UP_C_HEADER + 46)
#define IALRTDS_UP_C_094_93_STS		(IALRTDS_UP_C_HEADER + 47)
#define IALRTDS_UP_C_096_95_STS		(IALRTDS_UP_C_HEADER + 48)
#define IALRTDS_UP_C_098_97_STS		(IALRTDS_UP_C_HEADER + 49)
#define IALRTDS_UP_C_100_99_STS		(IALRTDS_UP_C_HEADER + 50)


// Lower Leg Data

// Type A
#define IALRTDS_LO_A_HEADER			(IALRTDS_FCOMM_CH4_START)
#define IALRTDS_LO_A_V_BRANCH			(IALRTDS_LO_A_HEADER + 1)
#define IALRTDS_LO_A_I_BRANCH			(IALRTDS_LO_A_HEADER + 2)
#define IALRTDS_LO_A_V_VALVES			(IALRTDS_LO_A_HEADER + 3)
#define IALRTDS_LO_A_RESERVED1			(IALRTDS_LO_A_HEADER + 4)
#define IALRTDS_LO_A_RESERVED2			(IALRTDS_LO_A_HEADER + 5)
#define IALRTDS_LO_A_RESERVED3			(IALRTDS_LO_A_HEADER + 6)
#define IALRTDS_LO_A_PASSTHRU1			(IALRTDS_LO_A_HEADER + 7)
#define IALRTDS_LO_A_PASSTHRU2			(IALRTDS_LO_A_HEADER + 8)
#define IALRTDS_LO_A_PASSTHRU3			(IALRTDS_LO_A_HEADER + 9)
#define IALRTDS_LO_A_PASSTHRU4			(IALRTDS_LO_A_HEADER + 10)
#define IALRTDS_LO_A_PASSTHRU5			(IALRTDS_LO_A_HEADER + 11)
#define IALRTDS_LO_A_PASSTHRU6			(IALRTDS_LO_A_HEADER + 12)
#define IALRTDS_LO_A_LAST				IALRTDS_LO_A_PASSTHRU6

// Type B
#define IALRTDS_LO_B_HEADER			(IALRTDS_LO_A_HEADER + 13)
#define IALRTDS_LO_B_CAPV_001			(IALRTDS_LO_A_HEADER + 14)
#define IALRTDS_LO_B_CAPV_002			(IALRTDS_LO_A_HEADER + 15)
#define IALRTDS_LO_B_CAPV_003			(IALRTDS_LO_A_HEADER + 16)
#define IALRTDS_LO_B_CAPV_004			(IALRTDS_LO_A_HEADER + 17)
#define IALRTDS_LO_B_CAPV_005			(IALRTDS_LO_A_HEADER + 18)
#define IALRTDS_LO_B_CAPV_006			(IALRTDS_LO_A_HEADER + 19)
#define IALRTDS_LO_B_CAPV_007			(IALRTDS_LO_A_HEADER + 20)
#define IALRTDS_LO_B_CAPV_008			(IALRTDS_LO_A_HEADER + 21)
#define IALRTDS_LO_B_CAPV_009			(IALRTDS_LO_A_HEADER + 22)
#define IALRTDS_LO_B_CAPV_010			(IALRTDS_LO_A_HEADER + 23)
#define IALRTDS_LO_B_CAPV_011			(IALRTDS_LO_A_HEADER + 24)
#define IALRTDS_LO_B_CAPV_012			(IALRTDS_LO_A_HEADER + 25)
#define IALRTDS_LO_B_CAPV_013			(IALRTDS_LO_A_HEADER + 26)
#define IALRTDS_LO_B_CAPV_014			(IALRTDS_LO_A_HEADER + 27)
#define IALRTDS_LO_B_CAPV_015			(IALRTDS_LO_A_HEADER + 28)
#define IALRTDS_LO_B_CAPV_016			(IALRTDS_LO_A_HEADER + 29)
#define IALRTDS_LO_B_CAPV_017			(IALRTDS_LO_A_HEADER + 30)
#define IALRTDS_LO_B_CAPV_018			(IALRTDS_LO_A_HEADER + 31)
#define IALRTDS_LO_B_CAPV_019			(IALRTDS_LO_A_HEADER + 32)
#define IALRTDS_LO_B_CAPV_020			(IALRTDS_LO_A_HEADER + 33)
#define IALRTDS_LO_B_CAPV_021			(IALRTDS_LO_A_HEADER + 34)
#define IALRTDS_LO_B_CAPV_022			(IALRTDS_LO_A_HEADER + 35)
#define IALRTDS_LO_B_CAPV_023			(IALRTDS_LO_A_HEADER + 36)
#define IALRTDS_LO_B_CAPV_024			(IALRTDS_LO_A_HEADER + 37)
#define IALRTDS_LO_B_CAPV_025			(IALRTDS_LO_A_HEADER + 38)
#define IALRTDS_LO_B_CAPV_026			(IALRTDS_LO_A_HEADER + 39) 
#define IALRTDS_LO_B_CAPV_027			(IALRTDS_LO_A_HEADER + 40)
#define IALRTDS_LO_B_CAPV_028			(IALRTDS_LO_A_HEADER + 41)
#define IALRTDS_LO_B_CAPV_029			(IALRTDS_LO_A_HEADER + 42)
#define IALRTDS_LO_B_CAPV_030			(IALRTDS_LO_A_HEADER + 43)
#define IALRTDS_LO_B_CAPV_031			(IALRTDS_LO_A_HEADER + 44)
#define IALRTDS_LO_B_CAPV_032			(IALRTDS_LO_A_HEADER + 45)
#define IALRTDS_LO_B_CAPV_033			(IALRTDS_LO_A_HEADER + 46)
#define IALRTDS_LO_B_CAPV_034			(IALRTDS_LO_A_HEADER + 47)
#define IALRTDS_LO_B_CAPV_035			(IALRTDS_LO_A_HEADER + 48)
#define IALRTDS_LO_B_CAPV_036			(IALRTDS_LO_A_HEADER + 49) 
#define IALRTDS_LO_B_CAPV_037			(IALRTDS_LO_A_HEADER + 50)
#define IALRTDS_LO_B_CAPV_038			(IALRTDS_LO_A_HEADER + 51)
#define IALRTDS_LO_B_CAPV_039			(IALRTDS_LO_A_HEADER + 52)
#define IALRTDS_LO_B_CAPV_040			(IALRTDS_LO_A_HEADER + 53)
#define IALRTDS_LO_B_CAPV_041			(IALRTDS_LO_A_HEADER + 54)
#define IALRTDS_LO_B_CAPV_042			(IALRTDS_LO_A_HEADER + 55)
#define IALRTDS_LO_B_CAPV_043			(IALRTDS_LO_A_HEADER + 56)
#define IALRTDS_LO_B_CAPV_044			(IALRTDS_LO_A_HEADER + 57)
#define IALRTDS_LO_B_CAPV_045			(IALRTDS_LO_A_HEADER + 58)
#define IALRTDS_LO_B_CAPV_046			(IALRTDS_LO_A_HEADER + 59) 
#define IALRTDS_LO_B_CAPV_047			(IALRTDS_LO_A_HEADER + 60)
#define IALRTDS_LO_B_CAPV_048			(IALRTDS_LO_A_HEADER + 61)
#define IALRTDS_LO_B_CAPV_049			(IALRTDS_LO_A_HEADER + 62)
#define IALRTDS_LO_B_CAPV_050			(IALRTDS_LO_A_HEADER + 63)
#define IALRTDS_LO_B_CAPV_051			(IALRTDS_LO_A_HEADER + 64)
#define IALRTDS_LO_B_CAPV_052			(IALRTDS_LO_A_HEADER + 65)
#define IALRTDS_LO_B_CAPV_053			(IALRTDS_LO_A_HEADER + 66)
#define IALRTDS_LO_B_CAPV_054			(IALRTDS_LO_A_HEADER + 67)
#define IALRTDS_LO_B_CAPV_055			(IALRTDS_LO_A_HEADER + 68)
#define IALRTDS_LO_B_CAPV_056			(IALRTDS_LO_A_HEADER + 69) 
#define IALRTDS_LO_B_CAPV_057			(IALRTDS_LO_A_HEADER + 70)
#define IALRTDS_LO_B_CAPV_058			(IALRTDS_LO_A_HEADER + 71)
#define IALRTDS_LO_B_CAPV_059			(IALRTDS_LO_A_HEADER + 72)
#define IALRTDS_LO_B_CAPV_060			(IALRTDS_LO_A_HEADER + 73)
#define IALRTDS_LO_B_CAPV_061			(IALRTDS_LO_A_HEADER + 74)
#define IALRTDS_LO_B_CAPV_062			(IALRTDS_LO_A_HEADER + 75)
#define IALRTDS_LO_B_CAPV_063			(IALRTDS_LO_A_HEADER + 76)
#define IALRTDS_LO_B_CAPV_064			(IALRTDS_LO_A_HEADER + 77)
#define IALRTDS_LO_B_CAPV_065			(IALRTDS_LO_A_HEADER + 78)
#define IALRTDS_LO_B_CAPV_066			(IALRTDS_LO_A_HEADER + 79) 
#define IALRTDS_LO_B_CAPV_067			(IALRTDS_LO_A_HEADER + 80)
#define IALRTDS_LO_B_CAPV_068			(IALRTDS_LO_A_HEADER + 81)
#define IALRTDS_LO_B_CAPV_069			(IALRTDS_LO_A_HEADER + 82)
#define IALRTDS_LO_B_CAPV_070			(IALRTDS_LO_A_HEADER + 83)
#define IALRTDS_LO_B_CAPV_071			(IALRTDS_LO_A_HEADER + 84)
#define IALRTDS_LO_B_CAPV_072			(IALRTDS_LO_A_HEADER + 85)
#define IALRTDS_LO_B_CAPV_073			(IALRTDS_LO_A_HEADER + 86)
#define IALRTDS_LO_B_CAPV_074			(IALRTDS_LO_A_HEADER + 87)
#define IALRTDS_LO_B_CAPV_075			(IALRTDS_LO_A_HEADER + 88)
#define IALRTDS_LO_B_CAPV_076			(IALRTDS_LO_A_HEADER + 89) 
#define IALRTDS_LO_B_CAPV_077			(IALRTDS_LO_A_HEADER + 90)
#define IALRTDS_LO_B_CAPV_078			(IALRTDS_LO_A_HEADER + 91)
#define IALRTDS_LO_B_CAPV_079			(IALRTDS_LO_A_HEADER + 92)
#define IALRTDS_LO_B_CAPV_080			(IALRTDS_LO_A_HEADER + 93)
#define IALRTDS_LO_B_CAPV_081			(IALRTDS_LO_A_HEADER + 94)
#define IALRTDS_LO_B_CAPV_082			(IALRTDS_LO_A_HEADER + 95)
#define IALRTDS_LO_B_CAPV_083			(IALRTDS_LO_A_HEADER + 96)
#define IALRTDS_LO_B_CAPV_084			(IALRTDS_LO_A_HEADER + 97)
#define IALRTDS_LO_B_CAPV_085			(IALRTDS_LO_A_HEADER + 98)
#define IALRTDS_LO_B_CAPV_086			(IALRTDS_LO_A_HEADER + 99) 
#define IALRTDS_LO_B_CAPV_087			(IALRTDS_LO_A_HEADER + 100)
#define IALRTDS_LO_B_CAPV_088			(IALRTDS_LO_A_HEADER + 101)
#define IALRTDS_LO_B_CAPV_089			(IALRTDS_LO_A_HEADER + 102)
#define IALRTDS_LO_B_CAPV_090			(IALRTDS_LO_A_HEADER + 103)
#define IALRTDS_LO_B_CAPV_091			(IALRTDS_LO_A_HEADER + 104)
#define IALRTDS_LO_B_CAPV_092			(IALRTDS_LO_A_HEADER + 105)
#define IALRTDS_LO_B_CAPV_093			(IALRTDS_LO_A_HEADER + 106)
#define IALRTDS_LO_B_CAPV_094			(IALRTDS_LO_A_HEADER + 107)
#define IALRTDS_LO_B_CAPV_095			(IALRTDS_LO_A_HEADER + 108)
#define IALRTDS_LO_B_CAPV_096			(IALRTDS_LO_A_HEADER + 109)
#define IALRTDS_LO_B_CAPV_097			(IALRTDS_LO_A_HEADER + 110)
#define IALRTDS_LO_B_CAPV_098			(IALRTDS_LO_A_HEADER + 111)
#define IALRTDS_LO_B_CAPV_099			(IALRTDS_LO_A_HEADER + 112)
#define IALRTDS_LO_B_CAPV_100			(IALRTDS_LO_A_HEADER + 113)
#define IALRTDS_LO_B_LAST				IALRTDS_LO_B_CAPV_100

// Type C
#define IALRTDS_LO_C_HEADER			(IALRTDS_LO_B_LAST + 1)
#define IALRTDS_LO_C_002_01_STS		(IALRTDS_LO_C_HEADER + 1)
#define IALRTDS_LO_C_004_03_STS		(IALRTDS_LO_C_HEADER + 2)
#define IALRTDS_LO_C_006_05_STS		(IALRTDS_LO_C_HEADER + 3)
#define IALRTDS_LO_C_008_07_STS		(IALRTDS_LO_C_HEADER + 4)
#define IALRTDS_LO_C_010_09_STS		(IALRTDS_LO_C_HEADER + 5)
#define IALRTDS_LO_C_012_11_STS		(IALRTDS_LO_C_HEADER + 6)
#define IALRTDS_LO_C_014_13_STS		(IALRTDS_LO_C_HEADER + 7)
#define IALRTDS_LO_C_016_15_STS		(IALRTDS_LO_C_HEADER + 8)
#define IALRTDS_LO_C_018_17_STS		(IALRTDS_LO_C_HEADER + 9)
#define IALRTDS_LO_C_020_19_STS		(IALRTDS_LO_C_HEADER + 10)
#define IALRTDS_LO_C_022_21_STS		(IALRTDS_LO_C_HEADER + 11)
#define IALRTDS_LO_C_024_23_STS		(IALRTDS_LO_C_HEADER + 12)
#define IALRTDS_LO_C_026_25_STS		(IALRTDS_LO_C_HEADER + 13)
#define IALRTDS_LO_C_028_27_STS		(IALRTDS_LO_C_HEADER + 14)
#define IALRTDS_LO_C_030_29_STS		(IALRTDS_LO_C_HEADER + 15)
#define IALRTDS_LO_C_032_31_STS		(IALRTDS_LO_C_HEADER + 16)
#define IALRTDS_LO_C_034_33_STS		(IALRTDS_LO_C_HEADER + 17)
#define IALRTDS_LO_C_036_35_STS		(IALRTDS_LO_C_HEADER + 18)
#define IALRTDS_LO_C_038_37_STS		(IALRTDS_LO_C_HEADER + 19)
#define IALRTDS_LO_C_040_39_STS		(IALRTDS_LO_C_HEADER + 20)
#define IALRTDS_LO_C_042_41_STS		(IALRTDS_LO_C_HEADER + 21)
#define IALRTDS_LO_C_044_43_STS		(IALRTDS_LO_C_HEADER + 22)
#define IALRTDS_LO_C_046_45_STS		(IALRTDS_LO_C_HEADER + 23)
#define IALRTDS_LO_C_048_47_STS		(IALRTDS_LO_C_HEADER + 24)
#define IALRTDS_LO_C_050_49_STS		(IALRTDS_LO_C_HEADER + 25)
#define IALRTDS_LO_C_052_51_STS		(IALRTDS_LO_C_HEADER + 26)
#define IALRTDS_LO_C_054_53_STS		(IALRTDS_LO_C_HEADER + 27)
#define IALRTDS_LO_C_056_55_STS		(IALRTDS_LO_C_HEADER + 28)
#define IALRTDS_LO_C_058_57_STS		(IALRTDS_LO_C_HEADER + 29)
#define IALRTDS_LO_C_060_59_STS		(IALRTDS_LO_C_HEADER + 30)
#define IALRTDS_LO_C_062_61_STS		(IALRTDS_LO_C_HEADER + 31)
#define IALRTDS_LO_C_064_63_STS		(IALRTDS_LO_C_HEADER + 32)
#define IALRTDS_LO_C_066_65_STS		(IALRTDS_LO_C_HEADER + 33)
#define IALRTDS_LO_C_068_67_STS		(IALRTDS_LO_C_HEADER + 34)
#define IALRTDS_LO_C_070_69_STS		(IALRTDS_LO_C_HEADER + 35)
#define IALRTDS_LO_C_072_71_STS		(IALRTDS_LO_C_HEADER + 36)
#define IALRTDS_LO_C_074_73_STS		(IALRTDS_LO_C_HEADER + 37)
#define IALRTDS_LO_C_076_75_STS		(IALRTDS_LO_C_HEADER + 38)
#define IALRTDS_LO_C_078_77_STS		(IALRTDS_LO_C_HEADER + 39)
#define IALRTDS_LO_C_080_79_STS		(IALRTDS_LO_C_HEADER + 40)
#define IALRTDS_LO_C_082_81_STS		(IALRTDS_LO_C_HEADER + 41)
#define IALRTDS_LO_C_084_83_STS		(IALRTDS_LO_C_HEADER + 42)
#define IALRTDS_LO_C_086_85_STS		(IALRTDS_LO_C_HEADER + 43)
#define IALRTDS_LO_C_088_87_STS		(IALRTDS_LO_C_HEADER + 44)
#define IALRTDS_LO_C_090_89_STS		(IALRTDS_LO_C_HEADER + 45)
#define IALRTDS_LO_C_092_91_STS		(IALRTDS_LO_C_HEADER + 46)
#define IALRTDS_LO_C_094_93_STS		(IALRTDS_LO_C_HEADER + 47)
#define IALRTDS_LO_C_096_95_STS		(IALRTDS_LO_C_HEADER + 48)
#define IALRTDS_LO_C_098_97_STS		(IALRTDS_LO_C_HEADER + 49)
#define IALRTDS_LO_C_100_99_STS		(IALRTDS_LO_C_HEADER + 50)
  

/****************************************************************************/
/* RTDS 의 수신 데이터는 연결된 시스템의 송신 으로 사용된다.                */
/****************************************************************************/
#define IALRTDS_RX_HEADER_LEGNUM_MASK	0x40000000
#define IALRTDS_RX_HEADER_LEGNUM_SHIFT	28

#define IALRTDS_FP_MASK		0xFF
#define IALRTDS_FP_OFFSET	8

#define IALRTDS_FP_T1_MASK		0x01		// 0x1 : fire,  0x0: no fire
#define IALRTDS_FP_T2_MASK		0x02		// 0x1 : fire,  0x0: no fire		
#define IALRTDS_FP_T3_MASK		0x04		// 0x1 : fire,  0x0: no fire
#define IALRTDS_FP_T4_MASK		0x08		// 0x1 : fire,  0x0: no fire
#define IALRTDS_FP_BRK_MASK		0x10		// 0x1 : on, 	0x0: off
#define IALRTDS_FP_DEACT_MASK	0x20		// 0x1 : deactive, 	0x0: active
#define IALRTDS_FP_FDISC_MASK	0x40		// 0x1 : yes, 	0x0: no
#define IALRTDS_FP_RSV_MASK		0x80		// no use

// Firing Pulse Index : Upper
#define IALRTDS_UP_HEADER			(IALRTDS_FCOMM_CH3_START)
#define IALRTDS_UP_004_01_FP		(IALRTDS_UP_HEADER + 1)
#define IALRTDS_UP_008_05_FP		(IALRTDS_UP_HEADER + 2)
#define IALRTDS_UP_012_09_FP		(IALRTDS_UP_HEADER + 3)
#define IALRTDS_UP_016_13_FP		(IALRTDS_UP_HEADER + 4)
#define IALRTDS_UP_020_17_FP		(IALRTDS_UP_HEADER + 5)
#define IALRTDS_UP_024_21_FP		(IALRTDS_UP_HEADER + 6)
#define IALRTDS_UP_028_25_FP		(IALRTDS_UP_HEADER + 7)
#define IALRTDS_UP_032_29_FP		(IALRTDS_UP_HEADER + 8)
#define IALRTDS_UP_036_33_FP		(IALRTDS_UP_HEADER + 9)
#define IALRTDS_UP_040_37_FP		(IALRTDS_UP_HEADER + 10)
#define IALRTDS_UP_044_41_FP		(IALRTDS_UP_HEADER + 11)
#define IALRTDS_UP_048_45_FP		(IALRTDS_UP_HEADER + 12)
#define IALRTDS_UP_052_49_FP		(IALRTDS_UP_HEADER + 13)
#define IALRTDS_UP_056_53_FP		(IALRTDS_UP_HEADER + 14)
#define IALRTDS_UP_060_57_FP		(IALRTDS_UP_HEADER + 15)
#define IALRTDS_UP_064_61_FP		(IALRTDS_UP_HEADER + 16)
#define IALRTDS_UP_068_65_FP		(IALRTDS_UP_HEADER + 17)
#define IALRTDS_UP_072_69_FP		(IALRTDS_UP_HEADER + 18)
#define IALRTDS_UP_076_73_FP		(IALRTDS_UP_HEADER + 19)
#define IALRTDS_UP_080_77_FP		(IALRTDS_UP_HEADER + 20)
#define IALRTDS_UP_084_81_FP		(IALRTDS_UP_HEADER + 21)
#define IALRTDS_UP_088_85_FP		(IALRTDS_UP_HEADER + 22)
#define IALRTDS_UP_092_89_FP		(IALRTDS_UP_HEADER + 23)
#define IALRTDS_UP_096_93_FP		(IALRTDS_UP_HEADER + 24)
#define IALRTDS_UP_100_97_FP		(IALRTDS_UP_HEADER + 25)

// Firing Pulse Index : Lower
#define IALRTDS_LO_HEADER			(IALRTDS_FCOMM_CH4_START)
#define IALRTDS_LO_004_01_FP		(IALRTDS_LO_HEADER + 1)
#define IALRTDS_LO_008_05_FP		(IALRTDS_LO_HEADER + 2)
#define IALRTDS_LO_012_09_FP		(IALRTDS_LO_HEADER + 3)
#define IALRTDS_LO_016_13_FP		(IALRTDS_LO_HEADER + 4)
#define IALRTDS_LO_020_17_FP		(IALRTDS_LO_HEADER + 5)
#define IALRTDS_LO_024_21_FP		(IALRTDS_LO_HEADER + 6)
#define IALRTDS_LO_028_25_FP		(IALRTDS_LO_HEADER + 7)
#define IALRTDS_LO_032_29_FP		(IALRTDS_LO_HEADER + 8)
#define IALRTDS_LO_036_33_FP		(IALRTDS_LO_HEADER + 9)
#define IALRTDS_LO_040_37_FP		(IALRTDS_LO_HEADER + 10)
#define IALRTDS_LO_044_41_FP		(IALRTDS_LO_HEADER + 11)
#define IALRTDS_LO_048_45_FP		(IALRTDS_LO_HEADER + 12)
#define IALRTDS_LO_052_49_FP		(IALRTDS_LO_HEADER + 13)
#define IALRTDS_LO_056_53_FP		(IALRTDS_LO_HEADER + 14)
#define IALRTDS_LO_060_57_FP		(IALRTDS_LO_HEADER + 15)
#define IALRTDS_LO_064_61_FP		(IALRTDS_LO_HEADER + 16)
#define IALRTDS_LO_068_65_FP		(IALRTDS_LO_HEADER + 17)
#define IALRTDS_LO_072_69_FP		(IALRTDS_LO_HEADER + 18)
#define IALRTDS_LO_076_73_FP		(IALRTDS_LO_HEADER + 19)
#define IALRTDS_LO_080_77_FP		(IALRTDS_LO_HEADER + 20)
#define IALRTDS_LO_084_81_FP		(IALRTDS_LO_HEADER + 21)
#define IALRTDS_LO_088_85_FP		(IALRTDS_LO_HEADER + 22)
#define IALRTDS_LO_092_89_FP		(IALRTDS_LO_HEADER + 23)
#define IALRTDS_LO_096_93_FP		(IALRTDS_LO_HEADER + 24)
#define IALRTDS_LO_100_97_FP		(IALRTDS_LO_HEADER + 25)
#endif


#if 1
typedef struct stFptype {			// Size 1 + SM Level (100/4) = 26
	U_IALDATA_TYPE	uFpHeader;
	U_IALDATA_TYPE	uFp[SM_LEVEL/4]; // Each Fp have Fp datas of 4 SM.
} ST_FP_TYPE;
#else
typedef struct stFptype {
	U_IALDATA_TYPE	uFpHeader;
	U_IALDATA_TYPE	uFp_004_01;
	U_IALDATA_TYPE	uFp_008_05;
	U_IALDATA_TYPE	uFp_012_09;
	U_IALDATA_TYPE	uFp_016_13;
	U_IALDATA_TYPE	uFp_020_17;
	U_IALDATA_TYPE	uFp_024_21;
	U_IALDATA_TYPE	uFp_028_25;
	U_IALDATA_TYPE	uFp_032_29;
	U_IALDATA_TYPE	uFp_036_33;
	U_IALDATA_TYPE	uFp_040_37;
	U_IALDATA_TYPE	uFp_044_41;
	U_IALDATA_TYPE	uFp_048_45;
	U_IALDATA_TYPE	uFp_052_49;
	U_IALDATA_TYPE	uFp_056_53;
	U_IALDATA_TYPE	uFp_060_57;
	U_IALDATA_TYPE	uFp_064_61;
	U_IALDATA_TYPE	uFp_068_65;
	U_IALDATA_TYPE	uFp_072_09;
	U_IALDATA_TYPE	uFp_076_73;
	U_IALDATA_TYPE	uFp_080_77;
	U_IALDATA_TYPE	uFp_084_81;
	U_IALDATA_TYPE	uFp_088_85;
	U_IALDATA_TYPE	uFp_092_89;
	U_IALDATA_TYPE	uFp_096_93;
	U_IALDATA_TYPE	uFp_100_97;
} ST_FP_TYPE;
#endif
typedef struct stAtype {			// Size 13(Fixed)
	U_IALDATA_TYPE	uHeader;
	U_IALDATA_TYPE	uVBranch;
	U_IALDATA_TYPE	uIBranch;
	U_IALDATA_TYPE	uVValves;
	U_IALDATA_TYPE	uReserved[3];
	U_IALDATA_TYPE	uPassthru[6];
} ST_A_TYPE;

typedef struct stBtype {			// Size 1 + SM Level (100)
	U_IALDATA_TYPE	uHeader;
	U_IALDATA_TYPE	CapV[SM_LEVEL];
} ST_B_TYPE;


typedef struct stCtype {			// Size 1 + SM Level/2 (100/2)
	U_IALDATA_TYPE	uHeader;
	U_IALDATA_TYPE	uState[SM_LEVEL/2];  // Each have State data of 2 SM.
} ST_C_TYPE;

typedef struct stCompacttype {		// 13 + 101 + 51 = 165
	ST_A_TYPE	stAFrame;
	ST_B_TYPE	stBFrame;
	ST_C_TYPE	stCFrame;
} ST_COMPACT_TYPE;

#endif


