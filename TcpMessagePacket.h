#ifndef TCP_MESSAGE_PACKET
#define TCP_MESSAGE_PACKET

#include "PacketDefine.h"
#include "PacketHandler.h"

class TcpMessagePacket : public PacketHandler
{
private:
    /* data */
public:
    TcpMessagePacket():PacketHandler(MESSAGE) {}
    ~TcpMessagePacket();

    int execute(unsigned int subOp, RingBuffer& buffer) override
    {
        
    }
};

#endif