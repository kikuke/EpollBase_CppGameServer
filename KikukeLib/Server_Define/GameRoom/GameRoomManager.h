#ifndef GAMEROOM_MANAGER
#define GAMEROOM_MANAGER

#include "TcpGameRoom.h"

#define DEFALT_GAMEROOM_SIZE 10

class GameRoomManager
{
private:
    JobQueue* jobQueue;
    int room_cnt;
    TcpGameRoom** gameRooms;
    bool* isUseRoom;
    bool* isStartRoom;

    size_t DoubleBuffer();

public:
    GameRoomManager(JobQueue* jobQueue)
    {
        this->jobQueue = jobQueue;
        room_cnt = DEFALT_GAMEROOM_SIZE;

        gameRooms = new TcpGameRoom*[room_cnt];
        for(int i=0; i<room_cnt; i++){
            gameRooms[i] = new TcpGameRoom(this->jobQueue);
        }

        isUseRoom = new bool[room_cnt];
        isStartRoom = new bool[room_cnt];
    }

    ~GameRoomManager()
    {
        for(int i=0; i<room_cnt; i++){
            delete gameRooms[i];
        }
        delete isUseRoom;
        delete isStartRoom;
    }

    //return room_id
    int OpenGameRoom(Object_Rule obj_rule, int npc_num, int clnt_num, int* clnt_socks);
    bool CloseGameRoom(int room_id);
    TcpGameRoom* GetGameRoom(int room_id);
    void UpdateGameRooms(timeval& nowtime);
};

#endif