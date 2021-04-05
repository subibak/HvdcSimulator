

#include "modbustcp\mbtServerProto.h"

static char *sprt_mb_func_code (char *out, int outSize, int functionCode)
{
	if (functionCode == MB_FC_READ_COILS)
		strcpy_s (out, outSize, "Read Coils");
	else if (functionCode == MB_FC_READ_DISCRETE_INPUTS)
		strcpy_s (out, outSize, "Read Discrete Inputs");
	else if (functionCode == MB_FC_READ_HOLDING_REGISTERS)
		strcpy_s (out, outSize, "Read Holding Registers");
	else if (functionCode == MB_FC_READ_INPUT_REGISTERS)
		strcpy_s (out, outSize, "Read Input Registers");
	else if (functionCode == MB_FC_WRITE_SINGLE_COIL)
		strcpy_s (out, outSize, "Write Single Coil");
	else if (functionCode == MB_FC_WRITE_SINGLE_REGISTER)
		strcpy_s (out, outSize, "Write Single Register");
	else if (functionCode == MB_FC_WRITE_MULTIPLE_COILS)
		strcpy_s (out, outSize, "Write Multiple Coils");
	else if (functionCode == MB_FC_WRITE_MULTIPLE_REGISTERS)
		strcpy_s (out, outSize, "Write Multiple Registers");
	else
		u_snprintf (out, outSize, "unknown(%d)", functionCode);
	return out;
}

static char *sprt_mb_exception_code (char *out, int outSize, int exceptionCode)
{
	static char *_exception_code[5] = {
					"OK",
					"ILLEGAL FUNCTION",
					"ILLEGAL DATA ADDRESS",
					"ILLEGAL DATA VALUE",
					"SLAVE DEVICE FAILURE" };

	if (exceptionCode >= 0 && exceptionCode < ARRAY_SIZE(_exception_code))
		strcpy_s (out, outSize, _exception_code[exceptionCode]);
	else
		strcpy_s (out, outSize, "unknown");
	return out;
}



void mtpDecode_Header (uint8 *pBuff, MBT_COMM_HEADER_ST *pHead)
{
	pHead->transactionId = pBuff[0] << 8 | pBuff[1];
	pHead->protocolId    = pBuff[2] << 8 | pBuff[3];
	pHead->length        = pBuff[4] << 8 | pBuff[5];
	pHead->unitId        = pBuff[6];
	pHead->functionCode  = pBuff[7];
}


static void mbtEncode_Header (uint8 *pBuff, MBT_COMM_HEADER_ST *pHead)
{
	pBuff[0] = pHead->transactionId >> 8;
	pBuff[1] = pHead->transactionId & 0xFF;
	pBuff[2] = 0;	
	pBuff[3] = 0;	
	pBuff[4] = pHead->length >> 8;
	pBuff[5] = pHead->length & 0xFF;
	pBuff[6] = pHead->unitId;
	pBuff[7] = pHead->functionCode;
}


