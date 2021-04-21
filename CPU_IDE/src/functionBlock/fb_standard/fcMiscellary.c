#include	<stdio.h>
#include	<math.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"

#include	"fbDef.h"

#include	"fb_code\fb_common\fbRunningErrCode.h"
#include	"fbMem.h"
#include	"sysDual.h"
#include	"netProtocol.h"
#include	"sysConfig.h"
#include	"protocolBuf.h"
#include	"cmdList.h"
#include	"systemDiag.h"
#include	"pmsUtil.h"

#include    "..\MHCAP\project_def.h"

#include	"fb_code\fb_standard\fcMiscellary.h"

/**************************************************************************
**		외부 변수 
**************************************************************************/
extern SWI_GT_PORT		*gtx_map_data[MAX_LINK_CNT];
extern unnCpuRasInfo gunHvdcCpuRasInfo;
extern uint32	M_MEMORY_AREA_START_OFFSET;

/**************************************************************************
**		외부 함수 
**************************************************************************/
extern uint32	readRuntimeFbData(uint32, uint32, uint32, uint32 *);
extern uint32	writeRuntimeFbData(uint32, uint32, uint32, uint32 *);
extern void 	setErrorCode(uint8 *, uint32,uint8 *,uint32);
extern uint32	netDataSendInFbTask(strNewNetProtocolInfo	*);
extern uint32	systemCnfgInfoRead( strSysConfigInfo *);
extern uint32 	fbMemInfoRead(uint32, uint32, strFbMemInfo *);
extern uint32 	getLogicTaskCycletime(uint32 , uint32 , float *);
extern void 	memoryClear ( uint8 *, uint32 );
extern void 	memoryCopy ( uint8 *, uint8 *, uint32 );
extern uint32 	fbMemPointerGet(uint32, strMemPointerInfo *);

/*********************************************************************
**	
**	스테이션간 Ethernet 통신, 즉 Read/Write 통신 FB 	
**
**********************************************************************/
uint32	ethDSndInitFunc
				(
			uint32	*specTypePtr,
            uint32	*varTypePtr,
            uint32	*outputTypePtr,
            strFbDefInfo	*paraInfoPtr
            	)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= ETH_SEND_CODE;
    paraInfoPtr->inputNo	= ETH_SEND_SPEC_NUM;
    paraInfoPtr->intlVarNo	= ETH_SEND_VAR_NUM;
    paraInfoPtr->outputNo	= ETH_SEND_OUTPUT_NUM;

	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= WORD_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= WORD_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= WORD_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= WORD_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr	= INT_TYPE|SIZE32_TYPE;	
	
	*varTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*varTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*varTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*varTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*varTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*varTypePtr++	= REAL_TYPE|SIZE32_TYPE; 
	*varTypePtr++	= REAL_TYPE|SIZE32_TYPE;
	*varTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*varTypePtr		= INT_TYPE|SIZE32_TYPE;		
    
	*outputTypePtr++= INT_TYPE|SIZE32_TYPE;			
	*outputTypePtr++= INT_TYPE|SIZE32_TYPE;			
	*outputTypePtr++= INT_TYPE|SIZE32_TYPE;			
	*outputTypePtr++= INT_TYPE|SIZE32_TYPE;			
	*outputTypePtr++= INT_TYPE|SIZE32_TYPE;			
	*outputTypePtr++= INT_TYPE|SIZE32_TYPE;			
	*outputTypePtr++= INT_TYPE|SIZE32_TYPE;			
	*outputTypePtr++= INT_TYPE|SIZE32_TYPE;			
	*outputTypePtr  = INT_TYPE|SIZE32_TYPE;			
	
    return(status);
}


