#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "SocketManager.h"
#include "Logger.h"
#include "TcpMessagePacket.h"

int TcpMessagePacket::execute(int sock, unsigned int subOp, RingBuffer& buffer)
{
    switch (subOp)
    {
    case MESSAGE_ECHO:
        MessageEchoData meData;
        if(DequeueData(meData, buffer) != TCP_PACKET_END_CODE){
            (*log).Log(LOGLEVEL::ERROR, "[%s] DequeueData() - DataBroken", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr));
            return 0;//에러코드로 바꿔주기
        }

        return MessageEcho(sock, meData);//에러나면 에러코드가 반환됨.
        break;
    default:
        (*log).Log(LOGLEVEL::ERROR, "[%s] execute() - DataBroken", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr));
        return 0;
        break;
    }
    return 1;
}

int TcpMessagePacket::catchError(int sock, unsigned int errorCode)
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

int TcpMessagePacket::MessageEcho(int sock, MessageEchoData meData)
{

    switch (meData.target)
    {
    case MESSAGE_ECHO_DATA_TARGET_SELF:
        (*log).Log(LOGLEVEL::INFO, "[%s] SELF ECHO MESSAGE: %s", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr), meData.msg);
        //임시 전송처리
        write(sock, meData.msg, strlen((const char*)meData.msg));
        break;
    case MESSAGE_ECHO_DATA_TARGET_ALL:
        (*log).Log(LOGLEVEL::INFO, "[%s] ALL ECHO MESSAGE: %s", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr), meData.msg);
        //아직 미구현이지만 임시로 SELF 에코 처리하기. 임시 전송처리
        write(sock, meData.msg, strlen((const char*)meData.msg));
        break;
    default:
        (*log).Log(LOGLEVEL::ERROR, "[%s] MessageEcho - Undefined Target: %d", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr), meData.target);
        return 0;
        break;
    }

    return 1;
}