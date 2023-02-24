#ifndef TCP_GAMEROOM
#define TCP_GAMEROOM

#include <queue>
#include <time.h>
#include <random>

#include "ObjectEvent.h"
#include "AI_Npc.h"

class TcpGameRoom
{
private:
    int room_num;
    //Comment: low endTime sort
    std::priority_queue<ObjectEvent, std::vector<ObjectEvent>, std::greater<ObjectEvent>> obj_event;

    int max_npc_num;
    AI_Npc** npc_pool;

    std::random_device rand_dv;
    std::mt19937* gen;
    std::uniform_int_distribution<int>* rand;

    //Todo: 나중에 분리하기
    Obj_Position RandomObjPos();
    Object_Info* CreateObject_Info(Obj_Position pos);
    AI_Npc* CreateAI_Npc(Object_Info* info);

public:
    TcpGameRoom();
    ~TcpGameRoom();

    void InitGame(int room_num, int max_npc_num);
    void StartGame();
    void update(timeval& nowtime);
};

#endif