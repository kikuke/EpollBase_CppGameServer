#ifndef GAMEROOM_MANAGER
#define GAMEROOM_MANAGER

#include "TcpGameRoom.h"

#define DEFALT_GAMEROOM_SIZE 10

class GameRoomManager
{
private:
    int room_cnt;
    TcpGameRoom** gameRooms;
    bool* isUseRoom;

    size_t DoubleBuffer();

public:
    GameRoomManager()
    {
        room_cnt = DEFALT_GAMEROOM_SIZE;

        gameRooms = new TcpGameRoom*[room_cnt];
        for(int i=0; i<room_cnt; i++){
            gameRooms[i] = new TcpGameRoom;
        }

        isUseRoom = new bool[room_cnt];
    }

    ~GameRoomManager()
    {
        for(int i=0; i<room_cnt; i++){
            delete gameRooms[i];
        }
        delete isUseRoom;
    }

    //return room_id
    int OpenGameRoom(Object_Rule obj_rule, int npc_num, int clnt_num, int* clnt_socks);
    bool CloseGameRoom();
    TcpGameRoom* GetGameRoom(int room_id);
};

#endif