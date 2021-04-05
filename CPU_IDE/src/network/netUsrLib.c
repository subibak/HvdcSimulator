
#include	<stdio.h>
#include	<string.h>

#include    "usrTypes.h"
#include	"usrDef.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include	"sysConfig.h"
#include    "netProtocol.h"
#include	"network.h"
#include	"msgQueData.h"
#include	"lkv120Board.h"
#include    "usrQueData.h"
#include    "osDepend.h"
#include	"usrErrCode.h"

#include	"segScheduler.h"


#include	"protocolBuf.h"
#include	"rmt_Comm/ethModbus.h"
#include	"rmt_Comm/rmtDevUtil.h"

#ifdef VXWORKS
#include <wrn/coreip/netinet/tcp.h>
#endif


#define CLIENT_CONNECT_TIMEOUT_250MSEC		(250)			


static int8   NET_ADDR_A_LINE[IP_ADDR_LEN] = {(int8)192,(int8)9,(int8)211,(int8)0};
static int8   NET_ADDR_B_LINE[IP_ADDR_LEN] = {(int8)192,(int8)9,(int8)212,(int8)0};
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
static int8   DLU_ADDR_LINE[IP_ADDR_LEN]  = {(int8)188,(int8)9,(int8)211,(int8)0};
#endif


extern int32 gDebugPortFd;


uint32 connectWithTimeout1(int32 socketFd, struct sockaddr *sockAddr, uint32 len, uint32 mSec, uint32 *sockErrPtr);
uint32	routeTblInfoUpdate(uint32, strRouteTblInfo *);
uint32	ipAddrMake(uint8 *, uint32, uint32, uint32);
uint32	ipAddrSet(int8 *, int8 *);
uint32	ipAddrGet(int8 *, int8 *);
uint32	etherNetAddrGet(uint8 *,uint32);
int32	socketOptionSet(  int32 ,int32 ,void *,int32 );
void	ipAddrIntToString(uint8 *, uint8 *);
void	ipAddrStrToHex (uint32 *, uint8 *);
void	ipAddrHexToString(int8 *, uint32 );
int32	addrInfoBuild(void *,uint32 *,uint16 );
uint32	gatewayAdd(int8 *, int8 *);

int32	connectionEstablishInServer(void *, int16 , int32 *);
int32	connectionAcceptInServer(int32 , void *, int32 *);
int32	connectionEstablishInClient(void *, int16 , int32 *);
int32	dataSendToServerOrClient(   strDataBuf *,void *,uint32 );

int32	dataRecvFromServerOrClient( strDataBuf  *, void *,uint32 );
#ifdef _WIN32
int32 tcpRecvFromModbusDevice(	strDataBuf  *,void *,uint32 );
#endif

uint32	portInfoTableInit(void);
uint32	portInfoSet( strMsgQueData  *);
uint32	portInfoGet(strinetAddr  *,strNewNetProtocolInfo *);
uint32	ipAddrValidCheck(uint8 *);
void	routeInfoShow(uint32, uint32);
void	routeInfoDelete(uint32, uint32);
void	routeInfoAdd(uint32, uint32);
#ifdef _WIN32
void	bzero(char *buffer, int nbytes);
#endif

extern int32	ioRead(int32 , int8 *, int32 );
extern void		memoryClear ( uint8 *, uint32 );
extern void		spaceSkip(int8 **);
extern uint32	routeTblInfoDelete( int8 *, int8 *);
extern uint32	routeTblInfoAdd( int8 *, int8 *);
extern void		memoryCopy ( uint8 *, uint8 *, uint32 );
extern uint32	nvRamWrite(int8 *, int32 , int32 );
extern uint32	nvRamRead(int8 *, int32 , int32 );

#ifdef _WIN32
extern uint32 ifAddrSet(int8 *, int8 *);
extern uint32 routeAdd(int8 *, int8 *);
extern void   routeTblShow(uint32, strRouteTblInfo*);
#else
#if ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD))
	
	extern int32   sysMpc8540EndGetMacAddr (uint8 *, uint32);
#elif (BOARD_TYPE == LKV120_BOARD)
    extern int32 sysGetMac(int unit, char *str);
#endif
#endif 
extern uint32	systemCnfgInfoRead(strSysConfigInfo *);
extern void		setErrorCodeWithVal(uint8 *, uint32, uint8 *, uint32,
					uint8   *, uint32,uint8 *, uint32,uint8 *, uint32);
extern int32   ioSysCtrlFunc(int32 , int32 , int32 );


uint32	routeTblInfoUpdate(uint32 routeIndex, strRouteTblInfo *dataPtr)
{
	uint32	status = NO_ERROR;
	uint32	i, routeNum = 0;
	int8	destIpAddr[32], gatewayIpAddr[32];

    for(i = 0; i < MAX_ROUTE_NUM; i++,dataPtr++) {
		if( (routeIndex >> i) & 0x1 ) {
			memoryClear( (uint8 *) destIpAddr, 32);
			ipAddrHexToString(	destIpAddr, 
								dataPtr->destIp
							  );
			memoryClear( (uint8 *) gatewayIpAddr, 32);
			ipAddrHexToString(	gatewayIpAddr, 
								dataPtr->gatewayIp
							  );
			if(routeTblInfoAdd(destIpAddr, gatewayIpAddr)) {
				printf( "Route Add Error !!!!\n"); 
			}
			routeNum += 1;
        }
    }
	printf( "Route Table Info Update Num [%d] !!\n", routeNum);
    return(status);
}