uint32	ethDSndRunFunc(uint32 taskId, uint32 fbBlockAddr)
{
	uint32	status = NO_ERROR;
	strFC0302Info	fd;
	strNewNetProtocolInfo	send_buff;
	uint32	tmp_buff[256];
	float cycleTime, comPeriod, respondDelay;
	uint32	comDeviceType, masterALineIP, masterBLineIP, slaveALineIP, slaveBLineIP;
 	strSysConfigInfo sysCnfgInfo;
 	uint32	stationId, dualId,cmdCode;

    strMemPointerInfo memInfoPtr;
	uint32 i,dataBuf[200];
	float  timeOut;
	uint8* write8Ptr; uint16* write16Ptr; uint32* write32Ptr;
	uint32  tcpComShelf = ETH_MASTER_COM;

    
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
    
    
	status = getLogicTaskCycletime(	LOGIC_ID,
							taskId,
							(float *)&cycleTime
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
	}
    
    
	systemCnfgInfoRead( &sysCnfgInfo);
	stationId 	= sysCnfgInfo.stationId;
	dualId		= sysCnfgInfo.shelfType;

 	
 	comDeviceType = fd.deviceSelect;	
 
 	
 	
 	comPeriod = fd.comPeriodNo * cycleTime;			
 	respondDelay = fd.respondDelayNo * comPeriod * 2;	
 
 	
 	masterALineIP 	= (fd.masterALineIP & 0xffffffff);
 	masterBLineIP 	= (fd.masterBLineIP & 0xffffffff);
 	slaveALineIP 	= (fd.slaveALineIP & 0xffffffff);
 	slaveBLineIP 	= (fd.slaveBLineIP & 0xffffffff);

 	
	
 	if( ( (fd.runMode == ETH_SEND_READ) || (fd.runMode == ETH_SEND_WRITE) )&&
		( (comDeviceType == 0) 
			|| ((masterALineIP == 0) && (slaveALineIP == 0)	&& (masterBLineIP == 0) && (slaveBLineIP == 0) )
			|| ((comDeviceType == LK11_CPU) && (fd.deviceStationNo > MAX_STATION_ID))
			|| (fd.dataType == 0) 
		)
	  )
	{    
     	fd.runModeStatOut 	= fd.runMode + 2 ;	 
     	fd.specErrOut 		= 1;		

     	setErrorCode(	__FILE__, __LINE__, __FUNCTION__, FB_INPUT_DATA_ERR);  
		
    	status = writeRuntimeFbData(	taskId,
    								LOGIC_ID,
            	                    fbBlockAddr,
                	                (uint32 *)&fd
							     );

		if(status) 
		{
			setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
			return(status);
    	}
    
  		status = FB_INPUT_DATA_ERR;
 		
 		return(status);
 	} 	
 	else
    	fd.specErrOut = 0;
 	
 	if(fd.runMode == ETH_SEND_READ) 
 	{ 
    	if((fd.comComand == ETH_COM_ACCEPT) 
     		&& (fd.runStatus != ETH_NO_RUN) && (fd.lineStatus != ETH_COM_NO_LINE)) 
     	{  
			
 			fd.periodSum += cycleTime;
			
			if((fd.periodSum >= comPeriod) && (fd.dataSendFlag == 0)) 	 
			{  			
				memoryCopy(send_buff.companyID, COMPANY_ID,8);
     			send_buff.dataLength 		= 0;	
     			send_buff.sequenceID  		= ++fd.sequenceNo;			
     			send_buff.sourceID			= stationId;				
     			send_buff.destinationID		= fd.deviceStationNo;	   	
     			send_buff.messageType		= REQUEST;					
     			send_buff.dualStatus		= (dualId<<16);				
				send_buff.loopID			= 0; 
  			
				
				cmdCode= (ETHERNET_DATA_ACC_OP<<16) | (ETHERNET_DATA_READ&0xffff);  
				memoryCopy( (uint8 *) &send_buff.dataBuff[0], (uint8 *)&cmdCode, DOUBLE_SIZE);
				send_buff.dataLength += DOUBLE_SIZE;

 	    			
 	    		if(fd.runStatus == 1) 
				{  
					slaveALineIP = 0;
					slaveBLineIP = 0;
					
     	    		if(fd.lineStatus == 1) 
     	    		{     
						masterBLineIP = 0;
						tmp_buff[0] = masterALineIP;							
                    	tmp_buff[1] = masterBLineIP; 
     				}
                	else if(fd.lineStatus == 2) 
                	{
						masterALineIP = 0;
						tmp_buff[0] = masterALineIP;	
         	    		tmp_buff[1] = masterBLineIP;    
                	}
     	    		else
     	    		{     
         	    		tmp_buff[0] = masterALineIP;    	
         	    		tmp_buff[1] = masterBLineIP;     
     	   			}

					tcpComShelf = ETH_MASTER_COM;
 	    		}
 	    		else 
				{ 
					masterALineIP = 0;
					masterBLineIP = 0;
					
     				if(fd.lineStatus == 1) 
     				{    
						slaveBLineIP = 0;
         	    		tmp_buff[0] = slaveALineIP;		
         	    		tmp_buff[1] = slaveBLineIP; 
     	        	}
     	        	else if(fd.lineStatus == 2) 
     	        	{
						slaveALineIP = 0;
         	    		tmp_buff[0] = slaveALineIP;	
         	    		tmp_buff[1] = slaveBLineIP;     
     	    		}
            		else
            		{    
         	    		tmp_buff[0] = slaveALineIP;     
         	    		tmp_buff[1] = slaveBLineIP;     
     	        	}

					tcpComShelf = ETH_SLAVE_COM;
            	}
            	
 	    		tmp_buff[2] = fbBlockAddr;        
 	    		tmp_buff[3] = fd.memoryAddress;   
 	    		tmp_buff[4] = comDeviceType;      
 	    		tmp_buff[5] = fd.dataType;        
 	    		tmp_buff[6] = fd.dataNumber;      
           
				
				memoryCopy( (uint8 *) &send_buff.dataBuff[4], (uint8 *)&tmp_buff[0], 7*DOUBLE_SIZE );
				send_buff.dataLength += DOUBLE_SIZE*7;

          	  
    			status = netDataSendInFbTask((strNewNetProtocolInfo	*)&send_buff);
     			if(status) 
     			{
					setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
					return(status);
    			}	
	    		
            	if((status == NO_ERROR)) 
            	{
       	        	fd.sendCountOut++;		
	        		fd.periodSum = 0;		
                	fd.dataSendFlag = 1;    	
     	    	}
     	    	else 
     	    	{
 	        		fd.sendErrCountOut++;	
					fd.sequenceNo--;
                	fd.dataSendFlag = 0;    	

					setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
					return(status);
	    		}	
			}  
 
        	if(fd.rcvTimeoutCheckFlag == ETH_RMT_COM_RCV_DONE) 
        	{ 
	    		fd.rcvTimeoutCheckFlag = 0;		
	    		fd.respondDelaySum = 0;		

            	fd.seqNoMisErrCntOut = fd.seqNumErrCount;	 		
	    		fd.runModeStatOut = fd.runMode;  	           
				
				
               	fd.dataSendFlag = 0;    	
			} 
        	else 
        	{ 
 	    		fd.respondDelaySum += cycleTime;
	    		if(fd.respondDelaySum >= respondDelay) 
	    		{	 
					fd.rcvTimeoutErrCntOut++;                 	
					fd.runModeStatOut = fd.runMode+2;          
	        		fd.respondDelaySum = 0; 	          	  
				
					
               		fd.dataSendFlag = 0;    	
	    		}
        	}
     	} 
     	else 
     	{	
        	if(fd.rcvTimeoutCheckFlag == ETH_RMT_COM_RCV_DONE) 
        	{ 	
	    		fd.seqNoMisErrCntOut = fd.seqNumErrCount;
	    		fd.runModeStatOut = fd.runMode+2;  	 
	    		fd.respondDelaySum = 0;				
 	    		fd.periodSum = 0;           		
           		fd.dataSendFlag = 0;    				
			} 
        	else if(fd.dataSendFlag == 1) 
        	{ 	
 	    		
 	    		fd.respondDelaySum += cycleTime;

	    		if(fd.respondDelaySum >= respondDelay) 
	    		{  
					fd.rcvTimeoutErrCntOut++;          		
					fd.runModeStatOut = fd.runMode+2;  	 
	        		fd.respondDelaySum = 0; 	   		
                	fd.dataSendFlag = 0;    	   			
	    		}
        	}
        	else 
        	{
 				
 				fd.periodSum = 0;
 				fd.respondDelaySum = 0;
        	}
     	}  
 	} 
	
 	else if(fd.runMode == ETH_SEND_WRITE) 
 	{ 
    	if((fd.comComand == ETH_COM_ACCEPT) 
     		&& (fd.runStatus != ETH_NO_RUN) && (fd.lineStatus != ETH_COM_NO_LINE)) 
     	{  
 			fd.periodSum += cycleTime;

			if((fd.periodSum >= comPeriod) && (fd.dataSendFlag == 0)) 	 
			{  		
				memoryCopy(send_buff.companyID, COMPANY_ID,8);
     			send_buff.dataLength 		= 0;	
     			send_buff.sequenceID  		= ++fd.sequenceNo;			
     			send_buff.sourceID			= stationId;				
     			send_buff.destinationID		= fd.deviceStationNo;	   	
     			send_buff.messageType		= REQUEST;					
     			send_buff.dualStatus		= (dualId<<16);				  
				send_buff.loopID			= 0; 

				
				cmdCode= (ETHERNET_DATA_ACC_OP<<16) | (ETHERNET_DATA_WRITE&0xffff);  
				memoryCopy( (uint8 *) &send_buff.dataBuff[0], (uint8 *)&cmdCode, DOUBLE_SIZE);
				send_buff.dataLength += DOUBLE_SIZE;

				
 	    			
 	    		if(fd.runStatus == 1) 
				{  
					slaveALineIP = 0;
					slaveBLineIP = 0;
					
     	    		if(fd.lineStatus == 1) {     
						masterBLineIP = 0;
         	    		tmp_buff[0] = masterALineIP;	
                    	tmp_buff[1] = masterBLineIP; 
     				}
                	else if(fd.lineStatus == 2) {
						masterALineIP = 0;
         	    		tmp_buff[0] = masterALineIP;	
         	    		tmp_buff[1] = masterBLineIP;    
                	}
     	    		else{     
         	    		tmp_buff[0] = masterALineIP;    	
         	    		tmp_buff[1] = masterBLineIP;     
     	   			}

					tcpComShelf = ETH_MASTER_COM;
 	    		}
 	    		else 
				{ 
					masterALineIP = 0;
					masterBLineIP = 0;
					
     				if(fd.lineStatus == 1) {    
						slaveBLineIP = 0;
         	    		tmp_buff[0] = slaveALineIP;		
         	    		tmp_buff[1] = slaveBLineIP; 
     	        	}
     	        	else if(fd.lineStatus == 2) {
						slaveALineIP = 0;
         	    		tmp_buff[0] = slaveALineIP;	
         	    		tmp_buff[1] = slaveBLineIP;     
     	    		}
            		else{    
         	    		tmp_buff[0] = slaveALineIP;     
         	    		tmp_buff[1] = slaveBLineIP;     
     	        	}

					tcpComShelf = ETH_SLAVE_COM;
            	}

 	    		tmp_buff[2] = fbBlockAddr;        
 	    		tmp_buff[3] = fd.memoryAddress;   
 	    		tmp_buff[4] = comDeviceType;      
 	    		tmp_buff[5] = fd.dataType;        
 	    		tmp_buff[6] = fd.dataNumber;      
          
 				memoryClear((uint8*)&memInfoPtr,sizeof(strMemPointerInfo));
				status = fbMemPointerGet(LOGIC_ID,&memInfoPtr);
				if(status != NO_ERROR) 
				{
					setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
					return(status);
				}

    			if(fd.dataType == ETH_BYTE_TYPE) 
    			{ 
					for(i=0; i<fd.dataNumber; i++) 
					{
						write8Ptr = (uint8*) ((int8*)(memInfoPtr.stSharedMemPtr) + fd.srcSharedMemAddr+i);
						dataBuf[i] = (uint32)(*write8Ptr);
					}
    			}
    			
    			else if(fd.dataType == ETH_WORD_TYPE) 
    			{
					for(i=0; i<fd.dataNumber; i++) 
					{
						write16Ptr = (uint16*) ((int16*)(memInfoPtr.stSharedMemPtr) + fd.srcSharedMemAddr+i);
						dataBuf[i] = (uint32)(*write16Ptr);
					}
    			}
    			else if(fd.dataType == ETH_LWORD_TYPE) 
    			{ 
					for(i=0; i<fd.dataNumber; i++)
					{
						write32Ptr = (uint32*) ((int32*)(memInfoPtr.stSharedMemPtr) + fd.srcSharedMemAddr+i);
						dataBuf[i] = (uint32)(*write32Ptr);
					}
    			}
				else
				{
					status = FB_INPUT_DATA_ERR;
					setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
					return(status);
				}
				
				memoryCopy ( (uint8 *) &tmp_buff[7], (uint8 *) &dataBuf[0], fd.dataNumber*DOUBLE_SIZE);                       						      

				
				memoryCopy( (uint8 *) &send_buff.dataBuff[4], (uint8 *)&tmp_buff[0], (7+fd.dataNumber)*DOUBLE_SIZE );		
				send_buff.dataLength += (7+fd.dataNumber)*DOUBLE_SIZE;
          	  
     			
    			status = netDataSendInFbTask((strNewNetProtocolInfo	*)&send_buff);
     			if(status) 
     			{
					setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
					return(status);
    			}	

            	if((status == NO_ERROR)) {
       	        	fd.sendCountOut++;		
	        		fd.periodSum = 0;		
                	fd.dataSendFlag = 1;    	
     	    	}
     	    	else {
 	        		fd.sendErrCountOut++;	
					fd.sequenceNo--;
	    		}	
			}  

        	 
 			
        	 
        	if(fd.rcvTimeoutCheckFlag == ETH_RMT_COM_RCV_DONE) { 
	    		fd.rcvTimeoutCheckFlag = 0;			
	    		fd.respondDelaySum = 0;			

            	fd.seqNoMisErrCntOut = fd.seqNumErrCount;	 	
	    		fd.runModeStatOut = fd.runMode;  	          
				
               	fd.dataSendFlag = 0;    	

			} 
        	else { 
 	    		fd.respondDelaySum += cycleTime;
	    		if(fd.respondDelaySum >= respondDelay) {	 
					fd.rcvTimeoutErrCntOut++;                		
					fd.runModeStatOut = fd.runMode+2;        	 
	        		fd.respondDelaySum = 0; 	          		
					
			       	fd.dataSendFlag = 0;    	
	    		}
        	}
     	} 
     	else {	
        	
 			
        	
        	if(fd.rcvTimeoutCheckFlag == ETH_RMT_COM_RCV_DONE) { 
	    		fd.seqNoMisErrCntOut = fd.seqNumErrCount;
	    		fd.runModeStatOut = fd.runMode+2;  	 
	    		fd.respondDelaySum = 0;				
 	    		fd.periodSum = 0;           		
           		fd.dataSendFlag = 0;    				
			} 
        	else if(fd.dataSendFlag == 1) { 	
 	    		
 	    		fd.respondDelaySum += cycleTime;
 	    		timeOut = fd.respondDelaySum;

	    		if(fd.respondDelaySum >= respondDelay) {	 
					fd.rcvTimeoutErrCntOut++;          			
					fd.runModeStatOut = fd.runMode+2;  		 
	        		fd.respondDelaySum = 0; 	   			
                	fd.dataSendFlag = 0;    	   				
	    		}
        	}
        	else {
 				
 				fd.periodSum = 0;
 				fd.respondDelaySum = 0;
        	}
     	}  
 	} 

	else{
     	fd.runModeStatOut = 0 ;	 
     	fd.specErrOut = 1;		
	}

 	
 	fd.rcvDataNumberOut = fd.rcvDataNumber;   
 	fd.rcvFrameErrCntOut = fd.rcvFrameErrCnt; 


    
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

/*********************************************************************
**	
**	특정 스테이션에서 네트웍 상에 있는 모든 스테이션에 데이터를 공유할 수 있는 Ethernet 통신
**	즉 Broadcast 통신 FB 	
**
**********************************************************************/
uint32	ethDBrdInitFunc
				(
			uint32	*specTypePtr,
            uint32	*varTypePtr,
            uint32	*outputTypePtr,
            strFbDefInfo	*paraInfoPtr
            	)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= ETH_BROAD_CODE;
    paraInfoPtr->inputNo	= ETH_BROAD_SPEC_NUM;
    paraInfoPtr->intlVarNo	= ETH_BROAD_VAR_NUM;
    paraInfoPtr->outputNo	= ETH_BROAD_OUTPUT_NUM;

	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*specTypePtr++	= INT_TYPE|SIZE32_TYPE;	

	
	
	*varTypePtr++	= REAL_TYPE|SIZE32_TYPE; 
	*varTypePtr++	= INT_TYPE|SIZE32_TYPE;	
	*varTypePtr		= INT_TYPE|SIZE32_TYPE;		
	
    
	*outputTypePtr++= INT_TYPE|SIZE32_TYPE;			
	*outputTypePtr++= INT_TYPE|SIZE32_TYPE;			
	*outputTypePtr++= INT_TYPE|SIZE32_TYPE;			
	*outputTypePtr++= INT_TYPE|SIZE32_TYPE;			


	
    return(status);
}


