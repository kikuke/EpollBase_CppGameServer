#ifndef OBJECT_EVENT
#define OBJECT_EVENT

#include "ObjectStruct.h"
#include "sptime.h"

class ObjectEvent
{
private:
    Object_Info* obj_info;

public:
    bool operator ==(const ObjectEvent& event) const {
        return (getTimeDist(&(this->obj_info->st_time.end_time), &(event.obj_info->st_time.end_time)) == 0);
    }
    bool operator <(const ObjectEvent& event) const {
        return (getTimeDist(&(this->obj_info->st_time.end_time), &(event.obj_info->st_time.end_time)) > 0);
    }
    bool operator >(const ObjectEvent& event) const {
        return (getTimeDist(&(this->obj_info->st_time.end_time), &(event.obj_info->st_time.end_time)) < 0);
    }
};

#endif
