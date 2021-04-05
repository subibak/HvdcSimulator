
#include	<stdio.h>
#include	<stdlib.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"cmdList.h"
#include	"netProtocol.h"
#include	"usrDefBuf.h"
#include	"usrErrCode.h"
#include	"sysdual.h"

#include	"fbMem.h"
#include	"fbDef.h"
#include	"lkv120Board.h"
#include	"osDepend.h"

#include	"sysConfig.h"
#include	"network.h"
#include	"msgQueData.h"
#include	"pcsDefs.h"
#include	"systemDiag.h"

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)

extern void *gMpuFbStartAddr;
extern strSysDualLiveCountInfo gSysDualLiveCount; 
extern  strSysDualInfo gRmtShelfDualInfo;
extern  uint32 gDebugPortFd;
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)

extern uint32	gDluNetCurSendCnt, gDluNetSendCntPerSec;
#endif

uint32 rmtDataCopyReqFunc(strDualProtocolInfo*, strBufInfo*);
uint32 rmtDualConfigReadReqFunc(strDualProtocolInfo*, strBufInfo*); 

uint32 rmtFbDataCopyRespFunc(strDualProtocolInfo*, strBufInfo*);
uint32 rmtDualConfigReadRespFunc(strDualProtocolInfo*, strBufInfo*); 
uint32 rmtDualConfigWriteRespFunc(strDualProtocolInfo*, strBufInfo*); 
uint32 rmtDualCmdWriteRespFunc(strDualProtocolInfo*, strBufInfo*);

uint32 rmtLiveCountRecvFunc(strDualProtocolInfo*, strBufInfo*);	 
uint32 rmtFbRunDataCopyRespFunc(strDualProtocolInfo*, strBufInfo*);	
uint32 rmtFbRunTypeDataCopyRespFunc(strDualProtocolInfo*, strBufInfo*);	
uint32 rmtFbRunBusDataCopyRespFunc(strDualProtocolInfo*, strBufInfo*);	


uint32	myRmtDataAcc(uint8 *, uint32, uint32, uint32, int8 *, int8 *);
uint32	myRmtFourByteDataWrite(	uint32, int8 *, int8 *);
uint32	myRmtAllTypeFlagDataWrite(	uint32, uint32 ,uint32 , int8 *, int8 *);

uint32 dualDataAreaGet(uint32 , uint8* );
uint32 dualDataAreaCheck(uint32 , uint8, uint32*);



extern  void	memoryCopy (uint8 *, uint8 *, uint32);
extern void		memoryClear ( uint8 *, uint32);
extern uint32	dualCmdNullFunc(strDualProtocolInfo *,strBufInfo *);
extern uint32	myBusDataAcc(uint8 *, uint32, uint32	, uint32 ,int8 *, int8 *);
extern uint32	dualCmdAccFunc(strSysDualInfo	*dualInfoPtr);
extern void		fourBytesDataCopy ( uint32 *, uint32 *, uint32 );

extern uint32	systemCnfgInfoRead( strSysConfigInfo *);
extern uint32	comPortInfoGet(uint32, strinetAddr *, strinetAddr *);
extern uint32   netMsgChecksumCalcu(uint32  ,uint32 *,uint32    *);
extern int32	dataSendToServerOrClient(strDataBuf *,void *, uint32 );

extern void		setErrorCode(uint8 *,uint32,uint8 *,uint32);
extern void		setErrorCodeWithVal(uint8 *, uint32, uint8 *, uint32,
					uint8   *, uint32,uint8 *, uint32,uint8 *, uint32);
extern void		logMessageSet (int8 *, uint32, int8 *, uint32,uint32);
extern void systemRasInfoGet(uint32 , uint32 *);


uint32	(*requestDualCmdOPFuncList[]) 
			(strDualProtocolInfo	*,strBufInfo *)
= {
	dualCmdNullFunc,			
	rmtDataCopyReqFunc,				 
	rmtDualConfigReadReqFunc		
};

uint32	(*responseDualCmdOPFuncList[]) 
			(strDualProtocolInfo	*,strBufInfo *)
= {
	dualCmdNullFunc,			
	rmtFbDataCopyRespFunc,			 
	rmtDualConfigReadRespFunc,		

	rmtDualConfigWriteRespFunc,		
	rmtDualCmdWriteRespFunc,		
	dualCmdNullFunc,			
	rmtFbRunDataCopyRespFunc,		
	rmtFbRunTypeDataCopyRespFunc,	
	rmtFbRunBusDataCopyRespFunc		
};

uint32	(*remotecastDualCmdOPFuncList[]) 
			(strDualProtocolInfo	*,strBufInfo *)
= {
	dualCmdNullFunc,			
	dualCmdNullFunc,			
	dualCmdNullFunc,			
	dualCmdNullFunc,			
	dualCmdNullFunc,			
	rmtLiveCountRecvFunc			
};

