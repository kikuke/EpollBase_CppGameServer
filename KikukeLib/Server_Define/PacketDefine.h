#ifndef PACKET_DEFINE
#define PACKET_DEFINE

#include "ObjectStruct.h"

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

//OBJECT_PAKCET_DATA
struct OBJECT_DATA
{
    Object_Info info;
};

//GAMEROOM

//GAMEROOM MAIN_OP 4Bit
#define GAMEROOM 0x5

//GAMEROOM SUB_OP 8Bit
#define GAMEROOM_CREATE 0xFE

//GAMEROOM_UPDATE_OBJECT PACKET_DATA
struct GameRoomCreateData
{
    Object_Rule rule;
    int npc_num;
    int clnt_num;
    int* clnt_id;
};//8 + sizeof(Object_Rule) + (4 * clnt_num) Byte

//GAMEROOM SUB_OP 8Bit
#define GAMEROOM_UPDATE_OBJECT 0x03

//Todo: 게임룸 번호도 넣어서 전달.

//GAMEROOM_UPDATE_OBJECT PACKET_DATA
struct GameRoomUpdateObjectData
{
    unsigned int room_id : 16;//방번호
    unsigned int update_obj_num : 16;//업데이트된 오브젝트 개수//4Byte
    OBJECT_DATA* objs_data;
};//4Byte + (sizeof(OBJECT_DATA) * update_obj_num) Byte

//PLAYER

//PLAYER MAIN_OP 4Bit
#define PLAYER 0x6

//PLAYER SUB_OP 8Bit
#define PLAYER_SET_ID_REQUEST 0x01
#define PLAYER_SET_ID_OK 0x02

//Todo: 게임룸 번호도 넣어서 전달. 근데 얘가 게임룸에 들어가는게 맞을수도 있음. 아니면 플레이어 입력 취급이니 플레이어로.

//PLAYER_UPDATE_OBJECT PACKET_DATA
struct PlayerSetIdRequestData
{
    int id;
};//4Byte

//PLAYER SUB_OP 8Bit
#define PLAYER_UPDATE_OBJECT 0x03

//Todo: 게임룸 번호도 넣어서 전달. 근데 얘가 게임룸에 들어가는게 맞을수도 있음. 아니면 플레이어 입력 취급이니 플레이어로.

//PLAYER_UPDATE_OBJECT PACKET_DATA
struct PlayerUpdateObjectData
{
    unsigned int room_id : 16;//방번호
    unsigned int : 16;

    OBJECT_DATA obj_data;
};//4Byte + sizeof(OBJECT_DATA) Byte

//Disconnect
//TCP Message MAIN_OP 4Bit
#define DISCONNECT 0xF

//TCP Message SUB_OP 8Bit
#define DISCONNECT_OK 0x00
#define DISCONNECT_ERROR 0x01

//TCP Message PACKET_DATA
struct DisconnectData
{
    unsigned int temp : 8;//미사용
    unsigned int : 24;//4Byte
};//4Byte

//Message
//TCP Message MAIN_OP 4Bit
#define MESSAGE 0x3

//TCP Message SUB_OP 8Bit
#define MESSAGE_ECHO 0x07

//TCP Message PACKET_DATA
struct MessageEchoData
{
    unsigned int target : 8;//전체 말인지, 혼잣말인지
    unsigned int : 24;//4Byte

    unsigned char msg[100];//100Byte
};//104Byte
//TCP Message PACKET_DATA target Code
#define MESSAGE_ECHO_DATA_TARGET_SELF 0x01
#define MESSAGE_ECHO_DATA_TARGET_ALL 0x02

#endif