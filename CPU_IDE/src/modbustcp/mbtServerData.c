

#include	"usrTypes.h"
#include	"sysDefs.h"
#include	"sysConfig.h"
#include 	"modbustcp\mbtServerProto.h"
#include	"vmeDef.h"
#include	"pmsUtil.h"

extern uint32 mMemReadPoint(uint32 mdbsAddr, uint32 *valuePtr);
extern uint32 mMemWordReadPoint(uint32 mdbsAddr, uint16 *valuePtr);
extern uint32 mMemWritePoint(uint32 mdbsAddr, uint32 *valuePtr);
extern uint32 mMemWordWritePoint(uint32 mdbsAddr, uint16 *valuePtr);

int mbtsData__ReadCoils (INF_CHANNEL_MBTS_ST *pCh,
				uint16 startAddress, uint16 quantity, uint8 *status)
{
	int i;
	uint32	exceptionCode;
	uint32	readVal;
	uint32	mapAddress;

	mapAddress = startAddress%10000;
	mapAddress += MIN_MB_BO_ADDR;

	for (i=0; i<quantity; i++)
	{
		exceptionCode = mMemReadPoint((uint32)(mapAddress + i),&readVal);
		if(exceptionCode == NO_ERROR )
		{
			if(readVal)
				status[i/8] |= (uint8)(0x01<<(i%8));
			else
				status[i/8] &= (~((uint8)(0x01<<(i%8))));
		}
		else {
			setErrorCode(	(uint8 *)__FILE__,
	    					(uint32)__LINE__,
	        	            (uint8 *)"mbtsData__ReadCoils",
		        	         exceptionCode
							);
			return (MB_EC_ILLEGAL_DATA_ADDRESS);
		}
	}

	return 0;
}


int mbtsData__ReadDiscreteInputs (INF_CHANNEL_MBTS_ST *pCh,
				uint16 startAddress, uint16 quantity, uint8 *status)
{
	int i;
	uint32	exceptionCode;
	uint32	readVal;
	uint32	mapAddress;

	mapAddress = startAddress%10000;
	mapAddress += MIN_MB_BI_ADDR;

	for (i=0; i<quantity; i++)
	{
		exceptionCode = mMemReadPoint((uint32)(mapAddress + i),&readVal);
		if(exceptionCode == NO_ERROR )
		{
			if(readVal)
				status[i/8] |= (uint8)(0x01<<(i%8));
			else
				status[i/8] &= (~((uint8)(0x01<<(i%8))));
		}
		else {
			setErrorCode(	(uint8 *)__FILE__,
	    					(uint32)__LINE__,
	        	            (uint8 *)"mbtsData__ReadDiscreteInputs",
		        	         exceptionCode
							);
			return (MB_EC_ILLEGAL_DATA_ADDRESS);
		}
	}

	return 0;
}


int mbtsData__ReadHoldingRegisters (INF_CHANNEL_MBTS_ST *pCh,
				uint16 startAddress, uint16 quantity, uint16 *registers)
{
	int i;
	uint32	exceptionCode, readVal;
	uint16  readVal16;
	uint32	mapAddress;
	strSysConfigInfo	sysCnfgInfo;

	mapAddress = startAddress%10000;
	mapAddress += MIN_MB_AO_ADDR;

	systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);
	if ((sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_DWORD_FIX) || (sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_DWORD_WSWAP))
	{
	
	if((startAddress%2)||(quantity%2))
	{
		setErrorCode(__FILE__, __LINE__, __FUNCTION__,MB_EC_ILLEGAL_DATA_ADDRESS); 
		return (MB_EC_ILLEGAL_DATA_ADDRESS);
	}
	} 

	if ((sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_DWORD_FIX) || (sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_DWORD_WSWAP))
	{
	for (i=0; i<quantity; i+=2)
	{
		exceptionCode = mMemReadPoint((uint32)(mapAddress + i),&readVal);
		if(exceptionCode == NO_ERROR )
		{
			registers[i] = (readVal)&0xFFFF;
			registers[i+1] = (readVal>>16)&0xFFFF;
		}
		else {
			setErrorCode(	(uint8 *)__FILE__,
	    					(uint32)__LINE__,
	        	            (uint8 *)"mbtsData__ReadHoldingRegisters",
		        	         exceptionCode
							);
			return (MB_EC_ILLEGAL_DATA_ADDRESS);
		}
	}
	} 
	else if (sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_WORD)
	{
	for (i=0; i<quantity; i++)
	{
		exceptionCode = mMemWordReadPoint((uint32)(mapAddress + i),&readVal16);
		if(exceptionCode == NO_ERROR )
		{
			registers[i] = (readVal16)&0xFFFF;
		}
		else {
			setErrorCode(	(uint8 *)__FILE__,
	    					(uint32)__LINE__,
	        	            (uint8 *)"mbtsData__ReadHoldingRegisters",
		        	         exceptionCode
							);
			return (MB_EC_ILLEGAL_DATA_ADDRESS);
		}
	}
	} 
	return 0;
}