uint32 rmtDataCopyReqFunc(strDualProtocolInfo	*reqMsgPtr,
							strBufInfo			*respMsgPtr )
{
	uint32	status = NO_ERROR;
	uint32	*reqDataPtr, *respDataPtr, respDataLen;
	uint32  opCommand = 0, reqTypeSize =0;
	uint32  startAddress,i;
	uint32 bitMask = 0x00ffffff;
	uint32 offSet = (uint32)MPU_FB_MEMORY_START_BUS_ADDR;
	uint32 reqSize, accDataType;
	uint8  *respBytePtr,*srcPtr;

	respDataLen = 0;
	respDataPtr = (uint32 *)respMsgPtr->dataBuf;
 	reqDataPtr = (uint32 *)reqMsgPtr->dataBuff;

 	opCommand = *reqDataPtr++ ;		
	startAddress = *reqDataPtr++ ;	
	
	if((startAddress & bitMask) >(uint32)MAX_MEMORY_SIZE) return(MAX_MEM_SIZE_EXCEED_ERR);
	
	reqTypeSize = *reqDataPtr;
	reqSize = (*reqDataPtr)&0xffff;	
	accDataType = (*reqDataPtr++)>>16;  
	


	if(reqSize > 2000) status = SOCK_SEND_LEN_ERR;
	
    if( (accDataType != BUS_BYTE_ACC) && (accDataType != BUS_WORD_ACC) &&
        (accDataType != BUS_LONG_ACC) && (accDataType != BUS_DLONG_ACC) 
	){
		status = BUS_DATA_ACC_TYPE_ERR;
    }

	if(status != NO_ERROR) 
		reqTypeSize = 0; 

	 
	*respDataPtr++ 	= opCommand;
	*respDataPtr++ 	= startAddress; 
	*respDataPtr++ 	= reqTypeSize;
	respDataLen 	= DOUBLE_SIZE*3;
	
	respBytePtr = (uint8*)respDataPtr;
	srcPtr = (uint8*)((startAddress & bitMask) + offSet);

	if(status == NO_ERROR) 
	{
		for(i = 0; i < (reqSize/accDataType); i++)
		{
		    status = myBusDataAcc(__FILE__,__LINE__, accDataType, 
									BUS_DATA_READ ,
									(uint8 *)srcPtr, 
									(uint8 *)respBytePtr
						 		 );
			if(status)
			{
				setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
				break;	
			}
			
			respBytePtr = (uint8*)(respBytePtr + accDataType);
			srcPtr 		= (uint8*)(srcPtr + accDataType);
			
			respDataLen += accDataType;
		}
	}
  	
	respMsgPtr->dataLen = respDataLen;

	return(status);
}


uint32 rmtFbDataCopyRespFunc(	strDualProtocolInfo	*reqMsgPtr,
								strBufInfo			*respMsgPtr 
							)
{
	uint32	status = NO_ERROR;
	uint32	*reqDataPtr, *respDataPtr, respDataLen;
	uint32  opCommand = 0;
	uint32  startAddress,i;
	
	uint32 bitMask = 0x00ffffff;
	uint32 offSet = (uint32)MPU_FB_MEMORY_START_BUS_ADDR;
	uint32 reqSize, accDataType,areaMaxAddr; 
	uint8  *reqBytePtr,*destPtr;

	respDataLen 	= 0;
	respDataPtr 	= (uint32 *)respMsgPtr->dataBuf;
 	reqDataPtr 		= (uint32 *)reqMsgPtr->dataBuff;

 	opCommand 		= *reqDataPtr++ ;		
	startAddress 	= *reqDataPtr++ ;

	
	if((startAddress & bitMask) > (uint32)MAX_MEMORY_SIZE) 
	{
		status = MAX_MEM_SIZE_EXCEED_ERR;
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
		return(status);
	}
	
	status = dualDataAreaCheck(startAddress,reqMsgPtr->dualDataArea, &areaMaxAddr);
	if(status!=NO_ERROR)
	{
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
		return(status);
	}

	reqSize 	= (*reqDataPtr)&0xffff;	
	accDataType = (*reqDataPtr++)>>16;  

	
	if(reqSize > 2000) 
	{
		status = SOCK_SEND_LEN_ERR; 
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
        return(status);
	}

	
    if( (accDataType != BUS_BYTE_ACC) && (accDataType != BUS_WORD_ACC) &&
        (accDataType != BUS_LONG_ACC) && (accDataType != BUS_DLONG_ACC)) 
	{
		status = BUS_DATA_ACC_TYPE_ERR;
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
        return(status);
    }

	reqBytePtr 	= (uint8*)reqDataPtr;
	destPtr 	= (uint8*)((startAddress & bitMask) + offSet);

	
	if(status == NO_ERROR) 
	{
		for(i=0;i < (reqSize/accDataType) ;i++)
		{
			status = myBusDataAcc(__FILE__,__LINE__,accDataType, 
									BUS_DATA_WRITE ,
									(uint8 *)destPtr, 
									(uint8 *)reqBytePtr
						 		 );
			
			if(status != NO_ERROR) 
			{
				setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
				break;
			}

			reqBytePtr 	= (uint8*)(reqBytePtr + accDataType);
			destPtr 	= (uint8*)(destPtr + accDataType);
			
		}
	}

	
	reqBytePtr 	= (uint8*)reqDataPtr;
	destPtr 	= (uint8*)((startAddress & bitMask) + (uint32)gMpuFbStartAddr);

	if(status == NO_ERROR) 
	{
		for(i=0;i < (reqSize/accDataType) ;i++)
		{
			memoryCopy((uint8*)destPtr,(uint8*)reqBytePtr,accDataType);

			reqBytePtr 	= (uint8*)(reqBytePtr + accDataType);
			destPtr 	= (uint8*)(destPtr + accDataType);
		}
	}
	
	 
	return(status);
}


