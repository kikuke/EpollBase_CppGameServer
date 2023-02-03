#ifndef SOCKET_INFO
#define SOCKET_INFO

#include "RingBuffer.h"
struct TCPSOCKETINFO
{
    int socket;

    RingBuffer recvBuffer;
};

#endif