void ipAddrHexToString(int8 *iPtr, uint32 hexVal)
{
    uint8   tmpVal, tmpArr[4];
    int32   i;

    for(i = 0; i < 4; i++) {
        tmpVal = (uint8) ((hexVal >> ((3-i)*8)) & 0xFF);
        tmpArr[i] = tmpVal;
    }
#ifdef VXWORKS
    sprintf((char *)iPtr, "%d.%d.%d.%d",	tmpArr[0],
											tmpArr[1],
											tmpArr[2],
											tmpArr[3]);
#else
	 
    sprintf((char *)iPtr, "%d.%d.%d.%d",	tmpArr[3],
											tmpArr[2],
											tmpArr[1],
											tmpArr[0]);
#endif 

}


void ipAddrStrToHex (uint32 *destHexPtr, uint8 *ipStr)
{
    uint32  hexData;

    hexData = inet_addr((char*)ipStr);

    *destHexPtr = hexData;
}

uint32 ipAddrMake(uint8 *ipAddr, uint32 stationId, uint32 lineKind, uint32 shelfType)
{
    uint32    status = NO_ERROR;
    uint8   *ipPtr = NULL;
	uint32	offSet;

    
    if(shelfType == MASTER_SHELF)
    	offSet = 1;
	else offSet = 0;

    
    if( (stationId >= MIN_STATION_ID) && (stationId <= MAX_STATION_ID)){

        if(lineKind == NETWORK_A_LINE){
            NET_ADDR_A_LINE[IP_ADDR_LEN-1] = (stationId * 2) - offSet;
            ipPtr = (uint8 *)NET_ADDR_A_LINE;
        }
        else if(lineKind == NETWORK_B_LINE) {
            NET_ADDR_B_LINE[IP_ADDR_LEN-1] = (stationId * 2) - offSet;
            ipPtr = (uint8 *)NET_ADDR_B_LINE;
        }
#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)
        else if(lineKind == DLU_SYS_LINE) {
            DLU_ADDR_LINE[IP_ADDR_LEN-1] = (stationId * 2) - offSet;
            ipPtr = (uint8 *)DLU_ADDR_LINE;
        }                
        else if(lineKind == DLU_RMT_LINE) {
            DLU_ADDR_LINE[IP_ADDR_LEN-1] = (stationId * 2) - (1 - offSet);    
            ipPtr = (uint8 *)DLU_ADDR_LINE;
        }
#endif
        else status = NETWORK_LINE_KIND_ERR;

        if(status == NO_ERROR)
            ipAddrIntToString(ipAddr, ipPtr);
    }
    else{
printf("[STATION_ID_RANGE_ERR]%d\n",stationId);
        status = STATION_ID_RANGE_ERR;
    }

    return(status);
}


uint32 ipAddrSet(int8 *devName, int8 *ipStr)
{
	uint32	status = NO_ERROR;

#if (defined(VXWORKS)||defined(_WIN32))
    status = (uint32)ifAddrSet(devName, ipStr);
    if(status)
    	status = IP_ADDR_SET_ERR;
#endif
    return(status);
}


uint32 ipAddrGet(int8 *devName, int8 *ipAddrPtr)
{
    uint32 status = NO_ERROR;
#ifdef VXWORKS

    if(ifAddrGet(devName, ipAddrPtr) < 0)
        status = IP_ADDR_GET_ERR;
#endif
    return(status);
}


uint32 etherNetAddrGet(uint8 *macAddrPtr, uint32 index)
{
    uint32 status = NO_ERROR;
    
#if ((BOARD_TYPE == MVME2100_BOARD)||(BOARD_TYPE == MVME2500_BOARD)||(BOARD_TYPE == X86_PC_BOARD))
    *macAddrPtr++ = (uint8)ENET_ADDR[0];
    *macAddrPtr++ = (uint8)ENET_ADDR[1];
    *macAddrPtr++ = (uint8)ENET_ADDR[2];
    *macAddrPtr++ = (uint8)ENET_ADDR[3];
    *macAddrPtr++ = (uint8)ENET_ADDR[4];
    *macAddrPtr   = (uint8)ENET_ADDR[5];

#elif (BOARD_TYPE == MVME3100_BOARD)
#ifdef VXWORKS
    status = (uint32)sysMpc8540EndGetMacAddr(macAddrPtr, index);
#endif
#elif (BOARD_TYPE == MVME4100_BOARD)
#ifdef VXWORKS
    status = (uint32)sysNetMacNVRamAddrGet("motetsec",index,macAddrPtr,6);
#endif
#elif (BOARD_TYPE == LKV120_BOARD)
#ifdef VXWORKS
    status = (uint32)sysGetMac(index,macAddrPtr);
#endif
#endif

    return(status);
}


int32 addrInfoBuild(void *data_ptr,uint32 *ip_addr,uint16 port_num)
{
    int32 status = NO_ERROR;

    struct sockaddr_in *info_ptr = (struct sockaddr_in *)data_ptr;

    
    info_ptr->sin_family = AF_INET;

    
    info_ptr->sin_port = htons(port_num);

    
    if(ip_addr == NULL) {
        info_ptr->sin_addr.s_addr = INADDR_ANY;
	}

    else {
        info_ptr->sin_addr.s_addr = *ip_addr; 
	}

    return(status);
}


void ipAddrIntToString(uint8 *dest_ptr, uint8 *src_ptr)
{
    sprintf((char *)dest_ptr, "%d.%d.%d.%d",    src_ptr[0],
												src_ptr[1],
												src_ptr[2],
												src_ptr[3]);
}