uint32 rmtFbRunBusDataCopyRespFunc(strDualProtocolInfo	*reqMsgPtr,
							strBufInfo			*respMsgPtr )
{
	uint32	status = NO_ERROR;
	uint32	*reqDataPtr, *respDataPtr, respDataLen;
	uint32  opCommand = 0;
	uint32  startAddress,i;
	
	uint32 bitMask = 0x00ffffff;
	uint32 offSet = (uint32)MPU_FB_MEMORY_START_BUS_ADDR;
	uint32 reqSize, accDataType,backupFlag=0,areaMaxAddr; 
	uint8  *reqBytePtr,*destPtr;

	respDataLen = 0;
	respDataPtr = (uint32 *)respMsgPtr->dataBuf;
 	reqDataPtr = (uint32 *)reqMsgPtr->dataBuff;

 	opCommand = *reqDataPtr++ ;		
	startAddress = *reqDataPtr++ ;
	backupFlag = *reqDataPtr++ ;

	
	if((startAddress & bitMask) >(uint32)MAX_MEMORY_SIZE) 
	{
		status = MAX_MEM_SIZE_EXCEED_ERR;
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
		return(status);
	}

	
	status = dualDataAreaCheck(startAddress,reqMsgPtr->dualDataArea, &areaMaxAddr);
	if(status!=NO_ERROR)
	{
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
		return(status);
	}

	reqSize 	= (*reqDataPtr)&0xffff;	
	accDataType = (*reqDataPtr++)>>16;  

	
	if(reqSize > 2000) 
	{
		status = SOCK_SEND_LEN_ERR; 
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
        return(status);
	}

	
    if( (accDataType != BUS_BYTE_ACC) && (accDataType != BUS_WORD_ACC) &&
        (accDataType != BUS_LONG_ACC) && (accDataType != BUS_DLONG_ACC)) 
	{
		status = BUS_DATA_ACC_TYPE_ERR;
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
        return(status);
    }

	reqBytePtr 	= (uint8*)reqDataPtr;
	destPtr 	= (uint8*)((startAddress & bitMask) + offSet);

	
	if( (status == NO_ERROR)&&(backupFlag==EN_SAVE) ) 
	{
		for(i=0;i < (reqSize/accDataType) ;i++)
		{
			status = myBusDataAcc(__FILE__,__LINE__,accDataType, BUS_DATA_WRITE ,
								(uint8 *)destPtr, 
								(uint8 *)reqBytePtr
						 		 );
			if(status != NO_ERROR) 
			{
				setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
				break;
			}
			
			reqBytePtr 	= (uint8*)(reqBytePtr + accDataType);
			destPtr 	= (uint8*)(destPtr + accDataType);
		}
	}

	
	reqBytePtr 	= (uint8*)reqDataPtr;
	destPtr 	= (uint8*)((startAddress & bitMask) + (uint32)gMpuFbStartAddr);

	if(status == NO_ERROR) 
	{
		for(i=0;i < (reqSize/accDataType);  i++)
		{
			memoryCopy((uint8*)destPtr,(uint8*)reqBytePtr,accDataType);

			reqBytePtr 	= (uint8*)(reqBytePtr + accDataType);
			destPtr 	= (uint8*)(destPtr + accDataType);

		}
	}
	
	 
	return(status);
}


uint32 rmtDualConfigReadReqFunc(	strDualProtocolInfo	*reqMsgPtr,
									strBufInfo			*respMsgPtr 
								)

{
	uint32	status = NO_ERROR;
	return(status);
}


uint32 rmtDualConfigReadRespFunc(	strDualProtocolInfo	*reqMsgPtr,
									strBufInfo			*respMsgPtr 
								)

{
	uint32	status = NO_ERROR;
	uint32	*reqDataPtr;
	uint32  opCommand;

 	reqDataPtr = (uint32 *)reqMsgPtr->dataBuff;

 	opCommand 	= *reqDataPtr++ ; 
	status 		= *reqDataPtr++ ;

	
	if( (reqMsgPtr->dataLength !=  (sizeof(strSysDualInfo) + 4*2)) )
	{
		status = MSG_LEN_EXCEED_ERR;
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
		return(status);		
	}

	if(status == NO_ERROR)
	{	
		fourBytesDataCopy(	(uint32*)&gRmtShelfDualInfo,
							(uint32*)reqDataPtr,
							sizeof(strSysDualInfo)/4 
						);
	}

	 
	return(status);
}

uint32 rmtDualConfigWriteRespFunc(	strDualProtocolInfo	*reqMsgPtr,
									strBufInfo			*respMsgPtr 
								)

{
	uint32	status = NO_ERROR;
	uint32	*reqDataPtr;
	uint32  opCommand = 0;

 	reqDataPtr 	= (uint32 *)reqMsgPtr->dataBuff;
 	opCommand 	= *reqDataPtr++ ; 
 	
	fourBytesDataCopy(	(uint32*)&gRmtShelfDualInfo,
						(uint32*)reqDataPtr,
						sizeof(strSysDualInfo)/4
					);

	return(status);
}


uint32 rmtDualCmdWriteRespFunc(strDualProtocolInfo	*reqMsgPtr,
								strBufInfo			*respMsgPtr )

{
	uint32	status = NO_ERROR;
	uint32	*reqDataPtr;
	uint32  opCommand = 0;
	strSysDualInfo dualInfo;

 	reqDataPtr = (uint32 *)reqMsgPtr->dataBuff;

 	opCommand 			= *reqDataPtr++ ; 
	dualInfo.dualCmd	= *reqDataPtr++ ;

	memoryCopy(	(uint8*)&gRmtShelfDualInfo.dualCmd, 
				(uint8*)&dualInfo.dualCmd, 
				DOUBLE_SIZE
			  );
			  
	return(status);
}



