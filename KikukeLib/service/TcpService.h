#ifndef TCP_SERVICE
#define TCP_SERVICE

#include "ServerInfo.h"
#include "Logger.h"

class TcpService
{
private:
    Logger* log;
public:
    TcpService();
    ~TcpService();

    bool AcceptTcpSocket(int serv_sock, int epfd);
    void Networking(int serv_sock, int event_sock, int epfd, JobQueue* jobQueue);
};

#endif