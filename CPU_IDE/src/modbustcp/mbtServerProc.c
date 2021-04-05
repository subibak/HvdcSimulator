#ifdef _WIN32
#include	<winsock2.h>
#endif

#include "modbustcp\mbtServer.h"
#include "modbustcp\mbtServerProto.h"
#include "sysConfig.h"
#include "network.h"
#include "lkv120Board.h"
#ifdef VXWORKS
#include  "vmedef.h"
#endif
#include  "pmsUtil.h"


CFG_MBT_SERVER_ST g_MbtServerCfg;


INF_DEV_MBTS_ST g_MbtsInf[MAX_DEV_MBTS];




static void MBTS_ProcessChannelError (INF_CHANNEL_MBTS_ST *pCh)
{
	MBT_ERR_DATA_ST	*pE = &pCh->rxError;

	if (pE->msgLength == 0)
		return;

	
	pCh->commStat.byteReceivedErrorCount += pE->msgLength;

	mbtDiag_Error (&pCh->diag, &pE->timeReceived, pE->msgBuffer, pE->msgLength);

	pE->msgLength = 0;
}


static void MBTS_AddChannelError (INF_CHANNEL_MBTS_ST *pCh, uint8 *msgBuffer, uint16 msgLength)
{
	MBT_ERR_DATA_ST	*pE = &pCh->rxError;

	if (pE->msgLength == 0)
	{
		GetDateTime (&pE->timeReceived);
		pE->firstByteTime = GetMSTime ();
	}

	if ((pE->msgLength + msgLength) >= sizeof(pE->msgBuffer))
		MBTS_ProcessChannelError (pCh);

	if (msgLength >= sizeof(pE->msgBuffer))
	{
		logtW( "MBTS_AddChannelError: too big msgLength %d\n", msgLength );
		memcpy (&pE->msgBuffer[0], msgBuffer, sizeof(pE->msgBuffer));
		pE->msgLength = sizeof(pE->msgBuffer);
		MBTS_ProcessChannelError (pCh);
		return;
	}
	memcpy (&pE->msgBuffer[pE->msgLength], msgBuffer, msgLength);
	pE->msgLength += msgLength;
}



static void MBTS_OpenChannel (INF_CHANNEL_MBTS_ST *pCh)
{
	
	mbtDiag_Open (&pCh->diag, pCh->peerIp);

	mbtDiag_Application (&pCh->diag, NULL, "OPEN", pCh->peerIp);

	pCh->pI->channelCount++;

	pCh->msg.rxState = MBT_RX_STATE_IDLE;
	pCh->msg.lastByteTime = GetMSTime ();	
	pCh->rxError.msgLength = 0;

	
	pCh->commStat.openCount++;
	pCh->commStat.byteSentCount = 0;
	pCh->commStat.byteReceivedCount = 0;
	pCh->commStat.byteReceivedErrorCount = 0;
	pCh->commStat.frameSentCount = 0;
	pCh->commStat.frameReceivedCount = 0;
	logtN ("channel open, peerIp(%s), channelCount(%d) commFd(%d)\n",
			pCh->peerIp, pCh->pI->channelCount, pCh->commFd);
}


static void MBTS_CloseChannel (INF_CHANNEL_MBTS_ST *pCh, char *reason)
{
	if (pCh->commFd > 0)
	{
		
		MBTS_ProcessChannelError (pCh);
		mbtDiag_Application (&pCh->diag, NULL, "CLOSE", reason);

		if( --pCh->pI->channelCount < 0 )
			pCh->pI->channelCount = 0;
		logtN( "channel close, peerIp(%s), channelCount(%d) commFd(%d)\n",
				pCh->peerIp, pCh->pI->channelCount, pCh->commFd );
		shutdown (pCh->commFd, 2);	

#ifdef _WIN32
		closesocket(pCh->commFd);
#else
		close (pCh->commFd);
#endif 
		
		pCh->commFd = -1;
		

	}
	
	mbtDiag_Close (&pCh->diag);
}


