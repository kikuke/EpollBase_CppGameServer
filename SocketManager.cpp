#include "SocketManager.h"

SocketManager SocketManager::m_Instance;

bool SocketManager::addTcpSocketInfo(int socket)
{
    std::pair<int, TCPSOCKETINFO*> inform = std::pair<int, TCPSOCKETINFO*>(socket, new TCPSOCKETINFO);
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