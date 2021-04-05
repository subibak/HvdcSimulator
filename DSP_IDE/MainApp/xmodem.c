/*

 */

 
 
#include "xmodem.h"
#include "Hal/Hwdef/Sram.h"

 
 
#define	TRACE_XMODEM		0


/* ------------------------------------------------------------------------ */
/*  X-Modem Definition                                                      */
/* ------------------------------------------------------------------------ */
/*
 * ascii constants
 */
#define SOH  			0x01
#define STX  			0x02
#define EOT  			0x04
#define	ENQ				0x05
#define ACK  			0x06
#define NAK  			0x15
#define CAN  			0x18
#define CTRLZ 			0x1A

#define MAXRETRANS 		25
#define RETRY_CNT		32


/* ------------------------------------------------------------------------ */
/*  Fuction Return Value                                                    */
/* ------------------------------------------------------------------------ */
#define XDM_SUCCESS    	1
#define XDM_TIMEOUT   	(-1)		/* Timeout Error */
#define XDM_CANCLE_R	(-1)		/* Cancle by remote */
#define XDM_SYNC_ERR   	(-2)		/* Syncrous Error */
#define XDM_RETRY_OV   	(-3)		/* Retry Counter over */
#define XDM_TX_ERROR	(-4)		/* Transmit Error */

#define XDM_FALSE   0
#define XDM_TRUE	1



/* ------------------------------------------------------------------------ */
/*  Global Variables                                                        */
/* ------------------------------------------------------------------------ */
uint8_t	*xbuff;


extern CORE_STATE eManagerState;
extern CORE_STATE ePrevManagerState;

/* ------------------------------------------------------------------------ */
/*  Functions                                                               */
/* ------------------------------------------------------------------------ */


unsigned char	*dest = (unsigned char *)HWREG_SYSSRAM_BASE;
int				lSramSize = SRAM_SIZE;
int32_t			ulNewImgLen;




void sendbyte(uint8_t bChar)
{
	uint32_t	ulLSRReg;
	
	while(true)
	{
		ReadReg(HWREG_UART_LSR, &ulLSRReg);
		
		if((ulLSRReg & HWREG_UART_LSR_TXBUFF_EMPTY_MASK) == HWREG_UART_LSR_TXBUFF_EMPTY_YES)
		{
			WriteReg(HWREG_UART_THR, (bChar&0xFF));
			break;
		}
		else
		{
			DelayUs(100);			
		}

	}
}

int32_t readbyte(uint32_t ulSec, unsigned char *pucChar)
{

	int32_t		lRet = XDM_TIMEOUT;
	uint32_t	ulMiliSec = 0;
	uint32_t	ulLSRReg;

	while(true)
	{
		ReadReg(HWREG_UART_LSR, &ulLSRReg);
		
		if((ulLSRReg & HWREG_UART_LSR_DATA_RDY_MASK) == HWREG_UART_LSR_DATA_RDY_YES)
		{
			ReadReg(HWREG_UART_RBR, pucChar);
			*pucChar = *pucChar & 0xFF;
			lRet = XDM_SUCCESS;			
			break;
		}
		else
		{
			ulMiliSec++;
			DelayUs(100);
			if(ulMiliSec >= ulSec*10000)
			{
				break;
			}
			
		}
	

	}
	return lRet;


}

// 수신 버퍼를 clear 하는 함수
static void
flush(void)
{
	int32_t	retVal;
	unsigned char	c;

	while(true) {
		retVal = readbyte(3, &c);
		if (retVal == XDM_TIMEOUT)		return;
		DelayUs(100);
	}
}

/* COMMAND ================================================================== */

/*
** UART 설정은 아래와 같아야 한다.
** Data Bit    : 8 bit
** Parity      : None
** Stop Bit    : 1
** Flow Control: None
**
** 명령 사용법:
**	- download
**		xmodem down 0x80100000 0x8010FFFF : 사용자가 PC에서 지정한 파일을 0x80100000에서 0x8010FFFF까지
**										   저장하고 이 내용이 정확하면, Flash memory의 첫 영역에 저장한다.
*/

uint32_t DebugXmodemCmdHandler(CmdTokenType *pToken, int32_t lMatchIndex)
//void do_xmodem(int argc, char **argv)
{

	int	lRet;

	CLI();
	ulDebugMode = IALDEBUG_MODE_XMODEM;

	HalTimerStop();

//	SetState(&ePrevManagerState, &eManagerState, STATE_INIT);

	HalUartWrite((uint8_t *)"\r\n",2);
	lRet = xmodemReceive(dest, lSramSize);
	if(lRet > 0)
	{
		ulNewImgLen = lRet;
	}

	ulDebugMode = IALDEBUG_MODE_NORMAL;
	STI();

	return ulCmdResult;
}


