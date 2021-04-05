
#ifndef OSDEPEND_H
#define OSDEPEND_H

#if (defined(VXWORKS))
#include    "errno.h"
#include    "taskLib.h"
#include    "ioLib.h"
#include    "sioLib.h"
#include    "inetLib.h"
#include    "sys/times.h"
#include    "msgQLib.h"
#include    "semLib.h"
#include    "sys\socket.h"
#include    "in.h"
#if ((BOARD_TYPE == MVME2100_BOARD)||(BOARD_TYPE == MVME3100_BOARD)||(BOARD_TYPE == MVME2500_BOARD))
#include    "ifLib.h"
#include    "sockLib.h"
#include    "routeLib.h"
#elif ((BOARD_TYPE == MVME4100_BOARD)||(BOARD_TYPE == LKV120_BOARD))
#include    "sockLib.h"
#endif
#include    "memLib.h"
#include    "signal.h"
#include    "wdLib.h"
#include    "fioLib.h"
#include    "selectLib.h"
#include    "logLib.h"
#include    "string.h"
#include    "sysLib.h"
#include    "rebootLib.h"
#include    "pingLib.h"
#include    "sysSymTbl.h"
#include    "errnoLib.h"
#include    "kernelLib.h"
#include    "intLib.h"
#include    "tickLib.h"

#ifdef MVME68K_BOARD
#include    <arch\mc68k\ivMc68k.h>
#endif

#if defined(BOARD_TYPE)
#include    <arch\ppc\ivPpc.h>
#include    <arch\ppc\archPpc.h>
#include	<arch\ppc\regsPpc.h>
#include    <arch\ppc\esfPpc.h>
#include 	<drv/pci/pciConfigLib.h>

#if (BOARD_TYPE == MVME3100_BOARD)
#include	"..\config\mpc8540\mpc8540.h"
#include	"..\config\mpc8540\xxx8540A.h"
#elif (BOARD_TYPE == MVME4100_BOARD)
#include	"..\config\mv4100\mv4100.h"
#elif (BOARD_TYPE == MVME2500_BOARD)
#include	"..\config\mvme2500\mvme2500.h"
#elif (BOARD_TYPE == LKV120_BOARD)
#include	"..\config\lk11_lkv_120\lkv120.h"
#endif 
#endif 


#include	<iv.h>
#include    <usrLib.h>
#include    <symLib.h>
#include    <objLib.h>
#include    <types\vxArch.h>
#include 	<private/taskLibP.h>
#include    <time.h>


#include	<aio.h>


#include	<asm.h>


#define TASK_SUPERVISOR_MODE    VX_SUPERVISOR_MODE
#define TASK_FP_MODE            VX_FP_TASK  
#define TASK_STDIO              VX_STDIO    

#define TASK_OPTION_DEFAULT     TASK_SUPERVISOR_MODE|\
                                TASK_FP_MODE|\
                                TASK_STDIO



#define MYIONREAD    FIONREAD       
#define MYIONWRITE   FIONWRITE      
#define MYIOFLUSH    FIOFLUSH       
#define MYIOOPTIONS  FIOOPTIONS       
#define MYIOBAUDRATE FIOBAUDRATE       
#define MYIONBIO     FIOBIO      
#define MYIOGETOPTIONS   FIOGETIOTONS      
#define MYIOSETOPTIONS   MYIOOPTIONS  
#define MYIORBUFSET  FIORBUFSET      
#define MYIOWBUFSET  FIOWBUFSET      
#define MYIORFLUSH   FIORFLUSH      
#define MYIOWFLUSH   FIOWFLUSH      


#define MYOPT_ECHO    OPT_ECHO        
#define MYOPT_CRMOD   OPT_CRMOD        
#define MYOPT_TANDEM  OPT_TANDEM        
#define MYOPT_7_BIT   OPT_7_BIT        

#define MYOPT_LINE    OPT_LINE        
#define MYOPT_ABORT   OPT_ABORT        
#define MYOPT_MON_TRAP    OPT_MON_TRAP        

