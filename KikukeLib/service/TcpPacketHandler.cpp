#include "SocketManager.h"
#include "TcpPacketHandler.h"

int TcpPacketHandler::ExecuteOP(int sock, unsigned int mainOp, unsigned int subOp, RingBuffer& buffer)
{
    for(int i=0; i<handler_size; i++){
        if((*(handlers[i])).getMainOp() == mainOp)
            return (*(handlers[i])).execute(sock, subOp, buffer);
    }

    (*log).Log(LOGLEVEL::ERROR, "ExecuteOp() - Undefined MainOp: %d", mainOp);
   return 0;
}

int TcpPacketHandler::CatchError(int sock, unsigned int mainOp, unsigned int errorCode)
{
    for(int i=0; i<handler_size; i++){
        if((*(handlers[i])).getMainOp() == mainOp)
            return (*(handlers[i])).catchError(sock, errorCode);
    }

    (*log).Log(LOGLEVEL::ERROR, "CatchError() - Undefined MainOp: %d", mainOp);
    return 0;
}

int TcpPacketHandler::execute(int sock)
{
    TCPSOCKETINFO* info = SocketManager::getInstance().getTcpSocketInfo(sock);
    TCPTestPacketHeader header;
    int ret;

    if(info == nullptr){
        (*log).Log(LOGLEVEL::ERROR, "TcpPacketHandler - %d TcpSocketInfo is null", sock);
        return 0;
    }
    
    size_t useSz = (*(info->recvBuffer)).getUseSize();

    if(sizeof(TCPTestPacketHeader) > useSz)
        return 0;

    (*(info->recvBuffer)).peek(&header, sizeof(header));

    ret = TCPHeaderCheck(&header, useSz);
    if(ret != 1){
        (*log).Log(LOGLEVEL::ERROR, "TCPHeaderChcek()");
        return 0;
    }
    
    (*(info->recvBuffer)) >> header;

    ret = ExecuteOP(sock, header.mainOp, header.subOp, *(info->recvBuffer));
    if(ret != 1){
        (*log).Log(LOGLEVEL::ERROR, "ExecuteOP()");
        //해당 소켓의 버퍼는 비게됨.
        //해당 오류에 대한 해당 기능의 에러 처리
        CatchError(sock, header.mainOp, ret);
        return 0;
    }

    return 1;
}

int TcpPacketHandler::TCPHeaderCheck(TCPTestPacketHeader* header, size_t useSz)
{
    //인증, 인가, 플래그 체크 등등의 작업하기
    if(!(header->startCode == TCP_PACKET_START_CODE)){
        (*log).Log(LOGLEVEL::ERROR, "TCPHeaderCheck() - Undefined StartCode");
        return 0;
    }

    if(header->packetLen > useSz){
        (*log).Log(LOGLEVEL::DEBUG, "TCPHeaderCheck() - packetLen: %d, useSiz: %d", header->packetLen, useSz);
        return 0;
    }

    return 1;
}