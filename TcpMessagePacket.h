#ifndef TCP_MESSAGE_PACKET
#define TCP_MESSAGE_PACKET

#include "PacketDefine.h"
#include "PacketHandler.h"

class TcpMessagePacket : public PacketHandler
{
private:
    int MessageEcho(int sock, MessageEchoData meData);

    Logger* log;
public:
    TcpMessagePacket():PacketHandler(MESSAGE) {
        log = new Logger("TcpMessagePacket");
    }

    ~TcpMessagePacket() {
        delete log;
    }
    int execute(int sock, unsigned int subOp, RingBuffer& buffer) override;
    int catchError(int sock, unsigned int errorCode) override;
};

#endif