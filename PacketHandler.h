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

    int TCPHeaderCheck(TCPSOCKETINFO* info);

public:
    //PacketHandler(int startCode, int endCode, const int (*handler)(const void*, size_t));
    //virtual ~PacketHandler();

    int execute(int sock);//아예 추상 클래스로 만들어보이기?
};

#endif