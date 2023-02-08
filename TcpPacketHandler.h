#ifndef TCP_PACKET_HANDLER
#define TCP_PACKET_HANDLER

#include <sys/types.h>

#include "PacketDefine.h"
#include "RingBuffer.h"
#include "PacketHandler.h"

class TcpPacketHandler
{
private:
    size_t handler_size;
    PacketHandler* handlers;

    RingBuffer readBuf;

    int TCPHeaderCheck(TCPTestPacketHeader* header);
    void ExecuteOP(unsigned int mainOp, unsigned int subOp);

public:
    TcpPacketHandler(size_t handler_size, PacketHandler* handlers){
        this->handler_size = handler_size;
        this->handlers = handlers;
    }
    virtual ~TcpPacketHandler(){
        delete[] handlers;
        handlers = nullptr;
    }

    int execute(int sock);
};

#endif