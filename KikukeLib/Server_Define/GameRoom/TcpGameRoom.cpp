
#include "sptime.h"
#include "TcpGameRoom.h"

TcpGameRoom::TcpGameRoom()
{
    gen = new std::mt19937(rand_dv());
    rand = new std::uniform_int_distribution<int>(0, 99);
}

TcpGameRoom::~TcpGameRoom()
{
    delete gen;
    delete rand;
}

//Todo: 재사용 할수 있는 로직으로 바꾸기 잦은 new delete줄이기
void TcpGameRoom::InitGame(int room_num, int max_npc_num, int clnt_num, int* clnt_socks)
{
    Object_Info* info;

    this->room_num = room_num;
    this->max_npc_num = max_npc_num;

    npc_pool = new AI_Npc*[max_npc_num];
    for(int i=0; i<max_npc_num; i++){
        info = CreateObject_Info(RandomObjPos());
        npc_pool[i] = CreateAI_Npc(info);
    }
}

void TcpGameRoom::StartGame(timeval& nowtime)
{
    ObjectEvent event;

    //Comment: npc들에게 초기 행동을 부여 후 정보를 가져와 events에 푸시
    for(int i=0; i<max_npc_num; i++){
        npc_pool[i]->action();
        event.set(npc_pool[i]->update(nowtime));
        obj_events.push(event);
    }
}

void TcpGameRoom::update(timeval& nowtime)//Todo: 함수 분리
{
    ObjectEvent event;
    Object_Info* info;
    AI_Npc* npc;
    while(!obj_events.empty())
    {
        event = obj_events.top();
        info = event.get();

        if(getTimeDist(&(info->st_time.end_time), &nowtime) >= 0){
            obj_events.pop();

            //Todo: 이벤트 처리

            //Todo: npc의 action과 update 합쳐서 액션 발생 클라이언트들에게 전송하는 함수 만들기
            //Comment: AI일 경우 처리와 행동 재시작
            if(!(info->ctrl.isPlayable)){
                npc = FindAI_Npc(info->id);
                npc->action();
                npc->update(nowtime);

                obj_events.push(event);
            }
            continue;
        }

        break;
    }
}