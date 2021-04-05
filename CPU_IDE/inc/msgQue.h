
#ifndef MSGQUE_H
#define MSGQUE_H


typedef struct message_queue_info {
    int32   queueCount;
    int32   queueSize;
    int32   queueOptions;
#ifdef _WIN32
	char*	msgName;
#endif
}strMsgQueInfo;

typedef struct message_queue_block {
    int32   queueId;
    strMsgQueInfo queueInfo;
}strMsgQueBlock;


#endif 