uint32	ethDBrdRunFunc(uint32 taskId, uint32 fbBlockAddr)
{
	uint32	status = NO_ERROR;
	strFC0303Info	fd;
	strNewNetProtocolInfo	send_buff;
	uint32	tmp_buff[256]; 	
 	int32  	dataBuf[200];
	float 	cycleTime, comPeriod;
	uint32	i;
 	strSysConfigInfo sysCnfgInfo;
 	uint32	stationId, dualId,cmdCode;
	strMemPointerInfo	memInfoPtr;
 	uint8* write8Ptr; uint16* write16Ptr; uint32* write32Ptr;

    
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
    
    
	status = getLogicTaskCycletime(	LOGIC_ID,
							taskId,
							(float *)&cycleTime
						   );
	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
	}
	
	
	systemCnfgInfoRead( &sysCnfgInfo);
	
	stationId 	= sysCnfgInfo.stationId;
	dualId		= sysCnfgInfo.shelfType;
	
 	
 	
 	comPeriod = fd.comPeriodNo * cycleTime ;			
 	
	
	fd.enableOut = fd.enableIn;
	if(fd.enableIn != FUNCTION_ENABLE) 
	{
		fd.sendCountOut 	= 0;
		fd.sendErrCountOut 	= 0;

		goto PARA_WRITE;
	}

	
 	fd.periodSum += cycleTime;

	if(fd.periodSum >= comPeriod) 
	{ 
		
 		memoryCopy(send_buff.companyID, COMPANY_ID,8);
 		
     	send_buff.dataLength 		= 0;	
     	send_buff.sequenceID  		= ++fd.sequenceNo;			
     	send_buff.sourceID			= stationId;				
     	send_buff.destinationID		= 0;						
     	send_buff.messageType		= BROADCAST;				
     	send_buff.dualStatus		= dualId;					  
		send_buff.loopID			= 0; 

		
		cmdCode= (ETHERNET_DATA_ACC_OP<<16) | (ETHERNET_DATA_BROAD&0xffff);  
		memoryCopy( (uint8 *) &send_buff.dataBuff[0], (uint8 *)&cmdCode, DOUBLE_SIZE);
		send_buff.dataLength += DOUBLE_SIZE;
		
 	    tmp_buff[0] = fd.memoryAddress;   
 	    tmp_buff[1] = fd.dataType;        
 	    tmp_buff[2] = fd.dataNumber;      
  
 		memoryClear((uint8*)&memInfoPtr,sizeof(strMemPointerInfo));

		status = fbMemPointerGet(LOGIC_ID,&memInfoPtr);
		if(status != NO_ERROR) 
		{
			setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
			return(status);
		}

   		 
	    if(fd.dataType == ETH_BYTE_TYPE) 
	    { 
			for(i=0; i<fd.dataNumber; i++) 
			{
				dataBuf[i] = 0;
				write8Ptr = (uint8*) ((uint8*)(memInfoPtr.stSharedMemPtr) + fd.memoryAddress+i);
			    dataBuf[i] = (uint32)(*write8Ptr);
			}
	    }
	    else if(fd.dataType == ETH_WORD_TYPE) 
	    {
			for(i=0; i<fd.dataNumber; i++) 
			{
				dataBuf[i] = 0;
				write16Ptr = (uint16*) ((uint16*)(memInfoPtr.stSharedMemPtr) + fd.memoryAddress+i);
			    dataBuf[i] = (uint32)(*write16Ptr);
			}
	    }
	    else 
	    { 
			for(i=0; i<fd.dataNumber; i++)
			{
				dataBuf[i] = 0;
				write32Ptr = (uint32*) ((uint32*)(memInfoPtr.stSharedMemPtr) + fd.memoryAddress+i);
			    dataBuf[i] = (uint32)(*write32Ptr);
			}
	    }

		
		memoryCopy ( (uint8 *) &tmp_buff[3], (uint8 *) &dataBuf[0], fd.dataNumber*DOUBLE_SIZE);                       						      

		
		memoryCopy( (uint8 *) &send_buff.dataBuff[4], (uint8 *)&tmp_buff[0], (3+fd.dataNumber)*DOUBLE_SIZE );		
		send_buff.dataLength += (3+fd.dataNumber)*DOUBLE_SIZE;
      
     	
     	status = netDataSendInFbTask((strNewNetProtocolInfo	*)&send_buff);
     	if(status) 
     	{
			setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
			return(status);
    	}	

        if((status == NO_ERROR)) 
        {
       	    fd.sendCountOut++;		
	        fd.periodSum = 0;		
            fd.dataSendFlag = 1;    	
     	}
     	else 
     	{
 	        fd.sendErrCountOut++;	
	    }	
	}  


