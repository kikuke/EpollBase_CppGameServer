#ifndef OBJECT_STRUCT
#define OBJECT_STRUCT

#include <time.h>

struct Obj_Control
{
    bool isPlayable;
    bool isDead;
};

struct Obj_Position
{
    float x, y, z;
};

struct Obj_Force
{
    float x, y, z;
};

enum Obj_State{
    IDLE,
    MOVE,
    ATTACK,
    MAX_NUM
};

struct Object_Info
{
    int id;
    timeval st_time;
    
    Obj_Control ctrl;
    Obj_State state;
    Obj_Position pos;
    Obj_Force force;
};

struct Object_Rule
{
    float speed;
};

#endif