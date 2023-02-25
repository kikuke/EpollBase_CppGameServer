
#include "sptime.h"
#include "TcpGameRoom.h"

TcpGameRoom::TcpGameRoom()
{
    gen = new std::mt19937(rand_dv());
    rand_world_pos = new std::uniform_int_distribution<int>(-99, 99);
}

TcpGameRoom::~TcpGameRoom()
{
    delete gen;
    delete rand_world_pos;
}

//Todo: 재사용 할수 있는 로직으로 바꾸기 잦은 new delete줄이기
void TcpGameRoom::InitGame(int room_num, int npc_num, int clnt_num, int* clnt_socks)
{
    Object_Info* info;

    this->room_num = room_num;
    this->obj_idCnt = 0;
    this->max_npc_num = npc_num;
    this->max_clnt_num = clnt_num;
    this->clnt_socks = clnt_socks;
    this->max_obj_num = npc_num + clnt_num;

    //Todo: delete 해주기
    log = new Logger("GameRoom " + room_num);

    //Todo: 안의 포인터들 Null로 만들고 delete로 삭제. 다른 것들중에 이런것 있나 체크하기.
    obj_infoMap = new Object_Info*[max_obj_num];
    ai_infoMap = new AI_Npc*[max_npc_num];

    //Todo: 안의 포인터들 Null로 만들고 delete로 삭제. 다른 것들중에 이런것 있나 체크하기.
    nowObjInfo = new Object_Info*[max_obj_num];
    updateObjInfo = new Object_Info*[max_obj_num];

    for(int i=0; i<clnt_num; i++){
        info = CreateObject_Info(RandomObjPos());
        info->ctrl.isPlayable = true;
    }

    npc_pool = new AI_Npc*[max_npc_num];
    for(int i=0; i<max_npc_num; i++){
        info = CreateObject_Info(RandomObjPos());

        //Todo: 나중에 speed같은 애들 설정하는 구조체 만들기
        npc_pool[i] = CreateAI_Npc(info, 3);
    }
}

void TcpGameRoom::StartGame(timeval& nowtime)
{
    ObjectEvent event;
    Object_Info* info;

    //Todo: 오브젝트들이 겹칠경우 서로 조금씩 밀어내는 식으로?
    //Comment: npc들에게 초기 행동을 부여 후 정보를 가져와 events에 푸시
    for(int i=0; i<max_npc_num; i++){
        npc_pool[i]->action();

        info = npc_pool[i]->update(nowtime);
        info->ctrl.isDead = false;
        event.set(info);
        obj_end_events.push(event);
    }

    //Todo: 초기데이터 전송
}

//Comment: 변경되는 이벤트들에 대한 처리. 새로운 입력에 대한 처리임.
//Comment: AI입력도 여기에 넣기
//Comment: 즉각 반영 되진 않고 매 프레임마다 실행되는 update때 반영되게끔.
//Comment: nowObjInfo에 데이터를 갱신함.
void TcpGameRoom::InterruptEvent(timeval& nowtime, Object_Info* info)
{
    if(!CheckValidateObjInfo(nowtime, info)){
        (*log).Log(LOGLEVEL::WARNING, "[%s] Invalidate Info - ID: %d", "GameRoom " + room_num, info->id);
        return;
    }

    //Todo: end에서 정보들 꺼내와서 갱신하기.
}

//Todo: 계산을 통해 기존의 예상 값들이 변경됐다면 이것도 다시 전송하기.

//Comment: 이건 단순히 정보 계산만 해주는 것임. 최종 업데이트 된 정보는 updateObjInfo를 통해 전달되고, nowObjInfo에 다시 저장되어 서버가 갱신된 정보들을 들고있음.
//Comment: nowObjInfo에 있는 정보들을 토대로 다시 정보들을 업데이트 시킴
void TcpGameRoom::update(timeval& nowtime)//Todo: 함수 분리
{
    ObjectEvent event;
    Object_Info* info;
    AI_Npc* npc;
    while(!obj_end_events.empty())
    {
        event = obj_end_events.top();
        info = event.get();

        //Comment: 동작의 시간이 끝났을 경우임.
        if(getTimeDist(&(info->st_time.end_time), &nowtime) >= 0){
            obj_end_events.pop();

            //Todo: 충돌 계산, 보간 등 이벤트 처리.

            //Todo: npc의 action과 update 합쳐서 액션 발생 클라이언트들에게 전송하는 함수 만들기
            //Comment: AI일 경우 처리와 행동 재시작
            if(info->ctrl.isPlayable){
                info->state = Obj_State::IDLE;
            }
            else{
                npc = FindAI_Npc(info->id);

                //Todo: 이거 세개 묶어서 함수로 만들지 고민
                npc->action();
                npc->update(nowtime);
                InterruptEvent(nowtime, info);

                //Comment: 이함수는 마지막에 넣기
                //AddUpdateInfo(info);
                //obj_end_events.push(event);
            }

            //Todo: 함수로 따로 분리해서 시간 다시 잘 표시하기
            (*log).Log(LOGLEVEL::INFO, "[%s] Object Update - ID: %d, State: %d\n\
            StartTime: %d, EndTime: %d\n\
            Pos: (%f, %f, %f), Force: (%f, %f, %f)",\
            "GameRoom " + room_num, info->id, info->state,\
            info->st_time.start_time.tv_sec, info->st_time.end_time.tv_sec,
            info->pos.x, info->pos.y, info->pos.z, info->force.x, info->force.y, info->force.z);
            continue;
        }

        break;
    }

    //Todo: 업데이트된 데이터들 전송
}

void TcpGameRoom::EndGame(timeval& nowtime)
{
    //Todo: 재사용하게 원상태로 되돌리는 로직
}

void TcpGameRoom::SendUpdateObject_Info(int sock)
{
    //Todo: 이런식으로 구상하기
    //memcpy(,,updateNum*sizeof())
}

bool TcpGameRoom::CheckValidateObjInfo(timeval& nowtime, Object_Info* newObjInfo)
{
    //Todo: 이런식으로 이전 데이터 비교해서 유효성 검사
    //Todo: 플레이어쪽에서 전송한 현재 오브젝트의 이동정보가 지금 속도와 일치하는지 체크.
    if(nowObjInfo[newObjInfo->id]){

    }

    return true;
}

void TcpGameRoom::AddUpdateInfo(Object_Info* info)
{
    updateObjInfo[updateNum] = info;
    updateNum++;
}

Object_Info* TcpGameRoom::FindObjInfo(int id)
{
    return obj_infoMap[id];
}

AI_Npc* TcpGameRoom::FindAI_Npc(int id)
{
    return ai_infoMap[id];
}

AI_Npc* TcpGameRoom::CreateAI_Npc(Object_Info* info, float speed)
{
    AI_Npc* npc = new AI_Npc(info, speed);

    ai_infoMap[info->id] = npc;

    return npc;
}

Object_Info* TcpGameRoom::CreateObject_Info(Obj_Position pos)
{
    Object_Info* info = new Object_Info;
    info->id = obj_idCnt++;
    info->ctrl.isDead = true;
    info->ctrl.isPlayable = false;
    info->pos = pos;
    info->force = {0};
    info->state = Obj_State::IDLE;
    info->st_time = {0};

    obj_infoMap[info->id] = info;

    return info;
}

Obj_Position TcpGameRoom::RandomObjPos()
{
    return {(float)((*rand_world_pos)(*gen)), (float)((*rand_world_pos)(*gen)), 0};
}