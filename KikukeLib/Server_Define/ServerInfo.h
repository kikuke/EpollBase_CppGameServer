#ifndef SERVER_INFO
#define SERVER_INFO

#include <netinet/in.h>

#include "RingBuffer.h"
#include "ThreadSafe/TSQueue.h"

struct JobQueue
{
    TSQueue<int> readQueue;
    TSQueue<int> workQueue;
};

struct TCPSOCKETINFO
{
    int socket;
    struct sockaddr_in sockAddr;

    RingBuffer recvBuffer;
};

#endif