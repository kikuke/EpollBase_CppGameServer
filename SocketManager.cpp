#include "SocketManager.h"

SocketManager SocketManager::m_Instance;

bool SocketManager::addTcpSocketInfo(int socket)
{
    //나중에 inform 세팅 관련 함수 만들기
    TCPSOCKETINFO* info_ptr = new TCPSOCKETINFO;
    info_ptr->socket = socket;
    
    std::pair<int, TCPSOCKETINFO*> inform = std::pair<int, TCPSOCKETINFO*>(socket, info_ptr);

    return tcpInfoMap.insert(inform).second;
}

bool SocketManager::delTcpSocketInfo(int socket)
{
    std::map<int, TCPSOCKETINFO*>::iterator iter = tcpInfoMap.find(socket);
    if(iter == tcpInfoMap.end())
        return false;

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