PARA_WRITE:
    
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

/*********************************************************************
**	
**	CPU 보드위에 장착되는 광 통신 PMC모듈을 Access하는 FB
**
[주의 사항]
	현재는 DWORD 기준으로 데이터 Access
**********************************************************************/
uint32	pmc502RdInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;
	uint32	iLoop;

	paraInfoPtr->fbId		= PMC502_RD_CODE;
    paraInfoPtr->inputNo	= PMC502_RD_SPEC_NUM;
    paraInfoPtr->intlVarNo	= PMC502_RD_VAR_NUM;
    paraInfoPtr->outputNo	= PMC502_RD_OUTPUT_NUM;

	for(iLoop = 0; iLoop < PMC502_RD_SPEC_NUM; iLoop++, specTypePtr++)
	{
		*specTypePtr	= INT_TYPE | SIZE32_TYPE;
	}

	for(iLoop = 0; iLoop < PMC502_RD_OUTPUT_NUM; iLoop++, outputTypePtr++)
	{
		*outputTypePtr	= INT_TYPE | SIZE32_TYPE;
	}

 	return (status);	 	
}

uint32	pmc502RdRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = 0; 
	strFC0304Info fd;
	int i;
	uint32  srcMemOffsetPtr = 0, busSrcMemOffsetPtr;
	uint32  dstMemOffsetPtr, busDstMemOffsetPtr;
	uint8   scanDataByte, oldScanDataByte;
	uint16  scanDataWord, oldScanDataWord;
	uint32  scanDataDword, oldScanDataDword;
	
  	strMemPointerInfo memInfoPtr;
  	strMemPointerInfo busMemInfoPtr;
    
    ushort	u16_data[512];

	uint32  useDataNumber;
	uint32  maxDataNumber, maxDataSize;
	uint32	byte1, byte2, byte3;
	uint32	word1, word2, word3;
	
	uint32	*gtx_rx_buf;
	uint32 read_data, swap_data, read_dump[128], head_data, data_length, readCrc32, seqNo, preSeqNo;
	static uint32 seqIncCnt=0, seqHoldCnt=0, headErrCnt=0, crcErrCnt=0;
	uint8 rxStatus;
	
	uint32	factor;
	
	/***********************************************************************************
	**	FB 파라미터 Read
	***********************************************************************************/
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

	/***********************************************************************************
	**	입력 파라미터 Check
	***********************************************************************************/
	/* 1. Port ID */
	if((fd.opticPortId >= MIN_PMC502_PORT_ID)&&(fd.opticPortId <= MAX_PMC502_PORT_ID))
	{
	     /* Link Error Check */
	     Lkp502GetRxErrorStatus (fd.opticPortId,&rxStatus);
	     
    	if(rxStatus != NO_ERROR)
    	{
    		fd.accErrId = PMC502_LINK_ERR;	
    		goto MODULE_END;
    	}
		else
		{
			fd.accErrId = PMC502_NORMAL;
		}
	}
	else
	{
		fd.accErrId = PMC502_INPUT_PARA_ERR;
		goto MODULE_END;
	}

	/*  2. Data Type Check */
	if( (fd.readDataType != ETH_BYTE_TYPE) &&
		(fd.readDataType != ETH_WORD_TYPE) &&
		(fd.readDataType != ETH_LWORD_TYPE)) 
	{
		fd.accErrId = PMC502_INPUT_PARA_ERR;
		goto MODULE_END;
	}	
		
	/* 3. 저장 메모리 종류 Check */	
	if( (fd.saveMemKindOfReadData != AIO_FB_MEM_M_REGION) &&
		(fd.saveMemKindOfReadData != AIO_FB_MEM_S_REGION)) 
	{
		fd.accErrId = PMC502_INPUT_PARA_ERR;
		goto MODULE_END;
	}	

	/* 4. 데이터 갯수 Check */
    switch(fd.readDataType) 
	{
		case ETH_BYTE_TYPE:
			factor = 4;
		break;
		
		case ETH_WORD_TYPE:
			factor = 2;
		break;
		case ETH_LWORD_TYPE:
			factor = 1;
		break;
		default:
			status = FB_INPUT_DATA_ERR;
			setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
			return(status);
		break;
	}
	
	if((fd.numOfReadData / factor) > (MAX_PMC502_READ_DATA_NUM))
	{
		fd.accErrId = PMC502_INPUT_PARA_ERR;
		goto MODULE_END;		
	}
	
	
	if( fd.accErrId != PMC502_NORMAL)
	{
		goto MODULE_END;
	}
			
	/***********************************************************************************
	**	PMC502 Access
	***********************************************************************************/
	status = fbMemPointerGet(LOGIC_ID,&memInfoPtr);
	if(status != NO_ERROR) 
	{
   		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
	}

	status = busFbMemPointerGet(LOGIC_ID,&busMemInfoPtr);
	if(status != NO_ERROR) 
	{
   		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
	}
	
	if(fd.saveMemKindOfReadData == AIO_FB_MEM_S_REGION) 
	{
		dstMemOffsetPtr 	= memInfoPtr.stSharedMemPtr;
		busDstMemOffsetPtr 	= busMemInfoPtr.stSharedMemPtr;
	} 
	else if(fd.saveMemKindOfReadData == AIO_FB_MEM_M_REGION) 
	{
		dstMemOffsetPtr 	= memInfoPtr.flagMemPtr;
		busDstMemOffsetPtr 	= busMemInfoPtr.stSharedMemPtr;
	} 
	else 
	{
		status = FB_INPUT_DATA_ERR;
   		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
	}
	
	if(fd.saveMemKindOfReadData == AIO_FB_MEM_M_REGION)
	{ 
		maxDataSize = (MAX_MPU_MMEM_NUM*4);
    }
    else
    {
		maxDataSize = MAX_SHARED_MEM_SIZE;
    }
    
    switch(fd.readDataType) 
	{
		case ETH_BYTE_TYPE:
			maxDataNumber = maxDataSize;
		break;
		
		case ETH_WORD_TYPE:
			maxDataNumber = maxDataSize/2;
		break;
		case ETH_LWORD_TYPE:
			maxDataNumber = maxDataSize/4;
		break;
		default:
			maxDataNumber = 0;
			status = FB_INPUT_DATA_ERR;
			setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
			return(status);
		break;
	}
				
	useDataNumber = fd.numOfReadData;

	if( (fd.saveMemKindOfReadData == AIO_FB_MEM_S_REGION )  &&
	    ( (uint32)(memInfoPtr.flagMemPtr + maxDataNumber) > (uint32)(srcMemOffsetPtr + fd.startAddrOfMem + useDataNumber)  )
	) {
		dstMemOffsetPtr 	= memInfoPtr.stSharedMemPtr;
		busDstMemOffsetPtr 	= busMemInfoPtr.stSharedMemPtr;
	} 
	else if( (fd.saveMemKindOfReadData == AIO_FB_MEM_M_REGION )  &&
	    ( (uint32)(memInfoPtr.flagMemPtr + maxDataNumber) > (uint32)(srcMemOffsetPtr + fd.startAddrOfMem + useDataNumber)  )
	) {
		dstMemOffsetPtr 	= memInfoPtr.flagMemPtr;
		busDstMemOffsetPtr 	= busMemInfoPtr.flagMemPtr;
	}
	else 
	{
		status = FB_INPUT_DATA_ERR;
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
	}
	
	/* PMC502 데이터 Read */
	gtx_rx_buf	= (uint32 *)gtx_map_data[fd.opticPortId]->pRxSpace;
	
	fourBytesDataCopy(	(uint32*)read_dump, 
						(uint32*)gtx_rx_buf,
						fd.numOfReadData*factor
					 ); 

	for(i=0; i < useDataNumber; i++) 
	{
		switch(fd.readDataType) 
		{
			case ETH_BYTE_TYPE:
			case ETH_WORD_TYPE:
			break;
			case ETH_LWORD_TYPE:
				read_data 		= read_dump[i];
				swap_data 		= SWAP_DWORD(read_data);
				read_dump[i] 	= swap_data;
				scanDataDword 	= read_dump[i];
				    
				*(uint32*)((int32*)(dstMemOffsetPtr) + fd.startAddrOfMem + i) 		= scanDataDword;
				*(uint32*)((int32*)(busDstMemOffsetPtr) + fd.startAddrOfMem + i) 	= scanDataDword;
				
				if(i == 2)
				    fd.readComSeqNum = scanDataDword;
				    
				if((i == 0) && (scanDataDword != PMC502_COM_START_CODE))
				    fd.accErrId = PMC502_FRAME_ERR;
			 
			break;
			default:
				status = FB_INPUT_DATA_ERR;
				setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
				return(status);
			break;
		}
	} /* for(i=0; i<useDataNumber; i++) */

