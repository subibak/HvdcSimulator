
#define _PMS_COMMON_SPRT_DEF	

#ifdef _WIN32
#include <winsock2.h>
#include <time.h>
#endif
#include	"pmsCommon.h"

char *sprt_device_status (char *out, int outSize, DEV_STS_ENUM eStatus)
{
	if (eStatus >= 0 && eStatus < ARRAY_SIZE(_device_sts))
		u_strncpy (out, _device_sts[eStatus], outSize);
	else
		u_snprintf (out, outSize, "unknown(%d)", eStatus);
	return out;
}

int u_snprintf (char *buf, int count, const char *format, ...)
{
	va_list va;
	int len;

	
	va_start(va, format);
#ifdef	VXWORKS
	(void)vsprintf(buf, format, va);
#else
	(void)_vsnprintf(buf, count, format, va);
#endif
	va_end(va);
	
	
	buf[count-1] = 0;
	len = strlen(buf); 
	return (len);
}


TMWTYPES_MILLISECONDS GetMSTime (void)
{
  
#if 0   
	return (((double)tickGet() / sysClkRateGet()) * 1000);
#else
	
	
#ifdef _WIN32
	return (GetTickCount());
#else
	return ((1000 / sysClkRateGet()) * tickGet());
#endif

#endif	
}



TMWTYPES_BOOL IsTimeExpired (TMWTYPES_MILLISECONDS *pLastTime, TMWTYPES_MILLISECONDS interval)
{
	TMWTYPES_MILLISECONDS currentTime, diff;

	currentTime = GetMSTime();
	if (currentTime >= *pLastTime)
	{
		diff = currentTime - *pLastTime;
	}
	else	
	{
		
		diff = (0xFFFFFFFF - *pLastTime) + 1 + currentTime;
	}
	if (diff >= interval)
	{
		*pLastTime = currentTime;
		return TMWDEFS_TRUE;
	}
	return TMWDEFS_FALSE;
}	

TMWTYPES_BOOL IsTimeExpiredEx (TMWTYPES_MILLISECONDS currentTime, TMWTYPES_MILLISECONDS *pLastTime, TMWTYPES_MILLISECONDS interval)
{
	TMWTYPES_MILLISECONDS diff;

	if (currentTime == 0)
		currentTime = GetMSTime();
	if (currentTime >= *pLastTime)
	{
		diff = currentTime - *pLastTime;
	}
	else	
	{
		
		diff = (0xFFFFFFFF - *pLastTime) + 1 + currentTime;
	}
	if( diff >= interval)
	{
		*pLastTime = currentTime;
		return TMWDEFS_TRUE;
	}
	return TMWDEFS_FALSE;
}	



void mSleep (int mSec)
{
	int ticks;
#ifdef _WIN32
	Sleep (mSec);
#else
	ticks = (mSec * sysClkRateGet()) / 1000;
	if (ticks < 1)
		ticks = 1;
	taskDelay (ticks);
#endif
}


#ifdef	_WIN32
 
static const uint64 epoch = (uint64)116444736000000000; 
#endif
void u_getTimeval (struct timeval *pTv)
{
#ifdef	VXWORKS
	int ret; 
	struct timespec tp; 

	if ( (ret=clock_gettime(CLOCK_REALTIME, &tp))==0) 
	{ 
		pTv->tv_sec  = tp.tv_sec + getSysClockTime(); 
		pTv->tv_usec = (tp.tv_nsec + 500) / 1000;

	}
	else
	{ 
		pTv->tv_sec  = 0; 
		pTv->tv_usec = 0;
	}
#elif	(defined(_WIN32))
	FILETIME	file_time;
	SYSTEMTIME	system_time; 
	ULARGE_INTEGER ularge; 
	
	GetSystemTime(&system_time); 
	SystemTimeToFileTime(&system_time, &file_time); 
	ularge.LowPart = file_time.dwLowDateTime; 
	ularge.HighPart = file_time.dwHighDateTime; 
	
	pTv->tv_sec = (long)((ularge.QuadPart - epoch) / 10000000L); 
	pTv->tv_usec = (long)(system_time.wMilliseconds * 1000); 
#else
	gettimeofday(pTv, NULL);
#endif
}

