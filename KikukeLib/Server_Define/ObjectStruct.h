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
    MOV,
    ATTACK
};

struct Object_Inform
{
    int id;
    timeval time;

    Obj_Position pos;
    Obj_Force force;
    Obj_State state;
};

#endif