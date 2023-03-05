#ifndef SERVER_INFO
#define SERVER_INFO

#include <netinet/in.h>

#include "ThreadSafe/TSRingBuffer.h"
#include "ThreadSafe/TSQueue.h"

struct JobQueue
{
    TSQueue<int> readQueue;
    TSQueue<int> workQueue;
    TSQueue<int> broadcastQueue;
};

struct TCPSOCKETINFO
{
    int id;
    
    int socket;
    struct sockaddr_in sockAddr;

    RingBuffer* recvBuffer;
};

#endif