uint32 rmtLiveCountRecvFunc(strDualProtocolInfo	*reqMsgPtr,
							strBufInfo			  *respMsgPtr )
{
	uint32 status = NO_ERROR;
	uint32	*reqDataPtr;
	uint32  opCommand = 0;
	uint32 myliveCount, myErrSummary;
	uint32 rmtliveCount, rmtErrSummary;
	uint32 sysRedundancyStatus = 0;

 	reqDataPtr = (uint32 *)reqMsgPtr->dataBuff;

 	opCommand = *reqDataPtr++ ;			

	rmtliveCount 	= *reqDataPtr++ ;			
	rmtErrSummary 	= *reqDataPtr++ ;			
	myliveCount 	= *reqDataPtr++ ;			
	myErrSummary 	= *reqDataPtr++ ;			

	sysRedundancyStatus = *reqDataPtr++ ;		

	gRmtShelfDualInfo.shelfLiveCount 		= rmtliveCount  ; 
	gRmtShelfDualInfo.myShelfErrSummary 	= rmtErrSummary;
	gRmtShelfDualInfo.rmtShelfLiveCount 	= myliveCount ; 
	gRmtShelfDualInfo.rmtShelfErrSummary 	= myErrSummary;

	return(status);
}


uint32 rmtFbRunDataCopyRespFunc(strDualProtocolInfo	*reqMsgPtr,
							strBufInfo			*respMsgPtr )
{
	uint32	status = NO_ERROR, retVal = NO_ERROR;
	uint32	*reqDataPtr, *respDataPtr, respDataLen;
	uint32  opCommand = 0;
	uint32  startAddress,i;
	
	uint32 bitMask = 0x00ffffff;
	uint32 offSet = (uint32)MPU_FB_MEMORY_START_BUS_ADDR;
	uint32 reqNumber,areaMaxAddr; 
	uint32 *destPtr, recvdata;

	respDataLen = 0;
	respDataPtr = (uint32 *)respMsgPtr->dataBuf;
 	reqDataPtr = (uint32 *)reqMsgPtr->dataBuff;

 	opCommand = *reqDataPtr++ ;		
	reqNumber = *reqDataPtr++ ;		

	if(reqNumber >  MAX_DUAL_COPY_DATA_NUM )
	{
		status = SOCK_SEND_LEN_ERR;
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, retVal);
		return(status);
	}

	for(i=0;i<reqNumber;i++)
	{
		startAddress 	= *reqDataPtr++ ;	
		recvdata 		= *reqDataPtr++ ;	
		
		if(i == 0) 
		{
			retVal = dualDataAreaCheck(startAddress,reqMsgPtr->dualDataArea, &areaMaxAddr);
			if(retVal!=NO_ERROR)
			{
				setErrorCode(__FILE__, __LINE__ , __FUNCTION__, retVal);
			}
		}
		else
		{
			if((startAddress & bitMask) > areaMaxAddr) 
			{
				retVal = MAX_MEM_SIZE_EXCEED_ERR;
				setErrorCodeWithVal(__FILE__, __LINE__ , __FUNCTION__, retVal,
										"startAddress",startAddress ,
										"areaMaxAddr",areaMaxAddr ,
										"Dual Area",reqMsgPtr->dualDataArea);
				continue; 
			}
		}

		
		destPtr = (uint32*)((startAddress & bitMask) + offSet);
		status = myBusDataAcc(__FILE__,__LINE__,BUS_LONG_ACC, BUS_DATA_WRITE ,
								(uint8 *)destPtr, 
								(uint8 *)&recvdata
							 );

		
		if(status == NO_ERROR)
		{
			destPtr = (uint32*)((startAddress & bitMask) + (uint32)gMpuFbStartAddr);
			fourBytesDataCopy((uint32*)destPtr,(uint32*)&recvdata,1);
		}

	}

	 
	return(status);
}



uint32 rmtFbRunTypeDataCopyRespFunc(strDualProtocolInfo	*reqMsgPtr,
							strBufInfo			*respMsgPtr )
{
	uint32	status = NO_ERROR;
	uint32	*reqDataPtr, *respDataPtr, respDataLen;
	uint32  opCommand = 0;
	uint32  startAddress,i;
	
	uint32 bitMask = 0x00ffffff;
	uint32 offSet = (uint32)MPU_FB_MEMORY_START_BUS_ADDR;
	uint32 reqNumber, dataType, backupFlag = 0; 
	uint32 *destPtr, recvdata = 0;
	uint32 retVal = NO_ERROR, areaMaxAddr;

	respDataLen = 0;
	respDataPtr = (uint32 *)respMsgPtr->dataBuf;
 	reqDataPtr = (uint32 *)reqMsgPtr->dataBuff;

 	opCommand = *reqDataPtr++ ;		
	reqNumber = *reqDataPtr++ ;		

	if(reqNumber >  MAX_DUAL_COPY_DATA_NUM )
	{
		status = SOCK_SEND_LEN_ERR;
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
		return(status);
	}

	for(i=0;i<reqNumber;i++)
	{
		startAddress 	= *reqDataPtr++ ;	
		recvdata 		= *reqDataPtr++ ;
		dataType  		= (*reqDataPtr & 0xffff);				
		backupFlag  	= (*reqDataPtr++ >>16) & 0xffff;	

		
		
		if(i == 0) 
		{
			retVal = dualDataAreaCheck(startAddress,reqMsgPtr->dualDataArea, &areaMaxAddr);
			if(retVal!=NO_ERROR)
			{
				setErrorCode(__FILE__, __LINE__ , __FUNCTION__, retVal);
			}
		}
		else
		{
			if((startAddress & bitMask) > areaMaxAddr) 
			{
				retVal = MAX_MEM_SIZE_EXCEED_ERR;
				setErrorCodeWithVal(__FILE__, __LINE__ , __FUNCTION__, retVal,
										"startAddress",startAddress ,
										"areaMaxAddr",areaMaxAddr ,
										"Dual Area",reqMsgPtr->dualDataArea);
				continue; 
			}
		}
		
		
		if( (dataType!= BUS_BYTE_ACC) &&
			(dataType!= BUS_WORD_ACC) && (dataType!= BUS_LONG_ACC) ) 
		{
			status = BUS_DATA_ACC_TYPE_ERR;
		 	setErrorCodeWithVal(__FILE__,__LINE__, __FUNCTION__, status,
								(uint8*)"Data Type",dataType ,
								(uint8*)"Bus Addr",(startAddress & bitMask),
								(uint8*)"Recv Data",recvdata );
			return(status);
		}

		
		destPtr = (uint32*)((startAddress & bitMask) + offSet);
	
		
		if(backupFlag == EN_SAVE)
		{
			status = myBusDataAcc(__FILE__,__LINE__,dataType, BUS_DATA_WRITE ,
								(uint8 *)destPtr, 
								(uint8 *)&recvdata
								 );
		}

		
		if(status == NO_ERROR)
		{
			destPtr = (uint32*)((startAddress & bitMask) + (uint32)gMpuFbStartAddr);
			memoryCopy((uint8*)destPtr,(uint8*)&recvdata,dataType);
		}

	}

	 
	return(status);
}



