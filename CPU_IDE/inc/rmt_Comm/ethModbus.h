
#ifndef ETHMODBUS_H
#define ETHMODBUS_H



#define MODBUS_TCP_PROTOCOL_HEAD_LEN 8	


#define MODBUS_TCP_LENGTH_OFFSET_SIZE 6		





#define FC_1_READ_COIL_STATUS			0x01	
#define FC_2_READ_INPUT_STATUS	        0x02	
#define FC_3_READ_HOLDING_REGISTERS  	0x03	
#define FC_4_READ_INPUT_REGISTERS    	0x04	

#define FC_15_FORCE_MULTIPLE_COILS 		0x0f	
#define FC_16_WRITE_MULTIPLE_REGISTERS 	0x10	


#define FC_5_FORCE_SINGLE_COIL       	0x05	
#define FC_6_PRESET_SINGLE_REGISTERS 	0x06	
#define READ_EXECPTION_STATUS   		0x07	
#define DIAGNOSTICS						0x08
#define FORCE_MULTIPLE_COIL     		0x10	
#define PRESET_MULTIPLE_REGISTERS		0x11
#define READ_GENERAL_REFERANCE			0x14
#define WRITE_GENERAL_REFERANCE			0x15


#define EXCEPTION_OF_FC_1				0x81	
#define EXCEPTION_OF_FC_2				0x82
#define EXCEPTION_OF_FC_3				0x83
#define EXCEPTION_OF_FC_4				0x84
#define EXCEPTION_OF_FC_5				0x85
#define EXCEPTION_OF_FC_6				0x86
#define EXCEPTION_OF_FC_16				0x90


#define	ILLEGAL_FUNCTION_EXCEPTION		(0x0001)
#define	ILLEGAL_DATA_ADDRESS_EXCEPTION	(0x0002)
#define	ILLEGAL_DATA_VALUE_EXCEPTION	(0x0003)
#define	SLAVE_DEVICE_FAIL_EXCEPTION		(0x0004)
#define	ACKNOWLEDGE_EXCEPTION			(0x0005)
#define	SLAVE_DEVICE_BUSY_EXCEPTION		(0x0006)


#define MAX_READ_COIL_STATUS_SIZE			2000
#define MAX_READ_INPUT_STATUS_SIZE			2000
#define MAX_READ_HOLDING_REGISTERS_SIZE		125
#define MAX_READ_INPUT_REGISTERS_SIZE		125
#define MAX_FORCE_SINGLE_COIL_SIZE			1
#define MAX_PRESET_SINGLE_REGISTERS_SIZE	1



#define MODBUS_0_BIT_OUTPUT_REGION			0x01
#define MODBUS_1_BIT_INPUT_REGION			0x02
#define MDOBUS_3_WORD_INPUT_REGION			0x03
#define MODBUS_4_WORD_OUTPUT_REGION			0x04


#define	MODBUS_TCP_MAX_CNET			4
#define	MODBUS_TCP_MAX_SLAVE_NUM	64
#define MODBUS_DATA_VALUE_LENGTH	1024
typedef struct ModbusTCPDeviceInfoData 
{
	uint32 slaveDeviceId;
	uint32 comShelf;
	uint32 ipAddrInfoList[MODBUS_TCP_MAX_CNET];
	int32  tcpClntSock[MODBUS_TCP_MAX_CNET];
	uint32 lastConnectTime[MODBUS_TCP_MAX_CNET];
	uint32 lastStatCheckTime[MODBUS_TCP_MAX_CNET];
	uint32 dataRecvLiveCount[MODBUS_TCP_MAX_CNET];
	uint32 preRecvLiveCount[MODBUS_TCP_MAX_CNET];
	BOOL   isConnected[MODBUS_TCP_MAX_CNET];
	uint32 lifeCount;

}strModbusTcpDevInfo;

typedef struct 
{
	uint32	numOfSlave;
    uint32  destStation[2];     
	strModbusTcpDevInfo	slaveDeviceListInfo[MODBUS_TCP_MAX_SLAVE_NUM];
}strMBClntAndSlvDevInfo;

typedef struct {
	uint8 transactionIdHi;	
	uint8 transactionIdLo;	
	uint8 protocolIDHi;	    
	uint8 protocolIDLo;	    
	uint8 lengthHi;			
	uint8 lengthLo;			
	uint8 unitID;			
	uint8 mbusFcode;		
}strModbusTCPHeadInfo;


typedef struct {
	strModbusTCPHeadInfo headInfo;
	unsigned char startAddrHi;		
	unsigned char startAddrLo;		
	unsigned char numOfRegHi;		
	unsigned char numOfRegLo;		
}strModbusTcpReadReqInfo;

typedef struct {
	strModbusTCPHeadInfo headInfo;
	unsigned char byteCount;		
	unsigned char data[MODBUS_DATA_VALUE_LENGTH];
}strModbusTcpReadRespInfo;


typedef struct {
	strModbusTCPHeadInfo headInfo;
	unsigned char startAddrHi;		
	unsigned char startAddrLo;		
	unsigned char writeData[2];
}strMBTcpWriteSingleCoilReqInfo;

typedef struct {
	strModbusTCPHeadInfo headInfo;
	unsigned char startAddrHi;		
	unsigned char startAddrLo;		
	unsigned char bitCntHi;		
	unsigned char bitCntLo;		
	unsigned char byteCnt;			
	unsigned char writeData[MODBUS_DATA_VALUE_LENGTH];
}strMBTcpForceMultipleCoilsReqInfo;

typedef struct {
	strModbusTCPHeadInfo headInfo;
	unsigned char startAddrHi;		
	unsigned char startAddrLo;		
	unsigned char writeData[2];
}strMBTcpWriteSingleRegisterReqInfo;

typedef struct {
	strModbusTCPHeadInfo headInfo;
	unsigned char startAddrHi;		
	unsigned char startAddrLo;		
	unsigned char wordCntHi;		
	unsigned char wordCntLo;		
	unsigned char byteCnt;			
	unsigned char writeData[MODBUS_DATA_VALUE_LENGTH];
}strMBTcpWriteMultipleRegisterReqInfo;

#if 0
typedef struct {
    uint16  command;
    uint16  dataType;
    uint16  memoryType;
    uint16  dataNum;
    uint16  memoryAddr[MAX_VAR_NUM];
    uint16  byteNum[MAX_VAR_NUM];
    uint32 data[256]; 
}strDataAccReqInfo;
#endif

#ifndef MAX_VAR_NUM
#define MAX_VAR_NUM             16
#endif

typedef struct {
	uint16	dstDevId;
	
    uint16  command;
    uint16  dataType;
    uint16  memoryType;
    uint16  dataNum;
    uint16  memoryAddr[MAX_VAR_NUM];
    uint16  byteNum[MAX_VAR_NUM];
    uint32 data[256]; 
}strMBDataAccReqInfo;
#endif 






