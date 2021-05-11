/* 
 * @file        sysVer.c
 * @author LK Eleven Corporation
 * @date        01-AUG-2018
 *
 * @brief software version����
 * 
 * Design Notes:
 *    01-AUG-18  �ڵ� ����
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
uint32 romSwVersion	= 107;
uint32 releaseDate	= 0x20210421;
	

/*
	======= ==========  =========================================================
    Version   ��¥                          ���� �̷�
	======= ==========  =========================================================
	[V108]
			21/04/28	1. ��� FB ���� ������ Send �� ť Full �߻� �Ͽ� ť ���� ���� (512-->2048)
							- ���� ���� : MAX_SYS_MSG_QUEUE_COUNT
			21/05/10	1. Measurement System�� ��� �ϴ� FB Code �Լ� ����
						   ������ �޸� ������ ��� ������ ��ü => ������ �����͸� �����ϴ� ������ ����	
							- ���� �Լ� : pmc502RdRunFunc()
						2. FB Code���� DSP ���忡 Read/Write �ϴ� ��� Swap�ϴ� ������ R/W ����
							- ���� �Լ� : dspRwRunFunc()
	[V107]
			21/04/21	1. M �޸� �ּ� ����(���� : HMI���� �޸� �ּҸ� 40000���� �����ؼ��� 
							- ���� ���� : M_MEMORY_MAX_ADDR_DWORD (16K-->80K)
			21/04/22	1. �����̼ǰ� ��Ž� RW �޸� ���� ���濡 ���� ����(S_MEMORY-->M_MEMORY)
							- ���� �Լ� :  sysRmtDataWriteToSharedMemqAndMakeResp()
										sysRmtDataReadInSharedMemAndMakeResp()
										sysRmtDataReadRespWriteToSharedMem()
										sysRmtDataBroadCastFunc()
				
						2. Station ��� ���� ����ü�� �̸� �� �ҽ� ����(����ü�� FB Code �Է� ���� matching ���� 
							- ���� ����ü : strFC0302Info
							- ���� �Լ� : ethDSndRunFunc

	[V106]
						1. AUTO �޸� Size ����(���� : HMI���� �޸� �ּҸ� 40000���� �����ؼ��� 
							- ���� ���� : MEM_MPU_AUTOVAR_NUM (16K-->80K)
	[V105]
			21/02/17	1. VBE�� Sub module �� Measurement�� ���� ���� ���� ����
							- Submodule �������� �������ݿ��� ���� : strVTypeCpuRasInfo
							- ODM �������� �������ݿ��� ���� : strMTypeCpuRasInfo
							- ��������� CAP.���й� ���� ������ M������ Ư�� ��ġ�� Write
							  ���� �Լ� : subModuleInfoReadAndWriteToMregion
							  
						2. M �޸� Size ����(���� : HMI���� �޸� �ּҸ� 40000���� �����ؼ��� 
							- ���� ���� : MEM_MPU_MMEM_NUM (16K-->80K)
	[V104]
			21/01/12	1. Protection FB Code �Է� Type ����
						   ���� �Լ� : xxxxxxInitFunc : UINT_TYPE --> INT_TYPE
						             strFC****Info.h : uint32 --> int32	
						2. Application �ʱ� ���ý� ������ �߻��� ��� ��Ʈ config mode�� set
						   ���� ������ FB Logic �ٿ�ν� ���� �� ���� ����� �� �޸� Clear�� ��� �߻� ����
						   ���� �Լ� : bootParaConfig(), systemInitErr() 	
	[V103]
			20/08/24	1. ���� �� ���丮 �̸� ����
	[V102]
			20/08/12	1. head���� ����
	[V101]
			20/06/30	1. ��� ���� �������� Logic ���� Overtime �߻����� FB Task Priority ����
						   ���� ���� : CYCLIC_FB_TASK_PRI : 110-->40
						
	[V100]
			20/03/28    FBEditor�� �������� ��ġ��Ŵ
	-------  ----------   ---------------------------------------------------------
*/


