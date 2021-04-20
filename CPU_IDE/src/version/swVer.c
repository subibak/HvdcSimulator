/* 
 * @file        sysVer.c
 * @author LK Eleven Corporation
 * @date        01-AUG-2018
 *
 * @brief software version관리
 * 
 * Design Notes:
 *    01-AUG-18  코딩 시작
 * 
 * -- LK Eleven Copyright Notice --
 *
 * @par
 * LK ELEVEN CONFIDENTIAL
 *
 * @par
 * Copyright 2018 LK Eleven Corporation All Rights Reserved.
 *
 * @par
 * The source code contained or described herein and all documents
 * related to the source code ("Material") are owned by LK Eleven Corporation
 * or its suppliers or licensors.  Title to the Material remains with
 * Intel Corporation or its suppliers and licensors.  The Material
 * contains trade secrets and proprietary and confidential information of
 * Intel or its suppliers and licensors.  The Material is protected by
 * worldwide copyright and trade secret laws and treaty provisions. No
 * part of the Material may be used, copied, reproduced, modified,
 * published, uploaded, posted, transmitted, distributed, or disclosed in
 * any way without Intel's prior express written permission.
 *
 * @par
 * No license under any patent, copyright, trade secret or other
 * intellectual property right is granted to or conferred upon you by
 * disclosure or delivery of the Materials, either expressly, by
 * implication, inducement, estoppel or otherwise.  Any license under
 * such intellectual property rights must be express and approved by
 * Intel in writing.
 *
 * @par
 * For further details, please see the file README.TXT distributed with
 * this software.
 * -- End LK Eleven Copyright Notice --
 */

#include	"usrTypes.h"
#ifdef VXWORKS
#include	"vxWorks.h"
#endif



int8 *verDate      = __DATE__ "," __TIME__;
uint32 romSwVersion	= 105;
uint32 releaseDate	= 0x20210217;
	

/*
	======= ==========  =========================================================
    Version   날짜                          변경 이력
	======= ==========  =========================================================
	[V105]
			21/02/17	1. VBE의 Sub module 및 Measurement에 대한 진단 정보 변경
							- Submodule 진단정보 프로토콜에서 제외 : strVTypeCpuRasInfo
							- ODM 진단정보 프로토콜에서 제외 : strMTypeCpuRasInfo
							- 서버모듈의 CAP.전압및 진단 정보를 M영역의 특정 위치에 Write
							  관련 함수 : subModuleInfoReadAndWriteToMregion
							  
						2. M 메모리 Size 변경(이유 : HMI에서 메모리 주소를 40000으로 시작해서임 
							- 관련 변수 : MEM_MPU_MMEM_NUM (16K-->80K)
	[V104]
			21/01/12	1. Protection FB Code 입력 Type 변경
						   관련 함수 : xxxxxxInitFunc : UINT_TYPE --> INT_TYPE
						             strFC****Info.h : uint32 --> int32	
						2. Application 초기 부팅시 에러가 발생할 경우 부트 config mode로 set
						   변경 사유는 FB Logic 다운로시 에러 시 보드 재부팅 때 메모리 Clear할 경우 발생 때문
						   관련 함수 : bootParaConfig(), systemInitErr() 	
	[V103]
			20/08/24	1. 파일 및 디렉토리 이름 변경
	[V102]
			20/08/12	1. head파일 정리
	[V101]
			20/06/30	1. 통신 수신 영향으로 Logic 수행 Overtime 발생으로 FB Task Priority 변경
						   관련 변수 : CYCLIC_FB_TASK_PRI : 110-->40
						
	[V100]
			20/03/28    FBEditor와 진단정보 일치시킴
	-------  ----------   ---------------------------------------------------------
*/


