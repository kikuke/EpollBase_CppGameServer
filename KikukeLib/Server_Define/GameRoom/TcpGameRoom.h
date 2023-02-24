#ifndef TCP_GAMEROOM
#define TCP_GAMEROOM

#include <queue>
#include <time.h>
#include <random>

#include "ServerInfo.h"
#include "ObjectEvent.h"
#include "AI_Npc.h"

//Todo: 메시지 그냥 여기서 바로 보내거나 TcpInfo에 send버퍼 만들어서 일괄 전송하기 일괄전송기능 사용 sendv였나
class TcpGameRoom
{
private:
    int room_num;
    //Comment: low endTime sort
    std::priority_queue<ObjectEvent, std::vector<ObjectEvent>, std::greater<ObjectEvent>> obj_events;

    //Todo: map으로 한번 더 매핑하기
    int max_npc_num;
    AI_Npc** npc_pool;

    //Todo: map으로 한번 더 매핑하기
    int max_clnt_num;
    TCPSOCKETINFO** clnt_info;

    std::random_device rand_dv;
    std::mt19937* gen;
    std::uniform_int_distribution<int>* rand;

    //Todo: 나중에 분리하기
    Obj_Position RandomObjPos();
    Object_Info* CreateObject_Info(Obj_Position pos);
    AI_Npc* CreateAI_Npc(Object_Info* info);

    AI_Npc* FindAI_Npc(int id);

public:
    TcpGameRoom();
    ~TcpGameRoom();

    void InitGame(int room_num, int max_npc_num, int clnt_num, int* clnt_socks);
    void StartGame(timeval& nowtime);
    void update(timeval& nowtime);
    void EndGame(timeval& nowtime);
};

#endif