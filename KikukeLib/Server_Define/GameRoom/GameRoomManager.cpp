#include "GameRoomManager.h"

size_t GameRoomManager::DoubleBuffer()
{
    int new_room_cnt = room_cnt*2;
    TcpGameRoom** newGameRooms = new TcpGameRoom*[new_room_cnt];
    bool* newIsUseRoom = new bool[new_room_cnt];
    for(int i=0; i<room_cnt; i++){
        newGameRooms[i] = gameRooms[i];
        newGameRooms[room_cnt + i] = new TcpGameRoom(jobQueue);
    }

    for(int i=0; i<room_cnt; i++){
        newIsUseRoom[i] = isUseRoom[i];
    }

    delete newGameRooms;
    delete isUseRoom;

    room_cnt = new_room_cnt;
    gameRooms = newGameRooms;
    isUseRoom = newIsUseRoom;

    return room_cnt;
}

void GameRoomManager::UpdateGameRooms(timeval& nowtime)
{
    int roomCnt = this->room_cnt;

    for(int i=0; i<roomCnt; i++){
        if(isUseRoom[i]){
            gameRooms[i]->update(nowtime);
        }
    }
}

int GameRoomManager::OpenGameRoom(Object_Rule obj_rule, int npc_num, int clnt_num, int* clnt_socks)
{
    int i;
    for(i=0; i<room_cnt; i++){
        if(!isUseRoom[i]){
            break;
        }
    }

    if(i >= room_cnt){
        DoubleBuffer();
    }

    gameRooms[i]->InitGame(i, obj_rule, npc_num, clnt_num, clnt_socks);
    isUseRoom[i] = true;
    return i;
}

bool GameRoomManager::CloseGameRoom(int room_id)
{
    return true;
}

TcpGameRoom* GameRoomManager::GetGameRoom(int room_id)
{
    if(room_id >= room_cnt || !isUseRoom[room_id]){
        return nullptr;
    }

    return gameRooms[room_id];
}