int32 socketOptionSet(  int32 sockFd,
                        int32 option,
                        void *optionVal,
                        int32 optionLen)
{
    int32 status = NO_ERROR;
    int32 sockOpt = 0;

    
    if(sockFd < 0)
        return(SOCK_FD_ERR);

    switch(option) {
        case SENDBUF_SIZE_OPT: 
            sockOpt = SO_SNDBUF;
        break;
        case RECVBUF_SIZE_OPT: 
            sockOpt = SO_RCVBUF;
        break;
        case DATA_BROADCAST_OPT: 
            sockOpt = SO_BROADCAST;
        break;
        default:
            status = SOCK_PARA_ERR;
        break;
    }
    if(status == NO_ERROR) {
        status = setsockopt(    sockFd,
                                SOL_SOCKET,
                                sockOpt,
								(int8 *)optionVal,
                                optionLen);
        if(status)
            status = SOCK_OPTSET_ERR;
    }

    return(status);
}


uint32 gatewayAdd(int8 *netGrpAddr, int8 *gateway)
{
    uint32 status = NO_ERROR;
    if(routeAdd(netGrpAddr, gateway))
        status = GATEWAY_ADD_ERR;
    return(status);
}


int32 connectionEstablishInServer(void *local, int16 proto_type, int32 *fd_ptr)
{
    int32   status = NO_ERROR;

    int32  socket_fd;
    struct sockaddr_in  *local_addr = (struct sockaddr_in *)local;

    
    *fd_ptr = 0;

    switch(proto_type) {
        case TCP_PROTO:
            
            if( 
				(socket_fd = socket(PF_INET, SOCK_STREAM, NOT_USED)) == 
				-1){
                
                status = SOCK_OPEN_ERR;
				printf( "TCP_SVR : SOCKET OPEN ERROR!!!\n");

            }

            
            else if(
				bind(socket_fd,(struct sockaddr *)local_addr,SOCK_ADDR_LENGTH) ==-1) {
                
#ifdef _WIN32
                closesocket(socket_fd);
#else
				close(socket_fd);
#endif 
                
                status = SOCK_BIND_ERR;
				printf( "TCP : SOCKET BIND ERROR!!!\n");
            }
            
            else {
                status = listen(socket_fd, LISTEN_NUM);
                if(status) {
                    
                    status = SOCK_LISTEN_ERR;
					printf( "TCP : SOCKET LISTEN ERROR!!!\n");
           		}
            }
            
            *fd_ptr = socket_fd;
        break;
        case UDP_PROTO:
            
            if( 
				(socket_fd = socket(PF_INET, SOCK_DGRAM, NOT_USED)) == 
				-1){
                
                status = SOCK_OPEN_ERR;
				printf( "UDP : SOCKET OPEN ERROR!!!\n");
            }
            
            else {
                status = bind( socket_fd,
                          (struct sockaddr *)local_addr,
                          SOCK_ADDR_LENGTH);
                if(status){
                    
#ifdef _WIN32
		            closesocket(socket_fd);
#else
					close(socket_fd);
#endif 
                    
                    status = SOCK_BIND_ERR;
					printf( "UDP : SOCKET BIND ERROR!!!\n");
                }
            }
            
            *fd_ptr = socket_fd;
        break;
        default:
            status = ETH_PROT_TYPE_ERR;
        break;
    }

    return(status);
}


int32 connectionAcceptInServer(int32 old_fd, void *peer, int32 *fd_ptr)
{
    int32 status = NO_ERROR;

    int32 new_fd;
	int32 addrLen = SOCK_ADDR_LENGTH;

    
    *fd_ptr = 0;

    new_fd = accept(old_fd,
                    (struct sockaddr *)peer,
                    (int32 *) &addrLen
                    );
	if(new_fd == -1)     
	   status = SOCK_ACCEPT_ERR;
     else
        *fd_ptr = new_fd;

    return(status);
}

