#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"fbDef.h"
#include	"fb_code\fb_common\fbRunningErrCode.h"

#include	"fbMem.h"
#include	"pcsDefs.h"
#include	"systemDiag.h"

#include	"../MHCAP/mhcap_types.h"
#include	"../MHCAP/vme.h"

#include	"fb_code\fb_standard\fcVmeBrdAcc.h"

/**************************************************************************
**		외부 변수 
**************************************************************************/
extern VME_DATA			gstVmeData[VME_MAX_SLOT];
extern VME_CONTROL		gstVmeCtrl[VME_MAX_SLOT];

extern strBaseShelfIoInfo gBio;

/**************************************************************************
**		내부 함수 
**************************************************************************/
static uint32 mhcap_vme_DiDataProc1 (uint32 , uint32 );
static uint32 mhcap_vme_DoDataProc1 (uint32 , uint32 , uint32 );

/**************************************************************************
**		외부 함수 
**************************************************************************/
extern uint32 readRuntimeFbData(uint32,uint32,uint32,uint32 *);
extern uint32 writeRuntimeFbData(uint32, uint32,uint32, uint32 *);
extern void	  setErrorCode(uint8 *, uint32, uint8 *, uint32);

extern uint32	fbMemPointerGet(uint32 , strMemPointerInfo *);
extern uint32	busFbMemPointerGet(uint32 , strMemPointerInfo *);
extern STATUS mhcap_vme_AioProc1 (int32 );

/*********************************************************************
**	
**	VME Bus상에 있는 DI Board Access FB 	
**
**********************************************************************/
uint32	diRdInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= IOB_DI_CODE;
    paraInfoPtr->inputNo		= IOB_DI_SPEC_NUM;
    paraInfoPtr->intlVarNo		= IOB_DI_VAR_NUM;
    paraInfoPtr->outputNo	= IOB_DI_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++	= INT_TYPE | SIZE32_TYPE;

	/* 내부변수 - ?개 */	
	*varTypePtr++	= UINT_TYPE | SIZE32_TYPE;
	*varTypePtr++	= UINT_TYPE | SIZE32_TYPE;

    /* 출력 - ?개 */
	*outputTypePtr++	= INT_TYPE | SIZE32_TYPE;
	*outputTypePtr++	= WORD_TYPE | SIZE32_TYPE;

	return (status);	 	
}

uint32	diRdRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	strFC0311Info fd;
	int		i, size;
	int		maxChanNo;
	uint32	addr, u32_data;
	uint32	board_type;
	uint16	u16_data[2];

    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }

	/* 알고리즘 연산부 */
#ifndef _WIN32
	if((fd.brdSlotId >= MIN_VME_IO_BRD_NUM)&&(fd.brdSlotId <= MAX_VME_IO_BRD_NUM))
	{
	    if(gstVmeCtrl[fd.brdSlotId].slave_board_type == DI_BOARD_ID)
	    {
        	size = gstVmeCtrl[fd.brdSlotId].size;
        	
        	mhcap_setVmeMasterStatus (fd.brdSlotId, TRUE);
    	    
    	    mhcap_vme_DiDataProc1 (fd.brdSlotId, size);
    
        	fd.diOutput = gstVmeData[fd.brdSlotId].iodata.di_data[0];
        	fd.diOutput |= gstVmeData[fd.brdSlotId].iodata.di_data[1]<<8;
        	fd.diOutput |= gstVmeData[fd.brdSlotId].iodata.di_data[2]<<16;
        	fd.diOutput |= gstVmeData[fd.brdSlotId].iodata.di_data[3]<<24;
        	
	        fd.ioBrdRunCnt 		= gstVmeCtrl[fd.brdSlotId].slave_run_count;
	        fd.ioBrdTickCnt 	= gstVmeCtrl[fd.brdSlotId].slave_run_tick;

        	fd.errOutput = gBio.diCard[fd.brdSlotId].errCode;
    	}
    	else
    	{
            fd.errOutput = IO_FB_BOARD_CNFG_ERR;
        }        		
	}
	else
	{
		fd.errOutput = IO_FB_INPUT_PARA_ERR;	// 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
	}
	
