#ifndef PACKET_HANDLER
#define PACKET_HANDLER

#include <sys/types.h>

#include "RingBuffer.h"

class PacketHandler
{
private:
    //const unsigned int startCode;
    //const unsigned int endCode;

    //const int (*handler)(const void*, size_t);

    RingBuffer readBuf;

public:
    //PacketHandler(int startCode, int endCode, const int (*handler)(const void*, size_t));
    //virtual ~PacketHandler();

    int execute(int sock);
};

#endif