int32 connectionEstablishInClient(void *peer, int16 proto_type, int32 *fd_ptr)
{
    int status = NO_ERROR;

    int32   socket_fd;
	int32	ret_connect, ret_close, nonblocking;
    struct sockaddr_in  *serv_addr = (struct sockaddr_in *)peer;
	uint32 sockErrNo=0;
	int32 errNo = 0;
	uint32 localAddr = 0;
	uint32 option = 0;

    
    *fd_ptr = 0;

    switch(proto_type) 
	{
        case TCP_PROTO:
             
			socket_fd = socket(PF_INET, SOCK_STREAM, NOT_USED);
            if(socket_fd == -1) 
			{
                
				printf( "TCP : SOCKET OPEN ERROR!!!\n");
                status = SOCK_OPEN_ERR;
            }
            else 
			{
				#if 111	
				ret_connect = connectWithTimeout1(	socket_fd,
													(struct sockaddr *)serv_addr,
													SOCK_ADDR_LENGTH,
													CLIENT_CONNECT_TIMEOUT_250MSEC,
													&sockErrNo
													);
				#else
				ret_connect = connect ( socket_fd,
										(struct sockaddr *)serv_addr,
										SOCK_ADDR_LENGTH
										);
				#endif
				if(ret_connect != NO_ERROR) 
				{
					status = SOCK_CONNECT_ERR;

					if (socket_fd > 0 )
					{
						#ifdef VXWORKS
						ret_close = close(socket_fd);
						#else
						ret_close = closesocket(socket_fd);
						#endif
						if (ret_close < 0) 
						{
							printf("close error\n");
						}
						else
						{
							
							socket_fd = -1;  
							*fd_ptr = socket_fd;
						}           
					}

					
				}
				else
				{
					#if 111
					
					option = 1;
					if( setsockopt( socket_fd, IPPROTO_TCP,
									TCP_NODELAY,(int8 *)&option,
									sizeof(option)) < 0 ) 
					{
	  					status = SOCK_OPTSET_ERR;
						printf( "TCP : SOCKET SET OPTION (TCP_NODELAY) ERROR!!!\n");

						if (socket_fd > 0 )
						{
							#ifdef VXWORKS
							ret_close = close(socket_fd);
							#else
							ret_close = closesocket(socket_fd);
							#endif
				
							if (ret_close < 0) 
							{
								printf("close error\n");
							}
							else
							{
								socket_fd = -1;  
							}           
						}
					}
					#endif

					*fd_ptr = socket_fd;
				}
			}

        break;
        case UDP_PROTO:
            if((socket_fd = socket(PF_INET, SOCK_DGRAM, NOT_USED)) ==  -1) {
                
				printf( "UDP : SOCKET OPEN ERROR!!!\n");
                status = SOCK_OPEN_ERR;
            }
            
            else 
			{
                status = bind(socket_fd,(struct sockaddr *)serv_addr,SOCK_ADDR_LENGTH);
				
                if(status) 
				{
					#ifdef _WIN32
					errNo = WSAGetLastError();
					#endif 

					
                    
#ifdef _WIN32
		            closesocket(socket_fd);
#else
					close(socket_fd);
#endif 
                    
                    status = SOCK_BIND_ERR;
					
					memcpy(&localAddr, &serv_addr->sin_addr, sizeof(int));
					setErrorCodeWithVal(	__FILE__,__LINE__, 
											"connectionEstablishInClient",status,
											"UDP : SOCKET BIND ERROR", 0,
											"IP ADDR", localAddr,
											"Not Using", 0 );

					
					
					
					
					
					#ifdef _WIN32
					if(errNo == WSAEADDRNOTAVAIL)
					{
						status = NO_ERROR;
					}
					#endif 
                }
                else {
                    *fd_ptr = socket_fd;
				}
            }
        break;

#ifdef _WIN32
#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)
		case UDP_UNICASTING:

		    if((socket_fd = socket(PF_INET, SOCK_DGRAM, NOT_USED)) ==  -1) {
                
				printf( "UDP : SOCKET OPEN ERROR!!!\n");
                status = SOCK_OPEN_ERR;
            }
			*fd_ptr = socket_fd;

        break;
#endif 
#endif
        default:
            status = ETH_PROT_TYPE_ERR;
        break;
    }

    return(status);
}


int32 dataSendToServerOrClient(   strDataBuf *data_ptr,
                                  void *addrPtr,
                                  uint32 proto_type)
{
    int32 status = NO_ERROR;
    int32   ret_val = NO_ERROR;
	strinetAddr	*dest_addr = (strinetAddr *)addrPtr;

    
    if(data_ptr->socket_fd <= 0)
        return(SOCK_FD_ERR);

    if(data_ptr->data_len <= 0)
        return(DATA_LEN_ERR);

    switch(proto_type) {
        case TCP_PROTO:
            
            ret_val= send(   data_ptr->socket_fd,
                             (char *)data_ptr->data_buf,
                             data_ptr->data_len,
#if (BOARD_TYPE == LKV120_BOARD)
                            MSG_DONTWAIT
#else
                            NOT_USED
#endif
                          );
        break;
        case UDP_PROTO:
            
            ret_val = sendto (  data_ptr->socket_fd,
                                (char *)data_ptr->data_buf,
                                data_ptr->data_len,
#if (BOARD_TYPE == LKV120_BOARD)
                            MSG_DONTWAIT,
#else
                            NOT_USED,
#endif
                                (struct sockaddr *)dest_addr,
                                SOCK_ADDR_LENGTH
                              );	
        break;
        default:
            status = ETH_PROT_TYPE_ERR;
        break;
    }

    
    if(status == NO_ERROR) {
        if(ret_val == -1)
        	status = SOCK_SEND_ERR;
      
        else if(ret_val != (int32)data_ptr->data_len){
            status = SOCK_SEND_LEN_ERR;
			setErrorCodeWithVal (	__FILE__,
									__LINE__,
									"dataSendToServerOrClient",
									status,
									"Send Sock Fd", data_ptr->socket_fd,
									"Send Data Len", data_ptr->data_len,
									"Ret Data Len", ret_val
								  );	            
        }
    }
    return(status);
}


int32   dataRecvFromServerOrClient( strDataBuf  *data_ptr,
                                    void *src_addr,
                                    uint32 proto_type)
{
    int32 status = NO_ERROR;
    int32 ret_val = NO_ERROR;
	int32 addrLen = SOCK_ADDR_LENGTH;

    
    if((data_ptr->socket_fd < 0))
        return (SOCK_FD_ERR);

    switch(proto_type) {
        case TCP_PROTO:
           
			
            ret_val = recv( data_ptr->socket_fd,
                            (char *)data_ptr->data_buf,
#ifdef _WIN32
                            data_ptr->data_len, 
#else
                            MAX_DATA_LEN,
#endif

#if (BOARD_TYPE == LKV120_BOARD)
                            MSG_DONTWAIT
#else
                            NOT_USED
#endif
                            );
        break;
        case UDP_PROTO:
            
            ret_val = recvfrom(  data_ptr->socket_fd,
                                (char *)data_ptr->data_buf,
                                MAX_DATA_LEN,
#if (BOARD_TYPE == LKV120_BOARD)
                                MSG_DONTWAIT,           
#else
                                  NOT_USED,	
#endif
                                src_addr,
                                (int32 *)&addrLen
                              );
        break;
        default:
            status = ETH_PROT_TYPE_ERR;
        break;
    }

    if(status == NO_ERROR) {
        if(ret_val == -1) {
            
            status = SOCK_RECV_ERR;
		}
        else {
            data_ptr->data_len = ret_val;
		}
    }
    return(status);
}


#ifdef _WIN32

