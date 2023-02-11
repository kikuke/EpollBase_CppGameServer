#include <sys/socket.h>
#include <arpa/inet.h>

#include "SocketManager.h"

SocketManager* SocketManager::m_Instance;

SocketManager::SocketManager()
{
    log = new Logger("SocketManager");
}

SocketManager::~SocketManager()
{
    delete log;
}

bool SocketManager::addTcpSocketInfo(int socket)
{
    //나중에 inform 세팅 관련 함수 만들기
    TCPSOCKETINFO* info_ptr = new TCPSOCKETINFO;
    socklen_t sockLen = sizeof(info_ptr->sockAddr);
    
    info_ptr->socket = socket;
    getpeername(socket, (struct sockaddr *)&(info_ptr->sockAddr), &sockLen);
    
    std::pair<int, TCPSOCKETINFO*> inform = std::pair<int, TCPSOCKETINFO*>(socket, info_ptr);

    (*log).Log(LOGLEVEL::INFO, "[%s] AddTcpInfo - Socket: %d", inet_ntoa(info_ptr->sockAddr.sin_addr), info_ptr->socket);

    return tcpInfoMap.insert(inform).second;
}

bool SocketManager::delTcpSocketInfo(int socket)
{
    std::map<int, TCPSOCKETINFO*>::iterator iter = tcpInfoMap.find(socket);
    if(iter == tcpInfoMap.end()){
        (*log).Log(LOGLEVEL::ERROR, "[%s] DeleteTcpInfo Failed - Socket: %d", inet_ntoa(iter->second->sockAddr.sin_addr), iter->second->socket);
        return false;
    }

    (*log).Log(LOGLEVEL::INFO, "[%s] DeleteTcpInfo - Socket: %d", inet_ntoa(iter->second->sockAddr.sin_addr), iter->second->socket);

    delete iter->second;
    tcpInfoMap.erase(iter);
    return true;
}

TCPSOCKETINFO* SocketManager::getTcpSocketInfo(int socket)
{
    std::map<int, TCPSOCKETINFO*>::iterator iter = tcpInfoMap.find(socket);
    if(iter == tcpInfoMap.end())
        return NULL;

    return iter->second;
}