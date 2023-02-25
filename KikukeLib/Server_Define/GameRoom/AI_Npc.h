#ifndef AI_NPC
#define AI_NPC

#include <time.h>
#include <random>

#include "ObjectStruct.h"

class AI_Npc
{
private:
    Object_Info* info;
    //Todo: 나중에 AI 설정정보 모아서 구조체로 만들어 일괄 전달 받기
    float speed;

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
    AI_Npc(Object_Info* info, Object_Rule* rule);//Todo: 시간은 자동으로 초기화 되게끔
    ~AI_Npc();

    //return end Time
    timeval* action();
    Object_Info* getObjInfo();
};

#endif