

#include "modbustcp\mbtServerProto.h"


#define MAX_ROW_LENGTH 16


void mbtDiag_Open (MB_DIAGNOSTICS_ST *pDiag, char *key)
{
	memset (pDiag, 0, sizeof(*pDiag));
	strcpy (pDiag->key, key);
	return;
}


void mbtDiag_Close (MB_DIAGNOSTICS_ST *pDiag)
{
	return;
}


void mbtDiag_SetDiaplayFlag (MB_DIAGNOSTICS_ST *pDiag, int flag)
{
	pDiag->logDispFlag = flag;
}

#if 0   

int mbtDiag_check( MB_DIAGNOSTICS_ST *pDiag, TYPES_DATETIME *pT )
{
	char	filename[32], dir[256], fname[256];

	if( pDiag->logFileFlag == 0 )
		return 0;

	if( pT == NULL )	
	{
		if( pDiag->fp )
			return 1;
		else
			return 0;
	}
	if( pDiag->fp )
	{
		
		if( pDiag->lastOpenDtime.hour != pT->hour )
		{
			fclose( pDiag->fp );
			pDiag->fp = NULL;
		}
	}
	if( pDiag->fp == NULL )
	{
		
		sprintf( dir, "%s/log/pmuLog/%s",
				getProjectDir(), pDiag->key );
		
		if( mkdir( dir, S_IRWXU | S_IRWXG | S_IRWXO  ) < 0 )
		{
			if( errno != EEXIST )
			{
				logtW( "mkdir fail : %s, <%d:%s>\n", 
						dir, errno, strerror(errno) );
				pDiag->logFileFlag = 0;
				return 0;
			}
		}
		
		sprintf( filename, "%04d%02d%02d_%02d.%s",
				pT->year, pT->month, pT->day, pT->hour, pDiag->key );
		sprintf( fname, "%s/%s", dir, filename );

		if( ( pDiag->fp = fopen( fname, "a+") ) == NULL )
		{
			logtW( "file(%s) open error<%d:%s>\n", fname, errno, strerror(errno) );
			pDiag->logFileFlag = 0;
			return 0;
		}
		memcpy( &pDiag->lastOpenDtime, pT, sizeof(TYPES_DATETIME) );
	}
	return 1;
}

#endif	


int mbtDiag_CanPrint (MB_DIAGNOSTICS_ST *pDiag)
{
	return pDiag->logDispFlag;
}



void mbtDiag_Packet (MB_DIAGNOSTICS_ST *pDiag, TYPES_DATETIME *timeReceived, uint8 *msg, int length, int tx)
{

	int		i, n, len, index, rowLength;
	char	buf[256];
	MBT_COMM_HEADER_ST	head;
	TYPES_DATETIME mTime, *pTime;

	if (pDiag->logDispFlag == 0)
		return;

	if (timeReceived == NULL)
	{
		GetDateTime (&mTime);
		pTime = &mTime;
	}
	else
		pTime = timeReceived;

	mtpDecode_Header (msg, &head);

	n = sprintf (buf, "\n%02u:%02u:%02u.%03u: %s ",
		pTime->hour, pTime->minute, pTime->second,
		pTime->mSec,
		tx ? "<..." : "...>" );

	sprintf (buf + n, "TID=0x%04X UID=0x%02X FC=0x%02X, Frame length=%d\n",
		head.transactionId, head.unitId, head.functionCode, length );

	printf ("%s", buf);

	n = 6;
	memset (buf, ' ', n);

	index = 0;
	len = n;
	for (i = 0; i < 8; i++, index++)
		len += sprintf(buf + len, "%02X ", msg[index]);
	sprintf(buf + len, "\n");
	printf ("%s", buf);

	while(index < length )
	{
		memset (buf, ' ', n);
		rowLength = length - index;
		if(rowLength > MAX_ROW_LENGTH)
			rowLength = MAX_ROW_LENGTH;

		len = n;
		for(i = 0; i < rowLength; i++, index++)
			len += sprintf(buf + len, "%02X ", msg[index]);

		sprintf(buf + len, "\n");
		printf ("%s", buf);
	}
}


void mbtDiag_Error (MB_DIAGNOSTICS_ST *pDiag, TYPES_DATETIME *timeReceived, uint8 *msg, int length)
{
	int		i, n, len, index, rowLength;
	char	buf[256];

	if (pDiag->logDispFlag == 0)
		return;

	n = sprintf (buf, "\n%02u:%02u:%02u.%03u: ...> ",
		timeReceived->hour, timeReceived->minute, timeReceived->second,
		timeReceived->mSec);

	sprintf (buf + n, "Error : length=%d\n", length);

	printf ("%s", buf);

	index = 0;
	n = 6;
	while (index < length)
	{
		memset (buf, ' ', n);
		rowLength = length - index;
		if (rowLength > MAX_ROW_LENGTH)
			rowLength = MAX_ROW_LENGTH;

		len = n;
		for(i = 0; i < rowLength; i++)
			len += sprintf (buf + len, "%02X ", msg[index++]);

		sprintf (buf + len, "\n");
		printf ("%s", buf);
	}
}


void mbtDiag_Application (MB_DIAGNOSTICS_ST *pDiag, TYPES_DATETIME *timeReceived, char *msg1, char *msg2)
{
	int		n;
	char	buf[256];
	TYPES_DATETIME mTime, *pTime;

	if (pDiag->logDispFlag == 0)
		return;

	if (timeReceived == NULL)
	{
		GetDateTime (&mTime);
		pTime = &mTime;
	}
	else
		pTime = timeReceived;

	n = sprintf (buf, "\n%02u:%02u:%02u.%03u: ...> ",
		pTime->hour, pTime->minute, pTime->second,
		pTime->mSec);

	sprintf (buf + n, "%s : %s\n", msg1, msg2);

	printf ("%s", buf);
}


void mbtDiag_Data (MB_DIAGNOSTICS_ST *pDiag, char *msg)
{
	if (pDiag->logDispFlag == 0)
		return;

	printf ("%s", msg);
}

