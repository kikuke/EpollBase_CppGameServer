#ifndef TCP_PACKET_HANDLER
#define TCP_PACKET_HANDLER

#include <sys/types.h>

#include "PacketDefine.h"
#include "RingBuffer.h"
#include "PacketHandler.h"

class TcpPacketHandler
{
private:
    PacketHandler** handlers;
    size_t handler_size;

    RingBuffer readBuf;

    int TCPHeaderCheck(TCPTestPacketHeader* header);
    //call packetHandler's execute()
    int ExecuteOP(int sock, unsigned int mainOp, unsigned int subOp);
    //call packetHandler's catchError()
    int CatchError(int sock, unsigned int mainOp, unsigned int errorCode);

public:
    TcpPacketHandler(PacketHandler** handlers, size_t handler_size){
        this->handler_size = handler_size;
        this->handlers = handlers;
    }
    virtual ~TcpPacketHandler(){
        for(int i=0; i<handler_size; i++){
            delete handlers[i];
        }
        delete[] handlers;
        handlers = nullptr;
    }

    int execute(int sock);
};

#endif