#define MYOPT_TERMINAL    (MYOPT_ECHO | MYOPT_CRMOD | MYOPT_7_BIT | MYOPT_LINE)

#define MYCON_BAUDRATE  CONSOLE_BAUD_RATE


#define S_DEV_BAUDRATE_SET	SIO_BAUD_SET
#define	S_DEV_BAUDRATE_GET	SIO_BAUD_GET

#define S_DEV_HW_OPTS_SET	SIO_HW_OPTS_SET
#define S_DEV_HW_OPTS_GET	SIO_HW_OPTS_GET
#define S_DEV_MODE_SET		SIO_MODE_SET
#define S_DEV_MODE_GET		SIO_MODE_GET

#define IGNORE_MODEM_STATUS	CLOCAL	
#define ENABLE_DEV_RECV		CREAD	

#define DATABIT5	CS5		
#define DATABIT6	CS6		
#define DATABIT7	CS7		
#define DATABIT8	CS8		

#define STOPBIT1	0x0		
							
#define STOPBIT2	STOPB	

#define	NOPARITY	0x0
#define	EVENPARITY	PARENB	
							
#define ODDPARITY	(PARENB|PARODD)	
									

#define	S_POLL_MODE		SIO_MODE_POLL
#define	S_INTR_MODE		SIO_MODE_INT


#define MYMSG_Q_FIFO      MSG_Q_FIFO        
#define MYMSG_Q_PRIORITY  MSG_Q_PRIORITY    


#define MYMSG_PRI_NORMAL    MSG_PRI_NORMAL     
#define MYMSG_PRI_URGENT    MSG_PRI_URGENT     


#ifndef NOWAIT
    #define NOWAIT  NO_WAIT 
#endif
#ifndef FOREVERWAIT
    #define FOREVERWAIT WAIT_FOREVER
#endif


#define MYSEM_Q_FIFO        SEM_Q_FIFO          
#define MYSEM_Q_PRIORITY    SEM_Q_PRIORITY      
#define MYSEM_DELETE_SAFE   SEM_DELETE_SAFE     
#define MYSEM_INVERSION_SAFE     SEM_INVERSION_SAFE  

#define sysMsgLogSet    logMsg

#define memoryCopy32   fourBytesDataCopy            

#elif (defined(_WIN32))
#define TASK_OPTION_DEFAULT		0x00
#define MYSEM_Q_FIFO			0x00     
#define MYSEM_Q_PRIORITY		0x00     
#define MYSEM_DELETE_SAFE		0x00     
#define MYSEM_INVERSION_SAFE    0x00	 
#define MYMSG_PRI_NORMAL		0x00     
#define MYMSG_PRI_URGENT		0x00     
#define MYMSG_Q_FIFO			0x00     
#define MYMSG_Q_PRIORITY		0x00     

#define MSG_PRI_NORMAL			0x00 
#define SEM_EMPTY				0x00 
#define SEM_FULL				0x01
#define BOOT_COLD				0x00 
#define NOWAIT		0
#define FOREVERWAIT	0xFFFFFFFF

#define VER_LENTH 8 
#define WATCHDOG_SEC 3

#define S_DEV_BAUDRATE_SET	0
#define	S_DEV_BAUDRATE_GET	0

#define S_DEV_HW_OPTS_SET	0
#define S_DEV_HW_OPTS_GET	0
#define S_DEV_MODE_SET		0
#define S_DEV_MODE_GET		0

#define SIO_MODE_GET	0
#define SIO_HW_OPTS_GET	0

#define	S_INTR_MODE	0

#define IGNORE_MODEM_STATUS 0
#define ENABLE_DEV_RECV	0

#define DATABIT5	5
#define DATABIT6	6
#define DATABIT7	7
#define DATABIT8	8

#define STOPBIT1	0x0		
#define STOPBIT2	0x1		

#define MYIONREAD	 0
#define MYIONWRITE	 0
#define MYIORFLUSH   0
#define MYIOWFLUSH   0
#endif 

#endif 