TMWTYPES_INT MBTS_LoadConfig (void)
{
	int index;
	CFG_DEV_MBTS_ST *pC;

	
	if (nvRamRead ((int8 *) &g_MbtServerCfg,
			USR_MBTS_CONFIG_OFFSET, sizeof(g_MbtServerCfg)) == NO_ERROR)
	{
		printf ("'MBTS Configuration' is loaded from NV-RAM  !!\n");
	}
	else
		printf ("MBTS_LoadConfig Error  !!\n");

	
	if (g_MbtServerCfg.iNumDevMbts < 0 || g_MbtServerCfg.iNumDevMbts > MAX_DEV_MBTS)
		g_MbtServerCfg.iNumDevMbts = 0;		
	for (index=0; index < g_MbtServerCfg.iNumDevMbts; index++)
	{
		pC = &g_MbtServerCfg.arDevMbts[index];
		if (pC->iChannelMax <= 0 || pC->iChannelMax > MAX_CHANNEL_MBTS)
			pC->iChannelMax = 1;	
		if (pC->uTcpPort < 1 || pC->uTcpPort > 65535)
			pC->uTcpPort = MBT_TCP_PORT;	
		if (pC->iRxFrameTimeout < 1000 || pC->iRxFrameTimeout > 10000)
			pC->iRxFrameTimeout = 3000;	
		if (pC->iMaxCommIdleTime < 0 || pC->iMaxCommIdleTime > 86400)
			pC->iMaxCommIdleTime = 0;	
	}
	return g_MbtServerCfg.iNumDevMbts;
}

static TMWTYPES_BOOL MBTS_InitializeInfo(CFG_DEV_MBTS_ST *pC, INF_DEV_MBTS_ST *pI)
{
	int i;
	INF_CHANNEL_MBTS_ST *pCh;

	pI->listenFd = -1;
	pI->bRun = TMWDEFS_FALSE;

	pCh = &pI->tChan[0];

	for (i=0; i<MAX_CHANNEL_MBTS; i++, pCh++)
	{
		pCh->iIndex = i;
		pCh->pC = pC;
		pCh->pI = pI;
		
		
		pCh->commFd = -1;
	}

	return TMWDEFS_TRUE;
}

static void MBTS_DestroyInfo (CFG_DEV_MBTS_ST *pC, INF_DEV_MBTS_ST *pI)
{
	if (pI->listenFd > 0)
	{
		shutdown (pI->listenFd, 2);	
#ifdef _WIN32
		closesocket(pI->listenFd);
#else
		close (pI->listenFd);
#endif 	
		pI->listenFd = -1;
	}
}



static int MBTS_AcceptPeerProc (CFG_DEV_MBTS_ST *pC, INF_DEV_MBTS_ST *pI)
{
	int sockLen;
	struct sockaddr_in sockAddr;	
	int acceptFd;
	int i;
	INF_CHANNEL_MBTS_ST *pCh;

	if (pI->listenFd <= 0)
		return 0;
	if (pI->channelCount >= MAX_CHANNEL_MBTS)
	{
		logtW("Connect requested by peer, But there is no empty channel\n");
		return -1;
	}
	sockLen = sizeof(sockAddr);
	acceptFd = accept (pI->listenFd, (struct sockaddr *)&sockAddr, &sockLen);
	if (acceptFd < 0)
	{
		logtW("accept failed <%d:%s>\n", errno, strerror(errno));
		return -1;
	}
	logtI("accept OK... fd(%d) addr(%s)\n", 
			acceptFd, inet_ntoa(sockAddr.sin_addr));

	
	pCh = &pI->tChan[0];
	for (i=0; i<MAX_CHANNEL_MBTS; i++, pCh++)
	{
		if (pCh->commFd <= 0)
		{
			pCh->commFd = acceptFd;
			u_snprintf (pCh->peerIp, sizeof(pCh->peerIp), "%s", inet_ntoa(sockAddr.sin_addr));
			MBTS_OpenChannel (pCh);
			return 1;
		}
	}

#ifdef _WIN32
	closesocket(acceptFd);
#else
	close (acceptFd);
#endif 
	
	logtW("Connect requested by peer, But there is no empty channel\n");
	return -1;
}