extern uint32   gDualTransRateCalcuFlag;
extern void		networkPerformanceCountSet(uint32   ,   uint32 );

uint32	myRmtDataAcc(	uint8 *fileName, uint32 lineNum,
						uint32	accDataType, uint32 mode, 
						int8 *busAddrPtr, int8 *bufPtr)
{
	uint32	status = NO_ERROR;
	
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
	strDataBuf sendData;
    strDualProtocolInfo *netMsgPtr;
	strinetAddr localAddr, peerAddr;
	strSysConfigInfo sysCnfgInfo;

	uint32 cmdCode,reqTypeSize, reqSize,checkSum;
	uint32 addrPtr;


    
    if( (accDataType != BUS_BYTE_ACC) && (accDataType != BUS_WORD_ACC) &&
        (accDataType != BUS_LONG_ACC)  && (accDataType != BUS_DLONG_ACC) 
	){
		status = BUS_DATA_ACC_TYPE_ERR;
        return(status);
    }

    if( (mode != BUS_DATA_READ) && (mode != BUS_DATA_WRITE)
	){
		status = BUS_ACC_TYPE_ERR;
        return(status);
    }

	addrPtr = (uint32)(busAddrPtr);
	if(	(addrPtr&0xffffff) > MAX_MEMORY_SIZE ) {
		status = MAX_MEM_SIZE_EXCEED_ERR;
        return(status);
	}
	
	else if( ((addrPtr&0xff000000) != MEM_BOARD_BUS_ADDR_ON_MASTER) &&
		     ((addrPtr&0xff000000) != MEM_BOARD_BUS_ADDR_ON_SLAVE)
	){
		status = BUS_DATA_ACC_ERR;
		return(status);
	}

	memoryClear ( (uint8 *) &sysCnfgInfo, sizeof(strSysConfigInfo));
  	systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);
  	
	
	localAddr.iAddr= sysCnfgInfo.redNetCnfgInfo.rmtDluIpAddrHex;	
	status = comPortInfoGet(RMTRESPS, &localAddr, &peerAddr); 
	
	
	sendData.socket_fd = peerAddr.sockFd;
	netMsgPtr = (strDualProtocolInfo *)sendData.data_buf;
	memoryClear ( (uint8 *)netMsgPtr , sizeof(strDualProtocolInfo));

	netMsgPtr->sequenceID  = 0;
	
	
	(void)dualDataAreaGet((uint32) addrPtr, (uint8*)&netMsgPtr->dualDataArea);

	
	cmdCode= (DUAL_ACCESS_CMD<<16) | (DUAL_MEMORY_COPY&0xffff);  
	
	if(mode == BUS_DATA_WRITE)
	{
		
		netMsgPtr->messageType = RMTRESPS; 
		
		reqTypeSize =  ( (accDataType <<16 ) | (accDataType&0xffff) );
		
		memoryCopy(&netMsgPtr->dataBuff[0],(uint8*)&cmdCode,4);
		memoryCopy(&netMsgPtr->dataBuff[4],(uint8*)&busAddrPtr,4);
		memoryCopy(&netMsgPtr->dataBuff[8],(uint8*)&reqTypeSize,4);
		if(accDataType == BUS_DLONG_ACC){
			memoryCopy(&netMsgPtr->dataBuff[12],(uint8*)bufPtr,8);
			netMsgPtr->dataLength  = 5*DOUBLE_SIZE;
		}
		else {
			memoryCopy(&netMsgPtr->dataBuff[12],(uint8*)bufPtr,4);
			netMsgPtr->dataLength  = 4*DOUBLE_SIZE; 
		}

		sendData.data_len   =   netMsgPtr->dataLength + MAX_DUAL_FRAME_HEAD_LEN;
	}
	else if(mode == BUS_DATA_READ){
		
		netMsgPtr->messageType = (uint8)RMTREQST; 
		netMsgPtr->dataLength  = (uint16)3*DOUBLE_SIZE; 
		memoryCopy((uint8*)&reqSize,(uint8*)bufPtr,DOUBLE_SIZE);
		reqTypeSize =  ( (accDataType <<16 ) | (reqSize&0xffff) );
		
		
		memoryCopy(&netMsgPtr->dataBuff[0],(uint8*)&cmdCode,4);
		memoryCopy(&netMsgPtr->dataBuff[4],(uint8*)&busAddrPtr,4);
		memoryCopy(&netMsgPtr->dataBuff[8],(uint8*)&reqTypeSize,4);
    }
	sendData.data_len   =   netMsgPtr->dataLength + MAX_DUAL_FRAME_HEAD_LEN;
	
	
    (void)netMsgChecksumCalcu   (   (uint32)netMsgPtr->dataLength,
                                    (uint32 *) netMsgPtr->dataBuff,
	                                (uint32 *) &checkSum
                                );
	netMsgPtr->checkSum = (uint16)checkSum;

	
	status = dataSendToServerOrClient( &sendData, (void *) &peerAddr, UDP_PROTO);
	if(status!=NO_ERROR){
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
	} else {

		
		gDluNetCurSendCnt++;

		
		if(gDualTransRateCalcuFlag){
			networkPerformanceCountSet( 0x1, 
										sendData.data_len
									); 
		}
	}
