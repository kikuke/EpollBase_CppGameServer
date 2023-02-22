#ifndef TCP_PACKET_HANDLER
#define TCP_PACKET_HANDLER

#include <sys/types.h>

#include "PacketDefine.h"
#include "RingBuffer.h"
#include "PacketHandler.h"
#include "Logger.h"

class TcpPacketHandler
{
private:
    PacketHandler** handlers;
    size_t handler_size;

    Logger* log;

    //나중에 인자 필요해지면 아예 소켓을 넣던가 해서 추가정보 주기
    int TCPHeaderCheck(TCPTestPacketHeader* header, size_t useSz);
    //call packetHandler's execute()
    int ExecuteOP(int sock, unsigned int mainOp, unsigned int subOp, RingBuffer& buffer);
    //call packetHandler's catchError()
    int CatchError(int sock, unsigned int mainOp, unsigned int errorCode);

public:
    TcpPacketHandler(PacketHandler** handlers, size_t handler_size){
        this->handler_size = handler_size;
        this->handlers = handlers;
        
        log = new Logger("TcpPacketHandle");
    }
    virtual ~TcpPacketHandler(){
        for(int i=0; i<handler_size; i++){
            delete handlers[i];
        }
        handlers = nullptr;

        delete log;
    }

    int execute(int sock);
};

#endif