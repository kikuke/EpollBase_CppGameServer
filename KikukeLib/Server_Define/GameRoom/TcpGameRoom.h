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
    Object_Rule obj_rule;
    //Comment: 변경한 정보만 발송하는용도.
    bool* isUpdateId;

    //Comment: AI_Npc에서만 사용. 플레이어는 측정할 필요가 없기 때문
    //Comment: low endTime sort
    std::priority_queue<NpcEndEvent, std::vector<NpcEndEvent>, std::greater<NpcEndEvent>> npc_end_events;

    Object_Info** obj_nowInfoMap;
    //Comment: 업데이트 전 임시 저장용
    Object_Info** obj_oldInfoMap;

    int max_npc_num;
    AI_Npc** npc_pool;
    AI_Npc** ai_infoMap;

    int max_clnt_num;
    //Comment: 전체 발송용 배열
    int* clnt_socks;

    int max_obj_num;

    std::random_device rand_dv;
    std::mt19937* gen;
    std::uniform_int_distribution<int>* rand_world_pos;

    Logger* log;

    //Todo: 나중에 분리하기
    Obj_Position RandomObjPos();
    Object_Info* CreateObject_Info(Obj_Position pos);
    AI_Npc* CreateAI_Npc(Object_Info* info, Object_Rule* rule);

    AI_Npc* FindAI_Npc(int id);
    Object_Info* FindObjInfo(int id);

    void AddUpdate(Object_Info* info);

    //Comment: 조작된 정보인지 유효한 데이터인지 체크.
    //return true if validate
    bool CheckValidateObjInfo(timeval& nowtime, Object_Info* newObjInfo);

    void UpdateNpcEndEvents(timeval& nowtime);

    void MoveObject(timeval& nowtime, Object_Info* info);
    //Todo: 위치순으로 정렬해놓은 데이터 테이블이 필요함. x,y 각각.
    //Todo: 인자도 이게 맞는지 다시 체크
    void CheckObjectEvent();

    //Todo: AI_Npc와 Object_Info 상태를 결정짓는 함수가 필요함.
    void NextFrame(timeval& nowtime);

    void MakeUpdateObjectPacket();
    void SendUpdateObject_Info(int sock);

    double GetObjPosDistance(Obj_Position pos1, Obj_Position pos2);

    //Comment: oldObjInfo는 움직이기 전의 위치, newObjInfo는 움직임을 전달받은 클라이언트의 위치.
    void DeadReckoning(Object_Info* oldObjInfo, Object_Info* nowObjInfo, Object_Info* newObjInfo);
    bool SyncPos(Object_Info* newObjInfo);

    void LogObjInfo(Object_Info* info);

public:
    TcpGameRoom();
    ~TcpGameRoom();

    //Comment: 클라이언트의 입력신호
    void InterruptEvent(timeval& nowtime, Object_Info info);
    void InitGame(int room_num, Object_Rule obj_rule, int npc_num, int clnt_num, int* clnt_socks);
    void StartGame(timeval& nowtime);
    void update(timeval& nowtime);
    void EndGame(timeval& nowtime);
};

#endif