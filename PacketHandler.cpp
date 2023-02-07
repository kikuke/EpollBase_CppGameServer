#include "PacketDefine.h"
#include "SocketManager.h"
#include "PacketHandler.h"

int PacketHandler::execute(int sock)
{
    TCPSOCKETINFO* info = SocketManager::getInstance().getTcpSocketInfo(sock);
    TCPTestPacketHeader header;

    int useSz = info->recvBuffer.getUseSize();

    if(sizeof(TCPTestPacketHeader) > useSz)
        return 0;
    info->recvBuffer.peek(&header, sizeof(header));

    if(header.packetLen > useSz)
        return 0;
    
    /*
    각 응답코드별 응답 구조체 또는 클래스를 만들어서 그대로 빼내기.
    응답 클래스에는 응답 구조체도 들어있음 응답 구조체를 이용해 한번에 빼내기.
    응답 구조체는 get()같은 상위 클래스의 메서드를 이용해 빼낼 수 있음.
    상위 클래스에 run 같은 느낌으로 실행되는 것들도 만들기
    */

    return 1;
}