static int MBTS_CheckAsyncProc (CFG_DEV_MBTS_ST *pC, INF_DEV_MBTS_ST *pI, int timeout)
{
	fd_set fds;
	struct timeval tvTimeout;
	int max_fd = 0, retval;
	int i, count;
	INF_CHANNEL_MBTS_ST *pCh;

	FD_ZERO (&fds);

		
	if (pI->listenFd > 0 && pI->channelCount < MAX_CHANNEL_MBTS)
	{
		FD_SET (pI->listenFd, &fds);
		if (pI->listenFd > max_fd)
			max_fd = pI->listenFd;
	}
	
	pCh = &pI->tChan[0];
	for (count=0, i=0; i<MAX_CHANNEL_MBTS; i++, pCh++)
	{
		if (pCh->commFd > 0)
		{
			FD_SET (pCh->commFd, &fds);
			if (pCh->commFd > max_fd)
				max_fd = pCh->commFd;
			count++;
		}
		if( count >= pI->channelCount )
			break;
	}
	tvTimeout.tv_sec = timeout / 1000;
	tvTimeout.tv_usec = (timeout % 1000) * 1000;

	retval = select (max_fd+1, &fds, NULL, NULL, &tvTimeout);
	if (retval < 0)
	{
		logtW ("select failed <%d:%s>\n", errno, strerror(errno));
		return -1;
	}
	else if (retval == 0)
	{
		return 0;
	}
	if (pI->listenFd > 0 && FD_ISSET(pI->listenFd, &fds))
	{
		MBTS_AcceptPeerProc (pC, pI);
	}
	return 1;
}


static void MBTS_SendFrame (INF_CHANNEL_MBTS_ST *pCh)
{
	MBT_MSG_DATA_ST *msg = &pCh->msg;
	int nWritten;

	if (pCh->commFd <= 0)
		return;

	nWritten = Tcp_Write (pCh->commFd, msg->txBuffer, msg->txLength);
	if( nWritten <= 0 )
	{
		MBTS_CloseChannel (pCh, "TCP write error");
		return;
	}
	
	pCh->commStat.frameSentCount++;
	pCh->commStat.byteSentCount += nWritten;

	
	GetDateTime (&msg->timeSent);
	mbtDiag_Packet (&pCh->diag, &msg->timeSent, msg->txBuffer, msg->txLength, 1 );
}


