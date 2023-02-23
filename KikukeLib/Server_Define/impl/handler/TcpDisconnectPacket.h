#ifndef TCP_DISCONNECT_PACKET
#define TCP_DISCONNECT_PACKET

#include "Logger.h"
#include "PacketDefine.h"
#include "PacketHandler.h"

class TcpDisconnectPacket : public PacketHandler
{
private:
    int epfd;

    int Disconnect(int sock, DisconnectData data);

    Logger* log;
public:
    TcpDisconnectPacket(int epfd):PacketHandler(DISCONNECT) {
        log = new Logger("TcpDisconnectPacket");
        this->epfd = epfd;
    }
    ~TcpDisconnectPacket() {
        delete log;
    }
    int execute(int sock, unsigned int subOp, RingBuffer& buffer) override;
    int catchError(int sock, unsigned int errorCode) override;
};

#endif