int32 tcpRecvFromModbusDevice(	strDataBuf  *data_ptr,
								void *src_addr,
								uint32 proto_type)
{
	int32 status = NO_ERROR;
	int32 ret_val = NO_ERROR;
	int32 addrLen = SOCK_ADDR_LENGTH;
	strDataBuf recvData;
	uint16 dataframeLength = 0;
	uint32 tepm_data_len = 0;

	
	if((data_ptr->socket_fd < 0))
		return (SOCK_FD_ERR);

	 
	 
	 
	memoryClear((uint8*)&recvData,sizeof(strDataBuf));

	
	recvData.data_len = MODBUS_TCP_LENGTH_OFFSET_SIZE;
	
	recvData.socket_fd = data_ptr->socket_fd;

	status = dataRecvFromServerOrClient(&recvData,
										(void *)src_addr,
										proto_type
										);
	if(status)
	{
		setErrorCode( __FILE__,   __LINE__,
						(uint8 *)"dataReadAndSendToDualProcTask", status);
		return(status);
	}
	else 
	{
		if(recvData.data_len != MODBUS_TCP_LENGTH_OFFSET_SIZE)
		{
			status = SOCK_RECV_ERR;
			setErrorCode( __FILE__,   __LINE__,
							(uint8 *)"dataReadAndSendToDualProcTask", status);
			return(status);
		}
	
	}

	
	memoryCopy(	(uint8*)data_ptr->data_buf,
				(uint8*)recvData.data_buf,
				recvData.data_len);

	data_ptr->data_len = recvData.data_len;

	 
	 
	 
	
#ifdef VXWORKS
	memoryCopy((uint8 *)&dataframeLength, (uint8 *)&recvData.data_buf[4], 2);
#else
	COPYS((uint8 *)&dataframeLength, (uint8 *)&recvData.data_buf[4]);
#endif

	if(dataframeLength > (MODBUS_DATA_VALUE_LENGTH - MODBUS_TCP_LENGTH_OFFSET_SIZE) ) 
	{
		status = MSG_LEN_EXCEED_ERR;
		setErrorCode( __FILE__,   __LINE__,
						(uint8 *)"dataReadAndSendToDualProcTask", status);
		return(status);
	}

	 
	 
	
	memoryClear((uint8*)&recvData,sizeof(strDataBuf));

	
	recvData.data_len = dataframeLength;
	
	recvData.socket_fd = data_ptr->socket_fd;

	status = dataRecvFromServerOrClient(&recvData,
										(void *)src_addr,
										proto_type
										);

	if(status)
	{
		setErrorCode( __FILE__,   __LINE__,
						(uint8 *)"dataReadAndSendToDualProcTask", status);
		return(status);
	}

	
	if(recvData.data_len < dataframeLength)
	{
		
		memoryCopy(	(uint8*)&data_ptr->data_buf[MODBUS_TCP_LENGTH_OFFSET_SIZE],
					(uint8*)recvData.data_buf,
					recvData.data_len);

		data_ptr->data_len += recvData.data_len;

		 
		 
		 
		tepm_data_len = recvData.data_len; 
		memoryClear((uint8*)&recvData,sizeof(strDataBuf));

		
		recvData.data_len = dataframeLength - tepm_data_len;
		
		recvData.socket_fd = data_ptr->socket_fd;

		status = dataRecvFromServerOrClient(&recvData,
											(void *)src_addr,
											proto_type
											);
		if(status)
		{
			setErrorCode( __FILE__,   __LINE__,
							(uint8 *)"dataReadAndSendToDualProcTask", status);
			return(status);
		}

		
		memoryCopy(	(uint8*)&data_ptr->data_buf[data_ptr->data_len],
					(uint8*)recvData.data_buf,
					recvData.data_len);

		data_ptr->data_len += recvData.data_len;
	}
	else if(recvData.data_len != dataframeLength)
	{
		status = MSG_LEN_INVALID_ERR;
		setErrorCode( __FILE__,   __LINE__,
						(uint8 *)"dataReadAndSendToDualProcTask", status);
		return(status);
	}
	else
	{
		
		memoryCopy(	(uint8*)&data_ptr->data_buf[MODBUS_TCP_LENGTH_OFFSET_SIZE],
					(uint8*)recvData.data_buf,
					recvData.data_len);

		data_ptr->data_len += recvData.data_len;
	}

	 
	 
	 
	if(data_ptr->data_len > MODBUS_DATA_VALUE_LENGTH)
	{
		status = MSG_LEN_INVALID_ERR;
		setErrorCode( __FILE__,   __LINE__,
						(uint8 *)"dataReadAndSendToDualProcTask", status);
		return(status);
	}

	return(status);
}
#endif 


uint32  ipAddrValidCheck(uint8 *addrPtr)
{
    uint32  status = NO_ERROR;
    uint8   index, dotCount = 0,*tmpPtr = addrPtr;
	uint32	ipAddrStrLen;
	
	ipAddrStrLen = strlen((const char*)addrPtr);
	
	
	if(ipAddrStrLen > MAX_IP_ADDR_STR_LEN)
		return(IP_ADDR_INVALID_ERR);

    for(index = 0; index < ipAddrStrLen; index++){
        if( *tmpPtr++ == '.') dotCount++;
    }
    if(dotCount != 3) status = IP_ADDR_INVALID_ERR;

    return(status);
}