static void MBTS_ReceivedFrame (INF_CHANNEL_MBTS_ST *pCh)
{
	MBT_MSG_DATA_ST *msg = &pCh->msg;
	int	exceptionCode = 0;
	uint16	startAddress, quantity, value;
	uint16	registers[MB_MAX_QUANTITY_REGISTERS];
	uint8		status[MB_MAX_QUANTITY_STATUS/8];
	uint8		byteCount;

	
	mbtDiag_Packet (&pCh->diag, &msg->timeReceived, msg->rxBuffer, msg->rxLength, 0);

	
	pCh->commStat.frameReceivedCount++;

	
	msg->txLength = 0;

	switch (msg->rxHeader.functionCode)
	{
		case	MB_FC_READ_COILS:	
			if (msg->rxHeader.length != 6)
			{
				logtW( "Error : Read Coils - header length(%d) is not valid(6)\n",
					msg->rxHeader.length);
				exceptionCode = MB_EC_ILLEGAL_DATA_VALUE;
				break;
			}
			mbtDecode_ReadData (&pCh->diag, msg->rxBuffer, msg->rxLength, &startAddress, &quantity);
			if (quantity < 1 || quantity > MB_MAX_QUANTITY_STATUS)
			{
				logtW( "Error : Read Coils - quantity(%d) not 1 ~ %d\n", quantity, MB_MAX_QUANTITY_STATUS);
				exceptionCode = MB_EC_ILLEGAL_DATA_VALUE;
				break;
			}
			exceptionCode = mbtsData__ReadCoils (pCh, startAddress, quantity, status);
			if (exceptionCode == 0)
			{
				msg->txLength = mbtEncode_ReadStatusResponse (&pCh->diag, msg->txBuffer, &msg->rxHeader, quantity, status);
			}
			break;

		case	MB_FC_READ_DISCRETE_INPUTS:	
			if (msg->rxHeader.length != 6)
			{
				logtW( "Error : Read Discrete Inputs - header length(%d) is not valid(6)\n",
					msg->rxHeader.length);
				exceptionCode = MB_EC_ILLEGAL_DATA_VALUE;
				break;
			}
			mbtDecode_ReadData (&pCh->diag, msg->rxBuffer, msg->rxLength, &startAddress, &quantity);
			if (quantity < 1 || quantity > MB_MAX_QUANTITY_STATUS)
			{
				logtW( "Error : Read Discrete Inputs - quantity(%d) not 1 ~ %d\n", quantity, MB_MAX_QUANTITY_STATUS);
				exceptionCode = MB_EC_ILLEGAL_DATA_VALUE;
				break;
			}
			exceptionCode = mbtsData__ReadDiscreteInputs (pCh, startAddress, quantity, status);
			if (exceptionCode == 0)
			{
				msg->txLength = mbtEncode_ReadStatusResponse (&pCh->diag, msg->txBuffer, &msg->rxHeader, quantity, status);
			}
			break;

		case	MB_FC_READ_HOLDING_REGISTERS:	
			if (msg->rxHeader.length != 6)
			{
				logtW( "Error : Read Holding Registers - header length(%d) is not valid(6)\n",
					msg->rxHeader.length);
				exceptionCode = MB_EC_ILLEGAL_DATA_VALUE;
				break;
			}
			mbtDecode_ReadData (&pCh->diag, msg->rxBuffer, msg->rxLength, &startAddress, &quantity);
			if (quantity < 1 || quantity > MB_MAX_QUANTITY_REGISTERS)
			{
				logtW( "Error : Read Holding Registers - quantity(%d) not 1 ~ %d\n", quantity, MB_MAX_QUANTITY_REGISTERS);
				exceptionCode = MB_EC_ILLEGAL_DATA_VALUE;
				break;
			}
			exceptionCode = mbtsData__ReadHoldingRegisters (pCh, startAddress, quantity, registers);
			if (exceptionCode == 0)
			{
				msg->txLength = mbtEncode_ReadRegisterResponse (&pCh->diag, msg->txBuffer, &msg->rxHeader, quantity, registers);
			}
			break;

		case	MB_FC_READ_INPUT_REGISTERS:	
			if (msg->rxHeader.length != 6)
			{
				logtW( "Error : Read Input Registers - header length(%d) is not valid(6)\n",
					msg->rxHeader.length);
				exceptionCode = MB_EC_ILLEGAL_DATA_VALUE;
				break;
			}
			mbtDecode_ReadData (&pCh->diag, msg->rxBuffer, msg->rxLength, &startAddress, &quantity);
			if (quantity < 1 || quantity > MB_MAX_QUANTITY_REGISTERS)
			{
				logtW( "Error : Read Input Registers - quantity(%d) not 1 ~ %d\n", quantity, MB_MAX_QUANTITY_REGISTERS);
				exceptionCode = MB_EC_ILLEGAL_DATA_VALUE;
				break;
			}
			exceptionCode = mbtsData__ReadInputRegisters (pCh, startAddress, quantity, registers);
			if (exceptionCode == 0)
			{
				msg->txLength = mbtEncode_ReadRegisterResponse (&pCh->diag, msg->txBuffer, &msg->rxHeader, quantity, registers);
			}
			break;

		case	MB_FC_WRITE_SINGLE_COIL:	
			if (msg->rxHeader.length != 6)
			{
				logtW( "Error : Write Single Coil - header length(%d) is not valid(6)\n",
					msg->rxHeader.length);
				exceptionCode = MB_EC_ILLEGAL_DATA_VALUE;
				break;
			}
			mbtDecode_WriteSingleData (&pCh->diag, msg->rxBuffer, msg->rxLength, &startAddress, &value);
			if (value != 0 && value != 0xFF00)
			{
				logtW( "Error : Write Single Coil - value(%X) is not 0 nor 0xFF00\n", value);
				exceptionCode = MB_EC_ILLEGAL_DATA_VALUE;
				break;
			}
			exceptionCode = mbtsData__WriteSingleCoil (pCh, startAddress, value);
			if (exceptionCode == 0)
			{
				msg->txLength = mbtEncode_WriteSingleDataResponse (&pCh->diag, msg->txBuffer, &msg->rxHeader, startAddress, value);
			}
			break;

		case	MB_FC_WRITE_SINGLE_REGISTER:	
			if (msg->rxHeader.length != 6)
			{
				logtW( "Error : Write Single Register - header length(%d) is not valid(6)\n",
					msg->rxHeader.length);
				exceptionCode = MB_EC_ILLEGAL_DATA_VALUE;
				break;
			}
			mbtDecode_WriteSingleData (&pCh->diag, msg->rxBuffer, msg->rxLength, &startAddress, &value);
			exceptionCode = mbtsData__WriteSingleRegister (pCh, startAddress, value);
			if (exceptionCode == 0)
			{
				msg->txLength = mbtEncode_WriteSingleDataResponse (&pCh->diag, msg->txBuffer, &msg->rxHeader, startAddress, value);
			}
			break;

		case	MB_FC_WRITE_MULTIPLE_COILS:		
			mbtDecode_WriteMultipleCoils (&pCh->diag, msg->rxBuffer, msg->rxLength, &startAddress, &quantity, &byteCount, status);
			if (quantity < 1 || quantity > MB_MAX_QUANTITY_MULTIPLE_COILS)
			{
				logtW( "Error : Write Multiple Coils - quantity(%d) not 1 ~ %d\n", quantity, MB_MAX_QUANTITY_MULTIPLE_COILS);
				exceptionCode = MB_EC_ILLEGAL_DATA_VALUE;
				break;
			}
			if (byteCount < 1 || byteCount > (MB_MAX_QUANTITY_MULTIPLE_COILS/8))
			{
				logtW( "Error : Write Multiple Coils - byteCount(%d) not 1 ~ %d\n", byteCount, MB_MAX_QUANTITY_MULTIPLE_COILS/8);
				exceptionCode = MB_EC_ILLEGAL_DATA_VALUE;
				break;
			}
			if (msg->rxHeader.length != (7+byteCount))
			{
				logtW( "Error : Write Multiple Coils - header length(%d) is not valid(7+byteCount(%d))\n",
					msg->rxHeader.length, byteCount);
				exceptionCode = MB_EC_ILLEGAL_DATA_VALUE;
				break;
			}
			exceptionCode = mbtsData__WriteMultipleCoils (pCh, startAddress, quantity, byteCount, status);
			if (exceptionCode == 0)
			{
				msg->txLength = mbtEncode_WriteMultipleDataResponse (&pCh->diag, msg->txBuffer, &msg->rxHeader, startAddress, quantity);
			}
			break;

		case	MB_FC_WRITE_MULTIPLE_REGISTERS:		
			mbtDecode_WriteMultipleRegisters (&pCh->diag, msg->rxBuffer, msg->rxLength, &startAddress, &quantity, &byteCount, registers);
			if (quantity < 1 || quantity > MB_MAX_QUANTITY_MULTIPLE_REGISTERS)
			{
				logtW( "Error : Write Multiple Registers - quantity(%d) not 1 ~ %d\n", quantity, MB_MAX_QUANTITY_MULTIPLE_REGISTERS);
				exceptionCode = MB_EC_ILLEGAL_DATA_VALUE;
				break;
			}
			if (byteCount != (quantity * 2))
			{
				logtW( "Error : Write Multiple Registers - byteCount(%d) not double of quantity(%d)\n", byteCount, quantity);
				exceptionCode = MB_EC_ILLEGAL_DATA_VALUE;
				break;
			}
			if (msg->rxHeader.length != (7+byteCount))
			{
				logtW( "Error : Write Multiple Registers - header length(%d) is not valid(7+byteCount(%d))\n",
					msg->rxHeader.length, byteCount);
				exceptionCode = MB_EC_ILLEGAL_DATA_VALUE;
				break;
			}
			exceptionCode = mbtsData__WriteMultipleRegisters (pCh, startAddress, quantity, byteCount, registers);
			if (exceptionCode == 0)
			{
				msg->txLength = mbtEncode_WriteMultipleDataResponse (&pCh->diag, msg->txBuffer, &msg->rxHeader, startAddress, quantity);
			}
			break;

		default:
			logtW ("FC(0x%.2X) is invalid\n", msg->rxHeader.functionCode);
			exceptionCode = MB_EC_ILLEGAL_FUNCTION;
			break;
	}

	
	if (exceptionCode)
	{
		msg->txLength = mbtEncode_Error (&pCh->diag, msg->txBuffer, &msg->rxHeader, exceptionCode);
	}
	
	if (msg->txLength)
		MBTS_SendFrame (pCh);
}



