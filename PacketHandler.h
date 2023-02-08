#ifndef PACKET_HANDLER
#define PACKET_HANDLER

#include "RingBuffer.h"

class PacketHandler
{
private:
    const unsigned int mainOp;
public:
    PacketHandler(unsigned int _mainOp) : mainOp(_mainOp){}
    PacketHandler(PacketHandler* target) : mainOp(target->mainOp){}

    unsigned int getMainOp() {
        return mainOp;
    }

    virtual int execute(unsigned int subOp, RingBuffer& buffer){
        //로그찍기나 패킷체크등 동작 생각해보기. 없어도됨.
        return 1;
    }
};

#endif