void routeInfoAdd(uint32 arg1, uint32 arg2)
{
	int8	destIpAddr[32];
    int8	gatewayIpAddr[32];
	int8	*cPtr;
	strSystemBootParaInfo	paraData;
  	strUserBootParaInfo	*usrParaPtr;
	uint32	hexVal, routeNum = 0, i;
	uint32	status;
	
    memoryClear((uint8 *)destIpAddr, 32);
    memoryClear((uint8 *)gatewayIpAddr, 32);
    memoryClear((uint8 *) &paraData, sizeof(strSystemBootParaInfo));

	status  = nvRamRead((int8 *)&paraData,
						USR_ENET_CONFIG_PARA_OFFSET, 
						USR_ENET_CONFIG_PARA_SIZE
					   );
	if(status){
    	printf( "NV-RAM Read Error !!\n");
        goto MODULE_END;
	}

    usrParaPtr = &paraData.usrBootParaInfo;

	
    routeNum = 0;
    for(i = 0; i < MAX_ROUTE_NUM; i++) {
		if( ((usrParaPtr->routeIndex >> i) & 0x1) == 0x1)
        	routeNum += 1;
    }
    if(routeNum >= MAX_ROUTE_NUM) {
    	printf( "ROUTE Number[%d] is Exceed[MAX ROUTE NUM :: %d]\n", 
									routeNum, 
									(uint32)MAX_ROUTE_NUM);

		printf( "You must Delete Route !!!\n");
        goto MODULE_END;
	}


DEST_IPADDR:	
    printf( "Enter Ip Addr of Dest Station(XXX.XXX.XXX.0) ? ");
    ioRead(gDebugPortFd, destIpAddr, sizeof(destIpAddr));

    cPtr = destIpAddr;
    spaceSkip(&cPtr);
    if( (*cPtr != ENDOFSTR) && (ipAddrValidCheck( (uint8 *) cPtr) == NO_ERROR)) {
		;
	}
	else if ( *cPtr == '.') {
		printf( "Route Adding Cancel !!!\n");
        goto MODULE_END;
    }
	else {
    	printf( "InValid Ip Addr !!!!!!\n");
		goto DEST_IPADDR;
	}	

GATEWAY_IPADDR:	
    printf( "Enter IP Addr of Local Router(XXX.XXX.XXX.XXX) ? ");
    ioRead(gDebugPortFd, gatewayIpAddr, sizeof(gatewayIpAddr));

    cPtr = gatewayIpAddr;
    spaceSkip(&cPtr);
    if( (*cPtr != ENDOFSTR) && (ipAddrValidCheck( (uint8 *) cPtr) == NO_ERROR)) { 
		;
	}
	else if ( *cPtr == '.') {
		printf( "Route Adding Cancel !!!\n");
        goto MODULE_END;
    }
	else {
    	printf( "InValid Ip Addr !!!!!!\n");
		goto GATEWAY_IPADDR;
	}	

    
    hexVal = 0;
    ipAddrStrToHex ((uint32 *) &hexVal,(uint8 *)destIpAddr);
    for(i = 0; i < MAX_ROUTE_NUM; i++) {
    	if( ((usrParaPtr->routeIndex >> i) & 0x1) == 0x0){
    		usrParaPtr->routeTbl[i].destIp = hexVal;
            break;
		}
    }

    hexVal = 0;
    ipAddrStrToHex ((uint32 *) &hexVal,(uint8 *)gatewayIpAddr);
    usrParaPtr->routeTbl[i].gatewayIp = hexVal;

   	usrParaPtr->routeIndex |= 1 << i; 

    
    if(routeTblInfoAdd(destIpAddr, gatewayIpAddr)) {
    	printf( "Route Table Update Error !!!\n");
	}
	else {
    	
  		(void)nvRamWrite(  (int8 *)&paraData,
        	               USR_ENET_CONFIG_PARA_OFFSET,
            	           USR_ENET_CONFIG_PARA_SIZE
                	    );
	}
MODULE_END:	
	return;    
}


void routeInfoDelete(uint32 arg1, uint32 arg2)
{
	int8	destIpAddr[32];
    int8	gatewayIpAddr[32];
	int8	*cPtr;
	strSystemBootParaInfo	paraData;
  	strUserBootParaInfo	*usrParaPtr;
	uint32	destHexVal,gateHexVal, i, routeNum;
	uint32	status;

    memoryClear((uint8 *)destIpAddr, 32);
    memoryClear((uint8 *)gatewayIpAddr, 32);
    memoryClear((uint8 *) &paraData, sizeof(strSystemBootParaInfo));

	status  = nvRamRead((int8 *)&paraData,
						USR_ENET_CONFIG_PARA_OFFSET, 
						USR_ENET_CONFIG_PARA_SIZE
					   );
	if(status){
    	printf( "NV-RAM Read Error !!\n");
        goto MODULE_END;
	}
	usrParaPtr = &paraData.usrBootParaInfo;

	
    routeNum = 0;
    for(i = 0; i < MAX_ROUTE_NUM; i++) {
		if( ((usrParaPtr->routeIndex >> i) & 0x1) == 0x1)
        	routeNum += 1;
    }

    if((routeNum == 0) || (routeNum > MAX_ROUTE_NUM)) {
    	printf( "Invalid Route Table Info[%d] !!\n",routeNum );
        goto MODULE_END;
	}


DEST_IPADDR:	
    printf( "Enter Ip Addr of Dest Station(XXX.XXX.XXX.0) ? ");
    ioRead(gDebugPortFd, destIpAddr, sizeof(destIpAddr));

    cPtr = destIpAddr;
    spaceSkip(&cPtr);
    if( (*cPtr != ENDOFSTR) && (ipAddrValidCheck( (uint8 *) cPtr) == NO_ERROR)) 
		;
	else if ( *cPtr == '.') {
		printf( "Route Deleting Cancel !!!\n");
        goto MODULE_END;
    }
	else {
    	printf( "InValid Ip Addr !!!!!!\n");
		goto DEST_IPADDR;
	}	

GATEWAY_IPADDR:	
    printf( "Enter IP Addr of Local Router(XXX.XXX.XXX.XXX) ? ");
    ioRead(gDebugPortFd, gatewayIpAddr, sizeof(gatewayIpAddr));

    cPtr = gatewayIpAddr;
    spaceSkip(&cPtr);
    if( (*cPtr != ENDOFSTR) && (ipAddrValidCheck( (uint8 *) cPtr) == NO_ERROR)) 
		;
	else if ( *cPtr == '.') {
		printf( "Route Deleting Cancel !!!\n");
        goto MODULE_END;
    }
	else {
    	printf( "InValid Ip Addr !!!!!!\n");
		goto GATEWAY_IPADDR;
	}	

    
    destHexVal = 0;
    ipAddrStrToHex ((uint32 *) &destHexVal,(uint8 *)destIpAddr);

    gateHexVal = 0;
    ipAddrStrToHex ((uint32 *) &gateHexVal,(uint8 *)gatewayIpAddr);

	
    for(i = 0; i < MAX_ROUTE_NUM; i++) {
    	if( (usrParaPtr->routeIndex >> i) & 0x1) {
			if( (usrParaPtr->routeTbl[i].destIp 	== destHexVal) &&
				(usrParaPtr->routeTbl[i].gatewayIp == gateHexVal)
		  	)
				break;
		} 
    }

    if(i == MAX_ROUTE_NUM) {
    	printf( "The Ip Addr is Not Exist In Route Table !!!\n");
        printf( "Check The Ip Address And Retry !!!\n");
        goto MODULE_END;
	}

    
	usrParaPtr->routeIndex ^= 1 << i; 		

    
    if(routeTblInfoDelete(destIpAddr, gatewayIpAddr)) {
    	printf( "Route Table Delete Error !!!\n");
	}
	else {
    	
			(void)nvRamWrite(  (int8 *)&paraData,
        	               USR_ENET_CONFIG_PARA_OFFSET,
            	           USR_ENET_CONFIG_PARA_SIZE
                	    );
	}

MODULE_END:	
	return;
}