#define SCALE	5

int _inbyte(unsigned short timeout) // msec timeout
{

	uint32_t	ulLoop, ulLSRReg;
	uint32_t	ulChar;
	int 		lRet;

	for(ulLoop=0;ulLoop<timeout*SCALE*10;ulLoop++)
	{
		ReadReg(HWREG_UART_LSR, &ulLSRReg);	

		if((ulLSRReg & HWREG_UART_LSR_DATA_RDY_MASK) == HWREG_UART_LSR_DATA_RDY_YES)
		{
			ReadReg(HWREG_UART_RBR, &ulChar);
			lRet = ulChar & 0xFF; 
			return lRet;
		}
		else
		{
			DelayUs(100/SCALE);
		}
	}

	return -1;

}

void _outbyte(int c)
{
	uint32_t	ulChar = (uint32_t)(c&0xFF);

	WriteReg(HWREG_UART_THR, ulChar);	

}








#include "Core/Core.h"
#include "Core/Crc.h"

#define SOH  0x01
#define STX  0x02
#define EOT  0x04
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define CTRLZ 0x1A

#define DLY_1S 1000
#define DLY_3S 3000
#define MAXRETRANS 25



static int check(int crc, const unsigned char *buf, int sz)
{

	if (crc) {
		unsigned short crc = Crc16Ccitt((uint8_t *)buf, sz);
		unsigned short tcrc = (buf[sz]<<8)+buf[sz+1];
		if (crc == tcrc)
			return 1;
	}
	else {
		int i;
		unsigned char cks = 0;
		for (i = 0; i < sz; ++i) {
			cks += buf[i];
		}
		if (cks == buf[sz])
		return 1;
	}

	return 0;
}

static void flushinput(void)
{
	while (_inbyte(((DLY_1S)*3)>>1) >= 0)
		;
}

int xmodemReceive(unsigned char *dest, int destsz)
{
	unsigned char xbuff[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */
	unsigned char *p;
	int bufsz, crc = 0;
	unsigned char trychar = 'C';
	unsigned char packetno = 1;
	int i, c, len = 0;
	int retry, retrans = MAXRETRANS;

	for(;;) {
		for( retry = 0; retry < 16; ++retry) {
			if (trychar) _outbyte(trychar);
			if ((c = _inbyte((DLY_3S)<<1)) >= 0) {
				switch (c) {
				case SOH:
					bufsz = 128;
					goto start_recv;
				case STX:
					bufsz = 1024;
					goto start_recv;
				case EOT:
					flushinput();
					_outbyte(ACK);
					return len; /* normal end */
				case CAN:
					if ((c = _inbyte(DLY_3S)) == CAN) {
						flushinput();
						_outbyte(ACK);
						return -1; /* canceled by remote */
					}
					break;
				default:
					break;
				}
			}
		}
		if (trychar == 'C') { trychar = NAK; continue; }
		flushinput();
		_outbyte(CAN);
		_outbyte(CAN);
		_outbyte(CAN);
		return -2; /* sync error */

	start_recv:
		if (trychar == 'C') crc = 1;
		trychar = 0;
		p = xbuff;
		*p++ = c;
		for (i = 0;  i < (bufsz+(crc?1:0)+3); ++i) {

			if ((c = _inbyte(DLY_3S)) < 0)
			{
				goto reject;
			}
			else
			{
				*p++ = c;
			}
		}

		if ((xbuff[1]&0XFF) == (unsigned char)((~xbuff[2])&0XFF) && 
			(xbuff[1] == packetno || xbuff[1] == (unsigned char)packetno-1) &&
			check(crc, &xbuff[3], bufsz)) {


			if (xbuff[1] == packetno)
			{
				register int count = destsz - len;
				if (count > bufsz) count = bufsz;
				if (count > 0) {
					memcpy (&dest[len], &xbuff[3], count);
					len += count;
				}
				if(packetno == 0xFF)
				{
					packetno = 0x0;
				}
				else
				{
					packetno++;
				}
				retrans = MAXRETRANS+1;
			}

			if (--retrans <= 0)
			{
				flushinput();
				_outbyte(CAN);
				_outbyte(CAN);
				_outbyte(CAN);
				return -3; /* too many retry error */
			}

			_outbyte(ACK);
			continue;
		}
	reject:
		flushinput();
		_outbyte(NAK);
	}
}




