#ifndef SOCKET_MANAGER
#define SOCKET_MANAGER

#include <map>

#include "socketinfo.h"

class SocketManager
{
private:
    SocketManager(){}
    static SocketManager m_Instance;

    std::map<int, TCPSOCKETINFO*> tcpInfoMap;
public:
    ~SocketManager(){}
    static SocketManager& getInstance()
    {
        return m_Instance;
    }

    //소켓인포 생성/삭제 관련 함수 만들기
    TCPSOCKETINFO* getTcpSocketInfo(int socket);
};

#endif