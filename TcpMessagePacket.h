#ifndef TCP_MESSAGE_PACKET
#define TCP_MESSAGE_PACKET

#include "PacketDefine.h"
#include "PacketHandler.h"

class TcpMessagePacket : public PacketHandler
{
private:
    int MessageEcho(MessageEchoData meData);

public:
    TcpMessagePacket():PacketHandler(MESSAGE) {}
    ~TcpMessagePacket() {}

    int execute(int sock, unsigned int subOp, RingBuffer& buffer) override
    {
        switch (subOp)
        {
        case MESSAGE_ECHO:
            MessageEchoData meData;
            if(DequeueData(meData, buffer) != TCP_PACKET_END_CODE){
                return 0;//에러코드로 바꿔주기
            }

            return MessageEcho(meData);//에러나면 에러코드가 반환됨.
            break;
        
        default:
            //error
            return 0;
            break;
        }
        return 1;
    }

    int catchError(int sock, unsigned int errorCode) override
    {
        switch (errorCode)
        {
        default:
            //error
            return 0;
            break;
        }
        return 1;
    }
};

#endif