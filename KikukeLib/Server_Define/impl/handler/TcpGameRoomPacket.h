#ifndef TCP_GAMEROOM_PACKET
#define TCP_GAMEROOM_PACKET

#include "Logger.h"
#include "PacketDefine.h"
#include "PacketHandler.h"
#include "GameRoom/GameRoomManager.h"

class TcpGameRoomPacket : public PacketHandler
{
private:
    GameRoomManager* gameRoomManager;

    Logger* log;
public:
    TcpGameRoomPacket(GameRoomManager* gameRoomManager):PacketHandler(GAMEROOM) {
        log = new Logger("TcpGameRoomPacket");

        this->gameRoomManager = gameRoomManager;
    }

    ~TcpGameRoomPacket() {
        delete log;
    }
    int execute(int sock, unsigned int subOp, RingBuffer& buffer) override;
    int catchError(int sock, unsigned int errorCode) override;
};

#endif