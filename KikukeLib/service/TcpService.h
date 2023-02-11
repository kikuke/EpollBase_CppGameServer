#ifndef TCP_SERVICE
#define TCP_SERVICE

#include "../utility/Logger.h"

class TcpService
{
private:
    Logger* log;
public:
    TcpService();
    ~TcpService();

    bool AcceptTcpSocket(int serv_sock, int epfd);
    bool CloseTcpSocket(int clnt_sock, int epfd);
};

#endif