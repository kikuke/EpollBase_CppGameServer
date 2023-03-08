#include <cmath>

#include "ServerDefine.h"
#include "PacketDefine.h"
#include "PacketGenerator.h"
#include "sptime.h"
#include "PacketIO.h"
#include "ThreadSafe/TSRingBuffer.h"
#include "TcpGameRoom.h"

TcpGameRoom::TcpGameRoom(JobQueue* jobQueue)
{
    this->jobQueue = jobQueue;

    broadCastBuffer = new TSRingBuffer;
    gen = new std::mt19937(rand_dv());
    rand_world_pos = new std::uniform_int_distribution<int>(-99, 99);
}

TcpGameRoom::~TcpGameRoom()
{
    delete broadCastBuffer;
    delete gen;
    delete rand_world_pos;
}

//Todo: 재사용 할수 있는 로직으로 바꾸기 잦은 new delete줄이기
void TcpGameRoom::InitGame(int room_id, Object_Rule obj_rule, int npc_num, int clnt_num, int* clnt_socks)
{
    Object_Info* info;

    this->room_id = room_id;
    this->obj_idCnt = 0;
    this->obj_rule = obj_rule;
    this->max_npc_num = npc_num;
    this->max_clnt_num = clnt_num;
    this->clnt_socks = clnt_socks;
    this->max_obj_num = npc_num + clnt_num;

    //Todo: delete 해주기
    log = new Logger("GameRoom " + room_id);

    //Todo: delete 해주기
    isUpdateId = new bool[max_obj_num];

    //Todo: 안의 포인터들 Null로 만들고 delete로 삭제. 다른 것들중에 이런것 있나 체크하기.
    obj_nowInfoMap = new Object_Info*[max_obj_num];
    ai_infoMap = new AI_Npc*[max_npc_num];

    //Todo: 종료시에 꼭 삭제하거나 재활용하게끔 바꿔주기
    obj_oldInfoMap = new Object_Info*[max_obj_num];
    for(int i=0; i<max_obj_num; i++){
        obj_oldInfoMap[i] = new Object_Info;
    }

    npc_pool = new AI_Npc*[max_npc_num];
    for(int i=0; i<max_npc_num; i++){
        info = CreateObject_Info(RandomObjPos());

        npc_pool[i] = CreateAI_Npc(info, &(this->obj_rule));
    }

    for(int i=0; i<clnt_num; i++){
        info = CreateObject_Info(RandomObjPos());
        info->ctrl.isPlayable = true;
    }
}

void TcpGameRoom::StartGame(timeval& nowtime)
{
    NpcEndEvent event;
    Object_Info* info;
    timeval endTime;

    //Todo: 오브젝트들이 겹칠경우 서로 조금씩 밀어내는 식으로?
    //Comment: npc들에게 초기 행동을 부여 후 정보를 가져와 events에 푸시
    for(int i=0; i<max_npc_num; i++){
        info = npc_pool[i]->getObjInfo();
        info->ctrl.isDead = false;
        
        //Todo: UpdateNpcEndEvent에서의 코드 중복 함수 만들기

        endTime = npc_pool[i]->action();
        event.set(info, endTime);
        npc_end_events.push(event);
    }

    for(int i=0; i<max_obj_num; i++){
        AddUpdate(obj_nowInfoMap[i]);
    }
    //Todo: 초기데이터 전송
}

//Todo: 전달되는 info값이 포인터이므로 누수되지 않게 확인하기
//Comment: 변경되는 이벤트들에 대한 처리. 새로운 입력에 대한 처리임.
//Comment: AI입력도 여기에 넣기
//Comment: 즉각 반영 되진 않고 매 프레임마다 실행되는 update때 반영되게끔.
//Comment: nowObjInfo에 데이터를 갱신함.
void TcpGameRoom::InterruptEvent(timeval& nowtime, Object_Info info)
{
    if(!CheckValidateObjInfo(nowtime, &info)){
        (*log).Log(LOGLEVEL::WARNING, "[%s] Invalidate Info - ID: %d", "GameRoom " + room_id, info.id);
        return;
    }

    if(SyncPos(&info)){
        info.pos = obj_nowInfoMap[info.id]->pos;

        (*log).Log(LOGLEVEL::DEBUG, "[%s] Dead Reckoning Ocurred - ID: %d", "GameRoom " + room_id, info.id);
    }

    AddUpdate(&info);
}

