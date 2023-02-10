#ifndef SOCKET_MANAGER
#define SOCKET_MANAGER

#include <map>

#include "Logger.h"
#include "socketinfo.h"

class SocketManager
{
private:
    SocketManager();
    static SocketManager m_Instance;

    Logger* log;
    std::map<int, TCPSOCKETINFO*> tcpInfoMap;
public:
    ~SocketManager();
    static SocketManager& getInstance()
    {
        return m_Instance;
    }
    
    bool addTcpSocketInfo(int socket);
    bool delTcpSocketInfo(int socket);

    TCPSOCKETINFO* getTcpSocketInfo(int socket);
};

#endif