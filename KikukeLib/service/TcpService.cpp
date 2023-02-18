#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#include "spepoll.h"
#include "SocketManager.h"
#include "TcpService.h"

TcpService::TcpService()
{
    log = new Logger("TcpServce");
}

TcpService::~TcpService()
{
    delete log;
}

bool TcpService::AcceptTcpSocket(int serv_sock, int epfd)
{
    int clnt_sock;
    struct sockaddr_in clnt_adr;
    socklen_t adr_sz;

    adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);

    (*log).Log(LOGLEVEL::INFO, "[%s] Connecting Server!", inet_ntoa(clnt_adr.sin_addr));

    AddETClntSock(epfd, clnt_sock);

    return SocketManager::getInstance().addTcpSocketInfo(clnt_sock);
}

bool TcpService::CloseTcpSocket(int clnt_sock, int epfd)
{
    (*log).Log(LOGLEVEL::INFO, "[%s] Disconnecting Server!", inet_ntoa(SocketManager::getInstance().getTcpSocketInfo(clnt_sock)->sockAddr.sin_addr));

    if(!SocketManager::getInstance().delTcpSocketInfo(clnt_sock)){
        perror("CloseTcpSocket delTcpSocketInfo Error");
        return false;
    }

    epoll_ctl(epfd, EPOLL_CTL_DEL, clnt_sock, NULL);
    close(clnt_sock);
    return true;
}

void TcpService::Networking(int serv_sock, int event_sock, int epfd, JobQueue* jobQueue)
{
    if (event_sock == serv_sock)//Todo: 얘를 함수로 추가 분리하기 시멘틱 프로그래밍
    {
        if(!AcceptTcpSocket(event_sock, epfd)){
            (*log).Log(LOGLEVEL::ERROR, "AcceptTcpSocket Failed!");
        }
        (*log).Log(LOGLEVEL::DEBUG, "AcceptTcpSocket()");

        return;
    }

    jobQueue->readQueue.push(event_sock);
    (*log).Log(LOGLEVEL::DEBUG, "Push ReadQueue: %d", event_sock);
    return;
}