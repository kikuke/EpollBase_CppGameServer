#ifndef PACKET_DEFINE
#define PACKET_DEFINE

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

//TCP CODE 1Byte
#define TCP_PACKET_START_CODE 0x77
#define TCP_PACKET_END_CODE 0x33

//TCP MAIN_OP 4Bit
#define MESSAGE 3

//TCP SUB_OP 8Bit
#define MESSAGE_ECHO 7

//TCP PACKET_DATA
struct MessageEchoData
{
    unsigned int target : 8;//전체 말인지, 혼잣말인지
    unsigned int : 24;//4Byte

    unsigned char msg[100];//100Byte
};//104Byte
#define MESSAGE_ECHO_DATA_TARGET_SELF 1
#define MESSAGE_ECHO_DATA_TARGET_ALL 2

#endif