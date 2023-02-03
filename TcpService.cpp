#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#include "spepoll.h"
#include "SocketManager.h"
#include "TcpService.h"

bool AcceptTcpSocket(int serv_sock, int epfd)
{
    int clnt_sock;
    struct sockaddr_in clnt_adr;
    socklen_t adr_sz;

    adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
    AddETClntSock(epfd, clnt_sock);

    return SocketManager::getInstance().addTcpSocketInfo(clnt_sock);
}

bool CloseTcpSocket(int clnt_sock, int epfd)
{
    if(!SocketManager::getInstance().delTcpSocketInfo(clnt_sock))
        return false;

    epoll_ctl(epfd, EPOLL_CTL_DEL, clnt_sock, NULL);
    close(clnt_sock);
    return true;
}