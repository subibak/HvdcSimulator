
#ifndef _modbustcp_h
#define _modbustcp_h

#include "pmsCommon.h"

#define	MBT_TCP_PORT	502		


typedef struct
{
	uint16	transactionId;
	uint16	protocolId;
	uint16	length;
	uint8		unitId;
	uint8		functionCode;
}	MBT_COMM_HEADER_ST;

#define MBT_MAX_PACKET_LENGTH	260	
									
#define	MBT_MAX_PDU_LENGTH		253	
#define	MBT_HEADER_LENGTH		8	
#define MBT_FC_INDEX			7	

#define MB_MAX_QUANTITY_STATUS				2000	
#define MB_MAX_QUANTITY_REGISTERS			125
#define MB_MAX_QUANTITY_MULTIPLE_COILS		0x7B0	
#define MB_MAX_QUANTITY_MULTIPLE_REGISTERS	0x7B	


typedef struct
{
	char			key[32];		
	int				logDispFlag;	
#if 0
	int				logFileFlag;	
	FILE			*fp;
	char			fname[256];		
	TYPES_DATETIME	lastOpenDtime;
#endif
}	MB_DIAGNOSTICS_ST;



#define	MB_FC_READ_COILS				0x01	
#define	MB_FC_READ_DISCRETE_INPUTS		0x02	
#define	MB_FC_READ_HOLDING_REGISTERS	0x03	
#define	MB_FC_READ_INPUT_REGISTERS		0x04	
#define	MB_FC_WRITE_SINGLE_COIL			0x05	
#define	MB_FC_WRITE_SINGLE_REGISTER		0x06	
#define	MB_FC_WRITE_MULTIPLE_COILS		0x0F	
#define	MB_FC_WRITE_MULTIPLE_REGISTERS	0x10	


#define	MB_EC_ILLEGAL_FUNCTION			0x01
#define	MB_EC_ILLEGAL_DATA_ADDRESS		0x02
#define	MB_EC_ILLEGAL_DATA_VALUE		0x03
#define	MB_EC_SLAVE_DEVICE_FAILURE		0x04	
#define	MB_EC_ACKNOWLEDGE				0x05	
#define	MB_EC_SLAVE_DEVICE_BUSY			0x06	





typedef enum 
{
	MBT_RX_STATE_IDLE,
	MBT_RX_STATE_WAIT_HEAD,
	MBT_RX_STATE_READ_HEAD,
	MBT_RX_STATE_READ_FC
} MBT_RX_STATE;

typedef struct
{

	
	uint16			txLength;
	uint8				txBuffer[MBT_MAX_PACKET_LENGTH];
	
	TYPES_DATETIME		timeSent;

	
	MBT_RX_STATE		rxState;

	uint16			currentLength;
	uint16			expectedLength;		

	
	TYPES_DATETIME		timeReceived;
	
	TYPES_MILLISECONDS	firstByteTime;
	
	TYPES_MILLISECONDS	lastByteTime;

	
	MBT_COMM_HEADER_ST	rxHeader;

	
	uint16			rxLength;
	uint8				rxBuffer[MBT_MAX_PACKET_LENGTH];
} MBT_MSG_DATA_ST;


#define MBT_MAX_ERROR_LENGTH	200

typedef struct
{
	uint8				msgBuffer[MBT_MAX_ERROR_LENGTH];
	uint16			msgLength;
	TYPES_DATETIME		timeReceived;
	TYPES_MILLISECONDS	firstByteTime;
} MBT_ERR_DATA_ST;


typedef struct
{
	int			openCount;
	int			closedCount;
	int			byteSentCount;
	int			byteReceivedCount;
	int			byteReceivedErrorCount;
	int			frameSentCount;
	int			frameReceivedCount;
} MBT_COMM_STAT_ST;



#endif 



