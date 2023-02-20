#ifndef SOCKET_MANAGER
#define SOCKET_MANAGER

#include <map>
#include <mutex>

#include "Logger.h"
#include "ServerInfo.h"

class SocketManager
{
private:
    SocketManager();
    static SocketManager* m_Instance;

    std::map<int, TCPSOCKETINFO*> tcpInfoMap;
    
    Logger* log;
public:
    ~SocketManager();
    static SocketManager& getInstance()
    {
        if(m_Instance == nullptr){
            m_Instance = new SocketManager();
        }

        return *m_Instance;
    }
    
    bool addTcpSocketInfo(int socket);
    bool delTcpSocketInfo(int socket);

    TCPSOCKETINFO* getTcpSocketInfo(int socket);
};

#endif