void u_makeTimeval(	int year, int month, int day,
					int hour, int minute, int second, int milisecond,
					struct timeval *pTvOut)
{
	struct tm ts;

	memset(&ts, 0, sizeof(ts));
	ts.tm_year = year - 1900;
	ts.tm_mon = month - 1;
	ts.tm_mday = day;
	ts.tm_hour = hour;
	ts.tm_min = minute;
	ts.tm_sec = second;

	pTvOut->tv_sec = (long)mktime(&ts);
	pTvOut->tv_usec = milisecond * 1000;
}

char *sprt_TimeVal (char *pBufOut, size_t bufSize, struct timeval *pTv)
{
	struct tm t;

#ifdef _WIN32
	struct tm *s = localtime((time_t *)&pTv->tv_sec); 
	
	if (s == NULL) 
			return NULL; 

	memcpy(&t, s, sizeof(struct tm)); 
#else
	localtime_r ((time_t *)&pTv->tv_sec, &t);
#endif
	u_snprintf(pBufOut, bufSize, "%d-%02d-%02d %02d:%02d:%02d.%03d",
			t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
			t.tm_hour, t.tm_min, t.tm_sec, pTv->tv_usec / 1000);
	

	return pBufOut;
}



int Tcp_IsClientConnected (int socket, int timeout)
{
	fd_set fdset;
	struct timeval tvTimeout;
	struct sockaddr_in tempAddr;
	int addrLen;

	tvTimeout.tv_sec = timeout / 1000;
	tvTimeout.tv_usec = (timeout % 1000) * 1000;
	FD_ZERO(&fdset);
	FD_SET(socket, &fdset);
	if (select (0, NULL, &fdset, NULL, &tvTimeout) == ERROR)
	{
		return -1;
	}
	addrLen = sizeof(tempAddr);
	if(FD_ISSET(socket, &fdset))
	{
		if(getpeername(socket, (struct sockaddr *)&tempAddr, &addrLen) != ERROR)
		{
			
			return 1;
		}
	}
	return 0;
}


