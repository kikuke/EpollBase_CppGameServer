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
    ~TcpMessagePacket() {}

    int execute(unsigned int subOp, RingBuffer& buffer) override
    {
        switch (subOp)
        {
        case MESSAGE_ECHO:
            /* code */
            //에코 코드 넣어두기
            break;
        
        default:
            //error
            return 0;
            break;
        }
        return 1;
    }
};

#endif