MODULE_END:

	/***********************************************************************************
	**	FB 파라미터 Write
	***********************************************************************************/
    status = writeRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						     );

	if(status) 
	{
		setErrorCode(	__FILE__, __LINE__, __FUNCTION__, status);
    }

    return(status);
}

/*********************************************************************
**	
**	시스템 진단 정보를 HMI(CIMON)이 Modbus를 통해 읽어갈 수 있도록 특정 메모리에 Write
**
[주의 사항]
	M영역의 주소는 5000 ~ 9000으로 고정 됨

[추후 구현]
	FB Code를 다시 구현 해야 함
**********************************************************************/
uint32	sysStaCpInitFunc	(uint32 *specTypePtr, uint32 *varTypePtr, uint32 *outputTypePtr, strFbDefInfo *paraInfoPtr)
{
	uint32	status = NO_ERROR;

	paraInfoPtr->fbId		= SYSCFG_CODE;
    paraInfoPtr->inputNo	= SYSCFG_SPEC_NUM;
    paraInfoPtr->intlVarNo	= SYSCFG_VAR_NUM;
    paraInfoPtr->outputNo	= SYSCFG_OUTPUT_NUM;

	/* 입력 - ?개 */
	*specTypePtr++	= INT_TYPE | SIZE32_TYPE;
	*specTypePtr++	= INT_TYPE | SIZE32_TYPE;
	*specTypePtr++	= WORD_TYPE | SIZE32_TYPE;
	*specTypePtr++	= WORD_TYPE | SIZE32_TYPE;
	*specTypePtr++	= WORD_TYPE | SIZE32_TYPE;
	*specTypePtr++	= WORD_TYPE | SIZE32_TYPE;
	*specTypePtr++	= WORD_TYPE | SIZE32_TYPE;
	*specTypePtr++	= WORD_TYPE | SIZE32_TYPE;

	/* 내부변수 - ?개 */	

    /* 출력 - ?개 */
	*outputTypePtr++	= INT_TYPE | SIZE32_TYPE;
	*outputTypePtr++	= INT_TYPE | SIZE32_TYPE;
	*outputTypePtr++	= INT_TYPE | SIZE32_TYPE;
	*outputTypePtr++	= INT_TYPE | SIZE32_TYPE;
	*outputTypePtr++	= INT_TYPE | SIZE32_TYPE;
	*outputTypePtr++	= INT_TYPE | SIZE32_TYPE;
	*outputTypePtr++	= INT_TYPE | SIZE32_TYPE;
	*outputTypePtr++	= INT_TYPE | SIZE32_TYPE;		

	return (status);	 	
}