#endif 
	return(status);
}


uint32	myRmtAllTypeFlagDataWrite(	uint32	accDataType, uint32 byteNum,uint32 backupFlag, 
						int8 *busAddrPtr, int8 *bufPtr)
{
	uint32	status = NO_ERROR;
	uint32  i;
	strDataBuf sendData;
    strDualProtocolInfo *netMsgPtr;
	strinetAddr localAddr, peerAddr;

	strSysConfigInfo sysCnfgInfo;

	uint32 cmdCode,reqTypeSize, checkSum;
	uint32 addrPtr;

    
    if( (accDataType != BUS_BYTE_ACC) && (accDataType != BUS_WORD_ACC) &&
        (accDataType != BUS_LONG_ACC)  && (accDataType != BUS_DLONG_ACC) 
	){
		status = BUS_DATA_ACC_TYPE_ERR;
        return(status);
    }

	memoryClear ( (uint8 *) &sysCnfgInfo, sizeof(strSysConfigInfo));
  	systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);
	
	localAddr.iAddr= sysCnfgInfo.redNetCnfgInfo.rmtDluIpAddrHex;	
	status = comPortInfoGet(RMTRESPS, &localAddr, &peerAddr); 
	if(status!=NO_ERROR){
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
        return(status);
	}

	
	sendData.socket_fd = peerAddr.sockFd;
	netMsgPtr = (strDualProtocolInfo *)sendData.data_buf;
	memoryClear ( (uint8 *)netMsgPtr , sizeof(strDualProtocolInfo));

	netMsgPtr->sequenceID  = 0;

	
	addrPtr = (uint32)(busAddrPtr);
	if(	(addrPtr&0xffffff) > MAX_MEMORY_SIZE ) {
		status = MAX_MEM_SIZE_EXCEED_ERR;
        return(status);
	}
	
	else if( ((addrPtr&0xff000000) != MEM_BOARD_BUS_ADDR_ON_MASTER) &&
		     ((addrPtr&0xff000000) != MEM_BOARD_BUS_ADDR_ON_SLAVE)
	){
		status = BUS_DATA_ACC_ERR;
		return(status);
	}
	(void)dualDataAreaGet((uint32)addrPtr, (uint8*)&netMsgPtr->dualDataArea);

	
	cmdCode= (DUAL_ACCESS_CMD<<16) | (DUAL_RUNBUS_COPY&0xffff);  

	
	netMsgPtr->messageType = RMTRESPS; 
	
	reqTypeSize =  ( (accDataType <<16 ) | (byteNum&0xffff) );
	
	memoryCopy(&netMsgPtr->dataBuff[0],(uint8*)&cmdCode,4);
	memoryCopy(&netMsgPtr->dataBuff[4],(uint8*)&busAddrPtr,4);
	memoryCopy(&netMsgPtr->dataBuff[8],(uint8*)&backupFlag,4);
	memoryCopy(&netMsgPtr->dataBuff[12],(uint8*)&reqTypeSize,4);
	netMsgPtr->dataLength  = 4*DOUBLE_SIZE;

	for(i=0;i<byteNum/accDataType; i++) 
	{
		if(accDataType == BUS_DLONG_ACC){
			memoryCopy(&netMsgPtr->dataBuff[16+i*8],(uint8*)bufPtr+i*accDataType,8);
			netMsgPtr->dataLength  += LONG_SIZE;
		}
		else {
			
			memoryCopy(&netMsgPtr->dataBuff[16+i*accDataType],(uint8*)bufPtr+i*4,accDataType);
			netMsgPtr->dataLength  += DOUBLE_SIZE; 
		}
	}

	sendData.data_len   =   netMsgPtr->dataLength + MAX_DUAL_FRAME_HEAD_LEN;

	
	
    (void)netMsgChecksumCalcu   (   (uint32)netMsgPtr->dataLength,
                                    (uint32 *) netMsgPtr->dataBuff,
	                                (uint32 *) &checkSum
                                );
	netMsgPtr->checkSum = (uint16)checkSum;

	
	status = dataSendToServerOrClient( &sendData, (void *) &peerAddr, UDP_PROTO);
	if(status!=NO_ERROR){
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
	}
	else{
		
		gDluNetCurSendCnt++;

		
		if(gDualTransRateCalcuFlag){
			networkPerformanceCountSet( 0x1, 
										sendData.data_len
									); 
		}
	}

	return(status);
}



