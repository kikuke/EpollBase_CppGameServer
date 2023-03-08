#ifndef TCP_PLAYER_PACKET
#define TCP_PLAYER_PACKET

#include "Logger.h"
#include "PacketDefine.h"
#include "GameRoom/GameRoomManager.h"
#include "PacketHandler.h"

#define TCP_PLAYER_PACKET_BUFFER_SIZE 2048

class TcpPlayerPacket : public PacketHandler
{
private:
    unsigned char buf[TCP_PLAYER_PACKET_BUFFER_SIZE];
    GameRoomManager* gameRoomManager;

    Logger* log;
public:
    TcpPlayerPacket(GameRoomManager* gameRoomManager):PacketHandler(PLAYER) {
        log = new Logger("TcpPlayerPacket");

        this->gameRoomManager = gameRoomManager;
    }

    ~TcpPlayerPacket() {
        delete log;
    }
    int execute(int sock, unsigned int subOp, RingBuffer& buffer) override;
    int catchError(int sock, unsigned int errorCode) override;
};

#endif