void TcpGameRoom::MoveObject(timeval& nowtime, Object_Info* info)
{
    double moveTime = getTimeDist(&(info->st_time), &nowtime);

    //Todo: 움직였으면 타임 초기화
    info->pos.x += info->force.x * moveTime;
    info->pos.y += info->force.y * moveTime;
    
    info->st_time = nowtime;
}

double TcpGameRoom::GetObjPosDistance(Obj_Position pos1, Obj_Position pos2)
{
    return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}

//Todo: oldObjInfo 배열 만들어놓기
//Todo: info는 이 함수를 사용하기 전에 시간에 맞춰 예상해 업데이트 되어있다는 것이 전제임.
//Comment: 많이 움직이게 되면 값들을 조정함.
void TcpGameRoom::DeadReckoning(Object_Info* oldObjInfo, Object_Info* nowObjInfo, Object_Info* newObjInfo)
{
    Obj_Position bef_velocity;
    Obj_Position aft_velocity;
    Obj_Position accel;
    double distTime;

    distTime = getTimeDist(&(oldObjInfo->st_time), &(newObjInfo->st_time));

    bef_velocity = (nowObjInfo->pos - oldObjInfo->pos)/distTime;
    aft_velocity = (newObjInfo->pos - oldObjInfo->pos)/distTime;
    accel = (aft_velocity - bef_velocity)/distTime;

    nowObjInfo->pos = oldObjInfo->pos + bef_velocity*distTime + (accel)*pow(distTime,2)*0.5f;
}

bool TcpGameRoom::SyncPos(Object_Info* newObjInfo)
{
    Object_Info* info = obj_nowInfoMap[newObjInfo->id];

    if(GetObjPosDistance(info->pos, newObjInfo->pos) < DEADRECKONING_THRESHOLD){
        return false;
    }
    DeadReckoning(obj_oldInfoMap[newObjInfo->id], info, newObjInfo);

    return true;
}

//Todo: 충돌 관련 처리 구상
//Todo: AI의 동작이 바뀌게 될경우 큐에서 빼주기
void TcpGameRoom::CheckObjectEvent()
{
    for(int i=0; i<max_obj_num; i++){

    }
}

void TcpGameRoom::NextFrame(timeval& nowtime)
{
    //Todo: 한번 쭉 이동시켜보고 이전 정보에 따라 충돌난 것들 처리해주기
    for(int i=0; i<max_obj_num; i++){
        MoveObject(nowtime, obj_nowInfoMap[i]);
    }

    CheckObjectEvent();
}

void TcpGameRoom::AddUpdate(Object_Info* info)
{
    (*obj_oldInfoMap[info->id]) = (*obj_nowInfoMap[info->id]);
    (*obj_nowInfoMap[info->id]) = *info;
    isUpdateId[info->id] = true;

    //LogObjInfo(info);
}

//Todo: 이거 잘못됨. 포인터를 고정으로 바꾸거나 포인터가리키는곳 복사하기
size_t UpdateObjectPacketFactory(void* buf, GameRoomUpdateObjectData* data)
{
    size_t packetLen = 0;
    const size_t dataInfoLen = sizeof(unsigned int);
    const size_t allDataSz = sizeof(OBJECT_DATA)*(data->update_obj_num);
    const unsigned char endCode = TCP_PACKET_END_CODE;

    TCPTestPacketHeader header = {TCP_PACKET_START_CODE, (unsigned int)(dataInfoLen + allDataSz + sizeof(unsigned char)),
        GAMEROOM, GAMEROOM_UPDATE_OBJECT, 0x000, 0x000};

    memcpy((unsigned char*)buf + packetLen, &header, sizeof(header));
    packetLen += sizeof(header);

    memcpy((unsigned char*)buf + packetLen, data, dataInfoLen);
    packetLen += dataInfoLen;

    memcpy((unsigned char*)buf + packetLen, data->objs_data, allDataSz);
    packetLen += allDataSz;

    memcpy((unsigned char*)buf + packetLen, &endCode, sizeof(unsigned char));
    packetLen += sizeof(unsigned char);

    return packetLen;
}

