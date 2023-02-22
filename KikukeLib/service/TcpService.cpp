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

bool TcpService::AcceptTcpSocket(int serv_sock, int epfd)//Todo: Handler로 기능 옮기기
{
    int clnt_sock;
    struct sockaddr_in clnt_adr;
    socklen_t adr_sz;

    adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);

    (*log).Log(LOGLEVEL::INFO, "[%s] Connecting Server!", inet_ntoa(clnt_adr.sin_addr));

    AddETClntSock(epfd, clnt_sock);
    if(!SocketManager::getInstance().addTcpSocketInfo(clnt_sock)){
        (*log).Log(LOGLEVEL::ERROR, "[%s] Failed AddTcpSocketInfo!", inet_ntoa(clnt_adr.sin_addr));

        epoll_ctl(epfd, EPOLL_CTL_DEL, clnt_sock, NULL);
        close(clnt_sock);

        return false;
    }
    
    return true;
}

void TcpService::Networking(int serv_sock, int event_sock, int epfd, JobQueue* jobQueue)
{
    if (event_sock == serv_sock)
    {
        if(!AcceptTcpSocket(event_sock, epfd)){//Todo: 안되는 이유 찾기
            (*log).Log(LOGLEVEL::ERROR, "%d AcceptTcpSocket Failed!", event_sock);
            return;
        }

        (*log).Log(LOGLEVEL::DEBUG, "AcceptTcpSocket()");

        return;
    }

    jobQueue->readQueue.push(event_sock);
    (*log).Log(LOGLEVEL::DEBUG, "Push ReadQueue: %d", event_sock);
    return;
}