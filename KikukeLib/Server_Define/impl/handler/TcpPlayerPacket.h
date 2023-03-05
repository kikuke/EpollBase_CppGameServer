#ifndef TCP_PLAYER_PACKET
#define TCP_PLAYER_PACKET

#include "Logger.h"
#include "PacketDefine.h"
#include "PacketHandler.h"

#define TCP_PLAYER_PACKET_BUFFER_SIZE 2048

class TcpPlayerPacket : public PacketHandler
{
private:
    unsigned char buf[TCP_PLAYER_PACKET_BUFFER_SIZE];

    Logger* log;
public:
    TcpPlayerPacket():PacketHandler(PLAYER) {
        log = new Logger("TcpPlayerPacket");
    }

    ~TcpPlayerPacket() {
        delete log;
    }
    int execute(int sock, unsigned int subOp, RingBuffer& buffer) override;
    int catchError(int sock, unsigned int errorCode) override;
};

#endif