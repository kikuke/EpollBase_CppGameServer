#ifndef OBJECT_EVENT
#define OBJECT_EVENT

#include <time.h>

#include "ObjectStruct.h"
#include "sptime.h"

class NpcEndEvent
{
private:
    Object_Info* obj_info;
    timeval end_time;

public:
    Object_Info* getObjInfo() const {
        return obj_info;
    }
    timeval* getEndTime() {
        return &end_time;
    }

    void set(Object_Info* obj_info, timeval end_time){
        this->obj_info = obj_info;
        this->end_time = end_time;
    }
    bool operator ==(const NpcEndEvent& event) const {
        return (getTimeDist(&(this->end_time), &(event.end_time)) == 0);
    }
    bool operator <(const NpcEndEvent& event) const {
        return (getTimeDist(&(this->end_time), &(event.end_time)) > 0);
    }
    bool operator >(const NpcEndEvent& event) const {
        return (getTimeDist(&(this->end_time), &(event.end_time)) < 0);
    }
};

#endif