uint32	myRmtFourByteDataWrite(	uint32	writeNum, int8 *busAddrPtr, int8 *bufPtr)
{
	uint32	status = NO_ERROR;
	strDataBuf sendData;
    strDualProtocolInfo *netMsgPtr;
	strinetAddr localAddr, peerAddr;

	strSysConfigInfo sysCnfgInfo;

	uint32 cmdCode,checkSum;
	uint32 i,*addrPtr;

	if(writeNum >  MAX_DUAL_COPY_DATA_NUM ){
		status = L_NET_MSG_LEN_ERR;
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
		return(status);
	}

	memoryClear ( (uint8 *) &sysCnfgInfo, sizeof(strSysConfigInfo));
  	systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);
  	
	
	localAddr.iAddr = sysCnfgInfo.redNetCnfgInfo.rmtDluIpAddrHex;	

	status = comPortInfoGet(RMTRESPS, &localAddr, &peerAddr); 
	if(status!=NO_ERROR){
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
        return(status);
	}

	
	sendData.socket_fd = peerAddr.sockFd;
	netMsgPtr = (strDualProtocolInfo *)sendData.data_buf;
	memoryClear ( (uint8 *)netMsgPtr , sizeof(strDualProtocolInfo));

	netMsgPtr->sequenceID = 0;
	
	
	addrPtr = (uint32*)(busAddrPtr);
	if(	(*addrPtr&0xffffff) > MAX_MEMORY_SIZE ) {
		status = MAX_MEM_SIZE_EXCEED_ERR;
        return(status);
	}
	
	else if( ((*addrPtr&0xff000000) != MEM_BOARD_BUS_ADDR_ON_MASTER) &&
		     ((*addrPtr&0xff000000) != MEM_BOARD_BUS_ADDR_ON_SLAVE)
	){
		status = BUS_DATA_ACC_ERR;
		return(status);
	}
	(void)dualDataAreaGet((uint32)*addrPtr, (uint8*)&netMsgPtr->dualDataArea);

	cmdCode= (DUAL_ACCESS_CMD<<16) | (DUAL_RUNDATA_COPY&0xffff);  

	
	netMsgPtr->messageType = RMTRESPS; 
	netMsgPtr->dataLength  = (2 + writeNum*2)*DOUBLE_SIZE ; 
	
	
	memoryCopy(&netMsgPtr->dataBuff[0],(uint8*)&cmdCode,4);
	memoryCopy(&netMsgPtr->dataBuff[4],(uint8*)&writeNum,4);
	for(i=0;i<writeNum; i++)
	{
		memoryCopy(&netMsgPtr->dataBuff[8+i*8],(uint8*)busAddrPtr,4);
		memoryCopy(&netMsgPtr->dataBuff[12+i*8],(uint8*)bufPtr,4);

		busAddrPtr+=4;
		bufPtr+=4;
	}

	sendData.data_len  =  netMsgPtr->dataLength + MAX_DUAL_FRAME_HEAD_LEN;

	
	
    (void)netMsgChecksumCalcu   (   (uint32)netMsgPtr->dataLength,
                                    (uint32 *) netMsgPtr->dataBuff,
	                                (uint32 *) &checkSum
                                );
	netMsgPtr->checkSum = (uint16)checkSum;

	
	status = dataSendToServerOrClient( &sendData, (void *) &peerAddr, UDP_PROTO);
	if(status!=NO_ERROR){
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
	}
	else{
		
		gDluNetCurSendCnt++;

		
		if(gDualTransRateCalcuFlag){
			networkPerformanceCountSet( 0x1, 
										sendData.data_len
									); 
		}
	}

	return(status);
}



uint32	myRmtAllTypeDataWrite(	uint32	writeNum, 
						int8 *busAddrPtr, int8 *bufPtr, int8 *bitType)
{
	uint32	status = NO_ERROR;
	strDataBuf sendData;
    strDualProtocolInfo *netMsgPtr;
	strinetAddr localAddr, peerAddr;

	strSysConfigInfo sysCnfgInfo;

	uint32 cmdCode,checkSum;
	uint32 i,*addrPtr;

	if(writeNum >  MAX_DUAL_COPY_TYPE_NUM )
		status = L_NET_MSG_LEN_ERR;

	memoryClear ( (uint8 *) &sysCnfgInfo, sizeof(strSysConfigInfo));
  	systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);
	
	localAddr.iAddr= sysCnfgInfo.redNetCnfgInfo.rmtDluIpAddrHex;	

	status = comPortInfoGet(RMTRESPS, &localAddr, &peerAddr); 
	if(status!=NO_ERROR){
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
        return(status);
	}

	
	sendData.socket_fd = peerAddr.sockFd;
	netMsgPtr = (strDualProtocolInfo *)sendData.data_buf;
	memoryClear ( (uint8 *)netMsgPtr , sizeof(strDualProtocolInfo));

	netMsgPtr->sequenceID = 0;

	
	addrPtr = (uint32*)(busAddrPtr);
	if(	(*addrPtr&0xffffff) > MAX_MEMORY_SIZE ) {
		status = MAX_MEM_SIZE_EXCEED_ERR;
        return(status);
	}
	
	else if( ((*addrPtr&0xff000000) != MEM_BOARD_BUS_ADDR_ON_MASTER) &&
		     ((*addrPtr&0xff000000) != MEM_BOARD_BUS_ADDR_ON_SLAVE)
	){
		status = BUS_DATA_ACC_ERR;
		return(status);
	}
	(void)dualDataAreaGet((uint32)*addrPtr, (uint8*)&netMsgPtr->dualDataArea);

	cmdCode= (DUAL_ACCESS_CMD<<16) | (DUAL_RUNTYPE_COPY&0xffff);  

	
	netMsgPtr->messageType = RMTRESPS; 
	netMsgPtr->dataLength  = (2 + writeNum*3)*DOUBLE_SIZE ; 
	
	
	memoryCopy(&netMsgPtr->dataBuff[0],(uint8*)&cmdCode,4);
	memoryCopy(&netMsgPtr->dataBuff[4],(uint8*)&writeNum,4);
	for(i=0;i<writeNum; i++)
	{
		memoryCopy(&netMsgPtr->dataBuff[8+i*12],(uint8*)busAddrPtr,4);
		memoryCopy(&netMsgPtr->dataBuff[12+i*12],(uint8*)bufPtr,4);
		memoryCopy(&netMsgPtr->dataBuff[16+i*12],(uint8*)bitType,4);

		busAddrPtr+=4;
		bufPtr+=4;
		bitType+=4;
	}

	sendData.data_len  =  netMsgPtr->dataLength + MAX_DUAL_FRAME_HEAD_LEN;

	
	
    (void)netMsgChecksumCalcu   (   (uint32)netMsgPtr->dataLength,
                                    (uint32 *) netMsgPtr->dataBuff,
	                                (uint32 *) &checkSum
                                );
	netMsgPtr->checkSum = (uint16)checkSum;

	
	status = dataSendToServerOrClient( &sendData, (void *) &peerAddr, UDP_PROTO);
	if(status!=NO_ERROR){
		setErrorCode(__FILE__, __LINE__ , __FUNCTION__, status);
	}
	else{
		
		gDluNetCurSendCnt++;

		
		if(gDualTransRateCalcuFlag){
			networkPerformanceCountSet( 0x1, 
										sendData.data_len
									); 
		}
	}

	return(status);
}


