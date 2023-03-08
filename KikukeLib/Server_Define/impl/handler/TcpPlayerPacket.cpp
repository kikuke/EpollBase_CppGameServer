#include <unistd.h>
#include <arpa/inet.h>

#include "SocketManager.h"
#include "PacketGenerator.h"
#include "TcpPlayerPacket.h"


int TcpPlayerPacket::execute(int sock, unsigned int subOp, RingBuffer& buffer)
{
    //Todo: 함수 분리하기
    const TCPTestPacketHeader setIdOK_header = {TCP_PACKET_START_CODE, sizeof(unsigned char),
        PLAYER, PLAYER_SET_ID_OK, 0x000, 0x000};
    size_t packet_len;

    PlayerSetIdRequestData data;//Todo: 지금은 case 전체가 사용할것처럼 밖으로 꺼내놨지만 subOp마다 다른 데이터들을 사용예정

    PlayerUpdateObjectData playerData;//Todo: 지금은 case 전체가 사용할것처럼 밖으로 꺼내놨지만 subOp마다 다른 데이터들을 사용예정
    TcpGameRoom* gameRoom;

    switch (subOp)
    {
    case PLAYER_SET_ID_REQUEST:
        if(DequeueData(data, buffer) != TCP_PACKET_END_CODE){
            (*log).Log(LOGLEVEL::ERROR, "[%s] DequeueData() - DataBroken", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr));
            return 0;//Todo: 에러코드로 바꿔주기
        }

        if(!SocketManager::getInstance().addID(sock, data.id))
        {
            (*log).Log(LOGLEVEL::ERROR, "[%s] AddID() - Add Id Failed", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr));
            return false;
        }

        //Comment: 템플릿으로 인해 필요없지만 data를 넣어둠. 패킷 처리 응답.
        packet_len = MakePacket(buf, &setIdOK_header, &data, 0, TCP_PACKET_END_CODE);
        write(sock, buf, packet_len);

        return true;
        break;

case PLAYER_UPDATE_OBJECT:
        if(DequeueData(playerData, buffer) != TCP_PACKET_END_CODE){
            (*log).Log(LOGLEVEL::ERROR, "[%s] DequeueData() - DataBroken", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr));
            return 0;//Todo: 에러코드로 바꿔주기
        }

        if((gameRoom = gameRoomManager->GetGameRoom(playerData.room_id)) == nullptr)
        {
            (*log).Log(LOGLEVEL::ERROR, "[%s] PlayerUpdateObject() - Can't find room: %d", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr), playerData.room_id);
            return false;
        }
        
        gameRoom->InterruptEvent(playerData.obj_data.info);

        return true;
        break;

    default:
        (*log).Log(LOGLEVEL::ERROR, "[%s] execute() - DataBroken", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr));
        return 0;
        break;
    }
    return 1;
}

int TcpPlayerPacket::catchError(int sock, unsigned int errorCode)
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