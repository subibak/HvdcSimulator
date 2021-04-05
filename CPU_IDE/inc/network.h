
#ifndef NETWORK_H
#define NETWORK_H

typedef struct {
    int16   type;
    uint16  port;
    uint32  iAddr;
    int32   sockFd;
    uint8    unused[4];
}strinetAddr;



#define	UNI_EWS_UDP_RECV_PORT_A				(11001)
#define	UNI_EWS_UDP_RECV_PORT_B				(12001)
#define	UNI_EWS_UDP_SEND_PORT_A				(11101)
#define	UNI_EWS_UDP_SEND_PORT_B				(12101)

#define BROAD_EWS_SEND_RECV_PORT_A			(11201)
#define BROAD_EWS_SEND_RECV_PORT_B			(12201)


#define	UNI_WDC_UDP_RECV_PORT_A				(11000)
#define	UNI_WDC_UDP_RECV_PORT_B				(12000)
#define	UNI_WDC_UDP_SEND_PORT_A				(11100)
#define	UNI_WDC_UDP_SEND_PORT_B				(12100)



#define	UNI_HST_UDP_RECV_PORT_A				(11200)
#define	UNI_HST_UDP_RECV_PORT_B				(12200)
#define	UNI_HST_UDP_SEND_PORT_A				(11300)
#define	UNI_HST_UDP_SEND_PORT_B				(12300)


#define	BROAD_UDP_SEND_RECV_PORT_A			(11400)
#define	BROAD_UDP_SEND_RECV_PORT_B			(12400)


#define	UNI_DUAL_CHK_UDP_SEND_RECV_PORT_A		(11600)
#define	UNI_DUAL_CHK_UDP_SEND_RECV_PORT_B		(12600)


#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)
	#define UNI_RMT_UDP_RECV_PORT				(11710)
	#define UNI_LIV_UDP_RECV_PORT				(11720)
	#define UNI_READ_UDP_RECV_PORT				(11730)
	#define UNI_RMT_UDP_SEND_PORT				(11740)
#endif


/* P3000AT¿Í Åë½Å */
#define UNI_UDP_SEND_RECV_PORT_A		(11500)
#define UNI_UDP_SEND_RECV_PORT_B		(12500)
#define UNI_MODBUS_TCP_SEND_RECV_PORT		(502) 

#define IP_ADDR_LEN 	(4)
#define MAX_IP_ADDR_STR_LEN 	(15)


 
#define SOCK_ADDR_LENGTH    ((int32)sizeof(struct sockaddr))


#ifdef _WIN32
#define MAX_SOCK_BUF_SIZE   (1024 * 200) 
#else
#define MAX_SOCK_BUF_SIZE   (1024 * 200) 
#endif


#define TCP_PROTO   (1)
#define UDP_PROTO   (2)
#ifdef _WIN32
#define UDP_UNICASTING (3)
#endif


#define LISTEN_NUM  32


#define SENDBUF_SIZE_OPT        (0x1)
#define RECVBUF_SIZE_OPT        (0x2)
#define DATA_BROADCAST_OPT      (0x3)



#define MAX_SOCKET_FD_NUM   (64)



#define MAX_RECV_SOCK_NUM   (20)  
#define MINI_RECV_SOCK_NUM  (10) 


#ifdef _WIN32

	#define	NET_DEV_NAME		"Comm\\PCI\\E100CE1\\Parms\\TCPIP"
	#define	NET_DEV_NAME0		"PCI\\E100CE1"
#ifdef _WINCE
	#define	DUAL_NET_DEV_NAME	"Comm\\PCI\\E100CE2\\Parms\\TCPIP"
#endif
	#define	DUAL_NET_DEV_NAME0	"PCI\\E100CE2"

	#define	DLU_NET_DEV_NAME	"Comm\\PCI\\E100CE3\\Parms\\TCPIP"
	#define	DLU_NET_DEV_NAME0	"PCI\\E100CE3"
	#define	DLU_NET_DEV_ADDR	"192.9.211.188"
#else

#if (BOARD_TYPE == MVME2100_BOARD)
	#define	NET_DEV_NAME		"dc"
	#define	NET_DEV_NAME0		"dc0"

#elif (BOARD_TYPE == MVME3100_BOARD)
	
	#define	NET_DEV_NAME		"mottsec"
	#define	NET_DEV_NAME0		"mottsec0"
	
#ifdef	INCLUDE_TSEC_END 
	
	#define	NET_DEV_NAME	"mottsec"
	
	
	
	#if NOT_USED
	#define	NET_DEV_NAME1	"mottsec1"
	#endif
#endif



#ifdef	INCLUDE_FEC_END 
	
	#define	DUAL_NET_DEV1_NAME	"motfcc"
	#define	DUAL_NET_DEV1_NAME0	"motfcc1"
#endif

#elif ((BOARD_TYPE == MVME4100_BOARD)||(BOARD_TYPE == MVME2500_BOARD)||(BOARD_TYPE == LKV120_BOARD))
	
	#define	NET_DEV_NAME		"motetsec"
	#define	NET_DEV_NAME0		"motetsec0"
	
#endif 


#define	PMC661J_END	1
#define	PMC661J_DEV_NAME	"RX"

#define	PMC661J_VENDOR_ID		0x1011
#define	PMC661J_DEVICE_ID		0x19


#define	PMC682E_END	2
#define	PMC682E_DEV_NAME	"rx"

#define	PMC682E_VENDOR_ID		0x8086
#define	PMC682E_DEVICE_ID		0x1209


#define	PMC676TX_END	3
#define	PMC676TX_DEV_NAME	"rxg"

#define	PMC676TX_VENDOR_ID		0x8086
#define	PMC676TX_DEVICE_ID		0x1010
#define	PMC676RCTX_DEVICE_ID	0x1079	

#endif 


#define	MAX_IP_A_CLASS_NUM		(126)
#define	MAX_IP_B_CLASS_NUM		(191)
#define	MAX_IP_C_CLASS_NUM		(223)   

#define  NO_PRINT		0x0
#define  MINTIME_FLAG	0x1 
#define  PRINT_FLAG		0x2 

#ifdef _WIN32
#define MAXADAPTERS		20
#define ADAPTERNUMBER	0
#define MAXIPS			200

#define	IP_ADDR(a,b,c,d) ((d&0xFF)<<24)|((c&0xFF)<<16)|((b&0xFF)<<8)|(a&0xFF)


typedef struct ip_z {
	char ip_address[16];
	char sub_netmask[16];
} ip_entry, *pip_entry;


#define  SOCKET_BINDING	(1)
#define  NOT_BINDING	(0)


#define	WORD_MAKE(highByte, LowByte)((highByte << 8)|LowByte)
#define	GET_LOBYTE(wordData)(wordData&0xff)
#define	GET_HIBYTE(wordData)(wordData>>8)
#endif



#define TCP_SEND_CONNECT_TIMEOUT			250 
#define SOCKET_ADDR_SIZE					sizeof(struct sockaddr)
#define	MAX_UNICAST_RECV_SOCKET_BUF_SIZE	(1024*256)
#define NON_BLOCKING_ENABLE					(1)
#define NON_BLOCKING_DISABLE				(0)

#endif 
