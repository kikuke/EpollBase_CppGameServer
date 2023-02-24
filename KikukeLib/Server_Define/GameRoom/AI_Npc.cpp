#include <sys/time.h>
#include <math.h>

#include "AI_Npc.h"

AI_Npc::AI_Npc(Object_Info* info)
{
    this->info = info;

    gen = new std::mt19937(rand_dv());
    rand = new std::uniform_int_distribution<int>(0, 99);
}

AI_Npc::~AI_Npc()
{
    delete info;
    delete gen;
    delete rand;
}

Object_Info& AI_Npc::getInfo(timeval& nowtime)
{
    if(info->state == Obj_State::MOVE){
        double actionTime = getTimeDist(info->st_time.start_time, info->st_time.end_time);
        double elapsed = getTimeDist(info->st_time.start_time, nowtime);

        if(elapsed < actionTime)
            actionTime = elapsed;

        info->pos.x += info->force.x * actionTime;
        info->pos.y += info->force.y * actionTime;
    }
    return *info;
}

//Todo: Get해서 정보를 가져오는 순간, 또는 끝날때 시간 계산해서 현재 위치(pos) 반환하기 근데 어차피 이걸 호출하는 경우는 끝났거나 이벤트 발생했거나라서 정보 가져갈때 갱신해주면 됨.
//Todo: priorityqueue?
//Todo: Attack은 나중에 거리 체크해서 실행하는 것으로. 또는 게임룸에서 Npc에게 원격으로 실행?
timeval* AI_Npc::action()
{
    info->state = RandomState();
    switch (info->state)
    {
    case IDLE:
        if(!Idle())
            return nullptr;
        break;

    case MOVE://Todo: 움직임 길이는 로그로 해서 맨 앞값이 잘나오도록. 방향은 균등하게
        if(!Move())
            return nullptr;
        break;

    default:
        return nullptr;
        break;
    }

    return &(info->st_time.end_time);
}

bool AI_Npc::Idle()
{
    int val = (*rand)(*gen);
    //Comment: 최소 1에서 최대 2
    val = log10(val + 10);

    return SetStateTime(val);
}

bool AI_Npc::Move()
{
    int time = (*rand)(*gen);
    float speed = (*rand)(*gen);

    SetRandomNormVector();
    //Comment: 최소 1에서 최대 2
    speed = log10(time + 10);

    info->force.x *= speed;
    info->force.y *= speed;

    //Comment: 최소 1에서 최대 3
    time = log10((time * 10) + 10);

    return SetStateTime(time);
}

Obj_State AI_Npc::RandomState()
{
    int val = (*rand)(*gen);
    
    if(val < 30){
        return Obj_State::IDLE;
    }

    return Obj_State::MOVE;
}

void AI_Npc::SetRandomNormVector()
{
    //Coment: radian으로 변환
    float vec_val = (*rand)(*gen) * 1.8f * 3.14f / 180;
    float sin_val = sin(vec_val);
    float cos_val = cos(vec_val);

    //Comment: 삼각함수 공식을 이용한 이동 방향 지정 x, y의 총합 1이됨.
    info->force.x = pow(sin_val, 2);
    if(sin_val<0){
        info->force.x *= -1;
    }
    info->force.y = pow(cos_val, 2);
    if(cos_val<0){
        info->force.y *= -1;
    }
}

double getTimeDist(timeval& start, timeval& end)
{
    double dist;

    dist = (end.tv_sec - start.tv_sec);
    dist += (end.tv_usec - start.tv_usec) / 1000000;

    return dist;
}

bool AI_Npc::SetStateTime(int stateTime)
{
    if(!SetNowTime(&(info->st_time.start_time)))
        return false;
    info->st_time.end_time.tv_sec = info->st_time.start_time.tv_sec + stateTime;
    
    return true;
}

bool AI_Npc::SetNowTime(timeval* target)
{
    if(gettimeofday(target, NULL)!=0){
        return false;
    }

    return true;
}