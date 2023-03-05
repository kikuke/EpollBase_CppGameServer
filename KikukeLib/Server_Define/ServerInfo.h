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
    //Comment: 클라이언트가 인증서버에서 발급받은 후 게임서버로 전달하는 임시 id(토큰 느낌)
    int id;

    int socket;
    struct sockaddr_in sockAddr;

    RingBuffer* recvBuffer;
};

#endif