#endif

    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }

    return(status);
}

static uint32 mhcap_vme_DiDataProc1 (uint32 slotId, uint32 size)
{
	uint32	vme_user_addr;
	uint8	rd_data;
	int32 	i;
	
	vme_user_addr = gstVmeCtrl[slotId].vme_addr + sizeof (VME_HEADER);
	
	gstVmeCtrl[slotId].vme_loop_count ++;
	
	for (i = 0; i < size; i ++) 
	{
		rd_data = *(vuchar *)(vme_user_addr + i);
	
		gstVmeCtrl[slotId].vme_rd_chg_count ++;
		
		gstVmeCtrl[slotId].vme_rd_data 			= rd_data;
		gstVmeData[slotId].iodata.di_data[i] 	= rd_data;
	}
	
	return (NO_ERROR);
}

/*********************************************************************
**	
**	VME Bus상에 있는 DO Board Access FB 	
**
**********************************************************************/
uint32	doWrInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= IOB_DO_CODE;
    paraInfoPtr->inputNo		= IOB_DO_SPEC_NUM;
    paraInfoPtr->intlVarNo		= IOB_DO_VAR_NUM;
    paraInfoPtr->outputNo	= IOB_DO_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;
	*specTypePtr++			= WORD_TYPE | SIZE32_TYPE;
	
	/* 내부변수 - ?개 */	
	*varTypePtr++			= UINT_TYPE | SIZE32_TYPE;
	*varTypePtr++			= UINT_TYPE | SIZE32_TYPE;

    /* 출력 - ?개 */
	*outputTypePtr++		= INT_TYPE | SIZE32_TYPE;
	*outputTypePtr++		= WORD_TYPE | SIZE32_TYPE;

	return (status);	 	
}

uint32	doWrRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	strFC0312Info fd;
	int		i, size;
	int		maxChanNo;
	uint32	addr, u32_data;
	uint32	board_type;
	uint16	u16_data[2];

    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }

	/* 알고리즘 연산부 */
#ifndef _WIN32
	if((fd.brdSlotId >= MIN_VME_IO_BRD_NUM)&&(fd.brdSlotId <= MAX_VME_IO_BRD_NUM))
	{
        if(gstVmeCtrl[fd.brdSlotId].slave_board_type == DO_BOARD_ID)
        {
        	size = gstVmeCtrl[fd.brdSlotId].size;

        	mhcap_setVmeMasterStatus (fd.brdSlotId, TRUE);

        	mhcap_vme_DoDataProc1 (fd.brdSlotId, size, fd.doInput);
    	
            fd.doOutput = fd.doInput;
            
			fd.ioBrdRunCnt = gstVmeCtrl[fd.brdSlotId].slave_run_count;
			fd.ioBrdTickCnt = gstVmeCtrl[fd.brdSlotId].slave_run_tick;
	       	
	       	fd.errOutput = gBio.doCard[fd.brdSlotId].errCode;
    	}
    	else
    	{
            fd.errOutput = IO_FB_BOARD_CNFG_ERR;
        }
	}
	else 
	{
		fd.errOutput = IO_FB_INPUT_PARA_ERR;	// 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
	}
	
#endif

    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }

    return(status);
}

static uint32 mhcap_vme_DoDataProc1 (uint32 slotId, uint32 size, uint32 writedata)
{
	uint32	vme_user_addr;
	uint8	rd_data;
	int32 	i;
	
	vme_user_addr = gstVmeCtrl[slotId].vme_addr + sizeof (VME_HEADER);
	
	gstVmeCtrl[slotId].vme_loop_count ++;
	
	for (i = 0; i < size; i ++) 
	{
		rd_data = *(vuchar *)(vme_user_addr + i);

		gstVmeCtrl[slotId].vme_rd_chg_count ++;

		gstVmeCtrl[slotId].vme_rd_data 			= rd_data;
		gstVmeData[slotId].iodata.do_data[i] 	= rd_data;
	}
	
	for (i = 0; i < size; i ++) 
	{
		*(vuchar *)(vme_user_addr + i) = (vuchar)((writedata>>(i*8))&0xFF);
		
		rd_data = *(vuchar *)(vme_user_addr + i);

		if ((uchar)((writedata>>(i*8))&0xFF) != rd_data) 
		{
			gstVmeCtrl[slotId].vme_wr_chg_count ++;
		}
	}
	
	gstVmeCtrl[slotId].vme_wr_data = writedata;

	return (NO_ERROR);
}