//Todo: 업데이트된 id들 패킷으로 만들기
void TcpGameRoom::SendUpdateObjectPacket()
{
    size_t len;
    int update_obj_num;
    GameRoomUpdateObjectData data;

    data.room_id = this->room_id;
    update_obj_num = 0;
    for(int i=0; i<max_obj_num; i++){
        if(isUpdateId[i]){
            update_obj_num++;
        }
    }
    
    if(update_obj_num <= 0)
        return;

    data.update_obj_num = update_obj_num;
    data.objs_data = new OBJECT_DATA[update_obj_num];

    update_obj_num = 0;
    for(int i=0; i<max_obj_num; i++){
        if(isUpdateId[i]){
            data.objs_data[update_obj_num++].info = *(obj_nowInfoMap[i]);
        }
    }

    for(int i=0; i<max_obj_num; i++){
        isUpdateId[i] = false;
    }

    //Todo: 패킷화, 전체 발송용 버퍼 설정하기
    len = UpdateObjectPacketFactory(buf, &data);
    broadCastBuffer->enqueue(buf, len);

    jobQueue->broadcastQueue.push(this->room_id);

    delete data.objs_data;
}

RingBuffer* TcpGameRoom::getBroadCastBuffer()
{
    return broadCastBuffer;
}

int TcpGameRoom::getClientNum()
{
    return max_clnt_num;
}

int* TcpGameRoom::getClientSocks()
{
    return clnt_socks;
}

//Comment: 매프레임마다 실행.
//Comment: 이건 단순히 정보 계산만 해주는 것임. 최종 업데이트 된 정보는 updateObjInfo를 통해 전달되고, nowObjInfo에 다시 저장되어 서버가 갱신된 정보들을 들고있음.
//Comment: nowObjInfo에 있는 정보들을 토대로 다시 정보들을 업데이트 시킴
void TcpGameRoom::update(timeval& nowtime)//Todo: 함수 분리
{
    //Todo: 충돌 계산, 보간 등 이벤트 처리.
    NextFrame(nowtime);

    UpdateNpcEndEvents(nowtime);

    SendUpdateObjectPacket();
}


//Comment: 동작의 시간이 끝났을 경우임.
//Comment: AI일 경우 처리와 행동 재시작
void TcpGameRoom::UpdateNpcEndEvents(timeval& nowtime)
{
    NpcEndEvent event;
    Object_Info* info;
    AI_Npc* npc;
    timeval endTime;

    double interval;

    while(!npc_end_events.empty())
    {
        event = npc_end_events.top();
        info = event.getObjInfo();

        interval = getTimeDist(event.getEndTime(), &nowtime);
        if(interval >= 0){
            npc_end_events.pop();

            npc = FindAI_Npc(info->id);
            //Todo: 이거 세개 묶어서 함수로 만들지 고민
            endTime = npc->action();

            AddUpdate(info);

            event.set(info, endTime);
            npc_end_events.push(event);
            continue;
        }

        break;
    }
}

void TcpGameRoom::EndGame(timeval& nowtime)
{
    //Todo: 재사용하게 원상태로 되돌리는 로직
}

//Comment: ObjectRule에 의한 판단.
bool TcpGameRoom::CheckValidateObjInfo(timeval& nowtime, Object_Info* newObjInfo)
{
    //Todo: 이런식으로 이전 데이터 비교해서 유효성 검사
    //Todo: 플레이어쪽에서 전송한 현재 오브젝트의 이동정보가 지금 속도와 일치하는지 체크.
    if(obj_nowInfoMap[newObjInfo->id]){

    }

    return true;
}

Object_Info* TcpGameRoom::FindObjInfo(int id)
{
    return obj_nowInfoMap[id];
}

AI_Npc* TcpGameRoom::FindAI_Npc(int id)
{
    return ai_infoMap[id];
}

AI_Npc* TcpGameRoom::CreateAI_Npc(Object_Info* info, Object_Rule* rule)
{
    AI_Npc* npc = new AI_Npc(info, rule);

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

    obj_oldInfoMap[info->id] = info;
    obj_nowInfoMap[info->id] = info;

    return info;
}

Obj_Position TcpGameRoom::RandomObjPos()
{
    return {(float)((*rand_world_pos)(*gen)), (float)((*rand_world_pos)(*gen)), 0};
}

void TcpGameRoom::LogObjInfo(Object_Info* info)
{
    //Todo: State 오버플로 체크
    (*log).Log(LOGLEVEL::INFO, "[%s] Object Update - ID: %d, StartTime: %d, State: %d\n\
    Pos: (%f, %f, %f), Force: (%f, %f, %f)",\
    "GameRoom " + room_id, info->id, info->st_time, info->state,\
    info->pos.x, info->pos.y, info->pos.z, info->force.x, info->force.y, info->force.z);
}