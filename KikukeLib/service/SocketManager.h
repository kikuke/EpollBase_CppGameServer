#ifndef SOCKET_MANAGER
#define SOCKET_MANAGER

#include <map>

#include "Logger.h"
#include "ServerInfo.h"

class SocketManager//Todo: 시간나면 종속적이지 않게 generic으로 수정하기
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

    TCPSOCKETINFO* getTcpSocketInfo(int socket);//Todo: 참조를 최대한 줄이기
};

#endif