int mbtsData__ReadInputRegisters (INF_CHANNEL_MBTS_ST *pCh,
				uint16 startAddress, uint16 quantity, uint16 *registers)
{
	int i;
	uint32	exceptionCode, readVal;
	uint16	readVal16;
	uint32	mapAddress;
	strSysConfigInfo	sysCnfgInfo;

	mapAddress = startAddress%10000;
	mapAddress += MIN_MB_AI_ADDR;
	
	systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);
	if ((sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_DWORD_FIX) || (sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_DWORD_WSWAP))
	{
	
	if((startAddress%2)||(quantity%2))
	{
		setErrorCode(__FILE__, __LINE__, __FUNCTION__,MB_EC_ILLEGAL_DATA_ADDRESS); 
		return (MB_EC_ILLEGAL_DATA_ADDRESS);
	}
	} 
	
	if ((sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_DWORD_FIX) || (sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_DWORD_WSWAP))
	{
	for (i=0; i<quantity; i+=2)
	{
		exceptionCode = mMemReadPoint((uint32)(mapAddress + i),&readVal);
		if(exceptionCode == NO_ERROR )
		{
			registers[i] = (readVal)&0xFFFF;
			registers[i+1] = (readVal>>16)&0xFFFF;
		}
		else {
			setErrorCode(	(uint8 *)__FILE__,
	    					(uint32)__LINE__,
	        	            (uint8 *)"mbtsData__ReadInputRegisters",
		        	         exceptionCode
							);
			return (MB_EC_ILLEGAL_DATA_ADDRESS);
		}
	}
	} 
	else if (sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_WORD)
	{
	for (i=0; i<quantity; i++)
	{
		exceptionCode = mMemWordReadPoint((uint32)(mapAddress + i),&readVal16);
		if(exceptionCode == NO_ERROR )
		{
			registers[i] = (readVal16)&0xFFFF;
		}
		else {
			setErrorCode(	(uint8 *)__FILE__,
	    					(uint32)__LINE__,
	        	            (uint8 *)"mbtsData__ReadInputRegisters",
		        	         exceptionCode
							);
			return (MB_EC_ILLEGAL_DATA_ADDRESS);
		}
	}
	} 
	return 0;
}


int mbtsData__WriteSingleCoil (INF_CHANNEL_MBTS_ST *pCh,
				uint16 address, uint16 value)
{
	uint32	exceptionCode, writeVal;
	uint32	mapAddress;

	mapAddress = address%10000;
	mapAddress += MIN_MB_BO_ADDR;

	switch(value)
	{
		case 0xFF00:
			writeVal = 1;
		break;
		case 0x0000:
			writeVal = 0;
		break;
		default:
			return (-1);
		break;
	}
	exceptionCode = mMemWritePoint((uint32)mapAddress,&writeVal);
	if(exceptionCode != NO_ERROR )
	{
		setErrorCode(	(uint8 *)__FILE__,
    					(uint32)__LINE__,
        	            (uint8 *)"mbtsData__WriteSingleCoil",
	        	         exceptionCode
						);
		return (MB_EC_ILLEGAL_DATA_ADDRESS);
	}		

	return 0;
}


int mbtsData__WriteSingleRegister (INF_CHANNEL_MBTS_ST *pCh,
				uint16 address, uint16 value)
{
	uint32	exceptionCode, writeVal;
	uint16	wordWriteVal;
	uint32	mapAddress;
	strSysConfigInfo	sysCnfgInfo;

	mapAddress = address%10000;
	mapAddress += MIN_MB_AO_ADDR;

	systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);
	if ((sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_DWORD_FIX) || (sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_DWORD_WSWAP))
	{
	
	if(address%2)
	{
		setErrorCode(__FILE__, __LINE__, __FUNCTION__,MB_EC_ILLEGAL_DATA_ADDRESS); 
		return (MB_EC_ILLEGAL_DATA_ADDRESS);
	}
	} 

	systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);
	if ((sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_DWORD_FIX) || (sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_DWORD_WSWAP))
	{
		writeVal = value;
		exceptionCode = mMemWritePoint((uint32)mapAddress,&writeVal);
	} 
	else if (sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_WORD)
	{
		wordWriteVal = value;
		exceptionCode = mMemWordWritePoint((uint32)mapAddress,&wordWriteVal);
	} 

	if(exceptionCode != NO_ERROR )
	{
		setErrorCode(	(uint8 *)__FILE__,
    					(uint32)__LINE__,
        	            (uint8 *)"mbtsData__WriteSingleRegister",
	        	         exceptionCode
						);
		return (MB_EC_ILLEGAL_DATA_ADDRESS);
	}

	return 0;
}


