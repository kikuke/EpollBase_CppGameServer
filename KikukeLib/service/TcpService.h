#ifndef TCP_SERVICE
#define TCP_SERVICE

#include "ServerInfo.h"
#include "Logger.h"

class TcpService
{
private:
    int epfd;
    int serv_sock;
    JobQueue* jobQueue;

    Logger* log;

public:
    TcpService(int epfd, int serv_sock, JobQueue* jobQueue);
    ~TcpService();

    bool AcceptTcpSocket();
    void Networking(int serv_sock);
};

#endif