/*********************************************************************
**	
**	VME Bus상에 있는 DSP Board Access FB 	
**
**********************************************************************/
uint32	dspRwInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= IOB_CC_CODE;
    paraInfoPtr->inputNo		= IOB_CC_SPEC_NUM;
    paraInfoPtr->intlVarNo		= IOB_CC_VAR_NUM;
    paraInfoPtr->outputNo	= IOB_CC_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;
	*specTypePtr++			= INT_TYPE | SIZE32_TYPE;

	/* 내부변수 - ?개 */	
	*varTypePtr++			= INT_TYPE | SIZE32_TYPE;
	*varTypePtr++			= INT_TYPE | SIZE32_TYPE;

    /* 출력 - ?개 */
	*outputTypePtr++		= INT_TYPE | SIZE32_TYPE;

	return (status);	 	
}

uint32	dspRwRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = NO_ERROR; 
	strFC0314Info fd;
	uint32	i;
	
	uint32  srcMemOffsetPtr, busSrcMemOffsetPtr;
	uint32  dstMemOffsetPtr, busDstMemOffsetPtr;
	uint8   scanDataByte, oldScanDataByte;
	uint16  scanDataWord, oldScanDataWord;
	uint32  scanDataDword, oldScanDataDword;
	
	strMemPointerInfo memInfoPtr;
	strMemPointerInfo busMemInfoPtr;
    uint32	addr;
    uint16	u16_data[512];

	uint32  useDataNumber;
	uint32  maxDataNumber, maxDataSize;
	uint32	byte1, byte2, byte3;
	uint32	word1, word2, word3;

    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		return(status);
    }


	/* 알고리즘 연산부 */