int mbtsData__WriteMultipleCoils (INF_CHANNEL_MBTS_ST *pCh,
			uint16 startAddress, uint16 quantity,
			uint8 byteCount, uint8 *status)
{
	int i, nbyte;
	uint32	exceptionCode, writeVal;
	uint32	mapAddress;

	mapAddress = startAddress%10000;
	mapAddress += MIN_MB_BO_ADDR;

	nbyte = quantity / 8;
	if (quantity % 8)
		nbyte++;
		
	if((nbyte!=byteCount))
	{
		setErrorCode(__FILE__, __LINE__, __FUNCTION__,MB_EC_ILLEGAL_DATA_VALUE); 
		return (MB_EC_ILLEGAL_DATA_VALUE);
	}
	
	for (i=0; i<quantity; i++)
	{
		writeVal = (status[i/8]>>(i%8))&0x1;
		exceptionCode = mMemWritePoint((uint32)(mapAddress + i),&writeVal);
		if(exceptionCode != NO_ERROR )
		{
			setErrorCode(	(uint8 *)__FILE__,
	    					(uint32)__LINE__,
	        	            (uint8 *)"mbtsData__WriteMultipleCoils",
		        	         exceptionCode
							);
			return (MB_EC_ILLEGAL_DATA_ADDRESS);
		}
	}

	return 0;
}


int mbtsData__WriteMultipleRegisters (INF_CHANNEL_MBTS_ST *pCh,
			uint16 startAddress, uint16 quantity,
			uint8 byteCount, uint16 *registers)
{
	int i = 0;
	uint32	exceptionCode = 0, writeVal = 0;
	uint32	mapAddress = 0;
	uint16  wordWriteVal = 0;
	strSysConfigInfo	sysCnfgInfo;

	mapAddress = startAddress%10000;
	mapAddress += MIN_MB_AO_ADDR;


	systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);
	if ((sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_DWORD_FIX) || (sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_DWORD_WSWAP))
	{
	
	if((startAddress%2)||(quantity%2)||((quantity*2)!=byteCount))
	{
		setErrorCode(__FILE__, __LINE__, __FUNCTION__,MB_EC_ILLEGAL_DATA_ADDRESS); 
		return (MB_EC_ILLEGAL_DATA_ADDRESS);
	}
	} 
	else if (sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_WORD)
	{
	if((quantity*2)!=byteCount)
	{
		setErrorCode(__FILE__, __LINE__, __FUNCTION__,MB_EC_ILLEGAL_DATA_ADDRESS); 
		return (MB_EC_ILLEGAL_DATA_ADDRESS);
	}
	} 

	if ((sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_DWORD_FIX) || (sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_DWORD_WSWAP))
	{
	for (i=0; i<quantity; i+=2)
	{
		writeVal = registers[i];
		writeVal |= (registers[i+1]<<16);

		exceptionCode = mMemWritePoint((uint32)(mapAddress + i),&writeVal);
		if(exceptionCode != NO_ERROR )
		{
			setErrorCode(	(uint8 *)__FILE__,
	    					(uint32)__LINE__,
	        	            (uint8 *)"mbtsData__WriteMultipleRegisters",
		        	         exceptionCode
							);
			return (MB_EC_ILLEGAL_DATA_ADDRESS);
		}
	}
	} 
	else if (sysCnfgInfo.mbSlaveMapIf == MB_SLV_IF_WORD)
	{
	for (i=0; i<quantity; i+=1)
	{
		wordWriteVal = registers[i];
		
		exceptionCode = mMemWordWritePoint((uint32)(mapAddress + i),&wordWriteVal);
		if(exceptionCode != NO_ERROR )
		{
			setErrorCode(	(uint8 *)__FILE__,
    						(uint32)__LINE__,
        					(uint8 *)"mbtsData__WriteMultipleRegisters",
	        				 exceptionCode
							);
			return (MB_EC_ILLEGAL_DATA_ADDRESS);
		}
	}
	} 
	return 0;
}

