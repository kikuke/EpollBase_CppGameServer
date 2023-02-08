#ifndef PACKET_HANDLER
#define PACKET_HANDLER

#include "RingBuffer.h"

class PacketHandler
{
private:
    const unsigned int mainOp;

protected:
    //abstract data, return endCode.
    template <typename T>
    unsigned char DequeueData(T& data, RingBuffer& buffer){
        unsigned char endCode;
        data << buffer;
        endCode << buffer;

        return endCode;
    }

public:
    PacketHandler(unsigned int _mainOp) : mainOp(_mainOp){}

    unsigned int getMainOp() {
        return mainOp;
    }

    virtual int execute(int sock, unsigned int subOp, RingBuffer& buffer) = 0;
    virtual int catchError(int sock, unsigned int errorCode) = 0;
};

#endif