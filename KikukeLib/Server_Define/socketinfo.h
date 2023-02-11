#ifndef SOCKET_INFO
#define SOCKET_INFO

#include <netinet/in.h>

#include "../struct/RingBuffer.h"

struct TCPSOCKETINFO
{
    int socket;
    struct sockaddr_in sockAddr;

    RingBuffer recvBuffer;
};

#endif