static int MBTS_CommunicateChannel (INF_CHANNEL_MBTS_ST *pCh)
{
	int		numCharsReceived, numCharsNeeded;
	int 	error;
	MBT_MSG_DATA_ST *msg = &pCh->msg;
	TMWTYPES_MILLISECONDS lastTime;

#if (BOARD_TYPE == LKV120_BOARD)
    int fd_max;
#endif /* #if (BOARD_TYPE == LKV120_BOARD) */
	struct timeval timer;
	fd_set fds;
    int32   status = NO_ERROR;

	if (pCh->commFd <= 0)
		return 0;

	
	if (msg->rxState == MBT_RX_STATE_IDLE)
	{
		msg->expectedLength = 7;	
		msg->currentLength = 0;		
	}

__READ_DATA:
	numCharsNeeded = msg->expectedLength - msg->currentLength;
	if (numCharsNeeded <= 0)
		numCharsNeeded = 1;
	else if (numCharsNeeded > MBT_MAX_PACKET_LENGTH)
		numCharsNeeded = MBT_MAX_PACKET_LENGTH;

	FD_ZERO (&fds);
	FD_SET (pCh->commFd, &fds);
	timer.tv_sec = 50/*timeout*/ / 1000;
	timer.tv_usec = ( 50/*timeout*/ % 1000 ) * 1000;

#if (BOARD_TYPE == LKV120_BOARD)
        fd_max = pCh->commFd; 
#endif

#if (BOARD_TYPE == LKV120_BOARD)
	status = inOutMultiplex(  fd_max + 1,
                              &fds,
                              NULL,
                              NULL,
                              &timer
							  );
#else
	status = inOutMultiplex( MAX_SOCKET_FD_NUM, 
                              &fds,
                              NULL,
                              NULL,
                              &timer
							  );
#endif

	numCharsReceived = 0;

	if(status == NO_ERROR) 
	{
#ifdef _WIN32
	numCharsReceived = recv(	pCh->commFd, 
								(char *)&msg->rxBuffer[msg->currentLength], 
								numCharsNeeded, 
								0
								);

#else
	numCharsReceived = Tcp_Read (	pCh->commFd, 
									&msg->rxBuffer[msg->currentLength], 
									numCharsNeeded, 
									0
									);
#endif
	}
	
	if (numCharsReceived < 0)
	{
		MBTS_CloseChannel (pCh, "TCP read error");
		return -1;
	}
	else if (numCharsReceived == 0)
		goto __CHECK_RX_FRAME_TIMEOUT;

	
	msg->lastByteTime = GetMSTime();

	
	if (msg->currentLength == 0)
	{
		msg->rxState = MBT_RX_STATE_WAIT_HEAD;
		GetDateTime (&msg->timeReceived);
		msg->firstByteTime = GetMSTime();
	}
	pCh->commStat.byteReceivedCount += numCharsReceived;

	msg->currentLength += numCharsReceived;

__CHECK_RX_STATE_IDLE:

	if (msg->currentLength <= 0)
	{
		msg->rxState = MBT_RX_STATE_IDLE;
		goto __CHECK_RX_FRAME_TIMEOUT;
	}
	if (msg->currentLength < 7)
		goto __CHECK_RX_FRAME_TIMEOUT;

	if (msg->rxState == MBT_RX_STATE_WAIT_HEAD)
	{
		error = 1;
		msg->rxHeader.transactionId = msg->rxBuffer[0] << 8 | msg->rxBuffer[1];
		msg->rxHeader.protocolId    = msg->rxBuffer[2] << 8 | msg->rxBuffer[3];
		msg->rxHeader.length        = msg->rxBuffer[4] << 8 | msg->rxBuffer[5];
		msg->rxHeader.unitId        = msg->rxBuffer[6];
		if (msg->rxHeader.protocolId == 0)
		{
			if (msg->rxHeader.length > 2 &&		
				msg->rxHeader.length <= (1+MBT_MAX_PDU_LENGTH))		
			{
				error = 0;
				msg->expectedLength = 6 + msg->rxHeader.length;
				msg->rxState = MBT_RX_STATE_READ_HEAD;
			}
		}
		if( error )
		{
			
			logtD( "Did not receive correct MBAP Header recevedByte(0x%.2X)\n",
				msg->rxBuffer[0] );
			MBTS_AddChannelError (pCh, &msg->rxBuffer[0], 1);
			memcpy (&msg->rxBuffer[0], &msg->rxBuffer[1], msg->currentLength - 1);
			if (--msg->currentLength > 0)
				goto __CHECK_RX_STATE_IDLE;
		}
	}
	if (msg->currentLength < 8)
		goto __READ_DATA;

	if (msg->rxState == MBT_RX_STATE_READ_HEAD)
	{
		error = 1;
		msg->rxHeader.functionCode = msg->rxBuffer[7];
		
		if (msg->rxHeader.functionCode >= 1 && msg->rxHeader.functionCode <= 127)
			error = 0;
		if (error)
		{
			logtI( "FC(0x%.2X) Error\n",
					msg->rxHeader.functionCode);
			MBTS_AddChannelError (pCh, &msg->rxBuffer[0], 1);
			memcpy (&msg->rxBuffer[0], &msg->rxBuffer[1], msg->currentLength - 1);
			msg->rxState = MBT_RX_STATE_WAIT_HEAD;
			--msg->currentLength;
			goto __CHECK_RX_STATE_IDLE;
		}
		msg->rxState = MBT_RX_STATE_READ_FC;
	}
	if (msg->currentLength < msg->expectedLength)
		goto __READ_DATA;

	if (msg->rxState == MBT_RX_STATE_READ_FC)
	{
		
		MBTS_ProcessChannelError (pCh);

		
		msg->rxLength = msg->expectedLength;


		MBTS_ReceivedFrame (pCh);

		if (msg->currentLength > msg->expectedLength)
		{
			memcpy (&msg->rxBuffer[0], &msg->rxBuffer[msg->expectedLength], msg->currentLength - msg->expectedLength);
			msg->currentLength -= msg->expectedLength;
			msg->rxState = MBT_RX_STATE_WAIT_HEAD;
			goto __CHECK_RX_STATE_IDLE;
		}
		msg->rxState = MBT_RX_STATE_IDLE;
		return 0;
	}

__CHECK_RX_FRAME_TIMEOUT:
	if (msg->rxState != MBT_RX_STATE_IDLE && msg->currentLength > 0)
	{
		
		lastTime = msg->firstByteTime;

		if (IsTimeExpired (&lastTime, pCh->pC->iRxFrameTimeout))
		{

			MBTS_AddChannelError (pCh, msg->rxBuffer, msg->currentLength);
			MBTS_ProcessChannelError (pCh);
			msg->rxState = MBT_RX_STATE_IDLE;
		}
	}

	if (pCh->rxError.msgLength > 0)
		
	{
		lastTime = pCh->rxError.firstByteTime;
		if (IsTimeExpired (&lastTime, TMWDEFS_SECONDS(10)))
		{
			MBTS_ProcessChannelError (pCh);
		}
	}

	
#ifndef _WIN32
	
	if (pCh->pC->iMaxCommIdleTime)
	{
		lastTime = msg->lastByteTime;
		if (IsTimeExpired (&lastTime, pCh->pC->iMaxCommIdleTime * 1000))
		{
			logtW ("TIMEOUT(%d), No data received\n", pCh->pC->iMaxCommIdleTime);
			MBTS_CloseChannel (pCh, "Timeout, No data received");
			return -1;
		}
	}
#endif

	return 0;
}