void routeInfoShow(uint32 whereToRead, uint32 arg)
{
	strSystemBootParaInfo	paraData;
  	strUserBootParaInfo	*usrParaPtr;
	uint32	routeNum, i;
	int8	*stringTitle[] =
    {(int8 *)"DESTINATION ", (int8 *)" GATEWAY", (int8 *)NULL};
	int8	**tMsg;
	int8	ipAddr[32];
	uint32	status;
	strSysConfigInfo configData;
    
	if(whereToRead == NVRAM_READ) 
	{
		memoryClear((uint8 *) &paraData, sizeof(strSystemBootParaInfo));
		status  = nvRamRead((int8 *)&paraData,
							USR_ENET_CONFIG_PARA_OFFSET, 
							USR_ENET_CONFIG_PARA_SIZE
						   );
		
		if(status){
    		printf( "NV-RAM Read Error !!\n");
			goto MODULE_END;
		}

		usrParaPtr = &paraData.usrBootParaInfo;

		
		routeNum = 0;
		for(i = 0; i < MAX_ROUTE_NUM; i++) {
			if( ((usrParaPtr->routeIndex >> i) & 0x1) == 0x1)
        		routeNum += 1;
		}

		printf( "\nROUTE NET TABLE In NV-RAM\n");
		if(routeNum == 0) {
    		printf( "\t********ROUTE TABLE INFO NONE !!!\n");
			goto MODULE_END;
		}

		printf( "----------------------------------------\n");
		for(tMsg = stringTitle; *tMsg != NULL; tMsg++) {
			printf( "%15s", *tMsg);
		}
		printf( "\n"); 
		printf( "----------------------------------------\n");

		for(i = 0; i < MAX_ROUTE_NUM; i++) {
			if( (usrParaPtr->routeIndex >> i) & 0x1) {
				memoryClear((uint8 *)ipAddr, 32);
				ipAddrHexToString(ipAddr, usrParaPtr->routeTbl[i].destIp);
				printf( "%15s", ipAddr);

				memoryClear((uint8 *)ipAddr, 32);
				ipAddrHexToString(ipAddr, usrParaPtr->routeTbl[i].gatewayIp);
				printf( "%18s\n", ipAddr);		
			}
		}
	}
	else if(whereToRead == EXT_VAR_READ)
	{
		memoryClear((uint8 *) &configData, sizeof(strSysConfigInfo));
		status =systemCnfgInfoRead((strSysConfigInfo *)&configData);
		if(status){
    		printf( "system Config Info Read Err!!\n");
			goto MODULE_END;
		}

		
		routeNum = 0;
		for(i = 0; i < MAX_ROUTE_NUM; i++) {
			if( ((configData.routeIndex >> i) & 0x1) == 0x1)
        		routeNum += 1;
		}

		printf( "\nROUTE NET TABLE In Memory\n");
		if(routeNum == 0) {
    		printf( "\t********ROUTE TABLE INFO NONE !!!\n");
			goto MODULE_END;
		}

		printf( "----------------------------------------\n");
		for(tMsg = stringTitle; *tMsg != NULL; tMsg++) {
			printf( "%15s", *tMsg);
		}
		printf( "\n"); 
		printf( "----------------------------------------\n");

		for(i = 0; i < MAX_ROUTE_NUM; i++) {
			if( (configData.routeIndex >> i) & 0x1) {
				memoryClear((uint8 *)ipAddr, 32);
				ipAddrHexToString(ipAddr, configData.routeTbl[i].destIp);
				printf( "%15s", ipAddr);

				memoryClear((uint8 *)ipAddr, 32);
				ipAddrHexToString(ipAddr, configData.routeTbl[i].gatewayIp);
				printf( "%18s\n", ipAddr);		
			}
		}

	}
#ifdef _WIN32
	routeTblShow(routeNum, &usrParaPtr->routeTbl[0]);
#endif 
MODULE_END:
	return;
}