#ifndef _WIN32
  
	if((fd.brdSlotId >= MIN_VME_IO_BRD_NUM)&&(fd.brdSlotId <= MAX_VME_IO_BRD_NUM))
	{
        if(gstVmeCtrl[fd.brdSlotId].slave_board_type == AIO_BOARD_ID)
        {
            mhcap_vme_AioProc1(fd.brdSlotId);
      		
	        fd.ioBrdRunCnt 	= gstVmeCtrl[fd.brdSlotId].slave_run_count;
	        fd.ioBrdTickCnt = gstVmeCtrl[fd.brdSlotId].slave_run_tick;

        	fd.errOutput 	= gBio.ccb.errCode;
    	}
    	else
    	{
            fd.errOutput = IO_FB_BOARD_CNFG_ERR;
       	}
	}
	else 
	{
		fd.errOutput = IO_FB_INPUT_PARA_ERR;	// 펑션블록 에러 (입력범위에러:-1/정상:0/ 보드접근에러:1/종류불일치:2,미준비상태:3,다운상태:4)
	}
	
	if((fd.brdSlotId >= MIN_VME_IO_BRD_NUM)&&(fd.brdSlotId <= MAX_VME_IO_BRD_NUM)&&(fd.errOutput == NO_ERROR))
	{	
		status = fbMemPointerGet(LOGIC_ID,&memInfoPtr);
		if(status != NO_ERROR) 
		{
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}
	
		status = busFbMemPointerGet(LOGIC_ID,&busMemInfoPtr);
		if(status != NO_ERROR) 
		{
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
		}
		
		/******************************************************************
		**		CPU 보드 에서 DSP 보드로 Write시 CPU 보드의 메모리 영역
		******************************************************************/
		if(fd.writeMemRegion == AIO_FB_MEM_S_REGION) 
		{
			srcMemOffsetPtr 	= memInfoPtr.stSharedMemPtr;
			busSrcMemOffsetPtr 	= busMemInfoPtr.stSharedMemPtr;
		} 
		else if(fd.writeMemRegion == AIO_FB_MEM_M_REGION) 
		{
			srcMemOffsetPtr		= memInfoPtr.flagMemPtr;
			busSrcMemOffsetPtr 	= busMemInfoPtr.flagMemPtr;
		} 
		else 
		{
			status = FB_INPUT_DATA_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
		
			fd.errOutput = IO_FB_INPUT_PARA_ERR;
			
			goto MODULE_END;
		}
		
		if((MAX_MPU_MMEM_NUM*4) > MAX_SHARED_MEM_SIZE)
		{ 
		  maxDataSize = (MAX_MPU_MMEM_NUM*4);
	    }
	    else
	    {
	      maxDataSize = MAX_SHARED_MEM_SIZE;
	    }
	    
	    switch(fd.accDataType) 
		{
			case VME_BYTE_ACC:
				maxDataNumber = maxDataSize;
			break;

			case VME_WORD_ACC:
				maxDataNumber = maxDataSize/2;
			break;

			case VME_DWORD_ACC:
				maxDataNumber = maxDataSize/4;
			break;

			default:
				maxDataNumber 	= 0;
				status 			= FB_INPUT_DATA_ERR;
				
				setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);

				fd.errOutput = IO_FB_INPUT_PARA_ERR;				
				goto MODULE_END;
			break;
		}
					
		/******************************************************************
		**		CPU 보드 에서 DSP 보드의 데이터 Read 
		******************************************************************/
		if(fd.readMemRegion == AIO_FB_MEM_S_REGION) 
		{
			dstMemOffsetPtr 	= memInfoPtr.stSharedMemPtr;
			busDstMemOffsetPtr 	= busMemInfoPtr.stSharedMemPtr;
		} 
		else if(fd.readMemRegion == AIO_FB_MEM_M_REGION) 
		{
			dstMemOffsetPtr 	= memInfoPtr.flagMemPtr;
			busDstMemOffsetPtr 	= busMemInfoPtr.stSharedMemPtr;
		} 
		else 
		{
			status = FB_INPUT_DATA_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);

			fd.errOutput = IO_FB_INPUT_PARA_ERR;
			
			goto MODULE_END;
		}
				
		useDataNumber = fd.numOfReadData;
		if( (useDataNumber < 0) || (useDataNumber > MAX_DSP_BRD_ACC_DATA_NUM))
		{
			status = FB_INPUT_DATA_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);

			fd.errOutput = IO_FB_INPUT_PARA_ERR;		
			goto MODULE_END;
		}
		
		if( (fd.readMemRegion == AIO_FB_MEM_S_REGION )  &&
		    ( (uint32)(memInfoPtr.flagMemPtr + (MAX_MPU_MMEM_NUM*4)) > (uint32)(dstMemOffsetPtr + fd.startAddrOfReadMem + useDataNumber)  )
		) {
			dstMemOffsetPtr 	= memInfoPtr.stSharedMemPtr;
			busDstMemOffsetPtr 	= busMemInfoPtr.stSharedMemPtr;
		} 
		else if( (fd.readMemRegion == AIO_FB_MEM_M_REGION )  &&
		    ( (uint32)(memInfoPtr.flagMemPtr + (MAX_MPU_MMEM_NUM*4)) > (uint32)(dstMemOffsetPtr + fd.startAddrOfReadMem + useDataNumber)  )
		) {
			dstMemOffsetPtr 	= memInfoPtr.flagMemPtr;
			busDstMemOffsetPtr 	= busMemInfoPtr.flagMemPtr;
		}
		else 
		{
			status = FB_INPUT_DATA_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);

			fd.errOutput = IO_FB_INPUT_PARA_ERR;		
			goto MODULE_END;
		}
		
		for(i = 0; i < useDataNumber; i++) 
		{
			switch(fd.accDataType) 
			{
				case VME_BYTE_ACC:
				case VME_WORD_ACC:
				break;

				case VME_DWORD_ACC:
					
					/* DSP 보드의 데이터 Read */
		            addr 			= mhcap_GetAIOVmeAddr (fd.brdSlotId);
		            			
			        u16_data[0] 	= *((ADDRESS16) (addr+ DSP_BRD_DATA_READ_MEM_OFFSET 	+ (i*4)));
			        u16_data[1] 	= *((ADDRESS16) (addr+ (DSP_BRD_DATA_READ_MEM_OFFSET+2) + (i*4)));
			        
			        scanDataDword = (u16_data[0] << 16) + u16_data[1];
			        
					*(uint32*)((int32*)(dstMemOffsetPtr) + fd.startAddrOfReadMem + i) 		= scanDataDword;
					*(uint32*)((int32*)(busDstMemOffsetPtr) + fd.startAddrOfReadMem + i) 	= scanDataDword;
				break;

				default:
					status = FB_INPUT_DATA_ERR;
					setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);

					fd.errOutput = IO_FB_INPUT_PARA_ERR;		
					goto MODULE_END;
				break;
			}
		}

		/******************************************************************
		**		CPU 보드 에서 DSP 보드에 데이터 Write 
		******************************************************************/
		useDataNumber = fd.numOfWriteData;
		    
		if( (useDataNumber < 0) || (useDataNumber > MAX_DSP_BRD_ACC_DATA_NUM))
		{
			status = FB_INPUT_DATA_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);

			fd.errOutput = IO_FB_INPUT_PARA_ERR;		
			goto MODULE_END;
		}
		
		if( (fd.writeMemRegion == AIO_FB_MEM_S_REGION )  &&
		    ( (uint32)(memInfoPtr.flagMemPtr + maxDataNumber) > (uint32)(srcMemOffsetPtr + fd.startAddrOfWriteMem + useDataNumber)  )
		) {
			srcMemOffsetPtr 	= memInfoPtr.stSharedMemPtr;
			busSrcMemOffsetPtr 	= busMemInfoPtr.stSharedMemPtr;
		} 
		else if( (fd.writeMemRegion == AIO_FB_MEM_M_REGION )  &&
		    ( (uint32)(memInfoPtr.flagMemPtr + maxDataNumber) > (uint32)(srcMemOffsetPtr + fd.startAddrOfWriteMem + useDataNumber)  )
		) {
			srcMemOffsetPtr 	= memInfoPtr.flagMemPtr;
			busSrcMemOffsetPtr 	= busMemInfoPtr.flagMemPtr;
		}
		else 
		{
			status = FB_INPUT_DATA_ERR;
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);

			fd.errOutput = IO_FB_INPUT_PARA_ERR;		
			goto MODULE_END;
		}
		
		
		for(i = 0; i < useDataNumber; i++) 
		{
			switch(fd.accDataType) 
			{
				case VME_BYTE_ACC:
				case VME_WORD_ACC:
				break;

				case VME_DWORD_ACC:
					
					/* DSP 보드에 데이터 Write */
		            addr 			= mhcap_GetAIOVmeAddr (fd.brdSlotId);
		            	
			        scanDataDword 	= *(uint32*)((int32*)(srcMemOffsetPtr) + fd.startAddrOfWriteMem + i);
		            u16_data[0] 	= (scanDataDword>>16)&0xFFFF;
		            u16_data[1] 	= (scanDataDword)&0xFFFF;
		            
		            *((ADDRESS16) (addr+ DSP_BRD_DATA_WRITE_MEM_OFFSET 		+ (i*4))) = u16_data[0]; 
		        	*((ADDRESS16) (addr+ (DSP_BRD_DATA_WRITE_MEM_OFFSET+2) 	+ (i*4))) = u16_data[1];
			        			
				break;

				default:
					status = FB_INPUT_DATA_ERR;
					setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
					return(status);
				break;
			}
		}
	} // if((fd.brdSlotId>=0)&&(fd.brdSlotId<=15)&&(fd.errOutput == 0)
#endif

MODULE_END:

    /* 출력 파라미터 쓰기 */
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						     );

	if(status) 
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);

    return(status);
}
