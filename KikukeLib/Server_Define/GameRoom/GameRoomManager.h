#ifndef GAMEROOM_MANAGER
#define GAMEROOM_MANAGER

#include "TcpGameRoom.h"

class GameRoomManager
{
private:
    int room_cnt;
    TcpGameRoom** gameRooms;
    bool* isEmptyRoom;

public:
    GameRoomManager()
    {
        room_cnt = 0;
    }

    ~GameRoomManager()
    {
        for(int i=0; i<room_cnt; i++){
            delete gameRooms[i];
        }
        delete isEmptyRoom;
    }

    TcpGameRoom* OpenGameRoom(int room_id, Object_Rule obj_rule, int npc_num, int clnt_num, int* clnt_socks);
    bool CloseGameRoom();
    TcpGameRoom* GetGameRoom(int room_id);
};

#endif