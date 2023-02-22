#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>

#include "SocketManager.h"
#include "TcpDisconnectPacket.h"

int TcpDisconnectPacket::execute(int sock, unsigned int subOp, RingBuffer& buffer)
{
    DisconnectData data;//지금은 case 전체가 사용할것처럼 밖으로 꺼내놨지만 subOp마다 다른 데이터들을 사용예정
    switch (subOp)
    {
    case DISCONNECT_OK:
        if(DequeueData(data, buffer) != TCP_PACKET_END_CODE){
            (*log).Log(LOGLEVEL::ERROR, "[%s] DequeueData() - DataBroken", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr));
            return 0;//Todo: 에러코드로 바꿔주기
        }

        return Disconnect(sock, data);//에러나면 에러코드가 반환됨.
        break;
    case DISCONNECT_ERROR://클라이언트의 비정상 종료 처리
        if(DequeueData(data, buffer) != TCP_PACKET_END_CODE){
            (*log).Log(LOGLEVEL::ERROR, "[%s] DequeueData() - DataBroken", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr));
            return 0;//Todo: 에러코드로 바꿔주기
        }

        //Todo: 비정상 종료가 생길경우 로직 짜주기
        (*log).Log(LOGLEVEL::INFO, "[%s] Bad Disconnect!", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr));
        return 1;
        break;
    default:
        (*log).Log(LOGLEVEL::ERROR, "[%s] execute() - DataBroken", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr));
        return 0;
        break;
    }
    return 1;
}

int TcpDisconnectPacket::catchError(int sock, unsigned int errorCode)
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

int TcpDisconnectPacket::Disconnect(int sock, DisconnectData data)
{
    //Comment: 아직은 단순한 종료 로직.
    //Todo: 지연처리 하거나 데이터 남겨두기. 다른 스레드에서 아직 데이터가 남는 경우가 있을수가 있어서.
    (*log).Log(LOGLEVEL::INFO, "[%s] Disconnecting Server!", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(sock)->sockAddr.sin_addr));

    if(!SocketManager::getInstance().delTcpSocketInfo(sock)){
        (*log).Log(LOGLEVEL::ERROR, "DelTcpSocketInfo - %s", strerror(errno));
        return 0;
    }

    epoll_ctl(epfd, EPOLL_CTL_DEL, sock, NULL);
    close(sock);
    return 1;
}