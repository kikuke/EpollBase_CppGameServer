#ifndef I_PACKET_HANDLER
#define I_PACKET_HANDLER

#include "RingBuffer.h"

class IPacketHandler
{
public:
    virtual int execute(RingBuffer& buffer);
};

#endif