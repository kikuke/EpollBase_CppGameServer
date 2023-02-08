#ifndef PACKET_DEFINE
#define PACKET_DEFINE

//TCP CODE
#define TCP_PACKET_START_CODE 0x77
#define TCP_PACKET_END_CODE 0x33

//TCP OP
#define MESSAGE 3
#define MESSAGE_ECHO 7

//TCP HEADER
struct TCPTestPacketHeader
{
    unsigned int startCode : 8;
    unsigned int packetLen : 16;
    unsigned int mainOp : 4;
    unsigned int : 4;//4Byte

    unsigned int subOp : 8;
    unsigned int flag : 12;
    unsigned int auth : 12;//4Byte
};//8byte

#endif