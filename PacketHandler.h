#ifndef PACKET_HANDLER
#define PACKET_HANDLER

#include "RingBuffer.h"

class PacketHandler
{
private:
    const unsigned int mainOp;
public:
    PacketHandler(unsigned int _mainOp) : mainOp(_mainOp){}

    unsigned int getMainOp() {
        return mainOp;
    }

    virtual int execute(unsigned int subOp, RingBuffer& buffer) = 0;
};

#endif