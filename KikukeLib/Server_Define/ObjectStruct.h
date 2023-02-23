#ifndef OBJECT_STRUCT
#define OBJECT_STRUCT

#include <time.h>

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

struct Obj_StateTime{
    timeval start_time;
    timeval end_time;
};

struct Object_Info
{
    int id;

    Obj_Position pos;
    Obj_Force force;
    Obj_State state;
    Obj_StateTime st_time;
};

#endif