uint32	sysStaCpRunFunc	(uint32 taskId, uint32 fbBlockAddr)
{
	uint32 status = 0; 
	strFC0401Info fd;
	uint32 *mdmemPtr, *cpuRasPtr, dataLength = 0;

    /* 스펙과 내부변수 파라미터 읽기 */
    status = readRuntimeFbData(	taskId,
    							LOGIC_ID,
                                fbBlockAddr,
                                (uint32 *)&fd
						   );
	if(status) {
		setErrorCode(	__FILE__, __LINE__,__FUNCTION__, status);
		return(status);
    }

	/* 알고리즘 연산부 */
#if 0
	gunHvdcCpuRasInfo.cpuC.cpuType = fd.CPU_I; 
#endif
	if((fd.AD_I>=5000)&&(fd.AD_I<=9000))
	{
	    mdmemPtr = (uint32 *)&(MMEM_DWORD((fd.AD_I)));
    	switch(gunHvdcCpuRasInfo.cpuC.cpuType)
    	{
    	    case HVDC_CPU_C_TYPE:
                cpuRasPtr = &gunHvdcCpuRasInfo.cpuC.date;
                dataLength = sizeof(strCnPTypeCpuRasInfo)/4;
    	        break;  
    	    case HVDC_CPU_P_TYPE:
    	        cpuRasPtr = &gunHvdcCpuRasInfo.cpuP.date;
    	        dataLength = sizeof(strCnPTypeCpuRasInfo)/4;
    	        break;
    	    case HVDC_CPU_V_TYPE:
    	        cpuRasPtr = &gunHvdcCpuRasInfo.cpuV.date;
    	        dataLength = sizeof(strVTypeCpuRasInfo)/4;
    	        break;
    	    case HVDC_CPU_M_TYPE:
        	    cpuRasPtr = &gunHvdcCpuRasInfo.cpuM.date;
        	    dataLength = sizeof(strMTypeCpuRasInfo)/4;
    	        break;
    	    default:
    	        break;
    	}
    	if((dataLength>0)&&(dataLength<=sizeof(unnCpuRasInfo)))
    	{
    	    fourBytesDataCopy(mdmemPtr,cpuRasPtr,dataLength);
    	}
    }
    else 
    {
    }
    fd.DLEN_0 = dataLength;


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
