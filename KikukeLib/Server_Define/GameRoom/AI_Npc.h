#ifndef AI_NPC
#define AI_NPC

#include <time.h>
#include <random>

#include "ObjectStruct.h"

class AI_Npc//Todo: 게임룸에 큐같은거 만들어서 종료시간 순으로 빼내는거 구현하기
{
private:
    Object_Info* info;

    std::random_device rand_dv;
    std::mt19937* gen;
    std::uniform_int_distribution<int>* rand;

    //return success value
    bool SetNowTime(timeval* target);
    bool SetStateTime(int stateTime);
    void SetRandomNormVector();

    //Todo: 나중에 확률 쉽게 수정할수 있도록 변수 빼내기
    Obj_State RandomState();

    bool Idle();
    bool Move();

public:
    AI_Npc(Object_Info* info);//Todo: 시간은 자동으로 초기화 되게끔
    ~AI_Npc();

    //return end Time
    timeval* action();
    Object_Info& getInfo(timeval& nowtime);
};

#endif