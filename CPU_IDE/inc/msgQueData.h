
#ifndef MSGQUEDATA_H
#define MSGQUEDATA_H

#define	MAX_DATA_LEN	MAX_PROTOCOL_DATA_LEN 


typedef struct data_str {
    uint32  socket_fd;
    uint32  data_len;
    uint8   data_buf[MAX_DATA_LEN]; 
}strDataBuf;


typedef struct  queProtocol {
    uint32  numOfBytes;
    uint8   msgData[MAX_DATA_LEN]; 
}strProtocolData;


typedef struct message_queue_data {
    strinetAddr     peerAddr; 
    strProtocolData protoData;
}strMsgQueData;

#endif 
