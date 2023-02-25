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
    //Todo: interupt event 발생하면 전부 빼내서 다시계산하는 방식으로.
    //Comment: low endTime sort
    std::priority_queue<ObjectEvent, std::vector<ObjectEvent>, std::greater<ObjectEvent>> obj_end_events;

    Object_Info** obj_infoMap;

    int max_npc_num;
    AI_Npc** npc_pool;
    AI_Npc** ai_infoMap;

    int max_clnt_num;
    //Comment: 전체 발송용 배열
    int* clnt_socks;

    int max_obj_num;
    int updateNum;
    //Comment: 업데이트 이전 정보 임시 저장 공간. 인덱스 = id.
    Object_Info** nowObjInfo;
    //Comment: 정보 업데이트를 위해 사용되는 계산용 임시 공간.
    Object_Info** updateObjInfo;

    std::random_device rand_dv;
    std::mt19937* gen;
    std::uniform_int_distribution<int>* rand_world_pos;

    Logger* log;

    //Todo: 나중에 분리하기
    Obj_Position RandomObjPos();
    Object_Info* CreateObject_Info(Obj_Position pos);
    AI_Npc* CreateAI_Npc(Object_Info* info, float speed);

    AI_Npc* FindAI_Npc(int id);
    Object_Info* FindObjInfo(int id);

    //Comment: 조작된 정보인지 유효한 데이터인지 체크.
    //return true if validate
    bool CheckValidateObjInfo(timeval& nowtime, Object_Info* newObjInfo);

    //Todo: AI_Npc와 Object_Info 상태를 결정짓는 함수가 필요함.

    void AddUpdateInfo(Object_Info* info);

    //Comment: 업데이트 된 정보들만 클라이언트들에게 넘겨줌.
    void SendUpdateObject_Info(int sock);

public:
    TcpGameRoom();
    ~TcpGameRoom();

    //Comment: 클라이언트의 입력신호
    void InterruptEvent(timeval& nowtime, Object_Info* info);
    void InitGame(int room_num, int npc_num, int clnt_num, int* clnt_socks);
    void StartGame(timeval& nowtime);
    void update(timeval& nowtime);
    void EndGame(timeval& nowtime);
};

#endif