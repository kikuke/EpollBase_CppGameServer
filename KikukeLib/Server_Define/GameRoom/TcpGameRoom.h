#ifndef TCP_GAMEROOM
#define TCP_GAMEROOM

#include <queue>
#include <time.h>
#include <random>

#include "ServerInfo.h"
#include "ObjectEvent.h"
#include "AI_Npc.h"
#include "Logger.h"

//Todo: 메시지 그냥 여기서 바로 보내거나 TcpInfo에 send버퍼 만들어서 일괄 전송하기
class TcpGameRoom
{
private:
    int room_num;
    int obj_idCnt;
    //Comment: low endTime sort
    std::priority_queue<ObjectEvent, std::vector<ObjectEvent>, std::greater<ObjectEvent>> obj_events;

    Object_Info** obj_infoMap;

    int max_npc_num;
    AI_Npc** npc_pool;
    AI_Npc** ai_infoMap;

    int max_clnt_num;
    //Comment: 전체 발송용 배열
    int* clnt_socks;

    //Comment: 업데이트 된 정보들만 넘겨주는 용도
    int max_obj_num;
    int updateNum;
    Object_Info** updateInfo;

    std::random_device rand_dv;
    std::mt19937* gen;
    std::uniform_int_distribution<int>* rand_world_pos;

    Logger* log;

    //Todo: 나중에 분리하기
    Obj_Position RandomObjPos();
    Object_Info* CreateObject_Info(Obj_Position pos);
    AI_Npc* CreateAI_Npc(Object_Info* info);

    AI_Npc* FindAI_Npc(int id);
    Object_Info* FindObjInfo(int id);

    void AddUpdateInfo(Object_Info* info);

    //Comment: 업데이트 된 정보들만 클라이언트들에게 넘겨줌.
    void SendUpdateObject_Info(int sock);

public:
    TcpGameRoom();
    ~TcpGameRoom();

    //Comment: 클라이언트의 입력신호
    void InterruptEvent(Object_Info* info);
    void InitGame(int room_num, int npc_num, int clnt_num, int* clnt_socks);
    void StartGame(timeval& nowtime);
    void update(timeval& nowtime);
    void EndGame(timeval& nowtime);
};

#endif