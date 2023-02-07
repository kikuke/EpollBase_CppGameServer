#include "PacketDefine.h"
#include "SocketManager.h"
#include "PacketHandler.h"

int PacketHandler::execute(int sock)
{
    TCPSOCKETINFO* info = SocketManager::getInstance().getTcpSocketInfo(sock);
    TestPacketHeader header;

    int useSz = info->recvBuffer.getUseSize();

    if(sizeof(TestPacketHeader) > useSz)
        return 0;
    info->recvBuffer.peek(&header, sizeof(header));

    if(header.packetLen > useSz)
        return 0;
    
    

    return 1;
}