int mbtEncode_Error (MB_DIAGNOSTICS_ST *pDiag, uint8 *pBuff,
			MBT_COMM_HEADER_ST *pHead, uint8 exceptionCode)
{
	int			idx;
	MBT_COMM_HEADER_ST	head;

	memcpy (&head, pHead, sizeof(head));
	head.length = 3;
	head.functionCode |= 0x80;
	
	mbtEncode_Header (pBuff, &head);
	
	idx = MBT_HEADER_LENGTH;
	pBuff[idx] = exceptionCode;
	idx += 1;

	if (mbtDiag_CanPrint (pDiag))
	{
		int 	n;
		char	ec[32], buf[256];
#ifdef _WIN32
		n = u_snprintf(buf, sizeof(buf),
#else
		n = snprintf (buf, sizeof(buf),
#endif
			"\n<~~~ [Error response] exceptionCode(0x%.2X:%s)\n",
			exceptionCode,
			sprt_mb_exception_code (ec, sizeof(ec), exceptionCode));

		mbtDiag_Data (pDiag, buf);
	}
	return idx;
}


int mbtEncode_ReadStatusResponse (MB_DIAGNOSTICS_ST *pDiag, uint8 *pBuff,
			MBT_COMM_HEADER_ST *pHead, uint16 quantity, uint8 *status)
{
	int	idx, nbytes;
	MBT_COMM_HEADER_ST	head;

	nbytes = quantity / 8;
	if (quantity % 8)
		nbytes++;

	memcpy (&head, pHead, sizeof(head));
	head.length = 3 + nbytes;
	
	mbtEncode_Header (pBuff, &head);
	
	idx = MBT_HEADER_LENGTH;
	
	pBuff[idx] = nbytes;
	idx += 1;
	
	memcpy (&pBuff[idx], status, nbytes);
	idx += nbytes;
	if (mbtDiag_CanPrint (pDiag))
	{
		int 	n;
		char	fc[32], buf[256];
#ifdef _WIN32
		n = u_snprintf(buf, sizeof(buf), 
#else
		n = snprintf (buf, sizeof(buf),
#endif
			"\n<~~~ [Response] '%s' ByteCount(%d)\n",
			sprt_mb_func_code (fc, sizeof(fc), pHead->functionCode),
			nbytes);

		mbtDiag_Data (pDiag, buf);
	}
	return idx;
}


int mbtEncode_ReadRegisterResponse (MB_DIAGNOSTICS_ST *pDiag, uint8 *pBuff,
			MBT_COMM_HEADER_ST *pHead, uint16 quantity, uint16 *word)
{
	int	i, idx;
	MBT_COMM_HEADER_ST	head;

	memcpy (&head, pHead, sizeof(head));
	head.length = 3 + quantity * 2;
	
	mbtEncode_Header (pBuff, &head);
	
	idx = MBT_HEADER_LENGTH;
	
	pBuff[idx] = quantity * 2;
	idx += 1;
	
	for (i=0; i < quantity; i++)
	{
		pBuff[idx] = *word >> 8;
		pBuff[idx+1] = *word & 0xFF;
		idx += 2;
		word++;
	}
	if (mbtDiag_CanPrint (pDiag))
	{
		int 	n;
		char	fc[32], buf[256];
#ifdef _WIN32
		n = u_snprintf(buf, sizeof(buf),
#else
		n = snprintf (buf, sizeof(buf),
#endif
			"\n<~~~ [Response] '%s' ByteCount(%d)\n",
			sprt_mb_func_code (fc, sizeof(fc), pHead->functionCode),
			quantity * 2);

		mbtDiag_Data (pDiag, buf);
	}
	return idx;
}


int mbtEncode_WriteSingleDataResponse (MB_DIAGNOSTICS_ST *pDiag, uint8 *pBuff,
			MBT_COMM_HEADER_ST *pHead, uint16 address, uint16 value)
{
	int	idx;
	MBT_COMM_HEADER_ST	head;

	memcpy (&head, pHead, sizeof(head));
	head.length = 6;
	
	mbtEncode_Header (pBuff, &head);
	
	idx = MBT_HEADER_LENGTH;
	
	pBuff[idx] = address >> 8;
	pBuff[idx+1] = address & 0xFF;
	idx += 2;
	
	pBuff[idx] = value >> 8;
	pBuff[idx+1] = value & 0xFF;
	idx += 2;
	if (mbtDiag_CanPrint (pDiag))
	{
		int 	n;
		char	fc[32], buf[256];
#ifdef _WIN32
		n = u_snprintf(buf, sizeof(buf),
#else
		n = snprintf (buf, sizeof(buf),
#endif
			"\n<~~~ [Response] '%s' address(0x%.4X), value(0x%.4X)\n",
			sprt_mb_func_code (fc, sizeof(fc), pHead->functionCode),
			address, value);

		mbtDiag_Data (pDiag, buf);
	}
	return idx;
}


int mbtEncode_WriteMultipleDataResponse(MB_DIAGNOSTICS_ST *pDiag, uint8 *pBuff,
			MBT_COMM_HEADER_ST *pHead, uint16 startAddress, uint16 quantity)
{
	int	idx;
	MBT_COMM_HEADER_ST	head;

	memcpy (&head, pHead, sizeof(head));
	head.length = 6;
	
	mbtEncode_Header (pBuff, &head);
	
	idx = MBT_HEADER_LENGTH;
	
	pBuff[idx]   = startAddress >> 8;
	pBuff[idx+1] = startAddress & 0xFF;
	idx += 2;
	
	pBuff[idx]   = quantity >> 8;
	pBuff[idx+1] = quantity & 0xFF;
	idx += 2;
	if (mbtDiag_CanPrint (pDiag))
	{
		int 	n;
		char	fc[32], buf[256];
#ifdef _WIN32
		n = u_snprintf(buf, sizeof(buf),
#else
		n = snprintf (buf, sizeof(buf),
#endif
			"\n<~~~ [Response] '%s' startAddress(0x%.4X), quantity(%d)\n",
			sprt_mb_func_code (fc, sizeof(fc), pHead->functionCode),
			startAddress, quantity );

		mbtDiag_Data (pDiag, buf);
	}
	return idx;
}


int mbtDecode_ReadData (MB_DIAGNOSTICS_ST *pDiag, uint8 *pBuff, int length,
			uint16 *startAddress, uint16 *quantity)
{
	int	idx;

	
	idx = MBT_HEADER_LENGTH;
	*startAddress = pBuff[idx] << 8 | pBuff[idx+1];
	idx += 2;
	*quantity = pBuff[idx] << 8 | pBuff[idx+1];

	
	if (mbtDiag_CanPrint(pDiag))
	{
		int n;
		char	fc[32], buf[256];
#ifdef _WIN32
		n = u_snprintf (buf, sizeof(buf),
#else
		n = snprintf (buf, sizeof(buf),
#endif
			"\n~~~> [Indication] '%s' startAddress(0x%.4X), quantity(%d)\n",
			sprt_mb_func_code (fc, sizeof(fc), pBuff[MBT_FC_INDEX]),
			*startAddress, *quantity);

		mbtDiag_Data (pDiag, buf);
	}
	return 0;	
}


int mbtDecode_WriteSingleData (MB_DIAGNOSTICS_ST *pDiag, uint8 *pBuff, int length,
			uint16 *address, uint16 *value)
{
	int	idx;

	
	idx = MBT_HEADER_LENGTH;
	*address = pBuff[idx] << 8 | pBuff[idx+1];
	idx += 2;
	*value = pBuff[idx] << 8 | pBuff[idx+1];

	
	if (mbtDiag_CanPrint(pDiag))
	{
		int n;
		char	fc[32], buf[256];
#ifdef _WIN32
		n = u_snprintf (buf, sizeof(buf),
#else
		n = snprintf (buf, sizeof(buf),
#endif
			"\n~~~> [Indication] '%s' address(0x%.4X), value(0x%.4X)\n",
			sprt_mb_func_code (fc, sizeof(fc), pBuff[MBT_FC_INDEX]),
			*address, *value);

		mbtDiag_Data (pDiag, buf);
	}
	return 0;	
}


int mbtDecode_WriteMultipleCoils (MB_DIAGNOSTICS_ST *pDiag, uint8 *pBuff, int length,
			uint16 *startAddress, uint16 *quantity, uint8 *byteCount, uint8 *status)
{
	int	idx, ret = 0;

	
	idx = MBT_HEADER_LENGTH;
	*startAddress = pBuff[idx] << 8 | pBuff[idx+1];
	idx += 2;
	*quantity = pBuff[idx] << 8 | pBuff[idx+1];
	idx += 2;
 	*byteCount = pBuff[idx];
	idx += 1;
	if (*byteCount <= (MB_MAX_QUANTITY_MULTIPLE_COILS/8))
	{
		memcpy (status, &pBuff[idx], *byteCount);
		idx += *byteCount;
	}
	else
		ret = 1;
	
	if (mbtDiag_CanPrint(pDiag))
	{
		int n;
		char	fc[32], buf[256];
#ifdef _WIN32
		n = u_snprintf (buf, sizeof(buf),
#else
		n = snprintf (buf, sizeof(buf),
#endif
			"\n~~~> [Indication] '%s' startAddress(0x%.4X), quantity(%d) byteCount(%d)\n",
			sprt_mb_func_code (fc, sizeof(fc), pBuff[MBT_FC_INDEX]),
			*startAddress, *quantity, *byteCount);

		mbtDiag_Data (pDiag, buf);
	}
	return ret;
}


int mbtDecode_WriteMultipleRegisters (MB_DIAGNOSTICS_ST *pDiag, uint8 *pBuff, int length,
			uint16 *startAddress, uint16 *quantity, uint8 *byteCount, uint16 *registers)
{
	int	i, idx, ret = 0;

	
	idx = MBT_HEADER_LENGTH;
	*startAddress = pBuff[idx] << 8 | pBuff[idx+1];
	idx += 2;
	*quantity = pBuff[idx] << 8 | pBuff[idx+1];
	idx += 2;
 	*byteCount = pBuff[idx];
	idx += 1;
	if (*quantity <= MB_MAX_QUANTITY_MULTIPLE_REGISTERS)
	{
		for (i=0; i<*quantity; i++)
		{
			*(registers+i) = pBuff[idx] << 8 | pBuff[idx+1];
			idx += 2;
		}
	}
	else
		ret = 1;
	
	if (mbtDiag_CanPrint(pDiag))
	{
		int n;
		char	fc[32], buf[256];
#ifdef _WIN32
		n = u_snprintf (buf, sizeof(buf),
#else
		n = snprintf (buf, sizeof(buf),
#endif
			"\n~~~> [Indication] '%s' startAddress(0x%.4X), quantity(%d) byteCount(%d)\n",
			sprt_mb_func_code (fc, sizeof(fc), pBuff[MBT_FC_INDEX]),
			*startAddress, *quantity, *byteCount);

		mbtDiag_Data (pDiag, buf);
	}
	return ret;
}

