#ifndef PACKET_DEFINE
#define PACKET_DEFINE

//use BitField memcpy
struct TestPacketHeader
{
    unsigned int startCode : 8;
    unsigned int packetLen : 16;
    unsigned int mainOp : 4;
    unsigned int : 4;//4Byte

    unsigned int subOp : 8;
    unsigned int flag : 12;
    unsigned int : 12;//4Byte
};//8byte

#endif