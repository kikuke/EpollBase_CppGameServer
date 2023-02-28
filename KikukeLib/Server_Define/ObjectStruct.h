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

    Obj_Position operator +(const Obj_Position& target){
        return {this->x + target.x, this->y + target.y, this->z + target.z};
    }

    Obj_Position operator -(const Obj_Position& target){
        return {this->x - target.x, this->y - target.y, this->z - target.z};
    }

    Obj_Position operator *(const float target){
        return {this->x * target, this->y * target, this->z * target};
    }

    Obj_Position operator /(const float target){
        return {this->x / target, this->y / target, this->z / target};
    }
};

struct Obj_Force
{
    float x, y, z;

    Obj_Force operator +(const Obj_Force& target){
        return {this->x + target.x, this->y + target.y, this->z + target.z};
    }

    Obj_Force operator -(const Obj_Force& target){
        return {this->x - target.x, this->y - target.y, this->z - target.z};
    }

    Obj_Force operator *(const float target){
        return {this->x * target, this->y * target, this->z * target};
    }

    Obj_Force operator /(const float target){
        return {this->x / target, this->y / target, this->z / target};
    }
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