extern uint32 gVarAreaOffset,gSFCAreaOffset,gSysExeOffset;
extern uint32 gDiuAreaOffset,gDualAreaOffset,gSysNvRamAreaOffset, gUserEndOffset;

uint32 dualDataAreaGet(uint32 dualDataAddr, uint8* dataAreaPtr)
{
	uint32 status = NO_ERROR;
	uint32 offsetAddr = 0;

	offsetAddr = (dualDataAddr)&0x00ffffff;

	if(offsetAddr < gVarAreaOffset)
		*dataAreaPtr = MPU_FB_AREA;
	else if(offsetAddr < gSFCAreaOffset)
		*dataAreaPtr = VAR_DATA_AREA;
	else if(offsetAddr < gSysExeOffset)
		*dataAreaPtr = SFC_INFO_AREA;
	else if(offsetAddr < gDiuAreaOffset)
		*dataAreaPtr = SYS_EXE_AREA;
#ifdef MP3000AT_APPLICATION
	else if(offsetAddr < gDualAreaOffset)
		*dataAreaPtr = DIU_FB_AREA;
	else if(offsetAddr < gSysNvRamAreaOffset)
		*dataAreaPtr = DUAL_INFO_AREA;
	else if(offsetAddr < gUserEndOffset)
		*dataAreaPtr = FERR_DATA_AREA;
#else
	else if(offsetAddr < gSysNvRamAreaOffset)
		*dataAreaPtr = DIU_FB_AREA;
	else if(offsetAddr < gDualAreaOffset)
		*dataAreaPtr = FERR_DATA_AREA;
	else if(offsetAddr < gUserEndOffset)
		*dataAreaPtr = DUAL_INFO_AREA;
#endif
	else {
		status = MAX_MEM_SIZE_EXCEED_ERR;
	}

	return(status);
}

uint32 dualDataAreaCheck(uint32 dualDataAddr, uint8 dataArea, uint32* areaMaxAddr)
{
	uint32 status = NO_ERROR;
	uint32 offsetAddr = 0;
	char fatalErrString[128];

	if( ((dualDataAddr&0xff000000) != MEM_BOARD_BUS_ADDR_ON_MASTER) &&
		     ((dualDataAddr&0xff000000) != MEM_BOARD_BUS_ADDR_ON_SLAVE)
	){
		status = BUS_DATA_ACC_ERR;
		return(status);
	}
	offsetAddr = (dualDataAddr)&0x00ffffff;

	*areaMaxAddr = 0; 
	switch(dataArea){
		case MPU_FB_AREA:
			*areaMaxAddr = gVarAreaOffset;
		break;
		case VAR_DATA_AREA:
			*areaMaxAddr = gSFCAreaOffset;
		break;
		case SFC_INFO_AREA:
			*areaMaxAddr = gSysExeOffset;
		break;
		case SYS_EXE_AREA:
			*areaMaxAddr = gDiuAreaOffset;
		break;
#ifdef MP3000AT_APPLICATION
		case DIU_FB_AREA:
			*areaMaxAddr = gDualAreaOffset;
		break;
		case DUAL_INFO_AREA:
			*areaMaxAddr = gSysNvRamAreaOffset;
		break;
#else
		case DIU_FB_AREA:
			*areaMaxAddr = gSysNvRamAreaOffset;
		break;
		case DUAL_INFO_AREA:
			*areaMaxAddr = gUserEndOffset;
		break;
#endif
		case DUAL_DEFAULT_AREA: 
			return(status);
		break;

		case FERR_DATA_AREA:	
		default:
			status = BUS_DATA_ACC_ERR;
			return(status);
		break;
	}

	if(offsetAddr>=(*areaMaxAddr)) {
		status = MAX_MEM_SIZE_EXCEED_ERR;
		setErrorCodeWithVal(__FILE__, __LINE__ , __FUNCTION__, status,
							"DUAL ADDR",dualDataAddr,
							"DUAL AREA ",dataArea,
							"AREA MAX ADDR",*areaMaxAddr);

		sprintf((char *)fatalErrString,"0x%x(%d)",dualDataAddr,dataArea);
		logMessageSet(__FILE__, __LINE__,fatalErrString, RMT_DUAL_MEM_COPY_ERR, 0);	
		return(status);
	}

	return(status);
}
#endif 