static TMWTYPES_BOOL MBTS_StartComm (CFG_DEV_MBTS_ST *pC, INF_DEV_MBTS_ST *pI)
{
	if (pI->listenFd <= 0)
	{
		
		if ((pI->listenFd = Tcp_BindForServer (pC->uTcpPort)) == -1)
			return TMWDEFS_FALSE;
	}
	return TMWDEFS_TRUE;
}

static void MBTS_StopComm (CFG_DEV_MBTS_ST *pC, INF_DEV_MBTS_ST *pI)
{
	int i;
	INF_CHANNEL_MBTS_ST *pCh;

	pCh = &pI->tChan[0];
	for (i=0; i<MAX_CHANNEL_MBTS; i++, pCh++)
	{
		if (pCh->commFd > 0)
		{
			MBTS_CloseChannel (pCh, "STOP");
		}
	}
	if (pI->listenFd > 0)
	{
		shutdown (pI->listenFd, 2);	
#ifdef _WIN32
		closesocket(pI->listenFd);
#else
		close (pI->listenFd);
#endif 	
		pI->listenFd = -1;
	}
}

static void MBTS_TerminateProc (CFG_DEV_MBTS_ST *pC, INF_DEV_MBTS_ST *pI)
{
	MBTS_StopComm (pC, pI);
	MBTS_DestroyInfo (pC, pI);
	logtW("MBTS_CommTask(%d) is dead\n", pI->iIndex);
}

