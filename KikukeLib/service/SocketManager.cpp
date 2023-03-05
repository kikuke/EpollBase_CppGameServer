#include <sys/socket.h>
#include <arpa/inet.h>

#include "ThreadSafe/TSRingBuffer.h"
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

bool SocketManager::addTcpSocketInfo(int socket)//Todo: 재사용 방식으로 바꾸기 ip기준으로 체크
{
    //나중에 inform 세팅 관련 함수 만들기
    TCPSOCKETINFO* info_ptr = new TCPSOCKETINFO;
    socklen_t sockLen = sizeof(info_ptr->sockAddr);
    
    info_ptr->recvBuffer = new TSRingBuffer();
    info_ptr->socket = socket;
    getpeername(socket, (struct sockaddr *)&(info_ptr->sockAddr), &sockLen);
    
    std::pair<int, TCPSOCKETINFO*> inform = std::pair<int, TCPSOCKETINFO*>(socket, info_ptr);
    if(!tcpInfoMap.insert(inform).second){
        delete info_ptr;
        
        return false;
    }

    (*log).Log(LOGLEVEL::INFO, "[%s] AddTcpInfo - Socket: %d", inet_ntoa(info_ptr->sockAddr.sin_addr), info_ptr->socket);

    return true;
}

bool SocketManager::delTcpSocketInfo(int socket)//Todo: 재사용 방식으로 바꾸기 ip기준으로 체크
{
    std::map<int, TCPSOCKETINFO*>::iterator iter = tcpInfoMap.find(socket);
    if(iter == tcpInfoMap.end()){
        (*log).Log(LOGLEVEL::ERROR, "[%s] DeleteTcpInfo Failed - Socket: %d", inet_ntoa(iter->second->sockAddr.sin_addr), iter->second->socket);
        return false;
    }

    delete iter->second->recvBuffer;
    (*log).Log(LOGLEVEL::INFO, "[%s] DeleteTcpInfo - Socket: %d", inet_ntoa(iter->second->sockAddr.sin_addr), iter->second->socket);

    delete iter->second;
    tcpInfoMap.erase(iter);
    return true;
}

//Todo: deleteId도 만들기
bool SocketManager::addID(int sock, int id)
{
    std::pair<int, int> data = std::pair<int, int>(id, sock);
    if(!id_socketMap.insert(data).second){
        return false;
    }
    getTcpSocketInfo(sock)->id = id;
    
    (*log).Log(LOGLEVEL::INFO, "AddID - Socket: %d, ID: %d", sock, id);

    return true;
}

int SocketManager::getSocketById(int id)
{
    std::map<int, int>::iterator iter = id_socketMap.find(id);
    if(iter == id_socketMap.end())
        return -1;

    return iter->second;
}

TCPSOCKETINFO* SocketManager::getTcpSocketInfo(int socket)//Todo: 재사용 방식으로 바꾸기 ip기준으로 체크
{
    std::map<int, TCPSOCKETINFO*>::iterator iter = tcpInfoMap.find(socket);
    if(iter == tcpInfoMap.end())
        return NULL;

    return iter->second;
}