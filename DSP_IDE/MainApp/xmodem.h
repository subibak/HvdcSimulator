/*

 */
#ifndef _XMODEM_H_
#define _XMODEM_H_

//#include "Debug/DebugCmdTable.h"

#include "DebugApp/DebugApp.h"

#if 1
#include "Hal/HalUart.h"
#include "Core/Crc.h"



#define	DEF_NCS_BOOT_FLASH_APP_SIZE			0x80000		// 512K
#define	DEF_NCS_NOR_FLASH_APP_SIZE			0x180000	// 1.5M
#define DEF_NCS_APP_SIZE					DEF_NCS_NOR_FLASH_APP_SIZE



#define		CON_BUF_SIZE			1024
/* buffer size = 1030bytes. 1024 for XModem 1k + 3 head chars + 2 crc + nul */
#define XMODEM_BUF_SIZE		1036	// 4배수로 정렬

#define	XMODEM_OVER_DATA	0x12345678



typedef unsigned int	uint;
typedef unsigned char	uchar;




typedef struct {
	unsigned char	xm_rcv_buffer[CON_BUF_SIZE];
	unsigned char	xm_proc_buffer[XMODEM_BUF_SIZE];
	unsigned char	xm_sw_buffer[DEF_NCS_APP_SIZE];
	unsigned char	over_data;
} XMODEM_BUF;

extern int		xm_download (uint, uint);
extern void		do_xmodem (int argc, char **argv);



int xmodemReceive(unsigned char *dest, int destsz);


extern uint		run_Xmodem;
#endif
#endif