int Tcp_BindForServer (int port)
{
	int fd = -1;
	int i, l_onoff = 1, optval;
	struct sockaddr_in  serv_addr;

	
	if ((fd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
	{
		logtE ("socket error, <%d:%s>\n", errno, strerror(errno));
		return -1;
	}
	
	memset ((char *)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	serv_addr.sin_port = htons (port);
	for (i=0; i < 10; i++)
	{
		if (setsockopt (fd, SOL_SOCKET, SO_REUSEADDR,
					(char *)&l_onoff, sizeof(l_onoff)) < 0)
		{
			logtW ("setsockopt(SO_REUSEADDR) failed <%d:%s>\n",
					errno, strerror(errno));
		}
		if (bind (fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		{
			logtW( "bind error, so retry <%d:%s>\n", errno, strerror(errno) );
#ifdef VXWORKS
			sleep(1);
#elif _WIN32
			Sleep(1);
#endif
		}
		else
			break;
	}
	if (i >= 10)
	{
		logtE ("bind fail\n");

#ifdef _WIN32
		closesocket(fd);
#else
		close (fd);
#endif
		
		return -1;
	}
	logtN ("bind OK..., port=%d\n", port);
	if (listen( fd, 5) < 0)
	{
		logtW( "listen failed <%d:%s>\n", errno, strerror(errno) );
		
#ifdef _WIN32
		closesocket(fd);
#else
		close(fd);
#endif
		
		return -1;
	}
	logtN ("listen OK..., port=%d\n", port);
	optval = 1;
	if (setsockopt (fd, SOL_SOCKET, SO_KEEPALIVE,
		(char *)&optval, sizeof(optval)) < 0)
	{
		logtW ("setsockopt(SO_KEEPALIVE) failed <%d:%s>\n",
			errno, strerror(errno));
	}
	return fd;
}


int Tcp_AcceptForServer (int listenFd, int timeout, int *pAcceptFd, char *pPeerIp)
{
	fd_set fds;
	struct timeval tvTimeout;
	int retval;
	int sockLen;
	struct sockaddr_in sockAddr;	
	int acceptFd;

	if (listenFd <= 0)
		return 0;

	FD_ZERO (&fds);
	FD_SET (listenFd, &fds);
	tvTimeout.tv_sec = timeout / 1000;
	tvTimeout.tv_usec = (timeout % 1000) * 1000;

	retval = select (listenFd+1, &fds, NULL, NULL, &tvTimeout);
	if (retval < 0)
	{
		logtW ("select failed <%d:%s>\n", errno, strerror(errno));
		return -1;
	}
	if (FD_ISSET(listenFd, &fds))
	{
		sockLen = sizeof(sockAddr);
		acceptFd = accept (listenFd, (struct sockaddr *)&sockAddr, &sockLen);
		if (acceptFd < 0)
		{
			logtW("accept failed <%d:%s>\n", errno, strerror(errno));
			return -1;
		}
		*pAcceptFd = acceptFd;
		strcpy (pPeerIp, inet_ntoa(sockAddr.sin_addr));

		return 1;
	}
	return 0;
}


int Tcp_Read (int fd, unsigned char *rx, int length, int timeout)
{
	int r_len = 0;
	struct timeval timer;
	fd_set fds;

	if (fd < 0)
	{
		logtW( "Tcp_Read : fd is not opened\n" );
		return -1;
	}

	FD_ZERO (&fds);
	FD_SET (fd, &fds);
	timer.tv_sec = timeout / 1000;
	timer.tv_usec = ( timeout % 1000 ) * 1000;

	if (select (fd + 1, &fds, NULL, NULL, &timer) == ERROR)
	{
		logtW( "Tcp_Read : select error <%d:%s>\n", errno, strerror(errno) );
		return -2;
	}
	if (FD_ISSET (fd, &fds))
	{
#ifdef _WIN32
		r_len = recv (fd, (char *)rx, length, 0);
#else
		r_len = read (fd, rx, length);
#endif
		if( r_len < 0 )
		{
			logtW( "Tcp_Read : read error <%d:%s>\n", errno, strerror(errno) );
			return -2;
		}
		if( r_len == 0 )
		{
			logtW( "Tcp_Read : received 0 byte length <%d>.... closed by peer\n",length);
			return -3;
		}
	}
	
	
	
	
	
	
	return r_len;
}


int Tcp_Write (int fd, unsigned char *tx, int length)
{
	int w_bytes;

	if (fd < 0)
	{
		logtW( "fd is not opened\n" );
		return -1;
	}
	
	
	
	
	
#ifdef _WIN32
	w_bytes = send (fd, (char *)tx, length, 0);
#else
	w_bytes = write (fd, tx, length);
#endif

	if (w_bytes < 0)
	{
		logtW( "write error <%d:%s>\n", errno, strerror(errno) );
		return -1;
	}
	else if (w_bytes != length)
	{
		logtW( "write length error %d != %d\n", w_bytes, length );
		return 0;
	}
	return length;
}



TMWTYPES_BOOL IsActiveSystem (void)
{
#ifdef _SAEIL_TEST
	static int count = 0;
	static TMWTYPES_BOOL bActive = TMWDEFS_TRUE;
	static TMWTYPES_BOOL bInitEnd = TMWDEFS_FALSE;

	if (!bInitEnd)
	{
		printf ("==================================================\n");
		printf ("Warnning : This Program is compiled by _SAEIL_TEST\n");
		printf ("==================================================\n");
		bInitEnd = TMWDEFS_TRUE;
	}
	if (++count > 1000)
	{
		count = 0;
		bActive = bActive ? TMWDEFS_FALSE : TMWDEFS_TRUE;
		printf ("==================================================\n");
		printf ("Warnning : This Program is compiled by _SAEIL_TEST\n");
		printf ("==================================================\n");
	}
	return bActive;
#else
	return TMWDEFS_TRUE;
#endif
}

