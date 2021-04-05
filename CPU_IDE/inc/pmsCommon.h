
#ifndef _pmsCommon_h
#define _pmsCommon_h

#include <stdio.h>
#include <string.h>

#include "sysDefs.h"
#include "usrTypes.h"
#include "osDepend.h"
#include "usrDefBuf.h"
#include "usrErrCode.h"

typedef unsigned long  TYPES_MILLISECONDS;  

#ifdef  _MBTS_PROGRAM	

#ifdef __cplusplus
typedef bool           TMWTYPES_BOOL;   
#else
typedef unsigned char  TMWTYPES_BOOL;   
#endif

typedef unsigned char  TMWTYPES_BYTE;   
typedef          char  TMWTYPES_CHAR;   
typedef unsigned char  TMWTYPES_UCHAR;  
typedef          short TMWTYPES_SHORT;  
typedef unsigned short TMWTYPES_USHORT; 
typedef          long  TMWTYPES_LONG;   
typedef unsigned long  TMWTYPES_ULONG;  
typedef          int   TMWTYPES_INT;    
typedef unsigned int   TMWTYPES_UINT;   
typedef          float TMWTYPES_SFLOAT; 
                                        
                                        
                                        
                                        
                                        
typedef         double TMWTYPES_DOUBLE; 
                                        
                                        
                                        
                                        
                                        
                                        

typedef TMWTYPES_ULONG TMWTYPES_MILLISECONDS;


typedef unsigned char	uint8;
typedef unsigned short	uint16;


#define TMWDEFS_SECONDS(s)  ((s)*1000UL)


#define TMWDEFS_TRUE  (0==0)
#define TMWDEFS_FALSE (0==1)

#endif	

#ifdef __cplusplus
#else
#define	true	TMWDEFS_TRUE
#define	false	TMWDEFS_FALSE
#endif


#define GetDateTime			systemTimeRead
#define TYPES_DATETIME		strTimeDataInfo

#define	U_IPC_LOCK_HANDLE_T	TMWDEFS_RESOURCE_LOCK	
#define	U_INVALID_HANDLE	0

#define	u_malloc			tmwtarg_alloc
#define	u_free				tmwtarg_free
#define	u_lockCreate		tmwtarg__lockInit
#define	u_lockDestroy		tmwtarg__lockDelete
#define	u_lockSection		tmwtarg__lockSection
#define	u_unlockSection		tmwtarg__unlockSection


#define timerisset(tvp)         ((tvp)->tv_sec || (tvp)->tv_usec)
#define timercmp(tvp, uvp, cmp) \
        ((tvp)->tv_sec cmp (uvp)->tv_sec || \
         (tvp)->tv_sec == (uvp)->tv_sec && (tvp)->tv_usec cmp (uvp)->tv_usec)
#define timerclear(tvp)         (tvp)->tv_sec = (tvp)->tv_usec = 0



#define ARRAY_SIZE(arr)	(sizeof (arr) / sizeof (arr[0]))
#define	MASK_DATA(old, mask, data)	(( (mask) & (data) ) | ( ~(mask) & (old) ))

#define	u_strncpy(d, s, n)	strcpy(d, s)
#define	strcpy_s(d, n, s)	strcpy(d, s)

#define	logtE	printf
#define	logtW	printf
#define	logtN	printf
#define	logtI	printf
#define	logtD	printf

#define	logsE	printf
#define	logsW	printf
#define	logsN	printf
#define	logsI	printf
#define	logsD	printf

#define	MAX_NAME_SZ				16		


typedef enum
{
	DEV_STS_NONE,		
	DEV_STS_STOP,		
	DEV_STS_WAIT,		
	DEV_STS_RUN,		
	DEV_STS_PRI,		
	DEV_STS_SEC,		
	DEV_STS_FAIL		
}	DEV_STS_ENUM;

#ifdef	_PMS_COMMON_SPRT_DEF
	static char *_device_sts[7] = 
			{ "NONE", "STOP", "WAIT", "RUN", "PRI", "SEC", "FAIL" };
#endif

#ifdef __cplusplus
extern "C" {
#endif


void 	*memoryAlloc(int32 );
uint32	systemTimeRead (strTimeDataInfo *dataPtr);
uint32	systemTimeWrite (strTimeDataInfo *dataPtr);
uint32 nvRamWrite(int8 *, int32 , int32 );
uint32 nvRamRead(int8 *, int32 , int32 );
void setErrorCode(uint8 *, uint32, uint8 *, uint32);
time_t getSysClockTime (void);


char *sprt_device_status (char *out, int outSize, DEV_STS_ENUM eStatus);
int u_snprintf (char *buf, int count, const char *format, ...);
TMWTYPES_MILLISECONDS GetMSTime (void);
TMWTYPES_BOOL IsTimeExpired (TMWTYPES_MILLISECONDS *pLastTime, TMWTYPES_MILLISECONDS interval);
TMWTYPES_BOOL IsTimeExpiredEx (TMWTYPES_MILLISECONDS currentTime, TMWTYPES_MILLISECONDS *pLastTime, TMWTYPES_MILLISECONDS interval);
void mSleep (int mSec);
void u_getTimeval (struct timeval *pTv);
void u_makeTimeval(int year, int month, int day,
	int hour, int minute, int second, int milisecond,
	struct timeval *pTvOut);

char *sprt_TimeVal (char *pBufOut, size_t bufSize, struct timeval *pTv);

int Tcp_IsClientConnected (int socket, int timeout);
int Tcp_BindForServer (int port);
int Tcp_AcceptForServer (int listenFd, int timeout, int *pAcceptFd, char *pPeerIp);
int Tcp_Read (int fd, unsigned char *rx, int length, int timeout);
int Tcp_Write (int fd, unsigned char *tx, int length);

TMWTYPES_BOOL IsActiveSystem (void);

#ifdef __cplusplus
}
#endif

#endif 