#ifdef _WIN32
int MBTS_CheckMaxCommIdleTime (INF_CHANNEL_MBTS_ST *pCh)
{
	TMWTYPES_MILLISECONDS lastTime;
	MBT_MSG_DATA_ST *msg = &pCh->msg;

	if (pCh->commFd <= 0)
		return 0;

	
	if (pCh->pC->iMaxCommIdleTime)
	{
		lastTime = msg->lastByteTime;
		if (IsTimeExpired (&lastTime, pCh->pC->iMaxCommIdleTime * 1000))
		{
			logtW ("TIMEOUT(%d), No data received\n", pCh->pC->iMaxCommIdleTime);
			MBTS_CloseChannel (pCh, "Timeout, No data received");
			return -1;
		}
	}
}
#endif 





int MBTS_CommTask
#ifndef _WIN32
(int index)
#else
(LPVOID lpParam)
#endif
{
#ifdef _WIN32
	int32 index;
#endif
	CFG_DEV_MBTS_ST *pC;
	INF_DEV_MBTS_ST *pI;
	int i;
	INF_CHANNEL_MBTS_ST *pCh;
	TMWTYPES_BOOL bActiveNew, bActiveOld;

#ifdef _WIN32
	index = *(int32*)lpParam;
#endif

	if((index >= MAX_DEV_MBTS)||(index < 0))
	{
		printf ("MBT Server %d Task Parameters Error\n", index);
		return 1;
	}

	pC = &g_MbtServerCfg.arDevMbts[index];
	pI = &g_MbtsInf[index];

	printf ("MBT Server %d Task start !!!!\n", index);
	pI->iIndex = index;

	if (!MBTS_InitializeInfo (pC, pI))
	{
		printf ("MBTS[%d] MBTS_InitializeInfo() Fail\n", index);
		MBTS_TerminateProc (pC, pI);
		return 1;
	}
	
	bActiveOld = TMWDEFS_FALSE;

	for(;;)
	{
		bActiveNew = IsActiveSystem ();
		if (bActiveOld != bActiveNew)
		{
			if (bActiveNew)	
			{
				if (!MBTS_StartComm (pC, pI))
				{
					MBTS_TerminateProc (pC, pI);
					return 1;
				}
				printf ("MBTS[%d] MBTS_StartComm()\n", index);
				pI->bRun = TMWDEFS_TRUE;
			}
			else	
			{
				MBTS_StopComm (pC, pI);
				printf ("MBTS[%d] MBTS_StopComm()\n", index);
				pI->bRun = TMWDEFS_FALSE;
			}
			bActiveOld = bActiveNew;
		}

		if (!pI->bRun)	
		{
			static int count = 0;

			if (++count > 100 && index == 0)
			{
				count = 0;
				printf ("My System is Standby...\n");
			}
			mSleep(100);
			continue;
		}

		
		MBTS_CheckAsyncProc (pC, pI, 50);

		pCh = &pI->tChan[0];
		for (i=0; i<MAX_CHANNEL_MBTS; i++, pCh++)
		{
			if (pCh->commFd > 0)
			{
				MBTS_CommunicateChannel (pCh);
			}
		}
	}
}
