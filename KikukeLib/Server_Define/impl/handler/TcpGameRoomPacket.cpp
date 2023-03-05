#include <arpa/inet.h>

#include "SocketManager.h"
#include "TcpGameRoomPacket.h"

int TcpGameRoomPacket::execute(int sock, unsigned int subOp, RingBuffer& buffer)
{
    int room_id;
    GameRoomCreateData data;//지금은 case 전체가 사용할것처럼 밖으로 꺼내놨지만 subOp마다 다른 데이터들을 사용예정
    unsigned char endCode;
    switch (subOp)
    {
    case GAMEROOM_CREATE:
        buffer >> data.rule;
        buffer >> data.npc_num;
        buffer >> data.clnt_num;
        data.clnt_id = new int[data.clnt_num];
        buffer.dequeue(data.clnt_id, sizeof(int)*data.clnt_num);
        buffer >> endCode;

        if(endCode != TCP_PACKET_END_CODE){
            (*log).Log(LOGLEVEL::ERROR, "[%s] DequeueData() - DataBroken", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr));
            return 0;//Todo: 에러코드로 바꿔주기
        }

        //Todo: id를 소켓 번호로 변환하는 함수 만들기.
        
        room_id = gameRoomManager->OpenGameRoom(data.rule, data.npc_num, data.clnt_num, );
        (*log).Log(LOGLEVEL::INFO, "[%s] Open GameRoom id: %d", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr), room_id);
        delete data.clnt_id;

        return room_id;
        break;

    default:
        (*log).Log(LOGLEVEL::ERROR, "[%s] execute() - DataBroken", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr));
        return 0;
        break;
    }
    return 1;
}

int TcpGameRoomPacket::catchError(int sock, unsigned int errorCode)
{
    switch (errorCode)
    {
    default:
        (*log).Log(LOGLEVEL::ERROR, "[%s] catchError - Undefined ErrorCode: %d", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr), errorCode);
        return 0;
        break;
    }
    return 1;
}