#ifndef AI_NPC
#define AI_NPC

#include <time.h>

#include "ObjectStruct.h"
class AI_Npc
{
private:
    Object_Inform obj;

    timeval GetNowTime();
public:
    AI_Npc();
    AI_Npc(Obj_Position pos);//Todo: 시간은 자동으로 초기화 되게끔
    ~AI_Npc();
};

#endif