#ifdef _WIN32
void bzero(char *buffer, int nbytes) {
	memset(buffer,0,nbytes);
}
#endif 




extern uint32 systemCnfgInfoRead( strSysConfigInfo *);
uint32  remoteShelfAddrGet(uint32 lineType, uint32 *addrPtr)
{
	uint32 status = NO_ERROR;
	strSysConfigInfo sysCnfgInfo;

	systemCnfgInfoRead((strSysConfigInfo *)&sysCnfgInfo);
	if(lineType==NET_LINE_LINK_A){
		*addrPtr = sysCnfgInfo.netCnfgInfo.rmtIpAddr[0];
	}
	else if(lineType==NET_LINE_LINK_B){
		*addrPtr = sysCnfgInfo.netCnfgInfo.rmtIpAddr[1];
	}
	else return(1);

	return(status);
}


#if (defined (UNIX) || defined (_WIN32) || defined(VXWORKS))
uint32 connectWithTimeout1(int32 socketFd, struct sockaddr *sockAddr, uint32 len, uint32 mSec, uint32 *sockErrPtr)
{
	uint32 status = NO_ERROR;
	int option, retVal, ErrNo;
	int	saveflags;
	int	maxFd;
	fd_set tempFd; 
	struct timeval timeout;
	struct sockaddr_in *sockAddrPtr;

	sockAddrPtr = (struct sockaddr_in*)sockAddr;

	FD_ZERO(&tempFd); 

	
#if (defined(_WIN32))
	option = NON_BLOCKING_ENABLE;
	if(ioctlsocket(socketFd, FIONBIO, (unsigned long*) &option) == SOCKET_ERROR)
#elif (defined(UNIX))
	saveflags=fcntl(socketFd,F_GETFL,0);
	if (saveflags < 0)
	{
		ErrNo=errno;
		status = SOCK_OPTSET_ERR;
		goto MODULE_END;
	}
	
	if(fcntl(socketFd,F_SETFL,saveflags|O_NONBLOCK)<0)
#elif (defined(VXWORKS))
	option = O_NONBLOCK;
	if (ioctl(socketFd, FIONBIO, option) == ERROR)
#endif	
	{
#ifdef _WIN32			
		ErrNo = WSAGetLastError();
#else
		ErrNo = errno;
#endif	
	
		status = SOCK_OPTSET_ERR;
		goto MODULE_END;
	}

	
	len = SOCK_ADDR_LENGTH;
	retVal=connect(socketFd,
                (struct sockaddr *)sockAddr,
                SOCK_ADDR_LENGTH); 
#ifdef _WIN32			
	ErrNo = WSAGetLastError();
#else
	ErrNo = errno;
#endif	
	
#ifdef _WIN32
	if( (retVal<0) && (ErrNo!=WSAEINPROGRESS) && (ErrNo!=WSAEWOULDBLOCK) )
#else
	if( (retVal<0) && (ErrNo!=EINPROGRESS) && (ErrNo!=EWOULDBLOCK) )
#endif	
	{
		#ifdef _WIN32			
			ErrNo = WSAGetLastError();
		#else
			ErrNo = errno;
		#endif	
		status = SOCK_CONNECT_ERR ;
		goto MODULE_END;
	} 
	
	
#if (defined(_WIN32))
	option = NON_BLOCKING_DISABLE;
	if(ioctlsocket(socketFd, FIONBIO, (unsigned long*) &option) == SOCKET_ERROR)
#elif (defined(UNIX))
	if(fcntl(socketFd,F_SETFL,saveflags) < 0)
#elif (defined(VXWORKS))
	option = ~O_NONBLOCK;
	if (ioctl(socketFd, FIONBIO, option) == ERROR)
#endif	
	{
	#ifdef _WIN32			
		ErrNo = WSAGetLastError();
	#else
		ErrNo = errno;
	#endif	
		status = SOCK_OPTSET_ERR;
		goto MODULE_END;
	}

	

	
	timeout.tv_sec = (long)(mSec/1000); 
	timeout.tv_usec = (long)((mSec%1000)*1000); 

	FD_SET(socketFd,&tempFd); 

	maxFd = socketFd;
	
	if((retVal=select(maxFd+1,NULL,&tempFd,NULL, &timeout)) == -1) {
	#ifdef _WIN32			
		ErrNo = WSAGetLastError();
	#else
		ErrNo = errno;
	#endif	
		status = SOCK_OPEN_ERR;
		goto MODULE_END;
	}


	
#if 1
	if(retVal==0) { 
	#ifdef _WIN32			
		ErrNo= WSAETIMEDOUT;
	#else
		ErrNo= ETIMEDOUT;
	#endif	
		status = SOCK_OPEN_ERR;
		goto MODULE_END;
	}
#endif

	
	len = sizeof(option); 
	if(getsockopt(socketFd,SOL_SOCKET,SO_ERROR,(char*)&option,(int*)&len) < 0 ) { 
	#ifdef _WIN32			
		ErrNo = WSAGetLastError();
	#else
		ErrNo = errno;
	#endif	
		status = SOCK_OPTSET_ERR;
		goto MODULE_END;
	} 

	
	if(option) { 
		ErrNo = option; 
		status  = SOCK_CONNECT_ERR;
		goto MODULE_END;
	} 

MODULE_END:

	if (socketFd > 0)
	{
		FD_CLR (socketFd, &tempFd);
	}

	if(status!=NO_ERROR) *sockErrPtr = ErrNo;

	return(status);
}
#endif	
