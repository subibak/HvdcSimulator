
#ifndef _mbtServerProto_h
#define _mbtServerProto_h

#include "modbustcp\mbtServer.h"

#ifdef __cplusplus
extern "C" {
#endif


TMWTYPES_INT MBTS_LoadConfig (void);
int MBTS_CommTask (int index);


int mbtsData__ReadCoils (INF_CHANNEL_MBTS_ST *pCh,
				uint16 startAddress, uint16 quantity, uint8 *status);
int mbtsData__ReadDiscreteInputs (INF_CHANNEL_MBTS_ST *pCh,
				uint16 startAddress, uint16 quantity, uint8 *status);
int mbtsData__ReadHoldingRegisters (INF_CHANNEL_MBTS_ST *pCh,
				uint16 startAddress, uint16 quantity, uint16 *registers);
int mbtsData__ReadInputRegisters (INF_CHANNEL_MBTS_ST *pCh,
				uint16 startAddress, uint16 quantity, uint16 *registers);
int mbtsData__WriteSingleCoil (INF_CHANNEL_MBTS_ST *pCh,
				uint16 startAddress, uint16 vaue);
int mbtsData__WriteSingleRegister (INF_CHANNEL_MBTS_ST *pCh,
				uint16 address, uint16 vaue);
int mbtsData__WriteMultipleCoils (INF_CHANNEL_MBTS_ST *pCh,
			uint16 startAddress, uint16 quantity,
			uint8 byteCount, uint8 *status);
int mbtsData__WriteMultipleRegisters (INF_CHANNEL_MBTS_ST *pCh,
			uint16 startAddress, uint16 quantity,
			uint8 byteCount, uint16 *registers);


void mtpDecode_Header (uint8 *pBuff, MBT_COMM_HEADER_ST *pHead);
int mbtEncode_Error (MB_DIAGNOSTICS_ST *pDiag, uint8 *pBuff,
			MBT_COMM_HEADER_ST *pHead, uint8 exceptionCode);
int mbtEncode_ReadStatusResponse (MB_DIAGNOSTICS_ST *pDiag, uint8 *pBuff,
			MBT_COMM_HEADER_ST *pHead, uint16 quantity, uint8 *status);
int mbtEncode_ReadRegisterResponse (MB_DIAGNOSTICS_ST *pDiag, uint8 *pBuff,
			MBT_COMM_HEADER_ST *pHead, uint16 quantity, uint16 *word );
int mbtEncode_WriteSingleDataResponse (MB_DIAGNOSTICS_ST *pDiag, uint8 *pBuff,
			MBT_COMM_HEADER_ST *pHead, uint16 address, uint16 value);
int mbtEncode_WriteMultipleDataResponse(MB_DIAGNOSTICS_ST *pDiag, uint8 *pBuff,
			MBT_COMM_HEADER_ST *pHead, uint16 startAddress, uint16 quantity);

int mbtDecode_ReadData (MB_DIAGNOSTICS_ST *pDiag, uint8 *pBuff, int length,
			uint16 *startAddress, uint16 *quantity);
int mbtDecode_WriteSingleData (MB_DIAGNOSTICS_ST *pDiag, uint8 *pBuff, int length,
			uint16 *startAddress, uint16 *value);
int mbtDecode_WriteMultipleCoils (MB_DIAGNOSTICS_ST *pDiag, uint8 *pBuff, int length,
			uint16 *startAddress, uint16 *quantity, uint8 *byteCount, uint8 *status);
int mbtDecode_WriteMultipleRegisters (MB_DIAGNOSTICS_ST *pDiag, uint8 *pBuff, int length,
			uint16 *startAddress, uint16 *quantity, uint8 *byteCount, uint16 *registers);


void mbtDiag_Open (MB_DIAGNOSTICS_ST *pDiag, char *key);
void mbtDiag_Close (MB_DIAGNOSTICS_ST *pDiag);
void mbtDiag_SetDiaplayFlag (MB_DIAGNOSTICS_ST *pDiag, int flag);

int mbtDiag_CanPrint (MB_DIAGNOSTICS_ST *pDiag);
void mbtDiag_Packet (MB_DIAGNOSTICS_ST *pDiag, TYPES_DATETIME *timeReceived, uint8 *msg, int length, int tx);
void mbtDiag_Error (MB_DIAGNOSTICS_ST *pDiag, TYPES_DATETIME *timeReceived, uint8 *msg, int length);
void mbtDiag_Application (MB_DIAGNOSTICS_ST *pDiag, TYPES_DATETIME *timeReceived, char *msg1, char *msg2);
void mbtDiag_Data (MB_DIAGNOSTICS_ST *pDiag, char *msg);

#ifdef __cplusplus
}
#endif

#endif 



